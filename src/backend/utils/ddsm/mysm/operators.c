/*-------------------------------------------------------------------------
 *
 * operators.c
 *	  Extend operators routines
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
 *	  src/backend/utils/ddsm/mysm/operators.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "fmgr.h"
#include "adapter/mysql/systemVar.h"
#include "datatype/timestamp.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/datetime.h"
#include "utils/fmgrprotos.h"
#include "utils/timestamp.h"
#include "utils/mysql/mys_date.h"
#include "utils/mysql/mys_timestamp.h"

PG_FUNCTION_INFO_V1(boolCondAndboolCImp);
Datum
boolCondAndboolCImp(PG_FUNCTION_ARGS)
{
    bool arg1 = PG_GETARG_BOOL(0);
    bool arg2 = PG_GETARG_BOOL(1);

    if (PG_ARGISNULL(0))
    {
        if (PG_ARGISNULL(1))
        {
            PG_RETURN_NULL();
        }
        else
        {
            if (arg2 == false)
            {
                PG_RETURN_BOOL(false);
            }
            else
            {
                PG_RETURN_NULL();
            }
        }
    }
    else if (PG_ARGISNULL(1))
    {
        if (PG_ARGISNULL(0))
        {
            PG_RETURN_NULL();
        }
        else
        {
            if (arg1 == false)
            {
                PG_RETURN_BOOL(false);
            }
            else
            {
                PG_RETURN_NULL();
            }
        }
    }
    else
    {
        PG_RETURN_BOOL(arg1 && arg2);
    }
}


PG_FUNCTION_INFO_V1(boolOrBool);
Datum
boolOrBool(PG_FUNCTION_ARGS)
{
    bool arg1 = PG_GETARG_BOOL(0);
    bool arg2 = PG_GETARG_BOOL(1);

    if (!PG_ARGISNULL(0))
    {
        if (!PG_ARGISNULL(1))
        {
            PG_RETURN_BOOL(arg1 || arg2);
        }
        else
        {
            if (arg1)
            {
                PG_RETURN_BOOL(arg1);
            }
            else 
            {
                PG_RETURN_NULL();
            }
        }
    }
    else 
    {
        if (!PG_ARGISNULL(1))
        {
            if (arg2)
            {
                PG_RETURN_BOOL(arg2);
            }
            else 
            {
                PG_RETURN_NULL();
            }
        }
        else 
        {
            PG_RETURN_NULL();
        }
    }
}


static int
mys_timestampCmpText(Timestamp arg1, text *arg2)
{
    char *argString1 = (char *)DirectFunctionCall1(timestamp_out, TimestampGetDatum(arg1));
    int argStringLen1 = strlen(argString1);
    char* argString2 = TextDatumGetCString(arg2);
    int argStringLen2 = 0;
    int timeFlags = MYS_TIME_FUZZY_DATE | MYS_TIME_INVALID_DATES;
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    fsec_t fsec = 0;
    int result;

    if (mys_sqlMode & MYS_MODE_NO_ZERO_IN_DATE)
    {
        timeFlags |= MYS_TIME_NO_ZERO_IN_DATE;
    }

    if (mys_sqlMode & MYS_MODE_NO_ZERO_DATE)
    {
        timeFlags |= MYS_TIME_NO_ZERO_DATE;
    }

    if (mys_DecodeStringDatetimeForCompare(argString2, tm, &fsec, timeFlags))
    {
        char buf[MAXDATELEN + 1];

        EncodeDateTime(tm, fsec, false, 0, NULL, DateStyle, buf);
        argString2 = pstrdup(buf);
    }

    argStringLen2 = strlen(argString2);
    
    result = memcmp(argString1, argString2, Min(argStringLen1, argStringLen2));
    if ((result == 0) && (argStringLen1 != argStringLen2))
    {
        result = (argStringLen1 < argStringLen2) ? -1 : 1;
    }

    return result;
}


static int
mys_textCmpTimestamp(text *arg1, Timestamp arg2)
{
    char *argString1 = TextDatumGetCString(arg1);
    int argStringLen1 = 0;
    char *argString2 = (char *)DirectFunctionCall1(timestamp_out, TimestampGetDatum(arg2));
    int argStringLen2 = strlen(argString2);
    int timeFlags = MYS_TIME_FUZZY_DATE | MYS_TIME_INVALID_DATES;
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    fsec_t fsec = 0;
    int result;

    if (mys_sqlMode & MYS_MODE_NO_ZERO_IN_DATE)
    {
        timeFlags |= MYS_TIME_NO_ZERO_IN_DATE;
    }

    if (mys_sqlMode & MYS_MODE_NO_ZERO_DATE)
    {
        timeFlags |= MYS_TIME_NO_ZERO_DATE;
    }

    if (mys_DecodeStringDatetimeForCompare(argString1, tm, &fsec, timeFlags))
    {
        char buf[MAXDATELEN + 1];

        EncodeDateTime(tm, fsec, false, 0, NULL, DateStyle, buf);
        argString1 = pstrdup(buf);
    }

    argStringLen1 = strlen(argString1);
    
    result = memcmp(argString1, argString2, Min(argStringLen1, argStringLen2));
    if ((result == 0) && (argStringLen1 != argStringLen2))
    {
        result = (argStringLen1 < argStringLen2) ? -1 : 1;
    }

    return result;
}


PG_FUNCTION_INFO_V1(mys_timestampGtText);
Datum
mys_timestampGtText(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_timestampCmpText(PG_GETARG_TIMESTAMP(0), PG_GETARG_TEXT_P(1)) > 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_timestampGeText);
Datum
mys_timestampGeText(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_timestampCmpText(PG_GETARG_TIMESTAMP(0), PG_GETARG_TEXT_P(1)) >= 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_timestampLtText);
Datum
mys_timestampLtText(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_timestampCmpText(PG_GETARG_TIMESTAMP(0), PG_GETARG_TEXT_P(1)) < 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_timestampLeText);
Datum
mys_timestampLeText(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_timestampCmpText(PG_GETARG_TIMESTAMP(0), PG_GETARG_TEXT_P(1)) <= 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_timestampEqText);
Datum
mys_timestampEqText(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_timestampCmpText(PG_GETARG_TIMESTAMP(0), PG_GETARG_TEXT_P(1)) == 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textGtTimestamp);
Datum
mys_textGtTimestamp(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_textCmpTimestamp(PG_GETARG_TEXT_P(0), PG_GETARG_TIMESTAMP(1)) > 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textGeTimestamp);
Datum
mys_textGeTimestamp(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_textCmpTimestamp(PG_GETARG_TEXT_P(0), PG_GETARG_TIMESTAMP(1)) >= 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textLtTimestamp);
Datum
mys_textLtTimestamp(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_textCmpTimestamp(PG_GETARG_TEXT_P(0), PG_GETARG_TIMESTAMP(1)) < 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textLeTimestamp);
Datum
mys_textLeTimestamp(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_textCmpTimestamp(PG_GETARG_TEXT_P(0), PG_GETARG_TIMESTAMP(1)) <= 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textEqTimestamp);
Datum
mys_textEqTimestamp(PG_FUNCTION_ARGS)
{
    bool result;

    result = (mys_textCmpTimestamp(PG_GETARG_TEXT_P(0), PG_GETARG_TIMESTAMP(1)) == 0);

    PG_RETURN_BOOL(result);
}


PG_FUNCTION_INFO_V1(mys_textPlInterval);
Datum
mys_textPlInterval(PG_FUNCTION_ARGS)
{
    Timestamp leftArg;
    char *leftArgString = TextDatumGetCString(PG_GETARG_TEXT_P(0));
    int timeFlags = MYS_TIME_NO_ZERO_DATE;
    Timestamp result;

    if (mys_sqlMode & MYS_MODE_INVALID_DATES)
    {
        timeFlags |= MYS_TIME_INVALID_DATES;
    }

    if (mys_strToDatetimeInternal(leftArgString, timeFlags, &leftArg))
    {
        result = DirectFunctionCall2(timestamp_pl_interval,
                                     TimestampGetDatum(leftArg),
                                     PointerGetDatum(PG_GETARG_INTERVAL_P(1)));
    }
    else
    {
        elog(ERROR, "Incorrect datetime value for mys_textPlInterval: %s", leftArgString);
    }

    PG_RETURN_TIMESTAMP(result);
}


PG_FUNCTION_INFO_V1(mys_intervalPlText);
Datum
mys_intervalPlText(PG_FUNCTION_ARGS)
{
    Timestamp rightArg;
    char *rightArgString = TextDatumGetCString(PG_GETARG_TEXT_P(1));
    int timeFlags = MYS_TIME_NO_ZERO_DATE;
    Timestamp result;

    if (mys_sqlMode & MYS_MODE_INVALID_DATES)
    {
        timeFlags |= MYS_TIME_INVALID_DATES;
    }

    if (mys_strToDatetimeInternal(rightArgString, timeFlags, &rightArg))
    {
        result = DirectFunctionCall2(timestamp_pl_interval,
                                     TimestampGetDatum(rightArg),
                                     PointerGetDatum(PG_GETARG_INTERVAL_P(0)));
    }
    else
    {
        elog(ERROR, "Incorrect datetime value for mys_intervalPlText: %s", rightArgString);
    }

    PG_RETURN_TIMESTAMP(result);
}


PG_FUNCTION_INFO_V1(mys_textMiInterval);
Datum
mys_textMiInterval(PG_FUNCTION_ARGS)
{
    Timestamp leftArg;
    char *leftArgString = TextDatumGetCString(PG_GETARG_TEXT_P(0));
    int timeFlags = MYS_TIME_NO_ZERO_DATE;
    Timestamp result;

    if (mys_sqlMode & MYS_MODE_INVALID_DATES)
    {
        timeFlags |= MYS_TIME_INVALID_DATES;
    }

    if (mys_strToDatetimeInternal(leftArgString, timeFlags, &leftArg))
    {
        Interval *span = PG_GETARG_INTERVAL_P(1);
        Interval tspan;
        
        tspan.month = -span->month;
        tspan.day = -span->day;
        tspan.time = -span->time;

        result = DirectFunctionCall2(timestamp_pl_interval,
                                     TimestampGetDatum(leftArg),
                                     PointerGetDatum(&tspan));
    }
    else
    {
        elog(ERROR, "Incorrect datetime value for mys_textMiInterval: %s", leftArgString);
    }

    PG_RETURN_TIMESTAMP(result);
}


