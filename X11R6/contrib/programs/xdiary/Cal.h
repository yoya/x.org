/*
 * Copyright (c) 1994  Harris Computer Systems Division
 *
 * The X Consortium, and any party obtaining a copy of these files from the X
 * Consortium, directly or indirectly, is granted, free of charge, a full and
 * unrestricted irrevocable, world-wide, paid up, royalty-free, nonexclusive
 * right and license to deal in this software and documentation files (the
 * "Software"), including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons who receive copies from any such party to do so.
 * This license includes without limitation a license to do the foregoing
 * actions under any patents of the party supplying this software to the X
 * Consortium.
 * 
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR
 * THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 * 
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
 * ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE
 * THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
 * GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
 * USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 * DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
 * PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
 * EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 */

#ifndef _CalendarWidget_h
#define _CalendarWidget_h

#include <X11/Core.h>
#include "StrRect.h"
#include "Date.h"

typedef struct {
   unsigned int mask;
   union {
      char * string;
      Pixmap pixmap;
   } info[9]; /* one per gravity */
} DayInfo;

typedef DayInfo MonthInfo[31];

/* Resource Names: */
#ifndef XtNlineWidth
#define XtNlineWidth        "lineWidth"
#endif
#define XtNdigitFont        "digitFont"
#define XtNweekdayFont      "weekdayFont"
#define XtNtitleFont        "titleFont"
#define XtNinfoFont         "infoFont"
#define XtNdigitGravity     "digitGravity"
#define XtNdigitNames       "digitNames"
#define XtNweekdayNames     "weekdayNames"
#define XtNmonthNames       "monthNames"
#ifndef XtNhighlight
#define XtNhighlight        "highlight"
#endif
#define XtNshowYear         "showYear"
#define XtNstartingWeekday  "startingWeekday"

/* Resource Classes: */
#ifndef XtCLineWidth
#define XtCLineWidth        "LineWidth"
#endif
#define XtCCalendarFont     "CalendarFont"
#define XtCDigitGravity     "DigitGravity"
#define XtCDigitNames       "DigitNames"
#define XtCWeekdayNames     "WeekdayNames"
#define XtCMonthNames       "MonthNames"
#ifndef XtCHighlight
#define XtCHighlight        "Highlight"
#endif
#define XtCShowYear         "ShowYear"
#define XtCStartingWeekday  "StartingWeekday"

extern WidgetClass calendarWidgetClass;

typedef struct _CalendarClassRec *CalendarWidgetClass;
typedef struct _CalendarRec      *CalendarWidget;

#define DIstringNW  (1L<<0)
#define DIstringN   (1L<<1)
#define DIstringNE  (1L<<2)
#define DIstringW   (1L<<3)
#define DIstringC   (1L<<4)
#define DIstringE   (1L<<5)
#define DIstringSW  (1L<<6)
#define DIstringS   (1L<<7)
#define DIstringSE  (1L<<8)

#define DIpixmapNW  (1L<<9)
#define DIpixmapN   (1L<<10)
#define DIpixmapNE  (1L<<11)
#define DIpixmapW   (1L<<12)
#define DIpixmapC   (1L<<13)
#define DIpixmapE   (1L<<14)
#define DIpixmapSW  (1L<<15)
#define DIpixmapS   (1L<<16)
#define DIpixmapSE  (1L<<17)


#define XtRcalendarDefaultTranslations \
   "<Btn1Down>:    select()\n\
    <Btn1Motion>:  select()\n\
    <Btn1Up>:      notify()"

/* Public functions: */
extern void CalendarSetDate(Widget, Date);
extern void CalendarShowMonth(Widget, Date);
extern void CalendarGetDate(Widget, Date*);
extern void CalendarIncMonth(Widget);
extern void CalendarDecMonth(Widget);
extern void CalendarIncYear(Widget);
extern void CalendarDecYear(Widget);
extern void CalendarIncDay(Widget, Boolean);
extern void CalendarDecDay(Widget, Boolean);
extern void CalendarIncWeek(Widget, Boolean);
extern void CalendarDecWeek(Widget, Boolean);
extern Date GetTodaysDate(void);
extern Date DateConverter(Widget, char*);
extern char* CalendarPrettyDate(Widget);

#endif /* _CalendarWidget_h */
