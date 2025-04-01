/*-------------------------------------------------------------------------
 *
 * parse_oper.c
 *		handle operator things for parser
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/parser/parse_oper.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/htup_details.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_type.h"
#include "lib/stringinfo.h"
#include "nodes/nodeFuncs.h"
#include "parser/parse_coerce.h"
#include "parser/parse_func.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/builtins.h"
#include "utils/inval.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "utils/typcache.h"
#include "parser/mysql/mys_parse_oper.h"


/*
 * The lookup key for the operator lookaside hash table.  Unused bits must be
 * zeroes to ensure hashing works consistently --- in particular, oprname
 * must be zero-padded and any unused entries in search_path must be zero.
 *
 * search_path contains the actual search_path with which the entry was
 * derived (minus temp namespace if any), or else the single specified
 * schema OID if we are looking up an explicitly-qualified operator name.
 *
 * search_path has to be fixed-length since the hashtable code insists on
 * fixed-size keys.  If your search path is longer than that, we just punt
 * and don't cache anything.
 */

/* If your search_path is longer than this, sucks to be you ... */
#define MAX_CACHED_PATH_LEN		16

typedef struct OprCacheKey
{
	char		oprname[NAMEDATALEN];
	Oid			left_arg;		/* Left input OID, or 0 if prefix op */
	Oid			right_arg;		/* Right input OID */
	Oid			search_path[MAX_CACHED_PATH_LEN];
} OprCacheKey;

typedef struct OprCacheEntry
{
	/* the hash lookup key MUST BE FIRST */
	OprCacheKey key;

	Oid			opr_oid;		/* OID of the resolved operator */
} OprCacheEntry;


//static Oid	binary_oper_exact(List *opname, Oid arg1, Oid arg2);
//static FuncDetailCode oper_select_candidate(int nargs,
//											Oid *input_typeids,
//											FuncCandidateList candidates,
//											Oid *operOid);
static const char *op_signature_string(List *op, char oprkind,
									   Oid arg1, Oid arg2);
//static void op_error(ParseState *pstate, List *op, char oprkind,
//					 Oid arg1, Oid arg2,
//					 FuncDetailCode fdresult, int location);
//static bool make_oper_cache_key(ParseState *pstate, OprCacheKey *key,
//								List *opname, Oid ltypeId, Oid rtypeId,
//								int location);
//static Oid	find_oper_cache_entry(OprCacheKey *key);
//static void make_oper_cache_entry(OprCacheKey *key, Oid opr_oid);
//static void InvalidateOprCacheCallBack(Datum arg, int cacheid, uint32 hashvalue);


///* binary_oper_exact()
// * Check for an "exact" match to the specified operand types.
// *
// * If one operand is an unknown literal, assume it should be taken to be
// * the same type as the other operand for this purpose.  Also, consider
// * the possibility that the other operand is a domain type that needs to
// * be reduced to its base type to find an "exact" match.
// */
//static Oid
//binary_oper_exact(List *opname, Oid arg1, Oid arg2)
//{
//	Oid			result;
//	bool		was_unknown = false;
//
//	/* Unspecified type for one of the arguments? then use the other */
//	if ((arg1 == UNKNOWNOID) && (arg2 != InvalidOid))
//	{
//		arg1 = arg2;
//		was_unknown = true;
//	}
//	else if ((arg2 == UNKNOWNOID) && (arg1 != InvalidOid))
//	{
//		arg2 = arg1;
//		was_unknown = true;
//	}
//
//	result = OpernameGetOprid(opname, arg1, arg2);
//	if (OidIsValid(result))
//		return result;
//
//	if (was_unknown)
//	{
//		/* arg1 and arg2 are the same here, need only look at arg1 */
//		Oid			basetype = getBaseType(arg1);
//
//		if (basetype != arg1)
//		{
//			result = OpernameGetOprid(opname, basetype, basetype);
//			if (OidIsValid(result))
//				return result;
//		}
//	}
//
//	return InvalidOid;
//}


///* oper_select_candidate()
// *		Given the input argtype array and one or more candidates
// *		for the operator, attempt to resolve the conflict.
// *
// * Returns FUNCDETAIL_NOTFOUND, FUNCDETAIL_MULTIPLE, or FUNCDETAIL_NORMAL.
// * In the success case the Oid of the best candidate is stored in *operOid.
// *
// * Note that the caller has already determined that there is no candidate
// * exactly matching the input argtype(s).  Incompatible candidates are not yet
// * pruned away, however.
// */
//static FuncDetailCode
//oper_select_candidate(int nargs,
//					  Oid *input_typeids,
//					  FuncCandidateList candidates,
//					  Oid *operOid) /* output argument */
//{
//	int			ncandidates;
//
//	/*
//	 * Delete any candidates that cannot actually accept the given input
//	 * types, whether directly or by coercion.
//	 */
//	ncandidates = func_match_argtypes(nargs, input_typeids,
//									  candidates, &candidates);
//
//	/* Done if no candidate or only one candidate survives */
//	if (ncandidates == 0)
//	{
//		*operOid = InvalidOid;
//		return FUNCDETAIL_NOTFOUND;
//	}
//	if (ncandidates == 1)
//	{
//		*operOid = candidates->oid;
//		return FUNCDETAIL_NORMAL;
//	}
//
//	/*
//	 * Use the same heuristics as for ambiguous functions to resolve the
//	 * conflict.
//	 */
//	candidates = func_select_candidate(nargs, input_typeids, candidates);
//
//	if (candidates)
//	{
//		*operOid = candidates->oid;
//		return FUNCDETAIL_NORMAL;
//	}
//
//	*operOid = InvalidOid;
//	return FUNCDETAIL_MULTIPLE; /* failed to select a best candidate */
//}


/*
 * op_signature_string
 *		Build a string representing an operator name, including arg type(s).
 *		The result is something like "integer + integer".
 *
 * This is typically used in the construction of operator-not-found error
 * messages.
 */
static const char *
op_signature_string(List *op, char oprkind, Oid arg1, Oid arg2)
{
	StringInfoData argbuf;

	initStringInfo(&argbuf);

	if (oprkind != 'l')
		appendStringInfo(&argbuf, "%s ", format_type_be(arg1));

	appendStringInfoString(&argbuf, NameListToString(op));

	appendStringInfo(&argbuf, " %s", format_type_be(arg2));

	return argbuf.data;			/* return palloc'd string buffer */
}

///*
// * op_error - utility routine to complain about an unresolvable operator
// */
//static void
//op_error(ParseState *pstate, List *op, char oprkind,
//		 Oid arg1, Oid arg2,
//		 FuncDetailCode fdresult, int location)
//{
//	if (fdresult == FUNCDETAIL_MULTIPLE)
//		ereport(ERROR,
//				(errcode(ERRCODE_AMBIGUOUS_FUNCTION),
//				 errmsg("operator is not unique: %s",
//						op_signature_string(op, oprkind, arg1, arg2)),
//				 errhint("Could not choose a best candidate operator. "
//						 "You might need to add explicit type casts."),
//				 parser_errposition(pstate, location)));
//	else
//		ereport(ERROR,
//				(errcode(ERRCODE_UNDEFINED_FUNCTION),
//				 errmsg("operator does not exist: %s",
//						op_signature_string(op, oprkind, arg1, arg2)),
//				 (!arg1 || !arg2) ?
//				 errhint("No operator matches the given name and argument type. "
//						 "You might need to add an explicit type cast.") :
//				 errhint("No operator matches the given name and argument types. "
//						 "You might need to add explicit type casts."),
//				 parser_errposition(pstate, location)));
//}

/*
 * make_op()
 *		Operator expression construction.
 *
 * Transform operator expression ensuring type compatibility.
 * This is where some type conversion happens.
 *
 * last_srf should be a copy of pstate->p_last_srf from just before we
 * started transforming the operator's arguments; this is used for nested-SRF
 * detection.  If the caller will throw an error anyway for a set-returning
 * expression, it's okay to cheat and just pass pstate->p_last_srf.
 */
Expr *
mys_make_op(ParseState *pstate, List *opname, Node *ltree, Node *rtree,
		Node *last_srf, int location)
{
	Oid			ltypeId,
				rtypeId;
	Operator	tup;
	Form_pg_operator opform;
	Oid			actual_arg_types[2];
	Oid			declared_arg_types[2];
	int			nargs;
	List	   *args;
	Oid			rettype;
	OpExpr	   *result;

	/* Check it's not a postfix operator */
	if (rtree == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("postfix operators are not supported")));

	/* Select the operator */
	if (ltree == NULL)
	{
		/* prefix operator */
		rtypeId = exprType(rtree);
		ltypeId = InvalidOid;
		tup = left_oper(pstate, opname, rtypeId, false, location);
	}
	else
	{
		/* otherwise, binary operator */
        Oid ltypeIdBaseId;
        Oid rtypeIdBaseId;
		ltypeId = exprType(ltree);
		rtypeId = exprType(rtree);
        ltypeIdBaseId = getBaseType(ltypeId);
        rtypeIdBaseId = getBaseType(rtypeId);
        if (((ltypeIdBaseId == TIMESTAMPOID) || (ltypeIdBaseId == TIMESTAMPTZOID) || (ltypeIdBaseId == DATEOID)) && 
            (rtypeId == UNKNOWNOID))
        {
			if (IsA(rtree, Const))
			{
				tup = oper(pstate, opname, ltypeId, TEXTOID, false, location);
			}
            else
            {
                tup = oper(pstate, opname, ltypeId, rtypeId, false, location);
            }
        }
        else if (((rtypeIdBaseId == TIMESTAMPOID) || (rtypeIdBaseId == TIMESTAMPTZOID) || (rtypeIdBaseId == DATEOID)) && 
                 (ltypeId == UNKNOWNOID))
        {
            if ((IsA(ltree, Const)) && (!((const Const *) ltree)->constisnull))
            //if (!((const Const *) ltree)->constisnull)
            {
                Datum datum = ((const Const *) ltree)->constvalue;
                char *val = DatumGetPointer(datum);
                int valLen = strlen(val);
                int i;
                for (i = (valLen - 1); 0 <= i; i--)
                {
                    if (val[i] != ' ')
                    {
                        break;
                    }
                }
                valLen = i + 1;
                if ((valLen == 0) || 
                    ((3 <= valLen) && (val[valLen - 3] == '-') && (val[valLen - 2] == '0') && (val[valLen - 1] == '0')))
                {
                    tup = oper(pstate, opname, TEXTOID, rtypeId, false, location);
                }
                else
                {
                    tup = oper(pstate, opname, ltypeId, rtypeId, false, location);
                }
            }
            else
            {
                tup = oper(pstate, opname, ltypeId, rtypeId, false, location);
            }
        }
		else if ((ltypeIdBaseId == INT4OID) && (rtypeId == UNKNOWNOID))
		{
            if ((IsA(rtree, Const)) && (!((const Const *) rtree)->constisnull))
			//if (!((const Const *) rtree)->constisnull)
			{
				tup = oper(pstate, opname, ltypeId, TEXTOID, false, location);
			}
			else
			{
				tup = oper(pstate, opname, ltypeId, rtypeId, false, location);
			}
		}
        else 
        {
            tup = oper(pstate, opname, ltypeId, rtypeId, false, location);
        }
	}

	opform = (Form_pg_operator) GETSTRUCT(tup);

	/* Check it's not a shell */
	if (!RegProcedureIsValid(opform->oprcode))
		ereport(ERROR,
				(errcode(ERRCODE_UNDEFINED_FUNCTION),
				 errmsg("operator is only a shell: %s",
						op_signature_string(opname,
											opform->oprkind,
											opform->oprleft,
											opform->oprright)),
				 parser_errposition(pstate, location)));

	/* Do typecasting and build the expression tree */
	if (ltree == NULL)
	{
		/* prefix operator */
		args = list_make1(rtree);
		actual_arg_types[0] = rtypeId;
		declared_arg_types[0] = opform->oprright;
		nargs = 1;
	}
	else
	{
		/* otherwise, binary operator */
		args = list_make2(ltree, rtree);
		actual_arg_types[0] = ltypeId;
		actual_arg_types[1] = rtypeId;
		declared_arg_types[0] = opform->oprleft;
		declared_arg_types[1] = opform->oprright;
		nargs = 2;
	}

	/*
	 * enforce consistency with polymorphic argument and return types,
	 * possibly adjusting return type or declared_arg_types (which will be
	 * used as the cast destination by make_fn_arguments)
	 */
	rettype = enforce_generic_type_consistency(actual_arg_types,
											   declared_arg_types,
											   nargs,
											   opform->oprresult,
											   false);

	/* perform the necessary typecasting of arguments */
	make_fn_arguments(pstate, args, actual_arg_types, declared_arg_types);

	/* and build the expression node */
	result = makeNode(OpExpr);
	result->opno = oprid(tup);
	result->opfuncid = opform->oprcode;
	result->opresulttype = rettype;
	result->opretset = get_func_retset(opform->oprcode);
	/* opcollid and inputcollid will be set by parse_collate.c */
	result->args = args;
	result->location = location;

	/* if it returns a set, check that's OK */
	if (result->opretset)
	{
		check_srf_call_placement(pstate, last_srf, location);
		/* ... and remember it for error checks at higher levels */
		pstate->p_last_srf = (Node *) result;
	}

	ReleaseSysCache(tup);

	return (Expr *) result;
}


/*
 * Lookaside cache to speed operator lookup.  Possibly this should be in
 * a separate module under utils/cache/ ?
 *
 * The idea here is that the mapping from operator name and given argument
 * types is constant for a given search path (or single specified schema OID)
 * so long as the contents of pg_operator and pg_cast don't change.  And that
 * mapping is pretty expensive to compute, especially for ambiguous operators;
 * this is mainly because there are a *lot* of instances of popular operator
 * names such as "=", and we have to check each one to see which is the
 * best match.  So once we have identified the correct mapping, we save it
 * in a cache that need only be flushed on pg_operator or pg_cast change.
 * (pg_cast must be considered because changes in the set of implicit casts
 * affect the set of applicable operators for any given input datatype.)
 *
 * XXX in principle, ALTER TABLE ... INHERIT could affect the mapping as
 * well, but we disregard that since there's no convenient way to find out
 * about it, and it seems a pretty far-fetched corner-case anyway.
 *
 * Note: at some point it might be worth doing a similar cache for function
 * lookups.  However, the potential gain is a lot less since (a) function
 * names are generally not overloaded as heavily as operator names, and
 * (b) we'd have to flush on pg_proc updates, which are probably a good
 * deal more common than pg_operator updates.
 */

///* The operator cache hashtable */
//static HTAB *OprCacheHash = NULL;


///*
// * make_oper_cache_key
// *		Fill the lookup key struct given operator name and arg types.
// *
// * Returns true if successful, false if the search_path overflowed
// * (hence no caching is possible).
// *
// * pstate/location are used only to report the error position; pass NULL/-1
// * if not available.
// */
//static bool
//make_oper_cache_key(ParseState *pstate, OprCacheKey *key, List *opname,
//					Oid ltypeId, Oid rtypeId, int location)
//{
//	char	   *schemaname;
//	char	   *opername;
//
//	/* deconstruct the name list */
//	DeconstructQualifiedName(opname, &schemaname, &opername);
//
//	/* ensure zero-fill for stable hashing */
//	MemSet(key, 0, sizeof(OprCacheKey));
//
//	/* save operator name and input types into key */
//	strlcpy(key->oprname, opername, NAMEDATALEN);
//	key->left_arg = ltypeId;
//	key->right_arg = rtypeId;
//
//	if (schemaname)
//	{
//		ParseCallbackState pcbstate;
//
//		/* search only in exact schema given */
//		setup_parser_errposition_callback(&pcbstate, pstate, location);
//		key->search_path[0] = LookupExplicitNamespace(schemaname, false);
//		cancel_parser_errposition_callback(&pcbstate);
//	}
//	else
//	{
//		/* get the active search path */
//		if (fetch_search_path_array(key->search_path,
//									MAX_CACHED_PATH_LEN) > MAX_CACHED_PATH_LEN)
//			return false;		/* oops, didn't fit */
//	}
//
//	return true;
//}

///*
// * find_oper_cache_entry
// *
// * Look for a cache entry matching the given key.  If found, return the
// * contained operator OID, else return InvalidOid.
// */
//static Oid
//find_oper_cache_entry(OprCacheKey *key)
//{
//	OprCacheEntry *oprentry;
//
//	if (OprCacheHash == NULL)
//	{
//		/* First time through: initialize the hash table */
//		HASHCTL		ctl;
//
//		ctl.keysize = sizeof(OprCacheKey);
//		ctl.entrysize = sizeof(OprCacheEntry);
//		OprCacheHash = hash_create("Operator lookup cache", 256,
//								   &ctl, HASH_ELEM | HASH_BLOBS);
//
//		/* Arrange to flush cache on pg_operator and pg_cast changes */
//		CacheRegisterSyscacheCallback(OPERNAMENSP,
//									  InvalidateOprCacheCallBack,
//									  (Datum) 0);
//		CacheRegisterSyscacheCallback(CASTSOURCETARGET,
//									  InvalidateOprCacheCallBack,
//									  (Datum) 0);
//	}
//
//	/* Look for an existing entry */
//	oprentry = (OprCacheEntry *) hash_search(OprCacheHash,
//											 (void *) key,
//											 HASH_FIND, NULL);
//	if (oprentry == NULL)
//		return InvalidOid;
//
//	return oprentry->opr_oid;
//}

///*
// * make_oper_cache_entry
// *
// * Insert a cache entry for the given key.
// */
//static void
//make_oper_cache_entry(OprCacheKey *key, Oid opr_oid)
//{
//	OprCacheEntry *oprentry;
//
//	Assert(OprCacheHash != NULL);
//
//	oprentry = (OprCacheEntry *) hash_search(OprCacheHash,
//											 (void *) key,
//											 HASH_ENTER, NULL);
//	oprentry->opr_oid = opr_oid;
//}

///*
// * Callback for pg_operator and pg_cast inval events
// */
//static void
//InvalidateOprCacheCallBack(Datum arg, int cacheid, uint32 hashvalue)
//{
//	HASH_SEQ_STATUS status;
//	OprCacheEntry *hentry;
//
//	Assert(OprCacheHash != NULL);
//
//	/* Currently we just flush all entries; hard to be smarter ... */
//	hash_seq_init(&status, OprCacheHash);
//
//	while ((hentry = (OprCacheEntry *) hash_seq_search(&status)) != NULL)
//	{
//		if (hash_search(OprCacheHash,
//						(void *) &hentry->key,
//						HASH_REMOVE, NULL) == NULL)
//			elog(ERROR, "hash table corrupted");
//	}
//}

