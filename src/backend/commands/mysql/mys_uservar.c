#include "postgres.h"

#include "catalog/pg_type.h"
#include "commands/mysql/mys_uservar.h"
#include "utils/builtins.h"
#include "utils/hsearch.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"

#define VAR_VALUE_EXTEND_BYTES 1

typedef struct MysqlUserVariableItem
{
    /* dynahash.c requires key to be first field */
    char varName[NAMEDATALEN];
    bytea *varValue;
    bool isnull;
} MysqlUserVariableItem;

static HTAB *mysql_user_variables = NULL;

static void initMysqlUserVariableHashTable(void);
static Datum byteastring2bytea(char *byteastring, bool isDigit);
static Datum bitstring2bytea(char *bitstring, bool isDigit);
static Datum boolstring2bytea(char *boolstring, bool isDigit);
static Datum cstring2bytea(char *cstring, bool isDigit);
static bytea *copyUserVarValue(bytea *varValue);


void 
clearUserVars(void)
{
    hash_destroy(mysql_user_variables);
    mysql_user_variables = NULL;
}


bool
varValueIsDigit(Datum varValue, Oid varValueType)
{
    bool isDigit = false;

    if (varValueType == BYTEAOID)
    {
        /*
         * 确保传入的varValue，内存使用palloc分配
         */
        bytea *vlena = DatumGetByteaPP(varValue);
        size_t userVarValueExtendByteOffset = VARSIZE_ANY(vlena);

        isDigit = (((char *)vlena)[userVarValueExtendByteOffset] == '1') ? true : false;
    }
    else if (varValueType == BOOLOID)
    {
        isDigit = true;
    }
    else if (varValueType == BITOID || varValueType == VARBITOID)
    {
        isDigit = true;
    }
    else
    {
        char ncategory;
		bool nispreferred;

        get_type_category_preferred(varValueType, &ncategory, &nispreferred);

        isDigit = (ncategory == TYPCATEGORY_NUMERIC) ? true : false;
    }

    return isDigit;
}


void
mysSetUserVarInternal(char *userVarName, char *userVarValue, Oid varValueType, bool isDigit, bool isNull)
{
    MysqlUserVariableItem *entry;
    bool found;
    MemoryContext oldContext;

    /* Initialize the hash table, if necessary */
	if (!mysql_user_variables)
		initMysqlUserVariableHashTable();

    entry = (MysqlUserVariableItem *) hash_search(mysql_user_variables,
                                                  userVarName,
                                                  HASH_ENTER,
                                                  &found);

    
    if (found)
    {
        if (!entry->isnull)
        {
            pfree(entry->varValue);
            entry->varValue = NULL;
        }
    }
    else
    {
        /* Nothing to do */
    }

    oldContext = MemoryContextSwitchTo(TopMemoryContext);

    if (!isNull)
    {
        Oid baseVarValueType = getBaseType(varValueType);
        bytea *result = NULL;

        if (baseVarValueType == BYTEAOID)
        {
            result = DatumGetByteaPP(byteastring2bytea(userVarValue, isDigit));
        }
        else if (baseVarValueType == BITOID || baseVarValueType == VARBITOID)
        {
            result = DatumGetByteaPP(bitstring2bytea(userVarValue, isDigit));
        }
        else if (baseVarValueType == BOOLOID)
        {
            result = DatumGetByteaPP(boolstring2bytea(userVarValue, isDigit));
        }
        else
        {
            result = DatumGetByteaPP(cstring2bytea(userVarValue, isDigit));
        }

        entry->varValue = result;
        entry->isnull = false;
    }
    else
    {
        entry->isnull = true;
    }

    MemoryContextSwitchTo(oldContext);
}


void
mysSetUserVarForPl(char *userVarName, Datum userVarValue, Oid varValueType, bool isNull)
{
    MysqlUserVariableItem *entry;
    bool found;
    MemoryContext oldContext;

    /* Initialize the hash table, if necessary */
	if (!mysql_user_variables)
		initMysqlUserVariableHashTable();

    entry = (MysqlUserVariableItem *) hash_search(mysql_user_variables,
                                                  userVarName,
                                                  HASH_ENTER,
                                                  &found);

    
    if (found)
    {
        if (!entry->isnull)
        {
            pfree(entry->varValue);
            entry->varValue = NULL;
        }
    }
    else
    {
        /* Nothing to do */
    }

    oldContext = MemoryContextSwitchTo(TopMemoryContext);

    if (!isNull)
    {
        Oid baseVarValueType = getBaseType(varValueType);
        Oid typOutput = InvalidOid;
        bool typIsVarlena;
        bool isDigit;
        char *varValue;
        bytea *result = NULL;

        getTypeOutputInfo(varValueType, &typOutput, &typIsVarlena);
        varValue = OidOutputFunctionCall(typOutput, userVarValue);
        isDigit = varValueIsDigit(userVarValue, varValueType);

        if (baseVarValueType == BYTEAOID)
        {
            result = DatumGetByteaPP(byteastring2bytea(varValue, isDigit));
        }
        else if (baseVarValueType == BITOID || baseVarValueType == VARBITOID)
        {
            result = DatumGetByteaPP(bitstring2bytea(varValue, isDigit));
        }
        else if (baseVarValueType == BOOLOID)
        {
            result = DatumGetByteaPP(boolstring2bytea(varValue, isDigit));
        }
        else
        {
            result = DatumGetByteaPP(cstring2bytea(varValue, isDigit));
        }

        entry->varValue = result;
        entry->isnull = false;
    }
    else
    {
        entry->isnull = true;
    }

    MemoryContextSwitchTo(oldContext);
}


bytea *
mysGetUserVarValueInternal(char *userVarName)
{
    MysqlUserVariableItem *entry;
    bool found;
    bytea *result;

    /* Initialize the hash table, if necessary */
	if (!mysql_user_variables)
		initMysqlUserVariableHashTable();
    
    entry = (MysqlUserVariableItem *) hash_search(mysql_user_variables,
                                                  userVarName,
                                                  HASH_FIND,
                                                  &found);
    
    result = NULL;

    if (found)
    {
        if (entry->isnull == false)
        {
            result = copyUserVarValue(entry->varValue);
        }
    }

    return result;
}


static void
initMysqlUserVariableHashTable(void)
{
	HASHCTL		hash_ctl;

	hash_ctl.keysize = NAMEDATALEN;
	hash_ctl.entrysize = sizeof(MysqlUserVariableItem);
    
	mysql_user_variables = hash_create("MySQL User Variables",
                                       1024,
                                       &hash_ctl,
                                       HASH_ELEM | HASH_STRINGS);
}


static Datum
byteastring2bytea(char *byteastring, bool isDigit)
{
    bytea *result;

    if ((byteastring[0] == '\\') && (byteastring[1] == 'x'))
    {
        /* 
         * bytea_output == BYTEA_OUTPUT_HEX
         * "\x3135"
         */
        size_t len = strlen(byteastring);
        int bc;

		bc = (len - 2) / 2 + VARHDRSZ + VAR_VALUE_EXTEND_BYTES;	/* maximum possible length */
		result = palloc(bc);
		bc = hex_decode(byteastring + 2, len - 2, VARDATA(result));
		SET_VARSIZE(result, bc + VARHDRSZ); /* actual length */
        
        if (isDigit)
        {
            ((char *)result)[bc + VARHDRSZ] = '1';
        }
        else
        {
            ((char *)result)[bc + VARHDRSZ] = '\0';
        }
    }
    else
    {
        elog(ERROR, "Invalid string format for bytea_output(BYTEA_OUTPUT_ESCAPE)");
    }

    PG_RETURN_BYTEA_P(result);
}


static Datum
bitstring2bytea(char *bitstring, bool isDigit)
{
    int bitLen = strlen(bitstring);
    int byteLen = (bitLen + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
    int needPrePadBitNum = byteLen * BITS_PER_BYTE - bitLen;
    char *rp;
    int i;
    int j;
    bytea *result;

    result = (bytea *) palloc0(byteLen + VARHDRSZ + VAR_VALUE_EXTEND_BYTES);
    SET_VARSIZE(result, byteLen + VARHDRSZ);

    rp = VARDATA(result);

    for (i = 0; i < (BITS_PER_BYTE - needPrePadBitNum); i++)
    {
        rp[0] = rp[0] * 2 + (bitstring[i] - '0');
    }

    for (j = 1; j < byteLen; j++)
    {
        int end = i + BITS_PER_BYTE;
        for (; i < end; i++)
        {
            rp[j] = rp[j] * 2 + (bitstring[i] - '0');
        }
    }

    if (isDigit)
    {
        ((char *)result)[byteLen + VARHDRSZ] = '1';
    }
    else
    {
        ((char *)result)[byteLen + VARHDRSZ] = '\0';
    }

    PG_RETURN_BYTEA_P(result);
}


static Datum
boolstring2bytea(char *boolstring, bool isDigit)
{
    char *p;
    bytea *result;

    result = (bytea *) palloc(1 + VARHDRSZ + VAR_VALUE_EXTEND_BYTES);
    SET_VARSIZE(result, 1 + VARHDRSZ);

    p = VARDATA(result);
    *p = (boolstring[0] == '1') ? '1' : '0';

    if (isDigit)
    {
        ((char *)result)[1 + VARHDRSZ] = '1';
    }
    else
    {
        ((char *)result)[1 + VARHDRSZ] = '\0';
    }

    PG_RETURN_BYTEA_P(result);
}


static Datum
cstring2bytea(char *cstring, bool isDigit)
{
    int cstringLen = strlen(cstring);
    char *tp;
    char *rp;
    bytea *result;

    result = (bytea *) palloc(cstringLen + VARHDRSZ + VAR_VALUE_EXTEND_BYTES);
    SET_VARSIZE(result, cstringLen + VARHDRSZ);

    tp = cstring;
    rp = VARDATA(result);

    while (*tp != '\0')
    {
        *rp++ = *tp++;
    }

    if (isDigit)
    {
        ((char *)result)[cstringLen + VARHDRSZ] = '1';
    }
    else
    {
        ((char *)result)[cstringLen + VARHDRSZ] = '\0';
    }

    PG_RETURN_BYTEA_P(result);
}


static bytea *
copyUserVarValue(bytea *varValue)
{
    bytea *vlena = varValue;
    char *byte = VARDATA_ANY(vlena);
    size_t byteLen = VARSIZE_ANY_EXHDR(vlena);
    bool isDigit = varValueIsDigit(PointerGetDatum(varValue), BYTEAOID);
    char *p;
    bytea *result;

    result = palloc(byteLen + VARHDRSZ + VAR_VALUE_EXTEND_BYTES);
    SET_VARSIZE(result, byteLen + VARHDRSZ);

    p = VARDATA(result);
    memcpy(p, byte, byteLen);

    if (isDigit)
    {
        ((char *)result)[byteLen + VARHDRSZ] = '1';
    }
    else
    {
        ((char *)result)[byteLen + VARHDRSZ] = '\0';
    }

    return result;
}
