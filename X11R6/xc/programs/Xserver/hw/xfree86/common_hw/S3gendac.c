/* $XFree86: xc/programs/Xserver/hw/xfree86/common_hw/S3gendac.c,v 3.5 1995/01/20 04:21:19 dawes Exp $ */
/*
 * Progaming of the S3 gendac programable clocks, from the S3 Gendac
 * programing documentation by S3 Inc. 
 * Jon Tombs <jon@esix2.us.es>
 */
 
#include "S3gendac.h" 
#include "compiler.h"
#define NO_OSLIB_PROTOTYPES
#include "xf86_OSlib.h"
#include <math.h>

extern int vgaIOBase;

static void setdacpll(
#if NeedFunctionPrototypes
int reg, unsigned char data1, unsigned char data2
#endif
);

static void settriopll(
#if NeedFunctionPrototypes
int reg, unsigned char data1, unsigned char data2
#endif
);

static int commonSetClock( 
#if NeedFunctionPrototypes
   long freq, int clock,
   int min_n2, int trio_pll,
   long freq_min, long freq_max
#endif
);     

int
S3gendacSetClock(freq, clk)
long freq;
int clk;
{
   return commonSetClock(freq, clk, 0, 0, 100000, 250000);
}

int
ICS5342SetClock(freq, clk)
long freq;
int clk;
{
   return commonSetClock(freq, clk, 1, 0, 100000, 250000);
}

int
S3TrioSetClock(freq, clk)
long freq;
int clk;
{
   return commonSetClock(freq, clk, 0, 1, 135000, 270000);
}

static int
commonSetClock(freq, clk, min_n2, trio_pll, freq_min, freq_max)
long freq;
int clk;
int min_n2, trio_pll;
long freq_min, freq_max;
{
   double ffreq, ffreq_min, ffreq_max;
   double div, diff, best_diff;
   unsigned int m;
   unsigned char n, n1, n2;
   unsigned char best_n1=16+2, best_n2=2, best_m=125+2;

   ffreq     = freq     / 1000.0 / BASE_FREQ;
   ffreq_min = freq_min / 1000.0 / BASE_FREQ;
   ffreq_max = freq_max / 1000.0 / BASE_FREQ;

   if (ffreq < ffreq_min/8) {
      ErrorF("invalid frequency %1.3f MHz  [freq >= %1.3f MHz]\n", 
	     ffreq*BASE_FREQ, ffreq_min*BASE_FREQ/8);
      ffreq = ffreq_min/8;
   }
   if (ffreq > ffreq_max / (1<<min_n2)) {
      ErrorF("invalid frequency %1.3f MHz  [freq <= %1.3f MHz]\n", 
	     ffreq*BASE_FREQ, ffreq_max*BASE_FREQ / (1<<min_n2));
      ffreq = ffreq_max / (1<<min_n2);
   }

   /* work out suitable timings */

   best_diff = ffreq;
   
   for (n2=min_n2; n2<=3; n2++) {
      for (n1 = 1+2; n1 <= 31+2; n1++) {
	 m = (int)(ffreq * n1 * (1<<n2) + 0.5) ;
	 if (m < 1+2 || m > 127+2) 
	    continue;
	 div = (double)(m) / (double)(n1);	 
	 if ((div >= ffreq_min) &&
	     (div <= ffreq_max)) {
	    diff = ffreq - div / (1<<n2);
	    if (diff < 0.0) 
	       diff = -diff;
	    if (diff < best_diff) {
	       best_diff = diff;
	       best_m    = m;
	       best_n1   = n1;
	       best_n2   = n2;
	    }
	 }
      }
   }
   
#if 0
   ErrorF("clk %d, setting to %1.6f MHz (m %d, n1 %d, n2 %d)\n", clk,
	  ((double)(best_m) / (double)(best_n1) / (1 << best_n2)) * BASE_FREQ
	  ,best_m-2 ,best_n1-2 ,best_n2
	  );
#endif

   n = (best_n1 - 2) | (best_n2 << 5);
   m = best_m - 2;

   if (trio_pll)
      settriopll(clk, m, n);
   else
      setdacpll(clk, m, n);

   return 0;
}	   


static void
setdacpll(reg, data1, data2)
int reg;
unsigned char data1;
unsigned char data2;
{
   unsigned char tmp, tmp1;
   int vgaCRIndex = vgaIOBase + 4;
   int vgaCRReg = vgaIOBase + 5;
		
   /* set RS2 via CR55, yuck */
   outb(vgaCRIndex, 0x55);
   tmp = inb(vgaCRReg) & 0xFC;
   outb(vgaCRReg, tmp | 0x01);  
   tmp1 = inb(GENDAC_INDEX);

   outb(GENDAC_INDEX, reg);
   outb(GENDAC_DATA, data1);
   outb(GENDAC_DATA, data2);

   /* Now clean up our mess */
   outb(GENDAC_INDEX, tmp1);  
   outb(vgaCRReg, tmp);
}


static void
settriopll(clk, m, n)
     int clk;
     unsigned char m;
     unsigned char n;
{
   unsigned char tmp;
   int index;

   /*
    * simlulate S3 GENDAC clock numbers:
    * 0,1 for fixed 25 and 28 MHz clocks
    * 2-8 free programmable
    * 10  MCLK
    */

   if (clk < 2) {
      tmp = inb(0x3cc);
      outb(0x3c2, (tmp & 0xf3) | (clk << 2));
   }
   else {
      tmp = inb(0x3cc);
      outb(0x3c2, tmp | 0x0c);

      if (clk != 10)  /* DCLK */
	 index = 0x12;
      else		/* MCLK */
	 index = 0x10;

      outb(0x3c4, 0x08);
      outb(0x3c5, 0x06);  /* unlock extended CR9-CR18 */
      
      outb(0x3c4, index);
      outb(0x3c5, n);
      outb(0x3c4, index+1);
      outb(0x3c5, m);

      outb(0x3c4, 0x15);
      tmp = inb(0x3c5);
      outb(0x3c4, tmp & ~0x20);
      outb(0x3c4, tmp |  0x20);
      outb(0x3c4, tmp & ~0x20);

      outb(0x3c4, 0x08);
      outb(0x3c5, 0x00);  /* lock extended CR9-CR18 */
      
   }
}
