/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: srchgrp.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:27 $
 */




#include "sj_euc.h"
#include "sj_kcnv.h"



Void	get_askknj();
Int codesize();



Int	srchg_atrb(p, ofs)
Uchar		*p;
TypeDicOfs	*ofs;
{
        if (((*p & KanjiCodeMask) << 8) + *(p + 1) == grp_grp)
		*ofs = grp_ptr - dicbuf;

	return 2;
}



Int	srchg_ofs(p, ofs)
Uchar		*p;
TypeDicOfs	*ofs;
{
	Int	contf = TRUE;

        p = dicbuf + ((*p & KanjiCodeMask) << 8) + *(p + 1);
	do {
		if (p[codesize(*p)] == KanjiStrEnd) contf = FALSE;
		switch(*p & KanjiModeMask) {
		      case ZenHiraAssyuku:
			p += srchg_norm1(p, ofs); break;
			
		      case ZenKataAssyuku:
			p += srchg_norm1(p, ofs); break;
			
		      case OffsetAssyuku:
                        p += srchg_ofs(p, ofs); break;
			
		      case AiAttribute:
			p += srchg_atrb(p, ofs); break;
			
		      case LeadingHankaku:
			p += srchg_norm2(p, ofs); break;
			
		      case KanjiAssyuku:
			p += srchg_knj(p, ofs); break;

		      case KanjiStrEnd:
			contf = FALSE; break;

		      default:
			p += srchg_norm2(p, ofs);
		}
	} while (contf);

	return 2;
}



Int	srchg_knj(p, ofs)
Uchar		*p;
TypeDicOfs	*ofs;
{
	Reg1 Int	contf = TRUE;

	p = askknj[*p & KnjAssyukuMask];
	do {
		if (p[codesize(*p)] == KanjiStrEnd) contf = FALSE;
		switch(*p&KanjiModeMask) {
		      case ZenHiraAssyuku:
			p += srchg_norm1(p, ofs); break;
			
		      case ZenKataAssyuku:
			p += srchg_norm1(p, ofs); break;
			
		      case OffsetAssyuku:
                        p += srchg_ofs(p, ofs); break;
			
		      case AiAttribute:
			p += srchg_atrb(p, ofs); break;
			
		      case LeadingHankaku:
			p += srchg_norm2(p, ofs); break;
			
		      case KanjiAssyuku:
			p += srchg_knj(p, ofs); break;

		      case KanjiStrEnd:
			contf = FALSE; break;

		      default:
			p += srchg_norm2(p, ofs);

		}
	} while (contf);

	return 1;
}



Int	srchg_norm1(p, ofs)
Uchar		*p;
TypeDicOfs	*ofs;
{
	return 1;
}



Int	srchg_norm2(p, ofs)
Uchar		*p;
TypeDicOfs	*ofs;
{
	return 2;
}



TypeDicOfs	srchgrp2nd()
{
	Uchar		*p;
	TypeDicOfs	ofs = 0;
	Int		contf;

	
	(*curdict->getdic)(curdict, grp_seg);
	get_askknj();

	
	while (*grp_ptr != HinsiBlkTerm) {

		
		for (contf = TRUE, p = grp_ptr ; contf ; ) {
			
			if (p[codesize(*p)] == KanjiStrEnd) contf = FALSE;

			
			switch(*p & KanjiModeMask) {
			      case ZenHiraAssyuku:
				p += srchg_norm1(p, &ofs); break;
				
			      case ZenKataAssyuku:
				p += srchg_norm1(p, &ofs); break;
				
			      case OffsetAssyuku:
				p += srchg_ofs(p, &ofs); break;
				
			      case AiAttribute:
				p += srchg_atrb(p, &ofs); break;
				
			      case LeadingHankaku:
				p += srchg_norm2(p, &ofs); break;

			      case KanjiAssyuku:
				p += srchg_knj(p, &ofs); break;

			      case KanjiStrEnd:
				contf = FALSE; break;

			      default:
				p += srchg_norm2(p, &ofs);
			}
		}
		p++; 
		
		grp_ptr = p;

		
		if (ofs) return ofs;
	}

	return 0;
}



TypeDicOfs	srchgrp1st(seg, ofs, grp)
TypeDicSeg	seg;
TypeDicOfs	ofs;
TypeGroup	grp;
{
	
	(*curdict->getdic)(curdict, grp_seg = seg);

	
	grp_ptr = dicbuf + ofs + 1;

	
	grp_grp = grp;

	
	return srchgrp2nd();
}
