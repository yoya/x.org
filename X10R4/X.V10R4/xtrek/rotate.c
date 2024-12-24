#ifndef lint
static char *rcsid_rotate_c = "$Header: rotate.c,v 10.1 86/11/30 15:28:23 jg Rel $";
#endif	lint
/* Copyright (c) 1986 	Chris Guthrie */

#include <stdio.h>
#include <math.h>



#define xx_width 16
#define xx_height 16

static short xx_bits[] = {
  0xfc3f, 0xf81f, 0xf00f, 0xf00f,
   0xf00f, 0xf81f, 0xfc3f, 0xf66f,
   0xf66f, 0xf66f, 0xf00f, 0xf66f,
   0xf66f, 0xf7ef, 0xf7ef, 0xf7ef};
int start[16][16];
int finish[16][16];

main()
{
    register int i, j;
    int bit;
    double deg = 3.14159 / 2;

    for (j = 0; j < 16; j++) {
	for (i = 0; i < 16; i++) {
		start[i][j] = (xx_bits[j] & 1);
		xx_bits[j] >>= 1;
	}
    }
    for (j = 0; j < 16; j++) {
	for (i = 0; i < 16; i++) {
	    if (start[i][j])
		finish[15-j][i] = 1;
	    else
		finish[15-j][i] = 0;
	}
    }
    fprintf(stderr, "#define xx_width 16\n");
    fprintf(stderr, "#define xx_height 16\n");
    fprintf(stderr, "static short xx_bits[] = {");
    for (j = 0; j < 16; j++) {
	bit = 0;
	for (i = 0; i < 16; i++) {
	    bit += (finish[i][j] << i);
	}
	if (!(j % 4))
	    fprintf(stderr, "\n    ", bit);
        fprintf(stderr, "0x%x, ", bit);
    }
    fprintf(stderr, "};\n");

    for (j = 0; j < 16; j++) {
	for (i = 0; i < 16; i++) {
	    if (finish[i][j])
		start[15-j][i] = 1;
	    else
		start[15-j][i] = 0;
	}
    }
    fprintf(stderr, "#define xx_width 16\n");
    fprintf(stderr, "#define xx_height 16\n");
    fprintf(stderr, "static short xx_bits[] = {");
    for (j = 0; j < 16; j++) {
	bit = 0;
	for (i = 0; i < 16; i++) {
	    bit += (start[i][j] << i);
	}
	if (!(j % 4))
	    fprintf(stderr, "\n    ", bit);
        fprintf(stderr, "0x%x, ", bit);
    }
    fprintf(stderr, "};\n");
    for (j = 0; j < 16; j++) {
	for (i = 0; i < 16; i++) {
	    if (start[i][j])
		finish[15-j][i] = 1;
	    else
		finish[15-j][i] = 0;
	}
    }
    fprintf(stderr, "#define xx_width 16\n");
    fprintf(stderr, "#define xx_height 16\n");
    fprintf(stderr, "static short xx_bits[] = {");
    for (j = 0; j < 16; j++) {
	bit = 0;
	for (i = 0; i < 16; i++) {
	    bit += (finish[i][j] << i);
	}
	if (!(j % 4))
	    fprintf(stderr, "\n    ", bit);
        fprintf(stderr, "0x%x, ", bit);
    }
    fprintf(stderr, "};\n");

}
