/*
 * Copyright (C) 1994 by Dave Glowacki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * to rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * DAVE GLOWACKI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * $Header: /usr/sww/share/src/X11R6/local/applications/xless-1.6/RCS/main.c,v 1.33 1994/06/03 05:14:27 dglo Exp $
 */

#include <stdio.h>

#include <X11/X.h>
#include <X11/Xos.h>

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include <X11/Xmu/Xmu.h>

#include "xless.h"
#include "XLessTop.icn"

#define XtRSearchType	"SearchType"

XtAppContext context;
Widget toplevel;
Display *disp;
const char *className;
const char *progname;

XLessResources resources;

static XtResource privResources[] = {
  {XtNgeometry, XtCGeometry, XtRString, sizeof(String),
     XtOffset(XLessResources *, geometry), XtRString, "80x24"},
  {XtNname, "Name", XtRString, sizeof(String),
     XtOffset(XLessResources *, name), XtRString, NULL},
  {XtNtitle, XtCTitle, XtRString, sizeof(String),
     XtOffset(XLessResources *, title), XtRString, NULL},
  {"helpFile", XtCFile, XtRString, sizeof(String),
     XtOffset(XLessResources *, helpFile), XtRString, HELPFILE},
  {"standardCur", XtCCursor, XtRCursor, sizeof(Cursor),
     XtOffset(XLessResources *, cursors.top), XtRString, STANDARDCUR},
  {"dialogCur", XtCCursor, XtRCursor, sizeof(Cursor),
     XtOffset(XLessResources *, cursors.dialog), XtRString, STANDARDCUR},
  {"standardFont", XtCFont, XtRFontStruct, sizeof(XFontStruct *),
     XtOffset(XLessResources *, fonts.standard), XtRString, STANDARDFONT},
  {"textFont", XtCFont, XtRFontStruct, sizeof(XFontStruct *),
     XtOffset(XLessResources *, fonts.text), XtRString, TEXTFONT},
  {"labelFont", XtCFont, XtRFontStruct, sizeof(XFontStruct *),
     XtOffset(XLessResources *, fonts.label), XtRString, LABELFONT},
  {"buttonFont", XtCFont, XtRFontStruct, sizeof(XFontStruct *),
     XtOffset(XLessResources *, fonts.button), XtRString, BUTTONFONT},
  {"editor", "Editor", XtRString, sizeof(String),
     XtOffset(XLessResources *, editor), XtRString, DEFEDITOR},
  {"editorDoesWindows", "EditorDoesWindows", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, editorDoesWindows), XtRString, "False"},
  {"printCommand", "PrintCommand", XtRString, sizeof(String),
     XtOffset(XLessResources *, printCmd), XtRString, PRINTCMD},
  {"printCmd", "PrintCommand", XtRString, sizeof(String),
     XtOffset(XLessResources *, printCmd), XtRString, PRINTCMD},
  {"maxWindows", "MaxWindows", XtRInt, sizeof(int),
     XtOffset(XLessResources *, maxWindows), XtRString, "0"},
  {"quitButton", "QuitButton", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, quitButton), XtRString, "False"},
  {"sizeToFit", "SizeToFit", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, sizeToFit), XtRString, "True"},
  {"removePath", "RemovePath", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, removePath), XtRString, "True"},
  {"helpMessage", "HelpMessage", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, helpMessage), XtRString, "False"},
  {"defaultSearchType", "SearchType", XtRSearchType, sizeof(unsigned),
     XtOffset(XLessResources *, defaultSearchType), XtRString, "ExactMatch"},
  {"monitorFile", "MonitorFile", XtRBoolean, sizeof(Boolean),
     XtOffset(XLessResources *, monitorFile), XtRString, "False"},
};

static XrmOptionDescRec options[] = {
  {"-f",	"*monitorFile", XrmoptionNoArg,	(XtPointer )"True"},
  {"-fn", 	"*textFont", XrmoptionSepArg,	(XtPointer )NULL},
  {"-font", 	"*textFont", XrmoptionSepArg,	(XtPointer )NULL},
  {"-help",	"*helpMessage", XrmoptionNoArg,	(XtPointer )"True"},
};

static Boolean cvtStringToSearchType __P((Display *, XrmValue *, Cardinal *,
					  XrmValue *, XrmValue *, XtPointer *));
static void cleanup __P((void));

#define done(type, value)				\
	{						\
	  if (toVal->addr != NULL) {			\
	    if (toVal->size < sizeof(type)) {		\
	      toVal->size = sizeof(type);		\
	      return False;				\
	    }						\
	    *(type *)(toVal->addr) = (value);		\
	  } else {					\
	    static type newValue;			\
							\
	    newValue = (value);				\
	    toVal->addr = (XtPointer )&newValue;	\
	  }						\
	  toVal->size = sizeof(type);			\
	  return True;					\
	}

static Boolean
cvtStringToSearchType(display, args, numArgs, fromVal, toVal, destructorData)
Display *display;
XrmValue *args;
Cardinal *numArgs;
XrmValue *fromVal;
XrmValue *toVal;
XtPointer *destructorData;
{
  static int initialized = 0;
  static XrmQuark QExactMatch, QCaseInsensitive, QRegularExpression;
  char *tmp;
  XrmQuark q;

  if (!initialized) {
    QExactMatch = XrmStringToQuark("exactmatch");
    QCaseInsensitive = XrmStringToQuark("caseinsensitive");
    QRegularExpression = XrmStringToQuark("regularexpression");
    initialized = 1;
  }

  if (*numArgs != 0)
    XtWarning("String to SearchType conversion needs no extra arguments");

  /* get quark for string */
  tmp = XtMalloc((Cardinal )(strlen((char *)fromVal->addr) + 1));
  XmuCopyISOLatin1Lowered(tmp, (char *)fromVal->addr);
  q = XrmStringToQuark(tmp);
  XtFree(tmp);

  if (q == QExactMatch)
    done(unsigned, XLessClearFlag);

  if (q == QCaseInsensitive)
    done(unsigned, XLessSearchInsensitive);

  if (q == QRegularExpression)
    done(unsigned, XLessSearchRegExpr);

  XtDisplayStringConversionWarning(display, fromVal->addr, "XtRSearchType");
  return False;
}

static void
cleanup()
{
  XtDestroyWidget(toplevel);
  XtDestroyApplicationContext(context);
}

void
main(argc, argv)
int argc;
char *argv[];
{
  int i, attempted;

#ifdef _DEBUG_MALLOC_INC
  {
    union dbmalloptarg	moa;

    moa.i = 0;
    dbmallopt(MALLOC_CKCHAIN, &moa);
  }
#endif

  /* save program name */
  if ((progname = strrchr(argv[0], '/')))
    progname++;
  else
    progname = argv[0];

  toplevel = XtVaAppInitialize(&context, XLESS_CLASS,
			       options, XtNumber(options),
			       &argc, argv,
			       NULL, NULL);

  XtAppSetTypeConverter(context, XtRString, XtRSearchType,
			cvtStringToSearchType, (XtConvertArgList )NULL,
			0, XtCacheAll, NULL);

  XtGetApplicationResources(toplevel, (XtPointer )&resources, privResources,
			    XtNumber(privResources), NULL, (Cardinal) 0);

  /* save class name */
  className = XLESS_CLASS;

  XtAppAddActions(context, actions, numactions);

  disp = XtDisplay(toplevel);

  XtVaSetValues(toplevel,
		XtNiconPixmap, XCreateBitmapFromData(disp,
						     XRootWindow(disp, 0),
						     XLessTop_bits,
						     XLessTop_width,
						     XLessTop_height),
		NULL);

  CheckFonts();

  attempted = 0;
  for (i = 1; resources.helpMessage || i < argc; i++) {

    /* whine if there's still an argument */
    if (resources.helpMessage || *argv[i] == '-') {
      fprintf(stdout, "usage:\t%s filename\n", progname);
      fprintf(stdout, "\t(%s also takes input from stdin)\n", progname);
      cleanup();
      exit(0);
    }

    CreateWindow(toplevel, argv[i]);
    attempted++;
  }

  /* die if everything failed */
  if (attempted > 0 && windowcount == 0) {
    fprintf(stderr, "%s: no windows found\n", progname);
    cleanup();
    exit(1);
  }

  /* if we haven't opened a window yet... */
  if (windowcount == 0) {
    /*
     * Not really necessary to call this an error,
     * but if the control terminal (for commands)
     * and the input file (for data) are the same,
     * we get weird results at best.
     */
    if (isatty(fileno(stdin))) {
      fprintf(stderr, "%s: can't take input from terminal\n", progname);
      cleanup();
      exit(1);
    }

    /* bring up controlling window using stdin as input */
    CreateWindow(toplevel, NULL);
  }

  XtAppMainLoop(context);
}
