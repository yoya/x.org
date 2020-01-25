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
/* otheredit.c */
#include <stdio.h>
#include "clientimage.h"	/* VID */
#include "fedor.h"		/* fedchar */
#include "couche.h"             /* Min, Max */

extern fedchar cartrav, carback ;
extern int Baseliney,Xleft,Baseback, BackLeft ;
extern ClientImage * bitmapsave ;
extern fedchar carundo[] ;
extern BackColor ;

Clear() ;
Back() ;
Mouse() ;
Magnify() ;
Tileh() ;
Tilev() ;
Mirrorh() ;
Mirrorv() ;
Reflecth() ;
Reflectv() ;
NoWhite() ;
Dodo() ;
Undo() ;

#define NUNDO 2
static int oldxleft[NUNDO],oldbase[NUNDO] ;
static int iundo ;

Clear(click)
	int click ;
{
			
	if (click == 1) {
		Dodo();
		cartrav.up = 0 ;
		cartrav.down = 0 ;
		cartrav.hsize = 0 ;
        	Rast_Off(&cartrav.image,BackColor) ;
		Xleft = 0 ;
	} 
	if (click == 2) {
		carback.up = 0 ;
		carback.down = 0 ;
		carback.hsize = 0 ;
        	Rast_Off(&carback.image,BackColor) ;
		BackLeft = 0 ;
	}		
	MontrerCarTrav() ;
}


Back()
{ 	

	CopyCar(&cartrav,&carback);
	BackLeft = Xleft; 
	Baseback = Baseliney ;
	Rast_Copy(cartrav.image,&carback.image);
	Afficher_back();
}


	
Mouse()
{
	int x , y, w, h ;

	x = Xleft ;
	y = Baseliney + cartrav.up ;
	w = cartrav.hsize ;
	h = cartrav.down - cartrav.up ;

	if ((x == 0) && (y == 0)) Translate(0,0,1,1) ; else
	if (x == 0) Translate(0,y,1,y) ; else
	if (y == 0) Translate(x,0,x,1) ;
	if ((x+w >= 128)||(y+h >= 128)) {
		gd_envoyermsg("MOUSE BBOX CAN'T BE CREATED");
		return ;
	}

	x = Xleft ;
	y = Baseliney + cartrav.up ;

	Rast_Copy(cartrav.image,&carback.image);

	Rast_Op(cartrav.image,&carback.image,x,y,x+1,y,w,h,VIDOR,BackColor);
	Rast_Op(cartrav.image,&carback.image,x,y,x,y+1,w,h,VIDOR,BackColor);
	Rast_Op(cartrav.image,&carback.image,x,y,x-1,y,w,h,VIDOR,BackColor);
	Rast_Op(cartrav.image,&carback.image,x,y,x,y-1,w,h,VIDOR,BackColor);
	
	BackLeft = Xleft - 1 ;
	Baseback = Baseliney ;

	CopyCar(&cartrav,&carback);
	carback.hsize += 2;
	carback.up -= 1;
	carback.down += 1;

	Afficher_back();
}



Magnify(numx,denx,numy,deny)
  int numx,denx,numy,deny ;
{
   int nouvx,nouvy ;
   int x, y ;
   int color ;
   int ox,oy,xlim,ylim;
   int bbw, bbh, newbbw, newbbh ;

   Dodo();
   Rast_Off(&bitmapsave,BackColor) ;

   bbw = cartrav.hsize ;
   newbbw = how_many( bbw * numx, denx );
   bbh = (cartrav.down - cartrav.up) ;
   newbbh = how_many( bbh * numy, deny );
  
   if (newbbw + Xleft>128) return ;
   if (Baseliney+cartrav.up+newbbh >128) return ;

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
		    Rast_Pix(bitmapsave,Xleft+nouvx,
			     Baseliney + cartrav.up+nouvy,color);
	       }
	    }
	 }
      }
   }
   Rast_Copy(bitmapsave,&cartrav.image);
   cartrav.hsize = newbbw ;
   if (Xleft+cartrav.hsize>128) cartrav.hsize = 128 - Xleft ;
   cartrav.down += (newbbh - bbh) ;
   if (Baseliney+cartrav.down>128) cartrav.down = 128 - Baseliney ;
   
   MontrerCarTrav() ;
           
}


Tileh()
{
	int x,y,x1,y1 ;

	Dodo() ;
	y = y1 = Baseliney + cartrav.up ;
	x = Xleft ;
	x1 = Xleft + cartrav.hsize ;
        Rast_Op(cartrav.image,&cartrav.image,x,y,x1,y1,
		cartrav.hsize,cartrav.down-cartrav.up,VIDSTR,BackColor);

	cartrav.hsize *=2 ;
	if (Xleft+cartrav.hsize>128) cartrav.hsize = 128 - Xleft ;

	MontrerCarTrav() ;
}



Tilev()
{

	int x,y,x1,y1 ;

	Dodo() ;
	x = x1 = Xleft ;
	y = Baseliney +cartrav.up  ; 
	y1 = Baseliney + cartrav.down ;
	Rast_Op(cartrav.image,&cartrav.image,x,y,x1,y1,
		cartrav.hsize,(cartrav.down - cartrav.up + 1),
		VIDSTR,BackColor);

	cartrav.down += (cartrav.down - cartrav.up);
	if (Baseliney+cartrav.down>128) cartrav.down = 128 - Baseliney ;

	MontrerCarTrav() ;
}

Mirrorh()
{
	
	int x,y,x1,y1 ;

	Dodo() ;
	y = y1 = Baseliney + cartrav.up ;
	Rast_Off(&bitmapsave,BackColor) ;
	for (x = Xleft, x1 = Xleft+cartrav.hsize-1; 
		x < Xleft+cartrav.hsize ; x ++, x1 --)
           	  Rast_Op(cartrav.image,&bitmapsave,x,y,x1,y1,1,
				cartrav.down-cartrav.up,VIDSTR,BackColor);

	Rast_Copy(bitmapsave,&cartrav.image);

	MontrerCarTrav() ;
}


Mirrorv()
{
	int x,y,x1,y1 ;

	Dodo() ;
	x = x1 = Xleft ;
	Rast_Off(&bitmapsave,BackColor) ;
	for (y = Baseliney +cartrav.up-1, y1 = Baseliney + cartrav.down;
		y < Baseliney+cartrav.down ; y++, y1--)
           	  Rast_Op(cartrav.image,&bitmapsave,x,y,x1,y1,
			cartrav.hsize,1,VIDSTR,BackColor);

	Rast_Copy(bitmapsave,&cartrav.image);

	MontrerCarTrav() ;
}


Reflecth()
{
	int x,y,x1,y1 ;

	Dodo() ;
	y = y1 = Baseliney + cartrav.up ;
	for (x = Xleft, x1 = Xleft + 2*cartrav.hsize - 1 ;
		x < Xleft+cartrav.hsize ; x ++, x1 --)
           	  Rast_Op(cartrav.image,&cartrav.image,x,y,x1,y1,
				1,cartrav.down-cartrav.up,VIDSTR,BackColor);

	cartrav.hsize *=2 ;
	if (Xleft+cartrav.hsize>128) cartrav.hsize = 128 - Xleft ;

	MontrerCarTrav() ;
}


Reflectv()
{
	int x,y,x1,y1 ;

	Dodo() ;
	x = x1 = Xleft ;
	for (y = Baseliney +cartrav.up-1, 
	     y1 = Baseliney + cartrav.down +(cartrav.down-cartrav.up);
		y < Baseliney+cartrav.down ; y++, y1--)
           	  Rast_Op(cartrav.image,&cartrav.image,x,y,x1,y1,
				cartrav.hsize,1,VIDSTR,BackColor);

	cartrav.down += (cartrav.down - cartrav.up);
	if (Baseliney+cartrav.down>128) cartrav.down = 128 - Baseliney ;

	MontrerCarTrav() ;
}

NoWhite()
{
  int xhg,yhg,xbd,ybd,l,h,a,b ;

  xhg = Xleft ;
  yhg = Baseliney+cartrav.up ;
  xbd = Xleft+cartrav.hsize ;
  ybd = Baseliney+cartrav.down ;

  /* on recalcule la bbox */
  while ( Colonne_color(cartrav.image,xhg,yhg,ybd+1,BackColor)) xhg++ ;
  Xleft = xhg ;
  while ( Colonne_color(cartrav.image,xbd,yhg,ybd+1,BackColor)) xbd-- ;
  cartrav.hsize = xbd - Xleft + 1 ;
  while ( Ligne_color(cartrav.image,yhg,Xleft,
		      Xleft+cartrav.hsize,BackColor) ) yhg++ ;
  cartrav.up = yhg - Baseliney;

  while ( Ligne_color(cartrav.image,ybd,Xleft,
		      Xleft+cartrav.hsize,BackColor) ) ybd-- ; 
  cartrav.down = ybd - Baseliney + 1 ;

  Aff_all() ;		
}


Dodo()
{
  static idodo = 0 ;
        iundo = idodo ;
	CopyCar(&cartrav,&(carundo[idodo]));
	Rast_Copy(cartrav.image,&carundo[idodo].image);
	oldxleft[idodo] = Xleft ;
	oldbase[idodo] = Baseliney ;
        if (idodo == (NUNDO - 1)) idodo = 0 ; else idodo ++ ;
}


Undo()
{
	CopyCar(&(carundo[iundo]),&cartrav) ;
	Rast_Copy(carundo[iundo].image,&cartrav.image) ;
	Xleft = oldxleft[iundo] ;
	Baseliney = oldbase[iundo] ;
        if (iundo == 0) iundo = (NUNDO - 1) ; else iundo -- ;

	MontrerCarTrav();
}


