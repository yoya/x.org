#include <Xr/xr-copyright.h>

/* $Header: keycode.h,v 7.0 86/11/13 08:32:47 rick Exp $ */
/* Copyright 1986, Hewlett-Packard Company */
/* Copyright 1986, Massachussetts Institute of Technology */


/*************************************<+>*************************************
 *****************************************************************************
 **
 **   File:        keycode.h
 **
 **   Project:     X-ray Toolbox
 **
 **   Description: 
 **         Input translation codes
 **
 **
 **   ------------------------ MODIFICATION RECORD   ------------------------
 *
 * $Log:	keycode.h,v $
 * Revision 7.0  86/11/13  08:32:47  08:32:47  rick ()
 * Final QA release
 * 
 * Revision 6.0  86/11/10  15:19:51  15:19:51  rick ()
 * QA #2 release
 * 
 * Revision 5.1  86/11/07  13:59:06  13:59:06  rick ()
 * Added the copyright message.
 * 
 * Revision 5.0  86/10/28  08:25:44  08:25:44  rick ()
 * QA #1.1 release
 * 
 * Revision 4.1  86/10/24  09:02:35  09:02:35  karen
 * update to make ALT and 16 BIT handling more general, remove CAPSLOCK
 * 
 * Revision 3.0  86/10/02  16:01:16  16:01:16  rick ()
 * Alpha release set to 3.0
 * 
 * Revision 1.3  86/10/02  15:12:35  15:12:35  fred ()
 * Modified the keymap.
 * 
 * Revision 1.2  86/09/30  12:07:51  12:07:51  fred ()
 * Removed rcsid string definition.
 * 
 * Revision 1.1  86/09/30  11:35:35  11:35:35  fred ()
 * Initial revision
 * 
 *
 *****************************************************************************
 *************************************<+>*************************************/



struct kbd_state {
    unsigned char k_iso7to8[128];
    unsigned char k_iso8to7[256];
    unsigned int flags;	/* Control & State Flags (Read/Write) */
    char type;		/* type of keyboard, HP or DEFAULT */
    char language;	/* Keyboard Nationality Language (Read/Write) */
    char pwr_language;	/* Keyboard Language At Power Up */
    unsigned char alt;	/* Method to access alternate keymap */
    unsigned char i16bit; /* Method to access 16 bit input */
    unsigned char *langtab; /* Pointer To Language Dependent Key Table */
    unsigned char *alttab; /* Pointer To Alternate Language Key Table */
    unsigned int last_char; /* Used for Muting */
};

#define	CURS_DN_KEY	0x01		/* start of funcs encoded by term0 */
#define	CURS_LF_KEY	0x02
#define	CURS_UP_KEY	0x03
#define	CURS_RT_KEY	0x04
#define	ROLL_DN_KEY	0x05
#define	ROLL_LF_KEY	0x06
#define	ROLL_UP_KEY	0x07
#define	ROLL_RT_KEY	0x08
#define TAB_KEY		0x09
#define BACKTAB_KEY	0x0A
#define NEXT_KEY	0x0B	
#define PREV_KEY	0x0C	
#define HOME_UP_KEY	0x0D	
#define HOME_DN_KEY	0x0E	
#define DEL_CHAR_KEY	0x0F	
#define INSERT_CHAR_KEY	0x10	
#define DEL_LINE_KEY	0x11	
#define INSERT_LINE_KEY	0x12	
#define CLR_DISP_KEY	0x13	
#define CLR_LINE_KEY	0x14	
#define CLR_ALL_DISP_KEY 0x15	
#define CLR_ALL_LINE_KEY 0x16	
#define BS_KEY		0x17		/*BS_KEY is last func encoded by term0*/

#define BLANK_1_KEY     0x20
#define BLANK_2_KEY     0x21
#define BLANK_3_KEY     0x22
#define BLANK_4_KEY     0x23
#define BLANK_5_KEY     0x24
#define BLANK_6_KEY     0x25
#define BLANK_7_KEY     0x26
#define BLANK_8_KEY     0x27
#define SELECT_KEY	0x28	
#define SH_SELECT_KEY	0x29	
#define BREAK_KEY	0x2A	
#define RESET_KEY	0x2B	
#define STOP_KEY	0x2C	
#define SH_STOP_KEY	0x2D	
#define MENU_KEY	0x2E	
#define SYS_KEY		0x2F	
#define USER_KEY	0x30	
#define PRINT_KEY	0x31	
#define SHIFT_KEY	0x32
#define CONTROL_KEY	0x33
#define	CAPS_KEY	0x34
#define TO_ALT_KEY	0x35
#define TO_ROMAN_KEY	0x36
#define KANJI_LF_KEY    0x37
#define KANJI_RT_KEY    0x38
#define GO_16BIT_KEY	0x39
#define LEAVE_16BIT_KEY	0x3A
#define HELP_KEY	0x3B		/* on DEC keyboard */
#define DO_KEY		0x3C		/* on DEC keyboard */
#define FIND_KEY	0x3D		/* on DEC keyboard */
#define ENTER_KEY	0x3E		/* ENTER_KEY + 1 = RETURN_KEY */
#define RETURN_KEY	0x3F		/* RETURN_KEY + 1 = F1_KEY */
#define F1_KEY		0x40		/* F1_KEY | 0x010 = WF1_KEY */
#define F2_KEY		0x41		/* F1_KEY - F16_KEY contiguous */
#define F3_KEY		0x42
#define F4_KEY		0x43
#define F5_KEY		0x44
#define F6_KEY		0x45
#define F7_KEY		0x46
#define F8_KEY		0x47
#define F9_KEY		0x48
#define F10_KEY		0x49
#define F11_KEY		0x4A
#define F12_KEY		0x4B
#define F13_KEY		0x4C
#define F14_KEY		0x4D
#define F15_KEY		0x4E
#define F16_KEY		0x4F
#define WF1_KEY		0x50		/* WF1_KEY & 0xFEF = F1_KEY */
#define WF2_KEY		0x51		/* WF1_KEY - WF16_KEY contiguous */
#define WF3_KEY		0x52
#define WF4_KEY		0x53
#define WF5_KEY		0x54
#define WF6_KEY		0x55
#define WF7_KEY		0x56
#define WF8_KEY		0x57
#define WF9_KEY		0x58
#define WF10_KEY	0x59
#define WF11_KEY	0x5A
#define WF12_KEY	0x5B
#define WF13_KEY	0x5C
#define WF14_KEY	0x5D
#define WF15_KEY	0x5E
#define WF16_KEY	0x5F
#define CF1_KEY		0x60		/* CF1_KEY - CF16_KEY contiguous */
#define CF2_KEY		0x61
#define CF3_KEY		0x62
#define CF4_KEY		0x63
#define CF5_KEY		0x64
#define CF6_KEY		0x65
#define CF7_KEY		0x66
#define CF8_KEY		0x67
#define CF9_KEY		0x68
#define CF10_KEY	0x69
#define CF11_KEY	0x6A
#define CF12_KEY	0x6B
#define CF13_KEY	0x6C
#define CF14_KEY	0x6D
#define CF15_KEY	0x6E
#define CF16_KEY	0x6F

/* ROMAN8 Characters Greater Than 127 */
#define	R_A_FQUOTE	161
#define	R_A_HAT		162
#define	R_E_FQUOTE	163
#define	R_E_HAT		164
#define	R_E_DDOT	165
#define	R_I_HAT		166
#define	R_I_DDOT	167
#define	R_BQUOTE	168
#define	R_FQUOTE	169
#define	R_HAT		170
#define	R_DDOT		171
#define	R_TILTA		172
#define	R_U_FQUOTE	173
#define	R_U_HAT		174
#define	R_DPOUND	175

#define	R_OVERBAR	176
#define	R_Y_BQUOTE	177
#define	R_y_BQUOTE	178
#define	R_DEGREE	179
#define	R_C_BEARD	180
#define	R_c_BEARD	181
#define	R_N_TILTA	182
#define	R_n_TILTA	183
#define	R_i		184
#define	R_UQUES		185
#define	R_OX		186
#define	R_SPOUND	187
#define	R_YEN		188
#define	R_SO		189
#define	R_F		190
#define	R_CENT		191

#define	R_a_HAT		192
#define	R_e_HAT		193
#define	R_o_HAT		194
#define	R_u_HAT		195
#define	R_a_BQUOTE	196
#define	R_e_BQUOTE	197
#define	R_o_BQUOTE	198
#define	R_u_BQUOTE	199
#define	R_a_FQUOTE	200
#define	R_e_FQUOTE	201
#define	R_o_FQUOTE	202
#define	R_u_FQUOTE	203
#define	R_a_DDOT	204
#define	R_e_DDOT	205
#define	R_o_DDOT	206
#define	R_u_DDOT	207

#define	R_A_DOT		208
#define	R_i_HAT		209
#define	R_ZERO		210
#define	R_AE		211
#define	R_a_DOT		212
#define	R_i_BQUOTE	213
#define	R_zero		214
#define	R_ae		215
#define	R_A_DDOT	216
#define	R_i_FQUOTE	217
#define	R_O_DDOT	218
#define	R_U_DDOT	219
#define	R_E_BQUOTE	220
#define	R_i_DDOT	221
#define	R_BETA		222
#define	R_O_HAT		223

#define	R_A_BQUOTE	224
#define	R_A_TILTA	225
#define	R_a_TILTA	226
#define	R_D_CROSS	227
#define	R_d_CROSS	228
#define	R_I_BQUOTE	229
#define	R_I_FQUOTE	230
#define	R_O_BQUOTE	231
#define	R_O_FQUOTE	232
#define	R_O_TILTA	233
#define	R_o_TILTA	234
#define	R_S_V		235
#define	R_s_V		236
#define	R_U_BQUOTE	237
#define	R_Y_DDOT	238
#define	R_y_DDOT	239

#define	R_IP		240
#define	R_ip		241
#define	R_MIDDOT	242
#define	R_MICRO		243
#define	R_PILCROW	244
#define	R_THREEFOURTH	245
#define	R_MIDLINE	246
#define	R_ONEFOURTH	247
#define	R_ONEHALF	248
#define	R_AUSCORE	249
#define	R_OUSCORE	250
#define	R_LEFTSHIFT	251
#define	R_BLACK		252
#define	R_RIGHTSHIFT	253
#define	R_PLUSMINUS	254

/* Definition of Language Fields */

#define	K_HP_USASCII	0	/* (0) HP ITF United States     */
#define	K_HP_BELGIAN	1	/* (1) HP ITF Belgian           */
#define	K_HP_CANENG	2	/* (2) HP ITF Canadian English  */
#define	K_HP_DANISH	3	/* (3) HP ITF Danish           */
#define	K_HP_DUTCH	4	/* (4) HP ITF Dutch            */
#define	K_HP_FINNISH	5	/* (5) HP ITF Finnish          */
#define	K_HP_FRENCH	6	/* (6) HP ITF French (AZERTY)  */
#define	K_HP_CANFRENCH	7	/* (7) HP ITF Canadian French  */
#define	K_HP_SWISSFRENCH 8	/* (8) HP ITF Swiss French     */
#define	K_HP_GERMAN	9	/* (9) HP ITF German           */
#define	K_HP_SWISSGERMAN 10	/* (10) HP ITF Swiss German     */
#define	K_HP_ITALIAN	11	/* (11) HP ITF Italian          */
#define	K_HP_NORWEGIAN	12	/* (12) HP ITF Norwegian        */
#define	K_HP_EUROSPANISH 13	/* (13) HP ITF European Spanish */
#define	K_HP_LATSPANISH	14	/* (14) HP ITF Latin Spanish    */
#define	K_HP_SWEDISH	15	/* (15) HP ITF Swedish          */
#define	K_HP_UNITEDK	16	/* (16) HP ITF United Kingdom   */
#define	K_HP_KATAKANA	17	/* (17) HP ITF Katakana         */
#define	K_HP_SWISSFRENCH2 18	/* (18) HP ITF Swiss French II  */
#define	K_HP_SWISSGERMAN2 19	/* (19) HP ITF Swiss German II  */
#define	K_HP_KANJI	20	/* (20) HP ITF Kanji		*/


/* Keyboard Control/State Flag Bits and Configuration Parameters */

/* Enable Bits (Default in Parenthesis) */

#define	K_ASCII8	0x0001	/* 8 bit (versus ISO 7 bit) (1)          */
#define	K_MUTE		0x0002	/* Non-advancing Diacriticals
				   (8 bit only) (Lg. Dependent)          */
#define	K_ALT_ENABLE	0x0004	/* Alternate Language Keyboard (Lg. Dependent)*/
#define	K_ALT		0x0008	/* Alternate Keyboard Mode (Read Only)(0)*/
#define	K_16BITIO_ENABLE 0x0010 /* Enable 16 bit Input (Lg. Dependent)   */
#define	K_16BITIO	0x0020  /* 16 bit Input Mode (Read Only)(0)      */
#define	K_TYPE		0x0040	/* Modify keyboard type (keyboard dependent) */
#define	K_LANGUAGE	0x0080  /* Modify Language (keyboard dependent)  */
#define	K_LOCALMAP	0x0100  /* User has a .Xkeymap to use 		 */

#define	K_FUNCTION	0x080	/* function code follows */

#define K_s		0x8000	/* indicates a function code in the tables */

#define K_HP		1
#define K_DEFAULT	2

/* Special Key Defines */

#define	K_ILLEGAL	0

#define FIRST_LK201	86
#define FIRST_CHAR	105
#define CHAR_SHIFT	61
#define CODE_SHIFT	166		/* CHAR_SHIFT + FIRST_CHAR */

#define K_LANG_START	160
#define K_LANG_END	213

/*	160-213						*/
#define	K_LANG0		193		/* ` (1) */
#define	K_LANG1		194		/* \ (2) */
#define	K_LANG2		195		/* 1 (80) */
#define	K_LANG3		196		/* 2 (81) */
#define	K_LANG4		197		/* 3 (82) */
#define	K_LANG5		198		/* 4 (83) */
#define	K_LANG6		199		/* 5 (84) */
#define	K_LANG7		200		/* 6 (85) */
#define	K_LANG8		201		/* 7 (86) */
#define	K_LANG9		202		/* 8 (87) */
#define	K_LANG10	203		/* 9 (88) */
#define	K_LANG11	204		/* 0 (89) */
#define	K_LANG12	167		/* - (90) */
#define	K_LANG13	169		/* = (91) */
#define	K_LANG14	173		/* [ (92) */
#define	K_LANG15	175		/* ] (93) */
#define	K_LANG16	177		/* ; (94) */
#define	K_LANG17	179		/* ' (95) */
#define	K_LANG18	205		/* , (96) */
#define	K_LANG19	206		/* . (97) */
#define	K_LANG20	185		/* / (98) */
#define	K_LANG21	188		/* q (104) */
#define	K_LANG22	190		/* w (105) */
#define	K_LANG23	171		/* y (109) */
#define	K_LANG24	192		/* a (112) */
#define	K_LANG25	207		/* m (119) */
#define	K_LANG26	181		/* z (120) */
#define	K_LANG27	208		/* ~ (Shift-1) */
#define	K_LANG28	209		/* | (Shift-2) */

#define	K_LANG30	210		/* ! (Shift-80) */
#define	K_LANG31	160		/* @ (Shift-81) */
#define	K_LANG32	186		/* # (Shift-82) */
#define	K_LANG33	211		/* $ (Shift-83) */
#define	K_LANG34	212		/* % (Shift-84) */
#define	K_LANG35	161		/* ^ (Shift-85) */
#define	K_LANG36	162		/* & (Shift-86) */
#define	K_LANG37	163		/* * (Shift-87) */
#define	K_LANG38	164		/* ( (Shift-88) */
#define	K_LANG39	165		/* ) (Shift-89) */
#define	K_LANG40	166		/* _ (Shift-90) */
#define	K_LANG41	168		/* + (Shift-91) */
#define	K_LANG42	172		/* { (Shift-92) */
#define	K_LANG43	174		/* } (Shift-93) */
#define	K_LANG44	176		/* : (Shift-94) */
#define	K_LANG45	178		/* " (Shift-95) */
#define	K_LANG46	182		/* < (Shift-96) */
#define	K_LANG47	183		/* > (Shift-97) */
#define	K_LANG48	184		/* ? (Shift-98) */
#define	K_LANG49	187		/* Q (Shift-104) */
#define	K_LANG50	189		/* W (Shift-105) */
#define	K_LANG51	170		/* Y (Shift-109) */
#define	K_LANG52	191		/* A (Shift-112) */
#define	K_LANG53	213		/* M (Shift-119) */
#define	K_LANG54	180		/* Z (Shift-120) */

/* Keyboard status register constants */
#define KEYSHIFTLOCK	0x0800  /* shift lock */
#define KEYSHIFT	0x1000	/* key was shifted     */
#define KEYMETA		0x2000	/* meta key (extend) */
#define KEYCNTL		0x4000	/* key was control key */
#define CNTLKEY_MASK	0x1f	/* maps keys to control key range */
