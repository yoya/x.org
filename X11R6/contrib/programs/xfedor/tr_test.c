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
/* tr_test.c */
#include <stdio.h>
#include "couche.h" 	/* myEvent */
#include "clientimage.h"
#include "fedor.h"	/* fedchar , BdfHeader */

extern fedchar chartab[MAXFONTCARD] ;	/* pour test fonte */
extern BdfHeader header ;
extern root, nf_tabbord ;
extern int BackColor ;
extern fedchar cartrav, carback ;
extern int Baseback, BackLeft,Xleft,Baseliney ;
	
int Init_test() ;
int Autom_cars();
int Autom_test();
int Test() ;             /* pour le menu TEST dans tr_tabbord */

int nf_test	;	/* sous fenetre test souris */
int nf_cars  ;		/* fenetre test cars de la fonte courante */

#define XCARS (10 + 120) 
#define YCARS (10 + 80)
#define WCARS 600
#define HCARS 400

#include "tabbord.h"

#define XTEST (WCAR+1)
#define YTEST (HTAB+2)
#define WTEST (WTAB-WCAR-4)
#define HTEST (HTABB-HTAB-5)

static int x,y ;	/* position ecriture test fonte */

Init_test()
{
    nf_test = w_ouvrir(nf_tabbord, XTEST, YTEST, WTEST, HTEST,
			"",CURS2,
			EnterWindow|LeaveWindow|ButtonPressed,WMOFF);
	
    nf_cars = w_ouvrir(root, XCARS, YCARS, WCARS, HCARS,
			"xfedor_test",CURS1,
			EnterWindow|ButtonPressed|LeaveWindow|KeyPressed,
			WMON);

    w_montrer(nf_test) ;
}

static Reaffich(fen)
int fen ;
{
     if (fen == nf_test) {	 
	 w_gris(nf_test,1,1,(WTAB-WCAR)/2,HCAR/2);
	 w_fore(nf_test,1,HCAR/2,(WTAB-WCAR)/2,HCAR);
     }
}

Test(click) 
	int click ;
{	
  ClientImage * forimage, * maskimage ;

	if (click==1)  {
	  Rast_Init(&forimage,cartrav.hsize+2,cartrav.down-cartrav.up+2,0);
	  Rast_Init(&maskimage,cartrav.hsize+2,cartrav.down-cartrav.up+2,0);
	  Rast_Op_n1(cartrav.image,&forimage,
		     Xleft,Baseliney+cartrav.up,
		     1,1,
		     cartrav.hsize,cartrav.down-cartrav.up,
		     VIDSTR,BackColor);
	  Rast_Op_n1(carback.image,&maskimage,
		     BackLeft,Baseback+carback.up,
		     0,0,
		     cartrav.hsize+2,cartrav.down-cartrav.up+2,
		     VIDSTR,BackColor);

	  w_souris(nf_test,
		   forimage,maskimage,
		   cartrav.hsize+2,cartrav.down-cartrav.up+2,
		   -cartrav.hadj,-cartrav.up) ;
	  Rast_Free(&forimage);
	  Rast_Free(&maskimage);
	}
	else {
		w_montrer(nf_cars) ;
		w_clear(nf_cars) ;
		x = 10 ;
		y = header.Size  ;
	}
}


static Echo(code)
	int code ;

{	

	if ((code <0) || (code >= MAXFONTCARD)) return ;
	if (code == 13) {
		 y+= header.Size ;
		 x = 10 ;
		 return;
	}
	if (chartab[code].hsize <= 0) return ;
	w_bitblt(nf_cars,chartab[code].image,
			0,0,chartab[code].hsize,
			chartab[code].down-chartab[code].up,
			x+chartab[code].hadj,
			y+chartab[code].up);
	x+= chartab[code].hincr ;
	if (x>WCARS) {	
		 y+= header.Size ;
		 x = 10 ;
	}
	if (y>HCARS) {
		y = header.Size ;
		x = 10 ;
		w_clear(nf_cars) ;
	}		
}



int Autom_cars(pev)
	myEvent * pev ;
{
   switch(pev->type){
	case EnterWindow   : 
                Afficher_boutons("Exit","Quit") ;
		break;   
	case KeyPressed :  
	    	Echo(pev->key);
            	break ;
	case ButtonPressed : 
		w_cacher(nf_cars) ;
		break ;
        case LeaveWindow : 
		Afficher_boutons("","") ; 
		break ;
   }
}

int Autom_test(pev)
	myEvent * pev ;
{
   switch(pev->type){
	case Exposure :
		Reaffich(pev->window);
		break ;
	case EnterWindow   : 
                Afficher_boutons("Mouse","Font") ;
		break;   
	case ButtonPressed :  
	    	Test(pev->click);	    
            	break ;
       case LeaveWindow :	
		Afficher_boutons("","") ; 
 		break ;
   }
}

