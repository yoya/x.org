/*    Copyright 1989-94 Daniel Dardailler.
Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Daniel Dardailler not
be used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  Daniel
Dardailler makes no representations about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.  
*/
/* ed_cercle.c */
#include <stdio.h>
#include <math.h>		/* sqrt */
#include "couche.h"		/* myEvent */
#include "style.h"		/* NORMAL .. */

static CirclePoint(x,y,dx,dy)
	int x,y,dx,dy ;
{
 	Adjpoint(x + dx, y + dy);  
	Adjpoint(x - dx, y - dy);  
	Adjpoint(x - dy, y + dx);  
	Adjpoint(x + dy, y - dx);  
}

static CircleLine(x1,x2,y)
	int x1,x2,y;
{
	register int i ;
	
	for (i= x1 ; i<= x2 ; i++) Adjpoint(i,y);

}

static CarCercle(x, y, r, click)
	int             x, y, r;
	int             click;
{
	register int E,px,py,oldpx ;

    px  =  r ;
    py = 1 ;
    E = 0 ;
    if (click == 1) {	/* contour */
	CirclePoint(x,y,r,0);
	while (px > py) {
		CirclePoint(x,y,px,py);
	  	CirclePoint(x,y,py,px);
	  	py++ ;
	  	if ( E <= 0 ) E += (py << 1) + 1 ; 
			else { px -- ; E += (py - px + 1) << 1 ;}
	}
	if (px == py) CirclePoint(x,y,px,py);
    } else { 		/* plein */
	CircleLine(x-r,x+r,y);
	oldpx = 0 ;
	while (px > py) {
		CircleLine(x-px,x+px,y+py);
		CircleLine(x-px,x+px,y-py);
		if (px != oldpx) {
			CircleLine(x-py,x+py,y+px);
			CircleLine(x-py,x+py,y-px);
			oldpx = px ;
		}
	  	py++ ;
	  	if ( E <= 0 ) E += (py << 1) + 1 ; 
			else { px -- ; E += (py - px + 1) << 1 ;}
	}
	if (px == py) {
		CircleLine(x-px,x+px,y+py);
		CircleLine(x-px,x+px,y-py);
	}
    }
    Aff_all() ;
    CarFen() ;
}


Autom_cercle(pev)
	myEvent *        pev;
{
	static int      x, y;

	switch (pev->type) {
	case EnterZone:
		Afficher_boutons("OUTSIDE", "FILLED");
		x = -1;
		break;
	case ButtonPressed:
		if (x == -1) {
			Afficher_boutons("RADIUS", "RADIUS");
			x = pev->x;
			y = pev->y;
			stylesouris(LIGNE, x, y);
		}
		break;
	case ButtonReleased:
		stylesouris(NORMAL, 0, 0);
		Afficher_boutons("OUTSIDE", "FILLED");
		if (x != -1) {
			int             r;
			Dodo();
			r = (int) sqrt((double)
				       ((x - pev->x) * (x - pev->x) +
					(y - pev->y) * (y - pev->y)));
			CarCercle(convert(x), convert(y),convert(r), pev->click);
		}
		x = -1;
		break;
	case MoveMouse:
		break;
	case LeaveZone:
		stylesouris(NORMAL, 0, 0);
		Afficher_numview(-1, -1);	/* restaure le gris */
		Afficher_boutons("", "");
		break;
	}
}
