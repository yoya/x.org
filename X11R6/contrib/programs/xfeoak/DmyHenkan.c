/* @(#)DmyHenkan.c      6.1 94/06/01 20:04:27 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1994  FUJITSU LIMITED

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

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Kiyoaki oya          FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include "KanjiTbl.h"
#include "Henkan.h"
typedef unsigned short  ushort;
typedef unsigned char   uchar;

#define Static static

#define nop	0

Static int	Dmy_Henkan();
Static int	Dmy_Zenkouho();
Static int	Dmy_HenkanCancel();

DictionaryFuncRec dmy_dictionary_func = {
	(char *)0,
	nop,
	nop,
	nop,
	nop,
	Dmy_Henkan,
	Dmy_Zenkouho,
	Dmy_HenkanCancel
	};

Static int	_mu_henkan_count = 0;
Static ushort	*_dic_old_buff = 0;

Static int
Dmy_Henkan( cnvrt, count, yomi, yomi_len, out_buff, out_size)
uchar	cnvrt;
int	count;
ushort	*yomi;
int	yomi_len;
ushort	*out_buff;
int	out_size;
{
    int		num;
    if( count == 0 ) {
	if( cnvrt == FKK_MAE ) {
	    return( 0 );
	}
        if( _dic_old_buff != 0 ) {
            Dic_HenkanCancel();
        }
	_mu_henkan_count = 0;
    }
    _dic_old_buff = yomi;
    num = _Dmy_Muhenkan(yomi,yomi_len,out_buff,out_size);
    return( num );
}

Static int
Dmy_Zenkouho( type, yomi, yomi_len )
int type;
ushort *yomi;
int	yomi_len;
{
    int		i, ret, num, out_size = 60, no, r_len;
    ushort	out_buff[60];

    InitialKanjiItem();
    if( _dic_old_buff != yomi ) {
        Dic_HenkanCancel();
        _dic_old_buff = yomi;
    }
    
    for( i=3; i>0; i--) {
        num = _Dmy_Muhenkan( yomi, yomi_len, out_buff, out_size);
	ret = AddKanjiItem( num, out_buff);
	if( ret == -1 )   return( -1 );
    }
    return( select_kanji_item_no );
}

Static int
Dmy_HenkanCancel()
{
    if( _dic_old_buff != (ushort *)0 ) {
        _dic_old_buff = (ushort *)0;
        _mu_henkan_count = 0;
    }
}

_Dmy_Muhenkan( yomi, yomi_length, out_buff, out_size)
ushort	*yomi, 		*out_buff;
int	yomi_length,	out_size;
{
    int		num = 0;
    ushort	save;
    save = yomi[ yomi_length];
    yomi[yomi_length] = 0;
    if( _mu_henkan_count == 0 ) {
	_mu_henkan_count = 1;
        num = CopyUshort( out_buff, yomi, out_size );
    } else
    if( _mu_henkan_count == 1 ) {
	_mu_henkan_count = 2;
        num = Hiragana_To_Katakana( out_buff, yomi, out_size );
    } else {
	_mu_henkan_count = 0;
        num = ZenkakuAsciiKana_To_Hankaku( out_buff, yomi, out_size, 1 );
    }
    yomi[ yomi_length] = save;
    return( num );
}
