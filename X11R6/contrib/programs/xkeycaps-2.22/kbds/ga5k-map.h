/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the default keymap of the Concurrent GA5000 keyboard.
 * By black@westford.ccur.com.
 */

static struct default_keycode_map ga5k_map [] = {
  { 128, ShiftMask,	XK_Shift_L },
  { 129, ControlMask,	XK_Control_L },
  { 130, LockMask,	XK_Caps_Lock },
  { 133, 0,		XK_Pause,	XK_Break },
  { 134, 0,		XK_KP_Enter },
  { 136, 0,		XK_BackSpace },
  { 137, 0,		XK_Tab },
  { 138, 0,		XK_Linefeed },
  { 139, ShiftMask,	XK_Shift_R },
  { 140, Mod1Mask,	XK_Meta_L },
  { 141, 0,		XK_Return },
  { 142, 0,		XK_KP_Decimal },
  { 143, 0,		XK_KP_0 },
  { 144, 0,		XK_Down },
  { 145, 0,		XK_Select },
  { 146, 0,		XK_Menu },
  { 147, 0,		XK_Left },
  { 148, 0,		XK_KP_3 },
  { 149, 0,		XK_KP_2 },
  { 150, 0,		XK_Right },
  { 151, 0,		XK_F2 },
  { 152, 0,		XK_F4 },
  { 153, 0,		XK_F6 },
  { 154, 0,		XK_F7 },
  { 155, 0,		XK_F9 },
  { 156, 0,		XK_F11 },
  { 157, 0,		XK_Insert },
  { 158, 0,		XK_Prior },
  { 159, 0,		XK_Next },
  { 160, 0,		XK_space },
  { 161, 0,		XK_End },
  { 162, 0,		XK_F1 },
  { 163, 0,		XK_F3 },
  { 164, 0,		XK_F5 },
  { 165, 0,		XK_F8 },
  { 166, 0,		XK_F10 },
  { 167, 0,		XK_apostrophe,	XK_quotedbl },
  { 168, 0,		XK_F12 },
  { 169, 0,		XK_Home },
  { 170, 0,		XK_Help },
  { 171, 0,		XK_Num_Lock },
  { 172, 0,		XK_comma,	XK_less },
  { 173, 0,		XK_minus,	XK_underscore },
  { 174, 0,		XK_period,	XK_greater },
  { 175, 0,		XK_slash,	XK_question },
  { 176, 0,		XK_0,	XK_parenright },
  { 177, 0,		XK_1,	XK_exclam },
  { 178, 0,		XK_2,	XK_at },
  { 179, 0,		XK_3,	XK_numbersign },
  { 180, 0,		XK_4,	XK_dollar },
  { 181, 0,		XK_5,	XK_percent },
  { 182, 0,		XK_6,	XK_asciicircum },
  { 183, 0,		XK_7,	XK_ampersand },
  { 184, 0,		XK_8,	XK_asterisk },
  { 185, 0,		XK_9,	XK_parenleft },
/* #### BUG!! What is the keycode of this key?? */
  { 185, 0,		XK_c,	XK_C },
  { 186, 0,		XK_KP_Divide },
  { 187, 0,		XK_semicolon,	XK_colon },
  { 188, 0,		XK_grave,	XK_asciitilde },
  { 189, 0,		XK_equal,	XK_plus },
  { 191, 0,		XK_KP_Multiply },
  { 193, 0,		XK_a,	XK_A },
  { 194, 0,		XK_b,	XK_B },
  { 196, 0,		XK_d,	XK_D },
  { 197, 0,		XK_e,	XK_E },
  { 198, 0,		XK_f,	XK_F },
  { 199, 0,		XK_g,	XK_G },
  { 200, 0,		XK_h,	XK_H },
  { 201, 0,		XK_i,	XK_I },
  { 202, 0,		XK_j,	XK_J },
  { 203, 0,		XK_k,	XK_K },
  { 204, 0,		XK_l,	XK_L },
  { 205, 0,		XK_m,	XK_M },
  { 206, 0,		XK_n,	XK_N },
  { 207, 0,		XK_o,	XK_O },
  { 208, 0,		XK_p,	XK_P },
  { 209, 0,		XK_q,	XK_Q },
  { 210, 0,		XK_r,	XK_R },
  { 211, 0,		XK_s,	XK_S },
  { 212, 0,		XK_t,	XK_T },
  { 213, 0,		XK_u,	XK_U },
  { 214, 0,		XK_v,	XK_V },
  { 215, 0,		XK_w,	XK_W },
  { 216, 0,		XK_x,	XK_X },
  { 217, 0,		XK_y,	XK_Y },
  { 218, 0,		XK_z,	XK_Z },
  { 220, 0,		XK_backslash,	XK_bar },
  { 221, 0,		XK_bracketright,	XK_braceright },
  { 227, 0,		XK_bracketleft,	XK_braceleft },
  { 230, 0,		XK_KP_Subtract },
  { 231, 0,		XK_KP_9 },
  { 232, 0,		XK_KP_7 },
  { 234, 0,		XK_KP_8 },
  { 239, 0,		XK_KP_6 },
  { 240, 0,		XK_KP_4 },
  { 241, 0,		XK_Up },
  { 242, 0,		XK_KP_Add },
  { 243, 0,		XK_KP_5 },
  { 244, 0,		XK_KP_1 },
  { 245, 0,		XK_Escape },
  { 255, 0,		XK_Delete }
  };
