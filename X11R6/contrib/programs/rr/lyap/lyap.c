/*
 *	@(#) lyap.c 11.3 93/11/12 SCOINC
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

/* Lyap - calculate and display Lyapunov exponents */

/* Written by Ronald Record (rr@sco.com) 03 Sep 1991 */

/* The idea here is to calculate the Lyapunov exponent for a periodically
 * forced logistic map (later i added several other nonlinear maps of the unit
 * interval). In order to turn the 1-dimensional parameter space of the
 * logistic map into a 2-dimensional parameter space, select two parameter
 * values ('a' and 'b') then alternate the iterations of the logistic map using
 * first 'a' then 'b' as the parameter. This program accepts an argument to 
 * specify a forcing function, so instead of just alternating 'a' and 'b', you
 * can use 'a' as the parameter for say 6 iterations, then 'b' for 6 iterations
 * and so on. An interesting forcing function to look at is abbabaab (the
 * Morse-Thue sequence, an aperiodic self-similar, self-generating sequence).
 * Anyway, step through all the values of 'a' and 'b' in the ranges you want,
 * calculating the Lyapunov exponent for each pair of values. The exponent
 * is calculated by iterating out a ways (specified by the variable "settle")
 * then on subsequent iterations calculating an average of the logarithm of
 * the absolute value of the derivative at that point. Points in parameter
 * space with a negative Lyapunov exponent are colored one way (using the
 * value of the exponent to index into a color map) while points with a
 * non-negative exponent are colored differently. 
 * 
 * The algorithm was taken from the September 1991 Scientific American article
 * by A. K. Dewdney who gives credit to Mario Markus of the Max Planck Institute
 * for its creation. Additional information and ideas were gleaned from the
 * discussion on alt.fractals involving Stephen Hall, Ed Kubaitis, Dave Platt
 * and Baback Moghaddam. Assistance with colormaps and spinning color wheels
 * and X was gleaned from Hiram Clawson. Rubber banding code was adapted from
 * an existing Mandelbrot program written by Stacey Campbell.
 */

#include "lyap.h"

static char *lyap_c_id = "@(#) lyap.c 11.3 93/11/12 SCOINC";
static char *version = LYAP_VERSION;

extern void BufferPoint(), InitBuffer(), FlushBuffer();

void
Cleanup() {
	XCloseDisplay(dpy);
}

main(ac, av)
    int ac;
    char **av;
{
    int i, j; 
	int x_center=0, y_center=0;
    XSizeHints hint;
    extern void init_canvas(), init_data(), init_color(), parseargs();
    extern void Clear(), restor_picture(); 
	extern void center_horizontal(), center_vertical();

    parseargs(ac, av);
    dpy = XOpenDisplay("");
    screen = DefaultScreen(dpy);
    background = BlackPixel(dpy, screen);
	if (width < 1)
		width = XDisplayWidth(dpy, screen);
	if (height < 1)
		height = XDisplayHeight(dpy, screen);
    setupmem();
    init_data();
    if (displayplanes > 1)
        foreground = startcolor;
    else
        foreground = WhitePixel(dpy,XDefaultScreen(dpy));
	if (xposition < 0) {
		x_center = 1;
		xposition = Max(0,(XDisplayWidth(dpy, screen) - width)/2);
	}
	if (yposition < 0) {
		y_center = 1;
		yposition = Max(0,(XDisplayHeight(dpy, screen) - height)/2);
	}
    hint.x = xposition;
    hint.y = yposition;
    hint.width = width;
    hint.height = height;
    hint.flags = PPosition | PSize;
    /*
     * Create the window to display the Lyapunov exponents 
     */
    canvas = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
        hint.x, hint.y, hint.width, hint.height,
        5, foreground, background);
    XSetStandardProperties(dpy, canvas, "Lyap by Ron Record", 
        "Lyap", None, av, ac, &hint);
    XChangeProperty( dpy, canvas, XA_WM_CLASS, XA_STRING, 8, PropModeReplace, 
					"lyap", strlen("lyap"));
    hint.x = 0;
    hint.y = XDisplayHeight(dpy, screen) / 2;
    hint.width = XDisplayWidth(dpy, screen) / 3;
    hint.height = hint.y / 2;
    info = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            	hint.x, hint.y, hint.width, hint.height, 5, 0, 1);
	XSetWindowBackground(dpy, info, BlackPixel(dpy, screen));
    /* Title */
    XSetStandardProperties(dpy,info,"Info","Info",None,av,ac,&hint);
    hint.x = XDisplayWidth(dpy, screen) / 2;
    hint.y = 0;
    hint.width = hint.x;
    hint.height = 4 * XDisplayHeight(dpy, screen) / 6;
    help = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            	hint.x, hint.y, hint.width, hint.height, 5, 0, 1);
	XSetWindowBackground(dpy, help, BlackPixel(dpy, screen));
    /* Title */
    XSetStandardProperties(dpy,help,"Help","Help",None,av,ac,&hint);
    init_canvas();
    XMapRaised(dpy, canvas);
	if (x_center)
		center_horizontal(dpy, canvas, width);
	if (y_center)
		center_vertical(dpy, canvas, height);
    /* Try to write into a new color map */
    cmap = XCreateColormap(dpy, canvas, DefaultVisual(dpy, screen), AllocAll);
    if (displayplanes > 1)
        init_color(dpy, canvas, cmap, Colors, startcolor, mincolindex, 
					numcolors, numwheels, "lyap", "Lyap", 0);
    else
        XQueryColors(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), 
                Colors, numcolors);
    /* install new color map */
    XSetWindowColormap(dpy, canvas, cmap);
    XSetWindowColormap(dpy, help, cmap);
    XSetWindowColormap(dpy, info, cmap);
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), width, height, 
                           DefaultDepth(dpy, screen));
    rubber_data.band_cursor=XCreateFontCursor(dpy, XC_hand2);
    CreateXorGC();
    Clear();
	if (restfile)
		restor_picture();
    if (demo)
        for(;;) {
            if (!run) {
				DemoSpin(dpy, cmap, Colors, startcolor, maxcolor, delay, 2);
				for (i=0; i<=MAXWHEELS; i++) {
					init_color(dpy, canvas, cmap, Colors, startcolor, 
							mincolindex, numcolors, i, "lyap", "Lyap", 0);
					sleep(1);
				}
                if (!run) {
                    Cleanup();
                    exit(0);
                } 
            }
        }
    else {
    	XSelectInput(dpy,help,KeyPressMask|ExposureMask);
    	XSelectInput(dpy,info,KeyPressMask|ExposureMask);
    	XSelectInput(dpy,canvas,KeyPressMask|ButtonPressMask|ButtonMotionMask|
        		ButtonReleaseMask|ExposureMask|StructureNotifyMask);
        for(;;)
            main_event();
	}
}

main_event()
{
    int n;
    XEvent event;

    if (complyap() == TRUE)
        run=0;
    n = XEventsQueued(dpy, QueuedAfterFlush);
    while (n--) {
        XNextEvent(dpy, &event);
        switch(event.type) {
            case KeyPress: Getkey(&event); break;
            case Expose: redisplay(canvas, &event); break;
            case ConfigureNotify: resize(); break;
            case ButtonPress: StartRubberBand(canvas, &rubber_data, &event);
                 break;
            case MotionNotify: TrackRubberBand(canvas, &rubber_data, &event);
                 break;
            case ButtonRelease: EndRubberBand(canvas, &rubber_data, &event);
                 break;
        }
    }
}

/* complyap() is the guts of the program. This is where the Lyapunov exponent
 * is calculated. For each iteration (past some large number of iterations)
 * calculate the logarithm of the absolute value of the derivative at that
 * point. Then average them over some large number of iterations. Some small
 * speed up is achieved by utilizing the fact that log(a*b) = log(a) + log(b).
 */
complyap()
{
    register i, bindex, findex;
    double total, prod, x, r;
    extern void save_to_file(), store_to_file();

    if (!run)
        return TRUE;
    if (a >= max_a)
        if (sendpoint(lyapunov) == TRUE)
            return FALSE;
        else {
			run=0;
            FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
            if (savefile)
                save_to_file();
			else if (storefile)
                store_to_file();
            return TRUE;
        }
    if (b >= max_b) {
		run=0;
        FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
        if (savefile)
            save_to_file();
		else if (storefile)
            store_to_file();
        return TRUE;
    }
    prod = 1.0;
    total = 0.0;
    bindex = 0;
    x = start_x;
    r = (forcing[bindex]) ? b : a;
#ifdef MAPS
    findex = 0;
    map = Maps[Forcing[findex]];
#endif
    for (i=0;i<settle;i++) {       /* Here's where we let the thing */
        x = (*map)(x, r);       /* "settle down". There is usually */
        if (++bindex >= maxindex) { /* some initial "noise" in the */
            bindex = 0;       /* iterations. How can we optimize */
            if (Rflag)        /* the value of settle ??? */
                setforcing();
        }
        r = (forcing[bindex]) ? b : a;
#ifdef MAPS
        if (++findex >= funcmaxindex)
            findex = 0;
        map = Maps[Forcing[findex]];
#endif
    }
#ifdef MAPS
    deriv = Derivs[Forcing[findex]];
#endif
    if (useprod) {            /* using log(a*b) */
        for (i=0;i<dwell;i++) {
            x = (*map)(x, r);
            prod *= ABS((*deriv)(x, r));
            /* we need to prevent overflow and underflow */
            if ((prod > 1.0e12) || (prod < 1.0e-12)) {
                if (prod == 0.0)
                    total += LN_MINDOUBLE;
                else if (prod < 0.0)
                    total += LN_MAXDOUBLE;
                else
                    total += log(prod);
                prod = 1.0;
            }
            if (++bindex >= maxindex) {
                bindex = 0;
                if (Rflag)
                    setforcing();
            }
            r = (forcing[bindex]) ? b : a;
#ifdef MAPS
            if (++findex >= funcmaxindex)
                findex = 0;
            map = Maps[Forcing[findex]];
            deriv = Derivs[Forcing[findex]];
#endif
        }
        if (prod == 0.0)
            total += LN_MINDOUBLE;
        else if (prod < 0.0)
            total += LN_MAXDOUBLE;
        else
            total += log(prod);
        lyapunov = (total * M_LOG2E) / (double)dwell;
    }
    else {                /* use log(a) + log(b) */
        for (i=0;i<dwell;i++) {
            x = (*map)(x, r);
            total += log(ABS((*deriv)(x, r)));
            if (++bindex >= maxindex) {
                bindex = 0;
                if (Rflag)
                    setforcing();
            }
            r = (forcing[bindex]) ? b : a;
#ifdef MAPS
            if (++findex >= funcmaxindex)
                findex = 0;
            map = Maps[Forcing[findex]];
            deriv = Derivs[Forcing[findex]];
#endif
        }
        lyapunov = (total * M_LOG2E) / (double)dwell;
    }
    if (sendpoint(lyapunov) == TRUE)
        return FALSE;
    else {
		run=0;
        FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
        if (savefile)
            save_to_file();
		else if (storefile)
            store_to_file();
        return TRUE;
    }
}

double
logistic(x, r)            /* the familiar logistic map */
double x, r;
{
    return(r * x * (1.0 - x));
}

double
dlogistic(x, r)            /* the derivative of logistic map */
double x, r;
{
    return(r - (2.0 * r * x));
}

double
circle(x, r)            /* sin() hump or sorta like the circle map */
double x, r;
{
    extern double sin();

    return(r * sin(M_PI * x));
}

double
dcircle(x, r)            /* derivative of the "sin() hump" */
double x, r;
{
    extern double cos();

    return(r * M_PI * cos(M_PI * x));
}

double
leftlog(x, r)            /* left skewed logistic */
double x, r;
{
    double d;

    d = 1.0 - x;
    return(r * x * d * d);
}

double
dleftlog(x, r)            /* derivative of the left skewed logistic */
double x, r;
{
    return(r * (1.0 - (4.0 * x) + (3.0 * x * x)));
}

double
rightlog(x, r)            /* right skewed logistic */
double x, r;
{
    return(r * x * x * (1.0 - x));
}

double
drightlog(x, r)            /* derivative of the right skewed logistic */
double x, r;
{
    return(r * ((2.0 * x) - (3.0 * x * x)));
}

double
doublelog(x, r)            /* double logistic */
double x, r;
{
    double d;

    d = 1.0 - x;
    return(r * x * x * d * d);
}

double
ddoublelog(x, r)        /* derivative of the double logistic */
double x, r;
{
    double d;

    d = x * x;
    return(r * ((2.0 * x) - (6.0 * d) + (4.0 * x * d)));
}

void
init_data()
{
    static int i;

    numcolors = XDisplayCells(dpy, XDefaultScreen(dpy));
    displayplanes = DisplayPlanes(dpy, XDefaultScreen(dpy));
    if (numcolors > maxcolor)
        numcolors = maxcolor;
    if (numcolors <= 16) {
        maxcolor = 16; startcolor = 0; 
        color_offset = 0; mincolindex = 1;
    }
    numfreecols = numcolors - mincolindex;
    lowrange = mincolindex - startcolor;
    a_inc = a_range / (double)width;
    b_inc = b_range / (double)height;
	if (!restfile) {
    	point.x = point.y = 0;
    	a = min_a; b = min_b;
	}
    rubber_data.p_min = min_a;
    rubber_data.q_min = min_b;
    rubber_data.p_max = max_a;
    rubber_data.q_max = max_b;
    if (show)
        show_defaults();
    InitBuffer(&Points, maxcolor);
    srand48(time(0));
}

void
init_canvas()
{
    static int i;

    /*
     * create default, writable, graphics contexts for the canvas.
     */
    for (i=0; i<maxcolor; i++) {
        Data_GC[i] = XCreateGC(dpy, DefaultRootWindow(dpy),
                (unsigned long) NULL, (XGCValues *) NULL);
        /* set the background to black */
        XSetBackground(dpy,Data_GC[i],BlackPixel(dpy,XDefaultScreen(dpy)));
        /* set the foreground of the ith context to i */
        XSetForeground(dpy, Data_GC[i], i);
    }
    XSetForeground(dpy,Data_GC[0],BlackPixel(dpy,XDefaultScreen(dpy)));
    XSetForeground(dpy,Data_GC[1],WhitePixel(dpy,XDefaultScreen(dpy)));
}

void
parseargs(ac, av)
int ac;
char **av;
{
    static int c;
    static int i;
    int bindex=0, findex;
    char *ch;
    extern int optind;
    extern char *optarg;
    extern double atof();
    extern void check_params(), usage(), restor_params();

    map = Maps[0];
    deriv = Derivs[0];
    maxexp=minlyap; minexp= -1.0 * minlyap;

    while ((c=getopt(ac,av,
        "Ldpuvc:i:m:C:W:H:I:M:N:O:R:S:a:b:D:F:f:o:w:h:s:x:y:"))!=EOF){
        switch (c) {
        case 'C':    mincolindex=atoi(optarg); break;
        case 'D':    if (strcmp(optarg, "elay"))
						dwell=atoi(optarg); 
					 else
						delay = atoi(av[optind++]);
					 break;
#ifdef MAPS
        case 'F':    funcmaxindex = strlen(optarg);
                if (funcmaxindex > FUNCMAXINDEX)
                    usage();
                ch = optarg;
                Force++;
                for (findex=0;findex<funcmaxindex;findex++) {
                    Forcing[findex] = (int)(*ch++ - '0');
                    if (Forcing[findex] >= NUMMAPS)
                        usage();
                }
                break;
#endif
        case 'H':    height=atoi(optarg); break;
        case 'I':    start_x=atof(optarg); break;
        case 'L':    useprod=0; break;
        case 'M':    minlyap=ABS(atof(optarg)); 
                maxexp=minlyap; minexp= -1.0 * minlyap; break;
        case 'N':    maxcolor=ABS(atoi(optarg)); 
                if ((maxcolor - startcolor) <= 0)
                    startcolor = 0;
                if ((maxcolor - mincolindex) <= 0) {
                    mincolindex = 1;
                    color_offset = 0;
                }
                break;
        case 'O':    color_offset=atoi(optarg); break;
        case 'R':    prob=atof(optarg); Rflag++; setforcing(); break;
        case 'S':    settle=atoi(optarg); break;
        case 'W':    width=atoi(optarg); break;
        case 'a':    min_a=atof(optarg); aflag++; break;
        case 'b':    min_b=atof(optarg); bflag++; break;
        case 'c':    numwheels=atoi(optarg); break;
        case 'd':    demo++; break;
        case 'f':    maxindex = strlen(optarg);
                if (maxindex > MAXINDEX)
                    usage();
                ch = optarg;
                force++;
                while (bindex < maxindex) {
                    if (*ch == 'a')
                        forcing[bindex++] = 0;
                    else if (*ch == 'b')
                        forcing[bindex++] = 1;
                    else
                        usage();
                    ch++;
                }
                break;
        case 'h':    b_range=atof(optarg); hflag++; break;
        case 'i':    restfile++; inname=optarg; break;
        case 'm':    mapindex=atoi(optarg); 
                if ((mapindex >= NUMMAPS) || (mapindex < 0))
                    usage();
                map = Maps[mapindex];
                deriv = Derivs[mapindex];
                if (!aflag)
                    min_a = amins[mapindex];
                if (!wflag)
                    a_range = aranges[mapindex];
                if (!bflag)
                    min_b = bmins[mapindex];
                if (!hflag)
                    b_range = branges[mapindex];
                if (!Force)
                    for (i=0;i<FUNCMAXINDEX;i++)
                        Forcing[i] = mapindex;
                break;
        case 'o':    savefile++; outname=optarg; break;
        case 'p':    negative--; break;
        case 's':    storefile++; savname=optarg; break;
        case 'u':    usage(); break;
        case 'v':    show=1; break;
        case 'w':    a_range=atof(optarg); wflag++; break;
        case 'x':    xposition=atoi(optarg); break;
        case 'y':    yposition=atoi(optarg); break;
        case '?':    usage(); break;
        }
    }
	if (restfile)
		restor_params();
	else {
    	max_a = min_a + a_range;
    	max_b = min_b + b_range;
	}
    a_minimums[0] = min_a; b_minimums[0] = min_b;
    a_maximums[0] = max_a; b_maximums[0] = max_b;
    if (Force)
        if (maxindex == funcmaxindex)
            for (findex=0;findex<funcmaxindex;findex++)
                check_params(Forcing[findex],forcing[findex]);
        else
            fprintf(stderr, "Warning! Unable to check parameters\n");
    else
        check_params(mapindex,2);
}

void
print_help() 
{
    static char str[80];
    static int x_str, y_str, spacing;
    static int ascent, descent, dir;
    static XCharStruct overall;
    static GC gc;

    gc = Data_GC[1];
    XClearWindow(dpy, help);
	x_str = 10;
    y_str = 20;
    sprintf(str,"During run-time, interactive control can be exerted via : ");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    XQueryTextExtents(dpy,(XID)XGContextFromGC(gc),"Hey!",
			4,&dir,&ascent,&descent,&overall);
    spacing = ascent + descent + 5;
    y_str += 2 * spacing;
    sprintf(str,"Mouse buttons allow rubber-banding of a zoom box");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"< halves the 'dwell', > doubles the 'dwell'");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"[ halves the 'settle', ] doubles the 'settle'");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"D flushes the drawing buffer");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"d descends a ladder of windows created via the u or U keys");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"e or E recalculates color indices");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"f saves program state to a file");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"F saves window to a PPM file");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    y_str += spacing;
    sprintf(str,"h or H or ? displays this message");
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"i decrements, I increments the stripe interval");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"KJMN increase/decrease minimum negative exponent");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"m increments the map index, changing maps");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"p or P reverses the colormap for negative/positive exponents");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"r redraws without recalculating");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"R redraws, recalculating with new dwell and settle values");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"s or S spins the colorwheel");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"u pops back up to the last zoom");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"U pops back up to the first picture");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"v or V displays the values of various settings");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"w decrements, W increments the color wheel index");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"x or X clears the window");
    y_str += spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"q or Q exits");
    y_str += 2*spacing;
    XDrawImageString(dpy,help,gc,x_str,y_str,str,strlen(str));
}

void
check_params(mapnum, parnum)
int mapnum;
int parnum;
{

    if (parnum != 1) {
        if ((max_a > pmaxs[mapnum]) || (min_a < pmins[mapnum])) {
            fprintf(stderr, "Warning! Parameter 'a' out of range.\n");
            fprintf(stderr, "You have requested a range of (%lf,%lf).\n",
                    min_a,max_a);
            fprintf(stderr, "Valid range is (%lf,%lf).\n",
                    pmins[mapnum],pmaxs[mapnum]);
        }
    }
    if (parnum != 0) {
        if ((max_b > pmaxs[mapnum]) || (min_b < pmins[mapnum])) {
            fprintf(stderr, "Warning! Parameter 'b' out of range.\n");
            fprintf(stderr, "You have requested a range of (%lf,%lf).\n",
                    min_b,max_b);
            fprintf(stderr, "Valid range is (%lf,%lf).\n",
                    pmins[mapnum],pmaxs[mapnum]);
        }
    }
}

void
usage()
{
    fprintf(stderr,"lyap [-Ls][-W#][-H#][-a#][-b#][-w#][-h#][-x xstart]\n");
    fprintf(stderr,"\t[-M#][-S#][-D#][-f string][-r#][-O#][-C#][-c#][-m#]\n");
#ifdef MAPS
    fprintf(stderr,"\t[-F string]\n");
#endif
    fprintf(stderr,"\tWhere: -C# specifies the minimum color index\n");
    fprintf(stderr,"\t       -r# specifies the maxzimum rgb value\n");
    fprintf(stderr,"\t       -u displays this message\n");
    fprintf(stderr,"\t       -a# specifies the minimum horizontal parameter\n");
    fprintf(stderr,"\t       -b# specifies the minimum vertical parameter\n");
    fprintf(stderr,"\t       -w# specifies the horizontal parameter range\n");
    fprintf(stderr,"\t       -h# specifies the vertical parameter range\n");
    fprintf(stderr,"\t       -D# specifies the dwell\n");
    fprintf(stderr,"\t       -S# specifies the settle\n");
    fprintf(stderr,"\t       -H# specifies the initial window height\n");
    fprintf(stderr,"\t       -W# specifies the initial window width\n");
    fprintf(stderr,"\t       -O# specifies the color offset\n");
    fprintf(stderr,"\t       -c# specifies the desired color wheel\n");
    fprintf(stderr,"\t       -m# specifies the desired map (0-4)\n");
    fprintf(stderr,"\t       -f aabbb specifies a forcing function of 00111\n");
#ifdef MAPS
    fprintf(stderr,"\t       -F 00111 specifies the function forcing function\n");
#endif
    fprintf(stderr,"\t       -L indicates use log(x)+log(y) rather than log(xy)\n");
    fprintf(stderr,"\tDuring display :\n");
    fprintf(stderr,"\t     Use the mouse to zoom in on an area\n");
    fprintf(stderr,"\t     e or E recalculates color indices\n");
    fprintf(stderr,"\t     f or F saves exponents to a file\n");
    fprintf(stderr,"\t     KJmn increase/decrease minimum negative exponent\n");
    fprintf(stderr,"\t     r or R redraws\n");
    fprintf(stderr,"\t     s or S spins the colorwheel\n");
    fprintf(stderr,"\t     w or W changes the color wheel\n");
    fprintf(stderr,"\t     x or X clears the window\n");
    fprintf(stderr,"\t     q or Q exits\n");
    exit(1);
}

Cycle_frames()
{
    static int i;
    extern void redraw();

    for (i=0;i<=maxframe;i++)
        redraw(exponents[i], expind[i], 1); 
}

void
print_info() 
{
	Window r, child;
    int x, y;
    unsigned int bw, d, new_w, new_h;
    static int i;
    static char str[80];
    static int x_str, y_str, spacing;
    static int ascent, descent, dir;
    static XCharStruct overall;
    static GC gc;
	static XWindowAttributes attr;
	static char *Mapname[NUMMAPS] = { "logistic", "circle", "leftlog",
			"rightlog", "doublelog" };

    XGetWindowAttributes(dpy, info, &attr);
	if (attr.map_state == IsUnmapped)
		return;
    if (XGetGeometry(dpy,canvas,&r,&x,&y,&new_w,&new_h,&bw,&d) != 0)
		XTranslateCoordinates(dpy, canvas, r, x, y, &x, &y, &child);

    gc = Data_GC[1];
    XClearWindow(dpy, info);
	x_str = 10;
    y_str = 20;
    sprintf(str,"Current values are as follows : ");
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    XQueryTextExtents(dpy,(XID)XGContextFromGC(gc),"Hey!",
			4,&dir,&ascent,&descent,&overall);
    spacing = ascent + descent + 5;
    y_str += 2 * spacing;
    sprintf(str,"Using the %s map with geometry = %dx%d+%d+%d", 
			Mapname[mapindex], new_w, new_h, x, y);
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"width = %d height = %d run = %d",width, height, run);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"minlyap = %lf minexp = %lf maxexp = %lf",
			minlyap,minexp,maxexp);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"settle = %d  dwell = %d start_x = %lf",settle,dwell, start_x);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"min_a = %lf  a_rng = %lf  max_a = %lf",min_a,a_range,max_a);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
    sprintf(str,"min_b = %lf  b_rng = %lf  max_b = %lf",min_b,b_range,max_b);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
	y_str += spacing;
    if (Rflag) {
        sprintf(str,"pseudo-random forcing");
    	XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
	}
    else if (force) {
        sprintf(str,"periodic forcing = ");
    	XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
        for (i=0;i<maxindex;i++) {
			x_str += XTextWidth(XQueryFont(dpy, XGContextFromGC(gc)), 
							str, strlen(str));
            sprintf(str,"%d",forcing[i]);
    		XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
		}
		x_str = 10;
    }
    else {
        sprintf(str,"periodic forcing = 01");
    	XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
	}
    if (Force) {
        sprintf(str,"function forcing = ");
		y_str += spacing;
    	XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
        for (i=0;i<funcmaxindex;i++) {
			x_str += XTextWidth(XQueryFont(dpy, XGContextFromGC(gc)), 
							str, strlen(str));
            sprintf(str,"%d",Forcing[i]);
    		XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
		}
		x_str = 10;
    }
    sprintf(str,"numcolors = %d delay = %d stripe interval = %d",
			numcolors, delay, stripe_interval);
	y_str += spacing;
    XDrawImageString(dpy,info,gc,x_str,y_str,str,strlen(str));
}

Getkey(event)
XKeyEvent *event;
{
    unsigned char key;
    static int i, running, spindir=0, spinning=0;
	static XWindowAttributes attr;
    extern void init_color(), recalc(), print_help();
    extern void go_init(), go_back(), go_down(), Clear(), write_cmap();
    extern void save_to_file(), Redraw(), redraw(), store_to_file();

    if (XLookupString(event, (char *)&key, sizeof(key), (KeySym *)0,
            (XComposeStatus *) 0) > 0)
        switch (key) {
            case '\015': /* write out current colormap to $HOME/.<prog>map */
        			write_cmap(dpy,cmap,Colors,maxcolor,"lyap","Lyap");
					break;
            case '(': delay -= 25; if (delay < 0) delay = 0; break;
            case ')': delay += 25; break;
            case '<': dwell /= 2; if (dwell < 1) dwell = 1; break;
            case '>': dwell *= 2; break;
            case '[': settle /= 2; if (settle < 1) settle = 0; break;
            case ']': settle *= 2; if (settle < 1) settle = 1; break;
            case 'd': go_down(); break;
            case 'D': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
								0, maxcolor); 
					break;
            case 'e':
            case 'E': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
								0, maxcolor);
                  dorecalc = (!dorecalc);
                  if (dorecalc)
                      recalc(); 
                  else {
                      maxexp = minlyap; minexp = -1.0 * minlyap;
                  }
                  redraw(exponents[frame], expind[frame], 1);
                  break;
        	case 'f': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
						0, maxcolor);
            		  store_to_file(); break;
        	case 'F': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
						0, maxcolor);
            		  save_to_file(); break;
            case 'i': 
                  if (stripe_interval > 0) {
                      stripe_interval--;
                      if (displayplanes > 1) {
                          running = run; run = 0;
						  init_color(dpy,canvas,cmap,Colors,startcolor,
							   mincolindex,numcolors,numwheels,"lyap","Lyap",0);
                          run = running;
                      }
                  }
                  break;
            case 'I': stripe_interval++;
                  if (displayplanes > 1) {
                      running = run; run = 0;
					  init_color(dpy,canvas,cmap,Colors,startcolor,
							mincolindex,numcolors,numwheels,"lyap","Lyap",0);
                      run = running;
                  }
                  break;
            case 'K': if (minlyap > 0.05)
                          minlyap -= 0.05;
                   break;
            case 'J': minlyap += 0.05; 
                   break;
            case 'm': mapindex++;
                  if (mapindex >= NUMMAPS)
                      mapindex=0;
                  map = Maps[mapindex];
                  deriv = Derivs[mapindex];
                  if (!aflag)
                      min_a = amins[mapindex];
                  if (!wflag)
                      a_range = aranges[mapindex];
                  if (!bflag)
                      min_b = bmins[mapindex];
                  if (!hflag)
                      b_range = branges[mapindex];
                  if (!Force)
                      for (i=0;i<FUNCMAXINDEX;i++)
                           Forcing[i] = mapindex;
                  max_a = min_a + a_range;
                  max_b = min_b + b_range;
                  a_minimums[0] = min_a; b_minimums[0] = min_b;
                  a_maximums[0] = max_a; b_maximums[0] = max_b;
                  a_inc = a_range / (double)width;
                  b_inc = b_range / (double)height;
                  point.x = 0;
                  point.y = 0;
                  a = rubber_data.p_min = min_a;
                  b = rubber_data.q_min = min_b;
                  rubber_data.p_max = max_a;
                  rubber_data.q_max = max_b;
                  Clear();
                  run = 1;
                  break;
            case 'M': if (minlyap > 0.005)
                      minlyap -= 0.005;
                  break;
            case 'N': minlyap += 0.005;
                  break;
            case 'p':
            case 'P': negative = (!negative); 
                  FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
								0, maxcolor); 
				  redraw(exponents[frame], expind[frame], 1); 
                  break;
            case 'r': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
							0, maxcolor); 
				  redraw(exponents[frame],expind[frame],1); 
                  break;
            case 'R': FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 
							0, maxcolor); 
				  Redraw(); 
				  break;
            case 'S': spinning=0; break;
            case 's': spinning=1; spindir=(!spindir); 
				  Spin(dpy,cmap,Colors,startcolor,numcolors,delay,spindir); 
                  break;
            case 'u': go_back(); break;
            case 'U': go_init(); break;
            case 'v':
            case 'V': XGetWindowAttributes(dpy, info, &attr);
					if (attr.map_state != IsUnmapped)
						XUnmapWindow(dpy, info);
					else {
						XMapRaised(dpy, info);
						print_info(); 
					}
					break;
            case '\027': /* (ctrl-W) read color palette from $HOME/.lyapmap */
                  numwheels = 0;
                  if (displayplanes > 1) {
                      running = run; run = 0;
					  init_color(dpy,canvas,cmap,Colors,startcolor,
							mincolindex,numcolors,numwheels,"lyap","Lyap",0);
                      run = running;
                  }
                  break;
            case 'W': if (numwheels < MAXWHEELS)
                          numwheels++;
                      else
                          numwheels = 0;
                      if (displayplanes > 1) {
                          running = run; run = 0;
					  	  init_color(dpy,canvas,cmap,Colors,startcolor,
							   mincolindex,numcolors,numwheels,"lyap","Lyap",0);
                          run = running;
                      }
                      break;
            case 'w': if (numwheels > 0)
                          numwheels--;
                      else
                          numwheels = MAXWHEELS;
                      if (displayplanes > 1) {
                          running = run; run = 0;
					  	  init_color(dpy,canvas,cmap,Colors,startcolor,
							   mincolindex,numcolors,numwheels,"lyap","Lyap",0);
                          run = running;
                      }
                      break;
            case 'x': Clear(); break;
            case 'X': Destroy_frame(); break;
            case 'z': running = run; run = 0;
                  Cycle_frames(); 
                  run = running; redraw(exponents[frame], expind[frame], 1);
                  break;
            case 'Z': running = run; run = 0;
                  while (!XPending(dpy)) Cycle_frames(); 
                  run = running; redraw(exponents[frame], expind[frame], 1); 
                  break;
            case 'q':
            case 'Q': Cleanup(); exit(0); break;
            case '?':
            case 'h':
            case 'H': XGetWindowAttributes(dpy, help, &attr);
					if (attr.map_state != IsUnmapped)
						XUnmapWindow(dpy, help);
					else {
						XMapRaised(dpy, help);
						print_help(); 
					}
					break;
            default:  break;
        }
		if (spinning)
			Spin(dpy,cmap,Colors,startcolor,numcolors,delay,spindir); 
		print_info();
}

/* Here's where we index into a color map. After the Lyapunov exponent is
 * calculated, it is used to determine what color to use for that point.
 * I suppose there are a lot of ways to do this. I used the following :
 * if it's non-negative then there's a reserved area at the lower range
 * of the color map that i index into. The ratio of some "minimum exponent
 * value" and the calculated value is used as a ratio of how high to index
 * into this reserved range. Usually these colors are dark red (see init_color).
 * If the exponent is negative, the same ratio (expo/minlyap) is used to index
 * into the remaining portion of the colormap (which is usually some light
 * shades of color or a rainbow wheel). The coloring scheme can actually make
 * a great deal of difference in the quality of the picture. Different colormaps
 * bring out different details of the dynamics while different indexing
 * algorithms also greatly effect what details are seen. Play around with this.
 */
sendpoint(expo)
double expo;
{
    static int index;
    static double tmpexpo;
    extern double exp();

    tmpexpo = (negative) ? expo : -1.0 * expo;
    if (tmpexpo > 0) {
        if (displayplanes >1) {
            index = (int)(tmpexpo*lowrange/maxexp);
            index = (index % lowrange) + startcolor;
        }
        else
            index = 0;
    }
    else {
        if (displayplanes >1) {
            index = (int)(tmpexpo*numfreecols/minexp);
            index = (index % numfreecols) + mincolindex;
        }
        else
            index = 1;
    }
	BufferPoint(dpy, canvas, pixmap, Data_GC, &Points, index, point.x, point.y);
    point.x++;
    a += a_inc;
    if (save)
        exponents[frame][expind[frame]++] = expo;
    if (point.x >= width) {
        point.y++;
        point.x = 0;
        if (save) {
            b += b_inc;
            a = min_a;
        }
        if (point.y >= height)
            return FALSE;
        else
            return TRUE;
    }
    return TRUE;
}

void 
redisplay (w, event)
Window          w;
XExposeEvent    *event;
{
	if (event->window == help)
		print_help();
	else if (event->window == info)
		print_info();
	else {
    	/*
     	* Extract the exposed area from the event and copy
     	* from the saved pixmap to the window.
     	*/
    	XCopyArea(dpy, pixmap, canvas, Data_GC[0], 
           	event->x,event->y,event->width,event->height,event->x, event->y);
	}
}

void
resize()
{
    Window r;
    int n, x, y;
    unsigned int bw, d, new_w, new_h;
	static XWindowAttributes attr;
    extern void Clear(), Redraw();

    XGetGeometry(dpy,canvas,&r,&x,&y,&new_w,&new_h,&bw,&d);
    if ((new_w == width) && (new_h == height)) {
		print_info();
        return;
	}
    freemem();
    width = new_w; height = new_h;
    XClearWindow(dpy, canvas);
    if (pixmap)
        XFreePixmap(dpy, pixmap);
    pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), 
            width, height, DefaultDepth(dpy, screen));
    a_inc = a_range / (double)width;
    b_inc = b_range / (double)height;
    point.x = 0;
    point.y = 0;
    run = 1;
    a = rubber_data.p_min = min_a;
    b = rubber_data.q_min = min_b;
    rubber_data.p_max = max_a;
    rubber_data.q_max = max_b;
    setupmem();
    for (n=0;n<MAXFRAMES;n++)
        if ((n <= maxframe) && (n != frame))
            resized[n] = 1;
    InitBuffer(&Points, maxcolor);
    Clear();
	print_info();
    Redraw();
}

void
redraw(exparray, index, cont)
double *exparray;
int index, cont;
{
    static int i;
    static int x_sav, y_sav;

    x_sav = point.x;
    y_sav = point.y;

    point.x = 0;
    point.y = 0;

    save=0;
    for (i=0;i<index;i++)
        sendpoint(exparray[i]);
    save=1;
    
    if (cont) {
        point.x = x_sav;
        point.y = y_sav;
    }
    else {
        a = point.x * a_inc + min_a;
        b = point.y * b_inc + min_b;
    }
    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
}

void
Redraw() 
{
    FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
    point.x = 0;
    point.y = 0;
    run = 1;
    a = min_a;
    b = min_b;
    expind[frame] = 0;
    resized[frame] = 0;
}

/* Store colormap indices in file so we can read them in later */
void
store_to_file() 
{
    FILE *outfile;
    unsigned char c;
    XImage *ximage;
    static int i,j;

    outfile = fopen(savname,"w");
    if(!outfile) {
        perror(savname);
        Cleanup();
        exit(-1);
    }
    fprintf(outfile,"# width=%d height=%d \n",width,height);
    fprintf(outfile,"# settle=%d dwell=%d start_x=%lf \n",settle,dwell,start_x);
    fprintf(outfile,"# min_a=%lf a_rng=%lf max_a=%lf \n",min_a,a_range,max_a);
    fprintf(outfile,"# min_b=%lf b_rng=%lf max_b=%lf \n",min_b,b_range,max_b);
    fprintf(outfile,"# run=%d point.x=%d point.y=%d \n",run,point.x,point.y);
    fprintf(outfile,"# negative=%d mincolindex=%d startcolor=%d \n",
			negative,mincolindex,startcolor);
    fprintf(outfile,"# minexp=%lf maxexp=%lf a=%lf b=%lf \n",
			minexp,maxexp,a,b);
    fprintf(outfile,"# Force=%d force=%d useprod=%d \n",Force,force,useprod);
    fprintf(outfile,"# mapindex=%d maxindex=%d \n", mapindex, maxindex);
    fprintf(outfile,"# numwheels=%d Rflag=%d color_offset=%d \n",
			numwheels,Rflag,color_offset);
    fprintf(outfile,"# maxcolor=%d start_x=%lf minlyap=%lf prob=%lf\n",
			maxcolor,start_x,minlyap,prob);
    fprintf(outfile,"# maxindex=%d periodic forcing=", maxindex);
        for (i=0;i<maxindex;i++) {
            fprintf(outfile," %d",forcing[i]);
        }
    fprintf(outfile," \n");
    fprintf(outfile,"# funcmaxindex=%d function forcing=", funcmaxindex);
    for (i=0;i<funcmaxindex;i++)
         fprintf(outfile," %d",Forcing[i]);
    fprintf(outfile," \n");
    fprintf(outfile,"# numcolors=%d\n",numcolors);

    ximage=XGetImage(dpy, pixmap, 0, 0, 
			(unsigned int)width, (unsigned int)height, AllPlanes, ZPixmap);

    for (j=0;j<=point.y;j++)
        for (i=0;i<width;i++) {
            c = (unsigned char)XGetPixel(ximage,i,j);
            fwrite((char *)&c,sizeof c,1,outfile);
        }
    fclose(outfile);
}

/* Store color pics in PPM format and monochrome in PGM */
void
save_to_file() 
{
    FILE *outfile;
    unsigned char c;
    XImage *ximage;
    static int i,j;
    struct Colormap {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    struct Colormap *colormap=NULL;

    if (colormap)
        free(colormap);
    if ((colormap=
        (struct Colormap *)malloc(sizeof(struct Colormap)*maxcolor))
            == NULL) {
        fprintf(stderr,"Error malloc'ing colormap array\n");
        Cleanup();
        exit(-1);
    }
    outfile = fopen(outname,"w");
    if(!outfile) {
        perror(outname);
        Cleanup();
        exit(-1);
    }

    ximage=XGetImage(dpy, pixmap, 0, 0, width, height, AllPlanes, ZPixmap);

    if (displayplanes > 1) {
        for (i=0;i<maxcolor;i++) {
            colormap[i].red=(unsigned char)(Colors[i].red >> 8);
            colormap[i].green=(unsigned char)(Colors[i].green >> 8);
            colormap[i].blue =(unsigned char)(Colors[i].blue >> 8);
        }
        fprintf(outfile,"P%d %d %d\n",6,width,height);
    }
    else
        fprintf(outfile,"P%d %d %d\n",5,width,height);
    fprintf(outfile,"# settle=%d  dwell=%d start_x=%lf\n",settle,dwell,start_x);
    fprintf(outfile,"# min_a=%lf  a_rng=%lf  max_a=%lf\n",min_a,a_range,max_a);
    fprintf(outfile,"# min_b=%lf  b_rng=%lf  max_b=%lf\n",min_b,b_range,max_b);
    if (Rflag)
        fprintf(outfile,"# pseudo-random forcing\n");
    else if (force) {
        fprintf(outfile,"# periodic forcing=");
        for (i=0;i<maxindex;i++) {
            fprintf(outfile,"%d",forcing[i]);
        }
        fprintf(outfile,"\n");
    }
    else
         fprintf(outfile,"# periodic forcing=01\n");
    if (Force) {
         fprintf(outfile,"# function forcing=");
         for (i=0;i<funcmaxindex;i++)
             fprintf(outfile,"%d",Forcing[i]);
         fprintf(outfile,"\n");
    }
    fprintf(outfile,"%d\n",numcolors-1);

    for (j=0;j<height;j++)
        for (i=0;i<width;i++) {
            c = (unsigned char)XGetPixel(ximage,i,j);
            if (displayplanes > 1)
                fwrite((char *)&colormap[c],sizeof colormap[0],1,outfile);
            else
                fwrite((char *)&c,sizeof c,1,outfile);
        }
    fclose(outfile);
}

/* Read saved file with parameters */
void
restor_params() 
{
	unsigned char s[16];
	static int i;

    infile = fopen(inname,"r");
    if(!infile) {
        perror(inname);
        Cleanup();
        exit(-1);
    }

    fscanf(infile,"%1s%6s%d%7s%d",s,s,&width,s,&height);
    fscanf(infile,"%1s%7s%d%6s%d%8s%lf",s,s,&settle,s,&dwell,s,&start_x);
    fscanf(infile,"%1s%6s%lf%6s%lf%6s%lf", s,s,&min_a,s,&a_range,s,&max_a);
    fscanf(infile,"%1s%6s%lf%6s%lf%6s%lf", s,s,&min_b,s,&b_range,s,&max_b);
    fscanf(infile,"%1s%4s%d%8s%d%8s%d",s,s,&run,s,&point.x,s,&point.y);
    fscanf(infile,"%1s%9s%d%12s%d%11s%d",
			s,s,&negative,s,&mincolindex,s,&startcolor);
    fscanf(infile,"%1s%7s%lf%7s%lf%2s%lf%2s%lf",
			s,s,&minexp,s,&maxexp,s,&a,s,&b);
    fscanf(infile,"%1s%6s%d%6s%d%8s%d",s,s,&Force,s,&force,s,&useprod);
    fscanf(infile,"%1s%9s%d%9s%d",
			s,s,&mapindex,s,&maxindex);
    fscanf(infile,"%1s%10s%d%6s%d%13s%d",
			s,s,&numwheels,s,&Rflag,s,&color_offset);
    fscanf(infile,"%1s%9s%d%8s%lf%8s%lf%5s%lf",
			s,s,&maxcolor,s,&start_x,s,&minlyap,s,&prob);
    fscanf(infile,"%1s%9s%d%s%s", s,s,&maxindex,s,s);
	for(i=0;i<maxindex;i++) {
		fscanf(infile, "%d", &forcing[i]);
	}
    fscanf(infile,"%1s%13s%d%s%s", s,s,&funcmaxindex,s,s);
	for(i=0;i<funcmaxindex;i++)
		fscanf(infile, "%d", &Forcing[i]);
	fscanf(infile, "%1s%10s%d", s,s,&numcolors);
	fread(s, sizeof s[0], 1, infile);
    map = Maps[mapindex];
    deriv = Derivs[mapindex];
}

void
restor_picture() {
    static int i, j, k;
	unsigned char c;

    for (j=0;j<point.y;j++)
        for (i=0;i<width;i++) {
			k = fread(&c, sizeof c, 1, infile);
			if (k) {
			    BufferPoint(dpy,canvas,pixmap,Data_GC,&Points,(int)c,i,j);
				if (k < mincolindex)
        			exponents[frame][expind[frame]++] = 
						-(double)k/(double)mincolindex;
				else
        			exponents[frame][expind[frame]++] = 
						(double)k/(double)numfreecols;
			}
			else
				break;
        }
    for (j=0;j<point.x;j++) {
		k = fread(&c, sizeof c, 1, infile);
		if (k) {
		    BufferPoint(dpy,canvas,pixmap,Data_GC,&Points,(int)c,j,point.y);
			if (k < mincolindex)
        		exponents[frame][expind[frame]++] = 
					-(double)k/(double)mincolindex;
			else
        		exponents[frame][expind[frame]++] = 
					(double)k/(double)numfreecols;
		}
		else
			break;
    }
    fclose(infile);
	FlushBuffer(dpy, canvas, pixmap, Data_GC, &Points, 0, maxcolor);
    XCopyArea(dpy, pixmap, canvas, Data_GC[0], 0, 0, width, height, 0, 0);
}

void
recalc() 
{
    static int i, index, x, y;
    
    minexp = maxexp = 0.0;
    x = y = 0;
    for (i=0;i<expind[frame];i++) {
        if (exponents[frame][i] < minexp)
            minexp = exponents[frame][i];
        if (exponents[frame][i] > maxexp)
            maxexp = exponents[frame][i];
    }
}

void
Clear() 
{
    XFillRectangle(dpy, pixmap, Data_GC[0], 0, 0, width, height);
    XCopyArea(dpy, pixmap, canvas, Data_GC[0], 
                0, 0, width, height, 0, 0);
    InitBuffer(&Points, maxcolor);
}

void
show_defaults() 
{

    printf("Width=%d  Height=%d  numcolors=%d  settle=%d  dwell=%d\n",
        width,height,numcolors,settle,dwell);
    printf("min_a=%lf  a_range=%lf  max_a=%lf\n", min_a,a_range,max_a);
    printf("min_b=%lf  b_range=%lf  max_b=%lf\n", min_b,b_range,max_b);
    printf("minlyap=%lf  minexp=%lf  maxexp=%lf\n", minlyap,minexp,maxexp);
    exit(0);
}

void
CreateXorGC()
{
    XGCValues values;

    values.foreground = foreground;
    values.line_style = LineSolid;
    values.function = GXxor;
    RubberGC = XCreateGC(dpy, DefaultRootWindow(dpy),
          GCForeground | GCBackground | GCFunction | GCLineStyle, &values);
}

void 
StartRubberBand(w, data, event)
Window w;
image_data_t *data;
XEvent *event;
{
    XPoint corners[5];
    extern void SetupCorners();

    nostart = 0;
    data->rubber_band.last_x = data->rubber_band.start_x = event->xbutton.x;
    data->rubber_band.last_y = data->rubber_band.start_y = event->xbutton.y;
    SetupCorners(corners, data);
    XDrawLines(dpy, canvas, RubberGC,
        corners, sizeof(corners) / sizeof(corners[0]), CoordModeOrigin);
}

void
SetupCorners(corners, data)
XPoint *corners;
image_data_t *data;
{
    corners[0].x = data->rubber_band.start_x;
    corners[0].y = data->rubber_band.start_y;
    corners[1].x = data->rubber_band.start_x;
    corners[1].y = data->rubber_band.last_y;
    corners[2].x = data->rubber_band.last_x;
    corners[2].y = data->rubber_band.last_y;
    corners[3].x = data->rubber_band.last_x;
    corners[3].y = data->rubber_band.start_y;
    corners[4] = corners[0];
}

void 
TrackRubberBand(w, data, event)
Window w;
image_data_t *data;
XEvent *event;
{
    XPoint corners[5];
    extern void SetupCorners();

    if (nostart)
        return;
    SetupCorners(corners, data);
    XDrawLines(dpy, canvas, RubberGC, corners, 
               sizeof(corners) / sizeof(corners[0]), CoordModeOrigin);
    data->rubber_band.last_x = Min(event->xbutton.x, width);
    data->rubber_band.last_y = Min(event->xbutton.y, height);
    if (data->rubber_band.last_y < data->rubber_band.start_y ||
        data->rubber_band.last_x < data->rubber_band.start_x) {
            data->rubber_band.last_y = data->rubber_band.start_y;
            data->rubber_band.last_x = data->rubber_band.start_x;
    }
    SetupCorners(corners, data);
    XDrawLines(dpy, canvas, RubberGC, corners, 
               sizeof(corners) / sizeof(corners[0]), CoordModeOrigin);
}

void 
EndRubberBand(w, data, event)
Window w;
image_data_t *data;
XEvent *event;
{
    XPoint corners[5];
    XPoint top, bot;
    double delta, diff;
    extern void set_new_params(), SetupCorners();

    nostart = 1;
    SetupCorners(corners, data);
    XDrawLines(dpy, canvas, RubberGC,
        corners, sizeof(corners) / sizeof(corners[0]), CoordModeOrigin);
    if (data->rubber_band.start_x >= data->rubber_band.last_x ||
        data->rubber_band.start_y >= data->rubber_band.last_y)
        return;
    top.x = data->rubber_band.start_x;
    bot.x = data->rubber_band.last_x;
    top.y = data->rubber_band.start_y;
    bot.y = data->rubber_band.last_y;
    diff = data->q_max - data->q_min;
    delta = (double)top.y / (double)height;
    data->q_min += diff * delta;
    delta = (double)(height - bot.y) / (double)height;
    data->q_max -= diff * delta;
    diff = data->p_max - data->p_min;
    delta = (double)top.x / (double)width;
    data->p_min += diff * delta;
    delta = (double)(width - bot.x) / (double)width;
    data->p_max -= diff * delta;
    fflush(stdout);
    set_new_params(w, data);
}

void
set_new_params(w, data)
Window w;
image_data_t *data;
{
	static XWindowAttributes attr;
    extern void Clear();

    frame = (maxframe + 1) % MAXFRAMES;
    if (frame > maxframe)
        maxframe = frame;
    a_range = data->p_max - data->p_min;
    b_range = data->q_max - data->q_min;
    a_minimums[frame] = min_a = data->p_min;
    b_minimums[frame] = min_b = data->q_min;
    a_inc = a_range / (double)width;
    b_inc = b_range / (double)height;
    point.x = 0;
    point.y = 0;
    run = 1;
    a = min_a;
    b = min_b;
    a_maximums[frame] = max_a = data->p_max;
    b_maximums[frame] = max_b = data->q_max;
    expind[frame] = 0;
    Clear();
	print_info();
}

void
go_down() 
{
    static int i;
    
    frame++;
    if (frame > maxframe)
        frame = 0;
    jumpwin();
}

void
go_back() 
{
    static int i;
    
    frame--;
    if (frame < 0)
        frame = maxframe;
    jumpwin();
}

jumpwin()
{
    rubber_data.p_min = min_a = a_minimums[frame];
    rubber_data.q_min = min_b = b_minimums[frame];
    rubber_data.p_max = max_a = a_maximums[frame];
    rubber_data.q_max = max_b = b_maximums[frame];
    a_range = max_a - min_a;
    b_range = max_b - min_b;
    a_inc = a_range / (double)width;
    b_inc = b_range / (double)height;
    point.x = 0;
    point.y = 0;
    a = min_a;
    b = min_b;
    Clear();
    if (resized[frame])
        Redraw();
    else
        redraw(exponents[frame], expind[frame], 0);
}

void
go_init() 
{
    static int i;
    
    frame = 0;
    jumpwin();
}

Destroy_frame()
{
    static int i;

    for (i=frame; i<maxframe; i++) {
        exponents[frame] = exponents[frame+1];
        expind[frame] = expind[frame+1];
        a_minimums[frame] = a_minimums[frame+1];
        b_minimums[frame] = b_minimums[frame+1];
        a_maximums[frame] = a_maximums[frame+1];
        b_maximums[frame] = b_maximums[frame+1];
    }
    maxframe--;
    go_back();
}

freemem()
{
    static int i;
    for (i=0;i<MAXFRAMES;i++)
        free(exponents[i]);
}

setupmem()
{
    static int i;

    for (i=0;i<MAXFRAMES;i++) {
        if((exponents[i]=
            (double *)malloc(sizeof(double)*width*height))==NULL){
                fprintf(stderr,"Error malloc'ing exponent array.\n");
                exit(-1);
        }
    }
}

setforcing()
{
    static int i;
    extern double drand48();

    for (i=0;i<MAXINDEX;i++)
        forcing[i] = (drand48() > prob) ? 0 : 1;
}
