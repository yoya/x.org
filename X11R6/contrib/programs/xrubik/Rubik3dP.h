/*
# X-BASED RUBIK'S CUBE
#
#  Rubik3dP.h
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

/* Private header file for Rubik3d */
#ifndef _Rubik3dP_h
#define _Rubik3dP_h

#include "Rubik3d.h"

#define TR 8
#define BR 9
#define BL 10
#define TL 11
#define SAME 0
#define OPPOSITE 1
#define DOWN 0
#define UP 1
#define COORD3D 12
#define MAXVIEWS 2
#define MAXORIENT 4

#define SQRT_3 1.732050808
#define MULTIPLY(a) ((int)((double)a*SQRT_3/2.0))
#define DIVIDE(a) ((int)((double)a*2.0/SQRT_3))

typedef struct _Rubik3DPart
{
  Position cube_width, cube_height, cube_diagonal, cube_diag;
  Position face_width, face_height, face_diagonal;
  Position view_width, view_height;
  XPoint view_middle;
} Rubik3DPart;

typedef struct _Rubik3DRec
{
  CorePart core;
  RubikPart rubik;
  Rubik3DPart rubik3d;
} Rubik3DRec;

/* This gets around C's inability to do inheritance */
typedef struct _Rubik3DClassPart
{
  int ignore;
} Rubik3DClassPart;

typedef struct _Rubik3DClassRec
{
  CoreClassPart core_class;
  RubikClassPart rubik_class;
  Rubik3DClassPart rubik3d_class;
} Rubik3DClassRec;

extern Rubik3DClassRec rubik3dClassRec;

#endif /* _Rubik3dP_h */
