/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                CCCC   OOO   N   N  V   V  EEEEE  RRRR   TTTTT               %
%               C      O   O  NN  N  V   V  E      R   R    T                 %
%               C      O   O  N N N  V   V  EEE    RRRR     T                 %
%               C      O   O  N  NN   V V   E      R R      T                 %
%                CCCC   OOO   N   N    V    EEEEE  R  R     T                 %
%                                                                             %
%                                                                             %
%                Convert an image from one format to another.                 %
%                                                                             %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                April 1992                                   %
%                                                                             %
%                                                                             %
%  Copyright 1994 E. I. Dupont de Nemours and Company                         %
%                                                                             %
%  Permission to use, copy, modify, distribute, and sell this software and    %
%  its documentation for any purpose is hereby granted without fee,           %
%  provided that the above Copyright notice appear in all copies and that     %
%  both that Copyright notice and this permission notice appear in            %
%  supporting documentation, and that the name of E. I. Dupont de Nemours     %
%  and Company not be used in advertising or publicity pertaining to          %
%  distribution of the software without specific, written prior               %
%  permission.  E. I. Dupont de Nemours and Company makes no representations  %
%  about the suitability of this software for any purpose.  It is provided    %
%  "as is" without express or implied warranty.                               %
%                                                                             %
%  E. I. Dupont de Nemours and Company disclaims all warranties with regard   %
%  to this software, including all implied warranties of merchantability      %
%  and fitness, in no event shall E. I. Dupont de Nemours and Company be      %
%  liable for any special, indirect or consequential damages or any           %
%  damages whatsoever resulting from loss of use, data or profits, whether    %
%  in an action of contract, negligence or other tortuous action, arising     %
%  out of or in connection with the use or performance of this software.      %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Convert converts an input file using one image format to an output file
%  with a differing image format.
%
%  The convert program syntax is:
%
%  Usage: convert [options ...] input_file output_file
%
%  Where options include:
%    -blur                apply a filter to blur the image
%    -border geometry     surround image with a border of color
%    -colors value        preferred number of colors in the image
%    -clip geometry       preferred size and location of the clipped image
%    -colorspace type     GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string      annotate image with comment
%    -compress type       RunlengthEncoded or QEncoded
%    -density geometry    vertical and horizontal density of the image
%    -despeckle           reduce the speckles within an image"
%    -display server      obtain image or font from this X server
%    -dither              apply Floyd/Steinberg error diffusion to image
%    -edge                apply a filter to detect edges in the image
%    -enhance             apply a digital filter to enhance a noisy image
%    -equalize            perform histogram equalization to an image
%    -flip                flip image in the vertical direction
%    -flop                flop image in the horizontal direction
%    -font name           X11 font for displaying text
%    -gamma value         level of gamma correction
%    -geometry geometry   width and height of the image
%    -interlace type      NONE, LINE, or PLANE
%    -label name          assign a label to an image
%    -matte               store matte channel if the image has one
%    -monochrome          transform image to black and white
%    -negate              apply color inversion to image
%    -noise               reduce noise with a noise peak elimination filter
%    -normalize           transform image to span the full range of colors
%    -page geometry       size and location of the Postscript page
%    -quality value       JPEG quality setting
%    -roll geometry       roll an image vertically or horizontally
%    -rotate degrees      apply Paeth rotation to the image
%    -sample geometry     scale image with pixel sampling
%    -scene value         image scene number
%    -sharpen             apply a filter to sharpen the image
%    -shear geometry      slide one edge of the image along the X or Y axis
%    -treedepth value     depth of the color classification tree
%    -undercolor geometry control undercolor removal and black generation
%    -verbose             print detailed information about the image
%
%  Change '-' to '+' in any option above to reverse its effect.  For
%  example,  specify +matte to store the image without its matte channel.
%
%  By default, the image format of `file' is determined by its magic
%  number.  To specify a particular image format, precede the filename
%  with an image format name and a colon (i.e. ps:image) or specify the
%  image type as the filename suffix (i.e. image.ps).  Specify 'file' as
%  '-' for standard input or output.
%
%  Convert recognizes the following image formats:
%
%    Tag   Description
%    ---------------------------------------------------
%    AVS   AVS X image file.
%    BMP   Microsoft Windows bitmap image file.
%    CMYK  Raw cyan, magenta, yellow, and black bytes.
%    EPS   Adobe Encapsulated PostScript file.
%    EPSF  Adobe Encapsulated PostScript file.
%    EPSI  Adobe Encapsulated PostScript Interchange format.
%    FAX   Group 3.
%    FITS  Flexible Image Transport System.
%    GIF   Compuserve Graphics image file.
%    GIF87 Compuserve Graphics image file (version 87a).
%    GRAY  Raw gray bytes.
%    HISTOGRAM
%    IRIS  SGI RGB image file.
%    JPEG
%    MAP   colormap intensities and indices
%    MATTE Raw matte bytes.
%    MIFF  Magick image file format.
%    MTV
%    PCD   Photo CD
%    PCX   ZSoft IBM PC Paintbrush file
%    PICT  Apple Macintosh QuickDraw/PICT file.
%    PNM   Portable bitmap.
%    PS    Adobe PostScript file.
%    PS2   Adobe Level II PostScript file.
%    RGB   Raw red, green, and blue bytes.
%    RLE   Utah Run length encoded image file; read only.
%    SUN   SUN Rasterfile.
%    TEXT  raw text file; read only.
%    TGA   Truevision Targa image file.
%    TIFF  Tagged Image File Format.
%    VICAR read only.
%    VIFF  Khoros Visualization image file.
%    X     select image from X server screen.
%    XC    constant image of X server color.
%    XBM   X11 bitmap file.
%    XPM   X11 pixmap file.
%    XWD   X Window System window dump image file.
%    YUV   CCIR 601 1:1:1 file.
%    YUV3  CCIR 601 2:1:1 files.
%
%
*/

#include "magick.h"
#include "image.h"
#include "X.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U s a g e                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Procedure Usage displays the program usage;
%
%  The format of the Usage routine is:
%
%      Usage()
%
%
*/
static void Usage()
{
  char
    **p;

  static char
    *ImageTypes[]=
    {
      "Tag   Description",
      "------------------------------------------------------------",
      "AVS   AVS X image file.",
      "BMP   Microsoft Windows bitmap image file.",
      "CMYK  Raw cyan, magenta, yellow, and black bytes.",
      "EPS   Adobe Encapsulated PostScript file.",
      "EPSF  Adobe Encapsulated PostScript file.",
      "EPSI  Adobe Encapsulated PostScript Interchange format.",
      "FAX   Group 3.",
      "FITS  Flexible Image Transport System.",
      "GIF   Compuserve Graphics image file.",
      "GIF87 Compuserve Graphics image file (version 87a).",
      "GRAY  Raw gray bytes.",
      "HISTOGRAM",
      "IRIS  SGI RGB image file.",
      "JPEG",
      "MAP   colormap intensities and indices.",
      "MATTE Raw matte bytes.",
      "MIFF  Magick image file format.",
      "MTV",
      "PCD   Photo CD.",
      "PCX   ZSoft IBM PC Paintbrush file.",
      "PICT  Apple Macintosh QuickDraw/PICT file.",
      "PNM   Portable bitmap.",
      "PS    Adobe PostScript file.",
      "PS2   Adobe Level II PostScript file.",
      "RGB   Raw red, green, and blue bytes.",
      "RLE   Utah Run length encoded image file; read only.",
      "SUN   SUN Rasterfile.",
      "TEXT  raw text file; read only.",
      "TGA   Truevision Targa image file.",
      "TIFF  Tagged Image File Format.",
      "VICAR read only.",
      "VIFF  Khoros Visualization image file.",
      "X     select image from X server screen.",
      "XC    constant image of X server color.",
      "XBM   X11 bitmap file.",
      "XPM   X11 pixmap file.",
      "XWD   X Window System window dump image file.",
      "YUV   CCIR 601 1:1:1 file.",
      "YUV3  CCIR 601 2:1:1 files.",
      (char *) NULL,
    },
    *options[]=
    {
      "-blur                apply a filter to blur the image",
      "-border geometry     surround image with a border of color",
      "-clip geometry       preferred size and location of the clipped image",
      "-colors value        preferred number of colors in the image",
      "-colorspace type     GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-comment string      annotate image with comment",
      "-compress type       RunlengthEncoded or QEncoded",
      "-density geometry    vertical and horizontal density of the image",
      "-despeckle           reduce the speckles within an image",
      "-display server      obtain image or font from this X server",
      "-dither              apply Floyd/Steinberg error diffusion to image",
      "-edge                apply a filter to detect edges in the image",
      "-enhance             apply a digital filter to enhance a noisy image",
      "-equalize            perform histogram equalization to an image",
      "-flip                flip image in the vertical direction",
      "-flop                flop image in the horizontal direction",
      "-font name           X11 font for displaying text",
      "-gamma value         level of gamma correction",
      "-geometry geometry   width and height of the image",
      "-interlace type      NONE, LINE, or PLANE",
      "-label name          assign a label to an image",
      "-matte               store matte channel if the image has one",
      "-monochrome          transform image to black and white",
      "-negate              apply color inversion to image",
      "-noise               reduce noise with a noise peak elimination filter",
      "-normalize           transform image to span the full range of colors",
      "-page geometry       size and location of the Postscript page",
      "-quality value       JPEG quality setting",
      "-roll geometry       roll an image vertically or horizontally",
      "-rotate degrees      apply Paeth rotation to the image",
      "-sample geometry     scale image with pixel sampling",
      "-scene value         image scene number",
      "-sharpen             apply a filter to sharpen the image",
      "-shear geometry      slide one edge of the image along the X or Y axis",
      "-treedepth value     depth of the color classification tree",
      "-undercolor geometry control undercolor removal and black generation",
      "-verbose             print detailed information about the image",
      (char *) NULL
    };
  (void) fprintf(stderr,"Usage: %s [options ...] input_file output_file\n",
    client_name);
  (void) fprintf(stderr,"\nWhere options include:\n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nChange '-' to '+' in any option above to reverse its effect.  For\n");
  (void) fprintf(stderr,
    "example,  specify +matte to store the image without an matte channel.\n");
  (void) fprintf(stderr,
    "\nBy default, the image format of `file' is determined by its magic\n");
  (void) fprintf(stderr,
    "number.  To specify a particular image format, precede the filename\n");
  (void) fprintf(stderr,
    "with an image format name and a colon (i.e. ps:image) or specify the\n");
  (void) fprintf(stderr,
    "image type as the filename suffix (i.e. image.ps).  Specify 'file' as\n");
  (void) fprintf(stderr,"'-' for standard input or output.\n");
  (void) fprintf(stderr,"\nThe following image formats are recognized: \n\n");
  for (p=ImageTypes; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  M a i n                                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/
int main(argc,argv)
int
  argc;

char
  *argv[];
{
#define NotInitialized  (unsigned int) (~0)

  char
    *border_color,
    *comment,
    *density,
    *filename,
    *font,
    *label,
    *image_geometry,
    *option,
    *page_geometry,
    *server_name,
    *undercolor_geometry;

  ColorPacket
    border_packet;

  double
    normalized_maximum_error,
    normalized_mean_error;

  Image
    *image,
    *next_image;

  ImageInfo
    image_info;

  int
    flags,
    status,
    x,
    y;

  register int
    i;

  time_t
    start_time;

  unsigned int
    colorspace,
    compression,
    dither,
    height,
    interlace,
    matte,
    monochrome,
    mean_error_per_pixel,
    number_colors,
    quality,
    scene,
    tree_depth,
    verbose,
    width;

  unsigned long
    total_colors;

  /*
    Initialize program variables.
  */
  client_name=argv[0];
  if (argc < 3)
    Usage();
  /*
    Read image and convert to MIFF format.
  */
  border_color=(char *) NULL;
  border_packet.red=0;
  border_packet.green=0;
  border_packet.blue=0;
  colorspace=RGBColorspace;
  compression=UndefinedCompression;
  comment=(char *) NULL;
  density=(char *) NULL;
  dither=False;
  font=(char *) NULL;
  image=(Image *) NULL;
  image_geometry=(char *) NULL;
  interlace=NoneInterlace;
  label=(char *) NULL;
  matte=NotInitialized;
  monochrome=False;
  number_colors=0;
  page_geometry=(char *) NULL;
  quality=85;
  scene=0;
  server_name=(char *) NULL;
  start_time=time((time_t *) NULL);
  tree_depth=0;
  undercolor_geometry=(char *) NULL;
  verbose=False;
  /*
    Check command syntax.
  */
  filename=(char *) NULL;
  for (i=1; i < (argc-1); i++)
  {
    option=argv[i];
    if (((int) strlen(option) < 2) || ((*option != '-') && (*option != '+')))
      {
        /*
          Read input image.
        */
        filename=argv[i];
        GetImageInfo(filename,&image_info);
        image_info.server_name=server_name;
        image_info.font=font;
        image_info.geometry=image_geometry;
        image_info.page=page_geometry;
        image_info.density=density;
        image_info.interlace=interlace;
        image_info.monochrome=monochrome;
        image_info.quality=quality;
        image_info.verbose=verbose;
        image_info.undercolor=undercolor_geometry;
        if (image != (Image *) NULL)
          Error("input image already specified",filename);
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          exit(1);
      }
    else
      switch(*(option+1))
      {
        case 'b':
        {
          if (strncmp("blur",option+1,2) == 0)
            break;
          if (strncmp("border",option+1,7) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing geometry on -border",(char *) NULL);
                }
              break;
            }
          if (strncmp("bordercolor",option+1,7) == 0)
            {
              border_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing color on -bordercolor",(char *) NULL);
                  border_color=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'c':
        {
          if (strncmp("clip",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -clip",(char *) NULL);
                }
              break;
            }
          if (strncmp("colors",option+1,7) == 0)
            {
              number_colors=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing colors on -colors",(char *) NULL);
                  number_colors=atoi(argv[i]);
                }
              break;
            }
          if (strncmp("colorspace",option+1,7) == 0)
            {
              colorspace=RGBColorspace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -colorspace",(char *) NULL);
                  option=argv[i];
                  colorspace=UndefinedColorspace;
                  if (Latin1Compare("gray",option) == 0)
                    {
                      colorspace=GRAYColorspace;
                      number_colors=256;
                      tree_depth=8;
                    }
                  if (Latin1Compare("ohta",option) == 0)
                    colorspace=OHTAColorspace;
                  if (Latin1Compare("rgb",option) == 0)
                    colorspace=RGBColorspace;
                  if (Latin1Compare("xyz",option) == 0)
                    colorspace=XYZColorspace;
                  if (Latin1Compare("ycbcr",option) == 0)
                    colorspace=YCbCrColorspace;
                  if (Latin1Compare("yiq",option) == 0)
                    colorspace=YIQColorspace;
                  if (Latin1Compare("yuv",option) == 0)
                    colorspace=YUVColorspace;
                  if (colorspace == UndefinedColorspace)
                    Error("Invalid colorspace type on -colorspace",option);
                }
              break;
            }
          if (strncmp("comment",option+1,4) == 0)
            {
              comment=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing comment on -comment",(char *) NULL);
                  comment=argv[i];
                }
              break;
            }
          if (strncmp("compress",option+1,4) == 0)
            {
              compression=NoCompression;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -compress",(char *) NULL);
                  option=argv[i];
                  if (Latin1Compare("runlengthencoded",option) == 0)
                    compression=RunlengthEncodedCompression;
                  else
                    if (Latin1Compare("qencoded",option) == 0)
                      compression=QEncodedCompression;
                    else
                      Error("Invalid compression type on -compress",option);
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'd':
        {
          if (strncmp("density",option+1,3) == 0)
            {
              density=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -density",(char *) NULL);
                  density=argv[i];
                }
              break;
            }
          if (strncmp("despeckle",option+1,3) == 0)
            break;
          if (strncmp("display",option+1,3) == 0)
            {
              server_name=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing server name on -display",(char *) NULL);
                  server_name=argv[i];
                }
              break;
            }
          if (strncmp("dither",option+1,3) == 0)
            {
              dither=(*option == '-');
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'e':
        {
          if (strncmp("edge",option+1,2) == 0)
            break;
          if (strncmp("enhance",option+1,2) == 0)
            break;
          if (strncmp("equalize",option+1,2) == 0)
            break;
          Error("Unrecognized option",option);
          break;
        }
        case 'f':
        {
          if (strncmp("flip",option+1,3) == 0)
            break;
          if (strncmp("flop",option+1,3) == 0)
            break;
          if (strncmp("font",option+1,2) == 0)
            {
              font=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing font name on -font",(char *) NULL);
                  font=argv[i];
                }
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'g':
        {
          if (strncmp("gamma",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing value on -gamma",(char *) NULL);
                }
              break;
            }
          if (strncmp("geometry",option+1,2) == 0)
            {
              image_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -geometry",(char *) NULL);
                  image_geometry=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'h':
        {
          Usage();
          break;
        }
        case 'i':
        {
          if (strncmp("interlace",option+1,3) == 0)
            {
              interlace=NoneInterlace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -interlace",(char *) NULL);
                  option=argv[i];
                  interlace=UndefinedInterlace;
                  if (Latin1Compare("none",option) == 0)
                    interlace=NoneInterlace;
                  if (Latin1Compare("line",option) == 0)
                    interlace=LineInterlace;
                  if (Latin1Compare("plane",option) == 0)
                    interlace=PlaneInterlace;
                  if (interlace == UndefinedInterlace)
                    Error("Invalid interlace type on -interlace",option);
                }
              break;
            }
          Error("Unrecognized option",option);
        }
        case 'l':
        {
          if (strncmp("label",option+1,2) == 0)
            {
              label=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing label name on -label",(char *) NULL);
                  label=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'm':
        {
          if (strncmp("matte",option+1,5) == 0)
            {
              matte=(*option == '-');
              break;
            }
          if (strncmp("monochrome",option+1,2) == 0)
            {
              monochrome=(*option == '-');
              if (monochrome)
                {
                  number_colors=2;
                  tree_depth=8;
                  colorspace=GRAYColorspace;
                }
              break;
            }
          Error("Unrecognized option",option);
        }
        case 'n':
        {
          if (strncmp("negate",option+1,3) == 0)
            break;
          if (strncmp("noise",option+1,3) == 0)
            break;
          if (strncmp("normalize",option+1,3) == 0)
            break;
          Error("Unrecognized option",option);
          break;
        }
        case 'p':
        {
          if (strncmp("page",option+1,2) == 0)
            {
              page_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing page geometry on -page",(char *) NULL);
                  page_geometry=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'q':
        {
          i++;
          if ((i == argc) || !sscanf(argv[i],"%d",&x))
            Error("Missing quality on -quality",(char *) NULL);
          quality=atoi(argv[i]);
          break;
        }
        case 'r':
        {
          if (strncmp("roll",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -roll",(char *) NULL);
                }
              break;
            }
          if (strncmp("rotate",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing degrees on -rotate",(char *) NULL);
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 's':
        {
          if (strncmp("sample",option+1,2) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -sample",(char *) NULL);
                }
              break;
            }
          if (strncmp("scene",option+1,3) == 0)
            {
              scene=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing scene number on -scene",(char *) NULL);
                  scene=atoi(argv[i]);
                }
              break;
            }
          if (strncmp("sharpen",option+1,3) == 0)
            break;
          if (strncmp("shear",option+1,3) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing shear geometry on -shear",(char *) NULL);
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 't':
        {
          tree_depth=0;
          if (*option == '-')
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%d",&x))
                Error("Missing depth on -treedepth",(char *) NULL);
              tree_depth=atoi(argv[i]);
            }
          break;
        }
        case 'u':
        {
          if (strncmp("undercolor",option+1,2) == 0)
            {
              undercolor_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing undercolor geometry on -undercolor",
                      (char *) NULL);
                  undercolor_geometry=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'v':
        {
          if (strncmp("verbose",option+1,1) == 0)
            {
              verbose=(*option == '-');
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        default:
        {
          Error("Unrecognized option",option);
          break;
        }
      }
  }
  if (image == (Image *) NULL)
    Error("Missing an image file name",(char *) NULL);
  /*
    Write images.
  */
  do
  {
    total_colors=0;
    if (matte != NotInitialized)
      image->matte=matte;
    if (compression != UndefinedCompression)
      image->compression=compression;
    if (scene != 0)
      image->scene=scene;
    LabelImage(image,label);
    if (comment != (char *) NULL)
      CommentImage(image,comment);
    (void) strcpy(image->filename,argv[i]);
    /*
      Transform image as defined by the image geometries.
    */
    for (i=1; i < (argc-1); i++)
    {
      option=argv[i];
      if (strncmp("-blur",option,3) == 0)
        {
          Image
            *blurred_image;

          /*
            Blur the image.
          */
          blurred_image=BlurImage(image);
          if (blurred_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=blurred_image;
            }
        }
      if (strcmp("-border",option) == 0)
        {
          Image
            *bordered_image;

          RectangleInfo
            border_info;

          /*
            Surround image with a border of solid color.
          */
          border_info.width=0;
          border_info.height=0;
          flags=XParseGeometry(argv[++i],&border_info.x,&border_info.y,
            &border_info.width,&border_info.height);
          if ((flags & HeightValue) == 0)
            border_info.height=border_info.width;
          bordered_image=BorderImage(image,&border_info,&border_packet);
          if (bordered_image != (Image *) NULL)
            {
              DestroyImage(image);
              bordered_image->class=DirectClass;
              image=bordered_image;
            }
        }
      if (strncmp("-bordercolor",option,8) == 0)
        {
          Display
            *display;

          i++;
          display=XOpenDisplay((char *) NULL);
          if (display != (Display *) NULL)
            {
              Colormap
                colormap;
        
              XColor
                color;
        
              /*
                Obtain border color from the X server.
              */
              colormap=XDefaultColormap(display,XDefaultScreen(display));
              (void) XParseColor(display,colormap,argv[i],&color);
              border_packet.red=color.red >> 8;
              border_packet.green=color.green >> 8;
              border_packet.blue=color.blue >> 8;
              XCloseDisplay(display);
            }
        }
      if (strncmp("-clip",option,3) == 0)
        TransformImage(&image,argv[++i],(char *) NULL);
      if (strncmp("-despeckle",option,4) == 0)
        {
          Image
            *despeckled_image;

          /*
            Reduce the speckles within an image.
          */
          despeckled_image=DespeckleImage(image);
          if (despeckled_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=despeckled_image;
            }
        }
      if (strncmp("-edge",option,3) == 0)
        {
          Image
            *edged_image;

          /*
            Detect edges within an image.
          */
          edged_image=EdgeImage(image);
          if (edged_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=edged_image;
            }
        }
      if (strncmp("-enhance",option,3) == 0)
        {
          Image
            *enhanced_image;

          /*
            Enhance image.
          */
          enhanced_image=EnhanceImage(image);
          if (enhanced_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=enhanced_image;
            }
        }
      if (strncmp("-equalize",option,3) == 0)
        EqualizeImage(image);
      if (strncmp("-flip",option,4) == 0)
        {
          Image
            *flipped_image;

          /*
            Flip image scanlines.
          */
          flipped_image=FlipImage(image);
          if (flipped_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=flipped_image;
            }
        }
      if (strncmp("-flop",option,4) == 0)
        {
          Image
            *flopped_image;

          /*
            Flop image scanlines.
          */
          flopped_image=FlopImage(image);
          if (flopped_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=flopped_image;
            }
        }
      if (strncmp("-gamma",option,3) == 0)
        GammaImage(image,argv[++i]);
      if (strncmp("-geometry",option,4) == 0)
        TransformImage(&image,(char *) NULL,argv[++i]);
      if (strncmp("-negate",option,4) == 0)
        NegateImage(image);
      if (strncmp("-noise",option,4) == 0)
        {
          Image
            *noisy_image;

          /*
            Reduce noise in image.
          */
          noisy_image=NoisyImage(image);
          if (noisy_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=noisy_image;
            }
        }
      if (strncmp("-normalize",option,4) == 0)
        NormalizeImage(image);
      if (strncmp("-roll",option,4) == 0)
        {
          Image
            *rolled_image;

          /*
            Roll image.
          */
          x=0;
          y=0;
          flags=XParseGeometry(argv[++i],&x,&y,&width,&height);
          rolled_image=RollImage(image,x,y);
          if (rolled_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=rolled_image;
            }
        }
      if (strncmp("-rotate",option,4) == 0)
        {
          Image
            *rotated_image;

          /*
            Rotate image.
          */
          rotated_image=RotateImage(image,(double) atof(argv[++i]),
            &border_packet,False);
          if (rotated_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=rotated_image;
            }
        }
      if (strncmp("-sample",option,3) == 0)
        {
          Image
            *sampled_image;

          unsigned int
            height,
            width;

          /*
            Sample image with pixel replication.
          */
          width=image->columns;
          height=image->rows;
          ParseImageGeometry(argv[++i],&width,&height);
          sampled_image=SampleImage(image,width,height);
          if (sampled_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=sampled_image;
            }
        }
      if (strncmp("-sharpen",option,4) == 0)
        {
          Image
            *sharpened_image;

          /*
            Sharpen the image.
          */
          sharpened_image=SharpenImage(image);
          if (sharpened_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=sharpened_image;
            }
        }
      if (strncmp("-shear",option,4) == 0)
        {
          float
            x_shear,
            y_shear;

          Image
            *sheared_image;

          /*
            Shear image.
          */
          x_shear=0.0;
          y_shear=0.0;
          (void) sscanf(argv[++i],"%fx%f",&x_shear,&y_shear);
          sheared_image=ShearImage(image,(double) x_shear,(double) y_shear,
            &border_packet,False);
          if (sheared_image != (Image *) NULL)
            {
              DestroyImage(image);
              image=sheared_image;
            }
      }
    }
    if (number_colors != 0)
      {
        /*
          Reduce the number of colors in the image.
        */
        if ((image->class == DirectClass) || (image->colors > number_colors) ||
            (colorspace == GRAYColorspace))
          QuantizeImage(image,number_colors,tree_depth,dither,colorspace,True);
        if (verbose)
          {
            /*
              Measure quantization error.
            */
            QuantizationError(image,&mean_error_per_pixel,
              &normalized_mean_error,&normalized_maximum_error);
            total_colors=NumberColors(image,(FILE *) NULL);
          }
        SyncImage(image);
      }
    status=WriteImage(&image_info,image);
    if (verbose)
      {
        /*
          Display detailed info about the image.
        */
        (void) fprintf(stderr,"[%u] %s=>%s %ux%u",image->scene,filename,
          image->filename,image->columns,image->rows);
        if (image->class == DirectClass)
          (void) fprintf(stderr," DirectClass");
        else
          if (total_colors == 0)
            (void) fprintf(stderr," PseudoClass %uc",image->colors);
          else
            {
              (void) fprintf(stderr," PseudoClass %lu=>%uc",total_colors,
                image->colors);
              (void) fprintf(stderr," %u/%.6f/%.6fe",mean_error_per_pixel,
                normalized_mean_error,normalized_maximum_error);
            }
        (void) fprintf(stderr," %s %lds\n",image->magick,
          time((time_t *) NULL)-start_time+1);
      }
    next_image=image->next;
    image=next_image;
  } while (image != (Image *) NULL);
  return(!status);
}
