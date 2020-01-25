/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%             DDDD   IIIII  SSSSS  PPPP   L       AAA   Y   Y                 %
%             D   D    I    SS     P   P  L      A   A   Y Y                  %
%             D   D    I     SSS   PPPP   L      AAAAA    Y                   %
%             D   D    I       SS  P      L      A   A    Y                   %
%             DDDD   IIIII  SSSSS  P      LLLLL  A   A    Y                   %
%                                                                             %
%                                                                             %
%          Display Machine Independent File Format Image via X11.             %
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
%  Display is a machine architecture independent image processing
%  and display program.  It can display any image in the MIFF format on
%  any workstation display running X.  Display first determines the
%  hardware capabilities of the workstation.  If the number of unique
%  colors in the image is less than or equal to the number the workstation
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
%  The Display program command syntax is:
%
%  Usage: display [options ...] file [ [options ...] file ...]
%
%  Where options include:
%    -backdrop           display image centered on a backdrop
%    -blur               apply a filter to blur the image
%    -border geometry    surround image with a border of color
%    -clip geometry      preferred size and location of the clipped image
%    -colormap type      Shared or Private
%    -colors value       preferred number of colors in the image
%    -colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV
%    -comment string     annotate image with comment",
%    -compress type      RunlengthEncoded or QEncoded
%    -delay seconds      display the next image after pausing
%    -density geometry   vertical and horizontal density of the image
%    -despeckle          reduce the speckles within an image
%    -display server     display image to this X server
%    -dither             apply Floyd/Steinberg error diffusion to image
%    -edge               apply a filter to detect edges in the image
%    -enhance            apply a digital filter to enhance a noisy image
%    -equalize           perform histogram equalization to an image
%    -flip               flip image in the vertical direction
%    -flop               flop image in the horizontal direction
%    -gamma value        level of gamma correction
%    -geometry geometry  preferred size and location of the image window
%    -interlace type     NONE, LINE, or PLANE
%    -label name         assign a label to an image
%    -map type           display image using this Standard Colormap
%    -matte              store matte channel if the image has one
%    -monochrome         transform image to black and white
%    -negate             apply color inversion to image
%    -noise              reduce noise with a noise peak elimination filter
%    -normalize          transform image to span the full the range of colors
%    -page geometry      size and location of the Postscript page
%    -quality value      JPEG quality setting
%    -roll geometry      roll an image vertically or horizontally
%    -rotate degrees     apply Paeth rotation to the image
%    -scene value        image scene number
%    -sharpen            apply a filter to sharpen the image
%    -shear geometry     slide one edge of the image along the X or Y axis
%    -treedepth value    depth of the color classification tree
%    -update seconds     detect when image file is modified and redisplay
%    -verbose            print detailed information about the image
%    -visual type        display image using this visual type
%    -window id          display image to background of this window
%    -write filename     write image to a file
%
%  In addition to those listed above, you can specify these standard X
%  resources as command line options:  -background, -bordercolor,
%  -borderwidth, -font, -foreground, -iconGeometry, -iconic, -mattecolor,
%  -name, or -title.
%
%  Change '-' to '+' in any option above to reverse its effect.  For
%  example, specify +matte to store the image without its matte channel.
%
%  By default, the image format of `file' is determined by its magic
%  number.  To specify a particular image format, precede the filename
%  with an image format name and a colon (i.e. ps:image) or specify the
%  image type as the filename suffix (i.e. image.ps).  Specify 'file' as
%  '-' for standard input or output.
%
%  Buttons:
%    1    press and drag to select a command from a menu
%    2    press and drag to select an image editing command from a menu
%    3    press to magnify a region or load an image from a montage
%
%  Keyboard accelerators:
%    i    display information about the image
%    w    write the image to a file
%    p    print the image to a Postscript printer
%    d    delete the image file
%    l    load an image from a file
%    n    display the next image
%    f    display the former image
%    u    undo last image transformation
%    r    restore the image to its original size
%    @    refresh the image window
%    <    half the image size
%    o    original image size
%    >    double the image size
%    %    resize the image
%    t    trim the image edges
%    [    clip the image
%    ]    cut the image
%    |    flop image in the horizontal direction
%    -    flip image in the vertical direction
%    /    rotate the image 90 degrees clockwise
%    \    rotate the image 90 degrees counter-clockwise
%    *    rotate the image
%    s    shear the image
%    ~    invert the colors of the image
%    =    perform histogram equalization on the image
%    N    perform histogram normalization on the image
%    g    gamma correct the image
%    D    reduce the speckles within an image
%    P    eliminate peak noise from an image
%    S    sharpen the image
%    B    blur the image
%    E    detect edges within an image
%    G    convert the image to grayscale
%    Q    set the maximum number of unique colors in the image
%    a    annotate the image with text
%    b    add a border to the image
%    x    composite image with another
%    c    edit an image pixel color
%    m    edit the image matte information
%    #    edit an image comment
%    h    display information about this program
%    v    display version number of this program
%    q    discard all images and exit program
%    1-9  change the level of magnification
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "image.h"
#include "utility.h"
#include "X.h"
#include "widget.h"
#include "PreRvIcccm.h"

/*
  State declarations.
*/
#define DefaultState  0x0000
#define EscapeState  0x0001
#define ExitState  0x0002
#define FormerImageState  0x0004
#define ModifierState  0x0008
#define MontageImageState  0x0010
#define NextImageState  0x0020
#define UpdateConfigurationState  0x0040

/*
  Forward declarations.
*/
static Image
  *XMagickCommand _Declare((Display *,XResourceInfo *,XWindows *,unsigned int,
    KeySym,Image **)),
  *XTileImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image *,
    XEvent *));

static int
  XScreenEvent _Declare((Display *,XEvent *,char *));

static unsigned int
  XConfigureImageWindow _Declare((Display *,XResourceInfo *,XWindows *,
    Image *)),
  XMatteEditImageWindow _Declare((Display *,XResourceInfo *,XWindows *,
    Image *)),
  XNoisyImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image **)),
  XPrintImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image *)),
  XRotateImageWindow _Declare((Display *,XResourceInfo *,XWindows *,double,
    Image **)),
  XSharpenImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image **)),
  XShearImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image **)),
  XTrimImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image *)),
  XWriteImageWindow _Declare((Display *,XResourceInfo *,XWindows *,Image *));

static void
  XConfigureImageColormap _Declare((Display *,XResourceInfo *,XWindows *,
    Image *)),
  XDrawPanRectangle _Declare((Display *,XWindows *)),
  XMagnifyImageWindow _Declare((Display *,XWindows *,XEvent *)),
  XMakePanImage _Declare((Display *,XResourceInfo *,XWindows *,Image *)),
  XPanImageWindow _Declare((Display *,XWindows *,XEvent *)),
  XMagnifyWindowCommand _Declare((Display *,XWindows *,KeySym)),
  XSetClipGeometry _Declare((Display *,XWindows *,RectangleInfo *,Image *)),
  XTranslateImageWindow _Declare((Display *,XWindows *,Image *,KeySym));

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
%    o terminate: A value other than zero is returned if the program is to
%      terminate immediately.
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
      "1    press and drag to select a command from a menu",
      "2    press and drag to select an image editing command from a menu",
      "3    press to magnify a region or load an image from a montage",
      (char *) NULL
    },
    *keys[]=
    {
      "i    display information about the image",
      "w    write the image to a file",
      "p    print the image to a Postscript printer",
      "d    delete the image file",
      "l    load an image from a file",
      "n    display the next image",
      "f    display the former image",
      "u    undo last image transformation",
      "r    restore the image to its original size",
      "@    refresh the image window",
      "<    half the image size",
      "o    original image size",
      ">    double the image size",
      "%    resize the image",
      "t    trim the image edges",
      "[    clip the image",
      "]    cut the image",
      "|    flop image in the horizontal direction",
      "-    flip image in the vertical direction",
      "/    rotate the image 90 degrees clockwise",
      "\\    rotate the image 90 degrees counter-clockwise",
      "*    rotate the image",
      "s    shear the image",
      "~    invert the colors of the image",
      "=    perform histogram equalization on the image",
      "N    perform histogram normalization on the image",
      "g    gamma correct the image",
      "D    reduce the speckles within an image",
      "P    eliminate peak noise from an image",
      "S    sharpen the image",
      "B    blur the image",
      "E    detect edges within an image",
      "G    convert the image to grayscale",
      "Q    set the maximum number of unique colors in the image",
      "a    annotate the image with text",
      "b    add a border to the image",
      "x    composite image with another",
      "c    edit an image pixel color",
      "m    edit the image matte information",
      "#    add an image comment",
      "h    display information about this program",
      "v    display version number of this program",
      "q    discard all images and exit program",
      "1-9  change the level of magnification",
      (char *) NULL
    },
    *options[]=
    {
      "-backdrop           display image centered on a backdrop",
      "-blur               apply a filter to blur the image",
      "-border geometry    surround image with a border of color",
      "-clip geometry      preferred size and location of the clipped image",
      "-colormap type      Shared or Private",
      "-colors value       preferred number of colors in the image",
      "-colorspace type    GRAY, OHTA, RGB, XYZ, YCbCr, YIQ, or YUV",
      "-comment string     annotate image with comment",
      "-compress type      RunlengthEncoded or QEncoded",
      "-delay seconds      display the next image after pausing",
      "-density geometry   vertical and horizontal density of the image",
      "-despeckle          reduce the speckles within an image",
      "-display server     display image to this X server",
      "-dither             apply Floyd/Steinberg error diffusion to image",
      "-edge               apply a filter to detect edges in the image",
      "-enhance            apply a digital filter to enhance a noisy image",
      "-equalize           perform histogram equalization to an image",
      "-flip               flip image in the vertical direction",
      "-flop               flop image in the horizontal direction",
      "-gamma value        level of gamma correction",
      "-geometry geometry  preferred size and location of the image window",
      "-interlace type     NONE, LINE, or PLANE",
      "-label name         assign a label to an image",
      "-map type           display image using this Standard Colormap",
      "-matte              store matte channel if the image has one",
      "-monochrome         transform image to black and white",
      "-negate             apply color inversion to image",
      "-noise              reduce noise with a noise peak elimination filter",
      "-normalize          transform image to span the full range of colors",
      "-page geometry      size and location of the Postscript page",
      "-quality value      JPEG quality setting",
      "-roll geometry      roll an image vertically or horizontally",
      "-rotate degrees     apply Paeth rotation to the image",
      "-scene value        image scene number",
      "-sharpen            apply a filter to sharpen the image",
      "-shear geometry     slide one edge of the image along the X or Y axis",
      "-treedepth value    depth of the color classification tree",
      "-update seconds     detect when image file is modified and redisplay",
      "-verbose            print detailed information about the image",
      "-visual type        display image using this visual type",
      "-window id          display image to background of this window",
      "-write filename     write image to a file",
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
    "-borderwidth, -font, -foreground, -iconGeometry, -iconic,\n");
  (void) fprintf(stderr,"-mattecolor, -name, or -title.\n");
  (void) fprintf(stderr,
    "\nChange '-' to '+' in any option above to reverse its effect.  For\n");
  (void) fprintf(stderr,
    "example, specify +matte to store the image without a matte channel.\n");
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
%   X A n n o t a t e I m a g e W i n d o w                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XAnnotateImageWindow annotates the image with text.
%
%  The format of the XAnnotateImageWindow routine is:
%
%    XAnnotateImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
*/
static unsigned int XAnnotateImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
  static char
    *ImageAnnotateHelp[]=
    {
      "Press a button to affect this behavior:",
      "",
      "1    Press to select a location within an image window to",
      "     begin entering text.",
      "",
      "2    Press and drag to select a font color from a pop-up",
      "     menu.  Choose from these font colors:",
      "",
      "         black",
      "         blue",
      "         cyan",
      "         green",
      "         gray",
      "         red",
      "         magenta",
      "         yellow",
      "         white",
      "         Browser...",
      "",
      "If you select the color browser and press Grab, you can",
      "choose the font color by moving the pointer to the desired",
      "color on the screen and press any button.",
      "",
      "3    Press and drag to select a font from a pop-up menu.",
      "     Choose from these fonts:",
      "",
      "         fixed",
      "         variable",
      "         5x8",
      "         6x10",
      "         7x13bold",
      "         8x13bold",
      "         9x15bold",
      "         10x20",
      "         12x24",
      "         Browser...",
      "",
      "     Other fonts can be specified with the font browser or",
      "     by setting the X resources font1 through font9.",
      "",
      "Choosing a font and its color is optional.  The default font",
      "is fixed and the default color is black.  However, you must",
      "choose a location to begin entering text and press button 1.",
      "An underscore character will appear at the location of the",
      "cursor where you pressed button 1.  The cursor changes to a",
      "pencil to indicate you are in text mode.  To exit",
      "immediately, press ESC.",
      "",
      "In text mode, any key presses will display the character at",
      "the location of the cursor and advance the underscore",
      "cursor.  Enter your text and once completed press ESC to",
      "finish your image annotation.  To correct errors press",
      "BACKSPACE.  To delete an entire line of text, press DELETE.",
      "Any text that exceeds the boundaries of the image window is",
      "automatically continued onto the next line.",
      "",
      "The actual color you request for the font is saved in the",
      "image.  However, the color that appears in your image window",
      "could be different.  For example, on a monochrome screen the",
      "text will appear black or white even if you choose the color",
      "red as the font color.  However, the image saved to a file",
      "with 'Write' is written with red lettering.  To assure",
      "the correct color text in the final image, any PseudoClass",
      "image is promoted to DirectClass.",
      (char *) NULL,
    };

  char
    *p,
    text[MaxTextLength];

  Cursor
    cursor;

  GC
    annotate_context;

  int
    i,
    x,
    y;

  static unsigned int
    font_id=0,
    pen_id=0;

  unsigned int
    height,
    mask,
    status,
    width;

  unsigned long
    state,
    x_factor,
    y_factor;

  Window
    xwindow;

  XAnnotateInfo
    *annotate_info,
    *previous_info;

  XFontStruct
    *font_info;

  XEvent
    event,
    text_event;

  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        switch (event.xbutton.button)
        {
          case Button1:
          {
            /*
              Change to text entering mode.
            */
            x=event.xbutton.x;
            y=event.xbutton.y;
            state|=ExitState;
            break;
          }
          case Button2:
          {
            char
              color_name[MaxTextLength],
              *ColorMenu[MaxNumberPens+1];

            int
              pen_number;

            /*
              Initialize menu selections.
            */
            for (i=0; i < MaxNumberPens; i++)
              ColorMenu[i]=resource_info->pen_color[i];
            ColorMenu[MaxNumberPens-1]="Browser...";
            ColorMenu[MaxNumberPens]=(char *) NULL;
            /*
              Select a pen color from the pop-up menu.
            */
            pen_number=
              XMenuWidget(display,windows,"Font Color",ColorMenu,color_name);
            if (pen_number == (MaxNumberPens-1))
              {
                static char
                  color_name[MaxTextLength]="gray";

                /*
                  Select a pen color from a dialog.
                */
                resource_info->pen_color[MaxNumberPens-1]=color_name;
                XColorBrowserWidget(display,windows,"Select",color_name);
                if (*color_name == '\0')
                  break;
              }
            if (pen_number >= 0)
              {
                /*
                  Set pen color.
                */
                pen_id=pen_number;
                (void) strcpy(color_name,resource_info->pen_color[pen_id]);
                status=XParseColor(display,windows->image.map_info->colormap,
                  color_name,&windows->image.pixel_info->pen_color[pen_id]);
                if (status == 0)
                  XNoticeWidget(display,windows,"Color is unknown to X server",
                    color_name);
                XBestPixel(display,windows->image.map_info->colormap,
                  (XColor *) NULL,(unsigned int)
                  Min(windows->image.visual_info->colormap_size,256),
                  &windows->image.pixel_info->pen_color[pen_id]);
              }
            break;
          }
          case Button3:
          {
            char
              font_name[MaxTextLength],
              *FontMenu[MaxNumberFonts+1];

            int
              font_number;

            /*
              Initialize menu selections.
            */
            for (i=0; i < MaxNumberFonts; i++)
              FontMenu[i]=resource_info->font_name[i];
            FontMenu[MaxNumberFonts-1]="Browser...";
            FontMenu[MaxNumberFonts]=(char *) NULL;
            /*
              Select a font name from the pop-up menu.
            */
            font_number=
              XMenuWidget(display,windows,"Font Name",FontMenu,font_name);
            if (font_number == (MaxNumberFonts-1))
              {
                static char
                  font_name[MaxTextLength]="fixed";

                /*
                  Select a font name from a browser.
                */
                resource_info->font_name[MaxNumberFonts-1]=font_name;
                XFontBrowserWidget(display,windows,"Select",font_name);
                if (*font_name == '\0')
                  break;
              }
            if (font_number >= 0)
              {
                /*
                  Initialize font info.
                */
                (void) strcpy(font_name,resource_info->font_name[font_number]);
                font_info=XLoadQueryFont(display,font_name);
                if (font_info == (XFontStruct *) NULL)
                  XNoticeWidget(display,windows,"Unable to load font",
                    font_name);
                else
                  {
                    font_id=font_number;
                    XFreeFont(display,font_info);
                  }
              }
            break;
          }
        }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            state|=EscapeState;
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XTextViewWidget(display,windows,"Help Viewer - Image Annotation",
              ImageAnnotateHelp);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XFlush(display);
  if (state & EscapeState)
    return(True);
  /*
    Set font info and check boundary conditions.
  */
  font_info=XLoadQueryFont(display,resource_info->font_name[font_id]);
  if (font_info == (XFontStruct *) NULL)
    {
      XNoticeWidget(display,windows,"Unable to load font",
        resource_info->font_name[font_id]);
      font_info=windows->image.font_info;
    }
  if ((x+font_info->max_bounds.width) >= windows->image.width)
    x=windows->image.width-font_info->max_bounds.width;
  if (y < (font_info->ascent+font_info->descent))
    y=font_info->ascent+font_info->descent;
  if ((font_info->max_bounds.width > windows->image.width) ||
      ((font_info->ascent+font_info->descent) >= windows->image.height))
    return(False);
  /*
    Initialize annotate structure.
  */
  annotate_info=(XAnnotateInfo *) malloc(sizeof(XAnnotateInfo));
  if (annotate_info == (XAnnotateInfo *) NULL)
    return(False);
  XGetAnnotateInfo(annotate_info);
  annotate_info->x=x;
  annotate_info->y=y;
  annotate_info->height=font_info->ascent+font_info->descent;
  annotate_info->font_info=font_info;
  annotate_info->text=(char *) malloc(
    (windows->image.width/Max(font_info->min_bounds.width,1)+2)*sizeof(char));
  if (annotate_info->text == (char *) NULL)
    return(False);
  /*
    Create cursor and set graphic context.
  */
  cursor=XCreateFontCursor(display,XC_pencil);
  XDefineCursor(display,windows->image.id,cursor);
  annotate_context=windows->image.annotate_context;
  XSetFont(display,annotate_context,font_info->fid);
  XSetForeground(display,annotate_context,
    windows->image.pixel_info->pen_color[pen_id].pixel);
  /*
    Begin annotating the image with text.
  */
  state=DefaultState;
  XDrawString(display,windows->image.id,annotate_context,x,y,"_",1);
  text_event.xexpose.width=(unsigned int) font_info->max_bounds.width;
  text_event.xexpose.height=font_info->max_bounds.ascent+
    font_info->max_bounds.descent;
  p=annotate_info->text;
  do
  {
    /*
      Display text cursor.
    */
    *p='\0';
    XDrawString(display,windows->image.id,annotate_context,x,y,"_",1);
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    /*
      Erase text cursor.
    */
    text_event.xexpose.x=x;
    text_event.xexpose.y=y-font_info->max_bounds.ascent;
    XRefreshWindow(display,&windows->image,&text_event);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            break;
          }
        if (event.xbutton.button == Button2)
          {
            /*
              Request primary selection.
            */
            XConvertSelection(display,XA_PRIMARY,XA_STRING,XA_STRING,
              windows->image.id,CurrentTime);
            break;
          }
        break;
      }
      case Expose:
      {
        if (event.xexpose.count == 0)
          {
            XAnnotateInfo
              *text_info;

            /*
              Refresh image window.
            */
            XRefreshWindow(display,&windows->image,(XEvent *) NULL);
            text_info=annotate_info;
            while (text_info != (XAnnotateInfo *) NULL)
            {
              XDrawString(display,windows->image.id,annotate_context,
                text_info->x,text_info->y,text_info->text,
                strlen(text_info->text));
              text_info=text_info->previous;
            }
            XDrawString(display,windows->image.id,annotate_context,x,y,"_",1);
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
        if ((event.xkey.state & ControlMask) || (event.xkey.state & Mod1Mask))
          state|=ModifierState;
        if (state & ModifierState)
          switch (key_symbol)
          {
            case XK_u:
            case XK_U:
            {
              key_symbol=XK_Delete;
              break;
            }
            default:
              break;
          }
        switch (key_symbol)
        {
          case XK_BackSpace:
          {
            /*
              Erase one character.
            */
            if (p == annotate_info->text)
              if (annotate_info->previous == (XAnnotateInfo *) NULL)
                break;
              else
                {
                  /*
                    Go to end of the previous line of text.
                  */
                  annotate_info=annotate_info->previous;
                  p=annotate_info->text;
                  x=annotate_info->x+annotate_info->width;
                  y=annotate_info->y;
                  if (annotate_info->width != 0)
                    p+=strlen(annotate_info->text);
                  break;
                }
            p--;
            x-=XTextWidth(font_info,p,1);
            text_event.xexpose.x=x;
            text_event.xexpose.y=y-font_info->max_bounds.ascent;
            XRefreshWindow(display,&windows->image,&text_event);
            break;
          }
          case XK_Delete:
          {
            /*
              Erase the entire line of text.
            */
            while (p != annotate_info->text)
            {
              p--;
              x-=XTextWidth(font_info,p,1);
              text_event.xexpose.x=x;
              XRefreshWindow(display,&windows->image,&text_event);
            }
            break;
          }
          case XK_Escape:
          {
            /*
              Finished annotating.
            */
            annotate_info->width=XTextWidth(font_info,annotate_info->text,
              strlen(annotate_info->text));
            XRefreshWindow(display,&windows->image,&text_event);
            state|=ExitState;
            break;
          }
          default:
          {
            /*
              Draw a single character on the image window.
            */
            if (state & ModifierState)
              break;
            if (*command == '\0')
              break;
            *p=(*command);
            XDrawString(display,windows->image.id,annotate_context,x,y,p,1);
            x+=XTextWidth(font_info,p,1);
            p++;
            if ((x+font_info->max_bounds.width) < windows->image.width)
              break;
          }
          case XK_Return:
          case XK_KP_Enter:
          {
            /*
              Advance to the next line of text.
            */
            *p='\0';
            annotate_info->width=XTextWidth(font_info,annotate_info->text,
              strlen(annotate_info->text));
            if (annotate_info->next != (XAnnotateInfo *) NULL)
              {
                /*
                  Line of text already exists.
                */
                annotate_info=annotate_info->next;
                x=annotate_info->x;
                y=annotate_info->y;
                p=annotate_info->text;
                break;
              }
            annotate_info->next=(XAnnotateInfo *) malloc(sizeof(XAnnotateInfo));
            if (annotate_info->next == (XAnnotateInfo *) NULL)
              return(False);
            *annotate_info->next=(*annotate_info);
            annotate_info->next->previous=annotate_info;
            annotate_info=annotate_info->next;
            annotate_info->text=(char *) malloc((windows->image.width/
              Max(font_info->min_bounds.width,1)+2)*sizeof(char));
            if (annotate_info->text == (char *) NULL)
              return(False);
            annotate_info->y+=annotate_info->height;
            if (annotate_info->y > windows->image.height)
              annotate_info->y=annotate_info->height;
            annotate_info->next=(XAnnotateInfo *) NULL;
            x=annotate_info->x;
            y=annotate_info->y;
            p=annotate_info->text;
            break;
          }
        }
        break;
      }
      case KeyRelease:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        state&=(~ModifierState);
        break;
      }
      case SelectionNotify:
      {
        Atom
          type;

        int
          format;

        unsigned char
          *data;

        unsigned long
          after,
          length;

        /*
          Obtain response from primary selection.
        */
        if (event.xselection.property == (Atom) None)
          break;
        status=XGetWindowProperty(display,event.xselection.requestor,
          event.xselection.property,0L,2047L,True,XA_STRING,&type,&format,
          &length,&after,&data);
        if ((status != Success) || (type != XA_STRING) || (format == 32) ||
            (length == 0))
          break;
        /*
          Annotate image window with primary selection.
        */
        for (i=0; i < length; i++)
        {
          if (data[i] != '\n')
            {
              /*
                Draw a single character on the image window.
              */
              *p=data[i];
              XDrawString(display,windows->image.id,annotate_context,x,y,p,1);
              x+=XTextWidth(font_info,p,1);
              p++;
              if ((x+font_info->max_bounds.width) < windows->image.width)
                continue;
            }
          /*
            Advance to the next line of text.
          */
          *p='\0';
          annotate_info->width=XTextWidth(font_info,annotate_info->text,
            strlen(annotate_info->text));
          if (annotate_info->next != (XAnnotateInfo *) NULL)
            {
              /*
                Line of text already exists.
              */
              annotate_info=annotate_info->next;
              x=annotate_info->x;
              y=annotate_info->y;
              p=annotate_info->text;
              continue;
            }
          annotate_info->next=(XAnnotateInfo *)
            malloc(sizeof(XAnnotateInfo));
          if (annotate_info->next == (XAnnotateInfo *) NULL)
            return(False);
          *annotate_info->next=(*annotate_info);
          annotate_info->next->previous=annotate_info;
          annotate_info=annotate_info->next;
          annotate_info->text=(char *) malloc((windows->image.width/
            Max(font_info->min_bounds.width,1)+2)*sizeof(char));
          if (annotate_info->text == (char *) NULL)
            return(False);
          annotate_info->y+=annotate_info->height;
          if (annotate_info->y > windows->image.height)
            annotate_info->y=annotate_info->height;
          annotate_info->next=(XAnnotateInfo *) NULL;
          x=annotate_info->x;
          y=annotate_info->y;
          p=annotate_info->text;
        }
        XFree((void *) data);
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XSetFont(display,annotate_context,windows->image.font_info->fid);
  XSetForeground(display,annotate_context,
    windows->image.pixel_info->foreground_color.pixel);
  XFreeCursor(display,cursor);
  /*
    Annotation is relative to image configuration.
  */
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  if (windows->image.clip_geometry != (char *) NULL)
    (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
  /*
    Initialize annotated image.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  while (annotate_info != (XAnnotateInfo *) NULL)
  {
    if (annotate_info->width == 0)
      {
        /*
          No text on this line--  go to the next line of text.
        */
        previous_info=annotate_info->previous;
        (void) free((char *) annotate_info->text);
        (void) free((char *) annotate_info);
        annotate_info=previous_info;
        continue;
      }
    /*
      Determine foreground pixel index for font color.
    */
    windows->image.pixel_info->annotate_color=
      windows->image.pixel_info->pen_color[pen_id];
    windows->image.pixel_info->annotate_index=0;
    if (windows->image.pixel_info->colors != 0)
      for (i=0; i < windows->image.pixel_info->colors; i++)
        if (windows->image.pixel_info->pixels[i] ==
            windows->image.pixel_info->pen_color[pen_id].pixel)
          {
            windows->image.pixel_info->annotate_index=i;
            break;
          }
    /*
      Define the annotate geometry string.
    */
    x_factor=UpShift(width)/windows->image.ximage->width;
    annotate_info->x+=windows->image.x;
    annotate_info->x=DownShift(annotate_info->x*x_factor);
    y_factor=UpShift(height)/windows->image.ximage->height;
    annotate_info->y+=(windows->image.y-font_info->ascent);
    annotate_info->y=DownShift(annotate_info->y*y_factor);
    (void) sprintf(annotate_info->geometry,"%ux%u%+d%+d",
      (unsigned int) DownShift(annotate_info->width*x_factor),
      (unsigned int) DownShift(annotate_info->height*y_factor),
      annotate_info->x+x,annotate_info->y+y);
    /*
      Annotate image with text.
    */
    status=XAnnotateImage(display,windows->image.pixel_info,annotate_info,
      False,image);
    if (status == 0)
      return(False);
    /*
      Free up memory.
    */
    previous_info=annotate_info->previous;
    (void) free((char *) annotate_info->text);
    (void) free((char *) annotate_info);
    annotate_info=previous_info;
  }
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XFreeFont(display,font_info);
  /*
    Update image colormap.
  */
  XConfigureImageColormap(display,resource_info,windows,image);
  (void) XConfigureImageWindow(display,resource_info,windows,image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X B l u r I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XBlurImageWindow blurs an image.
%
%  The format of the XBlurImageWindow routine is:
%
%    status=XBlurImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XBlurImageWindow return True if the edges within
%      the image are detected.  False is returned is there is a memory
%      shortage or if the edges cannot be detected.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XBlurImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define BlurImageText  "  Blurring image...  "

  Image
    *blurred_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,BlurImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,BlurImageText);
  /*
    Blur image scanlines.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  blurred_image=BlurImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (blurred_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=blurred_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C l i p I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XClipImageWindow clips the image window.
%
%  The format of the XClipImageWindow routine is:
%
%    status=XClipImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XClipImageWindow returns True if the image is
%      clipped.  False is returned is there is a memory shortage or if the
%      image fails to be cropped.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
%
*/
static unsigned int XClipImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
  static char
    *ImageClipHelp[]=
    {
      "To define a clipping region, press button 1 and drag.  The",
      "clipping region is defined by a highlighted rectangle that",
      "expands or contracts as it follows the pointer.  Once you",
      "are satisfied with the clipping region, release the button.",
      "You can make adjustments to the clipping rectangle with the",
      "arrow keys.  Press an arrow key to expand the region by",
      "one pixel.  Press ALT and an arrow key to contract the",
      "clipping region.",
      "",
      "Finally, press RETURN to commit your clipping region.  To",
      "exit without cropping the image, press ESC.",
      (char *) NULL,
    };

  char
    text[MaxTextLength];

  Cursor
    cursor;

  int
    i,
    x,
    y;

  RectangleInfo
    clip_info;

  unsigned int
    mask;

  unsigned long
    state;

  Window
    xwindow;

  XEvent
    event;

  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  clip_info.x=x;
  clip_info.y=y;
  clip_info.width=0;
  clip_info.height=0;
  cursor=XCreateFontCursor(display,XC_fleur);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        if (event.xbutton.button == Button1)
          {
            /*
              Note first corner of clipping rectangle-- exit loop.
            */
            XDefineCursor(display,windows->image.id,cursor);
            clip_info.x=event.xbutton.x;
            clip_info.y=event.xbutton.y;
            state|=ExitState;
            break;
          }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            state|=EscapeState;
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XTextViewWidget(display,windows,"Help Viewer - Image Clipping",
              ImageClipHelp);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as text cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
        clip_info.x=x;
        clip_info.y=y;
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  if (state & EscapeState)
    {
      /*
        User want to exit without cropping.
      */
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      XFreeCursor(display,cursor);
      return(True);
    }
  /*
    Size rectangle as pointer moves until the mouse button is released.
  */
  state=DefaultState;
  (void) sprintf(text," %ux%u+%u+%u ",windows->image.width,
    windows->image.height,windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  x=clip_info.x;
  y=clip_info.y;
  clip_info.width=0;
  clip_info.height=0;
  XSetFunction(display,windows->image.highlight_context,GXinvert);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  do
  {
    if ((clip_info.width > 3) && (clip_info.height > 3))
      {
        /*
          Display info and draw clipping rectangle.
        */
        if (!windows->info.mapped)
          XMapWindow(display,windows->info.id);
        (void) sprintf(text," %ux%u%+d%+d",clip_info.width,clip_info.height,
          clip_info.x,clip_info.y);
        XDisplayInfoString(display,&windows->info,text);
        XHighlightRegion(display,windows->image.id,
          windows->image.highlight_context,&clip_info);
      }
    else
      if (windows->info.mapped)
        XWithdrawWindow(display,windows->info.id,windows->info.screen);
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    if ((clip_info.width > 3) && (clip_info.height > 3))
      XHighlightRegion(display,windows->image.id,
        windows->image.highlight_context,&clip_info);
    switch (event.type)
    {
      case ButtonPress:
        break;
      case ButtonRelease:
      {
        /*
          User has committed to clipping rectangle.
        */
        clip_info.x=event.xbutton.x;
        clip_info.y=event.xbutton.y;
        state|=ExitState;
        break;
      }
      case Expose:
        break;
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        clip_info.x=event.xmotion.x;
        clip_info.y=event.xmotion.y;
      }
      default:
        break;
    }
    if ((clip_info.x != x) || (clip_info.y != y))
      {
        /*
          Check boundary conditions.
        */
        if (clip_info.x < 0)
          clip_info.x=0;
        else
          if (clip_info.x > windows->image.width)
            clip_info.x=windows->image.width;
        if (clip_info.x < x)
          clip_info.width=(unsigned int) (x-clip_info.x+1);
        else
          {
            clip_info.width=(unsigned int) (clip_info.x-x+1);
            clip_info.x=x;
          }
        if (clip_info.y < 0)
          clip_info.y=0;
        else
          if (clip_info.y > windows->image.height)
            clip_info.y=windows->image.height;
        if (clip_info.y < y)
          clip_info.height=(unsigned int) (y-clip_info.y+1);
        else
          {
            clip_info.height=(unsigned int) (clip_info.y-y+1);
            clip_info.y=y;
          }
      }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XFreeCursor(display,cursor);
  state=DefaultState;
  do
  {
    if ((clip_info.width > 3) && (clip_info.height > 3))
      {
        /*
          Display info and draw clipping rectangle.
        */
        if (!windows->info.mapped)
          XMapWindow(display,windows->info.id);
        (void) sprintf(text," %ux%u%+d%+d",clip_info.width,clip_info.height,
          clip_info.x,clip_info.y);
        XDisplayInfoString(display,&windows->info,text);
        XHighlightRegion(display,windows->image.id,
          windows->image.highlight_context,&clip_info);
      }
    else
      if (windows->info.mapped)
        XWithdrawWindow(display,windows->info.id,windows->info.screen);
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    if ((clip_info.width > 3) && (clip_info.height > 3))
      XHighlightRegion(display,windows->image.id,
        windows->image.highlight_context,&clip_info);
    switch (event.type)
    {
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Exit without cropping.
            */
            state|=EscapeState;
            state|=ExitState;
            break;
          }
          case XK_Return:
          {
            /*
              Exit.
            */
            state|=ExitState;
            break;
          }
          case XK_Up:
          case XK_KP_Up:
          {
            if (event.xkey.state & Mod1Mask)
              {
                clip_info.height--;
                break;
              }
            clip_info.y--;
            clip_info.height++;
            break;
          }
          case XK_Down:
          case XK_KP_Down:
          {
            if (event.xkey.state & Mod1Mask)
              {
                clip_info.y++;
                clip_info.height--;
                break;
              }
            clip_info.height++;
            break;
          }
          case XK_Left:
          case XK_KP_Left:
          {
            if (event.xkey.state & Mod1Mask)
              {
                clip_info.width--;
                break;
              }
            clip_info.x--;
            clip_info.width++;
            break;
          }
          case XK_Right:
          case XK_KP_Right:
          {
            if (event.xkey.state & Mod1Mask)
              {
                clip_info.x++;
                clip_info.width--;
                break;
              }
            clip_info.width++;
            break;
          }
          default:
            break;
        }
        break;
      }
      case KeyRelease:
        break;
      default:
        break;
    }
    /*
      Check boundary conditions.
    */
    if (clip_info.x < 0)
      clip_info.x=0;
    else
      if (clip_info.x > windows->image.width)
        clip_info.x=windows->image.width;
    if ((clip_info.x+clip_info.width) > windows->image.width)
      clip_info.width=windows->image.width-clip_info.x;
    if (clip_info.y < 0)
      clip_info.y=0;
    else
      if (clip_info.y > windows->image.height)
        clip_info.y=windows->image.height;
    if ((clip_info.y+clip_info.height) > windows->image.height)
      clip_info.height=windows->image.height-clip_info.y;
  } while (!(state & ExitState));
  XSetFunction(display,windows->image.highlight_context,GXcopy);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (state & EscapeState)
    return(True);
  if ((clip_info.width > 3) && (clip_info.height > 3))
    if ((clip_info.width != windows->image.width) ||
        (clip_info.height != windows->image.height))
      {
        /*
          Reconfigure image window as defined by clipping rectangle.
        */
        XSetClipGeometry(display,windows,&clip_info,image);
        windows->image.window_changes.width=clip_info.width;
        windows->image.window_changes.height=clip_info.height;
        (void) XConfigureImageWindow(display,resource_info,windows,image);
      }
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o l o r E d i t I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XColorEditImageWindow allows the user to interactively change
%  the color of one pixel for a DirectColor image or one colormap entry for
%  a PseudoClass image.
%
%  The format of the XColorEditImageWindow routine is:
%
%    XColorEditImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
*/
static unsigned int XColorEditImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
  static char
    *ImageColorEditHelp[]=
    {
      "Press a button to affect this behavior:",
      "",
      "1    Press to select a pixel within an image window to",
      "     change its color.  Any pixel within an image that",
      "     matches the color of the selected pixel is recolored.",
      "",
      "2    Press and drag to select a pixel color from a pop-up",
      "     menu.  Choose from these pixel colors:",
      "",
      "         black",
      "         blue",
      "         cyan",
      "         green",
      "         gray",
      "         red",
      "         magenta",
      "         yellow",
      "         white",
      "         Browser...",
      "",
      "     Other pixel colors are specified with the color browser",
      "     or by setting the X resources pen1 through pen9.  Refer",
      "     to X RESOURCES for more details.",
      "",
      "3    Press and drag to select a delta factor from a pop-up",
      "     menu.  Choose from these delta factor:",
      "",
      "         0",
      "         2",
      "         4",
      "         8",
      "         16",
      "         32",
      "         64",
      "         Dialog...",
      "",
      "     The delta factor you choose creates a range of colors",
      "     that are considered candidates for recoloring.  The",
      "     factor is added then subtracted from each color",
      "     component of the pixel you choose with button 1.  Any",
      "     pixel within an image that falls in this range is",
      "     recolored.",
      "",
      "The actual color you request for the pixel is saved in the",
      "image.  However, the color that appears in your image window",
      "may be different.  For example, on a monochrome screen the",
      "pixel will appear black or white even if you choose red as",
      "the pixel color.  However, the image saved to a file with",
      "'Write' is written with red color.  To assure the",
      "correct color pixel in the final image, any PseudoClass",
      "image is promoted to DirectClass",
      (char *) NULL,
    };

  char
    text[MaxTextLength];

  Cursor
    cursor;

  int
    i,
    x,
    x_offset,
    y,
    y_offset;

  register RunlengthPacket
    *p;

  RunlengthPacket
    pixel;

  static int
    delta = 0;

  static unsigned int
    pen_id = 0;

  unsigned int
    height,
    mask,
    status,
    width;

  unsigned long
    state,
    x_factor,
    y_factor;

  Window
    xwindow;

  XColor
    color;

  XEvent
    event;

  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  cursor=XMakeCursor(display,windows->image.id,
    windows->image.map_info->colormap,resource_info->background_color,
    resource_info->foreground_color);
  XDefineCursor(display,windows->image.id,cursor);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        switch (event.xbutton.button)
        {
          case Button1:
          {
            /*
              Exit loop.
            */
            x_offset=event.xbutton.x;
            y_offset=event.xbutton.y;
            state|=UpdateConfigurationState;
            break;
          }
          case Button2:
          {
            char
              color_name[MaxTextLength],
              *ColorMenu[MaxNumberPens+1];

            int
              pen_number;

            /*
              Initialize menu selections.
            */
            for (i=0; i < MaxNumberPens; i++)
              ColorMenu[i]=resource_info->pen_color[i];
            ColorMenu[MaxNumberPens-1]="Browser...";
            ColorMenu[MaxNumberPens]=(char *) NULL;
            /*
              Select a pen color from the pop-up menu.
            */
            pen_number=
              XMenuWidget(display,windows,"Pixel Color",ColorMenu,color_name);
            if (pen_number == (MaxNumberPens-1))
              {
                static char
                  color_name[MaxTextLength]="gray";

                /*
                  Select a pen color from a dialog.
                */
                resource_info->pen_color[MaxNumberPens-1]=color_name;
                XColorBrowserWidget(display,windows,"Select",color_name);
                if (*color_name == '\0')
                  break;
              }
            if (pen_number >= 0)
              {
                /*
                  Set pen color.
                */
                pen_id=pen_number;
                (void) strcpy(color_name,resource_info->pen_color[pen_id]);
                status=XParseColor(display,windows->image.map_info->colormap,
                  color_name,&windows->image.pixel_info->pen_color[pen_id]);
                if (status == 0)
                  XNoticeWidget(display,windows,"Color is unknown to X server",
                    color_name);
                XBestPixel(display,windows->image.map_info->colormap,
                  (XColor *) NULL,(unsigned int)
                  Min(windows->image.visual_info->colormap_size,256),
                  &windows->image.pixel_info->pen_color[pen_id]);
              }
            XDefineCursor(display,windows->image.id,cursor);
            break;
          }
          case Button3:
          {
            char
              command[MaxTextLength];

            int
              factor;

            static char
              *DeltaMenu[]=
              {
                "0",
                "2",
                "4",
                "8",
                "16",
                "32",
                "64",
                "Dialog...",
                (char *) NULL,
              };

            /*
              Select a delta from the pop-up menu.
            */
            factor=XMenuWidget(display,windows,"Delta",DeltaMenu,command);
            if (factor >= 0)
              if (factor != 7)
                delta=atoi(command);
              else
                {
                  /*
                    Select a delta from a dialog widget.
                  */
                  (void) sprintf(command,"%d",delta);
                  XDialogWidget(display,windows,"Delta","Enter color delta:",
                    command);
                  if (*command != '\0')
                    delta=atoi(command);
                }
            break;
          }
        }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XTextViewWidget(display,windows,"Help Viewer - Image Annotation",
              ImageColorEditHelp);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
        break;
      }
      default:
        break;
    }
    if (state & UpdateConfigurationState)
      {
        int
          x,
          y;

        /*
          Pixel edit is relative to image configuration.
        */
        x=0;
        y=0;
        width=image->columns;
        height=image->rows;
        if (windows->image.clip_geometry != (char *) NULL)
          (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,
            &height);
        x_factor=UpShift(width)/windows->image.ximage->width;
        x_offset=DownShift((windows->image.x+x_offset+1)*x_factor)+x;
        y_factor=UpShift(height)/windows->image.ximage->height;
        y_offset=DownShift((windows->image.y+y_offset+1)*y_factor)+y;
        /*
          Update color of pixel.
        */
        x=0;
        p=image->pixels;
        for (i=0; i < image->packets; i++)
        {
          x+=(p->length+1);
          if (x >= (y_offset*image->columns+x_offset))
            break;
          p++;
        }
        pixel=image->pixels[i];
        color=windows->image.pixel_info->pen_color[pen_id];
        if (image->class == DirectClass)
          {
            p=image->pixels;
            for (i=0; i < image->packets; i++)
            {
              if (((int) p->red > ((int) pixel.red-delta)) &&
                  ((int) p->red < ((int) pixel.red+delta)) &&
                  ((int) p->green > ((int) pixel.green-delta)) &&
                  ((int) p->green < ((int) pixel.green+delta)) &&
                  ((int) p->blue > ((int) pixel.blue-delta)) &&
                  ((int) p->blue < ((int) pixel.blue+delta)))
                {
                  p->red=color.red >> 8;
                  p->green=color.green >> 8;
                  p->blue=color.blue >> 8;
                }
              p++;
            }
          }
        else
          {
            register unsigned short
              index;

            index=pixel.index;
            image->colormap[index].red=color.red >> 8;
            image->colormap[index].green=color.green >> 8;
            image->colormap[index].blue=color.blue >> 8;
            SyncImage(image);
          }
        /*
          Update image colormap and return to color editing.
        */
        XConfigureImageColormap(display,resource_info,windows,image);
        (void) XConfigureImageWindow(display,resource_info,windows,image);
        XDefineCursor(display,windows->image.id,cursor);
        state&=(~UpdateConfigurationState);
      }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XFreeCursor(display,cursor);
  XFlush(display);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o m p o s i t e I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XCompositeImageWindow requests an image name from the user, reads
%  the image and composites it with the X window image at a location the user
%  chooses with the pointer.
%
%  The format of the XCompositeImageWindow routine is:
%
%    status=XCompositeImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XCompositeImageWindow returns True if the image is
%      composited.  False is returned is there is a memory shortage or if the
%      image fails to be composited.
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
*/
static unsigned int XCompositeImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
  static char
    *ImageCompositeHelp[]=
    {
      "First a popup window is displayed requesting you to enter an",
      "image name. Press Composite, enter 'X:', or type a file",
      "name.  Press Cancel if you choose not to create a composite",
      "image.  When you specify X: as your file name, the filename",
      "has special meaning.  It specifies an X window by id, name,",
      "or root.  If no name is specified, the window is selected by",
      "clicking the mouse in the desired window.",
      "",
      "A small window appears showing the location of the cursor in",
      "the image window.  You are now in composite mode.  To exit",
      "immediately, press ESC.  In composite mode,  Press a button",
      "to affect this behavior:",
      "",
      "1    Press to select a location within image window to",
      "     composite your image.",
      "",
      "2    Press and drag to select a composite operation from a",
      "     pop-up menu.  Choose from these composite operations:",
      "",
      "         over",
      "         in",
      "         out",
      "         atop",
      "         xor",
      "         plus",
      "         minus",
      "         add",
      "         subtract",
      "         difference",
      "         replace",
      "",
      "How each operator behaves is described below.  image window",
      "is the image currently displayed on your X server and image",
      "is the image obtained with the File Browser widget.",
      "",
      "over     The result is the union of the two image shapes,",
      "         with image obscuring image window in the region",
      "         of overlap.",
      "",
      "in       The result is simply image cut by the shape of",
      "         image window.  None of the image data of image",
      "         window is in the result.",
      "",
      "out      The resulting image is image with the shape of",
      "         image window cut out.",
      "",
      "atop     The result is the same shape as image image window,",
      "         with image obscuring image window where the image",
      "         shapes overlap.  Note this differs from over",
      "         because the portion of image outside image window's",
      "         shape does not appear in the result.",
      "",
      "xor      The result is the image data from both image and",
      "         image window that is outside the overlap region.",
      "         The overlap region is blank.",
      "",
      "plus     The result is just the sum of the image data.",
      "         Output values are clipped to 255 (no overflow).",
      "         This operation is independent of the matte",
      "         channels.",
      "",
      "minus    The result of image - image window, with underflow",
      "         clipped to zero.  The matte channel is ignored (set",
      "         to 255, full coverage).",
      "",
      "add      The result of image + image window, with overflow",
      "         wrapping around (mod 256).",
      "",
      "subtract The result of image - image window, with underflow",
      "         wrapping around (mod 256).  The add and subtract",
      "         operators can be used to perform reversible",
      "         transformations.",
      "",
      "difference",
      "         The result of abs(image - image window).  This is",
      "         useful for comparing two very similar images.",
      "",
      "replace  The resulting image is image window replaced with",
      "         image.  Here the matte information is ignored.",
      "",
      "The image compositor requires a matte, or alpha channel in",
      "the image for some operations.  This extra channel usually",
      "defines a mask which represents a sort of a cookie-cutter",
      "for the image.  This is the case when matte is 255 (full",
      "coverage) for pixels inside the shape, zero outside, and",
      "between zero and 255 on the boundary.  If image does not",
      "have a matte channel, it is initialized with 0 for any",
      "pixel matching in color to pixel location (0,0), otherwise",
      "255.",
      "",
      "Note that matte information for image window is not retained",
      "for colormapped X server visuals (e.g. StaticColor,",
      "StaticColor, GrayScale, PseudoColor).  Correct compositing",
      "behavior may require a TrueColor or DirectColor visual or a",
      "Standard Colormap.",
      "",
      "Choosing a composite operator is optional.  The default",
      "operator is over.  However, you must choose a location to",
      "composite your image and press button 1.  Press and hold",
      "button 1 before releasing and an outline of the image will",
      "appear to help you identify your location.",
      "",
      "The actual colors of the composite image is saved.  However,",
      "the color that appears in image window may be different.",
      "For example, on a monochrome screen image window will appear",
      "black or white even though your composited image may have",
      "many colors.  If the image is saved to a file it is written",
      "with the correct colors.  To assure the correct colors are",
      "saved in the final image, any PseudoClass image is promoted",
      "to DirectClass.",
      (char *) NULL,
    };

  char
    filename[MaxTextLength],
    text[MaxTextLength];

  Cursor
    cursor;

  Image
    *composite_image;

  int
    i,
    x,
    y;

  RectangleInfo
    composite_info;

  static unsigned int
    operator = OverCompositeOp;

  unsigned int
    height,
    mask,
    width;

  unsigned long
    scale_factor,
    state;

  Window
    xwindow;

  XEvent
    event;

  /*
    Request image file name from user.
  */
  (void) strcpy(filename,"x:");
  XFileBrowserWidget(display,windows,"Composite",filename);
  if (*filename == '\0')
    return(True);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XCheckRefreshWindow(display,&windows->image);
  /*
    Read image.
  */
  (void) strcpy(resource_info->image_info->filename,filename);
  composite_image=ReadImage(resource_info->image_info);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  if (composite_image == (Image *) NULL)
    {
      XNoticeWidget(display,windows,"Unable to read image",filename);
      return(False);
    }
  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  composite_info.x=x;
  composite_info.y=y;
  composite_info.width=0;
  composite_info.height=0;
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  cursor=XCreateFontCursor(display,XC_ul_angle);
  XSetFunction(display,windows->image.highlight_context,GXinvert);
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    XHighlightRegion(display,windows->image.id,windows->image.highlight_context,
      &composite_info);
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    XHighlightRegion(display,windows->image.id,windows->image.highlight_context,
      &composite_info);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Button Press: 0x%lx %u +%d+%d\n",
            event.xbutton.window,event.xbutton.button,event.xbutton.x,
            event.xbutton.y);
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        switch (event.xbutton.button)
        {
          case Button1:
          {
            composite_info.width=composite_image->columns;
            composite_info.height=composite_image->rows;
            XDefineCursor(display,windows->image.id,cursor);
            break;
          }
          case Button2:
          {
            char
              command[MaxTextLength];

            static char
              *CompositeMenu[]=
              {
                "over",
                "in",
                "out",
                "atop",
                "xor",
                "plus",
                "minus",
                "add",
                "subtract",
                "difference",
                "replace",
                (char *) NULL,
              };

            /*
              Select a command from the pop-up menu.
            */
            operator=
              XMenuWidget(display,windows,"Operations",CompositeMenu,command)+1;
            break;
          }
          default:
            break;
        }
        break;
      }
      case ButtonRelease:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Button Release: 0x%lx %u +%d+%d\n",
            event.xbutton.window,event.xbutton.button,event.xbutton.x,
            event.xbutton.y);
        if (event.xbutton.button == Button1)
          if ((composite_info.width != 0) && (composite_info.height != 0))
            {
              /*
                User has selected the location of the composite image.
              */
              composite_info.x=event.xbutton.x;
              composite_info.y=event.xbutton.y;
              state|=ExitState;
            }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        static int
          length;

        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (resource_info->debug)
          (void) fprintf(stderr,"Key press: 0x%lx (%s)\n",key_symbol,command);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            DestroyImage(composite_image);
            state|=EscapeState;
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XSetFunction(display,windows->image.highlight_context,GXcopy);
            XTextViewWidget(display,windows,"Help Viewer - Image Compositing",
              ImageCompositeHelp);
            XSetFunction(display,windows->image.highlight_context,GXinvert);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as text cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
        composite_info.x=x;
        composite_info.y=y;
        break;
      }
      default:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Event type: %d\n",event.type);
        break;
      }
    }
  } while (!(state & ExitState));
  XSetFunction(display,windows->image.highlight_context,GXcopy);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XFreeCursor(display,cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XFlush(display);
  if (state & EscapeState)
    return(True);
  /*
    Image compositing is relative to image configuration.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  x=0;
  y=0;
  width=(*image)->columns;
  height=(*image)->rows;
  if (windows->image.clip_geometry != (char *) NULL)
    (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
  scale_factor=UpShift(width)/windows->image.ximage->width;
  composite_info.x+=windows->image.x;
  composite_info.x=DownShift(composite_info.x*scale_factor);
  composite_info.width=DownShift(composite_info.width*scale_factor);
  scale_factor=UpShift(height)/windows->image.ximage->height;
  composite_info.y+=windows->image.y;
  composite_info.y=DownShift(composite_info.y*scale_factor);
  composite_info.height=DownShift(composite_info.height*scale_factor);
  if ((composite_info.width != composite_image->columns) ||
      (composite_info.height != composite_image->rows))
    {
      Image
        *scaled_image;

      /*
        Scale composite image.
      */
      scaled_image=
        ScaleImage(composite_image,composite_info.width,composite_info.height);
      DestroyImage(composite_image);
      if (scaled_image == (Image *) NULL)
        {
          XDefineCursor(display,windows->image.id,windows->image.cursor);
          return(False);
        }
      composite_image=scaled_image;
    }
  /*
    Composite image with X image window.
  */
  CompositeImage(*image,operator,composite_image,composite_info.x+x,
    composite_info.y+y);
  DestroyImage(composite_image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  /*
    Update image colormap.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o n f i g u r e I m a g e C o l o r m a p                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XConfigureImageColormap creates a new X colormap.
%
%  The format of the XConfigureImageColormap routine is:
%
%    XConfigureImageColormap(display,resource_info,windows,image)
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
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static void XConfigureImageColormap(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define ConfigureColormapText  "  Configuring colormap...  "

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,ConfigureColormapText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,ConfigureColormapText);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  /*
    Make standard colormap.
  */
  XMakeStandardColormap(display,windows->image.visual_info,resource_info,
    image,windows->image.map_info,windows->image.pixel_info);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XCheckRefreshWindow(display,&windows->image);
  XClientMessage(display,windows->image.id,windows->im_protocols,
    windows->im_window_colormap,CurrentTime);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o n f i g u r e I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XConfigureImageWindow creates a new X image.  It also notifies the
%  window manager of the new image size and configures the transient widows.
%
%  The format of the XConfigureImageWindow routine is:
%
%    status=XConfigureImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XConfigureImageWindow returns True if the window is
%      resized.  False is returned is there is a memory shortage or if the
%      window fails to resize.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XConfigureImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define ConfigureImageText  "  Configuring image...  "
#define MaxPanSize  96

  unsigned int
    height,
    mask,
    status,
    width,
    x_factor,
    y_factor;

  XWindowChanges
    window_changes;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,ConfigureImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,ConfigureImageText);
  /*
    Resize image to fit image window dimensions.
  */
  width=windows->image.window_changes.width;
  height=windows->image.window_changes.height;
  if (resource_info->debug)
    (void) fprintf(stderr,"Configure Image: %dx%d=>%ux%u\n",
      windows->image.ximage->width,windows->image.ximage->height,width,height);
  if ((width*height) == 0)
    return(True);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  x_factor=UpShift(width)/windows->image.ximage->width;
  y_factor=UpShift(height)/windows->image.ximage->height;
  status=XMakeImage(display,resource_info,&windows->image,image,width,height);
  if (resource_info->use_pixmap)
    (void) XMakePixmap(display,resource_info,&windows->image);
  if (status == False)
    XNoticeWidget(display,windows,"Unable to configure X image",
      windows->image.name);
  /*
    Notify window manager of the new configuration.
  */
  if (width > XDisplayWidth(display,windows->image.screen))
    width=XDisplayWidth(display,windows->image.screen);
  window_changes.width=width;
  if (height > XDisplayHeight(display,windows->image.screen))
    height=XDisplayHeight(display,windows->image.screen);
  window_changes.height=height;
  mask=CWWidth | CWHeight;
  if (resource_info->backdrop)
    {
      mask|=CWX | CWY;
      window_changes.x=
        (XDisplayWidth(display,windows->image.screen) >> 1)-(width >> 1);
      window_changes.y=
        (XDisplayHeight(display,windows->image.screen) >> 1)-(height >> 1);
    }
  if ((x_factor < UpShift(1)) && (y_factor < UpShift(1)))
    XSetWindowBackgroundPixmap(display,windows->image.id,None);
  XReconfigureWMWindow(display,windows->image.id,windows->image.screen,mask,
    &window_changes);
  if ((x_factor == UpShift(1)) && (y_factor == UpShift(1)))
    XRefreshWindow(display,&windows->image,(XEvent *) NULL);
  else
    {
      unsigned int
        scale_factor;

      /*
        Update transient configuration.
      */
      scale_factor=UpShift(MaxPanSize)/windows->image.ximage->width;
      if (scale_factor > (UpShift(MaxPanSize)/windows->image.ximage->height))
        scale_factor=UpShift(MaxPanSize)/windows->image.ximage->height;
      window_changes.width=DownShift(windows->image.ximage->width*scale_factor);
      window_changes.height=
        DownShift(windows->image.ximage->height*scale_factor);
      XReconfigureWMWindow(display,windows->pan.id,windows->pan.screen,CWWidth |
        CWHeight,&window_changes);
      XBestIconSize(display,&windows->icon,image);
      window_changes.width=windows->icon.width;
      window_changes.height=windows->icon.height;
      XReconfigureWMWindow(display,windows->icon.id,windows->icon.screen,
        CWWidth | CWHeight,&window_changes);
    }
  XSetWindowBackground(display,windows->image.id,
    windows->image.pixel_info->background_color.pixel);
  windows->magnify.x=DownShift(x_factor*windows->magnify.x);
  windows->magnify.y=DownShift(y_factor*windows->magnify.y);
  if (windows->magnify.mapped)
    XMakeMagnifyImage(display,windows);
  windows->image.x=DownShift(x_factor*windows->image.x);
  windows->image.y=DownShift(y_factor*windows->image.y);
  windows->pan.clip_geometry=windows->image.clip_geometry;
  if (windows->pan.mapped)
    XMakePanImage(display,resource_info,windows,image);
  windows->icon.clip_geometry=windows->image.clip_geometry;
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C u t I m a g e W i n d o w                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XCutImageWindow cuts the X image.
%
%  The format of the XCutImageWindow routine is:
%
%    status=XCutImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XCutImageWindow return True if the image is
%      cut.  False is returned is there is a memory shortage or if the
%      image fails to cut.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XCutImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define CutImageText  "  Cutting image...  "
#define HorizonalCutOp  0
#define VerticalCutOp  1

  static char
    *ImageCutHelp[]=
    {
      "Press a button to affect this behavior:",
      "",
      "1    Select a location within an image window to begin your",
      "     cut, press and hold.  Next, move the pointer to another",
      "     location in the image.  As you move a line will connect",
      "     the initial location and the pointer.  When you release",
      "     the button, the area within an image to cut is",
      "     determined by which cut direction you choose with",
      "     button 2.",
      "",
      "2    Press and drag to select a cut operator from a pop-up",
      "     menu.  Choose from these cut operators:",
      "",
      "         horizontal",
      "         vertical",
      "",
      "If the operator is horizontal, the area of the image between",
      "the two horizontal endpoints of the cut line is removed.",
      "Otherwise, the area of the image between the two vertical",
      "endpoints of the cut line is removed.",
      "",
      "To cancel the image cutting, move the pointer back to the",
      "starting point of the line and release the button.",
      (char *) NULL,
    };

  char
    text[MaxTextLength];

  Image
    *cut_image;

  int
    i,
    x,
    y;

  RectangleInfo
    cut_info;

  static unsigned int
    operator = HorizonalCutOp;

  unsigned int
    distance,
    height,
    mask,
    width;

  unsigned long
    scale_factor,
    state;

  Window
    xwindow;

  XEvent
    event;

  XSegment
    segment_info;

  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        switch (event.xbutton.button)
        {
          case Button1:
          {
            /*
              User has committed to start point of cutting line.
            */
            segment_info.x1=event.xbutton.x;
            segment_info.x2=event.xbutton.x;
            segment_info.y1=event.xbutton.y;
            segment_info.y2=event.xbutton.y;
            state|=ExitState;
            break;
          }
          case Button2:
          {
            char
              command[MaxTextLength];

            static char
              *CutMenu[]=
              {
                "horizonal",
                "vertical",
                (char *) NULL,
              };

            /*
              Select a command from the pop-up menu.
            */
            operator=
              XMenuWidget(display,windows,"Operations",CutMenu,command)+1;
            break;
          }
        }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,
          sizeof(command),&key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            state|=EscapeState;
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XSetFunction(display,windows->image.highlight_context,GXcopy);
            XTextViewWidget(display,windows,"Help Viewer - Image Cutting",
              ImageCutHelp);
            XSetFunction(display,windows->image.highlight_context,GXinvert);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as text cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
      }
    }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (state & EscapeState)
    return(True);
  /*
    Draw line as pointer moves until the mouse button is released.
  */
  distance=0;
  (void) sprintf(text," %ux%u+%u+%u ",windows->image.width,
    windows->image.height,windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XSetFunction(display,windows->image.highlight_context,GXinvert);
  state=DefaultState;
  do
  {
    /*
      Compute cutting geometry.
    */
    if (operator == HorizonalCutOp)
      {
        cut_info.width=segment_info.x2-segment_info.x1+1;
        cut_info.x=segment_info.x1;
        cut_info.height=0;
        cut_info.y=0;
        if (segment_info.x1 > segment_info.x2)
          {
            cut_info.width=segment_info.x1-segment_info.x2+1;
            cut_info.x=segment_info.x2;
          }
      }
    else
      {
        cut_info.width=0;
        cut_info.height=segment_info.y2-segment_info.y1+1;
        cut_info.x=0;
        cut_info.y=segment_info.y1;
        if (segment_info.y1 > segment_info.y2)
          {
            cut_info.height=segment_info.y1-segment_info.y2+1;
            cut_info.y=segment_info.y2;
          }
      }
    if (distance > 9)
      {
        /*
          Display info and draw cutting line.
        */
        if (!windows->info.mapped)
          XMapWindow(display,windows->info.id);
        (void) sprintf(text," %ux%u%+d%+d",cut_info.width,cut_info.height,
          cut_info.x,cut_info.y);
        XDisplayInfoString(display,&windows->info,text);
        XHighlightLine(display,windows->image.id,
          windows->image.highlight_context,&segment_info);
      }
    else
      if (windows->info.mapped)
        XWithdrawWindow(display,windows->info.id,windows->info.screen);
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    if (distance > 9)
      XHighlightLine(display,windows->image.id,
        windows->image.highlight_context,&segment_info);
    switch (event.type)
    {
      case ButtonPress:
        break;
      case ButtonRelease:
      {
        /*
          User has committed to cutting line.
        */
        segment_info.x2=event.xbutton.x;
        segment_info.y2=event.xbutton.y;
        state|=ExitState;
        break;
      }
      case Expose:
        break;
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        segment_info.x2=event.xmotion.x;
        segment_info.y2=event.xmotion.y;
      }
      default:
        break;
    }
    /*
      Check boundary conditions.
    */
    if (segment_info.x2 < 0)
      segment_info.x2=0;
    else
      if (segment_info.x2 > windows->image.width)
        segment_info.x2=windows->image.width;
    if (segment_info.y2 < 0)
      segment_info.y2=0;
    else
      if (segment_info.y2 > windows->image.height)
        segment_info.y2=windows->image.height;
    /*
      Compute distance.
    */
    distance=
      ((segment_info.x2-segment_info.x1)*(segment_info.x2-segment_info.x1))+
      ((segment_info.y2-segment_info.y1)*(segment_info.y2-segment_info.y1));
  } while (!(state & ExitState));
  XSetFunction(display,windows->image.highlight_context,GXcopy);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (distance <= 9)
    return(True);
  /*
    Map info window.
  */
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,CutImageText);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  /*
    Image cutting is relative to image configuration.
  */
  if ((windows->image.clip_geometry != (char *) NULL) ||
      ((*image)->columns != windows->image.ximage->width) ||
      ((*image)->rows != windows->image.ximage->height))
    {
      char
        image_geometry[MaxTextLength];

      /*
        Update image with user transforms.
      */
      (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
        windows->image.ximage->height);
      TransformImage(image,windows->image.clip_geometry,image_geometry);
      if (windows->image.clip_geometry != (char *) NULL)
        {
          (void) free((char *) windows->image.clip_geometry);
          windows->image.clip_geometry=(char *) NULL;
        }
      windows->image.x=0;
      windows->image.y=0;
    }
  windows->image.window_changes.width=
    windows->image.ximage->width-cut_info.width;
  windows->image.window_changes.height=
    windows->image.ximage->height-cut_info.height;
  x=0;
  y=0;
  width=(*image)->columns;
  height=(*image)->rows;
  if (windows->image.clip_geometry != (char *) NULL)
    (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
  scale_factor=UpShift(width)/windows->image.ximage->width;
  cut_info.x+=windows->image.x;
  cut_info.x=DownShift(cut_info.x*scale_factor);
  cut_info.width=DownShift(cut_info.width*scale_factor);
  scale_factor=UpShift(height)/windows->image.ximage->height;
  cut_info.y+=windows->image.y;
  cut_info.y=DownShift(cut_info.y*scale_factor);
  cut_info.height=DownShift(cut_info.height*scale_factor);
  /*
    Cut image.
  */
  cut_image=CutImage(*image,&cut_info);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (cut_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=cut_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D e s p e c k l e I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDespeckleImageWindow reduces the speckles within an image.
%
%  The format of the XDespeckleImageWindow routine is:
%
%    status=XDespeckleImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XDespeckleImageWindow return True if the edges within
%      the image are detected.  False is returned is there is a memory
%      shortage or if the edges cannot be detected.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XDespeckleImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define DespeckleImageText  "  Despeckling image...  "

  Image
    *despeckled_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,DespeckleImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,DespeckleImageText);
  /*
    Despeckle image.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  despeckled_image=DespeckleImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (despeckled_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=despeckled_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D i s p l a y B a c k g r o u n d I m a g e                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDisplayBackgroundImage displays an image in the background of a
%  window.
%
%  The format of the XDisplayBackgroundImage routine is:
%
%      XDisplayBackgroundImage(display,resource_info,image)
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
static void XDisplayBackgroundImage(display,resource_info,image)
Display
  *display;

XResourceInfo
  *resource_info;

Image
  *image;
{
  char
    visual_type[MaxTextLength];

  unsigned int
    height,
    status,
    width;

  Window
    root_window;

  XGCValues
    context_values;

  XPixelInfo
    pixel_info;

  XResourceInfo
    resources;

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
  window_info.matte_image=(XImage *) NULL;
  window_info.pixmap=(Pixmap) NULL;
  window_info.matte_pixmap=(Pixmap) NULL;
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
  resources=(*resource_info);
  resources.map_type=(char *) NULL;
  resources.visual_type=visual_type;
  visual_info=XBestVisualInfo(display,map_info,&resources);
  if (visual_info == (XVisualInfo *) NULL)
    Error("Unable to get visual",resource_info->visual_type);
  /*
    Free previous root colors.
  */
  if (window_info.id == root_window)
    XDestroyWindowColors(display,root_window);
  /*
    Initialize colormap.
  */
  resources.colormap=SharedColormap;
  XMakeStandardColormap(display,visual_info,&resources,image,map_info,
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
  /*
    Initialize image window attributes.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,(XFontStruct *) NULL,
    resource_info,&window_info);
  /*
    Create the X image.
  */
  window_info.width=image->columns;
  if (window_info.width >= window_attributes.width)
    window_info.width=window_attributes.width;
  window_info.height=image->rows;
  if (window_info.height >= window_attributes.height)
    window_info.height=window_attributes.height;
  status=XMakeImage(display,resource_info,&window_info,image,window_info.width,
    window_info.height);
  if (status == False)
    Error("Unable to create X image",(char *) NULL);
  /*
    Adjust image dimensions as specified by backdrop or geometry options.
  */
  width=window_info.width;
  height=window_info.height;
  if (resource_info->backdrop)
    {
      /*
        Center image on root window.
      */
      window_info.x=(window_attributes.width >> 1)-
        (window_info.ximage->width >> 1);
      window_info.y=(window_attributes.height >> 1)-
        (window_info.ximage->height >> 1);
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
  /*
    Display pixmap on the window.
  */
  if ((width > window_info.width) || (height > window_info.height))
    XFillRectangle(display,window_info.pixmap,window_info.annotate_context,
      0,0,width,height);
  XPutImage(display,window_info.pixmap,window_info.annotate_context,
    window_info.ximage,0,0,window_info.x,window_info.y,window_info.width,
    window_info.height);
  XSetWindowBackgroundPixmap(display,window_info.id,window_info.pixmap);
  XClearWindow(display,window_info.id);
  if (window_info.id == root_window)
    XRetainWindowColors(display,root_window);
  /*
    Free resources.
  */
  XFreePixmap(display,window_info.pixmap);
  XDestroyImage(window_info.ximage);
  XFreeGC(display,window_info.annotate_context);
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
%   X D i s p l a y I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDisplayImage displays an image via X11.  A new image is created
%  and returned if the user interactively transforms the displayed image.
%
%  The format of the XDisplayImage routine is:
%
%      loaded_image=XDisplayImage(display,resource_info,argv,argc,image,state)
%
%  A description of each parameter follows:
%
%    o loaded_image:  Function XDisplayImage returns an image when the
%      user chooses 'Load Image' from the command menu or picks a tile
%      from the image directory.  Otherwise a null image is returned.
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
%    o image: Specifies an address to an address of an Image structure;
%      returned from ReadImage.
%
%
*/
static Image *XDisplayImage(display,resource_info,argv,argc,image,state)
Display
  *display;

XResourceInfo
  *resource_info;

char
  **argv;

int
  argc;

Image
  **image;

unsigned long
  *state;
{
#define MagnifySize  256  /* must be a power of 2 */
#define MaxWindows  10

  static char
    *ImagePanHelp[]=
    {
      "When an image exceeds the width or height of the X server",
      "screen, display maps a small panning window.  The rectangle",
      "within an panning window shows the area that is currently",
      "displayed in the the image window.  To 'pan' about the",
      "image, press and drag the mouse within an panning window.",
      "The panning rectangle moves with the mouse and the image",
      "window is updated to reflect the location of the rectangle",
      "within an panning window.  When you have selected the area",
      "of the image you wish to view, just release the mouse",
      "button.",
      "",
      "Use the arrow keys to pan the image one pixel up, down,",
      "left, or right within an image window.",
      "",
      "The panning window goes away if the image becomes smaller",
      "than the dimensions of the X server screen.",
      "",
      "If you force the panning window to withdraw, the image is",
      "restored to its original size.",
      (char *) NULL,
    };

  static unsigned char
    HighlightBitmap[] = {0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55},
    ShadowBitmap[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  char
    command[MaxTextLength];

  Image
    *displayed_image,
    *loaded_image;

  int
    status;

  KeySym
    key_symbol;

  register int
    i;

  static Window
    root_window;

  static XClassHint
    *class_hint;

  static XImage
    former_ximage;

  static XPixelInfo
    icon_pixel,
    pixel_info;

  static XPoint
    montage_info;

  static XResourceInfo
    icon_resources;

  static XStandardColormap
    *icon_map,
    *map_info;

  static XVisualInfo
    *icon_visual,
    *visual_info = (XVisualInfo *) NULL;

  static XWindowInfo
    *magick_windows[MaxWindows];

  static XWindows
    *windows;

  static XWMHints
    *manager_hints;

  static unsigned int
    number_windows;

  struct stat
    file_info;

  time_t
    timer,
    time_stamp,
    update_time;

  unsigned int
    scale_factor;

  XEvent
    event;

  XFontStruct
    *font_info;

  XGCValues
    context_values;

  XWindowChanges
    window_changes;

  if (visual_info == (XVisualInfo *) NULL)
    {
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
        Allocate visual.
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
            visual_info->red_mask,visual_info->green_mask,
            visual_info->blue_mask);
          (void) fprintf(stderr,"  significant bits in color: %d bits\n",
            visual_info->bits_per_rgb);
        }
      /*
        Allocate atoms.
      */
      windows=(XWindows *) malloc(sizeof(XWindows));
      if (windows == (XWindows *) NULL)
        Error("Unable to create X windows","Memory allocation failed");
      windows->wm_protocols=XInternAtom(display,"WM_PROTOCOLS",False);
      windows->wm_delete_window=XInternAtom(display,"WM_DELETE_WINDOW",False);
      windows->wm_take_focus=XInternAtom(display,"WM_TAKE_FOCUS",False);
      windows->im_protocols=XInternAtom(display,"IM_PROTOCOLS",False);
      windows->im_window_colormap=
        XInternAtom(display,"IM_UPDATE_COLORMAP",False);
      windows->im_former_image=XInternAtom(display,"IM_FORMER_IMAGE",False);
      windows->im_next_image=XInternAtom(display,"IM_NEXT_IMAGE",False);
      windows->im_exit=XInternAtom(display,"IM_EXIT",False);
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
      magick_windows[number_windows++]=(&windows->magnify);
      magick_windows[number_windows++]=(&windows->pan);
      magick_windows[number_windows++]=(&windows->command);
      magick_windows[number_windows++]=(&windows->popup);
      for (i=0; i < number_windows; i++)
        magick_windows[i]->id=(Window) NULL;
      montage_info.x=0;
      montage_info.y=0;
    }
  /*
    Initialize Standard Colormap.
  */
  loaded_image=(Image *) NULL;
  displayed_image=(*image);
  if (resource_info->debug)
    {
      (void) fprintf(stderr,"Image: [%u] %s %ux%u ",displayed_image->scene,
        displayed_image->filename,displayed_image->columns,
        displayed_image->rows);
      if (displayed_image->colors != 0)
        (void) fprintf(stderr,"%uc ",displayed_image->colors);
      (void) fprintf(stderr,"%s\n",displayed_image->magick);
    }
  XMakeStandardColormap(display,visual_info,resource_info,displayed_image,
    map_info,&pixel_info);
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
  context_values.plane_mask=
    context_values.background ^ context_values.foreground;
  pixel_info.highlight_context=XCreateGC(display,windows->context.id,
    GCBackground | GCFont | GCForeground | GCGraphicsExposures | GCPlaneMask,
    &context_values);
  if (pixel_info.highlight_context == (GC) NULL)
    Error("Unable to create graphic context",(char *) NULL);
  XDestroyWindow(display,windows->context.id);
  /*
    Initialize icon window.
  */
  XGetWindowInfo(display,icon_visual,icon_map,&icon_pixel,(XFontStruct *) NULL,
    &icon_resources,&windows->icon);
  XBestIconSize(display,&windows->icon,displayed_image);
  windows->icon.attributes.colormap=
    XDefaultColormap(display,icon_visual->screen);
  windows->icon.attributes.event_mask=ExposureMask | StructureNotifyMask;
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
  if ((resource_info->title != (char *) NULL) && !(*state & MontageImageState))
    {
      /*
        User specified window name.
      */
      (void) strcpy(windows->image.name,resource_info->title);
      (void) strcpy(windows->image.icon_name,resource_info->title);
    }
  else
    {
      char
        *p;

      /*
        Window name is the base of the filename.
      */
      p=displayed_image->filename+strlen(displayed_image->filename)-1;
      while ((p > displayed_image->filename) && (*(p-1) != '/'))
        p--;
      if ((displayed_image->previous == (Image *) NULL) &&
          (displayed_image->next == (Image *) NULL))
        (void) sprintf(windows->image.name,"ImageMagick: %s",p);
      else
        (void) sprintf(windows->image.name,"ImageMagick: %s.%u",p,
          displayed_image->scene);
      (void) strcpy(windows->image.icon_name,p);
    }
  windows->image.geometry=resource_info->image_geometry;
  windows->image.width=displayed_image->columns;
  if (windows->image.width > XDisplayWidth(display,visual_info->screen))
    windows->image.width=XDisplayWidth(display,visual_info->screen);
  windows->image.height=displayed_image->rows;
  if (windows->image.height > XDisplayHeight(display,visual_info->screen))
    windows->image.height=XDisplayHeight(display,visual_info->screen);
  windows->image.attributes.event_mask=ButtonMotionMask | ButtonPressMask |
    ButtonReleaseMask | EnterWindowMask | ExposureMask | KeyPressMask |
    KeyReleaseMask | LeaveWindowMask | OwnerGrabButtonMask |
    StructureNotifyMask;
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->backdrop);
  if (resource_info->backdrop || (windows->backdrop.id != (Window) NULL))
    {
      /*
        Initialize backdrop window.
      */
      windows->backdrop.name="ImageMagick Backdrop";
      windows->backdrop.flags=USSize | USPosition;
      windows->backdrop.width=XDisplayWidth(display,visual_info->screen);
      windows->backdrop.height=XDisplayHeight(display,visual_info->screen);
      windows->backdrop.border_width=0;
      windows->backdrop.immutable=True;
      windows->backdrop.attributes.do_not_propagate_mask=ButtonPressMask |
        ButtonReleaseMask;
      windows->backdrop.attributes.event_mask=ButtonPressMask | KeyPressMask |
        OwnerGrabButtonMask;
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
      XMapWindow(display,windows->backdrop.id);
      XClearWindow(display,windows->backdrop.id);
      if (windows->image.id != (Window) NULL)
        {
          XDestroyWindow(display,windows->image.id);
          windows->image.id=(Window) NULL;
        }
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
  XMakeWindow(display,
    (Window) (resource_info->backdrop ? windows->backdrop.id : root_window),
    argv,argc,class_hint,manager_hints,&windows->image);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (image)\n",windows->image.id);
  /*
    Initialize X image structure.
  */
  windows->image.x=0;
  windows->image.y=0;
  status=XMakeImage(display,resource_info,&windows->image,displayed_image,
    displayed_image->columns,displayed_image->rows);
  if (status == False)
    Error("Unable to create X image",(char *) NULL);
  if (resource_info->use_pixmap)
    (void) XMakePixmap(display,resource_info,&windows->image);
  XMapWindow(display,windows->image.id);
  if (windows->image.mapped)
    if ((windows->image.ximage->width == former_ximage.width) &&
        (windows->image.ximage->height == former_ximage.height))
      XRefreshWindow(display,&windows->image,(XEvent *) NULL);
  /*
    Initialize info window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->info);
  windows->info.name="ImageMagick Info";
  windows->info.x=2;
  windows->info.y=2;
  windows->info.flags|=PPosition;
  windows->info.attributes.save_under=True;
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
    Initialize magnify window and cursor.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->magnify);
  windows->magnify.name=(char *) malloc(MaxTextLength*sizeof(char));
  if (windows->magnify.name == NULL)
    Error("Unable to create magnify window","Memory allocation failed");
  (void) sprintf(windows->magnify.name,"Magnify %uX",resource_info->magnify);
  windows->magnify.cursor=XMakeCursor(display,windows->image.id,
    map_info->colormap,resource_info->background_color,
    resource_info->foreground_color);
  if (windows->magnify.cursor == (Cursor) NULL)
    Error("Unable to create cursor",(char *) NULL);
  windows->magnify.width=MagnifySize;
  windows->magnify.height=MagnifySize;
  windows->magnify.x=XDisplayWidth(display,windows->magnify.screen) >> 1;
  windows->magnify.y=XDisplayHeight(display,windows->magnify.screen) >> 1;
  windows->magnify.min_width=MagnifySize;
  windows->magnify.min_height=MagnifySize;
  windows->magnify.width_inc=MagnifySize;
  windows->magnify.height_inc=MagnifySize;
  windows->magnify.data=resource_info->magnify;
  windows->magnify.attributes.save_under=True;
  windows->magnify.attributes.cursor=windows->magnify.cursor;
  windows->magnify.attributes.event_mask=ButtonPressMask | ButtonReleaseMask |
    ExposureMask | KeyPressMask | KeyReleaseMask | OwnerGrabButtonMask |
    StructureNotifyMask;
  class_hint->res_name="magnify";
  manager_hints->flags=InputHint | StateHint | WindowGroupHint;
  manager_hints->input=True;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->magnify);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (magnify)\n",windows->magnify.id);
  XSetTransientForHint(display,windows->magnify.id,windows->image.id);
  /*
    Initialize panning window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->pan);
  windows->pan.name="Pan Icon";
  scale_factor=UpShift(MaxPanSize)/displayed_image->columns;
  if (scale_factor > (UpShift(MaxPanSize)/displayed_image->rows))
    scale_factor=UpShift(MaxPanSize)/displayed_image->rows;
  windows->pan.width=DownShift(displayed_image->columns*scale_factor);
  windows->pan.height=DownShift(displayed_image->rows*scale_factor);
  windows->pan.x=XDisplayWidth(display,windows->pan.screen) >> 1;
  windows->pan.y=XDisplayHeight(display,windows->pan.screen) >> 1;
  windows->pan.immutable=True;
  windows->pan.attributes.save_under=True;
  windows->pan.attributes.event_mask=ButtonMotionMask | ButtonPressMask |
    ButtonReleaseMask | ExposureMask | OwnerGrabButtonMask | KeyPressMask |
    KeyReleaseMask | StructureNotifyMask;
  class_hint->res_name="pan";
  manager_hints->flags=InputHint | StateHint | WindowGroupHint;
  manager_hints->input=True;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->pan);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (pan)\n",windows->pan.id);
  XSetTransientForHint(display,windows->pan.id,windows->image.id);
  if (windows->image.mapped)
    if ((windows->image.width < windows->image.ximage->width) ||
        (windows->image.height < windows->image.ximage->height))
      XMapRaised(display,windows->pan.id);
  /*
    Initialize command window.
  */
  XGetWindowInfo(display,visual_info,map_info,&pixel_info,font_info,
    resource_info,&windows->command);
  windows->command.border_width=0;
  windows->command.flags|=PPosition;
  windows->command.attributes.backing_store=WhenMapped;
  windows->command.attributes.save_under=True;
  windows->command.attributes.event_mask=ButtonMotionMask | ButtonPressMask |
    ButtonReleaseMask | EnterWindowMask | ExposureMask | KeyPressMask |
    LeaveWindowMask | StructureNotifyMask;
  windows->command.attributes.override_redirect=True;
  class_hint->res_name="command";
  manager_hints->flags=InputHint | StateHint | WindowGroupHint;
  manager_hints->input=False;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->command);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (command)\n",windows->command.id);
  windows->command.highlight_stipple=XCreateBitmapFromData(display,
    windows->command.id,(char *) HighlightBitmap,8,8);
  windows->command.shadow_stipple=XCreateBitmapFromData(display,
    windows->command.id,(char *) ShadowBitmap,8,8);
  XSetTransientForHint(display,windows->command.id,windows->image.id);
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
  manager_hints->input=True;
  manager_hints->initial_state=NormalState;
  manager_hints->window_group=windows->image.id;
  XMakeWindow(display,root_window,argv,argc,class_hint,manager_hints,
    &windows->popup);
  if (resource_info->debug)
    (void) fprintf(stderr,"Window id: 0x%lx (pop up)\n",windows->popup.id);
  windows->popup.highlight_stipple=XCreateBitmapFromData(display,
    windows->popup.id,(char *) HighlightBitmap,8,8);
  windows->popup.shadow_stipple=XCreateBitmapFromData(display,
    windows->popup.id,(char *) ShadowBitmap,8,8);
  XSetTransientForHint(display,windows->popup.id,windows->image.id);
  /*
    Respond to events.
  */
  timer=time((time_t *) NULL)+resource_info->delay;
  update_time=0;
  if (resource_info->update)
    {
      /*
        Determine when file data was last modified.
      */
      status=stat(displayed_image->filename,&file_info);
      if (status == 0)
        update_time=file_info.st_mtime;
    }
  *state&=(~FormerImageState);
  *state&=(~MontageImageState);
  *state&=(~NextImageState);
  do
  {
    /*
      Handle a window event.
    */
    if (resource_info->delay != 0)
      {
        if (timer < time((time_t *) NULL))
          if (!resource_info->update)
            *state|=NextImageState | ExitState;
          else
            {
              /*
                Determine if image file was modified.
              */
              status=stat(displayed_image->filename,&file_info);
              if (status == 0)
                if (update_time != file_info.st_mtime)
                  {
                    /*
                      Redisplay image.
                    */
                    loaded_image=ReadImage(resource_info->image_info);
                    if (loaded_image != (Image *) NULL)
                      *state|=NextImageState | ExitState;
                  }
              timer=time((time_t *) NULL)+resource_info->delay;
            }
        if (XEventsQueued(display,QueuedAfterFlush) == 0)
          {
            /*
              Do not block if delay > 0.
            */
            XDelay(display,SuspendTime << 2);
            continue;
          }
      }
    time_stamp=time((time_t *) NULL);
    XNextEvent(display,&event);
    if (!windows->image.stasis)
      windows->image.stasis=(time((time_t *) NULL)-time_stamp) > 0;
    switch (event.type)
    {
      case ButtonPress:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Button Press: 0x%lx %u +%d+%d\n",
            event.xbutton.window,event.xbutton.button,event.xbutton.x,
            event.xbutton.y);
        if ((event.xbutton.button == Button3) &&
            (event.xbutton.state & Mod1Mask))
          {
            /*
              Convert Alt-Button3 to Button2.
            */
            event.xbutton.button=Button2;
            event.xbutton.state&=(~Mod1Mask);
          }
        if (event.xbutton.window == windows->backdrop.id)
          {
            XSetInputFocus(display,event.xbutton.window,RevertToParent,
              event.xbutton.time);
            break;
          }
        if (event.xbutton.window == windows->image.id)
          {
            switch (event.xbutton.button)
            {
              case Button1:
              {
                /*
                  Select a command from the command menu.
                */
                key_symbol=XCommandWidget(display,windows,command);
                if (key_symbol != XK_VoidSymbol)
                  loaded_image=XMagickCommand(display,resource_info,windows,0,
                    key_symbol,&displayed_image);
                break;
              }
              case Button2:
              {
                /*
                  User pressed the image magnify button.
                */
                if (windows->magnify.mapped)
                  XRaiseWindow(display,windows->magnify.id);
                else
                  {
                    /*
                      Make magnify image.
                    */
                    status=XMakeImage(display,resource_info,&windows->magnify,
                      (Image *) NULL,windows->magnify.width,
                      windows->magnify.height);
                    status|=
                      XMakePixmap(display,resource_info,&windows->magnify);
                    if (status == False)
                      Error("Unable to create magnify image",(char *) NULL);
                    XMapRaised(display,windows->magnify.id);
                  }
                XMagnifyImageWindow(display,windows,&event);
                break;
              }
              case Button3:
              {
                if (displayed_image->montage == (char *) NULL)
                  {
                    XBell(display,0);
                    break;
                  }
                /*
                  User picked an image tile to display.
                */
                loaded_image=XTileImageWindow(display,resource_info,windows,
                  displayed_image,&event);
                if (loaded_image != (Image *) NULL)
                  *state|=MontageImageState | NextImageState | ExitState;
                montage_info.x=windows->image.x;
                montage_info.y=windows->image.y;
                break;
              }
              default:
                break;
            }
            break;
          }
        if (event.xbutton.window == windows->magnify.id)
          {
            char
              command[MaxTextLength];

            int
              factor;

            static char
              *MagnifyMenu[]=
              {
                "2",
                "4",
                "5",
                "6",
                "7",
                "8",
                "9",
                "3",
                (char *) NULL,
              };

            static KeySym
              MagnifyKeys[]=
              {
                XK_2,
                XK_4,
                XK_5,
                XK_6,
                XK_7,
                XK_8,
                XK_9,
                XK_3
              };

            /*
              Select a magnify factor from the pop-up menu.
            */
            factor=XMenuWidget(display,windows,"Magnify",MagnifyMenu,command);
            if (factor >= 0)
              XMagnifyWindowCommand(display,windows,MagnifyKeys[factor]);
            break;
          }
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            break;
          }
        break;
      }
      case ClientMessage:
      {
        if (resource_info->debug)
          (void) fprintf(stderr,"Client Message: 0x%lx 0x%lx %d 0x%lx\n",
            event.xclient.window,event.xclient.message_type,
            event.xclient.format,event.xclient.data.l[0]);
        if (event.xclient.message_type == windows->im_protocols)
          {
            if (*event.xclient.data.l == windows->im_window_colormap)
              {
                /*
                  Update window colormap and graphic context.
                */
                for (i=0; i < number_windows; i++)
                {
                  if (magick_windows[i]->id == windows->icon.id)
                    continue;
                  XSetWindowColormap(display,magick_windows[i]->id,
                    map_info->colormap);
                  XSetBackground(display,magick_windows[i]->annotate_context,
                    pixel_info.background_color.pixel);
                  XSetForeground(display,magick_windows[i]->annotate_context,
                    pixel_info.foreground_color.pixel);
                  XSetBackground(display,magick_windows[i]->widget_context,
                    pixel_info.background_color.pixel);
                  XSetForeground(display,magick_windows[i]->widget_context,
                    pixel_info.foreground_color.pixel);
                  XSetBackground(display,magick_windows[i]->highlight_context,
                    pixel_info.foreground_color.pixel);
                  XSetForeground(display,magick_windows[i]->highlight_context,
                    pixel_info.background_color.pixel);
                }
                if (windows->backdrop.id != (Window) NULL)
                  XInstallColormap(display,map_info->colormap);
                break;
              }
            if (*event.xclient.data.l == windows->im_former_image)
              {
                *state|=FormerImageState | ExitState;
                break;
              }
            if (*event.xclient.data.l == windows->im_next_image)
              {
                *state|=NextImageState | ExitState;
                break;
              }
            if (*event.xclient.data.l == windows->im_exit)
              {
                *state|=ExitState;
                break;
              }
            break;
          }
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
            *state|=ExitState;
            break;
          }
        if (event.xclient.window == windows->pan.id)
          {
            /*
              Restore original image size when pan window is deleted.
            */
            windows->image.window_changes.width=windows->image.ximage->width;
            windows->image.window_changes.height=windows->image.ximage->height;
            (void) XConfigureImageWindow(display,resource_info,windows,
              displayed_image);
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
            if ((event.xconfigure.width == windows->image.width) &&
                (event.xconfigure.height == windows->image.height))
              break;
            windows->image.width=event.xconfigure.width;
            windows->image.height=event.xconfigure.height;
            windows->image.x=0;
            windows->image.y=0;
            if (displayed_image->montage != (char *) NULL)
              {
                windows->image.x=montage_info.x;
                windows->image.y=montage_info.y;
              }
            if (windows->image.mapped && windows->image.stasis)
              {
                /*
                  Update image window configuration.
                */
                windows->image.window_changes.width=event.xconfigure.width;
                windows->image.window_changes.height=event.xconfigure.height;
                (void) XConfigureImageWindow(display,resource_info,windows,
                  displayed_image);
              }
            else
              if ((event.xconfigure.width < windows->image.ximage->width) ||
                  (event.xconfigure.height < windows->image.ximage->height))
                {
                  XMapRaised(display,windows->pan.id);
                  XDrawPanRectangle(display,windows);
                }
              else
                if (windows->pan.mapped)
                  XWithdrawWindow(display,windows->pan.id,windows->pan.screen);
            break;
          }
        if (event.xconfigure.window == windows->magnify.id)
          {
            unsigned int
              magnify;

            /*
              Magnify window has a new configuration.
            */
            windows->magnify.width=event.xconfigure.width;
            windows->magnify.height=event.xconfigure.height;
            if (!windows->magnify.mapped)
              break;
            magnify=1;
            while (magnify <= event.xconfigure.width)
              magnify<<=1;
            while (magnify <= event.xconfigure.height)
              magnify<<=1;
            magnify>>=1;
            if ((magnify != event.xconfigure.width) ||
                (magnify != event.xconfigure.height))
              {
                window_changes.width=magnify;
                window_changes.height=magnify;
                XReconfigureWMWindow(display,windows->magnify.id,
                  windows->magnify.screen,CWWidth | CWHeight,&window_changes);
                break;
              }
            status=XMakeImage(display,resource_info,&windows->magnify,
              (Image *) NULL,windows->magnify.width,windows->magnify.height);
            status|=XMakePixmap(display,resource_info,&windows->magnify);
            if (status == False)
              Error("Unable to create magnify image",(char *) NULL);
            XMakeMagnifyImage(display,windows);
            break;
          }
        if (event.xconfigure.window == windows->pan.id)
          {
            /*
              Icon window has a new configuration.
            */
            windows->pan.width=event.xconfigure.width;
            windows->pan.height=event.xconfigure.height;
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
        break;
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
          Refresh windows that are now exposed.
        */
        if (event.xexpose.window == windows->image.id)
          if (windows->image.mapped)
            {
              XRefreshWindow(display,&windows->image,&event);
              timer=time((time_t *) NULL)+resource_info->delay;
              break;
            }
        if (event.xexpose.window == windows->magnify.id)
          if (event.xexpose.count == 0)
            if (windows->magnify.mapped)
              {
                XMakeMagnifyImage(display,windows);
                break;
              }
        if (event.xexpose.window == windows->pan.id)
          if (event.xexpose.count == 0)
            {
              XDrawPanRectangle(display,windows);
              break;
            }
        if (event.xexpose.window == windows->icon.id)
          if (event.xexpose.count == 0)
            {
              XRefreshWindow(display,&windows->icon,&event);
              break;
            }
        break;
      }
      case KeyPress:
      {
        static int
          length;

        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (resource_info->debug)
          (void) fprintf(stderr,"Key press: 0x%lx (%s)\n",key_symbol,command);
        if (event.xkey.window == windows->image.id)
          loaded_image=XMagickCommand(display,resource_info,windows,
            event.xkey.state,key_symbol,&displayed_image);
        if (event.xkey.window == windows->magnify.id)
          XMagnifyWindowCommand(display,windows,key_symbol);
        if (event.xkey.window == windows->pan.id)
          if ((key_symbol == XK_F1) || (key_symbol == XK_Help))
            XTextViewWidget(display,windows,"Help Viewer - Image Panning",
              ImagePanHelp);
        break;
      }
      case KeyRelease:
      {
        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if (resource_info->debug)
          (void) fprintf(stderr,"Key release: 0x%lx (%c)\n",key_symbol,
            *command);
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
            if (*displayed_image->filename == '\0')
              loaded_image=XMagickCommand(display,resource_info,windows,0,XK_l,
                &displayed_image);
            if ((windows->image.width < windows->image.ximage->width) ||
                (windows->image.height < windows->image.ximage->height))
              XMapRaised(display,windows->pan.id);
            windows->image.mapped=True;
            break;
          }
        if (event.xmap.window == windows->magnify.id)
          {
            XMakeMagnifyImage(display,windows);
            windows->magnify.mapped=True;
            break;
          }
        if (event.xmap.window == windows->pan.id)
          {
            XRefreshWindow(display,&windows->image,(XEvent *) NULL);
            XMakePanImage(display,resource_info,windows,displayed_image);
            windows->pan.mapped=True;
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
            XMakeStandardColormap(display,icon_visual,&icon_resources,
              displayed_image,icon_map,&icon_pixel);
            (void) XMakeImage(display,&icon_resources,&windows->icon,
              displayed_image,windows->icon.width,windows->icon.height);
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
      case NoExpose:
        break;
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
        if (event.xunmap.window == windows->magnify.id)
          {
            windows->magnify.mapped=False;
            break;
          }
        if (event.xunmap.window == windows->pan.id)
          {
            windows->pan.mapped=False;
            break;
          }
        if (event.xunmap.window == windows->info.id)
          {
            windows->info.mapped=False;
            break;
          }
        if (event.xunmap.window == windows->icon.id)
          {
            if (map_info->colormap == icon_map->colormap)
              XConfigureImageColormap(display,resource_info,windows,
                displayed_image);
            XFreeStandardColormap(display,icon_visual,icon_map,&icon_pixel);
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
  while (!(*state & ExitState));
  if ((*state & FormerImageState) || (*state & NextImageState))
    *state&=(~ExitState);
  former_ximage=(*windows->image.ximage);
  /*
    Alert user we are busy.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  if (resource_info->write_filename != (char *) NULL)
    {
      /*
        Update image with user transforms.
      */
      if ((windows->image.clip_geometry != (char *) NULL) ||
          (displayed_image->columns != windows->image.ximage->width) ||
          (displayed_image->rows != windows->image.ximage->height))
        {
          char
            image_geometry[MaxTextLength];

          /*
            Clip and/or scale displayed image.
          */
          (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
            windows->image.ximage->height);
          TransformImage(&displayed_image,windows->image.clip_geometry,
            image_geometry);
        }
      if (resource_info->number_colors != 0)
        {
          /*
            Reduce the number of colors in the image.
          */
          if ((displayed_image->class == DirectClass) ||
              (displayed_image->colors > resource_info->number_colors) ||
              (resource_info->colorspace == GRAYColorspace))
            QuantizeImage(displayed_image,resource_info->number_colors,
              resource_info->tree_depth,resource_info->dither,
              resource_info->colorspace,True);
          SyncImage(displayed_image);
        }
    }
  /*
    Withdraw pan and magnify window.
  */
  if (windows->pan.mapped)
    XWithdrawWindow(display,windows->pan.id,windows->pan.screen);
  if (windows->magnify.mapped)
    XWithdrawWindow(display,windows->magnify.id,windows->magnify.screen);
  XCheckRefreshWindow(display,&windows->image);
  /*
    Free X resources.
  */
  (void) free((char *) windows->popup.name);
  (void) free((char *) windows->magnify.name);
  (void) free((char *) windows->image.name);
  (void) free((char *) windows->image.icon_name);
  for (i=0; i < number_windows; i++)
  {
    XFreeCursor(display,magick_windows[i]->cursor);
    XFreeCursor(display,magick_windows[i]->busy_cursor);
  }
  if (windows->popup.highlight_stipple != (Pixmap) NULL)
    XFreePixmap(display,windows->popup.highlight_stipple);
  if (windows->popup.highlight_stipple != (Pixmap) NULL)
    XFreePixmap(display,windows->popup.shadow_stipple);
  XFreeGC(display,pixel_info.highlight_context);
  XFreeGC(display,pixel_info.widget_context);
  XFreeGC(display,pixel_info.annotate_context);
  XFreeFont(display,font_info);
  if (*state & ExitState)
    {
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
      }
      /*
        Free Standard Colormap.
      */
      if (resource_info->map_type == (char *) NULL)
        XFreeStandardColormap(display,visual_info,map_info,&pixel_info);
      (void) free((void *) windows);
      XFree((void *) class_hint);
      XFree((void *) manager_hints);
      XFree((void *) icon_visual);
      XFree((void *) visual_info);
      XFree((void *) icon_map);
      XFree((void *) map_info);
      visual_info=(XVisualInfo *) NULL;
    }
  XFlush(display);
  *image=displayed_image;
  return(loaded_image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w P a n R e c t a n g l e                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawPanRectangle draws a rectangle in the pan window.  The pan
%  window displays a scaled image and the rectangle shows which portion of
%  the image is displayed in the image window.
%
%  The format of the XDrawPanRectangle routine is:
%
%    XDrawPanRectangle(display,windows)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%
*/
static void XDrawPanRectangle(display,windows)
Display
  *display;

XWindows
  *windows;
{
  unsigned long
    scale_factor;

  RectangleInfo
    highlight_info;

  /*
    Determine dimensions of the panning rectangle.
  */
  scale_factor=(unsigned long)
    (UpShift(windows->pan.width)/windows->image.ximage->width);
  highlight_info.x=DownShift(windows->image.x*scale_factor);
  highlight_info.width=DownShift(windows->image.width*scale_factor);
  scale_factor=(unsigned long)
    (UpShift(windows->pan.height)/windows->image.ximage->height);
  highlight_info.y=DownShift(windows->image.y*scale_factor);
  highlight_info.height=DownShift(windows->image.height*scale_factor);
  /*
    Display the panning rectangle.
  */
  XClearWindow(display,windows->pan.id);
  XHighlightRegion(display,windows->pan.id,windows->pan.annotate_context,
    &highlight_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X E d g e I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XEdgeImageWindow detects edges within an image.
%
%  The format of the XEdgeImageWindow routine is:
%
%    status=XEdgeImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XEdgeImageWindow return True if the edges within
%      the image are detected.  False is returned is there is a memory
%      shortage or if the edges cannot be detected.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XEdgeImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define EdgeImageText  "  Detecting edges...  "

  Image
    *edged_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,EdgeImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,EdgeImageText);
  /*
    Edge image scanlines.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  edged_image=EdgeImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (edged_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=edged_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X F l i p I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XFlipImageWindow flips the scanlines of an image in the horizontal
%  direction.
%
%  The format of the XFlipImageWindow routine is:
%
%    status=XFlipImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XFlipImageWindow return True if the window scanlines
%      reverse.  False is returned is there is a memory shortage or if the
%      window scanlines fails to reverse.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XFlipImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define FlipImageText  "  Flipping image...  "

  Image
    *flipped_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,FlipImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,FlipImageText);
  /*
    Flip image scanlines.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  flipped_image=FlipImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (flipped_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=flipped_image;
  if (windows->image.clip_geometry != (char *) NULL)
    {
      int
        x,
        y;

      unsigned int
        height,
        width;

      /*
        Flip clip geometry.
      */
      (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
      (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",width,height,
        x,(int) (*image)->rows-(int) height-y);
    }
  /*
    Update image configuration.
  */
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X F l o p I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XFlopImageWindow flops the scanlines of an image in the horizontal
%  direction.
%
%  The format of the XFlopImageWindow routine is:
%
%    status=XFlopImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XFlopImageWindow return True if the window scanlines
%      reverse.  False is returned is there is a memory shortage or if the
%      window scanlines fails to reverse.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XFlopImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define FlopImageText  "  Flopping image...  "

  Image
    *flopped_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,FlopImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,FlopImageText);
  /*
    Flop image scanlines.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  flopped_image=FlopImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (flopped_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=flopped_image;
  if (windows->image.clip_geometry != (char *) NULL)
    {
      int
        x,
        y;

      unsigned int
        height,
        width;

      /*
        Flop clip geometry.
      */
      (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
      (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",width,height,
        (int) (*image)->columns-(int) width-x,y);
    }
  /*
    Update image configuration.
  */
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X L o a d I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XLoadImageWindow loads an image from a file.
%
%  The format of the XLoadImageWindow routine is:
%
%    loaded_image=XLoadImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XLoadImageWindow returns an image if can be loaded
%      successfully.  Otherwise a null image is returned.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static Image *XLoadImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define LoadImageText  "  Loading image...  "

  char
    filename[MaxTextLength];

  Image
    *loaded_image;

  /*
    Request file name from user.
  */
  (void) strcpy(filename,image->filename);
  XFileBrowserWidget(display,windows,"Load",filename);
  if (*filename == '\0')
    return((Image *) NULL);
  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,LoadImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,LoadImageText);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XCheckRefreshWindow(display,&windows->image);
  /*
    Load the image.
  */
  *resource_info->image_info->magick='\0';
  (void) strcpy(resource_info->image_info->filename,filename);
  loaded_image=ReadImage(resource_info->image_info);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (loaded_image != (Image *) NULL)
    XClientMessage(display,windows->image.id,windows->im_protocols,
      windows->im_next_image,CurrentTime);
  else
    {
      char
        *text,
        **textlist;

      FILE
        *file;

      int
        c;

      register char
        *p;

      unsigned int
        length;

      /*
        Unknown image format.
      */
      file=(FILE *) fopen(filename,"rb");
      if (file == (FILE *) NULL)
        {
          XNoticeWidget(display,windows,"Unable to load image",filename);
          return((Image *) NULL);
        }
      length=MaxTextLength;
      text=(char *) malloc(length*sizeof(char));
      for (p=text ; text != (char *) NULL; p++)
      {
        c=fgetc(file);
        if (c == EOF)
          break;
        if ((p-text+1) >= length)
          {
            *p='\0';
            length<<=1;
            text=(char *) realloc((char *) text,length*sizeof(char));
            if (text == (char *) NULL)
              break;
            p=text+strlen(text);
          }
        *p=(unsigned char) c;
      }
      (void) fclose(file);
      if (text == (char *) NULL)
        {
          XNoticeWidget(display,windows,"Unable to load image",filename);
          return((Image *) NULL);
        }
      *p='\0';
      textlist=StringToList(text);
      if (textlist != (char **) NULL)
        {
          char
            title[MaxTextLength];

          register int
            i;

          (void) sprintf(title,"Unknown format: %s",filename);
          XTextViewWidget(display,windows,title,textlist);
          for (i=0; textlist[i] != (char *) NULL; i++)
            (void) free((char *) textlist[i]);
          (void) free((char *) textlist);
        }
      (void) free((char *) text);
    }
  return(loaded_image);
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
%  Function XMagickCommand makes a transform to the image or image window
%  as specified by a user menu button or keyboard command.
%
%  The format of the XMagickCommand routine is:
%
%    loaded_image=XMagickCommand(display,resource_info,windows,state,
%      key_symbol,image)
%
%  A description of each parameter follows:
%
%    o loaded_image:  Function XMagickCommand returns an image when the
%      user chooses 'Load Image' from the command menu.  Otherwise a null
%      image is returned.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o state: key mask.
%
%    o key_symbol: Specifies a command to perform.
%
%    o image: Specifies a pointer to a Image structure;  XMagickCommand
%      may transform the image and return a new image pointer.
%
%
*/
static Image *XMagickCommand(display,resource_info,windows,state,key_symbol,
  image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

unsigned int
  state;

KeySym
  key_symbol;

Image
  **image;
{
  typedef struct _ImageState
  {
    unsigned int
      width,
      height;

    char
      *clip_geometry;

    Image
      *image;
  } ImageState;

  static char
    *ImageMagickHelp[]=
    {
      "Press F1 whenever you feel you need help.  There is a help",
      "pane associated with most of the image functions described",
      "below.",
      "",
      "BUTTONS",
      "   The effects of each button press is described below.  Three",
      "   buttons are required.  If you have a two button mouse,",
      "   button 1 and 3 are returned.  Press ALT and button 3 to",
      "   simulate button 2.",
      "",
      "   1    Press and drag to select a command from a pop-up menu.",
      "        Choose from these menu items:",
      "",
      "            File",
      "              Image Info",
      "              Write...",
      "              Print...",
      "              Load...",
      "              Next",
      "              Former",
      "            Edit",
      "              Undo",
      "              Restore",
      "              Refresh",
      "            View",
      "              Half Size",
      "              Original Size",
      "              Double Size",
      "              Resize...",
      "            Transform",
      "              Trim Edges",
      "              Clip",
      "              Cut",
      "              Flop",
      "              Flip",
      "              Rotate Right",
      "              Rotate Left",
      "              Rotate...",
      "              Shear...",
      "            Enhance",
      "              Negate",
      "              Equalize",
      "              Normalize",
      "              Gamma...",
      "            Effects",
      "              Despeckle",
      "              Peak Noisy",
      "              Sharpen",
      "              Blur",
      "              Edge Detect",
      "              Grayscale",
      "              Quantize...",
      "            Image Edit",
      "              Annotate",
      "              Add Border",
      "              Composite...",
      "              Color",
      "              Matte",
      "              Comment",
      "            Help",
      "              About IM",
      "              Version...",
      "            Quit",
      "",
      "        The indented items are members of a sub-menu.  Access",
      "        them by moving the pointer toward the right edge of the",
      "        menu.",
      "",
      "   2    Press and drag to define a region of the image to",
      "        magnify.",
      "",
      "   3    If the image is a montage image, choose a particular",
      "        tile of the image and press this button. The image",
      "        represented by the tile is then displayed.  To return",
      "        to the composite MIFF image, choose Next from the",
      "        command menu (refer to Button 1).  See montage(1) and",
      "        miff(5) for more details.",
      "",
      "KEYBOARD ACCELERATORS",
      "   i    Press to display information about the image.",
      "",
      "   w    Press to write the image to a file.",
      "",
      "   p    Press to print the image to a Postscript printer.",
      "",
      "   d    Press to delete the image file.",
      "",
      "   l    Press to load an image from a file.",
      "",
      "   n    Press to display the next image.",
      "",
      "   f    Press to display the former image.",
      "",
      "   u    Press to undo last image transformation.",
      "",
      "   r    Press to restore the image to its original size.",
      "",
      "   @    Press to refresh the image window.",
      "",
      "   <    Press to half the image size.",
      "",
      "   o    Press to return to the original image size",
      "",
      "   >    Press to double the image size.",
      "",
      "   %    Press to resize the image to a width and height you",
      "        specify.",
      "",
      "   t    Press to trim the image edges.",
      "",
      "   [    Press to crop the image.",
      "",
      "   ]    Press to cut the image.",
      "",
      "   |    Press to flop image in the horizontal direction.",
      "",
      "   -    Press to flip image in the vertical direction.",
      "",
      "   /    Press to rotate the image 90 degrees clockwise.",
      "",
      "   \\    Press to rotate the image 90 degrees counter-clockwise.",
      "",
      "   *    Press to rotate the image the number of degrees you",
      "        specify.",
      "",
      "   s    Press to shear the image the number of degrees you",
      "        specify.",
      "",
      "   ~    Press to invert the colors of the image.",
      "",
      "   =    Press to perform histogram equalization on the image.",
      "",
      "   N    Press to perform histogram normalization on the image",
      "",
      "   g    Press to gamma correct the image.",
      "",
      "   D    Press to reduce the speckles within an image.",
      "",
      "   P    Press to  eliminate peak noise from an image.",
      "",
      "   S    Press to sharpen the image.",
      "",
      "   B    Press to blur the image.",
      "",
      "   E    Press to detect edges within an image.",
      "",
      "   G    Press to convert the image colors to gray.",
      "",
      "   Q    Press to set the maximum number of unique colors in the",
      "        image.",
      "",
      "   a    Press to annotate the image with text.",
      "",
      "   b    Press to add a border to the image.",
      "",
      "   x    Press to composite the image with another.",
      "",
      "   c    Press to edit an image pixel color.",
      "",
      "   m    Press to edit the image matte information.",
      "",
      "   #    Press to add an image comment.",
      "",
      "   h    Press to display helpful information about display(1).",
      "",
      "        Function keys HELP or F1 are synonomous with the h key.",
      "",
      "   v    Press to display the version number of display(1).",
      "",
      "   q    Press to discard all images and exit program.",
      "",
      "   1-9  Press to change the level of magnification.",
      "",
      "Use the arrow keys to move the image one pixel up, down, left,",
      "or right within an magnify window.  Be sure to first map the",
      "magnify window by pressing button 2",
      "",
      "Press ALT and one of the arrow keys to trim off one pixel from",
      "any side of the image.",
      (char *) NULL,
    };

  char
    geometry[MaxTextLength];

  Image
    *loaded_image;

  static ImageState
    undo_image = {0, 0, (char *) NULL, (Image *) NULL};

  unsigned int
    status;

  XCheckRefreshWindow(display,&windows->image);
  if (key_symbol == XK_u)
    {
      /*
        Undo the last image transformation.
      */
      if (undo_image.image == (Image *) NULL)
        {
          XBell(display,0);
          return((Image *) NULL);
        }
      windows->image.window_changes.width=undo_image.width;
      windows->image.window_changes.height=undo_image.height;
      if (windows->image.clip_geometry != (char *) NULL)
        (void) free((char *) windows->image.clip_geometry);
      windows->image.clip_geometry=undo_image.clip_geometry;
      DestroyImage(*image);
      *image=undo_image.image;
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      undo_image.clip_geometry=(char *) NULL;
      undo_image.image=(Image *) NULL;
      return((Image *) NULL);
    }
  switch (key_symbol)
  {
    case XK_less:
    case XK_o:
    case XK_greater:
    case XK_percent:
    case XK_t:
    case XK_bracketright:
    case XK_minus:
    case XK_bar:
    case XK_slash:
    case XK_backslash:
    case XK_asterisk:
    case XK_s:
    case XK_asciitilde:
    case XK_equal:
    case XK_N:
    case XK_g:
    case XK_D:
    case XK_P:
    case XK_S:
    case XK_B:
    case XK_E:
    case XK_G:
    case XK_Q:
    case XK_a:
    case XK_b:
    case XK_x:
    case XK_c:
    case XK_m:
    {
      /*
        Save image before transformation.
      */
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XFlush(display);
      undo_image.width=windows->image.ximage->width;
      undo_image.height=windows->image.ximage->height;
      if (undo_image.clip_geometry != (char *) NULL)
        (void) free((char *) undo_image.clip_geometry);
      undo_image.clip_geometry=windows->image.clip_geometry;
      if (windows->image.clip_geometry != (char *) NULL)
        {
          undo_image.clip_geometry=(char *) malloc(MaxTextLength*sizeof(char));
          if (undo_image.clip_geometry != (char *) NULL)
            (void) strcpy(undo_image.clip_geometry,
              windows->image.clip_geometry);
        }
      if (undo_image.image != (Image *) NULL)
        DestroyImage(undo_image.image);
      undo_image.image=CopyImage(*image,(*image)->columns,(*image)->rows,True);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      break;
    }
    default:
      break;
  }
  /*
    Process user command.
  */
  loaded_image=(Image *) NULL;
  windows->image.window_changes.width=windows->image.ximage->width;
  windows->image.window_changes.height=windows->image.ximage->height;
  switch (key_symbol)
  {
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
      if ((*image)->matte)
        (void) strcat(text,"  matte: True\n");
      else
        (void) strcat(text,"  matte: False\n");
      (void) sprintf(text,"%s  geometry: %ux%u\n",text,
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
    case XK_w:
    {
      /*
        Write image.
      */
      status=XWriteImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to write X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_p:
    {
      /*
        Print image.
      */
      status=XPrintImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to print X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_d:
    {
      char
        filename[MaxTextLength];

      /*
        Delete image file.
      */
      (void) sprintf(filename,(*image)->filename);
      XFileBrowserWidget(display,windows,"Delete",filename);
      if (*filename == '\0')
        break;
      status=unlink(filename);
      if (status != False)
        XNoticeWidget(display,windows,"Unable to delete image file",filename);
      break;
    }
    case XK_l:
    {
      /*
        Load image.
      */
      loaded_image=XLoadImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_n:
    case XK_Next:
    case XK_space:
    {
      /*
        Display next image.
      */
      XClientMessage(display,windows->image.id,windows->im_protocols,
        windows->im_next_image,CurrentTime);
      break;
    }
    case XK_f:
    case XK_Prior:
    case XK_KP_Prior:
    case XK_BackSpace:
    {
      /*
        Display former image.
      */
      XClientMessage(display,windows->image.id,windows->im_protocols,
        windows->im_former_image,CurrentTime);
      break;
    }
    case XK_less:
    {
      /*
        Half image size.
      */
      windows->image.window_changes.width=windows->image.ximage->width >> 1;
      windows->image.window_changes.height=windows->image.ximage->height >> 1;
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_o:
    {
      /*
        Original image size.
      */
      windows->image.window_changes.width=(*image)->columns;
      windows->image.window_changes.height=(*image)->rows;
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_greater:
    {
      /*
        Double the image size.
      */
      windows->image.window_changes.width=windows->image.ximage->width << 1;
      windows->image.window_changes.height=windows->image.ximage->height << 1;
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_percent:
    {
      unsigned int
        height,
        width;

      /*
        Resize image.
      */
      width=windows->image.ximage->width;
      height=windows->image.ximage->height;
      (void) sprintf(geometry,"%ux%u",width,height);
      XDialogWidget(display,windows,"Resize",
        "Enter resize geometry (e.g. 640x480, 200%):",geometry);
      if (*geometry == '\0')
        break;
      ParseImageGeometry(geometry,&width,&height);
      windows->image.window_changes.width=width;
      windows->image.window_changes.height=height;
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_r:
    {
      /*
        Restore image window to its original size.
      */
      if ((windows->image.width == (*image)->columns) &&
          (windows->image.height == (*image)->rows) &&
          (windows->image.clip_geometry == (char *) NULL))
        {
          XBell(display,0);
          break;
        }
      windows->image.window_changes.width=(*image)->columns;
      windows->image.window_changes.height=(*image)->rows;
      if (windows->image.clip_geometry != (char *) NULL)
        {
          (void) free((char *) windows->image.clip_geometry);
          windows->image.clip_geometry=(char *) NULL;
          windows->image.x=0;
          windows->image.y=0;
        }
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_at:
    {
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_t:
    {
      /*
        Trim image.
      */
      status=XTrimImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to trim X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_bracketleft:
    {
      /*
        Clip image.
      */
      (void) XClipImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_bracketright:
    {
      /*
        Cut image.
      */
      status=XCutImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to cut X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_bar:
    {
      /*
        Flop image scanlines.
      */
      status=XFlopImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to flop X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_minus:
    {
      /*
        Flip image scanlines.
      */
      status=XFlipImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to flip X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_slash:
    {
      /*
        Rotate image 90 degrees clockwise.
      */
      status=XRotateImageWindow(display,resource_info,windows,90.0,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to rotate X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_backslash:
    {
      /*
        Rotate image 90 degrees counter-clockwise.
      */
      status=XRotateImageWindow(display,resource_info,windows,-90.0,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to rotate X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_asterisk:
    {
      /*
        Rotate image.
      */
      status=XRotateImageWindow(display,resource_info,windows,0.0,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to rotate X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_s:
    {
      /*
        Shear image.
      */
      status=XShearImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to shear X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_asciitilde:
    {
      /*
        Negate colors in image.
      */
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      NegateImage(*image);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_equal:
    {
      /*
        Perform histogram equalization on the image.
      */
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      EqualizeImage(*image);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_N:
    {
      /*
        Perform histogram normalization on the image.
      */
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      NormalizeImage(*image);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_g:
    {
      static char
        gamma[MaxTextLength] = "1.6";

      /*
        Gamma correct image.
      */
      XDialogWidget(display,windows,"Gamma","Enter gamma value:",gamma);
      if (*gamma == '\0')
        break;
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      GammaImage(*image,gamma);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_D:
    {
      /*
        Reduce speckles within an image.
      */
      status=XDespeckleImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to reduce speckles",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_P:
    {
      /*
        Reduce noise within an image.
      */
      status=XNoisyImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to reduce noise",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_S:
    {
      /*
        Sharpen an image.
      */
      status=XSharpenImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to detect edges",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_B:
    {
      /*
        Blur an image.
      */
      status=XBlurImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to detect edges",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_E:
    {
      /*
        Detect edges within an image.
      */
      status=XEdgeImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to detect edges",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_G:
    {
      /*
        Convert image to grayscale.
      */
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      QuantizeImage(*image,256,8,resource_info->dither,GRAYColorspace,True);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_Q:
    {
      static char
        colors[MaxTextLength] = "256";

      /*
        Color reduce the image.
      */
      XDialogWidget(display,windows,"Colors","Maximum number of colors:",
        colors);
      if (*colors == '\0')
        break;
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      QuantizeImage(*image,(unsigned int) atoi(colors),
        resource_info->tree_depth,resource_info->dither,
        resource_info->colorspace,True);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_a:
    {
      /*
        Annotate the image with text.
      */
      status=XAnnotateImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to annotate X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_b:
    {
      ColorPacket
        border_color;

      int
        flags;

      Image
        *bordered_image;

      RectangleInfo
        border_info;

      static char
        color_name[MaxTextLength] = "gray",
        geometry[MaxTextLength] = "10x10";

      XColor
        color;

      /*
        Add a border to the image.
      */
      XColorBrowserWidget(display,windows,"Select",color_name);
      if (*color_name == '\0')
        break;
      XCheckRefreshWindow(display,&windows->image);
      (void) XParseColor(display,windows->image.map_info->colormap,color_name,
        &color);
      border_color.red=color.red >> 8;
      border_color.green=color.green >> 8;
      border_color.blue=color.blue >> 8;
      XDialogWidget(display,windows,"Add Border","Enter border geometry:",
        geometry);
      if (*geometry == '\0')
        break;
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XCheckRefreshWindow(display,&windows->image);
      border_info.width=0;
      border_info.height=0;
      flags=XParseGeometry(geometry,&border_info.x,&border_info.y,
        &border_info.width,&border_info.height);
      if ((flags & HeightValue) == 0)
        border_info.height=border_info.width;
      bordered_image=BorderImage(*image,&border_info,&border_color);
      if (bordered_image != (Image *) NULL)
        {
          DestroyImage(*image);
          bordered_image->class=DirectClass;
          *image=bordered_image;
        }
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      windows->image.window_changes.width=(*image)->columns;
      windows->image.window_changes.height=(*image)->rows;
      XConfigureImageColormap(display,resource_info,windows,*image);
      (void) XConfigureImageWindow(display,resource_info,windows,*image);
      break;
    }
    case XK_x:
    {
      /*
        Composite image.
      */
      status=XCompositeImageWindow(display,resource_info,windows,image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to composite X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_c:
    {
      /*
        Color edit.
      */
      status=XColorEditImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to pixel edit X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_m:
    {
      /*
        Matte edit.
      */
      status=XMatteEditImageWindow(display,resource_info,windows,*image);
      if (status == False)
        {
          XNoticeWidget(display,windows,"Unable to matte edit X image",
            (*image)->filename);
          break;
        }
      break;
    }
    case XK_numbersign:
    {
      char
        command[MaxTextLength],
        *directory,
        filename[MaxTextLength];

      FILE
        *file;

      /*
        Edit image comment.
      */
      directory=(char *) getenv("TMPDIR");
      if (directory == (char *) NULL)
        directory="/tmp";
      (void) sprintf(filename,"%s/magickXXXXXX",directory);
      (void) mktemp(filename);
      if ((*image)->comments != (char *) NULL)
        {
          register char
            *p;

          file=fopen(filename,"wb");
          if (file == (FILE *) NULL)
            {
              XNoticeWidget(display,windows,"Unable to edit image comment",
                filename);
              break;
            }
          for (p=(*image)->comments; *p != '\0'; p++)
            (void) putc((int) *p,file);
          (void) putc('\n',file);
          (void) fclose(file);
        }
      (void) sprintf(command,EditorCommand,filename);
      if (resource_info->editor_command != (char *) NULL)
        (void) sprintf(command,resource_info->editor_command,filename);
      XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
      XFlush(display);
      (void) system(command);
      (void) sprintf(command,"@%s",filename);
      CommentImage(*image,command);
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      (void) unlink(filename);
      break;
    }
    case XK_h:
    case XK_F1:
    case XK_Help:
    {
      XTextViewWidget(display,windows,"Help Viewer - ImageMagick",
        ImageMagickHelp);
      break;
    }
    case XK_v:
    {
      XNoticeWidget(display,windows,Version,
        "Copyright (c) 1994 E. I. du Pont de Nemours and Company");
      break;
    }
    case XK_q:
    {
      /*
        Exit program.
      */
      XClientMessage(display,windows->image.id,windows->im_protocols,
        windows->im_exit,CurrentTime);
      break;
    }
    case XK_Home:
    case XK_KP_Home:
    {
      XTranslateImageWindow(display,windows,*image,key_symbol);
      break;
    }
    case XK_Up:
    case XK_KP_Up:
    case XK_Down:
    case XK_KP_Down:
    case XK_Left:
    case XK_KP_Left:
    case XK_Right:
    case XK_KP_Right:
    {
      if (state & Mod1Mask)
        {
          RectangleInfo
            clip_info;

          /*
            Trim one pixel from edge of image.
          */
          clip_info.x=0;
          clip_info.y=0;
          clip_info.width=windows->image.ximage->width;
          clip_info.height=windows->image.ximage->height;
          if ((key_symbol == XK_Up) || (key_symbol == XK_KP_Up))
            clip_info.height--;
          if ((key_symbol == XK_Down) || (key_symbol == XK_KP_Down))
            {
              clip_info.y++;
              clip_info.height--;
            }
          if ((key_symbol == XK_Left) || (key_symbol == XK_KP_Left))
            clip_info.width--;
          if ((key_symbol == XK_Right) || (key_symbol == XK_KP_Right))
            {
              clip_info.x++;
              clip_info.width--;
            }
          XSetClipGeometry(display,windows,&clip_info,*image);
          windows->image.window_changes.width=clip_info.width;
          windows->image.window_changes.height=clip_info.height;
          XSetWindowBackgroundPixmap(display,windows->image.id,None);
          (void) XConfigureImageWindow(display,resource_info,windows,*image);
          break;
        }
      XTranslateImageWindow(display,windows,*image,key_symbol);
      break;
    }
    case XK_Return:
    case XK_KP_Enter:
      break;
    default:
    {
      if (!IsModifierKey(key_symbol))
        XBell(display,0);
      break;
    }
  }
  return(loaded_image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a g n i f y I m a g e W i n d o w                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMagnifyImageWindow magnifies portions of the image as indicated
%  by the pointer.  The magnified portion is displayed in a separate window.
%
%  The format of the XMagnifyImageWindow routine is:
%
%    XMagnifyImageWindow(display,windows,event)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o event: Specifies a pointer to a XEvent structure.  If it is NULL,
%      the entire image is refreshed.
%
%
*/
static void XMagnifyImageWindow(display,windows,event)
Display
  *display;

XWindows
  *windows;

XEvent
  *event;
{
  char
    text[MaxTextLength];

  register int
    x,
    y;

  unsigned long
    state;

  /*
    Map info window.
  */
  state=DefaultState;
  (void) sprintf(text," +%u+%u ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Update magnified image until the mouse button is released.
  */
  XDefineCursor(display,windows->image.id,windows->magnify.cursor);
  x=event->xbutton.x;
  y=event->xbutton.y;
  windows->magnify.x=windows->image.x+x;
  windows->magnify.y=windows->image.y+y;
  do
  {
    /*
      Map and unmap info window as text cursor crosses its boundaries.
    */
    if (windows->info.mapped)
      {
        if ((x < (windows->info.x+windows->info.width)) &&
            (y < (windows->info.y+windows->info.height)))
          XWithdrawWindow(display,windows->info.id,windows->info.screen);
      }
    else
      if ((x > (windows->info.x+windows->info.width)) ||
          (y > (windows->info.y+windows->info.height)))
        XMapWindow(display,windows->info.id);
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",windows->magnify.x,windows->magnify.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,event,XScreenEvent,(char *) windows);
    switch (event->type)
    {
      case ButtonPress:
        break;
      case ButtonRelease:
      {
        /*
          User has finished magnifying image.
        */
        x=event->xbutton.x;
        y=event->xbutton.y;
        state|=ExitState;
        break;
      }
      case Expose:
        break;
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,event));
        x=event->xmotion.x;
        y=event->xmotion.y;
        break;
      }
      default:
        break;
    }
    /*
      Check boundary conditions.
    */
    if (x < 0)
      x=0;
    else
      if (x >= windows->image.width)
        x=windows->image.width-1;
    if (y < 0)
      y=0;
    else
     if (y >= windows->image.height)
       y=windows->image.height-1;
  } while (!(state & ExitState));
  /*
    Display magnified image.
  */
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a g n i f y W i n d o w C o m m a n d                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMagnifyWindowCommand moves the image within an magnify window by
%  one pixel as specified by the key symbol.
%
%  The format of the XMagnifyWindowCommand routine is:
%
%    XMagnifyWindowCommand(display,windows,key_symbol)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o key_symbol: Specifies a KeySym which indicates which side of the image
%      to trim.
%
%
*/
static void XMagnifyWindowCommand(display,windows,key_symbol)
Display
  *display;

XWindows
  *windows;

KeySym
  key_symbol;
{
  /*
    User specified a magnify factor or position.
  */
  switch (key_symbol)
  {
    case XK_Home:
    case XK_KP_Home:
    {
      windows->magnify.x=windows->image.width >> 1;
      windows->magnify.y=windows->image.height >> 1;
      break;
    }
    case XK_Left:
    case XK_KP_Left:
    {
      if (windows->magnify.x > 0)
        windows->magnify.x--;
      break;
    }
    case XK_Up:
    case XK_KP_Up:
    {
      if (windows->magnify.y > 0)
        windows->magnify.y--;
      break;
    }
    case XK_Right:
    case XK_KP_Right:
    {
      if (windows->magnify.x < (windows->image.width-1))
        windows->magnify.x++;
      break;
    }
    case XK_Down:
    case XK_KP_Down:
    {
      if (windows->magnify.y < (windows->image.height-1))
        windows->magnify.y++;
      break;
    }
    case XK_0:
    case XK_1:
    case XK_2:
    case XK_3:
    case XK_4:
    case XK_5:
    case XK_6:
    case XK_7:
    case XK_8:
    case XK_9:
    {
      windows->magnify.data=key_symbol-XK_0;
      break;
    }
    case XK_KP_0:
    case XK_KP_1:
    case XK_KP_2:
    case XK_KP_3:
    case XK_KP_4:
    case XK_KP_5:
    case XK_KP_6:
    case XK_KP_7:
    case XK_KP_8:
    case XK_KP_9:
    {
      windows->magnify.data=key_symbol-XK_KP_0;
      break;
    }
    default:
      break;
  }
  XMakeMagnifyImage(display,windows);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a k e P a n I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMakePanImage creates a thumbnail of the image and displays it in
%  the pan icon window.
%
%  The format of the XMakePanImage routine is:
%
%      XMakePanImage(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static void XMakePanImage(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
  unsigned int
    status;

  /*
    Create and display image for panning icon.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  windows->pan.x=windows->image.x;
  windows->pan.y=windows->image.y;
  status=XMakeImage(display,resource_info,&windows->pan,image,
    windows->pan.width,windows->pan.height);
  status|=XMakePixmap(display,resource_info,&windows->pan);
  if (status == False)
    Error("Unable to create pan icon image",(char *) NULL);
  XSetWindowBackgroundPixmap(display,windows->pan.id,windows->pan.pixmap);
  XClearWindow(display,windows->pan.id);
  XDrawPanRectangle(display,windows);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M a t t a E d i t I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMatteEditImageWindow allows the user to interactively change
%  the Matte channel of an image.  If the image is PseudoClass it is promoted
%  to DirectClass before the matte information is stored.
%
%  The format of the XMatteEditImageWindow routine is:
%
%    XMatteEditImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
*/
static unsigned int XMatteEditImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
  static char
    *ImageMatteEditHelp[]=
    {
      "Matte information within an image is useful for some",
      "operations such as image compositing.  This extra channel",
      "usually defines a mask which represents a sort of a",
      "cookie-cutter for the image.  This is the case when matte",
      "is 255 (full coverage) for pixels inside the shape, zero",
      "outside, and between zero and 255 on the boundary.",
      "",
      "A small window appears showing the location of the cursor in",
      "the image window.  You are now in matte edit mode.  To exit",
      "immediately, press ESC. In matte edit mode a button press",
      "has a different effect than described in BUTTONS.  Press a",
      "button to affect this behavior:",
      "",
      "1    Press to select a pixel within an image window to set",
      "     it's matte value.  The matte value of any pixel within",
      "     the image that matches the color of the selected pixel",
      "     is initialized.",
      "",
      "2    Press an a dialog appears requesting a matte value.",
      "     Enter a value between 0 and 255.  This value is",
      "     assigned as the matte value of the selected pixel or",
      "     pixels.",
      "",
      "Matte information is only valid in a DirectClass image.",
      "Therefore, any PseudoClass image is promoted to",
      "DirectClass.  Note that matte information for PseudoClass",
      "is not retained for colormapped X server visuals (e.g.",
      "StaticColor, StaticColor, GrayScale, PseudoColor) unless",
      "you immediately save the image to a file with 'Write'.",
      "Correct matte editing behavior may require a TrueColor or",
      "DirectColor visual or a Standard Colormap.",
      (char *) NULL,
    };

  char
    text[MaxTextLength];

  Cursor
    cursor;

  int
    i,
    x,
    x_offset,
    y,
    y_offset;

  register RunlengthPacket
    *p;

  RunlengthPacket
    pixel;

  static char
    matte[MaxTextLength]="255";

  unsigned int
    height,
    mask,
    width;

  unsigned long
    state,
    x_factor,
    y_factor;

  Window
    xwindow;

  XEvent
    event;

  /*
    Map info window.
  */
  (void) sprintf(text," +%u+%u  ",windows->image.width,windows->image.height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Track pointer until button 1 is pressed.
  */
  cursor=XMakeCursor(display,windows->image.id,
    windows->image.map_info->colormap,resource_info->background_color,
    resource_info->foreground_color);
  XDefineCursor(display,windows->image.id,cursor);
  /*
    Track pointer until button 1 is pressed.
  */
  XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,&mask);
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask |
    PointerMotionMask);
  state=DefaultState;
  do
  {
    if (windows->info.mapped)
      {
        /*
          Display pointer position.
        */
        (void) sprintf(text," %+d%+d ",x-windows->image.x,y-windows->image.y);
        XDisplayInfoString(display,&windows->info,text);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (event.xbutton.window == windows->pan.id)
          {
            XPanImageWindow(display,windows,&event);
            XSetWindowExtents(display,&windows->info,text);
            break;
          }
        switch (event.xbutton.button)
        {
          case Button1:
          {
            /*
              Update matte data.
            */
            x_offset=event.xbutton.x;
            y_offset=event.xbutton.y;
            state|=UpdateConfigurationState;
            break;
          }
          case Button2:
          {
            /*
              Request matte value from the user.
            */
            XDialogWidget(display,windows,"Matte","Enter matte value:",matte);
            XDefineCursor(display,windows->image.id,cursor);
            break;
          }
        }
        break;
      }
      case Expose:
        break;
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        switch (key_symbol)
        {
          case XK_Escape:
          {
            /*
              Prematurely exit.
            */
            state|=ExitState;
            break;
          }
          case XK_F1:
          case XK_Help:
          {
            XTextViewWidget(display,windows,"Help Viewer - Matte Edit",
              ImageMatteEditHelp);
            break;
          }
          default:
          {
            XBell(display,0);
            break;
          }
        }
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending pointer motion events.
        */
        while (XCheckMaskEvent(display,PointerMotionMask,&event));
        x=event.xmotion.x;
        y=event.xmotion.y;
        /*
          Map and unmap info window as cursor crosses its boundaries.
        */
        if (windows->info.mapped)
          {
            if ((x < (windows->info.x+windows->info.width)) &&
                (y < (windows->info.y+windows->info.height)))
              XWithdrawWindow(display,windows->info.id,windows->info.screen);
          }
        else
          if ((x > (windows->info.x+windows->info.width)) ||
              (y > (windows->info.y+windows->info.height)))
            XMapWindow(display,windows->info.id);
        break;
      }
      default:
        break;
    }
    if (state & UpdateConfigurationState)
      {
        int
          x,
          y;

        if (image->class == PseudoClass)
          {
            /*
              Promote PseudoClass image to DirectClass & initialize matte data.
            */
            image->class=DirectClass;
            p=image->pixels;
            for (i=0; i < image->packets; i++)
            {
              p->index=0;
              p++;
            }
          }
        image->matte=True;
        /*
          Matte edit is relative to image configuration.
        */
        x=0;
        y=0;
        width=image->columns;
        height=image->rows;
        if (windows->image.clip_geometry != (char *) NULL)
          (void) XParseGeometry(windows->image.clip_geometry,&x,&y,
            &width,&height);
        x_factor=UpShift(width)/windows->image.ximage->width;
        x_offset=DownShift((windows->image.x+x_offset+1)*x_factor)+x;
        y_factor=UpShift(height)/windows->image.ximage->height;
        y_offset=DownShift((windows->image.y+y_offset+1)*y_factor)+y;
        /*
          Update matte information.
        */
        x=0;
        p=image->pixels;
        for (i=0; i < image->packets; i++)
        {
          x+=(p->length+1);
          if (x >= (y_offset*image->columns+x_offset))
            break;
          p++;
        }
        pixel=image->pixels[i];
        p=image->pixels;
        for (i=0; i < image->packets; i++)
        {
          if ((p->red == pixel.red) && (p->green == pixel.green) &&
              (p->blue == pixel.blue))
            p->index=atoi(matte) & 0xff;
          p++;
        }
        if ((resource_info->map_type != (char *) NULL) ||
            (windows->image.class == DirectColor) ||
            (windows->image.class == TrueColor))
          {
            /*
              Update X matte image.
            */
            XConfigureImageColormap(display,resource_info,windows,image);
            (void) XConfigureImageWindow(display,resource_info,windows,image);
          }
        state&=(~UpdateConfigurationState);
      }
  } while (!(state & ExitState));
  XSelectInput(display,windows->image.id,windows->image.attributes.event_mask);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XFreeCursor(display,cursor);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D e s p e c k l e I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XNoisyImageWindow reduces the speckles within an image.
%
%  The format of the XNoisyImageWindow routine is:
%
%    status=XNoisyImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XNoisyImageWindow return True if the edges within
%      the image are detected.  False is returned is there is a memory
%      shortage or if the edges cannot be detected.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XNoisyImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define NoisyImageText  "  Eliminating peak noise...  "

  Image
    *noisy_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,NoisyImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,NoisyImageText);
  /*
    Noisy image.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  noisy_image=NoisyImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (noisy_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=noisy_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X P a n I m a g e W i n d o w                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XPanImageWindow pans the image until the mouse button is released.
%
%  The format of the XPanImageWindow routine is:
%
%    XPanImageWindow(display,windows,event)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o event: Specifies a pointer to a XEvent structure.  If it is NULL,
%      the entire image is refreshed.
%
*/
static void XPanImageWindow(display,windows,event)
Display
  *display;

XWindows
  *windows;

XEvent
  *event;
{
  char
    text[MaxTextLength];

  Cursor
    cursor;

  unsigned long
    state;

  unsigned long
    x_factor,
    y_factor;

  XRectangle
    pan_info;

  /*
    Map info window.
  */
  (void) sprintf(text," %ux%u%+d%+d  ",windows->image.width,
    windows->image.height,windows->image.ximage->width,
    windows->image.ximage->height);
  XSetWindowExtents(display,&windows->info,text);
  XMapWindow(display,windows->info.id);
  /*
    Define cursor.
  */
  if ((windows->image.ximage->width > windows->image.width) &&
      (windows->image.ximage->height > windows->image.height))
    cursor=XCreateFontCursor(display,XC_fleur);
  else
    if (windows->image.ximage->width > windows->image.width)
      cursor=XCreateFontCursor(display,XC_sb_h_double_arrow);
    else
      if (windows->image.ximage->height > windows->image.height)
        cursor=XCreateFontCursor(display,XC_sb_v_double_arrow);
      else
        cursor=XCreateFontCursor(display,XC_arrow);
  XDefineCursor(display,windows->pan.id,cursor);
  /*
    Pan image as pointer moves until the mouse button is released.
  */
  x_factor=(unsigned long)
    UpShift(windows->image.ximage->width)/windows->pan.width;
  y_factor=(unsigned long)
    UpShift(windows->image.ximage->height)/windows->pan.height;
  pan_info.x=event->xbutton.x;
  pan_info.y=event->xbutton.y;
  pan_info.width=(unsigned short) (UpShift(windows->image.width)/x_factor);
  pan_info.height=(unsigned short) (UpShift(windows->image.height)/y_factor);
  windows->image.x=pan_info.x+1;
  windows->image.y=pan_info.y+1;
  state=UpdateConfigurationState;
  do
  {
    /*
      Wait for next event.
    */
    XIfEvent(display,event,XScreenEvent,(char *) windows);
    switch (event->type)
    {
      case ButtonRelease:
      {
        /*
          User has finished panning the image.
        */
        pan_info.x=event->xbutton.x;
        pan_info.y=event->xbutton.y;
        state|=ExitState | UpdateConfigurationState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,event));
        pan_info.x=event->xmotion.x;
        pan_info.y=event->xmotion.y;
        state|=UpdateConfigurationState;
      }
      default:
        break;
    }
    if (state & UpdateConfigurationState)
      {
        /*
          Check boundary conditions.
        */
        pan_info.x=DownShift((pan_info.x-(pan_info.width >> 1))*x_factor);
        if (pan_info.x < 0)
          pan_info.x=0;
        else
          if ((pan_info.x+windows->image.width) > windows->image.ximage->width)
            pan_info.x=windows->image.ximage->width-windows->image.width;
        pan_info.y=DownShift((pan_info.y-(pan_info.height >> 1))*y_factor);
        if (pan_info.y < 0)
          pan_info.y=0;
        else
          if ((pan_info.y+windows->image.height) >
               windows->image.ximage->height)
            pan_info.y=windows->image.ximage->height-windows->image.height;
        if ((windows->image.x != pan_info.x) ||
            (windows->image.y != pan_info.y))
          {
            /*
              Display image pan offset.
            */
            windows->image.x=pan_info.x;
            windows->image.y=pan_info.y;
            (void) sprintf(text," %ux%u%+d%+d ",windows->image.width,
              windows->image.height,windows->image.x,windows->image.y);
            XDisplayInfoString(display,&windows->info,text);
            /*
              Refresh image window.
            */
            XDrawPanRectangle(display,windows);
            XRefreshWindow(display,&windows->image,(XEvent *) NULL);
          }
        state&=(~UpdateConfigurationState);
      }
  } while (!(state & ExitState));
  /*
    Restore cursor.
  */
  XDefineCursor(display,windows->pan.id,windows->pan.cursor);
  XFreeCursor(display,cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X P r i n t I m a g e W i n d o w                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XPrintImageWindow prints an image to a Postscript printer.
%
%  The format of the XPrintImageWindow routine is:
%
%    status=XPrintImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XPrintImageWindow return True if the image is
%      printed.  False is returned is there is a memory shortage or if the
%      image fails to print.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XPrintImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define PrintImageText  "  Printing image...  "

  char
    command[MaxTextLength];

  Image
    *output_image;

  int
    status;

  /*
    Request file name from user.
  */
  (void) strcpy(command,resource_info->print_command);
  XDialogWidget(display,windows,"Print","Print command:",command);
  if (*command == '\0')
    return(True);
  /*
    Alert user we are busy.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XSetWindowExtents(display,&windows->info,PrintImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,PrintImageText);
  XFlush(display);
  /*
    Copy image before applying image transforms.
  */
  image->orphan=True;
  output_image=CopyImage(image,image->columns,image->rows,True);
  image->orphan=False;
  if (output_image == (Image *) NULL)
    {
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      return(False);
    }
  if ((windows->image.clip_geometry != (char *) NULL) ||
     (output_image->columns != windows->image.ximage->width) ||
     (output_image->rows != windows->image.ximage->height))
    {
      char
        image_geometry[MaxTextLength];

      /*
        Clip and/or scale image.
      */
      (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
        windows->image.ximage->height);
      TransformImage(&output_image,windows->image.clip_geometry,image_geometry);
    }
  if (resource_info->number_colors != 0)
    {
      /*
        Reduce the number of colors in the image.
      */
      if ((output_image->class == DirectClass) ||
          (output_image->colors > resource_info->number_colors) ||
          (resource_info->colorspace == GRAYColorspace))
        QuantizeImage(output_image,resource_info->number_colors,
          resource_info->tree_depth,resource_info->dither,
          resource_info->colorspace,True);
      SyncImage(output_image);
    }
  (void) sprintf(output_image->filename,"ps:|%s",command);
  status=WriteImage(resource_info->image_info,output_image);
  DestroyImage(output_image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  return(status);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X R o t a t e I m a g e W i n d o w                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XRotateImageWindow rotates the X image.  If the degrees parameter
%  if zero, the rotation angle is computed from the slope of a line drawn by
%  the user.
%
%  The format of the XRotateImageWindow routine is:
%
%    status=XRotateImageWindow(display,resource_info,windows,degrees,image)
%
%  A description of each parameter follows:
%
%    o status: Function XRotateImageWindow return True if the image is
%      rotated.  False is returned is there is a memory shortage or if the
%      image fails to rotate.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o degrees: Specifies the number of degrees to rotate the image.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XRotateImageWindow(display,resource_info,windows,degrees,
  image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

double
  degrees;

Image
  **image;
{
#define RotateImageText  "  Rotating image...  "

  static char
    *ImageRotateHelp[]=
    {
      "A small horizontal line is drawn next to the pointer.  You",
      "are now in rotation mode. To exit immediately, press any",
      "button and release.  In rotation mode a button press has a",
      "different effect than described in BUTTONS.  Press a button",
      "to affect this behavior:",
      "",
      "1    Choose a point in the image window and press this",
      "     button and hold.  Next, move the pointer to another",
      "     location in the image.  As you move a line connects the",
      "     initial location and the pointer.  When you release the",
      "     button, the degree of image rotation is determined by",
      "     the slope of the line you just drew.",
      "",
      "2    Press and drag to select a background color from a",
      "     pop-up menu.  Choose from these background colors:",
      "",
      "         black",
      "         blue",
      "         cyan",
      "         green",
      "         gray",
      "         red",
      "         magenta",
      "         yellow",
      "         white",
      "         Browser...",
      "",
      "    If you select the color browser and press Grab, you can",
      "    choose the font color by moving the pointer to the",
      "    desired color on the screen and press any button.",
      "",
      "To cancel the image rotation, move the pointer back to the",
      "starting point of the line and release the button.",
      (char *) NULL,
    };

  char
    text[MaxTextLength];

  ColorPacket
    background;

  double
    normalized_degrees;

  Image
    *rotated_image;

  int
    i,
    x,
    y;

  static unsigned int
    pen_id=0;

  unsigned int
    height,
    rotations,
    status,
    width;

  if (degrees == 0.0)
    {
      unsigned int
        distance,
        mask;

      unsigned long
        state;

      Window
        xwindow;

      XEvent
        event;

      XSegment
        rotate_info;

      /*
        Wait for first button press.
      */
      XSetFunction(display,windows->image.highlight_context,GXinvert);
      XSelectInput(display,windows->image.id,
        windows->image.attributes.event_mask | PointerMotionMask);
      XQueryPointer(display,windows->image.id,&xwindow,&xwindow,&i,&i,&x,&y,
        &mask);
      rotate_info.x1=x;
      rotate_info.y1=y;
      rotate_info.x2=rotate_info.x1+32;
      rotate_info.y2=rotate_info.y1;
      state=DefaultState;
      do
      {
        XHighlightLine(display,windows->image.id,
          windows->image.highlight_context,&rotate_info);
        /*
          Wait for next event.
        */
        XIfEvent(display,&event,XScreenEvent,(char *) windows);
        XHighlightLine(display,windows->image.id,
          windows->image.highlight_context,&rotate_info);
        switch (event.type)
        {
          case ButtonPress:
          {
            if (event.xbutton.window == windows->pan.id)
              {
                XPanImageWindow(display,windows,&event);
                XSetWindowExtents(display,&windows->info,text);
                break;
              }
            XSetFunction(display,windows->image.highlight_context,GXcopy);
            switch (event.xbutton.button)
            {
              case Button1:
              {
                /*
                  Exit loop.
                */
                rotate_info.x1=event.xbutton.x;
                rotate_info.y1=event.xbutton.y;
                state|=ExitState;
                break;
              }
              case Button2:
              {
                char
                  color_name[MaxTextLength],
                  *ColorMenu[MaxNumberPens+1];

                int
                  pen_number;

                /*
                  Initialize menu selections.
                */
                for (i=0; i < MaxNumberPens; i++)
                  ColorMenu[i]=resource_info->pen_color[i];
                ColorMenu[MaxNumberPens-1]="Browser...";
                ColorMenu[MaxNumberPens]=(char *) NULL;
                /*
                  Select a pen color from the pop-up menu.
                */
                pen_number=XMenuWidget(display,windows,"Pixel Color",ColorMenu,
                  color_name);
                if (pen_number == (MaxNumberPens-1))
                  {
                    static char
                      color_name[MaxTextLength]="gray";

                    /*
                      Select a pen color from a dialog.
                    */
                    resource_info->pen_color[MaxNumberPens-1]=color_name;
                    XColorBrowserWidget(display,windows,"Select",color_name);
                    if (*color_name == '\0')
                      break;
                  }
                if (pen_number >= 0)
                  {
                    /*
                      Set pen color.
                    */
                    pen_id=pen_number;
                    (void) strcpy(color_name,resource_info->pen_color[pen_id]);
                    status=XParseColor(display,windows->image.map_info->colormap,
                      color_name,&windows->image.pixel_info->pen_color[pen_id]);
                    if (status == 0)
                      XNoticeWidget(display,windows,
                        "Color is unknown to X server",color_name);
                    XBestPixel(display,windows->image.map_info->colormap,
                      (XColor *) NULL,(unsigned int)
                      Min(windows->image.visual_info->colormap_size,256),
                      &windows->image.pixel_info->pen_color[pen_id]);
                  }
                break;
              }
            }
            XSetFunction(display,windows->image.highlight_context,GXinvert);
            break;
          }
          case Expose:
            break;
          case KeyPress:
          {
            static char
              command[MaxTextLength];

            static KeySym
              key_symbol;

            /*
              Respond to a user key press.
            */
            (void) XLookupString((XKeyEvent *) &event.xkey,command,
              sizeof(command),&key_symbol,(XComposeStatus *) NULL);
            switch (key_symbol)
            {
              case XK_Escape:
              {
                /*
                  Prematurely exit.
                */
                state|=EscapeState;
                state|=ExitState;
                break;
              }
              case XK_F1:
              case XK_Help:
              {
                XSetFunction(display,windows->image.highlight_context,GXcopy);
                XTextViewWidget(display,windows,"Help Viewer - Image Rotation",
                  ImageRotateHelp);
                XSetFunction(display,windows->image.highlight_context,GXinvert);
                break;
              }
              default:
              {
                XBell(display,0);
                break;
              }
            }
            break;
          }
          case MotionNotify:
          {
            /*
              Discard pending pointer motion events.
            */
            while (XCheckMaskEvent(display,PointerMotionMask,&event));
            rotate_info.x1=event.xmotion.x;
            rotate_info.y1=event.xmotion.y;
          }
        }
        rotate_info.x2=rotate_info.x1+32;
        rotate_info.y2=rotate_info.y1;
      } while (!(state & ExitState));
      XSelectInput(display,windows->image.id,
        windows->image.attributes.event_mask);
      XSetFunction(display,windows->image.highlight_context,GXcopy);
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      if (state & EscapeState)
        return(True);
      /*
        Draw line as pointer moves until the mouse button is released.
      */
      distance=0;
      (void) sprintf(text," %.2f",-180.0);
      XSetWindowExtents(display,&windows->info,text);
      XSetFunction(display,windows->image.highlight_context,GXinvert);
      state=DefaultState;
      do
      {
        if (distance > 9)
          {
            /*
              Display info and draw rotation line.
            */
            if (!windows->info.mapped)
              XMapWindow(display,windows->info.id);
            (void) sprintf(text," %.2f",degrees);
            XDisplayInfoString(display,&windows->info,text);
            XHighlightLine(display,windows->image.id,
              windows->image.highlight_context,&rotate_info);
          }
        else
          if (windows->info.mapped)
            XWithdrawWindow(display,windows->info.id,windows->info.screen);
        /*
          Wait for next event.
        */
        XIfEvent(display,&event,XScreenEvent,(char *) windows);
        if (distance > 9)
          XHighlightLine(display,windows->image.id,
            windows->image.highlight_context,&rotate_info);
        switch (event.type)
        {
          case ButtonPress:
            break;
          case ButtonRelease:
          {
            /*
              User has committed to rotation line.
            */
            rotate_info.x2=event.xbutton.x;
            rotate_info.y2=event.xbutton.y;
            state|=ExitState;
            break;
          }
          case Expose:
            break;
          case MotionNotify:
          {
            /*
              Discard pending button motion events.
            */
            while (XCheckMaskEvent(display,ButtonMotionMask,&event));
            rotate_info.x2=event.xmotion.x;
            rotate_info.y2=event.xmotion.y;
          }
          default:
            break;
        }
        /*
          Check boundary conditions.
        */
        if (rotate_info.x2 < 0)
          rotate_info.x2=0;
        else
          if (rotate_info.x2 > windows->image.width)
            rotate_info.x2=windows->image.width;
        if (rotate_info.y2 < 0)
          rotate_info.y2=0;
        else
          if (rotate_info.y2 > windows->image.height)
            rotate_info.y2=windows->image.height;
        /*
          Compute rotation angle from the slope of the line.
        */
        degrees=0.0;
        distance=
          ((rotate_info.x2-rotate_info.x1+1)*(rotate_info.x2-rotate_info.x1+1))+
          ((rotate_info.y2-rotate_info.y1+1)*(rotate_info.y2-rotate_info.y1+1));
        if (distance > 9)
          degrees=RadiansToDegrees(-atan2((double) (rotate_info.y2-
            rotate_info.y1),(double) (rotate_info.x2-rotate_info.x1)));
      } while (!(state & ExitState));
      XSetFunction(display,windows->image.highlight_context,GXcopy);
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      if (distance <= 9)
        return(True);
    }
  if (degrees == 0.0)
    return(True);
  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,RotateImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,RotateImageText);
  /*
    Rotate image.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  normalized_degrees=degrees;
  while (normalized_degrees < -45.0)
    normalized_degrees+=360.0;
  for (rotations=0; normalized_degrees > 45.0; rotations++)
    normalized_degrees-=90.0;
  if (normalized_degrees != 0.0)
    if ((windows->image.clip_geometry != (char *) NULL) ||
        ((*image)->columns != windows->image.ximage->width) ||
        ((*image)->rows != windows->image.ximage->height))
      {
        char
          image_geometry[MaxTextLength];

        /*
          Update image with user transforms.
        */
        (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
          windows->image.ximage->height);
        TransformImage(image,windows->image.clip_geometry,image_geometry);
        if (windows->image.clip_geometry != (char *) NULL)
          {
            (void) free((char *) windows->image.clip_geometry);
            windows->image.clip_geometry=(char *) NULL;
          }
        windows->image.x=0;
        windows->image.y=0;
      }
  background.red=windows->image.pixel_info->pen_color[pen_id].red >> 8;
  background.green=windows->image.pixel_info->pen_color[pen_id].green >> 8;
  background.blue=windows->image.pixel_info->pen_color[pen_id].blue >> 8;
  rotated_image=RotateImage(*image,degrees,&background,False);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (rotated_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=rotated_image;
  if (windows->image.clip_geometry != (char *) NULL)
    {
      /*
        Rotate clip geometry.
      */
      (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
      switch (rotations % 4)
      {
        default:
        case 0:
          break;
        case 1:
        {
          /*
            Rotate 90 degrees.
          */
          (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",
            height,width,(int) (*image)->columns-(int) height-y,x);
          break;
        }
        case 2:
        {
          /*
            Rotate 180 degrees.
          */
          (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",
            width,height,(int) width-x,(int) height-y);
          break;
        }
        case 3:
        {
          /*
            Rotate 270 degrees.
          */
          (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",
            height,width,y,(int) (*image)->rows-(int) width-x);
          break;
        }
      }
    }
  if (normalized_degrees != 0.0)
    {
      /*
        Update image colormap.
      */
      windows->image.window_changes.width=(*image)->columns;
      windows->image.window_changes.height=(*image)->rows;
      if (windows->image.clip_geometry != (char *) NULL)
        {
          /*
            Obtain dimensions of image from clip geometry.
          */
          (void) XParseGeometry(windows->image.clip_geometry,&x,&y,
            &width,&height);
          windows->image.window_changes.width=width;
          windows->image.window_changes.height=height;
        }
      XConfigureImageColormap(display,resource_info,windows,*image);
    }
  else
    if (((rotations % 4) == 1) || ((rotations % 4) == 3))
      {
        windows->image.window_changes.width=windows->image.ximage->height;
        windows->image.window_changes.height=windows->image.ximage->width;
      }
  /*
    Update image configuration.
  */
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S h a r p e n I m a g e W i n d o w                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XSharpenImageWindow sharpens an image.
%
%  The format of the XSharpenImageWindow routine is:
%
%    status=XSharpenImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XSharpenImageWindow return True if the edges within
%      the image are detected.  False is returned is there is a memory
%      shortage or if the edges cannot be detected.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XSharpenImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define SharpenImageText  "  Sharpening image...  "

  Image
    *sharpened_image;

  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,SharpenImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,SharpenImageText);
  /*
    Sharpen image scanlines.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  sharpened_image=SharpenImage(*image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (sharpened_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=sharpened_image;
  /*
    Update image configuration.
  */
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S h e a r I m a g e W i n d o w                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XShearImageWindow shears the X image.
%
%  The format of the XShearImageWindow routine is:
%
%    status=XShearImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XShearImageWindow return True if the image is
%      rotated.  False is returned is there is a memory shortage or if the
%      image fails to rotate.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XShearImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  **image;
{
#define ShearImageText  "  Shearing image...  "

  ColorPacket
    border_color;

  float
    x_shear,
    y_shear;

  Image
    *sheared_image;

  static char
    color_name[MaxTextLength] = "gray",
    geometry[MaxTextLength] = "10.0";

  XColor
    color;

  /*
    Get color of empty triangles.
  */
  XColorBrowserWidget(display,windows,"Select",color_name);
  if (*color_name == '\0')
    return(True);
  XCheckRefreshWindow(display,&windows->image);
  XDialogWidget(display,windows,"Shear","Enter shear geometry:",geometry);
  if (*geometry == '\0')
    return(True);
  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,ShearImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,ShearImageText);
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XCheckRefreshWindow(display,&windows->image);
  if ((windows->image.clip_geometry != (char *) NULL) ||
      ((*image)->columns != windows->image.ximage->width) ||
      ((*image)->rows != windows->image.ximage->height))
    {
      char
        image_geometry[MaxTextLength];
  
      /*
        Update image with user transforms.
      */
      (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
        windows->image.ximage->height);
      TransformImage(image,windows->image.clip_geometry,image_geometry);
      if (windows->image.clip_geometry != (char *) NULL)
        {
          (void) free((char *) windows->image.clip_geometry);
          windows->image.clip_geometry=(char *) NULL;
        }
      windows->image.x=0;
      windows->image.y=0;
    }
  /*
    Add a border to the image.
  */
  x_shear=0.0;
  y_shear=0.0;
  (void) sscanf(geometry,"%fx%f",&x_shear,&y_shear);
  (void) XParseColor(display,windows->image.map_info->colormap,color_name,
    &color);
  border_color.red=color.red >> 8;
  border_color.green=color.green >> 8;
  border_color.blue=color.blue >> 8;
  sheared_image=
    ShearImage(*image,(double) x_shear,(double) y_shear,&border_color,False);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (sheared_image == (Image *) NULL)
    return(False);
  DestroyImage(*image);
  *image=sheared_image;
  windows->image.window_changes.width=(*image)->columns;
  windows->image.window_changes.height=(*image)->rows;
  XConfigureImageColormap(display,resource_info,windows,*image);
  (void) XConfigureImageWindow(display,resource_info,windows,*image);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S c r e e n E v e n t                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XScreenEvent returns True if the certain events on the X server
%  queue is associated with the image or magnify window.
%
%  The format of the XScreenEvent function is:
%
%      XScreenEvent(display,event,data)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o event: Specifies a pointer to a X11 XEvent structure.
%
%    o data: Specifies a pointer to a XWindows structure.
%
%
*/
static int XScreenEvent(display,event,data)
Display
  *display;

XEvent
  *event;

char
  *data;
{
  register int
    x,
    y;

  register XWindows
    *windows;

  windows=(XWindows *) data;
  switch (event->type)
  {
    case ButtonPress:
    {
      if ((event->xbutton.button == Button3) &&
          (event->xbutton.state & Mod1Mask))
        {
          /*
            Convert Alt-Button3 to Button2.
          */
          event->xbutton.button=Button2;
          event->xbutton.state&=(~Mod1Mask);
        }
      if (event->xbutton.window == windows->backdrop.id)
        XSetInputFocus(display,event->xbutton.window,RevertToParent,
          event->xbutton.time);
      return(True);
    }
    case ButtonRelease:
    {
      if (event->xbutton.window == windows->image.id)
        if (windows->magnify.mapped)
          {
            /*
              Update magnified image.
            */
            x=event->xbutton.x;
            y=event->xbutton.y;
            if (x < 0)
              x=0;
            else
              if (x >= windows->image.width)
                x=windows->image.width-1;
            windows->magnify.x=windows->image.x+x;
            if (y < 0)
              y=0;
            else
             if (y >= windows->image.height)
               y=windows->image.height-1;
            windows->magnify.y=windows->image.y+y;
            XMakeMagnifyImage(display,windows);
          }
      return(True);
    }
    case ClientMessage:
    {
      /*
        If client window delete message, exit.
      */
      if (event->xclient.message_type != windows->wm_protocols)
        break;
      if (*event->xclient.data.l == windows->wm_take_focus)
        {
          XSetInputFocus(display,event->xclient.window,RevertToParent,
            event->xclient.data.l[1]);
          return(True);
        }
      if (*event->xclient.data.l != windows->wm_delete_window)
        break;
      if (event->xclient.window == windows->magnify.id)
        {
          XWithdrawWindow(display,windows->magnify.id,windows->magnify.screen);
          return(True);
        }
      break;
    }
    case Expose:
    {
      if (event->xexpose.window == windows->image.id)
        {
          XRefreshWindow(display,&windows->image,event);
          return(True);
        }
      if (event->xexpose.window == windows->pan.id)
        if (event->xexpose.count == 0)
          {
            XDrawPanRectangle(display,windows);
            return(True);
          }
      break;
    }
    case KeyPress:
    {
      static char
        command[MaxTextLength];

      static KeySym
        key_symbol;

      /*
        Respond to a user key press.
      */
      (void) XLookupString((XKeyEvent *) &event->xkey,command,sizeof(command),
        &key_symbol,(XComposeStatus *) NULL);
      if (event->xkey.window == windows->magnify.id)
        XMagnifyWindowCommand(display,windows,key_symbol);
      return(True);
    }
    case MapNotify:
    {
      if (event->xmap.window == windows->magnify.id)
        {
          XMakeMagnifyImage(display,windows);
          windows->magnify.mapped=True;
          return(True);
        }
      if (event->xmap.window == windows->info.id)
        {
          windows->info.mapped=True;
          return(True);
        }
      break;
    }
    case MotionNotify:
    {
      if (event->xmotion.window == windows->image.id)
        if (windows->magnify.mapped)
          {
            /*
              Update magnified image.
            */
            x=event->xmotion.x;
            y=event->xmotion.y;
            if (x < 0)
              x=0;
            else
              if (x >= windows->image.width)
                x=windows->image.width-1;
            windows->magnify.x=windows->image.x+x;
            if (y < 0)
              y=0;
            else
             if (y >= windows->image.height)
               y=windows->image.height-1;
            windows->magnify.y=windows->image.y+y;
            XMakeMagnifyImage(display,windows);
          }
      return(True);
    }
    case UnmapNotify:
    {
      if (event->xunmap.window == windows->magnify.id)
        {
          windows->magnify.mapped=False;
          return(True);
        }
      if (event->xunmap.window == windows->info.id)
        {
          windows->info.mapped=False;
          return(True);
        }
      break;
    }
    case KeyRelease:
    case SelectionNotify:
      return(True);
    default:
      break;
  }
  return(False);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S e t C l i p G e o m e t r y                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XSetClipGeometry accepts a clipping geometry relative to the
%  image window and translates it to a clipping geometry relative to the
%  image.
%
%  The format of the XSetClipGeometry routine is:
%
%    XSetClipGeometry(display,windows,clip_info,image)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o clip_info:  A pointer to a RectangleInfo that defines a region of the
%      image window to clip.
%
%    o image: Specifies a pointer to a Image structure.
%
%
*/
static void XSetClipGeometry(display,windows,clip_info,image)
Display
  *display;

XWindows
  *windows;

RectangleInfo
  *clip_info;

Image
  *image;
{
  char
    text[MaxTextLength];

  int
    x,
    y;

  unsigned int
    height,
    width;

  unsigned long
    scale_factor;

  if (windows->info.mapped)
    {
      /*
        Display info on clipping rectangle.
      */
      (void) sprintf(text," %ux%u+%u+%u ",windows->image.width,
        windows->image.height,windows->image.width,windows->image.height);
      XSetWindowExtents(display,&windows->info,text);
      XMapWindow(display,windows->info.id);
      (void) sprintf(text," %ux%u%+d%+d",clip_info->width,clip_info->height,
        clip_info->x,clip_info->y);
      XDisplayInfoString(display,&windows->info,text);
    }
  /*
    Clipping geometry is relative to any previous clip geometry.
  */
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  if (windows->image.clip_geometry != (char *) NULL)
    (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
  else
    {
      /*
        Allocate clip geometry string.
      */
      windows->image.clip_geometry=(char *) malloc(MaxTextLength*sizeof(char));
      if (windows->image.clip_geometry == (char *) NULL)
        Error("Unable to clip X image",windows->image.name);
    }
  /*
    Define the clip geometry string from the clipping rectangle.
  */
  scale_factor=UpShift(width)/windows->image.ximage->width;
  clip_info->x+=windows->image.x;
  if (clip_info->x > 0)
    x+=DownShift(clip_info->x*scale_factor);
  width=DownShift(clip_info->width*scale_factor);
  if (width == 0)
    width=1;
  scale_factor=UpShift(height)/windows->image.ximage->height;
  clip_info->y+=windows->image.y;
  if (clip_info->y > 0)
    y+=DownShift(clip_info->y*scale_factor);
  height=DownShift(clip_info->height*scale_factor);
  if (height == 0)
    height=1;
  (void) sprintf(windows->image.clip_geometry,"%ux%u%+d%+d",width,height,x,y);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X T i l e I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XTileImageWindow determines which individual tile of a composite
%  image was choosen with a button press and then displays it.
%
%  The format of the XTileImageWindow routine is:
%
%    tiled_image=XTileImageWindow(display,resource_info,windows,image,event)
%
%  A description of each parameter follows:
%
%    o tiled_image:  XTileImageWindow reads the tiled image and returns
%      it.  A null image is returned if an error occurs.
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure; returned from
%      ReadImage.
%
%    o event: Specifies a pointer to a XEvent structure.  If it is NULL,
%      the entire image is refreshed.
%
%
*/
static Image *XTileImageWindow(display,resource_info,windows,image,event)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;

XEvent
  *event;
{
#define TileImageText  "  Loading image...  "

  char
    filename[MaxTextLength];

  Image
    *tiled_image;

  int
    tile,
    x,
    y;

  register char
    *p,
    *q;

  unsigned int
    height,
    width;

  unsigned long
    scale_factor;

  /*
    Tile image is relative to composite image configuration.
  */
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  if (windows->image.clip_geometry != (char *) NULL)
    (void) XParseGeometry(windows->image.clip_geometry,&x,&y,&width,&height);
  scale_factor=UpShift(width)/windows->image.ximage->width;
  event->xbutton.x+=windows->image.x;
  event->xbutton.x=DownShift(event->xbutton.x*scale_factor)+x;
  scale_factor=UpShift(height)/windows->image.ximage->height;
  event->xbutton.y+=windows->image.y;
  event->xbutton.y=DownShift(event->xbutton.y*scale_factor)+y;
  /*
    Determine size and location of individual tiles of the composite.
  */
  x=0;
  y=0;
  width=image->columns;
  height=image->rows;
  (void) XParseGeometry(image->montage,&x,&y,&width,&height);
  tile=((event->xbutton.y-y)/height)*((image->columns-x)/width)+
    (event->xbutton.x-x)/width;
  if (tile < 0)
    {
      /*
        Button press is outside any tile.
      */
      XBell(display,0);
      return((Image *) NULL);
    }
  /*
    Determine file name from the tile directory.
  */
  p=image->directory;
  while ((tile != 0) && (*p != '\0'))
  {
    if (*p == '\n')
      tile--;
    p++;
  }
  if (*p == '\0')
    {
      /*
        Button press is outside any tile.
      */
      XBell(display,0);
      return((Image *) NULL);
    }
  q=p;
  while ((*q != '\n') && (*q != '\0'))
    q++;
  (void) strncpy(filename,p,q-p);
  filename[q-p]='\0';
  /*
    Map info window.
  */
  XSetWindowExtents(display,&windows->info,TileImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,TileImageText);
  /*
    Load tile image.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  (void) strcpy(resource_info->image_info->filename,filename);
  tiled_image=ReadImage(resource_info->image_info);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  if (tiled_image == (Image *) NULL)
    XNoticeWidget(display,windows,"Unable to load image",filename);
  return(tiled_image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X T r a n s l a t e I m a g e W i n d o w                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XTranslateImageWindow translates the image within an image window
%  by one pixel as specified by the key symbol.  If the image has a `montage'
%  string the translation is respect to the width and height contained within
%  the string.
%
%  The format of the XTranslateImageWindow routine is:
%
%    XTranslateImageWindow(display,windows,image,key_symbol)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%    o key_symbol: Specifies a KeySym which indicates which side of the image
%      to trim.
%
%
*/
static void XTranslateImageWindow(display,windows,image,key_symbol)
Display
  *display;

XWindows
  *windows;

Image
  *image;

KeySym
  key_symbol;
{
  int
    x,
    y;

  unsigned int
    x_offset,
    y_offset;

  /*
    User specified a pan position offset.
  */
  x_offset=1;
  y_offset=1;
  if (image->montage != (char *) NULL)
    (void) XParseGeometry(image->montage,&x,&y,&x_offset,&y_offset);
  switch (key_symbol)
  {
    case XK_Home:
    case XK_KP_Home:
    {
      windows->image.x=windows->image.width >> 1;
      windows->image.y=windows->image.height >> 1;
      break;
    }
    case XK_Left:
    case XK_KP_Left:
    {
      windows->image.x-=x_offset;
      break;
    }
    case XK_Up:
    case XK_KP_Up:
    {
      windows->image.y-=y_offset;
      break;
    }
    case XK_Right:
    case XK_KP_Right:
    {
      windows->image.x+=x_offset;
      break;
    }
    case XK_Down:
    case XK_KP_Down:
    {
      windows->image.y+=y_offset;
      break;
    }
    default:
      break;
  }
  /*
    Check boundary conditions.
  */
  if (windows->image.x < 0)
    windows->image.x=0;
  else
    if ((windows->image.x+windows->image.width) > windows->image.ximage->width)
      windows->image.x=windows->image.ximage->width-windows->image.width;
  if (windows->image.y < 0)
    windows->image.y=0;
  else
    if ((windows->image.y+windows->image.height) > windows->image.ximage->height)
      windows->image.y=windows->image.ximage->height-windows->image.height;
  /*
    Refresh image window.
  */
  XDrawPanRectangle(display,windows);
  XRefreshWindow(display,&windows->image,(XEvent *) NULL);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X T r i m I m a g e W i n d o w                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XTrimImageWindow trims the edges from the image window.
%
%  The format of the XTrimImageWindow routine is:
%
%    status=XTrimImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XTrimImageWindow returns True if the image is
%      clipped.  False is returned is there is a memory shortage or if the
%      image fails to be cropped.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure.
%
%
*/
static unsigned int XTrimImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define TrimImageText  "  Trimming image...  "

  RectangleInfo
    trim_info;

  register int
    x,
    y;

  unsigned long
    background,
    pixel;

  /*
    Alert user we are busy.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XSetWindowExtents(display,&windows->info,TrimImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,TrimImageText);
  XFlush(display);
  /*
    Trim edges from image.
  */
  background=XGetPixel(windows->image.ximage,0,0);
  trim_info.width=windows->image.ximage->width;
  for (x=0; x < windows->image.ximage->width; x++)
  {
    for (y=0; y < windows->image.ximage->height; y++)
    {
      pixel=XGetPixel(windows->image.ximage,x,y);
      if (pixel != background)
        break;
    }
    if (y < windows->image.ximage->height)
      break;
  }
  trim_info.x=x;
  if (trim_info.x == windows->image.ximage->width)
    {
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      return(False);
    }
  for (x=windows->image.ximage->width-1; x > 0; x--)
  {
    for (y=0; y < windows->image.ximage->height; y++)
    {
      pixel=XGetPixel(windows->image.ximage,x,y);
      if (pixel != background)
        break;
    }
    if (y < windows->image.ximage->height)
      break;
  }
  trim_info.width=x-trim_info.x+1;
  trim_info.height=windows->image.ximage->height;
  for (y=0; y < windows->image.ximage->height; y++)
  {
    for (x=0; x < windows->image.ximage->width; x++)
    {
      pixel=XGetPixel(windows->image.ximage,x,y);
      if (pixel != background)
        break;
    }
    if (x < windows->image.ximage->width)
      break;
  }
  trim_info.y=y;
  for (y=windows->image.ximage->height-1; y > 0; y--)
  {
    for (x=0; x < windows->image.ximage->width; x++)
    {
      pixel=XGetPixel(windows->image.ximage,x,y);
      if (pixel != background)
        break;
    }
    if (x < windows->image.ximage->width)
      break;
  }
  trim_info.height=y-trim_info.y+1;
  if ((trim_info.width != windows->image.width) ||
      (trim_info.height != windows->image.height))
    {
      /*
        Reconfigure image window as defined by the trimming rectangle.
      */
      XSetClipGeometry(display,windows,&trim_info,image);
      windows->image.window_changes.width=trim_info.width;
      windows->image.window_changes.height=trim_info.height;
      (void) XConfigureImageWindow(display,resource_info,windows,image);
    }
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  return(True);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X W r i t e I m a g e W i n d o w                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XWriteImageWindow writes an image to a file.
%
%  The format of the XWriteImageWindow routine is:
%
%    status=XWriteImageWindow(display,resource_info,windows,image)
%
%  A description of each parameter follows:
%
%    o status: Function XWriteImageWindow return True if the image is
%      written.  False is returned is there is a memory shortage or if the
%      image fails to write.
%
%    o display: Specifies a connection to an X server; returned from
%      XOpenDisplay.
%
%    o resource_info: Specifies a pointer to a X11 XResourceInfo structure.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o image: Specifies a pointer to a Image structure;  returned from
%      ReadImage.
%
%
*/
static unsigned int XWriteImageWindow(display,resource_info,windows,image)
Display
  *display;

XResourceInfo
  *resource_info;

XWindows
  *windows;

Image
  *image;
{
#define WriteImageText  "  Writing image...  "

  char
    filename[MaxTextLength];

  Image
    *write_image;

  ImageInfo
    *image_info;

  int
    status;

  /*
    Request file name from user.
  */
  (void) strcpy(filename,image->filename);
  if (resource_info->write_filename != (char *) NULL)
    (void) strcpy(filename,resource_info->write_filename);
  XFileBrowserWidget(display,windows,"Write",filename);
  if (*filename == '\0')
    return(True);
  image_info=resource_info->image_info;
  (void) strcpy(image_info->filename,filename);
  SetImageMagick(resource_info->image_info);
  if ((strcmp(image_info->magick,"JPEG") == 0) ||
      (strcmp(image_info->magick,"JPG") == 0))
    {
      char
        quality[MaxTextLength];

      /*
        Request JPEG quality from user.
      */
      (void) sprintf(quality,"%u",image_info->quality);
      XDialogWidget(display,windows,"Write","Enter JPEG quality:",quality);
      if (*quality != '\0')
        image_info->quality=atoi(quality);
    }
  if ((strcmp(image_info->magick,"EPS") == 0) ||
      (strcmp(image_info->magick,"PS") == 0) ||
      (strcmp(image_info->magick,"PS2") == 0))
    {
      char
        geometry[MaxTextLength];

      /*
        Request Postscript page geometry from user.
      */
      (void) sprintf(geometry,PSPageGeometry);
      if (image_info->page != (char *) NULL)
        (void) strcpy(geometry,image_info->page);
      XDialogWidget(display,windows,"Write","Enter Postscript page geometry:",
        geometry);
      if (*geometry != '\0')
        {
          image_info->page=(char *) malloc((strlen(geometry)+1)*sizeof(char));
          if (image_info->page != (char *) NULL)
            (void) sprintf(image_info->page,geometry);
        }
    }
  /*
    Alert user we are busy.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XSetWindowExtents(display,&windows->info,WriteImageText);
  XMapWindow(display,windows->info.id);
  XDisplayInfoString(display,&windows->info,WriteImageText);
  XCheckRefreshWindow(display,&windows->image);
  /*
    Copy image before applying image transforms.
  */
  image->orphan=True;
  write_image=CopyImage(image,image->columns,image->rows,True);
  image->orphan=False;
  if (write_image == (Image *) NULL)
    {
      XDefineCursor(display,windows->image.id,windows->image.cursor);
      XWithdrawWindow(display,windows->info.id,windows->info.screen);
      return(False);
    }
  if ((windows->image.clip_geometry != (char *) NULL) ||
     (write_image->columns != windows->image.ximage->width) ||
     (write_image->rows != windows->image.ximage->height))
    {
      char
        image_geometry[MaxTextLength];

      /*
        Clip and/or scale image.
      */
      (void) sprintf(image_geometry,"%dx%d",windows->image.ximage->width,
        windows->image.ximage->height);
      TransformImage(&write_image,windows->image.clip_geometry,image_geometry);
    }
  if (resource_info->number_colors != 0)
    {
      /*
        Reduce the number of colors in the image.
      */
      if ((write_image->class == DirectClass) ||
          (write_image->colors > resource_info->number_colors) ||
          (resource_info->colorspace == GRAYColorspace))
        QuantizeImage(write_image,resource_info->number_colors,
          resource_info->tree_depth,resource_info->dither,
          resource_info->colorspace,True);
      SyncImage(write_image);
    }
  (void) strcpy(write_image->filename,filename);
  status=WriteImage(image_info,write_image);
  DestroyImage(write_image);
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->info.id,windows->info.screen);
  return(status);
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
#define NotInitialized  (unsigned int) (~0)

  char
    *comment,
    *density,
    *label,
    *option,
    *page_geometry,
    *resource_value,
    *server_name;

  ColorPacket
    border_color;

  Display
    *display;

  Image
    *image,
    *next_image;

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
    compression,
    first_scene,
    *image_marker,
    image_number,
    interlace,
    last_scene,
    matte,
    quality,
    scene,
    verbose;

  unsigned long
    state;

  XColor
    color;

  XResourceInfo
    resource_info;

  XrmDatabase
    resource_database,
    server_database;

  /*
    Set defaults.
  */
  client_name=(*argv);
  display=(Display *) NULL;
  first_scene=0;
  image_marker=(unsigned int *) malloc((argc+1)*sizeof(unsigned int));
  if (image_marker == (unsigned int *) NULL)
    Error("Unable to display image","Memory allocation failed");
  for (i=0; i <= argc; i++)
    image_marker[i]=argc;
  image_number=0;
  matte=NotInitialized;
  last_scene=0;
  resource_database=(XrmDatabase) NULL;
  server_name=(char *) NULL;
  state=DefaultState;
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
          i++;
          if (i == argc)
            Error("Missing server name on -display",(char *) NULL);
          server_name=argv[i];
          break;
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
  (void) XParseColor(display,XDefaultColormap(display,XDefaultScreen(display)),
    resource_info.border_color,&color);
  border_color.red=color.red >> 8;
  border_color.green=color.green >> 8;
  border_color.blue=color.blue >> 8;
  resource_value=XGetResourceClass(resource_database,client_name,"compression",
    "RunlengthEncoded");
  if (Latin1Compare("qencoded",resource_value) == 0)
    compression=QEncodedCompression;
  else
    compression=RunlengthEncodedCompression;
  comment=
    XGetResourceClass(resource_database,client_name,"comment",(char *) NULL);
  density=
    XGetResourceClass(resource_database,client_name,"density",(char *) NULL);
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
  label=XGetResourceClass(resource_database,client_name,"label",(char *) NULL);
  page_geometry=XGetResourceClass(resource_database,client_name,"pageGeometry",
    (char *) NULL);
  resource_value=
    XGetResourceClass(resource_database,client_name,"quality","85");
  quality=atoi(resource_value);
  resource_value=XGetResourceClass(resource_database,client_name,"scene","0");
  scene=atoi(resource_value);
  resource_value=
    XGetResourceClass(resource_database,client_name,"verbose","False");
  verbose=IsTrue(resource_value);
  /*
    Parse command line.
  */
  p=1;
  for (i=1; ((i <= argc) && !(state & ExitState)); i++)
  {
    if (i < argc)
      option=argv[i];
    else
      if (image_number == 0)
        option="xc:";
      else
        break;
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
          if (strncmp("blur",option+1,2) == 0)
            break;
          if (strcmp("border",option+1) == 0)
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
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing geometry on -clip",(char *) NULL);
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
              resource_info.server_name=server_name;
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
          if (strncmp("help",option+1,2) == 0)
            {
              Usage(True);
              break;
            }
          Error("Unrecognized option",option);
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
          if (strncmp("magnify",option+1,3) == 0)
            {
              resource_info.magnify=2;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing level on -magnify",(char *) NULL);
                  resource_info.magnify=atoi(argv[i]);
                }
              break;
            }
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
          if (strcmp("matte",option+1) == 0)
            {
              matte=(*option == '-');
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
          if (strncmp("name",option+1,2) == 0)
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
          if (strncmp("negate",option+1,2) == 0)
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
        case 'u':
        {
          if (strncmp("update",option+1,2) == 0)
            {
              resource_info.update=(*option == '-');
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%d",&x))
                    Error("Missing seconds on -update",(char *) NULL);
                  resource_info.delay=atoi(argv[i]);
                }
              break;
            }
          if (strncmp("use_pixmap",option+1,2) == 0)
            {
              resource_info.use_pixmap=(*option == '-');
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
          if (strncmp("write",option+1,2) == 0)
            {
              resource_info.write_filename=(char *) NULL;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    Error("Missing file name on -write",(char *) NULL);
                  resource_info.write_filename=argv[i];
                  if (access(resource_info.write_filename,0) == 0)
                    {
                      char
                        answer[2];

                      (void) fprintf(stderr,"Overwrite %s? ",
                        resource_info.write_filename);
                      (void) gets(answer);
                      if (!((*answer == 'y') || (*answer == 'Y')))
                        exit(1);
                    }
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
          info_image;

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
        GetImageInfo(option,&image_info);
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
        resource_info.image_info=(&image_info);
        image=ReadImage(&image_info);
        if (image == (Image *) NULL)
          if (*option == '-')
            break;
          else
            continue;
        do
        {
          info_image=(*image);
          total_colors=0;
          if (matte != NotInitialized)
            image->matte=matte;
          if (compression != UndefinedCompression)
            image->compression=compression;
          else
            image->compression=info_image.compression;
          if (scene != 0)
            image->scene=scene;
          LabelImage(image,label);
          if (comment != (char *) NULL)
            CommentImage(image,comment);
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
                  Blur an image.
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
                flags=XParseGeometry(argv[++q],&border_info.x,&border_info.y,
                  &border_info.width,&border_info.height);
                if ((flags & HeightValue) == 0)
                  border_info.height=border_info.width;
                bordered_image=BorderImage(image,&border_info,&border_color);
                if (bordered_image != (Image *) NULL)
                  {
                    DestroyImage(image);
                    bordered_image->class=DirectClass;
                    image=bordered_image;
                  }
              }
            if (strncmp("-bordercolor",option,8) == 0)
              {
                Colormap
                  colormap;

                /*
                  Get border color.
                */
                colormap=XDefaultColormap(display,XDefaultScreen(display));
                (void) XParseColor(display,colormap,argv[++q],&color);
                border_color.red=color.red >> 8;
                border_color.green=color.green >> 8;
                border_color.blue=color.blue >> 8;
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

                unsigned int
                  height,
                  width;

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
                  &border_color,False);
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
                  Sharpen an image.
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
                  (double) y_shear,&border_color,False);
                if (sheared_image != (Image *) NULL)
                  {
                    DestroyImage(image);
                    sheared_image->class=DirectClass;
                    image=sheared_image;
                  }
              }
          }
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
          /*
            Display image to X server.
          */
          if (resource_info.window_id != (char *) NULL)
            {
              /*
                Display image to a specified X window.
              */
              XDisplayBackgroundImage(display,&resource_info,image);
              state&=ExitState;
            }
          else
            do
            {
              Image
                *loaded_image;

              /*
                Display montage image.
              */
              loaded_image=
                XDisplayImage(display,&resource_info,argv,argc,&image,&state);
              if (loaded_image == (Image *) NULL)
                break;
              while ((loaded_image != (Image *) NULL) && (!(state & ExitState)))
              {
                next_image=XDisplayImage(display,&resource_info,argv,argc,
                  &loaded_image,&state);
                DestroyImage(loaded_image);
                loaded_image=next_image;
              }
            } while (!(state & ExitState));
          if (resource_info.write_filename != (char *) NULL)
            {
              /*
                Write image.
              */
              (void) strcpy(image->filename,resource_info.write_filename);
              (void) WriteImage(&image_info,image);
            }
          if (verbose)
            {
              /*
                Display detailed info about the image.
              */
              (void) fprintf(stderr,"[%u] %s",
                image->scene == 0 ? image_number : image->scene,
                info_image.filename);
              if (resource_info.write_filename != (char *) NULL)
                (void) fprintf(stderr,"=>%s",resource_info.write_filename);
              (void) fprintf(stderr," %ux%u",info_image.columns,
                info_image.rows);
              if ((info_image.columns != image->columns) ||
                  (info_image.rows != image->rows))
                (void) fprintf(stderr,"=>%ux%u",image->columns,image->rows);
              if (image->class == DirectClass)
                (void) fprintf(stderr," DirectClass ");
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
            Proceed to next/previous image.
          */
          if (state & FormerImageState)
            next_image=image->previous;
          else
            next_image=image->next;
          if (next_image != (Image *) NULL)
            image=next_image;
        } while ((next_image != (Image *) NULL) && !(state & ExitState));
        /*
          Free image resources.
        */
        while (image->previous != (Image *) NULL)
          image=image->previous;
        DestroyImages(image);
        if (!(state & FormerImageState))
          image_marker[i]=image_number++;
        else
          {
            /*
              Proceed to previous image.
            */
            for (i--; i > 0; i--)
              if (image_marker[i] == (image_number-2))
                break;
            if (image_number != 0)
              image_number--;
          }
        p=i+1;
        if (state & ExitState)
          break;
      }
    if (i == (argc-1))
      if ((resource_info.delay != 0) && (image_number != 1))
        {
          /*
            Proceed to first image.
          */
          i=0;
          image_number=0;
        }
  }
  XCloseDisplay(display);
  (void) free((char *) image_marker);
  return(False);
}
