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
/* ed_transl.c */
#include <stdio.h>
#include "couche.h"
#include "clientimage.h"
#include "fedor.h"
#include "style.h"	/* NORMAL .. */

extern fedchar cartrav;
extern ClientImage * bitmapsave ;
extern Xleft,Baseliney ;
extern BackColor ;

Translate(xorig,yorig,xdest,ydest)
	int xorig,yorig,xdest,ydest ;
{
	int xdecal,ydecal ;

	if (cartrav.hsize==0) return ;

	Rast_Off(&bitmapsave,BackColor) ;
	xdecal = xdest - xorig ;
	ydecal = ydest - yorig ;
	if (Xleft+cartrav.hsize+xdecal >128) 
			xdecal = 128 - (Xleft+cartrav.hsize) ;
	if (Xleft+xdecal<0) xdecal = (- Xleft) ;
	if (Baseliney+cartrav.down+ydecal>128)
			ydecal = 128 - (Baseliney+cartrav.down) ;
	if (Baseliney+cartrav.up+ydecal <0) 
			ydecal = (-Baseliney-cartrav.up) ;

	Rast_Op(cartrav.image,&bitmapsave,
				Xleft,Baseliney+cartrav.up,
				Xleft+xdecal,Baseliney+cartrav.up+ydecal,
				cartrav.hsize,cartrav.down-cartrav.up,
				VIDSTR,BackColor);
	Rast_Copy(bitmapsave,&cartrav.image);

	Xleft += xdecal ;
	cartrav.down += ydecal ;
	cartrav.up += ydecal ;

	MontrerCarTrav() ;
}

Autom_transl (pev)
	myEvent * pev ; 
{  static int x,y ;
 	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("ORIGIN","ORIGIN");
			 x = -1 ;
			 break ;
	case ButtonPressed : if (x == -1 ) {
				Afficher_boutons("DEST","DEST");
			    	x = pev->x ; 
			    	y = pev->y ; 
			     	stylesouris(LIGNE,x,y);
			     }
			     break ;
	case ButtonReleased : stylesouris(NORMAL,0,0);
			      Afficher_boutons("ORIGIN","ORIGIN");
			      if (x != -1) {Dodo();
						Translate(convert(x),
						       convert(y),
						       convert(pev->x),
						       convert(pev->y));
			      }
			      x = -1 ;
			      break ;
	case MoveMouse : break ;				
	case LeaveZone : stylesouris(NORMAL,0);
	  		 Afficher_numview(-1,-1) ; /* restaure le gris */
			 Afficher_boutons("","");
			 break ;
    }
} 
