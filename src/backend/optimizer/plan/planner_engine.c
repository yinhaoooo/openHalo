/*-------------------------------------------------------------------------
 *
 * planner_engine.c
 *		Planner Engine Helper Routine
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/optimizer/plan/planner_engine.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "optimizer/planner.h"
#include "optimizer/planner_api.h"
#include "optimizer/planner_engine.h"
#include "parser/parsereng.h"
#include "miscadmin.h"
#include "libpq/libpq-be.h"

/* */
const PlannerRoutine *plannerengine = NULL;

/*
 * InitPlannerEngine
 * 
 * Initialize planner engine.
 * will set planner_hook to planner_main, and
 * will set create_upper_paths_hook to create_upper_paths.
 */
void
InitPlannerEngine(void)
{
    switch (database_compat_mode)
    {
        case POSTGRESQL_COMPAT_MODE:
			plannerengine = GetStandardPlannerEngine();
			break;
		case MYSQL_COMPAT_MODE:
            
            if ((MyProcPort != NULL) && 
                (nodeTag(MyProcPort->protocol_handler) == T_MySQLProtocol))
            {
                plannerengine = GetStandardPlannerEngine();
            }
            else
            {
                plannerengine = GetStandardPlannerEngine();
            }
			/* plannerengine = GetStandardPlannerEngine(); */
            
			break;

		default:
			plannerengine = GetStandardPlannerEngine();
			break;
    }

    Assert(plannerengine != NULL);
}