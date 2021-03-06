#ifndef	lint
#ifdef sccs
static char     sccsid[] = "@(#)ntfyperror.c 20.12 89/08/17 Copyr 1985 Sun Micro";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

/*
 * Ntfy_perror.c - Notify_perror implementation.
 */

#include <stdio.h>
#include <xview/xv_error.h>
#include <xview_private/ntfy.h>

extern void
notify_perror(str)
    char           *str;
{
    register char  *msg;
    char            dummy[128];

    switch (notify_errno) {
      case NOTIFY_OK:
	msg = "Success";
	break;
      case NOTIFY_UNKNOWN_CLIENT:
	msg = "Unknown client";
	break;
      case NOTIFY_NO_CONDITION:
	msg = "No condition for client";
	break;
      case NOTIFY_BAD_ITIMER:
	msg = "Unknown interval timer type";
	break;
      case NOTIFY_BAD_SIGNAL:
	msg = "Bad signal number";
	break;
      case NOTIFY_NOT_STARTED:
	msg = "Notifier not started";
	break;
      case NOTIFY_DESTROY_VETOED:
	msg = "Destroy vetoed";
	break;
      case NOTIFY_INTERNAL_ERROR:
	msg = "Notifier internal error";
	break;
      case NOTIFY_SRCH:
	msg = "No such process";
	break;
      case NOTIFY_BADF:
	msg = "Bad file number";
	break;
      case NOTIFY_NOMEM:
	msg = "Not enough memory";
	break;
      case NOTIFY_INVAL:
	msg = "Invalid argument";
	break;
      case NOTIFY_FUNC_LIMIT:
	msg = "Too many interposition functions";
	break;
      default:
	msg = "Unknown notifier error";
    }
    (void) sprintf(dummy, "%s: %s", str, msg);
    xv_error(NULL, XV_ERROR_RECOVERABLE, XV_ERROR_INTERNAL,
	     XV_ERROR_STRING, dummy, 0);
}
