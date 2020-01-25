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
static char *trcsid="$Header: ren.h,v 1.2 88/02/06 15:28:24 rws Exp $";
#endif	RCSID
/*
 * $Log:	ren.h,v $
 * Revision 1.2  88/02/06  15:28:24  rws
 * changes from HP
 * 
 * Revision 1.4  88/01/19  10:04:06  10:04:06  brianw (Brian Wilson)
 * modified waitbusy
 * 
 * Revision 1.3  87/12/21  17:59:34  17:59:34  harry ()
 * Merging with MIT sources
 * 
 * 
 * Revision 1.1  87/12/16  13:34:30  hp
 * Initial revision
 * 
 * Revision 1.1  87/12/14  14:47:13  hp
 * Initial revision
 * 
 * Revision 1.2  87/11/04 14:31:42 GMT  andreas
 * graphics.h is doubly included (by ren.h and ren.c)
 * removed include of graphics.h
 * 
 */

 
#include <sys/types.h>
/*
#include <sys/graphics.h>
*/

/*
 * list of renaissance external procedures
 */

extern Bool renCreateGC();
extern void renCopyArea();
extern void renMover();

/*
 * Map of the Renaissance control space chip in memory ...
 */

/*
 * Map of the Renaissance frame buffer controller chip in memory ...
 */

typedef struct {
    u_char	filler1[1];
    u_char	reset;		/** reset register		**//** 0x01 **/
    u_char	fb_address;	/** frame buffer address 	**//** 0x02 **/
    u_char	interrupt;	/** interrupt register		**//** 0x03 **/

    u_char	filler2[16451];

  /** Window Mover registers & Frame buffer control **/   

    u_char	wbusy;		/** window mover is active	**//** 0x4047**/
    u_char      filler3[0x405b - 0x4048];
    u_char	scanbusy;	/** scan converteris active	**//** 0x405B**/
    u_char      filler3b[0x4083 - 0x405c];
    u_char	video_enable;   /** lets drive vid. refresh bus **//** 0x4083**/
    u_char	filler4[3];
    u_char	display_enable;	/** enable the display		**//** 0x4087**/
    u_char	filler5[8];
    u_int	write_enable;	/** write enable register	**//** 0x4090**/
    u_char 	filler6[11];
    u_char	wmove;		/** start window mover		**//** 0x409f**/
    u_char	filler7[3];
    u_char	blink;		/** blink register		**//** 0x40a3**/
    u_char	filler8[15];
    u_char	fold;		/** fold  register		**//** 0x40b3**/
    u_int	opwen;		/** overlay plane write enable	**//*0x40b4..7*/
    u_char	filler9[3];
    u_char	tmode;		/** Tile mode size		**//** 0x40bb**/
    u_char	filler9a[3];		
    u_char	drive;		/** drive register		**//** 0x40bf**/
    u_char 	filler10[3];
    u_char	vdrive;		/** vdrive register		**//** 0x40c3**/
    u_char 	filler10a[0x40cb-0x40c4];
    u_char	zconfig;	/** Z-buffer mode		**//** 0x40cb**/
    u_char	filler11a[2];
    u_short	tpatt;		/** Transparency pattern	**//** 0x40ce**/
    u_char	filler11b[3];
    u_char	dmode;		/** dither mode			**//** 0x40d3**/
    u_char	filler11c[3];
    u_char	en_scan;	/** enable scan board to DTACK	**//** 0x40d7**/
    u_char	filler11d[0x40ef-0x40d8];
    u_char	rep_rule;	/** replacement rule		**//** 0x40ef**/
    u_char 	filler12[2];
    u_short	source_x;	/** source x			**//** 0x40f2**/
    u_char 	filler13[2];
    u_short	source_y;	/** source y			**//** 0x40f6**/
    u_char 	filler14[2];
    u_short	dest_x;		/** dest x			**//** 0x40fa**/
    u_char 	filler15[2];
    u_short	dest_y;		/** dest y			**//** 0x40fe**/
    u_char 	filler16[2];
    u_short	window_width;	/** window width		**//** 0x4102**/
    u_char 	filler17[2];
    u_short	window_height;	/** window height		**//** 0x4106**/
    u_char	filler18[18];
    u_short	patt_x;		/** pattern x			**//** 0x411a**/
    u_char	filler19[2];
    u_short	patt_y;		/** pattern y			**//** 0x411e**/
    u_char	filler20[0x8012 - 0x4120];
    u_short	te_status;	/** transform engine status	**//** 0x8012**/
    u_char	filler21[0x1ffff-0x8014];
    u_char	acebusy;	/** ace busy                    **//**0x1ffff**/
} RENAISSANCE;

/* private field for Renaissance display */
typedef struct {
    RENAISSANCE  *renDev;      /* pointer to device hardware */
    ColormapPtr InstalledMap;  /* pointer to installed colormap */
} renPriv;
typedef renPriv *renPrivPtr;

#define waitbusy(gp_hardware)	while ((gp_hardware)->wbusy) \
					{ int i; for (i=0; i<20; i++); }

#define getGpHardware(pScreen)						\
    (((renPrivPtr)((cfbPrivScreenPtr)((pScreen)->devPrivate))	\
       ->pHardwareScreen)->renDev)

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

#define		FOLDED		1
#define		UNFOLDED	0

#define		CM_INVERSE	0	/** use a inverted linear color map **/
#define		CM_LINEAR	1	/** use a linear color map	    **/
#define		CM_GAMMA	2	/** use a gamma corrected color map **/

#define		CM_MAPPED_0	0 	/** board 0 drives rgb				**/
#define		CM_MAPPED_1	1 	/** board 1 drives rgb				**/
#define		CM_UNMAPPED	2 	/** 0 drives blue, 1 drives green, 2 drives red **/


#define 	CM_STATUS		0x6007

#define		CM_BLINK_MASK		0x20
#define		CM_OVL_PRESENT_MASK	0x10
#define		CM_NOT_COPY_MASK	0x04
#define		CM_HBLANK_MASK		0x02
#define		CM_VSYNC_MASK		0x01

#define		RED1	0x6403
#define		RED2	0x7403
#define		GREEN1	0x6803
#define		GREEN2	0x7803
#define		BLUE1	0x6c03
#define		BLUE2	0x7c03

#define		OVERLAY1	0x63c3
#define		OVERLAY2 	0x73c3

#define		ren_cm_busy()		(*(u_char *)(gp_hardware + CM_STATUS) & CM_NOT_COPY_MASK)


#define		ren_cm_set_component(c,i,v)	(*(unsigned char *)(gp_hardware + c + (i<<2)) = v)

#define		ren_cm_set_red(i,v)	(ren_cm_set_component(RED1,i,v),	\
					 ren_cm_set_component(RED2,i,v))
#define		ren_cm_set_green(i,v)	(ren_cm_set_component(GREEN1,i,v),	\
					 ren_cm_set_component(GREEN2,i,v))
#define		ren_cm_set_blue(i,v)	(ren_cm_set_component(BLUE1,i,v),	\
					 ren_cm_set_component(BLUE2,i,v))
#define		ren_cm_set_overlay(i,v)	(ren_cm_set_component(OVERLAY1,i,v),	\
					 ren_cm_set_component(OVERLAY2,i,v))

/* Driver types */

#define HP98720  1	/* hp98720A without using Transform engine */
#define HP98721  2	/* hp98720A using Transform engine */
#define HP98720W 4	/* hp98720A using overlay planes */
#define HP98720X 8	/* hp98720A using overlay planes as main FB RAM */
#define REN_DEEP 0xb	/* hp98720A using main FB RAM */
#define REN_OVER 0x4	/* hp98720A using overlay planes */

#define transparentq(r, g, b)	((r) == 0x1234 && (g) == 0x5678 && (b) == 0x9abc)
/* #define transparentq(r, g, b)	(((r) & 0x8000) == 0x8000 && ((g) & 0x8000) == 0x8000 && ((b) & 0x8000) == 0x0000) */
/* #define transparentq(r, g, b)	0 */
/* #define transparentq(r, g, b)	(! ((r) | (g) | (b))) */

#define fb_mover_config(src_x,src_y,dst_x,dst_y,width,height)	\
		(hardware -> source_x = src_x,			\
		 hardware -> source_y = src_y,			\
		 hardware -> dest_x = dst_x,			\
		 hardware -> dest_y = dst_y,			\
		 hardware -> window_width = width,		\
		 hardware -> window_height = height)
