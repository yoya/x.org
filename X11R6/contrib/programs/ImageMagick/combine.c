/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                CCCC   OOO   M   M  BBBB   IIIII  N   N  EEEEE               %
%               C      O   O  MM MM  B   B    I    NN  N  E                   %
%               C      O   O  M M M  BBBB     I    N N N  EEE                 %
%               C      O   O  M   M  B   B    I    N  NN  E                   %
%                CCCC   OOO   M   N  BBBB   IIIII  N   N  EEEEE               %
%                                                                             %
%                                                                             %
%                        Digitally combine two images.                        %
%                                                                             %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                               January 1993                                  %
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
%  The combine program syntax is:
%
%  Usage: combine [options ...] input_file1 input_file2 output_file
%
%  Where options include:
%    -colors value       preferred number of colors in the image
%    -compose operator   composite operator
%    -colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string     annotate image with comment
%    -compress type      RunlengthEncoded or QEncoded
%    -density geometry   vertical and horizontal density of the image
%    -display server     obtain image or font from this X server
%    -dither             apply Floyd/Steinberg error diffusion to image
%    -font name          X11 font for displaying text
%    -geometry geometry  width and height of the image
%    -interlace type     NONE, LINE, or PLANE
%    -label name         assign a label to an image
%    -matte              store matte channel if the image has one
%    -monochrome         transform image to black and white
%    -page geometry      size and location of the Postscript page
%    -quality value      JPEG quality setting
%    -scene value        image scene number
%    -stereo             combine two images to form red-green stereo image
%    -treedepth value    depth of the color classification tree
%    -verbose            print detailed information about the image
%
%  Change '-' to '+' in any option above to reverse its effect.  For
%  example,  specify +matte to store the image without its matte channel.
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
    *options[]=
    {
      "-colors value       preferred number of colors in the image",
      "-colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-comment string     annotate image with comment",
      "-compose operator   composite operator",
      "-compress type      RunlengthEncoded or QEncoded",
      "-density geometry   vertical and horizontal density of the image",
      "-display server     obtain image or font from this X server",
      "-dither             apply Floyd/Steinberg error diffusion to image",
      "-font name          X11 font for displaying text",
      "-geometry geometry  width and height of the image",
      "-interlace type     NONE, LINE, or PLANE",
      "-label name         assign a label to an image",
      "-matte              store matte channel if the image has one",
      "-monochrome         transform image to black and white",
      "-page geometry      size and location of the Postscript page",
      "-quality value      JPEG quality setting",
      "-scene value        image scene number",
      "-stereo             combine two images to form red-green stereo image",
      "-treedepth value    depth of the color classification tree",
      "-verbose            print detailed information about the image",
      (char *) NULL
    };
  (void) fprintf(stderr,
    "Usage: %s [options ...] input_file1 input_file2 output_file\n",
    client_name);
  (void) fprintf(stderr,"\nWhere options include:\n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nChange '-' to '+' in any option above to reverse its effect.  For\n");
  (void) fprintf(stderr,
    "example,  specify +matte to store the image without an matte channel.\n");
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
    *comment,
    *density,
    *filename,
    *font,
    *image_geometry,
    *label,
    *option,
    *page_geometry,
    *server_name;

  double
    normalized_maximum_error,
    normalized_mean_error;

  Image
    *alpha_image,
    *beta_image,
    *combined_image;

  ImageInfo
    image_info;

  int
    i,
    status,
    x;

  time_t
    start_time;

  unsigned int
    colorspace,
    compose,
    compression,
    dither,
    interlace,
    matte,
    mean_error_per_pixel,
    monochrome,
    number_colors,
    quality,
    scene,
    stereo,
    tree_depth,
    verbose;

  unsigned long
    total_colors;

  /*
    Initialize program variables.
  */
  client_name=argv[0];
  if (argc < 4)
    Usage();
  /*
    Read image and convert to MIFF format.
  */
  alpha_image=(Image *) NULL;
  beta_image=(Image *) NULL;
  colorspace=RGBColorspace;
  comment=(char *) NULL;
  compose=OverCompositeOp;
  compression=UndefinedCompression;
  density=(char *) NULL;
  dither=False;
  font=(char *) NULL;
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
  stereo=False;
  tree_depth=0;
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
          Read input images.
        */
        filename=argv[i];
        GetImageInfo(filename,&image_info);
        image_info.server_name=server_name;
        image_info.font=font;
        image_info.geometry=image_geometry;
        image_info.page=page_geometry;
        image_info.density=density;
        image_info.interlace=interlace;
        image_info.quality=quality;
        image_info.verbose=verbose;
        if (alpha_image == (Image *) NULL)
          {
            alpha_image=ReadImage(&image_info);
            if (alpha_image == (Image *) NULL)
              exit(1);
            continue;
          }
        if (beta_image != (Image *) NULL)
          Error("input images already specified",filename);
        beta_image=ReadImage(&image_info);
        if (beta_image == (Image *) NULL)
          exit(1);
      }
    else
      switch(*(option+1))
      {
        case 'c':
        {
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
          if (strncmp("compose",option+1,5) == 0)
            {
              compose=ReplaceCompositeOp;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -compose",(char *) NULL);
                  option=argv[i];
                  compose=UndefinedCompositeOp;
                  if (Latin1Compare("over",option) == 0)
                    compose=OverCompositeOp;
                  if (Latin1Compare("in",option) == 0)
                    compose=InCompositeOp;
                  if (Latin1Compare("out",option) == 0)
                    compose=OutCompositeOp;
                  if (Latin1Compare("atop",option) == 0)
                    compose=AtopCompositeOp;
                  if (Latin1Compare("xor",option) == 0)
                    compose=XorCompositeOp;
                  if (Latin1Compare("plus",option) == 0)
                    compose=PlusCompositeOp;
                  if (Latin1Compare("minus",option) == 0)
                    compose=MinusCompositeOp;
                  if (Latin1Compare("add",option) == 0)
                    compose=AddCompositeOp;
                  if (Latin1Compare("subtract",option) == 0)
                    compose=SubtractCompositeOp;
                  if (Latin1Compare("difference",option) == 0)
                    compose=DifferenceCompositeOp;
                  if (Latin1Compare("replace",option) == 0)
                    compose=ReplaceCompositeOp;
                  if (compose == UndefinedCompositeOp)
                    Error("Invalid compose type on -compose",option);
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
        case 'f':
        {
          font=(char *) NULL;
          if (*option == '-')
            {
              i++;
              if (i == argc)
                Error("Missing font name on -font",(char *) NULL);
              font=argv[i];
            }
          break;
        }
        case 'g':
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
        case 's':
        {
          if (strncmp("scene",option+1,2) == 0)
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
          if (strncmp("stereo",option+1,2) == 0)
            {
              stereo=(*option == '-');
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
  }
  if ((alpha_image == (Image *) NULL) || (beta_image == (Image *) NULL))
    Usage();
  /*
    Combine image.
  */
  if (stereo)
    combined_image=StereoImage(alpha_image,beta_image);
  else
    {
      int
        x_offset,
        y_offset;

      unsigned int
        size;

      /*
        Digitally composite image.
      */
      x_offset=0;
      y_offset=0;
      if (image_info.geometry != (char *) NULL)
        (void) XParseGeometry(image_info.geometry,&x_offset,&y_offset,&size,
          &size);
      CompositeImage(beta_image,compose,alpha_image,x_offset,y_offset);
      combined_image=beta_image;
    }
  if (combined_image == (Image *) NULL)
    exit(1);
  /*
    Write image.
  */
  total_colors=0;
  if (matte != NotInitialized)
    combined_image->matte=matte;
  if (compression != UndefinedCompression)
    combined_image->compression=compression;
  if (scene != 0)
    combined_image->scene=scene;
  (void) strcpy(combined_image->filename,argv[i]);
  LabelImage(combined_image,label);
  if (comment != (char *) NULL)
    CommentImage(combined_image,comment);
  if (number_colors != 0)
    {
      /*
        Reduce the number of colors in the image.
      */
      if ((combined_image->class == DirectClass) ||
          (combined_image->colors > number_colors) ||
          (colorspace == GRAYColorspace))
        QuantizeImage(combined_image,number_colors,tree_depth,dither,colorspace,
          True);
      if (verbose)
        {
          /*
            Measure quantization error.
           */
           QuantizationError(combined_image,&mean_error_per_pixel,
             &normalized_mean_error,&normalized_maximum_error);
           total_colors=NumberColors(combined_image,(FILE *) NULL);
        }
      SyncImage(combined_image);
    }
  status=WriteImage(&image_info,combined_image);
  if (verbose)
    {
      /*
        Display detailed info about the image.
      */
      (void) fprintf(stderr,"[%u] %s %s=>%s %ux%u",combined_image->scene,
        alpha_image->filename,filename,combined_image->filename,
        combined_image->columns,combined_image->rows);
      if (combined_image->class == DirectClass)
        (void) fprintf(stderr," DirectClass");
      else
        if (total_colors == 0)
          (void) fprintf(stderr," PseudoClass %uc",combined_image->colors);
        else
          {
            (void) fprintf(stderr," PseudoClass %lu=>%uc",total_colors,
              combined_image->colors);
            (void) fprintf(stderr," %u/%.6f/%.6fe",mean_error_per_pixel,
              normalized_mean_error,normalized_maximum_error);
          }
      (void) fprintf(stderr," %s %lds\n",combined_image->magick,
        time((time_t *) NULL)-start_time+1);
    }
  return(!status);
}
