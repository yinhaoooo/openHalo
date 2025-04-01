/*-------------------------------------------------------------------------
 *
 * adtextapi.h
 *		Extension API for ADT Data Types
 *
 *
 * Copyright (c) 2021, Jet C.X. ZHANG
 *
 * src/include/utils/adtextapi.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef ADTEXTAPI_H
#define ADTEXTAPI_H

#include "fmgr.h"
#include "utils/date.h"

/*
 * callback functions
 */
typedef char *(*pre_numeric_in_function)(char *str);
typedef char *(*post_numeric_out_function) (char *str);
typedef TimeADT (*pre_time_in_function)(PG_FUNCTION_ARGS);
typedef char *(*post_time_out_function)(PG_FUNCTION_ARGS);
typedef char *(*pre_timetz_in_function)(char *str);
typedef char *(*post_timetz_out_function)(void  *time);
typedef char *(*pre_timestamp_in_function)(char *str);
typedef void  (*post_timestamp_out_function) (void *ts, int style, char *buf);
typedef Datum (*date_in_function)(PG_FUNCTION_ARGS);
typedef Datum (*timestamp_in_function)(PG_FUNCTION_ARGS);


typedef struct ADTExtMethod
{
    pre_numeric_in_function             pre_numeric_in;
    post_numeric_out_function           post_numeric_out;
    pre_time_in_function                pre_time_in;
    post_time_out_function              post_time_out;
    pre_timetz_in_function              pre_timetz_in;   
    post_timetz_out_function            post_timetz_out;  
    pre_timestamp_in_function           pre_timestamp_in;
    post_timestamp_out_function         post_timestamp_out;
    date_in_function                    date_in;
    timestamp_in_function               timestamp_in;
} ADTExtMethod;

#endif							/* ADTEXTAPI_H */