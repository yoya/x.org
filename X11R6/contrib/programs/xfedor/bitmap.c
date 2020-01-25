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
/* bitmap.c */
#include <stdio.h>
extern char * strcpy() ;
extern char * strcat() ;
extern char * getenv() ;

#include "clientimage.h"	/* ClientImage pour fedchar */
#include "fedor.h"		/* fedchar */

extern fedchar carback,cartrav ;
extern int Baseback, BackLeft,Xleft,Baseliney ;
extern int BackColor, CurColor ;
extern int UnixFileNameMode ;     /* 0 ou 1, initialiser dans main */

char * homedir ;         /* utilise aussi dans filer */

Init_bitmap() ;
SaveBitmap() ;
LoadBitmap() ;

static char * dirmap ;
static char * newdirmap ;
static char * bitmap_extension = ".xbm" ;
static char * pixmap_extension = ".xpm" ;

Init_bitmap()
/**********/
{

	if ((dirmap = getenv("MAP"))==0) 
	  if (UnixFileNameMode) dirmap = "." ; else dirmap = "" ;
	if (UnixFileNameMode) {
	   newdirmap = (char *)malloc(strlen(dirmap)+2);
	   strcpy(newdirmap,dirmap);
	   newdirmap[strlen(dirmap)] = '/' ;
	   newdirmap[strlen(dirmap)+1] = '\0' ;
	   dirmap = newdirmap ;
	}
	if ((homedir = getenv("HOME"))==0) homedir = "" ;
}


SaveBitmap(name,flag)
/*******************/
	char * name ;		/* short filename */
	int flag ;		/* 0 ou 1 ou 2 */
{
	char fullname[256],filemouse[256] ;
	ClientImage * forimage, * maskimage ;


        if (*name == '\0') return ;
        if (cartrav.hsize <= 0) return ;

	if (*name == '~') 
	  {
	    strcpy(fullname,homedir);
	    strcat(fullname,(name+1));
	} else
	if (index(name,'/') != NULL) strcpy(fullname,name); 
	else {
		strcpy(fullname,dirmap);
       		strcat(fullname,name);
	}

	if (flag == 1) {		/* sauvegarde souris => 2 fichiers */
	    strcpy(filemouse,fullname) ;
	    strcat(filemouse,"Fore");
	    Rast_Init(&forimage,cartrav.hsize+2,cartrav.down-cartrav.up+2,0);
	    Rast_Init(&maskimage,cartrav.hsize+2,cartrav.down-cartrav.up+2,0);
	    Rast_Op_n1(cartrav.image,&forimage,
		       Xleft,Baseliney+cartrav.up,
		       1,1,
		       cartrav.hsize,cartrav.down-cartrav.up,
		       VIDSTR,BackColor);
	    Rast_Op_n1(carback.image,&maskimage,
		       BackLeft,Baseback+carback.up,
		       0,0,
		       cartrav.hsize+2,cartrav.down-cartrav.up+2,
		       VIDSTR,BackColor);
	    if (!w_writebitmap(filemouse,
			  forimage,
			  cartrav.hsize+2,cartrav.down-cartrav.up+2,
			  -cartrav.hadj,-cartrav.up)) 
			gd_envoyermsg("CAN'T CREAT THE FORE FILE");

	    strcpy(filemouse,fullname) ;
	    strcat(filemouse,"Mask");
	    if (!w_writebitmap(filemouse,
			  maskimage,
			  cartrav.hsize+2,cartrav.down-cartrav.up+2,
			  -cartrav.hadj,-cartrav.up))
                        gd_envoyermsg("CAN'T CREAT THE MASK FILE");
	    Rast_Free(&forimage);
	    Rast_Free(&maskimage);
        } else 
	if (flag == 0) {
	    if (!Extension(fullname,bitmap_extension)) {
	       strcat(fullname,bitmap_extension);
	    }
	    Rast_Init(&forimage,cartrav.hsize,cartrav.down-cartrav.up,0);
	    Rast_Op_n1(cartrav.image,&forimage,
		       Xleft,Baseliney+cartrav.up,
		       0,0,
		       cartrav.hsize,cartrav.down-cartrav.up,
		       VIDSTR,BackColor);
            if (!w_writebitmap(fullname,
                          forimage,
                          cartrav.hsize,cartrav.down-cartrav.up,
                          -cartrav.hadj,-cartrav.up))
                        gd_envoyermsg("CAN'T CREAT THE BITMAP FILE");
	    Rast_Free(&forimage);
	  } else
  	 if (flag == 2) {
	    if (!Extension(fullname,pixmap_extension)) {
	       strcat(fullname,pixmap_extension);
	    }
            if (!w_writepixmap(fullname,
			       cartrav.image,
			       Xleft,Baseliney+cartrav.up,
			       cartrav.hsize,cartrav.down-cartrav.up))
                        gd_envoyermsg("CAN'T CREAT THE PIXMAP FILE");
	  }  
	   

}


int LoadBitmap(name,flag)
/***********************/
        char * name ;           /* short filename */
        int flag ;              /* 0 ou 1 ou 2 selon le format */
{
        char fullname[256],filemouse[256] ;
        int yhot, found;

    if (*name == '\0') return 1;

    if (*name == '~') 
	{
	    strcpy(fullname,homedir);
	    strcat(fullname,(name+1));
	} else
    if (index(name,'/') != NULL) strcpy(fullname,name); 
    else {
      strcpy(fullname,dirmap);
      strcat(fullname,name);

    }

        if (flag == 1) {             /* on lit 2 fichiers */
	    strcpy(filemouse,fullname) ;
	    strcat(filemouse,"Fore");
	    Dodo();
	    if (Get_fedchar(filemouse,&cartrav,1))
		{
		    Xleft = 1;
		    Baseliney = 1 - cartrav.up ;
		} else return 0 ;
	    strcpy(filemouse,fullname) ;
	    strcat(filemouse,"Mask");
	    if (Get_fedchar(filemouse,&carback,1))
		{
		    BackLeft = 0;
		    Baseback = -carback.up ;
		} else return 0 ;
	} else 
	if (flag == 0) {
	    Dodo();
	    if (!Extension(fullname,bitmap_extension)) {
	       strcpy(filemouse,fullname) ;
	       strcat(filemouse,bitmap_extension); /* hack but buffer */
	       if (Get_fedchar(filemouse,&cartrav,1))
		 {
		    Xleft = 0;
		    Baseliney = -cartrav.up ;
		    found = 1 ;
		 } else found = 0 ;
	    }
	    if (!found)
	      if (Get_fedchar(fullname,&cartrav,1))
		 {
		    Xleft = 0;
		    Baseliney = -cartrav.up ;
		 } else return 0;
	} else
	if (flag == 2) {
	    Dodo();
	    if (!Extension(fullname,pixmap_extension)) {
	       strcpy(filemouse,fullname) ; /* hack: to avoid modifying fullname */
	       strcat(filemouse,pixmap_extension); 
	       if (Get_fedchar(filemouse,&cartrav,2))
		 {
		    Xleft = 0;
		    Baseliney = -cartrav.up ;
		    found = 1 ;
		 } else found = 0 ;
	    }
	    if (!found)
	      if (Get_fedchar(fullname,&cartrav,2))
		 {
		    Xleft = 0;
		    Baseliney = -cartrav.up ;
		 } else return 0;
	  }
	return 1 ;
}



static int 
Get_fedchar(filename,fed,flag)
	char * filename ;
	fedchar * fed ;
	int  flag ;          /* 1 pour bitmap, 2 pour pixmap */
{
	int w,h,xhot,yhot ;
	int status ;
	ClientImage * bimage ;

	Rast_Off(&(fed->image),BackColor);
	if (flag == 1) {
	  status = w_readbitmap(filename,&bimage,&w,&h,&xhot,&yhot) ;  
	  if (status) {
	    Rast_Op_1n(bimage,&(fed->image),0,0,0,0,w,h,VIDSTR,CurColor);
	    fed->hsize = w ;
	    fed->up = -yhot ;
	    fed->down = h - yhot ;
	    fed->hadj = - xhot ;
	    fed->hincr = w ;
	    Rast_Free(&bimage);
	    return 1 ;
	  } else return 0 ;
	} else
	if (flag == 2) {
	  status = w_readpixmap(filename,&bimage,&w,&h) ;  
	  if (status) {
	    Rast_Op(bimage,&(fed->image),0,0,0,0,w,h,VIDSTR,CurColor);
	    fed->hsize = w ;
	    fed->up = 0 ;
	    fed->down = h  ;
	    fed->hadj = 0 ;
	    fed->hincr = w ;
	    Rast_Free(&bimage);
	    return 1 ;
	  } else return 0 ;
	}
	return 0 ;
}
