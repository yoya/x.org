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

#include <stdio.h>
#include <ctype.h>
#include <pwd.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/AsciiText.h>
#include "Cal.h"
#include "main.h"
#include "diary.h"
#include "template.h"

#define DOWNCASE(c) (isupper((c)) ? tolower((c)) : (c))
typedef enum { one_month, full_year } Mode;
typedef enum { hide, show } Action;

/*==========================================================================*/
/*                            Forward References:                           */
/*==========================================================================*/

void flip_calendar(void);
void set_the_date(Date);
void init_bitmaps(void);
void init_dialogs(void);
void init_button_box(Widget, int);
void init_menu(void);
void init_translations(void);
void init_one_month_calendar(void);
void init_full_year_calendar(void);
void process_options(void);
Widget make_inc_dec_button(
   char*, Widget, char*, XtCallbackProc, XtCallbackProc
);
void initialize_diary_widgets(void);
void show_new_diary_entry(void);
void save_old_diary_entry(void);
char* get_default_diary_file_name(void);
char* get_default_template_file_name(void);
void flash(char*);
void flash_back(void);
void set_initial_time_interval(void);
void set_toggle_calendar_menu(Action, Boolean);
void set_toggle_diary_menu(Action, Boolean);
void set_display_mode_menu(void);
void update_diary_label(void);

/*==========================================================================*/
/*                             Callbacks/Actions:                           */
/*==========================================================================*/

void enter_date_okay(Widget, XtPointer, XtPointer);
void enter_date_cancel(Widget, XtPointer, XtPointer);
void diary_modified_save(Widget, XtPointer, XtPointer);
void diary_modified_quit(Widget, XtPointer, XtPointer);
void diary_modified_cancel(Widget, XtPointer, XtPointer);
void quit(Widget, XtPointer, XtPointer);
void really_quit(Widget, XtPointer, XtPointer);
void save(Widget, XtPointer, XtPointer);
void day_inc(Widget, XtPointer, XtPointer);
void day_dec(Widget, XtPointer, XtPointer);
void week_inc(Widget, XtPointer, XtPointer);
void week_dec(Widget, XtPointer, XtPointer);
void month_inc(Widget, XtPointer, XtPointer);
void month_dec(Widget, XtPointer, XtPointer);
void year_inc(Widget, XtPointer, XtPointer);
void year_dec(Widget, XtPointer, XtPointer);
void entry_inc(Widget, XtPointer, XtPointer);
void entry_dec(Widget, XtPointer, XtPointer);
void goto_date(Widget, XtPointer, XtPointer);
void goto_today(Widget, XtPointer, XtPointer);
void toggle_diary(Widget, XtPointer, XtPointer);
void toggle_calendar(Widget, XtPointer, XtPointer);
void calendar_callback(Widget, XtPointer, XtPointer);
void set_full_year(Widget, XtPointer, XtPointer);
void set_one_month(Widget, XtPointer, XtPointer);
void diary_callback(Widget, XtPointer, XtPointer);

/*==========================================================================*/
/*                                 Globals:                                 */
/*==========================================================================*/

Widget top_level_widget;
Widget one_month_popup_shell_widget;
Widget full_year_popup_shell_widget;
Widget button_box_widget;
Widget option_button_widget;
Widget one_month_form_widget;
Widget one_month_calendar_widget;
Widget full_year_form_widget;
Widget full_year_calendar_widgets[12];
Widget enter_date_popup_shell_widget;
Widget enter_date_dialog_widget;
Widget enter_date_okay_button_widget;
Widget enter_date_cancel_button_widget;
Widget diary_modified_popup_shell_widget;
Widget diary_modified_dialog_widget;
Widget diary_modified_save_button_widget;
Widget diary_modified_quit_button_widget;
Widget diary_modified_cancel_button_widget;
Widget text_widget;
Widget menu_popup_shell_widget;
Widget menu_goto_today_widget;
Widget menu_goto_date_widget;
Widget menu_toggle_diary_widget;
Widget menu_toggle_calendar_widget;
Widget menu_full_year_widget;
Widget menu_one_month_widget;
Widget menu_save_widget;
Widget menu_quit_widget;
Widget diary_popup_shell_widget;
Widget diary_top_paned_widget;
Widget diary_label_widget;
Widget diary_form_widget;
Widget diary_text_widget;
Widget diary_text_source_widget;
Pixmap calendar_icon;
Pixmap diary_icon;
Pixmap right_arrow;
Pixmap left_arrow;
Pixmap check;
XtTranslations dialog_translations;
XtTranslations calendar_translations;
XtTranslations diary_translations;
XtAppContext app_context;
Date current_date;

/* It's safe to use the callback functions for actions, too. */
XtActionsRec xdiary_actions[] = {
   { "enter-date-okay",       (XtActionProc)enter_date_okay },
   { "quit",                  (XtActionProc)quit },
   { "save",                  (XtActionProc)save },
   { "day-inc",               (XtActionProc)day_inc },
   { "day-dec",               (XtActionProc)day_dec },
   { "week-inc",              (XtActionProc)week_inc },
   { "week-dec",              (XtActionProc)week_dec },
   { "month-inc",             (XtActionProc)month_inc },
   { "month-dec",             (XtActionProc)month_dec },
   { "year-inc",              (XtActionProc)year_inc },
   { "year-dec",              (XtActionProc)year_dec },
   { "entry-inc",             (XtActionProc)entry_inc },
   { "entry-dec",             (XtActionProc)entry_dec },
   { "goto-date",             (XtActionProc)goto_date },
   { "goto-today",            (XtActionProc)goto_today },
   { "toggle-calendar",       (XtActionProc)toggle_calendar },
   { "toggle-diary",          (XtActionProc)toggle_diary }
};

XrmOptionDescRec command_line_table[] = {
   { "-iconic",            "*iconic",               XrmoptionNoArg, "True" },
   { "-lineWidth",         "*Calendar*lineWidth",   XrmoptionSepArg, NULL  },
   { "-linewidth",         "*Calendar*lineWidth",   XrmoptionSepArg, NULL  },
   { "-oneMonthGeometry",  "*oneMonth.geometry",    XrmoptionSepArg, NULL  },
   { "-onemonthgeometry",  "*oneMonth.geometry",    XrmoptionSepArg, NULL  },
   { "-fullYearGeometry",  "*fullYear.geometry",    XrmoptionSepArg, NULL  },
   { "-fullyeargeometry",  "*fullYear.geometry",    XrmoptionSepArg, NULL  },
   { "-diaryGeometry",     "*diary.geometry",       XrmoptionSepArg, NULL  },
   { "-diarygeometry",     "*diary.geometry",       XrmoptionSepArg, NULL  },
   { "-digitFont",         "*oneMonth*digitFont",   XrmoptionSepArg, NULL  },
   { "-digitfont",         "*oneMonth*digitFont",   XrmoptionSepArg, NULL  },
   { "-weekdayFont",       "*oneMonth*weekdayFont", XrmoptionSepArg, NULL  },
   { "-weekdayfont",       "*oneMonth*weekdayFont", XrmoptionSepArg, NULL  },
   { "-titleFont",         "*oneMonth*titleFont",   XrmoptionSepArg, NULL  },
   { "-titlefont",         "*oneMonth*titleFont",   XrmoptionSepArg, NULL  },
   { "-digitGravity",      "*digitGravity",         XrmoptionSepArg, NULL  },
   { "-digitgravity",      "*digitGravity",         XrmoptionSepArg, NULL  },
   { "-startingWeekday",   "*startingWeekday",      XrmoptionSepArg, NULL  },
   { "-startingweekday",   "*startingWeekday",      XrmoptionSepArg, NULL  },
   { "-language",          "*language",             XrmoptionSepArg, NULL  },
   { "-diaryFile",         "*diaryFile",            XrmoptionSepArg, NULL  },
   { "-diaryfile",         "*diaryFile",            XrmoptionSepArg, NULL  },
   { "-templateFile",      "*templateFile",         XrmoptionSepArg, NULL  },
   { "-templatefile",      "*templateFile",         XrmoptionSepArg, NULL  },
   { "-noCalendar",        "*noCalendar",           XrmoptionNoArg, "True" },
   { "-nocalendar",        "*noCalendar",           XrmoptionNoArg, "True" },
   { "-noDiary",           "*noDiary",              XrmoptionNoArg, "True" },
   { "-nodiary",           "*noDiary",              XrmoptionNoArg, "True" },
   { "-long",              "*long",                 XrmoptionNoArg, "True" },
   { "-abbreviate",        "*abbreviate",           XrmoptionSepArg, NULL  },
   { "-fullYear",          "*fullYear",             XrmoptionNoArg, "True" },
   { "-fullyear",          "*fullYear",             XrmoptionNoArg, "True" },
   { "-oneMonth",          "*oneMonth",             XrmoptionNoArg, "True" },
   { "-onemonth",          "*oneMonth",             XrmoptionNoArg, "True" }
};

char* fallback_resources[] = {
   "*font:                          fixed",
   "*diary.height:                  196",
   "*diary*text*autoFill:           on",
   "*diary*text*wrap:               word",
   "*diary*text*scrollHorizontal:   never",
   "*diary*text*scrollVertical:     always",
   "*HorizontalMargins:             16",
   "*MenuButton.leftBitmap:         menu10",
   "*oneMonth*translations:"
   "   #override\\n"
   "   <Key>Left:                   day-dec()\\n"
   "   <Key>Right:                  day-inc()\\n"
   "   <Key>Up:                     week-dec()\\n"
   "   <Key>Down:                   week-inc()\\n"
   "   <Key>b:                      day-dec()\\n"
   "   <Key>c:                      toggle-calendar()\\n"
   "   <Key>f:                      day-inc()\\n"
   "   <Key>g:                      goto-date()\\n"
   "   <Key>n:                      week-inc()\\n"
   "   <Key>p:                      week-dec()\\n"
   "   <Key>q:                      quit()\\n"
   "   <Key>s:                      save()\\n"
   "   <Key>t:                      goto-today()\\n"
   "   <Key>x:                      toggle-diary()",
   "*fullYear*translations:"
   "   #override\\n"
   "   <Key>Left:                   day-dec()\\n"
   "   <Key>Right:                  day-inc()\\n"
   "   <Key>Up:                     week-dec()\\n"
   "   <Key>Down:                   week-inc()\\n"
   "   <Key>b:                      day-dec()\\n"
   "   <Key>c:                      toggle-calendar()\\n"
   "   <Key>f:                      day-inc()\\n"
   "   <Key>g:                      goto-date()\\n"
   "   <Key>n:                      week-inc()\\n"
   "   <Key>p:                      week-dec()\\n"
   "   <Key>q:                      quit()\\n"
   "   <Key>s:                      save()\\n"
   "   <Key>t:                      goto-today()\\n"
   "   <Key>x:                      toggle-diary()",
   "*diary*translations:"
   "   #override\\n"
   "   Shift<Key>Left:              day-dec()\\n"
   "   Shift<Key>Right:             day-inc()\\n"
   "   Shift<Key>Up:                week-dec()\\n"
   "   Shift<Key>Down:              week-inc()\\n"
   "   Ctrl<Key>c:                  toggle-calendar()\\n"
   "   Ctrl<Key>g:                  goto-date()\\n"
   "   Ctrl<Key>q:                  quit()\\n"
   "   Ctrl<Key>s:                  save()\\n"
   "   Ctrl<Key>t:                  goto-today()\\n"
   "   Ctrl<Key>x:                  toggle-diary()",
   "*enterDate*dialog*translations:"
   "   #override\\n"
   "   <Key>Return:                 enter-date-okay()",
   NULL
};

app_data_type application_data;

char help[] = "usage: xdiary [options] [date-string]\n"
"     -lineWidth pixels             Width of lines between cells in calendar\n"
"     -fullYearGeometry WxH+X+Y     Size and position of full year calendar\n"
"     -oneMonthGeometry WxH+X+Y     Size and position of one month calendar\n"
"     -diaryGeometry WxH+X+Y        Size and position of diary window\n"
"     -digitFont font               Font for digits in calendar cells\n"
"     -weekdayFont font             Font for calendar weekday column headings\n"
"     -titleFont font               Font for calendar month name and year\n"
"     -digitGravity gravity         Gravity of digits within calendar cells\n"
"     -startingWeekday day          Day in leftmost column of calendar\n"
"     -language language            Language to use in calendars and menus\n"
"     -diaryFile file               Diary file; default is ~/.xdiary\n"
"     -templateFile file            Template file; default is ~/.xdiary-template\n"
"     -noCalendar                   Do not display calendar at startup\n"
"     -noDiary                      Do not display diary at startup\n"
"     -long                         Use long weekday names in calendar\n"
"     -abbreviate count             Abbreviate weekday names to count letters\n"
"     -fullYear                     Display full year calendar at startup\n"
"     -oneMonth                     Display one month calendar at startup\n";

Boolean full_year_initialized = False;
Boolean one_month_initialized = False;
Boolean diary_initialized = False;
Boolean diary_modified = False;
Boolean currently_flashing = False;

/*==========================================================================*/
/*                            Argument Processing:                          */
/*==========================================================================*/

Boolean diary_showing = True;
Boolean calendar_showing = True;
Boolean lang_specified = False;
Boolean date_opt = False;
Boolean long_opt = False;
Boolean arg_error = False;
Boolean using_template = False;
Boolean help_requested = False;
Mode display_mode = one_month;
XtIntervalId timeout_id;
int abbrev_opt = 3;
int lang_opt = 0;
int interval = 0;
char* date_string = NULL;
char* diary_file_name = NULL;
char* template_file_name = NULL;


/* Ignores case.  Allows partial abbreviations or garbage at end. */
int check_option(char* string_ptr, char*option_ptr) {
   while (*string_ptr && *option_ptr) {
      if (DOWNCASE(*string_ptr) == *option_ptr) {
         /* Letters match */
         string_ptr++;
         option_ptr++;
      } else {
         /* Doesn't match. */
         return False;
      }
   }
   return True;
}


int parse_args(int argc, char* argv[]) {
   int count;
   for (count=1; count < argc; count++) {

      if (check_option(argv[count], "-help")) {
         help_requested = True;
         continue;
      } else if (check_option(argv[count], "-")) {
         fprintf(stderr, "Unknown option \"%s\"\n", argv[count]);
         arg_error = True;
         continue;
      }

      if (!date_opt++) {
         date_string = argv[count];
         continue;
      }

      /* No valid options match. */
      return False;
   }

   /* All options present and accounted for. */
   return True;
}


void process_resources() {
   if (application_data.language) {
      int i;
      lang_specified = True;
      lang_opt = -1;

      for (i=0; languages[i]; i++) {
         if (check_option(application_data.language, languages[i])) {
            lang_opt = i;
            break;
         }
      }

      if (lang_opt == -1) {
         lang_specified = False;
         fprintf(
            stderr,
            "xdiary: unknown language \"%s\"\n",
            application_data.language
         );
         lang_opt = 0;
      }
   }

   if (application_data.diaryFile) {
      diary_file_name = application_data.diaryFile;
   }

   if (application_data.templateFile) {
      template_file_name = application_data.templateFile;
   }

   if (application_data.noCalendar) {
      calendar_showing = False;
   }

   if (application_data.noDiary) {
      if (calendar_showing == False) {
         fprintf(
            stderr, 
            "xdiary: noCalendar and noDiary can't both be specified.\n"
         );
      } else {
         diary_showing = False;
      }
   }

   if (application_data.longOption) {
      long_opt = True;
      lang_specified = True;
   }

   if (application_data.abbreviate) {
      abbrev_opt = application_data.abbreviate;
      if (abbrev_opt < 0) {
         abbrev_opt = 0;
      }
      lang_specified = True;
   }

   if (application_data.fullYear) {
      display_mode = full_year;
   }

   if (application_data.oneMonth) {
      display_mode = one_month;
   }

}


void process_options() {
   int month;
   int weekday;

   /* Abbreviate the weekday names as specified. */
   if (lang_specified && !long_opt) {
      for (weekday=0; weekday < 7; weekday++) {
         if (strlen(weekday_names[lang_opt][weekday]) > abbrev_opt) {
            weekday_names[lang_opt][weekday][abbrev_opt] = NULL;
         }
      }
   }

   if (display_mode == full_year) {
      init_full_year_calendar();
   } else {
      init_one_month_calendar();
   }

   set_display_mode_menu();

   /* Realize a calendar. */
   if (calendar_showing && display_mode == one_month) {
      XtPopup(one_month_popup_shell_widget, XtGrabNone);
      if (!diary_showing) {
         set_toggle_calendar_menu(hide, False);
         set_toggle_diary_menu(show, True);
      } else {
         set_toggle_calendar_menu(hide, True);
         set_toggle_diary_menu(hide, True);
      }
   } else if (calendar_showing && display_mode == full_year) {
      XtPopup(full_year_popup_shell_widget, XtGrabNone);
      if (!diary_showing) {
         set_toggle_calendar_menu(hide, False);
         set_toggle_diary_menu(show, True);
      } else {
         set_toggle_calendar_menu(hide, True);
         set_toggle_diary_menu(hide, True);
      }
   } else {
      /* Realize it, but don't pop it up. */
      XtRealizeWidget(one_month_popup_shell_widget);
      set_toggle_calendar_menu(show, True);
      set_toggle_diary_menu(hide, False);
   }

   if (date_opt) {
      /* A date string was specified. */
      if (display_mode == full_year) {
         current_date = DateConverter(full_year_calendar_widgets[0], date_string);
       } else {
         current_date = DateConverter(one_month_calendar_widget, date_string);
      }
   }

   if (!date_opt || current_date.year == 0) {
      /* No date was specified or the specified date was bogus, */
      /* so calendar widget chose its own date.  Get that date. */
      if (display_mode == full_year) {
         CalendarGetDate(full_year_calendar_widgets[0], &current_date);
      } else {
         CalendarGetDate(one_month_calendar_widget, &current_date);
      }
   }

   set_the_date(current_date);

   /* Make calendar flip with the date. */
   set_initial_time_interval();
   timeout_id = XtAppAddTimeOut(
      app_context, interval, (XtTimerCallbackProc)flip_calendar, NULL
   );
   interval = TWENTY_FOUR_HOURS;

   if (!diary_file_name) {
      diary_file_name = get_default_diary_file_name();
   }

   if (template_file_name) {
      using_template = init_template(template_file_name);
      if (!using_template) {
         fprintf(stderr, "xdiary: unable to open template file\n");
      }
   } else {
      /* No template was specified. */
      template_file_name = get_default_template_file_name();
      using_template = init_template(template_file_name);
   }

   if (diary_showing) {
      initialize_diary_widgets();
      XtPopup(diary_popup_shell_widget, XtGrabNone);
      show_new_diary_entry();
   }
}


void set_display_mode_menu() {
   if (display_mode == full_year) {
      XtVaSetValues(menu_full_year_widget, XtNleftBitmap, check, NULL);
      XtVaSetValues(menu_one_month_widget, XtNleftBitmap, None, NULL);
   } else {
      XtVaSetValues(menu_full_year_widget, XtNleftBitmap, None, NULL);
      XtVaSetValues(menu_one_month_widget, XtNleftBitmap, check, NULL);
   }
}


void set_toggle_calendar_menu(Action action, Boolean active) {
   if (action == hide) {
      XtVaSetValues(
         menu_toggle_calendar_widget,
         XtNlabel, hide_calendar_label[lang_opt],
         NULL
      );
   } else {
      XtVaSetValues(
         menu_toggle_calendar_widget,
         XtNlabel, show_calendar_label[lang_opt],
         NULL
      );
   }
   XtVaSetValues(
      menu_toggle_calendar_widget,
      XtNsensitive, active,
      NULL
   );
}


void set_toggle_diary_menu(Action action, Boolean active) {
   if (action == hide) {
      XtVaSetValues(
         menu_toggle_diary_widget,
         XtNlabel, hide_diary_label[lang_opt],
         NULL
      );
   } else {
      XtVaSetValues(
         menu_toggle_diary_widget,
         XtNlabel, show_diary_label[lang_opt],
         NULL
      );
   }
   XtVaSetValues(
      menu_toggle_diary_widget,
      XtNsensitive, active,
      NULL
   );
}


/*==========================================================================*/
/*                                  Main:                                   */
/*==========================================================================*/

main(int argc, char* argv[]) {
   /* Never Realized */
   top_level_widget = XtAppInitialize(
      &app_context,                  /* application context */
      "XDiary",                      /* application class */
      command_line_table,            /* resource manager options */
      XtNumber(command_line_table),  /* number of resource manager options */
      &argc,                         /* number of command line args */
      argv,                          /* array of command line args */
      fallback_resources,            /* fallback resources */
      NULL,                          /* args */
      0                              /* number of args */
   );

   XtGetApplicationResources(
      top_level_widget,
      &application_data,
      application_resources,
      XtNumber(application_resources),
      NULL,
      0
   );

   init_translations();
   process_resources();

   if (!parse_args(argc, argv) || arg_error || help_requested) {
      if (help_requested) {
         fprintf(stdout, help);
         XtDestroyWidget(top_level_widget);
         exit(0);
      } else {
         fprintf(stderr, help);
         XtDestroyWidget(top_level_widget);
         exit(1);
      }
   }
   
   init_bitmaps();
   init_dialogs();
   init_menu();
   process_options();
   XtAppMainLoop(app_context);
   return(1);
}


void init_translations() {
   XtAppAddActions(app_context, xdiary_actions, XtNumber(xdiary_actions));
}


/*==========================================================================*/
/*                                 Bitmaps:                                 */
/*==========================================================================*/

void init_bitmaps() {
   calendar_icon = XCreateBitmapFromData(
      XtDisplay(top_level_widget),
      RootWindowOfScreen(XtScreen(top_level_widget)),
      calendar_bits,
      calendar_width,
      calendar_height
   );

   diary_icon = XCreateBitmapFromData(
      XtDisplay(top_level_widget),
      RootWindowOfScreen(XtScreen(top_level_widget)),
      diary_bits,
      diary_width,
      diary_height
   );

   left_arrow = XCreateBitmapFromData(
      XtDisplay(top_level_widget),
      RootWindowOfScreen(XtScreen(top_level_widget)),
      left_arrow_bits,
      left_arrow_width,
      left_arrow_height
   );

   right_arrow = XCreateBitmapFromData(
      XtDisplay(top_level_widget),
      RootWindowOfScreen(XtScreen(top_level_widget)),
      right_arrow_bits,
      right_arrow_width,
      right_arrow_height
   );

   check = XCreateBitmapFromData(
      XtDisplay(top_level_widget),
      RootWindowOfScreen(XtScreen(top_level_widget)),
      check_bits,
      check_width,
      check_height
   );
}


/*==========================================================================*/
/*                               Buttonbox:                                 */
/*==========================================================================*/

void init_button_box(Widget parent_widget, int mask) {
   Widget widget_list[MAX_WIDGETS];
   int num_to_manage = 0;

   button_box_widget = XtVaCreateManagedWidget(
      "buttons",           /* widget name */
      boxWidgetClass,      /* widget class */
      parent_widget,       /* parent widget */
      XtNorientation,      XtEhorizontal,
      XtNborderWidth,      0,
      XtNshowGrip,         False,
      XtNskipAdjust,       True,
      NULL                 /* end args */
   );
 
   widget_list[num_to_manage++] = option_button_widget = XtVaCreateWidget(
      "option",               /* widget name */
      menuButtonWidgetClass,  /* widget class */
      button_box_widget,      /* parent widget */
      XtNlabel,               option_label[lang_opt],
      XtNmenuName,            "menu",
      NULL                    /* end args */
   );

   if (mask & DAY) {
      widget_list[num_to_manage++] = make_inc_dec_button(
         "day",                  /* widget name */
         button_box_widget,      /* parent widget */
         day_label[lang_opt],    /* label */
         day_dec,                /* dec callback */
         day_inc                 /* inc callback */
      );
   }
    
   if (mask & MONTH) {
      widget_list[num_to_manage++] = make_inc_dec_button(
         "month",                /* widget name */
         button_box_widget,      /* parent widget */
         month_label[lang_opt],  /* label */
         month_dec,              /* dec callback */
         month_inc               /* inc callback */
      );
   }
   
   if (mask & YEAR) {
      widget_list[num_to_manage++] = make_inc_dec_button(
         "year",                 /* widget name */
         button_box_widget,      /* parent widget */
         year_label[lang_opt],   /* label */
         year_dec,               /* dec callback */
         year_inc                /* inc callback */
      );
   }
   
   if (mask & ENTRY) {
      widget_list[num_to_manage++] = make_inc_dec_button(
         "entry",                 /* widget name */
         button_box_widget,       /* parent widget */
         entry_label[lang_opt],   /* label */
         entry_dec,               /* dec callback */
         entry_inc                /* inc callback */
      );
   }

   /* Manage the children of the top form widget defined above. */
   XtManageChildren(widget_list, num_to_manage);
}


/*==========================================================================*/
/*                                 Dialogs:                                 */
/*==========================================================================*/

void init_dialogs() {

   /* ENTER DATE DIALOG */
   enter_date_popup_shell_widget = XtVaCreatePopupShell(
      "enterDate",                  /* widget name */
      transientShellWidgetClass,    /* widget class */
      top_level_widget,             /* parent widget */
      NULL                          /* end args */
   );

   enter_date_dialog_widget = XtVaCreateManagedWidget(
      "dialog",                        /* widget name */
      dialogWidgetClass,               /* widget class */
      enter_date_popup_shell_widget,   /* parent widget */
      XtNlabel,                        enter_date_label[lang_opt],
      XtNvalue,                        "",
      NULL                             /* end args */
   );

   enter_date_okay_button_widget = XtVaCreateManagedWidget(
      "okay",                    /* widget name */
      commandWidgetClass,        /* widget class */
      enter_date_dialog_widget,  /* parent widget */
      XtNlabel,                  okay_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(
      enter_date_okay_button_widget,
      XtNcallback, enter_date_okay, 0
   );

   enter_date_cancel_button_widget = XtVaCreateManagedWidget(
      "cancel",                  /* widget name */
      commandWidgetClass,        /* widget class */
      enter_date_dialog_widget,  /* parent widget */
      XtNlabel,                  cancel_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(
      enter_date_cancel_button_widget,
      XtNcallback, enter_date_cancel, 0
   );

   /* DIARY MODIFIED DIALOG */
   diary_modified_popup_shell_widget = XtVaCreatePopupShell(
      "diaryModified",              /* widget name */
      transientShellWidgetClass,    /* widget class */
      top_level_widget,             /* parent widget */
      NULL                          /* end args */
   );

   diary_modified_dialog_widget = XtVaCreateManagedWidget(
      "dialog",                           /* widget name */
      dialogWidgetClass,                  /* widget class */
      diary_modified_popup_shell_widget,  /* parent widget */
      XtNlabel,                           diary_modified_label[lang_opt],
      NULL                                /* end args */
   );

   diary_modified_save_button_widget = XtVaCreateManagedWidget(
      "save",                       /* widget name */
      commandWidgetClass,           /* widget class */
      diary_modified_dialog_widget, /* parent widget */
      XtNlabel,                     save_label[lang_opt],
      NULL                          /* end args */
   );
   XtAddCallback(
      diary_modified_save_button_widget,
      XtNcallback, diary_modified_save, 0
   );

   diary_modified_quit_button_widget = XtVaCreateManagedWidget(
      "quit",                       /* widget name */
      commandWidgetClass,           /* widget class */
      diary_modified_dialog_widget, /* parent widget */
      XtNlabel,                     quit_label[lang_opt],
      NULL                          /* end args */
   );
   XtAddCallback(
      diary_modified_quit_button_widget,
      XtNcallback, diary_modified_quit, 0
   );

   diary_modified_cancel_button_widget = XtVaCreateManagedWidget(
      "cancel",                     /* widget name */
      commandWidgetClass,           /* widget class */
      diary_modified_dialog_widget, /* parent widget */
      XtNlabel,                     cancel_label[lang_opt],
      NULL                          /* end args */
   );
   XtAddCallback(
      diary_modified_cancel_button_widget,
      XtNcallback, diary_modified_cancel, 0 
   );
}


/*==========================================================================*/
/*                                  Menu:                                   */
/*==========================================================================*/

void init_menu() {
   Widget widget_list[MAX_WIDGETS];
   int num_to_manage = 0;

   menu_popup_shell_widget = XtVaCreatePopupShell(
      "menu",                 /* widget name */
      simpleMenuWidgetClass,  /* widget class */
      top_level_widget,       /* parent widget */
      NULL                    /* end args */
   );

   /* GO TO TODAY */
   widget_list[num_to_manage++] = menu_goto_today_widget = XtVaCreateWidget(
      "menuGoToToday",           /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  goto_today_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_goto_today_widget, XtNcallback, goto_today, 0);

   /* GO TO DATE ... */
   widget_list[num_to_manage++] = menu_goto_date_widget = XtVaCreateWidget(
      "menuGoToDate",            /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  goto_date_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_goto_date_widget, XtNcallback, goto_date, 0);

   /* LINE */
   widget_list[num_to_manage++] = XtVaCreateWidget(
      "line",                    /* widget name */
      smeLineObjectClass,        /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      NULL                       /* end args */
   );

   /* HIDE/SHOW DIARY */
   widget_list[num_to_manage++] = menu_toggle_diary_widget = XtVaCreateWidget(
      "menuToggleDiary",         /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  hide_diary_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_toggle_diary_widget, XtNcallback, toggle_diary, 0);

   /* HIDE/SHOW CALENDAR */
   widget_list[num_to_manage++] = menu_toggle_calendar_widget = XtVaCreateWidget(
      "menuToggleCalendar",      /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  show_calendar_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_toggle_calendar_widget, XtNcallback, toggle_calendar, 0);

   /* LINE */
   widget_list[num_to_manage++] = XtVaCreateWidget(
      "line",                    /* widget name */
      smeLineObjectClass,        /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      NULL                       /* end args */
   );

   /* ONE MONTH CALENDAR */
   widget_list[num_to_manage++] = menu_one_month_widget = XtVaCreateWidget(
      "menuOneMonth",            /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  one_month_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_one_month_widget, XtNcallback, set_one_month, 0);

   /* FULL YEAR CALENDAR */
   widget_list[num_to_manage++] = menu_full_year_widget = XtVaCreateWidget(
      "menuFullYear",            /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  full_year_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_full_year_widget, XtNcallback, set_full_year, 0);

   /* LINE */
   widget_list[num_to_manage++] = XtVaCreateWidget(
      "line",                    /* widget name */
      smeLineObjectClass,        /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      NULL                       /* end args */
   );

   /* SAVE */
   widget_list[num_to_manage++] = menu_save_widget = XtVaCreateWidget(
      "menuSave",                /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  save_label[lang_opt],
      XtNsensitive,              False,
      NULL                       /* end args */
   );
   XtAddCallback(menu_save_widget, XtNcallback, save, 0);

   /* QUIT */
   widget_list[num_to_manage++] = menu_quit_widget = XtVaCreateWidget(
      "menuQuit",                /* widget name */
      smeBSBObjectClass,         /* widget class */
      menu_popup_shell_widget,   /* parent widget */
      XtNlabel,                  quit_label[lang_opt],
      NULL                       /* end args */
   );
   XtAddCallback(menu_quit_widget, XtNcallback, quit, 0);

   set_display_mode_menu();

   /* Manage the children of the top form widget defined above. */
   XtManageChildren(widget_list, num_to_manage);
}


/*==========================================================================*/
/*                             One Month Calendar:                          */
/*==========================================================================*/

void init_one_month_calendar() {
   Widget one_month_paned_widget;
   int linewidth;

   if (one_month_initialized) {
      return;
   } else {
      one_month_initialized = True;
   }

   one_month_popup_shell_widget = XtVaCreatePopupShell(
      "oneMonth",
      topLevelShellWidgetClass,
      top_level_widget,
      XtNiconPixmap,    calendar_icon,
      XtNiconName,      "calendar",
      NULL
   );

   one_month_paned_widget = XtVaCreateManagedWidget(
      "paned",                      /* widget name */
      panedWidgetClass,             /* widget class */
      one_month_popup_shell_widget, /* parent widget */
      NULL                          /* end args */
   );

   init_button_box(one_month_paned_widget, MONTH | YEAR);

   one_month_form_widget = XtVaCreateManagedWidget(
      "form",                 /* widget name */
      formWidgetClass,        /* widget class */
      one_month_paned_widget, /* parent widget */
      XtNshowGrip,            False,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   one_month_calendar_widget = XtVaCreateManagedWidget(
      "calendar",             /* widget name */
      calendarWidgetClass,    /* widget class */
      one_month_form_widget,  /* parent widget */
      XtNtop,                 XtChainTop,
      XtNbottom,              XtChainBottom,
      XtNright,               XtChainRight,
      XtNleft,                XtChainLeft,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );
   XtAddCallback(one_month_calendar_widget, XtNcallback, calendar_callback, 0);

   /* Get the border and the grid to be the same thickness */
   XtVaGetValues(
      one_month_calendar_widget,
      XtNlineWidth, &linewidth,
      NULL
   );

   XtVaSetValues(
      one_month_calendar_widget,
      XtNborderWidth,  linewidth/2,
      NULL
   );

   if (lang_specified) {
      XtVaSetValues(
         one_month_calendar_widget,
         XtNmonthNames,   (XtPointer) month_names[lang_opt],
         XtNweekdayNames, (XtPointer) weekday_names[lang_opt],
         NULL
      );
   }
}


/*==========================================================================*/
/*                              Full Year Calendar:                         */
/*==========================================================================*/

void init_full_year_calendar() {
   Widget full_year_paned_widget;
   Widget widget_list[MAX_WIDGETS];
   int num_to_manage = 0;
   int month;
   int linewidth;

   if (full_year_initialized) {
      return;
   } else {
      full_year_initialized = True;
   }

   full_year_popup_shell_widget = XtVaCreatePopupShell(
      "fullYear",
      topLevelShellWidgetClass,
      top_level_widget,
      XtNiconPixmap,    calendar_icon,
      XtNiconName,      "calendar",
      NULL
   );

   full_year_paned_widget = XtVaCreateManagedWidget(
      "paned",                      /* widget name */
      panedWidgetClass,             /* widget class */
      full_year_popup_shell_widget, /* parent widget */
      NULL                          /* end args */
   );

   init_button_box(full_year_paned_widget, YEAR);

   full_year_form_widget = XtVaCreateManagedWidget(
      "form",                  /* widget name */
      formWidgetClass,         /* widget class */
      full_year_paned_widget,  /* parent widget */
      XtNshowGrip,             False,
      XtNborderWidth,          1,
      NULL                     /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[0] = XtVaCreateWidget(
      "january",              /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNtop,                 XtChainTop,
      XtNleft,                XtChainLeft,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[1] = XtVaCreateWidget(
      "february",             /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromHoriz,           full_year_calendar_widgets[0],
      XtNtop,                 XtChainTop,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[2] = XtVaCreateWidget(
      "march",                /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromHoriz,           full_year_calendar_widgets[1],
      XtNtop,                 XtChainTop,
      XtNright,               XtChainRight,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[3] = XtVaCreateWidget(
      "april",                /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[0],
      XtNleft,                XtChainLeft,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[4] = XtVaCreateWidget(
      "may",                  /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[0],
      XtNfromHoriz,           full_year_calendar_widgets[3],
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[5] = XtVaCreateWidget(
      "june",                 /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[0],
      XtNfromHoriz,           full_year_calendar_widgets[4],
      XtNright,               XtChainRight,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[6] = XtVaCreateWidget(
      "july",                 /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[3],
      XtNleft,                XtChainLeft,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[7] = XtVaCreateWidget(
      "august",               /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[3],
      XtNfromHoriz,           full_year_calendar_widgets[6],
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[8] = XtVaCreateWidget(
      "september",            /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[3],
      XtNfromHoriz,           full_year_calendar_widgets[7],
      XtNright,               XtChainRight,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[9] = XtVaCreateWidget(
      "october",              /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[6],
      XtNbottom,              XtChainBottom,
      XtNleft,                XtChainLeft,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[10] = XtVaCreateWidget(
      "november",             /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[6],
      XtNfromHoriz,           full_year_calendar_widgets[9],
      XtNbottom,              XtChainBottom,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   widget_list[num_to_manage++] = full_year_calendar_widgets[11] = XtVaCreateWidget(
      "december",             /* widget name */
      calendarWidgetClass,    /* widget class */
      full_year_form_widget,  /* parent widget */
      XtNfromVert,            full_year_calendar_widgets[6],
      XtNfromHoriz,           full_year_calendar_widgets[10],
      XtNbottom,              XtChainBottom,
      XtNright,               XtChainRight,
      XtNborderWidth,         0,
      NULL                    /* end args */
   );

   /* Get the border and the grid to be the same thickness */
   XtVaGetValues(
      full_year_calendar_widgets[0],
      XtNlineWidth, &linewidth,
      NULL
   );

   for (month=0; month < 12; month++) {
      XtAddCallback(
         full_year_calendar_widgets[month],
         XtNcallback,
         calendar_callback,
         0
      );

      XtVaSetValues(
         full_year_calendar_widgets[month],
         XtNborderWidth,  linewidth/2,
         NULL
      );

      if (lang_specified) {
         XtVaSetValues(
            full_year_calendar_widgets[month],
            XtNmonthNames,   (XtPointer) month_names[lang_opt],
            XtNweekdayNames, (XtPointer) weekday_names[lang_opt],
            NULL
         );
      }
   }

   /* Manage all the children of the form widget */
   XtManageChildren(widget_list, num_to_manage);
}


/*==========================================================================*/
/*                            Make Inc/Dec Button:                          */
/*==========================================================================*/

Widget make_inc_dec_button(
   char* name,
   Widget parent_widget,
   char* label,
   XtCallbackProc callback1,
   XtCallbackProc callback2
) {
   Widget form_widget;
   Widget button1;
   Widget button2;
   Widget label_widget;
   Widget widget_list[MAX_WIDGETS];
   int num_to_manage = 0;

   form_widget = XtVaCreateManagedWidget(
      "form",              /* widget name */
      formWidgetClass,     /* widget class */
      parent_widget,       /* parent widget */
      XtNtop,              XtChainTop,
      XtNbottom,           XtChainBottom,
      XtNleft,             XtChainLeft,
      XtNright,            XtChainRight,
      XtNdefaultDistance,  0,
      XtNborderWidth,      0,
      NULL                 /* end args */
   );

   widget_list[num_to_manage++] = label_widget = XtVaCreateWidget(
      name,                /* widget name */
      labelWidgetClass,    /* widget class */
      form_widget,         /* parent widget */
      XtNlabel,            label,
      XtNtop,              XtChainTop,
      XtNbottom,           XtChainTop,
      XtNleft,             XtChainLeft,
      XtNright,            XtChainLeft,
      XtNborderWidth,      0,
      NULL                 /* end args */
   );
   
   widget_list[num_to_manage++] = button1 = XtVaCreateWidget(
      "dec",               /* widget name */
      commandWidgetClass,  /* widget class */
      form_widget,         /* parent widget */
      XtNfromHoriz,        label_widget,
      XtNtop,              XtChainTop,
      XtNbottom,           XtChainTop,
      XtNleft,             XtChainLeft,
      XtNright,            XtChainLeft,
      XtNbitmap,           left_arrow,
      XtNborderWidth,      1,
      NULL                 /* end args */
   );
   XtAddCallback(button1, XtNcallback, callback1, NULL);

   widget_list[num_to_manage++] = button2 = XtVaCreateWidget(
      "inc",               /* widget name */
      commandWidgetClass,  /* widget class */
      form_widget,         /* parent widget */
      XtNfromHoriz,        button1,
      XtNtop,              XtChainTop,
      XtNbottom,           XtChainTop,
      XtNleft,             XtChainLeft,
      XtNright,            XtChainLeft,
      XtNbitmap,           right_arrow,
      XtNborderWidth,      1,
      NULL                 /* end args */
   );
   XtAddCallback(button2, XtNcallback, callback2, NULL);

   /* Manage all the children of the form widget */
   XtManageChildren(widget_list, num_to_manage);

   return (form_widget);
}


/*==========================================================================*/
/*                          Miscellaneous Functions:                        */
/*==========================================================================*/

void set_the_date(Date new_date) {
   int month;
   Date temp_date;

   if (display_mode == full_year) {

      temp_date.year = new_date.year;

      /* Only change the calendar date if a day was specified. */
      if (new_date.day) {
         for (month=0; month < 12; month ++) {
            /* Set date to highlight for each calendar. */
            CalendarSetDate(full_year_calendar_widgets[month], new_date);
            current_date = new_date;
         }
      }

      for (month=0; month < 12; month ++) {
         /* Set month to show for each calendar. */
         temp_date.month = month+1;
         CalendarShowMonth(full_year_calendar_widgets[month], temp_date);
      }

   } else if (display_mode == one_month) {

      /* Only change the calendar date if a day was specified. */
      if (new_date.day) {
         CalendarSetDate(one_month_calendar_widget, new_date);
         current_date = new_date;
      }
      CalendarShowMonth(one_month_calendar_widget, new_date);

   } else {

      /* No calendar is displayed. */
      if (new_date.day) {
         CalendarSetDate(one_month_calendar_widget, new_date);
         current_date = new_date;
      }
   }

}


void flip_calendar() {
   goto_today(NULL, NULL, NULL);
   timeout_id = XtAppAddTimeOut(
      app_context, interval, (XtTimerCallbackProc)flip_calendar, NULL
   );
}


char* get_default_diary_file_name() {
   static char file_name[MAXPATHLEN];
   uid_t uid = getuid();
   struct passwd* pwent = getpwuid(uid);
   strcpy(file_name, pwent->pw_dir);
   strcat(file_name, DIARY_FILE);
   return(file_name);
}


char* get_default_template_file_name() {
   static char file_name[MAXPATHLEN];
   uid_t uid = getuid();
   struct passwd* pwent = getpwuid(uid);
   strcpy(file_name, pwent->pw_dir);
   strcat(file_name, TEMPLATE_FILE);
   return(file_name);
}


/* Determine micro seconds til midnight. */
void set_initial_time_interval() {
   time_t seconds = time(NULL);
   struct tm* tm_ptr = localtime(&seconds);
   int minutes = tm_ptr->tm_hour * 60 + tm_ptr->tm_min;

   /* Wait until 5 minutes after midnight and then flip */
   interval = TWENTY_FOUR_HOURS + FIVE_MINUTES - (1000 * 60 * minutes);
}


/*==========================================================================*/
/*                               Callbacks:                                 */
/*==========================================================================*/

void quit(Widget widget, XtPointer client_data, XtPointer call_data) {
   if (diary_modified) {
      Position x, y;

      XtTranslateCoords(
         widget,
         0, 0,
         &x, &y
      );

      XtVaSetValues(
         diary_modified_popup_shell_widget,
         XtNx, x+6,
         XtNy, y-6,
         NULL
      );
      XtPopup(diary_modified_popup_shell_widget, XtGrabExclusive);

   } else {
      really_quit(NULL, NULL, NULL);
   }
}


void really_quit(Widget widget, XtPointer client_data, XtPointer call_data) {
   XtDestroyWidget(top_level_widget);
   XtRemoveTimeOut(timeout_id);
   exit(0);
}


void diary_modified_cancel(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   XtPopdown(diary_modified_popup_shell_widget);
}


void diary_modified_quit(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   XtPopdown(diary_modified_popup_shell_widget);
   really_quit(NULL, NULL, NULL);
}


void diary_modified_save(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   XtPopdown(diary_modified_popup_shell_widget);
   save(NULL, NULL, NULL);
   quit(NULL, NULL, NULL);
}


void month_inc(Widget widget, XtPointer client_data, XtPointer call_data) {
   /* Only called from one_month calendar mode. */
   CalendarIncMonth(one_month_calendar_widget);
}


void month_dec(Widget widget, XtPointer client_data, XtPointer call_data) {
   /* Only called from one_month calendar mode. */
   CalendarDecMonth(one_month_calendar_widget);
}


void year_inc(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   if (display_mode == full_year) {
      for (month=0; month < 12; month++) {
         CalendarIncYear(full_year_calendar_widgets[month]);
      }
   } else {
      CalendarIncYear(one_month_calendar_widget);
   }
}


void year_dec(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   if (display_mode == full_year) {
      for (month=0; month < 12; month++) {
         CalendarDecYear(full_year_calendar_widgets[month]);
      }
   } else {
      CalendarDecYear(one_month_calendar_widget);
   }
}


void goto_date(Widget widget, XtPointer client_data, XtPointer call_data) {
   Position x, y;

   XtTranslateCoords(
      widget,
      0, 0,
      &x, &y
   );

   XtVaSetValues(
      enter_date_popup_shell_widget,
      XtNx, x+6,
      XtNy, y-6,
      NULL
   );
   XtPopup(enter_date_popup_shell_widget, XtGrabExclusive);
}


void goto_today(Widget widget, XtPointer client_data, XtPointer call_data) {
   Date todays_date;
   todays_date = GetTodaysDate();
   save_old_diary_entry();
   set_the_date(todays_date);
   show_new_diary_entry();
}


void day_inc(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   Date temp_date;

   save_old_diary_entry();

   if (calendar_showing && display_mode == full_year) {
      
      for (month=0; month < 12; month++) {
         CalendarIncDay(full_year_calendar_widgets[month], False);
      }

      CalendarGetDate(full_year_calendar_widgets[0], &current_date);

      temp_date.year = current_date.year;
      for (month=0; month < 12; month++) {
         temp_date.month = month+1;
         CalendarShowMonth(full_year_calendar_widgets[month], temp_date);
      }

   } else if (calendar_showing && display_mode == one_month) {

      CalendarIncDay(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
      CalendarShowMonth(one_month_calendar_widget, current_date);
   
   } else {

      /* No calendar is displayed. */
      CalendarIncDay(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
   }

   show_new_diary_entry();
}


void day_dec(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   Date temp_date;

   save_old_diary_entry();

   if (calendar_showing && display_mode == full_year) {
      
      for (month=0; month < 12; month++) {
         CalendarDecDay(full_year_calendar_widgets[month], False);
      }

      CalendarGetDate(full_year_calendar_widgets[0], &current_date);

      temp_date.year = current_date.year;
      for (month=0; month < 12; month++) {
         temp_date.month = month+1;
         CalendarShowMonth(full_year_calendar_widgets[month], temp_date);
      }

   } else if (calendar_showing && display_mode == one_month) {

      CalendarDecDay(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
      CalendarShowMonth(one_month_calendar_widget, current_date);

   } else {

      /* No calendar is displayed. */
      CalendarDecDay(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
   }

   show_new_diary_entry();
}


void week_dec(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   Date temp_date;

   save_old_diary_entry();

   if (calendar_showing && display_mode == full_year) {
      
      for (month=0; month < 12; month++) {
         CalendarDecWeek(full_year_calendar_widgets[month], False);
      }

      CalendarGetDate(full_year_calendar_widgets[0], &current_date);

      temp_date.year = current_date.year;
      for (month=0; month < 12; month++) {
         temp_date.month = month+1;
         CalendarShowMonth(full_year_calendar_widgets[month], temp_date);
      }

   } else if (calendar_showing && display_mode == one_month) {

      CalendarDecWeek(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
      CalendarShowMonth(one_month_calendar_widget, current_date);
   
   } else {

      /* No calendar is displayed. */
      CalendarDecWeek(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
   }

   show_new_diary_entry();
}
 

void week_inc(Widget widget, XtPointer client_data, XtPointer call_data) {
   int month;
   Date temp_date;

   save_old_diary_entry();

   if (calendar_showing && display_mode == full_year) {
      
      for (month=0; month < 12; month++) {
         CalendarIncWeek(full_year_calendar_widgets[month], False);
      }

      CalendarGetDate(full_year_calendar_widgets[0], &current_date);

      temp_date.year = current_date.year;
      for (month=0; month < 12; month++) {
         temp_date.month = month+1;
         CalendarShowMonth(full_year_calendar_widgets[month], temp_date);
      }

   } else if (calendar_showing && display_mode == one_month) {

      CalendarIncWeek(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
      CalendarShowMonth(one_month_calendar_widget, current_date);
   
   } else {

      /* No calendar is displayed. */
      CalendarIncWeek(one_month_calendar_widget, True);
      CalendarGetDate(one_month_calendar_widget, &current_date);
   }

   show_new_diary_entry();
}
 

void toggle_diary(Widget widget, XtPointer client_data, XtPointer call_data) {
   if (!calendar_showing) {
      return;
   }

   if (diary_showing) {
      /* Hide the diary. */
      XtPopdown(diary_popup_shell_widget);
      diary_showing = False;
      set_toggle_diary_menu(show, True);
      set_toggle_calendar_menu(hide, False);
      save_old_diary_entry();
   } else {
      /* Show the diary. */
      initialize_diary_widgets();
      XtPopup(diary_popup_shell_widget, XtGrabNone);
      diary_showing = True;
      set_toggle_diary_menu(hide, True);
      set_toggle_calendar_menu(hide, True);
      show_new_diary_entry();
   }
}


void toggle_calendar(Widget widget, XtPointer client_data, XtPointer call_data) {
   if (!diary_showing) {
      return;
   }

   if (calendar_showing) {
      /* Hide the calendar. */
      if (display_mode == one_month) {
         XtPopdown(one_month_popup_shell_widget);
      } else {
         XtPopdown(full_year_popup_shell_widget);
      }
      calendar_showing = False;
      set_toggle_calendar_menu(show, True);
      set_toggle_diary_menu(hide, False);
   } else {
      /* Show the calendar. */
      if (display_mode == one_month) {
         init_one_month_calendar();
         XtPopup(one_month_popup_shell_widget, XtGrabNone);
      } else {
         init_full_year_calendar();
         XtPopup(full_year_popup_shell_widget, XtGrabNone);
      }
      calendar_showing = True;
      set_toggle_calendar_menu(hide, True);
      set_toggle_diary_menu(hide, True);
   }
}


void set_one_month(Widget widget, XtPointer client_data, XtPointer call_data) {
   if (!calendar_showing) {
      display_mode = one_month;
      set_display_mode_menu();
      return;
   }

   if (display_mode == full_year) {
      init_one_month_calendar();
      XtPopdown(full_year_popup_shell_widget);
      XtPopup(one_month_popup_shell_widget, XtGrabNone);
      display_mode = one_month;
      set_the_date(current_date);
   }

   set_display_mode_menu();
}


void set_full_year(Widget widget, XtPointer client_data, XtPointer call_data) { 
   if (!calendar_showing) {
      display_mode = full_year;
      set_display_mode_menu();
      return;
   }

   if (display_mode == one_month) {
      init_full_year_calendar();
      XtPopdown(one_month_popup_shell_widget);
      XtPopup(full_year_popup_shell_widget, XtGrabNone);
      display_mode = full_year;
      set_the_date(current_date);
   }

   set_display_mode_menu();
}


void enter_date_okay(Widget widget, XtPointer client_data, XtPointer call_data) {
   char* string;
   Date new_date;

   XtPopdown(enter_date_popup_shell_widget);
   string = XawDialogGetValueString(enter_date_dialog_widget);
   if (!strcmp(string, "")) {
      return;
   }

   if (display_mode == full_year) {
      new_date = DateConverter(full_year_calendar_widgets[0], string);
   } else {
      new_date = DateConverter(one_month_calendar_widget, string);
   }

   if (new_date.year) {
      /* The date was valid. */
      if (diary_showing) {
         save_old_diary_entry();
         set_the_date(new_date);
         show_new_diary_entry();
      } else {
         set_the_date(new_date);
      }
   }
}


void enter_date_cancel(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   XtPopdown(enter_date_popup_shell_widget);
}


/*==========================================================================*/
/*                                  Diary:                                  */
/*==========================================================================*/

void initialize_diary_widgets() {
   Widget text_widget;

   if (diary_initialized) {
      return;
   } else {
      diary_initialized = True;
   }

   init_diary(diary_file_name);

   diary_popup_shell_widget = XtVaCreatePopupShell(
      "diary",
      topLevelShellWidgetClass,
      top_level_widget,
      XtNiconPixmap,    diary_icon,
      XtNiconName,      "diary",
      XtNwaitForWm,     False,
      NULL
   );

   diary_top_paned_widget = XtVaCreateManagedWidget(
      "paned",                   /* widget name */
      panedWidgetClass,          /* widget class */
      diary_popup_shell_widget,  /* parent widget */
      NULL                       /* end args */
   );

   init_button_box(diary_top_paned_widget, DAY | ENTRY);

   diary_label_widget = XtVaCreateManagedWidget(
     "label",                   /* widget name */
      labelWidgetClass,         /* widget class */
      diary_top_paned_widget,   /* parent widget */
      XtNlabel,                 "",
      XtNshowGrip,              False,
      XtNskipAdjust,            True,
      NULL                      /* end args */
   );

   diary_form_widget = XtVaCreateManagedWidget(
      "form",                  /* widget name */
      formWidgetClass,         /* widget class */
      diary_top_paned_widget,  /* parent widget */
      XtNshowGrip,             False,
      NULL                     /* end args */
   );

   diary_text_widget = XtVaCreateManagedWidget(
      "text",                  /* widget name */
      asciiTextWidgetClass,    /* widget class */
      diary_form_widget,       /* parent widget */
      XtNeditType,             XtEtextEdit,
      XtNtop,                  XtChainTop,
      XtNbottom,               XtChainBottom,
      XtNright,                XtChainRight,
      XtNleft,                 XtChainLeft,
      NULL                     /* end args */
   );

   XtVaGetValues(
      diary_text_widget,
      XtNtextSource, &diary_text_source_widget,
      NULL
   );
   XtAddCallback(diary_text_source_widget, XtNcallback, diary_callback, NULL);
}


void show_new_diary_entry() {
   char* entry_text;

   if (!diary_showing) {
      return;
   }

   entry_text = retrieve(current_date);

   if (using_template && !entry_text[0]) {
      /* Initialize the entry to the template. */
      entry_text = template;
   }

   XtVaSetValues(
      diary_text_widget,
      XtNstring, entry_text,
      NULL
   );

   update_diary_label();
}


void save_old_diary_entry() {
   char* old_text;
   char* copy;

   if (!diary_initialized) {
      return;
   }

   if (XawAsciiSourceChanged(diary_text_source_widget)) {

      XtVaGetValues(
         diary_text_widget,
         XtNstring, &old_text,
         NULL
      );

      if (using_template) {
         if (entry_modified(old_text)) {
            copy = XtNewString(old_text);
            install(current_date, copy, True);
         } else {
            /* Only whitespace changes were made to the template */
            install(current_date, "", False);
         }
      } else {
         copy = XtNewString(old_text);
         install(current_date, copy, True);
      }
   }
}


void update_diary_label() {
   static char dirty_buffer[128];
   char* pretty_string;

   if (!diary_showing) {
      return;
   }

   if (display_mode == full_year) {
      pretty_string = CalendarPrettyDate(full_year_calendar_widgets[0]);
   } else {
      pretty_string = CalendarPrettyDate(one_month_calendar_widget);
   }

   if (diary_modified) {
      strcpy(dirty_buffer, "* ");
      strcat(dirty_buffer, pretty_string);
      strcat(dirty_buffer, " *");
      pretty_string = dirty_buffer;
   }

   XtVaSetValues(
      diary_label_widget,
      XtNlabel,             pretty_string,
      NULL                  /* end args */
   );
}


/*==========================================================================*/
/*                             Diary Callbacks:                             */
/*==========================================================================*/

void entry_inc(Widget widget, XtPointer client_data, XtPointer call_data) {
   Date next_date;
   if (currently_flashing) {
      return;
   }
   next_date = next_entry(current_date);
   if (next_date.year) {
      save_old_diary_entry();
      set_the_date(next_date);
      show_new_diary_entry();
   } else if (diary_showing) {
      flash("No following diary entry exists");
   }
}


void entry_dec(Widget widget, XtPointer client_data, XtPointer call_data) {
   Date prev_date;
   if (currently_flashing) {
      return;
   }
   prev_date = prev_entry(current_date);
   if (prev_date.year) {
      save_old_diary_entry();
      set_the_date(prev_date);
      show_new_diary_entry();
   } else if (diary_showing) {
      flash("No previous diary entry exists");
   }
}


void save(Widget widget, XtPointer client_data, XtPointer call_data) {
   save_old_diary_entry();
   if (diary_modified) {
      save_diary(diary_file_name);
      diary_modified = False;
      update_diary_label();
      XtVaSetValues(menu_save_widget, XtNsensitive, False, NULL);
   }
}


void calendar_callback(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   /* Called when calendar changes. */
   Date* date_ptr = (Date*)call_data;
   if (diary_showing) {
      save_old_diary_entry();
      set_the_date(*date_ptr);
      show_new_diary_entry();
   } else {
      set_the_date(*date_ptr);
   }
}


void diary_callback(
   Widget widget, XtPointer client_data, XtPointer call_data
) {
   /* Called when diary changes. */
   if (!diary_modified) {
      diary_modified = True;
      update_diary_label();
      XtVaSetValues(menu_save_widget, XtNsensitive, True, NULL);
   } else {
      diary_modified = True;
   }
}


void flash(char* string) {
   currently_flashing = True;
   XtVaSetValues(
      diary_label_widget,
      XtNlabel,             string,
      NULL                  /* end args */
   );
   XtAppAddTimeOut(
      app_context, ONE_SECOND, (XtTimerCallbackProc)flash_back, NULL
   );
}


void flash_back() {
   update_diary_label();
   currently_flashing = False;
}
