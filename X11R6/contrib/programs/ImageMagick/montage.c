/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%             M   M   OOO   N   N  TTTTT   AAA    GGGG  EEEEE                 %
%             MM MM  O   O  NN  N    T    A   A  G      E                     %
%             M M M  O   O  N N N    T    AAAAA  G  GG  EEE                   %
%             M   M  O   O  N  NN    T    A   A  G   G  E                     %
%             M   M   OOO   N   N    T    A   A   GGGG  EEEEE                 %
%                                                                             %
%                                                                             %
%              Montage Magick Image File Format Image via X11.                %
%                                                                             %
%                                                                             %
%                                                                             %
%                           Software Design                                   %
%                             John Cristy                                     %
%                              July 1992                                      %
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
%  Montage creates a composite image by combining several separate
%  images.  The images are tiled on the composite image with the name of
%  the image appearing just above the individual tile.
%
%  The composite image is constructed in the following manner.  First,
%  each image specified on the command line, except for the last, is
%  scaled to fit the maximum tile size.  The maximum tile size by default
%  is 256x256.  It can be modified with the -geometry command line
%  argument or X resource.  Note that the maximum tile size need not be a
%  square.  To respect the aspect ratio of each image append ~ to the
%  geometry specification.
%
%  Next the composite image is initialized with the color specified by the
%  -background command line argument or X resource.  The width and height
%  of the composite image is determined by the maximum tile size, the
%  number of tiles per row, the tile border width and height, the image
%  border width, and the label height.  The number of tiles per row specifies
%  how many images are to appear in each row of the composite image.  The
%  default is to have an equal number of images in each row and column of the
%  composite.  This value can be specified with -tiles_per_row.  The tile
%  border width and height, and the image border width defaults to the value
%  of the X resource -borderwidth.  It can be changed with the -borderwidth or
%  -geometry command line argument or X resource.  The label height is
%  determined by the font you specify with the -font command line argument or
%  X resource.  If you do not specify a font, a font is choosen that allows
%  the name of the image to fit the maximum width of a tiled area.  The label
%  colors is determined by the -background and -foreground command line
%  argument or X resource.  Note, that if the background and foreground colors
%  are the same, labels will not appear.
%
%  Finally, each image is set onto the composite image, surrounded by its
%  border color, with its name centered just below it.  The individual images
%  are centered within the width of the tiled area.  The final argument on the
%  command line is the name assigned to the composite image.  The image is
%  written in the MIFF format and may by viewed or printed with `display'.
%
%  The Montage program command syntax is:
%
%  Usage: montage [options ...] file [ [options ...] file ...] file
%
%  Where options include:
%    -clip geometry        preferred size and location of the clipped image
%    -colors value         preferred number of colors in the image
%    -colorspace type      GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string       annotate image with comment
%    -compose operator     composite operator
%    -compress type        RunlengthEncoded or QEncoded
%    -density geometry     vertical and horizontal density of the image
%    -display server       query fonts from this X server
%    -dither               apply Floyd/Steinberg error diffusion to image
%    -frame                surround image with an ornamental border
%    -gamma value          level of gamma correction
%    -geometry geometry    preferred tile and border sizes
%    -gravity direction    which direction to gravitate towards
%    -interlace type       NONE, LINE, or PLANE
%    -label name           assign a label to an image
%    -monochrome           transform image to black and white
%    -page geometry        size and location of the Postscript page
%    -quality value        JPEG quality setting
%    -rotate degrees       apply Paeth rotation to the image
%    -scene value          image scene number
%    -tiles_per_row value  number of image tiles per row
%    -treedepth value      depth of the color classification tree
%    -verbose              print detailed information about the image
%
%  In addition to those listed above, you can specify these standard X
%  resources as command line options:  -background, -bordercolor -borderwidth,
%  -font, -foreground, -mattecolor, or -title.
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
#include "compress.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   M o n t a g e I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function MontageImage creates a composite image by combining several
%  separate images.
%
%  The format of the MontageImage routine is:
%
%      MontageImage(display,resource_info,images,number_tiles,tiles_per_row,
%        frame,compose)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
%    o number_tiles: Specifies the number of tiles to tile.
%
%    o tiles_per_row: Specifies the number of arguments.
%
%    o frame: An integer greater than zero will force an ornamental border
%      around each tile.
%
%    o compose: Specifies an image composite operator.
%
%
*/
static int SceneCompare(x,y)
const void
  *x,
  *y;
{
  Image
    **image_1,
    **image_2;

  image_1=(Image **) x;
  image_2=(Image **) y;
  return((int) (*image_1)->scene-(int) (*image_2)->scene);
}

static Image *MontageImage(display,resource_info,images,number_tiles,
  tiles_per_row,frame,compose)
Display
  *display;

XResourceInfo
  *resource_info;

Image
  **images;

unsigned int
  number_tiles,
  tiles_per_row,
  frame,
  compose;
{
  ColorPacket
    border_color,
    highlight_color,
    matte_color,
    shadow_color;

  Image
    *image,
    *montage_image;

  int
    x,
    x_offset,
    y,
    y_offset;

  register char
    *q;

  register int
    i;

  register RunlengthPacket
    *p;

  RectangleInfo
    label_info,
    tile_info;

  unsigned int
    border_width,
    bevel_width,
    count,
    max_width,
    status,
    tile;

  XAnnotateInfo
    annotate_info;

  XColor
    background_color;

  XFontStruct
    *font_info;

  XPixelInfo
    pixel_info;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  /*
    Determine tile sizes.
  */
  border_width=resource_info->border_width;
  if (frame)
    {
      bevel_width=(border_width >> 2)+1;
      border_width+=bevel_width << 1;
    }
  tile_info.x=resource_info->border_width;
  tile_info.y=resource_info->border_width;
  if (resource_info->image_geometry != (char *) NULL)
    {
      XParseGeometry(resource_info->image_geometry,&tile_info.x,
        &tile_info.y,&tile_info.width,&tile_info.height);
      if (tile_info.x < 0)
        tile_info.x=0;
      if (tile_info.y < 0)
        tile_info.y=0;
    }
  tile_info.width=images[0]->columns;
  tile_info.height=images[0]->rows;
  for (tile=1; tile < number_tiles; tile++)
  {
    if (images[tile]->columns > tile_info.width)
      tile_info.width=images[tile]->columns;
    if (images[tile]->rows > tile_info.height)
      tile_info.height=images[tile]->rows;
  }
  if (tiles_per_row == 0)
    {
      /*
        Compute tiles per row.
      */
      tiles_per_row=1;
      while ((tiles_per_row*tiles_per_row) < number_tiles)
        tiles_per_row++;
    }
  /*
    Initialize tile colors.
  */
  background_color.red=0;
  background_color.green=0;
  background_color.blue=0;
  border_color.red=0;
  border_color.green=0;
  border_color.blue=0;
  highlight_color=border_color;
  shadow_color=border_color;
  XGetAnnotateInfo(&annotate_info);
  /*
    Determine if any of the tiles have a label.
  */
  for (tile=0; tile < number_tiles; tile++)
    if (images[tile]->label != (char *) NULL)
      break;
  if (tile == number_tiles)
    display=(Display *) NULL;
  if (display != (Display *) NULL)
    {
      /*
        Initialize visual info.
      */
      map_info=XAllocStandardColormap();
      if (map_info == (XStandardColormap *) NULL)
        Error("Unable to create standard colormap","Memory allocation failed");
      visual_info=XBestVisualInfo(display,map_info,resource_info);
      if (visual_info == (XVisualInfo *) NULL)
        Error("Unable to get visual",resource_info->visual_type);
      map_info->colormap=(Colormap) NULL;
      pixel_info.pixels=(unsigned long *) NULL;
      /*
        Initialize font info.
      */
      font_info=XBestFont(display,resource_info);
      if (font_info == (XFontStruct *) NULL)
        Error("Unable to load font",resource_info->font);
      annotate_info.text=(char *) malloc(MaxTextLength*sizeof(char));
      if (annotate_info.text == (char *) NULL)
        Error("Unable to montage images","Memory allocation failed");
      annotate_info.font_info=font_info;
      annotate_info.height=font_info->ascent+font_info->descent;
      /*
        Determine background and border colors.
      */
      XGetMapInfo(visual_info,XDefaultColormap(display,visual_info->screen),
        map_info);
      XGetPixelInfo(display,visual_info,map_info,resource_info,(Image *) NULL,
        &pixel_info);
      background_color=pixel_info.background_color;
      border_color.red=pixel_info.border_color.red >> 8;
      border_color.green=pixel_info.border_color.green >> 8;
      border_color.blue=pixel_info.border_color.blue >> 8;
      matte_color.red=pixel_info.matte_color.red >> 8;
      matte_color.green=pixel_info.matte_color.green >> 8;
      matte_color.blue=pixel_info.matte_color.blue >> 8;
      highlight_color.red=pixel_info.highlight_color.red >> 8;
      highlight_color.green=pixel_info.highlight_color.green >> 8;
      highlight_color.blue=pixel_info.highlight_color.blue >> 8;
      shadow_color.red=pixel_info.shadow_color.red >> 8;
      shadow_color.green=pixel_info.shadow_color.green >> 8;
      shadow_color.blue=pixel_info.shadow_color.blue >> 8;
    }
  /*
    Allocate image structure.
  */
  montage_image=AllocateImage((ImageInfo *) NULL);
  if (montage_image == (Image *) NULL)
    Error("Memory allocation error",(char *) NULL);
  /*
    Initialize Image structure.
  */
  montage_image->columns=
    (tile_info.width+(tile_info.x+border_width)*2)*tiles_per_row;
  montage_image->rows=(tile_info.height+(tile_info.y+border_width)*2+
    (display ? annotate_info.height+4 : 0))*
    (number_tiles/tiles_per_row+((number_tiles % tiles_per_row) != 0));
  if (resource_info->title != (char *) NULL)
    montage_image->rows+=((annotate_info.height+4) << 1)+(tile_info.y << 1);
  montage_image->montage=(char *) malloc(MaxTextLength*sizeof(char));
  count=1;
  for (tile=0; tile < number_tiles; tile++)
    count+=strlen(images[tile]->filename)+1;
  montage_image->directory=(char *) malloc(count*sizeof(char));
  montage_image->packets=montage_image->columns*montage_image->rows;
  montage_image->pixels=(RunlengthPacket *)
    malloc((unsigned int) montage_image->packets*sizeof(RunlengthPacket));
  if ((montage_image->montage == (char *) NULL) ||
      (montage_image->directory == (char *) NULL) ||
      (montage_image->pixels == (RunlengthPacket *) NULL))
    Error("Memory allocation error",(char *) NULL);
  /*
    Set montage geometry.
  */
  x_offset=0;
  y_offset=0;
  if (resource_info->title != (char *) NULL)
    y_offset+=((annotate_info.height+4) << 1)+(tile_info.y << 1);
  *montage_image->directory='\0';
  (void) sprintf(montage_image->montage,"%dx%d%+d%+d",
    (int) (tile_info.width+(tile_info.x+border_width)*2),
    (int) (tile_info.height+(tile_info.y+border_width)*2+
    (display ? annotate_info.height+4 : 0)),x_offset,y_offset);
  /*
    Initialize montage image to background color.
  */
  p=montage_image->pixels;
  for (i=0; i < montage_image->packets; i++)
  {
    p->red=background_color.red >> 8;
    p->green=background_color.green >> 8;
    p->blue=background_color.blue >> 8;
    p->index=0;
    p->length=0;
    p++;
  }
  /*
    Sort images by increasing tile number.
  */
  i=0;
  for (tile=0; tile < number_tiles; tile++)
    i+=images[tile]->scene;
  if (i > 0)
    (void) qsort((void *) images,number_tiles,sizeof(Image *),SceneCompare);
  if ((display != (Display *) NULL) && (resource_info->title != (char *) NULL))
    {
      /*
        Copy title to the composite image.
      */
      (void) strcpy(annotate_info.text,resource_info->title);
      annotate_info.width=
        XTextWidth(font_info,annotate_info.text,strlen(annotate_info.text));
      max_width=montage_image->columns;
      if (((annotate_info.width+tile_info.x*2) << 1) >= max_width)
        {
          /*
            Label is too wide-- shorten.
          */
          q=annotate_info.text+strlen(annotate_info.text);
          do
          {
            *--q='\0';
            if ((int) strlen(annotate_info.text) > 2)
              (void) strcpy(q-2,"...");
            annotate_info.width=XTextWidth(font_info,annotate_info.text,
              strlen(annotate_info.text));
          } while ((2*(annotate_info.width+(tile_info.x << 1))) >= max_width);
        }
      label_info.width=(annotate_info.width << 1)-1;
      label_info.height=(annotate_info.height << 1)-1;
      label_info.x=tile_info.x+(int) (montage_image->columns >> 1)-
        (int) annotate_info.width;
      label_info.y=tile_info.y+4;
      (void) sprintf(annotate_info.geometry,"%ux%u%+d%+d",
        label_info.width,label_info.height,label_info.x,label_info.y);
      (void) XAnnotateImage(display,&pixel_info,&annotate_info,True,
        montage_image);
    }
  /*
    Copy tile images to the composite image.
  */
  x_offset=tile_info.x;
  y_offset=tile_info.y;
  if (display != (Display *) NULL)
    if (resource_info->title != (char *) NULL)
      y_offset+=((annotate_info.height+4) << 1)+(tile_info.y << 1);
  *montage_image->directory='\0';
  for (tile=0; tile < number_tiles; tile++)
  {
    /*
      Copy this tile to the composite image.
    */
    image=images[tile];
    (void) strcat(montage_image->directory,image->filename);
    (void) strcat(montage_image->directory,"\n");
    status=RunlengthDecodeImage(image);
    if (status == False)
      Error("Unable to unpack image",(char *) NULL);
    if (border_width != 0)
      {
        ColorPacket
          black;

        Image
          *bordered_image;

        RectangleInfo
          border_info;

        /*
          Put a border around the image.
        */
        border_info.width=border_width-1;
        border_info.height=border_width-1;
        if (frame)
          {
            border_info.width=(tile_info.width-image->columns+1) >> 1;
            border_info.height=(tile_info.height-image->rows+1) >> 1;
          }
        bordered_image=BorderImage(image,&border_info,&border_color);
        if (bordered_image != (Image *) NULL)
          {
            DestroyImage(image);
            image=bordered_image;
          }
        border_info.width=1;
        border_info.height=1;
        black.red=0;
        black.green=0;
        black.blue=0;
        bordered_image=BorderImage(image,&border_info,&black);
        if (bordered_image != (Image *) NULL)
          {
            DestroyImage(image);
            image=bordered_image;
          }
      }
    /*
      Gravitate image as specified by the tile gravity.
    */
    switch (resource_info->gravity)
    {
      case NorthWestGravity:
      {
        x=0;
        y=0;
        break;
      }
      case NorthGravity:
      {
        x=((tile_info.width+(border_width << 1))-image->columns) >> 1;
        y=0;
        break;
      }
      case NorthEastGravity:
      {
        x=(tile_info.width+(border_width << 1))-image->columns;
        y=0;
        break;
      }
      case WestGravity:
      {
        x=0;
        y=((tile_info.height+(border_width << 1))-image->rows) >> 1;
        break;
      }
      case ForgetGravity:
      case StaticGravity:
      case CenterGravity:
      default:
      {
        x=((tile_info.width+(border_width << 1))-image->columns) >> 1;
        y=((tile_info.height+(border_width << 1))-image->rows) >> 1;
        break;
      }
      case EastGravity:
      {
        x=(tile_info.width+(border_width << 1))-image->columns;
        y=((tile_info.height+(border_width << 1))-image->rows) >> 1;
        break;
      }
      case SouthWestGravity:
      {
        x=0;
        y=(tile_info.height+(border_width << 1))-image->rows;
        break;
      }
      case SouthGravity:
      {
        x=((tile_info.width+(border_width << 1))-image->columns) >> 1;
        y=(tile_info.height+(border_width << 1))-image->rows;
        break;
      }
      case SouthEastGravity:
      {
        x=(tile_info.width+(border_width << 1))-image->columns;
        y=(tile_info.height+(border_width << 1))-image->rows;
        break;
      }
    }
    if (frame)
      {
        Image
          *framed_image;

        RectangleInfo
          frame_info;

        /*
          Put an ornamental border around this tile.
        */
        frame_info.width=tile_info.width+(border_width << 1);
        frame_info.height=tile_info.height+(border_width << 1)+
          (annotate_info.height+4);
        frame_info.x=(x > 0 ? x : border_width);
        frame_info.y=(y > 0 ? y : border_width);
        framed_image=FrameImage(image,&frame_info,bevel_width,&matte_color,
          &highlight_color,&shadow_color);
        if (framed_image != (Image *) NULL)
          {
            DestroyImage(image);
            image=framed_image;
          }
        x=0;
        y=0;
      }
    /*
      Composite background image with tile image.
    */
    CompositeImage(montage_image,compose,image,x_offset+x,y_offset+y);
    if ((display != (Display *) NULL) && (image->label != (char *) NULL))
      {
        /*
          Copy tile label to the composite image.
        */
        (void) strcpy(annotate_info.text,image->label);
        annotate_info.width=
          XTextWidth(font_info,annotate_info.text,strlen(annotate_info.text));
        max_width=tile_info.width+(resource_info->border_width << 1)-8;
        if (annotate_info.width >= max_width)
          {
            /*
              Label is too wide-- shorten.
            */
            q=annotate_info.text+strlen(annotate_info.text);
            do
            {
              *--q='\0';
              if ((int) strlen(annotate_info.text) > 2)
                (void) strcpy(q-2,"...");
              annotate_info.width=XTextWidth(font_info,annotate_info.text,
                strlen(annotate_info.text));
            } while (annotate_info.width >= max_width);
          }
        label_info.width=annotate_info.width;
        label_info.height=annotate_info.height;
        label_info.x=x_offset+((tile_info.width+(border_width << 1)) >> 1)-
          (annotate_info.width >> 1);
        label_info.y=y_offset;
        if (!frame)
          label_info.y+=y+tile_info.y+image->rows+2;
        else
          label_info.y+=tile_info.height+(border_width << 1)-bevel_width-2;
        (void) sprintf(annotate_info.geometry,"%ux%u%+d%+d",
          label_info.width,label_info.height,label_info.x,label_info.y);
        (void) XAnnotateImage(display,&pixel_info,&annotate_info,!frame,
          montage_image);
      }
    DestroyImage(image);
    if (((tile+1) % tiles_per_row) != 0)
      x_offset+=tile_info.width+(tile_info.x+border_width)*2;
    else
      {
        x_offset=tile_info.x;
        y_offset+=tile_info.height+(tile_info.y+border_width)*2+
          (display ? annotate_info.height+4 : 0);
      }
  }
  (void) free((char *) annotate_info.text);
  if (display != (Display *) NULL)
    {
      /*
        Label is too wide-- shorten.
      */
      XFreeFont(display,font_info);
      XFree((char *) visual_info);
      XFree((char *) map_info);
    }
  return(montage_image);
}

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
      "-clip geometry        preferred size and location of the clipped image",
      "-colors value         preferred number of colors in the image",
      "-colorspace type      GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-comment string       annotate image with comment",
      "-compose operator     composite operator",
      "-compress type        RunlengthEncoded or QEncoded",
      "-density geometry     vertical and horizontal density of the image",
      "-display server       query font from this X server",
      "-dither               apply Floyd/Steinberg error diffusion to image",
      "-frame                surround image with an ornamental border",
      "-gamma value          level of gamma correction",
      "-geometry geometry    preferred tile and border sizes",
      "-gravity direction    which direction to gravitate towards",
      "-interlace type       NONE, LINE, or PLANE",
      "-label name           assign a label to an image",
      "-monochrome           transform image to black and white",
      "-page geometry        size and location of the Postscript page",
      "-quality value        JPEG quality setting",
      "-rotate degrees       apply Paeth rotation to the image",
      "-scene value          image scene number",
      "-tiles_per_row value  number of image tiles per row",
      "-treedepth value      depth of the color classification tree",
      "-verbose              print detailed information about the image",
      (char *) NULL
    };
  (void) fprintf(stderr,
    "Usage: %s [-options ...] file [ [-options ...] file ...] file\n",
    client_name);
  (void) fprintf(stderr,"\nWhere options include: \n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nIn addition to those listed above, you can specify these standard X\n");
  (void) fprintf(stderr,
    "resources as command line options:  -background, -bordercolor,\n");
  (void) fprintf(stderr,
    "-borderwidth, -font, -foreground, -mattecolor, or -title\n");
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
    *clip_geometry,
    *comment,
    *density,
    *gamma,
    *label,
    *option,
    *page_geometry,
    *server_name,
    *write_filename;

  Display
    *display;

  Image
    **images,
    *montage_image;

  ImageInfo
    image_info;

  int
    degrees,
    i,
    x;

  time_t
    start_time;

  unsigned int
    compose,
    compression,
    display_image,
    first_scene,
    frame,
    image_number,
    interlace,
    last_scene,
    quality,
    maximum_images,
    scene,
    tiles_per_row,
    verbose;

  XrmDatabase
    resource_database;

  XResourceInfo
    resource_info;

  /*
    Display usage profile if there are no command line arguments.
  */
  client_name=(*argv);
  if (argc < 3)
    Usage();
  /*
    Set defaults.
  */
  clip_geometry=(char *) NULL;
  comment=(char *) NULL;
  compose=ReplaceCompositeOp;
  compression=UndefinedCompression;
  degrees=0;
  density=(char *) NULL;
  display=(Display *) NULL;
  display_image=True;
  first_scene=0;
  frame=False;
  gamma=(char *) NULL;
  interlace=NoneInterlace;
  label=(char *) NULL;
  last_scene=0;
  page_geometry=(char *) NULL;
  quality=85;
  resource_database=(XrmDatabase) NULL;
  resource_info.border_color=(char *) NULL;
  resource_info.border_width=0;
  resource_info.colorspace=RGBColorspace;
  resource_info.dither=False;
  resource_info.gravity=CenterGravity;
  resource_info.image_geometry=(char *) NULL;
  resource_info.monochrome=False;
  resource_info.number_colors=0;
  resource_info.server_name=(char *) NULL;
  resource_info.title=(char *) NULL;
  resource_info.tree_depth=0;
  scene=0;
  server_name=(char *) NULL;
  start_time=0;
  tiles_per_row=0;
  verbose=False;
  maximum_images=MaxTextLength;
  images=(Image **) malloc(maximum_images*sizeof(Image *));
  if (images == (Image **) NULL)
    Error("Unable to montage images","Memory allocation failed");
  /*
    Check for server name specified on the command line.
  */
  for (i=1; i < argc; i++)
  {
    /*
      Check command line for server name.
    */
    option=argv[i];
    if (((int) strlen(option) > 1) && ((*option == '-') || (*option == '+')))
      if (strncmp("display",option+1,3) == 0)
        {
          /*
            User specified server name.
          */
          display_image=(*option == '-');
          if (display_image)
            {
              i++;
              if (i == argc)
                Error("Missing server name on -display",(char *) NULL);
              server_name=argv[i];
            }
          break;
        }
  }
  if (display_image)
    {
      char
        *resource_value;

      XrmDatabase
        server_database;

      /*
        Open X server connection.
      */
      display=XOpenDisplay(server_name);
      if (display == (Display *) NULL)
        Error("Unable to connect to X server",XDisplayName(server_name));
      /*
        Set our forgiving error handler.
      */
      XSetErrorHandler(XError);
      /*
        Initialize resource database.
      */
      XrmInitialize();
      XGetDefault(display,client_name,"dummy");
      resource_database=XrmGetDatabase(display);
      resource_value=XResourceManagerString(display);
      if (resource_value == (char *) NULL)
        resource_value="";
      server_database=XrmGetStringDatabase(resource_value);
      XrmMergeDatabases(server_database,&resource_database);
      /*
        Get user defaults from X resource database.
      */
      XGetResourceInfo(resource_database,client_name,&resource_info);
      resource_info.background_color=XGetResourceInstance(resource_database,
        client_name,"background","#000");
      resource_value=
        XGetResourceClass(resource_database,client_name,"borderWidth","15");
      resource_info.border_width=atoi(resource_value);
      clip_geometry=XGetResourceClass(resource_database,client_name,
        "clipGeometry",(char *) NULL);
      comment=XGetResourceClass(resource_database,client_name,"comment",
        (char *) NULL);
      resource_value=XGetResourceClass(resource_database,client_name,
        "compression","RunlengthEncoded");
      if (Latin1Compare("qencoded",resource_value) == 0)
        compression=QEncodedCompression;
      else
        compression=RunlengthEncodedCompression;
      density=XGetResourceClass(resource_database,client_name,"density",
        (char *) NULL);
      resource_info.foreground_color=XGetResourceInstance(resource_database,
        client_name,"foreground","#600");
      resource_value=
        XGetResourceClass(resource_database,client_name,"frame","True");
      frame=IsTrue(resource_value);
      gamma=
        XGetResourceClass(resource_database,client_name,"gamma",(char *) NULL);
      resource_value=
        XGetResourceClass(resource_database,client_name,"interlace","none");
      interlace=UndefinedInterlace;
      if (Latin1Compare("none",resource_value) == 0)
        interlace=NoneInterlace;
      if (Latin1Compare("line",resource_value) == 0)
        interlace=LineInterlace;
      if (Latin1Compare("plane",resource_value) == 0)
        interlace=PlaneInterlace;
      if (interlace == UndefinedInterlace)
        Warning("Unrecognized interlace type",resource_value);
      label=XGetResourceClass(resource_database,client_name,"label","%f");
      resource_info.image_geometry=XGetResourceInstance(resource_database,
        client_name,"imageGeometry","160x160+2+2");
      resource_info.matte_color=XGetResourceInstance(resource_database,
        client_name,"mattecolor","#ccc");
      page_geometry=XGetResourceClass(resource_database,client_name,
        "pageGeometry",(char *) NULL);
      resource_value=
        XGetResourceClass(resource_database,client_name,"rotate","0");
      degrees=atoi(resource_value);
      resource_value=
        XGetResourceClass(resource_database,client_name,"quality","85");
      quality=atoi(resource_value);
      resource_value=
        XGetResourceClass(resource_database,client_name,"tiles_per_row","0");
      tiles_per_row=atoi(resource_value);
      resource_value=
        XGetResourceClass(resource_database,client_name,"verbose","False");
      verbose=IsTrue(resource_value);
    }
  /*
    Composite image is the last item on the command line.
  */
  write_filename=argv[argc-1];
  if (access(write_filename,0) == 0)
    {
      char
        answer[2];

      (void) fprintf(stderr,"Overwrite %s? ",write_filename);
      (void) gets(answer);
      if (!((*answer == 'y') || (*answer == 'Y')))
        exit(1);
    }
  /*
    Parse command line.
  */
  image_number=0;
  for (i=1; i < (argc-1); i++)
  {
    option=argv[i];
    if (((int) strlen(option) > 1) && ((*option == '-') || (*option == '+')))
      switch (*(option+1))
      {
        case 'b':
        {
          if (strncmp("background",option+1,5) == 0)
            {
              resource_info.background_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing color on -background",(char *) NULL);
                  resource_info.background_color=argv[i];
                }
              break;
            }
          if (strncmp("bordercolor",option+1,7) == 0)
            {
              resource_info.border_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing color on -bordercolor",(char *) NULL);
                  resource_info.border_color=argv[i];
                }
              break;
            }
          if (strncmp("borderwidth",option+1,7) == 0)
            {
              resource_info.border_width=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing width on -borderwidth",(char *) NULL);
                  resource_info.border_width=atoi(argv[i]);
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
              clip_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -clip",(char *) NULL);
                  clip_geometry=argv[i];
                }
              break;
            }
          if (strncmp("colors",option+1,7) == 0)
            {
              resource_info.number_colors=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing colors on -colors",(char *) NULL);
                  resource_info.number_colors=atoi(argv[i]);
                }
              break;
            }
          if (strncmp("colorspace",option+1,7) == 0)
            {
              resource_info.colorspace=RGBColorspace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -colorspace",(char *) NULL);
                  option=argv[i];
                  resource_info.colorspace=UndefinedColorspace;
                  if (Latin1Compare("gray",option) == 0)
                    {
                      resource_info.colorspace=GRAYColorspace;
                      resource_info.number_colors=256;
                      resource_info.tree_depth=8;
                    }
                  if (Latin1Compare("ohta",option) == 0)
                    resource_info.colorspace=OHTAColorspace;
                  if (Latin1Compare("rgb",option) == 0)
                    resource_info.colorspace=RGBColorspace;
                  if (Latin1Compare("xyz",option) == 0)
                    resource_info.colorspace=XYZColorspace;
                  if (Latin1Compare("ycbcr",option) == 0)
                    resource_info.colorspace=YCbCrColorspace;
                  if (Latin1Compare("yiq",option) == 0)
                    resource_info.colorspace=YIQColorspace;
                  if (Latin1Compare("yuv",option) == 0)
                    resource_info.colorspace=YUVColorspace;
                  if (resource_info.colorspace == UndefinedColorspace)
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
          if (strncmp("compress",option+1,5) == 0)
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
              resource_info.server_name=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing server name on -display",(char *) NULL);
                  resource_info.server_name=argv[i];
                }
              break;
            }
          if (strncmp("dither",option+1,3) == 0)
            {
              resource_info.dither=(*option == '-');
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'f':
        {
          if (strncmp("font",option+1,3) == 0)
            {
              resource_info.font=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing font name on -font",(char *) NULL);
                  resource_info.font=argv[i];
                }
              break;
            }
          if (strncmp("foreground",option+1,3) == 0)
            {
              resource_info.foreground_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing foreground on -foreground",(char *) NULL);
                  resource_info.foreground_color=argv[i];
                }
              break;
            }
          if (strncmp("frame",option+1,2) == 0)
            {
              frame=(*option == '-');
              break;
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
              resource_info.image_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -geometry",(char *) NULL);
                  resource_info.image_geometry=argv[i];
                }
              break;
            }
          if (strncmp("gravity",option+1,2) == 0)
            {
              resource_info.gravity=CenterGravity;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -gravity",(char *) NULL);
                  option=argv[i];
                  resource_info.gravity=(-1);
                  if (Latin1Compare("Forget",option) == 0)
                    resource_info.gravity=ForgetGravity;
                  if (Latin1Compare("NorthWest",option) == 0)
                    resource_info.gravity=NorthWestGravity;
                  if (Latin1Compare("North",option) == 0)
                    resource_info.gravity=NorthGravity;
                  if (Latin1Compare("NorthEast",option) == 0)
                    resource_info.gravity=NorthEastGravity;
                  if (Latin1Compare("West",option) == 0)
                    resource_info.gravity=WestGravity;
                  if (Latin1Compare("Center",option) == 0)
                    resource_info.gravity=CenterGravity;
                  if (Latin1Compare("East",option) == 0)
                    resource_info.gravity=EastGravity;
                  if (Latin1Compare("SouthWest",option) == 0)
                    resource_info.gravity=SouthWestGravity;
                  if (Latin1Compare("South",option) == 0)
                    resource_info.gravity=SouthGravity;
                  if (Latin1Compare("SouthEast",option) == 0)
                    resource_info.gravity=SouthEastGravity;
                  if (Latin1Compare("Static",option) == 0)
                    resource_info.gravity=StaticGravity;
                  if (resource_info.gravity == (-1))
                    Error("Invalid gravity type on -gravity",option);
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
          if (strncmp("mattecolor",option+1,6) == 0)
            {
              resource_info.matte_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing color on -matte",(char *) NULL);
                  resource_info.matte_color=argv[i];
                }
              break;
            }
          if (strncmp("monochrome",option+1,2) == 0)
            {
              resource_info.monochrome=(*option == '-');
              if (resource_info.monochrome)
                {
                  resource_info.number_colors=2;
                  resource_info.tree_depth=8;
                  resource_info.colorspace=GRAYColorspace;
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
        case 'r':
        {
          degrees=0;
          if (*option == '-')
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%f",(float *) &x))
                Error("Missing degrees on -rotate",(char *) NULL);
              degrees=atoi(argv[i]);
            }
          break;
        }
        case 's':
        {
          if (strncmp("scene",option+1,3) == 0)
            {
              first_scene=0;
              last_scene=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing scene number on -scene",(char *) NULL);
                  first_scene=atoi(argv[i]);
                  last_scene=first_scene;
                  (void) sscanf(argv[i],"%u-%u",&first_scene,&last_scene);
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 't':
        {
          if (strncmp("tiles_per_row",option+1,3) == 0)
            {
              tiles_per_row=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing value on -tiles_per_row",(char *) NULL);
                  tiles_per_row=atoi(argv[i]);
                }
              break;
            }
          if (strncmp("title",option+1,3) == 0)
            {
              resource_info.title=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing title on -title",(char *) NULL);
                  resource_info.title=argv[i];
                }
              break;
            }
          if (strncmp("treedepth",option+1,2) == 0)
            {
              resource_info.tree_depth=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing depth on -treedepth",(char *) NULL);
                  resource_info.tree_depth=atoi(argv[i]);
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
      for (scene=first_scene; scene <= last_scene ; scene++)
      {
        Image
          *image,
          info_image;

        /*
          Option is a file name: begin by reading image from specified file.
        */
        start_time=time((time_t *) NULL);
        GetImageInfo(argv[i],&image_info);
        if (first_scene != last_scene)
          {
            char
              filename[MaxTextLength];

            /*
              Form filename for multi-part images.
            */
            (void) sprintf(filename,image_info.filename,scene);
            if (strcmp(filename,image_info.filename) == 0)
              (void) sprintf(filename,"%s.%u",image_info.filename,scene);
            (void) strcpy(image_info.filename,filename);
          }
        image_info.server_name=resource_info.server_name;
        image_info.font=resource_info.font;
        image_info.geometry=resource_info.image_geometry;
        image_info.page=page_geometry;
        image_info.density=density;
        image_info.interlace=interlace;
        image_info.monochrome=resource_info.monochrome;
        image_info.quality=quality;
        image_info.verbose=verbose;
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          if (*option == '-')
            break;
          else
            continue;
        do
        {
          if (image->scene == 0)
            image->scene=image_number;
          info_image=(*image);
          LabelImage(image,label);
          /*
            Tile size maintains the aspect ratio of the image.
          */
          if (resource_info.image_geometry == (char *) NULL)
            TransformImage(&image,clip_geometry,"256x256");
          else
            TransformImage(&image,clip_geometry,resource_info.image_geometry);
          if ((degrees % 360) != 0)
            {
              Colormap
                colormap;

              ColorPacket
                border_color;

              Image
                *rotated_image;

              XColor
                color;

              /*
                Rotate image.
              */
              colormap=XDefaultColormap(display,XDefaultScreen(display));
              (void) XParseColor(display,colormap,resource_info.border_color,
                &color);
              border_color.red=color.red >> 8;
              border_color.green=color.green >> 8;
              border_color.blue=color.blue >> 8;
              rotated_image=
                RotateImage(image,(double) degrees,&border_color,False);
              if (rotated_image != (Image *) NULL)
                {
                  DestroyImage(image);
                  image=rotated_image;
                }
            }
          if (gamma != (char *) NULL)
            GammaImage(image,gamma);
          if (verbose)
            {
              /*
                Display detailed info about the image.
              */
              (void) fprintf(stderr,"[%u] %s",
                image->scene == 0 ? image_number : image->scene,
                image->filename);
              (void) fprintf(stderr," %ux%u",info_image.columns,
                info_image.rows);
              if ((info_image.columns != image->columns) ||
                  (info_image.rows != image->rows))
                (void) fprintf(stderr,"=>%ux%u",image->columns,image->rows);
              if (image->class == DirectClass)
                (void) fprintf(stderr," DirectClass");
              else
                (void) fprintf(stderr," PseudoClass %uc",image->colors);
              (void) fprintf(stderr," %s\n",image->magick);
            }
          /*
            Pack image data to conserve memory (memory <=> speed).
          */
          image->matte=False;
          (void) RunlengthEncodeImage(image);
          (void) free((char *) image->pixels);
          image->pixels=(RunlengthPacket *) NULL;
          if (image_number == maximum_images)
            {
              /*
                Increase size of images array.
              */
              maximum_images<<=1;
              images=(Image **)
                realloc((char *) images,maximum_images*sizeof(Image *));
              if (images == (Image **) NULL)
                Error("Unable to montage images","Memory allocation failed");
            }
          images[image_number++]=image;
          image=image->next;
        } while (image != (Image *) NULL);
      }
    }
  if (image_number == 0)
    Error("Missing an image file name",(char *) NULL);
  /*
    Create composite image.
  */
  montage_image=MontageImage(display,&resource_info,images,image_number,
    tiles_per_row,frame,compose);
  if (resource_info.number_colors != 0)
    {
      QuantizeImage(montage_image,resource_info.number_colors,
        resource_info.tree_depth,resource_info.dither,resource_info.colorspace,
        True);
      SyncImage(montage_image);
    }
  if (compression != UndefinedCompression)
    montage_image->compression=compression;
  (void) strcpy(montage_image->filename,write_filename);
  if (comment != (char *) NULL)
    CommentImage(montage_image,comment);
  (void) WriteImage(&image_info,montage_image);
  if (verbose)
    {
      /*
        Display detailed info about the image.
      */
      if (montage_image->class == DirectClass)
        montage_image->colors=NumberColors(montage_image,False);
      (void) fprintf(stderr,"[%u] %s %s=>%ux%u",montage_image->scene,
        montage_image->filename,montage_image->montage,montage_image->columns,
        montage_image->rows);
      if (montage_image->class == DirectClass)
        (void) fprintf(stderr," DirectClass ");
      else
        (void) fprintf(stderr," PseudoClass ");
      (void) fprintf(stderr,"%uc %s %lds\n",montage_image->colors,
        montage_image->magick,time((time_t *) NULL)-start_time+1);
    }
  DestroyImage(montage_image);
  (void) free((char *) images);
  if (display != (Display *) NULL)
    XCloseDisplay(display);
  return(False);
}
