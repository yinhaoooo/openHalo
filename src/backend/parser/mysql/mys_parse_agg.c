/*-------------------------------------------------------------------------
 *
 * mys_parse_agg.c
 *	  MySQL handle aggregates and window functions in parser
 *
 * 
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 * 
 * 易景科技是Halo Database、Halo Database Management System、羲和数据
 * 库、羲和数据库管理系统（后面简称 Halo ）、openHalo软件的发明人同时也为
 * 知识产权权利人。Halo 软件的知识产权，以及与本软件相关的所有信息内容（包括
 * 但不限于文字、图片、音频、视频、图表、界面设计、版面框架、有关数据或电子文
 * 档等）均受中华人民共和国法律法规和相应的国际条约保护，易景科技享有上述知识
 * 产权，但相关权利人依照法律规定应享有的权利除外。未免疑义，本条所指的“知识
 * 产权”是指任何及所有基于 Halo 软件产生的：（a）版权、商标、商号、域名、与
 * 商标和商号相关的商誉、设计和专利；与创新、技术诀窍、商业秘密、保密技术、非
 * 技术信息相关的权利；（b）人身权、掩模作品权、署名权和发表权；以及（c）在
 * 本协议生效之前已存在或此后出现在世界任何地方的其他工业产权、专有权、与“知
 * 识产权”相关的权利，以及上述权利的所有续期和延长，无论此类权利是否已在相
 * 关法域内的相关机构注册。
 *
 * This software and related documentation are provided under a license
 * agreement containing restrictions on use and disclosure and are 
 * protected by intellectual property laws. Except as expressly permitted
 * in your license agreement or allowed by law, you may not use, copy, 
 * reproduce, translate, broadcast, modify, license, transmit, distribute,
 * exhibit, perform, publish, or display any part, in any form, or by any
 * means. Reverse engineering, disassembly, or decompilation of this 
 * software, unless required by law for interoperability, is prohibited.
 * 
 * This software is developed for general use in a variety of
 * information management applications. It is not developed or intended
 * for use in any inherently dangerous applications, including applications
 * that may create a risk of personal injury. If you use this software or
 * in dangerous applications, then you shall be responsible to take all
 * appropriate fail-safe, backup, redundancy, and other measures to ensure
 * its safe use. Halo Corporation and its affiliates disclaim any 
 * liability for any damages caused by use of this software in dangerous
 * applications.
 * 
 *
 * IDENTIFICATION
 *	  src/backend/parser/mysql/mys_parse_agg.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/relscan.h"
#include "access/tableam.h"
#include "catalog/namespace.h"
#include "catalog/pg_aggregate.h"
#include "catalog/pg_constraint.h"
#include "catalog/pg_type.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_proc_d.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "optimizer/optimizer.h"
#include "parser/parse_agg.h"
#include "parser/parse_clause.h"
#include "parser/parse_coerce.h"
#include "parser/parse_expr.h"
#include "parser/parsetree.h"
#include "parser/mysql/mys_parse_agg.h"
#include "rewrite/rewriteManip.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "nodes/nodes.h"


typedef struct
{
	ParseState *pstate;
	int			min_varlevel;
	int			min_agglevel;
	int			sublevels_up;
} check_agg_arguments_context;

typedef struct
{
	ParseState *pstate;
	Query	   *qry;
	bool		hasJoinRTEs;
	List	   *groupClauses;
	List	   *groupClauseCommonVars;
	bool		have_non_var_grouping;
	List	  **func_grouped_rels;
	int			sublevels_up;
	bool		in_agg_direct_args;
} check_ungrouped_columns_context;

static Oid dummy_min_for_bytea = InvalidOid;

static void check_ungrouped_columns(Node *node, ParseState *pstate, Query *qry,
									List *groupClauses, List *groupClauseCommonVars,
									bool have_non_var_grouping,
									List **func_grouped_rels);
static bool check_ungrouped_columns_walker(Node *node,
										   check_ungrouped_columns_context *context);
static void finalize_grouping_exprs(Node *node, ParseState *pstate, Query *qry,
									List *groupClauses, bool hasJoinRTEs,
									bool have_non_var_grouping);
//static bool check_agg_arguments_walker(Node *node, check_agg_arguments_context *context);
static bool finalize_grouping_exprs_walker(Node *node,
										   check_ungrouped_columns_context *context);
static Oid read_dummy_min_for_bytea_oid(void);

/*
 * parseCheckAggregates
 *	Check for aggregates where they shouldn't be and improper grouping.
 *	This function should be called after the target list and qualifications
 *	are finalized.
 *
 *	Misplaced aggregates are now mostly detected in transformAggregateCall,
 *	but it seems more robust to check for aggregates in recursive queries
 *	only after everything is finalized.  In any case it's hard to detect
 *	improper grouping on-the-fly, so we have to make another pass over the
 *	query for that.
 */
void
mys_parseCheckAggregates(ParseState *pstate, Query *qry)
{
	List	   *gset_common = NIL;
	List	   *groupClauses = NIL;
	List	   *groupClauseCommonVars = NIL;
	bool		have_non_var_grouping;
	List	   *func_grouped_rels = NIL;
	ListCell   *l;
	bool		hasJoinRTEs;
	bool		hasSelfRefRTEs;
	Node	   *clause;

	/* This should only be called if we found aggregates or grouping */
	Assert(pstate->p_hasAggs || qry->groupClause || qry->havingQual || qry->groupingSets);

	/*
	 * If we have grouping sets, expand them and find the intersection of all
	 * sets.
	 */
	if (qry->groupingSets)
	{
		/*
		 * The limit of 4096 is arbitrary and exists simply to avoid resource
		 * issues from pathological constructs.
		 */
		List	   *gsets = expand_grouping_sets(qry->groupingSets, qry->groupDistinct, 4096);

		if (!gsets)
			ereport(ERROR,
					(errcode(ERRCODE_STATEMENT_TOO_COMPLEX),
					 errmsg("too many grouping sets present (maximum 4096)"),
					 parser_errposition(pstate,
										qry->groupClause
										? exprLocation((Node *) qry->groupClause)
										: exprLocation((Node *) qry->groupingSets))));

		/*
		 * The intersection will often be empty, so help things along by
		 * seeding the intersect with the smallest set.
		 */
		gset_common = linitial(gsets);

		if (gset_common)
		{
			for_each_from(l, gsets, 1)
			{
				gset_common = list_intersection_int(gset_common, lfirst(l));
				if (!gset_common)
					break;
			}
		}

		/*
		 * If there was only one grouping set in the expansion, AND if the
		 * groupClause is non-empty (meaning that the grouping set is not
		 * empty either), then we can ditch the grouping set and pretend we
		 * just had a normal GROUP BY.
		 */
		if (list_length(gsets) == 1 && qry->groupClause)
			qry->groupingSets = NIL;
	}

	/*
	 * Scan the range table to see if there are JOIN or self-reference CTE
	 * entries.  We'll need this info below.
	 */
	hasJoinRTEs = hasSelfRefRTEs = false;
	foreach(l, pstate->p_rtable)
	{
		RangeTblEntry *rte = (RangeTblEntry *) lfirst(l);

		if (rte->rtekind == RTE_JOIN)
			hasJoinRTEs = true;
		else if (rte->rtekind == RTE_CTE && rte->self_reference)
			hasSelfRefRTEs = true;
	}

	/*
	 * Build a list of the acceptable GROUP BY expressions for use by
	 * check_ungrouped_columns().
	 *
	 * We get the TLE, not just the expr, because GROUPING wants to know the
	 * sortgroupref.
	 */
	foreach(l, qry->groupClause)
	{
		SortGroupClause *grpcl = (SortGroupClause *) lfirst(l);
		TargetEntry *expr;

		expr = get_sortgroupclause_tle(grpcl, qry->targetList);
		if (expr == NULL)
			continue;			/* probably cannot happen */

		groupClauses = lappend(groupClauses, expr);
	}

	/*
	 * If there are join alias vars involved, we have to flatten them to the
	 * underlying vars, so that aliased and unaliased vars will be correctly
	 * taken as equal.  We can skip the expense of doing this if no rangetable
	 * entries are RTE_JOIN kind.
	 */
	if (hasJoinRTEs)
		groupClauses = (List *) flatten_join_alias_vars(qry,
														(Node *) groupClauses);

	/*
	 * Detect whether any of the grouping expressions aren't simple Vars; if
	 * they're all Vars then we don't have to work so hard in the recursive
	 * scans.  (Note we have to flatten aliases before this.)
	 *
	 * Track Vars that are included in all grouping sets separately in
	 * groupClauseCommonVars, since these are the only ones we can use to
	 * check for functional dependencies.
	 */
	have_non_var_grouping = false;
	foreach(l, groupClauses)
	{
		TargetEntry *tle = lfirst(l);

		if (!IsA(tle->expr, Var))
		{
			have_non_var_grouping = true;
		}
		else if (!qry->groupingSets ||
				 list_member_int(gset_common, tle->ressortgroupref))
		{
			groupClauseCommonVars = lappend(groupClauseCommonVars, tle->expr);
		}
	}

	/*
	 * Check the targetlist and HAVING clause for ungrouped variables.
	 *
	 * Note: because we check resjunk tlist elements as well as regular ones,
	 * this will also find ungrouped variables that came from ORDER BY and
	 * WINDOW clauses.  For that matter, it's also going to examine the
	 * grouping expressions themselves --- but they'll all pass the test ...
	 *
	 * We also finalize GROUPING expressions, but for that we need to traverse
	 * the original (unflattened) clause in order to modify nodes.
	 */
	clause = (Node *) qry->targetList;
	finalize_grouping_exprs(clause, pstate, qry,
							groupClauses, hasJoinRTEs,
							have_non_var_grouping);
	if (hasJoinRTEs)
		clause = flatten_join_alias_vars(qry, clause);
	check_ungrouped_columns(clause, pstate, qry,
							groupClauses, groupClauseCommonVars,
							have_non_var_grouping,
							&func_grouped_rels);

	clause = (Node *) qry->havingQual;
	finalize_grouping_exprs(clause, pstate, qry,
							groupClauses, hasJoinRTEs,
							have_non_var_grouping);
	if (hasJoinRTEs)
		clause = flatten_join_alias_vars(qry, clause);
	check_ungrouped_columns(clause, pstate, qry,
							groupClauses, groupClauseCommonVars,
							have_non_var_grouping,
							&func_grouped_rels);

	/*
	 * Per spec, aggregates can't appear in a recursive term.
	 */
	if (pstate->p_hasAggs && hasSelfRefRTEs)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_RECURSION),
				 errmsg("aggregate functions are not allowed in a recursive query's recursive term"),
				 parser_errposition(pstate,
									locate_agg_of_level((Node *) qry, 0))));
}

/*
 * check_ungrouped_columns -
 *	  Scan the given expression tree for ungrouped variables (variables
 *	  that are not listed in the groupClauses list and are not within
 *	  the arguments of aggregate functions).  Emit a suitable error message
 *	  if any are found.
 *
 * NOTE: we assume that the given clause has been transformed suitably for
 * parser output.  This means we can use expression_tree_walker.
 *
 * NOTE: we recognize grouping expressions in the main query, but only
 * grouping Vars in subqueries.  For example, this will be rejected,
 * although it could be allowed:
 *		SELECT
 *			(SELECT x FROM bar where y = (foo.a + foo.b))
 *		FROM foo
 *		GROUP BY a + b;
 * The difficulty is the need to account for different sublevels_up.
 * This appears to require a whole custom version of equal(), which is
 * way more pain than the feature seems worth.
 */
static void
check_ungrouped_columns(Node *node, ParseState *pstate, Query *qry,
						List *groupClauses, List *groupClauseCommonVars,
						bool have_non_var_grouping,
						List **func_grouped_rels)
{
	check_ungrouped_columns_context context;

	context.pstate = pstate;
	context.qry = qry;
	context.hasJoinRTEs = false;	/* assume caller flattened join Vars */
	context.groupClauses = groupClauses;
	context.groupClauseCommonVars = groupClauseCommonVars;
	context.have_non_var_grouping = have_non_var_grouping;
	context.func_grouped_rels = func_grouped_rels;
	context.sublevels_up = 0;
	context.in_agg_direct_args = false;
	check_ungrouped_columns_walker(node, &context);
}

static bool
check_ungrouped_columns_walker(Node *node,
							   check_ungrouped_columns_context *context)
{
	ListCell   *gl;

	if (node == NULL)
		return false;
	if (IsA(node, Const) ||
		IsA(node, Param))
		return false;			/* constants are always acceptable */

	if (IsA(node, Aggref))
	{
		Aggref	   *agg = (Aggref *) node;

		if ((int) agg->agglevelsup == context->sublevels_up)
		{
			/*
			 * If we find an aggregate call of the original level, do not
			 * recurse into its normal arguments, ORDER BY arguments, or
			 * filter; ungrouped vars there are not an error.  But we should
			 * check direct arguments as though they weren't in an aggregate.
			 * We set a special flag in the context to help produce a useful
			 * error message for ungrouped vars in direct arguments.
			 */
			bool		result;

			Assert(!context->in_agg_direct_args);
			context->in_agg_direct_args = true;
			result = check_ungrouped_columns_walker((Node *) agg->aggdirectargs,
													context);
			context->in_agg_direct_args = false;
			return result;
		}

		/*
		 * We can skip recursing into aggregates of higher levels altogether,
		 * since they could not possibly contain Vars of concern to us (see
		 * transformAggregateCall).  We do need to look at aggregates of lower
		 * levels, however.
		 */
		if ((int) agg->agglevelsup > context->sublevels_up)
			return false;
	}

	if (IsA(node, GroupingFunc))
	{
		GroupingFunc *grp = (GroupingFunc *) node;

		/* handled GroupingFunc separately, no need to recheck at this level */

		if ((int) grp->agglevelsup >= context->sublevels_up)
			return false;
	}

	/*
	 * If we have any GROUP BY items that are not simple Vars, check to see if
	 * subexpression as a whole matches any GROUP BY item. We need to do this
	 * at every recursion level so that we recognize GROUPed-BY expressions
	 * before reaching variables within them. But this only works at the outer
	 * query level, as noted above.
	 */
	if (context->have_non_var_grouping && context->sublevels_up == 0)
	{
		foreach(gl, context->groupClauses)
		{
			TargetEntry *tle = lfirst(gl);

			if (equal(node, tle->expr))
				return false;	/* acceptable, do not descend more */
		}
	}

	/*
	 * If we have an ungrouped Var of the original query level, we have a
	 * failure.  Vars below the original query level are not a problem, and
	 * neither are Vars from above it.  (If such Vars are ungrouped as far as
	 * their own query level is concerned, that's someone else's problem...)
	 */
	if (IsA(node, Var))
	{
		Var		   *var = (Var *) node;
		RangeTblEntry *rte;
		Aggref   *newaggref;
		TargetEntry *tle;

        char* type_name;
        Form_pg_type pgType;
        HeapTuple tuple;

		if (var->varlevelsup != context->sublevels_up)
			return false;		/* it's not local to my query, ignore */

		/*
		 * Check for a match, if we didn't do it above.
		 */
		if (!context->have_non_var_grouping || context->sublevels_up != 0)
		{
			foreach(gl, context->groupClauses)
			{
				Var		   *gvar = (Var *) ((TargetEntry *) lfirst(gl))->expr;

				if (IsA(gvar, Var) &&
					gvar->varno == var->varno &&
					gvar->varattno == var->varattno &&
					gvar->varlevelsup == 0)
					return false;	/* acceptable, we're okay */
			}
		}

		/*
		 * Check whether the Var is known functionally dependent on the GROUP
		 * BY columns.  If so, we can allow the Var to be used, because the
		 * grouping is really a no-op for this table.  However, this deduction
		 * depends on one or more constraints of the table, so we have to add
		 * those constraints to the query's constraintDeps list, because it's
		 * not semantically valid anymore if the constraint(s) get dropped.
		 * (Therefore, this check must be the last-ditch effort before raising
		 * error: we don't want to add dependencies unnecessarily.)
		 *
		 * Because this is a pretty expensive check, and will have the same
		 * outcome for all columns of a table, we remember which RTEs we've
		 * already proven functional dependency for in the func_grouped_rels
		 * list.  This test also prevents us from adding duplicate entries to
		 * the constraintDeps list.
		 */
		if (list_member_int(*context->func_grouped_rels, var->varno))
			return false;		/* previously proven acceptable */

		Assert(var->varno > 0 &&
			   (int) var->varno <= list_length(context->pstate->p_rtable));
		rte = rt_fetch(var->varno, context->pstate->p_rtable);
		if (rte->rtekind == RTE_RELATION)
		{
			if (check_functional_grouping(rte->relid,
										  var->varno,
										  0,
										  context->groupClauseCommonVars,
										  &context->qry->constraintDeps))
			{
				*context->func_grouped_rels =
					lappend_int(*context->func_grouped_rels, var->varno);
				return false;	/* acceptable */
			}
		}

        newaggref = makeNode(Aggref);
        newaggref->aggtype = var->vartype;
        newaggref->aggcollid = 0;
        newaggref->inputcollid = 0;
        newaggref->aggtranstype = 0;
        // min
        /*
         * 根据type oid去查type名称，再根据type名称来确定aggfnoid
         * */
        tuple = SearchSysCache1(TYPEOID, ObjectIdGetDatum(newaggref->aggtype));
        if (HeapTupleIsValid(tuple))
        {
            pgType = (Form_pg_type)GETSTRUCT(tuple);
            type_name = pgType->typname.data;
            ReleaseSysCache(tuple);
        }
        else 
        {
            type_name = NULL;
        }
        if (type_name != NULL)
        {
            if (memcmp(type_name, "tinyint", 7) == 0)
            {
                newaggref->aggfnoid = 2133;     
            }
            else if (memcmp(type_name, "smallint unsigned", 17) == 0)
            {
                newaggref->aggfnoid = 2132;     
            }
            else if (memcmp(type_name, "smallint", 8) == 0)
            {
                newaggref->aggfnoid = 2133;     
            }
            else if (memcmp(type_name, "mediumint", 9) == 0)
            {
                newaggref->aggfnoid = 2132;     
            }
            else if (memcmp(type_name, "int unsigned", 12) == 0)
            {
                newaggref->aggfnoid = 2131;     
            }
			else if (memcmp(type_name, "datetime", 8) == 0)
			{
				newaggref->aggfnoid = 2142;
			}
            else if (memcmp(type_name, "enum_", 5) == 0)
            {
                newaggref->aggfnoid = 3527;
            }
            else if (memcmp(type_name, "set_", 4) == 0)
            {
                newaggref->aggfnoid = 2145;
                newaggref->aggcollid = 100;
                newaggref->inputcollid = 100;
            }
            else if (memcmp(type_name, "year_", 5) == 0)
            {
                newaggref->aggfnoid = 2132;     
            }
            else 
            {
                if (newaggref->aggtype == 23)           // int4
                {
                    newaggref->aggfnoid = 2132;     
                }
                else if (newaggref->aggtype == 20)      // int8
                {
                    newaggref->aggfnoid = 2131;     
                }
                else if (newaggref->aggtype == 21)      // int2
                {
                    newaggref->aggfnoid = 2133;     
                }
                else if (newaggref->aggtype == 1042)    // bpchar
                {
                    newaggref->aggfnoid = 2245;
                    newaggref->aggcollid = 100;
                    newaggref->inputcollid = 100;
                }
                else if (newaggref->aggtype == 1043)    // varchar
                {
                    newaggref->aggfnoid = 2145;
                    newaggref->aggcollid = 100;
                    newaggref->inputcollid = 100;
                }
                else if (newaggref->aggtype == 25)      // text
                {
                    newaggref->aggfnoid = 2145;
                    newaggref->aggcollid = 100;
                    newaggref->inputcollid = 100;
                }
                else if (newaggref->aggtype == 1114)    // timestamp
                {
                    newaggref->aggfnoid = 2142;     
                }
                else if (newaggref->aggtype == 1184)    // timestamptz
                {
                    newaggref->aggfnoid = 2143;
                }
                else if (newaggref->aggtype == 1082)    // date
                {
                    newaggref->aggfnoid = 2138;     
                }
                else if (newaggref->aggtype == 1083)    // time
                {
                    newaggref->aggfnoid = 2139;     
                }
                else if (newaggref->aggtype == 1700)    // numeric
                {
                    newaggref->aggfnoid = 2146;
                }
                else if (newaggref->aggtype == 17)      // bytea
                {
                    if (dummy_min_for_bytea == InvalidOid)
                    {
                        dummy_min_for_bytea = read_dummy_min_for_bytea_oid();
                    }

                    newaggref->aggfnoid = dummy_min_for_bytea;
                    //newaggref->aggcollid = 100;
                    //newaggref->inputcollid = 100;
                }
                else if (newaggref->aggtype == 26)      // oid
                {
                    newaggref->aggfnoid = 2134;     
                }
                else if (newaggref->aggtype == 27)      // tid
                {
                    newaggref->aggfnoid = 2798;
                }
                else if (newaggref->aggtype == 700)     // float4
                {
                    newaggref->aggfnoid = 2135;     
                }
                else if (newaggref->aggtype == 701)     // float8
                {
                    newaggref->aggfnoid = 2136;     
                }
                else if (newaggref->aggtype == 1266)    // timetz
                {
                    newaggref->aggfnoid = 2140;     
                }
                else if (newaggref->aggtype == 790)     // money
                {
                    newaggref->aggfnoid = 2141;     
                }
                else if (newaggref->aggtype == 1186)    // interval
                {
                    newaggref->aggfnoid = 2144;
                }
                else if (newaggref->aggtype == 2277)    // anyarray
                {
                    newaggref->aggfnoid = 2051;
                }
                else if (newaggref->aggtype == 869)     // inet
                {
                    newaggref->aggfnoid = 3565;
                }
                else if (newaggref->aggtype == 3220)    // pg_lsn
                {
                    newaggref->aggfnoid = 4190;
                }
                else if (newaggref->aggtype == 3500)    // anyenum
                {
                    newaggref->aggfnoid = 3527;
                }
                else 
                {
                    newaggref->aggfnoid = 2145;
                    newaggref->aggcollid = 100;
                    newaggref->inputcollid = 100;
                }
            }
        }
        else 
        {
            newaggref->aggfnoid = 2145;
            newaggref->aggcollid = 100;
            newaggref->inputcollid = 100;
        }
        newaggref->aggargtypes = lappend_oid(newaggref->aggargtypes, exprType(node));
        tle = makeTargetEntry((Expr*)var, 1, NULL, false);
        tle->resno = var->varattno;
        newaggref->args = lappend(newaggref->args, tle);
        newaggref->aggorder = NULL;
        newaggref->aggdistinct = NULL;
        newaggref->aggfilter = NULL;
        newaggref->aggstar = false;
        newaggref->aggvariadic = false;
        newaggref->aggkind = 'n';
        newaggref->agglevelsup = 0;
        newaggref->aggsplit = AGGSPLIT_SIMPLE;
        newaggref->aggno = -1;
        newaggref->aggtransno = -1;
        newaggref->location = 7;
        context->pstate->p_hasAggs = true;
        context->qry->hasAggs = true;
        foreach(gl, context->qry->targetList)
        {
            TargetEntry *roottle = (TargetEntry*)lfirst(gl);
            if ((void*)(roottle->expr) == (void*)node)
            {
                roottle->expr = (Expr*)newaggref;
                roottle->resorigtbl = 0;
                roottle->resorigcol = 0;
                break;
            }
            //else 
            //{
            //    Aggref *agg1 = (Aggref*)roottle->expr;
            //    ListCell   *lc;
            //    foreach(lc, agg1->args)
            //    {
            //        TargetEntry *tle1 = (TargetEntry*)lfirst(lc);
            //        Var* var1 = (Var*) tle1->expr;
            //        Var* var2 = (Node*) tle1->expr;
            //        //Var* var3 = castNode(Var, tle1->expr);
            //        break;
            //    }
            //}
        }
        //TargetEntry *roottle1 = castNode(TargetEntry, (&(*node) - sizeof(struct Expr)));

        return false;
    }

    if (IsA(node, Query))
    {
        /* Recurse into subselects */
        bool		result;

        context->sublevels_up++;
        result = query_tree_walker((Query *) node,
                                   check_ungrouped_columns_walker,
                                   (void *) context,
                                   0);
        context->sublevels_up--;
        return result;
    }
    return expression_tree_walker(node, check_ungrouped_columns_walker,
                                  (void *) context);
}

//static bool
//check_agg_arguments_walker(Node *node,
//                           check_agg_arguments_context *context)
//{
//    if (node == NULL)
//        return false;
//    if (IsA(node, Var))
//    {
//        int			varlevelsup = ((Var *) node)->varlevelsup;
//
//        /* convert levelsup to frame of reference of original query */
//        varlevelsup -= context->sublevels_up;
//        /* ignore local vars of subqueries */
//        if (varlevelsup >= 0)
//        {
//            if (context->min_varlevel < 0 ||
//                context->min_varlevel > varlevelsup)
//				context->min_varlevel = varlevelsup;
//		}
//		return false;
//	}
//	if (IsA(node, Aggref))
//	{
//		int			agglevelsup = ((Aggref *) node)->agglevelsup;
//
//		/* convert levelsup to frame of reference of original query */
//		agglevelsup -= context->sublevels_up;
//		/* ignore local aggs of subqueries */
//		if (agglevelsup >= 0)
//		{
//			if (context->min_agglevel < 0 ||
//				context->min_agglevel > agglevelsup)
//				context->min_agglevel = agglevelsup;
//		}
//		/* Continue and descend into subtree */
//	}
//	if (IsA(node, GroupingFunc))
//	{
//		int			agglevelsup = ((GroupingFunc *) node)->agglevelsup;
//
//		/* convert levelsup to frame of reference of original query */
//		agglevelsup -= context->sublevels_up;
//		/* ignore local aggs of subqueries */
//		if (agglevelsup >= 0)
//		{
//			if (context->min_agglevel < 0 ||
//				context->min_agglevel > agglevelsup)
//				context->min_agglevel = agglevelsup;
//		}
//		/* Continue and descend into subtree */
//	}
//
//	/*
//	 * SRFs and window functions can be rejected immediately, unless we are
//	 * within a sub-select within the aggregate's arguments; in that case
//	 * they're OK.
//	 */
//	if (context->sublevels_up == 0)
//	{
//		if ((IsA(node, FuncExpr) && ((FuncExpr *) node)->funcretset) ||
//			(IsA(node, OpExpr) && ((OpExpr *) node)->opretset))
//			ereport(ERROR,
//					(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
//					 errmsg("aggregate function calls cannot contain set-returning function calls"),
//					 errhint("You might be able to move the set-returning function into a LATERAL FROM item."),
//					 parser_errposition(context->pstate, exprLocation(node))));
//		if (IsA(node, WindowFunc))
//			ereport(ERROR,
//					(errcode(ERRCODE_GROUPING_ERROR),
//					 errmsg("aggregate function calls cannot contain window function calls"),
//					 parser_errposition(context->pstate,
//										((WindowFunc *) node)->location)));
//	}
//	if (IsA(node, Query))
//	{
//		/* Recurse into subselects */
//		bool		result;
//
//		context->sublevels_up++;
//		result = query_tree_walker((Query *) node,
//								   check_agg_arguments_walker,
//								   (void *) context,
//								   0);
//		context->sublevels_up--;
//		return result;
//	}
//
//	return expression_tree_walker(node,
//								  check_agg_arguments_walker,
//								  (void *) context);
//}

static void
finalize_grouping_exprs(Node *node, ParseState *pstate, Query *qry,
						List *groupClauses, bool hasJoinRTEs,
						bool have_non_var_grouping)
{
	check_ungrouped_columns_context context;

	context.pstate = pstate;
	context.qry = qry;
	context.hasJoinRTEs = hasJoinRTEs;
	context.groupClauses = groupClauses;
	context.groupClauseCommonVars = NIL;
	context.have_non_var_grouping = have_non_var_grouping;
	context.func_grouped_rels = NULL;
	context.sublevels_up = 0;
	context.in_agg_direct_args = false;
	finalize_grouping_exprs_walker(node, &context);
}

static bool
finalize_grouping_exprs_walker(Node *node,
							   check_ungrouped_columns_context *context)
{
    ListCell   *gl;

    if (node == NULL)
        return false;
    if (IsA(node, Const) ||
        IsA(node, Param))
        return false;			/* constants are always acceptable */

    if (IsA(node, Aggref))
    {
        Aggref	   *agg = (Aggref *) node;

        if ((int) agg->agglevelsup == context->sublevels_up)
        {
            /*
             * If we find an aggregate call of the original level, do not
             * recurse into its normal arguments, ORDER BY arguments, or
             * filter; GROUPING exprs of this level are not allowed there. But
             * check direct arguments as though they weren't in an aggregate.
             */
            bool		result;

            Assert(!context->in_agg_direct_args);
            context->in_agg_direct_args = true;
            result = finalize_grouping_exprs_walker((Node *) agg->aggdirectargs,
                                                    context);
            context->in_agg_direct_args = false;
            return result;
        }

        /*
         * We can skip recursing into aggregates of higher levels altogether,
         * since they could not possibly contain exprs of concern to us (see
         * transformAggregateCall).  We do need to look at aggregates of lower
         * levels, however.
         */
        if ((int) agg->agglevelsup > context->sublevels_up)
            return false;
    }

    if (IsA(node, GroupingFunc))
    {
        GroupingFunc *grp = (GroupingFunc *) node;

        /*
         * We only need to check GroupingFunc nodes at the exact level to
         * which they belong, since they cannot mix levels in arguments.
		 */

		if ((int) grp->agglevelsup == context->sublevels_up)
		{
			ListCell   *lc;
			List	   *ref_list = NIL;

			foreach(lc, grp->args)
			{
				Node	   *expr = lfirst(lc);
				Index		ref = 0;

				if (context->hasJoinRTEs)
					expr = flatten_join_alias_vars(context->qry, expr);

				/*
				 * Each expression must match a grouping entry at the current
				 * query level. Unlike the general expression case, we don't
				 * allow functional dependencies or outer references.
				 */

				if (IsA(expr, Var))
				{
					Var		   *var = (Var *) expr;

					if (var->varlevelsup == context->sublevels_up)
					{
						foreach(gl, context->groupClauses)
						{
							TargetEntry *tle = lfirst(gl);
							Var		   *gvar = (Var *) tle->expr;

							if (IsA(gvar, Var) &&
								gvar->varno == var->varno &&
								gvar->varattno == var->varattno &&
								gvar->varlevelsup == 0)
							{
								ref = tle->ressortgroupref;
								break;
							}
						}
					}
				}
				else if (context->have_non_var_grouping &&
						 context->sublevels_up == 0)
				{
					foreach(gl, context->groupClauses)
					{
						TargetEntry *tle = lfirst(gl);

						if (equal(expr, tle->expr))
						{
							ref = tle->ressortgroupref;
							break;
						}
					}
				}

				if (ref == 0)
					ereport(ERROR,
							(errcode(ERRCODE_GROUPING_ERROR),
							 errmsg("arguments to GROUPING must be grouping expressions of the associated query level"),
							 parser_errposition(context->pstate,
												exprLocation(expr))));

				ref_list = lappend_int(ref_list, ref);
			}

			grp->refs = ref_list;
		}

		if ((int) grp->agglevelsup > context->sublevels_up)
			return false;
	}

	if (IsA(node, Query))
	{
		/* Recurse into subselects */
		bool		result;

		context->sublevels_up++;
		result = query_tree_walker((Query *) node,
								   finalize_grouping_exprs_walker,
								   (void *) context,
								   0);
		context->sublevels_up--;
		return result;
	}
	return expression_tree_walker(node, finalize_grouping_exprs_walker,
								  (void *) context);
}

static Oid 
read_dummy_min_for_bytea_oid(void)
{
	Oid ret;
    FuncCandidateList clist;

	ret = InvalidOid;
    clist = FuncnameGetCandidates(list_make1(makeString("dummy_min_for_bytea")), -1, NIL, false, false, false, false);
    if (clist != NULL)
    {
        ret = clist->oid;
    }

    return ret;

	//Oid ret;
    //Relation    rel;
    //TableScanDesc scan;
    //HeapTuple   tup;

	//ret = InvalidOid;
    //rel = table_open(ProcedureRelationId, AccessShareLock);
    //scan = table_beginscan_catalog(rel, 0, NULL);
    //while ((tup = heap_getnext(scan, ForwardScanDirection)) != NULL)
    //{
    //    Form_pg_proc procform = (Form_pg_proc) GETSTRUCT(tup);
    //    if (strncpy(procform->proname.data, "dummy_min_for_bytea", 19) == 0)
    //    {
    //        ret = procform->oid;
    //        break;
    //    }
    //}
    //table_endscan(scan);
    //table_close(rel, AccessShareLock);

	//return ret;
}

