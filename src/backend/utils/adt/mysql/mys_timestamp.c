#include "postgres.h"

#include "datatype/timestamp.h"
#include "pgtime.h"
#include "utils/datetime.h"
#include "utils/mysql/mys_date.h"
#include "utils/mysql/mys_timestamp.h"

// year, month, day, hour, minute, second, microsecond, timezone
#define MYS_MAX_TIMESTAMP_PARTS 8
#define MYS_MAX_TIME_ZONE_HOURS 14


static const int64 log_10_int[MAX_TIMESTAMP_PRECISION + 1] = {
		INT64CONST(1),
		INT64CONST(10),
		INT64CONST(100),
		INT64CONST(1000),
		INT64CONST(10000),
		INT64CONST(100000),
		INT64CONST(1000000)
	};

static bool mys_decodeNumberDatetime(int64 number, struct pg_tm *tm, int flags);
static bool mysCheckTimestampRange(struct pg_tm *tm, fsec_t fsec);
static bool tzDisplacement2Seconds(const char *str, int length, int *result);


Datum
mys_timestamp_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
#ifdef NOT_USED
	Oid typelem = PG_GETARG_OID(1);
#endif
	int32 typmod = PG_GETARG_INT32(2);
    int timeFlags = MYS_TIME_NO_ZERO_IN_DATE | MYS_TIME_NO_ZERO_DATE;
    Timestamp result;

    if (mys_strToDatetimeInternal(str, timeFlags, &result) == false)
    {
        elog(ERROR, "Incorrect datetime value: '%s'", str);
    }

    AdjustTimestampForTypmod(&result, typmod);

	PG_RETURN_TIMESTAMP(result);
}


bool
mys_strToDatetimeInternal(char *str, int flags, Timestamp *result)
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
         * cast('2022-10-10 10:00:00+06:00' as datetime)，需要转换到当前会话时区
         *      假设当前时区为+08:00
         *      out: 2022-10-10 12:00:00
         */
        if (tm2timestamp(tm, fsec, &tz, result) != 0)
        {
            return false;
        }

        if (timestamp2tm(*result, &tz, tm, &fsec, NULL, session_timezone) != 0)
			ereport(ERROR,
					(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
					 errmsg("timestamp out of range")));
		if (tm2timestamp(tm, fsec, NULL, result) != 0)
			ereport(ERROR,
					(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
					 errmsg("timestamp out of range")));
    }
    else
    {
        /* 
         * cast('2022-10-10 10:00:00' as datetime)
         *      out: 2022-10-10 10:00:00
         */
        if (tm2timestamp(tm, fsec, NULL, result) != 0)
        {
            return false;
        }
    }

    return true;
}


Timestamp
mys_numberToDatetimeInternal(int64 number, int flags)
{
    Timestamp result;
    struct pg_tm tt;
    struct pg_tm *tm = &tt;

    if (mys_decodeNumberDatetime(number, tm, flags))
    {
        if (tm2timestamp(tm, 0, NULL, &result) != 0)
        {
           elog(ERROR, "Invalid datetime value: '%ld'", number);
        }
    }

    return result;
}


// TimeADT
// mys_strTotime(char *str, int flags)
// {

// }


TimeADT
mys_numberToTimeInternal(int64 number)
{
    TimeADT result;
    struct pg_tm tt;
    struct pg_tm *tm = &tt;
    int sign = ((number < 0) ? -1 : 1);

    memset(tm, 0, sizeof(struct pg_tm));

    if (number > MYS_MAX_TIME_HOUR)
    {
        /* For huge numbers try full DATETIME, like str_to_time does. */
        if (number >= (int64)10000000000) /* '0001-00-00 00-00-00' */
        {
            if (mys_decodeNumberDatetime(number, tm, 0))
            {
                goto ok;
            }
        }
        elog(ERROR, "Incorrect time value: '%ld' out of range", number);
    }
    else if (number < -MYS_MAX_TIME_HOUR)
    {
        elog(ERROR, "Incorrect time value: '%ld' out of range", number);
    }

    if (sign < 0)
    {
        number = -number;
    }

    if (number / 100 % 100 >= 60 || number % 100 >= 60) /* Check minutes and seconds */
    {
        elog(ERROR, "Incorrect time value: '%ld' out of range", number);
    }

    tm->tm_hour = number / 10000;
    tm->tm_min = (number / 100) % 100;
    tm->tm_sec = (number % 100);

ok:
    tm2time(tm, 0, &result);
    return result;
}


static bool
mys_decodeNumberDatetime(int64 number, struct pg_tm *tm, int flags)
{
    int64 part1;
    int64 part2;

    memset(tm, 0, sizeof(struct pg_tm));

    /* 10000101000000 ==> 1000-01-01 00:00:00 */
    if (number == 0 || number >= 10000101000000)
    {
        if (number > 99999999999999) /* 9999-99-99 99:99:99 */
        {
            elog(ERROR, "Incorrect datetime value: '%ld' out of range", number);
        }
        goto ok;
    }

    if (number < 101)
    {
        goto err;
    }

    if (number <= 691231) /* 2069-12-31 00:00:00 */
    {
        number= (number + 20000000) * 1000000;                 /* YYMMDD, year: 2000-2069 */
        goto ok;
    }
    
    if (number < 700101) /* 1970-01-01 00:00:00 */
    {
        goto err;
    }

    if (number <= 991231) /* 1999-12-31 00:00:00 */
    {
        number = (number + 19000000) * 1000000;                 /* YYMMDD, year: 1970-1999 */
        goto ok;
    }

    /*
     * Though officially we support DATE values from 1000-01-01 only, one can
     * easily insert a value like 1-1-1. So, for consistency reasons such dates
     * are allowed when TIME_FUZZY_DATE is set.
     */
    if (number < 10000101 && !(flags & MYS_TIME_FUZZY_DATE))
    {
        goto err;
    }

    if (number <= 99991231)
    {
        number = number * 1000000;
        goto ok;
    }

    if (number < 101000000)
    {
        goto err;
    }

    if (number <= 691231235959)
    {
        number = number + 20000000000000;                   /* YYMMDDHHMMSS, 2000-2069 */
        goto ok;
    }

    if (number <  700101000000)
    {
        goto err;
    }

    if (number <= 991231235959)
    {
        number = number + 19000000000000;		/* YYMMDDHHMMSS, 1970-1999 */
    }

ok:
    part1 = (int64)(number / 1000000);
    part2 = (int64)(number - (int64)part1 * 1000000);
    tm->tm_year=  (int) (part1/10000L);  part1%=10000L;
    tm->tm_mon = (int) part1 / 100;
    tm->tm_mday = (int) part1 % 100;
    tm->tm_hour = (int) (part2/10000L);  part2%=10000L;
    tm->tm_min = (int) part2 / 100;
    tm->tm_sec = (int) part2 % 100;

    if (mysCheckTimestampRange(tm, 0) &&
        mysCheckDate(tm, (number != 0), flags))
    {
        return true;
    }

    /* Don't want to have was_cut get set if TIME_NO_ZERO_DATE was violated. */
    if (!number && (flags & MYS_TIME_NO_ZERO_DATE))
        return false;

err:
    return false;
}


bool
mys_DecodeStringDatetimeForCompare(char *str, struct pg_tm *tm, fsec_t *fsec, int flags)
{
    int tz = 0;
    bool haveTz = false;

    if (mys_decodeStringDatetime(str, tm, fsec, &tz, flags, &haveTz))
    {
        if (haveTz)
        {
            elog(ERROR, "Unsupport timezone for function mys_DecodeStringDatetimeForCompare");
        }

        if (*fsec == USECS_PER_SEC)
        {
            /* 
             * '2022-09-00 00:00:00.1257'
             * or '2022-09-01 00:00:00.5786'
             */
            if (mysCheckDate(tm, true, (MYS_TIME_NO_ZERO_IN_DATE | MYS_TIME_NO_ZERO_DATE)) == false)
            {
                /* 
                 * 不正确的datetime
                 * '2022-09-00 00:00:00.9999996'
                 * or '2022-09-31 00:00:00.9999995'
                 */
                return false;
            }
            else
            {
                /*
                 * 正确的datetime
                 * '2022-09-05 00:00:00.9999996'
                 */
                Timestamp result;

                if (tm2timestamp(tm, *fsec, NULL, &result) != 0)
                {
                    ereport(ERROR,
                            (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                            errmsg("timestamp out of range: \"%s\"", str)));
                }

                if (timestamp2tm(result, NULL, tm, fsec, NULL, NULL) != 0)
                {
                    ereport(ERROR,
                            (errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
                            errmsg("timestamp out of range")));
                }
            }
        }
        else
        {
            /* 
             * '2022-09-00 00:00:00.1257'
             * or '2022-09-01 00:00:00.5786'
             */
            /* Nothing todo */
        }
    }
    else
    {
        return false;
    }

    return true;
}


/*
 * return:
 *      ture: Sucess
 *      false: Failed
 */
bool
mys_decodeStringDatetime(char *str, struct pg_tm *tm, fsec_t *fsec, int *tzp, int flags, bool *haveTz)
{
    int strIndex;
    int saveIndex;
    int yearLen;
    int numberOfParts;
    int currentPartLen;
    int tsPartIndex;
    int tsParts[MYS_MAX_TIMESTAMP_PARTS] = {0};
    int tsPartsLen[MYS_MAX_TIMESTAMP_PARTS] = {0};
    bool isInternalFormat = false;
    int strLen = strlen(str);
    int tzDisplacement = 0;
    bool foundTzDisplacement = false;
    // bool foundDelimiter = false;
    // bool foundSpace = false;
    int allowSpace;
    uint64 notZeroDate;

    *fsec = 0;

    for (strIndex = 0; strIndex < strLen; strIndex++)
    {
        if (!isspace(str[strIndex]))
            break;
    }

    if (strIndex == strLen || !isdigit(str[strIndex]))
    {
        goto error;
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
                goto error;
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
                if (tzDisplacement2Seconds(str + strIndex, strLen - strIndex, &tzDisplacement))
                {
                    foundTzDisplacement = true;
                    strIndex += (strLen - strIndex);
                }
                else
                {
                    goto error;
                }
            }
            continue;
        }
        else if (tsPartIndex == 6)
        {
            if (str[strIndex] == '+' || str[strIndex] == '-')
            {
                // time zone
                if (tzDisplacement2Seconds(str + strIndex, strLen - strIndex, &tzDisplacement))
                {
                    foundTzDisplacement = true;
                    strIndex += (strLen - strIndex);
                }
                else
                {
                    goto error;
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
                    goto error;
                }
                // foundSpace = true;
            }
            strIndex++;
            // foundDelimiter = true;
        }
    }

    if (!isInternalFormat)
    {
        yearLen = tsPartsLen[0];
        if (!yearLen)
        {
            goto error;
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
            *fsec = tsParts[6] * log_10_int[MAX_TIMESTAMP_PRECISION - tsPartsLen[6]];
        }
    }

    if (yearLen == 2 && notZeroDate)
    {
        tm->tm_year += (tm->tm_year < 70 ? 2000 : 1900);
    }

    if (numberOfParts < 3 || mysCheckTimestampRange(tm, *fsec) == false)
    {
        goto error;
    }

    if (mysCheckDate(tm, notZeroDate, flags) == false)
    {
        goto error;
    }

    if (tsPartsLen[6] == 6 && strIndex < strLen)
    {
        if (isdigit(str[strIndex]))
        {
            /*
                We don't need the exact nanoseconds value.
                Knowing the first digit is enough for rounding.
            */
            *fsec += ((str[strIndex] - '0' >= 5) ? 1 : 0);
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
            if (tzDisplacement2Seconds(str + strIndex, strLen - strIndex, &tzDisplacement))
            {
                foundTzDisplacement = true;
                strIndex += (strLen - strIndex);
            }
            else
            {
                goto error;
            }
        }
        else
        {
            for (; strIndex < strLen; strIndex++)
            {
                if(!isspace(str[strIndex]))
                {
                    goto error;
                }
            }
        }
    }

    if (foundTzDisplacement)
    {
        *tzp = tzDisplacement;
        *haveTz = true;
    }
    else
    {
        *tzp = 0;
        *haveTz = false;
    }

    return true;

error:
    return false;
}


/*
 * Check datetime, date, or normalized time (i.e. time without days) range.
 * return:
 *      ture: Sucess
 *      false: Failed
 */
static bool
mysCheckTimestampRange(struct pg_tm *tm, fsec_t fsec)
{
  return tm->tm_year <= 9999U && tm->tm_mon <= 12U && tm->tm_mday <= 31U &&
         tm->tm_hour <= 23U && tm->tm_min <= 59U && tm->tm_sec <= 59U &&
         fsec <= 999999U;
}


/*
 * return:
 *      ture: Sucess
 *      false: Failed
 */
bool
mysCheckDate(const struct pg_tm *tm, bool not_zero_date, int flags)
{
    if (not_zero_date)
    {
        if (((flags & MYS_TIME_NO_ZERO_IN_DATE) || !(flags & MYS_TIME_FUZZY_DATE)) &&
            (tm->tm_mon == 0 || tm->tm_mday == 0))
        {
            return false;
        }
        else if ((!(flags & MYS_TIME_INVALID_DATES)) &&
                 (tm->tm_mday > day_tab[isleap(tm->tm_year)][tm->tm_mon - 1]))
        {
            return false;
        }
    }
    else if (flags & MYS_TIME_NO_ZERO_DATE)
    {
        return false;
    }

    return true;
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

    *result = -(seconds * sign);
    return true;
}

