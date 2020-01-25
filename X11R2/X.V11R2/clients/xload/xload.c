#ifndef lint
static char rcsid[] = "$Header: xload.c,v 1.10 88/02/21 16:24:58 swick Exp $";
#endif  lint

#include <X11/Intrinsic.h>
#include <X11/Xatom.h>
#include <X11/Atoms.h>
#include <X11/Shell.h>
#include <X11/Load.h>
#include <stdio.h> 
#include "xload.bit"

extern void exit();

/* Command line options table.  Only resources are entered here...there is a
   pass over the remaining options after XtParseCommand is let loose. */

static XrmOptionDescRec options[] = {
{"-scale",	"*load.minScale",	XrmoptionSepArg,	 NULL},
{"-update",	"*load.update",		XrmoptionSepArg,	 NULL},
};


/* Exit with message describing command line format */

void usage()
{
    fprintf(stderr,
"usage: xload [-fn {font}] [-update {seconds}] [-scale {integer}] [-rv]\n"
);
    fprintf(stderr,
"             [-geometry [{width}][x{height}][{+-}{xoff}[{+-}{yoff}]]] [-display [{host}]:[{vs}]]\n"
);
    fprintf(stderr,
"             [-fg {color}] [-bg {color}] [-bd {color}] [-bw {pixels}]\n");
    exit(1);
}


void main(argc, argv)
    int argc;
    char **argv;
{
    char host[256];
    Widget toplevel;
    Arg arg;
    
    (void) gethostname(host,255);
    toplevel = XtInitialize("main", "XLoad", options, XtNumber(options), &argc, argv);
      
    if (argc != 1) usage();
    
    XtSetArg (arg, XtNiconPixmap, 
	      XCreateBitmapFromData (XtDisplay(toplevel),
				     XtScreen(toplevel)->root,
				     xload_bits, xload_width, xload_height));
    XtSetValues (toplevel, &arg, 1);

    XtSetArg (arg, XtNlabel, host);
    XtCreateManagedWidget ("load", loadWidgetClass, toplevel, &arg, 1);
    XtRealizeWidget (toplevel);
    XtMainLoop();
}
