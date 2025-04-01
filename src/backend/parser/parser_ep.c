/*-------------------------------------------------------------------------
 *
 * parser_ep.c
 *		public entry point for parse analyze
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/parser/parser_ep.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "nodes/nodeFuncs.h"
#include "parser/analyze.h"
#include "parser/parse_agg.h"
#include "parser/parse_clause.h"
#include "parser/parse_collate.h"
#include "parser/parse_cte.h"
#include "parser/parse_expr.h"
#include "parser/parse_func.h"
#include "parser/parse_node.h"
#include "parser/parse_target.h"
#include "parser/parse_utilcmd.h"
#include "parser/parserapi.h"
#include "rewrite/rewriteDefine.h"
#include "rewrite/rewriteHandler.h"
#include "rewrite/rowsecurity.h"

/*
 * parse_analyze
 * parse_analyze的入口函数，如果引擎中parse_analyze函数未定义，则将调用
 * standard_parse_analyze
 */
Query *
parse_analyze(RawStmt *parseTree, const char *sourceText,
			  Oid *paramTypes, int numParams,
			  QueryEnvironment *queryEnv)
{
	Query	   *query;

	query = standard_parse_analyze(parseTree, sourceText, paramTypes, numParams, queryEnv);
	
	return query;
}

/*
 * parse_analyze_varparams
 * parse_analyze_varparams的入口函数
 */
Query *
parse_analyze_varparams(RawStmt *parseTree, const char *sourceText,
						Oid **paramTypes, int *numParams)
{
	Query	   *query;

	query = standard_parse_analyze_varparams(parseTree, sourceText, paramTypes, numParams);

	return query;
}

/*
 * transformStmt
 * transformStmt的入口函数
 */
Query *
transformStmt(ParseState *pstate, Node *parseTree)
{
	Query	   *query;

	Assert(parserengine != NULL);

	if (parserengine->transformStmt)
		query = parserengine->transformStmt(pstate, parseTree);
	else
		query = standard_transformStmt(pstate, parseTree);

	return query;
}

/*
 * transformAggregateCall
 * transformAggregateCall的入口函数
 */
void
transformAggregateCall(ParseState *pstate, Aggref *agg,
					   List *args, List *aggorder, bool agg_distinct)
{
	standard_transformAggregateCall(pstate, agg,
										args, aggorder, agg_distinct);
}

/*
 * transformGroupingFunc
 * transformGroupingFunc的入口函数
 */
Node *
transformGroupingFunc(ParseState *pstate, GroupingFunc *p)
{
	Node	*n;

	n = standard_transformGroupingFunc(pstate, p);
	
	return n;
}

/*
 * transformWindowFuncCall
 * transformWindowFuncCall的入口函数
 */
void
transformWindowFuncCall(ParseState *pstate, WindowFunc *wfunc,
						WindowDef *windef)
{
	standard_transformWindowFuncCall(pstate, wfunc, windef);
}


/*
 * transformFromClause
 * transformFromClause的入口函数
 */
void
transformFromClause(ParseState *pstate, List *frmList)
{
	standard_transformFromClause(pstate, frmList);
}

/*
 * transformWhereClause
 * transformWhereClause的入口函数
 */
Node *
transformWhereClause(ParseState *pstate, Node *clause,
					 ParseExprKind exprKind, const char *constructName)
{
	Node	*n;

	n = standard_transformWhereClause(pstate, clause, exprKind, constructName);

	return n;
}

/*
 * transformLimitClause
 * transformLimitClause的入口函数
 */
Node *
transformLimitClause(ParseState *pstate, Node *clause,
					 ParseExprKind exprKind, const char *constructName,
					 LimitOption limitOption)
{
	Node	*n;

	n = standard_transformLimitClause(pstate, clause,
										  exprKind, constructName, limitOption);

	return n;	
}

/*
 * transformGroupClause
 * transformGroupClause的入口函数
 */
List *
transformGroupClause(ParseState *pstate, List *grouplist, List **groupingSets,
					 List **targetlist, List *sortClause,
					 ParseExprKind exprKind, bool useSQL99)
{
	List	*result;

	Assert(parserengine != NULL);

	if (parserengine->transformGroupClause)
		result = parserengine->transformGroupClause(pstate, grouplist, groupingSets,
													targetlist, sortClause,
													exprKind, useSQL99);
	else
		result = standard_transformGroupClause(pstate, grouplist, groupingSets,
											   targetlist, sortClause,
											   exprKind, useSQL99);

	return result;																						
}

/*
 * transformSortClause
 * transformSortClause的入口函数
 */
List *
transformSortClause(ParseState *pstate,
					List *orderlist,
					List **targetlist,
					ParseExprKind exprKind,
					bool useSQL99)
{
	List	*result;

	result = standard_transformSortClause(pstate, orderlist, targetlist, exprKind, useSQL99);

	return result;
}

/*
 * transformWindowDefinitions
 * transformWindowDefinitions的入口函数
 */
List *
transformWindowDefinitions(ParseState *pstate,
						   List *windowdefs,
						   List **targetlist)
{
	List	*result;

	result = standard_transformWindowDefinitions(pstate, windowdefs, targetlist);
	
	return result;
}

/*
 * transformDistinctClause
 * transformDistinctClause的入口函数
 */
List *
transformDistinctClause(ParseState *pstate,
						List **targetlist, List *sortClause, bool is_agg)
{
	List	*result;

	Assert(parserengine != NULL);

	if (parserengine->transformDistinctClause)
		result = parserengine->transformDistinctClause(pstate, targetlist, sortClause, is_agg);
	else
		result = standard_transformDistinctClause(pstate, targetlist, sortClause, is_agg);
	
	return result;
}

/*
 * transformDistinctOnClause
 * transformDistinctOnClause的入口函数
 */
List *
transformDistinctOnClause(ParseState *pstate, List *distinctlist,
						  List **targetlist, List *sortClause)
{
	List	*result;

	result = standard_transformDistinctOnClause(pstate, distinctlist, targetlist, sortClause);
	
	return result;
}

/*
 * transformOnConflictArbiter
 * transformOnConflictArbiter的入口函数
 */
void
transformOnConflictArbiter(ParseState *pstate,
						   OnConflictClause *onConflictClause,
						   List **arbiterExpr, Node **arbiterWhere,
						   Oid *constraint)
{
	Assert(parserengine != NULL);

	if (parserengine->transformOnConflictArbiter)
		parserengine->transformOnConflictArbiter(pstate, onConflictClause, arbiterExpr, arbiterWhere, constraint);
	else
		standard_transformOnConflictArbiter(pstate, onConflictClause, arbiterExpr, arbiterWhere, constraint);
}

/*
 * transformWithClause
 * transformWithClause的入口函数
 */
List *
transformWithClause(ParseState *pstate, WithClause *withClause)
{
    List    *result;
	
	result = standard_transformWithClause(pstate, withClause);

    return result;
}

/*
 * transformExpr
 * transformExpr的入口函数
 */
Node *
transformExpr(ParseState *pstate, Node *expr, ParseExprKind exprKind)
{
	Node	*n;

	Assert(parserengine != NULL);

	if (parserengine->transformExpr)
		n = parserengine->transformExpr(pstate, expr, exprKind);
	else
		n = standard_transformExpr(pstate, expr, exprKind);
	
	return n;
}

/*
 * transformContainerType
 * transformContainerType的入口函数
 */
void
transformContainerType(Oid *containerType, int32 *containerTypmod)
{
	standard_transformContainerType(containerType, containerTypmod);
}

/*
 * transformContainerSubscripts
 * transformContainerSubscripts的入口函数
 */
SubscriptingRef *
transformContainerSubscripts(ParseState *pstate,
							 Node *containerBase,
							 Oid containerType,
							 int32 containerTypMod,
							 List *indirection,
							 bool isAssignment)
{
	SubscriptingRef	*scref;

	scref = standard_transformContainerSubscripts(pstate, containerBase, containerType, 
													  containerTypMod, indirection, isAssignment);

	return scref;
}

/*
 * transformTargetList
 * transformTargetList的入口函数
 */
List *
transformTargetList(ParseState *pstate, List *targetlist,
					ParseExprKind exprKind)
{
	List	*result;

	Assert(parserengine != NULL);

	result = standard_transformTargetList(pstate, targetlist, exprKind);
	
	return result;
}

/*
 * transformExpressionList
 * transformExpressionList的入口函数
 */
List *
transformExpressionList(ParseState *pstate, List *exprlist,
                        ParseExprKind exprKind, bool allowDefault)
{
	List	*result;

	result = standard_transformExpressionList(pstate, exprlist, exprKind, allowDefault);

	return result;
}

/*
 * transformTargetEntry
 * transformTargetEntry的入口函数
 */
TargetEntry *
transformTargetEntry(ParseState *pstate,
					 Node *node,
					 Node *expr,
					 ParseExprKind exprKind,
					 char *colname,
					 bool resjunk)
{
	TargetEntry	*te;

	te = standard_transformTargetEntry(pstate, node, expr, exprKind, colname, resjunk);
	
	return te;
}

/*
 * transformAssignedExpr
 * transformAssignedExpr的入口函数
 */
Expr *
transformAssignedExpr(ParseState *pstate,
					  Expr *expr,
					  ParseExprKind exprKind,
					  const char *colname,
					  int attrno,
					  List *indirection,
					  int location)
{
	Expr	*xpr;

	xpr = standard_transformAssignedExpr(pstate, expr, exprKind, colname, attrno, indirection, location);
	
	return xpr;
}

/*
 * transformAssignmentIndirection
 * transformAssignmentIndirection的入口函数
 */
Node *
transformAssignmentIndirection(ParseState *pstate,
							   Node *basenode,
							   const char *targetName,
							   bool targetIsSubscripting,
							   Oid targetTypeId,
							   int32 targetTypMod,
							   Oid targetCollation,
							   List *indirection,
							   ListCell *indirection_cell,
							   Node *rhs,
							   CoercionContext ccontext,
							   int location)
{
	Node	*n;

	n = standard_transformAssignmentIndirection(pstate,
														 basenode,
														 targetName,
														 targetIsSubscripting,
														 targetTypeId,
														 targetTypMod,
														 targetCollation,
														 indirection,
														 indirection_cell,
														 rhs,
														 ccontext,
														 location);

	return n;
}

/*
 * transformCreateStmt
 * transformCreateStmt的入口函数
 */
List *
transformCreateStmt(CreateStmt *stmt, const char *queryString)
{
	List	*result;

	Assert(parserengine != NULL);

	if (parserengine->transformCreateStmt)
		result = parserengine->transformCreateStmt(stmt, queryString);
	else
		result = standard_transformCreateStmt(stmt, queryString);
	
	return result;
}

/*
 * transformAlterTableStmt
 * transformAlterTableStmt的入口函数
 */
AlterTableStmt *
transformAlterTableStmt(Oid relid, AlterTableStmt *stmt,
						const char *queryString,
						List **beforeStmts, List **afterStmts)
{
	AlterTableStmt	*atcmd;

	Assert(parserengine != NULL);

	if (parserengine->transformAlterTableStmt)
		atcmd = parserengine->transformAlterTableStmt(relid, stmt, queryString, beforeStmts, afterStmts);
	else
		atcmd = standard_transformAlterTableStmt(relid, stmt, queryString, beforeStmts, afterStmts);
	
	return atcmd;
}

/*
 * transformIndexStmt
 * transformIndexStmt的入口函数
 */
IndexStmt *
transformIndexStmt(Oid relid, IndexStmt *stmt, const char *queryString)
{
	IndexStmt	*istmt;

	istmt = standard_transformIndexStmt(relid, stmt, queryString);
	
	return istmt;
}

/*
 * transformStatsStmt
 * transformStatsStmt的入口函数
 */
CreateStatsStmt *
transformStatsStmt(Oid relid, CreateStatsStmt *stmt, const char *queryString)
{
	CreateStatsStmt	*csstmt;

	csstmt = standard_transformStatsStmt(relid, stmt, queryString);
	
	return csstmt;
}

/*
 * transformRuleStmt
 * transformRuleStmt的入口函数
 */
void
transformRuleStmt(RuleStmt *stmt, const char *queryString,
				  List **actions, Node **whereClause)
{
	standard_transformRuleStmt(stmt, queryString, actions, whereClause);
}

/*
 * transformCreateSchemaStmt
 * transformCreateSchemaStmt的入口函数
 */
List *
transformCreateSchemaStmt(CreateSchemaStmt *stmt)
{
	List	*result;

	result = standard_transformCreateSchemaStmt(stmt);
	
	return result;
}

/*
 * transformPartitionBound
 * transformPartitionBound的入口函数
 */
PartitionBoundSpec *
transformPartitionBound(ParseState *pstate, Relation parent,
						PartitionBoundSpec *spec)
{
	PartitionBoundSpec	*parbs;

	parbs = standard_transformPartitionBound(pstate, parent, spec);
	
	return parbs;
}

static void
standard_preTransformWindowAgg(ParseState *pstate, WindowFunc *wfunc, List *funcname, 
                               bool agg_distinct, List *agg_order, Node *last_srf, bool retset)
{
	if (agg_distinct)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				errmsg("DISTINCT is not implemented for window functions"),
				parser_errposition(pstate, wfunc->location)));

	/*
	 * Reject attempt to call a parameterless aggregate without (*)
	 * syntax.  This is mere pedantry but some folks insisted ...
	 */
	if (wfunc->winagg && wfunc->args == NIL && !wfunc->winstar)
		ereport(ERROR,
				(errcode(ERRCODE_WRONG_OBJECT_TYPE),
				 errmsg("%s(*) must be used to call a parameterless aggregate function",
						NameListToString(funcname)),
				 parser_errposition(pstate, wfunc->location)));

	/*
	 * ordered aggs not allowed in windows yet
	 */
	if (agg_order != NIL)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("aggregate ORDER BY is not implemented for window functions"),
				 parser_errposition(pstate, wfunc->location)));

	/*
	 * FILTER is not yet supported with true window functions
	 */
	if (!wfunc->winagg && wfunc->aggfilter)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("FILTER is not implemented for non-aggregate window functions"),
				 parser_errposition(pstate, wfunc->location)));

	/*
	 * Window functions can't either take or return sets
	 */
	if (pstate->p_last_srf != last_srf)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("window function calls cannot contain set-returning function calls"),
				 errhint("You might be able to move the set-returning function into a LATERAL FROM item."),
				 parser_errposition(pstate,
									exprLocation(pstate->p_last_srf))));

	if (retset)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
				 errmsg("window functions cannot return sets"),
				 parser_errposition(pstate, wfunc->location)));
}

/*
 * preTransformWindowAgg
 *     preTransformWindowAgg的入口函数
 */
void
preTransformWindowAgg(ParseState *pstate, WindowFunc *wfunc, List *funcname,
					  bool agg_distinct, List *agg_order, Node *last_srf, bool retset)
{
	Assert(parserengine != NULL);

	if (parserengine->preTransformWindowAgg)
		parserengine->preTransformWindowAgg(pstate, wfunc, funcname, 
												agg_distinct, agg_order, last_srf, retset);
	else
		standard_preTransformWindowAgg(pstate, wfunc, funcname, 
									   	   agg_distinct, agg_order, last_srf, retset);
}

/*
 * finalTransformColumnRef
 *   final chance to transform columnref
 *   just return NULL by default
 */
Node *
finalTransformColumnRef(ParseState *pstate, ColumnRef *cref)
{
	Node	*n;

	if (parserengine->finalTransformColumnRef)
		n = parserengine->finalTransformColumnRef(pstate, cref);
	else
		n = NULL;

	return n;
}

bool
analyze_requires_snapshot(RawStmt *parseTree)
{
	bool		result;

	if (parserengine->analyze_requires_snapshot)
		result = parserengine->analyze_requires_snapshot(parseTree);
	else
		result = standard_analyze_requires_snapshot(parseTree);

	return result;
}

/*
 * parse collate
 */
void
assign_query_collations(ParseState *pstate, Query *query)
{
	if (parserengine->assign_query_collations)
		parserengine->assign_query_collations(pstate, query);
	else
		standard_assign_query_collations(pstate, query);
}

void
assign_list_collations(ParseState *pstate, List *exprs)
{
	if (parserengine->assign_list_collations)
		parserengine->assign_list_collations(pstate, exprs);
	else
		standard_assign_list_collations(pstate, exprs);
}

void
assign_expr_collations(ParseState *pstate, Node *expr)
{
	if (parserengine->assign_expr_collations)
		parserengine->assign_expr_collations(pstate, expr);
	else
		standard_assign_expr_collations(pstate, expr);
}

Oid
select_common_collation(ParseState *pstate, List *exprs, bool none_ok)
{
	Oid		collation;

	if (parserengine->select_common_collation)
		collation = parserengine->select_common_collation(pstate, exprs, none_ok);
	else
		collation = standard_select_common_collation(pstate, exprs, none_ok);

	return collation;
}

/*
 * Rewrite subsystem's interfaces 
 */
List *
QueryRewrite(Query *parsetree)
{
	List	*result;

	if (parserengine->rewrite &&
			parserengine->rewrite->QueryRewrite)
		result = parserengine->rewrite->QueryRewrite(parsetree);
	else
		result = standard_QueryRewrite(parsetree);
	
	return result;
}

ObjectAddress
DefineRule(RuleStmt *stmt, const char *queryString)
{
	ObjectAddress	result;

	if (parserengine->rewrite &&
			parserengine->rewrite->DefineRule)
		result = parserengine->rewrite->DefineRule(stmt, queryString);
	else
		result = standard_DefineRule(stmt, queryString);
	
	return result;
}

ObjectAddress
DefineQueryRewrite(const char *rulename,
				   Oid event_relid,
				   Node *event_qual,
				   CmdType event_type,
				   bool is_instead,
				   bool replace,
				   List *action)
{
	ObjectAddress	result;

	if (parserengine->rewrite &&
			parserengine->rewrite->DefineQueryRewrite)
		result = parserengine->rewrite->DefineQueryRewrite(rulename, event_relid, 
														   event_qual, event_type,
														   is_instead, replace, action);
	else
		result = standard_DefineQueryRewrite(rulename, event_relid, 
											 event_qual, event_type,
											 is_instead, replace, action);
	
	return result;
}

ObjectAddress
RenameRewriteRule(RangeVar *relation,
				  const char *oldName,
				  const char *newName)
{
	ObjectAddress	result;

	if (parserengine->rewrite &&
			parserengine->rewrite->RenameRewriteRule)
		result = parserengine->rewrite->RenameRewriteRule(relation, oldName, newName);
	else
		result = standard_RenameRewriteRule(relation, oldName, newName);
	
	return result;
}

void
setRuleCheckAsUser(Node *node, Oid userid)
{
	if (parserengine->rewrite &&
			parserengine->rewrite->setRuleCheckAsUser)
		parserengine->rewrite->setRuleCheckAsUser(node, userid);
	else
		standard_setRuleCheckAsUser(node, userid);
}

void
EnableDisableRule(Relation rel, const char *rulename, char fires_when)
{
	if (parserengine->rewrite &&
			parserengine->rewrite->EnableDisableRule)
		parserengine->rewrite->EnableDisableRule(rel, rulename, fires_when);
	else
		standard_EnableDisableRule(rel, rulename, fires_when);
}

void
get_row_security_policies(Query *root,
						  RangeTblEntry *rte, int rt_index,
						  List **securityQuals, List **withCheckOptions,
						  bool *hasRowSecurity, bool *hasSubLinks)
{
	if (parserengine->rewrite &&
			parserengine->rewrite->get_row_security_policies)
		parserengine->rewrite->get_row_security_policies(root, rte, rt_index, 
														 securityQuals, withCheckOptions, 
														 hasRowSecurity, hasSubLinks);
	else
		standard_get_row_security_policies(root, rte, rt_index, 
										   securityQuals, withCheckOptions, 
										   hasRowSecurity, hasSubLinks);
}