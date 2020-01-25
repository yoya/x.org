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
/* ed_rotate.c */
#include <stdio.h>
#include "couche.h"
#include "clientimage.h"
#include "fedor.h"
#include "style.h"	/* NORMAL .. */

extern fedchar cartrav;
extern ClientImage * bitmapsave ;
extern Xleft,Baseliney ;
extern BackColor ;

static int dx,dy,alf ;

#define PASROT 5     /* pas de la rotation */

     /* TABLEAUX CONTENANT 1024 * LE SINUS ET LE COSINUS DES ANGLES DE
        0 A 45 DEGRES PAR PAS DE 5 DEGRES */
static int Sin[] = {0, 89, 178, 265, 350, 433, 512, 587, 658, 724} ;
static int Cos[] = {1024,1020,1008,989,962,928,887,839,784,724} ;

/*********************** procedure CHOIX_COSSIN **********************/
/* On passe un angle compris entre -180 et 180 degres et la procedure
   rend le cosinus et le sinus de l'angle */

static choix_cossin(angle, cosinus, sinus)
    short angle ;     /* angle de le rotation : -180 < angle < 180 */
    int *cosinus ;     /* cosinus de la rotation */ 
    int *sinus ;       /* sinus de la rotation */

   { int indtab ; 
   
     if (angle < 0) indtab = (360 + angle) / PASROT;
               else indtab = angle / PASROT;
             /* ANGLE COMPRIS ENTRE 0 ET 49 */
     if (indtab < 10) { *cosinus = Cos[indtab] ;
                        *sinus = Sin[indtab] ;
                      }
             /* ANGLE COMPRIS ENTRE 50 ET 94 */
      else if (indtab < 19) { *cosinus = Sin[18 - indtab] ;
                              *sinus = Cos[18 - indtab] ;
                           }
             /* ANGLE COMPRIS ENTRE 95 ET 139 */
      else if (indtab < 28) { *cosinus = - Sin[indtab - 18] ;
                              *sinus = Cos[indtab - 18] ;
                           }
             /* ANGLE COMPRIS ENTRE 140 ET 184 */
      else if (indtab < 37) { *cosinus = - Cos[36 - indtab] ;
                              *sinus = Sin[36 - indtab] ;
                            }
             /* ANGLE COMPRIS ENTRE 185 ET 229 */
      else if (indtab < 46) { *cosinus = - Cos[indtab - 36] ;
                              *sinus = - Sin[indtab - 36] ;
                            }
             /* ANGLE COMPRIS ENTRE 230 ET 274 */
      else if (indtab < 55) { *cosinus = - Sin[54 - indtab] ;
                              *sinus = - Cos[54 - indtab] ;
                            }
             /* ANGLE COMPRIS ENTRE 275 ET 319 */
      else if (indtab < 64) { *cosinus = Sin[indtab - 54] ; 
                              *sinus = - Cos[indtab - 54] ;
                            }
             /* ANGLE COMPRIS ENTRE 320 ET 359 */
      else { *cosinus = Cos[72 - indtab] ;
             *sinus = - Sin[72 - indtab] ;
           }
}

static Rotate(xcenter,ycenter)
	int xcenter,ycenter ;

{	
	int cosinus,sinus ;
	int color ;
	register short x,y,x1,y1,x2,y2,x3,y3,x4,y4,xa,xb,ya,yb ;
	register short 	supgx = Xleft,
			supgy = Baseliney + cartrav.up,
			infdx = Xleft + cartrav.hsize,
			infdy = Baseliney + cartrav.down ;
	
	choix_cossin(alf,&cosinus,&sinus);

	Rast_Off(&bitmapsave,BackColor) ;

	for(x = supgx; x< infdx ; x++) 
	    for(y= supgy; y < infdy ; y++) {
	      color = Rast_Inq(cartrav.image,x,y) ;
	      if (color != BackColor){
		x2 = x - xcenter ;
		y2 = y - ycenter ;
		x1 = ((x2*cosinus - y2*sinus)>>10) + xcenter;
		y1 = ((x2*sinus + y2*cosinus)>>10) + ycenter;
		Rast_Pix(bitmapsave,x1,y1,color);
	      }				
	    }   
 		

	Rast_Copy(bitmapsave,&cartrav.image);
	
	/* il faut recalculer la boundingbox */
	/* on calcule les limites theoriques */
	x1 = (((supgx-xcenter)*cosinus - 
		(supgy-ycenter)*sinus)>>10) + xcenter;
	y1 = (((supgx-xcenter)*sinus +  
		(supgy-ycenter)*cosinus)>>10) + ycenter;
	x2 = (((infdx-xcenter)*cosinus - 
		(supgy-ycenter)*sinus)>>10) + xcenter;
	y2 = (((infdx-xcenter)*sinus +  
		(supgy-ycenter)*cosinus)>>10) + ycenter;
	x3 = (((infdx-xcenter)*cosinus - 
		(infdy-ycenter)*sinus)>>10) + xcenter;
	y3 = (((infdx-xcenter)*sinus +  
		(infdy-ycenter)*cosinus)>>10) + ycenter;
	x4 = (((supgx-xcenter)*cosinus - 
		(infdy-ycenter)*sinus)>>10) + xcenter;
	y4 = (((supgx-xcenter)*sinus +  
		(infdy-ycenter)*cosinus)>>10) + ycenter;
 	
	xa = Min(Min(x1,x2),Min(x3,x4)); if (xa<0) xa=0;
	xb = Max(Max(x1,x2),Max(x3,x4)); if (xb>128) xb=128 ;
	ya = Min(Min(y1,y2),Min(y3,y4)); if (ya<0) ya=0;
	yb = Max(Max(y1,y2),Max(y3,y4)); if (yb>128) yb=128 ;

	/* on verifie le contenu de cette bbox(xa,ya,xb,yb) */
	x = xa ;
	while ( Colonne_color(cartrav.image,x,ya,yb,BackColor) 
	       && (x <=xb)) x++ ;
	if (x > xb) {
			cartrav.hsize = 0 ;
			cartrav.up = 0 ;
			cartrav.down = 0 ;
			Xleft = 0 ;
			return ;
	}
	Xleft = x ;
	x = xb ;
	while ( Colonne_color(cartrav.image,x,ya,yb,BackColor) ) x-- ;
	cartrav.hsize = x - Xleft + 1;

	y = ya ;
	while ( Ligne_color(cartrav.image,y,xa,xb,BackColor) ) y ++ ;
	cartrav.up = y - Baseliney;

	y = yb ;
	while ( Ligne_color(cartrav.image,y,xa,xb,BackColor) ) y -- ;
	cartrav.down = y - Baseliney + 1 ;

	MontrerCarTrav() ;
}

Autom_rotate (pev)
	myEvent * pev ; 
{  static int x,y ;
   static short etat ;
   float tang ;	
   char s[5] ;

    switch (pev->type) {
	case EnterZone : Afficher_boutons("CENTER","CENTER");
			 x = -1 ;
			 break ;
	case ButtonPressed : if (x == -1 ) {
				Afficher_boutons("ANGLE","ANGLE");
			    	x = pev->x ; 
			    	y = pev->y ; 
			     	stylesouris(LIGNE,x,y);
			     }
			     etat = 1 ;
			     break ;
	case ButtonReleased : stylesouris(NORMAL,0,0);
			      Afficher_boutons("CENTER","CENTER");
			      if (x != -1) {Dodo() ;
						Rotate(convert(x),
						       convert(y));
						/* alf global */
			      }
			      x = -1 ;
			      etat = 0 ;
			      break ;
	case MoveMouse : if (etat) { /* enfonce */
			     dx = pev->x - x ;
			     if (dx == 0) alf = 90 ;
				else {
			     	  dy = pev->y - y ;
				  tang = (float) (dy) / dx ;

				  if (tang < 0) tang = -tang ;
				  if (tang < 0.043658) alf = 00; else
				  if (tang < 0.131643) alf = 05; else
				  if (tang < 0.221678) alf = 10; else
				  if (tang < 0.315274) alf = 15; else
				  if (tang < 0.414179) alf = 20; else
				  if (tang < 0.520522) alf = 25; else
				  if (tang < 0.637011) alf = 30; else
				  if (tang < 0.767250) alf = 35; else
				  if (tang < 0.916230) alf = 40; else
				  if (tang < 1.091174) alf = 45; else
				  if (tang < 1.303042) alf = 50; else
				  if (tang < 1.569428) alf = 55; else
				  if (tang < 1.920603) alf = 60; else
				  if (tang < 2.413618) alf = 65; else
				  if (tang < 3.170559) alf = 70; else
				  if (tang < 4.508571) alf = 75; else
				  if (tang < 7.589500) alf = 80; else
				  if (tang < 22.844478)alf = 85; else
						       alf = 90; 
				}
			     	if (dx < 0) alf = 180 - alf ;
				if (dy < 0) alf = (-alf) ;
				sprintf(s,"%4d",alf) ;
				Afficher_sos(s) ;
			 }    
			 break ;				
	case LeaveZone : etat = 0 ;
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 stylesouris(NORMAL,0,0);
			 Afficher_boutons("","");
			 Afficher_sos("SOS");
			 break ;
    }
} 
