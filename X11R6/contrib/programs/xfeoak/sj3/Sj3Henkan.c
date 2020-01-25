/* @(#)Sj3Henkan.c      6.1 94/06/01 20:04:12 FUJITSU LIMITED. */
/*

Copyright (c) 1991  FUJITSU LIMITED

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Masahiko Narita      FUJITSU LIMITED
  	  Takanori Tateno      FUJITSU LIMITED
  Modify: Takashi Fujiwara     FUJITSU LIMITED 
          Kiyoaki oya          FUJITSU LIMITED

*/

#ifdef SJ3_DICTIONARY

#include <stdio.h>
#include <sj3lib.h>
#include "Henkan.h"
typedef unsigned short  ushort;
typedef unsigned char   uchar;

#ifdef DEBUG
#define Static /**/
#else
#define Static static
#endif

#ifndef NULL
#define NULL	(char *)0
#endif

static ushort *_dic_old_buff;

Static int	Sj3_Argv();
Static int	Sj3_Help();
Static int	Sj3_Open2();
Static int	Sj3_Open();
Static int	Sj3_Close();
Static int	Sj3_Henkan();
Static int	Sj3_Zenkouho();
Static int	Sj3_HenkanCancel();

DictionaryFuncRec sj3_dictionary_func = {
        "-sj3",
	Sj3_Argv,
	Sj3_Help,
	Sj3_Open2,
	Sj3_Close,
	Sj3_Henkan,
	Sj3_Zenkouho,
	Sj3_HenkanCancel
	};

Static char *uname = NULL;

#define BUFSIZE 	1024
#define True		1
#define False		0

static int	bun_max =0;
static int	open_dic =0;
static int	open_udic =0;

Static int
Sj3_Argv( argc, argv)
    int 	argc;
    char	*argv[];
{
    if(!strcmp(argv[0], "-uname")) {
	uname = argv[1];
	return( 2 );
	}
    return( 0 );
}

Static int
Sj3_Help( fp )
FILE    *fp;
{
    fprintf(fp,"[-uname <user name>]");
}

#define UNAME "xfeoak"

Static int
Sj3_Open2()
{
	char *home;
	if(uname == NULL) {
            uname = (char *)malloc( strlen(UNAME)+1);
            strcpy(uname,UNAME);
        }
	return Sj3_Open( uname );
}

Static int
Sj3_Open(uname)
uchar *uname;
{
	int	ret;
	ret = sj3_open( 0, (char *)uname );
	if( ret ) {
		return(-1);
	}
	open_dic = 1;
	return(0);
}

Static int
Sj3_Close(){
	if( open_dic)
		sj3_close();
	open_dic = 0;
	return(0);
}

Static int
Sj3_Henkan(cnvrt,count,in_euc_str,in_euc_num,out_euc_str,out_euc_num)
uchar	cnvrt;
int	count;
ushort	*in_euc_str;
int	in_euc_num;
ushort	*out_euc_str;
int	out_euc_num;
{
	int	ret,i,len;
	uchar	in_sjis_buff[BUFSIZE],*p;
	uchar	out_sjis_buff[BUFSIZE];	
	static uchar	kanji[BUFSIZE];	
	static struct bunsetu bun[BUFSIZE];
	static struct douon dou[BUFSIZE];  
	static int bnum,koho_num; 


	for(i=0;i<BUFSIZE;i++)out_sjis_buff[i]=0x0;
	ret=EucToShiftJis(in_euc_str,in_euc_num,in_sjis_buff,BUFSIZE);

	if(count == 0){
		if( _dic_old_buff != 0) {
			Sj3_HenkanCancel();
		}
	in_sjis_buff[ret]=0x0;
        bnum=sj3_getkan(in_sjis_buff,bun,kanji,BUFSIZE);
	
        p  = bun[bnum-1].deststr;
        p += bun[bnum-1].destlen;
        *p = '\0';
 
        p  = bun[0].srcstr;
        p += bun[0].srclen;
        *p = '\0';

	 koho_num = sj3_getdouon(bun[0].srcstr,dou);
		ret=ShiftJisToEuc(kanji,strlen(kanji),
						out_euc_str,out_euc_num);
		_dic_old_buff = in_euc_str;
				return ret;
	} else{

		if(_dic_old_buff != in_euc_str) {
			Sj3_HenkanCancel();
			_dic_old_buff = in_euc_str;
			bun_max = 0;
		}
			strncpy(out_sjis_buff,dou[count%koho_num].ddata,dou[count%koho_num].dlen);
			if(bun[1].deststr){
			strcpy(out_sjis_buff+dou[count%koho_num].dlen,bun[1].deststr);	
			len=strlen(bun[1].deststr);
			}else{len=0;}

		ret=ShiftJisToEuc(out_sjis_buff,
					dou[count%koho_num].dlen+len,
					out_euc_str,out_euc_num);
	return ret;
	}
}

Static int
Sj3_Zenkouho(type,euc_str,ecu_num)
int 	type;
ushort	*euc_str;
int	ecu_num;
{
	return -1;
}

Static int
Sj3_HenkanCancel()
{
	if(_dic_old_buff != (ushort *)0){
		_dic_old_buff=(ushort *)0;
	}
}

#endif /*SJ3_DICTIONARY*/
