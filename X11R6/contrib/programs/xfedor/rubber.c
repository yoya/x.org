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
/* rubber.c */
#include <stdio.h>
#include "couche.h"		/* myEvent */
#include "style.h"		/* NORMAL */

extern          editresol;
extern          nf_grille;

/* variables utilises pour gerer le stylesouris */
static int      StylCour;	/* style courant dans la fenetre edition */
static int      xAnCour, yAnCour, 
                xPrecPos, yPrecPos;	/* ancre et position
				         * precedente dans le
					 * repere de la fenetre */


int attente(pev)
     myEvent        *pev;	/* event de fenix */

{
  int             dx, dy;
  short           pas = (512 / editresol);

  w_getevent(pev);

  if ((pev->type == MoveMouse) && (pev->window == nf_grille)) {
    dx = convert(pev->x) * pas + pas/2;
    dy = convert(pev->y) * pas + pas/2;
    if ((dx != xPrecPos) || (dy != yPrecPos)) {
      Afficher_numview(convert(pev->x), convert(pev->y));
      figure(StylCour) ;
      xPrecPos = dx ;
      yPrecPos = dy ;
      figure(StylCour) ;
    }
   }
}


figure(style)
     int style ;
{
  switch(style) {
  case  LIGNE : 
    w_line(nf_grille, xAnCour, yAnCour, xPrecPos, yPrecPos, OFF);
    break ;
  case RECT :
    w_rect(nf_grille, xPrecPos, yPrecPos,
	   xPrecPos + xAnCour, yPrecPos + yAnCour, OFF);
    break ;
  case FIXRECT :
    w_rect(nf_grille, xAnCour, yAnCour,
	   xPrecPos, yPrecPos, OFF);
    break ;
  case FIXCIRC :
    w_ellips(nf_grille, xAnCour, yAnCour,
	   xPrecPos, yPrecPos, OFF);
    break ;
  }
}


/*--------------------------------------------------------------------*/

int
stylesouris(style, xancre, yancre)
/* style = NORMAL, ancre pour (LIGNE, FIXRECT, FIXCIRC) */
/* style = RECT, ancre = taille (positif ou pas) */
/* si on revient en style Normal, on efface la derniere trace */
/* sinon, on met a jour les variables globales de trace */
/* x et y ancre dans repere de la fenetre */
	int             style;
	int             xancre, yancre;
{
	short           pas = (512 / editresol);

	if (style == NORMAL) {
	  figure(StylCour) ;
	  xPrecPos = 0;
	  yPrecPos = 0;
	} else {
	  if (style != RECT) {
	    xPrecPos = xAnCour = convert(xancre) * pas + pas/2;
	    yPrecPos = yAnCour = convert(yancre) * pas + pas/2;
	  } else {
	    xAnCour = xancre;
	    yAnCour = yancre;
	    xPrecPos = 1000;
	    yPrecPos = 1000;
	    /* pour que le premier effacement soit inoperant */
	  }
	}
	StylCour = style;
}
