/* $XConsortium: logmsg.c,v 1.0 93/11/22 12:24:54 rws Exp $ */

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



/*
 * Message logging utilities
 */
#include "stdio.h"
#include "logmsg.h"
#include "varargs.h"

/*
 * global variables
 */
char    *testname = NULL;	/* current test name */
char    *testdesc = NULL;	/* test description */
int      maxfail = 5;    	/* max failures before abort */
int      numfail;    		/* current number of failures */
int      verbosity = 0;  	/* verbosity level */
int      pausebetweenframe = 0; /* pause flag */
char    *ref_image_dir = NULL;	/* path for ref image dir */
char    *bad_image_dir = NULL;	/* path for bad image dir */
char    *cur_image_dir = NULL;	/* path for current image  dir */
int      showpix = 0;        	/* 1 for showing where pixels are sampled */
int      nocheckpix = 0;        /* 1 for no image checking */
int      delay = 0;         	/* micro secs delay before checking image*/



/*
 * external functions
 */
char *getenv();

/*
 * forward declaration
 */
/*
 * export routines */
void tbegintest();
void tfprintf();
void tvprintf();
void tendtest();
void tabort();


/*
 * tbegintest(name, desc) initializes testing environment
 */
void
tbegintest(name, desc)
    char           *name;       /* test name */
    char           *desc;       /* test description */
{
    char           *cp;

    /* initialize testing environment */
    testname = name;
    testdesc = desc;
    numfail = 0;
    if (cp = getenv("NO_IMAGE_CHECK"))
        nocheckpix = 1;
    if (cp = getenv("MAXFAIL"))
        maxfail = atoi(cp);
    if (cp = getenv("VERBOSITY"))
        verbosity = atoi(cp);
    if (cp = getenv("PAUSE"))
        pausebetweenframe = 1;
    if (cp = getenv("SHOWPIX"))
        showpix = 1;
    if (cp = getenv("I_DELAY"))
        delay = atoi(cp);
    if (cp = getenv("REF_IMAGE_DIR"))
        ref_image_dir = cp;
    if (cp = getenv("BAD_IMAGE_DIR"))
        bad_image_dir = cp;
    if (cp = getenv("CUR_IMAGE_DIR"))
        cur_image_dir = cp;
    tvprintf(2, "Beginning test...\n");
}

/*
 * tendtest() terminates test
 */
void
tendtest()
{
    if (numfail)
        printf("%s: FAILED: %s\n", testname, testdesc);
    else
        printf("%s: PASSED: %s\n", testname, testdesc);
    tvprintf(2, "Ending test.\n");
}


/*
 * tfprintf(fmt,va_alist) prints out msg, incs fail
 */
void
tfprintf(fmt, va_alist)
    char           *fmt;        /* message fmt */
    va_dcl                      /* variable data */
{
    va_list         ap;
    
    va_start(ap);
    printf("%s: ",testname);
    vprintf(fmt, ap);
    va_end(ap);

    if (++numfail >= maxfail) {
        printf("%s: failed: Exiting, too many failures (%d).\n",testname,numfail);
	tendtest();
        exit(1); 
    }
}

/*
 * tvprintf(ver,fmt,va_alist) prints out message according to given verbosity
 */
void
tvprintf(ver, fmt, va_alist)
    int             ver;        /* current verbosity */
    char           *fmt;        /* message fmt */
    va_dcl                          /* variable data */
{
    va_list         ap;

    /* print out message if VERBOSITY >= this message's verbosity */
    if (ver <= verbosity) {
        va_start(ap);
        printf("%s: ",testname);
        vprintf(fmt, ap);
        va_end(ap);
    }
}

/*
 * tabort(fmt,va_alist)
 * prints out error message and abort program
 */
void
tabort(fmt,va_alist)

        char *fmt;              /* message fmt */
        va_dcl                  /* variable data */
{
        va_list ap;

        va_start(ap);
        printf("%s: ",testname);
        vprintf(fmt,ap);
        va_end(ap);
	++numfail;
	tendtest();
        exit(1);
}


