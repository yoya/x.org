#ifndef	lint
#ifdef sccs
static char     sccsid[] = "@(#)nint_i_in.c 20.8 89/08/17 Copyr 1985 Sun Micro";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

/*
 * Nint_i_in.c - Implement the notify_interpose_input_func interface.
 */

#include <xview_private/ntfy.h>
#include <xview_private/nint.h>

extern          Notify_error
notify_interpose_input_func(nclient, func, fd)
    Notify_client   nclient;
    Notify_func     func;
    int             fd;
{
    return (nint_interpose_fd_func(nclient, func, NTFY_INPUT, fd));
}
