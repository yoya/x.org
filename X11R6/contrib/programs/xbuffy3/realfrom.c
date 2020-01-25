/*******************************************************************************
 *  The Elm Mail System  -  $Revision: 5.1 $   $State: Exp $
 *
 * 			Copyright (c) 1993 USENET Community Trust
 *******************************************************************************/

/* I've deleted a lot of stuff for xbuffy */

#include <stdio.h>
#include <ctype.h>
#include <ctype.h>
#include "xbuffy.h"

#ifdef WFP_DEBUG
#include "/home/wfp5p/bin/debug_include/malloc.h"
#endif


#ifdef BSD
#undef tolower
#undef toupper
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define whitespace(c)    (c == ' ' || c == '\t')


static char *month_name[13] = {
	"jan", "feb", "mar", "apr", "may", "jun",
	"jul", "aug", "sep", "oct", "nov", "dec", NULL
};

static char *day_name[8] = {
	"sun", "mon", "tue", "wed", "thu", "fri", "sat", 0
};


int real_from(buffer)
	char *buffer;
{

	/*
	   Breakup and validate the "From_" line in the "buffer".  If "entry" is
	   not NULL then the structure is filled in with sender and time
	   information.  Returns TRUE if the "From_" line is valid, otherwise
	   FALSE.
	
	A valid from line will be in the following format:
	
	From <user> <weekday> <month> <day> <hr:min:sec> [TZ1 [TZ2]] <year> [remote
	   from sitelist]
	
	We insist that all of the <angle bracket> fields are present. If two
	   timezone fields are present, the first is used for date information.  We
	   do not look at anything beyond the <year> field. We just insist that
	   everything up to the <year> field is present and valid.
	*/

	char field[255];			/* buffer for current field of line	 */
	char save_tz[255];			/* extracted time zone field		 */
	int len;					/* length of current field		 */
	int day;
	int i;
	int found;


	/* From */

	if (strncmp(buffer, "From ", 5) != 0)
		goto failed;

	buffer += 5;


	/* <user> */
	if ((len = get_word(buffer, 0, field, sizeof(field))) < 0)
		goto failed;
	buffer += len;

	/* <weekday> */
	if ((len = get_word(buffer, 0, field, sizeof(field))) < 0)
		goto failed;
	else
	{
		found = 0;
		for (i = 0; day_name[i] != NULL; i++)
			found = found || (strincmp(day_name[i], field, 3) == 0);
		if (!found)
			goto failed;
	}

	buffer += len;

	/* <month> */
	if ((len = get_word(buffer, 0, field, sizeof(field))) < 0)
		goto failed;
	else
	{
		found = 0;
		for (i = 0; month_name[i] != NULL; i++)
			found = found || (strincmp(month_name[i], field, 3) == 0);
		if (!found)
			goto failed;
	}
	buffer += len;

	/* <day> */
	if ((len = get_word(buffer, 0, field, sizeof(field))) < 0 ||
		(day = atoi(field)) < 0 || day < 1 || day > 31)
		goto failed;
	buffer += len;


	/* The line is parsed and valid.  There might be more but we don't care. */
	return TRUE;

failed:
	return FALSE;
}
