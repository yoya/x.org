/* $XConsortium: xf86_IlHack.c,v 1.2 94/10/12 20:48:48 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/os-support/misc/xf86_IlHack.c,v 3.0 1994/07/24 11:51:47 dawes Exp $ */
/*
 * This file is an incredible crock to get the normally-inline functions
 * built into the server so that things can be debugged properly.
 *
 * Note: this doesn't work when using a compiler other than GCC.
 */


#define static /**/
#define __inline__ /**/
#undef NO_INLINE
#include "compiler.h"
