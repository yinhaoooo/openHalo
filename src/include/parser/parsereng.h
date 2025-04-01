/*-------------------------------------------------------------------------
 *
 * parsereng.h
 *		HEADER for parser engine
 *
 *
 * Portions Copyright (c) 2021, Jet C.X. ZHANG
 *
 * src/include/parser/parsereng.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSERENG_H
#define PARSERENG_H

typedef enum
{
	POSTGRESQL_COMPAT_MODE,
	MYSQL_COMPAT_MODE,
} DatabaseCompatModeType;

typedef enum
{
	RAW_PARSE_DEFAULT = 0,
	RAW_PARSE_TYPE_NAME,
	RAW_PARSE_PLPGSQL_EXPR,
	RAW_PARSE_PLPGSQL_ASSIGN1,
	RAW_PARSE_PLPGSQL_ASSIGN2,
	RAW_PARSE_PLPGSQL_ASSIGN3
} RawParseMode;

typedef enum
{
	STANDARDARD_PARSERENGINE_AUXILIARY_OFF,
	STANDARDARD_PARSERENGINE_AUXILIARY_ON
} StandardParserengineAuxiliary;

/* 前向声明 */
typedef struct ParserRoutine ParserRoutine;

/* GUC variables */
extern int database_compat_mode;
extern int standard_parserengine_auxiliary;

/* Raw Parser Routine */
extern const ParserRoutine *parserengine;

/* Set Gloabal Parser Engine */
extern void InitParserEngine(void);

#endif							/* PARSERENG_H */