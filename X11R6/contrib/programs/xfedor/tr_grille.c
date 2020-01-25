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
/* tr_grille.c */
#include <stdio.h>
#include "couche.h" 		/* myEvent */
#include "clientimage.h"
#include "fedor.h"		/* fedchar */
#include "edit.h"		/* MODBLANC WMENU WEDIT WHITE GREY */

extern nf_edit ;
extern MetrAff	; 	/* bool qui indique que la metrique est afficher */
			/* cad la BiBox + les points adj et incr */
extern modcolor ; 	/* MODBACK, MODINV, ou MODCOLOR */
extern modecour ;	/* point, line .. */
extern colortrame ;	/* WHITE GREY */
extern int BackColor ;

Init_grille() ;
Autom_grille() ;
convert();
Changer_baseline() ;
Changer_guideline() ;
Resol_possible() ;
Afficher_back() ;
Afficher_all() ;
Changer_editresol() ;
MontrerCarTrav() ;
Adjpoint() ;
carre_pas();
Invpoint() ;
BiBox() ;

int nf_grille ;   /* NUMERO DE LA SOUS-FENETRE GRILLE */      

fedchar cartrav, carback, carundo[20] ;
int Baseliney,Xleft ;		/* pour tr_font */
int BackLeft,Baseback ;
ClientImage * bitmapsave ;

int editresol ;		/* 16 32 64 128 */

#define XGRILLE WMENU
#define YGRILLE 0
#define WGRILLE WEDIT
#define HGRILLE WEDIT

static int zedit ;

#define NGL 4
static int tguide[NGL] ;
static int ngl = 0 ;

#define NUNDO 2

int CurColor, TrameColor ;

static int back_pix ;

Init_grille() 
{	int i ;

    nf_grille = w_ouvrir(nf_edit,XGRILLE,YGRILLE,WGRILLE,HGRILLE,
		    	      "",CURS4,
				EnterZone|ButtonPressed|ButtonReleased|
				KeyPressed|MoveMouse|LeaveZone,WMOFF) ;
    w_backing(nf_grille);

    zedit = w_crzon(nf_grille,0,0,WGRILLE,HGRILLE,CURS4);

    CurColor = w_blackpixel();

    TrameColor = w_greypixel();



	/* INIT DES BITMAP CLIENTS */
    Rast_Init(&cartrav.image,128,128,1);  /* flag = 1 pour depth n */
    Rast_Init(&bitmapsave,128,128,1);
    Rast_Init(&carback.image,128,128,1);
    for (i=0; i<NUNDO ; i++) {
      Rast_Init(&carundo[i].image,128,128,1);
    }
    Rast_Off(&cartrav.image,BackColor);  
    Rast_Off(&bitmapsave,BackColor);
    Rast_Off(&carback.image,BackColor);
    for (i=0; i<NUNDO ; i++) {
      Rast_Off(&carundo[i].image,BackColor);
    }

    editresol = 32 ; 
    Baseliney = 31 ; 	
    for (i=0 ; i<NGL; i++) tguide[i] = -1 ;

    back_pix = w_bitmap(32,32);

    w_montrer(nf_grille);

}

static Reaffich(pev)
	myEvent * pev ;
{ 
/*  printf("expose %d %d %d %d\n",pev->x,pev->y,pev->xz,pev->yz);*/
   	if (pev->window == nf_grille) Afficher_all();
}

int convert(xy)
	int xy ;
{
	if (xy == 0) return 0 ; else
	if (xy >= 513) return (editresol - 1) ; else
			return (xy - 1)/(512/editresol) ;
}


static Afficher_baseline()
{	short pas = (512/editresol);

	if (Baseliney == 0) return ;
	w_line(nf_grille,1,Baseliney*pas +1,WGRILLE-2,Baseliney*pas +1,ON);
	w_line(nf_grille,1,Baseliney*pas,WGRILLE-2,Baseliney*pas,ON);
}	
	
Changer_baseline(by) 
	int by ;	/* espace fenetre 0-514 */
/* efface l'ex baseline, modifie la globale Baseliney et l'affiche */
/* modifie aussi les valeurs up et down et les reaffiche */
{
	short dby,cby,i ;
	short pas = (512/editresol) ;	
	
	cby = convert(by) ;
		/* first, we verify that there is no guideline here */
	for (i=0 ; i<NGL ; i++) if (tguide[i] == cby) return ;
	dby = cby - Baseliney ; /* differentiel */
	if (dby) {
	  if (colortrame == GREY) 
	    w_color(nf_grille,0,Baseliney*pas,
		    WGRILLE,Baseliney*pas +2,TrameColor); else
	  if (colortrame == WHITE) 
	    w_color(nf_grille,0,Baseliney*pas,
		    WGRILLE,Baseliney*pas +2,BackColor);
		/* efface */
		/* on doit reafficher les pixels back dont on a effacer
			un cote horizontal */
		for (i=BackLeft ; i < BackLeft+carback.hsize ; i++)
		   if ((Rast_Inq(carback.image,i,Baseliney) != BackColor) ||
			(Rast_Inq(carback.image,i,Baseliney-1) != BackColor))
			w_line(nf_grille,i*pas +1 , Baseliney*pas, 
				(i+1)*pas +1, Baseliney*pas,ON);
		    	
		if (cartrav.down-cartrav.up!=0)	{ 	
				/* => up et down != 0 */
			cartrav.up -= dby ;
			cartrav.down -= dby ;
			Aff_all() ;
		}
		Baseliney += dby ;
		Afficher_baseline();
	}
}

static Afficher_guidelines() 
{
	short i,pas = (512/editresol) ;	
	
	for (i=0 ; i<NGL ; i++)
		if (tguide[i] > 0) 	
			w_line(nf_grille,0,tguide[i]*pas +1,
                  		WGRILLE,tguide[i]*pas +1,ON);
}

Changer_guideline(gy)
	int gy ;
{
	short dgy ;
	short i,pas = (512/editresol) ;	

	dgy = convert(gy) ;
	/* first we see if there is a collision with Baseline */
	/* we not permitted that */
	if (dgy == Baseliney)  return ;
	/* if there is already one guideline at this level, it erase it
		on the root and on the internal memory, else
	   it add one new guideline in this two structure */
	/* research of existense : we have NGL guideline possible :
		from tguide[0] to tguide[NGL-1] */

	for (i=0 ; i< NGL ;i++) if (tguide[i]==dgy) break ;

	if (i==NGL) {/* it means that we haven't found any guideline
			already stored */
		if (ngl < NGL) {/* we have at least one free cell */
			i = 0 ;
			while(tguide[i]!=-1) i++ ;
			tguide[i] = dgy ;
			ngl ++ ;
			if (dgy) w_line(nf_grille,0,dgy*pas +1,
                  			WGRILLE,dgy*pas +1,ON);
		} 
        } else {/* we have found a tguide[i] equal to dgy */
		tguide[i] = -1 ;
		ngl -- ;
		if (colortrame == GREY) 
		  w_color(nf_grille,0,dgy*pas +1,
			      WGRILLE,dgy*pas +2,TrameColor);	else
		if (colortrame == WHITE) 
		  w_color(nf_grille,0,dgy*pas +1,
			      WGRILLE,dgy*pas +2,BackColor);	
		/* efface */
		
	}	
}

static Afficher_grille ()    
{ 
    register short l,pas=(512/editresol) ;
    

    if (colortrame == GREY) 
      w_color(back_pix,0,0,pas,pas+1,TrameColor) ; else
    if (colortrame == WHITE) 
      w_color(back_pix,0,0,pas,pas+1,BackColor) ;
   
     w_color(back_pix,1,2,pas,pas+1,BackColor) ;     

     for(l=pas ; l<32 ; l<<=1)
         w_rastop(back_pix,back_pix,0, 0, l, pas, l, 0);

     for(l=pas ; l<32 ; l<<=1)
         w_rastop(back_pix,back_pix,0, 0, 32, l, 0, l) ;
 
    w_setback(nf_grille,back_pix);
     
/*   if (colortrame == GREY) 
      w_color(nf_grille,512,0,514,514,TrameColor) ; else
    if (colortrame == WHITE) 
      w_color(nf_grille,512,0,514,514,BackColor) ;
*/
}     


static Afficher_car() 
{
	register int x,y,limx,limy ;

	limx = Xleft + cartrav.hsize ;
	limy = Baseliney + cartrav.down ;
	for (x=Xleft; x < limx ; x ++)
	    for (y=Baseliney+cartrav.up; y < limy; y ++)
		 carre_pas(x,y,Rast_Inq(cartrav.image,x,y)) ;
			
}

Afficher_back()
/* ne fait rien si carback.hsize = 0 */
{	int x,y ;
	register short pas =(512/editresol), limx, limy ;

	limx = BackLeft + carback.hsize ;
	limy = Baseback + carback.down ;
	for (x=BackLeft; x < limx ; x ++)
		for (y=Baseback+carback.up; y < limy; y ++)
		   if (Rast_Inq(carback.image,x,y) != BackColor)
			w_rect(nf_grille,x*pas +1 , y*pas, 
				(x+1)*pas +1, (y+1)*pas,ON);
}
	
		


Afficher_all()
/* ne touche qu'a la fenetre nf_grille */
{
	Afficher_grille();
	Afficher_baseline();
	Afficher_guidelines();
	Afficher_car() ;
	Afficher_back() ;
	if (MetrAff == 1) {	/* il faut retracer la metrique */
				/* afficher par ed_metric */
		BiBox();
           	Invpoint(Xleft - cartrav.hadj,Baseliney);
           	Invpoint(Xleft - cartrav.hadj +cartrav.hincr ,Baseliney);
	}
	CarFen() ;
}


int Resol_possible(resol)
/* rends VRAI si la resolution donnee peut contenir le caractere courant */
{
      if (resol == 128) return 1 ; 
      return ( (Xleft+cartrav.hsize <= resol) &&
		 (Max(Baseliney,Baseliney+cartrav.down) <= resol) &&
		 (BackLeft+carback.hsize <= resol) &&
		 (Max(Baseback,Baseback+carback.down) <= resol)) ;
}


Changer_editresol() 
{ 

	editresol = 16 ;
	while (!Resol_possible(editresol))   editresol <<= 1;
}


MontrerCarTrav() 
/* cartrav est complet, Baseliney et Xleft sont fixes, on modifie 
   editresol en fonction de ces valeurs */
{ 

        Changer_editresol() ;
	Afficher_resol(editresol) ;
	Afficher_all() ;
	Aff_all() ;
	Aff_name(cartrav.name);
}



int Adjpoint(x,y)
/* x,y en 0_128 */
/* rends une couleur  ou -1 si on a rien trace */
	int x,y ;
{	
	int color ;

	if ((x < 0) || (x > 128) || (y < 0) || (y > 128)) return -1 ;

	color = Rast_Inq(cartrav.image,x,y) ;

	if ((modcolor == MODINV) && (color == BackColor))
	  color = CurColor ; else
	if ((modcolor == MODINV) && (color != BackColor))
	  color = BackColor ; else
	if ((modcolor == MODBACK) && (color == BackColor))
	  return -1 ; else
	if ((modcolor == MODCOLOR) && (color == CurColor))
	  return -1 ; else
        if (modcolor == MODBACK) color = BackColor ; else
        if (modcolor == MODCOLOR) color = CurColor ; 

	Rast_Pix(cartrav.image,x,y,color);
	if (color != BackColor) {  /* on "allume" un pixel  */
	  if ((cartrav.down-cartrav.up)== 0) { /* init */
	    Xleft = x ;
	    cartrav.hsize = 1 ;
	    cartrav.up = y - Baseliney ;
	    cartrav.down = y - Baseliney + 1 ;
	    *cartrav.name = '\0' ;
	  } else {
	    if (x < Xleft)  {
	      cartrav.hsize += Xleft - x ;
	      Xleft = x ;
	    } else
	      if (x >= Xleft + cartrav.hsize)  {
		cartrav.hsize = x - Xleft +1 ;
	      } 
	    if (y < Baseliney + cartrav.up) {
	      cartrav.up = y - Baseliney ;
	    } else
	      if (y >= Baseliney + cartrav.down) {
		cartrav.down = y - Baseliney + 1;
	      }
	  }
	} else {	/* il faut voir si cette remise a blanc d'un pixel
		           fait diminuer up,down, hsize ou Xleft */
		if (x==Xleft) {
		  while( (Colonne_color(cartrav.image,
					Xleft,Baseliney+cartrav.up,
					Baseliney+cartrav.down,
					color)) 
			&&(cartrav.hsize!=0))
		    {
		      Xleft ++ ;
		      cartrav.hsize -- ;
		    }
		} else
		  if (x==Xleft+cartrav.hsize-1) 
		    while ( (Colonne_color(cartrav.image,
					   Xleft+cartrav.hsize-1,
					   Baseliney+cartrav.up,
					   Baseliney+cartrav.down,
					   color)) 
			   && (cartrav.hsize!=0)) 
		      {
			cartrav.hsize -- ;
		      } 
		if (cartrav.hsize == 0 ) {
		  cartrav.up = 0 ;
		  cartrav.down = 0 ;
		  Xleft = 0 ;
		}
		if (y==Baseliney+cartrav.up)
		  while ( (Ligne_color(cartrav.image,
				       Baseliney+cartrav.up,Xleft,
				       Xleft+cartrav.hsize,
				       color)) 
			 && (cartrav.down-cartrav.up!=0)) {
		    cartrav.up ++ ;
		  } else
		    if (y==Baseliney+cartrav.down-1)
		      while ( (Ligne_color(cartrav.image,
					   Baseliney+cartrav.down-1,Xleft,
					   Xleft+cartrav.hsize,
					   color))  
			     && (cartrav.down-cartrav.up!=0)) {
			cartrav.down -- ;
		      } 
	      }
	carre_pas(x,y,color);
	return(color);
}

carre_pas(x,y,color)
	int x,y ;
	int color ;
{
	register short pas = (512/editresol) ;

	w_color(nf_grille,x*pas +2,y*pas+2, 
		(x+1)*pas, (y+1)*pas,color);
}

Invpoint(x,y)		/* pour ed_beziers et ed_metric */
	int x,y;
{	
	short pas = (512/editresol) ;

	if (x<0) return ;
	w_fellips(nf_grille,x*pas - 2 , y*pas - 2 , 
		(x+1)*pas + 3 , (y+1)*pas + 3,OFF);
}

BiBox() 
{	
	short pas = (512/editresol) ;

	w_rect(nf_grille,Xleft*pas,
		       	(Baseliney+cartrav.up)*pas,
		       	(Xleft+cartrav.hsize)*pas,
		       	(Baseliney+cartrav.down)*pas,
			OFF);
		        
}


Autom_grille(pev)
	myEvent * pev ;
{
    if (pev->type == Exposure ) Reaffich(pev) ; else
	switch ( modecour) {
	  case point : Autom_point(pev) ; break ;
	  case line : Autom_line(pev) ; break ;
	  case recta : Autom_rect (pev) ; break ;
	  case cercle : Autom_cercle(pev) ; break ;
	  case resize : Autom_resize(pev) ; break ;
	  case write : Autom_write(pev) ; break ;
	  case transl : Autom_transl(pev) ; break ;
	  case bez : Autom_bez(pev) ; break ;
	  case rotate: Autom_rotate(pev) ; break ;
	  case fil : Autom_fill(pev) ; break ;
	  case metric : Autom_metric(pev) ; break ;
	  case cutap : Autom_cutap(pev) ; break ;
        }
}

