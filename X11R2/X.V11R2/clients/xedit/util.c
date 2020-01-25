#ifndef lint
static char rcs_id[] = "$Header: util.c,v 1.7 88/02/23 21:06:14 rws Exp $";
#endif

/*
 *			  COPYRIGHT 1987
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT RIGHTS,
 * APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN ADDITION TO THAT
 * SET FORTH ABOVE.
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation,
 * and that the name of Digital Equipment Corporation not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission.
 */

#include "xedit.h"
XeditPrintf(fmt, arg1, arg2, arg3, arg4)
  char *fmt;
{
  char buf[1024];
  XtTextBlock text;
  
  XtTextPosition pos = (*messsource->Scan)(messsource, 0, XtstAll, XtsdRight,1,0);
    sprintf(buf, fmt, arg1, arg2, arg3, arg4);
    text.length = strlen(buf);
    text.ptr = buf;
    XtTextReplace( messwidget, pos, pos, &text);
    XtTextSetInsertionPoint(messwidget, pos + text.length);
}

Widget makeCommandButton(box, name, function)
  Window box;
  char *name;
  XtCallbackProc function;
{
  static XtCallbackRec callbackList[] = { {NULL, NULL}, {NULL, NULL} };
  static Arg arg[] = { {XtNcallback,(XtArgVal)callbackList} };
    callbackList[0].callback = function;
    return (XtCreateManagedWidget(name, commandWidgetClass, box, arg, 1));
}


Widget makeStringBox(parentBox, string, length)
  Widget parentBox;
  char *string;
{
  Arg args[5];
  Widget StringW;
  int numargs;
    numargs = 0;
    MakeArg(XtNeditType, (XtArgVal)XttextEdit );
    MakeArg(XtNtextOptions, (XtArgVal)( resizeWidth)); 
    MakeArg(XtNstring,(XtArgVal)string);     
    MakeArg(XtNwidth,  (XtArgVal)length);
    MakeArg(XtNlength, (XtArgVal)1000);
    StringW = XtCreateManagedWidget("stringthing", asciiStringWidgetClass, 
					parentBox, args, numargs);
    return(StringW);  
}
 
FixScreen(from)
    XtTextPosition from;
{
    XtTextPosition to;
    if(from >= 0){
        to = (*source->Scan)(source, 0, XtstAll, XtsdRight, 0,0) + 10;
	XtTextInvalidate(textwindow, (from > 0 ) ? from -1 : from, to); 
	XtTextSetInsertionPoint(textwindow, from); 
    } else {
	Feep();
    }
}


