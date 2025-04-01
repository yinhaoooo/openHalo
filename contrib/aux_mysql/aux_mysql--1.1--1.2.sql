
CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 mysql.smallint, arg2 mysql.smallint)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 int, arg2 int)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 bigint, arg2 bigint)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 char, arg2 char)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 varchar, arg2 varchar)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 text, arg2 text)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 name, arg2 name)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 time, arg2 time)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 timestamp, arg2 timestamp)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION mysql.is_and_eq(arg1 date, arg2 date)
returns pg_catalog.bool
AS
$$
BEGIN
    if arg1 is null then
        if arg2 is null then
            return true;
        else
            return false;
        end if;
    else
        if arg2 is null then
            return false;
        else
            if arg1 = arg2 then
                return true;
            else
                return false;
            end if;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = mysql.smallint,
    RIGHTARG = mysql.smallint
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = int,
    RIGHTARG = int
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = bigint,
    RIGHTARG = bigint
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = name,
    RIGHTARG = name
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = time,
    RIGHTARG = time
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = timestamp,
    RIGHTARG = timestamp
);

CREATE OPERATOR mysql.<=> (
    FUNCTION = mysql.is_and_eq,
    LEFTARG = date,
    RIGHTARG = date
);

CREATE OR REPLACE FUNCTION mysql.bpcharlike(character, text)
returns pg_catalog.bool
AS '$libdir/mysm', 'bpcharlike'
LANGUAGE C STRICT IMMUTABLE;
CREATE OPERATOR mysql.~~ (
    FUNCTION = mysql.bpcharlike,
    LEFTARG = character,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bpcharnlike(character, text)
returns pg_catalog.bool
AS '$libdir/mysm', 'bpcharnlike'
LANGUAGE C STRICT IMMUTABLE;
CREATE OPERATOR mysql.!~~ (
    FUNCTION = mysql.bpcharnlike,
    LEFTARG = character,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.int2EqBool(mysql.smallint, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return true;
    elsif ($1 = 0) and ($2 = false) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int2EqBool,
    LEFTARG = mysql.smallint,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolEqInt2(pg_catalog.bool, mysql.smallint)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return true;
    elsif ($1 = false) and ($2 = 0) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolEqInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.int2NeBool(mysql.smallint, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return false;
    elsif ($1 = 0) and ($2 = false) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.int2NeBool,
    LEFTARG = mysql.smallint,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolNeInt2(pg_catalog.bool, mysql.smallint)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return false;
    elsif ($1 = false) and ($2 = 0) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.boolNeInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.int4EqBool(int, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return true;
    elsif ($1 = 0) and ($2 = false) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int4EqBool,
    LEFTARG = int,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolEqInt4(pg_catalog.bool, int)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return true;
    elsif ($1 = false) and ($2 = 0) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolEqInt4,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = int
);

CREATE OR REPLACE FUNCTION mysql.int4NeBool(int, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return false;
    elsif ($1 = 0) and ($2 = false) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.int4NeBool,
    LEFTARG = int,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolNeInt4(pg_catalog.bool, int)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return false;
    elsif ($1 = false) and ($2 = 0) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.boolNeInt4,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = int
);

CREATE OR REPLACE FUNCTION mysql.int8EqBool(bigint, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return true;
    elsif ($1 = 0) and ($2 = false) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int8EqBool,
    LEFTARG = bigint,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolEqInt8(pg_catalog.bool, bigint)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return true;
    elsif ($1 = false) and ($2 = 0) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolEqInt8,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.int8NeBool(bigint, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return false;
    elsif ($1 = 0) and ($2 = false) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.int8NeBool,
    LEFTARG = bigint,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolNeInt8(pg_catalog.bool, bigint)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return false;
    elsif ($1 = false) and ($2 = 0) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.boolNeInt8,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.convert_digit_text_to_text_for_mysql(text)
RETURNS text
AS '$libdir/mysm', 'convert_digit_text_to_text_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.convert_text_to_digit_text_for_mysql(text)
RETURNS text
AS '$libdir/mysm', 'convert_text_to_digit_text_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.smallintbgtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(mysql.smallint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.smallintbgtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerbgtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(integer, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.integerbgtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintbgtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(bigint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.bigintbgtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, mysql.smallint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textbginteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, integer);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbginteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textbgbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, bigint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);


CREATE OR REPLACE FUNCTION mysql.smallintbgeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(mysql.smallint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.smallintbgeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerbgeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(integer, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.integerbgeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintbgeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(bigint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.bigintbgeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, mysql.smallint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textbgeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, integer);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textbgeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, bigint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.smallintletext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(mysql.smallint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.smallintletext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerletext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(integer, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.integerletext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintletext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(bigint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.bigintletext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textlesmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, mysql.smallint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlesmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textleinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, integer);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textleinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textlebigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, bigint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlebigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.smallintleeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(mysql.smallint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.smallintleeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerleeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(integer, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.integerleeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintleeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(bigint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.bigintleeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, mysql.smallint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textleeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, integer);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textleeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, bigint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.convert_digit_text_to_text_for_mysql(text)
RETURNS text
AS '$libdir/mysm', 'convert_digit_text_to_text_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.convert_text_to_digit_text_for_mysql(text)
RETURNS text
AS '$libdir/mysm', 'convert_text_to_digit_text_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.smallintbgtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';

drop operator if exists mysql.>(mysql.smallint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.smallintbgtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerbgtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(integer, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.integerbgtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintbgtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(bigint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.bigintbgtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(text, mysql.smallint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textbginteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(text, integer);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbginteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textbgbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(text, bigint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.smallintbgeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(mysql.smallint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.smallintbgeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerbgeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(integer, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.integerbgeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintbgeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(bigint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.bigintbgeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, mysql.smallint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textbgeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, integer);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textbgeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, bigint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.smallintletext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(mysql.smallint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.smallintletext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerletext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(integer, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.integerletext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintletext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(bigint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.bigintletext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textlesmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, mysql.smallint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlesmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textleinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, integer);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textleinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textlebigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, bigint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlebigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.smallintleeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(mysql.smallint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.smallintleeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.integerleeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(integer, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.integerleeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigintleeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(bigint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.bigintleeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, mysql.smallint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.textleeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, integer);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.textleeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, bigint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.numericeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 = tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.numericeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.timeeqtext(time, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 = str_to_date($2, '%H:%i:%s');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.timeeqtext,
    LEFTARG = time,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.dateeqtext(date, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 = str_to_date($2, '%Y-%m-%d');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.dateeqtext,
    LEFTARG = date,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.timestampeqtext(timestamp, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 = str_to_date($2, '%Y-%m-%d %H:%i:%s');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.timestampeqtext,
    LEFTARG = timestamp,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.texteqtime(text, time)
returns pg_catalog.bool
AS
$$
BEGIN
    return str_to_date($1, '%H:%i:%s') = $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.texteqtime,
    LEFTARG = text,
    RIGHTARG = time
);

CREATE OR REPLACE FUNCTION mysql.texteqdate(text, date)
returns pg_catalog.bool
AS
$$
BEGIN
    return (str_to_date($1, '%Y-%m-%d')) = $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.texteqdate,
    LEFTARG = text,
    RIGHTARG = date
);

CREATE OR REPLACE FUNCTION mysql.texteqtimestamp(text, timestamp)
returns pg_catalog.bool
AS
$$
BEGIN
    return str_to_date($1, '%Y-%m-%d %H:%i:%s') = $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.texteqtimestamp,
    LEFTARG = text,
    RIGHTARG = timestamp
);

CREATE OR REPLACE FUNCTION mysql.textne(text, text)
returns pg_catalog.bool
AS '$libdir/mysm', 'textne_mys'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.numericnetext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 != tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.numericnetext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.timenetext(time, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 != str_to_date($2, '%H:%i:%s');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.timenetext,
    LEFTARG = time,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.datenetext(date, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 != str_to_date($2, '%Y-%m-%d');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.datenetext,
    LEFTARG = date,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.timestampnetext(timestamp, text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 != str_to_date($2, '%Y-%m-%d %H:%i:%s');
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.timestampnetext,
    LEFTARG = timestamp,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textnenumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 != tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.textnenumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textlenumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, numeric);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlenumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textleeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, numeric);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(text, numeric);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, numeric);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.numericletext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(numeric, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.numericletext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericleeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(numeric, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.numericleeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
drop operator if exists mysql.>(numeric, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.numericbgtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(numeric, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.numericbgeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textnetime(text, time)
returns pg_catalog.bool
AS
$$
BEGIN
    return str_to_date($1, '%H:%i:%s') != $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.textnetime,
    LEFTARG = text,
    RIGHTARG = time
);

CREATE OR REPLACE FUNCTION mysql.textnedate(text, date)
returns pg_catalog.bool
AS
$$
BEGIN
    return str_to_date($1, '%Y-%m-%d') != $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.textnedate,
    LEFTARG = text,
    RIGHTARG = date
);

CREATE OR REPLACE FUNCTION mysql.textnetimestamp(text, timestamp)
returns pg_catalog.bool
AS
$$
BEGIN
    return str_to_date($1, '%Y-%m-%d %H:%i:%s') != $2;
END
$$
language 'plpgsql';
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.textnetimestamp,
    LEFTARG = text,
    RIGHTARG = timestamp
);

CREATE OR REPLACE FUNCTION mysql.char_eq_char_for_date_format(character, character)
returns pg_catalog.bool
AS '$libdir/mysm', 'char_eq_char_for_date_format'
LANGUAGE C STRICT IMMUTABLE;
CREATE OPERATOR mysql.=# (
    FUNCTION = mysql.char_eq_char_for_date_format,
    LEFTARG = character,
    RIGHTARG = character
);

CREATE OR REPLACE FUNCTION mysql.text_pl_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1::timestamp(6) + $2;
end;
$function$;
DROP OPERATOR IF EXISTS mysql.+(text, pg_catalog.interval);
CREATE OPERATOR mysql.+(
    function = mysql.text_pl_interval,
    leftarg = text,
    rightarg = pg_catalog.interval
);

CREATE OR REPLACE FUNCTION mysql.interval_pl_text(pg_catalog.interval, text)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1 + $2::timestamp(6);
end;
$function$;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.interval, pg_catalog.text);
CREATE OPERATOR mysql.+(
    function = mysql.interval_pl_text,
    leftarg = pg_catalog.interval,
    rightarg = text
);

CREATE OR REPLACE FUNCTION mysql.text_mi_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1::timestamp(6) - $2;
end;
$function$;
DROP OPERATOR IF EXISTS mysql.-(pg_catalog.text, pg_catalog.interval);
CREATE OPERATOR mysql.-(
    function = mysql.text_mi_interval,
    leftarg = text,
    rightarg = pg_catalog.interval
);

CREATE OR REPLACE FUNCTION mysql.texteqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 = tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(text, numeric);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.texteqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textlenumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, numeric);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlenumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textleeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, numeric);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, numeric);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1);
        tmp2 = mysql.convert_digit_text_to_text_for_mysql($2::text);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, numeric);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.numericletext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(numeric, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.numericletext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericleeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(numeric, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.numericleeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(numeric, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.numericbgtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 text;
    tmp2 text;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_digit_text_to_text_for_mysql($1::text);
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2);
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(numeric, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.numericbgeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

-- 为了date_format自定义函数中，比较时一定会区分大小写
CREATE OR REPLACE FUNCTION mysql.char_eq_char_for_date_format(character, character)
returns pg_catalog.bool
AS '$libdir/mysm', 'char_eq_char_for_date_format'
LANGUAGE C STRICT IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.=#(character, character);
CREATE OPERATOR mysql.=# (
    FUNCTION = mysql.char_eq_char_for_date_format,
    LEFTARG = character,
    RIGHTARG = character
);

CREATE OR REPLACE FUNCTION mysql.text_add_text(text, text)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;

    if ((tmp1 = null) or (tmp2 = null)) then
        return null;
    end if;

    return tmp1 + tmp2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.text, pg_catalog.text);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.text_add_text,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.char_add_char(char, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_add_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(char, char);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.char_add_char,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.varchar_add_varchar(varchar, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_add_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(varchar, varchar);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.varchar_add_varchar,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.text_add_bigint(text, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 + $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(text, bigint);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.text_add_bigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.char_add_bigint(char, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_add_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(char, bigint);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.char_add_bigint,
    LEFTARG = char,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.varchar_add_bigint(varchar, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_add_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(varchar, bigint);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.varchar_add_bigint,
    LEFTARG = varchar,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigint_add_text(bigint, text)
RETURNS bigint
AS
$$
DECLARE
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
    if (tmp2 = null) then
        return null;
    end if;

    return $1 + tmp2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(bigint, text);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.bigint_add_text,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigint_add_varchar(bigint, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_add_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(bigint, varchar);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.bigint_add_varchar,
    LEFTARG = bigint,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.bigint_add_char(bigint, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_add_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(bigint, char);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.bigint_add_char,
    LEFTARG = bigint,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.text_minus_text(text, text)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;

    if ((tmp1 = null) or (tmp2 = null)) then
        return null;
    end if;

    return tmp1 - tmp2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(text, text);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.text_minus_text,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.char_minus_char(char, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_minus_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(char, char);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.char_minus_char,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.varchar_minus_varchar(varchar, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_minus_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(varchar, varchar);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.varchar_minus_varchar,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.text_minus_bigint(text, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 - $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(text, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.text_minus_bigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.char_minus_bigint(char, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_minus_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(char, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.char_minus_bigint,
    LEFTARG = char,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.varchar_minus_bigint(varchar, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_minus_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(varchar, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.varchar_minus_bigint,
    LEFTARG = varchar,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigint_minus_text(bigint, text)
RETURNS bigint
AS
$$
DECLARE
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
    if (tmp2 = null) then
        return null;
    end if;

    return $1 - tmp2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(bigint, text);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.bigint_minus_text,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigint_minus_varchar(bigint, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_minus_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(bigint, varchar);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.bigint_minus_varchar,
    LEFTARG = bigint,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.bigint_minus_char(bigint, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_minus_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(bigint, char);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.bigint_minus_char,
    LEFTARG = bigint,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.text_multi_text(text, text)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;

    if ((tmp1 = null) or (tmp2 = null)) then
        return null;
    end if;

    return tmp1 * tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.text_multi_text,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.char_multi_char(char, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_multi_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.char_multi_char,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.varchar_multi_varchar(varchar, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_multi_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.varchar_multi_varchar,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.text_multi_bigint(text, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 * $2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.text_multi_bigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.char_multi_bigint(char, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_multi_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.char_multi_bigint,
    LEFTARG = char,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.varchar_multi_bigint(varchar, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_multi_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.varchar_multi_bigint,
    LEFTARG = varchar,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigint_multi_text(bigint, text)
RETURNS bigint
AS
$$
DECLARE
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
    if (tmp2 = null) then
        return null;
    end if;

    return $1 * tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.bigint_multi_text,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigint_multi_varchar(bigint, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_multi_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.bigint_multi_varchar,
    LEFTARG = bigint,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.bigint_multi_char(bigint, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_multi_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.* (
    FUNCTION = mysql.bigint_multi_char,
    LEFTARG = bigint,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.text_divide_text(text, text)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;

    if ((tmp1 = null) or (tmp2 = null) or (tmp2 = 0)) then
        return null;
    end if;

    return tmp1 / tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.text_divide_text,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.char_divide_char(char, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_divide_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.char_divide_char,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.varchar_divide_varchar(varchar, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_divide_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.varchar_divide_varchar,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.text_divide_bigint(text, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null) or ($2 = 0)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 / $2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.text_divide_bigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.char_divide_bigint(char, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_divide_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.char_divide_bigint,
    LEFTARG = char,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.varchar_divide_bigint(varchar, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_divide_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.varchar_divide_bigint,
    LEFTARG = varchar,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigint_divide_text(bigint, text)
RETURNS bigint
AS
$$
DECLARE
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
    if ((tmp2 = null) or (tmp2 = 0)) then
        return null;
    end if;

    return $1 / tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.bigint_divide_text,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigint_divide_varchar(bigint, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_divide_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.bigint_divide_varchar,
    LEFTARG = bigint,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.bigint_divide_char(bigint, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_divide_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql./ (
    FUNCTION = mysql.bigint_divide_char,
    LEFTARG = bigint,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.text_mod_text(text, text)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;

    if ((tmp1 = null) or (tmp2 = null) or (tmp2 = 0)) then
        return null;
    end if;

    return tmp1 % tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.text_mod_text,
    LEFTARG = text,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.char_mod_char(char, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_mod_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.char_mod_char,
    LEFTARG = char,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.varchar_mod_varchar(varchar, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_mod_text($1::text, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.varchar_mod_varchar,
    LEFTARG = varchar,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.text_mod_bigint(text, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null) or ($2 = 0)) then
        return null;
    end if;

    tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 % $2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.text_mod_bigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.char_mod_bigint(char, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_mod_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.char_mod_bigint,
    LEFTARG = char,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.varchar_mod_bigint(varchar, bigint)
RETURNS bigint
AS
$$
BEGIN
    return mysql.text_mod_bigint($1::text, $2);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.varchar_mod_bigint,
    LEFTARG = varchar,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigint_mod_text(bigint, text)
RETURNS bigint
AS
$$
DECLARE
    tmp2 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
    if ((tmp2 = null) or (tmp2 = 0)) then
        return null;
    end if;

    return $1 % tmp2;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.bigint_mod_text,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.bigint_mod_varchar(bigint, varchar)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_mod_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.bigint_mod_varchar,
    LEFTARG = bigint,
    RIGHTARG = varchar
);

CREATE OR REPLACE FUNCTION mysql.bigint_mod_char(bigint, char)
RETURNS bigint
AS
$$
BEGIN
    return mysql.bigint_mod_text($1, $2::text);
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.% (
    FUNCTION = mysql.bigint_mod_char,
    LEFTARG = bigint,
    RIGHTARG = char
);

CREATE OR REPLACE FUNCTION mysql.convert_time_text_to_numeric_for_mysql(text)
RETURNS float8
AS '$libdir/mysm', 'convert_time_text_to_numeric_for_mysql'
LANGUAGE C STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION mysql.time_add_bigint(time, bigint)
RETURNS float8
AS
$$
DECLARE
    tmp1 float8;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text);

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 + $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(time, bigit);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.time_add_bigint,
    LEFTARG = time,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.date_add_smallint(pg_catalog.date, pg_catalog.int2)
RETURNS int
AS
$$
BEGIN
    return mysql.date_add_int($1, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.date, pg_catalog.int2);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.date_add_smallint,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.date_add_int(date, int)
RETURNS int
AS
$$
DECLARE
    tmp1 int;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text)::int;

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 + $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(date, int);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.date_add_int,
    LEFTARG = date,
    RIGHTARG = int
);

CREATE OR REPLACE FUNCTION mysql.date_add_bigint(date, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text)::bigint;

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 + $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(date, bigint);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.date_add_bigint,
    LEFTARG = date,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.timestamp_add_bigint(timestamp, bigint)
RETURNS float8
AS
$$
DECLARE
    tmp1 float8;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text);

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 + $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.+(timestamp, bigint);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql.timestamp_add_bigint,
    LEFTARG = timestamp,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.time_minus_bigint(time, bigint)
RETURNS float8
AS
$$
DECLARE
    tmp1 float8;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text);

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 - $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(time, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.time_minus_bigint,
    LEFTARG = time,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.date_minus_smallint(pg_catalog.date, pg_catalog.int2)
RETURNS pg_catalog.int4
AS
$$
BEGIN
    return mysql.date_minus_int($1, $2);
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(pg_catalog.date, pg_catalog.int2);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.date_minus_smallint,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.date_minus_int(date, int)
RETURNS int
AS
$$
DECLARE
    tmp1 int;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text)::int;

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 - $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(date, int);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.date_minus_int,
    LEFTARG = date,
    RIGHTARG = int
);

CREATE OR REPLACE FUNCTION mysql.date_minus_bigint(date, bigint)
RETURNS bigint
AS
$$
DECLARE
    tmp1 bigint;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text)::bigint;

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 - $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(date, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.date_minus_bigint,
    LEFTARG = date,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.timestamp_minus_bigint(timestamp, bigint)
RETURNS float8
AS
$$
DECLARE
    tmp1 float8;
BEGIN
    if (($1 = null) or ($2 = null)) then
        return null;
    end if;

    tmp1 = mysql.convert_time_text_to_numeric_for_mysql($1::text);

    if (tmp1 = null) then
        return null;
    end if;

    return tmp1 - $2;
END;
$$
LANGUAGE plpgsql;
DROP OPERATOR IF EXISTS mysql.-(timestamp, bigint);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql.timestamp_minus_bigint,
    LEFTARG = timestamp,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.text_pl_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1::timestamp(6) + $2;
end;
$function$;
DROP OPERATOR IF EXISTS mysql.+(text, pg_catalog.interval);
CREATE OPERATOR mysql.+(
    function = mysql.text_pl_interval,
    leftarg = text,
    rightarg = pg_catalog.interval
);

CREATE OR REPLACE FUNCTION mysql.interval_pl_text(pg_catalog.interval, text)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1 + $2::timestamp(6);
end;
$function$;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.interval, text);
CREATE OPERATOR mysql.+(
    function = mysql.interval_pl_text,
    leftarg = pg_catalog.interval,
    rightarg = text
);

CREATE OR REPLACE FUNCTION mysql.text_mi_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
LANGUAGE plpgsql
AS $function$
begin
    return $1::timestamp(6) - $2;
end;
$function$;
DROP OPERATOR IF EXISTS mysql.-(text, pg_catalog.interval);
CREATE OPERATOR mysql.-(
    function = mysql.text_mi_interval,
    leftarg = text,
    rightarg = pg_catalog.interval
);

CREATE OR REPLACE FUNCTION mysql.json_object_field(from_json json, field_name text)
RETURNS json
LANGUAGE sql
IMMUTABLE PARALLEL SAFE STRICT
AS $function$
    select pg_catalog.json_object_field($1, ltrim($2, '$.'));
$function$;
DROP OPERATOR IF EXISTS mysql.->(json, text);
CREATE OPERATOR mysql.-> (
    FUNCTION = mysql.json_object_field,
    LEFTARG = json,
    RIGHTARG = text
);

-- operators for text and numeric
CREATE OR REPLACE FUNCTION mysql.texteqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::numeric;
        tmp2 = $2;
        if tmp1 = tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(text, numeric);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.texteqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textlenumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::numeric;
        tmp2 = $2;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, numeric);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlenumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textleeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::numeric;
        tmp2 = $2;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, numeric);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::numeric;
        tmp2 = $2;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, numeric);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.textbgeqnumeric(text, numeric)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::numeric;
        tmp2 = $2;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, numeric);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqnumeric,
    LEFTARG = text,
    RIGHTARG = numeric
);

CREATE OR REPLACE FUNCTION mysql.numericletext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::numeric;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(numeric, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.numericletext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericleeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::numeric;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(numeric, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.numericleeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::numeric;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(numeric, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.numericbgtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.numericbgeqtext(numeric, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 numeric;
    tmp2 numeric;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::numeric;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(numeric, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.numericbgeqtext,
    LEFTARG = numeric,
    RIGHTARG = text
);

-- operators for text and smallint
CREATE OR REPLACE FUNCTION mysql.smallintbgtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::mysql.smallint;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(mysql.smallint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.smallintbgtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::mysql.smallint;
        tmp2 = $2;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, mysql.smallint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.smallintbgeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::mysql.smallint;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(mysql.smallint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.smallintbgeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::mysql.smallint;
        tmp2 = $2;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, mysql.smallint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.smallintletext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::mysql.smallint;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(mysql.smallint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.smallintletext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textlesmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::mysql.smallint;
        tmp2 = $2;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, mysql.smallint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlesmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

CREATE OR REPLACE FUNCTION mysql.smallintleeqtext(mysql.smallint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::mysql.smallint;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(mysql.smallint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.smallintleeqtext,
    LEFTARG = mysql.smallint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleeqsmallint(text, mysql.smallint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 mysql.smallint;
    tmp2 mysql.smallint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::mysql.smallint;
        tmp2 = $2;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, mysql.smallint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqsmallint,
    LEFTARG = text,
    RIGHTARG = mysql.smallint
);

-- operators for text and integer
CREATE OR REPLACE FUNCTION mysql.integerbgtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::integer;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(integer, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.integerbgtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbginteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::integer;
        tmp2 = $2;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, integer);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbginteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.integerbgeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::integer;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(integer, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.integerbgeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::integer;
        tmp2 = $2;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, integer);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.integerletext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::integer;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(integer, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.integerletext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::integer;
        tmp2 = $2;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, integer);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textleinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

CREATE OR REPLACE FUNCTION mysql.integerleeqtext(integer, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::integer;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(integer, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.integerleeqtext,
    LEFTARG = integer,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleeqinteger(text, integer)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 integer;
    tmp2 integer;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::integer;
        tmp2 = $2;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, integer);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqinteger,
    LEFTARG = text,
    RIGHTARG = integer
);

-- operators for text and bigint
CREATE OR REPLACE FUNCTION mysql.bigintbgtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(bigint, text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.bigintbgtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
        tmp2 = $2;
        if tmp1 > tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(text, bigint);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.textbgbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigintbgeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(bigint, text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.bigintbgeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textbgeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
        tmp2 = $2;
        if tmp1 >= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(text, bigint);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.textbgeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigintletext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(bigint, text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.bigintletext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textlebigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
        tmp2 = $2;
        if tmp1 < tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(text, bigint);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.textlebigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

CREATE OR REPLACE FUNCTION mysql.bigintleeqtext(bigint, text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = $1;
        tmp2 = mysql.convert_text_to_digit_text_for_mysql($2)::bigint;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(bigint, text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.bigintleeqtext,
    LEFTARG = bigint,
    RIGHTARG = text
);

CREATE OR REPLACE FUNCTION mysql.textleeqbigint(text, bigint)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 bigint;
    tmp2 bigint;
BEGIN
    if $1 is not null and $2 is not null then
        tmp1 = mysql.convert_text_to_digit_text_for_mysql($1)::bigint;
        tmp2 = $2;
        if tmp1 <= tmp2 then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(text, bigint);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.textleeqbigint,
    LEFTARG = text,
    RIGHTARG = bigint
);

create or replace function mysql.byteaCondAndBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaCondAndByteaCImp'
language c immutable strict;
drop operator if exists mysql.&&(bytea, bytea);
create operator mysql.&& (
    function = mysql.byteaCondAndBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaCondOrBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaCondOrByteaCImp'
language c immutable;
drop operator if exists mysql.||(bytea, bytea);
create operator mysql.|| (
    function = mysql.byteaCondOrBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaGtBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaGtByteaCImp'
language c immutable strict;
drop operator if exists mysql.>(bytea, bytea);
create operator mysql.> (
    function = mysql.byteaGtBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaGeBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaGeByteaCImp'
language c immutable strict;
drop operator if exists mysql.>=(bytea, bytea);
create operator mysql.>= (
    function = mysql.byteaGeBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaLtBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaLtByteaCImp'
language c immutable strict;
drop operator if exists mysql.<(bytea, bytea);
create operator mysql.< (
    function = mysql.byteaLtBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaLeBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaLeByteaCImp'
language c immutable strict;
drop operator if exists mysql.<=(bytea, bytea);
create operator mysql.<= (
    function = mysql.byteaLeBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaRegexpBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaRegexpByteaCImp'
language c immutable strict;
drop operator if exists mysql.~*(bytea, bytea);
create operator mysql.~* (
    function = mysql.byteaRegexpBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaNregexpBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNregexpByteaCImp'
language c immutable strict;
drop operator if exists mysql.!~*(bytea, bytea);
create operator mysql.!~* (
    function = mysql.byteaNregexpBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaRegexpbinBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaRegexpbinByteaCImp'
language c immutable strict;
drop operator if exists mysql.~(bytea, bytea);
create operator mysql.~ (
    function = mysql.byteaRegexpbinBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaNregexpbinBytea(bytea, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNregexpbinByteaCImp'
language c immutable strict;
drop operator if exists mysql.!~(bytea, bytea);
create operator mysql.!~ (
    function = mysql.byteaNregexpbinBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaPlBytea(bytea, bytea)
returns numeric
as '$libdir/mysm', 'byteaPlByteaCImp'
language c immutable strict;
DROP OPERATOR IF EXISTS mysql.+(bytea, bytea);
create operator mysql.+ (
    function = mysql.byteaPlBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaMiBytea(bytea, bytea)
returns numeric
as '$libdir/mysm', 'byteaMiByteaCImp'
language c immutable strict;
drop operator if exists mysql.-(bytea, bytea);
create operator mysql.- (
    function = mysql.byteaMiBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaMulBytea(bytea, bytea)
returns numeric
as '$libdir/mysm', 'byteaMulByteaCImp'
language c immutable strict;
drop operator if exists mysql.*(bytea, bytea);
create operator mysql.* (
    function = mysql.byteaMulBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaDivBytea(bytea, bytea)
returns numeric
as '$libdir/mysm', 'byteaDivByteaCImp'
language c immutable strict;
drop operator if exists mysql./(bytea, bytea);
create operator mysql./ (
    function = mysql.byteaDivBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaModBytea(bytea, bytea)
returns numeric
as '$libdir/mysm', 'byteaModByteaCImp'
language c immutable strict;
drop operator if exists mysql.%(bytea, bytea);
create operator mysql.% (
    function = mysql.byteaModBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaAndBytea(bytea, bytea)
returns pg_catalog.int8
as '$libdir/mysm', 'byteaAndByteaCImp'
language c immutable strict;
drop operator if exists mysql.&(bytea, bytea);
create operator mysql.& (
    function = mysql.byteaAndBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.byteaOrBytea(bytea, bytea)
returns pg_catalog.int8
as '$libdir/mysm', 'byteaOrByteaCImp'
language c immutable strict;
drop operator if exists mysql.|(bytea, bytea);
create operator mysql.| (
    function = mysql.byteaOrBytea,
    leftarg = bytea,
    rightarg = bytea
);

create or replace function mysql.anycompatibleEqBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleEqByteaCImp'
language c immutable strict;
drop operator if exists mysql.=(pg_catalog.anycompatible, bytea);
create operator mysql.= (
    function = mysql.anycompatibleEqBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaEqAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaEqAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.=(bytea, pg_catalog.anycompatible);
create operator mysql.= (
    function = mysql.byteaEqAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleNeBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleNeByteaCImp'
language c immutable strict;
drop operator if exists mysql.!=(pg_catalog.anycompatible, bytea);
create operator mysql.!= (
    function = mysql.anycompatibleNeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);
drop operator if exists mysql.<>(pg_catalog.anycompatible, bytea);
create operator mysql.<> (
    function = mysql.anycompatibleNeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaNeAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNeAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.!=(bytea, pg_catalog.anycompatible);
create operator mysql.!= (
    function = mysql.byteaNeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);
drop operator if exists mysql.<>(bytea, pg_catalog.anycompatible);
create operator mysql.<> (
    function = mysql.byteaNeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleGtBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleGtByteaCImp'
language c immutable strict;
drop operator if exists mysql.>(pg_catalog.anycompatible, bytea);
create operator mysql.> (
    function = mysql.anycompatibleGtBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaGtAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaGtAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.>(bytea, pg_catalog.anycompatible);
create operator mysql.> (
    function = mysql.byteaGtAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleGeBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleGeByteaCImp'
language c immutable strict;
drop operator if exists mysql.>=(pg_catalog.anycompatible, bytea);
create operator mysql.>= (
    function = mysql.anycompatibleGeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaGeAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaGeAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.>=(bytea, pg_catalog.anycompatible);
create operator mysql.>= (
    function = mysql.byteaGeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleLtBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleLtByteaCImp'
language c immutable strict;
drop operator if exists mysql.<(pg_catalog.anycompatible, bytea);
create operator mysql.< (
    function = mysql.anycompatibleLtBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaLtAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaLtAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.<(bytea, pg_catalog.anycompatible);
create operator mysql.< (
    function = mysql.byteaLtAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleLeBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleLeByteaCImp'
language c immutable strict;
drop operator if exists mysql.<=(pg_catalog.anycompatible, bytea);
create operator mysql.<= (
    function = mysql.anycompatibleLeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaLeAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaLeAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.<=(bytea, pg_catalog.anycompatible);
create operator mysql.<= (
    function = mysql.byteaLeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleCondAndBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleCondAndByteaCImp'
language c immutable strict;
drop operator if exists mysql.&&(pg_catalog.anycompatible, bytea);
create operator mysql.&& (
    function = mysql.anycompatibleCondAndBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaCondAndAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaCondAndAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.&&(bytea, pg_catalog.anycompatible);
create operator mysql.&& (
    function = mysql.byteaCondAndAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleCondOrBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleCondOrByteaCImp'
language c immutable;
drop operator if exists mysql.||(pg_catalog.anycompatible, bytea);
create operator mysql.|| (
    function = mysql.anycompatibleCondOrBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaCondOrAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaCondOrAnycompatibleCImp'
language c immutable;
drop operator if exists mysql.||(bytea, pg_catalog.anycompatible);
create operator mysql.|| (
    function = mysql.byteaCondOrAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleLikeBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleLikeByteaCImp'
language c immutable strict;
drop operator if exists mysql.~~(pg_catalog.anycompatible, bytea);
create operator mysql.~~ (
    function = mysql.anycompatibleLikeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaLikeAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaLikeAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.~~(bytea, pg_catalog.anycompatible);
create operator mysql.~~ (
    function = mysql.byteaLikeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleNlikeBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleNlikeByteaCImp'
language c immutable strict;
drop operator if exists mysql.!~~(pg_catalog.anycompatible, bytea);
create operator mysql.!~~ (
    function = mysql.anycompatibleNlikeBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaNlikeAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNlikeAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.!~~(bytea, pg_catalog.anycompatible);
create operator mysql.!~~ (
    function = mysql.byteaNlikeAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleRegexpBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleRegexpByteaCImp'
language c immutable strict;
drop operator if exists mysql.~*(pg_catalog.anycompatible, bytea);
create operator mysql.~* (
    function = mysql.anycompatibleRegexpBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaRegexpAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaRegexpAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.~*(bytea, pg_catalog.anycompatible);
create operator mysql.~* (
    function = mysql.byteaRegexpAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleNregexpBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleNregexpByteaCImp'
language c immutable strict;
drop operator if exists mysql.!~*(pg_catalog.anycompatible, bytea);
create operator mysql.!~* (
    function = mysql.anycompatibleNregexpBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaNregexpAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNregexpAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.!~*(bytea, pg_catalog.anycompatible);
create operator mysql.!~* (
    function = mysql.byteaNregexpAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleRegexpbinBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleRegexpbinByteaCImp'
language c immutable strict;
drop operator if exists mysql.~(pg_catalog.anycompatible, bytea);
create operator mysql.~ (
    function = mysql.anycompatibleRegexpbinBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaRegexpbinAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaRegexpbinAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.~(bytea, pg_catalog.anycompatible);
create operator mysql.~ (
    function = mysql.byteaRegexpbinAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleNregexpbinBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.bool
as '$libdir/mysm', 'anycompatibleNregexpbinByteaCImp'
language c immutable strict;
drop operator if exists mysql.!~(pg_catalog.anycompatible, bytea);
create operator mysql.!~ (
    function = mysql.anycompatibleNregexpbinBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaNregexpbinAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.bool
as '$libdir/mysm', 'byteaNregexpbinAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.!~(bytea, pg_catalog.anycompatible);
create operator mysql.!~ (
    function = mysql.byteaNregexpbinAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatiblePlBytea(pg_catalog.anycompatible, bytea)
returns numeric
as '$libdir/mysm', 'anycompatiblePlByteaCImp'
language c immutable strict;
drop operator if exists mysql.+(pg_catalog.anycompatible, bytea);
create operator mysql.+ (
    function = mysql.anycompatiblePlBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaPlAnycompatible(bytea, pg_catalog.anycompatible)
returns numeric
as '$libdir/mysm', 'byteaPlAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.+(bytea, pg_catalog.anycompatible);
create operator mysql.+ (
    function = mysql.byteaPlAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleMiBytea(pg_catalog.anycompatible, bytea)
returns numeric
as '$libdir/mysm', 'anycompatibleMiByteaCImp'
language c immutable strict;
drop operator if exists mysql.-(pg_catalog.anycompatible, bytea);
create operator mysql.- (
    function = mysql.anycompatibleMiBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaMiAnycompatible(bytea, pg_catalog.anycompatible)
returns numeric
as '$libdir/mysm', 'byteaMiAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.-(bytea, pg_catalog.anycompatible);
create operator mysql.- (
    function = mysql.byteaMiAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleMulBytea(pg_catalog.anycompatible, bytea)
returns numeric
as '$libdir/mysm', 'anycompatibleMulByteaCImp'
language c immutable strict;
drop operator if exists mysql.*(pg_catalog.anycompatible, bytea);
create operator mysql.* (
    function = mysql.anycompatibleMulBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaMulAnycompatible(bytea, pg_catalog.anycompatible)
returns numeric
as '$libdir/mysm', 'byteaMulAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.*(bytea, pg_catalog.anycompatible);
create operator mysql.* (
    function = mysql.byteaMulAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleDivBytea(pg_catalog.anycompatible, bytea)
returns numeric
as '$libdir/mysm', 'anycompatibleDivByteaCImp'
language c immutable strict;
drop operator if exists mysql./(pg_catalog.anycompatible, bytea);
create operator mysql./ (
    function = mysql.anycompatibleDivBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaDivAnycompatible(bytea, pg_catalog.anycompatible)
returns numeric
as '$libdir/mysm', 'byteaDivAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql./(bytea, pg_catalog.anycompatible);
create operator mysql./ (
    function = mysql.byteaDivAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleModBytea(pg_catalog.anycompatible, bytea)
returns numeric
as '$libdir/mysm', 'anycompatibleModByteaCImp'
language c immutable strict;
drop operator if exists mysql.%(pg_catalog.anycompatible, bytea);
create operator mysql.% (
    function = mysql.anycompatibleModBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaModAnycompatible(bytea, pg_catalog.anycompatible)
returns numeric
as '$libdir/mysm', 'byteaModAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.%(bytea, pg_catalog.anycompatible);
create operator mysql.% (
    function = mysql.byteaModAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleAndBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.int8
as '$libdir/mysm', 'anycompatibleAndByteaCImp'
language c immutable strict;
drop operator if exists mysql.&(pg_catalog.anycompatible, bytea);
create operator mysql.& (
    function = mysql.anycompatibleAndBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaAndAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.int8
as '$libdir/mysm', 'byteaAndAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.&(bytea, pg_catalog.anycompatible);
create operator mysql.& (
    function = mysql.byteaAndAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

create or replace function mysql.anycompatibleOrBytea(pg_catalog.anycompatible, bytea)
returns pg_catalog.int8
as '$libdir/mysm', 'anycompatibleOrByteaCImp'
language c immutable strict;
drop operator if exists mysql.|(pg_catalog.anycompatible, bytea);
create operator mysql.| (
    function = mysql.anycompatibleOrBytea,
    leftarg = pg_catalog.anycompatible,
    rightarg = bytea
);

create or replace function mysql.byteaOrAnycompatible(bytea, pg_catalog.anycompatible)
returns pg_catalog.int8
as '$libdir/mysm', 'byteaOrAnycompatibleCImp'
language c immutable strict;
drop operator if exists mysql.|(bytea, pg_catalog.anycompatible);
create operator mysql.| (
    function = mysql.byteaOrAnycompatible,
    leftarg = bytea,
    rightarg = pg_catalog.anycompatible
);

CREATE OR REPLACE FUNCTION mysql.bit_eq_boolean(pg_catalog.bit, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    if $2 = true THEN
        if pg_catalog.int8($1) = 1 then
            return true;
        else
            return false;
        end if;
    else
        if pg_catalog.int8($1) = 0 then
            return true;
        else
            return false;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.bit_eq_boolean,
    LEFTARG = pg_catalog.bit,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolean_eq_bit(pg_catalog.bool, pg_catalog.bit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1 = true THEN
        if pg_catalog.int8($2) = 1 then
            return true;
        else
            return false;
        end if;
    else
        if pg_catalog.int8($2) = 0 then
            return true;
        else
            return false;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolean_eq_bit,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.bit
);

CREATE OR REPLACE FUNCTION mysql.varbit_eq_boolean(pg_catalog.varbit, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    if $2 = true THEN
        if pg_catalog.int8($1) = 1 then
            return true;
        else
            return false;
        end if;
    else
        if pg_catalog.int8($1) = 0 then
            return true;
        else
            return false;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_boolean,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.boolean_eq_varbit(pg_catalog.bool, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1 = true THEN
        if pg_catalog.int8($2) = 1 then
            return true;
        else
            return false;
        end if;
    else
        if pg_catalog.int8($2) = 0 then
            return true;
        else
            return false;
        end if;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolean_eq_varbit,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.varbit
);

CREATE OR REPLACE FUNCTION mysql.varbit_eq_int4(pg_catalog.varbit, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2::pg_catalog.int8 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int4,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int4_eq_varbit(pg_catalog.int4, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1::pg_catalog.int8 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int4_eq_varbit,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.varbit
);

CREATE OR REPLACE FUNCTION mysql.varbit_eq_int2(pg_catalog.varbit, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2::pg_catalog.int8 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int2,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int2_eq_varbit(pg_catalog.int2, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1::pg_catalog.int8 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int2_eq_varbit,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.varbit
);

CREATE OR REPLACE FUNCTION mysql.varbit_eq_int8(pg_catalog.varbit, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int8,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int8_eq_varbit(pg_catalog.int8, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int8_eq_varbit,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.varbit
);

CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int2);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int, pg_catalog.int4);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt8(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt8,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int2);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int, pg_catalog.int4);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
    ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt8(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt8,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int2AndInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1::pg_catalog.int4 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2AndInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int4AndInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int4;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int4AndInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int8AndInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int8AndInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2AndNumeric(pg_catalog.int2, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.numeric);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndNumeric,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.int4AndNumeric(pg_catalog.int4, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.numeric);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndNumeric,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.int8AndNumeric(pg_catalog.int8, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.numeric);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndNumeric,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.numericAndInt2(pg_catalog.numeric, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int2);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt2,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.numericAndInt4(pg_catalog.numeric, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int4);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt4,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.numericAndInt8(pg_catalog.numeric, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int8);
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt8,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int2OrInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1::pg_catalog.int4 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2OrInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int4OrInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int4;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int4OrInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int8OrInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.int8OrInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2OrNumeric(pg_catalog.int2, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.numeric);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrNumeric,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.int4OrNumeric(pg_catalog.int4, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.numeric);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrNumeric,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.int8OrNumeric(pg_catalog.int8, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.numeric);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrNumeric,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.numeric
);

CREATE OR REPLACE FUNCTION mysql.numericOrInt2(pg_catalog.numeric, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int2);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt2,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.numericOrInt4(pg_catalog.numeric, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int4);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt4,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.numericOrInt8(pg_catalog.numeric, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int8);
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt8,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int8
);

DROP OPERATOR IF EXISTS mysql.=(mysql.smallint, pg_catalog.bool);
DROP FUNCTION IF EXISTS mysql.int2EqBool(mysql.smallint, pg_catalog.bool);
CREATE OR REPLACE FUNCTION mysql.int2EqBool(pg_catalog.int2, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return true;
    elsif ($1 = 0) and ($2 = false) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.int2, pg_catalog.bool);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int2EqBool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP OPERATOR IF EXISTS mysql.=(pg_catalog.bool, mysql.smallint);
DROP FUNCTION IF EXISTS mysql.boolEqInt2(pg_catalog.bool, mysql.smallint);
CREATE OR REPLACE FUNCTION mysql.boolEqInt2(pg_catalog.bool, pg_catalog.int2)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return true;
    elsif ($1 = false) and ($2 = 0) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.bool, pg_catalog.int2);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolEqInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

DROP OPERATOR IF EXISTS mysql.<>(mysql.smallint, pg_catalog.bool);
DROP FUNCTION IF EXISTS mysql.int2NeBool(mysql.smallint, pg_catalog.bool);
CREATE OR REPLACE FUNCTION mysql.int2NeBool(pg_catalog.int2, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return false;
    elsif ($1 = 0) and ($2 = false) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.int2NeBool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP OPERATOR IF EXISTS mysql.<>(pg_catalog.bool, mysql.smallint);
DROP FUNCTION IF EXISTS mysql.boolNeInt2(pg_catalog.bool, mysql.smallint);
CREATE OR REPLACE FUNCTION mysql.boolNeInt2(pg_catalog.bool, pg_catalog.int2)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return false;
    elsif ($1 = false) and ($2 = 0) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.boolNeInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.bigint_bitxor_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 # $2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.^#(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.^# (
    FUNCTION = mysql.bigint_bitxor_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.bigint_xor_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int2
AS
$$
BEGIN
    if ((($1 = 0) and ($2 != 0)) or (($1 != 0) and ($2 = 0))) then
        return 1;
    else
        return 0;
    end if;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.^^(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.^^ (
    FUNCTION = mysql.bigint_xor_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.text_and_text(pg_catalog.text, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 is null) and ($2 is null)) then
        return null;
    else
        raise EXCEPTION 'invalid parameters';
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.text, pg_catalog.text);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.text_and_text,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.bigint_and_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 = 0::pg_catalog.int8) or ($2 = 0::pg_catalog.int8)) then
        return false;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return true;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.bigint_and_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.text_or_anynoarray(pg_catalog.text, pg_catalog.anynonarray)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is not null) then
        return $1 operator(pg_catalog.||) $2;
    else
        if $2::pg_catalog.int8 != 0 then
            return true;
        else
            return null;
        end if;
        return 0::pg_catalog.int8 operator(mysql.||) $2::pg_catalog.int8;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.anynonarray);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_anynoarray,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.anynonarray
);

CREATE OR REPLACE FUNCTION mysql.anynonarray_or_text(pg_catalog.anynonarray, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($2 is not null) then
        return $1 operator(pg_catalog.||) $2;
    else
        if $1::pg_catalog.int8 != 0 then
            return true;
        else
            return null;
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.anynonarray, pg_catalog.text);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.anynonarray_or_text,
    LEFTARG = pg_catalog.anynonarray,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.bigint_or_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if ((($1 is not null) and ($1 != 0::pg_catalog.int8)) or
        (($2 is not null) and ($2 != 0::pg_catalog.int8))) then
        return true;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return false;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bigint_or_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql._add_bool(pg_catalog.bool)
returns pg_catalog.int2
AS
$$
BEGIN
    return $1::pg_catalog.int2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.+(none, pg_catalog.bool);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql._add_bool,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql._add_null(pg_catalog.text)
returns pg_catalog.int2
AS
$$
BEGIN
    raise EXCEPTION 'invalid parameters';
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.+(none, pg_catalog.text);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql._add_null,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql._sub_bool(pg_catalog.bool)
returns pg_catalog.int2
AS
$$
BEGIN
    return 0 - $1::pg_catalog.int2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.-(none, pg_catalog.bool);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql._sub_bool,
    RIGHTARG = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql._sub_null(pg_catalog.text)
returns pg_catalog.int2
AS
$$
BEGIN
    raise EXCEPTION 'invalid parameters';
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.-(none, pg_catalog.text);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql._sub_null,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.varbit, pg_catalog.int4) cascade;
DROP FUNCTION IF EXISTS mysql.varbit_eq_int4(pg_catalog.varbit, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.varbit_eq_int4(pg_catalog.varbit, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2::pg_catalog.int8 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int4,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int4
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.int4, pg_catalog.varbit);
DROP FUNCTION IF EXISTS mysql.int4_eq_varbit(pg_catalog.int4, pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_eq_varbit(pg_catalog.int4, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1::pg_catalog.int8 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int4_eq_varbit,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.varbit
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.varbit, pg_catalog.int2);
DROP FUNCTION IF EXISTS mysql.varbit_eq_int2(pg_catalog.varbit, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.varbit_eq_int2(pg_catalog.varbit, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2::pg_catalog.int8 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int2,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int2
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.int2, pg_catalog.varbit);
DROP FUNCTION IF EXISTS mysql.int2_eq_varbit(pg_catalog.int2, pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_eq_varbit(pg_catalog.int2, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1::pg_catalog.int8 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int2_eq_varbit,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.varbit
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.varbit, pg_catalog.int8);
DROP FUNCTION IF EXISTS mysql.varbit_eq_int8(pg_catalog.varbit, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.varbit_eq_int8(pg_catalog.varbit, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if pg_catalog.int8($1) = $2 then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.varbit_eq_int8,
    LEFTARG = pg_catalog.varbit,
    RIGHTARG = pg_catalog.int8
);

DROP OPERATOR IF EXISTS mysql.= (pg_catalog.int8, pg_catalog.varbit);
DROP FUNCTION IF EXISTS mysql.int8_eq_varbit(pg_catalog.int8, pg_catalog.varbit) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_eq_varbit(pg_catalog.int8, pg_catalog.varbit)
returns pg_catalog.bool
AS
$$
BEGIN
    if $1 = pg_catalog.int8($2) THEN
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int8_eq_varbit,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.varbit
);

DROP FUNCTION IF EXISTS mysql.int2LeftmoveInt2(pg_catalog.int2, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int2LeftmoveInt4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2LeftmoveInt8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2LeftmoveInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int2LeftmoveInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
        return mysql.int4LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int4LeftmoveInt4(pg_catalog.int4, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int4LeftmoveInt8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4LeftmoveInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int4LeftmoveInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int8LeftmoveInt2(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8LeftmoveInt4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret * 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            raise EXCEPTION 'bigint out of range';
            -- MySQL有 unsigned bigit，Halo没有
            -- return 18446744073709551600 - ((0 - $1) << $2::pg_catalog.int4);
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int8LeftmoveInt8(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int8LeftmoveInt8(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8LeftmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<<(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.<< (
    FUNCTION = mysql.int8LeftmoveInt8,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int2RightmoveInt2(pg_catalog.int2, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int2RightmoveInt4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2RightmoveInt8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2RightmoveInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int2RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int2RightmoveInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int4RightmoveInt2(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int4RightmoveInt4(pg_catalog.int4, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int4RightmoveInt8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4RightmoveInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int4RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int4RightmoveInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int8RightmoveInt2(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8RightmoveInt4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
DECLARE
ret pg_catalog.int8;
BEGIN
    if $1 is not null and $2 is not null then
        if 0 <= $1 then
            if 0 <= $2 then
                ret = $1;
                for i in 1..$2 loop
                    ret = ret / 2;
                end loop;
                return ret;
            else
                return 0;
            end if;
        else
            if 0 <= $2 then
                raise EXCEPTION 'bigint out of range';
                -- MySQL有 unsigned bigit，Halo没有
                -- return 18446744073709551600 - ((0 - $1) >> $2::pg_catalog.int4);
            else
                return 0;
            end if;
        end if;
    else
        return null;
    end if;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int8RightmoveInt8(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int8RightmoveInt8(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return mysql.int8RightmoveInt4($1, $2::pg_catalog.int4);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>>(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.>> (
    FUNCTION = mysql.int8RightmoveInt8,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int2AndInt4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2AndInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1::pg_catalog.int4 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2AndInt8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2AndInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int4AndInt2(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int4AndInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int4;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int4AndInt8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4AndInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int8AndInt2(pg_catalog.int8, pg_catalog.int2) CASCADE;
CREATE OR REPLACE FUNCTION mysql.int8AndInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8AndInt4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8AndInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2AndNumeric(pg_catalog.int2, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int2AndNumeric(pg_catalog.int2, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int2, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int2AndNumeric,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.int4AndNumeric(pg_catalog.int4, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int4AndNumeric(pg_catalog.int4, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int4AndNumeric,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.int8AndNumeric(pg_catalog.int8, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int8AndNumeric(pg_catalog.int8, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 & round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int8, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.int8AndNumeric,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.numericAndInt2(pg_catalog.numeric, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.numericAndInt2(pg_catalog.numeric, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
        return round($1)::pg_catalog.int8 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt2,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.numericAndInt4(pg_catalog.numeric, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.numericAndInt4(pg_catalog.numeric, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 & $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt4,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.numericAndInt8(pg_catalog.numeric, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.numericAndInt8(pg_catalog.numeric, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 & $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.numeric, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.& (
    FUNCTION = mysql.numericAndInt8,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int2OrInt4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2OrInt4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1::pg_catalog.int4 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrInt4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2OrInt8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2OrInt8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrInt8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int4OrInt2(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int4OrInt2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.int4
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int4;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrInt2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2

);

DROP FUNCTION IF EXISTS mysql.int4OrInt8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4OrInt8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrInt8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int8OrInt2(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int8OrInt2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrInt2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8OrInt4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8OrInt4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrInt4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2OrNumeric(pg_catalog.int2, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int2OrNumeric(pg_catalog.int2, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int2, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int2OrNumeric,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.int4OrNumeric(pg_catalog.int4, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int4OrNumeric(pg_catalog.int4, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1::pg_catalog.int8 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int4, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int4OrNumeric,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.int8OrNumeric(pg_catalog.int8, pg_catalog.numeric) cascade;
CREATE OR REPLACE FUNCTION mysql.int8OrNumeric(pg_catalog.int8, pg_catalog.numeric)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 | round($2)::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.int8, pg_catalog.numeric) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.int8OrNumeric,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.numeric
);

DROP FUNCTION IF EXISTS mysql.numericOrInt2(pg_catalog.numeric, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.numericOrInt2(pg_catalog.numeric, pg_catalog.int2)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int2) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt2,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.numericOrInt4(pg_catalog.numeric, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.numericOrInt4(pg_catalog.numeric, pg_catalog.int4)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2::pg_catalog.int8;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int4) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt4,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.numericOrInt8(pg_catalog.numeric, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.numericOrInt8(pg_catalog.numeric, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return round($1)::pg_catalog.int8 | $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.|(pg_catalog.numeric, pg_catalog.int8) cascade;
CREATE OPERATOR mysql.| (
    FUNCTION = mysql.numericOrInt8,
    LEFTARG = pg_catalog.numeric,
    RIGHTARG = pg_catalog.int8
);

DROP OPERATOR IF EXISTS mysql.=(mysql.smallint, pg_catalog.bool) cascade;
DROP FUNCTION IF EXISTS mysql.int2EqBool(mysql.smallint, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int2EqBool(pg_catalog.int2, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return true;
    elsif ($1 = 0) and ($2 = false) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.int2, pg_catalog.bool) cascade;
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.int2EqBool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP OPERATOR IF EXISTS mysql.=(pg_catalog.bool, mysql.smallint);
DROP FUNCTION IF EXISTS mysql.boolEqInt2(pg_catalog.bool, mysql.smallint);
CREATE OR REPLACE FUNCTION mysql.boolEqInt2(pg_catalog.bool, pg_catalog.int2)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return true;
    elsif ($1 = false) and ($2 = 0) then
        return true;
    else
        return false;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.bool, pg_catalog.int2);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.boolEqInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

DROP OPERATOR IF EXISTS mysql.<>(mysql.smallint, pg_catalog.bool);
DROP FUNCTION IF EXISTS mysql.int2NeBool(mysql.smallint, pg_catalog.bool);
CREATE OR REPLACE FUNCTION mysql.int2NeBool(pg_catalog.int2, pg_catalog.bool)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = 1) and ($2 = true) then
        return false;
    elsif ($1 = 0) and ($2 = false) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.<> (pg_catalog.int2, pg_catalog.bool);
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.int2NeBool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP OPERATOR IF EXISTS mysql.<>(pg_catalog.bool, mysql.smallint);
DROP FUNCTION IF EXISTS mysql.boolNeInt2(pg_catalog.bool, mysql.smallint);
CREATE OR REPLACE FUNCTION mysql.boolNeInt2(pg_catalog.bool, pg_catalog.int2)
RETURNS pg_catalog.bool
AS
$$
BEGIN
    if ($1 = true) and ($2 = 1) then
        return false;
    elsif ($1 = false) and ($2 = 0) then
        return false;
    else
        return true;
    end if;
END;
$$
LANGUAGE plpgsql STRICT;
DROP OPERATOR IF EXISTS mysql.<> (pg_catalog.bool, pg_catalog.int2);
CREATE OPERATOR mysql.<> (
    FUNCTION = mysql.boolNeInt2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.bigint_bitxor_bigint(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bigint_bitxor_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int8
AS
$$
BEGIN
    return $1 # $2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.^#(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.^# (
    FUNCTION = mysql.bigint_bitxor_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.bigint_xor_bigint(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bigint_xor_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.int2
AS
$$
BEGIN
    if ((($1 = 0) and ($2 != 0)) or (($1 != 0) and ($2 = 0))) then
        return 1;
    else
        return 0;
    end if;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.^^(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.^^ (
    FUNCTION = mysql.bigint_xor_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.text_and_text(pg_catalog.text, pg_catalog.text) cascade;
CREATE OR REPLACE FUNCTION mysql.text_and_text(pg_catalog.text, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 is null) and ($2 is null)) then
        return null;
    else
        raise EXCEPTION 'invalid parameters';
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.text, pg_catalog.text);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.text_and_text,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.text
);

DROP FUNCTION IF EXISTS mysql.bigint_and_bigint(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bigint_and_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 = 0::pg_catalog.int8) or ($2 = 0::pg_catalog.int8)) then
        return false;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return true;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.bigint_and_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.text_or_anynoarray(pg_catalog.text, pg_catalog.anynonarray) cascade;
CREATE OR REPLACE FUNCTION mysql.text_or_anynoarray(pg_catalog.text, pg_catalog.anynonarray)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is not null) then
        return $1 operator(pg_catalog.||) $2;
    else
        if $2::pg_catalog.int8 != 0 then
            return true;
        else
            return null;
        end if;
        return 0::pg_catalog.int8 operator(mysql.||) $2::pg_catalog.int8;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.anynonarray);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_anynoarray,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.anynonarray
);

DROP FUNCTION IF EXISTS mysql.anynonarray_or_text(pg_catalog.anynonarray, pg_catalog.text) cascade;
CREATE OR REPLACE FUNCTION mysql.anynonarray_or_text(pg_catalog.anynonarray, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($2 is not null) then
        return $1 operator(pg_catalog.||) $2;
    else
        if $1::pg_catalog.int8 != 0 then
            return true;
        else
            return null;
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.anynonarray, pg_catalog.text);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.anynonarray_or_text,
    LEFTARG = pg_catalog.anynonarray,
    RIGHTARG = pg_catalog.text
);

DROP FUNCTION IF EXISTS mysql.bigint_or_bigint(pg_catalog.int8, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bigint_or_bigint(pg_catalog.int8, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if ((($1 is not null) and ($1 != 0::pg_catalog.int8)) or
        (($2 is not null) and ($2 != 0::pg_catalog.int8))) then
        return true;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return false;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int8, pg_catalog.int8);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bigint_or_bigint,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql._add_bool(pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql._add_bool(pg_catalog.bool)
returns pg_catalog.int2
AS
$$
BEGIN
    return $1::pg_catalog.int2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.+(none, pg_catalog.bool);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql._add_bool,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql._add_null(pg_catalog.text) cascade;
CREATE OR REPLACE FUNCTION mysql._add_null(pg_catalog.text)
returns pg_catalog.int2
AS
$$
BEGIN
    raise EXCEPTION 'invalid parameters';
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.+(none, pg_catalog.text);
CREATE OPERATOR mysql.+ (
    FUNCTION = mysql._add_null,
    RIGHTARG = pg_catalog.text
);

DROP FUNCTION IF EXISTS mysql._sub_bool(pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql._sub_bool(pg_catalog.bool)
returns pg_catalog.int2
AS
$$
BEGIN
    return 0 - $1::pg_catalog.int2;
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.-(none, pg_catalog.bool);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql._sub_bool,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql._sub_null(pg_catalog.text) cascade;
CREATE OR REPLACE FUNCTION mysql._sub_null(pg_catalog.text)
returns pg_catalog.int2
AS
$$
BEGIN
    raise EXCEPTION 'invalid parameters';
END;
$$
immutable strict language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.-(none, pg_catalog.text);
CREATE OPERATOR mysql.- (
    FUNCTION = mysql._sub_null,
    RIGHTARG = pg_catalog.text
);

create or replace function mysql.boolCondAndbool(pg_catalog.bool, pg_catalog.bool)
returns pg_catalog.bool
as '$libdir/mysm', 'boolCondAndboolCImp'
language c immutable;
create operator mysql.&& (
    function = mysql.boolCondAndbool,
    leftarg = pg_catalog.bool,
    rightarg = pg_catalog.bool
);

CREATE OR REPLACE FUNCTION mysql.int4_and_int4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 = 0::pg_catalog.int4) or ($2 = 0::pg_catalog.int4)) then
        return false;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return true;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int4_and_int4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

CREATE OR REPLACE FUNCTION mysql.int2_and_int2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 = 0::pg_catalog.int2) or ($2 = 0::pg_catalog.int2)) then
        return false;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return true;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int2_and_int2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

CREATE OR REPLACE FUNCTION mysql.bool_or_bool(pg_catalog.bool, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 is not null) and ($2 is not null)) then
        if (($1 != 0::pg_catalog.int2) or ($2 != 0::pg_catalog.int2)) then
            return true;
        else
            return false;
        end if;
    elsif (($1 is not null) and ($2 is null)) then
        if ($1 != 0::pg_catalog.int2) then
            return true;
        else
            return false;
        end if;
    elsif (($1 is null) and ($2 is not null)) then
        if ($2 != 0::pg_catalog.int2) then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bool_or_bool,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int8_or_bool(pg_catalog.int8, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_or_bool(pg_catalog.int8, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    if ((($1 is not null) and ($1 != 0)) or
        (($2 is not null) and ($2 != false))) then
        return true;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return false;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int8, pg_catalog.bool);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int8_or_bool,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int8_or_int2(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_or_int2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int8_or_int2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8_or_int4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_or_int4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int8_or_int4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2_or_bool(pg_catalog.int2, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_or_bool(pg_catalog.int2, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.int8_or_bool($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int2, pg_catalog.bool);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int2_or_bool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int2_or_int2(pg_catalog.int2, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_or_int2(pg_catalog.int2, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int2, pg_catalog.int2);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int2_or_int2,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int2_or_int4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_or_int4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int2_or_int4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2_or_int8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_or_int8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int2_or_int8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int4_or_bool(pg_catalog.int4, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_or_bool(pg_catalog.int4, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.int8_or_bool($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int4, pg_catalog.bool);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int4_or_bool,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int4_or_int2(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_or_int2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int4_or_int2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int4_or_int4(pg_catalog.int4, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_or_int4(pg_catalog.int4, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int4, pg_catalog.int4);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int4_or_int4,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int4_or_int8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_or_int8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_or_bigint($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.int4_or_int8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.bool_or_int8(pg_catalog.bool, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_or_int8(pg_catalog.bool, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if ((($1 is not null) and ($1 != false)) or
        (($2 is not null) and ($2 != 0))) then
        return true;
    elsif (($1 is null) or ($2 is null)) then
        return null;
    else
        return false;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.bool, pg_catalog.int8);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bool_or_int8,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.bool_or_int2(pg_catalog.bool, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_or_int2(pg_catalog.bool, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.bool_or_int8($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.bool, pg_catalog.int2);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bool_or_int2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.bool_or_int4(pg_catalog.bool, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_or_int4(pg_catalog.bool, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.bool_or_int8($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.bool, pg_catalog.int4);
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bool_or_int4,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int8_and_bool(pg_catalog.int8, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_and_bool(pg_catalog.int8, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 is not null) and ($2 is not null)) then
        if (($1 != 0) and ($2 = true)) then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int8, pg_catalog.bool);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int8_and_bool,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int8_and_int2(pg_catalog.int8, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_and_int2(pg_catalog.int8, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int8, pg_catalog.int2);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int8_and_int2,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int8_and_int4(pg_catalog.int8, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int8_and_int4(pg_catalog.int8, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int8, pg_catalog.int4);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int8_and_int4,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2_and_bool(pg_catalog.int2, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_and_bool(pg_catalog.int2, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.int8_and_bool($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int2, pg_catalog.bool);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int2_and_bool,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int2_and_int4(pg_catalog.int2, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_and_int4(pg_catalog.int2, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int2, pg_catalog.int4);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int2_and_int4,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.int2_and_int8(pg_catalog.int2, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int2_and_int8(pg_catalog.int2, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int2, pg_catalog.int8);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int2_and_int8,
    LEFTARG = pg_catalog.int2,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.int4_and_bool(pg_catalog.int4, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_and_bool(pg_catalog.int4, pg_catalog.bool)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.int8_and_bool($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int4, pg_catalog.bool);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int4_and_bool,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.bool
);

DROP FUNCTION IF EXISTS mysql.int4_and_int2(pg_catalog.int4, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_and_int2(pg_catalog.int4, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1::pg_catalog.int8, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int4, pg_catalog.int2);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int4_and_int2,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.int4_and_int8(pg_catalog.int4, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.int4_and_int8(pg_catalog.int4, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return bigint_and_bigint($1::pg_catalog.int8, $2);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.int4, pg_catalog.int8);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.int4_and_int8,
    LEFTARG = pg_catalog.int4,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.bool_and_int8(pg_catalog.bool, pg_catalog.int8) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_and_int8(pg_catalog.bool, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    if (($1 is not null) and ($2 is not null)) then
        if (($1 = true) and ($2 != 0)) then
            return true;
        else
            return false;
        end if;
    else
        return null;
    end if;
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.bool, pg_catalog.int8);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.bool_and_int8,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int8
);

DROP FUNCTION IF EXISTS mysql.bool_and_int2(pg_catalog.bool, pg_catalog.int2) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_and_int2(pg_catalog.bool, pg_catalog.int2)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.bool_and_int8($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.bool, pg_catalog.int2);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.bool_and_int2,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int2
);

DROP FUNCTION IF EXISTS mysql.bool_and_int4(pg_catalog.bool, pg_catalog.int4) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_and_int4(pg_catalog.bool, pg_catalog.int4)
returns pg_catalog.bool
AS
$$
BEGIN
    return mysql.bool_and_int8($1, $2::pg_catalog.int8);
END;
$$
immutable language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.&&(pg_catalog.bool, pg_catalog.int4);
CREATE OPERATOR mysql.&& (
    FUNCTION = mysql.bool_and_int4,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.int4
);

DROP FUNCTION IF EXISTS mysql.bool_or_bool(pg_catalog.bool, pg_catalog.bool) cascade;
CREATE OR REPLACE FUNCTION mysql.bool_or_bool(pg_catalog.bool, pg_catalog.bool)
RETURNS pg_catalog.bool
AS '$libdir/mysm', 'boolOrBool'
LANGUAGE C IMMUTABLE;
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.bool_or_bool,
    LEFTARG = pg_catalog.bool,
    RIGHTARG = pg_catalog.bool
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.text_or_text(pg_catalog.text, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.text_or_text(pg_catalog.text, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_text,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.anynonarray, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.anynonarray_or_text(pg_catalog.anynonarray, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.anycompatiblenonarray_or_text(pg_catalog.anycompatiblenonarray, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.anycompatiblenonarray_or_text,
    LEFTARG = pg_catalog.anycompatiblenonarray,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.anynonarray);
DROP FUNCTION IF EXISTS mysql.text_or_anynoarray(pg_catalog.text, pg_catalog.anynonarray);
CREATE OR REPLACE FUNCTION mysql.text_or_anycompatiblenonarray(pg_catalog.text, pg_catalog.anycompatiblenonarray)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_anycompatiblenonarray,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.anycompatiblenonarray
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.text_or_text(pg_catalog.text, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.text_or_text(pg_catalog.text, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_text,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.anynonarray, pg_catalog.text);
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.anycompatiblenonarray, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.anynonarray_or_text(pg_catalog.anynonarray, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.anycompatiblenonarray_or_text(pg_catalog.anycompatiblenonarray, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.anycompatiblenonarray_or_text,
    LEFTARG = pg_catalog.anycompatiblenonarray,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.anynonarray);
DROP OPERATOR IF EXISTS mysql.||(pg_catalog.text, pg_catalog.anycompatiblenonarray);
DROP FUNCTION IF EXISTS mysql.text_or_anynoarray(pg_catalog.text, pg_catalog.anynonarray);
CREATE OR REPLACE FUNCTION mysql.text_or_anycompatiblenonarray(pg_catalog.text, pg_catalog.anycompatiblenonarray)
returns pg_catalog.bool
AS
$$
BEGIN
    if ($1 is null) then
        if ($2 is null) then
            return null;
        else
            if ($2::int8 != 0) then
                return true;
            else
                return null;
            end if;
        end if;
    else
        if ($2 is null) then
            if ($1::int8 != 0) then
                return true;
            else
                return null;
            end if;
        else
            return ($1::int8 != 0) || ($2::int8 != 0);
        end if;
    end if;
END;
$$
immutable language 'plpgsql';
CREATE OPERATOR mysql.|| (
    FUNCTION = mysql.text_or_anycompatiblenonarray,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.anycompatiblenonarray
);

CREATE OR REPLACE FUNCTION mysql.timestamp_gt_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 > tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 > $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.timestamp_gt_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamptz_gt_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 > tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 > $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.timestamptz_gt_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_ge_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 >= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 >= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.timestamp_ge_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_lt_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 < tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 < $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.timestamp_lt_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_le_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 <= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 <= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.timestamp_le_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_eq_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 = tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 = $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.timestamp_eq_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_ne_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamp;
BEGIN
    tmp2 := $2::pg_catalog.timestamp;
    return ($1 != tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 != $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.timestamp_ne_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamptz_ge_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 >= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 >= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.timestamptz_ge_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamptz_lt_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 < tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 < $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.timestamptz_lt_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);


CREATE OR REPLACE FUNCTION mysql.timestamptz_le_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 <= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 <= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.timestamptz_le_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamptz_eq_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 = tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 = $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.timestamptz_eq_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamptz_ne_text(pg_catalog.timestamptz, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.timestamptz;
BEGIN
    tmp2 := $2::pg_catalog.timestamptz;
    return ($1 != tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 != $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.timestamptz, pg_catalog.text);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.timestamptz_ne_text,
    LEFTARG = pg_catalog.timestamptz,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_gt_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 > tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 > $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.time_gt_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_ge_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 >= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 >= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.time_ge_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_lt_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 < tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 < $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.time_lt_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_le_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 <= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 <= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.time_le_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_eq_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 = tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 = $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.time_eq_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.time_ne_text(pg_catalog.time, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.time;
BEGIN
    tmp2 := $2::pg_catalog.time;
    return ($1 != tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 != $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.time, pg_catalog.text);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.time_ne_text,
    LEFTARG = pg_catalog.time,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_gt_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 > tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 > $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.date_gt_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_ge_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 >= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 >= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.date_ge_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_lt_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 < tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 < $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.date_lt_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_le_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 <= tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 <= $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.date_le_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_eq_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 = tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 = $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.date_eq_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.date_ne_text(pg_catalog.date, pg_catalog.text)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.text;
    tmp2 pg_catalog.date;
BEGIN
    tmp2 := $2::pg_catalog.date;
    return ($1 != tmp2);

    EXCEPTION
        WHEN others THEN
            tmp1 := $1::pg_catalog.text;
            return (tmp1 != $2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.date, pg_catalog.text);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.date_ne_text,
    LEFTARG = pg_catalog.date,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.part_list_columns(VARIADIC pg_catalog.any)
RETURNS pg_catalog.text
AS 'MODULE_PATHNAME', 'partListColumns'
IMMUTABLE LANGUAGE C;

CREATE OR REPLACE FUNCTION mysql.year(date)
RETURNS integer
AS
$$
    SELECT EXTRACT(YEAR FROM $1)::integer
$$
IMMUTABLE STRICT LANGUAGE SQL;

CREATE OR REPLACE FUNCTION mysql.text_gt_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 > $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 > tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.text_gt_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_ge_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 >= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 >= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.text_ge_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_lt_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 < $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 < tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.text_lt_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_le_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 <= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 <= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.text_le_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_eq_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 = $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 = tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.text_eq_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_ne_timestamp(pg_catalog.text, pg_catalog.timestamp)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamp;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamp;
    return (tmp1 != $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 != tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.text, pg_catalog.timestamp);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.text_ne_timestamp,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamp
);

CREATE OR REPLACE FUNCTION mysql.text_gt_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 > $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 > tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.text_gt_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_ge_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 >= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 >= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.text_ge_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_lt_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 < $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 < tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.text_lt_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_le_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 <= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 <= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.text_le_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_eq_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 = $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 = tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.text_eq_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_ne_timestamptz(pg_catalog.text, pg_catalog.timestamptz)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.timestamptz;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.timestamptz;
    return (tmp1 != $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 != tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.text, pg_catalog.timestamptz);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.text_ne_timestamptz,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.timestamptz
);

CREATE OR REPLACE FUNCTION mysql.text_gt_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 > $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 > tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.text_gt_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_ge_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 >= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 >= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.text_ge_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_lt_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 < $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 < tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.text_lt_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_le_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 <= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 <= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.text_le_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_eq_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 = $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 = tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.text_eq_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_ne_time(pg_catalog.text, pg_catalog.time)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.time;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.time;
    return (tmp1 != $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 != tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.text, pg_catalog.time);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.text_ne_time,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.time
);

CREATE OR REPLACE FUNCTION mysql.text_gt_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 > $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 > tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.text_gt_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.text_ge_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 >= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 >= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.text_ge_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.text_lt_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 < $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 < tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.text_lt_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.text_le_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 <= $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 <= tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.text_le_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.text_eq_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 = $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 = tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.text_eq_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.text_ne_date(pg_catalog.text, pg_catalog.date)
returns pg_catalog.bool
AS
$$
DECLARE
    tmp1 pg_catalog.date;
    tmp2 pg_catalog.text;
BEGIN
    tmp1 := $1::pg_catalog.date;
    return (tmp1 != $2);

    EXCEPTION
        WHEN others THEN
            tmp2 := $2::pg_catalog.text;
            return ($1 != tmp2);
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!=(pg_catalog.text, pg_catalog.date);
CREATE OPERATOR mysql.!= (
    FUNCTION = mysql.text_ne_date,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.date
);

CREATE OR REPLACE FUNCTION mysql.int4andtext(pg_catalog.int4, text)
RETURNS pg_catalog.int4
AS
$$
BEGIN
    return $1 & mysql.convert_text_to_digit_text_for_mysql($2)::integer;
END;
$$
STRICT
language plpgsql;
DROP OPERATOR IF EXISTS mysql.&(pg_catalog.int4, text);
CREATE OPERATOR mysql.&(
    FUNCTION=mysql.int4andtext,
    LEFTARG=pg_catalog.int4,
    RIGHTARG=text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_gt_text(pg_catalog.timestamp, pg_catalog.text)
RETURNS pg_catalog.bool
AS 'MODULE_PATHNAME', 'mys_timestampGtText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.>(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.> (
    FUNCTION = mysql.timestamp_gt_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_ge_text(pg_catalog.timestamp, pg_catalog.text)
RETURNS pg_catalog.bool
AS 'MODULE_PATHNAME', 'mys_timestampGeText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.>=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.>= (
    FUNCTION = mysql.timestamp_ge_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_lt_text(pg_catalog.timestamp, pg_catalog.text)
RETURNS pg_catalog.bool
AS 'MODULE_PATHNAME', 'mys_timestampLtText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.<(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.< (
    FUNCTION = mysql.timestamp_lt_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_le_text(pg_catalog.timestamp, pg_catalog.text)
RETURNS pg_catalog.bool
AS 'MODULE_PATHNAME', 'mys_timestampLeText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.<=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.<= (
    FUNCTION = mysql.timestamp_le_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.timestamp_eq_text(pg_catalog.timestamp, pg_catalog.text)
returns pg_catalog.bool
AS 'MODULE_PATHNAME', 'mys_timestampEqText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.=(pg_catalog.timestamp, pg_catalog.text);
CREATE OPERATOR mysql.= (
    FUNCTION = mysql.timestamp_eq_text,
    LEFTARG = pg_catalog.timestamp,
    RIGHTARG = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.text_pl_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
AS 'MODULE_PATHNAME', 'mys_textPlInterval'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.text, pg_catalog.interval);
CREATE OPERATOR mysql.+(
    function = mysql.text_pl_interval,
    leftarg = pg_catalog.text,
    rightarg = pg_catalog.interval
);

CREATE OR REPLACE FUNCTION mysql.interval_pl_text(pg_catalog.interval, text)
RETURNS timestamp without time zone
AS 'MODULE_PATHNAME', 'mys_intervalPlText'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.+(pg_catalog.interval, pg_catalog.text);
CREATE OPERATOR mysql.+(
    function = mysql.interval_pl_text,
    leftarg = pg_catalog.interval,
    rightarg = pg_catalog.text
);

CREATE OR REPLACE FUNCTION mysql.text_mi_interval(text, pg_catalog.interval)
RETURNS timestamp without time zone
AS 'MODULE_PATHNAME', 'mys_textMiInterval'
LANGUAGE C
STRICT
IMMUTABLE;
DROP OPERATOR IF EXISTS mysql.-(pg_catalog.text, pg_catalog.interval);
CREATE OPERATOR mysql.-(
    function = mysql.text_mi_interval,
    leftarg = text,
    rightarg = pg_catalog.interval
);

DROP OPERATOR IF EXISTS mysql.~~(pg_catalog.int8, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.bigint_like_text(pg_catalog.int8, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.bigint_like_text(pg_catalog.int8, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1::pg_catalog.text like $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.~~(pg_catalog.int8, pg_catalog.text);
CREATE OPERATOR mysql.~~ (
    FUNCTION = mysql.bigint_like_text,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.~~(pg_catalog.text, pg_catalog.int8);
DROP FUNCTION IF EXISTS mysql.text_like_bigint(pg_catalog.text, pg_catalog.int8);
CREATE OR REPLACE FUNCTION mysql.text_like_bigint(pg_catalog.text, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 like $2::pg_catalog.text;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.~~(pg_catalog.text, pg_catalog.int8);
CREATE OPERATOR mysql.~~ (
    FUNCTION = mysql.text_like_bigint,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.int8
);

DROP OPERATOR IF EXISTS mysql.!~~(pg_catalog.int8, pg_catalog.text);
DROP FUNCTION IF EXISTS mysql.bigint_not_like_text(pg_catalog.int8, pg_catalog.text);
CREATE OR REPLACE FUNCTION mysql.bigint_not_like_text(pg_catalog.int8, pg_catalog.text)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1::pg_catalog.text not like $2;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!~~(pg_catalog.int8, pg_catalog.text);
CREATE OPERATOR mysql.!~~ (
    FUNCTION = mysql.bigint_not_like_text,
    LEFTARG = pg_catalog.int8,
    RIGHTARG = pg_catalog.text
);

DROP OPERATOR IF EXISTS mysql.!~~(pg_catalog.text, pg_catalog.int8);
DROP FUNCTION IF EXISTS mysql.text_not_like_bigint(pg_catalog.text, pg_catalog.int8);
CREATE OR REPLACE FUNCTION mysql.text_not_like_bigint(pg_catalog.text, pg_catalog.int8)
returns pg_catalog.bool
AS
$$
BEGIN
    return $1 not like $2::pg_catalog.text;
END
$$
language 'plpgsql';
DROP OPERATOR IF EXISTS mysql.!~~(pg_catalog.text, pg_catalog.int8);
CREATE OPERATOR mysql.!~~ (
    FUNCTION = mysql.text_not_like_bigint,
    LEFTARG = pg_catalog.text,
    RIGHTARG = pg_catalog.int8
);


GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mysql TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mys_informa_schema TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA sys TO public;
GRANT ALL PRIVILEGES ON ALL tables IN SCHEMA mys_sys TO public;

