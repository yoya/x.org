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

#if defined(_IBMR2) && defined(_AIX32)
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#endif

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xmu/Xmu.h>
#include <X11/Xmu/SysUtil.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "xsession.h"
#include "resources.h"

#ifdef SOUNDS
#include <rplay.h>
#endif /* SOUNDS */

#if __STDC__
extern void input_ready_callback(Widget w, int *input, XtInputId *input_id);
extern void clear_proc(Widget w, XEvent *event, String params[],
                       Cardinal *num_params);
extern void clear2_proc(Widget w, XEvent *event, String params[],
                        Cardinal *num_params);
extern void iconify_proc(Widget w, XEvent *event, String params[],
                         Cardinal *num_params);
extern void deiconify_proc(Widget w, XEvent *event, String params[],
                           Cardinal *num_params);
extern void start_choosing_proc(Widget w, XEvent *event, String params[],
                                Cardinal *num_params);
extern void end_choosing_proc(Widget w, XEvent *event, String params[],
                              Cardinal *num_params);
extern void console_log(char *s);
extern void text_insert(Widget w, char *s, int len);
extern long text_length(Widget w);
extern char *_XawTextGetSTRING(TextWidget ctx, XawTextPosition left,
			       XawTextPosition right);
void wm_protocols_proc(Widget w, XEvent *event, String params[],
		       Cardinal *num_params);
#else
extern void input_ready_callback();
extern void clear_proc();
extern void clear2_proc();
extern void iconify_proc();
extern void deiconify_proc();
extern void start_choosing_proc();
extern void end_choosing_proc();
extern void console_log();
extern void text_insert();
extern long text_length();
extern char *_XawTextGetSTRING();
void wm_protocols_proc();
#endif

extern void activate_timeout();
extern void startup();
extern void print_motd();
extern void create_widgets();
extern void get_app_resources();
extern void get_wm_resources();
extern void end_current_wm();
extern void grimreaper();
extern int  redir_console();

extern int           errno;
extern Widget        top_shell, ascii_text, ascii_text2, clear2_menu_item;
extern pid_t         osm_child_pid;
extern Boolean       child_died;

char                *program_name;
int                  pipe_fd[2];
XtAppContext         app_context;
resources_rec        resources;
Boolean              widget_ok, closing_top_shell, suid;
XtInputId            console_id;

static char          mit_console_name[268] = "MIT_CONSOLE_";
static Atom          XA_WM_DELETE_WINDOW, XA_MIT_CONSOLE;
static XtInputId     input_id;
static int           return_code;
static XtActionsRec  actions[] =
  {
    {
      "WMProtocols",   wm_protocols_proc
    },
    {
      "Clear",         clear_proc
    },
    {
      "Clear2",        clear2_proc
    },
    {
      "Iconify",       iconify_proc
    },
    {
      "Deiconify",     deiconify_proc
    },
    {
      "StartChoosing", start_choosing_proc
    },
    {
      "EndChoosing",   end_choosing_proc
    }
  };


#ifdef SOUNDS
/*****************************************************************************
 *                                 play_sound                                *
 *---------------------------------------------------------------------------*
 * Plays a sound on the current display, using the rplay library.            *
 *****************************************************************************/

#if __STDC__
void play_sound(char *sound_name)
#else
void play_sound(sound_name)
     char *sound_name;
#endif
{
  if (resources.sounds && (sound_name != NULL) && strcmp(sound_name, "none"))
    {
      if (rplay_display(sound_name) < 0)
	console_log("Error: can't play a sound\n");
    }
}
#endif /* SOUNDS */


/*****************************************************************************
 *                               quit_callback                               *
 *---------------------------------------------------------------------------*
 * Terminates the program.                                                   *
 *****************************************************************************/

#if __STDC__
void quit_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void quit_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
  end_current_wm();
  if (osm_child_pid)
    {
      kill(osm_child_pid, SIGTERM);
      while (osm_child_pid)
	{
	  while (child_died == True)
	    grimreaper();
	  sleep(1);
	}
    }
  widget_ok = False;
  if (input_id)
    XtRemoveInput(input_id);
  if (console_id)
    XtRemoveInput(console_id);
  if (top_shell)
    XtDestroyWidget(top_shell);
  /*
   * The following hack shuts down the socket used by "rsh".  If xsession is
   * launched with a remote shell (e.g. "xon numbercruncher xsession -remote"),
   * this is the only way to be sure that the "rsh" ends as soon as you quit
   * xsession.  If you don't use the "-remote" option, the socket will remain
   * open as long as there is a child of xsession alive (e.g. xterm, emacs,...)
   * and the "rsh" won't end.
   */
  if (resources.remote)
    shutdown(fileno(stdout), 2);
#ifdef SOUNDS
  play_sound(resources.sound_end);
#endif /* SOUNDS */
  exit(return_code);
}


/*****************************************************************************
 *                                fatal_error                                *
 *---------------------------------------------------------------------------*
 * Prints an error message and terminates the program.                       *
 *****************************************************************************/

#if __STDC__
void fatal_error(int error_code, char *message)
#else
void fatal_error(error_code, message)
     int   error_code;
     char *message;
#endif
{
  return_code = error_code;
  fprintf(stderr, "%s: ", program_name);
  switch (error_code)
    {
      case ERR_SYNTAX :
        fprintf(stderr, "Syntax error.  Parameter \"%s\" not recognized.\n",
		message);
        break;
      case ERR_BAD_LIST :
        fprintf(stderr, "Invalid list.\n");
        break;
      case ERR_MEMORY :
        fprintf(stderr, "Out of memory!  Cannot allocate %u bytes\n",
		*((unsigned *) message));
        break;
      case ERR_PREPROCESSOR : 
	fprintf(stderr, "Syntax error.  Bad value for the `-preprocessor' option.\n");
	break;
      case ERR_SUID :
	fprintf(stderr, "Compilation error.  Setuid has not been enabled at compilation-time.\n");
	break;
      case ERR_ROOT :
	fprintf(stderr, "Setuid error.  Non root-setuid program.\n");
	break;
      default :
        fprintf(stderr, "Error code #%d.\n", error_code);
    }
  quit_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
}


/*****************************************************************************
 *                                  zmalloc                                  *
 *---------------------------------------------------------------------------*
 * Just a wrapper around malloc().                                           *
 *****************************************************************************/

#if __STDC__
char *zmalloc(unsigned size)
#else
char *zmalloc(size)
     unsigned size;
#endif
{
  char *p;

  p = (char *) malloc(size);
  if (p == NULL)
    fatal_error(ERR_MEMORY, (char *)&size);
  return p;
}


/*****************************************************************************
 *                                  zrealloc                                 *
 *---------------------------------------------------------------------------*
 * Just a wrapper around realloc().                                          *
 *****************************************************************************/

#if __STDC__
char *zrealloc(char *ptr, unsigned size)
#else
char *zrealloc(ptr, size)
     char *ptr;
     unsigned size;
#endif
{
  char *p;

  p = (char *) realloc(ptr, size);
  if (p == NULL)
    fatal_error(ERR_MEMORY, (char *)&size);
  return p;
}


/*****************************************************************************
 *                               wm_protocols_proc                           *
 *---------------------------------------------------------------------------*
 * Handles WM_PROTOCOL client messages.  If a WM_DELETE_WINDOW is sent, then *
 * the program must exit.                                                    *
 *****************************************************************************/

#if __STDC__
void wm_protocols_proc(Widget w, XEvent *event, String params[],
		       Cardinal *num_params)
#else
void wm_protocols_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  if (event -> xclient.data.l[0] == XA_WM_DELETE_WINDOW)
    if (! closing_top_shell)
      {
        closing_top_shell = True;
        quit_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
        closing_top_shell = False;
      }
}


/* The three following functions (selection_lost, convert_selection and      */
/* copy_selection_callback) were taken and munged from "xconsole.c".         */
/*                                        XConsole is (C) 1990 X Consortium. */

/*****************************************************************************
 *                               selection_lost                              *
 *---------------------------------------------------------------------------*
 * Warns the user that the console redirection is now inactive.              *
 *****************************************************************************/

#if __STDC__
static void selection_lost(Widget w, Atom *selection)
#else
static void selection_lost(w, selection)
     Widget  w;
     Atom   *selection;
#endif
{
  if (!resources.quiet)
    console_log("Warning: console output lost.\n");
  if (console_id)
    {
      XtRemoveInput(console_id);
      console_id = 0;
    }
  if (resources.split_window)
    {
      Arg arg[1];

      resources.split_window = False;
      XtUnmanageChild(ascii_text2);
      XtSetArg(arg[0], XtNsensitive, False);
      XtSetValues(clear2_menu_item, arg, ONE);
    }
}


/*****************************************************************************
 *                             convert_selection                             *
 *---------------------------------------------------------------------------*
 * Is called whenever someone requests the current value of the selection;   *
 * we can offer the console output only if separate windows are used.        *
 *****************************************************************************/

#if __STDC__
static Boolean convert_selection(Widget w, Atom *selection, Atom *target,
				 Atom *type, XtPointer *value,
				 unsigned long *length, int *format)
#else
static Boolean convert_selection(w, selection, target, type, value, length,
				 format)
     Widget         w;
     Atom          *selection, *target, *type;
     XtPointer     *value;
     unsigned long *length;
     int           *format;
#endif
{
  Display                *d;
  XSelectionRequestEvent *req;
  Atom                   *targetP, *std_targets;
  unsigned long           std_length;
  long                   *temp;

  if (resources.console && resources.split_window)
    {
      d = XtDisplay(w);
      req = XtGetSelectionRequest(w, *selection, (XtRequestId) NULL);
      if (*target == XA_TARGETS(d))
	{
	  XmuConvertStandardSelection(w, req->time, selection, target, type,
				      (caddr_t *) &std_targets, &std_length,
				      format);
	  *value = (XtPointer) XtMalloc(sizeof(Atom) * (std_length + 5));
	  targetP = *(Atom **) value;
	  *targetP++ = XA_STRING;
	  *targetP++ = XA_TEXT(d);
	  *targetP++ = XA_LENGTH(d);
	  *targetP++ = XA_LIST_LENGTH(d);
	  *targetP++ = XA_CHARACTER_POSITION(d);
	  *length = std_length + (targetP - (*(Atom **) value));
	  memcpy((char *) std_targets, (char *) targetP, sizeof(Atom) *
		 std_length);
	  XtFree((char *) std_targets);
	  *type = XA_ATOM;
	  *format = 32;
	  return True;
	}
      if (*target == XA_LIST_LENGTH(d) || *target == XA_LENGTH(d))
	{
	  temp = (long *) XtMalloc(sizeof(long));
	  if (*target == XA_LIST_LENGTH(d))
	    *temp = 1L;
	  else			/* *target == XA_LENGTH(d) */
	    *temp = (long) text_length(ascii_text2);
	  *value = (XtPointer) temp;
	  *type = XA_INTEGER;
	  *length = 1L;
	  *format = 32;
	  return True;
	}
      if (*target == XA_CHARACTER_POSITION(d))
	{
	  temp = (long *) XtMalloc(2 * sizeof(long));
	  temp[0] = (long) 0;
	  temp[1] = text_length(ascii_text2);
	  *value = (XtPointer) temp;
	  *type = XA_SPAN(d);
	  *length = 2L;
	  *format = 32;
	  return True;
	}
      if (*target == XA_STRING || *target == XA_TEXT(d) ||
	  *target == XA_COMPOUND_TEXT(d))
	{
	  if (*target == XA_COMPOUND_TEXT(d))
	    *type = *target;
	  else
	    *type = XA_STRING;
	  *length = text_length(ascii_text2);
	  *value = (XtPointer) _XawTextGetSTRING((TextWidget) ascii_text2, 0,
						 *length);
	  *format = 8;
	  return True;
	}
      if (XmuConvertStandardSelection(w, req->time, selection, target, type,
				      (caddr_t *) value, length, format))
	return True;
      return False;
    }
  else
    return False;
}


/*****************************************************************************
 *                          copy_selection_callback                          *
 *---------------------------------------------------------------------------*
 * Copies the content of the running XConsole window to our own window.      *
 *****************************************************************************/

#if __STDC__
static void copy_selection_callback(Widget w, XtPointer client_data,
				    Atom *selection, Atom *type,
				    XtPointer value, unsigned long *length,
				    int *format)
#else
static void copy_selection_callback(w, client_data, selection, type, value,
				    length, format)
     Widget         w;
     XtPointer      client_data;
     Atom          *selection, *type;
     XtPointer      value;
     unsigned long *length;
     int           *format;
#endif
{
  if (*type != XT_CONVERT_FAIL)
    text_insert(ascii_text, (char *) value, *length);
  XtOwnSelection(top_shell, XA_MIT_CONSOLE, CurrentTime, convert_selection,
		 selection_lost, NULL);
}
    

/*****************************************************************************
 *                                    main                                   *
 *---------------------------------------------------------------------------*
 * Believe me: the program won't run without this function...                *
 *****************************************************************************/

#if __STDC__
#if defined(sgi) || defined(__sgi) || defined(__sgi__)
void main(unsigned int argc, char *argv[])
#else
void main(int argc, char *argv[])
#endif
#else
void main(argc, argv)
#if defined(sgi) || defined(__sgi) || defined(__sgi__)
     unsigned int argc;
#else
     int   argc;
#endif
     char *argv[];
#endif
{
  char    *p;
  int      console;
  Boolean  suid_fail;

  if (getuid() != geteuid())
    {
      suid = True;
#ifndef ENABLE_SUID
      suid_fail = True;
#else
      suid_fail = False;
#endif /* ENABLE_SUID */
    }
  else
    {
      suid = False;
      suid_fail = False;
    }
  if ((program_name = strrchr(argv[0], (int) '/')) == NULL)
    program_name = argv[0];
  else
    program_name ++;
  if ((suid == True) && (suid_fail == True))
    fatal_error(ERR_SUID, (char *) NULL);
  if ((suid == True) && geteuid())
    fatal_error(ERR_ROOT, (char *) NULL);
  return_code = 0;
  widget_ok = False;
  top_shell = XtAppInitialize(&app_context, "XSession",
                              options, XtNumber(options),
			      &argc, (String *) argv,
                              fallback_resources, NULL, ZERO);
  XA_WM_DELETE_WINDOW = XInternAtom(XtDisplay(top_shell),
				    "WM_DELETE_WINDOW", False);
  XtGetApplicationResources(top_shell, (XtPointer) &resources,
                            resources_desc, XtNumber(resources_desc),
			    NULL, ZERO);
  XtAppAddActions(app_context, actions, XtNumber(actions));

  resources.num_defines = 0;
  for (argv++, argc--; argc; argv++, argc--)
    if (!strcmp(*argv, "-help") || !strcmp(*argv, "--help")
	|| !strcmp(*argv, "-usage") || !strcmp(*argv, "--usage"))
      {
	printf("Usage: %s [-toolkitoption ...] [-console] [-cpp] [-defaultWM <name>]\n",
	       program_name);
	printf("                [-D<symbol>[=<value>]] [-keep] [-m4] [-notify]\n");
#ifdef SOUNDS
	printf("                [-preprocessor m4|cpp] [-remote] [-sounds] [-split] [-startup]\n");
        printf("                [-version] [-help]\n");
#else
	printf("                [-preprocessor m4|cpp] [-remote] [-split] [-startup] [-version]\n");
        printf("                [-help]\n");
#endif /* SOUNDS */
        quit_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
      }
    else
      if (!strcmp(*argv, "-version") || !strcmp(*argv, "--version"))
	{
	  printf("XSession %s\n", VERSION);
	  quit_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
	}
      else
	if ((argv[0][0] == '-') && (argv[0][1] == 'D'))   /* defines */
	  {
	    p = *argv + 2;
	    while (*p && (*p != '='))
	      p++;
	    if (*p == '=')
	      {
		*p = '\0';
		p++;
	      }
	    if (resources.num_defines++ == 0)
	      {
		resources.defines_sym = (String *) zmalloc(sizeof(String));
		resources.defines_val = (String *) zmalloc(sizeof(String));
		resources.defines_sym[0] = *argv + 2;
		resources.defines_val[0] = p;
	      }
	    else
	      {
		resources.defines_sym =
		  (String *) zrealloc((char *) resources.defines_sym,
				      resources.num_defines * sizeof(String));
		resources.defines_val =
		  (String *) zrealloc((char *) resources.defines_val,
				      resources.num_defines * sizeof(String));
		resources.defines_sym[resources.num_defines - 1] = *argv + 2;
		resources.defines_val[resources.num_defines - 1] = p;
	      }
	  }
	else
	  fatal_error(ERR_SYNTAX, *argv);
  if (strcmp(resources.preprocessor, "cpp") && strcmp(resources.preprocessor, "m4"))
    fatal_error(ERR_PREPROCESSOR, (char *) NULL);
  get_wm_resources();
  get_app_resources();
  p = getenv("HOME");
  if (p != NULL)
    chdir(p);
  if (getenv("DISPLAY") == NULL)
    {
      p = (char *) zmalloc((10 + strlen(DisplayString(XtDisplay(top_shell))))
			   * sizeof(char));
      sprintf(p, "DISPLAY=%s", DisplayString(XtDisplay(top_shell)));
      putenv(p);
    }
  if (!resources.console)
    resources.split_window = False;
  create_widgets();
  XtRealizeWidget(top_shell);
  widget_ok = True;
  XSetWMProtocols(XtDisplay(top_shell), XtWindow(top_shell),
		  &XA_WM_DELETE_WINDOW, 1);
  pipe(pipe_fd);
  input_id = XtAppAddInput(app_context, pipe_fd[0],
			   (XtPointer) XtInputReadMask,
                           (XtInputCallbackProc) input_ready_callback, NULL);
  activate_timeout();
  if (resources.motd)
    print_motd();
  osm_child_pid = 0;
  if (resources.console)
    {
      XmuGetHostname(mit_console_name + 12, 255);
      XA_MIT_CONSOLE = XInternAtom(XtDisplay(top_shell), mit_console_name,
				   False);
      if (XGetSelectionOwner(XtDisplay(top_shell), XA_MIT_CONSOLE))
	XtGetSelectionValue(top_shell, XA_MIT_CONSOLE, XA_STRING,
			    copy_selection_callback, NULL, CurrentTime);
      else
	XtOwnSelection(top_shell, XA_MIT_CONSOLE, CurrentTime,
		       convert_selection, selection_lost, NULL);
      console = redir_console();
      if (console >= 0)
	{
	  console_id = XtAppAddInput(app_context, console,
				     (XtPointer) XtInputReadMask,
				     (XtInputCallbackProc)
				     input_ready_callback, NULL);
	}
      else
	{
	  console_id = 0;
	  if (!resources.quiet)
	    console_log("Warning: cannot redirect console messages.\n");
	  if (resources.split_window)
	    {
	      Arg arg[1];

	      resources.split_window = False;
	      XtUnmanageChild(ascii_text2);
	      XtSetArg(arg[0], XtNsensitive, False);
	      XtSetValues(clear2_menu_item, arg, ONE);
	    }
	}
    }
  else
    console_id = 0;
  if (suid == True)
    {
      setgid(getgid());
      setuid(getuid());
    }
#ifdef SOUNDS
  play_sound(resources.sound_start);
#endif /* SOUNDS */
  startup();
  XtAppMainLoop(app_context);
}
