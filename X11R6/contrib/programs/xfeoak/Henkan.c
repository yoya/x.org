/* @(#)Henkan.c      6.1 94/06/01 20:04:27 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993  FUJITSU LIMITED

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

#include <stdio.h>
#include "Henkan.h"

#ifndef       AIXV3
typedef unsigned short	ushort;
typedef unsigned char	uchar;
#endif

#define Static static

DictionaryFuncRec *current_dictionary_func;
extern DictionaryFuncRec dmy_dictionary_func;

#ifdef OAK_DICTIONARY					/* SXG OAK */
extern DictionaryFuncRec sxg_dictionary_func;
#endif /*OAK_DICTIONARY*/				/* SXG OAK */

#ifdef OAK2_DICTIONARY					/* OAK */
extern DictionaryFuncRec oak_dictionary_func;
#endif /*OAK2_DICTIONARY*/				/* OAK */


#ifdef SJ3_DICTIONARY					/* SJ3 */
extern DictionaryFuncRec sj3_dictionary_func;
#endif /*SJ3_DICTIONARY*/				/* SJ3 */


DictionaryFuncRec *dictionary_func_tbl[] = {

    &dmy_dictionary_func,

#ifdef OAK_DICTIONARY					/* SXG OAK */
    &sxg_dictionary_func,				/* SXG OAK */
#endif /*OAK_DICTIONARY*/				/* SXG OAK */

#ifdef OAK2_DICTIONARY					/* OAK */
    &oak_dictionary_func,				/* OAK */
#endif /*OAK2_DICTIONARY*/				/* OAK */

#ifdef SJ3_DICTIONARY					/* SJ3 */
    &sj3_dictionary_func				/* SJ3 */
#endif /*SJ3_DICTIONARY*/				/* SJ3 */

};

int     dictionary_no = 0;

int	no_of_dictionary_func = sizeof(dictionary_func_tbl)
			      / sizeof(dictionary_func_tbl[0]);

int
Dic_Argv( argc, argv)
    int 	argc;
    char	*argv[];
{
    int		j, rc;
    DictionaryFuncRec *current_dictionary_func;

    for( j=0; j < no_of_dictionary_func; j++) {
	current_dictionary_func = dictionary_func_tbl[j];
	if( current_dictionary_func->selecut_name
	&& !strcmp(argv[0], current_dictionary_func->selecut_name)) {
	    dictionary_no = j;
	    return( 1 );
	}
    }

    for( j=0; j < no_of_dictionary_func; j++) {
	current_dictionary_func = dictionary_func_tbl[j];
	if( current_dictionary_func->argv ) {
	    rc = (*current_dictionary_func->argv)( argc, argv);
	    if( rc != 0 )
	        return( rc );
        }
    }
    return( 0 );
}

int
Dic_Help( fp)
FILE	*fp;
{
    int		j;
    DictionaryFuncRec *current_dictionary_func;

    if( no_of_dictionary_func > 2 ) {
        for( j=0; j < no_of_dictionary_func-1; j++) {
	    current_dictionary_func = dictionary_func_tbl[j];
	    if( *current_dictionary_func->selecut_name ) {
		fprintf(fp,"\t[%s ", current_dictionary_func->selecut_name );
		if( current_dictionary_func->help ) {
		   (*current_dictionary_func->help)( fp );
		}
    		fprintf(fp,"]\n");
	    }
	}
    } else {
	current_dictionary_func = dictionary_func_tbl[0];
	if( current_dictionary_func->help ) {
	    fprintf(fp,"\t");
	    (*current_dictionary_func->help)( fp );
    	    fprintf(fp,"\n");
	}
    }
    return( 0);
}

int
Dic_Open()
{
    current_dictionary_func = dictionary_func_tbl[ dictionary_no ];
    if( current_dictionary_func->open )
        return (*current_dictionary_func->open)();
    return(0);
}

int
Dic_Close()
{
    if( current_dictionary_func->close )
        return (*current_dictionary_func->close)();
    return(0);
}

int
Dic_Henkan( cnvrt, count, yomi, yomi_len, out_buff, out_size)
uchar	cnvrt;
int	count;
ushort	*yomi;
int	yomi_len;
ushort	*out_buff;
int	out_size;
{
    if( current_dictionary_func->henkan )
        return (*current_dictionary_func->henkan)( cnvrt, count, yomi,
					yomi_len, out_buff, out_size);
    return(0);
}

int
Dic_Zenkouho( type, yomi, yomi_len )
int 	type;
ushort *yomi;
int	yomi_len;
{
    if( current_dictionary_func->zenkouho )
        return (*current_dictionary_func->zenkouho)( type, yomi, yomi_len );
    return(0);
}

int
Dic_HenkanCancel()
{
    if( current_dictionary_func->cancel )
        return (*current_dictionary_func->cancel)();
    return(0);
}
