
%{

/* 
     Copyright (c) 1991 xtexcad  V1.3 by K. Zitzmann

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.
*/


/* scanner definition yyscan.l */

/* 07.11.91 */
 
/* the scanner was defined by Ulrich Koch, Koblenz */



#include <string.h>
#include <ctype.h>

/* fuer atof */
#if defined(__TURBOC__)
#   include <stdlib.h>
# else
#   include "extdef.h"
#endif

#if defined(__TURBOC__)
#  define yywrap()  1
#endif

/* Prototypen von Funktionen, die von lex NACH ihrem Aufruf
   definiert werden
 */
int yylook(void);
int yyback(int *p, int m);


#include "yyscan.h"

char *readText(unsigned numBr);

float yyfloatval;
  /* Falls eine float erkannt wurde (Token TOK_FLOAT oder TOK_UNITLENGTH),
     enthaelt yyfloatval deren Wert.
   */
char yystrval[YY_MAX_LEN];
  /* Falls ein Text oder Buchstaben erkannt wurden (Token TOK_TEXT oder
     TOK_LETTERS_OPT), enthaelt yystrval deren Wert.
   */

enum {gtNo,gtNext,gtForceNext,gtForceNext2} getText;
  /* Gibt an, wann ein 'Text'-Argument vorkommen kann bzw. muss.
     Moegliche Werte:
       gtNo        : kann nicht
       gtNext      : kann nach der naechsten {
       gtForceNext : muss nach der naechsten {
       gtForceNext2: muss nach der uebernaechsten {
   */

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0

BOOL getUnitLen,
  /* Gibt an, ob die Laengenangabe fuer \unitlength gelesen werden soll. */
     getDim;
  /* Gibt an, ob die Dimension fuer \unitlength gelesen werden soll. */
%}


%s  inPictEnv  inPut  lookOut


ws           [ \t\n]
digit        [0-9]
letter       [A-Za-z]
sign         [+-]


%%


{ws}+                  {/* ignorieren */}
"%".*\n                {/* LaTeX-Kommentar ignorieren */}

<INITIAL>\\unitlength=                 getUnitLen = TRUE;
<INITIAL>{sign}?{digit}+               |
<INITIAL>{sign}?{digit}*"."{digit}*    {if(getUnitLen)
					  {
					    yyfloatval = (float) atof(yytext);
					    getUnitLen = FALSE;
					    getDim = TRUE;
					    return TOK_UNITLENGTH;
					  }
				       }
<INITIAL>pt           {if(getDim)
			 {
			   getDim = FALSE;
			   return TOK_DIM_PT;
			 }
		      }
<INITIAL>cm           {if(getDim)
			 {
			   getDim = FALSE;
			   return TOK_DIM_CM;
			 }
		      }
<INITIAL>mm           {if(getDim)
			 {
			   getDim = FALSE;
			   return TOK_DIM_MM;
			 }
		      }
<INITIAL>pc           {if(getDim)
			 {
			   getDim = FALSE;
			   return TOK_DIM_PC;
			 }
		      }
<INITIAL>in           {if(getDim)
			 {
			   getDim = FALSE;
			   return TOK_DIM_IN;
			 }
		      }

<INITIAL>\\begin\{picture\}       {BEGIN inPictEnv; return TOK_BEGIN;}

<INITIAL>.|\n                     {/* ignorieren */}


<inPictEnv>\\bezier		return TOK_BEZIER;
<inPictEnv>\\put                {BEGIN inPut; return TOK_PUT;}

<lookOut>\\framebox             {getText = gtForceNext;  BEGIN inPictEnv;
				 return TOK_FRAMEBOX;}
<lookOut>\\makebox              {getText = gtForceNext;  BEGIN inPictEnv;
				 return TOK_MAKEBOX;}
<lookOut>\\dashbox              {getText = gtForceNext2; BEGIN inPictEnv;
				 return TOK_DASHBOX;}
<lookOut>\\line                 {BEGIN inPictEnv; return TOK_LINE;}
<lookOut>\\vector               {BEGIN inPictEnv; return TOK_VECTOR;}
<lookOut>\\circle               {BEGIN inPictEnv; return TOK_CIRCLE;}
<lookOut>\\circle\*             {BEGIN inPictEnv; return TOK_CIRCLE_AST;}
<lookOut>\\oval                 {BEGIN inPictEnv; return TOK_OVAL;}
<lookOut>\\rule                 {BEGIN inPictEnv; return TOK_RULE;}

<lookOut>\}			{/* 'leerer' Text */
				 yystrval[0] = '\0';
				 BEGIN inPictEnv;
				 return TOK_TEXT;}
<lookOut>.			{/* 'nichtleerer' Text */
				 yystrval[0] = yytext[0];
				 (void) strcpy(&yystrval[1],
				    readText(yytext[0] == '{'));
				 BEGIN inPictEnv;
				 return TOK_TEXT;}

<inPictEnv>\\unitlength           return TOK_DIM_UNITLENGTH;
<inPictEnv>pt			  return TOK_DIM_PT;
<inPictEnv>cm			  return TOK_DIM_CM;
<inPictEnv>mm			  return TOK_DIM_MM;
<inPictEnv>pc			  return TOK_DIM_PC;
<inPictEnv>in			  return TOK_DIM_IN;

<inPictEnv,inPut>{sign}?{digit}+               |
<inPictEnv,inPut>{sign}?{digit}*"."{digit}*    {yyfloatval = (float) atof(yytext);
				 	        return TOK_FLOAT;}

<inPictEnv>\{          {if(getText == gtForceNext)
			    {
			      (void) strcpy(yystrval,readText(0));
			      getText = gtNo;
			      return TOK_TEXT;
			    }
			  else if(getText == gtForceNext2)
			    getText = gtForceNext;}

<inPut>\{		  BEGIN lookOut;
<inPictEnv>\}             {/* ignorieren */}
<inPictEnv,inPut>[(),]    {/* ignorieren */}

<inPictEnv>\\end\{picture\}     return TOK_END;

<inPictEnv>\[({letter}{1,2})?\]   {(void) strcpy(yystrval,&yytext[1]);
				   yystrval[yyleng - 2] = '\0';
				   return TOK_LETTERS_OPT;}

<inPictEnv>.        {(void) fprintf(stderr,
		       "yylex: unrecognized char '%c' (%#X) in line %d\n",
		       yytext[0],yytext[0],yylineno);}


%%


/* Muss vor dem Beginn des Scannens aufgerufen werden.
   Setzt die Datei auf fp und geht in den richtigen Zustand
   ('eat up') ueber. Die Datei fp muss offen sein!
 */
void yyinitscan(FILE *fp)
  {
    yyin = fp;

    BEGIN INITIAL;

    getUnitLen = FALSE;
    getDim = FALSE;
  } /* yyinitscan */


/* Liest (LaTeX-) Text, der als Argument eines Befehls auftritt,
   bis (einschliesslich) zur schliessenden Argumentklammer.
   Kommt mit verschachtelten { } zurecht. Liefert den Text (ohne
   die Argumentklammern) zurueck. DIESES ERGEBNIS MUSS SOFORT
   KOPIERT WERDEN, DA ES DIE ADRESSE EINES LOKALEN ARRAYS IST!!
   Im Fehlerfall (unerwartetes Dateiende) liefert die Funktion
   einen Zeiger auf den leeren String zurueck.
 */
char *readText(unsigned numBr)
  {
    static char text[YY_MAX_LEN];
    unsigned    i = 0;
    char        ch;
    unsigned    numBraces = numBr + 1;


    /* pre: (numBr + 1) '{' wurden schon ueberlesen */
    while(numBraces > 0)
      {
	if((ch = input()) == '\0' /*end of file*/)
	      {
		(void) fprintf(stderr,
		  "readText: premature end of file in line %d\n",
		  yylineno);
		return "";
	      }
	      
	if(ch == '{')
	    numBraces++;
	  else if(ch == '}')
	    numBraces--;

	if(i >= YY_MAX_LEN)
	      {
		(void) fprintf(stderr,
		  "readText: text argument too long in line %d\n",
		  yylineno);
		return "";
	      }
	text[i++] = ch;
      }
    /* jetzt ist die schliessende Argumentklammer das letzte Zeichen
       in text */
    text[--i] = '\0';

    return text;
  } /* readText */
