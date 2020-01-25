/* $XConsortium: xkbcomp.c,v 1.3 94/04/08 15:29:47 erik Exp $ */
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

#include <stdio.h>
#include <ctype.h>
#include <X11/keysym.h>

#if defined(sgi)
#include <malloc.h>
#endif

#define	DEBUG_VAR_NOT_LOCAL
#define	DEBUG_VAR debugFlags
#include "xkbcomp.h"
#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#endif
#include "xkbfile.h"
#include "tokens.h"

#define	lowbit(x)	((x) & (-(x)))

/***====================================================================***/

#define	WANT_DEFAULT	0
#define	WANT_XKM_FILE	1
#define	WANT_C_HDR	2
#define	WANT_XKB_FILE	3

static char *fileTypeExt[] = {
	"XXX",
	"xkm",
	"h",
	"xkb"
};

static	unsigned	type;
static	char *		inputFile;
static	char *		outputFile;
static	char *		mapName;

	unsigned	warningLevel= 5;

/***====================================================================***/

void
Usage(argc,argv)
    int 	argc;
    char *	argv[];
{
    fprintf(stderr,"Usage: %s [options] file\n",argv[0]);
    fprintf(stderr,"Legal options:\n");
    fprintf(stderr,"-I[<dir>]   Specifies a top level directory\n");
    fprintf(stderr,"            for include directives.  You can\n");
    fprintf(stderr,"            specify multiple directories.\n");
    fprintf(stderr,"-o <file>   Specifies output file name\n");
    fprintf(stderr,"-m <name>   Specifies map to compile from a multi-map\n");
    fprintf(stderr,"            directory file\n");
    fprintf(stderr,"-C          Produce a C header file\n");
    fprintf(stderr,"-xkm        Produce an xkm (compiled X Key Map) file\n");
    fprintf(stderr,"-xkb        Produce an xkb (X KeyBoard map Source) file\n");
    fprintf(stderr,"-w <lvl>    Set warning level (0=none, 10=all)\n");
#ifdef DEBUG
    fprintf(stderr,"-d [flags]  Report debugging information\n");
#endif
    exit(1);
}

/***====================================================================***/

Bool
parseArgs(argc,argv)
    int		argc;
    char *	argv[];
{
register int i;

    for (i=1;i<argc;i++) {
	if (argv[i][0]!='-') {
	    if (inputFile!=NULL) {
		uWarning("Multiple source files specified\n");
		uAction("Compiling %s, ignoring %s\n",inputFile,argv[i]);
	    }
	    else inputFile= argv[i];
	}
	else if (strncmp(argv[i],"-I",2)==0) {
	    if (!AddDirectoryToPath(&argv[i][2])) {
		uAction("Exiting\n");
	    }
	    exit(1);
	}
	else if (strcmp(argv[i],"-o")==0) {
	    if (++i>=argc) {
		uWarning("No output file specified\n");
		uAction("Trailing \"-o\" option ignored\n");
	    }
	    else if (outputFile!=NULL) {
		uWarning("Multiple output files specified\n");
		uAction("Compiling %s, ignoring %s\n",outputFile,argv[i]);
	    }
	    else outputFile= argv[i];
	}
	else if (strcmp(argv[i],"-m")==0) {
	    if (++i>=argc) {
		uWarning("No map name specified\n");
		uAction("Trailing \"-m\" option ignored\n");
	    }
	    else if (mapName!=NULL) {
		uWarning("Multiple map names specified\n");
		uAction("Compiling %s, ignoring %s\n",mapName,argv[i]);
	    }
	    else mapName= argv[i];
	}
	else if (strcmp(argv[i],"-C")==0) {
	    if ((type!=WANT_DEFAULT)&&(type!=WANT_C_HDR)) {
		uWarning("Multiple output file types specified\n");
		uAction("\"%s\" flag ignored\n",argv[i]);
	    }
	    else type= WANT_C_HDR;
	}
	else if (strcmp(argv[i],"-xkm")==0) {
	    if ((type!=WANT_DEFAULT)&&(type!=WANT_XKM_FILE)) {
		uWarning("Multiple output file types specified\n");
		uAction("\"%s\" flag ignored\n",argv[i]);
	    }
	    else type= WANT_XKM_FILE;
	}
	else if (strcmp(argv[i],"-xkb")==0) {
	    if ((type!=WANT_DEFAULT)&&(type!=WANT_XKB_FILE)) {
		uWarning("Multiple output file types specified\n");
		uAction("\"%s\" flag ignored\n",argv[i]);
	    }
	    else type= WANT_XKB_FILE;
	}
	else if (strcmp(argv[i],"-w")==0) {
	    if ((i>=(argc-1))||(!isdigit(argv[i+1][0]))) {
		warningLevel= 0;
	    }
	    else {
		sscanf(argv[++i],"%i",&warningLevel);
	    }
	}
#ifdef DEBUG
	else if (strcmp(argv[i],"-d")==0) {
	    if ((i>=(argc-1))||(!isdigit(argv[i+1][0]))) {
		debugFlags= 1;
	    }
	    else {
		sscanf(argv[++i],"%i",&debugFlags);
	    }
	    uInformation("Setting debug flags to %d\n",debugFlags);
	}
#endif
	else {
	    if (strcmp(argv[i],"help")!=0) {
		uError("Unknown flag \"%s\" on command line\n",argv[i]);
	    }
	    return False;
	}
    }
    if (inputFile==NULL) {
	uError("No input file specified\n");
	return False;
    }
    if (type==WANT_DEFAULT)
	type= WANT_XKM_FILE;
    if (outputFile==NULL) {
	int len;
	char *base;
	base= strrchr(inputFile,'/');
	if (base==NULL)	base= inputFile;
	else		base++;

	len= strlen(base)+strlen(fileTypeExt[type])+2;
	outputFile= uTypedCalloc(len,char);
	if (outputFile==NULL) {
	    uInternalError("Couldn't allocate space for output file name\n");
	    uAction("Exiting\n");
	    exit(1);
	}
	sprintf(outputFile,"%s.%s",base,fileTypeExt[type]);
    }
    return True;
}

/***====================================================================***/

#define MAX_INCLUDE_OPTS	10
static char	*includeOpt[MAX_INCLUDE_OPTS];
static int	numIncludeOpts = 0;

int
main(argc,argv)
    int		argc;
    char *	argv[];
{
FILE 	*	file;
XkbFile	*	rtrn;
int		ok;
XkbFileInfo 	result;

    if (!parseArgs(argc,argv)) {
	Usage(argc,argv);
	/* NOTREACHED */
    }
#ifdef DEBUG
    if (debugFlags&0x2) {
	extern int yydebug;
	yydebug= 1;
    }
#ifdef sgi
    if (debugFlags&0x4)
	mallopt(M_DEBUG,0xff);
#endif
#endif
    stInit(200,10,False,NULL);
    InitIncludePath();

    file= fopen(inputFile,"r");
    if (file) {
	ok= True;
	setScanState(inputFile,1);
	if (XKBParseFile(file,&rtrn)&&(rtrn!=NULL)) {
	    fclose(file);
	    bzero((char *)&result,sizeof(result));
	    result.dpy= NULL;	/* for now */
	    result.type= rtrn->type;
	    switch (rtrn->type) {
		case XkmSemanticsFile:
		case XkmLayoutFile:
		case XkmKeymapFile:
		    ok= CompileKeymap(rtrn,&result,MergeReplace);
		    break;
		case XkmKeyNamesIndex:
		    ok= CompileKeycodes(rtrn,&result,MergeReplace);
		    break;
		case XkmTypesIndex:
		    ok= CompileKeyTypes(rtrn,&result,MergeReplace);
		    break;
		case XkmSymbolsIndex:
		    uError("Symbols files cannot be compiled on their own\n");
		    uAction("You must include them in other files\n");
		    ok= False;
		    break;
		case XkmCompatMapIndex:
		    ok= CompileCompatMap(rtrn,&result,MergeReplace);
		    break;
		case XkmGeometryFile:
		case XkmGeometryIndex:
		    uInternalError("Geometry file not supported yet\n");
		    ok= False;
		    break;
		case XkmAlternateSymsFile:
		    uInternalError("Alternate Syms file not supported yet\n");
		    ok= False;
		    break;
		default:
		    uInternalError("Unknown file type %d\n",rtrn->type);
		    ok= False;
		    break;
	    }
	    if (ok) {
		switch (type) {
		    case WANT_XKM_FILE:
			ok= XkbWriteXKMFile(outputFile,&result);
			break;
		    case WANT_XKB_FILE:
			ok= XkbWriteXKBFile(outputFile,&result);
			break;
		    case WANT_C_HDR:
			ok= XkbWriteCFile(outputFile,&result);
			break;
		    default:
			uInternalError("Unknown output file type %d\n",type);
			uAction("No output file created\n");
			ok= False;
			break;
		}
	    }
	}
    }
    else {
	fprintf(stderr,"Couldn't open \"%s\" to compile\n",inputFile);
	ok= 0;
    }
    return (ok==0);
}
