/*

Copyright (c) 1994  Randolf Werner

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*******************************************************************
 * Module "jump.c" containing stuff for jump menu                  *
 *                                                                 *
 * Randolf Werner                                                  *
 * University Koblenz                                              *
 * Germany , 6.3.91                                                *
 *******************************************************************/


#include "xedit.h"

extern Widget textwindow;
extern Widget line_popup;
extern Widget line_text;


/*******************************************************************
 * Callback for "Line" menu entry				   *
 *******************************************************************/
void DoJumpLine(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
Arg args[2];
Position x,y;
XawTextPosition insert_pos,pos;
int line_nr;
char linestring[256];

/* Get actual Line Number */
insert_pos = XawTextGetInsertionPoint(textwindow);
pos = 0;
line_nr = 1;
while(pos < insert_pos)
    {
    pos = XawTextSourceScan(XawTextGetSource(textwindow),pos,XawstEOL,XawsdRight,1,True);
    line_nr++;
    }
sprintf(linestring,"%d",line_nr);
XtSetArg(args[0],XtNstring,linestring);
XtSetValues(line_text,args,1);
XawTextSetInsertionPoint(line_text,255);

/* Popup Dialog */
XtSetArg(args[0],XtNx,&x);
XtSetArg(args[1],XtNy,&y);
XtGetValues(widget,args,2);
XtTranslateCoords(widget,x,y,&x,&y);
XtSetArg(args[0],XtNx,x);
XtSetArg(args[1],XtNy,y);
XtSetValues(line_popup,args,2);
centerpopup(textwindow,line_popup);
XtPopup(line_popup,XtGrabExclusive);
}


/*******************************************************************
 * Callback for "Begin" menu entry				   *
 *******************************************************************/
void DoJumpBegin(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XawTextSetInsertionPoint(textwindow,0);
}


/*******************************************************************
 * Callback for "End" menu entry				   *
 *******************************************************************/
void DoJumpEnd(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XawTextSetInsertionPoint(textwindow,9999999);
}


/*******************************************************************
 * Callback for "Selection Start" menu entry			   *
 *******************************************************************/
void DoJumpSelStart(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XawTextPosition begin,end;

XawTextGetSelectionPos(textwindow,&begin,&end);
XawTextSetInsertionPoint(textwindow,begin);
}


/*******************************************************************
 * Callback for "Selection End" menu entry			   *
 *******************************************************************/
void DoJumpSelEnd(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XawTextPosition begin,end;

XawTextGetSelectionPos(textwindow,&begin,&end);
XawTextSetInsertionPoint(textwindow,end);
}


/*******************************************************************
 * Action Procedure for pressing <RETURN> in linenumber popup      *
 *******************************************************************/
void goto_line(widget,event,params,n_params)
Widget widget;
XEvent *event;
String *params;
Cardinal *n_params;
{
Arg args[2];
int line_nr;
char *inputstring;

XtSetArg(args[0],XtNstring,&inputstring);
XtGetValues(widget,args,1);
line_nr = atoi(inputstring);
XtPopdown(line_popup);

XawTextSetInsertionPoint(textwindow,
                         XawTextSourceScan(XawTextGetSource(textwindow),
                                           0,XawstEOL,XawsdRight,line_nr-1,True));
}

