/* xkeycaps, Copyright (c) 1991, 1992, 1993 Jamie Zawinski <jwz@lucid.com>
 *
 * This file describes the keycodes of the Sun Swiss-French type 5 keyboard.
 * By Philippe A. Kipfer (Philippe.Kipfer@di.epfl.ch), dec.1993.
 * This keyboard meet the TUV standard.
 * Defined from Sun document "Notes claviers Sun type 5" ref. #800-6803-10
 *
 */

static struct key_geometry sun5swfr_geom_row0 [] = {
 {14, 7}, {4, 0},
 {7, 7}, {7, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {3, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5swfr_geom_row1 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {14, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5swfr_geom_row2 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {10, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {2, 0}, {9, 14}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 14}
};

static struct key_geometry sun5swfr_geom_row3 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {13, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {7, 7}, {29, 0},
 {7, 7}, {7, 7}, {7, 7}
};

static struct key_geometry sun5swfr_geom_row4 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {9, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7},
 {7, 7}, {7, 7}, {7, 7}, {17,7}, {11, 0},
 {7, 7}, {11, 0},
 {7, 7}, {7, 7}, {7, 7}, {7, 14}
};

static struct key_geometry sun5swfr_geom_row5 [] = {
 {7, 7}, {7, 7}, {4, 0},
 {11, 7}, {7, 7}, {7, 7}, {7, 7}, {59, 7}, {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {7, 7}, {7, 7}, {7, 7}, {4, 0},
 {14, 7}, {7, 7}
};


static struct keyboard_row_geometry sun5swfr_geom_rows [] = {
  ROW (sun5swfr_geom_row0, 7),
  EMPTYROW (7),
  ROW (sun5swfr_geom_row1, 7),
  ROW (sun5swfr_geom_row2, 7),
  ROW (sun5swfr_geom_row3, 7),
  ROW (sun5swfr_geom_row4, 7),
  ROW (sun5swfr_geom_row5, 7),
};


static struct keyboard_geometry sun5swfr_geom = {
  sizeof (sun5swfr_geom_rows) / sizeof (struct keyboard_row_geometry),
  sun5swfr_geom_rows, 6, 3, 3
};
