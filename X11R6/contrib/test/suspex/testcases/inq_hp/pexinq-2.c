/* $XConsortium: pexinq-2.c,v 1.0 93/11/22 12:48:08 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <misc.h>

/* 
 * PEXlib Inquiries Module:  Test Case 2  --  PEXGetEnumTypeInfo
 *
 */

Window local_win;

test_image(Display         *dpy, 
	   XID              resourceID, 
	   PEXOCRequestType req_type)
    {
    Status              status = 0;
    PEXEnumTypeDesc     *enum_info_return = 0;
    int                 enum_types[4];
    unsigned long       enum_item_mask = 0;
    unsigned long       *info_count_return = 0;
    PEXExtensionInfo    *info_return;
    static char         *enum_mnemonics[] = {"PEXETLineType",
					     "PEXETSurfaceEdgeType",
					     "PEXETPickAllMethod",
					     "PEXETEscape"};
    int i,j;
    int enum_index=0;

    enum_types[0] = PEXETLineType;
    enum_types[1] = PEXETSurfaceEdgeType;
    enum_types[2] = PEXETPickAllMethod;
    enum_types[3] = PEXETEscape;
    enum_item_mask = PEXETAll;

    status = 0;
    status = PEXGetEnumTypeInfo(dpy, local_win, 4, enum_types, enum_item_mask, &info_count_return, &enum_info_return);
    if (status == 0)
      _hppex_stderr_print("ERROR - Zero Status from PEXGetEnumTypeInfo()\n");


    for (i=0; i<4; i++)
	{
	_hppex_stderr_print("Enum Info for %s\n", enum_mnemonics[i]);

	for (j=0; j<info_count_return[i]; j++, enum_index++)
	    {
	    _hppex_stderr_print("  index    = %8d  mnemonic = %s\n",
				enum_info_return[enum_index].index,
				enum_info_return[enum_index].descriptor);
	    }
	}

    PEXFreeEnumInfo(3, info_count_return, enum_info_return);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexinq-2");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexinq-2");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexinq-2.stderr", "pexinq-2");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nInquiries Test #2 (PEXGetEnumTypeInfo)\n\n");
    describe_test("  This test queries and prints to stderr enum type info for\n");
    describe_test("  PEXETLineType, PEXETSurfaceEdgeType, PEXETPickAllMethod,\n");
    describe_test("  and PEXETEscape.  Output will be implementation dependant.\n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexinq-2.stderr", "pexinq-2");
    }
