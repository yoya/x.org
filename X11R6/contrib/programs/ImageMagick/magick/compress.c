/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%           CCCC   OOO   M   M  PPPP   RRRR   EEEEE   SSSSS  SSSSS            %
%          C      O   O  MM MM  P   P  R   R  E       SS     SS               %
%          C      O   O  M M M  PPPP   RRRR   EEE      SSS    SSS             %
%          C      O   O  M   M  P      R R    E          SS     SS            %
%           CCCC   OOO   M   M  P      R  R   EEEEE   SSSSS  SSSSS            %
%                                                                             %
%                                                                             %
%                  Image Compression/Decompression Coders                     %
%                                                                             %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                              May  1993                                      %
%                                                                             %
%                                                                             %
%  Copyright 1994 E. I. du Pont de Nemours and Company                        %
%                                                                             %
%  Permission to use, copy, modify, distribute, and sell this software and    %
%  its documentation for any purpose is hereby granted without fee,           %
%  provided that the above Copyright notice appear in all copies and that     %
%  both that Copyright notice and this permission notice appear in            %
%  supporting documentation, and that the name of E. I. du Pont de Nemours    %
%  and Company not be used in advertising or publicity pertaining to          %
%  distribution of the software without specific, written prior               %
%  permission.  E. I. du Pont de Nemours and Company makes no representations %
%  about the suitability of this software for any purpose.  It is provided    %
%  "as is" without express or implied warranty.                               %
%                                                                             %
%  E. I. du Pont de Nemours and Company disclaims all warranties with regard  %
%  to this software, including all implied warranties of merchantability      %
%  and fitness, in no event shall E. I. du Pont de Nemours and Company be     %
%  liable for any special, indirect or consequential damages or any           %
%  damages whatsoever resulting from loss of use, data or profits, whether    %
%  in an action of contract, negligence or other tortious action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "image.h"
#include "utility.h"

/*
  Define declarations.
*/
#define LowerBound  0
#define MaxContextStates  121
#define MinimumIntervalD  (unsigned short) 0xf000  /* ~-0.75 */
#define MinimumIntervalE  (unsigned short) 0x1000  /* ~0.75 */
#define No  0
#define UpperBound  2
#define Yes  1
/*
  State classification.
*/
#define ZeroState  0
#define SmallPostitiveState  1
#define SmallNegativeState  2
#define LargePostitiveState  3
#define LargeNegativeState  4

/*
  Typedef declarations.
*/
typedef struct HuffmanTable
{
  int
    id,
    code,
    length,
    count;
} HuffmanTable;

typedef struct _ScanlinePacket
{
  unsigned char
    pixel;

  int
    state;
} ScanlinePacket;

/*
  Huffman coding declarations.
*/
#define TWId  23
#define MWId  24
#define TBId  25
#define MBId  26
#define EXId  27

static HuffmanTable
  MBTable[]=
  {
    { MBId, 0x0f, 10, 64 }, { MBId, 0xc8, 12, 128 },
    { MBId, 0xc9, 12, 192 }, { MBId, 0x5b, 12, 256 },
    { MBId, 0x33, 12, 320 }, { MBId, 0x34, 12, 384 },
    { MBId, 0x35, 12, 448 }, { MBId, 0x6c, 13, 512 },
    { MBId, 0x6d, 13, 576 }, { MBId, 0x4a, 13, 640 },
    { MBId, 0x4b, 13, 704 }, { MBId, 0x4c, 13, 768 },
    { MBId, 0x4d, 13, 832 }, { MBId, 0x72, 13, 896 },
    { MBId, 0x73, 13, 960 }, { MBId, 0x74, 13, 1024 },
    { MBId, 0x75, 13, 1088 }, { MBId, 0x76, 13, 1152 },
    { MBId, 0x77, 13, 1216 }, { MBId, 0x52, 13, 1280 },
    { MBId, 0x53, 13, 1344 }, { MBId, 0x54, 13, 1408 },
    { MBId, 0x55, 13, 1472 }, { MBId, 0x5a, 13, 1536 },
    { MBId, 0x5b, 13, 1600 }, { MBId, 0x64, 13, 1664 },
    { MBId, 0x65, 13, 1728 }, { MBId, 0x00, 0, 0 }
  };

static HuffmanTable
  EXTable[]=
  {
    { EXId, 0x08, 11, 1792 }, { EXId, 0x0c, 11, 1856 },
    { EXId, 0x0d, 11, 1920 }, { EXId, 0x12, 12, 1984 },
    { EXId, 0x13, 12, 2048 }, { EXId, 0x14, 12, 2112 },
    { EXId, 0x15, 12, 2176 }, { EXId, 0x16, 12, 2240 },
    { EXId, 0x17, 12, 2304 }, { EXId, 0x1c, 12, 2368 },
    { EXId, 0x1d, 12, 2432 }, { EXId, 0x1e, 12, 2496 },
    { EXId, 0x1f, 12, 2560 }, { EXId, 0x00, 0, 0 }
  };

static HuffmanTable
  MWTable[]=
  {
    { MWId, 0x1b, 5, 64 }, { MWId, 0x12, 5, 128 },
    { MWId, 0x17, 6, 192 }, { MWId, 0x37, 7, 256 },
    { MWId, 0x36, 8, 320 }, { MWId, 0x37, 8, 384 },
    { MWId, 0x64, 8, 448 }, { MWId, 0x65, 8, 512 },
    { MWId, 0x68, 8, 576 }, { MWId, 0x67, 8, 640 },
    { MWId, 0xcc, 9, 704 }, { MWId, 0xcd, 9, 768 },
    { MWId, 0xd2, 9, 832 }, { MWId, 0xd3, 9, 896 },
    { MWId, 0xd4, 9, 960 }, { MWId, 0xd5, 9, 1024 },
    { MWId, 0xd6, 9, 1088 }, { MWId, 0xd7, 9, 1152 },
    { MWId, 0xd8, 9, 1216 }, { MWId, 0xd9, 9, 1280 },
    { MWId, 0xda, 9, 1344 }, { MWId, 0xdb, 9, 1408 },
    { MWId, 0x98, 9, 1472 }, { MWId, 0x99, 9, 1536 },
    { MWId, 0x9a, 9, 1600 }, { MWId, 0x18, 6, 1664 },
    { MWId, 0x9b, 9, 1728 }, { MWId, 0x00, 0, 0 }
  };

static HuffmanTable
  TBTable[]=
  {
    { TBId, 0x37, 10, 0 }, { TBId, 0x02, 3, 1 }, { TBId, 0x03, 2, 2 },
    { TBId, 0x02, 2, 3 }, { TBId, 0x03, 3, 4 }, { TBId, 0x03, 4, 5 },
    { TBId, 0x02, 4, 6 }, { TBId, 0x03, 5, 7 }, { TBId, 0x05, 6, 8 },
    { TBId, 0x04, 6, 9 }, { TBId, 0x04, 7, 10 }, { TBId, 0x05, 7, 11 },
    { TBId, 0x07, 7, 12 }, { TBId, 0x04, 8, 13 }, { TBId, 0x07, 8, 14 },
    { TBId, 0x18, 9, 15 }, { TBId, 0x17, 10, 16 }, { TBId, 0x18, 10, 17 },
    { TBId, 0x08, 10, 18 }, { TBId, 0x67, 11, 19 }, { TBId, 0x68, 11, 20 },
    { TBId, 0x6c, 11, 21 }, { TBId, 0x37, 11, 22 }, { TBId, 0x28, 11, 23 },
    { TBId, 0x17, 11, 24 }, { TBId, 0x18, 11, 25 }, { TBId, 0xca, 12, 26 },
    { TBId, 0xcb, 12, 27 }, { TBId, 0xcc, 12, 28 }, { TBId, 0xcd, 12, 29 },
    { TBId, 0x68, 12, 30 }, { TBId, 0x69, 12, 31 }, { TBId, 0x6a, 12, 32 },
    { TBId, 0x6b, 12, 33 }, { TBId, 0xd2, 12, 34 }, { TBId, 0xd3, 12, 35 },
    { TBId, 0xd4, 12, 36 }, { TBId, 0xd5, 12, 37 }, { TBId, 0xd6, 12, 38 },
    { TBId, 0xd7, 12, 39 }, { TBId, 0x6c, 12, 40 }, { TBId, 0x6d, 12, 41 },
    { TBId, 0xda, 12, 42 }, { TBId, 0xdb, 12, 43 }, { TBId, 0x54, 12, 44 },
    { TBId, 0x55, 12, 45 }, { TBId, 0x56, 12, 46 }, { TBId, 0x57, 12, 47 },
    { TBId, 0x64, 12, 48 }, { TBId, 0x65, 12, 49 }, { TBId, 0x52, 12, 50 },
    { TBId, 0x53, 12, 51 }, { TBId, 0x24, 12, 52 }, { TBId, 0x37, 12, 53 },
    { TBId, 0x38, 12, 54 }, { TBId, 0x27, 12, 55 }, { TBId, 0x28, 12, 56 },
    { TBId, 0x58, 12, 57 }, { TBId, 0x59, 12, 58 }, { TBId, 0x2b, 12, 59 },
    { TBId, 0x2c, 12, 60 }, { TBId, 0x5a, 12, 61 }, { TBId, 0x66, 12, 62 },
    { TBId, 0x67, 12, 63 }, { TBId, 0x00, 0, 0 }
  };

static HuffmanTable
  TWTable[]=
  {
    { TWId, 0x35, 8, 0 }, { TWId, 0x07, 6, 1 }, { TWId, 0x07, 4, 2 },
    { TWId, 0x08, 4, 3 }, { TWId, 0x0b, 4, 4 }, { TWId, 0x0c, 4, 5 },
    { TWId, 0x0e, 4, 6 }, { TWId, 0x0f, 4, 7 }, { TWId, 0x13, 5, 8 },
    { TWId, 0x14, 5, 9 }, { TWId, 0x07, 5, 10 }, { TWId, 0x08, 5, 11 },
    { TWId, 0x08, 6, 12 }, { TWId, 0x03, 6, 13 }, { TWId, 0x34, 6, 14 },
    { TWId, 0x35, 6, 15 }, { TWId, 0x2a, 6, 16 }, { TWId, 0x2b, 6, 17 },
    { TWId, 0x27, 7, 18 }, { TWId, 0x0c, 7, 19 }, { TWId, 0x08, 7, 20 },
    { TWId, 0x17, 7, 21 }, { TWId, 0x03, 7, 22 }, { TWId, 0x04, 7, 23 },
    { TWId, 0x28, 7, 24 }, { TWId, 0x2b, 7, 25 }, { TWId, 0x13, 7, 26 },
    { TWId, 0x24, 7, 27 }, { TWId, 0x18, 7, 28 }, { TWId, 0x02, 8, 29 },
    { TWId, 0x03, 8, 30 }, { TWId, 0x1a, 8, 31 }, { TWId, 0x1b, 8, 32 },
    { TWId, 0x12, 8, 33 }, { TWId, 0x13, 8, 34 }, { TWId, 0x14, 8, 35 },
    { TWId, 0x15, 8, 36 }, { TWId, 0x16, 8, 37 }, { TWId, 0x17, 8, 38 },
    { TWId, 0x28, 8, 39 }, { TWId, 0x29, 8, 40 }, { TWId, 0x2a, 8, 41 },
    { TWId, 0x2b, 8, 42 }, { TWId, 0x2c, 8, 43 }, { TWId, 0x2d, 8, 44 },
    { TWId, 0x04, 8, 45 }, { TWId, 0x05, 8, 46 }, { TWId, 0x0a, 8, 47 },
    { TWId, 0x0b, 8, 48 }, { TWId, 0x52, 8, 49 }, { TWId, 0x53, 8, 50 },
    { TWId, 0x54, 8, 51 }, { TWId, 0x55, 8, 52 }, { TWId, 0x24, 8, 53 },
    { TWId, 0x25, 8, 54 }, { TWId, 0x58, 8, 55 }, { TWId, 0x59, 8, 56 },
    { TWId, 0x5a, 8, 57 }, { TWId, 0x5b, 8, 58 }, { TWId, 0x4a, 8, 59 },
    { TWId, 0x4b, 8, 60 }, { TWId, 0x32, 8, 61 }, { TWId, 0x33, 8, 62 },
    { TWId, 0x34, 8, 63 }, { TWId, 0x00, 0, 0 }
  };

/*
  Static Q-coder declarations.
*/
static int
  decrement_less_probable[]=
  {
    0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 3, 2, 3, 2, 3, 2
  };

static int
  increment_more_probable[]=
  {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0
  };

static int
  more_probable_exchange[]=
  {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  };

static int
  statistics[][5]=
  {
     0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
    68, 72, 76, 80, 84, 88, 92, 96
  };

static unsigned short
  probability[]=
  {
    0x0ac1, 0x0a81, 0x0a01, 0x0901, 0x0701, 0x0681,
    0x0601, 0x0501, 0x0481, 0x0441, 0x0381, 0x0301,
    0x02c1, 0x0281, 0x0241, 0x0181, 0x0121, 0x00e1,
    0x00a1, 0x0071, 0x0059, 0x0053, 0x0027, 0x0017,
    0x0013, 0x000b, 0x0007, 0x0005, 0x0003, 0x0001
  };
/*
  Declarations and initializations for predictive arithimetic coder.
*/
static int
  code,
  less_probable[MaxContextStates],
  more_probable[MaxContextStates],
  probability_estimate[MaxContextStates];

static unsigned char
  *q;

static unsigned short
  interval;

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   B M P D e c o d e I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function BMPDecodeImage unpacks the packed image pixels into
%  runlength-encoded pixel packets.
%
%  The format of the BMPDecodeImage routine is:
%
%      status=BMPDecodeImage(compressed_pixels,pixels,compression,
%        number_columns,number_rows)
%
%  A description of each parameter follows:
%
%    o status:  Function BMPDecodeImage returns True if all the pixels are
%      uncompressed without error, otherwise False.
%
%    o compressed_pixels:  The address of a byte (8 bits) array of compressed
%      pixel data.
%
%    o pixels:  The address of a byte (8 bits) array of pixel data created by
%      the decoding process.
%
%    o compression:  A value of 1 means the compressed pixels are runlength
%      encoded for a 256-color bitmap.  A value of 2 means a 16-color bitmap.
%
%    o number_columns:  An integer value that is the number of columns or
%      width in pixels of your source image.
%
%    o number_rows:  An integer value that is the number of rows or
%      heigth in pixels of your source image.
%
%
*/
unsigned int BMPDecodeImage(compressed_pixels,pixels,compression,
  number_columns,number_rows)
unsigned char
  *compressed_pixels,
  *pixels;

unsigned int
  compression,
  number_columns,
  number_rows;
{
  register int
    i,
    x,
    y;

  register unsigned char
    *p,
    *q;

  unsigned char
    byte,
    count;

  p=compressed_pixels;
  q=pixels;
  x=0;
  for (y=0; y < number_rows; )
  {
    count=(*p++);
    if (count != 0)
      {
        /*
          Encoded mode.
        */
        byte=(*p++);
        for (i=0; i < (int) count; i++)
        {
          if (compression == 1)
            *q++=byte;
          else
            *q++=(i & 0x01) ? (byte & 0x0f) : ((byte >> 4) & 0x0f);
          x++;
        }
      }
    else
      {
        /*
          Escape mode.
        */
        count=(*p++);
        if (count == 0x01)
          return(True);
        switch (count)
        {
          case 0x00:
          {
            /*
              End of line.
            */
            x=0;
            y++;
            q=pixels+y*number_columns;
            break;
          }
          case 0x02:
          {
            /*
              Delta mode.
            */
            x+=(*p++);
            y+=(*p++);
            q=pixels+y*number_columns+x;
            break;
          }
          default:
          {
            /*
              Absolute mode.
            */
            for (i=0; i < (int) count; i++)
            {
              if (compression == 1)
                *q++=(*p++);
              else
                {
                  if ((i & 0x01) == 0)
                    byte=(*p++);
                  *q++=(i & 0x01) ? (byte & 0x0f) : ((byte >> 4) & 0x0f);
                }
              x++;
            }
            /*
              Read pad byte.
            */
            if (compression == 1)
              {
                if (count & 0x01)
                  p++;
              }
            else
              if (((count & 0x03) == 1) || ((count & 0x03) == 2))
                p++;
            break;
          }
        }
     }
  }
  return(False);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   B M P E n c o d e I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function BMPEncodeImage compresses pixels using a runlength encoded format.
%
%  The format of the BMPEncodeImage routine is:
%
%      status=BMPEncodeImage(pixels,compressed_pixels,number_columns,
%        number_rows)
%
%  A description of each parameter follows:
%
%    o status:  Function BMPEncodeImage returns the number of bytes in the
%      runlength encoded compress_pixels array.
%
%    o pixels:  The address of a byte (8 bits) array of pixel data created by
%      the compression process.
%
%    o compressed_pixels:  The address of a byte (8 bits) array of compressed
%      pixel data.
%
%    o number_columns:  An integer value that is the number of columns or
%      width in pixels of your source image.
%
%    o number_rows:  An integer value that is the number of rows or
%      heigth in pixels of your source image.
%
%
*/
unsigned int BMPEncodeImage(pixels,compressed_pixels,number_columns,number_rows)
unsigned char
  *pixels,
  *compressed_pixels;

unsigned int
  number_columns,
  number_rows;
{
  register int
    i,
    x,
    y;

  register unsigned char
    *p,
    *q;

  /*
    Runlength encode pixels.
  */
  p=pixels;
  q=compressed_pixels;
  for (y=0; y < number_rows; y++)
  {
    for (x=0; x < number_columns; x+=i)
    {
      /*
        Determine runlength.
      */
      for (i=1; ((x+i) < number_columns); i++)
        if ((*(p+i) != *p) || (i == 255))
          break;
      *q++=i;
      *q++=(*p);
      p+=i;
    }
    /*
      End of line.
    */
    *q++=0;
    *q++=0x00;
  }
  /*
    End of bitmap.
  */
  *q++=0;
  *q++=0x01;
  return(q-compressed_pixels);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   D e c o d e                                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function Decode uncompresses a string.
%
%  The format of the Decode routine is:
%
%      Decode(state,decision)
%
%  A description of each parameter follows:
%
%    o state:  An integer value representing the current state.
%
%    o decision:  A pointer to an integer.  The output of the binary
%      decision (Yes/No) is returned in this value.
%
%
*/
static void Decode(state,decision)
register int
  state,
  *decision;
{
  interval+=probability[probability_estimate[state]];
  if (((code >> 16) & 0xffff) < ((int) interval))
    {
      code-=(interval << 16);
      interval=(-probability[probability_estimate[state]]);
      *decision=less_probable[state];
    }
  else
    {
      *decision=more_probable[state];
      if (interval <= MinimumIntervalD)
        return;
    }
  do
  {
    if ((code & 0xff) == 0)
      {
        code&=0xffff0000;
        if ((*q++) == 0xff)
          code+=((int) (*q) << 9)+0x02;
        else
          code+=((int) (*q) << 8)+0x01;
      }
    interval<<=1;
    code<<=1;
  } while (interval > MinimumIntervalD);
  /*
    Update probability estimates.
  */
  if (*decision == more_probable[state])
    probability_estimate[state]+=
      increment_more_probable[probability_estimate[state]];
  else
    probability_estimate[state]-=
      decrement_less_probable[probability_estimate[state]];
  if (more_probable_exchange[probability_estimate[state]] != 0)
    {
      /*
        Exchange sense of most probable and least probable.
      */
      less_probable[state]=more_probable[state];
      more_probable[state]=1-more_probable[state];
    }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   E n c o d e                                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function Encode generate compressed data string by encoding yes-no decision
%  given state s.
%
%  The format of the Encode routine is:
%
%      Encode(state,decision)
%
%  A description of each parameter follows:
%
%    o state:  An integer value representing the current state.
%
%    o decision:  An integer value representing a binary decision.
%
%
*/
static void Encode(state,decision)
register int
  state,
  decision;
{
  /*
    Test on "most-probable-symbol" for state s(more_probable[state])
  */
  interval-=probability[probability_estimate[state]];
  if (more_probable[state] != decision)
    {
      code-=interval;
      interval=probability[probability_estimate[state]];
    }
  else
    if (interval >= MinimumIntervalE)
      return;
  /*
    Encoder renormalization.
  */
  do
  {
    interval<<=1;
    if (code >= 0)
      code<<=1;
    else
      {
        /*
          Shift unsigned char of data from Code register to compressed string.
        */
        code<<=1;
        if (code > 0)
          {
            /*
              Add eight bits from Code register to compressed data string.
            */
            (*q++)--;
            *q=(unsigned char) (code >> 16);
            code&=0x0000ffff;
            code|=0x01800000;
          }
        else
          {
            code&=0x01ffffff;
            if ((int) interval > code)
              {
                /*
                  Add eight bits from Code register to compressed data string.
                */
                (*q++)--;
                *q=0xff;
                code|=0x01810000;
              }
            else
              if ((*q++) == 0xff)
                {
                  /*
                    Add seven bits from Code register plus one stuffed bit to
                    compressed data string.
                  */
                  *q=(unsigned char) (code >> 17);
                  code&=0x0001ffff;
                  code|=0x03000000;
                }
              else
                {
                  /*
                    Add eight bits from Code register to compressed data string.
                  */
                  *q=(unsigned char) (code >> 16);
                  code&=0x0000ffff;
                  code|=0x01800000;
                }
          }
      }
  } while (interval < MinimumIntervalE);
  /*
    Update probability estimates
  */
  if (decision == more_probable[state])
    probability_estimate[state]+=
      increment_more_probable[probability_estimate[state]];
  else
    probability_estimate[state]-=
      decrement_less_probable[probability_estimate[state]];
  if (more_probable_exchange[probability_estimate[state]] != 0)
    more_probable[state]=1-more_probable[state];
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   F l u s h                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function Flush flushes the final bits of data from the Code register to the
%  compressed data string.
%
%  The format of the Flush routine is:
%
%      Flush()
%
%
*/
static void Flush()
{
  register int
    extra_bits;

  code-=interval;
  extra_bits=24;
  extra_bits--;
  while (code >= 0)
  {
    code<<=1;
    extra_bits--;
  }
  code<<=1;
  if (code > 0)
    (*q)--;
  /*
    Add the final compressed data unsigned chars to the compressed data string.
  */
  do
  {
    if ((*q++) == 0xff)
      {
        /*
          Add seven bits of data plus one stuffed bit to the compressed data
          string during final Flush of Code register.
        */
        *q=(unsigned char) (code >> 17);
        code&=0x0001ffff;
        code<<=7;
        extra_bits-=7;
      }
    else
      {
        /*
           Add eight bits of data to the compressed data string during final
           flush of Code register.
        */
        *q=(unsigned char) (code >> 16);
        code&=0x0000ffff;
        code<<=8;
        extra_bits-=8;
      }
  } while (extra_bits > 0);
  q++;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   H u f f m a n D e c o d e I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function HuffmanDecodeImage uncompresses an image via Huffman-coding.
%
%  The format of the HuffmanDecodeImage routine is:
%
%      status=HuffmanDecodeImage(image)
%
%  A description of each parameter follows:
%
%    o status:  Function HuffmanDecodeImage returns True if all the pixels are
%      compressed without error, otherwise False.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int HuffmanDecodeImage(image)
Image
  *image;
{
#define HashSize  1021
#define MBHashA  293
#define MBHashB  2695
#define MWHashA  3510
#define MWHashB  1178

#define InitializeHashTable(hash,table,a,b) \
{  \
  entry=table;  \
  while (entry->code != 0)  \
  {  \
    hash[((entry->length+a)*(entry->code+b)) % HashSize]=entry;  \
    entry++;  \
  }  \
}

#define InputBit(bit)  \
{  \
  if ((mask & 0xff) == 0)  \
    {  \
      byte=getc(image->file);  \
      mask=0x80;  \
    }  \
  runlength++;  \
  bit=byte & mask ? 1 : 0; \
  mask>>=1;  \
  if (bit)  \
    runlength=0;  \
}

  HuffmanTable
    *entry,
    **mb_hash,
    **mw_hash;

  int
    code,
    color,
    count,
    length,
    null_lines,
    runlength,
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    bit,
    byte,
    mask,
    *scanline;

  /*
    Allocate buffers.
  */
  mb_hash=(HuffmanTable **) malloc(HashSize*sizeof(HuffmanTable *));
  mw_hash=(HuffmanTable **) malloc(HashSize*sizeof(HuffmanTable *));
  scanline=(unsigned char *) malloc(image->columns*sizeof(unsigned char));
  if ((mb_hash == (HuffmanTable **) NULL) ||
      (mw_hash == (HuffmanTable **) NULL) ||
      (scanline == (unsigned char *) NULL))
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  /*
    Initialize Huffman tables.
  */
  for (i=0; i < HashSize; i++)
  {
    mb_hash[i]=(HuffmanTable *) NULL;
    mw_hash[i]=(HuffmanTable *) NULL;
  }
  InitializeHashTable(mw_hash,TWTable,MWHashA,MWHashB);
  InitializeHashTable(mw_hash,MWTable,MWHashA,MWHashB);
  InitializeHashTable(mw_hash,EXTable,MWHashA,MWHashB);
  InitializeHashTable(mb_hash,TBTable,MBHashA,MBHashB);
  InitializeHashTable(mb_hash,MBTable,MBHashA,MBHashB);
  InitializeHashTable(mb_hash,EXTable,MBHashA,MBHashB);
  /*
    Uncompress 1D Huffman to runlength encoded pixels.
  */
  mask=0;
  null_lines=0;
  q=image->pixels;
  for (y=0; ((y < image->rows) && (null_lines < 3)); y++)
  {
    /*
      Initialize scanline to white.
    */
    p=scanline;
    for (x=0; x < image->columns; x++)
      *p++=0;
    /*
      Decode Huffman encoded scanline.
    */
    color=True;
    code=0;
    count=0;
    length=0;
    runlength=0;
    for (x=0; x < image->columns; )
    {
      do
      {
        if (runlength < 11)
          InputBit(bit)
        else
          {
            InputBit(bit);
            if (bit)
              length=13;
          }
        code=(code << 1)+bit;
        length++;
      } while ((code <= 0) && (length < 14));
      if (length > 13)
        break;
      if (color)
        {
          if (length < 4)
            continue;
          entry=mw_hash[((length+MWHashA)*(code+MWHashB)) % HashSize];
        }
      else
        {
          if (length < 2)
            continue;
          entry=mb_hash[((length+MBHashA)*(code+MBHashB)) % HashSize];
        }
      if (!entry)
        continue;
      else
        if ((entry->length != length) || (entry->code != code))
          continue;
      switch (entry->id)
      {
        case TWId:
        case TBId:
        {
          count+=entry->count;
          if ((x+count) > image->columns)
            count=image->columns-x;
          if (count > 0)
            if (color)
              {
                x+=count;
                count=0;
              }
            else
              for ( ; count > 0; count--)
                scanline[x++]=1;
          color=!color;
          break;
        }
        case MWId:
        case MBId:
        {
          count+=entry->count;
          break;
        }
        case EXId:
        {
          count+=entry->count;
          break;
        }
        default:
          break;
      }
      code=0;
      length=0;
    }
    null_lines++;
    if (x != 0)
      {
        /*
          Skip to end of scanline.
        */
        while (runlength < 11)
          InputBit(bit);
        do
        {
          InputBit(bit);
        } while (bit == 0);
        null_lines=0;
      }
    /*
      Transfer scanline to image pixels.
    */
    p=scanline;
    for (x=0; x < image->columns; x++)
    {
      q->index=(unsigned short) (*p);
      q->length=0;
      p++;
      q++;
    }
  }
  image->rows=y-1;
  /*
    Free decoder memory.
  */
  (void) free((char *) mw_hash);
  (void) free((char *) mb_hash);
  (void) free((char *) scanline);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   H u f f m a n E n c o d e I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function HuffmanEncodeImage compresses an image via Huffman-coding.
%
%  The format of the HuffmanEncodeImage routine is:
%
%      status=HuffmanEncodeImage(image)
%
%  A description of each parameter follows:
%
%    o status:  Function HuffmanEncodeImage returns True if all the pixels are
%      compressed without error, otherwise False.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int HuffmanEncodeImage(image)
Image
  *image;
{
#define HuffmanOutputCode(entry)  \
{  \
  mask=1 << (entry->length-1);  \
  while (mask != 0)  \
  {  \
    OutputBit((entry->code & mask ? 1 : 0));  \
    mask>>=1;  \
  }  \
}

#define OutputBit(count)  \
{  \
  if(count > 0)  \
    byte=byte | bit;  \
  bit>>=1;  \
  if ((bit & 0xff) == 0)   \
    {  \
      (void) fputc((char) byte,image->file);  \
      byte=0;  \
      bit=0x80;  \
    }  \
}

  HuffmanTable
    *entry;

  int
    i,
    k,
    runlength;

  register int
    j,
    n,
    x;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  register unsigned short
    polarity;

  unsigned char
    bit,
    byte,
    *scanline;

  unsigned int
    mask;

  /*
    Allocate scanline buffer.
  */
  scanline=(unsigned char *)
    malloc((Max(image->columns,1728)+1)*sizeof(unsigned char));
  if (scanline == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  /*
    Compress runlength encoded to 1D Huffman pixels.
  */
  polarity=0;
  if (image->colors == 2)
    polarity=(Intensity(image->colormap[0]) >
      Intensity(image->colormap[1]) ? 0 : 1);
  q=scanline;
  for (i=0; i < Max(image->columns,1728); i++)
    *q++=polarity;
  byte=0;
  bit=0x80;
  p=image->pixels;
  q=scanline;
  x=0;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
    {
      *q++=(unsigned char) (p->index == polarity ? polarity : !polarity);
      x++;
      if (x < image->columns)
        continue;
      /*
        Huffman encode scanline.
      */
      q=scanline;
      for (n=Max(image->columns,1728); n > 0; )
      {
        /*
          Output white run.
        */
        for (runlength=0; ((*q == polarity) && (n > 0)); n--)
        {
          q++;
          runlength++;
        }
        if (runlength >= 64)
          {
            entry=MWTable+((runlength/64)-1);
            runlength-=entry->count;
            HuffmanOutputCode(entry);
          }
        entry=TWTable+runlength;
        HuffmanOutputCode(entry);
        if (n != 0)
          {
            /*
              Output black run.
            */
            for (runlength=0; ((*q != polarity) && (n > 0)); n--)
            {
              q++;
              runlength++;
            }
            if (runlength >= 64)
              {
                entry=MBTable+((runlength/64)-1);
                runlength-=entry->count;
                HuffmanOutputCode(entry);
              }
            entry=TBTable+runlength;
            HuffmanOutputCode(entry);
          }
      }
      /*
        End of line.
      */
      for (k=0; k < 11; k++)
        OutputBit(0);
      OutputBit(1);
      x=0;
      q=scanline;
    }
    p++;
  }
  /*
    End of page.
  */
  for (i=0; i < 6; i++)
  {
    for (k=0; k < 11; k++)
      OutputBit(0);
    OutputBit(1);
  }
  /*
    Flush bits.
  */
  if (bit != 0x80)
    (void) fputc((char) byte,image->file);
  (void) free((char *) scanline);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   L Z W D e c o d e I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function LZWDecodeImage uncompresses an image via LZW-coding.
%
%  The format of the LZWDecodeImage routine is:
%
%      status=LZWDecodeImage(image)
%
%  A description of each parameter follows:
%
%    o status:  Function LZWDecodeImage returns True if all the pixels are
%      uncompressed without error, otherwise False.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int LZWDecodeImage(image)
Image
  *image;
{
#define MaxStackSize  4096
#define NullCode  (-1)

  int
    available,
    clear,
    code_mask,
    code_size,
    end_of_information,
    in_code,
    old_code,
    status;

  register int
    bits,
    code,
    count,
    i;

  register RunlengthPacket
    *p;

  register unsigned char
    *c;

  register unsigned int
    datum;

  short
    *prefix;

  unsigned char
    data_size,
    first,
    *packet,
    *pixel_stack,
    *suffix,
    *top_stack;

  /*
    Allocate decoder tables.
  */
  packet=(unsigned char *) malloc(256*sizeof(unsigned char));
  prefix=(short *) malloc(MaxStackSize*sizeof(short));
  suffix=(unsigned char *) malloc(MaxStackSize*sizeof(unsigned char));
  pixel_stack=(unsigned char *) malloc((MaxStackSize+1)*sizeof(unsigned char));
  if ((packet == (unsigned char *) NULL) ||
      (prefix == (short *) NULL) ||
      (suffix == (unsigned char *) NULL) ||
      (pixel_stack == (unsigned char *) NULL))
    return(False);
  /*
    Initialize LZW data stream decoder.
  */
  data_size=fgetc(image->file);
  clear=1 << data_size;
  end_of_information=clear+1;
  available=clear+2;
  old_code=NullCode;
  code_size=data_size+1;
  code_mask=(1 << code_size)-1;
  for (code=0; code < clear; code++)
  {
    prefix[code]=0;
    suffix[code]=code;
  }
  /*
    Decode LZW pixel stream.
  */
  datum=0;
  bits=0;
  c=0;
  count=0;
  first=0;
  top_stack=pixel_stack;
  p=image->pixels;
  for (i=0; i < image->packets; )
  {
    if (top_stack == pixel_stack)
      {
        if (bits < code_size)
          {
            /*
              Load bytes until there is enough bits for a code.
            */
            if (count == 0)
              {
                /*
                  Read a new data block.
                */
                count=ReadDataBlock((char *) packet,image->file);
                if (count <= 0)
                  break;
                c=packet;
              }
            datum+=(*c) << bits;
            bits+=8;
            c++;
            count--;
            continue;
          }
        /*
          Get the next code.
        */
        code=datum & code_mask;
        datum>>=code_size;
        bits-=code_size;
        /*
          Interpret the code
        */
        if ((code > available) || (code == end_of_information))
          break;
        if (code == clear)
          {
            /*
              Reset decoder.
            */
            code_size=data_size+1;
            code_mask=(1 << code_size)-1;
            available=clear+2;
            old_code=NullCode;
            continue;
          }
        if (old_code == NullCode)
          {
            *top_stack++=suffix[code];
            old_code=code;
            first=code;
            continue;
          }
        in_code=code;
        if (code == available)
          {
            *top_stack++=first;
            code=old_code;
          }
        while (code > clear)
        {
          *top_stack++=suffix[code];
          code=prefix[code];
        }
        first=suffix[code];
        /*
          Add a new string to the string table,
        */
        if (available >= MaxStackSize)
          break;
        *top_stack++=first;
        prefix[available]=old_code;
        suffix[available]=first;
        available++;
        if (((available & code_mask) == 0) && (available < MaxStackSize))
          {
            code_size++;
            code_mask+=available;
          }
        old_code=in_code;
      }
    /*
      Pop a pixel off the pixel stack.
    */
    top_stack--;
    p->index=(unsigned short) *top_stack;
    p->length=0;
    p++;
    i++;
  }
  /*
    Initialize any remaining color packets to a known color.
  */
  status=i == image->packets;
  for ( ; i < image->packets; i++)
  {
    p->index=0;
    p->length=0;
    p++;
  }
  SyncImage(image);
  /*
    Free decoder memory.
  */
  (void) free((char *) pixel_stack);
  (void) free((char *) suffix);
  (void) free((char *) prefix);
  (void) free((char *) packet);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   L Z W E n c o d e F i l t e r                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function LZWEncodeFilter compresses an image via LZW-coding specific to
%  Postscript Level II.
%
%  The format of the LZWEncodeFilter routine is:
%
%      status=LZWEncodeFilter(file,pixels,number_pixels)
%
%  A description of each parameter follows:
%
%    o status:  Function LZWEncodeFilter returns True if all the pixels are
%      compressed without error, otherwise False.
%
%    o file: The address of a structure of type FILE.  LZW encoded pixels
%      are written to this file.
%
%    o pixels: The address of an unsigned array of characters containing the
%      pixels to compress.
%
%    o number_pixels:  An unsigned interger that specifies the number of
%      pixels to compress.
%
%
*/
unsigned int LZWEncodeFilter(file,pixels,number_pixels)
FILE
  *file;

unsigned char
  *pixels;

unsigned int
  number_pixels;
{
#define LZWClr  256  /* Clear Table Marker */
#define LZWEod  257  /* End of Data marker */
#define OutputCode(code) \
{ \
    accumulator+=((long) code) << (32-code_width-number_bits); \
    number_bits+=code_width; \
    while (number_bits >= 8) \
    { \
        PrintByte(accumulator >> 24); \
        accumulator=accumulator << 8; \
        number_bits-=8; \
    } \
}
#define PrintByte(value)  \
{ \
  (void) fprintf(file,"%02x",(int) (value & 0x0ff));  \
  count++;  \
  if (count >= 36)  \
    {  \
      (void) fputc('\n',file);  \
      count=0; \
    } \
}

  typedef struct _TableType
  {
    short
      prefix,
      suffix,
      next;
  } TableType;

  int
    index;

  register int
    i;

  short
    number_bits,
    code_width,
    count,
    last_code,
    next_index;

  TableType
    *table;

  unsigned long
    accumulator;

  /*
    Allocate string table.
  */
  table=(TableType *) malloc((1 << 12)*sizeof(TableType));
  if (table == (TableType *) NULL)
    return(False);
  /*
    Initialize variables.
  */
  accumulator=0;
  code_width=9;
  count=0;
  number_bits=0;
  last_code=0;
  OutputCode(LZWClr);
  for (index=0; index < 256; index++)
  {
    table[index].prefix=(-1);
    table[index].suffix=index;
    table[index].next=(-1);
  }
  next_index=LZWEod+1;
  code_width=9;
  last_code=pixels[0];
  for (i=1; i < number_pixels; i++)
  {
    /*
      Find string.
    */
    index=last_code;
    while (index != -1)
      if ((table[index].prefix != last_code) ||
          (table[index].suffix != pixels[i]))
        index=table[index].next;
      else
        {
          last_code=index;
          break;
        }
    if (last_code != index)
      {
        /*
          Add string.
        */
        OutputCode(last_code);
        table[next_index].prefix=last_code;
        table[next_index].suffix=pixels[i];
        table[next_index].next=table[last_code].next;
        table[last_code].next=next_index;
        next_index++;
        /*
          Did we just move up to next bit width?
        */
        if ((next_index >> code_width) != 0)
          {
            code_width++;
            if (code_width > 12)
              {
                /*
                  Did we overflow the max bit width?
                */
                code_width--;
                OutputCode(LZWClr);
                for (index=0; index < 256; index++)
                {
                  table[index].prefix=(-1);
                  table[index].suffix=index;
                  table[index].next=(-1);
                }
                next_index=LZWEod+1;
                code_width=9;
              }
            }
          last_code=pixels[i];
      }
  }
  /*
    Flush tables.
  */
  OutputCode(last_code);
  OutputCode(LZWEod);
  if (number_bits != 0)
    PrintByte(accumulator >> 24);
  (void) free(table);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   L Z W E n c o d e I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function LZWEncodeImage compresses an image via LZW-coding.
%
%  The format of the LZWEncodeImage routine is:
%
%      status=LZWEncodeImage(image,data_size)
%
%  A description of each parameter follows:
%
%    o status:  Function LZWEncodeImage returns True if all the pixels are
%      compressed without error, otherwise False.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int LZWEncodeImage(image,data_size)
Image
  *image;

unsigned int
  data_size;
{
#define MaxCode(number_bits)  ((1 << (number_bits))-1)
#define MaxHashTable  5003
#define MaxLZWBits  12
#define MaxLZWTable  (1 << MaxLZWBits)
#define LZWOutputCode(code) \
{ \
  /*  \
    Emit a code. \
  */ \
  if (bits > 0) \
    datum|=((long) code << bits); \
  else \
    datum=(long) code; \
  bits+=number_bits; \
  while (bits >= 8)  \
  { \
    /*  \
      Add a character to current packet. \
    */ \
    packet[byte_count++]=(unsigned char) (datum & 0xff); \
    if (byte_count >= 254) \
      { \
        (void) fputc(byte_count,image->file); \
        (void) fwrite((char *) packet,1,byte_count,image->file); \
        byte_count=0; \
      } \
    datum>>=8; \
    bits-=8; \
  } \
  if (free_code > max_code)  \
    { \
      number_bits++; \
      if (number_bits == MaxLZWBits) \
        max_code=MaxLZWTable; \
      else \
        max_code=MaxCode(number_bits); \
    } \
}

  int
    bits,
    byte_count,
    next_pixel,
    number_bits;

  long
    datum;

  register int
    displacement,
    i,
    j;

  register RunlengthPacket
    *p;

  short
    clear_code,
    end_of_information_code,
    free_code,
    *hash_code,
    *hash_prefix,
    index,
    max_code,
    waiting_code;

  unsigned char
    *packet,
    *hash_suffix;

  /*
    Uncompress image.
  */
  if (!UncompressImage(image))
    return(False);
  /*
    Allocate encoder tables.
  */
  packet=(unsigned char *) malloc(256*sizeof(unsigned char));
  hash_code=(short *) malloc(MaxHashTable*sizeof(short));
  hash_prefix=(short *) malloc(MaxHashTable*sizeof(short));
  hash_suffix=(unsigned char *) malloc(MaxHashTable*sizeof(unsigned char));
  if ((packet == (unsigned char *) NULL) || (hash_code == (short *) NULL) ||
      (hash_prefix == (short *) NULL) ||
      (hash_suffix == (unsigned char *) NULL))
    return(False);
  /*
    Initialize LZW encoder.
  */
  number_bits=data_size;
  max_code=MaxCode(number_bits);
  clear_code=((short) 1 << (data_size-1));
  end_of_information_code=clear_code+1;
  free_code=clear_code+2;
  byte_count=0;
  datum=0;
  bits=0;
  for (i=0; i < MaxHashTable; i++)
    hash_code[i]=0;
  LZWOutputCode(clear_code);
  /*
    Encode pixels.
  */
  p=image->pixels;
  waiting_code=p->index;
  for (i=1; i < (image->columns*image->rows); i++)
  {
    /*
      Probe hash table.
    */
    p++;
    index=p->index & 0xff;
    j=(int) ((int) index << (MaxLZWBits-8))+waiting_code;
    if (j >= MaxHashTable)
      j-=MaxHashTable;
    if (hash_code[j] > 0)
      {
        if ((hash_prefix[j] == waiting_code) && (hash_suffix[j] == index))
          {
            waiting_code=hash_code[j];
            continue;
          }
        if (j == 0)
          displacement=1;
        else
          displacement=MaxHashTable-j;
        next_pixel=False;
        for ( ; ; )
        {
          j-=displacement;
          if (j < 0)
            j+=MaxHashTable;
          if (hash_code[j] == 0)
            break;
          if ((hash_prefix[j] == waiting_code) && (hash_suffix[j] == index))
            {
              waiting_code=hash_code[j];
              next_pixel=True;
              break;
            }
        }
        if (next_pixel == True)
          continue;
      }
    LZWOutputCode(waiting_code);
    if (free_code < MaxLZWTable)
      {
        hash_code[j]=free_code++;
        hash_prefix[j]=waiting_code;
        hash_suffix[j]=index;
      }
    else
      {
        /*
          Fill the hash table with empty entries.
        */
        for (j=0; j < MaxHashTable; j++)
          hash_code[j]=0;
        /*
          Reset compressor and issue a clear code.
        */
        free_code=clear_code+2;
        LZWOutputCode(clear_code);
        number_bits=data_size;
        max_code=MaxCode(number_bits);
      }
    waiting_code=index;
  }
  /*
    Flush out the buffered code.
  */
  LZWOutputCode(waiting_code);
  LZWOutputCode(end_of_information_code);
  if (bits > 0)
    {
      /*
        Add a character to current packet.
      */
      packet[byte_count++]=(unsigned char) (datum & 0xff);
      if (byte_count >= 254)
        {
          (void) fputc(byte_count,image->file);
          (void) fwrite((char *) packet,1,byte_count,image->file);
          byte_count=0;
        }
    }
  /*
    Flush accumulated data.
  */
  if (byte_count > 0)
    {
      (void) fputc(byte_count,image->file);
      (void) fwrite((char *) packet,1,byte_count,image->file);
    }
  /*
    Free encoder memory.
  */
  (void) free((char *) hash_suffix);
  (void) free((char *) hash_prefix);
  (void) free((char *) hash_code);
  (void) free((char *) packet);
  if (i < image->packets)
    return(False);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   P a c k b i t s E n c o d e I m a g e                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function PackbitsEncodeImage compresses an image via Macintosh pack bits
%  encoding.
%
%  The format of the PackbitsEncodeImage routine is:
%
%      status=PackbitsEncodeImage(image,scanline,packbits)
%
%  A description of each parameter follows:
%
%    o status:  Function PackbitsEncodeImage returns True if all the pixels are
%      compressed without error, otherwise False.
%
%    o image: The address of a structure of type Image.
%
%    o scanline: A pointer to an array of characters to pack.
%
%    o packbits: A pointer to an array of characters where the packed
%      characters are stored.
%
%
*/
unsigned int PackbitsEncodeImage(image,scanline,packbits)
Image
  *image;

unsigned char
  *scanline,
  *packbits;
{
#define MaxCount  128
#define MaxPackBitsRunlength  128

  int
    count,
    number_packets,
    repeat_count,
    runlength;

  register int
    i;

  register unsigned char
    *p,
    *q;

  unsigned char
    index;

  unsigned int
    bytes_per_line;

  /*
    Pack scanline.
  */
  bytes_per_line=image->columns;
  if (image->class == DirectClass)
    bytes_per_line*=3;
  count=0;
  runlength=0;
  p=scanline+(bytes_per_line-1);
  q=packbits;
  index=(*p);
  for (i=bytes_per_line-1; i >= 0; i--)
  {
    if (index == *p)
      runlength++;
    else
      {
        if (runlength < 3)
          while (runlength > 0)
          {
            *q++=index;
            runlength--;
            count++;
            if (count == MaxCount)
              {
                *q++=MaxCount-1;
                count-=MaxCount;
              }
          }
        else
          {
            if (count > 0)
              *q++=count-1;
            count=0;
            while (runlength > 0)
            {
              repeat_count=runlength;
              if (repeat_count > MaxPackBitsRunlength)
                repeat_count=MaxPackBitsRunlength;
              *q++=index;
              *q++=257-repeat_count;
              runlength-=repeat_count;
            }
          }
        runlength=1;
      }
    index=(*p);
    p--;
  }
  if (runlength < 3)
    while (runlength > 0)
    {
      *q++=index;
      runlength--;
      count++;
      if (count == MaxCount)
        {
          *q++=MaxCount-1;
          count-=MaxCount;
        }
    }
  else
    {
      if (count > 0)
        *q++=count-1;
      count=0;
      while (runlength > 0)
      {
        repeat_count=runlength;
        if (repeat_count > MaxPackBitsRunlength)
          repeat_count=MaxPackBitsRunlength;
        *q++=index;
        *q++=257-repeat_count;
        runlength-=repeat_count;
      }
    }
  if (count > 0)
    *q++=count-1;
  /*
    Write the number of and the packed packets.
  */
  number_packets=q-packbits;
  if ((bytes_per_line-1) > 250)
    {
      MSBFirstWriteShort((unsigned short) number_packets,image->file);
      number_packets+=2;
    }
  else
    {
      index=number_packets;
      (void) fputc((char) index,image->file);
      number_packets++;
    }
  while (q != packbits)
  {
    q--;
    (void) fputc((char) *q,image->file);
  }
  return(number_packets);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   Q D e c o d e I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function QDecodeImage uncompresses an image via Q-coding.
%
%  The format of the QDecodeImage routine is:
%
%      count=QDecodeImage(compressed_pixels,pixels,number_columns,number_rows)
%
%  A description of each parameter follows:
%
%    o count:  The QDecodeImage routine returns this integer value.  It is
%      the actual number of pixels created by the decompression process.
%
%    o compressed_pixels:  The address of a byte (8 bits) array of compressed
%      pixel data.
%
%    o pixels:  The address of a byte (8 bits) array of pixel data created by
%      the uncompression process.  The number of bytes in this array
%      must be at least equal to the number columns times the number of rows
%      of the source pixels.
%
%    o number_columns:  An integer value that is the number of columns or
%      width in pixels of your source image.
%
%    o number_rows:  An integer value that is the number of rows or
%      heigth in pixels of your source image.
%
%
*/
unsigned int QDecodeImage(compressed_pixels,pixels,number_columns,number_rows)
unsigned char
  *compressed_pixels,
  *pixels;

unsigned int
  number_columns,
  number_rows;
{
  int
    decision,
    i,
    prediction,
    row;

  register int
    column,
    magnitude,
    sign,
    state,
    value;

  register ScanlinePacket
    *cs,
    *ls;

  register unsigned char
    *p;

  ScanlinePacket
    *scanline;

  for (i=0; i < MaxContextStates; i++)
  {
    probability_estimate[i]=0;
    more_probable[i]=0;
    less_probable[i]=1;
  }
  /*
    Allocate scanline for row values and states
  */
  scanline=(ScanlinePacket *)
    malloc((((number_columns+1) << 1)*sizeof(ScanlinePacket)));
  if (scanline == (ScanlinePacket *) NULL)
    {
      Warning("Unable to compress image, Unable to allocate memory",
        (char *) NULL);
      exit(1);
    }
  cs=scanline;
  for (i=0; i < ((number_columns+1) << 1); i++)
  {
    cs->pixel=0;
    cs->state=ZeroState;
    cs++;
  }
  interval=MinimumIntervalD;
  p=pixels;
  q=compressed_pixels+1;
  /*
    Add a new unsigned char of compressed data to the Code register.
  */
  code=(int) (*q) << 16;
  if ((*q++) == 0xff)
    code+=((int) (*q) << 9)+0x02;
  else
    code+=((*q) << 8)+0x01;
  code<<=4;
  code+=(interval << 16);
  /*
    Decode each image scanline.
  */
  for (row=0; row < number_rows; row++)
  {
    ls=scanline+(number_columns+1)*((row+0) % 2);
    cs=scanline+(number_columns+1)*((row+1) % 2);
    for (column=0; column < number_columns; column++)
    {
      prediction=(int) cs->pixel-(int) ls->pixel;
      ls++;
      prediction+=(int) ls->pixel;
      state=statistics[cs->state][ls->state];
      cs++;
      cs->state=ZeroState;
      /*
        Branch for zero code value
      */
      Decode(state,&decision);
      if (decision == No)
        value=0;
      else
        {
          /*
            Decode sign information
          */
          state++;
          Decode(state,&decision);
          if (decision == Yes)
            sign=(-1);
          else
            {
              sign=1;
              state++;
            }
          state++;
          /*
            Branch for value=+-1
          */
          Decode(state,&decision);
          if (decision == No)
            value=1;
          else
            {
              /*
                Establish magnitude of value.
              */
              magnitude=2;
              state=100;
              Decode(state,&decision);
              while (decision != No)
              {
                if (state < 107)
                  state++;
                magnitude<<=1;
                Decode(state,&decision);
              }
              /*
                Code remaining bits.
              */
              state+=7;
              value=1;
              magnitude>>=2;
              if (magnitude != 0)
                {
                  Decode(state,&decision);
                  state+=6;
                  value=(value << 1) | decision;
                  magnitude>>=1;
                  while (magnitude != 0)
                  {
                    Decode(state,&decision);
                    value=(value << 1) | decision;
                    magnitude>>=1;
                  }
                }
              value++;
            }
          if (value > LowerBound)
            if (value <= UpperBound)
              cs->state=
                (sign < ZeroState ? SmallPostitiveState : SmallNegativeState);
            else
              cs->state=
                (sign < ZeroState ? LargePostitiveState : LargeNegativeState);
          if (sign < 0)
            value=(-value);
        }
      cs->pixel=(unsigned char) (value+prediction);
      *p++=cs->pixel;
    }
  }
  (void) free((char *) scanline);
  return((unsigned int) (p-pixels));
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   Q E n c o d e I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function QEncodeImage compresses an image via q-coding.  QEncodeImage uses
%  a simple predictive method.  The predictor combines three neighboring
%  samples (A, B, and C) to form a prediction of the sample X:
%
%    C B
%    A X
%
%  The prediction formula is A + B - C.  The prediction is subtracted from
%  from the actual sample X and the difference is encoded by an arithmetic
%  entropy coding method.
%
%  The format of the QEncodeImage routine is:
%
%      count=QEncodeImage(pixels,compressed_pixels,number_columns,number_rows)
%
%  A description of each parameter follows:
%
%    o count:  The QEncodeImage routine returns this integer value.  It is
%      the actual number of compressed pixels created by the compression
%      process.
%
%    o pixels:  The address of a byte (8 bits) array of pixel data.
%
%    o compressed_pixels:  The address of a byte (8 bits) array of pixel data
%      created by the compression process.  The number of bytes in this array
%      must be at least equal to the number columns times the number of rows
%      of the source pixels to allow for the possibility that no compression
%      is possible.  The actual number of bytes used is reflected by the
%      count parameter.
%
%    o number_columns:  An integer value that is the number of columns or
%      width in pixels of your source image.
%
%    o number_rows:  An integer value that is the number of rows or
%      heigth in pixels of your source image.
%
%
%
*/
unsigned int QEncodeImage(pixels,compressed_pixels,number_columns,number_rows)
unsigned char
  *pixels,
  *compressed_pixels;

unsigned int
  number_columns,
  number_rows;
{
  int
    i,
    prediction,
    row;

  register int
    column,
    magnitude,
    sign,
    state,
    value;

  register ScanlinePacket
    *cs,
    *ls;

  register unsigned char
    *p;

  ScanlinePacket
    *scanline;

  void
    Flush();

  for (i=0; i < MaxContextStates; i++)
  {
    probability_estimate[i]=0;
    more_probable[i]=0;
  }
  /*
    Allocate scanline for row values and states.
  */
  scanline=(ScanlinePacket *)
    malloc(((number_columns+1) << 1)*sizeof(ScanlinePacket));
  if (scanline == (ScanlinePacket *) NULL)
    {
      Warning("Unable to compress image, Unable to allocate memory",
        (char *) NULL);
      exit(1);
    }
  cs=scanline;
  for (i=0; i < ((number_columns+1) << 1); i++)
  {
    cs->pixel=0;
    cs->state=ZeroState;
    cs++;
  }
  interval=MinimumIntervalE;
  p=pixels;
  q=compressed_pixels;
  (*q)++;
  code=0x00180000;
  /*
    Encode each scanline.
  */
  for (row=0; row < number_rows; row++)
  {
    ls=scanline+(number_columns+1)*((row+0) % 2);
    cs=scanline+(number_columns+1)*((row+1) % 2);
    for (column=0; column < number_columns; column++)
    {
      prediction=(int) cs->pixel-(int) ls->pixel;
      ls++;
      prediction+=(int) ls->pixel;
      state=statistics[cs->state][ls->state];
      cs++;
      cs->pixel=(*p++);
      cs->state=ZeroState;
      value=(int) cs->pixel-prediction;
      Encode(state,(int) (value == 0 ? No : Yes));
      if (value != 0)
        {
          /*
            Code sign information
          */
          state++;
          sign=(value < 0 ? -1 : 1);
          Encode(state,(int) (sign >= 0 ? No : Yes));
          if (sign < 0)
            value=(-value);
          else
            state++;
          state++;
          value--;
          /*
            Branch for code=+-1
          */
          Encode(state,(int) (value == 0 ? No : Yes));
          if (value != 0)
            {
              /*
                Establish magnitude of value.
              */
              state=100;
              magnitude=2;
              while (value >= magnitude)
              {
                Encode(state,Yes);
                if (state < 107)
                  state++;
                magnitude<<=1;
              }
              Encode(state,No);
              /*
                Code remaining bits
              */
              state+=7;
              magnitude>>=2;
              if (magnitude != 0)
                {
                  Encode(state,(int) ((magnitude & value) == 0 ? No : Yes));
                  state+=6;
                  magnitude>>=1;
                  while (magnitude != 0)
                  {
                    Encode(state,(int) ((magnitude & value) == 0 ? No : Yes));
                    magnitude>>=1;
                  }
                }
            }
          if (value >= LowerBound)
            if (value < UpperBound)
              cs->state=
                (sign < ZeroState ? SmallPostitiveState : SmallNegativeState);
            else
              cs->state=
                (sign < ZeroState ? LargePostitiveState : LargeNegativeState);
        }
    }
  }
  Flush();
  (void) free((char *) scanline);
  return((unsigned int) (q-compressed_pixels));
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R u n l e n g t h D e c o d e I m a g e                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function RunlengthDecodeImage unpacks the packed image pixels into
%  runlength-encoded pixel packets.  The packed image pixel memory is then
%  freed.
%
%  The format of the RunlengthDecodeImage routine is:
%
%      status=RunlengthDecodeImage(image)
%
%  A description of each parameter follows:
%
%    o status: Function RunlengthDecodeImage return True if the image is
%      decoded.  False is returned if there is an error occurs.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int RunlengthDecodeImage(image)
Image
  *image;
{
  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned long
    count;

  if (image->packed_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to unpack pixels","no packed image pixels");
      return(False);
    }
  /*
    Allocate pixels.
  */
  if (image->pixels != (RunlengthPacket *) NULL)
    (void) free((char *) image->pixels);
  image->pixels=(RunlengthPacket *)
    malloc((unsigned int) image->packets*sizeof(RunlengthPacket));
  if (image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Unable to unpack pixels","Memory allocation failed");
      return(False);
    }
  /*
    Unpack the packed image pixels into runlength-encoded pixel packets.
  */
  p=image->packed_pixels;
  q=image->pixels;
  count=0;
  if (image->class == DirectClass)
    {
      register int
        matte;

      matte=image->matte;
      if (image->compression == RunlengthEncodedCompression)
        for (i=0; i < image->packets; i++)
        {
          q->red=(*p++);
          q->green=(*p++);
          q->blue=(*p++);
          q->index=(unsigned short) (matte ? (*p++) : 0);
          q->length=(*p++);
          count+=(q->length+1);
          q++;
        }
      else
        for (i=0; i < image->packets; i++)
        {
          q->red=(*p++);
          q->green=(*p++);
          q->blue=(*p++);
          q->index=(unsigned short) (matte ? (*p++) : 0);
          q->length=0;
          count++;
          q++;
        }
    }
  else
    {
      register unsigned short
        index;

      if (image->compression == RunlengthEncodedCompression)
        {
          if (image->colors <= 256)
            for (i=0; i < image->packets; i++)
            {
              q->index=(unsigned short) (*p++);
              q->length=(*p++);
              count+=(q->length+1);
              q++;
            }
          else
            for (i=0; i < image->packets; i++)
            {
              index=(*p++) << 8;
              index|=(*p++);
              q->index=index;
              q->length=(*p++);
              count+=(q->length+1);
              q++;
            }
        }
      else
        if (image->colors <= 256)
          for (i=0; i < image->packets; i++)
          {
            q->index=(unsigned short) (*p++);
            q->length=0;
            count++;
            q++;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            index=(*p++) << 8;
            index|=(*p++);
            q->index=index;
            q->length=0;
            count++;
            q++;
          }
      SyncImage(image);
    }
  /*
    Free packed pixels memory.
  */
  (void) free((char *) image->packed_pixels);
  image->packed_pixels=(unsigned char *) NULL;
  /*
    Guarentee the correct number of pixel packets.
  */
  if (count > (image->columns*image->rows))
    {
      Warning("insufficient image data in file",image->filename);
      return(False);
    }
  else
    if (count < (image->columns*image->rows))
      {
        Warning("too much image data in file",image->filename);
        return(False);
      }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R u n l e n g t h E n c o d e I m a g e                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function RunlengthEncodeImage packs the runlength-encoded pixel packets
%  into the minimum number of bytes.
%
%  The format of the RunlengthEncodeImage routine is:
%
%      status=RunlengthEncodeImage(image)
%
%  A description of each parameter follows:
%
%    o status: Function RunlengthEncodeImage return True if the image is
%      packed.  False is returned if an error occurs.
%
%    o image: The address of a structure of type Image.
%
%
*/
unsigned int RunlengthEncodeImage(image)
Image
  *image;
{
  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned long
    count,
    packets;

  if (image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Unable to pack pixels","no image pixels");
      return(False);
    }
  /*
    Runlength-encode only if it consumes less memory than no compression.
  */
  if (image->compression == RunlengthEncodedCompression)
    CompressImage(image);
  /*
    Determine packed packet size.
  */
  if (image->class == PseudoClass)
    {
      image->packet_size=1;
      if (image->colors > 256)
        image->packet_size++;
    }
  else
    {
      image->packet_size=3;
      if (image->matte)
        image->packet_size++;
    }
  if (image->compression == RunlengthEncodedCompression)
    image->packet_size++;
  /*
    Allocate packed pixel memory.
  */
  if (image->packed_pixels != (unsigned char *) NULL)
    (void) free((char *) image->packed_pixels);
  packets=image->packets;
  if (image->compression != RunlengthEncodedCompression)
    packets=image->columns*image->rows;
  image->packed_pixels=(unsigned char *)
    malloc((unsigned int) packets*image->packet_size*sizeof(unsigned char));
  if (image->packed_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to pack pixels","Memory allocation failed");
      return(False);
    }
  /*
    Packs the runlength-encoded pixel packets into the minimum number of bytes.
  */
  p=image->pixels;
  q=image->packed_pixels;
  count=0;
  if (image->class == DirectClass)
    {
      register int
        matte;

      matte=image->matte;
      if (image->compression == RunlengthEncodedCompression)
        for (i=0; i < image->packets; i++)
        {
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
          if (matte)
            *q++=(unsigned char) p->index;
          *q++=p->length;
          count+=(p->length+1);
          p++;
        }
      else
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= ((int) p->length); j++)
          {
            *q++=p->red;
            *q++=p->green;
            *q++=p->blue;
            if (matte)
              *q++=(unsigned char) p->index;
          }
          count+=(p->length+1);
          p++;
        }
    }
  else
    if (image->compression == RunlengthEncodedCompression)
      {
        if (image->colors <= 256)
          for (i=0; i < image->packets; i++)
          {
            *q++=(unsigned char) p->index;
            *q++=p->length;
            count+=(p->length+1);
            p++;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            *q++=(unsigned char) (p->index >> 8);
            *q++=(unsigned char) p->index;
            *q++=p->length;
            count+=(p->length+1);
            p++;
          }
      }
    else
      if (image->colors <= 256)
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= ((int) p->length); j++)
            *q++=(unsigned char) p->index;
          count+=(p->length+1);
          p++;
        }
      else
        {
          register unsigned char
            xff00,
            xff;

          for (i=0; i < image->packets; i++)
          {
            xff00=(unsigned char) (p->index >> 8);
            xff=(unsigned char) p->index;
            for (j=0; j <= ((int) p->length); j++)
            {
              *q++=xff00;
              *q++=xff;
            }
            count+=(p->length+1);
            p++;
          }
        }
  image->packets=packets;
  /*
    Guarentee the correct number of pixel packets.
  */
  if (count < (image->columns*image->rows))
    {
      Warning("insufficient image data in",image->filename);
      return(False);
    }
  else
    if (count > (image->columns*image->rows))
      {
        Warning("too much image data in",image->filename);
        return(False);
      }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   S U N D e c o d e I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function SUNDecodeImage unpacks the packed image pixels into
%  runlength-encoded pixel packets.
%
%  The format of the SUNDecodeImage routine is:
%
%      status=SUNDecodeImage(compressed_pixels,pixels,number_columns,
%        number_rows)
%
%  A description of each parameter follows:
%
%    o status:  Function SUNDecodeImage returns True if all the pixels are
%      uncompressed without error, otherwise False.
%
%    o compressed_pixels:  The address of a byte (8 bits) array of compressed
%      pixel data.
%
%    o pixels:  The address of a byte (8 bits) array of pixel data created by
%      the uncompression process.  The number of bytes in this array
%      must be at least equal to the number columns times the number of rows
%      of the source pixels.
%
%    o number_columns:  An integer value that is the number of columns or
%      width in pixels of your source image.
%
%    o number_rows:  An integer value that is the number of rows or
%      heigth in pixels of your source image.
%
%
*/
unsigned int SUNDecodeImage(compressed_pixels,pixels,number_columns,number_rows)
unsigned char
  *compressed_pixels,
  *pixels;

unsigned int
  number_columns,
  number_rows;
{
  register int
    count;

  register unsigned char
    *p,
    *q;

  unsigned char
    byte;

  p=compressed_pixels;
  q=pixels;
  while ((q-pixels) <= (number_columns*number_rows))
  {
    byte=(*p++);
    if (byte != 128)
      *q++=byte;
    else
      {
        /*
          Runlength-encoded packet: <count><byte>
        */
        count=(*p++);
        if (count > 0)
          byte=(*p++);
        while (count >= 0)
        {
          *q++=byte;
          count--;
        }
     }
  }
  return(True);
}
