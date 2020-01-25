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

#define VERSION                "1.1"

#define ERR_SYNTAX             -1
#define ERR_BAD_LIST           -2
#define ERR_MEMORY             -3
#define ERR_PREPROCESSOR       -4
#define ERR_SUID               -5
#define ERR_ROOT               -6

#define NONE                   -1
#define MAX_PROGS              64

typedef struct _resources_rec
  {
    Boolean   notify;
    Boolean   motd;
    Boolean   remote;
    Boolean   keep_tmp_file;
    int       wm_count;
    String    wm_names;
    String    default_wm;
    Boolean   allow_bell;
    Boolean   ring_bell;
    Boolean   startup;
    int       app_count;
    String    app_names;
    String    lock_command;
    int       num_defines;
    String   *defines_sym;
    String   *defines_val;
    Boolean   console;
    String    preprocessor;
    Boolean   split_window;
    Boolean   quiet;
    Boolean   sounds;
#ifdef SOUNDS
    String    sound_start;
    String    sound_end;
    String    sound_start_wm;
    String    sound_end_wm;
    String    sound_start_app;
    String    sound_end_app;
    String    sound_lock;
    String    sound_notify;
#endif /* SOUNDS */
  }
  resources_rec;

typedef struct _wm_info_rec
  {
    char  *short_name;
    char  *full_name;
    char  *command;
    char  *config_file;
    char  *cmdline_opt;
    char  *env_opt;
    char  *tmp_config_file;
  }
  wm_info_rec;

typedef struct _app_info_rec
  {
    char  *short_name;
    char  *full_name;
    char  *command;
  }
  app_info_rec;


#ifndef UsingImake
#include "noimake.h"
#endif /* UsingImake */

#if !defined(HAS_VFORK) && !defined(NO_VFORK)
#ifdef HasVFork
#define HAS_VFORK
#else
#define NO_VFORK
#endif /* HasVFork */
#endif /* !HAS_VFORK && !NO_VFORK */

#if !defined(CPP_CMD)
#define CPP_STRING CppCmd
#else
#define CPP_STRING CPP_CMD
#endif /* !CPP_CMD */
