/*-------------------------------------------------------------------------
 *
 * netTransceiver.c
 *	  MySQL adapter netTransceiver routines
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
 *	  src/backend/adapter/mysql/netTransceiver.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "libpq/libpq.h"
#include "miscadmin.h"
#include "storage/ipc.h"
#include "utils/elog.h"
#include "utils/memutils.h"
#include "adapter/mysql/netTransceiver.h"


NetTransceiver *netTransceiver = NULL;
int halo_mysql_max_allowed_packet = 67108864;


static int readBytes(char *buff, size_t len);
static int readAndProcessPacketHeader(void);
static bool readPacketPayload(StringInfo inBuf, int payloadLen);
static bool readAndProcessPacket(StringInfo inBuf);
static bool readPayloadForLogon(StringInfo inBuf);
static bool readPayload(StringInfo inBuf);
static void getWriteBufForHeaderPayload(char **buf);
static void getWriteBufForPayload(char **buf);
static void getBufForPayload(char **buf, int bufSize);
static void finishWriteToBufNoFlush(size_t writedLen);
static void finishWriteToBufFlush(size_t writedLen);
static int flushWBuff(void);
static int writeBytes(const char *buff, size_t len);
static bool writePacketHeaderNoFlush(size_t payloadLen);
static bool writePacketPayloadNoFlush(const char *payload, size_t payloadLen);
static bool writePacketPayloadFlush(const char *payload, size_t payloadLen);
static bool writePacketHeaderPayloadNoFlush(const char *payload, size_t payloadLen);
static bool writePacketHeaderPayloadFlush(const char *payload, size_t payloadLen);


void
initNetTransceiver(void)
{
    MemoryContext oldContext;

    oldContext = MemoryContextSwitchTo(TopMemoryContext);

    netTransceiver = palloc0(sizeof(NetTransceiver));
    netTransceiver->mysPort = MyProcPort;
    netTransceiver->mysPort->noblock = false;  /* set to block */
    netTransceiver->netPacketSeqID = 0;
    netTransceiver->writePointer = 0;
    netTransceiver->sendStart = 0;
    netTransceiver->WBuffSize = WBUFF_INITIAL_SIZE;
    netTransceiver->WBuff = palloc0(WBUFF_INITIAL_SIZE);

    netTransceiver->readPayloadForLogon = readPayloadForLogon;
    netTransceiver->readPayload = readPayload;
    netTransceiver->getWriteBufForHeaderPayload = getWriteBufForHeaderPayload;
    netTransceiver->getWriteBufForPayload = getWriteBufForPayload;
    netTransceiver->getBufForPayload = getBufForPayload;
    netTransceiver->finishWriteToBufNoFlush = finishWriteToBufNoFlush;
    netTransceiver->finishWriteToBufFlush = finishWriteToBufFlush;
    netTransceiver->writePacketHeaderNoFlush = writePacketHeaderNoFlush;
    netTransceiver->writePacketPayloadNoFlush = writePacketPayloadNoFlush;
    netTransceiver->writePacketPayloadFlush = writePacketPayloadFlush;
    netTransceiver->writePacketHeaderPayloadNoFlush = writePacketHeaderPayloadNoFlush;
    netTransceiver->writePacketHeaderPayloadFlush = writePacketHeaderPayloadFlush;

    MemoryContextSwitchTo(oldContext);
}


static int
readBytes(char *buff, size_t len)
{
    while (0 < len)
    {
        int ret = 0;
        ret = secure_read(netTransceiver->mysPort, buff, len);
        if (0 < ret)
        {
            len -= ret;
            buff += ret;
        }
        else if (ret == 0)
		{
			return EOF;
		}
        else
		{
			if (errno == EINTR)
				continue;

			ereport(COMMERROR,
					(errcode_for_socket_access(),
					 errmsg("could not receive data from client: %m")));
			return EOF;
		}
    }

    return 0;
}


static int
readAndProcessPacketHeader(void)
{
    int payloadLen = 0;
    int readRet;
    char buff[EACH_PACKET_HEARDER_LENGTH] = {0};

    readRet = readBytes(buff, EACH_PACKET_HEARDER_LENGTH);
    if (readRet == 0)
    {
        int packetSeqIdFromFront = 0;

        memcpy(&payloadLen, buff, 3);

        packetSeqIdFromFront = (uint8)(buff[3]);
        if (packetSeqIdFromFront == netTransceiver->netPacketSeqID)
        {
            netTransceiver->netPacketSeqID++;
            return payloadLen;
        }
        else
        {
            elog(ERROR, 
                 "The packet sequence number from client is error, except %d get %d.", 
                 netTransceiver->netPacketSeqID, packetSeqIdFromFront);
            return EOF;
        }
    }
    else
    {
        return EOF;
    }
}


static bool
readPacketPayload(StringInfo inBuf, int payloadLen)
{
    if (payloadLen > (inBuf->maxlen - inBuf->offset - inBuf->len))
    {
        enlargeStringInfo(inBuf, (payloadLen + inBuf->offset));
    }

    if (readBytes((inBuf->data + inBuf->offset + inBuf->len), payloadLen) == 0)
    {
        inBuf->len += payloadLen;
        return true;
    }
    else
    {
        return false;
    }
}


static bool
readAndProcessPacket(StringInfo inBuf)
{
    int payloadLen;

    payloadLen = readAndProcessPacketHeader();
    if (payloadLen == EOF)
    {
        return false;
    }

    if (payloadLen > halo_mysql_max_allowed_packet)
    {
        elog(ERROR, 
             "The length of payload(%d) from client bigger than\
             'max_allowed_packet(%d)' bytes. Connection will be disconnect.", 
             payloadLen, 
             halo_mysql_max_allowed_packet);
        proc_exit(1);
    }

    if (payloadLen < SINGLE_PACKET_MAX_PAYLOAD_LENGTH)
    {
        readPacketPayload(inBuf, payloadLen);
    }
    else
    {
        readPacketPayload(inBuf, payloadLen);
        do
        {
            payloadLen = readAndProcessPacketHeader();
            if (payloadLen == EOF)
            {
                return false;
            }

            if ((inBuf->len + payloadLen) > halo_mysql_max_allowed_packet)
            {
                elog(ERROR, 
                     "The length of payload(%d) from client bigger than 'max_allowed_packet(%d)' bytes. Connection will be disconnect.", 
                     (inBuf->len + payloadLen), halo_mysql_max_allowed_packet);
                proc_exit(1);
            }

            if (readPacketPayload(inBuf, payloadLen) == false)
            {
                return false;
            }
        } while (payloadLen == SINGLE_PACKET_MAX_PAYLOAD_LENGTH);
    }

    inBuf->data[inBuf->len + inBuf->offset] = '\0';

    return true;
}


static bool
readPayloadForLogon(StringInfo inBuf)
{
    resetStringInfo(inBuf);

    if (readAndProcessPacket(inBuf))
    {
        return true;
    }
    else
    {
        elog(ERROR, "Client has disconnect when read.");
        proc_exit(1);
    }
}


static bool
readPayload(StringInfo inBuf)
{
    resetStringInfo(inBuf);

    netTransceiver->netPacketSeqID = 0;

    if (readAndProcessPacket(inBuf))
    {
        return true;
    }
    else
    {
        elog(ERROR, "Client has disconnect when read.");
        proc_exit(1);
    }
}


static void 
getWriteBufForHeaderPayload(char **buf)
{
    if (102400 <= (netTransceiver->WBuffSize - netTransceiver->writePointer))
    {
        *buf = netTransceiver->WBuff + netTransceiver->writePointer + 4;
    }
    else 
    {
        if (flushWBuff() != EOF)
        {
            *buf = netTransceiver->WBuff + netTransceiver->writePointer + 4;
        }
        else
        {
            return;
        }
    }
}


static void 
getWriteBufForPayload(char **buf)
{
    if (102400 <= (netTransceiver->WBuffSize - netTransceiver->writePointer))
    {
        *buf = netTransceiver->WBuff + netTransceiver->writePointer;
    }
    else 
    {
        if (flushWBuff() != EOF)
        {
            *buf = netTransceiver->WBuff + netTransceiver->writePointer;
        }
        else
        {
            return;
        }
    }
}


static void 
getBufForPayload(char **buf, int bufSize)
{
    *buf = pallocO(bufSize);
}


static void 
finishWriteToBufNoFlush(size_t writedLen)
{
    netTransceiver->writePointer += writedLen;
}


static void 
finishWriteToBufFlush(size_t writedLen)
{
    netTransceiver->writePointer += writedLen;

    if (flushWBuff() == EOF)
    {
        proc_exit(1);
    }
}


static int
flushWBuff(void)
{
    char *sendBeginPtr = netTransceiver->WBuff + netTransceiver->sendStart;
	char *sendEndPtr = netTransceiver->WBuff + netTransceiver->writePointer;

    while (sendBeginPtr < sendEndPtr)
    {
        int ret = 0;
        ret = secure_write(netTransceiver->mysPort, 
                           sendBeginPtr, 
                           (sendEndPtr - sendBeginPtr));
        if (0 < ret)
        {
            sendBeginPtr += ret;
            netTransceiver->sendStart += ret;
        }
        else if (ret == 0)
        {
			netTransceiver->sendStart = 0;
			netTransceiver->writePointer = 0;
			ClientConnectionLost = 1;
			InterruptPending = 1;
            return EOF;
        }
        else
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                proc_exit(1);
            }
        }
    }

    netTransceiver->sendStart = 0;
    netTransceiver->writePointer = 0;

    return 0;
}


static int
writeBytes(const char *buff, size_t len)
{
    int amount = 0;

    while (len > 0)
    {
        if (netTransceiver->writePointer >= netTransceiver->WBuffSize)
        {
            if (flushWBuff() == EOF)
            {
                return EOF;
            }
        }

        amount = netTransceiver->WBuffSize - netTransceiver->writePointer;
        if (amount > len)
        {
			amount = len;
        }
        memcpy((netTransceiver->WBuff + netTransceiver->writePointer), buff, amount);
		netTransceiver->writePointer += amount;
		buff += amount;
		len -= amount;
    }

    return 0;
}


static bool
writePacketHeaderNoFlush(size_t payloadLen)
{
    char buff[EACH_PACKET_HEARDER_LENGTH] = {0};

    memcpy(buff, &payloadLen, 3);
    memcpy((buff + 3), &(netTransceiver->netPacketSeqID), 1);
    if (netTransceiver->netPacketSeqID < 255)
    {
        netTransceiver->netPacketSeqID++;
    }
    else 
    {
        netTransceiver->netPacketSeqID = 0;
    }

    if (writeBytes(buff, EACH_PACKET_HEARDER_LENGTH) != EOF)
    {
        return true;
    }
    else
    {
        elog(ERROR, "Client has disconnect when write.");
        proc_exit(1);
    }
}


static bool
writePacketPayloadNoFlush(const char *payload, size_t payloadLen)
{
    if (writeBytes(payload, payloadLen) != EOF)
    {
        return true;
    }
    else
    {
        elog(ERROR, "Client has disconnect when write.");
        proc_exit(1);
    }
}


static bool
writePacketPayloadFlush(const char *payload, size_t payloadLen)
{
    if (writeBytes(payload, payloadLen) != EOF)
    {
        /* do nothing; */
    }
    else
    {
        elog(ERROR, "Client has disconnect when write.");
        proc_exit(1);
    }

    if (flushWBuff() != EOF)
    {
        return true;
    }
    else
    {
        return EOF;
    }
}


static bool
writePacketHeaderPayloadNoFlush(const char *payload, size_t payloadLen)
{
    while (payloadLen >= SINGLE_PACKET_MAX_PAYLOAD_LENGTH)
    {
        if (writePacketHeaderNoFlush(SINGLE_PACKET_MAX_PAYLOAD_LENGTH))
        {
            if (writePacketPayloadNoFlush(payload, SINGLE_PACKET_MAX_PAYLOAD_LENGTH))
            {
                payloadLen -= SINGLE_PACKET_MAX_PAYLOAD_LENGTH;
                payload += SINGLE_PACKET_MAX_PAYLOAD_LENGTH;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    if (writePacketHeaderNoFlush(payloadLen))
    {
        if (writePacketPayloadNoFlush(payload, payloadLen))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


static bool
writePacketHeaderPayloadFlush(const char *payload, size_t payloadLen)
{
    if (writePacketHeaderPayloadNoFlush(payload, payloadLen))
    {
        /* do nothing; */
    }
    else
    {
        elog(ERROR, "Client has disconnect when write.");
        proc_exit(1);
    }

    if (flushWBuff() != EOF)
    {
        return true;
    }
    else
    {
        elog(ERROR, "Client has disconnect when write.");
        proc_exit(1);
    }
}

