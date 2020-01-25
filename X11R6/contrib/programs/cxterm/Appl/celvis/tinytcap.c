/* tinytcap.c */

/* This file contains functions which simulate the termcap functions, but which
 * can only describe the capabilities of the ANSI.SYS and NANSI.SYS drivers on
 * an MS-DOS system or the VT-52 emulator of an Atari-ST.  These functions
 * do *NOT* access a "termcap" database file.
 */

#include "config.h"
#if MSDOS || TOS

#define CAP(str) CAP2((str)[0], (str)[1])
#define CAP2(a,b) (((a) << 8) + ((b) & 0xff))

#if MSDOS
# define VAL2(v,a)	(a)
# define VAL3(v,a,n)	(nansi ? (n) : (a))
static int	nansi = 0;
#endif

#if TOS
# define VAL2(v,a)	(v)
# define VAL3(v,a,n)	(v)
#endif


/*ARGSUSED*/
int tgetent(bp, name)
	char	*bp;	/* buffer for storing the entry -- ignored */
	char	*name;	/* name of the entry */
{
#if MSDOS
	if (!strcmp(name, "ansi") || !strcmp(name, "nansi"))
	{
		nansi = (name[0] == 'n');
		return 1;
	}
#endif
#if TOS
	if (!strcmp(name, "vt52"))
	{
		return 1;
	}
#endif
	return 0;
}

int tgetnum(id)
	char	*id;
{
	switch (CAP(id))
	{
	  case CAP2('l','i'):	return 25;
	  case CAP2('c','o'):	return 80;
	  case CAP2('s','g'):	return 0;
	  case CAP2('u','g'):	return 0;
	  default:		return -1;
	}
}

int tgetflag(id)
	char	*id;
{
	switch (CAP(id))
	{
	  case CAP2('a','m'):	return 1;
	  case CAP2('b','s'):	return 1;
	  case CAP2('m','i'):	return 1;
	  default:		return 0;
	}
}

/*ARGSUSED*/
char *tgetstr(id, bp)
	char	*id;
	char	**bp;	/* pointer to pointer to buffer - ignored */
{
	switch (CAP(id))
	{
	  case CAP2('c','e'):	return VAL2("\033K", "\033[K");
	  case CAP2('c','l'):	return VAL2("\033E", "\033[2J");

	  case CAP2('a','l'):	return VAL3("\033L", (char *)0, "\033[L");
	  case CAP2('d','l'):	return VAL3("\033M", (char *)0, "\033[M");

	  case CAP2('c','m'):	return VAL2("\033Y%i%+ %+ ", "\033[%i%d;%dH");
	  case CAP2('d','o'):	return VAL2("\033B", "\033[B");
	  case CAP2('n','d'):	return VAL2("\033C", "\033[C");
	  case CAP2('u','p'):	return VAL2("\033A", "\033[A");
	  case CAP2('t','i'):	return VAL2("\033e", "");
	  case CAP2('t','e'):	return VAL2("", "");

	  case CAP2('s','o'):	return VAL2("\033p", "\033[7m");
	  case CAP2('s','e'):	return VAL2("\033q", "\033[m");
	  case CAP2('u','s'):	return VAL2((char *)0, "\033[4m");
	  case CAP2('u','e'):	return VAL2((char *)0, "\033[m");
	  case CAP2('V','B'):	return VAL2((char *)0, "\033[1m");
	  case CAP2('V','b'):	return VAL2((char *)0, "\033[m");

	  case CAP2('k','u'):	return "#H";
	  case CAP2('k','d'):	return "#P";
	  case CAP2('k','l'):	return "#K";
	  case CAP2('k','r'):	return "#M";
	  case CAP2('H','M'):	return "#G";
	  case CAP2('E','N'):	return "#O";
	  case CAP2('P','U'):	return "#I";
	  case CAP2('P','D'):	return "#Q";

	  default:		return (char *)0;
	}
}

/*ARGSUSED*/
char *tgoto(cm, destcol, destrow)
	char	*cm;	/* cursor movement string -- ignored */
	int	destcol;/* destination column, 0 - 79 */
	int	destrow;/* destination row, 0 - 24 */
{
	static char buf[30];

#if MSDOS
	sprintf(buf, "\033[%d;%dH", destrow + 1, destcol + 1);
#endif
#if TOS
	sprintf(buf, "\033Y%c%c", ' ' + destrow, ' ' + destcol);
#endif
	return buf;
}

/*ARGSUSED*/
void tputs(cp, affcnt, outfn)
	char	*cp;		/* the string to output */
	int	affcnt;		/* number of affected lines -- ignored */
	int	(*outfn)();	/* the output function */
{
	while (*cp)
	{
		(*outfn)(*cp);
		cp++;
	}
}
#endif
