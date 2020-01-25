/* $XConsortium: fileutils.c,v 1.0 93/11/22 12:24:49 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

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


#include "X11/PEX5/PEXlib.h"
#include <stdio.h>

extern int errno;
extern char *getenv();

FILE *fp;

FILE *
suspex_open_file(testname, testdesc)
    char *testname;
    char *testdesc;
{
    char str[80];
    char *env;
    char *cur_dir = ".";

    if (!(env = getenv("CUR_IMAGE_DIR")))
        env = cur_dir;
    sprintf(str, "%s/%s.txt", env, testname);
    fp = fopen(str, "w");
    return (fp);
}
    
suspex_close_file(fp)
    FILE *fp;
{
    fclose(fp);
}

suspex_print_el_info(fp, num, info)
    FILE *fp;
    int  num;
    PEXElementInfo *info;
{
    int  i;

    for (i = 0; i < num; i++, info++) 
	fprintf(fp, "%d	%d\n", (int)info->type, (int)info->length);
}
