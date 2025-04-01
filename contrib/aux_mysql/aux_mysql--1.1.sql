
CREATE SCHEMA IF NOT EXISTS mysql;
GRANT USAGE ON SCHEMA mysql TO PUBLIC;

CREATE SCHEMA IF NOT EXISTS mys_informa_schema;
GRANT USAGE ON SCHEMA mys_informa_schema TO PUBLIC;

CREATE SCHEMA IF NOT EXISTS performance_schema;
GRANT USAGE ON SCHEMA performance_schema TO PUBLIC;

CREATE SCHEMA IF NOT EXISTS sys;
GRANT USAGE ON SCHEMA sys TO PUBLIC;

CREATE SCHEMA IF NOT EXISTS mys_sys;
GRANT USAGE ON SCHEMA mys_sys TO PUBLIC;


create domain mysql.tinyint as pg_catalog.int2 check((-128 <= value) and (VALUE <= 127));
create domain mysql."tinyint signed" as pg_catalog.int2 check((-128 <= value) and (VALUE <= 127));
create domain mysql."tinyint unsigned" as pg_catalog.int2 check((0 <= value) and (VALUE <= 255));
create domain mysql.tinyint1 as pg_catalog.int2 check((-128 <= value) and (VALUE <= 127));
create domain mysql."tinyint1 signed" as pg_catalog.int2 check((-128 <= value) and (VALUE <= 127));
create domain mysql."tinyint1 unsigned" as pg_catalog.int2 check((0 <= value) and (VALUE <= 255));
create domain mysql.smallint as pg_catalog.int2;
create domain mysql."smallint signed" as pg_catalog.int2;
create domain mysql."smallint unsigned" as pg_catalog.int4 check((0 <= value) and (VALUE <= 65535));
create domain mysql.mediumint as pg_catalog.int4 check((-8388608 <= value) and (VALUE <= 8388607));
create domain mysql."mediumint signed" as pg_catalog.int4 check((-8388608 <= value) and (VALUE <= 8388607));
create domain mysql."mediumint unsigned" as pg_catalog.int4 check((0 <= VALUE) and (VALUE <= 16777215));
--create domain mysql.int as pg_catalog.int4;
create domain mysql."int signed" as pg_catalog.int4;
create domain mysql."int unsigned" as pg_catalog.int8 check((0 <= VALUE) and (VALUE <= 4294967295));
--create domain mysql.bigint as pg_catalog.int8;
create domain mysql."bigint signed" as pg_catalog.int8;
create domain mysql."bigint unsigned" as pg_catalog.int8 check(0 <= VALUE);
-- create domain mysql.char as pg_catalog.bpchar;
-- create domain mysql.varchar as pg_catalog.varchar;
create domain mysql.real as pg_catalog.float8;
create domain mysql.double as pg_catalog.float8;
create domain mysql.datetime as pg_catalog.timestamp;
--create domain mysql.tinytext as pg_catalog.text;
--create domain mysql.text as pg_catalog.text;
--create domain mysql."text byte" as pg_catalog.bytea;
--create domain mysql.mediumtext as pg_catalog.text;
--create domain mysql.longtext as pg_catalog.text;
create domain mysql.binary as pg_catalog.bytea;
create domain mysql.varbinary as pg_catalog.bytea;
--create domain mysql.bit as pg_catalog.bytea;
--create domain mysql.tinyblob as pg_catalog.bytea;
create domain mysql.blob as pg_catalog.bytea;
--create domain mysql.mediumblob as pg_catalog.bytea;
--create domain mysql.longblob as pg_catalog.bytea;
create domain mysql.year_ as pg_catalog.int4 check((1901 <= VALUE) and (VALUE <= 2155));


update pg_cast set castcontext = 'i' where castsource = 23 and casttarget = 16;
update pg_cast set castcontext = 'a' where castsource = 16 and casttarget = 23;

create or replace function mysql.cast_boolean_to_tinyint(pg_catalog.bool) 
returns int2
as 
$$
begin
    if $1 then
        return 1;
    else
        return 0;
    end if;
end;
$$ 
language 'plpgsql' strict; 
create cast (pg_catalog.bool as int2) with function mysql.cast_boolean_to_tinyint as assignment;

create or replace function mysql.cast_boolean_to_bigint(pg_catalog.bool) 
returns int8
as 
$$
begin
    if $1 then
        return 1;
    else
        return 0;
    end if;
end;
$$ 
language 'plpgsql' strict; 
create cast (pg_catalog.bool as int8) with function mysql.cast_boolean_to_bigint as assignment;

CREATE OR REPLACE FUNCTION mysql.cast_bittext_to_bytea(text)
RETURNS bytea
AS '$libdir/mysm', 'cast_bittext_to_bytea'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.cast_bit_to_bytea(bit)
RETURNS bytea
as 
$$
BEGIN
    return mysql.cast_bittext_to_bytea($1::text);
END; 
$$ 
language 'plpgsql'; 
create cast (pg_catalog.bit as pg_catalog.bytea) with function mysql.cast_bit_to_bytea as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.interval(text, integer)
RETURNS pg_catalog.interval
LANGUAGE c
IMMUTABLE PARALLEL SAFE STRICT
AS '$libdir/mysm', $function$text_interval$function$;
CREATE CAST(text as pg_catalog.interval) with function mysql.interval(text, integer) as implicit;

CREATE OR REPLACE FUNCTION mysql.bit_to_text(pg_catalog.bit)
RETURNS text
AS '$libdir/mysm', 'mys_bit_to_text'
LANGUAGE C STRICT IMMUTABLE;
create cast (pg_catalog.bit as text) with function mysql.bit_to_text as ASSIGNMENT;

DROP CAST IF EXISTS(pg_catalog.bit as bytea);
DROP FUNCTION IF EXISTS mysql.cast_bittext_to_bytea(text);
DROP FUNCTION IF EXISTS mysql.cast_bit_to_bytea(bit);
CREATE OR REPLACE FUNCTION mysql.cast_bit_to_bytea(pg_catalog.bit)
RETURNS bytea
AS '$libdir/mysm', 'cast_bittext_to_bytea'
LANGUAGE C STRICT IMMUTABLE;
create cast (pg_catalog.bit as bytea) with function mysql.cast_bit_to_bytea as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_text_to_bigint_for_mysql(text)
RETURNS int8
AS '$libdir/mysm', 'convert_text_to_bigint_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_char_to_tinyint(char)
returns int2
as
$$
DECLARE
    tmp int2;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp::int2;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (char as int2) with function mysql.cast_char_to_tinyint as ASSIGNMENT;

create or replace function mysql.cast_char_to_int(char)
returns int4
as
$$
DECLARE
    tmp int4;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp::int4;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (char as int4) with function mysql.cast_char_to_int as ASSIGNMENT;

create or replace function mysql.cast_char_to_bigint(char)
returns int8
as
$$
DECLARE
    tmp int8;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (char as int8) with function mysql.cast_char_to_bigint as ASSIGNMENT;

create or replace function mysql.cast_varchar_to_tinyint(varchar)
returns int2
as
$$
DECLARE
    tmp int2;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp::int2;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (varchar as int2) with function mysql.cast_varchar_to_tinyint as ASSIGNMENT;

create or replace function mysql.cast_varchar_to_int(varchar)
returns int4
as
$$
DECLARE
    tmp int4;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp::int4;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (varchar as int4) with function mysql.cast_varchar_to_int as ASSIGNMENT;

create or replace function mysql.cast_varchar_to_bigint(varchar)
returns int8
as
$$
DECLARE
    tmp int8;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1::text));
        if (tmp is not null) then
            return tmp;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (varchar as int8) with function mysql.cast_varchar_to_bigint as ASSIGNMENT;

create or replace function mysql.cast_text_to_tinyint(text)
returns int2
as
$$
DECLARE
    tmp int2;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1));
        if (tmp is not null) then
            return tmp::int2;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (text as int2) with function mysql.cast_text_to_tinyint as ASSIGNMENT;

create or replace function mysql.cast_text_to_int(text)
returns int4
as
$$
DECLARE
    tmp int4;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1));
        if (tmp is not null) then
            return tmp::int4;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (text as int4) with function mysql.cast_text_to_int as ASSIGNMENT;

create or replace function mysql.cast_text_to_bigint(text)
returns int8
as
$$
DECLARE
    tmp int8;
begin
    if $1 is not null then
        tmp = (mysql.convert_text_to_bigint_for_mysql($1));
        if (tmp is not null) then
            return tmp;
        else
            raise exception 'Data truncated';
        end if;
    else
        return null;
    end if;
end;
$$
language 'plpgsql';
create cast (text as int8) with function mysql.cast_text_to_bigint as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_smallint(pg_catalog.varbit)
RETURNS pg_catalog.int2
AS '$libdir/mysm', 'convertVarbitToSmallint'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_varbit_to_smallint(pg_catalog.varbit)
returns pg_catalog.int2
as
$$
declare
    tmp pg_catalog.int2;
begin
    tmp = mysql.convert_varbit_to_smallint($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int2) with function mysql.cast_varbit_to_smallint as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_int(pg_catalog.varbit)
RETURNS pg_catalog.int4
AS '$libdir/mysm', 'convertVarbitToInt'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_varbit_to_int(pg_catalog.varbit)
returns pg_catalog.int4
as
$$
declare
    tmp pg_catalog.int4;
begin
    tmp = mysql.convert_varbit_to_int($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int4) with function mysql.cast_varbit_to_int as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_bigint(pg_catalog.varbit)
RETURNS pg_catalog.int8
AS '$libdir/mysm', 'convertVarbitToBigint'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_varbit_to_bigint(pg_catalog.varbit)
returns pg_catalog.int8
as
$$
declare
    tmp pg_catalog.int8;
begin
    tmp = mysql.convert_varbit_to_bigint($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int8) with function mysql.cast_varbit_to_bigint as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_smallint_to_varbit(pg_catalog.int2)
RETURNS pg_catalog.varbit
AS '$libdir/mysm', 'convertSmallintToVarbit'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_smallint_to_varbit(pg_catalog.int2)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_smallint_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int2 as pg_catalog.varbit) with function mysql.cast_smallint_to_varbit as assignment;

CREATE OR REPLACE FUNCTION mysql.convert_int_to_varbit(pg_catalog.int4)
returns pg_catalog.varbit
AS '$libdir/mysm', 'convertIntToVarbit'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_int_to_varbit(pg_catalog.int4)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_int_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int4 as pg_catalog.varbit) with function mysql.cast_int_to_varbit as assignment;

CREATE OR REPLACE FUNCTION mysql.convert_bigint_to_varbit(pg_catalog.int8)
RETURNS pg_catalog.varbit
AS '$libdir/mysm', 'convertBigintToVarbit'
LANGUAGE C STRICT IMMUTABLE;

create or replace function mysql.cast_bigint_to_varbit(pg_catalog.int8)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_bigint_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int8 as pg_catalog.varbit) with function mysql.cast_bigint_to_varbit as assignment;

create or replace function mysql.cast_bool_to_varbit(pg_catalog.bool)
returns pg_catalog.varbit
as
$$
begin
    if $1 then
        return b'1';
    else
        return b'0';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.bool as pg_catalog.varbit) with function mysql.cast_bool_to_varbit as assignment;

create or replace function mysql.bytea2int8(pg_catalog.bytea)
returns pg_catalog.int8
as '$libdir/mysm', 'mys_bytea2int8'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.int8) with function mysql.bytea2int8(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2int4(pg_catalog.bytea)
returns pg_catalog.int4
as '$libdir/mysm', 'mys_bytea2int4'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.int4) with function mysql.bytea2int4(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2int2(pg_catalog.bytea)
returns pg_catalog.int2
as '$libdir/mysm', 'mys_bytea2int2'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.int2) with function mysql.bytea2int2(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2numeric(pg_catalog.bytea)
returns pg_catalog.numeric
as '$libdir/mysm', 'mys_bytea2numeric'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.numeric) with function mysql.bytea2numeric(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2float8(pg_catalog.bytea)
returns pg_catalog.float8
as '$libdir/mysm', 'mys_bytea2float8'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.float8) with function mysql.bytea2float8(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2float4(pg_catalog.bytea)
returns pg_catalog.float4
as '$libdir/mysm', 'mys_bytea2float4'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.float4) with function mysql.bytea2float4(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2text(pg_catalog.bytea)
returns pg_catalog.text
as '$libdir/mysm', 'mys_bytea2text'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.text) with function mysql.bytea2text(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2varchar(pg_catalog.bytea)
returns pg_catalog.varchar
as '$libdir/mysm', 'mys_bytea2varchar'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.varchar) with function mysql.bytea2varchar(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2bpchar(pg_catalog.bytea)
returns pg_catalog.bpchar
as '$libdir/mysm', 'mys_bytea2bpchar'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.bpchar) with function mysql.bytea2bpchar(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2date(pg_catalog.bytea)
returns pg_catalog.date
as '$libdir/mysm', 'mys_bytea2date'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.date) with function mysql.bytea2date(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2time(pg_catalog.bytea)
returns pg_catalog.time
as '$libdir/mysm', 'mys_bytea2time'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.time) with function mysql.bytea2time(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2timetz(pg_catalog.bytea)
returns pg_catalog.timetz
as '$libdir/mysm', 'mys_bytea2timetz'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.timetz) with function mysql.bytea2timetz(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2timestamp(pg_catalog.bytea)
returns pg_catalog.timestamp
as '$libdir/mysm', 'mys_bytea2timestamp'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.timestamp) with function mysql.bytea2timestamp(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2timestamptz(pg_catalog.bytea)
returns pg_catalog.timestamptz
as '$libdir/mysm', 'mys_bytea2timestamptz'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.timestamptz) with function mysql.bytea2timestamptz(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2bool(pg_catalog.bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'mys_bytea2bool'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.bool) with function mysql.bytea2bool(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2bit(pg_catalog.bytea)
returns pg_catalog.bit
as '$libdir/mysm', 'mys_bytea2bit'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.bit) with function mysql.bytea2bit(pg_catalog.bytea) as assignment;

create or replace function mysql.bytea2varbit(pg_catalog.bytea)
returns pg_catalog.varbit
as '$libdir/mysm', 'mys_bytea2varbit'
LANGUAGE C STRICT IMMUTABLE;
create cast(pg_catalog.bytea as pg_catalog.varbit) with function mysql.bytea2varbit(pg_catalog.bytea) as assignment;

create or replace function mysql.cast_smallint_to_boolean(int2)
returns pg_catalog.bool
as
$$
begin
    if $1 != 0 then
        return true;
    else
        return false;
    end if;
end;
$$
language 'plpgsql' strict;
create cast (int2 as pg_catalog.bool) with function mysql.cast_smallint_to_boolean as implicit;

create or replace function mysql.cast_bigint_to_boolean(int8)
returns pg_catalog.bool
as
$$
begin
    if $1 != 0 then
        return true;
    else
        return false;
    end if;
end;
$$
language 'plpgsql' strict;
create cast (int8 as pg_catalog.bool) with function mysql.cast_bigint_to_boolean as implicit;

create or replace function mysql.cast_bit_to_boolean(pg_catalog.bit)
returns pg_catalog.bool
as
$$
begin
    if pg_catalog.int8($1) != 0 then
        return true;
    else
        return false;
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.bit as pg_catalog.bool) with function mysql.cast_bit_to_boolean as implicit;

create or replace function mysql.cast_varbit_to_boolean(pg_catalog.varbit)
returns pg_catalog.bool
as
$$
begin
    if pg_catalog.int8($1) != 0 then
        return true;
    else
        return false;
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.bool) with function mysql.cast_varbit_to_boolean as implicit;

DROP FUNCTION IF EXISTS mysql.convert_varbit_to_smallint(pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_smallint(pg_catalog.varbit)
RETURNS pg_catalog.int2
AS '$libdir/mysm', 'convertVarbitToSmallint'
LANGUAGE C STRICT IMMUTABLE;

drop function if exists mysql.cast_varbit_to_smallint(pg_catalog.varbit) cascade;
create or replace function mysql.cast_varbit_to_smallint(pg_catalog.varbit)
returns pg_catalog.int2
as
$$
declare
tmp pg_catalog.int2;
begin
    tmp = mysql.convert_varbit_to_smallint($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int2) with function mysql.cast_varbit_to_smallint as ASSIGNMENT;

DROP FUNCTION IF EXISTS mysql.convert_varbit_to_int(pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_int(pg_catalog.varbit)
RETURNS pg_catalog.int4
AS '$libdir/mysm', 'convertVarbitToInt'
LANGUAGE C STRICT IMMUTABLE;

drop function if exists mysql.cast_varbit_to_int(pg_catalog.varbit) cascade;
create or replace function mysql.cast_varbit_to_int(pg_catalog.varbit)
returns pg_catalog.int4
as
$$
declare
tmp pg_catalog.int4;
begin
    tmp = mysql.convert_varbit_to_int($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int4) with function mysql.cast_varbit_to_int as ASSIGNMENT;

DROP FUNCTION IF EXISTS mysql.convert_varbit_to_bigint(pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_varbit_to_bigint(pg_catalog.varbit)
RETURNS pg_catalog.int8
AS '$libdir/mysm', 'convertVarbitToBigint'
LANGUAGE C STRICT IMMUTABLE;

DROP function if exists mysql.cast_varbit_to_bigint(pg_catalog.varbit) cascade;
create or replace function mysql.cast_varbit_to_bigint(pg_catalog.varbit)
returns pg_catalog.int8
as
$$
declare
tmp pg_catalog.int8;
begin
    tmp = mysql.convert_varbit_to_bigint($1);
    if tmp is not null then
        return tmp;
    else
        raise exception 'Out of range';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.varbit as pg_catalog.int8) with function mysql.cast_varbit_to_bigint as ASSIGNMENT;

DROP FUNCTION IF EXISTS mysql.convert_smallint_to_varbit(pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_smallint_to_varbit(pg_catalog.int2)
RETURNS pg_catalog.varbit
AS '$libdir/mysm', 'convertSmallintToVarbit'
LANGUAGE C STRICT IMMUTABLE;

drop function if exists mysql.cast_smallint_to_varbit(pg_catalog.int2) cascade;
create or replace function mysql.cast_smallint_to_varbit(pg_catalog.int2)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_smallint_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int2 as pg_catalog.varbit) with function mysql.cast_smallint_to_varbit as assignment;

DROP FUNCTION IF EXISTS mysql.convert_int_to_varbit(pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_int_to_varbit(pg_catalog.int4)
returns pg_catalog.varbit
AS '$libdir/mysm', 'convertIntToVarbit'
LANGUAGE C STRICT IMMUTABLE;

drop function if exists mysql.cast_int_to_varbit(pg_catalog.int4) cascade;
create or replace function mysql.cast_int_to_varbit(pg_catalog.int4)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_int_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int4 as pg_catalog.varbit) with function mysql.cast_int_to_varbit as assignment;

DROP FUNCTION IF EXISTS mysql.convert_bigint_to_varbit(pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.convert_bigint_to_varbit(pg_catalog.int8)
RETURNS pg_catalog.varbit
AS '$libdir/mysm', 'convertBigintToVarbit'
LANGUAGE C STRICT IMMUTABLE;

drop function if exists mysql.cast_bigint_to_varbit(pg_catalog.int8) cascade;
create or replace function mysql.cast_bigint_to_varbit(pg_catalog.int8)
returns pg_catalog.varbit
as
$$
begin
    return mysql.convert_bigint_to_varbit($1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.int8 as pg_catalog.varbit) with function mysql.cast_bigint_to_varbit as assignment;

drop function if exists mysql.cast_bool_to_varbit(pg_catalog.bool) cascade;
create or replace function mysql.cast_bool_to_varbit(pg_catalog.bool)
returns pg_catalog.varbit
as
$$
begin
    if $1 then
        return b'1';
    else
        return b'0';
    end if;
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.bool as pg_catalog.varbit) with function mysql.cast_bool_to_varbit as assignment;

drop function if exists mysql.cast_text_to_time(pg_catalog.text) cascade;
create or replace function mysql.cast_text_to_time(pg_catalog.text)
returns pg_catalog.time
as
$$
begin
    return pg_catalog.time_in($1::cstring, 0::Oid, -1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.text as pg_catalog.time) with function mysql.cast_text_to_time as ASSIGNMENT;

drop function if exists mysql.cast_text_to_date(pg_catalog.text) cascade;
create or replace function mysql.cast_text_to_date(pg_catalog.text)
returns pg_catalog.date
as
$$
begin
    return pg_catalog.date_in($1::cstring);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.text as pg_catalog.date) with function mysql.cast_text_to_date as ASSIGNMENT;

drop function if exists mysql.cast_text_to_timestamp(pg_catalog.text) cascade;
create or replace function mysql.cast_text_to_timestamp(pg_catalog.text)
returns pg_catalog.timestamp
as
$$
begin
    return pg_catalog.timestamp_in($1::cstring, 0::Oid, -1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.text as pg_catalog.timestamp) with function mysql.cast_text_to_timestamp as ASSIGNMENT;

drop function if exists mysql.cast_text_to_timestamptz(pg_catalog.text) cascade;
create or replace function mysql.cast_text_to_timestamptz(pg_catalog.text)
returns pg_catalog.timestamptz
as
$$
begin
    return pg_catalog.timestamptz_in($1::cstring, 0::Oid, -1);
end;
$$
language 'plpgsql' strict;
create cast (pg_catalog.text as pg_catalog.timestamptz) with function mysql.cast_text_to_timestamptz as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_text_to_float8(pg_catalog.text)
RETURNS pg_catalog.float8
AS '$libdir/mysm', 'mys_convertTextToFloat8'
LANGUAGE C STRICT IMMUTABLE;
create cast (pg_catalog.text as pg_catalog.float8) with function mysql.convert_text_to_float8 as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_varchar_to_float8(pg_catalog.varchar)
RETURNS pg_catalog.float8
AS
$$
BEGIN
    return mysql.convert_text_to_float8($1::text);
END;
$$
LANGUAGE plpgsql;
create cast (pg_catalog.varchar as pg_catalog.float8) with function mysql.convert_varchar_to_float8 as ASSIGNMENT;

CREATE OR REPLACE FUNCTION mysql.convert_char_to_float8(pg_catalog.char)
RETURNS pg_catalog.float8
AS
$$
BEGIN
    return mysql.convert_text_to_float8($1::text);
END;
$$
LANGUAGE plpgsql;
create cast (pg_catalog.char as pg_catalog.float8) with function mysql.convert_char_to_float8 as ASSIGNMENT;


CREATE TYPE mysql.enum_proc AS ENUM('FUNCTION', 'PROCEDURE');
CREATE TYPE mysql.enum_security AS ENUM('INVOKER', 'DEFINER');


-- 如果OS上的icu是50.*版，则需要使用下面这个方式
-- 如果OS上的icu是60.*版及以上，则使用下面两个方式都可以
CREATE COLLATION mysql.case_insensitive (provider = icu, locale = '@colStrength=secondary', deterministic = false);
CREATE COLLATION mysql.ignore_accents (provider = icu, locale = 'und-u-ks-level1-kc-true', deterministic = false);


GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mysql TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mys_informa_schema TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA sys TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mys_sys TO public;

