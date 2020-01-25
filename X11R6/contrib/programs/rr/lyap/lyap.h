/*
 *	@(#) lyap.h 9.5 93/09/28 SCOINC
 */
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

/*
 *	Written by Ron Record (rr@sco.com) 03 Sep 1991.
 */

static char *lyap_h_id = "@(#) lyap.h 9.5 93/09/28 SCOINC";

#include "patchlevel.h"
#include "libXrr.h"
#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>
#include <X11/Xlib.h> 
#include <X11/StringDefs.h> 
#include <X11/keysym.h> 
#include <X11/cursorfont.h> 
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#define ABS(a)	(((a)<0) ? (0-(a)) : (a) )
#define Min(x,y) ((x < y)?x:y)
#define Max(x,y) ((x > y)?x:y)

/* Useful mathematical constants that should have been defined in math.h 
 * M_LOG2E	- log2(e)
 * M_LN2        - ln(2)
 * M_PI		- pi
 */
#ifndef M_LOG2E
#define M_LOG2E	1.4426950408889634074
#endif
#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif
#ifndef M_LN2
#define M_LN2      6.9314718055994530942E-1 /*Hex  2^-1 * 1.62E42FEFA39EF */
#endif

/* Useful machine-dependent values that should have been defined in values.h
 * LN_MAXDOUBLE - the natural log of the largest double  -- log(MAXDOUBLE)
 * LN_MINDOUBLE - the natural log of the smallest double -- log(MINDOUBLE)
 */
#ifndef LN_MINDOUBLE
#define LN_MINDOUBLE (M_LN2 * (DMINEXP - 1))
#endif
#ifndef LN_MAXDOUBLE
#define LN_MAXDOUBLE (M_LN2 * DMAXEXP)
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define MAXFRAMES 2

int maxcolor=256, startcolor=17, color_offset=96, mincolindex=33;
int dwell=100, settle=50;
int width=256, height=256, xposition=-1, yposition=-1;
short delay=0;
int bottom, screen;
Display*	dpy;
char*		displayname = 0;
FILE *infile;

extern double log();
extern double fabs();
extern long time();
extern int optind;
extern char *optarg;

unsigned long foreground, background;

Window canvas, help, info;

void     resize();
void     redisplay();
void     quit();
void     Spin();
void     start_iterate();
void     stop_iterate();
void	 show_defaults();
void	 StartRubberBand();
void	 TrackRubberBand();
void	 EndRubberBand();
void	 CreateXorGC();
void	 InitBuffer();
void	 BufferPoint();
void	 FlushBuffer();

typedef struct {
	int x, y;
} xy_t;

typedef struct {
	int start_x, start_y;
	int last_x, last_y;
	} rubber_band_data_t;

typedef struct {
	Cursor band_cursor;
	double p_min, p_max, q_min, q_max;
	rubber_band_data_t rubber_band;
	} image_data_t;

points_t Points; /* typedef in ../lib/libXrr.h */
image_data_t rubber_data;

GC Data_GC[MAXCOLOR], RubberGC;

#define MAXINDEX 64
#define FUNCMAXINDEX 16
#define NUMMAPS 5

typedef double (*PFD)();

double logistic(), circle(), leftlog(), rightlog(), doublelog();
double dlogistic(), dcircle(), dleftlog(), drightlog(), ddoublelog();
PFD map, deriv;
PFD Maps[NUMMAPS] = { logistic, circle, leftlog, rightlog, doublelog };
PFD Derivs[NUMMAPS] = { dlogistic, dcircle, dleftlog, drightlog, ddoublelog };

int aflag=0, bflag=0, wflag=0, hflag=0, Rflag=0;
double pmins[NUMMAPS] = { 2.0, 0.0, 0.0, 0.0, 0.0 };
double pmaxs[NUMMAPS] = { 4.0, 1.0, 6.75, 6.75, 16.0 };
double amins[NUMMAPS] = { 2.0, 0.0, 0.0, 0.0, 0.0 };
double aranges[NUMMAPS] = { 2.0, 1.0, 6.75, 6.75, 16.0 };
double bmins[NUMMAPS] = { 2.0, 0.0, 0.0, 0.0, 0.0 };
double branges[NUMMAPS] = { 2.0, 1.0, 6.75, 6.75, 16.0 };

int   forcing[MAXINDEX] = { 0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
			0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
			0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1 };
int   Forcing[FUNCMAXINDEX] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int   maxindex = MAXINDEX;
int   funcmaxindex = FUNCMAXINDEX;
double	min_a=2.0, min_b=2.0, a_range=2.0, b_range=2.0, minlyap=1.0;
double  max_a=4.0, max_b=4.0;
double  start_x=0.5, lyapunov, a_inc, b_inc, a, b;
int	numcolors=16, numfreecols, displayplanes, lowrange;
xy_t	point, velocity;
Pixmap  pixmap;
Colormap cmap;
XColor	Colors[MAXCOLOR];
double  *exponents[MAXFRAMES];
double  a_minimums[MAXFRAMES], b_minimums[MAXFRAMES];
double  a_maximums[MAXFRAMES], b_maximums[MAXFRAMES];
double  minexp, maxexp, prob=0.5;
int     expind[MAXFRAMES]={0}, resized[MAXFRAMES]={0};
int	numwheels=MAXWHEELS, force=0, Force=0, negative=1;
int     nostart=1, stripe_interval=7;
int	save=1, show=0, useprod=1, savefile=0, restfile=0, storefile=0;
int	maxframe=0, frame=0, dorecalc=0, mapindex=0, run=1, demo=0;
char	*outname="lyap.ppm";
char	*inname="lyap.sav";
char	*savname="lyap.sav";
