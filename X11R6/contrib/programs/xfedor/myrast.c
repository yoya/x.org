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
/* myrast.c */
#include <stdio.h>
#include "clientimage.h"	/*  ClientImage .. */

#include <X11/Xlib.h>

extern Display * Dpl ;
extern Window root ;
extern int screen ;
extern int depth ;
extern int ImaForm ;
static ClientImage * backcol ;

Init_Rast(color)
/**************/
	int color ;
{
   int i,j ;

   /* the first time, we create an ClientImage fill in the color(one!)
      it has the same content structure as the climag to come for bcopy :
      128x128 depth, ImaForm and MSB */
   Rast_Init(&backcol,128,128,1);
   for (i=0 ; i<128; i++) {
     for (j=0 ; j<128; j++) {
       Rast_Pix(backcol,i,j,color);
    }
}

}
  
Rast_Free(climag)
/***************/
	ClientImage * * climag ;
{
  XDestroyImage(*climag) ;
  *climag = NULL ;
}

Rast_Init(climag,w,h,flag)
/************************/
/* alloue et initialise une image :
     si flag = 0 : depth 1 pad 8 au format XYBitmap 
     si flag = 1 : depth n pad 8 au format ImaForm 
*/
	ClientImage * * climag ;
	int w,h ;
        int flag ; /* 0 ou 1 */
{	
  char * data ;
  int vidwidth ;

  vidwidth = ((w+7)>>3) ;       /* pad 8 */
  data = (char *) calloc (1,vidwidth*h*((flag)?depth:1)) ;   /* tout a 0 */

  * climag = (ClientImage *) XCreateImage(Dpl,DefaultVisual(Dpl,screen),
			(flag)?depth:1,
	                (flag)?ImaForm:XYBitmap,
			0,data,w,h,8,0) ;
  if (flag == 0) {
  /* il semble qu'il suffise d'ecraser ces valeurs pour changer
   le format de l'image cree */
     ((XImage *)*climag)->byte_order = MSBFirst ;
     ((XImage *)*climag)->bitmap_unit = 8 ;
     ((XImage *)*climag)->bitmap_bit_order = MSBFirst ;
  /* cette image sera manipuler par XPutPixel, XGetPixel, XPutImage
     qui garderont son format tel quel */
  }

}


Rast_Off(climag,color)
/********************/ 
/* 128x128 only , format MSB/pad8 only */
	ClientImage * * climag ;
        int color ;
{
   bcopy(((XImage *)backcol)->data,((XImage *)*climag)->data,128/8*128*depth);
}


Rast_Copy(sourceimage,destimage)
/******************************/
/* 128x128 only , format MSB/pad8 only */
	ClientImage * sourceimage, * * destimage ;
{
   bcopy(((XImage *)sourceimage)->data,((XImage *)*destimage)->data,
	 128/8*128*depth);
}


Rast_Put(climag,data,w,h)
/***********************/
/* data pointe sur un plan bitmap w*h pad=8 MSB */
/* on construit une XImage monochrome depth 1 */
	ClientImage * * climag ;
        char * data ;
	int w,h ;
{

  *climag = (ClientImage *) XCreateImage(Dpl,DefaultVisual(Dpl,screen),
			1,XYBitmap,0,data,w,h,8,0) ;
  ((XImage *)*climag)->byte_order = MSBFirst ;
  ((XImage *)*climag)->bitmap_unit = 8 ;
  ((XImage *)*climag)->bitmap_bit_order = MSBFirst ;

}


Rast_Mem(climap,w,h,pmem,pwidth)
/******************************/
/* rend le pointeur sur la memoire image et la largeur en bytes */
/* on saura que c'est en pad8 et MSB */
	ClientImage * climap ;
        int w,h ;
	char * * pmem ;
	int * pwidth ;
{
  *pmem = climap->data ;
  *pwidth = climap->bytes_per_line ;
}


Rast_Inq(climag,x,y)
/*******************/
/* return the pixel value */
	ClientImage * climag ;
	int x,y ;
{
  return XGetPixel(climag,x,y) ;
}


Rast_Pix(climag,x,y,color)
/************************/
	ClientImage* climag ;
	int x,y ;
	int color ;
{
  XPutPixel(climag,x,y,color) ;
}


Ligne_color(climag,y,x1,x2,color)
/*******************************/
	ClientImage * climag ;
	int y,x1,x2 ;
	int color ;
/* rends 0 si au moins un pixel de climag entre x1
   et x2, avec y en ordonnee est de la couleur color */
{
	int i ;

	for ( i=x1; i<x2 ; i++) 
		if (Rast_Inq(climag,i,y)!=color) return 0 ;
	return 1 ;
}	


Colonne_color(climag,x,y1,y2,color)
/*********************************/
	ClientImage * climag ;
	int x,y1,y2 ;
	int color ;
/* rends 0 si au moins un pixel de climag entre y1
   et y2, avec x en ordonnee est de la couleur color */
{
	int j ;

	for ( j=y1; j<y2; j++) 
		if (Rast_Inq(climag,x,j)!=color) return 0 ;
	return 1 ;
}	

Rast_Op(sourceimage,destimage, x1,y1,x2,y2,w,h, mode, color)
/**********************************************************/
	ClientImage * sourceimage, * * destimage ;
	int x1,y1,x2,y2 ;
	int w,h ;
	int mode ;
        int color ;
{
  register int i,j ;
  register int dx, dy, lx, ly;
  register int p,q ;
  
  dx = x2 - x1 ;
  dy = y2 - y1 ;
  lx = x1+w ;
  ly = y1+h ;
  if (mode == VIDSTR ) {
    for (i=x1 ; i<lx ; i++)
      for (j=y1 ; j<ly ; j++) {
	p = XGetPixel(sourceimage,i,j) ;
	XPutPixel(*destimage,i+dx,j+dy,p) ;
      }
  } else
  if (mode == VIDOR ) {
    for (i=x1 ; i<lx ; i++)
      for (j=y1 ; j<ly ; j++) {
	p = XGetPixel(sourceimage,i,j) ;
	if (p == color) continue ;
	if (XGetPixel(*destimage,i+dx,j+dy) != color) continue ;
	XPutPixel(*destimage,i+dx,j+dy,p) ;
      }
  } else
  if (mode == VIDAND ) {
    for (i=x1 ; i<lx ; i++)
      for (j=y1 ; j<ly ; j++) {
	p = XGetPixel(sourceimage,i,j) ;
	q = XGetPixel(*destimage,i+dx,j+dy) ;
	if ((p == color) || (q == color))
	  XPutPixel(*destimage,i+dx,j+dy,color) ; else continue ;
      }
  } else
  if (mode == VIDXOR ) {
    for (i=x1 ; i<lx ; i++)
      for (j=y1 ; j<ly ; j++) {
	p = XGetPixel(sourceimage,i,j) ;
	q = XGetPixel(*destimage,i+dx,j+dy) ;
	if ((p != color) && (q == color))
	  XPutPixel(*destimage,i+dx,j+dy,p) ; else
	if ((p != color) && (q != color))
	  XPutPixel(*destimage,i+dx,j+dy,color) ; else continue ;
      }
  }

}

Rast_Op_1n(sourceimage,destimage, x1,y1,x2,y2,w,h, mode, colorcur)
/****************************************************************/
/* copy d'image de depth = 1 sur depth = n */
/* on ne fait que mettre des pixels colorcur sur destimage */
	ClientImage * sourceimage, * * destimage ;
	int x1,y1,x2,y2 ;
	int w,h ;
	int mode ;
        int colorcur ;
{
  register int i,j ;
  register int dx, dy, lx, ly;
  
  dx = x2 - x1 ;
  dy = y2 - y1 ;
  lx = x1+w ;
  ly = y1+h ;
  for (i=x1 ; i<lx ; i++)
    for (j=y1 ; j<ly ; j++) 
      if (XGetPixel(sourceimage,i,j) == 1)
	XPutPixel(*destimage,i+dx,j+dy,colorcur) ;
}

Rast_Op_n1(sourceimage,destimage, x1,y1,x2,y2,w,h, mode, colorback)
/*****************************************************************/
/* copy d'image depth = n vers depth = 1 */
/* on met des pixel a 1 dans le bitmap depth = 1 */
	ClientImage * sourceimage, * * destimage ;
	int x1,y1,x2,y2 ;
	int w,h ;
	int mode ;
        int colorback ;
{
  register int i,j ;
  register int dx, dy, lx, ly;
  
  dx = x2 - x1 ;
  dy = y2 - y1 ;
  lx = x1+w ;
  ly = y1+h ;
  for (i=x1 ; i<lx ; i++)
    for (j=y1 ; j<ly ; j++) 
      if (XGetPixel(sourceimage,i,j) != colorback)
	XPutPixel(*destimage,i+dx,j+dy,1) ;
}


/*********
  Une XImage est caracterisee par 
      sa taille, son format, son pad, sa depth,
  qui sont donnees a la creation par l'utilisateur de XCreateImage et par
      son byte_order, bitmap_unit, bitmap_bit_order et bits_per_pixel
  qui sont mises a jour a partir des caracteristiques du serveur X courant.
      (hors controle de l'utilisateur...sauf hack)

  Ensuite les primitives XGetPixel et XPutPixel "normalize" en LSBFirst 
  les bitmap_unit (pas l'image complete) de l'image avant de faire le boulot. 


  HORS, je prefere que toutes mes XImages soient en MSBFirst, unit=8, pad=8 : 
       - celles de la fonte car je les lit et les ecrit ainsi rapidos.
       - celles des autres images car on fait des copies avec bcopy en
         connaissant ainsi le format.

***********/

