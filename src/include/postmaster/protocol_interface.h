/*-------------------------------------------------------------------------
 *
 * protocol_interface.h
 *	  Wired protocol interface
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
 *	  src/include/postmaster/protocol_interface.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PROTOCOL_INTERFACE_H
#define PROTOCOL_INTERFACE_H

#include "lib/stringinfo.h"
#include "tcop/dest.h"


typedef struct ProtocolRoutine ProtocolInterface;
typedef struct QueryCompletion QueryCompletion;
typedef struct TupleTableSlot TupleTableSlot;
/*
 * ProtocolRoutine
 *
 * 	All the callbacks implementing a specific wire protocol
 *  AWS Babelfish compatible
 */
typedef void    (*fn_listen_init)(void);
typedef int		(*fn_accept)(pgsocket server_fd, struct Port *port);
typedef void	(*fn_close)(pgsocket server_fd);
typedef void	(*fn_init)(void);
typedef int		(*fn_start)(struct Port *port);
typedef void	(*fn_authenticate)(struct Port *port, const char **username);
typedef void	(*fn_mainfunc)(struct Port *port, int argc, char *argv[]) pg_attribute_noreturn();
typedef void	(*fn_send_message)(ErrorData *edata);
typedef void	(*fn_send_cancel_key)(int pid, int32 key);
typedef void	(*fn_comm_reset)(void);
typedef bool	(*fn_is_reading_msg)(void);
typedef void	(*fn_send_ready_for_query)(CommandDest dest);
typedef int		(*fn_read_command)(StringInfo inBuf);
typedef void	(*fn_end_command)(QueryCompletion *qc, CommandDest dest);
typedef bool	(*fn_printtup)(TupleTableSlot *slot, DestReceiver *self, CommandTag  commandTag);
typedef void	(*fn_printtup_startup)(DestReceiver *self, int operation, TupleDesc typeinfo, CommandTag  commandTag);
typedef void	(*fn_printtup_shutdown)(DestReceiver *self);
typedef void	(*fn_printtup_destroy)(DestReceiver *self);
typedef int		(*fn_process_command)(int *first_char, StringInfo inBuf);
typedef void	(*fn_report_param_status)(const char *name, char *val);

typedef struct ProtocolRoutine {
	NodeTag						type;
	
    fn_listen_init              listen_init;
	fn_accept					accept;
	fn_close					close;
	fn_init						init;
	fn_start					start;
	fn_authenticate				authenticate;
	fn_mainfunc					mainfunc;
	fn_send_message			    send_message;
	fn_send_cancel_key			send_cancel_key;
	fn_comm_reset				comm_reset;
	fn_is_reading_msg			is_reading_msg;
	fn_send_ready_for_query		send_ready_for_query;
	fn_read_command				read_command;
	fn_end_command				end_command;
	fn_printtup					printtup;
	fn_printtup_startup			printtup_startup;
	fn_printtup_shutdown		printtup_shutdown;
	fn_printtup_destroy			printtup_destroy;
	fn_process_command			process_command;
	fn_report_param_status		report_param_status;
} ProtocolRoutine;


#endif							/* PROTOCOL_INTERFACE_H */
