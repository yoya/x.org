/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/
#if	RCSID && IN_MAIN
static char *trcsid="$Header: catseye.h,v 1.1 87/12/16 13:22:46 hp Exp $";
#endif	RCSID

/*
 *  topcat.h	- access to topcat chip
 *
 *  This module is part of the low-level graphics primitives 
 *  for the Hewlett-Packard 9000 Series 300.
 *
 *  Hewlett-Packard Instrument Systems Lab - Loveland, Colorado
 *
 *  (c) Copyright 1986 Hewlett-Packard Company.  All rights are
 *  reserved.  Copying or other reproduction of the program except
 *  for archival purposes is prohibited without prior written
 *  consent of Hewlett-Packard Company.
 *
 * $Log:	catseye.h,v $
 * Revision 1.1  87/12/16  13:22:46  hp
 * Initial revision
 * 
 * Revision 1.1  87/12/14  14:36:26  hp
 * Initial revision
 * 
 * Revision 1.1  87/12/03 11:08:37 GMT  peter
 * Initial revision
 * 
 * 
 * 
 */
 
#include <sys/types.h>
/*
#include <sys/graphics.h>
*/

/*
 * list of topcat external procedures
 */

extern Bool catseyeCreateGC();
extern void catseyeCopyArea();
extern void catseyeMover();

/*
 * Map of the topcat chip in memory ...
 */

typedef struct {
    u_short id_reset;		/* id and reset register 	*/	/* 0x00&1 */
    u_char filler2;
    u_char interrupts;		/* interrupts			*/	/* 0x03	*/
    u_char filler2a;
    u_char t_memwide;		/* top half frame buf width	*/	/* 0x05 */
    u_char filler2b;
    u_char b_memwide;		/* bottom half frame buf width	*/	/* 0x07 */
    u_char filler2c;
    u_char t_memhigh;		/* top half frame buf height 	*/	/* 0x09 */
    u_char filler2d;
    u_char b_memhigh;		/* bot half frame buf height	*/	/* 0x0b */
    u_char filler2e;
    u_char t_dispwide;		/* top half display width	*/	/* 0x0d	*/
    u_char filler2f;
    u_char b_dispwide;		/* bot half display width	*/	/* 0x0f */
    u_char filler2g;
    u_char t_disphigh;		/* top half display height	*/	/* 0x11 */
    u_char filler2h;
    u_char b_disphigh;		/* bot half display height	*/	/* 0x13 */
    u_char filler2i[3];
    u_char bits;		/* 0 square pixels, 1 double hi */	/* 0x17 */
    u_char filler3[67];
    u_char num_planes;		/* number of color planes	*/	/* 0x5b */
    u_char id_font[16356];	/* display id, font, ...	*/

/* plane status registers (read only) at 0x0040*/

    u_char vert_blank;		/* vertical blanking		*/	/* 0x4040 */
    u_char filler01[3];
    u_char move_active;		/* window move active		*/
    u_char filler02[3];
    u_char vert_request;	/* vert retrace intr request	*/
    u_char filler03[3];
    u_char move_request;	/* window move intr request	*/   
    u_char filler04[51];

/* plane control registers (read/write) at 0x0080 */

    u_char nblank;		/* video output enable		*/	/* 0x4080 */
    u_char filler4[3];
    u_char enable_sync;		/* sync enable			*/
    u_char filler5[3];
    u_char write_enable;	/* enable writes		*/
    u_char filler6[3];
    u_char read_enable;		/* enable reads			*/
    u_char filler7[3];
    u_char frame_buf_write_enable; /* frame buffer write enable	*/
    u_char filler8[3];
    u_char vert_enable;		/* vertical retrace int enable	*/
    u_char filler9[3];
    u_char move_enable;		/* window move interrupt enable */
    u_char filler10[3];
    u_char  start_move;		/* start window move		*/
    u_char filler11[3];
    u_char blink;		/* blink control		*/
    u_char filler12[11];
    u_char cursor_on;		/* turn cursor on		*/
    u_char filler13[61];

/* raster op control registers (read/write, word wide) at 0x00ea */

    u_char    pixel_write_replacement_rule;				/* 0x40ea */
    u_char fillerA[4];
    u_char    window_move_replacement_rule;	/* this is 0x40ef */
    u_char fillerB[1];
    u_short source_x;
    u_short fillerC;
    u_short source_y;
    u_short fillerD;
    u_short dest_x;
    u_short fillerE;
    u_short dest_y;
    u_short fillerF;
    u_short window_width;
    u_short fillerG;
    u_short window_height;
    u_short fillerH;
    u_short cursor_x;
    u_short fillerI;
    u_short cursor_y;
    u_short fillerJ;
    u_short cursor_length;
    u_char filler99[7919];

/* color map status (color systems only) at 0x2002 */

    u_char  colormap_status;	/* bit 2 set if RAM is busy	*/	/* 0x6003 */
    u_char  filler999[172];

/* color map registers (color systems only) at 0x20B2 */		/* 0x60b2 */

    u_short filler1A;
    u_short red_data;
    u_short green_data;
    u_short blue_data;
    u_short color_index;
    u_short plane_mask;
    u_char  filler1E[52];
    u_short colormap_writestrobe;
} CATSEYE;

/* additional info for topcat control. */
#define IDREG	(0x1)
#define INTREG	(0x3)
#define RAI0	(0x23)
#define RAI1	(0x27)
#define RAI2	(0x2b)
#define RAI3	(0x2f)

/* private field for topcat display */
typedef struct {
    CATSEYE  *catseyeDev;        /* pointer to device hardware */
    ColormapPtr InstalledMap;  /* pointer to installed colormap */
} catseyePriv;
typedef catseyePriv *catseyePrivPtr;

#define waitbusy(pMask)	while (pMask & gp_hardware->move_active)	\
			{ int i; for (i=0;i<20;i++); }

#define getGpHardware(pScreen)						\
    (((catseyePrivPtr)((cfbPrivScreenPtr)((pScreen)->devPrivate))	\
       ->pHardwareScreen)->catseyeDev)

#define getPlanesMask(pScreen)						\
    (((cfbPrivScreenPtr)((pScreen)->devPrivate))->planesMask)

#ifndef	GXclear

/* defines for replacement rules -- same as for X window system */

#define	GXclear		0x0		/* 0 			*/
#define GXand		0x1		/* src AND dst 		*/
#define GXandReverse	0x2		/* src AND NOT dst	*/
#define GXcopy		0x3		/* src 			*/
#define GXandInverted	0x4		/* NOT src AND dst	*/
#define GXnoop		0x5		/* dst			*/
#define GXxor		0x6		/* src XOR dst		*/
#define GXor		0x7		/* src OR dst		*/
#define GXnor		0x8		/* NOT src AND NOT dst	*/
#define GXequiv		0x9		/* NOT src XOR dst	*/
#define GXinvert	0xa		/* NOT dst		*/
#define GXorReverse	0xb		/* src OR NOT dst	*/
#define GXcopyInverted	0xc		/* NOT src		*/
#define GXorInverted	0xd		/* NOT src OR dst	*/
#define GXnand		0xe		/* NOT src OR NOT dst	*/
#define GXset		0xf		/* 1			*/

#endif	GXclear

#ifdef	TEST_DEFINES

#include <stdio.h>

CATSEYE t;

main ()
{
    int T;

    T = (int) &t;

    printf ("          \tGot\tWanted\n");
    printf ("catseye at:\t0x%x\t0x%x\n", T);
    printf ("vert_blank at:\t0x%x\t0x%x\n", &t.vert_blank,  0x4040 + T);
    printf ("nblank at:\t0x%x\t0x%x\n", &t.nblank,  0x4080 + T);
    printf ("cursor_on at:\t0x%x\t0x%x\n", &t.cursor_on,  0x40ac + T);
    printf ("pixel_wrr at:\t0x%x\t0x%x\n", &t.pixel_write_replacement_rule,  0x40ea + T);
    printf ("cursor_len at:\t0x%x\t0x%x\n", &t.cursor_length,  0x4112 + T);
    printf ("cmap_status at:\t0x%x\t0x%x\n", &t.colormap_status,  0x6003 + T);
    printf ("red_data at:\t0x%x\t0x%x\n", &t.red_data,  0x60b2 + T);
    printf ("green_data at:\t0x%x\t0x%x\n", &t.green_data,  0x60b4 + T);
    printf ("blue_data at:\t0x%x\t0x%x\n", &t.blue_data,  0x60b6 + T);
    printf ("write_str at:\t0x%x\t0x%x\n", &t.colormap_writestrobe,  0x60f0 + T);
    fflush (stdout);
    exit (0);
}

#endif	TEST_DEFINES
