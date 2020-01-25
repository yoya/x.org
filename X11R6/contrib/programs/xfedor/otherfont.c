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
/* otherfont.c */
#include <stdio.h>
#include "clientimage.h"  /* VIDSTR ,ClientImage.. */
#include "fedor.h"	  /* fedchar .. MAXFONTCARD */
#include "math.h"	  /* sqrt, ceil */
#include "couche.h"       /* how_many */

extern int First, Last ;
extern fedchar chartab[MAXFONTCARD] ;

extern int nf_garb ;	/* pour l'animation */
extern ClientImage * bitmapsave ;  /* Bold,Scale et Slant */
extern int BackColor, CurColor ;

Scale(numx,denx,numy,deny)
	int numx,denx,numy,deny ;
{
   int nouvx,nouvy ;
   int x, y ;
   int color ;
   int ox,oy,xlim,ylim;
   int bbw, bbh, newbbw, newbbh ;
   int code ;

  for (code=First; (code<=Last) && (code < MAXFONTCARD); code++) 
    if (chartab[code].hsize>0)
      {
	Rast_Off(&bitmapsave,BackColor) ;
	bbw = chartab[code].hsize ;
	newbbw = how_many( bbw * numx, denx );
	bbh = (chartab[code].down - chartab[code].up) ;
	newbbh = how_many( bbh * numy, deny );
  
	if (newbbw > 128) return ;
	if (newbbh > 128) return ;

	for ( y = 0; y < bbh; y ++ ) {
	   for ( x = 0; x < bbw; x ++ ) {
	      color = Rast_Inq(chartab[code].image,x,y) ;
	      if (color != 0) {     /* Hack 1n n1 */
		 ox = x*newbbw ;
		 oy = y*newbbh ;
		 xlim = how_many( ox+newbbw, bbw );
		 ylim = how_many( oy+newbbh, bbh );
		 for ( nouvy = oy/bbh; nouvy < ylim; nouvy++ ) {
		    for ( nouvx = ox/bbw; nouvx < xlim; nouvx++ ) {
		       Rast_Pix(bitmapsave,nouvx,nouvy,CurColor);
		    }
		 }
	      }
	   }
	}
	chartab[code].hsize = newbbw ;
	chartab[code].down += (newbbh - bbh) ;
	chartab[code].hincr += (newbbw - bbw) ;

	Rast_Free(&chartab[code].image) ;
	Rast_Init(&chartab[code].image,
		  chartab[code].hsize,
		  chartab[code].down-chartab[code].up,0);

	Rast_Op_n1(bitmapsave,&chartab[code].image,0,0,
		0,0,
		chartab[code].hsize,
		chartab[code].down-chartab[code].up,
		VIDSTR,BackColor);
      }
  ShowDoIt(First,Last);
}

Slant()
{
	register int h,		/* hauteur du car */
		     n,ni,	/* nb de decalages */
		     b,		/* hauteur de base d'un decalage */
		     r ;	/* surplus a repartir */
		     
	int code, dec, x,x1,x2 ;

	/* color traite les caracteres voulus */
    for (code=First; (code<=Last) && (code < MAXFONTCARD); code++) 
	    if (chartab[code].hsize>0) {
		h = chartab[code].down-chartab[code].up ;if(h==1)continue ;
		ni = n = (int) ceil(sqrt((double)h)) ; 
		b = h/n ;
		r = h%n ;
		Rast_Off(&bitmapsave,BackColor) ;
		dec = 0 ;

		do {	register int bprim ;

		   dec += (bprim = b + (r-- > 0)) ;

		   Rast_Op_1n(chartab[code].image,&bitmapsave,
			0, h - dec,
			ni - n ,
			h - dec,
			chartab[code].hsize,
			bprim,VIDSTR,CurColor);
		} while (n--) ;	

		x2 = chartab[code].hsize + ni - 1 ;
		while (Colonne_color(bitmapsave,x2--,0,h,BackColor)) ;x2+=2;
		x1 = 0 ;
		while (Colonne_color(bitmapsave,x1++,0,h,BackColor)) ;x1--;
		x = x2 - x1 ;
		if (chartab[code].hsize < x) { 
		  chartab[code].hincr += x-chartab[code].hsize ;
		  chartab[code].hsize = x ;

		  Rast_Free(&chartab[code].image) ;
		  Rast_Init(&chartab[code].image,
				chartab[code].hsize,
				chartab[code].down-chartab[code].up,0);
		}
		Rast_Op_n1(bitmapsave,&chartab[code].image,
			x1,0,0,0,
			chartab[code].hsize,
			chartab[code].down-chartab[code].up,
			VIDSTR,BackColor);
	    }
  	ShowDoIt(First,Last);
}


 Kill()
{
	int code ;

	/* on efface les caracteres voulus */
    for (code=First; (code<=Last) && (code < MAXFONTCARD); code++) 
	    if (chartab[code].hsize>0) {
		Rast_Free(&chartab[code].image) ;
		chartab[code].hsize = 0 ;
	    }
  	ShowDoIt(First,Last);
}

 Bold()
{
	int code ;
	int i,j ;

	/* on sauve le car, on libere, on ralloue plus large et on reecrit
		plus large. seul hsize change */
    for (code=First; (code<=Last) && (code < MAXFONTCARD); code++) 
	    if (chartab[code].hsize>0) {
		Rast_Off(&bitmapsave,BackColor) ;
	        Rast_Op_1n(chartab[code].image,&bitmapsave,
			0,0,0,0,
			chartab[code].hsize,
			chartab[code].down-chartab[code].up,
			VIDSTR,CurColor);
		chartab[code].hsize += 1 ; ;
		chartab[code].hincr += 1 ;
		Rast_Free(&chartab[code].image) ;
		Rast_Init(&chartab[code].image,
				chartab[code].hsize,
				chartab[code].down-chartab[code].up,0);
	        Rast_Op_n1(bitmapsave,&chartab[code].image,
			0,0,0,0,
			chartab[code].hsize - 1,
			chartab[code].down-chartab[code].up,
			VIDSTR,BackColor);
	        Rast_Op_n1(bitmapsave,&chartab[code].image,
			0,0,1,0,
			chartab[code].hsize - 1,
			chartab[code].down-chartab[code].up,
			VIDOR,BackColor);
	    }
  	ShowDoIt(First,Last);
}

Anim(delay)
/* on fait une animation dans la poubelle :
   on envoi les caracteres entre First et Last jusqu'a un event click */
{
	int code,temp ;

	code = First ;
	do {
	    if (chartab[code].hsize>0) {
	           w_cleararea(nf_garb, 0,0, 128,128);
		   w_bitblt(nf_garb,chartab[code].image,
			0,0,chartab[code].hsize,
			chartab[code].down-chartab[code].up,0,0);
	           for(temp=0 ; temp<delay; temp++) w_flush() ;
		}
	    code ++ ;
	    if (code > Last) code = First ;
	  } while (w_click());
	afficher_car();   	
}
