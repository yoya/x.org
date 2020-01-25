/* $XConsortium: lut_inq_capx.c,v 1.0 93/11/22 12:28:47 rws Exp $ */

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
#include <string.h>

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <misc.h>

#ifdef _HPUX_SOURCE
#include <X11/PEX5/PEXHPlib.h>
#else
#define PEXEscapeQueryColorApprox    0x80010001
#define PEXETEscapeQueryColorApprox  0x8101

/* QueryColorApprox mnemonic                                                  */
#define PEXETMEscapeQueryColorApprox "QueryColorApprox"

typedef struct
{
   Drawable            drawable;     /* Drawable to compare capx against     */
   PEXColorApproxEntry capx;         /* Color approx to check                */
} PEXEscapeQueryColorApproxData;

typedef struct
{
    char          capx_is_supported;  /* True if given input approx supported */
    char          all_capxs;          /* True if all alt capx's are returned  */
    char          reserved1[2];
    unsigned long count;              /* Number of alternative color approx's */
    unsigned int  reserved2[3];
} PEXEscapeQueryColorApproxReplyData;
#endif

#define NUM_CAPX_TEST_CASES        12

Window local_window;

PEXEscapeQueryColorApproxData       capx_data;
PEXEscapeQueryColorApproxReplyData *capx_reply_data;
int                                 capx_reply_data_size;

char  *test_descriptions[NUM_CAPX_TEST_CASES] =
  {
  "\nTest Case #  1:  Valid ColorSpace input capx\n",
  "\nTest Case #  2:  Invalid type field in capx\n",
  "\nTest Case #  3:  Invalid model field in ColorSpace capx\n",
  "\nTest Case #  4:  Invalid max field in ColorSpace capx\n",
  "\nTest Case #  5:  Invalid dither field in ColorSpace capx\n",
  "\nTest Case #  6:  Reserved field = 99 in capx (should be ignored)\n",
  "\nTest Case #  7:  Invalid mult fields in ColorSpace capx\n",
  "\nTest Case #  8:  Invalid weight fields in ColorSpace capx (should ignore)\n",
  "\nTest Case #  9:  Invalid base_pixel field in ColorSpace capx\n",
  "\nTest Case # 10:  Valid ColorRange input capx\n",
  "\nTest Case # 11:  Invalid max  fields in ColorRange capx\n",
  "\nTest Case # 12:  Invalid mult fields in ColorRange capx\n"
  };

PEXColorApproxEntry sample_capx[NUM_CAPX_TEST_CASES]  = 
			       {{PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 7, 7, 3,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 32, 4, 1,                   /* mult1,2,3     */
				 0.0, 0.0, 0.0,              /* weight1,2,3   */
				  0},                        /* base_pixel    */

                                {99,                         /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 36, 6, 1,                   /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 99,                         /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 36, 6, 1,                   /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 99,                   /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 36, 6, 1,                   /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 99,                         /* dither        */
				 0,                          /* reserved      */
				 36, 6, 1,                   /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 7, 7, 3,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 99,                         /* reserved      */
				 32, 4, 1,                   /* mult1,2,3     */
				 0.0, 0.0, 0.0,              /* weight1,2,3   */
				  0},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 36, 6, 99,                  /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 7, 7, 3,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 32, 4, 1,                   /* mult1,2,3     */
				 99, 99, 99,                 /* weight1,2,3   */
				  0},                        /* base_pixel    */

                                {PEXColorSpace,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 36, 6, 1,                   /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 99},                        /* base_pixel    */

			        {PEXColorRange,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 100, 0, 0,                  /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 1, 0, 0,                    /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

			        {PEXColorRange,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 999, 1, 1,                  /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 1, 0, 0,                    /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40},                        /* base_pixel    */

			        {PEXColorRange,              /* type          */
				 PEXColorApproxRGB,          /* model         */
				 5, 5, 5,                    /* max1,2,3      */
				 PEXOn,                      /* dither        */
				 0,                          /* reserved      */
				 99, 99, 99,                 /* mult1,2,3     */
				 1.0, 0.0, 0.0,              /* weight1,2,3   */
				 40}};                       /* base_pixel    */

int capx_query_test(Display *dpy, Window window)
    {
    int i, j;
    PEXColorApproxEntry *capx;

    for (i=0; i<NUM_CAPX_TEST_CASES; i++)
	{
        capx_data.drawable = window;
        capx_data.capx     = sample_capx[i];

        capx_reply_data = ((PEXEscapeQueryColorApproxReplyData *) 
			    PEXEscapeWithReply(dpy,
				               PEXEscapeQueryColorApprox,
				               sizeof(capx_data),
			            ((char *) &capx_data), 
                           ((unsigned long *) &capx_reply_data_size)));

	_hppex_stderr_print("%s", test_descriptions[i]);
	_hppex_stderr_print("  capx_is_supported      = %d\n", 
		(int) capx_reply_data->capx_is_supported);
	_hppex_stderr_print("  all_capxs              = %d\n", 
		(int) capx_reply_data->all_capxs);
	_hppex_stderr_print("  count                  = %d\n", 
		(int) capx_reply_data->count);

        capx = (PEXColorApproxEntry *) 
		((int) capx_reply_data + sizeof(*capx_reply_data));

	for (j=0; j<capx_reply_data->count; j++)
	    {
	    _hppex_stderr_print("Returned Color Approx Entry #%d\n", (j+1));
	    _hppex_stderr_print("    type                 = %d\n", 
		   (int) capx->type);
	    _hppex_stderr_print("    model                = %d\n", 
		   (int) capx->model);
	    _hppex_stderr_print("    max1                 = %d\n", 
		   (int) capx->max1);
	    _hppex_stderr_print("    max2                 = %d\n", 
		   (int) capx->max2);
	    _hppex_stderr_print("    max3                 = %d\n", 
		   (int) capx->max3);
	    _hppex_stderr_print("    dither               = %d\n", 
		   (int) capx->dither);
	    _hppex_stderr_print("    mult1                = %d\n", 
		   (int) capx->mult1);
	    _hppex_stderr_print("    mult2                = %d\n", 
		   (int) capx->mult2);
	    _hppex_stderr_print("    mult3                = %d\n", 
		   (int) capx->mult3);
	    _hppex_stderr_print("    weight1              = %d\n", 
		   (int) capx->weight1);
	    _hppex_stderr_print("    weight2              = %d\n", 
		   (int) capx->weight2);
	    _hppex_stderr_print("    weight3              = %d\n", 
		   (int) capx->weight3);
	    _hppex_stderr_print("    base_pixel           = %d\n", 
		   (int) capx->base_pixel);
	    }
	}
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "lut_inq_capx");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "lut_inq_capx");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_window = window;
    _hppex_set_stderr("lut_inq_capx.stderr", "lut_inq_capx");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test( "\nPEXlib Inquire Color Approx Escape Tests:\n\n" );
    describe_test( "This test invokes the PEXQueryColorApprox escape with\n");
    describe_test( "both valid and invalid input color approximation entries.\n");
    describe_test( "The stderr output consists of the ruturned status and color\n");
    describe_test( "approximation entries.\n\n");
    describe_test( "This output is VERY vendor dependant and should be examined carefully.\n\n");
    describe_test( "The individual sub-tests are:\n");
    describe_test( "Test Case #  1:  Valid ColorSpace input capx\n");
    describe_test( "Test Case #  2:  Invalid type field in capx\n");
    describe_test( "Test Case #  3:  Invalid model field in ColorSpace capx\n");
    describe_test( "Test Case #  4:  Invalid max field in ColorSpace capx\n");
    describe_test( "Test Case #  5:  Invalid dither field in ColorSpace capx\n");
    describe_test( "Test Case #  6:  Reserved field = 99 in capx (should be ignored)\n");
    describe_test( "Test Case #  7:  Invalid mult fields in ColorSpace capx\n");
    describe_test( "Test Case #  8:  Invalid weight fields in ColorSpace capx (should ignore)\n");
    describe_test( "Test Case #  9:  Invalid base_pixel field in ColorSpace capx\n");
    describe_test( "Test Case # 10:  Valid ColorRange input capx\n");
    describe_test( "Test Case # 11:  Invalid max  fields in ColorRange capx\n");
    describe_test( "Test Case # 12:  Invalid mult fields in ColorRange capx\n");
    fflush(stdout);

    capx_query_test(dpy, local_window);
    _hppex_close_stderr();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("lut_inq_capx.stderr", "lut_inq_capx");
    }
