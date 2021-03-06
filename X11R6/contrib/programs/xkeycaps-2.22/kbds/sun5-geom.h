/* xkeycaps, Copyright (c) 1991, 1992, 1993, 1994
 * Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the physical layout of the Sun type 5 keyboards.  
 * By Carl Witty <cwitty@ai.mit.edu>, Sylvain Meunier <meunier@inf.enst.fr>,
 * and jwz.
 *
 * The "PC" version differs from the regular type 5 keyboard in that the top
 * row of the main block ends in a wide BS key instead of "|\" and "~`", and 
 * the first key on the bottom row (Control here, Caps on the other one) is
 * wider.
 */

static struct key_geometry sun5_geom_row0 [] = {
 {14, 7}, {4, 0},
 {7, 7}, {7, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {3, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5_geom_row1 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5pc_geom_row1 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {14, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5_geom_row2 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {10, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {11, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 14}
};

static struct key_geometry sun5_geom_row3 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {13, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {15, 7}, {29, 0},
 {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5_geom_row4 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {16, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {19, 7}, {11, 0},
 {7, 7}, {11, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 14}
};

static struct key_geometry sun5_geom_row5 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {11, 7}, {7, 7}, {7, 7}, {59, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {14, 7}, {7, 7}
};

static struct key_geometry sun5pc_geom_row5 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {13, 7}, {7, 7}, {7, 7}, {57, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {14, 7}, {7, 7}
};

static struct keyboard_row_geometry sun5_geom_rows [] = {
  ROW (sun5_geom_row0, 7),
  EMPTYROW (7),
  ROW (sun5_geom_row1, 7),
  ROW (sun5_geom_row2, 7),
  ROW (sun5_geom_row3, 7),
  ROW (sun5_geom_row4, 7),
  ROW (sun5_geom_row5, 7),
};

static struct keyboard_row_geometry sun5pc_geom_rows [] = {
  ROW (sun5_geom_row0, 7),
  EMPTYROW (7),
  ROW (sun5pc_geom_row1, 7),
  ROW (sun5_geom_row2, 7),
  ROW (sun5_geom_row3, 7),
  ROW (sun5_geom_row4, 7),
  ROW (sun5pc_geom_row5, 7),
};

static struct keyboard_geometry sun5_geom = {
  sizeof (sun5_geom_rows) / sizeof (struct keyboard_row_geometry),
  sun5_geom_rows, 6, 3, 3
};

static struct keyboard_geometry sun5pc_geom = {
  sizeof (sun5pc_geom_rows) / sizeof (struct keyboard_row_geometry),
  sun5pc_geom_rows, 6, 3, 3
};
