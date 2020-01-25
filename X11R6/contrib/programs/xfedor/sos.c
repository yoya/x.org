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
/* sos.c */
        /* module de traitement du sos  */
        
#include <stdio.h>
#include "couche.h"
extern char * getenv() ;
extern int UnixFileNameMode ;     /* 0 ou 1, initialiser dans main */

extern int root ;

int  nf_sos ;

#include "sos.h"	/* PUT GET .. */

#define WFIC (8*(80+2))
#define HFIC (12*(22+2))

static char * dirsos ;
static char * newdirsos ;

static char Prompt[20], Sosfile[50] ;

void Init_sos()
{

	if ((dirsos = getenv("SOS"))==0)
  	     if (UnixFileNameMode) dirsos = "/usr/local/lib/X11/xfedor" ; 
	                      else dirsos = "";
	if (UnixFileNameMode) {
	   newdirsos = (char *)malloc(strlen(dirsos)+2);
	   strcpy(newdirsos,dirsos);
	   newdirsos[strlen(dirsos)] = '/' ;
	   newdirsos[strlen(dirsos)+1] = '\0' ;
	   dirsos = newdirsos ;
	}

	nf_sos = w_ouvrir(root, 140, 200, WFIC, HFIC,
			  "xfedor_sos",CURS3,
			  EnterWindow|ButtonPressed|LeaveWindow,
			  WMON);
}

void Helping(code)
    int code ;
{   
    strcpy(Sosfile,dirsos);
    switch(code){
	case GARB : strcpy(Prompt,"Commandes GARB");
		    strcat(Sosfile,"GARB.sos");
		   break ;
	case FONT : strcpy(Prompt,"Fenetre FONTE");
		   strcat(Sosfile,"FONT.sos");
		   break ;
	case SAVE : strcpy(Prompt,"Fenetre SAVE");
		   strcat(Sosfile,"SAVE.sos");
		   break ;
	case EDIT : strcpy(Prompt,"Fenetre EDIT");
		   strcat(Sosfile,"EDIT.sos");
		   break ;
	case LINE : strcpy(Prompt,"Mode LINE");
		   strcat(Sosfile,"LINE.sos");
		   break ;
	case WRITE : strcpy(Prompt,"Mode WRITE");
		   strcat(Sosfile,"WRITE.sos");
		   break ;
	case METRIC : strcpy(Prompt,"Mode METRIC");
		   strcat(Sosfile,"METRIC.sos");
		   break ;
	case CUTAP : strcpy(Prompt,"Mode CUTAP");
		   strcat(Sosfile,"CUTAP.sos");
		   break ;
	case CIRCLE : strcpy(Prompt,"Mode CIRCLE ");
		   strcat(Sosfile,"CIRCLE.sos");
		   break ;
	case RECT : strcpy(Prompt,"Mode RECTANGLE");
		   strcat(Sosfile,"RECTANGLE.sos");
		   break ;
	case TRANSL : strcpy(Prompt,"Mode TRANSLATE");
		   strcat(Sosfile,"TRANSLATE.sos");
		   break ;
	case ROTATE : strcpy(Prompt,"Mode ROTATE");
		   strcat(Sosfile,"ROTATE.sos");
		   break ;
	case FILL : strcpy(Prompt,"Mode FILL");
		   strcat(Sosfile,"FILL.sos");
		   break ;
	case BEZIERS : strcpy(Prompt,"Mode BEZIERS");
		   strcat(Sosfile,"BEZIERS.sos");
		   break ;
	case RESIZE : strcpy(Prompt,"Mode RESIZE");
		   strcat(Sosfile,"RESIZE.sos");
		   break ;
	case SOS : strcpy(Prompt,"Commandes generales");
		   strcat(Sosfile,"SOS.sos");
		   break ;
	case NUM : strcpy(Prompt,"Fenetre Numerique");
		   strcat(Sosfile,"NUM.sos");
		   break ;
	default : strcpy(Prompt,"Commande default");
		   strcat(Sosfile,"default.sos");
		   break ;
    }
    w_cacher(nf_sos) ;
    w_montrer(nf_sos) ;

}

/*---------------------------------------*/
Reaffich(fen)
int fen ;

{ 	register FILE *fptr;
	myEvent ev;
	int x,y,i ;
	char s[80] ;

	if ((fptr = fopen(Sosfile,"r")) != NULL)
	    {
		w_centrer(fen,Prompt,0,0,WFIC,20);
		w_inv(fen,0,0,WFIC,20);
    		x = 7;
   		y = 42 ;	/* unites pixels */
    
    		while( fgets(s,80,fptr) != 0) {
			  for (i=0; s[i]; i++) ;
			  s[i-1] = ' ';
      		          w_ecrire(fen,s,x,y);
		        y += 14;
	        }
    		fclose(fptr);
         } else w_centrer(fen,"CAN'T OPEN HELP FILE.",0,0,WFIC,HFIC);
	w_centrer(fen,"Click anywhere to UnMap",0,HFIC-20,WFIC,HFIC);
	w_inv(fen,0,HFIC-20,WFIC,HFIC);
}


int Autom_sos(pev)
	myEvent * pev ;
{
   switch(pev->type){
	case Exposure :
		Reaffich(pev->window);
		break ;
	case EnterWindow   : 
                Afficher_boutons("Exit","Exit") ;
		break;   
	case ButtonPressed :  
	    	w_cacher(pev->window);	    
            	break ;
       case LeaveWindow :	
		Afficher_boutons("","") ; 
		break ;
   }
}


