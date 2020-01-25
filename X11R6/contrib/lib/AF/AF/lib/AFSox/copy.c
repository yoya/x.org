#ifndef LINT
#ifdef RCS_ID
static char *rcsid = "@(#)$Header: /home/wesw/nwd/terminator/src/av/crl/audio/AF/lib/AFSox/RCS/copy.c,v 1.6 1994/02/02 01:22:51 marvinw Exp $";
#endif /* RCS_ID */
#endif /* LINT */
/***********************************************************
$Copyright$,1994 by Tektronix, Inc., Wilsonville, Oregon.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Tektronix or Tek not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/*
 * July 5, 1991
 * Copyright 1991 Lance Norskog And Sundry Contributors
 * This source code is freely redistributable and may be used for
 * any purpose.  This copyright notice must be maintained. 
 * Lance Norskog And Sundry Contributors are not responsible for 
 * the consequences of using this software.
 */

/*
 * Sound Tools skeleton effect file.
 */

#include "st.h"

/*
 * Process options
 */
copy_getopts(ft, n, argv) 
ft_t ft;
int n;
char **argv;
{
	eff_t effp = &ft->eff;
	if (n)
	{
		fail(ft, "Copy effect takes no options.");
		exit(-1);
	}
}

/*
 * Start processing
 */
copy_start(ft)
ft_t ft;
{
	eff_t effp = &ft->eff;
	/* nothing to do */
	/* stuff data into delaying effects here */
}

/*
 * Read up to len samples from file.
 * Convert to signed longs.
 * Place in buf[].
 * Return number of samples read.
 */

copy_flow(ft, ibuf, obuf, isamp, osamp)
ft_t ft;
long *ibuf, *obuf;
int *isamp, *osamp;
{
	int done;
        char *tmp;
        int tmpsize;
	
	done = ((*isamp < *osamp) ? *isamp : *osamp);
/***********
	memcpy(obuf, ibuf, done * sizeof(long));
***********/
        tmp = ft->ibuf;
        tmpsize = ft->isize;
        ft->ibuf = ft->obuf;
        ft->isize = ft->osize;
        ft->obuf = tmp;
        ft->osize = tmpsize;
	*isamp = *osamp = done;
	return done;
}

/*
 * Do anything required when you stop reading samples.  
 * Don't close input file! 
 */
copy_stop(ft)
ft_t ft;
{
	eff_t effp = &ft->eff;
	/* nothing to do */
}




