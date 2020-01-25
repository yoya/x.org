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
/* ed_cutap.c */
#include <stdio.h>
#include "couche.h"
#include "clientimage.h" 
#include "fedor.h"
#include "style.h"	/* NORMAL .. */

extern fedchar cartrav;
extern Xleft,Baseliney ;
extern editresol ;
extern ClientImage * bitmapsave ;
extern int nf_grille ;
extern BackColor ;

static TranslBox(click,x1,y1,x2,y2,x,y)
	int click ;
	int x1,y1,x2,y2,x,y ;
{
	int xhg,yhg,xbd,ybd,l,h,a,b ;
	int i,j , minx1x2, miny1y2 ;

	if (cartrav.hsize==0){
		gd_envoyermsg("NOTHING TO DO.");
		return ;
	}
	l = (x1-x2>0)?(x1-x2):(x2-x1) ;l++ ;
	h = (y1-y2>0)?(y1-y2):(y2-y1) ;h++ ;
	a = Min(x-(x2-x1),x);
	b = Min(y-(y2-y1),y);
	if (click == 1) /* Copy */
		Rast_Op(cartrav.image,&cartrav.image,
			Min(x1,x2),Min(y1,y2),
			a,b,l,h,VIDSTR,BackColor); else
	if (click == 2) { /* Move : on doit passer par une sauvegarde */
		Rast_Op(cartrav.image,&bitmapsave,
			Min(x1,x2),Min(y1,y2),
			0,0,
			l,h,VIDSTR,BackColor);
		minx1x2 = Min(x1,x2) ;
		miny1y2 = Min(y1,y2) ;
		for (i=minx1x2; i<minx1x2+l; i++)
		  for (j=miny1y2; j<miny1y2+h; j++)
		    Rast_Pix(cartrav.image,i,j,BackColor);
		Rast_Op(bitmapsave,&cartrav.image,
			0,0,a,b,l,h,VIDSTR,BackColor);
	}

	xhg = Min(Xleft,a);
	yhg = Min(Baseliney+cartrav.up,b);
	xbd = Max(Xleft+cartrav.hsize,Max(x-(x2-x1),x));
	ybd = Max(Baseliney+cartrav.down,Max(y-(y2-y1),y));

	/* on recalcule la bbox */
	while ( Colonne_color(cartrav.image,xhg,yhg,ybd+1,BackColor)) xhg++ ;
	Xleft = xhg ;
	while ( Colonne_color(cartrav.image,xbd,yhg,ybd+1,BackColor)) xbd-- ;
	cartrav.hsize = xbd - Xleft +1 ;
	if (cartrav.hsize > 128) cartrav.hsize = 128 ; /* Hack */
	while ( Ligne_color(cartrav.image,yhg,Xleft,
			    Xleft+cartrav.hsize,BackColor) ) yhg++ ;
	cartrav.up = yhg - Baseliney;

	while ( Ligne_color(cartrav.image,ybd,Xleft,
			    Xleft+cartrav.hsize,BackColor) ) ybd-- ; 
	cartrav.down = ybd - Baseliney + 1 ;
	if (cartrav.down - cartrav.up > 128) cartrav.down = 128 + cartrav.up; /* Hack */

	MontrerCarTrav() ;
}

Autom_cutap (pev)
	myEvent * pev ; 
{  static int x,y,a,b,flag ;	/* flag sert pour 2 choses : savoir si
				un bouton est deja enfonce (1) et savoir si 
				c'est la premiere fois ou pas (0,2)*/
   short pas = (512/editresol) ; 	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("DefBox","DefBox");
			 flag = 0 ;
			 break ;
	case ButtonPressed : if (flag == 0 ) {
				Afficher_boutons("EndBox","EndBox");
			    	x = pev->x ;
			    	y = pev->y ;
			     	stylesouris(FIXRECT,x,y);
				flag = 1 ;
			     } 
			     break ;
	case ButtonReleased : if (flag == 1) {
				stylesouris(NORMAL,0,0);
	        		a = pev->x ;
			        b = pev->y ;
			      	stylesouris(RECT,
					(convert(x)-convert(a))*pas,
					(convert(y)-convert(b))*pas);
				Afficher_boutons("Copy","Move");
			      	flag = 2 ;
			      } else
			      if (flag == 2) {
				stylesouris(NORMAL,0,0);
				flag = 0 ;
				Afficher_boutons("DefBox","DefBox");
				Dodo() ;
				TranslBox(pev->click,
					convert(x),convert(y),
					convert(a),convert(b),
					convert(pev->x),
					convert(pev->y));
			      }
			      break ;
	case MoveMouse : break ;				
	case LeaveZone : stylesouris(NORMAL,0);
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 Afficher_boutons("","");
			 break ;
    }
} 
