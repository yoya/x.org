/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the default keymap of the NCD N102 french keyboard.
 * By Francois Regis Colin <fcolin@cenatls.cena.dgac.fr>
 */

static struct default_keycode_map ncd102f_map [] = {

  /* this key has a keycode but no keysym in the default map */
  {  98, 0,		0 },	/* Pause */

  {   8, 0,		XK_Escape },
  {  13, 0,		XK_Tab },
  {  14, 0, 		XK_twosuperior },
  {  15, 0,		XK_F2 },
  {  16, 0,		XK_F1 },
  {  17, ControlMask,	XK_Control_L },
  {  18, ShiftMask,	XK_Shift_L },
  {  19, 0,		XK_less,	XK_greater },
  {  20, LockMask,	XK_Caps_Lock },
  {  21, 0,		XK_A },
  {  22, 0,		XK_ampersand,	XK_1 },
  {  23, 0,		XK_F3 },
  {  25, Mod1Mask,	XK_Alt_L },
  {  26, 0,		XK_W },
  {  27, 0,		XK_S },
  {  28, 0,		XK_Q },
  {  29, 0,		XK_Z },
  {  30, 0,		XK_eacute,	XK_2,	XK_asciitilde },
  {  31, 0,		XK_F4 },
  {  33, 0,		XK_C },
  {  34, 0,		XK_X },
  {  35, 0,		XK_D },
  {  36, 0,		XK_E },
  {  37, 0,		XK_apostrophe,	XK_4,	XK_braceleft },
  {  38, 0,		XK_quotedbl,	XK_3,	XK_numbersign },
  {  39, 0,		XK_F5 },
  {  41, 0,		XK_space },
  {  42, 0,		XK_V },
  {  43, 0,		XK_F },
  {  44, 0,		XK_T },
  {  45, 0,		XK_R },
  {  46, 0,		XK_parenleft,	XK_5,	XK_bracketleft },
  {  47, 0,		XK_F6 },
  {  49, 0,		XK_N },
  {  50, 0,		XK_B },
  {  51, 0,		XK_H },
  {  52, 0,		XK_G },
  {  53, 0,		XK_Y },
  {  54, 0,		XK_minus,	XK_6,	XK_bar },
  {  55, 0,		XK_F7 },
  {  58, 0,		XK_comma,	XK_question },
  {  59, 0,		XK_J },
  {  60, 0,		XK_U },
  {  61, 0,		XK_egrave,	XK_7,	XK_grave },
  {  62, 0,		XK_underscore,	XK_8,	XK_backslash },
  {  63, 0,		XK_F8 },
  {  65, 0,		XK_semicolon,	XK_period },
  {  66, 0,		XK_K },
  {  67, 0,		XK_I },
  {  68, 0,		XK_O },
  {  69, 0,		XK_agrave,	XK_0,	XK_at },
  {  70, 0,		XK_ccedilla,	XK_9,	XK_asciicircum },
  {  71, 0,		XK_F9 },
  {  73, 0,		XK_colon,	XK_slash },
  {  74, 0,		XK_exclam,	XK_section },
  {  75, 0,		XK_L },
  {  76, 0,		XK_M },
  {  77, 0,		XK_P },
  {  78, 0,		XK_parenright,	XK_degree,	XK_bracketright },
  {  79, 0,		XK_F10 },
  {  82, 0,		XK_ugrave,	XK_percent },
  {  83, 0,		XK_asterisk,	XK_mu },
  {  84, 0,		XK_asciicircum,	XK_diaeresis },
  {  85, 0,		XK_equal,	XK_plus,	XK_braceright },
  {  86, 0,		XK_F11 },
  {  87, 0,		XK_Print },
  {  88, ControlMask,	XK_Control_R },
  {  89, ShiftMask,	XK_Shift_R },
  {  90, 0,		XK_Return },
  {  91, 0,		XK_dollar,	XK_sterling,	XK_currency },
  {  94, 0,		XK_F12 },
  {  95, 0,		XK_Scroll_Lock },
  {  96, 0,		XK_Down },
  {  97, 0,		XK_Left },
  {  99, 0,		XK_Up },
  { 100, 0,		XK_Delete },
  { 101, 0,		XK_End },
  { 102, 0,		XK_BackSpace },
  { 103, 0,		XK_Insert },
  { 105, 0,		XK_KP_1 },
  { 106, 0,		XK_Right },
  { 107, 0,		XK_KP_4 },
  { 108, 0,		XK_KP_7 },
  { 109, 0,		XK_Next },
  { 110, 0,		XK_Home },
  { 111, 0,		XK_Prior },
  { 112, 0,		XK_KP_0 },
  { 113, 0,		XK_KP_Decimal },
  { 114, 0,		XK_KP_2 },
  { 115, 0,		XK_KP_5 },
  { 116, 0,		XK_KP_6 },
  { 117, 0,		XK_KP_8 },
  { 118, Mod5Mask,	XK_Num_Lock },
  { 119, 0,		XK_KP_Divide },
  { 121, 0,		XK_KP_Enter },
  { 122, 0,		XK_KP_3 },
  { 124, 0,		XK_KP_Add },
  { 125, 0,		XK_KP_9 },
  { 126, 0,		XK_KP_Multiply },
  { 132, 0,		XK_KP_Subtract }
  };
