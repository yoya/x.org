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
 * Module "xedit.c" contains main procedure and creates widgets    *
 *                                                                 *
 * Randolf Werner                                                  *
 * University Koblenz                                              *
 * Germany , 6.3.91                                                *
 *								   *
 * Because some stuff is taken from the "xedit" programm here      *
 * comes the copyright message of "xedit"                          *
 *******************************************************************/

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
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Digital Equipment Corporation not be 
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 */

static char abouttext[] = "\n\n\n\
                                                                 X E D  (Version 1.0)\n\
\n\
                                               The interactive Texteditor for X-Windows\n\
\n\
        Authors:   Randolf Werner, University Koblenz      (Main programming)\n\
                           Karl-Heinz Staudt, University Koblenz  (Basic design of the fileselector)\n\
                           Chris D. Peterson, MIT X Consortium   (Some stuff taken from his \"xedit\")\n\
\n";
#include "xedit.h"


/* Icon Bitmap */
#define stop_width 31
#define stop_height 31
static char stop_bits[] = {
   0x00, 0xfe, 0x3f, 0x80, 0x00, 0x01, 0x40, 0x80, 0x80, 0xfc, 0x9f, 0x80,
   0x40, 0xfe, 0x3f, 0x81, 0x20, 0xff, 0x7f, 0x82, 0x90, 0xff, 0xff, 0x84,
   0xc8, 0xff, 0xff, 0x89, 0xe4, 0xff, 0xff, 0x93, 0xf2, 0xff, 0xff, 0xa7,
   0xf9, 0xff, 0xff, 0xcf, 0xf9, 0xff, 0xff, 0xcf, 0xf9, 0xff, 0xff, 0xcf,
   0x19, 0x83, 0x31, 0xcc, 0xe9, 0xee, 0xae, 0xcb, 0xe9, 0xef, 0xae, 0xcb,
   0x19, 0xef, 0x2e, 0xcc, 0xf9, 0xee, 0xae, 0xcf, 0xe9, 0xee, 0xae, 0xcf,
   0x19, 0xef, 0xb1, 0xcf, 0xf9, 0xff, 0xff, 0xcf, 0xf9, 0xff, 0xff, 0xcf,
   0xf9, 0xff, 0xff, 0xcf, 0xf2, 0xff, 0xff, 0xa7, 0xe4, 0xff, 0xff, 0x93,
   0xc8, 0xff, 0xff, 0x89, 0x90, 0xff, 0xff, 0x84, 0x20, 0xff, 0x7f, 0x82,
   0x40, 0xfe, 0x3f, 0x81, 0x80, 0xfc, 0x9f, 0x80, 0x00, 0x01, 0x40, 0x80,
   0x00, 0xfe, 0x3f, 0x80};

/* Global widgets */
Widget textwindow, messwidget, labelwindow, filenamewindow;
Widget file_s;
Widget line_popup,line_text,error_popup,error_label;
Widget warn_popup,warn_label,warn_box;
Widget option_popup,tabsize_text;
Widget wrap_group,indent_group;
Widget sed_popup,sed_text;
Widget help_popup,help_text;
Widget replace_text,search_text,search_popup;
Widget start_group,direction_group;
Widget veto_popup,about_popup;
char   **wrapmode,**indentmode; 
Display *CurDpy;

struct _app_resources app_resources;

#define offset(field) XtOffset(struct _app_resources*, field)
static XtResource resources[] = {
   {"enableBackups", "EnableBackups", XtRBoolean, sizeof(Boolean),
         offset(enableBackups), XtRImmediate, FALSE},
   {"backupNameSuffix", "BackupNameSuffix", XtRString, sizeof(char *),
         offset(backupNameSuffix),XtRString, ".BAK"},
   {"printCommand", "PrintCommand",XtRString, sizeof(char *),
         offset(printCommand),XtRString, "lpr -p -T %t %f"},
   {"autoIndent", "AutoIndent", XtRBoolean, sizeof(Boolean),
         offset(autoIndent), XtRImmediate, FALSE},
   {"tabsize", "Tabsize",XtRInt, sizeof(int),
         offset(tabsize),XtRImmediate, (caddr_t)(int)8},
   {"maxScrollbreak", "MaxScrollbreak",XtRInt, sizeof(int),
         offset(maxScrollbreak),XtRImmediate, (caddr_t)(int)3}
};
#undef offset

static XtActionsRec action_goto_line[]={{"goto_line",goto_line},};
static XtActionsRec action_autoindent[]={{"autoindent",autoindent},};
#ifdef SCROLLBREAK
static XtActionsRec action_cursorup[]={{"cursor_up",cursor_up},};
static XtActionsRec action_cursordown[]={{"cursor_down",cursor_down},};
#endif


/********************************************************************************
 *  Creating all popups								*
 ********************************************************************************/
static void creat_popups(top)
Widget top;
{
Widget widget,widget2,box,box2;
Arg args[10];
XtTranslations trans;
Pixmap bitmap;
char tabstring[256];
XFontStruct *font;

  /* Popup for Goto Line */
  line_popup=XtCreatePopupShell("line_popup",transientShellWidgetClass,top,args,0);
  widget=XtCreateManagedWidget("line_box",boxWidgetClass,line_popup,NULL,0);
  XtSetArg(args[0],XtNlabel,"Linenumber ?");
  XtSetArg(args[1],XtNborderWidth,0);
  XtCreateManagedWidget("line_label",labelWidgetClass,widget,args,2);
  XtSetArg(args[0],XtNeditType,XawtextEdit);
  line_text=XtCreateManagedWidget("line_text",asciiTextWidgetClass,widget,args,1);
  XtAppAddActions(XtWidgetToApplicationContext(line_text),action_goto_line,1);
  trans=XtParseTranslationTable(":<Key>Return:goto_line()");
  XtOverrideTranslations(line_text,trans);

  /* Popup for error messages */
  error_popup=XtCreatePopupShell("error_popup",transientShellWidgetClass,top,args,0);
  widget=XtCreateManagedWidget("error_box",boxWidgetClass,error_popup,NULL,0);
  bitmap = XCreateBitmapFromData(CurDpy,RootWindow(CurDpy,DefaultScreen(CurDpy)),
                                 stop_bits,stop_width,stop_height);
  XtSetArg(args[0],XtNbitmap,bitmap);
  XtSetArg(args[1],XtNborderWidth,0);
  XtCreateManagedWidget("stop_icon",labelWidgetClass,widget,args,2);
  XtSetArg(args[0],XtNlabel,"Fehlermeldung                !");
  XtSetArg(args[1],XtNborderWidth,0);
  error_label=XtCreateManagedWidget("error_label",labelWidgetClass,widget,args,2);
  XtSetArg(args[0],XtNlabel,"  OK  ");
  widget=XtCreateManagedWidget("error_ok",commandWidgetClass,widget,args,1);
  XtAddCallback(widget,XtNcallback,error_ready,0);

  /* Popup for warning messages */
  warn_popup=XtCreatePopupShell("warn_popup",transientShellWidgetClass,top,args,0);
  warn_box = widget=XtCreateManagedWidget("warn_box",boxWidgetClass,warn_popup,NULL,0);
  XtSetArg(args[0],XtNbitmap,bitmap);
  XtSetArg(args[1],XtNborderWidth,0);
  XtCreateManagedWidget("stop_icon2",labelWidgetClass,widget,args,2);
  XtSetArg(args[0],XtNlabel,"Warnung                !");
  XtSetArg(args[1],XtNborderWidth,0);
  warn_label=XtCreateManagedWidget("warn_label",labelWidgetClass,widget,args,2);
  XtSetArg(args[0],XtNlabel,"      ABORT      ");
  widget2=XtCreateManagedWidget("warn_abort",commandWidgetClass,widget,args,1);
  XtAddCallback(widget2,XtNcallback,warn_ready,ABORT);
  XtSetArg(args[0],XtNlabel,"     CONTINUE    ");
  widget2=XtCreateManagedWidget("warn_continue",commandWidgetClass,widget,args,1);
  XtAddCallback(widget2,XtNcallback,warn_ready,(caddr_t)CONTINUE);

  /* Popup for options */
  wrapmode = (char**) malloc(3*sizeof(char*));
  wrapmode[0]=  "Never";
  wrapmode[1]=  "Line";
  wrapmode[2]=  "Word";
  indentmode = (char**) malloc(2*sizeof(char*));
  indentmode[0]= "yes";
  indentmode[1]= "false";
  option_popup=XtCreatePopupShell("option_popup",transientShellWidgetClass,top,args,0);
    box = XtCreateManagedWidget("option_box",boxWidgetClass,option_popup,NULL,0);
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      box2 = XtCreateManagedWidget("option_box2",boxWidgetClass,box,args,2); 
        XtSetArg(args[0],XtNlabel,"Wrapmode:   ");
        XtSetArg(args[1],XtNborderWidth,0);
        XtCreateManagedWidget("warp_label",labelWidgetClass,box2,args,2);
        XtSetArg(args[0],XtNlabel,"Never");
	XtSetArg(args[1],XtNradioData,(caddr_t)wrapmode[0]);
        wrap_group = widget = XtCreateManagedWidget("wrap_never",toggleWidgetClass,box2,args,2);
        XtSetArg(args[0],XtNlabel,"Line ");
        XtSetArg(args[1],XtNradioGroup,widget);
	XtSetArg(args[2],XtNradioData,(caddr_t)wrapmode[1]);
        XtCreateManagedWidget("wrap_line",toggleWidgetClass,box2,args,3);
        XtSetArg(args[0],XtNlabel,"Word ");  
        XtSetArg(args[1],XtNradioGroup,widget);
	XtSetArg(args[2],XtNradioData,(caddr_t)wrapmode[2]);
        XtCreateManagedWidget("wrap_word",toggleWidgetClass,box2,args,3);
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      box2 = XtCreateManagedWidget("option_box4",boxWidgetClass,box,args,2); 
        XtSetArg(args[0],XtNlabel,"Tabsize:    ");
        XtSetArg(args[1],XtNborderWidth,0);
 	XtCreateManagedWidget("tab_label",labelWidgetClass,box2,args,2);
        XtSetArg(args[0],XtNeditType,XawtextEdit);
        if(app_resources.tabsize > 0)
            sprintf(tabstring,"%d",app_resources.tabsize);
        else
            sprintf(tabstring,"8");
	XtSetArg(args[1],XtNstring,tabstring);
        tabsize_text=XtCreateManagedWidget("tab_text",asciiTextWidgetClass,box2,args,2);
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      box2 = XtCreateManagedWidget("option_box4",boxWidgetClass,box,args,2); 
        XtSetArg(args[0],XtNlabel,"Autoindent: ");
        XtSetArg(args[1],XtNborderWidth,0);
        XtCreateManagedWidget("indent_label",labelWidgetClass,box2,args,2);
        XtSetArg(args[0],XtNlabel,"Yes  ");
	XtSetArg(args[1],XtNradioData,(caddr_t)indentmode[0]);
        indent_group = XtCreateManagedWidget("indent_yes",toggleWidgetClass,box2,args,2);
        XtSetArg(args[0],XtNlabel,"No   ");
        XtSetArg(args[1],XtNradioGroup,indent_group);
	XtSetArg(args[2],XtNradioData,(caddr_t)indentmode[1]);
        widget = XtCreateManagedWidget("indent_no",toggleWidgetClass,box2,args,3);
	XtSetArg(args[0],XtNstate,True);
        if(app_resources.autoIndent)
            XtSetValues(indent_group,args,1);
        else
            XtSetValues(widget,args,1);
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      box2 = XtCreateManagedWidget("option_box5",boxWidgetClass,box,args,2);
        XtSetArg(args[0],XtNlabel,"  OK  ");
        widget2=XtCreateManagedWidget("option_ok",commandWidgetClass,box2,args,1);
        XtAddCallback(widget2,XtNcallback,option_ready,OK);
        XtSetArg(args[0],XtNlabel,"CANCEL");
        widget2=XtCreateManagedWidget("option_cancel",commandWidgetClass,box2,args,1);
        XtAddCallback(widget2,XtNcallback,option_ready,(caddr_t)CANCEL);

  /* Popup for sed commands */
  sed_popup=XtCreatePopupShell("sed",wmShellWidgetClass,top,args,0);
  box = XtCreateManagedWidget("sed_paned",panedWidgetClass,sed_popup,args,0);
      XtSetArg(args[0],XtNlabel,"Perform a stream editor command (sed)");
      XtSetArg(args[1],XtNborderWidth,0);
      XtSetArg(args[2],XtNshowGrip,False);
      XtCreateManagedWidget("sed_label",labelWidgetClass,box,args,3);
      
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False);
      XtSetArg(args[3],XtNinternalBorderWidth,0);
      box2 = XtCreateManagedWidget("option_box4",panedWidgetClass,box,args,4); 
 
	  XtSetArg(args[0],XtNborderWidth,0);
	  XtSetArg(args[1],XtNlabel,"Command: ");
	  XtSetArg(args[2],XtNshowGrip,False);
	  XtCreateManagedWidget("sed_label2",labelWidgetClass,box2,args,3);
	  
          XtSetArg(args[0],XtNeditType,XawtextEdit);
          XtSetArg(args[2],XtNshowGrip,False);
          sed_text=XtCreateManagedWidget("sed_text",asciiTextWidgetClass,box2,args,2);
      
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False); 
      XtSetArg(args[3],XtNskipAdjust,True);   
      box2 = XtCreateManagedWidget("sed_box3",boxWidgetClass,box,args,4);
      
          XtSetArg(args[0],XtNlabel," Do it ");
          widget = XtCreateManagedWidget("sed_doit",commandWidgetClass,box2,args,1);
          XtAddCallback(widget,XtNcallback,sed_do,NULL);
          
          XtSetArg(args[0],XtNlabel,"Undo it");
          widget = XtCreateManagedWidget("sed_undoit",commandWidgetClass,box2,args,1);
          XtAddCallback(widget,XtNcallback,sed_undo,NULL);
          
          XtSetArg(args[0],XtNlabel," Close ");
          widget = XtCreateManagedWidget("sed_close",commandWidgetClass,box2,args,1);
	  XtAddCallback(widget,XtNcallback,sed_close,NULL);

  /* Popup for help */
  help_popup=XtCreatePopupShell("help",wmShellWidgetClass,top,args,0);
  box = XtCreateManagedWidget("help_paned",panedWidgetClass,help_popup,args,0);
      	    	
      XtSetArg(args[0],XtNtype,XawAsciiFile);
      XtSetArg(args[1],XtNstring,HELPFILE);
      XtSetArg(args[2],XtNscrollVertical,XawtextScrollAlways);
      help_text = XtCreateManagedWidget("help_text",asciiTextWidgetClass,box,args,3);

      XtSetArg(args[0],XtNlabel," Close ");
      XtSetArg(args[1],XtNshowGrip,False);
      XtSetArg(args[2],XtNskipAdjust,True);
      widget = XtCreateManagedWidget("help_close",commandWidgetClass,box,args,3);
      XtAddCallback(widget,XtNcallback,help_close,NULL);

  /* Popup for about */
  about_popup=XtCreatePopupShell("about",transientShellWidgetClass,top,args,0);
  box = XtCreateManagedWidget("about_paned",panedWidgetClass,about_popup,args,0);
      	    	
      XtSetArg(args[0],XtNtype,XawAsciiString);
      XtSetArg(args[1],XtNstring,abouttext);
      XtSetArg(args[2],XtNwidth,710);
      XtSetArg(args[3],XtNheight,270);
      font = XLoadQueryFont(CurDpy,"-adobe-times-medium-i-normal--20-140-100-100-p-94-iso8859-1");
      if(font == NULL)
          font = XLoadQueryFont(CurDpy,"-adobe-times-medium-i-normal--18-180-75-75-p-94-iso8859-1");
      if(font == NULL)
          XtCreateManagedWidget("about_text",asciiTextWidgetClass,box,args,4);
      else
          {
          XtSetArg(args[4],XtNfont, font);
          XtCreateManagedWidget("about_text",asciiTextWidgetClass,box,args,5);
          }
          
      XtSetArg(args[0],XtNlabel," Close ");
      XtSetArg(args[1],XtNshowGrip,False);
      XtSetArg(args[2],XtNskipAdjust,True);
      widget = XtCreateManagedWidget("about_close",commandWidgetClass,box,args,3);
      XtAddCallback(widget,XtNcallback,about_close,NULL); 
       
  /* Popup for Search and Replace */
  search_popup=XtCreatePopupShell("Search or Replace",wmShellWidgetClass,top,args,0);
  XtSetArg(args[0],XtNinternalBorderWidth,0);
  box = XtCreateManagedWidget("search_paned",panedWidgetClass,search_popup,args,1);

      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False); 
      XtSetArg(args[3],XtNskipAdjust,True);   
      box2 = XtCreateManagedWidget("search_box1",boxWidgetClass,box,args,4);
          
          XtSetArg(args[0],XtNlabel,"Start at    :");
          XtSetArg(args[1],XtNborderWidth,0);
          XtCreateManagedWidget("start_at",labelWidgetClass,box2,args,2);
          
          XtSetArg(args[0],XtNlabel,"Cursor position");
          XtSetArg(args[1],XtNstate,True);
          start_group = widget = XtCreateManagedWidget("Cursor position",toggleWidgetClass,box2,args,2);
          
          XtSetArg(args[0],XtNlabel,"Textbegining");
          XtSetArg(args[1],XtNradioGroup,widget);
          XtCreateManagedWidget("Textbegining",toggleWidgetClass,box2,args,2);
          
          XtSetArg(args[0],XtNlabel,"Textending");
          XtSetArg(args[1],XtNradioGroup,widget);
          XtCreateManagedWidget("Textending",toggleWidgetClass,box2,args,2);
          
      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False); 
      XtSetArg(args[3],XtNskipAdjust,True);   
      box2 = XtCreateManagedWidget("search_box2",boxWidgetClass,box,args,4);
          
          XtSetArg(args[0],XtNlabel,"Direction   :");
          XtSetArg(args[1],XtNborderWidth,0);
          XtCreateManagedWidget("direction",labelWidgetClass,box2,args,2); 

          XtSetArg(args[0],XtNlabel,"Forward");
          XtSetArg(args[1],XtNstate,True);
          direction_group = widget = XtCreateManagedWidget("Forward",toggleWidgetClass,box2,args,2);
          
          XtSetArg(args[0],XtNlabel,"Backward");
          XtSetArg(args[1],XtNradioGroup,widget);
          XtCreateManagedWidget("Backward",toggleWidgetClass,box2,args,2);

      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False);
      XtSetArg(args[3],XtNinternalBorderWidth,0);
      XtSetArg(args[4],XtNskipAdjust,True);
      box2 = XtCreateManagedWidget("search_box3",panedWidgetClass,box,args,5); 
 
	  XtSetArg(args[0],XtNborderWidth,0);
	  XtSetArg(args[1],XtNlabel,"Search for  : ");
	  XtSetArg(args[2],XtNshowGrip,False);
	  XtSetArg(args[3],XtNinternalWidth,8);
	  XtSetArg(args[4],XtNinternalHeight,8);
	  XtCreateManagedWidget("search_label",labelWidgetClass,box2,args,5);
	  
          XtSetArg(args[0],XtNeditType,XawtextEdit);
          XtSetArg(args[1],XtNshowGrip,False);
          XtSetArg(args[2],XtNtopMargin,8);
          search_text=XtCreateManagedWidget("search_text",asciiTextWidgetClass,box2,args,3);

      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False);
      XtSetArg(args[3],XtNinternalBorderWidth,0);
      XtSetArg(args[4],XtNskipAdjust,True);
      box2 = XtCreateManagedWidget("search_box4",panedWidgetClass,box,args,5); 
 
	  XtSetArg(args[0],XtNborderWidth,0);
	  XtSetArg(args[1],XtNlabel,"Replace with: ");
	  XtSetArg(args[2],XtNshowGrip,False);
	  XtSetArg(args[3],XtNinternalWidth,8);
	  XtSetArg(args[4],XtNinternalHeight,8);
	  XtCreateManagedWidget("replace_label",labelWidgetClass,box2,args,5);
	  
          XtSetArg(args[0],XtNeditType,XawtextEdit);
          XtSetArg(args[1],XtNshowGrip,False);
          XtSetArg(args[2],XtNtopMargin,8);
         
          replace_text=XtCreateManagedWidget("replace_text",asciiTextWidgetClass,box2,args,3);

      XtSetArg(args[0],XtNborderWidth,0);
      XtSetArg(args[1],XtNorientation,XtorientHorizontal);
      XtSetArg(args[2],XtNshowGrip,False); 
      XtSetArg(args[3],XtNskipAdjust,True);   
      box2 = XtCreateManagedWidget("search_box5",boxWidgetClass,box,args,4);
          
          XtSetArg(args[0],XtNlabel,"   Search   ");
          widget=XtCreateManagedWidget("search",commandWidgetClass,box2,args,1); 
          XtAddCallback(widget,XtNcallback,search,NULL);
          
          XtSetArg(args[0],XtNlabel,"  Replace   ");
          widget=XtCreateManagedWidget("replace",commandWidgetClass,box2,args,1); 
          XtAddCallback(widget,XtNcallback,replace,REPLACE);
          
          XtSetArg(args[0],XtNlabel,"Replace veto");
          widget=XtCreateManagedWidget("replace_veto",commandWidgetClass,box2,args,1); 
          XtAddCallback(widget,XtNcallback,replace,(caddr_t)REPLACE_VETO);
          
          XtSetArg(args[0],XtNlabel,"Replace all ");
          widget=XtCreateManagedWidget("replace_all",commandWidgetClass,box2,args,1); 
          XtAddCallback(widget,XtNcallback,replace,(caddr_t)REPLACE_ALL);

          XtSetArg(args[0],XtNlabel,"   Cancel   ");
          widget = XtCreateManagedWidget("dirrection",commandWidgetClass,box2,args,1);
          XtAddCallback(widget,XtNcallback,search_ready,NULL); 

  /* Popup for Replace veto */
  veto_popup=XtCreatePopupShell("veto_popup",transientShellWidgetClass,top,args,0);
      box = XtCreateManagedWidget("veto_box",boxWidgetClass,veto_popup,NULL,0);
          XtSetArg(args[0],XtNlabel,"Replace ?");
          XtSetArg(args[1],XtNborderWidth,0);
          XtCreateManagedWidget("veto_label",labelWidgetClass,box,args,2);

          XtSetArg(args[0],XtNborderWidth,0);
          XtSetArg(args[1],XtNorientation,XtorientHorizontal);
          box2 = XtCreateManagedWidget("veto_box2",boxWidgetClass,box,args,2);
  
              XtSetArg(args[0],XtNlabel," Yes  ");
              widget=XtCreateManagedWidget("veto_yes",commandWidgetClass,box2,args,1);
              XtAddCallback(widget,XtNcallback,veto_ready,(caddr_t)YES);

              XtSetArg(args[0],XtNlabel,"  No  ");
              widget=XtCreateManagedWidget("veto_no",commandWidgetClass,box2,args,1);
              XtAddCallback(widget,XtNcallback,veto_ready,(caddr_t)NO);

              XtSetArg(args[0],XtNlabel,"Cancel");
              widget=XtCreateManagedWidget("veto_cancel",commandWidgetClass,box2,args,1);
              XtAddCallback(widget,XtNcallback,veto_ready,(caddr_t)CANCEL);
}


/********************************************************************************
 *    Creating menus, statusline and textwindow				        *
 ********************************************************************************/
static void makeButtonsAndBoxes(parent, filename)
Widget parent;
char * filename;
{
  Widget outer, b_row;
  Arg arglist[10];
  Cardinal num_args;

  Widget widget,menu;
  XtTranslations trans;
 
  outer = XtCreateManagedWidget( "paned", panedWidgetClass, parent,
				NULL, ZERO);
 
  b_row= XtCreateManagedWidget("buttons", panedWidgetClass, outer, NULL, 0);
  {

    /* File - Menue */
    menu=XtCreatePopupShell("filemenu",simpleMenuWidgetClass,parent,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Load");
    widget=XtCreateManagedWidget("file_load",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileLoad,NULL);

    XtSetArg(arglist[0],XtNlabel,"Insert");
    widget=XtCreateManagedWidget("file_insert",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileInsert,NULL);

    XtCreateManagedWidget("line",smeLineObjectClass,menu,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Save");
    widget=XtCreateManagedWidget("file_save",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileSave,NULL);

    XtSetArg(arglist[0],XtNlabel,"Save as");
    widget=XtCreateManagedWidget("file_saveas",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileSaveAs,NULL);

    XtSetArg(arglist[0],XtNlabel,"Save Selection");
    widget=XtCreateManagedWidget("file_savesel",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileSaveSelection,NULL);

    XtCreateManagedWidget("line",smeLineObjectClass,menu,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Print");
    widget=XtCreateManagedWidget("file_print",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFilePrint,NULL);

    XtSetArg(arglist[0],XtNlabel,"Print Selection");
    widget=XtCreateManagedWidget("file_printsel",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFilePrintSelection,NULL);

    XtCreateManagedWidget("line",smeLineObjectClass,menu,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Quit");
    widget=XtCreateManagedWidget("file_quit",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoFileQuit,NULL);
		
    XtSetArg(arglist[0],XtNlabel,"File");
    XtSetArg(arglist[1],XtNmenuName,"filemenu");
    XtCreateManagedWidget("file_menu_button",menuButtonWidgetClass,b_row,arglist,2);


    /* Edit Menue */
    menu=XtCreatePopupShell("editmenu",simpleMenuWidgetClass,parent,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Undo");
    widget=XtCreateManagedWidget("edit_undo",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoEditUndo,NULL);

    XtSetArg(arglist[0],XtNlabel,"Cut");
    widget=XtCreateManagedWidget("edit_cut",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoEditCut,NULL);

    XtSetArg(arglist[0],XtNlabel,"Paste");
    widget=XtCreateManagedWidget("edit_paste",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoEditPaste,NULL);

    XtSetArg(arglist[0],XtNlabel,"Shift Selection right");
    widget=XtCreateManagedWidget("edit_right",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoEditShiftSelRight,NULL);

    XtSetArg(arglist[0],XtNlabel,"Shift Selection left");
    widget=XtCreateManagedWidget("edit_left",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoEditShiftSelLeft,NULL);

    XtSetArg(arglist[0],XtNlabel,"Edit");
    XtSetArg(arglist[1],XtNmenuName,"editmenu");
    XtCreateManagedWidget("edit_menu_button",menuButtonWidgetClass,b_row,arglist,2);

    /* Jump - Menue */
    menu=XtCreatePopupShell("jumpmenu",simpleMenuWidgetClass,parent,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Line");
    widget=XtCreateManagedWidget("jump_line",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoJumpLine,NULL);

    XtSetArg(arglist[0],XtNlabel,"Begin");
    widget=XtCreateManagedWidget("jump_begin",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoJumpBegin,0);

    XtSetArg(arglist[0],XtNlabel,"End");
    widget=XtCreateManagedWidget("jump_end",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoJumpEnd,0);

    XtSetArg(arglist[0],XtNlabel,"Selection Start");
    widget=XtCreateManagedWidget("jump_selstart",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoJumpSelStart,NULL);

    XtSetArg(arglist[0],XtNlabel,"Selection End");
    widget=XtCreateManagedWidget("jump_selend",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoJumpSelEnd,NULL);

    XtSetArg(arglist[0],XtNlabel,"Jump");
    XtSetArg(arglist[1],XtNmenuName,"jumpmenu");
    XtCreateManagedWidget("jump_menu_button",menuButtonWidgetClass,b_row,arglist,2); 


    /* Search - Menue */
    menu=XtCreatePopupShell("searchmenu",simpleMenuWidgetClass,parent,NULL,0);
    XtSetArg(arglist[0],XtNlabel,"Search");
    widget=XtCreateManagedWidget("search_search",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchSearch,NULL);

    XtSetArg(arglist[0],XtNlabel,"Search Selection");
    widget=XtCreateManagedWidget("search_search_selection",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchSearchselection,NULL);

    XtCreateManagedWidget("line",smeLineObjectClass,menu,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Replace");
    widget=XtCreateManagedWidget("search_replace",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchReplace,NULL);

    XtSetArg(arglist[0],XtNlabel,"Replace Selection");
    widget=XtCreateManagedWidget("search_replace_same",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchReplaceselection,NULL);

    XtCreateManagedWidget("line",smeLineObjectClass,menu,NULL,0);

    XtSetArg(arglist[0],XtNlabel,"Find bracket");
    widget=XtCreateManagedWidget("search_find_bracket",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchFindbracket,NULL);

    XtSetArg(arglist[0],XtNlabel,"Check brackets");
    widget=XtCreateManagedWidget("search_check_brackets",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSearchCheckbrackets,NULL);

    XtSetArg(arglist[0],XtNlabel,"Search");
    XtSetArg(arglist[1],XtNmenuName,"searchmenu");
    XtCreateManagedWidget("search_menu_button",menuButtonWidgetClass,b_row,arglist,2);

    /* Spezial - Menue */
    menu=XtCreatePopupShell("spezialmenu",simpleMenuWidgetClass,parent,NULL,0);
    XtSetArg(arglist[0],XtNlabel,"Options");   
    widget=XtCreateManagedWidget("spezial_options",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSpezialOptions,NULL);

    XtSetArg(arglist[0],XtNlabel,"Call sed");   
    widget=XtCreateManagedWidget("spezial_call_sed",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSpezialCallSed,NULL);

    XtSetArg(arglist[0],XtNlabel,"Help");      
    widget=XtCreateManagedWidget("spezial_help",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSpezialHelp,NULL);

    XtSetArg(arglist[0],XtNlabel,"About");      
    widget=XtCreateManagedWidget("spezial_about",smeBSBObjectClass,menu,arglist,1);
    XtAddCallback(widget,XtNcallback,DoSpezialAbout,NULL);

    XtSetArg(arglist[0],XtNlabel,"Special");
    XtSetArg(arglist[1],XtNmenuName,"spezialmenu");
    XtCreateManagedWidget("spezial_menu_button",menuButtonWidgetClass,b_row,arglist,2);

    

    /* Menue Ending */
    XtSetArg(arglist[0],XtNborderWidth,0);
    XtSetArg(arglist[1],XtNlabel,"                                                            ");
    XtCreateManagedWidget("rest",labelWidgetClass,b_row,arglist,2);
  }

  num_args = 0;
  if (filename != NULL) 
    {
    XtSetArg(arglist[num_args], XtNlabel, filename); 
    num_args++;
    }

  labelwindow = XtCreateManagedWidget("labelWindow",labelWidgetClass, 
				      outer, arglist, num_args);

  num_args = 0;
  XtSetArg(arglist[num_args], XtNtype, XawAsciiFile); num_args++;
  XtSetArg(arglist[num_args], XtNeditType, XawtextEdit); num_args++;
  XtSetArg(arglist[num_args], XtNshowGrip, False); num_args++;
  textwindow =  XtCreateManagedWidget("editWindow", asciiTextWidgetClass, 
				      outer, arglist, num_args);
  XtAppAddActions(XtWidgetToApplicationContext(textwindow),action_autoindent,1);
  trans=XtParseTranslationTable(":<Key>Return:autoindent()");
  
  if(app_resources.autoIndent)
      XtOverrideTranslations(textwindow,trans);
  if(app_resources.tabsize > 0)
    set_tabsize(textwindow,app_resources.tabsize);

#ifdef SCROLLBREAK 
  XtAppAddActions(XtWidgetToApplicationContext(textwindow),action_cursorup,1);
  trans=XtParseTranslationTable(":<Key>Up:cursor_up()");
  XtOverrideTranslations(textwindow,trans);
  XtAppAddActions(XtWidgetToApplicationContext(textwindow),action_cursordown,1);
  trans=XtParseTranslationTable(":<Key>Down:cursor_down()");
  XtOverrideTranslations(textwindow,trans);
#endif
 
  XtRealizeWidget(parent);  
  if (filename != NULL)
      load_file(filename);
  ResetSourceChanged(textwindow);
}


/*	Function Name: Feep
 *	Description: feeps the bell.
 *	Arguments: none.
 *	Returns: none.
 */
void
Feep()
{
  XBell(CurDpy, 0);
}


/********************************************************************************
 *       Main function								*
 ********************************************************************************/
void
main(argc, argv)
int argc;
char **argv;
{
  Widget top;
  String filename = NULL;

  top = XtInitialize( "xed", "Xed", NULL, 0, &argc, argv);
  file_s = init_file_select(top);

  XtGetApplicationResources(top, &app_resources, resources,
			    XtNumber(resources), NULL, 0);

  CurDpy = XtDisplay(top);

  if (argc > 1) {
    Boolean exists;
    filename = argv[1];

    switch ( CheckFilePermissions(filename, &exists)) {
    case NO_READ:
	if (exists)
	    fprintf(stderr, 
		    "File %s exists, and could not be opened for reading.\n", 
		    filename);
	else
	    fprintf(stderr, "File %s %s %s",  filename, "does not exist,",
		    "and the directory could not be opened for writing.\n");
	exit(1);
    case READ_OK:
    case WRITE_OK:         
    case CREATE_OK:
	makeButtonsAndBoxes(top, filename);
	break;
    default:
	fprintf(stderr, "%s %s", "Internal function MaybeCreateFile()",
		"returned unexpected value.\n");
	exit(1);
    }
  }  
  else
      makeButtonsAndBoxes(top, NULL);
  
  creat_popups(top);
  XtRealizeWidget(top);

  
          
  XDefineCursor(XtDisplay(top),XtWindow(top),
		XCreateFontCursor( XtDisplay(top), XC_left_ptr));
  XtMainLoop();
}
