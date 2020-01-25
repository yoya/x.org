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
/* ed_metric.c */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "clientimage.h"	/* ClientImage pour fedor.h */
#include "fedor.h"	/* fedchar */

extern fedchar cartrav ;
extern Xleft,Baseliney ;

int MetrAff = 0 ;

Autom_metric(pev)
	myEvent * pev ; 
{  
	 int dhadj ;

    switch (pev->type) {
	case EnterZone : Afficher_boutons("HADJ","HINCR");
			 BiBox();
			 Invpoint(Xleft - cartrav.hadj, Baseliney);
			 Invpoint(Xleft - cartrav.hadj +cartrav.hincr , 
				  Baseliney);
			 MetrAff = 1 ;
			 break ;
	case ButtonPressed : 
			     if (pev->click == 1) {
				 Invpoint(Xleft - cartrav.hadj , Baseliney);
				 dhadj = cartrav.hadj ;
				 cartrav.hadj = Xleft -	convert(pev->x) ;
				 cartrav.hincr += (cartrav.hadj - dhadj) ;
				 Invpoint(Xleft - cartrav.hadj , Baseliney);
			     } else
			     if (pev->click == 2) {
			 	 Invpoint(Xleft -cartrav.hadj+cartrav.hincr , 
					  Baseliney);
				 cartrav.hincr = convert(pev->x) -
						Xleft + cartrav.hadj;
			 	 Invpoint(Xleft -cartrav.hadj+cartrav.hincr , 
					  Baseliney);
			     } 	
			     Aff_all() ;
			     break ;
	case ButtonReleased : break ;
	case MoveMouse : break ;				
	case LeaveZone : Afficher_boutons("","");
	          	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 BiBox();
			 Invpoint(Xleft - cartrav.hadj, Baseliney);
			 Invpoint(Xleft - cartrav.hadj + cartrav.hincr , 
				  Baseliney);
			 MetrAff = 0 ;
			 break ;
    }
} 
