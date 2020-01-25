/*
 * (c) Copyright 1994 by Holger Veit
 *			<Holger.Veit@gmd.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
 * HOLGER VEIT  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 * Except as contained in this notice, the name of Holger Veit shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from Holger Veit.
 *
 */

/* $XConsortium: OS_Os2.c,v 1.1 95/01/06 20:47:15 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/SuperProbe/OS_Os2.c,v 3.0 1994/12/17 09:58:09 dawes Exp $ */


#include "Probe.h"

#define INCL_32
#define INCL_DOS
#include <os2.h>

#define UNUSED (Byte*)0xffffffffl

HFILE consFd = -1;
HFILE testFd = -1;
ULONG action;
char *videoDrvPath = "\\DEV\\SMVDD01$";
char *testCfgPath = "\\DEV\\TESTCFG$";
Byte* videoAddr = UNUSED;

/*
 * OpenVideo --
 *
 * Enable access to the installed video hardware.  For OS/2, we take
 * advantage of a special driver that is part of MMPM/2, which is a
 * package that is part of OS/2 2.X, and should also be part of OS/2 3.0.
 */
int OpenVideo()
{
	if (DosOpen((PSZ)videoDrvPath, (PHFILE)&consFd, (PULONG)&action,
	   (ULONG)0, FILE_SYSTEM, FILE_OPEN,
	   OPEN_SHARE_DENYNONE|OPEN_FLAGS_NOINHERIT|OPEN_ACCESS_READONLY,
	   (ULONG)0) != 0 ||
	   DosOpen((PSZ)testCfgPath, (PHFILE)&testFd, (PULONG)&action,
	   (ULONG)0, FILE_SYSTEM, FILE_OPEN,
	   OPEN_SHARE_DENYNONE|OPEN_FLAGS_NOINHERIT|OPEN_ACCESS_READONLY,
	   (ULONG)0) != 0)
		return -1;
	return 1;
}

/* this structure is used as a parameter packet for the direct access
 * ioctl of smvdd.sys
 */
typedef struct {
	ULONG	hstream;
	ULONG 	hid;
	ULONG	flag;
	ULONG	addr;
	ULONG	size;
} DIOParPkt;

/* this structure is used as a return value for the direct access ioctl
 * of smvdd.sys. Attention this structure is unaligned and packed!
 */
typedef struct {
	USHORT	lin[3]; /* this is actually USHORT,ULONG but unpacked */
} DIODtaPkt;

typedef union {
	ULONG l;
	USHORT s[2];
} S2L;

static Byte* UnmapPhys()
{
	DIOParPkt	par;
	ULONG		plen;

	par.hstream	= 0l;
	par.hid		= 0l;
	par.flag	= 0l;
	par.addr	= 0l;
	par.size	= 0l;
	plen 		= sizeof(par);

	if (consFd != -1)
		DosDevIOCtl(consFd, (ULONG)0x81, (ULONG)0x42,
		   (PVOID)&par, (ULONG)plen, (PULONG)&plen,
		   NULL, 0, NULL);
	return UNUSED; /* no error */
}

/*
 * CloseVideo --
 *
 * Disable access to the video hardware.
 */
void CloseVideo()
{
	if (videoAddr != UNUSED)
		videoAddr = UnmapPhys();

	DosClose(consFd);
	DosClose(testFd);
}

/*
 * MapVGA --
 *
 * Map the VGA memory window (0xA0000-0xAFFFF) as read/write memory for
 * the process for use in probing memory.
 */
Byte *MapVGA()
{
	DIOParPkt	par;
	ULONG		plen;
	DIODtaPkt	dta;
	ULONG		dlen;

	par.hstream	= 0;
	par.hid		= 0;
	par.flag	= 1l;
	par.addr	= 0xa0000l;
	par.size	= 0x10000l;
	plen 		= sizeof(par);

	if (consFd != -1 &&
	    DosDevIOCtl(consFd, (ULONG)0x81, (ULONG)0x42,
	       (PVOID)&par, (ULONG)plen, (PULONG)&plen,
	       (PVOID)&dta, (ULONG)6, (PULONG)&dlen) == 0) {
		S2L x;
		x.s[0] = dta.lin[1];
		x.s[1] = dta.lin[2];
		if (dlen==6) {
			videoAddr = (Byte*)x.l;
			return videoAddr;
		}
	} else {
		fprintf(stderr, "%s: Failed to map framebuffer\n", MyName);
		return (Byte*)0;
	}
}

/*
 * UnMapVGA --
 *
 * Unmap the VGA memory window.
 */
void UnMapVGA(base)
Byte *base;
{
	UnmapPhys();
}

/*
 * ReadBIOS --
 *
 * Read 'Len' bytes from the video BIOS at address 'Bios_Base'+'Offset' into 
 * buffer 'Buffer'.
 */

int ReadBIOS(Offset, Buffer, Len)
unsigned Offset;
Byte *Buffer;
int Len;
{
	struct {
		ULONG cmd;
		ULONG phys;
		USHORT len;
	} par;
	UCHAR	*dta;
	ULONG	plen,dlen;
	int 	i;

	par.cmd 	= 0;
	par.phys 	= (ULONG)Bios_Base+(Offset & 0xffff8000);
	par.len 	= (Offset & 0x7fff) + Len;
	plen 		= sizeof(par);

	dta		= (UCHAR*)malloc(par.len);
	dlen 		= Len;

	if (DosDevIOCtl(testFd, (ULONG)0x80, (ULONG)0x40,
	   (PVOID)&par, (ULONG)plen, (PULONG)&plen,
	   (PVOID)dta, (ULONG)dlen, (PULONG)&dlen)) {
		fprintf(stderr, "%s: BIOS map failed, addr=%lx\n",
			MyName, Bios_Base+Offset);
		return -1;
	}

	/*
 	 * Sanity check...
 	 */
	if ((Offset & 0x7fff) != 0 && 
		(dta[0] != 0x55 || dta[1] != 0xaa)) {
		fprintf(stderr, 
			"%s: BIOS sanity check failed, addr=%x\n",
			MyName, Bios_Base+Offset);
		return -1;
	}

	memcpy(Buffer,dta + (Offset & 0x7fff), Len);
	free(dta);
	return(Len);
}

/*
 * EnableIOPort --
 *
 * Enable access to 'NumPorts' IO ports listed in array 'Ports'.
 * This is a no op here: I/O access is implicitly enabled for this 
 * process through a special library
 */
/*ARGSUSED*/
int EnableIOPorts(NumPorts, Ports)
CONST int NumPorts;
CONST Word *Ports;
{
	return(0);
}

/*
 * DisableIOPort --
 *
 * Disable access to 'NumPorts' IO ports listed in array  'Ports'.
 * This is a no op here: I/O access is implicitly enabled for this 
 * process through a special library.
 */
/*ARGSUSED*/
int DisableIOPorts(NumPorts, Ports)
CONST int NumPorts;
CONST Word *Ports;
{
	return(0);
}

/*
 * ShortSleep --
 *
 * Sleep for the number of milliseconds specified in 'Delay'.
 * 
 */
void ShortSleep(Delay)
int Delay;
{
	DosSleep(Delay ? Delay : 1);
}
