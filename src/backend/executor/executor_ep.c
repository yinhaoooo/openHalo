/*-------------------------------------------------------------------------
 *
 * executor_ep.c
 *		Executor Engine Entry Point
 *
 *
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 *
 * IDENTIFICATION
 *	  src/backend/executor/executor_ep.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execPartition.h"
#include "executor/executor.h"
#include "executor/executor_engine.h"

/*
 * InitPlan
 *
 * see InitPlan in execMain
 */

/*
 * ExecutePlan
 *
 * see ExecutePlan in execMain
 */

/*
 * ExecPostprocessPlan
 *
 * see ExecPostprocessPlan in execMain
 */

/*
 * ExecInitNode
 *
 *     entry point for ExecInitNode(...)
 */
PlanState *
ExecInitNode(Plan *node, EState *estate, int eflags)
{
    PlanState   *result;

    Assert(executorengine != NULL);

    if (executorengine->ExecInitNode)
        result = executorengine->ExecInitNode(node, estate, eflags);
    else
        result = standard_ExecInitNode(node, estate, eflags);

    return result;
}

/*
 * ExecEndNode
 *
 *     entry point for ExecEndNode(...)
 */
void
ExecEndNode(PlanState *node)
{
    Assert(executorengine != NULL);

    if (executorengine->ExecEndNode)
        executorengine->ExecEndNode(node);
    else
        standard_ExecEndNode(node);
}

/*
 * ExecReScan
 *
 *     entry point for ExecReScan(...)
 */
void
ExecReScan(PlanState *node)
{
    Assert(executorengine != NULL);

    if (executorengine->ExecReScan)
        executorengine->ExecReScan(node);
    else
        standard_ExecReScan(node);
}

/*
 * MultiExecProcNode
 *
 *     entry point for MultiExecProcNode(...)
 */
Node *
MultiExecProcNode(PlanState *node)
{
    Node    *result;

    Assert(executorengine != NULL);

    if (executorengine->MultiExecProcNode)
        result = executorengine->MultiExecProcNode(node);
    else
        result = standard_MultiExecProcNode(node);
    
    return result;
}

/*
 * ExecShutdownNode
 *
 *     entry point for ExecShutdownNode(...)
 */
bool
ExecShutdownNode(PlanState *node)
{
    bool    result;

    Assert(executorengine != NULL);

    if (executorengine->ExecShutdownNode)
        result = executorengine->ExecShutdownNode(node);
    else
        result = standard_ExecShutdownNode(node);

    return result;
}

/* Partition routine entries */
PartitionTupleRouting *
ExecSetupPartitionTupleRouting(EState *estate,
                               Relation rel)
{
    PartitionTupleRouting   *result;

    result = standard_ExecSetupPartitionTupleRouting(estate, rel);

    return result;
}

ResultRelInfo *
ExecFindPartition(ModifyTableState *mtstate,
				  ResultRelInfo *rootResultRelInfo,
				  PartitionTupleRouting *proute,
				  TupleTableSlot *slot,
				  EState *estate)
{
    ResultRelInfo   *result;

    if (executorengine->partition &&
            executorengine->partition->ExecFindPartition)
        result = executorengine->partition->ExecFindPartition(mtstate, rootResultRelInfo,
                                                                proute, slot, estate);
    else
        result = standard_ExecFindPartition(mtstate, rootResultRelInfo,
                                                proute, slot, estate);

    return result;
}

void
ExecCleanupTupleRouting(ModifyTableState *mtstate,
						PartitionTupleRouting *proute)
{
    standard_ExecCleanupTupleRouting(mtstate, proute);
}

PartitionPruneState *
ExecCreatePartitionPruneState(PlanState *planstate,
							  PartitionPruneInfo *partitionpruneinfo)
{
    PartitionPruneState   *result;

    result = standard_ExecCreatePartitionPruneState(planstate,
                                                        partitionpruneinfo);

    return result;
}

Bitmapset *
ExecFindMatchingSubPlans(PartitionPruneState *prunestate)
{
    Bitmapset   *result;

    result = standard_ExecFindMatchingSubPlans(prunestate);

    return result;
}

Bitmapset *
ExecFindInitialMatchingSubPlans(PartitionPruneState *prunestate,
								int nsubplans)
{
    Bitmapset   *result;

    result = standard_ExecFindInitialMatchingSubPlans(prunestate, nsubplans);

    return result;
}

/* Replication routine entries */
bool
RelationFindReplTupleByIndex(Relation rel, Oid idxoid,
							 LockTupleMode lockmode,
							 TupleTableSlot *searchslot,
							 TupleTableSlot *outslot)
{
    bool   result;

    result = standard_RelationFindReplTupleByIndex(rel, idxoid,
                                                        lockmode, searchslot, outslot);

    return result;
}

bool
RelationFindReplTupleSeq(Relation rel, LockTupleMode lockmode,
						 TupleTableSlot *searchslot, TupleTableSlot *outslot)
{
    bool   result;

    result = standard_RelationFindReplTupleSeq(rel, lockmode, searchslot, outslot);

    return result;
}

void
ExecSimpleRelationInsert(ResultRelInfo *resultRelInfo,
						 EState *estate, TupleTableSlot *slot)
{

    standard_ExecSimpleRelationInsert(resultRelInfo, estate, slot);
}

void
ExecSimpleRelationUpdate(ResultRelInfo *resultRelInfo,
						 EState *estate, EPQState *epqstate,
						 TupleTableSlot *searchslot, TupleTableSlot *slot)
{
    standard_ExecSimpleRelationUpdate(resultRelInfo, estate, epqstate,
                                            searchslot, slot);
}

void ExecSimpleRelationDelete(ResultRelInfo *resultRelInfo,
							  EState *estate, EPQState *epqstate,
							  TupleTableSlot *searchslot)
{
    standard_ExecSimpleRelationDelete(resultRelInfo, estate, epqstate,
                                            searchslot);
}

void
CheckCmdReplicaIdentity(Relation rel, CmdType cmd)
{
    standard_CheckCmdReplicaIdentity(rel, cmd);
}

void
CheckSubscriptionRelkind(char relkind, const char *nspname,
						 const char *relname)
{
    standard_CheckSubscriptionRelkind(relkind, nspname, relname);
}

void
ExecEvalSQLValueFunction(ExprState *state, ExprEvalStep *op)
{
    standard_ExecEvalSQLValueFunction(state, op);
}