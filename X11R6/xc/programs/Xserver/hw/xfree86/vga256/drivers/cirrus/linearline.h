/* $XConsortium: linearline.h,v 1.1 95/01/06 20:37:49 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga256/drivers/cirrus/linearline.h,v 3.0 1994/12/25 12:35:15 dawes Exp $ */

/* linearline.c */
extern void LinearFramebufferVerticalLine(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int
#endif
);

extern void LinearFramebufferDualVerticalLine(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int,
    int
#endif
);

extern void LinearFramebufferSlopedLineLeft(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int,
    int,
    int
#endif
);

extern void LinearFramebufferSlopedLineRight(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int,
    int,
    int
#endif
);

extern void LinearFramebufferSlopedLineVerticalLeft(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int,
    int,
    int
#endif
);

extern void LinearFramebufferSlopedLineVerticalRight(
#if NeedFunctionPrototypes
    unsigned char *,
    int,
    int,
    int,
    int,
    int
#endif
);
