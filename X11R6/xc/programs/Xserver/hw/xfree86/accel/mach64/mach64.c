/* $XConsortium: mach64.c,v 1.4 95/01/23 15:33:50 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/mach64/mach64.c,v 3.6 1995/01/21 07:15:01 dawes Exp $ */
/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 * Copyright 1993,1994 by Kevin E. Martin, Chapel Hill, North Carolina.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL, KEVIN E. MARTIN, AND RICKARD E. FAITH DISCLAIM ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Thomas Roell, roell@informatik.tu-muenchen.de
 *
 * Rewritten for the 8514/A by Kevin E. Martin (martin@cs.unc.edu)
 * Modified for the Mach-8 by Rickard E. Faith (faith@cs.unc.edu)
 * Rewritten for the Mach32 by Kevin E. Martin (martin@cs.unc.edu)
 * Rewritten for the Mach64 by Kevin E. Martin (martin@cs.unc.edu)
 *
 */

#include "X.h"
#include "Xmd.h"
#include "input.h"
#include "scrnintstr.h"
#include "pixmapstr.h"
#include "mipointer.h"
#include "cursorstr.h"
#include "mi.h"
#include "cfb.h"
#include "cfb16.h"
#include "cfb32.h"
#include "gc.h"
#include "windowstr.h"

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86Procs.h"
#include "xf86_OSlib.h"
#include "xf86_HWlib.h"
#include "mach64.h"
#ifdef PIXPRIV
#include "mach64im.h"
#endif

#define XCONFIG_FLAGS_ONLY
#include "xf86_Config.h"

extern int defaultColorVisualClass;
extern int mach64MaxClock;
extern int mach64MaxATI68860Clock;
extern Bool xf86Verbose, xf86Resetting, xf86Exiting, xf86ProbeFailed;
extern void mach64QueryBestSize();
extern void mach64WarpCursor();
extern void mach64RepositionCursor();
extern Bool miDCInitialize();
extern void SetTimeSinceLastInputEvent();
unsigned int mach64MemorySize = 0;
extern char *xf86VisualNames[];

#ifdef PIXPRIV
int mach64PixmapIndex;
#endif

static Bool mach64ValidMode(
#if NeedFunctionPrototypes
    DisplayModePtr
#endif
); 

ScrnInfoRec mach64InfoRec = {
    FALSE,		/* Bool configured */
    -1,			/* int tmpIndex */
    -1,			/* int scrnIndex */
    mach64Probe,      	/* Bool (* Probe)() */
    mach64Initialize,	/* Bool (* Init)() */
    mach64ValidMode,	/* Bool (* ValidMode)() */
    mach64EnterLeaveVT, /* void (* EnterLeaveVT)() */
    (void (*)())NoopDDA,/* void (* EnterLeaveMonitor)() */
    (void (*)())NoopDDA,/* void (* EnterLeaveCursor)() */
    mach64AdjustFrame,	/* void (* AdjustFrame)() */
    mach64SwitchMode,	/* Bool (* SwitchMode)() */
    mach64PrintIdent,	/* void (* PrintIdent)() */
    8,			/* int depth */
    {5, 6, 5},          /* xrgb weight */
    8,			/* int bitsPerPixel */
    PseudoColor,       	/* int defaultVisual */
    -1, -1,		/* int virtualX,virtualY */
    -1,                 /* int displayWidth */
    -1, -1, -1, -1,	/* int frameX0, frameY0, frameX1, frameY1 */
    {0, },	       	/* OFlagSet options */
    {0, },	       	/* OFlagSet clockOptions */
    {0, },	       	/* OFlagSet xconfigFlag */
    NULL,	       	/* char *chipset */
    NULL,               /* char *ramdac */
    0,                  /* int dacSpeed */
    0,			/* int clocks */
    {0, },		/* int clock[MAXCLOCKS] */
    0,			/* int maxClock */
    0,			/* int videoRam */
    0xC0000,            /* int BIOSbase */   
    0,			/* unsigned long MemBase */
    240, 180,		/* int width, height */
    0,                  /* unsigned long  speedup */
    NULL,	       	/* DisplayModePtr modes */
    NULL,               /* DisplayModePtr pModes */
    NULL,               /* char *clockprog */
    -1,                 /* int textclock */   
    FALSE,              /* Bool bankedMono */
    "Mach64",           /* char *name */
    {0, },		/* xrgb blackColour */
    {0, },		/* xrgb whiteColour */
    mach64ValidTokens,	/* int *validTokens */
    MACH64_PATCHLEVEL,	/* char *patchlevel */
    0,                  /* int IObase */
    0,                  /* int PALbase */
    0,                  /* int COPbase */
    0,                  /* int POSbase */
    0,                  /* int instance */
    0,                  /* int s3Madjust */
    0,                  /* int s3Nadjust */
    0,                  /* int s3MClk */
};

int mach64alu[16] = {
    MIX_0,
    MIX_AND,
    MIX_SRC_AND_NOT_DST,
    MIX_SRC,
    MIX_NOT_SRC_AND_DST,
    MIX_DST,
    MIX_XOR,
    MIX_OR,
    MIX_NOR,
    MIX_XNOR,
    MIX_NOT_DST,
    MIX_SRC_OR_NOT_DST,
    MIX_NOT_SRC,
    MIX_NOT_SRC_OR_DST,
    MIX_NAND,
    MIX_1
};

extern miPointerScreenFuncRec xf86PointerScreenFuncs;

LUTENTRY mach64savedLUT[256];
static Bool LUTissaved = FALSE;

int mach64MaxX, mach64MaxY;
int mach64VirtX, mach64VirtY;

Bool mach64Use4MbAperture = FALSE;
Bool mach64DAC8Bit = FALSE;

static unsigned Mach64_IOPorts[] = {
	/* VGA Registers */
        0x3B4, 0x3B5, 0x3BA, 0x3C0, 0x3C1, 0x3C2, 0x3C4, 0x3C5, 0x3C6, 0x3C7, 
	0x3C8, 0x3C9, 0x3CA, 0x3CB, 0x3CC, 0x3CE, 0x3CF, 0x3D4, 0x3D5, 0x3DA,
	/* ATI VGA Registers */
	ATIEXT, ATIEXT+1,
	/* Mach64 Registers */
	ioCONFIG_CNTL, ioCONFIG_CNTL+2,
	ioSCRATCH_REG0, ioSCRATCH_REG0+2,
	ioCONFIG_STAT0, ioCONFIG_STAT0+2,
	ioMEM_CNTL, ioMEM_CNTL+2,
};
static int Num_Mach64_IOPorts = (sizeof(Mach64_IOPorts)/
				 sizeof(Mach64_IOPorts[0]));

short mach64WeightMask;

static mach64CRTCRegRec mach64CRTCRegs;
static ScreenPtr savepScreen = NULL;
static PixmapPtr ppix = NULL;
static int mach64HWCursorSave = -1;


/*
 * ATI Hardware Probe
 *
 */

typedef struct ATIInformationBlock {
   char board_identifier[2];
   char equipment_flags[2];
   char asic_identifier;
   int  bios_major;
   int  bios_minor;
   char bios_date[21];
   int  VGA_Wonder_Present;
   int  Mach64_Present;
   int  Bus_Type;
   int  Mem_Size;
   int  Mem_Type;
   int  DAC_Type;
   int  DAC_SubType;
   int  Clock_Type;
   int  Clocks[MACH64_NUM_CLOCKS];
   mach64FreqRec Freq_Table[MACH64_NUM_FREQS];
   mach64FreqRec Freq_Table2[MACH64_NUM_FREQS];
#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
   int  MinFreq;
   int  MaxFreq;
   int  RefFreq;
   int  RefDivider;
   int  NAdj;
   int  CXClk;
#endif
} ATIInformationBlock;

int	mach64Ramdac;
char	*mach64ramdac_names[] = {
	"ATI-68830",
	"IMS-G173/SC1148[368]",
	"ATI-68875/TLC34075/Bt885",
	"Bt47[68]/INMOS17[68]",
	"AT&T20C49[01]/Bt48[12]/IMS-G174/MU9C{1880,4910}/SC1502[56]",
	"ATI-68860",
	"unknown",
	"unknown",
};

int	mach64RamdacSubType;
int	mach64BusType;
int	mach64MemType;
int	mach64ClockType;
int	mach64Clocks[MACH64_NUM_CLOCKS];
mach64FreqRec mach64FreqTable[MACH64_NUM_FREQS];
mach64FreqRec mach64FreqTable2[MACH64_NUM_FREQS];
#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
int	mach64MinFreq;
int	mach64MaxFreq;
int	mach64RefFreq;
int	mach64RefDivider;
int	mach64NAdj;
int	mach64CXClk;
#endif

static ATIInformationBlock *GetATIInformationBlock()
{
#define BIOS_DATA_SIZE 0x8000
   char                       signature[]    = " 761295520";
   char                       bios_data[BIOS_DATA_SIZE];
   char                       bios_signature[10];
   unsigned short             *sbios_data = (unsigned short *)bios_data;
   int                        tmp,i,j;
   static ATIInformationBlock info = { 0, };
   int                        ROM_Table_Offset;
   int                        Freq_Table_Ptr;
   int                        CDepth_Table_Ptr;
   int                        CTable_Size;

	 
   if (xf86ReadBIOS(mach64InfoRec.BIOSbase, 0x30,
		    (unsigned char *)bios_signature, 10) != 10) {
      return NULL;
   }
   if (strncmp( signature, bios_signature, 10 ))
	 return NULL;

   if (xf86ReadBIOS(mach64InfoRec.BIOSbase, 0x00,
		    (unsigned char *)bios_data, BIOS_DATA_SIZE)
       != BIOS_DATA_SIZE) {
      return NULL;
   }

   info.board_identifier[0]    = bios_data[ 0x40 ];
   info.board_identifier[1]    = bios_data[ 0x41 ];
   info.equipment_flags[0]     = bios_data[ 0x42 ];
   info.equipment_flags[1]     = bios_data[ 0x44 ];
   info.asic_identifier        = bios_data[ 0x43 ];
   info.bios_major             = bios_data[ 0x4c ];
   info.bios_minor             = bios_data[ 0x4d ];
   strncpy( info.bios_date, bios_data + 0x50, 20 );
   
   info.VGA_Wonder_Present     = bios_data[ 0x44 ] & 0x40;

   info.Mach64_Present = 1;	/* Test for Mach64 product */

   tmp = inl(ioSCRATCH_REG0);
   outl(ioSCRATCH_REG0, 0x55555555);
   if (inl(ioSCRATCH_REG0) != 0x55555555) {
      info.Mach64_Present = 0;
   } else {
      outl(ioSCRATCH_REG0, 0xaaaaaaaa);
      if (inl(ioSCRATCH_REG0) != 0xaaaaaaaa) {
	 info.Mach64_Present = 0;
      }
   }
   outl(ioSCRATCH_REG0, tmp);

   if (!info.Mach64_Present)
	 return &info;

   tmp = inl(ioCONFIG_STAT0);
   info.Bus_Type = tmp & CFG_BUS_TYPE;
   info.Mem_Type = (tmp & CFG_MEM_TYPE) >> 3;
   info.DAC_Type = (tmp & CFG_INIT_DAC_TYPE) >> 9;

   tmp = inl(ioMEM_CNTL);
   switch (tmp & MEM_SIZE_ALIAS) {
   case MEM_SIZE_512K:
       info.Mem_Size = 512;
       break;
   case MEM_SIZE_1M:
       info.Mem_Size = 1024;
       break;
   case MEM_SIZE_2M:
       info.Mem_Size = 2*1024;
       break;
   case MEM_SIZE_4M:
       info.Mem_Size = 4*1024;
       break;
   case MEM_SIZE_6M:
       info.Mem_Size = 6*1024;
       break;
   case MEM_SIZE_8M:
       info.Mem_Size = 8*1024;
       break;
   }

   info.DAC_SubType = inb(ioSCRATCH_REG1+1) & 0xf0 | info.DAC_Type;

   ROM_Table_Offset = sbios_data[0x48 >> 1];
   Freq_Table_Ptr = sbios_data[(ROM_Table_Offset >> 1) + 8];
   info.Clock_Type = bios_data[Freq_Table_Ptr];

#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
   info.MinFreq = sbios_data[(Freq_Table_Ptr >> 1) + 1];
   info.MaxFreq = sbios_data[(Freq_Table_Ptr >> 1) + 2];
   info.RefFreq = sbios_data[(Freq_Table_Ptr >> 1) + 4];
   info.RefDivider = sbios_data[(Freq_Table_Ptr >> 1) + 5];
   info.NAdj = sbios_data[(Freq_Table_Ptr >> 1) + 6];
   info.CXClk = bios_data[Freq_Table_Ptr + 6];
#endif

   CDepth_Table_Ptr = sbios_data[(Freq_Table_Ptr >> 1) - 3];
   Freq_Table_Ptr = sbios_data[(Freq_Table_Ptr >> 1) - 1];

   for (i = 0; i < MACH64_NUM_CLOCKS; i++)
      info.Clocks[i] = sbios_data[(Freq_Table_Ptr >> 1) + i];

   CTable_Size = bios_data[CDepth_Table_Ptr - 1];
   for (i = 0, j = 0;
        bios_data[CDepth_Table_Ptr + i] != 0;
        i += CTable_Size, j++) {
     info.Freq_Table[j].h_disp        = bios_data[CDepth_Table_Ptr + i];
     info.Freq_Table[j].dacmask       = bios_data[CDepth_Table_Ptr + i + 1];
     info.Freq_Table[j].ram_req       = bios_data[CDepth_Table_Ptr + i + 2];
     info.Freq_Table[j].max_dot_clock = bios_data[CDepth_Table_Ptr + i + 3];
     info.Freq_Table[j].color_depth   = bios_data[CDepth_Table_Ptr + i + 4];
   }
   info.Freq_Table[j].h_disp = 0;

   CDepth_Table_Ptr += i + 2;
   CTable_Size = bios_data[CDepth_Table_Ptr - 1];
   for (i = 0, j = 0;
        bios_data[CDepth_Table_Ptr + i] != 0;
        i += CTable_Size, j++) {
     info.Freq_Table2[j].h_disp        = bios_data[CDepth_Table_Ptr + i];
     info.Freq_Table2[j].dacmask       = bios_data[CDepth_Table_Ptr + i + 1];
     info.Freq_Table2[j].ram_req       = bios_data[CDepth_Table_Ptr + i + 2];
     info.Freq_Table2[j].max_dot_clock = bios_data[CDepth_Table_Ptr + i + 3];
     info.Freq_Table2[j].color_depth   = bios_data[CDepth_Table_Ptr + i + 4];
   }
   info.Freq_Table2[j].h_disp = 0;

   return &info;
}

/*
 * mach64Probe --
 *     Probe the hardware
 */
Bool
mach64Probe()
{
    int                   i, j;
    DisplayModePtr        pMode, pEnd;
    ATIInformationBlock   *info;
    int                   available_ram;
    Bool                  sw_cursor_supplied;
    OFlagSet              validOptions;
    int                   tx, ty;

    xf86ClearIOPortList(mach64InfoRec.scrnIndex);
    xf86AddIOPorts(mach64InfoRec.scrnIndex, Num_Mach64_IOPorts, Mach64_IOPorts);

    xf86EnableIOPorts(mach64InfoRec.scrnIndex);

    info = GetATIInformationBlock();
    if (!info || !info->Mach64_Present) {
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    if (xf86bpp < 0) {
        xf86bpp = mach64InfoRec.depth;
    }

    if (xf86weight.red == 0 ||
	xf86weight.green == 0 ||
	xf86weight.blue == 0) {
        xf86weight = mach64InfoRec.weight;
    }

    if (info->DAC_Type != DAC_ATI68860_ATI68880 && xf86bpp != 8) {
	ErrorF("mach64Probe: Invalid bpp: %d\n", xf86bpp);
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    switch (xf86bpp) {
    case 8:
        break;
    case 16:
	if (xf86weight.red == 5 &&
	    xf86weight.green == 5 &&
	    xf86weight.blue == 5) {
	    mach64WeightMask = RGB16_555;
            mach64InfoRec.depth = 15;
	} else if (xf86weight.red == 5 &&
		   xf86weight.green == 6 &&
		   xf86weight.blue == 5) {
	    mach64WeightMask = RGB16_565;
	    mach64InfoRec.depth = 16;
	} else {
	    ErrorF("Invalid color weighting %1d%1d%1d (only 555 and 565 are valid)\n",
		   xf86weight.red, xf86weight.green, xf86weight.blue);
	    xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	    return(FALSE);
	}
        mach64InfoRec.bitsPerPixel = 16;
	if (mach64InfoRec.defaultVisual < 0)
	    mach64InfoRec.defaultVisual = TrueColor;
        if (defaultColorVisualClass < 0)
	    defaultColorVisualClass = mach64InfoRec.defaultVisual;
        if (defaultColorVisualClass != TrueColor) {
            ErrorF("Invalid default visual type: %d (%s)\n",
		   defaultColorVisualClass,
                   xf86VisualNames[defaultColorVisualClass]);
	    xf86DisableIOPorts(mach64InfoRec.scrnIndex);
            return(FALSE);
        }
	break;
    case 32:
        mach64InfoRec.depth = 24;
        mach64InfoRec.bitsPerPixel = 32;
	xf86weight.red = xf86weight.green = xf86weight.blue = 8;
        if (mach64InfoRec.defaultVisual < 0)
            mach64InfoRec.defaultVisual = TrueColor;
        if (defaultColorVisualClass < 0)
            defaultColorVisualClass = mach64InfoRec.defaultVisual;
        if (defaultColorVisualClass != TrueColor) {
            ErrorF("Invalid default visual type: %d (%s)\n",
                   defaultColorVisualClass,
                   xf86VisualNames[defaultColorVisualClass]);
	    xf86DisableIOPorts(mach64InfoRec.scrnIndex);
            return(FALSE);
        }
        break;
    default:
	ErrorF("mach64Probe: Invalid bpp: %d\n", xf86bpp);
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    switch(info->DAC_Type) {
    case DAC_ATI68860_ATI68880:
	mach64InfoRec.maxClock = mach64MaxATI68860Clock;
	break;
    default:
	mach64InfoRec.maxClock = mach64MaxClock;
	break;
    }

    OFLG_ZERO(&validOptions);
    OFLG_SET(OPTION_SW_CURSOR, &validOptions);
    OFLG_SET(OPTION_CSYNC, &validOptions);
    OFLG_SET(OPTION_DAC_8_BIT, &validOptions);
    xf86VerifyOptions(&validOptions, &mach64InfoRec);
    
    mach64InfoRec.chipset = "mach64";
    xf86ProbeFailed = FALSE;

    mach64BusType = info->Bus_Type;
    if (xf86Verbose)
    {
	ErrorF("%s %s: card type: ", XCONFIG_PROBED, mach64InfoRec.name);
	switch(mach64BusType)
	{
	case ISA:
	    ErrorF("ISA\n");
	    break;
	case EISA:
	    ErrorF("EISA\n");
	    break;
	case LOCAL_BUS:
	    ErrorF("VESA LocalBus\n");
	    break;
	case PCI:
	    ErrorF("PCI\n");
	    break;
	default:
	    ErrorF("Unknown\n");
	}
    }

    mach64MemType = info->Mem_Type;
    mach64Ramdac = info->DAC_Type;
    mach64RamdacSubType = info->DAC_SubType;
#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
    mach64MinFreq = info->MinFreq;
    mach64MaxFreq = info->MaxFreq;
    mach64RefFreq = info->RefFreq;
    mach64RefDivider = info->RefDivider;
    mach64NAdj = info->NAdj;
    mach64CXClk = info->CXClk;
#endif

    mach64ClockType = info->Clock_Type;
    for (i = 0; i < MACH64_NUM_CLOCKS; i++)
	mach64Clocks[i] = info->Clocks[i];
    for (i = 0; i < MACH64_NUM_FREQS; i++)
	mach64FreqTable[i] = info->Freq_Table[i];
    for (i = 0; i < MACH64_NUM_FREQS; i++)
	mach64FreqTable2[i] = info->Freq_Table2[i];

    /* No need to use auto probing code since the BIOS contains all of
     * the clocks. */
    if (!mach64InfoRec.clocks) {
	for (i = 0; i < MACH64_NUM_CLOCKS; i++) {
	    mach64InfoRec.clock[i] = mach64Clocks[i] * 10;
#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
	    mach64InfoRec.clock[i + MACH64_NUM_CLOCKS] = mach64Clocks[i] * 5;
#endif
	}

#ifdef IMPLEMENTED_CLOCK_PROGRAMMING
	mach64InfoRec.clocks = MACH64_NUM_CLOCKS*2;
#else
	mach64InfoRec.clocks = MACH64_NUM_CLOCKS;
#endif
    }

    if (xf86Verbose) {
	ErrorF("%s ",OFLG_ISSET(XCONFIG_CLOCKS,&mach64InfoRec.xconfigFlag) ?
			XCONFIG_GIVEN : XCONFIG_PROBED);
	ErrorF("%s: ", mach64InfoRec.name);
	ErrorF("Number of Clocks: %d", mach64InfoRec.clocks);

	for (i = 0; i < mach64InfoRec.clocks; i++) {
	    if (i % 8 == 0) 
               ErrorF("\n%s %s: clocks:", 
                 OFLG_ISSET(XCONFIG_CLOCKS,&mach64InfoRec.xconfigFlag) ?
                 XCONFIG_GIVEN : XCONFIG_PROBED,
                 mach64InfoRec.name);
	    ErrorF(" %6.2f", mach64InfoRec.clock[i]/1000.0);
	}
	ErrorF("\n");
    }

    tx = mach64InfoRec.virtualX;
    ty = mach64InfoRec.virtualY;
    pMode = mach64InfoRec.modes;
    if (pMode == NULL) {
        ErrorF("No modes supplied in XF86Config\n");
        xf86DisableIOPorts(mach64InfoRec.scrnIndex);
        return(FALSE);
    }
    pEnd = (DisplayModePtr)NULL;
    do {
          DisplayModePtr pModeSv;
          /*
           * xf86LookupMode returns FALSE if it ran into an invalid
           * parameter
           */
          if(xf86LookupMode(pMode, &mach64InfoRec) == FALSE) {
                pModeSv=pMode->next;
                xf86DeleteMode(&mach64InfoRec, pMode);
                pMode = pModeSv; 
          } else if (((tx > 0) && (pMode->HDisplay > tx)) || 
                     ((ty > 0) && (pMode->VDisplay > ty))) {
                pModeSv=pMode->next;
                ErrorF("%s %s: Resolution %dx%d too large for virtual %dx%d\n",
                       XCONFIG_PROBED, mach64InfoRec.name,
                        pMode->HDisplay, pMode->VDisplay, tx, ty);
                xf86DeleteMode(&mach64InfoRec, pMode);
                pMode = pModeSv;
          } else {
	        int found = FALSE;
		int mach64CDepths[] = { -1, 4, 8, 15, 16, 24, 32, -1 };

	        for (i = 0; mach64FreqTable2[i].h_disp != 0; i++)
		    if ((mach64FreqTable2[i].h_disp << 3 >= pMode->HDisplay) &&
			(mach64FreqTable2[i].dacmask == mach64RamdacSubType) &&
			(mach64FreqTable2[i].max_dot_clock >=
			 mach64InfoRec.clock[pMode->Clock] / 1000) &&
			(mach64CDepths[mach64FreqTable2[i].color_depth & 0x07]
			 >= mach64InfoRec.depth)) {
			found = TRUE;
			break;
		    }

		if (!found)
		    for (i = 0; mach64FreqTable[i].h_disp != 0; i++)
			if ((mach64FreqTable[i].h_disp << 3 >= pMode->HDisplay) &&
			    (mach64FreqTable[i].dacmask & (1 << mach64Ramdac)) &&
			    (mach64FreqTable[i].max_dot_clock >=
			     mach64InfoRec.clock[pMode->Clock] / 1000) &&
			    (mach64CDepths[mach64FreqTable[i].color_depth & 0x07]
			     >= mach64InfoRec.depth)) {
			    found = TRUE;
			    break;
			}

		if (!found) {
		    pModeSv=pMode->next;
		    ErrorF("%s %s: Illegal mode according to Mach64 BIOS\n",
			   XCONFIG_PROBED, mach64InfoRec.name);
		    xf86DeleteMode(&mach64InfoRec, pMode);
		    pMode = pModeSv;
		} else {
		    /*
		     * Successfully looked up this mode.  If pEnd isn't 
		     * initialized, set it to this mode.
		     */
		    if(pEnd == (DisplayModePtr) NULL)
                        pEnd = pMode;
		    pMode = pMode->next;
		    mach64InfoRec.virtualX = max(mach64InfoRec.virtualX,
						 pMode->HDisplay);
		    mach64InfoRec.virtualY = max(mach64InfoRec.virtualY,
						 pMode->VDisplay);
		}
          }
    } while (pMode != pEnd);

    mach64VirtX = mach64InfoRec.virtualX = (mach64InfoRec.virtualX+7) & 0xfff8;
    mach64VirtY = mach64InfoRec.virtualY;

    if ((tx != mach64InfoRec.virtualX) || (ty != mach64InfoRec.virtualY))
	OFLG_CLR(XCONFIG_VIRTUAL,&mach64InfoRec.xconfigFlag);

    if (xf86Verbose) {
	ErrorF("%s %s: Virtual resolution: %dx%d\n",
	       OFLG_ISSET(XCONFIG_VIRTUAL, &mach64InfoRec.xconfigFlag) ?
		XCONFIG_GIVEN : XCONFIG_PROBED,
		mach64InfoRec.name, mach64VirtX, mach64VirtY);
    }

    if ((mach64VirtX) < 1024) {
	ErrorF("mach64 X server requires virtual screen width >= 1024\n");
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    if (!mach64InfoRec.videoRam) {
	mach64InfoRec.videoRam = info->Mem_Size;
    }

    /* Set mach64MemorySize to required MEM_SIZE value in MISC_OPTIONS */
    if (mach64InfoRec.videoRam <= 512)
	mach64MemorySize = MEM_SIZE_512K;
    else if (mach64InfoRec.videoRam <= 1024)
	mach64MemorySize = MEM_SIZE_1M;
    else if (mach64InfoRec.videoRam <= 2048)
	mach64MemorySize = MEM_SIZE_2M;
    else if (mach64InfoRec.videoRam <= 4096)
	mach64MemorySize = MEM_SIZE_4M;
    else if (mach64InfoRec.videoRam <= 6144)
	mach64MemorySize = MEM_SIZE_6M;
    else
	mach64MemorySize = MEM_SIZE_8M;
	
    if (xf86Verbose) {
       ErrorF("%s %s: videoram: %dk\n",
	      OFLG_ISSET(XCONFIG_VIDEORAM, &mach64InfoRec.xconfigFlag) ?
	      XCONFIG_GIVEN : XCONFIG_PROBED, mach64InfoRec.name,
	      mach64InfoRec.videoRam );
    }

    if (((mach64VirtX) * (mach64VirtY) * (mach64InfoRec.bitsPerPixel / 8)) > 
	(mach64InfoRec.videoRam*1024)) {
	ErrorF("Not enough memory for requested virtual resolution (%dx%d)\n",
	       mach64VirtX, mach64VirtY);
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    /* Set values of mach64MaxX and mach64MaxY.
     * This must be done here so that cache area and
     * scissor limits are set up correctly (this setup
     * happens before the pixmap cache is initialized).
     */

    mach64MaxX = mach64VirtX - 1;
    mach64MaxY = (mach64InfoRec.videoRam * 1024) /
                 (mach64VirtX * (mach64InfoRec.bitsPerPixel / 8)) - 1;

    /* Reserve space for the registers at the end of video memory */
    mach64InfoRec.videoRam--;

    available_ram = mach64InfoRec.videoRam * 1024;

    sw_cursor_supplied = OFLG_ISSET(OPTION_SW_CURSOR, &mach64InfoRec.options);

    if (!sw_cursor_supplied) 
    {
	if (available_ram - (mach64VirtX * mach64VirtY) >= MACH64_CURSBYTES)
	{
	    available_ram -= (MACH64_CURSBYTES + 1023) & ~1023;
	    mach64InfoRec.videoRam -= (MACH64_CURSBYTES + 1023) / 1024;
	}
	else
	{
	    OFLG_SET(OPTION_SW_CURSOR, &mach64InfoRec.options);
	    ErrorF("Warning: Not enough memory to use the hardware cursor.\n");
	    ErrorF("  Decreasing the virtual Y resolution by 1 will allow\n");
	    ErrorF("  you to use the hardware cursor.\n");
	}
    }
    ErrorF("%s %s: Using %s cursor\n", sw_cursor_supplied ? XCONFIG_GIVEN :
	   XCONFIG_PROBED, mach64InfoRec.name,
	   OFLG_ISSET(OPTION_SW_CURSOR, &mach64InfoRec.options) ?
	   "software" : "hardware");

    /* Assume we are not on an ISA bus since supporting one would require
     * a tremendous rewrite of this code.  The best solution is to make
     * another accel server (XF86_Mach64ISA) for ISA cards if there are
     * any masochists out there.
     */
    mach64Use4MbAperture = xf86LinearVidMem();
    if (!mach64Use4MbAperture) {
	ErrorF("The mach64 X server requires at least a 4Mb memory aperture.\n");
	xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	return(FALSE);
    }

    if (xf86Verbose) {
	if (mach64Use4MbAperture) {
	    if (mach64MemorySize <= MEM_SIZE_4M) {
	        ErrorF("%s %s: Using 4 MB aperture\n", XCONFIG_PROBED,
		       mach64InfoRec.name);
	    } else {
	        ErrorF("%s %s: Using 8 MB aperture\n", XCONFIG_PROBED,
		       mach64InfoRec.name);
	    }
	} else {
	    ErrorF("To use the Mach64 X server you need to be able to use\n");
	    ErrorF("a 4 or 8 Mb memory aperture.\n");
	    xf86DisableIOPorts(mach64InfoRec.scrnIndex);
	    return(FALSE);
	}
	ErrorF("%s %s: Ramdac is %s\n", XCONFIG_PROBED, mach64InfoRec.name,
	       mach64ramdac_names[mach64Ramdac]);
    }

    mach64DAC8Bit = (OFLG_ISSET(OPTION_DAC_8_BIT, &mach64InfoRec.options)
			&& (info->DAC_Type == DAC_ATI68860_ATI68880)
			&& (mach64InfoRec.bitsPerPixel == 8))
		    || (mach64InfoRec.bitsPerPixel == 16)
		    || (mach64InfoRec.bitsPerPixel == 32);

    if (xf86Verbose) {
	if (mach64InfoRec.bitsPerPixel == 8) {
	    ErrorF("%s %s: Using %d bits per RGB value\n",
	           (info->DAC_Type == DAC_ATI68860_ATI68880)
	           ? XCONFIG_GIVEN : XCONFIG_PROBED,
	           mach64InfoRec.name,
	           mach64DAC8Bit ?  8 : 6);
	} else if (mach64InfoRec.bitsPerPixel == 16) {
            ErrorF("%s %s: Color weight: %1d%1d%1d\n", XCONFIG_GIVEN,
		   mach64InfoRec.name, xf86weight.red,
		   xf86weight.green, xf86weight.blue);
	}
    }

    return(TRUE);
}


/*
 * mach64PrintIdent --
 *     Print the indentification of the video card.
 */
void
mach64PrintIdent()
{
    ErrorF("  %s: accelerated server for ATI Mach64 graphics adaptors ",
	   mach64InfoRec.name);
    ErrorF("(Patchlevel %s)\n", mach64InfoRec.patchLevel);
}


/*
 * mach64Initialize --
 *      Attempt to find and initialize a VGA framebuffer
 *      Most of the elements of the ScreenRec are filled in.  The
 *      video is enabled for the frame buffer...
 */

Bool
mach64Initialize (scr_index, pScreen, argc, argv)
    int            scr_index;    /* The index of pScreen in the ScreenInfo */
    ScreenPtr      pScreen;      /* The Screen to initialize */
    int            argc;         /* The number of the Server's arguments. */
    char           **argv;       /* The arguments themselves. Don't change! */
{
    int displayResolution = 75;  /* default to 75dpi */
    extern int monitorResolution;

    mach64InitGC();
    mach64InitDisplay(scr_index);
    mach64CalcCRTCRegs(&mach64CRTCRegs, mach64InfoRec.modes);
    mach64SetCRTCRegs(&mach64CRTCRegs);
    mach64InitEnvironment();

    /* Clear the display.
     * Need to set the color, origin, and size.  Then draw.
     */
    WaitQueue(4);
    regw(DP_FRGD_CLR, 1);
    regw(DST_Y_X, 0);
    regw(DST_CNTL, 0x03);
    regw(DST_HEIGHT_WIDTH, ((mach64VirtX << 16) | mach64VirtY));

    mach64CacheInit(mach64VirtX, mach64VirtY);
    mach64FontCache8Init(mach64VirtX, mach64VirtY);

    mach64ImageInit();

    WaitIdleEmpty(); /* Make sure that all commands have finished */

    /*
     * Take display resolution from the -dpi flag if specified
     */

    if (monitorResolution)
	displayResolution = monitorResolution;

    if (!mach64ScreenInit(pScreen, mach64VideoMem,
			  mach64VirtX, mach64VirtY,
			  displayResolution, displayResolution,
			  mach64VirtX))

		return(FALSE);

    savepScreen = pScreen;

    pScreen->CloseScreen = mach64CloseScreen;
    pScreen->SaveScreen = mach64SaveScreen;

    switch (mach64InfoRec.bitsPerPixel) {
    case 8:
        pScreen->InstallColormap = mach64InstallColormap;
        pScreen->UninstallColormap = mach64UninstallColormap;
        pScreen->ListInstalledColormaps = mach64ListInstalledColormaps;
        pScreen->StoreColors = mach64StoreColors;
        break;
    case 16:
    case 32:
        pScreen->InstallColormap = cfbInstallColormap;
        pScreen->UninstallColormap = cfbUninstallColormap;
        pScreen->ListInstalledColormaps = cfbListInstalledColormaps;
        pScreen->StoreColors = (void (*)())NoopDDA;
    }

    if (OFLG_ISSET(OPTION_SW_CURSOR, &mach64InfoRec.options)) {
	miDCInitialize (pScreen, &xf86PointerScreenFuncs);
    } else {
        pScreen->QueryBestSize = mach64QueryBestSize;
        xf86PointerScreenFuncs.WarpCursor = mach64WarpCursor;
        (void)mach64CursorInit(0, pScreen);
    }

#ifdef PIXPRIV
    mach64PixmapIndex = AllocatePixmapPrivateIndex();
    if (!AllocatePixmapPrivate(pScreen, mach64PixmapIndex,
			       sizeof(mach64PixPrivRec)))
	return FALSE;
#endif

    return (cfbCreateDefColormap(pScreen));
}


/*
 *      Assign a new serial number to the window.
 *      Used to force GC validation on VT switch.
 */

/*ARGSUSED*/
static int
mach64NewSerialNumber(pWin, data)
    WindowPtr pWin;
    pointer data;
{
    pWin->drawable.serialNumber = NEXT_SERIAL_NUMBER;
    return WT_WALKCHILDREN;
}


/*
 * mach64EnterLeaveVT -- 
 *      grab/ungrab the current VT completely.
 */

void
mach64EnterLeaveVT(enter, screen_idx)
     Bool enter;
     int screen_idx;
{
    PixmapPtr pspix;
    ScreenPtr pScreen = savepScreen;

    if (!xf86Exiting && !xf86Resetting) {
        switch (mach64InfoRec.bitsPerPixel) {
        case 8:
            pspix = (PixmapPtr)pScreen->devPrivate;
            break;
        case 16:
	    pspix = (PixmapPtr)pScreen->devPrivates[cfb16ScreenPrivateIndex].ptr;
            break;
        case 32:
	    pspix = (PixmapPtr)pScreen->devPrivates[cfb32ScreenPrivateIndex].ptr;
            break;
        }
    }

    if (pScreen)
        WalkTree(pScreen, mach64NewSerialNumber, 0);

    if (enter) {
	if (vgaBase)
	    xf86MapDisplay(screen_idx, VGA_REGION);
	if (mach64VideoMem != vgaBase)
	    xf86MapDisplay(screen_idx, LINEAR_REGION);
	if (!xf86Resetting) {
	    ScrnInfoPtr pScr = (ScrnInfoPtr)XF86SCRNINFO(pScreen);

	    mach64InitDisplay(screen_idx);
	    mach64SetCRTCRegs(&mach64CRTCRegs);
	    mach64InitEnvironment();

	    WaitQueue(4);
	    regw(DP_FRGD_CLR, 1);
	    regw(DST_Y_X, 0);
	    regw(DST_CNTL, 0x03);
	    regw(DST_HEIGHT_WIDTH, ((mach64VirtX << 16) | mach64VirtY));

	    mach64CacheInit(mach64VirtX, mach64VirtY);
	    mach64FontCache8Init(mach64VirtX, mach64VirtY);

	    mach64RestoreCursor(pScreen);
	    mach64AdjustFrame(pScr->frameX0, pScr->frameY0);

	    WaitIdleEmpty(); /* Make sure that all commands have finished */

	    if (LUTissaved) {
		mach64RestoreLUT(mach64savedLUT);
		LUTissaved = FALSE;
		mach64RestoreColor0(pScreen);
	    }

	    if (pspix->devPrivate.ptr != mach64VideoMem && ppix) {
		pspix->devPrivate.ptr = mach64VideoMem;
		(mach64ImageWriteFunc)(0, 0, pScreen->width, pScreen->height,
				 ppix->devPrivate.ptr,
				 PixmapBytePad(pScreen->width,
					       pScreen->rootDepth),
				 0, 0, MIX_SRC, ~0);
	    }
            if (pScreen) {
                pScreen->CopyWindow = mach64CopyWindow;
                pScreen->PaintWindowBackground = mach64PaintWindow;
                pScreen->PaintWindowBorder = mach64PaintWindow;
                switch (mach64InfoRec.bitsPerPixel) {
                case 8:
                    pScreen->GetSpans = cfbGetSpans;
		    break;
                case 16:
                    pScreen->GetSpans = cfb16GetSpans;
		    break;
                case 32:
                    pScreen->GetSpans = cfb32GetSpans;
		    break;
		}
            }
	}
	if (ppix) {
	    (pScreen->DestroyPixmap)(ppix);
	    ppix = NULL;
	}
    } else {
	if (vgaBase)
	    xf86MapDisplay(screen_idx, VGA_REGION);
	if (mach64VideoMem != vgaBase)
	    xf86MapDisplay(screen_idx, LINEAR_REGION);
	if (!xf86Exiting) {
	    ppix = (pScreen->CreatePixmap)(pScreen,
					   pScreen->width, pScreen->height,
					   pScreen->rootDepth);

	    if (ppix) {
		(mach64ImageReadFunc)(0, 0, pScreen->width, pScreen->height,
				ppix->devPrivate.ptr,
				PixmapBytePad(pScreen->width,
					      pScreen->rootDepth),
				0, 0, ~0);
		pspix->devPrivate.ptr = ppix->devPrivate.ptr;
	    }
            switch (mach64InfoRec.bitsPerPixel) {
            case 8:
                pScreen->CopyWindow = cfbCopyWindow;
                pScreen->GetSpans = cfbGetSpans;
                pScreen->PaintWindowBackground = cfbPaintWindow;
                pScreen->PaintWindowBorder = cfbPaintWindow;
                break;
            case 16:
                pScreen->CopyWindow = cfb16CopyWindow;
                pScreen->GetSpans = cfb16GetSpans;
                pScreen->PaintWindowBackground = cfb16PaintWindow;
                pScreen->PaintWindowBorder = cfb16PaintWindow;
                break;
            case 32:
                pScreen->CopyWindow = cfb32CopyWindow;
                pScreen->GetSpans = cfb32GetSpans;
                pScreen->PaintWindowBackground = cfb32PaintWindow;
                pScreen->PaintWindowBorder = cfb32PaintWindow;
                break;
            }
	}

	mach64CursorOff();
	mach64SaveLUT(mach64savedLUT);
	LUTissaved = TRUE;
	if (!xf86Resetting) {
	    mach64CleanUp();
	}
	if (vgaBase)
	    xf86UnMapDisplay(screen_idx, VGA_REGION);
	if (mach64VideoMem != vgaBase)
	    xf86UnMapDisplay(screen_idx, LINEAR_REGION);
    }
}

/*
 * mach64CloseScreen --
 *      called to ensure video is enabled when server exits.
 */

/*ARGSUSED*/
Bool
mach64CloseScreen(screen_idx, pScreen)
    int        screen_idx;
    ScreenPtr  pScreen;
{
    extern void mach64ClearSavedCursor();

    /*
     * Hmm... The server may shut down even if it is not running on the
     * current vt. Let's catch this case here.
     */
    xf86Exiting = TRUE;
    if (xf86VTSema)
	mach64EnterLeaveVT(LEAVE, screen_idx);
    else if (ppix) {
    /* 7-Jan-94 CEG: The server is not running on the current vt.
     * Free the screen snapshot taken when the server vt was left.
     */
	    (savepScreen->DestroyPixmap)(ppix);
	    ppix = NULL;
    }
    mach64ClearSavedCursor(screen_idx);

    switch (mach64InfoRec.bitsPerPixel) {
    case 8:
        cfbCloseScreen(screen_idx, savepScreen);
	break;
    case 16:
        cfb16CloseScreen(screen_idx, savepScreen);
	break;
    case 32:
        cfb32CloseScreen(screen_idx, savepScreen);
	break;
    }

    savepScreen = NULL;
    return(TRUE);
}

/*
 * mach64SaveScreen --
 *      blank the screen.
 */
Bool
mach64SaveScreen (pScreen, on)
     ScreenPtr     pScreen;
     Bool          on;
{
    if (on)
	SetTimeSinceLastInputEvent();

    if (xf86VTSema) {
	if (on) {
	    if (mach64HWCursorSave != -1) {
		mach64SetRamdac(mach64CRTCRegs.color_depth >> 8, TRUE);
		regwb(GEN_TEST_CNTL, mach64HWCursorSave);
		mach64HWCursorSave = -1;
	    }
	    mach64RestoreColor0(pScreen);
	    outb(ioDAC_REGS+2, 0xff);
	} else {
	    outb(ioDAC_REGS, 0);
	    outb(ioDAC_REGS+1, 0);
	    outb(ioDAC_REGS+1, 0);
	    outb(ioDAC_REGS+1, 0);
	    outb(ioDAC_REGS+2, 0x00);

	    mach64SetRamdac(CRTC_PIX_WIDTH_8BPP >> 8, TRUE);
	    mach64HWCursorSave = regrb(GEN_TEST_CNTL);
	    regwb(GEN_TEST_CNTL, mach64HWCursorSave & ~HWCURSOR_ENABLE);
	}
    }

    return(TRUE);
}

/*
 * mach64AdjustFrame --
 *      Modify the CRT_OFFSET for panning the display.
 */
void
mach64AdjustFrame(x, y)
    int x, y;
{
    int byte_offset = ((x + y*mach64VirtX) *
                        (mach64InfoRec.bitsPerPixel / 8)) >> 3;
    mach64CursorOff();

    regw(CRTC_OFF_PITCH, (regr(CRTC_OFF_PITCH) & 0xfff00000) | byte_offset);

    mach64RepositionCursor(savepScreen);
}

/*
 * mach64SwitchMode --
 *      Reinitialize the CRTC registers for the new mode.
 */
Bool
mach64SwitchMode(mode)
    DisplayModePtr mode;
{
    mach64CalcCRTCRegs(&mach64CRTCRegs, mode);
    mach64SetCRTCRegs(&mach64CRTCRegs);

    return(TRUE);
}

/*
 * mach64ValidMode --
 *
 */
static Bool
mach64ValidMode(mode)
DisplayModePtr mode;
{
return TRUE;
}

