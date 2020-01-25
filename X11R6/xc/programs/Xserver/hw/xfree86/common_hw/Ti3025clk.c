/* $XFree86: xc/programs/Xserver/hw/xfree86/common_hw/Ti3025clk.c,v 3.7 1995/01/26 02:19:00 dawes Exp $ */

/*
 * Copyright 1994 The XFree86 Project, Inc
 *
 * programming the on-chip clock on the Ti3025, derived from the
 * S3 gendac code by Jon Tombs
 * Dirk Hohndel <hohndel@aib.com>
 * Robin Cutshaw <robin@XFree86.org>
 */

#include "s3Ti3020.h" 
#include "compiler.h"
#define NO_OSLIB_PROTOTYPES
#include "xf86_OSlib.h"
#include <math.h>

#ifdef __STDC__
static void
s3ProgramTi3025Clock(int clk, unsigned char n, unsigned char m,
                          unsigned char p)
#else
static void
s3ProgramTi3025Clock(clk, n, m, p)
int clk;
unsigned char n;
unsigned char m;
unsigned char p;
#endif
{
   /*
    * Reset the clock data index
    */
   s3OutTiIndReg(TI_PLL_CONTROL, 0x00, 0x00);

   if (clk != TI_MCLK_PLL_DATA) {
      /*
       * Now output the clock frequency
       */
      s3OutTiIndReg(TI_PIXEL_CLOCK_PLL_DATA, 0x00, n);
      s3OutTiIndReg(TI_PIXEL_CLOCK_PLL_DATA, 0x00, m);
      s3OutTiIndReg(TI_PIXEL_CLOCK_PLL_DATA, 0x00, p | TI_PLL_ENABLE);

      /*
       * And now set up the loop clock for RCLK
       */
      s3OutTiIndReg(TI_LOOP_CLOCK_PLL_DATA, 0x00, 0x01);
      s3OutTiIndReg(TI_LOOP_CLOCK_PLL_DATA, 0x00, 0x01);
      s3OutTiIndReg(TI_LOOP_CLOCK_PLL_DATA, 0x00, p>0 ? p : 1);
      s3OutTiIndReg(TI_MISC_CONTROL, 0x00,
		    TI_MC_LOOP_PLL_RCLK | TI_MC_LCLK_LATCH | TI_MC_INT_6_8_CONTROL);

      /*
       * And finally enable the clock
       */
      s3OutTiIndReg(TI_INPUT_CLOCK_SELECT, 0x00, TI_ICLK_PLL);
   }
   else {
      /*
       * Set MCLK
       */
      s3OutTiIndReg(TI_MCLK_PLL_DATA, 0x00, n);
      s3OutTiIndReg(TI_MCLK_PLL_DATA, 0x00, m);
      s3OutTiIndReg(TI_MCLK_PLL_DATA, 0x00, p | 0x80);
   }
}

#ifdef __STDC__
void Ti3025SetClock(long freq, int clk)
#else
void
Ti3025SetClock(freq, clk)
long freq;
int clk;
#endif
{
   double ffreq;
   int n, p, m;
   int best_n=32, best_m=32;
   double  diff, mindiff;
   
#define FREQ_MIN   27500  /* 110/8 MHz is the specified limit */
#undef  FREQ_MIN
#define FREQ_MIN   12000
#define FREQ_MAX  220000

   if (freq < FREQ_MIN)
      ffreq = FREQ_MIN / 1000.0;
   else if (freq > FREQ_MAX)
      ffreq = FREQ_MAX / 1000.0;
   else
      ffreq = freq / 1000.0;
   


   /* work out suitable timings */

   /* pick the right p value */

   for(p=0; p<4 && ffreq < 110.0; p++)
      ffreq *= 2;
#if FREQ_MIN < 110000/8
   if (p==4) {
      ffreq /= 2;
      p--;
   }
#endif
   
   /* now 110.0 <= ffreq <= 220.0 */   
   
   ffreq /= TI_REF_FREQ;
   
   /* now 7.6825 <= ffreq <= 15.3650 */
   /* the remaining formula is  ffreq = (m+2)*8 / (n+2) */
   
   
   mindiff = ffreq;
   
   for (n = 1; n <= (int)(TI_REF_FREQ/0.5 - 2); n++) {
      m = (int)(ffreq * (n+2) / 8.0 + 0.5) - 2;
      if (m < 1)
	 m = 1;
      else if (m > 127) 
	 m = 127;
      
      diff = ((m+2) * 8) / (n+2.0) - ffreq;
      if (diff<0)
	 diff = -diff;
      
      if (diff < mindiff) {
	 mindiff = diff;
	 best_n = n;
	 best_m = m;
      }
   }
#ifdef DEBUG
   ErrorF("clk %d (%f), setting to %f, n %02x, m %02x, p %d\n", clk,freq/1e3,
	  8.0/(1 << p)*((best_m+2.0)/(best_n+2)) * TI_REF_FREQ,
	  best_n, best_m, p);
#endif
   
   s3ProgramTi3025Clock(clk, best_n, best_m, p);
}
