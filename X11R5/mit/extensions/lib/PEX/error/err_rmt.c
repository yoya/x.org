/* $XConsortium: err_rmt.c,v 5.2 91/02/18 11:13:16 rws Exp $ */

/***********************************************************
Copyright 1989, 1990, 1991 by Sun Microsystems, Inc. and the X Consortium.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Sun Microsystems,
the X Consortium, and MIT not be used in advertising or publicity 
pertaining to distribution of the software without specific, written 
prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include "phg.h"
#include <X11/Xproto.h>

extern Err_handle	phg_erh_create();
extern int		phg_err_pex_reply();
extern int		phg_err_x();

static void
err_destroy_remote( erh)
    Err_handle		erh;
{
    free( (char *)erh);
}

static void
err_buf_remote( erh, errnum)
    Err_handle	erh;
    Pint	errnum;
{
    Err_msg		msg;

    /* Don't do it if an inquiry. */
    if ( erh->cur_func_num != Pfn_INQUIRY ) {
	msg.errnum = errnum;
	msg.funcnum = erh->cur_func_num;

	if ( write( erh->data.remote.sfd, (char *)&msg, sizeof(msg)) <= 0 )
	    perror("Error in buf remote(write)");
    }
}

int
err_rmt_pex_reply( display, err, codes, ret_code, erh )
    Display     *display;
    xError      *err;
    XExtCodes   *codes;
    int         *ret_code;
    Err_handle	erh;
{
#ifdef DEBUG
    fprintf( stderr, "X/PEX IO error in phigsmon\n" );
#endif
    return phg_err_pex_reply( display, err, codes, ret_code, erh );
}

int
err_rmt_x( display, error )
    Display     *display;
    XErrorEvent	*error;
{
#ifdef DEBUG
    fprintf( stderr, "X/PEX error in phigsmon\n" );
#endif
    return phg_err_x( display, error );
}

Err_handle
phg_err_init_remote( fd)
    int		fd;
{
    Err_handle		erh;
    Err_msg		msg;
    extern void		PexSetErrorHandler();

    if ( (erh = phg_erh_create())) {
	erh->data.remote.sfd = fd;
	erh->buf_func = err_buf_remote;
	erh->flush_func = NULL;		/* servers don't flush */
	erh->destroy_func = err_destroy_remote;
	erh->report_func = NULL;	/* servers don't report */
	(void)PexSetErrorHandler( (caddr_t)erh, err_rmt_pex_reply );
	(void)XSetErrorHandler( err_rmt_x );

    } else {
	msg.errnum = ERR900;
	msg.funcnum = Pfn_open_phigs;
        write( fd, (char *)&msg, sizeof(msg));
    }

    return( erh);
}
