
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


/* scanner definition header-file yyscan.h */


/* by Ulrich Koch, Koblenz */

/* 06.11.91 */


/* fuer FILE */
#include <stdio.h>


/* Tokens, die von yylex in yyscan.l
   zurueckgeliefert werden
*/

#define   TOK_BEGIN           1
#define   TOK_END             2

#define   TOK_TEXT            3
#define   TOK_LETTERS_OPT     4

#define   TOK_FLOAT           5

#define   TOK_PUT             6
#define   TOK_FRAMEBOX        7
#define   TOK_DASHBOX         8
#define   TOK_LINE            9
#define   TOK_VECTOR         10
#define   TOK_CIRCLE         11
#define   TOK_CIRCLE_AST     12
#define   TOK_OVAL           13
#define   TOK_RULE           14
#define   TOK_MAKEBOX        15
#define	  TOK_BEZIER	     16

#define   TOK_UNITLENGTH     20

#define   TOK_DIM_PT         21
#define   TOK_DIM_CM         22
#define   TOK_DIM_MM         23
#define   TOK_DIM_PC         24
#define   TOK_DIM_IN         25
#define   TOK_DIM_UNITLENGTH 26


/* Prototypen der Funktionen, die von yyscan.l 'exportiert' werden */

int yylex(void);
  /* Scanner; liefert das jeweils naechste erkannte Token */

void yyinitscan(FILE *fp);
  /* MUSS vor dem Beginn des Scannens aufgerufen werden (also vor dem
     ersten Aufruf von yylex).
     Setzt die Eingabedatei auf fp. Dabei MUSS die Datei fp schon offen sein!
   */


/* Daten, die von yyscan.l 'exportiert' werden */

extern float yyfloatval;
  /* Falls eine float erkannt wurde (Token TOK_FLOAT oder TOK_UNITLENGTH),
     enthaelt yyfloatval deren Wert.
   */

#define YY_MAX_LEN  200

extern char yystrval[YY_MAX_LEN];
  /* Falls ein Text oder Buchstaben erkannt wurden (Token TOK_TEXT oder
     TOK_LETTERS_OPT), enthaelt yystrval deren Wert.
   */
