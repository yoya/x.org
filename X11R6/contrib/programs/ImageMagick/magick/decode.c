/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                   DDDD   EEEEE   CCCC   OOO   DDDD   EEEEE                  %
%                   D   D  E      C      O   O  D   D  E                      %
%                   D   D  EEE    C      O   O  D   D  EEE                    %
%                   D   D  E      C      O   O  D   D  E                      %
%                   DDDD   EEEEE   CCCC   OOO   DDDD   EEEEE                  %
%                                                                             %
%                                                                             %
%                    Utility Routines to Read Image Formats                   %
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
#include "compress.h"
#include "utility.h"
#include "X.h"
#include "XWDFile.h"

/*
  Forward declarations.
*/
static Image
  *ReadMIFFImage _Declare((ImageInfo *)),
  *ReadPNMImage _Declare((ImageInfo *));

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d A V S I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadAVSImage reads a AVS X image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadAVSImage routine is:
%
%      image=ReadAVSImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadAVSImage returns a pointer to the image after
%      reading. A null image is returned if there is a a memory shortage or if
%      the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadAVSImage(image_info)
ImageInfo
  *image_info;
{
  typedef struct _AVSHeader
  {
    int
      width,
      height;
  } AVSHeader;

  AVSHeader
    avs_header;

  Image
    *image;

  register int
    i;

  register unsigned char
    *p;

  register RunlengthPacket
    *q;

  unsigned char
    *avs_pixels;

  unsigned int
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read AVS image.
  */
  status=ReadData((char *) &avs_header,1,sizeof(AVSHeader),image->file);
  if (status == False)
    {
      Warning("Not a AVS image file",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  do
  {
    /*
      Initialize image structure.
    */
    image->matte=True;
    image->columns=avs_header.width;
    image->rows=avs_header.height;
    image->packets=image->columns*image->rows;
    avs_pixels=(unsigned char *) malloc(4*image->packets*sizeof(unsigned char));
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if ((avs_pixels == (unsigned char *) NULL) ||
        (image->pixels == (RunlengthPacket *) NULL))
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert AVS raster image to runlength-encoded packets.
    */
    (void) ReadData((char *) avs_pixels,4,(int) image->packets,image->file);
    p=avs_pixels;
    q=image->pixels;
    for (i=0; i < image->packets; i++)
    {
      q->index=(unsigned char) (*p++);
      q->red=(*p++);
      q->green=(*p++);
      q->blue=(*p++);
      q->length=0;
      q++;
    }
    (void) free((char *) avs_pixels);
    status=ReadData((char *) &avs_header,1,sizeof(AVSHeader),image->file);
    if (status == True)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (status == True);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d B M P I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadBMPImage reads a Microsoft Windows bitmap image file and
%  returns it.  It allocates the memory necessary for the new Image structure
%  and returns a pointer to the new image.
%
%  The format of the ReadBMPImage routine is:
%
%      image=ReadBMPImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadBMPImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadBMPImage(image_info)
ImageInfo
  *image_info;
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

  Image
    *image;

  register int
    bit,
    i,
    x,
    y;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *bmp_data,
    *bmp_pixels,
    magick[12];

  unsigned int
    bytes_per_line,
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine if this is a BMP file.
  */
  status=ReadData((char *) magick,1,2,image->file);
  do
  {
    /*
      Verify BMP identifier.
    */
    if ((status == False) || (strncmp((char *) magick,"BM",2) != 0))
      {
        Warning("Not a BMP image file",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    bmp_header.file_size=LSBFirstReadLong(image->file);
    bmp_header.reserved[0]=LSBFirstReadShort(image->file);
    bmp_header.reserved[1]=LSBFirstReadShort(image->file);
    bmp_header.offset_bits=LSBFirstReadLong(image->file);
    bmp_header.size=LSBFirstReadLong(image->file);
    if (bmp_header.size == 12)
      {
        /*
          OS/2 BMP image file.
        */
        bmp_header.width=(unsigned long) LSBFirstReadShort(image->file);
        bmp_header.height=(unsigned long) LSBFirstReadShort(image->file);
        bmp_header.planes=LSBFirstReadShort(image->file);
        bmp_header.bit_count=LSBFirstReadShort(image->file);
        bmp_header.number_colors=0;
        bmp_header.compression=0;
        bmp_header.image_size=0;
      }
    else
      {
        /*
          Microsoft Windows BMP image file.
        */
        bmp_header.width=LSBFirstReadLong(image->file);
        bmp_header.height=LSBFirstReadLong(image->file);
        bmp_header.planes=LSBFirstReadShort(image->file);
        bmp_header.bit_count=LSBFirstReadShort(image->file);
        bmp_header.compression=LSBFirstReadLong(image->file);
        bmp_header.image_size=LSBFirstReadLong(image->file);
        bmp_header.x_pixels=LSBFirstReadLong(image->file);
        bmp_header.y_pixels=LSBFirstReadLong(image->file);
        bmp_header.number_colors=LSBFirstReadLong(image->file);
        bmp_header.colors_important=LSBFirstReadLong(image->file);
        for (i=0; i < ((int) bmp_header.size-40); i++)
          (void) fgetc(image->file);
      }
    if (bmp_header.bit_count < 24)
      {
        unsigned char
          *bmp_colormap;

        unsigned int
          packet_size;

        /*
          Read BMP raster colormap.
        */
        image->class=PseudoClass;
        image->colors=1 << bmp_header.bit_count;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        bmp_colormap=(unsigned char *)
          malloc(4*image->colors*sizeof(unsigned char));
        if ((image->colormap == (ColorPacket *) NULL) ||
            (bmp_colormap == (unsigned char *) NULL))
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        packet_size=4;
        if (bmp_header.size == 12)
          packet_size=3;
        (void) ReadData((char *) bmp_colormap,(int) packet_size,
          (int) image->colors,image->file);
        p=bmp_colormap;
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].blue=(*p++);
          image->colormap[i].green=(*p++);
          image->colormap[i].red=(*p++);
          if (bmp_header.size != 12)
            p++;
        }
        (void) free((char *) bmp_colormap);
      }
    /*
      Read image data.
    */
    if (bmp_header.image_size == 0)
      bmp_header.image_size=
        ((bmp_header.width*bmp_header.bit_count+31)/32)*4*bmp_header.height;
    bmp_data=(unsigned char *)
      malloc(bmp_header.image_size*sizeof(unsigned char));
    if (bmp_data == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    status=ReadData((char *) bmp_data,1,(int) bmp_header.image_size,
      image->file);
    if (status == False)
      {
        Warning("Unable to read image data",image_info->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    bmp_pixels=bmp_data;
    if (bmp_header.compression != 0)
      {
        unsigned int
          packets;

        /*
          Convert run-length encoded raster pixels.
        */
        packets=
          ((bmp_header.width*bmp_header.bit_count+31)/32)*4*bmp_header.height;
        if (bmp_header.compression == 2)
          packets<<=1;
        bmp_pixels=(unsigned char *) malloc(packets*sizeof(unsigned char));
        if (bmp_pixels == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) BMPDecodeImage(bmp_data,bmp_pixels,
          (unsigned int) bmp_header.compression,(unsigned int) bmp_header.width,
          (unsigned int) bmp_header.height);
        if (bmp_header.compression == 2)
          bmp_header.bit_count<<=1;
        (void) free((char *) bmp_data);
      }
    /*
      Initialize image structure.
    */
    image->columns=bmp_header.width;
    image->rows=bmp_header.height;
    image->packets=image->columns*image->rows;
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert BMP raster image to runlength-encoded packets.
    */
    bytes_per_line=((image->columns*bmp_header.bit_count+31)/32)*4;
    switch (bmp_header.bit_count)
    {
      case 1:
      {
        /*
          Convert bitmap scanline to runlength-encoded color packets.
        */
        for (y=image->rows-1; y >= 0; y--)
        {
          p=bmp_pixels+(image->rows-y-1)*bytes_per_line;
          q=image->pixels+(y*image->columns);
          for (x=0; x < (image->columns-7); x+=8)
          {
            for (bit=0; bit < 8; bit++)
            {
              q->index=((*p) & (0x80 >> bit) ? 0x00 : 0x01);
              q->length=0;
              q++;
            }
            p++;
          }
          if ((image->columns % 8) != 0)
            {
              for (bit=0; bit < (8-(image->columns % 8)); bit++)
              {
                q->index=((*p) & (0x80 >> bit) ? 0x00 : 0x01);
                q->length=0;
                q++;
              }
              p++;
            }
        }
        SyncImage(image);
        break;
      }
      case 4:
      {
        /*
          Convert PseudoColor scanline to runlength-encoded color packets.
        */
        for (y=image->rows-1; y >= 0; y--)
        {
          p=bmp_pixels+(image->rows-y-1)*bytes_per_line;
          q=image->pixels+(y*image->columns);
          for (x=0; x < (image->columns-1); x+=2)
          {
            q->index=(*p >> 4) & 0xf;
            q->length=0;
            q++;
            q->index=(*p) & 0xf;
            q->length=0;
            p++;
            q++;
          }
          if ((image->columns % 2) != 0)
            {
              q->index=(*p >> 4) & 0xf;
              q->length=0;
              q++;
              p++;
            }
        }
        SyncImage(image);
        CompressColormap(image);
        break;
      }
      case 8:
      {
        /*
          Convert PseudoColor scanline to runlength-encoded color packets.
        */
        for (y=image->rows-1; y >= 0; y--)
        {
          p=bmp_pixels+(image->rows-y-1)*bytes_per_line;
          q=image->pixels+(y*image->columns);
          for (x=0; x < image->columns; x++)
          {
            q->index=(*p++);
            q->length=0;
            q++;
          }
        }
        SyncImage(image);
        CompressColormap(image);
        break;
      }
      case 24:
      {
        /*
          Convert DirectColor scanline to runlength-encoded color packets.
        */
        for (y=image->rows-1; y >= 0; y--)
        {
          p=bmp_pixels+(image->rows-y-1)*bytes_per_line;
          q=image->pixels+(y*image->columns);
          for (x=0; x < image->columns; x++)
          {
            q->index=(unsigned short) (image->matte ? (*p++) : 0);
            q->blue=(*p++);
            q->green=(*p++);
            q->red=(*p++);
            q->length=0;
            q++;
          }
        }
        break;
      }
      default:
      {
        Warning("Not a BMP image file",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    }
    (void) free((char *) bmp_pixels);
    /*
      Proceed to next image.
    */
    status=ReadData((char *) magick,1,2,image->file);
    if ((status == True) && (strncmp((char *) magick,"BM",2) == 0))
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while ((status == True) && (strncmp((char *) magick,"BM",2) == 0));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d C M Y K I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadCMYKImage reads an image of raw cyan, magenta, yellow, and
%  black bytes and returns it.  It allocates the memory necessary for the new
%  Image structure and returns a pointer to the new image.
%
%  The format of the ReadCMYKImage routine is:
%
%      image=ReadCMYKImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadCMYKImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadCMYKImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    black,
    *cmyk_pixels,
    cyan,
    magenta,
    yellow;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize image structure.
  */
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  cmyk_pixels=(unsigned char *) malloc(4*image->packets*sizeof(unsigned char));
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if ((cmyk_pixels == (unsigned char *) NULL) ||
      (image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) cmyk_pixels,4,(int) image->packets,image->file);
  p=cmyk_pixels;
  switch (image_info->interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  CMYKCMYKCMYKCMYKCMYKCMYK...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->green=(*p++);
        q->blue=(*p++);
        q->index=(*p++);
        q->length=0;
        q++;
      }
      break;
    }
    case LineInterlace:
    {
      /*
        Line interlacing:  CCC...MMM...YYY...KKK...CCC...MMM...YYY...KKK...
      */
      for (y=0; y < image->rows; y++)
      {
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->red=(*p++);
          q->length=0;
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->green=(*p++);
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->blue=(*p++);
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->index=(*p++);
          q++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  CCCCCC...MMMMMM...YYYYYY...KKKKKK...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->length=0;
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->green=(*p++);
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->blue=(*p++);
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->index=(*p++);
        q++;
      }
      break;
    }
  }
  /*
    Transform image to CMYK.
  */
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    cyan=q->red;
    magenta=q->green;
    yellow=q->blue;
    black=q->index;
    if ((unsigned int) (cyan+black) > MaxRGB)
      q->red=0;
    else
      q->red=MaxRGB-(cyan+black);
    if ((unsigned int) (magenta+black) > MaxRGB)
      q->green=0;
    else
      q->green=MaxRGB-(magenta+black);
    if ((unsigned int) (yellow+black) > MaxRGB)
      q->blue=0;
    else
      q->blue=MaxRGB-(yellow+black);
    q->index=0;
    q->length=0;
    q++;
  }
  (void) free((char *) cmyk_pixels);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d F A X I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadFAXImage reads a Group 3 FAX image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadFAXImage routine is:
%
%      image=ReadFAXImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadFAXImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadFAXImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  unsigned int
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize image structure.
  */
  image->class=PseudoClass;
  image->columns=1728;
  image->rows=2156;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  image->colors=2;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if ((image->pixels == (RunlengthPacket *) NULL) ||
      (image->colormap == (ColorPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Monochrome colormap.
  */
  image->colormap[0].red=MaxRGB;
  image->colormap[0].green=MaxRGB;
  image->colormap[0].blue=MaxRGB;
  image->colormap[1].red=0;
  image->colormap[1].green=0;
  image->colormap[1].blue=0;
  status=HuffmanDecodeImage(image);
  if (status == False)
    {
      Warning("Unable to read image data",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  SyncImage(image);
  CompressImage(image);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d F I T S I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadFITSImage reads a FITS image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadFITSImage routine is:
%
%      image=ReadFITSImage(image_info)
%
%  A description of each parameter follows:
%
%    o image: Function ReadFITSImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or if
%      the image cannot be read.
%
%    o filename: Specifies the name of the image to read.
%
%
*/
static Image *ReadFITSImage(image_info)
ImageInfo
  *image_info;
{
  typedef struct _FITSHeader
  {
    unsigned int
      simple;

    int
      bits_per_pixel;

    unsigned int
      number_of_axis,
      columns,
      rows,
      depth;

    double
      min_data,
      max_data,
      zero,
      scale;
  } FITSHeader;

  char
    keyword[MaxTextLength],
    value[MaxTextLength];

  double
    pixel,
    scaled_pixel;

  FITSHeader
    fits_header;

  Image
    *image;

  int
    packet_size;

  long
    count,
    quantum;

  register int
    c,
    i,
    j;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *fits_pixels;

  unsigned int
    status,
    value_expected;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize image header.
  */
  fits_header.simple=False;
  fits_header.bits_per_pixel=8;
  fits_header.columns=1;
  fits_header.rows=1;
  fits_header.depth=1;
  fits_header.min_data=0.0;
  fits_header.max_data=0.0;
  fits_header.zero=0.0;
  fits_header.scale=1.0;
  /*
    Decode image header.
  */
  c=fgetc(image->file);
  count=1;
  if (c == EOF)
    {
      DestroyImage(image);
      return((Image *) NULL);
    }
  while (count < 2880)
  {
    if (!isalnum(c))
      {
        c=fgetc(image->file);
        count++;
      }
    else
      {
        register char
          *p;

        /*
          Determine a keyword and its value.
        */
        p=keyword;
        do
        {
          if ((p-keyword) < (MaxTextLength-1))
            *p++=(char) c;
          c=fgetc(image->file);
          count++;
        } while (isalnum(c) || (c == '_'));
        *p='\0';
        if (strcmp(keyword,"END") == 0)
          break;
        value_expected=False;
        while (isspace(c) || (c == '='))
        {
          if (c == '=')
            value_expected=True;
          c=fgetc(image->file);
          count++;
        }
        if (value_expected == False)
          continue;
        p=value;
        while (isalnum(c) || (c == '-') || (c == '+') || (c == '.'))
        {
          if ((p-value) < (MaxTextLength-1))
            *p++=(char) c;
          c=fgetc(image->file);
          count++;
        }
        *p='\0';
        /*
          Assign a value to the specified keyword.
        */
        if (strcmp(keyword,"SIMPLE") == 0)
          fits_header.simple=(*value == 'T') || (*value == 't');
        if (strcmp(keyword,"BITPIX") == 0)
          fits_header.bits_per_pixel=(unsigned int) atoi(value);
        if (strcmp(keyword,"NAXIS") == 0)
          fits_header.number_of_axis=(unsigned int) atoi(value);
        if (strcmp(keyword,"NAXIS1") == 0)
          fits_header.columns=(unsigned int) atoi(value);
        if (strcmp(keyword,"NAXIS2") == 0)
          fits_header.rows=(unsigned int) atoi(value);
        if (strcmp(keyword,"NAXIS3") == 0)
          fits_header.depth=(unsigned int) atoi(value);
        if (strcmp(keyword,"DATAMAX") == 0)
          fits_header.max_data=atof(value);
        if (strcmp(keyword,"DATAMIN") == 0)
          fits_header.min_data=atof(value);
        if (strcmp(keyword,"BZERO") == 0)
          fits_header.zero=atof(value);
        if (strcmp(keyword,"BSCALE") == 0)
          fits_header.scale=atof(value);
      }
    while (isspace(c) && (count < 2880))
    {
      c=fgetc(image->file);
      count++;
    }
  }
  for ( ; count < 2880; count++)
    (void) fgetc(image->file);
  /*
    Verify that required image information is defined.
  */
  if ((!fits_header.simple) || (fits_header.number_of_axis < 1) ||
      (fits_header.number_of_axis > 3) ||
      (fits_header.columns*fits_header.rows) == 0)
    {
      Warning("Unable to read FITS image","image type not supported");
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Create linear colormap.
  */
  image->columns=fits_header.columns;
  image->rows=fits_header.rows;
  image->class=PseudoClass;
  image->colors=MaxRGB+1;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if (image->colormap == (ColorPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  for (i=0; i < image->colors; i++)
  {
    image->colormap[i].red=(unsigned char) i;
    image->colormap[i].green=(unsigned char) i;
    image->colormap[i].blue=(unsigned char) i;
  }
  /*
    Initialize image structure.
  */
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  packet_size=fits_header.bits_per_pixel/8;
  if (packet_size < 0)
    packet_size=(-packet_size);
  fits_pixels=(unsigned char *)
    malloc(image->packets*packet_size*sizeof(unsigned char));
  if ((image->pixels == (RunlengthPacket *) NULL) ||
      (fits_pixels == (unsigned char *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert FITS pixels to runlength-encoded packets.
  */
  status=ReadData((char *) fits_pixels,(int) packet_size,(int) image->packets,
    image->file);
  if (status == False)
    Warning("Insufficient image data in file",image->filename);
  if ((fits_header.min_data == 0.0) && (fits_header.max_data == 0.0))
    {
      /*
        Determine minimum and maximum intensity.
      */
      p=fits_pixels;
      quantum=(*p++);
      for (j=0; j < (packet_size-1); j++)
        quantum=(quantum << 8) | (*p++);
      pixel=(double) quantum;
      if (fits_header.bits_per_pixel == -32)
        pixel=(double) (*((float *) &quantum));
      fits_header.min_data=pixel*fits_header.scale+fits_header.zero;
      fits_header.max_data=pixel*fits_header.scale+fits_header.zero;
      for (i=1; i < image->packets; i++)
      {
        quantum=(*p++);
        for (j=0; j < (packet_size-1); j++)
          quantum=(quantum << 8) | (*p++);
        pixel=(double) quantum;
        if (fits_header.bits_per_pixel == -32)
          pixel=(double) (*((float *) &quantum));
        scaled_pixel=pixel*fits_header.scale+fits_header.zero;
        if (scaled_pixel < fits_header.min_data)
          fits_header.min_data=scaled_pixel;
        if (scaled_pixel > fits_header.max_data)
          fits_header.max_data=scaled_pixel;
      }
    }
  /*
    Convert FITS pixels to runlength-encoded packets.
  */
  p=fits_pixels;
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    quantum=(*p++);
    for (j=0; j < (packet_size-1); j++)
      quantum=(quantum << 8) | (*p++);
    pixel=(double) quantum;
    if (fits_header.bits_per_pixel == -32)
      pixel=(double) (*((float *) &quantum));
    scaled_pixel=MaxRGB*(pixel*fits_header.scale+fits_header.zero+
      fits_header.min_data)/(fits_header.max_data-fits_header.min_data);
    q->index=(unsigned short) scaled_pixel;
    q->length=0;
    q++;
  }
  (void) free((char *) fits_pixels);
  SyncImage(image);
  CompressColormap(image);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d G I F I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadGIFImage reads a Compuserve Graphics image file and returns it.
%  It allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadGIFImage routine is:
%
%      image=ReadGIFImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadGIFImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      an error occurs.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadGIFImage(image_info)
ImageInfo
  *image_info;
{
#define BitSet(byte,bit)  (((byte) & (bit)) == (bit))
#define LSBFirstOrder(x,y)  (((y) << 8) | (x))

  Image
    *image;

  int
    pass,
    status,
    transparency_index,
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    c,
    *global_colormap,
    header[MaxTextLength],
    magick[12];

  unsigned int
    global_colors,
    image_count,
    interlace,
    local_colormap;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine if this is a GIF file.
  */
  status=ReadData((char *) magick,1,6,image->file);
  if ((status == False) || ((strncmp((char *) magick,"GIF87",5) != 0) &&
      (strncmp((char *) magick,"GIF89",5) != 0)))
    {
      Warning("Not a GIF image file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read the screen descriptor.
  */
  status=ReadData((char *) header,1,7,image->file);
  if (status == False)
    {
      Warning("Failed to read screen descriptor",(char *) NULL );
      DestroyImage(image);
      return((Image *) NULL);
    }
  global_colors=0;
  global_colormap=(unsigned char *) NULL;
  if (BitSet(header[4],0x80))
    {
      /*
        Read global colormap.
      */
      global_colors=1 << ((header[4] & 0x07)+1);
      global_colormap=(unsigned char *)
        malloc(3*global_colors*sizeof(unsigned char));
      if (global_colormap == (unsigned char *) NULL)
        {
          Warning("Unable to read image colormap","Memory allocation failed");
          DestroyImage(image);
          return((Image *) NULL);
        }
      (void) ReadData((char *) global_colormap,3,(int) global_colors,
        image->file);
    }
  transparency_index=(-1);
  image_count=0;
  for ( ; ; )
  {
    status=ReadData((char *) &c,1,1,image->file);
    if (status == False)
      break;
    if (c == ';')
      break;  /* terminator */
    if (c == '!')
      {
        /*
          GIF Extension block.
        */
        status=ReadData((char *) &c,1,1,image->file);
        if (status == False)
          {
            Warning("Unable to read extention block",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        switch (c)
        {
          case 0xf9:
          {
            /*
              Transparency extension block.
            */
            while (ReadDataBlock((char *) header,image->file) != 0);
            if ((header[0] & 0x01) == 1)
              transparency_index=header[3];
            break;
          }
          case 0xfe:
          {
            int
              length;

            /*
              Comment extension block.
            */
            for ( ; ; )
            {
              length=ReadDataBlock((char *) header,image->file);
              if (length == 0)
                break;
              if (image->comments != (char *) NULL)
                image->comments=(char *) realloc((char *) image->comments,
                  (strlen(image->comments)+length+1)*sizeof(char));
              else
                {
                  image->comments=(char *) malloc((length+1)*sizeof(char));
                  if (image->comments != (char *) NULL)
                    *image->comments='\0';
                }
              if (image->comments == (char *) NULL)
                {
                  Warning("Memory allocation error",(char *) NULL);
                  DestroyImages(image);
                  return((Image *) NULL);
                }
              header[length]='\0';
              (void) strcat(image->comments,(char *) header);
            }
            break;
          }
          default:
          {
            while (ReadDataBlock((char *) header,image->file) != 0);
            break;
          }
        }
      }
    if (c != ',')
      continue;
    /*
      Read image attributes.
    */
    if (image_count != 0)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
    image_count++;
    status=ReadData((char *) header,1,9,image->file);
    if (status == False)
      {
        Warning("Unable to read left/top/width/height",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    interlace=BitSet(header[8],0x40);
    local_colormap=BitSet(header[8],0x80);
    /*
      Allocate image.
    */
    image->columns=LSBFirstOrder(header[4],header[5]);
    image->rows=LSBFirstOrder(header[6],header[7]);
    image->packets=image->columns*image->rows;
    if (image->pixels != (RunlengthPacket *) NULL)
      (void) free((char *) image->pixels);
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Unable to read image","Memory allocation failed");
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Inititialize colormap.
    */
    image->class=PseudoClass;
    image->colors=!local_colormap ? global_colors : 1 << ((header[8] & 0x07)+1);
    image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
    if (image->colormap == (ColorPacket *) NULL)
      {
        Warning("Unable to read image","Memory allocation failed");
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (!local_colormap)
      {
        /*
          Use global colormap.
        */
        p=global_colormap;
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=(*p++);
          image->colormap[i].green=(*p++);
          image->colormap[i].blue=(*p++);
        }
      }
    else
      {
        unsigned char
          *colormap;

        /*
          Read local colormap.
        */
        colormap=(unsigned char *)
          malloc(3*image->colors*sizeof(unsigned char));
        if (colormap == (unsigned char *) NULL)
          {
            Warning("Unable to read local colormap","Memory allocation failed");
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) ReadData((char *) colormap,3,(int) image->colors,image->file);
        p=colormap;
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=(*p++);
          image->colormap[i].green=(*p++);
          image->colormap[i].blue=(*p++);
        }
        (void) free((char *) colormap);
      }
    /*
      Decode image.
    */
    status=LZWDecodeImage(image);
    if (status == False)
      {
        Warning("Unable to read image data","file is truncated");
        break;
      }
    if (interlace)
      {
        Image
          *interlaced_image;

        register RunlengthPacket
          *p;

        static int
          interlace_rate[4] = { 8, 8, 4, 2 },
          interlace_start[4] = { 0, 4, 2, 1 };

        /*
          Interlace image.
        */
        image->orphan=True;
        interlaced_image=CopyImage(image,image->columns,image->rows,True);
        image->orphan=False;
        if (interlaced_image == (Image *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImages(image);
            return((Image *) NULL);
          }
        p=interlaced_image->pixels;
        q=image->pixels;
        for (pass=0; pass < 4; pass++)
        {
          y=interlace_start[pass];
          while (y < image->rows)
          {
            q=image->pixels+(y*image->columns);
            for (x=0; x < image->columns; x++)
            {
              *q=(*p);
              p++;
              q++;
            }
            y+=interlace_rate[pass];
          }
        }
        DestroyImage(interlaced_image);
      }
    if (transparency_index >= 0)
      {
        /*
          Create matte channel.
        */
        q=image->pixels;
        for (i=0; i < image->packets; i++)
        {
          if (q->index != transparency_index)
            q->index=0;
          else
            q->index=MaxRGB;
          q++;
        }
        transparency_index=(-1);
        image->class=DirectClass;
        image->matte=True;
      }
  }
  (void) free((char *) global_colormap);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d G R A Y I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadGRAYImage reads an image of raw grayscale bytes and returns it.
%  It allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadGRAYImage routine is:
%
%      image=ReadGRAYImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadGRAYImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadGRAYImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    index,
    *p;

  unsigned char
    *gray_pixels;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Create linear colormap.
  */
  image->class=PseudoClass;
  image->colors=256;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if (image->colormap == (ColorPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  for (i=0; i < image->colors; i++)
  {
    image->colormap[i].red=(unsigned char) i;
    image->colormap[i].green=(unsigned char) i;
    image->colormap[i].blue=(unsigned char) i;
  }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize image structure.
  */
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  gray_pixels=(unsigned char *) malloc(image->packets*sizeof(unsigned char));
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if ((gray_pixels == (unsigned char *) NULL) ||
      (image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) gray_pixels,1,(int) image->packets,image->file);
  p=gray_pixels;
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    index=(*p++);
    q->red=index;
    q->green=index;
    q->blue=index;
    q->index=(unsigned short) index;
    q->length=0;
    q++;
  }
  (void) free((char *) gray_pixels);
  CompressColormap(image);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d H I S T O G R A M I m a g e                                        %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadHISTOGRAMImage reads a HISTOGRAM image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadHISTOGRAMImage routine is:
%
%      image=ReadHISTOGRAMImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadHISTOGRAMImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadHISTOGRAMImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  Warning("Cannot read HISTOGRAM images",image_info->filename);
  image=ReadMIFFImage(image_info);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d I R I S I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadIRISImage reads a SGI RGB image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadIRISImage routine is:
%
%      image=ReadIRISImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadIRISImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/

static void IRISDecode(packets,pixels)
unsigned char
  *packets,
  *pixels;
{
  unsigned char
    count,
    pixel;

  for ( ; ;)
  {
    pixel=(*packets++);
    count=pixel & 0x7f;
    if (count == 0)
      break;
    if (pixel & 0x80)
      for ( ; count != 0; count--)
      {
        *pixels=(*packets++);
        pixels+=4;
      }
    else
      {
        pixel=(*packets++);
        for ( ; count != 0; count--)
        {
          *pixels=pixel;
          pixels+=4;
        }
      }
  }
}

static Image *ReadIRISImage(image_info)
ImageInfo
  *image_info;
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

  Image
    *image;

  IRISHeader
    iris_header;

  register int
    i,
    x,
    y,
    z;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *iris_pixels;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read IRIS raster header.
  */
  iris_header.magic=MSBFirstReadShort(image->file);
  do
  {
    /*
      Verify IRIS identifier.
    */
    if (iris_header.magic != 0x01DA)
      {
        Warning("Not a IRIS RGB image,",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    iris_header.storage=fgetc(image->file);
    iris_header.bytes_per_pixel=fgetc(image->file);
    if (iris_header.bytes_per_pixel != 1)
      {
        Warning("Image must have 1 byte per pixel channel,",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    iris_header.dimension=MSBFirstReadShort(image->file);
    iris_header.columns=MSBFirstReadShort(image->file);
    iris_header.rows=MSBFirstReadShort(image->file);
    iris_header.depth=MSBFirstReadShort(image->file);
    iris_header.minimum_value=MSBFirstReadLong(image->file);
    iris_header.maximum_value=MSBFirstReadLong(image->file);
    (void) ReadData((char *) iris_header.filler,1,sizeof(iris_header.filler),
      image->file);
    /*
      Allocate IRIS pixels.
    */
    iris_pixels=(unsigned char *)
      malloc(4*iris_header.columns*iris_header.rows*sizeof(unsigned char));
    if (iris_pixels == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (iris_header.storage != 0x01)
      {
        unsigned char
          *scanline;

        /*
          Read standard image format.
        */
        scanline=(unsigned char *)
          malloc(iris_header.columns*sizeof(unsigned char));
        if (scanline == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",image->filename);
            DestroyImages(image);
            return((Image *) NULL);
          }
        for (z=0; z < (int) iris_header.depth; z++)
        {
          p=iris_pixels+z;
          for (y=0; y < (int) iris_header.rows; y++)
          {
            (void) ReadData((char *) scanline,1,(int) iris_header.columns,
              image->file);
            for (x=0; x < (int) iris_header.columns; x++)
            {
              *p=scanline[x];
              p+=4;
            }
          }
        }
        (void) free((char *) scanline);
      }
    else
      {
        unsigned char
          *packets;

        unsigned int
          data_order;

        unsigned long
          offset,
          *offsets,
          *runlength;

        /*
          Read runlength-encoded image format.
        */
        offsets=(unsigned long *)
          malloc(iris_header.rows*iris_header.depth*sizeof(unsigned long));
        packets=(unsigned char *)
          malloc(((iris_header.columns << 1)+10)*sizeof(unsigned char));
        runlength=(unsigned long *)
          malloc(iris_header.rows*iris_header.depth*sizeof(unsigned long));
        if ((offsets == (unsigned long *) NULL) ||
            (packets == (unsigned char *) NULL) ||
            (runlength == (unsigned long *) NULL))
          {
            Warning("Memory allocation error",image->filename);
            DestroyImages(image);
            return((Image *) NULL);
          }
        for (i=0; i < (int) (iris_header.rows*iris_header.depth); i++)
          offsets[i]=MSBFirstReadLong(image->file);
        for (i=0; i < (int) (iris_header.rows*iris_header.depth); i++)
          runlength[i]=MSBFirstReadLong(image->file);
        /*
          Check data order.
        */
        offset=0;
        data_order=0;
        for (y=0; ((y < (int) iris_header.rows) && !data_order); y++)
          for (z=0; ((z < (int) iris_header.depth) && !data_order); z++)
          {
            if (offsets[y+z*iris_header.rows] < offset)
              data_order=1;
            offset=offsets[y+z*iris_header.rows];
          }
        offset=512+4*((iris_header.rows*iris_header.depth) << 1);
        if (data_order == 1)
          {
            for (z=0; z < (int) iris_header.depth; z++)
            {
              p=iris_pixels;
              for (y=0; y < (int) iris_header.rows; y++)
              {
                if (offset != offsets[y+z*iris_header.rows])
                  {
                    offset=offsets[y+z*iris_header.rows];
                    (void) fseek(image->file,(int) offset,0);
                  }
                (void) ReadData((char *) packets,1,
                  (int) runlength[y+z*iris_header.rows],image->file);
                offset+=runlength[y+z*iris_header.rows];
                IRISDecode(packets,p+z);
                p+=(iris_header.columns*4);
              }
            }
          }
        else
          {
            p=iris_pixels;
            for (y=0; y < (int) iris_header.rows; y++)
            {
              for (z=0; z < (int) iris_header.depth; z++)
              {
                if (offset != offsets[y+z*iris_header.rows])
                  {
                    offset=offsets[y+z*iris_header.rows];
                    (void) fseek(image->file,(int) offset,0);
                  }
                (void) ReadData((char *) packets,1,
                  (int) runlength[y+z*iris_header.rows],image->file);
                offset+=runlength[y+z*iris_header.rows];
                IRISDecode(packets,p+z);
              }
              p+=(iris_header.columns*4);
            }
          }
        (void) free(runlength);
        (void) free(packets);
        (void) free(offsets);
      }
    /*
      Initialize image structure.
    */
    image->matte=iris_header.depth == 4;
    image->columns=iris_header.columns;
    image->rows=iris_header.rows;
    image->packets=image->columns*image->rows;
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert IRIS raster image to runlength-encoded packets.
    */
    q=image->pixels;
    if (iris_header.depth >= 3)
      {
        /*
          Convert IRIS image to DirectClass runlength-encoded packets.
        */
        for (y=0; y < image->rows; y++)
        {
          p=iris_pixels+((image->rows-1)-y)*(image->columns*4);
          for (x=0; x < image->columns; x++)
          {
            q->red=(*p);
            q->green=(*(p+1));
            q->blue=(*(p+2));
            q->index=(*(p+3));
            q->length=0;
            p+=4;
            q++;
          }
        }
      }
    else
      {
        unsigned short
          index;

        /*
          Create grayscale map.
        */
        image->class=PseudoClass;
        image->colors=256;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        if (image->colormap == (ColorPacket *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImage(image);
            return((Image *) NULL);
          }
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=(unsigned char) i;
          image->colormap[i].green=(unsigned char) i;
          image->colormap[i].blue=(unsigned char) i;
        }
        /*
          Convert IRIS image to PseudoClass runlength-encoded packets.
        */
        for (y=0; y < image->rows; y++)
        {
          p=iris_pixels+((image->rows-1)-y)*(image->columns*4);
          for (x=0; x < image->columns; x++)
          {
            index=(unsigned short) (*p);
            q->red=image->colormap[index].red;
            q->green=image->colormap[index].green;
            q->blue=image->colormap[index].blue;
            q->index=index;
            q->length=0;
            p+=4;
            q++;
          }
        }
      }
    (void) free((char *) iris_pixels);
    /*
      Proceed to next image.
    */
    iris_header.magic=MSBFirstReadShort(image->file);
    if (iris_header.magic == 0x01DA)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (iris_header.magic == 0x01DA);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

#ifdef HasJPEG
#undef FREAD
#undef FWRITE
#undef const
#include "jinclude.h"
Image
  *jpeg_image;

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d J P E G I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadJPEGImage reads a JPEG image file and returns it.  It allocates
%  the memory necessary for the new Image structure and returns a pointer to
%  the new image.
%
%  The format of the ReadJPEGImage routine is:
%
%      image=ReadJPEGImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadJPEGImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o filename:  Specifies the name of the jpeg image to read.
%
%
*/

static void MIFFInitializeImage(jpeg_info)
decompress_info_ptr
  jpeg_info;
{
  /*
    Create JPEG image.
  */
  jpeg_image->columns=jpeg_info->image_width;
  jpeg_image->rows=jpeg_info->image_height;
  jpeg_image->packets=jpeg_image->columns*jpeg_image->rows;
  jpeg_image->pixels=(RunlengthPacket *)
    malloc(jpeg_image->packets*sizeof(RunlengthPacket));
  if (jpeg_image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      exit(1);
    }
  jpeg_image->packet=jpeg_image->pixels;
}

static void MIFFComment(jpeg_info,length)
decompress_info_ptr
  jpeg_info;

long
  length;
{
  register char
    *p;

  if (jpeg_image->comments != (char *) NULL)
    jpeg_image->comments=(char *) realloc((char *) jpeg_image->comments,
      (unsigned int) (strlen(jpeg_image->comments)+length+1)*sizeof(char));
  else
    {
      jpeg_image->comments=(char *)
        malloc((unsigned int) (length+1)*sizeof(char));
      if (jpeg_image->comments != (char *) NULL)
        *jpeg_image->comments='\0';
    }
  if (jpeg_image->comments == (char *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      return;
    }
  p=jpeg_image->comments+strlen(jpeg_image->comments);
  for ( ; length > 0; length--)
    *p++=JGETC(jpeg_info);
  *p='\0';
}

static void MIFFOutputTermMethod(jpeg_info)
decompress_info_ptr
  jpeg_info;
{
}

static void MIFFWriteGRAY(jpeg_info,number_rows,pixel_data)
decompress_info_ptr
  jpeg_info;

int
  number_rows;

JSAMPIMAGE
  pixel_data;
{
  register int
    column,
    row;

  register JSAMPROW
    gray;

  register RunlengthPacket
    *q;

  /*
    Transfer grayscale JPEG pixel data to MIFF pixels.
  */
  q=jpeg_image->packet;
  for (row=0; row < number_rows; row++)
  {
    gray=pixel_data[0][row];
    for (column=jpeg_info->image_width; column > 0; column--)
    {
      q->red=GETJSAMPLE(*gray);
      q->green=GETJSAMPLE(*gray);
      q->blue=GETJSAMPLE(*gray);
      q->index=(unsigned short) GETJSAMPLE(*gray);
      q->length=0;
      q++;
      gray++;
    }
  }
  jpeg_image->packet=q;
}

static void MIFFWriteRGB(jpeg_info,number_rows,pixel_data)
decompress_info_ptr
  jpeg_info;

int
  number_rows;

JSAMPIMAGE
  pixel_data;
{
  register int
    column,
    row;

  register JSAMPROW
    blue,
    green,
    red;

  register RunlengthPacket
    *q;

  /*
    Transfer JPEG pixel data to MIFF pixels.
  */
  q=jpeg_image->packet;
  for (row=0; row < number_rows; row++)
  {
    red=pixel_data[0][row];
    green=pixel_data[1][row];
    blue=pixel_data[2][row];
    for (column=jpeg_info->image_width; column > 0; column--)
    {
      q->red=GETJSAMPLE(*red++);
      q->green=GETJSAMPLE(*green++);
      q->blue=GETJSAMPLE(*blue++);
      q->index=0;
      q->length=0;
      q++;
    }
  }
  jpeg_image->packet=q;
}

static void MIFFSelectMethod(jpeg_info)
decompress_info_ptr
  jpeg_info;
{
  jpeg_info->methods->put_pixel_rows=MIFFWriteRGB;
  jpeg_info->out_color_space=CS_RGB;
  if (jpeg_info->jpeg_color_space == CS_GRAYSCALE)
    {
      jpeg_info->out_color_space=CS_GRAYSCALE;
      jpeg_info->methods->put_pixel_rows=MIFFWriteGRAY;
    }
  jpeg_info->data_precision=8;
}

static Image *ReadJPEGImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  struct Decompress_info_struct
    jpeg_info;

  struct Decompress_methods_struct
    jpeg_methods;

  struct External_methods_struct
    external_methods;

  /*
    Allocate jpeg_image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize the JPEG system-dependent methods.
  */
  jpeg_image=image;
  jpeg_info.input_file=image->file;
  jpeg_info.output_file=(FILE *) NULL;
  jpeg_info.methods=(&jpeg_methods);
  jpeg_info.emethods=(&external_methods);
  jselerror(&external_methods);
  jselmemmgr(&external_methods);
  jpeg_info.methods->output_init=MIFFInitializeImage;
  jpeg_info.methods->output_term=MIFFOutputTermMethod;
  jpeg_methods.d_ui_method_selection=MIFFSelectMethod;
  j_d_defaults(&jpeg_info,True);
#ifdef HUFF_LOOKAHEAD
  jpeg_info.methods->process_comment=MIFFComment;
#endif
  /*
    Read a JFIF JPEG file.
  */
  jselrjfif(&jpeg_info);
  jpeg_decompress(&jpeg_info);
  if (jpeg_info.jpeg_color_space == CS_GRAYSCALE)
    {
      register int
        i;

      /*
        Initialize grayscale colormap.
      */
      jpeg_image->class=PseudoClass;
      jpeg_image->colors=256;
      jpeg_image->colormap=(ColorPacket *)
        malloc(image->colors*sizeof(ColorPacket));
      if (jpeg_image->colormap == (ColorPacket *) NULL)
        {
          Warning("Unable to create image colormap","Memory allocation failed");
          DestroyImage(jpeg_image);
          return((Image *) NULL);
        }
      for (i=0; i < jpeg_image->colors; i++)
      {
        jpeg_image->colormap[i].red=(unsigned short) i;
        jpeg_image->colormap[i].green=(unsigned short) i;
        jpeg_image->colormap[i].blue=(unsigned short) i;
      }
    }
  CloseImage(jpeg_image);
  return(jpeg_image);
}
#else
static Image *ReadJPEGImage(image_info)
ImageInfo
  *image_info;
{
  Warning("JPEG library is not available",image_info->filename);
  return(ReadMIFFImage(image_info));
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d M A P I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadMAPImage reads an image of raw RGB colormap and colormap index
%  bytes and returns it.  It allocates the memory necessary for the new Image
%  structure and returns a pointer to the new image.
%
%  The format of the ReadMAPImage routine is:
%
%      image=ReadMAPImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadMAPImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadMAPImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    colors;

  register int
    i;

  register unsigned char
    *p;

  unsigned char
    *colormap;

  unsigned int
    height,
    packet_size,
    status,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine width, height, and number of colors, e.g. 640x512+256.
  */
  width=512;
  height=512;
  colors=256;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&colors,&colors,&width,&height);
  /*
    Initialize image structure.
  */
  image->class=PseudoClass;
  image->compression=NoCompression;
  image->columns=width;
  image->rows=height;
  image->colors=colors;
  image->packets=image->columns*image->rows;
  packet_size=1;
  if (image->colors > 256)
    packet_size++;
  colormap=(unsigned char *) malloc(3*image->colors*sizeof(unsigned char));
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  image->packed_pixels=(unsigned char *)
    malloc(image->packets*packet_size*sizeof(unsigned char));
  if ((colormap == (unsigned char *) NULL) ||
      (image->colormap == (ColorPacket *) NULL) ||
      (image->packed_pixels == (unsigned char *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read image colormap.
  */
  (void) ReadData((char *) colormap,3,(int) image->colors,image->file);
  p=colormap;
  for (i=0; i < image->colors; i++)
  {
    image->colormap[i].red=(*p++);
    image->colormap[i].green=(*p++);
    image->colormap[i].blue=(*p++);
  }
  (void) free((char *) colormap);
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) image->packed_pixels,(int) packet_size,
    (int) image->packets,image->file);
  status=RunlengthDecodeImage(image);
  if (status == False)
    {
      DestroyImages(image);
      return((Image *) NULL);
    }
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d M A T T E I m a g e                                                %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadMATTEImage reads an image of raw matte bytes and returns it.
%  It allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadMATTEImage routine is:
%
%      image=ReadMATTEImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadMATTEImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadMATTEImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    index,
    *p;

  unsigned char
    *matte_pixels;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize image structure.
  */
  image->matte=True;
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  matte_pixels=(unsigned char *) malloc(image->packets*sizeof(unsigned char));
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if ((matte_pixels == (unsigned char *) NULL) ||
      (image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) matte_pixels,1,(int) image->packets,image->file);
  p=matte_pixels;
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    index=(*p++);
    q->red=0;
    q->green=0;
    q->blue=0;
    q->index=(unsigned short) index;
    q->length=0;
    q++;
  }
  (void) free((char *) matte_pixels);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d M I F F I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadMIFFImage reads a MIFF image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadMIFFImage routine is:
%
%      image=ReadMIFFImage(filename)
%
%  A description of each parameter follows:
%
%    o image: Function ReadMIFFImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadMIFFImage(image_info)
ImageInfo
  *image_info;
{
  char
    keyword[MaxTextLength],
    value[MaxTextLength];

  Image
    *image;

  register int
    c,
    i;

  register unsigned char
    *p;

  unsigned int
    length,
    packet_size,
    status;

  unsigned long
    count,
    packets;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Decode image header;  header terminates one character beyond a ':'.
  */
  c=fgetc(image->file);
  if (c == EOF)
    {
      DestroyImage(image);
      return((Image *) NULL);
    }
  do
  {
    /*
      Decode image header;  header terminates one character beyond a ':'.
    */
    image->compression=NoCompression;
    while (isgraph(c) && (c != ':'))
    {
      register char
        *p;

      if (c == '{')
        {
          /*
            Read comment-- any text between { }.
          */
          if (image->comments != (char *) NULL)
            {
              length=strlen(image->comments);
              p=image->comments+length;
            }
          else
            {
              length=MaxTextLength;
              image->comments=(char *) malloc(length*sizeof(char));
              p=image->comments;
            }
          for ( ; image->comments != (char *) NULL; p++)
          {
            c=fgetc(image->file);
            if ((c == EOF) || (c == '}'))
              break;
            if ((p-image->comments+1) >= length)
              {
                *p='\0';
                length<<=1;
                image->comments=(char *)
                  realloc((char *) image->comments,length*sizeof(char));
                if (image->comments == (char *) NULL)
                  break;
                p=image->comments+strlen(image->comments);
              }
            *p=(unsigned char) c;
          }
          if (image->comments == (char *) NULL)
            {
              Warning("Unable to read image","Memory allocation failed");
              DestroyImages(image);
              return((Image *) NULL);
            }
          *p='\0';
          c=fgetc(image->file);
        }
      else
        if (isalnum(c))
          {
            /*
              Determine a keyword and its value.
            */
            p=keyword;
            do
            {
              if ((p-keyword) < (MaxTextLength-1))
                *p++=(char) c;
              c=fgetc(image->file);
            } while (isalnum(c));
            *p='\0';
            while (isspace(c) || (c == '='))
              c=fgetc(image->file);
            p=value;
            while (!isspace(c))
            {
              if ((p-value) < (MaxTextLength-1))
                *p++=(char) c;
              c=fgetc(image->file);
            }
            *p='\0';
            /*
              Assign a value to the specified keyword.
            */
            if (strcmp(keyword,"class") == 0)
              if (strcmp(value,"PseudoClass") == 0)
                image->class=PseudoClass;
              else
                if (strcmp(value,"DirectClass") == 0)
                  image->class=DirectClass;
                else
                  image->class=UndefinedClass;
            if (strcmp(keyword,"colors") == 0)
              image->colors=(unsigned int) atoi(value);
            if (strcmp(keyword,"compression") == 0)
              if (strcmp(value,"QEncoded") == 0)
                image->compression=QEncodedCompression;
              else
                if (strcmp(value,"RunlengthEncoded") == 0)
                  image->compression=RunlengthEncodedCompression;
                else
                  image->compression=UndefinedCompression;
            if (strcmp(keyword,"columns") == 0)
              image->columns=(unsigned int) atoi(value);
            if (strcmp(keyword,"id") == 0)
              if (strcmp(value,"ImageMagick") == 0)
                image->id=ImageMagickId;
              else
                image->id=UndefinedId;
            if ((strcmp(keyword,"matte") == 0) ||
                (strcmp(keyword,"alpha") == 0))
              if ((strcmp(value,"True") == 0) || (strcmp(value,"true") == 0))
                image->matte=True;
              else
                image->matte=False;
            if (strcmp(keyword,"montage") == 0)
              {
                image->montage=(char *) malloc(strlen(value)+1*sizeof(char));
                if (image->montage == (char *) NULL)
                  {
                    Warning("Unable to read image","Memory allocation failed");
                    DestroyImages(image);
                    return((Image *) NULL);
                  }
                (void) strcpy(image->montage,value);
              }
            if (strcmp(keyword,"packets") == 0)
              image->packets=(unsigned int) atoi(value);
            if (strcmp(keyword,"rows") == 0)
              image->rows=(unsigned int) atoi(value);
            if (strcmp(keyword,"scene") == 0)
              image->scene=(unsigned int) atoi(value);
            if (strcmp(keyword,"signature") == 0)
              {
                image->signature=(char *)
                  malloc((strlen(value)+1)*sizeof(char));
                if (image->signature == (char *) NULL)
                  {
                    Warning("Unable to read image","Memory allocation failed");
                    DestroyImages(image);
                    return((Image *) NULL);
                  }
                (void) strcpy(image->signature,value);
              }
          }
        else
          c=fgetc(image->file);
      while (isspace(c))
        c=fgetc(image->file);
    }
    (void) fgetc(image->file);
    /*
      Verify that required image information is defined.
    */
    if ((image->id == UndefinedId) || (image->class == UndefinedClass) ||
        (image->compression == UndefinedCompression) || (image->columns == 0) ||
        (image->rows == 0))
      {
        Warning("Incorrect image header in file",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if ((image->columns*image->rows) > MaxImageSize)
      {
        Warning("Unable to read image","image size too large");
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (image->montage != (char *) NULL)
      {
        register char
          *p;

        /*
          Image directory.
        */
        image->directory=(char *) malloc(MaxTextLength*sizeof(char));
        if (image->directory == (char *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImages(image);
            return((Image *) NULL);
          }
        p=image->directory;
        do
        {
          if ((((int) strlen(image->directory)+1) % MaxTextLength) == 0)
            {
              /*
                Allocate more memory for the image directory.
              */
              *p='\0';
              image->directory=(char *) realloc((char *) image->directory,
                (strlen(image->directory)+MaxTextLength+1)*sizeof(char));
              if (image->directory == (char *) NULL)
                {
                  Warning("Unable to read image","Memory allocation failed");
                  DestroyImages(image);
                  return((Image *) NULL);
                }
              p=image->directory+strlen(image->directory);
            }
          c=fgetc(image->file);
          *p++=(unsigned char) c;
        } while (c != '\0');
      }
    if (image->class == PseudoClass)
      {
        unsigned int
          colors;

        /*
          PseudoClass image cannot have matte data or be QEncoded.
        */
        if (image->matte)
          {
            Warning("Unable to read image","matte images must be DirectClass");
            DestroyImages(image);
            return((Image *) NULL);
          }
        if (image->compression == QEncodedCompression)
          {
            Warning("Unable to read image",
              "QEncoded images must be DirectClass");
            DestroyImages(image);
            return((Image *) NULL);
          }
        /*
          Create image colormap.
        */
        colors=image->colors;
        if (colors == 0)
          colors=256;
        image->colormap=(ColorPacket *) malloc(colors*sizeof(ColorPacket));
        if (image->colormap == (ColorPacket *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImages(image);
            return((Image *) NULL);
          }
        if (image->colors == 0)
          for (i=0; i < colors; i++)
          {
            image->colormap[i].red=(unsigned char) i;
            image->colormap[i].green=(unsigned char) i;
            image->colormap[i].blue=(unsigned char) i;
            image->colors++;
          }
        else
          {
            unsigned char
              *colormap;

            /*
              Read image colormap from file.
            */
            colormap=(unsigned char *)
              malloc(3*image->colors*sizeof(unsigned char));
            if (colormap == (unsigned char *) NULL)
              {
                Warning("Unable to read image","Memory allocation failed");
                DestroyImages(image);
                return((Image *) NULL);
              }
            (void) ReadData((char *) colormap,1,(int) (3*image->colors),
              image->file);
            p=colormap;
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=(*p++);
              image->colormap[i].green=(*p++);
              image->colormap[i].blue=(*p++);
            }
            (void) free((char *) colormap);
          }
      }
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
    packet_size=image->packet_size;
    if (image->compression == QEncodedCompression)
      packet_size=1;
    /*
      Allocate image pixels.
    */
    if (image->compression == NoCompression)
      image->packets=image->columns*image->rows;
    packets=image->packets;
    if (image->packets == 0)
      packets=image->columns*image->rows;
    image->packed_pixels=(unsigned char *)
      malloc((unsigned int) packets*packet_size*sizeof(unsigned char));
    if (image->packed_pixels == (unsigned char *) NULL)
      {
        Warning("Unable to read image","Memory allocation failed");
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Read image pixels from file.
    */
    if ((image->compression != RunlengthEncodedCompression) ||
        (image->packets != 0))
      (void) ReadData((char *) image->packed_pixels,1,
        (int) (packets*packet_size),image->file);
    else
      {
        /*
          Number of runlength packets is unspecified.
        */
        count=0;
        p=image->packed_pixels;
        do
        {
          (void) ReadData((char *) p,1,(int) packet_size,image->file);
          image->packets++;
          p+=(packet_size-1);
          count+=(*p+1);
          p++;
        }
        while (count < (image->columns*image->rows));
      }
    if (image->compression ==  QEncodedCompression)
      {
        unsigned char
          *compressed_pixels;

        /*
          Uncompress image pixels with Q encoding.
        */
        image->packets=image->columns*image->rows;
        compressed_pixels=image->packed_pixels;
        image->packed_pixels=(unsigned char *) malloc((unsigned int)
          image->packets*image->packet_size*sizeof(unsigned char));
        if (image->packed_pixels == (unsigned char *) NULL)
          {
            Warning("Unable to write image","Memory allocation failed");
            DestroyImage(image);
            return((Image *) NULL);
          }
        packets=QDecodeImage(compressed_pixels,image->packed_pixels,
          image->columns*(int) image->packet_size,image->rows);
        if (packets != (image->packets*image->packet_size))
          {
            Warning("Q encoding failed",image->filename);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) free((char *) compressed_pixels);
      }
    /*
      Unpack the packed image pixels into runlength-encoded pixel packets.
    */
    status=RunlengthDecodeImage(image);
    if (status == False)
      {
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Proceed to next image.
    */
    do
    {
      c=fgetc(image->file);
    } while (!isgraph(c) && (c != EOF));
    if (c != EOF)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (c != EOF);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d M T V I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadMTVImage reads a MTV image file and returns it.  It allocates
%  the memory necessary for the new Image structure and returns a pointer to
%  the new image.
%
%  The format of the ReadMTVImage routine is:
%
%      image=ReadMTVImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadMTVImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadMTVImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    count;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *mtv_pixels;

  unsigned int
    columns,
    rows;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read MTV image.
  */
  count=fscanf(image->file,"%u %u\n",&columns,&rows);
  if (count == 0)
    {
      Warning("Not a MTV image,",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  do
  {
    /*
      Initialize image structure.
    */
    image->columns=columns;
    image->rows=rows;
    image->packets=image->columns*image->rows;
    mtv_pixels=(unsigned char *) malloc(3*image->packets*sizeof(unsigned char));
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if ((mtv_pixels == (unsigned char *) NULL) ||
        (image->pixels == (RunlengthPacket *) NULL))
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert MTV raster image to runlength-encoded packets.
    */
    (void) ReadData((char *) mtv_pixels,3,(int) image->packets,image->file);
    p=mtv_pixels;
    q=image->pixels;
    for (i=0; i < image->packets; i++)
    {
      q->red=(*p++);
      q->green=(*p++);
      q->blue=(*p++);
      q->index=0;
      q->length=0;
      q++;
    }
    (void) free((char *) mtv_pixels);
    /*
      Proceed to next image.
    */
    count=fscanf(image->file,"%u %u\n",&columns,&rows);
    if (count > 0)
      {
        /*
          Allocate next image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (count > 0);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d P C D I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadPCDImage reads a Photo CD image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadPCDImage routine is:
%
%      image=ReadPCDImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadPCDImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadPCDImage(image_info)
ImageInfo
  *image_info;
{
  char
    filename[MaxTextLength],
    options[MaxTextLength];

  Image
    *image,
    *next_image;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  CloseImage(image);
  DestroyImage(image);
  (void) strcpy(options,"-3");
  if (image_info->geometry != (char *) NULL)
    {
      int
        x,
        y;

      register int
        i;

      unsigned int
        height,
        width;

      /*
        Determine width and height, e.g. 640x512.
      */
      width=512;
      height=768;
      (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
      for (i=1; i < 5; i++)
      {
        width>>=1;
        height>>=1;
        if ((width < 128) && (height < 192))
          break;
      }
      (void) sprintf(options,"-%d",i);
    }
  /*
    Use hpcdtoppm to convert Photo CD image.
  */
  (void) strcpy(filename,image_info->filename);
  (void) sprintf(image_info->filename,"|hpcdtoppm -ppm %s %s",options,filename);
  image=ReadPNMImage(image_info);
  if (image == (Image *) NULL)
    {
      Warning("Photo CD translation failed",image_info->filename);
      return((Image *) NULL);
    }
  /*
    Assign proper filename.
  */
  do
  {
    (void) strcpy(image->filename,filename);
    next_image=image->next;
    if (next_image != (Image *) NULL)
      image=next_image;
  } while (next_image != (Image *) NULL);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d P C X I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadPCXImage reads a ZSoft IBM PC Paintbrush file and returns it.
%  It allocates the memory necessary for the new Image structure and returns
%  a pointer to the new image.
%
%  The format of the ReadPCXImage routine is:
%
%      image=ReadPCXImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadPCXImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadPCXImage(image_info)
ImageInfo
  *image_info;
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

  Image
    *image;

  int
    count,
    packets;

  register int
    i,
    x,
    y;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    packet,
    *pcx_colormap,
    *pcx_pixels;

  unsigned int
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine if this is a PCX file.
  */
  status=ReadData((char *) &pcx_header.identifier,1,1,image->file);
  do
  {
    /*
      Verify PCX identifier.
    */
    if ((status == False) || (pcx_header.identifier != 0x0a))
      {
        Warning("Not a PCX image file",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    (void) ReadData((char *) &pcx_header.version,1,1,image->file);
    (void) ReadData((char *) &pcx_header.encoding,1,1,image->file);
    (void) ReadData((char *) &pcx_header.bits_per_pixel,1,1,image->file);
    pcx_header.left=LSBFirstReadShort(image->file);
    pcx_header.top=LSBFirstReadShort(image->file);
    pcx_header.right=LSBFirstReadShort(image->file);
    pcx_header.bottom=LSBFirstReadShort(image->file);
    pcx_header.horizontal_resolution=LSBFirstReadShort(image->file);
    pcx_header.vertical_resolution=LSBFirstReadShort(image->file);
    /*
      Read PCX raster colormap.
    */
    image->columns=(pcx_header.right-pcx_header.left)+1;
    image->rows=(pcx_header.bottom-pcx_header.top)+1;
    image->class=PseudoClass;
    image->colors=16;
    image->colormap=(ColorPacket *) malloc(256*sizeof(ColorPacket));
    pcx_colormap=(unsigned char *) malloc(3*256*sizeof(unsigned char));
    if ((image->colormap == (ColorPacket *) NULL) ||
        (pcx_colormap == (unsigned char *) NULL))
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    (void) ReadData((char *) pcx_colormap,3,(int) image->colors,image->file);
    p=pcx_colormap;
    for (i=0; i < image->colors; i++)
    {
      image->colormap[i].red=(*p++);
      image->colormap[i].green=(*p++);
      image->colormap[i].blue=(*p++);
    }
    (void) ReadData((char *) &pcx_header.reserved,1,1,image->file);
    (void) ReadData((char *) &pcx_header.planes,1,1,image->file);
    pcx_header.bytes_per_line=LSBFirstReadShort(image->file);
    pcx_header.palette_info=LSBFirstReadShort(image->file);
    for (i=0; i < 58; i++)
      (void) fgetc(image->file);
    /*
      Read image data.
    */
    packets=image->rows*pcx_header.bytes_per_line*pcx_header.planes;
    pcx_pixels=(unsigned char *) malloc(packets*sizeof(unsigned char));
    if (pcx_pixels == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Uncompress image data.
    */
    p=pcx_pixels;
    while (packets > 0)
    {
      packet=fgetc(image->file);
      if ((packet & 0xc0) != 0xc0)
        {
          *p++=packet;
          packets--;
          continue;
        }
      count=packet & 0x3f;
      packet=fgetc(image->file);
      packets-=count;
      while (--count >= 0)
        *p++=packet;
    }
    image->colors=1 << (pcx_header.bits_per_pixel*pcx_header.planes);
    if (image->colors > 16)
      {
        /*
          256 color images have their color map at the end of the file.
        */
        (void) ReadData((char *) &pcx_header.colormap_signature,1,1,
          image->file);
        (void) ReadData((char *) pcx_colormap,3,(int) image->colors,
          image->file);
        p=pcx_colormap;
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=(*p++);
          image->colormap[i].green=(*p++);
          image->colormap[i].blue=(*p++);
        }
      }
    else
      if (image->colors == 2)
        if (Intensity(image->colormap[0]) == Intensity(image->colormap[1]))
          {
            /*
              Monochrome colormap.
            */
            image->colormap[0].red=MaxRGB;
            image->colormap[0].green=MaxRGB;
            image->colormap[0].blue=MaxRGB;
            image->colormap[1].red=0;
            image->colormap[1].green=0;
            image->colormap[1].blue=0;
          }
    (void) free((char *) pcx_colormap);
    /*
      Initialize image structure.
    */
    image->packets=image->columns*image->rows;
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert PCX raster image to runlength-encoded packets.
    */
    q=image->pixels;
    if (pcx_header.planes > 1)
      {
        register int
          bits,
          mask;

        /*
          Convert multi-plane format into runlength-encoded pixels.
        */
        for (i=0; i < image->packets; i++)
        {
          q->index=0;
          q->length=0;
          q++;
        }
        for (y=0; y < image->rows; y++)
        {
          p=pcx_pixels+(y*pcx_header.bytes_per_line*pcx_header.planes);
          for (i=0; i < (int) pcx_header.planes; i++)
          {
            q=image->pixels+y*image->columns;
            for (x=0; x < pcx_header.bytes_per_line; x++)
            {
              bits=(*p++);
              for (mask=0x80; mask != 0; mask>>=1)
              {
                if (bits & mask)
                  q->index|=1 << i;
                q++;
              }
            }
          }
        }
      }
    else
      for (y=0; y < image->rows; y++)
      {
        p=pcx_pixels+y*pcx_header.bytes_per_line;
        switch (pcx_header.bits_per_pixel)
        {
          case 1:
          {
            register int
              bit;

            for (x=0; x < (image->columns-7); x+=8)
            {
              for (bit=7; bit >= 0; bit--)
              {
                q->index=((*p) & (0x01 << bit) ? 0x01 : 0x00);
                q->length=0;
                q++;
              }
              p++;
            }
            if ((image->columns % 8) != 0)
              {
                for (bit=7; bit >= (8-(image->columns % 8)); bit--)
                {
                  q->index=((*p) & (0x01 << bit) ? 0x01 : 0x00);
                  q->length=0;
                  q++;
                }
                p++;
              }
            break;
          }
          case 2:
          {
            for (x=0; x < (image->columns-3); x+=4)
            {
              q->index=(*p >> 6) & 0x3;
              q->length=0;
              q++;
              q->index=(*p >> 4) & 0x3;
              q->length=0;
              q++;
              q->index=(*p >> 2) & 0x3;
              q->length=0;
              q++;
              q->index=(*p) & 0x3;
              q->length=0;
              q++;
              p++;
            }
            if ((image->columns % 4) != 0)
              {
                for (i=3; i >= (4-(image->columns % 4)); i--)
                {
                  q->index=(*p >> (i*2)) & 0x03;
                  q->length=0;
                  q++;
                }
                p++;
              }
            break;
          }
          case 4:
          {
            for (x=0; x < (image->columns-1); x+=2)
            {
              q->index=(*p >> 4) & 0xf;
              q->length=0;
              q++;
              q->index=(*p) & 0xf;
              q->length=0;
              q++;
              p++;
            }
            if ((image->columns % 2) != 0)
              {
                q->index=(*p >> 4) & 0xf;
                q->length=0;
                q++;
                p++;
              }
            break;
          }
          case 8:
          {
            for (x=0; x < image->columns; x++)
            {
              q->index=(*p);
              q->length=0;
              q++;
              p++;
            }
            break;
          }
          default:
            break;
        }
      }
    (void) free((char *) pcx_pixels);
    SyncImage(image);
    /*
      Proceed to next image.
    */
    status=ReadData((char *) &pcx_header.identifier,1,1,image->file);
    if ((status == True) && (pcx_header.identifier == 0x0a))
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while ((status == True) && (pcx_header.identifier == 0x0a));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d P I C T I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadPICTImage reads an Apple Macintosh QuickDraw/PICT image file
%  and returns it.  It allocates the memory necessary for the new Image
%  structure and returns a pointer to the new image.
%
%  The format of the ReadPICTImage routine is:
%
%      image=ReadPICTImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadPICTImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadPICTImage(image_info)
ImageInfo
  *image_info;
{
  char
    filename[MaxTextLength];

  Image
    *image,
    *next_image;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  CloseImage(image);
  DestroyImage(image);
  /*
    Use picttoppm to convert Macintosh PICT image.
  */
  (void) strcpy(filename,image_info->filename);
  (void) sprintf(image_info->filename,"|picttoppm %s",filename);
  image=ReadPNMImage(image_info);
  if (image == (Image *) NULL)
    {
      Warning("PICT translation failed",image_info->filename);
      return((Image *) NULL);
    }
  /*
    Assign proper filename.
  */
  do
  {
    (void) strcpy(image->filename,filename);
    next_image=image->next;
    if (next_image != (Image *) NULL)
      image=next_image;
  } while (next_image != (Image *) NULL);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d P N M I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadPNMImage reads a Portable Anymap image file and returns it.
%  It allocates the memory necessary for the new Image structure and returns
%  a pointer to the new image.
%
%  The format of the ReadPNMImage routine is:
%
%      image=ReadPNMImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadPNMImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static unsigned int GetInteger(image,base)
Image
  *image;

unsigned int
  base;
{
  int
    c;

  unsigned int
    value;

  /*
    Skip any leading whitespace.
  */
  do
  {
    c=fgetc(image->file);
    if (c == EOF)
      return(0);
    if (c == '#')
      {
        register char
          *p;

        unsigned int
          length;

        /*
          Read comment.
        */
        if (image->comments != (char *) NULL)
          {
            length=strlen(image->comments);
            p=image->comments+length;
          }
        else
          {
            length=MaxTextLength;
            image->comments=(char *) malloc(length*sizeof(char));
            p=image->comments;
          }
        for ( ; image->comments != (char *) NULL; p++)
        {
          if ((p-image->comments+2) >= length)
            {
              *p='\0';
              length<<=1;
              image->comments=(char *)
                realloc((char *) image->comments,length*sizeof(char));
              if (image->comments == (char *) NULL)
                break;
              p=image->comments+strlen(image->comments);
            }
          c=fgetc(image->file);
          if ((c == EOF) || (c == '\n'))
            break;
          *p=(unsigned char) c;
        }
        if (image->comments == (char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            return(0);
          }
        *p++='\n';
        *p='\0';
      }
  } while (!isdigit(c));
  if (base == 2)
    return(c-'0');
  /*
    Evaluate number.
  */
  value=0;
  do
  {
    value*=10;
    value+=c-'0';
    c=fgetc(image->file);
    if (c == EOF)
      return(0);
  }
  while (isdigit(c));
  return(value);
}

static Image *ReadPNMImage(image_info)
ImageInfo
  *image_info;
{
  char
    format;

  Image
    *image;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *pixels,
    *scale;

  unsigned int
    max_value,
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read PNM image.
  */
  status=ReadData((char *) &format,1,1,image->file);
  do
  {
    /*
      Verify PNM identifier.
    */
    if ((status == False) || (format != 'P'))
      {
        Warning("Not a PNM image file",(char *) NULL);
        DestroyImage(image);
        return((Image *) NULL);
      }
    /*
      Initialize image structure.
    */
    format=fgetc(image->file);
    image->columns=GetInteger(image,10);
    image->rows=GetInteger(image,10);
    image->packets=image->columns*image->rows;
    if (image->packets == 0)
      {
        Warning("Unable to read image","image dimensions are zero");
        return((Image *) NULL);
      }
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if ((format == '1') || (format == '4'))
      max_value=1;  /* bitmap */
    else
      max_value=GetInteger(image,10);
    scale=(unsigned char *) NULL;
    if (max_value != MaxRGB)
      {
        /*
          Compute pixel scaling table.
        */
        scale=(unsigned char *) malloc((max_value+1)*sizeof(unsigned char));
        if (scale == (unsigned char *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImages(image);
            return((Image *) NULL);
          }
        for (i=0; i <= max_value; i++)
          scale[i]=(unsigned char) ((i*MaxRGB+(max_value >> 1))/max_value);
      }
    if ((format != '3') && (format != '6'))
      {
        /*
          Create gray scale colormap.
        */
        image->class=PseudoClass;
        image->colors=Min(max_value,MaxRGB)+1;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        if (image->colormap == (ColorPacket *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=(MaxRGB*i)/(image->colors-1);
          image->colormap[i].green=(MaxRGB*i)/(image->colors-1);
          image->colormap[i].blue=(MaxRGB*i)/(image->colors-1);
        }
      }
    /*
      Convert PNM pixels to runlength-encoded MIFF packets.
    */
    q=image->pixels;
    switch (format)
    {
      case '1':
      {
        /*
          Convert PBM image to runlength-encoded packets.
        */
        for (i=0; i < image->packets; i++)
        {
          q->index=!GetInteger(image,2);
          q->length=0;
          q++;
        }
        SyncImage(image);
        break;
      }
      case '2':
      {
        /*
          Convert PGM image to runlength-encoded packets.
        */
        if (max_value == MaxRGB)
          for (i=0; i < image->packets; i++)
          {
            q->index=GetInteger(image,10);
            q->length=0;
            q++;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            q->index=scale[GetInteger(image,10)];
            q->length=0;
            q++;
          }
        SyncImage(image);
        break;
      }
      case '3':
      {
        /*
          Convert PNM image to runlength-encoded packets.
        */
        if (max_value == MaxRGB)
          for (i=0; i < image->packets; i++)
          {
            q->red=GetInteger(image,10);
            q->green=GetInteger(image,10);
            q->blue=GetInteger(image,10);
            q->index=0;
            q->length=0;
            q++;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            q->red=scale[GetInteger(image,10)];
            q->green=scale[GetInteger(image,10)];
            q->blue=scale[GetInteger(image,10)];
            q->index=0;
            q->length=0;
            q++;
          }
        break;
      }
      case '4':
      {
        unsigned char
          bit,
          byte;

        unsigned int
          x,
          y;

        /*
          Convert PBM raw image to runlength-encoded packets.
        */
        for (y=0; y < image->rows; y++)
        {
          bit=0;
          byte=0;
          for (x=0; x < image->columns; x++)
          {
            if (bit == 0)
              byte=fgetc(image->file);
            q->index=(byte & 0x80) ? 0 : 1;
            q->length=0;
            q++;
            bit++;
            if (bit == 8)
              bit=0;
            byte<<=1;
          }
        }
        SyncImage(image);
        break;
      }
      case '5':
      {
        /*
          Convert PGM raw image to runlength-encoded packets.
        */
        pixels=(unsigned char *)
          malloc(image->packets*sizeof(unsigned char));
        if (pixels == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        status=ReadData((char *) pixels,1,(int) image->packets,image->file);
        if (status == False)
          {
            Warning("Insufficient image data in file",image->filename);
            DestroyImages(image);
            return((Image *) NULL);
          }
        /*
          Convert PNM raw image to runlength-encoded packets.
        */
        p=pixels;
        for (i=0; i < image->packets; i++)
        {
          q->index=(*p++);
          q->length=0;
          q++;
        }
        SyncImage(image);
        (void) free((char *) pixels);
        break;
      }
      case '6':
      {
        /*
          Convert PNM raster image to runlength-encoded packets.
        */
        pixels=(unsigned char *)
          malloc(image->packets*3*sizeof(unsigned char));
        if (pixels == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        status=ReadData((char *) pixels,1,(int) image->packets*3,image->file);
        if (status == False)
          {
            Warning("Insufficient image data in file",image->filename);
            DestroyImages(image);
            return((Image *) NULL);
          }
        p=pixels;
        if (max_value == MaxRGB)
          for (i=0; i < image->packets; i++)
          {
            q->red=(*p++);
            q->green=(*p++);
            q->blue=(*p++);
            q->index=0;
            q->length=0;
            q++;
          }
        else
          for (i=0; i < image->packets; i++)
          {
            q->red=scale[*p++];
            q->green=scale[*p++];
            q->blue=scale[*p++];
            q->index=0;
            q->length=0;
            q++;
          }
        (void) free((char *) pixels);
        break;
      }
      default:
      {
        Warning("Not a PNM image file",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    }
    if (scale != (unsigned char *) NULL)
      (void) free((char *) scale);
    if (image->class == PseudoClass)
      CompressColormap(image);
    /*
      Proceed to next image.
    */
    if ((format == '1') || (format == '2') || (format == '3'))
      do
      {
        /*
          Skip to end of line.
        */
        status=ReadData(&format,1,1,image->file);
        if (status == False)
          break;
      } while (format != '\n');
    status=ReadData((char *) &format,1,1,image->file);
    if ((status == True) && (format == 'P'))
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while ((status == True) && (format == 'P'));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d P S I m a g e                                                      %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadPSImage reads a Adobe Postscript image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadPSImage routine is:
%
%      image=ReadPSImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadPSImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadPSImage(image_info)
ImageInfo
  *image_info;
{
#define BoundingBox  "%%BoundingBox:"
#define PageBoundingBox  "%%PageBoundingBox:"

  char
    command[MaxTextLength],
    clip_geometry[MaxTextLength],
    *device,
    *directory,
    filename[MaxTextLength],
    options[MaxTextLength];

  FILE
    *file;

  Image
    *image,
    *next_image;

  ImageInfo
    postscript_info;

  unsigned int
    showpage;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Open temporary output file.
  */
  directory=(char *) getenv("TMPDIR");
  if (directory == (char *) NULL)
    directory="/tmp";
  (void) sprintf(filename,"%s/magickXXXXXX",directory);
  (void) mktemp(filename);
  file=fopen(filename,"wb");
  if (file == (FILE *) NULL)
    {
      Warning("Unable to write file",filename);
      return((Image *) NULL);
    }
  /*
    Determine if Postscript specifies a bounding box.
  */
  showpage=False;
  *options='\0';
  *clip_geometry='\0';
  while (fgets(command,MaxTextLength-1,image->file) != (char *) NULL)
  {
    showpage|=GlobExpression(command,"*showpage*");
    if ((strncmp(BoundingBox,command,strlen(BoundingBox)) == 0) ||
        (strncmp(PageBoundingBox,command,strlen(PageBoundingBox)) == 0))
      {
        int
          count,
          flags,
          lower_x,
          lower_y,
          upper_x,
          upper_y,
          x,
          y;

        unsigned int
          dx_resolution,
          dy_resolution,
          x_resolution,
          y_resolution;

        if (strncmp(BoundingBox,command,strlen(BoundingBox)) == 0)
          count=sscanf(command,"%%%%BoundingBox: %d %d %d %d",&lower_x,&lower_y,
            &upper_x,&upper_y);
        if (strncmp(PageBoundingBox,command,strlen(PageBoundingBox)) == 0)
          count=sscanf(command,"%%%%PageBoundingBox: %d %d %d %d",
            &lower_x,&lower_y,&upper_x,&upper_y);
        if (count != 4)
          continue;
        /*
          Determine bounding box.
        */
        (void) XParseGeometry(PSDensityGeometry,&x,&y,&dx_resolution,
          &dy_resolution);
        x_resolution=dx_resolution;
        y_resolution=dy_resolution;
        flags=XParseGeometry(image_info->density,&x,&y,&x_resolution,
          &y_resolution);
        if ((flags & HeightValue) == 0)
          y_resolution=x_resolution;
        if (image_info->page == (char *) NULL)
          {
            unsigned int
              height,
              width;

            /*
              Set Postscript render geometry.
            */
            width=(upper_x*x_resolution+(x_resolution >> 1))/dx_resolution;
            height=(upper_y*y_resolution+(y_resolution >> 1))/dy_resolution;
            if (width > height)
              (void) sprintf(options,"-g%ux%u",width,width);
            else
              (void) sprintf(options,"-g%ux%u",height,height);
          }
        (void) strcpy(clip_geometry,"0x0");
      }
      (void) fputs(command,file);
    }
  CloseImage(image);
  DestroyImage(image);
  /*
    Force *showpage* operator and close files.
  */
  if (!showpage)
    (void) fputs("showpage\n",file);
  if (ferror(file))
    {
      Warning("An error has occurred writing to file",filename);
      (void) fclose(file);
      return((Image *) NULL);
    }
  (void) fclose(file);
  /*
    Determine if page geometry or density options are specified.
  */
  if (image_info->page != (char *) NULL)
    {
      (void) strcat(options," -g");
      (void) strcat(options,image_info->page);
    }
  if (image_info->density != (char *) NULL)
    {
      (void) strcat(options," -r");
      (void) strcat(options,image_info->density);
    }
  /*
    Use Ghostscript to convert Postscript image.
  */
  device="ppmraw";
  if (image_info->monochrome)
    device="pbmraw";
  postscript_info=(*image_info);
  (void) sprintf(postscript_info.filename,
    "|gs -q -dNOPAUSE -sDEVICE=%s -sOutputFile=- %s %s < /dev/null",
    device,options,filename);
  image=ReadPNMImage(&postscript_info);
  (void) unlink(filename);
  if (image == (Image *) NULL)
    {
      Warning("Postscript translation failed",image_info->filename);
      return((Image *) NULL);
    }
  do
  {
    (void) strcpy(image->filename,image_info->filename);
    if (*clip_geometry != '\0')
      {
        /*
          Clip to bounding box.
        */
        TransformImage(&image,clip_geometry,(char *) NULL);
        if (image->next != (Image *) NULL)
          image->next->previous=image;
      }
    next_image=image->next;
    if (next_image != (Image *) NULL)
      image=next_image;
  } while (next_image != (Image *) NULL);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d R G B I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadRGBImage reads an image of raw red, green, and blue bytes and
%  returns it.  It allocates the memory necessary for the new Image structure
%  and returns a pointer to the new image.
%
%  The format of the ReadRGBImage routine is:
%
%      image=ReadRGBImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadRGBImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadRGBImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *rgb_pixels;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize image structure.
  */
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  rgb_pixels=(unsigned char *) malloc(3*image->packets*sizeof(unsigned char));
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if ((rgb_pixels == (unsigned char *) NULL) ||
      (image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) rgb_pixels,3,(int) image->packets,image->file);
  p=rgb_pixels;
  switch (image_info->interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  RGBRGBRGBRGBRGBRGB...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->green=(*p++);
        q->blue=(*p++);
        q->index=0;
        q->length=0;
        q++;
      }
      break;
    }
    case LineInterlace:
    {
      /*
        Line interlacing:  RRR...GGG...BBB...RRR...GGG...BBB...
      */
      for (y=0; y < image->rows; y++)
      {
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->red=(*p++);
          q->index=0;
          q->length=0;
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->green=(*p++);
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->blue=(*p++);
          q++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  RRRRRR...GGGGGG...BBBBBB...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->index=0;
        q->length=0;
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->green=(*p++);
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->blue=(*p++);
        q++;
      }
      break;
    }
  }
  (void) free((char *) rgb_pixels);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d R L E I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadRLEImage reads a run-length encoded Utah Raster Toolkit
%  image file and returns it.  It allocates the memory necessary for the new
%  Image structure and returns a pointer to the new image.
%
%  The format of the ReadRLEImage routine is:
%
%      image=ReadRLEImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadRLEImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadRLEImage(image_info)
ImageInfo
  *image_info;
{
#define SkipLinesOp  0x01
#define SetColorOp  0x02
#define SkipPixelsOp  0x03
#define ByteDataOp  0x05
#define RunDataOp  0x06
#define EOFOp  0x07

  char
    magick[12];

  Image
    *image;

  int
    opcode,
    operand,
    status,
    x,
    y;

  register int
    i,
    j;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    background_color[256],
    *colormap,
    pixel,
    plane,
    *rle_pixels;

  unsigned int
    bits_per_pixel,
    flags,
    map_length,
    number_colormaps,
    number_planes;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine if this is a RLE file.
  */
  status=ReadData((char *) magick,1,2,image->file);
  if ((status == False) || (strncmp(magick,"\122\314",2) != 0))
    {
      Warning("Not a RLE image file",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  do
  {
    /*
      Read image header.
    */
    (void) LSBFirstReadShort(image->file);
    (void) LSBFirstReadShort(image->file);
    image->columns=LSBFirstReadShort(image->file);
    image->rows=LSBFirstReadShort(image->file);
    image->packets=image->columns*image->rows;
    flags=fgetc(image->file);
    image->matte=flags & 0x04;
    number_planes=fgetc(image->file);
    bits_per_pixel=fgetc(image->file);
    number_colormaps=fgetc(image->file);
    map_length=1 << fgetc(image->file);
    if ((number_planes == 0) || (number_planes == 2) || (bits_per_pixel != 8) ||
        (image->columns == 0))
      {
        Warning("Unsupported RLE image file",(char *) NULL);
        DestroyImage(image);
        return((Image *) NULL);
      }
    if (flags & 0x02)
      {
        /*
          No background color-- initialize to black.
        */
        for (i=0; i < number_planes; i++)
          background_color[i]=(unsigned char) 0;
        (void) fgetc(image->file);
      }
    else
      {
        /*
          Initialize background color.
        */
        p=background_color;
        for (i=0; i < number_planes; i++)
          *p++=(unsigned char) fgetc(image->file);
      }
    if ((number_planes & 0x01) == 0)
      (void) fgetc(image->file);
    colormap=(unsigned char *) NULL;
    if (number_colormaps != 0)
      {
        /*
          Read image colormaps.
        */
        colormap=(unsigned char *)
          malloc(number_colormaps*map_length*sizeof(unsigned char));
        if (colormap == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImage(image);
            return((Image *) NULL);
          }
        p=colormap;
        for (i=0; i < number_colormaps; i++)
          for (j=0; j < map_length; j++)
            *p++=(unsigned char) (LSBFirstReadShort(image->file) >> 8);
      }
    if (flags & 0x08)
      {
        unsigned int
          length;

        /*
          Read image comment.
        */
        length=LSBFirstReadShort(image->file);
        image->comments=(char *) malloc((length+1)*sizeof(char));
        if (image->comments == (char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImage(image);
            return((Image *) NULL);
          }
        (void) ReadData((char *) image->comments,1,(int) length,image->file);
        image->comments[length]='\0';
        if ((length & 0x01) == 0)
          (void) fgetc(image->file);
      }
    /*
      Allocate RLE pixels.
    */
    if (image->matte)
      number_planes++;
    rle_pixels=(unsigned char *)
      malloc(image->packets*number_planes*sizeof(unsigned char));
    if (rle_pixels == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImage(image);
        return((Image *) NULL);
      }
    if ((flags & 0x01) && ((~flags) & 0x02))
      {
        /*
          Set background color.
        */
        p=rle_pixels;
        for (i=0; i < image->packets; i++)
        {
          if (!image->matte)
            for (j=0; j < number_planes; j++)
              *p++=background_color[j];
          else
            {
              for (j=0; j < (number_planes-1); j++)
                *p++=background_color[j];
              *p++=0;  /* initialize matte channel */
            }
        }
      }
    /*
      Read runlength-encoded image.
    */
    plane=0;
    x=0;
    y=0;
    (void) fgetc(image->file);
    opcode=fgetc(image->file);
    while (((opcode & 0x3f) != EOFOp) && (opcode != EOF))
    {
      switch (opcode & 0x3f)
      {
        case SkipLinesOp:
        {
          operand=fgetc(image->file);
          if (opcode & 0x40)
            operand=LSBFirstReadShort(image->file);
          x=0;
          y+=operand;
          break;
        }
        case SetColorOp:
        {
          operand=fgetc(image->file);
          plane=operand;
          if (plane == 255)
            plane=number_planes-1;
          x=0;
          break;
        }
        case SkipPixelsOp:
        {
          operand=fgetc(image->file);
          if (opcode & 0x40)
            operand=LSBFirstReadShort(image->file);
          x+=operand;
          break;
        }
        case ByteDataOp:
        {
          operand=fgetc(image->file);
          if (opcode & 0x40)
            operand=LSBFirstReadShort(image->file);
          p=rle_pixels+((image->rows-y-1)*image->columns*number_planes)+
            x*number_planes+plane;
          operand++;
          for (i=0; i < operand; i++)
          {
            pixel=fgetc(image->file);
            if ((y < image->rows) && ((x+i) < image->columns))
              *p=pixel;
            p+=number_planes;
          }
          if (operand & 0x01)
            (void) fgetc(image->file);
          x+=operand;
          break;
        }
        case RunDataOp:
        {
          operand=fgetc(image->file);
          if (opcode & 0x40)
            operand=LSBFirstReadShort(image->file);
          pixel=fgetc(image->file);
          (void) fgetc(image->file);
          operand++;
          p=rle_pixels+((image->rows-y-1)*image->columns*number_planes)+
            x*number_planes+plane;
          for (i=0; i < operand; i++)
          {
            if ((y < image->rows) && ((x+i) < image->columns))
              *p=pixel;
            p+=number_planes;
          }
          x+=operand;
          break;
        }
        default:
          break;
      }
      opcode=fgetc(image->file);
    }
    if (number_colormaps != 0)
      {
        unsigned int
          mask;

        /*
          Apply colormap transformation to image.
        */
        mask=(map_length-1);
        p=rle_pixels;
        if (number_colormaps == 1)
          for (i=0; i < image->packets; i++)
          {
            *p=(unsigned char) colormap[*p & mask];
            p++;
          }
        else
          if ((number_planes >= 3) && (number_colormaps >= 3))
            for (i=0; i < image->packets; i++)
              for (j=0; j < number_planes; j++)
              {
                *p=(unsigned char) colormap[j*map_length+(*p & mask)];
                p++;
              }
      }
    /*
      Initialize image structure.
    */
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImage(image);
        return((Image *) NULL);
      }
    q=image->pixels;
    if (number_planes >= 3)
      {
        /*
          Convert raster image to DirectClass runlength-encoded packets.
        */
        p=rle_pixels;
        for (i=0; i < image->packets; i++)
        {
          q->red=(*p++);
          q->green=(*p++);
          q->blue=(*p++);
          q->index=(unsigned short) (image->matte ? (*p++) : 0);
          q->length=0;
          q++;
        }
      }
    else
      {
        /*
          Create colormap.
        */
        image->class=PseudoClass;
        image->colors=256;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        if (image->colormap == (ColorPacket *) NULL)
          {
            Warning("Unable to read image","Memory allocation failed");
            DestroyImage(image);
            return((Image *) NULL);
          }
        p=colormap;
        if (number_colormaps == 1)
          for (i=0; i < image->colors; i++)
          {
            image->colormap[i].red=(unsigned char) i;
            image->colormap[i].green=(unsigned char) i;
            image->colormap[i].blue=(unsigned char) i;
          }
        else
          for (i=0; i < image->colors; i++)
          {
            image->colormap[i].red=(*p);
            image->colormap[i].green=(*(p+256));
            image->colormap[i].blue=(*(p+512));
            p++;
          }
        /*
          Convert raster image to PseudoClass runlength-encoded packets.
        */
        p=rle_pixels;
        for (i=0; i < image->packets; i++)
        {
          q->index=(unsigned short) (*p++);
          q->length=0;
          q++;
        }
        SyncImage(image);
      }
    if (number_colormaps != 0)
      (void) free((char *) colormap);
    (void) free((char *) rle_pixels);
    /*
      Proceed to next image.
    */
    (void) fgetc(image->file);
    status=ReadData((char *) magick,1,2,image->file);
    if ((status == True) && (strncmp(magick,"\122\314",2) == 0))
      {
        /*
          Allocate next image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while ((status == True) && (strncmp(magick,"\122\314",2) == 0));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d S U N I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadSUNImage reads a SUN image file and returns it.  It allocates
%  the memory necessary for the new Image structure and returns a pointer to
%  the new image.
%
%  The format of the ReadSUNImage routine is:
%
%      image=ReadSUNImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadSUNImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadSUNImage(image_info)
ImageInfo
  *image_info;
{
#define RMT_EQUAL_RGB  1
#define RMT_NONE  0
#define RMT_RAW  2
#define RT_STANDARD  1
#define RT_ENCODED  2
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

  Image
    *image;

  register int
    bit,
    i,
    x,
    y;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  SUNHeader
    sun_header;

  unsigned char
    *sun_data,
    *sun_pixels;

  unsigned int
    status;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read SUN raster header.
  */
  sun_header.magic=MSBFirstReadLong(image->file);
  do
  {
    /*
      Verify SUN identifier.
    */
    if (sun_header.magic != 0x59a66a95)
      {
        Warning("Not a SUN raster,",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    sun_header.width=MSBFirstReadLong(image->file);
    sun_header.height=MSBFirstReadLong(image->file);
    sun_header.depth=MSBFirstReadLong(image->file);
    sun_header.length=MSBFirstReadLong(image->file);
    sun_header.type=MSBFirstReadLong(image->file);
    sun_header.maptype=MSBFirstReadLong(image->file);
    sun_header.maplength=MSBFirstReadLong(image->file);
    switch (sun_header.maptype)
    {
      case RMT_NONE:
      {
        if (sun_header.depth < 24)
          {
            /*
              Create linear color ramp.
            */
            image->colors=1 << sun_header.depth;
            image->colormap=(ColorPacket *)
              malloc(image->colors*sizeof(ColorPacket));
            if (image->colormap == (ColorPacket *) NULL)
              {
                Warning("Memory allocation error",(char *) NULL);
                return((Image *) NULL);
              }
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=(MaxRGB*i)/(image->colors-1);
              image->colormap[i].green=(MaxRGB*i)/(image->colors-1);
              image->colormap[i].blue=(MaxRGB*i)/(image->colors-1);
            }
          }
        break;
      }
      case RMT_EQUAL_RGB:
      {
        unsigned char
          *sun_colormap;

        /*
          Read SUN raster colormap.
        */
        image->colors=sun_header.maplength/3;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        sun_colormap=(unsigned char *)
          malloc(image->colors*sizeof(unsigned char));
        if ((image->colormap == (ColorPacket *) NULL) ||
            (sun_colormap == (unsigned char *) NULL))
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) ReadData((char *) sun_colormap,1,(int) image->colors,
          image->file);
        for (i=0; i < image->colors; i++)
          image->colormap[i].red=sun_colormap[i];
        (void) ReadData((char *) sun_colormap,1,(int) image->colors,
          image->file);
        for (i=0; i < image->colors; i++)
          image->colormap[i].green=sun_colormap[i];
        (void) ReadData((char *) sun_colormap,1,(int) image->colors,
          image->file);
        for (i=0; i < image->colors; i++)
          image->colormap[i].blue=sun_colormap[i];
        (void) free((char *) sun_colormap);
        break;
      }
      case RMT_RAW:
      {
        unsigned char
          *sun_colormap;

        /*
          Read SUN raster colormap.
        */
        sun_colormap=(unsigned char *)
          malloc(sun_header.maplength*sizeof(unsigned char));
        if (sun_colormap == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) ReadData((char *) sun_colormap,1,(int) sun_header.maplength,
          image->file);
        (void) free((char *) sun_colormap);
        break;
      }
      default:
      {
        Warning("Colormap type is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    }
    sun_data=(unsigned char *) malloc(sun_header.length*sizeof(unsigned char));
    if (sun_data == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    status=ReadData((char *) sun_data,1,(int) sun_header.length,image->file);
    if ((status == False) && (sun_header.type != RT_ENCODED))
      {
        Warning("Unable to read image data",image_info->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    sun_pixels=sun_data;
    if (sun_header.type == RT_ENCODED)
      {
        unsigned int
          width,
          height;

        /*
          Read run-length encoded raster pixels.
        */
        width=sun_header.width*(((sun_header.depth-1) >> 3)+1);
        height=sun_header.height;
        sun_pixels=(unsigned char *) malloc(width*height*sizeof(unsigned char));
        if (sun_pixels == (unsigned char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) SUNDecodeImage(sun_data,sun_pixels,width,height);
        (void) free((char *) sun_data);
      }
    /*
      Initialize image structure.
    */
    image->matte=(sun_header.depth == 32);
    image->class=(sun_header.depth < 24 ? PseudoClass : DirectClass);
    image->columns=sun_header.width;
    image->rows=sun_header.height;
    image->packets=image->columns*image->rows;
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert SUN raster image to runlength-encoded packets.
    */
    p=sun_pixels;
    q=image->pixels;
    if (sun_header.depth == 1)
      for (y=0; y < image->rows; y++)
      {
        /*
          Convert bitmap scanline to runlength-encoded color packets.
        */
        for (x=0; x < (image->columns >> 3); x++)
        {
          for (bit=7; bit >= 0; bit--)
          {
            q->index=((*p) & (0x01 << bit) ? 0x00 : 0x01);
            q->length=0;
            q++;
          }
          p++;
        }
        if ((image->columns % 8) != 0)
          {
            for (bit=7; bit >= (8-(image->columns % 8)); bit--)
            {
              q->index=((*p) & (0x01 << bit) ? 0x00 : 0x01);
              q->length=0;
              q++;
            }
            p++;
          }
        if ((((image->columns/8)+(image->columns % 8 ? 1 : 0)) % 2) != 0)
          p++;
      }
    else
      if (image->class == PseudoClass)
        for (y=0; y < image->rows; y++)
        {
          /*
            Convert PseudoColor scanline to runlength-encoded color packets.
          */
          for (x=0; x < image->columns; x++)
          {
            q->index=(*p++);
            q->length=0;
            q++;
          }
          if ((image->columns % 2) != 0)
            p++;
        }
      else
        for (y=0; y < image->rows; y++)
        {
          /*
            Convert DirectColor scanline to runlength-encoded color packets.
          */
          for (x=0; x < image->columns; x++)
          {
            q->index=(unsigned short) (image->matte ? (*p++) : 0);
            if (sun_header.type == RT_STANDARD)
              {
                q->blue=(*p++);
                q->green=(*p++);
                q->red=(*p++);
              }
            else
              {
                q->red=(*p++);
                q->green=(*p++);
                q->blue=(*p++);
              }
            if (image->colors != 0)
              {
                q->red=image->colormap[q->red].red;
                q->green=image->colormap[q->green].green;
                q->blue=image->colormap[q->blue].blue;
              }
            q->length=0;
            q++;
          }
          if (((image->columns % 2) != 0) && (image->matte == False))
            p++;
        }
    (void) free((char *) sun_pixels);
    if (image->class == PseudoClass)
      {
        SyncImage(image);
        CompressColormap(image);
      }
    /*
      Proceed to next image.
    */
    sun_header.magic=MSBFirstReadLong(image->file);
    if (sun_header.magic == 0x59a66a95)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (sun_header.magic == 0x59a66a95);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d T A R G A I m a g e                                                %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadTARGAImage reads a Truevision Targa image file and returns
%  it.  It allocates the memory necessary for the new Image structure and
%  returns a pointer to the new image.
%
%  The format of the ReadTARGAImage routine is:
%
%      image=ReadTARGAImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadTARGAImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadTARGAImage(image_info)
ImageInfo
  *image_info;
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

  Image
    *image;

  register int
    i,
    x,
    y;

  register RunlengthPacket
    *q;

  TargaHeader
    targa_header;

  unsigned char
    blue,
    green,
    j,
    k,
    red,
    runlength;

  unsigned int
    base,
    flag,
    real,
    skip,
    status,
    true;

  unsigned short
    index;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read TARGA header information.
  */
  status=ReadData((char *) &targa_header.id_length,1,1,image->file);
  targa_header.colormap_type=fgetc(image->file);
  targa_header.image_type=fgetc(image->file);
  if ((status == False) || (targa_header.image_type == 0) ||
      (targa_header.image_type > 11))
    {
      Warning("Not a TARGA image file",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  targa_header.colormap_index=LSBFirstReadShort(image->file);
  targa_header.colormap_length=LSBFirstReadShort(image->file);
  targa_header.colormap_size=fgetc(image->file);
  targa_header.x_origin=LSBFirstReadShort(image->file);
  targa_header.y_origin=LSBFirstReadShort(image->file);
  targa_header.width=LSBFirstReadShort(image->file);
  targa_header.height=LSBFirstReadShort(image->file);
  targa_header.pixel_size=fgetc(image->file);
  targa_header.attributes=fgetc(image->file);
  /*
    Initialize image structure.
  */
  image->matte=targa_header.pixel_size == 32;
  image->columns=targa_header.width;
  image->rows=targa_header.height;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if (image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  if (targa_header.id_length != 0)
    {
      /*
        TARGA image comment.
      */
      image->comments=(char *)
        malloc((targa_header.id_length+1)*sizeof(char));
      if (image->comments == (char *) NULL)
        {
          Warning("Memory allocation error",(char *) NULL);
          DestroyImage(image);
          return((Image *) NULL);
        }
      (void) ReadData(image->comments,1,(int) targa_header.id_length,
        image->file);
      image->comments[targa_header.id_length]='\0';
    }
  if (targa_header.colormap_type != 0)
    {
      /*
        Read TARGA raster colormap.
      */
      if ((targa_header.image_type == TargaRLEColormap) ||
          (targa_header.image_type == TargaRLERGB))
        image->class=PseudoClass;
      image->colors=targa_header.colormap_length;
      image->colormap=(ColorPacket *)
        malloc(image->colors*sizeof(ColorPacket));
      if (image->colormap == (ColorPacket *) NULL)
        {
          Warning("Memory allocation error",(char *) NULL);
          DestroyImage(image);
          return((Image *) NULL);
        }
      for (i=0; i < image->colors; i++)
      {
        switch (targa_header.colormap_size)
        {
          case 8:
          default:
          {
            /*
              Gray scale.
            */
            red=fgetc(image->file);
            green=red;
            blue=red;
            break;
          }
          case 15:
          case 16:
          {
            /*
              5 bits each of red green and blue.
            */
            j=fgetc(image->file);
            k=fgetc(image->file);
            red=(unsigned char) ((MaxRGB*((int) (k & 0x7c) >> 2))/31);
            green=(unsigned char)
              ((MaxRGB*(((int) (k & 0x03) << 3)+((int) (j & 0xe0) >> 5)))/31);
            blue=(unsigned char) ((MaxRGB*((int) (j & 0x1f)))/31);
            break;
          }
          case 32:
          case 24:
          {
            /*
              8 bits each of blue green and red.
            */
            blue=fgetc(image->file);
            green=fgetc(image->file);
            red=fgetc(image->file);
            break;
          }
        }
        image->colormap[i].red=red;
        image->colormap[i].green=green;
        image->colormap[i].blue=blue;
      }
    }
  /*
    Convert TARGA pixels to runlength-encoded packets.
  */
  base=0;
  flag=0;
  index=0;
  skip=False;
  real=0;
  runlength=0;
  true=0;
  for (y=0; y < image->rows; y++)
  {
    real=true;
    if (((unsigned char) (targa_header.attributes & 0x20) >> 5) == 0)
      real=image->rows-real-1;
    q=image->pixels+(real*image->columns);
    for (x=0; x < image->columns; x++)
    {
      if ((targa_header.image_type == TargaRLEColormap) ||
          (targa_header.image_type == TargaRLERGB) ||
          (targa_header.image_type == TargaRLEMonochrome))
        if (runlength != 0)
          {
            runlength--;
            skip=flag != 0;
          }
        else
          {
            status=ReadData((char *) &runlength,1,1,image->file);
            if (status == False)
              {
                Warning("Unable to read image data",image_info->filename);
                DestroyImage(image);
                return((Image *) NULL);
              }
            flag=runlength & 0x80;
            if (flag != 0)
              runlength-=128;
            skip=False;
          }
      if (!skip)
        switch (targa_header.pixel_size)
        {
          case 8:
          default:
          {
            /*
              Gray scale.
            */
            index=fgetc(image->file);
            if (targa_header.colormap_type == 0)
              {
                red=(unsigned char) index;
                green=(unsigned char) index;
                blue=(unsigned char) index;
              }
            else
              {
                red=image->colormap[index].red;
                green=image->colormap[index].green;
                blue=image->colormap[index].blue;
              }
            break;
          }
          case 15:
          case 16:
          {
            /*
              5 bits each of red green and blue.
            */
            j=fgetc(image->file);
            k=fgetc(image->file);
            red=(unsigned char) ((MaxRGB*((int) (k & 0x7c) >> 2))/31);
            green=(unsigned char)
              ((MaxRGB*(((int) (k & 0x03) << 3)+((int) (j & 0xe0) >> 5)))/31);
            blue=(unsigned char) ((MaxRGB*((int) (j & 0x1f)))/31);
            index=((unsigned int) k << 8)+j;
            break;
          }
          case 24:
          case 32:
          {
            /*
              8 bits each of blue green and red.
            */
            blue=fgetc(image->file);
            green=fgetc(image->file);
            red=fgetc(image->file);
            if (targa_header.pixel_size == 32)
              index=fgetc(image->file);
            break;
          }
        }
      if (status == False)
        {
          Warning("Unable to read image data",image_info->filename);
          DestroyImage(image);
          return((Image *) NULL);
        }
      q->red=red;
      q->green=green;
      q->blue=blue;
      q->index=index;
      q->length=0;
      q++;
    }
    if (((unsigned char) (targa_header.attributes & 0xc0) >> 6) == 4)
      true+=4;
    else
      if (((unsigned char) (targa_header.attributes & 0xc0) >> 6) == 2)
        true+=2;
      else
        true++;
    if (true >= image->rows)
      {
        base++;
        true=base;
      }
  }
  if (image->class == PseudoClass)
    SyncImage(image);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d T E X T I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadTEXTImage reads a text file and returns it as an image.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadTEXTImage routine is:
%
%      image=ReadTEXTImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadTEXTImage returns a pointer to the image after
%      reading. A null image is returned if there is a a memory shortage or if
%      the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadTEXTImage(image_info)
ImageInfo
  *image_info;
{
  char
    *resource_value,
    *text_status,
    text[MaxTextLength];

  Display
    *display;

  Image
    *image;

  int
    status,
    offset,
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *p;

  RunlengthPacket
    background_color;

  unsigned int
    height,
    width;

  XAnnotateInfo
    annotate_info;

  XFontStruct
    *font_info;

  XPixelInfo
    pixel_info;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Open X server connection.
  */
  display=XOpenDisplay(image_info->server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",
        XDisplayName(image_info->server_name));
      DestroyImage(image);
      return((Image *) NULL);
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
  XGetAnnotateInfo(&annotate_info);
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
      /*
        Initialize font info.
      */
      if (image_info->font != (char *) NULL)
        resource_info.font=image_info->font;
      font_info=XBestFont(display,&resource_info);
      if (font_info == (XFontStruct *) NULL)
        Warning("Unable to load font",resource_info.font);
      annotate_info.font_info=font_info;
      annotate_info.height=font_info->ascent+font_info->descent;
    }
  if ((map_info == (XStandardColormap *) NULL) ||
      (visual_info == (XVisualInfo *) NULL) ||
      (font_info == (XFontStruct *) NULL))
    {
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        font_info,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize Standard Colormap.
  */
  XGetMapInfo(visual_info,XDefaultColormap(display,visual_info->screen),
    map_info);
  XGetPixelInfo(display,visual_info,map_info,&resource_info,(Image *) NULL,
    &pixel_info);
  pixel_info.annotate_context=XDefaultGC(display,visual_info->screen);
  pixel_info.annotate_index=1;
  background_color.red=pixel_info.background_color.red >> 8;
  background_color.green=pixel_info.background_color.red >> 8;
  background_color.blue=pixel_info.background_color.red >> 8;
  background_color.index=0;
  background_color.length=0;
  /*
    Initialize Image structure.
  */
  (void) XParseGeometry(TextPageGeometry,&x,&y,&width,&height);
  (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  image->class=PseudoClass;
  image->colors=2;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if ((image->pixels == (RunlengthPacket *) NULL) ||
      (image->colormap == (ColorPacket *) NULL))
    {
      Warning("Unable to allocate image","Memory allocation error");
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        font_info,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize colormap.
  */
  image->colormap[0].red=pixel_info.background_color.red >> 8;
  image->colormap[0].green=pixel_info.background_color.green >> 8;
  image->colormap[0].blue=pixel_info.background_color.blue >> 8;
  image->colormap[1].red=pixel_info.foreground_color.red >> 8;
  image->colormap[1].green=pixel_info.foreground_color.green >> 8;
  image->colormap[1].blue=pixel_info.foreground_color.blue >> 8;
  /*
    Initialize text image to background color.
  */
  p=image->pixels;
  for (i=0; i < image->packets; i++)
    *p++=background_color;
  /*
    Annotate the text image.
  */
  annotate_info.text=(char *) malloc(MaxTextLength*sizeof(char));
  if (annotate_info.text == (char *) NULL)
    {
      Warning("Unable to read image","Memory allocation failed");
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        font_info,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  text[MaxTextLength-1]='\0';
  text_status=fgets(text,MaxTextLength,image->file);
  if ((int) strlen(text) > 0)
    text[strlen(text)-1]='\0';
  offset=0;
  while (text_status != (char *) NULL)
  {
    *annotate_info.text='\0';
    if (*text != '\0')
      {
        /*
          Compute width of text.
        */
        (void) strcpy(annotate_info.text,text);
        annotate_info.width=
          XTextWidth(font_info,annotate_info.text,strlen(annotate_info.text));
        if ((annotate_info.width+4) >= image->columns)
          {
            /*
              Reduce text until width is within bounds.
            */
            i=strlen(annotate_info.text);
            for (; (annotate_info.width+(x << 1)) >= image->columns; i--)
              annotate_info.width=
                XTextWidth(font_info,annotate_info.text,(unsigned int) i);
            annotate_info.text[i]='\0';
            while ((i > 0) && !isspace(annotate_info.text[i]))
              i--;
            if (i > 0)
              annotate_info.text[i]='\0';
            annotate_info.width=XTextWidth(font_info,annotate_info.text,
              strlen(annotate_info.text));
          }
        /*
          Annotate image with text.
        */
        (void) sprintf(annotate_info.geometry,"%ux%u%+d%+d",
          annotate_info.width,annotate_info.height,x,y+offset);
        status=XAnnotateImage(display,&pixel_info,&annotate_info,False,image);
        if (status == 0)
          {
            Warning("Unable to annotate image","Memory allocation error");
            break;
          }
      }
    /*
      Get next string.
    */
    if (strlen(text) != strlen(annotate_info.text))
      (void) strcpy(text,text+strlen(annotate_info.text)+1);
    else
      {
        text_status=fgets(text,MaxTextLength,image->file);
        if ((int) strlen(text) > 0)
          text[strlen(text)-1]='\0';
      }
    offset+=annotate_info.height;
    if ((text_status != (char *) NULL) &&
        (((y << 1)+offset+annotate_info.height) >= image->rows))
      {
        /*
          Page is full-- allocate next image structure.
        */
        image->orphan=True;
        image->next=CopyImage(image,image->columns,image->rows,False);
        image->orphan=False;
        if (image->next == (Image *) NULL)
          {
            Warning("Unable to annotate image","Memory allocation error");
            break;
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
        /*
          Initialize text image to background color.
        */
        p=image->pixels;
        for (i=0; i < image->packets; i++)
          *p++=background_color;
        offset=0;
      }
  }
  /*
    Force class to PseudoClass.
  */
  while (image->previous != (Image *) NULL)
  {
    image->class=PseudoClass;
    image=image->previous;
  }
  image->class=PseudoClass;
  CloseImage(image);
  /*
    Free resources.
  */
  (void) free((char *) annotate_info.text);
  XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
    font_info,&resource_info,(XWindowInfo *) NULL);
  return(image);
}

#ifdef HasTIFF
#include "tiff.h"
#include "tiffio.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d T I F F I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadTIFFImage reads a Tagged image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadTIFFImage routine is:
%
%      image=ReadTIFFImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadTIFFImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadTIFFImage(image_info)
ImageInfo
  *image_info;
{
  char
    *comment;

  Image
    *image;

  int
    range;

  register int
    i,
    quantum,
    x,
    y;

  register RunlengthPacket
    *q;

  TIFF
    *tiff;

  unsigned int
    status;

  unsigned long
    height,
    width;

  unsigned short
    bits_per_sample,
    max_sample_value,
    min_sample_value,
    photometric,
    samples_per_pixel;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open TIFF image tiff.
  */
  tiff=TIFFOpen(image->filename,"rb");
  if (tiff == (TIFF *) NULL)
    {
      Warning("Unable to open tiff image",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  do
  {
    if (image_info->verbose)
      TIFFPrintDirectory(tiff,stderr,False);
    /*
      Allocate memory for the image and pixel buffer.
    */
    TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH,&width);
    TIFFGetField(tiff,TIFFTAG_IMAGELENGTH,&height);
    for (quantum=1; quantum <= 16; quantum<<=1)
    {
      image->columns=width/quantum;
      image->rows=height/quantum;
      image->packets=image->columns*image->rows;
      image->pixels=(RunlengthPacket *)
        malloc(image->packets*sizeof(RunlengthPacket));
      if ((image->pixels != (RunlengthPacket *) NULL))
        break;
    }
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Unable to allocate memory",(char *) NULL);
        DestroyImages(image);
        TIFFClose(tiff);
        return((Image *) NULL);
      }
    TIFFGetFieldDefaulted(tiff,TIFFTAG_BITSPERSAMPLE,&bits_per_sample);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_MINSAMPLEVALUE,&min_sample_value);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_MAXSAMPLEVALUE,&max_sample_value);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_PHOTOMETRIC,&photometric);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_SAMPLESPERPIXEL,&samples_per_pixel);
    comment=(char *) NULL;
    TIFFGetField(tiff,TIFFTAG_IMAGEDESCRIPTION,&comment);
    if (comment != (char *) NULL)
      if ((int) strlen(comment) > 4)
        {
          image->comments=(char *)
            malloc((unsigned int) (strlen(comment)+1)*sizeof(char));
          if (image->comments == (char *) NULL)
            {
              Warning("Unable to allocate memory",(char *) NULL);
              DestroyImages(image);
              TIFFClose(tiff);
              return((Image *) NULL);
            }
          (void) strcpy(image->comments,comment);
        }
    range=max_sample_value-min_sample_value;
    if ((bits_per_sample > 8) || (samples_per_pixel > 1) || TIFFIsTiled(tiff))
      {
        register unsigned long
          *p,
          *pixels;

        /*
          Convert TIFF image to DirectClass MIFF image.
        */
        image->matte=samples_per_pixel > 3;
        pixels=(unsigned long *) malloc(image->packets*sizeof(unsigned long));
        if (pixels == (unsigned long *) NULL)
          {
            Warning("Unable to allocate memory",(char *) NULL);
            DestroyImages(image);
            TIFFClose(tiff);
            return((Image *) NULL);
          }
        if (quantum > 1)
          Warning("Not enough memory","cropping required");
        status=TIFFReadRGBAImage(tiff,image->columns,image->rows,pixels,0);
        if (status == False)
          {
            Warning("Unable to read TIFF image",(char *) NULL);
            (void) free((char *) pixels);
            DestroyImages(image);
            TIFFClose(tiff);
            return((Image *) NULL);
          }
        /*
          Convert image to DirectClass runlength-encoded packets.
        */
        q=image->pixels;
        for (y=image->rows-1; y >= 0; y--)
        {
          p=pixels+y*image->columns;
          for (x=0; x < image->columns; x++)
          {
            q->red=TIFFGetR(*p);
            q->green=TIFFGetG(*p);
            q->blue=TIFFGetB(*p);
            q->index=(unsigned short) (image->matte ? TIFFGetA(*p) : 0);
            q->length=0;
            p++;
            q++;
          }
        }
        (void) free((char *) pixels);
        if (samples_per_pixel == 1)
          QuantizeImage(image,(unsigned int) range,8,False,RGBColorspace,True);
      }
    else
      {
        unsigned char
          *p,
          *r,
          *quantum_scanline,
          *scanline;

        /*
          Convert TIFF image to PseudoClass MIFF image.
        */
        image->class=PseudoClass;
        image->colors=range+1;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        quantum_scanline=(unsigned char *) malloc(width*sizeof(unsigned char));
        scanline=(unsigned char *) malloc(TIFFScanlineSize(tiff));
        if ((image->colormap == (ColorPacket *) NULL) ||
            (quantum_scanline == (unsigned char *) NULL) ||
            (scanline == (unsigned char *) NULL))
          {
            Warning("Unable to allocate memory",(char *) NULL);
            DestroyImages(image);
            TIFFClose(tiff);
            return((Image *) NULL);
          }
        /*
          Create colormap.
        */
        switch (photometric)
        {
          case PHOTOMETRIC_MINISBLACK:
          {
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=(MaxRGB*i)/range;
              image->colormap[i].green=(MaxRGB*i)/range;
              image->colormap[i].blue=(MaxRGB*i)/range;
            }
            break;
          }
          case PHOTOMETRIC_MINISWHITE:
          {
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=((range-i)*MaxRGB)/range;
              image->colormap[i].green=((range-i)*MaxRGB)/range;
              image->colormap[i].blue=((range-i)*MaxRGB)/range;
            }
            break;
          }
          case PHOTOMETRIC_PALETTE:
          {
            unsigned short
              *blue_colormap,
              *green_colormap,
              *red_colormap;

            TIFFGetField(tiff,TIFFTAG_COLORMAP,&red_colormap,&green_colormap,
              &blue_colormap);
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=((int) red_colormap[i]*MaxRGB)/65535;
              image->colormap[i].green=((int) green_colormap[i]*MaxRGB)/65535;
              image->colormap[i].blue=((int) blue_colormap[i]*MaxRGB)/65535;
            }
            break;
          }
          default:
            break;
        }
        /*
          Convert image to PseudoClass runlength-encoded packets.
        */
        if (quantum > 1)
          Warning("Not enough memory","subsampling required");
        q=image->pixels;
        for (y=0; y < image->rows; y++)
        {
          for (i=0; i < quantum; i++)
            TIFFReadScanline(tiff,scanline,y*quantum+i,0);
          p=scanline;
          r=quantum_scanline;
          switch (photometric)
          {
            case PHOTOMETRIC_MINISBLACK:
            case PHOTOMETRIC_MINISWHITE:
            {
              switch (bits_per_sample)
              {
                case 1:
                {
                  register int
                    bit;

                  for (x=0; x < (width-7); x+=8)
                  {
                    for (bit=7; bit >= 0; bit--)
                      *r++=((*p) & (0x01 << bit) ? 0x01 : 0x00);
                    p++;
                  }
                  if ((width % 8) != 0)
                    {
                      for (bit=7; bit >= (8-(width % 8)); bit--)
                        *r++=((*p) & (0x01 << bit) ? 0x00 : 0x01);
                      p++;
                    }
                  break;
                }
                case 2:
                {
                  for (x=0; x < (width-3); x+=4)
                  {
                    *r++=(*p >> 6) & 0x3;
                    *r++=(*p >> 4) & 0x3;
                    *r++=(*p >> 2) & 0x3;
                    *r++=(*p) & 0x3;
                    p++;
                  }
                  if ((width % 4) != 0)
                    {
                      for (i=3; i >= (4-(width % 4)); i--)
                        *r++=(*p >> (i*2)) & 0x03;
                      p++;
                    }
                  break;
                }
                case 4:
                {
                  for (x=0; x < (width-1); x+=2)
                  {
                    *r++=(*p >> 4) & 0xf;
                    *r++=(*p) & 0xf;
                    p++;
                  }
                  if ((width % 2) != 0)
                    {
                      *r++=(*p >> 4) & 0xf;
                      p++;
                    }
                  break;
                }
                case 8:
                {
                  for (x=0; x < width; x++)
                  {
                    *r++=(*p);
                    p++;
                  }
                  break;
                }
                default:
                  break;
              }
              break;
            }
            case PHOTOMETRIC_PALETTE:
            {
              for (x=0; x < width; x++)
              {
                *r++=(*p);
                p++;
              }
              break;
            }
            default:
              break;
          }
          /*
            Subsample quantum scanline.
          */
          r=quantum_scanline;
          for (x=0; x < image->columns; x++)
          {
            q->index=(*r);
            q->length=0;
            q++;
            r+=quantum;
          }
        }
        (void) free((char *) quantum_scanline);
        (void) free((char *) scanline);
      }
    if (image->class == PseudoClass)
      {
        SyncImage(image);
        CompressColormap(image);
      }
    /*
      Proceed to next image.
    */
    status=TIFFReadDirectory(tiff);
    if (status == True)
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while (status == True);
  TIFFClose(tiff);
  while (image->previous != (Image *) NULL)
    image=image->previous;
  return(image);
}
#else
static Image *ReadTIFFImage(image_info)
ImageInfo
  *image_info;
{
  Warning("TIFF library is not available",image_info->filename);
  return(ReadMIFFImage(image_info));
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d V I C A R I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadVICARImage reads a VICAR image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadVICARImage routine is:
%
%      image=ReadVICARImage(image_info)
%
%  A description of each parameter follows:
%
%    o image: Function ReadVICARImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or if
%      the image cannot be read.
%
%    o filename: Specifies the name of the image to read.
%
%
*/
static Image *ReadVICARImage(image_info)
ImageInfo
  *image_info;
{
  char
    keyword[MaxTextLength],
    value[MaxTextLength];

  Image
    *image;

  long
    count;

  register int
    c,
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *vicar_pixels;

  unsigned int
    header_length,
    status,
    value_expected;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Decode image header.
  */
  c=fgetc(image->file);
  count=1;
  if (c == EOF)
    {
      DestroyImage(image);
      return((Image *) NULL);
    }
  header_length=0;
  while (isgraph(c) && ((image->columns*image->rows) == 0))
  {
    if (!isalnum(c))
      {
        c=fgetc(image->file);
        count++;
      }
    else
      {
        register char
          *p;

        /*
          Determine a keyword and its value.
        */
        p=keyword;
        do
        {
          if ((p-keyword) < (MaxTextLength-1))
            *p++=(char) c;
          c=fgetc(image->file);
          count++;
        } while (isalnum(c) || (c == '_'));
        *p='\0';
        value_expected=False;
        while (isspace(c) || (c == '='))
        {
          if (c == '=')
            value_expected=True;
          c=fgetc(image->file);
          count++;
        }
        if (value_expected == False)
          continue;
        p=value;
        while (isalnum(c))
        {
          if ((p-value) < (MaxTextLength-1))
            *p++=(char) c;
          c=fgetc(image->file);
          count++;
        }
        *p='\0';
        /*
          Assign a value to the specified keyword.
        */
        if (strcmp(keyword,"LABEL_RECORDS") == 0)
          header_length=(unsigned int) atoi(value);
        if (strcmp(keyword,"LBLSIZE") == 0)
          header_length=(unsigned int) atoi(value);
        if (strcmp(keyword,"RECORD_BYTES") == 0)
          image->columns=(unsigned int) atoi(value);
        if (strcmp(keyword,"NS") == 0)
          image->columns=(unsigned int) atoi(value);
        if (strcmp(keyword,"LINES") == 0)
          image->rows=(unsigned int) atoi(value);
        if (strcmp(keyword,"NL") == 0)
          image->rows=(unsigned int) atoi(value);
      }
    while (isspace(c))
    {
      c=fgetc(image->file);
      count++;
    }
  }
  /*
    Read the rest of the header.
  */
  while (count < header_length)
  {
    c=fgetc(image->file);
    count++;
  }
  /*
    Verify that required image information is defined.
  */
  if ((image->columns*image->rows) == 0)
    {
      Warning("Incorrect image header in file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Create linear colormap.
  */
  image->class=PseudoClass;
  image->colors=256;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if (image->colormap == (ColorPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  for (i=0; i < image->colors; i++)
  {
    image->colormap[i].red=(unsigned char) i;
    image->colormap[i].green=(unsigned char) i;
    image->colormap[i].blue=(unsigned char) i;
  }
  /*
    Initialize image structure.
  */
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  vicar_pixels=(unsigned char *) malloc(image->packets*sizeof(unsigned char));
  if ((image->pixels == (RunlengthPacket *) NULL) ||
      (vicar_pixels == (unsigned char *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert VICAR pixels to runlength-encoded packets.
  */
  status=ReadData((char *) vicar_pixels,1,(int) image->packets,image->file);
  if (status == False)
    {
      Warning("Insufficient image data in file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert VICAR pixels to runlength-encoded packets.
  */
  p=vicar_pixels;
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    q->red=(*p);
    q->green=(*p);
    q->blue=(*p);
    q->index=(unsigned short) *p;
    q->length=0;
    p++;
    q++;
  }
  (void) free((char *) vicar_pixels);
  CompressColormap(image);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d V I F F I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadVIFFImage reads a Khoros Visualization image file and returns
%  it.  It allocates the memory necessary for the new Image structure and
%  returns a pointer to the new image.
%
%  The format of the ReadVIFFImage routine is:
%
%      image=ReadVIFFImage(image_info)
%
%  A description of each parameter follows:
%
%    o image: Function ReadVIFFImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or if
%      the image cannot be read.
%
%    o filename: Specifies the name of the image to read.
%
%
*/
static Image *ReadVIFFImage(image_info)
ImageInfo
  *image_info;
{
#define VFF_CM_genericRGB  15
#define VFF_CM_ntscRGB  1
#define VFF_CM_NONE  0
#define VFF_DEP_DECORDER  0x4
#define VFF_DEP_NSORDER  0x8
#define VFF_DES_RAW  0
#define VFF_LOC_IMPLICIT  1
#define VFF_MAPTYP_NONE  0
#define VFF_MAPTYP_1_BYTE  1
#define VFF_MS_NONE  0
#define VFF_MS_ONEPERBAND  1
#define VFF_MS_SHARED  3
#define VFF_TYP_BIT  0
#define VFF_TYP_1_BYTE  1
#define VFF_TYP_2_BYTE  2
#define VFF_TYP_4_BYTE  4

  typedef struct _ViffHeader
  {
    unsigned char
      identifier,
      file_type,
      release,
      version,
      machine_dependency,
      reserve[3];

    char
      comment[512];

    unsigned long
      rows,
      columns,
      subrows;

    long
      x_offset,
      y_offset;

    float
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

  Image
    *image;

  register int
    bit,
    i,
    x,
    y;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    buffer[7],
    *viff_pixels;

  unsigned int
    bytes_per_pixel,
    status;

  unsigned long
    packets;

  ViffHeader
    viff_header;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read VIFF header (1024 bytes).
  */
  status=ReadData((char *) &viff_header.identifier,1,1,image->file);
  do
  {
    /*
      Verify VIFF identifier.
    */
    if ((status == False) || ((unsigned char) viff_header.identifier != 0xab))
      {
        Warning("Not a VIFF raster,",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Initialize VIFF image.
    */
    (void) ReadData((char *) buffer,1,7,image->file);
    viff_header.file_type=buffer[0];
    viff_header.release=buffer[1];
    viff_header.version=buffer[2];
    viff_header.machine_dependency=buffer[3];
    (void) ReadData((char *) viff_header.comment,1,512,image->file);
    viff_header.comment[511]='\0';
    if ((int) strlen(viff_header.comment) > 4)
      {
        image->comments=(char *)
          malloc((unsigned int) (strlen(viff_header.comment)+1)*sizeof(char));
        if (image->comments == (char *) NULL)
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->comments,viff_header.comment);
      }
    if ((viff_header.machine_dependency == VFF_DEP_DECORDER) ||
        (viff_header.machine_dependency == VFF_DEP_NSORDER))
      {
        viff_header.rows=LSBFirstReadLong(image->file);
        viff_header.columns=LSBFirstReadLong(image->file);
        viff_header.subrows=LSBFirstReadLong(image->file);
        viff_header.x_offset=LSBFirstReadLong(image->file);
        viff_header.y_offset=LSBFirstReadLong(image->file);
        viff_header.x_pixel_size=(float) LSBFirstReadLong(image->file);
        viff_header.y_pixel_size=(float) LSBFirstReadLong(image->file);
        viff_header.location_type=LSBFirstReadLong(image->file);
        viff_header.location_dimension=LSBFirstReadLong(image->file);
        viff_header.number_of_images=LSBFirstReadLong(image->file);
        viff_header.number_data_bands=LSBFirstReadLong(image->file);
        viff_header.data_storage_type=LSBFirstReadLong(image->file);
        viff_header.data_encode_scheme=LSBFirstReadLong(image->file);
        viff_header.map_scheme=LSBFirstReadLong(image->file);
        viff_header.map_storage_type=LSBFirstReadLong(image->file);
        viff_header.map_rows=LSBFirstReadLong(image->file);
        viff_header.map_columns=LSBFirstReadLong(image->file);
        viff_header.map_subrows=LSBFirstReadLong(image->file);
        viff_header.map_enable=LSBFirstReadLong(image->file);
        viff_header.maps_per_cycle=LSBFirstReadLong(image->file);
        viff_header.color_space_model=LSBFirstReadLong(image->file);
      }
    else
      {
        viff_header.rows=MSBFirstReadLong(image->file);
        viff_header.columns=MSBFirstReadLong(image->file);
        viff_header.subrows=MSBFirstReadLong(image->file);
        viff_header.x_offset=MSBFirstReadLong(image->file);
        viff_header.y_offset=MSBFirstReadLong(image->file);
        viff_header.x_pixel_size=(float) MSBFirstReadLong(image->file);
        viff_header.y_pixel_size=(float) MSBFirstReadLong(image->file);
        viff_header.location_type=MSBFirstReadLong(image->file);
        viff_header.location_dimension=MSBFirstReadLong(image->file);
        viff_header.number_of_images=MSBFirstReadLong(image->file);
        viff_header.number_data_bands=MSBFirstReadLong(image->file);
        viff_header.data_storage_type=MSBFirstReadLong(image->file);
        viff_header.data_encode_scheme=MSBFirstReadLong(image->file);
        viff_header.map_scheme=MSBFirstReadLong(image->file);
        viff_header.map_storage_type=MSBFirstReadLong(image->file);
        viff_header.map_rows=MSBFirstReadLong(image->file);
        viff_header.map_columns=MSBFirstReadLong(image->file);
        viff_header.map_subrows=MSBFirstReadLong(image->file);
        viff_header.map_enable=MSBFirstReadLong(image->file);
        viff_header.maps_per_cycle=MSBFirstReadLong(image->file);
        viff_header.color_space_model=MSBFirstReadLong(image->file);
      }
    for (i=0; i < 420; i++)
      (void) fgetc(image->file);
    /*
      Verify that we can read this VIFF image.
    */
    if ((viff_header.columns*viff_header.rows) == 0)
      {
        Warning("Image column or row size is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if ((viff_header.data_storage_type != VFF_TYP_BIT) &&
        (viff_header.data_storage_type != VFF_TYP_1_BYTE) &&
        (viff_header.data_storage_type != VFF_TYP_2_BYTE) &&
        (viff_header.data_storage_type != VFF_TYP_4_BYTE))
      {
        Warning("Data storage type is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (viff_header.data_encode_scheme != VFF_DES_RAW)
      {
        Warning("Data encoding scheme is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if ((viff_header.map_storage_type != VFF_MAPTYP_NONE) &&
        (viff_header.map_storage_type != VFF_MAPTYP_1_BYTE))
      {
        Warning("Map storage type is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if ((viff_header.color_space_model != VFF_CM_NONE) &&
        (viff_header.color_space_model != VFF_CM_ntscRGB) &&
        (viff_header.color_space_model != VFF_CM_genericRGB))
      {
        Warning("Color space model is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (viff_header.location_type != VFF_LOC_IMPLICIT)
      {
        Warning("Location type is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    if (viff_header.number_of_images != 1)
      {
        Warning("Number of images is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    switch (viff_header.map_scheme)
    {
      case VFF_MS_NONE:
      {
        if (viff_header.number_data_bands < 3)
          {
            /*
              Create linear color ramp.
            */
            if (viff_header.data_storage_type == VFF_TYP_BIT)
              image->colors=2;
            else
              image->colors=1 << (viff_header.number_data_bands*8);
            image->colormap=(ColorPacket *)
              malloc(image->colors*sizeof(ColorPacket));
            if (image->colormap == (ColorPacket *) NULL)
              {
                Warning("Memory allocation error",(char *) NULL);
                return((Image *) NULL);
              }
            for (i=0; i < image->colors; i++)
            {
              image->colormap[i].red=(MaxRGB*i)/(image->colors-1);
              image->colormap[i].green=(MaxRGB*i)/(image->colors-1);
              image->colormap[i].blue=(MaxRGB*i)/(image->colors-1);
            }
          }
        break;
      }
      case VFF_MS_ONEPERBAND:
      case VFF_MS_SHARED:
      {
        unsigned char
          *viff_colormap;

        /*
          Read VIFF raster colormap.
        */
        image->colors=viff_header.map_columns;
        image->colormap=(ColorPacket *)
          malloc(image->colors*sizeof(ColorPacket));
        viff_colormap=(unsigned char *)
          malloc(image->colors*sizeof(unsigned char));
        if ((image->colormap == (ColorPacket *) NULL) ||
            (viff_colormap == (unsigned char *) NULL))
          {
            Warning("Memory allocation error",(char *) NULL);
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) ReadData((char *) viff_colormap,1,(int) image->colors,
          image->file);
        for (i=0; i < image->colors; i++)
        {
          image->colormap[i].red=viff_colormap[i];
          image->colormap[i].green=viff_colormap[i];
          image->colormap[i].blue=viff_colormap[i];
        }
        if (viff_header.map_rows > 1)
          {
            (void) ReadData((char *) viff_colormap,1,(int) image->colors,
              image->file);
            for (i=0; i < image->colors; i++)
              image->colormap[i].green=viff_colormap[i];
          }
        if (viff_header.map_rows > 2)
          {
            (void) ReadData((char *) viff_colormap,1,(int) image->colors,
              image->file);
            for (i=0; i < image->colors; i++)
              image->colormap[i].blue=viff_colormap[i];
          }
        (void) free((char *) viff_colormap);
        break;
      }
      default:
      {
        Warning("Colormap type is not supported",image->filename);
        DestroyImages(image);
        return((Image *) NULL);
      }
    }
    /*
      Allocate VIFF pixels.
    */
    bytes_per_pixel=1;
    if (viff_header.data_storage_type == VFF_TYP_2_BYTE)
      bytes_per_pixel=2;
    if (viff_header.data_storage_type == VFF_TYP_4_BYTE)
      bytes_per_pixel=4;
    if (viff_header.data_storage_type == VFF_TYP_BIT)
      packets=((viff_header.columns+7) >> 3)*viff_header.rows;
    else
      packets=
        viff_header.columns*viff_header.rows*viff_header.number_data_bands;
    viff_pixels=(unsigned char *)
      malloc(bytes_per_pixel*packets*sizeof(unsigned char));
    if (viff_pixels == (unsigned char *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    (void) ReadData((char *) viff_pixels,(int) bytes_per_pixel,(int) packets,
      image->file);
    if (viff_header.data_storage_type == VFF_TYP_2_BYTE)
      {
        register short int
          *p;

        register unsigned char
          *q;

        short int
          max_value,
          min_value,
          value;

        unsigned long
          scale_factor;

        /*
          Ensure the header byte-order is most-significant byte first.
        */
        if ((viff_header.machine_dependency == VFF_DEP_DECORDER) ||
            (viff_header.machine_dependency == VFF_DEP_NSORDER))
          MSBFirstOrderShort((char *) &viff_header,
            (unsigned int) (bytes_per_pixel*packets));
        /*
          Determine scale factor.
        */
        p=(short int *) viff_pixels;
        max_value=(*p);
        min_value=(*p);
        for (i=0; i < packets; i++)
        {
          if (*p > max_value)
            max_value=(*p);
          else
            if (*p < min_value)
              min_value=(*p);
          p++;
        }
        if ((min_value == 0) && (max_value == 0))
          scale_factor=0;
        else
          if (min_value == max_value)
            {
              scale_factor=UpShift(MaxRGB)/min_value;
              min_value=0;
            }
          else
            scale_factor=UpShift(MaxRGB)/(max_value-min_value);
        /*
          Scale integer pixels to [0..MaxRGB].
        */
        p=(short int *) viff_pixels;
        q=viff_pixels;
        for (i=0; i < packets; i++)
        {
          value=DownShift((*p-min_value)*scale_factor);
          if (value > MaxRGB)
            value=MaxRGB;
          else
            if (value < 0)
              value=0;
          *q=(unsigned char) value;
          p++;
          q++;
        }
      }
    if (viff_header.data_storage_type == VFF_TYP_4_BYTE)
      {
        int
          max_value,
          min_value,
          value;

        register int
          *p;

        register unsigned char
          *q;

        unsigned long
          scale_factor;

        /*
          Ensure the header byte-order is most-significant byte first.
        */
        if ((viff_header.machine_dependency == VFF_DEP_DECORDER) ||
            (viff_header.machine_dependency == VFF_DEP_NSORDER))
          MSBFirstOrderLong((char *) &viff_header,
            (unsigned int) (bytes_per_pixel*packets));
        /*
          Determine scale factor.
        */
        p=(int *) viff_pixels;
        max_value=(*p);
        min_value=(*p);
        for (i=0; i < packets; i++)
        {
          if (*p > max_value)
            max_value=(*p);
          else
            if (*p < min_value)
              min_value=(*p);
          p++;
        }
        if ((min_value == 0) && (max_value == 0))
          scale_factor=0;
        else
          if (min_value == max_value)
            {
              scale_factor=UpShift(MaxRGB)/min_value;
              min_value=0;
            }
          else
            scale_factor=UpShift(MaxRGB)/(max_value-min_value);
        /*
          Scale integer pixels to [0..MaxRGB].
        */
        p=(int *) viff_pixels;
        q=viff_pixels;
        for (i=0; i < packets; i++)
        {
          value=DownShift((*p-min_value)*scale_factor);
          if (value > MaxRGB)
            value=MaxRGB;
          else
            if (value < 0)
              value=0;
          *q=(unsigned char) value;
          p++;
          q++;
        }
      }
    /*
      Initialize image structure.
    */
    image->matte=(viff_header.number_data_bands == 4);
    image->class=
      (viff_header.number_data_bands < 3 ? PseudoClass : DirectClass);
    image->columns=viff_header.rows;
    image->rows=viff_header.columns;
    image->packets=image->columns*image->rows;
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      {
        Warning("Memory allocation error",(char *) NULL);
        DestroyImages(image);
        return((Image *) NULL);
      }
    /*
      Convert VIFF raster image to runlength-encoded packets.
    */
    p=viff_pixels;
    q=image->pixels;
    if (viff_header.data_storage_type == VFF_TYP_BIT)
      {
        unsigned int
          polarity;

        /*
          Convert bitmap scanline to runlength-encoded color packets.
        */
        polarity=(viff_header.machine_dependency == VFF_DEP_DECORDER) ||
          (viff_header.machine_dependency == VFF_DEP_NSORDER);
        for (y=0; y < image->rows; y++)
        {
          /*
            Convert bitmap scanline to runlength-encoded color packets.
          */
          for (x=0; x < (image->columns >> 3); x++)
          {
            for (bit=0; bit < 8; bit++)
            {
              q->index=((*p) & (0x01 << bit) ? polarity : !polarity);
              q->length=0;
              q++;
            }
            p++;
          }
          if ((image->columns % 8) != 0)
            {
              for (bit=0; bit < (image->columns % 8); bit++)
              {
                q->index=((*p) & (0x01 << bit) ? polarity : !polarity);
                q->length=0;
                q++;
              }
              p++;
            }
        }
      }
    else
      if (image->class == PseudoClass)
        for (y=0; y < image->rows; y++)
        {
          /*
            Convert PseudoColor scanline to runlength-encoded color packets.
          */
          for (x=0; x < image->columns; x++)
          {
            q->index=(*p++);
            q->length=0;
            q++;
          }
        }
      else
        {
          unsigned long
            offset;

          /*
            Convert DirectColor scanline to runlength-encoded color packets.
          */
          offset=image->columns*image->rows;
          for (y=0; y < image->rows; y++)
          {
            for (x=0; x < image->columns; x++)
            {
              q->red=(*p);
              q->green=(*(p+offset));
              q->blue=(*(p+offset*2));
              if (image->colors != 0)
                {
                  q->red=image->colormap[q->red].red;
                  q->green=image->colormap[q->green].green;
                  q->blue=image->colormap[q->blue].blue;
                }
              q->index=(unsigned short) (image->matte ? (*(p+offset*3)) : 0);
              q->length=0;
              p++;
              q++;
            }
          }
        }
    (void) free((char *) viff_pixels);
    if (image->class == PseudoClass)
      {
        SyncImage(image);
        CompressColormap(image);
      }
    /*
      Proceed to next image.
    */
    status=ReadData((char *) &viff_header.identifier,1,1,image->file);
    if ((status == True) && (viff_header.identifier == 0xab))
      {
        /*
          Allocate image structure.
        */
        image->next=AllocateImage(image_info);
        if (image->next == (Image *) NULL)
          {
            DestroyImages(image);
            return((Image *) NULL);
          }
        (void) strcpy(image->next->filename,image_info->filename);
        image->next->file=image->file;
        image->next->scene=image->scene+1;
        image->next->previous=image;
        image=image->next;
      }
  } while ((status == True) && (viff_header.identifier == 0xab));
  while (image->previous != (Image *) NULL)
    image=image->previous;
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d X I m a g e                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure ReadXImage reads an image from an X window.
%
%  The format of the ReadXImage routine is:
%
%      image=ReadXImage(image_info,frame,borders,screen,descend)
%
%  A description of each parameter follows:
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%    o frame: Specifies whether to include the window manager frame with the
%      image.
%
%    o borders: Specifies whether borders pixels are to be saved with
%      the image.
%
%    o screen: Specifies whether the GetImage request used to obtain the image
%      should be done on the root window, rather than directly on the specified
%      window.
%
%    o descend: If this option is zero the colormap of the chosen window is
%      used to obtain the red, green, and blue values.  Otherwise the image
%      is obtained by descending the window hierarchy and reading each
%      subwindow and its colormap.
%
%
*/
Image *ReadXImage(image_info,frame,borders,screen,descend)
ImageInfo
  *image_info;

unsigned int
  frame,
  borders,
  screen,
  descend;
{
  Colormap
    *colormaps;

  Display
    *display;

  Image
    *image;

  int
    number_colormaps,
    number_windows,
    status,
    x;

  RectangleInfo
    clip_info;

  Window
    client_window,
    *colormap_windows,
    root_window,
    target_window;

  XTextProperty
    window_name;

  /*
    Open X server connection.
  */
  display=XOpenDisplay(image_info->server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",
        XDisplayName(image_info->server_name));
      return((Image *) NULL);
    }
  /*
    Set our forgiving error handler.
  */
  XSetErrorHandler(XError);
  /*
    Select target window.
  */
  clip_info.x=0;
  clip_info.y=0;
  clip_info.width=0;
  clip_info.height=0;
  root_window=XRootWindow(display,XDefaultScreen(display));
  target_window=(Window) NULL;
  if ((image_info->filename != (char *) NULL) &&
      (*image_info->filename != '\0'))
    if (Latin1Compare(image_info->filename,"root") == 0)
      target_window=root_window;
    else
      {
        /*
          Select window by ID or name.
        */
        if (isdigit(*image_info->filename))
          target_window=XWindowByID(display,root_window,
            (Window) strtol(image_info->filename,(char **) NULL,0));
        if (target_window == (Window) NULL)
          target_window=XWindowByName(display,root_window,image_info->filename);
        if (target_window == (Window) NULL)
          Warning("No window with specified id exists",image_info->filename);
      }
  /*
    If target window is not defined, interactively select one.
  */
  if (target_window == (Window) NULL)
    target_window=XSelectWindow(display,&clip_info);
  client_window=target_window;
  if (target_window != root_window)
    {
      unsigned int
        d;

      /*
        Get client window.
      */
      status=XGetGeometry(display,target_window,&root_window,&x,&x,&d,&d,&d,&d);
      if (status != 0)
        {
          client_window=XClientWindow(display,target_window);
          if (!frame)
            target_window=client_window;
        }
    }
  if (screen)
    {
      int
        y;

      Window
        child;

      XWindowAttributes
        window_attributes;

      /*
        Obtain window image directly from screen.
      */
      status=XGetWindowAttributes(display,target_window,&window_attributes);
      if (status == False)
        {
          Warning("Unable to read X window attributes",image_info->filename);
          XCloseDisplay(display);
          return((Image *) NULL);
        }
      XTranslateCoordinates(display,target_window,root_window,0,0,&x,&y,&child);
      clip_info.x=x;
      clip_info.y=y;
      clip_info.width=window_attributes.width;
      clip_info.height=window_attributes.height;
      if (borders)
        {
          /*
            Include border in image.
          */
          clip_info.x-=window_attributes.border_width;
          clip_info.y-=window_attributes.border_width;
          clip_info.width+=window_attributes.border_width << 1;
          clip_info.height+=window_attributes.border_width << 1;
        }
      target_window=root_window;
    }
  /*
    If WM_COLORMAP_WINDOWS property is set or multiple colormaps, descend.
  */
  number_windows=0;
  status=XGetWMColormapWindows(display,target_window,&colormap_windows,
    &number_windows);
  if ((status == True) && (number_windows > 0))
    {
      descend=True;
      XFree ((char *) colormap_windows);
    }
  colormaps=XListInstalledColormaps(display,target_window,&number_colormaps);
  if (number_colormaps > 0)
    {
      if (number_colormaps > 1)
        descend=True;
      XFree((char *) colormaps);
    }
  /*
    Alert the user not to alter the screen.
  */
  XBell(display,0);
  /*
    Get image by window id.
  */
  XGrabServer(display);
  image=XGetWindowImage(display,target_window,borders,descend);
  XUngrabServer(display);
  (void) strcpy(image->filename,image_info->filename);
  if (image == (Image *) NULL)
    Warning("Unable to read X window image",image_info->filename);
  else
    {
      if ((clip_info.width != 0) && (clip_info.height != 0))
        {
          Image
            *clipped_image;

          /*
            Clip image as defined by the clipping rectangle.
          */
          clipped_image=ClipImage(image,&clip_info);
          if (clipped_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=clipped_image;
            }
        }
      status=XGetWMName(display,target_window,&window_name);
      if (status == True)
        {
          if ((image_info->filename != (char *) NULL) &&
              (*image_info->filename == '\0'))
            {
              /*
                Initialize image filename.
              */
              (void) strncpy(image->filename,(char *) window_name.value,
                (int) window_name.nitems);
              image->filename[window_name.nitems]='\0';
            }
        }
    }
  /*
    Alert the user we're done.
  */
  XBell(display,0);
  XBell(display,0);
  XCloseDisplay(display);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d X B M I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadXBMImage reads an X11 bitmap image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadXBMImage routine is:
%
%      image=ReadXBMImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadXBMImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadXBMImage(image_info)
ImageInfo
  *image_info;
{
  char
    data[MaxTextLength];

  Image
    *image;

  register int
    x,
    y;

  register RunlengthPacket
    *q;

  register unsigned char
    bit;

  register unsigned short
    index;

  unsigned int
    byte;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read X bitmap header.
  */
  while (fgets(data,MaxTextLength,image->file) != (char *) NULL)
    if (sscanf(data,"#define %*32s %u",&image->columns) == 1)
      break;
  while (fgets(data,MaxTextLength,image->file) != (char *) NULL)
    if (sscanf(data,"#define %*32s %u",&image->rows) == 1)
      break;
  if ((image->columns == 0) || (image->rows == 0))
    {
      Warning("XBM file is not in the correct format",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  while (fgets(data,MaxTextLength,image->file) != (char *) NULL)
    if (sscanf(data,"%*[^#] char"))
      break;
  if (feof(image->file))
    {
      Warning("XBM file is not in the correct format",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize image structure.
  */
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if (image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Create colormap.
  */
  image->class=PseudoClass;
  image->colors=2;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if (image->colormap == (ColorPacket *) NULL)
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  image->colormap[0].red=0;
  image->colormap[0].green=0;
  image->colormap[0].blue=0;
  image->colormap[1].red=MaxRGB;
  image->colormap[1].green=MaxRGB;
  image->colormap[1].blue=MaxRGB;
  /*
    Convert X bitmap image to runlength-encoded packets.
  */
  q=image->pixels;
  for (y=0; y < image->rows; y++)
  {
    bit=0;
    for (x=0; x < image->columns; x++)
    {
      if (bit == 0)
        (void) fscanf(image->file,"%i,",&byte);
      index=(byte & 0x01) ? 0 : 1;
      q->red=image->colormap[index].red;
      q->green=image->colormap[index].green;
      q->blue=image->colormap[index].blue;
      q->index=index;
      q->length=0;
      q++;
      bit++;
      byte>>=1;
      if (bit == 8)
        bit=0;
    }
  }
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d X C I m a g e                                                      %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadXCImage creates a constant image and initializes to the
%  X server color as specified by the filename.  It allocates the memory
%  necessary for the new Image structure and returns a pointer to the new
%  image.
%
%  The format of the ReadXCImage routine is:
%
%      image=ReadXCImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadXCImage returns a pointer to the image after
%      creating it. A null image is returned if there is a a memory shortage
%      or if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadXCImage(image_info)
ImageInfo
  *image_info;
{
  char
    *resource_value;

  Display
    *display;

  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  unsigned int
    height,
    width;

  XPixelInfo
    pixel_info;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open X server connection.
  */
  display=XOpenDisplay(image_info->server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",
        XDisplayName(image_info->server_name));
      DestroyImage(image);
      return((Image *) NULL);
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
  if ((map_info == (XStandardColormap *) NULL) ||
      (visual_info == (XVisualInfo *) NULL))
    {
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine image color-- specified by the filename.
  */
  if (*image_info->filename != '\0')
    resource_info.background_color=image_info->filename;
  XGetMapInfo(visual_info,XDefaultColormap(display,visual_info->screen),
    map_info);
  XGetPixelInfo(display,visual_info,map_info,&resource_info,(Image *) NULL,
    &pixel_info);
  pixel_info.annotate_context=XDefaultGC(display,visual_info->screen);
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  (void) strcpy(image->filename,image_info->filename);
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize Image structure.
  */
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  image->class=PseudoClass;
  image->colors=1;
  image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
  if ((image->pixels == (RunlengthPacket *) NULL) ||
      (image->colormap == (ColorPacket *) NULL))
    {
      Warning("Unable to allocate image","Memory allocation error");
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize colormap.
  */
  image->colormap[0].red=pixel_info.background_color.red >> 8;
  image->colormap[0].green=pixel_info.background_color.green >> 8;
  image->colormap[0].blue=pixel_info.background_color.blue >> 8;
  q=image->pixels;
  for (i=0; i < image->packets; i++)
  {
    q->index=0;
    q->length=0;
    q++;
  }
  SyncImage(image);
  /*
    Free resources.
  */
  XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
    (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d X P M I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadXBMImage reads an X11 pixmap image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadXPMImage routine is:
%
%      image=ReadXPMImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadXPMImage returns a pointer to the image after
%      creating it. A null image is returned if there is a a memory shortage
%      or if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
#ifdef HasXPM
#include "xpm.h"
static Image *ReadXPMImage(image_info)
ImageInfo
  *image_info;
{
  char
    *resource_value,
    *xpm_buffer;

  Display
    *display;

  Image
    *image;

  int
    length,
    status,
    x,
    y;

  register char
    *q;

  register int
    i;

  register RunlengthPacket
    *p;

  register unsigned long
    pixel;

  XColor
    *colors;

  XImage
    *matte_image,
    *ximage;

  XpmAttributes
    xpm_attributes;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read XPM file.
  */
  length=MaxTextLength;
  xpm_buffer=(char *) malloc(length*sizeof(char));
  if (xpm_buffer != (char *) NULL)
    {
      q=xpm_buffer;
      while (fgets(q,MaxTextLength,image->file) != (char *) NULL)
      {
        q+=strlen(q);
        if ((q-xpm_buffer+MaxTextLength) > length)
          {
            *q='\0';
            length<<=1;
            xpm_buffer=(char *)
              realloc((char *) xpm_buffer,length*sizeof(char));
            if (xpm_buffer == (char *) NULL)
              break;
            q=xpm_buffer+strlen(xpm_buffer);
          }
      }
    }
  if (xpm_buffer == (char *) NULL)
    {
      Warning("Unable to read XPM image","Memory allocation failed");
      DestroyImage(image);
      return((Image *) NULL);
    }
  CloseImage(image);
  /*
    Open X server connection.
  */
  display=XOpenDisplay(image_info->server_name);
  if (display == (Display *) NULL)
    {
      Warning("Unable to connect to X server",
        XDisplayName(image_info->server_name));
      DestroyImage(image);
      return((Image *) NULL);
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
    }
  if ((map_info == (XStandardColormap *) NULL) ||
      (visual_info == (XVisualInfo *) NULL))
    {
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize X colormap.
  */
  map_info->colormap=XCreateColormap(display,
    XRootWindow(display,visual_info->screen),visual_info->visual,
    visual_info->class == DirectColor ? AllocAll : AllocNone);
  if (map_info->colormap == (Colormap) NULL)
    {
      Warning("Unable to create colormap",(char *) NULL);
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize XPM attributes.
  */
  xpm_attributes.valuemask=XpmColorKey | XpmColormap | XpmDepth | XpmVisual;
  xpm_attributes.visual=visual_info->visual;
  xpm_attributes.colormap=map_info->colormap;
  xpm_attributes.depth=visual_info->depth;
  xpm_attributes.color_key=XPM_COLOR;
  if (IsGrayImage(image))
    if (image->colors > 16)
      xpm_attributes.color_key=XPM_GRAY;
    else
      if (image->colors == 2)
        xpm_attributes.color_key=XPM_MONO;
      else
        xpm_attributes.color_key=XPM_GRAY4;
  /*
    Read in a file in the XPM format into a X image structure.
  */
  status=XpmCreateImageFromBuffer(display,xpm_buffer,&ximage,&matte_image,
    &xpm_attributes);
  if (status != XpmSuccess)
    {
      Warning("Unable to read XPM image",(char *) NULL);
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  XpmFreeAttributes(&xpm_attributes);
  /*
    Get the colormap colors.
  */
  colors=(XColor *) malloc(visual_info->colormap_size*sizeof(XColor));
  if (colors == (XColor *) NULL)
    {
      Warning("Unable to read X colormap",(char *) NULL);
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      XDestroyImage(ximage);
      DestroyImage(image);
      return((Image *) NULL);
    }
  if ((visual_info->class != DirectColor) && (visual_info->class != TrueColor))
    for (i=0; i < visual_info->colormap_size; i++)
    {
      colors[i].pixel=i;
      colors[i].pad=0;
    }
  else
    {
      unsigned long
        blue,
        blue_bit,
        green,
        green_bit,
        red,
        red_bit;

      /*
        DirectColor or TrueColor visual.
      */
      red=0;
      green=0;
      blue=0;
      red_bit=visual_info->red_mask & (~(visual_info->red_mask)+1);
      green_bit=visual_info->green_mask & (~(visual_info->green_mask)+1);
      blue_bit=visual_info->blue_mask & (~(visual_info->blue_mask)+1);
      for (i=0; i < visual_info->colormap_size; i++)
      {
        colors[i].pixel=red | green | blue;
        colors[i].pad=0;
        red+=red_bit;
        if (red > visual_info->red_mask)
          red=0;
        green+=green_bit;
        if (green > visual_info->green_mask)
          green=0;
        blue+=blue_bit;
        if (blue > visual_info->blue_mask)
          blue=0;
      }
    }
  XQueryColors(display,map_info->colormap,colors,visual_info->colormap_size);
  /*
    Convert X image to MIFF format.
  */
  if ((visual_info->class != TrueColor) && (visual_info->class != DirectColor))
    image->class=PseudoClass;
  image->columns=ximage->width;
  image->rows=ximage->height;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if (image->pixels == (RunlengthPacket *) NULL)
    {
      (void) free((char *) colors);
      XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
        (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
      XDestroyImage(ximage);
      DestroyImage(image);
      return((Image *) NULL);
    }
  p=image->pixels;
  switch (image->class)
  {
    case DirectClass:
    {
      register unsigned long
        color,
        index;

      unsigned long
        blue_mask,
        blue_shift,
        green_mask,
        green_shift,
        red_mask,
        red_shift;

      /*
        Determine shift and mask for red, green, and blue.
      */
      red_mask=visual_info->red_mask;
      red_shift=0;
      while ((red_mask & 0x01) == 0)
      {
        red_mask>>=1;
        red_shift++;
      }
      green_mask=visual_info->green_mask;
      green_shift=0;
      while ((green_mask & 0x01) == 0)
      {
        green_mask>>=1;
        green_shift++;
      }
      blue_mask=visual_info->blue_mask;
      blue_shift=0;
      while ((blue_mask & 0x01) == 0)
      {
        blue_mask>>=1;
        blue_shift++;
      }
      /*
        Convert X image to DirectClass packets.
      */
      if ((visual_info->colormap_size > 0) &&
          (visual_info->class == DirectColor))
        for (y=0; y < image->rows; y++)
        {
          for (x=0; x < image->columns; x++)
          {
            pixel=XGetPixel(ximage,x,y);
            index=(pixel >> red_shift) & red_mask;
            p->red=(unsigned char) (colors[index].red >> 8);
            index=(pixel >> green_shift) & green_mask;
            p->green=(unsigned char) (colors[index].green >> 8);
            index=(pixel >> blue_shift) & blue_mask;
            p->blue=(unsigned char) (colors[index].blue >> 8);
            p->index=0;
            p->length=0;
            p++;
          }
        }
      else
        for (y=0; y < image->rows; y++)
          for (x=0; x < image->columns; x++)
          {
            pixel=XGetPixel(ximage,x,y);
            color=(pixel >> red_shift) & red_mask;
            p->red=(unsigned char)
              ((((unsigned long) color*65535)/red_mask) >> 8);
            color=(pixel >> green_shift) & green_mask;
            p->green=(unsigned char)
              ((((unsigned long) color*65535)/green_mask) >> 8);
            color=(pixel >> blue_shift) & blue_mask;
            p->blue=(unsigned char)
              ((((unsigned long) color*65535)/blue_mask) >> 8);
            p->index=0;
            p->length=0;
            p++;
          }
      break;
    }
    case PseudoClass:
    {
      /*
        Create colormap.
      */
      image->colors=visual_info->colormap_size;
      image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
      if (image->colormap == (ColorPacket *) NULL)
        {
          (void) free((char *) colors);
          XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
            (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
          XDestroyImage(ximage);
          DestroyImage(image);
          return((Image *) NULL);
        }
      for (i=0; i < image->colors; i++)
      {
        image->colormap[colors[i].pixel].red=colors[i].red >> 8;
        image->colormap[colors[i].pixel].green=colors[i].green >> 8;
        image->colormap[colors[i].pixel].blue=colors[i].blue >> 8;
      }
      /*
        Convert X image to PseudoClass packets.
      */
      for (y=0; y < image->rows; y++)
        for (x=0; x < image->columns; x++)
        {
          pixel=XGetPixel(ximage,x,y);
          p->index=(unsigned short) pixel;
          p->length=0;
          p++;
        }
      SyncImage(image);
      break;
    }
  }
  if (matte_image != (XImage *) NULL)
    {
      /*
        Initialize image matte.
      */
      image->class=DirectClass;
      image->matte=True;
      p=image->pixels;
      for (y=0; y < image->rows; y++)
        for (x=0; x < image->columns; x++)
        {
          pixel=XGetPixel(matte_image,x,y);
          p->index=(unsigned short) !pixel;
          p++;
        }
      XDestroyImage(matte_image);
    }
  if (image->class == PseudoClass)
    CompressColormap(image);
  /*
    Free resources.
  */
  (void) free((char *) colors);
  XDestroyImage(ximage);
  XFreeResources(display,visual_info,map_info,(XPixelInfo *) NULL,
    (XFontStruct *) NULL,&resource_info,(XWindowInfo *) NULL);
  return(image);
}
#else
static Image *ReadXPMImage(image_info)
ImageInfo
  *image_info;
{
  Warning("XPM library is not available",image_info->filename);
  return(ReadMIFFImage(image_info));
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d X W D I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadXWDImage reads an X Window System window dump image file and
%  returns it.  It allocates the memory necessary for the new Image structure
%  and returns a pointer to the new image.
%
%  The format of the ReadXWDImage routine is:
%
%      image=ReadXWDImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadXWDImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadXWDImage(image_info)
ImageInfo
  *image_info;
{
  char
    *window_name;

  Image
    *image;

  int
    status,
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned short
    index;

  register unsigned long
    pixel;

  unsigned long
    lsb_first,
    packets;

  XColor
    *colors;

  XImage
    *ximage;

  XWDFileHeader
    header;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
     Read in header information.
  */
  status=ReadData((char *) &header,sz_XWDheader,1,image->file);
  if (status == False)
    {
      Warning("Unable to read dump file header",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Ensure the header byte-order is most-significant byte first.
  */
  lsb_first=1;
  if (*(char *) &lsb_first)
    MSBFirstOrderLong((char *) &header,sz_XWDheader);
  /*
    Check to see if the dump file is in the proper format.
  */
  if (header.file_version != XWD_FILE_VERSION)
    {
      Warning("XWD file format version mismatch",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  if (header.header_size < sz_XWDheader)
    {
      Warning("XWD header size is too small",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  packets=(header.header_size-sz_XWDheader);
  window_name=(char *) malloc((unsigned int) packets*sizeof(char));
  if (window_name == (char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  status=ReadData((char *) window_name,1,(int) packets,image->file);
  if (status == False)
    {
      Warning("Unable to read window name from dump file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Initialize the X image.
  */
  ximage=(XImage *) malloc(sizeof(XImage));
  if (ximage == (XImage *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  ximage->depth=header.pixmap_depth;
  ximage->format=header.pixmap_format;
  ximage->xoffset=header.xoffset;
  ximage->data=(char *) NULL;
  ximage->width=header.pixmap_width;
  ximage->height=header.pixmap_height;
  ximage->bitmap_pad=header.bitmap_pad;
  ximage->bytes_per_line=header.bytes_per_line;
  ximage->byte_order=header.byte_order;
  ximage->bitmap_unit=header.bitmap_unit;
  ximage->bitmap_bit_order=header.bitmap_bit_order;
  ximage->bits_per_pixel=header.bits_per_pixel;
  ximage->red_mask=header.red_mask;
  ximage->green_mask=header.green_mask;
  ximage->blue_mask=header.blue_mask;
  status=XInitImage(ximage);
  if (status == False)
    {
      Warning("Invalid XWD header",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Read colormap.
  */
  colors=(XColor *) NULL;
  if (header.ncolors != 0)
    {
      XWDColor
        color;

      colors=(XColor *) malloc((unsigned int) header.ncolors*sizeof(XColor));
      if (colors == (XColor *) NULL)
        {
          Warning("Unable to allocate memory",(char *) NULL);
          DestroyImage(image);
          return((Image *) NULL);
        }
      for (i=0; i < header.ncolors; i++)
      {
        status=ReadData((char *) &color,sz_XWDColor,1,image->file);
        if (status == False)
          {
            Warning("Unable to read color map from dump file",image->filename);
            DestroyImage(image);
            return((Image *) NULL);
          }
        colors[i].pixel=color.pixel;
        colors[i].red=color.red;
        colors[i].green=color.green;
        colors[i].blue=color.blue;
        colors[i].flags=color.flags;
      }
      /*
        Ensure the header byte-order is most-significant byte first.
      */
      lsb_first=1;
      if (*(char *) &lsb_first)
        for (i=0; i < header.ncolors; i++)
        {
          MSBFirstOrderLong((char *) &colors[i].pixel,sizeof(unsigned long));
          MSBFirstOrderShort((char *) &colors[i].red,3*sizeof(unsigned short));
        }
    }
  /*
    Allocate the pixel buffer.
  */
  if (ximage->format == ZPixmap)
    packets=ximage->bytes_per_line*ximage->height;
  else
    packets=ximage->bytes_per_line*ximage->height*ximage->depth;
  ximage->data=(char *) malloc(packets*sizeof(unsigned char));
  if (ximage->data == (char *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  status=ReadData(ximage->data,1,(int) packets,image->file);
  if (status == False)
    {
      Warning("Unable to read dump pixmap",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert image to MIFF format.
  */
  image->columns=ximage->width;
  image->rows=ximage->height;
  /*
    Initial image comment.
  */
  if ((ximage->red_mask != 0) || (ximage->green_mask != 0) ||
      (ximage->blue_mask != 0))
    image->class=DirectClass;
  else
    image->class=PseudoClass;
  image->colors=header.ncolors;
  image->packets=image->columns*image->rows;
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if (image->pixels == (RunlengthPacket *) NULL)
    {
      Warning("Unable to allocate memory",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  q=image->pixels;
  switch (image->class)
  {
    case DirectClass:
    {
      register unsigned long
        color;

      unsigned long
        blue_mask,
        blue_shift,
        green_mask,
        green_shift,
        red_mask,
        red_shift;

      /*
        Determine shift and mask for red, green, and blue.
      */
      red_mask=ximage->red_mask;
      red_shift=0;
      while ((red_mask & 0x01) == 0)
      {
        red_mask>>=1;
        red_shift++;
      }
      green_mask=ximage->green_mask;
      green_shift=0;
      while ((green_mask & 0x01) == 0)
      {
        green_mask>>=1;
        green_shift++;
      }
      blue_mask=ximage->blue_mask;
      blue_shift=0;
      while ((blue_mask & 0x01) == 0)
      {
        blue_mask>>=1;
        blue_shift++;
      }
      /*
        Convert X image to DirectClass packets.
      */
      if (image->colors != 0)
        for (y=0; y < image->rows; y++)
        {
          for (x=0; x < image->columns; x++)
          {
            pixel=XGetPixel(ximage,x,y);
            index=(unsigned short) ((pixel >> red_shift) & red_mask);
            q->red=(unsigned char) (colors[index].red >> 8);
            index=(unsigned short) ((pixel >> green_shift) & green_mask);
            q->green=(unsigned char) (colors[index].green >> 8);
            index=(unsigned short) ((pixel >> blue_shift) & blue_mask);
            q->blue=(unsigned char) (colors[index].blue >> 8);
            q->index=0;
            q->length=0;
            q++;
          }
        }
      else
        for (y=0; y < image->rows; y++)
          for (x=0; x < image->columns; x++)
          {
            pixel=XGetPixel(ximage,x,y);
            color=(pixel >> red_shift) & red_mask;
            q->red=(unsigned char)
              ((((unsigned long) color*65535)/red_mask) >> 8);
            color=(pixel >> green_shift) & green_mask;
            q->green=(unsigned char)
              ((((unsigned long) color*65535)/green_mask) >> 8);
            color=(pixel >> blue_shift) & blue_mask;
            q->blue=(unsigned char)
              ((((unsigned long) color*65535)/blue_mask) >> 8);
            q->index=0;
            q->length=0;
            q++;
          }
      break;
    }
    case PseudoClass:
    {
      /*
        Convert X image to PseudoClass packets.
      */
      image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
      if (image->colormap == (ColorPacket *) NULL)
        {
          Warning("Unable to allocate memory",(char *) NULL);
          DestroyImage(image);
          return((Image *) NULL);
        }
      for (i=0; i < image->colors; i++)
      {
        image->colormap[i].red=colors[i].red >> 8;
        image->colormap[i].green=colors[i].green >> 8;
        image->colormap[i].blue=colors[i].blue >> 8;
      }
      for (y=0; y < image->rows; y++)
        for (x=0; x < image->columns; x++)
        {
          pixel=XGetPixel(ximage,x,y);
          q->red=(unsigned char) (colors[pixel].red >> 8);
          q->green=(unsigned char) (colors[pixel].green >> 8);
          q->blue=(unsigned char) (colors[pixel].blue >> 8);
          q->index=(unsigned short) pixel;
          q->length=0;
          q++;
        }
      CompressColormap(image);
      break;
    }
  }
  /*
    Free image and colormap.
  */
  (void) free((char *) window_name);
  if (header.ncolors != 0)
    (void) free((char *) colors);
  XDestroyImage(ximage);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d Y U V I m a g e                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadYUVImage reads an image with digital YUV (CCIR 601 1:1:1) bytes
%  and returns it.  It allocates the memory necessary for the new Image
%  structure and returns a pointer to the new image.  U and V, normally -0.5
%  through 0.5, are expected to be normalized to the range 0 through MaxRGB
%  fit withing a byte.
%
%  The format of the ReadYUVImage routine is:
%
%      image=ReadYUVImage(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadYUVImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadYUVImage(image_info)
ImageInfo
  *image_info;
{
  Image
    *image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *yuv_pixels;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  image=AllocateImage(image_info);
  if (image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  OpenImage(image,"rb");
  if (image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",image->filename);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Initialize image structure.
  */
  image->columns=width;
  image->rows=height;
  image->packets=image->columns*image->rows;
  yuv_pixels=(unsigned char *) malloc(3*image->packets*sizeof(unsigned char));
  image->pixels=(RunlengthPacket *)
    malloc(image->packets*sizeof(RunlengthPacket));
  if ((yuv_pixels == (unsigned char *) NULL) ||
      (image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(image);
      return((Image *) NULL);
    }
  /*
    Convert raster image to runlength-encoded packets.
  */
  (void) ReadData((char *) yuv_pixels,3,(int) image->packets,image->file);
  p=yuv_pixels;
  switch (image_info->interlace)
  {
    case NoneInterlace:
    default:
    {
      /*
        No interlacing:  YUVYUVYUVYUVYUVYUV...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->green=(*p++);
        q->blue=(*p++);
        q->index=0;
        q->length=0;
        q++;
      }
      break;
    }
    case LineInterlace:
    {
      /*
        Line interlacing:  YYY...UUU...VVV...YYY...UUU...VVV...
      */
      for (y=0; y < image->rows; y++)
      {
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->red=(*p++);
          q->index=0;
          q->length=0;
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->green=(*p++);
          q++;
        }
        q=image->pixels+y*image->columns;
        for (x=0; x < image->columns; x++)
        {
          q->blue=(*p++);
          q++;
        }
      }
      break;
    }
    case PlaneInterlace:
    {
      /*
        Plane interlacing:  YYYYYY...UUUUUU...VVVVVV...
      */
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->red=(*p++);
        q->index=0;
        q->length=0;
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->green=(*p++);
        q++;
      }
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        q->blue=(*p++);
        q++;
      }
      break;
    }
  }
  (void) free((char *) yuv_pixels);
  TransformRGBImage(image,YCbCrColorspace);
  CloseImage(image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  R e a d Y U V 3 I m a g e                                                  %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadYUV3Image reads an image with digital YUV (CCIR 601 2:1:1)
%  bytes and returns it.  It allocates the memory necessary for the new Image
%  structure and returns a pointer to the new image.  U and V, normally -0.5
%  through 0.5, are expected to be normalized to the range 0 through MaxRGB
%  fit withing a byte.
%
%  The format of the ReadYUV3Image routine is:
%
%      image=ReadYUV3Image(image_info)
%
%  A description of each parameter follows:
%
%    o image:  Function ReadYUV3Image returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
static Image *ReadYUV3Image(image_info)
ImageInfo
  *image_info;
{
  char
    filename[MaxTextLength];

  Image
    *image,
    *scaled_image;

  int
    x,
    y;

  register int
    i;

  register RunlengthPacket
    *q;

  register unsigned char
    *p;

  unsigned char
    *uv_pixels,
    *y_pixels;

  unsigned int
    height,
    width;

  /*
    Allocate image structure.
  */
  scaled_image=AllocateImage(image_info);
  if (scaled_image == (Image *) NULL)
    return((Image *) NULL);
  /*
    Open image file.
  */
  (void) strcpy(filename,image_info->filename);
  (void) strcpy(scaled_image->filename,filename);
  if (strcmp(scaled_image->filename,"-") != 0)
    (void) strcat(scaled_image->filename,".Y");
  OpenImage(scaled_image,"rb");
  if (scaled_image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",scaled_image->filename);
      DestroyImage(scaled_image);
      return((Image *) NULL);
    }
  /*
    Determine width and height, e.g. 640x512.
  */
  width=512;
  height=512;
  if (image_info->geometry != (char *) NULL)
    (void) XParseGeometry(image_info->geometry,&x,&y,&width,&height);
  /*
    Read Y channel.
  */
  scaled_image->columns=width >> 1;
  scaled_image->rows=height >> 1;
  scaled_image->packets=scaled_image->columns*scaled_image->rows;
  uv_pixels=(unsigned char *)
    malloc(scaled_image->packets*sizeof(unsigned char));
  y_pixels=(unsigned char *)
    malloc(4*scaled_image->packets*sizeof(unsigned char));
  scaled_image->pixels=(RunlengthPacket *)
    malloc(scaled_image->packets*sizeof(RunlengthPacket));
  if ((uv_pixels == (unsigned char *) NULL) ||
      (y_pixels == (unsigned char *) NULL) ||
      (scaled_image->pixels == (RunlengthPacket *) NULL))
    {
      Warning("Memory allocation error",(char *) NULL);
      DestroyImage(scaled_image);
      return((Image *) NULL);
    }
  (void) ReadData((char *) y_pixels,4,(int) scaled_image->packets,
    scaled_image->file);
  CloseImage(scaled_image);
  /*
    Read U channel.
  */
  (void) strcpy(scaled_image->filename,filename);
  if (strcmp(scaled_image->filename,"-") != 0)
    (void) strcat(scaled_image->filename,".U");
  OpenImage(scaled_image,"rb");
  if (scaled_image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",scaled_image->filename);
      DestroyImage(scaled_image);
      return((Image *) NULL);
    }
  (void) ReadData((char *) uv_pixels,1,(int) scaled_image->packets,
    scaled_image->file);
  p=uv_pixels;
  q=scaled_image->pixels;
  for (i=0; i < scaled_image->packets; i++)
  {
    q->green=(*p);
    q->index=0;
    q->length=0;
    p++;
    q++;
  }
  CloseImage(scaled_image);
  /*
    Read V channel.
  */
  (void) strcpy(scaled_image->filename,filename);
  if (strcmp(scaled_image->filename,"-") != 0)
    (void) strcat(scaled_image->filename,".V");
  OpenImage(scaled_image,"rb");
  if (scaled_image->file == (FILE *) NULL)
    {
      Warning("Unable to open file",scaled_image->filename);
      DestroyImage(scaled_image);
      return((Image *) NULL);
    }
  (void) ReadData((char *) uv_pixels,1,(int) scaled_image->packets,
    scaled_image->file);
  p=uv_pixels;
  q=scaled_image->pixels;
  for (i=0; i < scaled_image->packets; i++)
  {
    q->blue=(*p);
    p++;
    q++;
  }
  CloseImage(scaled_image);
  (void) free((char *) uv_pixels);
  /*
    Scale image.
  */
  scaled_image->orphan=True;
  image=
    ScaleImage(scaled_image,scaled_image->columns << 1,scaled_image->rows << 1);
  DestroyImage(scaled_image);
  if (image == (Image *) NULL)
    {
      Warning("Unable to read image","Memory allocation failed");
      return((Image *) NULL);
    }
  p=y_pixels;
  q=image->pixels;
  for (i=0; i < (image->columns*image->rows); i++)
  {
    q->red=(*p);
    p++;
    q++;
  }
  (void) free((char *) y_pixels);
  TransformRGBImage(image,YCbCrColorspace);
  (void) strcpy(image->filename,filename);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d I m a g e                                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function ReadImage reads an image and returns it.  It allocates
%  the memory necessary for the new Image structure and returns a pointer to
%  the new image.  By default, the image format is determined by its magic
%  number. To specify a particular image format, precede the filename with an
%  explicit image format name and a colon (i.e.  ps:image) or as the filename
%  suffix  (i.e. image.ps).
%
%  The format of the ReadImage routine is:
%
%      image=ReadImage(image_info)
%
%  A description of each parameter follows:
%
%    o image: Function ReadImage returns a pointer to the image after
%      reading.  A null image is returned if there is a a memory shortage or
%      if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%
*/
Image *ReadImage(image_info)
ImageInfo
  *image_info;
{
  char
    magic_number[12];

  Image
    *image;

  unsigned int
    temporary_file;

  temporary_file=False;
  SetImageMagick(image_info);
  if (!image_info->assert)
    {
      Image
        decode_image;

      /*
        Determine type from image magic number.
      */
      *magic_number='\0';
      (void) strcpy(decode_image.filename,image_info->filename);
      OpenImage(&decode_image,"rb");
      if (decode_image.file != (FILE *) NULL)
        if ((decode_image.file == stdin) || decode_image.pipe)
          {
            char
              *directory;

            FILE
              *file;

            int
              c;

            /*
              Copy standard input or pipe to temporary file.
            */
            temporary_file=True;
            directory=(char *) getenv("TMPDIR");
            if (directory == (char *) NULL)
              directory="/tmp";
            (void) sprintf(image_info->filename,"%s/magickXXXXXX",directory);
            (void) mktemp(image_info->filename);
            file=fopen(image_info->filename,"wb");
            if (file == (FILE *) NULL)
              {
                Warning("Unable to write file",image_info->filename);
                return((Image *) NULL);
              }
            c=fgetc(decode_image.file);
            while (c != EOF)
            {
              (void) putc(c,file);
              c=fgetc(decode_image.file);
            }
            (void) fclose(file);
            CloseImage(&decode_image);
            (void) strcpy(decode_image.filename,image_info->filename);
            OpenImage(&decode_image,"rb");
          }
      if (decode_image.file != (FILE *) NULL)
        {
          /*
            Read magic number.
          */
          (void) ReadData(magic_number,sizeof(char),sizeof(magic_number),
            decode_image.file);
          CloseImage(&decode_image);
        }
      /*
        Determine the image format.
      */
      if (strncmp(magic_number,"BM",2) == 0)
        (void) strcpy(image_info->magick,"BMP");
      if (strncmp(magic_number,"GIF8",4) == 0)
        (void) strcpy(image_info->magick,"GIF");
      if (strncmp(magic_number,"\001\332",2) == 0)
        (void) strcpy(image_info->magick,"IRIS");
      if (strncmp(magic_number,"\377\330\377",3) == 0)
        (void) strcpy(image_info->magick,"JPEG");
      else
        if ((strcmp(image_info->magick,"JPEG") == 0) ||
            (strcmp(image_info->magick,"JPG") == 0))
          (void) strcpy(image_info->magick,"MIFF");
      if ((unsigned char) *magic_number == 0x0a)
        (void) strcpy(image_info->magick,"PCX");
      if ((*magic_number == 'P') && isdigit(magic_number[1]))
        (void) strcpy(image_info->magick,"PNM");
      if (strncmp(magic_number,"%!",2) == 0)
        (void) strcpy(image_info->magick,"PS");
      if (strncmp(magic_number,"\131\246\152\225",4) == 0)
        (void) strcpy(image_info->magick,"SUN");
      if ((strncmp(magic_number,"\115\115",2) == 0) ||
          (strncmp(magic_number,"\111\111",2) == 0))
        (void) strcpy(image_info->magick,"TIFF");
      if (strncmp(magic_number,"\122\314",2) == 0)
        (void) strcpy(image_info->magick,"RLE");
      if ((strncmp(magic_number,"LBLSIZE",7) == 0) ||
         (strncmp(magic_number,"NJPL1I",6) == 0))
        (void) strcpy(image_info->magick,"VICAR");
      if ((unsigned char) *magic_number == 0xab)
        (void) strcpy(image_info->magick,"VIFF");
      if (strncmp(magic_number,"#define",7) == 0)
        (void) strcpy(image_info->magick,"XBM");
      if ((magic_number[1] == 0x00) && (magic_number[2] == 0x00))
        if ((magic_number[5] == 0x00) && (magic_number[6] == 0x00))
          if ((magic_number[4] == 0x07) || (magic_number[7] == 0x07))
            (void) strcpy(image_info->magick,"XWD");
    }
  /*
    Call appropriate image reader based on image type.
  */
  switch (*image_info->magick)
  {
    case 'A':
    {
      image=ReadAVSImage(image_info);
      break;
    }
    case 'B':
    {
      image=ReadBMPImage(image_info);
      break;
    }
    case 'C':
    {
      image=ReadCMYKImage(image_info);
      break;
    }
    case 'E':
    {
      image=ReadPSImage(image_info);
      break;
    }
    case 'F':
    {
      if (strcmp(image_info->magick,"FAX") == 0)
        image=ReadFAXImage(image_info);
      else
        image=ReadFITSImage(image_info);
      break;
    }
    case 'G':
    {
      if (strcmp(image_info->magick,"GIF") == 0)
        image=ReadGIFImage(image_info);
      else
        if (strcmp(image_info->magick,"GIF87") == 0)
          image=ReadGIFImage(image_info);
        else
          if (strcmp(image_info->magick,"GRAY") == 0)
            image=ReadGRAYImage(image_info);
          else
            image=ReadFAXImage(image_info);
      break;
    }
    case 'H':
    {
      image=ReadHISTOGRAMImage(image_info);
      break;
    }
    case 'I':
    {
      image=ReadIRISImage(image_info);
      break;
    }
    case 'J':
    {
      image=ReadJPEGImage(image_info);
      break;
    }
    case 'M':
    {
      if (strcmp(image_info->magick,"MAP") == 0)
        image=ReadMAPImage(image_info);
      else
        if (strcmp(image_info->magick,"MATTE") == 0)
          image=ReadMATTEImage(image_info);
        else
          if (strcmp(image_info->magick,"MIFF") == 0)
            image=ReadMIFFImage(image_info);
          else
            image=ReadMTVImage(image_info);
      break;
    }
    case 'P':
    {
      if (strcmp(image_info->magick,"PCD") == 0)
        image=ReadPCDImage(image_info);
      else
        if (strcmp(image_info->magick,"PCX") == 0)
          image=ReadPCXImage(image_info);
        else
          if (strcmp(image_info->magick,"PICT") == 0)
            image=ReadPICTImage(image_info);
          else
            if (strcmp(image_info->magick,"PM") == 0)
              image=ReadXPMImage(image_info);
            else
              if ((strcmp(image_info->magick,"PS") == 0) ||
                  (strcmp(image_info->magick,"PS2") == 0))
                image=ReadPSImage(image_info);
              else
                image=ReadPNMImage(image_info);
      break;
    }
    case 'R':
    {
      if (strcmp(image_info->magick,"RAS") == 0)
        image=ReadSUNImage(image_info);
      else
        if (strcmp(image_info->magick,"RGB") == 0)
          image=ReadRGBImage(image_info);
        else
          image=ReadRLEImage(image_info);
      break;
    }
    case 'S':
    {
      image=ReadSUNImage(image_info);
      break;
    }
    case 'T':
    {
      if (strcmp(image_info->magick,"TGA") == 0)
        image=ReadTARGAImage(image_info);
      else
        if ((strcmp(image_info->magick,"TIF") == 0) ||
            (strcmp(image_info->magick,"TIFF") == 0))
          image=ReadTIFFImage(image_info);
        else
          image=ReadTEXTImage(image_info);
      break;
    }
    case 'V':
    {
      if (strcmp(image_info->magick,"VICAR") == 0)
        image=ReadVICARImage(image_info);
      else
        image=ReadVIFFImage(image_info);
      break;
    }
    case 'X':
    {
      if (strcmp(image_info->magick,"X") == 0)
        image=ReadXImage(image_info,False,False,False,False);
      else
        if (strcmp(image_info->magick,"XC") == 0)
          image=ReadXCImage(image_info);
        else
          if (strcmp(image_info->magick,"XBM") == 0)
            image=ReadXBMImage(image_info);
          else
            if (strcmp(image_info->magick,"XPM") == 0)
              image=ReadXPMImage(image_info);
            else
              if (strcmp(image_info->magick,"XV") == 0)
                image=ReadVIFFImage(image_info);
              else
                image=ReadXWDImage(image_info);
      break;
    }
    case 'Y':
    {
      if (strcmp(image_info->magick,"YUV") == 0)
        image=ReadYUVImage(image_info);
      else
        image=ReadYUV3Image(image_info);
      break;
    }
    default:
    {
      (void) strcpy(image_info->magick,"MIFF");
      image=ReadMIFFImage(image_info);
    }
  }
  if (temporary_file)
    (void) unlink(image_info->filename);
  if (image != (Image *) NULL)
    {
      if (image->status)
        {
          Warning("An error has occurred reading from file",image->filename);
          DestroyImages(image);
          return((Image *) NULL);
        }
      if (image->comments == (char *) NULL)
        CommentImage(image,"  Imported from %m image: %f");
    }
  return(image);
}
