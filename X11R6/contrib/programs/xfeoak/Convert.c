/* @(#)Convert.c      6.1 94/06/01 20:04:27 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993, 1994  FUJITSU LIMITED

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

#include <ctype.h>			/* islower	*/
#ifndef	_toupper
#define _toupper(c)    ((c)-'a'+'A')
#endif
#include "sxc/slcode.h"
#include "Xfeoak.h"
#include "KanjiTbl.h"
#include "Henkan.h"

Static int	RomajiHenkan();
Static int	CancelRomaji();
Static int	SetOutputBuff();
Static int	Henkan();
Static int	HenkanCancel();
Static int	Kakutei();

int
fj_PutKana( string, length)
	unsigned char	*string;
	int 		length;
	{
	Kakutei(0);
	CancelRomaji();
	for(; length>0; length--) {
	    SetOutputBuff( *string++ );
	    }
	DisplayOutputBuff();
	DisplayCursor();
	return( 0);
	}

int
fj_PutAscii( string, length)
	unsigned char	*string;
	int		length;
	{
	Kakutei(0);
	if( (current_client->siftmode & FS_KANA )
	&&  (current_client->romaji_mode) )
	    RomajiHenkan( string );	
	else {
	    for(; length>0; length--) {
	        SetOutputBuff( *string++ );
		}
	    }
	DisplayOutputBuff();
	DisplayCursor();
	return( 0);
	}

Static int
RomajiHenkan( string )
	unsigned char *string;
	{
	unsigned char	ac, ec;
	int		num, j;

	if( current_client->slcrtok_input_no == 0) {
	    current_client->romaji_colums = current_client->current_colums;
	    bzero( current_client->slcrtok_inbuf,  4 );
	    bzero( current_client->slcrtok_outbuf, 4 );
	    if( current_client->last_colums > current_client->current_colums ) {
		CopyUshort( current_client->input_buff,
		  current_client->output_buff, current_client->last_colums); 
		}
	    }
	while( ac= *string++ ) {
	    if( islower(ac) ) ac = _toupper( (int)ac); 
            if( ac == '-'
            && current_client->current_colums > 0 ) {
    	        unsigned short ww, w0;
    	        ww = current_client->output_buff[current_client->current_colums-1];
    	        w0 = ww & 0xff00;
    	        if( w0 == 0xA400 || w0 == 0xA500
    	        || ( ww >= 0xa0 && ww <= 0xDF )
    	        ||  ww == 0xA1AB || ww == 0xA1AC || ww == 0xA1BC )
    	            ac = 0xB0;
    	    }
	    ec = slcatoe( ac, C_JPNKAT);
	    num = slcrtok( ec, current_client->slcrtok_inbuf,
			       current_client->slcrtok_outbuf,
			  (int)current_client->slcrtok_nhtype);
	    if( num != 0 ) {
		if( num < 0 ) num = -num;
		if( (int)current_client->slcrtok_input_no >= num ) {
		    int old_num = (int)current_client->slcrtok_input_no;
		    if( current_client->romaji_colums + old_num
		     == current_client->last_colums )
			current_client->last_colums = current_client->romaji_colums;
		    else {	
			int	num2 = current_client->romaji_colums + num;
			int	length  = old_num - num;
			if( num > 1 ) {
			    num2--;
			    length++;
			    }
			CopyUshort( &current_client->output_buff[num2],
			  	    &current_client->input_buff[num2], length );
		        }
		    }
		current_client->current_colums = current_client->romaji_colums;
		for( j=0; j<num; j++) {
		    ec = current_client->slcrtok_outbuf[j];
		    ac = slcetoa( ec, C_JPNKAT);	
		    if( ac > 0)
		        SetOutputBuff( ac );	
		    }
	        bzero( current_client->slcrtok_outbuf, 4 );
	        current_client->romaji_colums = current_client->current_colums; 
		ec = current_client->slcrtok_inbuf[0];
		if( ec == 0 ) {
		    current_client->slcrtok_input_no = 0;
		    }
		else {
		    current_client->slcrtok_input_no = 1;	
		    ac = slcetoa( ec, C_JPNKAT);
		    if( ac > 0)
		        SetOutputBuff( ac );
		    }
		}
	    else {
	        current_client->slcrtok_input_no++;
		ac = slcetoa( ec, C_JPNKAT);
		if( ac > 0)
		    SetOutputBuff( ac ); 
		}
	    }
	return( 0);
	}

Static int
CancelRomaji()
	{
	if( current_client->slcrtok_input_no != 0) {
	    current_client->slcrtok_input_no = 0;
	    bzero( current_client->slcrtok_inbuf, 4);
	    bzero( current_client->slcrtok_outbuf, 4);
	    }
	current_client->romaji_colums = current_client->last_colums;
	return( 0);
	}

Static int
FlashRomaji()
	{
	unsigned char	ac, ec = 0xfe;
	int	num, j;
	if( current_client->slcrtok_input_no != 0) {
	    num = slcrtok( ec, current_client->slcrtok_inbuf,
			       current_client->slcrtok_outbuf,
			  (int)current_client->slcrtok_nhtype);
	    if( num != 0 ) {
		if( num < 0 ) num = -num;
		num--;
		if( (int)current_client->slcrtok_input_no >= num ) {
		    int old_num = current_client->slcrtok_input_no;
		    if( current_client->romaji_colums + old_num
		     == current_client->last_colums )
			current_client->last_colums = current_client->romaji_colums;
		    else {	
			int	num2 = current_client->romaji_colums + num;
			int	length  = old_num - num;
			if( num > 1 ) {
			    num2--;
			    length++;
			    }
			CopyUshort( &current_client->output_buff[num2],
				    &current_client->input_buff[num2], length );
		        }
		    }
		current_client->current_colums = current_client->romaji_colums;
		for( j=0; j<num; j++) {
		    ec = current_client->slcrtok_outbuf[j];
		    ac = slcetoa( ec, C_JPNKAT );	
		    if( ac > 0)
		        SetOutputBuff( ac );	
		    }
		}
	    current_client->slcrtok_input_no = 0;
	    bzero( current_client->slcrtok_inbuf,  4 );
	    bzero( current_client->slcrtok_outbuf, 4 );
	    }
	current_client->romaji_colums = current_client->last_colums;
	return( 0);
	}

Static int
SetOutputBuff( ac )
	unsigned char ac;
	{
	ushort	buff, *bwc;
	int	cnv_kata = 0xa5a1 - 0xa4a1;
	int	kana = ac & 0x80;
	int	CheckKanaDakuten();

	if( current_client->current_colums >= FXIM_LINEBUFF_LENGTH - 2 ) {
		Beep();
		return(-1);
		}
	ac &= 0xff;
	if( current_client->hankakumode == 1 ) {
	    if( (kana != 0) && (current_client->siftmode == FS_HIRAGANA)) 
	        buff = Hankaku_To_ZenkakuAsciiHiragana( ac ); 
	    else
	        buff = ac;
	    }
	else {
	    buff = Hankaku_To_ZenkakuAsciiHiragana( ac );     
	    if( (kana != 0) && (current_client->siftmode == FS_KATAKANA)) 
		if( ac > 0xa5 && ac < 0xde && ac != 0xb0 )
	    	    buff += cnv_kata;
	    }
	current_client->input_flag = FINF_MODY;
	if( current_client->current_colums > 0 ) {
	    if( buff == 0xA1AB || 
	        buff == 0xA1AC    ) {
	    	bwc = &(current_client->output_buff[current_client->current_colums-1]);
	        if( CheckKanaDakuten(bwc,buff) == 0 )
	    	    return(0);
		}
	    }
	current_client->output_buff[current_client->current_colums++] = buff;
	if(current_client->current_colums > current_client->last_colums) {
	    current_client->last_colums = current_client->current_colums;
	    current_client->input_flag = FINF_ADD;
	    }
	return( 0);
	}

int
fj_Muhenkan()
	{
	int		num, rest_colums;
	ushort		*yomi, *out_buff, save;
	static int	muhenkan_count = 0;

	if( current_client->current_colums <= 0 )
	    return(0);

	yomi     = current_client->input_buff;
	out_buff = current_client->output_buff;
	if( current_client->convert_count == 0 ) {
	    FlashRomaji();
	    CopyUshort( yomi, out_buff, current_client->last_colums );
	    current_client->input_colums = current_client->last_colums;
	    current_client->convert_colums = current_client->current_colums;
	    muhenkan_count = 0;
	    }
	else
	if( current_client->convert_count > 0 ) {
	    muhenkan_count = 0;
	    Dic_HenkanCancel();
	    }
	current_client->convert_count = -1;
	
	save = yomi[current_client->convert_colums];
	yomi[current_client->convert_colums] = 0;
	if( muhenkan_count == 0 ) {
	    muhenkan_count = 1;
	    num = CopyUshort( out_buff, yomi, FXIM_LINEBUFF_LENGTH );
	    }
	else if( muhenkan_count == 1 ) {
	    muhenkan_count = 2;
	    num = Hiragana_To_Katakana( out_buff, yomi, FXIM_LINEBUFF_LENGTH );
	    }
	else {
	    muhenkan_count = 0;
	    num = ZenkakuAsciiKana_To_Hankaku( out_buff, yomi, FXIM_LINEBUFF_LENGTH, 1 );
	    }
	yomi[current_client->convert_colums] = save;
	
	rest_colums = current_client->input_colums - current_client->convert_colums;
	CopyUshort( &out_buff[num], &yomi[current_client->convert_colums], rest_colums );
	current_client->current_colums = num;
	current_client->reverse_colums = num;
	current_client->last_colums = num + rest_colums;
	current_client->input_flag = FINF_MODY;
	DisplayOutputBuff();
	DisplayCursor();       
	return( 0);
	}

int
fj_Henkan()
	{
	return Henkan(FKK_HENKAN);
	}

int
fj_Maekouho()
	{
	return Henkan(FKK_MAE);
	}

Static int
Henkan(fkk_type)
	unsigned char	fkk_type;
	{
	int	num;
	ushort	*yomi, *out_buff;

	if( current_client->current_colums <= 0 )
	    return(0);

	yomi     = current_client->input_buff;
	out_buff = current_client->output_buff;
	if( current_client->convert_count == 0 ) {
	    FlashRomaji();
	    CopyUshort( yomi, out_buff, current_client->last_colums );
	    current_client->input_colums = current_client->last_colums;
	    current_client->convert_colums = current_client->current_colums;
	    }
	else if( current_client->convert_count < 0 )
	    current_client->convert_count = 0;
	num = Dic_Henkan( fkk_type, (int)current_client->convert_count, yomi,
			  (int)current_client->convert_colums,
			  out_buff, FXIM_LINEBUFF_LENGTH);
	if( num == -1 ) {
	    Muhenkan();
	    current_client->current_colums = 0;
	    Beep();
	    }
	else {
	    int   rest_colums;
	    if( current_client->hankakumode ) {
		current_client->output_buff[num] = 0;
		num = ZenkakuAsciiKana_To_Hankaku( current_client->output_buff,
			current_client->output_buff, FXIM_LINEBUFF_LENGTH, 0);
		}
	    rest_colums = current_client->input_colums - current_client->convert_colums;
	    CopyUshort( &out_buff[num], &yomi[current_client->convert_colums], rest_colums );
	    current_client->current_colums = num;
	    current_client->reverse_colums = num;
	    current_client->last_colums = num + rest_colums;
	    if( fkk_type == FKK_HENKAN )
	        current_client->convert_count++;
	    else if( fkk_type == FKK_MAE ) {
		if( --(current_client->convert_count) <= 0)
		    current_client->convert_count = 1;
	        }
	    else    current_client->convert_count = 1;    
	    current_client->input_flag = FINF_MODY;
	    }
	DisplayOutputBuff();
	DisplayCursor();	
	return( 0);
	}

int
Muhenkan()
	{
	Dic_HenkanCancel();
	CopyUshort( current_client->output_buff, current_client->input_buff,
			current_client->input_colums );
	current_client->last_colums = current_client->input_colums;
	current_client->current_colums = current_client->convert_colums;
	current_client->reverse_colums = 0;
	current_client->input_colums = 0;
	current_client->convert_count = 0;
	current_client->input_flag = FINF_MODY;
	return( 0);
	}

int
fj_HenkanCancel()
	{
	Dic_HenkanCancel();

	if( current_client->convert_count != 0 ) {
	    Muhenkan();
	    current_client->current_colums = 0;
	    }
	else {
	    HenkanCancel();
	    }
	DisplayOutputBuff();
	DisplayCursor();
	return( 0);
	}

Static int
HenkanCancel()
	{
	current_client->last_colums = 0;
	current_client->current_colums = 0;
	current_client->input_colums = 0;
	current_client->convert_count = 0;
	current_client->reverse_colums = 0;
	CancelRomaji();
	current_client->input_flag = FINF_MODY;
	return( 0);
	}

int
fj_exec()
	{
	DisplayCursor();
	if( current_client->slcrtok_input_no > 0 ) {
	    Beep();
	    return(-1);
	    }
	Kakutei(1);
	DisplayCursor();
	return( 0);
	}

Static int
Kakutei(disp_sw)
	int	disp_sw;
	{
	int	length;
	if( current_client->convert_count != 0) {
	    current_client->convert_count = 0;
	    if( current_client != root_client ) {
		SendBuffByLength( current_client->window,
				  current_client->output_buff,
				  current_client->reverse_colums );
		sleep(0);
		}
	    length = current_client->last_colums - current_client->reverse_colums;
	    current_client->last_colums = length;
	    CopyUshort( &(current_client->output_buff[0]),
		     &(current_client->output_buff[current_client->reverse_colums]), length );
	    current_client->reverse_colums = 0;
	    current_client->current_colums = 0;
	    current_client->input_flag = FINF_MODY;
	    if( disp_sw )
	        DisplayOutputBuff();
	    }
	return( 0);
	}

int
fj_HenkanReset()
	{
	Fj_InitialMode();
	DisplayCursor();
	CancelRomaji();
	current_client->reverse_colums = current_client->last_colums; 
	current_client->convert_count = 1;	
	if( current_client != root_client
	&&  current_client->prot_type == FPRT_XIM ) {
	    XimResetSendBuffByLength( current_client->window,
				  current_client->output_buff,
				  current_client->reverse_colums );
	    sleep(0);
	    }
	current_client->last_colums = 0;
	current_client->reverse_colums = 0;
	current_client->current_colums = 0;
	current_client->input_flag = FINF_MODY;
	DisplayOutputBuff();
	DisplayCursor();
	return( 0);
	}

int
fj_CursorMove(flag)
	int	flag;
	{
	DisplayCursor();
	CancelRomaji();
	Kakutei(1);
	switch( flag ) {
	case KS_TOP:
	    current_client->current_colums = current_client->reverse_colums;
	    break;
	case KS_BOTTOM:
	    current_client->current_colums = current_client->last_colums;
	    break;
	case KS_FOWARD:
	    current_client->current_colums++;
	    if( current_client->current_colums > current_client->last_colums ) 
	        current_client->current_colums = current_client->last_colums;
	    break;
	case KS_BACKWARD:
	    current_client->current_colums--;
	    if( current_client->current_colums < current_client->reverse_colums ) 
	        current_client->current_colums = current_client->reverse_colums;
	    break;
	    }
	DisplayCursor();
	return( 0);
	}

int
fj_DeleteChar()
	{
	int length, j;
	Kakutei(0);
	if(current_client->last_colums > 0 ) { 
	    if( current_client->current_colums == current_client->last_colums ) {
	        Beep();
	        return(-1);
	        }
	    for(j=current_client->current_colums; j<current_client->last_colums; j++ ) 
	        current_client->output_buff[j] = current_client->output_buff[j+1];
	    current_client->last_colums--;
	    if(current_client->current_colums > current_client->last_colums)
	        current_client->current_colums--;
	    current_client->input_flag = FINF_MODY;
	    DisplayOutputBuff();
	    DisplayCursor();
	    }
	return( 0);
	}

int
fj_InsertChar()
	{
	int	j;
	Kakutei(0);
	for(j=current_client->last_colums-1; j>=current_client->current_colums; j-- ) 
	    current_client->output_buff[j+1] = current_client->output_buff[j];
	current_client->output_buff[current_client->current_colums]
	  = Hankaku_To_ZenkakuAsciiHiragana(' ');
	if( current_client->last_colums < FXIM_LINEBUFF_LENGTH - 2 )
	    current_client->last_colums++;
	current_client->input_flag = FINF_MODY;
	DisplayOutputBuff();
	DisplayCursor();
	return( 0);
	}

int
Beep()
	{
	if(!no_beep)
	    XBell(display, 50);
	return( 0);
	}
