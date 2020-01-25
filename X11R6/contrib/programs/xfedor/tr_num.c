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
/* tr_num.c */
#include <stdio.h>
#include "couche.h"		/* myEvent */
#include "clientimage.h"
#include "fedor.h"
#include "sos.h"	  	/* constante NUM */

extern nf_mono ;
extern fedchar cartrav ;	/* depuis tr_edit */

extern int maxup, maxdown, maxhsize, maxhincr, maxhadj ;
extern char codemaxup, codemaxdown, codemaxhsize, codemaxhincr, codemaxhadj ;


Init_num();
Aff_all() ;
Aff_max() ;
Aff_name();
Autom_helpnum () ;
Autom_num();

int nf_num ;    /* NUMERO DE LA FENETRE tableau numerique */
int nf_helpnum ;  /* NUMERO DE LA FENETRE help numerique */

	/*--------- reste du LOCAL */

static char * lib [] = {"UP","DOWN","HSIZE","HINCR","HADJ","NAME"};
#define RangUp 1 
#define RangDown 2
#define RangHsize 3
#define RangHincr 4 
#define RangHadj 5 
#define RangName 6


static int zhelp, zname,zhincr,zhadj ; 

#define NbItem 6	/* voir lib */
#define HNUMITEM 23
#define HNUM ((NbItem+1)*HNUMITEM)	/* +1 pour l'entete */

#define WNUMITEM 55
#define WCHAR 87
#define WFONT 98
#define WNUM (WNUMITEM+WCHAR+WFONT)  /* 3 colonnes : field, char et font */

#define XNUM (10 + 100+514+12 + 10 + 2)
#define YNUM (10 + 22 + 2*22+130 + 2) 


Init_num ()   

{    int i,y ;
 
     	 nf_num = w_ouvrir(nf_mono,XNUM, YNUM, WNUM, HNUM,
		  	"XFEDOR : Metric",CURS2, 
		  	EnterZone|ButtonPressed|LeaveZone,WMON) ;
	 zhelp = w_crzon(nf_num,0,0,WNUM,HNUMITEM,CURS2);
	 zhincr = w_crzon(nf_num,WNUMITEM,RangHincr*HNUMITEM,
			(WNUMITEM+WCHAR),(RangHincr+1)*HNUMITEM,CURS2);
	 zhadj = w_crzon(nf_num,WNUMITEM,RangHadj*HNUMITEM,
			(WNUMITEM+WCHAR),(RangHadj+1)*HNUMITEM,CURS2);
	 zname = w_crzon(nf_num,WNUMITEM,RangName*HNUMITEM,
			(WNUMITEM+WCHAR+WFONT),(RangName+1)*HNUMITEM,CURS2);
         w_montrer(nf_num);    

	 /* on cree ici la fenetre help_num */
	 nf_helpnum = w_ouvrir(nf_mono,XNUM, YNUM, WNUM, HNUM , 
		  	"HelpMetric",CURS2,ButtonPressed,WMON);
}

static Reaffich(fen)
int fen ;
{   int i,y ;
    if (fen == nf_num) {
         w_clear(nf_num);
	 for (i=1,y=HNUMITEM;i<= NbItem ;i++,y+=HNUMITEM) {
		 w_line(nf_num,0,y,WNUM,y,ON);
		 w_centrer(nf_num,lib[i-1],0,y,WNUMITEM,y+HNUMITEM);
	 } 
	 w_line(nf_num,WNUMITEM,0,WNUMITEM,HNUM,ON);
	 w_line(nf_num,WNUMITEM+WCHAR,0,WNUMITEM+WCHAR,HNUM-HNUMITEM,ON);

	 w_centrer(nf_num,"SOS",0,0,WNUMITEM,HNUMITEM);
	 w_centrer(nf_num,"CHAR",WNUMITEM,0,WNUMITEM+WCHAR,HNUMITEM);
	 w_centrer(nf_num,"FONT",WNUMITEM+WCHAR,0,WNUM,HNUMITEM);

	 Aff_all() ;
	 Aff_name(cartrav.name) ;
	 Aff_max() ;
    } else
    if (fen == nf_helpnum) {	
	   w_line(nf_helpnum,0,HNUM/2,WNUM,  HNUM/2,ON);	/* baseline */
	   w_rect(nf_helpnum,WNUM/3,HNUM/5,2*WNUM/3,4*HNUM/5,ON);  /*  BBox */

	   w_line(nf_helpnum,WNUM-30,HNUM/2 -2,WNUM-30,HNUM/2 +5,ON);
	   w_line(nf_helpnum,WNUM-30,HNUM -10,WNUM-30,HNUM,ON);
					/* marque hincr sur baseline */

	   w_ecrire(nf_helpnum,"<-HADJ->",30,HNUM/2-10);
	   w_ecrire(nf_helpnum,"<-HSIZE->",WNUM/3+20,20);
	   w_ecrire(nf_helpnum,"<--HINCR-->",WNUM/3-10,HNUM-10);
	   w_ecrire(nf_helpnum,"Baseline",WNUM*2/3+20,HNUM/2-5);
	   w_ecrire(nf_helpnum,"Up",WNUM*2/3+3,HNUM/5);
	   w_ecrire(nf_helpnum,"Down",WNUM*2/3+3,HNUM*4/5);
	   w_fellips(nf_helpnum,WNUM/3+2,HNUM/5+2,WNUM*2/3-2,HNUM/2-2,ON);
	   w_fore(nf_helpnum,WNUM/3+2,HNUM/5+2,WNUM/3+30,HNUM*4/5-2);
    }
}    


static int Inverser_zone(zone)
	int zone;
{
    if (zone==zhincr) w_inv(nf_num,WNUMITEM+2,RangHincr*HNUMITEM+2,
		WNUMITEM+WCHAR-2,(RangHincr+1)*HNUMITEM-2); else
    if (zone==zhadj) w_inv(nf_num,WNUMITEM+2,RangHadj*HNUMITEM+2,
		WNUMITEM+WCHAR-2,(RangHadj+1)*HNUMITEM-2); else
    if (zone==zname) w_inv(nf_num,WNUMITEM+2,RangName*HNUMITEM+2,
		WNUMITEM+WCHAR+WFONT-2,(RangName+1)*HNUMITEM-2); 
}	

static int Traiter_clic(evt)
	myEvent * evt ;
{	char s[50] ;
	long lf ;	/* en virgule fixe */

	if (evt->zone==zhelp) {
		 if (evt->x <= WNUMITEM) Helping(NUM); 
		  else	 w_montrer(nf_helpnum); 
	         } else
        if (evt->zone==zhincr) {
		 gd_lireentier("Value for Hincr ?",&lf);
		 cartrav.hincr = lf ;
		 Aff_item(cartrav.hincr,RangHincr); 
		}else
        if (evt->zone==zhadj) {
		 gd_lireentier("Value for Hadj ?",&lf);
		 cartrav.hadj = lf;
		 Aff_item(cartrav.hadj,RangHadj) ;
	      }else
        if (evt->zone==zname) {
	   gd_lirechaine("Name for Glyph ?",cartrav.name);
	   Aff_name(cartrav.name);
	}	
}

Aff_all()
{
	Aff_item(cartrav.hsize,RangHsize); 
	Aff_item(cartrav.up,RangUp) ;
	Aff_item(cartrav.down,RangDown) ;
	Aff_item(cartrav.hincr,RangHincr); 
	Aff_item(cartrav.hadj,RangHadj) ;
}

Aff_max() 
{
	AffMax_item(codemaxup,maxup,RangUp);
	AffMax_item(codemaxdown,maxdown,RangDown);
	AffMax_item(codemaxhsize,maxhsize,RangHsize);
	AffMax_item(codemaxhincr,maxhincr,RangHincr);
	AffMax_item(codemaxhadj,maxhadj,RangHadj);
	
}

static int Aff_item(val,rang)
	int val ;
	int rang ;
{   char s [10];
    sprintf(s,"%d",val);
    w_centrer(nf_num,s,WNUMITEM,rang*HNUMITEM,
			WNUMITEM+WCHAR,(rang+1)*HNUMITEM);
}
	
static int AffMax_item(c,val,rang)
	char c ;
	int val ;
	int rang ;
{   char s [10];
    sprintf(s,"%d  %c",val,c);
    w_centrer(nf_num,s,WNUMITEM+WCHAR,rang*HNUMITEM,
			WNUM,(rang+1)*HNUMITEM);
}
	
Aff_name(name)
	char * name;
{   
  w_cleararea(nf_num,WNUMITEM+1,HNUM-HNUMITEM+1,WNUM-1,HNUM-1);
  w_ecrire(nf_num,name,WNUMITEM+5,HNUM-HNUMITEM/3);
}
	

/************************ procedure AUTOM_NUM ***************************/ 
/* Des que le controleur detecte l'entree de la souris dans le tableau
   numerique, il appelle cette procedure */

Autom_num (pev) 
   myEvent * pev ;
{
	static zonecour = 0 ;
    switch(pev->type){
	case Exposure : 
		Reaffich(pev->window) ;
		if (zonecour) Inverser_zone(zonecour);
		break ;
	case EnterZone : 
		if (pev->zone == zhelp) Afficher_boutons("Help","Help") ;else
				        Afficher_boutons("Select","Select") ;
		Inverser_zone(pev->zone);
		zonecour = pev->zone ;
		break ;
	case ButtonPressed : 
		Traiter_clic(pev);
		break ;
	case LeaveZone :
                   Afficher_boutons("","") ;
		   Inverser_zone(pev->zone);
		   zonecour = 0 ;
		   break ;
      }
  }		

/************************ procedure AUTOM_HELPNUM ***************************/ 
/* si la fenetre helpnum est visible, elle peut recevoir l'evenement
	ButtonPressed qui la cache */

Autom_helpnum (pev) 
   myEvent * pev ;
{
   switch(pev->type){
	case Exposure :
		Reaffich(pev->window);
		break ;
	case ButtonPressed : 
		w_cacher(nf_helpnum);
		break ;
	default : break ;
      }
}		

