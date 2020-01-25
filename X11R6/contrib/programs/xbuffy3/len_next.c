
/*******************************************************************************
 *  The Elm Mail System  -  $Revision: 5.3 $   $State: Exp $
 *
 * 			Copyright (c) 1992 USENET Community Trust
 ******************************************************************************/

/** get the length of the next part of the address/data field

	This code returns the length of the next part of the
  string field containing address/data.  It takes into account
  quoting via " as well as \ escapes.
  Quoting via ' is not taken into account, as RFC-822 does not
  consider a ' character a valid 'quoting character'

  A 1 is returned for a single character unless:

  A 0 is returned at end of string.

  A 2 is returned for strings that start \

  The length of quoted sections is returned for quoted fields

**/

#include <ctype.h>

#ifdef WFP_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include "/home/wfp5p/bin/debug_include/malloc.h"
#endif


int
    len_next_part(s)
	register char *s;
{
	register char *c, quot;

	quot = *s;

	if (quot == '\0')
		return (0);

	if (quot == '\\')
		return (*++s != '\0' ? 2 : 1);

	if (quot != '"')
		return (1);

	for (c = s + 1; *c; c++)
	{
		if (*c == quot)
			return (1 + c - s);

		if (*c == '\\')
		{
			if (*c++)
				c++;
		}
	}

	return (c - s);
}
