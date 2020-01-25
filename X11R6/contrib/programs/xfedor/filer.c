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
/* filer.c */
#include <stdio.h>
extern char * getenv() ;


#include "clientimage.h"		/* ClientImage */
#include "fedor.h"		/* fedchar + BdfHeader */

extern fedchar chartab[MAXFONTCARD] ;	
extern fedchar extratab[MAXFONTCARD] ;	
extern int maxup, maxdown, maxhsize, maxhadj ;	/* pour SauveFont */
extern nf_font ;
extern CurColor ;
extern int Prem, PremExtra ;
extern char * homedir ;       /* initialiser dans bitmap */
extern int UnixFileNameMode ; /* 0 ou 1, initialiser dans main */

Init_filer() ;
int GetFont() ;
SauveFont() ;
int GetExtraFont() ;

char FileName[256];
char ExtraName[256];	
BdfHeader header ;	
BdfHeader xeader ;	/* ExtraFont header */

	/** LOCAL */

static FILE * pF ; 	/* pour lire et ecrire */
static char * dirbdf ;
static char * newdirbdf ;


Init_filer()
{
    if ((dirbdf = getenv("BDF"))==0) 
      if (UnixFileNameMode) dirbdf = "." ; else dirbdf = "" ;
    if (UnixFileNameMode) {
       newdirbdf = (char *)malloc(strlen(dirbdf)+2);
       strcpy(newdirbdf,dirbdf);
       newdirbdf[strlen(dirbdf)] = '/' ;
       newdirbdf[strlen(dirbdf)+1] = '\0' ;
       dirbdf = newdirbdf ;
    }
}


int GetFont(name)
	char * name ;
{
	int rep ;

	rep = ReadFont(name,chartab,&header,&Prem);
	if (rep == 1) {
		strcpy(FileName,name);
		ShowHeader() ;
		if (FontMax_Resize() != 2) {
		  /* genere un resize diminuant qui ne fera pas d'expose ou
		     pas de resize du tout */
		  w_cacher(nf_font) ;
		  w_montrer(nf_font) ;  /* hack ? */
		}
	}
	return rep ;
}


SauveFont()
/* a partir de chartab[0..MAXFONTCARD-1], de header, 
   on ecrit un FileName format bdf */
/* rend 0 si c'est ok */
{
	int i ;
	int nchars ;
	char * filename[138] ;
	int bytesperrow ;
        unsigned char * vidmem ;
	int vidwidth ;

	if (strcmp( FileName, "No File") == NULL) return -1 ;

        if (*FileName == '~') 
	{
	    strcpy(filename,homedir);
	    strcat(filename,(FileName+1));
	} else
	if (index(FileName,'/') != NULL) strcpy(filename,FileName); 
	else {
		strcpy(filename,dirbdf);
       		strcat(filename,FileName);
	}

        if (!Extension(filename,".bdf")) strcat(filename,".bdf");
	
	if ((pF = fopen( filename, "w")) == NULL) return -1 ;
		
	fprintf(pF,"STARTFONT 2.1\n");
	fprintf(pF,"FONT %s\n",header.FamilyName);
	fprintf(pF,"SIZE %d %d %d\n",header.Size,header.Resol,header.Resol);
	fprintf(pF,"FONTBOUNDINGBOX %d %d %d %d\n",
		  maxhsize,(maxdown-maxup),maxhadj,-maxdown);
	fprintf(pF,"STARTPROPERTIES %d\n",header.nprops);
	for (i=header.nprops-1; i >= 0; i--)
		if (prefix(header.Properties[i],"FONT_ASCENT"))
			fprintf(pF,"FONT_ASCENT %d\n",-maxup); else
		if (prefix(header.Properties[i],"FONT_DESCENT"))
			fprintf(pF,"FONT_DESCENT %d\n",maxdown); else
 		fprintf(pF,"%s",header.Properties[i]);
	fprintf(pF,"ENDPROPERTIES\n");
	nchars = 0 ;	
	for (i=0; i<MAXFONTCARD; i++) 
		if (chartab[i].hsize>0) nchars++ ;
	fprintf(pF,"CHARS %d\n",nchars);
	for (i=0 ;i<MAXFONTCARD; i++) {
	  int row, bh ;
	  int ix ;
	  if (chartab[i].hsize>0) {
	       	fprintf(pF,"STARTCHAR %s\n",chartab[i].name);
		fprintf(pF,"ENCODING %d\n",i);
		fprintf(pF,"SWIDTH %d %d\n",chartab[i].vadj,0);
		fprintf(pF,"DWIDTH %d %d\n",chartab[i].hincr,chartab[i].vincr);
		fprintf(pF,"BBX %d %d %d %d\n",
				chartab[i].hsize,
				(bh=chartab[i].down - chartab[i].up),
				chartab[i].hadj,
				-chartab[i].down);
		fprintf(pF,"BITMAP\n");
		Rast_Mem(chartab[i].image,
			 chartab[i].hsize,
			 chartab[i].down - chartab[i].up,
			 &vidmem,&vidwidth) ;
	        bytesperrow = (((chartab[i].hsize)+7)>>3) ;	
				/* nb d'octet par ligne (pad 8) */
		for (row=0; row < bh;row++) {
		    	for (ix=0 ; ix < bytesperrow ; ix++) {
				fprintf(pF,"%.2x",*(vidmem++));
			      }
			vidmem += (vidwidth - bytesperrow) ;
			fprintf(pF,"\n");
		}
		fprintf(pF,"ENDCHAR\n");
	    }
	}
	fprintf(pF,"ENDFONT\n");

	fclose(pF);
	return 0 ;
}

/************************************************/
/************************************************/

int Extension(filename, extent)   /* use also in bitmap.c */
/* rend 1 si extent est l'extension de filename, 0 sinon */
	char * filename, * extent ;
{ char * s ;
	if (( s = (char*)rindex(filename,'.'))==NULL) return 0 ; else
	   return(strcmp(extent,s)==NULL)?1:0 ;
}

/*
 * return 1 if str is a prefix of buf
 */
static prefix(buf, str)
    char *buf, *str;
{
    return strncmp(buf, str, strlen(str))? 0:1;
}


static int fatal(msg, p1, p2, p3, p4)
    char *msg;
{
    fprintf(stderr,"\n");
    fprintf(stderr, msg, p1, p2, p3, p4);
    fprintf(stderr,"\n");
    fclose(pF);
    return -1 ;
}

/*
 * read the next line and keep a count for error messages
 */
static char *
getline(s)
    char *s;
{
    s = fgets(s,256,pF);
    while (s) {
	int len = strlen(s);
	if (len && s[len-1] == '\015')
	    s[--len] = '\0';
	if ((len==0) || (prefix(s, "COMMENT"))) {
	    s = fgets(s,256,pF);
	} else break;
    }
    return(s);
}


/*
 * make a byte from the first two hex characters in s
 * the byte is bitmap_bit_order MSBFirst
 */
static unsigned char
hexbyte(s)
    char *s;
{
    unsigned char b = 0;
    register char c;
    int i;

    for (i=2; i; i--) {
	c = *s++;
	if ((c >= '0') && (c <= '9'))
	    b = (b<<4) + (c - '0');
	else if ((c >= 'A') && (c <= 'F'))
	    b = (b<<4) + 10 + (c - 'A');
	else if ((c >= 'a') && (c <= 'f'))
	    b = (b<<4) + 10 + (c - 'a');
	else
	    return 0; /* bad data */
    } 
    return b;
}


/************************************************/
/************************************************/

static int ReadFont(name,tabchar,head,pprem)
/* lit le fichier  au format BDF name et
   rempli le tableau tabchar[MAXFONTCARD] de fedchar, et 
   initialise de BdfHeader head */
/* rend 1 en cas de succes, 0 si rien, -1 sinon */

	char * name ;
	fedchar * tabchar ;
	BdfHeader * head ;
        int * pprem ;

{   	int i ;
	char filename[128] ;	/* nom absolu */
        char linebuf[256] ;
	char namebuf[100];
	int yRes ;
	int nchars;
	int first = 1 ;

	if (*name=='\0') return 0;

	if (strcmp(name,"initfont") == 0 ) {
		for (i=0 ; i<MAXFONTCARD ; i++ )
		    if (tabchar[i].hsize > 0) {
			tabchar[i].up = 0 ;
			tabchar[i].down = 0 ;
			tabchar[i].hsize = 0 ;
			tabchar[i].hincr = 0 ;
			tabchar[i].hadj = 0 ;
			*tabchar[i].name = '\0' ;

			Rast_Free(&tabchar[i].image) ;
		    }
		/* initialise header */
		strcpy(head->FamilyName,"xfedor");
		head->nprops = 2 ;
		strcpy(head->Properties[0],"FONT_ASCENT 20\n");
		strcpy(head->Properties[1],"FONT_DESCENT 12\n");
	        head->Size = 32 ;
		head->Resol= 72 ;
	
	       	strcpy(FileName,"No File");
		return 1 ;
	}


    if (*name == '~') 
	{
	    strcpy(filename,homedir);
	    strcat(filename,(name+1));
	} else
    if (index(name,'/') != NULL) strcpy(filename,name); 
    else {
	    strcpy(filename,dirbdf);
	    strcat(filename,name);
	}

    if (!Extension(filename,".bdf")) strcat(filename,".bdf");

    if ((pF = fopen( filename, "r")) == NULL) return fatal("Can't open file");

    getline(linebuf);
    if ((sscanf(linebuf, "STARTFONT %s", namebuf) != 1) ||
	strcmp(namebuf, "2.1"))  return fatal("bad 'STARTFONT 2.1'");

    getline(linebuf);
    if (sscanf(linebuf, "FONT %s", head->FamilyName) != 1)
	return fatal("bad 'FONT'");

    getline(linebuf);
    if (!prefix(linebuf, "SIZE"))
	return fatal("missing 'SIZE'");
    if ((sscanf(linebuf, "SIZE %d%d%d",
			 &head->Size, &head->Resol, &yRes) != 3))
	return fatal("bad 'SIZE'");
    if (head->Resol != yRes)
        return fatal("x and y resolution must be equal");

    getline(linebuf);
    if (!prefix(linebuf, "FONTBOUNDINGBOX"))
	return fatal("missing 'FONTBOUNDINGBOX'");

    getline(linebuf);
    if (prefix(linebuf, "STARTPROPERTIES")) {
	int nprops;
	int prop_fontasc, prop_fontdesc ;

	prop_fontasc = -1 ;
	prop_fontdesc = -1 ;

	sscanf(linebuf, "%*s%d", &nprops);
	if (nprops > 50) return fatal("Too Many Properties : %d",nprops);
	head->nprops = nprops ;

	getline(linebuf);
	while((nprops-- > 0) && !prefix(linebuf, "ENDPROPERTIES")) {
	    if (prefix(linebuf,"FONT_ASCENT"))
		prop_fontasc = nprops ; else
	    if (prefix(linebuf,"FONT_DESCENT"))
		prop_fontdesc = nprops ; 
	    strncpy(head->Properties[nprops],linebuf,80);

	    getline(linebuf);
	}
	if (!prefix(linebuf, "ENDPROPERTIES"))
	    return fatal("missing 'ENDPROPERTIES'");
	if ((prop_fontasc == -1) || (prop_fontdesc==-1))
	    return fatal("missing 'FONT_ASCENT' or 'FONT_DESCENT' properties?");
	if (nprops != -1)
	    return fatal("%d too few properties", nprops+1);

    } else { /* no properties */
	return fatal("missing 'PROPERTIES'");
    }

    getline(linebuf);
    if (!prefix(linebuf, "CHARS"))
        return fatal("missing 'CHARS'");
    sscanf(linebuf, "%*s%d", &nchars);
    if (nchars > MAXFONTCARD) return fatal("Too Many Chars : %d",nchars);

	/* on reinitialise la tabchar avant de le remplir partiellement */
	for (i=0 ; i<MAXFONTCARD ; i++ )
	    if (tabchar[i].hsize > 0) {
			tabchar[i].up = 0 ;
			tabchar[i].down = 0 ;
			tabchar[i].hsize = 0 ;
			tabchar[i].hincr = 0 ;
			tabchar[i].hadj = 0 ;
			*tabchar[i].name = '\0' ;

			Rast_Free(&tabchar[i].image) ;
		}
	
    getline(linebuf);
    while ((nchars-- > 0) && prefix(linebuf, "STARTCHAR"))  {
	char	charName[50];
	int	enc, enc2;	/* encoding */
	int  	t ;
	int	wx;	/* x component of width */
	int	wy;	/* y component of width */
	int	bw;	/* bounding-box width */
	int	bh;	/* bounding-box height */
	int	bl;	/* bounding-box left */
	int	bb;	/* bounding-box bottom */
	int	bytesperrow, row, ix, vidwidth ;
	char 	* p, * vidmem, *mem;
	char    buff[50];

	if (sscanf(linebuf, "STARTCHAR %s", charName) != 1)
	    return fatal("bad character name");

	getline( linebuf);
	if ((t=sscanf(linebuf, "ENCODING %d %d", &enc, &enc2)) < 1)
	    return fatal("bad 'ENCODING'");
	if ((enc < -1) || ((t == 2) && (enc2 < -1)))
	    fatal("bad ENCODING value");
	if (t == 2 && enc == -1)
	    enc = enc2;
	if (enc == -1) {
	    do {
	    	char *s = getline(linebuf);
		if (!s)
		    return fatal("Unexpected EOF");
	    } while (!prefix(linebuf, "ENDCHAR"));
	    getline(linebuf);
	    continue;
	}
	if (enc >= MAXFONTCARD)
	    return fatal("'%s' has encoding(=%d) too large", charName, enc);
	strncpy(tabchar[enc].name,charName,20);
	if (first) {
	  *pprem = enc ;
	  first = 0 ;
	}
	getline( linebuf);
	if (!prefix(linebuf, "SWIDTH"))
	    return fatal("bad 'SWIDTH'");
	sscanf( linebuf, "SWIDTH %d %d", &wx, &wy);
	tabchar[enc].vadj = wx;	/* pour le stocker */

	getline( linebuf);
	if (!prefix(linebuf, "DWIDTH"))
	    return fatal("bad 'DWIDTH'");
	sscanf( linebuf, "DWIDTH %d %d", &wx, &wy);
	tabchar[enc].hincr = wx ;
	tabchar[enc].vincr = wy ;/* pour le stocker */

	getline( linebuf);
	if (!prefix(linebuf, "BBX"))
	    return fatal("bad 'BBX'");
	sscanf( linebuf, "BBX %d %d %d %d", &bw, &bh, &bl, &bb);
	tabchar[enc].up = -(bh + bb) ;
	tabchar[enc].down = -bb ;
	tabchar[enc].hadj = bl;
	tabchar[enc].hsize = bw ;

	getline( linebuf);
	if (prefix(linebuf, "ATTRIBUTES"))
	    getline( linebuf);	

	if (!prefix(linebuf, "BITMAP"))
	    return fatal("missing 'BITMAP'");

	bytesperrow = (((bw)+7)>>3) ;	/* nb d'octet par ligne (<pad 8) */

	if (tabchar[enc].image != 0) Rast_Free(&tabchar[enc].image) ;

	/* alloc du plan pour ce bitmap bdf */
	mem = vidmem = ( char *) malloc (bytesperrow*
				   (tabchar[enc].down-tabchar[enc].up)) ;
	/* transfert de l'image vers vidmem : pad 8 */
	for (row=0; row < bh; row++) {
	    getline(linebuf);
	    p = linebuf ;
	    for ( ix=0; ix < bytesperrow ; ix++)
	    {
	        *vidmem++ = hexbyte(p);
	        p += 2;
	    }
	}

	/* fonction de conversion vidmem vers ClientImage :
	   on recupere une image issue du bitmap bdf monochrome */
	Rast_Put(&tabchar[enc].image,mem,
		 tabchar[enc].hsize,tabchar[enc].down-tabchar[enc].up);
	

	getline( linebuf);
	if (!prefix(linebuf, "ENDCHAR")) {
	    sprintf(buff,"%d missing 'ENDCHAR'",enc);
            return fatal(buff);
	  }

	getline( linebuf);		/* get STARTCHAR or ENDFONT */
    }

    if (!prefix(linebuf, "ENDFONT"))
        return fatal("missing 'ENDFONT'");
    if (nchars != -1)
        return fatal("%d too few characters", nchars+1);
	
        fclose(pF);
	return 1 ;
}


/************************************************/
/************************************************/
int GetExtraFont(name)
	char * name ;
{
	int rep ;

	rep = ReadFont(name,extratab,&xeader,&PremExtra);
	if (rep == 1) {
		strcpy(ExtraName,name);
		Extra_Resize() ;
	}
	return rep ;
}
