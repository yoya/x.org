
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


/* pickedit.c */

#include "x_stuff.h"
#include "oberfl.h"
#include "ereignis.h"
#include "graphics.h"
#include "pickedit.h"
#include <math.h>



/* this data-structure contains the best matching objects which are */
/* found during a pick-action... */

#define sumobj 5

static char	question_mark = '?';

int             maxobj = 5;

char            kind[sumobj];	/* the object type: 'L'=line, 'V'=vector,
				 * 'N'=framedBox etc */
int             distance[sumobj];	/* the distance to the selected point
					 * on the screen */
void           *obj[sumobj];	/* pointer to the current element in kind[] */
int             tip;

int             x_pick, y_pick, currIndex;
/* currIndex is THE index which points ALWAYS to the current object */

int             points = 10;	/* radius for edit circle */
/* ----------------------------------- */
/*
 * interface to application is : set_pick_stack(x,y) and nextObject()  and
 * currIndex
 */


void
init_pick()
{
	int             i;
	for (i = 0; i <= maxobj; i++)
	{
		kind[i] = '-';
		distance[i] = 9999;
		obj[i] = NULL;
	}

	tip = 0;
	currIndex = 0;
}


int
nextObject()
{
	/* returns -1 if there is no object & otherwise the index */
	/* of the object in the global data-stucture */

	if (kind[tip] == '-')
		return -1;	/* no object */

	if (tip <= maxobj)
		tip++;
	else
		return -1;

	currIndex = tip - 1;

	return currIndex;
}



void
set_pick_stack(int x, int y)
{
	/* fills the stack with matching objects */
	int             d, res;
	float           h, v;
	init_pick();

	if (zoomed == True)
	{
		h = (float) x;
		v = (float) y;
		zoomed2real(&h, &v);
		x = h;
		y = v;
	}
	/* bezier */
	if (bezier_start != NULL)
	{	/* there is min. 1 entry */
		bezier_marker = bezier_start;
		res = analyse_bezier(bezier_marker, &d, x, y);
		if (res == 1)
			insert_object(bezier_marker, d, ii);

		if (bezier_marker != bezier_curr)
		{
			do
			{
				bezier_marker = bezier_marker->next;
				res = analyse_bezier(bezier_marker, &d, x, y);
				if (res == 1)
					insert_object(bezier_marker, d, ii);
			} while (bezier_marker != bezier_curr);
		}
	}
	/* line */
	if (strich_start != NULL)
	{	/* there is min. 1 entry */
		strich_marker = strich_start;
		res = analyse_pin(strich_marker, &d, x, y);
		if (res == 1)
			insert_object(strich_marker, d, ll);

		if (strich_marker != strich_curr)
		{
			do
			{
				strich_marker = strich_marker->next;
				res = analyse_pin(strich_marker, &d, x, y);
				if (res == 1)
					insert_object(strich_marker, d, ll);
			} while (strich_marker != strich_curr);
		}
	}
	/* vector */
	if (pfeil_start != NULL)
	{	/* there is min. 1 entry */
		pfeil_marker = pfeil_start;
		res = analyse_pin(pfeil_marker, &d, x, y);
		if (res == 1)
			insert_object(pfeil_marker, d, vv);

		if (pfeil_marker != pfeil_curr)
		{
			do
			{
				pfeil_marker = pfeil_marker->next;
				res = analyse_pin(pfeil_marker, &d, x, y);
				if (res == 1)
					insert_object(pfeil_marker, d, vv);
			} while (pfeil_marker != pfeil_curr);
		}
	}
	/* framedBox */
	if (framedBox_start != NULL)
	{	/* there is min. 1 entry */
		framedBox_marker = framedBox_start;
		res = analyse_nbox(framedBox_marker, &d, x, y);
		if (res == 1)
			insert_object(framedBox_marker, d, nn);

		if (framedBox_marker != framedBox_curr)
		{
			do
			{
				framedBox_marker = framedBox_marker->next;
				res = analyse_nbox(framedBox_marker, &d, x, y);
				if (res == 1)
					insert_object(framedBox_marker, d, nn);
			} while (framedBox_marker != framedBox_curr);
		}
	}
	/* dashedBox */
	if (dashedBox_start != NULL)
	{	/* there is min. 1 entry */
		dashedBox_marker = dashedBox_start;
		res = analyse_dbox(dashedBox_marker, &d, x, y);
		if (res == 1)
			insert_object(dashedBox_marker, d, dd);

		if (dashedBox_marker != dashedBox_curr)
		{
			do
			{
				dashedBox_marker = dashedBox_marker->next;
				res = analyse_dbox(dashedBox_marker, &d, x, y);
				if (res == 1)
					insert_object(dashedBox_marker, d, dd);
			} while (dashedBox_marker != dashedBox_curr);
		}
	}
	/* filledBox */
	if (filledBox_start != NULL)
	{	/* there is min. 1 entry */
		filledBox_marker = filledBox_start;
		res = analyse_fbox(filledBox_marker, &d, x, y);
		if (res == 1)
			insert_object(filledBox_marker, d, ff);



		if (filledBox_marker != filledBox_curr)
		{
			do
			{
				filledBox_marker = filledBox_marker->next;
				res = analyse_fbox(filledBox_marker, &d, x, y);
				if (res == 1)
					insert_object(filledBox_marker, d, ff);
			} while (filledBox_marker != filledBox_curr);
		}
	}
	/* normal circle */
	if (kreis_start != NULL)
	{	/* there is min. 1 entry */
		kreis_marker = kreis_start;
		res = analyse_ncircle(kreis_marker, &d, x, y);
		if (res == 1)
			insert_object(kreis_marker, d, cc);

		if (kreis_marker != kreis_curr)
		{
			do
			{
				kreis_marker = kreis_marker->next;
				res = analyse_ncircle(kreis_marker, &d, x, y);
				if (res == 1)
					insert_object(kreis_marker, d, cc);
			} while (kreis_marker != kreis_curr);
		}
	}
	/* filled circle */
	if (disc_start != NULL)
	{	/* there is min. 1 entry */
		disc_marker = disc_start;
		res = analyse_fcircle(disc_marker, &d, x, y);
		if (res == 1)
			insert_object(disc_marker, d, bb);

		if (disc_marker != disc_curr)
		{
			do
			{
				disc_marker = disc_marker->next;
				res = analyse_fcircle(disc_marker, &d, x, y);
				if (res == 1)
					insert_object(disc_marker, d, bb);
			} while (disc_marker != disc_curr);
		}
	}
/* rel13 */
	/* TEXT */
	if (message_start != NULL)
	{	/* there is min. 1 entry */
		message_marker = message_start;
		res = analyse_message(message_marker, &d, x, y);
		if (res == 1)
			insert_object(message_marker, d, tt);

		if (message_marker != message_curr)
		{
			do
			{
				message_marker = message_marker->next;
				res = analyse_message(message_marker, &d, x, y);
				if (res == 1)
					insert_object(message_marker, d, tt);
			} while (message_marker != message_curr);
		}
	}
	/* oval */
	if (oval_start != NULL)
	{	/* there is min. 1 entry */
		oval_marker = oval_start;
		res = analyse_ocircle(oval_marker, &d, x, y);
		if (res == 1)
			insert_object(oval_marker, d, oo);

		if (oval_marker != oval_curr)
		{
			do
			{
				oval_marker = oval_marker->next;
				res = analyse_ocircle(oval_marker, &d, x, y);
				if (res == 1)
					insert_object(oval_marker, d, oo);
			} while (oval_marker != oval_curr);
		}
	}
}


void
insert_object(void *p, int d, char c)
{
	int             i, k;
	/*
	 * operations performed on global data-structure :
	 * 
	 * 
	/* compute insert-position
	 */
	for (i = 0; (distance[i] < d) && (i != maxobj); i++);

	if (i == maxobj)
		return;	/* the distance was too far; there are better objects */

	/* move elements */
	for (k = (maxobj - 1); k > i; k--)
	{
		kind[k] = kind[k - 1];
		distance[k] = distance[k - 1];
		obj[k] = obj[k - 1];
	}

	/* the new entry */
	kind[i] = c;
	distance[i] = d;
	obj[i] = p;

	/* fin */

}


/* function int analyse_xxxxxx() : */
/* returns wether the object is a possible pick(1) or not(0) */
/* p points to the object; d returns the distance to the object */
/* xpos,ypos reference the pick position of the pointer */


int analyse_bezier(struct fig6 *p, int *d, int xpos, int ypos)
{
	int cmpdiff=9999;
	int diff;
	int x,y;
	float u;
	int zone=20;
	
	for (u=0; u<=1; u+=0.1)
	{
		x=(int)(p->ax*(1-u)*(1-u)+p->sx*(1-u)*2*u+p->ex*u*u);
        	y=(int)(p->ay*(1-u)*(1-u)+p->sy*(1-u)*2*u+p->ey*u*u);

		diff=calc_distance(x,y,xpos,ypos);
		
		cmpdiff=(cmpdiff<diff) ? cmpdiff : diff;
	}

	(*d)= cmpdiff;

	return (cmpdiff<zone) ? 1 : 0;
}




int
analyse_pin(struct fig2 * p, int *d, int xpos, int ypos)
{
	float           d1, d2;
	float           x1, x2, y1, y2;
	float           x, y, h, v;
	float           zone;
	double          alpha;


	/* 1. test : bounding rectangle */

	x = p->x;
	y = p->y;
	v = p->v;
	h = p->h;

	zone = 30.0;

	norm_rectangle(&x, &y, &h, &v);
	/* now, x,y is the upper left corner of the rectangle */



	x = x - zone;
	h = h + zone;
	y = y - zone;
	v = v + zone;


	if ((xpos < (int) x) || (xpos > (int) h) || (ypos < (int) y) || (ypos > (int) v))
		return 0;




	/* 2. test : distance */

	x = (p->x);
	y = (p->y);

	/* %%%%%%%%%%%%%%%%%%%%%  see documentation  %%%%%%%%%%%%%%%%%%%%% */

	/* Vector 1 */
	x1 = (float) xpos - x;
	x2 = (float) ypos - y;

	/* compute length of the above vector */

	d1 = (float) sqrt((double) (x1 * x1 + x2 * x2));
	/* --- */

	/* Vector 2 */
	y1 = (p->h) - x;
	y2 = (p->v) - y;

	d2 = (float) sqrt((double) (y1 * y1 + y2 * y2));
	/* --- */


	alpha = (double) ((x1 * y1 + x2 * y2) / (d1 * d2));

	/* acos(x),-1<=x<=1 */
	if (alpha < -1.0)
		alpha = -1.0;
	if (alpha > 1.0)
		alpha = 1.0;


	alpha = acos(alpha);


	(*d) = (int) (d1 * (float) sin(alpha));


	if ((*d) <= (int) zone)
		return 1;
	else
		return 0;

}



int
analyse_nbox(struct fig3 * p, int *d, int xpos, int ypos)
{
	/* normal box */
	float           zone;
	int             x, y, h, v;
	int             dt, db, dr, dl;



	zone = 20.0;

	x = (int) ((p->x) - zone);
	y = (int) ((p->y) - zone);
	v = (int) ((p->v) + zone);
	h = (int) ((p->h) + zone);

	/* 1. test : bounding rectangle */

	if ((xpos < x) || (xpos > h) || (ypos < y) || (ypos > v))
		return 0;


	/* 2. test : distance to a boundery */

	dl = abs(xpos - (int) (p->x));
	dr = abs(xpos - (int) (p->h));
	dt = abs(ypos - (int) (p->y));
	db = abs(ypos - (int) (p->v));

	/* compute the minimum of dr,dl,dt,db and store it in d */

	if (dr < dl)
		dl = dr;	/* minimum in dl */
	if (db < dt)
		dt = db;	/* minimum in dt */

	if (dt < dl)
		(*d) = dt;
	else
		(*d) = dl;


	if ((*d) <= (int) zone)
		return 1;
	else
		return 0;

}



int
analyse_dbox(struct fig4 * p, int *d, int xpos, int ypos)
{
	/* dashed box */
	float           zone;
	int             x, y, h, v;
	int             dt, db, dr, dl;
	zone = 20;

	x = (int) ((p->x) - zone);
	y = (int) ((p->y) - zone);
	v = (int) ((p->v) + zone);
	h = (int) ((p->h) + zone);

	/* 1. test : bounding rectangle */

	if ((xpos < x) || (xpos > h) || (ypos < y) || (ypos > v))
		return 0;


	/* 2. test : distance to a boundery */

	dl = abs(xpos - (int) (p->x));
	dr = abs(xpos - (int) (p->h));
	dt = abs(ypos - (int) (p->y));
	db = abs(ypos - (int) (p->v));

	/* compute the minimum of dr,dl,dt,db and store it in d */

	if (dr < dl)
		dl = dr;	/* minimum in dl */
	if (db < dt)
		dt = db;	/* minimum in dt */

	if (dt < dl)
		(*d) = dt;
	else
		(*d) = dl;


	if ((*d) <= (int) zone)
		return 1;
	else
		return 0;

}



int
analyse_fbox(struct fig2 * p, int *d, int xpos, int ypos)
{
	/* filled box */
	int             x, y, h, v;
	x = (int) (p->x);
	y = (int) (p->y);
	v = (int) (p->v);
	h = (int) (p->h);

	/* test : bounding rectangle */

	if ((xpos < x) || (xpos > h) || (ypos < y) || (ypos > v))
		return 0;

	(*d) = 0;	/* optimal pick */

	return 1;

}


int
analyse_ncircle(struct fig2 * p, int *d, int xpos, int ypos)
{
	/* normal circle */
	float           zone;
	int             x, y, h, v;


	zone = 20.0;

	/* x,y is middlepoint/referencepoint */
	x = (int) ((p->x) - zone) - (p->radius);
	y = (int) ((p->y) - zone) - (p->radius);
	v = (int) ((p->y) + zone) + (p->radius);
	h = (int) ((p->x) + zone) + (p->radius);

	/* now, x,y,h and v define the boundery-rectangle of the circle */

	/* 1. test : bounding rectangle */

	if ((xpos < x) || (xpos > h) || (ypos < y) || (ypos > v))
		return 0;


	/* 2. test : distance to a boundery (point on the circle-slice) */
	/*
	 * compute the difference between radius and the length from
	 * (xpos,ypos) to
	 */
	/* the midlepoint */

	x = xpos - (int) (p->x);
	y = ypos - (int) (p->y);

	(*d) = abs((p->radius) -
		   (int) sqrt((double) ((float) x * (float) x + (float) y * (float) y)));

	if ((*d) <= (int) zone)
		return 1;
	else
		return 0;
}



int
analyse_ocircle(struct fig1 * p, int *d, int xpos, int ypos)
{
	/* oval */
	int             zone;
	int             x, y, h, v;
	int             dt, db, dr, dl;
	zone = 20;

	x = (int) (p->x) - zone;
	y = (int) (p->y) - zone;
	v = (int) (p->v) + zone;
	h = (int) (p->h) + zone;

	/* 1. test : bounding rectangle */

	if ((xpos < x) || (xpos > h) || (ypos < y) || (ypos > v))
		return 0;


	/* 2. test : distance to a boundery */

	dl = abs(xpos - (int) (p->x));
	dr = abs(xpos - (int) (p->h));
	dt = abs(ypos - (int) (p->y));
	db = abs(ypos - (int) (p->v));



	/* compute the minimum of dr,dl,dt,db and store it in d */

	if (dr < dl)
		dl = dr;	/* minimum in dl */
	if (db < dt)
		dt = db;	/* minimum in dt */

	if (dt < dl)
		(*d) = dt;
	else
		(*d) = dl;


	if ((*d) <= zone)
		return 1;
	else
		return 0;

}



int
analyse_fcircle(struct fig2 * p, int *d, int xpos, int ypos)
{
	/* filled circle */
	int             x, y, h;




	/*
	 * 1. test : the position of the selection point must be inside the
	 * fille circle
	 */
	/*
	 * compute the difference between radius and the length from
	 * (xpos,ypos) to
	 */
	/* the midlepoint */

	x = (xpos - (int) (p->x));
	y = (ypos - (int) (p->y));

	h = (int) sqrt((double) ((float) x * (float) x + (float) y * (float) y));	/* length of this vector */

	if (h < (p->radius))
		(*d) = 0;	/* the selection point is fully inside the
				 * circle */
	else
		return 0;

	return 1;
}

/* rel13 */
int
analyse_message(struct fig5 * p, int *d, int xpos, int ypos)
{
	/* filled circle */
	int             x, y, h;
	int		pickzone=15;




	/*
	 * 1. test : the position of the selection point must be inside the
	 * fille circle
	 */
	/*
	 * compute the difference between radius and the length from
	 * (xpos,ypos) to
	 */
	/* the midlepoint */

	x = (xpos - (int) (p->x));
	y = (ypos - (int) (p->y));

	h = (int) sqrt((double) ((float) x * (float) x + (float) y * (float) y));	/* length of this vector */

	if (h < (pickzone))
		(*d) = 0;	/* the selection point is fully inside the
				 * circle */
	else
		return 0;

	return 1;
}



void
set_pick_object(void)
{
	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Up>: manage_pick()";
	static XtActionsRec actions[80] = {{"manage_pick", manage_pick}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}


void
set_copy_object(void)
{
	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Down>: manage_copy()";
	static XtActionsRec actions[80] = {{"manage_copy", manage_copy}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);
}



void
leave_pick(char *message)
{
	Position        xx, yy, y_rel, x_rel;
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

	n = 0;
	XtSetArg(args[n], XtNlabel, message);
	n++;
	XtSetValues(sign_msg, args, n);
	XtPopupSpringLoaded(sign_up);
}


void
manage_pick(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{


	int             index;
	unsigned int    mask;




	PointerPosition(&x_pick, &y_pick, &mask);

	XtUninstallTranslations(pboard);

	set_pick_stack(x_pick, y_pick);	/* pick_stack is filled with matching
					 * objects */

	index = nextObject();

	if (index == -1)
	{	/* no objects found */
		left(0, 0, 0, 0);
		leave_pick("No valid object found !");
		return;
	} else
		pick_manager();
}





void
pick_manager()
{
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	int             index;
	int             x, y, h, v;
	unsigned int    mask, mask2;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	headline(toplevel, "Actions: Button 1 to EDIT ---- Button 2 to MOVE ---- Button 3 for MORE");

	while (True)
	{

		prepare_top_stack_object();

		do
		{	/* watch for any buttonPress */
			PointerPosition(&x, &y, &mask);
			if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
			{
				left(0, 0, 0, 0);
				prepare_top_stack_object();
				return;
			}
			draw_coords(pboard, 0, 0, 0);

		} while (!((mask & btn3) || (mask & btn2) || (mask & btn1)));

		do
		{
			PointerPosition(&h, &v, &mask2);
		} while ((mask2 & btn3));


		prepare_top_stack_object();

		if (mask & btn1)
			pick_edit();
		else if (mask & btn2)
			index = pick_move(x, y);
		else
		{	/* more */
			index = nextObject();

			if (index == -1)
			{	/* no objects found */
				left(0, 0, 0, 0);
				leave_pick("No valid object found !");
				return;
			}
		}

	}	/* while */
}



int
pick_move(int startx, int starty)
{
	/* currIndex points to object-reference in global database */

	struct fig1    *one;
	struct fig2    *two;
	struct fig3    *three;
	struct fig4    *four;
	struct fig5    *five;
	struct fig6    *six;	
	int             x, y, h, v, oldh, oldv;
	unsigned int    mask;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	float           ax, bx, ay, by, sx, sy;
	int             rad;
	float           zoomx, zoomy, zoomh, zoomv, zoomsx, zoomsy;
	Boolean         redraw;
	int             snapx, snapy;


	by = -999.0;
	oldh = -999;
	oldv = oldh;


	do
	{
		PointerPosition(&x, &y, &mask);

		draw_coords(pboard, 0, 0, 0);

		switch (kind[currIndex])
		{
		case 'I':
			six = (struct fig6 *) obj[currIndex];
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				zoomx = six->ax;
				zoomy = six->ay;
				zoomh = six->ex;
				zoomv = six->ey;
				zoomsx = six->sx;
				zoomsy = six->sy;
				
				if (zoomed == True) real2zoomed(&zoomsx, &zoomsy);
				
				/* delete old line */
				if (by == -999.0)
					DrawBezier(zoomx, zoomy, zoomh, zoomv, zoomsx, zoomsy);
				else
					DrawBezier(ax, ay, bx, by, sx, sy);
					
				if (zoomed == True) 
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;
				sx = zoomsx + (float) h;
				sy = zoomsy + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					sx -= (ax - (float) snapx);
					sy -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				/* draw new line */
				
				if (zoomed==True)
				{
					zoomed2real(&ax,&ay);
					zoomed2real(&bx,&by);
				}
				
				DrawBezier(ax, ay, bx, by, sx, sy);
			}
			break;

		case 'L':
			two = (struct fig2 *) obj[currIndex];
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				zoomx = two->x;
				zoomy = two->y;
				zoomh = two->h;
				zoomv = two->v;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				/* delete old line */
				if (by == -999.0)
					XDrawLine(disp, win, gc, (int) zoomx, (int) zoomy,
						  (int) zoomh, (int) zoomv);
				else
					XDrawLine(disp, win, gc, (int) ax, (int) ay,
						  (int) bx, (int) by);
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				/* draw new line */
				XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
			}
			break;

		case 'V':
+			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				two = (struct fig2 *) obj[currIndex];
				zoomx = two->x;
				zoomy = two->y;
				zoomh = two->h;
				zoomv = two->v;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
				{
					XDrawLine(disp, win, gc, (int) zoomx, (int) zoomy,
						  (int) zoomh, (int) zoomv);
					draw_vector_marker(zoomx, zoomy, zoomh, zoomv);
				} else
				{
					XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
					draw_vector_marker(ax, ay, bx, by);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
				draw_vector_marker(ax, ay, bx, by);
			}
			break;

		case 'N':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				three = (struct fig3 *) obj[currIndex];
				zoomx = three->x;
				zoomy = three->y;
				zoomh = three->h;
				zoomv = three->v;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
				{
					XDrawRectangle(disp, win, gc, (int) zoomx, (int) zoomy,
					     (unsigned int) (zoomh - zoomx),
					    (unsigned int) (zoomv - zoomy));
					print_box_text(three->textpos, zoomx, zoomy, zoomh, zoomv, three->text);
				} else
				{
					XDrawRectangle(disp, win, gc,
						       (int) ax, (int) ay,
						   (unsigned int) (bx - ax),
						  (unsigned int) (by - ay));
					XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
					XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawRectangle(disp, win, gc,
					       (int) ax, (int) ay,
					       (unsigned int) (bx - ax),
					       (unsigned int) (by - ay));
				XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
				XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
			}
			break;

		case 'D':
			h = x - startx;
			v = y - starty;
			if ((h != oldh) || (v != oldv))
			{
				oldh = h;
				oldv = v;
				four = (struct fig4 *) obj[currIndex];
				zoomx = four->x;
				zoomy = four->y;
				zoomh = four->h;
				zoomv = four->v;
				XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
				{
					XDrawRectangle(disp, win, gc, (int) zoomx, (int) zoomy,
					     (unsigned int) (zoomh - zoomx),
					    (unsigned int) (zoomv - zoomy));
					print_box_text(four->textpos, zoomx, zoomy, zoomh, zoomv, four->text);
				} else
				{
					XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
						   (unsigned int) (bx - ax),
						  (unsigned int) (by - ay));
					XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
					XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
					       (unsigned int) (bx - ax),
					       (unsigned int) (by - ay));
				XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
				XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
				XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
			}
			break;

		case 'F':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				two = (struct fig2 *) obj[currIndex];
				zoomx = two->x;
				zoomy = two->y;
				zoomh = two->h;
				zoomv = two->v;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
				{
					XFillRectangle(disp, win, gc, (int) zoomx, (int) zoomy,
					     (unsigned int) (zoomh - zoomx),
					    (unsigned int) (zoomv - zoomy));
				} else
				{
					XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
						   (unsigned int) (bx - ax),
						  (unsigned int) (by - ay));
					XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
					XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
					       (unsigned int) (bx - ax),
					       (unsigned int) (by - ay));
				XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
				XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
			}
			break;

		case 'C':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				two = (struct fig2 *) obj[currIndex];
				zoomx = two->x;
				zoomy = two->y;
				rad = two->radius;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
					rad *= 10;
				}
				if (by == -999.0)
				{
					XDrawArc(disp, win, gc, (int) zoomx - rad,
						 (int) zoomy - rad,
						 (unsigned int) (rad + rad),
						 (unsigned int) (rad + rad), 0, 64 * 360);
					by = 0;
				} else
				{
					XDrawArc(disp, win, gc, (int) (ax - (float) rad),
						 (int) (ay - (float) rad),
						 (unsigned int) (rad + rad),
						 (unsigned int) (rad + rad), 0, 64 * 360);
					XDrawLine(disp, win, gc,
						  (int) ax, (int) (ay - rad),
						  (int) ax, (int) (ay + rad));
					XDrawLine(disp, win, gc,
						  (int) (ax - rad), (int) ay,
						  (int) (ax + rad), (int) ay);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawArc(disp, win, gc, (int) (ax - (float) rad),
					 (int) (ay - (float) rad),
					 (unsigned int) (rad + rad),
				   (unsigned int) (rad + rad), 0, 64 * 360);
				XDrawLine(disp, win, gc, (int) ax, (int) ay - rad, (int) ax, (int) ay + rad);
				XDrawLine(disp, win, gc, (int) ax - rad, (int) ay, (int) ax + rad, (int) ay);
			}
			break;

		case 'B':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				two = (struct fig2 *) obj[currIndex];
				zoomx = two->x;
				zoomy = two->y;
				rad = two->radius;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
					rad *= 10;
				}
				if (by == -999.0)
				{
					XFillArc(disp, win, gc, (int) zoomx - rad, (int) zoomy - rad,
						 (unsigned int) (rad + rad),
						 (unsigned int) (rad + rad), 0, 64 * 360);
					by = 0;
				} else
				{
					XDrawArc(disp, win, gc, (int) (ax - (float) rad),
						 (int) (ay - (float) rad),
						 (unsigned int) (rad + rad),
						 (unsigned int) (rad + rad), 0, 64 * 360);
					XDrawLine(disp, win, gc, (int) ax, (int) ay - rad, (int) ax, (int) ay + rad);
					XDrawLine(disp, win, gc, (int) ax - rad, (int) ay, (int) ax + rad, (int) ay);
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				XDrawArc(disp, win, gc, (int) (ax - (float) rad),
					 (int) (ay - (float) rad),
					 (unsigned int) (rad + rad),
				   (unsigned int) (rad + rad), 0, 64 * 360);
				XDrawLine(disp, win, gc, (int) ax, (int) ay - rad, (int) ax, (int) ay + rad);
				XDrawLine(disp, win, gc, (int) ax - rad, (int) ay, (int) ax + rad, (int) ay);
			}
			break;
			
/* rel13 */
		case 'T':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				five = (struct fig5 *) obj[currIndex];
				zoomx = five->x;
				zoomy = five->y;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
				{
					if (zoomed==True) print_text_text_marker(message_marker->textpos,zoomx,
							zoomy,message_marker->text);
				        else XDrawString(disp, win, gc, message_marker->x, message_marker->y,
				                  		message_marker->text, 1);
					by = 0;
				} else
				{
					print_text_text_marker(message_marker->textpos, ax, ay,message_marker->text);
				        
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				print_text_text_marker(message_marker->textpos,ax, ay,message_marker->text);
				
			}
			break;
			

		case 'O':
			h = x - startx;
			v = y - starty;
			if ((oldh != h) || (oldv != v))
			{
				oldh = h;
				oldv = v;
				one = (struct fig1 *) obj[currIndex];
				zoomx = one->x;
				zoomy = one->y;
				zoomh = one->h;
				zoomv = one->v;
				if (zoomed == True)
				{
					real2zoomed(&zoomx, &zoomy);
					real2zoomed(&zoomh, &zoomv);
				}
				if (by == -999.0)
					DrawOval((int) zoomx, (int) zoomy, (int) zoomh, (int) zoomv);
				else
				{
					DrawOval((int) ax, (int) ay, (int) bx, (int) by);
					XDrawLine(disp, win, gc, (int) (ax + (bx - ax) / 2), (int) ay, (int) (ax + (bx - ax) / 2), (int) by);
					XDrawLine(disp, win, gc, (int) ax, (int) (ay + (by - ay) / 2), (int) bx, (int) (ay + (by - ay) / 2));
				}
				ax = zoomx + (float) h;
				ay = zoomy + (float) v;
				bx = zoomh + (float) h;
				by = zoomv + (float) v;

				if ((snap == True) && (raster == True))
				{
					snapx = (int) ax;
					snapy = (int) ay;
					valid_snap_coords(&snapx, &snapy);
					bx -= (ax - (float) snapx);
					by -= (ay - (float) snapy);
					ax = (float) snapx;
					ay = (float) snapy;
				}
				DrawOval((int) ax, (int) ay, (int) bx, (int) by);
				XDrawLine(disp, win, gc, (int) (ax + (bx - ax) / 2), (int) ay, (int) (ax + (bx - ax) / 2), (int) by);
				XDrawLine(disp, win, gc, (int) ax, (int) (ay + (by - ay) / 2), (int) bx, (int) (ay + (by - ay) / 2));
			}
			break;

		default:
			break;

		}	/* switch */
	} while ((mask & btn2));	/* || ( (graph_action=='Y') && (mask & btn1) ) ); */


	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
	{	/* cancel */

		if ((refr_auto == True) && (graph_action != 'Y'))
			refresh(pboard, &question_mark, (caddr_t)DUMMY);
		return -1;
	}
	/* store the new position of the object */

	switch (kind[currIndex])
	{
	case 'I':
		six = (struct fig6 *) obj[currIndex];
		if (zoomed == True) zoomed2real(&sx, &sy);
		six->ax = ax;
		six->ay = ay;
		six->ex = bx;
		six->ey = by;
		six->sx = sx;
		six->sy = sy;
		break;

	case 'L':
		two = (struct fig2 *) obj[currIndex];
		if (zoomed == True)
		{
			zoomed2real(&ax, &ay);
			zoomed2real(&bx, &by);
		}
		two->x = ax;
		two->y = ay;
		two->h = bx;
		two->v = by;
		break;

	case 'V':
		two = (struct fig2 *) obj[currIndex];
		if (zoomed == True)
		{
			zoomed2real(&ax, &ay);
			zoomed2real(&bx, &by);
		}
		two->x = ax;
		two->y = ay;
		two->h = bx;
		two->v = by;
		break;


	case 'N':
		three = (struct fig3 *) obj[currIndex];
		XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
		XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
		zoomx = ax;
		zoomy = ay;
		zoomh = bx;
		zoomv = by;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		three->x = zoomx;
		three->y = zoomy;
		three->h = zoomh;
		three->v = zoomv;
		print_box_text(three->textpos, zoomx, zoomy, zoomh, zoomv, three->text);
		break;

	case 'D':
		four = (struct fig4 *) obj[currIndex];
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
		XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
		XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
		zoomx = ax;
		zoomy = ay;
		zoomh = bx;
		zoomv = by;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		four->x = zoomx;
		four->y = zoomy;
		four->h = zoomh;
		four->v = zoomv;
		print_box_text(four->textpos, zoomx, zoomy, zoomh, zoomv, four->text);
		break;

	case 'F':
		two = (struct fig2 *) obj[currIndex];
		XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
		XDrawLine(disp, win, gc, (int) ax, (int) by, (int) bx, (int) ay);
		XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
			       (unsigned int) (bx - ax),
			       (unsigned int) (by - ay));
		XFillRectangle(disp, win, gc, (int) ax, (int) ay,
			       (unsigned int) (bx - ax),
			       (unsigned int) (by - ay));
		zoomx = ax;
		zoomy = ay;
		zoomh = bx;
		zoomv = by;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		two->x = zoomx;
		two->y = zoomy;
		two->h = zoomh;
		two->v = zoomv;
		break;

	case 'C':
		two = (struct fig2 *) obj[currIndex];
		XDrawLine(disp, win, gc, (int) ax - rad, (int) ay, (int) ax + rad, (int) ay);
		XDrawLine(disp, win, gc, (int) ax, (int) ay - rad, (int) ax, (int) ay + rad);
		zoomx = ax;
		zoomy = ay;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		two->x = zoomx;
		two->y = zoomy;
		break;

	case 'B':
		two = (struct fig2 *) obj[currIndex];
		XDrawLine(disp, win, gc, (int) ax - rad, (int) ay, (int) ax + rad, (int) ay);
		XDrawLine(disp, win, gc, (int) ax, (int) ay - rad, (int) ax, (int) ay + rad);
		XDrawArc(disp, win, gc, (int) ax - rad, (int) ay - rad,
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 64 * 360);
		XFillArc(disp, win, gc, (int) ax - rad, (int) ay - rad,
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 64 * 360);
		zoomx = ax;
		zoomy = ay;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		two->x = zoomx;
		two->y = zoomy;
		break;
/* rel13 */
	case 'T':
		five = (struct fig5 *) obj[currIndex];
		zoomx = ax;
		zoomy = ay;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		five->x = zoomx;
		five->y = zoomy;
		break;

	case 'O':
		one = (struct fig1 *) obj[currIndex];
		XDrawLine(disp, win, gc, (int) (ax + (bx - ax) / 2), (int) ay,
			  (int) (ax + (bx - ax) / 2), (int) by);
		XDrawLine(disp, win, gc, (int) ax, (int) (ay + (by - ay) / 2),
			  (int) bx, (int) (ay + (by - ay) / 2));
		zoomx = ax;
		zoomy = ay;
		zoomh = bx;
		zoomv = by;
		if (zoomed == True)
		{
			zoomed2real(&zoomx, &zoomy);
			zoomed2real(&zoomh, &zoomv);
		}
		one->x = zoomx;
		one->y = zoomy;
		one->h = zoomh;
		one->v = zoomv;
		break;


	default:
		break;

	}	/* switch */




	if (refr_auto == True)
		refresh(pboard, &question_mark, (caddr_t)DUMMY);

	return 0;


}


void
pick_edit()
{
	/* currIndex points to object-reference in global database */
	/* button 1 is pressed */

	struct fig1    *one;
	struct fig2    *two;
	struct fig3    *three;
	struct fig4    *four;
	struct fig5   *five;
	struct fig6    *six;
	int             x, y, dist1, dist2, dist3, dist4, low_val;
	unsigned int    mask;
	double          vec_x, vec_y, vec_h, vec_v;
	int             res1, res2;
	float           h_pos, v_pos, x_pos, y_pos;
	float           h, v;





	PointerPosition(&x, &y, &mask);


	if (zoomed == True)
	{
		h = (float) x;
		v = (float) y;
		zoomed2real(&h, &v);
		x = (int) h;
		y = (int) v;
	}
	switch (kind[currIndex])
	{
	case 'I':	/* which point ? */
		six = (struct fig6 *) obj[currIndex];
		dist1 = calc_distance(x, y, (int) six->ax, (int) six->ay);	/* ax,ay */
		dist2 = calc_distance(x, y, (int) six->ex, (int) six->ey);	/* ex,ey */
		dist3 = calc_distance(x, y, (int) six->sx, (int) six->sy);	/* sx,sy */
		if (zoomed == True)
		{
			dist1 *= 10;
			dist2 *= 10;
			dist3 *= 10;
		}
		/* compute minimum */
		low_val=(dist1<dist2) ? dist1 : dist2;
		low_val=(low_val<dist3) ? low_val : dist3;
		if (low_val<=points)
		{ /* buttonpress was inside of circle */
		if (low_val==dist1) edit_bezier_curveA();
			else if (low_val==dist2) edit_bezier_curveE();
			else edit_bezier_curveS();	

			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}	/* if low_val<=points */
		break;

	case 'L':	/* edit which endpoint ? */
		two = (struct fig2 *) obj[currIndex];
		vec_x = (double) two->x - (double) x;
		vec_y = (double) two->y - (double) y;
		vec_h = (double) two->h - (double) x;
		vec_v = (double) two->v - (double) y;
		res1 = abs((int) sqrt(vec_x * vec_x + vec_y * vec_y));
		res2 = abs((int) sqrt(vec_h * vec_h + vec_v * vec_v));
		if (zoomed == True)
		{
			res1 = res1 * 10;
			res2 = res2 * 10;
		}
		if ((res1 <= points) || (res2 <= points))
		{
			if (res1 < res2)
			{
				edit_x_line();
				if (refr_auto == True)
					refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
			} else
			{
				edit_y_line();
				if (refr_auto == True)
					refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
			}
		}
		break;

	case 'V':	/* edit which endpoint ? */
		two = (struct fig2 *) obj[currIndex];
		vec_x = (double) two->x - (double) x;
		vec_y = (double) two->y - (double) y;
		vec_h = (double) two->h - (double) x;
		vec_v = (double) two->v - (double) y;
		res1 = abs((int) sqrt(vec_x * vec_x + vec_y * vec_y));
		res2 = abs((int) sqrt(vec_h * vec_h + vec_v * vec_v));
		if (zoomed == True)
		{
			res1 = res1 * 10;
			res2 = res2 * 10;
		}
		if ((res1 <= points) || (res2 <= points))
		{
			if (res1 < res2)
				edit_x_vector();
			else
				edit_y_vector();
		}
		break;

	case 'N':	/* which point ? */
		three = (struct fig3 *) obj[currIndex];
		dist1 = calc_distance(x, y, (int) three->x, (int) three->y);	/* upper left */
		dist2 = calc_distance(x, y, (int) three->h, (int) three->v);	/* lower right */
		dist3 = calc_distance(x, y, (int) three->h, (int) three->y);	/* upper right */
		dist4 = calc_distance(x, y, (int) three->x, (int) three->v);	/* lower left */
		if (zoomed == True)
		{
			dist1 *= 10;
			dist2 *= 10;
			dist3 *= 10;
			dist4 *= 10;
		}
		/* compute minimum */
		if (dist1 < dist2)
			low_val = dist1;
		else
			low_val = dist2;
		if (dist3 < low_val)
			low_val = dist3;
		if (dist4 < low_val)
			low_val = dist4;
		if (low_val <= points)
		{
			x_pos = three->x;
			y_pos = three->y;
			h_pos = three->h;
			v_pos = three->v;
			if (low_val == dist1)
			{
				three->x = h_pos;
				three->y = v_pos;
				three->h = x_pos;
				three->v = y_pos;
				/*
				 * now, coordinates are in the correct
				 * order...
				 */
			} else if (low_val == dist3)
			{
				three->y = v_pos;
				three->v = y_pos;
			} else if (low_val == dist4)
			{
				three->x = h_pos;
				three->h = x_pos;
			}
			/* low_val==dist2 is default... */
			edit_frame(nn);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}	/* if low_val<=points */
		break;


	case 'F':	/* which point ? */
		two = (struct fig2 *) obj[currIndex];
		dist1 = calc_distance(x, y, (int) two->x, (int) two->y);	/* upper left */
		dist2 = calc_distance(x, y, (int) two->h, (int) two->v);	/* lower right */
		dist3 = calc_distance(x, y, (int) two->h, (int) two->y);	/* upper right */
		dist4 = calc_distance(x, y, (int) two->x, (int) two->v);	/* lower left */
		if (zoomed == True)
		{
			dist1 *= 10;
			dist2 *= 10;
			dist3 *= 10;
			dist4 *= 10;
		}
		/* compute minimum */
		if (dist1 < dist2)
			low_val = dist1;
		else
			low_val = dist2;
		if (dist3 < low_val)
			low_val = dist3;
		if (dist4 < low_val)
			low_val = dist4;
		if (low_val <= points)
		{
			x_pos = two->x;
			y_pos = two->y;
			h_pos = two->h;
			v_pos = two->v;
			if (low_val == dist1)
			{
				two->x = h_pos;
				two->y = v_pos;
				two->h = x_pos;
				two->v = y_pos;
				/*
				 * now, coordinates are in the correct
				 * order...
				 */
			} else if (low_val == dist3)
			{
				two->y = v_pos;
				two->v = y_pos;
			} else if (low_val == dist4)
			{
				two->x = h_pos;
				two->h = x_pos;
			}
			/* low_val==dist2 is default... */
			edit_frame(ff);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}	/* if low_val<=points */
		break;



	case 'D':	/* which point ? */
		four = (struct fig4 *) obj[currIndex];
		dist1 = calc_distance(x, y, (int) four->x, (int) four->y);	/* upper left */
		dist2 = calc_distance(x, y, (int) four->h, (int) four->v);	/* lower right */
		dist3 = calc_distance(x, y, (int) four->h, (int) four->y);	/* upper right */
		dist4 = calc_distance(x, y, (int) four->x, (int) four->v);	/* lower left */
		if (zoomed == True)
		{
			dist1 *= 10;
			dist2 *= 10;
			dist3 *= 10;
			dist4 *= 10;
		}
		/* compute minimum */
		if (dist1 < dist2)
			low_val = dist1;
		else
			low_val = dist2;
		if (dist3 < low_val)
			low_val = dist3;
		if (dist4 < low_val)
			low_val = dist4;
		if (low_val <= points)
		{
			x_pos = four->x;
			y_pos = four->y;
			h_pos = four->h;
			v_pos = four->v;
			if (low_val == dist1)
			{
				four->x = h_pos;
				four->y = v_pos;
				four->h = x_pos;
				four->v = y_pos;
				/*
				 * now, coordinates are in the correct
				 * order...
				 */
			} else if (low_val == dist3)
			{
				four->y = v_pos;
				four->v = y_pos;
			} else if (low_val == dist4)
			{
				four->x = h_pos;
				four->h = x_pos;
			}
			/* low_val==dist2 is default... */
			edit_frame(dd);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}	/* if low_val<=points */
		break;

	case 'O':	/* which point ? */
		one = (struct fig1 *) obj[currIndex];
		dist1 = calc_distance(x, y, (int) one->x, (int) one->y);	/* upper left */
		dist2 = calc_distance(x, y, (int) one->h, (int) one->v);	/* lower right */
		dist3 = calc_distance(x, y, (int) one->h, (int) one->y);	/* upper right */
		dist4 = calc_distance(x, y, (int) one->x, (int) one->v);	/* lower left */
		if (zoomed == True)
		{
			dist1 *= 10;
			dist2 *= 10;
			dist3 *= 10;
			dist4 *= 10;
		}
		/* compute minimum */
		if (dist1 < dist2)
			low_val = dist1;
		else
			low_val = dist2;
		if (dist3 < low_val)
			low_val = dist3;
		if (dist4 < low_val)
			low_val = dist4;
		if (low_val <= points)
		{
			x_pos = one->x;
			y_pos = one->y;
			h_pos = one->h;
			v_pos = one->v;
			if (low_val == dist1)
			{
				one->x = h_pos;
				one->y = v_pos;
				one->h = x_pos;
				one->v = y_pos;
				/*
				 * now, coordinates are in the correct
				 * order...
				 */
			} else if (low_val == dist3)
			{
				one->y = v_pos;
				one->v = y_pos;
			} else if (low_val == dist4)
			{
				one->x = h_pos;
				one->h = x_pos;
			}
			/* low_val==dist2 is default... */
			edit_frame(oo);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}	/* if low_val<=points */
		break;

	case 'C':
		low_val = valid_circle_edit(x, y);
		if (zoomed == True)
			low_val *= 10;
		if (low_val <= points)
		{
			edit_circle(cc);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}
		break;

	case 'B':
		low_val = valid_circle_edit(x, y);
		if (zoomed == True)
			low_val *= 10;
		if (low_val <= points)
		{
			edit_circle(bb);
			if (refr_auto == True)
				refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */
		}
		break;



	default:
		break;

	}	/* switch */
}/* pick_edit */


int
valid_circle_edit(int x, int y)
{
	struct fig2    *two;
	int             dist;
	two = (struct fig2 *) obj[currIndex];

	dist = calc_distance((int) two->x, (int) two->y, x, y);

	return abs(dist - two->radius);
}





void
edit_circle(char ch)
{
	struct fig2    *two;
	int             x, y, rad, zoom, rad2, oldrad;
	unsigned int    mask;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             btn1 = 256, btn2 = 512, btn3 = 1024;


	two = (struct fig2 *) obj[currIndex];

	oldrad = two->radius;


	if (zoomed == True)
	{
		real2zoomed(&two->x, &two->y);
		zoom = 10;
	} else
		zoom = 1;



	do
	{
		PointerPosition(&x, &y, &mask);

		/* delete old */
		rad = two->radius;
		if (ch == 'C')
			valid_kreis_coords((int) (two->x), (int) (two->y), &x, &y, &rad2);
		else
			valid_disc_coords((int) (two->x), (int) (two->y), &x, &y, &rad2);

		if (rad2 != rad)
		{	/* redraw necessary */


			draw_coords(pboard, 0, 0, 0);

			if (ch == 'C')
				XDrawArc(disp, win, gc,
					 (int) (two->x) - (rad * zoom), (int) (two->y) - (rad * zoom),
					 (unsigned int) (zoom * (rad + rad)),
					 (unsigned int) (zoom * (rad + rad)), 0, 360 * 64);
			else
				XFillArc(disp, win, gc,
					 (int) (two->x) - (rad * zoom), (int) (two->y) - (rad * zoom),
					 (unsigned int) (zoom * (rad + rad)),
					 (unsigned int) (zoom * (rad + rad)), 0, 360 * 64);

			rad = rad2;
			two->radius = rad;
			if (ch == 'C')
				XDrawArc(disp, win, gc,
					 (int) (two->x) - (rad * zoom), (int) (two->y) - (rad * zoom),
					 (unsigned int) (zoom * (rad + rad)),
					 (unsigned int) (zoom * (rad + rad)), 0, 360 * 64);
			else
				XFillArc(disp, win, gc,
					 (int) (two->x) - (rad * zoom), (int) (two->y) - (rad * zoom),
					 (unsigned int) (zoom * (rad + rad)),
					 (unsigned int) (zoom * (rad + rad)), 0, 360 * 64);

		}	/* redraw necessary */
	} while (mask & btn1);


	if (zoomed == True)
		zoomed2real(&two->x, &two->y);

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
	{	/* cancel */
		two->radius = oldrad;
		return;
	}
}


void
edit_bezier_curveA()
{
  struct fig6 *six=(struct fig6 *) obj[currIndex];
  float sx,sy;
  float x_old;
  float y_old;
  Display *disp=XtDisplay(pboard);
  Window win=XtWindow(pboard);
  int x,y;
  unsigned int mask;
  int btn1=256, btn2=512, btn3=1024;
  float help1,help2;


	sx=six->sx;
	sy=six->sy;

	if (zoomed==True) real2zoomed(&sx,&sy);

	x_old=six->ax;
	y_old=six->ay;
 
	do
	{
		snapPointerPosition(&x, &y, &mask);
		
		if (zoomed==True)
			{
				help1=(float)x;
				help2=(float)y;
				zoomed2real(&help1,&help2);
				x=(int)help1;
				y=(int)help2;
			}
		
		if ((x != (int)x_old) || (y != (int)y_old)) 
		{	/* redraw necessary */
			draw_coords(pboard, 0, 0, 0);

			

			/* delete old object */
			
			DrawBezier(x_old,y_old,six->ex,six->ey,sx,sy);

			x_old=(float)x;
			y_old=(float)y;

			/* draw new */

			DrawBezier(x_old,y_old,six->ex,six->ey,sx,sy);

		}  /* redraw */

	} while (mask & btn1);

	/* btn1 was released... */
	/* cancel, if pointer is outside of pboard */

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))	/* cancel */
		return;


	/* save new coordinates & translate them to the right mode */
	/* new coordinate is in x_old,y_old */

	six->ax=x_old;
	six->ay=y_old;
}



void edit_bezier_curveE()
{
  struct fig6 *six=(struct fig6 *) obj[currIndex];
  float sx,sy;
  float x_old;
  float y_old;
  Display *disp=XtDisplay(pboard);
  Window win=XtWindow(pboard);
  int x,y;
  unsigned int mask;
  int btn1=256,btn2=512,btn3=1024;
  float help1,help2;


	sx=six->sx;
	sy=six->sy;

	if (zoomed==True) real2zoomed(&sx,&sy);

	x_old=six->ex;
	y_old=six->ey;
 
	do
	{
		snapPointerPosition(&x, &y, &mask);
		if (zoomed==True)
		{
			help1=(float)x;
			help2=(float)y;
			zoomed2real(&help1,&help2);
			x=(int)help1;
			y=(int)help2;
		}
		if ((x != (int)x_old) || (y != (int)y_old))
		{	/* redraw necessary */
			draw_coords(pboard, 0, 0, 0);

			/* delete old object */
			
			DrawBezier(six->ax,six->ay,x_old,y_old,sx,sy);

			x_old=(float)x;
			y_old=(float)y;

			/* draw new */

			DrawBezier(six->ax,six->ay,x_old,y_old,sx,sy);

		} /* redraw */

	} while (mask & btn1);

	/* btn1 was released... */
	/* cancel, if pointer is outside of pboard */

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))	/* cancel */
		return;


	/* save new coordinates & translate them to the right mode */
	/* new coordinate is in x_old,y_old */

	six->ex=x_old;
	six->ey=y_old;
}


void edit_bezier_curveS()
{
  struct fig6 *six=(struct fig6 *) obj[currIndex];
  float sx,sy;
  float x_old;
  float y_old;
  Display *disp=XtDisplay(pboard);
  Window win=XtWindow(pboard);
  int x,y;
  unsigned int mask;
  int btn1=256,btn2=512,btn3=1024;
  

	

	

	x_old=six->sx;
	y_old=six->sy;

	if (zoomed==True) real2zoomed(&x_old,&y_old);
 
	do
	{
		snapPointerPosition(&x, &y, &mask);
		if ((x != (int)x_old) || (y != (int)y_old))
		{	/* redraw necessary */
			draw_coords(pboard, 0, 0, 0);

			/* delete old object */
			
			DrawBezier(six->ax,six->ay,six->ex,six->ey,x_old,y_old);

			x_old=(float)x;
			y_old=(float)y;

			/* draw new */

			DrawBezier(six->ax,six->ay,six->ex,six->ey,x_old,y_old);

		} /* redraw */

	} while (mask & btn1);

	/* btn1 was released... */
	/* cancel, if pointer is outside of pboard */

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))	/* cancel */
		return;


	/* save new coordinates & translate them to the right mode */
	/* new coordinate is in x_old,y_old */

	if (zoomed==True) zoomed2real(&x_old,&y_old);

	six->sx=x_old;
	six->sy=y_old;
}


void
edit_frame(char ch)
{
	struct fig1    *one;
	struct fig2    *two;
	struct fig3    *three;
	struct fig4    *four;
	unsigned int    mask;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	float           x1, x2, y1, y2, y1old, y2old;
	float           x11, x22, y11, y22;
	int             x, y;
	y22 = 999.0;

	switch (ch)
	{

	case 'N':
		three = (struct fig3 *) obj[currIndex];
		x1 = three->x;
		x2 = three->y;
		y1 = three->h;
		y2 = three->v;
		break;

	case 'D':
		four = (struct fig4 *) obj[currIndex];
		x1 = four->x;
		x2 = four->y;
		y1 = four->h;
		y2 = four->v;
		break;

	case 'F':
		two = (struct fig2 *) obj[currIndex];
		x1 = two->x;
		x2 = two->y;
		y1 = two->h;
		y2 = two->v;
		break;

	case 'O':
		one = (struct fig1 *) obj[currIndex];
		x1 = one->x;
		x2 = one->y;
		y1 = one->h;
		y2 = one->v;
		break;

	default:
		break;

	}	/* switch */





	/* x=(x1,x2)  y=(y1,y2) */

	/* save y-coordinate */
	y1old = y1;
	y2old = y2;


	if (zoomed == True)
	{
		real2zoomed(&x1, &x2);
		real2zoomed(&y1, &y2);
	}
	/* (x1,x2) is a fixed point, 'edit' influences (y1,y2) only */



	do
	{
		snapPointerPosition(&x, &y, &mask);
		if ((x != y1) || (y != y2))
		{	/* redraw necessary */
			draw_coords(pboard, 0, 0, 0);
			/* delete old object */
			if (y22 == 999.0)
			{
				y11 = y1;
				y22 = y2;
				x11 = x1;
				x22 = x2;
				norm_rectangle(&x11, &x22, &y11, &y22);
			}
			switch (ch)
			{
			case 'N':
				XDrawRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				break;
			case 'F':
				XFillRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				break;


			case 'D':
				XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
				XDrawRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
				break;

			case 'O':
				DrawOval((int) x11, (int) x22, (int) y11, (int) y22);
				break;

			default:
				break;

			}	/* switch */



			/* draw new */
			x11 = x1;
			x22 = x2;
			y11 = (float) x;
			y22 = (float) y;
			y1 = y11;
			y2 = y22;
			norm_rectangle(&x11, &x22, &y11, &y22);
			switch (ch)
			{
			case 'N':
				XDrawRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				break;
			case 'F':
				XFillRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				break;


			case 'D':
				XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
				XDrawRectangle(disp, win, gc, (int) x11, (int) x22,
					 (unsigned int) ((int) (y11 - x11)),
					(unsigned int) ((int) (y22 - x22)));
				XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
				break;

			case 'O':
				DrawOval((int) x11, (int) x22, (int) y11, (int) y22);
				break;

			default:
				break;

			}	/* switch */

		}	/* redraw */
	} while (mask & btn1);

	/* btn1 was released... */
	/* cancel, if pointer is outside of pboard */

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))	/* cancel */
		return;


	/* save new coordinates & translate them to the right mode */

	switch (ch)
	{
	case 'N':
		three->h = y1;
		three->v = y2;
		if (zoomed == True)
			zoomed2real(&three->h, &three->v);
		norm_rectangle(&three->x, &three->y, &three->h, &three->v);
		break;

	case 'D':
		four->h = y1;
		four->v = y2;
		if (zoomed == True)
			zoomed2real(&four->h, &four->v);
		norm_rectangle(&four->x, &four->y, &four->h, &four->v);
		break;

	case 'F':
		two->h = y1;
		two->v = y2;
		if (zoomed == True)
			zoomed2real(&two->h, &two->v);
		norm_rectangle(&two->x, &two->y, &two->h, &two->v);
		break;

	case 'O':
		one->h = y1;
		one->v = y2;
		if (zoomed == True)
			zoomed2real(&one->h, &one->v);
		norm_rectangle(&one->x, &one->y, &one->h, &one->v);
		break;

	default:
		break;

	}	/* switch */


}



int
calc_distance(int x, int y, int h, int v)
{
	double          res;
	x -= h;
	y -= v;

	res = (double) (x * x + y * y);
	return (int) sqrt(res);
}





void
edit_x_line()
{

	struct fig2    *two;
	int             x, y;
	unsigned int    mask;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	float           xold, yold;
	float           x1, x2, y1, y2;
	two = (struct fig2 *) obj[currIndex];

	if (zoomed == True)
	{	/* prepare coordinates for right mode */
		real2zoomed(&two->x, &two->y);
		real2zoomed(&two->h, &two->v);
	}
	/* save coordinates */
	xold = two->x;
	yold = two->y;

	do
	{
		PointerPosition(&x, &y, &mask);
		/*
		 * watch the correct slopes (line and vector slopes are
		 * different !)
		 */
		/* so, what object is selected ? line or vector ? */
		if (kind[currIndex] == 'L')
			valid_line_coords((int) two->h, (int) two->v, &x, &y);
		else
			valid_vector_coords((int) two->h, (int) two->v, &x, &y);

		if ((two->x != (float) x) || (two->y != (float) y))
		{	/* redraw necessary */


			draw_coords(pboard, 0, 0, 0);

			/* delete old */
			XDrawLine(disp, win, gc, (int) two->x, (int) two->y, (int) two->h, (int) two->v);
			/* draw new */
			two->x = (float) x;
			two->y = (float) y;
			XDrawLine(disp, win, gc, (int) two->x, (int) two->y, (int) two->h, (int) two->v);
		}	/* redraw */
	} while (mask & btn1);

	/* btn1 was released... */
	/* cancel, if pointer is outside of pboard */

	if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
	{	/* cancel */
		two->x = xold;
		two->y = yold;
		if (zoomed == True)
		{
			zoomed2real(&two->h, &two->v);
			zoomed2real(&two->x, &two->y);
		}
		return;
	}
	/* translate coordinates to the right mode */

	if (zoomed == True)
	{
		zoomed2real(&two->h, &two->v);
		zoomed2real(&two->x, &two->y);
	}
}/* edit_x_line */




void
edit_y_line()
{
	float           swap_x, swap_y;
	struct fig2    *two;


	/* swap x,y with h,v and call edit_x_line() */

	two = (struct fig2 *) obj[currIndex];

	swap_x = two->x;
	swap_y = two->y;

	two->x = two->h;
	two->y = two->v;

	two->h = swap_x;
	two->v = swap_y;

	edit_x_line();

}/* edit_y_line */



void
del_vector_marker(float x, float y, float h, float v)
{
	if (zoomed == True)
	{
		real2zoomed(&x, &y);
		real2zoomed(&h, &v);
	}
	draw_vector_marker(x, y, h, v);
}



void
edit_x_vector()
{
	struct fig2    *two;
	two = (struct fig2 *) obj[currIndex];

	del_vector_marker(two->x, two->y, two->h, two->v);

	edit_x_line();

	if (refr_auto == True)
		refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */

}



void
edit_y_vector()
{
	struct fig2    *two;
	float           x, y;
	two = (struct fig2 *) obj[currIndex];

	del_vector_marker(two->x, two->y, two->h, two->v);

	edit_y_line();

	/* swap coordinates */
	x = two->x;
	y = two->y;
	two->x = two->h;
	two->y = two->v;
	two->v = y;
	two->h = x;

	if (refr_auto == True)
		refresh(pboard, &question_mark, (caddr_t)DUMMY);	/* wipe screen */


}






/* --==--==--==--==--==--==--==--==--==-- */
void
set_pick_erase(void)
{
	/* Override Translation Manager */
	XtTranslations  trans_table;
	char            destination[80] = "<Btn1Down>: pick_erase()";
	static XtActionsRec actions[80] = {{"pick_erase", pick_erase}};
	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

}



void
pick_erase(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	/* pointer button one was pressed */


	Display        *disp;
	Window          win;
	unsigned int    mask, mask2;
	int             h, v, x, y;
	Position        x_rel, y_rel, xx, yy;
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	PointerPosition(&x, &y, &mask);

	disp = XtDisplay(pboard);
	win = XtWindow(pboard);

	XtUninstallTranslations(pboard);

	set_pick_stack(x, y);

	while (True)
	{
		h = nextObject();

		if (h == -1)
		{
			left(0, 0, 0, 0);
			leave_pick("No valid object found !");
			return;
		}
		prepare_top_stack_object();

		headline(toplevel, "Actions: press Button 2 to ERASE object ----- press Button 3 for MORE");

		do
		{	/* listen closely to button number 2! If pressed ->
			 * erase object */
			PointerPosition(&x, &y, &mask);
			if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
			{
				left(0, 0, 0, 0);
				prepare_top_stack_object();
				return;
			}
		} while ((!(mask & btn3)) && (!(mask & btn2)));

		do
		{
			PointerPosition(&x, &y, &mask2);
			if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
			{
				left(0, 0, 0, 0);
				prepare_top_stack_object();
				return;
			}
		} while (mask == mask2);


		prepare_top_stack_object();

		if (mask & 512)
			erase_current_object();	/* yes, erase this object */

		/* now work with the next object on the pick-stack */
	}

}


void
set_dash_mode()
{
	XGCValues       gcvals;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	XSetLineAttributes(disp, gc, 0, LineDoubleDash, CapButt, JoinMiter);

	gcvals.dashes = 12;
	XChangeGC(disp, gc, GCDashList, &gcvals);
}



void
set_normal_mode()
{
	XGCValues       gcvals;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);

	gcvals.dashes = 5;
	XChangeGC(disp, gc, GCDashList, &gcvals);
}



void
prepare_top_stack_object()
{
	struct fig1    *one;
	struct fig2    *two;
	struct fig3    *three;
	struct fig4    *four;
	struct fig5   *five;
	struct fig6    *six;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	float           x, y, h, v, a, b;
	int             rad;



	switch (kind[currIndex])
	{
	case 'I':
		six = (struct fig6 *) obj[currIndex];
		x = six->ax;
		y = six->ay;
		h = six->ex;
		v = six->ey;
		a = six->sx;
		b = six->sy;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
			real2zoomed(&a, &b);
		}
		XDrawArc(disp, win, gc, (int) x - points, (int) y - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		XDrawArc(disp, win, gc, (int) a - points, (int) b - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		XDrawArc(disp, win, gc, (int) h - points, (int) v - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		set_dash_mode();
		XDrawLine(disp,win,gc,(int)a,(int)b,(int)h,(int)v);
		XDrawLine(disp,win,gc,(int)h,(int)v,(int)x,(int)y);
		XDrawLine(disp,win,gc,(int)x,(int)y,(int)a,(int)b);
		set_normal_mode();
		break;

	case 'L':
		two = (struct fig2 *) obj[currIndex];
		x = two->x;
		y = two->y;
		h = two->h;
		v = two->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		set_dash_mode();
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		set_normal_mode();
		XDrawArc(disp, win, gc, (int) x - points, (int) y - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		XDrawArc(disp, win, gc, (int) h - points, (int) v - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		break;

	case 'V':
		two = (struct fig2 *) obj[currIndex];
		x = two->x;
		y = two->y;
		h = two->h;
		v = two->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		set_dash_mode();
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		set_normal_mode();
		XDrawArc(disp, win, gc, (int) x - points, (int) y - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		XDrawArc(disp, win, gc, (int) h - points, (int) v - points,
			 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
		break;


	case 'N':
		three = (struct fig3 *) obj[currIndex];
		x = three->x;
		y = three->y;
		h = three->h;
		v = three->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		set_dash_mode();
		XDrawRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		XDrawLine(disp, win, gc, (int) x, (int) v, (int) h, (int) y);
		set_normal_mode();
		draw_edit_marker(x, y, h, v);
		break;

	case 'D':
		four = (struct fig4 *) obj[currIndex];
		x = four->x;
		y = four->y;
		h = four->h;
		v = four->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		x -= 1;
		y -= 1;
		h += 1;
		v += 1;
		set_dash_mode();
		XDrawRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		XDrawLine(disp, win, gc, (int) x, (int) v, (int) h, (int) y);
		set_normal_mode();
		draw_edit_marker(x, y, h, v);
		break;

	case 'F':
		two = (struct fig2 *) obj[currIndex];
		x = two->x;
		y = two->y;
		h = two->h;
		v = two->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		set_dash_mode();
		XDrawRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		XDrawLine(disp, win, gc, (int) x, (int) v, (int) h, (int) y);
		set_normal_mode();
		draw_edit_marker(x, y, h, v);
		break;

	case 'C':
		two = (struct fig2 *) obj[currIndex];
		x = two->x;
		y = two->y;
		rad = two->radius;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			rad *= 10;
		}
		set_dash_mode();
		XDrawArc(disp, win, gc, (int) x - rad, (int) y - rad,
			 (unsigned int) (2 * rad), (unsigned int) (2 * rad), 0, 360 * 64);
		set_normal_mode();

		break;

	case 'B':
		two = (struct fig2 *) obj[currIndex];
		x = two->x;
		y = two->y;
		rad = two->radius;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			rad *= 10;
		}
		rad += 5;
		set_dash_mode();
		XDrawArc(disp, win, gc, (int) x - rad, (int) y - rad,
			 (unsigned int) (2 * rad), (unsigned int) (2 * rad), 0, 360 * 64);
		set_normal_mode();
		break;
/* rel13 */
	case 'T':
		five = (struct fig5 *) obj[currIndex];
		x = five->x;
		y = five->y;
		rad = 15;
		if (zoomed == True) real2zoomed(&x, &y);
		XDrawArc(disp, win, gc, (int) x-rad, (int) y-rad ,(unsigned int) (2 * rad), (unsigned int) (2 * rad), 0, 360 * 64);
		
		break;

	case 'O':
		one = (struct fig1 *) obj[currIndex];
		x = one->x;
		y = one->y;
		h = one->h;
		v = one->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		set_dash_mode();
		DrawOval((int) x, (int) y, (int) h, (int) v);
		set_normal_mode();
		draw_edit_marker(x, y, h, v);
		break;

	default:
		break;
	}	/* switch */


	set_normal_mode();

}



void
draw_edit_marker(float x, float y, float h, float v)
{
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	XDrawArc(disp, win, gc, (int) x - points, (int) y - points,
		 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
	XDrawArc(disp, win, gc, (int) h - points, (int) v - points,
		 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
	XDrawArc(disp, win, gc, (int) x - points, (int) v - points,
		 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
	XDrawArc(disp, win, gc, (int) h - points, (int) y - points,
		 (unsigned int) (2 * points), (unsigned int) (2 * points), 0, 360 * 64);
}



void
erase_current_object()
{
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	struct fig1    *one;
	struct fig2    *two;
	struct fig3    *three;
	struct fig4    *four;
	struct fig5    *five;
	struct fig6    *six;
	float           x, y, h, v;
	int             rad;


	/* the object with index currIndex is to be erased */

	switch (kind[currIndex])
	{
	case 'I':
		bezier_marker = (struct fig6 *) obj[currIndex];
		/* redraw object */
		if (zoomed == True) real2zoomed(&bezier_marker->sx, &bezier_marker->sy);
		
		DrawBezier(bezier_marker->ax, bezier_marker->ay,
			   bezier_marker->ex, bezier_marker->ey,
			   bezier_marker->sx, bezier_marker->sy);

		if (bezier_start == obj[currIndex])
		{	/* first entry */
			bezier_marker = (struct fig6 *) obj[currIndex];

			if (bezier_curr == bezier_start)
			{	/* exactly 1 entry */

				bezier_start = NULL;
				bezier_curr = NULL;

			} else
				bezier_start = bezier_start->next;

			free(bezier_marker);
		} else
		{
			bezier_marker = bezier_start;
			do
			{
				six = (struct fig6 *) bezier_marker;
				/* remember the last object */
				bezier_marker = bezier_marker->next;

			} while (bezier_marker != obj[currIndex]);
			/* <bezier_marker> points to the desired object and */
			/* <six> to the object before */

			/* last entry ? */
			if (bezier_marker == bezier_curr)
			{
				bezier_marker = NULL;
				free(bezier_curr);
				bezier_curr = six;
			} else
				/* no! the object was somewhere in the middle */
			{
				six->next = bezier_marker->next;
				free(bezier_marker);
			}
		}	/* else & line */
		break;

	case 'L':
		strich_marker = (struct fig2 *) obj[currIndex];
		/* redraw object */
		x = strich_marker->x;
		y = strich_marker->y;
		h = strich_marker->h;
		v = strich_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);

		if (strich_start == obj[currIndex])
		{	/* first entry */
			strich_marker = (struct fig2 *) obj[currIndex];

			if (strich_curr == strich_start)
			{	/* exactly 1 entry */

				strich_start = NULL;
				strich_curr = NULL;

			} else
				strich_start = strich_start->next;

			free(strich_marker);
		} else
		{
			strich_marker = strich_start;
			do
			{
				two = (struct fig2 *) strich_marker;
				/* remember the last object */
				strich_marker = strich_marker->next;

			} while (strich_marker != obj[currIndex]);
			/* <strich_marker> points to the desired objects and */
			/* <two> to the object before */

			/* last entry ? */
			if (strich_marker == strich_curr)
			{
				strich_marker = NULL;
				free(strich_curr);
				strich_curr = two;
			} else
				/* no! the object was somewhere in the middle */
			{
				two->next = strich_marker->next;
				free(strich_marker);
			}
		}	/* else & line */
		break;

	case 'V':
		pfeil_marker = (struct fig2 *) obj[currIndex];
		/* redraw object */
		x = pfeil_marker->x;
		y = pfeil_marker->y;
		h = pfeil_marker->h;
		v = pfeil_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		XDrawLine(disp, win, gc, (int) x, (int) y, (int) h, (int) v);
		draw_vector_marker(x, y, h, v);

		if (pfeil_start == obj[currIndex])
		{	/* first entry */
			pfeil_marker = (struct fig2 *) obj[currIndex];

			if (pfeil_curr == pfeil_start)
			{	/* exactly 1 entry */

				pfeil_start = NULL;
				pfeil_curr = NULL;

			} else
				pfeil_start = pfeil_start->next;

			free(pfeil_marker);
		} else
		{
			pfeil_marker = pfeil_start;
			do
			{
				two = (struct fig2 *) pfeil_marker;
				/* remember the last object */
				pfeil_marker = pfeil_marker->next;

			} while (pfeil_marker != obj[currIndex]);
			/* <pfeil_marker> points to the desired objects and */
			/* <two> to the object before */

			/* last entry ? */
			if (pfeil_marker == pfeil_curr)
			{
				pfeil_marker = NULL;
				free(pfeil_curr);
				pfeil_curr = two;
			} else
				/* no! the object was somewhere in the middle */
			{
				two->next = pfeil_marker->next;
				free(pfeil_marker);
			}
		}	/* else & pfeil */
		break;

	case 'N':
		framedBox_marker = (struct fig3 *) obj[currIndex];
		/* redraw object */
		x = framedBox_marker->x;
		y = framedBox_marker->y;
		h = framedBox_marker->h;
		v = framedBox_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		XDrawRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));

		if (framedBox_start == obj[currIndex])
		{	/* first entry */
			framedBox_marker = (struct fig3 *) obj[currIndex];

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

			} while (framedBox_marker != obj[currIndex]);
			/*
			 * <framedBox_marker> points to the desired objects
			 * and
			 */
			/* <three> to the object before */

			/* last entry ? */
			if (framedBox_marker == framedBox_curr)
			{
				framedBox_marker = NULL;
				free(framedBox_curr);
				framedBox_curr = three;
			} else
				/* no! the object was somewhere in the middle */
			{
				three->next = framedBox_marker->next;
				free(framedBox_marker);
			}
		}	/* else & framedBox */
		break;

	case 'D':
		dashedBox_marker = (struct fig4 *) obj[currIndex];
		/* redraw object */
		x = dashedBox_marker->x;
		y = dashedBox_marker->y;
		h = dashedBox_marker->h;
		v = dashedBox_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
		XDrawRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));
		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);

		if (dashedBox_start == obj[currIndex])
		{	/* first entry */
			dashedBox_marker = (struct fig4 *) obj[currIndex];

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

			} while (dashedBox_marker != obj[currIndex]);
			/*
			 * <dashedBox_marker> points to the desired objects
			 * and
			 */
			/* <four> to the object before */

			/* last entry ? */
			if (dashedBox_marker == dashedBox_curr)
			{
				dashedBox_marker = NULL;
				free(dashedBox_curr);
				dashedBox_curr = four;
			} else
				/* no! the object was somewhere in the middle */
			{
				four->next = dashedBox_marker->next;
				free(dashedBox_marker);
			}
		}	/* else & dashedBox */
		break;

	case 'F':
		filledBox_marker = (struct fig2 *) obj[currIndex];
		/* redraw object */
		x = filledBox_marker->x;
		y = filledBox_marker->y;
		h = filledBox_marker->h;
		v = filledBox_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		XFillRectangle(disp, win, gc, (int) x, (int) y,
			    (unsigned int) (h - x), (unsigned int) (v - y));

		if (filledBox_start == obj[currIndex])
		{	/* first entry */
			filledBox_marker = (struct fig2 *) obj[currIndex];

			if (filledBox_curr == filledBox_start)
			{	/* exactly 1 entry */

				filledBox_start = NULL;
				filledBox_curr = NULL;

			} else
				filledBox_start = filledBox_start->next;

			free(filledBox_marker);
		} else
		{
			filledBox_marker = filledBox_start;
			do
			{
				two = (struct fig2 *) filledBox_marker;
				/* remember the last object */
				filledBox_marker = filledBox_marker->next;

			} while (filledBox_marker != obj[currIndex]);
			/*
			 * <filledBox_marker> points to the desired object
			 * and
			 */
			/* <two> to the object before */

			/* last entry ? */
			if (filledBox_marker == filledBox_curr)
			{
				filledBox_marker = NULL;
				free(filledBox_curr);
				filledBox_curr = two;
			} else
				/* no! the object was somewhere in the middle */
			{
				two->next = filledBox_marker->next;
				free(filledBox_marker);
			}
		}	/* else & filledBox */
		break;

	case 'C':
		kreis_marker = (struct fig2 *) obj[currIndex];
		/* redraw object */
		x = kreis_marker->x;
		y = kreis_marker->y;
		rad = kreis_marker->radius;

		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			rad *= 10;
		}
		XDrawArc(disp, win, gc, (int) (x - rad), (int) (y - rad),
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 360 * 64);

		if (kreis_start == obj[currIndex])
		{	/* first entry */
			kreis_marker = (struct fig2 *) obj[currIndex];

			if (kreis_curr == kreis_start)
			{	/* exactly 1 entry */

				kreis_start = NULL;
				kreis_curr = NULL;

			} else
				kreis_start = kreis_start->next;

			free(kreis_marker);
		} else
		{
			kreis_marker = kreis_start;
			do
			{
				two = (struct fig2 *) kreis_marker;
				/* remember the last object */
				kreis_marker = kreis_marker->next;

			} while (kreis_marker != obj[currIndex]);
			/* <kreis_marker> points to the desired object and */
			/* <two> to the object before */

			/* last entry ? */
			if (kreis_marker == kreis_curr)
			{
				kreis_marker = NULL;
				free(kreis_curr);
				kreis_curr = two;
			} else
				/* no! the object was somewhere in the middle */
			{
				two->next = kreis_marker->next;
				free(kreis_marker);
			}
		}	/* else & kreis */
		break;
		
	case 'T':
		message_marker = (struct fig5 *) obj[currIndex];
		/* redraw object */
		x = message_marker->x;
		y = message_marker->y;
		
		if (zoomed == True) 
		{
			real2zoomed(&x, &y);
			print_text_text_marker(message_marker->textpos,x,y,message_marker->text);
		}
		else XDrawString(disp, win, gc, x,y,message_marker->text,1);
			 
		if (message_start == obj[currIndex])
		{	/* first entry */
			message_marker = (struct fig5 *) obj[currIndex];

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

			} while (message_marker != obj[currIndex]);
			/* <message_marker> points to the desired object and */
			/* <five> to the object before */

			/* last entry ? */
			if (message_marker == message_curr)
			{
				message_marker = NULL;
				free(message_curr);
				message_curr = five;
			} else
				/* no! the object was somewhere in the middle */
			{
				five->next = message_marker->next;
				free(message_marker);
			}
		}	/* else & message */
		break;

	case 'B':
		disc_marker = (struct fig2 *) obj[currIndex];
		/* redraw object */
		x = disc_marker->x;
		y = disc_marker->y;
		rad = disc_marker->radius;

		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			rad *= 10;
		}
		XFillArc(disp, win, gc, (int) (x - rad), (int) (y - rad),
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 360 * 64);

		if (disc_start == obj[currIndex])
		{	/* first entry */
			disc_marker = (struct fig2 *) obj[currIndex];

			if (disc_curr == disc_start)
			{	/* exactly 1 entry */

				disc_start = NULL;
				disc_curr = NULL;

			} else
				disc_start = disc_start->next;

			free(disc_marker);
		} else
		{
			disc_marker = disc_start;
			do
			{
				two = (struct fig2 *) disc_marker;
				/* remember the last object */
				disc_marker = disc_marker->next;

			} while (disc_marker != obj[currIndex]);
			/* <disc_marker> points to the desired object and */
			/* <two> to the object before */

			/* last entry ? */
			if (disc_marker == disc_curr)
			{
				disc_marker = NULL;
				free(disc_curr);
				disc_curr = two;
			} else
				/* no! the object was somewhere in the middle */
			{
				two->next = disc_marker->next;
				free(disc_marker);
			}
		}	/* else & disc */
		break;

	case 'O':
		oval_marker = (struct fig1 *) obj[currIndex];
		/* redraw object */
		x = oval_marker->x;
		y = oval_marker->y;
		h = oval_marker->h;
		v = oval_marker->v;
		if (zoomed == True)
		{
			real2zoomed(&x, &y);
			real2zoomed(&h, &v);
		}
		DrawOval((int) x, (int) y, (int) h, (int) v);

		if (oval_start == obj[currIndex])
		{	/* first entry */
			oval_marker = (struct fig1 *) obj[currIndex];

			if (oval_curr == oval_start)
			{	/* exactly 1 entry */

				oval_start = NULL;
				oval_curr = NULL;

			} else
				oval_start = oval_start->next;

			free(oval_marker);
		} else
		{
			oval_marker = oval_start;
			do
			{
				one = (struct fig1 *) oval_marker;
				/* remember the last object */
				oval_marker = oval_marker->next;

			} while (oval_marker != obj[currIndex]);
			/* <oval_marker> points to the desired object and */
			/* <one> to the object before */

			/* last entry ? */
			if (oval_marker == oval_curr)
			{
				oval_marker = NULL;
				free(oval_curr);
				oval_curr = one;
			} else
				/* no! the object was somewhere in the middle */
			{
				one->next = oval_marker->next;
				free(oval_marker);
			}
		}	/* else & oval */
		break;



	}	/* switch */

}


void
init_no_objects_sign()
{
	XtTranslations  trans_table =
	XtParseTranslationTable("<LeaveNotify>	:	XtMenuPopdown() \n");
	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 250);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 250);
	n++;
	
	sign_up = XtCreatePopupShell("Report", transientShellWidgetClass, pboard, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 200);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 200);
	n++;
	XtSetArg(args[n], XtNborderWidth, (XtArgVal) 5);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNlabel, "No valid object found !");
	n++;
	
	sign_msg = XtCreateManagedWidget("dialogue", labelWidgetClass, sign_up, args, n);

	XtOverrideTranslations(sign_up, trans_table);

}



void
manage_copy(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{


	int             index;
	unsigned int    mask;




	PointerPosition(&x_pick, &y_pick, &mask);

	XtUninstallTranslations(pboard);

	set_pick_stack(x_pick, y_pick);	/* pick_stack is filled with matching
					 * objects */

	index = nextObject();

	if (index == -1)
	{	/* no objects found */
		left(0, 0, 0, 0);
		leave_pick("No valid object found !");
		return;
	} else
		copy_manager();
}



void
copy_manager()
{
	int             btn1 = 256, btn2 = 512, btn3 = 1024;
	int             index;
	int             x, y, h, v;
	unsigned int    mask, mask2;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	headline(toplevel, "Actions: hold Button 2 to MOVE  -----  press Button 3 for MORE");

	while (True)
	{

		prepare_top_stack_object();

		do
		{	/* watch for any buttonPress */
			PointerPosition(&x, &y, &mask);
			if ((x < x_A4_min) || (x > x_A4_max) || (y < y_A4_min) || (y > y_A4_max))
			{
				left(0, 0, 0, 0);
				prepare_top_stack_object();
				return;
			}
			draw_coords(pboard, 0, 0, 0);


		} while ((!(mask & btn3)) && (!(mask & btn2)));


		/* wait for btn3 release */
		do
			PointerPosition(&x, &y, &mask2);
		while (mask2 & btn3);



		prepare_top_stack_object();	/* redraw */



		if (mask & btn2)
			copy_it(x, y);
		else
		{	/* more */
			index = nextObject();

			if (index == -1)
			{	/* no objects found */
				left(0, 0, 0, 0);
				leave_pick("No valid object found !");
				return;
			}
		}

	}	/* while */
}




void
copy_it(int x, int y)
{
	/* min. 1 object exists ! */
	struct fig1    *one, *one_mark;
	struct fig2    *two, *two_mark;
	struct fig3    *three, *three_mark;
	struct fig4    *four, *four_mark;
	struct fig5    *five, *five_mark;
	struct fig6    *six, *six_mark;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	float           ax, ay, bx, by;
	int             res, rad;


	switch (kind[currIndex])
	{
	case 'I':
		six = (struct fig6 *) malloc(sizeof(bezier));
		/* copy struct */
		bezier_marker = (struct fig6 *) obj[currIndex];
		six->ax = bezier_marker->ax + 5.0;
		six->ay = bezier_marker->ay + 5.0;
		six->ex = bezier_marker->ex + 5.0;
		six->ey = bezier_marker->ey + 5.0;
		six->sx = bezier_marker->sx + 5.0;
		six->sy = bezier_marker->sy + 5.0;
		six->next = NULL;
		bezier_marker = bezier_curr;
		bezier_curr->next = six;
		obj[currIndex] = six;
		bezier_curr = bezier_curr->next;
		/* make a copy */
		ax = six->sx;
		ay = six->sy;
		if (zoomed == True) real2zoomed(&ax, &ay);
			
		DrawBezier(six->ax, six->ay,
			   six->ex, six->ey,
			   ax,ay);

		six_mark = bezier_marker;
		break;

	case 'L':
		two = (struct fig2 *) malloc(sizeof(strich));
		/* copy struct */
		strich_marker = (struct fig2 *) obj[currIndex];
		two->x = strich_marker->x + 5.0;
		two->y = strich_marker->y + 5.0;
		two->h = strich_marker->h + 5.0;
		two->v = strich_marker->v + 5.0;
		two->next = NULL;
		strich_marker = strich_curr;
		strich_curr->next = two;
		obj[currIndex] = two;
		strich_curr = strich_curr->next;
		/* make a copy */
		ax = two->x;
		ay = two->y;
		bx = two->h;
		by = two->v;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
		two_mark = strich_marker;
		break;

	case 'V':
		two = (struct fig2 *) malloc(sizeof(pfeil));
		/* copy struct */
		pfeil_marker = (struct fig2 *) obj[currIndex];
		two->x = pfeil_marker->x + 5.0;
		two->y = pfeil_marker->y + 5.0;
		two->h = pfeil_marker->h + 5.0;
		two->v = pfeil_marker->v + 5.0;
		two->next = NULL;
		pfeil_marker = pfeil_curr;
		pfeil_curr->next = two;
		obj[currIndex] = two;
		pfeil_curr = pfeil_curr->next;
		/* make a copy */
		ax = two->x;
		ay = two->y;
		bx = two->h;
		by = two->v;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		XDrawLine(disp, win, gc, (int) ax, (int) ay, (int) bx, (int) by);
		draw_vector_marker(ax, ay, bx, by);
		two_mark = pfeil_marker;

		break;

	case 'F':
		two = (struct fig2 *) malloc(sizeof(filledBox));
		/* copy struct */
		filledBox_marker = (struct fig2 *) obj[currIndex];
		two->x = filledBox_marker->x + 5.0;
		two->y = filledBox_marker->y + 5.0;
		two->h = filledBox_marker->h + 5.0;
		two->v = filledBox_marker->v + 5.0;
		two->next = NULL;
		filledBox_marker = filledBox_curr;
		filledBox_curr->next = two;
		obj[currIndex] = two;
		filledBox_curr = filledBox_curr->next;
		/* make a copy */
		ax = two->x;
		ay = two->y;
		bx = two->h;
		by = two->v;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		XFillRectangle(disp, win, gc, (int) ax, (int) ay,
			(unsigned int) (bx - ax), (unsigned int) (by - ay));
		two_mark = filledBox_marker;
		break;

	case 'C':
		two = (struct fig2 *) malloc(sizeof(kreis));
		/* copy struct */
		kreis_marker = (struct fig2 *) obj[currIndex];
		two->x = kreis_marker->x + 5.0;
		two->y = kreis_marker->y + 5.0;
		two->radius = kreis_marker->radius;
		two->next = NULL;
		rad = two->radius;
		kreis_marker = kreis_curr;
		kreis_curr->next = two;
		obj[currIndex] = two;
		kreis_curr = kreis_curr->next;
		/* make a copy */
		ax = two->x;
		ay = two->y;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			rad *= 10;
		}
		XDrawArc(disp, win, gc, (int) (ax - rad), (int) (ay - rad),
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 360 * 64);
		two_mark = kreis_marker;
		break;


	case 'B':
		two = (struct fig2 *) malloc(sizeof(disc));
		/* copy struct */
		disc_marker = (struct fig2 *) obj[currIndex];
		two->x = disc_marker->x + 5.0;
		two->y = disc_marker->y + 5.0;
		two->radius = disc_marker->radius;
		two->next = NULL;
		rad = two->radius;
		disc_marker = disc_curr;
		disc_curr->next = two;
		obj[currIndex] = two;
		disc_curr = disc_curr->next;
		/* make a copy */
		ax = two->x;
		ay = two->y;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			rad *= 10;
		}
		XFillArc(disp, win, gc, (int) (ax - rad), (int) (ay - rad),
			 (unsigned int) (rad + rad),
			 (unsigned int) (rad + rad), 0, 360 * 64);
		two_mark = disc_marker;
		break;
	case 'T': 
		five = (struct fig5 *) malloc(sizeof(message));
		/* copy struct */
		message_marker = (struct fig5 *) obj[currIndex];
		five->x = message_marker->x + 5.0;
		five->y = message_marker->y + 5.0;
		five->text = (char *) malloc(strlen(message_marker->text) + 1);
		strcpy(five->text,message_marker->text);
		strcpy(five->textpos,message_marker->textpos);
		five->next = NULL;
		message_marker = message_curr;
		message_curr->next = five;
		obj[currIndex] = five;
		message_curr = message_curr->next;
		/* make a copy */
		ax = five->x;
		ay = five->y;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			print_text_text_marker(five->textpos,ax,ay,five->text);
		} else XDrawString(disp, win, gc, ax,ay,five->text,1);
		five_mark = message_marker;
		break;

	case 'N':
		three = (struct fig3 *) malloc(sizeof(framedBox));
		/* copy struct */
		framedBox_marker = (struct fig3 *) obj[currIndex];
		three->x = framedBox_marker->x + 5.0;
		three->y = framedBox_marker->y + 5.0;
		three->h = framedBox_marker->h + 5.0;
		three->v = framedBox_marker->v + 5.0;
		three->next = NULL;
		framedBox_marker = framedBox_curr;
		framedBox_curr->next = three;
		obj[currIndex] = three;
		framedBox_curr = framedBox_curr->next;
		/* make a copy */
		ax = three->x;
		ay = three->y;
		bx = three->h;
		by = three->v;
		three->text = (char *) malloc(strlen(framedBox_marker->text) + 1);
		strcpy(three->text, framedBox_marker->text);
		strcpy(three->textpos, framedBox_marker->textpos);
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
			(unsigned int) (bx - ax), (unsigned int) (by - ay));
		print_box_text(three->textpos, ax, ay, bx, by, three->text);
		three_mark = framedBox_marker;
		break;

	case 'D':
		four = (struct fig4 *) malloc(sizeof(dashedBox));
		/* copy struct */
		dashedBox_marker = (struct fig4 *) obj[currIndex];
		four->x = dashedBox_marker->x + 5.0;
		four->y = dashedBox_marker->y + 5.0;
		four->h = dashedBox_marker->h + 5.0;
		four->v = dashedBox_marker->v + 5.0;
		four->next = NULL;
		dashedBox_marker = dashedBox_curr;
		dashedBox_curr->next = four;
		obj[currIndex] = four;
		dashedBox_curr = dashedBox_curr->next;
		/* make a copy */
		ax = four->x;
		ay = four->y;
		bx = four->h;
		by = four->v;
		four->text = (char *) malloc(strlen(dashedBox_marker->text) + 1);
		strcpy(four->text, dashedBox_marker->text);
		strcpy(four->textpos, dashedBox_marker->textpos);
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		XSetLineAttributes(disp, gc, 0, LineOnOffDash, CapButt, JoinMiter);
		XDrawRectangle(disp, win, gc, (int) ax, (int) ay,
			(unsigned int) (bx - ax), (unsigned int) (by - ay));
		XSetLineAttributes(disp, gc, 0, LineSolid, CapButt, JoinMiter);
		print_box_text(four->textpos, ax, ay, bx, by, four->text);
		four_mark = dashedBox_marker;
		break;

	case 'O':
		one = (struct fig1 *) malloc(sizeof(oval));
		/* copy struct */
		oval_marker = (struct fig1 *) obj[currIndex];
		one->x = oval_marker->x + 5.0;
		one->y = oval_marker->y + 5.0;
		one->h = oval_marker->h + 5.0;
		one->v = oval_marker->v + 5.0;
		one->next = NULL;
		oval_marker = oval_curr;
		oval_curr->next = one;
		obj[currIndex] = one;
		oval_curr = oval_curr->next;
		/* make a copy */
		ax = one->x;
		ay = one->y;
		bx = one->h;
		by = one->v;
		if (zoomed == True)
		{
			real2zoomed(&ax, &ay);
			real2zoomed(&bx, &by);
		}
		DrawOval((int) ax, (int) ay, (int) bx, (int) by);
		one_mark = oval_marker;
		break;

	default:
		break;

	}	/* switch */



	res = pick_move(x, y);




	if (res == -1)
	{	/* cancel */
		switch (kind[currIndex])
		{
		case 'I':
			bezier_curr = six_mark;
			bezier_curr->next = NULL;
			free(six);
			break;

		case 'L':
			strich_curr = two_mark;
			strich_curr->next = NULL;
			free(two);
			break;

		case 'V':
			pfeil_curr = two_mark;
			pfeil_curr->next = NULL;
			free(two);
			break;

		case 'B':
			disc_curr = two_mark;
			disc_curr->next = NULL;
			free(two);
			break;

		case 'C':
			kreis_curr = two_mark;
			kreis_curr->next = NULL;
			free(two);
			break;
			
		case 'T':
			message_curr = five_mark;
			message_curr->next = NULL;
			free(five);
			break;

		case 'F':
			filledBox_curr = two_mark;
			filledBox_curr->next = NULL;
			free(two);
			break;

		case 'O':
			oval_curr = one_mark;
			oval_curr->next = NULL;
			free(one);
			break;

		case 'N':
			framedBox_curr = three_mark;
			framedBox_curr->next = NULL;
			free(three);
			break;

		case 'D':
			dashedBox_curr = four_mark;
			dashedBox_curr->next = NULL;
			free(four);
			break;

		default:
			break;

		}	/* switch */
		refresh(pboard, &question_mark, (caddr_t)DUMMY);
	}	/* cancel */
}/* copy_it */
