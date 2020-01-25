/*
# X-BASED RUBIK'S CUBE
#
#  Rubik2dP.h
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

/* Private header file for Rubik2d */
#ifndef _Rubik2dP_h
#define _Rubik2dP_h

#include "Rubik2d.h"

#define FRONTLOC 0
#define BACKLOC 11
#define COORD2D 8
#define MAXX 3
#define MAXY 4
#define MAXRECT (MAXX*MAXY)

typedef struct _Rubik2DPart
{
  Position cube_length;
  Position face_length;
  Position view_length;
} Rubik2DPart;

typedef struct _Rubik2DRec
{
  CorePart core;
  RubikPart rubik;
  Rubik2DPart rubik2d;
} Rubik2DRec;

/* This gets around C's inability to do inheritance */
typedef struct _Rubik2DClassPart
{
  int ignore;
} Rubik2DClassPart;

typedef struct _XsRubik2DClassRec
{
  CoreClassPart core_class;
  RubikClassPart rubik_class;
  Rubik2DClassPart rubik2d_class;
} Rubik2DClassRec;

extern Rubik2DClassRec rubik2dClassRec;

#endif /* _Rubik2dP_h */
