/*-------------------------------------------------------------------------
 *
 * user_var.c
 *	  Extend user_var routines
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
 *	  src/backend/utils/ddsm/mysm/user_var.c
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"
#include "nodes/nodes.h"
#include "postgres.h"

#include "fmgr.h"
#include "mb/pg_wchar.h"
#include "utils/builtins.h"
#include "utils/bytea.h"
#include "utils/date.h"
#include "utils/datetime.h"
#include "utils/float.h"
#include "utils/int8.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"
#include "utils/numeric.h"
#include "utils/timestamp.h"
#include "utils/varbit.h"
#include "commands/mysql/mys_uservar.h"


#define COLLATION 100


static Oid getParameterType(FmgrInfo *fmgrinfo, int parmIndex);
static Datum anycompatibleEqBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleNeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleCondAndBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleCondOrBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleLikeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleNlikeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleRegexpBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaRegexpAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleNregexpBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaNregexpAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleRegexpbinBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaRegexpbinAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleNregexpbinBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaNregexpbinAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleLtBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleLeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleGtBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleGeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatiblePlBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleMiBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaMiAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleMulBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleDivBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaDivAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleModBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum byteaModAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleAndBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);
static Datum anycompatibleOrBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val);

/* static void truncateStrForDigit(size_t strLen, char *str); */
static void truncateStrTailWhileSpacesForDigit(size_t strLen, char *str);
static Datum convertParameterToText(Oid parmType, Datum parmVal);
static Datum convertParameterToNumeric(Oid parmType, Datum parm);
static Datum convertParameterToInt8(Oid parmType, Datum parm);
static Datum bytea2int8_internal(bytea *vlena);
static Datum bytea2numeric_internal(bytea *vlena);
static char *AppendSeconds(char *cp, int sec, fsec_t fsec, int precision, bool fillzeros);


PG_FUNCTION_INFO_V1(convertTimeToDouble);
Datum
convertTimeToDouble(PG_FUNCTION_ARGS)
{
    Datum ret;

    TimeADT timeADT = DatumGetTimeADT(PG_GETARG_DATUM(0));
    char parmStr[MAXDATELEN + 1];
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    fsec_t fsec;
    char buf[MAXDATELEN + 1];
    char *strp;
    bool nega = false;

    if (timeADT < 0)
    {
        nega = true;
        timeADT = timeADT * -1;
    }

    strp = buf;
    time2tm(timeADT, tm, &fsec);
    strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
    strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
    strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIME_PRECISION, true);
    *strp = '\0';

    if (nega)
    {
        pg_snprintf(parmStr, MAXDATELEN, "-%s", buf);
    }
    else
    {
        pg_snprintf(parmStr, MAXDATELEN, "%s", buf);
    }

    ret = DirectFunctionCall3(float8in,
                              CStringGetDatum(parmStr),
                              ObjectIdGetDatum(InvalidOid),
                              Int32GetDatum(-1));

    return ret;
}

PG_FUNCTION_INFO_V1(convertDateToDouble);
Datum
convertDateToDouble(PG_FUNCTION_ARGS)
{
    Datum ret;

    DateADT dateADT = DatumGetDateADT(PG_GETARG_DATUM(0));
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    char parmStr[MAXDATELEN + 1];
    char *strp;

    strp = parmStr;
    j2date(dateADT + POSTGRES_EPOCH_JDATE,
           &(tm->tm_year), &(tm->tm_mon), &(tm->tm_mday));
    strp = pg_ultostr_zeropad(strp, 
                              (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 
                              4);
    strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
    strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
    *strp = '\0';
    ret = DirectFunctionCall3(float8in,
                              CStringGetDatum(parmStr),
                              ObjectIdGetDatum(InvalidOid),
                              Int32GetDatum(-1));

    return ret;
}

PG_FUNCTION_INFO_V1(convertTimestampToDouble);
Datum
convertTimestampToDouble(PG_FUNCTION_ARGS)
{
    Datum ret;

    Timestamp timestamp = DatumGetTimestamp(PG_GETARG_DATUM(0));
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    fsec_t fsec;
    char parmStr[MAXDATELEN + 1];

    if (timestamp2tm(timestamp, NULL, tm, &fsec, NULL, NULL) == 0)
    {
        char *strp = parmStr;
        strp = pg_ultostr_zeropad(strp, 
                                  (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 
                                  4);
        strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
        strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIMESTAMP_PRECISION, true);
        *strp = '\0';
    }		
    else
    {
        ereport(ERROR,
                (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                 errmsg("timestamp out of range")));
    }
    ret = DirectFunctionCall3(float8in,
                              CStringGetDatum(parmStr),
                              ObjectIdGetDatum(InvalidOid),
                              Int32GetDatum(-1));
    return ret;
}

PG_FUNCTION_INFO_V1(mysSetUserVar);
Datum
mysSetUserVar(PG_FUNCTION_ARGS)
{
    char *varName = TextDatumGetCString(PG_GETARG_DATUM(0));

    if (!PG_ARGISNULL(1))
    {
        Datum datumValue = PG_GETARG_DATUM(1);
        Oid varValueType = get_fn_expr_argtype(fcinfo->flinfo, 1);
        Oid	typOutput;
        bool typIsVarlena;
        char *varValue;
        bool isDigit;

        getTypeOutputInfo(varValueType, &typOutput, &typIsVarlena);
        varValue = OidOutputFunctionCall(typOutput, datumValue);
        varValueType = get_fn_expr_argtype(fcinfo->flinfo, 1);
        isDigit = varValueIsDigit(datumValue, varValueType);

        mysSetUserVarInternal(varName, varValue, varValueType, isDigit, false);
    }
    else
    {
        mysSetUserVarInternal(varName, NULL, InvalidOid, false, true);
    }

    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(mysClearUserVariables);
Datum
mysClearUserVariables(PG_FUNCTION_ARGS)
{
    clearUserVars();

    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(mysSetUserVarInExpr);
Datum
mysSetUserVarInExpr(PG_FUNCTION_ARGS)
{
    char *varName = TextDatumGetCString(PG_GETARG_DATUM(0));
    FmgrInfo *foutcache;
    Oid varValueType;
    bytea *result;

    /* Get output function info, building it if first time through */
    foutcache = (FmgrInfo *) fcinfo->flinfo->fn_extra;
	if (foutcache == NULL)
    {
        Oid	typOutput;
        bool typIsVarlena;

        foutcache = (FmgrInfo *) MemoryContextAlloc(fcinfo->flinfo->fn_mcxt, sizeof(FmgrInfo) + sizeof(Oid));

        varValueType = get_fn_expr_argtype(fcinfo->flinfo, 1);
        if (!OidIsValid(varValueType))
            elog(ERROR, "could not determine data type of mysSetUserVar() input");

        getTypeOutputInfo(varValueType, &typOutput, &typIsVarlena);
        fmgr_info_cxt(typOutput, foutcache, fcinfo->flinfo->fn_mcxt);
        memcpy(((char *)foutcache + sizeof(FmgrInfo)), &varValueType, sizeof(Oid));

        fcinfo->flinfo->fn_extra = foutcache;
    }
    else
    {
        varValueType = *(Oid *)((char *)foutcache + sizeof(FmgrInfo));
    }

    if (!PG_ARGISNULL(1))
    {
        Datum datumValue = PG_GETARG_DATUM(1);
        char *varValue = OutputFunctionCall(foutcache, datumValue);
        bool isDigit = varValueIsDigit(datumValue, varValueType);

        mysSetUserVarInternal(varName, varValue, varValueType, isDigit, false);
    }
    else
    {
        mysSetUserVarInternal(varName, NULL, varValueType, false, true);
    }

    /* 获取用户变量的值 */
    result = mysGetUserVarValueInternal(varName);
    
    if (result)
    {
        PG_RETURN_BYTEA_P(result);
    }
    else
    {
        PG_RETURN_NULL();
    }
}


PG_FUNCTION_INFO_V1(mysGetUserVarValue);
Datum
mysGetUserVarValue(PG_FUNCTION_ARGS)
{
    char *varName = TextDatumGetCString(PG_GETARG_DATUM(0));
    bytea *result;

    result = mysGetUserVarValueInternal(varName);

    if (result)
    {
        PG_RETURN_BYTEA_P(result);
    }
    else
    {
        PG_RETURN_NULL();
    }
}


PG_FUNCTION_INFO_V1(mys_bytea2int8);
Datum
mys_bytea2int8(PG_FUNCTION_ARGS)
{
    return bytea2int8_internal(PG_GETARG_BYTEA_PP(0));
}

static Datum
bytea2int8_internal(bytea *vlena)
{
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (!(isdigit(byte[index]) || byte[index] == '-' || byte[index] == '+'))
        {
            elog(ERROR, "Invalid input syntax for type int8");
        }
    }

    result = DirectFunctionCall1(int8in, CStringGetDatum(num));
    pfree(num);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2int4);
Datum
mys_bytea2int4(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (!(isdigit(byte[index]) || byte[index] == '-' || byte[index] == '+'))
        {
            elog(ERROR, "Invalid input syntax for type int4");
        }
    }

    result = DirectFunctionCall1(int4in, CStringGetDatum(num));
    pfree(num);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2int2);
Datum
mys_bytea2int2(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (!(isdigit(byte[index]) || byte[index] == '-' || byte[index] == '+'))
        {
            elog(ERROR, "Invalid input syntax for type int2");
        }
    }

    result = DirectFunctionCall1(int2in, CStringGetDatum(num));
    pfree(num);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2numeric);
Datum
mys_bytea2numeric(PG_FUNCTION_ARGS)
{
    return bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
}

static Datum
bytea2numeric_internal(bytea *vlena)
{
    Datum result;

    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type numeric");
        }
    }

    result = DirectFunctionCall3(numeric_in,
                                 CStringGetDatum(num),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(num);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2float8);
Datum
mys_bytea2float8(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type double precision");
        }
    }

    result = DirectFunctionCall1(float8in, CStringGetDatum(num));
    pfree(num);

    return result;
}


PG_FUNCTION_INFO_V1(mys_bytea2float4);
Datum
mys_bytea2float4(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *num = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type real");
        }
    }

    result = DirectFunctionCall1(float4in, CStringGetDatum(num));
    pfree(num);

	return result;
}


PG_FUNCTION_INFO_V1(mys_bytea2text);
Datum
mys_bytea2text(PG_FUNCTION_ARGS)
{
    Datum result;

    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);

    pg_verifymbstr(byte, byteLen, false);

    // TODO: 最好的做法是，普通的text和varchar类型，最好是在入库前去掉末尾的空格，
    // 这样不影响set @var = '   '
    // TODO: 现在这里只是把bytea转为text时字符串末尾的空格去掉，
    // 其他本来就是text/varchar的末尾的空格还是存在。
    for (int i = (byteLen - 1); i >= 0; i--)
    {
        if (str[i] != ' ')
        {
            break;
        }
        else
        {
            str[i] = '\0';
            byteLen -= 1;
        }
    }

    result = DirectFunctionCall1(textin, CStringGetDatum(str));
    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2varchar);
Datum
mys_bytea2varchar(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    Datum result;

    pg_verifymbstr(byte, byteLen, false);

    result = DirectFunctionCall3(varcharin,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));
    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2bpchar);
Datum
mys_bytea2bpchar(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    Datum result;

    pg_verifymbstr(byte, byteLen, false);

    result = DirectFunctionCall3(bpcharin,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));
    pfree(str);

	return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2date);
Datum
mys_bytea2date(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type date");
        }
    }

    result = DirectFunctionCall1(date_in,
                                 CStringGetDatum(str));

    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2time);
Datum
mys_bytea2time(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type time");
        }
    }

    result = DirectFunctionCall3(time_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2timetz);
Datum
mys_bytea2timetz(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type time with time zone");
        }
    }

    result = DirectFunctionCall3(timetz_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2timestamp);
Datum
mys_bytea2timestamp(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type timestamp");
        }
    }

    result = DirectFunctionCall3(timestamp_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2timestamptz);
Datum
mys_bytea2timestamptz(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str = pnstrdup(byte, byteLen);
    size_t index;
    Datum result;

    for (index = 0; index < byteLen; index++)
    {
        if (byte[index] == '\0')
        {
            elog(ERROR, "Invalid input syntax for type timestamp with time zone");
        }
    }

    result = DirectFunctionCall3(timestamptz_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2bool);
Datum
mys_bytea2bool(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    size_t index;
    Datum result;

    result = false;
    for (index = 0; index < byteLen; index++)
    {
        if (!isdigit(byte[index]))
        {
            elog(ERROR, "Invalid input syntax for type boolean");
        }
        else
        {
            if (byte[index] != '0')
            {
                result = true;
            }
        }
    }

    return result;
}

PG_FUNCTION_INFO_V1(mys_bytea2bit);
Datum
mys_bytea2bit(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str;
    int strIndex;
    int strLen;
    size_t index;
    int i;
    size_t pre_zero_bit_num;
    bool ok;
    Datum result;

    ok = false;
    pre_zero_bit_num = 0;
    str = NULL;
    strLen = 0;
    for (index = 0; index < byteLen; index++)
    {
        for (i = 0; i < BITS_PER_BYTE; i++)
        {
            bool curBitIsZero = ((byte[index] & (0x80 >> i)) == 0) ? true : false;
            if (ok)
            {
                str[strIndex] = curBitIsZero ? '0' : '1';
                strIndex++;
            }
            else
            {
                if (curBitIsZero)
                {
                    pre_zero_bit_num++;
                }
                else
                {
                    size_t vaild_bit_num = byteLen * BITS_PER_BYTE - pre_zero_bit_num;
                    /* MySQL中bit类型的最大长度为64 */
                    if (vaild_bit_num > 64)
                    {
                        elog(ERROR, "Data too long for type bit");
                    }
                    else
                    {
                        str = palloc(1 + vaild_bit_num + 1);
                        str[0] = 'b';
                        str[1] = curBitIsZero ? '0' : '1';
                        strIndex = 2;
                        strLen = 1 + vaild_bit_num;
                        ok = true;
                    }
                }
            }
        }
    }

    if (str == NULL)
    {
        str = pstrdup("");
    }
    else
    {
        str[strLen] = '\0';
    }

    result = DirectFunctionCall3(bit_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}


PG_FUNCTION_INFO_V1(mys_bytea2varbit);
Datum
mys_bytea2varbit(PG_FUNCTION_ARGS)
{
    bytea *vlena = PG_GETARG_BYTEA_PP(0);
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    char *str;
    int strIndex;
    int strLen;
    size_t index;
    int i;
    size_t pre_zero_bit_num;
    bool ok;
    Datum result;

    ok = false;
    pre_zero_bit_num = 0;
    str = NULL;
    strLen = 0;
    for (index = 0; index < byteLen; index++)
    {
        for (i = 0; i < BITS_PER_BYTE; i++)
        {
            bool curBitIsZero = ((byte[index] & (0x80 >> i)) == 0) ? true : false;
            if (ok)
            {
                str[strIndex] = curBitIsZero ? '0' : '1';
                strIndex++;
            }
            else
            {
                if (curBitIsZero)
                {
                    pre_zero_bit_num++;
                }
                else
                {
                    size_t vaild_bit_num = byteLen * BITS_PER_BYTE - pre_zero_bit_num;
                    /* MySQL中bit类型的最大长度为64 */
                    if (vaild_bit_num > 64)
                    {
                        elog(ERROR, "Data too long for type bit");
                    }
                    else
                    {
                        str = palloc(1 + vaild_bit_num + 1);
                        str[0] = 'b';
                        str[1] = curBitIsZero ? '0' : '1';
                        strIndex = 2;
                        strLen = 1 + vaild_bit_num;
                        ok = true;
                    }
                }
            }
        }
    }

    if (str == NULL)
    {
        str = pstrdup("");
    }
    else
    {
        str[strLen] = '\0';
    }

    result = DirectFunctionCall3(varbit_in,
                                 CStringGetDatum(str),
                                 ObjectIdGetDatum(InvalidOid),
                                 Int32GetDatum(-1));

    pfree(str);

    return result;
}


PG_FUNCTION_INFO_V1(anycompatibleEqByteaCImp);
Datum
anycompatibleEqByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleEqBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaEqAnycompatibleCImp);
Datum
byteaEqAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleEqBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleNeByteaCImp);
Datum
anycompatibleNeByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleNeBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaNeAnycompatibleCImp);
Datum
byteaNeAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleNeBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleCondAndByteaCImp);
Datum
anycompatibleCondAndByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleCondAndBytea(parmType, 
                                     PG_GETARG_DATUM(0), 
                                     PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaCondAndAnycompatibleCImp);
    Datum
byteaCondAndAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleCondAndBytea(parmType, 
                                     PG_GETARG_DATUM(1), 
                                     PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleCondOrByteaCImp);
Datum
anycompatibleCondOrByteaCImp(PG_FUNCTION_ARGS)
{
    if (!PG_ARGISNULL(0) && !PG_ARGISNULL(1))
    {
        Oid parmType = getParameterType(fcinfo->flinfo, 0);
        return anycompatibleCondOrBytea(parmType, 
                                        PG_GETARG_DATUM(0), 
                                        PG_GETARG_DATUM(1));
    }
    else if (!PG_ARGISNULL(0) && PG_ARGISNULL(1))
    {
        Oid parmType = getParameterType(fcinfo->flinfo, 0);
        Datum parm0 = convertParameterToNumeric(parmType, 
                                                PG_GETARG_DATUM(0));
        Datum num0 = DirectFunctionCall3(numeric_in,
                                         CStringGetDatum("0"),
                                         ObjectIdGetDatum(InvalidOid),
                                         Int32GetDatum(-1));
        return DirectFunctionCall2(numeric_ne, parm0, num0);
    }
    else if (PG_ARGISNULL(0) && !PG_ARGISNULL(1))
    {
        Datum parm1 = bytea2numeric_internal(
                                             DatumGetByteaPP(PG_GETARG_DATUM(1)));
        Datum num0 = DirectFunctionCall3(numeric_in,
                                         CStringGetDatum("0"),
                                         ObjectIdGetDatum(InvalidOid),
                                         Int32GetDatum(-1));
        return DirectFunctionCall2(numeric_ne, parm1, num0);
    }
    else 
    {
        PG_RETURN_NULL();
    }
}

PG_FUNCTION_INFO_V1(byteaCondOrAnycompatibleCImp);
Datum
byteaCondOrAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    if (!PG_ARGISNULL(0) && !PG_ARGISNULL(1))
    {
        Oid parmType = getParameterType(fcinfo->flinfo, 1);
        return anycompatibleCondOrBytea(parmType, 
                                        PG_GETARG_DATUM(1), 
                                        PG_GETARG_DATUM(0));
    }
    else if (!PG_ARGISNULL(0) && PG_ARGISNULL(1))
    {
        Datum parm0 = bytea2numeric_internal(
                                             DatumGetByteaPP(PG_GETARG_DATUM(0)));
        Datum num0 = DirectFunctionCall3(numeric_in,
                                         CStringGetDatum("0"),
                                         ObjectIdGetDatum(InvalidOid),
                                         Int32GetDatum(-1));
        return DirectFunctionCall2(numeric_ne, parm0, num0);
    }
    else if (PG_ARGISNULL(0) && !PG_ARGISNULL(1))
    {
        Oid parmType = getParameterType(fcinfo->flinfo, 1);
        Datum parm1 = convertParameterToNumeric(parmType, 
                                                PG_GETARG_DATUM(1));
        Datum num0 = DirectFunctionCall3(numeric_in,
                                         CStringGetDatum("0"),
                                         ObjectIdGetDatum(InvalidOid),
                                         Int32GetDatum(-1));
        return DirectFunctionCall2(numeric_ne, parm1, num0);
    }
    else 
    {
        PG_RETURN_NULL();
    }
}

PG_FUNCTION_INFO_V1(anycompatibleLikeByteaCImp);
Datum
anycompatibleLikeByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleLikeBytea(parmType, 
                                  PG_GETARG_DATUM(0), 
                                  PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaLikeAnycompatibleCImp);
Datum
byteaLikeAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleLikeBytea(parmType, 
                                   PG_GETARG_DATUM(1), 
                                   PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleNlikeByteaCImp);
Datum
anycompatibleNlikeByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleNlikeBytea(parmType, 
                                   PG_GETARG_DATUM(0), 
                                   PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaNlikeAnycompatibleCImp);
Datum
byteaNlikeAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleNlikeBytea(parmType, 
                                   PG_GETARG_DATUM(1), 
                                   PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleRegexpByteaCImp);
Datum
anycompatibleRegexpByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleRegexpBytea(parmType, 
                                    PG_GETARG_DATUM(0), 
                                    PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaRegexpAnycompatibleCImp);
Datum
byteaRegexpAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaRegexpAnycompatible(parmType, 
                                    PG_GETARG_DATUM(0), 
                                    PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(anycompatibleNregexpByteaCImp);
Datum
anycompatibleNregexpByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleNregexpBytea(parmType, 
                                     PG_GETARG_DATUM(0), 
                                     PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaNregexpAnycompatibleCImp);
Datum
byteaNregexpAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaNregexpAnycompatible(parmType, 
                                     PG_GETARG_DATUM(0), 
                                     PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(anycompatibleRegexpbinByteaCImp);
Datum
anycompatibleRegexpbinByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleRegexpbinBytea(parmType, 
                                       PG_GETARG_DATUM(0), 
                                       PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaRegexpbinAnycompatibleCImp);
Datum
byteaRegexpbinAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaRegexpbinAnycompatible(parmType, 
                                       PG_GETARG_DATUM(0), 
                                       PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(anycompatibleNregexpbinByteaCImp);
Datum
anycompatibleNregexpbinByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleNregexpbinBytea(parmType, 
                                        PG_GETARG_DATUM(0), 
                                        PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaNregexpbinAnycompatibleCImp);
Datum
byteaNregexpbinAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaNregexpbinAnycompatible(parmType, 
                                        PG_GETARG_DATUM(0), 
                                        PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(anycompatibleLtByteaCImp);
Datum
anycompatibleLtByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleLtBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaLtAnycompatibleCImp);
Datum
byteaLtAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleGtBytea(
                                parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleLeByteaCImp);
Datum
anycompatibleLeByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleLeBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaLeAnycompatibleCImp);
Datum
byteaLeAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleGeBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(byteaCondAndByteaCImp);
Datum
byteaCondAndByteaCImp(PG_FUNCTION_ARGS)
{
    bool isParm0ValNot0 = false;
    bool isParm1ValNot0 = false;

    Datum num0 = DirectFunctionCall3(numeric_in,
								     CStringGetDatum("0"),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    Datum parm0 = bytea2numeric_internal(
                                         DatumGetByteaPP(PG_GETARG_DATUM(0)));
    Datum parm1 = bytea2numeric_internal(
                                         DatumGetByteaPP(PG_GETARG_DATUM(1)));
    isParm0ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm0, num0));
    isParm1ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm1, num0));

    PG_RETURN_BOOL(isParm0ValNot0 && isParm1ValNot0);
}

PG_FUNCTION_INFO_V1(byteaCondOrByteaCImp);
Datum
byteaCondOrByteaCImp(PG_FUNCTION_ARGS)
{
    bool isParm0ValNot0 = false;
    bool isParm1ValNot0 = false;

    Datum num0 = DirectFunctionCall3(numeric_in,
								     CStringGetDatum("0"),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    if (!PG_ARGISNULL(0))
    {
        Datum parm0 = bytea2numeric_internal(
                                             DatumGetByteaPP(PG_GETARG_DATUM(0)));
        isParm0ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm0, num0));
    }
    if (!PG_ARGISNULL(1))
    {
        Datum parm1 = bytea2numeric_internal(
                                             DatumGetByteaPP(PG_GETARG_DATUM(1)));
        isParm1ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm1, num0));
    }

    PG_RETURN_BOOL(isParm0ValNot0 || isParm1ValNot0);
}

PG_FUNCTION_INFO_V1(byteaGtByteaCImp);
Datum
byteaGtByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Val = PG_GETARG_DATUM(0);
    Datum parm1Val = PG_GETARG_DATUM(1);
    bool isDigit0 = varValueIsDigit(parm0Val, BYTEAOID);
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    if (isDigit0 || isDigit1)
    {
        Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
        Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else
    {
        return DirectFunctionCall2(byteagt, parm0Val, parm1Val);
    }
}

PG_FUNCTION_INFO_V1(byteaGeByteaCImp);
Datum
byteaGeByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Val = PG_GETARG_DATUM(0);
    Datum parm1Val = PG_GETARG_DATUM(1);
    bool isDigit0 = varValueIsDigit(parm0Val, BYTEAOID);
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    if (isDigit0 || isDigit1)
    {
        Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
        Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else
    {
        return DirectFunctionCall2(byteage, parm0Val, parm1Val);
    }
}

PG_FUNCTION_INFO_V1(byteaLtByteaCImp);
Datum
byteaLtByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Val = PG_GETARG_DATUM(0);
    Datum parm1Val = PG_GETARG_DATUM(1);
    bool isDigit0 = varValueIsDigit(parm0Val, BYTEAOID);
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    if (isDigit0 || isDigit1)
    {
        Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
        Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else
    {
        return DirectFunctionCall2(bytealt, parm0Val, parm1Val);
    }
}

PG_FUNCTION_INFO_V1(byteaLeByteaCImp);
Datum
byteaLeByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Val = PG_GETARG_DATUM(0);
    Datum parm1Val = PG_GETARG_DATUM(1);
    bool isDigit0 = varValueIsDigit(parm0Val, BYTEAOID);
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    if (isDigit0 || isDigit1)
    {
        Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
        Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else
    {
        return DirectFunctionCall2(byteale, parm0Val, parm1Val);
    }
}

PG_FUNCTION_INFO_V1(byteaRegexpByteaCImp);
Datum
byteaRegexpByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0 = PG_GETARG_DATUM(0);
    Datum parm1 = PG_GETARG_DATUM(1);
    bytea *vlena = DatumGetByteaPP(parm1);
    size_t len = VARSIZE_ANY_EXHDR(vlena);
    if (0 < len)
    {
        return DirectFunctionCall2Coll(texticregexeq, COLLATION, parm0, parm1);
    }
    else 
    {
        elog(ERROR, "Got error 'empty (sub)expression' from regexp");
    }
}

PG_FUNCTION_INFO_V1(byteaNregexpByteaCImp);
Datum
byteaNregexpByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0 = PG_GETARG_DATUM(0);
    Datum parm1 = PG_GETARG_DATUM(1);
    bytea *vlena = DatumGetByteaPP(parm1);
    size_t len = VARSIZE_ANY_EXHDR(vlena);
    if (0 < len)
    {
        return DirectFunctionCall2Coll(texticregexne, COLLATION, parm0, parm1);
    }
    else 
    {
        elog(ERROR, "Got error 'empty (sub)expression' from regexp");
    }
}

PG_FUNCTION_INFO_V1(byteaRegexpbinByteaCImp);
Datum
byteaRegexpbinByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0 = PG_GETARG_DATUM(0);
    Datum parm1 = PG_GETARG_DATUM(1);
    bytea *vlena = DatumGetByteaPP(parm1);
    size_t len = VARSIZE_ANY_EXHDR(vlena);
    if (0 < len)
    {
        return DirectFunctionCall2Coll(textregexeq, COLLATION, parm0, parm1);
    }
    else 
    {
        elog(ERROR, "Got error 'empty (sub)expression' from regexp");
    }
}

PG_FUNCTION_INFO_V1(byteaNregexpbinByteaCImp);
Datum
byteaNregexpbinByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0 = PG_GETARG_DATUM(0);
    Datum parm1 = PG_GETARG_DATUM(1);
    bytea *vlena = DatumGetByteaPP(parm1);
    size_t len = VARSIZE_ANY_EXHDR(vlena);
    if (0 < len)
    {
        return DirectFunctionCall2Coll(textregexne, COLLATION, parm0, parm1);
    }
    else 
    {
        elog(ERROR, "Got error 'empty (sub)expression' from regexp");
    }
}

PG_FUNCTION_INFO_V1(anycompatibleGtByteaCImp);
Datum
anycompatibleGtByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleGtBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaGtAnycompatibleCImp);
Datum
byteaGtAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleLtBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(anycompatibleGeByteaCImp);
Datum
anycompatibleGeByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleGeBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaGeAnycompatibleCImp);
Datum
byteaGeAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleLeBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(byteaPlByteaCImp);
Datum
byteaPlByteaCImp(PG_FUNCTION_ARGS)
{
    Numeric	result;

    Datum parm0 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
    Datum parm1 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
    result = numeric_add_opt_error(DatumGetNumeric(parm0), DatumGetNumeric(parm1), NULL);

    PG_RETURN_NUMERIC(result);
}

PG_FUNCTION_INFO_V1(anycompatiblePlByteaCImp);
Datum
anycompatiblePlByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatiblePlBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaPlAnycompatibleCImp);
Datum
byteaPlAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatiblePlBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(byteaMiByteaCImp);
Datum
byteaMiByteaCImp(PG_FUNCTION_ARGS)
{
    Numeric	result;

    Datum parm0 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
    Datum parm1 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
    result = numeric_sub_opt_error(DatumGetNumeric(parm0), DatumGetNumeric(parm1), NULL);

    PG_RETURN_NUMERIC(result);
}

PG_FUNCTION_INFO_V1(anycompatibleMiByteaCImp);
Datum
anycompatibleMiByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleMiBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaMiAnycompatibleCImp);
Datum
byteaMiAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaMiAnycompatible(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaMulByteaCImp);
Datum
byteaMulByteaCImp(PG_FUNCTION_ARGS)
{
    Numeric	result;

    Datum parm0 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
    Datum parm1 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
    result = numeric_mul_opt_error(DatumGetNumeric(parm0), DatumGetNumeric(parm1), NULL);

    PG_RETURN_NUMERIC(result);
}

PG_FUNCTION_INFO_V1(anycompatibleMulByteaCImp);
Datum
anycompatibleMulByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleMulBytea(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaMulAnycompatibleCImp);
Datum
byteaMulAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleMulBytea(parmType, 
                                 PG_GETARG_DATUM(1), 
                                 PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(byteaDivByteaCImp);
Datum
byteaDivByteaCImp(PG_FUNCTION_ARGS)
{
    Numeric	result;

    Datum parm0 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
    Datum parm1 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
    result = numeric_div_opt_error(DatumGetNumeric(parm0), DatumGetNumeric(parm1), NULL);

    PG_RETURN_NUMERIC(result);
}

PG_FUNCTION_INFO_V1(anycompatibleDivByteaCImp);
Datum
anycompatibleDivByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleDivBytea(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaDivAnycompatibleCImp);
Datum
byteaDivAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaDivAnycompatible(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaModByteaCImp);
Datum
byteaModByteaCImp(PG_FUNCTION_ARGS)
{
    Numeric	result;

    Datum parm0 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
    Datum parm1 = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
    result = numeric_mod_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

PG_FUNCTION_INFO_V1(anycompatibleModByteaCImp);
Datum
anycompatibleModByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleModBytea(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaModAnycompatibleCImp);
Datum
byteaModAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return byteaModAnycompatible(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaAndByteaCImp);
Datum
byteaAndByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Int64;
    Datum parm1Int64;

    bool isDigit0 = varValueIsDigit(PG_GETARG_DATUM(0), BYTEAOID);
    bool isDigit1 = varValueIsDigit(PG_GETARG_DATUM(1), BYTEAOID);

    if (isDigit0)
    {
        Datum parm0Numeric = bytea2numeric_internal(
                                                    PG_GETARG_BYTEA_PP(0));
        parm0Int64 = DirectFunctionCall1(numeric_int8, parm0Numeric);
    }
    else
    {
        parm0Int64 = bytea2int8_internal(PG_GETARG_BYTEA_PP(0));
    }

    if (isDigit1)
    {
        Datum parm1Numeric = bytea2numeric_internal(
                                                    PG_GETARG_BYTEA_PP(1));
        parm1Int64 = DirectFunctionCall1(numeric_int8, parm1Numeric);
    }
    else
    {
        parm1Int64 = bytea2int8_internal(PG_GETARG_BYTEA_PP(1));
    }

    PG_RETURN_INT64(DatumGetInt64(parm0Int64) & DatumGetInt64(parm1Int64));
}

PG_FUNCTION_INFO_V1(anycompatibleAndByteaCImp);
Datum
anycompatibleAndByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleAndBytea(parmType, 
                                 PG_GETARG_DATUM(0), 
                                 PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaAndAnycompatibleCImp);
Datum
byteaAndAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleAndBytea(parmType, 
                                 PG_GETARG_DATUM(1), 
                                 PG_GETARG_DATUM(0));
}

PG_FUNCTION_INFO_V1(byteaOrByteaCImp);
Datum
byteaOrByteaCImp(PG_FUNCTION_ARGS)
{
    Datum parm0Int64;
    Datum parm1Int64;

    bool isDigit0 = varValueIsDigit(PG_GETARG_DATUM(0), BYTEAOID);
    bool isDigit1 = varValueIsDigit(PG_GETARG_DATUM(1), BYTEAOID);

    if (isDigit0)
    {
        Datum parm0Numeric = bytea2numeric_internal(PG_GETARG_BYTEA_PP(0));
        parm0Int64 = DirectFunctionCall1(numeric_int8, parm0Numeric);
    }
    else
    {
        parm0Int64 = bytea2int8_internal(PG_GETARG_BYTEA_PP(0));
    }

    if (isDigit1)
    {
        Datum parm1Numeric = bytea2numeric_internal(PG_GETARG_BYTEA_PP(1));
        parm1Int64 = DirectFunctionCall1(numeric_int8, parm1Numeric);
    }
    else
    {
        parm1Int64 = bytea2int8_internal(PG_GETARG_BYTEA_PP(1));
    }

    PG_RETURN_INT64(DatumGetInt64(parm0Int64) | DatumGetInt64(parm1Int64));
}

PG_FUNCTION_INFO_V1(anycompatibleOrByteaCImp);
Datum
anycompatibleOrByteaCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 0);
    return anycompatibleOrBytea(parmType, 
                                PG_GETARG_DATUM(0), 
                                PG_GETARG_DATUM(1));
}

PG_FUNCTION_INFO_V1(byteaOrAnycompatibleCImp);
Datum
byteaOrAnycompatibleCImp(PG_FUNCTION_ARGS)
{
    Oid parmType = getParameterType(fcinfo->flinfo, 1);
    return anycompatibleOrBytea(parmType, 
                                PG_GETARG_DATUM(1), 
                                PG_GETARG_DATUM(0));
}

static Oid 
getParameterType(FmgrInfo *flinfo, int parmIndex)
{
    Oid parmType;

    parmType = get_fn_expr_argtype(flinfo, parmIndex);
    if (!OidIsValid(parmType))
        elog(ERROR, "could not determine data type of input");

    parmType = getBaseType(parmType);

    return parmType;
}

static Datum 
anycompatibleEqBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_eq, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(texteq, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_eq, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(texteq, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_eq, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpchareq, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_eq, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_eq, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_eq, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_eq, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_eq, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_eq, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleLtBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}


static Datum 
anycompatibleNeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ne, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(textne, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ne, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(textne, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ne, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpcharne, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_ne, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_ne, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_ne, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_ne, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_ne, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ne, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleNeBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}

static Datum 
anycompatibleCondAndBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    bool isParm0ValNot0 = false;
    bool isParm1ValNot0 = false;

    Datum num0 = DirectFunctionCall3(numeric_in,
								     CStringGetDatum("0"),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    isParm0ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm0, num0));
    isParm1ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm1, num0));

    PG_RETURN_BOOL(isParm0ValNot0 && isParm1ValNot0);
}

static Datum 
anycompatibleCondOrBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    bool isParm0ValNot0 = false;
    bool isParm1ValNot0 = false;

    Datum num0 = DirectFunctionCall3(numeric_in,
								     CStringGetDatum("0"),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    isParm0ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm0, num0));
    isParm1ValNot0 = DatumGetBool(DirectFunctionCall2(numeric_ne, parm1, num0));

    PG_RETURN_BOOL(isParm0ValNot0 || isParm1ValNot0);
}

static Datum 
anycompatibleLikeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(texticlike, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleNlikeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(texticnlike, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleRegexpBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(texticregexeq, COLLATION, parm0, parm1);
}

static Datum 
byteaRegexpAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = DirectFunctionCall1(mys_bytea2text, parm0Val);
    Datum parm1 = convertParameterToText(parm1Type, parm1Val);
    return DirectFunctionCall2Coll(texticregexeq, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleNregexpBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(texticregexne, COLLATION, parm0, parm1);
}

static Datum 
byteaNregexpAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = DirectFunctionCall1(mys_bytea2text, parm0Val);
    Datum parm1 = convertParameterToText(parm1Type, parm1Val);
    return DirectFunctionCall2Coll(texticregexne, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleRegexpbinBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(textregexeq, COLLATION, parm0, parm1);
}

static Datum 
byteaRegexpbinAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = DirectFunctionCall1(mys_bytea2text, parm0Val);
    Datum parm1 = convertParameterToText(parm1Type, parm1Val);
    return DirectFunctionCall2Coll(textregexeq, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleNregexpbinBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = convertParameterToText(parm0Type, parm0Val);
    Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
    return DirectFunctionCall2Coll(textregexne, COLLATION, parm0, parm1);
}

static Datum 
byteaNregexpbinAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Datum parm0 = DirectFunctionCall1(mys_bytea2text, parm0Val);
    Datum parm1 = convertParameterToText(parm1Type, parm1Val);
    return DirectFunctionCall2Coll(textregexne, COLLATION, parm0, parm1);
}

static Datum 
anycompatibleLtBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_lt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(text_lt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_lt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(text_lt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_lt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpcharlt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_lt, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_lt, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_lt, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_lt, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_lt, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_lt, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleLtBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}

static Datum 
anycompatibleLeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_le, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(text_le, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_le, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(text_le, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_le, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpcharle, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_le, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_le, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_le, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_le, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_le, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_le, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleLtBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}

static Datum 
anycompatibleGtBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_gt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(text_gt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_gt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(text_gt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_gt, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpchargt, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_gt, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_gt, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_gt, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_gt, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_gt, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_gt, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleLtBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}

static Datum 
anycompatibleGeBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    if (parm0Type == INT2OID)
    {
        /* int2 (oid: 21) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == INT4OID)
    {
        /* int4 (oid: 23) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == INT8OID)
    {
        /* int8 (oid: 20) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == TEXTOID)
    {
        /* text (oid: 25) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ge, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2text, parm1Val);
            return DirectFunctionCall2Coll(text_ge, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ge, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2varchar, parm1Val);
            return DirectFunctionCall2Coll(text_ge, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == BPCHAROID)
    {
        /* char (oid: 1042) */
        bool isDigit = varValueIsDigit(parm1Val, BYTEAOID);
        if (isDigit)
        {
            Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
            Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
            return DirectFunctionCall2(numeric_ge, parm0, parm1);
        }
        else
        {
            Datum parm0 = parm0Val;
            Datum parm1 = DirectFunctionCall1(mys_bytea2bpchar, parm1Val);
            return DirectFunctionCall2Coll(bpcharge, COLLATION, parm0, parm1);
        }
    }
    else if (parm0Type == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == BOOLOID)
    {
        /* boolean (oid: 16) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamp, parm1Val);
        return DirectFunctionCall2(timestamp_ge, parm0, parm1);
    }
    else if (parm0Type == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timestamptz, parm1Val);
        return DirectFunctionCall2(timestamp_ge, parm0, parm1);
    }
    else if (parm0Type == TIMEOID)
    {
        /* time (oid: 1083) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2time, parm1Val);
        return DirectFunctionCall2(time_ge, parm0, parm1);
    }
    else if (parm0Type == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2timetz, parm1Val);
        return DirectFunctionCall2(timetz_ge, parm0, parm1);
    }
    else if (parm0Type == DATEOID)
    {
        /* date (oid: 1082) */
        Datum parm0 = parm0Val;
        Datum parm1 = DirectFunctionCall1(mys_bytea2date, parm1Val);
        return DirectFunctionCall2(date_ge, parm0, parm1);
    }
    else if (parm0Type == BITOID)
    {
        /* date (oid: 1560) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else if (parm0Type == VARBITOID)
    {
        /* date (oid: 1562) */
        Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
        Datum parm1 = DirectFunctionCall1(mys_bytea2numeric, parm1Val);
        return DirectFunctionCall2(numeric_ge, parm0, parm1);
    }
    else 
    {
        elog(WARNING, "Invalid parameter tpye \"%d\" for \"anycompatibleLtBytea\" function.", parm0Type);
        PG_RETURN_BOOL(false);
    }
}

static Datum 
anycompatiblePlBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Numeric	result;
    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    result = numeric_add_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);
    PG_RETURN_NUMERIC(result);
}

static Datum 
anycompatibleMiBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    result = numeric_sub_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);
    PG_RETURN_NUMERIC(result);
}

static Datum 
byteaMiAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
    Datum parm1 = convertParameterToNumeric(parm1Type, parm1Val);
    result = numeric_sub_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum 
anycompatibleMulBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    result = numeric_mul_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum 
anycompatibleDivBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    result = numeric_div_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum
byteaDivAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
    Datum parm1 = convertParameterToNumeric(parm1Type, parm1Val);
    result = numeric_div_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum 
anycompatibleModBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = convertParameterToNumeric(parm0Type, parm0Val);
    Datum parm1 = bytea2numeric_internal(DatumGetByteaPP(parm1Val));
    result = numeric_mod_opt_error(DatumGetNumeric(parm0), 
                                   DatumGetNumeric(parm1), 
                                   NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum
byteaModAnycompatible(Oid parm1Type, Datum parm0Val, Datum parm1Val)
{
    Numeric result;

    Datum parm0 = bytea2numeric_internal(DatumGetByteaPP(parm0Val));
    Datum parm1 = convertParameterToNumeric(parm1Type, parm1Val);
    result = numeric_mod_opt_error(DatumGetNumeric(parm0), DatumGetNumeric(parm1), NULL);

    PG_RETURN_NUMERIC(result);
}

static Datum 
anycompatibleAndBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    Datum parm0Int64 = convertParameterToInt8(parm0Type, parm0Val);
    Datum parm1Int64;

    if (isDigit1)
    {
        Datum parm1Numeric = bytea2numeric_internal(
                                                    DatumGetByteaPP(parm1Val));
        parm1Int64 = DirectFunctionCall1(numeric_int8, parm1Numeric);
    }
    else
    {
        parm1Int64 = bytea2int8_internal(DatumGetByteaPP(parm1Val));
    }

    PG_RETURN_INT64(DatumGetInt64(parm0Int64) & DatumGetInt64(parm1Int64));
}

static Datum 
anycompatibleOrBytea(Oid parm0Type, Datum parm0Val, Datum parm1Val)
{
    bool isDigit1 = varValueIsDigit(parm1Val, BYTEAOID);
    Datum parm0Int64 = convertParameterToInt8(parm0Type, parm0Val);
    Datum parm1Int64;

    if (isDigit1)
    {
        Datum parm1Numeric = bytea2numeric_internal(
                                                    DatumGetByteaPP(parm1Val));
        parm1Int64 = DirectFunctionCall1(numeric_int8, parm1Numeric);
    }
    else
    {
        parm1Int64 = bytea2int8_internal(DatumGetByteaPP(parm1Val));
    }

    PG_RETURN_INT64(DatumGetInt64(parm0Int64) | DatumGetInt64(parm1Int64));
}

/**
 * 暂时不用
static void 
truncateStrForDigit(size_t strLen, char *str)
{
    bool signExist = false;
    bool digitExist = false;
    bool dotExist = false;
    for (size_t index = 0; index < strLen; index++)
    {
        if (('0' <= str[index]) && (str[index] <= '9'))
        {
            if (!digitExist)
            {
                digitExist = true;
            }
        }
        else if ((str[index] == '+') || (str[index] == '-'))
        {
            if (!digitExist && !dotExist && !signExist)
            {
                signExist = true;
            }
            else 
            {
                str[0] = '0';
                str[1] = '\0';
                break;
            }
        }
        else if (str[index] == '.')
        {
            if (!dotExist)
            {
                dotExist = true;
            }
            else
            {
                str[index] = '\0';
                break;
            }
        }
        else if (str[index] == ' ')
        {
            if (!digitExist && !dotExist && !signExist)
            {
                continue;
            }
            else 
            {
                str[index] = '\0';
                break;
            }
        }
        else 
        {
            if (0 < index)
            {
                str[index] = '\0';
            }
            else 
            {
                str[0] = '0';
                str[1] = '\0';
            }
            break;
        }
    }
}
**/

static void 
truncateStrTailWhileSpacesForDigit(size_t strLen, char *str)
{
    for (int i = (strLen - 1); i >= 0; i--)
    {
        if (str[i] != ' ')
        {
            break;
        }
        else
        {
            str[i] = '\0';
        }
    }
}

static Datum 
convertParameterToText(Oid parmType, Datum parmVal)
{
    if (parmType == INT2OID)
    {
        /* int2 (oid: 21) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(int2out, parmVal)));
    }
    else if (parmType == INT4OID)
    {
        /* int4 (oid: 23) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(int4out, parmVal)));
    }
    else if (parmType == INT8OID)
    {
        /* int8 (oid: 20) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(int8out, parmVal)));
    }
    else if (parmType == TEXTOID)
    {
        /* text (oid: 25) */
        return parmVal;
    }
    else if (parmType == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(varcharout, parmVal)));
    }
    else if (parmType == BPCHAROID)
    {
        /* char (oid: 1042) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(bpcharout, parmVal)));
    }
    else if (parmType == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(numeric_out, parmVal)));
    }
    else if (parmType == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(float4out, parmVal)));
    }
    else if (parmType == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(float8out, parmVal)));
    }
    else if (parmType == BOOLOID)
    {
        /* boolean (oid: 16) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(boolout, parmVal)));
    }
    else if (parmType == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(timestamp_out, parmVal)));
    }
    else if (parmType == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(timestamptz_out, parmVal)));
    }
    else if (parmType == TIMEOID)
    {
        /* time (oid: 1083) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(time_out, parmVal)));
    }
    else if (parmType == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(timetz_out, parmVal)));
    }
    else if (parmType == DATEOID)
    {
        /* date (oid: 1082) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(date_out, parmVal)));
    }
    else if (parmType == BITOID)
    {
        /* date (oid: 1560) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(bit_out, parmVal)));
    }
    else if (parmType == VARBITOID)
    {
        /* date (oid: 1562) */
        return CStringGetTextDatum(DatumGetCString(DirectFunctionCall1(varbit_out, parmVal)));
    }
    else 
    {
        elog(ERROR, "Invalid parameter tpye \"%d\" for \"anycompatibleEqBytea\" function.", parmType);
    }
}

static Datum
convertParameterToNumeric(Oid parmType, Datum parm)
{
    Datum result;

    if (parmType == INT2OID)
    {
        /* int2 (oid: 21) */
        result = NumericGetDatum(int64_to_numeric(DatumGetInt16(parm)));
    }
    else if (parmType == INT4OID)
    {
        /* int4 (oid: 23) */
        result = NumericGetDatum(int64_to_numeric(DatumGetInt32(parm)));
    }
    else if (parmType == INT8OID)
    {
        /* int8 (oid: 20) */
        result = NumericGetDatum(int64_to_numeric(DatumGetInt64(parm)));
    }
    else if (parmType == TEXTOID)
    {
        /* text (oid: 25) */
        char *parmStr = TextDatumGetCString(parm);
        size_t parmStrLen = strlen(parmStr);
        truncateStrTailWhileSpacesForDigit(parmStrLen, parmStr);
        result = DirectFunctionCall3(numeric_in,
                                     CStringGetDatum(parmStr),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    }
    else if (parmType == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as float8 */
        char *parmStr = TextDatumGetCString(parm);
        size_t parmStrLen = strlen(parmStr);
        truncateStrTailWhileSpacesForDigit(parmStrLen, parmStr);
        result = DirectFunctionCall3(numeric_in,
                                     CStringGetDatum(parmStr),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    }
    else if (parmType == BPCHAROID)
    {
        /* char (oid: 1042) */
        char *parmStr = TextDatumGetCString(parm);
        size_t parmStrLen = strlen(parmStr);
        truncateStrTailWhileSpacesForDigit(parmStrLen, parmStr);
        result = DirectFunctionCall3(numeric_in,
                                     CStringGetDatum(parmStr),
                                     ObjectIdGetDatum(InvalidOid),
                                     Int32GetDatum(-1));
    }
    else if (parmType == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        result = parm;
    }
    else if (parmType == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        result = DirectFunctionCall1(float4_numeric, parm);
    }
    else if (parmType == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        result = DirectFunctionCall1(float8_numeric, parm);
    }
    else if (parmType == BOOLOID)
    {
        /* boolean (oid: 16) */
        char *parmStr;
        bool parmBool = DatumGetBool(parm);
        if (parmBool)
        {
            parmStr = "1";
        }
        else 
        {
            parmStr = "0";
        }
        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Timestamp parmTimestamp = DatumGetTimestamp(parm);
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        fsec_t fsec;
        char parmStr[MAXDATELEN + 1];

        if (timestamp2tm(parmTimestamp, NULL, tm, &fsec, NULL, NULL) == 0)
        {
            char *strp = parmStr;
            strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
            strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
            strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIMESTAMP_PRECISION, true);
            *strp = '\0';
        }		
        else
            ereport(ERROR,
                    (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                    errmsg("timestamp out of range")));

        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        TimestampTz parmTimestamptz = DatumGetTimestampTz(parm);
        int	tz;
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        fsec_t fsec;
        const char *tzn;
        char parmStr[MAXDATELEN + 1];

        if (timestamp2tm(parmTimestamptz, &tz, tm, &fsec, &tzn, NULL) == 0)
        {
            char *strp = parmStr;
            strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
            strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
            strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIMESTAMP_PRECISION, true);
            *strp = '\0';
        }
        else
            ereport(ERROR,
                    (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                    errmsg("timestamp out of range")));
        
        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == TIMEOID)
    {
        /* time (oid: 1083) */
        TimeADT parmTime = DatumGetTimeADT(parm);
        char parmStr[MAXDATELEN + 1];
        struct pg_tm tt;
		struct pg_tm *tm = &tt;
        fsec_t fsec;
        char buf[MAXDATELEN + 1];
        char *strp;
        bool nega = false;

        if (parmTime < 0)
        {
            nega = true;
            parmTime = parmTime * -1;
        }

        strp = buf;
        time2tm(parmTime, tm, &fsec);
        strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
        strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIME_PRECISION, true);
        *strp = '\0';

        if (nega)
        {
            pg_snprintf(parmStr, MAXDATELEN, "-%s", buf);
        }
        else
        {
            pg_snprintf(parmStr, MAXDATELEN, "%s", buf);
        }

        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        TimeTzADT *parmTimetz = DatumGetTimeTzADTP(parm);
        char parmStr[MAXDATELEN + 1];
        struct pg_tm tt;
		struct pg_tm *tm = &tt;
        fsec_t fsec;
        int tz;
        char buf[MAXDATELEN + 1];
        char *strp;
        bool nega = false;

        if (parmTimetz->time < 0)
        {
            nega = true;
            parmTimetz->time = parmTimetz->time * -1;
        }
        
        strp = buf;
        timetz2tm(parmTimetz, tm, &fsec, &tz);
        strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
        strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIME_PRECISION, true);
        *strp = '\0';

        if (nega)
        {
            pg_snprintf(parmStr, MAXDATELEN, "-%s", buf);
        }
        else
        {
            pg_snprintf(parmStr, MAXDATELEN, "%s", buf);
        }

        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == DATEOID)
    {
        /* date (oid: 1082) */
        DateADT parmDate = DatumGetDateADT(parm);
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        char parmStr[MAXDATELEN + 1];
        char *strp;

        strp = parmStr;
        j2date(parmDate + POSTGRES_EPOCH_JDATE,
			   &(tm->tm_year), &(tm->tm_mon), &(tm->tm_mday));
        strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
        strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
        *strp = '\0';

        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == BITOID)
    {
        /* bit (oid: 1560) */
        int64 parmNum;
        char parmStr[30 + 1];
        parmNum = (int64)DirectFunctionCall1(bittoint8, parm);
        pg_snprintf(parmStr, 30, "%lu", (uint64)parmNum);
        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == VARBITOID)
    {
        /* varbit (oid: 1562) */
        //result = // convert to float8
        int64 parmNum;
        char parmStr[30 + 1];
        parmNum = (int64)DirectFunctionCall1(bittoint8, parm);
        pg_snprintf(parmStr, 30, "%lu", (uint64)parmNum);
        result = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
    }
    else if (parmType == BYTEAOID)
    {
        result = bytea2numeric_internal(DatumGetByteaPP(parm));
    }
    else 
    {
        elog(ERROR, "Invalid parameter tpye \"%d\" for \"anycompatibleEqBytea\" function.", parmType);
    }

    return result;
}

static Datum
convertParameterToInt8(Oid parmType, Datum parm)
{
    Datum result;

    if (parmType == INT2OID)
    {
        /* int2 (oid: 21) */
        int16 parmVal = DatumGetInt16(parm);

        result = Int64GetDatum((int64)parmVal);
    }
    else if (parmType == INT4OID)
    {
        /* int4 (oid: 23) */
        int32 parmVal = DatumGetInt32(parm);

        result = Int64GetDatum((int64)parmVal);
    }
    else if (parmType == INT8OID)
    {
        /* int8 (oid: 20) */
        result = parm;
    }
    else if (parmType == TEXTOID)
    {
        /* text (oid: 25) */
        int64 resultTemp;
        char *parmVal = TextDatumGetCString(parm);
        scanint8(parmVal, false, &resultTemp);
        result = Int64GetDatum(resultTemp);
    }
    else if (parmType == VARCHAROID)
    {
        /* varchar (oid: 1043) operator is same as text */
        int64 resultTemp;
        char *parmVal = TextDatumGetCString(parm);
        scanint8(parmVal, false, &resultTemp);
        result = Int64GetDatum(resultTemp);
    }
    else if (parmType == BPCHAROID)
    {
        /* char (oid: 1042) */
        int64 resultTemp;
        char *parmVal = TextDatumGetCString(parm);
        /* TODO: 去掉尾部空格 */
        scanint8(parmVal, false, &resultTemp);
        result = Int64GetDatum(resultTemp);
    }
    else if (parmType == NUMERICOID)
    {
        /* numeric (oid: 1700) */
        result = DirectFunctionCall1(numeric_int8, parm);
    }
    else if (parmType == FLOAT4OID)
    {
        /* float4 (oid: 700) */
        result = DirectFunctionCall1(ftoi8, parm);
    }
    else if (parmType == FLOAT8OID)
    {
        /* float8 (oid: 701) */
        result = DirectFunctionCall1(dtoi8, parm);
    }
    else if (parmType == BOOLOID)
    {
        /* boolean (oid: 16) */
        bool parmVal = DatumGetBool(parm);
        if (parmVal)
        {
            result = Int64GetDatum((int64)1);
        }
        else 
        {
            result = Int64GetDatum((int64)0);
        }
    }
    else if (parmType == TIMESTAMPOID)
    {
        /* timestamp (oid: 1114) */
        Timestamp parmTimestamp = DatumGetTimestamp(parm);
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        fsec_t fsec;
        char parmStr[MAXDATELEN + 1];
        Datum numericTemp;

        if (timestamp2tm(parmTimestamp, NULL, tm, &fsec, NULL, NULL) == 0)
        {
            char *strp = parmStr;
            strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
            strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
            strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIMESTAMP_PRECISION, true);
            *strp = '\0';
        }		
        else
            ereport(ERROR,
                    (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                    errmsg("timestamp out of range")));
        
        numericTemp = DirectFunctionCall3(numeric_in,
								          CStringGetDatum(parmStr),
                                          ObjectIdGetDatum(InvalidOid),
                                          Int32GetDatum(-1));
        
        result = DirectFunctionCall1(numeric_int8, numericTemp);
    }
    else if (parmType == TIMESTAMPTZOID)
    {
        /* timestamptz (oid: 1184) */
        TimestampTz parmTimestamptz = DatumGetTimestampTz(parm);
        int	tz;
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        fsec_t fsec;
        const char *tzn;
        char parmStr[MAXDATELEN + 1];
        Datum numericTemp;

        if (timestamp2tm(parmTimestamptz, &tz, tm, &fsec, &tzn, NULL) == 0)
        {
            char *strp = parmStr;
            strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
            strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
            strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
            strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIMESTAMP_PRECISION, true);
            *strp = '\0';
        }
        else
            ereport(ERROR,
                    (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                    errmsg("timestamp out of range")));
        
        numericTemp = DirectFunctionCall3(numeric_in,
                                          CStringGetDatum(parmStr),
                                          ObjectIdGetDatum(InvalidOid),
                                          Int32GetDatum(-1));
        
        result = DirectFunctionCall1(numeric_int8, numericTemp);
    }
    else if (parmType == TIMEOID)
    {
        /* time (oid: 1083) */
        TimeADT parmTime = DatumGetTimeADT(parm);
        char parmStr[MAXDATELEN + 1];
        struct pg_tm tt;
		struct pg_tm *tm = &tt;
        fsec_t fsec;
        char buf[MAXDATELEN + 1];
        char *strp;
        Datum numericTemp;
        bool nega = false;

        if (parmTime < 0)
        {
            nega = true;
            parmTime = parmTime * -1;
        }

        strp = buf;
        time2tm(parmTime, tm, &fsec);
        strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
        strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIME_PRECISION, true);
        *strp = '\0';

        if (nega)
        {
            pg_snprintf(parmStr, MAXDATELEN, "-%s", buf);
        }
        else
        {
            pg_snprintf(parmStr, MAXDATELEN, "%s", buf);
        }

        numericTemp = DirectFunctionCall3(numeric_in,
                                          CStringGetDatum(parmStr),
                                          ObjectIdGetDatum(InvalidOid),
                                          Int32GetDatum(-1));
        
        result = DirectFunctionCall1(numeric_int8, numericTemp);
    }
    else if (parmType == TIMETZOID)
    {
        /* timetz (oid: 1266) */
        TimeTzADT *parmTimetz = DatumGetTimeTzADTP(parm);
        char parmStr[MAXDATELEN + 1];
        struct pg_tm tt;
		struct pg_tm *tm = &tt;
        fsec_t fsec;
        int tz;
        char buf[MAXDATELEN + 1];
        char *strp;
        Datum numericTemp;
        bool nega = false;

        if (parmTimetz->time < 0)
        {
            nega = true;
            parmTimetz->time = parmTimetz->time * -1;
        }
        
        strp = buf;
        timetz2tm(parmTimetz, tm, &fsec, &tz);
        strp = pg_ultostr_zeropad(strp, tm->tm_hour, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_min, 2);
        strp = AppendSeconds(strp, tm->tm_sec, fsec, MAX_TIME_PRECISION, true);
        *strp = '\0';

        if (nega)
        {
            pg_snprintf(parmStr, MAXDATELEN, "-%s", buf);
        }
        else
        {
            pg_snprintf(parmStr, MAXDATELEN, "%s", buf);
        }

        numericTemp = DirectFunctionCall3(numeric_in,
								     CStringGetDatum(parmStr),
								     ObjectIdGetDatum(InvalidOid),
								     Int32GetDatum(-1));
        
        result = DirectFunctionCall1(numeric_int8, numericTemp);
    }
    else if (parmType == DATEOID)
    {
        /* date (oid: 1082) */
        DateADT parmDate = DatumGetDateADT(parm);
        struct pg_tm tt;
        struct pg_tm *tm = &tt;
        char parmStr[MAXDATELEN + 1];
        char *strp;

        strp = parmStr;
        j2date(parmDate + POSTGRES_EPOCH_JDATE,
			   &(tm->tm_year), &(tm->tm_mon), &(tm->tm_mday));
        strp = pg_ultostr_zeropad(strp, (tm->tm_year > 0) ? tm->tm_year : -(tm->tm_year - 1), 4);
        strp = pg_ultostr_zeropad(strp, tm->tm_mon, 2);
        strp = pg_ultostr_zeropad(strp, tm->tm_mday, 2);
        *strp = '\0';

        result = DirectFunctionCall1(int8in,
								     CStringGetDatum(parmStr));
    }
    else if (parmType == BITOID)
    {
        /* date (oid: 1560) */
        result = DirectFunctionCall1(bittoint8,
                                     parm);
    }
    else if (parmType == VARBITOID)
    {
        /* date (oid: 1562) */
        result = DirectFunctionCall1(bittoint8,
                                     parm);
    }
    else 
    {
        elog(ERROR, "Invalid parameter tpye \"%d\" for \"anycompatibleEqBytea\" function.", parmType);
    }

    return result;
}

/*
 * Append seconds and fractional seconds (if any) at *cp.
 *
 * precision is the max number of fraction digits, fillzeros says to
 * pad to two integral-seconds digits.
 *
 * Returns a pointer to the new end of string.  No NUL terminator is put
 * there; callers are responsible for NUL terminating str themselves.
 *
 * Note that any sign is stripped from the input seconds values.
 */
static char *
AppendSeconds(char *cp, int sec, fsec_t fsec, int precision, bool fillzeros)
{
    Assert(precision >= 0);

    if (fillzeros)
        cp = pg_ultostr_zeropad(cp, Abs(sec), 2);
    else
        cp = pg_ultostr(cp, Abs(sec));

    /* fsec_t is just an int32 */
    if (fsec != 0)
    {
        int32		value = Abs(fsec);
        char	   *end = &cp[precision + 1];
        bool		gotnonzero = false;

        *cp++ = '.';

        /*
         * Append the fractional seconds part.  Note that we don't want any
         * trailing zeros here, so since we're building the number in reverse
         * we'll skip appending zeros until we've output a non-zero digit.
         */
        while (precision--)
        {
            int32		oldval = value;
            int32		remainder;

            value /= 10;
            remainder = oldval - value * 10;

            /* check if we got a non-zero */
            if (remainder)
                gotnonzero = true;

            if (gotnonzero)
                cp[precision] = '0' + remainder;
            else
                end = &cp[precision];
        }

        /*
         * If we still have a non-zero value then precision must have not been
		 * enough to print the number.  We punt the problem to pg_ltostr(),
		 * which will generate a correct answer in the minimum valid width.
		 */
		if (value)
			return pg_ultostr(cp, Abs(fsec));

		return end;
	}
	else
		return cp;
}

//static Datum 
//convertByteaToInt2(Datum val)
//{
//    // TODO:
//    return NULL;
//}
//
//static Datum 
//convertByteaToInt4(Datum val)
//{
//    // TODO:
//    return NULL;
//}
//
//static Datum 
//convertByteaToInt8(Datum val)
//{
//    // TODO:
//    return NULL;
//}
//
//static Datum
//convertByteaToText(Datum val)
//{
//    // TODO:
//    return NULL;
//}
//
//static Datum
//convertByteaToVarchar(Datum val)
//{
//    // TODO:
//    return NULL;
//}
//
//static Datum
//convertByteaToChar(Datum val)
//{
//    // TODO:
//    return NULL;
//}

