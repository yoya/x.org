/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%              M   M   OOO    GGGG  RRRR   IIIII  FFFFF  Y   Y                %
%              MM MM  O   O  G      R   R    I    F       Y Y                 %
%              M M M  O   O  G GG   RRRRR    I    FFF      Y                  %
%              M   M  O   O  G   G  R R      I    F        Y                  %
%              M   M   OOO    GGG   R  R   IIIII  F        Y                  %
%                                                                             %
%                                                                             %
%               Transmogrify an Image or Sequence of Images.                  %
%                                                                             %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                            December 1992                                    %
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
%  `Mogrify' applies one or more image transforms to an image or sequence of
%   images and overwrites the original image.
%
%  The Mogrify program command syntax is:
%
%  Usage: mogrify [options ...] file [ [options ...] file ...]
%
%  Where options include:
%    -blur                apply a filter to blur the image
%    -border geometry     surround image with a border of color
%    -clip geometry       preferred size and location of the clipped image
%    -colormap filename   transform image colors to match this set of colors
%    -colors value        preferred number of colors in the image
%    -colorspace type     GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string      annotate image with comment
%    -compress type       RunlengthEncoded or QEncoded
%    -density geometry    vertical and horizontal density of the image
%    -despeckle           reduce the speckles within an image",
%    -display server      obtain image or font from this X server
%    -dither              apply Floyd/Steinberg error diffusion to image
%    -edge                apply a filter to detect edges in the image
%    -enhance             apply a digital filter to enhance a noisy image
%    -equalize            perform histogram equalization to an image
%    -flip                flip image in the vertical direction
%    -flop                flop image in the horizontal direction
%    -font name           X11 font for displaying text
%    -gamma value         level of gamma correction
%    -geometry geometry   preferred size of the image
%    -interlace type      NONE, LINE, or PLANE
%    -label name          assign a label to an image
%    -monochrome          transform image to black and white
%    -negate              apply color inversion to image
%    -noise               reduce noise with a noise peak elimination filter
%    -normalize           transform image to span the full range of colors
%    -page geometry       size and location of the Postscript page
%    -quality value       JPEG quality setting
%    -roll geometry       roll an image vertically or horizontally
%    -rotate degrees      apply Paeth rotation to the image
%    -scene value         image scene number
%    -sharpen             apply a filter to sharpen the image
%    -shear geometry      slide one edge of the image along the X or Y axis
%    -treedepth value     depth of the color classification tree
%    -undercolor geometry control undercolor removal and black generation
%    -verbose             print detailed information about the image
%
%  Change '-' to '+' in any option above to reverse its effect.  For
%  example, specify +compress to store the image as uncompressed.
%
%  By default, the image format of `file' is determined by its magic
%  number.  To specify a particular image format, precede the filename
%  with an image format name and a colon (i.e. ps:image) or specify the
%  image type as the filename suffix (i.e. image.ps).  Specify 'file' as
%  '-' for standard input or output.
%
%
*/

/*
  Include declarations.
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
%  Function Usage displays the program command syntax.
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
    *options[]=
    {
      "-blur                apply a filter to blur the image",
      "-border geometry     surround image with a border of color",
      "-clip geometry       preferred size and location of the clipped image",
      "-colormap filename   transform image colors to match this set of colors",
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
      "-geometry geometry   preferred size of the image",
      "-interlace type      NONE, LINE, or PLANE",
      "-label name          assign a label to an image",
      "-monochrome          transform image to black and white",
      "-negate              apply color inversion to image",
      "-noise               reduce noise with a noise peak elimination filter",
      "-normalize           transform image to span the full range of colors",
      "-page geometry       size and location of the Postscript page",
      "-quality value       JPEG quality setting",
      "-roll geometry       roll an image vertically or horizontally",
      "-rotate degrees      apply Paeth rotation to the image",
      "-scene number        image scene number",
      "-sharpen             apply a filter to sharpen the image",
      "-shear geometry      slide one edge of the image along the X or Y axis",
      "-treedepth value     depth of the color classification tree",
      "-undercolor geometry control undercolor removal and black generation",
      "-verbose             print detailed information about the image",
      (char *) NULL
    };
  (void) fprintf(stderr,
    "Usage: %s [-options ...] file [ [-options ...] file ...]\n",client_name);
  (void) fprintf(stderr,"\nWhere options include: \n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nChange '-' to '+' in any option above to reverse its effect.  For\n");
  (void) fprintf(stderr,
    "example, specify +compress to store the image as uncompressed.\n");
  (void) fprintf(stderr,
    "\nBy default, the image format of `file' is determined by its magic\n");
  (void) fprintf(stderr,
    "number.  To specify a particular image format, precede the filename\n");
  (void) fprintf(stderr,
    "with an image format name and a colon (i.e. ps:image) or specify the\n");
  (void) fprintf(stderr,
    "image type as the filename suffix (i.e. image.ps).  Specify 'file' as\n");
  (void) fprintf(stderr,"'-' for standard input or output.\n");
  exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%    M a i n                                                                  %
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
  **argv;
{
  char
    *border_color,
    *colormap_filename,
    *comment,
    *density,
    *font,
    *gamma,
    *image_geometry,
    *label,
    *option,
    *page_geometry,
    *server_name,
    *undercolor_geometry;

  ColorPacket
    border_packet;

  Image
    **images;

  ImageInfo
    image_info;

  int
    flags,
    i,
    p,
    q,
    x,
    y;

  unsigned int
    colorspace,
    compression,
    dither,
    height,
    image_number,
    interlace,
    maximum_images,
    monochrome,
    number_colors,
    quality,
    scene,
    tree_depth,
    verbose,
    width;

  /*
    Display usage profile if there are no command line arguments.
  */
  client_name=(*argv);
  if (argc < 2)
    Usage();
  /*
    Set defaults.
  */
  border_color=(char *) NULL;
  border_packet.red=0;
  border_packet.green=0;
  border_packet.blue=0;
  colormap_filename=(char *) NULL;
  colorspace=RGBColorspace;
  comment=(char *) NULL;
  compression=UndefinedCompression;
  density=(char *) NULL;
  dither=False;
  font=(char *) NULL;
  gamma=(char *) NULL;
  image_geometry=(char *) NULL;
  interlace=NoneInterlace;
  label=(char *) NULL;
  monochrome=False;
  number_colors=0;
  quality=85;
  page_geometry=(char *) NULL;
  server_name=(char *) NULL;
  scene=0;
  tree_depth=0;
  undercolor_geometry=(char *) NULL;
  verbose=False;
  maximum_images=MaxTextLength;
  images=(Image **) malloc(maximum_images*sizeof(Image *));
  if (images == (Image **) NULL)
    Error("Unable to mogrify images","Memory allocation failed");
  /*
    Parse command line.
  */
  image_number=0;
  p=1;
  for (i=1; i < argc; i++)
  {
    option=argv[i];
    if (((int) strlen(option) > 1) && ((*option == '-') || (*option == '+')))
      switch (*(option+1))
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
          if (strncmp("colormap",option+1,7) == 0)
            {
              colormap_filename=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing file name on -colormap",(char *) NULL);
                  colormap_filename=argv[i];
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
          if (strncmp("compress",option+1,3) == 0)
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
              gamma=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                    Error("Missing value on -gamma",(char *) NULL);
                  gamma=argv[i];
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
          if (strncmp("help",option+1,2) == 0)
            {
              Usage();
              break;
            }
          Error("Unrecognized option",option);
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
          break;
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
          monochrome=(*option == '-');
          if (monochrome)
            {
              number_colors=2;
              tree_depth=8;
              colorspace=GRAYColorspace;
            }
          break;
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
          quality=atoi(argv[i]);;
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
          verbose=(*option == '-');
          break;
        }
        default:
        {
          Error("Unrecognized option",option);
          break;
        }
      }
    else
      {
        double
          normalized_maximum_error,
          normalized_mean_error;

        Image
          *image,
          info_image,
          *next_image;

        time_t
          start_time;

        unsigned int
          mean_error_per_pixel;

        unsigned long
          total_colors;

        /*
          Option is a file name: begin by reading image from specified file.
        */
        start_time=time((time_t *) NULL);
        GetImageInfo(argv[i],&image_info);
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
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          if (*option == '-')
            break;
          else
            continue;
        do
        {
          info_image=(*image);
          if (scene != 0)
            image->scene=scene;
          LabelImage(image,label);
          if (comment != (char *) NULL)
            CommentImage(image,comment);
          total_colors=0;
          /*
            Transform image as defined by the image geometries.
          */
          for (q=p; q < i; q++)
          {
            option=argv[q];
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
                RectangleInfo
                  border_info;

                Image
                  *bordered_image;

                /*
                  Surround image with a border of solid color.
                */
                border_info.width=0;
                border_info.height=0;
                flags=XParseGeometry(argv[++q],&border_info.x,&border_info.y,
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
              TransformImage(&image,argv[++q],(char *) NULL);
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
                  Detect edges in the image.
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
              GammaImage(image,argv[++q]);
            if (strncmp("-geometry",option,4) == 0)
              TransformImage(&image,(char *) NULL,argv[++q]);
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
                flags=XParseGeometry(argv[++q],&x,&y,&width,&height);
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
                rotated_image=RotateImage(image,(double) atof(argv[++q]),
                  &border_packet,False);
                if (rotated_image != (Image *) NULL)
                  {
                    DestroyImage(image);
                    image=rotated_image;
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
                (void) sscanf(argv[++q],"%fx%f",&x_shear,&y_shear);
                sheared_image=ShearImage(image,(double) x_shear,
                  (double) y_shear,&border_packet,False);
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
              if ((image->class == DirectClass) ||
                  (image->colors > number_colors) ||
                  (colorspace == GRAYColorspace))
                QuantizeImage(image,number_colors,tree_depth,dither,colorspace,
                  True);
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
          if (verbose)
            {
              /*
                Display detailed info about the image.
              */
              (void) fprintf(stderr,"[%u] %s",
                image->scene == 0 ? image_number : image->scene,
                info_image.filename);
              (void) fprintf(stderr," %ux%u",info_image.columns,
                info_image.rows);
              if ((info_image.columns != image->columns) ||
                  (info_image.rows != image->rows))
                (void) fprintf(stderr,"=>%ux%u",image->columns,image->rows);
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
          if (compression != UndefinedCompression)
            image->compression=compression;
          else
            image->compression=info_image.compression;
          if (number_colors == 0)
            (void) WriteImage(&image_info,image);
          if (image_number == maximum_images)
            {
              /*
                Increase size of images array.
              */
              maximum_images<<=1;
              images=(Image **)
                realloc((char *) images,maximum_images*sizeof(Image *));
              if (images == (Image **) NULL)
                Error("Unable to mogrify images","Memory allocation failed");
            }
          images[image_number++]=image;
          next_image=image->next;
          if (next_image != (Image *) NULL)
            image=next_image;
        } while (next_image != (Image *) NULL);
        if (number_colors == 0)
          DestroyImages(image);
        p=i+1;
      }
    }
  if (image_number == 0)
    Error("Missing an image file name",(char *) NULL);
  if (number_colors != 0)
    {
      Image
        *colormap_image;

      /*
        Global colormap option;  reduce colors then write image.
      */
      colormap_image=(Image *) NULL;
      if (colormap_filename != (char *) NULL)
        {
          (void) strcpy(image_info.filename,colormap_filename);
          colormap_image=ReadImage(&image_info);
          if (colormap_image == (Image *) NULL)
            Error("Unable to mogrify images","cannot read image colormap");
        }
      maximum_images=image_number;
      QuantizeImages(images,maximum_images,colormap_image,number_colors,
        tree_depth,dither,colorspace,True);
      if (colormap_image != (Image *) NULL)
        DestroyImage(colormap_image);
      for (image_number=0; image_number < maximum_images; image_number++)
      {
        (void) WriteImage(&image_info,images[image_number]);
        DestroyImage(images[image_number]);
      }
    }
  (void) free((char *) images);
  return(False);
}
