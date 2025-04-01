/*-------------------------------------------------------------------------
 *
 * time_func.c
 *	  Extend time_func routines
 *
 * 
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 * 
 * 易景科技是Halo Database、Halo Database Management System、羲和数据
 * 库、羲和数据库管理系统（后面简称 Halo ）、openHalo软件的发明人同时也为
 * 知识产权权利人。Halo 软件的知识产权，以及与本软件相关的所有信息内容（包括
 * 但不限于文字、图片、音频、视频、图表、界面设计、版面框架、有关数据或电子文
 * 档等）均受中华人民共和国法律法规和相应的国际条约保护，易景科技享有上述知识
 * 产权，但相关权利人依照法律规定应享有的权利除外。未免疑义，本条所指的“知识
 * 产权”是指任何及所有基于 Halo 软件产生的：（a）版权、商标、商号、域名、与
 * 商标和商号相关的商誉、设计和专利；与创新、技术诀窍、商业秘密、保密技术、非
 * 技术信息相关的权利；（b）人身权、掩模作品权、署名权和发表权；以及（c）在
 * 本协议生效之前已存在或此后出现在世界任何地方的其他工业产权、专有权、与“知
 * 识产权”相关的权利，以及上述权利的所有续期和延长，无论此类权利是否已在相
 * 关法域内的相关机构注册。
 *
 * This software and related documentation are provided under a license
 * agreement containing restrictions on use and disclosure and are 
 * protected by intellectual property laws. Except as expressly permitted
 * in your license agreement or allowed by law, you may not use, copy, 
 * reproduce, translate, broadcast, modify, license, transmit, distribute,
 * exhibit, perform, publish, or display any part, in any form, or by any
 * means. Reverse engineering, disassembly, or decompilation of this 
 * software, unless required by law for interoperability, is prohibited.
 * 
 * This software is developed for general use in a variety of
 * information management applications. It is not developed or intended
 * for use in any inherently dangerous applications, including applications
 * that may create a risk of personal injury. If you use this software or
 * in dangerous applications, then you shall be responsible to take all
 * appropriate fail-safe, backup, redundancy, and other measures to ensure
 * its safe use. Halo Corporation and its affiliates disclaim any 
 * liability for any damages caused by use of this software in dangerous
 * applications.
 * 
 *
 * IDENTIFICATION
 *	  src/backend/utils/ddsm/mysm/time_func.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/fmgrprotos.h"
#include "utils/mysql/mys_date.h"
#include "utils/timestamp.h"


PG_FUNCTION_INFO_V1(subtime);

/*
 * SUBTIME()
 * returns expr1 − expr2 expressed as a value in the same
 * format as expr1. expr1 is a time or datetime expression, and expr2 is a
 * time expression.
 */
Datum
subtime(PG_FUNCTION_ARGS)
{
    text *result = NULL;
    text *text0 = PG_GETARG_TEXT_PP(0);
    char *str0 = VARDATA_ANY(text0);
    int str0Len = VARSIZE_ANY_EXHDR(text0);
    char oriCha0 = str0[str0Len];
    struct pg_tm tt0;
    struct pg_tm *tm0 = &tt0;
    fsec_t fsec0 = 0;
    int sign0 = 1;
    text *text1 = PG_GETARG_TEXT_PP(1);
    char *str1 = VARDATA_ANY(text1);
    int str1Len = VARSIZE_ANY_EXHDR(text1);
    char oriCha1 = str1[str1Len];
    struct pg_tm tt1;
    struct pg_tm *tm1 = &tt1;
    fsec_t fsec1 = 0;
    int sign1 = 1;

    tm0->tm_year = 0;
    tm1->tm_year = 0;
    str0[str0Len] = '\0';
    str1[str1Len] = '\0';
    mys_pre_time_in_for_subtime(str0, tm0, &fsec0, &sign0);
    mys_pre_time_in_for_subtime(str1, tm1, &fsec1, &sign1);
    str0[str0Len] = oriCha0;
    str1[str1Len] = oriCha1;

    if (tm1->tm_year > 0)
    {
        PG_RETURN_NULL();
    }

    if (tm0->tm_year > 0)
    {
        // timestamp - time
        Timestamp tempResult;
        Timestamp ts0;
        TimeADT t1;
        tm2timestamp(tm0, fsec0, NULL, &ts0);
        tm2time(tm1, fsec1, &t1);
        tempResult = sign0 * ts0 - sign1 * t1;
        result = cstring_to_text(DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(tempResult))));
    }
    else
    {
        // time - time
        char *resultStr;
        TimeADT tempResult;
        struct pg_tm tempTm;
        fsec_t tempFsec;
        TimeADT t0;
        TimeADT t1;
        tm2time(tm0, fsec0, &t0);
        tm2time(tm1, fsec1, &t1);
        tempResult = sign0 * t0 - sign1 * t1;
        resultStr = DatumGetCString(DirectFunctionCall1(time_out, TimeADTGetDatum(tempResult)));

        time2tm(tempResult, &tempTm, &tempFsec);
        if (tempTm.tm_hour > MYS_MAX_TIME_HOUR || tempTm.tm_hour < MYS_MAX_TIME_HOUR * -1)
        {
            elog(ERROR, "Truncated incorrect time value: \'%s\'", resultStr);
        }

        result = cstring_to_text(resultStr);
    }

    PG_RETURN_TEXT_P(result);
}


PG_FUNCTION_INFO_V1(addtime);

/*
 * ADDTIME()
 * adds expr2 to expr1 and returns the result.
 * expr1 is a time or datetime expression, and expr2 is a time expression.
 */
Datum
addtime(PG_FUNCTION_ARGS)
{
    text *result = NULL;
    text *text0 = PG_GETARG_TEXT_PP(0);
    char *str0 = VARDATA_ANY(text0);
    int str0Len = VARSIZE_ANY_EXHDR(text0);
    char oriCha0 = str0[str0Len];
    struct pg_tm tt0;
    struct pg_tm *tm0 = &tt0;
    fsec_t fsec0 = 0;
    int sign0 = 1;
    text *text1 = PG_GETARG_TEXT_PP(1);
    char *str1 = VARDATA_ANY(text1);
    int str1Len = VARSIZE_ANY_EXHDR(text1);
    char oriCha1 = str1[str1Len];
    struct pg_tm tt1;
    struct pg_tm *tm1 = &tt1;
    fsec_t fsec1 = 0;
    int sign1 = 1;

    tm0->tm_year = 0;
    tm1->tm_year = 0;
    str0[str0Len] = '\0';
    str1[str1Len] = '\0';
    mys_pre_time_in_for_subtime(str0, tm0, &fsec0, &sign0);
    mys_pre_time_in_for_subtime(str1, tm1, &fsec1, &sign1);
    str0[str0Len] = oriCha0;
    str1[str1Len] = oriCha1;

    if (tm1->tm_year > 0)
    {
        PG_RETURN_NULL();
    }

    if (tm0->tm_year > 0)
    {
        // timestamp + time
        Timestamp tempResult;
        Timestamp ts0;
        TimeADT t1;
        tm2timestamp(tm0, fsec0, NULL, &ts0);
        tm2time(tm1, fsec1, &t1);
        tempResult = sign0 * ts0 + sign1 * t1;
        result = cstring_to_text(DatumGetCString(DirectFunctionCall1(timestamp_out, TimestampGetDatum(tempResult))));
    }
    else
    {
        // time + time
        char *resultStr;
        TimeADT tempResult;
        struct pg_tm tempTm;
        fsec_t tempFsec;
        TimeADT t0;
        TimeADT t1;
        tm2time(tm0, fsec0, &t0);
        tm2time(tm1, fsec1, &t1);
        tempResult = sign0 * t0 + sign1 * t1;
        resultStr = DatumGetCString(DirectFunctionCall1(time_out, TimeADTGetDatum(tempResult)));

        time2tm(tempResult, &tempTm, &tempFsec);
        if (tempTm.tm_hour > MYS_MAX_TIME_HOUR || tempTm.tm_hour < MYS_MAX_TIME_HOUR * -1)
        {
            elog(ERROR, "Truncated incorrect time value: \'%s\'", resultStr);
        }

        result = cstring_to_text(resultStr);
    }

    PG_RETURN_TEXT_P(result);
}


PG_FUNCTION_INFO_V1(timediff);

/*
 * TIMEDIFF()
 * returns expr1 − expr2 expressed as a time value. 
 * expr1 and expr2 are time or date-and-time expressions, but both must be of the same type.
 */
Datum
timediff(PG_FUNCTION_ARGS)
{
    TimeADT result = 0;
    char *resultStr = NULL;
    struct pg_tm tempTm;
    fsec_t tempFsec;
    text *text0 = PG_GETARG_TEXT_PP(0);
    char *str0 = VARDATA_ANY(text0);
    int str0Len = VARSIZE_ANY_EXHDR(text0);
    char oriCha0 = str0[str0Len];
    struct pg_tm tt0;
    struct pg_tm *tm0 = &tt0;
    fsec_t fsec0 = 0;
    int sign0 = 1;
    text *text1 = PG_GETARG_TEXT_PP(1);
    char *str1 = VARDATA_ANY(text1);
    int str1Len = VARSIZE_ANY_EXHDR(text1);
    char oriCha1 = str1[str1Len];
    struct pg_tm tt1;
    struct pg_tm *tm1 = &tt1;
    fsec_t fsec1 = 0;
    int sign1 = 1;

    tm0->tm_year = 0;
    tm1->tm_year = 0;
    str0[str0Len] = '\0';
    str1[str1Len] = '\0';
    mys_pre_time_in_for_subtime(str0, tm0, &fsec0, &sign0);
    mys_pre_time_in_for_subtime(str1, tm1, &fsec1, &sign1);
    str0[str0Len] = oriCha0;
    str1[str1Len] = oriCha1;

    if ((tm0->tm_year > 0 && tm1->tm_year == 0) ||
        (tm0->tm_year == 0 && tm1->tm_year > 0))
    {
        PG_RETURN_NULL();
    }

    if (tm0->tm_year > 0)
    {
        // timestamp - timestamp
        Timestamp ts0;
        Timestamp ts1;
        tm2timestamp(tm0, fsec0, NULL, &ts0);
        tm2timestamp(tm1, fsec1, NULL, &ts1);
        result = sign0 * ts0 - sign1 * ts1;
    }
    else
    {
        // time - time
        TimeADT t0;
        TimeADT t1;
        tm2time(tm0, fsec0, &t0);
        tm2time(tm1, fsec1, &t1);
        result = sign0 * t0 - sign1 * t1;
    }

    resultStr = DatumGetCString(DirectFunctionCall1(time_out, TimeADTGetDatum(result)));

    time2tm(result, &tempTm, &tempFsec);
    if (tempTm.tm_hour > MYS_MAX_TIME_HOUR || tempTm.tm_hour < MYS_MAX_TIME_HOUR * -1)
    {
        elog(ERROR, "Truncated incorrect time value: \'%s\'", resultStr);
    }

    PG_RETURN_TIMEADT(result);
}
