/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the key surfaces of the Atari TT (USA) keyboard
 * By Mats Wichmann <mats@alruna.com>
 */

static struct key_strings tt_caps [] = {

  /* Row 0 */

  {"F1", 0},
  {"F2", 0},
  {"F3", 0},
  {"F4", 0},
  {"F5", 0},
  {"F6", 0},
  {"F7", 0},
  {"F8", 0},
  {"F9", 0},
  {"F10", 0},

  /* Row 2 */

  {"Esc", 0},
  {"!", "1", 0},
  {"@", "2", 0},
  {"#", "3", 0},
  {"$", "4", 0},
  {"%", "5", 0},
  {"^", "6", 0},
  {"&", "7", 0},
  {"*", "8", 0},
  {"(", "9", 0},
  {")", "0", 0},
  {"_", "-", 0},
  {"+", "=", 0},
  {"~", "`", 0},
  {"Backspace", 0},
  {"Help", 0},
  {"Undo", 0},
  {"(", 0},
  {")", 0},
  {"/", 0},
  {"*", 0},

  /* Row 3 */

  {"Tab", 0},
  {"Q", 0},
  {"W", 0},
  {"E", 0},
  {"R", 0},
  {"T", 0},
  {"Y", 0},
  {"U", 0},
  {"I", 0},
  {"O", 0},
  {"P", 0},
  {"{", "[", 0},
  {"}", "]", 0},
  {"Delete", 0},
  {"Insert", 0},
  {"UpArrow", 0},
  {"Clr", "Home", 0},
  {"7", 0},
  {"8", 0},
  {"9", 0},
  {"-", 0},

  /* Row 4 */

  {"Control", 0},
  {"A", 0},
  {"S", 0},
  {"D", 0},
  {"F", 0},
  {"G", 0},
  {"H", 0},
  {"J", 0},
  {"K", 0},
  {"L", 0},
  {":", ";", 0},
  {"\"", "'", 0},
  {"Return", 0},
  {"|", "\\", 0},
  {"LeftArrow", 0},
  {"DownArrow", 0},
  {"RightArrow", 0},
  {"4", 0},
  {"5", 0},
  {"6", 0},
  {"+", 0},

  /* Row 5 */

  {"Shift", 0},
#if 0
  /* Non-US Atari keyboards have, instead of a wide left-shift key,
     a narrower left-shift and an additional "standard" key */
  {"???", 0},
#endif
  {"Z", 0},
  {"X", 0},
  {"C", 0},
  {"V", 0},
  {"B", 0},
  {"N", 0},
  {"M", 0},
  {"<", ",", 0},
  {">", ".", 0},
  {"?", "/", 0},
  {"Shift", 0},
  {"1", 0},
  {"2", 0},
  {"3", 0},
  {"Enter", 0},

  /* Row 6 */

  {"Alternate", 0},
  {" ", 0},
  {"CapsLock", 0},
  {"0", 0},
  {".", 0}
};
