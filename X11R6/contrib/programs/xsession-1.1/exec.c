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

#if ((defined(sun) || defined(__sun)) && (defined(__SVR4) || defined(__svr4__))) || (defined(_IBMR2) && defined(_AIX32))
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#endif

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xmu/Xmu.h>
#include <X11/Xmu/SysUtil.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <ctype.h>
#include <signal.h>
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
extern char *zmalloc(unsigned size);
extern void console_log(char *s);
#else
extern char *zmalloc();
extern void console_log();
#endif

#if (defined(sun) || defined(__sun)) && defined(__SVR4) && defined(BSD) && !defined(__STDC__)
extern char *tempnam();   /* forgotten prototype when using SUNWspro C compiler
			     from Solaris 2.x in BSD compatibility mode */
#endif

extern void activate_timeout();
extern void desactivate_timeout();

extern char             **environ;
extern Widget             top_shell, ascii_text2, clear2_menu_item;
extern int                pipe_fd[2];
extern resources_rec      resources;
extern XtInputId          console_id;

wm_info_rec              *wm_info;
app_info_rec             *app_info;
Boolean                   child_died, hangup_received;
pid_t                     osm_child_pid;

static pid_t              child_pids[MAX_PROGS];
static char              *child_names[MAX_PROGS];
static int                current_wm = NONE;
static char              *current_tmp_file;
static char              *current_cfg_file;
static char              *next_wm_command;
static char              *next_env_string;


/*****************************************************************************
 *                                extract_name                               *
 *---------------------------------------------------------------------------*
 * Strips the path and arguments from a command string, in order to return   *
 * the command name alone.                                                   *
 *****************************************************************************/

#if __STDC__
static char *extract_name(char *cmd)
#else
static char *extract_name(cmd)
     char *cmd;
#endif
{
  char *s, *name;

  name = (char *) zmalloc((strlen(cmd) + 1) * sizeof(char));

  for (s = strcpy(name, cmd); *s != '\0'; s++)
    if (isspace(*s))
      *s-- = '\0';

  if ((s = strrchr(name, '/')) == NULL)
    return name;
  else
    return s++;
}


/*****************************************************************************
 *                                is_readable                                *
 *---------------------------------------------------------------------------*
 * Returns True if the file "filename" can be opened for reading.            *
 *****************************************************************************/

#if __STDC__
static Boolean is_readable(char *filename)
#else
static Boolean is_readable(filename)
     char *filename;
#endif
{
  FILE *f;

  f = fopen(filename, "r");
  fclose(f);
  return (f != NULL);
}


/* Code taken and munged from "parse.c" (in the "ctwm" sources), which was   */
/* itself taken and munged from "xrdb.c".                                    */
/*                                     CTWM is (C) 1992 Claude Lecommandeur, */
/*                     XRDB is (C) 1987, 1991 Digital Equipment Corporation. */

#define MAXHOSTNAME 255
#define Resolution(pixels, mm)  ((((pixels) * 100000 / (mm)) + 50) / 100)


/*****************************************************************************
 *                         write_preprocessor_defs                           *
 *---------------------------------------------------------------------------*
 * Writes the preprocessor macros in a temporary file.                       *
 *---------------------------------------------------------------------------*
 * The following macros are defined :                                        *
 *                                                                           *
 * XSESSION_VERSION  The version number of XSession.                         *
 * XSESSION_PID  The process number of XSession.  Useful to send signals.    *
 * SERVERHOST    The name of the machine that is running the X server.       *
 * CLIENTHOST    The name of the machine that is running xsession.           *
 * HOSTNAME      Fully-qualified version of CLIENTHOST.                      *
 * MACHINE       The machine type of CLIENTHOST (e.g. sun4m, mips, i486 ...) *
 * USER          The name of the user running the program.                   *
 * HOME          The user's home directory.                                  *
 * VERSION       The X major protocol version.                               *
 * REVISION      The X minor protocol revision.                              *
 * VENDOR        The vendor of your X server.                                *
 * RELEASE       The release number of your X server.                        *
 * WIDTH         The width of your display in pixels.                        *
 * HEIGHT        The height of your display in pixels.                       *
 * X_RESOLUTION  The X resolution of your display in pixels per meter.       *
 * Y_RESOLUTION  The Y resolution of your display in pixels per meter.       *
 * PLANES        The number of bit planes in the default root window.        *
 * BITS_PER_RGB  The number of significant bits in an RGB color.             *
 * CLASS         Your visual class. (e.g. StaticGray, PseudoColor...).       *
 * COLOR         May be `Yes' or `No' if you use "m4", or nothing for "cpp". *
 * WM_NAME       The name of the current window manager.                     *
 * TWM_TYPE      Same as WM_NAME.  For compatibility with "ctwm".            *
 *****************************************************************************/

#if __STDC__
static Boolean write_preprocessor_defs(char *wm_cmd, char *wm_cfg)
#else
static Boolean write_preprocessor_defs(wm_cmd, wm_cfg)
     char *wm_cmd;
     char *wm_cfg;
#endif
{
  Display        *disp;
  Screen         *screen;
  Visual         *visual;
  char            client[MAXHOSTNAME];
  char            server[MAXHOSTNAME];
  char           *s, *wm_config, *abs_wm_config, *p;
  struct hostent *host_info;
  struct utsname  arch_info;
  FILE           *tmp_file;
  int             i;
  static char     m4_definestr[]   = "define(%s, [%s])\n";
  static char     m4_definenum[]   = "define(%s, %d)\n";
  static char     m4_includestr[]  = "sinclude(%s)\n";
  static char     cpp_definestr[]  = "#define %s\t%s\n";
  static char     cpp_definenum[]  = "#define %s\t%d\n";
  static char     cpp_includestr[] = "#include \"%s\"\n";
  char           *definestr, *definenum, *includestr;

  if (! strcmp(resources.preprocessor, "m4"))
    {
      definestr = m4_definestr;
      definenum = m4_definenum;
      includestr = m4_includestr;
    }
  else
    {
      definestr = cpp_definestr;
      definenum = cpp_definenum;
      includestr = cpp_includestr;
    }

  if (wm_cfg == NULL)  /* No preprocessor-specific configuration file. */
    return False;

  /* Add the `m4' or `cpp' extension to wm_cfg. */
  wm_config = (char *) zmalloc((strlen(wm_cfg) + 4) * sizeof(char));
  sprintf(wm_config, wm_cfg, resources.preprocessor);

  /* Check if the configuration file exists and is readable */
  if (! is_readable(wm_config))
    {
      if (!resources.quiet)
	{
	  s = (char *) zmalloc(100 * sizeof(char));
	  sprintf(s, "Warning: %s configuration file not found (%s).  Using default file instead.\n",
		  extract_name(wm_cmd), wm_config);
	  console_log(s);
	  free(s);
	}
      free(wm_config);
      return False;
    }

  /* Create a temporary file that will include the configuration file */
  current_tmp_file = tempnam((char *) NULL, "xs_wm");
  if (current_tmp_file == NULL)
    {
      console_log("Error : Can't create temporary file name.\n");
      free(wm_config);
      return False;
    }
  tmp_file = fopen(current_tmp_file, "w+");
  if (tmp_file == NULL)
    {
      console_log("Error : Can't create temporary file.\n");
      free(wm_config);
      return False;
    }

  /* Now that we have the file, put all macros in it */
  if (! strcmp(resources.preprocessor, "m4"))
    fprintf(tmp_file, "changequote([,])\n");
  fprintf(tmp_file, definestr, "XSESSION_VERSION", VERSION);
  fprintf(tmp_file, definenum, "XSESSION_PID", getpid());
  disp = XtDisplay(top_shell);
  XmuGetHostname(client, MAXHOSTNAME);
  host_info = gethostbyname(client);
  strcpy(server, DisplayString(disp));
  s = strchr(server, ':');
  if (s != NULL)
    *s = '\0';
  if ((server[0] == '\0') || (! strcmp(server, "unix"))
      || (! strcmp(server, "sun")))
    strcpy(server, client);
  fprintf(tmp_file, definestr, "SERVERHOST", server);
  fprintf(tmp_file, definestr, "CLIENTHOST", client);
  if (host_info != NULL)
    fprintf(tmp_file, definestr, "HOSTNAME", host_info->h_name);
  else
    fprintf(tmp_file, definestr, "HOSTNAME", client);
  uname(&arch_info);
  fprintf(tmp_file, definestr, "MACHINE", arch_info.machine);
  fprintf(tmp_file, definestr, "USER", getenv("USER"));
  fprintf(tmp_file, definestr, "HOME", getenv("HOME"));
  fprintf(tmp_file, definenum, "VERSION", ProtocolVersion(disp));
  fprintf(tmp_file, definenum, "REVISION", ProtocolRevision(disp));
  fprintf(tmp_file, definestr, "VENDOR", ServerVendor(disp));
  fprintf(tmp_file, definenum, "RELEASE", VendorRelease(disp));
  screen = DefaultScreenOfDisplay(disp);
  visual = DefaultVisualOfScreen(screen);
  fprintf(tmp_file, definenum, "WIDTH", screen->width);
  fprintf(tmp_file, definenum, "HEIGHT", screen->height);
  fprintf(tmp_file, definenum, "X_RESOLUTION",
	  Resolution(screen->width, screen->mwidth));
  fprintf(tmp_file, definenum, "Y_RESOLUTION",
	  Resolution(screen->height, screen->mheight));
  fprintf(tmp_file, definenum, "PLANES",
	  DisplayPlanes(disp, DefaultScreen(disp)));
  fprintf(tmp_file, definenum, "BITS_PER_RGB", visual->bits_per_rgb);
  switch(visual->class)
    {
      case(StaticGray) :
	fprintf(tmp_file, definestr, "CLASS", "StaticGray");
        break;
      case(GrayScale) :
	fprintf(tmp_file, definestr, "CLASS", "GrayScale");
        break;
      case(StaticColor) :
	fprintf(tmp_file, definestr, "CLASS", "StaticColor");
        break;
      case(PseudoColor) :
	fprintf(tmp_file, definestr, "CLASS", "PseudoColor");
        break;
      case(TrueColor) :
        fprintf(tmp_file, definestr, "CLASS", "TrueColor");
        break;
      case(DirectColor) :
	fprintf(tmp_file, definestr, "CLASS", "DirectColor");
        break;
      default :
	fprintf(tmp_file, definestr, "CLASS", "NonStandard");
        break;
    }
  if (visual->class == StaticColor || visual->class == PseudoColor || visual->class == TrueColor || visual->class == DirectColor)
    fprintf(tmp_file, definestr, "COLOR", "Yes");
  else
    if (! strcmp(resources.preprocessor, "m4"))
      fprintf(tmp_file, definestr, "COLOR", "No");
  s = extract_name(wm_cmd);
  fprintf(tmp_file, definestr, "WM_NAME", s);
  fprintf(tmp_file, definestr, "TWM_TYPE", s); /* Compatibility with "ctwm" */
  /* >>> free(s); ? <<< */
  for (i = 0; i < resources.num_defines; i++)
    fprintf(tmp_file, definestr, resources.defines_sym[i], resources.defines_val[i]);
  fprintf(tmp_file, "\n");
  /* Include the common configuration file */
  if (! strcmp(resources.preprocessor, "cpp") && (p = getenv("HOME")) != NULL)
    {
      abs_wm_config = (char *) zmalloc((strlen(wm_config) + strlen(p) + 2)
				       * sizeof(char));
      sprintf(abs_wm_config, "%s/%s", p, wm_config);
      fprintf(tmp_file, includestr, abs_wm_config);
      free(abs_wm_config);
    }
  else
    fprintf(tmp_file, includestr, wm_config);
  fclose(tmp_file);
  free(wm_config);
  return True;
}


/*****************************************************************************
 *                                  execsh                                   *
 *---------------------------------------------------------------------------*
 * Executes the command "cmd" (command name + arguments) and outputs the     *
 * results in "output_fd".  This function never returns.                     *
 *---------------------------------------------------------------------------*
 * This function is used instead of the various exec* functions.  I wrote    *
 * this piece of code because it has several advantages:                     *
 * - you can execute a shell script even if it doesn't start with the usual  *
 *   comment "#!/bin/sh".                                                    *
 * - the current value of the PATH variable is used to look for the command. *
 * - the new process is a direct child of xsession, so its return code can   *
 *   be examined.                                                            *
 *****************************************************************************/

#if __STDC__
static void execsh(char *cmd, int output_fd, int err_fd)  /* Never returns */
#else
static void execsh(cmd, output_fd, err_fd)  /* Never returns */
     char *cmd;
     int   output_fd;
     int   err_fd;
#endif
{
  char *tmpargv[4];
  char *s;
  int   rc;

  s = (char *) zmalloc((strlen(cmd) + 6) * sizeof(char));
  dup2(err_fd, fileno(stderr));
  dup2(output_fd, fileno(stdout));
  strcpy(s, "exec ");
  tmpargv[0] = "/bin/sh";
  tmpargv[1] = "-c";                    /* "-ce" ? */
  tmpargv[2] = (char *)strcat(s, cmd);
  tmpargv[3] = NULL;
  rc = execve(tmpargv[0], tmpargv, environ);
  console_log("Error: execve failed.  Cannot start application.\n");
  _exit(rc);
}


/*****************************************************************************
 *                                start_wm                                   *
 *---------------------------------------------------------------------------*
 * Starts a new window manager.  If the WM's configuration file has to be    *
 * pre-processed, this function only starts the preprocessor.  The actual WM *
 * will be started when the preprocessor dies (see the "grimreaper"          *
 * function).                                                                *
 *****************************************************************************/

#if __STDC__
static void start_wm(int wm_id)
#else
static void start_wm(wm_id)
     int wm_id;
#endif
{
  char           *s, *cmd, *cpp_string;
  struct utsname  arch_info;
  int             n, output_fd;

  if (wm_id != NONE)
    {
      desactivate_timeout();
      cmd = wm_info[wm_id].command;
      if ((cmd == NULL) || (cmd[0] == '\0'))
	{
	  console_log("Error: invalid command string\n");
	  current_wm = NONE;
          activate_timeout();
	  return;
	}
      current_wm = wm_id;   /* New window manager */
      output_fd = pipe_fd[1];   /* WM's output goes to XSession's console */
      next_wm_command = NULL;
      next_env_string = NULL;
      if (write_preprocessor_defs(cmd, wm_info[wm_id].config_file))
	{
	  FILE *cfg_file;

	  /* >>> free(current_cfg_file); ? <<< */
	  current_cfg_file = tempnam((char *) NULL, "xs_WM");
	  if (current_cfg_file == NULL)
	    {
	      console_log("Error : Can't create configuration file name.\n");
	      current_wm = NONE;
	      activate_timeout();
	      return; /* >>> continue with default file ? <<< */
	    }
	  cfg_file = fopen(current_cfg_file, "w+");
	  if (cfg_file == NULL)
	    {
	      console_log("Error : Can't create configuration file.\n");
	      current_wm = NONE;
	      activate_timeout();
	      return; /* >>> continue with default file ? <<< */
	    }
	  n = strlen(cmd) + 1;
	  if (wm_info[wm_id].cmdline_opt != NULL)   /* Command line parameter -> config file name */
	    {
	      n += strlen(wm_info[wm_id].cmdline_opt) + 1;
	      s = (char *) zmalloc(n * sizeof(char));
	      strcpy(s, cmd);
	      strcat(s, " ");
	      strcat(s, wm_info[wm_id].cmdline_opt);
	      n += strlen(current_cfg_file);
	      next_wm_command = (char *) zmalloc(n * sizeof(char));
	      sprintf(next_wm_command, s, current_cfg_file);   /* Keep the command + parameter(s) */
	      free(s);
	    }
	  else
	    {
	      next_wm_command = (char *) zmalloc(n * sizeof(char));
	      strcpy(next_wm_command, cmd);  /* Keep the command for "grimreaper" */
	    }
	  if (wm_info[wm_id].env_opt != NULL)   /* Environment string -> config file name */
	    {
	      n = strlen(wm_info[wm_id].env_opt) + strlen(current_cfg_file)
		+ 1;
	      s = (char *) zmalloc(n * sizeof(char));
	      sprintf(s, wm_info[wm_id].env_opt, current_cfg_file);
	      next_env_string = s;   /* Keep it for "grimreaper" */
	    }
	  cpp_string = (char *) zmalloc((strlen(CPP_STRING) + 1)
					* sizeof(char));
	  sprintf(cpp_string, CPP_STRING);
	  s = (char *) zmalloc((strlen(current_tmp_file) + strlen(cpp_string)
				+ 5) * sizeof(char));
	  if (! strcmp(resources.preprocessor, "m4"))
	    sprintf(s, "m4 %s", current_tmp_file);
	  else
	    sprintf(s, "%s -P %s", cpp_string, current_tmp_file);
	  free(cpp_string);
	  output_fd = fileno(cfg_file);   /* Output from the preprocessor goes to the new file */
	  cmd = s;
	}
#ifdef SOUNDS
      play_sound(resources.sound_start_wm);
#endif /* SOUNDS */
#ifdef HAS_VFORK
      child_pids[0] = vfork();
#else
      child_pids[0] = fork();
#endif /* HAS_VFORK */
      /* Start the window manager or the preprocessor */
      if (child_pids[0] == 0)
	execsh(cmd, output_fd, pipe_fd[1]); 
      if (child_pids[0] < 0)
	{
	  console_log("Error: cannot fork\n");
	  current_wm = NONE;
          activate_timeout();
	  return;
	}
      if (!resources.quiet)
	{
	  s = (char *) zmalloc((100 + strlen(wm_info[wm_id].full_name)
				+ strlen(wm_info[wm_id].short_name)
				+ strlen(DisplayString(XtDisplay(top_shell))))
			       * sizeof(char));
	  uname(&arch_info);
	  sprintf(s, "\n%s (%s) is running on %s (%s) to display %s\n\n",
		  wm_info[wm_id].full_name, wm_info[wm_id].short_name,
		  arch_info.nodename, arch_info.machine,
		  DisplayString(XtDisplay(top_shell)));
	  console_log(s);
	  free(s);
	}
    }
}


/*****************************************************************************
 *                               child_dies                                  *
 *---------------------------------------------------------------------------*
 * This is the SIGCHLD handler.  This function is called when a child        *
 * process has died.  See also the "grimreaper" function.                    *
 *****************************************************************************/

static void child_dies()
{
  /* This little hack ensures that input_ready_callback() is called at least
   * once when a child process dies.  This function will then call
   * grimreaper() which will check the status of the child, etc.  If the
   * processing of child processes had been done here, some of them could
   * have been lost.
   */
  write(pipe_fd[1], "", 1);
  child_died = True;
}


/*****************************************************************************
 *                                 hangup                                    *
 *---------------------------------------------------------------------------*
 * This is the SIGHUP handler.  This function is called when such a signal   *
 * is received.                                                              *
 *****************************************************************************/

static void hangup()
{
  /* This little hack ensures that input_ready_callback() is called at least
   * once when the signal is received.  This function will then terminate the
   * program by calling the `quit' callback.  If this call had been done here,
   * some of the signals could have been lost.  (Yes, we don't assume here
   * that only one SIGHUP will be sent by the user, even if this should always
   * be true!)
   */
  write(pipe_fd[1], "", 1);
  hangup_received = True;
}


/*****************************************************************************
 *                               grimreaper                                  *
 *---------------------------------------------------------------------------*
 * This function notifies the user when a child process has died.  If this   *
 * process was a preprocessor running on the window manager's configuration  *
 * file, then the window manager is launched with the new file.              *
 *****************************************************************************/

void grimreaper()
{
  pid_t       pid;
#ifdef OLD_WAITPID
  union wait  status;
#else
  int         status;
#endif /* OLD_WAITPID */
  char       *s;
  int         i;
  char       *cmd;

  child_died = False;
  pid = waitpid(-1, &status, WNOHANG);
  if (pid <= 0)
    return;
  else
    child_died = True;

  for (i = 0; i < MAX_PROGS; i++)
    if (child_pids[i] == pid)
      break;
  if (i == 0)   /* The window manager (or the preprocessor) has ended. */
    {
      if (next_wm_command != NULL)
	{
	  if (current_tmp_file != NULL)
	    {
	      if (! resources.keep_tmp_file)
		unlink(current_tmp_file);
	      current_tmp_file = NULL;
	    }
	  cmd = next_wm_command;
	  next_wm_command = NULL;
#ifdef HAS_VFORK
	  child_pids[0] = vfork();
#else
	  child_pids[0] = fork();
#endif /* HAS_VFORK */
	  if (child_pids[0] == 0)
	    {
	      if (next_env_string != NULL)
		putenv(next_env_string);
	      execsh(cmd, pipe_fd[1], pipe_fd[1]);
	    }
	  if (child_pids[0] < 0)
	    {
	      console_log("Error: cannot fork\n");
	      current_wm = NONE;
	      activate_timeout();
	      return;
	    }
	  return;
	}
      else
	{
	  if (current_cfg_file != NULL)
	    {
	      if (! resources.keep_tmp_file)
		unlink(current_cfg_file);
	      current_cfg_file = NULL;
	    }
	}
#ifdef SOUNDS
      play_sound(resources.sound_end_wm);
#endif /* SOUNDS */
      if (!resources.quiet)
	{
	  s = (char *) zmalloc((100 + strlen(wm_info[current_wm].short_name))
			       * sizeof(char));
	  if (WIFSIGNALED(status))
	    {
	      sprintf(s, "%s has been killed by a signal %d.\n",
		      wm_info[current_wm].short_name, WTERMSIG(status));
	    }
	  else
	    {
	      if (WEXITSTATUS(status) != 0)
		sprintf(s, "\n%s has ended (return code = %d)\n\n",
			wm_info[current_wm].short_name, WEXITSTATUS(status));
	      else
		sprintf(s, "\n%s has ended\n\n", wm_info[current_wm].short_name);
	    }
	  console_log(s);
	  free(s);
	}
      current_wm = NONE;
      activate_timeout();
    }
  else
    if (i != MAX_PROGS)
      {
	if (child_names[i] != NULL)
	  {
#ifdef SOUNDS
	    play_sound(resources.sound_end_app);
#endif /* SOUNDS */
	    if (!resources.quiet)
	      {
		s = (char *) zmalloc((strlen(child_names[i]) + 100)
				     * sizeof(char));
		if (WIFSIGNALED(status))
		  {
		    sprintf(s,
			    "\nProcess %d (%s) has been killed by a signal %d.\n\n",
			    pid, child_names[i], WTERMSIG(status));
		  }
		else
		  {
		    if (WEXITSTATUS(status) != 0)
		      sprintf(s,
			      "\nProcess %d (%s) has ended (return code = %d)\n\n",
			      pid, child_names[i], WEXITSTATUS(status));
		    else
		      sprintf(s, "\nProcess %d (%s) has ended\n\n", pid,
			      child_names[i]);
		  }
		console_log(s);
		free(s);
	      }
	  }
      }
    else
      {
	if (pid == osm_child_pid)
	  {
	    osm_child_pid = 0;
	    if (WIFSIGNALED(status))
	      if (WTERMSIG(status) != SIGTERM)
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
	    if (WEXITSTATUS(status) == 1)
	      {
		if (!resources.quiet)
		  console_log("Warning: cannot redirect console messages.\n");
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
	  }
      }
  if (i != MAX_PROGS)
    {
      child_pids[i] = 0;
      child_names[i] = NULL;
    }
}


/*****************************************************************************
 *                             end_current_wm                                *
 *---------------------------------------------------------------------------*
 * Kills the current window manager and waits until it has really ended.     *
 *****************************************************************************/

void end_current_wm()
{
  if (current_wm != NONE)
    if (! kill(child_pids[0], SIGTERM))
      while (current_wm != NONE)
	{
	  while (child_died == True)
	    grimreaper();
	  sleep(1);
	}
}


/*****************************************************************************
 *                                launch_prog                                *
 *---------------------------------------------------------------------------*
 * Starts a new program ("cmd") and prints a message.                        *
 *****************************************************************************/

#if __STDC__
static void launch_prog(char *cmd, char *name, char *short_name)
#else
static void launch_prog(cmd, name, short_name)
     char *cmd, *name, *short_name;
#endif
{
  char           *s;
  int             i;

  if ((cmd == NULL) || (cmd[0] == '\0'))
    {
      console_log("Error: invalid command string\n");
      return;
    }
  for (i = 1; i < MAX_PROGS; i++)
    if (child_pids[i] == 0)
      break;
  if ((child_pids[i] == 0) && (i != MAX_PROGS))
#ifdef HAS_VFORK
    child_pids[i] = vfork();
#else
    child_pids[i] = fork();
#endif /* HAS_VFORK */
  else
    console_log("Error: Internal process table is full\n");
  child_names[i] = short_name;
  if (child_pids[i] == 0)
    execsh(cmd, pipe_fd[1], pipe_fd[1]);
  if (child_pids[i] < 0)
    {
      console_log("Error: cannot fork\n");
      return;
    }
  if ((name != NULL) && (name[0] != '\0') && !resources.quiet)
    {
      s = (char *) zmalloc((strlen(name) + 30) * sizeof(char));
      sprintf(s, "\nStarting %s (pid = %d)\n\n", name, child_pids[i]);
      console_log(s);
      free(s);
    }
}


/*****************************************************************************
 *                               wm_menu_callback                            *
 *---------------------------------------------------------------------------*
 * Terminates the current window manager (if any) and starts a new one.      *
 *****************************************************************************/

#if __STDC__
void wm_menu_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void wm_menu_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
  end_current_wm();
  start_wm((int) client_data);
}


/*****************************************************************************
 *                              app_menu_callback                            *
 *---------------------------------------------------------------------------*
 * Starts a new program (form the "applications" menu).                      *
 *****************************************************************************/

#if __STDC__
void app_menu_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void app_menu_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
#ifdef SOUNDS
  play_sound(resources.sound_start_app);
#endif /* SOUNDS */
  launch_prog(app_info[(int) client_data].command,
	      app_info[(int) client_data].full_name,
	      app_info[(int) client_data].short_name);
}


/*****************************************************************************
 *                                lock_callback                              *
 *---------------------------------------------------------------------------*
 * Starts the display locker (from the "session" menu).                      *
 *****************************************************************************/

#if __STDC__
void lock_callback(Widget w, XtPointer client_data, XtPointer call_data)
#else
void lock_callback(w, client_data, call_data)
     Widget    w;
     XtPointer client_data, call_data;
#endif
{
#ifdef SOUNDS
  play_sound(resources.sound_lock);
#endif /* SOUNDS */
  launch_prog(resources.lock_command, (char *) NULL, (char *) NULL);
}


/*****************************************************************************
 *                                get_first_item                             *
 *---------------------------------------------------------------------------*
 * Returns a pointer to the first item in a comma-separated list ("s").      *
 *****************************************************************************/

#if __STDC__
char *get_first_item(char *s)
#else
char *get_first_item(s)
     char *s;
#endif
{
  char *p;

  p = strtok(s, ",");
  if (p != NULL)
    while (isspace(*p))
      p++;
  return p;
}


/*****************************************************************************
 *                                get_next_item                              *
 *---------------------------------------------------------------------------*
 * Returns a pointer to the next item in the current list.                   *
 *****************************************************************************/

char *get_next_item()
{
  char *p;
  
  p = strtok((char *) NULL, ",");
  if (p != NULL)
    while (isspace(*p))
      p++;
  return p;
}


/*****************************************************************************
 *                              get_wm_resources                             *
 *---------------------------------------------------------------------------*
 * Retreives the resources for all window managers (for the "window          *
 * managers" menu).                                                          *
 *****************************************************************************/

void get_wm_resources()
{
  int         i;
  char       *p, *s;
  XtResource *wm_resources_desc;
  String     *wm_resources;

  resources.wm_count = 1;
  p = resources.wm_names;
  while ((p = strchr(p, ',')) != NULL)
    {
      resources.wm_count++;
      p++;
    }
  wm_info = (wm_info_rec *) zmalloc(resources.wm_count
				    * sizeof(wm_info_rec));
  wm_resources_desc = (XtResource *) zmalloc(resources.wm_count
					     * sizeof(XtResource));
  wm_resources = (String *) zmalloc(resources.wm_count * sizeof(String));
  for (i = 0; i < resources.wm_count; i ++)
    {
      if (i == 0)
	p = get_first_item(resources.wm_names);
      else
	p = get_next_item();
      wm_info[i].short_name = p;
      wm_info[i].full_name = "<Unknown>";
      wm_info[i].command = "/bin/echo Error : Command string not defined";
      wm_info[i].config_file = NULL;
      wm_info[i].cmdline_opt = NULL;
      wm_info[i].env_opt = NULL;
      s = (char *) zmalloc(4 + strlen(p));
      strcpy(s, "wm_");
      strcat(s, p);
      wm_resources_desc[i].resource_name = s;
      s = (char *) zmalloc(4 + strlen(p));
      strcpy(s, "WM_");
      strcat(s, p);
      wm_resources_desc[i].resource_class = s;
      wm_resources_desc[i].resource_type = XtRString;
      wm_resources_desc[i].resource_size = sizeof(String);
      wm_resources_desc[i].resource_offset = i * sizeof(String);
      wm_resources_desc[i].default_type = XtRString;
      wm_resources_desc[i].default_addr = NULL;
    }
  XtGetApplicationResources(top_shell, (XtPointer) wm_resources,
			    wm_resources_desc, resources.wm_count,
			    NULL, ZERO);
  for (i = 0; i < resources.wm_count; i ++)
    {
      p = get_first_item(wm_resources[i]);
      if (p == NULL)
	continue;
      wm_info[i].full_name = p;
      p = get_next_item();
      if (p == NULL)
	continue;
      wm_info[i].command = p;
      p = get_next_item();
      if (p == NULL)
	continue;
      wm_info[i].config_file = p;
      p = get_next_item();
      if (p == NULL)
	continue;
      wm_info[i].cmdline_opt = p;
      p = get_next_item();
      if (p == NULL)
	continue;
      wm_info[i].env_opt = p;
    }
  free(wm_resources_desc);
  free(wm_resources);
  current_wm = NONE;
}


/*****************************************************************************
 *                              get_app_resources                            *
 *---------------------------------------------------------------------------*
 * Retreives the resources for all applications (for the "applications"      *
 * menu).                                                                    *
 *****************************************************************************/

void get_app_resources()
{
  int         i;
  char       *p, *s;
  XtResource *app_resources_desc;
  String     *app_resources;

  resources.app_count = 1;
  p = resources.app_names;
  while ((p = strchr(p, ',')) != NULL)
    {
      resources.app_count++;
      p++;
    }
  app_info = (app_info_rec *) zmalloc(resources.app_count
				      * sizeof(app_info_rec));
  app_resources_desc = (XtResource *) zmalloc(resources.app_count
					      * sizeof(XtResource));
  app_resources = (String *) zmalloc(resources.app_count * sizeof(String));
  for (i = 0; i < resources.app_count; i ++)
    {
      if (i == 0)
	p = get_first_item(resources.app_names);
      else
	p = get_next_item();
      app_info[i].short_name = p;
      app_info[i].full_name = "<Unknown>";
      app_info[i].command = "/bin/echo Error : Command string not defined";
      s = (char *) zmalloc(4 + strlen(p));
      strcpy(s, "app_");
      strcat(s, p);
      app_resources_desc[i].resource_name = s;
      s = (char *) zmalloc(4 + strlen(p));
      strcpy(s, "App_");
      strcat(s, p);
      app_resources_desc[i].resource_class = s;
      app_resources_desc[i].resource_type = XtRString;
      app_resources_desc[i].resource_size = sizeof(String);
      app_resources_desc[i].resource_offset = i * sizeof(String);
      app_resources_desc[i].default_type = XtRString;
      app_resources_desc[i].default_addr = NULL;
    }
  XtGetApplicationResources(top_shell, (XtPointer) app_resources,
			    app_resources_desc, resources.app_count,
			    NULL, ZERO);
  for (i = 0; i < resources.app_count; i ++)
    {
      p = get_first_item(app_resources[i]);
      if (p == NULL)
	continue;
      app_info[i].full_name = p;
      p = get_next_item();
      if (p == NULL)
	continue;
      app_info[i].command = p;
    }
  free(app_resources_desc);
  free(app_resources);
}


/*****************************************************************************
 *                                   startup                                 *
 *---------------------------------------------------------------------------*
 * Starts the default window manager and executes the .X11Startup script.    *
 *****************************************************************************/

void startup()
{
  int               i;
  int               wm_found;
  char             *s, *p;
  struct sigaction  action;
  
  for (i = 0; i < MAX_PROGS; i++)
    child_pids[i] = 0;
  child_died = False;
  hangup_received = False;
  /* change the signal handler for SIGCHLD */
  action.sa_handler = child_dies;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0 /* >>> SA_NOCLDSTOP <<< */;
  if (sigaction(SIGCHLD, &action, (struct sigaction *)NULL) < 0)
    console_log("Error: could not set signal handler for SIGCHLD!\n");
  /* change the signal handler for SIGHUP */
  action.sa_handler = hangup;
  if (sigaction(SIGHUP, &action, (struct sigaction *)NULL) < 0)
    console_log("Error: could not set signal handler for SIGHUP!\n");
  /* Start window manager */
  wm_found = NONE;
  for (i = 0; (wm_found == NONE) && (i < resources.wm_count); i++)
    if (! XmuCompareISOLatin1(resources.default_wm, wm_info[i].short_name))
      wm_found = i;
  if (wm_found != NONE)
    start_wm(wm_found);
  else
    {
      s = (char *) zmalloc((strlen(resources.default_wm) + 30)
			   * sizeof(char));
      sprintf(s, "Unknown window manager: %s.\n", resources.default_wm);
      console_log(s);
      free(s);
    }
  /* Execute .X11Startup script */
  if (resources.startup)
    {
      p = getenv("HOME");
      if (p == NULL)
	p = ".";
      s = (char *) zmalloc((strlen(p) + 30) * sizeof(char));
      strcpy(s, p);
      strcat(s, "/.X11Startup");
      launch_prog(s, (char*) NULL, (char *) NULL);
      free(s);
    }
}
