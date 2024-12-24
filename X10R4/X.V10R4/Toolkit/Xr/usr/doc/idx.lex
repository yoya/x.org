%{
/*
 *     Index Generator filter....   13NOV86--Paul Johnson   
 *
 *     expects records of the form:
 *
 *     Section# "\t" Page# "\t" MainEntry "\t" SubEntry"\n"
 * |   Section# "\t" Page# "\t" MainEntry "\t""\n"
 *
 */

#include <string.h>
#include <ctype.h>

#define	TRUE	1
#define	FALSE	0

char	curmain[80];
char	cursecnd[80];
char	cursec[10];
int	curpage;
int	diffMain = TRUE;
int	diffSecnd = TRUE;
int	secndry = FALSE;

%}

SECNUM	[A-Z]|[1-9]{1,2}
PAGNUM	[0-9]+
ENTRY	[^\t\n]+

%START PROCESS PAGNO MAIN SECND

%%

<INITIAL>^{SECNUM}	{
			strcpy(cursec,yytext);
			}
<INITIAL>"\t"		{
			BEGIN PAGNO;
			}
<PAGNO>{PAGNUM}		{
			curpage = atoi(yytext);
			}
<PAGNO>"\t"		{
			BEGIN MAIN;
			}
<MAIN>{ENTRY}		{
			if(strcmp(yytext,curmain)){
				strcpy(curmain,yytext);
				diffMain = TRUE;
				diffSecnd = TRUE;
			} else {
				diffMain = FALSE;
				}
			}
<MAIN>"\t\n"		{
			if(strlen(cursecnd) == 0){
				diffSecnd = FALSE;
			} else {
				diffSecnd = TRUE;
				strcpy(cursecnd,"");
				}
			secndry = FALSE;
	printentry(curmain,cursecnd,cursec,curpage,diffMain,diffSecnd,secndry);
			BEGIN INITIAL;
			}
<MAIN>"\t"		{
			secndry = TRUE;
			BEGIN SECND;
			}

<SECND>{ENTRY}		{
			if(strcmp(yytext,cursecnd)){
				strcpy(cursecnd,yytext);
				diffSecnd = TRUE;
			} else {
				diffSecnd = FALSE;
				}
			}
<SECND>"\n"		{
	printentry(curmain,cursecnd,cursec,curpage,diffMain,diffSecnd,secndry);
			BEGIN INITIAL;
			}



%%

printentry(curmain,cursecnd,cursec,curpage,diffMain,diffSecnd,secndry)
char	curmain[80];
char	cursecnd[80];
char	cursec[10];
int	curpage;
int	diffMain;
int	diffSecnd;
int	secndry;
{
	static char CurAlph;

/*
	printf("curmain = %s\tcursecnd = %s\tcursec = %s\tcurpage = %d\n",curmain,cursecnd,cursec,curpage);
	printf("diffMain = %d\tdiffSecnd = %d\tsecndry = %d\n\n",diffMain,diffSecnd,secndry);

*/
	if(CurAlph != toupper(curmain[0])){
		if(isalpha(curmain[0])){
			CurAlph = toupper(curmain[0]);
			printAlpha(CurAlph);
		} else if(isalpha(curmain[1])){
			if(CurAlph != toupper(curmain[1])){
			   CurAlph = toupper(curmain[1]);
			   printAlpha(CurAlph);
			}
		} else if(isalpha(curmain[2])){
			if(CurAlph != toupper(curmain[2])){
			   CurAlph = toupper(curmain[2]);
			   printAlpha(CurAlph);
			}
		}
	}
	if(diffMain || diffSecnd){
		printf("\n.LI\n%s",curmain);
		if(secndry)
			printf(",\n%s",cursecnd);
	}

	printf(",\n%s\\-%d",cursec,curpage);
}

printAlpha(Alpha)
char Alpha;
{
	printf("\n.SP\n.LI\n\\s14\\fB%c\\s0\\fP",Alpha);
}

yywrap()
{
	printf("\n.LE\n");
}
