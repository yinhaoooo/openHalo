/*-------------------------------------------------------------------------
 *
 * planner_ep.c
 *		Planner Engine Entry Point
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/optimizer/plan/planner_ep.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "nodes/pathnodes.h"
#include "nodes/plannodes.h"
#include "optimizer/appendinfo.h"
#include "optimizer/planmain.h"
#include "optimizer/planner.h"
#include "optimizer/planner_engine.h"

PlannerInfo *
subquery_planner(PlannerGlobal *glob, Query *parse,
				 PlannerInfo *parent_root,
				 bool hasRecursion, double tuple_fraction)
{
    PlannerInfo *root;

    root = standard_subquery_planner(glob, parse, parent_root, hasRecursion, tuple_fraction);
    
    return root;
}

Plan *
create_plan(PlannerInfo *root, Path *best_path)
{
    Plan    *plan;

	plan = standard_create_plan(root, best_path);
    
    return plan;
}

void
add_row_identity_columns(PlannerInfo *root, Index rtindex,
						 RangeTblEntry *target_rte,
						 Relation target_relation)
{
	standard_add_row_identity_columns(root, rtindex, target_rte, target_relation);
}

void
distribute_row_identity_vars(PlannerInfo *root)
{
	standard_distribute_row_identity_vars(root);
}