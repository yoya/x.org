/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the keymap of the Tektronix VT200 keyboard.
 * _TEK_KEYBOARD_TYPE(STRING) = "vt200"
 * _TEK_KEYBOARD_LAYOUT(STRING) = "x_esc"
 * _TEK_KEYBOARD_NATIONALITY(STRING) = "usascii"
 * By Juergen Stuber <juergen.stuber@mpi-sb.mpg.de>
 * with Tektronix XP338 Version 6.0 Level 0.
 */

static struct default_keycode_map tek200xesc_us_map [] = {
  {  15, 0,		XK_F1,	XK_Pause },
  {  16, 0,		XK_KP_F1 },
  {  17, 0,		XK_less,	XK_greater },
  {  18, 0,		XK_Escape,	XK_F11 },
  {  19, 0,		XK_BackSpace,	XK_F12 },
  {  20, 0,		XK_Linefeed,	XK_F13 },
  {  21, 0,		XK_Tab },
  {  22, 0,		XK_grave,	XK_asciitilde },
  {  23, 0,		XK_F2,	XK_Print },
  {  25, ControlMask,	XK_Control_L },
  {  26, ShiftMask,	XK_Shift_L },
  {  28, LockMask,	XK_Caps_Lock },
  {  29, 0,		XK_Q },
  {  30, 0,		XK_1,	XK_exclam },
  {  33, Mod1Mask,	XK_Alt_L,	XK_Multi_key },
  {  34, 0,		XK_Z },
  {  35, 0,		XK_S },
  {  36, 0,		XK_A },
  {  37, 0,		XK_W },
  {  38, 0,		XK_2,	XK_at },
  {  39, 0,		XK_F4 },
  {  41, 0,		XK_C },
  {  42, 0,		XK_X },
  {  43, 0,		XK_D },
  {  44, 0,		XK_E },
  {  45, 0,		XK_4,	XK_dollar },
  {  46, 0,		XK_3,	XK_numbersign },
  {  47, 0,		XK_F5,	XK_Break },
  {  49, 0,		XK_space },
  {  50, 0,		XK_V },
  {  51, 0,		XK_F },
  {  52, 0,		XK_T },
  {  53, 0,		XK_R },
  {  54, 0,		XK_5,	XK_percent },
  {  55, 0,		XK_F6 },
  {  57, 0,		XK_N },
  {  58, 0,		XK_B },
  {  59, 0,		XK_H },
  {  60, 0,		XK_G },
  {  61, 0,		XK_Y },
  {  62, 0,		XK_6,	XK_asciicircum },
  {  63, 0,		XK_F7 },
  {  66, 0,		XK_M },
  {  67, 0,		XK_J },
  {  68, 0,		XK_U },
  {  69, 0,		XK_7,	XK_ampersand },
  {  70, 0,		XK_8,	XK_asterisk },
  {  71, 0,		XK_F8 },
  {  73, 0,		XK_comma },
  {  74, 0,		XK_K },
  {  75, 0,		XK_I },
  {  76, 0,		XK_O },
  {  77, 0,		XK_0,	XK_parenright },
  {  78, 0,		XK_9,	XK_parenleft },
  {  79, 0,		XK_F9 },
  {  81, 0,		XK_period },
  {  82, 0,		XK_slash,	XK_question },
  {  83, 0,		XK_L },
  {  84, 0,		XK_semicolon,	XK_colon },
  {  85, 0,		XK_P },
  {  86, 0,		XK_minus,	XK_underscore },
  {  87, 0,		XK_F10 },
  {  88, 0,		XK_F14 },
  {  89, Mod5Mask,	XK_Help },
  {  90, 0,		XK_apostrophe,	XK_quotedbl },
  {  91, 0,		XK_backslash,	XK_bar },
  {  92, 0,		XK_bracketleft, XK_braceleft },
  {  93, 0,		XK_equal,	XK_plus },
  {  95, 0,		XK_KP_F4 },
  {  97, ShiftMask,	XK_Shift_R },
  {  98, 0,		XK_Return },
  {  99, 0,		XK_bracketright,	XK_braceright },
  { 103, 0,		XK_KP_F3 },
  { 104, 0,		XK_Down },
  { 105, 0,		XK_Left },
  { 106, 0,		TekXK_Setup,	XK_F3 },
  { 107, 0,		XK_Up },
  { 108, 0,		DXK_Remove },
  { 109, 0,		XK_Select },
  { 110, 0,		XK_Delete },
  { 111, 0,		XK_Insert },
  { 113, 0,		XK_KP_1 },
  { 114, 0,		XK_Right },
  { 115, 0,		XK_KP_4 },
  { 116, 0,		XK_KP_7 },
  { 117, 0,		XK_Next },
  { 118, 0,		XK_Find },
  { 119, 0,		XK_Prior },
  { 120, 0,		XK_KP_0 },
  { 121, 0,		XK_KP_Decimal },
  { 122, 0,		XK_KP_2 },
  { 123, 0,		XK_KP_5 },
  { 124, 0,		XK_KP_6 },
  { 125, 0,		XK_KP_8 },
  { 126, 0,		XK_KP_F2 },
  { 129, 0,		XK_KP_Enter },
  { 130, 0,		XK_KP_3 },
  { 132, 0,		XK_KP_Separator },
  { 133, 0,		XK_KP_9 },
  { 135, 0,		XK_Menu },
  { 136, 0,		XK_F17 },
  { 137, 0,		XK_F18 },
  { 138, 0,		XK_F19 },
  { 139, 0,		XK_F20 },
  { 140, 0,		XK_KP_Subtract }
  };
