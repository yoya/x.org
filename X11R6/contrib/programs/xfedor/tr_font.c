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
/* tr_font.c */
#include <stdio.h>
#include "clientimage.h"  /* pour fedor.h et VID  */
#include "couche.h"	  /*  CURS2 , myEvent */
#include "sos.h"	  /* FONT .. */
#include "fedor.h"	  /* fedchar .. MAXFONTCARD */

extern nf_mono ;
extern fedchar cartrav ;	/* depuis tr_edit */
extern int Baseliney,Xleft ;	/* depuis tr_edit */
extern int BackColor ;

Init_font();
ShowDoIt() ;
FontMax_Resize() ;
New_Resol() ;
Autom_font();
      
int nf_font ;   /* NUMERO DE LA FENETRE FONTE */      

fedchar chartab[MAXFONTCARD];	/* pour filer GetFont */

int maxup, maxdown, maxhsize,  maxhincr, maxhadj ;
char codemaxup, codemaxdown, codemaxhsize, codemaxhincr, codemaxhadj ;

int First, Last ;	/* pour otherfont */
int Prem ;

	/*------------ reste du LOCAL */

#define HMENU 22

#include "font.h"

#define WHELP 45
#define WASCII (WHELP+(2*WITEM-WHELP)/2)

#define WITEM (WFONT / 9) 
/* pourquoi 9 : 
   |SOS| ASCII 48 a | Command : ITALIC | First : 12 | Last : 100 | DOIT | 
        1     2          3        4       5       6     7     8     9
*/

#define XFONT 0
#define YFONT (10+514+10)


static int zfont,zhelp,zleft,zright,zlift,
	zascii,zmode,zfirst,zlast,zdoit ; /* id des zones crees */

static int idmode ;
static char * choixmode[]= {"Put/Get","Kill","Animate","Bold",
				"Slant","Scale",0};
static int idanim ;
static char * choixanim[]= {"Slow","Normal","Quick",0};

static int Mode ;	/* modifie par le popup */

#define putget 0
#define kill 1
#define anim 2 		/* pour le switch de DoIt() */
#define bold 3
#define slant 4
#define scale 5


static int lifton = 0 ; 	/* flag ascenseur en cours */

static int Dern  ;

static int Resol ; 	/* 16,32,64,ou 128 : donne la taille de 
			la fenetre fonte. est modifie par ShowFont ou Put */
static codeputget ;	/* code du dernier car select */

static int codecour, codeprec ;/* avec codeputget global, permettent la 
			 	gestion du tracking code ascii */
static hfont ;	/* hauteur fenetre */


/************************* procedure INIT_FONT ***************************/
/* Procedure d'initialisation du module  */
/* la fenetre est cree sans sa zone fonte (ou ya les chars) */
 
Init_font () 

{ 
  int i ;

    nf_font = w_ouvrir(nf_mono,XFONT,YFONT,WFONT,HFONTMAX,
		  	"XFEDOR : Font Window",CURS2,
		  	EnterZone|ButtonPressed|ButtonReleased|
		  	MoveMouse|LeaveZone,WMON) ;

    zascii = w_crzon(nf_font,WHELP,0,2*WITEM,HMENU,CURS2);
    zmode = w_crzon(nf_font,2*WITEM,0,4*WITEM,HMENU,CURS2);
    zfirst = w_crzon(nf_font,4*WITEM,0,6*WITEM,HMENU,CURS2);
    zlast = w_crzon(nf_font,6*WITEM,0,8*WITEM,HMENU,CURS2);
    zdoit = w_crzon(nf_font,8*WITEM,0,WFONT,HMENU,CURS2);

    zhelp = w_crzon(nf_font,0,0,WHELP,HMENU,CURS2);
	
    zleft = w_crzon(nf_font,0,HMENU,WSCROLL,HFONT,CURS2);
    zlift = w_crzon(nf_font,WSCROLL,HMENU,WFONT-WSCROLL,HFONT,CURS2);
    zright = w_crzon(nf_font,WFONT-WSCROLL,HMENU,WFONT,HFONT,CURS2);

    zfont =  w_crzon(nf_font,0,HFONT,WFONT,HFONTMAX,CURS1);

    Mode = putget ;
    First = 0 ; Last = MAXFONTCARD ;
    Prem = 64 ;

    for (i=0 ; i<MAXFONTCARD; i++) {
      chartab[i].hsize = 0 ;
    }

    w_montrer(nf_font) ;

    idmode = MenuCreat(choixmode);
    idanim = MenuCreat(choixanim);
}

static Reaffich(pev)
	myEvent * pev ;

{  	int code, code1, code2 ;

    if (pev->window == nf_font) {

	 w_centrer(nf_font,"Ascii ",WHELP,0,WASCII,HMENU);
	 w_centrer(nf_font,"Command :",2*WITEM,0,3*WITEM,HMENU);
	 w_centrer(nf_font,"First :",4*WITEM,0,5*WITEM,HMENU);
	 w_centrer(nf_font,"Last :",6*WITEM,0,7*WITEM,HMENU);
	 w_centrer(nf_font,"DO IT",8*WITEM,0,WFONT,HMENU);
	 
	 w_line(nf_font,WHELP,0,WHELP,HMENU,ON);
	 w_centrer(nf_font,"SOS",0,0,WHELP,HMENU);

	 w_line(nf_font,2*WITEM,0,2*WITEM,HMENU,ON);
	 w_line(nf_font,4*WITEM,0,4*WITEM,HMENU,ON);
	 w_line(nf_font,6*WITEM,0,6*WITEM,HMENU,ON);
 	 w_line(nf_font,8*WITEM,0,8*WITEM,HMENU,ON); 
					/* barre vertic menu */

         w_line(nf_font,0,HMENU,WFONT,HMENU,ON); 	
					/* barre horiz sous menu */
	 w_line(nf_font,WSCROLL,HMENU,WSCROLL,HFONT,ON);
						/* vertic zone left */
	 w_line(nf_font,WFONT-WSCROLL,HMENU,WFONT-WSCROLL,HFONT,ON);
							/*right*/
	 ShowArrow(nf_font,HFONT) ;
	 	
	 code1 = Prem + pev->x/Resol ;
	 code2 = Prem + (pev->x+pev->xz)/Resol ;
	 if ((pev->y+pev->yz) > HFONT)
	    for (code=code1; code<=code2; code++) 
		ShowAChar(nf_font,chartab,code,Prem,Dern,
				Resol,HFONT,hfont,maxup);
         ShowLift(nf_font,ON,Prem,Dern,HFONT) ;
	 Aff_mode() ;
	 Aff_first() ; 
	 Aff_last() ;
    }
}    

ShowDoIt(First,Last)
	int First, Last ;
{	int res ;
	int code ;

      res = FontMax_Resize() ;
	/* res = 0 si il n'y a aucune modif de taille dans la police 
	   res = 1 si la resolution horizontale doit changer ou la hauteur
	            diminue
	   res = 2 si la taille augmente (=> un expose genere) */
	
      if (res == 0) 
	  for (code=First; code<=Last; code++) 
		ShowAChar(nf_font,chartab,code,Prem,Dern,
				Resol,HFONT,hfont,maxup);
       else
      if (res == 1) 
	 ShowAFont(nf_font,chartab,Prem,Dern,
		   Resol,HFONT,hfont,maxup) ;
}
   
int FontMax_Resize()  

{
	int code, res ;
	static old_hfont, old_Resol=32 ;

	maxup = maxdown = maxhsize = maxhincr = maxhadj = 0 ;

	for (code=0; code < MAXFONTCARD;code++) 
	   if (chartab[code].hsize>0) {
		if (chartab[code].up < maxup ) 
			{ maxup = chartab[code].up ;	/* up NEGATIF */
			  codemaxup = code ;}
		if (chartab[code].down > maxdown ) 
			{ maxdown = chartab[code].down ;
			  codemaxdown = code ;}
		if (chartab[code].hsize > maxhsize ) 
			{ maxhsize = chartab[code].hsize;
			  codemaxhsize = code ;}
		if (chartab[code].hincr > maxhincr ) 
			{ maxhincr = chartab[code].hincr ;
			  codemaxhincr = code ;}
		if (chartab[code].hadj > maxhadj ) 
			{ maxhadj = chartab[code].hadj ;
			  codemaxhadj = code ;}
	}
	Aff_max() ;

	hfont = (((maxdown-maxup)>0)?(maxdown-maxup):32) + 1;
	Resol = New_Resol((maxhsize>0)?maxhsize:32);

 	if (hfont == old_hfont) {
		if (Resol == old_Resol) res = 0 ; 
		else {
			old_Resol = Resol ;
			ShowLift(nf_font,OFF,Prem,Dern,HFONT) ;
			res = 1 ;
		}
	 } else {
		w_resize(nf_font,WFONT,HFONT+hfont);
		w_resize(nf_mono,
			 10+100+514+12+10+240+10,       /* Hack */
			 10+514+10+22+25+hfont);
		if (hfont > old_hfont) res = 2 ; /* un expose genere */
		else res = 1 ;      /* pas d'expose car bit_gravity */
		old_hfont = hfont ;
		old_Resol = Resol ;
	}		
	Dern = Prem + WFONT / Resol - 1 ;
	return res ;
}


int New_Resol(val) 
	int val ;
/* rends la resolution entrainee par cette val : 16, 32, 64, 128 */
{
	if (val <= 16) return 16 ; else
	if (val <= 32) return 32 ; else
	if (val <= 64) return 64 ; else return 128 ;
}

static PutToFont(code)
	int code ;
{

	if ((code < 0) || (code >= MAXFONTCARD)) return ;
	if (chartab[code].hsize > 0 ) {
		FontToGarb(&chartab[code]); 
		Rast_Free(&chartab[code].image) ;
	}
	Rast_Init(&chartab[code].image,
		  cartrav.hsize,cartrav.down-cartrav.up,0); 
	                                      /* flag = 0 pour depth 1 */
	Rast_Op_n1(cartrav.image,&chartab[code].image,
			Xleft,Baseliney+cartrav.up,
			0,0,
			cartrav.hsize,cartrav.down-cartrav.up,
		        VIDSTR,BackColor);
	CopyCar(&cartrav,&chartab[code]);	/* metrique */
	/* on veut tester si le nom du caractere est valable */
	if (*chartab[code].name == '\0')	
            if ((code <33) || (code >126))
		sprintf(chartab[code].name,"C%d",code) ; else
		sprintf(chartab[code].name,"%c",code) ;

	ShowDoIt(code,code);
	codeputget = code ;
}


static Aff_code(code)
	int code ;
{ char s[20] ;
	if (code != -1) {
		sprintf(s,"%d %c",code,code);
		w_centrer(nf_font,s,WASCII,0,2*WITEM,HMENU);
	} else w_centrer(nf_font,"",WASCII,0,2*WITEM,HMENU);
}

static Aff_mode() 
/* affiche le champs Mode dans la fenetre */
{ 
	w_centrer(nf_font,choixmode[Mode],3*WITEM,0,4*WITEM,HMENU);
}

static Aff_first()
/* affiche la valeur du first : debut de traitement sur char */
{ char s[20] ;
	sprintf(s,"%d",First);
	w_centrer(nf_font,s,5*WITEM,0,6*WITEM,HMENU);
}

static Aff_last()
/* affiche la valeur du Last : fin de traitement sur char */
{ char s[20] ;
	sprintf(s,"%d",Last);
	w_centrer(nf_font,s,7*WITEM,0,8*WITEM,HMENU);
}


static DoIt()
/* First et Last sont connus.
   Mode est connu. */
{
	int rep ;
	static int delay = 0 ;
	int numx,denx,numy,deny ;

    switch (Mode) {
	case putget : 
		    First = 0 ;
		    Last = MAXFONTCARD-1 ;
		    Aff_first() ;
		    Aff_last();
		    break ;
	case slant : Slant();
		      break ;	
	case bold   : Bold();
		      break ;
	case kill   : Kill(); 
		      break ;
	case anim   : 
		rep = MenuPop(idanim,choixanim) ;
		if (w_nosaveunder()) 
		  { myEvent e ;
			e.window = nf_font ;
			e.x = WFONT - 2*WITEM ;
			e.y = 0 ;
			e.xz = 2*WITEM ;
			e.yz = HFONTMAX ;
			Reaffich(&e) ;		/* HACK OF THE SHAME */
		}
		switch(rep) {
		   case -1 : break ;
		   case 0 : delay = 50 ; break ;
		   case 1 : delay = 5 ; break ;
		   case 2 : delay = 0 ; break ;
		}
		Anim(delay) ;
		break ;
	case scale  : 
	  gd_askscale(&numx,&denx,&numy,&deny) ;
	  Scale(numx,denx,numy,deny);
          break ;
	default     : break ;
    }
}

		 

static int Traiter_zone(evt)
/* on a recu un click dans la zone */
	myEvent * evt ;
{
	int i ;

	if ((evt->zone == zleft) && (evt->click == 1))
		Scroll(nf_font,chartab,LEFT,&Prem,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zleft) && (evt->click == 2))
		Scroll(nf_font,chartab,RIGHT,&Prem,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zright) && (evt->click == 1))
		Scroll(nf_font,chartab,RIGHT,&Prem,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zright) && (evt->click == 2))
		Scroll(nf_font,chartab,LEFT,&Prem,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if (evt->zone==zhelp) Helping(FONT) ; else
	if (evt->zone==zmode) {
		i = MenuPop(idmode,choixmode) ; /* 1,2..6 */
		if (i != -1) { 
			Mode = i ;
			Aff_mode();
		}
	} else
	if (evt->zone==zfirst) {
		gd_lireentier("Value for First ?",&First);
		First = (First < 0)? 0 : First ;
		First = (First > MAXFONTCARD)? MAXFONTCARD : First ;
		Aff_first() ; 
	} else
	if (evt->zone==zlast) {
		gd_lireentier("Value for Last ?",&Last);
		Last = (Last < 0)? 0 : Last ;
		Last = (Last > MAXFONTCARD)? MAXFONTCARD : Last ;
		Aff_last() ;
	} else		
	if (evt->zone==zdoit) DoIt() ; else
	if (evt->zone==zlift) {
			lifton = 1 ;
			Shadow(nf_font,evt->x,Prem,Dern,HFONT);
	} else 
	if (evt->zone==zfont) {   
	  if (Mode == putget ) {  /* pas de mode courant => PUT et GET */
	     switch (evt->click){
		case 1 : if (cartrav.hsize > 0)
				PutToFont(Prem + evt->x/Resol) ;
			 break ;
		case 2 : GetFromaFont(chartab,Prem + evt->x/Resol); 
			 break ;
		default : break ;
	     }
	  } else
	     switch (evt->click) {
		case 1 : 
		  i = evt->x / Resol + Prem ;
		  if ((i >= 0) || (i < MAXFONTCARD)) First = i ;
		  Aff_first() ;
		  break ;
		case 2 :
		  i = evt->x / Resol + Prem ;
		  if ((i >= 0) || (i < MAXFONTCARD)) Last = i ;
		  Aff_last() ;
		  break ;
		default : break ;
	     }
	}
}



static int Inverser_zone(zone)
	int zone;
{
    if (zone==zmode) w_inv(nf_font,2*WITEM+2,2,3*WITEM-2,HMENU-2);
					    else
    if (zone==zfirst) w_inv(nf_font,4*WITEM+2,2,5*WITEM-2,HMENU-2);
					    else
    if (zone==zlast) w_inv(nf_font,6*WITEM+2,2,7*WITEM-2,HMENU-2);
					    else
    if (zone==zdoit) w_inv(nf_font,8*WITEM+2,2,WFONT-2,HMENU-2);
}	

/*********************** procedure AUTOM_FONT ***************************/ 
/* Des que le controleur detecte l'entree de la souris dans la fenetre,
   il appelle cette procedure */

Autom_font(pev) 
  myEvent * pev ;  
{ int code ;	/* pour reaffichage apres scroll ascenseur */

  static zonecour = 0 ;
  int xshadow ;

  switch (pev->type){
	case Exposure :
		Reaffich(pev);
		if (zonecour) Inverser_zone(zonecour);
		if (lifton) Shadow(nf_font,-1,Prem,Dern,HFONT);
		break ;
	case EnterZone   : 
	  if ((pev->zone==zfont)&&(Mode==putget))
	    Afficher_boutons("PUT","GET") ; else
	  if ((pev->zone==zfont)&&(Mode!=putget)) 
	    Afficher_boutons("First","Last") ; else
      	  if (pev->zone==zhelp) Afficher_boutons("Help","Help") ; else
          if (pev->zone == zlift) {
	    Afficher_boutons("Select","Select"); 
	    lifton = 1 ;
	    Shadow(nf_font,pev->x,Prem,Dern,HFONT);
	  } else
	  if (pev->zone==zfont) codeprec = codeputget ; else
	    Afficher_boutons("Select","Select"); 

	  Inverser_zone(pev->zone);
	  zonecour = pev->zone ;
	  break;
	case ButtonPressed :  
	  if ((pev->zone != zlift) || (lifton == 0)){
	    Traiter_zone(pev);
	    break ;
	  }    /* no break : meme effet que le release ... SHAME on CODE */
	case ButtonReleased :
	  if ((pev->zone==zlift)&&(lifton)) { 
	    xshadow = Shadow(nf_font,-1,Prem,Dern,HFONT);
	    lifton = 0 ;
	    ShowLift(nf_font,OFF,Prem,Dern,HFONT) ;
	    Prem = ((xshadow-WSCROLL) * MAXFONTCARD) / WLIFT ;
	    Dern = Prem + WFONT/Resol - 1;
	    if (Dern >= MAXFONTCARD) Dern = MAXFONTCARD -1 ;
	    for (code=Prem ; code <= Dern; code++) 
	      ShowAChar(nf_font,chartab,code,Prem,Dern,
			Resol,HFONT,hfont,maxup);
	    ShowLift(nf_font,ON,Prem,Dern,HFONT) ;
	  }
	  break ;		
   	case MoveMouse :
		if ((pev->zone==zlift)&&(lifton)) {
			Shadow(nf_font,-1,Prem,Dern,HFONT);
			Shadow(nf_font,pev->x,Prem,Dern,HFONT);
		}
		if (pev->zone==zfont){
			codecour = Prem + (pev->x/Resol) ;
			if ((codeprec != codecour) && (codecour < MAXFONTCARD)) {
				Aff_code(codecour);
				codeprec = codecour ;
			}
		}		/* gestion track code ascii */	
		break ;
        case LeaveZone : 
		Inverser_zone(pev->zone);
		zonecour = 0 ;
		if ((pev->zone==zlift)&&(lifton)) {
			Shadow(nf_font,-1,Prem,Dern,HFONT);
			lifton = 0 ;
		}
		if (pev->zone==zfont) Aff_code(codeputget);
		Afficher_boutons("","");
		break ;
      }
}
       
