/* $XConsortium: xkbpath.c,v 1.1 94/04/08 15:49:55 erik Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#include <X11/Xlib.h>

#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include "utils.h"
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#endif

#ifndef DFLT_XKB_CONFIG_ROOT
#define DFLT_XKB_CONFIG_ROOT	"/usr/lib/X11/xkb"
#endif

#ifndef PATH_MAX
#define	PATH_MAX 1024
#endif

#define	PATH_CHUNK	8

static	int	 longestPath;
static	int	 szPath;
static	int	 nPathEntries;
static	char **	 includePath;

Bool
InitIncludePath()
{
    szPath= PATH_CHUNK;
    includePath=  uTypedCalloc(szPath,char *);
    if (includePath==NULL)
	return False;
    AddDirectoryToPath(".");
    AddDirectoryToPath(DFLT_XKB_CONFIG_ROOT);
}

void
ClearIncludePath()
{
register int i;

    if (szPath>0) {
	for (i=0;i<nPathEntries;i++) {
	    if (includePath[i]!=NULL) {
		uFree(includePath[i]);
		includePath[i]= NULL;
	    }
	}
	nPathEntries= 0;
	longestPath= 0;
    }
    return;
}

Bool
AddDirectoryToPath(dir)
    char	*dir;
{
int	len;
    if ((dir==NULL)||(dir[0]=='\0')) {
	ClearIncludePath();
	return True;
    }
    len= strlen(dir);
    if (len+2>=PATH_MAX) { /* allow for '/' and at least one character */
	uError("Path entry (%s) too long (maxiumum length is %d)\n",
							dir,PATH_MAX-3);
	return False;
    }
    if (len>longestPath)
	longestPath= len;
    if (nPathEntries>=szPath) {
	szPath+= PATH_CHUNK;
	includePath= uTypedRealloc(includePath,szPath,char *);
	if (includePath==NULL) {
	    uInternalError("Allocation failed (includePath)\n");
	    return False;
	}
    }
    includePath[nPathEntries]= uTypedCalloc(strlen(dir)+1,char);
    if (includePath[nPathEntries]==NULL) {
	uInternalError("Allocation failed (includePath[%d])\n",nPathEntries);
	return False;
    }
    strcpy(includePath[nPathEntries++],dir);
    return True;
}

FILE *
FindFileInPath(name,type,pathRtrn)
    char *	name;
    char *	type;
    char **	pathRtrn;
{
register int i;
FILE	*file;
int	 nameLen,typeLen,pathLen;
char	 buf[PATH_MAX];

    nameLen= strlen(name);
    typeLen= strlen(type);
    for (i=0;i<nPathEntries;i++) {
	pathLen= strlen(includePath[i]);
	if ((nameLen+pathLen+1)>=PATH_MAX) {
	    uError("File name (%s/%s) too long\n",includePath[i],name);
	    uAction("Ignored\n");
	    continue;
	}
	sprintf(buf,"%s/%s",includePath[i],name);
	file= fopen(buf,"r");
	if (file!=NULL)
	     break;
	else if (typeLen<1)
	     continue;

	if ((nameLen+typeLen+pathLen+2)>=PATH_MAX) {
	    uError("File name (%s/%s/%s) too long\n",includePath[i],type,name);
	    uAction("Ignored\n");
	    continue;
	}
	sprintf(buf,"%s/%s/%s",includePath[i],type,name);
	file= fopen(buf,"r");
	if (file!=NULL)
	    break;
    }

    if ((file!=NULL)&&(pathRtrn!=NULL)) {
	*pathRtrn= uTypedCalloc(strlen(buf)+1,char);
	if (*pathRtrn!=NULL)
	    strcpy(*pathRtrn,buf);
    }
    return file;
}

