/* fgetstr.c : get from FILE* a string not containing TERMINATORS */

#if ! defined(__STDC__)
#define const
#define volatile
#define register
#endif

static const char *Version[] = {
	"@(#) fgetstr.c Copyright (c) 1992 Christopher Alexander North-Keys",
	"$Grueppe: Talismangrueppe $",
	"$Anfang: Sat Aug  8 00:19:24 GMT 1992 $",
#if defined(__STDC__)
	"$Compiliert: "__DATE__" " __TIME__ " $",
#endif
	"$Source: /home/erlkonig/hub/Lib/fgetstr.c $",
	"$Revision: $",
	"$Date: Sat Aug  8 00:19:24 GMT 1992 $",
	"$Author: erlkonig $",
	(char*)0
	};

#include "fgetstr.h"

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#ifdef TEST
#define DEBUG
#endif

#ifdef DEBUG
#undef DEBUG
#define DEBUG(b) {b}
#else
#define DEBUG(b)  
#endif

/*------- debugging ``main'' -------*/
#ifdef TEST
#if defined(__STDC__)
int main (int argc, char **argv)
#else
int main(argc, argv)
int argc;
char **argv;
#endif
{
	void *a = malloc(5*sizeof(char));
	void *b = malloc(5*sizeof(char));
	char *str = (char*)0;
	
	free(a);
	str = Fgetstr (stdin, (argc > 1) ? argv[1] : "\n");
	puts (str ? str : "<null>");
	free(b);
	if(str)free(str);
	
	return 0;
}
#endif

/*	return specifications - (acually ripped out of a different implementation)
 *
 *	terminators include : ends, \0, and EOF
 *
 *	root	EOF?	text?	ended? 	stat	returned value
 *			-		-		-		...	
 *	1		-		-		1		return	""
 *			-		1		-		...	
 *	2		-		1		1		return	"text"
 *	3		1		-		-		return	-null-		EOF-*accepted*
 *	4		1		-		1		return	""			EOF-postponed
 *	5		1		1		-		return	"text"		EOF-postponed/fake-end
 *	6		1		1		1		return	"text"		EOF-postponed/true-end
 *
 *	on ENOMEM, return -null-
 *
 */

#if defined(__STDC__)
char *Fgetstr(FILE *ifp, const char *terminators)
#else
char *Fgetstr(ifp, terminators)
FILE *ifp;
char *terminators;
#endif
{
	char *string     = (char*)0;
	size_t chunksize = 1;		/* initial size */
    long  index      = 0;
	
	string = (char*)malloc(chunksize * sizeof(char));	/* initial alloc */

	for (index = 0 ; string ; index++)		/* note: revalidates string */
	{
		/* insure string large enough for next char (eventually '\0') */
		if (chunksize < (index + 1))
		{
	   	    string = (char*)realloc((void*)string,
									(chunksize *= 2)*sizeof(char));
			DEBUG(printf("\n[realloc: req-index-> %d, bufchrs-> %d @ 0x%x]\n ",
						 index, chunksize, string););
		}
		if (string)							/* verify realloc's */
		{
			int c = 0;
			c = getc(ifp);

			DEBUG(printf("\t%d/%d:%c", index, chunksize-1, (char)c););
			if ( c && (EOF != c) &&
				(terminators ? !strchr(terminators, c) : 1))
				string[index] = (char)c;			/* unterminated */
			else
			{										/* terminated */
				if((0 == index) && (EOF == c))
				{
					free(string);
					return (char*)0;
				}
				else
				{
					char *newstring = (char*)0;
					string[index] = '\0';
					if(newstring = (char*)realloc((void*)string,
											   (index + 1) * sizeof(char)))
					{
						DEBUG(printf("\n[trm'd: idx %d bytes %d resze 0x%x]\n",
									 index, index+1, string););
						return newstring;		/* cool!  done. */
					}
					else
					{
						DEBUG(printf("\n[trm'd: idx %d bytes %d resze fail]\n",
									 index, index+1, string););
						return string;			/* now how could this fail? */
					}
				}
			}
		}
	}
	perror ("Fgetstr");
	return (char*)0;
}
