/*-------------------------------------------------------------------------
 *
 * adtext.c
 *		Extension for ADT Data Types
 *
 *
 * Copyright (c) 2021, Jet C.X. ZHANG
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/adtext.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "parser/parsereng.h"
#include "utils/adtext.h"
#include "utils/mysql/mys_adtext.h"

#include "miscadmin.h"
#include "libpq/libpq-be.h"


void InitADTExt(void);
const ADTExtMethod *adtext = NULL;
const ADTExtMethod *GetStandardADTExt(void);

static const ADTExtMethod standard_adtext;


/*
 * Make standard ADT Extention
 */
static const ADTExtMethod standard_adtext = {
    .pre_numeric_in = NULL,
    .post_numeric_out = NULL,
	.pre_timetz_in = NULL,
	.post_timetz_out = NULL,
	.post_timestamp_out = NULL
};

const ADTExtMethod *
GetStandardADTExt(void)
{
    return &standard_adtext;
}

void
InitADTExt(void)
{
    switch (database_compat_mode)
	{
		case POSTGRESQL_COMPAT_MODE:
			adtext = GetStandardADTExt();
			break;
		
        case MYSQL_COMPAT_MODE:
            
            if ((MyProcPort != NULL) && 
                (nodeTag(MyProcPort->protocol_handler) == T_MySQLProtocol))
            {
                adtext = GetMysADTExt();
            }
            else
            {
                adtext = GetStandardADTExt();
            }
            
            break;

		default:
			adtext = GetStandardADTExt();
			break;
	}
}