/* xscreensaver, Copyright (c) 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#include <X11/Intrinsic.h>

#if !__STDC__
# define _NO_PROTO
#endif

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/ToggleB.h>

#include "xscreensaver.h"
#include <stdio.h>

extern Time timeout, cycle, lock_timeout;
#ifndef NO_LOCKING
extern Time passwd_timeout;
#endif
extern int fade_seconds, fade_ticks;
extern Bool verbose_p, install_cmap_p, fade_p, unfade_p;
extern Bool lock_p, locking_disabled_p;

static void demo_mode_hack P((char *));
static void demo_mode_done P((void));

extern void demo_mode_restart_process ();

extern Widget demo_dialog;
extern Widget label1;
extern Widget text_line;
extern Widget demo_form;
extern Widget demo_list;
extern Widget next, prev, done, restart, edit;

extern Widget resources_dialog;
extern Widget resources_form;
extern Widget res_done, res_cancel;
extern Widget timeout_text, cycle_text, fade_text, ticks_text;
extern Widget lock_time_text, passwd_time_text;
extern Widget verbose_toggle, cmap_toggle, fade_toggle, unfade_toggle,
  lock_toggle;

extern create_demo_dialog ();
extern create_resources_dialog ();

static void
focus_fuckus (dialog)
     Widget dialog;
{
  XSetInputFocus (XtDisplay (dialog), XtWindow (dialog),
		  RevertToParent, CurrentTime);
}

static void
raise_screenhack_dialog ()
{
  XMapRaised (XtDisplay (demo_dialog), XtWindow (demo_dialog));
  if (resources_dialog)
    XMapRaised (XtDisplay (resources_dialog), XtWindow (resources_dialog));
  focus_fuckus (resources_dialog ? resources_dialog : demo_dialog);
}

static void
destroy_screenhack_dialogs ()
{
  if (demo_dialog) XtDestroyWidget (demo_dialog);
  if (resources_dialog) XtDestroyWidget (resources_dialog);
  demo_dialog = resources_dialog = 0;
}

static void
text_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  char *line = XmTextGetString (button);
  demo_mode_hack (line);
}


static void
select_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  char **hacks = (char **) client_data;
  XmListCallbackStruct *lcb = (XmListCallbackStruct *) call_data;
  XmTextSetString (text_line, hacks [lcb->item_position - 1]);
  if (lcb->reason == XmCR_DEFAULT_ACTION)
    text_cb (text_line, 0, 0);
  focus_fuckus (demo_dialog);
}

static void
next_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  int *pos_list;
  int pos_count;
  if (! XmListGetSelectedPos (demo_list, &pos_list, &pos_count))
    XmListSelectPos (demo_list, 1, True);
  else
    {
      int pos = pos_list [0];
      XmListSelectPos (demo_list, pos + 1, True);
      XtFree ((char *) pos_list);
      if (! XmListGetSelectedPos (demo_list, &pos_list, &pos_count))
	abort ();
      if (pos_list [0] == pos)
	XmListSelectPos (demo_list, 1, True);
      XtFree ((char *) pos_list);
    }
  text_cb (text_line, 0, 0);
}

static void
prev_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  int *pos_list;
  int pos_count;
  if (! XmListGetSelectedPos (demo_list, &pos_list, &pos_count))
    XmListSelectPos (demo_list, 0, True);
  else
    {
      XmListSelectPos (demo_list, pos_list [0] - 1, True);
      XtFree ((char *) pos_list);
    }
  text_cb (text_line, 0, 0);
}


static void pop_resources_dialog ();
static void make_resources_dialog ();

static void
edit_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  Widget parent = (Widget) client_data;
  if (! resources_dialog)
    make_resources_dialog (parent);
  pop_resources_dialog ();
}

static void
done_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  demo_mode_done ();
}


static void
restart_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  demo_mode_restart_process ();
}

void
pop_up_dialog_box (dialog, form, where)
     Widget dialog, form;
     int where;
{
  /* I'm sure this is the wrong way to pop up a dialog box, but I can't
     figure out how else to do it.

     It's important that the screensaver dialogs not get decorated or
     otherwise reparented by the window manager, because they need to be
     children of the *real* root window, not the WM's virtual root, in
     order for us to guarentee that they are visible above the screensaver
     window itself.
   */
  Arg av [100];
  int ac = 0;
  Dimension sw, sh, x, y, w, h;
  XtRealizeWidget (form);
  sw = WidthOfScreen (XtScreen (dialog));
  sh = HeightOfScreen (XtScreen (dialog));
  ac = 0;
  XtSetArg (av [ac], XmNwidth, &w); ac++;
  XtSetArg (av [ac], XmNheight, &h); ac++;
  XtGetValues (form, av, ac);
  switch (where)
    {
    case 0:	/* center it in the top-right quadrant */
      x = (sw/2 + w) / 2 + (sw/2) - w;
      y = (sh/2 + h) / 2 - h;
      break;
    case 1:	/* center it in the bottom-right quadrant */
      x = (sw/2 + w) / 2 + (sw/2) - w;
      y = (sh/2 + h) / 2 + (sh/2) - h;
      break;
    case 2:	/* center it on the screen */
      x = (sw + w) / 2 - w;
      y = (sh + h) / 2 - h;
      break;
    default:
      abort ();
    }
  if (x + w > sw) x = sw - w;
  if (y + h > sh) y = sh - h;
  ac = 0;
  XtSetArg (av [ac], XmNx, x); ac++;
  XtSetArg (av [ac], XmNy, y); ac++;
  XtSetArg (av [ac], XtNoverrideRedirect, True); ac++;
  XtSetArg (av [ac], XmNdefaultPosition, False); ac++;
  /* I wonder whether this does anything useful? */
  /*  XtSetArg (av [ac], XmNdialogStyle, XmDIALOG_SYSTEM_MODAL); ac++; */
  XtSetValues (dialog, av, ac);
  XtSetValues (form, av, ac);
  XtManageChild (form);

  focus_fuckus (dialog);
}


static void
make_screenhack_dialog (parent, hacks)
     Widget parent;
     char **hacks;
{
  char buf [255];
  Arg av[10];
  int ac;
  char *label;
  Dimension max_w = 0;
  XmString xm_label = 0;
  XmString new_xm_label;

  create_demo_dialog (parent);
  ac = 0;
  XtSetArg (av [ac], XmNlabelString, &xm_label); ac++;
  XtGetValues (label1, av, ac);
  XmStringGetLtoR (xm_label, XmSTRING_DEFAULT_CHARSET, &label);
  if (!strcmp (label, XtName (label1)))
    strcpy (buf, "ERROR: RESOURCES ARE NOT INSTALLED CORRECTLY");
  else
    sprintf (buf, label, screensaver_version);
  new_xm_label = XmStringCreate (buf, XmSTRING_DEFAULT_CHARSET);
  ac = 0;
  XtSetArg (av [ac], XmNlabelString, new_xm_label); ac++;
  XtSetValues (label1, av, ac);
  XmStringFree (new_xm_label);
  XtFree (label);

  XtAddCallback (demo_list, XmNbrowseSelectionCallback, select_cb,
		 (XtPointer) hacks);
  XtAddCallback (demo_list, XmNdefaultActionCallback, select_cb,
		 (XtPointer) hacks);

  XtAddCallback (text_line, XmNactivateCallback, text_cb, 0);
  XtAddCallback (next, XmNactivateCallback, next_cb, 0);
  XtAddCallback (prev, XmNactivateCallback, prev_cb, 0);
  XtAddCallback (done, XmNactivateCallback, done_cb, 0);
  XtAddCallback (restart, XmNactivateCallback, restart_cb, 0);
  XtAddCallback (edit, XmNactivateCallback, edit_cb, (XtPointer) parent);

  for (; *hacks; hacks++)
    {
      XmString xmstr = XmStringCreate (*hacks, XmSTRING_DEFAULT_CHARSET);
      XmListAddItem (demo_list, xmstr, 0);
      /* XmListSelectPos (widget, i, False); */
      XmStringFree (xmstr);
    }

#if 0
  /* Dialogs that have scroll-lists don't obey maxWidth!  Fuck!!  Hack it. */
  ac = 0;
  XtSetArg (av [ac], XmNmaxWidth, &max_w); ac++;
  XtGetValues (demo_dialog, av, ac); /* great, this SEGVs */
#endif

  pop_up_dialog_box (demo_dialog, demo_form, 0);
}


/* the Screensaver Parameters dialog */

static struct resources {
  int timeout, cycle, secs, ticks, lock_time, passwd_time;
  int verb, cmap, fade, unfade, lock_p;
} res;


extern int parse_time ();

static void 
hack_time_cb (dpy, line, store, sec_p)
     Display *dpy;
     char *line;
     int *store;
     Bool sec_p;
{
  if (*line)
    {
      int value;
      value = parse_time (line, sec_p, True);
      if (value < 0)
	/*XBell (dpy, 0)*/;
      else
	*store = value;
    }
}

static void
res_sec_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  hack_time_cb (XtDisplay (button), XmTextGetString (button),
		(int *) client_data, True);
}

static void
res_min_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  hack_time_cb (XtDisplay (button), XmTextGetString (button),
		(int *) client_data, False);
}

static void
res_int_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  char *line = XmTextGetString (button);
  int *store = (int *) client_data;
  unsigned int value;
  char c;
  if (! *line)
    ;
  else if (sscanf (line, "%u%c", &value, &c) != 1)
    XBell (XtDisplay (button), 0);
  else
    *store = value;
}

static void
res_bool_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  int *store = (int *) client_data;
  *store = ((XmToggleButtonCallbackStruct *) call_data)->set;
}

static void
res_cancel_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  XtDestroyWidget (resources_dialog);
  resources_dialog = 0;
  raise_screenhack_dialog ();
}


static void
res_done_cb (button, client_data, call_data)
     Widget button;
     XtPointer client_data, call_data;
{
  res_cancel_cb (button, client_data, call_data);

  if (res.timeout < 10) res.timeout = 10;
  if (res.cycle < 2) res.cycle = 2;
  if (res.passwd_time < 2) res.passwd_time = 30;

  timeout = res.timeout * 1000;
  cycle = res.cycle * 1000;
  lock_timeout = res.lock_time * 1000;
#ifndef NO_LOCKING
  passwd_timeout = res.passwd_time * 1000;
#endif
  fade_seconds = res.secs;
  fade_ticks = res.ticks;
  verbose_p = res.verb;
  install_cmap_p = res.cmap;
  fade_p = res.fade;
  unfade_p = res.unfade;
  lock_p = res.lock_p;
}


static void
make_resources_dialog (parent)
     Widget parent;
{
  Arg av[10];
  int ac;

  create_resources_dialog (parent);

  XtAddCallback (res_done, XmNactivateCallback, res_done_cb, 0);
  XtAddCallback (res_cancel, XmNactivateCallback, res_cancel_cb, 0);

#define CB(widget,type,slot) \
	XtAddCallback ((widget), XmNvalueChangedCallback, (type), \
		       (XtPointer) (slot))
  CB (timeout_text,	res_min_cb,  &res.timeout);
  CB (cycle_text,	res_min_cb,  &res.cycle);
  CB (fade_text,	res_sec_cb,  &res.secs);
  CB (ticks_text,	res_int_cb,  &res.ticks);
  CB (lock_time_text,	res_min_cb,  &res.lock_time);
  CB (passwd_time_text,	res_sec_cb,  &res.passwd_time);
  CB (verbose_toggle,	res_bool_cb, &res.verb);
  CB (cmap_toggle,	res_bool_cb, &res.cmap);
  CB (fade_toggle,	res_bool_cb, &res.fade);
  CB (unfade_toggle,	res_bool_cb, &res.unfade);
  CB (lock_toggle,	res_bool_cb, &res.lock_p);
#undef CB
  ac = 0;
  XtSetArg (av[ac], XmNsensitive, False); ac++;

  if (locking_disabled_p)
    {
      XtSetValues (passwd_time_text, av, ac);
      XtSetValues (lock_time_text, av, ac);
      XtSetValues (lock_toggle, av, ac);
    }
  if (CellsOfScreen (XtScreen (parent)) <= 2)
    {
      XtSetValues (fade_text, av, ac);
      XtSetValues (ticks_text, av, ac);
      XtSetValues (cmap_toggle, av, ac);
      XtSetValues (fade_toggle, av, ac);
      XtSetValues (unfade_toggle, av, ac);
    }
}


static void
fmt_time (buf, s, min_p)
     char *buf;
     unsigned int s;
     int min_p;
{
  unsigned int h = 0, m = 0;
  if (s >= 60)
    {
      m += (s / 60);
      s %= 60;
    }
  if (m >= 60)
    {
      h += (m / 60);
      m %= 60;
    }
/*
  if (min_p && h == 0 && s == 0)
    sprintf (buf, "%u", m);
  else if (!min_p && h == 0 && m == 0)
    sprintf (buf, "%u", s);
  else
  if (h == 0)
    sprintf (buf, "%u:%02u", m, s);
  else
*/
    sprintf (buf, "%u:%02u:%02u", h, m, s);
}

static void
pop_resources_dialog ()
{
  char buf [100];

  res.timeout = timeout / 1000;
  res.cycle = cycle / 1000;
  res.lock_time = lock_timeout / 1000;
#ifndef NO_LOCKING
  res.passwd_time = passwd_timeout / 1000;
#endif
  res.secs = fade_seconds;
  res.ticks = fade_ticks;
  res.verb = verbose_p;
  res.cmap = install_cmap_p;
  res.fade = fade_p;
  res.unfade = unfade_p;
  res.lock_p = (lock_p && !locking_disabled_p);

  fmt_time (buf, res.timeout, 1);     XmTextSetString (timeout_text, buf);
  fmt_time (buf, res.cycle, 1);       XmTextSetString (cycle_text, buf);
  fmt_time (buf, res.lock_time, 1);   XmTextSetString (lock_time_text, buf);
  fmt_time (buf, res.passwd_time, 0); XmTextSetString (passwd_time_text, buf);
  fmt_time (buf, res.secs, 0);        XmTextSetString (fade_text, buf);
  sprintf (buf, "%u", res.ticks);     XmTextSetString (ticks_text, buf);

  XmToggleButtonSetState (verbose_toggle, res.verb, True);
  XmToggleButtonSetState (cmap_toggle, res.cmap, True);
  XmToggleButtonSetState (fade_toggle, res.fade, True);
  XmToggleButtonSetState (unfade_toggle, res.unfade, True);
  XmToggleButtonSetState (lock_toggle, res.lock_p, True);

  pop_up_dialog_box (resources_dialog, resources_form, 1);
}


/* The code on this page isn't actually Motif-specific */

Bool dbox_up_p = False;
Bool demo_mode_p = False;

extern XtAppContext app;
extern Widget toplevel_shell;
extern Bool use_xidle;
extern Time notice_events_timeout;

extern char **screenhacks;
extern char *demo_hack;

extern void notice_events_timer P((XtPointer closure, void *timer));
extern Bool handle_clientmessage P((/*XEvent *, Bool*/));

void
demo_mode ()
{
  dbox_up_p = True;
  initialize_screensaver_window ();
  raise_window (True, False);
  make_screenhack_dialog (toplevel_shell, screenhacks);
  while (demo_mode_p)
    {
      XEvent event;
      XtAppNextEvent (app, &event);
      switch (event.xany.type)
	{
	case 0:		/* synthetic "timeout" event */
	  break;

	case ClientMessage:
	  handle_clientmessage (&event, False);
	  break;

	case CreateNotify:
#ifdef HAVE_XIDLE
	  if (! use_xidle)
#endif
	    XtAppAddTimeOut (app, notice_events_timeout, notice_events_timer,
			     (XtPointer) event.xcreatewindow.window);
	  break;

	case ButtonPress:
	case ButtonRelease:
	  if (!XtWindowToWidget (dpy, event.xbutton.window))
	    raise_screenhack_dialog ();
	  /* fall through */

	default:
	  XtDispatchEvent (&event);
	  break;
	}
    }
  destroy_screenhack_dialogs ();
  initialize_screensaver_window ();
  unblank_screen ();
}

static void
demo_mode_hack (hack)
     char *hack;
{
  if (! demo_mode_p) abort ();
  kill_screenhack ();
  if (! demo_hack)
    blank_screen ();
  demo_hack = hack;
  spawn_screenhack (False);
}

static void
demo_mode_done ()
{
  kill_screenhack ();
  if (demo_hack)
    unblank_screen ();
  demo_mode_p = False;
  dbox_up_p = False;
  demo_hack = 0;
}
