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
 * $SonyRCSfile: level1.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:54 $
 */



#include "sj_sysvdef.h" 
#include <stdio.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#ifdef TLI              
#include <fcntl.h>
#include <tiuser.h>
#include <stropts.h>
#include <netdir.h>
#include <netconfig.h>
#else
#include <sys/socket.h>
#endif
#include <signal.h>

#include "Const.h"
#include "sj3cmd.h"
#include "sj3err.h"
#include "sj3lib.h"
#include "sj3lowlib.h"

#ifdef SVR4
#define signal sigset
#endif

char	*getlogin();

#ifndef lint
static	char	rcsid_sony[] = "$SonyId: level1.c,v 1.1 1994/06/03 08:03:54 notanaka Exp $ SONY;";
#endif

int	sj3_error_number;
char	*sj3_socket_name	= SocketName;
char	*sj3_port_name	= PortName;
#ifdef TLI
extern  int t_errno;
#else
int	sj3_port_number	= PortNumber;
#endif

#define BUFFER_SIZE BUFSIZ  
static	char	*af_unix_str = "unix";
static	u_char	putbuf[BUFFER_SIZE];
static	u_char	getbuf[BUFFER_SIZE];	
static	int	putpos = 0;		
static	int	getlen = 0;		
static	int	getpos = 0;		
static	SJ3_CLIENT_ENV	*cliptr;
static	int	server_fd = -1;

static        int     sj3_timeout = 0;                

static  int     INTLEN = sizeof(int);
static  int     CMDLEN = sizeof(int);
static  int     ReadErrorFlag = FALSE;

#define	client_init(p)	{ \
		cliptr = (p); \
		if ((server_fd = cliptr -> fd) == -1) { \
			sj3_error_number = SJ3_NotOpened; \
			return ERROR; \
		} \
	}

static	put_flush()
{
	register int	i, j;
	register u_char	*p;

	for (i = putpos, p = putbuf ; i > 0 ; i -= j, p += j) {
#ifdef TLI
	  if ((j = write(server_fd, p, i)) <= 0) {
	    t_sndrel(server_fd);
            close(server_fd);  
#else
	  if ((j = write(server_fd, p, i)) <= 0) {
	    shutdown(server_fd, 2);
	    close(server_fd);
#endif
	    cliptr -> fd = server_fd = -1;
	    sj3_error_number = SJ3_ServerDown;
	    return(ERROR);
	  }
	}
	putpos = 0;
}
static	put_byte(c)
register int	c;
{
	putbuf[putpos++] = c;
}
static	put_word(c)
register int	c;
{
	put_byte(c >> 8);
	put_byte(c & 0xff);
}
static	put_int(c)
register int	c;
{
	put_byte(c >> (8 * 3));
	put_byte(c >> (8 * 2));
	put_byte(c >> (8 * 1));
	put_byte(c);
}
static	put_cmd(cmd)
register int	cmd;
{
        ReadErrorFlag = FALSE;
	putpos = getlen = 0;
	put_int(cmd);
}

#define put_string put_ndata

static	u_char	*put_ndata(p, n)
register u_char	*p;
register int     n;
{
	while (n-- > 0) put_byte(p ? *p++ : 0);
	return p;
}

static put_over(buflen, n, func1, str1, len1, func2, str2, len2, 
		func3, str3, len3, func4, str4, len4)
int buflen, n;
u_char *(*func1)(), *(*func2)(), *(*func3)(), *(*func4)();
u_char *str1, *str2, *str3, *str4;
int len1, len2, len3, len4;
{
#define ARGNUM 4
  u_char *(*func[ARGNUM])();
  u_char *data[ARGNUM];
  int len[ARGNUM];
  int i;

  func[0] = func1; data[0] = str1; len[0] = len1;
  func[1] = func2; data[1] = str2; len[1] = len2;
  func[2] = func3; data[2] = str3; len[2] = len3;
  func[3] = func4; data[3] = str4; len[3] = len4;

  for(i = 0; i < n; i++) {
    if (len[i] < buflen) {
      (*func[i])(data[i], len[i]);
      buflen -= len[i];
    } else {
      while (len[i] > 0) {
        data[i] = (*func[i])(data[i], (len[i] < buflen) ? len[i] : buflen);
	if (put_flush() == ERROR) return ERROR;
	len[i] -= buflen;
	buflen = BUFFER_SIZE;
      }
    }
  }
  if ((buflen != BUFFER_SIZE) && (put_flush() == ERROR))
    return ERROR;

  return  0;
}

static  get_buffer()
{
        if (ReadErrorFlag) return ERROR;  
        getpos = getlen = 0;

        getlen = read(server_fd, getbuf, sizeof(getbuf));
        if (getlen <=0) {
#ifdef TLI
          t_close(server_fd);  
#else
	  shutdown(server_fd, 2);
	  close(server_fd);
#endif
	  cliptr -> fd = server_fd = -1;
	  sj3_error_number = SJ3_ServerDown;
	  return ERROR;
	}
        return getlen;
}

static	get_byte()
{
        if ((getpos >= getlen) && (get_buffer() == ERROR)) {
	  ReadErrorFlag = TRUE;
	  return 0;
	}
	return(getbuf[getpos++] & 0xff);
}
static	get_word()
{
	register int	i;

	i = get_byte();
	return ((i << 8) | get_byte());
}
static	get_int()
{
        register int    i0;
        register int    i1;
        register int    i2;

	i0 = get_byte();
	i1 = get_byte();
	i2 = get_byte();
	return ((i0 << (8*3)) | (i1 << (8*2)) | (i2 << (8*1)) | get_byte());
}
static	u_char	*get_string(p)
register u_char	*p;
{
	register int	c;

	do {
		*p++ = c = get_byte();
	} while (c);

	return p;
}
static	get_nstring(p, n)
register u_char	*p;
register int	n;
{
	register int	c;

	c = get_byte();
	while (c) {
		if (n > 1) { *p++ = c; n--; }
		c = get_byte();
	}
	if (n > 0) *p++ = 0;
	return n;
}
static	u_char	*get_ndata(p, n)
register u_char	*p;
register int	n;
{
	while (n-- > 0) *p++ = get_byte();
	return p;
}
static	skip_string()
{
	while (get_byte()) ;
}
static	skip_ndata(n)
register int	n;
{
	while (n-- > 0) get_byte();
}
#ifndef TLI
static	open_unix()
{
	int	fd;
	struct	sockaddr_un	sunix;

	sunix.sun_family      = AF_UNIX;
	strcpy(sunix.sun_path, sj3_socket_name);

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == ERROR) {
		sj3_error_number = SJ3_OpenSocket;
		return ERROR;
	}

	if (connect(fd, (struct sockaddr *)&sunix, strlen(sunix.sun_path)+sizeof(sunix.sun_family))
			== ERROR) {
		sj3_error_number = SJ3_ConnectSocket;
		return ERROR;
	}

	return fd;
}
#endif

sj3_set_timeout(timeout)   
int   timeout;
    {
        sj3_timeout = timeout;  
    }
static        void
connect_timeout()
	{
	}


static	open_inet(host)
char	*host;
{
#ifdef TLI
	int val;
        char *t_alloc();
        struct t_call *callptr;
        struct netconfig *nconf;
        struct nd_hostserv hserv;
        struct nd_addrlist *addrs;
        void *handlep;
	char host_name[BUFSIZ], proto_name[BUFSIZ], *port_name, *tmp_addr;
	extern char *strchr();
        extern void *setnetpath();
        extern struct netconfig *getnetpath();
#else 
	struct	hostent	*hp;
	struct	servent	*sp;
	int	port;
	struct	sockaddr_in	sin;
#endif
	int	fd, ret;

#ifdef TLI
        if ((tmp_addr = strchr(host, '/')) != NULL) {
		strncpy(proto_name, host, (unsigned int) (tmp_addr - host));
		proto_name[(unsigned int) (tmp_addr - host)] = '\0';
                tmp_addr++;
        } else {
		strncpy(proto_name, ProtoName, strlen(ProtoName));
		proto_name[strlen(ProtoName)] = '\0';
		tmp_addr = host;
	}
        if ((port_name = strchr(tmp_addr, ':')) != NULL) {
		strncpy(host_name, tmp_addr, (unsigned int)(port_name - tmp_addr));
		host_name[(unsigned int)(port_name - tmp_addr)] = '\0';
                port_name++;
        } else {
		strcpy(host_name, tmp_addr);
		port_name = PortName;
	}
        hserv.h_host = host_name;
        hserv.h_serv = port_name;

	if ((handlep = setnetpath()) == NULL) {
                sj3_error_number = SJ3_GetHostByName;
                return ERROR;
	}
	while ((nconf = getnetpath(handlep)) != NULL) {
                if (((nconf->nc_semantics == NC_TPI_COTS_ORD) || 
		    (nconf->nc_semantics == NC_TPI_COTS)) &&
                    (strncmp(nconf->nc_proto, proto_name, strlen(proto_name)) == 0))
                  break;
	}
	if (nconf == NULL) {
                sj3_error_number = SJ3_GetHostByName;
                return ERROR;
	}
	if (netdir_getbyname(nconf, &hserv, &addrs) != 0) {
                hserv.h_serv = PortNumber;
		if (netdir_getbyname(nconf, &hserv, &addrs) != 0) {
                        sj3_error_number = SJ3_GetHostByName;
                        return ERROR;
		}
	}
	if (( fd = t_open(nconf->nc_device, O_RDWR, (struct t_info *)0)) < 0) {
                sj3_error_number = SJ3_OpenSocket;
                return ERROR;
	}
	if (t_bind(fd, (struct t_bind *)0, (struct t_bind *) 0) < 0) {
                sj3_error_number = SJ3_OpenSocket;
                return ERROR;
	}
#else
	if (!(hp = gethostbyname(host))) {
		sj3_error_number = SJ3_GetHostByName;
		return ERROR;
	}

	if (sp = getservbyname(sj3_port_name, "tcp"))
		port = ntohs(sp -> s_port);
	else
		port = sj3_port_number;

	memset((char *)&sin, '\0',sizeof(sin));
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(port);
	memcpy(&sin.sin_addr, hp -> h_addr, hp -> h_length);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		sj3_error_number = SJ3_OpenSocket;
		return ERROR;
	}
#endif
        
        if (sj3_timeout > 0) {
                signal(SIGALRM, (void (*)())connect_timeout);
                alarm(sj3_timeout);
        }
#ifdef TLI
	if((callptr = (struct t_call *) t_alloc(fd, T_CALL, T_ALL)) == NULL) {
		sj3_error_number = SJ3_NotEnoughMemory;
		return ERROR;
	}
        callptr->addr = *(addrs->n_addrs);
        callptr->opt.len = 0;
        callptr->opt.maxlen = 0;
        callptr->opt.buf = (char *)NULL;
        callptr->udata.len = 0;
        callptr->udata.maxlen = 0;
        callptr->udata.buf = (char *)NULL;

        ret = t_connect(fd, callptr, (struct t_call *) 0);
#else
        ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
#endif
        if (sj3_timeout > 0) {
                alarm(0);
                signal(SIGALRM, SIG_IGN);
        }
        if (ret == ERROR) {
		sj3_error_number = SJ3_ConnectSocket;
		return ERROR;
	}
#ifdef TLI
	if (ioctl(fd, I_POP, (char *) 0) < 0) {
                sj3_error_number = SJ3_NotOpened;
                return ERROR;
	}
	if (ioctl(fd, I_PUSH, "tirdwr") < 0) {
                sj3_error_number = SJ3_NotOpened;
                return ERROR;
	}
        val = 0;
        (void) ioctl(fd, I_SETCLTIME, &val);
        endnetpath(handlep);
#endif

	return fd;
}



int	sj3_make_connection(client, serv, user, prog)
SJ3_CLIENT_ENV	*client;
char		*serv;		
char		*user;		
char		*prog;		
{
	char	host[MAXHOSTNAMELEN];
	int	tmp;
        int     hostlen, userlen, proglen, datalen, buflen;
        int     curlen;
        char    *curdata;

	client -> fd = -1;

	if (!serv || *serv == '\0' || !strcmp(serv, af_unix_str)) {
#ifdef TLI
                serv = LocalHost;
                strcpy(host, af_unix_str);
	} else {
                gethostname(host, sizeof(host));
	}
        if ((server_fd = open_inet(serv)) == ERROR) return ERROR;

#else
		if ((server_fd = open_unix()) == ERROR) return ERROR;
		strcpy(host, af_unix_str);
	}
	else {
		if ((server_fd = open_inet(serv)) == ERROR) return ERROR;
		gethostname(host, sizeof(host));
	}
#endif
	client -> fd = server_fd;
	client_init(client);

        hostlen = strlen(host) + 1;
        userlen = strlen(user) + 1;
        proglen = strlen(prog) + 1;
        datalen = hostlen + userlen + proglen;

	put_cmd(SJ3_CONNECT);
	put_int(SJ3SERV_VERSION_NO);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_string(host, hostlen);
	  put_string(user, userlen);
	  put_string(prog, proglen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, host, hostlen, put_string, user, 
		       userlen, put_string, prog, proglen, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

        if ((tmp = get_int()) == SJ3_DifferentVersion) {
		if (ReadErrorFlag) return ERROR;
		put_cmd(SJ3_CONNECT);
		put_int(1);
		if (datalen <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
			put_string(host, hostlen);
			put_string(user, userlen);
			put_string(prog, proglen);
			if (put_flush() == ERROR) return ERROR;
		} else {
			if (put_over(buflen, 3, put_string, host, hostlen, put_string, user, 
				     userlen, put_string, prog, proglen, NULL, NULL, 0) == ERROR)
			    return ERROR;
		}
		if (tmp = get_int()) {
			sj3_erase_connection(client);
			sj3_error_number = tmp;
			return ERROR;
		}
	} else if (tmp && (-2 < tmp)) {
		sj3_erase_connection(client);
		sj3_error_number = tmp;
		return ERROR;
	}		
        cliptr -> svr_version = ((tmp)? -tmp : 1);
        cliptr -> cli_version = SJ3SERV_VERSION_NO;
        cliptr -> default_char[0] = 0x81;
        cliptr -> default_char[1] = 0x40;
	sj3_error_number = 0;

        if (ReadErrorFlag) return ERROR;
	put_cmd(SJ3_STDYSIZE);
        if (put_flush() == ERROR) return ERROR;

	if (tmp = get_int()) {
		sj3_erase_connection(client);
		sj3_error_number = tmp;
		return ERROR;
	}

	cliptr -> stdy_size = get_int();
	cliptr -> fd        = server_fd;

        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_erase_connection(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_DISCONNECT);
        if (put_flush() == ERROR) return ERROR;

	sj3_error_number = get_int();
#ifdef TLI
        t_close(server_fd);
#else
	close(server_fd);
#endif
	cliptr -> fd = -1;
        if (ReadErrorFlag) return ERROR;
	return sj3_error_number ? ERROR : 0;
}



long	sj3_open_dictionary(client, dictname, password)
SJ3_CLIENT_ENV	*client;
char	*dictname;
char	*password;
{
        int res;
        int dictlen, passlen;
        int datalen, buflen;

	client_init(client);

        dictlen = strlen(dictname) + 1;
        passlen = ((password) ? strlen(password) : 0) + 1;
        datalen = dictlen + passlen;

	put_cmd(SJ3_OPENDICT);
        if (datalen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(dictname, dictlen);
	  put_string(password, passlen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 2, put_string, dictname, dictlen, put_string, 
		       password, passlen, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if (sj3_error_number = get_int()) return 0;
        res = get_int();
        return ReadErrorFlag ? ERROR : res;
}
int	sj3_close_dictionary(client, dicid)
SJ3_CLIENT_ENV	*client;
long	dicid;
{
	client_init(client);

	put_cmd(SJ3_CLOSEDICT);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_open_study_file(client, stdyname, password)
SJ3_CLIENT_ENV	*client;
char	*stdyname;
char	*password;
{
        int stdylen, passlen;
        int datalen, buflen;

	client_init(client);

        stdylen = strlen(stdyname) + 1;
        passlen = strlen(password) + 1;
        datalen = stdylen + passlen;

	put_cmd(SJ3_OPENSTDY);
        if (datalen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(stdyname, stdylen);
	  put_string(password, passlen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 2, put_string, stdyname, stdylen, put_string, 
		       password, passlen, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_close_study_file(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_CLOSESTDY);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_get_id_size(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_STDYSIZE);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
	cliptr -> stdy_size = get_int();
        return ReadErrorFlag ? ERROR : cliptr -> stdy_size;
}



int	sj3_lock_server(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_LOCK);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_unlock_server(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_UNLOCK);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_ikkatu_henkan(client, src, dst, dstsiz, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*src;
register u_char	*dst;
register int	dstsiz;
int mb_flag;
{
	register int	c;
	u_char	*top;
	int	result;
	int	len;
	int	len1;
	int	stysiz;

        int     srclen;
        int     buflen;

	client_init(client);

        srclen = strlen(src) + 1;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PH2KNJ);
	else
	  put_cmd(SJ3_PH2KNJ_EUC);
        if (srclen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(src, srclen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 1, put_string, src, srclen, NULL, NULL, 0, 
			  NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;

	result = get_int();

	
	result = 0;
	stysiz = cliptr -> stdy_size;
	len1 =  1 + stysiz + 1 + 1;	
	while (c = get_byte()) {

		top = dst;		
		if (dstsiz < len1) goto error1;

		*dst++ = len = c;
		dst = get_ndata(dst, stysiz);
		dstsiz -= (stysiz + 1);

		while (c = get_byte()) {
			if (dstsiz-- < 3) goto error2;
			*dst++ = c;
		}
		*dst++ = 0;
		dstsiz--;
		result += len;
	}

	*dst = 0;
        return ReadErrorFlag ? ERROR : result;

	
error1:
	do {
		skip_ndata(stysiz);
error2:
		while (get_byte()) ;
	} while (get_byte());

	*top = 0;
        return ReadErrorFlag ? ERROR : result;
}



int	sj3_bunsetu_henkan(client, yomi, len, kanji, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*yomi;
int	len;
u_char	*kanji;
int mb_flag;
{
	int	result;
        int     buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ);
	else
	  put_cmd(SJ3_CL2KNJ_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  
	  if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
		       NULL, NULL, 0, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}
int	sj3_bunsetu_jikouho(client, kanji, mode, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*kanji;
int	mode;
int     mb_flag;
{
	int	result;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_NEXTCL);
	else
	  put_cmd(SJ3_NEXTCL_EUC);
	put_int(mode);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}
int	sj3_bunsetu_maekouho(client, kanji, mode, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*kanji;
int	mode;
int     mb_flag;
{
	int	result;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PREVCL);
	else
	  put_cmd(SJ3_PREVCL_EUC);
	put_int(mode);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}



int	sj3_bunsetu_kouhosuu(client, yomi, len, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*yomi;
int	len;
int     mb_flag;
{
        int result;
        int buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ_CNT);
	else
	  put_cmd(SJ3_CL2KNJ_CNT_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	
          if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
                       NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;

	result = get_int();
        return ReadErrorFlag ? ERROR : result;
}
int	sj3_bunsetu_zenkouho(client, yomi, len, douon, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*yomi;
int	len;
SJ3_DOUON	*douon;
int     mb_flag;
{
	int	cnt = 0;
        int buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ_ALL);
	else
	  put_cmd(SJ3_CL2KNJ_ALL_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	
          if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
                       NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;

	while (get_int()) {
                get_ndata(&(douon -> dcid), cliptr -> stdy_size);
		get_string(douon -> ddata);
		douon -> dlen = strlen(douon -> ddata);
		douon++;
		cnt++;
	}

        return ReadErrorFlag ? ERROR : cnt;
}



int	sj3_tango_gakusyuu(client, stdy)
SJ3_CLIENT_ENV	*client;
SJ3_STUDYREC	*stdy;
{
        int buflen;

	client_init(client);
    
	put_cmd(SJ3_STUDY);
        if (cliptr -> stdy_size <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_ndata(stdy, cliptr -> stdy_size);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  
	  if (put_over(buflen, 1, put_ndata, stdy, cliptr -> stdy_size, NULL, 
		       NULL, 0, NULL, NULL, 0, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}


	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_bunsetu_gakusyuu(client, yomi1, yomi2, stdy, mb_flag)
SJ3_CLIENT_ENV	*client;
u_char	*yomi1;
u_char	*yomi2;
SJ3_STUDYREC	*stdy;
int     mb_flag;
{
        int yomilen1, yomilen2;
        int datalen, buflen;

	client_init(client);

        yomilen1 = strlen(yomi1) + 1;
        yomilen2 = strlen(yomi2) + 1;
        datalen = yomilen1 + yomilen2 + cliptr -> stdy_size;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CLSTUDY);
	else
  	  put_cmd(SJ3_CLSTUDY_EUC);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(yomi1, yomilen1);
	  put_string(yomi2, yomilen2);
	  put_ndata(stdy, cliptr -> stdy_size);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi1, yomilen1, put_string, 
		       yomi2, yomilen2, put_ndata, stdy, cliptr->stdy_size,
		       NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_tango_touroku(client, dicid, yomi, kanji, code, mb_flag)
SJ3_CLIENT_ENV	*client;
long	dicid;
u_char	*yomi;
u_char	*kanji;
int	code;
int     mb_flag;
{
        int yomilen, kanjilen;
        int datalen, buflen;

	client_init(client);

        yomilen = strlen(yomi) + 1;
        kanjilen = strlen(kanji) + 1;
        datalen = yomilen + kanjilen + INTLEN;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_ADDDICT);
	else
	  put_cmd(SJ3_ADDDICT_EUC);	  
	put_int(dicid);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN -INTLEN)) {
	  put_string(yomi, yomilen);
	  put_string(kanji, kanjilen);
	  put_int(code);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi, yomilen, put_string, kanji,
		       kanjilen, put_ndata, &code, INTLEN, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_tango_sakujo(client, dicid, yomi, kanji, code, mb_flag)
SJ3_CLIENT_ENV	*client;
long	dicid;
u_char	*yomi;
u_char	*kanji;
int	code;
int     mb_flag;
{
        int yomilen, kanjilen;
        int datalen, buflen;

	client_init(client);

        yomilen = strlen(yomi) + 1;
        kanjilen = strlen(kanji) + 1;
        datalen = yomilen + kanjilen + INTLEN;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_DELDICT);
	else
	  put_cmd(SJ3_DELDICT_EUC);
	put_int(dicid);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN -INTLEN)) {
	  put_string(yomi, yomilen);
	  put_string(kanji, kanjilen);
	  put_int(code);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi, yomilen, put_string, kanji,
		       kanjilen, put_ndata, &code, INTLEN, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_tango_syutoku(client, dicid, buf, mb_flag)
SJ3_CLIENT_ENV	*client;
int	dicid;
u_char	*buf;
int     mb_flag;
{
	register u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_GETDICT);
	else
	  put_cmd(SJ3_GETDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_tango_jikouho(client, dicid, buf, mb_flag)
SJ3_CLIENT_ENV	*client;
int	dicid;
u_char	*buf;
int     mb_flag;
{
	register u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_NEXTDICT);
	else
	  put_cmd(SJ3_NEXTDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_tango_maekouho(client, dicid, buf, mb_flag)
SJ3_CLIENT_ENV	*client;
int	dicid;
u_char	*buf;
int     mb_flag;
{
	register u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PREVDICT);
	else
	  put_cmd(SJ3_PREVDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_make_dict_file(client, path, idxlen, seglen, segnum)
SJ3_CLIENT_ENV	*client;
char	*path;
int	idxlen;
int	seglen;
int	segnum;
{
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN + INTLEN + INTLEN;

	put_cmd(SJ3_MAKEDICT);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(idxlen);
	  put_int(seglen);
	  put_int(segnum);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 4, put_string, path, pathlen, put_ndata,
                       &idxlen, INTLEN, put_ndata, &seglen, INTLEN,
                       put_ndata, &segnum, 0) == ERROR)
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_make_study_file(client, path, stynum, clstep, cllen)
SJ3_CLIENT_ENV	*client;
char	*path;
int	stynum;
int	clstep;
int	cllen;
{
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN + INTLEN + INTLEN;

	put_cmd(SJ3_MAKESTDY);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(stynum);
	  put_int(clstep);
	  put_int(cllen);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 4, put_string, path, pathlen, put_ndata,
                       &stynum, INTLEN, put_ndata, &clstep, INTLEN,
                       put_ndata, &cllen, 0) == ERROR)
            return ERROR;
	}

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_make_directory(client, path)
SJ3_CLIENT_ENV	*client;
char	*path;
{
        int pathlen;
        int buflen;

	client_init(client);

        pathlen = strlen(path) + 1;

	put_cmd(SJ3_MAKEDIR);
        if (pathlen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 1, put_string, path, pathlen, NULL, NULL, 0,
		       NULL, NULL, 0, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}
	  

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_access(client, path, mode)
SJ3_CLIENT_ENV	*client;
char	*path;
int	mode;
{
        int result;
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN;

	put_cmd(SJ3_ACCESS);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(mode);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 2, put_string, path, pathlen, put_ndata,
                       &mode, INTLEN, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	sj3_error_number = 0;
	result = get_int();
        return ReadErrorFlag ? ERROR : result;
}



int	sj3_who(client, ret, num)
SJ3_CLIENT_ENV	*client;
SJ3_WHO_STRUCT	*ret;
int	num;
{
	int	i, j;

	client_init(client);

	put_cmd(SJ3_WHO);
        if (put_flush() == ERROR) return ERROR;

	if ((i = get_int()) < 0) {
		sj3_error_number = SJ3_InternalError;
		return -1;
	}

	sj3_error_number = 0;
	for (j = 0 ; j < i ; j++) {
		if (j < num) {
			ret -> fd = get_int();
			get_nstring(ret -> hostname, SJ3_NAME_LENGTH);
			get_nstring(ret -> username, SJ3_NAME_LENGTH);
			get_nstring(ret -> progname, SJ3_NAME_LENGTH);
			ret++;
		}
		else {
			get_int();
			skip_string();
			skip_string();
			skip_string();
		}
	}

        return ReadErrorFlag ? ERROR : ((i < num) ? i : num);
}



int	sj3_quit(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_QUIT);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}
int	sj3_kill(client)
SJ3_CLIENT_ENV	*client;
{
	client_init(client);

	put_cmd(SJ3_KILL);
        if (put_flush() == ERROR) return ERROR;

	if (sj3_error_number = get_int()) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}



int	sj3_version(client, dst, dstsiz)
SJ3_CLIENT_ENV	*client;
char	*dst;
int	dstsiz;
{
	int	c;

	client_init(client);

	put_cmd(SJ3_VERSION);
        if (put_flush() == ERROR) return ERROR;

	sj3_error_number = get_int();
	c = get_byte();
	while (c) {
		while (c) {
			if (dstsiz > 2) { *dst++ = c; dstsiz--; }
			c = get_byte();
		}
		if (dstsiz > 1) { *dst++ = 0; dstsiz--; }
		c = get_byte();
	}
	if (dstsiz > 0) { *dst++ = 0; dstsiz--; }
        return ReadErrorFlag ? ERROR : 0;
}
