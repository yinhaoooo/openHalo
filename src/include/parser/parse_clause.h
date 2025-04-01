/*-------------------------------------------------------------------------
 *
 * parse_clause.h
 *	  handle clauses in parser
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/parser/parse_clause.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSE_CLAUSE_H
#define PARSE_CLAUSE_H

#include "parser/parse_node.h"

extern void transformFromClause(ParseState *pstate, List *frmList);
extern int	setTargetTable(ParseState *pstate, RangeVar *relation,
						   bool inh, bool alsoSource, AclMode requiredPerms);

extern Node *transformWhereClause(ParseState *pstate, Node *clause,
								  ParseExprKind exprKind, const char *constructName);
extern Node *transformLimitClause(ParseState *pstate, Node *clause,
								  ParseExprKind exprKind, const char *constructName,
								  LimitOption limitOption);
extern List *transformGroupClause(ParseState *pstate, List *grouplist,
								  List **groupingSets,
								  List **targetlist, List *sortClause,
								  ParseExprKind exprKind, bool useSQL99);
extern List *transformSortClause(ParseState *pstate, List *orderlist,
								 List **targetlist, ParseExprKind exprKind,
								 bool useSQL99);

extern List *transformWindowDefinitions(ParseState *pstate,
										List *windowdefs,
										List **targetlist);

extern List *transformDistinctClause(ParseState *pstate,
									 List **targetlist, List *sortClause, bool is_agg);
extern List *transformDistinctOnClause(ParseState *pstate, List *distinctlist,
									   List **targetlist, List *sortClause);
extern void transformOnConflictArbiter(ParseState *pstate,
									   OnConflictClause *onConflictClause,
									   List **arbiterExpr, Node **arbiterWhere,
									   Oid *constraint);

extern List *addTargetToSortList(ParseState *pstate, TargetEntry *tle,
								 List *sortlist, List *targetlist, SortBy *sortby);
extern Index assignSortGroupRef(TargetEntry *tle, List *tlist);
extern bool targetIsInSortList(TargetEntry *tle, Oid sortop, List *sortList);


extern void standard_transformFromClause(ParseState *pstate, List *frmList);
extern Node *standard_transformWhereClause(ParseState *pstate, Node *clause,
								  		   ParseExprKind exprKind, const char *constructName);
extern Node *standard_transformLimitClause(ParseState *pstate, Node *clause,
								  		   ParseExprKind exprKind, const char *constructName,
								  		   LimitOption limitOption);
extern List *standard_transformGroupClause(ParseState *pstate, List *grouplist,
								  		   List **groupingSets,
								  		   List **targetlist, List *sortClause,
								  		   ParseExprKind exprKind, bool useSQL99);
extern List *standard_transformSortClause(ParseState *pstate, List *orderlist,
								 		  List **targetlist, ParseExprKind exprKind,
								 		  bool useSQL99);
extern List *standard_transformWindowDefinitions(ParseState *pstate,
												 List *windowdefs,
												 List **targetlist);
extern List *standard_transformDistinctClause(ParseState *pstate,
									 		  List **targetlist, List *sortClause, bool is_agg);
extern List *standard_transformDistinctOnClause(ParseState *pstate, List *distinctlist,
									   			List **targetlist, List *sortClause);
extern void standard_transformOnConflictArbiter(ParseState *pstate,
									   			OnConflictClause *onConflictClause,
									   			List **arbiterExpr, Node **arbiterWhere,
									   			Oid *constraint);	

#endif							/* PARSE_CLAUSE_H */
