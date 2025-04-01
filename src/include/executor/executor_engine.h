/*-------------------------------------------------------------------------
 *
 * executor_engine.h
 *		HEADER for executor engine
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * src/include/executor/executor_engine.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef EXECUTOR_ENGINE_H
#define EXECUTOR_ENGINE_H

#include "executor/executor_api.h"

/* planner engine instance */
extern const ExecutorRoutine *executorengine;


/* initialize planner engine */
extern void InitExecutorEngine(void);

#endif							/* EXECUTOR_ENGINE_H */