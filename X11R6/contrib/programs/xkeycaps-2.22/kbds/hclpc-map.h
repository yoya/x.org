/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the default keymap of the HCL eXceed PC X-server
 * keyboard.  By Grant McDorman <ohcscc!grendel!grant@uunet.ca>
 *
 * By default, Alt_L, Print and Num_Lock are handled locally (Alt_L goes to
 * Microsoft Weendoze). The Alt_L and Num_Lock can be changed by editing a
 * server configuration file, although if you pass both Alt keys to X
 * applications you may break Weendoze.
 */

static struct default_keycode_map hclpc_map [] = {
  {   8, 0,		XK_Escape,	XK_Escape },
  {   9, 0,		XK_1,	XK_exclam },
  {  10, 0,		XK_2,	XK_at },
  {  11, 0,		XK_3,	XK_numbersign },
  {  12, 0,		XK_4,	XK_dollar },
  {  13, 0,		XK_5,	XK_percent },
  {  14, 0,		XK_6,	XK_asciicircum },
  {  15, 0,		XK_7,	XK_ampersand },
  {  16, 0,		XK_8,	XK_asterisk },
  {  17, 0,		XK_9,	XK_parenleft },
  {  18, 0,		XK_0,	XK_parenright },
  {  19, 0,		XK_minus,	XK_underscore },
  {  20, 0,		XK_equal,	XK_plus },
  {  21, 0,		XK_Delete,	XK_BackSpace },
  {  22, 0,		XK_Tab,	XK_KP_Tab },
  {  23, 0,		XK_q,	XK_Q },
  {  24, 0,		XK_w,	XK_W },
  {  25, 0,		XK_e,	XK_E },
  {  26, 0,		XK_r,	XK_R },
  {  27, 0,		XK_t,	XK_T },
  {  28, 0,		XK_y,	XK_Y },
  {  29, 0,		XK_u,	XK_U },
  {  30, 0,		XK_i,	XK_I },
  {  31, 0,		XK_o,	XK_O },
  {  32, 0,		XK_p,	XK_P },
  {  33, 0,		XK_bracketleft,	XK_braceleft },
  {  34, 0,		XK_bracketright,	XK_braceright },
  {  35, 0,		XK_Return,	XK_Return },
  {  36, ControlMask,	XK_Control_L,	XK_Control_L },
  {  37, 0,		XK_a,	XK_A },
  {  38, 0,		XK_s,	XK_S },
  {  39, 0,		XK_d,	XK_D },
  {  40, 0,		XK_f,	XK_F },
  {  41, 0,		XK_g,	XK_G },
  {  42, 0,		XK_h,	XK_H },
  {  43, 0,		XK_j,	XK_J },
  {  44, 0,		XK_k,	XK_K },
  {  45, 0,		XK_l,	XK_L },
  {  46, 0,		XK_semicolon,	XK_colon },
  {  47, 0,		XK_apostrophe,	XK_quotedbl },
  {  48, 0,		XK_grave,	XK_asciitilde },
  {  49, ShiftMask,	XK_Shift_L,	XK_Shift_L },
  {  50, 0,		XK_backslash,	XK_bar },
  {  51, 0,		XK_z,	XK_Z },
  {  52, 0,		XK_x,	XK_X },
  {  53, 0,		XK_c,	XK_C },
  {  54, 0,		XK_v,	XK_V },
  {  55, 0,		XK_b,	XK_B },
  {  56, 0,		XK_n,	XK_N },
  {  57, 0,		XK_m,	XK_M },
  {  58, 0,		XK_comma,	XK_less },
  {  59, 0,		XK_period,	XK_greater },
  {  60, 0,		XK_slash,	XK_question },
  {  61, ShiftMask,	XK_Shift_R,	XK_Shift_R },
  {  62, 0,		XK_KP_Multiply,	XK_Print },
  {  63, Mod1Mask,	XK_Alt_L,	XK_Alt_L },
  {  64, 0,		XK_space,	XK_space },
  {  65, LockMask,	XK_Caps_Lock,	XK_Caps_Lock },
  {  66, 0,		XK_F1,	XK_F1 },
  {  67, 0,		XK_F2,	XK_F2 },
  {  68, 0,		XK_F3,	XK_F3 },
  {  69, 0,		XK_F4,	XK_F4 },
  {  70, 0,		XK_F5,	XK_F5 },
  {  71, 0,		XK_F6,	XK_F6 },
  {  72, 0,		XK_F7,	XK_F7 },
  {  73, 0,		XK_F8,	XK_F8 },
  {  74, 0,		XK_F9,	XK_F9 },
  {  75, 0,		XK_F10,	XK_F10 },
  {  77, 0,		XK_Scroll_Lock,	XK_Scroll_Lock },
  {  78, 0,		XK_Home,	XK_KP_7 },
  {  79, 0,		XK_Up,	XK_KP_8 },
  {  80, 0,		XK_Prior,	XK_KP_9 },
  {  81, 0,		XK_KP_Subtract,	XK_KP_Subtract },
  {  82, 0,		XK_Left,	XK_KP_4 },
  {  83, 0,		XK_KP_Space,	XK_KP_5 },
  {  84, 0,		XK_Right,	XK_KP_6 },
  {  85, 0,		XK_KP_Add,	XK_KP_Add },
  {  86, 0,		XK_End,	XK_KP_1 },
  {  87, 0,		XK_Down,	XK_KP_2 },
  {  88, 0,		XK_Next,	XK_KP_3 },
  {  89, 0,		XK_Insert,	XK_KP_0 },
  {  90, 0,		XK_Delete,	XK_KP_Decimal },
  {  91, Mod1Mask,	XK_Alt_R,	XK_Alt_R },
  {  92, ControlMask,	XK_Control_R,	XK_Control_R },
  {  94, 0,		XK_F11,	XK_F11 },
  {  95, 0,		XK_F12,	XK_F12 },
  {  96, 0,		XK_KP_Enter,	XK_KP_Enter },
  {  97, 0,		XK_Insert,	XK_Insert },
  {  98, 0,		XK_Delete,	XK_Delete },
  {  99, 0,		XK_Left,	XK_Left },
  { 100, 0,		XK_Home,	XK_Home },
  { 101, 0,		XK_End,	XK_End },
  { 102, 0,		XK_Up,	XK_Up },
  { 103, 0,		XK_Down,	XK_Down },
  { 104, 0,		XK_Prior,	XK_Prior },
  { 105, 0,		XK_Next,	XK_Next },
  { 106, 0,		XK_Right,	XK_Right },
  { 107, 0,		XK_KP_Divide,	XK_KP_Divide },
  { 108, 0,		XK_Print,	XK_Print },
  { 109, 0,		XK_Pause,	XK_Pause },
  { 110, 0,		XK_KP_7,	XK_Home },
  { 111, 0,		XK_KP_4,	XK_Left },
  { 112, 0,		XK_KP_1,	XK_End },
  { 113, 0,		XK_KP_8,	XK_Up },
  { 114, 0,		XK_KP_5,	XK_KP_Space },
  { 115, 0,		XK_KP_2,	XK_Down },
  { 116, 0,		XK_KP_0,	XK_Insert },
  { 117, 0,		XK_KP_9,	XK_Prior },
  { 118, 0,		XK_KP_6,	XK_Right },
  { 119, 0,		XK_KP_3,	XK_Next },
  { 120, 0,		XK_KP_Decimal,	XK_Delete }
  };
