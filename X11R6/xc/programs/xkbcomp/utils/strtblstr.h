#ifndef	STRTBLSTR_H
#define	STRTBLSTR_H	1

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

  /*\
   * $XConsortium: strtblstr.h,v 1.2 94/04/04 15:31:40 rws Exp $
   *
   *		              COPYRIGHT 1990
   *		        DIGITAL EQUIPMENT CORPORATION
   *		           MAYNARD, MASSACHUSETTS
   *			    ALL RIGHTS RESERVED.
   *
   * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
   * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
   * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE 
   * FOR ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED 
   * WARRANTY.
   *
   * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
   * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
   * ADDITION TO THAT SET FORTH ABOVE.
   *
   * Permission to use, copy, modify, and distribute this software and its
   * documentation for any purpose and without fee is hereby granted, provided
   * that the above copyright notice appear in all copies and that both that
   * copyright notice and this permission notice appear in supporting
   * documentation, and that the name of Digital Equipment Corporation not be
   * used in advertising or publicity pertaining to distribution of the 
   * software without specific, written prior permission.
  \*/

#include "strtbl.h"

typedef union _STRTBLENTRY {
	char		*string;
	int		 next;
} StringTableEntry;

#define	stCreateToken(ch,ndx)  ((StringToken)(((ch&0xff)<<24)|((ndx)&0xffffff)))
#define	stTokenIndex(tok)      (((unsigned)(tok))&0xffffff)
#define	stTokenChar(tok)       ((((unsigned)(tok))>>24)&0xff)

_XFUNCPROTOEND

#endif /* STRTBLSTR_H */
