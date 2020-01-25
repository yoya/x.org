
/* 
     Copyright (c) 1991 xtexcad  V1.3 by K. Zitzmann

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.
*/


/* graphics.c */

#include "x_stuff.h"
#include "oberfl.h"
#include "ereignis.h"
#include "io_trans.h"
#include "graphics.h"
#include "file_sel.h"
#include <math.h>
#include <ctype.h>





int black_white_patch = (1==0); /* JM */



/* the LaTeX-dilemma : restricted slopes, length and diameters */

/* possible line slopes */
/*
 * the constants indicate the possible values for the numerator or
 * denominator of the fraction which represents the slope, i.e "6" means,
 * values from -6 to 6
 */
/*
 * convention : some TeX implementations support unlimited slopes, diameters
 * and length. This might be indicated by changing the r-value to 0 !!!
 */

int             line_slope = 6;
int             slope_line = 6;	/* a copy */
int             arrow_slope = 4;
int             slope_arrow = 4;	/* a copy */
int             line_length = 10;	/* each line/arrow must be 10 points
					 * long */
/* const circle_diameter[0]={-1}; */
/* diameters must! be sorted by size */
/* -1 is an end-marker */
int             circle_diameter[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20, 24, 28, 32, 36, 40, -1};
int             disc_diameter[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1};
int             rast_height = 40;

float           l_slope[30][3];	/* static !!! depends on 'line_slope' !!!! */
float           a_slope[20][3];	/* dito! */
Boolean         visible;
int		ab=-9;
Widget		muschel; /* popup-shell for textedit */
Widget		fb_muschel; /* popup-shell ... */
Widget		db_muschel; /* popup-shell ... */

struct textdel {
		Widget tg; /* pointer to the togglewidget */
		void *obj; /* pointer to text */
		} mem[10];		




/*****************************************************************************/
/* Callbackfunktion fuer allgemeine Events die unterdreuckt werden sollen    */
/* Die Funktion wird aufgerufen, raeumt den Stack auf und kehrt zureuck      */
/*                                                                           */
/* Aufruf: Dummy(widget,event,params,n_params);                              */
/*                                                                           */
/* Funktion wird nur von X-Windows aufgerufen.                               */
/*****************************************************************************/
/* Prozedur dient dazu unerwuenscht Events abzufangen */
void Dummy(widget,event,params,n_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *n_params;
{
}


void
transform(char *name)
{
	/* eliminates all special characters in this string */

	char           *buffer;
	int             i, k, name_length;



	name_length = strlen(name);

	buffer = (char *) malloc(name_length + 1);

	strcpy(buffer, name);

	for (i = 0, k = 0; (i <= name_length) && (buffer[i] != '\0'); i++)
	{
		if (isprint(buffer[i]))
			name[k++] = buffer[i];
	}

	if (name[k - 1] != '\0')
		name[k] = '\0';

	free(buffer);


}





void
make_GCcontext()
{
	/* make a GC */

	XGCValues       gcvals;
	Display        *disp=XtDisplay(toplevel);
	Drawable        win=(Drawable)XtWindow(toplevel);
	unsigned long   maske,black_pixel,white_pixel;
	char            d_list[5];
	Screen		*scr=XtScreen(toplevel); 
	XColor		color_approx,color_exact;
	Colormap	cmap=DefaultColormap(disp,DefaultScreen(disp));
	

	
	
	/* get color */
	
	if (XAllocNamedColor(disp,cmap,"BLACK",&color_approx,&color_exact))
		black_pixel=color_approx.pixel;
	else 
	{
		black_pixel=BlackPixel(disp,XScreenNumberOfScreen(scr));
		fprintf(stderr,"\nServer refused color allocation !\n...taking default foreground color\n");
	}
	
	if (XAllocNamedColor(disp,cmap,"WHITE",&color_approx,&color_exact))
		white_pixel=color_approx.pixel;
	else 
	{
		white_pixel=WhitePixel(disp,XScreenNumberOfScreen(scr));
		fprintf(stderr,"\nServer refused color allocation !\n...taking default background color\n");
	}
		
		
	
	

	gcvals.foreground = black_pixel;
	gcvals.background = white_pixel;
	gcvals.line_width = 0;
	gcvals.plane_mask = AllPlanes;
	gcvals.line_style = LineSolid;
	gcvals.cap_style = CapButt;
	gcvals.join_style = JoinMiter;
	gcvals.fill_style = FillSolid;
	gcvals.fill_rule = WindingRule;
	gcvals.dash_offset = 5;
	
	/* gcvals.function = GXxor; */ 
	/* JM */ 
	/* starts here... */
	black_white_patch = (gcvals.foreground == 0);
	if (black_white_patch)
		gcvals.function = GXequiv;
	else
		gcvals.function = GXxor;
	/* ...ends here ! */

	maske = (GCForeground | GCBackground | GCLineWidth | GCLineWidth |
	GCPlaneMask | GCLineStyle | GCCapStyle | GCJoinStyle | GCFillStyle |
		 GCFillRule | GCDashOffset | GCFunction);


	gc = XCreateGC(disp, win, maske, &gcvals);

	/* for dashed lines only */
	copy_gc = XCreateGC(disp, win, maske, &gcvals);

	d_list[0] = 3;
	d_list[1] = 10;
	d_list[2] = '\0';
	XSetLineAttributes(disp, copy_gc, 0, LineOnOffDash, CapButt, JoinMiter);
	XSetDashes(disp, copy_gc, 0, d_list, strlen(d_list));


	
}


void
PointerPosition(int *x, int *y, unsigned int *mask)
{
	/* return the (relative) coordinates of the pointer in pboard */
	/* XQueryPointer returns origin-coordinates: (-1,-1) !!! */
	/* origin is top-left */

	Display        *disp;
	Window          win, egal1, egal2;
	int             h, v;
	Bool            result;
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);
	result = XQueryPointer(disp, win, &egal1, &egal2, &h, &v, x, y, mask);
	/* important parameters are disp,win,x,y */
	/* result==False is an impossible error ! */
	/* corrections of origin : */

	if ((*x) < 0)
		x = 0;
	if ((*y) < 0)
		y = 0;


}/* function returns the x,y coordinates of the pointer in pboard */


void
snapPointerPosition(int *x, int *y, unsigned int *mask)
{
	PointerPosition(x, y, mask);
	if ((snap == True) && (raster == True))
		valid_snap_coords(x, y);
}


void
valid_snap_coords(int *x, int *y)
{
	/* compute the coordinates of the nearest raster-node */
	int             res, h = rast_height;
	float           a, b;
	a = (float) (*x);
	b = (float) (*y);

	if (zoomed == True)
		zoomed2real(&a, &b);

	(*x) = (int) a;
	(*y) = (int) b;

	res = (h / 2 + (*x)) / h;
	(*x) = res * h;

	res = (h / 2 + (*y)) / h;
	(*y) = res * h;


	a = (float) (*x);
	b = (float) (*y);

	if (zoomed == True)
		real2zoomed(&a, &b);

	(*x) = (int) a;
	(*y) = (int) b;


}


void
set_TM_btn1_first(Widget w, XEvent * event, String * id, Cardinal * num)
{
	switch (graph_action)
	{
	case 'Z':set_zoom_window();
		break;

	case 'T':
		set_text_input();
		break;

	case 'P':
		set_pick_object();
		break;

	case 'Y':
		set_copy_object();
		break;

	case 'G':
		set_pick_erase();
		break;

	default:
		set_first();
		break;
	}
}


void
set_first()
{
	/* sets the translation manager of pboard */
	/* if button is pressed, execute the function */

	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Down>: btn1_press_first()";
	static XtActionsRec actions[80] = {{"btn1_press_first", btn1_press_first}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

}

void
set_text_input()
{
	/* does the same (as the function above) for 'text' */
	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Down>: manage_text_input()";
	static XtActionsRec actions[80] = {{"manage_text_input", manage_text_input}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

}

void print_head(char ch)
{
  char txt[80];

	strcpy(txt, titlename);
	strcat(txt, "  :  ");

	switch (ch)
	{
		case 'L':strcat(txt,"#### Line ####");
			 break;
		case 'I':strcat(txt,"#### Bezier ####");
			 break;
		case 'V':strcat(txt,"#### Vector ####");
			 break;
		case 'N':strcat(txt,"#### Box --> framed ####");
			 break;
		case 'D':strcat(txt,"#### Box --> dashed ####");
			 break;
		case 'F':strcat(txt,"#### Box --> filled ####");
			 break;
		case 'O':strcat(txt,"#### Circle --> oval ####");
			 break;
		case 'C':strcat(txt,"#### Circle --> normal ####");
			 break;
		case 'B':strcat(txt,"#### Circle --> filled ####");
			 break;
		default :strcat(txt,titlename);
			 break;
	}
	
	headline(toplevel,txt);

}


void
leave_translation()
{
  char          	destination[80] = "<LeaveWindow> : left() \n";
  static 		XtActionsRec actions[80] = {{"left", left}};
  XtTranslations  	trans_table;


	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}


void
newline(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
  char ch;
  
	/* button 3 or 4 was pressed, start drawing a new line (object) */

	XtUninstallTranslations(pboard);	

	leave_translation();
	ch=graph_action;
	leaving(0, 0, 0, 0);
	leave_translation();
	print_head(ch);
	graph_action=ch;
	set_first();
	
}
 


void
btn1_press_first(Widget w, XEvent * event, String * id, Cardinal * num)
{
	/* ------------------------------------------------------------- */
	/* btn1 in pboard was pressed for the first time */
	/* ------------------------------------------------------------- */

	XtTranslations  trans_table;
	char            destination[80] = "<LeaveNotify>: leaving()";
	char            destination2[80] = "<Btn2Down>: newline()\n\
					    <Btn3Down>: newline()\n";
	static XtActionsRec actions[80] = {{"leaving", leaving},
					   {"newline", newline}
					  };
	int             x, y;
	unsigned int    mask;




	/* get the position of the pointer */
	snapPointerPosition(&x, &y, &mask);

	/* install interrupt-action */
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
	trans_table = XtParseTranslationTable(destination2);
	XtOverrideTranslations(pboard, trans_table);




	/* prepare the 2nd btn1-press */
	set_TM_btn1_second();




	/* store the coordinates */
	switch (graph_action)
	{
	case 'I':
		if (bezier_start == NULL)
		{	/* first entry */
			bezier_start = (struct fig6 *) malloc(sizeof(bezier));
			bezier_curr = bezier_start;
		} else
		{	/* insert at end */
			bezier_curr->next = (struct fig6 *) malloc(sizeof(bezier));
			bezier_curr = bezier_curr->next;
		}
		bezier_curr->ax = (float) x;
		bezier_curr->ay = (float) y;
		bezier_curr->ex= -999.0;	/* special marker */
		bezier_curr->next = NULL;
		break;	/* bezier_curr points to (incomplete) structure */

	case 'L':
		if (strich_start == NULL)
		{	/* first entry */
			strich_start = (struct fig2 *) malloc(sizeof(strich));
			strich_curr = strich_start;
		} else
		{	/* insert at end */
			strich_curr->next = (struct fig2 *) malloc(sizeof(strich));
			strich_curr = strich_curr->next;
		}
		strich_curr->x = (float) x;
		strich_curr->y = (float) y;
		strich_curr->h = -999.0;	/* special marker */
		strich_curr->next = NULL;
		break;	/* strich_curr points to (incomplete) structure */

	case 'V':
		if (pfeil_start == NULL)
		{	/* first entry */
			pfeil_start = (struct fig2 *) malloc(sizeof(pfeil));
			pfeil_curr = pfeil_start;
		} else
		{	/* insert at end */
			pfeil_curr->next = (struct fig2 *) malloc(sizeof(pfeil));
			pfeil_curr = pfeil_curr->next;
		}
		pfeil_curr->x = (float) x;
		pfeil_curr->y = (float) y;
		pfeil_curr->h = -999.0; 	/* special marker */
		pfeil_curr->next = NULL;
		break;	/* pfeil_curr points to (incomplete) structure */

	case 'N':
		if (framedBox_start == NULL)
		{	/* first entry */
			framedBox_start = (struct fig3 *) malloc(sizeof(framedBox));
			framedBox_curr = framedBox_start;
		} else
		{	/* insert at end */
			framedBox_curr->next = (struct fig3 *) malloc(sizeof(framedBox));
			framedBox_curr = framedBox_curr->next;
		}
		framedBox_curr->x = (float) x;
		framedBox_curr->y = (float) y;
		framedBox_curr->h = -999;	/* special marker */
		framedBox_curr->next = NULL;
		break;	/* framedBox_curr points to (incomplete) structure */

	case 'D':
		if (dashedBox_start == NULL)
		{	/* first entry */
			dashedBox_start = (struct fig4 *) malloc(sizeof(dashedBox));
			dashedBox_curr = dashedBox_start;
		} else
		{	/* insert at end */
			dashedBox_curr->next = (struct fig4 *) malloc(sizeof(dashedBox));
			dashedBox_curr = dashedBox_curr->next;
		}
		dashedBox_curr->x = (float) x;
		dashedBox_curr->y = (float) y;
		dashedBox_curr->h = -999;	/* special marker */
		dashedBox_curr->next = NULL;
		break;	/* dashedBox_curr points to (incomplete) structure */

	case 'F':
		if (filledBox_start == NULL)
		{	/* first entry */
			filledBox_start = (struct fig2 *) malloc(sizeof(filledBox));
			filledBox_curr = filledBox_start;
		} else
		{	/* insert at end */
			filledBox_curr->next = (struct fig2 *) malloc(sizeof(filledBox));
			filledBox_curr = filledBox_curr->next;
		}
		filledBox_curr->x = (float) x;
		filledBox_curr->y = (float) y;
		filledBox_curr->h = -999;	/* special marker */
		filledBox_curr->next = NULL;
		break;	/* filledBox_curr points to (incomplete) structure */

	case 'C':
		if (kreis_start == NULL)
		{	/* first entry */
			kreis_start = (struct fig2 *) malloc(sizeof(kreis));
			kreis_curr = kreis_start;
		} else
		{	/* insert at end */
			kreis_curr->next = (struct fig2 *) malloc(sizeof(kreis));
			kreis_curr = kreis_curr->next;
		}
		kreis_curr->x = (float) x;
		kreis_curr->y = (float) y;
		kreis_curr->h = -999;	/* special marker */
		kreis_curr->next = NULL;
		break;	/* kreis_curr points to (incomplete) structure */

	case 'B':
		if (disc_start == NULL)
		{	/* first entry */
			disc_start = (struct fig2 *) malloc(sizeof(disc));
			disc_curr = disc_start;
		} else
		{	/* insert at end */
			disc_curr->next = (struct fig2 *) malloc(sizeof(disc));
			disc_curr = disc_curr->next;
		}
		disc_curr->x = (float) x;
		disc_curr->y = (float) y;
		disc_curr->h = -999;	/* special marker */
		disc_curr->next = NULL;
		break;	/* disc_curr points to (incomplete) structure */

	case 'O':
		if (oval_start == NULL)
		{	/* first entry */
			oval_start = (struct fig1 *) malloc(sizeof(oval));
			oval_curr = oval_start;
		} else
		{	/* insert at end */
			oval_curr->next = (struct fig1 *) malloc(sizeof(oval));
			oval_curr = oval_curr->next;
		}
		oval_curr->x = (float) x;
		oval_curr->y = (float) y;
		oval_curr->h = -999;	/* special marker */
		oval_curr->next = NULL;
		break;	/* oval_curr points to (incomplete) structure */

	default:
		break;
	}	/* switch */

	enable_tracker();


}





void
set_TM_btn1_second()
{
	/*
	 * btn1 was pressed for the first time in pboard. Now, prepare the
	 * second btn1-press
	 */

	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Down>: btn1_press_second()";
	static XtActionsRec actions[80] = {{"btn1_press_second", btn1_press_second}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}



void
btn1_press_second(Widget w, XEvent * event, String * id, Cardinal * num)
{
	/* button was pressed in pboard (for the 2nd time) */
	int             x, y, r;
	float           a1, a2, e1, e2;
	Display        *disp;
	Drawable        win;
	char 		ch=graph_action;
	int		lenn=81;


	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	track_it(&x, &y);	/* the rubber-band-tracker */


	switch (graph_action)
	{
	case 'I':
		/* bezier_curr points to incomplete srtucture */
		XtUninstallTranslations(pboard);
		bezier_curr->ex = (float) x;
		bezier_curr->ey = (float) y;
		/* delete the line */
		XDrawLine(disp,win,gc,(int)bezier_curr->ax,(int)bezier_curr->ay,(int)x,(int)y);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			zoomed2real(&bezier_curr->ax,&bezier_curr->ay);
			/* translate second coordinate to real-system */
			zoomed2real(&bezier_curr->ex,&bezier_curr->ey);
		}
		bezier_curr->sx=-999.0; /* special marker */
		set_lastBezier_press(); /* watch 3rd btn1-press */
		break;

	case 'L':	/* correct coordinates */
		valid_line_coords((int) strich_curr->x, (int) strich_curr->y, &x, &y);
		/* strich_curr points to incomplete srtucture */
		strich_curr->h = (float) x;
		strich_curr->v = (float) y;
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = strich_curr->x;
			a2 = strich_curr->y;
			zoomed2real(&a1, &a2);
			strich_curr->x = a1;
			strich_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = strich_curr->h;
			e2 = strich_curr->v;
			zoomed2real(&e1, &e2);
			strich_curr->h = e1;
			strich_curr->v = e2;
		}
		/* create new struct */
		strich_curr->next = (struct fig2 *) malloc(sizeof(strich));
		strich_curr = strich_curr->next;
		strich_curr->x = (float) x;
		strich_curr->y = (float) y;
		strich_curr->h = -999;	/* special marker */
		strich_curr->next = NULL;
		break;	/* strich_curr points to (incomplete) structure */

	case 'V':	/* correct coordinates */
		valid_vector_coords((int) pfeil_curr->x, (int) pfeil_curr->y, &x, &y);
		/* pfeil_curr points to incomplete srtucture */
		pfeil_curr->h = (float) x;
		pfeil_curr->v = (float) y;
		draw_vector_marker(pfeil_curr->x, pfeil_curr->y, pfeil_curr->h, pfeil_curr->v);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = pfeil_curr->x;
			a2 = pfeil_curr->y;
			zoomed2real(&a1, &a2);
			pfeil_curr->x = a1;
			pfeil_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = pfeil_curr->h;
			e2 = pfeil_curr->v;
			zoomed2real(&e1, &e2);
			pfeil_curr->h = e1;
			pfeil_curr->v = e2;
		}
		/* create new struct */
		pfeil_curr->next = (struct fig2 *) malloc(sizeof(pfeil));
		pfeil_curr = pfeil_curr->next;
		pfeil_curr->x = (float) x;
		pfeil_curr->y = (float) y;
		pfeil_curr->h = -999;	/* special marker */
		pfeil_curr->next = NULL;
		break;	/* pfeil_curr points to (incomplete) structure */

	case 'N':	/* framedBox_curr points to incomplete structure */
		/* forget translations */
		XtUninstallTranslations(pboard); 
		framedBox_curr->h = (float) x;
		framedBox_curr->v = (float) y;
		norm_rectangle(&framedBox_curr->x, &framedBox_curr->y,
			       &framedBox_curr->h, &framedBox_curr->v);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = framedBox_curr->x;
			a2 = framedBox_curr->y;
			zoomed2real(&a1, &a2);
			framedBox_curr->x = a1;
			framedBox_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = framedBox_curr->h;
			e2 = framedBox_curr->v;
			zoomed2real(&e1, &e2);
			framedBox_curr->h = e1;
			framedBox_curr->v = e2;
		}
		/* ask for text in box */
		txtpos[0] = 'X';
		txtpos[1] = 'X';
		query_box_text_and_position();
		/* now, txtpos contains the position of the text in your box */
		/* boxinput contains the text-string */
		if (txtpos[0] == '?')	/* cancel option */
			leaving(0, 0, 0, 0);
		else
		{
			if (lenn<1+strlen(boxinput)) lenn=1+strlen(boxinput);
			framedBox_curr->text = (char *) malloc(lenn);
			strcpy(framedBox_curr->text, boxinput);
			strcpy(framedBox_curr->textpos, txtpos);
			/* draw the shadow-style */
			draw_the_shadow((void *) framedBox_curr, nn);
			/* the global variable <shadow> decides, which shadow */
			/* will be drawn */
			left(0, 0, 0, 0);
		}
		graph_action='N';
		print_head(graph_action);
		leave_translation();
		set_first();	
		break;	/* framedBox terminated */

	case 'D':	/* dashedBox_curr points to incomplete structure */
		/* forget translations */
		XtUninstallTranslations(pboard);
		dashedBox_curr->h = (float) x;
		dashedBox_curr->v = (float) y;
		norm_rectangle(&dashedBox_curr->x, &dashedBox_curr->y,
			       &dashedBox_curr->h, &dashedBox_curr->v);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = dashedBox_curr->x;
			a2 = dashedBox_curr->y;
			zoomed2real(&a1, &a2);
			dashedBox_curr->x = a1;
			dashedBox_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = dashedBox_curr->h;
			e2 = dashedBox_curr->v;
			zoomed2real(&e1, &e2);
			dashedBox_curr->h = e1;
			dashedBox_curr->v = e2;
		}
		/* ask for text in box */
		txtpos[0] = 'X';
		txtpos[1] = 'X';
		query_box_text_and_position();
		/* now, txtpos contains the position of the text in your box */
		/* boxinput contains the text-string */
		if (txtpos[0] == '?')	/* cancel option */
			leaving(0, 0, 0, 0);
		else
		{
			if (lenn<1+strlen(boxinput)) lenn=1+strlen(boxinput);
			dashedBox_curr->text = (char *) malloc(lenn);
			strcpy(dashedBox_curr->text, boxinput);
			strcpy(dashedBox_curr->textpos, txtpos);
			dashedBox_curr->dashlength = 0.5;	/* constant */
			/* draw the shadow-style */
			draw_the_shadow((void *) dashedBox_curr, dd);
			/* the global variable <shadow> decides, which shadow */
			/* will be drawn */
			left(0, 0, 0, 0);
		}
		graph_action='D';
		print_head(graph_action);
		leave_translation();
		set_first();	
		break;	/* dashedBox terminated */

	case 'F':	/* filledBox_curr points to incomplete structure */
		/* forget translations */
		XtUninstallTranslations(pboard); 
		filledBox_curr->h = (float) x;
		filledBox_curr->v = (float) y;
		norm_rectangle(&filledBox_curr->x, &filledBox_curr->y,
			       &filledBox_curr->h, &filledBox_curr->v);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = filledBox_curr->x;
			a2 = filledBox_curr->y;
			zoomed2real(&a1, &a2);
			filledBox_curr->x = a1;
			filledBox_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = filledBox_curr->h;
			e2 = filledBox_curr->v;
			zoomed2real(&e1, &e2);
			filledBox_curr->h = e1;
			filledBox_curr->v = e2;
		}
		graph_action='F';
		print_head(graph_action);
		leave_translation();
		set_first();
		break;	/* filledBox terminated */

	case 'C':	/* correct coordinates */
		/* forget translations */
		XtUninstallTranslations(pboard); 
		valid_kreis_coords((int) kreis_curr->x, (int) kreis_curr->y, &x, &y, &r);
		/* kreis_curr points to incomplete srtucture */
		kreis_curr->h = (float) x;
		kreis_curr->v = (float) y;
		kreis_curr->radius = r;
		if (zoomed == True)
			zoomed2real(&kreis_curr->x, &kreis_curr->y);
		kreis_curr->next = NULL;
		graph_action='C';
		leave_translation();
		set_first();
		break;

	case 'B':	/* correct coordinates */
		/* forget translations */
		XtUninstallTranslations(pboard); 
		valid_disc_coords((int) disc_curr->x, (int) disc_curr->y, &x, &y, &r);
		/* disc_curr points to incomplete srtucture */
		disc_curr->h = (float) x;
		disc_curr->v = (float) y;
		disc_curr->radius = r;
		if (zoomed == True)
			zoomed2real(&disc_curr->x, &disc_curr->y);
		disc_curr->next = NULL;
		graph_action='B';
		leave_translation();
		set_first();
		break;

	case 'O':	/* oval_curr points to incomplete structure */
		/* forget translations */
		XtUninstallTranslations(pboard); 
		oval_curr->h = (float) x;
		oval_curr->v = (float) y;
		norm_rectangle(&oval_curr->x, &oval_curr->y,
			       &oval_curr->h, &oval_curr->v);
		if (zoomed == True)
		{
			/* translate first coordinate to real-system */
			a1 = oval_curr->x;
			a2 = oval_curr->y;
			zoomed2real(&a1, &a2);
			oval_curr->x = a1;
			oval_curr->y = a2;

			/* translate second coordinate to real-system */

			e1 = oval_curr->h;
			e2 = oval_curr->v;
			zoomed2real(&e1, &e2);
			oval_curr->h = e1;
			oval_curr->v = e2;
		}
		graph_action='O';
		print_head(graph_action);
		leave_translation();
		set_first();
		break;	/* oval terminated */

	default:
		break;

	}	/* switch */
}





void
enable_tracker()
{
	/* Add to Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<MotionNotify> : track_it() \n";
	static XtActionsRec actions[80] = {{"track_it", action_track_it}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}


void
disable_tracker()
{
	/* Add to Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<MotionNotify> : dummy()\n";
	static XtActionsRec actions[80] = {{"dummy", Dummy}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}


void
action_track_it(Widget widget, XEvent *event, String *params,
		Cardinal *num_params)
{
     int x0, y0;
     track_it(&x0, &y0);
}

void
track_it(int *x0, int *y0)
{
	Display        *disp;
	Drawable        win;
	int             x, y, r, r2;
	float           x_old, y_old, h_old, v_old;
	unsigned int    mask;


	disp = XtDisplay(pboard);
	win = XtWindow(pboard);
	if ((graph_action == 'N') || (graph_action == 'F') || 
            (graph_action == 'D') || (graph_action == 'O') ||
	    (graph_action == 'I'))
		snapPointerPosition(&x, &y, &mask);
	else
		PointerPosition(&x, &y, &mask);

	/* rubber-band-effect */
	switch (graph_action)
	{
	case 'I':
		x_old = bezier_curr->ax;	/* get old coordinates (firstpress) */
		y_old = bezier_curr->ay;

		h_old = bezier_curr->ex;
		v_old = bezier_curr->ey;
		if (h_old != -999)
			XDrawLine(disp, win, gc, (int) x_old, (int) y_old, (int) h_old, (int) v_old);
		/* old line deleted */
		/* -999 is an impossible coordinate and used as marker */
		bezier_curr->ex = (float) x;	/* save new coordinates */
		bezier_curr->ey = (float) y;
		/* draw new line */
		XDrawLine(disp, win, gc, (int) x_old, (int) y_old, x, y);	/* draw new */
		/* return coordinates */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'L':
		x_old = strich_curr->x;	/* get old coordinates (firstpress) */
		y_old = strich_curr->y;

		h_old = strich_curr->h;
		v_old = strich_curr->v;
		if (h_old != -999)
			XDrawLine(disp, win, gc, (int) x_old, (int) y_old, (int) h_old, (int) v_old);
		/* old line deleted */
		/* -999 is an impossible coordinate and used as marker */
		valid_line_coords((int) x_old, (int) y_old, &x, &y);
		strich_curr->h = (float) x;	/* save new coordinates */
		strich_curr->v = (float) y;
		/* draw new line */
		XDrawLine(disp, win, gc, (int) x_old, (int) y_old, x, y);	/* draw new */
		/* return coordinates */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'V':
		x_old = pfeil_curr->x;	/* get old coordinates (firstpress) */
		y_old = pfeil_curr->y;

		h_old = pfeil_curr->h;
		v_old = pfeil_curr->v;
		if (h_old != -999)
			XDrawLine(disp, win, gc, (int) x_old, (int) y_old, (int) h_old, (int) v_old);
		/* old line deleted */
		/* -999 is an impossible coordinate and used as marker */
		valid_vector_coords((int) x_old, (int) y_old, &x, &y);
		pfeil_curr->h = (float) x;	/* save new coordinates */
		pfeil_curr->v = (float) y;
		/* draw new line */
		XDrawLine(disp, win, gc, (int) x_old, (int) y_old, x, y);	/* draw new */
		/* return coordinates */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'N':	/* normate (x,y)-coordinates --> upper left corner.  */
		if (((framedBox_curr->h) != (float) x) || ((framedBox_curr->v) != (float) y))
		{	/* redraw is necessary !! */
			x_old = (framedBox_curr->x);
			y_old = (framedBox_curr->y);
			h_old = (framedBox_curr->h);
			v_old = (framedBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			if ((framedBox_curr->h) != -999)
				XDrawRectangle(disp, win, gc, (int) x_old, (int) y_old,
					     (unsigned int) (h_old - x_old),
					    (unsigned int) (v_old - y_old));
			(framedBox_curr->h) = (float) x;
			(framedBox_curr->v) = (float) y;
			/* new line */
			x_old = (framedBox_curr->x);
			y_old = (framedBox_curr->y);
			h_old = (framedBox_curr->h);
			v_old = (framedBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			XDrawRectangle(disp, win, gc, (int) x_old, (int) y_old,
				       (unsigned int) (h_old - x_old),
				       (unsigned int) (v_old - y_old));
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'D':	/* normate (x,y)-coordinates --> upper left corner.  */
		if (((dashedBox_curr->h) != (float) x) || ((dashedBox_curr->v) != (float) y))
		{	/* redraw is necessary !! */
			x_old = (dashedBox_curr->x);
			y_old = (dashedBox_curr->y);
			h_old = (dashedBox_curr->h);
			v_old = (dashedBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
			if ((dashedBox_curr->h) != -999)
				XDrawRectangle(disp, win, gc, (int) x_old, (int) y_old,
					     (unsigned int) (h_old - x_old),
					    (unsigned int) (v_old - y_old));
			(dashedBox_curr->h) = (float) x;
			(dashedBox_curr->v) = (float) y;
			/* new line */
			x_old = (dashedBox_curr->x);
			y_old = (dashedBox_curr->y);
			h_old = (dashedBox_curr->h);
			v_old = (dashedBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			XDrawRectangle(disp, win, gc, (int) x_old, (int) y_old,
				       (unsigned int) (h_old - x_old),
				       (unsigned int) (v_old - y_old));
			XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'F':	/* normate (x,y)-coordinates --> upper left corner.  */
		if (((filledBox_curr->h) != (float) x) || ((filledBox_curr->v) != (float) y))
		{	/* redraw is necessary !! */
			x_old = (filledBox_curr->x);
			y_old = (filledBox_curr->y);
			h_old = (filledBox_curr->h);
			v_old = (filledBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			if ((filledBox_curr->h) != -999)
				XFillRectangle(disp, win, gc, (int) x_old, (int) y_old,
					     (unsigned int) (h_old - x_old),
					    (unsigned int) (v_old - y_old));
			(filledBox_curr->h) = (float) x;
			(filledBox_curr->v) = (float) y;
			/* new line */
			x_old = (filledBox_curr->x);
			y_old = (filledBox_curr->y);
			h_old = (filledBox_curr->h);
			v_old = (filledBox_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			XFillRectangle(disp, win, gc, (int) x_old, (int) y_old,
				       (unsigned int) (h_old - x_old),
				       (unsigned int) (v_old - y_old));
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'C':
		r2 = kreis_curr->radius;	/* old radius */
		x_old = kreis_curr->x;	/* get old coordinates (firstpress) */
		y_old = kreis_curr->y;
		valid_kreis_coords((int) x_old, (int) y_old, &x, &y, &r);
		if (r != r2)
		{	/* r is new radius *//* redraw is necessary !! */
			h_old = kreis_curr->h;
			if (zoomed == True)
				r2 = r2 * 10;
			if (h_old != -999)
				XDrawArc(disp, win, gc,
				     (int) (x_old) - r2, (int) (y_old) - r2,
					 (unsigned int) (r2 + r2), (unsigned int) (r2 + r2),
					 0, 360 * 64);

			/* old line deleted */
			/*
			 * -999 is an impossible coordinate and used as
			 * marker
			 */
			kreis_curr->h = (float) x;	/* save new coordinates */
			kreis_curr->v = (float) y;
			kreis_curr->radius = r;
			if (zoomed == True)
				r = r * 10;
			/* draw new line */
			XDrawArc(disp, win, gc,
			(int) (x_old - (float) r), (int) (y_old - (float) r),
				 (unsigned int) (r + r), (unsigned int) (r + r), 0, 360 * 64);
			/* return coordinates */
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'B':
		r2 = disc_curr->radius;	/* old radius */
		x_old = disc_curr->x;	/* get old coordinates (firstpress) */
		y_old = disc_curr->y;
		valid_disc_coords((int) x_old, (int) y_old, &x, &y, &r);
		if (r != r2)
		{	/* r is new radius *//* redraw is necessary !! */
			h_old = disc_curr->h;
			if (zoomed == True)
				r2 = r2 * 10;
			if (h_old != -999)
				XFillArc(disp, win, gc,
				     (int) (x_old) - r2, (int) (y_old) - r2,
					 (unsigned int) (r2 + r2), (unsigned int) (r2 + r2),
					 0, 360 * 64);

			/* old line deleted */
			/*
			 * -999 is an impossible coordinate and used as
			 * marker
			 */
			disc_curr->h = (float) x;	/* save new coordinates */
			disc_curr->v = (float) y;
			disc_curr->radius = r;
			if (zoomed == True)
				r = r * 10;
			/* draw new line */
			XFillArc(disp, win, gc,
			(int) (x_old - (float) r), (int) (y_old - (float) r),
				 (unsigned int) (r + r), (unsigned int) (r + r), 0, 360 * 64);
			/* return coordinates */
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	case 'O':	/* normate (x,y)-coordinates --> upper left corner.  */
		if (((oval_curr->h) != (float) x) || ((oval_curr->v) != (float) y))
		{	/* redraw is necessary !! */
			x_old = (oval_curr->x);
			y_old = (oval_curr->y);
			h_old = (oval_curr->h);
			v_old = (oval_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			if ((oval_curr->h) != -999)
				DrawOval((int) x_old, (int) y_old, (int) h_old, (int) v_old);
			(oval_curr->h) = (float) x;
			(oval_curr->v) = (float) y;
			/* new line */
			x_old = (oval_curr->x);
			y_old = (oval_curr->y);
			h_old = (oval_curr->h);
			v_old = (oval_curr->v);
			norm_rectangle(&x_old, &y_old, &h_old, &v_old);
			DrawOval((int) x_old, (int) y_old, (int) h_old, (int) v_old);
		}	/* redraw was necessary */
		(*x0) = x;
		(*y0) = y;
		break;

	default:
		break;

	}	/* switch */

}/* track_it */



void
valid_kreis_coords(int x, int y, int *h, int *v, int *rad)
{
	/*
	 * conclude from the coordinates, if the radius is correct (because
	 * limited)
	 */
	/* if not, return the correct values */

	int             r, i, diameter, zoom;
	double          a, b;
	if (zoomed == True)
		zoom = 10;
	else
		zoom = 1;
	a = (*h) - x;
	b = (*v) - y;
	if (a < 0)
		a = a * (-1);
	if (b < 0)
		b = b * (-1);
	r = (int) sqrt((double) (a * a + b * b));

	if (circle_diameter[0] == -1)
	{	/* no diameter restriction */
		(*rad) = r;
		if (zoomed == True)
		{
			a = (double) r;
			a = a / 10;
			if (a < 1.0)
				a = 1.0;
			(*rad) = (int) a;
		}
		return;
	}
	r += r;	/* r is  diameter */

	i = 0;
	while (((circle_diameter[i]) != -1) && ((zoom * circle_diameter[i]) < r))
		i++;

	if (i == 0)
		diameter = circle_diameter[i];
	else if (circle_diameter[i] == -1)
		diameter = circle_diameter[i - 1];
	else
	{
		/* circle_diameter[i]>=r */
		if ((r - (zoom * circle_diameter[i - 1])) < ((zoom * circle_diameter[i]) - r))
			diameter = circle_diameter[i - 1];
		else
			diameter = circle_diameter[i];
	}

	/* <diameter> represents the correct LaTeX diameter */

	r = (int) (0.5 * (float) diameter);	/* r is correct radius */

	if (r < 1)
		r = 1;

	(*rad) = r;

}


void
valid_disc_coords(int x, int y, int *h, int *v, int *rad)
{
	/* see above */

	int             r, i, diameter, zoom;
	double          a, b;
	if (zoomed == True)
		zoom = 10;
	else
		zoom = 1;
	a = (*h) - x;
	b = (*v) - y;
	if (a < 0)
		a = a * (-1);
	if (b < 0)
		b = b * (-1);
	r = (int) sqrt((double) (a * a + b * b));

	if (disc_diameter[0] == -1)
	{	/* no diameter restriction */
		(*rad) = r;
		if (zoomed == True)
		{
			a = (double) r;
			a = a / 10;
			if (a < 1.0)
				a = 1.0;
			(*rad) = (int) a;
		}
		return;
	}
	r += r;	/* r is  diameter */

	i = 0;
	while (((disc_diameter[i]) != -1) && ((zoom * disc_diameter[i]) < r))
		i++;

	if (i == 0)
		diameter = disc_diameter[i];
	else if (disc_diameter[i] == -1)
		diameter = disc_diameter[i - 1];
	else
	{
		/* disc_diameter[i]>=r */
		if ((r - (zoom * disc_diameter[i - 1])) < ((zoom * disc_diameter[i]) - r))
			diameter = disc_diameter[i - 1];
		else
			diameter = disc_diameter[i];
	}

	/* <diameter> represents the correct LaTeX diameter */

	r = (int) (0.5 * (float) diameter);	/* r is correct radius */

	if (r < 1)
		r = 1;


	(*rad) = r;

}





void
leaving(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	/* users had decided to leave without completion of action */
	int             i, x, y;
	Display        *disp;
	Drawable        win;
	char           *txt;
	int             zoom, rad;


	if (zoomed == True)
		zoom = 10;
	else
		zoom = 1;

	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	/* throw away the incomplete struct */
	switch (graph_action)
	{
	case 'I':	/* delete last line of bezier*/
		if (bezier_curr == bezier_start)
		{	/* first entry */
			if (bezier_curr->ex!=-999.0)
				XDrawLine(disp, win, gc, (int) bezier_curr->ax, (int) bezier_curr->ay,
					(int) bezier_curr->ex, (int) bezier_curr->ey);
			free(bezier_curr);	/* give memory back to system */
			bezier_curr = NULL;
			bezier_start = NULL;
		} else
		{
			for (bezier_marker = (struct fig6 *) bezier_start;
			     (bezier_marker->next != bezier_curr); bezier_marker = bezier_marker->next);

			if (bezier_curr->ex!=-999.0)
				XDrawLine(disp, win, gc, (int) bezier_curr->ax, (int) bezier_curr->ay,
					(int) bezier_curr->ex, (int) bezier_curr->ey);
			free(bezier_curr);
			bezier_curr = bezier_marker;	/* points to last
							 * complete struct */
		}
		break;

	case 'L':	/* delete last line */
		if (strich_curr == strich_start)
		{	/* first entry */
			if (strich_curr->h!=-999.0)
				XDrawLine(disp, win, gc, (int) strich_curr->x, (int) strich_curr->y,
					(int) strich_curr->h, (int) strich_curr->v);
			free(strich_curr);	/* give memory back to system */
			strich_curr = NULL;
			strich_start = NULL;
		} else
		{
			for (strich_marker = (struct fig2 *) strich_start;
			     (strich_marker->next != strich_curr); strich_marker = strich_marker->next);

			if (strich_curr->h!=-999.0)
				XDrawLine(disp, win, gc, (int) strich_curr->x, (int) strich_curr->y,
					(int) strich_curr->h, (int) strich_curr->v);
			free(strich_curr);
			strich_curr = strich_marker;	/* points to last
							 * complete struct */
		}
		break;

	case 'V':	/* delete last vector */
		if (pfeil_curr == pfeil_start)
		{	/* first entry */
			if (pfeil_curr->h!=-999.0)
				XDrawLine(disp, win, gc, (int) pfeil_curr->x, (int) pfeil_curr->y,
					  (int) pfeil_curr->h, (int) pfeil_curr->v);
			free(pfeil_curr);	/* give memory back to system */
			pfeil_curr = NULL;
			pfeil_start = NULL;
		} else
		{
			for (pfeil_marker = (struct fig2 *) pfeil_start;
			     (pfeil_marker->next != pfeil_curr); pfeil_marker = pfeil_marker->next);
			if (pfeil_curr->h!=-999.0)
				XDrawLine(disp, win, gc, (int) pfeil_curr->x, (int) pfeil_curr->y,
					  (int) pfeil_curr->h, (int) pfeil_curr->v);
			free(pfeil_curr);
			pfeil_curr = pfeil_marker;	/* points to last
							 * complete struct */
		}
		break;

	case 'N':	/* framed box: pointer was moved out of pboard ->
			 * cancel operation ! */
		norm_rectangle(&framedBox_curr->x, &framedBox_curr->y,
			       &framedBox_curr->h, &framedBox_curr->v);
		if (framedBox_curr == framedBox_start)
		{	/* first entry */
			XDrawRectangle(disp, win, gc, (int) framedBox_curr->x, (int) framedBox_curr->y,
				       (unsigned int) ((int) ((framedBox_curr->h) - (framedBox_curr->x))),
				       (unsigned int) ((int) ((framedBox_curr->v) - (framedBox_curr->y))));
			free(framedBox_curr);	/* give memory back to system */
			framedBox_curr = NULL;
			framedBox_start = NULL;
		} else
		{
			for (framedBox_marker = (struct fig3 *) framedBox_start;
			     (framedBox_marker->next != framedBox_curr); framedBox_marker = framedBox_marker->next);
			XDrawRectangle(disp, win, gc, (int) framedBox_curr->x, (int) framedBox_curr->y,
				       (unsigned int) ((int) ((framedBox_curr->h) - (framedBox_curr->x))),
				       (unsigned int) ((int) ((framedBox_curr->v) - (framedBox_curr->y))));
			free(framedBox_curr);
			framedBox_curr = framedBox_marker;	/* points to last
								 * complete struct */
		}
		break;

	case 'D':	/* dashed box: pointer was moved out of pboard ->
			 * cancel operation ! */
		norm_rectangle(&dashedBox_curr->x, &dashedBox_curr->y,
			       &dashedBox_curr->h, &dashedBox_curr->v);
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
		if (dashedBox_curr == dashedBox_start)
		{	/* first entry */
			XDrawRectangle(disp, win, gc, (int) dashedBox_curr->x, (int) dashedBox_curr->y,
				       (unsigned int) ((int) ((dashedBox_curr->h) - (dashedBox_curr->x))),
				       (unsigned int) ((int) ((dashedBox_curr->v) - (dashedBox_curr->y))));
			free(dashedBox_curr);	/* give memory back to system */
			dashedBox_curr = NULL;
			dashedBox_start = NULL;
		} else
		{
			for (dashedBox_marker = (struct fig4 *) dashedBox_start;
			     (dashedBox_marker->next != dashedBox_curr); dashedBox_marker = dashedBox_marker->next);
			XDrawRectangle(disp, win, gc, (int) dashedBox_curr->x, (int) dashedBox_curr->y,
				       (unsigned int) ((int) ((dashedBox_curr->h) - (dashedBox_curr->x))),
				       (unsigned int) ((int) ((dashedBox_curr->v) - (dashedBox_curr->y))));
			free(dashedBox_curr);
			dashedBox_curr = dashedBox_marker;	/* points to last
								 * complete struct */
		}
		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
		break;

	case 'F':	/* filled box: pointer was moved out of pboard ->
			 * cancel operation ! */
		norm_rectangle(&filledBox_curr->x, &filledBox_curr->y,
			       &filledBox_curr->h, &filledBox_curr->v);
		if (filledBox_curr == filledBox_start)
		{	/* first entry */
			XFillRectangle(disp, win, gc, (int) filledBox_curr->x, (int) filledBox_curr->y,
				       (unsigned int) ((int) ((filledBox_curr->h) - (filledBox_curr->x))),
				       (unsigned int) ((int) ((filledBox_curr->v) - (filledBox_curr->y))));
			free(filledBox_curr);	/* give memory back to system */
			filledBox_curr = NULL;
			filledBox_start = NULL;
		} else
		{
			for (filledBox_marker = (struct fig2 *) filledBox_start;
			     (filledBox_marker->next != filledBox_curr); filledBox_marker = filledBox_marker->next);
			XFillRectangle(disp, win, gc, (int) filledBox_curr->x, (int) filledBox_curr->y,
				       (unsigned int) ((int) ((filledBox_curr->h) - (filledBox_curr->x))),
				       (unsigned int) ((int) ((filledBox_curr->v) - (filledBox_curr->y))));
			free(filledBox_curr);
			filledBox_curr = filledBox_marker;	/* points to last
								 * complete struct */
		}
		break;

	case 'C':	/* delete last normal circle */
		if (kreis_curr == kreis_start)
		{	/* first entry */
			rad = zoom * (kreis_curr->radius);
			XDrawArc(disp, win, gc, (int) (kreis_curr->x) - rad,
				 (int) (kreis_curr->y) - rad,
				 (unsigned int) (rad + rad),
				 (unsigned int) (rad + rad),
				 0, 360 * 64);
			free(kreis_curr);	/* give memory back to system */
			kreis_curr = NULL;
			kreis_start = NULL;
		} else
		{
			for (kreis_marker = (struct fig2 *) kreis_start;
			     (kreis_marker->next != kreis_curr); kreis_marker = kreis_marker->next);

			rad = zoom * (kreis_curr->radius);
			XDrawArc(disp, win, gc, (int) (kreis_curr->x) - rad,
				 (int) (kreis_curr->y) - rad,
				 (unsigned int) (rad + rad),
				 (unsigned int) (rad + rad),
				 0, 360 * 64);
			free(kreis_curr);
			kreis_curr = kreis_marker;	/* points to last
							 * complete struct */
		}
		break;

	case 'B':	/* delete last filled circle */
		if (disc_curr == disc_start)
		{	/* first entry */
			rad = zoom * (disc_curr->radius);
			XFillArc(disp, win, gc, (int) (disc_curr->x) - rad,
				 (int) (disc_curr->y) - rad,
				 (unsigned int) (rad + rad),
				 (unsigned int) (rad + rad),
				 0, 360 * 64);
			free(disc_curr);	/* give memory back to system */
			disc_curr = NULL;
			disc_start = NULL;
		} else
		{
			for (disc_marker = (struct fig2 *) disc_start;
			     (disc_marker->next != disc_curr); disc_marker = disc_marker->next);

			rad = zoom * (disc_curr->radius);
			XFillArc(disp, win, gc, (int) (disc_curr->x) - rad,
				 (int) (disc_curr->y) - rad,
				 (unsigned int) (rad + rad),
				 (unsigned int) (rad + rad),
				 0, 360 * 64);
			free(disc_curr);
			disc_curr = disc_marker;	/* points to last
							 * complete struct */
		}
		break;

	case 'O':	/* oval: pointer was moved out of pboard -> cancel
			 * operation ! */
		norm_rectangle(&oval_curr->x, &oval_curr->y,
			       &oval_curr->h, &oval_curr->v);
		if (oval_curr == oval_start)
		{	/* first entry */
			DrawOval((int) oval_curr->x, (int) oval_curr->y,
				 (int) oval_curr->h, (int) oval_curr->v);

			free(oval_curr);	/* give memory back to system */
			oval_curr = NULL;
			oval_start = NULL;
		} else
		{
			for (oval_marker = (struct fig1 *) oval_start;
			     (oval_marker->next != oval_curr); oval_marker = oval_marker->next);
			DrawOval((int) oval_curr->x, (int) oval_curr->y,
				 (int) oval_curr->h, (int) oval_curr->v);
			free(oval_curr);
			oval_curr = oval_marker;	/* points to last
							 * complete struct */
		}
		break;


	default:
		graph_action = '?';
		break;
	}	/* switch */

	/* throw away all translations */
	XtUninstallTranslations(pboard);

	left(0, 0, 0, 0);

}/* leaving */



void
left(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	headline(toplevel, titlename);
	graph_action = '?';
}


void
norm_rectangle(float *x, float *y, float *h, float *v)
{/* (x,y) and (h,v) are opposite (diagonal) coordinates  of a rectangle */
	/* this routine transforms (x,y) to the upper left... */

	float           a1, a2;
	if (((*h) <= (*x)) && ((*v) <= (*y)))
	{
		a1 = (*x);
		a2 = (*y);
		(*x) = (*h);
		(*y) = (*v);
		(*h) = a1;
		(*v) = a2;
	} else if (((*h) >= (*x)) && ((*v) <= (*y)))
	{
		a1 = (*y);
		(*y) = (*v);
		(*v) = a1;
	} else if (((*h) <= (*x)) && ((*v) >= (*y)))
	{
		a1 = (*x);
		(*x) = (*h);
		(*h) = a1;
	}
	/*
	 * if ( ((*h)>=(*x)) && ((*v)>=(*y)) )  this is already the right
	 * case !
	 */
}



void
zoomed2real(float *x, float *y)
{
	/*
	 * translates the coordinates of a zoomed screen into the
	 * corresponding real-ones
	 */

	float           c,test;


	/* y coordinate translation */

	c = ((float) y_A4_max - (*y)) / (*y);
	test=((float) (c + 1.0));
	test=(test==0) ? 0.0001 : test;
	(*y) = ((float) y_zoom_max + (c * (float) y_zoom_min)) / test;

	/* x coordinate translation */

	c = ((float) x_A4_max - (*x)) / (*x);
	test=((float) (c + 1.0));
	test=(test==0) ? 0.0001 : test;
	(*x) = ((float) x_zoom_max + c * (float) x_zoom_min) / test;

}







void
real2zoomed(float *x, float *y)
{
	/*
	 * this function transfers coordinates from within the zoom-area to
	 * the corresponding (zoomed) coordinates on the screen
	 */

	float           c,test;


	/* y coordinate translation */
	test=((*y) - (float) y_zoom_min);
	test=(test==0) ? 0.0001 : test;
	c = ((float) y_zoom_max - (*y)) / test;
	
	test=(float) (c + 1.0);
	test=(test==0) ? 0.0001 : test;
	(*y) = ((float) y_A4_max) / test;

	/* x coordinate translation */

	test=((*x) - (float) x_zoom_min);
	test=(test==0) ? 0.0001 : test;
	c = ((float) x_zoom_max - (*x)) / test;
	
	test=(float) (c + 1.0);
	test=(test==0) ? 0.0001 : test;
	(*x) = ((float) x_A4_max) / test;




}






int
ggt(int a, int b)
{
	/* compute the biggest common divisor of a and b */
	/* (c) by Euklid  */

	int             r;
	if (b != 0)
		for (r = 1; r != 0; r = (a % b), a = b, b = r);

	return a;
}


void
valid_length(int x, int y, int *x_ret, int *y_ret)
{
	/* if the length of a line/vector is less than 10 points */
	/* the coordinates have to be adjusted. this is an option in */
	/* the settings-menu */

	float           lambda;
	int             newh, newv;
	int             h = (*x_ret);
	int             v = (*y_ret);
	int             zm = (zoomed == True) ? 10 * line_length : line_length;


	/* disabled ? */
	if (line_length == 0)
		return;


	/* line shorter than 10 points ? */
	if ((abs(1 + x - h) >= zm))
		return;

	if ((x == h) || (y == v))	/* there is no restriction at horiz.
					 * and vert. lines */
		return;
	else
	{
		newh = (h < x) ? (x - zm) : (x + zm);

		lambda = (newh - x) / (h - x);

		newv = y + lambda * (v - y);
	}


	(*x_ret) = newh;
	(*y_ret) = newv;

}


void
draw_vector_marker(float x, float y, float h, float v)
{
	/* draws an arrow at the top of the line, given by the */
	/* the coordinates. */

	Display        *disp;
	Drawable        win;
	float           betrag;
	float           a, b, c, d, lx, ly, rx, ry;
	XPoint          points[3];
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);


	a = (h - x);
	b = (v - y);
	betrag = sqrt((double) (a * a + b * b));

	a = a / betrag;
	b = b / betrag;
	/* vector |(a,b)|=1 */

	/* reverse direction */
	a = a * (-1);
	b = b * (-1);

	h = h + a;
	v = v + b;

	/* knot point (c,d) */
	c = h + 10 * a;
	d = v + 10 * b;

	a = a * (-1);
	rx = b;
	b = a;
	a = rx;

	rx = c + 5 * a;
	ry = d + 5 * b;

	a = a * (-1);
	b = b * (-1);

	lx = c + 5 * a;
	ly = d + 5 * b;

	points[0].x = (short) lx;
	points[0].y = (short) ly;
	points[1].x = (short) rx;
	points[1].y = (short) ry;
	points[2].x = (short) h;
	points[2].y = (short) v;

	XFillPolygon(disp, win, gc, points, 3, Convex, CoordModeOrigin);
	/*
	 * XDrawLine(disp,win,gc,(int)lx,(int)ly,(int)rx,(int)ry);
	 * XDrawLine(disp,win,gc,(int)rx,(int)ry,(int)h,(int)v);
	 * XDrawLine(disp,win,gc,(int)h,(int)v,(int)lx,(int)ly);
	 */


}




int
valid_vector_coords(int x_origin, int y_origin, int *x_ret, int *y_ret)
{
	/*
	 * LaTeX supports only several arrow-slopes... this routine garantees
	 * correct LaTeX-slopes and, furthermore, dependent on different
	 * TeX-implementations, other (& unlimited) slopes, too .
	 */
	/* slope = y/x */
	/*
	 * input : 2 coordinates which represent the line output : 1
	 * (adjusted) coordinate
	 */

	float           x, y;	/* real slope as fraction */
	float           h, v;	/* best approximating slope as fraction */
	int             i, sign;	/* sign indicates positive or
					 * negative slope */
	float           vgl, vgl2;	/* real slope */
	float           lambda1, lambda2;
	float           x_new, y_new;	/* (x_new,y_ret) or (x_ret,y_new) are
					 * the best new coordinates of the
					 * endpoint of the line */


	/* the minimum length is 10 points */
	/* correct it, if necessary ...call <valid_length> to do this... */

	if (arrow_slope == 0)
	{	/* unlimited slope require no coordinate-modification */
		valid_length(x_origin, y_origin, x_ret, y_ret);
		return -1;
	}
	x = (float) (*x_ret - x_origin);
	y = (float) (*y_ret - y_origin);

	/* exceptions: if slope is zero or infinite return directly */
	if ((x == 0) || (y == 0))
	{
		valid_length(x_origin, y_origin, x_ret, y_ret);
		return -2;
	}
	vgl = y / x;	/* the real (positive|negative) slope */
	sign = (vgl > 0) ? 1 : -1;
	if (vgl < 0)
		vgl = vgl * sign;

	/* searching the table */
	for (i = 0; (a_slope[i][0] < vgl); i++);
	/*
	 * i points to the first element, which is equal to/bigger than 'vgl'
	 * ( or it points to the marker
	 */
	if (a_slope[i][0] == 999.0)
	{
		if (vgl > (float) (2 * arrow_slope))
		{
			*x_ret = x_origin;	/* slope is infinite */
			valid_length(x_origin, y_origin, x_ret, y_ret);
			return -3;
		} else
			i--;
	} else if (i == 0)
		if (vgl < (float) (0.5 * a_slope[0][0]))
		{
			*y_ret = y_origin;
			valid_length(x_origin, y_origin, x_ret, y_ret);
			return -4;
		}
	/* compute best approximating slope */
	if (i != 0)
		if ((vgl - a_slope[i - 1][0]) < (a_slope[i][0] - vgl))
			i--;
	/* i points to the best slope */


	/* compute new coordinates */
	/* it should be the nearest one to the original coordinate */
	/* see documentation... */
	h = (float) (a_slope[i][1] * sign);
	v = a_slope[i][2];
	vgl2 = a_slope[i][0];

	/* compute new */

	/* lambda first */
	lambda1 = x / h;
	lambda2 = y / v;

	x_new = (float) x_origin + lambda2 * h;
	y_new = (float) y_origin + lambda1 * v;


	h = x_new - (float) (*x_ret);
	v = y_new - (float) (*y_ret);
	if (h < 0)
		h *= (-1);
	if (v < 0)
		v *= (-1);
	if (h < v)
		*x_ret = (int) x_new;
	else
		*y_ret = (int) y_new;

	/* coordinates are corrected */

	valid_length(x_origin, y_origin, x_ret, y_ret);
	return i;

}


int
valid_line_coords(int x_origin, int y_origin, int *x_ret, int *y_ret)
{
	/*
	 * LaTeX supports only a small number of line-slopes... this routine
	 * garantees correct LaTeX-slopes and, furthermore, dependent on
	 * different TeX-implementations, other (& unlimited) slopes, too .
	 */
	/* slope = y/x */
	/*
	 * input : 2 coordinates which represent the vector output : 1
	 * (adjusted) coordinate
	 */

	float           x, y;	/* real slope as fraction */
	float           h, v;	/* best approximating slope as fraction */
	int             i, sign;	/* sign indicates positive or
					 * negative slope */
	float           vgl, vgl2;	/* real slope */
	float           lambda1, lambda2;
	float           x_new, y_new;	/* (x_new,y_ret) or (x_ret,y_new) are
					 * the best new coordinates of the
					 * endpoint of the line */


	/* the minimum length is 10 points */


	if (line_slope == 0)
	{	/* unlimited slope require no coordinate-modification */
		valid_length(x_origin, y_origin, x_ret, y_ret);
		return -1;
	}
	x = (float) (*x_ret - x_origin);
	y = (float) (*y_ret - y_origin);

	/* exceptions: if slope is zero or infinite return directly */
	if ((x == 0) || (y == 0))
	{
		valid_length(x_origin, y_origin, x_ret, y_ret);
		return -2;
	}
	vgl = y / x;	/* the real (positive|negative) slope */
	sign = (vgl > 0) ? 1 : -1;
	if (vgl < 0)
		vgl = vgl * sign;

	/* searching the table */
	for (i = 0; (l_slope[i][0] < vgl); i++);
	/*
	 * i points to the first element, which is equal to/bigger than 'vgl'
	 * ( or it points to the marker
	 */
	if (l_slope[i][0] == 999.0)
	{
		if (vgl > (float) (2 * line_slope))
		{
			*x_ret = x_origin;	/* slope is infinite */
			valid_length(x_origin, y_origin, x_ret, y_ret);
			return -3;
		} else
			i--;
	} else if (i == 0)
		if (vgl < (float) (0.5 * l_slope[0][0]))
		{
			*y_ret = y_origin;
			valid_length(x_origin, y_origin, x_ret, y_ret);
			return -4;
		}
	/* compute best approximating slope */
	if (i != 0)
		if ((vgl - l_slope[i - 1][0]) < (l_slope[i][0] - vgl))
			i--;
	/* i points to the best slope */


	/* compute new coordinates */
	/* it should be the nearest one to the original coordinate */
	/* see documentation... */
	h = (float) (l_slope[i][1] * sign);
	v = l_slope[i][2];
	vgl2 = l_slope[i][0];

	/* compute new */

	/* lambda first */
	lambda1 = x / h;
	lambda2 = y / v;

	x_new = (float) x_origin + lambda2 * h;
	y_new = (float) y_origin + lambda1 * v;


	h = x_new - (float) (*x_ret);
	v = y_new - (float) (*y_ret);
	if (h < 0)
		h *= (-1);
	if (v < 0)
		v *= (-1);
	if (h < v)
		*x_ret = (int) x_new;
	else
		*y_ret = (int) y_new;

	/* coordinates are corrected */

	valid_length(x_origin, y_origin, x_ret, y_ret);
	return i;
}



void
shorten(int *x, int *y)
{
	int             divi;
	divi = ggt(*x, *y);
	(*x) /= divi;
	(*y) /= divi;
}





void
init_l_slope()
{
	/* builds the line-slope-table */

	int             x, y;
	float           r;
	l_slope[0][0] = 999.0;	/* init marker */
	for (x = line_slope; x != 0; x--)
		for (y = line_slope; y != 0; y--)
		{
			r = (float) x / (float) y;
			l_insert(r, y, x);
			r = (float) y / (float) x;
			l_insert(r, x, y);
		}
	/* slope-table for LaTeX' lines now prepared */



}


void
l_insert(float value, int x, int y)
{
	/* insertion sort into the table */

	int             i, j, k;
	/* kuerzen */
	shorten(&x, &y);

	/* searching for insertion position */
	j = 0;
	while ((l_slope[j][0] != 999.0) && (l_slope[j][0] < value))
		j++;
	/*
	 * j is new insertion point; it may point to 999 or any value, bigger
	 * than 'value'
	 */

	/* testing, if value already in table */
	if (l_slope[j][0] == value)
		return;	/* no entry */

	/* searching for the end of the table */
	i = 0;
	if (l_slope[0][0] != 999.0)
		do
			i += 1;
		while (l_slope[i][0] != 999.0);
	/* i points to 999 */

	/* insert data */
	l_slope[i + 1][0] = 999.0;
	for (k = i; k != j; k--)
	{
		l_slope[k][0] = l_slope[k - 1][0];
		l_slope[k][1] = l_slope[k - 1][1];
		l_slope[k][2] = l_slope[k - 1][2];
	}
	/* insert new data at j */
	l_slope[j][0] = value;
	l_slope[j][1] = x;
	l_slope[j][2] = y;
}/* end */









void
init_a_slope()
{
	/* see above */

	int             x, y;
	float           r;
	a_slope[0][0] = 999.0;	/* init marker */
	for (x = arrow_slope; x != 0; x--)
		for (y = arrow_slope; y != 0; y--)
		{
			r = (float) x / (float) y;
			a_insert(r, y, x);
			r = (float) y / (float) x;
			a_insert(r, x, y);
		}
	/* slope-table for LaTeX' arrows now prepared */


}



void
a_insert(float value, int x, int y)
{
	/* see above */

	int             i, j, k;
	/* kuerzen */
	shorten(&x, &y);

	/* searching for insertion position */
	j = 0;
	while ((a_slope[j][0] != 999.0) && (a_slope[j][0] < value))
		j++;
	/*
	 * j is new insertion point; it may point to 999 or any value, bigger
	 * than 'value'
	 */

	/* testing, if value already in table */
	if (a_slope[j][0] == value)
		return;	/* no entry */

	/* searching for the end of the table */
	i = 0;
	if (a_slope[0][0] != 999.0)
		do
			i += 1;
		while (a_slope[i][0] != 999.0);
	/* i points to 999 */

	/* insert data */
	a_slope[i + 1][0] = 999.0;
	for (k = i; k != j; k--)
	{
		a_slope[k][0] = a_slope[k - 1][0];
		a_slope[k][1] = a_slope[k - 1][1];
		a_slope[k][2] = a_slope[k - 1][2];
	}
	/* insert new data at j */
	a_slope[j][0] = value;
	a_slope[j][1] = x;
	a_slope[j][2] = y;
}/* end */






void
graphics_init()
{

	make_GCcontext();	/* GC Kreiren */

	init_l_slope();
	init_a_slope();
}


void
set_zoom_window()
{
	Widget          w;
	caddr_t         call_data;
	/* Add to Translation Manager */
	XtTranslations  trans_table;
	char            destination[150] = "<MotionNotify> : frame_track() \n\
		        <Btn1Down> : zoom_it() \n\
			<LeaveWindow> : cancel_zoom()\n";
	static XtActionsRec actions[150] = {{"frame_track", action_frame_track},
	{"zoom_it", zoom_it},
	{"cancel_zoom", cancel_zoom}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

	if (zoomed == True)
	{
		zoomed = False;
		refresh(w, &point, call_data);	/* force refresh */
	}
	x_zoom_min = -999;
}


void
action_frame_track(Widget widget, XEvent *event, String *params,
		   Cardinal *num_params)
{
     int x0, y0;
     frame_track(&x0, &y0);
}

void
frame_track(int *x0, int *y0)
{
	/* tracks the zoom-frame */

	Display        *disp;
	Drawable        win;
	int             x, y;
	unsigned int    mask;


	disp = XtDisplay(pboard);
	win = XtWindow(pboard);
	PointerPosition(&x, &y, &mask);

	if ((x != x_zoom_min) || (y != x_zoom_max))
	{	/* redraw of frame is necessary */
		if (x_zoom_min != -999)	/* delete last frame */
			XDrawRectangle(disp, win, gc, x_zoom_min, x_zoom_max, 60, 84);
		/* draw current frame */
		XDrawRectangle(disp, win, gc, x, y, 60, 84);
		x_zoom_min = x;
		x_zoom_max = y;
	}
	(*x0) = x;
	(*y0) = y;

}

void
zoom_it(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	/* zoom the area */

	Display        *disp;
	Drawable        win;
	int             x, y;
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);
	frame_track(&x, &y);
	zoomed = True;
	/* delete last frame */
	XDrawRectangle(disp, win, gc, x_zoom_min, x_zoom_max, 60, 84);
	/* set coordinates */
	/* 84x60 are the proportions of a DIN A4 page */
	if (x > 600 - 60)
		x = 540;
	if (y > (840 - 84))
		y = (840 - 84);
	x_zoom_min = x;
	x_zoom_max = x + 59;
	y_zoom_min = y;
	y_zoom_max = y + 83;
	/* terminate zoom */
	/* display zoomed graphics */
	show_zoomed_objects();
	graph_action = '?';
	/* uninstall zoom-translations */
	XtUninstallTranslations(pboard);
	left(0, 0, 0, 0);

}



void
clear_zoom()
{
	int             x, y;
	x_zoom_min = -999;
	graph_action = '?';
	/* uninstall zoom-translations */
	XtUninstallTranslations(pboard);

}


void
cancel_zoom(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	int             x, y;
	Display        *disp;
	Drawable        win;
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);
	/* redraw last frame */
	frame_track(&x, &y);
	XDrawRectangle(disp, win, gc, x, y, 60, 84);
	clear_zoom();
	left(0, 0, 0, 0);
}



void
event_refresh(Widget cmd, XtPointer closure, XEvent *event,
	      Boolean *continue_to_dispatch)
{
     refresh(cmd, closure, 0);
}

void
refresh(Widget cmd, XtPointer closure, XtPointer call_data)
{
	/* refreshes the screen. May occur by an user request, but */
	/* usually by an expose event */

	Display        *disp;
	Drawable        win;
	char		id;

	if (closure)
	     id = *(char *)closure;
	else
	     id = '\0';

	/* needed for correct cross painting... */
	h_cross = -999;
	v_cross = -999;

	if ((zoomed == True) && (id != '.'))
	{
		/* refresh button was NOT pressed, it was NO forced refresh */
		/* it was an expose refresh */
		show_zoomed_objects();
		return;
	}
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	zoomed = False;
	/* clear window */
	XClearWindow(disp, win);

	if (ruler == True)
		ruler_on();

	if (raster == True)
		raster_on();

	if (cross == True)
		draw_coords(cmd, 0, 0, 0);	/* dummy parameter */


	/* redraw objects */

	/* bezier */
	if (bezier_start != NULL)
	{	/* min. 1 entry */
		bezier_marker = bezier_start;
		while (bezier_marker != bezier_curr)
		{
			DrawBezier(bezier_marker->ax,bezier_marker->ay,
			    	   bezier_marker->ex,bezier_marker->ey,
				   bezier_marker->sx,bezier_marker->sy);
			bezier_marker = bezier_marker->next;
		}
		/* last curve */
			DrawBezier(bezier_curr->ax,bezier_curr->ay,
			  	   bezier_curr->ex,bezier_curr->ey,
				   bezier_curr->sx,bezier_curr->sy);
	}	/* bezier curves */

	/* lines */
	if (strich_start != NULL)
	{	/* min. 1 entry */
		strich_marker = strich_start;
		while (strich_marker != strich_curr)
		{
			XDrawLine(disp, win, gc, (int) strich_marker->x, (int) strich_marker->y,
			    (int) strich_marker->h, (int) strich_marker->v);
			strich_marker = strich_marker->next;
		}
		/* last line */
		XDrawLine(disp, win, gc, (int) strich_curr->x, (int) strich_curr->y,
			  (int) strich_curr->h, (int) strich_curr->v);
	}	/* lines */
	/* vectors */
	if (pfeil_start != NULL)
	{	/* min. 1 entry */
		pfeil_marker = pfeil_start;
		while (pfeil_marker != pfeil_curr)
		{
			XDrawLine(disp, win, gc, (int) pfeil_marker->x, (int) pfeil_marker->y,
			      (int) pfeil_marker->h, (int) pfeil_marker->v);
			draw_vector_marker(pfeil_marker->x, pfeil_marker->y,
					   pfeil_marker->h, pfeil_marker->v);
			pfeil_marker = pfeil_marker->next;
		}
		/* last line */
		XDrawLine(disp, win, gc, (int) pfeil_curr->x, (int) pfeil_curr->y,
			  (int) pfeil_curr->h, (int) pfeil_curr->v);
		draw_vector_marker(pfeil_curr->x, pfeil_curr->y, pfeil_curr->h, pfeil_curr->v);
	}	/* vectors */
	/* text */
	if (message_start != NULL)
	{	/* min. 1 entry */
		message_marker = message_start;
		while (message_marker != message_curr)
		{
			XDrawString(disp, win, gc, (int) message_marker->x,
				    (int) message_marker->y,
				    message_marker->text, 1);
			message_marker = message_marker->next;
		}
		/* last text */
		XDrawString(disp, win, gc, (int) message_curr->x,
			    (int) message_curr->y,
			    message_curr->text, 1);
	}	/* text */
	/* boxes */
	/* FRAMED */
	if (framedBox_start != NULL)
	{	/* min. 1 entry */
		framedBox_marker = framedBox_start;
		while (framedBox_marker != framedBox_curr)
		{
			XDrawRectangle(disp, win, gc, (int) framedBox_marker->x, (int) framedBox_marker->y,
				       (unsigned int) ((int) ((framedBox_marker->h) - (framedBox_marker->x))),
				       (unsigned int) ((int) ((framedBox_marker->v) - (framedBox_marker->y))));

			print_box_text(framedBox_marker->textpos, framedBox_marker->x,
				   framedBox_marker->y, framedBox_marker->h,
			       framedBox_marker->v, framedBox_marker->text);

			framedBox_marker = framedBox_marker->next;
		}
		/* last one */
		XDrawRectangle(disp, win, gc, (int) framedBox_curr->x, (int) framedBox_curr->y,
			       (unsigned int) ((int) ((framedBox_curr->h) - (framedBox_curr->x))),
			       (unsigned int) ((int) ((framedBox_curr->v) - (framedBox_curr->y))));

		print_box_text(framedBox_curr->textpos, framedBox_curr->x,
			       framedBox_curr->y, framedBox_curr->h,
			       framedBox_curr->v, framedBox_curr->text);

	}	/* framed */
	/* DASHED */
	if (dashedBox_start != NULL)
	{	/* min. 1 entry */
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);

		dashedBox_marker = dashedBox_start;
		while (dashedBox_marker != dashedBox_curr)
		{
			XDrawRectangle(disp, win, gc, (int) dashedBox_marker->x, (int) dashedBox_marker->y,
				       (unsigned int) ((int) ((dashedBox_marker->h) - (dashedBox_marker->x))),
				       (unsigned int) ((int) ((dashedBox_marker->v) - (dashedBox_marker->y))));

			print_box_text(dashedBox_marker->textpos, dashedBox_marker->x,
				   dashedBox_marker->y, dashedBox_marker->h,
			       dashedBox_marker->v, dashedBox_marker->text);

			dashedBox_marker = dashedBox_marker->next;
		}
		/* last one */
		XDrawRectangle(disp, win, gc, (int) dashedBox_curr->x, (int) dashedBox_curr->y,
			       (unsigned int) ((int) ((dashedBox_curr->h) - (dashedBox_curr->x))),
			       (unsigned int) ((int) ((dashedBox_curr->v) - (dashedBox_curr->y))));

		print_box_text(dashedBox_curr->textpos, dashedBox_curr->x,
			       dashedBox_curr->y, dashedBox_curr->h,
			       dashedBox_curr->v, dashedBox_curr->text);

		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);

	}	/* dashed */
	/* FILLED */
	if (filledBox_start != NULL)
	{	/* min. 1 entry */
		filledBox_marker = filledBox_start;
		while (filledBox_marker != filledBox_curr)
		{
			/* fill it */
			XFillRectangle(disp, win, gc, (int) filledBox_marker->x,
				       (int) (filledBox_marker->y),
				       (unsigned int) ((filledBox_marker->h) - (filledBox_marker->x)),
				       (unsigned int) ((filledBox_marker->v) - (filledBox_marker->y)));

			filledBox_marker = filledBox_marker->next;
		}
		/* last one */
		XFillRectangle(disp, win, gc, (int) filledBox_curr->x,
			       (int) (filledBox_curr->y),
		 (unsigned int) ((filledBox_curr->h) - (filledBox_curr->x)),
		(unsigned int) ((filledBox_curr->v) - (filledBox_curr->y)));

	}	/* filled */
	/* circle */
	/* normal */
	if (kreis_start != NULL)
	{	/* min. 1 entry */
		kreis_marker = kreis_start;
		while (kreis_marker != kreis_curr)
		{
			XDrawArc(disp, win, gc, (int) (kreis_marker->x) - (kreis_marker->radius),
			   (int) (kreis_marker->y) - (kreis_marker->radius),
				 (unsigned int) ((kreis_marker->radius) + (kreis_marker->radius)),
				 (unsigned int) ((kreis_marker->radius) + (kreis_marker->radius)),
				 0, 360 * 64);
			kreis_marker = kreis_marker->next;
		}
		/* last circle */
		XDrawArc(disp, win, gc, (int) (kreis_curr->x) - (kreis_curr->radius),
			 (int) (kreis_curr->y) - (kreis_curr->radius),
		(unsigned int) ((kreis_curr->radius) + (kreis_curr->radius)),
		(unsigned int) ((kreis_curr->radius) + (kreis_curr->radius)),
			 0, 360 * 64);
	}	/* normal circle */
	/* filled */
	if (disc_start != NULL)
	{	/* min. 1 entry */
		disc_marker = disc_start;
		while (disc_marker != disc_curr)
		{
			XFillArc(disp, win, gc, (int) (disc_marker->x) - (disc_marker->radius),
			     (int) (disc_marker->y) - (disc_marker->radius),
				 (unsigned int) ((disc_marker->radius) + (disc_marker->radius)),
				 (unsigned int) ((disc_marker->radius) + (disc_marker->radius)),
				 0, 360 * 64);
			disc_marker = disc_marker->next;
		}
		/* last circle */
		XFillArc(disp, win, gc, (int) (disc_curr->x) - (disc_curr->radius),
			 (int) (disc_curr->y) - (disc_curr->radius),
		 (unsigned int) ((disc_curr->radius) + (disc_curr->radius)),
		 (unsigned int) ((disc_curr->radius) + (disc_curr->radius)),
			 0, 360 * 64);
	}	/* filled circle */
	/* oval */
	if (oval_start != NULL)
	{	/* min. 1 entry */
		oval_marker = oval_start;
		while (oval_marker != oval_curr)
		{
			DrawOval((int) oval_marker->x, (int) oval_marker->y,
				 (int) oval_marker->h, (int) oval_marker->v);
			oval_marker = oval_marker->next;
		}
		/* last circle */
		DrawOval((int) oval_curr->x, (int) oval_curr->y,
			 (int) oval_curr->h, (int) oval_curr->v);

	}	/* oval circle */
}



void
show_zoomed_objects()
{
	/* in case of zoom, rebuild screen. */
	/* this is a refresh to the zoomed-area */

	Display        *disp;
	Drawable        win;
	float           a1, a2, e1, e2, rad;
	int             r;



	disp = XtDisplay(pboard);
	win = XtWindow(pboard);		

	XClearWindow(disp, win);

	/* needed for correct cross painting... */
	h_cross = -999;
	v_cross = -999;

	if (raster == True)
		raster_on();
	if (ruler == True)
		ruler_on();
	if (cross == True)
		draw_coords(pboard, 0, 0, 0);

	/* bezier */
	if (bezier_start != NULL)
	{	/* min. 1 entry */
		bezier_marker = bezier_start;
		while (bezier_marker != bezier_curr)
		{
			a1=bezier_marker->sx;
			a2=bezier_marker->sy;
			real2zoomed(&a1,&a2); /* sx,sy are expected to be in zoom-mode */
			DrawBezier(bezier_marker->ax,bezier_marker->ay,
			    	   bezier_marker->ex,bezier_marker->ey,
				   a1,a2);
			bezier_marker = bezier_marker->next;
		}
		/* last line */
		a1 = bezier_curr->sx;
		a2 = bezier_curr->sy;
		real2zoomed(&a1, &a2);
		DrawBezier(bezier_curr->ax,bezier_curr->ay,
			bezier_curr->ex,bezier_curr->ey,
			a1,a2);
	}	/* bezier curves */

	/* lines */
	if (strich_start != NULL)
	{	/* min. 1 entry */
		strich_marker = strich_start;
		while (strich_marker != strich_curr)
		{
			a1 = strich_marker->x;
			a2 = strich_marker->y;
			e1 = strich_marker->h;
			e2 = strich_marker->v;
			real2zoomed(&a1, &a2);
			real2zoomed(&e1, &e2);
			XDrawLine(disp, win, gc, (int) a1, (int) a2, (int) e1, (int) e2);
			strich_marker = strich_marker->next;
		}
		/* last line */
		a1 = strich_curr->x;
		a2 = strich_curr->y;
		e1 = strich_curr->h;
		e2 = strich_curr->v;
		real2zoomed(&a1, &a2);
		real2zoomed(&e1, &e2);
		XDrawLine(disp, win, gc, (int) a1, (int) a2, (int) e1, (int) e2);
	}	/* lines */
	/* vectors */
	if (pfeil_start != NULL)
	{	/* min. 1 entry */
		pfeil_marker = pfeil_start;
		while (pfeil_marker != pfeil_curr)
		{
			a1 = pfeil_marker->x;
			a2 = pfeil_marker->y;
			e1 = pfeil_marker->h;
			e2 = pfeil_marker->v;
			real2zoomed(&a1, &a2);
			real2zoomed(&e1, &e2);
			XDrawLine(disp, win, gc, (int) a1, (int) a2, (int) e1, (int) e2);
			draw_vector_marker(a1, a2, e1, e2);
			pfeil_marker = pfeil_marker->next;
		}
		/* last vector */
		a1 = pfeil_curr->x;
		a2 = pfeil_curr->y;
		e1 = pfeil_curr->h;
		e2 = pfeil_curr->v;
		real2zoomed(&a1, &a2);
		real2zoomed(&e1, &e2);
		XDrawLine(disp, win, gc, (int) a1, (int) a2, (int) e1, (int) e2);
		draw_vector_marker(a1, a2, e1, e2);
	}	/* vectors */
	/* text */
	if (message_start != NULL)
	{	/* min. 1 entry */
		message_marker = message_start;
		while (message_marker != message_curr)
		{
			a1 = message_marker->x;
			a2 = message_marker->y;
			real2zoomed(&a1, &a2);
			print_text_text_marker(message_marker->textpos,a1,a2,message_marker->text);
			message_marker = message_marker->next;
		}
		/* last */
		a1 = message_curr->x;
		a2 = message_curr->y;
		real2zoomed(&a1, &a2);
		print_text_text_marker(message_curr->textpos,a1,a2,message_curr->text);
	}	/* text */
	/* boxes */
	/* framed */
	if (framedBox_start != NULL)
	{	/* min. 1 entry */
		framedBox_marker = framedBox_start;
		while (framedBox_marker != framedBox_curr)
		{
			a1 = framedBox_marker->x;
			a2 = framedBox_marker->y;
			e1 = framedBox_marker->h;
			e2 = framedBox_marker->v;
			real2zoomed(&a1, &a2);
			real2zoomed(&e1, &e2);
			XDrawRectangle(disp, win, gc, (int) a1, (int) a2,
				       (unsigned int) ((int) (e1 - a1)),
				       (unsigned int) ((int) (e2 - a2)));

			print_box_text(framedBox_marker->textpos,
				    a1, a2, e1, e2, framedBox_marker->text);

			framedBox_marker = framedBox_marker->next;
		}
		/* last one */
		a1 = framedBox_curr->x;
		a2 = framedBox_curr->y;
		e1 = framedBox_curr->h;
		e2 = framedBox_curr->v;
		real2zoomed(&a1, &a2);
		real2zoomed(&e1, &e2);
		XDrawRectangle(disp, win, gc, (int) a1, (int) a2,
			       (unsigned int) ((int) (e1 - a1)),
			       (unsigned int) ((int) (e2 - a2)));

		print_box_text(framedBox_curr->textpos,
			       a1, a2, e1, e2, framedBox_curr->text);

	}	/* framed */
	/* dashed */
	if (dashedBox_start != NULL)
	{	/* min. 1 entry */
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
		dashedBox_marker = dashedBox_start;
		while (dashedBox_marker != dashedBox_curr)
		{
			a1 = dashedBox_marker->x;
			a2 = dashedBox_marker->y;
			e1 = dashedBox_marker->h;
			e2 = dashedBox_marker->v;
			real2zoomed(&a1, &a2);
			real2zoomed(&e1, &e2);
			XDrawRectangle(disp, win, gc, (int) a1, (int) a2,
				       (unsigned int) ((int) (e1 - a1)),
				       (unsigned int) ((int) (e2 - a2)));

			print_box_text(dashedBox_marker->textpos,
				    a1, a2, e1, e2, dashedBox_marker->text);

			dashedBox_marker = dashedBox_marker->next;
		}
		/* last one */
		a1 = dashedBox_curr->x;
		a2 = dashedBox_curr->y;
		e1 = dashedBox_curr->h;
		e2 = dashedBox_curr->v;
		real2zoomed(&a1, &a2);
		real2zoomed(&e1, &e2);
		XDrawRectangle(disp, win, gc, (int) a1, (int) a2,
			       (unsigned int) ((int) (e1 - a1)),
			       (unsigned int) ((int) (e2 - a2)));

		print_box_text(dashedBox_curr->textpos,
			       a1, a2, e1, e2, dashedBox_curr->text);

		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);

	}	/* dashed */
	/* filled */
	if (filledBox_start != NULL)
	{	/* min. 1 entry */
		filledBox_marker = filledBox_start;
		while (filledBox_marker != filledBox_curr)
		{
			a1 = filledBox_marker->x;
			a2 = filledBox_marker->y;
			e1 = filledBox_marker->h;
			e2 = filledBox_marker->v;
			real2zoomed(&a1, &a2);
			real2zoomed(&e1, &e2);

			XFillRectangle(disp, win, gc, (int) a1, (int) a2,
			(unsigned int) (e1 - a1), (unsigned int) (e2 - a2));

			filledBox_marker = filledBox_marker->next;
		}
		/* last one */
		a1 = filledBox_curr->x;
		a2 = filledBox_curr->y;
		e1 = filledBox_curr->h;
		e2 = filledBox_curr->v;
		real2zoomed(&a1, &a2);
		real2zoomed(&e1, &e2);
		XFillRectangle(disp, win, gc, (int) a1, (int) a2,
			(unsigned int) (e1 - a1), (unsigned int) (e2 - a2));
	}	/* filled */
	/* circle */
	/* normal */
	if (kreis_start != NULL)
	{	/* min. 1 entry */
		kreis_marker = kreis_start;
		while (kreis_marker != kreis_curr)
		{
			a1 = kreis_marker->x;
			a2 = kreis_marker->y;
			real2zoomed(&a1, &a2);
			r = (int) (10 * kreis_marker->radius);
			XDrawArc(disp, win, gc, (int) (a1) - r,
				 (int) (a2) - r,
				 (unsigned int) (r + r),
				 (unsigned int) (r + r),
				 0, 360 * 64);
			kreis_marker = kreis_marker->next;
		}
		/* last circle */
		a1 = kreis_curr->x;
		a2 = kreis_curr->y;
		real2zoomed(&a1, &a2);
		r = (int) (10 * kreis_curr->radius);
		XDrawArc(disp, win, gc, (int) (a1) - r,
			 (int) (a2) - r,
			 (unsigned int) (r + r),
			 (unsigned int) (r + r),
			 0, 360 * 64);
	}	/* normal circle */
	/* filled */
	if (disc_start != NULL)
	{	/* min. 1 entry */
		disc_marker = disc_start;
		while (disc_marker != disc_curr)
		{
			a1 = disc_marker->x;
			a2 = disc_marker->y;
			real2zoomed(&a1, &a2);
			r = (int) (10 * disc_marker->radius);
			XFillArc(disp, win, gc, (int) (a1) - r,
				 (int) (a2) - r,
				 (unsigned int) (r + r),
				 (unsigned int) (r + r),
				 0, 360 * 64);
			disc_marker = disc_marker->next;
		}
		/* last circle */
		a1 = disc_curr->x;
		a2 = disc_curr->y;
		real2zoomed(&a1, &a2);
		r = (int) (10 * disc_curr->radius);
		XFillArc(disp, win, gc, (int) (a1) - r,
			 (int) (a2) - r,
			 (unsigned int) (r + r),
			 (unsigned int) (r + r),
			 0, 360 * 64);
	}	/* filled circle */
	/* oval */
	if (oval_start != NULL)
	{	/* min. 1 entry */
		oval_marker = oval_start;
		while (oval_marker != oval_curr)
		{
			a1 = oval_marker->x;
			a2 = oval_marker->y;
			real2zoomed(&a1, &a2);
			e1 = oval_marker->h;
			e2 = oval_marker->v;
			real2zoomed(&e1, &e2);
			DrawOval((int) a1, (int) a2, (int) e1, (int) e2);
			oval_marker = oval_marker->next;
		}
		/* last circle */
		a1 = oval_curr->x;
		a2 = oval_curr->y;
		real2zoomed(&a1, &a2);
		e1 = oval_curr->h;
		e2 = oval_curr->v;
		real2zoomed(&e1, &e2);
		DrawOval((int) a1, (int) a2, (int) e1, (int) e2);
	}	/* oval circle */
}/* proc */


void
DrawOval(int x, int y, int h, int v)
{
	/* x,y refer to upper left corner of the rectangle */

	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             zoom;
	int             d1, d2;
	float           rad;
	d1 = h - x;
	d2 = v - y;

	if (d1 < d2)
		rad = (float) d1 / (float) 2;
	else
		rad = (float) d2 / (float) 2;

	if (zoomed == False)
	{
		zoom = 1;
		if (rad > 20.0)
			rad = 20.0;
	} else
	{
		zoom = 10;
		if (rad > 200.0)
			rad = 200.0;
	}



	XDrawLine(disp, win, gc, x + (int) (rad), y, h - (int) (rad), y);
	XDrawLine(disp, win, gc, h, y + (int) (rad), h, v - (int) (rad));
	XDrawLine(disp, win, gc, h - (int) (rad), v, x + (int) (rad), v);
	XDrawLine(disp, win, gc, x, v - (int) (rad), x, y + (int) (rad));

	XDrawArc(disp, win, gc, x, y, (int) (2 * rad), (int) (2 * rad), 90 * 64, 90 * 64);
	XDrawArc(disp, win, gc, h - (int) (2 * rad), y, (int) (2 * rad), (int) (2 * rad), 0, 90 * 64);
	XDrawArc(disp, win, gc, h - (int) (2 * rad), v - (int) (2 * rad), (int) (2 * rad), (int) (2 * rad), 0, -90 * 64);
	XDrawArc(disp, win, gc, x, v - (int) (2 * rad), (int) (2 * rad), (int) (2 * rad), 180 * 64, 90 * 64);


}


void
print_text_text_marker(char *pos, float x, float y, char *txt)
{
 /* print the text and the marker of the text-input */

	int xpos,ypos;
	int fnt_height=10;
	int fnt_width=8;
	Display *disp=XtDisplay(pboard);
	Window win=XtWindow(pboard);
	int lenn=strlen(txt)*fnt_width;




	switch(pos[0])
	{
	case 't':	if (pos[1]=='l') 
			{
				xpos=x;
				ypos=y+fnt_height;
			}
			else if (pos[1]=='r') 
			{
				xpos=x-lenn;
				ypos=y+fnt_height;
			}
			else
			{
				xpos=x-lenn/2;
				ypos=y+fnt_height;
			}
			draw_vector_marker(x,y-fnt_height,x,y);
			break;

	case 'b':	if (pos[1]=='l') 
			{
				xpos=x;
				ypos=y;
			}
			else if (pos[1]=='r') 
			{
				xpos=x-lenn;
				ypos=y;
			}
			else
			{
				xpos=x-lenn/2;
				ypos=y;
			}
			draw_vector_marker(x,y+fnt_height,x,y);	
			break;

	case 'l':	xpos=x;
			ypos=y+fnt_height/2;
			draw_vector_marker(x-fnt_width,y,x,y);
			break;

	case 'r':	xpos=x-lenn;
			ypos=y+fnt_height/2;
			draw_vector_marker(x+fnt_width,y,x,y);
			break;

	default:	xpos=x-lenn/2;
			ypos=y+fnt_height/2;
			draw_vector_marker(x,y+fnt_height,x,y);
			break;	

	} /* switch */

	XDrawString(disp,win,gc,xpos,ypos,txt,lenn/fnt_width);
					
}

void
print_box_text(char *pos, float x, float y, float h, float v, char *txt)
{
	/* displays the first letter of the box-text at */
	/* the corresponding position in the frame */

	Display        *disp;
	Drawable        win;
	int             hor, ver,lenn;
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	switch (pos[0])
	{
	case 't':
		ver = (int) (y + 0.1 * (v - y));
		hor = (int) (x + 0.5 * (h - x));
		break;

	case 'b':
		ver = (int) (v - 0.1 * (v - y));
		hor = (int) (x + 0.5 * (h - x));
		break;

	case 'r':
		hor = (int) (h - 0.1 * (h - x));
		ver = (int) (v - 0.5 * (v - y));
		break;

	case 'l':
		hor = (int) (x + 0.1 * (h - x));
		ver = (int) (v - 0.5 * (v - y));
		break;

	case '\0':
		ver = (int) (y + (v - y) / 2);	/* center */
		hor = (int) (x + (h - x) / 2);
		break;

	}	/* switch */


	switch (pos[1])
	{
	case 'r':
		hor = (int) (h - 0.1 * (h - x));
		break;
	case 'l':
		hor = (int) (x + 0.1 * (h - x));
		break;
	}	/* switch */


	if (zoomed==False) XDrawString(disp, win, gc, hor, ver, txt, 1);
		else
		{
			lenn=strlen(txt)*8; /* 8 Bit horizontal width */
			/* right adjustment ? */
			if ( (pos[0]=='r') || (pos[1]=='r') )
				XDrawString(disp, win, gc, hor-lenn, ver, txt, lenn/8);
			else 
				if ( (pos[0]=='l') || (pos[1]=='l') )
				 XDrawString(disp, win, gc, hor, ver, txt, lenn/8);
			else /* center text */
				XDrawString(disp, win, gc, hor-lenn/2, ver, txt, lenn/8);

			/* show selection point */
			draw_vector_marker((float) hor, (float) (ver+20),
					   (float) hor, (float) ver);
		}
			
			

}




void
init_prompt_text()
{
	/* called once. builds the interface for text-queries */

	Widget          container, txt, cancel, ok;
	XtTranslations  trans_table;
	char            temp_str[200] = "<KeyPress>Return: ok_text() \n\
			Ctrl<Key>J      : dummy() \n\
			Ctrl<Key>O      : dummy() \n\
			Ctrl<Key>M      : dummy()";

	static XtActionsRec actions[150] = {{"dummy", Dummy},
	{"ok_text", ok_text}};



	
	input_up = XtCreatePopupShell("Text Input", transientShellWidgetClass, canvas, NULL,0);


	n=0;
	XtSetArg(args[n],XtNorientation,XtorientHorizontal);
	container = XtCreateManagedWidget("hi", boxWidgetClass, input_up, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 520);
	n++;
	XtSetArg(args[n], XtNautoFill, False);
	n++;
	XtSetArg(args[n], XtNuseStringInPlace, True);
	n++;
	XtSetArg(args[n], XtNstring, textinput);
	n++;
	XtSetArg(args[n], XtNlength, XtNumber(textinput));
	n++;
	XtSetArg(args[n], XtNeditType, XawtextEdit);
	n++;
	XtSetArg(args[n], XtNresize, XawtextResizeWidth);
	n++;
	XtSetArg(args[n], XtNwrap, XawtextWrapWord);
	n++;

	

	txt = XtCreateManagedWidget("dialogue", asciiTextWidgetClass, container, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 30);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 255);
	n++;
	XtSetArg(args[n], XtNlabel, "Cancel");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;

	
	cancel = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(cancel, XtNcallback, cancel_text, NULL);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 30);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 255);
	n++;
	XtSetArg(args[n], XtNlabel, "Accept");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;

	

	ok = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(ok, XtNcallback, ok_text, NULL);

	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(temp_str);
	XtOverrideTranslations(txt, trans_table);

}

void
cancel_text()
{
	transform(textinput);
	XtPopdown(input_up);
}

void
ok_text()
{
	float           x, y;
	Display        *disp;
	Drawable        win;
	int		lenn=81;


	transform(textinput);

	if (strlen(textinput) == 0)
	{	/* the input was zero-string */
		cancel_text();
		return;
	}
	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	XtPopdown(input_up);
	/* insert text into list */

	x = (float) n;
	y = (float) m;
	if (zoomed == True)
		zoomed2real(&x, &y);
	/* allocate memory */
	if (message_start == NULL)
	{	/* first entry */
		message_start = (struct fig5 *) malloc(sizeof(message));
		message_curr = message_start;
	} else
	{	/* insert at end */
		message_curr->next = (struct fig5 *) malloc(sizeof(message));
		message_curr = message_curr->next;
	}
	/* message_curr points to struct */
	/* save coordinates */
	message_curr->x = x;
	message_curr->y = y;
	/* save text-string */
	/* preserve a total string length of 80 Bytes */
	/* this is useful to edit the text */
	if (lenn<1+strlen(textinput)) lenn=1+strlen(textinput);
	message_curr->text = (char *) malloc(lenn);
	strcpy(message_curr->text, textinput);
	message_curr->next = NULL;

	/* query the position */

	/* initial values */
	txtpos[0] = '\0';
	txtpos[1] = '\0';
	query_text_adjustment();

	if (txtpos[0] == '\0')	/* center */
	{
		txtpos[0] = 'c';
		txtpos[1] = 'c';
	}
	message_curr->textpos[0] = txtpos[0];
	message_curr->textpos[1] = txtpos[1];

	/* refresh show the new object ! */
}



void
manage_text_input()
{
	Position        x, y, x_rel, y_rel;
	int             h, v;
	unsigned int    mask;


	PointerPosition(&h, &v, &mask);
	/* button1 was pressed in pboard at x,y */

	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
	n++;
	XtSetArg(args[n], XtNy, y + 4 * cmd_btn_height);
	n++;
	XtSetValues(input_up, args, n);
	/* position set ! */

	XtPopupSpringLoaded(input_up);
	n = h;
	m = v;


}



void
init_text_and_position()
{
	/* called once. used to buid the interface for text-queries and */
	/* text-positions. used with dashed and framed boxes */

	Widget          container, txt1, cancel, center, tl, tr, bl, br, b,
	                t, quit, lbl, r, l;
	XtTranslations  trans_table;
	char            temp_str[200] = "<KeyPress>Return: dummy() \n\
			Ctrl<Key>J      : dummy() \n\
			Ctrl<Key>O      : dummy() \n\
			Ctrl<Key>M      : dummy()";


	static XtActionsRec actions[150] = {{"dummy", Dummy}};



	

	box_me_up = XtCreatePopupShell("Text Input", transientShellWidgetClass, canvas, NULL,0);


	container = XtCreateManagedWidget("hi", boxWidgetClass, box_me_up, NULL, 0);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 40);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Text Placement");
	n++;
	
	lbl = XtCreateManagedWidget("hi", labelWidgetClass, container, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNautoFill, False);
	n++;
	XtSetArg(args[n], XtNuseStringInPlace, True);
	n++;
	XtSetArg(args[n], XtNstring, boxinput);
	n++;
	XtSetArg(args[n], XtNlength, XtNumber(boxinput));
	n++;
	XtSetArg(args[n], XtNeditType, XawtextEdit);
	n++;
	XtSetArg(args[n], XtNresize, XawtextResizeWidth);
	n++;
	XtSetArg(args[n], XtNwrap, XawtextWrapWord);
	n++;
	
	txt1 = XtCreateManagedWidget("dialogue", asciiTextWidgetClass, container, args, n);

	n = 0;
	XtSetArg(args[n], XtNlabel, "Position: CENTER");
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyLeft);
	n++;
	
	txt2 = XtCreateManagedWidget("< position of text >", labelWidgetClass, container, args, n);


	n = 0;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Style: NO SHADOW");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyLeft);
	n++;
	
	txt3 = XtCreateManagedWidget("shadow", labelWidgetClass, container, args, n);

	n = 0;
	XtSetArg(args[n], XtNbackground, XtDefaultForeground);
	n++;
	XtSetArg(args[n], XtNforeground, XtDefaultBackground);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 7);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	
	XtCreateManagedWidget("", labelWidgetClass, container, args, n);



	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 25);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Center Top and Bottom");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	center = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(center, XtNcallback, box_text, &km);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Top Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	tl = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(tl, XtNcallback, box_text, &ka);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 55);
	n++;
	XtSetArg(args[n], XtNlabel, "Top");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	t = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(t, XtNcallback, box_text, &ke);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Top Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	tr = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(tr, XtNcallback, box_text, &kb);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	l = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(l, XtNcallback, box_text, &kl);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	r = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(r, XtNcallback, box_text, &kr);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	bl = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(bl, XtNcallback, box_text, &kc);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 55);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	b = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(b, XtNcallback, box_text, &kf);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	br = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(br, XtNcallback, box_text, &kd);

	n = 0;
	XtSetArg(args[n], XtNbackground, XtDefaultForeground);
	n++;
	XtSetArg(args[n], XtNforeground, XtDefaultBackground);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 7);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtCreateManagedWidget("", labelWidgetClass, container, args, n);


	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 120);
	n++;
	XtSetArg(args[n], XtNlabel, "Filled shadow");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	
	XtAddCallback(XtCreateManagedWidget("hi", commandWidgetClass, container, args, n)
		      ,XtNcallback, box_text, &ff);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 120);
	n++;
	XtSetArg(args[n], XtNlabel, "Framed shadow");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	
	XtAddCallback(XtCreateManagedWidget("hi", commandWidgetClass, container, args, n)
		      ,XtNcallback, box_text, &nn);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "No shadow");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	
	XtAddCallback(XtCreateManagedWidget("hi", commandWidgetClass, container, args, n)
		      ,XtNcallback, box_text, &minus);


	n = 0;
	XtSetArg(args[n], XtNbackground, XtDefaultForeground);
	n++;
	XtSetArg(args[n], XtNforeground, XtDefaultBackground);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 7);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtCreateManagedWidget("", labelWidgetClass, container, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 45);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Accept");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	quit = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(quit, XtNcallback, box_text, &kp);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 45);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Cancel");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	cancel = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(cancel, XtNcallback, box_text, &kq);

	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(temp_str);
	XtOverrideTranslations(txt1, trans_table);
	txtpos[0] = '\0';	/* default: center */


}


void
box_text(Widget cmd, XtPointer closure, XtPointer call_data)
{
	/* store the text-position */

	char id = *(char *)closure;
	static char     s1[100], s2[100];
	strcpy(s1, "Position : ");
	strcpy(s2, "Style: ");


	switch (id)
	{
	case 'q':
		txtpos[0] = '?';
		XtPopdown(box_me_up);	/* cancel */
		visible = False;
		break;	/* cancel */
	case 'm':
		strcat(s1, "CENTER text");
		txtpos[0] = '\0';
		break;
	case 'a':
		strcat(s1, "TOP LEFT Placement");
		strcpy(txtpos, "tl");
		break;
	case 'b':
		strcat(s1, "TOP RIGHT Placement");
		strcpy(txtpos, "tr");
		break;
	case 'c':
		strcat(s1, "BOTTOM LEFT Placement");
		strcpy(txtpos, "bl");
		break;
	case 'd':
		strcat(s1, "BOTTOM RIGHT Placement");
		strcpy(txtpos, "br");
		break;
	case 'e':
		strcat(s1, "TOP Placement");
		strcpy(txtpos, "t");
		break;
	case 'f':
		strcat(s1, "BOTTOM Placement");
		strcpy(txtpos, "b");
		break;
	case 'l':
		strcat(s1, "LEFT Placement");
		strcpy(txtpos, "l");
		break;
	case 'r':
		strcat(s1, "RIGHT Placement");
		strcpy(txtpos, "r");
		break;
	case 'p':
		XtPopdown(box_me_up);
		visible = False;
		transform(boxinput);	/* remove special chars */
		break;
	default:
		shadow = id;
		if (shadow == '-')
			strcat(s2, "NO SHADOW");
		else if (shadow == 'N')
			strcat(s2, "FRAMED SHADOW");
		else
			strcat(s2, "FILLED SHADOW");
		break;
	}	/* end of switch */

	if ((id != 'q') && (id != 'p'))
	{
		if ((id != '-') && (id != 'N') && (id != 'F'))
		{
			XtSetArg(args[0], XtNlabel, s1);
			XtSetValues(txt2, args, 1);
		} else
		{
			XtSetArg(args[0], XtNlabel, s2);
			XtSetValues(txt3, args, 1);
		}
	}
}

void
query_box_text_and_position()
{
	/* called to enter the text and textposition */

	Position        x, y, x_rel, y_rel;
	int             h, v;
	XtAppContext    app_context;	/* Applikationscontext */
	XEvent          event;	/* Event */
	unsigned int    mask;


	PointerPosition(&h, &v, &mask);
	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
	n++;
	XtSetArg(args[n], XtNy, y + 4 * cmd_btn_height);
	n++;
	XtSetValues(box_me_up, args, n);
	/* position set ! */


	n = 0;
	XtSetArg(args[n], XtNlabel, "Position : CENTER text");
	n++;
	XtSetValues(txt2, args, n);

	txtpos[0] = '\0';

	XtPopupSpringLoaded(box_me_up);


	app_context = XtWidgetToApplicationContext(box_me_up);

	visible = True;

	while (visible)
	{
		XtAppNextEvent(app_context, &event);
		XtDispatchEvent(&event);

	}

	transform(boxinput);


}



void
perform_erase()
{
	/* erases all */

	xur = 0.0;
	yur = 0.0;


	/* normal circle */
	if (kreis_start != NULL)
	{
		do
		{
			kreis_marker = kreis_start;
			kreis_start = kreis_marker->next;
			free(kreis_marker);
		} while (kreis_marker != kreis_curr);

		kreis_start = NULL;
		kreis_curr = NULL;
	}
	/* filled circle */
	if (disc_start != NULL)
	{
		do
		{
			disc_marker = disc_start;
			disc_start = disc_marker->next;
			free(disc_marker);
		} while (disc_marker != disc_curr);

		disc_start = NULL;
		disc_curr = NULL;
	}
	/* oval */
	if (oval_start != NULL)
	{
		do
		{
			oval_marker = oval_start;
			oval_start = oval_marker->next;
			free(oval_marker);
		} while (oval_marker != oval_curr);

		oval_start = NULL;
		oval_curr = NULL;
	}
	/* framedBox */
	if (framedBox_start != NULL)
	{
		do
		{
			framedBox_marker = framedBox_start;
			framedBox_start = framedBox_marker->next;
			free(framedBox_marker);
		} while (framedBox_marker != framedBox_curr);

		framedBox_start = NULL;
		framedBox_curr = NULL;
	}
	/* dashedBox */
	if (dashedBox_start != NULL)
	{
		do
		{
			dashedBox_marker = dashedBox_start;
			dashedBox_start = dashedBox_marker->next;
			free(dashedBox_marker);
		} while (dashedBox_marker != dashedBox_curr);

		dashedBox_start = NULL;
		dashedBox_curr = NULL;
	}
	/* filledBox */
	if (filledBox_start != NULL)
	{
		do
		{
			filledBox_marker = filledBox_start;
			filledBox_start = filledBox_marker->next;
			free(filledBox_marker);
		} while (filledBox_marker != filledBox_curr);

		filledBox_start = NULL;
		filledBox_curr = NULL;
	}
	/* line */
	if (strich_start != NULL)
	{
		do
		{
			strich_marker = strich_start;
			strich_start = strich_marker->next;
			free(strich_marker);
		} while (strich_marker != strich_curr);

		strich_start = NULL;
		strich_curr = NULL;
	}
	/* vector */
	if (pfeil_start != NULL)
	{
		do
		{
			pfeil_marker = pfeil_start;
			pfeil_start = pfeil_marker->next;
			free(pfeil_marker);
		} while (pfeil_marker != pfeil_curr);

		pfeil_start = NULL;
		pfeil_curr = NULL;
	}
	/* text */
	if (message_start != NULL)
	{
		do
		{
			message_marker = message_start;
			message_start = message_marker->next;
			free(message_marker);
		} while (message_marker != message_curr);

		message_start = NULL;
		message_curr = NULL;
	}
	/* bezier */
	if (bezier_start != NULL)
	{
		do
		{
			bezier_marker = bezier_start;
			bezier_start = bezier_marker->next;
			free(bezier_marker);
		} while (bezier_marker != bezier_curr);

		bezier_start = NULL;
		bezier_curr = NULL;
	}
}



void
init_ask_erase()
{
	/* initializes the 'are you sure ?' - widget */

	Widget          container, c1, c2, lbl;
	
	
	
	
	ask_up = XtCreatePopupShell("P A X", transientShellWidgetClass, canvas, NULL,0);


	container = XtCreateManagedWidget("hi", boxWidgetClass, ask_up, NULL, 0);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 40);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 160);
	n++;
	XtSetArg(args[n], XtNlabel, "ARE YOU SURE ?");
	n++;
	
	lbl = XtCreateManagedWidget("hi", labelWidgetClass, container, args, n);


	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 25);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 75);
	n++;
	XtSetArg(args[n], XtNlabel, "Accept");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	c1 = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(c1, XtNcallback, answer, &kj);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 25);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 75);
	n++;
	XtSetArg(args[n], XtNlabel, "Cancel");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	c2 = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(c2, XtNcallback, answer, &kn);
}


void
query_ask_erase()
{
	int             h, v;
	unsigned int    mask;
	Position        x_rel, y_rel, x, y, xx, yy;
	XtAppContext    app_context;	/* Applikationcontext */
	XEvent          event;	/* Event */



	txtpos[0] = '\0';

	/* is there anything in the database ? */

	if (
	    (message_start == NULL) && (strich_start == NULL) && 
	    (pfeil_start == NULL) && (framedBox_start == NULL) && 
	    (dashedBox_start == NULL) && (filledBox_start == NULL)
	    && (kreis_start == NULL) && (disc_start == NULL) && 
	    (oval_start == NULL) && (bezier_start == NULL)
	   )
	{
		/* database empty */
		n = 0;
		XtSetArg(args[n], XtNx, &x_rel);
		n++;
		XtSetArg(args[n], XtNy, &y_rel);
		n++;
		XtGetValues(pboard, args, n);
		XtTranslateCoords(canvas, x_rel, y_rel, &xx, &yy);
		n = 0;
		XtSetArg(args[n], XtNx, 175 + xx);
		n++;
		XtSetArg(args[n], XtNy, 295 + yy);
		n++;
		XtSetValues(sign_up, args, n);
		XtPopup(sign_up, XtGrabExclusive);
		return;
	}
	PointerPosition(&h, &v, &mask);
	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
	n++;
	XtSetArg(args[n], XtNy, v);
	n++;
	XtSetValues(ask_up, args, n);
	/* position set ! */


	XtPopup(ask_up, XtGrabExclusive);


	app_context = XtWidgetToApplicationContext(ask_up);

	visible = True;

	while (visible)
	{
		XtAppNextEvent(app_context, &event);
		XtDispatchEvent(&event);

	}




}




void
draw_the_shadow(void *object, char id)
{
	/* shadows are drawn by the <refresh>-function */
	struct fig2    *shade;
	struct fig3    *frame;
	struct fig4    *dash;
	float           x, y, h, v;
	float           ver, hor;
	int		ext; /* width/height of shadow */
	if (shadow == '-')
		return;	/* no shadow */

	if (id == 'N')
	{
		frame = (struct fig3 *) object;
		x = frame->x;
		y = frame->y;
		h = frame->h;
		v = frame->v;
	} else
	{
		dash = (struct fig4 *) object;
		x = dash->x;
		y = dash->y;
		h = dash->h;
		v = dash->v;
	}

	norm_rectangle(&x, &y, &h, &v);
	
	hor=0.1*(h-x); /* h is always bigger than x; see norm_rectangle ! */
	ver=0.1*(v-y); 
	
	ext=(hor<ver) ? hor : ver;
	ext=(ext<10) ? ext : 10;
	
	hor=ext;
	ver=ext;
	
	if (shadow == 'N')
	{
		/* get new memory for lines */
		shade = (struct fig2 *) malloc(sizeof(strich));
		shade->x = x + hor;
		shade->y = v;
		shade->h = x + hor;
		shade->v = v + ver;
		if (strich_curr != NULL)
			strich_curr->next = shade;
		shade->next = NULL;
		strich_curr = shade;
		if (strich_start == NULL)
			strich_start = shade;
		/* that was the first one */
		shade = (struct fig2 *) malloc(sizeof(strich));
		shade->x = x + hor;
		shade->y = v + ver;
		shade->h = h + hor;
		shade->v = v + ver;
		if (strich_curr != NULL)
			strich_curr->next = shade;
		shade->next = NULL;
		strich_curr = shade;
		/* now comes the 3rd one */
		shade = (struct fig2 *) malloc(sizeof(strich));
		shade->x = h + hor;
		shade->y = v + ver;
		shade->h = h + hor;
		shade->v = y + ver;
		if (strich_curr != NULL)
			strich_curr->next = shade;
		shade->next = NULL;
		strich_curr = shade;
		/* and finally... */
		shade = (struct fig2 *) malloc(sizeof(strich));
		shade->x = h + hor;
		shade->y = y + ver;
		shade->h = h;
		shade->v = y + ver;
		if (strich_curr != NULL)
			strich_curr->next = shade;
		shade->next = NULL;
		strich_curr = shade;
	} else
	{
		/* first fbox */
		shade = (struct fig2 *) malloc(sizeof(filledBox));
		shade->x = x + hor;
		shade->y = v;
		shade->h = h + hor;
		shade->v = v + hor;
		if (filledBox_curr != NULL)
			filledBox_curr->next = shade;
		shade->next = NULL;
		filledBox_curr = shade;
		if (filledBox_start == NULL)
			filledBox_start = shade;
		/* 2nd fbox */
		shade = (struct fig2 *) malloc(sizeof(filledBox));
		shade->x = h;
		shade->y = y + hor;
		shade->h = h + hor;
		shade->v = v;
		if (filledBox_curr != NULL)
			filledBox_curr->next = shade;
		shade->next = NULL;
		filledBox_curr = shade;
	}

}





void
init_text_adjustment()
{

	Widget          container, center, tl, t, tr, l, r, bl, b, br,
	                quit;

	
	pop_adjust_up = XtCreatePopupShell("Text Adjustment", transientShellWidgetClass, canvas, NULL,0);


	container = XtCreateManagedWidget("hi", boxWidgetClass, pop_adjust_up, NULL, 0);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 40);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Text Adjustment");
	n++;
	
	XtCreateManagedWidget("hi", labelWidgetClass, container, args, n);


	n = 0;
	XtSetArg(args[n], XtNlabel, "Position: CENTER");
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyLeft);
	n++;
	
	adj_txt2 = XtCreateManagedWidget("< position of text >", labelWidgetClass, container, args, n);



	n = 0;
	XtSetArg(args[n], XtNbackground, XtDefaultForeground);
	n++;
	XtSetArg(args[n], XtNforeground, XtDefaultBackground);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 7);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtCreateManagedWidget("", labelWidgetClass, container, args, n);



	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 25);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Center text");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	center = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(center, XtNcallback, adjust_text, &kz);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Top Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	tl = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(tl, XtNcallback, adjust_text, &ka);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 55);
	n++;
	XtSetArg(args[n], XtNlabel, "Top");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	t = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(t, XtNcallback, adjust_text, &ke);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Top Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	tr = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(tr, XtNcallback, adjust_text, &kb);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	l = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(l, XtNcallback, adjust_text, &kl);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 125);
	n++;
	XtSetArg(args[n], XtNlabel, "Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	r = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(r, XtNcallback, adjust_text, &kr);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom Left");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	bl = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(bl, XtNcallback, adjust_text, &kc);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 55);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	b = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(b, XtNcallback, adjust_text, &kf);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 95);
	n++;
	XtSetArg(args[n], XtNlabel, "Bottom Right");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	br = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(br, XtNcallback, adjust_text, &kd);

	n = 0;
	XtSetArg(args[n], XtNbackground, XtDefaultForeground);
	n++;
	XtSetArg(args[n], XtNforeground, XtDefaultBackground);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 7);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtCreateManagedWidget("", labelWidgetClass, container, args, n);


	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 45);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 260);
	n++;
	XtSetArg(args[n], XtNlabel, "Accept");
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	
	quit = XtCreateManagedWidget("hi", commandWidgetClass, container, args, n);
	XtAddCallback(quit, XtNcallback, adjust_text, &kp);


	txtpos[0] = '\0';	/* default: center */


}





void
adjust_text(Widget cmd, XtPointer closure, XtPointer call_data)
{
	/* store the text-position */

	char id = *(char *)closure;
	static char     s1[100];
	strcpy(s1, "Position : ");



	switch (id)
	{

	case 'z':
		strcat(s1, "CENTER");
		txtpos[0] = '\0';
		break;
	case 'a':
		strcat(s1, "TOP LEFT");
		strcpy(txtpos, "tl");
		break;
	case 'b':
		strcat(s1, "TOP RIGHT");
		strcpy(txtpos, "tr");
		break;
	case 'c':
		strcat(s1, "BOTTOM LEFT");
		strcpy(txtpos, "bl");
		break;
	case 'd':
		strcat(s1, "BOTTOM RIGHT");
		strcpy(txtpos, "br");
		break;
	case 'e':
		strcat(s1, "TOP");
		strcpy(txtpos, "t");
		break;
	case 'f':
		strcat(s1, "BOTTOM");
		strcpy(txtpos, "b");
		break;
	case 'l':
		strcat(s1, "LEFT");
		strcpy(txtpos, "l");
		break;
	case 'r':
		strcat(s1, "RIGHT");
		strcpy(txtpos, "r");
		break;
	case 'p':
		XtPopdown(pop_adjust_up);
		visible = False;
		transform(textinput);	/* remove special chars */
		break;
	default:
		break;
	}	/* end of switch */

	if ((id != 'q') && (id != 'p'))
	{
		XtSetArg(args[0], XtNlabel, s1);
		XtSetValues(adj_txt2, args, 1);
	}
}





void
query_text_adjustment()
{

	Position        x, y, x_rel, y_rel;
	int             h, v;
	XtAppContext    app_context;	/* Applikationscontext */
	XEvent          event;	/* Event */
	unsigned int    mask;


	PointerPosition(&h, &v, &mask);
	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
	n++;
	XtSetArg(args[n], XtNy, y + 4 * cmd_btn_height);
	n++;
	XtSetValues(pop_adjust_up, args, n);
	/* position set ! */


	n = 0;
	XtSetArg(args[n], XtNlabel, "Position : CENTER");
	n++;
	XtSetValues(adj_txt2, args, n);

	txtpos[0] = '\0';

	XtPopupSpringLoaded(pop_adjust_up);


	app_context = XtWidgetToApplicationContext(pop_adjust_up);

	visible = True;

	while (visible)
	{
		XtAppNextEvent(app_context, &event);
		XtDispatchEvent(&event);

	}

	transform(textinput);


}

int number_of_text()
{
  int i;
	/* compute number of text-entries */
	i=1;
	if (message_start==NULL) return 0;
	if (message_start==message_curr) return 1;
	message_marker=message_start;
	do
	{
		i++;
		if (message_marker!=message_curr) message_marker=message_marker->next;
	} while (message_marker!=message_curr);

	return i;
}



void action_textedit(Widget widget, XEvent *event, String *params,
		     Cardinal *num_params)
{
     textedit(widget, 0, 0);
}

void textedit(Widget widget, XtPointer closure, XtPointer call_data)
{
  Widget perle;
  Widget kiste,toggle,blabla;
  Widget ok,del,more;
  int i,lenn=81;
  int anz_text;
  XtTranslations  trans_table,trans_table2;
  char		  leave_str[100]="<LeaveNotify> : beende_textedit() correct_text_error()\n";
  char		  ok_str[100]="<ButtonRelease> : beende_textedit() correct_text_error()\n";
  char		  more_str[100]="<ButtonRelease>    : beende_textedit() textedit()\n";
  char		  del_str[100]="<ButtonRelease>    : delete_textedit() beende_textedit() textedit()\n";

  char            temp_str2[100] = 
			"<KeyPress>Return	: dummy() \n";
  
  static XtActionsRec actions[200] = {{"dummy", Dummy},
				      {"textedit", action_textedit},
				      {"beende_textedit", beende_textedit},
				      {"delete_textedit", delete_textedit},
				      {"correct_text_error",correct_text_error}};
  Position x_rel, y_rel, x, y;
	

	muschel=XtCreatePopupShell("Edit/Delete", transientShellWidgetClass, canvas, NULL, 0);

	trans_table = XtParseTranslationTable(leave_str);
	XtOverrideTranslations(muschel, trans_table);

	perle=XtCreateManagedWidget("behaelter", boxWidgetClass, muschel, NULL, 0);
	
	n=0;
	XtSetArg(args[n], XtNlabel,"Edit Normal Text");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,560);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	
	XtCreateManagedWidget("", labelWidgetClass, perle, args, n);


	n=0;
	XtSetArg(args[n], XtNlabel,"Accept");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	
	ok=XtCreateManagedWidget("ok", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(ok_str);
	XtOverrideTranslations(ok, trans_table);


	/* compute number of text-entries */
	anz_text=number_of_text();

	if (anz_text==0)
	{
		leave_pick("Sorry - No text available !\n");
		return;
	}	


	if (anz_text>10)
	{
		n=0;
		XtSetArg(args[n], XtNlabel,"More");n++;
		XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
		XtSetArg(args[n], XtNwidth,130);n++;
		XtSetArg(args[n], XtNheight,40);n++;
		XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);n++;
		
		more=XtCreateManagedWidget("more", commandWidgetClass, perle, args, n);
		trans_table = XtParseTranslationTable(more_str);
		XtOverrideTranslations(more, trans_table);
	}


	n=0;
	XtSetArg(args[n], XtNlabel,"Delete");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	del=XtCreateManagedWidget("del", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(del_str);
	XtOverrideTranslations(del, trans_table);
	
	
	if (ab>anz_text) ab=-9;
	ab+=10; /* ab was initially -9 */
	if (ab>anz_text) ab=1;

	/* message_marker must point to the "ab"th text! */

	message_marker=message_start;
	for (i=2; i<=ab; i++) message_marker=message_marker->next;

	for (i=ab; ( (i<=anz_text) && (i-ab<10) ); i++)
	{
		n=0;
		XtSetArg(args[n],XtNorientation,XtorientHorizontal);n++;
		XtSetArg(args[n],XtNborderWidth,0);n++;
		kiste=XtCreateManagedWidget("drin", boxWidgetClass, perle, args, n);
		n=0;
		XtSetArg(args[n],XtNstate,False);n++;
		XtSetArg(args[n],XtNheight,20);n++;
		XtSetArg(args[n],XtNwidth,20);n++;
		XtSetArg(args[n],XtNshapeStyle,XmuShapeOval);n++;
		toggle=XtCreateManagedWidget("!", toggleWidgetClass, kiste, args, n);
		n=0;
		XtSetArg(args[n],XtNuseStringInPlace,True);n++;
		XtSetArg(args[n],XtNstring,message_marker->text);n++;
		XtSetArg(args[n],XtNwidth,500);n++;
		if (lenn<1+strlen(message_marker->text)) lenn=1+strlen(message_marker->text);
		XtSetArg(args[n],XtNlength,lenn);n++; 
		/* warning, this is constant */
	/* it is a fact, that each of these pointers point to a memory-location, which */
	/* is at last exactly 80 Bytes big. */
	/* Well, this is an arbitrary restriction */
		XtSetArg(args[n],XtNeditType,XawtextEdit);n++;
		
		blabla=XtCreateManagedWidget("text", asciiTextWidgetClass, kiste, args, n);
		XtAddActions(actions, XtNumber(actions));
		trans_table2 = XtParseTranslationTable(temp_str2);
		XtOverrideTranslations(blabla, trans_table2);

		/* Store the pointers, used for a delete action */
		mem[i-ab].tg=toggle;
		mem[i-ab].obj=(struct fig5 *)message_marker;
		mem[i-ab+1].tg=NULL;
		mem[i-ab+1].obj=NULL;

		message_marker=message_marker->next;
		
	}

	XtAddActions(actions, XtNumber(actions));
	
	

	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width);
	n++;
	XtSetArg(args[n], XtNy, y + 2 * cmd_btn_height);
	n++;
	XtSetValues(muschel, args, n);
	/* position set ! */


	XtPopupSpringLoaded(muschel);

	
}

void correct_text_error(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)
{

	/* remove special chars from text */

	message_marker=message_start;
	if (message_marker==NULL) return;

	while (message_marker!=NULL)
	{
		transform(message_marker->text);
		message_marker=message_marker->next;
	}
	
	ab=-9; /* for a new start */
	
}
		
void beende_textedit(Widget widget, XEvent *event, String *params,
		     Cardinal *num_params)	
{
	XtPopdown(muschel);
	XtDestroyWidget(muschel); /* and its children ! */
}	



void delete_textedit(Widget widget, XEvent *event, String *params,
		     Cardinal *num_params)
{
	/* perform an erase on the selected (toggled) text */

 int i=0; /* counter */
 Boolean offon;
 struct fig5 * five;

	while ( (mem[i].tg!=NULL) && (i<10) )
	{
		/* was the toggle active ? */
		n=0;
		XtSetArg(args[n],XtNstate,&offon);n++;
		XtGetValues(mem[i].tg, args, n);

		if (offon==True) /* it was selected - delete text ! */
		{
			message_marker = (struct fig5 *)mem[i].obj;
			if (message_start == mem[i].obj)
			{	/* first entry */
				if (message_curr == message_start)
				{	/* exactly 1 entry */

					message_start = NULL;
					message_curr = NULL;

				} else
					message_start = message_start->next;

				free(message_marker);
			} else
			{
				message_marker = message_start;
				do
				{
					five = (struct fig5 *) message_marker;
					/* remember the last object */
					message_marker = message_marker->next;

				} while (message_marker != mem[i].obj);
				/* <message_marker> points to the desired object and */
				/* <five> to the object before */

				/* last entry ? */
				if (message_marker == message_curr)
				{
					message_marker = NULL;
					free(message_curr);
					message_curr = five;
					five->next=NULL;
				} else
					/* no! the object was somewhere in the middle */
				{
					five->next = message_marker->next;
					free(message_marker);
				}
			} /* else */
		

			
		} /* if offon */

		i++;

	} /* while */
} /* proc */


	
int sorry_msg()
{
 int i=-1;

	if (
		(pfeil_start==NULL) &&
		(strich_start==NULL) &&
		(message_start==NULL) &&
		(framedBox_start==NULL) &&
		(dashedBox_start==NULL) &&
		(filledBox_start==NULL) &&
		(kreis_start==NULL) &&
		(disc_start==NULL) &&
		(bezier_start==NULL) &&
		(oval_start==NULL)
	   )
		leave_pick("Sorry - No objects !\n");

	else i=0;

	return i;

}	



int fb_number_of_text()
{
  int i;
	/* compute number of text-entries */
	i=1;
	if (framedBox_start==NULL) return 0;
	if (framedBox_start==framedBox_curr) return 1;
	framedBox_marker=framedBox_start;
	do
	{
		i++;
		if (framedBox_marker!=framedBox_curr) framedBox_marker=framedBox_marker->next;
	} while (framedBox_marker!=framedBox_curr);

	return i;
}



void action_fb_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)
{
     fb_textedit(widget, 0, 0);
}

void fb_textedit(Widget widget, XtPointer closure, XtPointer call_data)
{
  Widget perle;
  Widget kiste,toggle,blabla;
  Widget ok,del,more;
  int i,lenn=81;
  int anz_text;
  XtTranslations  trans_table,trans_table2;
  char		  leave_str[100]="<LeaveNotify> : fb_beende_textedit() fb_correct_text_error()\n";
  char		  ok_str[100]="<ButtonRelease> : fb_beende_textedit() fb_correct_text_error()\n";
  char		  more_str[100]="<ButtonRelease>    : fb_beende_textedit() fb_textedit()\n";
  char		  del_str[100]="<ButtonRelease>    : fb_delete_textedit() fb_beende_textedit() fb_textedit()\n";

  char            temp_str2[100] = 
			"<KeyPress>Return	: dummy() \n";
  
  static XtActionsRec actions[200] = {{"dummy", Dummy},
				      {"fb_textedit", action_fb_textedit},
				      {"fb_beende_textedit", fb_beende_textedit},
				      {"fb_delete_textedit", fb_delete_textedit},
				      {"fb_correct_text_error",fb_correct_text_error}};
  Position x_rel, y_rel, x, y;
	

	fb_muschel=XtCreatePopupShell("Edit/Delete", transientShellWidgetClass, canvas, NULL, 0);

	trans_table = XtParseTranslationTable(leave_str);
	XtOverrideTranslations(fb_muschel, trans_table);

	perle=XtCreateManagedWidget("behaelter", boxWidgetClass, fb_muschel, NULL, 0);
	
	n=0;
	XtSetArg(args[n], XtNlabel,"Edit Text: FRAMED BOX");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,560);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	
	XtCreateManagedWidget("", labelWidgetClass, perle, args, n);


	n=0;
	XtSetArg(args[n], XtNlabel,"Accept");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	ok=XtCreateManagedWidget("ok", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(ok_str);
	XtOverrideTranslations(ok, trans_table);


	/* compute number of text-entries */
	anz_text=fb_number_of_text();

	if (anz_text==0)
	{
		leave_pick("Sorry - No text available !\n");
		return;
	}	


	if (anz_text>10)
	{
		n=0;
		XtSetArg(args[n], XtNlabel,"More");n++;
		XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
		XtSetArg(args[n], XtNwidth,130);n++;
		XtSetArg(args[n], XtNheight,40);n++;
		XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);n++;
		
	
		more=XtCreateManagedWidget("more", commandWidgetClass, perle, args, n);
		trans_table = XtParseTranslationTable(more_str);
		XtOverrideTranslations(more, trans_table);
	}


	n=0;
	XtSetArg(args[n], XtNlabel,"Delete");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	del=XtCreateManagedWidget("del", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(del_str);
	XtOverrideTranslations(del, trans_table);
	
	
	if (ab>anz_text) ab=-9;
	ab+=10; /* ab was initially -9 */
	if (ab>anz_text) ab=1;

	/* framedBox_marker must point to the "ab"th text! */

	framedBox_marker=framedBox_start;
	for (i=2; i<=ab; i++) framedBox_marker=framedBox_marker->next;

	for (i=ab; ( (i<=anz_text) && (i-ab<10) ); i++)
	{
		n=0;
		XtSetArg(args[n],XtNorientation,XtorientHorizontal);n++;
		XtSetArg(args[n],XtNborderWidth,0);n++;
		kiste=XtCreateManagedWidget("drin", boxWidgetClass, perle, args, n);
		n=0;
		XtSetArg(args[n],XtNstate,False);n++;
		XtSetArg(args[n],XtNheight,20);n++;
		XtSetArg(args[n],XtNwidth,20);n++;
		XtSetArg(args[n],XtNshapeStyle,XmuShapeOval);n++;
		toggle=XtCreateManagedWidget("!", toggleWidgetClass, kiste, args, n);
		n=0;
		XtSetArg(args[n],XtNuseStringInPlace,True);n++;
		XtSetArg(args[n],XtNstring,framedBox_marker->text);n++;
		XtSetArg(args[n],XtNwidth,500);n++;
		if (lenn<1+strlen(framedBox_marker->text)) lenn=1+strlen(framedBox_marker->text);
		XtSetArg(args[n],XtNlength,lenn);n++; 
		/* warning, this is constant */
	/* it is a fact, that each of these pointers point to a memory-location, which */
	/* is at last exactly 80 Bytes big. */
	/* Well, this is an arbitrary restriction */
		XtSetArg(args[n],XtNeditType,XawtextEdit);n++;
		
	
		blabla=XtCreateManagedWidget("text", asciiTextWidgetClass, kiste, args, n);
		XtAddActions(actions, XtNumber(actions));
		trans_table2 = XtParseTranslationTable(temp_str2);
		XtOverrideTranslations(blabla, trans_table2);

		/* Store the pointers, used for a delete action */
		mem[i-ab].tg=toggle;
		mem[i-ab].obj=(struct fig3 *)framedBox_marker;
		mem[i-ab+1].tg=NULL;
		mem[i-ab+1].obj=NULL;

		framedBox_marker=framedBox_marker->next;
		
	}

	XtAddActions(actions, XtNumber(actions));
	
	

	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width);
	n++;
	XtSetArg(args[n], XtNy, y + 2 * cmd_btn_height);
	n++;
	XtSetValues(fb_muschel, args, n);
	/* position set ! */


	XtPopupSpringLoaded(fb_muschel);

	
}

void fb_correct_text_error(Widget widget, XEvent *event, String *params,
			   Cardinal *num_params)
{

	/* remove special chars from text */

	framedBox_marker=framedBox_start;
	if (framedBox_marker==NULL) return;

	while (framedBox_marker!=NULL)
	{
		transform(framedBox_marker->text);
		framedBox_marker=framedBox_marker->next;
	}
	
	ab=-9; /* for a new start */
	
}
		
void fb_beende_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)	
{
	XtPopdown(fb_muschel);
	XtDestroyWidget(fb_muschel); /* and its children ! */
}	



void fb_delete_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)
{
	/* perform an erase on the selected (toggled) text */

 int i=0; /* counter */
 Boolean offon;
 struct fig3 * three;

	while ( (mem[i].tg!=NULL) && (i<10) )
	{
		/* was the toggle active ? */
		n=0;
		XtSetArg(args[n],XtNstate,&offon);n++;
		XtGetValues(mem[i].tg, args, n);

		if (offon==True) /* it was selected - delete text ! */
		{
			framedBox_marker = (struct fig3 *)mem[i].obj;
			if (framedBox_start == mem[i].obj)
			{	/* first entry */
				if (framedBox_curr == framedBox_start)
				{	/* exactly 1 entry */

					framedBox_start = NULL;
					framedBox_curr = NULL;

				} else
					framedBox_start = framedBox_start->next;

				free(framedBox_marker);
			} else
			{
				framedBox_marker = framedBox_start;
				do
				{
					three = (struct fig3 *) framedBox_marker;
					/* remember the last object */
					framedBox_marker = framedBox_marker->next;

				} while (framedBox_marker != mem[i].obj);
				/* <framedBox_marker> points to the desired object and */
				/* <three> to the object before */

				/* last entry ? */
				if (framedBox_marker == framedBox_curr)
				{
					framedBox_marker = NULL;
					free(framedBox_curr);
					framedBox_curr = three;
					three->next=NULL;
				} else
					/* no! the object was somewhere in the middle */
				{
					three->next = framedBox_marker->next;
					free(framedBox_marker);
				}
			} /* else */
		

			
		} /* if offon */

		i++;

	} /* while */
} /* proc */


/* ------- */


int db_number_of_text()
{
  int i;
	/* compute number of text-entries */
	i=1;
	if (dashedBox_start==NULL) return 0;
	if (dashedBox_start==dashedBox_curr) return 1;
	dashedBox_marker=dashedBox_start;
	do
	{
		i++;
		if (dashedBox_marker!=dashedBox_curr) dashedBox_marker=dashedBox_marker->next;
	} while (dashedBox_marker!=dashedBox_curr);

	return i;
}



void action_db_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)
{
     db_textedit(widget, 0, 0);
}

void db_textedit(Widget widget, XtPointer closure, XtPointer call_data)
{
  Widget perle;
  Widget kiste,toggle,blabla;
  Widget ok,del,more;
  int i,lenn=81;
  int anz_text;
  XtTranslations  trans_table,trans_table2;
  char		  leave_str[100]="<LeaveNotify> : db_beende_textedit() db_correct_text_error()\n";
  char		  ok_str[100]="<ButtonRelease> : db_beende_textedit() db_correct_text_error()\n";
  char		  more_str[100]="<ButtonRelease>    : db_beende_textedit() db_textedit()\n";
  char		  del_str[100]="<ButtonRelease>    : db_delete_textedit() db_beende_textedit() db_textedit()\n";

  char            temp_str2[100] = 
			"<KeyPress>Return	: dummy() \n";
  
  static XtActionsRec actions[200] = {{"dummy", Dummy},
				      {"db_textedit", action_db_textedit},
				      {"db_beende_textedit", db_beende_textedit},
				      {"db_delete_textedit", db_delete_textedit},
				      {"db_correct_text_error",db_correct_text_error}};
  Position x_rel, y_rel, x, y;
	

	db_muschel=XtCreatePopupShell("Edit/Delete", transientShellWidgetClass, canvas, NULL, 0);

	trans_table = XtParseTranslationTable(leave_str);
	XtOverrideTranslations(db_muschel, trans_table);

	perle=XtCreateManagedWidget("behaelter", boxWidgetClass, db_muschel, NULL, 0);
	
	n=0;
	XtSetArg(args[n], XtNlabel,"Edit Text: DASHED BOX");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,560);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	
	XtCreateManagedWidget("", labelWidgetClass, perle, args, n);


	n=0;
	XtSetArg(args[n], XtNlabel,"Accept");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	ok=XtCreateManagedWidget("ok", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(ok_str);
	XtOverrideTranslations(ok, trans_table);


	/* compute number of text-entries */
	anz_text=db_number_of_text();

	if (anz_text==0)
	{
		leave_pick("Sorry - No text available !\n");
		return;
	}	


	if (anz_text>10)
	{
		n=0;
		XtSetArg(args[n], XtNlabel,"More");n++;
		XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
		XtSetArg(args[n], XtNwidth,130);n++;
		XtSetArg(args[n], XtNheight,40);n++;
		XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);n++;
		
		more=XtCreateManagedWidget("more", commandWidgetClass, perle, args, n);
		trans_table = XtParseTranslationTable(more_str);
		XtOverrideTranslations(more, trans_table);
	}


	n=0;
	XtSetArg(args[n], XtNlabel,"Delete");n++;
	XtSetArg(args[n], XtNjustify,XtJustifyCenter);n++;
	XtSetArg(args[n], XtNwidth,130);n++;
	XtSetArg(args[n], XtNheight,40);n++;
	XtSetArg(args[n], XtNshapeStyle,XmuShapeOval);n++;
	
	del=XtCreateManagedWidget("del", commandWidgetClass, perle, args, n);
	trans_table = XtParseTranslationTable(del_str);
	XtOverrideTranslations(del, trans_table);
	
	
	if (ab>anz_text) ab=-9;
	ab+=10; /* ab was initially -9 */
	if (ab>anz_text) ab=1;

	/* dashedBox_marker must point to the "ab"th text! */

	dashedBox_marker=dashedBox_start;
	for (i=2; i<=ab; i++) dashedBox_marker=dashedBox_marker->next;

	for (i=ab; ( (i<=anz_text) && (i-ab<10) ); i++)
	{
		n=0;
		XtSetArg(args[n],XtNorientation,XtorientHorizontal);n++;
		XtSetArg(args[n],XtNborderWidth,0);n++;
		kiste=XtCreateManagedWidget("drin", boxWidgetClass, perle, args, n);
		n=0;
		XtSetArg(args[n],XtNstate,False);n++;
		XtSetArg(args[n],XtNheight,20);n++;
		XtSetArg(args[n],XtNwidth,20);n++;
		XtSetArg(args[n],XtNshapeStyle,XmuShapeOval);n++;
		toggle=XtCreateManagedWidget("!", toggleWidgetClass, kiste, args, n);
		n=0;
		XtSetArg(args[n],XtNuseStringInPlace,True);n++;
		XtSetArg(args[n],XtNstring,dashedBox_marker->text);n++;
		XtSetArg(args[n],XtNwidth,500);n++;
		if (lenn<1+strlen(dashedBox_marker->text)) lenn=1+strlen(dashedBox_marker->text);
		XtSetArg(args[n],XtNlength,lenn);n++; 
		/* warning, this is constant */
	/* it is a fact, that each of these pointers point to a memory-location, which */
	/* is at last exactly 80 Bytes big. */
	/* Well, this is an arbitrary restriction */
		XtSetArg(args[n],XtNeditType,XawtextEdit);n++;
		
	
		blabla=XtCreateManagedWidget("text", asciiTextWidgetClass, kiste, args, n);
		XtAddActions(actions, XtNumber(actions));
		trans_table2 = XtParseTranslationTable(temp_str2);
		XtOverrideTranslations(blabla, trans_table2);

		/* Store the pointers, used for a delete action */
		mem[i-ab].tg=toggle;
		mem[i-ab].obj=(struct fig4 *)dashedBox_marker;
		mem[i-ab+1].tg=NULL;
		mem[i-ab+1].obj=NULL;

		dashedBox_marker=dashedBox_marker->next;
		
	}

	XtAddActions(actions, XtNumber(actions));
	
	

	/* setting position of widget */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width);
	n++;
	XtSetArg(args[n], XtNy, y + 2 * cmd_btn_height);
	n++;
	XtSetValues(db_muschel, args, n);
	/* position set ! */


	XtPopupSpringLoaded(db_muschel);

	
}

void db_correct_text_error(Widget widget, XEvent *event, String *params,
			   Cardinal *num_params)
{

	/* remove special chars from text */

	dashedBox_marker=dashedBox_start;
	if (dashedBox_marker==NULL) return;

	while (dashedBox_marker!=NULL)
	{
		transform(dashedBox_marker->text);
		dashedBox_marker=dashedBox_marker->next;
	}
	
	ab=-9; /* for a new start */
	
}
		
void db_beende_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)	
{
	XtPopdown(db_muschel);
	XtDestroyWidget(db_muschel); /* and its children ! */
}	



void db_delete_textedit(Widget widget, XEvent *event, String *params,
			Cardinal *num_params)
{
	/* perform an erase on the selected (toggled) text */

 int i=0; /* counter */
 Boolean offon;
 struct fig4 * four;

	while ( (mem[i].tg!=NULL) && (i<10) )
	{
		/* was the toggle active ? */
		n=0;
		XtSetArg(args[n],XtNstate,&offon);n++;
		XtGetValues(mem[i].tg, args, n);

		if (offon==True) /* it was selected - delete text ! */
		{
			dashedBox_marker = (struct fig4 *)mem[i].obj;
			if (dashedBox_start == mem[i].obj)
			{	/* first entry */
				if (dashedBox_curr == dashedBox_start)
				{	/* exactly 1 entry */

					dashedBox_start = NULL;
					dashedBox_curr = NULL;

				} else
					dashedBox_start = dashedBox_start->next;

				free(dashedBox_marker);
			} else
			{
				dashedBox_marker = dashedBox_start;
				do
				{
					four = (struct fig4 *) dashedBox_marker;
					/* remember the last object */
					dashedBox_marker = dashedBox_marker->next;

				} while (dashedBox_marker != mem[i].obj);
				/* <dashedBox_marker> points to the desired object and */
				/* <four> to the object before */

				/* last entry ? */
				if (dashedBox_marker == dashedBox_curr)
				{
					dashedBox_marker = NULL;
					free(dashedBox_curr);
					dashedBox_curr = four;
					four->next=NULL;
				} else
					/* no! the object was somewhere in the middle */
				{
					four->next = dashedBox_marker->next;
					free(dashedBox_marker);
				}
			} /* else */
		

			
		} /* if offon */

		i++;

	} /* while */
} /* proc */



void event_move_base(Widget cmd, XtPointer closure, XEvent *event,
		     Boolean *continue_to_dispatch)
{
     move_base(cmd, closure, 0);
}

void move_base(Widget cmd, XtPointer closure, XtPointer call_data)
{
 int xsteps = 10;
 int ysteps = 10;
 FILE *fd; /* dummy */
 int             btn1 = 256, btn2 = 512, btn3 = 1024;
 unsigned int mask;
 int x,y;
 char typ = *(char *)closure;

  do 
  {
 
 	/* global */
 	xmotion=0;
 	ymotion=0;
 	
 	/* resetting values, global */
 	dim_x_max = -99999.0;
	dim_x_min = 99999.0;
	dim_y_max = -99999.0;
	dim_y_min = 99999.0;
 
	switch (typ)
	{
		case 'U':	ymotion=ysteps;
				break;
		case 'D':	ymotion=-ysteps;
				break;
		case 'L':	xmotion=xsteps;
				break;
		case 'R':	xmotion=-xsteps;
				break;
		default: 	/* compute the dimension of the image */
				/* this is the center image function */
				manage_line(0, fd);
				manage_vector(0, fd);
				manage_filled(0, fd);
				manage_framed(0, fd);
				manage_dashed(0, fd);
				manage_kreis(0, fd);
				manage_disc(0, fd);
				manage_oval(0, fd);
				manage_text(0, fd);
				manage_bezier(0, fd);
				/* dimensions are set ! */
				
				/* compute the move steps */
				xmotion=(x_A4_max-dim_x_max+dim_x_min)/2;
				ymotion=(y_A4_max-dim_y_max+dim_y_min)/2;
				xmotion=xmotion-dim_x_min;
				ymotion=ymotion-dim_y_min;
				break;
	} /* eoswitch */
	
	/* move the objects now */
	
	manage_line(2, fd);
	manage_vector(2, fd);
	manage_filled(2, fd);
	manage_framed(2, fd);
	manage_dashed(2, fd);
	manage_kreis(2, fd);
	manage_disc(2, fd);
	manage_oval(2, fd);
	manage_text(2, fd);
	manage_bezier(2, fd);

	/* redraw screen by refresh */
	refresh((Widget)DUMMY, "?", (caddr_t)DUMMY);	/* force refresh */

	
	PointerPosition(&x,&y,&mask);
	
	
	


   } while ( ((mask&btn1) || (mask&btn2) || (mask&btn3)) && (typ!='Z') );
	
}






void DrawBezier(float ax, float ay, float ex, float ey, float sx, float sy)
{
 /* ax,ay,ex,ey are expected to be always in real coordinate system */
 /* sx,sy depends on variable 'zoomed' !!! : if (zoomed==True), sx,sy are */
 /* expected to be zoomed coordinates */
 
	float u=0;
	Display *disp=XtDisplay(pboard);
	Window win=XtWindow(pboard);
	float x,y,x_old,y_old;
	float steps=0.04;
	
 
  if (zoomed==True)
  {
	real2zoomed(&ax,&ay);
	real2zoomed(&ex,&ey);
  } 
 

  /* draw from u=0, the first point of the curve (that is ax,ay) */
  x_old=ax;
  y_old=ay;

  do
  {
        u+=steps;

        x=ax*(1-u)*(1-u)+sx*(1-u)*2*u+ex*u*u;
        y=ay*(1-u)*(1-u)+sy*(1-u)*2*u+ey*u*u;
        
        XDrawLine(disp,win,gc,(int)x_old,(int)y_old,(int)x,(int)y);

        x_old=x;
        y_old=y;
  } while (u<1-steps);

  /* draw to u=1, the last point of the curve (that is ex,ey) */
  XDrawLine(disp,win,gc,(int)x_old,(int)y_old,(int)ex,(int)ey);
  
}


void set_lastBezier_press()
{
	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[200] = "<Btn1Down>: entry_bezier()\n\
					   <Btn2Down>: cancel_curve()\n\
					   <Btn3Down>: cancel_curve()\n\
					   <LeaveNotify>:exit_curve()";
	char            destination2[80] = "<MotionNotify>: manage_curve()";
	static XtActionsRec actions[80] = {{"manage_curve", manage_curve},
					   {"entry_bezier", entry_bezier},
					   {"cancel_curve", cancel_curve},
					   {"exit_curve", exit_curve}
					  };

	XtAddActions(actions, XtNumber(actions));

	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

	trans_table = XtParseTranslationTable(destination2);
	XtOverrideTranslations(pboard, trans_table);
}


void manage_curve()
{
 /* controlling the curve */

 unsigned int mask;
 int x,y;


	snapPointerPosition(&x,&y,&mask);

	if ( (x!=(int)bezier_curr->sx) || (y!=(int)bezier_curr->sy) ) /* redraw necessary */
	{
	/* delete old curve, if exists */
	if (bezier_curr->sx!=-999.0) /* ax,ay,ex,ey are in real coordinate system */
		DrawBezier(bezier_curr->ax,bezier_curr->ay,
			   bezier_curr->ex,bezier_curr->ey,
			   bezier_curr->sx,bezier_curr->sy);

	bezier_curr->sx=(float)x;
	bezier_curr->sy=(float)y;

	/* draw the curve */
	DrawBezier(bezier_curr->ax,bezier_curr->ay,
		bezier_curr->ex,bezier_curr->ey,
		bezier_curr->sx,bezier_curr->sy);
	}
}

void entry_bezier()
{
	if (bezier_curr->sx==-999.0) return; /* MotionNotify first, please */
	XtUninstallTranslations(pboard);
	if (zoomed==True) zoomed2real(&bezier_curr->sx,&bezier_curr->sy);
	set_first(); /* for the next curve */
}
	

void cancel_curve(Widget widget, XEvent *event, String *params,
		  Cardinal *num_params)
{
 /* btn3 or btn2 was pressed */

	if (bezier_curr->sx!=-999.0) /* delete curve */
		DrawBezier(bezier_curr->ax,bezier_curr->ay,
			   bezier_curr->ex,bezier_curr->ey,
			   bezier_curr->sx,bezier_curr->sy);
	
	XDrawLine(XtDisplay(pboard),XtWindow(pboard),gc,
		(int)bezier_curr->ax,(int)bezier_curr->ay,(int)bezier_curr->ex,(int)bezier_curr->ey);	
	newline(0, 0, 0, 0);
}

void exit_curve()
{
	cancel_curve(0, 0, 0, 0);
	XtUninstallTranslations(pboard);
	graph_action='?';
	headline(pboard,titlename);
	
}









