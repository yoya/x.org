/*
# X-BASED RUBIK'S CUBE
#
#  RubikP.h
#
###
#
#  Copyright (c) 1994		David Bagley
#
#                   All Rights Reserved
#
#  The X Consortium, and any party obtaining a copy of these files from
#  the X Consortium, directly or indirectly, is granted, free of charge, a
#  full and unrestricted irrevocable, world-wide, paid up, royalty-free,
#  nonexclusive right and license to deal in this software and
#  documentation files (the "Software"), including without limitation the
#  rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons who receive
#  copies from any such party to do so.  This license includes without
#  limitation a license to do the foregoing actions under any patents of
#  the party supplying this software to the X Consortium.
#
#  This program is distributed in the hope that it will be "playable",
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
*/

/* Private header file for Rubik */
#ifndef _RubikP_h
#define _RubikP_h

#include "Rubik.h"

/*** random generator ***/
#if	defined(HAS_48)		/* Use lrand48() and srand48() */
#define	LRAND()		lrand48()
#define	SRAND(X)	srand48((long) (X))
extern long     lrand48(
#if	NeedFunctionPrototypes
    void
#endif
);
#else
#if	defined(HAS_RANDOM)	/* Use random() and srandom() */
#define	LRAND()		random()
#define	SRAND(X)	srandom((unsigned int) (X))
extern long     random(
#if	NeedFunctionPrototypes
    void
#endif
);
#else				/* Use rand() and srand() */
#define	LRAND()		((long) rand())
#define	SRAND(X)	srand(X)
#endif
#endif
#define NRAND(X)	((int)(LRAND()%(X)))

#define IGNORE (-1)
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3
#define STRT 4
#define CW 5
#define HALF 6
#define CCW 7
#define MAXORIENT 4

/* The following 3 are in xrubik.c also */
#define MAXFACES 6
#define MINCUBES 1
#define MAXCUBES 6

#define DEFAULTCUBES 3
#define MAXCUBES_SQ (MAXCUBES*MAXCUBES)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct _RubikLoc
{
  int face, rotation;
} RubikLoc;

typedef struct _RubikPart
{
  Pixel foreground;
  Pixel face_color[MAXFACES];
  RubikLoc cube_loc[MAXFACES][MAXCUBES][MAXCUBES];
  RubikLoc face_loc[MAXCUBES][MAXCUBES];
  RubikLoc row_loc[MAXORIENT][MAXCUBES];
  int started, randomized;
  int practice, mode, orient, vertical, mono;
  int depth;
  int size, size_size;
  int face, i, j, direction;
  Position delta;
  Position orient_line_length;
  Position puzzle_width, puzzle_height;
  XPoint puzzle_offset, letter_offset;
  GC puzzle_GC;
  GC face_GC[MAXFACES];
  GC inverse_GC;
  String face_name[MAXFACES];
  XtCallbackList select;
} RubikPart;

typedef struct _RubikRec
{
  CorePart core;
  RubikPart rubik;
} RubikRec;

/* This gets around C's inability to do inheritance */
typedef struct _RubikClassPart
{
  int ignore;
} RubikClassPart;

typedef struct _RubikClassRec
{
  CoreClassPart core_class;
  RubikClassPart rubik_class;
} RubikClassRec;

typedef struct _RowNext
{
  int face, direction, side_face;
} RowNext;

extern RubikClassRec rubikClassRec;

extern void quit_rubik();
extern void check_polyhedrons();
extern void reset_polyhedrons();
/* rc : row or column */
extern void read_rc();
extern void rotate_rc();
extern void reverse_rc();
extern int check_solved();

#ifdef DEBUG
extern void print_cube();
extern void print_face();
extern void print_row();
#endif

#endif /* _RubikP_h */
