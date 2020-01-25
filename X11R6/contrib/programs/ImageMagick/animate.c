/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%              AAA   N   N  IIIII  M   M   AAA   TTTTT  EEEEE                 %
%             A   A  NN  N    I    MM MM  A   A    T    E                     %
%             AAAAA  N N N    I    M M M  AAAAA    T    EEE                   %
%             A   A  N  NN    I    M   M  A   A    T    E                     %
%             A   A  N   N  IIIII  M   M  A   A    T    EEEEE                 %
%                                                                             %
%                                                                             %
%          Animate Machine Independent File Format Image via X11.             %
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
%  Animate displays a sequence of images in the MIFF format on any
%  workstation display running an X server.  Animate first determines the
%  hardware capabilities of the workstation.  If the number of unique
%  colors in an image is less than or equal to the number the workstation
%  can support, the image is displayed in an X window.  Otherwise the
%  number of colors in the image is first reduced to match the color
%  resolution of the workstation before it is displayed.
%
%  This means that a continuous-tone 24 bits-per-pixel image can display on a
%  8 bit pseudo-color device or monochrome device.  In most instances the
%  reduced color image closely resembles the original.  Alternatively, a
%  monochrome or pseudo-color image can display on a continuous-tone 24
%  bits-per-pixel device.
%
%  The Animate program command syntax is:
%
%  Usage: animate [options ...] file [ [options ...] file ...]
%
%  Where options include:
%    -backdrop            display image centered on a backdrop
%    -clip geometry       preferred size and location of the clipped image
%    -colormap type       Shared or Private
%    -colors value        preferred number of colors in the image
%    -colorspace type     GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -delay milliseconds  display the next image after pausing
%    -density geometry    vertical and horizontal density of the image
%    -display server      display image to this X server
%    -dither              apply Floyd/Steinberg error diffusion to image
%    -gamma value         level of gamma correction
%    -geometry geometry   preferred size and location of the image window
%    -interlace type      NONE, LINE, or PLANE
%    -map type            display image using this Standard Colormap
%    -monochrome          transform image to black and white
%    -scene value         image scene number
%    -treedepth value     depth of the color classification tree
%    -verbose             print detailed information about the image
%    -visual type         display image using this visual type
%    -window id           display image to background of this window
%
%  In addition to those listed above, you can specify these standard X
%  resources as command line options:  -background, -bordercolor,
%  -borderwidth, -font, -foreground, -iconGeometry, -iconic, -name,
%  -mattecolor, or -title.
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
%  Buttons:
%    1    press and drag to select a command from a pop-up menu
%
%  Keyboard accelerators:
%    p    press to animate the sequence of images
%    s    press to display the next image in the sequence
%    .    press to continually display the sequence of images
%    a    press to automatically reverse the sequence of images
%    <    press to slow the display of the images
%    >    press to speed-up the display of the images
%    f    press to animate in the forward direction
%    r    press to animate in the reverse direction
%    i    press to display information about the image
%    q    press to discard all images and exit program
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "image.h"
#include "utility.h"
#include "compress.h"
#include "X.h"
#include "widget.h"

/*
  State declarations.
*/
#define AutoReverseAnimationState 0x0001
#define DefaultState  0x0000
#define ExitState  0x0002
#define ForwardAnimationState 0x0004
#define HighlightState  0x0008
#define PlayAnimationState 0x0010
#define RepeatAnimationState 0x0020
#define StepAnimationState 0x0040

/*
  Forward declarations.
*/
static void XMagickCommand _Declare((Display *,XResourceInfo *,XWindows *,
  KeySym,Image **,unsigned int *));

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
%      Usage(terminate)
%
%  A description of each parameter follows:
%
%    o terminate: The program will exit if the value is not zero.
%
%
*/
static void Usage(terminate)
unsigned int
  terminate;
{
  char
    **p;

  static char
    *buttons[]=
    {
      "1    press and drag to select a command from a pop-up menu",
      (char *) NULL
    },
    *keys[]=
    {
      "0-9  press to change the level of delay",
      "p    press to animate the sequence of images",
      "s    press to display the next image in the sequence",
      ".    press to continually display the sequence of images",
      "a    press to automatically reverse the sequence of images",
      "<    press to slow the display of the images",
      ">    press to speed-up the display of images",
      "f    press to animate in the forward direction",
      "r    press to animate in the reverse direction",
      "i    press to display information about the image",
      "q    press to discard all images and exit program",
      (char *) NULL
    },
    *options[]=
    {
      "-backdrop            display image centered on a backdrop",
      "-clip geometry       preferred size and location of the clipped image",
      "-colormap type       Shared or Private",
      "-colors value        preferred number of colors in the image",
      "-colorspace type     GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-delay milliseconds  display the next image after pausing",
      "-density geometry    vertical and horizontal density of the image",
      "-display server      display image to this X server",
      "-dither              apply Floyd/Steinberg error diffusion to image",
      "-gamma value         level of gamma correction",
      "-geometry geometry   preferred size and location of the image window",
      "-interlace type      NONE, LINE, or PLANE",
      "-map type            display image using this Standard Colormap",
      "-monochrome          transform image to black and white",
      "-scene value         image scene number",
      "-treedepth value     depth of the color classification tree",
      "-verbose             print detailed information about the image",
      "-visual type         display image using this visual type",
      "-window id          display image to background of this window",
      (char *) NULL
    };
  (void) fprintf(stderr,
    "Usage: %s [-options ...] file [ [-options ...] file ...]\n",client_name);
  (void) fprintf(stderr,"\nWhere options include: \n");
  for (p=options; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,
    "\nIn addition to those listed above, you can specify these standard X\n");
  (void) fprintf(stderr,
    "resources as command line options:  -background, -bordercolor,\n");
  (void) fprintf(stderr,
    "-borderwidth, -font, -foreground, -iconGeometry, -iconic, -name,\n");
  (void) fprintf(stderr,"-mattecolor, or -title.\n");
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
  (void) fprintf(stderr,"\nButtons: \n");
  for (p=buttons; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  (void) fprintf(stderr,"\nKeyboard accelerators: \n");
  for (p=keys; *p != (char *) NULL; p++)
    (void) fprintf(stderr,"  %s\n",*p);
  if (terminate)
    exit(1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X A n i m a t e B a c k g r o u n d I m a g e                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XAnimateBackgroundImage animates an image sequence in the
%  background of a window.
%
%  The format of the XAnimateBackgroundImage routine is:
%
%      XAnimateBackgroundImage(display,resource_info,image)
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

static void XAnimateBackgroundImage(display,resource_info,images,number_scenes)
Display
  *display;

XResourceInfo
  *resource_info;

Image
  **images;

unsigned int
  number_scenes;
{
  char
    visual_type[MaxTextLength];

  int
    i,
    scene;

  unsigned int
    height,
    status,
    width;

  Window
    root_window;

  XEvent
    event;

  XGCValues
    context_values;

  XPixelInfo
    pixel_info,
    scene_info;

  XStandardColormap
    *map_info;

  XVisualInfo
    *visual_info;

  XWindowAttributes
    window_attributes;

  XWindowInfo
    window_info;

  /*
    Determine target window.
  */
  window_info.id=(Window) NULL;
  window_info.ximage=(XImage *) NULL;
  window_info.pixmap=(Pixmap) NULL;
  root_window=XRootWindow(display,XDefaultScreen(display));
  if (Latin1Compare(resource_info->window_id,"root") == 0)
    window_info.id=root_window;
  else
    {
      if (isdigit(*resource_info->window_id))
        window_info.id=XWindowByID(display,root_window,
          (Window) strtol((char *) resource_info->window_id,(char **) NULL,0));
      if (window_info.id == (Window) NULL)
        window_info.id=
          XWindowByName(display,root_window,resource_info->window_id);
      if (window_info.id == (Window) NULL)
        Error("No window with specified id exists",resource_info->window_id);
    }
  /*
    Determine window visual id.
  */
  window_attributes.width=XDisplayWidth(display,XDefaultScreen(display));
  window_attributes.height=XDisplayHeight(display,XDefaultScreen(display));
  (void) strcpy(visual_type,"default");
  status=XGetWindowAttributes(display,window_info.id,&window_attributes);
  if (status != False)
    (void) sprintf(visual_type,"0x%lx",
      XVisualIDFromVisual(window_attributes.visual));
  resource_info->map_type=(char *) NULL;
  resource_info->visual_type=visual_type;
  /*
    Allocate standard colormap.
  */
  map_info=XAllocStandardColormap();
  if (map_info == (XStandardColormap *) NULL)
    Error("Unable to create standard colormap","Memory allocation failed");
  map_info->colormap=(Colormap) NULL;
  pixel_info.pixels=(unsigned long *) NULL;
  /*
    Initialize visual info.
  */
  visual_info=XBestVisualInfo(display,map_info,resource_info);
  if (visual_info == (XVisualInfo *) NULL)
    Error("Unable to get visual",resource_info->visual_type);
  /*
    Sort images by increasing scene number.
  */
  i=0;
  for (scene=0; scene < number_scenes; scene++)
    i+=images[scene]->scene;
  if (i > 0)
    (void) qsort((void *) images,number_scenes,sizeof(Image *),SceneCompare);
  if (resource_info->map_type == (char *) NULL)
    if ((visual_info->class != TrueColor) &&
        (visual_info->class != DirectColor))
      {
        unsigned int
          identical_colormap;

        /*
          Determine if the sequence of images has the identical colormap.
        */
        identical_colormap=True;
        for (scene=0; scene < number_scenes; scene++)
        {
          if ((images[scene]->class == DirectClass) ||
              (images[scene]->colors > visual_info->colormap_size))
            {
              /*
                Image has more colors than the visual supports.
              */
              status=RunlengthDecodeImage(images[scene]);
              if (status == False)
                Error("Unable to unpack image",(char *) NULL);
              QuantizeImage(images[scene],(unsigned int)
                visual_info->colormap_size,resource_info->tree_depth,
                resource_info->dither,resource_info->colorspace,False);
            }
          if (images[scene]->signature == (char *) NULL)
            ColormapSignature(images[scene]);
          status=strcmp(images[scene]->signature,images[0]->signature);
          if (status != 0)
            identical_colormap=False;
        }
        if (!identical_colormap)
          {
            /*
              Create a single colormap for the sequence of images.
            */
            for (scene=0; scene < number_scenes; scene++)
              if (images[scene]->packed_pixels != (unsigned char *) NULL)
                {
                  status=RunlengthDecodeImage(images[scene]);
                  if (status == False)
                    Error("Unable to unpack image",(char *) NULL);
                }
            QuantizeImages(images,number_scenes,(Image *) NULL,(unsigned int)
              visual_info->colormap_size,resource_info->tree_depth,
              resource_info->dither,resource_info->colorspace,False);
          }
      }
  /*
    Initialize Standard Colormap.
  */
  if (images[0]->packed_pixels != (unsigned char *) NULL)
    {
      status=RunlengthDecodeImage(images[0]);
      if (status == False)
        Error("Unable to unpack image",(char *) NULL);
    }
  XMakeStandardColormap(display,visual_info,resource_info,images[0],map_info,
    &pixel_info);
  /*
    Graphic context superclass.
  */
  context_values.background=pixel_info.background_color.pixel;
  context_values.foreground=pixel_info.foreground_color.pixel;
  pixel_info.annotate_context=XCreateGC(display,window_info.id,GCBackground |
    GCForeground,&context_values);
  if (pixel_info.annotate_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  context_values.background=pixel_info.foreground_color.pixel;
  context_values.foreground=pixel_info.background_color.pixel;
  context_values.plane_mask=
    context_values.background ^ context_values.foreground;
  pixel_info.highlight_context=XCreateGC(display,window_info.id,GCBackground |
    GCForeground | GCPlaneMask,&context_values);
  if (pixel_info.highlight_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  /*
    Initialize image window attributes.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    resource_info,&window_info);
  /*
    Create the X image.
  */
  window_info.width=images[0]->columns;
  if (window_info.width >= window_attributes.width)
    window_info.width=window_attributes.width;
  window_info.height=images[0]->rows;
  if (window_info.height >= window_attributes.height)
    window_info.height=window_attributes.height;
  status=XMakeImage(display,resource_info,&window_info,images[0],
    images[0]->columns,images[0]->rows);
  if (status == False)
    Error("Unable to create X image",(char *) NULL);
  if (resource_info->debug)
    {
      (void) fprintf(stderr,"Image: [%u] %s %ux%u ",images[0]->scene,
        images[0]->filename,images[0]->columns,images[0]->rows);
      if (images[0]->colors != 0)
        (void) fprintf(stderr,"%uc ",images[0]->colors);
      (void) fprintf(stderr,"%s\n",images[0]->magick);
    }
  /*
    Adjust image dimensions as specified by backdrop or geometry options.
  */
  width=window_info.width;
  height=window_info.height;
  if (resource_info->backdrop)
    {
      /*
        Center image on window.
      */
      window_info.x=(window_attributes.width >> 1)-(images[0]->columns >> 1);
      window_info.y=(window_attributes.height >> 1)-(images[0]->rows >> 1);
      width=window_attributes.width;
      height=window_attributes.height;
    }
  if (resource_info->image_geometry != (char *) NULL)
    {
      char
        default_geometry[MaxTextLength];

      int
        flags,
        gravity;

      XSizeHints
        *size_hints;

      /*
        User specified geometry.
      */
      size_hints=XAllocSizeHints();
      if (size_hints == (XSizeHints *) NULL)
        Error("Unable to display on window","Memory allocation failed");
      size_hints->flags=(long) NULL;
      (void) sprintf(default_geometry,"%ux%u",width,height);
      flags=XWMGeometry(display,visual_info->screen,
        resource_info->image_geometry,default_geometry,
        window_info.border_width,size_hints,&window_info.x,&window_info.y,
        (int *) &width,(int *) &height,&gravity);
      if (flags & (XValue | YValue))
        {
          width=window_attributes.width;
          height=window_attributes.height;
        }
      XFree((void *) size_hints);
    }
  /*
    Create the X pixmap.
  */
  window_info.pixmap=
    XCreatePixmap(display,window_info.id,width,height,window_info.depth);
  if (window_info.pixmap == (Pixmap) NULL)
    Error("Unable to create X pixmap",(char *) NULL);
  if ((width > window_info.width) || (height > window_info.height))
    XFillRectangle(display,window_info.pixmap,window_info.highlight_context,
      0,0,width,height);
  XPutImage(display,window_info.pixmap,window_info.annotate_context,
    window_info.ximage,0,0,window_info.x,window_info.y,window_info.width,
    window_info.height);
  /*
    Initialize image pixmaps structure.
  */
  window_info.pixmaps=(Pixmap *) malloc(number_scenes*sizeof(Pixmap));
  if (window_info.pixmaps == (Pixmap *) NULL)
    Error("Unable to animate images","Memory allocation failed");
  window_info.pixmaps[0]=window_info.pixmap;
  scene_info.pixels=(unsigned long *) NULL;
  for (scene=1; scene < number_scenes; scene++)
  {
    /*
      Create X image.
    */
    window_info.pixmap=(Pixmap) NULL;
    if (images[scene]->packed_pixels != (unsigned char *) NULL)
      {
        status=RunlengthDecodeImage(images[scene]);
        if (status == False)
          Error("Unable to unpack image",(char *) NULL);
      }
    if ((resource_info->map_type != (char *) NULL) ||
        (visual_info->class == TrueColor) ||
        (visual_info->class == DirectColor))
      if (images[scene]->class == PseudoClass)
        {
          /*
            Get pixel info for this scene.
          */
          XGetPixelInfo(display,visual_info,map_info,resource_info,
            images[scene],&scene_info);
          window_info.pixel_info=(&scene_info);
        }
    status=XMakeImage(display,resource_info,&window_info,images[scene],
      images[scene]->columns,images[scene]->rows);
    /*
      Create the X pixmap.
    */
    window_info.pixmap=
      XCreatePixmap(display,window_info.id,width,height,window_info.depth);
    if (window_info.pixmap == (Pixmap) NULL)
      Error("Unable to create X pixmap",(char *) NULL);
    if ((width > window_info.width) || (height > window_info.height))
      XFillRectangle(display,window_info.pixmap,window_info.highlight_context,
        0,0,width,height);
    XPutImage(display,window_info.pixmap,window_info.annotate_context,
      window_info.ximage,0,0,window_info.x,window_info.y,window_info.width,
      window_info.height);
    if (resource_info->debug)
      {
        (void) fprintf(stderr,"Image: [%u] %s %ux%u ",images[scene]->scene,
          images[scene]->filename,images[scene]->columns,images[scene]->rows);
        if (images[scene]->colors != 0)
          (void) fprintf(stderr,"%uc ",images[scene]->colors);
        (void) fprintf(stderr,"%s\n",images[scene]->magick);
      }
    window_info.pixmaps[scene]=window_info.pixmap;
    /*
      Free image pixels.
    */
    if (scene_info.pixels != (unsigned long *) NULL)
      (void) free((char *) scene_info.pixels);
    (void) free((char *) images[scene]->pixels);
    images[scene]->pixels=(RunlengthPacket *) NULL;
  }
  /*
    Display pixmap on the window.
  */
  XSelectInput(display,window_info.id,StructureNotifyMask);
  event.type=Expose;
  do
  {
    for (scene=0; scene < number_scenes; scene++)
    {
      if (XEventsQueued(display,QueuedAfterFlush) > 0)
        {
          XNextEvent(display,&event);
          if (event.type == DestroyNotify)
            break;
        }
      window_info.pixmap=window_info.pixmaps[scene];
      XSetWindowBackgroundPixmap(display,window_info.id,window_info.pixmap);
      XClearWindow(display,window_info.id);
      XSync(display,False);
      if (resource_info->delay != 0)
        XDelay(display,(unsigned long) resource_info->delay);
    }
  } while (event.type != DestroyNotify);
  /*
    Free resources.
  */
  for (scene=0; scene < number_scenes; scene++)
  {
    XFreePixmap(display,window_info.pixmaps[scene]);
    DestroyImage(images[scene]);
  }
  (void) free((char *) window_info.pixmaps);
  XDestroyImage(window_info.ximage);
  XFreeStandardColormap(display,visual_info,map_info,&pixel_info);
  XFreeGC(display,window_info.annotate_context);
  XFreeGC(display,window_info.highlight_context);
  XFreeCursor(display,window_info.cursor);
  XFreeCursor(display,window_info.busy_cursor);
  if (pixel_info.pixels != (unsigned long *) NULL)
    (void) free((char *) pixel_info.pixels);
  XFree((void *) map_info);
  XFree((void *) visual_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X A n i m a t e I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XAnimateImage displays an image via X11.
%
%  The format of the XAnimateImage routine is:
%
%      XAnimateImage(display,resource_info,argv,argc,image,number_scenes)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o argv: Specifies the application's argument list.
%
%    o argc: Specifies the number of arguments.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
%    o number_scenes: Specifies the number of scenes to animate.
%
%
*/
static void XAnimateImage(display,resource_info,argv,argc,images,number_scenes)
Display
  *display;

XResourceInfo
  *resource_info;

char
  **argv;

int
  argc;

Image
  **images;

unsigned int
  number_scenes;
{
#define MaxWindows  9

  static unsigned char
    HighlightBitmap[] = {0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55},
    ShadowBitmap[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  Image
    *image;

  int
    i,
    scene;

  unsigned int
    number_windows,
    state,
    status;

  Window
    root_window;

  XClassHint
    *class_hint;

  XEvent
    event;

  XFontStruct
    *font_info;

  XGCValues
    context_values;

  XPixelInfo
    icon_pixel,
    pixel_info,
    scene_info;

  XResourceInfo
    icon_resources;

  XStandardColormap
    *icon_map,
    *map_info;

  XVisualInfo
    *icon_visual,
    *visual_info;

  XWindowInfo
    *magick_windows[MaxWindows];

  XWindows
    *windows;

  XWMHints
    *manager_hints;

  /*
    Allocate standard colormap.
  */
  if (resource_info->debug)
    XSynchronize(display,True);
  map_info=XAllocStandardColormap();
  icon_map=XAllocStandardColormap();
  if ((map_info == (XStandardColormap *) NULL) ||
      (icon_map == (XStandardColormap *) NULL))
    Error("Unable to create standard colormap","Memory allocation failed");
  map_info->colormap=(Colormap) NULL;
  icon_map->colormap=(Colormap) NULL;
  pixel_info.pixels=(unsigned long *) NULL;
  icon_pixel.pixels=(unsigned long *) NULL;
  /*
    Get the best visual this server supports.
  */
  icon_resources=(*resource_info);
  icon_resources.map_type=(char *) NULL;
  icon_resources.visual_type="default";
  icon_resources.colormap=SharedColormap;
  visual_info=XBestVisualInfo(display,map_info,resource_info);
  icon_visual=XBestVisualInfo(display,icon_map,&icon_resources);
  if ((visual_info == (XVisualInfo *) NULL) ||
      (icon_visual == (XVisualInfo *) NULL))
    Error("Unable to get visual",resource_info->visual_type);
  if (resource_info->debug)
    {
      (void) fprintf(stderr,"Visual:\n");
      (void) fprintf(stderr,"  visual id: 0x%lx\n",visual_info->visualid);
      (void) fprintf(stderr,"  class: %s\n",
         XVisualClassName(visual_info->class));
      (void) fprintf(stderr,"  depth: %d planes\n",visual_info->depth);
      (void) fprintf(stderr,"  size of colormap: %d entries\n",
        visual_info->colormap_size);
      (void) fprintf(stderr,"  red, green, blue masks: 0x%lx 0x%lx 0x%lx\n",
        visual_info->red_mask,visual_info->green_mask,visual_info->blue_mask);
      (void) fprintf(stderr,"  significant bits in color: %d bits\n",
        visual_info->bits_per_rgb);
    }
  /*
    Initialize atoms.
  */
  windows=(XWindows *) malloc(sizeof(XWindows));
  if (windows == (XWindows *) NULL)
    Error("Unable to create X windows","Memory allocation failed");
  windows->wm_protocols=XInternAtom(display,"WM_PROTOCOLS",False);
  windows->wm_delete_window=XInternAtom(display,"WM_DELETE_WINDOW",False);
  windows->wm_take_focus=XInternAtom(display,"WM_TAKE_FOCUS",False);
  /*
    Allocate class and manager hints.
  */
  class_hint=XAllocClassHint();
  manager_hints=XAllocWMHints();
  if ((class_hint == (XClassHint *) NULL) ||
      (manager_hints == (XWMHints *) NULL))
    Error("Unable to allocate X hints",(char *) NULL);
  /*
    Initialize window id's.
  */
  root_window=XRootWindow(display,visual_info->screen);
  number_windows=0;
  magick_windows[number_windows++]=(&windows->backdrop);
  magick_windows[number_windows++]=(&windows->icon);
  magick_windows[number_windows++]=(&windows->image);
  magick_windows[number_windows++]=(&windows->info);
  magick_windows[number_windows++]=(&windows->popup);
  for (i=0; i < number_windows; i++)
    magick_windows[i]->id=(Window) NULL;
  /*
    Sort images by increasing scene number.
  */
  i=0;
  for (scene=0; scene < number_scenes; scene++)
    i+=images[scene]->scene;
  if (i > 0)
    (void) qsort((void *) images,number_scenes,sizeof(Image *),SceneCompare);
  if (resource_info->map_type == (char *) NULL)
    if ((visual_info->class != TrueColor) &&
        (visual_info->class != DirectColor))
      {
        unsigned int
          identical_colormap;

        /*
          Determine if the sequence of images has the identical colormap.
        */
        identical_colormap=True;
        for (scene=0; scene < number_scenes; scene++)
        {
          if ((images[scene]->class == DirectClass) ||
              (images[scene]->colors > visual_info->colormap_size))
            {
              /*
                Image has more colors than the visual supports.
              */
              status=RunlengthDecodeImage(images[scene]);
              if (status == False)
                Error("Unable to unpack image",(char *) NULL);
              QuantizeImage(images[scene],(unsigned int)
                visual_info->colormap_size,resource_info->tree_depth,
                resource_info->dither,resource_info->colorspace,False);
            }
          if (images[scene]->signature == (char *) NULL)
            ColormapSignature(images[scene]);
          status=strcmp(images[scene]->signature,images[0]->signature);
          if (status != 0)
            identical_colormap=False;
        }
        if (!identical_colormap)
          {
            /*
              Create a single colormap for the sequence of images.
            */
            for (scene=0; scene < number_scenes; scene++)
              if (images[scene]->packed_pixels != (unsigned char *) NULL)
                {
                  status=RunlengthDecodeImage(images[scene]);
                  if (status == False)
                    Error("Unable to unpack image",(char *) NULL);
                }
            QuantizeImages(images,number_scenes,(Image *) NULL,(unsigned int)
              visual_info->colormap_size,resource_info->tree_depth,
              resource_info->dither,resource_info->colorspace,False);
          }
      }
  /*
    Initialize Standard Colormap.
  */
  if (images[0]->packed_pixels != (unsigned char *) NULL)
    {
      status=RunlengthDecodeImage(images[0]);
      if (status == False)
        Error("Unable to unpack image",(char *) NULL);
    }
  XMakeStandardColormap(display,visual_info,resource_info,images[0],map_info,
    &pixel_info);
  /*
    Initialize font info.
  */
  font_info=XBestFont(display,resource_info);
  if (font_info == (XFontStruct *) NULL)
    Error("Unable to load font",resource_info->font);
  /*
    Initialize graphic context.
  */
  windows->context.id=(Window) NULL;
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->context);
  class_hint->res_name="superclass";
  class_hint->res_class="ImageMagick";
  manager_hints->flags=InputHint | StateHint;
  manager_hints->input=False;
  manager_hints->initial_state=WithdrawnState;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->context);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (context)\n",windows->context.id);
  context_values.background=pixel_info.background_color.pixel;
  context_values.font=font_info->fid;
  context_values.foreground=pixel_info.foreground_color.pixel;
  context_values.graphics_exposures=False;
  pixel_info.annotate_context=XCreateGC(display,windows->context.id,
    GCBackground | GCFont | GCForeground | GCGraphicsExposures,&context_values);
  if (pixel_info.annotate_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  context_values.background=pixel_info.depth_color.pixel;
  pixel_info.widget_context=XCreateGC(display,windows->context.id,
    GCBackground | GCFont | GCForeground | GCGraphicsExposures,&context_values);
  if (pixel_info.widget_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  context_values.background=pixel_info.foreground_color.pixel;
  context_values.foreground=pixel_info.background_color.pixel;
  pixel_info.highlight_context=XCreateGC(display,windows->context.id,
    GCBackground | GCFont | GCForeground | GCGraphicsExposures,&context_values);
  if (pixel_info.highlight_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  XDestroyWindow(display,windows->context.id);
  /*
    Initialize icon window.
  */
  XGetWindowInfo(display,icon_visual,icon_map,&icon_pixel,(XFontStruct *) NULL,
    &icon_resources,&windows->icon);
  XBestIconSize(display,&windows->icon,images[0]);
  windows->icon.attributes.colormap=
    XDefaultColormap(display,icon_visual->screen);
  windows->icon.attributes.event_mask=StructureNotifyMask;
  class_hint->res_name="icon";
  manager_hints->flags=InputHint | StateHint;
  manager_hints->input=False;
  manager_hints->initial_state=IconicState;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->icon);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (icon)\n",windows->icon.id);
  /*
    Initialize graphic context for icon window.
  */
  context_values.background=icon_pixel.background_color.pixel;
  context_values.foreground=icon_pixel.foreground_color.pixel;
  icon_pixel.annotate_context=XCreateGC(display,windows->icon.id,
    GCBackground | GCForeground,&context_values);
  if (icon_pixel.annotate_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  windows->icon.annotate_context=icon_pixel.annotate_context;
  /*
    Initialize image window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->image);
  windows->image.name=(char *) malloc(MaxTextLength*sizeof(char));
  windows->image.icon_name=(char *) malloc(MaxTextLength*sizeof(char));
  if ((windows->image.name == NULL) || (windows->image.icon_name == NULL))
    Error("Unable to create image window","Memory allocation failed");
  if (resource_info->title != (char *) NULL)
    {
      /*
        User specified window name.
      */
      (void) strcpy(windows->image.name,resource_info->title);
      (void) strcpy(windows->image.icon_name,resource_info->title);
    }
  else
    {
      register char
        *p;

      /*
        Window name is the base of the filename.
      */
      p=images[0]->filename+strlen(images[0]->filename)-1;
      while ((p > images[0]->filename) && (*(p-1) != '/'))
        p--;
      (void) strcpy(windows->image.name,"ImageMagick: ");
      (void) strcat(windows->image.name,p);
      (void) strcpy(windows->image.icon_name,p);
    }
  windows->image.geometry=resource_info->image_geometry;
  windows->image.width=images[0]->columns;
  if (windows->image.width >= XDisplayWidth(display,visual_info->screen))
    windows->image.width=XDisplayWidth(display,visual_info->screen);
  windows->image.height=images[0]->rows;
  if (windows->image.height >= XDisplayHeight(display,visual_info->screen))
    windows->image.height=XDisplayHeight(display,visual_info->screen);
  windows->image.attributes.event_mask=ButtonMotionMask | ButtonPressMask |
    ButtonReleaseMask | EnterWindowMask | ExposureMask | KeyPressMask |
    LeaveWindowMask | OwnerGrabButtonMask | StructureNotifyMask;
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->backdrop);
  if (resource_info->backdrop)
    {
      /*
        Initialize backdrop window.
      */
      windows->backdrop.name="ImageMagick Background";
      windows->backdrop.flags=USSize | USPosition;
      windows->backdrop.width=XDisplayWidth(display,visual_info->screen);
      windows->backdrop.height=XDisplayHeight(display,visual_info->screen);
      windows->backdrop.border_width=0;
      windows->backdrop.immutable=True;
      windows->backdrop.attributes.do_not_propagate_mask=
        ButtonPressMask | ButtonReleaseMask;
      windows->backdrop.attributes.override_redirect=True;
      class_hint->res_name="backdrop";
      manager_hints->flags=IconWindowHint | InputHint | StateHint;
      manager_hints->icon_window=windows->icon.id;
      manager_hints->input=True;
      manager_hints->initial_state=
        resource_info->iconic ? IconicState : NormalState;
      XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
        &windows->backdrop);
      if (resource_info->debug)
        (void) fprintf(stderr,"Window id: 0x%lx (backdrop)\n",
          windows->backdrop.id);
      XSetTransientForHint(display,windows->backdrop.id,windows->backdrop.id);
      XMapWindow(display,windows->backdrop.id);
      /*
        Position image in the center the backdrop.
      */
      windows->image.flags|=USPosition;
      windows->image.x=(XDisplayWidth(display,visual_info->screen) >> 1)-
        (windows->image.width >> 1);
      windows->image.y=(XDisplayHeight(display,visual_info->screen) >> 1)-
        (windows->image.height >> 1);
    }
  if (resource_info->name == (char *) NULL)
    class_hint->res_name=client_name;
  else
    class_hint->res_name=resource_info->name;
  manager_hints->flags=IconWindowHint | InputHint | StateHint;
  manager_hints->icon_window=windows->icon.id;
  manager_hints->input=True;
  manager_hints->initial_state=
    resource_info->iconic ? IconicState : NormalState;
  XMakeWindow(display,(resource_info->backdrop ? windows->backdrop.id :
    root_window),argv,argc,class_hint,manager_hints,&windows->image);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (image)\n",windows->image.id);
  /*
    Initialize image X image structure.
  */
  windows->image.x=0;
  windows->image.y=0;
  status=XMakeImage(display,resource_info,&windows->image,images[0],
    images[0]->columns,images[0]->rows);
  status|=XMakePixmap(display,resource_info,&windows->image);
  if (status == False)
    Error("Unable to create X image",(char *) NULL);
  if (resource_info->debug)
    {
      (void) fprintf(stderr,"Image: [%u] %s %ux%u ",images[0]->scene,
        images[0]->filename,images[0]->columns,images[0]->rows);
      if (images[0]->colors != 0)
        (void) fprintf(stderr,"%uc ",images[0]->colors);
      (void) fprintf(stderr,"%s\n",images[0]->magick);
    }
  /*
    Initialize info window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->info);
  windows->info.name="ImageMagick Info";
  windows->info.x=2;
  windows->info.y=2;
  windows->info.flags|=PPosition;
  windows->info.attributes.win_gravity=UnmapGravity;
  windows->info.attributes.event_mask=ButtonPressMask | OwnerGrabButtonMask |
    StructureNotifyMask;
  class_hint->res_name="info";
  manager_hints->flags=InputHint | StateHint | WindowGroupHint;
  manager_hints->input=False;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,windows->image.id,argv,argc,class_hint,manager_hints,
    &windows->info);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (info)\n",windows->info.id);
  /*
    Initialize command window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->command);
  /*
    Initialize popup window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->popup);
  windows->popup.name=(char *) malloc(MaxTextLength*sizeof(char));
  if (windows->popup.name == NULL)
    Error("Unable to create popup window","Memory allocation failed");
  windows->popup.border_width=0;
  windows->popup.flags|=PPosition;
  windows->popup.attributes.backing_store=WhenMapped;
  windows->popup.attributes.save_under=True;
  windows->popup.attributes.event_mask=ButtonMotionMask | ButtonPressMask |
    ButtonReleaseMask | EnterWindowMask | ExposureMask | KeyPressMask |
    KeyReleaseMask | LeaveWindowMask | StructureNotifyMask;
  class_hint->res_name="popup";
  manager_hints->flags=InputHint | StateHint | WindowGroupHint;
  manager_hints->input=False;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->popup);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (popup)\n",windows->popup.id);
  windows->popup.highlight_stipple=XCreateBitmapFromData(display,
    windows->popup.id,(char *) HighlightBitmap,8,8);
  windows->popup.shadow_stipple=XCreateBitmapFromData(display,
    windows->popup.id,(char *) ShadowBitmap,8,8);
  XSetTransientForHint(display,windows->popup.id,windows->image.id);
  /*
    Initialize image pixmaps structure.
  */
  windows->image.pixmaps=(Pixmap *) malloc(number_scenes*sizeof(Pixmap));
  if (windows->image.pixmaps == (Pixmap *) NULL)
    Error("Unable to animate images","Memory allocation failed");
  windows->image.pixmaps[0]=windows->image.pixmap;
  scene_info.pixels=(unsigned long *) NULL;
  for (scene=1; scene < number_scenes; scene++)
  {
    /*
      Create X image.
    */
    windows->image.pixmap=(Pixmap) NULL;
    if (images[scene]->packed_pixels != (unsigned char *) NULL)
      {
        status=RunlengthDecodeImage(images[scene]);
        if (status == False)
          Error("Unable to unpack image",(char *) NULL);
      }
    if ((resource_info->map_type != (char *) NULL) ||
        (visual_info->class == TrueColor) ||
        (visual_info->class == DirectColor))
      if (images[scene]->class == PseudoClass)
        {
          /*
            Get pixel info for this scene.
          */
          XGetPixelInfo(display,visual_info,map_info,resource_info,
            images[scene],&scene_info);
          windows->image.pixel_info=(&scene_info);
        }
    status=XMakeImage(display,resource_info,&windows->image,images[scene],
      images[scene]->columns,images[scene]->rows);
    status|=XMakePixmap(display,resource_info,&windows->image);
    if (status == False)
      Error("Unable to create X image",(char *) NULL);
    if (resource_info->debug)
      {
        (void) fprintf(stderr,"Image: [%u] %s %ux%u ",images[scene]->scene,
          images[scene]->filename,images[scene]->columns,images[scene]->rows);
        if (images[scene]->colors != 0)
          (void) fprintf(stderr,"%uc ",images[scene]->colors);
        (void) fprintf(stderr,"%s\n",images[scene]->magick);
      }
    windows->image.pixmaps[scene]=windows->image.pixmap;
    /*
      Free image pixels.
    */
    if (scene_info.pixels != (unsigned long *) NULL)
      (void) free((char *) scene_info.pixels);
    (void) free((char *) images[scene]->pixels);
    images[scene]->pixels=(RunlengthPacket *) NULL;
  }
  windows->image.pixel_info=(&pixel_info);
  XMapWindow(display,windows->image.id);
  /*
    Respond to events.
  */
  scene=0;
  image=images[0];
  state=ForwardAnimationState | PlayAnimationState | RepeatAnimationState;
  do
  {
    if (XEventsQueued(display,QueuedAfterFlush) == 0)
      if ((state & PlayAnimationState) || (state & StepAnimationState))
        {
          if (windows->info.mapped)
            XWithdrawWindow(display,windows->info.id,windows->info.screen);
          if (resource_info->title == (char *) NULL)
            {
              register char
                *p;

              XTextProperty
                window_name;

              /*
                Window name is the base of the filename.
              */
              p=images[scene]->filename+strlen(images[scene]->filename)-1;
              while ((p > images[scene]->filename) && (*(p-1) != '/'))
                p--;
              (void) strcpy(windows->image.name,"ImageMagick: ");
              (void) strcat(windows->image.name,p);
              status=
                XStringListToTextProperty(&windows->image.name,1,&window_name);
              if (status != 0)
                XSetWMName(display,windows->image.id,&window_name);
            }
          /*
            Copy X pixmap to image window.
          */
          image=images[scene];
          windows->image.pixmap=windows->image.pixmaps[scene];
          XRefreshWindow(display,&windows->image,(XEvent *) NULL);
          XSync(display,False);
          if (state & StepAnimationState)
            state&=(~StepAnimationState);
          if (resource_info->delay != 0)
            XDelay(display,(unsigned long) resource_info->delay);
          if (state & ForwardAnimationState)
            {
              /*
                Forward animation:  increment scene number.
              */
              scene++;
              if (scene == number_scenes)
                if (state & AutoReverseAnimationState)
                  {
                    state&=(~ForwardAnimationState);
                    scene--;
                  }
                else
                  {
                    if (!(state & RepeatAnimationState))
                      state&=(~PlayAnimationState);
                    scene=0;
                  }
            }
          else
            {
              /*
                Reverse animation:  decrement scene number.
              */
              scene--;
              if (scene < 0)
                if (state & AutoReverseAnimationState)
                  {
                    state|=ForwardAnimationState;
                    scene=0;
                  }
                else
                  {
                    if (!(state & RepeatAnimationState))
                      state&=(~PlayAnimationState);
                    scene=number_scenes-1;
                  }
            }
          continue;
        }
    /*
      Handle a window event.
    */
    XNextEvent(display,&event);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Button Press: 0x%lx %u +%d+%d\n",
            event.xbutton.window,event.xbutton.button,event.xbutton.x,
            event.xbutton.y);
        if (event.xbutton.window == windows->backdrop.id)
          {
            XSetInputFocus(display,event.xbutton.window,RevertToParent,
              event.xbutton.time);
            break;
          }
        if (event.xbutton.window == windows->image.id)
          switch (event.xbutton.button)
          {
            case Button1:
            {
              char
                command[MaxTextLength];

              int
                command_number;

              static char
                *CommandMenu[]=
                {
                  "Image Info",
                  "Play",
                  "Step",
                  "Repeat",
                  "Auto Reverse",
                  "Slower",
                  "Faster",
                  "Forward",
                  "Reverse",
                  "Quit",
                  (char *) NULL,
                };

              static KeySym
                Commands[]=
                {
                  XK_i,
                  XK_p,
                  XK_s,
                  XK_period,
                  XK_a,
                  XK_less,
                  XK_greater,
                  XK_f,
                  XK_r,
                  XK_q
                };

              /*
                Select a command from the pop-up menu.
              */
              command_number=XMenuWidget(display,windows,"Commands",
                CommandMenu,command);
              if (command_number >= 0)
                XMagickCommand(display,resource_info,windows,
                  Commands[command_number],&image,&state);
              break;
            }
            default:
              break;
          }
        break;
      }
      case ClientMessage:
      {
        /*
          If client window delete message, exit.
        */
        if (event.xclient.message_type != windows->wm_protocols)
          break;
        if (*event.xclient.data.l == windows->wm_take_focus)
          {
            XSetInputFocus(display,event.xclient.window,RevertToParent,
              event.xclient.data.l[1]);
            break;
          }
        if (*event.xclient.data.l != windows->wm_delete_window)
          break;
        XWithdrawWindow(display,event.xclient.window,visual_info->screen);
        if (event.xclient.window == windows->image.id)
          {
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Configure Notify: 0x%lx %dx%d+%d+%d\n",
            event.xconfigure.window,event.xconfigure.width,
            event.xconfigure.height,event.xconfigure.x,event.xconfigure.y);
        if (event.xconfigure.window == windows->image.id)
          {
            /*
              Image window has a new configuration.
            */
            windows->image.width=event.xconfigure.width;
            windows->image.height=event.xconfigure.height;
            break;
          }
        if (event.xconfigure.window == windows->icon.id)
          {
            /*
              Icon window has a new configuration.
            */
            windows->icon.width=event.xconfigure.width;
            windows->icon.height=event.xconfigure.height;
            break;
          }
      }
      case EnterNotify:
      {
        /*
          Selectively install colormap.
        */
        if (map_info->colormap != XDefaultColormap(display,visual_info->screen))
          if (event.xcrossing.mode != NotifyUngrab)
            XInductColormap(display,map_info->colormap);
        break;
      }
      case Expose:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Expose: 0x%lx %dx%d+%d+%d\n",
            event.xexpose.window,event.xexpose.width,event.xexpose.height,
            event.xexpose.x,event.xexpose.y);
        /*
          Repaint windows that are now exposed.
        */
        if (event.xexpose.window == windows->image.id)
          {
            windows->image.pixmap=windows->image.pixmaps[scene];
            XRefreshWindow(display,&windows->image,&event);
            break;
          }
        break;
      }
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static int
          length;

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (resource_info->debug)
          (void) fprintf(stderr,"Key press: 0x%lx (%c)\n",key_symbol,*command);
        if (key_symbol == XK_Help)
          Usage(False);
        else
          if (!IsCursorKey(key_symbol))
            XMagickCommand(display,resource_info,windows,key_symbol,&image,
              &state);
        break;
      }
      case LeaveNotify:
      {
        /*
          Selectively uninstall colormap.
        */
        if (map_info->colormap != XDefaultColormap(display,visual_info->screen))
          if (event.xcrossing.mode != NotifyUngrab)
            XUninductColormap(display,map_info->colormap);
        break;
      }
      case MapNotify:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Map Notify: 0x%lx\n",event.xmap.window);
        if (event.xmap.window == windows->image.id)
          {
            if (windows->backdrop.id != (Window) NULL)
              XInstallColormap(display,map_info->colormap);
            windows->image.mapped=True;
            break;
          }
        if (event.xmap.window == windows->info.id)
          {
            windows->info.mapped=True;
            break;
          }
        if (event.xmap.window == windows->icon.id)
          {
            /*
              Create an icon image.
            */
            XMakeStandardColormap(display,icon_visual,&icon_resources,images[0],
              icon_map,&icon_pixel);
            (void) XMakeImage(display,&icon_resources,&windows->icon,images[0],
              windows->icon.width,windows->icon.height);
            (void) XMakePixmap(display,&icon_resources,&windows->icon);
            XSetWindowBackgroundPixmap(display,windows->icon.id,
              windows->icon.pixmap);
            XClearWindow(display,windows->icon.id);
            windows->icon.mapped=True;
            break;
          }
        break;
      }
      case MappingNotify:
      {
        XRefreshKeyboardMapping(&event.xmapping);
        break;
      }
      case ReparentNotify:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Reparent Notify: 0x%lx=>0x%lx\n",
            event.xreparent.parent,event.xreparent.window);
        break;
      }
      case UnmapNotify:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Unmap Notify: 0x%lx\n",event.xunmap.window);
        if (event.xunmap.window == windows->image.id)
          {
            windows->image.mapped=False;
            break;
          }
        if (event.xunmap.window == windows->info.id)
          {
            windows->info.mapped=False;
            break;
          }
        if (event.xunmap.window == windows->icon.id)
          {
            windows->icon.mapped=False;
            break;
          }
        break;
      }
      default:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Event type: %d\n",event.type);
        break;
      }
    }
  }
  while (!(state & ExitState));
  /*
    Free X resources.
  */
  (void) free((char *) windows->popup.name);
  (void) free((char *) windows->image.name);
  (void) free((char *) windows->image.icon_name);
  if (resource_info->backdrop)
    XFreeCursor(display,windows->backdrop.cursor);
  if (windows->popup.highlight_stipple != (Pixmap) NULL)
    XFreePixmap(display,windows->popup.highlight_stipple);
  if (windows->popup.highlight_stipple != (Pixmap) NULL)
    XFreePixmap(display,windows->popup.shadow_stipple);
  XFreeGC(display,pixel_info.annotate_context);
  XFreeGC(display,pixel_info.highlight_context);
  XFreeFont(display,font_info);
  /*
    Destroy X windows.
  */
  for (i=0; i < number_windows; i++)
  {
    if (magick_windows[i]->id != (Window) NULL)
      XDestroyWindow(display,magick_windows[i]->id);
    if (magick_windows[i]->ximage != (XImage *) NULL)
      XDestroyImage(magick_windows[i]->ximage);
    if (magick_windows[i]->pixmap != (Pixmap) NULL)
      XFreePixmap(display,magick_windows[i]->pixmap);
    XFreeCursor(display,magick_windows[i]->cursor);
    XFreeCursor(display,magick_windows[i]->busy_cursor);
  }
  for (scene=0; scene < number_scenes; scene++)
  {
    XFreePixmap(display,windows->image.pixmaps[scene]);
    DestroyImage(images[scene]);
  }
  (void) free((char *) windows->image.pixmaps);
  /*
    Free Standard Colormap.
  */
  XFreeStandardColormap(display,icon_visual,icon_map,&icon_pixel);
  if (resource_info->map_type == (char *) NULL)
    XFreeStandardColormap(display,visual_info,map_info,&pixel_info);
  (void) free((void *) windows);
  XFree((void *) manager_hints);
  XFree((void *) class_hint);
  XFree((void *) icon_visual);
  XFree((void *) visual_info);
  XFree((void *) icon_map);
  XFree((void *) map_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a g i c k C o m m a n d                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMagickCommand makes a transform to the image or image window as
%  specified by a user menu button or keyboard command.
%
%  The format of the XMagickCommand routine is:
%
%    XMagickCommand(display,resource_info,windows,image,key_symbol,state);
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  XMagickCommand
%      may transform the image and return a new image pointer.
%
%    o state: Specifies an unsigned int;  XMagickCommand may return a
%      modified state.
%
%
*/
static void XMagickCommand(display,resource_info,windows,key_symbol,image,state)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

KeySym
  key_symbol;

Image
  **image;

unsigned int
  *state;
{
  /*
    Process user command.
  */
  if (windows->info.mapped)
    XWithdrawWindow(display,windows->info.id,windows->info.screen);
  switch (key_symbol)
  {
    case XK_KP_Space:
      break;
    case XK_Help:
    {
      Usage(False);
      break;
    }
    case XK_less:
    {
      resource_info->delay<<=1;
      if (resource_info->delay == 0)
        resource_info->delay=1;
      break;
    }
    case XK_greater:
    {
      resource_info->delay>>=1;
      break;
    }
    case XK_period:
    {
      *state|=RepeatAnimationState;
      *state&=(~AutoReverseAnimationState);
      *state|=PlayAnimationState;
      break;
    }
    case XK_a:
    {
      *state|=AutoReverseAnimationState;
      *state&=(~RepeatAnimationState);
      *state|=PlayAnimationState;
      break;
    }
    case XK_f:
    {
      *state=ForwardAnimationState;
      *state&=(~AutoReverseAnimationState);
      break;
    }
    case XK_i:
    {
      char
        *text,
        **textlist,
        title[MaxTextLength];

      int
        length;

      /*
        Display information about the image in the Text View widget.
      */
      length=11*MaxTextLength;
      if ((*image)->directory != (char *) NULL)
        length+=strlen((*image)->directory);
      if ((*image)->comments != (char *) NULL)
        length+=strlen((*image)->comments);
      text=(char *) malloc(length*sizeof(char));
      if (text == (char *) NULL)
        {
          XNoticeWidget(display,windows,"Unable to display image info",
            "Memory allocation failed");
          break;
        }
      (void) sprintf(title," Image Info: %s",(*image)->filename);
      (void) sprintf(text,"X\n  visual: %s\n",
        XVisualClassName(windows->image.class));
      (void) sprintf(text,"%s  depth: %d\n",text,windows->image.depth);
      (void) sprintf(text,"%s  geometry: %dx%d\n",text,
        windows->image.ximage->width,windows->image.ximage->height);
      if (windows->image.clip_geometry != (char *) NULL)
        (void) sprintf(text,"%s  clip geometry: %s\n",text,
          windows->image.clip_geometry);
      (void) sprintf(text,"%sImage\n  format: %s\n",text,(*image)->magick);
      if ((*image)->class == DirectClass)
        (void) strcat(text,"  class: DirectClass\n");
      else
        (void) strcat(text,"  class: PseudoClass\n");
      (void) sprintf(text,"%sImage geometry: %dx%d\n",text,
        (*image)->columns,(*image)->rows);
      if ((*image)->colors != 0)
        (void) sprintf(text,"%s  colors: %u\n",text,(*image)->colors);
      if ((*image)->signature != (char *) NULL)
        (void) sprintf(text,"%s  signature: %s\n",text,(*image)->signature);
      if ((*image)->scene != 0)
        (void) sprintf(text,"%s  scene: %u\n",text,(*image)->scene);
      if ((*image)->montage != (char *) NULL)
        (void) sprintf(text,"%s  montage: %s\n",text,(*image)->montage);
      if ((*image)->directory != (char *) NULL)
        (void) sprintf(text,"%s  directory:\n\n%s\n",text,(*image)->directory);
      if ((*image)->label != (char *) NULL)
        (void) sprintf(text,"%s  label: %s\n",text,(*image)->label);
      if ((*image)->comments != (char *) NULL)
        (void) sprintf(text,"%s  comments:\n\n%s",text,(*image)->comments);
      textlist=StringToList(text);
      if (textlist != (char **) NULL)
        {
          register int
            i;

          XTextViewWidget(display,windows,title,textlist);
          for (i=0; textlist[i] != (char *) NULL; i++)
            (void) free((char *) textlist[i]);
          (void) free((char *) textlist);
        }
      (void) free((char *) text);
      break;
    }
    case XK_p:
    {
      *state|=PlayAnimationState;
      *state&=(~AutoReverseAnimationState);
      break;
    }
    case XK_s:
    case XK_Return:
    case XK_KP_Enter:
    {
      *state|=StepAnimationState;
      *state&=(~PlayAnimationState);
      break;
    }
    case XK_q:
    {
      /*
        Exit program
      */
      *state|=ExitState;  /* exit program */
      break;
    }
    case XK_r:
    case XK_BackSpace:
    {
      *state&=(~ForwardAnimationState);
      *state&=(~AutoReverseAnimationState);
      break;
    }
    default:
    {
      if (!IsModifierKey(key_symbol))
        XBell(display,0);
      break;
    }
  }
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
    *density,
    *gamma,
    *option,
    *resource_value,
    *server_name;

  Display
    *display;

  Image
    **images;

  int
    i,
    x;

  unsigned int
    first_scene,
    image_number,
    interlace,
    last_scene,
    maximum_images,
    scene,
    verbose;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  /*
    Display usage profile if there are no command line arguments.
  */
  client_name=(*argv);
  if (argc < 2)
    Usage(True);
  /*
    Set defaults.
  */
  display=(Display *) NULL;
  first_scene=0;
  last_scene=0;
  server_name=(char *) NULL;
  maximum_images=MaxTextLength;
  images=(Image **) malloc(maximum_images*sizeof(Image *));
  if (images == (Image **) NULL)
    Error("Unable to animate images","Memory allocation failed");
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
          if (*option == '-')
            {
              i++;
              if (i == argc)
                Error("Missing server name on -display",(char *) NULL);
              server_name=argv[i];
              break;
            }
        }
  }
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
  clip_geometry=XGetResourceClass(resource_database,client_name,
    "clipGeometry",(char *) NULL);
  resource_value=XGetResourceClass(resource_database,client_name,"delay","30");
  resource_info.delay=atoi(resource_value);
  density=XGetResourceClass(resource_database,client_name,"density",
    (char *) NULL);
  gamma=XGetResourceClass(resource_database,client_name,"gamma",(char *) NULL);
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
  resource_value=
    XGetResourceClass(resource_database,client_name,"verbose","False");
  verbose=IsTrue(resource_value);
  /*
    Parse command line.
  */
  image_number=0;
  for (i=1; i < argc; i++)
  {
    option=argv[i];
    if (((int) strlen(option) > 1) && ((*option == '-') || (*option == '+')))
      switch (*(option+1))
      {
        case 'b':
        {
          if (strncmp("backdrop",option+1,5) == 0)
            {
              resource_info.backdrop=(*option == '-');
              break;
            }
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
          if (strncmp("colormap",option+1,6) == 0)
            {
              resource_info.colormap=PrivateColormap;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing type on -colormap",(char *) NULL);
                  option=argv[i];
                  resource_info.colormap=UndefinedColormap;
                  if (Latin1Compare("private",option) == 0)
                    resource_info.colormap=PrivateColormap;
                  if (Latin1Compare("shared",option) == 0)
                    resource_info.colormap=SharedColormap;
                  if (resource_info.colormap == UndefinedColormap)
                    Error("Invalid colormap type on -colormap",option);
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
          Error("Unrecognized option",option);
          break;
        }
        case 'd':
        {
          if (strncmp("debug",option+1,3) == 0)
            {
              resource_info.debug=(*option == '-');
              break;
            }
          if (strncmp("delay",option+1,3) == 0)
            {
              resource_info.delay=0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing seconds on -delay",(char *) NULL);
                  resource_info.delay=atoi(argv[i]);
                }
              break;
            }
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
          Error("Unrecognized option",option);
          break;
        }
        case 'h':
        {
          Usage(True);
          break;
        }
        case 'i':
        {
          if (strncmp("iconGeometry",option+1,5) == 0)
            {
              resource_info.icon_geometry=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -iconGeometry",(char *) NULL);
                  resource_info.icon_geometry=argv[i];
                }
              break;
            }
          if (strncmp("iconic",option+1,5) == 0)
            {
              resource_info.iconic=(*option == '-');
              break;
            }
          if (strncmp("interlace",option+1,3) == 0)
            {
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
          if (strncmp("map",option+1,3) == 0)
            {
              resource_info.map_type=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing map type on -map",(char *) NULL);
                  resource_info.map_type=argv[i];
                }
              break;
            }
          if (strncmp("mattecolor",option+1,6) == 0)
            {
              resource_info.matte_color=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing color on -mattecolor",(char *) NULL);
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
        case 'n':
        {
          resource_info.name=(char *) NULL;
          if (*option == '-')
            {
              i++;
              if (i == argc)
                Error("Missing name on -name",(char *) NULL);
              resource_info.name=argv[i];
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
          if (strncmp("title",option+1,2) == 0)
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
          if (strncmp("verbose",option+1,2) == 0)
            {
              verbose=(*option == '-');
              break;
            }
          if (strncmp("visual",option+1,2) == 0)
            {
              resource_info.visual_type=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing visual class on -visual",(char *) NULL);
                  resource_info.visual_type=argv[i];
                }
              break;
            }
          Error("Unrecognized option",option);
          break;
        }
        case 'w':
        {
          if (strncmp("window",option+1,2) == 0)
            {
              resource_info.window_id=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing id, name, or 'root' on -window",
                      (char *) NULL);
                  resource_info.window_id=argv[i];
                }
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
    else
      for (scene=first_scene; scene <= last_scene ; scene++)
      {
        double
          normalized_maximum_error,
          normalized_mean_error;

        Image
          *image,
          info_image,
          *next_image;

        ImageInfo
          image_info;

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
        image_info.density=density;
        image_info.interlace=interlace;
        image_info.monochrome=resource_info.monochrome;
        image_info.verbose=verbose;
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          if (*option == '-')
            break;
          else
            continue;
        do
        {
          info_image=(*image);
          if (image->scene == 0)
            image->scene=image_number;
          total_colors=0;
          /*
            Transform image as defined by the clip, image and scale geometries.
          */
          TransformImage(&image,clip_geometry,resource_info.image_geometry);
          if (gamma != (char *) NULL)
            GammaImage(image,gamma);
          if (resource_info.number_colors != 0)
            {
              /*
                Reduce the number of colors in the image.
              */
              if ((image->class == DirectClass) ||
                  (image->colors > resource_info.number_colors) ||
                  (resource_info.colorspace == GRAYColorspace))
                QuantizeImage(image,resource_info.number_colors,
                  resource_info.tree_depth,resource_info.dither,
                  resource_info.colorspace,True);
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
              (void) fprintf(stderr,"[%u] %s",image->scene,info_image.filename);
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
                Error("Unable to animate images","Memory allocation failed");
            }
          images[image_number++]=image;
          next_image=image->next;
          if (next_image != (Image *) NULL)
            image=next_image;
        } while (next_image != (Image *) NULL);
      }
  }
  if (image_number == 0)
    Error("Missing an image file name",(char *) NULL);
  if (resource_info.window_id != (char *) NULL)
    XAnimateBackgroundImage(display,&resource_info,images,image_number);
  else
    XAnimateImage(display,&resource_info,argv,argc,images,image_number);
  (void) free((char *) images);
  XCloseDisplay(display);
  return(False);
}
