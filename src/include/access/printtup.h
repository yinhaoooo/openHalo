/*-------------------------------------------------------------------------
 *
 * printtup.h
 *
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/access/printtup.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PRINTTUP_H
#define PRINTTUP_H

#include "utils/portal.h"
#include "postgres_ext.h"
#include "fmgr.h"
#include "tcop/dest.h"
#include "lib/stringinfo.h"
#include "utils/palloc.h"

typedef struct
{/* Per-attribute information */
    Oid typoutput;/* Oid for the type's text output fn */
    Oid typsend;/* Oid for the type's binary output fn */
    bool typisvarlena;/* is it varlena (ie possibly toastable)? */
    int16 format;/* format code for this column */
    FmgrInfo finfo;/* Precomputed call info for output fn */
} PrinttupAttrInfo;

typedef struct
{
    DestReceiver pub;/* publicly-known function pointers */
    Portal portal;/* the Portal we are printing from */
    bool sendDescrip;/* send RowDescription at startup? */
    TupleDesc attrinfo;/* The attr info we are set up for */
    int nattrs;
    PrinttupAttrInfo *myinfo;/* Cached info about each attr */
    StringInfoData buf;/* output buffer (*not* in tmpcontext) */
    MemoryContext tmpcontext;/* Memory context for per-row workspace */
} DR_printtup;

extern DestReceiver *printtup_create_DR(CommandDest dest);

extern void SetRemoteDestReceiverParams(DestReceiver *self, Portal portal);

extern void SendRowDescriptionMessage(StringInfo buf,
                                      TupleDesc typeinfo, List *targetlist, int16 *formats);

extern void debugStartup(DestReceiver *self, int operation,
						 TupleDesc typeinfo, CommandTag commandTag);
extern bool debugtup(TupleTableSlot *slot, DestReceiver *self, CommandTag commandTag);

/* XXX these are really in executor/spi.c */
extern void spi_dest_startup(DestReceiver *self, int operation,
							 TupleDesc typeinfo, CommandTag commandTag);
extern bool spi_printtup(TupleTableSlot *slot, DestReceiver *self, CommandTag commandTag);

#endif							/* PRINTTUP_H */
