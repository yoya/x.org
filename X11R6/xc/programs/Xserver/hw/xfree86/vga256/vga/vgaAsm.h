/* $XConsortium: vgaAsm.h,v 1.3 95/01/13 20:13:39 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga256/vga/vgAsm.h,v 3.0 1994/11/26 12:48:16 dawes Exp $ */

/* Definitions for VGA bank assembler routines */

#ifdef CSRG_BASED
#define VGABASE CONST(0xFF000000)
#else
#define VGABASE CONST(0xF0000000)
#endif

