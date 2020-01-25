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
/* ed_resize.c */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "style.h"	/* NORMAL .. */
#include "clientimage.h"	/* ClientImage pour fedor.h */
#include "fedor.h"	/* fedchar */

extern   editresol;
extern   Xleft, Baseliney ;
extern   fedchar cartrav ;
extern   ClientImage * bitmapsave ;
extern   BackColor ;

static Resize(xorig,yorig,xfin,yfin,click)
	int xorig,yorig,xfin,yfin ;
	int click ;
{	
   int x1,y1,x2,y2 ;
   int nouvx,nouvy ;
   int x, y ;
   int color ;
   int ox,oy,xlim,ylim;
   int bbw, bbh, newbbw, newbbh ;
   int flag_modify = 0 ;

   /* make the box coordinates be ordered */
   x1 = Min(xorig,xfin) ; 
   y1 = Min(yorig,yfin) ;
   x2 = Max(xorig,xfin) ;
   y2 = Max(yorig,yfin) ;

   x2 ++ ; y2++ ;

   if (click == 2) {      /* define the new bbox */
      if (Baseliney + cartrav.up < y1) {
	flag_modify = 1 ;
	for (y = Baseliney + cartrav.up; y < y1; y++) 
	  Rast_Op(cartrav.image,&cartrav.image,
		  Xleft,y,Xleft,y,cartrav.hsize,1,
		  VIDXOR,BackColor);
      }
      if (Baseliney + cartrav.down > y2) {
	flag_modify = 1 ;
	for (y = Baseliney + cartrav.down; y >= y2; y--)
	  Rast_Op(cartrav.image,&cartrav.image,
		  Xleft,y,Xleft,y,cartrav.hsize,1,
		  VIDXOR,BackColor);
     }
      if (Xleft < x1) {
	flag_modify = 1 ;
	for (x = Xleft; x < x1; x++)
	  Rast_Op(cartrav.image,&cartrav.image,
		  x,Baseliney + cartrav.up,x,Baseliney + cartrav.up,
		  1,cartrav.down-cartrav.up,
		  VIDXOR,BackColor);
     }
      if (Xleft + cartrav.hsize > x2) {
	flag_modify = 1 ;
	for (x = Xleft+cartrav.hsize; x >= x2; x--)
	  Rast_Op(cartrav.image,&cartrav.image,
		  x,Baseliney + cartrav.up,x,Baseliney + cartrav.up,
		  1,cartrav.down-cartrav.up,
		  VIDXOR,BackColor);
     }
      Xleft = x1 ;
      cartrav.hsize = x2 - x1 ;
      cartrav.up = y1 - Baseliney ;
      cartrav.down = y2 - Baseliney ;
      if (flag_modify == 0) Aff_all() ; else  MontrerCarTrav() ;
   } else {               /* rescale the actual fedchar */
      Rast_Off(&bitmapsave,BackColor) ;

      bbw = cartrav.hsize ;
      newbbw = x2 - x1;
      bbh = (cartrav.down - cartrav.up) ;
      newbbh = y2 - y1;
  
      for ( y = 0; y < bbh; y ++ ) {
	 for ( x = 0; x < bbw; x ++ ) {
	    color = Rast_Inq(cartrav.image,Xleft+x,y+Baseliney + cartrav.up) ;
	    if (color != BackColor) {
	       ox = x*newbbw ;
	       oy = y*newbbh ;
	       xlim = how_many( ox+newbbw, bbw );
	       ylim = how_many( oy+newbbh, bbh );
	       for ( nouvy = oy/bbh; nouvy < ylim; nouvy++ ) {
		  for ( nouvx = ox/bbw; nouvx < xlim; nouvx++ ) {
		     Rast_Pix(bitmapsave,x1+nouvx,y1+nouvy,color);
		  }
	       }
	    }
	 }
      }
      Rast_Copy(bitmapsave,&cartrav.image);
      cartrav.hsize = newbbw ;
      Xleft = x1 ;
      cartrav.up = y1 - Baseliney ;
      cartrav.down = y2 - Baseliney ;
      MontrerCarTrav() ;
   }      
}

Autom_resize (pev)
	myEvent * pev ; 
{   static int x,y ;
	short           pas = (512 / editresol);
	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("SCALE","ADJUST");
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
			      Afficher_boutons("SCALE","ADJUST");
			      if (x != -1) {Dodo();
						Resize(convert(x),
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
