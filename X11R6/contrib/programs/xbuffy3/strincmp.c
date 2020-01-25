
/*******************************************************************************
 *  The Elm Mail System  -  $Revision: 5.1 $   $State: Exp $
 *
 *			Copyright (c) 1988-1992 USENET Community Trust
 *			Copyright (c) 1986,1987 Dave Taylor
 ******************************************************************************/

/** compare strings ignoring case - length limited
**/

#include <ctype.h>

#ifdef WFP_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include "/home/wfp5p/bin/debug_include/malloc.h"
#endif


#ifdef BSD
#undef tolower
#undef toupper
#endif

int strincmp(s1, s2, n)
	register char *s1, *s2;
	register int n;
{
	/* case insensitive comparison */
	register int d;

	while (--n >= 0)
	{
		d = (isupper(*s1) ? tolower(*s1) : *s1)
			- (isupper(*s2) ? tolower(*s2) : *s2);
		if (d != 0 || *s1 == '\0' || *s2 == '\0')
			return d;
		++s1;
		++s2;
	}
	return (0);
}
