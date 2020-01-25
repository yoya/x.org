/* $XConsortium: logmsg.h,v 1.0 93/11/22 12:25:12 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1989, 1990, 1991, 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/


#ifndef _LOGMSG_H
#define _LOGMSG_H

/*
 * Message logging utilities
 */
extern char    *testname;	/* current test name */
extern char    *testdesc;	/* test description */
extern int      maxfail;    	/* max failures before abort */
extern int      numfail;    	/* current number of failures */
extern int      verbosity;  	/* verbosity level */
extern int      pausebetweenframe;  	/* pause flag */
extern char    *ref_image_dir;	/* path for ref image directory */
extern char    *bad_image_dir;	/* path for bad image directory */
extern char    *cur_image_dir;	/* path for current image directory */
extern int      showpix;	/* 1 for showing where pixels are sampled */
extern int      nocheckpix;	/* 1 for no image checking */
extern int      delay;		/* micro secs delay before checking image */

extern void tbegintest();
extern void tfprintf();
extern void tvprintf();
extern void tendtest();
extern void tabort();

#endif /* _LOGMSG_H */
