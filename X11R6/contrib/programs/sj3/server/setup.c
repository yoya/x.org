/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: setup.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:56 $
 */




#include "sj_sysvdef.h"
#include <stdio.h>
#include <ctype.h>
#ifdef SVR4
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/param.h>
#include "sj_typedef.h"
#include "sj_const.h"
#include "sj_var.h"
#include "sj_dict.h"
#include "sj_struct.h"
#include "sj_rename.h"
#include "Const.h"
#include "Struct.h"
#include "sj_global.h"

typedef	struct	strlist {
	u_char	*str1;
	u_char	*str2;
	struct	strlist	*link;
} StrList;

char	program_name	[PathNameLen];		
char	runcmd_file	[PathNameLen];		

char	*debug_file	= NULL;			
int	debug_level	= -1;			
int	fork_flag	= -1;			

int	max_client	= -1;			
char	*dict_dir	= NULL;			
StrList	*read_dict	= NULL;
char	*error_file	= NULL;			
char	*log_file	= NULL;
char	*port_name	= NULL;
#ifdef TLI
char	*port_number	= NULL;
#else
int	port_number	= -1;
#endif
char	*socket_name	= NULL;
#ifdef	LOCK_FILE
char	*lock_file	= NULL;
#endif
int	dir_mode	= -1;			
int	file_mode	= -1;			
StrList	*allow_user	= NULL;
#ifdef TLI
char    *proto_name     = NULL;
#endif

static	int	line_number;			

char	*malloc();



#ifdef	OLD
#else
RcError(p)
char	*p;
{
	fprintf(stderr, "%s: \"%s\", line %d: %s\r\n",
		program_name, runcmd_file, line_number, p);
	fflush(stderr);
	exit(1);
}
RcWarning(p)
char	*p;
{
	fprintf(stderr, "%s: warning: \"%s\", line %d: %s\r\n",
		program_name, runcmd_file, line_number, p);
	fflush(stderr);
}
#endif



static	cmpstr(src, dst)
u_char	*src, *dst;
{
	int	flg;
	int	c;

	flg = (isupper(*src)) ? -1 : 0;
	while (*src) {
		c = flg ? *dst : (isupper(*dst) ? tolower(*dst) : *dst);
		if (*src != c) return -1;
		src += 1;
		dst += 1;
	}
	if (*dst) return -1;
	return 0;
}



static	u_char	*get_str(p, dst)
u_char	*p;
char	**dst;
{
	if (!*dst) {
		*dst = malloc(strlen((char *)p) + 1);
		if (!*dst) RcError("no more memory");
		strcpy(*dst, (char *)p);
	}
	while (*p++) ;
	return p;
}
static	u_char	*get_int(p, dst)
u_char	*p;
int	*dst;
{
	register char	*fmt;

	fmt = (*p == '0') ? "%o" : "%d";
	if (sscanf((char *)p, fmt, dst) != 1) RcError("bad integer");
	while (*p++) ;
	return p;
}
static	u_char	*get_flag(p, dst)
u_char	*p;
int	*dst;
{
	if (sscanf((char *)p, "%d", dst) == 1)
		;
	else if (!cmpstr("on", p))
		*dst = -1;
	else if (!cmpstr("off", p))
		*dst = 0;
	else
		RcError("bad flag");
	while (*p++) ;
	return p;
}
static	u_char	*get_list(p, dst)
u_char	*p;
StrList	**dst;
{
	StrList	*s1, *s2;

	if (*p) {
		s1 = (StrList *)malloc(sizeof(StrList));
		if (!s1) RcError("no more memory");
		s1 -> link = NULL;

		s1 -> str1 = (u_char *)malloc(strlen((char *)p) + 1);
		if (!(s1 -> str1)) RcError("no more memory");
		strcpy((char *)s1 -> str1, (char *)p);
		while (*p++) ;

		if (*p) {
			s1 -> str2 = (u_char *)malloc(strlen((char *)p) + 1);
			if (!(s1 -> str2)) RcError("no more memory");
			strcpy((char *)s1 -> str2, (char *)p);
			while (*p++) ;
		}
		else
			s1 -> str2 = NULL;

		if (*dst) {
			for (s2 = *dst ; s2 -> link ; s2 = s2 -> link)
				;
			s2 -> link = s1;
		}
		else
			*dst = s1;
	}

	return p;
}



struct	optlist {
	char	*optname;
	u_char	*(*optfunc)();
	int	optarg;
} option[] = {
	"DebugOut",	get_str,	(int)&debug_file,
	"DebugLevel",	get_int,	(int)&debug_level,
	"ForkFlag",	get_flag,	(int)&fork_flag,

	"PortName",	get_str,	(int)&port_name,
#ifdef TLI
	"PortNumber",	get_str,	(int)&port_number,
        "ProtoName",    get_str,        (int)&proto_name,
#else
	"PortNumber",	get_int,	(int)&port_number,
#endif
	"SocketName",	get_str,	(int)&socket_name,
#ifdef	LOCK_FILE
	"LockFile",	get_str,	(int)&lock_file;
#endif

	"maxclient",	get_int,	(int)&max_client,
	"dictdir",	get_str,	(int)&dict_dir,
	"readdict",	get_list,	(int)&read_dict,
	"errorout",	get_str,	(int)&error_file,
	"logout",	get_str,	(int)&log_file,
	"dirmode",	get_int,	(int)&dir_mode,
	"filemode",	get_int,	(int)&file_mode,
	"allowuser",	get_list,	(int)&allow_user,
	0, 0, 0
};



static	int	skip_blank(fp)
FILE	*fp;
{
	register int	c;

	c = getc(fp);
	while (c == ' ' || c == '\t') c = getc(fp);
	return c;
}
static	int	skip_line(fp)
FILE	*fp;
{
	register int	c;

	c = getc(fp);
	while (c != '\n' && c != EOF) c = getc(fp);
	return c;
}
static	readln(fp, p, len)
FILE	*fp;
char	*p;
int	len;		
{
	int	c;
	int	quote = EOF;
	int	flg = 0;

	c = skip_blank(fp);
	while (c != '\n' && c != EOF) {
		if (c == '\\') {
			if ((c = getc(fp)) == EOF) break;
			if (c == '\n') {
				line_number++; c = getc(fp); continue;
			}
			switch (c) {
			case 'n':	c = '\n'; break;
			case 't':	c = '\t'; break;
			case 'r':	c = '\r'; break;
			default:	break;
			}
		}
		else if (c == '^') {
			c = getc(fp);
			if (c < '@' || '_' < c) {
				ungetc(c, fp); c = '^';
			}
			else
				c -= '@';
		}
		else if (c == quote) {
			quote = EOF; c = getc(fp); continue;
		}
		else if (quote != EOF)
			;
		else if (c == '\'' || c == '"') {
			quote = c; c = getc(fp); continue;
		}
		else if (c == '#') {
			len -= 1; *p++ = 0; flg = 0;
			c = skip_line(fp); continue;
		}
		else if (c == ' ' || c == '\t') {
			len -= 1; *p++ = 0; flg = 0;
			c = skip_blank(fp); continue;
		}

		if (len > 2) { len -= 1; *p++ = c; flg = -1; }
		c = getc(fp);
	}
	line_number++;
	if (flg) *p++ = 0;
	*p++ = 0;

	return c;
}
read_line(fp, buf, len)
FILE	*fp;
char	*buf;
int	len;
{
	int	flg;

	do {
		flg = readln(fp, buf, len);
		if (*buf) return 0;
	} while (flg != EOF);

	return EOF;
}



void	read_runcmd()
{
	FILE	*fp;
	u_char	buf[BUFSIZ];
	struct	optlist *opt;
	u_char	*p;
	int	i;

	if (!(fp = fopen(runcmd_file, "r"))) {
		warning_out("Can't open run-command file \"%s\"", runcmd_file);
		return;
	}
	line_number = 0;
	while (read_line(fp, buf, sizeof(buf)) != EOF) {
		for (opt = option ; p = (u_char *)opt -> optname ; opt++)
			if (!cmpstr(p, buf)) break;
		if (p) {
			p = buf; while (*p++) ;
			p = (*opt->optfunc)(p, opt->optarg);
		}
		else
			RcError("undefined option");

		if (*p) RcError("too many argment");
	}

	fclose(fp);
}



static	set_defstr(d, s)
char	**d, *s;
{
	if (*d == NULL && s) {
		*d = malloc(strlen(s) + 1);
		if (!*d) error_out("failed at memory allocation");
		strcpy(*d, s);
	}
}
static	set_defint(d, s)
int	*d, s;
{
	if (*d < 0) *d = s;
}
void	set_default()
{
	set_defstr(&debug_file,		DebugOutFile);
	set_defstr(&error_file,		ErrorOutFile);
	set_defstr(&dict_dir,		DictRootDir);
	set_defstr(&log_file,		LogOutFile);
	set_defstr(&port_name,		PortName);
	set_defstr(&socket_name,	SocketName);
#ifdef	LOCK_FILE
	set_defstr(&lock_file,		LockFile);
#endif
#ifdef TLI
        set_defstr(&proto_name,         ProtoName);
#endif

	set_defint(&debug_level,	DebugLevel);
	set_defint(&fork_flag,		ForkFlag);
	set_defint(&max_client,		MaxClientNum);
#ifdef TLI
	set_defstr(&port_number,	PortNumber);
#else
	set_defint(&port_number,	PortNumber);
#endif
	set_defint(&dir_mode,		DirectryMode);
	set_defint(&file_mode,		DictFileMode);
}



void	parse_arg(argc, argv)
int	argc;
char	**argv;
{
	int	c;
	int	errflg = 0;
	char	*p;

	extern	char	*optarg, *strrchr();
	extern	int	optind;

	p = (p = strrchr(argv[0], '/')) ? p + 1 : argv[0];
	strcpy(program_name,	p);
	strcpy(runcmd_file,	RunCmdFile);

	while ((c = getopt(argc, argv, "f:")) != EOF) {
		switch (c) {
		case 'f':
			strcpy(runcmd_file, optarg);
			break;

		case '?':
		default:
			errflg++;
			break;
		}
	}

	if (errflg || optind < argc) {
		fprintf(stderr, "Usage: %s [-f RunCmdFile]\n", program_name);
		exit(1);
	}
}


void
preload_dict()
{
	StrList	*p;
	char	filename[PathNameLen];

	work_base = (Global *)malloc(sizeof(Global));
	if (!work_base) {
		warning_out("can't load default dictionary(not enough memory)");
		return;
	}

	for (p = read_dict ; p ; p = p -> link) {
		strncpy(filename, (char *)p -> str1, sizeof(filename));
		filename[sizeof(filename) - 1] = '\0';
		if (make_full_path(filename))
			warning_out("too long filename \"%s\"", p -> str1);

		else if (opendict(filename, p -> str2))
			logging_out("load dictionary \"%s\"", filename);

		else
			warning_out("can't open dictionary \"%s\"", filename);
	}

	free(work_base);
}



static	str_match(s, d)
char	*s;
char	*d;
{
	while (*d) {
		if (*s == '*') {
			while (*s == '*') s++;
			if (!*s) return TRUE;
			while (*d) {
				if (str_match(s, d)) return TRUE;
				d++;
			}
			return FALSE;
		}
		if (*s != '?' && *s != *d) break;
		s++;
		d++;
	}
	return (!*s) ? TRUE : FALSE;
}



check_user(user, host)
char	*user;
char	*host;
{
	StrList	*p;

	if (!allow_user) return TRUE;

	for (p = allow_user ; p ; p = p -> link) {
		if (!str_match(p -> str1, user)) continue;

		if (p -> str2)
			if (!str_match(p -> str2, host)) continue;

		return TRUE;
	}

	return FALSE;
}
