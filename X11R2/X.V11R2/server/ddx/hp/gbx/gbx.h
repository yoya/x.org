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
/*
 * Structures for Hewlett-Packard 9000s300 Gatorbox display
 */

typedef struct {
    u_short id;			/* id and reset register 	*/	/* 0x01 */
    u_char sec_interrupt;	/* secondary interrupt reg. 	*/	/* 0x02	*/
    u_char interrupts;		/* interrupts			*/	/* 0x03	*/
    u_char filler2a[0x3ffd];
    u_char crtc_address;	/* CTR controller address reg	*/	/* 0x4001	*/
    u_char filler2b;
    u_char crtc_data;		/* CTR controller data reg	*/	/* 0x4003	*/
    u_char filler2c[0xffd];
    u_char width;		/* width in tiles		*/	/* 0x5001 */
    u_char	filler3[1];
    u_char height;		/* height in tiles		*/	/* 0x5003 */
    u_char	filler4[3];
    u_char rep_rule;		/* replacement rule		*/	/* 0x5007 */
    u_char 	filler5[4089];
    u_char blink1;		/* blink 1			*/ 	/* 0x6001 */
    u_char 	filler6[3];
    u_char blink2;		/* blink 2			*/ 	/* 0x6005 */
    u_char 	filler7[3];
    u_char write_protect;	/* write protect 		*/	/* 0x6009 */
    u_char	filler8[3];
    u_char cmap_id;		/* CMAP ID Register (read only) */	/* 0x600d */
    u_char	filler9[0x6803 - 0x600d - 1];
    u_char cmap_busy;		/* Color Map Busy Register (ro) */	/* 0x6803 */
    u_char	filler10[0x68b9 - 0x6803 - 1];
    u_char	cmap_pointer;	/* Pointer Register		*/	/* 0x68b9 */
    u_char	filler11[0x68f1 - 0x68b9 - 1];
    u_char	cmap_write;	/* Pointer Register		*/	/* 0x68f1 */
    u_char	filler12[0x68f9 - 0x68f1 - 1];
    u_char	cmap_read;	/* Pointer Register		*/	/* 0x68f9 */
    u_char	filler13[0x69b3 - 0x68f9 - 1];
    u_char	red_data;	/* Red Data Register (rw)	*/	/* 0x69b3 */
    u_char	filler14[1];
    u_char	green_data;	/* Green Data Register (rw)	*/	/* 0x69b5 */
    u_char	filler15[1];
    u_char	blue_data;	/* Blue Data Register (rw)	*/	/* 0x69b7 */
    u_char	filler16[0x10000 - 0x69b7 - 1];
    u_char	frame_buffer[1048576];					/*0x10000 */
} GbxDisplay;

#define GBX_BLOCK_MOVER_BUSY	0x10
#define GBX_TILE_MOVER_ENABLE	0x80
#define	GBX_MOVER_UP_LEFT	0x00
#define GBX_MOVER_DOWN_RIGHT	0x40

#define GBX_CMAP_BUSY		0x04
#define GBX_CRT_RETRACE		0x02
#define GBX_VSYNC		0x01

#define GbxWaitBusy()	while (gp_hardware->sec_interrupt & GBX_BLOCK_MOVER_BUSY);
/*
			{ int i; for (i=0;i<20;i++); } \
			while (gp_hardware->sec_interrupt & GBX_BLOCK_MOVER_BUSY) \
			{ int i; for (i=0;i<20;i++); } 
*/

#define GbxCMapWaitBusy(gbx) while ( (gbx)->cmap_busy & GBX_CMAP_BUSY ) ;

typedef struct {
    u_int source_x;
    u_int source_y;
    u_int dest_x;
    u_int dest_y;
    u_int window_width;
    u_int window_height;
    u_int rule;
    u_int mask;
} GBX_WINDOW_MOVER;

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

#define GBX_WIDTH	1024
#define GBX_HEIGHT	768

void GbxCMapWrite();
void GbxCMapRead();
