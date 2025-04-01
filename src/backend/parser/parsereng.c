/*-------------------------------------------------------------------------
 *
 * parsereng.c
 *		Parser Engine Helper Routine
 *
 *
 * Portions Copyright (c) 2021, Jet C.X. ZHANG
 *
 * IDENTIFICATION
 *	  src/backend/parser/parsereng.c
 *
 *-------------------------------------------------------------------------
 */
#include "c.h"
#include "postgres.h"

#include "parser/parsereng.h"
#include "parser/parser.h"
#include "parser/mysql/mys_parser.h"
#include "miscadmin.h"
#include "libpq/libpq-be.h"

/* GUC variable */
int database_compat_mode = POSTGRESQL_COMPAT_MODE;
int	standard_parserengine_auxiliary = STANDARDARD_PARSERENGINE_AUXILIARY_ON;

/* Parser Engine Instance */
const ParserRoutine *parserengine = NULL;

/* Initialize Parser Engine Instanace */
void InitParserEngine(void);

/*
 * Initialize Parser Engine Instance
 * Currently, only postgresql and oracle support
 */
void
InitParserEngine(void)
{
	switch (database_compat_mode)
	{
		case POSTGRESQL_COMPAT_MODE:
			parserengine = GetStandardParserEngine();
			break;
		case MYSQL_COMPAT_MODE:
            
            if ((MyProcPort != NULL) && 
                (nodeTag(MyProcPort->protocol_handler) == T_MySQLProtocol))
            {
                parserengine = GetMysParserEngine();
            }
            else
            {
                parserengine = GetStandardParserEngine();
            }
			/* parserengine = GetMysParserEngine(); */
            
			break;

		default:
			parserengine = GetStandardParserEngine();
			break;
	}
}
