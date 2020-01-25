/* $XConsortium: misc.h,v 1.0 93/11/22 12:22:17 rws Exp $ */

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

#ifndef _MISC_H
#define _MISC_H

#include <stdio.h>

extern int glob_verbose_mode;
extern int glob_window_width;
extern int glob_window_height;
extern int glob_window_x;
extern int glob_window_y;
extern int glob_test_mode;

extern char img_desc[];

extern FILE *suspex_open_file();
extern int   suspex_close_file();

extern int 
parse_options (int argc, char **argv, int *pexmode, int *image_depth);

#define BLACK        0
#define WHITE        1
#define RED          2
#define YELLOW       3
#define GREEN        4
#define CYAN         5
#define BLUE         6
#define MAGENTA      7

#define IMM_MODE     1
#define STR_MODE     2
#define WKS_MODE     4
#define NON_OC_MODE  8

#endif /* _MISC_H */
