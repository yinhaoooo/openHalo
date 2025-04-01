/*-------------------------------------------------------------------------
 *
 * parse_oper.h
 *		handle operator things for parser
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/parser/parse_oper.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef MYS_PARSE_OPER_H
#define MYS_PARSE_OPER_H

#include "access/htup.h"
#include "nodes/parsenodes.h"
#include "parser/parse_node.h"


typedef HeapTuple Operator;


/* Build expression tree for an operator invocation */
extern Expr *mys_make_op(ParseState *pstate, List *opname,
                         Node *ltree, Node *rtree, Node *last_srf, int location);

#endif							/* MYS_PARSE_OPER_H */
