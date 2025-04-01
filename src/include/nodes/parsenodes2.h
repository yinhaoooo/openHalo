/*-------------------------------------------------------------------------
 *
 * parsenodes2.h
 *	  definitions for parse tree nodes
 *
 * Many of the node types used in parsetrees include a "location" field.
 * This is a byte (not character) offset in the original source text, to be
 * used for positioning an error cursor when there is an error related to
 * the node.  Access to the original source text is needed to make use of
 * the location.  At the topmost (statement) level, we also provide a
 * statement length, likewise measured in bytes, for convenience in
 * identifying statement boundaries in multi-statement source strings.
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/nodes/parsenodes2.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSENODES2_H
#define PARSENODES2_H

#include "nodes/nodes.h"
#include "nodes/parsenodes.h"


/* ----------------------
 *		Merge Statement
 * ----------------------
 */
typedef struct MergeStmt
{
	NodeTag		type;
	Node   	   *relation;		/* target relation to merge into */
	Node	   *sourceRelation; /* source relation */
	Node	   *joinCondition;	/* join condition between source and target */
	List	   *mergeWhenClauses;	/* list of MergeWhenClause(es) */
	WithClause *withClause;		/* WITH clause */
} MergeStmt;

/*
 * MergeWhenClause -
 *		raw parser representation of a WHEN clause in a MERGE statement
 *
 * This is transformed into MergeAction by parse analysis
 */
typedef struct MergeWhenClause
{
	NodeTag		type;
	bool		matched;		/* true=MATCHED, false=NOT MATCHED */
	CmdType		commandType;	/* INSERT/UPDATE/DELETE/DO NOTHING */
	OverridingKind override;	/* OVERRIDING clause */
	Node	   *condition;		/* WHEN conditions (raw parser) */
	List	   *targetList;		/* INSERT/UPDATE targetlist */
	/* the following members are only used in INSERT actions */
	List	   *values;			/* VALUES to INSERT, or NULL */
} MergeWhenClause;

/*
 * MergeAction -
 *		Transformed representation of a WHEN clause in a MERGE statement
 */
typedef struct MergeAction
{
	NodeTag		type;
	bool		matched;		/* true=MATCHED, false=NOT MATCHED */
	CmdType		commandType;	/* INSERT/UPDATE/DELETE/DO NOTHING */
	OverridingKind override;	/* OVERRIDING clause */
	Node	   *qual;			/* transformed WHEN conditions */
	List	   *targetList;		/* the target list (of TargetEntry) */
	List	   *updateColnos;	/* target attribute numbers of an UPDATE */
} MergeAction;

#endif							/* PARSENODES2_H */
