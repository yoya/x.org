/* $XConsortium: err_handler.h,v 1.0 93/11/22 12:21:59 rws Exp $ */

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

#ifndef ERR_HANDLER_H
#define ERR_HANDLER_H

#include "stdio.h"

#define NO_ERR 0
#define X_ERR 1
#define PEX_ERR 2

#define ERROR_CODE   0x1
#define REQUEST_CODE 0x2
#define SUB_CODE     0x4
#define OC_CODE      0x8

/*
 * Error handler initialization routine
 *
 * This routine installs the testing error handler.
 * This error handler does not exit on an error. It can either
 * trap errors or print out a terse description of the errors
 * it encounters.
 */
extern void _hppex_test_init_error_handler (void);

extern void _hppex_test_set_expected_error (int error_type,
                                            int error_code,
                                            int request_code,
                                            int sub_code,
                                            int oc_code);

extern int _hppex_test_check_expected_error (unsigned long mask);

extern FILE *_hppex_set_err_file(char *err_file, char *test_name);

#endif
