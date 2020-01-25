/* $XConsortium: xrecord.c,v 1.6 94/03/25 16:43:31 dpw Exp $ */
/***************************************************************************
 * Copyright 1994 Network Computing Devices; 
 * Portions Copyright 1988 by Digital Equipment Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices or Digital 
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 **************************************************************************/

#define ProgName "xrecord"  
  
/*  
**
**  FACILITY:  xrecord - Sample client to test output from Record extension
**
**  AUTHOR:
**
**      Martha Zimet
**	NCD
**
**  CREATION DATE:  22 November 1993
**
*/

#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h> 
#include <X11/Xmd.h>
#include "recordstr.h" 
#include "record.h"

/*  Condition handling macros */
#ifndef _InitExceptionHandling
#define _InitExceptionHandling(rtn)                       \
     (void)signal(SIGBUS,  rtn); /* Bus error */                 \
     (void)signal(SIGSEGV, rtn); /* Accvio/Segment error */      \
     (void)signal(SIGFPE,  rtn); /* Floating point exception */  \
     (void)signal(SIGILL,  rtn); /* Illegal instruction */       \
     (void)signal(SIGSYS,  rtn); /* Param error in sys call */   \
     (void)signal(SIGINT,  rtn);                                 \
     (void)signal(SIGTERM, rtn)
#endif /* _InitExceptionHandling */
 
#ifndef _ClearExceptionHandling
#define _ClearExceptionHandling() \
    (void)signal(SIGBUS,  SIG_DFL); /* Bus error */                 \
    (void)signal(SIGSEGV, SIG_DFL); /* Accvio/Segment error */      \
    (void)signal(SIGFPE,  SIG_DFL); /* Floating point exception */  \
    (void)signal(SIGILL,  SIG_DFL); /* Illegal instruction */       \
    (void)signal(SIGSYS,  SIG_DFL); /* Param error in sys call */   \
    (void)signal(SIGINT,  SIG_DFL);                                 \
    (void)signal(SIGTERM, SIG_DFL)
#endif /* _ClearExceptionHandling */

char *ProgramName;

extern void Dispatch();

Display *dpy1, *dpy2;          /* 
				** X connections: 
      				**  	dpy1 is control connection 
			        **  	dpy2 is data connection 
				*/
XRecordConfig			rec_config1; 
XRecordFlags                    *rec_flags1 = NULL;

Bool GlobalDone = False;
static void SetGlobalDone()
{
    GlobalDone = 1L;
    fprintf(stderr,"Process Completed!\n");
    (void)XRecordDisableCG(dpy2, rec_config1);
    XSync(dpy2, False);

    XRecordFreeCG(dpy1, rec_config1);   
    XCloseDisplay(dpy1); 
    XCloseDisplay(dpy2);

    free(rec_flags1);
    
    exit(0L); 
}

XID parse_id (s)
    char *s;
{
    XID retval = None;
    char *fmt = "%ld";	/* since XID is long */

    if (s) {
	if (*s == '0') s++, fmt = "%lo";
	if (*s == 'x' || *s == 'X') s++, fmt = "%lx";
	sscanf (s, fmt, &retval);
    }
    return (retval);
}

static void usage ()
{
    static char *help[] = {
"    -help                  print this message", 
"    -display displayname   X server to contact",
"    -id <id_base>          XID base of client to record (default is key client)", 
"", NULL };

    char **cpp;

    fprintf (stderr, "usage:\n        %s [-options ...]\n\n",
	     ProgramName);
    fprintf (stderr, "where options include:\n");
    for (cpp = help; *cpp; cpp++) {
	fprintf (stderr, "%s\n", *cpp);
    }
    exit (1);
}


main(argc,argv)
    int argc;
    char *argv[];
{
    XID		id = None;
    char 	*displayname = NULL;      /* server to contact */
    int 	i;
    Bool	async = False;

    XRecordQueryVersionReply 	ret;

    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {

	if (!strcmp(argv[i], "-help")) {
	    usage();
	}
	if(!strcmp(argv[i], "-display")){
            if (++i >= argc) 
	        usage();
            displayname = argv[i];
	    continue;
	}
        if(!strcmp(argv[i], "-id")) {
            if (++i >= argc)
	        usage();
            id = parse_id (argv[i]);
            continue;
        }
        if(!strcmp(argv[i], "-async")) {
	    async = True;
            continue;
        }
        usage(); 
     }

    dpy1 = XOpenDisplay (displayname);
    if (!dpy1) {
        fprintf (stderr, "%s:  unable to open display \"%s\".\n",
                 ProgramName, XDisplayName (displayname));
        exit (1);
    }
    dpy2 = XOpenDisplay (displayname);
    if(!dpy2) 
    {
         fprintf (stderr, "%s:  unable to open display \"%s\".\n",
                  ProgramName, XDisplayName (displayname));
          exit (1);
     }

    fprintf(stderr,"Display:  %s \n", DisplayString(dpy1));
    XSynchronize(dpy1, True);
   
    if(!XRecordQueryVersion(dpy1, 1, 0, &ret) ) 
    {
    	exit(0);
    }
    fprintf(stderr,"Available XRecord Information: \n"); 
    fprintf(stderr,"\tRelease: %d.%d\n", ret.majorVersion, ret.minorVersion); 

   /* Event names.  Used in "type" field in XEvent structures.
   They start from 2 because 0 and 1 are reserved in the protocol 
   for errors and replies. */

    rec_flags1 = (XRecordFlags* )malloc(sizeof(XRecordFlags)); 
    rec_config1 = XRecordCreateCG(dpy1);

    rec_flags1->events.first = X_Error; 
    rec_flags1->events.last = MappingNotify;  
    rec_flags1->core_requests.first = X_CreateWindow;
    rec_flags1->core_requests.last = X_NoOperation;
    rec_flags1->core_replies.first = X_CreateWindow;
    rec_flags1->core_replies.last = X_NoOperation;
    rec_flags1->errors.first = BadRequest;
    rec_flags1->errors.last = BadImplementation; 
    rec_flags1->ext_requests.ext_major = 0;
    rec_flags1->ext_requests.ext_minor.first = 0;
    rec_flags1->ext_requests.ext_minor.last = 0;
    rec_flags1->ext_replies.ext_major = 0;
    rec_flags1->ext_replies.ext_minor.first = 0;
    rec_flags1->ext_replies.ext_minor.last = 0;

    if (!id)
	id = 0x400000;
    XRecordChangeCG(dpy1, rec_config1, id, rec_flags1, True);
    XSync(dpy1, False);
          
    _InitExceptionHandling((XRecord_void_function)SetGlobalDone); 

    if (async) {
	XRecordEnableCGAsync(dpy2, rec_config1, Dispatch, NULL);
	XSync(dpy2, False);
    } else
	XRecordEnableCG(dpy2, rec_config1, Dispatch, NULL);

    _ClearExceptionHandling(); 
  
    exit(0);
}

