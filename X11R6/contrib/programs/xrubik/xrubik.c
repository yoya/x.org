/*
# X-BASED RUBIK'S CUBE
#
#  xrubik.c
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

/*
  Version 4: 94/04/07 Xt
  Version 3: 93/05/20 Motif
  Version 2: 92/01/16 XView
  Version 1: 91/01/16 SunView
*/

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include "Rubik.h"
#include "Rubik2d.h"
#include "Rubik3d.h"
#include "rubik.bit"

#ifndef SCOREFILE
#define SCOREFILE "/usr/games/lib/rubik.scores"
#endif

/* The following 3 are in RubikP.h also */
#define MINCUBES 1
#define MAXCUBES 6
#define MAXFACES 6

#define MAXRECORD 32767
#define MAXPROGNAME 80
#define MAXNAME 256

void initialize();
void moves_text();

Widget rubik2d, rubik3d;
Arg arg[5];
int rubik_record[2][MAXCUBES - MINCUBES + 1], moves = 0;
char prog[64] = "xrubik";
char record[16] = "INF";
char message[128] = "Randomize to start";
char title[256] = "";

void usage()
{
  fprintf(stderr, "usage: xrubik\n");
  fprintf(stderr,
    "\t[-geometry [{width}][x{height}][{+-}{xoff}[{+-}{yoff}]]]\n");
  fprintf(stderr,
    "\t[-display [{host}]:[{vs}]][-fg {color}] [-bg {color}]\n");
  fprintf(stderr,
    "\t[-size {int}] [-[no]orient] [-mono] [-face{0|1|2|3|4|5} {color}]\n");
  exit(1);
}

static XrmOptionDescRec options[] = {
  {"-fg",		"rubik.Foreground",	XrmoptionSepArg,	NULL},
  {"-bd",		"rubik.Foreground",	XrmoptionSepArg,	NULL},
  {"-bg",		"*Background",		XrmoptionSepArg,	NULL},
  {"-foreground",	"rubik.Foreground",	XrmoptionSepArg,	NULL},
  {"-background",	"*Background",		XrmoptionSepArg,	NULL},
  {"-bordercolor",	"rubik.Foreground",	XrmoptionSepArg,	NULL},
  {"-size",		"rubik.size",		XrmoptionSepArg,	NULL},
  {"-orient",		"rubik.orient",		XrmoptionNoArg,		"TRUE"},
  {"-noorient",		"rubik.orient",		XrmoptionNoArg,		"FALSE"},
  {"-mono",		"rubik.mono",		XrmoptionNoArg,		"TRUE"},
  {"-face0",		"rubik.faceColor0",	XrmoptionSepArg,	NULL},
  {"-face1",		"rubik.faceColor1",	XrmoptionSepArg,	NULL},
  {"-face2",		"rubik.faceColor2",	XrmoptionSepArg,	NULL},
  {"-face3",		"rubik.faceColor3",	XrmoptionSepArg,	NULL},
  {"-face4",		"rubik.faceColor4",	XrmoptionSepArg,	NULL},
  {"-face5",		"rubik.faceColor5",	XrmoptionSepArg,	NULL},
};

void main(argc, argv)
int argc;
char *argv[];
{
  Widget toplevel, shell;

  toplevel = XtInitialize(argv[0], "Rubik",
    options, XtNumber(options), &argc, argv);
  if (argc != 1)
    usage();

  shell = XtCreateApplicationShell(argv[0], topLevelShellWidgetClass, NULL,
    0);
  XtSetArg(arg[0], XtNiconPixmap,
    XCreateBitmapFromData(XtDisplay(toplevel), XtScreen(toplevel)->root,
      rubik_bits, rubik_width, rubik_height));
  XtSetValues(toplevel, arg, 1);
  XtSetArg(arg[0], XtNiconPixmap, 
    XCreateBitmapFromData(XtDisplay(shell), XtScreen(shell)->root,
      rubik_bits, rubik_width, rubik_height));
  XtSetValues(shell, arg, 1);
  rubik2d = XtCreateManagedWidget("rubik", rubik2dWidgetClass, toplevel,
    NULL, 0);
  XtAddCallback(rubik2d, XtNselectCallback, moves_text, NULL);
  rubik3d = XtCreateManagedWidget("rubik", rubik3dWidgetClass, shell,
    NULL, 0);
  XtAddCallback(rubik3d, XtNselectCallback, moves_text, NULL);
  initialize();
  XtRealizeWidget(toplevel);
  XtRealizeWidget(shell);
  XGrabButton(XtDisplay(rubik2d), AnyButton, AnyModifier, XtWindow(rubik2d),
    TRUE, ButtonPressMask | ButtonMotionMask | ButtonReleaseMask,
    GrabModeAsync, GrabModeAsync, XtWindow(rubik2d),
    XCreateFontCursor(XtDisplay(rubik2d), XC_crosshair));
  XGrabButton(XtDisplay(rubik3d), AnyButton, AnyModifier, XtWindow(rubik3d),
    TRUE, ButtonPressMask | ButtonMotionMask | ButtonReleaseMask,
    GrabModeAsync, GrabModeAsync, XtWindow(rubik3d),
    XCreateFontCursor(XtDisplay(rubik3d), XC_crosshair));
  XtMainLoop();
}

/* There's probably a better way to assure that they are the same but
   I don't know it off hand. */
void  make_equivalent(size, orient)
int *size;
Boolean *orient;
{
  Boolean mono;
  Pixel foreground, background;
  String facecolor[MAXFACES];

  XtVaGetValues(rubik2d,
    XtNsize, size,
    XtNorient, orient,
    XtNmono, &mono,
    XtNforeground, &foreground,
    XtNbackground, &background,
    XtNfaceColor0, &(facecolor[0]),
    XtNfaceColor1, &(facecolor[1]),
    XtNfaceColor2, &(facecolor[2]),
    XtNfaceColor3, &(facecolor[3]),
    XtNfaceColor4, &(facecolor[4]),
    XtNfaceColor5, &(facecolor[5]),
    NULL);
  XtVaSetValues(rubik2d,
    XtNface, -1,
    XtNpractice, FALSE,
    XtNstart, FALSE,
    NULL);
  XtVaSetValues(rubik3d,
    XtNsize, *size,
    XtNorient, *orient,
    XtNmono, mono,
    XtNface, -1,
    XtNpractice, FALSE,
    XtNstart, FALSE,
    XtNforeground, foreground,
    XtNbackground, background,
    XtNfaceColor0, facecolor[0],
    XtNfaceColor1, facecolor[1],
    XtNfaceColor2, facecolor[2],
    XtNfaceColor3, facecolor[3],
    XtNfaceColor4, facecolor[4],
    XtNfaceColor5, facecolor[5],
    NULL);
}

void initialize()
{
  int size;
  Boolean orient;

  make_equivalent(&size, &orient);
  read_records();
  print_record(orient, size, record);
  print_state(XtParent(rubik2d), prog, 2, size, moves, record, message);
  print_state(XtParent(rubik3d), prog, 3, size, moves, record, message);
}

void moves_text(w, client_data, call_data)
Widget w;
caddr_t client_data;
rubikCallbackStruct *call_data;
{
  int size, dim, otherdim;
  Boolean orient, practice;
  Widget otherw;

  if (w == rubik2d) {
    dim = 2;
    otherw = rubik3d;
    otherdim = 3;
  } else { /* (w == rubik3d) */
    dim = 3;
    otherw = rubik2d;
    otherdim = 2;
  }
  XtVaGetValues(w,
    XtNsize, &size,
    XtNorient, &orient,
    XtNpractice, &practice,
    NULL);
  strcpy(message, "");
  switch (call_data->reason) {
    case RUBIK_IGNORE:
      strcpy(message, "Randomize to start");
      break;
    case RUBIK_MOVED:
      moves++;
      XtSetArg(arg[0], XtNstart, TRUE);
      XtSetArg(arg[1], XtNface, call_data->face);
      XtSetArg(arg[2], XtNi, call_data->i);
      XtSetArg(arg[3], XtNj, call_data->j);
      XtSetArg(arg[4], XtNdirection, call_data->direction);
      XtSetValues(otherw, arg, 5);
      XtSetValues(w, arg, 1);
      break;
    case RUBIK_CONTROL:
      XtSetArg(arg[0], XtNface, call_data->face);
      XtSetArg(arg[1], XtNi, call_data->i);
      XtSetArg(arg[2], XtNj, call_data->j);
      XtSetArg(arg[3], XtNdirection, call_data->direction);
      XtSetValues(otherw, arg, 4);
      return;
    case RUBIK_SOLVED:
      if (practice)
        moves = 0;
      else { 
        if (handle_solved(moves, size, orient))
          sprintf(message, "Congratulations %s!!", getlogin());
        else
          strcpy(message, "Solved!");
      }
      XtSetArg(arg[0], XtNstart, FALSE);
      XtSetValues(w, arg, 1);
      XtSetValues(otherw, arg, 1);
      break;
    case RUBIK_PRACTICE:
      moves = 0;
      practice = !practice;
      if (practice)
        strcpy(record, "practice");
      else {
        strcpy(message, "Randomize to start");
        print_record(orient, size, record);
      }
      XtSetArg(arg[0], XtNpractice, practice);
      XtSetArg(arg[1], XtNstart, FALSE);
      XtSetValues(w, arg, 2);
      XtSetValues(otherw, arg, 2);
      break;
    case RUBIK_RANDOMIZE:
      moves = 0;
      XtSetArg(arg[0], XtNpractice, FALSE);
      XtSetArg(arg[1], XtNstart, FALSE);
      XtSetValues(w, arg, 2);
      XtSetValues(otherw, arg, 2);
      break; 
    case RUBIK_DEC:
      moves = 0;
      size--;
      print_record(orient, size, record);
      XtSetArg(arg[0], XtNsize, size);
      XtSetValues(w, arg, 1);
      XtSetValues(otherw, arg, 1);
      break;
    case RUBIK_ORIENT:
      moves = 0;
      orient = !orient;
      print_record(orient, size, record);
      XtSetArg(arg[0], XtNorient, orient);
      XtSetValues(w, arg, 1);
      XtSetValues(otherw, arg, 1);
      break;
    case RUBIK_INC:
      moves = 0;
      size++;
      print_record(orient, size, record);
      XtSetArg(arg[0], XtNsize, size);
      XtSetValues(w, arg, 1);
      XtSetValues(otherw, arg, 1);
      break;
  }
  print_state(XtParent(w), prog, dim, size, moves, record, message);
  print_state(XtParent(otherw), prog, otherdim, size, moves, record, message);
}

print_record(orient, size, record)
Boolean orient;
int size;
char *record;
{
  if (rubik_record[orient][size - MINCUBES] >= MAXRECORD)
    strcpy(record, "NEVER");
  else
    sprintf(record, "%d", rubik_record[orient][size - MINCUBES]);
}

int handle_solved(counter, size, orient)
int counter, size;
Boolean orient;
{
  if (counter < rubik_record[orient][size - MINCUBES]) {
    rubik_record[orient][size - MINCUBES] = counter;
    if (size < 2 || (orient && rubik_record[orient][size - MINCUBES] < 
                       rubik_record[!orient][size - MINCUBES]))
      rubik_record[!orient][size - MINCUBES] = counter;
    write_records();
    sprintf(record, "%d", rubik_record[orient][size - MINCUBES]);
    return TRUE;
  }
  return FALSE;
}

print_state(w, prog, dim, size, moves, record, message)
Widget w;
char *prog, *record, *message;
int dim, size, moves;
{
  sprintf(title, "%s%dd: %d@ (%d/%s) - %s", prog, dim, size, moves,
    record, message);
  XtSetArg(arg[0], XtNtitle, title);
  XtSetValues(w, arg, 1);
}

read_records()
{
  FILE *fp, *fopen();
  int i, n, orient;

  for (orient = 0; orient < 2; orient++)
    for (i = 0; i < MAXCUBES - MINCUBES + 1; i++)
      rubik_record[orient][i] = MAXRECORD;
  if ((fp = fopen(SCOREFILE, "r")) == NULL)
    sprintf(message, "Can not open %s, taking defaults.", SCOREFILE);
  else {
    for (orient = 0; orient < 2; orient++)
      for (i = 0; i < MAXCUBES - MINCUBES + 1; i++) {
        fscanf(fp, "%d", &n);
        rubik_record[orient][i] = n;
      }
    fclose(fp);
  }
}

write_records()
{
  FILE *fp, *fopen();
  int i, orient;

  if ((fp = fopen(SCOREFILE, "w")) == NULL)
    sprintf(message, "Can not write to %s.", SCOREFILE);
  else {
    for (orient = 0; orient < 2; orient++) {
      for (i = 0; i < MAXCUBES - MINCUBES + 1; i++)
        fprintf(fp, "%d ", rubik_record[orient][i]);
      fprintf(fp, "\n");
    }
    fclose(fp);
  }
}
