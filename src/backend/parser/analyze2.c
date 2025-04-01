/*-------------------------------------------------------------------------
 *
 * analyze2.c
 *		extra routines to parse analyze
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
 *	  src/backend/parser/analyze2.c
 *
 *-------------------------------------------------------------------------
 */
Query *
transformSelectStmt(ParseState *pstate, SelectStmt *stmt)
{
	Query	   *query;

	Assert(parserengine != NULL);

	if (parserengine->transformSelectStmt)
		query = parserengine->transformSelectStmt(pstate, stmt);
	else
		query = standard_transformSelectStmt(pstate, stmt);

	return query;
}

Query *
transformInsertStmt(ParseState *pstate, InsertStmt *stmt)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformInsertStmt)
		query = parserengine->transformInsertStmt(pstate, stmt);
	else
		query = standard_transformInsertStmt(pstate, stmt);

	return query;
}

Query *
transformDeleteStmt(ParseState *pstate, DeleteStmt *stmt)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformDeleteStmt)
		query = parserengine->transformDeleteStmt(pstate, stmt);
	else
		query = standard_transformDeleteStmt(pstate, stmt);
	
	return query;
}

Query *
transformUpdateStmt(ParseState *pstate, UpdateStmt *stmt)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformUpdateStmt)
		query = parserengine->transformUpdateStmt(pstate, stmt);
	else
		query = standard_transformUpdateStmt(pstate, stmt);
	
	return query;
}

Query *
transformValuesClause(ParseState *pstate, SelectStmt *stmt)
{
	Query	*query;

	query = standard_transformValuesClause(pstate, stmt);

	return query;
}

Query *
transformSetOperationStmt(ParseState *pstate, SelectStmt *stmt)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformSetOperationStmt)
		query = parserengine->transformSetOperationStmt(pstate, stmt);
	else
		query = standard_transformSetOperationStmt(pstate, stmt);
	
	return query;
}

Query *
transformReturnStmt(ParseState *pstate, ReturnStmt *stmt)
{
	Query	*query;

	query = standard_transformReturnStmt(pstate, stmt);
	
	return query;
}

Query *
transformPLAssignStmt(ParseState *pstate, PLAssignStmt *stmt)
{
	Query	*query;

	query = standard_transformPLAssignStmt(pstate, stmt);
	
	return query;
}

Query *
transformDeclareCursorStmt(ParseState *pstate, DeclareCursorStmt *stmt)
{
	Query	*query;

	query = standard_transformDeclareCursorStmt(pstate, stmt);
	
	return query;
}

Query *
transformExplainStmt(ParseState *pstate, ExplainStmt *stmt)
{
	Query	*query;

	query = standard_transformExplainStmt(pstate, stmt);
	
	return query;
}

Query *
transformCreateTableAsStmt(ParseState *pstate, CreateTableAsStmt *stmt)
{
	Query	*query;

	query = standard_transformCreateTableAsStmt(pstate, stmt);
	
	return query;
}

Query *
transformCallStmt(ParseState *pstate, CallStmt *stmt)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformCallStmt)
		query = parserengine->transformCallStmt(pstate, stmt);
	else
		query = standard_transformCallStmt(pstate, stmt);
	
	return query;
}

List *
transformReturningList(ParseState *pstate, List *returningList)
{
	List	*result;

	result = standard_transformReturningList(pstate, returningList);
	
	return result;
}

List *
transformInsertRow(ParseState *pstate, List *exprlist,
				   List *stmtcols, List *icolumns, List *attrnos,
				   bool strip_indirection)
{
	List	*result;

	result = standard_transformInsertRow(pstate, exprlist,
											 stmtcols, icolumns, attrnos,
											 strip_indirection);

	return result;
}

OnConflictExpr *
transformOnConflictClause(ParseState *pstate,
						  OnConflictClause *onConflictClause)
{
	OnConflictExpr	*xpr;
	
	xpr = standard_transformOnConflictClause(pstate, onConflictClause);
	
	return xpr;
}

List *
transformUpdateTargetList(ParseState *pstate, List *origTlist)
{
	List	*result;

	result = standard_transformUpdateTargetList(pstate, origTlist);

	return result;
}

void
transformLockingClause(ParseState *pstate, Query *qry, LockingClause *lc,
					   bool pushedDown)
{
	standard_transformLockingClause(pstate, qry, lc, pushedDown);
}

Query *
transformOptionalSelectInto(ParseState *pstate, Node *parseTree)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->transformOptionalSelectInto)
		query = parserengine->transformOptionalSelectInto(pstate, parseTree);
	else
		query = standard_transformOptionalSelectInto(pstate, parseTree);
	
	return query;
}

Node *
transformSetOperationTree(ParseState *pstate, SelectStmt *stmt,
						  bool isTopLevel, List **targetlist)
{
	Node	*n;

	Assert(parserengine != NULL);

	if (parserengine->transformSetOperationTree)
		n = parserengine->transformSetOperationTree(pstate, stmt, isTopLevel, targetlist);
	else
		n = standard_transformSetOperationTree(pstate, stmt, isTopLevel, targetlist);
	
	return n;
}

Query *
parse_sub_analyze(Node *parseTree, ParseState *parentParseState,
					CommonTableExpr *parentCTE,
					bool locked_from_parent,
					bool resolve_unknowns)
{
	Query	*query;

	Assert(parserengine != NULL);

	if (parserengine->parse_sub_analyze)
		query = parserengine->parse_sub_analyze(parseTree, parentParseState,
												parentCTE,
												locked_from_parent,
												resolve_unknowns);
	else
		query = standard_parse_sub_analyze(parseTree, parentParseState,
											parentCTE,
											locked_from_parent,
											resolve_unknowns);	
	return query;
}