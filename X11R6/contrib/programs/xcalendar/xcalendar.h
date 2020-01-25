/*
 *
 * 	Copyright (C) 1988 Massachusetts Institute of Technology	
 *	
 */

/*

   Permission to use, copy, modify, and distribute this
   software and its documentation for any purpose and without
   fee is hereby granted, provided that the above copyright
   notice appear in all copies and that both that copyright
   notice and this permission notice appear in supporting
   documentation, and that the name of M.I.T. not be used in
   advertising or publicity pertaining to distribution of the
   software without specific, written prior permission.
   M.I.T. makes no representations about the suitability of
   this software for any purpose.  It is provided "as is"
   without express or implied warranty.

*/

/***********************************************************************
* Copyright 1990-1994 by Richard Bingle and Purdue University.  All rights
* reserved.  Some individual files may be covered by other copyrights.
*
* Redistribution and use in source and binary forms are permitted
* provided that this entire copyright notice is duplicated in all such
* copies.  Neither the name of the University, nor the name of the author
* may be used to endorse or promote products derived from this material
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
* MERCHANTIBILITY AND FITNESS FOR ANY PARTICULAR PURPOSE.
************************************************************************/


#include <stdio.h>
#include  <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifndef XlibSpecificationRelease
#define XlibSpecificationRelease 0
#endif
#if (XlibSpecificationRelease >= 5)
#include <X11/Xfuncs.h>
#endif

#include <X11/Shell.h>

#ifdef ATHENA
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/AsciiSrc.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Paned.h>
#else
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/SashP.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#endif

#ifdef XI18N
#include <X11/Xaw/Xawi18n.h>
#endif

#if defined(SYSV) || defined(__osf__) || defined(SVR4)
#include <dirent.h>
#else
#include <sys/dir.h>
#endif
#include <sys/stat.h>

#include <errno.h>
extern int errno;

typedef struct _appRes{
   Boolean reverseVideo;
   Boolean setBackground;
   Pixel   markBackground;
   Boolean setForeground;
   Pixel   markForeground;
   char    *monthnames;
   char    *monthnms;
   char    *daynames;
   char    *daynms;
   char    *date;
   char    *months[12];
   int     firstDay;
   Boolean markOnStartup;
   char    *helpFile;
   int     textBufferSize;
   char	   *calendarDir;
   char    *holidayFile;
   Boolean oldStyle;
   Boolean markCurrent;
   int     updateCurrent;
   Pixel   currentForeground;
   Boolean markHoliday;
   Pixel   holidayForeground;
} AppResourcesRec, *AppResources;

typedef struct holiday{
   Cardinal      day;
   Cardinal      month;
   Cardinal      year;
   char     *text;
} *Holiday;

typedef struct _list{
   char     *list;
   Cardinal pos,len,size;
} ListRec, *List;


typedef struct _llist{
   caddr_t element;
   caddr_t data;
   struct _llist *prev;
   struct _llist *next;
} LList;

typedef struct _markcolors{
   Pixel     dayBackground,dayForeground;
} MarkColors;

#define DayForeground(p) ((MarkColors *)(p)->data)->dayForeground
#define DayBackground(p) ((MarkColors *)(p)->data)->dayBackground
#define XMS(foo)        XmStringCreateSimple(foo)

extern Widget   help_button;

typedef struct _dayeditor {
  Cardinal day,month,year;
  Widget   shell;
  Widget   title;
  Widget   editor;
#ifdef ATHENA
  XawTextEditType mode;
#else
  int      mode;
#endif
  Widget   button;		/* button which popped the editor */
  Widget   doneButton;
  Widget   saveButton;
  Widget   clearEntry;
  Widget   prevday;
  Widget   succday;
  Widget   holiday;
  Boolean  open;
  Boolean     saved;
  Boolean     used;
  char     *filename;
  char     *buffer;
  int      bufSize;
} DayEditorRec, *DayEditor;

extern List       CreateList();
extern Cardinal   PushWidgetOnList();
extern Widget     GetWidgetFromList();
extern DayEditor  GetEditorFromList();
extern Cardinal   PushEditorOnList();
extern Cardinal	  PushOnList();
extern Boolean       ReadFile();
extern Boolean       InitEditors();
extern Boolean       initialized;
#ifdef ATHENA
extern DayEditor  CreateDayEditor();
extern Widget     CreateCalendar();
#else
extern void       HideSashes();
extern DayEditor  CreateMotifDayEditor();
extern Widget     CreateMotifCalendar();
extern void       _XEditResCheckMessages();
#endif
extern void       MarkDayEntry();
extern void       GetResources();
extern void       ChangeTextSource();
extern void       StartEditor();
extern void	  CurrentTic();
extern void	  CloseEditors();
extern Boolean	  is_today();
extern Boolean	  is_holiday();
extern void       LoadHolidays();
extern void       Fill_In_Date();
extern void	  MakeDayList();
extern int	  current_month();
extern int	  current_day();
extern int	  current_year();
extern int	  FirstDay();
extern int	  NumberOfDays();
extern int	  jan1();
extern int	  write_to_file();
extern int	  read_file();
extern LList	  *pput();
extern LList      *premove();
extern LList	  *lookup();

extern Display    *dsp;
extern Window     markedDaysKey;
extern Widget     toplevel;
extern char       *smonth[];
extern int        debug;
extern LList      *dayEntryList;
extern List       daylist;
extern List       holilist;
extern Cardinal   day,month,year;
extern AppResourcesRec appResources;	/* application specific resources */
extern LList      *allDaysList;

#define ListLength(listp) ((listp)->pos)
#define GetWidgetList(listp) ((WidgetList)(listp)->list)

#define PushEditorOnList(list,editor) PushOnList(list,(caddr_t)&editor)
#define PushWidgetOnList(list,widget) PushOnList(list,(caddr_t)&widget)

#define GetEditorFromList(listp,index) \
  (index > listp->pos || index <= 0)? NULL : *((DayEditor *)listp->list + index -1)
#define GetWidgetFromList(listp,index) \
  (index > listp->pos || index <= 0)? NULL : *((WidgetList)listp->list + index -1)

#define EmptyBuffer(editor) (strlen((editor)->buffer) == 0)
#define TextSize(editor) strlen((editor)->buffer)
