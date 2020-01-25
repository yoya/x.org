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
/* dialogue.c */
#include <stdio.h>              
#include "couche.h"	  /* CURS */

extern root,nf_mono ;

int gd_confirm() ;
gd_envoyermsg() ;
gd_lireentier() ;
gd_lirechaine() ;
gd_askscale() ;

static int nf_scan ;		/* id fenetre question */
static int nf_msg ;             /* id fenetre message  */

static int idmconfirm;       /* idf du menu oui-non */
static char * choixconfirm[] = {"YES",0};

#define WSCAN 250 
#define HENT 20
#define HLEC 30
#define HSCAN (HENT + HLEC)

#define WMSG 600
#define HMSG 200

Init_dialogue()
{
	idmconfirm = MenuCreat(choixconfirm);

	nf_scan = w_ouvrir(root, 0, 0, WSCAN, HSCAN, 
			   "XFEDOR : Scan Window",CURS3,
			   KeyPressed|ButtonPressed,WMOFF);

	nf_msg  = w_ouvrir(nf_mono, 140, 200, WMSG, HMSG, 
			   "XFEDOR : Message Window",CURS3,
			   KeyPressed|ButtonPressed,WMOFF);

}

/*---------------------------------------*/
int gd_confirm()
{
	return((MenuPop(idmconfirm,choixconfirm)==0));
}


/*---------------------------------------------------------*/
gd_envoyermsg(s)
register char * s;
{
	w_montrer(nf_msg);
	w_centrer(nf_msg,"ATTENTION",0,0,WMSG,20);
	w_inv(nf_msg,0,0,WMSG,20);
	w_centrer(nf_msg,s,0,20,WMSG,HMSG);

	w_flush() ;
	sleep(2) ;

	w_cacher(nf_msg);
}


/*-------------------------------------------------------*/
static wscan(prompt, format, scanned)
     char * prompt;
     int format ;       /* 0 = string, 1 = int */
     char * scanned;

/* prompt est le titre de la fenetre saisie 
   format le type de la variable lue 
   scanned un pointeur sur la variable lue et afficher */

{
		int x, y ;
		register char c;
		register int strlg;
		char buffer[256];
		char * cutbuf ;
		myEvent evt;
		int i ;

    w_pop(nf_scan) ;   /* montre la fenetre et place la souris dedans
		      gere les depassements hors ecran */  

    w_centrer(nf_scan,prompt,0,0,WSCAN,HENT);
    w_inv(nf_scan,0,0,WSCAN,HENT);
    w_setfocus(nf_scan);

    for (i=0 ; i< 255 ; i++) buffer[i] = '\0' ;
    if (format == 0) strcpy(buffer,scanned);
    strlg = strlen(buffer) ;

    do {
      w_centrer(nf_scan, buffer, 4, HENT, WSCAN, HSCAN);
      w_getevent(&evt);
      if (evt.window != nf_scan) continue ;
      if ((evt.type == ButtonPressed) && (evt.click == 2)){
	cutbuf = (char *) w_cutbuf() ;
	while ((c = *cutbuf++) && (c!='\r')) {
	  if ((c >= 32) && (strlg < 49)) {
	    buffer[strlg++] = c;
	    buffer[strlg] = '\0' ;
	  }
	}
      }  else
      if (evt.type == ButtonPressed) break ; 
         else
      if (evt.type == KeyPressed) {
	c = evt.key;
	if ((c == '\b') && (strlg > 0)) buffer[--strlg] = '\0' ;
	  else
	if (c == 11) {
	  strlg = 0 ;
	  for (i=0 ; i<256 ; i++) buffer[i] = '\0' ;
	} else
	if ((c >= 32) && (strlg < 49)) {
	    buffer[strlg++] = c;
	    buffer[strlg] = '\0' ;
	  }
      }
     } while ((c!='\r')&&(strlg<255));
     w_cacher(nf_scan);
     w_clear(nf_scan);
     if (format == 0) 
       strcpy(scanned,buffer) ; else
     if (format == 1) 
       if ((buffer[0] < '0') || (buffer[0] > '9'))
	 *(int*)scanned = 0 ; else
	   sscanf(buffer, "%d", scanned) ;
}


gd_lirechaine(prompt,ch) 
	char * prompt ;
	char * ch ;
{
	wscan(prompt,0,ch) ;
}

gd_lireentier(prompt,i) 
	char * prompt ;
	int * i ;
{
	wscan(prompt,1,i) ;
}

	
gd_askscale(numx,denx,numy,deny)
   int *numx,*denx,*numy,*deny ;
{
   char scale[50] ;
   int ns,i ;

   scale[0] = '\0' ;
   gd_lirechaine("syntax : N [D [Ny Dy]]",scale) ;
   ns = sscanf(scale,"%2d%2d%2d%2d",numx,denx,numy,deny);
   if (ns == 1) {
      *denx = 1 ;
      *numy = *numx ;
      *deny = 1 ;
   } else
   if (ns == 2) {
      *numy = *numx ;
      *deny = *denx ;
   } else 
   if (ns == 3) {
      *deny = 1 ;
   } 
      
   if (*numx <= 0) *numx = 1 ;
   if (*denx <= 0) *denx = 1 ;
   if (*numy <= 0) *numy = 1 ;
   if (*deny <= 0) *deny = 1 ;
}
