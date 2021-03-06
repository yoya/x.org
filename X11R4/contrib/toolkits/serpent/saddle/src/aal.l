%{

static char rcsid [] =
	"$Header: /u/04c/mnt/integ/saddle/src/RCS/aal.l,v 1.6 89/10/18 10:37:53 ejh Exp $";

/*--------------------------------------------------------------------------*\
|
|  Name:  aal.l
|
|  Description:
|	This is the lexical analyzer for SADDLE
|
|  Component of:
|	SADDLE processor
|
|  Related files:
|	aay.y
|
|  Project:
|	SERPENT System
|	User Interface Prototyping
|	Software Engineering Institute
|	Carnegie Mellon University
|	Pittsburgh, PA  15213
|
|  Version: 1.0
|
|  Owner:  
|	Len Bass
|	ljb@sei.cmu.edu
|
|  Author:  
|	Erik J. Hardy
|	erik@sei.cmu.edu
|
|  Change History:
$Log:	aal.l,v $
 * Revision 1.6  89/10/18  10:37:53  ejh
 * redefined YYLMAX to BUFSIZ; switched over to u22_convert_string and
 * u22_copy_string
 * 
 * Revision 1.5  89/02/10  13:00:54  ejh
 * moved lex offset symbol to top of file to make it lex
 * 
 * Revision 1.4  89/02/09  19:23:28  ejh
 * added copyright notice
 * 
 * Revision 1.3  89/01/11  14:54:08  ejh
 * changed EXECSTRING to be integer; added code to handle empty exec strings
 * 
 * Revision 1.2  88/12/06  15:37:58  ejh
 * lex problem with Log
 * 
 * Revision 1.1  88/12/06  14:28:43  ejh
 * Initial revision
 * 
|	13 Nov 1988:	created
|	14 Dec 1987:	original version
|
\*--------------------------------------------------------------------------*\
|
|   Distribution: Approved for public release; distribution is unlimited.
|
|   Copyright (c) 1989  by  Carnegie  Mellon  University, Pittsburgh, PA.  The
|  Software Engineering  Institute  (SEI) is a federally  funded  research and
|  development center established  and  operated by Carnegie Mellon University
|  (CMU).  Sponsored  by  the  U.S.  Department  of   Defense  under  contract
|  F19628-85-C-0003,  the  SEI  is  supported  by  the  services  and  defense
|  agencies, with the U.S. Air Force as the executive contracting agent.
|
|    Permission to use,  copy,  modify, or  distribute  this  software and its
|  documentation for  any purpose and without fee  is hereby granted, provided
|  that  the above copyright notice appear  in  all copies and that both  that
|  copyright  notice  and   this  permission  notice  appear   in   supporting
|  documentation.   Further,  the  names  Software  Engineering  Institute  or
|  Carnegie  Mellon  University  may  not be used in  advertising or publicity
|  pertaining to distribution of the software without specific, written  prior
|  permission.  CMU makes no claims  or representations  about the suitability
|  of this software for any purpose.  This software is provided "as is" and no
|  warranty,  express  or  implied,  is  made  by  the  SEI  or CMU, as to the
|  accuracy  and  functioning of the program and related program material, nor
|  shall   the  fact of  distribution   constitute  any   such  warranty.   No
|  responsibility is assumed by the SEI or CMU in connection herewith.
|
\*--------------------------------------------------------------------------*/

#include	"string.h"
#include	<stdio.h>

#include	"u22.h"
#include	"a22.h"
#include	"aap.h"
#include	"aay.h"	/* generated by yacc */

/*	these are to redefine input and unput */

#undef	YYLMAX
#define	YYLMAX BUFSIZ

char	lextext [YYLMAX];	/* line buffer */
char	*lextp = lextext;

#define	get_line() \
	(*lextp=='\0'?(fgets(lextext,YYLMAX-1,yyin),lextp=lextext,*lextp++)\
	:*lextp++)

#undef	input
#define	input() \
	(((yytchar=yysptr>yysbuf?U(*--yysptr):get_line())=='\n'?yylineno++\
	:0),feof(yyin)?0:yytchar)

#undef	unput
#define	unput(c) \
	{yytchar=(c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}

int	i;

%}

%%

[bB][oO][oO][lL][eE][aA][nN]	{

/*	let's look for keywords, like BOOLEAN, etc. */

	DEBUG3 (fprintf (stderr, "aal: found the word BOOLEAN\n"));
	return (xBOOLEAN);
	}

[bB][uU][fF][fF][eE][rR]	{
	DEBUG3 (fprintf (stderr, "aal: found the word BUFFER\n"));
	return (xBUFFER);
	}

[dD][aA][tT][aA]	{
	DEBUG3 (fprintf (stderr, "aal: found the word DATA\n"));
	return (xDATA);
	}

[eE][nN][dD]	{
	DEBUG3 (fprintf (stderr, "aal: found the word END\n"));
	return (xEND);
	}

[iI][dD]	{
	DEBUG3 (fprintf (stderr, "aal: found the word ID\n"));
	return (xID);
	}

[iI][nN][tT][eE][gG][eE][rR]	{
	DEBUG3 (fprintf (stderr, "aal: found the word INTEGER\n"));
	return (xINTEGER);
	}

[oO][fF]	{
	DEBUG3 (fprintf (stderr, "aal: found the word OF\n"));
	return (xOF);
	}

[rR][eE][aA][lL]	{
	DEBUG3 (fprintf (stderr, "aal: found the word REAL\n"));
	return (xREAL);
	}

[rR][eE][cC][oO][rR][dD]	{
	DEBUG3 (fprintf (stderr, "aal: found the word RECORD\n"));
	return (xRECORD);
	}

[sS][hH][aA][rR][eE][dD]	{
	DEBUG3 (fprintf (stderr, "aal: found the word SHARED\n"));
	return (xSHARED);
	}

[sS][tT][rR][iI][nN][gG]	{
	DEBUG3 (fprintf (stderr, "aal: found the word STRING\n"));
	return (xSTRING);
	}

[a-zA-Z][a-zA-Z0-9_]*	{

/*	now we look for names; any alphanumeric string (including underscores)
	which starts with a letter is valid text; if it's bigger than 32
	characters, truncate it to 32, then convert it to lower case */

	if (yyleng > 32)
	    {
	    yytext [32] = '\0';
	    yyleng = 32;
	    }
	yylval.xstring = u22_convert_string (yytext, LOWERCASE);
	DEBUG3 (fprintf (stderr, "aal: found a NAME: %s\n", yytext));
	return (xTEXT);
	}

"<<"[^(">>")]*">>"	{

/*	now we look for execution strings */

	i = 2;
	while (yytext [i] == ' ' || yytext [i] == '\t')
	    i++;
	if (yytext [i] == '-')
	    aap_error (yytext, EXEC_ERROR, NULL);
	else if (yytext [i] != '>' || yytext [i + 1] != '>')
	    {
	    yyleng = yyleng - 2;
	    yytext [yyleng] = '\0';
	    aap_tap_exec = u22_copy_string (yytext + i);
	    DEBUG3 (fprintf (stderr, "aal: found an EXECSTRING: %s\n",
		yytext + i));
	    }
	return (xEXECSTRING);
	}

[0-9]+	{

/*	now we look for numbers */

	yylval.xint = atoi (yytext);
	DEBUG3 (fprintf (stderr, "aal: found a NUMBER: %s\n", yytext));
	return (xNUMBER);
	}

"("	{

/*	these are all the special characters */

	DEBUG3 (fprintf (stderr, "aal: found a LEFT PAREN\n"));
	return (xLEFTPAREN);
	}

")"	{
	DEBUG3 (fprintf (stderr, "aal: found a RIGHT PAREN\n"));
	return (xRIGHTPAREN);
	}

"{"	{
	DEBUG3 (fprintf (stderr, "aal: found a LEFT CURLY BRACKET\n"));
	return (xLEFTCURLY);
	}

"}"	{
	DEBUG3 (fprintf (stderr, "aal: found a RIGHT CURLY BRACKET\n"));
	return (xRIGHTCURLY);
	}

"["	{
	DEBUG3 (fprintf (stderr, "aal: found a LEFT SQUARE BRACKET\n"));
	return (xLEFTBRACKET);
	}

"]"	{
	DEBUG3 (fprintf (stderr, "aal: found a RIGHT SQUARE BRACKET\n"));
	return (xRIGHTBRACKET);
	}

":"	{
	DEBUG3 (fprintf (stderr, "aal: found a COLON\n"));
	return (xCOLON);
	}

";"	{
	DEBUG3 (fprintf (stderr, "aal: found a SEMICOLON\n"));
	return (xSEMICOLON);
	}

","	{
	DEBUG3 (fprintf (stderr, "aal: found a COMMA\n"));
	return (xCOMMA);
	}

\n[ \t]*!.*$	{

/*	comment starter: read the whole line (but not the EOL) and return
	what was found immediately after the ! */

	i = 1;
	while (yytext [i] != '!')
	    i++;
	yylval.xstring = u22_copy_string (yytext + i + 1);
	DEBUG3 (fprintf (stderr, "aal: found a COMMENT\n"));
	return (xCOMMENT);
	}

!.*$	{

/*	inline comment: read the whole line (but not the EOL) and return
	what was found immediately after the ! */

	yylval.xstring = u22_copy_string (yytext);
	DEBUG3 (fprintf (stderr, "aal: found an INLINE COMMENT\n"));
	return (xINLINECOMMENT);
	}

\n*"#".*$	{

/*	if a pound sign is found at the beginning of the line, skip the
	whole line (dropping left by the c preprocessor) */

	}

[ \t\n]	{

/*	skip spaces, tabs, and EOLs */

	}


.	{

/*	anything else is an error */

	aap_error (yytext, FATAL_ERROR, BAD_LINE);
	}
