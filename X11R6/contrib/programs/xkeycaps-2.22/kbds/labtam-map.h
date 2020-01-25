/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the default keymap of the Labtam MT200 keyboard
 * (Australian layout), which is really a Honeywell-101RXd keyboard, 
 * also used with some IBM PCs.  
 * Generated by Anthony Thyssen <anthony@cit.gu.edu.au> against the Labtam
 * X11R5 server, patch level 26, running on a SUN4/60 SLC with SunOS 4.1.2.
 */

static struct default_keycode_map labtam_map [] = {

  /* this has a keycode but no keysyms in the default map */
  { 134, 0,		0 },	/* Pause */

  {   9, 0,		XK_grave,	XK_asciitilde },
  {  10, 0,		XK_1,	XK_exclam },
  {  11, 0,		XK_2,	XK_at },
  {  12, 0,		XK_3,	XK_numbersign,
			XK_sterling },
  {  13, 0,		XK_4,	XK_dollar },
  {  14, 0,		XK_5,	XK_percent },
  {  15, 0,		XK_6,	XK_asciicircum },
  {  16, 0,		XK_7,	XK_ampersand },
  {  17, 0,		XK_8,	XK_asterisk },
  {  18, 0,		XK_9,	XK_parenleft },
  {  19, 0,		XK_0,	XK_parenright },
  {  20, 0,		XK_minus,	XK_underscore },
  {  21, 0,		XK_equal,	XK_plus },
  {  23, 0,		XK_BackSpace },
  {  24, 0,		XK_Tab },
  {  25, 0,		XK_Q },
  {  26, 0,		XK_W },
  {  27, 0,		XK_E },
  {  28, 0,		XK_R },
  {  29, 0,		XK_T },
  {  30, 0,		XK_Y },
  {  31, 0,		XK_U },
  {  32, 0,		XK_I },
  {  33, 0,		XK_O },
  {  34, 0,		XK_P },
  {  35, 0,		XK_bracketleft,	XK_braceleft },
  {  36, 0,		XK_bracketright,	XK_braceright },
  {  37, 0,		XK_backslash,	XK_bar },
  {  38, LockMask,	XK_Caps_Lock },
  {  39, 0,		XK_A },
  {  40, 0,		XK_S },
  {  41, 0,		XK_D },
  {  42, 0,		XK_F },
  {  43, 0,		XK_G },
  {  44, 0,		XK_H },
  {  45, 0,		XK_J },
  {  46, 0,		XK_K },
  {  47, 0,		XK_L },
  {  48, 0,		XK_semicolon,	XK_colon },
  {  49, 0,		XK_apostrophe,	XK_quotedbl },
  {  51, 0,		XK_Return },
  {  52, ShiftMask,	XK_Shift_L },
  {  54, 0,		XK_Z },
  {  55, 0,		XK_X },
  {  56, 0,		XK_C },
  {  57, 0,		XK_V },
  {  58, 0,		XK_B },
  {  59, 0,		XK_N },
  {  60, 0,		XK_M },
  {  61, 0,		XK_comma,	XK_less },
  {  62, 0,		XK_period,	XK_greater },
  {  63, 0,		XK_slash,	XK_question },
  {  65, ShiftMask,	XK_Shift_R },
  {  66, ControlMask,	XK_Control_L },
  {  68, Mod1Mask,	XK_Alt_L },
  {  69, 0,		XK_space },
  {  70, Mod2Mask,	XK_Alt_R },
  {  72, ControlMask,	XK_Control_R },
  {  83, 0,		XK_Insert },
  {  84, 0,		XK_Delete },
  {  87, 0,		XK_Left },
  {  88, 0,		XK_Home },
  {  89, 0,		XK_End },
  {  91, 0,		XK_Up },
  {  92, 0,		XK_Down },
  {  93, 0,		XK_Prior },
  {  94, 0,		XK_Next },
  {  97, 0,		XK_Right },
  {  98, ShiftMask,	XK_Num_Lock },
  {  99, 0,		XK_Home,	XK_KP_7,
			XK_KP_7,	XK_Home },
  { 100, 0,		XK_Left,	XK_KP_4,
			XK_KP_4,	XK_Left },
  { 101, 0,		XK_End,	XK_KP_1,
			XK_KP_1,	XK_End },
  { 103, 0,		XK_KP_Divide },
  { 104, 0,		XK_Up,	XK_KP_8,
			XK_KP_8,	XK_Up },
  { 105, 0,		0,	XK_KP_5,
			XK_KP_5 },
  { 106, 0,		XK_Down,	XK_KP_2,
			XK_KP_2,	XK_Down },
  { 107, 0,		XK_Insert,	XK_KP_0,
			XK_KP_0,	XK_Insert },
  { 108, 0,		XK_KP_Multiply },
  { 109, 0,		XK_Prior,	XK_KP_9,
			XK_KP_9,	XK_Prior },
  { 110, 0,		XK_Right,	XK_KP_6,
			XK_KP_6,	XK_Right },
  { 111, 0,		XK_Next,	XK_KP_3,
			XK_KP_3,	XK_Next },
  { 112, 0,		XK_Delete,	XK_KP_Decimal,
			XK_KP_Decimal,	XK_Delete },
  { 113, 0,		XK_KP_Subtract },
  { 114, 0,		XK_KP_Add },
  { 116, 0,		XK_KP_Enter },
  { 118, 0,		XK_Escape },
  { 120, 0,		XK_F1 },
  { 121, 0,		XK_F2 },
  { 122, 0,		XK_F3 },
  { 123, 0,		XK_F4 },
  { 124, 0,		XK_F5 },
  { 125, 0,		XK_F6 },
  { 126, 0,		XK_F7 },
  { 127, 0,		XK_F8 },
  { 128, 0,		XK_F9 },
  { 129, 0,		XK_F10 },
  { 130, 0,		XK_F11 },
  { 131, 0,		XK_F12 },
  { 132, 0,		XK_Print },
  { 170, 0,		XK_Scroll_Lock },
  { 178, 0,		XK_Break }
  };
