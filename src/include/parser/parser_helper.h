/*-------------------------------------------------------------------------
 *
 * parserapi.h
 *		API for parser
 *
 * This is the external API for the raw lexing/parsing functions.
 *
 * Portions Copyright (c) 2021, Jet C.X. ZHANG
 *
 * src/include/parser/parserapi.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSERHELPER_H
#define PARSERHELPER_H

#include "nodes/parsenodes.h"

/* From gram.y */
extern TypeName *SystemTypeName(char *name);
extern List *SystemFuncName(char *name);

#endif							/* PARSERHELPER_H */