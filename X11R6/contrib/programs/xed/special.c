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
 * Module "special.c" containing stuff for commands in the Special *
 * Menu								   *
 *                                                                 *
 * Randolf Werner                                                  *
 * University Koblenz                                              *
 * Germany , 6.3.91                                                *
 *******************************************************************/

#include <fcntl.h>
#include "xedit.h"
#include <limits.h>

/* Used global widgets */
extern Widget textwindow,option_popup;
extern Widget wrap_group;
extern Widget indent_group;
extern Widget tabsize_text;
extern Widget sed_popup,sed_text;
extern Widget help_popup,help_text;
extern Widget about_popup;
extern char **wrapmode;
extern char **indentmode;


/*******************************************************************
 * Calllback for "Optiions" menu entry				   *
 *******************************************************************/
void DoSpezialOptions(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
Arg args[2];
XawTextWrapMode mode;
caddr_t data;

/* Get Wrapmode */
XtSetArg(args[0],XtNwrap,&mode);
XtGetValues(textwindow,args,1);
switch(mode)
    {
    case XawtextWrapNever:  {
			    data = wrapmode[0];
			    break;
			    }
    case XawtextWrapLine:   {
			    data = wrapmode[1];
			    break;
			    }
    case XawtextWrapWord:   {
			    data = wrapmode[2];
			    break;
			    }
    default:		    break;
    }
XawToggleSetCurrent(wrap_group,data);
if(app_resources.autoIndent)
    XawToggleSetCurrent(indent_group,indentmode[0]);
else
    XawToggleSetCurrent(indent_group,indentmode[1]);

centerpopup(textwindow,option_popup);
XtPopup(option_popup,XtGrabExclusive);
}


/*******************************************************************
 * Set tabsize for a textwidget					   *
 *******************************************************************/
void set_tabsize(widget,tabsize)
Widget widget;
int tabsize;
{
Widget sink;
int tabs[100],i;
Arg args[2];

XtSetArg(args[0],XtNtextSink,&sink);
XtGetValues(widget,args,1);
for(i=0;i<100;i++)
   tabs[i]=tabsize*i;
XawTextSinkSetTabs(sink,100,tabs);
}


/*******************************************************************
 * Callback for option popup OK and CANCEL button		   *
 *******************************************************************/
void option_ready(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
static tabsize = 8;
Arg args[1];
char *inputstring,outputstring[256];
XawTextPosition end;
XtTranslations trans;

XtPopdown(option_popup);
if((int)closure == CANCEL)
    {
    sprintf(outputstring,"%d",tabsize);
    XtSetArg(args[0],XtNstring,outputstring);
    XtSetValues(tabsize_text,args,1);
    XawTextSetInsertionPoint(tabsize_text,255);
    return;
    }
 
/* Set Wrapmode */
if (XawToggleGetCurrent(wrap_group) == wrapmode[0])
    XtSetArg(args[0],XtNwrap,XawtextWrapNever);
if (XawToggleGetCurrent(wrap_group) == wrapmode[1])
    XtSetArg(args[0],XtNwrap,XawtextWrapLine);
if (XawToggleGetCurrent(wrap_group) == wrapmode[2])
    XtSetArg(args[0],XtNwrap,XawtextWrapWord);
XtSetValues(textwindow,args,1);

/* Set Tabsize */
XtSetArg(args[0],XtNstring,&inputstring);
XtGetValues(tabsize_text,args,1);
tabsize = atoi(inputstring);
if(tabsize < 1)
    tabsize = 1;
sprintf(outputstring,"%d",tabsize);
XtSetArg(args[0],XtNstring,outputstring);
XtSetValues(tabsize_text,args,1);
XawTextSetInsertionPoint(tabsize_text,255);
set_tabsize(textwindow,tabsize);

/* Set Indent Mode */
if (XawToggleGetCurrent(indent_group) == indentmode[0])
    {
    app_resources.autoIndent = True;
    trans=XtParseTranslationTable(":<Key>Return:autoindent()");
    XtOverrideTranslations(textwindow,trans);
    }
else
    {
    app_resources.autoIndent = False;
    trans=XtParseTranslationTable(":<Key>Return:newline()");
    XtOverrideTranslations(textwindow,trans);
    }

/* Redisplay all */
end = XawTextSourceScan(XawTextGetSource(textwindow),INT_MAX,XawstEOL,XawsdRight,1,True);
XawTextInvalidate(textwindow,0,end);
XawTextEnableRedisplay(textwindow);
}


/*******************************************************************
 * Action procedure for autoindent in a textwidget                 *
 *******************************************************************/
void autoindent(widget,event,params,n_params)
Widget widget;
XEvent *event;
String *params;
Cardinal *n_params;
{
XawTextPosition end,begin,begin2,nbytes;
char *pos,line_buf[1024];
XawTextBlock buf;

buf.firstPos = 0;
buf.format = FMT8BIT;
*line_buf = '\0';

/* Get current Position and Line Begin */
end = XawTextGetInsertionPoint(widget);
begin = XawTextSourceScan(XawTextGetSource(widget),end,XawstEOL,XawsdLeft,1,True);
if (begin == XawTextSearchError)
    begin = 0;
if (begin > 0)
    begin++;

/* Insert Newline */
buf.ptr = "\n";
buf.length = 1;
XawTextReplace(widget,end,end,&buf);
		  
begin2 = begin;
while(begin2 < end)
    {
    nbytes = end - begin;
    XawTextSourceRead(XawTextGetSource(widget),begin2,&buf,nbytes);
    begin2 += buf.length;
    strncat(line_buf,buf.ptr,buf.length);
    }
buf.length = 0;
pos = line_buf;
while(((*pos == ' ') || (*pos == '\t')) && (*pos != NULL))
    {
    buf.length++;
    pos++;
    }
buf.ptr = line_buf;
buf.firstPos = 0;
XawTextReplace(widget,end+1,end+1,&buf);
XawTextSetInsertionPoint(widget,end+buf.length+1);
}


/*******************************************************************
 * Callback for "Call sed" menu entry				   *
 *******************************************************************/
void DoSpezialCallSed(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
centerpopup(textwindow,sed_popup);
XtPopup(sed_popup,XtGrabNone);
XRaiseWindow(XtDisplay(sed_popup),XtWindow(sed_popup));
}

static char tmpin_file[L_tmpnam+3],tmpout_file[L_tmpnam+3];
static Boolean undo_possible = False;


/*******************************************************************
 * Internal function for replacing current text with contens of    *
 * a file							   *
 *******************************************************************/
static void replace_file(filename)
char* filename;
{
XawTextBlock block;
XawTextPosition position,textend;
char buffer[1024];
int file_d;

block.firstPos = 0;
block.ptr = buffer;
block.length = 0;
block.format = FMT8BIT;
position = 0;

file_d = open(filename,O_RDONLY);
if(file_d == -1)
    return;
textend = XawTextSourceScan(XawTextGetSource(textwindow),INT_MAX,XawstEOL,
                            XawsdRight,1,True);
XawTextReplace(textwindow,0,textend,&block);
while( (block.length = read(file_d,block.ptr, 1024)) > 0)
    {
    XawTextReplace(textwindow,position,position,&block);
    position += 1024;
    }
close(file_d);
}


/*******************************************************************
 * Callback for "Do it" button in sed popup			   *
 *******************************************************************/
void sed_do(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char *commandstring,command[1024];
Arg args[2];

if(undo_possible)
    {
    unlink(tmpin_file);
    }
else
    undo_possible = True;
tmpnam(tmpin_file);strcat(tmpin_file,"in");
XawAsciiSaveAsFile(XawTextGetSource(textwindow),tmpin_file);
tmpnam(tmpout_file);strcat(tmpout_file,"out");
XtSetArg(args[0],XtNstring,&commandstring);
XtGetValues(sed_text,args,1);
#ifdef USE_CSH_SYSTEM
sprintf(command,"cat %s | sed %s >& %s",tmpin_file,commandstring,tmpout_file);
#else
sprintf(command,"cat %s | sed %s 1> %s 2> %s",tmpin_file,commandstring,tmpout_file,tmpout_file);
#endif
system(command);
replace_file(tmpout_file);
unlink(tmpout_file);
SourceChanged(XawTextGetSource(textwindow),NULL,NULL);
}


/*******************************************************************
 * Callback for "Undo it" button in sed popup			   *
 *******************************************************************/
void sed_undo(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char command[1024];

if(undo_possible)
    {
    replace_file(tmpin_file);
    unlink(tmpin_file);
    undo_possible = False;
    }
else
    Feep();
}


/*******************************************************************
 * Callback for "Close" button in sed popup			   *
 *******************************************************************/
void sed_close(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
char command[1024];

XtPopdown(sed_popup);
if(undo_possible)
    {
    unlink(tmpin_file);
    }
}


/*******************************************************************
 * Callback for "Help" menu entry				   *
 *******************************************************************/
void DoSpezialHelp(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
Arg args[2];
Dimension h,w;

XtSetArg(args[0],XtNwidth,&w);
XtSetArg(args[1],XtNheight,&h);
XtGetValues(textwindow,args,2);
XtSetArg(args[0],XtNwidth,w);
XtSetArg(args[1],XtNheight,h);
XtSetValues(help_text,args,2);
centerpopup(textwindow,help_popup);
XtPopup(help_popup,XtGrabNone);
XRaiseWindow(XtDisplay(help_popup),XtWindow(help_popup));
}


/*******************************************************************
 * Callback for "About" menu entry			    	   *
 *******************************************************************/
void DoSpezialAbout(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
centerpopup(textwindow,about_popup);
XtPopup(about_popup,XtGrabExclusive);
}


/*******************************************************************
 * Callback for "close" button in about popup			   *
 *******************************************************************/
void about_close(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XtPopdown(about_popup);
}


/*******************************************************************
 * Callback for "close" button in help popup			   *
 *******************************************************************/
void help_close(widget,closure,calldata)
Widget widget;
caddr_t closure,calldata;
{
XtPopdown(help_popup);
}
