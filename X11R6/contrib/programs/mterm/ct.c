/*
 *	$Header: /var/home/sm/ws/michael/work/xterm/RCS/ct.c,v 1.1 1994/06/07 00:05:30 michael Exp $ Sony Corporation
 */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/

#include "ptyx.h"

static char * def_gs[] = {
	"B,ABB",	/* T_LATIN1 */
	"B,ABB",	/* T_LATIN2 */
	"B,ACB",	/* T_LATIN3 */
	"B,ADB",	/* T_LATIN4 */
	"B,LBB",	/* T_CYRILLIC */
	"B,GBB",	/* T_ARABIC */
	"B,FBB",	/* T_GREEK */
	"B,HBB",	/* T_HEBREW */
	"B,MBB",	/* T_LATIN5 */
	"JIBB",		/* T_JIS */
	"JIBB",		/* T_SJIS */
	"B$BIB",	/* T_JEUC */
	"B$CBB",	/* T_KEUC */
	"B$ABB",	/* T_CEUC */
	NULL,
};

char *
getgs(icode)
	int icode;
{
	if (icode >= T_LATIN1 && icode <= T_CEUC)
		return (def_gs[icode]);
	else
		return (NULL);
}

int
convct(ibuf, obuf, len, icode)
	register char *ibuf;
	register char *obuf;
	int len;
	int icode;
{
	register char *gs = def_gs[icode];
	int mbcs = 0;
	int mbcsl = 0;
	int mbcsr = 0;
	int kana = 0;
	int state = 0;
	int skipl = 0;
	int skipr;
	unsigned char c;
	int i, l;

	if (icode != T_LATIN1) {
		skipr = 1;
	} else {
		skipr = 0;
	}
	for (i = 0, l = 0; i < len; i++) {
		c = ibuf[i];
		switch (state) {
		case 1:
			if (c == '$') {
				mbcs = 1;
			} else if (c == '(') {
				if (mbcs) {
					mbcsl = 1;
					mbcs = 0;
				} else {
					mbcsl = 0;
				}
				state = 2;
			} else if (c == ')') {
				if (mbcs) {
					mbcsr = 1;
					mbcs = 0;
				} else {
					mbcsr = 0;
				}
				state = 3;
			} else if (c == '-') {
				state = 3;
				mbcsr = 0;
			}
			break;
		case 2:
			if (mbcsl) {
				skipl = 1;
				switch (icode) {
				case T_JIS:
				case T_SJIS:
				case T_JEUC:
					if (c == 'B') {
						skipl = 0;
					}
					break;
				case T_KEUC:
					if (c == 'C') {
						skipl = 0;
					}
					break;
				case T_CEUC:
					if (c == 'A') {
						skipl = 0;
					}
					break;
				default:
					break;
				}
			} else {
				skipl = 0;
			}
			state = 0;
			break;
		case 3:
			skipr = 1;
			if (mbcsr) {
				switch (icode) {
				case T_JIS:
				case T_SJIS:
				case T_JEUC:
					if (c == 'B') {
						skipr = 0;
					}
					break;
				case T_KEUC:
					if (c == 'C') {
						skipr = 0;
					}
					break;
				case T_CEUC:
					if (c == 'A') {
						skipr = 0;
					}
					break;
				default:
					break;
				}
			} else {
				switch (c) {
				case 'A':
					if (icode == T_LATIN1) {
						skipr = 0;
					}
					break;
				case 'B':
					if (icode == T_LATIN2) {
						skipr = 0;
					}
					break;
				case 'C':
					if (icode == T_LATIN3) {
						skipr = 0;
					}
					break;
				case 'D':
					if (icode == T_LATIN4) {
						skipr = 0;
					}
					break;
				case 'L':
					if (icode == T_CYRILLIC) {
						skipr = 0;
					}
					break;
				case 'G':
					if (icode == T_ARABIC) {
						skipr = 0;
					}
					break;
				case 'F':
					if (icode == T_GREEK) {
						skipr = 0;
					}
					break;
				case 'H':
					if (icode == T_HEBREW) {
						skipr = 0;
					}
					break;
				case 'M':
					if (icode == T_LATIN5) {
						skipr = 0;
					}
					break;
				case 'I':
					switch (icode) {
					case T_JEUC:
						kana = 1;
					case T_SJIS:
					case T_JIS:
						skipr = 0;
						break;
					default:
						break;
					}
				}
			}
			state = 0;
			break;
		default:
			if (c == '\033') {
				state = 1;
				mbcs = 0;
			} else if (c <= 0x7f) {
				if (skipl) {
					if (mbcsl) {
						i++;
					}
				} else {
					if (mbcsl) {
						if (icode == T_SJIS) {
							obuf[l++] = ((c - 0x21) >> 1) + ((c <= 0x5e) ? 0x81: 0xc1);
							i++;
							if (c & 1) {
								obuf[l++] = ibuf[i]  + ((ibuf[i] <= 0x5f) ? 0x1f: 0x20);
							} else {
								obuf[l++] = ibuf[i] + 0x7e;
							}
						} else {
							obuf[l++] = c | 0x80;
							obuf[l++] = ibuf[++i] | 0x80;
						}
					} else {
						obuf[l++] = c;
					}
				}
			} else {
				if (skipr) {
					if (mbcsr) {
						i++;
					}
				} else {
					if (mbcsr) {
						if (icode == T_SJIS) {
							obuf[l++] = ((c - 0xa1) >> 1) + ((c <= 0xde) ? 0x81: 0xc1);
							i++;
							if (c & 1) {
								obuf[l++] = ibuf[i] - ((ibuf[i] <= 0xdf) ? 0x61: 0x60);
							} else {
								obuf[l++] = ibuf[i] - 0x2;
							}
						} else {
							obuf[l++] = c;
							obuf[l++] = ibuf[++i];
						}
					} else {
						if (kana) {
							obuf[l++] = 0x8e;
						}
						obuf[l++] = c;
					}
				}
			}
			break;
		}
	}
	return (l);
}
