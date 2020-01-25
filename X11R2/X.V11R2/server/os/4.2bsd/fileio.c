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
/* $Header: fileio.c,v 1.10 88/02/01 07:53:45 rws Exp $ */
#include <stdio.h>
#include "os.h"
#include "osdep.h"

#ifdef COMPRESSED_FONTS
#include <sys/types.h>
#include <sys/file.h>

#ifndef UNCOMPRESS
#define UNCOMPRESS "uncompress"
#endif
#ifndef ZSUFFIX
#define ZSUFFIX ".Z"
#endif
#define ZSUFLEN (sizeof(ZSUFFIX)-1)

extern int CompressedFonts;
#endif

/*
 *   FiOpenForRead, FiRead, FiClose,
 */

FID
FiOpenForRead( lenn, name)
    int		lenn;
    char 	*name;
{
    FID		fid;
    char	*unixfname = name;

#ifdef COMPRESSED_FONTS
    int  fontWasCompressed = 0;
    char commandBuf[256];
    char *tmpFileName, *tmpPtr;
#endif

    if ( name[lenn-1] != '\0')
    {
	unixfname = (char *) ALLOCATE_LOCAL( lenn+1);
	strncpy( unixfname, name, lenn);
	unixfname[lenn] = '\0';
    }
    else
	unixfname = name;

#ifdef COMPRESSED_FONTS
    if (CompressedFonts) {
	for (tmpPtr=unixfname; *tmpPtr; tmpPtr++) ;
	tmpPtr -= ZSUFLEN;

	if (strcmp(tmpPtr,ZSUFFIX) == 0) {
	    fontWasCompressed = 1;
	    tmpFileName = (char *) ALLOCATE_LOCAL(256);

	    /************************************/
	    /** build the name of the tmp file **/
	    /************************************/
	    tmpPtr=(char *)rindex(unixfname,'/');
	    strcpy(tmpFileName,"/tmp");
	    strcat(tmpFileName,tmpPtr);
	    tmpFileName[strlen(tmpFileName)-ZSUFLEN] = 0; /* nuke the .Z */

	    /***************************************************/
	    /** build the command line to uncompress the file **/
	    /***************************************************/
	    sprintf(commandBuf, "%s < %s > %s",
		    UNCOMPRESS, unixfname, tmpFileName);

	    system(commandBuf);

	    if (access(tmpFileName, R_OK) == 0) {
		DEALLOCATE_LOCAL(unixfname);
		unixfname = tmpFileName;
	    }
	}
    }
#endif

    fid = (FID)fopen( unixfname, "r");

#ifdef COMPRESSED_FONTS
    if (fontWasCompressed)
	unlink(unixfname);
#endif

    if(unixfname != name)
        DEALLOCATE_LOCAL(unixfname);
    return fid;
}


/*
 * returns 0 on error
 */
int
FiRead( buf, itemsize, nitems, fid)
    char	*buf;
    unsigned	itemsize;
    unsigned	nitems;
    FID		fid;
{
    return fread( buf, itemsize, nitems, (FILE *)fid);
}

int
FiClose(fid)
    FID		fid;
{
    return fclose( (FILE *)fid);
}

