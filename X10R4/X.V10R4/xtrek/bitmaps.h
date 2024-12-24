/* $Header: bitmaps.h,v 10.1 86/11/30 15:29:52 jg Rel $ */
/* Copyright (c) 1986 	Chris Guthrie */

/**** These are the ships ****/
#define VIEWS		16
#define ship_width	16
#define ship_height	16

static short fed_bits[VIEWS][16] = {
  {		/* fed rotated 0 degrees */
   0x03c0, 0x07e0, 0x0ff0, 0x0ff0, 0x0ff0, 0x07e0, 0x03c0, 0x0990,
   0x0990, 0x0990, 0x0ff0, 0x0990, 0x0990, 0x0810, 0x0810, 0x0810
  },
  {		/* fed rotated 22.5000 degrees */
   0x0600, 0x1fc0, 0x1fc0, 0x1fc0, 0x3fc0, 0x1f80, 0x1f90, 0x0390,
   0x0998, 0x08f8, 0x05c8, 0x06e4, 0x0244, 0x0202, 0x0200, 0x0100
  },
  {		/* fed rotated 45.0000 degrees */
   0x0000, 0x1e00, 0x3f00, 0x7f00, 0x7f00, 0x7f20, 0x7f10, 0x3f98,
   0x1de4, 0x00e2, 0x04f1, 0x02a0, 0x0180, 0x0080, 0x0040, 0x0020
  },
  {		/* fed rotated 67.5000 degrees */
   0x0000, 0x0000, 0x0800, 0x7e00, 0x7ec0, 0xfe30, 0xff1e, 0x7fa1,
   0x7ff0, 0x7878, 0x0050, 0x03c0, 0x00e0, 0x0018, 0x0004, 0x0000
  },
  {		/* fed rotated 90.0000 degrees */
   0x0000, 0x0000, 0x0000, 0x0000, 0x39ff, 0x7c20, 0xfe20, 0xfff8,
   0xfff8, 0xfe20, 0x7c20, 0x39ff, 0x0000, 0x0000, 0x0000, 0x0000
  },
  {		/* fed rotated 112.5000 degrees */
   0x0000, 0x0004, 0x0018, 0x00e0, 0x03c0, 0x0050, 0x7878, 0x7ff0,
   0x7fa1, 0xff1e, 0xfe30, 0x7ec0, 0x7e00, 0x0800, 0x0000, 0x0000
  },
  {		/* fed rotated 135.0000 degrees */
   0x0020, 0x0040, 0x0080, 0x0100, 0x0320, 0x04f1, 0x00e2, 0x01fc,
   0x3f88, 0x7f10, 0x7fa0, 0x7f80, 0x7f80, 0x3f00, 0x1e00, 0x0000
  },
  {		/* fed rotated 157.5000 degrees */
   0x0100, 0x0200, 0x0202, 0x0244, 0x06e4, 0x05c8, 0x08f8, 0x0998,
   0x0390, 0x1f90, 0x1f80, 0x3fc0, 0x1fc0, 0x1fc0, 0x1fc0, 0x0600
  },
  {		/* fed rotated 180.0000 degrees */
   0x0810, 0x0810, 0x0810, 0x0990, 0x0990, 0x0ff0, 0x0990, 0x0990,
   0x0990, 0x03c0, 0x07e0, 0x0ff0, 0x0ff0, 0x0ff0, 0x07e0, 0x03c0
  },
  {		/* fed rotated 202.5000 degrees */
   0x0080, 0x0040, 0x4040, 0x2240, 0x2760, 0x13a0, 0x1f10, 0x1990,
   0x09c0, 0x09f8, 0x01f8, 0x03fc, 0x03f8, 0x03f8, 0x03f8, 0x0060
  },
  {		/* fed rotated 225.0000 degrees */
   0x0400, 0x0200, 0x0100, 0x0180, 0x0540, 0x8f20, 0x4700, 0x27b8,
   0x19fc, 0x08fe, 0x04fe, 0x00fe, 0x00fe, 0x00fc, 0x0078, 0x0000
  },
  {		/* fed rotated 247.5000 degrees */
   0x0000, 0x2000, 0x1800, 0x0700, 0x03c0, 0x0a00, 0x1e1e, 0x0ffe,
   0x85fe, 0x78ff, 0x0c7f, 0x037e, 0x007e, 0x0010, 0x0000, 0x0000
  },
  {		/* fed rotated 270.0000 degrees */
   0x0000, 0x0000, 0x0000, 0x0000, 0xff9c, 0x043e, 0x047f, 0x1fff,
   0x1fff, 0x047f, 0x043e, 0xff9c, 0x0000, 0x0000, 0x0000, 0x0000
  },
  {		/* fed rotated 292.5000 degrees */
   0x0000, 0x0000, 0x0010, 0x007e, 0x037e, 0x0c7f, 0x78ff, 0x85fe,
   0x0ffe, 0x1e1e, 0x0a00, 0x03c0, 0x0700, 0x1800, 0x2000, 0x0000
  },
  {		/* fed rotated 315.0000 degrees */
   0x0000, 0x0078, 0x00fc, 0x01fe, 0x01fe, 0x05fe, 0x08fe, 0x11fc,
   0x3f80, 0x4700, 0x8f20, 0x04c0, 0x0080, 0x0100, 0x0200, 0x0400
  },
  {		/* fed rotated 337.5000 degrees */
   0x0060, 0x03f8, 0x03f8, 0x03f8, 0x03fc, 0x01f8, 0x09f8, 0x09c0,
   0x1990, 0x1f10, 0x13a0, 0x2760, 0x2240, 0x4040, 0x0040, 0x0080
  },
};

static short rom_bits[VIEWS][16] = {
  {		/* rom rotated 0 degrees */
   0x0000, 0x0000, 0x0000, 0x43c2, 0x47e2, 0x4ff2, 0x5ffa, 0x5ffa,
   0x7ffe, 0x7ffe, 0x700e, 0x4002, 0x4002, 0x0000, 0x0000, 0x0000
  },
  {		/* rom rotated 22.5000 degrees */
   0x0000, 0x0008, 0x0008, 0x0384, 0x0ff4, 0x0ff6, 0x8ffe, 0x9ffe,
   0x5ff7, 0x5fc1, 0x7e00, 0x3800, 0x3800, 0x1000, 0x1000, 0x0000
  },
  {		/* rom rotated 45.0000 degrees */
   0x0060, 0x0020, 0x0010, 0x07c8, 0x0ffc, 0x1fff, 0x1ff9, 0x1fe0,
   0x1fc0, 0x9f80, 0xcf00, 0x2e00, 0x1e00, 0x0c00, 0x0400, 0x0600
  },
  {		/* rom rotated 67.5000 degrees */
   0x0300, 0x00e0, 0x0038, 0x01fe, 0x0ff8, 0x0fe0, 0x1fe0, 0x1fc0,
   0x1fc0, 0x0fc0, 0x0f80, 0x0f80, 0x6300, 0x1f80, 0x0780, 0x00c0
  },
  {		/* rom rotated 90.0000 degrees */
   0x0000, 0x1ff8, 0x00e0, 0x03e0, 0x07c0, 0x0fc0, 0x1fc0, 0x1fc0,
   0x1fc0, 0x1fc0, 0x0fc0, 0x07c0, 0x03e0, 0x00e0, 0x1ff8, 0x0000
  },
  {		/* rom rotated 112.5000 degrees */
   0x00c0, 0x0780, 0x1f80, 0x6300, 0x0f80, 0x0f80, 0x0fc0, 0x1fc0,
   0x1fc0, 0x1fe0, 0x0fe0, 0x0ff8, 0x01fe, 0x0038, 0x00e0, 0x0300
  },
  {		/* rom rotated 135.0000 degrees */
   0x0600, 0x0400, 0x0c00, 0x1e00, 0x2e00, 0xcf00, 0x9f80, 0x1fc0,
   0x1fe0, 0x1ff9, 0x1fff, 0x0ffc, 0x07e8, 0x0010, 0x0020, 0x0060
  },
  {		/* rom rotated 157.5000 degrees */
   0x0000, 0x1000, 0x1000, 0x3800, 0x3800, 0x7e00, 0x5fc1, 0x5ff7,
   0x9ffe, 0x8ffe, 0x0ff6, 0x0ff4, 0x0384, 0x0008, 0x0008, 0x0000
  },
  {		/* rom rotated 180.0000 degrees */
   0x0000, 0x0000, 0x0000, 0x4002, 0x4002, 0x700e, 0x7ffe, 0x7ffe,
   0x5ffa, 0x5ffa, 0x4ff2, 0x47e2, 0x43c2, 0x0000, 0x0000, 0x0000
  },
  {		/* rom rotated 202.5000 degrees */
   0x0000, 0x0008, 0x0008, 0x001c, 0x001c, 0x007e, 0x83fa, 0xeffa,
   0x7ff9, 0x7ff1, 0x6ff0, 0x2ff0, 0x21c0, 0x1000, 0x1000, 0x0000
  },
  {		/* rom rotated 225.0000 degrees */
   0x0060, 0x0020, 0x0030, 0x0078, 0x0074, 0x00f3, 0x01f9, 0x03f8,
   0x07f8, 0x9ff8, 0xfff8, 0x3ff0, 0x13e0, 0x0800, 0x0400, 0x0600
  },
  {		/* rom rotated 247.5000 degrees */
   0x0300, 0x01e0, 0x01f8, 0x00c6, 0x01f0, 0x01f0, 0x03f0, 0x03f8,
   0x03f8, 0x07f8, 0x07f0, 0x1ff0, 0x7f80, 0x1c00, 0x0700, 0x00c0
  },
  {		/* rom rotated 270.0000 degrees */
   0x0000, 0x1ff8, 0x0700, 0x07c0, 0x03e0, 0x03f0, 0x03f8, 0x03f8,
   0x03f8, 0x03f8, 0x03f0, 0x03e0, 0x07c0, 0x0700, 0x1ff8, 0x0000
  },
  {		/* rom rotated 292.5000 degrees */
   0x00c0, 0x0700, 0x1c00, 0x7f80, 0x1ff0, 0x07f0, 0x07f8, 0x03f8,
   0x03f8, 0x03f0, 0x01f0, 0x01f0, 0x00c6, 0x01f8, 0x01e0, 0x0300
  },
  {		/* rom rotated 315.0000 degrees */
   0x0600, 0x0400, 0x0800, 0x13e0, 0x3ff0, 0xfff8, 0x9ff8, 0x07f8,
   0x07f8, 0x01f9, 0x00f3, 0x0074, 0x0078, 0x0030, 0x0020, 0x0060
  },
  {		/* rom rotated 337.5000 degrees */
   0x0000, 0x1000, 0x1000, 0x21c0, 0x2ff0, 0x6ff0, 0x7ff1, 0x7ff9,
   0xeffa, 0x83fa, 0x007e, 0x001c, 0x001c, 0x0008, 0x0008, 0x0000
  },
};

static short kli_bits[VIEWS][16] = {
  {		/* kli rotated 0 degrees */
   0x0180, 0x03c0, 0x07e0, 0x03c0, 0x0180, 0x0180, 0x0180, 0x2184,
   0x2184, 0x3ffc, 0x3ffc, 0x318c, 0x2004, 0x2004, 0x2004, 0x2004
  },
  {		/* kli rotated 22.5000 degrees */
   0x0400, 0x0e80, 0x0f00, 0x1f00, 0x0f00, 0x0308, 0x0304, 0x018c,
   0x01fe, 0x21f6, 0x17c3, 0x1ec1, 0x1c01, 0x0800, 0x0800, 0x0400
  },
  {		/* kli rotated 45.0000 degrees */
   0x0000, 0x0000, 0x1c00, 0x3c10, 0x3c08, 0x3e0c, 0x071e, 0x03bd,
   0x01e0, 0x00e0, 0x01e0, 0x1380, 0x0f00, 0x0700, 0x0200, 0x0180
  },
  {		/* kli rotated 67.5000 degrees */
   0x0000, 0x0000, 0x0040, 0x1038, 0x781e, 0xf839, 0x7e30, 0x3fe0,
   0x41f0, 0x00f0, 0x00c0, 0x00c0, 0x0580, 0x03c0, 0x00e0, 0x0038
  },
  {		/* kli rotated 90.0000 degrees */
   0x0000, 0x0000, 0x01ff, 0x0070, 0x0060, 0x2060, 0x7060, 0xfff0,
   0xfff0, 0x7060, 0x2060, 0x0060, 0x0070, 0x01ff, 0x0000, 0x0000
  },
  {		/* kli rotated 112.5000 degrees */
   0x0038, 0x00e0, 0x03c0, 0x0580, 0x00c0, 0x00c0, 0x00f0, 0x41f0,
   0x3fe0, 0x7e30, 0xf839, 0x781e, 0x1038, 0x0040, 0x0000, 0x0000
  },
  {		/* kli rotated 135.0000 degrees */
   0x0180, 0x0200, 0x0700, 0x0f00, 0x1380, 0x01e0, 0x00e0, 0x01f1,
   0x03bd, 0x071e, 0x3e0c, 0x3c08, 0x3c10, 0x1c00, 0x0000, 0x0000
  },
  {		/* kli rotated 157.5000 degrees */
   0x0400, 0x0800, 0x0800, 0x1c01, 0x1ec1, 0x17c3, 0x21f6, 0x01fe,
   0x018c, 0x0304, 0x0308, 0x0f00, 0x1f00, 0x0f00, 0x0e80, 0x0400
  },
  {		/* kli rotated 180.0000 degrees */
   0x2004, 0x2004, 0x2004, 0x2004, 0x318c, 0x3ffc, 0x3ffc, 0x2184,
   0x2184, 0x0180, 0x0180, 0x0180, 0x03c0, 0x07e0, 0x03c0, 0x0180
  },
  {		/* kli rotated 202.5000 degrees */
   0x0020, 0x0010, 0x0010, 0x8038, 0x8378, 0xc3e8, 0x6f84, 0x7f80,
   0x3180, 0x20c0, 0x10c0, 0x00f0, 0x00f8, 0x00f0, 0x0170, 0x0020
  },
  {		/* kli rotated 225.0000 degrees */
   0x0180, 0x0040, 0x00e0, 0x00f0, 0x01c8, 0x0780, 0x0700, 0x8f80,
   0xbdc0, 0x78e0, 0x307c, 0x103c, 0x083c, 0x0038, 0x0000, 0x0000
  },
  {		/* kli rotated 247.5000 degrees */
   0x1c00, 0x0700, 0x03c0, 0x01a0, 0x0300, 0x0300, 0x0f00, 0x0f82,
   0x07fc, 0x0c7e, 0x9c1f, 0x781e, 0x1c08, 0x0200, 0x0000, 0x0000
  },
  {		/* kli rotated 270.0000 degrees */
   0x0000, 0x0000, 0xff80, 0x0e00, 0x0600, 0x0604, 0x060e, 0x0fff,
   0x0fff, 0x060e, 0x0604, 0x0600, 0x0e00, 0xff80, 0x0000, 0x0000
  },
  {		/* kli rotated 292.5000 degrees */
   0x0000, 0x0000, 0x0200, 0x1c08, 0x781e, 0x9c1f, 0x0c7e, 0x07fc,
   0x0f82, 0x0f00, 0x0300, 0x0300, 0x01a0, 0x03c0, 0x0700, 0x1c00
  },
  {		/* kli rotated 315.0000 degrees */
   0x0000, 0x0000, 0x0038, 0x083c, 0x103c, 0x307c, 0x78e0, 0xbdc0,
   0x8f80, 0x0700, 0x0780, 0x01c8, 0x00f0, 0x00e0, 0x0040, 0x0080
  },
  {		/* kli rotated 337.5000 degrees */
   0x0020, 0x0170, 0x00f0, 0x00f8, 0x00f0, 0x10c0, 0x20c0, 0x3180,
   0x7f80, 0x6f84, 0xc3e8, 0x8378, 0x8038, 0x0010, 0x0010, 0x0020
  },
};

static short ori_bits[VIEWS][16] = {
  {		/* ori rotated 0 degrees */
   0x0180, 0x03c0, 0x03c0, 0x0180, 0x0180, 0x0180, 0x03c0, 0x07e0,
   0x0ff0, 0x1ff8, 0x3bdc, 0x318c, 0x318c, 0x318c, 0x318c, 0x318c
  },
  {		/* ori rotated 22.5000 degrees */
   0x0400, 0x0e00, 0x0f00, 0x0e00, 0x0700, 0x0300, 0x03e0, 0x03f8,
   0x07fe, 0x0fe6, 0x07c7, 0x0ce3, 0x1c63, 0x0c61, 0x0c30, 0x0620
  },
  {		/* ori rotated 45.0000 degrees */
   0x0000, 0x0000, 0x1800, 0x3c00, 0x3c00, 0x1f7c, 0x07fe, 0x03e7,
   0x07e3, 0x07e1, 0x07f0, 0x0638, 0x061c, 0x0708, 0x0380, 0x01c0
  },
  {		/* ori rotated 67.5000 degrees */
   0x0000, 0x0000, 0x0000, 0x0008, 0x705e, 0xf8ff, 0x7fe1, 0x2fe0,
   0x03f0, 0x03fc, 0x03df, 0x0182, 0x0180, 0x00e0, 0x00f8, 0x003c
  },
  {		/* ori rotated 90.0000 degrees */
   0x0000, 0x0000, 0x003f, 0x007f, 0x00e0, 0x01c0, 0x63e0, 0xffff,
   0xffff, 0x63e0, 0x01c0, 0x00e0, 0x007f, 0x003f, 0x0000, 0x0000
  },
  {		/* ori rotated 112.5000 degrees */
   0x003c, 0x00f8, 0x00e0, 0x0180, 0x0182, 0x03df, 0x03fc, 0x03f0,
   0x2fe0, 0x7fe1, 0xf8ff, 0x705e, 0x0008, 0x0000, 0x0000, 0x0000
  },
  {		/* ori rotated 135.0000 degrees */
   0x01e0, 0x0380, 0x0708, 0x061c, 0x0638, 0x0770, 0x07e1, 0x03c3,
   0x07e7, 0x07fe, 0x1efc, 0x3c00, 0x3c00, 0x1800, 0x0000, 0x0000
  },
  {		/* ori rotated 157.5000 degrees */
   0x0620, 0x0c30, 0x0c61, 0x1c63, 0x0ce3, 0x07c7, 0x0fe6, 0x07fe,
   0x03f8, 0x03e0, 0x0300, 0x0700, 0x0e00, 0x0f00, 0x0e00, 0x0400
  },
  {		/* ori rotated 180.0000 degrees */
   0x318c, 0x318c, 0x318c, 0x318c, 0x318c, 0x3bdc, 0x1ff8, 0x0ff0,
   0x07e0, 0x03c0, 0x0180, 0x0180, 0x0180, 0x03c0, 0x03c0, 0x0180
  },
  {		/* ori rotated 202.5000 degrees */
   0x0460, 0x0c30, 0x8630, 0xc638, 0xc730, 0xe3e0, 0x67f0, 0x7fe0,
   0x1fc0, 0x07c0, 0x00c0, 0x00e0, 0x0070, 0x00f0, 0x0070, 0x0020
  },
  {		/* ori rotated 225.0000 degrees */
   0x0780, 0x01c0, 0x10e0, 0x3860, 0x1c60, 0x8ee0, 0x87e0, 0xc3e0,
   0xe7c0, 0x7fe0, 0x3ef8, 0x003c, 0x003c, 0x0018, 0x0000, 0x0000
  },
  {		/* ori rotated 247.5000 degrees */
   0x3c00, 0x1f00, 0x0700, 0x0180, 0x4180, 0xfbc0, 0x3fc0, 0x0fc0,
   0x07f4, 0x87fe, 0xff1f, 0x7a0e, 0x1000, 0x0000, 0x0000, 0x0000
  },
  {		/* ori rotated 270.0000 degrees */
   0x0000, 0x0000, 0xfc00, 0xfe00, 0x0700, 0x0380, 0x07c6, 0xffff,
   0xffff, 0x07c6, 0x0380, 0x0700, 0xfe00, 0xfc00, 0x0000, 0x0000
  },
  {		/* ori rotated 292.5000 degrees */
   0x0000, 0x0000, 0x0000, 0x1000, 0x7a0e, 0xff1f, 0x87fe, 0x07f4,
   0x0fc0, 0x3fc0, 0xfbc0, 0x4180, 0x0180, 0x0700, 0x1f00, 0x3c00
  },
  {		/* ori rotated 315.0000 degrees */
   0x0000, 0x0000, 0x0018, 0x003c, 0x003c, 0x3f78, 0x7fe0, 0xf7e0,
   0xc3c0, 0x87e0, 0x8ee0, 0x1c60, 0x38e0, 0x10e0, 0x00c0, 0x0380
  },
  {		/* ori rotated 337.5000 degrees */
   0x0020, 0x0070, 0x00f0, 0x0070, 0x00e0, 0x00c0, 0x07c0, 0x1fc0,
   0x7fe0, 0x67f0, 0xe3e0, 0xc730, 0xc638, 0x8630, 0x0c30, 0x0460
  },
};

/**** These are the explosions ****/

#define EX_FRAMES		5
#define ex_width	64
#define ex_height	64

static short ex_bits[5][256] = {
    {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x0002, 0x0000,
	0x0000, 0x1000, 0x001a, 0x0000, 0x0000, 0x2800, 0x001c, 0x0000,
	0x0000, 0x0000, 0x000b, 0x0000, 0x0000, 0x0a00, 0x0008, 0x0000,
	0x0000, 0xe000, 0x000f, 0x0000, 0x0000, 0x9400, 0x0015, 0x0000,
	0x0000, 0xd500, 0x0075, 0x0000, 0x0000, 0x9500, 0x0022, 0x0000,
	0x0000, 0x6000, 0x001b, 0x0000, 0x0000, 0xf400, 0x001e, 0x0000,
	0x0000, 0x1000, 0x0001, 0x0000, 0x0000, 0xa000, 0x0026, 0x0000,
	0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x8000, 0x0006, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    }, {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0004, 0x0000,
	0x0000, 0x0000, 0x0104, 0x0000, 0x0000, 0x0200, 0x0080, 0x0000,
	0x0000, 0x0000, 0x0140, 0x0000, 0x0000, 0x0480, 0x0010, 0x0000,
	0x0000, 0x0000, 0x0082, 0x0000, 0x0000, 0x0000, 0x0044, 0x0000,
	0x0000, 0x0004, 0x0080, 0x0000, 0x0000, 0x0088, 0x0000, 0x0000,
	0x0000, 0xa000, 0x00c0, 0x0000, 0x0000, 0xa800, 0x0025, 0x0000,
	0x0000, 0x0020, 0x0212, 0x0000, 0x0000, 0x8100, 0x0202, 0x0000,
	0x0000, 0x6000, 0x2c12, 0x0000, 0x0000, 0x9222, 0x1200, 0x0000,
	0x0000, 0x4000, 0x0004, 0x0000, 0x0000, 0x4212, 0x0800, 0x0000,
	0x0000, 0x2800, 0x0048, 0x0000, 0x0000, 0x2000, 0x0247, 0x0000,
	0x0000, 0x2820, 0x0048, 0x0000, 0x0000, 0x4200, 0x0118, 0x0000,
	0x0000, 0x0100, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0400, 0x0400, 0x0000, 0x0000, 0xc000, 0x0014, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000,
	0x0000, 0x0000, 0x0028, 0x0000, 0x0000, 0x4000, 0x0014, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   }, {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x2008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0040, 0x0800, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000,
	0x0000, 0x0000, 0x2080, 0x0000, 0x0000, 0x0084, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0800, 0x0000, 0x0000, 0x0000, 0x0004, 0x0000,
	0x0000, 0x0000, 0x0208, 0x0000, 0x0800, 0x0000, 0x0800, 0x0000,
	0x0000, 0x0004, 0x0000, 0x0000, 0x1000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x5000, 0x0900, 0x0000,
	0x0000, 0x9100, 0x0009, 0x0000, 0x0000, 0x0001, 0x2004, 0x0000,
	0x0000, 0x4000, 0x4044, 0x0000, 0x0000, 0x8010, 0x0000, 0x0000,
	0x0000, 0x3000, 0x8044, 0x0010, 0x0400, 0x0040, 0x0000, 0x0005,
	0x0000, 0x8801, 0x4000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000,
	0x0000, 0x4040, 0x0000, 0x0000, 0x4400, 0x2000, 0x0000, 0x0002,
	0x0000, 0x1200, 0x0400, 0x0000, 0x0000, 0x0000, 0x4022, 0x0000,
	0x0000, 0x0800, 0x0412, 0x0000, 0x0000, 0x0101, 0x0200, 0x0000,
	0x0000, 0x1000, 0x2090, 0x0000, 0x0000, 0x2040, 0x0030, 0x0000,
	0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0000, 0x0000,
	0x0000, 0x0000, 0x8000, 0x0000, 0x0000, 0xc000, 0x0048, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000,
	0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0x0110, 0x0000,
	0x0000, 0x2000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   }, {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x2000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002,
	0x0000, 0x0008, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x2000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002,
	0x0000, 0x0000, 0x0200, 0x0000, 0x4000, 0x0010, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8008, 0x0000,
	0x0000, 0x0000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000,
	0x0000, 0x0000, 0x8000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000,
	0x4000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x1000, 0x0000, 0x0000, 0x4800, 0x8000, 0x0000,
	0x0000, 0x0040, 0x0800, 0x0000, 0x0000, 0x4400, 0x0011, 0x0000,
	0x0000, 0x0000, 0x0008, 0x0000, 0x0800, 0x0000, 0x0100, 0x0004,
	0x0000, 0x4002, 0x0008, 0x0004, 0x0000, 0x8000, 0x0000, 0x0000,
	0x0000, 0x2000, 0x0208, 0x0810, 0x0000, 0x0800, 0x0000, 0x0040,
	0x0008, 0x0008, 0x0000, 0x0100, 0x0400, 0x8200, 0x0000, 0x0008,
	0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x2000, 0x0000, 0x0000,
	0x0000, 0x0008, 0x0000, 0x0040, 0x0208, 0x1000, 0x0000, 0x0000,
	0x0000, 0x0800, 0x2000, 0x0000, 0x0000, 0x0080, 0x0080, 0x0000,
	0x0000, 0x0000, 0x0004, 0x0008, 0x0000, 0x0400, 0x2022, 0x0000,
	0x0000, 0x0040, 0x1000, 0x0000, 0x0400, 0x0800, 0x0040, 0x0000,
	0x0000, 0x0000, 0x0200, 0x0002, 0x0000, 0x1008, 0x00c0, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0004, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010,
	0x0000, 0x6000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0110, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x2000, 0x0000, 0x0000, 0x0000, 0x3000, 0x0000,
	0x0000, 0x0000, 0x0440, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0120, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   }, {
	0x0000, 0x0000, 0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0040,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0008,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002,
	0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0000, 0x0800, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0400, 0x0002, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0008,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x0000,
	0x0000, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000,
	0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x2400, 0x4000, 0x0008, 0x0000, 0x0010, 0x0000, 0x0000,
	0x0000, 0x4200, 0x0021, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000,
	0x0040, 0x0000, 0x0000, 0x0080, 0x0000, 0x0000, 0x0400, 0x0080,
	0x2000, 0x2000, 0x0010, 0x0000, 0x0000, 0x8000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0800, 0x0200, 0x0000, 0x0200, 0x0008, 0x1000,
	0x8000, 0x0000, 0x0000, 0x4000, 0x0000, 0x0080, 0x0000, 0x0100,
	0x0020, 0x8000, 0x0000, 0x0080, 0x0000, 0x0000, 0x0020, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1001, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000, 0x1000, 0x0008, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0400, 0x0000, 0x0003, 0x0000, 0x0020, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0208, 0x0100, 0x0000, 0x0000, 0x0000, 0x0002,
	0x0000, 0x0100, 0x0044, 0x0000, 0x0000, 0x0010, 0x8000, 0x0000,
	0x0020, 0x0000, 0x0100, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0900, 0x0020, 0x0000, 0x0801, 0x0080, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x2000, 0x0000, 0x0004, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0200,
	0x0000, 0x5000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    }
};

Bitmap	expview[EX_FRAMES];

/**** These are the misc things ****/

Bitmap fedview[VIEWS];
Bitmap romview[VIEWS];
Bitmap kliview[VIEWS];
Bitmap oriview[VIEWS];
Bitmap cloud;
Bitmap etorp, mtorp;
Bitmap bplanet;
Bitmap mbplanet;
Bitmap shield;

Cursor crosshair;

#define cloud_width 7
#define cloud_height 7
static short cloud_bits[] = {
   0x0014, 0x002a, 0x0055, 0x002a,
   0x0055, 0x002a, 0x0014};

#define etorp_width 3
#define etorp_height 3
static short etorp_bits[] = {
   0x0005, 0x0002, 0x0005};

#define mtorp_width 3
#define mtorp_height 3
static short mtorp_bits[] = {
   0x0002, 0x0007, 0x0002};

#define crossw 15
#define crossh 15
static short crossbits[] = {
   0x0080, 0x0080, 0x0080, 0x0080,
   0x0080, 0x0080, 0x0080, 0x7fff,
   0x0080, 0x0080, 0x0080, 0x0080,
   0x0080, 0x0080, 0x0080};

#define crossmask_width 15
#define crossmask_height 15
static short crossmask_bits[] = {
   0x01c0, 0x01c0, 0x01c0, 0x01c0,
   0x01c0, 0x01c0, 0x7fff, 0x7fff,
   0x7fff, 0x01c0, 0x01c0, 0x01c0,
   0x01c0, 0x01c0, 0x01c0};

#define planet_width 30
#define planet_height 30
static short planet_bits[] = {
   0xf800, 0x0003, 0x0700, 0x001c,
   0x00c0, 0x0060, 0x0020, 0x0080,
   0x0010, 0x0100, 0x0008, 0x0200,
   0x0004, 0x0400, 0x0004, 0x0400,
   0x0002, 0x0800, 0x0002, 0x0800,
   0x0002, 0x0800, 0x0001, 0x1000,
   0x0001, 0x1000, 0x0001, 0x1000,
   0x0001, 0x1000, 0x0001, 0x1000,
   0x0001, 0x1000, 0x0001, 0x1000,
   0x0002, 0x0800, 0x0002, 0x0800,
   0x0002, 0x0800, 0x0004, 0x0400,
   0x0004, 0x0400, 0x0008, 0x0200,
   0x0010, 0x0100, 0x0020, 0x0080,
   0x00c0, 0x0060, 0x0700, 0x001c,
   0xf800, 0x0003, 0x0000, 0x0000};

#define mplanet_width 16
#define mplanet_height 16
static short mplanet_bits[] = {
   0x03e0, 0x0c18, 0x1004, 0x2002,
   0x2002, 0x4001, 0x4001, 0x4001,
   0x4001, 0x4001, 0x2002, 0x2002,
   0x1004, 0x0c18, 0x03e0, 0x0000};

#define shield_width 20
#define shield_height 20
static short shield_bits[] = {
   0x3fc0, 0x0000, 0xc030, 0x0000,
   0x0008, 0x0001, 0x0004, 0x0002,
   0x0002, 0x0004, 0x0002, 0x0004,
   0x0001, 0x0008, 0x0001, 0x0008,
   0x0001, 0x0008, 0x0001, 0x0008,
   0x0001, 0x0008, 0x0001, 0x0008,
   0x0001, 0x0008, 0x0001, 0x0008,
   0x0002, 0x0004, 0x0002, 0x0004,
   0x0004, 0x0002, 0x0008, 0x0001,
   0xc030, 0x0000, 0x3fc0, 0x0000};

Pixmap	e_bplanetPix, a_bplanetPix;
Pixmap	e_mbplanetPix, a_mbplanetPix;

#define icon_width 70
#define icon_height 70
static short icon_bits[] = {
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0020, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0040,
   0x0020, 0x0000, 0x0000, 0x0000,
   0x0040, 0x0010, 0x0000, 0x0000,
   0x0000, 0x0080, 0x0010, 0x0000,
   0x0000, 0x0000, 0x0080, 0x0008,
   0x0000, 0x0000, 0x0000, 0x0100,
   0x0008, 0x0000, 0x0000, 0x0000,
   0x0100, 0x0004, 0x0000, 0x0000,
   0x0000, 0x0200, 0x0004, 0x0000,
   0x0000, 0x0000, 0x0200, 0x0002,
   0x0000, 0x0000, 0x0000, 0x0400,
   0x0002, 0x0000, 0x0000, 0x0000,
   0x0400, 0x0001, 0x0000, 0x0000,
   0x0000, 0x0800, 0x0001, 0x0000,
   0x0000, 0x0000, 0x8800, 0x0000,
   0x0000, 0x0000, 0x0000, 0x9000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x5000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x6000, 0xfc00, 0x7f3f,
   0x0000, 0x0000, 0x2000, 0x0000,
   0xc101, 0x09fc, 0x0002, 0x6000,
   0x0000, 0x8101, 0x0804, 0x0002,
   0x5000, 0x0000, 0x8101, 0x0804,
   0x0003, 0x9000, 0x0000, 0x8101,
   0x0804, 0x0001, 0x8800, 0x0000,
   0xc101, 0x8804, 0x0001, 0x0800,
   0x0001, 0x6101, 0x8804, 0x0000,
   0x0400, 0x0001, 0x3f01, 0x7804,
   0x0000, 0x0400, 0x0002, 0x1101,
   0x88fc, 0x0000, 0x0200, 0x0002,
   0x1101, 0x8804, 0x0003, 0x0200,
   0x0004, 0x3101, 0x0804, 0x0002,
   0x0100, 0x0004, 0x2101, 0x0804,
   0x0002, 0x0100, 0x0008, 0x4101,
   0x0804, 0x0004, 0x0080, 0x0008,
   0x4101, 0x0804, 0x0004, 0x0080,
   0x0010, 0x8101, 0x0804, 0x0004,
   0x0040, 0x0010, 0x8101, 0x09fc,
   0x0004, 0x0040, 0x0020, 0x0000,
   0x0000, 0x0000, 0x0020, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000, 0x0000, 0x0000,
   0x0000, 0x0000};