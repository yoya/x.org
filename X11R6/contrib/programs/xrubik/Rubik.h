/*
# X-BASED RUBIK'S CUBE
#
#  Rubik.h
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

/* Public header file for Rubik */

#ifndef _XtRubik_h
#define _XtRubik_h

/***********************************************************************
 *
 * Rubik Widget
 *
 ***********************************************************************/

#define XtNselectCallback "selectCallback"
#define XtNsize "size"
#define XtNorient "orient"
#define XtNmono "mono"
#define XtNface "face"
#define XtNi "i"
#define XtNj "j"
#define XtNdirection "direction"
#define XtNpractice "practice"
#define XtNstart "start"
#define XtNfaceColor0 "faceColor0"
#define XtNfaceColor1 "faceColor1"
#define XtNfaceColor2 "faceColor2"
#define XtNfaceColor3 "faceColor3"
#define XtNfaceColor4 "faceColor4"
#define XtNfaceColor5 "faceColor5"
#define XtCSize "Size"
#define XtCOrient "Orient"
#define XtCMono "Mono"
#define XtCFace "Face"
#define XtCI "I"
#define XtCJ "J"
#define XtCDirection "Direction"

#define RUBIK_RESET (-2)
#define RUBIK_IGNORE (-1)
#define RUBIK_MOVED 0
#define RUBIK_CONTROL 1
#define RUBIK_SOLVED 2
#define RUBIK_PRACTICE 3
#define RUBIK_RANDOMIZE 4
#define RUBIK_DEC 5
#define RUBIK_ORIENT 6
#define RUBIK_INC 7

typedef struct _RubikClassRec *RubikWidgetClass;
typedef struct _RubikRec *RubikWidget;

extern WidgetClass rubikWidgetClass;

typedef struct
{
  XEvent *event;
  int reason;
  int face, i, j, direction;
} rubikCallbackStruct;

#endif _XtRubik_h
/* DON'T ADD STUFF AFTER THIS #endif */
