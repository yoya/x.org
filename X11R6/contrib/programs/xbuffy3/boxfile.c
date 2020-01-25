/*******************************************************************************

     Copyright (c) 1994    William Pemberton (wfp5p@virginia.edu)

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.

*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "xbuffy.h"

#define cmpTok(tok,str,tokVal) {if ( strcmp(tok,str) == 0) return(tokVal);}

static char *tokens[17] = {
	"box", "title", "command", "audio", "mailbox", "newsbox", "origmode",
	"newmode", "beep", "nobeep", "last", "headertime", "polltime",
	"shortname", "longname", NULL
};

enum TokType
{
	UNK_T = 0, BOX_T, TITLE_T, COMMAND_T, AUDIO_T, MAILBOX_T, NEWSBOX_T,
	ORIGMODE_T, NEWMODE_T, BEEP_T, NOBEEP_T, LAST_T, HEADER_T, POLL_T,
	SHORT_T, LONG_T
};

typedef enum TokType TokenType;

TokenType token(line, next)
	char *line;
	char *next;
{
	char tok[30];
	char *p1;
	int x;

	p1 = line;
	x = 0;

	while ((*p1 != '\0') && (isspace(*p1)))
		p1++;

	while ((*p1 != '\0') && (!isspace(*p1)) && (x < 29))
	{
		tok[x++] = (isupper(*p1) ? tolower(*p1) : *p1);
		p1++;
	}
	tok[x] = '\0';

	while ((*p1 != '\0') && (isspace(*p1)))
		p1++;

	strcpy(next, p1);
	next[strlen(next) - 1] = '\0';	/* strip the newline */

	for (x = 0; tokens[x] != NULL; x++)
	{
		if
			(strcmp(tok, tokens[x]) == 0)
			return (x + 1);
	}
	return (UNK_T);

}

void clearBox(tempBox)
	BoxInfo_t *tempBox;
{
	if (tempBox->box != NULL)
	{
		free(tempBox->box);
		tempBox->box = NULL;
	}

	if (tempBox->command != NULL)
	{
		free(tempBox->command);
		tempBox->command = NULL;
	}

	if (tempBox->audioCmd != NULL)
	{
		free(tempBox->audioCmd);
		tempBox->audioCmd = NULL;
	}

	if (tempBox->boxTitle != NULL)
	{
		free(tempBox->boxTitle);
		tempBox->boxTitle = NULL;
	}

	tempBox->type = 0;
	tempBox->last = 0;
	tempBox->headerTime = tempBox->nobeep = tempBox->origMode = 0;
	tempBox->pollTime = tempBox->headerTime = -1;
	tempBox->BoxNameType = UNDEF;
}


char *parseTwiddle(str)
	char *str;
{
	static char retVal[MAX_STRING];
	char *ptr, *res;
	char *home;

	ptr = str;
	res = retVal;

	while (*ptr != '\0')
	{
		if (*ptr == '~')
		{
			home = (char *) getenv("HOME");
			strcpy(res, home);
			res += strlen(home);
		}
		else
			*(res++) = *ptr;

		++ptr;
	}
	*res = '\0';

	return (retVal);
}



#ifdef TESTBOX

char *showNull(w)
	char *w;

{
	if (w == NULL)
		return ("NULL");
	else
		return (w);
}

void dumpBox(tempBox)
	BoxInfo_t tempBox;
{
	printf("Dumping Box = *%s*\n", tempBox.box);
	printf("type = %i\n", tempBox.type);
	printf("command	= *%s*\n", showNull(tempBox.command));
	printf("audio = *%s*\n", showNull(tempBox.audioCmd));
	printf("boxTitle = *%s*\n", showNull(tempBox.boxTitle));
	printf("pollTime = %i  headerTime = %i\n", tempBox.pollTime, tempBox.headerTime);
	printf("nobeep = %i  origMode = %i \n", tempBox.nobeep, tempBox.origMode);
	printf("nametype = %i\n\n", tempBox.BoxNameType);
}

#endif



void readBoxfile(boxFile)
	char *boxFile;
{
	BoxInfo_t tempBox;
	FILE *boxes;
	char line[MAX_STRING];
	int inBox, oldBox;
	char next[MAX_STRING];

	tempBox.box = tempBox.command = tempBox.audioCmd = tempBox.boxTitle = NULL;
	clearBox(&tempBox);


	if ((boxes = fopen(boxFile, "r")) == 0)
	{
		fprintf(stderr, "Could not open boxfile %s\n", boxFile);
		return;
	}

	inBox = 0;

	while (fgets(line, MAX_STRING - 1, boxes) != 0)
	{

		if (line[0] == '#')		/* it's a comment */
			continue;

		switch (token(line, next))
		{
		case BOX_T:
			if (inBox)
			{
#ifndef TESTBOX

				initBox(tempBox.box,
						tempBox.type, tempBox.pollTime,
						tempBox.headerTime, tempBox.BoxNameType,
						tempBox.command,
						tempBox.audioCmd,
						tempBox.boxTitle,
						tempBox.origMode, tempBox.nobeep);

#else
				dumpBox(tempBox);
#endif
				clearBox(&tempBox);
			}

			tempBox.box = (char *) strdup(parseTwiddle(next));
			inBox = TRUE;


			break;

		case TITLE_T:
			tempBox.boxTitle = (char *) strdup(next);
			tempBox.BoxNameType = USR;
			break;

		case COMMAND_T:
			tempBox.command = (char *) strdup(next);
			break;

		case AUDIO_T:
			tempBox.audioCmd = (char *) strdup(next);
			break;

		case MAILBOX_T:
			tempBox.type = MAILBOX;
			break;

		case NEWSBOX_T:
			tempBox.type = NNTPBOX;
			break;

		case ORIGMODE_T:
			tempBox.origMode = TRUE;
			break;

		case NEWMODE_T:
			tempBox.origMode = FALSE;
			break;

		case BEEP_T:
			tempBox.nobeep = FALSE;
			break;

		case NOBEEP_T:
			tempBox.nobeep = TRUE;
			break;

		case LAST_T:
			tempBox.last = atoi(next);
			break;

		case HEADER_T:
			tempBox.headerTime = atoi(next);
			break;

		case POLL_T:
			tempBox.pollTime = atoi(next);
			break;

		case SHORT_T:
			tempBox.BoxNameType = SHORT;
			break;

		case LONG_T:
			tempBox.BoxNameType = LONG;
			break;

		default:
			break;
		}

	}							/* while */

	if (inBox)
#ifndef TESTBOX
		initBox(tempBox.box,
				tempBox.type, tempBox.pollTime,
				tempBox.headerTime, tempBox.BoxNameType,
				tempBox.command,
				tempBox.audioCmd,
				tempBox.boxTitle,
				tempBox.origMode, tempBox.nobeep);

#else
		dumpBox(tempBox);
#endif

	fclose(boxes);

}



#ifdef TESTBOX

main()
{


	readBoxfile("boxfile.sample");
}

#endif
