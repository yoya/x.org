/*
# X-BASED RUBIK'S CUBE
#
#  Rubik.c
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

/* Methods file for Rubik */

#include <stdio.h>
#include <math.h>
#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>
#include "RubikP.h"

static void InitializeRubik();
static void DestroyRubik();
static Boolean SetValuesRubik();
static void get_color();

RubikClassRec rubikClassRec =
{
  {
    (WidgetClass) &widgetClassRec,	/* superclass */
    "Rubik",				/* class name */
    sizeof(RubikRec),			/* widget size */
    NULL,				/* class initialize */
    NULL,				/* class part initialize */
    FALSE,				/* class inited */
    InitializeRubik,			/* initialize */
    NULL,				/* initialize hook */
    XtInheritRealize,			/* realize */
    NULL,				/* actions */
    0,					/* num actions */
    NULL,				/* resources */
    0,					/* num resources */
    NULLQUARK,				/* xrm class */
    TRUE,				/* compress motion */
    TRUE,				/* compress exposure */
    TRUE,				/* compress enterleave */
    TRUE,				/* visible interest */
    DestroyRubik,			/* destroy */
    NULL,				/* resize */
    NULL,				/* expose */
    SetValuesRubik,			/* set values */
    NULL,				/* set values hook */
    XtInheritSetValuesAlmost,		/* set values almost */
    NULL,				/* get values hook */
    NULL,				/* accept focus */
    XtVersion,				/* version */
    NULL,				/* callback private */
    NULL,				/* tm table */
    NULL,				/* query geometry */
    NULL,				/* display accelerator */
    NULL				/* extension */
  },
  {
    0					/* ignore */
  }
};

WidgetClass rubikWidgetClass = (WidgetClass) &rubikClassRec;

static void InitializeRubik(request, new)
RubikWidget request, new;
{
  XGCValues values;
  XtGCMask valueMask;
  int face;

  check_polyhedrons(new);
  reset_polyhedrons(new);
  new->rubik.practice = FALSE;
  SRAND(getpid());
  valueMask = GCForeground | GCBackground;
  values.foreground = new->rubik.foreground;
  values.background = new->core.background_pixel;
  new->rubik.puzzle_GC = XtGetGC((Widget) new, valueMask, &values);
  new->rubik.depth = DefaultDepthOfScreen(XtScreen(new));
  valueMask = GCForeground | GCBackground;
  values.foreground = new->core.background_pixel;
  values.background = new->rubik.foreground;
  new->rubik.inverse_GC = XtGetGC((Widget) new, valueMask, &values);
  for (face = 0; face < MAXFACES; face++)
    get_color(new, face, TRUE);
}

static void DestroyRubik(w)
RubikWidget w;
{
  int face;

  for (face = 0; face < MAXFACES; face++)
    XtReleaseGC((Widget) w, w->rubik.face_GC[face]);
  XtReleaseGC((Widget) w, w->rubik.puzzle_GC);
  XtReleaseGC((Widget) w, w->rubik.inverse_GC);
  XtRemoveCallbacks((Widget) w, XtNselectCallback, w->rubik.select);
}

static Boolean SetValuesRubik(current, request, new)
RubikWidget current, request, new;
{
  XGCValues values;
  XtGCMask valueMask;
  Boolean redraw = FALSE;
  int face;

  check_polyhedrons(new);
  if (new->rubik.foreground != current->rubik.foreground) {
    valueMask = GCForeground | GCBackground;
    values.foreground = new->rubik.foreground;
    values.background = new->core.background_pixel;
    XtReleaseGC((Widget) new, new->rubik.puzzle_GC);
    new->rubik.puzzle_GC = XtGetGC((Widget) new, valueMask, &values);
    if (new->rubik.mono || new->rubik.depth == 1) {
      for (face = 0; face < MAXFACES; face++) {
        values.background = new->core.background_pixel;
        values.foreground = new->rubik.foreground;
        XtReleaseGC((Widget) new, new->rubik.face_GC[face]);
        new->rubik.face_GC[face] = XtGetGC((Widget) new, valueMask, &values);
      }
      current->rubik.mono = new->rubik.mono;
    }
    redraw = TRUE;
  }
  if (new->core.background_pixel != current->core.background_pixel) {
    valueMask = GCForeground | GCBackground;
    values.foreground = new->core.background_pixel;
    values.background = new->rubik.foreground;
    XtReleaseGC((Widget) new, new->rubik.inverse_GC);
    new->rubik.inverse_GC = XtGetGC((Widget) new, valueMask, &values);
    redraw = TRUE;
  }
  for (face = 0; face < MAXFACES; face++) {
    if (strcmp(new->rubik.face_name[face], current->rubik.face_name[face]))
      get_color(new, face, FALSE);
  }
  if (new->rubik.orient != current->rubik.orient) {
    reset_polyhedrons(new);
    new->rubik.practice = FALSE;
    redraw = TRUE;
  } else if (new->rubik.practice != current->rubik.practice) {
    reset_polyhedrons(new);
    redraw = TRUE;
  }
  return redraw;
}

void quit_rubik(w, event, args, n_args)
RubikWidget w;
XEvent *event;
char *args[];
int n_args;
{
  XtCloseDisplay(XtDisplay(w));
  exit(0);
}

static void get_color(w, face, init)
RubikWidget w;
int face, init;
{
  XGCValues values;
  XtGCMask valueMask;
  XColor color_cell, rgb;
 
  valueMask = GCForeground | GCBackground;
  values.background = w->core.background_pixel;
  if (w->rubik.depth > 1 && !w->rubik.mono) {
    if (XAllocNamedColor(XtDisplay(w),
        DefaultColormap(XtDisplay(w), XtWindow(w)),
        w->rubik.face_name[face], &color_cell, &rgb)) {
      values.foreground = w->rubik.face_color[face] = color_cell.pixel;
      if (!init)
        XtReleaseGC((Widget) w, w->rubik.face_GC[face]);
      w->rubik.face_GC[face] = XtGetGC((Widget) w, valueMask, &values);
      return;
    } else {
      char buf[121];

      sprintf(buf, "Color name \"%s\" is not defined",
        w->rubik.face_name[face]);
      XtWarning(buf);
    }
  }
  values.foreground = w->rubik.foreground;
  if (!init)
    XtReleaseGC((Widget) w, w->rubik.face_GC[face]);
  w->rubik.face_GC[face] = XtGetGC((Widget) w, valueMask, &values);
}

void check_polyhedrons(new)
RubikWidget new;
{
  if (new->rubik.size < MINCUBES || new->rubik.size > MAXCUBES) {
    char buf[121];

    sprintf(buf, "Number of Cubes on edge out of bounds, use %d..%d",
      MINCUBES, MAXCUBES);
    XtWarning(buf);
    new->rubik.size = DEFAULTCUBES;
  }
}

void reset_polyhedrons(new)
RubikWidget new;
{
  int face, i, j;

  new->rubik.size_size = new->rubik.size * new->rubik.size;
  for (face = 0; face < MAXFACES; face++)
    for (i = 0; i < new->rubik.size; i++)
      for (j = 0; j < new->rubik.size; j++) {
        new->rubik.cube_loc[face][i][j].face = face; 
        new->rubik.cube_loc[face][i][j].rotation = STRT - MAXORIENT;
      }
  new->rubik.started = FALSE;
  new->rubik.randomized = FALSE;
}

void read_rc(w, face, dir, h, orient)
RubikWidget w;
int face, dir, h, orient;
{
  int g;

  if (dir == TOP || dir == BOTTOM)
    for (g = 0; g < w->rubik.size; g++)
      w->rubik.row_loc[orient][g] = w->rubik.cube_loc[face][h][g];
  else /* dir == RIGHT || dir == LEFT */
    for (g = 0; g < w->rubik.size; g++)
      w->rubik.row_loc[orient][g] = w->rubik.cube_loc[face][g][h];
}

void rotate_rc(w, rotate, orient)
RubikWidget w;
int rotate, orient;
{
  int g;

  for (g = 0; g < w->rubik.size; g++)
    w->rubik.row_loc[orient][g].rotation =
      (w->rubik.row_loc[orient][g].rotation + rotate) % MAXORIENT;
}

void reverse_rc(w, orient)
RubikWidget w;
int orient;
{
  int g;
  RubikLoc Temp[MAXCUBES_SQ];

  for (g = 0; g < w->rubik.size; g++)
    Temp[g] = w->rubik.row_loc[orient][g];
  for (g = 0; g < w->rubik.size; g++)
    w->rubik.row_loc[orient][g] = Temp[w->rubik.size - 1 - g];
}

int check_solved(w)
RubikWidget w;
{
  int face, i, j;
  RubikLoc test;

  for (face = 0; face < MAXFACES; face++)
    for (j = 0; j < w->rubik.size; j++)
      for (i = 0; i < w->rubik.size; i++)
      {
        if (i + j == 0)
        {
          test.face =
            w->rubik.cube_loc[face][i][j].face;
          test.rotation =
            w->rubik.cube_loc[face][i][j].rotation;
        }
        else if (test.face != /*face*/
                 w->rubik.cube_loc[face][i][j].face ||
                 (w->rubik.orient && test.rotation != /*STRT - MAXORIENT*/
                  w->rubik.cube_loc[face][i][j].rotation))
          return FALSE;
      }
  return TRUE;
}

#ifdef DEBUG

void print_cube(w)
RubikWidget w;
{
  int face, i, j;

  for (face = 0; face < MAXFACES; face++) {
    for (j = 0; j < w->rubik.size; j++) {
      for (i = 0; i < w->rubik.size; i++) {
         printf("%d %d  ", w->rubik.cube_loc[face][i][j].face,
                w->rubik.cube_loc[face][i][j].rotation);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}

void print_face(w)
RubikWidget w;
{
  int i, j;

  for (j = 0; j < w->rubik.size; j++) {
    for (i = 0; i < w->rubik.size; i++) {
       printf("%d %d  ", w->rubik.face_loc[i][j].face,
              w->rubik.face_loc[i][j].rotation);
     }
    printf("\n");
  }
  printf("\n");
}

void print_row(w, orient)
RubikWidget w;
int orient;
{
  int i;

  printf("Row %d:\n", orient);
  for (i = 0; i < w->rubik.size; i++)
    printf("%d %d  ", w->rubik.row_loc[orient][i].face,
           w->rubik.row_loc[orient][i].rotation);
  printf("\n");
}

#endif
