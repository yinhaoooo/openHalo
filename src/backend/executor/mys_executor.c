/*-------------------------------------------------------------------------
 *
 * mys_executor.c
 *		Mysql Executor Engine
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/executor/mys_executor.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/executor.h"
#include "executor/execPartition.h"
#include "executor/mys_executor.h"
#include "executor/mys_execPartition.h"
#include "nodes/nodes.h"

/* 
 * define mysql executor engine
 */
static PartitionRoutine mys_partition_engine = {
    .ExecFindPartition = mys_ExecFindPartition
};

static const ExecutorRoutine mys_executor_engine = {
    .type = T_ExecutorRoutine,

    .ExecutorStart = mys_ExecutorStart,
    .ExecutorRun = mys_ExecutorRun,
    .ExecInitNode = mys_ExecInitNode,
    .ExecEndNode = mys_ExecEndNode,
    .ExecReScan = mys_ExecReScan,

    .partition = &mys_partition_engine
};

/*
 * GetMysExecutorEngine
 *      get mysql executor engine
 */
const ExecutorRoutine *
GetMysExecutorEngine(void)
{
    return &mys_executor_engine;
}