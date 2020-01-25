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
/* ed_rect.c */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "style.h"	/* NORMAL .. */

extern          editresol;

static CarRect(xorig,yorig,xfin,yfin,click)
	int xorig,yorig,xfin,yfin ;
	int click ;
{		
	register int bit,i,j,x1,y1,x2,y2;

	x1 = Min(xorig,xfin) ; 
	x2 = Max(xorig,xfin) ;
	y1 = Min(yorig,yfin) ; 
	y2 = Max(yorig,yfin) ;
	if (click==1) { 	/* contour */
	    for(i=x1; i<=x2 ; i++) Adjpoint(i,y1) ;
	    for(j=y1; j<=y2 ; j++) {
			Adjpoint(x1,j) ;
			Adjpoint(x2,j) ;
	    }
	    for(i=x1; i<=x2 ; i++) Adjpoint(i,y2) ;
	} else     /*  plein */
	    for(i=x1; i<=x2 ; i++) 
	       for(j=y1; j<=y2 ; j++) 	Adjpoint(i,j) ;
        CarFen();
	Aff_all();
}


Autom_rect (pev)
	myEvent * pev ; 
{   static int x,y ;
	short           pas = (512 / editresol);
	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("OUTSIDE","FILLED");
			 x = -1 ;
			 break ;
	case ButtonPressed : if (x == -1) {
				Afficher_boutons("ENDRECT","ENDRECT");
			     	x = pev->x ; 
			     	y = pev->y ; 
			     	stylesouris(FIXRECT,x,y);
			     }
			     break ;
	case ButtonReleased : stylesouris(NORMAL,0,0);
			      Afficher_boutons("OUTSIDE","FILLED");
			      if (x != -1) {Dodo();
						CarRect(convert(x),
						       convert(y),
						       convert(pev->x),
						       convert(pev->y),
						       pev->click);
			      }
			      x = -1 ;
			      break ;
	case MoveMouse : break ;				
	case LeaveZone : stylesouris(NORMAL,0,0);
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 Afficher_boutons("","");
			 break ;
    }
} 
