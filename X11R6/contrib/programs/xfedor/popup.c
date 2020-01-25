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
/* popup.c */
#include <stdio.h>
#include 	"couche.h"	/* CURS */

extern int root ;

MenuCreat() ;
int MenuPop() ;

#define POPBODY 16		/* Hack */
#define BAR 10

/*----------------------MENUCREAT----------------------------*/
MenuCreat (choix)
	char * choix[] ;
/* cree la fenetre et les zones calculees a partir du tableau de chaines */
/* rend l'id de la fenetre */
{

	int maxl=0 ;
	int idm,	/* id de la fenetre du menu */
	    i,j ;	/* compteur sur le tableau choix */

		/* on calcule maxl = largeur max des choix[]  */

	for (i=0; choix[i]>0 ; i++) 
		maxl = Max(w_strwidth(choix[i]),maxl) ;
	
	idm = w_ouvrir(root,0, 0, maxl+10, (i)*POPBODY + BAR, "",CURS1,	
			LeaveWindow|EnterZone|ButtonReleased|LeaveZone,WMOFF) ;

	for (j=0 ; j<i; j++) 
		w_crzon(idm,0,(j)*POPBODY+BAR,maxl+10,(j+1)*POPBODY+BAR,CURS1);

	w_crzon(idm,0,0,maxl+10,BAR,CURS1);
	return idm ;
}


/*-----------------------------MENU_POP---------------------*/
int MenuPop (idm,choix)
	int idm ;
	char * choix[] ;
/* idm est un id fenetre W */
/* choix est le tableau de chaines a afficher */
	     
{	int x,y,i ;
	myEvent evt ;
	static int Largmenu ;  /* calculee par MenuPop une fois */


	w_pop(idm) ;   /* montre la fenetre et place la souris dedans
			  gere les depassements hors ecran */

	Largmenu = w_largeur(idm) ; /* utilise par Inverser() */

	  /* on affiche les choix */
	for (i=0 ; choix[i]>0 ; i++) 
		w_centrer (idm,choix[i],0,(i)*POPBODY+BAR,
			   Largmenu,(i+1)*POPBODY-3+BAR);

	w_gris(idm,0,0,Largmenu,BAR-1);

     do {
	w_getevent (&evt) ;          /* on attend de rentrer dans le menu */
     } while (evt.window != idm) ;  /* sur car on met la souris dedans */

     do {
	w_getevent (&evt) ;	/* attente sur le process 
				   evt.pzone est l'origine de la zone
				   ou bien {0,0} */
	if (evt.window != idm) continue ;
	switch(evt.type) {
	case EnterZone :
	case LeaveZone :
	    if (evt.yz != 0)
	      w_inv(idm,2,evt.yz +2,Largmenu -2, evt.yz + POPBODY -2);
				               /* allume ou eteint */
	  break ;
	case ButtonReleased :
	  w_cacher(idm);
	  if (evt.yz != 0) return ((evt.yz - BAR) / POPBODY) ;
	    else return -1 ;
	case LeaveWindow :
	  w_cacher(idm);
	  return -1 ;
	}
      } while (1) ;
/* rends le choix (0..n) ou -1 */
}



	   
