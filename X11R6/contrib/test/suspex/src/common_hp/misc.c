/* $XConsortium: misc.c,v 1.0 93/11/22 12:22:11 rws Exp $ */

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
#include <stdlib.h>

#include <misc.h>

#ifdef XSYSTEM
int glob_verbose_mode  = 1;
int glob_window_width  = 400;
int glob_window_height = 400;
int glob_window_x      = 800;
int glob_window_y      = 50;

#else
int glob_verbose_mode  = 0;
int glob_window_width  = 200;
int glob_window_height = 200;
int glob_window_x      = 0;
int glob_window_y      = 0;

#endif

char  img_desc[10000];  /* for a description of each image */

int glob_test_mode;

void describe_test(char *text)
    {
    strcat(img_desc,text);
    if (glob_verbose_mode)
	fprintf(stdout, text);
    }

extern char *optarg;            /* for getopt */
extern int optind, opterr;

int parse_options (int argc, char **argv, int *pexmode, int *image_depth)
{
    int			c;
    char		dummy[2];

#ifndef XSYSTEM
    /* process command line options */
    while ((c = getopt(argc,argv,"d:inpsvw")) != EOF) {
        switch(c) {
            case 'd':
                *image_depth = atoi(optarg);
                break;
            case 'i':
                *pexmode |= IMM_MODE;
                break;
            case 'n':
                *pexmode |= NON_OC_MODE;
                break;
            case 'p':
                *pexmode |= WKS_MODE;
                break;
            case 's':
                *pexmode |= STR_MODE;
                break;
            case 'v':
                glob_verbose_mode = 1;
                break;
	    case 'w':	/* wait for messing around with debugger :-) */
	        printf("Test waiting for <CR>: ");
	        scanf("%c", dummy);
		printf("-continuing\n");
	        break;
	    case '?':
	        printf("usage: %s: -[ipsvw]\n",argv[0]);
	        printf("-i		immediate mode\n");
	        printf("-n		non-oc mode\n");
	        printf("-p		PHIGS workstation mode\n");
	        printf("-s		structure rendering mode\n");
	        printf("-v		verbose mode (test descriptions)\n");
	        printf("-w		wait for <CR> before continuing\n");
	        printf("default	immediate mode\n");
	        return(1);
	}
    }
#endif

    return 0;
}
