/*-------------------------------------------------------------------------
 *
 * timestamp_func.c
 *	  Extend timestamp_func routines
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
 *	  src/backend/utils/ddsm/mysm/timestamp_func.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "fmgr.h"
#include "datatype/timestamp.h"
#include "parser/scansup.h"
#include "utils/datetime.h"
#include "utils/float.h"
#include "utils/fmgrprotos.h"
#include "utils/numeric.h"
#include "utils/timestamp.h"

static TimeOffset time2t(const int hour, const int min, const int sec, const fsec_t fsec);
static Datum timestamp_part_common(PG_FUNCTION_ARGS, bool retnumeric);
static float8 NonFiniteTimestampTzPart(int type, int unit, char *lowunits, bool isNegative, bool isTz);


PG_FUNCTION_INFO_V1(mys_extract_timestamp);

Datum
mys_extract_timestamp(PG_FUNCTION_ARGS)
{
	return timestamp_part_common(fcinfo, true);
}


PG_FUNCTION_INFO_V1(timestampdiff);

/* 
 * timestampdiff()
 * Returns datetime_expr2 − datetime_expr1, where datetime_expr1 and
 * datetime_expr2 are date or datetime expressions. One expression may be
 * a date and the other a datetime; a date value is treated as a datetime
 * having the time part '00:00:00' where necessary. The unit for the
 * result (an integer) is given by the unit argument. The legal values for
 * unit are the same as those listed in the description of the
 * TIMESTAMPADD() function.
 */
Datum
timestampdiff(PG_FUNCTION_ARGS)
{
    text *units = PG_GETARG_TEXT_PP(0);
	Timestamp ts1 = PG_GETARG_TIMESTAMP(1);
    Timestamp ts2 = PG_GETARG_TIMESTAMP(2);
    bool is_negative = ts1 < ts2 ? false : true;
    Interval *result;
    fsec_t fsec1;
    fsec_t fsec2;
    struct pg_tm tt1;
    struct pg_tm tt2;
    struct pg_tm *tm1 = &tt1;
    struct pg_tm *tm2 = &tt2;
    char *lowunits;
    int	type;
	int	val;
    int year_diff;
    int month_diff;
    int64 intresult;

    result = (Interval *) palloc(sizeof(Interval));

	if (TIMESTAMP_NOT_FINITE(ts1) || TIMESTAMP_NOT_FINITE(ts2))
		ereport(ERROR,
				(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
				 errmsg("cannot subtract infinite timestamps")));

    if (is_negative)
    {
        Timestamp temp_ts = ts1;
        ts1 = ts2;
        ts2 = temp_ts;
    }


    /* 根据tm1, tm2求出相差的year, month */
    timestamp2tm(ts1, NULL, tm1, &fsec1, NULL, NULL);
    timestamp2tm(ts2, NULL, tm2, &fsec2, NULL, NULL);
    {
        month_diff = tm2->tm_mon - tm1->tm_mon;
        if (tm2->tm_mday < tm1->tm_mday)
            month_diff -= 1;
        else if (tm2->tm_mday == tm1->tm_mday)
        {
            TimeOffset time1 = time2t(tm1->tm_hour, tm1->tm_min, tm1->tm_sec, fsec1);
            TimeOffset time2 = time2t(tm2->tm_hour, tm2->tm_min, tm2->tm_sec, fsec2);
            if (time2 < time1)
                month_diff -= 1;
        }

        year_diff = tm2->tm_year - tm1->tm_year;
        if (month_diff < 0)
        {
            month_diff = year_diff * 12 + month_diff;
            year_diff -= 1;
        }
        else
        {
            month_diff = year_diff * 12 + month_diff;
        }
    }


    /* 处理interval */
    result->time = ts2 - ts1;
    result->month = 0;
	result->day = 0;
    result = DatumGetIntervalP(DirectFunctionCall1(interval_justify_hours, IntervalPGetDatum(result)));


    lowunits = downcase_truncate_identifier(VARDATA_ANY(units),
											VARSIZE_ANY_EXHDR(units),
											false);

	type = DecodeUnits(0, lowunits, &val);
	if (type == UNKNOWN_FIELD)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("timestampdiff units \"%s\" not recognized", lowunits)));
    

    if (type == UNITS)
    {
        fsec_t fsec;
        struct pg_tm tt;
        struct pg_tm *tm = &tt;

        if (interval2tm(*result, tm, &fsec) == 0)
        {
            switch (val)
            {
                case DTK_MICROSEC:
                    {
                        TimeOffset time = time2t(tm->tm_hour, tm->tm_min, tm->tm_sec, fsec);
                        intresult = (tm->tm_mday * USECS_PER_DAY) + time;
                        break;
                    }
                case DTK_SECOND:
                    {
                        TimeOffset second = (((tm->tm_hour * MINS_PER_HOUR) + tm->tm_min) * SECS_PER_MINUTE) + tm->tm_sec;
                        intresult = (tm->tm_mday * SECS_PER_DAY) + second;
                        break;
                    }
                case DTK_MINUTE:
                    {
                        TimeOffset minute = (tm->tm_hour * MINS_PER_HOUR) + tm->tm_min;
                        intresult = (tm->tm_mday * HOURS_PER_DAY * MINS_PER_HOUR) + minute;
                        break;
                    }
                case DTK_HOUR:
                    {
                        TimeOffset hour = tm->tm_hour;
                        intresult = (tm->tm_mday * HOURS_PER_DAY) + hour;
                        break;
                    }
                case DTK_DAY:
                    intresult = tm->tm_mday;
                    break;
                case DTK_WEEK:
                    intresult = tm->tm_mday / 7;
                    break;
                case DTK_MONTH:
                    intresult = month_diff;
                    break;
                case DTK_QUARTER:
                    intresult = month_diff / 3;
                    break;
                case DTK_YEAR:
                    intresult = year_diff;
                    break;
                default:
                        ereport(ERROR,
                                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                                errmsg("timestampdiff units \"%s\" not supported",
                                        lowunits)));
                        intresult = 0;
            }
        }
        else
		{
			elog(ERROR, "could not convert interval to tm");
			intresult = 0;
		}
    }
    else
    {
        ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("interval units \"%s\" not recognized",
						lowunits)));
		intresult = 0;
    }

    if (is_negative)
    {
        intresult *= -1;
    }

    PG_RETURN_INT64(intresult);
}


static TimeOffset
time2t(const int hour, const int min, const int sec, const fsec_t fsec)
{
	return (((((hour * MINS_PER_HOUR) + min) * SECS_PER_MINUTE) + sec) * USECS_PER_SEC) + fsec;
}


/* timestamp_part() and extract_timestamp()
 * Extract specified field from timestamp.
 */
static Datum
timestamp_part_common(PG_FUNCTION_ARGS, bool retnumeric)
{
	text	   *units = PG_GETARG_TEXT_PP(0);
	Timestamp	timestamp = PG_GETARG_TIMESTAMP(1);
	int64		intresult;
	Timestamp	epoch;
	int			type,
				val;
	char	   *lowunits;
	fsec_t		fsec;
	struct pg_tm tt,
			   *tm = &tt;

	lowunits = downcase_truncate_identifier(VARDATA_ANY(units),
											VARSIZE_ANY_EXHDR(units),
											false);

	type = DecodeUnits(0, lowunits, &val);
	if (type == UNKNOWN_FIELD)
		type = DecodeSpecial(0, lowunits, &val);

	if (TIMESTAMP_NOT_FINITE(timestamp))
	{
		double		r = NonFiniteTimestampTzPart(type, val, lowunits,
												 TIMESTAMP_IS_NOBEGIN(timestamp),
												 false);

		if (r)
		{
			if (retnumeric)
			{
				if (r < 0)
					return DirectFunctionCall3(numeric_in,
											   CStringGetDatum("-Infinity"),
											   ObjectIdGetDatum(InvalidOid),
											   Int32GetDatum(-1));
				else if (r > 0)
					return DirectFunctionCall3(numeric_in,
											   CStringGetDatum("Infinity"),
											   ObjectIdGetDatum(InvalidOid),
											   Int32GetDatum(-1));
			}
			else
				PG_RETURN_FLOAT8(r);
		}
		else
			PG_RETURN_NULL();
	}

	if (type == UNITS)
	{
		if (timestamp2tm(timestamp, NULL, tm, &fsec, NULL, NULL) != 0)
			ereport(ERROR,
					(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
					 errmsg("timestamp out of range")));

		switch (val)
		{
			case DTK_MICROSEC:
				intresult = fsec;
				break;

			case DTK_MILLISEC:
				if (retnumeric)
					/*---
					 * tm->tm_sec * 1000 + fsec / 1000
					 * = (tm->tm_sec * 1'000'000 + fsec) / 1000
					 */
					PG_RETURN_NUMERIC(int64_div_fast_to_numeric(tm->tm_sec * INT64CONST(1000000) + fsec, 3));
				else
					PG_RETURN_FLOAT8(tm->tm_sec * 1000.0 + fsec / 1000.0);
				break;

			case DTK_SECOND:
                intresult = tm->tm_sec;
				break;

			case DTK_MINUTE:
				intresult = tm->tm_min;
				break;

			case DTK_HOUR:
				intresult = tm->tm_hour;
				break;

			case DTK_DAY:
				intresult = tm->tm_mday;
				break;

			case DTK_MONTH:
				intresult = tm->tm_mon;
				break;

			case DTK_QUARTER:
				intresult = (tm->tm_mon - 1) / 3 + 1;
				break;

			case DTK_WEEK:
				intresult = date2isoweek(tm->tm_year, tm->tm_mon, tm->tm_mday);
				break;

			case DTK_YEAR:
				if (tm->tm_year > 0)
					intresult = tm->tm_year;
				else
					/* there is no year 0, just 1 BC and 1 AD */
					intresult = tm->tm_year - 1;
				break;

			case DTK_DECADE:

				/*
				 * what is a decade wrt dates? let us assume that decade 199
				 * is 1990 thru 1999... decade 0 starts on year 1 BC, and -1
				 * is 11 BC thru 2 BC...
				 */
				if (tm->tm_year >= 0)
					intresult = tm->tm_year / 10;
				else
					intresult = -((8 - (tm->tm_year - 1)) / 10);
				break;

			case DTK_CENTURY:

				/* ----
				 * centuries AD, c>0: year in [ (c-1)* 100 + 1 : c*100 ]
				 * centuries BC, c<0: year in [ c*100 : (c+1) * 100 - 1]
				 * there is no number 0 century.
				 * ----
				 */
				if (tm->tm_year > 0)
					intresult = (tm->tm_year + 99) / 100;
				else
					/* caution: C division may have negative remainder */
					intresult = -((99 - (tm->tm_year - 1)) / 100);
				break;

			case DTK_MILLENNIUM:
				/* see comments above. */
				if (tm->tm_year > 0)
					intresult = (tm->tm_year + 999) / 1000;
				else
					intresult = -((999 - (tm->tm_year - 1)) / 1000);
				break;

			case DTK_JULIAN:
				if (retnumeric)
					PG_RETURN_NUMERIC(numeric_add_opt_error(int64_to_numeric(date2j(tm->tm_year, tm->tm_mon, tm->tm_mday)),
															numeric_div_opt_error(int64_to_numeric(((((tm->tm_hour * MINS_PER_HOUR) + tm->tm_min) * SECS_PER_MINUTE) + tm->tm_sec) * INT64CONST(1000000) + fsec),
																				  int64_to_numeric(SECS_PER_DAY * INT64CONST(1000000)),
																				  NULL),
															NULL));
				else
					PG_RETURN_FLOAT8(date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) +
									 ((((tm->tm_hour * MINS_PER_HOUR) + tm->tm_min) * SECS_PER_MINUTE) +
									  tm->tm_sec + (fsec / 1000000.0)) / (double) SECS_PER_DAY);
				break;

			case DTK_ISOYEAR:
				intresult = date2isoyear(tm->tm_year, tm->tm_mon, tm->tm_mday);
				/* Adjust BC years */
				if (intresult <= 0)
					intresult -= 1;
				break;

			case DTK_DOW:
			case DTK_ISODOW:
				intresult = j2day(date2j(tm->tm_year, tm->tm_mon, tm->tm_mday));
				if (val == DTK_ISODOW && intresult == 0)
					intresult = 7;
				break;

			case DTK_DOY:
				intresult = (date2j(tm->tm_year, tm->tm_mon, tm->tm_mday)
							 - date2j(tm->tm_year, 1, 1) + 1);
				break;

			case DTK_TZ:
			case DTK_TZ_MINUTE:
			case DTK_TZ_HOUR:
			default:
				ereport(ERROR,
						(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
						 errmsg("timestamp units \"%s\" not supported",
								lowunits)));
				intresult = 0;
		}
	}
	else if (type == RESERV)
	{
		switch (val)
		{
			case DTK_EPOCH:
				epoch = SetEpochTimestamp();
				/* (timestamp - epoch) / 1000000 */
				if (retnumeric)
				{
					Numeric		result;

					if (timestamp < (PG_INT64_MAX + epoch))
						result = int64_div_fast_to_numeric(timestamp - epoch, 6);
					else
					{
						result = numeric_div_opt_error(numeric_sub_opt_error(int64_to_numeric(timestamp),
																			 int64_to_numeric(epoch),
																			 NULL),
													   int64_to_numeric(1000000),
													   NULL);
						result = DatumGetNumeric(DirectFunctionCall2(numeric_round,
																	 NumericGetDatum(result),
																	 Int32GetDatum(6)));
					}
					PG_RETURN_NUMERIC(result);
				}
				else
				{
					float8		result;

					/* try to avoid precision loss in subtraction */
					if (timestamp < (PG_INT64_MAX + epoch))
						result = (timestamp - epoch) / 1000000.0;
					else
						result = ((float8) timestamp - epoch) / 1000000.0;
					PG_RETURN_FLOAT8(result);
				}
				break;

			default:
				ereport(ERROR,
						(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
						 errmsg("timestamp units \"%s\" not supported",
								lowunits)));
				intresult = 0;
		}

	}
	else
	{
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("timestamp units \"%s\" not recognized", lowunits)));
		intresult = 0;
	}

	if (retnumeric)
		PG_RETURN_NUMERIC(int64_to_numeric(intresult));
	else
		PG_RETURN_FLOAT8(intresult);
}


/*
 * NonFiniteTimestampTzPart
 *
 *	Used by timestamp_part and timestamptz_part when extracting from infinite
 *	timestamp[tz].  Returns +/-Infinity if that is the appropriate result,
 *	otherwise returns zero (which should be taken as meaning to return NULL).
 *
 *	Errors thrown here for invalid units should exactly match those that
 *	would be thrown in the calling functions, else there will be unexpected
 *	discrepancies between finite- and infinite-input cases.
 */
static float8
NonFiniteTimestampTzPart(int type, int unit, char *lowunits,
						 bool isNegative, bool isTz)
{
	if ((type != UNITS) && (type != RESERV))
	{
		if (isTz)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("timestamp with time zone units \"%s\" not recognized",
							lowunits)));
		else
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("timestamp units \"%s\" not recognized",
							lowunits)));
	}

	switch (unit)
	{
			/* Oscillating units */
		case DTK_MICROSEC:
		case DTK_MILLISEC:
		case DTK_SECOND:
		case DTK_MINUTE:
		case DTK_HOUR:
		case DTK_DAY:
		case DTK_MONTH:
		case DTK_QUARTER:
		case DTK_WEEK:
		case DTK_DOW:
		case DTK_ISODOW:
		case DTK_DOY:
		case DTK_TZ:
		case DTK_TZ_MINUTE:
		case DTK_TZ_HOUR:
			return 0.0;

			/* Monotonically-increasing units */
		case DTK_YEAR:
		case DTK_DECADE:
		case DTK_CENTURY:
		case DTK_MILLENNIUM:
		case DTK_JULIAN:
		case DTK_ISOYEAR:
		case DTK_EPOCH:
			if (isNegative)
				return -get_float8_infinity();
			else
				return get_float8_infinity();

		default:
			if (isTz)
				ereport(ERROR,
						(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
						 errmsg("timestamp with time zone units \"%s\" not supported",
								lowunits)));
			else
				ereport(ERROR,
						(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
						 errmsg("timestamp units \"%s\" not supported",
								lowunits)));
			return 0.0;			/* keep compiler quiet */
	}
}
