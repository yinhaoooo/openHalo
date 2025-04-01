/*-------------------------------------------------------------------------
 *
 * executor_engine.c
 *		Executor Engine Helper Routine
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/executor/executor_engine.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/executor_api.h"
#include "executor/executor_engine.h"
#include "executor/executor.h"
#include "executor/mys_executor.h"
#include "parser/parsereng.h"
#include "tcop/utility.h"
#include "miscadmin.h"
#include "libpq/libpq-be.h"

/* */
const ExecutorRoutine *executorengine = NULL;

/*
 * InitExecutorEngine
 * 
 * Initialize executor engine.
 * will set ExecutorStart_hook to ExecutorStart, and
 * will set ExecutorRun_hook to ExecutorRun, and
 * will set ExecutorFinish_hook to ExecutorFinish, and
 * will set ExecutorEnd_hook to ExecutorEnd, and
 * will set ExecutorCheckPerms_hook to ExecutorCheckPerms.
 */
void
InitExecutorEngine(void)
{
    switch (database_compat_mode)
    {
        case POSTGRESQL_COMPAT_MODE:
			executorengine = GetStandardExecutorEngine();
			break;
		case MYSQL_COMPAT_MODE:
            
            if ((MyProcPort != NULL) && 
                (nodeTag(MyProcPort->protocol_handler) == T_MySQLProtocol))
            {
                executorengine = GetMysExecutorEngine();
                ProcessUtility_hook = mys_standard_ProcessUtility;
            }
            else
            {
                executorengine = GetStandardExecutorEngine();
            }
			/* executorengine = GetMysExecutorEngine(); */
			/* ProcessUtility_hook = mys_standard_ProcessUtility; */
            
			break;

		default:
			executorengine = GetStandardExecutorEngine();
			break;
    }

    Assert(executorengine != NULL);

    if (executorengine->ExecutorStart)
		ExecutorStart_hook = executorengine->ExecutorStart;
    
    if (executorengine->ExecutorRun)
		ExecutorRun_hook = executorengine->ExecutorRun;
}