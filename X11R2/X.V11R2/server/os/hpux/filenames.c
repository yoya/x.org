/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header: filenames.c,v 1.5 88/02/01 18:52:50 rws Exp $ */

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/dir.h>
#include <ctype.h>
#ifdef hpux
#include <string.h>
#else
#include <strings.h>
#endif
#include "misc.h"
#include "resource.h"
#include "osstruct.h"
#include "X.h"
#include "Xmd.h"
#include "opaque.h"

extern char *defaultFontPath;

#ifdef COMPRESSED_FONTS
#ifndef ZSUFFIX
#define ZSUFFIX ".Z"
#endif
#define ZSUFLEN (sizeof(ZSUFFIX)-1)

extern int CompressedFonts;
#endif

/* The fontSearchList should contain zero paths to start with. */
static FontPathRec fontSearchList = { 0, 0, 0};

/*
 * description
 *	DDX interface routines
 *
 * caveats:
 *	These are OS-dependent to the extent that they assume pathnames contain
 *	no NULL characters.
 */


/*
 * This is called from main.c, not in response to a protocol request, so it
 * may take a null-terminated string as argument.
 */

void
SetDefaultFontPath( name)
    char *	name;
{
    int		len = strlen( name);
    int i;
/* If there is a previous fontSearchList, free the space that it contains. */

    for (i=0; i<fontSearchList.npaths; i++)
        xfree(fontSearchList.paths[i]);
    xfree(fontSearchList.paths);
    xfree(fontSearchList.length);

    if (name[len-1] != '/')
        len++;
    fontSearchList.npaths = 1;
    fontSearchList.length = (int *)xalloc(sizeof(int));
    fontSearchList.length[0] = len;
    fontSearchList.paths = (char **)xalloc(sizeof(char *));
    fontSearchList.paths[0] = (char *)xalloc(len + 1);
    bcopy(name, fontSearchList.paths[0], len);
    if (name[strlen(name)-1] != '/')
        fontSearchList.paths[0][len-1] = '/';
    fontSearchList.paths[0][len] = '\0';        
}


void
FreeFontRecord(pFP)
    FontPathPtr pFP;
{
    int i;
    for (i=0; i<pFP->npaths; i++)
        xfree(pFP->paths[i]);
    xfree(pFP->paths);
    xfree(pFP->length);
    xfree(pFP);
}

/*
 * pnames is a pointer to counted strings, each string long word
 * aligned
 */
void
SetFontPath( npaths, totalLength, countedStrings)
    unsigned	npaths;
    unsigned	totalLength;
    char *	countedStrings;
{
    int i;
    char * bufPtr = countedStrings;
    char n, len;

    if (npaths == 0)
    {
	SetDefaultFontPath(defaultFontPath); /* this frees old paths */
    }
    else
    {
        for (i=0; i<fontSearchList.npaths; i++)
	    xfree(fontSearchList.paths[i]);
	xfree(fontSearchList.paths);
	xfree(fontSearchList.length);

        fontSearchList.length = (int *)xalloc(npaths * sizeof(int));
        fontSearchList.paths = (char **)xalloc(npaths * sizeof(char *));
	for (i=0; i<npaths; i++)
        {
	    n = len = *bufPtr;
            if (bufPtr[n] != '/')
                len++;
	    fontSearchList.length[i] = len;
	    fontSearchList.paths[i] = (char *) xalloc(len + 1);
	    bcopy(bufPtr+1, fontSearchList.paths[i], n);
            if (bufPtr[n] != '/')
                fontSearchList.paths[i][n] = '/';
	    fontSearchList.paths[i][len] = '\0';
	    bufPtr += n + 1;
	}
	fontSearchList.npaths = npaths;
    }
}

FontPathPtr
GetFontPath()
{
    return( & fontSearchList);
}

static Bool
match( pat, string)
    register char	*pat;
    register char	*string;
{
    register int	ip;
    Bool		matched;

    for ( ip=0; pat[ip]!='\0'; ip++)
    {
        if ( pat[ip] == '*')
	{
	    matched = FALSE;
	    while (! matched )   /* find where pat & string start to match */
	    {
		if ( string[ip] == '\0')
		{
		    if (pat[ip+1] == '\0')
		        return TRUE;
		    break;
		}
		while ( ! (matched = match( &pat[ip+1], &string[ip])))
		{
		    string++;
		    if ( string[0] == '\0') 
		        return FALSE;
		}
	    }
            if (matched)
                return TRUE;
	}
        else if (string[ip] == '\0')
            return FALSE;
	else if (( pat[ip] != '?') && (pat[ip] != string[ip]))
	     return FALSE;
    }
    return TRUE;
}


/*
 * Make further assumption that '/' is the pathname separator.
 * Still assume NULL ends strings.
 */
/*
 * used by OpenFont
 *
 *  returns length of ppathname.
 *  may set *ppPathname to fname;
 */

int
ExpandFontName( ppPathname, lenfname, fname)
    char	**ppPathname;
    unsigned	lenfname;
    char	*fname;
{
    int len;

    len = TryToExpandFontName(ppPathname, lenfname, fname, 0);
#ifdef COMPRESSED_FONTS
    if (CompressedFonts && !len)
	len = TryToExpandFontName(ppPathname, lenfname, fname, 1);
#endif
    return(len);
}

int
TryToExpandFontName( ppPathname, lenfname, fname, zflag)
    char	**ppPathname;
    unsigned	lenfname;
    char	*fname;
    int		zflag;
{
    unsigned	in;
    char	*fullname = NULL;
    char	*lowername;
    char	*pch;
    int		extra = 5;

#ifdef COMPRESSED_FONTS
    if (zflag)
	extra += ZSUFLEN;
#endif

    lowername = (char *) xalloc(lenfname + extra);
    bzero(lowername, lenfname + extra);
    strncpy(lowername, fname, lenfname);
    /* if the name doesn't end in .snf, append that to the name */
    pch = lowername+lenfname;
    if ((lenfname < 4) ||
	(*(pch - 4) != '.') ||
	(*(pch - 3) != 's') ||
	(*(pch - 2) != 'n') ||
	(*(pch - 1) != 'f'))
    {
	strcat(lowername, ".snf");
	lenfname += 4;
    }


    /*
     * reduce to lower case only
     */
    for ( in=0; in<lenfname; in++)
	if ( isupper( lowername[in]))
	    lowername[in] = tolower( lowername[in]);
    
#ifdef COMPRESSED_FONTS
    /** append a .Z if necessary **/
    if (zflag) {
	strcat(lowername, ZSUFFIX);
	lenfname += ZSUFLEN;
    }
#endif

    if (lowername[0] == '/')
    {
	if ( access( lowername, R_OK) == 0)
	{
	    *ppPathname = lowername;
	    return (int)lenfname;
	}
    }
    else	
    {
	int		n, ifp;

	for ( ifp=0; ifp<fontSearchList.npaths; ifp++)
	{
	    fullname = (char *) xrealloc( fullname, 
			  n = fontSearchList.length[ifp] + lenfname + 1);
	    strcpy( fullname, fontSearchList.paths[ifp]);
	    strncat( fullname, lowername, lenfname);
	    fullname[n-1] = '\0';
	    if ( access( fullname, R_OK) == 0)
	    {
		*ppPathname = fullname;
		xfree(lowername);
		return strlen( fullname);
	    }
	}
    }
    xfree(lowername);
    xfree(fullname);
    *ppPathname = NULL;
    return 0;
}

static void
SearchDirectory(dname, pat, pFP, limit)
    char *dname;
    char *pat;
    FontPathPtr pFP;
    unsigned limit;
{
    DIR		*dp;
    register struct direct *nextdent;
    register char *fname;
    register int n, i;

    dp = opendir( dname);  
    if (dp == NULL)
        return ;
    i = pFP->npaths;
    while ( (nextdent = readdir( dp)) != NULL) 
    { 
   	if (i >= limit)  
	    break ;

	fname = nextdent->d_name;
	n = strlen(fname);
#ifdef COMPRESSED_FONTS
	if (n >= ZSUFLEN)
	{
	    register int j = ZSUFLEN;
	    while ((--j >= 0) && (fname[n-ZSUFLEN+j] == ZSUFFIX[j])) ;
	    if (j < 0)
		n -= ZSUFLEN;	   
	}
#endif
	if ((n <= 4) ||
	    (fname[n-4] != '.') ||
	    (fname[n-3] != 's') ||
	    (fname[n-2] != 'n') ||
	    (fname[n-1] != 'f'))
	    continue;
	n -= 4;
	fname[n] = '\0';
	

	if ( match( pat, fname))
	{  
	    pFP->length = (int *)xrealloc(pFP->length, (i+1)*sizeof(int));
	    pFP->paths = (char **)xrealloc(pFP->paths, (i+1)*sizeof(char *));  
	    pFP->length[i] = n;
	    pFP->paths[i] = (char *) xalloc(n);
	    bcopy(fname, pFP->paths[i], n);
	    i = ++pFP->npaths;
	}
    }
    closedir(dp);
}

/*******************************************************************
 *  ExpandFontPathPattern
 *
 *	Returns a FontPathPtr with at most max-names, of names of fonts
 *      matching
 *	the pattern.  The pattern should use the ASCII encoding, and
 *      upper/lower case does not matter.  In the pattern, the '?' character
 *	(octal value 77) will match any single character, and the character '*'
 *	(octal value 52) will match any number of characters.  The return
 *	names are in lower case.
 *
 *      Used only by protocol request ListFonts
 *******************************************************************/


FontPathPtr
ExpandFontNamePattern(lenpat, countedPattern, maxNames)
    unsigned	lenpat;
    char	*countedPattern;
    unsigned	maxNames;
{
    char	*pattern;
    long	i;
    FontPathPtr	fpr;


    fpr = (FontPathPtr)xalloc(sizeof(FontPathRec));
    fpr->npaths = 0;
    fpr->length = (int *)NULL;
    fpr->paths = (char **)NULL;

    /*
     * make a pattern which is guaranteed NULL-terminated
     */
    pattern = (char *) ALLOCATE_LOCAL( lenpat + 1);
    strncpy( pattern, countedPattern, lenpat);
    pattern[lenpat] = '\0';

    /*
     * find last '/' in pattern, if any
     */
    for ( i=((long)lenpat)-1; i>=0; i--)
	if ( pattern[i] == '/')
	    break;

    if ( i >= 0)		/* pattern contains its own dir prefix */
    {
	pattern[i] = '\0';	/* break pattern at the last path separator */
        SearchDirectory(pattern, &pattern[i+1], fpr, maxNames);
    }
    else
    {
        /*
	 * for each prefix in the font path list
	 */
	for ( i=0; i<fontSearchList.npaths; i++)
	{
	    SearchDirectory( fontSearchList.paths[i], pattern, fpr, maxNames);
	    if (fpr->npaths >= maxNames)
	        break;
	}
    }
    DEALLOCATE_LOCAL(pattern);
    return fpr;
}


