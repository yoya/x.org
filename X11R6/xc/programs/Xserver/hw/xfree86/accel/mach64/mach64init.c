/* $XConsortium: mach64init.c,v 1.3 95/01/16 13:16:33 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/mach64/mach64init.c,v 3.2 1995/01/15 10:31:10 dawes Exp $ */
/*
 * Written by Jake Richter
 * Copyright (c) 1989, 1990 Panacea Inc., Londonderry, NH - All Rights Reserved
 * Copyright 1993,1994 by Kevin E. Martin, Chapel Hill, North Carolina.
 *
 * This code may be freely incorporated in any program without royalty, as
 * long as the copyright notice stays intact.
 *
 * Additions by Kevin E. Martin (martin@cs.unc.edu)
 *
 * KEVIN E. MARTIN AND RICKARD E. FAITH DISCLAIM ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL KEVIN E. MARTIN BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Modified for the Mach-8 by Rickard E. Faith (faith@cs.unc.edu)
 * Rewritten for the Mach32 by Kevin E. Martin (martin@cs.unc.edu)
 * Rewritten for the Mach64 by Kevin E. Martin (martin@cs.unc.edu)
 *
 */

#include "X.h"
#include "input.h"

#include "xf86.h"
#include "xf86_OSlib.h"
#include "mach64.h"
#include "ativga.h"
#define XCONFIG_FLAGS_ONLY
#include "xf86_Config.h"

extern Bool xf86Verbose;

static LUTENTRY oldlut[256];
static Bool LUTInited = FALSE;

static int old_DAC_CNTL;
static int old_DAC_MASK;
static int old_DAC_in_clock;
static int old_DAC_out_clock;
static int old_DAC_mux_clock;
static int old_clock_sel;

static Bool mach64Inited = FALSE;

pointer mach64VideoMem = NULL;
unsigned int mach64MemRegOffset;

static char old_ATI2E, old_ATI32, old_ATI36;
static char old_GRA06, old_SEQ02, old_SEQ04;

static unsigned long old_BUS_CNTL;
static unsigned long old_CONFIG_CNTL;
static unsigned long old_MEM_CNTL;

static unsigned long old_CRTC_OFF_PITCH;
static unsigned long old_DST_OFF_PITCH;
static unsigned long old_SRC_OFF_PITCH;


/*
 * mach64CalcCRTCRegs --
 *	Initializes the Mach64 for the currently selected CRTC parameters.
 */
void mach64CalcCRTCRegs(crtcRegs, mode)
     mach64CRTCRegPtr crtcRegs;
     DisplayModePtr mode;
{
    int VRAMshift, RAMType, MHz, Res;
    int i;

    crtcRegs->h_total_disp =
	(((mode->CrtcHDisplay >> 3) - 1) << 16) |
	    ((mode->CrtcHTotal >> 3) - 1);
    crtcRegs->h_sync_strt_wid =
	(((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) >> 3) << 16) |
	    ((mode->CrtcHSyncStart >> 3) - 1);

    if ((crtcRegs->h_sync_strt_wid >> 16) > 0x1f) {
	ErrorF("%s %s: Horizontal Sync width (%d) in mode \"%s\"\n",
	       XCONFIG_PROBED, mach64InfoRec.name,
	       crtcRegs->h_sync_strt_wid >> 16,
	       mode->name);
	ErrorF("\tshortened to 248 pixels\n");
	crtcRegs->h_sync_strt_wid &= 0x001fffff;
    }

    if (mode->Flags & V_NHSYNC)
	crtcRegs->h_sync_strt_wid |= CRTC_H_SYNC_NEG;

    crtcRegs->v_total_disp =
	((mode->CrtcVDisplay - 1) << 16) |
	    (mode->CrtcVTotal - 1);
    crtcRegs->v_sync_strt_wid =
	((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) << 16) |
	    (mode->CrtcVSyncStart - 1);

    if ((crtcRegs->v_sync_strt_wid >> 16) > 0x1f) {
	ErrorF("%s %s: Vertical Sync width (%d) in mode \"%s\"\n",
	       XCONFIG_PROBED, mach64InfoRec.name,
	       crtcRegs->v_sync_strt_wid >> 16,
	       mode->name);
	ErrorF("\tshortened to 31 lines\n");
	crtcRegs->v_sync_strt_wid &= 0x001fffff;
    }

    if (mode->Flags & V_NVSYNC)
	crtcRegs->v_sync_strt_wid |= CRTC_V_SYNC_NEG;

    switch(mach64InfoRec.bitsPerPixel)
    {
	case 8:
    	    crtcRegs->color_depth = CRTC_PIX_WIDTH_8BPP;
	    break;
	case 16:
	    if (mach64WeightMask == RGB16_555)
		crtcRegs->color_depth = CRTC_PIX_WIDTH_15BPP;
	    else
		crtcRegs->color_depth = CRTC_PIX_WIDTH_16BPP;
	    break;
	case 32:
    	    crtcRegs->color_depth = CRTC_PIX_WIDTH_32BPP;
	    break;
    }

    crtcRegs->crtc_gen_cntl = 0;
    if (mode->Flags & V_INTERLACE)
	crtcRegs->crtc_gen_cntl |= CRTC_INTERLACE_EN;
    if (mode->Flags & V_CSYNC)
	crtcRegs->crtc_gen_cntl |= CRTC_CSYNC_EN;
    if (OFLG_ISSET(OPTION_CSYNC, &mach64InfoRec.options))
	crtcRegs->crtc_gen_cntl |= CRTC_CSYNC_EN;

    crtcRegs->clock_cntl = mode->Clock;
    crtcRegs->dot_clock = mach64InfoRec.clock[mode->Clock] / 10;
    crtcRegs->fifo_v1 =	mach64FIFOdepth((crtcRegs->color_depth >> 8),
					crtcRegs->dot_clock);
}

/*
 * mach64FIFOdepth --
 *	Calculates the correct FIFO depth for th Mach64 depending on the
 *	color depth and clock selected.
 */
int mach64FIFOdepth(cdepth, clock)
    int cdepth;
    int clock;
{
    int fifo_v1;
    int div[8] = { 6, 6, 12, 12, 12, 12, 12, 12 };
    int fifoVals[][6] = { { 0x02, 0x0c, 0x02, 0x06, 0x02, 0x0e},
			  { 0x02, 0x0e, 0x02, 0x0e, 0x02, 0x0e},
			  { 0x02, 0x0e, 0x02, 0x0e, 0x02, 0x0e},
			  { 0x02, 0x0e, 0x02, 0x0e, 0x02, 0x0e} };

    fifo_v1 = clock/100;

    switch (cdepth) {
    case CRTC_PIX_WIDTH_16BPP:
	fifo_v1 <<= 1;
	break;
    case CRTC_PIX_WIDTH_32BPP:
	fifo_v1 <<= 2;
	break;
    default:
	break;
    }

    fifo_v1 /= div[mach64MemorySize];

    if (fifo_v1 < fifoVals[cdepth - 2][mach64MemorySize * 2])
	fifo_v1 = fifoVals[cdepth - 2][mach64MemorySize * 2];
    if (fifo_v1 > fifoVals[cdepth - 2][mach64MemorySize * 2 + 1])
	fifo_v1 = fifoVals[cdepth - 2][mach64MemorySize * 2 + 1];

    return(fifo_v1);
}


#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
/*
 * mach64StrobeClock --
 *
 */
void mach64StrobeClock()
{
    char tmp;

    usleep(26); /* delay for 26 us */
    tmp = inb(ioCLOCK_CNTL);
    outb(ioCLOCK_CNTL, tmp | CLOCK_STROBE);
}

/*
 * mach64ICS2595_1bit --
 *
 */
mach64ICS2595_1bit(data)
    char data;
{
    char tmp;

    tmp = inb(ioCLOCK_CNTL);
    outb(ioCLOCK_CNTL, (tmp & ~0x04) | (data << 2));

    tmp = inb(ioCLOCK_CNTL);
    outb(ioCLOCK_CNTL, (tmp & ~0x08) | (0 << 3));

    mach64StrobeClock();

    tmp = inb(ioCLOCK_CNTL);
    outb(ioCLOCK_CNTL, (tmp & ~0x08) | (1 << 3));

    mach64StrobeClock();
}

/*
 * mach64ProgramICS2595 --
 *
 */
void mach64ProgramICS2595(clkCntl, MHz100)
    int clkCntl;
    int MHz100;
{
    char old_clock_cntl;
    char old_crtc_ext_disp;
    unsigned int program_word;
    unsigned int divider;
    int i;

#define MAX_FREQ_2595 15938
#define ABS_MIN_FREQ_2595 1000
#define MIN_FREQ_2595 8000
#define N_ADJ_2595 257
#define REF_DIV_2595 46
#define REF_FREQ_2595 1432
#define STOP_BITS_2595 0x1800

    old_clock_cntl = inb(ioCLOCK_CNTL);
    outb(ioCLOCK_CNTL, 0);

    old_crtc_ext_disp = inb(ioCRTC_GEN_CNTL+3);
    outb(ioCRTC_GEN_CNTL+3, old_crtc_ext_disp | (CRTC_EXT_DISP_EN >> 24));

    usleep(15000); /* delay for 15 ms */

    /* Calculate the programming word */
    program_word = -1;
    divider = 1;

    if (MHz100 > MAX_FREQ_2595)
	MHz100 = MAX_FREQ_2595;
    else if (MHz100 < ABS_MIN_FREQ_2595)
	program_word = 0;
    else
	while (MHz100 < MIN_FREQ_2595) {
	    MHz100 *= 2;
	    divider *= 2;
	}

    MHz100 *= 1000;
    MHz100 = (REF_DIV_2595*MHz100)/REF_FREQ_2595;

    MHz100 += 500;
    MHz100 /= 1000;

    if (program_word == -1) {
	program_word = MHz100 - N_ADJ_2595;
	switch (divider) {
	case 1:
	    program_word |= 0x0600;
	    break;
	case 2:
	    program_word |= 0x0400;
	    break;
	case 4:
	    program_word |= 0x0200;
	    break;
	case 8:
	default:
	    break;
	}
    }

   program_word |= STOP_BITS_2595;

    /* Program the clock chip */
    outb(ioCLOCK_CNTL, 0);
    mach64StrobeClock();
    outb(ioCLOCK_CNTL, 1);
    mach64StrobeClock();

    mach64ICS2595_1bit(1); /* Send start bits */
    mach64ICS2595_1bit(0);
    mach64ICS2595_1bit(0);

    for (i = 0; i < 5; i++) {
	mach64ICS2595_1bit(clkCntl & 1);
	clkCntl >>= 1;
    }

    for (i = 0; i < 8 + 1 + 2 + 2; i++) {
	mach64ICS2595_1bit(program_word & 1);
	program_word >>= 1;
    }

    usleep(1000); /* delay for 1 ms */

    inb(ioDAC_REGS); /* Clear DAC Counter */
    outb(ioCRTC_GEN_CNTL+3, old_crtc_ext_disp);
    outb(ioCLOCK_CNTL, old_clock_cntl | CLOCK_STROBE);
}


/*
 * mach64ProgramClk --
 *	Program the clock chip for the use with RAMDAC.
 */
void mach64ProgramClk(clkCntl, MHz100)
    int clkCntl;
    int MHz100;
{
    switch (mach64ClockType) {
    case 1:
	mach64ProgramICS2595(clkCntl, MHz100);
	break;
    case 2:
    default:
	ErrorF("mach64ProgramClk: ClockType %d not currently supported.\n",
	       mach64ClockType);
	break;
    }
}
#endif


/*
 * mach64SetCRTCRegs --
 *	Initializes the Mach64 for the currently selected CRTC parameters.
 */
void mach64SetCRTCRegs(crtcRegs)
     mach64CRTCRegPtr crtcRegs;
{
    int crtcGenCntl;

    /* Now initialize the display controller part of the Mach64.
     * The CRTC registers are passed in from the calling routine.
     */

    WaitIdleEmpty();
    crtcGenCntl = regr(CRTC_GEN_CNTL);
    regw(CRTC_GEN_CNTL, crtcGenCntl & ~CRTC_EXT_EN);

#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
    /* Check to see if we need to program the clock chip */
    if (mach64ClockType != 0 && mach64Ramdac == DAC_ATI68860_ATI68880)
	if (crtcRegs->clock_cntl & 0x30) {
	    mach64ProgramClk(mach64CXClk, crtcRegs->dot_clock);
	    crtcRegs->clock_cntl = mach64CXClk;
	}
#endif

    WaitQueue(12);
    /* Horizontal CRTC registers */
    regw(CRTC_H_TOTAL_DISP,    crtcRegs->h_total_disp);
    regw(CRTC_H_SYNC_STRT_WID, crtcRegs->h_sync_strt_wid);

    /* Vertical CRTC registers */
    regw(CRTC_V_TOTAL_DISP,    crtcRegs->v_total_disp);
    regw(CRTC_V_SYNC_STRT_WID, crtcRegs->v_sync_strt_wid);

    /* Clock select register */
    regw(CLOCK_CNTL, crtcRegs->clock_cntl | CLOCK_STROBE);

    /* Zero overscan register to insure proper color */
    regw(OVR_CLR, 0);
    regw(OVR_WID_LEFT_RIGHT, 0);
    regw(OVR_WID_TOP_BOTTOM, 0);

    /* Set the width of the display */
    regw(CRTC_OFF_PITCH, (mach64VirtX >> 3) << 22);
    regw(DST_OFF_PITCH, (mach64VirtX >> 3) << 22);
    regw(SRC_OFF_PITCH, (mach64VirtX >> 3) << 22);

    /* Display control register -- this one turns on the display */
    regw(CRTC_GEN_CNTL,
	 (crtcGenCntl & 0xff0000ff &
	  ~(CRTC_PIX_BY_2_EN | CRTC_DBL_SCAN_EN | CRTC_INTERLACE_EN)) |
	 crtcRegs->color_depth |
	 (crtcRegs->crtc_gen_cntl & ~CRTC_PIX_BY_2_EN) |
	 (crtcRegs->fifo_v1 << 16) |
	 CRTC_EXT_DISP_EN | CRTC_EXT_EN);

    /* Set the DAC for the currect mode */
    mach64SetRamdac(crtcRegs->color_depth >> 8, TRUE);

    WaitIdleEmpty();
    crtcGenCntl = regr(CRTC_GEN_CNTL) & ~CRTC_PIX_BY_2_EN;
    if (crtcRegs->crtc_gen_cntl & CRTC_PIX_BY_2_EN)
	crtcGenCntl |= CRTC_PIX_BY_2_EN;
    regw(CRTC_GEN_CNTL, crtcGenCntl);
}

/*
 * mach64SaveLUT --
 *	 Saves the LUT in lut.
 */
void mach64SaveLUT(lut)
     LUTENTRY *lut;
{
    int i;

    /* set DAC read index */
    WaitQueue(1);
    outb(ioDAC_REGS+3, 0);

    for (i = 0; i < 256; i++) {
        WaitQueue(3);
        lut[i].r = inb(ioDAC_REGS+1);
        lut[i].g = inb(ioDAC_REGS+1);
        lut[i].b = inb(ioDAC_REGS+1);
    }
}

/*
 * mach64RestoreLUT -- 
 *	Restores the LUT in lut.
 */
void mach64RestoreLUT(lut)
     LUTENTRY *lut;
{
    int i;

    /* set DAC write index */
    WaitQueue(1);
    outb(ioDAC_REGS, 0);

    for (i = 0; i < 256; i++) {
        WaitQueue(3);
        outb(ioDAC_REGS+1, lut[i].r);
        outb(ioDAC_REGS+1, lut[i].g);
        outb(ioDAC_REGS+1, lut[i].b);
    }
}

/*
 * mach64InitLUT --
 *	Loads the Look-Up Table with all black.
 *	Assumes 8-bit board is in use.
 */
void mach64InitLUT()
{
    int i;

    mach64SaveLUT(oldlut);
    LUTInited = TRUE;

    /* set DAC write index */
    WaitQueue(1);
    outb(ioDAC_REGS, 0);

    /* Load the LUT entries */
    for (i = 0; i < 256; i++) {
        WaitQueue(3);
        outb(ioDAC_REGS+1, 0);
        outb(ioDAC_REGS+1, 0);
        outb(ioDAC_REGS+1, 0);
    }
}

/*
 * mach64ResetEngine --
 *	Resets the GUI engine and clears any FIFO errors.
 */
void mach64ResetEngine()
{
    /* Reset Engine */
    regw(GEN_TEST_CNTL,0);
    regw(GEN_TEST_CNTL, GUI_ENGINE_ENABLE);

    /* PCI: Write to status registers if (mach64BusType == PCI) */
    regw(GUI_STAT, 0);
    regw(FIFO_STAT, 0);

    /* Ensure engine is not locked up by clearing any FIFO errors */
    regw(BUS_CNTL, (regr(BUS_CNTL) & 0xff00ffff) | 0x00ff0000);

    /* Reset engine again for security */
    regw(GEN_TEST_CNTL,0);
    switch (mach64MemType) {
    case DRAMx4:
    case DRAMx16:
    case GraphicsDRAMx16:
	regw(GEN_TEST_CNTL, GUI_ENGINE_ENABLE);
	break;
    case VRAMx16:
    case VRAMx16ssr:
    case EnhancedVRAMx16:
    case EnhancedVRAMx16ssr:
	regw(GEN_TEST_CNTL, GUI_ENGINE_ENABLE | BLOCK_WRITE_ENABLE);
	break;
    default:
	regw(GEN_TEST_CNTL, GUI_ENGINE_ENABLE);
	ErrorF("mach64ResetEngine: Unknown Memory Type (%d)\n", mach64MemType);
    }

    WaitIdleEmpty();
}

/*
 * mach64InitEnvironment --
 *	Initializes the Mach64's drawing environment and clears the display.
 */
void mach64InitEnvironment()
{
    mach64ResetEngine();

    WaitIdleEmpty();

    /* Set current color depth (8bpp) */
    switch (mach64InfoRec.bitsPerPixel) {
    case 8:
	regw(DP_PIX_WIDTH, HOST_8BPP | SRC_8BPP | DST_8BPP);
	regw(DP_CHAIN_MASK, DP_CHAIN_8BPP);
	break;
    case 16:
	if (mach64WeightMask == RGB16_555) {
	    regw(DP_PIX_WIDTH, HOST_16BPP | SRC_15BPP | DST_15BPP);
	    regw(DP_CHAIN_MASK, DP_CHAIN_15BPP);
	} else {
	    regw(DP_PIX_WIDTH, HOST_16BPP | SRC_16BPP | DST_16BPP);
	    regw(DP_CHAIN_MASK, DP_CHAIN_16BPP);
	}
	break;
    case 32:
	regw(DP_PIX_WIDTH, HOST_32BPP | SRC_32BPP | DST_32BPP);
	regw(DP_CHAIN_MASK, DP_CHAIN_32BPP);
	break;
    }

    regw(CONTEXT_MASK, 0xffffffff);

    regw(DST_Y_X, 0);
    regw(DST_HEIGHT, 0);
    regw(DST_BRES_ERR, 0);
    regw(DST_BRES_INC, 0);
    regw(DST_BRES_DEC, 0);
    regw(DST_CNTL, (DST_X_LEFT_TO_RIGHT | DST_Y_TOP_TO_BOTTOM));

    regw(SRC_Y_X, 0);
    regw(SRC_HEIGHT1_WIDTH1, 0);
    regw(SRC_Y_X_START, 0);
    regw(SRC_HEIGHT2_WIDTH2, 0);
    regw(SRC_CNTL, 0);

    WaitQueue(15);
    regw(HOST_CNTL, 0);
    regw(PAT_REG0, 0);
    regw(PAT_REG1, 0);
    regw(PAT_CNTL, 0);

    regw(SC_LEFT_RIGHT, ((mach64MaxX << 16) | 0 ));
    regw(SC_TOP_BOTTOM, ((mach64MaxY << 16) | 0 ));

    regw(DP_BKGD_CLR, 0);
    regw(DP_FRGD_CLR, 1);
    regw(DP_WRITE_MASK, 0xffffffff);
    regw(DP_MIX, (MIX_SRC << 16) | MIX_DST);
    regw(DP_SRC, FRGD_SRC_FRGD_CLR);

    regw(CLR_CMP_CLR, 0);
    regw(CLR_CMP_MASK, 0xffffffff);
    regw(CLR_CMP_CNTL, 0);

    regw(GUI_TRAJ_CNTL, DST_X_LEFT_TO_RIGHT | DST_Y_TOP_TO_BOTTOM);
    WaitIdleEmpty();
}

/*
 * mach64InitAperture --
 *	Initialize the aperture for the Mach64.
 */
void mach64InitAperture(screen_idx)
    int screen_idx;
{
    int i, apsize, apaddr;

    if (!mach64VideoMem) {
	old_CONFIG_CNTL = inw(ioCONFIG_CNTL);
    }

    if (mach64BusType == PCI)
	apaddr = 0x7c000000;
    else
	apaddr = 0x04000000;  /* for VLB board */

    if (mach64MemorySize <= MEM_SIZE_4M) {
	apsize = 4 * 1024 * 1024;
	mach64MemRegOffset = 0x3ffc00;
	outw(ioCONFIG_CNTL, ((apaddr/(4*1024*1024)) << 4) | 1);
    } else {
	apsize = 8 * 1024 * 1024;
	mach64MemRegOffset = 0x7ffc00;
	outw(ioCONFIG_CNTL, ((apaddr/(4*1024*1024)) << 4) | 2);
    }

    if (!mach64VideoMem) {
	mach64VideoMem = xf86MapVidMem(screen_idx, LINEAR_REGION, 
				       (pointer)apaddr, apsize);
	if (xf86Verbose) {
	    ErrorF("%s %s: Aperture mapped to 0x%x\n",
		   XCONFIG_PROBED, mach64InfoRec.name, apaddr);
	}
    }
}

/* 
 * mach64SetRamdac --
 * 
 */
void mach64SetRamdac(colorDepth, AccelMode)
    int colorDepth;
    int AccelMode;
{
    int temp, temp2, mask;
    int gmode[]  = {0x01, 0x82, 0x83, 0xA0, 0xA1, 0xC0, 0xE3, 0x80};
    int dsetup[] = {0x63, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x61};

    WaitIdleEmpty();
    if (!AccelMode) {
	regwb(CRTC_GEN_CNTL+3, 0);
    }

    temp = regrb(CRTC_GEN_CNTL+3);
    regwb(CRTC_GEN_CNTL+3, temp | (CRTC_EXT_DISP_EN >> 24));

    switch(mach64Ramdac) {
    case DAC_ATI68860_ATI68880:
	if (!mach64DAC8Bit) {
	    dsetup[1] |= 0x01;
	    dsetup[2] |= 0x01;
	}

	if (!AccelMode)
	    colorDepth = 7;

	temp2 = inb(ioDAC_CNTL);
	outb(ioDAC_CNTL, (temp2 & ~DAC_EXT_SEL_RS2) | DAC_EXT_SEL_RS3);

	outb(ioDAC_REGS+2, 0x1d);
	outb(ioDAC_REGS+3, gmode[colorDepth]);
	outb(ioDAC_REGS, 0x02);

	temp2 = inb(ioDAC_CNTL);
	outb(ioDAC_CNTL, temp2 | DAC_EXT_SEL_RS2 | DAC_EXT_SEL_RS3);

	if (mach64MemorySize < MEM_SIZE_1M)
	    mask = 0x04;
	else if (mach64MemorySize == MEM_SIZE_1M)
	    mask = 0x08;
	else
	    mask = 0x0c;

#define A860_DELAY_L	0

	/* Don't know why this is necessary, but it fixes the problems
	 * snow on the screen above 2Mb on my system. */
	mask |= 0x80;

	temp2 = inb(ioDAC_REGS);
	outb(ioDAC_REGS, (dsetup[colorDepth] | mask) | (temp2 & A860_DELAY_L));
	temp2 = inb(ioDAC_CNTL);
	outb(ioDAC_CNTL, (temp2 & ~(DAC_EXT_SEL_RS2 | DAC_EXT_SEL_RS3)));
	break;
    }

    (void)inb(ioDAC_REGS);
    regwb(CRTC_GEN_CNTL+3, temp);
}

/*
 * mach64InitDisplay --
 *	Initializes the display for the Mach64.
 */
void mach64InitDisplay(screen_idx)
     int screen_idx;
{
    if (mach64Inited)
	return;

    xf86EnableIOPorts(mach64InfoRec.scrnIndex);
    mach64InitAperture(screen_idx);
    mach64ResetEngine();

    mach64SaveVGAInfo(screen_idx);

    outb(ATIEXT, ATI2E); old_ATI2E = inb(ATIEXT+1);
    outb(ATIEXT, ATI32); old_ATI32 = inb(ATIEXT+1);
    outb(ATIEXT, ATI36); old_ATI36 = inb(ATIEXT+1);
    outb(VGAGRA, GRA06); old_GRA06 = inb(VGAGRA+1);
    outb(VGASEQ, SEQ02); old_SEQ02 = inb(VGASEQ+1);
    outb(VGASEQ, SEQ04); old_SEQ04 = inb(VGASEQ+1);

    old_DAC_MASK = inb(ioDAC_REGS+2);

    WaitIdleEmpty();

    old_BUS_CNTL = regr(BUS_CNTL);
    old_MEM_CNTL = regr(MEM_CNTL);

    old_CRTC_OFF_PITCH = regr(CRTC_OFF_PITCH);
    old_DST_OFF_PITCH = regr(DST_OFF_PITCH);
    old_SRC_OFF_PITCH = regr(SRC_OFF_PITCH);

    /* Turn off the VGA memory boundary */
    regw(MEM_CNTL, old_MEM_CNTL & 0xfff8ffff);

    WaitQueue(4);

    /* Disable all interrupts. */
    regw(CRTC_INT_CNTL, 0);

    /* Initialize the drawing and display offsets */
    regw(CRTC_OFF_PITCH, (mach64VirtX >> 3) << 22);
    regw(DST_OFF_PITCH, (mach64VirtX >> 3) << 22);
    regw(SRC_OFF_PITCH, (mach64VirtX >> 3) << 22);

    mach64AdjustFrame(mach64InfoRec.frameX0, mach64InfoRec.frameY0);

    /* Save the colormap */
    if (mach64InfoRec.bitsPerPixel == 8)
        mach64InitLUT();

    old_DAC_CNTL = regr(DAC_CNTL);
    if (mach64DAC8Bit)
	regw(DAC_CNTL, old_DAC_CNTL | DAC_8BIT_EN);

    WaitIdleEmpty(); /* Make sure that all commands have finished */

    mach64Inited = TRUE;
}

/*
 * mach64Cleanup -- 
 * 	Resets the state of the video display for text.
 */
void mach64CleanUp()
{
    if (!mach64Inited)
	return;

    WaitIdleEmpty();

    switch(mach64Ramdac) {
    case DAC_ATI68860_ATI68880:
	mach64SetRamdac(CRTC_PIX_WIDTH_8BPP >> 8, FALSE);
	break;
    }

    if (LUTInited) {
	mach64RestoreLUT(oldlut);
	LUTInited = FALSE;
    }

    /* Reset the VGA registers */
    WaitQueue(6);
    outw(ATIEXT, ATI2E | old_ATI2E << 8);
    outw(ATIEXT, ATI32 | old_ATI32 << 8);
    outw(ATIEXT, ATI36 | old_ATI36 << 8);
    outw(VGAGRA, GRA06 | old_GRA06 << 8);
    outw(VGASEQ, SEQ02 | old_SEQ02 << 8);
    outw(VGASEQ, SEQ04 | old_SEQ04 << 8);

    WaitQueue(8);
    outb(ioDAC_REGS+2, old_DAC_MASK);
    regw(DAC_CNTL, old_DAC_CNTL);

    regw(BUS_CNTL, old_BUS_CNTL);
    regw(MEM_CNTL, old_MEM_CNTL);

    regw(CRTC_OFF_PITCH, old_CRTC_OFF_PITCH);
    regw(DST_OFF_PITCH, old_DST_OFF_PITCH);
    regw(SRC_OFF_PITCH, old_SRC_OFF_PITCH);

    regw(CRTC_GEN_CNTL, 0x00020200);

    mach64CursorOff();

    mach64RestoreVGAInfo();

    outw(ioCONFIG_CNTL, old_CONFIG_CNTL);

    xf86DisableIOPorts(mach64InfoRec.scrnIndex);

    mach64Inited = FALSE;
}
