/*-------------------------------------------------------------------------
 *
 * postmaster2.c
 *	  Wired protocol extend
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
 *	  src/backend/postmaster/postmaster2.c
 *
 *-------------------------------------------------------------------------
 */

const ProtocolInterface     *secondProtocolHandler = NULL;

static const ProtocolInterface    *ListenHandler[MAXLISTEN];
static const ProtocolInterface    standard_protocol_handler = {
    .type = T_StandardProtocol,

    .listen_init = NULL,
    .accept = standard_accept,
    .close = standard_close,
    .init = standard_init,
    .start = standard_start,
    .authenticate = standard_authenticate,
    .mainfunc = standard_mainfunc,
    .send_message =standard_send_message,
    .send_cancel_key = standard_send_cancel_key,
    .comm_reset = standard_comm_reset,
    .is_reading_msg = standard_is_reading_msg,
    .send_ready_for_query = standard_send_ready_for_query,
    .read_command = standard_read_command,
    .end_command = standard_end_command,
    .printtup = NULL,
    .printtup_startup = NULL,
    .printtup_shutdown = NULL,
    .printtup_destroy = NULL,
    .process_command = NULL,
    .report_param_status = standard_report_param_status
};

/*
 * setSocketProtocolHandler
 *     Set listen socket corresponding handler
 */
static void
setSocketProtocolHandler(int index, const ProtocolInterface *handler)
{
    Assert(index < MAXLISTEN);
    Assert(handler != NULL);

    ListenHandler[index] = handler;
}

static void
setStandardSocketProtocolHandler(int index)
{
    setSocketProtocolHandler(index, &standard_protocol_handler);
}

static void
setSecondSocketProtocolHandler(int index)
{
    setSocketProtocolHandler(index, secondProtocolHandler);
}

/*
 * setProtocolSocket
 *     Add opened socket to ListenSocket array.
 */
void
setStandardProtocolSocket(pgsocket fd)
{
    int	listen_index = 0;

	/* Lookup the next free slot */
	for (; listen_index < MAXLISTEN; listen_index++)
	{
		if (ListenSocket[listen_index] == PGINVALID_SOCKET)
			break;
	}

    Assert(listen_index < MAXLISTEN);

    ListenSocket[listen_index] = fd;
    setStandardSocketProtocolHandler(listen_index);
}


/*
 * setSecondProtocolSocket
 *     Add opened socket to ListenSocket array.
 */
void
setSecondProtocolSocket(pgsocket fd)
{
    int	listen_index = 0;

	/* Lookup the next free slot */
	for (; listen_index < MAXLISTEN; listen_index++)
	{
		if (ListenSocket[listen_index] == PGINVALID_SOCKET)
			break;
	}

    Assert(listen_index < MAXLISTEN);

    ListenSocket[listen_index] = fd;
    setSecondSocketProtocolHandler(listen_index);
}


/*
 * getProtocolHandlerByFd
 *     Find corresponding protocol handler by socket id
 */
const ProtocolInterface *
getProtocolHandlerByFd(int serverFd)
{
    int listen_index = 0;

    for (; listen_index < MAXLISTEN; listen_index ++)
    {
        if (ListenSocket[listen_index] == serverFd)
            break;
    }

    Assert(listen_index < MAXLISTEN);

    return ListenHandler[listen_index];
}

/*
 * getProtocolHandlerByFd
 *     Find corresponding protocol handler by listen index
 */
const ProtocolInterface *
getProtocolHandlerByIndex(int index)
{
    Assert(index < MAXLISTEN);

    return ListenHandler[index];
}

/*
 * getStandardProtocolHandler
 *     Get standard protocol handler
 */
const ProtocolInterface *
getStandardProtocolHandler(void)
{
    return &standard_protocol_handler;
}

/*
 * getSecondProtocolHandler
 *     Get Second protocol handler
 */
const ProtocolInterface *
getSecondProtocolHandler(void)
{
    ProtocolInterface *handler = NULL;
    
    switch (database_compat_mode)
    {
        case POSTGRESQL_COMPAT_MODE:
            ereport(FATAL,
					    (errmsg("second listener only works for MySQL mode")));
			break;
		case MYSQL_COMPAT_MODE:
            {
                handler = getMysProtocolHandler();
                break;
            }
		
		default:
            ereport(FATAL,
                    (errmsg("second listener only works for MySQL mode")));
			break;
    }

    return handler;
}

/*
 * setSecondProtocolHandler
 *     Set the second protocol handler
 */
void
setSecondProtocolHandler(const ProtocolInterface *handler)
{
    Assert(handler != NULL);

    secondProtocolHandler = handler;
}

int
standard_accept(pgsocket server_fd, Port *port)
{
	return StreamConnection(server_fd, port);
}

void
standard_close(pgsocket server_fd)
{
	StreamClose(server_fd);
}

void
standard_init(void)
{
	pq_init();
}

int
standard_start(Port *port)
{
	return ProcessStartupPacket(port, false, false);
}

void
standard_mainfunc(Port *port, int argc, char *argv[])
{
	PostgresMain(argc, argv, port->database_name, port->user_name);
}

void
standard_send_cancel_key(int pid, int32 key)
{
	StringInfoData buf;

	pq_beginmessage(&buf, 'K');
	pq_sendint32(&buf, (int32) pid);
	pq_sendint32(&buf, (int32) key);
	pq_endmessage(&buf);
	/* Need not flush since ReadyForQuery will do it. */

}

void
standard_report_param_status(const char *name, char *val)
{
	StringInfoData msgbuf;

	pq_beginmessage(&msgbuf, 'S');
	pq_sendstring(&msgbuf, name);
	pq_sendstring(&msgbuf, val);
	pq_endmessage(&msgbuf);
}

void
standard_comm_reset(void)
{
	pq_comm_reset();
}

bool
standard_is_reading_msg(void)
{
	return pq_is_reading_msg();
}

void
standard_send_ready_for_query(CommandDest dest)
{
	ReadyForQuery(dest);
}

void
standard_end_command(QueryCompletion *qc, CommandDest dest)
{
	EndCommand(qc, dest, false);
} 

int
listen_have_free_slot(void)
{
	int	listen_index = 0;

	/* See if there is still room to add 1 more socket. */
	for (; listen_index < MAXLISTEN; listen_index++)
	{
		if (ListenSocket[listen_index] == PGINVALID_SOCKET)
			return true;
	}

	ereport(LOG,
			(errmsg("could not bind to all requested addresses: MAXLISTEN (%d) exceeded",
					MAXLISTEN)));

	return false;
}
