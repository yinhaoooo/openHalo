/*-------------------------------------------------------------------------
 *
 * analyze.h
 *		parse analysis for optimizable statements
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/parser/analyze.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef ANALYZE_H
#define ANALYZE_H

#include "parser/parse_node.h"
#include "utils/queryjumble.h"

/* Hook for plugins to get control at end of parse analysis */
typedef void (*post_parse_analyze_hook_type) (ParseState *pstate,
											  Query *query,
											  JumbleState *jstate);
extern PGDLLIMPORT post_parse_analyze_hook_type post_parse_analyze_hook;


extern Query *parse_analyze(RawStmt *parseTree, const char *sourceText,
							Oid *paramTypes, int numParams, QueryEnvironment *queryEnv);
extern Query *parse_analyze_varparams(RawStmt *parseTree, const char *sourceText,
									  Oid **paramTypes, int *numParams);

extern Query *parse_sub_analyze(Node *parseTree, ParseState *parentParseState,
								CommonTableExpr *parentCTE,
								bool locked_from_parent,
								bool resolve_unknowns);

extern Query *transformTopLevelStmt(ParseState *pstate, RawStmt *parseTree);
extern Query *transformStmt(ParseState *pstate, Node *parseTree);

extern bool stmt_requires_parse_analysis(RawStmt *parseTree);
extern bool analyze_requires_snapshot(RawStmt *parseTree);

extern const char *LCS_asString(LockClauseStrength strength);
extern void CheckSelectLocking(Query *qry, LockClauseStrength strength);
extern void applyLockingClause(Query *qry, Index rtindex,
							   LockClauseStrength strength,
							   LockWaitPolicy waitPolicy, bool pushedDown);

extern List *BuildOnConflictExcludedTargetlist(Relation targetrel,
											   Index exclRelIndex);

extern SortGroupClause *makeSortGroupClauseForSetOp(Oid rescoltype, bool require_hash);


extern Query *standard_parse_analyze(RawStmt *parseTree, const char *sourceText,
									 Oid *paramTypes, int numParams, QueryEnvironment *queryEnv);
extern Query *standard_parse_analyze_varparams(RawStmt *parseTree, const char *sourceText,
									  		   Oid **paramTypes, int *numParams);
extern Query *standard_transformStmt(ParseState *pstate, Node *parseTree);

extern Query *transformSelectStmt(ParseState *pstate, SelectStmt *stmt);
extern Query *transformInsertStmt(ParseState *pstate, InsertStmt *stmt);
extern Query *transformDeleteStmt(ParseState *pstate, DeleteStmt *stmt);
extern Query *transformUpdateStmt(ParseState *pstate, UpdateStmt *stmt);
extern Query *transformValuesClause(ParseState *pstate, SelectStmt *stmt);
extern Query *transformSetOperationStmt(ParseState *pstate, SelectStmt *stmt);
extern Query *transformReturnStmt(ParseState *pstate, ReturnStmt *stmt);
extern Query *transformPLAssignStmt(ParseState *pstate, PLAssignStmt *stmt);
extern Query *transformDeclareCursorStmt(ParseState *pstate, DeclareCursorStmt *stmt);
extern Query *transformExplainStmt(ParseState *pstate, ExplainStmt *stmt);
extern Query *transformCreateTableAsStmt(ParseState *pstate, CreateTableAsStmt *stmt);
extern Query *transformCallStmt(ParseState *pstate, CallStmt *stmt);
extern List *transformReturningList(ParseState *pstate, List *returningList);
extern List *transformInsertRow(ParseState *pstate, List *exprlist,
                                List *stmtcols, List *icolumns, List *attrnos,
                                bool strip_indirection);
extern OnConflictExpr *transformOnConflictClause(ParseState *pstate, OnConflictClause *onConflictClause);
extern List *transformUpdateTargetList(ParseState *pstate, List *origTlist);
extern void transformLockingClause(ParseState *pstate, Query *qry, LockingClause *lc, bool pushedDown);
extern Query *transformOptionalSelectInto(ParseState *pstate, Node *parseTree);
extern Node *transformSetOperationTree(ParseState *pstate, SelectStmt *stmt,
									   bool isTopLevel, List **targetlist);
extern bool standard_analyze_requires_snapshot(RawStmt *parseTree);
extern Query *standard_parse_sub_analyze(Node *parseTree, ParseState *parentParseState,
											CommonTableExpr *parentCTE,
											bool locked_from_parent,
											bool resolve_unknowns);

#endif							/* ANALYZE_H */
