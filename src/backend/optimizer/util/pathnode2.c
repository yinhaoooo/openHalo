/*-------------------------------------------------------------------------
 *
 * pathnode2.c
 *		extra routines to manipulate pathlists
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
 * IDENTIFICATION
 *	  src/backend/optimizer/util/pathnode2.c
 *
 *-------------------------------------------------------------------------
 */
#include "optimizer/pathnode2.h"

bool _enable_nestloop_antiunique_ = false;
bool _enable_nestloop_semiunique_ = false;
/*
 * create_modifytable_path2
 *	  Almost like create_modifytable_path, but
 * 
 * mergeActionLists: merge action list
 *
 */
ModifyTablePath *
create_modifytable_path2(PlannerInfo *root, RelOptInfo *rel,
						 Path *subpath,
						 CmdType operation, bool canSetTag,
						 Index nominalRelation, Index rootRelation,
						 bool partColsUpdated,
						 List *resultRelations,
						 List *updateColnosLists,
						 List *withCheckOptionLists, List *returningLists,
						 List *rowMarks, OnConflictExpr *onconflict,
						 List *mergeActionLists, int epqParam)
{
	ModifyTablePath *pathnode = makeNode(ModifyTablePath);

	Assert(operation == CMD_UPDATE ?
		   list_length(resultRelations) == list_length(updateColnosLists) :
		   updateColnosLists == NIL);
	Assert(withCheckOptionLists == NIL ||
		   list_length(resultRelations) == list_length(withCheckOptionLists));
	Assert(returningLists == NIL ||
		   list_length(resultRelations) == list_length(returningLists));

	pathnode->path.pathtype = T_ModifyTable;
	pathnode->path.parent = rel;
	/* pathtarget is not interesting, just make it minimally valid */
	pathnode->path.pathtarget = rel->reltarget;
	/* For now, assume we are above any joins, so no parameterization */
	pathnode->path.param_info = NULL;
	pathnode->path.parallel_aware = false;
	pathnode->path.parallel_safe = false;
	pathnode->path.parallel_workers = 0;
	pathnode->path.pathkeys = NIL;

	/*
	 * Compute cost & rowcount as subpath cost & rowcount (if RETURNING)
	 *
	 * Currently, we don't charge anything extra for the actual table
	 * modification work, nor for the WITH CHECK OPTIONS or RETURNING
	 * expressions if any.  It would only be window dressing, since
	 * ModifyTable is always a top-level node and there is no way for the
	 * costs to change any higher-level planning choices.  But we might want
	 * to make it look better sometime.
	 */
	pathnode->path.startup_cost = subpath->startup_cost;
	pathnode->path.total_cost = subpath->total_cost;
	if (returningLists != NIL)
	{
		pathnode->path.rows = subpath->rows;

		/*
		 * Set width to match the subpath output.  XXX this is totally wrong:
		 * we should return an average of the RETURNING tlist widths.  But
		 * it's what happened historically, and improving it is a task for
		 * another day.  (Again, it's mostly window dressing.)
		 */
		pathnode->path.pathtarget->width = subpath->pathtarget->width;
	}
	else
	{
		pathnode->path.rows = 0;
		pathnode->path.pathtarget->width = 0;
	}

	pathnode->subpath = subpath;
	pathnode->operation = operation;
	pathnode->canSetTag = canSetTag;
	pathnode->nominalRelation = nominalRelation;
	pathnode->rootRelation = rootRelation;
	pathnode->partColsUpdated = partColsUpdated;
	pathnode->resultRelations = resultRelations;
	pathnode->updateColnosLists = updateColnosLists;
	pathnode->withCheckOptionLists = withCheckOptionLists;
	pathnode->returningLists = returningLists;
	pathnode->rowMarks = rowMarks;
	pathnode->onconflict = onconflict;
    pathnode->mergeActionLists = mergeActionLists;
	pathnode->epqParam = epqParam;

	return pathnode;
}