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

/*==========================================================================*/
/*                              Header Files:                               */
/*==========================================================================*/
#include <stdio.h>
#include <X11/Xos.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Xmu/Converters.h>
#include "CalP.h"

#ifdef DEBUG
#define BEGIN(str) (fprintf(stderr, "%s()\n", (str)))
#define END(str) ;
#else
#define BEGIN(str) ;
#define END(str) ;
#endif


/*==========================================================================*/
/*                            Forward References:                           */
/*==========================================================================*/
static void initialize_line_GCs(CalendarWidget);
static void initialize_font_GCs_and_info(CalendarWidget);
static void compute_minimum_cell_size(CalendarWidget);
static void compute_cell_geometry(CalendarWidget);
static void free_line_GCs(CalendarWidget);
static void free_font_GCs_and_info(CalendarWidget);
static void compute_grid(CalendarWidget);
static void draw_grid(CalendarWidget);
static void draw_weekdays(CalendarWidget);
static void draw_title(CalendarWidget);
static void draw_digits(CalendarWidget);
static void compute_title_string(CalendarWidget);
static void toggle_highlight(CalendarWidget, int);
static void rotate_weekdays(CalendarWidget);
static void calendar_update(CalendarWidget);
static void compute_month_data(CalendarWidget);
static void clear_calendar(CalendarWidget);
static int compute_weekday(Date);
static void toggle_highlight(CalendarWidget, int);




/*==========================================================================*/
/*                            Static Global Data:                           */
/*==========================================================================*/
static int days_in_month[] =  {
   /* jan feb mar apr may jun jul aug sep oct nov dec */
       31, -1, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


/*==========================================================================*/
/*                              Resource List:                              */
/*==========================================================================*/
#define offset(field) XtOffset(CalendarWidget, field)
static XtResource calendar_resources[] = {
   {
      XtNcallback,
      XtCCallback,
      XtRCallback,
      sizeof(XtPointer),
      offset(calendar.callback),
      XtRCallback,
      NULL
   },
   {
      XtNlineWidth,
      XtCLineWidth,
      XtRInt,
      sizeof(int),
      offset(calendar.line_width),
      XtRImmediate,
      (XtPointer)DEFAULT_LINE_WIDTH
   },
   {
      XtNforeground,
      XtCForeground,
      XtRPixel,
      sizeof(Pixel),
      offset(calendar.foreground),
      XtRString,
      XtDefaultForeground
   },
   {
      XtNbackground,
      XtCBackground,
      XtRPixel,
      sizeof(Pixel),
      offset(calendar.background),
      XtRString,
      XtDefaultBackground
   },
   {
      XtNdigitFont,
      XtCCalendarFont,
      XtRFont,
      sizeof(Font),
      offset(calendar.digit_font),
      XtRString,
      CALENDAR_DEFAULT_DIGIT_FONT
   },
   {
      XtNweekdayFont,
      XtCCalendarFont,
      XtRFont,
      sizeof(Font),
      offset(calendar.weekday_font),
      XtRString,
      CALENDAR_DEFAULT_WEEKDAY_FONT
   },
   {
      XtNtitleFont,
      XtCCalendarFont,
      XtRFont,
      sizeof(Font),
      offset(calendar.title_font),
      XtRString,
      CALENDAR_DEFAULT_TITLE_FONT
   },
   {
      XtNinfoFont,
      XtCCalendarFont,
      XtRFont,
      sizeof(Font),
      offset(calendar.info_font),
      XtRString,
      CALENDAR_DEFAULT_INFO_FONT
   },
   {
      XtNdigitGravity,
      XtCDigitGravity,
      RGravity,
      sizeof(Gravity),
      offset(calendar.digit_gravity),
      XtRImmediate,
      (XtPointer)Center
   },
   {  XtNdigitNames,
      XtCDigitNames,
      XtRStringTable,
      sizeof(StringTable),
      offset(calendar.digit_names),
      XtRString,
      XtNDefaultDigitNames
   },
   {  XtNweekdayNames,
      XtCWeekdayNames,
      XtRStringTable,
      sizeof(StringTable),
      offset(calendar.weekday_names),
      XtRString,
      XtNDefaultWeekdayNames
   },
   {  XtNmonthNames,
      XtCMonthNames,
      XtRStringTable,
      sizeof(StringTable),
      offset(calendar.month_names),
      XtRString,
      XtNDefaultMonthNames
   },
   {  XtNhighlight,
      XtCHighlight,
      XtRBoolean,
      sizeof(Boolean),
      offset(calendar.highlight),
      XtRImmediate,
      (XtPointer)True
   },
   {  XtNshowYear,
      XtCShowYear,
      XtRBoolean,
      sizeof(Boolean),
      offset(calendar.show_year),
      XtRImmediate,
      (XtPointer)True
   },
   {  XtNstartingWeekday,
      XtCStartingWeekday,
      XtRDayName,
      sizeof(XtDayName),
      offset(calendar.starting_weekday),
      XtRImmediate,
      (XtPointer)Sunday
   }
};


/*==========================================================================*/
/*                           Action Declarations:                           */
/*==========================================================================*/
static void select_cell_action(Widget, XEvent*, String*, Cardinal*);
static void default_button_up_action(Widget, XEvent*, String*, Cardinal*);


/*==========================================================================*/
/*                              Actions Table:                              */
/*==========================================================================*/
static XtActionsRec calendar_actions[] = {
   { "select", select_cell_action },
   { "notify", default_button_up_action }
};

static char calendar_default_translations[] = XtRcalendarDefaultTranslations;


/*==========================================================================*/
/*                           Method Declarations:                           */
/*==========================================================================*/
static void calendar_initialize_method(Widget, Widget, ArgList, Cardinal*);
static void calendar_class_initialize_method(void);
static void calendar_expose_method(Widget, XEvent*, Region);
static void calendar_destroy_method(Widget);
static void calendar_resize_method(Widget);
static Boolean calendar_set_values_method(
   Widget, Widget, Widget, ArgList, Cardinal*
);  
static XtGeometryResult calendar_query_geometry_method(
   Widget, XtWidgetGeometry*, XtWidgetGeometry*
);


/*==========================================================================*/
/*                       Class Record Initialization:                       */
/*==========================================================================*/
CalendarClassRec calendarClassRec = {
   {
      /* CORE CLASS PART:      */
      /* superclass            */ (WidgetClass)&coreClassRec,
      /* class_name            */ "Calendar",
      /* widget_size           */ sizeof(CalendarRec),
      /* class_initialize      */ calendar_class_initialize_method,
      /* class_part_initialize */ NULL,
      /* class_inited          */ FALSE,
      /* initialize            */ calendar_initialize_method,
      /* initialize_hook       */ NULL,
      /* realize               */ XtInheritRealize,
      /* actions               */ calendar_actions,
      /* num_actions           */ XtNumber(calendar_actions),
      /* resources             */ calendar_resources,
      /* num_resources         */ XtNumber(calendar_resources),
      /* xrm_class             */ NULLQUARK,
      /* compress_motion       */ TRUE,
      /* compress_exposure     */ TRUE,
      /* compress_enterleave   */ TRUE,
      /* visible_interest      */ FALSE,
      /* destroy               */ calendar_destroy_method,
      /* resize                */ calendar_resize_method,
      /* expose                */ calendar_expose_method,
      /* set_values            */ calendar_set_values_method,
      /* set_values_hook       */ NULL,
      /* set_values_almost     */ XtInheritSetValuesAlmost,
      /* get_values_hook       */ NULL,
      /* accept_focus          */ NULL,
      /* version               */ XtVersion,
      /* callback_private      */ NULL,
      /* tm_table              */ calendar_default_translations,
      /* query_geometry        */ calendar_query_geometry_method,
      /* display_accelerator   */ XtInheritDisplayAccelerator,
      /* extension             */ NULL
   },
   {
      /* CALENDAR CLASS PART:  */
      /* dummy field           */ 0
   }
};

/* Define the calendar widget class in terms of the above record. */
WidgetClass calendarWidgetClass = (WidgetClass)&calendarClassRec;


static XtConvertArgRec tempConvertArg[] = {
   {  
      XtBaseOffset,
      (XtPointer)XtOffset(Widget, core.screen),
      sizeof(Screen *)
   }
};


/*==========================================================================*/
/*                           Method Definitions:                            */
/*==========================================================================*/
static void calendar_class_initialize_method() {
   /* Register the string to gravity resource converter. */
   XtAddConverter(
      XtRString,                   /* source type */
      RGravity,                    /* target type */
      GravityConverter,            /* converter routine */
      NULL,                        /* args for converter */
      0                            /* num args for converter */
   );

   /* Register the string to gravity resource converter. */
   XtAddConverter(
      XtRString,                   /* source type */
      XtRDayName,                  /* target type */
      DayNameConverter,            /* converter routine */
      NULL,                        /* args for converter */
      0                            /* num args for converter */
   );

   /* Register the string to string table resource converter. */
   XtAddConverter(
      XtRString,                   /* source type */
      XtRStringTable,              /* target type */
      StringTableConverter,        /* converter routine */
      NULL,                        /* args for converter */
      0                            /* num args for converter */
   );

   /* Register the string to bitmap resource converter. */
   XtAddConverter(
      XtRString,                   /* source type */
      XtRBitmap,                   /* target type */
      XmuCvtStringToBitmap,        /* converter routine */
      tempConvertArg,              /* args for converter */
      XtNumber(tempConvertArg)     /* num args for converter */
   );
}


static void calendar_initialize_method(
   Widget request_arg,
   Widget new_arg,
   ArgList unused1,
   Cardinal* unused2
) {
   CalendarWidget request = (CalendarWidget)request_arg;
   CalendarWidget new = (CalendarWidget)new_arg;
   XrmValue from;
   XrmValue to;
   int i;
   float initial_cell_width  = (float)request->core.width / (float)COLS;
   float initial_cell_height = (float)request->core.height / (float)ROWS;

   BEGIN("calendar_initialize_method");

   /* Now entering the foggy zone. */
   new->calendar.state = foggy;

   /* First check resources: */

   /* Did we get 7 weekday names? */
   if (StringTableNumber(request->calendar.weekday_names) != 7) {
      fprintf(stderr, "CalendarWidget: You must specify 7 weekday names.\n");

      /* Explicitly invoke converter on default value */
      from.addr = XtNDefaultWeekdayNames;
      from.size = sizeof(StringTable);
      XtConvert((Widget)new, XtRString, &from, XtRStringTable, &to);
      request->calendar.weekday_names = *(StringTable*)to.addr;
   }
   
   /* Make a widget specific copy so we can twiddle with it if necessary. */
   new->calendar.weekday_names 
      = StringTableCopy(request->calendar.weekday_names);

   /* Do we need to rotate the weekdays? */
   if (new->calendar.starting_weekday != SUNDAY) {
      rotate_weekdays(new);
   }

   /* Did we get 12 month names? */
   if (StringTableNumber(request->calendar.month_names) != 12) {
      fprintf(stderr, "CalendarWidget: You must specify 12 month names.\n");

      /* Explicitly invoke converter on default value */
      from.addr = XtNDefaultMonthNames;
      from.size = sizeof(StringTable);
      XtConvert((Widget)new, XtRString, &from, XtRStringTable, &to);
      new->calendar.month_names = *(StringTable*)to.addr;
      
   }

   /* Did we get 31 digit names? */
   if (StringTableNumber(request->calendar.digit_names) != 31) {
      fprintf(stderr, "CalendarWidget: You must specify 31 digit names.\n");

      /* Explicitly invoke converter on default value */
      from.addr = XtNDefaultDigitNames;
      from.size = sizeof(StringTable);
      XtConvert((Widget)new, XtRString, &from, XtRStringTable, &to);
      new->calendar.digit_names = *(StringTable*)to.addr;
   }

   range_check(
      XtNlineWidth,
      new->calendar.line_width,
      MIN_LINE_WIDTH,
      MAX_LINE_WIDTH
   );

   range_check(
      XtNdigitGravity,
      new->calendar.digit_gravity,
      NorthWest,
      SouthEast
   );

   /* Next compute initial state: */

   /* 
    * Compute the lengths of the month, weekday and digit names once and keep
    * around so we won't have to recompute them every time they're drawn.
    */
   for (i=0; i < DAYS_IN_WEEK; i++) {
      new->calendar.weekday_name_lengths[i] =
         strlen(new->calendar.weekday_names[i]);
   }
   for (i=0; i < MONTHS_IN_YEAR; i++) {
      new->calendar.month_name_lengths[i] =
         strlen(new->calendar.month_names[i]);
   }
   for (i=0; i < MAX_DAYS_IN_MONTH; i++) {
      new->calendar.digit_name_lengths[i] =
         strlen(new->calendar.digit_names[i]);
   }

   initialize_line_GCs(new);
   initialize_font_GCs_and_info(new);
   compute_minimum_cell_size(new);

   new->calendar.real_cell_width
      = MAX(new->calendar.min_cell_width+1, initial_cell_width);
   new->calendar.real_cell_height
      = MAX(new->calendar.min_cell_height+1, initial_cell_height);

   new->calendar.old_window_width = -1;
   new->calendar.old_window_height = -1;

   if (new->core.width == 0) {
      new->core.width = WINDOW_WIDTH(new->calendar.real_cell_width);
   }
   if (new->core.height == 0) {
      new->core.height = WINDOW_HEIGHT(new->calendar.real_cell_height);
   }

   compute_grid(new);
   compute_cell_geometry(new);

   /* Calendar defaults to current date. */
   new->calendar.date = GetTodaysDate();

   /* Allocate a fixed amount of space for the title string. */
   new->calendar.title_string = (char*)XtMalloc(MAX_TITLE_LEN*sizeof(char));

   /* Force a re-calculation of the month data for the current year. */
   new->calendar.current_year = 0;
   compute_month_data(new);

   new->calendar.highlight_days_in_february 
      = new->calendar.days_in_february;

   /* Calendar highlights current day by default, if highligh is true. */
   new->calendar.highlight_date = new->calendar.date;

   compute_title_string(new);

   END("calendar_initialize_method");
}


static void calendar_expose_method(
   Widget widget_arg,
   XEvent* event_arg,
   Region unused
) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   XExposeEvent* event = (XExposeEvent*)event_arg;
   BEGIN("calendar_expose_method");

   /* Okay, it's safe to draw now. */
   widget->calendar.state = solid;

   draw_weekdays(widget);
   draw_title(widget);
   draw_digits(widget);

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   }

   draw_grid(widget);
   
   END("calendar_expose_method");
}


static Boolean calendar_set_values_method(
   Widget current_arg,
   Widget request_arg,
   Widget new_arg,
   ArgList unused1,
   Cardinal* unused2
) {
   CalendarWidget current = (CalendarWidget)current_arg;
   CalendarWidget request = (CalendarWidget)request_arg;
   CalendarWidget new = (CalendarWidget)new_arg;
   Boolean redraw = False;
   Boolean resize = False;
   Boolean new_weekday_names = False;
   Boolean new_starting_weekday = False;
   Display* display;
   XGCValues values;
   XtGCMask mask;
   int i; 
   int new_width;
   int new_height;

   BEGIN("calendar_set_values_method");

   display = XtDisplay(current);
   
   /* Line Width: */
   if  (request->calendar.line_width != current->calendar.line_width) {
      new->calendar.line_width = bound(
         request->calendar.line_width,
         MIN_LINE_WIDTH,
         MAX_LINE_WIDTH
      );
   }
   if (new->calendar.line_width != current->calendar.line_width) {
      mask = GCLineWidth;
      values.line_width = new->calendar.line_width;
      XChangeGC(
         display,
         new->calendar.draw_gc,
         mask,
         &values
      );
      redraw = True;
   }

   /* Digit Gravity: */
   if (request->calendar.digit_gravity != current->calendar.digit_gravity) {
      new->calendar.digit_gravity = (Gravity)bound(
         (int)request->calendar.digit_gravity,
         (int)NorthWest,
         (int)SouthEast
      );
   }
   if (new->calendar.digit_gravity != current->calendar.digit_gravity) {
      redraw = True;
   }
   
   /* Weekday Names: */
   if (request->calendar.weekday_names != current->calendar.weekday_names) {
      if (StringTableNumber(request->calendar.weekday_names) != DAYS_IN_WEEK) {
         fprintf(stderr, "CalendarWidget: must specify 7 weekday names\n");
      } else {
         /* Copy the string table so we can rotate it if necessary. */
         new->calendar.weekday_names = 
            StringTableCopy(request->calendar.weekday_names);
         for (i=0; i < DAYS_IN_WEEK; i++) {
            new->calendar.weekday_name_lengths[i] 
               = strlen(request->calendar.weekday_names[i]);
         }
         new_weekday_names = True;
         redraw = True;
         resize = True;
      }
   }

   /* Month Names: */
   if (request->calendar.month_names != current->calendar.month_names) {
      if (StringTableNumber(request->calendar.month_names) != MONTHS_IN_YEAR) {
         fprintf(stderr, "CalendarWidget: must specify 12 month names\n");
      } else {
         new->calendar.month_names = request->calendar.month_names;
         for (i=0; i < MONTHS_IN_YEAR; i++) {
            new->calendar.month_name_lengths[i] 
               = strlen(request->calendar.month_names[i]);
         }
         compute_title_string(new);
         redraw = True;
         resize = True;
      }
   }

   /* Digit Names: */
   if (request->calendar.digit_names != current->calendar.digit_names) {
      if (StringTableNumber(request->calendar.digit_names) != MAX_DAYS_IN_MONTH) {
         fprintf(stderr, "CalendarWidget: must specify 31 digit names\n");
      } else {
         new->calendar.digit_names = request->calendar.digit_names;
         for (i=0; i < MAX_DAYS_IN_MONTH; i++) {
            new->calendar.digit_name_lengths[i] 
               = strlen(request->calendar.digit_names[i]);
         }
         redraw = True;
         resize = True;
      }
   }

   /* Starting Weekday. */
   new_starting_weekday = (request->calendar.starting_weekday
                              != current->calendar.starting_weekday);
   if (new_starting_weekday 
       && (request->calendar.starting_weekday < SUNDAY
       || request->calendar.starting_weekday > SATURDAY)) {
      /* Bogus value specified. */
      fprintf(
         stderr, "CalendarWidget: invalid starting weekday, use [0..6]\n"
      );
      new_starting_weekday = FALSE;
   }
   if (new_starting_weekday && new_weekday_names) {
      new->calendar.starting_weekday = request->calendar.starting_weekday;
      rotate_weekdays(new);
   } else if (new_weekday_names) {
      rotate_weekdays(new);
   } else if (new_starting_weekday) {
      /* Rotate weekdays, taking into account current rotation. */
      new->calendar.starting_weekday =
         ( (7 - current->calendar.starting_weekday) 
           + request->calendar.starting_weekday ) %7;
      rotate_weekdays(new);
      new->calendar.starting_weekday = request->calendar.starting_weekday;
      redraw = True;
   }

   /* Highlight. */
   if (request->calendar.highlight != current->calendar.highlight) {
      new->calendar.highlight = request->calendar.highlight;
   }

   if (resize) {
      compute_minimum_cell_size(new);
      new_width = WINDOW_WIDTH(new->calendar.min_cell_width);
      new_height = WINDOW_HEIGHT(new->calendar.min_cell_height);
      XtMakeResizeRequest((Widget)new, new_width, new_height, NULL, NULL);
   }

   END("calendar_set_values_method");

   /* Return true if value change requires expose event to redraw. */
   return (redraw);
}


static void calendar_resize_method(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   int window_width = widget->core.width;
   int window_height = widget->core.height;
   float avail_cell_width  = (float)window_width  / (float)COLS;
   float avail_cell_height = (float)window_height / (float)ROWS;

   BEGIN("calendar_resize_method");

   /* Now entering the foggy zone. */
   widget->calendar.state = foggy;

   widget->calendar.real_cell_width = low_bound(
      avail_cell_width, 
      widget->calendar.min_cell_width
   );

   widget->calendar.real_cell_height = low_bound(
      avail_cell_height, 
      widget->calendar.min_cell_height
   );

   compute_grid(widget);
   compute_cell_geometry(widget);

   /* Don't need to redraw the calendar, an expose event follows. */
   END("calendar_resize_method");
}


static XtGeometryResult calendar_query_geometry_method(
   Widget widget_arg,
   XtWidgetGeometry *proposed,
   XtWidgetGeometry *preferred
) {
   /* 
    * There are three scenarios:
    *
    *   1. The geometry proposed by our parent is the same as our preferred
    *      geometry.  Return XtGeometryYes to say, "Yes, I'm happy with your
    *      proposed geometry so please resize me to it."
    *
    *   2. My preferred geometry is the same as our current geometry.  Return
    *      XtGeometryNo to say, "I'm at my preferred geometry so please don't
    *      resize me."
    *
    *   3. The geometry proposed by our parent is not our preferred geometry,
    *      nor am I at my preferred geometry.  Return XtGeometryAlmost to say
    *      "I'm sending you my preferred geometry.  Can't you try again to get
    *      me the size I want?"
    *
    * Note that my parent is free to totally ignore this information, just like
    * most parents ignore what their kids tell them.
    */

   CalendarWidget widget = (CalendarWidget)widget_arg;
   BEGIN("calendar_query_geometry_method");

   preferred->request_mode = CWWidth|CWHeight;
   preferred->width = WINDOW_WIDTH(widget->calendar.min_cell_width) + 7;
   preferred->height = WINDOW_HEIGHT(widget->calendar.min_cell_height) + 8;

   if ( ((proposed->request_mode & (CWWidth|CWHeight)) == (CWWidth|CWHeight))
         && ((proposed->width == preferred->width) 
         && (proposed->height == preferred->height)) ) {
      /* Yes, I like your proposal. */
      return XtGeometryYes;
   } else {
      if ( (preferred->width == widget->core.width) &&
           (preferred->height == widget->core.height) ) {
         /* Don't resize me!  I'm already at my preferred geometry. */
         return XtGeometryNo;
      } else {
         /* Please try again. */
         return XtGeometryAlmost;
      }
   }

   END("calendar_query_geometry_method");
}


static void calendar_destroy_method(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   free_line_GCs(widget);
   free_font_GCs_and_info(widget);
}


/*==========================================================================*/
/*                           Action Definitions:                            */
/*==========================================================================*/
static void select_cell_action(
   Widget widget_arg,
   XEvent* event_arg,
   String* unused1,
   Cardinal* unused2
) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   XButtonEvent* event = (XButtonEvent*)event_arg;
   int window_width = widget->core.width;
   int window_height = widget->core.height;
   float cell_width = widget->calendar.real_cell_width;
   float cell_height = widget->calendar.real_cell_height;
   int current_x_cell = widget->calendar.current_x_cell;
   int current_y_cell = widget->calendar.current_y_cell;
   int x = event->x;
   int y = event->y;
   int new_y_cell = y / cell_height;
   int new_x_cell = x / cell_width;
   int cellnum;

   if (widget->calendar.state == foggy) {
      return;
   }

   /* Ignore events that occur outside the window.  These are motion */
   /* events that started inside the window but were dragged outside. */
   if ((x<1) || (x>window_width-1) || (y<1) || (y>window_height-1)) {
      return;
   }

   /* Ignore repeat events for the same cell. */
   if (new_x_cell == current_x_cell && new_y_cell == current_y_cell) {
      return;
   }

   /* Ignore events in the month/year box. */
   if (new_y_cell<2) {
      return;
   }

   /* Ignore events outide the cells that we care about for current month. */
   cellnum = CELLXYtoCELLNUM(new_x_cell, new_y_cell);
   if (cellnum < widget->calendar.month_start_cellnum
       || cellnum > widget->calendar.month_end_cellnum) {
      return;
   }

   widget->calendar.cell_selected = True;

   /* Reset the old selected cell. */
   if (SYNC(widget)) {
      toggle_highlight(widget, OFF);
   }

   /* Highlight the newly selected cell. */
   widget->calendar.highlight_date.year = widget->calendar.date.year;
   widget->calendar.highlight_date.month = widget->calendar.date.month;
   widget->calendar.highlight_date.day = CELLNUMtoDAYNUM(cellnum);
   toggle_highlight(widget, ON);

   return;
}


static void default_button_up_action(
   Widget widget_arg, XEvent* event_arg, String* s, Cardinal* c
) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   XButtonEvent* event = (XButtonEvent*)event_arg;
   if (!widget->calendar.cell_selected) {
      return;
   }
   widget->calendar.cell_selected = False;

   /* The button-down or button-motion event inverted the cell, so all     */
   /* we have to do is call any callbacks that the application registered. */
   XtCallCallbacks(
      (Widget)widget, XtNcallback, &widget->calendar.highlight_date
   );
}


/*==========================================================================*/
/*                         Method Support Routines:                         */
/*==========================================================================*/
static void initialize_line_GCs(CalendarWidget widget) {
   XGCValues values;
   XtGCMask mask;

   mask = GCForeground | GCBackground | GCLineWidth;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.line_width = widget->calendar.line_width;
   widget->calendar.draw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground;
   values.foreground = widget->calendar.background;
   values.background = widget->calendar.foreground;
   widget->calendar.undraw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground | GCFunction;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.function = GXinvert;
   widget->calendar.invert_gc = XtGetGC((Widget)widget, mask, &values);
}


static void initialize_font_GCs_and_info(CalendarWidget widget) {
   XGCValues values;
   XtGCMask mask;

   /* Digit font */
   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.font = widget->calendar.digit_font;
   widget->calendar.digit_draw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.background;
   values.background = widget->calendar.foreground;
   values.font = widget->calendar.digit_font;
   widget->calendar.digit_undraw_gc = XtGetGC((Widget)widget, mask, &values);

   widget->calendar.digit_fsp = XQueryFont(
      XtDisplay(widget),
      XGContextFromGC(widget->calendar.digit_draw_gc)
   );

   /* Weekday font */
   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.font = widget->calendar.weekday_font;
   widget->calendar.weekday_draw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.background;
   values.background = widget->calendar.foreground;
   values.font = widget->calendar.weekday_font;
   widget->calendar.weekday_undraw_gc = XtGetGC((Widget)widget, mask, &values);

   widget->calendar.weekday_fsp = XQueryFont(
      XtDisplay(widget),
      XGContextFromGC(widget->calendar.weekday_draw_gc)
   );

   /* Title font */
   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.font = widget->calendar.title_font;
   widget->calendar.title_draw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.background;
   values.background = widget->calendar.foreground;
   values.font = widget->calendar.title_font;
   widget->calendar.title_undraw_gc = XtGetGC((Widget)widget, mask, &values);

   widget->calendar.title_fsp = XQueryFont(
      XtDisplay(widget),
      XGContextFromGC(widget->calendar.title_draw_gc)
   );

   /* Info font */
   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.foreground;
   values.background = widget->calendar.background;
   values.font = widget->calendar.info_font;
   widget->calendar.info_draw_gc = XtGetGC((Widget)widget, mask, &values);

   mask = GCForeground | GCBackground | GCFont;
   values.foreground = widget->calendar.background;
   values.background = widget->calendar.foreground;
   values.font = widget->calendar.info_font;
   widget->calendar.info_undraw_gc = XtGetGC((Widget)widget, mask, &values);

   widget->calendar.info_fsp = XQueryFont(
      XtDisplay(widget),
      XGContextFromGC(widget->calendar.info_draw_gc)
   );
}


/* 
 * Compute the minimum allowable height and width of a cell based on the
 * specified fonts.  This is done during initialization and when XtSetValues
 * changes a font or string table.
 */
static void compute_minimum_cell_size(CalendarWidget new) {
   int max_day_digit_width = 0;
   int max_day_digit_height = 0;
   int max_day_name_width = 0;
   int max_day_name_height = 0;
   int max_month_name_width = 0;
   int max_year_width = 0;
   int max_title_width;
   int max_title_height;
   XFontStruct* font;
   char digit_string[4];
   char* string;
   int length;
   int i;
   int temp;

   /* WIDTH: */

   /* Compute the max width needed for all digits in the digit font. */
   font = new->calendar.digit_fsp;
   for (i=0; i < MAX_DAYS_IN_MONTH; i++) {
      temp = 
         XTextWidth(
            font, 
            new->calendar.digit_names[i],
            new->calendar.digit_name_lengths[i]
         );
      max_day_digit_width = MAX(max_day_digit_width, temp);
   }
   /* Add in a space to avoid cramming. */
   max_day_digit_width += XTextWidth(font, " ", 1);

   /* Compute the max width needed for all weekday names in the names font. */
   font = new->calendar.weekday_fsp;
   for (i=0; i < DAYS_IN_WEEK; i++) {
      string = new->calendar.weekday_names[i];
      length = new->calendar.weekday_name_lengths[i];
      temp = XTextWidth(font, string, length);
      max_day_name_width = MAX(max_day_name_width, temp);
   }
   max_day_name_width += XTextWidth(font, " ", 1);

   /* Compute the max width needed for all month names in title font.  */
   font = new->calendar.title_fsp;
   for (i=0; i < MONTHS_IN_YEAR; i++) {
      string = new->calendar.month_names[i];
      length = new->calendar.month_name_lengths[i];
      temp = XTextWidth(font, string, length);
      max_month_name_width = MAX(max_month_name_width, temp);
   }

   /* Compute the max width needed to hold any 5 digit year in title font. */
   digit_string[1] = NULL;
   for (i=0; i < 10; i++) {
      digit_string[0] = (char) ('0' + i);
      temp = XTextWidth(font, digit_string, 1);
      max_year_width = MAX(max_year_width, temp);
   }
   max_year_width *= 5;

   max_title_width
      = max_month_name_width 
      + max_year_width
      + XTextWidth(font, " ", 1);

   /* Divide this by the number of rows.  Add one for spacing. */
   max_title_width /= (ROWS + 1);

   /* Set the minimum cell width to the max width required above. */
   new->calendar.min_cell_width =
      (float)MAX3(max_day_digit_width, max_day_name_width, max_title_width);

   /* HEIGHT: */
   
   font = new->calendar.digit_fsp;
   max_day_digit_height = font->ascent + font->descent;
  
   font = new->calendar.weekday_fsp;
   max_day_name_height = font->ascent + font->descent;

   font = new->calendar.title_fsp;
   max_title_height = font->ascent + font->descent;

   /* Set the minimum cell height to the max height required above plus */
   /* whatever we need to for the line width and borders. */
   new->calendar.min_cell_height =
      (float) MAX3(max_day_digit_height, max_day_name_height, max_title_height)
      + 2 * INVERT_BORDER 
      + 2 * new->calendar.line_width
      + 2;
}


static void free_line_GCs(CalendarWidget widget) {
   Display* display = XtDisplay(widget);

   XFreeGC(display, widget->calendar.draw_gc);
   XFreeGC(display, widget->calendar.undraw_gc);
   XFreeGC(display, widget->calendar.invert_gc);
}


static void free_font_GCs_and_info(CalendarWidget widget) {
   Display* display = XtDisplay(widget);

   XFreeGC(display, widget->calendar.digit_draw_gc);
   XFreeGC(display, widget->calendar.digit_undraw_gc);
   XFreeFont(display, widget->calendar.digit_fsp);

   XFreeGC(display, widget->calendar.weekday_draw_gc);
   XFreeGC(display, widget->calendar.weekday_undraw_gc);
   XFreeFont(display, widget->calendar.weekday_fsp);

   XFreeGC(display, widget->calendar.title_draw_gc);
   XFreeGC(display, widget->calendar.title_undraw_gc);
   XFreeFont(display, widget->calendar.title_fsp);

   XFreeGC(display, widget->calendar.info_draw_gc);
   XFreeGC(display, widget->calendar.info_undraw_gc);
   XFreeFont(display, widget->calendar.info_fsp);
}


static void rotate_weekdays(CalendarWidget widget) {
   int weekday;
   int i;
   char* save_names[DAYS_IN_WEEK];
   int save_lengths[DAYS_IN_WEEK];

   weekday = widget->calendar.starting_weekday;
   if (!weekday) {
      return;
   }

   /* Save a copy of the table. */
   for (i=0; i < DAYS_IN_WEEK; i++) {
      save_names[i] = widget->calendar.weekday_names[i];
      save_lengths[i] = widget->calendar.weekday_name_lengths[i];
   }

   /* Rotate the original. */
   for (i=0; i < DAYS_IN_WEEK; i++) {
      widget->calendar.weekday_names[i] = save_names[weekday];
      widget->calendar.weekday_name_lengths[i] = save_lengths[weekday];
      weekday++;
      if (weekday == DAYS_IN_WEEK) {
         weekday = 0;
      }
   }
}


/*
 * For simplicity and convenience, the window is divided into a grid of 7 by 8
 * rectangles.  However, vertical lines are not drawn in the top 2 rows of
 * boxes because that's where the month/year text and day-of-week names go.
 */
static void compute_grid(CalendarWidget widget) {
   int window_width = widget->core.width;
   int window_height = widget->core.height;
   float cell_width = widget->calendar.real_cell_width;
   float cell_height = widget->calendar.real_cell_height;
   float min_cell_width = widget->calendar.min_cell_width;
   float min_cell_height = widget->calendar.min_cell_height;
   int bottom_showing = (window_height >= ROWS * min_cell_height);
   int right_showing = (window_width >= COLS * min_cell_width);
   int line_width = widget->calendar.line_width;
   int fudge = line_width % 2;
   int seg;
   int i;

   /* Compute horizontal segments. */
   seg = 0;
   for (i=2; i < HORIZ_SEGMENTS+2; i++, seg++) {
      /* Starting point. */
      widget->calendar.segments[seg].x1 = (short)(0);
      widget->calendar.segments[seg].y1 = (short)(cell_height * i);

      /* Ending point. */
      widget->calendar.segments[seg].x2 = (short)(window_width);
      widget->calendar.segments[seg].y2 = (short)(cell_height * i);
   }

   /* Compute vertical segments. */
   for (i=1; i < VERTI_SEGMENTS+1; i++, seg++) {
      /* Starting point. */
      widget->calendar.segments[seg].x1 = (short)(cell_width * i);
      widget->calendar.segments[seg].y1 = (short)(cell_height * 2);

      /* Ending point. */
      widget->calendar.segments[seg].x2 = (short)(cell_width * i);
      widget->calendar.segments[seg].y2 = (short)(window_height);
   }

   /* Draw a border around the whole grid.  I could do this with an  */
   /* XRectangle, but since I'm calling XDrawSegments, it's one less */
   /* function call to do it this way (could still be slower though) */

   /* draw solid top */
   widget->calendar.segments[seg].x1 = (short)(0);
   widget->calendar.segments[seg].y1 = (short)(0);
   widget->calendar.segments[seg].x2 = (short)(window_width);
   widget->calendar.segments[seg].y2 = (short)(0);

   /* draw solid left */
   seg++;
   widget->calendar.segments[seg].x1 = (short)(0);
   widget->calendar.segments[seg].y1 = (short)(0);
   widget->calendar.segments[seg].x2 = (short)(0);
   widget->calendar.segments[seg].y2 = (short)(window_height);

   /* draw solid bottom */
   if (bottom_showing) {
      seg++;
      widget->calendar.segments[seg].x1 = (short)(0);
      widget->calendar.segments[seg].y1 = (short)(window_height - fudge);
      widget->calendar.segments[seg].x2 = (short)(window_width);
      widget->calendar.segments[seg].y2 = (short)(window_height - fudge);
   }

   /* draw solid right */
   if (right_showing) {
      seg++;
      widget->calendar.segments[seg].x1 = (short)(window_width - fudge);
      widget->calendar.segments[seg].y1 = (short)(0);
      widget->calendar.segments[seg].x2 = (short)(window_width - fudge);
      widget->calendar.segments[seg].y2 = (short)(window_height);
   }

   widget->calendar.number_segments = seg+1;
}

   
static void draw_grid(CalendarWidget widget) {
   if (widget->calendar.state == foggy) {
      return;
   }

   /* Draw the grid. */
   XDrawSegments(
      XtDisplay(widget),
      XtWindow(widget),
      widget->calendar.draw_gc,
      widget->calendar.segments,
      widget->calendar.number_segments
   );
}


static void compute_cell_geometry(CalendarWidget widget) {
   int window_width = widget->core.width;
   int window_height = widget->core.height;
   int old_window_width = widget->calendar.old_window_width;
   int old_window_height = widget->calendar.old_window_height;
   float cell_width = widget->calendar.real_cell_width;
   float cell_height = widget->calendar.real_cell_height;
   int line_width = widget->calendar.line_width;
   int line_factor = line_width / 2;
   int fudge = line_width % 2;
   int temp_x;
   int temp_y;
   int x,y;

   /*
    * If the window size has changed, re-compute the cell_geometry array.
    * This takes a little while (lots of float arithmetic) but that's okay
    * because it only happens on resize, and we should punish the user.
    */

   if ( old_window_width != window_width
        || old_window_height != window_height ) {

      /* Compute geometries for cells not along top, right or bottom edge. */
      for (x=0; x < COLS-1; x++) {
         for (y=1; y < ROWS-1; y++) {
            GEOMETRY(widget, x, y).x = temp_x = (short)
               (x*cell_width + line_factor+fudge+INVERT_BORDER);
            GEOMETRY(widget, x, y).y = temp_y = (short)
               (y*cell_height + line_factor+fudge+INVERT_BORDER);
            GEOMETRY(widget, x, y).width = (unsigned short)
               ((x+1)*cell_width - line_factor-INVERT_BORDER-temp_x);
            GEOMETRY(widget, x, y).height = (unsigned short)
               ((y+1)*cell_height - line_factor-INVERT_BORDER-temp_y);
         }
      }

      /* Compute geometries for cells along right edge. */
      x = COLS-1;
      for (y=0; y < ROWS-1; y++) {
         GEOMETRY(widget, x, y).x = temp_x = (short)
            (x*cell_width + line_factor+fudge+INVERT_BORDER);
         GEOMETRY(widget, x, y).y = temp_y = (short)
            (y*cell_height + line_factor+fudge+INVERT_BORDER);
         GEOMETRY(widget, x, y).width = (unsigned short)
            (COLS*cell_width - line_factor-INVERT_BORDER-temp_x-fudge);
         GEOMETRY(widget, x, y).height = (unsigned short)
            ((y+1)*cell_height - line_factor-INVERT_BORDER-temp_y);
      }

      /* Compute geometries for cells along bottom edge. */
      /* For a valid calendar, at most the first two cells in the bottom */
      /* row will be used. */
      y = ROWS-1;
      for (x=0; x < 3; x++) {
         GEOMETRY(widget, x, y).x = temp_x = (short)
            (x*cell_width + line_factor+fudge+INVERT_BORDER);
         GEOMETRY(widget, x, y).y = temp_y = (short)
            (y*cell_height + line_factor+fudge+INVERT_BORDER);
         GEOMETRY(widget, x, y).width = (unsigned short)
            ((x+1)*cell_width - line_factor-INVERT_BORDER-temp_x);
         GEOMETRY(widget, x, y).height = (unsigned short)
            (ROWS*cell_height - line_factor-INVERT_BORDER-temp_y-fudge);
      }

      /* Finally, compute the available rectangle for centering the title. */
      widget->calendar.title_geometry.x = temp_x = (short)
         (line_factor + fudge + INVERT_BORDER);
      widget->calendar.title_geometry.y = temp_y = (short)
         (line_factor + fudge + INVERT_BORDER);
      widget->calendar.title_geometry.width = (unsigned short)
         (COLS*cell_width - line_factor-INVERT_BORDER-temp_x-fudge);
      widget->calendar.title_geometry.height = (unsigned short)
         (cell_height - line_factor - INVERT_BORDER - temp_y);

      /* Factor in the day names font size for a better appearance. */
      widget->calendar.title_geometry.height
         = (unsigned short)(widget->calendar.title_geometry.height
                            + cell_height
                            - widget->calendar.weekday_fsp->ascent 
                            - widget->calendar.weekday_fsp->descent 
                            - 1);

      widget->calendar.old_window_height = window_height;
      widget->calendar.old_window_width = window_width;
   }
}


static void draw_weekdays(CalendarWidget widget) {
   if (widget->calendar.state == foggy) {
      return;
   }

   DrawStringsInRects(
      XtDisplay(widget), 
      XtWindow(widget),
      widget->calendar.weekday_draw_gc,
      widget->calendar.weekday_fsp, 
      widget->calendar.weekday_names, 
      widget->calendar.weekday_name_lengths,
      &GEOMETRY(widget, 0, 1),
      DAYS_IN_WEEK,
      South,
      1,0
   );
}


static void draw_digits(CalendarWidget widget) {
   int x,y;
   int start = widget->calendar.month_start_cellnum;
   int end = widget->calendar.month_end_cellnum;

   if (widget->calendar.state == foggy) {
      return;
   }

   CELLNUMtoCELLXY(start,x,y);

   DrawStringsInRects(
      XtDisplay(widget), 
      XtWindow(widget),
      widget->calendar.digit_draw_gc,
      widget->calendar.digit_fsp, 
      widget->calendar.digit_names,
      widget->calendar.digit_name_lengths,
      &GEOMETRY(widget, x, y),
      end - start + 1,
      widget->calendar.digit_gravity,
      1,0
   );
}


static void draw_title(CalendarWidget widget) {
   if (widget->calendar.state == foggy) {
      return;
   }

   DrawStringInRect(
      XtDisplay(widget), 
      XtWindow(widget),
      widget->calendar.title_draw_gc,
      widget->calendar.title_fsp, 
      widget->calendar.title_string,
      widget->calendar.title_string_length,
      &widget->calendar.title_geometry,
      Center,
      1,0
   );
}


/* 
 * Routine to fill in the widget->calendar.month_starting_weekdays[] and the
 * widget->calendar.days_in_february field, given a certain date.  The data is
 * only computed when the widget's current_year has changed.
 */
static void compute_month_data(CalendarWidget widget) {
   int month = widget->calendar.date.month - 1;
   int year = widget->calendar.date.year;
   int leap_days;
   int total_days;
   int weekday;
   int i;

   if (widget->calendar.current_year != year) {
      /* Compute data for the whole year while we're at it. */
      leap_days = 1;            /* Year 0 was a leap year. */
      leap_days += year/4;      /* Add all years divisible by 4. */
      leap_days -= year/100;    /* Subtract all century years. */
      leap_days += year/400;    /* Add back century years divisible by 400. */

      if (A_LEAP_YEAR(year)) {
         leap_days--;
         widget->calendar.days_in_february = 29;
      } else {
         widget->calendar.days_in_february = 28;
      }

      /* Compute days elapsed to beginning of the year. */
      total_days = (year*365) + leap_days;

      /* Compute the starting weekday for january in this year. */
      weekday = DAYStoWEEKDAY(total_days);
      weekday = ROTATE(widget, weekday);
      widget->calendar.month_starting_weekdays[JANUARY] = weekday;

      /* Compute the starting weekday for february in this year. */
      total_days += days_in_month[JANUARY];
      weekday = DAYStoWEEKDAY(total_days);
      weekday = ROTATE(widget, weekday);
      widget->calendar.month_starting_weekdays[FEBRUARY] = weekday;

      /* Compute the starting weekday for the rest of the months. */
      total_days += widget->calendar.days_in_february;
      for (i=MARCH; i <= DECEMBER; i++) {
         weekday = DAYStoWEEKDAY(total_days);
         weekday = ROTATE(widget, weekday);
         widget->calendar.month_starting_weekdays[i] = weekday;
         total_days += days_in_month[i];
      }
      widget->calendar.current_year = year;

      /* Compute the new year string. */
      if (widget->calendar.show_year) {
         sprintf(
            widget->calendar.year_string, "%d" ,widget->calendar.date.year
         );
         widget->calendar.year_string_length =
            strlen(widget->calendar.year_string);
      } else {
         strcpy(widget->calendar.year_string, "");
         widget->calendar.year_string_length = 0;
      }
   }

   widget->calendar.month_start_cellnum =
      widget->calendar.month_starting_weekdays[month];

   widget->calendar.month_end_cellnum =
      widget->calendar.month_start_cellnum 
      + DAYS_IN_MONTH(widget) - 1;
}


static void compute_title_string(CalendarWidget widget) {
   int month = widget->calendar.date.month - 1;

   /* Compute the title string length. */
   widget->calendar.title_string_length =
      widget->calendar.month_name_lengths[month];

   /* Fill in the title string. */
   strcpy(widget->calendar.title_string, widget->calendar.month_names[month]);

   if (widget->calendar.show_year) {
      strcat(widget->calendar.title_string, " ");
      widget->calendar.title_string_length += 1;

      strcat(widget->calendar.title_string, widget->calendar.year_string);
      widget->calendar.title_string_length +=
         widget->calendar.year_string_length;
   }
}


static void clear_calendar(CalendarWidget widget) {
   int month_start_cellnum = widget->calendar.month_start_cellnum;
   int month_end_cellnum = widget->calendar.month_end_cellnum;
   Display* display;
   Window window;
   int x,y;

   if (widget->calendar.state == foggy) {
      return;
   }
   BEGIN("clear_calendar");

   display = XtDisplay(widget);
   window = XtWindow(widget);

   CELLNUMtoCELLXY(month_start_cellnum, x, y);
   
   /* Erase the digits. */
   XFillRectangles(
      display,
      window,
      widget->calendar.undraw_gc,
      &GEOMETRY(widget,x,y),
      month_end_cellnum - month_start_cellnum + 1
   );

   /* Erase the title box.  */
   XFillRectangles(
      display,
      window,
      widget->calendar.undraw_gc,
      &widget->calendar.title_geometry,
      1
   );

   END("clear_calendar");
}


static void calendar_update(CalendarWidget widget) {
   BEGIN("calendar_update");

   clear_calendar(widget);
   compute_month_data(widget);
   compute_title_string (widget);
   draw_title(widget);
   draw_digits(widget);

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   } else {
      widget->calendar.current_x_cell = 0;
      widget->calendar.current_y_cell = 0;
   }

   END("calendar_update");
}


static void toggle_highlight(CalendarWidget widget, int state) {
   int current_x_cell;
   int current_y_cell;
   int day;
   Display* display;
   Window window;
   GC draw_gc;
   GC undraw_gc;

   if (widget->calendar.state == foggy) {
      return;
   }
   BEGIN("toggle_highlight");

   WIDGETtoCELLXY(
      widget,
      current_x_cell,
      current_y_cell
   );

   day = widget->calendar.highlight_date.day - 1;

   if (current_y_cell && widget->calendar.highlight) {

      display = XtDisplay(widget);
      window = XtWindow(widget);

      if (state == ON) {
         draw_gc = widget->calendar.draw_gc;
         undraw_gc = widget->calendar.digit_undraw_gc;
      } else {
         draw_gc = widget->calendar.undraw_gc;
         undraw_gc = widget->calendar.digit_draw_gc;
      }

      XFillRectangles(
         display,
         window,
         draw_gc,
         &GEOMETRY(widget, current_x_cell, current_y_cell),
         1
      );

      DrawStringInRect(
         display, 
         window,
         undraw_gc,
         widget->calendar.digit_fsp, 
         widget->calendar.digit_names[day], 
         widget->calendar.digit_name_lengths[day],
         &GEOMETRY(widget, current_x_cell, current_y_cell),
         widget->calendar.digit_gravity,
         1,0
      );

      /* Eventually this will need to draw the day info as well. */
   }

   if (state == OFF) {
      widget->calendar.current_x_cell = 0;
      widget->calendar.current_y_cell = 0;
   } else {
      widget->calendar.current_x_cell = current_x_cell;
      widget->calendar.current_y_cell = current_y_cell;
   }

   END("toggle_highlight");
}
 
 
/*==========================================================================*/
/*                             Public Functions:                            */
/*==========================================================================*/
void CalendarIncMonth(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (widget->calendar.date.month == DECEMBER+1) {
      widget->calendar.date.month = JANUARY+1;
      widget->calendar.date.year++;
   } else {
      widget->calendar.date.month++;
   }
   calendar_update(widget);
}


void CalendarDecMonth(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (widget->calendar.date.month == JANUARY+1) {
      widget->calendar.date.month = DECEMBER+1;
      widget->calendar.date.year--;
   } else {
      widget->calendar.date.month--;
   }
   calendar_update(widget);
}


void CalendarIncYear(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   widget->calendar.date.year++;
   calendar_update(widget);
}


void CalendarDecYear(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   widget->calendar.date.year--;
   calendar_update(widget);
}


void CalendarIncDay(Widget widget_arg, Boolean show) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (!widget->calendar.highlight_date.day) {
      return;
   }
   BEGIN("CalendarIncDay");

   if (SYNC(widget)) {
      toggle_highlight(widget, OFF);
   }

   if (widget->calendar.highlight_date.day == DAYS_IN_HIGHLIGHT_MONTH(widget)) {
      /* We've rolled over a month. */ 
      widget->calendar.highlight_date.day = 1;

      if (widget->calendar.highlight_date.month == DECEMBER+1) {
         widget->calendar.highlight_date.month = JANUARY+1;
         widget->calendar.highlight_date.year++;

         /* We've rolled over a year, get new days in feb. */
         widget->calendar.highlight_days_in_february =
            28 + A_LEAP_YEAR(widget->calendar.highlight_date.year);

      } else {
         widget->calendar.highlight_date.month++;
      }

      if (show) {
         CalendarShowMonth((Widget)widget, widget->calendar.highlight_date);
      }

   } else {
      widget->calendar.highlight_date.day++;
   }

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   }

   END("CalendarIncDay");
}


void CalendarDecDay(Widget widget_arg, Boolean show) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (!widget->calendar.highlight_date.day) {
      return;
   }
   BEGIN("CalendarDecDay");

   if (SYNC(widget)) {
      toggle_highlight(widget, OFF);
   }

   if (widget->calendar.highlight_date.day == 1) {
      /* We've rolled over a month. */

      if (widget->calendar.highlight_date.month == JANUARY+1) {
         widget->calendar.highlight_date.month = DECEMBER+1;
         widget->calendar.highlight_date.year--;
         widget->calendar.highlight_date.day = 31;

         /* We've rolled over a year, get new days in feb. */
         widget->calendar.highlight_days_in_february =
            28 + A_LEAP_YEAR(widget->calendar.highlight_date.year);

      } else {
         widget->calendar.highlight_date.month--;
         widget->calendar.highlight_date.day = DAYS_IN_HIGHLIGHT_MONTH(widget);
      }

      if (show) {
         CalendarShowMonth((Widget)widget, widget->calendar.highlight_date);
      }

   } else {
      widget->calendar.highlight_date.day--;
   }

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   }

   END("CalendarDecDay");
}


void CalendarIncWeek(Widget widget_arg, Boolean show) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   int days_in_highlight_month;

   if (!widget->calendar.highlight_date.day) {
      return;
   }
   BEGIN("CalendarIncWeek");

   if (SYNC(widget)) {
      toggle_highlight(widget, OFF);
   }

   days_in_highlight_month = DAYS_IN_HIGHLIGHT_MONTH(widget);

   if (widget->calendar.highlight_date.day+7 > days_in_highlight_month) {
      /* We've rolled over a month. */
      widget->calendar.highlight_date.day += 7;
      widget->calendar.highlight_date.day -= days_in_highlight_month;

      if (widget->calendar.highlight_date.month == DECEMBER+1) {
         widget->calendar.highlight_date.month = JANUARY+1;
         widget->calendar.highlight_date.year++;

         /* We've rolled over a year, get new days in feb. */
         widget->calendar.highlight_days_in_february =
            28 + A_LEAP_YEAR(widget->calendar.highlight_date.year);

      } else {
         widget->calendar.highlight_date.month++;
      }

      if (show) {
         CalendarShowMonth((Widget)widget, widget->calendar.highlight_date);
      }

   } else {
      widget->calendar.highlight_date.day += 7;
   }

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   }

   END("CalendarIncWeek");
}


void CalendarDecWeek(Widget widget_arg, Boolean show) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (!widget->calendar.highlight_date.day) {
      return;
   }
   BEGIN("CalendarDecDay");

   if (SYNC(widget)) {
      toggle_highlight(widget, OFF);
   }

   if (widget->calendar.highlight_date.day <= 7) {
      /* We've rolled over a month. */

      if (widget->calendar.highlight_date.month == JANUARY+1) {
         widget->calendar.highlight_date.month = DECEMBER+1;
         widget->calendar.highlight_date.year--;
         widget->calendar.highlight_date.day += 31;
         widget->calendar.highlight_date.day -= 7;

         /* We've rolled over a year, get new days in feb. */
         widget->calendar.highlight_days_in_february =
            28 + A_LEAP_YEAR(widget->calendar.highlight_date.year);

      } else {
         widget->calendar.highlight_date.month--;
         widget->calendar.highlight_date.day -= 7;
         widget->calendar.highlight_date.day += DAYS_IN_HIGHLIGHT_MONTH(widget);
      }

      if (show) {
         CalendarShowMonth((Widget)widget, widget->calendar.highlight_date);
      }

   } else {
      widget->calendar.highlight_date.day -= 7;
   }

   if (SYNC(widget)) {
      toggle_highlight(widget, ON);
   }

   END("CalendarDecDay");
}


void CalendarSetDate(Widget widget_arg, Date date) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   if (!is_valid_date(&date)) {
      return;
   }
   BEGIN("CalendarSetDate");

   if (SYNC(widget)) {
      /* Reset any already highlighted date. */
      toggle_highlight(widget, OFF);
   }

   if (widget->calendar.date.month == date.month &&
       widget->calendar.date.year  == date.year) {
      /* The date specified is on the calendar currently being shown. */

      /* Highlight the specified date. */
      widget->calendar.highlight_date = date;
      toggle_highlight(widget, ON);
   } else {
      /* The date specified is not on the current calendar. */

      if (date.year != widget->calendar.highlight_date.year) {
         widget->calendar.highlight_days_in_february =
            28 + A_LEAP_YEAR(date.year);
      }
      widget->calendar.highlight_date = date;
   }

   END("CalendarSetDate");
}


void CalendarShowMonth(Widget widget_arg, Date date) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   date.day = 1;
   if (!is_valid_date(&date)) {
      return;
   }
   BEGIN("CalendarShowMonth");

   if (widget->calendar.date.month == date.month
       && widget->calendar.date.year == date.year) {
      /* The corresponding calendar is currently being displayed. */
   } else {
      /* The corresponding calendar is not currently being displayed. */
      widget->calendar.date.month = date.month;
      widget->calendar.date.year = date.year;
      calendar_update(widget);
   }

   END("CalendarShowMonth");
}


void CalendarGetDate(Widget widget_arg, Date* date_ptr) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   *date_ptr = widget->calendar.highlight_date;
}


Date DateConverter(Widget widget_arg, char* string) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   /* Call the real converter.  This just allows us to hide details. */
   return (convert_string_to_date(widget->calendar.month_names, string));
}


/*
 * Returns a Date structure corresponding to the current date.  Most
 * applications are going to need it so I'll just provide it.
 */
Date GetTodaysDate() {
   Date date;
   time_t current_time = time(NULL);
   struct tm* current_tm = localtime(&current_time);
   date.month = current_tm->tm_mon + 1;
   date.day = current_tm->tm_mday;
   date.year = current_tm->tm_year + 1900;
   return date;
}


/* 
 * Return a string like "Thursday October 15, 1990".
 */
char* CalendarPrettyDate(Widget widget_arg) {
   CalendarWidget widget = (CalendarWidget)widget_arg;
   static char buffer[MAX_PRETTY_DATE_STRING_LENGTH];
   Date highlight_date;
   Date showing_date;
   int weekday;
   int month;

   highlight_date = widget->calendar.highlight_date;
   showing_date = widget->calendar.date;
   
   if (showing_date.year == highlight_date.year) {
      month = highlight_date.month - 1;
      weekday = widget->calendar.month_starting_weekdays[month];
      weekday = (weekday + highlight_date.day - 1) % 7;
   } else {
      /* No info available, so resort to brute force. */
      weekday = compute_weekday(highlight_date);
      weekday = ROTATE(widget, weekday);
   }

   sprintf(
      buffer,
      "%s %s %d, %d",
      widget->calendar.weekday_names[weekday],
      widget->calendar.month_names[highlight_date.month-1],
      highlight_date.day,
      highlight_date.year
   );

   return buffer;
}


static int compute_weekday(Date date) {
   int leap_days;
   int total_days;
   int month;
   int days_in_february;
   
   leap_days = 1;                 /* Year 0 was a leap year. */
   leap_days += date.year/4;      /* Add all years divisible by 4. */
   leap_days -= date.year/100;    /* Subtract all century years. */
   leap_days += date.year/400;    /* Add back century years divisible by 400. */
   
   if (A_LEAP_YEAR(date.year)) {
      leap_days--;
      days_in_february = 29;
   } else {
      days_in_february = 28;
   }

   total_days = (date.year*365) + leap_days;

   for (month=0; month < date.month - 1; month ++) {
      if (month == FEBRUARY) {
         total_days += days_in_february;
      } else {
         total_days += days_in_month[month];
      }
   }

   total_days += date.day - 1;

   return DAYStoWEEKDAY(total_days);
}
