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

static String             fallback_resources[] =
  {
#include "XSess-ad.h"
    NULL
  };


#define Offset(field) XtOffsetOf(struct _resources_rec, field)
static XtResource    resources_desc[] =
  {
#ifdef SOUNDS
    {
      "soundStart", "SoundStart", XtRString, sizeof(String),
      Offset(sound_start), XtRString, "none"
    },
    {
      "soundEnd", "SoundEnd", XtRString, sizeof(String),
      Offset(sound_end), XtRString, "none"
    },
    {
      "soundStartWM", "SoundStartWM", XtRString, sizeof(String),
      Offset(sound_start_wm), XtRString, "none"
    },
    {
      "soundEndWM", "SoundEndWM", XtRString, sizeof(String),
      Offset(sound_end_wm), XtRString, "none"
    },
    {
      "soundStartApp", "SoundStartApp", XtRString, sizeof(String),
      Offset(sound_start_app), XtRString, "none"
    },
    {
      "soundEndApp", "SoundEndApp", XtRString, sizeof(String),
      Offset(sound_end_app), XtRString, "none"
    },
    {
      "soundLock", "SoundLock", XtRString, sizeof(String),
      Offset(sound_lock), XtRString, "none"
    },
    {
      "soundNotify", "SoundNotify", XtRString, sizeof(String),
      Offset(sound_notify), XtRString, "none"
    },
    {
      "sounds", "Sounds", XtRBoolean, sizeof(Boolean),
      Offset(sounds), XtRImmediate, (XtPointer)TRUE
    },
#else
    {
      "sounds", "Sounds", XtRBoolean, sizeof(Boolean),
      Offset(sounds), XtRImmediate, (XtPointer)FALSE
    },
#endif /* SOUNDS */
    {
      "allowBell", "AllowBell", XtRBoolean, sizeof(Boolean),
      Offset(allow_bell), XtRImmediate, (XtPointer)FALSE
    },
    {
      "appNames", "AppNames", XtRString, sizeof(String),
      Offset(app_names), XtRString, "XTerm"
    },
    {
      "console", "Console", XtRBoolean, sizeof(Boolean),
      Offset(console), XtRImmediate, (XtPointer)FALSE
    },
    {
      "defaultWM", "DefaultWM", XtRString, sizeof(String),
      Offset(default_wm), XtRString, "TWM"
    },
    {
      "keepTmpFile", "KeepTmpFile", XtRBoolean, sizeof(Boolean),
      Offset(keep_tmp_file), XtRImmediate, (XtPointer)FALSE
    },
    {
      "lockCommand", "LockCommand", XtRString, sizeof(String),
      Offset(lock_command), XtRString, "xscreensaver-command -lock"
    },
    {
      "motd", "Motd", XtRBoolean, sizeof(Boolean),
      Offset(motd), XtRImmediate, (XtPointer)TRUE
    },
    {
      "notify", "Notify", XtRBoolean, sizeof(Boolean),
      Offset(notify), XtRImmediate, (XtPointer)TRUE
    },
    {
      "preprocessor", "Preprocessor", XtRString, sizeof(String),
      Offset(preprocessor), XtRString, "m4"
    },
    {
      "quiet", "Quiet", XtRBoolean, sizeof(Boolean),
      Offset(quiet), XtRImmediate, (XtPointer)FALSE
    },
    {
      "remote", "Remote", XtRBoolean, sizeof(Boolean),
      Offset(remote), XtRImmediate, (XtPointer)FALSE
    },
    {
      "ringBell", "RingBell", XtRBoolean, sizeof(Boolean),
      Offset(ring_bell), XtRImmediate, (XtPointer)FALSE
    },
    {
      "splitWindow", "SplitWindow", XtRBoolean, sizeof(Boolean),
      Offset(split_window), XtRImmediate, (XtPointer)TRUE
    },
    {
      "startup", "Startup", XtRBoolean, sizeof(Boolean),
      Offset(startup), XtRImmediate, (XtPointer)TRUE
    },
    {
      "wmNames", "WMNames", XtRString, sizeof(String),
      Offset(wm_names), XtRString, "TWM"
    }
  };
#undef Offset


static XrmOptionDescRec options[] =
  {
    {
      "-sounds",    ".sounds",      XrmoptionNoArg, "TRUE"
    },
    {
      "+sounds",    ".sounds",      XrmoptionNoArg, "FALSE"
    },
    {
      "-nosounds",  ".sounds",      XrmoptionNoArg, "FALSE"
    },
    {
      "+nosounds",  ".sounds",      XrmoptionNoArg, "TRUE"
    },
    {
      "-notify",    ".notify",      XrmoptionNoArg, "TRUE"
    },
    {
      "+notify",    ".notify",      XrmoptionNoArg, "FALSE"
    },
    {
      "-nonotify",  ".notify",      XrmoptionNoArg, "FALSE"
    },
    {
      "+nonotify",  ".notify",      XrmoptionNoArg, "TRUE"
    },
    {
      "-remote",    ".remote",      XrmoptionNoArg, "TRUE"
    },
    {
      "+remote",    ".remote",      XrmoptionNoArg, "FALSE"
    },
    {
      "-keep",      ".keepTmpFile", XrmoptionNoArg, "TRUE"
    },
    {
      "+keep",      ".keepTmpFile", XrmoptionNoArg, "FALSE"
    },
    {
      "-startup",   ".startup",     XrmoptionNoArg, "TRUE"
    },
    {
      "+startup",   ".startup",     XrmoptionNoArg, "FALSE"
    },
    {
      "-nostartup", ".startup",     XrmoptionNoArg, "FALSE"
    },
    {
      "+nostartup", ".startup",     XrmoptionNoArg, "TRUE"
    },
    {
      "-defaultWM", ".defaultWM",   XrmoptionSepArg, NULL
    },
    {
      "-preprocessor", ".preprocessor", XrmoptionSepArg, NULL
    },
    {
      "-console",   ".console",     XrmoptionNoArg, "TRUE"
    },
    {
      "+console",   ".console",     XrmoptionNoArg, "FALSE"
    },
    {
      "-cpp",       ".preprocessor", XrmoptionNoArg, "cpp"
    },
    {
      "-m4",        ".preprocessor", XrmoptionNoArg, "m4"
    },
    {
      "-split",     ".splitWindow",  XrmoptionNoArg, "TRUE"
    },
    {
      "+split",     ".splitWindow",  XrmoptionNoArg, "FALSE"
    },
    {
      "-nosplit",   ".splitWindow",  XrmoptionNoArg, "FALSE"
    },
    {
      "+nosplit",   ".splitWindow",  XrmoptionNoArg, "TRUE"
    },
    {
      "-quiet",     ".quiet",        XrmoptionNoArg, "TRUE"
    },
    {
      "+quiet",     ".quiet",        XrmoptionNoArg, "FALSE"
    },
    {
      "-noquiet",   ".quiet",        XrmoptionNoArg, "FALSE"
    },
    {
      "+noquiet",   ".quiet",        XrmoptionNoArg, "TRUE"
    }
  };
