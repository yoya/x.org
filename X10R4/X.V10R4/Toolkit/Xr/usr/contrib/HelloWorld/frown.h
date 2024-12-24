UINT16 frownRaster[] =
{
   0xffff, 0x01ff, 0xfe00, 0xffff, 
   0xffff, 0x003f, 0xe000, 0xffff, 
   0xffff, 0xfc0f, 0xc1ff, 0xffff, 
   0xffff, 0xff83, 0x0fff, 0xffff, 
   0xffff, 0xfff0, 0x3fff, 0xfffc, 
   0x3fff, 0xfffc, 0xffff, 0xfff8, 
   0x1fff, 0xffff, 0xffff, 0xfff1, 
   0xcfff, 0xffff, 0xffff, 0xfff3, 
   0xe7ff, 0xffff, 0xffff, 0xffe7, 
   0xf3ff, 0xffff, 0xffff, 0xffcf, 
   0xf9ff, 0xffff, 0xffff, 0xff8f, 
   0xfcff, 0xffff, 0xffff, 0xff9f, 
   0xfc7f, 0xfe1f, 0xf87f, 0xff3f, 
   0xfe7f, 0xf9e7, 0xe79f, 0xff3f, 
   0xff3f, 0xf7fb, 0xdfef, 0xfe7f, 
   0xff3f, 0xeffd, 0xbff7, 0xfe7f, 
   0xff3f, 0xeffd, 0xbff7, 0xfcff, 
   0xff9f, 0xdffe, 0x7ffb, 0xfcff, 
   0xff9f, 0xdffe, 0x7ffb, 0xf9ff, 
   0xff9f, 0xc1fe, 0x07fb, 0xf9ff, 
   0xffcf, 0xc0fe, 0x03fb, 0xf9ff, 
   0xffcf, 0xe07d, 0x81f7, 0xf3ff, 
   0xffcf, 0xe03d, 0x80f7, 0xf3ff, 
   0xffcf, 0xf03b, 0xc0ef, 0xf3ff, 
   0xffcf, 0xf827, 0xe09f, 0xf3ff, 
   0xffcf, 0xfe1f, 0xf87f, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0xffff, 0xffff, 0xf3ff, 
   0xffcf, 0x07ff, 0xff00, 0xf3ff, 
   0xffcf, 0xf8ff, 0xf8ff, 0xfbff, 
   0xffdf, 0xff1f, 0xe7ff, 0xfbff, 
   0xff9f, 0xffe7, 0x9fff, 0xf9ff, 
   0xff9f, 0xfffb, 0x7fff, 0xf9ff, 
   0xff3f, 0xfffc, 0xffff, 0xfcfe, 
   0x7f3f, 0xffff, 0xffff, 0xfcfd, 
   0xbf3f, 0xffff, 0xffff, 0xfefb, 
   0xde7f, 0xffff, 0xffff, 0xfe77, 
   0xfe7f, 0xffff, 0xffff, 0xff3f, 
   0xfcff, 0xffff, 0xffff, 0xff3f, 
   0xf8ff, 0xffff, 0xffff, 0xff9f, 
   0xf9ff, 0xffff, 0xffff, 0xffcf, 
   0xf3ff, 0xffff, 0xffff, 0xffc7, 
   0xe7ff, 0xffff, 0xffff, 0xffe7, 
   0xcfff, 0xffff, 0xffff, 0xfff3, 
   0x8fff, 0xffff, 0xffff, 0xfff9, 
   0x1fff, 0xffff, 0xffff, 0xfffc, 
   0x3fff, 0xfffe, 0x3fff, 0xfffe, 
   0x7fff, 0xfffc, 0x1fff, 0xffff, 
   0xffff, 0xfff0, 0xc3ff, 0xffff, 
   0xffff, 0xffe1, 0xe0ff, 0xffff, 
   0xffff, 0xff03, 0xf83f, 0xffff, 
   0xffff, 0x000f, 0xfe00, 0xffff, 
   0xffff, 0x00ff, 0xffc0, 0xffff, 
   0xffff, 0xffff, 0xffff, 0xffff, 
   0xffff, 0xffff, 0xffff, 0xffff, 
   0xffff, 0xffff, 0xffff, 0xffff, 
   0xffff, 0xffff, 0xffff, 0xffff, 
   0xffff, 0xffff, 0xffff, 0xffff, 
   
};


xrPixmap frown = 
{
   64,			/*  raster width       */
   64,			/*  raster height      */
   1,			/*  raster depth       */
   frownRaster		/*  ptr to the raster  */
};
