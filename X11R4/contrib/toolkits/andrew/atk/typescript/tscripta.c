/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/typescript/RCS/tscripta.c,v 2.10 89/10/09 13:48:59 tpn Exp $ */
/* $ACIS:tscripta.c 1.5$ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/typescript/RCS/tscripta.c,v $ */

#ifndef lint
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/typescript/RCS/tscripta.c,v 2.10 89/10/09 13:48:59 tpn Exp $";
#endif /* lint */

#include <class.h>
#include <tscripta.eh>
#include <app.ih>
#include <tscript.ih>
#include <style.ih>
#include <frame.ih>
#include <im.ih>
#include <event.ih>
#include <environ.ih>
#include <fontdesc.ih>

#define DOSOMETHINGINTERVAL 30 /* Every 30 seconds, do something... */

/* A nonsubtle hint to the OS that we would like to occupy some memory... */
static void DontSwapMeOut()
{

    im_EnqueueEvent(DontSwapMeOut, 0, event_SECtoTU(DOSOMETHINGINTERVAL));
}

boolean typescriptapp__ParseArgs(self,argc,argv)
struct typescriptapp *self;
int argc;
char **argv;
{
    if(!super_ParseArgs(self,argc,argv))
	return FALSE;

#define GETARGSTR(var)\
{\
    if((*argv)[2]!='\0')\
        var= ((*argv)[2]=='=' ? &(*argv)[3] : &(*argv)[2]);\
    else if(argv[1]==NULL){\
	fprintf(stderr,"%s: %s switch requires an argument.\n",typescriptapp_GetName(self),*argv);\
        return FALSE;\
    }else\
    	var= *++argv;\
}

    while(*++argv!=NULL && **argv=='-'){
	switch((*argv)[1]){
            case 's':
                self->ShellMenu=TRUE;
                break;
	    case 't':
		GETARGSTR(self->titleLine);
		break;
	    case 'F':
		self->filemenu=TRUE;
		break;
	    case 'm':
		self->ShellMenu=TRUE;
		GETARGSTR(self->Menu);
		break;
	    case 'f':
		GETARGSTR(self->font);
		break;
	    case 'p':
		/* obsolete */
		break;
	    default:
		fprintf(stderr,"%s: unrecognized switch: %s\n", typescriptapp_GetName(self), *argv);
		return FALSE;
	}
    }

    self->argv=argv;
    if(*argv==NULL)
	self->ShellMenu=TRUE;

    return TRUE;
}

boolean typescriptapp__InitializeObject(classID,self)
struct classheader *classID;
struct typescriptapp *self;
{
    typescriptapp_SetName(self, "typescript");
    self->argv=NULL;
    self->filemenu = FALSE;
    self->titleLine = NULL;
    self->font = NULL;
    self->ShellMenu = FALSE;
    self->ss = NULL;
    self->Menu = NULL;
    typescriptapp_SetMajorVersion(self, 7);
    typescriptapp_SetMinorVersion(self, 2);
    return TRUE;
}

boolean typescriptapp__Start(self)
struct typescriptapp *self;
{
    struct view *vs;
    struct im *im;
    char nbuf[256];
    FILE *df = NULL;
    char *home;

    if(!super_Start(self))
	return FALSE;

    if (self->Menu == NULL || (df = fopen(self->Menu, "r")) == NULL)  {
	/*
	 Uses the default shmenu if either the user has not specified a menu file
	     or the menu file specified could not be read.
	     */

	if (self->Menu != NULL)
	    perror(self->Menu);
	if (self->ShellMenu) {
	    home = environ_Get("HOME");
	    if (home != NULL)  {
		sprintf (nbuf, "%s/.shmenu", home);
		df = fopen (nbuf, "r");
	    }
	    if (df == NULL)  {
		char *fileName;

		fileName = environ_AndrewDir("/lib/shmenu");
		df = fopen (fileName, "r");
	    }
	}
    }
    self->ss = typescript_Create(self->argv,df,self->filemenu);
    if (df != NULL)
	fclose(df);

    if (self->font != NULL) {

        char family[256];
        long size, style;
        struct style *defaultStyle = typescript_GetDefaultStyle(self->ss);

        if (defaultStyle == NULL)
            defaultStyle = style_New();
        if (fontdesc_ExplodeFontName(self->font, family, sizeof(family), &style, &size)) {
            style_SetFontFamily(defaultStyle, family);
            style_SetFontSize(defaultStyle, style_ConstantFontSize, size);
            style_ClearNewFontFaces(defaultStyle);
            style_AddNewFontFace(defaultStyle, style);
            typescript_SetDefaultStyle(self->ss, defaultStyle);
        }
    }
    vs = typescript_GetApplicationLayer(self->ss);
    self->frame = frame_New();
    im = im_Create(NULL);
    frame_SetView(self->frame,vs);
    im_SetView(im, self->frame);
    if (self->titleLine != NULL)
	frame_SetTitle(self->frame, self->titleLine);
    typescript_WantInputFocus(self->ss,self->ss);
    im_PostDefaultHandler(im, "message",
                           self->frame->messageLine);

    DontSwapMeOut(); /* Start eviction protection measures. */

    return TRUE;
}
