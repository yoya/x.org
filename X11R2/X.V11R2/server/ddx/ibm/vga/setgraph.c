/***********************************************************
		Copyright IBM Corporation 1988

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/* $Header:setgraph.c 1.7$ */
/* $ACIS:setgraph.c 1.7$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/vga/RCS/setgraph.c,v $ */

#ifndef lint
static char *rcsid = "$Header:setgraph.c 1.7$";
#endif

#include "vga_video.h"
#include <sys/file.h>
#include <sys/ioctl.h>
#define ATR
#include <machinecons/buf_emul.h>
#undef ATR
#include <machinecons/xio.h>

#include "X.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "cursorstr.h"

#include "rtio.h"
#include "rtscreen.h"

/* Video State On Program Entry */
static struct video_hardware_state VS_Start ;
static DAC_TABLE init_dac_table ;

void vgaSetColor( color, r, g, b )
register int color ;
register short r, b, g ;
{
outb( 0x3C8, color ) ; /* Point PEL Address Register To Color Entry */
outb( 0x3C9, r >> 10 ) ;
outb( 0x3C9, g >> 10 ) ;
outb( 0x3C9, b >> 10 ) ;

return ;
}

void save_dac( tablePtr )
register DAC_TABLE *tablePtr ;
{
register int i ;
register unsigned char *cptr ;

outb( 0x3C7, 0x0 ) ; /* Point PEL Address Register To First Entry */
for ( i = 768, cptr = (unsigned char *) tablePtr ; --i ; )
	*cptr++ = inb( 0x03C9 ) ;
return ;
}

void restore_dac( tablePtr )
register DAC_TABLE *tablePtr ;
{
register int i ;
register unsigned char *cptr ;

outb( 0x3C8, 0x0 ) ; /* Point PEL Address Register To First Entry */
for ( i = 768, cptr = (unsigned char *) tablePtr ; --i ; )
	outb( 0x03C9, *cptr++ ) ;
return ;
}

/*
 * Initialize the vga to 640 x 480, 16 of 64 colors @ a0000
 */

void set_graphics_mode()
{
/* Setup I/O Base Address */
/* Color 640 by 480 -- 16 Color */
#define Color_Base_Reg ( 0x3D0 )

/* Sequencer Registers  03C4 & 03C5 */
outb( 0x3C4, 0x00 ) ;
outb( 0x3C5, /* VS.Seq_Reset */ 0x1 ) ; /* -- Hardware Syncronous RESET */
outb( 0x3C4, 0x01 ) ;
outb( 0x3C5, /* VS.Clock_Mode */ 0x01 ) ;
outb( 0x3C4, 0x02 ) ;
outb( 0x3C5, /* VS.Mask_Map */ 0x0F ) ; /* All Planes Writable */
outb( 0x3C4, 0x03 ) ;
outb( 0x3C5, /* VS.Char_Map_Select */ 0x00 ) ;
outb( 0x3C4, 0x04 ) ;
outb( 0x3C5, /* VS.Memory_Mode */ 0x06 ) ;

/* Write General Registers */
/* VS.Input_Status_0 & VS.Input_Status_1 are READ-ONLY */
outb( 0x3C2, /* VS.Misc_Output_Reg */ 0xE3 ) ; /* VS.Misc_Output_Reg */
outb( Color_Base_Reg + 0xA, VS_Start.Feature_Control ) ;
outb( 0x3C3, /* VS.Video_Enable */ 0x1 ) ;

/* Re-Enable Hardware i.e. Reset Register */
outb( 0x3C4, 0x00 ) ;
outb( 0x3C5, /* VS.Seq_Reset */ 0x03 ) ; /* Enable Hardware Reset Register */

/* Attribute Registers */
INTS_OFF() ;
/* Initialize Flip-Flop */
{ register tmp = inb( Color_Base_Reg + 0xA ) ; }

/* Set Palette Register Value Equal To Palette Register Index Number */
/* i.e. Palette is 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F */
{
	register IO_Address Target_Reg ;
	for ( Target_Reg = 0 ; Target_Reg <= 0xF ; Target_Reg++ ) {
		outb( 0x3C0, Target_Reg ) ;
		outb( 0x3C0, Target_Reg ) ;
	}
}
/* Rest Of The Attribute Registers */
/* Note: 0x20 is added to the index */
outb( 0x3c0, 0x30 ) ;
outb( 0x3C0, 0x81 ) ; /* VS.Attr_Mode - P4,P5 for color select */
outb( 0x3c0, 0x31 ) ;
outb( 0x3C0, 0x00 ) ; /* VS.Overscan_Color */
outb( 0x3c0, 0x32 ) ;
outb( 0x3C0, 0x0F ) ; /* VS.Color_Plane_En */
outb( 0x3c0, 0x33 ) ;
outb( 0x3C0, 0x00 ) ; /* VS.Horiz_PEL_Pan */
outb( 0x3c0, 0x34 ) ;
outb( 0x3C0, 0x00 ) ; /* VS.Color_Select */	/*	Attr_Addr_Reg == 14 */
/* Re-Enable Video Access To The Color Palette */
INTS_ON() ;

/* Enable CRT Controller Registers 0-7 */
outb( 0x3D4, 0x11 ) ;
outb( 0x3D5, 0x0C ) ;
/* Set CRT Controller Registers  03?4 & 03?5 */
outb( 0x3D4, 0x00 ) ;
outb( 0x3D5, 95 ) ; /* VS.Horiz_Total */
outb( 0x3D4, 0x01 ) ;
outb( 0x3D5, 79 ) ; /* VS.Horiz_End */
outb( 0x3D4, 0x02 ) ;
outb( 0x3D5, 0x50 ) ; /* VS.H_Blank_Start */
outb( 0x3D4, 0x03 ) ;
outb( 0x3D5, /* VS.H_Blank_End */ 0x82 ) ;
outb( 0x3D4, 0x04 ) ;
outb( 0x3D5, /* VS.H_Retrace_Start */ 0x54 ) ;
outb( 0x3D4, 0x05 ) ;
outb( 0x3D5, /* VS.H_Retrace_End */ 0x80 ) ;
outb( 0x3D4, 0x06 ) ;
outb( 0x3D5, /* VS.Vert_Total */ 0x0B ) ;
outb( 0x3D4, 0x07 ) ;
outb( 0x3D5, /* VS.Overflow */ 0x3E ) ;
outb( 0x3D4, 0x08 ) ;
outb( 0x3D5, /* VS.Preset_Row_Scan */ 0 ) ;
outb( 0x3D4, 0x09 ) ;
outb( 0x3D5, /* VS.Max_Scan_Line */ 0x40 ) ;
outb( 0x3D4, 0x0A ) ;
outb( 0x3D5, /* VS.Cursor_Start */ 0 ) ;
outb( 0x3D4, 0x0B ) ;
outb( 0x3D5, /* VS.Cursor_End */ 0 ) ;
outb( 0x3D4, 0x0C ) ;
outb( 0x3D5, /* VS.Start_Addr_Hi */ 0 ) ;
outb( 0x3D4, 0x0D ) ;
outb( 0x3D5, /* VS.Start_Addr_Lo */ 0 ) ;
outb( 0x3D4, 0x0E ) ;
outb( 0x3D5, /* VS.Cursor_Loc_Hi */ 0 ) ;
outb( 0x3D4, 0x0F ) ;
outb( 0x3D5, /* VS.Cursor_Loc_Lo */ 0 ) ;
outb( 0x3D4, 0x10 ) ;
outb( 0x3D5, /* VS.V_Retrace_Start */ 234 ) ;
outb( 0x3D4, 0x11 ) ;
outb( 0x3D5, /* VS.V_Retrace_End */ 0x8C ) ;
outb( 0x3D4, 0x12 ) ;
outb( 0x3D5, /* VS.V_Display_End */ 223 ) ;
outb( 0x3D4, 0x13 ) ;
outb( 0x3D5, /* VS.Underline_Loc */ 40 ) ;
outb( 0x3D4, 0x14 ) ;
outb( 0x3D5, /* VS.Offset */ 0 ) ;
outb( 0x3D4, 0x15 ) ;
outb( 0x3D5, /* VS.V_Blank_Start */ 231 ) ;
outb( 0x3D4, 0x16 ) ;
outb( 0x3D5, /* VS.V_Blank_End */ 4 ) ;
outb( 0x3D4, 0x17 ) ;
outb( 0x3D5, /* VS.CRTC_Mode */ 0xE3 ) ;
outb( 0x3D4, 0x18 ) ;
outb( 0x3D5, /* VS.Line_Compare */ 255 ) ;

/* Set Graphics Registers  03CE & 03CF */
outb( 0x3CE, 0x00 ) ;
outb( 0x3CF, /* VS.Set_Reset */ 0x00 ) ;
outb( 0x3CE, 0x01 ) ;
outb( 0x3CF, /* VS.Enb_Set_Reset */ 0x00 ) ;
outb( 0x3CE, 0x02 ) ;
outb( 0x3CF, /* VS.Color_Compare */ 0x00 ) ;
outb( 0x3CE, 0x03 ) ;
outb( 0x3CF, /* VS.Data_Rotate */ 0x00 ) ;
outb( 0x3CE, 0x04 ) ;
outb( 0x3CF, /* VS.Read_Map_Select */ 0x00 ) ;
outb( 0x3CE, 0x05 ) ;
outb( 0x3CF, /* VS.Graphics_Mode */ 0x02 ) ; /* Set For Writing Individual Pixels */
outb( 0x3CE, 0x06 ) ;
outb( 0x3CF, /* VS.Miscellaneous */ 0x05 ) ;
outb( 0x3CE, 0x07 ) ;
outb( 0x3CF, /* VS.Color_Dont_Care */ 0x0F ) ;
outb( 0x3CE, 0x08 ) ;
outb( 0x3CF, /* VS.Bit_Mask */ 0xFF ) ; /* All Bits Writable */

/* Video DAC Registers  03C7 & 03C8 */
outb( 0x3C9, VS_Start.PEL_Data_Reg ) ;	/*	03C9	--	SAME */
outb( 0x3C6, VS_Start.PEL_Mask_Reg ) ;

return ;
}

#ifndef iAPX286
static int unix_vga_init()
{
register int fd ;
static const int mode = (int) MODE_124 ;
static unsigned page = SCREEN_ADDR ;

extern int open() ;
extern void perror() ;
extern void exit() ;
extern int ioctl() ;

/* Open Device File */
if ( ( fd = open( "/dev/vga", O_RDWR ) ) < 0 ) {
	perror( "unix_vga_init: can't open /dev/vga, exiting..." ) ;
	exit( 1 ) ;
}
/* Set 128k window to point to vga display buffer */
if ( ioctl( fd, BUFSETWIND, &page ) < 0 )
	perror( "unix_vga_init: vgaioctl, set window" ) ;
/* Set The Display Mode To Avoid Confusing The BIOS Later */
if ( ioctl( fd, BUFINITVGA, &mode ) < 0 )
	perror( "unix_vga_init: vgaioctl, vga init" ) ;

return fd ;
}
#endif

extern int vgaDisplayTubeType ;

int vgaScreenInitHW( index )
register int	index ;
{
static char been_here = 0 ;
static int vga_file_desc ;

extern void save_vga_state( ) ;

if ( !been_here ) {
#ifndef iAPX286
	rtScreenFD( index ) = vga_file_desc = unix_vga_init() ;
#endif
	/* Save Extant Video State & Setup For Graphics */
	save_vga_state( &VS_Start ) ;
	save_dac( &init_dac_table ) ;
	been_here = 1 ;
}
/* If The Display Is Turned Off Or Changed It Should Take Effect Now */
if ( ( vgaDisplayTubeType = vgaCheckDisplay( vga_file_desc ) ) > 0 ) {
	set_graphics_mode() ;
	return 4 ; /* Number Of Bit Planes */
}
else
	return 0 ; /* Error Condition */
}

void vgaCloseHW( index )
register int	index ;
{
extern void restore_vga_state( ) ;

restore_dac( &init_dac_table ) ;
restore_vga_state( &VS_Start ) ;

return ;
}
