/* $XConsortium: inout.s,v 1.3 94/10/12 20:50:05 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/os-support/shared/inout.s,v 3.0 1994/08/31 04:40:50 dawes Exp $ */

#include "assyntax.h"

/*
 *	Make i80386 io primitives available at C-level.
 */

	FILE("inout.s")
	AS_BEGIN
	SEG_TEXT

/*
 *-----------------------------------------------------------------------
 * inb ---
 *	Input one byte.
 *
 * Results:
 *      Byte in al.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(inb)
GLNAME(inb):
	MOV_L	(REGOFF(4,ESP),EDX)
	SUB_L	(EAX,EAX)
	IN1_B	(DX)
	RET

/*
 *-----------------------------------------------------------------------
 * outb ---
 *	Output one byte.
 *
 * Results:
 *      None.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(outb)
GLNAME(outb):
	MOV_L	(REGOFF(4,sp),EDX)
	MOV_L	(REGOFF(8,sp),EAX)
	OUT1_B	(DX)
	RET
/*
 *-----------------------------------------------------------------------
 * inw ---
 *	Input one 16-bit word.
 *
 * Results:
 *      Word in ax.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(inw)
GLNAME(inw):
	MOV_L	(REGOFF(4,ESP),EDX)
	IN1_W	(DX)
	RET

/*
 *-----------------------------------------------------------------------
 * outw ---
 *	Output one 16-bit word.
 *
 * Results:
 *      None.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(outw)
GLNAME(outw):
	MOV_L	(REGOFF(4,ESP),EDX)
	MOV_L	(REGOFF(8,ESP),EAX)
	OUT1_W	(DX)
	RET

/*
 *-----------------------------------------------------------------------
 * inl ---
 *	Input one 32-bit longword.
 *
 * Results:
 *      Word in eax.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(inl)
GLNAME(inl):
	MOV_L	(REGOFF(4,ESP),EDX)
	IN1_L	(EDX)
	RET

/*
 *-----------------------------------------------------------------------
 * outl ---
 *	Output one 32-bit longword.
 *
 * Results:
 *      None.
 *-----------------------------------------------------------------------
 */
	GLOBL	GLNAME(outl)
GLNAME(outl):
	MOV_L	(REGOFF(4,ESP),EDX)
	MOV_L	(REGOFF(8,ESP),EAX)
	OUT1_L	(EDX)
	RET

