/* @(#)Sj3Convert.c      6.1 94/06/01 20:04:12 FUJITSU LIMITED. */
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

  Author: Takanori Tateno      FUJITSU LIMITED

*/

typedef unsigned short    ushort;
typedef unsigned char     uchar;
#include <stdio.h>

ushort jis_to_sjis();
ushort sjis_to_jis();

ShiftJisToEuc(sjis_str,sjis_size,euc_str,euc_num)
uchar	*sjis_str;
int	sjis_size;
ushort	*euc_str;
int	euc_num;
{
	int i,j;
	ushort	tmp;
	i=j=0;

	while(euc_num>i && sjis_size >j){
		if( (sjis_str[j]>=0x81 && sjis_str[j]<=0x9f) ||
			(sjis_str[j]>=0xe0 && sjis_str[j]<=0xfc)){
				tmp=sjis_to_jis(
					((((ushort)(sjis_str[j]))<<8) & 0xff00)|
					((ushort)(sjis_str[j+1]) & 0x00ff)
			  			);
				tmp=tmp | 0x8080;
				euc_str[i]=tmp;
				j+=2;
				i++;
		}else{
				euc_str[i]=((ushort)(sjis_str[j])) & 0x00ff ;
				i++;
				j++;
		}
	}
	return(i);
}

int
EucToShiftJis(euc_str,euc_num,sjis_str,sjis_size)
ushort	*euc_str;
int	euc_num;
uchar	*sjis_str;
int	sjis_size;
{
	int i,j=0;
	ushort	tmp;
	for(i=0;i<euc_num ;i++){
		if(euc_str[i]<256){
			if(j<sjis_size){
				sjis_str[j]=(uchar)euc_str[i];
				j++;
			}else{
				return(j);
			}
		}else{
			if(j+1 < sjis_size){
				tmp = euc_str[i] & 0x7f7f;
				tmp=jis_to_sjis(tmp);
				sjis_str[j]=(uchar)(tmp>>8);
				sjis_str[j+1]=(uchar)(tmp & 0xff);
				j+=2;
			}else{
				return(j);
			}
		}
	}
	return(j);
}

ushort
jis_to_sjis(code)
ushort	code;
{
	unsigned short	a,b;
	a=(code>>8) & 0xff;
	b=code & 0xff;
	if(a<0x21 || a>0x7e && a<0x21 || b> 0x7e ){
		return(-1);
	}
	a++;
	b+=(a & 0x1 ? 0x7e : 0x20);
	if(b < 0x80)--b;
	a=a >> 1;
	a+=0x70;
	if(a>=0x9f)a+=0x40;
	
	return(((a<<8) & 0xff00)|b);
}

ushort
sjis_to_jis(code)
ushort	code;
{
	unsigned short	a,b;
	a=(code>>8) & 0xff;
	b=code & 0xff;
	if(a<0x81 || (a>0x9f && a<0xe0) || a> 0xfc ||
		b < 0x40 || b== 0x7f || b>0xfc){
		return(-1);
	}
	a-=(a>=0xe0 ? 0xb0 : 0x70);
	a =a << 1;
	if(b<0x80)b++;
	if(b>=0x9f){
		a|=0x01;
		b-=0x7e;
	}else{
		b-= 0x20;
	}
	--a;
	return(((a<<8) & 0xff00)|b);
}
