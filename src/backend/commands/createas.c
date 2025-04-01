/*-------------------------------------------------------------------------
 *
 * createas.c
 *	  Execution of CREATE TABLE ... AS, a/k/a SELECT INTO.
 *	  Since CREATE MATERIALIZED VIEW shares syntax and most behaviors,
 *	  we implement that here, too.
 *
 * We implement this by diverting the query's normal output to a
 * specialized DestReceiver type.
 *
 * Formerly, CTAS was implemented as a variant of SELECT, which led
 * to assorted legacy behaviors that we still try to preserve, notably that
 * we must return a tuples-processed count in the QueryCompletion.  (We no
 * longer do that for CTAS ... WITH NO DATA, however.)
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/commands/createas.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "access/htup_details.h"
#include "access/reloptions.h"
#include "access/sysattr.h"
#include "access/tableam.h"
#include "access/xact.h"
#include "access/xlog.h"
#include "catalog/heap.h"
#include "catalog/namespace.h"
#include "catalog/pg_trigger.h"
#include "catalog/pg_type.h"
#include "catalog/toasting.h"
#include "commands/alter.h"
#include "commands/createas.h"
#include "commands/defrem.h"
#include "commands/matview.h"
#include "commands/prepare.h"
#include "commands/tablecmds.h"
#include "commands/trigger.h"
#include "commands/view.h"
#include "executor/executor.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "parser/parse_clause.h"
#include "parser/mysql/mys_parse_utilcmd.h"
#include "rewrite/rewriteHandler.h"
#include "storage/smgr.h"
#include "tcop/tcopprot.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/rel.h"
#include "utils/rls.h"
#include "utils/snapmgr.h"
#include "tcop/utility.h"
#include "adapter/mysql/adapter.h"
#include "libpq/libpq-be.h"
#include "access/heaptoast.h"
#include "access/htup_details.h"
#include "access/tupdesc_details.h"
#include "executor/tuptable.h"


typedef struct
{
    DestReceiver pub;			/* publicly-known function pointers */
    IntoClause *into;			/* target relation specification */
    /* These fields are filled by intorel_startup: */
    Relation	rel;			/* relation to write to */
    ObjectAddress reladdr;		/* address of rel, for ExecCreateTableAs */
    CommandId	output_cid;		/* cmin to insert in output tuples */
    int			ti_options;		/* table_tuple_insert performance options */
    BulkInsertState bistate;	/* bulk insert state */
} DR_intorel;

/* utility functions for CTAS definition creation */
static ObjectAddress create_ctas_internal(List *attrList, IntoClause *into);
static ObjectAddress create_ctas_internal2(CreateStmt *create, IntoClause *into);
static ObjectAddress create_ctas_nodata(List *tlist, IntoClause *into);

/* DestReceiver routines for collecting data */
static void intorel_startup(DestReceiver *self, int operation, TupleDesc typeinfo, CommandTag commandTag);
static bool intorel_receive(TupleTableSlot *slot, DestReceiver *self, CommandTag commandTag);
static void intorel_shutdown(DestReceiver *self);
static void intorel_destroy(DestReceiver *self);

static void getOrigTableOidAndAttNum(char* colName, List* targetList, Oid* origTableOid, AttrNumber* attNumInOrigTable);
static void fillTupleDescOptions(TupleDesc source, List* targetList, TupleDesc target);
static void fillPlanStateOptions(struct PlanState* node, List* targetList, TupleDesc tupleDesc);

static Relation getRelation(TupleDesc tupleDesc);
static char* getNewColName(List* targetList, TupleDesc tupleDesc, char* colName);
static void fillAutoUpdateTrigger(ParseState *pstate, TupleDesc origTupleDesc, List* targetList, Relation newRelation);
static void fillPlanStateAutoUpdateTrigger(ParseState *pstate, struct PlanState* node, List* targetList, Relation newRelation);

/*
 * create_ctas_internal
 *
 * Internal utility used for the creation of the definition of a relation
 * created via CREATE TABLE AS or a materialized view.  Caller needs to
 * provide a list of attributes (ColumnDef nodes).
 */
static ObjectAddress
create_ctas_internal(List *attrList, IntoClause *into)
{
    CreateStmt *create = makeNode(CreateStmt);
    bool		is_matview;
    char		relkind;
    Datum		toast_options;
    static char *validnsps[] = HEAP_RELOPT_NAMESPACES;
    ObjectAddress intoRelationAddr;

    /* This code supports both CREATE TABLE AS and CREATE MATERIALIZED VIEW */
    is_matview = (into->viewQuery != NULL);
    relkind = is_matview ? RELKIND_MATVIEW : RELKIND_RELATION;

    /*
     * Create the target relation by faking up a CREATE TABLE parsetree and
     * passing it to DefineRelation.
     */
    create->relation = into->rel;
    create->tableElts = attrList;
    create->inhRelations = NIL;
    create->ofTypename = NULL;
    create->constraints = NIL;
    create->options = into->options;
    create->oncommit = into->onCommit;
    create->tablespacename = into->tableSpaceName;
    create->if_not_exists = false;
    create->accessMethod = into->accessMethod;

    /*
     * Create the relation.  (This will error out if there's an existing view,
     * so we don't need more code to complain if "replace" is false.)
     */
    intoRelationAddr = DefineRelation(create, relkind, InvalidOid, NULL, NULL);

    /*
     * If necessary, create a TOAST table for the target table.  Note that
     * NewRelationCreateToastTable ends with CommandCounterIncrement(), so
     * that the TOAST table will be visible for insertion.
     */
    CommandCounterIncrement();

    /* parse and validate reloptions for the toast table */
    toast_options = transformRelOptions((Datum) 0,
                                        create->options,
                                        "toast",
                                        validnsps,
                                        true, false);

    (void) heap_reloptions(RELKIND_TOASTVALUE, toast_options, true);

    NewRelationCreateToastTable(intoRelationAddr.objectId, toast_options);

    /* Create the "view" part of a materialized view. */
    if (is_matview)
    {
        /* StoreViewQuery scribbles on tree, so make a copy */
        Query	   *query = (Query *) copyObject(into->viewQuery);

        StoreViewQuery(intoRelationAddr.objectId, query, false);
        CommandCounterIncrement();
    }

    return intoRelationAddr;
}

static ObjectAddress
create_ctas_internal2(CreateStmt *create, IntoClause *into)
{
    //CreateStmt *create = makeNode(CreateStmt);
    bool		is_matview;
    char		relkind;
    Datum		toast_options;
    static char *validnsps[] = HEAP_RELOPT_NAMESPACES;
    ObjectAddress intoRelationAddr;

    /* This code supports both CREATE TABLE AS and CREATE MATERIALIZED VIEW */
    is_matview = (into->viewQuery != NULL);
    relkind = is_matview ? RELKIND_MATVIEW : RELKIND_RELATION;

    /*
     * Create the target relation by faking up a CREATE TABLE parsetree and
     * passing it to DefineRelation.
     */
    //create->relation = into->rel;
    //create->tableElts = attrList;
    //create->inhRelations = NIL;
    //create->ofTypename = NULL;
    //create->constraints = constraints;
    //create->options = into->options;
    //create->oncommit = into->onCommit;
    //create->tablespacename = into->tableSpaceName;
    //create->if_not_exists = false;
    //create->accessMethod = into->accessMethod;

    /*
     * Create the relation.  (This will error out if there's an existing view,
     * so we don't need more code to complain if "replace" is false.)
     */
    intoRelationAddr = DefineRelation(create, relkind, InvalidOid, NULL, NULL);

    /*
     * If necessary, create a TOAST table for the target table.  Note that
     * NewRelationCreateToastTable ends with CommandCounterIncrement(), so
     * that the TOAST table will be visible for insertion.
     */
    CommandCounterIncrement();

    /* parse and validate reloptions for the toast table */
    toast_options = transformRelOptions((Datum) 0,
                                        create->options,
                                        "toast",
                                        validnsps,
                                        true, false);

    (void) heap_reloptions(RELKIND_TOASTVALUE, toast_options, true);

    NewRelationCreateToastTable(intoRelationAddr.objectId, toast_options);

    /* Create the "view" part of a materialized view. */
    if (is_matview)
    {
        /* StoreViewQuery scribbles on tree, so make a copy */
        Query	   *query = (Query *) copyObject(into->viewQuery);

        StoreViewQuery(intoRelationAddr.objectId, query, false);
        CommandCounterIncrement();
    }

    return intoRelationAddr;
}


/*
 * create_ctas_nodata
 *
 * Create CTAS or materialized view when WITH NO DATA is used, starting from
 * the targetlist of the SELECT or view definition.
 */
static ObjectAddress
create_ctas_nodata(List *tlist, IntoClause *into)
{
    List	   *attrList;
    ListCell   *t,
               *lc;

    /*
     * Build list of ColumnDefs from non-junk elements of the tlist.  If a
     * column name list was specified in CREATE TABLE AS, override the column
     * names in the query.  (Too few column names are OK, too many are not.)
     */
    attrList = NIL;
    lc = list_head(into->colNames);
    foreach(t, tlist)
    {
        TargetEntry *tle = (TargetEntry *) lfirst(t);

        if (!tle->resjunk)
        {
            ColumnDef  *col;
            char	   *colname;

            if (lc)
            {
                colname = strVal(lfirst(lc));
                lc = lnext(into->colNames, lc);
            }
            else
                colname = tle->resname;

            col = makeColumnDef(colname,
                                exprType((Node *) tle->expr),
                                exprTypmod((Node *) tle->expr),
                                exprCollation((Node *) tle->expr));

            /*
             * It's possible that the column is of a collatable type but the
             * collation could not be resolved, so double-check.  (We must
             * check this here because DefineRelation would adopt the type's
             * default collation rather than complaining.)
             */
            if (!OidIsValid(col->collOid) &&
                type_is_collatable(col->typeName->typeOid))
                ereport(ERROR,
                        (errcode(ERRCODE_INDETERMINATE_COLLATION),
                         errmsg("no collation was derived for column \"%s\" with collatable type %s",
                                col->colname,
                                format_type_be(col->typeName->typeOid)),
                         errhint("Use the COLLATE clause to set the collation explicitly.")));

            attrList = lappend(attrList, col);
        }
    }

    if (lc != NULL)
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("too many column names were specified")));

    /* Create the relation definition using the ColumnDef list */
    return create_ctas_internal(attrList, into);
}


/*
 * ExecCreateTableAs -- execute a CREATE TABLE AS command
 */
ObjectAddress
ExecCreateTableAs(ParseState *pstate, CreateTableAsStmt *stmt,
                  ParamListInfo params, QueryEnvironment *queryEnv,
                  QueryCompletion *qc)
{
    Query	   *query = castNode(Query, stmt->query);
    IntoClause *into = stmt->into;
    bool		is_matview = (into->viewQuery != NULL);
    DestReceiver *dest;
    Oid			save_userid = InvalidOid;
    int			save_sec_context = 0;
    int			save_nestlevel = 0;
    ObjectAddress address;
    List	   *rewritten;
    PlannedStmt *plan;
    QueryDesc  *queryDesc;

    /* Check if the relation exists or not */
    if (CreateTableAsRelExists(stmt))
        return InvalidObjectAddress;

    /*
     * Create the tuple receiver object and insert info it will need
     */
    dest = CreateIntoRelDestReceiver(into);

    /*
     * The contained Query could be a SELECT, or an EXECUTE utility command.
     * If the latter, we just pass it off to ExecuteQuery.
     */
    if (query->commandType == CMD_UTILITY &&
        IsA(query->utilityStmt, ExecuteStmt))
    {
        ExecuteStmt *estmt = castNode(ExecuteStmt, query->utilityStmt);

        Assert(!is_matview);	/* excluded by syntax */
        ExecuteQuery(pstate, estmt, into, params, dest, qc);

        /* get object address that intorel_startup saved for us */
        address = ((DR_intorel *) dest)->reladdr;

        return address;
    }
    Assert(query->commandType == CMD_SELECT);

    /*
     * For materialized views, lock down security-restricted operations and
     * arrange to make GUC variable changes local to this command.  This is
     * not necessary for security, but this keeps the behavior similar to
     * REFRESH MATERIALIZED VIEW.  Otherwise, one could create a materialized
     * view not possible to refresh.
     */
    if (is_matview)
    {
        GetUserIdAndSecContext(&save_userid, &save_sec_context);
        SetUserIdAndSecContext(save_userid,
                               save_sec_context | SECURITY_RESTRICTED_OPERATION);
        save_nestlevel = NewGUCNestLevel();
    }

    if (into->skipData)
    {
        /*
         * If WITH NO DATA was specified, do not go through the rewriter,
         * planner and executor.  Just define the relation using a code path
         * similar to CREATE VIEW.  This avoids dump/restore problems stemming
         * from running the planner before all dependencies are set up.
         */
        address = create_ctas_nodata(query->targetList, into);
    }
    else
    {
        /*
         * Parse analysis was done already, but we still have to run the rule
         * rewriter.  We do not do AcquireRewriteLocks: we assume the query
         * either came straight from the parser, or suitable locks were
         * acquired by plancache.c.
         */
        rewritten = QueryRewrite(query);

        /* SELECT should never rewrite to more or less than one SELECT query */
        if (list_length(rewritten) != 1)
            elog(ERROR, "unexpected rewrite result for %s",
                 is_matview ? "CREATE MATERIALIZED VIEW" :
                 "CREATE TABLE AS SELECT");
        query = linitial_node(Query, rewritten);
        Assert(query->commandType == CMD_SELECT);

        /* plan the query */
        plan = pg_plan_query(query, pstate->p_sourcetext,
                             CURSOR_OPT_PARALLEL_OK, params);

        /*
         * Use a snapshot with an updated command ID to ensure this query sees
         * results of any previously executed queries.  (This could only
         * matter if the planner executed an allegedly-stable function that
         * changed the database contents, but let's do it anyway to be
         * parallel to the EXPLAIN code path.)
         */
        PushCopiedSnapshot(GetActiveSnapshot());
        UpdateActiveSnapshotCommandId();

        /* Create a QueryDesc, redirecting output to our tuple receiver */
        queryDesc = CreateQueryDesc(plan, pstate->p_sourcetext,
                                    GetActiveSnapshot(), InvalidSnapshot,
                                    dest, params, queryEnv, 0);

        /* call ExecutorStart to prepare the plan for execution */
        ExecutorStart(queryDesc, GetIntoRelEFlags(into));

        
        /* 针对MySQL的create table as 
         * 补充 not null属性，has def属性，
         * 补充 def 值
         * */
        queryDesc->tupDesc->constr = (TupleConstr *) palloc0(sizeof(TupleConstr));
        queryDesc->tupDesc->constr->defval = (AttrDefault *)palloc(queryDesc->tupDesc->natts * sizeof(AttrDefault));
        queryDesc->tupDesc->constr->num_defval = 0;
        fillPlanStateOptions(queryDesc->planstate, query->targetList, queryDesc->tupDesc);
        

        /* run the plan to completion */
        ExecutorRun(queryDesc, ForwardScanDirection, 0L, true);

        
        /* [> save the rowcount if we're given a qc to fill <] */
        /* if (qc) */
        /*     SetQueryCompletion(qc, CMDTAG_SELECT, queryDesc->estate->es_processed); */

        /* [> get object address that intorel_startup saved for us <] */
        /* address = ((DR_intorel *) dest)->reladdr; */

        if ((MyProcPort == NULL) || 
            (nodeTag(MyProcPort->protocol_handler) != T_MySQLProtocol))
        {
            /* save the rowcount if we're given a qc to fill */
            if (qc)
                SetQueryCompletion(qc, CMDTAG_SELECT, queryDesc->estate->es_processed);

            /* get object address that intorel_startup saved for us */
            address = ((DR_intorel *) dest)->reladdr;
        }
        else 
        {
            /* save the rowcount if we're given a qc to fill */
            if (qc)
                SetQueryCompletion(qc, 
                                   CMDTAG_CREATE_TABLE_AS, 
                                   queryDesc->estate->es_processed);

            /* get object address that intorel_startup saved for us */
            address = ((DR_intorel *) dest)->reladdr;

            /* 根据需要添加自动更新时间的触发器 */
            if ((queryDesc->planstate->scandesc != NULL))
            {
                Relation newRelation = table_open(address.objectId, AccessExclusiveLock);
                fillPlanStateAutoUpdateTrigger(pstate, queryDesc->planstate, query->targetList, newRelation);
                table_close(newRelation, AccessExclusiveLock);
            }
            else 
            {
                /* do nothing; */
            }
        }
        

        /* and clean up */
        ExecutorFinish(queryDesc);
        ExecutorEnd(queryDesc);

        FreeQueryDesc(queryDesc);

        PopActiveSnapshot();
    }

    if (is_matview)
    {
        /* Roll back any GUC changes */
        AtEOXact_GUC(false, save_nestlevel);

        /* Restore userid and security context */
        SetUserIdAndSecContext(save_userid, save_sec_context);
    }

    return address;
}

/*
 * GetIntoRelEFlags --- compute executor flags needed for CREATE TABLE AS
 *
 * This is exported because EXPLAIN and PREPARE need it too.  (Note: those
 * callers still need to deal explicitly with the skipData flag; since they
 * use different methods for suppressing execution, it doesn't seem worth
 * trying to encapsulate that part.)
 */
int
GetIntoRelEFlags(IntoClause *intoClause)
{
    int			flags = 0;

    if (intoClause->skipData)
        flags |= EXEC_FLAG_WITH_NO_DATA;

    return flags;
}

/*
 * CreateTableAsRelExists --- check existence of relation for CreateTableAsStmt
 *
 * Utility wrapper checking if the relation pending for creation in this
 * CreateTableAsStmt query already exists or not.  Returns true if the
 * relation exists, otherwise false.
 */
bool
CreateTableAsRelExists(CreateTableAsStmt *ctas)
{
    Oid			nspid;
    Oid			oldrelid;
    ObjectAddress address;
    IntoClause *into = ctas->into;

    nspid = RangeVarGetCreationNamespace(into->rel);

    oldrelid = get_relname_relid(into->rel->relname, nspid);
    if (OidIsValid(oldrelid))
    {
        if (!ctas->if_not_exists)
            ereport(ERROR,
                    (errcode(ERRCODE_DUPLICATE_TABLE),
                     errmsg("relation \"%s\" already exists",
                            into->rel->relname)));

        /*
         * The relation exists and IF NOT EXISTS has been specified.
         *
         * If we are in an extension script, insist that the pre-existing
         * object be a member of the extension, to avoid security risks.
         */
        ObjectAddressSet(address, RelationRelationId, oldrelid);
        checkMembershipInCurrentExtension(&address);

        /* OK to skip */
        ereport(NOTICE,
                (errcode(ERRCODE_DUPLICATE_TABLE),
                 errmsg("relation \"%s\" already exists, skipping",
                        into->rel->relname)));
        return true;
    }

    /* Relation does not exist, it can be created */
    return false;
}

/*
 * CreateIntoRelDestReceiver -- create a suitable DestReceiver object
 *
 * intoClause will be NULL if called from CreateDestReceiver(), in which
 * case it has to be provided later.  However, it is convenient to allow
 * self->into to be filled in immediately for other callers.
 */
DestReceiver *
CreateIntoRelDestReceiver(IntoClause *intoClause)
{
    DR_intorel *self = (DR_intorel *) palloc0(sizeof(DR_intorel));

    self->pub.receiveSlot = intorel_receive;
    self->pub.rStartup = intorel_startup;
    self->pub.rShutdown = intorel_shutdown;
    self->pub.rDestroy = intorel_destroy;
    self->pub.mydest = DestIntoRel;
    self->into = intoClause;
    /* other private fields will be set during intorel_startup */

    return (DestReceiver *) self;
}

/*
 * intorel_startup --- executor startup
 */
static void
intorel_startup(DestReceiver *self, int operation, TupleDesc typeinfo, CommandTag commandTag)
{
    DR_intorel *myState = (DR_intorel *) self;
    IntoClause *into = myState->into;
    bool		is_matview;
    List	   *attrList;
    List *stmts;    /* for MySQL */
    CreateStmt *createStmt;      /* for MySQL */
    char *queryString;  /* for MySQL */
    ObjectAddress intoRelationAddr;
    Relation	intoRelationDesc;
    ListCell   *lc;
    int			attnum;

    Assert(into != NULL);		/* else somebody forgot to set it */

    /* This code supports both CREATE TABLE AS and CREATE MATERIALIZED VIEW */
    is_matview = (into->viewQuery != NULL);

    /*
     * Build column definitions using "pre-cooked" type and collation info. If
     * a column name list was specified in CREATE TABLE AS, override the
     * column names derived from the query.  (Too few column names are OK, too
     * many are not.)
     */
    attrList = NIL;
    stmts = NIL;

    
    createStmt = NULL; 
    if ((MyProcPort == NULL) || 
        ((MyProcPort != NULL) && 
         (nodeTag(MyProcPort->protocol_handler) != T_MySQLProtocol)))
    {
        lc = list_head(into->colNames);
        for (attnum = 0; attnum < typeinfo->natts; attnum++)
        {
            Form_pg_attribute attribute = TupleDescAttr(typeinfo, attnum);
            ColumnDef  *col;
            char	   *colname;

            if (lc)
            {
                colname = strVal(lfirst(lc));
                lc = lnext(into->colNames, lc);
            }
            else
                colname = NameStr(attribute->attname);

            col = makeColumnDef(colname, 
                                attribute->atttypid,
                                attribute->atttypmod,
                                attribute->attcollation);

            /*
             * It's possible that the column is of a collatable type but the
             * collation could not be resolved, so double-check.  (We must check
             * this here because DefineRelation would adopt the type's default
             * collation rather than complaining.)
             */
            if (!OidIsValid(col->collOid) &&
                type_is_collatable(col->typeName->typeOid))
                ereport(ERROR,
                        (errcode(ERRCODE_INDETERMINATE_COLLATION),
                         errmsg("no collation was derived for column \"%s\" with collatable type %s",
                                col->colname,
                                format_type_be(col->typeName->typeOid)),
                         errhint("Use the COLLATE clause to set the collation explicitly.")));

            attrList = lappend(attrList, col);
        }

        if (lc != NULL)
            ereport(ERROR,
                    (errcode(ERRCODE_SYNTAX_ERROR),
                     errmsg("too many column names were specified")));
    }
    else 
    {
        lc = list_head(into->colNames);
        if (lc == NULL)
        {
            lc = list_head(into->colNames);
            for (attnum = 0; attnum < typeinfo->natts; attnum++)
            {
                Form_pg_attribute attribute = TupleDescAttr(typeinfo, attnum);
                ColumnDef  *col;
                char	   *colname;

                if (lc)
                {
                    colname = strVal(lfirst(lc));
                    lc = lnext(into->colNames, lc);
                }
                else
                    colname = NameStr(attribute->attname);

                col = mys_makeColumnDef(colname, 
                                        attribute->attnotnull,
                                        attribute->atttypid,
                                        attribute->atttypmod,
                                        attribute->attcollation);

                if (typeinfo->constr != NULL)
                {
                    char* nextvalFunc = "funcid 1574";     // func of nextval
                    int nextvalFuncLen = strlen(nextvalFunc);
                    char* generated = "opfuncid 141";     
                    int generatedLen = strlen(generated);
                    for (int j = 0; j < typeinfo->constr->num_defval; j++)
                    {
                        if (typeinfo->constr->defval[j].adnum == (attnum + 1))
                        {
                            char* defVal = typeinfo->constr->defval[j].adbin;
                            int defValLen = strlen(defVal);

                            col->cooked_default = stringToNode(defVal);

                            for (int k = 0; k < defValLen; k++)
                            {
                                if (strncasecmp((defVal + k), nextvalFunc, nextvalFuncLen) == 0)
                                {
                                    // MySQL def： 0
                                    col->cooked_default = (Node*)makeConst(INT4OID, 
                                                                           -1, 
                                                                           InvalidOid, 
                                                                           sizeof(int32), 
                                                                           Int32GetDatum(0), 
                                                                           false, 
                                                                           true);
                                    break;
                                }
                                else if (strncasecmp((defVal + k), generated, generatedLen) == 0)
                                {
                                    // MySQL def： NULL
                                    col->cooked_default = NULL;
                                    break;
                                }
                                else 
                                {
                                    continue;
                                }
                            }
                        }
                    }
                }

                /*
                 * It's possible that the column is of a collatable type but the
                 * collation could not be resolved, so double-check.  (We must check
                 * this here because DefineRelation would adopt the type's default
                 * collation rather than complaining.)
                 */
                if (!OidIsValid(col->collOid) &&
                    type_is_collatable(col->typeName->typeOid))
                    ereport(ERROR,
                            (errcode(ERRCODE_INDETERMINATE_COLLATION),
                             errmsg("no collation was derived for column \"%s\" with collatable type %s",
                                    col->colname,
                                    format_type_be(col->typeName->typeOid)),
                             errhint("Use the COLLATE clause to set the collation explicitly.")));

                attrList = lappend(attrList, col);
            }

            if (lc != NULL)
                ereport(ERROR,
                        (errcode(ERRCODE_SYNTAX_ERROR),
                         errmsg("too many column names were specified")));
        }
        else 
        {
            Node *n = (Node *)(lfirst(lc));
            if (IsA(n, ColumnDef))
            {
                ListCell *lc;
                createStmt = makeNode(CreateStmt);
                queryString = "create table(columnDefs) as ...";
                createStmt->relation = into->rel;
                createStmt->inhRelations = NIL;
                createStmt->ofTypename = NULL;
                createStmt->options = into->options;
                createStmt->oncommit = into->onCommit;
                createStmt->tablespacename = into->tableSpaceName;
                createStmt->if_not_exists = false;
                createStmt->accessMethod = into->accessMethod;
                foreach (lc, into->colNames)
                {
                    Node *node = (Node *)lfirst(lc);
                    if (node->type == T_ColumnDef)
                    {
                        ColumnDef *columnDef = (ColumnDef *)node;
                        bool found = false;
                        for (int i = 0; i < typeinfo->natts; i++)
                        {
                            Form_pg_attribute att = TupleDescAttr(typeinfo, i);
                            if ((strlen(columnDef->colname) == strlen(att->attname.data)) && 
                                (strncasecmp(columnDef->colname, att->attname.data, strlen(columnDef->colname)) == 0))
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            createStmt->tableElts = lappend(createStmt->tableElts, node);
                        }
                    }
                }
                for (int i = 0; i < typeinfo->natts; i++)
                {
                    Form_pg_attribute att = TupleDescAttr(typeinfo, i);
                    ListCell *lc;
                    ColumnDef *columnDef = NULL;
                    bool found = false;
                    foreach (lc, into->colNames)
                    {
                        Node *node = (Node *)lfirst(lc);
                        if (node->type == T_ColumnDef)
                        {
                            columnDef = (ColumnDef *)node;
                            if ((strlen(columnDef->colname) == strlen(att->attname.data)) && 
                                (strncasecmp(columnDef->colname, att->attname.data, strlen(columnDef->colname)) == 0))
                            {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        columnDef = makeColumnDef(att->attname.data, att->atttypid, att->atttypmod, att->attcollation);
                    }
                    createStmt->tableElts = lappend(createStmt->tableElts, (Node *)columnDef);
                }
                foreach (lc, into->colNames)
                {
                    Node *node = (Node *)lfirst(lc);
                    if (node->type == T_Constraint)
                    {
                        createStmt->tableElts = lappend(createStmt->tableElts, node);
                    }
                }
                //for (int i = 0; i < typeinfo->natts; i++)
                //{
                //    Form_pg_attribute att = TupleDescAttr(typeinfo, i);
                //    int j = 0;
                //    foreach (lc, createStmt->tableElts)
                //    {
                //        Node *node = (Node *)lfirst(lc);
                //        ++j;
                //        if (node->type == T_ColumnDef)
                //        {
                //            ColumnDef *columnDef = (ColumnDef *)node;
                //            if ((strlen(columnDef->colname) == strlen(att->attname.data)) && 
                //                (strncasecmp(columnDef->colname, att->attname.data, strlen(columnDef->colname)) == 0))
                //            {
                //                att->attnum = j;
                //            }
                //        }
                //    }
                //}
                stmts = mys_transformCreateStmt(createStmt, queryString);
            }
            else 
            {
                ereport(ERROR,
                        (errcode(ERRCODE_SYNTAX_ERROR),
                         errmsg("unsupport create as syyntax!")));
            } 
        }
    }
    

    /*
     * Actually create the target table
     */
    
    if ((MyProcPort == NULL) || 
        ((MyProcPort != NULL) && 
         (nodeTag(MyProcPort->protocol_handler) != T_MySQLProtocol)))
    {
        intoRelationAddr = create_ctas_internal(attrList, into);
    }
    else 
    {
        if (createStmt == NULL)
        {
            intoRelationAddr = create_ctas_internal(attrList, into);
        }
        else 
        {
            while (stmts != NIL)
            {
                Node *stmt = (Node *) linitial(stmts);
                stmts = list_delete_first(stmts);
                if (IsA(stmt, CreateStmt))
                {
                    CreateStmt *cstmt = (CreateStmt *) stmt;
                    intoRelationAddr = create_ctas_internal2(cstmt, into);
                }
                else
                {
                    /*
                     * Recurse for anything else.  Note the recursive
                     * call will stash the objects so created into our
                     * event trigger context.
                     */
                    PlannedStmt *wrapper;
                    wrapper = makeNode(PlannedStmt);
                    wrapper->commandType = CMD_UTILITY;
                    wrapper->canSetTag = false;
                    wrapper->utilityStmt = stmt;
                    //wrapper->stmt_location = pstmt->stmt_location;
                    //wrapper->stmt_len = pstmt->stmt_len;
                    ProcessUtility(wrapper,
                                   queryString,
                                   false,
                                   PROCESS_UTILITY_SUBCOMMAND,
                                   NULL, //params,
                                   NULL,
                                   None_Receiver,
                                   NULL);
                }
            }
        }
    }
    

    /*
     * Finally we can open the target table
     */
    intoRelationDesc = table_open(intoRelationAddr.objectId, AccessExclusiveLock);

    /*
     * Make sure the constructed table does not have RLS enabled.
     *
     * check_enable_rls() will ereport(ERROR) itself if the user has requested
     * something invalid, and otherwise will return RLS_ENABLED if RLS should
     * be enabled here.  We don't actually support that currently, so throw
     * our own ereport(ERROR) if that happens.
     */
    if (check_enable_rls(intoRelationAddr.objectId, InvalidOid, false) == RLS_ENABLED)
        ereport(ERROR,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("policies not yet implemented for this command")));

    /*
     * Tentatively mark the target as populated, if it's a matview and we're
     * going to fill it; otherwise, no change needed.
     */
    if (is_matview && !into->skipData)
        SetMatViewPopulatedState(intoRelationDesc, true);

    /*
     * Fill private fields of myState for use by later routines
     */
    myState->rel = intoRelationDesc;
    myState->reladdr = intoRelationAddr;
    myState->output_cid = GetCurrentCommandId(true);
    myState->ti_options = TABLE_INSERT_SKIP_FSM;

    /*
     * If WITH NO DATA is specified, there is no need to set up the state for
     * bulk inserts as there are no tuples to insert.
     */
    if (!into->skipData)
        myState->bistate = GetBulkInsertState();
    else
        myState->bistate = NULL;

    /*
     * Valid smgr_targblock implies something already wrote to the relation.
     * This may be harmless, but this function hasn't planned for it.
     */
    Assert(RelationGetTargetBlock(intoRelationDesc) == InvalidBlockNumber);
}

/*
 * intorel_receive --- receive one tuple
 */
static bool
intorel_receive(TupleTableSlot *slot, DestReceiver *self, CommandTag commandTag)
{
    DR_intorel *myState = (DR_intorel *) self;

    /* Nothing to insert if WITH NO DATA is specified. */
    if (!myState->into->skipData)
    {
        
        if ((MyProcPort == NULL) || 
            ((MyProcPort != NULL) && 
             (nodeTag(MyProcPort->protocol_handler) != T_MySQLProtocol)))
        {
            /*
             * Note that the input slot might not be of the type of the target
             * relation. That's supported by table_tuple_insert(), but slightly
             * less efficient than inserting with the right slot - but the
             * alternative would be to copy into a slot of the right type, which
             * would not be cheap either. This also doesn't allow accessing per-AM
             * data (say a tuple's xmin), but since we don't do that here...
             */
            table_tuple_insert(myState->rel,
                               slot,
                               myState->output_cid,
                               myState->ti_options,
                               myState->bistate);
        }
        else
        {
            bool matched = true;
            if (myState->rel->rd_att->natts == slot->tts_tupleDescriptor->natts)
            {
                for (int i = 0; i < myState->rel->rd_att->natts; i++)
                {
                    Form_pg_attribute relAtt = &(myState->rel->rd_att->attrs[i]);
                    Form_pg_attribute slotAtt = TupleDescAttr(slot->tts_tupleDescriptor, i);
                    if ((strlen(relAtt->attname.data) != strlen(slotAtt->attname.data)) || 
                        (strncasecmp(relAtt->attname.data, 
                                     slotAtt->attname.data, 
                                     strlen(relAtt->attname.data)) != 0) || 
                        (relAtt->atttypid != slotAtt->atttypid))
                    {
                        matched = false;
                        break;
                    }
                }
            }
            else
            {
                matched = false;
            }

            if (matched)
            {
                /*
                 * Note that the input slot might not be of the type of the target
                 * relation. That's supported by table_tuple_insert(), but slightly
                 * less efficient than inserting with the right slot - but the
                 * alternative would be to copy into a slot of the right type, which
                 * would not be cheap either. This also doesn't allow accessing per-AM
                 * data (say a tuple's xmin), but since we don't do that here...
                 */
                table_tuple_insert(myState->rel,
                                   slot,
                                   myState->output_cid,
                                   myState->ti_options,
                                   myState->bistate);
            }
            else 
            {
                // oldSlot: id, nm, dsc
                // newSlot: id1, dsc1, id, nm, dsc
                TupleTableSlot *newSlot;
                TupleDesc newTupleDesc;
                Datum *newTtsValues;
                bool *newTtsIsNull;
                MemoryContext oldContext;

                oldContext = MemoryContextSwitchTo(slot->tts_mcxt);
                newTupleDesc = (TupleDesc)palloc(sizeof(TupleDescData) + 
                                                 (myState->rel->rd_att->natts * 
                                                  sizeof(FormData_pg_attribute)));
                newTupleDesc->natts = myState->rel->rd_att->natts;
                newSlot = MakeTupleTableSlot(newTupleDesc, slot->tts_ops);
                MemoryContextSwitchTo(oldContext);
                newSlot->type = slot->type;
                newSlot->tts_flags = slot->tts_flags;
                newSlot->tts_nvalid = slot->tts_nvalid;
                newSlot->tts_mcxt = slot->tts_mcxt;
                newSlot->tts_tid = slot->tts_tid;
                newSlot->tts_tableOid = slot->tts_tableOid;
                newTupleDesc->natts = myState->rel->rd_att->natts;
                newTupleDesc->tdtypeid = slot->tts_tupleDescriptor->tdtypeid;
                newTupleDesc->tdtypmod = slot->tts_tupleDescriptor->tdtypmod;
                //newTupleDesc->tdrefcount = slot->tts_tupleDescriptor->tdrefcount;
                newTupleDesc->constr = NULL;    // TODO
                for (int i = 0; i < myState->rel->rd_att->natts; i++)
                {
                    Form_pg_attribute srcAtt;
                    Form_pg_attribute relAtt = &(myState->rel->rd_att->attrs[i]);
                    srcAtt = relAtt;
                    for (int j = 0; j < slot->tts_tupleDescriptor->natts; j++)
                    {
                        Form_pg_attribute slotAtt = TupleDescAttr(slot->tts_tupleDescriptor, j);
                        if ((strlen(relAtt->attname.data) == strlen(slotAtt->attname.data)) && 
                            (strncasecmp(relAtt->attname.data, 
                                         slotAtt->attname.data, 
                                         strlen(relAtt->attname.data)) == 0) && 
                            (relAtt->atttypid == slotAtt->atttypid))
                        {
                            srcAtt = slotAtt;
                            break;
                        }
                    }
                    newTupleDesc->attrs[i].attrelid = srcAtt->attrelid;
                    memcpy(newTupleDesc->attrs[i].attname.data, 
                           srcAtt->attname.data,
                           (strlen(srcAtt->attname.data) + 1));
                    newTupleDesc->attrs[i].atttypid = srcAtt->atttypid;
                    newTupleDesc->attrs[i].attstattarget = srcAtt->attstattarget;
                    newTupleDesc->attrs[i].attlen = srcAtt->attlen;
                    newTupleDesc->attrs[i].attnum = i + 1;
                    newTupleDesc->attrs[i].attndims = srcAtt->attndims;
                    newTupleDesc->attrs[i].attndims = srcAtt->attndims;
                    newTupleDesc->attrs[i].attcacheoff = srcAtt->attcacheoff;
                    newTupleDesc->attrs[i].atttypmod = srcAtt->atttypmod;
                    newTupleDesc->attrs[i].attbyval = srcAtt->attbyval;
                    newTupleDesc->attrs[i].attalign = srcAtt->attalign;
                    newTupleDesc->attrs[i].attstorage = srcAtt->attstorage;
                    newTupleDesc->attrs[i].attcompression = srcAtt->attcompression;
                    newTupleDesc->attrs[i].attnotnull = srcAtt->attnotnull;
                    newTupleDesc->attrs[i].atthasdef = srcAtt->atthasdef;
                    newTupleDesc->attrs[i].atthasmissing = srcAtt->atthasmissing;
                    newTupleDesc->attrs[i].attidentity = srcAtt->attidentity;
                    newTupleDesc->attrs[i].attgenerated = srcAtt->attgenerated;
                    newTupleDesc->attrs[i].attisdropped = srcAtt->attisdropped;
                    newTupleDesc->attrs[i].attislocal = srcAtt->attislocal;
                    newTupleDesc->attrs[i].attinhcount = srcAtt->attinhcount;
                    newTupleDesc->attrs[i].attcollation = srcAtt->attcollation;
#ifdef CATALOG_VARLEN			/* variable-length fields start here */
                    newTupleDesc->attrs[i].attacl[0] = srcAtt->attacl[0];
                    newTupleDesc->attrs[i].attoptions[0] = srcAtt->attoptions[0];
                    newTupleDesc->attrs[i].attfdwoptions[0] = srcAtt->attfdwoptions[0];
                    newTupleDesc->attrs[i].attmissingval = srcAtt->attmissingval;
#endif
                }
                newTtsValues = newSlot->tts_values;
                newTtsIsNull = newSlot->tts_isnull;
                for (int i = 0; i < myState->rel->rd_att->natts; i++)
                {
                    Form_pg_attribute relAtt;
                    Oid typeInputFunc;
                    Oid	typioparam;
                    bool found;
                    relAtt = &(myState->rel->rd_att->attrs[i]);
                    getTypeInputInfo(relAtt->atttypid, &typeInputFunc, &typioparam);
                    found = false;
                    for (int j = 0; j < slot->tts_tupleDescriptor->natts; j++)
                    {
                        Form_pg_attribute slotAtt = TupleDescAttr(slot->tts_tupleDescriptor, j);
                        if ((strlen(relAtt->attname.data) == strlen(slotAtt->attname.data)) && 
                            (strncasecmp(relAtt->attname.data, 
                                         slotAtt->attname.data, 
                                         strlen(relAtt->attname.data)) == 0))
                        {
                            char *columnVal;
                            bool is_null;
                            Datum data;
                            data = slot_getattr(slot, (j + 1), &is_null);
                            if (!is_null)
                            {
                                Oid typeOutputFunc;
                                bool typisvarlena;
                                getTypeOutputInfo(slotAtt->atttypid, 
                                                  &typeOutputFunc, 
                                                  &typisvarlena);
                                columnVal = OidOutputFunctionCall(typeOutputFunc, 
                                                                  data);
                            }
                            else 
                            {
                                columnVal = NULL;
                            }
                            newTtsValues[i] = OidInputFunctionCall(typeInputFunc, 
                                                                   columnVal, 
                                                                   typioparam, 
                                                                   relAtt->atttypmod);
                            newTtsIsNull[i] = slot->tts_isnull[j];
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        /* 如果有默认值，此时无法计算出实际值 */
                        newTtsValues[i] = OidInputFunctionCall(typeInputFunc, 
                                                               NULL, 
                                                               typioparam, 
                                                               relAtt->atttypmod);
                        newTtsIsNull[i] = true;
                    }
                }

                /*
                 * Note that the input slot might not be of the type of the target
                 * relation. That's supported by table_tuple_insert(), but slightly
                 * less efficient than inserting with the right slot - but the
                 * alternative would be to copy into a slot of the right type, which
                 * would not be cheap either. This also doesn't allow accessing per-AM
                 * data (say a tuple's xmin), but since we don't do that here...
                 */
                table_tuple_insert(myState->rel,
                                   newSlot,
                                   myState->output_cid,
                                   myState->ti_options,
                                   myState->bistate);

                ReleaseTupleDesc(newTupleDesc);
            }
        }
        

        ///*
        // * Note that the input slot might not be of the type of the target
        // * relation. That's supported by table_tuple_insert(), but slightly
        // * less efficient than inserting with the right slot - but the
        // * alternative would be to copy into a slot of the right type, which
        // * would not be cheap either. This also doesn't allow accessing per-AM
        // * data (say a tuple's xmin), but since we don't do that here...
        // */
        //table_tuple_insert(myState->rel,
        //                   //slot,
        //                   newSlot,
        //                   myState->output_cid,
        //                   myState->ti_options,
        //                   myState->bistate);

        //
        /* 比较复杂，相关数据很难获取到 */
        /* 同时，Halo中，create as本身约束索引等都是被丢掉的， */
        /* 如果新表中某字段有约束，而基表中的数据不符合该约束，就会失败。 */
        //if ((MyProcPort != NULL) && 
        //    (nodeTag(MyProcPort->protocol_handler) == T_MySQLProtocol))
        //{
        //    ResultRelInfo *resultRelInfo;
        //    EState *estate;
        //    bool specConflict;
        //    List *arbiterIndexes;
        //    ListCell *lc;
        //    int i = 0;

        //    resultRelInfo->ri_NumIndices = 0;
        //    foreach (lc, myState->into->colNames)
        //    {
        //        Node *node = (Node *)lfirst(lc);
        //        if (node->type == T_Constraint)
        //        {
        //            Constraint *constraint = (Constraint *)node;
        //            if ((constraint->contype == CONSTR_PRIMARY) || 
        //                (constraint->contype == CONSTR_KEY) || 
        //                (constraint->contype == CONSTR_UNIQUE) ||
        //                (constraint->contype == CONSTR_FOREIGN))
        //            {
        //                resultRelInfo->ri_NumIndices += 1;
        //            }
        //        }
        //    }
        //    resultRelInfo->ri_IndexRelationDescs = &(myState->rel);
        //    resultRelInfo->ri_IndexRelationInfo = (IndexInfo **) palloc(resultRelInfo->ri_NumIndices * sizeof(IndexInfo *));
        //    foreach (lc, myState->into->colNames)
        //    {
        //        Node *node = (Node *)lfirst(lc);
        //        if (node->type == T_Constraint)
        //        {
        //            Constraint *constraint = (Constraint *)node;
        //            if ((constraint->contype == CONSTR_PRIMARY) || 
        //                (constraint->contype == CONSTR_KEY) || 
        //                (constraint->contype == CONSTR_UNIQUE) || 
        //                (constraint->contype == CONSTR_FOREIGN))
        //            {
        //                IndexInfo *indexInfo = resultRelInfo->ri_IndexRelationInfo[i++]; 
        //                indexInfo->type = T_IndexInfo;
        //                indexInfo->ii_NumIndexAttrs = list_length(constraint->keys);
        //                indexInfo->ii_NumIndexKeyAttrs = list_length(constraint->keys);
        //                // constraint->keys: indexParam = (IndexElem *)lfirst(lc);
        //                //indexInfo->ii_IndexAttrNumbers[0] = constraint
        //                indexInfo->ii_Expressions = constraint->cooked_expr;
        //                indexInfo->ii_Unique = (constraint->contype == CONSTR_UNIQUE);
        //                indexInfo->ii_ReadyForInserts = true;
        //            }
        //        }
        //    }
        //    resultRelInfo->ri_RelationDesc = myState->rel;
        //    specConflict = false;
        //    ExecInsertIndexTuples(resultRelInfo,
        //                          slot, 
        //                          estate, 
        //                          false, 
        //                          true,
        //                          &specConflict,
        //                          arbiterIndexes);
        //}
        //
    }

    /* We know this is a newly created relation, so there are no indexes */

    return true;
}

/*
 * intorel_shutdown --- executor end
 */
static void
intorel_shutdown(DestReceiver *self)
{
    DR_intorel *myState = (DR_intorel *) self;
    IntoClause *into = myState->into;

    if (!into->skipData)
    {
        FreeBulkInsertState(myState->bistate);
        table_finish_bulk_insert(myState->rel, myState->ti_options);
    }

    /* close rel, but keep lock until commit */
    table_close(myState->rel, NoLock);
    myState->rel = NULL;
}

/*
 * intorel_destroy --- release DestReceiver object
 */
static void
intorel_destroy(DestReceiver *self)
{
    pfree(self);
}

static void
getOrigTableOidAndAttNum(char* colName, List* targetList, Oid* origTableOid, 
                         AttrNumber* attNumInOrigTable)
{
    ListCell *tc;
    int colNameLen = strlen(colName);

    *origTableOid = InvalidOid;
    *attNumInOrigTable = 0;

    foreach(tc, targetList)
    {
        TargetEntry *tle = (TargetEntry *) lfirst(tc);
        char* origColName = tle->resname;
        int origColNameLen = strlen(origColName);
        if (colNameLen == origColNameLen)
        {
            if (strncasecmp(colName, origColName, colNameLen) == 0)
            {
                *origTableOid = tle->resorigtbl;
                *attNumInOrigTable = tle->resorigcol;
                break;
            }
            else 
            {
                continue;
            }
        }
        else 
        {
            continue;
        }
    }
}

static void
fillTupleDescOptions(TupleDesc source, List* targetList, TupleDesc target)
{
    if (source == NULL)
    {
        return;
    }

    for (int i = 0; i < target->natts; i++)
    {
        char* colName = NameStr(target->attrs[i].attname);
        AttrNumber attNum = target->attrs[i].attnum;
        Oid origTableOid;
        AttrNumber attNumInOrigTable;
        getOrigTableOidAndAttNum(colName, targetList, &origTableOid, &attNumInOrigTable);
        if (origTableOid != InvalidOid)
        {
            for (int j = 0; j < source->natts; j++)
            {
                if ((source->attrs[j].attrelid == origTableOid) && 
                    (source->attrs[j].attnum == attNumInOrigTable ))
                {
                    target->attrs[i].attnotnull = source->attrs[j].attnotnull;
                    target->attrs[i].atthasdef = source->attrs[j].atthasdef;
                    if (target->attrs[i].atthasdef)
                    {
                        for (int k = 0; k < source->constr->num_defval; k++)
                        {
                            if (source->constr->defval[k].adnum == attNumInOrigTable)
                            {
                                target->constr->defval[target->constr->num_defval].adbin = pstrdup(source->constr->defval[k].adbin);
                                target->constr->defval[target->constr->num_defval].adnum = attNum;
                                target->constr->num_defval += 1;
                                break;
                            }
                            else 
                            {
                                continue;
                            }
                        }
                    }
                    break;
                }
                else 
                {
                    continue;
                }
            }
        }
        else 
        {
            continue;
        }
    }
}

static void
fillPlanStateOptions(struct PlanState* node, List* targetList, TupleDesc tupleDesc)
{
    if (node == NULL)
    {
        return;
    }

    fillTupleDescOptions(node->scandesc, targetList, tupleDesc);
    fillPlanStateOptions(node->lefttree, targetList, tupleDesc);
    fillPlanStateOptions(node->righttree, targetList, tupleDesc);
}

static Relation 
getRelation(TupleDesc tupleDesc)
{
    for (int i = 0; i < tupleDesc->natts; i++)
    {
        Oid relId = tupleDesc->attrs[0].attrelid;
        return RelationIdGetRelation(relId);
    }

    return NULL;
}

static char* 
getNewColName(List* targetList, TupleDesc tupleDesc, char* colName)
{
    char* newColName;
    Oid origTableOid;
    AttrNumber origAttNum;
    int colNameLen;
    ListCell *tc;

    newColName = NULL;
    origTableOid = InvalidOid;
    origAttNum = 0;
    colNameLen = strlen(colName);
    for (int i = 0; i < tupleDesc->natts; i++)
    {
        char* attName = NameStr(tupleDesc->attrs[i].attname);
        int attNameLen = strlen(attName);
        if ((attNameLen == colNameLen) && 
            (strncasecmp(attName, colName, attNameLen) == 0))
        {
            origTableOid = tupleDesc->attrs[i].attrelid;
            origAttNum = tupleDesc->attrs[i].attnum;
            break;
        }
        else 
        {
            continue;
        }
    }
    
    foreach(tc, targetList)
    {
        TargetEntry *tle = (TargetEntry *) lfirst(tc);
        if ((tle->resorigtbl == origTableOid) && 
            (tle->resorigcol == origAttNum))
        {
            newColName = tle->resname;
        }
        else 
        {
            continue;
        }
    }

    return newColName;
}

static void
fillAutoUpdateTrigger(ParseState *pstate, TupleDesc origTupleDesc, 
                      List* targetList, Relation newRelation)
{
    Relation origRelation;
    Oid namespaceID;
    char *namespaceName;
    char* newRelName;
    //MemoryContext perTupCxt;

    if (origTupleDesc == NULL)
    {
        return;
    }

    origRelation = getRelation(origTupleDesc);
    if (origRelation == NULL)
    {
        return;
    }

    namespaceID = RelationGetNamespace(newRelation);
    namespaceName = get_namespace_name(namespaceID);
    newRelName = RelationGetRelationName(newRelation);
    //perTupCxt = NULL;

    for (int i = 1; i < origRelation->rd_att->natts; i++)
    {
        if (!origRelation->rd_att->attrs[i].attisdropped && 
            existAutoUpdateTrigOnThisAtt(origRelation, &(origRelation->rd_att->attrs[i])))
        {
            char *newColName;
            char *origColName;
            Form_pg_attribute origAttrForm;
            CreateFunctionStmt *createFuncStmt;
            CreateTrigStmt *createTrigStmt;
            AlterObjectDependsStmt *alterDependStmt1;
            AlterObjectDependsStmt *alterDependStmt2;
            //MemoryContext oldcxt;

            //oldcxt = MemoryContextSwitchTo(perTupCxt);

            origAttrForm = &(origRelation->rd_att->attrs[i]);
            origColName = NameStr(origAttrForm->attname);
            newColName = getNewColName(targetList, origTupleDesc, origColName);
            if (newColName == NULL)
            {
                continue;
            }

            createFuncStmt = createAutoUpdateTimeStampTriggerFunc(namespaceName, 
                                                                  newRelName, 
                                                                  newColName);
            CreateFunction(pstate, createFuncStmt);

            createTrigStmt = createAutoUpdateTimeStampTrigger(namespaceName, 
                                                              newRelName, 
                                                              newColName);
            CreateTrigger(createTrigStmt,
                          NULL, newRelation->rd_id, InvalidOid,
                          InvalidOid, InvalidOid, InvalidOid,
                          InvalidOid, NULL, false, false);

            alterDependStmt1 = bindTriggerFunctionToTrigger(createTrigStmt->funcname, 
                                                            createTrigStmt->relation, 
                                                            createTrigStmt->trigname);
            ExecAlterObjectDependsStmt(alterDependStmt1, NULL);

            alterDependStmt2 = bindTriggerToColumn(createTrigStmt->relation, 
                                                   createTrigStmt->trigname, 
                                                   newColName);
            ExecAlterObjectDependsStmt(alterDependStmt2, NULL);

            //MemoryContextSwitchTo(oldcxt);
            //MemoryContextReset(perTupCxt);
        }
    }
    
    table_close(origRelation, NoLock);
    //if (perTupCxt != NULL)
    //{
    //    MemoryContextDelete(perTupCxt);
    //}
}

static void
fillPlanStateAutoUpdateTrigger(ParseState *pstate, struct PlanState* node, 
                               List* targetList, Relation newRelation)
{
    if (node == NULL)
    {
        return;
    }

    fillAutoUpdateTrigger(pstate, node->scandesc, targetList, newRelation);
    fillPlanStateAutoUpdateTrigger(pstate, node->lefttree, targetList, newRelation);
    fillPlanStateAutoUpdateTrigger(pstate, node->righttree, targetList, newRelation);
}

