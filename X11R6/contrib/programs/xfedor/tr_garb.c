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
/* tr_garb.c */
#include <stdio.h>
#include "clientimage.h"	  /* VID */
#include "couche.h"	  /*  CURS2 */
#include "sos.h"	/* SOS .. */
#include "fedor.h"	  /* fedchar .. */


extern nf_mono ;
extern fedchar cartrav ;	/* depuis tr_edit */
extern int Baseliney, Xleft ;
extern int BackColor, CurColor ;

Autom_garb();
Init_garb();
CopyCar() ;	
FontToGarb() ;	/* pour tr_font  */
afficher_car() ;

int nf_garb ;   /* NUMERO DE LA FENETRE POUBELLE */      
int courant ;   /* pour animation */


	/*---------- reste du LOCAL */
      
# define MAXCARGARB  8      

static int idmraster ;	/* id du popup RASTER */
static char *choixrast[] = {"OR","AND","XOR", 0 } ;

static int zgarb,zhelp,zput,zget,zrast,zraz ; /* id des zones crees */
     
#define HGARB 128
#define WGARB 128
#define WHELP 10
#define WGARBMENU 100
#define HGARBITEM (HGARB/4)
#define WFEN (WGARB+WHELP+WGARBMENU)

#define XGARB (10 + 100+514+12 + 10 + 2)
#define YGARB (10 + 22 + 2*22+130 + 2 + 22 + 7*23 +2) 

static fedchar tabgarb[MAXCARGARB] ;  
static int occupe[MAXCARGARB] ;
static int taborx[MAXCARGARB] ;
static int tabory[MAXCARGARB] ;

static int BlackColor ;

/************************ INIT_GARB ******************/
Init_garb () 

{ int i ;
   
     nf_garb = w_ouvrir(nf_mono, XGARB, YGARB, WFEN, HGARB,
		  	"XFEDOR : buffer",CURS2,
		  	EnterZone|ButtonPressed|LeaveZone,WMON) ;

    zgarb = w_crzon(nf_garb,0,0,WGARB,HGARB,CURS5);
    zhelp = w_crzon(nf_garb,WGARB,0,WGARB+WHELP,HGARB,CURS2);
    zput  = w_crzon(nf_garb,WGARB+WHELP,0,WFEN,HGARBITEM,CURS2);
    zget  = w_crzon(nf_garb,WGARB+WHELP,HGARBITEM,WFEN,2*HGARBITEM,CURS2);
    zrast = w_crzon(nf_garb,WGARB+WHELP,2*HGARBITEM,WFEN,3*HGARBITEM,CURS2);
    zraz  = w_crzon(nf_garb,WGARB+WHELP,3*HGARBITEM,WFEN,4*HGARBITEM,CURS2);
         
    w_montrer(nf_garb) ;    

		/* init du tabgarb de fedchar */
    for (i=0 ; i < MAXCARGARB ; i++)      
          { 
		Rast_Init(&tabgarb[i].image,128,128,1);
		Rast_Off(&tabgarb[i].image,BackColor);
            	occupe[i] = 0 ;
          }

    idmraster = MenuCreat(choixrast);
    courant = 0 ;

    BlackColor = w_blackpixel() ;
}

static Reaffich(fen)
int fen ;
{	char s[2] ;

    if (fen == nf_garb) {
         w_cleararea(nf_garb,WGARB,0,WGARB+WHELP,HGARB);	/* help */
	 s[1] = '\0' ;
	 s[0] = 'S' ;
	 w_ecrire(nf_garb,s,WGARB+3,HGARB-40);
	 w_ecrire(nf_garb,s,WGARB+3,HGARB-10);
	 s[0] = 'O' ;
	 w_ecrire(nf_garb,s,WGARB+3,HGARB-25);		/* SOS vertical */

    	 w_line(nf_garb,WGARB,0,WGARB,HGARB,ON); 
         w_line(nf_garb,WGARB+WHELP,0,WGARB+WHELP,WGARB+WHELP,ON); 
         w_line(nf_garb,WGARB+WHELP,0,WFEN,0,ON); 	
         w_line(nf_garb,WGARB+WHELP,HGARBITEM,WFEN,HGARBITEM,ON); 
         w_line(nf_garb,WGARB+WHELP,2*HGARBITEM,WFEN,2*HGARBITEM,ON); 
         w_line(nf_garb,WGARB+WHELP,3*HGARBITEM,WFEN,3*HGARBITEM,ON); 

	 w_centrer(nf_garb,"PUT",WGARB+WHELP,0,WFEN,HGARBITEM);
	 w_centrer(nf_garb,"GET",WGARB+WHELP,HGARBITEM,WFEN,2*HGARBITEM);
	 w_centrer(nf_garb,"RASTER",WGARB+WHELP,2*HGARBITEM,WFEN,3*HGARBITEM);
	 w_centrer(nf_garb,"RAZGARB",WGARB+WHELP,3*HGARBITEM,WFEN,4*HGARBITEM);
          	
         afficher_car() ;
    }
}


/************************* procedure AFFICHER_CAR ***********************/
afficher_car()
{
	char s[2] ;
    s[0] = (char) (courant + 48);
    s[1] = '\0' ;
    w_ecrire(nf_garb,s,130,10);
    if (occupe[courant])	{  /* raster de memoire dans fenetre donne */
       	w_bitblt(nf_garb,tabgarb[courant].image,0,0,WGARB,HGARB,0,0) ;
     }
   else w_gris(nf_garb,0,0,WGARB,HGARB) ;
}


/***************************** procedure RAZGARB **************************/
/* Procedure faisant une remise a zero de la poubelle */

static Razgarb()

{ register int i ;

  if (gd_confirm())
   { for(i = 0 ; i < MAXCARGARB ; i++)
     { if (occupe[i])
       { occupe[i] = 0 ;
         Rast_Off(&tabgarb[i].image, BackColor) ;
       }
     }
     courant = 0 ;
     afficher_car() ;
   }
}


CopyCar(c1,c2) 
/* metrique seulement */
	fedcharptr c1,c2 ;
{
	strcpy(c2->name,c1->name) ;
	c2->hincr = c1->hincr ;
	c2->vincr = c1->vincr ;
	c2->hadj = c1->hadj ;
	c2->vadj = c1->vadj ;
	c2->up = c1->up ;
	c2->down = c1->down ;
	c2->hsize = c1->hsize ;
}

/***************************** procedure Put *******************************/
/* Met le caractere de travail  dans la poubelle[courant] et le montre */

static Put()
{
    if (cartrav.hsize > 0){
	CopyCar(&cartrav,&tabgarb[courant]);
	Rast_Copy(cartrav.image,&tabgarb[courant].image);
	taborx[courant] = Xleft ;
	tabory[courant] = Baseliney + cartrav.up ;

        occupe[courant] = 1 ;
        afficher_car() ;
    }
}


FontToGarb(fedc)
	fedcharptr fedc ;
{

	CopyCar(fedc,&tabgarb[courant]);
        Rast_Off(&tabgarb[courant].image,BackColor) ;
	Rast_Op_1n(fedc->image,&tabgarb[courant].image,
			0,0,0,0,
			fedc->hsize,fedc->down-fedc->up,VIDSTR,BlackColor);
	taborx[courant] = 0 ;
	tabory[courant] = 0 ;
	
        occupe[courant] = 1 ;
        afficher_car() ;
}


/***************************** procedure RASTER ***************************/
static Raster()
{
     switch(MenuPop(idmraster,choixrast)) {
           case 0: Rast(VIDOR);
                   break;
           case 1: Rast(VIDAND);
                   break;
           case 2: Rast(VIDXOR);
                   break;
           default: return;
     }
}

static Rast(mode)
	int mode ;
{	short x1,y1,x2,y2,w,h ;


        Dodo();
	x1 = Min(Xleft,taborx[courant]) ;
	x2 = Max(Xleft+cartrav.hsize,taborx[courant]+tabgarb[courant].hsize) ;
	w = x2 - x1 ;
	y1 = Min(Baseliney+cartrav.up,tabory[courant]) ;
	y2 = Max(Baseliney+cartrav.down,tabory[courant]+
		tabgarb[courant].down-tabgarb[courant].up);
	h = y2 - y1 ;
        
	Rast_Op(tabgarb[courant].image,&cartrav.image,
		x1,y1,x1,y1,w,h,mode,BackColor);

	if (w > cartrav.hsize) cartrav.hsize = w ;
	if (Xleft > taborx[courant]) 
		Xleft = taborx[courant] ;
	if (Baseliney+cartrav.up > tabory[courant])
	   cartrav.up = tabory[courant] - Baseliney ;
	if (Baseliney+cartrav.down < tabory[courant]+
			tabgarb[courant].down-tabgarb[courant].up)
	   cartrav.down = tabory[courant]+
		tabgarb[courant].down-tabgarb[courant].up - Baseliney ;

	MontrerCarTrav() ;
}


/*************************** procedure GET *****************************/   
static Get()
   
{
        Dodo();
	CopyCar(&tabgarb[courant],&cartrav) ;
	Rast_Copy(tabgarb[courant].image,&cartrav.image);
	Xleft = taborx[courant] ;
	Baseliney = (tabory[courant] - cartrav.up) ;
	if (Baseliney <0) Baseliney = 0 ;
	MontrerCarTrav();	
}

   
static int Inverser_zone(zone)
	int zone;
{
    if (zone==zput) w_inv(nf_garb,WGARB+WHELP+2,2,WFEN-2,HGARBITEM-2);else
    if (zone==zget) w_inv(nf_garb,WGARB+WHELP+2,HGARBITEM+2,WFEN-2,2*HGARBITEM-2);else
    if (zone==zrast) w_inv(nf_garb,WGARB+WHELP+2,2*HGARBITEM+2,WFEN-2,3*HGARBITEM-2);else
    if (zone==zraz) w_inv(nf_garb,WGARB+WHELP+2,3*HGARBITEM+2,WFEN-2,4*HGARBITEM-2);
}

static int Traiter_clic(evt)
	myEvent * evt ;
{	    
	if (evt->zone==zgarb) {
   		    switch(evt->click){
                      case 1 : 
			  courant -= 1 ;
                          if (courant == -1) {courant = MAXCARGARB-1;}
                          afficher_car() ;
                          break ;
                      case 2 : 
			  courant = (courant + 1) % MAXCARGARB ;
                          afficher_car() ;
                          break ;
		      default : break ;
                    }  					     
	} else
	if (evt->zone==zhelp) Helping(GARB); else
	{
	   if (evt->zone==zput) Put(); else
	   if (evt->zone==zget) Get();  else
	   if (evt->zone==zrast) Raster(); else
	   if (evt->zone==zraz) Razgarb();
 	};		    	
}

/*********************** procedure AUTOM_GARB ***************************/ 
/* Des que le controleur detecte l'entree de la souris dans la poubelle,
   il appelle cette procedure */

Autom_garb(pev) 
  myEvent * pev ;  
{
	static zonecour = 0 ;

  switch (pev->type){
	case Exposure : 
		Reaffich(pev->window) ;
		if (zonecour) Inverser_zone(zonecour);
		break ;
	case EnterZone   : 
		if (pev->zone==zgarb) Afficher_boutons("<--","-->") ; else
      		if (pev->zone==zhelp) Afficher_boutons("Help","Help") ;else
				      Afficher_boutons("Select","Select") ;
		Inverser_zone(pev->zone);
		zonecour = pev->zone ;
		break;
	case ButtonPressed :  
		Traiter_clic(pev);
           	break ;
        case LeaveZone : 
		Inverser_zone(pev->zone);
		zonecour = 0 ;
            	Afficher_boutons("","") ;
           	break ;
      }
  }
            
