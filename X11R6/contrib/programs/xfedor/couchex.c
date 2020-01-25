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
/* couchex.c */
/***  COUCHE DE PORTAGE SUR X WINDOW ***/

#include <stdio.h>
extern int strlen() ;

#include <X11/Xlib.h>
#include <X11/Xatom.h>		/* XA_WM_NAME */
#include <X11/cursorfont.h>		/* XC_arrow */
#include <X11/Xutil.h>		/* BitmapSucces */
#include <X11/keysym.h>		/* IsModifierKey */
#include <X11/keysymdef.h>		/* IsModifierKey */

#include "couche.h"         /* myEvent, ..CURS, ON */
#include "clientimage.h"    /* ClientImage */

#include "xpm.h"        /* PixmapSuccess ...*/


/*===== VARIABLES EXPORTEES DU MODULE ==============================*/
/* principalement vers myrast.c */

Display * Dpl ;
Window root ;
int depth ;
int screen ;
int BackColor ;
int ImaForm ;

/*===== VARIABLES LOCALES DU MODULE ==============================*/

static GC Gc, GcInv, GcCInv, GcTil, GcPft, GcFontColor, GcColor ; 
static Colormap colormap ;

	/* pour la gestion des zones */
#define NBZONE 200
static struct {int zone,mere,xorz,yorz;} Tzone[NBZONE];

static XColor fore,back ; /* pour XCreatePixmapCursor */

static unsigned long background_pixel, border_pixel ; /* pour wattrib */

static XFontStruct * ft ;		/* pour w_centrer */

static Cursor ServCurs[5];     /* tableau indic'es par les macros .. CURS */

static int argc ;
static char ** argv ;	/* pour XSetStandardProperties */

#include "bitmaps/nfmono"           /* fond fenetre mere */
#include "bitmaps/bull"             /* icone arbre bull */

static Pixmap bullicon ;

static long flagPosition ;

#define PIXELGREY -13      /* pour traitement special monochome entre
			      w_color et w_greypixel */
int ErrorStatus ;

NoXError(display, error)
Display        *display;
XErrorEvent    *error;
{
    ErrorStatus = error -> error_code;
}

/*--------------------------------------------------------------*/      
/* W_INITIAL */
int w_initial (reverse,display,font1,font2,r_argc,r_argv,geom,imaform)
	int reverse ;
	char  * display ;
	char * font1, * font2 ;
	int r_argc ;
	char ** r_argv ;
        char * geom ;
        int imaform ;   /* determine le format d'image XY ou ZPixmap */
        
{  
         XGCValues xgcv;
	 Pixmap MyGrey ;
	 long TabGris[8];	/* 16 sur 16 bits */
	 int i ;

      if ((Dpl = XOpenDisplay(display)) == NULL) {
		fprintf(stderr,"Cannot open display : %s\n",
			XDisplayName(display));
		exit(0);
      }

/* XSynchronize(Dpl,1);  /* use in debug */

      screen = DefaultScreen(Dpl) ;
      root = DefaultRootWindow(Dpl);
      depth = DefaultDepth(Dpl,screen) ;
      colormap = DefaultColormap(Dpl,screen) ;

      if (imaform == 0) {  /* pas de format specifie : choix hack */
	if (depth > 2) ImaForm = ZPixmap ; else ImaForm = XYPixmap ; 
      } else
      if ((imaform == 2) && (depth != 2)) ImaForm = ZPixmap ; else
	ImaForm = XYPixmap ;

      if (reverse == OFF ) {
	      	xgcv.foreground = BlackPixel(Dpl,screen) ;
 		border_pixel = BlackPixel(Dpl,screen) ;
		fore.pixel = BlackPixel(Dpl,screen) ;
	      
		xgcv.background = WhitePixel(Dpl,screen) ;
		background_pixel = WhitePixel(Dpl,screen) ;
 		back.pixel = WhitePixel(Dpl,screen) ;
	} else {
	      	xgcv.foreground = WhitePixel(Dpl,screen) ;
		border_pixel = WhitePixel(Dpl,screen) ;
		fore.pixel = WhitePixel(Dpl,screen) ;
	      
		xgcv.background = BlackPixel(Dpl,screen) ;
		background_pixel = BlackPixel(Dpl,screen) ;
		back.pixel = BlackPixel(Dpl,screen) ;
      }

      BackColor = WhitePixel(Dpl,screen) ;
      XQueryColor(Dpl,colormap,&fore) ;
      XQueryColor(Dpl,colormap,&back) ;

	/* init de shapes souris predefinis */
      ServCurs[CURS1]  = XCreateFontCursor(Dpl,XC_dot);
      ServCurs[CURS2]   = XCreateFontCursor(Dpl,XC_plus);
      ServCurs[CURS3] = XCreateFontCursor(Dpl,XC_fleur);
      ServCurs[CURS4]  = XCreateFontCursor(Dpl,XC_left_ptr);
      ServCurs[CURS5]  = XCreateFontCursor(Dpl,XC_sb_h_double_arrow);

	/* Creation des Gc de travail globaux */
      xgcv.graphics_exposures = False ;
      xgcv.plane_mask = 1 ;     

      XSynchronize(Dpl,1);
      ErrorStatus = 0;
      XSetErrorHandler(NoXError);
      xgcv.font = XLoadFont(Dpl,font1);
      if (ErrorStatus) {
	  fprintf(stderr,"Can'f find font named \"%s\", try another one with -f1 option\n",font1);
          exit(0);
/*	  xgcv.font = XQueryFont(Dpl,DefaultGC(Dpl,screen) -> gid)-> fid; */
      }
      ft = XQueryFont(Dpl,xgcv.font) ;
      Gc = XCreateGC(Dpl,root,GCPlaneMask|GCForeground|GCBackground|GCFont|
				GCGraphicsExposures, &xgcv);

      GcColor = XCreateGC(Dpl,root,GCGraphicsExposures, &xgcv);

      GcFontColor = XCreateGC(Dpl,root,GCBackground|GCForeground|
			      GCGraphicsExposures, &xgcv);

      ErrorStatus = 0;
      XSetErrorHandler(NoXError);
      xgcv.font = XLoadFont(Dpl,font2);
      if (ErrorStatus) {
	  fprintf(stderr,"Can'f find font named \"%s\", try another one with -f2 option\n",font2);
          exit(0);
/*	  xgcv.font = XQueryFont(Dpl,DefaultGC(Dpl,screen) -> gid)-> fid; */
      }
      XSynchronize(Dpl,0);
      GcPft = XCreateGC(Dpl,root,GCPlaneMask|GCForeground|GCBackground|GCFont|
				GCGraphicsExposures, &xgcv);

      xgcv.function = GXinvert ;
      GcInv = XCreateGC(Dpl,root,GCPlaneMask|GCForeground|GCBackground|
			GCFunction|GCGraphicsExposures, &xgcv);

      xgcv.function = GXcopyInverted ;
      GcCInv = XCreateGC(
Dpl,root,GCPlaneMask|GCForeground|GCBackground|
			 GCFunction|GCGraphicsExposures, &xgcv);

      for (i=0;i<8;i++) TabGris[i] = 0x5555aaaa ;
      MyGrey = XCreatePixmap(Dpl,root,16,16,depth);
      XFillRectangle(Dpl,MyGrey,GcColor,0,0,16,16);
      XPutImage(Dpl,MyGrey,GcColor,
		XCreateImage(Dpl,DefaultVisual(Dpl,screen),
			     1,XYBitmap,0,(char *)TabGris,16,16,16,0),
		0,0,0,0,16,16);

      xgcv.tile = MyGrey ;
      xgcv.fill_style = FillTiled ;
      GcTil = XCreateGC(Dpl,root,GCForeground|GCBackground|
			GCFillStyle|GCTile|GCGraphicsExposures, &xgcv);

      argc = r_argc ;
      argv = r_argv ;
 
      bullicon = XCreateBitmapFromData(Dpl,root,
				       bull_bits,bull_width,bull_height);

      if (geom) flagPosition = USPosition ; else
	        flagPosition = PPosition ;

      return((int)root);
}
   
/*---------------------------------------------------------------------*/
/* W_OUVRIR */
long TransformMask(mask)
	int mask ;
{	int eventmask=0 ;

	if (mask & KeyPressed) eventmask |= KeyPressMask ;
	if (mask & ButtonPressed) eventmask |= ButtonPressMask ;
	if (mask & MoveMouse) eventmask |= PointerMotionMask ;
	if (mask & LeaveWindow) eventmask |= LeaveWindowMask ;
	if (mask & EnterWindow) eventmask |= EnterWindowMask ;
	if (mask & ResizeWindow) eventmask |= StructureNotifyMask ;
	if (mask & LeaveZone) eventmask |= LeaveWindowMask ;
	if (mask & EnterZone) eventmask |= EnterWindowMask ;
	return eventmask|ExposureMask|ButtonReleaseMask ;
}

int w_ouvrir (wd,x,y,w,h,titre,mlook,eventmask,no_wm)
   int wd ;		/* fenetre mere */
   int x,y,w,h ;	/* dimension + origine */
   char *  titre;   	/* titre de la fenetre , ou "" si on veut rien */
   int mlook; 		/* look de la souris */
   int eventmask ;	/* masque des evenements */
   int no_wm ;		/* False or True */
{ 
	Window wn;
	XSetWindowAttributes wattrib ;
	XSizeHints hints ;
	XWMHints wmhints ;
	XClassHint classhint ;
	int backflag  ;
	Pixmap pix ;

	wattrib.event_mask = TransformMask(eventmask) ;
	wattrib.cursor = ServCurs[mlook] ; 

	if ((wd == root) && (no_wm == WMOFF)) {
	  wattrib.save_under = False ;
	  wattrib.override_redirect = True ;
	} else {
	  wattrib.save_under = False ;
	  wattrib.override_redirect = False ;
	}

	if (eventmask == 0) {  /* moyen de test sur nf_mono */
		wattrib.background_pixmap = XCreatePixmap(Dpl,wd,
					nfmono_width,nfmono_height,depth);
		pix = XCreateBitmapFromData(Dpl,wd,
				nfmono_bits,nfmono_width,nfmono_height);
		XFillRectangle(Dpl,wattrib.background_pixmap,GcColor,
			       0,0,nfmono_width,nfmono_height);
		XCopyPlane(Dpl,pix,wattrib.background_pixmap,GcColor,0,0,
					nfmono_width,nfmono_height,0,0,1);

		backflag = CWBackPixmap ;
	} else {
		wattrib.background_pixel = background_pixel ;
		backflag = CWBackPixel ;
	}
	wattrib.border_pixel = border_pixel ;
	wattrib.bit_gravity = StaticGravity ;
	wattrib.win_gravity = StaticGravity ;

        wn = XCreateWindow(Dpl,wd,x,y,w,h,1,depth,
			   InputOutput,CopyFromParent,
			   CWOverrideRedirect |  backflag | CWBorderPixel | 
			   CWSaveUnder | CWEventMask | CWCursor,
			   /*| CWBitGravity | CWWinGravity */
			&wattrib);

	/* on a determiner si l'utilisateur a donner un ordre de
	   positionnement avec l'option -geometry, dans ce cas le flag
	   de position est USPosition, sinon c'est le programme qui
	   positionne */ 
	
	if ((wd == root) && (no_wm == WMON)) {
		hints.flags = PPosition|PSize|PMinSize|PMaxSize ;
		hints.x = x ;
		hints.y = y ;
 		hints.width = w ;
		hints.height = h ;
		hints.min_width = hints.width ;
		hints.min_height = hints.height ;
		hints.max_width = hints.width ;
		hints.max_height = hints.height ;
		XSetNormalHints(Dpl,wn,&hints);	

		XStoreName(Dpl,wn,titre);	
		XSetIconName(Dpl,wn,"XFEDOR");	
		XSetCommand(Dpl,wn,argv,argc);
			
		wmhints.flags = InputHint | IconPixmapHint ;
		wmhints.input = True ;
		wmhints.icon_pixmap = bullicon ;
		XSetWMHints(Dpl,wn,&wmhints);

		classhint.res_name = "xfedor" ;
		classhint.res_class = "font_editor" ;
		XSetClassHint(Dpl,wn,&classhint);
	}
	return ((int)wn);
}
   
/*-------------------------------------------------------------*/
/* W_CRZON */
int w_crzon (wd,x1,y1,x2,y2,mlook) 
   int wd ;
   int x1,y1,x2,y2 ;
   int mlook;	  	   /* numero d'icone souris */
{
	Window wn;
	XSetWindowAttributes wattrib ;
	XWindowAttributes watt ;

        static int iz = 0 ;

	XGetWindowAttributes(Dpl,wd,&watt);
	wattrib.cursor = ServCurs[mlook] ; 
	wattrib.event_mask = watt.your_event_mask ;

        wn = XCreateWindow(Dpl,wd,x1,y1,x2-x1,y2-y1,0,0,
			   InputOnly,CopyFromParent,CWEventMask|CWCursor,
			   &wattrib);

	Tzone[iz].zone = wn ;
	Tzone[iz].mere = wd ;	/* gestion des zones pour GetEvent */
      	Tzone[iz].xorz = x1 ;
      	Tzone[iz].yorz = y1 ;
	iz ++ ;			
        XMapWindow(Dpl,wn);
	return ((int)wn);
		
}

/*-------------------------------------------------------------*/
/* W_GETEVENT */
int TransformEvent(xtype)
	int xtype ;
{
    switch(xtype) {
	case KeyPress : return KeyPressed ; 
	case ButtonPress : return ButtonPressed ; 
	case ButtonRelease : return ButtonReleased ; 
	case MotionNotify : return MoveMouse ; 
	case LeaveNotify : return LeaveWindow ; 
	case EnterNotify : return EnterWindow ; 
	case Expose : return Exposure ; 
    }
}

void w_getevent (pev)
   myEvent * pev;
		
{
	XEvent rep ;
	static int ZoneCour = 0 ;
	int i, keysym ;
	char s[1] ;

	do {
	   XNextEvent(Dpl,&rep) ;
           if (rep.type == KeyPress) XLookupString(&rep,s,1,&keysym,NULL);
	} while ( ((rep.type != Expose) || (rep.xexpose.count != 0)) &&
		  ((rep.type != KeyPress) || (IsModifierKey(keysym))) &&
		  (rep.type != ButtonPress) &&
		  (rep.type != ButtonRelease) &&
		  (rep.type != MotionNotify) &&
		  (rep.type != LeaveNotify) &&
		  (rep.type != EnterNotify)) ;

	pev->key = (char) keysym ;	
	if ((rep.type == KeyPress) && (pev->key > 96) && (pev->key < 123) &&
	    ((rep.xkey.state && ControlMask))) pev->key -= 96 ;

        if (rep.type == ButtonPress) XUngrabPointer(Dpl,CurrentTime);

	/* recherche dans la liste des fenetres zones pour voir si
		cette fenetre X est une zone a moi */
	for (i=0;((i<NBZONE)&&(Tzone[i].zone!= rep.xany.window));i++) ;

	if (i==NBZONE) {	/* window normale */
		pev->window = rep.xany.window ;
		pev->zone = ZoneCour ;
	    	pev->type = TransformEvent(rep.type) ;
	} else {		/* window = zone */
		pev->window = Tzone[i].mere ;
		pev->zone = rep.xany.window ;
		pev->xz = Tzone[i].xorz ;
		pev->yz = Tzone[i].yorz ;
		if (rep.type == EnterNotify) {
			ZoneCour = pev->zone ;
			pev->type = EnterZone ;
		} else
		if (rep.type == LeaveNotify) {
			ZoneCour = 0 ;
			pev->type = LeaveZone ;
                } else  pev->type = TransformEvent(rep.type) ;
	}

	pev->click = (rep.xbutton.button == 1)? 1:2 ;
	pev->x = rep.xbutton.x ;
	pev->y = rep.xbutton.y ;

	if (rep.type == Expose) {
		pev->x = rep.xexpose.x ;
		pev->y = rep.xexpose.y ;
		pev->xz = rep.xexpose.width ;
		pev->yz = rep.xexpose.height ;
	}
}

/*--------------------------------------------------------------------*/
/* W_ECRIRE */	
void w_ecrire (wd,chaine,x,y)
   int wd; 		/* la fenetre */
   char * chaine ; 	/* la chaine a ecrire */
   int x,y ; 		/* ou ecrire */
{
      XDrawImageString (Dpl,wd,GcPft,x,y,chaine,strlen(chaine));
}
   
/*--------------------------------------------------------------------*/
/* W_CENTRER */
void w_centrer (wd,chaine,x1,y1,x2,y2)
   int wd; /* la fenetre */
   char * chaine ; /* la chaine a ecrire */
   int x1,y1,x2,y2 ; /* boite  dans laquelle on centre */

{	register int x,y,l,len,w,h ;

      w = x2 - x1 ;
      h = y2 - y1 ;
      XFillRectangle(Dpl,wd,GcCInv,x1+1,y1+1,w-1,h-1);
      len = strlen(chaine) ;
      do { 
        l = XTextWidth(ft,chaine,len);
	if (l <= w) break ;
	len -- ;
      } while (1) ;
      x = x1 + ((w - l)>>1) ;
      y = y1 + ((h + ft->ascent + ft->descent)>>1)-1 ;
      XDrawString(Dpl,wd,Gc,x,y,chaine,len);
}
   
/*--------------------------------------------------------------------*/
/* W_LINE */
void w_line (wd,x1,y1,x2,y2,mode)
   	int wd ;
	int x1,y1,x2,y2;
	int mode ;
{
      	XDrawLine(Dpl,wd,(mode==ON)?Gc:GcInv,x1,y1,x2,y2);
}    

/*--------------------------------------------------------------------*/
/* W_RECT */
void w_rect (wd,x1,y1,x2,y2,mode)
   	int wd ;
	int x1,y1,x2,y2 ;
	int mode ;
{	
     	XDrawRectangle(Dpl,wd,(mode==ON)?Gc:GcInv,x1,y1,x2-x1,y2-y1);	
}		 


/*--------------------------------------------------------------------*/ 
/* W_FELLIPS */	
void w_fellips (wd,x1,y1,x2,y2,mode)    
   	int wd ;
	int x1,y1,x2,y2 ;
 	int mode ;
{
      	XFillArc(Dpl,wd,(mode==ON)?Gc:GcInv,x1,y1,x2-x1,y2-y1,0,23040) ;
}

/*----------------------------------------------------------------*/
/* W_ELLIPS */
void w_ellips (wd,x1,y1,x2,y2,mode)    
   	int wd ;
	int x1,y1,x2,y2 ;
	int mode ;
{
      	XDrawArc(Dpl,wd,(mode==ON)?Gc:GcInv,x1,y1,x2-x1,y2-y1,0,23040) ;
}

/*--------------------------------------------------------------------*/
/* W_GRIS */
void w_gris (wd,x1,y1,x2,y2) 
   	int wd ;
	int x1,y1,x2,y2 ;
{
      	XFillRectangle(Dpl,wd,GcTil,x1,y1,x2-x1,y2-y1);
}

/*--------------------------------------------------------------------*/
/* W_INV */
void w_inv (wd,x1,y1,x2,y2)
   	int wd ;
	int x1,y1,x2,y2 ;
{
      	XFillRectangle(Dpl,wd,GcInv,x1,y1,x2-x1,y2-y1);
}

/*--------------------------------------------------------------------*/
/* W_FORE */
void w_fore (wd,x1,y1,x2,y2)
   	int wd ;
	int x1,y1,x2,y2 ;
{
      	XFillRectangle(Dpl,wd,Gc,x1,y1,x2-x1,y2-y1);
}

/*--------------------------------------------------------------------*/
/* W_COLOR */
void w_color (wd,x1,y1,x2,y2,color)
   	int wd ;
	int x1,y1,x2,y2 ;
        int color ;
{
  if ((depth == 1) &&
      (color == PIXELGREY))
    w_gris(wd,x1,y1,x2,y2) ; else
      {
        XSetForeground(Dpl,GcColor,color);
      	XFillRectangle(Dpl,wd,GcColor,x1,y1,x2-x1,y2-y1);
      }
}

/* ------------------------------------------------------------*/
/* W_POINT */
void w_point(wd,x,y,color)
	int wd ;
	int x,y ;
	int color ;    /* pixel value */
{
        XSetForeground(Dpl,GcColor,color);
	XDrawPoint(Dpl,wd,GcColor,x,y);
}

/*---------------------------------------------------------------------*/
/* W_RASTOP */
void w_rastop(ws,wd,sx,sy,w,h,dx,dy)
	int ws,wd ;
	int sx,sy,w,h,dx,dy ;
{
	XCopyArea(Dpl,ws, wd, GcColor, sx, sy, w, h, dx, dy);
}	

/*---------------------------------------------------------------------*/
/* W_BITBLT */
w_bitblt(wd,clientimage,sx,sy,w,h,dx,dy)
	int wd ;
	ClientImage * clientimage ;
	int sx,sy,w,h,dx,dy ;
{
        XPutImage(Dpl,wd,GcFontColor,
		  (XImage *) clientimage, 
		  sx,sy,dx,dy,w,h);
}

/*---------------------------------------------------------------------*/
/* W_PIXEL */
w_pixel(wd,data,x,y,w,h)
	int wd ;
	char * data ;
	int x,y,w,h ;
{
	XCopyPlane(Dpl,XCreateBitmapFromData(Dpl,root,data,w,h),
		       wd,
		       Gc,0,0,w,h,x,y,1);
}

/*--------------------------------------------------------------------*/
/* W_SOURIS */
void w_souris (wd, climafore, climask, w,h,xhot,yhot)	
	int wd ;
        ClientImage * climafore, * climask ;  /* depth 1 */
	int w,h ;
	int xhot, yhot ;
{
	Pixmap pfore1, pmask1 ;
        XGCValues xgcv;
	GC Gc1 ;

	pfore1 = XCreatePixmap(Dpl,root,w,h,1);
	pmask1 = XCreatePixmap(Dpl,root,w,h,1);

	xgcv.graphics_exposures = False ;
	xgcv.function = GXcopyInverted ;

        Gc1 = XCreateGC(Dpl,pfore1,GCFunction|GCGraphicsExposures, &xgcv);


	XPutImage(Dpl,pfore1,Gc1,
		  (XImage*)climafore,
		  0,0,0,0,w,h);
	XPutImage(Dpl,pmask1,Gc1,
		  (XImage*)climask,
		  0,0,0,0,w,h);

	XDefineCursor(Dpl,wd,
		XCreatePixmapCursor(Dpl,pfore1,pmask1,&fore,&back,
			(xhot<0)?0:(xhot>w)?w:xhot,
			(yhot<0)?0:(yhot>h)?h:yhot));
}

/*--------------------------------------------------------------------*/
/* W_WRITEPIXMAP */
int w_writepixmap (name,clientimage,x,y,w,h)	
	char * name ;
	ClientImage * clientimage ;   /* depth n */
	int x,y,w,h ;

{	
    Pixmap p ;
    XpmAttributes icon_attributes ;

    if ((w == 0) || (h == 0)) return 0 ;
    p = XCreatePixmap(Dpl,root,w,h,depth);

    XPutImage(Dpl,p,GcColor,
	      (XImage *) clientimage, 
	      x,y,0,0,w,h);

    icon_attributes.width = w;
    icon_attributes.height = h;
    icon_attributes.valuemask = XpmSize ;

    if (XpmWriteFileFromPixmap (Dpl, name, p, NULL, &icon_attributes)
	== XpmSuccess)
	return 1 ; 
    else return 0 ;
}

/*--------------------------------------------------------------------*/
/* W_WRITEBITMAP */
int w_writebitmap (name,clientimage,w,h,xhot,yhot)	
	char * name ;
	ClientImage * clientimage ;   /* depth 1 */
	int w,h,xhot,yhot ;
{	Pixmap p ;
        XGCValues xgcv;
	GC Gc1 ;

	if ((w == 0) || (h == 0)) return 0 ;
	p = XCreatePixmap(Dpl,root,w,h,1);

	xgcv.graphics_exposures = False ;
	xgcv.function = GXcopyInverted ;

        Gc1 = XCreateGC(Dpl,p,GCFunction|GCGraphicsExposures, &xgcv);

	XPutImage(Dpl,p,Gc1,
		  (XImage *) clientimage, 
		  0,0,0,0,w,h);

      	if (XWriteBitmapFile(Dpl,name,p,w,h,xhot,yhot)==BitmapSuccess)
		return 1 ; 
	else    return 0 ;
}

/*--------------------------------------------------------------------*/
/* W_READBITMAP */
w_readbitmap (name,bitmage,w,h,xhot,yhot)	
	char * name ;
        ClientImage * * bitmage ; 
	int *w,*h,*xhot,*yhot ;
{	
  Pixmap bit ;

        if (XReadBitmapFile(Dpl,root,name,w,h,&bit,xhot,yhot)!=BitmapSuccess)
				return 0 ;
	
	*bitmage = XGetImage(Dpl,bit,0,0,*w,*h,1,XYPixmap) ;

	return (1);
}

/*--------------------------------------------------------------------*/
/* W_READPIXMAP */
w_readpixmap (name,bitmage,w,h)	
	char * name ;
        ClientImage * * bitmage ; 
	int *w,*h ;
{	
    Pixmap pix ;
    XpmAttributes icon_attributes;

    icon_attributes.valuemask = XpmReturnPixels | XpmSize;
	
    if (XpmReadFileToPixmap(Dpl, root, name, &pix, NULL, &icon_attributes)
	!= XpmSuccess)
	return 0 ;

    *w =  icon_attributes.width;
    *h =  icon_attributes.height;
    
    *bitmage = XGetImage(Dpl,pix,0,0,*w,*h,AllPlanes,ImaForm) ;

    return (1);
}

/*------------------------------------------------------------------*/
/* W_CLEARAREA */
void w_cleararea(wd,x1,y1,x2,y2)
     	int wd ;
	int x1,y1,x2,y2 ;
{
	XClearArea(Dpl,wd,x1,y1,x2-x1,y2-y1,False);
}

/*------------------------------------------------------------------*/
/* W_CLEAR */
void w_clear(wd)
     	int wd ;
{
	XClearWindow(Dpl,wd);
}

/*---------------------------------------------------------------------*/
/* W_BITMAP */
int w_bitmap(w,h)
	int w,h ;
{
        return (int)XCreatePixmap(Dpl,root,w,h,depth);
}

/*------------------------------------------------------------------*/
/* W_MONTRER */
void w_montrer (wd)
        int wd ;
{
  XMapRaised(Dpl,wd);
}

/*------------------------------------------------------------------*/
/* W_FLUSH */
void w_flush ()
{
        XSync(Dpl,0);
}

                
/*------------------------------------------------------------------*/
/* W_CACHER */
void w_cacher (wd)
        int wd ;
{
  	XUnmapWindow(Dpl,wd);
}

/*----------------------------------------------------------------------*/
/* W_RESIZE */
void w_resize(wd,largeur,hauteur)
	int wd;
	int largeur,hauteur ;
{
	XSizeHints hints ;

	hints.flags = USSize|PMinSize|PMaxSize ;
	hints.width = largeur ;
	hints.height = hauteur ;
	hints.min_width = largeur ;
	hints.min_height = hauteur ;
	hints.max_width = largeur ;
	hints.max_height = hauteur ;
	XSetNormalHints(Dpl,wd,&hints);

	XResizeWindow(Dpl,wd,largeur,hauteur);
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/* W_POP */
void w_pop (wd)
        int wd ;
{ 
        int px, py ;
	long bidon ;
	XWindowAttributes watt ;

	XGetWindowAttributes(Dpl,wd,&watt);
	XQueryPointer(Dpl,root,&bidon,&bidon,&px,&py,&bidon,&bidon,&bidon);
	
	if ((px + watt.width/2) > DisplayWidth(Dpl,screen))
	  px = DisplayWidth(Dpl,screen) - watt.width/2 ;
	if ((py + watt.height) > DisplayHeight(Dpl,screen))
	  py = DisplayHeight(Dpl,screen) - watt.height ;

	XMoveWindow(Dpl,wd,px-watt.width/2,py-7);
	XMapRaised(Dpl,wd);

}


/*----------------------------------------------------------------------*/
/* W_LARGEUR*/
int w_largeur(wd)
	int wd ;
{	XWindowAttributes watt ;

	XGetWindowAttributes(Dpl,wd,&watt);
	return(watt.width);
}


/* ------------------------------------------------------------*/
/* W_STRWIDTH */	
int w_strwidth(s)	     /* popup.c l'utilise */
	 register char * s;
{  
	return(XTextWidth(ft,s,strlen(s))) ;
}

/* ------------------------------------------------------------*/
/* W_CLICK */	

Bool fpress (display,event,args)
/** utilisee pour XCheckIfEvent **/
	Display * display ;
	XEvent * event ;
	char * args ;
{
	return ((event->type == ButtonPress)?True:False) ;
}


int w_click()      /* rends vrai si un event click est arrive */
{  
     XEvent e ;
	return(XCheckIfEvent(Dpl, &e, fpress, NULL) == False);
}

/* ------------------------------------------------------------*/
/* W_SETFOCUS */	
int w_setfocus(w)
     int w ;
{
     XSetInputFocus(Dpl,w,RevertToPointerRoot,CurrentTime);
}

/* ------------------------------------------------------------*/
/* W_SETROOT */	
int w_setroot(clientimage,x,y,w,h)	
        ClientImage * clientimage ;
	int x,y,w,h ;
{	Pixmap p ;

	if (w == 0 ) return ;
	p = w_bitmap(w,h);

	XPutImage(Dpl,p,GcColor,
		  (XImage *) clientimage, 
		  x,y,0,0,w,h);

	XSetWindowBackgroundPixmap(Dpl,
                                RootWindow(Dpl, DefaultScreen(Dpl)),p);
        XClearWindow(Dpl,RootWindow(Dpl, DefaultScreen(Dpl)));
        XFlush(Dpl) ; 
}


/* ------------------------------------------------------------*/
/* W_NCOLOR */	
int w_ncolor()
/* rends le nombre de couleurs differentes du display */
{
  return DisplayCells(Dpl,screen);

}


/* ------------------------------------------------------------*/
/* W_BLACKPIXEL */	
int w_blackpixel()
/* rends le pixel noir */
{
  return BlackPixel(Dpl,screen);
}

/* ------------------------------------------------------------*/
/* W_GREYPIXEL */	
int w_greypixel()
/* rends un pixel gris (compatible w_color) */
{
  XColor def, exact ;

  if (depth > 1) {
    XAllocNamedColor(Dpl,colormap,"LightGrey",&def,&exact);
    return def.pixel ;
  } else return PIXELGREY ;
}


/* ------------------------------------------------------------*/
/* W_CUTBUF */	
char *  w_cutbuf()
/* rends un pointeur sur le cut buffer 0 ou '\0' au moins */
{
  int n,i=0 ;
  char * s ;
  char * buff ;

  s = XFetchBytes(Dpl,&n);
  if (s == NULL) buff = (char*) malloc(1) ;
  else {
    buff = (char*) malloc(n+1) ;
    for (i=0 ;i<n ; i++) buff[i] = s[i] ;
  }
  buff[i] = '\0' ;
  return buff ;
}

/* ------------------------------------------------------------*/
/* W_NOSAVEUNDER */	
int w_nosaveunder()
{
   return !XDoesSaveUnders(DefaultScreenOfDisplay(Dpl)) ;

}

/* ------------------------------------------------------------*/
/* W_SETBACK */	
int w_setback(win,pix)	
	int win;
	int pix ;
{
	XSetWindowBackgroundPixmap(Dpl,win,pix);
        XClearWindow(Dpl,win);
        XFlush(Dpl) ; 
}

/* ------------------------------------------------------------*/
/* W_CLOSE */	
int w_close()
{
   XCloseDisplay(Dpl);
}

/* ------------------------------------------------------------*/
/* W_BACKING */	
int w_backing(wd)
   int wd ;
{
   XSetWindowAttributes wattrib ;

   wattrib.backing_store = WhenMapped ;

   XChangeWindowAttributes(Dpl,wd,CWBackingStore,&wattrib);
}
