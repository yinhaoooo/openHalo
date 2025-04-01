/*-------------------------------------------------------------------------
 *
 * mys_prepare.c
 *	  Extra routines for create function handling
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
 *	  src/backend/commands/mysql/mys_prepare.c
 *
 *-------------------------------------------------------------------------
 */

/*
 * 该.c文件被src/backend/commands/prepare.c包含
 *
 */

#include "commands/mysql/mys_uservar.h"
#include "mb/pg_wchar.h"
#include "nodes/mysql/mys_parsenodes.h"
#include "nodes/nodes.h"
#include "parser/parser.h"
#include "adapter/mysql/adapter.h"

static bool mysUtilityCanPrepare(Node *parsetree);

/*
 * Implements the 'PREPARE' utility statement for MySQL.
 */
void
mys_PrepareQuery(ParseState *pstate, PrepareStmt *stmt,
			     int stmt_location, int stmt_len)
{
    List *raw_parsetree_list;
	RawStmt *rawstmt;
    char *prepareStmt;
	CachedPlanSource *plansource;
	Oid	*argtypes = NULL;
	int	nargs;
	Query *query;
	List *query_list;
	int i;

	/*
	 * Disallow empty-string statement name (conflicts with protocol-level
	 * unnamed statement).
	 */
	if (!stmt->name || stmt->name[0] == '\0')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PSTATEMENT_DEFINITION),
				 errmsg("invalid statement name: must not be empty")));

	DropPreparedStatement(stmt->name, false);

	/*
	 * Need to wrap the contained statement in a RawStmt node to pass it to
	 * parse analysis.
	 */
    prepareStmt = NULL;
    if (nodeTag(stmt->query) == T_String)
    {
		prepareStmt = strVal(stmt->query);
    }
    else if (nodeTag(stmt->query) == T_UserVarRef)
    {
        char *userVarName = ((UserVarRef *)(stmt->query))->userVarName;
        bytea *userVarValue = mysGetUserVarValueInternal(userVarName);

        if (userVarValue != NULL)
        {
            char *byte = VARDATA_ANY(userVarValue);
            size_t byteLen = VARSIZE_ANY_EXHDR(userVarValue);

            prepareStmt = pnstrdup(byte, byteLen);

            if (pg_verifymbstr(byte, byteLen, true))
            {
                /* Nothing to do */
            }
            else
            {
                elog(ERROR, "user variable %s is not a string literal", userVarName);
            }
        }
        else
        {
            elog(ERROR, "user variable %s is NULL", userVarName);
        }
    }
    else
    {
        /* mys_gram.y保证不会到这里 */
    }

    raw_parsetree_list = raw_parser(prepareStmt, RAW_PARSE_DEFAULT);
	rawstmt = (RawStmt *)linitial(raw_parsetree_list);
	rawstmt->stmt_location = stmt_location;
	rawstmt->stmt_len = stmt_len;

	/*
	 * Create the CachedPlanSource before we do parse analysis, since it needs
	 * to see the unmodified raw parse tree.
	 */
	plansource = CreateCachedPlan(rawstmt, pstate->p_sourcetext,
								  CMD_UNKNOWN);

	/* Transform list of TypeNames to array of type OIDs */
	nargs = list_length(stmt->argtypes);

	if (nargs)
	{
		ListCell   *l;

		argtypes = (Oid *) palloc(nargs * sizeof(Oid));
		i = 0;

		foreach(l, stmt->argtypes)
		{
			TypeName   *tn = lfirst(l);
			Oid			toid = typenameTypeId(pstate, tn);

			argtypes[i++] = toid;
		}
	}

	/*
	 * Analyze the statement using these parameter types (any parameters
	 * passed in from above us will not be visible to it), allowing
	 * information about unknown parameters to be deduced from context.
	 */
	query = parse_analyze_varparams(rawstmt, pstate->p_sourcetext,
									&argtypes, &nargs);

    plansource->commandTag = CreateCommandTag((Node *)query);

	/*
	 * Check that all parameter types were determined.
	 */
	for (i = 0; i < nargs; i++)
	{
		Oid			argtype = argtypes[i];

		if (argtype == InvalidOid || argtype == UNKNOWNOID)
			ereport(ERROR,
					(errcode(ERRCODE_INDETERMINATE_DATATYPE),
					 errmsg("could not determine data type of parameter $%d",
							i + 1)));
	}

	/*
	 * grammar only allows PreparableStmt, so this check should be redundant
	 */
	switch (query->commandType)
	{
		case CMD_SELECT:
			{
				int attIndex = 0;
				ListCell *lc;

				foreach (lc, query->targetList)
				{
					TargetEntry *tle = lfirst(lc);

					if (tle->resjunk)
						continue;
					mys_setCurrentPreStmtColumnInfo(attIndex, exprType((Node *) tle->expr));
					attIndex++;
				}
			}
			break;
		case CMD_INSERT:
		case CMD_UPDATE:
		case CMD_DELETE:
			/* OK */
			break;
        case CMD_UTILITY:
            if (mysUtilityCanPrepare(query->utilityStmt))
            {
                /* OK */
            }
            else
            {
                ereport(ERROR,
					    (errcode(ERRCODE_INVALID_PSTATEMENT_DEFINITION),
					     errmsg("This command is not supported in the prepared statement protocol yet")));
            }
            break;
		default:
            /* should not reach here */
			ereport(ERROR,
                    (errcode(ERRCODE_INVALID_PSTATEMENT_DEFINITION),
                     errmsg("This command is not supported in the prepared statement protocol yet")));
			break;
	}

	/* Rewrite the query. The result could be 0, 1, or many queries. */
	query_list = QueryRewrite(query);

	/* Finish filling in the CachedPlanSource */
	CompleteCachedPlan(plansource,
					   query_list,
					   NULL,
					   argtypes,
					   nargs,
					   NULL,
					   NULL,
					   CURSOR_OPT_PARALLEL_OK,	/* allow parallel mode */
					   true);	/* fixed result */

	/*
	 * Save the results.
	 */
	StorePreparedStatement(stmt->name,
						   plansource,
						   true);
}

static bool
mysUtilityCanPrepare(Node *parsetree)
{
    switch (nodeTag(parsetree))
    {
        case T_MysVariableSetStmt:
        case T_MysSelectIntoStmt:
            return true;
        
        case T_CreateStmt:
        case T_CreateTableAsStmt:
            return true;
        
        case T_AlterTableStmt:
            return true;

        case T_CallStmt:
            return true;
		
		case T_ViewStmt:
			return true;
		
		case T_DropStmt:
			{
				DropStmt *stmt = (DropStmt *)parsetree;
				if (stmt->removeType == OBJECT_INDEX ||
					stmt->removeType == OBJECT_SCHEMA ||
					stmt->removeType == OBJECT_TABLE ||
					stmt->removeType == OBJECT_VIEW ||
					stmt->removeType == OBJECT_SEQUENCE)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
        
        default:
            return false;
    }
}
