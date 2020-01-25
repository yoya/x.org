/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                   EEEEE  N   N   CCCC   OOO   DDDD   EEEEE                  %
%                   E      NN  N  C      O   O  D   D  E                      %
%                   EEE    N N N  C      O   O  D   D  EEE                    %
%                   E      N  NN  C      O   O  D   D  E                      %
%                   EEEEE  N   N   CCCC   OOO   DDDD   EEEEE                  %
%                                                                             %
%                                                                             %
%                    Utility Routines to Write Image Formats                  %
%                                                                             %
%                                                                             %
%                                                                             %
%                             Software Design                                 %
%                               John Cristy                                   %
%                              January 1992                                   %
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
%  in an action of contract, negligence or other tortuous action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Functions in this library convert to and from `alien' image formats to the
%  MIFF image format.
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "image.h"
#include "X.h"
#include "compress.h"
#include "utility.h"
#include "XWDFile.h"

/*
  Forward declarations.
*/
static unsigned int
  WriteMIFFImage _Declare((Image *));

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e A V S I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteAVSImage writes an image to a file in AVS X image format.
%
%  The format of the WriteAVSImage routine is:
%
%      status=WriteAVSImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteAVSImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteAVSImage(image)
Image
  *image;
{
  typedef struct _AVSHeader
  {
    int
      width,
      height;
  } AVSHeader;

  AVSHeader
    avs_header;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *avs_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Initialize raster file header.
  */
  avs_header.width=image->columns;
  avs_header.height=image->rows;
  avs_pixels=(unsigned char *)
    malloc(4*image->columns*image->rows*sizeof(unsigned char));
  if (avs_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  (void) fwrite((char *) &avs_header,sizeof(AVSHeader),1,image->file);
  p=image->pixels;
  q=avs_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= (int) p->length; j++)
    {
      *q++=(unsigned char) (image->matte ? p->index : 0);
      *q++=p->red;
      *q++=p->green;
      *q++=p->blue;
    }
    p++;
  }
  (void) fwrite((char *) avs_pixels,4,(int)(image->columns*image->rows),
    image->file);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e B M P I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteBMPImage writes an image in Microsoft Windows bitmap encoded
%  image format.
%
%  The format of the WriteBMPImage routine is:
%
%      status=WriteBMPImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteBMPImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteBMPImage(image)
Image
  *image;
{
  typedef struct _BMPHeader
  {
    unsigned long
      file_size;

    unsigned short
      reserved[2];

    unsigned long
      offset_bits,
      size,
      width,
      height;

    unsigned short
      planes,
      bit_count;

    unsigned long
      compression,
      image_size,
      x_pixels,
      y_pixels,
      number_colors,
      colors_important;
  } BMPHeader;

  BMPHeader
    bmp_header;

  register int
    i,
    j,
    x,
    y;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *bmp_data,
    *bmp_pixels;

  unsigned int
    bytes_per_line;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize BMP raster file header.
  */
  bmp_header.file_size=14+40;
  bmp_header.offset_bits=14+40;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color BMP raster.
      */
      bmp_header.bit_count=24;
      bmp_header.number_colors=0;
    }
  else
    {
      /*
        Colormapped BMP raster.
      */
      bmp_header.bit_count=8;
      if (IsGrayImage(image) && (image->colors == 2))
        bmp_header.bit_count=1;
      bmp_header.file_size+=4*(1 << bmp_header.bit_count);
      bmp_header.offset_bits+=4*(1 << bmp_header.bit_count);
      bmp_header.number_colors=1 << bmp_header.bit_count;
    }
  bmp_header.reserved[0]=0;
  bmp_header.reserved[1]=0;
  bmp_header.size=40;
  bmp_header.width=image->columns;
  bmp_header.height=image->rows;
  bmp_header.planes=1;
  bmp_header.compression=0;
  bytes_per_line=((image->columns*bmp_header.bit_count+31)/32)*4;
  bmp_header.image_size=bytes_per_line*image->rows;
  bmp_header.file_size+=bmp_header.image_size;
  bmp_header.x_pixels=75*39;
  bmp_header.y_pixels=75*39;
  bmp_header.colors_important=bmp_header.number_colors;
  /*
    Convert MIFF to BMP raster pixels.
  */
  bmp_pixels=(unsigned char *)
    malloc(bmp_header.image_size*sizeof(unsigned char));
  if (bmp_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  x=0;
  y=image->rows-1;
  switch (bmp_header.bit_count)
  {
    case 1:
    {
      register unsigned char
        bit,
        byte,
        polarity;

      /*
        Convert PseudoClass image to a BMP monochrome image.
      */
      polarity=0;
      if (image->colors == 2)
        polarity=Intensity(image->colormap[1]) > Intensity(image->colormap[0]);
      bit=0;
      byte=0;
      q=bmp_pixels+y*bytes_per_line;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          byte<<=1;
          if (p->index == polarity)
            byte|=0x01;
          bit++;
          if (bit == 8)
            {
              *q++=byte;
              bit=0;
              byte=0;
            }
          x++;
          if (x == image->columns)
            {
              /*
                Advance to the next scanline.
              */
              if (bit != 0)
                *q++=byte << (8-bit);
              bit=0;
              byte=0;
              x=0;
              y--;
              q=bmp_pixels+y*bytes_per_line;
           }
        }
        p++;
      }
      break;
    }
    case 8:
    {
      /*
        Convert PseudoClass packet to BMP pixel.
      */
      q=bmp_pixels+y*bytes_per_line;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          *q++=p->index;
          x++;
          if (x == image->columns)
            {
              x=0;
              y--;
              q=bmp_pixels+y*bytes_per_line;
            }
        }
        p++;
      }
      break;
    }
    case 24:
    {
      /*
        Convert DirectClass packet to BMP RGB pixel.
      */
      q=bmp_pixels+y*bytes_per_line;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          *q++=p->blue;
          *q++=p->green;
          *q++=p->red;
          x++;
          if (x == image->columns)
            {
              x=0;
              y--;
              q=bmp_pixels+y*bytes_per_line;
            }
        }
        p++;
      }
      break;
    }
  }
  if (bmp_header.bit_count == 8)
    {
      if (image->compression == RunlengthEncodedCompression)
        CompressImage(image);
      if (image->compression == RunlengthEncodedCompression)
        {
          unsigned int
            packets;

          /*
            Convert run-length encoded raster pixels.
          */
          packets=(bytes_per_line*(bmp_header.height+2)+1) << 1;
          bmp_data=(unsigned char *) malloc(packets*sizeof(unsigned char));
          if (bmp_pixels == (unsigned char *) NULL)
            {
              Warning("Memory allocation error",(char *) NULL);
              (void) free((char *) bmp_pixels);
              return(False);
            }
          bmp_header.image_size=
            BMPEncodeImage(bmp_pixels,bmp_data,image->columns,image->rows);
          (void) free((char *) bmp_pixels);
          bmp_pixels=bmp_data;
          bmp_header.compression=1;
        }
    }
  /*
    Write BMP header.
  */
  (void) fwrite("BM",1,2,image->file);
  LSBFirstWriteLong(bmp_header.file_size,image->file);
  LSBFirstWriteShort(bmp_header.reserved[0],image->file);
  LSBFirstWriteShort(bmp_header.reserved[1],image->file);
  LSBFirstWriteLong(bmp_header.offset_bits,image->file);
  LSBFirstWriteLong(bmp_header.size,image->file);
  LSBFirstWriteLong(bmp_header.width,image->file);
  LSBFirstWriteLong(bmp_header.height,image->file);
  LSBFirstWriteShort(bmp_header.planes,image->file);
  LSBFirstWriteShort(bmp_header.bit_count,image->file);
  LSBFirstWriteLong(bmp_header.compression,image->file);
  LSBFirstWriteLong(bmp_header.image_size,image->file);
  LSBFirstWriteLong(bmp_header.x_pixels,image->file);
  LSBFirstWriteLong(bmp_header.y_pixels,image->file);
  LSBFirstWriteLong(bmp_header.number_colors,image->file);
  LSBFirstWriteLong(bmp_header.colors_important,image->file);
  if (image->class == PseudoClass)
    {
      unsigned char
        *bmp_colormap;

      /*
        Dump colormap to file.
      */
      bmp_colormap=(unsigned char *)
        malloc(4*(1 << bmp_header.bit_count)*sizeof(unsigned char));
      if (bmp_colormap == (unsigned char *) NULL)
        {
          Warning("Unable to allocate memory",(char *) NULL);
          return(False);
        }
      q=bmp_colormap;
      for (i=0; i < image->colors; i++)
      {
        *q++=image->colormap[i].blue;
        *q++=image->colormap[i].green;
        *q++=image->colormap[i].red;
        q++;
      }
      (void) fwrite((char *) bmp_colormap,4,1 << bmp_header.bit_count,
        image->file);
      (void) free((char *) bmp_colormap);
    }
  (void) fwrite((char *) bmp_pixels,1,(int) bmp_header.image_size,image->file);
  (void) free((char *) bmp_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e C M Y K I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteCMYKImage writes an image to a file in cyan, magenta,
%  yellow, and black rasterfile format.
%
%  The format of the WriteCMYKImage routine is:
%
%      status=WriteCMYKImage(image,interlace,undercolor_geometry)
%
%  A description of each parameter follows.
%
%    o status: Function WriteCMYKImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%    o interlace:  An unsigned integer that specifies the interlacing
%      scheme.
%
%    o undercolor_geometry: Specifies a pointer to a undercolor geometry
%      string.  The specified undercolor removal and black generation of this
%      geometry string are relative.
%
%
*/
static unsigned int WriteCMYKImage(image,interlace,undercolor_geometry)
Image
  *image;

unsigned int
  interlace;

char
  *undercolor_geometry;
{
  float
    black_generation,
    undercolor;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    black,
    cyan,
    magenta,
    *cmyk_pixels,
    yellow;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert MIFF to CMYK raster pixels.
  */
  cmyk_pixels=(unsigned char *)
    malloc(4*image->columns*image->rows*sizeof(unsigned char));
  if (cmyk_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  undercolor=1.0;
  black_generation=1.0;
  if (undercolor_geometry != (char *) NULL)
    {
      (void) sscanf(undercolor_geometry,"%fx%f",&undercolor,&black_generation);
      if (black_generation == 1.0)
        black_generation=undercolor;
    }
  q=cmyk_pixels;
  switch (interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  CMYKCMYKCMYKCMYKCMYKCMYK...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=cyan-undercolor*black;
          *q++=magenta-undercolor*black;
          *q++=yellow-undercolor*black;
          *q++=black_generation*black;
        }
        p++;
      }
      break;
    }
    case LineInterlace:
    {
      register int
        x,
        y;

      /*
        Line interlacing:  CCC...MMM...YYY...CCC...MMM...YYY...
      */
      if (!UncompressImage(image))
        return(False);
      for (y=0; y < image->rows; y++)
      {
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=cyan-undercolor*black;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=magenta-undercolor*black;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=yellow-undercolor*black;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=black_generation*black;
          p++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  CCCCCC...MMMMMM...YYYYYY...KKKKKK...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=cyan-undercolor*black;
        }
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=magenta-undercolor*black;
        }
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
           cyan=MaxRGB-p->red;
           magenta=MaxRGB-p->green;
           yellow=MaxRGB-p->blue;
           black=cyan;
           if (magenta < black)
             black=magenta;
           if (yellow < black)
             black=yellow;
           *q++=yellow-undercolor*black;
        }
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          cyan=MaxRGB-p->red;
          magenta=MaxRGB-p->green;
          yellow=MaxRGB-p->blue;
          black=cyan;
          if (magenta < black)
            black=magenta;
          if (yellow < black)
            black=yellow;
          *q++=black_generation*black;
        }
        p++;
      }
      break;
    }
  }
  (void) fwrite((char *) cmyk_pixels,4,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) cmyk_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e F A X I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure WriteFAXImage writes an image to a file in 1 dimensional Huffman
%  encoded format.
%
%  The format of the WriteFAXImage routine is:
%
%      status=WriteFAXImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteFAXImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteFAXImage(image)
Image
  *image;
{
  unsigned int
    status;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert MIFF to monochrome.
  */
  QuantizeImage(image,2,8,False,GRAYColorspace,True);
  SyncImage(image);
  status=HuffmanEncodeImage(image);
  CloseImage(image);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e F I T S I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteFITSImage writes a Flexible Image Transport System image to a
%  file as gray scale intensities [0..255].
%
%  The format of the WriteFITSImage routine is:
%
%      status=WriteFITSImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteFITSImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteFITSImage(image)
Image
  *image;
{
  char
    buffer[81],
    *fits_header;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *fits_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Allocate image header.
  */
  fits_header=(char *) malloc(2880*sizeof(unsigned char));
  if (fits_header == (char *) NULL)
    {
      Warning("Unable to write image","Memory allocation failed");
      return(False);
    }
  /*
    Initialize image header.
  */
  for (i=0; i < 2880; i++)
    fits_header[i]=' ';
  (void) strcpy(buffer,"SIMPLE  =                    T");
  (void) strncpy(fits_header+0,buffer,strlen(buffer));
  (void) strcpy(buffer,"BITPIX  =                    8");
  (void) strncpy(fits_header+80,buffer,strlen(buffer));
  (void) strcpy(buffer,"NAXIS  =                     2");
  (void) strncpy(fits_header+160,buffer,strlen(buffer));
  (void) sprintf(buffer,"NAXIS1  =           %10u",image->columns);
  (void) strncpy(fits_header+240,buffer,strlen(buffer));
  (void) sprintf(buffer,"NAXIS2  =           %10u",image->rows);
  (void) strncpy(fits_header+320,buffer,strlen(buffer));
  (void) strcpy(buffer,"HISTORY Created by ImageMagick.");
  (void) strncpy(fits_header+400,buffer,strlen(buffer));
  (void) strcpy(buffer,"END");
  (void) strncpy(fits_header+480,buffer,strlen(buffer));
  (void) fwrite((char *) fits_header,1,2880,image->file);
  (void) free((char *) fits_header);
  /*
    Convert image to fits scale PseudoColor class.
  */
  fits_pixels=(unsigned char *)
    malloc(image->columns*image->rows*sizeof(unsigned char));
  if (fits_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=fits_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
      *q++=Intensity(*p);
    p++;
  }
  (void) fwrite((char *) fits_pixels,1,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) fits_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e G I F I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteGIFImage writes an image to a file in the Compuserve Graphics
%  image format.
%
%  The format of the WriteGIFImage routine is:
%
%      status=WriteGIFImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteGIFImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteGIFImage(image)
Image
  *image;
{
  int
    transparency_index;

  register int
    i;

  register RunlengthPacket
    *p;

  unsigned char
    bits_per_pixel,
    c;

  unsigned int
    status;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  transparency_index=(-1);
  if (image->matte)
    {
      /*
        Determine transparency index.
      */
      transparency_index=0;
      p=image->pixels;
      image->runlength=p->length+1;
      for (i=0; i < (image->columns*image->rows); i++)
      {
        if (image->runlength != 0)
          image->runlength--;
        else
          {
            p++;
            image->runlength=p->length;
          }
        if (p->index == 255)
          {
            transparency_index=i;
            break;
          }
      }
    }
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Demote DirectClass to PseudoClass.
      */
      QuantizeImage(image,256,8,True,RGBColorspace,True);
      SyncImage(image);
    }
  for (bits_per_pixel=1; bits_per_pixel < 8; bits_per_pixel++)
    if ((1 << bits_per_pixel) >= image->colors)
      break;
  /*
    Write GIF header.
  */
  if (strcmp(image->magick,"GIF87") == 0)
    (void) fwrite("GIF87a",1,6,image->file);
  else
    (void) fwrite("GIF89a",1,6,image->file);
  LSBFirstWriteShort(image->columns,image->file);
  LSBFirstWriteShort(image->rows,image->file);
  c=0x80;  /* global colormap */
  c|=(8-1) << 4;  /* color resolution */
  c|=(bits_per_pixel-1);   /* size of global colormap */
  (void) fputc((char) c,image->file);
  (void) fputc(0x0,image->file);  /* background color */
  (void) fputc(0x0,image->file);  /* reserved */
  /*
    Write colormap.
  */
  for (i=0; i < image->colors; i++)
  {
    (void) fputc((char) image->colormap[i].red,image->file);
    (void) fputc((char) image->colormap[i].green,image->file);
    (void) fputc((char) image->colormap[i].blue,image->file);
  }
  for ( ; i < (int) (1 << bits_per_pixel); i++)
  {
    (void) fputc(0x0,image->file);
    (void) fputc(0x0,image->file);
    (void) fputc(0x0,image->file);
  }
  if (strcmp(image->magick,"GIF87") != 0)
    {
      if (transparency_index >= 0)
        {
          /*
            Write out extension for transparent color index.
          */
          p=image->pixels;
          image->runlength=p->length+1;
          for (i=0; i < transparency_index; i++)
            if (image->runlength != 0)
              image->runlength--;
            else
              {
                p++;
                image->runlength=p->length;
              }
          (void) fputc(0x21,image->file);
          (void) fputc(0xf9,image->file);
          (void) fputc(0x4,image->file);
          (void) fputc(0x1,image->file);
          (void) fputc(0x0,image->file);
          (void) fputc(0x0,image->file);
          (void) fputc((char) p->index,image->file);
          (void) fputc(0x0,image->file);
        }
      if (image->comments != (char *) NULL)
        {
          register char
            *p;

          register unsigned int
            count;

          /*
            Write comment extension block.
          */
          (void) fputc(0x21,image->file);
          (void) fputc(0xfe,image->file);
          p=image->comments;
          while ((int) strlen(p) > 0)
          {
            count=Min((int) strlen(p),255);
            (void) fputc(count,image->file);
            for (i=0; i < count; i++)
              (void) fputc(*p++,image->file);
          }
          (void) fputc(0x0,image->file);
        }
    }
  (void) fputc(',',image->file);  /* image separator */
  /*
    Write the image header.
  */
  LSBFirstWriteShort(0,image->file);
  LSBFirstWriteShort(0,image->file);
  LSBFirstWriteShort(image->columns,image->file);
  LSBFirstWriteShort(image->rows,image->file);
  (void) fputc(0x0,image->file);
  c=Max(bits_per_pixel,2);
  (void) fputc((char) c,image->file);
  status=LZWEncodeImage(image,Max(bits_per_pixel,2)+1);
  if (status == False)
    {
      Warning("Unable to write image","Memory allocation failed");
      return(False);
    }
  (void) fputc(0x0,image->file);
  (void) fputc(';',image->file); /* terminator */
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e G R A Y I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteGRAYImage writes an image to a file as gray scale intensity
%  values [0..255].
%
%  The format of the WriteGRAYImage routine is:
%
%      status=WriteGRAYImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteGRAYImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteGRAYImage(image)
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

  unsigned char
    *gray_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert image to gray scale PseudoColor class.
  */
  gray_pixels=(unsigned char *)
    malloc(image->columns*image->rows*sizeof(unsigned char));
  if (gray_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=gray_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
      *q++=Intensity(*p);
    p++;
  }
  (void) fwrite((char *) gray_pixels,1,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) gray_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e H I S T O G R A M I m a g e                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteHISTOGRAMImage writes an image to a file in HISTOGRAM format.
%  A list of unique pixel values and the number of times each occurs in the
%  image is written to the file.
%
%  The format of the WriteHISTOGRAMImage routine is:
%
%      status=WriteHISTOGRAMImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteHISTOGRAMImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteHISTOGRAMImage(image)
Image
  *image;
{
  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  (void) NumberColors(image,image->file);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e I R I S I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteIRISImage writes an image in SGI RGB encoded image format.
%
%  The format of the WriteIRISImage routine is:
%
%      status=WriteIRISImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteIRISImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/

static int IRISEncode(pixels,count,packets)
unsigned char
  *pixels;

int
  count;

unsigned char
  *packets;
{
  short
    runlength;

  unsigned char
    *limit,
    *mark,
    *p,
    *q;

  p=pixels;
  limit=p+count*4;
  q=packets;
  while (p < limit)
  {
    mark=p;
    p+=8;
    while ((p < limit) && ((*(p-8) != *(p-4)) || (*(p-4) != *p)))
      p+=4;
    p-=8;
    count=((p-mark) >> 2);
    while (count)
    {
      runlength=count > 126 ? 126 : count;
      count-=runlength;
      *q++=0x80 | runlength;
      for ( ; runlength > 0; runlength--)
      {
        *q++=(*mark);
        mark+=4;
      }
    }
    mark=p;
    p+=4;
    while ((p < limit) && (*p == *mark))
      p+=4;
    count=((p-mark) >> 2);
    while (count)
    {
      runlength=count > 126 ? 126 : count;
      count-=runlength;
      *q++=runlength;
      *q++=(*mark);
    }
  }
  *q++=0;
  return(q-packets);
}

static unsigned int WriteIRISImage(image)
Image
  *image;
{
  typedef struct _IRISHeader
  {
    unsigned short
      magic;

    unsigned char
      storage,
      bytes_per_pixel;

    unsigned short
      dimension,
      columns,
      rows,
      depth;

    unsigned long
      minimum_value,
      maximum_value;

    unsigned char
      filler[492];
  } IRISHeader;

  IRISHeader
    iris_header;

  register int
    i,
    j,
    x,
    y,
    z;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *iris_pixels,
    *packets;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Initialize IRIS raster file header.
  */
  iris_header.magic=0x01DA;
  if (image->compression == NoCompression)
    iris_header.storage=0x01;
  else
    iris_header.storage=0x01;
  iris_header.bytes_per_pixel=1;  /* one byte per pixel */
  iris_header.dimension=3;
  iris_header.columns=image->columns;
  iris_header.rows=image->rows;
  iris_header.depth=image->matte ? 4 : 3;
  if (IsGrayImage(image))
    {
      iris_header.dimension=2;
      iris_header.depth=1;
    }
  iris_header.minimum_value=0;
  iris_header.maximum_value=MaxRGB;
  for (i=0; i < sizeof(iris_header.filler); i++)
    iris_header.filler[i]=0;
  /*
    Write IRIS header.
  */
  MSBFirstWriteShort(iris_header.magic,image->file);
  (void) fputc(iris_header.storage,image->file);
  (void) fputc(iris_header.bytes_per_pixel,image->file);
  MSBFirstWriteShort(iris_header.dimension,image->file);
  MSBFirstWriteShort(iris_header.columns,image->file);
  MSBFirstWriteShort(iris_header.rows,image->file);
  MSBFirstWriteShort(iris_header.depth,image->file);
  MSBFirstWriteLong(iris_header.minimum_value,image->file);
  MSBFirstWriteLong(iris_header.maximum_value,image->file);
  (void) fwrite(iris_header.filler,1,sizeof(iris_header.filler),image->file);
  /*
    Allocate IRIS pixels.
  */
  iris_pixels=(unsigned char *)
    malloc(4*image->columns*image->rows*sizeof(unsigned char));
  if (iris_pixels == (unsigned char *) NULL)
    {
      Warning("Memory allocation error",image->filename);
      return(False);
    }
  /*
    Convert runlength-encoded packets to uncompressed IRIS pixels.
  */
  x=0;
  y=0;
  p=image->pixels;
  q=iris_pixels+(iris_header.rows-1)*(iris_header.columns*4);
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
    {
      *q++=p->red;
      *q++=p->green;
      *q++=p->blue;
      *q++=(unsigned char) p->index;
      x++;
      if (x == image->columns)
        {
          y++;
          q=iris_pixels+((iris_header.rows-1)-y)*(iris_header.columns*4);
          x=0;
        }
    }
    p++;
  }
  if (image->compression == NoCompression)
    {
      unsigned char
        *scanline;

      /*
        Write uncompressed IRIS pixels.
      */
      scanline=(unsigned char *)
        malloc(iris_header.columns*sizeof(unsigned char));
      if (scanline == (unsigned char *) NULL)
        {
          Warning("Memory allocation error",image->filename);
          return(False);
        }
      for (z=0; z < (int) iris_header.depth; z++)
      {
        q=iris_pixels+z;
        for (y=0; y < (int) iris_header.rows; y++)
        {
          for (x=0; x < (int) iris_header.columns; x++)
          {
            scanline[x]=(*q);
            q+=4;
          }
          (void) fwrite(scanline,sizeof(unsigned char),iris_header.columns,
            image->file);
        }
      }
      (void) free(scanline);
    }
  else
    {
      unsigned long
        length,
        number_packets,
        offset,
        *offsets,
        *runlength;

      /*
        Convert IRIS uncompressed pixels to runlength-encoded pixels.
      */
      offsets=(unsigned long *)
        malloc(iris_header.rows*iris_header.depth*sizeof(unsigned long));
      packets=(unsigned char *) malloc(4*((iris_header.columns << 1)+10)*
        image->rows*sizeof(unsigned char));
      runlength=(unsigned long *)
        malloc(iris_header.rows*iris_header.depth*sizeof(unsigned long));
      if ((offsets == (unsigned long *) NULL) ||
          (packets == (unsigned char *) NULL) ||
          (runlength == (unsigned long *) NULL))
        {
          Warning("Memory allocation error",image->filename);
          return(False);
        }
      offset=512+4*((iris_header.rows*iris_header.depth) << 1);
      number_packets=0;
      q=iris_pixels;
      for (y=0; y < (int) iris_header.rows; y++)
      {
        for (z=0; z < (int) iris_header.depth; z++)
        {
          length=
            IRISEncode(q+z,(int) iris_header.columns,packets+number_packets);
          number_packets+=length;
          offsets[y+z*iris_header.rows]=offset;
          runlength[y+z*iris_header.rows]=length;
          offset+=length;
        }
        q+=(iris_header.columns*4);
      }
      /*
        Write out line start and length tables and runlength-encoded pixels.
      */
      for (i=0; i < (int) (iris_header.rows*iris_header.depth); i++)
        MSBFirstWriteLong(offsets[i],image->file);
      for (i=0; i < (int) (iris_header.rows*iris_header.depth); i++)
        MSBFirstWriteLong(runlength[i],image->file);
      (void) fwrite(packets,sizeof(unsigned char),number_packets,image->file);
      /*
        Free memory.
      */
      (void) free(runlength);
      (void) free(packets);
      (void) free(offsets);
    }
  (void) free(iris_pixels);
  CloseImage(image);
  return(True);
}

#ifdef HasJPEG
#undef FREAD
#undef FWRITE
#undef const
#include "jinclude.h"
static Image
  *jpeg_image;
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  W r i t e J P E G I m a g e                                                %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteJPEGImage writes a JPEG image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the WriteJPEGImage routine is:
%
%      status=WriteJPEGImage(image,quality)
%
%  A description of each parameter follows:
%
%    o status:  Function WriteJPEGImage return True if the image is written.
%      False is returned is there is of a memory shortage or if the image
%      file cannot be opened for writing.
%
%    o jpeg_image:  A pointer to a Image structure.
%
%    o quality:  Specifies quality setting.  Quality is 0 (worst) to 100 (best).
%
%
*/

METHODDEF void JPEGInitializeImage(jpeg_info)
compress_info_ptr
  jpeg_info;
{
  /*
    Initialize JPEG image.
  */
  jpeg_info->image_width=jpeg_image->columns;
  jpeg_info->image_height=jpeg_image->rows;
  jpeg_info->data_precision=8;
  jpeg_info->input_components=3;
  jpeg_info->in_color_space=CS_RGB;
  if (IsGrayImage(jpeg_image))
    {
      jpeg_info->input_components=1;
      jpeg_info->in_color_space=CS_GRAYSCALE;
    }
  jpeg_image->packet=jpeg_image->pixels;
  jpeg_image->runlength=jpeg_image->packet->length+1;
}

static void JPEGInputTermMethod(jpeg_info)
compress_info_ptr
  jpeg_info;
{
}

static void JPEGWriteGRAY(jpeg_info,pixel_data)
compress_info_ptr
  jpeg_info;

JSAMPARRAY
  pixel_data;
{
  register int
    column;

  register JSAMPROW
    gray;

  register RunlengthPacket
    *p;

  /*
    Convert run-length encoded grayscale MIFF packets to JPEG pixels.
  */
  gray=pixel_data[0];
  p=jpeg_image->packet;
  for (column=jpeg_info->image_width; column > 0; column--)
  {
    if (jpeg_image->runlength > 0)
      jpeg_image->runlength--;
    else
      {
        p++;
        jpeg_image->runlength=p->length;
      }
    *gray++=(JSAMPLE) p->red;
  }
  jpeg_image->packet=p;
}

static void JPEGWriteRGB(jpeg_info,pixel_data)
compress_info_ptr
  jpeg_info;

JSAMPARRAY
  pixel_data;
{
  register int
    column;

  register JSAMPROW
    blue,
    green,
    red;

  register RunlengthPacket
    *p;

  /*
    Convert run-length encoded MIFF packets to JPEG pixels.
  */
  red=pixel_data[0];
  green=pixel_data[1];
  blue=pixel_data[2];
  p=jpeg_image->packet;
  for (column=jpeg_info->image_width; column > 0; column--)
  {
    if (jpeg_image->runlength > 0)
      jpeg_image->runlength--;
    else
      {
        p++;
        jpeg_image->runlength=p->length;
      }
    *red++=(JSAMPLE) p->red;
    *green++=(JSAMPLE) p->green;
    *blue++=(JSAMPLE) p->blue;
  }
  jpeg_image->packet=p;
}

static void JPEGSelectMethod(jpeg_info)
compress_info_ptr
  jpeg_info;
{
  jpeg_info->methods->get_input_row=JPEGWriteRGB;
  if (jpeg_info->in_color_space == CS_GRAYSCALE)
    {
      j_monochrome_default(jpeg_info);
      jpeg_info->methods->get_input_row=JPEGWriteGRAY;
    }
}

static unsigned int WriteJPEGImage(image,quality)
Image
  *image;

unsigned int
  quality;
{
  struct Compress_info_struct
    jpeg_info;

  struct Compress_methods_struct
    jpeg_methods;

  struct External_methods_struct
    external_methods;

  /*
    Open image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize the JPEG system-dependent methods.
  */
  jpeg_image=image;
  jpeg_info.methods=(&jpeg_methods);
  jpeg_info.emethods=(&external_methods);
  jselerror(&external_methods);
  jselmemmgr(&external_methods);
  jpeg_info.methods->input_init=JPEGInitializeImage;
  jpeg_info.methods->input_term=JPEGInputTermMethod;
  jpeg_methods.c_ui_method_selection=JPEGSelectMethod;
  j_c_defaults(&jpeg_info,quality,False);
  jpeg_info.optimize_coding=True;
  jpeg_info.restart_in_rows=1;
  if ((image->class == PseudoClass) && (image->colors <= 256))
    jpeg_info.smoothing_factor=25;
  /*
    Write a JFIF JPEG file.
  */
#ifdef HUFF_LOOKAHEAD
  jpeg_info.comment_text=jpeg_image->comments;
#endif
  jpeg_info.input_file=(FILE *) NULL;
  jpeg_info.output_file=image->file;
  jselwjfif(&jpeg_info);
  jpeg_compress(&jpeg_info);
  CloseImage(image);
  return(True);
}
#else
static unsigned int WriteJPEGImage(image,quality)
Image
  *image;

unsigned int
  quality;
{
  unsigned int
    status;

  Warning("JPEG library is not available",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e M A P I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteMAPImage writes an image to a file as red, green, and blue
%  colormap bytes followed by the colormap indexes.
%
%  The format of the WriteMAPImage routine is:
%
%      status=WriteMAPImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteMAPImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteMAPImage(image)
Image
  *image;
{
  register int
    i;

  register unsigned char
    *q;

  unsigned char
    *colormap;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Allocate colormap.
  */
  if (image->class == DirectClass)
    {
      /*
        Demote DirectClass to PseudoClass.
      */
      QuantizeImage(image,MaxColormapSize,8,False,RGBColorspace,True);
      SyncImage(image);
    }
  colormap=(unsigned char *) malloc(3*image->colors*sizeof(unsigned char));
  if (colormap == (unsigned char *) NULL)
    {
      Warning("Unable to write image","Memory allocation failed");
      return(False);
    }
  /*
    Write colormap to file.
  */
  q=colormap;
  for (i=0; i < image->colors; i++)
  {
    *q++=image->colormap[i].red;
    *q++=image->colormap[i].green;
    *q++=image->colormap[i].blue;
  }
  (void) fwrite((char *) colormap,3,(int) image->colors,image->file);
  (void) free((char *) colormap);
  /*
    Write image pixels to file.
  */
  image->compression=NoCompression;
  (void) RunlengthEncodeImage(image);
  (void) fwrite((char *) image->packed_pixels,(int) image->packet_size,
    (int) image->packets,image->file);
  (void) free((char *) image->packed_pixels);
  image->packed_pixels=(unsigned char *) NULL;
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e M A T T E I m a g e                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteMATTEImage writes an mage of matte bytes to a file.  It
%  consists of data from the matte component of the image [0..255].
%
%  The format of the WriteMATTEImage routine is:
%
%      status=WriteMATTEImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteMATTEImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteMATTEImage(image)
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

  unsigned char
    *matte_pixels;

  if (!image->matte)
    {
      Warning("Image does not have an matte channel",image->filename);
      return(False);
    }
  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Allocate matte pixels.
  */
  matte_pixels=(unsigned char *)
    malloc(image->columns*image->rows*sizeof(unsigned char));
  if (matte_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=matte_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
      *q++=(unsigned char) p->index;
    p++;
  }
  (void) fwrite((char *) matte_pixels,1,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) matte_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e M I F F I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteMIFFImage writes an image to a file.
%
%  The format of the WriteMIFFImage routine is:
%
%      status=WriteMIFFImage(image)
%
%  A description of each parameter follows:
%
%    o status: Function WriteMIFFImage return True if the image is written.
%      False is returned if there is a memory shortage or if the image file
%      fails to write.
%
%    o image: A pointer to a Image structure.
%
%
*/
static unsigned int WriteMIFFImage(image)
Image
  *image;
{
  register int
    i;

  unsigned long
    packets;

  if ((image->class != DirectClass) && (image->class != PseudoClass))
    {
      Warning("Unable to write image","unknown image class");
      return(False);
    }
  if ((image->compression != RunlengthEncodedCompression) &&
      (image->compression != QEncodedCompression) &&
      (image->compression != NoCompression))
    {
      Warning("Unable to write image","unknown image compression");
      return(False);
    }
  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Pack image pixels.
  */
  if ((image->class == PseudoClass) &&
      (image->compression == QEncodedCompression))
    image->class=DirectClass;
  (void) RunlengthEncodeImage(image);
  packets=image->packets;
  if (image->compression == QEncodedCompression)
    {
      unsigned char
        *compressed_pixels;

      /*
        Compress image pixels with Q encoding.
      */
      compressed_pixels=(unsigned char *)
        malloc((unsigned int) packets*image->packet_size*sizeof(unsigned char));
      if (compressed_pixels == (unsigned char *) NULL)
        {
          Warning("Unable to write image","Memory allocation failed");
          return(False);
        }
      packets=QEncodeImage(image->packed_pixels,compressed_pixels,
        image->columns*(int) image->packet_size,image->rows);
      (void) free((char *) image->packed_pixels);
      image->packed_pixels=compressed_pixels;
      image->packet_size=1;
    }
  if (image->class == PseudoClass)
    ColormapSignature(image);
  /*
    Write header to file.
  */
  if (image->comments != (char *) NULL)
    (void) fprintf(image->file,"{\n%s\n}\n",image->comments);
  (void) fprintf(image->file,"id=ImageMagick\n");
  if (image->class == PseudoClass)
    (void) fprintf(image->file,"class=PseudoClass  colors=%u  signature=%s\n",
      image->colors,image->signature);
  else
    if (image->matte)
      (void) fprintf(image->file,"class=DirectClass  matte=True\n");
    else
      (void) fprintf(image->file,"class=DirectClass\n");
  if (image->compression == RunlengthEncodedCompression)
    (void) fprintf(image->file,"compression=RunlengthEncoded  packets=%lu\n",
      packets);
  else
    if (image->compression == QEncodedCompression)
      (void) fprintf(image->file,"compression=QEncoded  packets=%lu\n",packets);
  (void) fprintf(image->file,"columns=%u  rows=%u\n",image->columns,
    image->rows);
  if (image->scene != 0)
    (void) fprintf(image->file,"scene=%u\n",image->scene);
  if (image->montage != (char *) NULL)
    (void) fprintf(image->file,"montage=%s\n",image->montage);
  (void) fprintf(image->file,"\f\n:\n");
  if (image->montage != (char *) NULL)
    {
      /*
        Write montage tile directory.
      */
      if (image->directory != (char *) NULL)
        (void) fprintf(image->file,"%s",image->directory);
      (void) fputc('\0',image->file);
    }
  if (image->class == PseudoClass)
    {
      register unsigned char
        *q;

      unsigned char
        *colormap;

      /*
        Allocate colormap.
      */
      colormap=(unsigned char *) malloc(3*image->colors*sizeof(unsigned char));
      if (colormap == (unsigned char *) NULL)
        {
          Warning("Unable to write image","Memory allocation failed");
          return(False);
        }
      q=colormap;
      for (i=0; i < image->colors; i++)
      {
        *q++=image->colormap[i].red;
        *q++=image->colormap[i].green;
        *q++=image->colormap[i].blue;
      }
      /*
        Write colormap to file.
      */
      (void) fwrite((char *) colormap,3,(int) image->colors,image->file);
      (void) free((char *) colormap);
    }
  /*
    Write image pixels to file.
  */
  (void) fwrite((char *) image->packed_pixels,(int) image->packet_size,
    (int) packets,image->file);
  (void) free((char *) image->packed_pixels);
  image->packed_pixels=(unsigned char *) NULL;
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e M T V I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteMTVImage writes an image to a file in red, green, and blue
%  MTV rasterfile format.
%
%  The format of the WriteMTVImage routine is:
%
%      status=WriteMTVImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteMTVImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteMTVImage(image)
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

  unsigned char
    *mtv_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert MIFF to MTV raster pixels.
  */
  mtv_pixels=(unsigned char *)
    malloc(3*image->columns*image->rows*sizeof(unsigned char));
  if (mtv_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=mtv_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
    {
      *q++=p->red;
      *q++=p->green;
      *q++=p->blue;
    }
    p++;
  }
  (void) fprintf(image->file,"%u %u\n",image->columns,image->rows);
  (void) fwrite((char *) mtv_pixels,3,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) mtv_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P C D I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WritePCDImage writes an image in the Photo CD encoded image
%  format.
%
%  The format of the WritePCDImage routine is:
%
%      status=WritePCDImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WritePCDImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WritePCDImage(image)
Image
  *image;
{
  unsigned int
    status;

  Warning("Cannot write PCD images",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P C X I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WritePCXImage writes an image in the ZSoft IBM PC Paintbrush file
%  format.
%
%  The format of the WritePCXImage routine is:
%
%      status=WritePCXImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WritePCXImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WritePCXImage(image)
Image
  *image;
{
  typedef struct _PCXHeader
  {
    unsigned char
      identifier,
      version,
      encoding,
      bits_per_pixel;

    short int
      left,
      top,
      right,
      bottom,
      horizontal_resolution,
      vertical_resolution;

    unsigned char
      reserved,
      planes;

    short int
      bytes_per_line,
      palette_info;

    unsigned char
      colormap_signature;
  } PCXHeader;

  PCXHeader
    pcx_header;

  register int
    i,
    j,
    x,
    y;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    count,
    packet,
    *pcx_colormap,
    *pcx_pixels,
    previous;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Demote DirectClass to PseudoClass.
      */
      QuantizeImage(image,256,8,True,RGBColorspace,True);
      SyncImage(image);
    }
  /*
    Initialize PCX raster file header.
  */
  pcx_header.identifier=0x0a;
  pcx_header.version=5;
  pcx_header.encoding=1;
  pcx_header.bits_per_pixel=8;
  if (IsGrayImage(image) && (image->colors == 2))
    pcx_header.bits_per_pixel=1;
  pcx_header.left=0;
  pcx_header.top=0;
  pcx_header.right=image->columns-1;
  pcx_header.bottom=image->rows-1;
  pcx_header.horizontal_resolution=image->columns;
  pcx_header.vertical_resolution=image->rows;
  pcx_header.reserved=0;
  pcx_header.planes=1;
  pcx_header.bytes_per_line=(image->columns*pcx_header.bits_per_pixel+7)/8;
  pcx_header.palette_info=1;
  pcx_header.colormap_signature=0x0c;
  /*
    Write PCX header.
  */
  (void) fwrite(&pcx_header.identifier,1,1,image->file);
  (void) fwrite(&pcx_header.version,1,1,image->file);
  (void) fwrite(&pcx_header.encoding,1,1,image->file);
  (void) fwrite(&pcx_header.bits_per_pixel,1,1,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.left,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.top,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.right,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.bottom,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.horizontal_resolution,
    image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.vertical_resolution,image->file);
  /*
    Dump colormap to file.
  */
  pcx_colormap=(unsigned char *) malloc(3*256*sizeof(unsigned char));
  if (pcx_colormap == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  q=pcx_colormap;
  for (i=0; i < image->colors; i++)
  {
    *q++=image->colormap[i].red;
    *q++=image->colormap[i].green;
    *q++=image->colormap[i].blue;
  }
  (void) fwrite((char *) pcx_colormap,3,16,image->file);
  (void) fwrite(&pcx_header.reserved,1,1,image->file);
  (void) fwrite(&pcx_header.planes,1,1,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.bytes_per_line,image->file);
  LSBFirstWriteShort((unsigned int) pcx_header.palette_info,image->file);
  for (i=0; i < 58; i++)
    (void) fwrite("\0",1,1,image->file);
  /*
    Convert MIFF to PCX raster pixels.
  */
  pcx_pixels=(unsigned char *)
    malloc(pcx_header.bytes_per_line*image->rows*sizeof(unsigned char));
  if (pcx_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  x=0;
  y=0;
  p=image->pixels;
  q=pcx_pixels;
  if (pcx_header.bits_per_pixel > 1)
    for (i=0; i < image->packets; i++)
    {
      for (j=0; j <= (int) p->length; j++)
      {
        *q++=p->index;
        x++;
        if (x == image->columns)
          {
            x=0;
            y++;
            q=pcx_pixels+y*pcx_header.bytes_per_line;
          }
      }
      p++;
    }
  else
    {
      register unsigned char
        bit,
        byte,
        polarity;

      /*
        Convert PseudoClass image to a PCX monochrome image.
      */
      polarity=0;
      if (image->colors == 2)
        polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
      bit=0;
      byte=0;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          byte<<=1;
          if (p->index == polarity)
            byte|=0x01;
          bit++;
          if (bit == 8)
            {
              *q++=byte;
              bit=0;
              byte=0;
            }
          x++;
          if (x == image->columns)
            {
              /*
                Advance to the next scanline.
              */
              if (bit != 0)
                *q++=byte << (8-bit);
              bit=0;
              byte=0;
              x=0;
              y++;
              q=pcx_pixels+y*pcx_header.bytes_per_line;
           }
        }
        p++;
      }
    }
  /*
    Runlength-encoded PCX pixels.
  */
  for (y=0; y < image->rows; y++)
  {
    q=pcx_pixels+y*pcx_header.bytes_per_line;
    previous=(*q++);
    count=1;
    for (x=0; x < (pcx_header.bytes_per_line-1); x++)
    {
      packet=(*q++);
      if ((packet == previous) && (count < 63))
        {
          count++;
          continue;
        }
      if ((count > 1) || ((previous & 0xc0) == 0xc0))
        {
          count|=0xc0;
          (void) fwrite(&count,1,1,image->file);
        }
      (void) fwrite(&previous,1,1,image->file);
      previous=packet;
      count=1;
    }
    if ((count > 1) || ((previous & 0xc0) == 0xc0))
      {
        count|=0xc0;
        (void) fwrite(&count,1,1,image->file);
      }
    (void) fwrite(&previous,1,1,image->file);
  }
  if (image->colors > 16)
    {
      (void) fwrite(&pcx_header.colormap_signature,1,1,image->file);
      (void) fwrite((char *) pcx_colormap,3,256,image->file);
    }
  (void) free((char *) pcx_pixels);
  (void) free((char *) pcx_colormap);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P I C T I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WritePICTImage writes an image to a file in the Apple Macintosh
%  QuickDraw/PICT image format.
%
%  The format of the WritePICTImage routine is:
%
%      status=WritePICTImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WritePICTImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WritePICTImage(image)
Image
  *image;
{
#define MaxCount  128
#define PictClipRegionOp  0x01
#define PictEndOfPictureOp  0xff
#define PictHeaderOp  0x0C00
#define PictHeaderSize  512
#define PictPixmapOp  0x9A
#define PictPackbitsOp  0x98
#define PictVersion  0x11

  typedef struct _PICTRectangle
  {
    unsigned short
      top,
      left,
      bottom,
      right;
  } PICTRectangle;

  typedef struct _PICTPixmap
  {
    unsigned short
      base_address,
      row_bytes;

    PICTRectangle
      bounds;

    unsigned short
      version,
      pack_type;

    unsigned long
      pack_size,
      horizontal_resolution,
      vertical_resolution;

    unsigned short
      pixel_type,
      pixel_size,
      component_count,
      component_size;

    unsigned short
      plane_bytes,
      table,
      reserved;
  } PICTPixmap;

  int
    count;

  PICTPixmap
    pixmap;

  PICTRectangle
    clip_rectangle,
    destination_rectangle,
    frame_rectangle,
    size_rectangle,
    source_rectangle;

  register int
    i,
    j,
    x;

  register RunlengthPacket
    *p;

  unsigned char
    *buffer,
    *packed_scanline,
    *scanline;

  unsigned int
    bytes_per_line;

  unsigned short
    transfer_mode;

  unsigned long
    horizontal_resolution,
    vertical_resolution;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Allocate memory.
  */
  bytes_per_line=image->columns;
  if (image->class == DirectClass)
    bytes_per_line*=3;
  buffer=(unsigned char *) malloc(PictHeaderSize*sizeof(unsigned char));
  packed_scanline=(unsigned char *)
    malloc((bytes_per_line+bytes_per_line/MaxCount+1)*sizeof(unsigned char));
  scanline=(unsigned char *) malloc(bytes_per_line*sizeof(unsigned char));
  if ((buffer == (unsigned char *) NULL) ||
      (packed_scanline == (unsigned char *) NULL) ||
      (scanline == (unsigned char *) NULL))
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  /*
    Initialize image info.
  */
  size_rectangle.top=0;
  size_rectangle.left=0;
  size_rectangle.right=image->rows;
  size_rectangle.bottom=image->columns;
  frame_rectangle=size_rectangle;
  clip_rectangle=size_rectangle;
  source_rectangle=size_rectangle;
  destination_rectangle=size_rectangle;
  horizontal_resolution=0x00480000;
  vertical_resolution=0x00480000;
  pixmap.base_address=0xff;
  pixmap.row_bytes=
    ((image->class == DirectClass ? 4 : 1)*image->columns) | 0x8000;
  pixmap.version=0;
  pixmap.bounds.top=0;
  pixmap.bounds.left=0;
  pixmap.bounds.right=image->rows;
  pixmap.bounds.bottom=image->columns;
  pixmap.pack_type=(image->class == DirectClass ? 0x4 : 0x0);
  pixmap.pack_size=0;
  pixmap.horizontal_resolution=horizontal_resolution;
  pixmap.vertical_resolution=vertical_resolution;
  pixmap.pixel_type=(image->class == DirectClass ? 16 : 0);
  pixmap.pixel_size=(image->class == DirectClass ? 32 : 8);
  pixmap.component_count=(image->class == DirectClass ? 3 : 1);
  pixmap.component_size=8;
  pixmap.plane_bytes=0;
  pixmap.table=0;
  pixmap.reserved=0;
  transfer_mode=(image->class == DirectClass ? 0x40 : 0);
  /*
    Write header, header size, size bounding box, version, and reserved.
  */
  for (i=0; i < PictHeaderSize; i++)
    buffer[i]=0;
  (void) fwrite((char *) buffer,1,PictHeaderSize,image->file);
  MSBFirstWriteShort(0,image->file);
  MSBFirstWriteShort(size_rectangle.top,image->file);
  MSBFirstWriteShort(size_rectangle.left,image->file);
  MSBFirstWriteShort(size_rectangle.right,image->file);
  MSBFirstWriteShort(size_rectangle.bottom,image->file);
  MSBFirstWriteShort(PictVersion,image->file);
  MSBFirstWriteShort(0x02FF,image->file);
  MSBFirstWriteShort(PictHeaderOp,image->file);
  /*
    Write full size of the file, resolution, frame bounding box, and reserved.
  */
  MSBFirstWriteLong(0xFFFE0000L,image->file);
  MSBFirstWriteLong(horizontal_resolution,image->file);
  MSBFirstWriteLong(vertical_resolution,image->file);
  MSBFirstWriteShort(frame_rectangle.top,image->file);
  MSBFirstWriteShort(frame_rectangle.left,image->file);
  MSBFirstWriteShort(frame_rectangle.right,image->file);
  MSBFirstWriteShort(frame_rectangle.bottom,image->file);
  MSBFirstWriteLong(0L,image->file);
  /*
    Write clip region opcode and clip bounding box.
  */
  MSBFirstWriteShort(PictClipRegionOp,image->file);
  MSBFirstWriteShort(0xA,image->file);
  MSBFirstWriteShort(clip_rectangle.top,image->file);
  MSBFirstWriteShort(clip_rectangle.left,image->file);
  MSBFirstWriteShort(clip_rectangle.right,image->file);
  MSBFirstWriteShort(clip_rectangle.bottom,image->file);
  /*
    Write picture opcode, row bytes, and picture bounding box, and version.
  */
  if (image->class == PseudoClass)
    MSBFirstWriteShort(PictPackbitsOp,image->file);
  else
    {
      MSBFirstWriteShort(PictPixmapOp,image->file);
      MSBFirstWriteLong((unsigned long) pixmap.base_address,image->file);
    }
  if (pixmap.row_bytes & 0x01)
    pixmap.row_bytes++;
  MSBFirstWriteShort(pixmap.row_bytes | 0x8000,image->file);
  MSBFirstWriteShort(pixmap.bounds.top,image->file);
  MSBFirstWriteShort(pixmap.bounds.left,image->file);
  MSBFirstWriteShort(pixmap.bounds.right,image->file);
  MSBFirstWriteShort(pixmap.bounds.bottom,image->file);
  MSBFirstWriteShort(pixmap.version,image->file);
  /*
    Write pack type, pack size, resolution, pixel type, and pixel size.
  */
  MSBFirstWriteShort(pixmap.pack_type,image->file);
  MSBFirstWriteLong(pixmap.pack_size,image->file);
  MSBFirstWriteLong(pixmap.horizontal_resolution,image->file);
  MSBFirstWriteLong(pixmap.vertical_resolution,image->file);
  MSBFirstWriteShort(pixmap.pixel_type,image->file);
  MSBFirstWriteShort(pixmap.pixel_size,image->file);
  /*
    Write component count, size, plane bytes, table size, and reserved.
  */
  MSBFirstWriteShort(pixmap.component_count,image->file);
  MSBFirstWriteShort(pixmap.component_size,image->file);
  MSBFirstWriteLong((unsigned long) pixmap.plane_bytes,image->file);
  MSBFirstWriteLong((unsigned long) pixmap.table,image->file);
  MSBFirstWriteLong((unsigned long) pixmap.reserved,image->file);
  if (image->class == PseudoClass)
    {
      unsigned short
        red,
        green,
        blue;

      /*
        Write image colormap.
      */
      MSBFirstWriteLong(0L,image->file);  /* color seed */
      MSBFirstWriteShort(0L,image->file);  /* color flags */
      MSBFirstWriteShort((unsigned short) (image->colors-1),image->file);
      for (i=0; i < image->colors; i++)
      {
        red=(image->colormap[i].red*65535)/(unsigned int) MaxRGB;
        green=(image->colormap[i].green*65535)/(unsigned int) MaxRGB;
        blue=(image->colormap[i].blue*65535)/(unsigned int) MaxRGB;
        MSBFirstWriteShort((unsigned int) i,image->file);
        MSBFirstWriteShort(red,image->file);
        MSBFirstWriteShort(green,image->file);
        MSBFirstWriteShort(blue,image->file);
      }
    }
  /*
    Write source and destination rectangle.
  */
  MSBFirstWriteShort(source_rectangle.top,image->file);
  MSBFirstWriteShort(source_rectangle.left,image->file);
  MSBFirstWriteShort(source_rectangle.right,image->file);
  MSBFirstWriteShort(source_rectangle.bottom,image->file);
  MSBFirstWriteShort(destination_rectangle.top,image->file);
  MSBFirstWriteShort(destination_rectangle.left,image->file);
  MSBFirstWriteShort(destination_rectangle.right,image->file);
  MSBFirstWriteShort(destination_rectangle.bottom,image->file);
  MSBFirstWriteShort(transfer_mode,image->file);
  /*
    Write picture data.
  */
  count=0;
  x=0;
  p=image->pixels;
  if (image->class == PseudoClass)
    {
      register unsigned char
        *index;

      index=scanline;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          *index++=(unsigned char) p->index;
          x++;
          if (x == image->columns)
            {
              count+=PackbitsEncodeImage(image,scanline,packed_scanline);
              index=scanline;
              x=0;
            }
        }
        p++;
      }
    }
  else
    {
      register unsigned char
        *blue,
        *red,
        *green;

      red=scanline;
      green=scanline+image->columns;
      blue=scanline+(image->columns << 1);
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          *red++=p->red;
          *green++=p->green;
          *blue++=p->blue;
          x++;
          if (x == image->columns)
            {
              red=scanline;
              green=scanline+image->columns;
              blue=scanline+(image->columns << 1);
              count+=PackbitsEncodeImage(image,red,packed_scanline);
              x=0;
            }
        }
        p++;
      }
    }
  if (count & 0x1)
    (void) fputc('\0',image->file);
  MSBFirstWriteShort(PictEndOfPictureOp,image->file);
  (void) free((char *) scanline);
  (void) free((char *) packed_scanline);
  (void) free((char *) buffer);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P N M I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure WritePNMImage writes an image to a file in the PNM rasterfile
%  format.
%
%  The format of the WritePNMImage routine is:
%
%      status=WritePNMImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WritePNMImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WritePNMImage(image)
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

  unsigned char
    format,
    *pnm_pixels;

  unsigned int
    packets;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Promote/Demote image based on image type.
  */
  if (strcmp(image->magick,"PPM") == 0)
    image->class=DirectClass;
  else
    if (strcmp(image->magick,"PGM") == 0)
      {
        QuantizeImage(image,256,8,False,GRAYColorspace,True);
        SyncImage(image);
      }
    else
      if (strcmp(image->magick,"PBM") == 0)
        {
          QuantizeImage(image,2,8,False,GRAYColorspace,True);
          SyncImage(image);
        }
      else
        if (image->class == DirectClass)
          if (NumberColors(image,(FILE *) NULL) <= 256)
            {
              /*
                Demote DirectClass to PseudoClass.
              */
              QuantizeImage(image,256,8,False,RGBColorspace,True);
              SyncImage(image);
            }
  /*
    Write PNM file header.
  */
  packets=image->columns*image->rows;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color PNM image.
      */
      format='6';
      packets*=3;
    }
  else
    {
      /*
        Colormapped PNM image.
      */
      format='6';
      if (!IsGrayImage(image))
        packets*=3;
      else
        {
          /*
            Grayscale PNM image.
          */
          format='5';
          if (image->colors == 2)
            format='4';
        }
    }
  (void) fprintf(image->file,"P%c\n",format);
  if (image->comments != (char *) NULL)
    {
      register char
        *p;

      /*
        Write comments to file.
      */
      (void) fprintf(image->file,"# ");
      for (p=image->comments; *p != '\0'; p++)
      {
        (void) fputc(*p,image->file);
        if (*p == '\n')
          (void) fprintf(image->file,"# ");
      }
      (void) fputc('\n',image->file);
    }
  (void) fprintf(image->file,"%u %u\n",image->columns,image->rows);
  /*
    Convert MIFF to PNM raster pixels.
  */
  pnm_pixels=(unsigned char *) malloc(packets*sizeof(unsigned char));
  if (pnm_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=pnm_pixels;
  switch (format)
  {
    case '4':
    {
      register unsigned char
        bit,
        byte,
        polarity;

      unsigned int
        x;

      /*
        Convert image to a PBM image.
      */
      polarity=0;
      if (image->colors == 2)
        polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
      bit=0;
      byte=0;
      x=0;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          byte<<=1;
          if (p->index == polarity)
            byte|=0x01;
          bit++;
          if (bit == 8)
            {
              *q++=byte;
              bit=0;
              byte=0;
            }
          x++;
          if (x == image->columns)
            {
              /*
                Advance to the next scanline.
              */
              if (bit != 0)
                *q++=byte << (8-bit);
              bit=0;
              byte=0;
              x=0;
           }
        }
        p++;
      }
      packets=q-pnm_pixels;
      break;
    }
    case '5':
    {
      /*
        Convert image to a PGM image.
      */
      (void) fprintf(image->file,"%d\n",MaxRGB);
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=image->colormap[p->index].red;
        p++;
      }
      break;
    }
    case '6':
    {
      /*
        Convert image to a PNM image.
      */
      (void) fprintf(image->file,"%d\n",MaxRGB);
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
        }
        p++;
      }
      break;
    }
  }
  (void) fwrite((char *) pnm_pixels,1,(int) packets,image->file);
  (void) free((char *) pnm_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e R L E I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteRLEImage writes an image in the Utah Run length encoded image
%  format.
%
%  The format of the WriteRLEImage routine is:
%
%      status=WriteRLEImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteRLEImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteRLEImage(image)
Image
  *image;
{
  unsigned int
    status;

  Warning("Cannot write RLE images",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P S I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WritePSImage translates an image to encapsulated Postscript
%  Level I for printing.  If the supplied geometry is null, the image is
%  centered on the Postscript page.  Otherwise, the image is positioned as
%  specified by the geometry.
%
%  The format of the WritePSImage routine is:
%
%      status=WritePSImage(image,geometry,density)
%
%  A description of each parameter follows:
%
%    o status: Function WritePSImage return True if the image is printed.
%      False is returned if the image file cannot be opened for printing.
%
%    o image: The address of a structure of type Image;  returned from
%      ReadImage.
%
%    o geometry: A pointer to a geometry string.  Specifies the width and
%      height of the Postscript image.
%
%    o density: A pointer to a density string.  Specifies the dots-per-inch
%      of the Postscript image.
%
%
*/
static unsigned int WritePSImage(image,geometry,density)
Image
  *image;

char
  *geometry,
  *density;
{
  static char
    *Postscript[]=
    {
      "%%BeginProlog",
      "%",
      "% Display a color image.  The image is displayed in color on",
      "% Postscript viewers or printers that support color, otherwise",
      "% it is displayed as grayscale.",
      "%",
      "/buffer 512 string def",
      "/byte 1 string def",
      "/color_packet 3 string def",
      "/pixels 768 string def",
      "",
      "/DirectClassPacket",
      "{",
      "  %",
      "  % Get a DirectClass packet.",
      "  %",
      "  % Parameters: ",
      "  %   red.",
      "  %   green.",
      "  %   blue.",
      "  %   length: number of pixels minus one of this color (optional).",
      "  %",
      "  currentfile color_packet readhexstring pop pop",
      "  compression 0 gt",
      "  {",
      "    /number_pixels 3 def",
      "  }",
      "  {",
      "    currentfile byte readhexstring pop 0 get",
      "    /number_pixels exch 1 add 3 mul def",
      "  } ifelse",
      "  0 3 number_pixels 1 sub",
      "  {",
      "    pixels exch color_packet putinterval",
      "  } for",
      "  pixels 0 number_pixels getinterval",
      "} bind def",
      "",
      "/DirectClassImage",
      "{",
      "  %",
      "  % Display a DirectClass image.",
      "  %",
      "  systemdict /colorimage known",
      "  {",
      "    columns rows 8",
      "    [",
      "      columns 0 0",
      "      rows neg 0 rows",
      "    ]",
      "    { DirectClassPacket } false 3 colorimage",
      "  }",
      "  {",
      "    %",
      "    % No colorimage operator;  convert to grayscale.",
      "    %",
      "    columns rows 8",
      "    [",
      "      columns 0 0",
      "      rows neg 0 rows",
      "    ]",
      "    { GrayDirectClassPacket } image",
      "  } ifelse",
      "} bind def",
      "",
      "/GrayDirectClassPacket",
      "{",
      "  %",
      "  % Get a DirectClass packet;  convert to grayscale.",
      "  %",
      "  % Parameters: ",
      "  %   red",
      "  %   green",
      "  %   blue",
      "  %   length: number of pixels minus one of this color (optional).",
      "  %",
      "  currentfile color_packet readhexstring pop pop",
      "  color_packet 0 get 0.299 mul",
      "  color_packet 1 get 0.587 mul add",
      "  color_packet 2 get 0.114 mul add",
      "  cvi",
      "  /gray_packet exch def",
      "  compression 0 gt",
      "  {",
      "    /number_pixels 1 def",
      "  }",
      "  {",
      "    currentfile byte readhexstring pop 0 get",
      "    /number_pixels exch 1 add def",
      "  } ifelse",
      "  0 1 number_pixels 1 sub",
      "  {",
      "    pixels exch gray_packet put",
      "  } for",
      "  pixels 0 number_pixels getinterval",
      "} bind def",
      "",
      "/GrayPseudoClassPacket",
      "{",
      "  %",
      "  % Get a PseudoClass packet;  convert to grayscale.",
      "  %",
      "  % Parameters: ",
      "  %   index: index into the colormap.",
      "  %   length: number of pixels minus one of this color (optional).",
      "  %",
      "  currentfile byte readhexstring pop 0 get",
      "  /offset exch 3 mul def",
      "  /color_packet colormap offset 3 getinterval def",
      "  color_packet 0 get 0.299 mul",
      "  color_packet 1 get 0.587 mul add",
      "  color_packet 2 get 0.114 mul add",
      "  cvi",
      "  /gray_packet exch def",
      "  compression 0 gt",
      "  {",
      "    /number_pixels 1 def",
      "  }",
      "  {",
      "    currentfile byte readhexstring pop 0 get",
      "    /number_pixels exch 1 add def",
      "  } ifelse",
      "  0 1 number_pixels 1 sub",
      "  {",
      "    pixels exch gray_packet put",
      "  } for",
      "  pixels 0 number_pixels getinterval",
      "} bind def",
      "",
      "/PseudoClassPacket",
      "{",
      "  %",
      "  % Get a PseudoClass packet.",
      "  %",
      "  % Parameters: ",
      "  %   index: index into the colormap.",
      "  %   length: number of pixels minus one of this color (optional).",
      "  %",
      "  currentfile byte readhexstring pop 0 get",
      "  /offset exch 3 mul def",
      "  /color_packet colormap offset 3 getinterval def",
      "  compression 0 gt",
      "  {",
      "    /number_pixels 3 def",
      "  }",
      "  {",
      "    currentfile byte readhexstring pop 0 get",
      "    /number_pixels exch 1 add 3 mul def",
      "  } ifelse",
      "  0 3 number_pixels 1 sub",
      "  {",
      "    pixels exch color_packet putinterval",
      "  } for",
      "  pixels 0 number_pixels getinterval",
      "} bind def",
      "",
      "/PseudoClassImage",
      "{",
      "  %",
      "  % Display a PseudoClass image.",
      "  %",
      "  % Parameters: ",
      "  %   colors: number of colors in the colormap.",
      "  %   colormap: red, green, blue color packets.",
      "  %",
      "  currentfile buffer readline pop",
      "  token pop /colors exch def pop",
      "  /colors colors 3 mul def",
      "  /colormap colors string def",
      "  currentfile colormap readhexstring pop pop",
      "  systemdict /colorimage known",
      "  {",
      "    columns rows 8",
      "    [",
      "      columns 0 0",
      "      rows neg 0 rows",
      "    ]",
      "    { PseudoClassPacket } false 3 colorimage",
      "  }",
      "  {",
      "    %",
      "    % No colorimage operator;  convert to grayscale.",
      "    %",
      "    columns rows 8",
      "    [",
      "      columns 0 0",
      "      rows neg 0 rows",
      "    ]",
      "    { GrayPseudoClassPacket } image",
      "  } ifelse",
      "} bind def",
      "",
      "/DisplayImage",
      "{",
      "  %",
      "  % Display a DirectClass or PseudoClass image.",
      "  %",
      "  % Parameters: ",
      "  %   x & y translation.",
      "  %   x & y scale.",
      "  %   image label.",
      "  %   image columns & rows.",
      "  %   class: 0-DirectClass or 1-PseudoClass.",
      "  %   compression: 0-RunlengthEncodedCompression or 1-NoCompression.",
      "  %   hex color packets.",
      "  %",
      "  gsave",
      "  currentfile buffer readline pop",
      "  token pop /x exch def",
      "  token pop /y exch def pop",
      "  x y translate",
      "  currentfile buffer readline pop",
      "  token pop /x exch def",
      "  token pop /y exch def pop",
      "  /NewCenturySchlbk-Roman findfont 24 scalefont setfont",
      "  currentfile buffer readline pop",
      "  0 y 12 add moveto buffer show pop",
      "  x y scale",
      "  currentfile buffer readline pop",
      "  token pop /columns exch def",
      "  token pop /rows exch def pop",
      "  currentfile buffer readline pop",
      "  token pop /class exch def pop",
      "  currentfile buffer readline pop",
      "  token pop /compression exch def pop",
      "  class 0 gt { PseudoClassImage } { DirectClassImage } ifelse",
      "  grestore",
      "  showpage",
      "} bind def",
      "%%EndProlog",
      "%%Page:  1 1",
      NULL
    };

  char
    *label,
    **q;

  int
    sans_offset,
    x,
    y;

  register RunlengthPacket
    *p;

  register int
    i,
    j;

  time_t
    timer;

  unsigned int
    dx_resolution,
    dy_resolution,
    height,
    text_size,
    x_resolution,
    y_resolution,
    width;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Scale image to size of Postscript page.
  */
  text_size=image->label == (char *) NULL ? 0 : 36;
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  if (strcmp(image->magick,"PS") == 0)
    {
      int
        flags;

      unsigned int
        page_height,
        page_width;

      unsigned long
        scale_factor;

      /*
        Center image on Postscript page.
      */
      (void) XParseGeometry(PSPageGeometry,&x,&y,&page_width,&page_height);
      flags=XParseGeometry(geometry,&x,&y,&page_width,&page_height);
      scale_factor=UpShift(page_width-(x << 1))/image->columns;
      if (scale_factor > (UpShift(page_height-(y << 1)-text_size)/image->rows))
        scale_factor=UpShift(page_height-(y << 1)-text_size)/image->rows;
      width=DownShift(image->columns*scale_factor);
      height=DownShift(image->rows*scale_factor);
      if (((flags & XValue) == 0) && ((flags & YValue) == 0))
        {
          int
            delta_x,
            delta_y;

          delta_x=page_width-(width+(x << 1));
          delta_y=page_height-(height+(y << 1))-text_size;
          if (delta_x >= 0)
            x=(delta_x >> 1)+x;
          if (delta_y >= 0)
            y=(delta_y >> 1)+y;
        }
    }
  /*
    Scale relative to dots-per-inch.
  */
  (void) XParseGeometry(PSDensityGeometry,&sans_offset,&sans_offset,
    &dx_resolution,&dy_resolution);
  x_resolution=dx_resolution;
  y_resolution=dy_resolution;
  (void) XParseGeometry(density,&sans_offset,&sans_offset,&x_resolution,
    &y_resolution);
  width=(width*dx_resolution)/x_resolution;
  height=(height*dy_resolution)/y_resolution;
  /*
    Output Postscript header.
  */
  if (strcmp(image->magick,"PS") == 0)
    (void) fprintf(image->file,"%%!PS-Adobe-3.0\n");
  else
    (void) fprintf(image->file,"%%!PS-Adobe-3.0 EPSF-3.0\n");
  (void) fprintf(image->file,"%%%%Title: %s\n",image->filename);
  (void) fprintf(image->file,"%%%%Creator: ImageMagick\n");
  timer=time((time_t *) NULL);
  (void) localtime(&timer);
  (void) fprintf(image->file,"%%%%CreationDate: %s",ctime(&timer));
  (void) fprintf(image->file,"%%%%BoundingBox: %d %d %d %d\n",x,y,x+(int) width,
    y+(int) height+text_size);
  if (strcmp(image->magick,"PS") == 0)
    {
      (void) fprintf(image->file,"%%%%Orientation: Portrait\n");
      (void) fprintf(image->file,"%%%%PageOrder: Ascend\n");
    }
  (void) fprintf(image->file,"%%%%Pages: %d\n",strcmp(image->magick,"PS") == 0);
  (void) fprintf(image->file,"%%%%EndComments\n");
  if (strcmp(image->magick,"EPSI") == 0)
    {
      Image
        *preview_image;

      register unsigned char
        bit,
        byte,
        polarity;

      unsigned int
        count;

      /*
        Initialize bordered image attributes.
      */
      image->orphan=True;
      preview_image=CopyImage(image,image->columns,image->rows,True);
      image->orphan=False;
      if (preview_image == (Image *) NULL)
        {
          Warning("Unable to include preview image","Memory allocation failed");
          return(False);
        }
      /*
        Convert MIFF to bitmap pixels.
      */
      QuantizeImage(preview_image,2,8,False,GRAYColorspace,True);
      polarity=0;
      if (image->colors == 2)
        polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
      bit=0;
      byte=0;
      count=0;
      x=0;
      p=preview_image->pixels;
      (void) fprintf(image->file,"%%%%BeginPreview: %u %u %u %u\n%%  ",
        preview_image->columns,preview_image->rows,(unsigned int) 1,
        (((preview_image->columns+7) >> 3)*preview_image->rows+35)/36);
      for (i=0; i < preview_image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          byte<<=1;
          if (p->index == polarity)
            byte|=0x01;
          bit++;
          if (bit == 8)
            {
              /*
                Write a bitmap byte to the image file.
              */
              (void) fprintf(image->file,"%02x",byte & 0xff);
              count++;
              if (count == 36)
                {
                  (void) fprintf(image->file,"\n%%  ");
                  count=0;
                };
              bit=0;
              byte=0;
            }
          x++;
          if (x == preview_image->columns)
            {
              if (bit != 0)
                {
                  /*
                    Write a bitmap byte to the image file.
                  */
                  byte<<=(8-bit);
                  (void) fprintf(image->file,"%02x",byte & 0xff);
                  count++;
                  if (count == 36)
                    {
                      (void) fprintf(image->file,"\n%%  ");
                      count=0;
                    };
                  bit=0;
                  byte=0;
                };
              x=0;
            }
          }
          p++;
        }
        (void) fprintf(image->file,"\n%%%%EndPreview\n");
        DestroyImage(preview_image);
      }
  /*
    Output Postscript commands.
  */
  for (q=Postscript; *q; q++)
    (void) fprintf(image->file,"%s\n",*q);
  if (strcmp(image->magick,"PS") != 0)
    (void) fprintf(image->file,"userdict begin\n");
  (void) fprintf(image->file,"%%%%BeginData:\n");
  (void) fprintf(image->file,"DisplayImage\n");
  /*
    Output image data.
  */
  if (image->compression == RunlengthEncodedCompression)
    CompressImage(image);
  label=image->label;
  if (label == (char *) NULL)
    label=" ";
  p=image->pixels;
  switch (image->class)
  {
    case DirectClass:
    {
      (void) fprintf(image->file,"%d %d\n%u %u\n%s           \n%u %u\n%d\n%d\n",
        x,y,width,height,label,image->columns,image->rows,
        image->class == PseudoClass,image->compression == NoCompression);
      switch (image->compression)
      {
        case RunlengthEncodedCompression:
        default:
        {
          /*
            Dump runlength-encoded DirectColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            x++;
            (void) fprintf(image->file,"%02x%02x%02x%02x",p->red,p->green,
              p->blue,p->length);
            if (x == 9)
              {
                x=0;
                (void) fprintf(image->file,"\n");
              }
            p++;
          }
          break;
        }
        case NoCompression:
        {
          /*
            Dump uncompressed DirectColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= ((int) p->length); j++)
            {
              x++;
              (void) fprintf(image->file,"%02x%02x%02x",p->red,p->green,
                p->blue);
              if (x == 12)
                {
                  x=0;
                  (void) fprintf(image->file,"\n");
                }
            }
            p++;
          }
          break;
        }
      }
      break;
    }
    case PseudoClass:
    {
      (void) fprintf(image->file,"%d %d\n%u %u\n%s           \n%u %u\n%d\n%d\n",
        x,y,width,height,label,image->columns,image->rows,
        image->class == PseudoClass,image->compression == NoCompression);
      /*
        Dump number of colors and colormap.
      */
      (void) fprintf(image->file,"%u\n",image->colors);
      for (i=0; i < image->colors; i++)
        (void) fprintf(image->file,"%02x%02x%02x\n",image->colormap[i].red,
          image->colormap[i].green,image->colormap[i].blue);
      switch (image->compression)
      {
        case RunlengthEncodedCompression:
        default:
        {
          /*
            Dump runlength-encoded PseudoColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            x++;
            (void) fprintf(image->file,"%02x%02x",p->index,p->length);
            if (x == 18)
              {
                x=0;
                (void) fprintf(image->file,"\n");
              }
            p++;
          }
          break;
        }
        case NoCompression:
        {
          /*
            Dump uncompressed PseudoColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= ((int) p->length); j++)
            {
              x++;
              (void) fprintf(image->file,"%02x",p->index);
              if (x == 36)
                {
                  x=0;
                  (void) fprintf(image->file,"\n");
                }
            }
            p++;
          }
          break;
        }
      }
    }
  }
  (void) fprintf(image->file,"\n");
  (void) fprintf(image->file,"%%%%EndData\n");
  if (strcmp(image->magick,"PS") != 0)
    (void) fprintf(image->file,"end\n");
  (void) fprintf(image->file,"%%%%PageTrailer\n");
  (void) fprintf(image->file,"%%%%Trailer\n");
  (void) fprintf(image->file,"%%%%EOF\n");
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e P S 2 I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WritePS2Image translates an image to encapsulated Postscript
%  Level II for printing.  If the supplied geometry is null, the image is
%  centered on the Postscript page.  Otherwise, the image is positioned as
%  specified by the geometry.
%
%  The format of the WritePS2Image routine is:
%
%      status=WritePS2Image(image,geometry,density)
%
%  A description of each parameter follows:
%
%    o status: Function WritePS2Image return True if the image is printed.
%      False is returned if the image file cannot be opened for printing.
%
%    o image: The address of a structure of type Image;  returned from
%      ReadImage.
%
%    o geometry: A pointer to a geometry string.  Specifies the width and
%      height of the Postscript image.
%
%    o density: A pointer to a density string.  Specifies the dots-per-inch
%      of the Postscript image.
%
%
*/
static unsigned int WritePS2Image(image,geometry,density)
Image
  *image;

char
  *geometry,
  *density;
{
  static char
    *Postscript[]=
    {
      "%%BeginProlog",
      "%",
      "% Display a color image.  The image is displayed in color on",
      "% Postscript viewers or printers that support color, otherwise",
      "% it is displayed as grayscale.",
      "%",
      "/buffer 512 string def",
      "",
      "/DirectClassImage",
      "{",
      "  %",
      "  % Display a DirectClass image.",
      "  %",
      "  /DeviceRGB setcolorspace",
      "  <<",
      "    /ImageType 1",
      "    /Interpolate true",
      "    /Width columns",
      "    /Height rows",
      "    /BitsPerComponent 8",
      "    /Decode [0 1 0 1 0 1]",
      "    /ImageMatrix [columns 0 0 rows neg 0 rows]",
      "    compression 0 gt",
      "    {",
      "      /DataSource currentfile /ASCIIHexDecode filter",
      "    }",
      "    {",
      "      /DataSource currentfile /ASCIIHexDecode filter /LZWDecode filter",
      "    } ifelse",
      "  >> image",
      "} bind def",
      "",
      "/PseudoClassImage",
      "{",
      "  %",
      "  % Display a PseudoClass image.",
      "  %",
      "  % Parameters: ",
      "  %   colors: number of colors in the colormap.",
      "  %   colormap: red, green, blue color packets.",
      "  %",
      "  currentfile buffer readline pop",
      "  token pop /colors exch def pop",
      "  /colormap colors 3 mul string def",
      "  currentfile colormap readhexstring pop pop",
      "  [ /Indexed /DeviceRGB colors 1 sub colormap ] setcolorspace",
      "  <<",
      "    /ImageType 1",
      "    /Interpolate true",
      "    /Width columns",
      "    /Height rows",
      "    /BitsPerComponent 8",
      "    /Decode [0 255]",
      "    /ImageMatrix [columns 0 0 rows neg 0 rows]",
      "    compression 0 gt",
      "    {",
      "      /DataSource currentfile /ASCIIHexDecode filter",
      "    }",
      "    {",
      "      /DataSource currentfile /ASCIIHexDecode filter /LZWDecode filter",
      "    } ifelse",
      "  >> image",
      "} bind def",
      "",
      "/DisplayImage",
      "{",
      "  %",
      "  % Display a DirectClass or PseudoClass image.",
      "  %",
      "  % Parameters: ",
      "  %   x & y translation.",
      "  %   x & y scale.",
      "  %   image label.",
      "  %   image columns & rows.",
      "  %   class: 0-DirectClass or 1-PseudoClass.",
      "  %   compression: 0-RunlengthEncodedCompression or 1-NoCompression.",
      "  %   hex color packets.",
      "  %",
      "  gsave",
      "  currentfile buffer readline pop",
      "  token pop /x exch def",
      "  token pop /y exch def pop",
      "  x y translate",
      "  currentfile buffer readline pop",
      "  token pop /x exch def",
      "  token pop /y exch def pop",
      "  /NewCenturySchlbk-Roman findfont 24 scalefont setfont",
      "  currentfile buffer readline pop",
      "  0 y 12 add moveto buffer show pop",
      "  x y scale",
      "  currentfile buffer readline pop",
      "  token pop /columns exch def",
      "  token pop /rows exch def pop",
      "  currentfile buffer readline pop",
      "  token pop /class exch def pop",
      "  currentfile buffer readline pop",
      "  token pop /compression exch def pop",
      "  class 0 gt { PseudoClassImage } { DirectClassImage } ifelse",
      "  grestore",
      "  showpage",
      "} bind def",
      "%%EndProlog",
      "%%Page:  1 1",
      NULL
    };

  char
    *label,
    **q;

  int
    sans_offset,
    x,
    y;

  register RunlengthPacket
    *p;

  register int
    i,
    j;

  time_t
    timer;

  unsigned char
    *pixels;

  unsigned int
    dx_resolution,
    dy_resolution,
    height,
    number_packets,
    text_size,
    x_resolution,
    y_resolution,
    width;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Scale image to size of Postscript page.
  */
  text_size=image->label == (char *) NULL ? 0 : 36;
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  if (strcmp(image->magick,"PS") == 0)
    {
      int
        flags;

      unsigned int
        page_height,
        page_width;

      unsigned long
        scale_factor;

      /*
        Center image on Postscript page.
      */
      (void) XParseGeometry(PSPageGeometry,&x,&y,&page_width,&page_height);
      flags=XParseGeometry(geometry,&x,&y,&page_width,&page_height);
      scale_factor=UpShift(page_width-(x << 1))/image->columns;
      if (scale_factor > (UpShift(page_height-(y << 1)-text_size)/image->rows))
        scale_factor=UpShift(page_height-(y << 1)-text_size)/image->rows;
      width=DownShift(image->columns*scale_factor);
      height=DownShift(image->rows*scale_factor);
      if (((flags & XValue) == 0) && ((flags & YValue) == 0))
        {
          int
            delta_x,
            delta_y;

          delta_x=page_width-(width+(x << 1));
          delta_y=page_height-(height+(y << 1))-text_size;
          if (delta_x >= 0)
            x=(delta_x >> 1)+x;
          if (delta_y >= 0)
            y=(delta_y >> 1)+y;
        }
    }
  /*
    Scale relative to dots-per-inch.
  */
  (void) XParseGeometry(PSDensityGeometry,&sans_offset,&sans_offset,
    &dx_resolution,&dy_resolution);
  x_resolution=dx_resolution;
  y_resolution=dy_resolution;
  (void) XParseGeometry(density,&sans_offset,&sans_offset,&x_resolution,
    &y_resolution);
  width=(width*dx_resolution)/x_resolution;
  height=(height*dy_resolution)/y_resolution;
  /*
    Output Postscript header.
  */
  if (strcmp(image->magick,"PS") == 0)
    (void) fprintf(image->file,"%%!PS-Adobe-3.0\n");
  else
    (void) fprintf(image->file,"%%!PS-Adobe-3.0 EPSF-3.0\n");
  (void) fprintf(image->file,"%%%%Title: %s\n",image->filename);
  (void) fprintf(image->file,"%%%%Creator: ImageMagick\n");
  timer=time((time_t *) NULL);
  (void) localtime(&timer);
  (void) fprintf(image->file,"%%%%CreationDate: %s",ctime(&timer));
  (void) fprintf(image->file,"%%%%BoundingBox: %d %d %d %d\n",x,y,x+(int) width,
    y+(int) height+text_size);
  if (strcmp(image->magick,"PS") == 0)
    {
      (void) fprintf(image->file,"%%%%Orientation: Portrait\n");
      (void) fprintf(image->file,"%%%%PageOrder: Ascend\n");
    }
  (void) fprintf(image->file,"%%%%Pages: %d\n",strcmp(image->magick,"PS") == 0);
  (void) fprintf(image->file,"%%%%EndComments\n");
  /*
    Output Postscript commands.
  */
  for (q=Postscript; *q; q++)
    (void) fprintf(image->file,"%s\n",*q);
  if (strcmp(image->magick,"PS") != 0)
    (void) fprintf(image->file,"userdict begin\n");
  (void) fprintf(image->file,"%%%%BeginData:\n");
  (void) fprintf(image->file,"DisplayImage\n");
  /*
    Output image data.
  */
  label=image->label;
  if (label == (char *) NULL)
    label=" ";
  p=image->pixels;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      (void) fprintf(image->file,"%d %d\n%u %u\n%s           \n%u %u\n%d\n%d\n",
        x,y,width,height,label,image->columns,image->rows,
        image->class == PseudoClass,image->compression == NoCompression);
      switch (image->compression)
      {
        case RunlengthEncodedCompression:
        default:
        {
          register unsigned char
            *q;

          /*
            Allocate pixel array.
          */
          number_packets=3*image->columns*image->rows;
          pixels=(unsigned char *) malloc(number_packets*sizeof(unsigned char));
          if (pixels == (unsigned char *) NULL)
            {
              Warning("Memory allocation error",(char *) NULL);
              return(False);
            }
          /*
            Dump LZW encoded pixels.
          */
          q=pixels;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
            {
              *q++=p->red;
              *q++=p->green;
              *q++=p->blue;
            }
            p++;
          }
          (void) LZWEncodeFilter(image->file,pixels,number_packets);
          (void) fprintf(image->file,">");
          (void) free((char *) pixels);
          break;
        }
        case NoCompression:
        {
          /*
            Dump uncompressed DirectColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= ((int) p->length); j++)
            {
              x++;
              (void) fprintf(image->file,"%02x%02x%02x",p->red,p->green,
                p->blue);
              if (x == 12)
                {
                  x=0;
                  (void) fprintf(image->file,"\n");
                }
            }
            p++;
          }
          break;
        }
      }
    }
  else
    {
      (void) fprintf(image->file,"%d %d\n%u %u\n%s           \n%u %u\n%d\n%d\n",
        x,y,width,height,label,image->columns,image->rows,
        image->class == PseudoClass,image->compression == NoCompression);
      /*
        Dump number of colors and colormap.
      */
      (void) fprintf(image->file,"%u\n",image->colors);
      for (i=0; i < image->colors; i++)
        (void) fprintf(image->file,"%02x%02x%02x\n",image->colormap[i].red,
          image->colormap[i].green,image->colormap[i].blue);
      switch (image->compression)
      {
        case RunlengthEncodedCompression:
        default:
        {
          register unsigned char
            *q;

          /*
            Allocate pixel array.
          */
          number_packets=image->columns*image->rows;
          pixels=(unsigned char *) malloc(number_packets*sizeof(unsigned char));
          if (pixels == (unsigned char *) NULL)
            {
              Warning("Memory allocation error",(char *) NULL);
              return(False);
            }
          /*
            Dump LZW encoded pixels.
          */
          q=pixels;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
              *q++=(unsigned char) p->index;
            p++;
          }
          (void) LZWEncodeFilter(image->file,pixels,number_packets);
          (void) fprintf(image->file,">");
          (void) free((char *) pixels);
          break;
        }
        case NoCompression:
        {
          /*
            Dump uncompressed PseudoColor packets.
          */
          x=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= ((int) p->length); j++)
            {
              x++;
              (void) fprintf(image->file,"%02x",p->index);
              if (x == 36)
                {
                  x=0;
                  (void) fprintf(image->file,"\n");
                }
            }
            p++;
          }
          break;
        }
      }
    }
  (void) fprintf(image->file,"\n");
  (void) fprintf(image->file,"%%%%EndData\n");
  if (strcmp(image->magick,"PS") != 0)
    (void) fprintf(image->file,"end\n");
  (void) fprintf(image->file,"%%%%PageTrailer\n");
  (void) fprintf(image->file,"%%%%Trailer\n");
  (void) fprintf(image->file,"%%%%EOF\n");
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e R G B I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteRGBImage writes an image to a file in red, green, and
%  blue rasterfile format.
%
%  The format of the WriteRGBImage routine is:
%
%      status=WriteRGBImage(image,interlace)
%
%  A description of each parameter follows.
%
%    o status: Function WriteRGBImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%    o interlace:  An unsigned integer that specifies the interlacing
%      scheme.
%
%
*/
static unsigned int WriteRGBImage(image,interlace)
Image
  *image;

unsigned int
  interlace;
{
  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *rgb_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert MIFF to RGB raster pixels.
  */
  rgb_pixels=(unsigned char *)
    malloc(3*image->columns*image->rows*sizeof(unsigned char));
  if (rgb_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  q=rgb_pixels;
  switch (interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  RGBRGBRGBRGBRGBRGB...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
        }
        p++;
      }
      break;
    }
    case LineInterlace:
    {
      register int
        x,
        y;

      /*
        Line interlacing:  RRR...GGG...BBB...RRR...GGG...BBB...
      */
      if (!UncompressImage(image))
        return(False);
      for (y=0; y < image->rows; y++)
      {
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->red;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->green;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->blue;
          p++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  RRRRRR...GGGGGG...BBBBBB...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->red;
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->green;
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->blue;
        p++;
      }
      break;
    }
  }
  (void) fwrite((char *) rgb_pixels,3,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) rgb_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e S U N I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteSUNImage writes an image in the SUN rasterfile format.
%
%  The format of the WriteSUNImage routine is:
%
%      status=WriteSUNImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteSUNImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteSUNImage(image)
Image
  *image;
{
#define RMT_EQUAL_RGB  1
#define RMT_NONE  0
#define RMT_RAW  2
#define RT_STANDARD  1
#define RT_FORMAT_RGB  3

  typedef struct _SUNHeader
  {
    unsigned long
      magic,
      width,
      height,
      depth,
      length,
      type,
      maptype,
      maplength;
  } SUNHeader;

  register int
    i,
    j,
    x;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  SUNHeader
    sun_header;

  unsigned char
    *sun_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize SUN raster file header.
  */
  sun_header.magic=0x59a66a95;
  sun_header.width=image->columns;
  sun_header.height=image->rows;
  sun_header.type=(image->class == DirectClass ? RT_FORMAT_RGB : RT_STANDARD);
  sun_header.maptype=RMT_NONE;
  sun_header.maplength=0;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color SUN raster.
      */
      sun_header.depth=(image->matte ? 32 : 24);
      sun_header.length=image->columns*image->rows*(image->matte ? 4 : 3);
      sun_header.length+=image->columns % 2 ? image->rows : 0;
    }
  else
    if (IsGrayImage(image) && (image->colors == 2))
      {
        /*
          Monochrome SUN raster.
        */
        sun_header.depth=1;
        sun_header.length=((image->columns+7) >> 3)*image->rows;
        sun_header.length+=((image->columns/8)+(image->columns % 8 ? 1 : 0)) %
          2 ? image->rows : 0;
      }
    else
      {
        /*
          Colormapped SUN raster.
        */
        sun_header.depth=8;
        sun_header.length=image->columns*image->rows;
        sun_header.length+=image->columns % 2 ? image->rows : 0;
        sun_header.maptype=RMT_EQUAL_RGB;
        sun_header.maplength=image->colors*3;
      }
  /*
    Write SUN header.
  */
  MSBFirstWriteLong(sun_header.magic,image->file);
  MSBFirstWriteLong(sun_header.width,image->file);
  MSBFirstWriteLong(sun_header.height,image->file);
  MSBFirstWriteLong(sun_header.depth,image->file);
  MSBFirstWriteLong(sun_header.length,image->file);
  MSBFirstWriteLong(sun_header.type,image->file);
  MSBFirstWriteLong(sun_header.maptype,image->file);
  MSBFirstWriteLong(sun_header.maplength,image->file);
  /*
    Convert MIFF to SUN raster pixels.
  */
  sun_pixels=(unsigned char *) malloc(sun_header.length*sizeof(unsigned char));
  if (sun_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=sun_pixels;
  x=0;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Convert DirectClass packet to SUN RGB pixel.
      */
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          if (image->matte)
            *q++=(unsigned char) p->index;
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
          x++;
          if (x == image->columns)
            {
              if ((image->columns % 2) != 0)
                q++;  /* pad scanline */
              x=0;
            }
        }
        p++;
      }
    }
  else
    if (IsGrayImage(image) && (image->colors == 2))
      {
        register unsigned char
          bit,
          byte,
          polarity;

        /*
          Convert PseudoClass image to a SUN monochrome image.
        */
        polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
        bit=0;
        byte=0;
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
          {
            byte<<=1;
            if (p->index == polarity)
              byte|=0x01;
            bit++;
            if (bit == 8)
              {
                *q++=byte;
                bit=0;
                byte=0;
              }
            x++;
            if (x == image->columns)
              {
                /*
                  Advance to the next scanline.
                */
                if (bit != 0)
                  *q++=byte << (8-bit);
                if ((((image->columns/8)+
                    (image->columns % 8 ? 1 : 0)) % 2) != 0)
                  q++;  /* pad scanline */
                bit=0;
                byte=0;
                x=0;
             }
          }
          p++;
        }
      }
    else
      {
        unsigned char
          *sun_colormap;

        /*
          Dump colormap to file.
        */
        sun_colormap=(unsigned char *)
          malloc(sun_header.maplength*sizeof(unsigned char));
        if (sun_colormap == (unsigned char *) NULL)
          {
            Warning("Unable to allocate memory",(char *) NULL);
            return(False);
          }
        q=sun_colormap;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].red;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].green;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].blue;
        (void) fwrite((char *) sun_colormap,1,(int) sun_header.maplength,
          image->file);
        (void) free((char *) sun_colormap);
        /*
          Convert PseudoClass packet to SUN colormapped pixel.
        */
        q=sun_pixels;
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
          {
            *q++=p->index;
            x++;
            if (x == image->columns)
              {
                if ((image->columns % 2) != 0)
                  q++;  /* pad scanline */
                x=0;
              }
          }
          p++;
        }
      }
  (void) fwrite((char *) sun_pixels,1,(int) sun_header.length,image->file);
  (void) free((char *) sun_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e T A R G A I m a g e                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteTARGAImage writes a image in the Truevision Targa rasterfile
%  format.
%
%  The format of the WriteTARGAImage routine is:
%
%      status=WriteTARGAImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteTARGAImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteTARGAImage(image)
Image
  *image;
{
#define TargaColormap 1
#define TargaRGB 2
#define TargaMonochrome 3
#define TargaRLEColormap  9
#define TargaRLERGB  10
#define TargaRLEMonochrome  11

  typedef struct _TargaHeader
  {
    unsigned char
      id_length,
      colormap_type,
      image_type;

    unsigned short
      colormap_index,
      colormap_length;

    unsigned char
      colormap_size;

    unsigned short
      x_origin,
      y_origin,
      width,
      height;

    unsigned char
      pixel_size,
      attributes;
  } TargaHeader;

  int
    count,
    runlength;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q,
    *r;

  TargaHeader
    targa_header;

  unsigned char
    *targa_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Reflect image.
  */
  image=FlopImage(image);
  if (image == (Image *) NULL)
    {
      Warning("Unable to reflect file",image->filename);
      return(False);
    }
  if (image->compression == RunlengthEncodedCompression)
    CompressImage(image);
  /*
    Initialize TARGA raster file header.
  */
  targa_header.id_length=0;
  if (image->comments != (char *) NULL)
    targa_header.id_length=Min((int) strlen(image->comments),255);
  targa_header.colormap_type=0;
  targa_header.colormap_index=0;
  targa_header.colormap_length=0;
  targa_header.colormap_size=0;
  targa_header.x_origin=0;
  targa_header.y_origin=0;
  targa_header.width=image->columns;
  targa_header.height=image->rows;
  targa_header.pixel_size=8;
  targa_header.attributes=0;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color TARGA raster.
      */
      targa_header.image_type=TargaRGB;
      if (image->compression == RunlengthEncodedCompression)
        targa_header.image_type=TargaRLERGB;
      targa_header.pixel_size=image->matte ? 32 : 24;
    }
  else
    {
      /*
        Colormapped TARGA raster.
      */
      targa_header.image_type=TargaColormap;
      if (image->compression == RunlengthEncodedCompression)
        targa_header.image_type=TargaRLEColormap;
      if (!IsGrayImage(image) || (image->colors != 2))
        {
          targa_header.colormap_type=1;
          targa_header.colormap_index=0;
          targa_header.colormap_length=image->colors;
          targa_header.colormap_size=24;
        }
      else
        {
          /*
            Monochrome TARGA raster.
          */
          targa_header.image_type=TargaMonochrome;
          if (image->compression == RunlengthEncodedCompression)
            targa_header.image_type=TargaRLEMonochrome;
        }
    }
  /*
    Write TARGA header.
  */
  (void) fputc((char) targa_header.id_length,image->file);
  (void) fputc((char) targa_header.colormap_type,image->file);
  (void) fputc((char) targa_header.image_type,image->file);
  LSBFirstWriteShort(targa_header.colormap_index,image->file);
  LSBFirstWriteShort(targa_header.colormap_length,image->file);
  (void) fputc((char) targa_header.colormap_size,image->file);
  LSBFirstWriteShort(targa_header.x_origin,image->file);
  LSBFirstWriteShort(targa_header.y_origin,image->file);
  LSBFirstWriteShort(targa_header.width,image->file);
  LSBFirstWriteShort(targa_header.height,image->file);
  (void) fputc((char) targa_header.pixel_size,image->file);
  (void) fputc((char) targa_header.attributes,image->file);
  if (targa_header.id_length != 0)
    (void) fwrite((char *) image->comments,1,targa_header.id_length,
      image->file);
  /*
    Convert MIFF to TARGA raster pixels.
  */
  count=(unsigned int)
    (targa_header.pixel_size*targa_header.width*targa_header.height) >> 3;
  if (image->compression == RunlengthEncodedCompression)
    count+=(count/128)+1;
  targa_pixels=(unsigned char *) malloc(count*sizeof(unsigned char));
  if (targa_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels+(image->packets-1);
  q=targa_pixels;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Convert DirectClass packet to TARGA RGB pixel.
      */
      if (image->compression != RunlengthEncodedCompression)
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
          {
            *q++=p->blue;
            *q++=p->green;
            *q++=p->red;
            if (image->matte)
              *q++=p->index;
          }
          p--;
        }
      else
        for (i=0; i < image->packets; i++)
        {
          runlength=p->length+1;
          if (runlength > 128)
            {
              *q++=0xff;
              *q++=p->blue;
              *q++=p->green;
              *q++=p->red;
              if (image->matte)
                *q++=p->index;
              runlength-=128;
            }
          r=q;
          *q++=0x80+(runlength-1);
          *q++=p->blue;
          *q++=p->green;
          *q++=p->red;
          if (image->matte)
            *q++=p->index;
          if (runlength != 1)
            p--;
          else
            {
              for ( ; i < image->packets; i++)
              {
                p--;
                if ((p->length != 0) || (runlength == 128))
                  break;
                *q++=p->blue;
                *q++=p->green;
                *q++=p->red;
                if (image->matte)
                  *q++=p->index;
                runlength++;
              }
              *r=runlength-1;
            }
        }
    }
  else
    if (!IsGrayImage(image) || (image->colors != 2))
      {
        unsigned char
          *targa_colormap;

        /*
          Dump colormap to file (blue, green, red byte order).
        */
        targa_colormap=(unsigned char *)
          malloc(3*targa_header.colormap_length*sizeof(unsigned char));
        if (targa_colormap == (unsigned char *) NULL)
          {
            Warning("Unable to allocate memory",(char *) NULL);
            return(False);
          }
        q=targa_colormap;
        for (i=0; i < image->colors; i++)
        {
          *q++=image->colormap[i].blue;
          *q++=image->colormap[i].green;
          *q++=image->colormap[i].red;
        }
        (void) fwrite((char *) targa_colormap,1,
          (int) 3*targa_header.colormap_length,image->file);
        (void) free((char *) targa_colormap);
        /*
          Convert PseudoClass packet to TARGA colormapped pixel.
        */
        q=targa_pixels;
        if (image->compression != RunlengthEncodedCompression)
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
              *q++=p->index;
            p--;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            runlength=p->length+1;
            if (runlength > 128)
              {
                *q++=0xff;
                *q++=p->index;
                runlength-=128;
              }
            r=q;
            *q++=0x80+(runlength-1);
            *q++=p->index;
            if (runlength != 1)
              p--;
            else
              {
                for ( ; i < image->packets; i++)
                {
                  p--;
                  if ((p->length != 0) || (runlength == 128))
                    break;
                  *q++=p->index;
                  runlength++;
                }
                *r=runlength-1;
              }
          }
      }
    else
      {
        unsigned int
          polarity;

        /*
          Convert PseudoClass image to a TARGA monochrome image.
        */
        polarity=0;
        if (image->colors == 2)
          polarity=
            Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
        if (image->compression != RunlengthEncodedCompression)
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
              *q++=p->index == polarity ? 0 : MaxRGB;
            p--;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            runlength=p->length+1;
            if (runlength > 128)
              {
                *q++=0xff;
                *q++=p->index == polarity ? 0 : MaxRGB;
                runlength-=128;
              }
            r=q;
            *q++=0x80+(runlength-1);
            *q++=p->index == polarity ? 0 : MaxRGB;
            if (runlength != 1)
              p--;
            else
              {
                for ( ; i < image->packets; i++)
                {
                  p--;
                  if ((p->length != 0) || (runlength == 128))
                    break;
                  *q++=p->index == polarity ? 0 : MaxRGB;
                  runlength++;
                }
                *r=runlength-1;
              }
          }
      }
  (void) fwrite((char *) targa_pixels,1,(int) (q-targa_pixels),image->file);
  (void) free((char *) targa_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e T E X T I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteTEXTImage writes an image in the TEXT image forma.
%
%  The format of the WriteTEXTImage routine is:
%
%      status=WriteTEXTImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteTEXTImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteTEXTImage(image)
Image
  *image;
{
  unsigned int
    status;

  Warning("Cannot write TEXT images",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}

#ifdef HasTIFF
#include "tiff.h"
#include "tiffio.h"
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e T I F F I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteTIFFImage writes an image in the Tagged image file format.
%
%  The format of the WriteTIFFImage routine is:
%
%      status=WriteTIFFImage(image,verbose)
%
%  A description of each parameter follows:
%
%    o status:  Function WriteTIFFImage return True if the image is written.
%      False is returned is there is of a memory shortage or if the image
%      file cannot be opened for writing.
%
%    o image:  A pointer to a Image structure.
%
%    o verbose:  A value greater than zero prints detailed information about
%      the image.
%
%
*/
static unsigned int WriteTIFFImage(image,verbose)
Image
  *image;

unsigned int
  verbose;
{
  register RunlengthPacket
    *p;

  register int
    i,
    j,
    x,
    y;

  register unsigned char
    *q;

  TIFF
    *tiff;

  unsigned char
    *scanline;

  unsigned short
    photometric,
    rows_per_strip;

  /*
    Open TIFF file.
  */
  tiff=TIFFOpen(image->filename,"wb");
  if (tiff == (TIFF *) NULL)
    return(False);
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize TIFF fields.
  */
  TIFFSetField(tiff,TIFFTAG_DOCUMENTNAME,image->filename);
  TIFFSetField(tiff,TIFFTAG_SOFTWARE,"ImageMagick");
  if (image->comments != (char *) NULL)
    TIFFSetField(tiff,TIFFTAG_IMAGEDESCRIPTION,image->comments);
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color TIFF raster.
      */
      photometric=PHOTOMETRIC_RGB;
      TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,8);
      TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,(image->matte ? 4 : 3));
    }
  else
    {
      /*
        Colormapped TIFF raster.
      */
      photometric=PHOTOMETRIC_PALETTE;
      TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,1);
      TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,8);
      if (IsGrayImage(image))
        {
          /*
            Grayscale TIFF raster.
          */
          photometric=PHOTOMETRIC_MINISBLACK;
          if (image->colors == 2)
            TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,1);
        }
    }
  TIFFSetField(tiff,TIFFTAG_PHOTOMETRIC,photometric);
  TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,image->rows);
  TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,image->columns);
  TIFFSetField(tiff,TIFFTAG_FILLORDER,FILLORDER_MSB2LSB);
  TIFFSetField(tiff,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT);
  TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);
  TIFFSetField(tiff,TIFFTAG_COMPRESSION,COMPRESSION_LZW);
  if (image->compression == NoCompression)
    TIFFSetField(tiff,TIFFTAG_COMPRESSION,COMPRESSION_NONE);
  rows_per_strip=8192/TIFFScanlineSize(tiff);
  if (rows_per_strip == 0)
    rows_per_strip=1;
  TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,rows_per_strip);
  scanline=(unsigned char *) malloc(TIFFScanlineSize(tiff));
  if (scanline == (unsigned char *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=scanline;
  x=0;
  y=0;
  if (photometric == PHOTOMETRIC_RGB)
    for (i=0; i < image->packets; i++)
    {
      for (j=0; j <= (int) p->length; j++)
      {
        /*
          Convert DirectClass packets to contiguous RGB scanlines.
        */
        *q++=p->red;
        *q++=p->green;
        *q++=p->blue;
        if (image->matte)
          *q++=(unsigned char) p->index;
        x++;
        if (x == image->columns)
          {
            if (TIFFWriteScanline(tiff,scanline,y,0) < 0)
              break;
            q=scanline;
            x=0;
            y++;
          }
      }
      p++;
    }
  else
    if (photometric == PHOTOMETRIC_PALETTE)
      {
        unsigned short
          blue[256],
          green[256],
          red[256];

        /*
          Initialize TIFF colormap.
        */
        for (i=0; i < image->colors; i++)
        {
          red[i]=(unsigned short)
            ((image->colormap[i].red*65535)/(unsigned int) MaxRGB);
          green[i]=(unsigned short)
            ((image->colormap[i].green*65535)/(unsigned int) MaxRGB);
          blue[i]=(unsigned short)
            ((image->colormap[i].blue*65535)/(unsigned int) MaxRGB);
        }
        TIFFSetField(tiff,TIFFTAG_COLORMAP,red,green,blue);
        /*
          Convert PseudoClass packets to contiguous colormap indexed scanlines.
        */
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
          {
            *q++=(unsigned char) p->index;
            x++;
            if (x == image->columns)
              {
                if (TIFFWriteScanline(tiff,scanline,y,0) < 0)
                  break;
                q=scanline;
                x=0;
                y++;
              }
          }
          p++;
        }
      }
    else
      if (image->colors > 2)
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
          {
            /*
              Convert PseudoClass packets to contiguous grayscale scanlines.
            */
            *q++=(unsigned char) image->colormap[p->index].red;
            x++;
            if (x == image->columns)
              {
                if (TIFFWriteScanline(tiff,scanline,y,0) < 0)
                  break;
                q=scanline;
                x=0;
                y++;
              }
          }
          p++;
        }
      else
        {
          register unsigned char
            bit,
            byte,
            polarity;

          /*
            Convert PseudoClass packets to contiguous monochrome scanlines.
          */
          polarity=0;
          if (image->colors == 2)
            polarity=
              Intensity(image->colormap[0]) < Intensity(image->colormap[1]);
          bit=0;
          byte=0;
          x=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
            {
              byte<<=1;
              if (p->index == polarity)
                byte|=0x01;
              bit++;
              if (bit == 8)
                {
                  *q++=byte;
                  bit=0;
                  byte=0;
                }
              x++;
              if (x == image->columns)
                {
                  /*
                    Advance to the next scanline.
                  */
                  if (bit != 0)
                    *q++=byte << (8-bit);
                  if (TIFFWriteScanline(tiff,scanline,y,0) < 0)
                    break;
                  q=scanline;
                  bit=0;
                  byte=0;
                  x=0;
                  y++;
               }
            }
            p++;
          }
        }
  (void) free((char *) scanline);
  (void) TIFFFlushData(tiff);
  if (verbose == True)
    TIFFPrintDirectory(tiff,stderr,False);
  (void) TIFFClose(tiff);
  return(True);
}
#else
static unsigned int WriteTIFFImage(image,verbose)
Image
  *image;

unsigned int
  verbose;
{
  unsigned int
    status;

  Warning("TIFF library is not available",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e V I C A R I m a g e                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteVICARImage writes an image in the VICAR rasterfile format.
%
%  The format of the WriteVICARImage routine is:
%
%      status=WriteVICARImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteVICARImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteVICARImage(image)
Image
  *image;
{
  unsigned int
    status;

  Warning("Cannot write VICAR images",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e V I F F I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteVIFFImage writes an image to a file in the VIFF image format.
%
%  The format of the WriteVIFFImage routine is:
%
%      status=WriteVIFFImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteVIFFImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteVIFFImage(image)
Image
  *image;
{
#define VFF_CM_genericRGB  15
#define VFF_CM_NONE  0
#define VFF_DEP_IEEEORDER  0x2
#define VFF_DES_RAW  0
#define VFF_LOC_IMPLICIT  1
#define VFF_MAPTYP_NONE  0
#define VFF_MAPTYP_1_BYTE  1
#define VFF_MS_NONE  0
#define VFF_MS_ONEPERBAND  1
#define VFF_TYP_BIT  0
#define VFF_TYP_1_BYTE  1

  typedef struct _ViffHeader
  {
    char
      identifier,
      file_type,
      release,
      version,
      machine_dependency,
      reserve[3],
      comment[512];

    unsigned long
      rows,
      columns,
      subrows;

    long
      x_offset,
      y_offset;

    unsigned int
      x_pixel_size,
      y_pixel_size;

    unsigned long
      location_type,
      location_dimension,
      number_of_images,
      number_data_bands,
      data_storage_type,
      data_encode_scheme,
      map_scheme,
      map_storage_type,
      map_rows,
      map_columns,
      map_subrows,
      map_enable,
      maps_per_cycle,
      color_space_model;
  } ViffHeader;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    buffer[8],
    *viff_pixels;

  unsigned long
    packets;

  ViffHeader
    viff_header;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize VIFF image structure.
  */
  viff_header.identifier=0xab;
  viff_header.file_type=1;
  viff_header.release=1;
  viff_header.version=3;
  viff_header.machine_dependency=VFF_DEP_IEEEORDER;  /* IEEE byte ordering */
  *viff_header.comment='\0';
  if (image->comments != (char *) NULL)
    {
      (void) strncpy(viff_header.comment,image->comments,
        Min((int) strlen(image->comments),511));
      viff_header.comment[Min((int) strlen(image->comments),511)]='\0';
    }
  viff_header.rows=image->columns;
  viff_header.columns=image->rows;
  viff_header.subrows=0;
  viff_header.x_offset=(~0);
  viff_header.y_offset=(~0);
  viff_header.x_pixel_size=0;
  viff_header.y_pixel_size=0;
  viff_header.location_type=VFF_LOC_IMPLICIT;
  viff_header.location_dimension=0;
  viff_header.number_of_images=1;
  viff_header.data_encode_scheme=VFF_DES_RAW;
  viff_header.map_scheme=VFF_MS_NONE;
  viff_header.map_storage_type=VFF_MAPTYP_NONE;
  viff_header.map_rows=0;
  viff_header.map_columns=0;
  viff_header.map_subrows=0;
  viff_header.map_enable=1;  /* no colormap */
  viff_header.maps_per_cycle=0;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Full color VIFF raster.
      */
      viff_header.number_data_bands=image->matte ? 4 : 3;
      viff_header.color_space_model=VFF_CM_genericRGB;
      viff_header.data_storage_type=VFF_TYP_1_BYTE;
      packets=image->columns*image->rows*viff_header.number_data_bands;
    }
  else
    {
      viff_header.number_data_bands=1;
      viff_header.color_space_model=VFF_CM_NONE;
      viff_header.data_storage_type=VFF_TYP_1_BYTE;
      packets=image->columns*image->rows;
      if (!IsGrayImage(image))
        {
          /*
            Colormapped VIFF raster.
          */
          viff_header.map_scheme=VFF_MS_ONEPERBAND;
          viff_header.map_storage_type=VFF_MAPTYP_1_BYTE;
          viff_header.map_rows=3;
          viff_header.map_columns=image->colors;
        }
      else
        if (image->colors == 2)
          {
            /*
              Monochrome VIFF raster.
            */
            viff_header.data_storage_type=VFF_TYP_BIT;
            packets=((image->columns+7) >> 3)*image->rows;
          }
    }
  /*
    Write VIFF image header (pad to 1024 bytes).
  */
  buffer[0]=viff_header.identifier;
  buffer[1]=viff_header.file_type;
  buffer[2]=viff_header.release;
  buffer[3]=viff_header.version;
  buffer[4]=viff_header.machine_dependency;
  buffer[5]=viff_header.reserve[0];
  buffer[6]=viff_header.reserve[1];
  buffer[7]=viff_header.reserve[2];
  (void) fwrite((char *) buffer,1,8,image->file);
  (void) fwrite((char *) viff_header.comment,1,512,image->file);
  MSBFirstWriteLong(viff_header.rows,image->file);
  MSBFirstWriteLong(viff_header.columns,image->file);
  MSBFirstWriteLong(viff_header.subrows,image->file);
  MSBFirstWriteLong((unsigned long) viff_header.x_offset,image->file);
  MSBFirstWriteLong((unsigned long) viff_header.y_offset,image->file);
  MSBFirstWriteLong((unsigned long) viff_header.x_pixel_size,image->file);
  MSBFirstWriteLong((unsigned long) viff_header.y_pixel_size,image->file);
  MSBFirstWriteLong(viff_header.location_type,image->file);
  MSBFirstWriteLong(viff_header.location_dimension,image->file);
  MSBFirstWriteLong(viff_header.number_of_images,image->file);
  MSBFirstWriteLong(viff_header.number_data_bands,image->file);
  MSBFirstWriteLong(viff_header.data_storage_type,image->file);
  MSBFirstWriteLong(viff_header.data_encode_scheme,image->file);
  MSBFirstWriteLong(viff_header.map_scheme,image->file);
  MSBFirstWriteLong(viff_header.map_storage_type,image->file);
  MSBFirstWriteLong(viff_header.map_rows,image->file);
  MSBFirstWriteLong(viff_header.map_columns,image->file);
  MSBFirstWriteLong(viff_header.map_subrows,image->file);
  MSBFirstWriteLong(viff_header.map_enable,image->file);
  MSBFirstWriteLong(viff_header.maps_per_cycle,image->file);
  MSBFirstWriteLong(viff_header.color_space_model,image->file);
  for (i=0; i < 420; i++)
    (void) fputc('\0',image->file);
  /*
    Convert MIFF to VIFF raster pixels.
  */
  viff_pixels=(unsigned char *) malloc(packets*sizeof(unsigned char));
  if (viff_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  p=image->pixels;
  q=viff_pixels;
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      unsigned long
        offset;

      /*
        Convert DirectClass packet to VIFF RGB pixel.
      */
      offset=image->columns*image->rows;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= (int) p->length; j++)
        {
          *q=p->red;
          *(q+offset)=p->green;
          *(q+offset*2)=p->blue;
          if (image->matte)
            *(q+offset*3)=(unsigned char) p->index;
          q++;
        }
        p++;
      }
    }
  else
    if (!IsGrayImage(image))
      {
        unsigned char
          *viff_colormap;

        /*
          Dump colormap to file.
        */
        viff_colormap=(unsigned char *)
          malloc(image->colors*3*sizeof(unsigned char));
        if (viff_colormap == (unsigned char *) NULL)
          {
            Warning("Unable to allocate memory",(char *) NULL);
            return(False);
          }
        q=viff_colormap;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].red;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].green;
        for (i=0; i < image->colors; i++)
          *q++=image->colormap[i].blue;
        (void) fwrite((char *) viff_colormap,1,(int) image->colors*3,
          image->file);
        (void) free((char *) viff_colormap);
        /*
          Convert PseudoClass packet to VIFF colormapped pixels.
        */
        q=viff_pixels;
        for (i=0; i < image->packets; i++)
        {
          for (j=0; j <= (int) p->length; j++)
            *q++=p->index;
          p++;
        }
      }
    else
      if (image->colors == 2)
        {
          register unsigned char
            bit,
            byte,
            polarity;

          register int
            x;

          /*
            Convert PseudoClass image to a VIFF monochrome image.
          */
          polarity=
            Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
          x=0;
          bit=0;
          byte=0;
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
            {
              byte>>=1;
              if (p->index == polarity)
                byte|=0x80;
              bit++;
              if (bit == 8)
                {
                  *q++=byte;
                  bit=0;
                  byte=0;
                }
              x++;
              if (x == image->columns)
                {
                  /*
                    Advance to the next scanline.
                  */
                  if (bit != 0)
                    *q++=byte >> (8-bit);
                  bit=0;
                  byte=0;
                  x=0;
               }
            }
            p++;
          }
        }
      else
        {
          /*
            Convert PseudoClass packet to VIFF grayscale pixel.
          */
          for (i=0; i < image->packets; i++)
          {
            for (j=0; j <= (int) p->length; j++)
              *q++=p->red;
            p++;
          }
        }
  (void) fwrite((char *) viff_pixels,1,(int) packets,image->file);
  (void) free((char *) viff_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e X I m a g e                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteXImage writes an image to an X server.
%
%  The format of the WriteXImage routine is:
%
%      status=WriteXImage(image,server_name)
%
%  A description of each parameter follows.
%
%    o status: Function WriteXImage return True if the image is displayed on
%      the X server.  False is returned is there is a memory shortage or if
%      the image file fails to write.
%
%    o image:  A pointer to a Image structure.
%
%    o server_name:  A pointer to a character string representing the
%      X server to display the image.
%
%
*/
static unsigned int WriteXImage(image,server_name)
Image
  *image;

char
  *server_name;
{
  Atom
    wm_delete_window,
    wm_protocols;

  char
    *resource_value;

  Display
    *display;

  register char
    *p;

  unsigned int
    status;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  XEvent
    event;

  XPixelInfo
    pixel_info;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  XWindowInfo
    window_info;

  /*
    Open X server connection.
  */
  display=XOpenDisplay(server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",XDisplayName(server_name));
      return(False);
    }
  /*
    Set our forgiving error handler.
  */
  XSetErrorHandler(XError);
  /*
    Get user defaults from X resource database.
  */
  XrmInitialize();
  XGetDefault(display,client_name,"dummy");
  resource_database=XrmGetDatabase(display);
  resource_value=XResourceManagerString(display);
  if (resource_value == (char *) NULL)
    resource_value="";
  server_database=XrmGetStringDatabase(resource_value);
  XrmMergeDatabases(server_database,&resource_database);
  XGetResourceInfo(resource_database,client_name,&resource_info);
  /*
    Allocate standard colormap.
  */
  map_info=XAllocStandardColormap();
  if (map_info == (XStandardColormap *) NULL)
    Warning("Unable to create standard colormap","Memory allocation failed");
  else
    {
      /*
        Initialize visual info.
      */
      visual_info=XBestVisualInfo(display,map_info,&resource_info);
      if (visual_info == (XVisualInfo *) NULL)
        Warning("Unable to get visual",resource_info.visual_type);
      map_info->colormap=(Colormap) NULL;
      pixel_info.pixels=(unsigned long *) NULL;
    }
  /*
    Initialize atoms.
  */
  wm_protocols=XInternAtom(display,"WM_PROTOCOLS",False);
  wm_delete_window=XInternAtom(display,"WM_DELETE_WINDOW",False);
  if ((wm_protocols == (Atom) NULL) || (wm_delete_window == (Atom) NULL))
    Warning("Unable to create property",(char *) NULL);
  if ((map_info == (XStandardColormap *) NULL) ||
      (visual_info == (XVisualInfo *) NULL) ||
      (wm_protocols == (Atom) NULL) || (wm_delete_window == (Atom) NULL))
    {
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return(False);
    }
  /*
    Initialize Standard Colormap.
  */
  XMakeStandardColormap(display,visual_info,&resource_info,image,map_info,
    &pixel_info);
  pixel_info.annotate_context=(GC) NULL;
  pixel_info.highlight_context=(GC) NULL;
  pixel_info.widget_context=(GC) NULL;
  /*
    Initialize window info structure.
  */
  window_info.id=(Window) NULL;
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    &resource_info,&window_info);
  p=image->filename+strlen(image->filename)-1;
  while ((p > image->filename) && (*(p-1) != '/'))
    p--;
  window_info.name=p;
  window_info.width=image->columns;
  window_info.height=image->rows;
  window_info.attributes.event_mask=ButtonPressMask | ExposureMask;
  XMakeWindow(display,XRootWindow(display,visual_info->screen),(char **) NULL,0,
    (XClassHint *) NULL,(XWMHints *) NULL,&window_info);
  /*
    Initialize graphic context.
  */
  window_info.annotate_context=
    XCreateGC(display,window_info.id,0,(XGCValues *) NULL);
  if (window_info.annotate_context == (GC) NULL)
    Warning("Unable to create property",(char *) NULL);
  /*
    Initialize X image.
  */
  status=XMakeImage(display,&resource_info,&window_info,image,image->columns,
    image->rows);
  if (status == False)
    Warning("Unable to make X image",(char *) NULL);
  if ((status == False) || (window_info.annotate_context == (GC) NULL))
    {
      XFreeResources(display,visual_info,map_info,&pixel_info,
        (XFontStruct *) NULL,&resource_info,&window_info);
      return(False);
    }
  /*
    Display image and wait for button press to exit.
  */
  XMapWindow(display,window_info.id);
  for ( ; ; )
  {
    XNextEvent(display,&event);
    if (event.type == ButtonPress)
      break;
    if (event.type == ClientMessage)
      if (event.xclient.message_type == wm_protocols)
        if (*event.xclient.data.l == wm_delete_window)
          if (event.xclient.window == window_info.id)
            break;
    if (event.type == Expose)
      XRefreshWindow(display,&window_info,&event);
  }
  XWithdrawWindow(display,window_info.id,window_info.screen);
  /*
    Free X resources.
  */
  XFreeResources(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    &resource_info,&window_info);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e X B M I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure WriteXBMImage writes an image to a file in the X bitmap format.
%
%  The format of the WriteXBMImage routine is:
%
%      status=WriteXBMImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteXBMImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteXBMImage(image)
Image
  *image;
{
  char
    name[MaxTextLength];

  register int
    i,
    j,
    x;

  register char
    *q;

  register RunlengthPacket
    *p;

  register unsigned char
    bit,
    byte,
    polarity;

  unsigned int
    count;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Write X bitmap header.
  */
  (void) strcpy(name,image->filename);
  q=name;
  while ((*q != '.') && (*q != '\0'))
    q++;
  if (*q == '.')
    *q='\0';
  (void) fprintf(image->file,"#define %s_width %u\n",name,image->columns);
  (void) fprintf(image->file,"#define %s_height %u\n",name,image->rows);
  (void) fprintf(image->file,"static char %s_bits[] = {\n",name);
  (void) fprintf(image->file," ");
  /*
    Convert MIFF to X bitmap pixels.
  */
  QuantizeImage(image,2,8,False,GRAYColorspace,True);
  SyncImage(image);
  polarity=0;
  if (image->colors == 2)
    polarity=Intensity(image->colormap[0]) > Intensity(image->colormap[1]);
  bit=0;
  byte=0;
  count=0;
  x=0;
  p=image->pixels;
  (void) fprintf(image->file," ");
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
    {
      byte>>=1;
      if (p->index == polarity)
        byte|=0x80;
      bit++;
      if (bit == 8)
        {
          /*
            Write a bitmap byte to the image file.
          */
          (void) fprintf(image->file,"0x%02x, ",byte & 0xff);
          count++;
          if (count == 12)
            {
              (void) fprintf(image->file,"\n  ");
              count=0;
            };
          bit=0;
          byte=0;
        }
      x++;
      if (x == image->columns)
        {
          if (bit != 0)
            {
              /*
                Write a bitmap byte to the image file.
              */
              byte>>=(8-bit);
              (void) fprintf(image->file,"0x%02x, ",byte & 0xff);
              count++;
              if (count == 12)
                {
                  (void) fprintf(image->file,"\n  ");
                  count=0;
                };
              bit=0;
              byte=0;
            };
          x=0;
        }
    }
    p++;
  }
  (void) fprintf(image->file,"};\n");
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e X C I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteXCImage writes an image in the X constant image format.
%
%  The format of the WriteXCImage routine is:
%
%      status=WriteXCImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteXCImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteXCImage(image)
Image
  *image;
{
  unsigned int
    status;

  Warning("Cannot write XC images",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e X P M I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure WriteXPMImage writes an image to a file in the X pixmap format.
%
%  The format of the WriteXPMImage routine is:
%
%      status=WriteXPMImage(image,server_name)
%
%  A description of each parameter follows.
%
%    o status: Function WriteXPMImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%    o server_name:  A pointer to a character string representing the
%      X server to display the image.
%
%
*/
#ifdef HasXPM
#include "xpm.h"
static unsigned int WriteXPMImage(image,server_name)
Image
  *image;

char
  *server_name;
{
  char
    *resource_value,
    *xpm_buffer;

  Display
    *display;

  register char
    *p;

  unsigned int
    status;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  XImage
    *matte_image;

  XPixelInfo
    pixel_info;

  XpmAttributes
    xpm_attributes;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  XWindowInfo
    window_info;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Open X server connection.
  */
  display=XOpenDisplay(server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",XDisplayName(server_name));
      return(False);
    }
  /*
    Set our forgiving error handler.
  */
  XSetErrorHandler(XError);
  /*
    Get user defaults from X resource database.
  */
  XrmInitialize();
  XGetDefault(display,client_name,"dummy");
  resource_database=XrmGetDatabase(display);
  resource_value=XResourceManagerString(display);
  if (resource_value == (char *) NULL)
    resource_value="";
  server_database=XrmGetStringDatabase(resource_value);
  XrmMergeDatabases(server_database,&resource_database);
  XGetResourceInfo(resource_database,client_name,&resource_info);
  resource_info.colormap=PrivateColormap;
  /*
    Allocate standard colormap.
  */
  map_info=XAllocStandardColormap();
  if (map_info == (XStandardColormap *) NULL)
    Warning("Unable to create standard colormap","Memory allocation failed");
  else
    {
      /*
        Initialize visual info.
      */
      visual_info=XBestVisualInfo(display,map_info,&resource_info);
      if (visual_info == (XVisualInfo *) NULL)
        Warning("Unable to get visual",resource_info.visual_type);
      map_info->colormap=(Colormap) NULL;
      pixel_info.pixels=(unsigned long *) NULL;
    }
  if ((map_info == (XStandardColormap *) NULL) ||
      (visual_info == (XVisualInfo *) NULL))
    {
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return(False);
    }
  matte_image=(XImage *) NULL;
  if (image->matte)
    {
      /*
        Create X shape image.
      */
      matte_image=XCreateImage(display,visual_info->visual,1,XYBitmap,0,
        (char *) NULL,image->columns,image->rows,XBitmapPad(display),0);
      if (matte_image == (XImage *) NULL)
        Warning("Unable to create matte image",(char *) NULL);
      else
        {
          /*
            Allocate X shape image data.
          */
          matte_image->data=(char *)
            malloc(matte_image->bytes_per_line*matte_image->height);
          if (matte_image->data == (char *) NULL)
            Warning("Unable to create matte image","Memory allocation failed");
          else
            {
              int
                x,
                y;

              register int
                i,
                j;

              register RunlengthPacket
                *p;

              /*
                Initialize X shape image.
              */
              x=0;
              y=0;
              p=image->pixels;
              for (i=0; i < image->packets; i++)
              {
                for (j=0; j <= (int) p->length; j++)
                {
                  XPutPixel(matte_image,x,y,p->index ? False : True);
                  x++;
                  if (x == image->columns)
                    {
                      x=0;
                      y++;
                    }
                }
                p++;
              }
            }
        }
    }
  /*
    Initialize Standard Colormap.
  */
  if ((image->class == DirectClass) || (image->colors > 256))
    {
      /*
        Demote DirectClass to PseudoClass.
      */
      QuantizeImage(image,256,8,True,RGBColorspace,True);
      SyncImage(image);
    }
  XMakeStandardColormap(display,visual_info,&resource_info,image,map_info,
    &pixel_info);
  pixel_info.annotate_context=(GC) NULL;
  pixel_info.highlight_context=(GC) NULL;
  pixel_info.widget_context=(GC) NULL;
  /*
    Initialize window info structure.
  */
  window_info.id=(Window) NULL;
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    &resource_info,&window_info);
  p=image->filename+strlen(image->filename)-1;
  while ((p > image->filename) && (*(p-1) != '/'))
    p--;
  window_info.name=p;
  window_info.width=image->columns;
  window_info.height=image->rows;
  XMakeWindow(display,XRootWindow(display,visual_info->screen),(char **) NULL,0,
    (XClassHint *) NULL,(XWMHints *) NULL,&window_info);
  /*
    Initialize X image.
  */
  status=XMakeImage(display,&resource_info,&window_info,image,image->columns,
    image->rows);
  if (status == False)
    {
      Warning("Unable to make X image",(char *) NULL);
      XFreeResources(display,visual_info,map_info,&pixel_info,
        (XFontStruct *) NULL,&resource_info,&window_info);
      return(False);
    }
  /*
    Intialize XPM attributes.
  */
  xpm_attributes.valuemask=
    XpmColormap | XpmDepth | XpmRgbFilename | XpmSize | XpmVisual;
  xpm_attributes.visual=visual_info->visual;
  xpm_attributes.depth=visual_info->depth;
  xpm_attributes.colormap=map_info->colormap;
  xpm_attributes.rgb_fname=RGBColorDatabase;
  xpm_attributes.width=image->columns;
  xpm_attributes.height=image->rows;
  status=XpmCreateBufferFromImage(display,&xpm_buffer,window_info.ximage,
    matte_image,&xpm_attributes);
  /*
    Free X resources.
  */
  if (matte_image != (XImage *) NULL)
    XDestroyImage(matte_image); 
  XpmFreeAttributes(&xpm_attributes);
  XFreeResources(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    &resource_info,&window_info);
  if (status != XpmSuccess)
    {
      Warning("Unable to write image",(char *) NULL);
      return(False);
    }
  /*
    Write XPM image.
  */
  status=fputs(xpm_buffer,image->file);
  (void) free((char *) xpm_buffer);
  CloseImage(image);
  return(status != EOF);
}
#else
static unsigned int WriteXPMImage(image,server_name)
Image
  *image;

char
  *server_name;
{
  unsigned int
    status;

  Warning("XPM library is not available",image->filename);
  status=WriteMIFFImage(image);
  return(status);
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e Y U V I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteYUVImage writes an image to a file in the digital YUV
%  (CCIR 601 1:1:1) format.
%
%  The format of the WriteYUVImage routine is:
%
%      status=WriteYUVImage(image,interlace)
%
%  A description of each parameter follows.
%
%    o status: Function WriteYUVImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%    o interlace:  An unsigned integer that specifies the interlacing
%      scheme.
%
%
*/
static unsigned int WriteYUVImage(image,interlace)
Image
  *image;

unsigned int
  interlace;
{
  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *yuv_pixels;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Convert MIFF to YUV raster pixels.
  */
  yuv_pixels=(unsigned char *)
    malloc(3*image->columns*image->rows*sizeof(unsigned char));
  if (yuv_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  RGBTransformImage(image,YCbCrColorspace);
  q=yuv_pixels;
  switch (interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  YUVYUVYUVYUVYUVYUV...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
        {
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
        }
        p++;
      }
      break;
    }
    case LineInterlace:
    {
      register int
        x,
        y;

      /*
        Line interlacing:  YYY...UUU...VVV...YYY...UUU...VVV...
      */
      if (!UncompressImage(image))
        return(False);
      for (y=0; y < image->rows; y++)
      {
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->red;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->green;
          p++;
        }
        p=image->pixels+(y*image->columns);
        for (x=0; x < image->columns; x++)
        {
          *q++=p->blue;
          p++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  YYYYYY...UUUUUU...VVVVVV...
      */
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->red;
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->green;
        p++;
      }
      p=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        for (j=0; j <= ((int) p->length); j++)
          *q++=p->blue;
        p++;
      }
      break;
    }
  }
  (void) fwrite((char *) yuv_pixels,3,(int) (image->columns*image->rows),
    image->file);
  (void) free((char *) yuv_pixels);
  TransformRGBImage(image,YCbCrColorspace);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e Y U V 3 I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteYUV3Image writes an image to a file in the digital YUV
%  (CCIR 601 2:1:1) format.
%
%  The format of the WriteYUV3Image routine is:
%
%      status=WriteYUV3Image(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteYUV3Image return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteYUV3Image(image)
Image
  *image;
{
  char
    filename[MaxTextLength];

  Image
    *scaled_image;

  register int
    i;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *yuv_pixels;

  if (!UncompressImage(image))
    return(False);
  /*
    Open output image file.
  */
  (void) strcpy(filename,image->filename);
  if (strcmp(image->filename,"-") != 0)
    (void) strcat(image->filename,".Y");
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  /*
    Write Y channel.
  */
  yuv_pixels=(unsigned char *)
    malloc(image->columns*image->rows*sizeof(unsigned char));
  if (yuv_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  RGBTransformImage(image,YCbCrColorspace);
  p=image->pixels;
  q=yuv_pixels;
  for (i=0; i < (image->rows*image->columns); i++)
  {
    *q=p->red;
    p++;
    q++;
  }
  (void) fwrite((char *) yuv_pixels,1,(int) (image->columns*image->rows),
    image->file);
  CloseImage(image);
  /*
    Scale image.
  */
  image->orphan=True;
  scaled_image=ScaleImage(image,image->columns >> 1,image->rows >> 1);
  image->orphan=False;
  if (scaled_image == (Image *) NULL)
    {
      Warning("Unable to scale image",image->filename);
      return(False);
    }
  /*
    Write U channel.
  */
  (void) strcpy(scaled_image->filename,filename);
  if (strcmp(scaled_image->filename,"-") != 0)
    (void) strcat(scaled_image->filename,".U");
  OpenImage(scaled_image,"wb");
  if (scaled_image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",scaled_image->filename);
      DestroyImage(scaled_image);
      return(False);
    }
  p=scaled_image->pixels;
  q=yuv_pixels;
  for (i=0; i < (scaled_image->rows*scaled_image->columns); i++)
  {
    *q=p->green;
    p++;
    q++;
  }
  (void) fwrite((char *) yuv_pixels,1,(int)
    (scaled_image->columns*scaled_image->rows),scaled_image->file);
  CloseImage(scaled_image);
  /*
    Write V channel.
  */
  (void) strcpy(scaled_image->filename,filename);
  if (strcmp(scaled_image->filename,"-") != 0)
    (void) strcat(scaled_image->filename,".V");
  OpenImage(scaled_image,"wb");
  if (scaled_image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",scaled_image->filename);
      DestroyImage(scaled_image);
      return(False);
    }
  p=scaled_image->pixels;
  q=yuv_pixels;
  for (i=0; i < (scaled_image->rows*scaled_image->columns); i++)
  {
    *q=p->blue;
    p++;
    q++;
  }
  (void) fwrite((char *) yuv_pixels,1,(int)
    (scaled_image->columns*scaled_image->rows),scaled_image->file);
  CloseImage(scaled_image);
  DestroyImage(scaled_image);
  (void) free((char *) yuv_pixels);
  TransformRGBImage(image,YCbCrColorspace);
  (void) sprintf(image->filename,filename);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e X W D I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteXWDImage writes an image to a file in X window dump
%  rasterfile format.
%
%  The format of the WriteXWDImage routine is:
%
%      status=WriteXWDImage(image)
%
%  A description of each parameter follows.
%
%    o status: Function WriteXWDImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image:  A pointer to a Image structure.
%
%
*/
static unsigned int WriteXWDImage(image)
Image
  *image;
{
  int
    x;

  register int
    i,
    j;

  register RunlengthPacket
    *p;

  register unsigned char
    *q;

  unsigned char
    *xwd_pixels;

  unsigned int
    bits_per_pixel,
    bytes_per_line,
    scanline_pad;

  unsigned long
    lsb_first,
    packets;

  XWDFileHeader
    xwd_header;

  /*
    Open output image file.
  */
  OpenImage(image,"wb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      return(False);
    }
  if (image->class == DirectClass)
    if (NumberColors(image,(FILE *) NULL) <= 256)
      {
        /*
          Demote DirectClass to PseudoClass.
        */
        QuantizeImage(image,256,8,False,RGBColorspace,True);
        SyncImage(image);
      }
  /*
    Initialize XWD file header.
  */
  xwd_header.header_size=sz_XWDheader+strlen(image->filename)+1;
  xwd_header.file_version=(unsigned long) XWD_FILE_VERSION;
  xwd_header.pixmap_format=(unsigned long) ZPixmap;
  xwd_header.pixmap_depth=(unsigned long)
    (image->class == DirectClass ? 24 : 8);
  xwd_header.pixmap_width=(unsigned long) image->columns;
  xwd_header.pixmap_height=(unsigned long) image->rows;
  xwd_header.xoffset=(unsigned long) 0;
  xwd_header.byte_order=(unsigned long) MSBFirst;
  xwd_header.bitmap_unit=(unsigned long) (image->class == DirectClass ? 32 : 8);
  xwd_header.bitmap_bit_order=(unsigned long) MSBFirst;
  xwd_header.bitmap_pad=(unsigned long) (image->class == DirectClass ? 32 : 8);
  bits_per_pixel=(image->class == DirectClass ? 24 : 8);
  xwd_header.bits_per_pixel=(unsigned long) bits_per_pixel;
  bytes_per_line=(unsigned long) ((((xwd_header.bits_per_pixel*
    xwd_header.pixmap_width)+((xwd_header.bitmap_pad)-1))/
    (xwd_header.bitmap_pad))*((xwd_header.bitmap_pad) >> 3));
  xwd_header.bytes_per_line=(unsigned long) bytes_per_line;
  xwd_header.visual_class=(unsigned long)
    (image->class == DirectClass ? DirectColor : PseudoColor);
  xwd_header.red_mask=(unsigned long)
    (image->class == DirectClass ? 0xff0000 : 0);
  xwd_header.green_mask=(unsigned long)
    (image->class == DirectClass ? 0xff00 : 0);
  xwd_header.blue_mask=(unsigned long) (image->class == DirectClass ? 0xff : 0);
  xwd_header.bits_per_rgb=(unsigned long)
    (image->class == DirectClass ? 24 : 8);
  xwd_header.colormap_entries=(unsigned long)
    (image->class == DirectClass ? 256 : image->colors);
  xwd_header.ncolors=(image->class == DirectClass ? 0 : image->colors);
  xwd_header.window_width=(unsigned long) image->columns;
  xwd_header.window_height=(unsigned long) image->rows;
  xwd_header.window_x=0;
  xwd_header.window_y=0;
  xwd_header.window_bdrwidth=(unsigned long) 0;
  /*
    Write XWD header.
  */
  lsb_first=1;
  if (*(char *) &lsb_first)
    MSBFirstOrderLong((char *) &xwd_header,sizeof(xwd_header));
  (void) fwrite((char *) &xwd_header,sz_XWDheader,1,image->file);
  (void) fwrite((char *) image->filename,1,strlen(image->filename)+1,
    image->file);
  if (image->class == PseudoClass)
    {
      XColor
        *colors;

      XWDColor
        color;

      /*
        Dump colormap to file.
      */
      colors=(XColor *) malloc(image->colors*sizeof(XColor));
      if (colors == (XColor *) NULL)
        {
          Warning("Unable to allocate memory",(char *) NULL);
          return(False);
        }
      for (i=0; i < image->colors; i++)
      {
        colors[i].pixel=i;
        colors[i].red=image->colormap[i].red << 8;
        colors[i].green=image->colormap[i].green << 8;
        colors[i].blue=image->colormap[i].blue << 8;
        colors[i].flags=DoRed | DoGreen | DoBlue;
        colors[i].pad=0;
        if (*(char *) &lsb_first)
          {
            MSBFirstOrderLong((char *) &colors[i].pixel,sizeof(long));
            MSBFirstOrderShort((char *) &colors[i].red,3*sizeof(short));
          }
      }
      for (i=0; i < image->colors; i++)
      {
        color.pixel=colors[i].pixel;
        color.red=colors[i].red;
        color.green=colors[i].green;
        color.blue=colors[i].blue;
        color.flags=colors[i].flags;
        (void) fwrite((char *) &color,sz_XWDColor,1,image->file);
      }
      (void) free((char *) colors);
    }
  /*
    Convert MIFF to XWD raster pixels.
  */
  packets=bytes_per_line*image->rows;
  xwd_pixels=(unsigned char *)
    malloc((unsigned int) packets*sizeof(unsigned char));
  if (xwd_pixels == (unsigned char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      return(False);
    }
  scanline_pad=(unsigned int)
    (bytes_per_line-((image->columns*bits_per_pixel) >> 3));
  x=0;
  p=image->pixels;
  q=xwd_pixels;
  for (i=0; i < image->packets; i++)
  {
    for (j=0; j <= ((int) p->length); j++)
    {
      if (image->class == PseudoClass)
        *q++=p->index;
      else
        {
          *q++=p->red;
          *q++=p->green;
          *q++=p->blue;
        }
      x++;
      if (x == image->columns)
        {
          q+=scanline_pad;
          x=0;
        }
    }
    p++;
  }
  /*
    Dump pixels to file.
  */
  (void) fwrite((char *) xwd_pixels,1,(int) packets,image->file);
  (void) free((char *) xwd_pixels);
  CloseImage(image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e I m a g e                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function WriteImage writes an image to a file.  You can specify a
%  particular image format by prefixing the file with the image type and a
%  colon (i.e. ps:image) or specify the image type as the filename suffix
%  (i.e. image.ps).
%
%  The format of the WriteImage routine is:
%
%      status=WriteImage(image_info,image)
%
%  A description of each parameter follows:
%
%    o status: Function WriteImage return True if the image is written.
%      False is returned is there is a memory shortage or if the image file
%      fails to write.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%    o image: A pointer to a Image structure.
%
%
*/
unsigned int WriteImage(image_info,image)
ImageInfo
  *image_info;

Image
  *image;
{
  unsigned int
    status;

  /*
    Call appropriate image writer based on image type.
  */
  (void) strcpy(image_info->filename,image->filename);
  (void) strcpy(image_info->magick,image->magick);
  SetImageMagick(image_info);
  (void) strcpy(image->filename,image_info->filename);
  (void) strcpy(image->magick,image_info->magick);
  switch (*image_info->magick)
  {
    case 'A':
    {
      status=WriteAVSImage(image);
      break;
    }
    case 'B':
    {
      status=WriteBMPImage(image);
      break;
    }
    case 'C':
    {
      status=WriteCMYKImage(image,image_info->interlace,image_info->undercolor);
      break;
    }
    case 'E':
    {
      status=WritePSImage(image,image_info->page,image_info->density);
      break;
    }
    case 'F':
    {
      if (strcmp(image_info->magick,"FAX") == 0)
        status=WriteFAXImage(image);
      else
        status=WriteFITSImage(image);
      break;
    }
    case 'G':
    {
      if (strcmp(image_info->magick,"GIF") == 0)
        status=WriteGIFImage(image);
      else
        if (strcmp(image_info->magick,"GIF87") == 0)
          status=WriteGIFImage(image);
        else
          if (strcmp(image_info->magick,"GRAY") == 0)
            status=WriteGRAYImage(image);
          else
            status=WriteFAXImage(image);
      break;
    }
    case 'H':
    {
      status=WriteHISTOGRAMImage(image);
      break;
    }
    case 'I':
    {
      status=WriteIRISImage(image);
      break;
    }
    case 'J':
    {
      status=WriteJPEGImage(image,image_info->quality);
      break;
    }
    case 'M':
    {
      if (strcmp(image_info->magick,"MAP") == 0)
        status=WriteMAPImage(image);
      else
        if (strcmp(image_info->magick,"MATTE") == 0)
          status=WriteMATTEImage(image);
        else
          if (strcmp(image_info->magick,"MIFF") == 0)
            status=WriteMIFFImage(image);
          else
            status=WriteMTVImage(image);
      break;
    }
    case 'P':
    {
      if (strcmp(image_info->magick,"PCD") == 0)
        status=WritePCDImage(image);
      else
        if (strcmp(image_info->magick,"PCX") == 0)
          status=WritePCXImage(image);
        else
          if (strcmp(image_info->magick,"PICT") == 0)
            status=WritePICTImage(image);
          else
            if (strcmp(image_info->magick,"PM") == 0)
              status=WriteXPMImage(image,image_info->server_name);
            else
              if (strcmp(image_info->magick,"PS") == 0)
                status=WritePSImage(image,image_info->page,image_info->density);
              else
                if (strcmp(image_info->magick,"PS2") == 0)
                  status=
                    WritePS2Image(image,image_info->page,image_info->density);
                else
                  status=WritePNMImage(image);
      break;
    }
    case 'R':
    {
      if (strcmp(image_info->magick,"RAS") == 0)
        status=WriteSUNImage(image);
      else
        if (strcmp(image_info->magick,"RGB") == 0)
          status=WriteRGBImage(image,image_info->interlace);
        else
          status=WriteRLEImage(image);
      break;
    }
    case 'S':
    {
      status=WriteSUNImage(image);
      break;
    }
    case 'T':
    {
      if (strcmp(image_info->magick,"TGA") == 0)
        status=WriteTARGAImage(image);
      else
        if ((strcmp(image_info->magick,"TIF") == 0) ||
            (strcmp(image_info->magick,"TIFF") == 0))
          status=WriteTIFFImage(image,image_info->verbose);
        else
          status=WriteTEXTImage(image);
      break;
    }
    case 'V':
    {
      if (strcmp(image_info->magick,"VICAR") == 0)
        status=WriteVICARImage(image);
      else
        status=WriteVIFFImage(image);
      break;
    }
    case 'X':
    {
      if (strcmp(image_info->magick,"X") == 0)
        status=WriteXImage(image,image_info->server_name);
      else
        if (strcmp(image_info->magick,"XBM") == 0)
          status=WriteXBMImage(image);
        else
          if (strcmp(image_info->magick,"XC") == 0)
            status=WriteXCImage(image);
          else
            if (strcmp(image_info->magick,"XPM") == 0)
              status=WriteXPMImage(image,image_info->server_name);
            else
              if (strcmp(image_info->magick,"XV") == 0)
                status=WriteVIFFImage(image);
              else
                status=WriteXWDImage(image);
      break;
    }
    case 'Y':
    {
      if (strcmp(image_info->magick,"YUV") == 0)
        status=WriteYUVImage(image,image_info->interlace);
      else
        status=WriteYUV3Image(image);
      break;
    }
    default:
      status=WriteMIFFImage(image);
  }
  if (image->status)
    {
      Warning("An error has occurred writing to file",image->filename);
      return(False);
    }
  return(status);
}
