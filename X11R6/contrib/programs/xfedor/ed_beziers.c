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
/* ed_beziers.c */
#include <stdio.h>
#include <math.h>             
#include "couche.h"	/* myEvent */

#define round(f) ((((int)((f)*100)-((int)(f)*100))<50)?((int)(f)):((int)(f)+1))

static tracercourbe(x0,y0,x1,y1,x2,y2,x3,y3)
	int x0,y0,x1,y1,x2,y2,x3,y3 ;

{
 float t;
 float unmoinst;
 float t2,t3;
 float nbp; /*nombre de Points a calculer */
 float lepas;
 float pfx,pfy;

 nbp = (float) (
       sqrt((float)((x1 - x2)*(x1 - x2) +
   		    (y1 - y2)*(y1 - y2)))+
       sqrt((float)((x3 - x2)*(x3 - x2) + 
		    (y3 - y2)*(y3 - y2)))+
       sqrt((float)((x1 - x0)*(x1 - x0) +
   		    (y1 - y0)*(y1 - y0)))) ;
 lepas = (float) 1 / nbp;
 for (t = 0; t <= 1; t += lepas)
   {unmoinst = 1-t;
    t2 = t*t;
    t3 = t2*t;
   pfx = ((unmoinst)*(unmoinst)*(unmoinst)* x0 + 
	    3*t*(unmoinst)*(unmoinst)* x1 + 
	    3*t2*(unmoinst)* x2 + t3* x3 );
   pfy = ((unmoinst)*(unmoinst)*(unmoinst)* y0 + 
	  3*t*(unmoinst)*(unmoinst)* y1 + 
	  3*t2*(unmoinst)* y2 + t3* y3 );
   Adjpoint(round(pfx),round(pfy));
   }
   Aff_all() ;
   CarFen();
}


Autom_bez (pev)
	myEvent * pev ; 
{   
	static numpoint ;
	static int x0,y0,x1,y1,x2,y2,x3,y3 ;

    switch (pev->type) {
	case EnterZone : Afficher_boutons("POINT","POINT");
			 numpoint = 1 ;
			 break ;
	case ButtonPressed : switch (numpoint) {
				case 1 : x0 = convert(pev->x) ;
					 y0 = convert(pev->y) ;
					 numpoint ++ ;
					 Invpoint(x0,y0) ;
					 break ;
				case 2 : x1 = convert(pev->x) ;
					 y1 = convert(pev->y) ;
					 numpoint ++ ;
					 Invpoint(x1,y1);
					 break ;
				case 3 : x2 = convert(pev->x) ;
					 y2 = convert(pev->y) ;
					 numpoint ++ ;
					 Invpoint(x2,y2) ;
					 break ;
				case 4 : x3 = convert(pev->x) ;
					 y3 = convert(pev->y) ;
					 Invpoint(x0,y0) ;
					 Invpoint(x1,y1) ;
					 Invpoint(x2,y2) ;
					 Dodo();
					 tracercourbe(x0,y0,x1,y1,
							x2,y2,x3,y3);
					 numpoint = 1 ;
					 break ;
			     }
			     break ;
	case ButtonReleased : break ;
	case MoveMouse : break ;				
	case LeaveZone : Afficher_boutons("","");
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 switch (numpoint) {
				case 2 : Invpoint(x0,y0) ; break ;
				case 3 : Invpoint(x0,y0) ;
					 Invpoint(x1,y1) ; break ;
				case 4 : Invpoint(x0,y0) ;
					 Invpoint(x1,y1) ;
					 Invpoint(x2,y2) ; break ;
				default : break ;
			 }
			 break ;
    }
} 
