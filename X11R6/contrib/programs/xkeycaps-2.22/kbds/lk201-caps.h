/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the key surfaces of the DEC LK201 and LK401 keyboards.
 * By Tom Ivar Helbekkmo <tih@barsoom.nhh.no> and jwz.
 */

#ifdef LK401_KLUDGE
# define lk201_caps lk401_caps
#endif

static struct key_strings lk201_caps [] = {

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
  {"F11", 0},
  {"F12", 0},
  {"F13", 0},
  {"F14", 0},
  {"Help", 0},
  {"Do", 0},
  {"F17", 0},
  {"F18", 0},
  {"F19", 0},
  {"F20", 0},

  /* Row 2 */

  {"~", "`", 0},
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
  {"<X|", 0},
  {"Find", 0},
  {"Insert", "Here", 0},
  {"Re-", "move", 0},
  {"PF1", 0},
  {"PF2", 0},
  {"PF3", 0},
  {"PF4", 0},

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
  {"Return", 0},
  {"Select", 0},
  {"Prev", "Screen", 0},
  {"Next", "Screen", 0},
  {"7", 0},
  {"8", 0},
  {"9", 0},
  {"-", 0},

  /* Row 4 */

  {"Ctrl", 0},
  {"Lock", 0},
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
  {"|", "\\", 0},
  {"UpArrow", 0},
  {"4", 0},
  {"5", 0},
  {"6", 0},
  {",", 0},

  /* Row 5 */

  {"Shift", 0},
  {">", "<", 0},
  {"Z", 0},
  {"X", 0},
  {"C", 0},
  {"V", 0},
  {"B", 0},
  {"N", 0},
  {"M", 0},
#ifdef LK401_KLUDGE
  {"<", ",", 0},
  {">", ".", 0},
#else /* LK201 */
  {",", ",", 0},
  {".", ".", 0},
#endif
  {"?", "/", 0},
  {"Shift", 0},
  {"LeftArrow", 0},
  {"DownArrow", 0},
  {"RightArrow", 0},
  {"1", 0},
  {"2", 0},
  {"3", 0},
  {"Enter", 0},

  /* Row 6 */

  {"Compose", "Character", 0},
#ifdef LK401_KLUDGE
  {"Alt", "Function", 0},
#endif
  {" ", 0},
#ifdef LK401_KLUDGE
  {"Alt", "Function", 0},	/* LK201 doesn't have this */
  {"Compose", "Character", 0},	/* LK201 doesn't have this */
#endif
  {"0", 0},
  {".", 0}
};

#undef lk201_caps
