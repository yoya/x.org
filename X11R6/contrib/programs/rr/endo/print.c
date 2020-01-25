/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1992, 1993 Ronald Joe Record                           *
 *                                                                       *
 *      Ronald Joe Record (408) 458-3718                                 *
 *      212 Owen St., Santa Cruz, California 95062 USA                   *
 *                                                                       *
 *************************************************************************/
 /*
 *  The X Consortium, and any party obtaining a copy of these files from
 *  the X Consortium, directly or indirectly, is granted, free of charge,
 *  a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *  nonexclusive right and license to deal in this software and
 *  documentation files (the "Software"), including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons who receive
 *  copies from any such party to do so.  This license includes without
 *  limitation a license to do the foregoing actions under any patents of
 *  the party supplying this software to the X Consortium.
 */

#include <stdio.h>
#include "x.h"

void
usage()
{
    printf("endo [-Vpuvciy][-W#][-H#][-a#][-b#][-w#][-h#][-S#][-n#]\n");
    printf("\t[-ABCDEFGIJKLM #][-N#][-X#][-Y#][-O#]\n");
    printf("\t[-l# #][-m#][-o filename][-r#][-s#][-x#][-z#]\n");
    printf("\tWhere: -V turns off period detection\n");
    printf("\t       -p turns on phase portrait display\n");
    printf("\t       -u displays this message\n");
    printf("\t       -v displays parameters that would be used\n");
    printf("\t       -c turns off display of critical curves\n");
    printf("\t       -i specifies use of random initial conditions\n");
    printf("\t       -y turns on attractor detection\n");
    printf("\t       -a# specifies the minimum horizontal value\n");
    printf("\t       -b# specifies the minimum vertical value\n");
    printf("\t       -w# specifies the horizontal value range\n");
    printf("\t       -h# specifies the vertical value range\n");
    printf("\t       -n# specifies the dwell\n");
    printf("\t       -S# specifies the settle\n");
    printf("\t       -H# specifies the initial window height\n");
    printf("\t       -W# specifies the initial window width\n");
    printf("\t       -ABCDEFGIJKLM# specifies the parameter values\n");
    printf("\t       -N# specifies the maximum color index\n");
    printf("\t       -XY# specifies the (x,y) value to iterate\n");
    printf("\t       -O# specifies the color offset\n");
    printf("\t       -l# # specifies the two parameters to alter\n");
    printf("\t       -m# specifies the desired map (0-6)\n");
    printf("\t       -o filename specifies the output file name\n");
    printf("\t       -r# specifies the maximum rgb value\n");
    printf("\t       -s# specifies the spin length\n");
    printf("\t       -x# specifies the delta used in period detection\n");
    printf("\t\t-z# specifies the delta used in critical curve detection\n");
    printf("\tDuring display :\n");
    printf("\t     Use the left mouse button to zoom in\n");
    printf("\t     Use the middle mouse button to zoom out\n");
    printf("\t     h or H or ? displays a help window\n");
    printf("\t     q or Q exits\n");
    exit(1);
}

void
show_defaults() 
{

	printf("Width=%d  Height=%d  numcolors=%d  settle=%d  dwell=%d\n",
		width,height,numcolors,settle,dwell);
	printf("min_x=%lf  x_range=%lf  max_x=%lf\n", min_x,x_range,max_x);
	printf("min_y=%lf  y_range=%lf  max_y=%lf\n", min_y,y_range,max_y);
	printf("minperiod=%d  maxperiod=%d\n", minperiod,maxperiod);
	exit(0);
}
