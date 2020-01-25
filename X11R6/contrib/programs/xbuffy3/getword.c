
/*******************************************************************************
 *  The Elm Mail System  -  $Revision: 5.1 $   $State: Exp $
 *
 * 			Copyright (c) 1993 USENET Community Trust
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef WFP_DEBUG
#include "/home/wfp5p/bin/debug_include/malloc.h"
#endif


int get_word(buffer, start, word, wordlen)
	char *buffer;
	int start;
	char *word;
	int wordlen;
{
	/*
	   Extracts the next white-space delimited word from the "buffer" starting
	   at "start" characters into the buffer and skipping any leading
	   white-space there.  Handles backslash-quoted characters and double-quote
	   bracked strings as an atomic unit.  The resulting word, up to "wordlen"
	   bytes long, is saved in "word".  Returns the buffer index where
	   extraction terminated, e.g. the next word can be extracted by starting
	   at start+<return-val>.  If no words are found in the buffer then -1 is
	   returned.
	*/

	register int len;
	register char *p;

	for (p = buffer + start; isspace(*p); ++p)
		;

	if (*p == '\0')
		return (-1);			/* nothing IN buffer! */

	while (*p != '\0')
	{
		len = len_next_part(p);
		if (len == 1 && isspace(*p))
			break;

		while (--len >= 0)
		{
			if (--wordlen > 0)
				*word++ = *p;
			++p;
		}
	}

	*word = '\0';
	return (p - buffer);
}


#ifdef _TEST
main()
{
	char buf[1024], word[1024], *bufp;
	int start, len;

	while (gets(buf) != NULL)
	{

		puts("parsing with front of buffer anchored");
		start = 0;
		while ((len = get_word(buf, start, word, sizeof(word))) > 0)
		{
			printf("start=%d len=%d word=%s\n", start, len, word);
			start = len;
		}
		putchar('\n');

		puts("parsing with front of buffer updated");
		bufp = buf;
		while ((len = get_word(bufp, 0, word, sizeof(word))) > 0)
		{
			printf("start=%d len=%d word=%s\n", 0, len, word);
			bufp += len;
		}
		putchar('\n');

	}

	exit(0);
}

#endif
