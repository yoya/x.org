/*
 * Config file for xscreensaver, Copyright (c) 1991-1994 Jamie Zawinski.
 * This file is included by the various Imakefiles.
 */

/*  Uncomment the following line if you have the XIdle extension installed.
 *  XIdle comes on the X11r5 contrib tape, in .../contrib/extensions/xidle/
 *  (and maybe it will be on the r6 tape as well but I'm not sure where.)
 *  Installing it is the morally superior thing to do, but xscreensaver will
 *  work without it.
 *
 *  Actually it's likely that the R6 `screensaver' server extension will turn
 *  out to be the better thing to use, but I haven't had time to add support
 *  for it yet.  (See README to find the latest version of this program.)
 */
/* #define HAVE_XIDLE */

/*  Uncomment the following line if you have the XPM library installed.
 *  Some of the demos can make use of this.
 */
/* #define HAVE_XPM */

/*  Uncomment the following line if you don't have Motif.  If you don't have
 *  Motif, then the screensaver won't have any dialog boxes, which means
 *  that it won't be compiled with support for demo-mode or display-locking.
 */
/* #define NO_MOTIF */

/* Uncomment the following line if for some reason the locking code doesn't
 * work (for example, if you don't have the crypt() system call, or if you
 * don't use standard passwd files.)
 */
/* #define NO_LOCKING */

/*  Uncomment the following line if your system doesn't have the select()
 *  system call.
 */
/* #define NO_SELECT */

/*  Uncomment the following line if your system doesn't have the setuid(),
 *  setregid(), and getpwnam() library routines.
 *
 *  WARNING: if you do this, it will be unsafe to run xscreensaver as root
 *  (which probably means you can't have it be started by xdm.)  If you are
 *  on such a system, please try to find the corresponding way to do this,
 *  and then tell me what it is.
 */
/* #define NO_SETUID */

/*  Uncomment the following line if your system uses `shadow' passwords,
 *  that is, the passwords live in /etc/shadow instead of /etc/passwd,
 *  and one reads them with getspnam() instead of getpwnam().
 */
/* #define HAVE_SHADOW */

/*  You may need to edit these to correspond to where Motif is installed.
 */
#ifndef NO_MOTIF
  MOTIFINCLUDES = /* -I... */
 MOTIFLDOPTIONS = /* -L... */
      MOTIFLIBS = -lXm
#endif

/*  On some systems, only programs running as root can use the getpwent()
    library routine.  This means that, in order for locking to work, the
    screensaver must be installed as setuid to root.  Define this to make
    that happen.  (You must run "make install" as root for it to work.)
    (What systems other than HP need this?  Let me know.)
 */
#if defined(HPArchitecture) || defined(HAVE_SHADOW)
# define INSTALL_SETUID
#endif

#ifdef HPArchitecture
      CCOPTIONS = -Aa -D_HPUX_SOURCE	/* eat me */
# if (ProjectX <= 4)
  MOTIFINCLUDES = -I/usr/include/Motif1.1
 MOTIFLDOPTIONS = -L/usr/lib/Motif1.1
# else /* R5 */
  MOTIFINCLUDES = -I/usr/include/Motif1.2
 MOTIFLDOPTIONS = -L/usr/lib/Motif1.2
# endif /* R5 */
#endif /* HPArchitecture */

#ifdef MacIIArchitecture
      CCOPTIONS = -D_POSIX_SOURCE
#endif /* MacIIArchitecture */

#if (ProjectX <= 4)
# define R5ISMS -DXPointer="char*"
#else /* r5 or better */
# define R5ISMS
#endif

/* It seems that some versions of Sun's dynamic X libraries are broken; if
   you get link errors about _get_wmShellWidgetClass being undefined, try
   adding -Bstatic to the link command.
 */
