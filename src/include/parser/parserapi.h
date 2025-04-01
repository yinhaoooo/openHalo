/*-------------------------------------------------------------------------
 *
 * parserapi.h
 *		API for parser
 *
 * This is the external API for the raw lexing/parsing functions.
 *
 * Portions Copyright (c) 2021, Jet C.X. ZHANG
 *
 * src/include/parser/parserapi.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSERAPI_H
#define PARSERAPI_H

#include "common/kwlookup.h"
#include "nodes/parsenodes.h"
#include "parser/parse_func.h"
#include "parser/parse_node.h"
#include "parser/parsereng.h"
#include "rewrite/rewriteapi.h"
#include "catalog/namespace.h"


/*
 * Parser callback functions
 */
typedef List *(*raw_parser_function) (const char *str, RawParseMode mode);

typedef Query *(*transformStmt_function)(ParseState *pstate, Node *parseTree);
typedef Query *(*transformSelectStmt_function)(ParseState *pstate, SelectStmt *stmt);
typedef Query *(*transformInsertStmt_function)(ParseState *pstate, InsertStmt *stmt);
typedef Query *(*transformDeleteStmt_function)(ParseState *pstate, DeleteStmt *stmt);
typedef Query *(*transformUpdateStmt_function)(ParseState *pstate, UpdateStmt *stmt);
typedef Query *(*transformSetOperationStmt_function)(ParseState *pstate, SelectStmt *stmt);
typedef Query *(*transformCallStmt_function)(ParseState *pstate, CallStmt *stmt);
typedef Query *(*transformOptionalSelectInto_function)(ParseState *pstate, Node *parseTree);
typedef Node *(*transformSetOperationTree_function)(ParseState *pstate, SelectStmt *stmt,
									   			    bool isTopLevel, List **targetlist);
typedef bool (*analyze_requires_snapshot_function)(RawStmt *parseTree);

typedef Query *(*parse_sub_analyze_function)(Node *parseTree, ParseState *parentParseState,
                                                CommonTableExpr *parentCTE,
                                                bool locked_from_parent,
                                                bool resolve_unknowns);

// parse_clause
typedef List *(*transformGroupClause_function)(ParseState *pstate, List *grouplist,
                                               List **groupingSets,
                                               List **targetlist, List *sortClause,
                                               ParseExprKind exprKind, bool useSQL99);
typedef List *(*transformDistinctClause_function)(ParseState *pstate,
									              List **targetlist, List *sortClause, bool is_agg);
typedef void (*transformOnConflictArbiter_function)(ParseState *pstate,
									                OnConflictClause *onConflictClause,
									                List **arbiterExpr, Node **arbiterWhere,
									                Oid *constraint);
// parse_expr
typedef Node *(*transformExpr_function)(ParseState *pstate, Node *expr, ParseExprKind exprKind);

// parse_utilcmd
typedef List *(*transformCreateStmt_function)(CreateStmt *stmt, const char *queryString);
typedef AlterTableStmt *(*transformAlterTableStmt_function)(Oid relid, AlterTableStmt *stmt,
											                const char *queryString,
											                List **beforeStmts,
											                List **afterStmts);
typedef List *(*transformCreateSchemaStmt_function)(CreateSchemaStmt *stmt);

// parse_collate
typedef void (*assign_query_collations_function)(ParseState *pstate, Query *query);
typedef void (*assign_list_collations_function)(ParseState *pstate, List *exprs);
typedef void (*assign_expr_collations_function)(ParseState *pstate, Node *expr);
typedef Oid	(*select_common_collation_function)(ParseState *pstate, List *exprs, bool none_ok);

// others
typedef void (*preTransformWindowAgg_fn)(ParseState *pstate, WindowFunc *wfunc, List *funcname,
                                         bool agg_distinct, List *agg_order, Node *last_srf, bool retset);
typedef Node *(*finalTransformColumnRef_fn)(ParseState *pstate, ColumnRef *cref);
typedef Node *(*ParseFuncOrColumn_function)(ParseState *pstate, List *funcname, List *fargs,
                                            Node *last_srf, FuncCall *fn, bool proc_call,
                                            int location);
typedef FuncDetailCode (*func_get_detail_function)(List *funcname,
                                                   List *fargs, List *fargnames,
                                                   int nargs, Oid *argtypes,
                                                   bool expand_variadic, bool expand_defaults,
                                                   bool include_out_arguments,
                                                   Oid *funcid, Oid *rettype,
                                                   bool *retset, int *nvargs, Oid *vatype,
                                                   Oid **true_typeids, List **argdefaults);
typedef void (*make_fn_arguments_function)(ParseState *pstate,
							               List *fargs,
							               Oid *actual_arg_types,
							               Oid *declared_arg_types);
/*
 * API for parser engine
 */
typedef struct ParserRoutine
{
    NodeTag		type;
    bool        is_standard_parser;
    bool        need_standard_parser;

    const ScanKeywordList     *keywordlist;
    const uint16              *keyword_tokens;
    const uint8               *keyword_categories;
    
    raw_parser_function                           raw_parser;
    transformStmt_function                        transformStmt;
    transformSelectStmt_function                  transformSelectStmt;
    transformInsertStmt_function                  transformInsertStmt;
    transformDeleteStmt_function                  transformDeleteStmt;
    transformUpdateStmt_function                  transformUpdateStmt;
    transformSetOperationStmt_function            transformSetOperationStmt;

    transformCallStmt_function                    transformCallStmt;
    transformOptionalSelectInto_function          transformOptionalSelectInto;
    transformSetOperationTree_function            transformSetOperationTree;
    analyze_requires_snapshot_function            analyze_requires_snapshot;
    parse_sub_analyze_function                    parse_sub_analyze;


    transformGroupClause_function                 transformGroupClause;

    transformDistinctClause_function              transformDistinctClause;
    transformOnConflictArbiter_function           transformOnConflictArbiter;

    transformExpr_function                        transformExpr;

    transformCreateStmt_function                  transformCreateStmt;
    transformAlterTableStmt_function              transformAlterTableStmt;
    assign_query_collations_function              assign_query_collations;
    assign_list_collations_function               assign_list_collations;
    assign_expr_collations_function               assign_expr_collations;
    select_common_collation_function              select_common_collation;

    preTransformWindowAgg_fn                      preTransformWindowAgg;
    finalTransformColumnRef_fn                    finalTransformColumnRef;

    ParseFuncOrColumn_function                    ParseFuncOrColumn;
    func_get_detail_function                      func_get_detail;
    make_fn_arguments_function                    make_fn_arguments;
    RewriteInterface                              rewrite;
} ParserRoutine;

/*
 * Deprecated routines in kernel
 */
extern List *transformCreateSchemaStmt(CreateSchemaStmt *stmt);

#endif							/* PARSERAPI_H */