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
/* $Header:ega_video.h 1.2$ */
/* $ACIS:ega_video.h 1.2$ */
/* $Source: /ibm/acis/usr/src/X11/server/ddx/ibm/ega/RCS/ega_video.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidega_video = "$Header:ega_video.h 1.2$";
#endif

#ifdef lint
#define volatile /**/
#endif

#define EGA 1

/*
 * References to all pc ( i.e. '286 ) memory in the
 * regions used by the ega server ( the 128K windows )
 * MUST be long-word ( i.e. 32-bit ) reads or writes.
 * This definition will change for other memory architectures
 */
typedef unsigned volatile char *EgaMemoryPtr ;

typedef unsigned char io86reg ;
typedef short int IO_Address ;

struct video_hardware_state {
	/* Address locations			READ	--	WRITE */
	/* General Registers */
	io86reg Misc_Output_Reg ;	/*	03CC	--	03C2 */
	io86reg Input_Status_0 ;	/*	03C2	--	XXXX */
	io86reg Input_Status_1 ;	/*	03?A	--	XXXX */
	io86reg Feature_Control ;	/*	03?C	--	03CA */
	io86reg Video_Enable ;		/*	03C3	--	SAME */

	/* Attribute Registers  03C0 & 03C1 */
	io86reg Attr_Addr_Reg ;		/*	03C0	--	SAME */
	/* io86reg ??????? */		/*	03C0	--	SAME */
	io86reg Palette[16] ;		/*	Attr_Addr_Reg == 00 - 0F */
	io86reg Attr_Mode ;		/*	Attr_Addr_Reg == 10 */
	io86reg Overscan_Color ;	/*	Attr_Addr_Reg == 11 */
	io86reg Color_Plane_En ;	/*	Attr_Addr_Reg == 12 */
	io86reg Horiz_PEL_Pan ;		/*	Attr_Addr_Reg == 13 */
	io86reg Color_Select ;		/*	Attr_Addr_Reg == 14 */

	/* Crt Controller Registers  03?4 & 03?5 */
	io86reg Index_Reg ;		/*	03?4	--	SAME */
	/* io86reg ??????? */		/*	03?5	--	SAME */
	io86reg Horiz_Total ;		/*	Index_Reg == 00   */
	io86reg Horiz_End ;		/*	Index_Reg == 01   */
	io86reg H_Blank_Start ;		/*	Index_Reg == 02   */
	io86reg H_Blank_End ;		/*	Index_Reg == 03   */
	io86reg H_Retrace_Start ;	/*	Index_Reg == 04   */
	io86reg H_Retrace_End ;		/*	Index_Reg == 05   */
	io86reg Vert_Total ;		/*	Index_Reg == 06   */
	io86reg Overflow ;		/*	Index_Reg == 07   */
	io86reg Preset_Row_Scan ;	/*	Index_Reg == 08   */
	io86reg Max_Scan_Line ;		/*	Index_Reg == 09   */
	io86reg Cursor_Start ;		/*	Index_Reg == 0A   */
	io86reg Cursor_End ;		/*	Index_Reg == 0B   */
	io86reg Start_Addr_Hi ;		/*	Index_Reg == 0C   */
	io86reg Start_Addr_Lo ;		/*	Index_Reg == 0D   */
	io86reg Cursor_Loc_Hi ;		/*	Index_Reg == 0E   */
	io86reg Cursor_Loc_Lo ;		/*	Index_Reg == 0F   */
	io86reg V_Retrace_Start ;	/*	Index_Reg == 10   */
	io86reg V_Retrace_End ;		/*	Index_Reg == 11   */
	io86reg V_Display_End ;		/*	Index_Reg == 12   */
	io86reg Underline_Loc ;		/*	Index_Reg == 13   */
	io86reg Offset ;		/*	Index_Reg == 14   */
	io86reg V_Blank_Start ;		/*	Index_Reg == 15   */
	io86reg V_Blank_End ;		/*	Index_Reg == 16   */
	io86reg CRTC_Mode ;		/*	Index_Reg == 17   */
	io86reg Line_Compare ;		/*	Index_Reg == 18   */

	/* Sequencer Registers  03C4 & 03C5 */
	io86reg Seq_Addr_Reg ;		/*	03C4	--	SAME */
	/* io86reg ??????? */		/*	03?5	--	SAME */
	io86reg Seq_Reset ;		/*	Seq_Addr_Reg == 00   */
	io86reg Clock_Mode ;		/*	Seq_Addr_Reg == 01   */
	io86reg Mask_Map ;		/*	Seq_Addr_Reg == 02   */
	io86reg Char_Map_Select ;	/*	Seq_Addr_Reg == 03   */
	io86reg Memory_Mode ;		/*	Seq_Addr_Reg == 04   */

	/* Graphics Registers  03CE & 03CF */
	io86reg Graphics_Addr ;		/*	03CE	--	SAME */
	/* io86reg ??????? */		/*	03CF	--	SAME */
	io86reg Set_Reset ;		/*	Graphics_Addr == 00   */
	io86reg Enb_Set_Reset ;		/*	Graphics_Addr == 01   */
	io86reg Color_Compare ;		/*	Graphics_Addr == 02   */
	io86reg Data_Rotate ;		/*	Graphics_Addr == 03   */
	io86reg Read_Map_Select ;	/*	Graphics_Addr == 04   */
	io86reg Graphics_Mode ;		/*	Graphics_Addr == 05   */
	io86reg Miscellaneous ;		/*	Graphics_Addr == 06   */
	io86reg Color_Dont_Care ;	/*	Graphics_Addr == 07   */
	io86reg Bit_Mask ;		/*	Graphics_Addr == 08   */

	/* Video DAC Registers  03CE & 03CF */
	io86reg PEL_WR_Addr ;		/*	03C8	--	SAME */
	io86reg PEL_RD_Addr ;		/*	XXXX	--	03C7 */
	io86reg DAC_State ;		/*	XXXX	--	03C7 */
	io86reg PEL_Data_Reg ;		/*	03C9	--	SAME */
	io86reg PEL_Mask_Reg ;		/*	03C6	--	SAME */
} ;

/*
 * ega video screen defines & macros
 */
#define SCREEN_ADDR	( 0x000B8000 )
#define NUMSCREENS 1
#define MAXPLANES 4
#define ALLPLANES 0xF
#define BYTES_PER_ROW 80
#define PIX_PER_BYTE 8
#define MAX_ROW	350
#define MAX_OFFSCREEN_ROW   408
#define MAX_COLUMN ( ( BYTES_PER_ROW * PIX_PER_BYTE ) - 1 )

#define ROW_OFFSET( PIXEL_X_VALUE ) ( ( PIXEL_X_VALUE ) >> 3 )
#define BIT_OFFSET( PIXEL_X_VALUE ) ( ( PIXEL_X_VALUE ) & 0x7 )
#define BYTE_OFFSET( PIXEL_X_VALUE, PIXEL_Y_VALUE ) \
	( ( ( PIXEL_Y_VALUE ) * BYTES_PER_ROW ) + ( ( PIXEL_X_VALUE ) >> 3 ) )

#ifdef iAPX286
#define EGABASE ( (volatile unsigned char *) ( SCREEN_ADDR ) )
extern io86reg inb( ) ;
extern void outb( ) ;
extern io86reg inw( ) ;
extern void outw( ) ;
extern int GO_BIOS_VIDEO_MODE( ) ;
extern void INTS_ON() ;
extern void INTS_OFF() ;
#else
extern int ega_screen_file ; /* Program GLOBAL File Descriptor for /dev/ega */
#define INTS_ON() /**/
#define INTS_OFF() /**/
#define EGABASE ( (volatile unsigned char *) \
	( 0xF4000000 | ( SCREEN_ADDR & 0x000fffff ) ) )
/* Hardware Registers */
#define ATTRIBUTE_ADDR	( (volatile unsigned char *) 0xf00003da )
#define ATTRIBUTE_REG	( (volatile unsigned char *) 0xf00003c0 )
#define DAC_ADDR 	( (volatile unsigned char *) 0xf00003c8 )
#define DAC_DATA 	( (volatile unsigned char *) 0xf00003c9 )

#define inb( pc_io_address ) ( ( *( (volatile unsigned char *) \
	( 0xF0000000 + ( ( pc_io_address ) ) ) ) ) )
#define inw( pc_io_address ) ( ( *( (volatile unsigned short int *) \
	( 0xF0000000 + ( ( pc_io_address ) ) ) ) ) )
#define outb( pc_io_address, value ) ( ( *( (volatile unsigned char *) \
	( 0xF0000000 + ( pc_io_address ) ) ) = ( value ) ) )
#define outw( pc_io_address, value ) ( ( *( (volatile unsigned short int *) \
	( 0xF0000000 + ( ( pc_io_address ) ) ) ) = ( value ) ) )
#define GO_BIOS_VIDEO_MODE( mode ) ioctl( ega_screen_file, BUFINITEGA, &mode )
#endif
