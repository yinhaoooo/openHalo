/*-------------------------------------------------------------------------
 *
 * errorConvertor.c
 *	  MySQL adapter errorConvertor routines
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
 *	  src/backend/adapter/mysql/errorConvertor.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "utils/hsearch.h"
#include "adapter/mysql/errorConvertor.h"


typedef struct 
{
    int haloErrorCode;   /* hash key must be first */
    int mySQLErrorCode;
} HaloMySQLErrorCode;
static HTAB *haloMysqlErrorCodes = NULL;

static void initErrorCode(int haloErrorCode, int mySQLErrorCode);


static void 
initErrorCode(int haloErrorCode, int mySQLErrorCode)
{
    HaloMySQLErrorCode *ret = NULL;
    bool found = false;

    ret = (HaloMySQLErrorCode *)hash_search(haloMysqlErrorCodes, 
                                            &haloErrorCode, 
                                            HASH_ENTER, 
                                            &found);
    if (found)
    {
        /* do nothing; */
    }
    else
    {
        ret->mySQLErrorCode = mySQLErrorCode;
    }
}


void 
initErrorCodeHashTable(void)
{
	HASHCTL hashctl;
	hashctl.keysize = sizeof(int);
	hashctl.entrysize = sizeof(HaloMySQLErrorCode);
    haloMysqlErrorCodes = hash_create("Halo MySQL(914) Error Codes", 
                                      32768, 
                                      &hashctl, 
                                      HASH_ELEM | HASH_BLOBS);

    /* 有个比较麻烦的问题，halo的错误码和mysql的错误码不是一一对应的 */
    initErrorCode(1064, 1265);
    initErrorCode(1292, 1366);
    initErrorCode(1364, 1048);
    initErrorCode(1411, 1049);
    initErrorCode(2600, 1292);
    initErrorCode(16777248, 1264);
    initErrorCode(16777346, 1406);
    initErrorCode(16801924, 1478);
    initErrorCode(16908420, 1146);
    initErrorCode(33575106, 1048);
    initErrorCode(33685634, 1064);
    initErrorCode(50331778, 1264);
    initErrorCode(50352322, 1452);
    initErrorCode(50360452, 1054);
    initErrorCode(67391682, 1264);
    initErrorCode(83906754, 1062);
    initErrorCode(117571716, 1050);
    //initErrorCode(ERRCODE_STRING_DATA_RIGHT_TRUNCATION, 
    //              HALO_ERR_DATA_TOO_LONG);
    //initErrorCode(ERRCODE_INVALID_TEXT_REPRESENTATION, 
    //              HALO_ERR_TRUNCATED_WRONG_VALUE_FOR_FIELD);
    //initErrorCode(ERRCODE_NOT_NULL_VIOLATION, 
    //              HALO_ERR_BAD_NULL_ERROR);
    //initErrorCode(ERRCODE_FOREIGN_KEY_VIOLATION, 
    //              HALO_ERR_NO_REFERENCED_ROW_2);
    //initErrorCode(ERRCODE_UNIQUE_VIOLATION, 
    //              HALO_ERR_DUP_ENTRY);
    //initErrorCode(ERRCODE_SYNTAX_ERROR, 
    //              HALO_ERR_PARSE_ERROR);
    //initErrorCode(ERRCODE_NAME_TOO_LONG, 
    //              HALO_ERR_TOO_LONG_IDENT);
    //initErrorCode(ERRCODE_AMBIGUOUS_COLUMN, 
    //              HALO_ERR_NON_UNIQ_ERROR);
    //initErrorCode(ERRCODE_UNDEFINED_COLUMN, 
    //              HALO_ERR_BAD_FIELD_ERROR);
    //initErrorCode(ERRCODE_DUPLICATE_ALIAS, 
    //              HALO_ERR_NONUNIQ_TABLE);
    //initErrorCode(ERRCODE_UNDEFINED_FUNCTION, 
    //              HALO_ERR_SP_DOES_NOT_EXIST);
    //initErrorCode(ERRCODE_UNDEFINED_TABLE, 
    //              HALO_ERR_BAD_TABLE_ERROR);
    //initErrorCode(ERRCODE_DUPLICATE_TABLE, 
    //              HALO_ERR_TABLE_EXISTS_ERROR);
    //initErrorCode(ERRCODE_DATETIME_FIELD_OVERFLOW, 
    //              HALO_ERR_TRUNCATED_WRONG_VALUE);
    //initErrorCode(ERRCODE_UNDEFINED_OBJECT, 
    //              HALO_ERR_ACCESS_DENIED_ERROR);
}

int 
convertErrorCode(int haloErrorCode)
{
    HaloMySQLErrorCode *ret = NULL;
    bool found = false;

    if (haloMysqlErrorCodes != NULL)
    {
        ret = (HaloMySQLErrorCode *)hash_search(haloMysqlErrorCodes, 
                                                &haloErrorCode, 
                                                HASH_FIND, 
                                                &found);
        if (found)
        {
            return ret->mySQLErrorCode;
        }
        else
        {
            return haloErrorCode;
        }
    }
    else
    {
        return haloErrorCode;
    }
}

