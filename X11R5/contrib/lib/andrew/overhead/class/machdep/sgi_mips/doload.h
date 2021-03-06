/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* 
	doload.h - environment for dynamic loader

	Author:  Zalman Stern July 1989
 */


#ifdef _DYDDX_INTERNAL

/* here is the state during a load */

/* CHANGES:	Several pieces of the data are malloc'ed together
 *		to generate many pointers at once.
 *		Don't free the wrong ones!
 */
struct doload_environment {
    int fd;			/* input file descriptor */
    int problems;		/* count of problems encountered */
    jmp_buf errorJump;		/* error exit */
    char *text;			/* text, init, rdata & data segments */
	void (*initpoint)() ;	/* start of the ".init" section */
	char *gpBase ;		/* start of the "near" data area */
	int gpSize ;		/* size of the "near" data area */
	int gpOffset ;		/* Change in "near" data area */
	int firstGpSection ;	/* usually .lit8 */
	char *membase ;		/* initial page of mapped file */
	char *commonArea ;	/* malloc'ed common symbols */
	int runTimeGnum ;	/* equivolent to ld -G xxx */
    long totalsize;
    struct filehdr filehdr;	/* header at beginning of a.out file */
    struct aouthdr aouthdr;	/* Where the entry point resides among
				 * other things. */
    HDRR symheader;
    struct scnhdr *sections;
	char **contents ;	/* Allocated with "sections" DON'T FREE */
    struct reloc *rtab;		/* relocation table */
    pEXTR symtab;		/* symbol table */
	/* scratch data during relocation */
	short *lastHighStart ;	/* Allocated with "symtab" DON'T FREE */
	short *lastHighResult ;	/* Allocated with "symtab" DON'T FREE */
	short lastExternHighStart ;
	short lastExternHighResult ;
	long  lastExternSymndx ;
	int prevType ;
    char *stringtab;		/* string table */
} ;

/* From sdpool.c */
extern unsigned short int sdpoolstart ;
extern unsigned short int sdpoolend ;
extern unsigned long int sdpool[] ;

#endif /* _DYDDX_INTERNAL */

extern void *(* dyddxload( int , struct dynldinfo * ) )() ;

#define doload_extension ".do"

extern void *(* doload( int, char *, char **, long int *, char * ) )() ;
