/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the keycodes of the Sun Swiss-French type 5 keyboard.
 * By Philippe A. Kipfer (Philippe.Kipfer@di.epfl.ch), dec.1993.
 * This keyboard meet the TUV standard.
 * Defined from Sun document "Notes claviers Sun type 5" ref. #800-6803-10
 *
 */

#ifndef  SunXK_Dead_Grave
# define SunXK_Dead_Grave	0x1005FF00	/* Same as SunXK_FA_Grave? */
#endif
#ifndef  SunXK_Dead_Circum
# define SunXK_Dead_Circum	0x1005FF01	/* Same as SunXK_FA_Circum? */
#endif
#ifndef  SunXK_Dead_Tilde
# define SunXK_Dead_Tilde	0x1005FF02	/* Same as SunXK_FA_Tilde? */
#endif
#ifndef  SunXK_FA_Acute
# define SunXK_FA_Acute		0x1005FF03
#endif
#ifndef  SunXK_FA_Diaeresis
# define SunXK_FA_Diaeresis	0x1005FF04
#endif

static struct default_keycode_map sun5swfr_ow_map [] = {
  {   8, 0,		XK_F11,	XK_F11,
			XK_Cancel },
  {   9, 0,		XK_KP_Divide },
  {  10, 0,		XK_F12,	XK_F12,
			XK_Redo },
  {  11, 0,		XK_KP_Multiply },
  {  12, 0,		XK_F1 },
  {  13, 0,		XK_F2 },
  {  14, 0,		XK_F10 },
  {  15, 0,		XK_F3 },
  {  16, 0,		SunXK_F36 },
  {  17, 0,		XK_F4 },
  {  18, 0,		SunXK_F37 },
  {  19, 0,		XK_F5 },
  {  20, Mod2Mask,	XK_Mode_switch },
  {  21, 0,		XK_F6 },
  {  22, 0,		XK_KP_Decimal },
  {  23, 0,		XK_F7 },
  {  24, 0,		XK_F8 },
  {  25, 0,		XK_F9 },
  {  26, Mod4Mask,	XK_Alt_L },
  {  27, 0,		XK_Up },
  {  28, 0,		XK_F21,	XK_F21,
			XK_Pause,	XK_Break },
  {  29, 0,		XK_F22,	XK_F22,
			XK_Print,	SunXK_Sys_Req },
  {  30, 0,		XK_F23,	XK_F23,
			XK_Scroll_Lock },
  {  31, 0,		XK_Left },
  {  32, Mod5Mask,	XK_F13,	XK_F13,
			SunXK_Props },
  {  33, 0,		XK_F14,	XK_F14,
			XK_Undo },
  {  34, 0,		XK_Down },
  {  35, 0,		XK_Right },
  {  36, 0,		XK_Escape },
  {  37, 0,		XK_1,	XK_plus,
			XK_bar },
  {  38, 0,		XK_2,	XK_quotedbl,
			XK_at },
  {  39, 0,		XK_3,	XK_asterisk,
			XK_numbersign },
  {  40, 0,		XK_4,	XK_ccedilla,
			XK_asciicircum },
  {  41, 0,		XK_5,	XK_percent,
			XK_asciitilde },
  {  42, 0,		XK_6,	XK_ampersand },
  {  43, 0,		XK_7,	XK_slash },
  {  44, 0,		XK_8,	XK_parenleft },
  {  45, 0,		XK_9,	XK_parenright },
  {  46, 0,		XK_0,	XK_equal,
			XK_grave },
  {  47, 0,		XK_apostrophe,	XK_question,
			SunXK_FA_Acute },
  {  48, 0,		SunXK_Dead_Circum,	SunXK_Dead_Grave,
			SunXK_Dead_Tilde },
  {  49, 0,		XK_section,	XK_degree },
  {  50, 0,		XK_BackSpace },
  {  51, 0,		XK_Insert },
  {  52, 0,		XK_KP_7,	XK_Home },
  {  53, 0,		XK_F25,	XK_F25,
			XK_KP_Divide },
  {  54, 0,		XK_F26,	XK_F26,
			XK_KP_Multiply },
  {  55, 0,		XK_KP_8 },
  {  56, 0,		XK_F15,	XK_F15,
			SunXK_Front },
  {  57, 0,		XK_Delete,	XK_Delete,
			XK_KP_Decimal },
  {  58, 0,		XK_F16,	XK_F16,
			SunXK_Copy },
  {  59, 0,		XK_Home },
  {  60, 0,		XK_Tab },
  {  61, 0,		XK_Q },
  {  62, 0,		XK_W },
  {  63, 0,		XK_E },
  {  64, 0,		XK_R },
  {  65, 0,		XK_T },
  {  66, 0,		XK_Z },
  {  67, 0,		XK_U },
  {  68, 0,		XK_I },
  {  69, 0,		XK_O },
  {  70, 0,		XK_P },
  {  71, 0,		XK_egrave,	XK_udiaeresis,
			XK_bracketleft },
  {  72, 0,		SunXK_FA_Diaeresis,	XK_exclam,
			XK_bracketright },
  {  73, 0,		XK_Delete },
  {  74, 0,		XK_Multi_key },
  {  75, 0,		XK_F27,	XK_F27,
			XK_KP_7,	XK_Home },
  {  76, 0,		XK_Up,	XK_F28,
			XK_KP_8 },
  {  77, 0,		XK_F29,	XK_F29,
			XK_KP_9,	XK_Prior },
  {  78, 0,		XK_F24,	XK_F24,
			XK_KP_Subtract },
  {  79, 0,		XK_F17,	XK_F17,
			SunXK_Open },
  {  80, Mod5Mask,	XK_F18,	XK_F18,
			SunXK_Paste },
  {  81, 0,		XK_End },
  {  82, 0,		XK_KP_9,	XK_Prior },
  {  83, ControlMask,	XK_Control_L },
  {  84, 0,		XK_A },
  {  85, 0,		XK_S },
  {  86, 0,		XK_D },
  {  87, 0,		XK_F },
  {  88, 0,		XK_G },
  {  89, 0,		XK_H },
  {  90, 0,		XK_J },
  {  91, 0,		XK_K },
  {  92, 0,		XK_L },
  {  93, 0,		XK_eacute,	XK_odiaeresis },
  {  94, 0,		XK_agrave,	XK_adiaeresis,
			XK_braceleft },
  {  95, 0,		XK_dollar,	XK_sterling,
			XK_braceright },
  {  96, 0,		XK_Return },
  {  97, 0,		XK_KP_Enter },
  {  98, 0,		XK_Left,	XK_F30,
			XK_KP_4 },
  {  99, 0,		XK_F31,	XK_F31,
			XK_KP_5 },
  { 100, 0,		XK_Right,	XK_F32,
			XK_KP_6 },
  { 101, 0,		XK_Insert,	XK_Insert,
			XK_KP_0 },
  { 102, 0,		XK_F19,	XK_F19,
			XK_Find },
  { 103, 0,		XK_Prior },
  { 104, Mod5Mask,	XK_F20,	XK_F20,
			SunXK_Cut },
  { 105, Mod3Mask,	XK_Num_Lock },
  { 106, ShiftMask,	XK_Shift_L },
  { 107, 0,		XK_Y },
  { 108, 0,		XK_X },
  { 109, 0,		XK_C },
  { 110, 0,		XK_V },
  { 111, 0,		XK_B },
  { 112, 0,		XK_N },
  { 113, 0,		XK_M },
  { 114, 0,		XK_comma,	XK_semicolon },
  { 115, 0,		XK_period,	XK_colon },
  { 116, 0,		XK_minus,	XK_underscore },
  { 117, ShiftMask,	XK_Shift_R },
  { 118, 0,		XK_KP_Subtract },
  { 119, 0,		XK_F33,	XK_F33,
			XK_KP_1,	XK_End },
  { 120, 0,		XK_Down,	XK_F34,
			XK_KP_2 },
  { 121, 0,		XK_F35,	XK_F35,
			XK_KP_3,	XK_Next },
  { 122, 0,		XK_KP_4 },
  { 123, 0,		XK_KP_5 },
  { 124, 0,		XK_KP_6 },
  { 125, 0,		XK_Help },
  { 126, LockMask,	XK_Caps_Lock },
  { 127, Mod1Mask,	XK_Meta_L },
  { 128, 0,		XK_space },
  { 129, Mod1Mask,	XK_Meta_R },
  { 130, 0,		XK_Next },
  { 131, 0,		XK_less,	XK_greater,
			XK_backslash },
  { 132, 0,		XK_KP_Add },
  { 133, 0,		XK_KP_0 },
  { 134, 0,		XK_KP_1,	XK_End },
  { 135, 0,		XK_KP_2 },
  { 136, 0,		XK_KP_3,	XK_Next },
  { 137, 0,		XK_nobreakspace,	XK_nobreakspace,
			XK_nobreakspace,	XK_nobreakspace },
  { 138, 0,		XK_exclamdown,	XK_exclamdown,
			XK_exclamdown,	XK_exclamdown },
  { 139, 0,		XK_brokenbar,	XK_brokenbar,
			XK_brokenbar,	XK_brokenbar },
  { 140, 0,		XK_section,	XK_section,
			XK_section,	XK_section },
  { 141, 0,		XK_section,	XK_section,
			XK_section,	XK_section },
  { 142, 0,		XK_diaeresis,	XK_diaeresis,
			XK_diaeresis,	XK_diaeresis },
  { 143, 0,		XK_copyright,	XK_copyright,
			XK_copyright,	XK_copyright },
  { 144, 0,		XK_copyright,	XK_copyright,
			XK_copyright,	XK_copyright },
  { 145, 0,		XK_ordfeminine,	XK_ordfeminine,
			XK_ordfeminine,	XK_ordfeminine },
  { 146, 0,		XK_ordfeminine,	XK_ordfeminine,
			XK_ordfeminine,	XK_ordfeminine },
  { 147, 0,		XK_guillemotleft,	XK_guillemotleft,
			XK_guillemotleft,	XK_guillemotleft },
  { 148, 0,		XK_notsign,	XK_notsign,
			XK_notsign,	XK_notsign },
  { 149, 0,		XK_notsign,	XK_notsign,
			XK_notsign,	XK_notsign },
  { 150, 0,		XK_hyphen,	XK_hyphen,
			XK_hyphen,	XK_hyphen },
  { 151, 0,		XK_registered,	XK_registered,
			XK_registered,	XK_registered },
  { 152, 0,		XK_registered,	XK_registered,
			XK_registered,	XK_registered },
  { 153, 0,		XK_macron,	XK_macron,
			XK_macron,	XK_macron },
  { 154, 0,		XK_paragraph,	XK_paragraph,
			XK_paragraph,	XK_paragraph },
  { 155, 0,		XK_mu,	XK_mu,
			XK_mu,	XK_mu },
  { 156, 0,		XK_degree,	XK_degree,
			XK_degree,	XK_degree },
  { 157, 0,		XK_periodcentered,	XK_periodcentered,
			XK_periodcentered,	XK_periodcentered },
  { 158, 0,		XK_plusminus,	XK_plusminus,
			XK_plusminus,	XK_plusminus },
  { 159, 0,		XK_division,	XK_division,
			XK_division,	XK_division },
  { 160, 0,		XK_onesuperior,	XK_onesuperior,
			XK_onesuperior,	XK_onesuperior },
  { 161, 0,		XK_twosuperior,	XK_twosuperior,
			XK_twosuperior,	XK_twosuperior },
  { 162, 0,		XK_threesuperior,	XK_threesuperior,
			XK_threesuperior,	XK_threesuperior },
  { 163, 0,		XK_onequarter,	XK_onequarter,
			XK_onequarter,	XK_onequarter },
  { 164, 0,		XK_onehalf,	XK_onehalf,
			XK_onehalf,	XK_onehalf },
  { 165, 0,		XK_threequarters,	XK_threequarters,
			XK_threequarters,	XK_threequarters },
  { 166, 0,		XK_multiply,	XK_multiply,
			XK_multiply,	XK_multiply },
  { 167, 0,		XK_acute,	XK_acute,
			XK_acute,	XK_acute },
  { 168, 0,		XK_cedilla,	XK_cedilla,
			XK_cedilla,	XK_cedilla },
  { 169, 0,		XK_masculine,	XK_masculine,
			XK_masculine,	XK_masculine },
  { 170, 0,		XK_masculine,	XK_masculine,
			XK_masculine,	XK_masculine },
  { 171, 0,		XK_guillemotright,	XK_guillemotright,
			XK_guillemotright,	XK_guillemotright },
  { 172, 0,		XK_questiondown,	XK_questiondown,
			XK_questiondown,	XK_questiondown },
  { 173, 0,		XK_aacute,	XK_aacute,
			XK_aacute,	XK_aacute },
  { 174, 0,		XK_agrave,	XK_agrave,
			XK_agrave,	XK_agrave },
  { 175, 0,		XK_adiaeresis,	XK_adiaeresis,
			XK_adiaeresis,	XK_adiaeresis },
  { 176, 0,		XK_acircumflex,	XK_acircumflex,
			XK_acircumflex,	XK_acircumflex },
  { 177, 0,		XK_atilde,	XK_atilde,
			XK_atilde,	XK_atilde },
  { 178, 0,		XK_aring,	XK_aring,
			XK_aring,	XK_aring },
  { 179, 0,		XK_ae,	XK_ae,
			XK_ae,	XK_ae },
  { 180, 0,		XK_ccedilla,	XK_ccedilla,
			XK_ccedilla,	XK_ccedilla },
  { 181, 0,		XK_eth,	XK_eth,
			XK_eth,	XK_eth },
  { 182, 0,		XK_egrave,	XK_egrave,
			XK_egrave,	XK_egrave },
  { 183, 0,		XK_eacute,	XK_eacute,
			XK_eacute,	XK_eacute },
  { 184, 0,		XK_ecircumflex,	XK_ecircumflex,
			XK_ecircumflex,	XK_ecircumflex },
  { 185, 0,		XK_ediaeresis,	XK_ediaeresis,
			XK_ediaeresis,	XK_ediaeresis },
  { 186, 0,		XK_igrave,	XK_igrave,
			XK_igrave,	XK_igrave },
  { 187, 0,		XK_iacute,	XK_iacute,
			XK_iacute,	XK_iacute },
  { 188, 0,		XK_icircumflex,	XK_icircumflex,
			XK_icircumflex,	XK_icircumflex },
  { 189, 0,		XK_idiaeresis,	XK_idiaeresis,
			XK_idiaeresis,	XK_idiaeresis },
  { 190, 0,		XK_ntilde,	XK_ntilde,
			XK_ntilde,	XK_ntilde },
  { 191, 0,		XK_ograve,	XK_ograve,
			XK_ograve,	XK_ograve },
  { 192, 0,		XK_oacute,	XK_oacute,
			XK_oacute,	XK_oacute },
  { 193, 0,		XK_ocircumflex,	XK_ocircumflex,
			XK_ocircumflex,	XK_ocircumflex },
  { 194, 0,		XK_otilde,	XK_otilde,
			XK_otilde,	XK_otilde },
  { 195, 0,		XK_odiaeresis,	XK_odiaeresis,
			XK_odiaeresis,	XK_odiaeresis },
  { 196, 0,		XK_oslash,	XK_oslash,
			XK_oslash,	XK_oslash },
  { 197, 0,		XK_thorn,	XK_thorn,
			XK_thorn,	XK_thorn },
  { 198, 0,		XK_thorn,	XK_thorn,
			XK_thorn,	XK_thorn },
  { 199, 0,		XK_ssharp,	XK_ssharp,
			XK_ssharp,	XK_ssharp },
  { 200, 0,		XK_ugrave,	XK_ugrave,
			XK_ugrave,	XK_ugrave },
  { 201, 0,		XK_uacute,	XK_uacute,
			XK_uacute,	XK_uacute },
  { 202, 0,		XK_ucircumflex,	XK_ucircumflex,
			XK_ucircumflex,	XK_ucircumflex },
  { 203, 0,		XK_udiaeresis,	XK_udiaeresis,
			XK_udiaeresis,	XK_udiaeresis },
  { 204, 0,		XK_yacute,	XK_yacute,
			XK_yacute,	XK_yacute },
  { 205, 0,		XK_ydiaeresis,	XK_ydiaeresis,
			XK_ydiaeresis,	XK_ydiaeresis },
  { 206, 0,		XK_Aacute,	XK_Aacute,
			XK_Aacute,	XK_Aacute },
  { 207, 0,		XK_Agrave,	XK_Agrave,
			XK_Agrave,	XK_Agrave },
  { 208, 0,		XK_Acircumflex,	XK_Acircumflex,
			XK_Acircumflex,	XK_Acircumflex },
  { 209, 0,		XK_Atilde,	XK_Atilde,
			XK_Atilde,	XK_Atilde },
  { 210, 0,		XK_Adiaeresis,	XK_Adiaeresis,
			XK_Adiaeresis,	XK_Adiaeresis },
  { 211, 0,		XK_Aring,	XK_Aring,
			XK_Aring,	XK_Aring },
  { 212, 0,		XK_AE,	XK_AE,
			XK_AE,	XK_AE },
  { 213, 0,		XK_Ccedilla,	XK_Ccedilla,
			XK_Ccedilla,	XK_Ccedilla },
  { 214, 0,		XK_ETH,	XK_ETH,
			XK_ETH,	XK_ETH },
  { 215, 0,		XK_Egrave,	XK_Egrave,
			XK_Egrave,	XK_Egrave },
  { 216, 0,		XK_Eacute,	XK_Eacute,
			XK_Eacute,	XK_Eacute },
  { 217, 0,		XK_Ecircumflex,	XK_Ecircumflex,
			XK_Ecircumflex,	XK_Ecircumflex },
  { 218, 0,		XK_Ediaeresis,	XK_Ediaeresis,
			XK_Ediaeresis,	XK_Ediaeresis },
  { 219, 0,		XK_Iacute,	XK_Iacute,
			XK_Iacute,	XK_Iacute },
  { 220, 0,		XK_Idiaeresis,	XK_Idiaeresis,
			XK_Idiaeresis,	XK_Idiaeresis },
  { 221, 0,		XK_Icircumflex,	XK_Icircumflex,
			XK_Icircumflex,	XK_Icircumflex },
  { 222, 0,		XK_Igrave,	XK_Igrave,
			XK_Igrave,	XK_Igrave },
  { 223, 0,		XK_Ntilde,	XK_Ntilde,
			XK_Ntilde,	XK_Ntilde },
  { 224, 0,		XK_Ograve,	XK_Ograve,
			XK_Ograve,	XK_Ograve },
  { 225, 0,		XK_Oacute,	XK_Oacute,
			XK_Oacute,	XK_Oacute },
  { 226, 0,		XK_Ocircumflex,	XK_Ocircumflex,
			XK_Ocircumflex,	XK_Ocircumflex },
  { 227, 0,		XK_Otilde,	XK_Otilde,
			XK_Otilde,	XK_Otilde },
  { 228, 0,		XK_Odiaeresis,	XK_Odiaeresis,
			XK_Odiaeresis,	XK_Odiaeresis },
  { 229, 0,		XK_Ooblique,	XK_Ooblique,
			XK_Ooblique,	XK_Ooblique },
  { 230, 0,		XK_THORN,	XK_THORN,
			XK_THORN,	XK_THORN },
  { 231, 0,		XK_THORN,	XK_THORN,
			XK_THORN,	XK_THORN },
  { 232, 0,		XK_Ugrave,	XK_Ugrave,
			XK_Ugrave,	XK_Ugrave },
  { 233, 0,		XK_Uacute,	XK_Uacute,
			XK_Uacute,	XK_Uacute },
  { 234, 0,		XK_Ucircumflex,	XK_Ucircumflex,
			XK_Ucircumflex,	XK_Ucircumflex },
  { 235, 0,		XK_Udiaeresis,	XK_Udiaeresis,
			XK_Udiaeresis,	XK_Udiaeresis },
  { 236, 0,		XK_Yacute,	XK_Yacute,
			XK_Yacute,	XK_Yacute },
  { 237, 0,		XK_cent,	XK_cent,
			XK_cent,	XK_cent },
  { 238, 0,		XK_cent,	XK_cent,
			XK_cent,	XK_cent },
  { 239, 0,		XK_sterling,	XK_sterling,
			XK_sterling,	XK_sterling },
  { 240, 0,		XK_sterling,	XK_sterling,
			XK_sterling,	XK_sterling },
  { 241, 0,		XK_yen,	XK_yen,
			XK_yen,	XK_yen },
  { 242, 0,		XK_yen,	XK_yen,
			XK_yen,	XK_yen },
  { 243, 0,		XK_currency,	XK_currency,
			XK_currency,	XK_currency },
  { 244, 0,		XK_currency,	XK_currency,
			XK_currency,	XK_currency },
  { 245, 0,		XK_currency,	XK_currency,
			XK_currency,	XK_currency },
  { 246, 0,		XK_currency,	XK_currency,
			XK_currency,	XK_currency }
  };
