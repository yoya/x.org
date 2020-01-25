/* $XConsortium: prelim.c,v 1.0 93/11/22 12:26:56 rws Exp $ */

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

#include <stdio.h>
#include <X11/PEX5/PEXlib.h>

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "pexutcmap.h"
#include <misc.h>
#include <err_handler.h>

/* 
	Inquires some basic information about the PEX implementation
	and prints warnings if it does not meet the minimum requirements
	for running the PEX-IC susPEX test suite.
*/

Window local_win;

test_body(Display *dpy, XID resourceID, PEXOCRequestType req_type)
    {
	PEXExtensionInfo    *ext_info;
	unsigned int 	    subset_info;
	unsigned int	    float_format;


	/*
		Get the extension information.
	*/
	if ((ext_info = PEXGetExtensionInfo (dpy)) == NULL) {
	    _hppex_stderr_print("ERROR:  PEXGetExtensionInfo returned NULL pointer\n" );
	}

	_hppex_stderr_print("Information about this PEX implementation:\n");
	_hppex_stderr_print("  Vendor string: %s\n", ext_info->vendor_name);
	_hppex_stderr_print("  Release:  %d (0x%x)\n\n", ext_info->release, ext_info->release);
	

	/*
		Check the protocol version.
	*/
	if ((ext_info->major_version != 5) || (ext_info->minor_version < 1)) {
	    _hppex_stderr_print("WARNING:  Protocol version is not 5.1 or higher.\n" );
	    _hppex_stderr_print("          The PEX-IC susPEX suite is written to use\n");
	    _hppex_stderr_print("          PEX 5.1 requests and is unlikely to work correctly\n");
	    _hppex_stderr_print("          on 5.0 implementations.\n\n");
	}


	/*
		Check the subset support.
	*/
	subset_info = ext_info->subset_info & 0xffff;
	_hppex_stderr_print("Subsets supported are:\n");

	if (subset_info & PEXImmediateMode)
	    _hppex_stderr_print("  Immediate mode\n");
	if (subset_info & PEXStructureMode)
	    _hppex_stderr_print("  Structure mode\n");
	if (subset_info & PEXWorkstationOnly)
	    _hppex_stderr_print("  PHIGS Workstation mode\n");
	_hppex_stderr_print("\n");
	if (! ((subset_info & PEXImmediateMode) &&
	       (subset_info & PEXStructureMode))) {
	    _hppex_stderr_print("WARNING:  Without the immediate mode and structure mode\n");
	    _hppex_stderr_print("          subsets, the PEX-IC susPEX test suite can be\n");
	    _hppex_stderr_print("          expected to exhibit failures in testing basic\n");
	    _hppex_stderr_print("          functionality.\n\n");
	}
	if (! (subset_info & PEXWorkstationOnly)) {
	    _hppex_stderr_print("WARNING:  Without the PHIGS workstation support,\n");
	    _hppex_stderr_print("          the PEX-IC susPEX test suite can test only\n");
	    _hppex_stderr_print("          basic functionality but not full functionality\n\n");
	}


	/*
		Check the floating point format.
	*/
	float_format = PEXGetProtocolFloatFormat (dpy);
	if (float_format != PEXIEEE_754_32) {
	    _hppex_stderr_print("WARNING:  The natural protocol float format of the server\n");
	    _hppex_stderr_print("          is not IEEE_754_32.  There may be serious problems\n");
	    _hppex_stderr_print("          in interoperability unless the client library has\n");
	    _hppex_stderr_print("          floating point format conversion\n\n");
	}

	
	/*
		Check various enumerated types as determined by the PEX-IC
		test suite Basic functionality definition.
	*/
	{
	    Status              status = 0;
	    int 		enum_count;
	    unsigned long       enum_item_mask = 0;
	    unsigned long       *info_count_return = 0;
	    PEXEnumTypeDesc     *enum_info_return = 0;

	    int 		enum_index, j;
	    int		    	RGB_found, Indexed_found, approx_found, hlhsr_found;

	    int                 enum_types[3];
	    enum_count = 3;
	    enum_types[0] = PEXETColorType;
	    enum_types[1] = PEXETColorApproxModel;
	    enum_types[2] = PEXETHLHSRMode;
	    enum_item_mask = PEXETIndex;

	    status = 0;
	    status = PEXGetEnumTypeInfo(dpy, local_win, enum_count, enum_types, enum_item_mask, 
				    &info_count_return, &enum_info_return);
	    if (status == 0)
		_hppex_stderr_print("ERROR:  zero status from PEXGetEnumTypeInfo()\n\n");

	    enum_index=0;

	    /*
		Color types required:  PEXColorTypeRGB and PEXColorTypeIndexed.
	    */
	    RGB_found = False;
	    Indexed_found = False;
	    for (j=0; j<info_count_return[0]; j++) {
		if (((short) enum_info_return[enum_index].index) == 
		    ((short) PEXColorTypeRGB))
		    RGB_found = True;
		else if (((short) enum_info_return[enum_index].index) == 
		    ((short) PEXColorTypeIndexed))
		    Indexed_found = True;
		enum_index++;
	    }
	    if (!RGB_found) {
		_hppex_stderr_print("WARNING:  The PEX-IC susPEX test suite makes heavy use\n");
		_hppex_stderr_print("          of color type PEXColorTypeRGB.  This type is not\n");
		_hppex_stderr_print("          supported by this server\n\n");
	    }
	    if (!Indexed_found) {
		_hppex_stderr_print("WARNING:  The PEX-IC susPEX test suite makes heavy use\n");
		_hppex_stderr_print("          of color type PEXColorTypeRGB.  This type is not\n");
		_hppex_stderr_print("          supported by this server\n\n");
	    }

	    /*
		Color approximation models required:  PEXColorApproxRGB.
	    */
	    approx_found = False;
	    for (j=0; j<info_count_return[1]; j++) {
		if (((short) enum_info_return[enum_index].index) == 
		    ((short) PEXColorApproxRGB))
		    approx_found = True;
		enum_index++;
	    }
	    if (!approx_found) {
		_hppex_stderr_print("WARNING:  The PEX-IC susPEX test suite relies on\n");
		_hppex_stderr_print("          use of color approximation model PEXColorApproxRGB\n");
		_hppex_stderr_print("          which is not supported by this server\n\n");
	    }

	    /*
		HLHSR modes required:  PEXHLHSRZBuffer.
	    */
	    hlhsr_found = False;
	    for (j=0; j<info_count_return[2]; j++) {
		if (((short) enum_info_return[enum_index].index) == 
		    ((short) PEXHLHSRZBuffer))
		    hlhsr_found = True;
		enum_index++;
	    }
	    if (!hlhsr_found) {
		_hppex_stderr_print("WARNING:  The PEX-IC susPEX test suite makes some\n");
		_hppex_stderr_print("          use of HLHSR mode PEXHLHSRZBuffer\n");
		_hppex_stderr_print("          which is not supported by this server\n");
		_hppex_stderr_print("          The suite also occasionally uses PEXHLHSRZBufferID\n\n");
	    }

	    PEXFreeEnumInfo(enum_count, info_count_return, enum_info_return);
	}
    }



void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "prelim");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "prelim");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    _hppex_set_stderr("prelim.stderr", "prelim");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nQualifier Test for PEX-IC Test Suite\n\n");
    describe_test("This tests determines some basic information about\n");
    describe_test("the implementation and prints warnings if problems\n");
    describe_test("are predicted when running the PEX-IC suite.\n\n");

    test_body(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
        file_testproc("prelim.stderr", "prelim");
    }
