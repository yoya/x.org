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
/* tr_tabbord.c */
#include <stdio.h>
#include "clientimage.h" /* pour fedor.h */
#include "couche.h" 	/* CURS2 */
#include "sos.h"	/* SOS .. */
#include "fedor.h"	/* fedchar */

extern root,nf_mono ;
extern int FIN ;		/* depuis main.c */

extern fedchar cartrav ;		/* CarFen */
extern int Xleft, Baseliney ;	

extern int nf_extrafont ;   /* depuis tr_save */
extern int nf_save ;   /* depuis tr_save */

Autom_tabbord();
Init_tabbord();
CarFen() ;
CarPoint() ;

int nf_tabbord ;   /* NUMERO DE LA FENETRE TABLEAU DE BORD */      

	/*--------- le reste est du LOCAL */

static int idmload,idbitmap,idmtest ;	/* id de popup */
static char *choixtest[] = {"MOUSE","CHARS", "BITMAP", 0 } ;
static char *choixload[] = {"FONT","EXTRA", 0 } ;
static char *choixbitmap[] = {"LOAD MOUSE","SAVE MOUSE",
			      "LOAD BITMAP","SAVE BITMAP",
			      "LOAD PIXMAP","SAVE PIXMAP",0 } ;

static int zsos,zquit,ztest,zload,zsave,zbitmap,zcar ;
			/* id des zones crees */     
#include "tabbord.h"

#define XTAB (10 + 100+514+12 + 10 + 2)
#define YTAB 10

#define XIC 900
#define YIC 170 
#define WIC 81
#define HIC 100


static int invert = 0 ;

static char mousename[50] ;
static char bitmapname[50] ;
static char pixmapname[50] ;


/*----------------------------------------------------------*/
int Init_tabbord(pixfile,mousefile,bitfile)
     char * pixfile, * mousefile, * bitfile ;
{
     
    nf_tabbord = w_ouvrir(nf_mono,XTAB, YTAB, WTAB, HTABB, 
		  	"XFEDOR : Control",CURS2,
		  	EnterZone|ButtonPressed|LeaveZone,WMON) ;
	
    zquit = w_crzon(nf_tabbord,0,0,WTABITEM,HTABITEM,CURS2);
    zsos = w_crzon(nf_tabbord,WTABITEM,0,2*WTABITEM,HTABITEM,CURS2);
    ztest = w_crzon(nf_tabbord,2*WTABITEM,0,WTAB,HTABITEM,CURS2);
    zload = w_crzon(nf_tabbord,0,HTABITEM,WTABITEM,HTAB,CURS2);
    zsave = w_crzon(nf_tabbord,WTABITEM,HTABITEM,2*WTABITEM,HTAB,CURS2);
    zbitmap = w_crzon(nf_tabbord,2*WTABITEM,HTABITEM,WTAB,HTAB,CURS2);

    zcar = w_crzon(nf_tabbord,0,HTAB,WCAR,HTABB,CURS3);

    w_montrer(nf_tabbord) ; 
	 
	 /* creation des popups du tableau de bord */
    idmtest = MenuCreat(choixtest);
    idmload = MenuCreat(choixload);
    idbitmap = MenuCreat(choixbitmap);
     
    strcpy(pixmapname,pixfile);
    strcpy(mousename,mousefile);
    strcpy(bitmapname,bitfile);
}

static Reaffich(fen)
int fen ;
{
     if (fen == nf_tabbord) {
         w_line(nf_tabbord,0,HTABITEM,WTAB,HTABITEM,ON);
         w_line(nf_tabbord,0,HTAB,WTAB,HTAB,ON);
         w_line(nf_tabbord,WTABITEM,0,WTABITEM,HTAB,ON);
         w_line(nf_tabbord,2*WTABITEM,0,2*WTABITEM,HTAB,ON);

	 w_centrer(nf_tabbord,"QUIT",0,0,WTABITEM,HTABITEM);
	 w_centrer(nf_tabbord,"SOS",WTABITEM,0,2*WTABITEM,HTABITEM);
	 w_centrer(nf_tabbord,"TEST",2*WTABITEM,0,WTAB,HTABITEM);
	 w_centrer(nf_tabbord,"LOAD",0,HTABITEM,WTABITEM,HTAB);
	 w_centrer(nf_tabbord,"SAVE",WTABITEM,HTABITEM,2*WTABITEM,HTAB);
	 w_centrer(nf_tabbord,"BITMAP",2*WTABITEM,HTABITEM,WTAB,HTAB);

	 CarFen() ;
     } 
}

CarFen()
{
	w_bitblt(nf_tabbord,cartrav.image,
			0,0,128,128,1,HTAB+2) ;
}

Carpoint(x,y,color)
	int x,y ;
	int color ;
{
	   w_point(nf_tabbord,x+1,HTAB+1+y+1,color) ;
}

static InvertCar()
{
	invert = (invert==0)?1:0 ;
	w_inv(nf_tabbord,1,HTAB+2,WCAR-1,HTABB) ;
}

static int Inverser_zone(zone)
	int zone;
{
	if (zone==zquit) 
		w_inv(nf_tabbord,3,3,WTABITEM-3,HTABITEM-3);  else
	if (zone==zsos ) 
		w_inv(nf_tabbord,WTABITEM+3,3,2*WTABITEM-3,HTABITEM-3); else
	if (zone== ztest) 
		w_inv(nf_tabbord,2*WTABITEM+3,3,WTAB-3,HTABITEM-3); else
	if (zone== zload ) 
		w_inv(nf_tabbord,3,HTABITEM+3,WTABITEM-3,HTAB-3); else
	if (zone== zsave)
		w_inv(nf_tabbord,WTABITEM+3,HTABITEM+3,2*WTABITEM-3,HTAB-3); else
	if (zone== zbitmap)
		w_inv(nf_tabbord,2*WTABITEM+3,HTABITEM+3,WTAB-3,HTAB-3); 
			
}
	

static int Load()
{
     char name[50];
     int rep ;

	rep = MenuPop(idmload,choixload) ;
	if (w_nosaveunder()) Reaffich(nf_tabbord) ;		/* HACK */
	name[0] = '\0' ;   /* pour gd_lirechaine ++ */
	switch(rep) {
	   case 0 : 
		gd_lirechaine("Filename ?",name);
		rep = GetFont(name) ;
		if (rep<0) gd_envoyermsg("CAN'T READ FONT FILE");
		break ;
	   case 1 :
		gd_lirechaine("Filename ?",name);
		rep = GetExtraFont(name) ;
		if (rep == 0) w_cacher(nf_extrafont);
		if (rep > 0) w_montrer(nf_extrafont); else
		if (rep < 0) gd_envoyermsg("CAN'T READ FONT FILE");
		break ;
	   default : break ;
	}
}

static int MenuTest()
{
     int rep ;

	rep = MenuPop(idmtest,choixtest) ;
	switch(rep) {
	   case 0 : 
                Test(1) ;           /* equiv a click == 1 */
		break ;
	   case 1 :
                Test(2) ;           /* equiv a click == 2 */
		break ;
	   case 2 :
                w_setroot(cartrav.image,
			  Xleft,Baseliney+cartrav.up,
			  cartrav.hsize,cartrav.down-cartrav.up);
		      /* equiv a xsetroot -bitmap */
		break ;
	   default : break ;
	}
}

static int Bitmap()
{     
  int rep ;

	rep = MenuPop(idbitmap,choixbitmap) ;
	if (w_nosaveunder()) Reaffich(nf_tabbord);	/* HACK */
	switch(rep) {
	   case 0 :
		gd_lirechaine("File (Fore+Mask) ?",mousename);
		if (LoadBitmap(mousename,1) != 1)
		  gd_envoyermsg("CAN'T READ THE MOUSE FILES ");
		else  MontrerCarTrav();
		break ;
	   case 1 :
		gd_lirechaine("File (Fore+Mask) ?",mousename);
		SaveBitmap(mousename,1);
		break ;
	   case 2 :
		gd_lirechaine("BitmapFile ?",bitmapname);
		if (LoadBitmap(bitmapname,0) != 1) 
		  gd_envoyermsg("CAN'T READ THE BITMAP FILE ");
		else  MontrerCarTrav();
		break ;
	   case 3 :
		gd_lirechaine("BitmapFile ?",bitmapname);
		SaveBitmap(bitmapname,0);
		break ;
	   case 4 :
		gd_lirechaine("PixmapFile ?",pixmapname);
		if (LoadBitmap(pixmapname,2) != 1)
		  gd_envoyermsg("CAN'T READ THE PIXMAP FILE ");
		else MontrerCarTrav();
		break ;
	   case 5 :
		gd_lirechaine("PixmapFile ?",pixmapname);
		SaveBitmap(pixmapname,2);
		break ;
	   default : break ;
	}
}


int Autom_tabbord(pev)
	myEvent * pev ;
{
	static zonecour = 0 ;
   switch(pev->type){
	case Exposure : 
		Reaffich(pev->window) ;
		if (zonecour) Inverser_zone(zonecour);
		break ;
	case EnterZone   : 
                if (pev->zone==zcar) Afficher_boutons("Invert","Invert") ; else
			Afficher_boutons("Select","Select");
		Inverser_zone(pev->zone);
		zonecour = pev->zone ;
		break;   
	case ButtonPressed :  
	    if (pev->zone==zsos) Helping(SOS); else
	    if (pev->zone==zquit) {
	       if (gd_confirm()) {
		  FIN = 1 ;
	       }
	    } else 
	    if (pev->zone==ztest) MenuTest(); else
	    if (pev->zone==zload)  Load(); else
     	    if (pev->zone==zbitmap) Bitmap(); else
  	    if (pev->zone==zsave)  {
	      w_cacher(nf_save);
	      w_montrer(nf_save);
	    } else
	    if (pev->zone==zcar)  InvertCar();
            break ;
       case LeaveZone : 
		Inverser_zone(pev->zone); 
		zonecour = 0 ;
		if ((pev->zone == zcar) && invert)
			InvertCar() ;
		Afficher_boutons("","") ; 
		break ;
   }
 }


