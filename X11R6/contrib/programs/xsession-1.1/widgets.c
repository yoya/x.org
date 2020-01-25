/* 
 * Copyright (C) 1993, 1994, Alain Nissen & Raphael Quinet,
 *                                                University of Liege, Belgium.
 *
 * The X Consortium, and any party obtaining a copy of these files from the
 * X Consortium, directly or indirectly, is granted, free of charge, a full
 * and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and documentation
 * files (the "Software"), including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons who receive copies from any such party
 * to do so.  This license includes without limitation a license to do the
 * foregoing actions under any patents of the party supplying this software
 * to the X Consortium.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * X CONSORTIUM, THE AUTHORS OR THE UNIVERSITY OF LIEGE BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Authors :
 *    Raphael Quinet <quinet@montefiore.ulg.ac.be>
 *                   <quinet@stud.montefiore.ulg.ac.be>
 *    Alain Nissen   <nissen@montefiore.ulg.ac.be>
 *                   <nissen@stud.montefiore.ulg.ac.be>
 */

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Grip.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <stdio.h>
#include <stdlib.h>
#include "xsession.h"

#ifdef SOUNDS
#if __STDC__
extern void play_sound(char *sound_name);
#else
extern void play_sound();
#endif
#endif /* SOUNDS */

#if __STDC__
extern void wm_menu_callback(Widget w, XtPointer client_data,
			     XtPointer call_data);
extern void app_menu_callback(Widget w, XtPointer client_data,
			      XtPointer call_data);
extern void lock_callback(Widget w, XtPointer client_data,
			  XtPointer call_data);
extern void quit_callback(Widget w, XtPointer client_data,
			  XtPointer call_data);
extern void clear_callback(Widget w, XtPointer client_data,
			   XtPointer call_data);
extern void clear2_callback(Widget w, XtPointer client_data,
			    XtPointer call_data);
extern char *zmalloc(unsigned size);
extern char *zrealloc(char *ptr, unsigned size);
#else
extern void wm_menu_callback();
extern void app_menu_callback();
extern void lock_callback();
extern void quit_callback();
extern void clear_callback();
extern void clear2_callback();
extern char *zmalloc();
extern char *zrealloc();
#endif

void activate_timeout();


extern XtAppContext       app_context;
extern resources_rec      resources;
extern wm_info_rec       *wm_info;
extern app_info_rec      *app_info;

Widget                    top_shell, ascii_text, ascii_text2;
Widget                    clear2_menu_item;

static XtIntervalId       timeout_id;
static Boolean            notified, iconified;
static Boolean            timeout_active, choosing;

/*****************************************************************************
 *                                  notify                                   *
 *---------------------------------------------------------------------------*
 * Adds an asterisk to the icon name, if the "notify" resource is True.      *
 *****************************************************************************/

void notify()
{
  Arg	args[1];
  char *old_name;
  char *new_name;

#ifdef SOUNDS
  play_sound(resources.sound_notify);
#endif /* SOUNDS */
  if (! iconified || ! resources.notify || notified)
    return;
  XtSetArg(args[0], XtNiconName, &old_name);
  XtGetValues(top_shell, args, ONE);
  new_name = (char *) zmalloc(strlen(old_name) + 3);
  if (! new_name)
    return;
  sprintf(new_name, "%s *", old_name);
  XtSetArg(args[0], XtNiconName, new_name);
  XtSetValues(top_shell, args, ONE);
  free(new_name);
  notified = True;
}


/*****************************************************************************
 *                            timeout_callback                               *
 *---------------------------------------------------------------------------*
 * Raises the main window to the top of the window stack.  This is useful if *
 * the window manager has died.  The window is also moved to a "visible"     *
 * part of the screen.                                                       *
 *****************************************************************************/

#if __STDC__
void timeout_callback(XtPointer client_data, XtIntervalId *timeout_id)
#else
void timeout_callback(client_data, timeout_id)
     XtPointer     client_data;
     XtIntervalId *timeout_id;
#endif
{
  Arg       args[4];
  Position  x, y;
  Dimension w, h;
  int       maxx, maxy;

  XtSetArg(args[0], XtNx, &x);
  XtSetArg(args[1], XtNy, &y);
  XtSetArg(args[2], XtNwidth, &w);
  XtSetArg(args[3], XtNheight, &h);
  XtGetValues(top_shell, args, FOUR);
  maxx = WidthOfScreen(XtScreen(top_shell));
  maxy = HeightOfScreen(XtScreen(top_shell));
  if ((x < 0) || (y < 0) || ((int) (x + w) > maxx) || ((int) (y + h) > maxy))
    {
      if ((int) (x + w) > maxx)
	x = maxx - w - 1;
      if ((int) (y + h) > maxy)
	y = maxy - h - 1;
      if (x < 0)
	x = 0;
      if (y < 0)
	y = 0;
      XtSetArg(args[0], XtNx, x);
      XtSetArg(args[1], XtNy, y);
      XtSetValues(top_shell, args, TWO);
    }
  if (! choosing)
    XRaiseWindow(XtDisplay(top_shell), XtWindow(top_shell));
  activate_timeout();
}


/*****************************************************************************
 *                             activate_timeout                              *
 *---------------------------------------------------------------------------*
 * Activates the automatic call to "timeout_callback" every second.          *
 *****************************************************************************/

void activate_timeout()
{
  timeout_id = XtAppAddTimeOut(app_context, 1000, timeout_callback, NULL);
  timeout_active = True;
}


/*****************************************************************************
 *                           desactivate_timeout                             *
 *---------------------------------------------------------------------------*
 * Desactivates the automatic call to "timeout_callback" every second.       *
 *****************************************************************************/

void desactivate_timeout()
{
  if (timeout_active)
    {
      XtRemoveTimeOut(timeout_id);
      timeout_active = False;
    }
}


/*****************************************************************************
 *                             start_choosing_proc                           *
 *---------------------------------------------------------------------------*
 * Sets the "choosing" flag when the user opens a menu.                      *
 *****************************************************************************/

#if __STDC__
void start_choosing_proc(Widget w, XEvent *event, String params[], Cardinal *num_params)
#else
void start_choosing_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  choosing = True;
}


/*****************************************************************************
 *                              end_choosing_proc                            *
 *---------------------------------------------------------------------------*
 * Clears the "choosing" flag when the user closes a menu.                   *
 *****************************************************************************/

#if __STDC__
void end_choosing_proc(Widget w, XEvent *event, String params[], Cardinal *num_params)
#else
void end_choosing_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  choosing = False;
}


/*****************************************************************************
 *                             start_choosing_proc                           *
 *---------------------------------------------------------------------------*
 * Sets the "iconified" flag when the user minimizes the main window.        *
 *****************************************************************************/

#if __STDC__
void iconify_proc(Widget w, XEvent *event, String params[], Cardinal *num_params)
#else
void iconify_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  iconified = True;
}


/*****************************************************************************
 *                                deiconify_proc                             *
 *---------------------------------------------------------------------------*
 * Clears the "iconified" flag when the user restores the main window and    *
 * restores the icon name if it has been modified by the "notify" function.  *
 *****************************************************************************/

#if __STDC__
void deiconify_proc(Widget w, XEvent *event, String params[], Cardinal *num_params)
#else
void deiconify_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  Arg	args[1];
  char *old_name;
  char *new_name;
  int   oldlen;

  iconified = False;
  if (! resources.notify || ! notified)
    return;
  XtSetArg(args[0], XtNiconName, &old_name);
  XtGetValues(top_shell, args, ONE);
    oldlen = strlen(old_name);
  if (oldlen >= 2)
    {
      new_name = (char *) zmalloc((oldlen - 1) * sizeof(char));
      if (! new_name)
	return;
      strncpy(new_name, old_name, oldlen - 2);
      new_name[oldlen - 2] = '\0';
      XtSetArg(args[0], XtNiconName, new_name);
      XtSetValues(top_shell, args, ONE);
      free(new_name);
    }
  notified = False;
}


/*****************************************************************************
 *                               create_widgets                              *
 *---------------------------------------------------------------------------*
 * Creates the wigets used by the program, registers the callback functions  *
 * and modifies the translation table.                                       *
 *****************************************************************************/

void create_widgets()
{
  Arg           args[2];
  XtCallbackRec callback[2];
  int           i;
  Widget        top_pane, box, menu_button, menu, menu_item;
  char          item_name[1025];

  top_pane = XtCreateManagedWidget((String) "top_pane", panedWidgetClass,
                                   top_shell, NULL, ZERO);
  XtSetArg(args[0], XtNorientation, XtorientHorizontal);
  box = XtCreateManagedWidget((String) "menubox", boxWidgetClass,
			      top_pane, args, ONE);

  menu_button = XtCreateManagedWidget((String) "session_button",
                                      menuButtonWidgetClass, box, NULL,
				      ZERO);
  XtOverrideTranslations(menu_button, XtParseTranslationTable(
                         "<BtnDown>: StartChoosing() PopupMenu()"));
  menu = XtCreatePopupShell((String) "menu", simpleMenuWidgetClass,
			    menu_button, NULL, ZERO);
  XtOverrideTranslations(menu, XtParseTranslationTable(
                         "<BtnUp>: MenuPopdown() notify() unhighlight()\
                                   EndChoosing()"));
  callback[0].callback = lock_callback;
  callback[0].closure = NULL;
  callback[1].callback = NULL;
  callback[1].closure = NULL;
  XtSetArg(args[0], XtNcallback, callback);
  menu_item = XtCreateManagedWidget((String) "lock_menu_item",
				    smeBSBObjectClass, menu, args, ONE);
  callback[0].callback = quit_callback;
  callback[0].closure = NULL;
  callback[1].callback = NULL;
  callback[1].closure = NULL;
  XtSetArg(args[0], XtNcallback, callback);
  menu_item = XtCreateManagedWidget((String) "quit_menu_item",
				    smeBSBObjectClass, menu, args, ONE);

  menu_button = XtCreateManagedWidget((String) "wm_button",
                                      menuButtonWidgetClass, box, NULL,
				      ZERO);
  XtOverrideTranslations(menu_button, XtParseTranslationTable(
                         "<BtnDown>: StartChoosing() PopupMenu()"));
  menu = XtCreatePopupShell((String) "menu", simpleMenuWidgetClass,
			    menu_button, NULL, ZERO);
  XtOverrideTranslations(menu, XtParseTranslationTable(
                         "<BtnUp>: MenuPopdown() notify() unhighlight()\
                                   EndChoosing()"));
  callback[0].callback = wm_menu_callback;
  callback[1].callback = NULL;
  callback[1].closure = NULL;
  for (i = 0; i < resources.wm_count; i ++)
    {
      XtSetArg(args[0], XtNlabel, wm_info[i].short_name);
      callback[0].closure = (XtPointer) i;
      XtSetArg(args[1], XtNcallback, callback);
      sprintf(item_name, "%s_menu_item", wm_info[i].short_name);
      menu_item = XtCreateManagedWidget((String) item_name, smeBSBObjectClass,
					menu, args, TWO);
    }
  XtSetArg(args[0], XtNlabel, "None");
  callback[0].closure = (XtPointer) NONE;
  XtSetArg(args[1], XtNcallback, callback);
  menu_item = XtCreateManagedWidget((String) "none_menu_item",
			       smeBSBObjectClass, menu, args, TWO);

  menu_button = XtCreateManagedWidget((String) "app_button",
                                      menuButtonWidgetClass, box, NULL,
				      ZERO);
  XtOverrideTranslations(menu_button, XtParseTranslationTable(
                         "<BtnDown>: StartChoosing() PopupMenu()"));
  menu = XtCreatePopupShell((String) "menu", simpleMenuWidgetClass,
			    menu_button, NULL, ZERO);
  XtOverrideTranslations(menu, XtParseTranslationTable(
                         "<BtnUp>: MenuPopdown() notify() unhighlight()\
                                   EndChoosing()"));
  callback[0].callback = app_menu_callback;
  callback[1].callback = NULL;
  callback[1].closure = NULL;
  for (i = 0; i < resources.app_count; i ++)
    {
      XtSetArg(args[0], XtNlabel, app_info[i].short_name);
      callback[0].closure = (XtPointer) i;
      XtSetArg(args[1], XtNcallback, callback);
      sprintf(item_name, "%s_menu_item", app_info[i].short_name);
      menu_item = XtCreateManagedWidget((String) item_name, smeBSBObjectClass,
					menu, args, TWO);
    }

  menu_button = XtCreateManagedWidget((String) "misc_button",
                                      menuButtonWidgetClass, box, NULL,
				      ZERO);
  XtOverrideTranslations(menu_button, XtParseTranslationTable(
                         "<BtnDown>: StartChoosing() PopupMenu()"));
  menu = XtCreatePopupShell((String) "menu", simpleMenuWidgetClass,
			    menu_button, NULL, ZERO);
  XtOverrideTranslations(menu, XtParseTranslationTable(
                         "<BtnUp>: MenuPopdown() notify() unhighlight()\
                                   EndChoosing()"));
  callback[0].callback = clear_callback;
  callback[0].closure = NULL;
  callback[1].callback = NULL;
  callback[1].closure = NULL;
  XtSetArg(args[0], XtNcallback, callback);
  menu_item = XtCreateManagedWidget((String) "clear_menu_item",
				    smeBSBObjectClass, menu, args, ONE);
  if (resources.split_window)
    {
      callback[0].callback = clear2_callback;
      callback[0].closure = NULL;
      callback[1].callback = NULL;
      callback[1].closure = NULL;
      XtSetArg(args[0], XtNcallback, callback);
      clear2_menu_item = XtCreateManagedWidget((String) "clear2_menu_item",
					   smeBSBObjectClass, menu, args, ONE);
    }

  XtSetArg(args[0], XtNfromHoriz, NULL);
  XtSetArg(args[1], XtNfromVert, box);
  ascii_text = XtCreateManagedWidget((String) "ascii_text",
				     asciiTextWidgetClass, top_pane, args,
				     TWO);
  if (resources.split_window)
    {
      XtSetArg(args[0], XtNfromHoriz, NULL);
      XtSetArg(args[1], XtNfromVert, ascii_text);
      ascii_text2 = XtCreateManagedWidget((String) "ascii_text2",
					  asciiTextWidgetClass, top_pane, args,
					  TWO);
    }
  choosing = False;
}
