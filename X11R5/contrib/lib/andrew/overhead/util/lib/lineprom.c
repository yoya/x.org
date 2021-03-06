/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/*
	lineprom.c -- Decide whether a datastream line should be promoted
		to the top of the message that encapsulates it.
*/
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/overhead/util/lib/RCS/lineprom.c,v 2.4 1991/09/12 17:27:39 bobg Exp $ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/overhead/util/lib/RCS/lineprom.c,v $ */

#ifndef lint
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/overhead/util/lib/RCS/lineprom.c,v 2.4 1991/09/12 17:27:39 bobg Exp $";
#endif /* lint */

#include <stdio.h>
#include <andrewos.h>	/* strings.h */
#include <ctype.h>
#include <util.h>
#define NULL 0

int BE2LinePromoteInit(refstate)
struct LinePromState **refstate;
{/*		Returns < 0 for (malloc) failure, 0 for OK.  Initializes *refstate
		to point to malloc'ed storage that will hold the LinePromote state.
*/
    struct LinePromState *lps;

    lps = (struct LinePromState *) malloc(sizeof(struct LinePromState));
    if (lps == NULL) return -1;
    lps->BeginDataLevel = 0;
    lps->InDefine = 0;
    lps->Promoting = 2;
    lps->SawBlankLine = 0;
    *refstate = lps;
    return 0;
}

int BE2LinePromote(line, state)
char *line; struct LinePromState *state;
{/*		*Works only on BE2 Datastream messages*
		Returns 2 if this line (including the newline) should be
		``promoted'' from the beginning of a message to the
		beginning of an error message that encapsulates the
		message.  Returns 0 if the line should stay where it is.
		Returns 1 if this line should be ``demoted'' to the very
		tail end of the encapsulated message.  Returns < 0
		on errors.
*/
    int LLen; char *CP;

    if (state->Promoting == 1) return 1;
    if (state->Promoting == 0) {
	if (*line == '\\') {
	    if (strncmp(line, "\\begindata{", 11) == 0) {
		++(state->BeginDataLevel);
	    } else if (strncmp(line, "\\enddata{", 9) == 0) {
		--(state->BeginDataLevel);
		if (state->BeginDataLevel == 0) {
		    state->Promoting = 1;
		    return 1;
		}
	    }
	}
	return 0;	/* Done promoting */
    }
    if (*line == '\0' || strcmp(line, "\n") == 0) {	/* Blank line */
	if (state->BeginDataLevel == 0) return 2;
	else if (state->SawBlankLine == 0) {
	    state->SawBlankLine = 1;
	    return 2;
	} else {
	    state->Promoting = 0;		/* a real extra blank line. */
	    return 0;
	}
    }
    if (state->InDefine) {
	LLen = strlen(line);
	CP = &line[LLen - 1];
	if (LLen > 0 && *CP == '\n') {--LLen; --CP;}
	while (LLen > 0 && isspace(*CP)) {--LLen; --CP;}
	if (LLen > 0 && *CP == '}') {
	    state->InDefine = 0;	/* End of the definition */
	    return 2;		/* but it should be promoted always */
	} else {
	    return 2;		/* Not yet end of defn.  Promote it. */
	}
    } else {
	if (*line != '\\') {state->Promoting = 0; return 0;}
	++line;
	if (!islower(*line)) {state->Promoting = 0; return 0;}
	if (strncmp(line, "begindata{", 10) == 0) {	/* promote only one of these */
	    ++(state->BeginDataLevel);
	    if (state->BeginDataLevel != 1) {state->Promoting = 0; return 0;}
	    return 2;
	} else if (strncmp(line, "enddata{", 8) == 0) {
	    --(state->BeginDataLevel);
	    state->Promoting = 0;
	    return 0;
	} else if (strncmp(line, "define{", 7) == 0) {
	    state->InDefine = 1;
	    return 2;
	} else {
/*
The BE2 group, in the person of Maria Wadlow, says, in January of 1988, that the only four promoted things are:
	\begindata{...
	\textdsversion{...
	\define{...
	\template{
and, in fact, that only the first \begindata should be promoted.  This could be an opening ``\smaller{'' or something--something that shouldn't be promoted as a header.
*/
	    state->Promoting = 0;
	    return 0;
	}
    }
}

int BE2LinePromoteEnd(state)
struct LinePromState *state;
{/*		Cleans up the malloc'ed storage and returns 0 if OK,
		non-zero on errors.
*/
    if (state != NULL) free(state);
    return 0;
}
