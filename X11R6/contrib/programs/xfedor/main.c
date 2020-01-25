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
/* main.c */
#include "stdio.h"
extern int strcmp() ;

#include "couche.h"
#include "edit.h"      /* WMENU, WEDIT, WBASE, HEDIT */
#include "tabbord.h"   /* WTAB */
#define HMENU 22
#include "font.h"      /* HFONT */

extern int BackColor ;

/*************************** VARIABLES IMPORTEES ************************/

extern int nf_font ;
extern int nf_garb ;
extern int nf_edit ;
extern int nf_tabbord ;
extern int nf_num ;
extern int nf_grille ;
extern int nf_helpnum ;
extern int nf_save ;
extern int nf_test ;
extern int nf_cars ;
extern int nf_extrafont ;
extern int nf_sos ;
extern int nf_colormap ;

/************************************************************************/

int FIN ;		/* booleen de terminaison */
int root,nf_mono ; 	/* fenetre mere de l'application */

int xori = 10, yori = 10;

int UnixFileNameMode = 1 ;     /* 0 ou 1, utiliser dans filer,bitmap et sos */

/*****************************************************************/

Init_fedor(reverse,display,
	   fontfile,bitfile,mousefile,pixfile,
	   argc,argv,geom,imaform,font1,font2)
	int reverse ;
	char * display ;
	char * fontfile ;
	char * bitfile ;
	char * mousefile ;
	char * pixfile ;
	int argc;
	char ** argv ;
        char * geom ;
        int imaform ;        
        char * font1 ;
        char * font2 ;

{
	root = w_initial(reverse,display,font1,font2,
			 argc,argv,geom,imaform);
				
	nf_mono = w_ouvrir (root,xori, yori,
			    10+WMENU+WEDIT+WBASE+10+WTAB+10,
			    10+HEDIT+10+HFONT,
			    "xfedor_font_editor",CURS2,0,WMON) ;

	Init_Rast(BackColor) ;
	Init_dialogue();     /* creation popup confirm + nf_msg + nf_scan */
	Init_bitmap();       /* getenv MAP */
	Init_sos() ;         /* getenv SOS + fenetre nf_sos */
	Init_filer() ;       /* getenv BDF */

	Init_edit();         /* creatmap nf_edit */
	Init_grille();       /* creatmap nf_grille + 3+nundo getimage */
	Init_tabbord(pixfile,mousefile,bitfile);  /* creatmap nf_tabbord */
	Init_test();         /* creat    nf_test */
	Init_num();	     /* creatmap nf_num */
	Init_garb();         /* creatmap nf_garb */
	Init_save();         /* creatmap nf_save */
	Init_extrafont() ;   /* creat    nf_extra */
	Init_font();         /* creatmap nf_font */
	Init_colormap();     /* creatmap nf_colormap */
	
	if (GetFont(fontfile) != 1) {
		fprintf(stderr,"Fedor : Can't read font : %s\n",fontfile);
		GetFont("initfont");
	}

	LoadBitmap(pixfile,2) ;
	LoadBitmap(mousefile,1);
	LoadBitmap(bitfile,0);

	MontrerCarTrav() ;

	w_montrer(nf_mono);
}

/*****************************************************************/
Autom_General(evt)  
/* dispatching d'un evenement sur les diverses fenetres */
	myEvent * evt;
{
  if (evt->window == nf_tabbord)  	Autom_tabbord(evt); else
  if (evt->window == nf_garb)   	Autom_garb(evt); else
  if (evt->window == nf_num)            Autom_num(evt); else
  if (evt->window == nf_helpnum) 	Autom_helpnum(evt); else
  if (evt->window == nf_font) 		Autom_font(evt); else
  if (evt->window == nf_edit) 		Autom_edit(evt); else
  if (evt->window == nf_save) 		Autom_save(evt); else
  if (evt->window == nf_test) 		Autom_test(evt); else  
  if (evt->window == nf_cars) 		Autom_cars(evt); else
  if (evt->window == nf_extrafont) 	Autom_extrafont(evt); else
  if (evt->window == nf_sos) 	        Autom_sos(evt); else
  if (evt->window == nf_grille) 	Autom_grille(evt); else
  if (evt->window == nf_colormap) 	Autom_colormap(evt); 
}

Dispatch()
/* Lecture des evenements .
   verif de la grammaire avec correction possible
   envoi a l'automate general qui aiguille vers les modules
   jusqu'a FIN */
{	myEvent evt ;
	static int zonecour = -1 ;
	static int fencour = -1 ;
	
	FIN = 0 ;
	do {
	     attente(&evt);
		/* PHASE DE CORRECTION SUR EVENEMENTS INCOHERENTS */
	     if (evt.type==EnterZone) {
		  if (zonecour == evt.zone) continue /* -> w_attente */;
				/* INUTILE DE REENTRER DANS LA MEME ZONE */
		  if (zonecour != -1) { int z,f ;
				/* ON RENTRE DANS UNE NOUVELLE ZONE ALORS
				   QU'ON N'EST PAS SORTI DE LA COURANTE */
			  z = evt.zone ;
			  f = evt.window ;	  /* memorise l'evt */
			  evt.window = fencour ;
			  evt.type = LeaveZone ; /* on quitte */
			  evt.zone = zonecour ;   /* proprement la */
			  Autom_General(&evt);	  /* zone courante */
			  evt.zone = z ;
			  evt.window = f ;
			  evt.type = EnterZone ; /* restaure l'evt */
		   }
		   zonecour = evt.zone ;
		   fencour = evt.window ; 	  /* MISE A JOUR ETAT */
	     } else
	     if (evt.type==LeaveZone) {
		   if (zonecour != evt.zone) {
				/* ON QUITTE UNE ZONE SANS Y ETRE RENTRER */
			  evt.zone = zonecour ;
			  evt.window = fencour ;  /* on quitte d'abord */
			  Autom_General(&evt);	  /* la zone courante */
			  zonecour = -1 ;
			  fencour = -1 ;
			  continue ;		  /* ignore le leave */
		   }
		   zonecour = -1 ;
		   fencour = -1 ;  		  /* MISE A JOUR ETAT */
	     } 
	     Autom_General(&evt);
	} while (!FIN) ;
}

int Usage() 
{
	  fprintf(stderr,"\n\
Usage : xfedor [-f fontfile] [-m mousefile] [-b bitmapfile] [-p pixmapfile]\n") ; 
	  fprintf(stderr,"\
               [-z] [-xy] [-f1 fontname] [-f2 fontname]\n");
	  fprintf(stderr,"\
               [-g +x+y] [-d host:display.screen]\n") ;
          fprintf(stderr,"\
        fontfile   : BDF font file (.bdf appended if not present)\n");
          fprintf(stderr,"\
        bitmapfile : X11 bitmap file (.xbm is first search)\n");
          fprintf(stderr,"\
        mousefile  : 2 X11 bitmap files (mousefileFore & Mask)\n");
          fprintf(stderr,"\
        pixmapfile : XPM file format (.xpm is first search)\n");
	  fprintf(stderr,"\
        -z         : specify ZPixmap image transfert with X Server\n");
	  fprintf(stderr,"\
        -xy        : specify XYPixmap image transfert with X Server\n");
	  fprintf(stderr,"\
        -f1        : specify font to be use in general interface\n");
	  fprintf(stderr,"\
        -f2        : specify font to be use in mouse guide lines\n");
	exit(0) ;
}


/*****************************************************************/
main(argc,argv)
int argc; 
char **argv ;

{
	int reverse = 0 ;
	char * display = NULL ;
	char * fontfile = "initfont" ;
	char * bitfile = "" ;
	char * mousefile = "" ;
	char * pixfile = "" ;
	char * geom = NULL ;
	int bidon ;
	char ** command ;
	int imaform = 0 ;       /* XYPixmap = 0 , ZPixmap = 1 */
	char * font1 = 
	  "-adobe-times-bold-r-normal--14-140-75-75-p-77-iso8859-1";
	char * font2 = 
	  "-adobe-helvetica-medium-r-normal--10-100-75-75-p-56-iso8859-1" ;

	printf("XFEDOR (daniel) v_color_xpm3 5.0 07/91\n");


	command = argv ;
	while (*++argv) {
		if (!strcmp(*argv,"-d")) 
			if (*++argv) display = *argv ; else Usage(); else
		if (!strcmp(*argv,"-display")) 
			if (*++argv) display = *argv ; else Usage(); else
		if (!strcmp(*argv,"-g")) 
			if (*++argv) geom = *argv ; else Usage(); else
		if (!strcmp(*argv,"-geometry")) 
			if (*++argv) geom = *argv ; else Usage(); else
		if (!strcmp(*argv,"-geom")) 
			if (*++argv) geom = *argv ; else Usage(); else
		if (!strcmp(*argv,"-rv")) reverse = 1 ; else
		if (!strcmp(*argv,"-z")) imaform = 2 ; else
		if (!strcmp(*argv,"-xy")) imaform = 1 ; else
		if (!strcmp(*argv,"-f")) 
			if (*++argv) fontfile = *argv ; else Usage(); else
		if (!strcmp(*argv,"-f1")) 
			if (*++argv) font1 = *argv ; else Usage(); else
		if (!strcmp(*argv,"-f2")) 
			if (*++argv) font2 = *argv ; else Usage(); else
		if (!strcmp(*argv,"-b")) 
			if (*++argv) bitfile = *argv ; else Usage(); else
		if (!strcmp(*argv,"-m")) 
			if (*++argv) mousefile = *argv ; else Usage(); else
		if (!strcmp(*argv,"-p")) 
			if (*++argv) pixfile = *argv ; else Usage(); 
		else Usage() ;
	}

	if (geom) XParseGeometry(geom,&xori,&yori,&bidon,&bidon);

	Init_fedor(reverse,display,
		   fontfile,bitfile,mousefile,pixfile,
		   argc,command,geom,imaform,font1,font2);
	
	Dispatch();
	w_close() ;
}
