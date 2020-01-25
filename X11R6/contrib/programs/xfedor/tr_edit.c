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
/* tr_edit.c */
#include <stdio.h>
#include "couche.h" 		/* myEvent */
#include "sos.h"		/* EDIT,LINE .. */
#include "edit.h"		/* MODBACK WMENU WEDIT WHITE GREY point */

extern nf_mono ;
extern editresol ;		/* Afficher_resol */
extern BackColor, CurColor ;
extern nf_colormap ;

Init_edit() ;
Autom_edit() ;
Afficher_resol() ;
Afficher_colors() ;
Afficher_sos();
Afficher_boutons();
Afficher_numview();

int nf_edit ;   /* NUMERO DE LA FENETRE EDIT */      

int modcolor ;		/* MODBACK, MODINV ou MODCOLOR */
int modepoint ; 	/* NOCONSTRAINT, XCONSTRAINT, YCONSTRAINT */
int modecour ; 		/* point, line .. */
int colortrame ;	/* WHITE or GREY  */

	/*---------- reste du LOCAL */

#define WFENED  (WMENU + WEDIT + WBASE)

#define HITEM 22
#define WICON (WMENU/2)
#define HBOUT 92
#define HCOLOR 40
#define WCOLOR (WMENU/3)
#define WIC (WMENU/3)
#define HICON 50
#define HINT (6*HITEM + HBOUT)
#define HINT0 (HINT+HCOLOR)
#define HINT1 (HINT+HCOLOR+HICON)
#define HINT2 (HINT+HCOLOR+2*HICON)
#define HINT3 (HINT+HCOLOR+3*HICON)
#define HINT4 (HINT+HCOLOR+4*HICON)
#define HINT5 (HINT+HCOLOR+5*HICON)
#define XEDIT 10 
#define YEDIT 10 


static int idmother ;
static char *choixother[] = {"Scale","TileH","TileV","MirrorH",
				"MirrorV","ReflectH","ReflectV",
				"Xconstraint","Yconstraint","NumView",
			        "NoWhite",0 } ;
static int zhelp,zpas,zback,zmouse,zundo,zother,zclear,zbout,zcolor,
	   zline,zrect,zwrite,zcercle,ztransl,zrotate,
	   zbez,zfill,zmetric,zcutap,zresize, zbase ;
			/* id des zones crees */     

static int numview ;		/* ON, OFF */


#include "bitmaps/mouse"

#include "bitmaps/line"	
#include "bitmaps/rect"	
#include "bitmaps/cercle"	
#include "bitmaps/write"	
#include "bitmaps/resize"	
#include "bitmaps/transl"	
#include "bitmaps/rotate"	
#include "bitmaps/bez"	
#include "bitmaps/fill"	
#include "bitmaps/metric"	
#include "bitmaps/cutap"	



Init_edit()
{
     nf_edit = w_ouvrir(nf_mono,XEDIT,YEDIT,WFENED,HEDIT,
		  	"XFEDOR : Edit Window",
		  	CURS2,
		  	EnterZone|ButtonPressed|LeaveZone,WMON) ;
/*     w_backing(nf_edit);*/

     zhelp = w_crzon(nf_edit,0,0,WICON,HITEM,CURS2);
     zpas = w_crzon(nf_edit,WICON,0,WMENU,HITEM,CURS2);

     zback = w_crzon(nf_edit,0,HITEM,WMENU,2*HITEM,CURS2);
     zmouse = w_crzon(nf_edit,0,2*HITEM,WMENU,3*HITEM,CURS2);
     zundo = w_crzon(nf_edit,0,3*HITEM,WMENU,4*HITEM,CURS2);
     zother = w_crzon(nf_edit,0,4*HITEM,WMENU,5*HITEM,CURS2);
     zclear = w_crzon(nf_edit,0,5*HITEM,WMENU,6*HITEM,CURS2);

     zbout = w_crzon(nf_edit,0,6*HITEM,WMENU,HINT,CURS3);
     zcolor = w_crzon(nf_edit,0,HINT,WMENU,HINT0,CURS2);

     zline = w_crzon(nf_edit,0,HINT0,WIC,HINT1,CURS1);
     zrect = w_crzon(nf_edit,WIC,HINT0,2*WIC,HINT1,CURS1);
     zcercle = w_crzon(nf_edit,2*WIC,HINT0,WMENU,HINT1,CURS1);

     zwrite = w_crzon(nf_edit,0,HINT1,WICON,HINT2,CURS1);
     zresize = w_crzon(nf_edit,WICON,HINT1,WMENU,HINT2,CURS1);

     ztransl = w_crzon(nf_edit,0,HINT2,WICON,HINT3,CURS1);
     zrotate = w_crzon(nf_edit,WICON,HINT2,WMENU,HINT3,CURS1);

     zbez = w_crzon(nf_edit,0,HINT3,WICON,HINT4,CURS1);
     zfill = w_crzon(nf_edit,WICON,HINT3,WMENU,HINT4,CURS1);
     
     zmetric = w_crzon(nf_edit,0,HINT4,WICON,HINT5,CURS1);
     zcutap = w_crzon(nf_edit,WICON,HINT4,WMENU,HINT5,CURS1);
     
     zbase = w_crzon(nf_edit,WFENED-WBASE,0,WFENED,HEDIT,CURS1); 

/* CREATION POPUP OTHER */
    idmother = MenuCreat(choixother);

/* INITIALISATION DE EDIT */
    colortrame = GREY ;
    modcolor = MODINV ;
    modecour = point ;

    modepoint = NOCONSTRAINT ;
    numview = OFF ;

    w_montrer(nf_edit);

}

static Reaffich(fen)
int fen ;
{ 
    if (fen == nf_edit)  {

	/* ligne des menus */
	  w_line(nf_edit,0,HITEM,WMENU,HITEM,ON);
	  w_line(nf_edit,0,2*HITEM,WMENU,2*HITEM,ON);
	  w_line(nf_edit,0,3*HITEM,WMENU,3*HITEM,ON);
	  w_line(nf_edit,0,4*HITEM,WMENU,4*HITEM,ON);
	  w_line(nf_edit,0,5*HITEM,WMENU,5*HITEM,ON);
	  w_line(nf_edit,0,6*HITEM,WMENU,6*HITEM,ON);
			/* horizontale des menus */
	  w_line(nf_edit,0,HINT,WMENU,HINT,ON);
	  w_line(nf_edit,0,HINT0,WMENU,HINT0,ON);
			/* horizontales intermediaires */
	  w_line(nf_edit,WCOLOR,HINT,WCOLOR,HINT0,ON);
	  w_line(nf_edit,2*WCOLOR,HINT,2*WCOLOR,HINT0,ON);
			/* verticales des colors */
	  w_line(nf_edit,WICON,0,WICON,HITEM,ON);
			/* verticales zones help */

	/* labels */
	  w_centrer(nf_edit,"BACK",0,HITEM,WMENU,2*HITEM);
	  w_centrer(nf_edit,"MOUSE",0,2*HITEM,WMENU,3*HITEM);
	  w_centrer(nf_edit,"UNDO",0,3*HITEM,WMENU,4*HITEM);
	  w_centrer(nf_edit,"OTHER",0,4*HITEM,WMENU,5*HITEM);
	  w_centrer(nf_edit,"CLEAR",0,5*HITEM,WMENU,6*HITEM);
	  w_centrer(nf_edit,"SOS",0,0,WICON,HITEM);
	  
	/* Afficher_boutons */
	  w_gris(nf_edit,1,6*HITEM+1,WMENU-1,HINT-1);
	  w_line(nf_edit,23,HINT-23,23,6*HITEM+32,ON);
	  w_line(nf_edit,WMENU-25,HINT-23,WMENU-25,6*HITEM+62,ON);

	  w_line(nf_edit,23,HINT-23,WMENU/2-4,HINT-23,ON);
	  w_line(nf_edit,WMENU/2+1,HINT-23,WMENU-25,HINT-23,ON);
	  w_line(nf_edit,WMENU/2-4,HINT-23,WMENU/2-4,HINT-18,ON);
	  w_line(nf_edit,WMENU/2+1,HINT-23,WMENU/2+1,HINT-18,ON);
	  w_pixel(nf_edit,mouse_bits,WMENU/2-6,HINT-18,13,13);

	/* dessin d'icones */
	  w_pixel(nf_edit,line_bits,3,HINT0+2,31,46);
	  w_pixel(nf_edit,rect_bits,WIC+2,HINT0+2,31,46);
	  w_pixel(nf_edit,cercle_bits,2*WIC+2,HINT0+2,31,46);

	  w_pixel(nf_edit,write_bits,3,HINT1+2,46,46);
	  w_pixel(nf_edit,resize_bits,WICON+2,HINT1+2,46,46);
	  w_pixel(nf_edit,transl_bits,3,HINT2+2,46,46);
	  w_pixel(nf_edit,rotate_bits,WICON+2,HINT2+2,46,46);
	  w_pixel(nf_edit,bez_bits,3,HINT3+2,46,46);
	  w_pixel(nf_edit,fill_bits,WICON+2,HINT3+2,46,46);
	  w_pixel(nf_edit,metric_bits,3,HINT4+2,46,46);
	  w_pixel(nf_edit,cutap_bits,WICON+2,HINT4+2,46,46);


    	Afficher_colors() ;
    	Afficher_resol(editresol) ;

	Afficher_boutons(NULL,NULL); /* reaffichage force' */
	Afficher_modcolor(ON);
	Afficher_modecour() ;

    }
}

Afficher_colors()      /* pas static pour tr_colormap */
{
	/* zone choix de couleurs */
	  w_color(nf_edit,7,HINT+9,WCOLOR-8,HINT0-10,BackColor);
	  w_rect(nf_edit,7,HINT+9,WCOLOR-8,HINT0-10,ON);

	  w_color(nf_edit,WCOLOR+8,HINT+9,2*WCOLOR-8,HINT0-10,BackColor);
	  w_color(nf_edit,WCOLOR+WCOLOR/2,HINT+9,
		            2*WCOLOR-8,HINT0-10,CurColor);
	  w_rect(nf_edit,WCOLOR+8,HINT+9,2*WCOLOR-8,HINT0-10,ON);

	  w_color(nf_edit,2*WCOLOR+8,HINT+9,3*WCOLOR-7,HINT0-10,CurColor);
	  w_rect(nf_edit,2*WCOLOR+8,HINT+9,3*WCOLOR-7,HINT0-10,ON);
}


static Afficher_modcolor(onoff)
	int onoff ;
{ 
	int xoffset ;

	if (modcolor == MODBACK) xoffset = 0 ; else
	if (modcolor == MODINV) xoffset = WCOLOR+1 ; else
	      xoffset = 2*WCOLOR+1 ; 

	if (onoff == ON) {
		w_fore(nf_edit,xoffset+3,HINT+3,xoffset+WCOLOR-4,HINT+7);
		w_fore(nf_edit,xoffset+3,HINT+3,xoffset+6,HINT0-3);
		w_fore(nf_edit,xoffset+3,HINT0-7,xoffset+WCOLOR-4,HINT0-3);
		w_fore(nf_edit,xoffset+WCOLOR-6,HINT+3,xoffset+WCOLOR-3,HINT0-3);
	} else {
		w_cleararea(nf_edit,xoffset+3,HINT+3,xoffset+WCOLOR-4,HINT+7);
		w_cleararea(nf_edit,xoffset+3,HINT+3,xoffset+6,HINT0-3);
		w_cleararea(nf_edit,xoffset+3,HINT0-7,xoffset+WCOLOR-4,HINT0-3);
		w_cleararea(nf_edit,xoffset+WCOLOR-6,HINT+3,xoffset+WCOLOR-3,HINT0-3);
	}
}

static Afficher_modecour()
{
	switch(modecour) {
	   case line  : w_inv(nf_edit,4,HINT0+3,WIC,HINT1-4); break ;
	   case recta  : w_inv(nf_edit,WIC+3,HINT0+3,2*WIC-1,HINT1-4); break ;
	   case cercle  : w_inv(nf_edit,2*WIC+3,HINT0+3,WMENU-2,HINT1-4); break ;
	   case write  : w_inv(nf_edit,4,HINT1+3,WICON-2,HINT2-4); break ;
	   case resize  : w_inv(nf_edit,WICON+3,HINT1+3,WMENU-3,HINT2-4); break ;
	   case transl  : w_inv(nf_edit,4,HINT2+3,WICON-2,HINT3-4); break ;
	   case rotate  : w_inv(nf_edit,WICON+3,HINT2+3,WMENU-3,
					HINT3-4); break ;
	   case bez  : w_inv(nf_edit,4,HINT3+3,WICON-2,
	   				HINT4-4); break ;
	   case fil  : w_inv(nf_edit,WICON+3,HINT3+3,WMENU-3,
			        	HINT4-4); break ;
	   case metric  : w_inv(nf_edit,4,HINT4+3,WICON-2,
	   				HINT5-4); break ;
	   case cutap  : w_inv(nf_edit,WICON+3,HINT4+3,WMENU-3,
			        	HINT5-4); break ;
	   default : break ;
        }
}

Afficher_resol(resolcour)
	int resolcour ;
{	char s[4] ;
	sprintf(s,"%3d",resolcour);
	w_centrer(nf_edit,s,WICON,0,WMENU,HITEM);
}
	
static Changer_resol(click)
   int click ;

 { int resol ;
	
   switch(click) {
       case 1 :  if (editresol==16) return ;
		  resol = editresol >> 1;
		  if (Resol_possible(resol))
		   {
			editresol = resol ;
		  	Afficher_resol(editresol) ;
		  	Afficher_all() ;	/* grille+baseline+car */
		   }
	          break ;
       case 2 :   if (editresol==128) return ;
		  editresol = editresol << 1;
		  Afficher_resol(editresol) ;
		  Afficher_all() ;	/* grille+baseline+car */
	          break ;
     } 
} 



Afficher_sos(s)
	char * s ;
{
	  w_centrer(nf_edit,s,0,0,WICON,HITEM);
}

Afficher_boutons(gauche,milieu)
     char * gauche, * milieu;
{	
	static char oldgauche[20], oldmilieu[20];


	if (gauche == NULL) {
		w_cleararea(nf_edit,8,6*HITEM+20,70,6*HITEM+35);
		w_ecrire(nf_edit,oldgauche,10,6*HITEM+30);
		w_cleararea(nf_edit,WMENU-65,6*HITEM+47,WMENU-8,6*HITEM+62);
		w_ecrire(nf_edit,oldmilieu,WMENU-62,6*HITEM+57);
		return ;
	}
	if (strcmp(gauche,oldgauche)) {
		w_cleararea(nf_edit,8,6*HITEM+20,70,6*HITEM+35);
		w_ecrire(nf_edit,gauche,10,6*HITEM+30);
		strcpy(oldgauche,gauche) ; 
	}
	if (strcmp(milieu,oldmilieu)) {
		w_cleararea(nf_edit,WMENU-65,6*HITEM+47,WMENU-8,6*HITEM+62);
		w_ecrire(nf_edit,milieu,WMENU-62,6*HITEM+57);
		strcpy(oldmilieu,milieu) ;
	}
}

Afficher_numview(x,y)
/* x= -1 reinitialise le graphique et le flag prem */
	int x,y;
{	char s[5];
	static int prem = 1 ;
	
	if (numview == OFF) return ;
	if (x==-1) {
		w_gris(nf_edit,1,6*HITEM+1,WMENU-1,6*HITEM+18);
		prem = 1 ;
		return ;
	}
	if (prem == 1) {
		w_cleararea(nf_edit,2,6*HITEM+2,40,6*HITEM+17);
		w_rect(nf_edit,2,6*HITEM+2,40,6*HITEM+17,ON);
		w_cleararea(nf_edit,WMENU-40,6*HITEM+2,WMENU-2,6*HITEM+17);
		w_rect(nf_edit,WMENU-40,6*HITEM+2,WMENU-2,6*HITEM+17,ON);
		prem = 0 ;
	}
	sprintf(s,"x %3d",x);
	w_ecrire(nf_edit,s,4,6*HITEM+14);
	sprintf(s,"y %3d",y);
	w_ecrire(nf_edit,s,WMENU-38,6*HITEM+14);
}	

 
static int Inverser_zone(zone)
	int zone;
{
	if (zone==zback ) w_inv(nf_edit,3,HITEM+3,WMENU-3,2*HITEM-3);else
	if (zone==zmouse ) w_inv(nf_edit,3,2*HITEM+3,WMENU-3,
					3*HITEM-3);else
	if (zone==zundo ) w_inv(nf_edit,3,3*HITEM+3,WMENU-3,
					4*HITEM-3);else
	if (zone==zother ) w_inv(nf_edit,3,4*HITEM+3,WMENU-3,
					5*HITEM-3);else
	if (zone==zclear ) w_inv(nf_edit,3,5*HITEM+3,WMENU-3,
					6*HITEM-3);
}

static int Traiter_clic(evt)
	myEvent * evt ;
{
   int numx,denx,numy,deny ;

   if (evt->zone==zhelp) 
		switch(modecour) {
		   case point : Helping( EDIT ); break ;
		   case line  : Helping( LINE ); break ;
		   case recta  : Helping( RECT ); break ;
		   case cercle  : Helping( CIRCLE ); break ;
		   case resize  : Helping( RESIZE ); break ;
		   case write  : Helping( WRITE ); break ;
		   case transl  : Helping( TRANSL ); break ;
		   case bez  : Helping( BEZIERS ); break ;
		   case rotate  : Helping( ROTATE ); break ;
		   case fil  : Helping( FILL ); break ;
		   case metric : Helping( METRIC ); break ;
		   case cutap : Helping( CUTAP ); break ;
		   default : Helping( EDIT ); break ;
               } else
	if (evt->zone==zclear) Clear(evt->click) ; else
	if (evt->zone==zbase) {
		if (evt->click == 1) Changer_baseline(evt->y) ; else
			Changer_guideline(evt->y) ;
		} else
	if (evt->zone==zundo) Undo() ; else
	if (evt->zone==zback) Back() ; else
	if (evt->zone==zmouse) Mouse() ; else
	if (evt->zone==zbout) {
		if (evt->click == 1) {
				Afficher_modecour() ;
				modecour = point ;
				modepoint = NOCONSTRAINT ;
		}
		else {
			colortrame = (colortrame==WHITE)?GREY:WHITE ;
			Afficher_all() ;
		}
	    } else
	if (evt->zone==zpas) Changer_resol(evt->click) ; else
	if (evt->zone==zother) 
		switch (MenuPop(idmother,choixother)) {
		case 0 : 	
		  if (w_nosaveunder()) Reaffich(nf_edit) ;  /* HACK */
		  gd_askscale(&numx,&denx,&numy,&deny) ;
		  Magnify(numx,denx,numy,deny);
		  break ;
	       case 1 : Tileh() ; break ;
	       case 2 : Tilev() ; break ;
	       case 3 : Mirrorh() ; break ;
	       case 4 : Mirrorv() ; break ;
	       case 5 : Reflecth() ; break ;
	       case 6 : Reflectv() ; break ;
	       case 7 : modepoint = XCONSTRAINT ; break ;
	       case 8 : modepoint = YCONSTRAINT ; break ;
	       case 9 : numview = (numview==OFF)?ON:OFF ; break ;
	       case 10: NoWhite() ; break ;
		  default : break ;
	       } else
	if (evt->zone==zcolor) {
		if (evt->click == 1) {
		  Afficher_modcolor(OFF) ;
		  if (evt->x < WCOLOR) modcolor = MODBACK ; else
		    if (evt->x < 2*WCOLOR) modcolor = MODINV; else
		      modcolor = MODCOLOR ;
		  Afficher_modcolor(ON);
		} else {
		  w_montrer(nf_colormap) ;
		}
	} else {	/* zone = line .. fill */
		Afficher_modecour() ;
		if (evt->zone == zline) modecour = line ; else
		if (evt->zone == zrect) modecour = recta ; else
		if (evt->zone == zcercle) modecour = cercle  ; else
		if (evt->zone == zresize) modecour = resize  ; else
		if (evt->zone == zwrite) modecour = write  ; else
		if (evt->zone == ztransl) modecour = transl ; else
		if (evt->zone == zbez) modecour = bez ; else
		if (evt->zone == zrotate) modecour = rotate ; else
		if (evt->zone == zfill) modecour = fil ; else
		if (evt->zone == zmetric) modecour = metric ; else
		if (evt->zone == zcutap) modecour = cutap ;
		Afficher_modecour() ;
	}	
}

Autom_edit(pev)
	myEvent * pev ;
{
	static zonecour = 0 ;

     switch (pev->type){
	case Exposure :
		Reaffich(pev->window) ;
		if (zonecour) Inverser_zone(zonecour);
		break ;
	case EnterZone   : 
	  if (pev->zone==zhelp) Afficher_boutons("Help","Help") ;else
	  if (pev->zone==zpas)  Afficher_boutons("<--","-->") ;else
	  if (pev->zone==zbout) Afficher_boutons("abort","gridcolor") ;else
          if (pev->zone==zbase) Afficher_boutons("baseline","guideline");else 
          if (pev->zone==zcolor) Afficher_boutons("Select","colormap") ;else
 	  if (pev->zone==zclear) {
	    Afficher_boutons("car","back") ;
	    Inverser_zone(pev->zone);
	    zonecour = pev->zone ;
	  } else {
	    Afficher_boutons("Select","Select") ;
	    Inverser_zone(pev->zone);
	    zonecour = pev->zone ;
	  }
	  break;
	case ButtonPressed :  
		Traiter_clic(pev);
           	break ;
        case LeaveZone : 
		Inverser_zone(pev->zone);
		zonecour = 0 ;
        	break ;
      }		/* fin automate du menu */
}



