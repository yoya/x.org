/* this is from elm 2.4 so: */

/*******************************************************************************
*                      Copyright (c) 1988-1992 USENET Community Trust
*                      Copyright (c) 1986,1987 Dave Taylor
********************************************************************************/


/**
	compare a header, ignoring case and allowing linear white space
	around the :.  Header must be anchored to the start of the line.

	returns NULL if no match, or first character after trailing linear
	white space of the :.

**/
#include <stdio.h>
#include <ctype.h>


#define whitespace(c)    (c == ' ' || c == '\t')

#ifdef BSD
#undef tolower
#endif


char *header_cmp(char *header, char *prefix, char *suffix)
{
	int len;

	len = strlen(prefix);
	if (strincmp(header, prefix, len))
		return (NULL);

	/* skip over while space if any */
	header += len;

	if (*header != ':')			/* headers must end in a : */
		return (NULL);

	/* skip over while space if any */
	header++;

	while (*header)
	{
		if (!whitespace(*header))
			break;
		header++;
	}

	if (suffix != NULL)
	{
		len = strlen(suffix);
		if (len > 0)
			if (strincmp(header, suffix, len))
				return (NULL);
	}

	return (header);
}
