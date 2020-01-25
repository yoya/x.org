/* $XConsortium: oak_driver.c,v 1.6 95/01/23 15:35:17 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga256/drivers/oak/oak_driver.c,v 3.12 1995/01/21 07:18:44 dawes Exp $ */
/*
 * Copyright 1994 by Jorge Delgado <ernar@dit.upm.es>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Jorge Delgado not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission. Jorge Delgado makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * JORGE DELGADO DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL JORGE DELGADO BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
 /*************************************************************************/

/*
 * This is a oak SVGA driver for XFree86.  
 *
 *  Built from Xfree86 1.3 stub file.
 *  9/ 1/93 Initial version Steve Goldman  sgoldman@encore.com
 * 10/ 9/94 Revamped 067/077 version with added support for 
 *          087 chipset by Jorge Delgado  ernar@dit.upm.es  
 * 24/10/94 New version by Jorge Delgado with support for
 *          2MB of DRAM in 087 chipsets with extended bank
 *          switching, and ported to X11R6 structure, adding ALPHA linear  
 * 24/11/94 At last I managed to fix the linear mode that did not
 *          work in the previous release, also fixed a typo which
 *          made impossible to hardcode "oti077" as chipset.
 * 23/12/94 Okay, now linear addressing is cleaner, and more
 *          efficient, Membase has been set at 0xE00000 (14Mb) and
 *          aperture is selected matching video memory.
 * 18/ 1/95 Yabadabadoo!!! I found an undocumented way of setting
 *          linear addressing above 16Mbytes for OTI087 VLB cards, activating
 *          the 64Mbyte pin of my VLB card. Thanks must be given to
 *          the people that wrote the README.cirrus, it gave me the clue.
 *          okay for all you there..voila!! >16Mb linear addressing.
 *
 * This one file can be used for both the color and monochrome servers.
 * Remember that the monochrome server is actually using a 16-color mode,
 * with only one bitplane active.  To distinguish between the two at
 * compile-time, use '#ifdef MONOVGA', etc. (But there is NO support for
 * monochrome in the 087, use the 077 driver harcoding chipset oti077
 * and VideoRam 1024 in XF86Config)
 */

/*************************************************************************/

/*
 * These are X and server generic header files.
 */
#include "X.h"
#include "input.h"
#include "screenint.h"
#include "dix.h"

/*
 * These are XFree86-specific header files
 */

#include "compiler.h"
#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"
#include "xf86_HWlib.h"
#include "vga.h"

/*
 * If the driver makes use of XF86Config 'Option' flags, the following will be
 * required
 */
#include "xf86_Config.h"

/*
 * This header is required for drivers that implement STUBFbInit().
 *
 * #if !defined(MONOVGA) && !defined(XF86VGA16)
 * #include "vga256.h"
 * #endif
 */ 
  
/*
 * Driver data structures.
 */
typedef struct {
  /*
   * This structure defines all of the register-level information
   * that must be stored to define a video mode for this chipset.
   * The 'vgaHWRec' member must be first, and contains all of the
   * standard VGA register information, as well as saved text and
   * font data.
   */
  vgaHWRec std;               /* good old IBM VGA */
  /* 
   * Any other registers or other data that the new chipset needs
   * to be saved should be defined here.  The Init/Save/Restore
   * functions will manipulate theses fields.  Examples of things
   * that would go here are registers that contain bank select
   * registers, or extended clock select bits, or extensions to 
   * the timing registers.  Use 'unsigned char' as the type for
   * these registers.
   */
  
  /* These first are for the 067/077 boards */
  
  unsigned char oakMisc;    	/* Misc register */
  unsigned char oakOverflow;    	/* overflow register */
  unsigned char oakHsync2;    	/* Hsync/2 start register */
  unsigned char oakOverflow2;    	/* overflow2 register */
  unsigned char oakConfig;    	/* config 67 vs. 77 */
  unsigned char oakBCompat;    	/* backward compatibility */
  unsigned char oakBank;    	/* initial bank, debug only */
  
  /* These are for the 087 ;) */
  unsigned char oti87RBank             ;
  unsigned char oti87WBank             ;
  unsigned char oti87VLBControl        ;
  unsigned char oti87Mapping           ;
  unsigned char oti87Clock             ; 
  unsigned char oti87BusControl        ;
  unsigned char oti87Hsync2            ; 
  unsigned char oti87XCrt              ;
  unsigned char oti87ColorPalette      ;
  unsigned char oti87Overflow          ;
  unsigned char oti87FIFODepth         ;
  unsigned char oti87ModeSelect        ; 
  unsigned char oti87Feature           ;
  unsigned char oti87ColorExpansion    ;
  unsigned char oti87FGExpansion       ;
  unsigned char oti87BGExpansion       ;
  unsigned char oti87ColorPattern      ;
  unsigned char oti87PixMask           ;
  unsigned char oti87HC1               ;
  unsigned char oti87HC2               ;
  unsigned char oti87HC3               ;
  unsigned char oti87HC4               ;
  unsigned char oti87HC5               ;
  unsigned char oti87HC6               ;
  unsigned char oti87HC7               ;
  unsigned char oti87HC8               ;
  unsigned char oti87HC9               ;
  unsigned char oti87HC10              ;
  unsigned char oti87HC11              ;
  unsigned char oti87HC12              ;
  
  
} vgaOAKRec, *vgaOAKPtr;

/*
 * Forward definitions for the functions that make up the driver.  See
 * the definitions of these functions for the real scoop.
 */
static Bool     OAKProbe();
static char *   OAKIdent();
static Bool     OAKClockSelect();
static void     OAKEnterLeave();
static Bool     OAKInit();
static Bool     OAKValidMode();
static void *   OAKSave();
static void     OAKRestore();
static void     OAKAdjust();

/*
 * These are the bank select functions.  There are defined in oak_bank.s
 */

extern void     OAKSetRead();
extern void     OAKSetWrite();
extern void     OAKSetReadWrite();

extern void     OTI87SetRead();
extern void     OTI87SetWrite();
extern void     OTI87SetReadWrite();

/*
 * This data structure defines the driver itself.  The data structure is
 * initialized with the functions that make up the driver and some data 
 * that defines how the driver operates.
 */
vgaVideoChipRec OAK = {
  /* 
   * Function pointers
   */
  OAKProbe,
  OAKIdent,
  OAKEnterLeave,
  OAKInit,
  OAKValidMode,
  OAKSave,
  OAKRestore,
  OAKAdjust,
  (void (*)())NoopDDA, /*  OAKSaveScreen,  */
  (void (*)())NoopDDA, /* OAKGetMode, */
  (void (*)())NoopDDA, /* OAKFbInit, */
  OAKSetRead,
  OAKSetWrite,
  OAKSetReadWrite,
  /*
   * This is the size of the mapped memory window, usually 64k.
   */
  0x10000,		
  /*
   * This is the size of a video memory bank for this chipset.
   */
  0x10000,
  /*
   * This is the number of bits by which an address is shifted
   * right to determine the bank number for that address.
   */
  16,
  /*
   * This is the bitmask used to determine the address within a
   * specific bank.
   */
  0xFFFF,
  /*
   * These are the bottom and top addresses for reads inside a
   * given bank.
   */
  0x00000, 0x10000,
  /*
   * And corresponding limits for writes.
   */
  0x00000, 0x10000,
  /*
   * Whether this chipset supports a single bank register or
   * separate read and write bank registers.  Almost all chipsets
   * support two banks, and two banks are almost always faster
   * (Trident 8900C and 9000 are odd exceptions).
   */
  TRUE, /* two banks */
  /*
   * If the chipset requires vertical timing numbers to be divided
   * by two for interlaced modes, set this to VGA_DIVIDE_VERT.
   */
  VGA_NO_DIVIDE_VERT,
  /*
   * This is a dummy initialization for the set of vendor/option flags
   * that this driver supports.  It gets filled in properly in the
   * probe function, if the probe succeeds (assuming the driver
					    * supports any such flags).
					    */
  {0,},
  /*
   * This specifies how the virtual width is to be rounded.  The
   * virtual width will be rounded down the nearest multiple of
   * this value
   */
  16,
  /*
   * If the driver includes support for a linear-mapped frame buffer
   * this should be set to TRUE.  In most cases it should be FALSE.
   */
  FALSE,
  /*
   * This is the physical base address of the linear-mapped frame
   * buffer (when used).  Set it to 0 when not in use.
   */
  0,
  /*
   * This is the size  of the linear-mapped frame buffer (when used).
   * Set it to 0 when not in use.
   */
  0,
  /*
   * This is TRUE if the driver has support for 16bpp
   */
  FALSE,
  /*
   * This is TRUE if the driver has support for 32bpp
   */
  FALSE,
  /*
   * This is a pointer to a list of builtin driver modes.
   * This is rarely used, and in must cases, set it to NULL
   */
  NULL,
  /*
   * This is a factor that can be used to scale the raw clocks
   * to pixel clocks.  This is rarely used, and in most cases, set
   * it to 1.
   */
  1,
};

/*
 * This is a convenience macro, so that entries in the driver structure
 * can simply be dereferenced with 'new->xxx'.
 */
#define new ((vgaOAKPtr)vgaNewVideoState)

/*
   A bunch of defines that match Oak's register names
   so we don't use a bunch of hardcoded constants in the code. (for 067/077)
   */
#define OTI_INDEX 0x3DE			/* Oak extended index register */
#define OTI_R_W 0x3DF			/* Oak extended r/w register */
#define OTI_CRT_CNTL 0xC		/* Oak CRT COntrol Register */
#define OTI_MISC  0xD			/* Oak Misc register */
#define OTI_BCOMPAT  0xE		/* Oak Back compat register */
#define OTI_SEGMENT  0x11		/* Oak segment register */
#define OTI_CONFIG  0x12		/* Oak config register */
#define OTI_OVERFLOW  0x14		/* Oak overflow register */
#define OTI_HSYNC2  0x15		/* Oak hsync/2 start register */
#define OTI_OVERFLOW2  0x16		/* Oak overflow2 register */


/* Now the ones for 087, being a much higher performance board, has more :) */
/* These defines I put now are to avoid using magic numbers harcoded into
 * the server, as I prefer using register names, this way when a poor guy
 * wants to code something for the otixx7 (oti097 for example) he will have
 * it easy to trace my code ;). (Poor him, I even use gotos )
 */


/* The following are the indexes to be loaded on OTI_INDEX, for the
 * write to OTI_R_W reach the selected register, an r preceding the name
 * means the register is read-only, r/w means a nice read-write thingy 
 */

#define OTI87_IDENTIFY            0x0     /* r Identify register, should not be
					   *    used to identify the board   
					   */
#define OTI87_STATUS              0x2     /* r/w Identifies CPUtype & VG-RAM */
#define OTI87_TEST                0x3     /* r/w DON'T USE */
#define OTI87_VLB_CONTROL         0x4     /* r/w Sets VLB's wait states */ 
#define OTI87_MEM_MAPPING         0x5     /* r/w Enables and controls the 
					   *     linear framebuffer 
					   */ 

#define OTI87_CLOCK               0x6      /* r/w Selects from 16 clocks */
#define OTI87_CONFIG_1            0x7      /* r  DON'T USE */ 
#define OTI87_CONFIG_2            0x8      /* r  DON'T USE */ 
#define OTI87_MISC_COMPAT         0xD      /* r/w Is for compatibility with
					    *     67/77 boards, DON'T USE 
					    */
#define OTI87_BUS_CTRL           0x13      /* r/w Controls the AT-BUS */
#define OTI87_OVERFLOW           0x14
#define OTI87_HSYNC              0x15      
#define OTI87_EXT_CRTC_CTRL      0x17      
#define OTI87_PALETTE_RANGE      0x19
#define OTI87_FIFO_DEPTH         0x20     /* r/w Controls the command execution
					   *     FIFO depth 
					   */
#define OTI87_MODE               0x21     /* r/w Enables HiColor, Packed pixel
					   *     etc... 
					   */ 
#define OTI87_FEATURE_CTRL       0x22      /* r/w Accel. functions enabling */
#define OTI87_XREAD              0x23      /* r/w Extended Read Bank  */
#define OTI87_XWRITE             0x24      /* r/w Extended Write Bank */
#define OTI87_XREADWRITE         0x25      /* r/w Extended R/W Bank */

/* The next registers control the color expansion routines */

#define OTI87_COLOR_EXPANDER     0x30      
#define OTI87_FORE               0x31
#define OTI87_BACK               0x32
#define OTI87_PATTERN            0x33
#define OTI87_MASK               0x34

/* End of color expanding feature registers */

/* Latch control */

#define OTI87_LATCH_INDEX        0x35
#define OTI87_LATCH_DATA         0x36

/* The next registers control the Hardware Cursor  */

#define OTI87_HC_HORIZONTAL_H    0x40
#define OTI87_HC_HORIZONTAL_L    0x41
#define OTI87_HC_VERTICAL_H      0x42
#define OTI87_HC_VERTICAL_L      0x43
#define OTI87_HC_HOR_PRESET      0x44
#define OTI87_HC_VER_PRESET      0x45
#define OTI87_HC_START_HL        0x47
#define OTI87_HC_START_LH        0x48
#define OTI87_HC_START_LL        0x49
#define OTI87_HC_BACK            0x4A
#define OTI87_HC_FORE            0x4B
#define OTI87_HC_CTRL            0x4C


#define OTI67 0   /* same index as ident function */
#define OTI77 1   /* same index as ident function */
#define OTI87 2   /* same index as ident function */


static int OTI_chipset;
static Bool OTI_2mb_bank = FALSE;
static Bool OTI_linear = FALSE;
static Bool OTI_vlb = FALSE;

static unsigned OAK_ExtPorts[] = { OTI_INDEX, OTI_R_W };
static int Num_OAK_ExtPorts = (sizeof(OAK_ExtPorts)/sizeof(OAK_ExtPorts[0]));

/*
 * OAKIdent --
 *
 * Returns the string name for supported chipset 'n'.  Most drivers only
 * support one chipset, but multiple version may require that the driver
 * identify them individually (e.g. the Trident driver).  The Ident function
 * should return a string if 'n' is valid, or NULL otherwise.  The
 * server will call this function when listing supported chipsets, with 'n' 
 * incrementing from 0, until the function returns NULL.  The 'Probe'
 * function should call this function to get the string name for a chipset
 * and when comparing against an Xconfig-supplied chipset value.  This
 * cuts down on the number of places errors can creep in.
 */
static char *
OAKIdent(n)
     int n;
{
  static char *chipsets[] = {"oti067","oti077","oti087"};
  
  if (n + 1 > sizeof(chipsets) / sizeof(char *))
    return(NULL);
  else
    return(chipsets[n]);
}

/*
 * OAKClockSelect --
 * 
 * This function selects the dot-clock with index 'no'.  In most cases
 * this is done my setting the correct bits in various registers (generic
 * VGA uses two bits in the Miscellaneous Output Register to select from
 * 4 clocks).  Care must be taken to protect any other bits in these
 * registers by fetching their values and masking off the other bits.
 */
static Bool
OAKClockSelect(no)
     int no;
{
  static unsigned char save1,save2;
  unsigned char temp;
  
  switch(no)
    {
    case CLK_REG_SAVE:
      
      /* We only use the Extended clock register for the OTI87 */
      
      if (OTI_chipset == OTI87)
	{
	  outb(OTI_INDEX, OTI87_CLOCK); 
	  save1 = inb(OTI_R_W);
	  break;
	}      
      else 
	{
	  /*
	   * Here all of the registers that can be affected by
	   * clock setting should be saved into static variables.
	   */
	  save1 = inb(0x3CC);
	  /* Any extended registers would go here */
	  outb(OTI_INDEX, OTI_MISC);
	  save2 = inb(OTI_R_W);
	  break;
	}
    case CLK_REG_RESTORE:
      /*
       * Here all the previously saved registers are restored.
       */
      if (OTI_chipset==OTI87) 
	{
	  outb(OTI_INDEX, OTI87_CLOCK);
	  outb(OTI_R_W,save1);
	  break;
	}
      else 
	{
	  outb(0x3C2, save1);
	  /* Any extended registers would go here */
	  outw(OTI_INDEX, OTI_MISC | (save2 << 8));
	  break;
	}
    default:
      /* The new oti087 chipset has an extended register to take care
       * of all clock bits, thats why we only modify extended registers
       */      
      if (OTI_chipset==OTI87) 
	{
	  outb(OTI_INDEX, OTI87_CLOCK);
	  outb(OTI_R_W,no);
	}
      else 
	{
	  /* 
	   * These are the generic two low-order bits of the clock select 
	   */
	  temp = inb(0x3CC);
	  outb(0x3C2, ( temp & 0xF3) | ((no << 2) & 0x0C));
	  /* 
	   * Here is where the high order bit(s) supported by the chipset 
	   * are set.  This is done by fetching the appropriate register,
	   * masking off bits that will be changing, then shifting and
	   * masking 'no' to set the bits as appropriate.
	   */
	  outb(OTI_INDEX, OTI_MISC);
	  temp = inb(OTI_R_W);
	  outw(OTI_INDEX, OTI_MISC | 
	       ((( temp & 0xDF ) | (( no & 4) << 3)) << 8));
	}
    }
}


/*
 * OAKProbe --
 *
 * This is the function that makes a yes/no decision about whether or not
 * a chipset supported by this driver is present or not.  The server will
 * call each driver's probe function in sequence, until one returns TRUE
 * or they all fail.
 *
 * Pretty much any mechanism can be used to determine the presence of the
 * chipset.  If there is a BIOS signature (e.g. ATI, GVGA), it can be read
 * via /dev/mem on most OSs, but some OSs (e.g. Mach) require special
 * handling, and others (e.g. Amoeba) don't allow reading  the BIOS at
 * all.  Hence, this mechanism is discouraged, if other mechanisms can be
 * found.  If the BIOS-reading mechanism must be used, examine the ATI and
 * GVGA drivers for the special code that is needed.  Note that the BIOS 
 * base should not be assumed to be at 0xC0000 (although most are).  Use
 * 'vga256InfoRec.BIOSbase', which will pick up any changes the user may
 * have specified in the Xconfig file.
 *
 * The preferred mechanism for doing this is via register identification.
 * It is important not only the chipset is detected, but also to
 * ensure that other chipsets will not be falsely detected by the probe
 * (this is difficult, but something that the developer should strive for).  
 * For testing registers, there are a set of utility functions in the 
 * "compiler.h" header file.  A good place to find example probing code is
 * in the SuperProbe program, which uses algorithms from the "vgadoc2.zip"
 * package (available on most PC/vga FTP mirror sites, like ftp.uu.net and
 * wuarchive.wustl.edu).
 *
 * Once the chipset has been successfully detected, then the developer needs 
 * to do some other work to find memory, and clocks, etc, and do any other
 * driver-level data-structure initialization may need to be done.
 */
static Bool
OAKProbe()
{
  unsigned char save, temp1, temp, temp2;
  
  xf86ClearIOPortList(vga256InfoRec.scrnIndex);
  xf86AddIOPorts(vga256InfoRec.scrnIndex, Num_VGA_IOPorts, VGA_IOPorts);
  xf86AddIOPorts(vga256InfoRec.scrnIndex, Num_OAK_ExtPorts, OAK_ExtPorts);
  
  /*
   * First we attempt to figure out if one of the supported chipsets
   * is present.
   */
  if (vga256InfoRec.chipset)
    {
      /*
       * This is the easy case.  The user has specified the
       * chipset in the XF86Config file.  All we need to do here
       * is a string comparison against each of the supported
       * names available from the Ident() function.  If this
       * driver supports more than one chipset, there would be
       * nested conditionals here (see the Trident and WD drivers
       * for examples).
       */

      if (!StrCaseCmp(vga256InfoRec.chipset, OAKIdent(0))) {
	OTI_chipset = OTI67;
      } else if (!StrCaseCmp(vga256InfoRec.chipset, OAKIdent(1))) {
	OTI_chipset = OTI77;
      } else if (!StrCaseCmp(vga256InfoRec.chipset, OAKIdent(2))) {
	OTI_chipset = OTI87;
      } 
      else 
	{
	  return (FALSE); 
	}
      OAKEnterLeave(ENTER);
    }
  else
    {
      /*
       * OK.  We have to actually test the hardware.  The
       * EnterLeave() function (described below) unlocks access
       * to registers that may be locked, and for OSs that require
       * it, enables I/O access.  So we do this before we probe,
       * even though we don't know for sure that this chipset
       * is present.
       */
      OAKEnterLeave(ENTER);
      
      /*
       * Here is where all of the probing code should be placed.  
       * The best advice is to look at what the other drivers are 
       * doing.  If you are lucky, the chipset reference will tell 
       * how to do this.  Other resources include SuperProbe/vgadoc2,
       * and the Ferraro book.
       */
      
      /* First we see if the segment register is present */

      outb(OTI_INDEX, OTI_SEGMENT);
      save = inb(OTI_R_W);

      /* I assume that once I set the index I can r/w/r/w to
	 my hearts content */

      outb(OTI_R_W, save ^ 0x11);
      temp1 = inb(OTI_R_W);
      outb(OTI_R_W, save);
      if (temp1 != ( save ^ 0x11 )) {

	/*
	 * Turn things back off if the probe is going to fail.
	 * Returning FALSE implies failure, and the server
	 * will go on to the next driver.
	 */

	OAKEnterLeave(LEAVE);
	return(FALSE);
      }
      /* figure out which chipset */
      
      temp1 = inb(OTI_INDEX);
      temp1 &= 0xE0;
      switch (temp1) {
      case 0xE0 : /* oti 57 don't know it */
	ErrorF("%s %s: oak: OTI-57 unsupported.\n", XCONFIG_PROBED,
	       vga256InfoRec.name);
	OAKEnterLeave(LEAVE);
	return(FALSE);
      case 0x40 : /* oti 67 */
	OTI_chipset = OTI67;
	break;
      case 0xA0 : /* oti 77 */
	OTI_chipset = OTI77;
	break;
      default : /* don't know it by these bits */
	outb(OTI_INDEX, OTI87_IDENTIFY );
	if (inb(OTI_R_W) != 1 ) {
	  ErrorF("%s %s: oak: unknown chipset.\n", XCONFIG_PROBED,
		 vga256InfoRec.name);
	  OAKEnterLeave(LEAVE);
	  return(FALSE);
	}
	else 
	  { /* oti087 */
	    OTI_chipset = OTI87;
	  }
      }
    }
  
  /*
   * If the user has specified the amount of memory in the XF86Config
   * file, we respect that setting.
   */
  if (!vga256InfoRec.videoRam) {
    /*
     * Otherwise, do whatever chipset-specific things are 
     * necessary to figure out how much memory (in kBytes) is 
     * available.
     */
    
    if (OTI_chipset == OTI87) {
      outb(OTI_INDEX, OTI87_STATUS);
      temp = ( inb(OTI_R_W) & 0x6);
      switch (temp) 
	{
	case 0:
	  vga256InfoRec.videoRam = 256;
	  break;
	case 2:
	  vga256InfoRec.videoRam = 512;
	  break;        
	case 4:
	  vga256InfoRec.videoRam = 1024;
	  break;
	case 6:
	  vga256InfoRec.videoRam = 2048;
	  break;
	default:
	  ErrorF("%s %s: oti087: unknown video memory\n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  OAKEnterLeave(LEAVE);
	  return(FALSE);
	}
    }
    else {
      outb(OTI_INDEX, OTI_MISC);
      temp1 = inb(OTI_R_W);
      temp1 &= 0xC0;
      if (temp1 == 0xC0 )
	vga256InfoRec.videoRam = 1024;
      else if (temp1 == 0x80 )
	vga256InfoRec.videoRam = 512;
      else if (temp1 == 0x00 )
	vga256InfoRec.videoRam = 256;
      else {
	ErrorF("%s %s: oak: unknown video memory.\n",
	       XCONFIG_PROBED, vga256InfoRec.name);
	OAKEnterLeave(LEAVE);
	return(FALSE);
      }
    }
  }

    if (OTI_chipset == OTI87) {
            
      /* Now we perform other detections, such as RAMDAC and
       * the like.
       */
      
      outb(OTI_INDEX,OTI87_CONFIG_1);
      temp2 = ( inb(OTI_R_W) & 0x06);
      switch ( temp2 )
	{
	case 0x00 :
	  ErrorF("%s %s: oti087: VLB card integrated in MotherBoard. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: MemBase for Linear Addressing will be\n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: set at 0x4E00000 (the 78Mbyte mark). \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  OTI_vlb = TRUE;
	  break;
	case 0x02 :
	  ErrorF("%s %s: oti087: VLB card in a 32-bit VESA slot. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: MemBase for Linear Addressing will be. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: set at 0x4E00000 (the 78Mbyte mark). \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  OTI_vlb = TRUE;
	  break;
	case 0x04 :
	  ErrorF("%s %s: oti087: ISA card integrated in MotherBoard. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: MemBase for Linear Addressing will be. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: set at 0xE00000 (the 14Mbyte mark). \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  OTI_vlb = FALSE;
	  break;
	case 0x06 :
	  ErrorF("%s %s: oti087: ISA card in a 16-bit AT-BUS slot.  \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: MemBase for Linear Addressing will be. \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  ErrorF("%s %s: oti087: set at 0xE00000 (the 14Mbyte mark). \n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  OTI_vlb = FALSE;
	  break;
	}
      
      outb(OTI_INDEX,OTI87_CONFIG_2);
      temp2 = ( inb(OTI_R_W) & 0x0C);
      switch (temp2 )
	{
	case 0x00 :
	  ErrorF("%s %s: oti087: BT476, SC11487, IMSG174 or equivalent RAMDAC.\n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  break;
	case 0x04 :
	  ErrorF("%s %s: oti087: MU9C1715 or equivalent RAMDAC.\n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  break;
	case 0x08 :
	  ErrorF("%s %s: oti087: BT484 or equivalent RAMDAC.\n",
		 XCONFIG_PROBED, vga256InfoRec.name);
	  break;
	}
    }

  /* Here we know the chipset so we change the banking routines, and
   * the structure, allowing for "Linear Framebuffer"
   */
  
  if (OTI_chipset == OTI87)
    {
      OAK.ChipSetRead = OTI87SetRead ;
      OAK.ChipSetWrite = OTI87SetWrite ;
      OAK.ChipSetReadWrite = OTI87SetReadWrite ;
      OTI_2mb_bank = TRUE;

      if (OFLG_ISSET(OPTION_LINEAR, &vga256InfoRec.options))     
	{
	  OAK.ChipUseLinearAddressing = TRUE ;
	  if (OTI_vlb) 
	    {
	      OAK.ChipLinearBase=0x4E00000 ;/* This is a hack ;), but works */
	    } 
	  else 
	    {
	      OAK.ChipLinearBase=0xE00000 ;/* This is a default, just in case*/
	    }
	  OAK.ChipLinearSize = vga256InfoRec.videoRam * 1024 ;
	  OTI_linear = TRUE; /* Don't take this out, it is needed */
#if 0
	  OAK.ChipHas16bpp = TRUE ;
	  OAK.ChipHas32bpp = TRUE ;
	  ErrorF("OTI87 driver: HiColor and TrueColor Enabled \n");
#endif
	}
    }

  /*
   * Again, if the user has specified the clock values in the XF86Config
   * file, we respect those choices.
   */
  if (!vga256InfoRec.clocks)
    {
      /*
       * This utility function will probe for the clock values.
       * It is passed the number of supported clocks, and a
       * pointer to the clock-select function.
       */
      if (OTI_chipset == OTI87) {
	vgaGetClocks(16, OAKClockSelect);
      }
      else {
	vgaGetClocks(8, OAKClockSelect);
      }
    }
  
  /*
   * It is recommended that you fill in the maximum allowable dot-clock
   * rate for your chipset.  If you don't do this, the default of
   * 90MHz will be used; this is likely too high for many chipsets.
   * This is specified in KHz, so 90Mhz would be 90000 for this
   * setting.
   *
   * I could have used a #define to set this max speed, but the board 
   * manual recomends a max dot clock of 80Mhz, and the usual clock
   * generator of these boards is the OTI068, which gives a maximum clock
   * of 78Mhz, so I suppose its not necessary to define it. Personally,
   * I doubt any OEM will be able to use higher dot clocks with 70ns DRAM
   * and this chipset.
   */
  
  vga256InfoRec.maxClock = 80000 ;
  
  
  /*
   * Last we fill in the remaining data structures.  We specify
   * the chipset name, using the Ident() function and an appropriate
   * index.  We set a boolean for whether or not this driver supports
   * banking for the Monochrome server.  And we set up a list of all
   * the vendor flags that this driver can make use of.
   */
  vga256InfoRec.chipset = OAKIdent(OTI_chipset);
  vga256InfoRec.bankedMono = TRUE;
  
  /* Okay, here go the option flags, some are used for debugging as
   * well as testing, others should improve performance. The comments
   * after the things describe their use.
   */
  
  OFLG_SET(OPTION_LINEAR, &OAK.ChipOptionFlags);
  OFLG_SET(OPTION_FAVOUR_BITBLT, &OAK.ChipOptionFlags);
  OFLG_SET(OPTION_FIFO_CONSERV, &OAK.ChipOptionFlags);
  OFLG_SET(OPTION_FIFO_AGGRESSIVE, &OAK.ChipOptionFlags);

  return(TRUE);
  
}

/*
 * OAKEnterLeave --
 *
 * This function is called when the virtual terminal on which the server
 * is running is entered or left, as well as when the server starts up
 * and is shut down.  Its function is to obtain and relinquish I/O 
 * permissions for the SVGA device.  This includes unlocking access to
 * any registers that may be protected on the chipset, and locking those
 * registers again on exit.
 */
static void 
OAKEnterLeave(enter)
     Bool enter;
{
  static unsigned char save;
  unsigned char temp;
  
  if (enter)
    {
      xf86EnableIOPorts(vga256InfoRec.scrnIndex);
      
      /* 
       * This is a global.  The CRTC base address depends on
       * whether the VGA is functioning in color or mono mode.
       * This is just a convenient place to initial)ze this
       * variable.
       */
      vgaIOBase = (inb(0x3CC) & 0x01) ? 0x3D0 : 0x3B0;
      
      /*
       * Here we deal with register-level access locks.  This
       * is a generic VGA protection; most SVGA chipsets have
       * similar register locks for their extended registers
       * as well.
       */
      
      /* Unprotect CRTC[0-7] */
      outb(vgaIOBase + 4, 0x11); temp = inb(vgaIOBase + 5);
      outb(vgaIOBase + 5, temp & 0x7F);
      
      
      if (OTI_chipset != OTI87) { /* no locks on the 087*/
	outb(OTI_INDEX, OTI_CRT_CNTL);
	temp = inb(OTI_R_W);
	outb(OTI_R_W, temp & 0xF0);
	save = temp;
      }
    }
  else {
    /*
     * Here undo what was done above.
     */
    if (OTI_chipset != OTI87) { /* no locks on the 087 */
      
      outb(OTI_INDEX, OTI_CRT_CNTL);
      /* don't set the i/o write test bit even though
	 we cleared it on entry */
      outb(OTI_R_W, (save & 0xF7) );
    }
    /* Protect CRTC[0-7] */
    outb(vgaIOBase + 4, 0x11); temp = inb(vgaIOBase + 5);
    outb(vgaIOBase + 5, (temp & 0x7F) | 0x80);
    
    xf86DisableIOPorts(vga256InfoRec.scrnIndex);
  }
}


/*
 * OAKRestore --
 *
 * This function restores a video mode.  It basically writes out all of
 * the registers that have previously been saved in the vgaOAKRec data 
 * structure.
 *
 * Note that "Restore" is a little bit incorrect.  This function is also
 * used when the server enters/changes video modes.  The mode definitions 
 * have previously been initialized by the Init() function, below.
 */
extern void 
OAKRestore(restore)
     vgaOAKPtr restore;
{
  unsigned char temp;
  
  /*
   * Whatever code is needed to get things back to bank zero should be
   * placed here.  Things should be in the same state as when the
   * Save/Init was done.
   */
  /* put the segment regs back to zero */
  
  if (OTI_chipset != OTI87 ) 
    outw(OTI_INDEX, OTI_SEGMENT);
  else
    {
      outw(OTI_INDEX, OTI87_XWRITE);
      outw(OTI_INDEX, OTI87_XREAD);
    }
  /* Ok. to prevent text mode to be absolutely fouled up, oti077-67 
   * chipsets needed the extended clock bit set to 0 to avoid 
   * something, as the oti087 text mode is currenlty fouled when
   * I leave X, I will use his experience ;) that is, I will cut&paste
   * a bit of his code. OK,OK its not a c&p, but the function is the
   * same. :) 
   */
  
  /*
   *  Set the OTI-Misc register. We must be sure that we
   *  aren't in one of the extended graphics modes when
   *  we are leaving X and about to call vgaHWRestore for
   *  the last time. If we don't text mode is completely
   *  fouled up.
   */
  
  
  if (OTI_chipset == OTI87)
    outw(OTI_INDEX, OTI87_MODE   + (restore->oti87ModeSelect << 8 ));
  else 
    {
      outb(OTI_INDEX, OTI_MISC);
      temp = inb(OTI_R_W) & 0x20; /* get the clock bit */
      temp |= (restore->oakMisc & 0xDF);
      outb(OTI_R_W, temp);
    }
  
  /*
   * Code to restore any SVGA registers that have been saved/modified
   * goes here.  Note that it is allowable, and often correct, to 
   * only modify certain bits in a register by a read/modify/write cycle.
   *
   * A special case - when using an external clock-setting program,
   * this function must not change bits associated with the clock
   * selection.  This condition can be checked by the condition:
   *
   *	if (restore->std.NoClock >= 0)
   *		restore clock-select bits.
   */
  
  
  
  
  if (OTI_chipset != OTI87)
    {
      
      outb(OTI_INDEX, OTI_SEGMENT);
      outb(OTI_R_W, restore->oakBank);
      
      if (restore->std.NoClock >= 0) {
	/* restore clock-select bits. */
	outw(OTI_INDEX, OTI_MISC + (restore->oakMisc << 8));
      } else {
	/* don't restore clock-select bits. */
	outb(OTI_INDEX, OTI_MISC);
	temp = inb(OTI_R_W) & 0x20; /* get the clock bit */
	temp |= (restore->oakMisc & 0xDF);
	outb(OTI_R_W, temp);
      }
      
      outb(OTI_INDEX, OTI_BCOMPAT);
      temp = inb(OTI_R_W);
      temp &= 0xF9;
      temp |= (restore->oakBCompat & 0x6);
      outb(OTI_INDEX, OTI_CONFIG);
      temp = inb(OTI_R_W);
      temp &= 0xF7;
      temp |= (restore->oakConfig & 0x8);
      outb(OTI_R_W, temp);
      outw(OTI_INDEX, OTI_OVERFLOW + (restore->oakOverflow << 8));
      outw(OTI_INDEX, OTI_HSYNC2 + (restore->oakHsync2 << 8));
      
      if ( OTI_chipset != OTI67)
	outw(OTI_INDEX, OTI_OVERFLOW2 + (restore->oakOverflow2 << 8));
    }
  else     /* code for 87 */
    {
      
      outw(OTI_INDEX, OTI87_XREAD             + (restore->oti87RBank << 8 ));
      outw(OTI_INDEX, OTI87_XWRITE            + (restore->oti87WBank << 8 ));
      outw(OTI_INDEX, OTI87_MODE       + (restore->oti87ModeSelect << 8 ));
      if (restore->std.NoClock >= 0) 
	{
	  outw(OTI_INDEX, OTI87_CLOCK    + (restore->oti87Clock      << 8 ));
	}
      outw(OTI_INDEX, OTI87_FIFO_DEPTH  + (restore->oti87FIFODepth  << 8 ));
      outw(OTI_INDEX, OTI87_OVERFLOW     + (restore->oti87Overflow << 8 ));
      outw(OTI_INDEX, OTI87_HSYNC       + (restore->oti87Hsync2     << 8 ));
      
      /* Advanced feature registers */
      
      outw(OTI_INDEX, OTI87_VLB_CONTROL +(restore->oti87VLBControl << 8 ));
      outw(OTI_INDEX, OTI87_MEM_MAPPING + (restore->oti87Mapping    << 8 ));
      outw(OTI_INDEX, OTI87_BUS_CTRL    + (restore->oti87BusControl << 8 ));
      outw(OTI_INDEX, OTI87_EXT_CRTC_CTRL + (restore->oti87XCrt    << 8 ));
      outw(OTI_INDEX, OTI87_PALETTE_RANGE +(restore->oti87ColorPalette <<8));
      outw(OTI_INDEX, OTI87_FEATURE_CTRL + (restore->oti87Feature    << 8 ));
      outw(OTI_INDEX,OTI87_COLOR_EXPANDER + 
	   (restore->oti87ColorExpansion<<8));
      outw(OTI_INDEX, OTI87_FORE      + (restore->oti87FGExpansion  << 8 ));
      outw(OTI_INDEX, OTI87_BACK       + (restore->oti87BGExpansion << 8 ));
      outw(OTI_INDEX, OTI87_PATTERN   + (restore->oti87ColorPattern << 8 ));
      outw(OTI_INDEX, OTI87_MASK       + (restore->oti87PixMask     << 8 ));
      outw(OTI_INDEX, OTI87_HC_HORIZONTAL_H  + (restore->oti87HC1    << 8 ));
      outw(OTI_INDEX, OTI87_HC_HORIZONTAL_L  + (restore->oti87HC2    << 8 ));
      outw(OTI_INDEX, OTI87_HC_VERTICAL_H    + (restore->oti87HC3    << 8 ));
      outw(OTI_INDEX, OTI87_HC_VERTICAL_L    + (restore->oti87HC4    << 8 ));
      outw(OTI_INDEX, OTI87_HC_HOR_PRESET    + (restore->oti87HC5    << 8 ));
      outw(OTI_INDEX, OTI87_HC_VER_PRESET    + (restore->oti87HC6    << 8 ));
      outw(OTI_INDEX, OTI87_HC_START_HL      + (restore->oti87HC7    << 8 ));
      outw(OTI_INDEX, OTI87_HC_START_LH      + (restore->oti87HC8    << 8 ));
      outw(OTI_INDEX, OTI87_HC_START_LL      + (restore->oti87HC9    << 8 ));
      outw(OTI_INDEX, OTI87_HC_BACK          + (restore->oti87HC10   << 8 ));
      outw(OTI_INDEX, OTI87_HC_FORE          + (restore->oti87HC11   << 8 ));
      outw(OTI_INDEX, OTI87_HC_CTRL          + (restore->oti87HC12   << 8 ));
    }

  /*
   * This function handles restoring the generic VGA registers.
   */
  vgaHWRestore((vgaHWPtr)restore);
    
  outw(0x3C4, 0x0300); /* now reenable the timing sequencer */
  
}

/*
 * OAKSave --
 *
 * This function saves the video state.  It reads all of the SVGA registers
 * into the vgaOAKRec data structure.  There is in general no need to
 * mask out bits here - just read the registers.
 */
extern void *
OAKSave(save)
     vgaOAKPtr save;
{
  unsigned char temp, temp1;
  /*
   * Whatever code is needed to get back to bank zero goes here.
   */
  
  
  if (OTI_chipset != OTI87)
    {
      outb(OTI_INDEX, OTI_SEGMENT);
      temp = inb(OTI_R_W);
      /* put segment register to zero */
      outb(OTI_R_W, 0x0);
    }
  else
    {
      outb(OTI_INDEX, OTI87_XREAD);
      temp = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_XWRITE);
      temp1 = inb(OTI_R_W);
      outb(OTI_R_W, 0x0);
    }
  
  
  /*
   * This function will handle creating the data structure and filling
   * in the generic VGA portion.
   */
  save = (vgaOAKPtr)vgaHWSave((vgaHWPtr)save, sizeof(vgaOAKRec));
  
  /*
   * The port I/O code necessary to read in the extended registers 
   * into the fields of the vgaOAKRec structure goes here.
   */
  
  if (OTI_chipset == OTI87)
    {
      
      save->oti87RBank = temp;      
      save->oti87WBank = temp1; 
      
      outb(OTI_INDEX, OTI87_MODE );
      save->oti87ModeSelect          = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_CLOCK);
      save->oti87Clock               = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_FIFO_DEPTH );
      save->oti87FIFODepth           = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_OVERFLOW );
      save->oti87Overflow            = inb (OTI_R_W);
      outb(OTI_INDEX, OTI87_HSYNC );
      save->oti87Hsync2              = inb(OTI_R_W);
      
      outb(OTI_INDEX, OTI87_VLB_CONTROL);
      save->oti87VLBControl          = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_MEM_MAPPING );
      save->oti87Mapping             = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_CLOCK);
      save->oti87Clock               = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_BUS_CTRL);
      save->oti87BusControl          = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_EXT_CRTC_CTRL );
      save->oti87XCrt               = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_PALETTE_RANGE );
      save->oti87ColorPalette        = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_FEATURE_CTRL );
      save->oti87Feature             = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_COLOR_EXPANDER );
      save->oti87ColorExpansion      = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_FORE );
      save->oti87FGExpansion         = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_BACK );
      save->oti87BGExpansion         = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_PATTERN);
      save->oti87ColorPattern        = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_MASK );
      save->oti87PixMask             = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_HORIZONTAL_H);
      save->oti87HC1                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_HORIZONTAL_L );
      save->oti87HC2                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_VERTICAL_H );
      save->oti87HC3                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_VERTICAL_L );
      save->oti87HC4                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_HOR_PRESET);
      save->oti87HC5                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_VER_PRESET);
      save->oti87HC6                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_START_HL);
      save->oti87HC7                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_START_LH);
      save->oti87HC8                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_START_LL );
      save->oti87HC9                 = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_BACK);
      save->oti87HC10                = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_FORE);
      save->oti87HC11                = inb(OTI_R_W);
      outb(OTI_INDEX, OTI87_HC_CTRL);
      save->oti87HC12                = inb(OTI_R_W);
    }
  else 
    {
      save->oakBank = temp; /* this seems silly, leftover from textmode
			     *  problems, but it works my friend, it works.
			     */
      
      outb(OTI_INDEX, OTI_MISC);
      save->oakMisc = inb(OTI_R_W);
      outb(OTI_INDEX, OTI_CONFIG);
      save->oakConfig = inb(OTI_R_W);
      outb(OTI_INDEX, OTI_BCOMPAT);
      save->oakBCompat = inb(OTI_R_W);
      outb(OTI_INDEX, OTI_OVERFLOW);
      save->oakOverflow = inb(OTI_R_W);
      outb(OTI_INDEX, OTI_HSYNC2);
      save->oakHsync2 = inb(OTI_R_W);
      if ( OTI_chipset != OTI67) {
	outb(OTI_INDEX, OTI_OVERFLOW2);
	save->oakOverflow2 = inb(OTI_R_W);
      }
    }
  return ((void *) save);
}

/*
 * OAKInit --
 *
 * This is the most important function (after the Probe) function.  This
 * function fills in the vgaOAKRec with all of the register values needed
 * to enable either a 256-color mode (for the color server) or a 16-color
 * mode (for the monochrome server).
 *
 * The 'mode' parameter describes the video mode.  The 'mode' structure 
 * as well as the 'vga256InfoRec' structure can be dereferenced for
 * information that is needed to initialize the mode.  The 'new' macro
 * (see definition above) is used to simply fill in the structure.
 */
static Bool
OAKInit(mode)
     DisplayModePtr mode;
{
  
  if (mode->Flags & V_INTERLACE ) {
    /*
       When in interlace mode cut the vertical numbers in half.
       (Try and find that in the manual!)
       Could just set VGA_DIVIDE_VERT but we'd have to
       test it and do the divides here anyway.
       */ 
    if (!mode->CrtcVAdjusted) {
      mode->CrtcVTotal >>= 1;
      mode->CrtcVDisplay >>= 1;
      mode->CrtcVSyncStart >>= 1;
      mode->CrtcVSyncEnd >>= 1;
      mode->CrtcVAdjusted = TRUE;
    }
  }
  /*
   * This will allocate the datastructure and initialize all of the
   * generic VGA registers.
   */
  if (!vgaHWInit(mode,sizeof(vgaOAKRec)))
    return(FALSE);
  
  /*
   * Here all of the other fields of 'new' get filled in, to
   * handle the SVGA extended registers.  It is also allowable
   * to override generic registers whenever necessary.
   *
   * A special case - when using an external clock-setting program,
   * this function must not change bits associated with the clock
   * selection.  This condition can be checked by the condition:
   *
   *	if (new->std.NoClock >= 0)
   *		initialize clock-select bits.
   */
  
  if (OTI_chipset != OTI87 )
    {
      
#ifndef MONOVGA
      /* new->std.CRTC[19] = vga256InfoRec.virtualX >> 3; /* 3 in byte mode */
      /* much clearer as 0x01 than 0x41, seems odd though... */
      new->std.Attribute[16] = 0x01; 
      
      /*
       *  We set the fifo depth to maximum since it seems to
       *  remove screen interference at high resolution. This
       *  could probably be set to some other value for better
       *  performance.
       */
      
      if ( new->std.NoClock >= 0 ) {
	new->oakMisc = 0x0F | ((new->std.NoClock & 0x04) << 3);
      } else
	new->oakMisc = 0x0F; /*  high res mode, deep fifo */
#else
      if ( new->std.NoClock >= 0 ) {
	new->oakMisc = 0x18 | ((new->std.NoClock & 0x04) << 3);
      } else
	new->oakMisc = 0x18; /*  16 color high res mode */
#endif
      new->oakBank = 0; 
      new->oakBCompat = 0x80; /* vga mode */
      new->oakConfig = (OTI_chipset != OTI67 ? 0x8 : 0 );
      /* set number of ram chips! */                    /* 40 */
      new->oakMisc |= (vga256InfoRec.videoRam == 1024 ? 0x40 : 0x00 );
      new->oakMisc |= (vga256InfoRec.videoRam >= 512 ? 0x80 : 0x00 );
      if (mode->Flags & V_INTERLACE ) {
	new->oakOverflow = 0x80 |
	  /* V-retrace-start */  (((mode->CrtcVSyncStart ) & 0x400) >> 8 ) |
	    /* V-blank-start */  ((((mode->CrtcVDisplay-1) ) & 0x400) >> 9 ) |
	      /* V-total */      ((((mode->CrtcVTotal-2) ) & 0x400) >> 10 ) ;
	/* can set overflow2 no matter what here since restore will
	   do the right thing */
	new->oakOverflow2 = 0;
	/* Doc. says this is when vertical retrace will start in 
	   every odd frame in interlaced mode in characters. Hmm??? */
	new->oakHsync2 =  (mode->CrtcVTotal-2) >> 3; 
      } else {
	new->oakOverflow = (mode->Flags & V_INTERLACE ? 0x80 : 0x00) |
	  /* V-retrace-start */  ((mode->CrtcVSyncStart & 0x400) >> 8 ) |
	    /* V-blank-start */  (((mode->CrtcVDisplay-1) & 0x400) >> 9 ) |
	      /* V-total */      (((mode->CrtcVTotal-2) & 0x400) >> 10 ) ;
	/* can set overflow2 no matter what here since restore will
	   do the right thing */
	new->oakOverflow2 = 0;
	new->oakHsync2 = 0;
      }
    }
  else       /* oti087 procedure */
    {
      new->std.Attribute[16] = 0x01; 
      
      if (OFLG_ISSET(OPTION_FIFO_CONSERV, &vga256InfoRec.options)) {
	new->oti87FIFODepth = 0xE ;
	ErrorF ("%s %s: oti087: FIFO set to 14. \n", XCONFIG_GIVEN,
		vga256InfoRec.name);
      } else if (OFLG_ISSET(OPTION_FIFO_AGGRESSIVE, &vga256InfoRec.options)) {
	new->oti87FIFODepth = 0x2 ;
	ErrorF ("%s %s: oti087: FIFO set to 2. \n", XCONFIG_GIVEN,
		vga256InfoRec.name);
      } else {
	new->oti87FIFODepth = 0x6 ;
	ErrorF ("%s %s: oti087: FIFO set to 6. \n", XCONFIG_PROBED,
		vga256InfoRec.name);
      }
      new->oti87ModeSelect = 0x04 ;
      if ( new->std.NoClock >= 0 ) {
	new->oti87Clock = new->std.NoClock ;
      }
      new->oti87RBank = 0;
      new->oti87WBank = 0; 
      if (mode->Flags & V_INTERLACE ) 
	{
	  new->oti87Overflow = 0x80 |
	    /* V-retrace-start */  (((mode->CrtcVSyncStart ) & 0x400) >> 8 ) |
	      /* V-blank-start *   ((((mode->CrtcVDisplay-1) ) & 0x400) >> 9 ) |
		/* V-total */      ((((mode->CrtcVTotal-2) ) & 0x400) >> 10 ) ;
	  
	  
	  /* Doc. says this is when vertical retrace will start in 
	     every odd frame in interlaced mode in characters. Hmm??? */
	  
	  new->oti87Hsync2 =  (mode->CrtcVTotal-2) >> 3; 
	} 
      else 
	{
	  new->oti87Overflow = (mode->Flags & V_INTERLACE ? 0x80 : 0x00) |
	    /* V-retrace-start */  ((mode->CrtcVSyncStart & 0x400) >> 8 ) |
	      /* V-blank-start */  (((mode->CrtcVDisplay-1) & 0x400) >> 9 ) |
		/* V-total */      (((mode->CrtcVTotal-2) & 0x400) >> 10 ) ;
	  
	  new->oti87Hsync2 = 0;
	}
      
      if (OFLG_ISSET(OPTION_NOACCEL, &vga256InfoRec.options))   {
	new-> oti87VLBControl = 0x00;
      } else {
	new-> oti87VLBControl = 0x10;
      }

      
      new-> oti87BusControl = 0xC8 ;
      new-> oti87ColorPalette = 0xf ; 
      new-> oti87Feature = 0x8 ; 
      new-> oti87ColorExpansion = 0x0 ; 
      new-> oti87FGExpansion = 0x0;
      new-> oti87BGExpansion = 0x0; 
      new-> oti87ColorPattern = 0xff ;
      new-> oti87PixMask = 0xff ;
      
      /*
	 new-> oti87  
	 new-> oti87 
	 new-> oti87   No HC support, I will leave the stubs here anyway 
	 new-> oti87  
	 new-> oti87  
	 new-> oti87  
	 new-> oti87  
	 new-> oti87
	 new-> oti87
	 new-> oti87
	 new-> oti87
	 new-> oti87
	 */
    }
  
  /* Explain what it to be done to the stderr messages, just to be sure */

  if (OTI_2mb_bank)
    {
      ErrorF ("%s %s: oti087: Using 2 MB banking routines. \n", XCONFIG_PROBED,
	      vga256InfoRec.name);
    }

  if (OTI_linear)
    {
      if (OTI_vlb)
	{
    
  /* This is an undocumented option of the board. Disabling DMA tranasfers
   * the 64MB (0x4000000) address mark pin is enabled, allowing higher-than
   * 16Mbyte-mark addressing, I must thank the people who did the
   * README.cirrus for the clue, and the binaries of the OTI087 MS-Windoze
   * drivers for allowing me to do some diff's :), it was a hell to find 
   * and debug, but at last....it's here (and I will be able to buy another
   * 4 Mbytes of RAM and be allowed linear addressing)
   */
   
	  switch(vga256InfoRec.videoRam)
	    {
	    case 256:
	      new -> oti87Mapping = 0xE3;
	      break;
	    case 512:
	      new -> oti87Mapping = 0xE7;
	      break;
	    case 1024:
	      new -> oti87Mapping = 0xEB; 
	      break;
	    case 2048:
	      new -> oti87Mapping = 0xEF; 
	      break;
	    } 
	}
      else
	{
	  switch(vga256InfoRec.videoRam)
	    {
	    case 256:
	      new -> oti87Mapping = 0xE1;
	      break;
	    case 512:
	      new -> oti87Mapping = 0xE5;
	      break;
	    case 1024:
	      new -> oti87Mapping = 0xE9; 
	      break;
	    case 2048:
	      new -> oti87Mapping = 0xED; 
	      break;
	    } 
	}    
      ErrorF ("%s %s: oti087: linear framebuffer enabled. \n", XCONFIG_GIVEN,
	      vga256InfoRec.name);
      ErrorF ("%s %s: oti087: base address is set at 0x%X.\n", XCONFIG_GIVEN,
              vga256InfoRec.name, OAK.ChipLinearBase);
      ErrorF ("%s %s: oti087: aperture size is %d Kbytes. \n", XCONFIG_PROBED,
              vga256InfoRec.name,(OAK.ChipLinearSize/1024));
    }
  if (OFLG_ISSET(OPTION_FAVOUR_BITBLT, &vga256InfoRec.options))
    {
      new -> oti87Feature = 0x0C;
      ErrorF ("%s %s: oti087: BitBlt engine enabled. \n", XCONFIG_GIVEN,
	      vga256InfoRec.name);
    }
  return(TRUE);
}

/*
 * OAKAdjust --
 *
 * This function is used to initialize the SVGA Start Address - the first
 * displayed location in the video memory.  This is used to implement the
 * virtual window.
 */
static void 
OAKAdjust(x, y)
     int x, y;
{
  int temp;
  /*
   * The calculation for Base works as follows:
   *
   *	(y * virtX) + x ==> the linear starting pixel
   *
   * This number is divided by 8 for the monochrome server, because
   * there are 8 pixels per byte.
   *
   * For the color server, it's a bit more complex.  There is 1 pixel
   * per byte.  In general, the 256-color modes are in word-mode 
   * (16-bit words).  Word-mode vs byte-mode is will vary based on
   * the chipset - refer to the chipset databook.  So the pixel address 
   * must be divided by 2 to get a word address.  In 256-color modes, 
   * the 4 planes are interleaved (i.e. pixels 0,3,7, etc are adjacent 
   * on plane 0). The starting address needs to be as an offset into 
   * plane 0, so the Base address is divided by 4.
   *
   * So:
   *    Monochrome: Base is divided by 8
   *    Color:
   *	if in word mode, Base is divided by 8
   *	if in byte mode, Base is divided by 4
   *
   * The generic VGA only supports 16 bits for the Starting Address.
   * But this is not enough for the extended memory.  SVGA chipsets
   * will have additional bits in their extended registers, which
   * must also be set.
   */
  
  int Base = (y * vga256InfoRec.virtualX + x ) >> 3;
  
  /*
   * These are the generic starting address registers.
   */
  outw(vgaIOBase + 4, (Base & 0x00FF00) | 0x0C);
  outw(vgaIOBase + 4, ((Base & 0x00FF) << 8) | 0x0D);
  
  /*
   * Here the high-order bits are masked and shifted, and put into
   * the appropriate extended registers.
   */
  
  if (OTI_chipset != OTI87 ) 
    {
      outb(OTI_INDEX, OTI_OVERFLOW);
      temp = inb(OTI_R_W);
      temp &= 0xF7;
      temp |= ((Base & 0x10000) >> (5+8));
      outb(OTI_R_W, temp);
      if ( OTI_chipset != OTI67) { 
	outb(OTI_INDEX, OTI_OVERFLOW2);
	temp = inb(OTI_R_W);
	temp &= 0xF7;
	temp |= ((Base & 0x20000) >> (6 + 8));
	outb(OTI_R_W, temp);
      }
    }
  else
    {
      outb (OTI_INDEX, OTI87_EXT_CRTC_CTRL);
      outb (OTI_R_W, Base >> 16 );
    }
}

/*
 * OAKValidMode --
 *
 */
static Bool
OAKValidMode(mode)
DisplayModePtr mode;
{
return TRUE;
}
