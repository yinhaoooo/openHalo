/*-------------------------------------------------------------------------
 *
 * executor_api.h
 *		HEADER for executor routine
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * src/include/executor/executor_api.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef EXECUTOR_API_H
#define EXECUTOR_API_H

#include "executor/execdesc.h"

#include "executor/execExpr.h"

#include "executor/execPartition.h"


/* executor callback function */
typedef void (*fn_ExecutorStart) (QueryDesc *queryDesc, int eflags);
typedef void (*fn_ExecutorRun) (QueryDesc *queryDesc,
								ScanDirection direction,
								uint64 count,
								bool execute_once);
typedef PlanState *(*fn_ExecInitNode)(Plan *node, EState *estate, int eflags);
typedef void (*fn_ExecEndNode)(PlanState *node);
typedef void (*fn_ExecReScan)(PlanState *node);
typedef Node *(*fn_MultiExecProcNode)(PlanState *node);
typedef bool (*fn_ExecShutdownNode)(PlanState *node);

/* Partiton related interface */
typedef ResultRelInfo *(*fn_ExecFindPartition)(ModifyTableState *mtstate,
                                               ResultRelInfo *rootResultRelInfo,
                                               PartitionTupleRouting *proute,
                                               TupleTableSlot *slot,
                                               EState *estate);

typedef struct PartitionRoutine
{
	fn_ExecFindPartition							ExecFindPartition;
} PartitionRoutine;
typedef PartitionRoutine	*PartitionInterface;

/*
 * API for executor routine
 */
typedef struct ExecutorRoutine
{
    NodeTag		            type;

    fn_ExecutorStart         ExecutorStart;
    fn_ExecutorRun           ExecutorRun;
	fn_ExecInitNode			 ExecInitNode;
	fn_ExecEndNode			 ExecEndNode;
	fn_ExecReScan            ExecReScan;
	fn_MultiExecProcNode     MultiExecProcNode;
	fn_ExecShutdownNode		 ExecShutdownNode;
	PartitionInterface		 partition;
} ExecutorRoutine;

#endif							/* EXECUTOR_API_H */