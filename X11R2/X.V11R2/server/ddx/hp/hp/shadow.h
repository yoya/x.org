/* 
 * shadow.h : all the extern defs for the cursor shadow layer
 */
 
	/* cfb shadow */
extern int cfbpolytext8(), cfbpolytext16();
extern unsigned int *cfbgetspans();
extern void
  cfbsetspans(), cfbputimage(), cfbgetimage(), cfbcopyarea(),
  cfbcopyplane(), cfbpolypoint(), cfbpolylines(), cfbpolysegment(),
  cfbpolyrectangle(), cfbpolyarc(), cfbfillpolygon(), cfbpolyfillrect(),
  cfbpolyfillarc(), cfbimagetext8(), cfbimagetext16(), cfbimageglyphblt(),
  cfbpolyglyphblt(), cfbpushpixels(), cfbunnaturaltilefs(),
  cfbunnaturalstipplefs(), cfbsolidfs() ;

	/* mi shadow */
extern int mipolytext8(), mipolytext16();
extern unsigned int *migetspans();
extern void
  mifillspans(), misetspans(), miputimage(), migetimage(), micopyarea(),
  micopyplane(), mipolypoint(), mipolylines(), mipolysegment(),
  mipolyrectangle(), mipolyarc(), mifillpolygon(), mipolyfillrect(),
  mipolyfillarc(), miimagetext8(), miimagetext16(), miimageglyphblt(),
  mipolyglyphblt(), mipushpixels(), miwideline(), miwidedash(), mizeroline();

	/* mfb shadow */
extern int mfbpolytext8(), mfbpolytext16();
extern unsigned int *mfbgetspans();
extern void
  mfbfillspans(), mfbsetspans(), mfbputimage(), mfbgetimage(), mfbcopyarea(),
  mfbcopyplane(), mfbpolypoint(), mfbpolylines(), mfbpolysegment(),
  mfbpolyrectangle(), mfbpolyarc(), mfbfillpolygon(), mfbpolyfillrect(),
  mfbpolyfillarc(), mfbimagetext8(), mfbimagetext16(), mfbimageglyphblt(),
  mfbpolyglyphblt(), mfbpushpixels();

	/* misc */
extern void
  hpzerodash(), hpfbcopyarea(), hpfbpolyfillrect(),
  topcatzeroline(), topcatzerodash(), tcimageglyphblt(), tcpolyfillsolidrect(),
  hpgetbyteimage(), hpputbyteimage();

	/* the window shadow */
extern void
  cfbpaintareanone(), cfbpaintareapr(),  cfbpaintarea32(), mipaintwindow(),
  tcpaintareasolid(), cfbpaintareapr(), cfbpaintareaother(), cfbcopywindow();
