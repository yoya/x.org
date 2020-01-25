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
/* fedor.h */
#define	MAXFONTCARD		256

typedef struct {
	int hincr,vincr;
	int hadj ,vadj;
	char name[50] ;
	int up,down,hsize ;

	ClientImage * image ;		
} fedchar ,*fedcharptr ;


typedef struct {
	char FamilyName[256] ;   
	int Size ;
	int Resol ;		/* on lit xres seul */
	char Properties[50][256] ;     /* on stocke au max 50 proprietes */
	int nprops ;
} BdfHeader ;
