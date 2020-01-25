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
/* ed_fill.c */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "clientimage.h"	/* ClientImage dans fedor.h */
#include "fedor.h"	/* fedchar */
#include "edit.h" 	/* MODBLANC .. */

extern modcolor ;
extern fedchar cartrav ;
extern Xleft,Baseliney ;
extern BackColor, CurColor ;

#define empiler(a) T[i++] = a
#define depiler(a) a = T[--i]

static short T[128 * 64];

static remp(x,y,couleur,rempcouleur)
/* OLDFEDOR algorithm */
	int x,y ;
	int couleur;
	int rempcouleur;
{	
	int i, marqueH, marqueB;

  marqueH = marqueB = 0;
  i = 0; /* base de la pile */
E1:
  do  /* aller a gauche */
  { x -- ;}
  while ((couleur == Rast_Inq(cartrav.image,x,y)) &&
	 (x >= Xleft));
  x ++ ;
				
E2:
  y -- ; /* on marque le haut */
  if (y < Baseliney + cartrav.up) goto E4;
  if (couleur == Rast_Inq(cartrav.image,x,y)) 
	if ( ! marqueH) {
           empiler(x);
           empiler(y);
           marqueH = 1;
        } else marqueH = 0;
E4:
 y += 2; /* on marque le bas */
 if (y > Baseliney + cartrav.down) goto E3;
 if (couleur == Rast_Inq(cartrav.image,x,y)) 
	if ( ! marqueB) {
	   empiler(x);
           empiler(y);
           marqueB = 1;
          } else marqueB = 0;
E3:
  y -- ; /* on revient au Point initial */
  Rast_Pix(cartrav.image,x,y,rempcouleur);
  carre_pas(x,y,rempcouleur) ;
  x ++ ; /* on avance de 1 en x */
  if ((couleur == Rast_Inq(cartrav.image,x,y)) &&
	 (x <= Xleft + cartrav.hsize)) goto E2;
   else {
    if (i == 0) return;
    depiler(y);
    depiler(x);
    marqueH = 0;
    marqueB = 0;
    goto E1;
   }

}  

static Remplir(x,y)
	int x,y ;
{	
	int couleur ; 	/* couleur du point (x,y) */
        int rempcouleur ;  /* couleur de remplissage */

	if (cartrav.hsize==0) return ;

	/* si le point initial est hors bbox : exit */
    	if ((x < Xleft) || (x > Xleft+cartrav.hsize) || 
	    (y < Baseliney + cartrav.up) || 
	    (y > Baseliney + cartrav.down )) return ;

	rempcouleur = (modcolor==MODBACK)?BackColor:CurColor ;

	couleur = Rast_Inq(cartrav.image,x,y);
	if (modcolor==MODINV) {
	      if (couleur == BackColor) rempcouleur = CurColor ;
	                    else rempcouleur = BackColor ;
	    }

	/* si le point initial est de la couleur demandee : exit */
	if (rempcouleur == couleur ) return ;


	remp(x,y,couleur,rempcouleur) ;
	CarFen();
	
}
	
Autom_fill (pev)
	myEvent * pev ; 
{   
    int x,y ; 	/* coords courantes et prec en 0-128 */
	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("INSIDE","INSIDE");
			 break ;
	case ButtonPressed : 
			Dodo();
			Remplir(convert(pev->x),convert(pev->y));
			break ;
	case ButtonReleased : break ;
	case MoveMouse : break ;
	case LeaveZone : Afficher_boutons("","");
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 break ;
    }
} 
