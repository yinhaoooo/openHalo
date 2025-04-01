#include <limits.h>
#include "postgres.h"

#include "datatype/timestamp.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "pgtime.h"
#include "utils/date.h"
#include "utils/datetime.h"
#include "utils/mysql/mys_date.h"
#include "utils/mysql/mys_timestamp.h"


// /* Allow zero day and zero month */
// const uint32 TIME_FUZZY_DATE = 1;

// /* Only allow full datetimes. */
// const uint32 TIME_DATETIME_ONLY = 2;

// const uint32 TIME_FRAC_TRUNCATE = 4;
// const uint32 TIME_NO_DATE_FRAC_WARN = 8;

// /* Don't allow zero day or zero month */
// const uint32 TIME_NO_ZERO_IN_DATE = 16;

// /* Don't allow 0000-00-00 date */
// const uint32 TIME_NO_ZERO_DATE = 32;

// /* Allow 2000-02-31 */
// const uint32 TIME_INVALID_DATES = 64;

// /* Allow only HH:MM:SS or MM:SS time formats */
// const uint32 TIME_STRICT_COLON = 128;

static const int64 log_10_int[MAX_TIMESTAMP_PRECISION + 1] = {
		INT64CONST(1),
		INT64CONST(10),
		INT64CONST(100),
		INT64CONST(1000),
		INT64CONST(10000),
		INT64CONST(100000),
		INT64CONST(1000000)
	};


/*
 * mys_pre_timestamp_in使用的context
 */
// typedef struct mysPreTimestampInContext
// {
//     // 是否可以解析为timestamp
//     bool isTimestamp;
//     uint32 flags;
// } mysPreTimestampInContext;

static bool mys_strToDateInternal(char *str, int flags, DateADT *result);
static bool mysCheckTimestampRange(struct pg_tm *tm, fsec_t fsec);
static bool tzDisplacement2Seconds(const char *str, int length, int *result);
static char *mysPreTimeInInternal(char *str);
static char *mysProcessStringTimeForFormat1(char *str);
static char *mysProcessStringTimeForFormat2(char *str);
static char *mysProcessStringTimeForFormat3(char *str);
static char *mysPreTimestampInForTime(char *str, bool *isTimestamp);


Datum
mys_date_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    int timeFlags = MYS_TIME_NO_ZERO_IN_DATE | MYS_TIME_NO_ZERO_DATE;
    DateADT result;

    if (mys_strToDateInternal(str, timeFlags, &result) == false)
    {
        elog(ERROR, "Incorrect datetime value: '%s'", str);
    }

	PG_RETURN_DATEADT(result);
}


static bool
mys_strToDateInternal(char *str, int flags, DateADT *result)
{
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    fsec_t fsec = 0;
    int tz = 0;
    bool haveTz = false;

    if (mys_decodeStringDatetime(str, tm, &fsec, &tz, flags, &haveTz))
    {
        if (fsec == USECS_PER_SEC)
        {
            if (mysCheckDate(tm, true, (MYS_TIME_NO_ZERO_IN_DATE | MYS_TIME_NO_ZERO_DATE)) == false)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
        // elog(ERROR, "Incorrect datetime value: '%s'", str);
    }

    if (haveTz)
    {
        /* 
         * cast('2022-10-10 23:00:00+06:00' as date)，需要转换到当前会话时区
         *      假设当前时区为+08:00
         *      out: 2022-10-11
         */
        Timestamp ts;

        if (tm2timestamp(tm, fsec, &tz, &ts) != 0)
        {
            return false;
        }

        if (timestamp2tm(ts, &tz, tm, &fsec, NULL, session_timezone) != 0)
			ereport(ERROR,
					(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
					 errmsg("timestamp out of range")));
    }

    /* Prevent overflow in Julian-day routines */
	if (!IS_VALID_JULIAN(tm->tm_year, tm->tm_mon, tm->tm_mday))
		ereport(ERROR,
				(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
				 errmsg("date out of range: \"%s\"", str)));

	*result = date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - POSTGRES_EPOCH_JDATE;

	/* Now check for just-out-of-range dates */
	if (!IS_VALID_DATE(*result))
		ereport(ERROR,
				(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
				 errmsg("date out of range: \"%s\"", str)));

    return true;
}


TimeADT
mys_pre_time_in(PG_FUNCTION_ARGS)
{
    char	   *str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid			typelem = PG_GETARG_OID(1);
#endif
	int32		typmod = PG_GETARG_INT32(2);
	TimeADT		result;
	fsec_t		fsec;
	struct pg_tm tt,
			   *tm = &tt;
	int			tz;
	int			nf;
	int			dterr;
	char		workbuf[MAXDATELEN + 1];
	char	   *field[MAXDATEFIELDS];
	int			dtype;
	int			ftype[MAXDATEFIELDS];
    int sign = 1;

    str = mysPreTimeInInternal(str);
    if (str == NULL)
    {
        elog(ERROR, "Incorrect time value.");
    }
    if (str[0] == '-')
    {
        sign = -1;
        str = str + 1;
    }

    tm->tm_year = 0;
	dterr = ParseDateTime(str, workbuf, sizeof(workbuf),
						  field, ftype, MAXDATEFIELDS, &nf);
	if (dterr == 0)
		dterr = DecodeTimeOnly(field, ftype, nf, &dtype, tm, &fsec, &tz);
	if (dterr != 0)
		DateTimeParseError(dterr, str, "time");

    if (tm->tm_year > 0)
    {
        sign = 1;
    }

	tm2time(tm, fsec, &result);
	AdjustTimeForTypmod(&result, typmod);

    result = (int64)result * sign;

    return result;
}


char *
mys_post_time_out(PG_FUNCTION_ARGS)
{
    TimeADT		time = PG_GETARG_TIMEADT(0);
	char	   *result;
	struct pg_tm tt,
			   *tm = &tt;
	fsec_t		fsec;
	char		buf[MAXDATELEN + 1];
    bool nega = false;

    if (time < 0)
    {
        nega = true;
        time = time * -1;
    }
	time2tm(time, tm, &fsec);
	EncodeTimeOnly(tm, fsec, false, 0, DateStyle, buf);

	result = palloc0(MAXDATELEN + 1);
    if (nega)
    {
        pg_snprintf(result, MAXDATELEN, "-%s", buf);
    }
    else
    {
        pg_snprintf(result, MAXDATELEN, "%s", buf);
    }

    return result;
}


void
mys_pre_time_in_for_subtime(char *str, struct pg_tm *tm, fsec_t *fsec, int *sign)
{
	int			tz;
	int			nf;
	int			dterr;
	char		workbuf[MAXDATELEN + 1];
	char	   *field[MAXDATEFIELDS];
	int			dtype;
	int			ftype[MAXDATEFIELDS];
    
    *sign = 1;

    str = mysPreTimeInInternal(str);
    if (str == NULL)
    {
        elog(ERROR, "Incorrect time value.");
    }
    if (str[0] == '-')
    {
        *sign = -1;
        str = str + 1;
    }

	dterr = ParseDateTime(str, workbuf, sizeof(workbuf),
						  field, ftype, MAXDATEFIELDS, &nf);
	if (dterr == 0)
		dterr = DecodeTimeOnly(field, ftype, nf, &dtype, tm, fsec, &tz);
	if (dterr != 0)
		DateTimeParseError(dterr, str, "time");
    
    if (tm->tm_year > 0)
    {
        *sign = 1;
    }
}


static char *
mysPreTimeInInternal(char *str)
{
    char *result;
    char *tempResult;
    bool nega = false;
    int startIndex;
    int strIndex;
    int strLen = strlen(str);
    uint64 value;
    int numOfSpaceAfterDay;

    for (strIndex = 0; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
            break;
    }

    if (strIndex < strLen && str[strIndex] == '-')
    {
        nega = true;
        strIndex++;
    }

    if (strIndex == strLen)
    {
        return NULL;
    }

    // Remember beginning of first non-space/- char.
    startIndex = strIndex;
    result = palloc0(64);

    if ((strLen - strIndex) >= 12)
    {
        bool isTimestamp;
        tempResult = mysPreTimestampInForTime(str + strIndex, &isTimestamp);
        if (isTimestamp)
        {
            // 是timestmap格式
            if (tempResult != NULL)
            {
                if (nega)
                {
                    pg_snprintf(result, 64, "-%s", tempResult);
                }
                else
                {
                    pg_snprintf(result, 64, "%s", tempResult);
                }
                return result;
            }
            else 
            {
                return NULL;
            }
        }
    }

    /* Not a timestamp. Try to get this as a DAYS_TO_SECOND string */
    for (value = 0; strIndex < strLen && isdigit(str[strIndex]); strIndex++)
    {
        value = value * 10L + (str[strIndex] - '0');
        
        if (value > UINT_MAX)
        {
            return NULL;
        }
    }

    for (numOfSpaceAfterDay = 0; strIndex < strLen; strIndex++)
    {
        if (isspace(str[strIndex]))
        {
            numOfSpaceAfterDay++;
        }
        else
        {
            break;
        }
    }

    if (numOfSpaceAfterDay > 0)
    {
        if (strIndex < strLen)
        {
            if (isdigit(str[strIndex]))
            {
                // 'D hh:mm:ss[.fsec]' | 'D hh:mm' | 'D hh'
                tempResult = mysProcessStringTimeForFormat1(str + startIndex);
            }
            else if (str[strIndex] == ':')
            {
                // 'hh :mm:[ss[.fsec]]' | 'mm :ss[.fsec]'
                tempResult = mysProcessStringTimeForFormat2(str + startIndex);
            }
            else if (str[strIndex] == '.')
            {
                // 'value .[fsec]', value当作HHMMSS format
                tempResult = mysProcessStringTimeForFormat3(str + startIndex);
            }
            else
            {
                // value当作HHMMSS format
                tempResult = mysProcessStringTimeForFormat3(str + startIndex);
            }
        }
        else
        {
            // value当作HHMMSS format
            tempResult = mysProcessStringTimeForFormat3(str + startIndex);
        }
    }
    else
    {
        if (strIndex < strLen)
        {
            if (str[strIndex] == ':')
            {
                // 'hh:mm:[ss[.fsec]]' | 'mm:ss[.fsec]'
                tempResult = mysProcessStringTimeForFormat2(str + startIndex);
            }
            else if (str[strIndex] == '.')
            {
                // 'value.[fsec]', value当作HHMMSS format
                tempResult = mysProcessStringTimeForFormat3(str + startIndex);
            }
            else
            {
                // value当作HHMMSS format
                tempResult = mysProcessStringTimeForFormat3(str + startIndex);
            }
        }
        else
        {
            // value当作HHMMSS format
            tempResult = mysProcessStringTimeForFormat3(str + startIndex);
        }
    }

    if (tempResult == NULL)
    {
        result = NULL;
    }
    else
    {
        if (nega)
        {
            pg_snprintf(result, 64, "-%s", tempResult);
        }
        else
        {
            pg_snprintf(result, 64, "%s", tempResult);
        }
    }

    return result;
}


/*
 * As a string in 'D hh:mm:ss[.fsec]' format. You can also use one of the following “relaxed” syntaxes:
 * 'D hh:mm' or 'D hh'. Here D represents days and can have a value from 0 to 34.
 */
static char *
mysProcessStringTimeForFormat1(char *str)
{
    char *result = NULL;
    int strIndex = 0;
    int strLen = strlen(str);
    // day, hour, min, second, fsec
    uint64 data[5] = {0};
    int dataIndex = 0;
    int nanoSecond = 0;

    if (strLen == 0 || str[0] == ' ')
    {
        return NULL;
    }

    for (; dataIndex < 5 - 1; dataIndex++)
    {
        uint64 tempValue = 0;
        for (; strIndex < strLen; strIndex++)
        {
            char cha = str[strIndex];
            if (isdigit(cha))
            {
                tempValue = tempValue * 10 + (cha - '0');
            }
            else
            {
                break;
            }
        }
        data[dataIndex] = tempValue;

        if (strIndex >= strLen)
        {
            break;
        }

        if (dataIndex == 0)
        {
            if (!isspace(str[strIndex]))
            {
                return NULL;
            }
            for (; strIndex < strLen; strIndex++)
            {
                char cha = str[strIndex];
                if (isspace(cha))
                {
                    continue;
                }
                else
                {
                    // 肯定是数字
                    break;
                }
            }
        }
        else if (dataIndex == 3)    // second
        {
            break;
        }
        else    // hour, min
        {
            if (str[strIndex] != ':')
            {
                // '1 10 :10' | '1 10+:10'
                break;
            }
            else
            {
                if (strIndex + 1 < strLen)
                {
                    if (!isdigit(str[strIndex + 1]))
                    {
                        // '1 10:a' | '1 10: 10'
                        break;
                    }
                    else
                    {
                        // 跳过':'('1 10:10')
                        strIndex++;
                    }
                }
                else
                {
                    // '1 10:'
                    break;
                }
            }
        }
    }

    if (strIndex < strLen && str[strIndex] == '.')
    {
        //microsecond
        int fsec = 0;
        int fsecLen = 0;

        strIndex++;     // 跳过'.'
        // microsecond
        for (; strIndex < strLen; strIndex++)
        {
            char cha = str[strIndex];

            if (fsecLen >= MAX_TIMESTAMP_PRECISION)
            {
                break;
            }

            if (isdigit(cha))
            {
                fsec = fsec * 10 + (cha - '0');
                fsecLen++;
            }
            else
            {
                break;
            }
        }

        // nanoSecond
        if (strIndex < strLen && isdigit(str[strIndex]))
        {
            nanoSecond = str[strIndex] - '0';
        }

        strIndex++;
        for (; strIndex < strLen; strIndex++)
        {
            if (!isdigit(str[strIndex]))
            {
                break;
            }
        }

        data[4] = fsec * log_10_int[MAX_TIMESTAMP_PRECISION - fsecLen];
    }

    for (; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
        {
            return NULL;
        }
    }

    if ((data[0] * 24 + data[1]) > MYS_MAX_TIME_HOUR || data[2] > 59 || data[3] > 59 || data[4] > 999999)
    {
        return NULL;
    }
    else
    {
        result = palloc0(64);
        pg_snprintf(result, 64, "%u:%02u:%02u.%06u%u",
                            (uint)(data[0] * 24 + data[1]), (uint)data[2], (uint)data[3],
                            (uint)data[4], (uint)nanoSecond);
    }

    return result;
}


/*
 * As a string in 'hh:mm:ss[.fsec]' or 'hh :mm:ss[.fsec]' format. You can also use one of the following “relaxed” syntaxes:
 * 'hh:mm[.fsec]' or 'hh :mm[.fsec]' or 'hh :mm[.fsec]' or 'hh :'.
 */
static char *
mysProcessStringTimeForFormat2(char *str)
{
    char *result = NULL;
    int strIndex = 0;
    int strLen = strlen(str);
    // hour, min, second, fsec
    uint64 data[4] = {0};
    int dataIndex = 0;
    int nanoSecond = 0;

    if (strLen == 0 || str[0] == ' ')
    {
        return NULL;
    }

    for (; dataIndex < 4 - 1; dataIndex++)
    {
        uint64 tempValue = 0;
        for (; strIndex < strLen; strIndex++)
        {
            char cha = str[strIndex];
            if (isdigit(cha))
            {
                tempValue = tempValue * 10 + (cha - '0');
            }
            else
            {
                break;
            }
        }
        data[dataIndex] = tempValue;

        if (strIndex >= strLen)
        {
            break;
        }

        if (dataIndex == 0)     // hour
        {
            for (; strIndex < strLen; strIndex++)
            {
                char cha = str[strIndex];
                if (isspace(cha))
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            if (str[strIndex] != ':')
            {
                break;
            }
            else
            {
                strIndex++;     // 跳过':'
            }
        }
        else if (dataIndex == 1)    // min
        {
            if (str[strIndex] != ':')
            {
                break;
            }
            else
            {
                strIndex++;     // 跳过':'
            }
        }
        else if (dataIndex == 2)    // second
        {
            break;
        }
    }

    if (strIndex < strLen && str[strIndex] == '.')
    {
        //microsecond
        int fsec = 0;
        int fsecLen = 0;

        strIndex++;     // 跳过'.'
        // microsecond
        for (; strIndex < strLen; strIndex++)
        {
            char cha = str[strIndex];

            if (fsecLen >= MAX_TIMESTAMP_PRECISION)
            {
                break;
            }

            if (isdigit(cha))
            {
                fsec = fsec * 10 + (cha - '0');
                fsecLen++;
            }
            else
            {
                break;
            }
        }

        // nanoSecond
        if (strIndex < strLen && isdigit(str[strIndex]))
        {
            nanoSecond = str[strIndex] - '0';
        }

        strIndex++;
        for (; strIndex < strLen; strIndex++)
        {
            if (!isdigit(str[strIndex]))
            {
                break;
            }
        }

        data[3] = fsec * log_10_int[MAX_TIMESTAMP_PRECISION - fsecLen];
    }

    for (; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
        {
            return NULL;
        }
    }

    if (data[0] > MYS_MAX_TIME_HOUR || data[1] > 59 || data[2] > 59 || data[3] > 999999)
    {
        return NULL;
    }
    else
    {
        result = palloc0(64);
        pg_snprintf(result, 64, "%u:%02u:%02u.%06u%u",
                            (uint)data[0], (uint)data[1], (uint)data[2],
                            (uint)data[3], (uint)nanoSecond);
    }

    return result;
}


/*
 * As a string with no delimiters in 'hhmmss[.fsec]' format, provided that it makes sense as a time. For
 * example, '101112' is understood as '10:11:12', but '109712' is illegal (it has a nonsensical
 * minute part) and becomes '00:00:00'.
 */
static char *
mysProcessStringTimeForFormat3(char *str)
{
    char *result = NULL;
    int strIndex = 0;
    int strLen = strlen(str);
    // hour, min, second, fsec
    uint64 data[4] = {0};
    int nanoSecond = 0;
    uint64 value = 0;

    if (strLen == 0 || str[0] == ' ')
    {
        return NULL;
    }

    for (; strIndex < strLen; strIndex++)
    {
        char cha = str[strIndex];
        if (isdigit(cha))
        {
            value = value * 10 + (cha - '0');
        }
        else
        {
            break;
        }
    }

    {
        data[0] = value / 10000;
        data[1] = value / 100 % 100;
        data[2] = value % 100;
    }

    for (; strIndex < strLen; strIndex++)
    {
        if (isspace(str[strIndex]))
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (strIndex < strLen && str[strIndex] == '.')
    {
        //microsecond
        int fsec = 0;
        int fsecLen = 0;

        strIndex++;     // 跳过'.'
        // microsecond
        for (; strIndex < strLen; strIndex++)
        {
            char cha = str[strIndex];

            if (fsecLen >= MAX_TIMESTAMP_PRECISION)
            {
                break;
            }

            if (isdigit(cha))
            {
                fsec = fsec * 10 + (cha - '0');
                fsecLen++;
            }
            else
            {
                break;
            }
        }

        // nanoSecond
        if (strIndex < strLen && isdigit(str[strIndex]))
        {
            nanoSecond = str[strIndex] - '0';
        }

        strIndex++;
        for (; strIndex < strLen; strIndex++)
        {
            if (!isdigit(str[strIndex]))
            {
                break;
            }
        }

        data[3] = fsec * log_10_int[MAX_TIMESTAMP_PRECISION - fsecLen];
    }

    for (; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
        {
            return NULL;
        }
    }

    if (data[0] > MYS_MAX_TIME_HOUR || data[1] > 59 || data[2] > 59 || data[3] > 999999)
    {
        return NULL;
    }
    else
    {
        result = palloc0(64);
        pg_snprintf(result, 64, "%u:%02u:%02u.%06u%u",
                            (uint)data[0], (uint)data[1], (uint)data[2],
                            (uint)data[3], (uint)nanoSecond);
    }

    return result;
}


static char *
mysPreTimestampInForTime(char *str, bool *isTimestamp)
{
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    char *result;
    int strIndex;
    int strLen = strlen(str);
    int saveIndex;
    int yearLen;
    int numberOfParts;
    int currentPartLen;
    int tsPartIndex;
    int tsParts[MYS_MAX_TIMESTAMP_PARTS] = {0};
    int tsPartsLen[MYS_MAX_TIMESTAMP_PARTS] = {0};
    bool isInternalFormat = false;
    int tzDisplacement = 0;
    bool foundTzDisplacement = false;
    fsec_t fsec = 0;
    int nanoseconds = 0;
    bool foundDelimiter = false;
    bool foundSpace = false;
    int allowSpace;
    uint64 notZeroDate;

    for (strIndex = 0; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
            break;
    }

    if (strIndex == strLen || !isdigit(str[strIndex]))
    {
        goto error1;
    }

    saveIndex = strIndex;
    for (; strIndex < strLen; strIndex++)
    {
        char cha = str[strIndex];
        if (isdigit(cha) || cha == 'T')
            continue;
        else
            break;
    }

    if (strIndex == strLen || str[strIndex] == '.')
    {
        int numLen = strIndex - saveIndex;
        yearLen = (numLen == 4 || numLen == 8 || numLen >= 14) ? 4 : 2;
        currentPartLen = yearLen;
        isInternalFormat = true;
    }
    else
    {
        currentPartLen = 4;
    }

    strIndex = saveIndex;

    /*
     * Only allow space in the first "part" of the datetime field and:
     * - after days, part seconds
     * 
     * 2003-03-03 20:00:20.44
     */
    allowSpace = ((1 << 2) | (1 << 6));
    notZeroDate = 0;
    numberOfParts = 0;

    for (tsPartIndex = 0; 
         (tsPartIndex < MYS_MAX_TIMESTAMP_PARTS - 1 && strIndex < strLen && isdigit(str[strIndex]));
         tsPartIndex++)
    {
        int currentPartStartIndex = strIndex;
        int tempPartValue = 0;
        /*
         * Internal format means no delimiters; every field has a fixed
         * width. Otherwise, we scan until we find a delimiter and discard
         * leading zeroes -- except for the microsecond part, where leading
         * zeroes are significant, and where we never process more than six
         * digits.
         */
        bool scanUntilDelim = !isInternalFormat && (tsPartIndex != 6);

        while (strIndex < strLen && isdigit(str[strIndex]) &&
               (scanUntilDelim || currentPartLen--))
        {
            tempPartValue = tempPartValue * 10 + (int)(str[strIndex] - '0');
            strIndex++;
            if (tempPartValue > 999999)
            {
                goto error1;
            }
        }
        tsPartsLen[tsPartIndex] = strIndex - currentPartStartIndex;
        tsParts[tsPartIndex] = tempPartValue;
        notZeroDate |= tempPartValue;
        numberOfParts++;

        /* Length of next part */
        currentPartLen = 2;

        if (strIndex == strLen)
        {
            break;
        }

        /* Allow a 'T' after day to allow CCYYMMDDT type of fields */
        if (tsPartIndex == 2 && str[strIndex] == 'T')
        {
            if (str[strIndex] == 'T')
            {
                /* ISO8601:  CCYYMMDDThhmmss */
                strIndex++;
                continue;
            }
        }
        else if (tsPartIndex == 5)
        {
            /* Seconds */
            if (str[strIndex] == '.')
            {
                // Followed by part microsecond
                strIndex++;
                currentPartLen = 6;
            }
            else if (isdigit(str[strIndex]))
            {
                /*
                 * We do not see a decimal point which would have indicated a
                 * fractional second part in further read. So we skip the further
                 * processing of digits.
                 */
                break;
            }
            else if (str[strIndex] == '+' || str[strIndex] == '-')
            {
                // time zone
                if (tzDisplacement2Seconds(str, strLen - strIndex, &tzDisplacement))
                {
                    foundTzDisplacement = true;
                    strIndex += (strLen - strIndex);
                }
                else
                {
                    goto error1;
                }
            }
            continue;
        }
        else if (tsPartIndex == 6)
        {
            if (str[strIndex] == '+' || str[strIndex] == '-')
            {
                // time zone
                if (tzDisplacement2Seconds(str, strLen - strIndex, &tzDisplacement))
                {
                    foundTzDisplacement = true;
                    strIndex += (strLen - strIndex);
                }
                else
                {
                    goto error1;
                }
            }
        }

        while (strIndex < strLen && (ispunct(str[strIndex]) || isspace(str[strIndex])))
        {
            if (isspace(str[strIndex]))
            {
                if (!(allowSpace & (1 << tsPartIndex)))
                {
                    // '17. 0511101010.1234567'
                    goto error1;
                }
                foundSpace = true;
            }
            strIndex++;
            foundDelimiter = true;
        }
    }

    if (foundDelimiter)
    {
        if (!foundSpace)
        {
            goto error1;
        }
    }

    if (!isInternalFormat)
    {
        yearLen = tsPartsLen[0];
        if (!yearLen)
        {
            goto error1;
        }
    }
    
    {
        tm->tm_year = tsParts[0];
        tm->tm_mon = tsParts[1];
        tm->tm_mday = tsParts[2];
        tm->tm_hour = tsParts[3];
        tm->tm_min = tsParts[4];
        tm->tm_sec = tsParts[5];
        if (tsPartsLen[6] <= 6)
        {
            fsec = tsParts[6] * log_10_int[MAX_TIMESTAMP_PRECISION - tsPartsLen[6]];
        }
    }

    if (yearLen == 2 && notZeroDate)
    {
        tm->tm_year += (tm->tm_year < 70 ? 2000 : 1900);
    }

    *isTimestamp = true;

    if (numberOfParts < 3 || mysCheckTimestampRange(tm, fsec) == false)
    {
        goto error2;
    }

    if (tsPartsLen[6] == 6 && strIndex < strLen)
    {
        if (isdigit(str[strIndex]))
        {
            /*
                We don't need the exact nanoseconds value.
                Knowing the first digit is enough for rounding.
            */
            nanoseconds = str[strIndex] - '0';
            strIndex++;
            for (; strIndex < strLen && isdigit(str[strIndex]); strIndex++)
            {
                
            }
        }
    }

    if (strIndex < strLen)
    {
        if (str[strIndex] == '+' || str[strIndex] == '-')
        {
            if (tzDisplacement2Seconds(str, strLen - strIndex, &tzDisplacement))
            {
                *isTimestamp = true;
                foundTzDisplacement = true;
                strIndex += (strLen - strIndex);
            }
            else
            {
                goto error1;
            }
        }
        else
        {
            for (; strIndex < strLen; strIndex++)
            {
                if (!isspace(str[strIndex]))
                {
                    goto error2;
                }
            }
        }
    }

    {
        // char tzSign;
        // int tzHours;
        // int tzMins;

        // if (foundTzDisplacement)
        // {
        //     int tempTzDisplacement;
        //     if (tzDisplacement < 0)
        //     {
        //         tzSign = '-';
        //         tempTzDisplacement = tzDisplacement * -1;
        //     }
        //     else
        //     {
        //         tzSign = '+';
        //         tempTzDisplacement = tzDisplacement;
        //     }
        //     tzHours = tempTzDisplacement / SECS_PER_HOUR;
        //     tzMins = (tempTzDisplacement % SECS_PER_HOUR) / SECS_PER_MINUTE;
        // }

        result = palloc0(64);
        if (fsec != 0)
        {
            if (foundTzDisplacement)
            {
                // TODO: 5.7.32 与 8.0有不同表现
            }
            else
            {
                pg_snprintf(result, 64, "%04d-%02d-%02d %02d:%02d:%02d.%06d%d",
                            tm->tm_year, tm->tm_mon, tm->tm_mday,
                            tm->tm_hour, tm->tm_min, tm->tm_sec,
                            fsec, nanoseconds);
            }
        }
        else
        {
            if (foundTzDisplacement)
            {

            }
            else
            {
                pg_snprintf(result, 64, "%04d-%02d-%02d %02d:%02d:%02d",
                            tm->tm_year, tm->tm_mon, tm->tm_mday,
                            tm->tm_hour, tm->tm_min, tm->tm_sec);
            }
        }
    }

    return result;

error1:
    *isTimestamp = false;
    return NULL;
error2:
    *isTimestamp = true;
    return NULL;
}


/**
  Check datetime, date, or normalized time (i.e. time without days) range.
  @param tm  timestamp value.
  @retval false on error
  @retval true  on sucess
*/
static bool
mysCheckTimestampRange(struct pg_tm *tm, fsec_t fsec)
{
  return tm->tm_year <= 9999U && tm->tm_mon <= 12U && tm->tm_mday <= 31U &&
         tm->tm_hour <= 23U && tm->tm_min <= 59U && tm->tm_sec <= 59U &&
         fsec <= 999999U;
}


/*
 * Parses a time zone displacement string on the form `{+-}HH:MM`, converting
 * to seconds.
 *
 * @param[in]  str    Time zone displacement string.
 * @param[in]  length Length of said string.
 * @param[out] result Calculated displacement in seconds.
 *
 * @retval true Ok.
 * @retval false  Not a valid time zone displacement string.
 */
static bool
tzDisplacement2Seconds(const char *str, int length, int *result)
{
    int sign;
    int hours;
    int minutes;
    int seconds;
    int i;
    
    if (length < 6) return false;

    sign = str[0] == '+' ? 1 : (str[0] == '-' ? -1 : 0);
    if (sign == 0)
    {
        return false;
    }

    if (!(isdigit(str[1]) && isdigit(str[2])))
    {
        return false;
    }
    hours = (str[1] - '0') * 10 + str[2] - '0';

    if (str[3] != ':')
    {
        return false;
    }

    if (!(isdigit(str[4]) && isdigit(str[5])))
    {
        return false;
    }
    minutes = (str[4] - '0') * 10 + str[5] - '0';
    if (minutes >= MINS_PER_HOUR)
    {
        return false;
    }

    seconds = hours * SECS_PER_HOUR + minutes * SECS_PER_MINUTE;

    if (seconds > MYS_MAX_TIME_ZONE_HOURS * SECS_PER_HOUR)
    {
        return false;
    }

    // The SQL standard forbids -00:00.
    if (sign == -1 && hours == 0 && minutes == 0)
    {
        return false;
    }

    for (i = 6; i < length; ++i)
    {
        if (!isspace(str[i]))
        {
            return false;
        }
    }

    *result = seconds * sign;
    return true;
}

