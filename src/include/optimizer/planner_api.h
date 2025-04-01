/*-------------------------------------------------------------------------
 *
 * planner_api.h
 *		API for planner
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * src/include/optimizer/planner_api.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PLANNER_API_H
#define PLANNER_API_H

#include "nodes/pathnodes.h"
#include "nodes/plannodes.h"
#include "utils/relcache.h"

/*
 * API for planner routine
 */
typedef struct PlannerRoutine
{
    NodeTag		            type;
} PlannerRoutine;

#endif							/* PLANNER_API_H */