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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "libXrr.h"

int _numdivs = 2;
static char mapfile[128];

void
set_mapfile_name(display, res, prog)
Display *display;
char *res;
char *prog;
{
	char *tmpmapfile;

    if ((tmpmapfile = XGetDefault(display, res, "Mapfile")) == NULL) {
       	if (strcpy(mapfile, getenv("HOME")) == NULL)
			fprintf(stderr, "Warning: strcpy of HOME failed\n");
       	if (strcat(mapfile, "/.") == NULL)
			fprintf(stderr, "Warning: strcat of /. failed\n");
       	if (strcat(mapfile, prog) == NULL)
			fprintf(stderr, "Warning: strcat of prog failed\n");
       	if (strcat(mapfile, "map") == NULL)
			fprintf(stderr, "Warning: strcat of map failed\n");
    }
	else
		if (strcpy(mapfile, tmpmapfile) == NULL)
			fprintf(stderr, "Warning: strcpy of tmpmapfile failed\n");
}

void
init_color(display, win, cmap, Colors, s_col, min, n_cols, wheel, prog, res, n)
Display *display;
Window win;
Colormap cmap;
XColor *Colors;
int s_col, min, n_cols, wheel, n;
char *prog;
char *res;
{
    static short i, j, first_time=1;
    static int colgap, leg, step, numfree;
	static double d;
    Colormap def_cmap;
    int hls[3], rgb[3];
	FILE *map;
	char buf[255];
	int a, b, c, c_end;
    extern void hls2rgb();
	extern long lrand48();

    def_cmap = DefaultColormap(display, DefaultScreen(display));
    for (i=0; i<n_cols; i++) {
        Colors[i].pixel = i;
        Colors[i].flags = DoRed|DoGreen|DoBlue;
    }
    XQueryColors(display, def_cmap, Colors, n_cols);
    colgap = RGB_MAX / n_cols;
    hls[0] = 50;    /* Hue in low range */
    hls[2] = 1000;  /* Fully saturated */
    for (i=s_col; i<min; i++) {
        hls[1] = 1000L * (i-s_col) / (min - s_col);
        hls2rgb(hls, rgb);
        Colors[i].red = rgb[0];
        Colors[i].green = rgb[1];
        Colors[i].blue = rgb[2];
    }
    numfree = n_cols - min;
	switch (wheel) {
	case -2: /* black & white palette */
		hls[1] = 1000;	/* Fully Light */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<((n_cols-s_col)/2)+s_col; i++) {
			hls[0] = 3000L;
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		hls[1] = 0;	/* Fully Dark */
		hls[2] = 0;
		for (i=((n_cols-s_col)/2)+s_col; i<n_cols; i++) {
			hls[0] = 0;
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
	case -1:	/* white palette */
		hls[1] = 1000;	/* Fully Light */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<n_cols; i++) {
			hls[0] = 3000L;
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
    case 0: /* read in cmap from $HOME/.<progname>map */
		if (first_time) {
			set_mapfile_name(display, res, prog);
			first_time = 0;
		}
        i = s_col;
        if ((map = fopen(mapfile, "r")) != NULL) {
            while(!feof(map) && i <n_cols) {
                fgets(buf,sizeof buf,map);
                if(sscanf(buf,"%d %d %d",&a,&b,&c) == 3) {
                    Colors[i].red = a * 256;
                    Colors[i].green = b * 256;
                    Colors[i].blue = c * 256;
                    i++;
					if (i >= n_cols)
						break;
                }
            }
            c_end = i;
            while (i<n_cols) {
                Colors[i].red = Colors[i - c_end].red;
                Colors[i].green = Colors[i - c_end].green;
                Colors[i].blue = Colors[i - c_end].blue;
                i++;
            }
        }
        fclose(map);
    	break;
	case 1: /* gray-scale palette */
        for (i=s_col; i<n_cols; i++) {
            Colors[i].red = ((i-s_col)*60000/(n_cols-s_col)) + 5535;
            Colors[i].green = ((i-s_col)*60000/(n_cols-s_col)) + 5535;
            Colors[i].blue = ((i-s_col)*60000/(n_cols-s_col)) + 5535;
        }
    	break;
	case 2:
        hls[0] = 800;    /* Hue in mid range */
        hls[2] = 1000;    /* Fully saturated */
        for (i=s_col; i<min; i++) {
            hls[1]=1000L*(i-s_col) / (min - s_col);
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
        for (i=min; i<min+(numfree/2); i++) {
            Colors[i].blue = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].red=(i*2*RGB_MAX/n_cols);
        }
        for (i=min+(numfree/2); i<n_cols; i++) {
            Colors[i].blue = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].red=((n_cols - i)*2*RGB_MAX/n_cols);
        }
    	break;
	case 3:
        hls[0] = 800;    /* Hue in mid range */
        hls[2] = 1000;    /* Fully saturated */
        for (i=s_col; i<min; i++) {
            hls[1]=1000L*(i-s_col) / (min - s_col);
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
        colgap = 4*RGB_MAX/n_cols;
        for (i=min; i<min+(numfree/4); i++) {
            Colors[i].blue = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].red=(i*colgap);
        }
        for (i=min+(numfree/4);i<min+(numfree/2);i++) {
            Colors[i].red = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].blue=(min+(numfree/2) - i) * colgap;
        }
        for(i=min+(numfree/2);i<min+(0.75*numfree);i++){
            Colors[i].red = RGB_MAX;
            Colors[i].blue=(i * colgap);
            Colors[i].green = 0;
        }
        for (i=min+(0.75*numfree); i<n_cols; i++) {
            Colors[i].blue = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].red=(n_cols-i)*colgap;
        }
    	break;
	case 4:
        hls[0] = 800;    /* Hue in mid range */
        hls[2] = 1000;    /* Fully saturated */
        for (i=s_col; i<min; i++) {
            hls[1] = 1000L * (i-s_col) / (min - s_col);
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
        colgap = wheel * RGB_MAX / n_cols;
        for (i=min; i<(n_cols/wheel); i++) {
            Colors[i].blue = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].red=(i*colgap);
        }
        for (i=(n_cols/wheel); i<(2*n_cols/wheel); i++) {
            Colors[i].red = RGB_MAX;
            Colors[i].green = 0;
            Colors[i].blue=((2*n_cols/wheel) - i) * colgap;
        }
        for (i=(2*n_cols/wheel); i<n_cols; i++) {
            Colors[i].red = RGB_MAX;
            Colors[i].green=(i - (2*n_cols/wheel)) * colgap;
            Colors[i].blue = 0;
        }
    	break;
	case 5:
        hls[1] = 700;    /* Lightness in midrange */
        hls[2] = 1000;    /* Fully saturated */
        for (i=min; i<n_cols; i++) {
            hls[0] = 3600L * i / n_cols;
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
        for (i=min; i<n_cols; i+=((n_cols-min)/10)) {
            hls[0] = 3600L * i / n_cols;
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0] / 2;
            Colors[i].green = rgb[1] / 2;
            Colors[i].blue = rgb[2] / 2;
        }
    	break;
	case 6:
        hls[0] = 800;    /* Hue in mid range */
        hls[2] = 1000;    /* Fully saturated */
        for (i=s_col; i<min; i++) {
            hls[1] = 1000L * (i-s_col) / (min - s_col);
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
        step = numfree / 3;
        leg = step+min;
        for (i = min; i < leg; ++i) {
            Colors[i].pixel = i;
            Colors[i].red = fabs(65535 - (double)i / step * 65535.0);
            Colors[i].blue = (double)i / step * 65535.0;
            Colors[i].green = 0;
            Colors[i].flags = DoRed | DoGreen | DoBlue;
        }
        for (j = 0, i = leg, leg += step; i < leg; ++i, ++j) {
            Colors[i].pixel = i;
            Colors[i].red = (double)j / step * 65535.0;
            Colors[i].blue = 65535;
            Colors[i].green = Colors[i].red;
            Colors[i].flags = DoRed | DoGreen | DoBlue;
        }
        for (j = 0, i = leg, leg += step; i < leg; ++i, ++j) {
            Colors[i].pixel = i;
            Colors[i].red = 65535;
            Colors[i].blue = fabs(65535 - (double)j / step * 65535.0);
            Colors[i].green = Colors[i].blue;
            Colors[i].flags = DoRed | DoGreen | DoBlue;
        }
    	break;
	case 7: /* random palette */
        hls[1] = 500;    /* Lightness in midrange */
        hls[2] = 1000;    /* Fully saturated */
        for (i=min; i<n_cols; i++) {
            hls[0] = 3600L * (lrand48() % n_cols) / n_cols;
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
    	break;
	/* wheel=8 indicates the default color map which is already queried */
	case 9: /* red, green, blue for 3 basins */
		hls[0] = 60;   /* hue at low end */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<((n_cols-s_col)/3)+s_col; i++) {
			hls[1] = 800L * i / (((n_cols-s_col)/3)+s_col);
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		hls[0] = 800;   /* hue in middle */
		for (i=((n_cols-s_col)/3)+s_col;i<(2*(n_cols-s_col)/3)+s_col;i++) {
			hls[1] = 800L * i / ((2*(n_cols-s_col)/3)+s_col);
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		hls[0] = 1600;   /* hue in upper range */
		for (i=(2*(n_cols-s_col)/3)+s_col; i<n_cols; i++) {
			hls[1] = 800L * i / n_cols;
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
	case 10: /* palette of several rainbows */
		hls[1] = 500;	/* Lightness in midrange */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<n_cols; i++) {
			hls[0] = 3000L*((i-s_col)%((n_cols-s_col)/_numdivs)) 
					/ ((n_cols-s_col)/_numdivs);
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
    case 11:
        colgap = 2*RGB_MAX/(n_cols - ((n_cols + min)/2));
        for (i=min; i<min+(numfree/2); i++) {
            Colors[i].blue = 0;
            Colors[i].green=((i+((n_cols + min)/2))*colgap);
            Colors[i].red=((i+((n_cols + min)/2))*colgap);
        }
        for (i=min+(numfree/2); i<n_cols; i++) {
            Colors[i].blue = 0;
            Colors[i].green=(((n_cols-i)+((n_cols + min)/2))*colgap);
            Colors[i].red=(((n_cols-i)+((n_cols + min)/2))*colgap);
        }
    	break;
	case 12: /* green colormap */
		hls[0] = 1200;	/* Green Hue */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<n_cols; i++) {
			hls[1] = (800L * (n_cols - i) / (n_cols - s_col)) + 100L;
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
	case 13: /* green "tent" shaped colormap */
		colgap = 2*RGB_MAX/(n_cols - ((n_cols + min)/2));
		for (i=s_col; i<s_col+((n_cols - s_col)/2); i++) {
			Colors[i].blue = 0;
			Colors[i].green=((i+((n_cols + min)/2))*colgap);
			Colors[i].red=((i+((n_cols + min)/2))*colgap);
		}
		for (i=s_col+((n_cols - s_col)/2); i<n_cols; i++) {
			Colors[i].blue = 0;
			Colors[i].green=(((n_cols-i)+((n_cols + min)/2))*colgap);
			Colors[i].red=(((n_cols-i)+((n_cols + min)/2))*colgap);
		}
		break;
	case 14: /* red, green, blue for n basins */
		hls[2] = 1000L;
		if (n) {
			d = (double)(n_cols - s_col)/(double)n;
			for (j=0; j<n; j++) {
				hls[0] = (j*2500L/n)+250L; /* hue */
				for (i=s_col+(j*d); i<((j+1)*d)+s_col; i++) {
					if (i >= n_cols)
						break;
					hls[1] = 250L + (500L * (double)(i-s_col-(j*d)) / d);
					hls2rgb(hls, rgb);
					Colors[i].red = rgb[0];
					Colors[i].green = rgb[1];
					Colors[i].blue = rgb[2];
				}
			}
		}
		else {
			hls[0] = 0L; /* hue */
			for (i=s_col; i<n_cols; i++) {
				hls[1] = 100L + (800L * (double)(i-s_col) / (n_cols - s_col));
				hls2rgb(hls, rgb);
				Colors[i].red = rgb[0];
				Colors[i].green = rgb[1];
				Colors[i].blue = rgb[2];
			}
		}
		break;
	case 15: /* reverse rainbow palette */
		hls[1] = 500;	/* Lightness in midrange */
		hls[2] = 1000;	/* Fully saturated */
		for (i=s_col; i<n_cols; i++) {
			hls[0] = 3000L * (i - s_col) / (n_cols - s_col);
			hls2rgb(hls, rgb);
			Colors[i].red = rgb[0];
			Colors[i].green = rgb[1];
			Colors[i].blue = rgb[2];
		}
		break;
	case 16: /* rainbow palette */
        hls[1] = 500;    /* Lightness in midrange */
        hls[2] = 1000;    /* Fully saturated */
        for (i=s_col; i<n_cols; i++) {
            hls[0] = 3000L * (i - s_col) / (n_cols - s_col);
            hls2rgb(hls, rgb);
            Colors[i].red = rgb[0];
            Colors[i].green = rgb[1];
            Colors[i].blue = rgb[2];
        }
    	break;
	}
    XStoreColors(display, cmap, Colors, n_cols);
	/* install new color map */
	XSetWindowColormap(display, win, cmap);
}

void
write_cmap(display, cmap, Colors, n_cols, prog, res)
Display *display;
Colormap cmap;
XColor *Colors;
int n_cols;
char *prog;
char *res;
{
    FILE *outfile;
	static int i;

	set_mapfile_name(display, res, prog);
	outfile = fopen(mapfile,"w");
	if (!outfile) {
		perror(mapfile);
		return;
	}
	else {
    	XQueryColors(display, cmap, Colors, n_cols);
		for (i=0; i<n_cols; i++)
			fprintf(outfile,"%d %d %d\n",
				Colors[i].red/256, Colors[i].green/256, Colors[i].blue/256);
	}
	fclose(outfile);
}
