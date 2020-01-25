/* @(#)Message.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1993, 1994  FUJITSU LIMITED

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

#include "Xfeoak.h"

typedef struct _message {
    int		 no;
    char	*data;
} Message;

#include "Message.h"

Static char *
GetMessage(no)
int		 no;
{
    static char	 buff[64];
    register int i;

    for(i = 0; i < max_message_no; i++) {
	if (message[i].no == no)
	    return  message[i].data;
    }
    (void)sprintf(buff, "Invalid message no. (%d)", no);
    return buff;
}

void
ErrorMessage(no, a1, a2, a3, a4, a5)
    int			 no;
    unsigned long	 a1, a2, a3, a4, a5;
{
    char		*form;

    form = GetMessage(no);
    (void)fprintf(stderr,"%s error : ", command_name);
    (void)fprintf(stderr, form, a1, a2, a3, a4, a5);
    (void)fprintf(stderr,"\n");
    (void)fflush(stderr);
    return;
}

void
WarningMessage(no, a1, a2, a3, a4, a5)
    int			 no;
    unsigned long	 a1, a2, a3, a4, a5;
{
    char		*form;

    form = GetMessage(no);
    (void)fprintf(stderr,"%s warning : ", command_name);
    (void)fprintf(stderr, form, a1, a2, a3, a4, a5);
    (void)fprintf(stderr,"\n");
    (void)fflush(stderr);
    return;
}
