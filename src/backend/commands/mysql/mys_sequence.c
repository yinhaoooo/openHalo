/*-------------------------------------------------------------------------
 *
 * mys_sequence.c
 *	  MySQL sequence process
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
 *	  src/backend/commands/mysql/mys_sequence.c
 *
 *-------------------------------------------------------------------------
 */

/*
 * 该.c文件被src/backend/commands/sequence.c包含
 *
 */


/*
 * 如果next > seq->last_value, 设置seq->last_value = next
 * 否则，什么都不做
 */
void
mys_setval3_oid(Oid seqOid, int64 next, bool isCalled)
{
    SeqTable elm;
	Relation seqRel;
    Buffer buf;
    HeapTupleData seqTuple;
    Form_pg_sequence_data seq;

    /* open and lock sequence */
	init_sequence(seqOid, &elm, &seqRel);

    if (pg_class_aclcheck(seqOid, GetUserId(), ACL_SELECT | ACL_USAGE | ACL_UPDATE) != ACLCHECK_OK)
		ereport(ERROR,
				(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
				 errmsg("permission denied for sequence %s",
						RelationGetRelationName(seqRel))));

    seq = read_seq_tuple(seqRel, &buf, &seqTuple);

    if (seq->last_value < next)
    {
        HeapTuple pgstuple;
        Form_pg_sequence pgsform;
        int64 maxv;
		int64 minv;

        pgstuple = SearchSysCache1(SEQRELID, ObjectIdGetDatum(seqOid));
        if (!HeapTupleIsValid(pgstuple))
            elog(ERROR, "cache lookup failed for sequence %u", seqOid);
        pgsform = (Form_pg_sequence) GETSTRUCT(pgstuple);
        maxv = pgsform->seqmax;
        minv = pgsform->seqmin;
        ReleaseSysCache(pgstuple);

        /* read-only transactions may only modify temp sequences */
        if (!seqRel->rd_islocaltemp)
            PreventCommandIfReadOnly("mys_setval_oid()");

        /*
         * Forbid this during parallel operation because, to make it work, the
         * cooperating backends would need to share the backend-local cached
         * sequence information.  Currently, we don't support that.
         */
        PreventCommandIfParallelMode("mys_setval_oid()");

        if ((next < minv) || (next > maxv))
        {
            char bufv[100];
            char bufm[100];
            char bufx[100];

            snprintf(bufv, sizeof(bufv), INT64_FORMAT, next);
            snprintf(bufm, sizeof(bufm), INT64_FORMAT, minv);
            snprintf(bufx, sizeof(bufx), INT64_FORMAT, maxv);
            ereport(ERROR,
                    (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                    errmsg("mys_setval_oid: value %s is out of bounds for sequence \"%s\" (%s..%s)",
                            bufv, RelationGetRelationName(seqRel),
                            bufm, bufx)));
        }

        /* Set the currval() state only if iscalled = true */
        if (isCalled)
        {
            elm->last = next;		/* last returned number */
            elm->last_valid = true;
        }

        /* In any case, forget any future cached numbers */
        elm->cached = elm->last;

        /* check the comment above nextval_internal()'s equivalent call. */
        if (RelationNeedsWAL(seqRel))
            GetTopTransactionId();

        /* ready to change the on-disk (or really, in-buffer) tuple */
        START_CRIT_SECTION();

        seq->last_value = next;		/* last fetched number */
        seq->is_called = isCalled;
        seq->log_cnt = 0;

        MarkBufferDirty(buf);

        /* XLOG stuff */
        if (RelationNeedsWAL(seqRel))
        {
            xl_seq_rec xlrec;
            XLogRecPtr recptr;
            Page page = BufferGetPage(buf);

            XLogBeginInsert();
            XLogRegisterBuffer(0, buf, REGBUF_WILL_INIT);

            xlrec.node = seqRel->rd_node;
            XLogRegisterData((char *) &xlrec, sizeof(xl_seq_rec));
            XLogRegisterData((char *) seqTuple.t_data, seqTuple.t_len);

            recptr = XLogInsert(RM_SEQ_ID, XLOG_SEQ_LOG);

            PageSetLSN(page, recptr);
        }

        END_CRIT_SECTION();
    }
    else
    {
        /* Nothing to do */
    }

    UnlockReleaseBuffer(buf);
	relation_close(seqRel, NoLock);
}
