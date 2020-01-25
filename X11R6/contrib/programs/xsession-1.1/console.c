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

#if ((defined(sun) || defined(__sun)) && (defined(__SVR4)) || defined(__svr4__)) || (defined(_IBMR2) && defined(_AIX32))
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#endif

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/AsciiText.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <signal.h>
#if defined(sgi) || defined(__sgi) || defined(__sgi__)
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "xsession.h"

extern char         *program_name;
extern resources_rec resources;
extern Widget        top_shell, ascii_text, ascii_text2;
extern Boolean       widget_ok, child_died, hangup_received, closing_top_shell,
                     suid;
extern XtInputId     console_id;
extern pid_t         osm_child_pid;

#if __STDC__
extern void quit_callback(Widget w, XtPointer client_data,
			  XtPointer call_data);
#else
extern void quit_callback();
#endif

#if __STDC__ && (defined(sun) || defined(__sun)) && (defined(__SVR4) || defined(__svr4__))
extern char *ptsname(int);  /* forgotten prototypes when using the ANSI mode */
extern int   grantpt(int);  /* on Solaris 2.x.                               */
extern int   unlockpt(int);
#endif

extern void grimreaper();
extern void notify();

/* The seven following functions (strip_non_print, text_length,              */
/* text_replace, text_append, text_insert, clear_callback and clear_proc)    */
/* were taken and munged from "xconsole.c".                                  */
/*                                        XConsole is (C) 1990 X Consortium. */

/*****************************************************************************
 *                            strip_non_print                                *
 *---------------------------------------------------------------------------*
 * Strips any non-printing characters from the input buffer "old" and        *
 * places the stripped text in the same buffer.  If the text contains BEL    *
 * characters (007), this function returns True.                             *
 *****************************************************************************/

#if __STDC__
static Boolean strip_non_print(char *old)
#else
static Boolean strip_non_print(old)
     char *old;
#endif
{
  char    *new;
  Boolean  ring_bell;

  ring_bell = False;
  new = old;
  while(*old != '\0')
    {
      if (isalnum(*old) || ispunct(*old) || (isspace(*old) && *old != '\r'))
        {
          if (new != old)
            *new = *old;
          new++;
        }
      else
	if (*old == '\7')
	  ring_bell = True;
      old++;
    }
  *new = '\0';
  return ring_bell;
}


/*****************************************************************************
 *                               text_length                                 *
 *---------------------------------------------------------------------------*
 * Returns the length of the text string in the AsciiText widget "w".        *
 *****************************************************************************/

#if __STDC__
long text_length(Widget w)
#else
long text_length(w)
     Widget  w;
#endif
{
  return XawTextSourceScan(XawTextGetSource(w), (XawTextPosition) 0, XawstAll,
                           XawsdRight, 1, TRUE);
}


/*****************************************************************************
 *                               text_replace                                *
 *---------------------------------------------------------------------------*
 * Replaces a block of text in the AsciiText widget "w" by another block.    *
 *****************************************************************************/

#if __STDC__
static void text_replace(Widget w, int start, int end, XawTextBlock *block)
#else
static void text_replace(w, start, end, block)
     Widget	   w;
     int           start, end;
     XawTextBlock *block;
#endif
{
  Arg		  arg;
  Widget	  source;
  XawTextEditType edit_mode;

  source = XawTextGetSource(w);
  XtSetArg(arg, XtNeditType, &edit_mode);
  XtGetValues(source, &arg, ONE);
  XtSetArg(arg, XtNeditType, XawtextEdit);
  XtSetValues(source, &arg, ONE);
  XawTextReplace(w, start, end, block);
  XtSetArg(arg, XtNeditType, edit_mode);
  XtSetValues(source, &arg, ONE);
}


/*****************************************************************************
 *                               text_append                                 *
 *---------------------------------------------------------------------------*
 * Appends a text string "s" to the end of the text block in the AsciiText   *
 * widget "w".  If the insertion point was at the end of the text block, it  *
 * is moved to the new end.                                                  *
 *****************************************************************************/

#if __STDC__
static void text_append(Widget w, char *s, int len)
#else
static void text_append(w, s, len)
     Widget  w;
     char   *s;
     int     len;
#endif
{
  int	       last, current;
  XawTextBlock block;

  current = XawTextGetInsertionPoint(w);
  last = text_length(w);
  block.ptr = s;
  block.firstPos = 0;
  block.length = len;
  block.format = FMT8BIT;
  text_replace(w, last, last, &block);
  if (current == last)
    XawTextSetInsertionPoint(w, last + block.length);
}


/*****************************************************************************
 *                               text_insert                                 *
 *---------------------------------------------------------------------------*
 * Inserts a text string "s" at the beginningof the text block in the        *
 * AsciiText widget "w".  If the insertion point was at the beginning of the *
 * text block, it is moved after the new string "s".                         *
 *****************************************************************************/

#if __STDC__
void text_insert(Widget w, char *s, int len)
#else
void text_insert(w, s, len)
     Widget  w;
     char   *s;
     int     len;
#endif
{
  int	       current;
  XawTextBlock block;

  current = XawTextGetInsertionPoint(w);
  block.ptr = s;
  block.firstPos = 0;
  block.length = len;
  block.format = FMT8BIT;
  text_replace(w, 0, 0, &block);
  if (current == 0)
    XawTextSetInsertionPoint(w, len);
}


/*****************************************************************************
 *                               clear_callback                              *
 *---------------------------------------------------------------------------*
 * Clears the "ascii text" widget.                                           *
 *****************************************************************************/

#if __STDC__
void clear_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void clear_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
  int          last;
  XawTextBlock block;

  last = text_length(ascii_text);
  block.ptr = "";
  block.firstPos = 0;
  block.length = 0;
  block.format = FMT8BIT;
  text_replace(ascii_text, 0, last, &block);
}


/*****************************************************************************
 *                                 clear_proc                                *
 *---------------------------------------------------------------------------*
 * Clears the "ascii_text" widget.                                           *
 *****************************************************************************/

#if __STDC__
void clear_proc(Widget w, XEvent *event, String params, Cardinal *num_params)
#else
void clear_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  clear_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
}


/*****************************************************************************
 *                               clear2_callback                             *
 *---------------------------------------------------------------------------*
 * Clears the "ascii text2" widget (console output).                         *
 *****************************************************************************/

#if __STDC__
void clear2_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void clear2_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
  int          last;
  XawTextBlock block;

  last = text_length(ascii_text2);
  block.ptr = "";
  block.firstPos = 0;
  block.length = 0;
  block.format = FMT8BIT;
  text_replace(ascii_text2, 0, last, &block);
}


/*****************************************************************************
 *                                 clear2_proc                               *
 *---------------------------------------------------------------------------*
 * Clears the "ascii_text2" widget (console output).                         *
 *****************************************************************************/

#if __STDC__
void clear2_proc(Widget w, XEvent *event, String params, Cardinal *num_params)
#else
void clear2_proc(w, event, params, num_params)
     Widget    w;
     XEvent   *event;
     String    params[];
     Cardinal *num_params;
#endif
{
  clear2_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
}


/*****************************************************************************
 *                               console_log                                 *
 *---------------------------------------------------------------------------*
 * Prints the message "s" in the "ascii_text" widget if possible, or on the  *
 * standard error output (stderr).                                           *
 *****************************************************************************/

#if __STDC__
void console_log(char *s)
#else
void console_log(s)
     char *s;
#endif
{
  if (*s == '\0')
    return;
  if (widget_ok)
    {
      text_append(ascii_text, s, strlen(s));
      notify();
    }
  else
    fprintf(stderr, "%s: %s", program_name, s);
}


/*****************************************************************************
 *                                print_motd                                 *
 *---------------------------------------------------------------------------*
 * Prints the contents of the "/etc/motd" file, if available.                *
 *****************************************************************************/

void print_motd()
{
  FILE *motd;
  char  buffer[1025];
  int   l;

  motd = fopen("/etc/motd", "r");
  if (motd != NULL)
    {
      while ((l = fread(buffer, sizeof(char), 1024, motd)) > 0)
        {
          buffer[l] = '\0';
          strip_non_print(buffer);
          console_log(buffer);
        }
      console_log("\n");
      fclose(motd);
    }
}

/* The two following functions (get_pty and redir_console) were taken and    */
/* munged from "xconsole.c".      */
/*                                        XConsole is (C) 1990 X Consortium. */

#if defined(TIOCCONS) || defined(SRIOCSREDIR)

/*****************************************************************************
 *                                  get_pty                                  *
 *---------------------------------------------------------------------------*
 * Tries to allocate a new pty/tty pair; returns 0 if it can or -1 if it     *
 * cannot.  The file descriptors of the pty and the tty are returned into    *
 * the two arguments.                                                        *
 *****************************************************************************/

#if __STDC__
static int get_pty(int *pty, int *tty)
#else
static int get_pty(pty, tty)
     int *pty, *tty;
#endif
{

#if (defined(SVR4) || defined(_SVR4) || defined(__SVR4) || defined(__SVR4__) || defined(__svr4__)) && !defined(sgi) && !defined(__sgi) && !defined(__sgi__)

  *pty = open("/dev/ptmx", O_RDWR);
  if (*pty < 0)
    return -1;
  if (grantpt(*pty) || unlockpt(*pty))
    {
      close(*pty);
      return -1;
    }
  *tty = open(ptsname(*pty), O_RDWR);
  if (*tty < 0)
    {
      close(*pty);
      return -1;
    }
  ioctl(*tty, I_PUSH, "ttcompat");
  return 0;

#else /* (SVR4 || _SVR4 || __SVR4 || __SVR4__ || __svr4__) && ! sgi && ! __sgi && ! __sgi__ */
#if defined(umips) && (defined (SYSTYPE_SYSV) || defined(__SYSTYPE_SYSV) || defined(__SYSTYPE_SYSV__) || defined(_SYSTYPE_SYSV) || defined(___SYSTYPE_SYSV) || defined(___SYSTYPE_SYSV__))

  struct stat fstat_buf;
  char        ttydev[64];

  *pty = open ("/dev/ptc", O_RDWR);
  if (*pty < 0)
    return -1;
  if (fstat (*pty, &fstat_buf) < 0)
    {
      close(*pty);
      return -1;
    }
  sprintf(ttydev, "/dev/ttyq%d", minor(fstat_buf.st_rdev));
  *tty = open(ttydev, O_RDWR);
  if (*tty < 0)
    {
      close(*pty);
      return -1;
    }
  return 0;

#else /* umips && (SYSTYPE_SYSV || __SYSTYPE_SYSV || __SYSTYPE_SYSV__ ||
	 _SYSTYPE_SYSV || ___SYSTYPE_SYSV || ___SYSTYPE_SYSV__) */
#if defined(CRAY) || defined(_CRAY)

  static int index = 0;
  char       ptydev[64], ttydev[64];

  while (index < 256)
    {
      sprintf(ttydev, "/dev/ttyp%03d", index);
      sprintf(ptydev, "/dev/pty/%03d", index);
      if ((*pty = open(ptydev, O_RDWR)) >= 0 &&
	  (*tty = open(ttydev, O_RDWR)) >= 0)
	{
	  index++;
	  return 0;
	}
      if (*pty >= 0)
	close(*pty);
      index++;
    }
  return -1;

#else /* CRAY || _CRAY */
#if defined(sgi) || defined(__sgi) || defined(__sgi__)

  char *slave;

  slave = _getpty(pty, O_RDWR, 0622, 0);
  if (slave == NULL)
    return -1;
  *tty = open(slave, O_RDWR);
  if (*tty >= 0)
    return 0;
  else
    {
      close(*pty);
      return -1;
    }

#else /* sgi || __sgi || __sgi__ */

  char       ptydev[64], ttydev[64];
  static int letter = 0, digit = 0;

#ifndef PTYCHAR1
#if defined(hpux) || defined(__hpux)
#define PTYCHAR1 "zyxwvutsrqp"
#else	/* hpux || __hpux */
#define	PTYCHAR1 "pqrstuvwxyzPQRSTUVWXYZ"
#endif	/* hpux || __hpux*/
#endif	/* PTYCHAR1 */

#ifndef PTYCHAR2
#if defined(hpux) || defined(__hpux)
#define	PTYCHAR2 "fedcba9876543210"
#else	/* hpux || __hpux */
#define	PTYCHAR2 "0123456789abcdef"
#endif	/* !hpux || __hpux */
#endif	/* PTYCHAR2 */

  strcpy(ttydev, "/dev/ttyxx");
  strcpy(ptydev, "/dev/ptyxx");
  while (PTYCHAR1[letter] != '\0')
    {
      ttydev[8] = ptydev[8] = PTYCHAR1[letter];
      while (PTYCHAR2[digit] != '\0')
	{
	  ttydev[9] = ptydev[9] = PTYCHAR2[digit];
	  if ((*pty = open(ptydev, O_RDWR)) >= 0 &&
	      (*tty = open(ttydev, O_RDWR)) >= 0)
	    {
	      digit++;
	      return 0;
	    }
	  if (*pty >= 0)
	    close (*pty);
	  digit++;
	}
      digit = 0;
      letter++;
    }
  return -1;

#endif /* sgi || __sgi || __sgi__ */
#endif /* CRAY || _CRAY */
#endif /* umips && (SYSTYPE_SYSV || __SYSTYPE_SYSV || __SYSTYPE_SYSV__ ||
	 _SYSTYPE_SYSV || ___SYSTYPE_SYSV || ___SYSTYPE_SYSV__) */
#endif /* (SVR4 || _SVR4 || __SVR4 || __SVR4__ || __svr4__) && ! sgi && ! __sgi && ! __sgi__ */
}

#endif /* TIOCCONS || SRIOCSREDIR */


/*****************************************************************************
 *                               redir_console                               *
 *---------------------------------------------------------------------------*
 * Tries to redirect the console output; returns the file descriptor on      *
 * select(2) can be done or -1 if the redirection of the console has failed. *
 *****************************************************************************/

int redir_console()
{

#if !defined(sgi) && !defined(__sgi) && !defined(__sgi__)

  struct stat sbuf;

  if (stat("/dev/console", &sbuf) || (sbuf.st_uid != getuid()))
    return -1;

#endif /* ! sgi && ! __sgi && ! __sgi__ */

  {

#if defined(ultrix) || defined(__ultrix) || defined(__ultrix__)

    return open("/dev/xcons", O_RDONLY);

#else /* ultrix || __ultrix || __ultrix__ */
#ifdef SRIOCSREDIR

    int cons_fd, pty_fd, tty_fd;

    cons_fd = open("/dev/console", O_RDONLY);
    if (cons_fd >= 0)
      {
	if (!get_pty(&pty_fd, &tty_fd) &&
	    (ioctl(fd, SRIOCSREDIR, tty_fd) != -1))
	  return pty_fd;
	else
	  {
	    close(cons_fd);
	    return -1;
	  }
      }
    else
      return -1;
    
#else /* SRIOCSREDIR */
#ifdef TIOCCONS

    int pty_fd, tty_fd, on;
    
    on = 1;
    if (!get_pty(&pty_fd, &tty_fd) &&
	(ioctl(tty_fd, TIOCCONS, (char *) &on) != -1))
      return pty_fd;
    else
      return -1;

#else /* TIOCCONS */
#if defined(SVR4) || defined(_SVR4) || defined(__SVR4) || defined(__SVR4__) || (defined(SYSV) && defined(SYSV386))

    int  tty;
    char ttydev[64];
    
    if (access("/dev/osm", R_OK))
      return -1;
    tty = open("/dev/ptmx", O_RDWR);
    if (tty < 0)
      return -1;
    if (grantpt(tty) || unlockpt(tty))
      {
	close(tty);
	return -1;
      }
    strcpy(ttydev, (char *) ptsname(tty));
#ifdef HAS_VFORK
    osm_child_pid = vfork();
#else
    osm_child_pid = fork();
#endif /* HAS_VFORK */
    if (osm_child_pid == 0)
      {
	int              pty, osm, osm1, nbytes, skip;
	char             cbuf[128];
	struct sigaction action;
	
	if (suid == True)
	  {
	    setgid(getgid());
	    setuid(getuid());
	  }
	action.sa_handler = SIG_DFL;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 /* >>> SA_NOCLDSTOP <<< */;
	if (sigaction(SIGCHLD, &action, (struct sigaction *)NULL) < 0)
	  exit(1);
	if (sigaction(SIGHUP, &action, (struct sigaction *)NULL) < 0)
	  exit(1);
	skip = 0;
	osm1 = open("/dev/osm1", O_RDONLY);
	if (osm1 >= 0)
	  {
	    while ((nbytes = read(osm1, cbuf, sizeof(cbuf))) > 0)
	      skip += nbytes;
	    close(osm1);
	  }
	pty = open(ttydev, O_RDWR);
	if (pty < 0)
	  {
	    close(tty);
	    exit(1);
	  }
	osm = open("/dev/osm", O_RDONLY);
	if (osm < 0)
	  {
	    close(pty);
	    close(tty);
	    exit(1);
	  }
	nbytes = 0;
	while (skip > 0 && nbytes >= 0)
	  {
	    nbytes = skip;
	    if (nbytes > sizeof(cbuf))
	      nbytes = sizeof(cbuf);
	    nbytes = read(osm, cbuf, nbytes);
	    skip -= nbytes;
	  }
	while ((nbytes = read(osm, cbuf, sizeof(cbuf))) >= 0)
	  write(pty, cbuf, nbytes);
	close(osm);
	close(pty);
	close(tty);
	exit(0);
      }
    if (osm_child_pid < 0)
      {
	close(tty);
	return -1;
      }
    return tty;
    
#else /* SVR4 || _SVR4 || __SVR4 || __SVR4__ || (SYSV && SYSV386) */
    
    return -1;

#endif /* SVR4 || _SVR4 || __SVR4 || __SVR4__ || (SYSV && SYSV386) */
#endif /* TIOCCONS */
#endif /* SRIOCSREDIR */
#endif /* ultrix || __ultrix || __ultrix__ */
  }
}


/*****************************************************************************
 *                            input_ready_callback                           *
 *---------------------------------------------------------------------------*
 * Prints the text coming from "input"; also calls the "grimreaper" function *
 * function if a child process has died (hack, hack...).                     *
 *****************************************************************************/

#if __STDC__
void input_ready_callback(XtPointer client_data, int *input, XtInputId *input_id)
#else
void input_ready_callback(client_data, input, input_id)
     XtPointer  client_data;
     int       *input;
     XtInputId *input_id;
#endif
{
  char buffer[1025];
  int  n;

  if (*input_id != console_id)
    {
      while (child_died)  /* Don't change this ! */
	grimreaper();
      if (hangup_received)
	{
	  hangup_received = False;
	  if (! closing_top_shell)
	    {
	      closing_top_shell = True;
	      quit_callback(top_shell, (XtPointer) NULL, (XtPointer) NULL);
	      closing_top_shell = False;
	    }
	}
    }
  n = read(*input, buffer, 1024);
  if (n > 0)
    {
      buffer[n] = '\0';
      strip_non_print(buffer);
      if (widget_ok)
	{
	  if (resources.split_window && (*input_id == console_id))
	    text_append(ascii_text2, buffer, strlen(buffer));
	  else
	    text_append(ascii_text, buffer, strlen(buffer));
	  notify();
	}
      else
	fprintf(stderr, "%s: %s", program_name, buffer);
    }
}
