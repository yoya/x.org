/* $XConsortium: err_handler.c,v 1.0 93/11/22 12:21:53 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

/*
 *  Header files required by this C file:
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <varargs.h>

#include "X11/PEX5/PEXlib.h"
#define  TRUE  1
#define  FALSE 0

#include "X11/Xproto.h"
#include "err_handler.h"
#include "misc.h"

#ifndef XSYSTEM
extern FILE *   suspex_open_file();
extern int      suspex_close_file();
#endif

static int   trap_error;
static int   error_occured;
static FILE *stderr_file = stderr;

void _hppex_stderr_print();

static struct 
	{
	    int error_type;
	    int error_code;
	    int request_code;
	    int sub_code;
	    int oc_code;
		int foobar;    
	} trapped_error_data;


static struct 
	{
	    int error_type;
	    int error_code;
	    int request_code;
	    int sub_code;
	    int oc_code;
	} expected_error_data;


/*
 * Testing error handler
 */
static int testing_error_handler (Display *dpy, XErrorEvent *error)
{
    PEXOCErrorEvent  *pex_error;
    PEXExtensionInfo *pexinfo;
       
    int pex_error_code;
	int first_pex_error;
	int error_type;
	int foobar;

    error_occured = TRUE;

    /*
     * See what kind of error we have
     */

	 error_type=X_ERR;
	 foobar = 0;

     pex_error = (PEXOCErrorEvent *) error;

	 if (pexinfo = PEXGetExtensionInfo(dpy))
	   {
	    first_pex_error =  pexinfo->first_error;

	 	if (((int)pex_error->error_code >= first_pex_error) &&
			((int)pex_error->error_code <= first_pex_error + PEXMaxError))
			{
				error_type=PEX_ERR;
			}
       }
     else /* PEX not initialized on this connection!!! */
	   {
			foobar = 1;
	   }
     if (error_type == PEX_ERR )
     {

	    /*
         * It's a PEX error so handle it as such
         */

	     pex_error_code = pex_error->error_code - first_pex_error;



	if (trap_error)
	{
	    /*
             * Save trapped data to be looked at later.
             */
	    trapped_error_data.error_type = PEX_ERR;
	    trapped_error_data.error_code = pex_error_code;
	    trapped_error_data.request_code = pex_error->minor_code;
	    /*
	    trapped_error_data.sub_code = pex_error->sub_error_code;
	    */
	    if (pex_error_code == BadPEXOutputCommand)
	    {
		trapped_error_data.oc_code = pex_error->op_code;
	    }
	    trapped_error_data.foobar = foobar; 
	}
	else
	{

		if (foobar)
		{
		    _hppex_stderr_print( 
					 "*** Test Error Handler Warning: \n\tReceived following error from connection that has not had PEX \n\tInitialized! This may indicate either a bug in Xlib error handling or \n\ta defect in PEXlib internal logic. Something is probably foobar.\n");
		}

	    /*
             * Print out short message
             */
	    switch (pex_error->error_code - first_pex_error)
	    {
		case BadPEXColorType:
		    _hppex_stderr_print( "PEXError: Bad PEX Color Type\n");
		    break;

		case BadPEXRendererState:
		    _hppex_stderr_print( "PEXError: Bad PEX Renderer State\n");
		    break;

		case BadPEXFloatingPointFormat:
		    _hppex_stderr_print( "PEXError: Bad PEX FloatingPointFormat\n");
		    break;

		case BadPEXLabel:
		    _hppex_stderr_print( "PEXError: Bad PEX Label\n");
		    break;

		case BadPEXLookupTable:
		    _hppex_stderr_print( "PEXError: Bad PEX Lookup Table\n");
		    break;

		case BadPEXNameSet:
		    _hppex_stderr_print( "PEXError: Bad PEX Nameset\n");
		    break;

		case BadPEXPath:
		    _hppex_stderr_print( "PEXError: Bad PEX Path\n");
		    break;

		case BadPEXFont:
		    _hppex_stderr_print( "PEXError: Bad PEX Font\n");
		    break;

		case BadPEXWorkstation:
		    _hppex_stderr_print( "PEXError: Bad PEX Workstation\n");
		    break;

		case BadPEXPickMeasure:
		    _hppex_stderr_print( "PEXError: Bad PEX Pick Measure\n");
		    break;

		case BadPEXPipelineContext:
		    _hppex_stderr_print( "PEXError: Bad PEX Pipeline Context\n");
		    break;

		case BadPEXRenderer:
		    _hppex_stderr_print( "PEXError: Bad PEX Renderer\n");
		    break;

		case BadPEXSearchContext:
		    _hppex_stderr_print( "PEXError: Bad PEX Search Context\n");
		    break;

		case BadPEXStructure:
		    _hppex_stderr_print( "PEXError: Bad PEX Structure\n");
		    break;

		case BadPEXOutputCommand:
		    _hppex_stderr_print( "PEXError: Bad PEX OC\n");
	            _hppex_stderr_print( "OC code: %d\n", pex_error->op_code);
		
	    }
	   _hppex_stderr_print( "Request code: %d\n", pex_error->minor_code);
	   /*
	   _hppex_stderr_print( "Sub code: %d\n\n", pex_error->sub_error_code);
	   */
	}
    }
    else
    {
	/*
         * It's not a PEX error. Treat the error in the "default" manner.
         */
	if (trap_error)
	{
	    /*
             * Save trapped data to be looked at later.
             */
	    trapped_error_data.error_type = X_ERR;
	    trapped_error_data.error_code = error->error_code;
	    trapped_error_data.request_code = error->minor_code;
	    trapped_error_data.sub_code = 0;
	    trapped_error_data.foobar = foobar; 
		if (trapped_error_data.foobar)
		{
	    	_hppex_stderr_print( 
					 "*** Test Error Handler Warning: \n\tReceived following error from connection that has not had PEX \n\tInitialized! This may indicate either a bug in Xlib error handling or \n\ta defect in PEXlib internal logic. Something is probably foobar.\n");

			_hppex_stderr_print( 
				"    trapped error type was %d\n",
				 trapped_error_data.error_type);	

			_hppex_stderr_print( 
				"    trapped error code was %d\n",
				 trapped_error_data.error_code);	
	
			_hppex_stderr_print( 
				"    trapped request code was %d\n",
				 trapped_error_data.request_code);	

	  		_hppex_stderr_print( 
				"    trapped sub code was %d\n",
				 trapped_error_data.sub_code);	

	  		_hppex_stderr_print( 
				"    trapped OC code was %d\n",
				trapped_error_data.oc_code);	
			}
	}
	else
	{

		if (foobar)
		{
		    _hppex_stderr_print( 
					 "*** Test Error Handler Warning: \n\tReceived following error from connection that has not had PEX \n\tInitialized! This may indicate either a bug in Xlib error handling or \n\ta defect in PEXlib internal logic. Something is probably foobar.\n");
		}

	    /*
	     * Print small message.
             * See X.h for definitions of opcodes used below
             */
	    switch (error->error_code)
	    {
		case BadRequest:
		    _hppex_stderr_print( "X Error: Bad Request\n");
		    break;
	
		case BadValue:
		    _hppex_stderr_print( "X Error: Bad Value\n");
		    break;
	
		case BadWindow:
		    _hppex_stderr_print( "X Error: Bad Window\n");
		    break;
	
		case BadPixmap:
		    _hppex_stderr_print( "X Error: Bad Pixmap\n");
		    break;
	
		case BadAtom:
		    _hppex_stderr_print( "X Error: Bad Atom\n");
		    break;
	
		case BadCursor:
		    _hppex_stderr_print( "X Error: Bad Cursor\n");
		    break;
	
		case BadFont:
		    _hppex_stderr_print( "X Error: Bad Font\n");
		    break;
	
		case BadMatch:
		    _hppex_stderr_print( "X Error: Bad Match\n");
		    break;
	
		case BadDrawable:
		    _hppex_stderr_print( "X Error: Bad Drawable\n");
		    break;
	
		case BadAccess:
		    _hppex_stderr_print( "X Error: Bad Access\n");
		    break;
	
		case BadAlloc:
		    _hppex_stderr_print( "X Error: Bad Alloc\n");
		    break;
	
		case BadColor:
		    _hppex_stderr_print( "X Error: Bad Color\n");
		    break;
	
		case BadGC:
		    _hppex_stderr_print( "X Error: Bad GC\n");
		    break;
	
		case BadIDChoice:
		    _hppex_stderr_print( "X Error: Bad ID Choice\n");
		    break;
	
		case BadName:
		    _hppex_stderr_print( "X Error: Bad Name\n");
		    break;
	
		case BadLength:
		    _hppex_stderr_print( "X Error: Bad Length\n");
		    break;
	
		case BadImplementation:
		    _hppex_stderr_print( "X Error: Bad Implementation\n");
		    break;
	
		default:
		    _hppex_stderr_print( "X Error: Unknown error\n");
		    break;
	    }
	    _hppex_stderr_print("Request code: %d\n\n", error->minor_code);
	}
    }
}

/*
 * Testing error initialization routine
 */
void _hppex_test_init_error_handler ( void )
{
    XSetErrorHandler (testing_error_handler);
    trap_error    = FALSE;
    error_occured = FALSE;
}	




/*
 * Trap expected error
 */
void _hppex_test_set_expected_error (int error_type, 
				     int error_code, 
				     int request_code,
				     int sub_code,
				     int oc_code)
{

    if (error_type == NO_ERR)
    {
        trap_error = TRUE;
	expected_error_data.error_type = error_type;
	expected_error_data.error_code = 0;
	expected_error_data.request_code = 0;
	expected_error_data.sub_code = 0;
	expected_error_data.oc_code = 0;
    }
    else if (error_type == X_ERR)
    {
        trap_error = TRUE;
	expected_error_data.error_type = error_type;
	expected_error_data.error_code = error_code;
	expected_error_data.request_code = request_code;
	expected_error_data.sub_code = 0;
	expected_error_data.oc_code = 0;
    }
    else if (error_type == PEX_ERR)
    {
        trap_error = TRUE;
	expected_error_data.error_type = error_type;
	expected_error_data.error_code = error_code;
	expected_error_data.request_code = request_code;
	expected_error_data.sub_code = sub_code;
	expected_error_data.oc_code = oc_code;
    }
    else
    {
	_hppex_stderr_print( "Attempt to trap bogus error type\n\n");
    }
	
    if (trap_error) {
	trapped_error_data.error_type = NO_ERR;
	trapped_error_data.error_code = -1;
	trapped_error_data.request_code = -1;
	trapped_error_data.sub_code = -1;
	trapped_error_data.oc_code = -1;
    }
}


/*
 * Check expected error
 */
int _hppex_test_check_expected_error (unsigned long mask)
{
    int any_error = FALSE;
		
    /*
     * Check error type always
     */
    if (expected_error_data.error_type != trapped_error_data.error_type)
    {
	any_error = TRUE;
	_hppex_stderr_print( 
		"ERROR: Expected error type was %d, trapped error type was %d\n",
		expected_error_data.error_type, trapped_error_data.error_type);	
    }

    /*
     * Check error code if required
     */
    if ((expected_error_data.error_type > NO_ERR) && (mask & ERROR_CODE))
    {
	if (expected_error_data.error_code != trapped_error_data.error_code)
	{
            any_error = TRUE;
	    _hppex_stderr_print( 
		"ERROR: Expected error code was %d, trapped error code was %d\n",
		expected_error_data.error_code, trapped_error_data.error_code);	
	    
	}
    }

    /*
     * Check request code if required
     */
    if ((expected_error_data.error_type > NO_ERR) && (mask & REQUEST_CODE))
    {
	if (expected_error_data.request_code != trapped_error_data.request_code)
	{
            any_error = TRUE;
	    _hppex_stderr_print( 
		"ERROR: Expected request code was %d, trapped request code was %d\n",
		expected_error_data.request_code, trapped_error_data.request_code);	
	    
	}
    }

    /*
     * Check sub code if required
     */
    if ((expected_error_data.error_type == PEX_ERR) && (mask & SUB_CODE))
    {
	if (expected_error_data.sub_code != trapped_error_data.sub_code)
	{
            any_error = TRUE;
	    _hppex_stderr_print( 
		"ERROR: Expected sub code was %d, trapped sub code was %d\n",
		expected_error_data.sub_code, trapped_error_data.sub_code);	
	    
	}
    }

    /*
     * Check OC code if required
     */
    if ((expected_error_data.error_type == PEX_ERR) && (mask & SUB_CODE) &&
	(expected_error_data.error_code == BadPEXOutputCommand))
    {
	if (expected_error_data.oc_code != trapped_error_data.oc_code)
	{
            any_error = TRUE;
	    _hppex_stderr_print( 
		"ERROR: Expected OC code was %d, trapped OC code was %d\n",
		expected_error_data.oc_code, trapped_error_data.oc_code);	
	    
	}
    }

    if (any_error)
	_hppex_stderr_print( "\n");

    trap_error = FALSE;

    return any_error;
}

/*
 * Set error reporting file
 */
void _hppex_set_stderr (char *err_file_name, 
			char *test_name)
{
#ifndef XSYSTEM
    stderr_file = suspex_open_file(err_file_name,
				       test_name);
#endif
}

/*
 * Close error reporting file
 */
void _hppex_close_stderr ()
{
#ifndef XSYSTEM
    if (stderr_file != stderr)
	{
        suspex_close_file(stderr_file);
	stderr_file = stderr;
	}
#endif
}

/*
 * Issue a messsage to stderr_file, and also to the real stderr
 * if it is different from stderr_file and verbose mode is on.
 */
void _hppex_stderr_print(va_alist)
    va_dcl
    {
    char   *fmt;
    va_list ap;

    va_start(ap);
    fmt = va_arg(ap, char *);
    vfprintf(stderr_file, fmt, ap);
    fflush(stderr_file);
    va_end(ap);

    if (glob_verbose_mode && (stderr_file != stderr))
	{
        va_start(ap);
        fmt = va_arg(ap, char *);
        vfprintf(stderr, fmt, ap);
        fflush(stderr);
        va_end(ap);
	}
    }
