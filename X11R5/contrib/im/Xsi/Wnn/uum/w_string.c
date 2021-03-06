/*
 * $Id: w_string.c,v 1.2 1991/09/16 21:34:06 ohm Exp $
 */
/*
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that all of the following conditions are satisfied:
 *
 * 1) The above copyright notices appear in all copies
 * 2) Both those copyright notices and this permission notice appear
 *    in supporting documentation
 * 3) The name of "Wnn" isn't changed unless substantial modifications
 *    are made, or
 * 3') Following words followed by the above copyright notices appear
 *    in all supporting documentation of software based on "Wnn":
 *
 *   "This software is based on the original version of Wnn developed by
 *    Kyoto University Research Institute for Mathematical Sciences (KURIMS),
 *    OMRON Corporation and ASTEC Inc."
 *
 * 4) The names KURIMS, OMRON and ASTEC not be used in advertising or
 *    publicity pertaining to distribution of the software without
 *    specific, written prior permission
 *
 * KURIMS, OMRON and ASTEC make no representations about the suitability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * Wnn consortium is one of distributors of the official Wnn source code
 * release.  Wnn consortium also makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * KURIMS, OMRON, ASTEC AND WNN CONSORTIUM DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL KURIMS, OMRON, ASTEC OR
 * WNN CONSORTIUM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */
/*	Version 4.0
 */
#include <stdio.h>
#include "commonhd.h"
#include "sdefine.h"
#include "sheader.h"
#define CONTLINE '$'


void
w_printf(x , len)
w_char *x;
int len;
{
	for(;*x != 0 && len > 0  ; x++ , len--){
	    w_putchar(*x);
	}
	flushw_buf();
	if(len <= 0) putchar(CONTLINE);
}

void
chartowchar(c , w)
w_char *w;
UCHAR *c;
{
    Sstrcpy(w, c);
}

int
wchartochar(w , c)
w_char *w;
UCHAR *c;
{
    int flg = 0;
    for(;*w != 0 ; w++){
	if((*w) > 256){
	    *c++ = (UCHAR)(*w & 0xff00) >> 8;
	    *c++ = (UCHAR)*w & 0x00ff;
	    flg = 1;
	}
	*c++ = *w & 0x00ff;
    }
    *c++ = 0;
    *c++ = 0;
    return(flg);
}


void
w_sttost(w, c)
w_char *w;
char *c;
{
    sStrcpy(c , w);
}

void
w_sttostn(w, c, n)
w_char *w;
char *c;
int n;
{
    sStrncpy(c, w, n);
}

int
eu_columlen(c)
unsigned char *c;
{
  int len = 0;
  w_char tmp;
  
  for(;*c;){
    if((*c & 0x80)){
      tmp = ((w_char)*c++) << 8;
      tmp |= *c++;
    }else{
      tmp = *c++;
    }
    if(hankaku(tmp)){
      len += 1;
    }else{
      len += 2;
    }
  }
  return(len);
}
