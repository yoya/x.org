/* $SonyId: SjString.h,v 1.2 1994/06/03 06:33:04 makoto Exp $ */
/******************************************************************

Copyright (c) 1994  Sony Corporation

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
#ifndef	_SjString_H
#define _SjString_H
#include	"sjxa.h"

#ifdef HAS_JLS_FUNC

#include	<jctype.h>

#else /* !HAS_JLS_FUNC */

#define euc2sjis(c)	(jis2sjis((c) & 0x7f7f))
#define jis2euc(c)	((c) | 0x8080)
#define kuten2euc(c)	(kuten2jis(c) | 0x8080)
#define kuten2sjis(c)	(jis2sjis(kuten2jis(c)))

#define	iseuc(c)	((unsigned char)(c) >= 0xa1 &&		\
			 (unsigned char)(c) <= 0xfe)
#define issjis1(c)	(((unsigned char)(c) >= 0x81 &&		\
			  (unsigned char)(c) <= 0x9f) ||	\
			 ((unsigned char)(c) >= 0xe0 &&		\
			  (unsigned char)(c) <= 0xfc))
#define issjis2(c)	((unsigned char)(c) >= 0x40  &&		\
			 (unsigned char)(c) <= 0xfc  &&		\
			 (unsigned char)(c) != 0x7f)
#define iskana2(c)	((unsigned char)(c) >= 0xa1 &&		\
			 (unsigned char)(c) <= 0xdf)
#endif /* HAS_JLS_FUNC */

#define SS2	0x8e
#define SS3	0x8f


#if NeedFunctionPrototypes
#define __P(protos)	protos
#else /* !NeedFunctionPrototypes */
#define __P(protos)	()
#endif /* NeedFunctionPrototypes */

#ifndef HAS_JLS_FUNC
int		 iskanji __P(( int ));
int		 iskana __P(( int ));
unsigned short	 sjis2euc __P(( unsigned short ));
unsigned short	 jis2sjis __P(( unsigned short ));
unsigned short	 kuten2jis __P(( unsigned short ));
unsigned short	 jtohira __P(( unsigned short ));
#endif /* !HAS_JLS_FUNC */
size_t		 mbs2wcs __P(( wchar_t *, const char *, size_t ));
size_t		 wcs2mbs __P(( char *, const wchar_t *, size_t ));
#if !defined(sun)  ||  !defined(SVR4)
wchar_t		*wscat __P(( wchar_t *, wchar_t * ));
wchar_t		*wscpy __P(( wchar_t *, wchar_t * ));
wchar_t		*wsncpy __P(( wchar_t *, wchar_t *, int ));
int		 wscmp __P(( wchar_t *, wchar_t * ));
int		 wsncmp __P(( wchar_t *, wchar_t *, int ));
int		 wslen __P(( wchar_t * ));
#endif
int		 wcswidth __P(( wchar_t * ));
wchar_t		 kuten2wc __P(( unsigned short ));
wchar_t		 sjis2wc __P(( unsigned short ));
wchar_t		 euc2wc __P(( unsigned short ));
wchar_t		 jis2wc __P(( unsigned short ));
int		 wc2sjis __P(( char *, wchar_t ));
int		 sjiss2wcs __P(( wchar_t *, unsigned char *, int ));
int		 wcs2sjiss __P(( char [], wchar_t *, int ));
int		 eucs2wcs __P(( wchar_t [], char *, int ));
char		*Sjis2Euc __P(( char * ));
char		*Euc2Sjis __P(( char * ));
unsigned short	 isdakuon __P(( unsigned char, unsigned, ConversionMode ));
unsigned short	 ishandakuon __P(( unsigned char, unsigned char, ConversionMode ));
void		 hankaku2zenkaku __P(( wchar_t *, wchar_t [], ConversionMode ));
void		 ZKtoZH __P(( wchar_t *, wchar_t [] ));
void		 ZHtoZK __P(( wchar_t *, wchar_t [] ));
void		 ZHtoHK __P(( wchar_t *, wchar_t [] ));
int		 StrnCaseCmp __P(( char *, char *, int ));
#endif /* _SjString_H */
