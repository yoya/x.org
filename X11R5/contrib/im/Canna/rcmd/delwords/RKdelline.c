/* Copyright 1991 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of NEC Corporation
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  NEC 
 * Corporation makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 */

static	char	rcs_id[] = "@(#) 102.1 $Header: /work/nk.R3.1/rcmd/delwords/RCS/RKdelline.c,v 1.1 91/06/05 10:23:51 kon Exp $";

#include "RK.h"
#include "RKintern.h"

RkDeleteLine(cx_num, name, line)
int cx_num;
char *name;
char *line;
{
  int linelen = strlen(line);
  int yomilen, yomihinshilen = 0;
  char *buf = (char *)malloc(linelen + 1), *sp, *dp;
  int res = 0;

  
  if ( !buf ) {
    return -1;
  }
  sp = line;
  while (*sp == ' ')
    sp++; /* 空白の読み飛ばし */

  if (!*sp || *sp == '#') /* コメント行 */
    goto endDeleteLine;

  dp = buf;
  while (*sp && *sp != ' ') { /* 読みのとりだし */
    *dp++ = *sp++;
  }
  *dp++ = ' ';
  yomilen = dp - buf;

  while (*sp) {
    while (*sp == ' ')
      sp++; /* 空白の読み飛ばし */

    if (*sp) {
      if (*sp == '#') {
	dp = buf + yomilen;
	while (*sp && *sp != ' ') { /* 品詞と頻度のコピー */
	  *dp++ = *sp++;
	}
	*dp++ = ' ';
	yomihinshilen = dp - buf;

	while (*sp == ' ')
	  sp++; /* 空白の読み飛ばし */
      }
      if (yomihinshilen == 0) { /* ここまで品詞情報が出てこなかった。 */
	goto endDeleteLine;
      }
      else if (*sp) { /* 候補があるのなら */
	dp = buf + yomihinshilen;
	while (*sp && *sp != ' ') { /* 候補のコピー */
	  *dp++ = *sp++;
	}
	*dp++ = '\0'; /* ヌル文字を最後に入れる */
	res = RkDeleteDic(cx_num, name, buf);
	if (res < 0) {
	  goto endDeleteLine;
	}
      }
    }
  }
 endDeleteLine:
  free(buf);
  return res;
}

#ifdef TEST_DELETEDIC
RkDeleteDic(cx_num, name, word)
int cx_num;
char *name;
char *word;
{
  printf("☆単語の定義(辞書:%s) \"%s\"\n", name, word);
  return 0;
}

main()
{
  char buf[2048], *p;
  int c;

  p = buf;
  c = getchar();
  while (c >= 0) {
    if (c == '\n') {
      *p++ = '\0';
      RkDeleteLine(0, "tempdic", buf);
      p = buf;
    }
    else {
      *p++ = c;
    }
    c = getchar();
  }
}
#endif /* TEST_DELETEDIC */
