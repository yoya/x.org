/* $XConsortium: testutils.c,v 1.0 93/11/22 12:24:37 rws Exp $ */

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

#include <X11/Xlib.h>
#include "logmsg.h"
#include <sys/file.h>

#ifdef SYSV
#define R_OK 4
#endif

void standard_testproc();
void image_testproc();
void file_testproc();

void
standard_testproc(testname, testdesc, dpy, window, x, y, w, h)
    char 		*testname;
    char 		*testdesc;
    Display		*dpy;
    Window		window;
    int			x, y, w, h;
{
    tbegintest(testname,testdesc);
    i_check_image(testname,dpy,window,x,y,w,h,testdesc);
    if (pausebetweenframe) {
	printf("hit return to continue...");
	getchar();
    }
    tendtest();
} 

void
image_testproc(testname, testdesc, imagedesc,dpy, window, x, y, w, h)
    char 		*testname;
    char 		*testdesc;
    char 		*imagedesc;
    Display		*dpy;
    Window		window;
    int			x, y, w, h;
{
    tbegintest(testname,testdesc);
    i_check_image(testname,dpy,window,x,y,w,h,imagedesc);
    if (pausebetweenframe) {
	printf("hit return to continue...");
	getchar();
    }
    tendtest();
} 

void
file_testproc(testname, testdesc)
    char 		*testname;
    char 		*testdesc;
{
    char refpath[80];
    char curpath[80];
    char badpath[80];
    char *curp = NULL;          
    char *refp = NULL;          
    char *badp = NULL;         
    char command[200];
    int  status;
   
    tbegintest(testname,testdesc);
    if (nocheckpix) {        
	tvprintf(0,"Warning: file_testproc: file checking disabled\n");
        tendtest();
        return;
    }

    if (ref_image_dir == NULL)
        tabort("ERROR: REF_IMAGE_DIR is not specified, exiting...");
    if (bad_image_dir == NULL)
        tabort("ERROR: BAD_IMAGE_DIR is not specified, exiting...");
    if (cur_image_dir == NULL)
        tabort("ERROR: CUR_IMAGE_DIR is not specified, exiting...");
    sprintf(refpath,"%s/%s.txt",ref_image_dir,testname);
    sprintf(curpath,"%s/%s.txt",cur_image_dir,testname);
    sprintf(badpath,"%s/%s-.txt",bad_image_dir,testname);
    if (access(refpath, R_OK) == 0) refp = refpath;
    if (access(badpath, R_OK) == 0) badp = badpath;
    if (access(curpath, R_OK) == 0) curp = curpath;


    /* No current file, something's went wrong */

    if (!curp) {
      tfprintf("failed: no current file %s\n", testname);

    } else {

    /* CASE 1: no reference file found - save current file for inspection */

      if (!refp) {
	tfprintf("failed: no reference file %s, saving current file\n", testname);

      } else { /* reference file found - compare */

        sprintf(command,"cmp -s %s %s",refpath, curpath);
        status = system(command);

    /* CASE 2: matches good reference file - delete current and bad file */

        if (status == 0) {
          if (unlink(curpath) == 1) 
            tvprintf(0,"error: couldn't delete current file %s\n", curpath);
          if (badp != NULL) 
            if (unlink(badpath) == 1) 
              tvprintf(0,"error: couldn't delete bad file %s\n", badpath);

        } else { /* doesn't match good reference file */

          if (badp != NULL) { /* bad reference file exist, compare */

            sprintf(command,"cmp -s %s %s",badpath, curpath);
            status = system(command);

    /* CASE 3: matches bad reference file, delete current file */

            if (status == 0) {
              tfprintf("failed: %s matched known bad file\n", testname);
              if (unlink(curpath) == 1) 
                  tvprintf(0,"error: couldn't delete current file %s\n", 
		    curpath);

            } else {

    /* CASE 4: doesn't match reference files, save current file for inspection*/
	      tfprintf("failed: pending manual comparision of file %s\n",
		testname);
	    }
          } else {

    /* CASE 5: doesn't match good reference, bad reference doesn't exist */
    /*         save current file for inspection*/
	    tfprintf("failed: pending manual comparision of file %s\n",testname);
          }
        }
      }
    }
    tendtest();
} 
