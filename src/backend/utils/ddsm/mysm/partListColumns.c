/*-------------------------------------------------------------------------
 *
 * partListColumns.c
 *	  Extend strfuncs routines
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
 *	  src/backend/utils/ddsm/mysm/partListColumns.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <ctype.h>
#include <limits.h>
#include <strings.h>

#include "access/detoast.h"
#include "access/toast_compression.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_type.h"
#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"


static FmgrInfo *build_concat_foutcache(FunctionCallInfo fcinfo, int argidx);


PG_FUNCTION_INFO_V1(partListColumns);
Datum
partListColumns(PG_FUNCTION_ARGS)
{
	text *result;
    StringInfoData str;
    FmgrInfo *foutcache;

    initStringInfo(&str);
    foutcache = (FmgrInfo *) fcinfo->flinfo->fn_extra;
    if (foutcache == NULL)
    {
        foutcache = build_concat_foutcache(fcinfo, 0);
    }

    appendStringInfoString(&str, "(");
    for (int i = 0; i < PG_NARGS(); i++)
    {
        if (0 < i)
        {
            appendStringInfoString(&str, ",");
        }

        if (!PG_ARGISNULL(i))
        {
            Datum value = PG_GETARG_DATUM(i);
            Oid valtype = get_fn_expr_argtype(fcinfo->flinfo, i);
            Oid valBaseType = getBaseType(valtype);

            if (valBaseType != BYTEAOID)
            {
                appendStringInfoString(&str, 
                                       OutputFunctionCall(&foutcache[i], value));
            }
            else 
            {
                bytea *vlena = DatumGetByteaPP(value);
                char *byte = VARDATA_ANY(vlena);
                size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
                pg_verifymbstr(byte, byteLen, false);
                appendBinaryStringInfo(&str, byte, byteLen);
            }
        }
    }
    appendStringInfoString(&str, ")");

    result = cstring_to_text_with_len(str.data, str.len);
    pfree(str.data);

    PG_RETURN_TEXT_P(result);
}


static FmgrInfo *
build_concat_foutcache(FunctionCallInfo fcinfo, int argidx)
{
	FmgrInfo   *foutcache;
	int			i;

	/* We keep the info in fn_mcxt so it survives across calls */
	foutcache = (FmgrInfo *) MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
												PG_NARGS() * sizeof(FmgrInfo));

	for (i = argidx; i < PG_NARGS(); i++)
	{
		Oid			valtype;
		Oid			typOutput;
		bool		typIsVarlena;

		valtype = get_fn_expr_argtype(fcinfo->flinfo, i);
		if (!OidIsValid(valtype))
			elog(ERROR, "could not determine data type of concat() input");

		getTypeOutputInfo(valtype, &typOutput, &typIsVarlena);
		fmgr_info_cxt(typOutput, &foutcache[i], fcinfo->flinfo->fn_mcxt);
	}

	fcinfo->flinfo->fn_extra = foutcache;

	return foutcache;
}

