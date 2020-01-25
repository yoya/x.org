/*
 *	$Id: kterm.h,v 6.0 1994/06/04 15:44:00 kagotani Rel $
 */

/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993 and 1994
 * XXI working group in Japan Unix Society (XXI).
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 * 
 * Author:
 * 	Hiroto Kagotani
 * 	Dept. of Computer Science
 *	Tokyo Institute of Technology
 * 	2-12-2 Ookayama, Meguro-ku, Tokyo 152, Japan
 * 	kagotani@cs.titech.ac.jp
 */ 

#ifndef _KTERM_H_
#define _KTERM_H_
#ifdef KTERM

#define KTERM_VERSION "6.1.0"
#define KTERM_MBCS /* multi-byte character set sequence support (only Kanji in this release) */
#define KTERM_KANJI /* Kanji specific functions support */
#define KTERM_KCONV /* talk with {ROMAJI,KANA}->KANJI converter */
#define KTERM_COLOR /* accept color sequences */
#define KTERM_MBCC /* multi-byte character class support for word selection */

/* gset of Ichr,Bchr */
#define CS96		0x80 /* character sets which have 96 characters */
#define MBCS		0x40 /* multi-byte character sets */
#define MBC2		0x7f /* second byte of a mbcs character */
  /*
   * there is no character set using designating character less than ' '.
   * but who can say there is no set greater than '_' (0x3f + ' ').
   */
#define GSET(c)		((c) - ' ')
#define GSETFC(i)	(((i) & ~(MBCS|CS96)) + ' ')
		/* final character of a designation sequense for a gset */
/* code of Ichr,Bchr */
#define NEEDMAP		0x80
typedef struct {
	unsigned char gset;
	unsigned char code;
} Ichr; /* char for interchanging with other processes */

#define GSET_GRAPH	GSET('0')
#define GSET_UK		GSET('A')
#define GSET_ASCII	GSET('B')
#define GSET_JISROMAN	GSET('J')
#define GSET_KANA	GSET('I')
#define GSET_LATIN1R	(CS96|GSET('A'))

#ifdef KTERM_MBCS
# define GSET_KANJI	(MBCS|GSET('B'))
# define GSET_OLDKANJI	(MBCS|GSET('@'))
#endif /* KTERM_MBCS */

#endif /* KTERM */
#endif /* !_KTERM_H_ */
