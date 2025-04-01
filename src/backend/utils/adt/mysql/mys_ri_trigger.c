#include "postgres.h"

#include "utils/mysql/mys_ri_trigger.h"

void
mys_quoteOneName(char *buffer, const char *name)
{
	/* Rather than trying to be smart, just always quote it. */
	*buffer++ = '`';
	while (*name)
	{
		if (*name == '`')
			*buffer++ = '`';
		*buffer++ = *name++;
	}
	*buffer++ = '`';
	*buffer = '\0';
}
