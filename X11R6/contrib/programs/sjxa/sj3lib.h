/* $SonyId: sj3lib.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifndef _Sj3lib_H
#define _Sj3lib_H
#include	<X11/Xfuncproto.h>

/*
 *	sj3_open/sj3_close error code
 */
#define	SJ3_NORMAL_END			0
#define	SJ3_SERVER_DEAD			(1 << 0)

/*
 *	sj3_open error code
 */
#define	SJ3_CONNECT_ERROR		(1 << 1)
#define	SJ3_ALREADY_CONNECTED		(1 << 2)
#define	SJ3_CANNOT_OPEN_MDICT		(1 << 3)
#define	SJ3_CANNOT_OPEN_UDICT		(1 << 4)
#define	SJ3_CANNOT_OPEN_STUDY		(1 << 5)
#define	SJ3_CANNOT_MAKE_UDIR		(1 << 6)
#define	SJ3_CANNOT_MAKE_UDICT		(1 << 7)
#define	SJ3_CANNOT_MAKE_STUDY		(1 << 8)

/*
 *	sj3_close error code
 */
#define	SJ3_DISCONNECT_ERROR		(1 << 1)
#define	SJ3_NOT_CONNECTED		(1 << 2)
#define	SJ3_NOT_OPENED_MDICT		(1 << 3)
#define	SJ3_NOT_OPENED_UDICT		(1 << 4)
#define	SJ3_NOT_OPENED_STUDY		(1 << 5)
#define	SJ3_CLOSE_MDICT_ERROR		(1 << 6)
#define	SJ3_CLOSE_UDICT_ERROR		(1 << 7)
#define	SJ3_CLOSE_STUDY_ERROR		(1 << 8)

/*
 *	sj3_touroku/sj3_syoukyo error code
 */
#define	SJ3_DICT_ERROR			1
#define	SJ3_DICT_LOCKED			2
#define	SJ3_BAD_YOMI_STR		3
#define	SJ3_BAD_KANJI_STR		4
#define	SJ3_BAD_HINSI_CODE		5

#define	SJ3_WORD_EXIST			6
#define	SJ3_DOUON_FULL			7
#define	SJ3_DICT_FULL			8
#define	SJ3_INDEX_FULL			9
#define	SJ3_TOUROKU_FAILED		10

#define	SJ3_WORD_NOT_EXIST		6
#define	SJ3_SYOUKYO_FAILED		10


/*
 *	Bunsyo Ikkatu Henkan 
 */
#define	SJ3_IKKATU_YOMI			512		/* max yomi */


/*
 *	Bunsetu Henkan
 */
#define	SJ3_BUNSETU_YOMI		128		/* max yomi */
#define	SJ3_BUNSETU_KANJI		512		/* kanji buf */


/*
 *	dummy
 */
#ifndef	SJ3_WORD_ID_SIZE
#define	SJ3_WORD_ID_SIZE		32
#endif

typedef	struct	studyrec {
	unsigned char	dummy[SJ3_WORD_ID_SIZE];
} SJ3_STUDYREC;


/*
 *	Bunsetu structure
 */
typedef	struct bunsetu {
	int		srclen;				/* length of yomi */
	int		destlen;			/* length of kanji */
	unsigned char	*srcstr;			/* yomi */
	unsigned char	*deststr;			/* kanji */
	struct studyrec	dcid;				/* study id */
} SJ3_BUNSETU;

/*
 *	Douongo structure
 */
typedef	struct douon {
	unsigned char	ddata[SJ3_BUNSETU_KANJI];	/* kanji data */
	int		dlen;				/* length of kanji */
	SJ3_STUDYREC	dcid;				/* study id */
} SJ3_DOUON;

/*
 *	Hinsi code
 */
#define	SJ3_H_NRMNOUN	1		/* Futuu Meisi */
#define	SJ3_H_PRONOUN	12		/* Dai Meisi */
#define	SJ3_H_LNAME	21		/* Myouji */
#define	SJ3_H_FNAME	22		/* Namae */
#define	SJ3_H_LOCNAME	24		/* Chimei */
#define	SJ3_H_PREFIC	25		/* Ken/Kumei */
#define	SJ3_H_RENTAI	26		/* Rentaisi */
#define	SJ3_H_CONJUNC	27		/* Setuzokusi */
#define	SJ3_H_SUBNUM	29		/* Josuusi */
#define	SJ3_H_NUMERAL	30		/* Suusi */
#define	SJ3_H_PREFIX	31		/* Settougo */
#define	SJ3_H_POSTFIX	36		/* Setubigo */
#define	SJ3_H_ADVERB	45		/* Fukusi */
#define	SJ3_H_ADJECT	60		/* Keiyousi */
#define	SJ3_H_ADJVERB	71		/* Keiyou Dousi */
#define	SJ3_H_SILVERB	80		/* Sahen Dousi */
#define	SJ3_H_ZILVERB	81		/* Zahen Dousi */
#define	SJ3_H_ONEVERB	90		/* Ichidan Dousi */
#define	SJ3_H_KAVERB	91		/* Kagyou Godan Dousi */
#define	SJ3_H_GAVERB	92		/* Gagyou Godan Dousi */
#define	SJ3_H_SAVERB	93		/* Sagyou Godan Dousi */
#define	SJ3_H_TAVERB	94		/* Tagyou Godan Dousi */
#define	SJ3_H_NAVERB	95		/* Nagyou Godan Dousi */
#define	SJ3_H_BAVERB	96		/* Bagyou Godan Dousi */
#define	SJ3_H_MAVERB	97		/* Magyou Godan Dousi */
#define	SJ3_H_RAVERB	98		/* Ragyou Godan Dousi */
#define	SJ3_H_WAVERB	99		/* Wagyou Godan Dousi */
#define	SJ3_H_SINGLE	189		/* Tan Kanji */

#if NeedFunctionPrototypes
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif

int	sj3_open __P(( char *, char * ));
int	sj3_getkan __P(( unsigned char *, SJ3_BUNSETU *, unsigned char *, int ));
int	sj3_getkan_mb __P(( unsigned char *, SJ3_BUNSETU *, unsigned char *, int ));
int	sj3_douoncnt __P(( unsigned char * ));
int	sj3_douoncnt_mb __P(( unsigned char * ));
int	sj3_getdouon __P(( unsigned char *, SJ3_DOUON * ));
int	sj3_getdouon_mb __P(( unsigned char *, SJ3_DOUON * ));
int	sj3_gakusyuu __P(( SJ3_STUDYREC * ));
int	sj3_gakusyuu2 __P(( unsigned char *, unsigned char *, SJ3_STUDYREC * ));
int	sj3_gakusyuu2_mb __P(( unsigned char *, unsigned char *, SJ3_STUDYREC * ));
int	sj3_touroku __P(( unsigned char *, unsigned char *, int ));
int	sj3_touroku_mb __P(( unsigned char *, unsigned char *, int ));
int	sj3_syoukyo __P(( unsigned char *, unsigned char *, int ));
int	sj3_syoukyo_mb __P(( unsigned char *, unsigned char *, int ));
int	sj3_close __P(( void ));
int	sj3_lockserv __P(( void ));
int	sj3_unlockserv __P(( void ));
int	sj3_getdict __P(( unsigned char * ));
int	sj3_getdict_mb __P(( unsigned char * ));
int	sj3_nextdict __P(( unsigned char * ));
int	sj3_nextdict_mb __P(( unsigned char * ));

#endif /* _Sj3lib_H */
