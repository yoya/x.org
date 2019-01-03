/*
 * Copyright 1989 Jon Bennett, Mike Bolotski, David Gagne, Dan Lovinger
 * Copyright 1986 Chris Gutherie
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in 
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The copyright holders make no 
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

long isin[] =
{
  -8192,
  -8189,
  -8182,
  -8169,
  -8152,
  -8130,
  -8103,
  -8071,
  -8034,
  -7992,
  -7947,
  -7895,
  -7839,
  -7779,
  -7713,
  -7643,
  -7568,
  -7489,
  -7405,
  -7317,
  -7225,
  -7127,
  -7027,
  -6921,
  -6811,
  -6697,
  -6580,
  -6458,
  -6333,
  -6203,
  -6070,
  -5933,
  -5793,
  -5649,
  -5501,
  -5351,
  -5197,
  -5040,
  -4880,
  -4717,
  -4552,
  -4383,
  -4212,
  -4038,
  -3862,
  -3683,
  -3503,
  -3321,
  -3135,
  -2949,
  -2760,
  -2570,
  -2378,
  -2185,
  -1991,
  -1795,
  -1599,
  -1401,
  -1203,
  -1003,
  -804,
  -603,
  -403,
  -202,
  0,
  199,
  400,
  601,
  801,
  1001,
  1200,
  1399,
  1596,
  1794,
  1989,
  2183,
  2376,
  2568,
  2758,
  2947,
  3133,
  3318,
  3501,
  3682,
  3860,
  4037,
  4210,
  4381,
  4549,
  4716,
  4879,
  5038,
  5196,
  5350,
  5500,
  5647,
  5791,
  5931,
  6068,
  6202,
  6331,
  6456,
  6578,
  6696,
  6810,
  6920,
  7025,
  7127,
  7223,
  7316,
  7404,
  7488,
  7567,
  7642,
  7712,
  7778,
  7838,
  7894,
  7946,
  7992,
  8034,
  8070,
  8102,
  8130,
  8152,
  8169,
  8182,
  8189,
  8192,
  8189,
  8182,
  8169,
  8152,
  8130,
  8103,
  8071,
  8034,
  7992,
  7946,
  7895,
  7838,
  7778,
  7712,
  7643,
  7568,
  7489,
  7405,
  7317,
  7224,
  7127,
  7026,
  6921,
  6811,
  6697,
  6579,
  6458,
  6332,
  6202,
  6070,
  5933,
  5792,
  5649,
  5501,
  5351,
  5197,
  5040,
  4879,
  4716,
  4551,
  4382,
  4211,
  4037,
  3861,
  3683,
  3502,
  3320,
  3135,
  2948,
  2759,
  2569,
  2378,
  2184,
  1990,
  1794,
  1598,
  1400,
  1202,
  1003,
  803,
  602,
  402,
  201,
  0,
  -200,
  -401,
  -602,
  -802,
  -1002,
  -1201,
  -1400,
  -1597,
  -1794,
  -1989,
  -2184,
  -2377,
  -2569,
  -2759,
  -2947,
  -3134,
  -3319,
  -3502,
  -3683,
  -3860,
  -4037,
  -4210,
  -4381,
  -4550,
  -4716,
  -4879,
  -5039,
  -5196,
  -5350,
  -5500,
  -5648,
  -5792,
  -5932,
  -6069,
  -6202,
  -6332,
  -6457,
  -6579,
  -6696,
  -6810,
  -6920,
  -7026,
  -7127,
  -7224,
  -7317,
  -7405,
  -7489,
  -7567,
  -7643,
  -7712,
  -7778,
  -7838,
  -7895,
  -7946,
  -7992,
  -8034,
  -8071,
  -8103,
  -8130,
  -8152,
  -8169,
  -8182,
  -8189
};
long icos[] =
{
  0,
  200,
  402,
  602,
  802,
  1002,
  1201,
  1400,
  1598,
  1794,
  1990,
  2184,
  2378,
  2569,
  2759,
  2948,
  3135,
  3319,
  3502,
  3683,
  3861,
  4037,
  4211,
  4382,
  4551,
  4716,
  4879,
  5039,
  5197,
  5351,
  5500,
  5648,
  5792,
  5932,
  6069,
  6202,
  6332,
  6457,
  6579,
  6697,
  6811,
  6920,
  7026,
  7127,
  7224,
  7317,
  7405,
  7489,
  7568,
  7643,
  7712,
  7778,
  7838,
  7895,
  7946,
  7992,
  8034,
  8071,
  8103,
  8130,
  8152,
  8169,
  8182,
  8189,
  8192,
  8189,
  8182,
  8169,
  8152,
  8130,
  8103,
  8071,
  8034,
  7992,
  7946,
  7895,
  7839,
  7778,
  7713,
  7643,
  7568,
  7489,
  7405,
  7317,
  7224,
  7127,
  7027,
  6921,
  6811,
  6697,
  6579,
  6458,
  6333,
  6203,
  6070,
  5933,
  5793,
  5649,
  5501,
  5351,
  5197,
  5040,
  4880,
  4717,
  4551,
  4383,
  4212,
  4038,
  3862,
  3683,
  3502,
  3320,
  3135,
  2949,
  2760,
  2570,
  2378,
  2185,
  1991,
  1795,
  1599,
  1400,
  1202,
  1003,
  803,
  603,
  403,
  201,
  0,
  -199,
  -401,
  -602,
  -801,
  -1001,
  -1200,
  -1400,
  -1597,
  -1794,
  -1989,
  -2183,
  -2377,
  -2569,
  -2759,
  -2947,
  -3134,
  -3318,
  -3502,
  -3682,
  -3860,
  -4037,
  -4210,
  -4381,
  -4550,
  -4716,
  -4879,
  -5038,
  -5196,
  -5350,
  -5500,
  -5648,
  -5791,
  -5932,
  -6069,
  -6202,
  -6331,
  -6457,
  -6578,
  -6696,
  -6810,
  -6920,
  -7026,
  -7127,
  -7224,
  -7317,
  -7404,
  -7489,
  -7567,
  -7642,
  -7712,
  -7778,
  -7838,
  -7894,
  -7946,
  -7992,
  -8034,
  -8071,
  -8103,
  -8130,
  -8152,
  -8169,
  -8182,
  -8189,
  -8192,
  -8189,
  -8182,
  -8169,
  -8152,
  -8130,
  -8103,
  -8071,
  -8034,
  -7992,
  -7947,
  -7895,
  -7839,
  -7779,
  -7713,
  -7643,
  -7568,
  -7489,
  -7406,
  -7317,
  -7225,
  -7128,
  -7027,
  -6921,
  -6812,
  -6698,
  -6580,
  -6458,
  -6333,
  -6203,
  -6071,
  -5934,
  -5793,
  -5650,
  -5502,
  -5351,
  -5197,
  -5041,
  -4880,
  -4717,
  -4552,
  -4383,
  -4212,
  -4038,
  -3862,
  -3684,
  -3503,
  -3321,
  -3135,
  -2949,
  -2761,
  -2570,
  -2379,
  -2186,
  -1992,
  -1796,
  -1599,
  -1401,
  -1203,
  -1004,
  -804,
  -603,
  -403,
  -202
};


/*
** table for computing inverse tangent values
*/


long iatan[] = {
0,
201,
402,
604,
806,
1010,
1215,
1421,
1629,
1839,
2051,
2267,
2485,
2706,
2931,
3160,
3393,
3631,
3874,
4123,
4378,
4640,
4910,
5187,
5473,
5769,
6075,
6393,
6723,
7066,
7424,
7799,
8192,
8604,
9038,
9496,
9981,
10497,
11045,
11631,
12260,
12936,
13667,
14460,
15326,
16274,
17320,
18480,
19777,
21236,
22895,
24797,
27005,
29602,
32704,
36480,
41183,
47211,
55225,
66418,
83174,
111056,
166751,
333705
};

#include <X11/Xlib.h>
#include "defs.h"

unsigned char iatan2(y, x)
int x, y;
{
  int bottom, top, i;
  long z, scaled_y;

  scaled_y = y << TRIGSCALE;
  if (x == 0) i = (y >= 0 ? 64 : 192);
  else {
    z = abs(scaled_y/x);

#ifdef DEBUG
    printf("Looking for closest table value to %ld\n", z);
#endif DEBUG

    bottom = 0;
    top = sizeof(iatan)/sizeof(long) - 1;

/*-------------------------------------------------------------------------
 * Loop Invariant: top - bottom > 1    &&   iatan[bottom] < z <= iatan[top]
 * 
 * At end of loop this guarantees:
 * top-bottom <= 1 && top-prev_bottom > 1  ==> top-bottom = 1
 * Also iatan[bottom] < z <= iatan[top].  So we will return the closer of
 * these two values.
 *-------------------------------------------------------------------------
 */
    while (top-bottom > 1) {
      i = bottom + ((top-bottom)>>1);
      if (iatan[i] < z) bottom = i;
      else top = i;
    }

    /* Choose the closer of the two values */
    i = (abs(iatan[top] - z) < abs(iatan[bottom] - z)) ? top : bottom;
  } /* else */
 
#ifdef DEBUG
      printf("original i: %d; ", i);
#endif DEBUG
  if (y == 0) i = (x >= 0) ? 0 : 128;
  else if (x < 0 && y > 0) i = 128-i;
  else if (x < 0 && y < 0) i += 128;
  else if (x > 0 && y < 0) i = 256-i;
  
#ifdef DEBUG
  printf("iatan2(%d, %d) = %d\n", y, x, i);
#endif DEBUG

  return(i);
}

/*
 * The isin and icos routines in trigtab.c are made to work in the obscure
 * coordinate system used by xtrek (angles measured from the positive y axis).
 * This routine snags out 1/4 of the isin table and uses it to produce
 * values of the sin function from the normal coordinate system.  This
 * has the advantage of making things like the law of sines work.
 */
int
normal_isin(in_angle)
unsigned char in_angle;
{
  int minus = 1;
  unsigned char angle = in_angle;

  /* sin(a) = -sin(a-pi) */
  if (angle > 128) {
    angle = angle - 128;
    minus *= -1;				/* why is this? */
  }
  /* sin(a) = sin(pi-a) */
  if (angle > 64) {
    angle = 128 - angle;
  }

  angle = angle + 64; /* stupid isin table is broken */
  return(isin[angle]);
}




