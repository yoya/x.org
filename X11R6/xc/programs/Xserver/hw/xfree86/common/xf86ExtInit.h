/* $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86ExtInit.h,v 3.2 1994/12/25 13:46:17 dawes Exp $ */

/*
 * Dummy functions to disable various extensions
 */

#ifdef MONO_SERVER
/* For Mono and VGA16 servers, always turn off PEX and XIE */

void PexExtensionInit() {}

void XieInit() {}

#endif /* MONO_SERVER */

/* Hack to avoid multiple versions of dixfonts in vga{2,16}misc.o */
#ifndef LBX
void LbxFreeFontTag() {}
#endif
