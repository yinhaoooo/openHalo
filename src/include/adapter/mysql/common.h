/*-------------------------------------------------------------------------
 *
 * common.h
 *	  MySQL adapter common routines
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
 *	  src/include/adapter/mysql/com.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>

#include "c.h"
#include "port.h"


#define MYS_MAX_PACKET_LENGTH (256L * 256L * 256L - 1)

#define MYS_CLT_LONG_PASSWORD 1
#define MYS_CLT_FOUND_ROWS 2
#define MYS_CLT_LONG_FLAG 4
#define MYS_CLT_CONNECT_WITH_DB 8
#define MYS_CLT_NO_SCHEMA 16
#define MYS_CLT_COMPRESS 32
#define MYS_CLT_ODBC 64
#define MYS_CLT_LOCAL_FILES 128
#define MYS_CLT_IGNORE_SPACE 256
#define MYS_CLT_PROTOCOL_41 512
#define MYS_CLT_INTERACTIVE 1024
#define MYS_CLT_SSL 2048
#define MYS_CLT_IGNORE_SIGPIPE 4096
#define MYS_CLT_TRANSACTIONS 8192
#define MYS_CLT_RESERVED 16384
#define MYS_CLT_RESERVED2 32768
#define MYS_CLT_MULTI_STATEMENTS (1UL << 16)
#define MYS_CLT_MULTI_RESULTS (1UL << 17)
#define MYS_CLT_PS_MULTI_RESULTS (1UL << 18)
#define MYS_CLT_PLUGIN_AUTH (1UL << 19)
#define MYS_CLT_CONNECT_ATTRS (1UL << 20)
#define MYS_CLT_PLUGIN_AUTH_LENENC_CLIENT_DATA (1UL << 21)
#define MYS_CLT_CAN_HANDLE_EXPIRED_PASSWORDS (1UL << 22)
#define MYS_CLT_SESSION_TRACK (1UL << 23)
#define MYS_CLT_DEPRECATE_EOF (1UL << 24)
#define MYS_CLT_OPTIONAL_RESULTSET_METADATA (1UL << 25)
#define MYS_CLT_ZSTD_COMPRESSION_ALGORITHM (1UL << 26)
#define MYS_CLT_QUERY_ATTRIBUTES (1UL << 27)
#define MYS_CLT_MULTI_FACTOR_AUTHENTICATION (1UL << 28)
#define MYS_CLT_CAPABILITY_EXTENSION (1UL << 29)
#define MYS_CLT_SSL_VERIFY_SERVER_CERT (1UL << 30)
#define MYS_CLT_REMEMBER_OPTIONS (1UL << 31)

enum SERVER_STATUS_FLAGS
{
    HALO_SVR_STATUS_IN_TRANS = 1,
    HALO_SVR_STATUS_AUTOCOMMIT = 2, 
    HALO_SVR_MORE_RESULTS_EXISTS = 8,
    HALO_SVR_QUERY_NO_GOOD_INDEX_USED = 16,
    HALO_SVR_QUERY_NO_INDEX_USED = 32,
    HALO_SVR_STATUS_CURSOR_EXISTS = 64,
    HALO_SVR_STATUS_LAST_ROW_SENT = 128,
    HALO_SVR_STATUS_DB_DROPPED = 256,
    HALO_SVR_STATUS_NO_BACKSLASH_ESCAPES = 512,
    HALO_SVR_STATUS_METADATA_CHANGED = 1024,
    HALO_SVR_QUERY_WAS_SLOW = 2048,
    HALO_SVR_PS_OUT_PARAMS = 4096,
    HALO_SVR_STATUS_IN_TRANS_READONLY = 8192,
    HALO_SVR_SESSION_STATE_CHANGED = (1UL << 14)
};


#endif                          /* COMMON_H */

