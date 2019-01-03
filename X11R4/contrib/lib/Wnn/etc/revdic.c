/*
	Copyright
		Kyoto University Research Institute for Mathematical Sciences
		1987, 1988, 1989
	Copyright OMRON TATEISI ELECTRONICS CO. 1987, 1988, 1989
	Copyright ASTEC, Inc. 1987, 1988, 1989


    Permission to use, copy, modify, and distribute this software
    and its documentation for any purpose and without any fee is
    hereby granted, subject to the following restrictions:

    The above copyright notice and this permission notice must appear
    in all versions of this software;

    The name of "Wnn" may not be changed; 

    All documentations of software based on "Wnn" must contain the wording
    "This software is based on the original version of Wnn developed by
    Kyoto University Research Institute for Mathematical Sciences (KURIMS),
    OMRON TATEISI ELECTRONICS CO. and
    ASTEC, Inc.", followed by the above copyright notice;

    The name of KURIMS, OMRON and ASTEC may not be used
    for any purpose related to the marketing or advertising
    of any product based on this software.

    KURIMS, OMRON and ASTEC make no representations about
    the suitability of this software for any purpose.
    It is provided "as is" without express or implied warranty.

    KURIMS, OMRON and ASTEC DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
    SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
    IN NO EVENT SHALL KURIMS, OMRON AND ASTEC BE LIABLE FOR ANY SPECIAL,
    INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
    LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
    OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
    PERFORMANCE OF THIS SOFTWARE.

    Author: Hideki Tsuiki	Kyoto University
				tsuiki%kaba.or.jp@uunet.uu.net

	    Hiroshi Kuribayashi	OMRON TATEISI ELECTRONICS CO.
				kuri@frf.omron.co.jp
				uunet!nff.ncl.omron.co.jp!kuri

	    Naouki Nide		Kyoto University
				nide%kaba.or.jp@uunet.uu.net

	    Shozo Takeoka	ASTEC, Inc.
				take%astec.co.jp@uunet.uu.net

	    Takashi Suzuki	Advanced Software Technology & Mechatronics
				Research Institute of KYOTO
				suzuki%astem.or.jp@uunet.uu.net

*/
/*	Version 4.0
 */
#include <stdio.h>
#include "commonhd.h"
#include "jslib.h"
#include "jh.h"
#include "jdata.h"


#ifndef min
#define min(a, b) ((a > b)? b:a)
#define max(a, b) ((a < b)? b:a)
#endif


/*
 * rev_ud_hontai(hostart, maxhontai)
 * rev_ud_table(tary, maxtable)
 * rev_sd_hontai(hostart)
 * rev_hinsi(hinsi, maxserial)
 */

static int tmptmp;

#define rev_int(x) (tmptmp = *(x), *(x) = (tmptmp >> 24) & 0xff | \
			              ((tmptmp >> 16) & 0xff) << 8 | \
			              ((tmptmp >> 8) & 0xff) << 16 | \
			              ((tmptmp & 0xff) << 24))

#define rev_short(x) (tmptmp = *(x), *(x) = (tmptmp >> 8) & 0xff | \
		                            (tmptmp & 0xff) << 8)

#define rev_int_org(x) (tmptmp = *(x), *(x) = (tmptmp >> 24) & 0xff | \
			              ((tmptmp >> 16) & 0xff) << 8 | \
			              ((tmptmp >> 8) & 0xff) << 16 | \
			              ((tmptmp & 0xff) << 24), tmptmp)

#define rev_short_org(x) (tmptmp = *(x), *(x) = (tmptmp >> 8) & 0xff | \
		                            (tmptmp & 0xff) << 8, tmptmp)



rev_ud_hontai(hostart, maxhontai, match)
UCHAR *hostart;
int maxhontai;
int match;  /* whether the cpu type match the dictionary */
{
    UCHAR *hoend = hostart + maxhontai;
    struct uind2 *hop, *hop1;
    int k;
    int len;

    for(hop = (struct uind2 *)hostart;(UCHAR *)hop < hoend;){
	rev_int(&(hop->next));
	rev_int(&(hop->serial));
	rev_int(&(hop->kanjipter));
	rev_short(&(hop->kosuu));
	if(match){
	    len = hop->yomi[0];
	    hop1 = (struct uind2 *)(AL_INT(&(hop->yomi[0]) + 1
					   + max((len - 4),0)));
	    rev_short(&(hop->yomi[0]));
	    for(k = 0 ; k < len - 4; k++){
		rev_short(&(hop->yomi[k + 1]));
	    }
	    hop = hop1;
	}else{
	    rev_short(&(hop->yomi[0]));
	    len = hop->yomi[0];
	    for(k = 0 ; k < len - 4; k++){
		rev_short(&(hop->yomi[k + 1]));
	    }
	    hop = (struct uind2 *)(AL_INT(&(hop->yomi[0]) + 1
					   + max((len - 4),0)));
	}
    }
}


rev_ud_table(tary, maxtable, match)
struct uind1 *tary;
int maxtable;
int match;
{
    int k;

    for(k = 0 ; k < maxtable; k++){
	rev_int(&(tary[k].pter1));
	rev_int(&(tary[k].pter));
	rev_int(&(tary[k].yomi1));
	rev_int(&(tary[k].yomi2));
    }
}

rev_hinsi(hinsi, maxserial, match)
unsigned short *hinsi;
int maxserial;
int match;
{
    int k;

    for(k = 0 ; k < maxserial; k++){
	rev_short(&(hinsi[k]));
    }
}

/*
 * first element of each node is syurui,
 * which is unsigned short less than 0xff.
 */

#define match_machine(x) (*(unsigned short *)(x) & 0xff)


rev_sd_hontai(hostart, match)
int match;
{
    rev_sd_hontai0(hostart, hostart, match);
}

rev_sd_hontai0(hopter, hostart, match)
UCHAR *hostart;
UCHAR *hopter;
int match;
{
    if(match){
	travel_next_nodes(hopter, hostart, match);
	rev_sd_node(hopter, hostart, match);
    }else{
	rev_sd_node(hopter, hostart, match);
	travel_next_nodes(hopter, hostart, match);
    }
}

travel_next_nodes(hopter, hostart, match)
UCHAR *hostart, *hopter;
int match;
{
    int k;
    int tsize;
    w_char *charst;
    w_char *sumst;
    int *ptrst;
    
    switch(*(unsigned short *)hopter){
    case ST_NORMAL:
	tsize = *(w_char *)(hopter + 2);
	charst = (w_char *)(hopter + 12);
	sumst = ((w_char *)charst + tsize + 2); /* + 2 keeps two zero words */
	ptrst = (int *)((w_char *)sumst + tsize);
	for(k = 0 ; k < tsize ; k++){
	    if(ptrst[k] != ENDPTR){
		rev_sd_hontai0(hostart + ptrst[k], hostart, match);
	    }
	}
	break;
    case ST_NOPTER:
	break;
    case ST_NOENT:
	tsize = *(w_char *)(hopter + 2);
	charst = (w_char *)(hopter + 4);
	ptrst = (int *)AL_INT((w_char *)charst + tsize);
	for(k = 0 ; k < tsize ; k++){
	    rev_sd_hontai0(hostart + ptrst[k], hostart, match);
	}
	break;
    case ST_SMALL:
	rev_sd_hontai0(hopter + 4, hostart, match);
	break;
    }	
}

#define rev_if_short(x) (match? rev_short_org(x): rev_short(x))
#define rev_if_int(x) (match? rev_int_org(x): rev_int(x))

rev_sd_node(hopter, hostart, match)
UCHAR *hostart, *hopter;
int match;
{
    /* whether current state of dict matches the cpu type of the machine */
    int k;
    int tsize;
    w_char *charst;
    w_char *sumst;
    int *ptrst;
    int state;
    
    state = rev_if_short((unsigned short *)hopter);
    switch(state){
    case ST_NORMAL:
    case ST_NOPTER:
	tsize = rev_if_short((w_char *)(hopter + 2));
	rev_int((int *)(hopter + 4));
	rev_int((int *)(hopter + 8));
	charst = (w_char *)(hopter + 12);
	sumst = ((w_char *)charst + tsize + 2); /* + 2 keeps two zero words */
	ptrst = (int *)((w_char *)sumst + tsize);
	for(k = 0 ; k < tsize ; k++){
	    rev_short(charst + k);
	    rev_short(sumst + k);
	}
	if(state == ST_NORMAL){
	    for(k = 0 ; k < tsize ; k++){
		rev_int(ptrst + k);
	    }
	}
	break;
    case ST_NOENT:
	tsize = rev_if_short((w_char *)(hopter + 2));
	charst = (w_char *)(hopter + 4);
	ptrst = (int *)AL_INT((w_char *)charst + tsize);
	for(k = 0 ; k < tsize ; k++){
	    rev_short(charst + k);
	    rev_int(ptrst + k);
	}
	break;
    case ST_SMALL:
	rev_short((unsigned short *)(hopter + 2));
	break;
    }	
}


little_endian()
{
    int a = 1;
    return(*(char *)&a == 1);
}

rev_rd_rind1(ri1, maxri1, match)
struct rind1 *ri1;
int maxri1;
int match;
{
    int k;

    for(k = 0 ; k < maxri1; k++){
	rev_int(&(ri1[k].pter1));
	rev_int(&(ri1[k].pter));
    }
}

rev_rd_rind2(ri2, maxri2, match)
struct rind2 *ri2;
int maxri2;
int match;
{
    int k;

    for(k = 0 ; k < maxri2; k++){
	rev_int(&(ri2[k].kanjipter));
	rev_int(&(ri2[k].next[D_YOMI]));
	rev_int(&(ri2[k].next[D_KANJI]));
    }
}

rev_kanji(kpter, maxk, match)
UCHAR *kpter;
int maxk;
int match;
{
    UCHAR *kend = kpter + maxk;

    while(kpter < kend && *kpter){
	rev_w_char((w_char *)(kpter + 2), *kpter / 2 - 1);
	kpter += *kpter;
    }
}

rev_w_char(p, maxp)
w_char *p;
int maxp;
{
    for(;maxp > 0; maxp--){
	rev_short(p++);
    }
}

/*
 *  Change the byte-order of dictionary except for the header area.
 *  Header and hindo area don't depend on byte-order.
 *  If match is nonzero, the dict-type matches CPU type before conversion.
 */

revdic(jtl, match)
struct JT *jtl;
int match;
{
    if(jtl->syurui == WNN_UD_DICT){
      if(rev_ud(jtl, match) == -1) return(-1);
    }else if (jtl->syurui == WNN_REV_DICT){
      if(rev_rd(jtl, match) == -1) return(-1);
    }else{
      if(rev_sd(jtl, match) == -1) return(-1);
    }
    return(0);
}

rev_ud(jtl, match)
struct JT *jtl;
int match;
{
    rev_common(jtl, match);
    rev_ud_hontai(jtl->hontai,jtl->maxhontai, match);
    rev_ud_table(jtl->table,jtl->maxtable, match);
    return(0);
}

rev_sd(jtl, match)
struct JT *jtl;
int match;
{
    rev_common(jtl, match);
    rev_sd_hontai(jtl->hontai, match);
    return(0);
}

rev_rd(jtl, match)
struct JT *jtl;
int match;
{
    rev_common(jtl, match);
    rev_rd_rind1(jtl->ri1[D_YOMI],jtl->maxri1[D_YOMI], match);
    rev_rd_rind1(jtl->ri1[D_KANJI],jtl->maxri1[D_KANJI], match);
    rev_rd_rind2(jtl->ri2,jtl->maxri2, match);
    return(0);
}

rev_common(jtl, match)
struct JT *jtl;
int match;
{
    rev_hinsi(jtl->hinsi, jtl->maxserial, match);
    rev_kanji(jtl->kanji, jtl->maxkanji, match);
    rev_w_char(jtl->comment, jtl->maxcomment);
    rev_w_char(jtl->hinsi_list, jtl->maxhinsi_list);
}
