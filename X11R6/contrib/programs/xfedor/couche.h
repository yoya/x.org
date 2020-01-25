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
/* couche.h */
typedef struct {
	int type ;   
	int window ;
	int zone ;  	
	int xz, yz ;	/* origine de la zone ou taille de l'expose-region */
        int x,y ;       /* coord button ou expose-region */
	char click ;    /* le bouton modifie */
	char key ;      /* caractere tape au clavier */
} myEvent ;

#define Exposure	0x1		/* an exposure event */
#define KeyPressed  	0x2		/* a key was pressed on keyboard  */
#define ButtonPressed	0x4		/* a mouse button was pressed     */
#define ButtonReleased  0x8		/* a mouse button was released    */
#define MoveMouse	0x10		/* just a mouse motion 		  */
#define LeaveWindow	0x20		/* leavewindow event		  */
#define EnterWindow	0x40		/* enter window event		  */
#define EnterZone	0x200		/* enter user defined region	  */
#define LeaveZone	0x400		/* leave user defined region	  */
#define ResizeWindow    0x2000		/* resized window event		  */

#define ON 1
#define OFF 0

#define WMON 1
#define WMOFF 0

#define	CURS1		0
#define CURS2		1
#define	CURS3 		2      /* indice de cursor */
#define	CURS4		3
#define	CURS5		4

#define Min(a,b) (((a) < (b)) ? (a) : (b))
#define Max(a,b) (((a) > (b)) ? (a) : (b))

#define how_many(x, y)   (((x)+((y)-1))/(y))



