/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%                  W   W  IIIII  DDDD    GGGG  EEEEE  TTTTT                   %
%                  W   W    I    D   D  G      E        T                     %
%                  W W W    I    D   D  G  GG  EEE      T                     %
%                  WW WW    I    D   D  G   G  E        T                     %
%                  W   W  IIIII  DDDD    GGGG  EEEEE    T                     %
%                                                                             %
%                X11 User Interface Routines for ImageMagick.                 %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                              September 1993                                 %
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
  Define declarations.
*/
#define MatteIsActive(matte_info,position)  \
  ((position.x >= (matte_info.x-matte_info.bevel_width)) && \
   (position.y >= (matte_info.y-matte_info.bevel_width)) &&  \
   (position.x < (matte_info.x+matte_info.width+matte_info.bevel_width)) &&  \
   (position.y < (matte_info.y+matte_info.height+matte_info.bevel_width)))
/*
  State declarations.
*/
#define ControlState  0x0001
#define DefaultState  0x0000
#define ExitState  0x0002
#define InactiveWidgetState  0x0004
#define RedrawActionState  0x0008
#define RedrawListState  0x0010
#define RedrawWidgetState  0x0020
#define UpdateConfigurationState  0x0040
#define UpdateListState  0x0080

/*
  Forward declarations.
*/
static void
  XDrawMatte _Declare((Display *,XWindowInfo *,XWidgetInfo *)),
  XSetBevelColor _Declare((Display *,XWindowInfo *,unsigned int)),
  XSetMatteColor _Declare((Display *,XWindowInfo *,unsigned int)),
  XSetTextColor _Declare((Display *,XWindowInfo *,unsigned int));

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w B e v e l                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawBevel "sets off" an area with a highlighted upper and
%  left bevel and a shadowed lower and right bevel.  The highlighted and
%  shadowed bevels create a 3-D effect.
%
%  The format of the XDrawBevel function is:
%
%      XDrawBevel(display,window_info,bevel_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o bevel_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the bevel.
%
%
*/
static void XDrawBevel(display,window_info,bevel_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *bevel_info;
{
  int
    x1,
    x2,
    y1,
    y2;

  unsigned int
    bevel_width;

  XPoint
    points[6];

  /*
    Draw upper and left beveled border.
  */
  x1=bevel_info->x;
  y1=bevel_info->y+bevel_info->height;
  x2=bevel_info->x+bevel_info->width;
  y2=bevel_info->y;
  bevel_width=bevel_info->bevel_width;
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x1;
  points[1].y=y2;
  points[2].x=x2;
  points[2].y=y2;
  points[3].x=x2+bevel_width;
  points[3].y=y2-bevel_width;
  points[4].x=x1-bevel_width;
  points[4].y=y2-bevel_width;
  points[5].x=x1-bevel_width;
  points[5].y=y1+bevel_width;
  XSetBevelColor(display,window_info,bevel_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,6,
    Complex,CoordModeOrigin);
  /*
    Draw lower and right beveled border.
  */
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x2;
  points[1].y=y1;
  points[2].x=x2;
  points[2].y=y2;
  points[3].x=x2+bevel_width;
  points[3].y=y2-bevel_width;
  points[4].x=x2+bevel_width;
  points[4].y=y1+bevel_width;
  points[5].x=x1-bevel_width;
  points[5].y=y1+bevel_width;
  XSetBevelColor(display,window_info,(unsigned int) !bevel_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,6,
    Complex,CoordModeOrigin);
  XSetFillStyle(display,window_info->widget_context,FillSolid);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w B e v e l e d B u t t o n                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawBeveledButton draws a button with a highlighted upper and
%  left bevel and a shadowed lower and right bevel.  The highlighted and
%  shadowed bevels create a 3-D effect.
%
%  The format of the XDrawBeveledButton function is:
%
%      XDrawBeveledButton(display,window_info,button_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o button_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the button.
%
%
*/
static void XDrawBeveledButton(display,window_info,button_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *button_info;
{
  int
    x,
    y;

  unsigned int
    width;

  XFontStruct
    *font_info;

  XRectangle
    clip_info;

  /*
    Draw matte.
  */
  XDrawBevel(display,window_info,button_info);
  XSetMatteColor(display,window_info,button_info->raised);
  XFillRectangle(display,window_info->id,window_info->widget_context,
    button_info->x,button_info->y,button_info->width,button_info->height);
  x=button_info->x-button_info->bevel_width-1;
  y=button_info->y-button_info->bevel_width-1;
  XSetForeground(display,window_info->widget_context,
    window_info->pixel_info->trough_color.pixel);
  if (button_info->raised || (window_info->depth == 1))
    XDrawRectangle(display,window_info->id,window_info->widget_context,x,y,
      button_info->width+(button_info->bevel_width << 1)+1,button_info->height+
      (button_info->bevel_width << 1)+1);
  if (button_info->text == (char *) NULL)
    return;
  /*
    Set clipping region.
  */
  clip_info.width=button_info->width;
  clip_info.height=button_info->height;
  clip_info.x=button_info->x;
  clip_info.y=button_info->y;
  /*
    Draw text.
  */
  font_info=window_info->font_info;
  width=XTextWidth(font_info,button_info->text,strlen(button_info->text));
  x=button_info->x+(button_info->width >> 1)-(width >> 1);
  if (x < button_info->x)
    x=button_info->x;
  y=button_info->y+
    ((button_info->height-(font_info->ascent+font_info->descent)) >> 1);
  if (y < button_info->y)
    y=button_info->y;
  y+=window_info->font_info->ascent;
  XSetClipRectangles(display,window_info->widget_context,0,0,&clip_info,1,
    Unsorted);
  XSetTextColor(display,window_info,button_info->raised);
  XDrawString(display,window_info->id,window_info->widget_context,x,y,
    button_info->text,strlen(button_info->text));
  XSetClipMask(display,window_info->widget_context,None);
  if (!button_info->raised)
    XDelay(display,SuspendTime << 2);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w B e v e l e d M a t t e                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawBeveledMatte draws a matte with a shadowed upper and
%  left bevel and a highlighted lower and right bevel.  The highlighted and
%  shadowed bevels create a 3-D effect.
%
%  The format of the XDrawBeveledMatte function is:
%
%      XDrawBeveledMatte(display,window_info,matte_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o matte_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the matte.
%
%
*/
static void XDrawBeveledMatte(display,window_info,matte_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *matte_info;
{
  /*
    Draw matte.
  */
  XDrawBevel(display,window_info,matte_info);
  XDrawMatte(display,window_info,matte_info);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w M a t t e                                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawMatte fills a rectangular area with the matte color.
%
%  The format of the XDrawMatte function is:
%
%      XDrawMatte(display,window_info,matte_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o matte_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the matte.
%
%
*/
static void XDrawMatte(display,window_info,matte_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *matte_info;
{
  /*
    Draw matte.
  */
  if (!matte_info->trough || (window_info->depth == 1))
    XFillRectangle(display,window_info->id,window_info->highlight_context,
      matte_info->x,matte_info->y,matte_info->width,matte_info->height);
  else
    {
      XSetForeground(display,window_info->widget_context,
        window_info->pixel_info->trough_color.pixel);
      XFillRectangle(display,window_info->id,window_info->widget_context,
        matte_info->x,matte_info->y,matte_info->width,matte_info->height);
    }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w M a t t e T e x t                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawMatteText draws a matte with text.  If the text exceeds the
%  extents of the text, a portion of the text relative to the cursor is
%  displayed.
%
%  The format of the XDrawMatteText function is:
%
%      XDrawMatteText(display,window_info,text_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o text_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the text.
%
%
*/
static void XDrawMatteText(display,window_info,text_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *text_info;
{
  char
    *text;

  int
    x,
    y;

  register int
    i;

  unsigned int
    height,
    width;

  XFontStruct
    *font_info;

  XRectangle
    clip_info;

  /*
    Clear the text area.
  */
  XSetMatteColor(display,window_info,False);
  XFillRectangle(display,window_info->id,window_info->widget_context,
    text_info->x,text_info->y,text_info->width,text_info->height);
  if (text_info->text == (char *) NULL)
    return;
  if (*text_info->text == '\0')
    return;
  /*
    Set clipping region.
  */
  clip_info.width=text_info->width;
  clip_info.height=text_info->height;
  clip_info.x=text_info->x;
  clip_info.y=text_info->y;
  /*
    Determine beginning of the visible text.
  */
  font_info=window_info->font_info;
  width=text_info->width-(font_info->max_bounds.width >> 1);
  height=font_info->ascent+font_info->descent;
  if (text_info->cursor < text_info->marker)
    text_info->marker=text_info->cursor;
  else
    {
      text=text_info->marker;
      if (XTextWidth(font_info,text,text_info->cursor-text) > width)
        {
          text=text_info->text;
          for (i=0; i < (int) strlen(text); i++)
            if (XTextWidth(font_info,text+i,text_info->cursor-text-i) <= width)
              break;
          text_info->marker=text+i;
        }
    }
  /*
    Draw text and cursor.
  */
  XSetTextColor(display,window_info,text_info->highlight);
  x=text_info->x+(font_info->max_bounds.width >> 2);
  y=text_info->y+font_info->ascent+(text_info->height >> 2);
  if (!text_info->highlight)
    {
      XSetClipRectangles(display,window_info->widget_context,0,0,&clip_info,1,
        Unsorted);
      XDrawString(display,window_info->id,window_info->widget_context,x,y,
        text_info->marker,strlen(text_info->marker));
      XSetClipMask(display,window_info->widget_context,None);
    }
  else
    {
      XSetClipRectangles(display,window_info->annotate_context,0,0,&clip_info,
        1,Unsorted);
      XFillRectangle(display,window_info->id,window_info->annotate_context,x,
        y-font_info->ascent,text_info->width,height);
      XSetClipMask(display,window_info->annotate_context,None);
      XSetClipRectangles(display,window_info->highlight_context,0,0,&clip_info,
        1,Unsorted);
      XDrawString(display,window_info->id,window_info->highlight_context,x,y,
        text_info->marker,strlen(text_info->marker));
      XSetClipMask(display,window_info->highlight_context,None);
    }
  x+=
    XTextWidth(font_info,text_info->marker,text_info->cursor-text_info->marker);
  XDrawLine(display,window_info->id,window_info->annotate_context,x,y+3,
    x,y-height+3);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w T r i a n g l e N o r t h                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawTriangleNorth draws a triangle with a highlighted left
%  bevel and a shadowed right and lower bevel.  The highlighted and
%  shadowed bevels create a 3-D effect.
%
%  The format of the XDrawTriangleNorth function is:
%
%      XDrawTriangleNorth(display,window_info,triangle_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o triangle_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the triangle.
%
%
*/
static void XDrawTriangleNorth(display,window_info,triangle_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *triangle_info;
{
  int
    x1,
    x2,
    x3,
    y1,
    y2,
    y3;

  unsigned int
    bevel_width;

  XPoint
    points[4];

  /*
    Draw triangle matte.
  */
  x1=triangle_info->x;
  y1=triangle_info->y+triangle_info->height;
  x2=triangle_info->x+(triangle_info->width >> 1);
  y2=triangle_info->y;
  x3=triangle_info->x+triangle_info->width;
  y3=triangle_info->y+triangle_info->height;
  bevel_width=triangle_info->bevel_width;
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x2;
  points[1].y=y2;
  points[2].x=x3;
  points[2].y=y3;
  XSetMatteColor(display,window_info,triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,3,
    Complex,CoordModeOrigin);
  /*
    Draw left bevel.
  */
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x2;
  points[1].y=y2;
  points[2].x=x2;
  points[2].y=y2-bevel_width-2;
  points[3].x=x1-bevel_width-1;
  points[3].y=y1+bevel_width;
  XSetBevelColor(display,window_info,triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  /*
    Draw right bevel.
  */
  points[0].x=x2;
  points[0].y=y2;
  points[1].x=x3;
  points[1].y=y3;
  points[2].x=x3+bevel_width;
  points[2].y=y3+bevel_width;
  points[3].x=x2;
  points[3].y=y2-bevel_width;
  XSetBevelColor(display,window_info,(unsigned int) !triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  /*
    Draw lower bevel.
  */
  points[0].x=x3;
  points[0].y=y3;
  points[1].x=x1;
  points[1].y=y1;
  points[2].x=x1-bevel_width;
  points[2].y=y1+bevel_width;
  points[3].x=x3+bevel_width;
  points[3].y=y3+bevel_width;
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  XSetFillStyle(display,window_info->widget_context,FillSolid);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w T r i a n g l e S o u t h                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawTriangleSouth draws a border with a highlighted left and
%  right bevel and a shadowed lower bevel.  The highlighted and shadowed
%  bevels create a 3-D effect.
%
%  The format of the XDrawTriangleSouth function is:
%
%      XDrawTriangleSouth(display,window_info,triangle_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o triangle_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the triangle.
%
%
*/
static void XDrawTriangleSouth(display,window_info,triangle_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *triangle_info;
{
  int
    x1,
    x2,
    x3,
    y1,
    y2,
    y3;

  unsigned int
    bevel_width;

  XPoint
    points[4];

  /*
    Draw triangle matte.
  */
  x1=triangle_info->x;
  y1=triangle_info->y;
  x2=triangle_info->x+(triangle_info->width >> 1);
  y2=triangle_info->y+triangle_info->height;
  x3=triangle_info->x+triangle_info->width;
  y3=triangle_info->y;
  bevel_width=triangle_info->bevel_width;
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x2;
  points[1].y=y2;
  points[2].x=x3;
  points[2].y=y3;
  XSetMatteColor(display,window_info,triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,3,
    Complex,CoordModeOrigin);
  /*
    Draw top bevel.
  */
  points[0].x=x3;
  points[0].y=y3;
  points[1].x=x1;
  points[1].y=y1;
  points[2].x=x1-bevel_width;
  points[2].y=y1-bevel_width;
  points[3].x=x3+bevel_width;
  points[3].y=y3-bevel_width;
  XSetBevelColor(display,window_info,triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  /*
    Draw right bevel.
  */
  points[0].x=x2;
  points[0].y=y2;
  points[1].x=x3+1;
  points[1].y=y3-bevel_width;
  points[2].x=x3+bevel_width;
  points[2].y=y3-bevel_width;
  points[3].x=x2;
  points[3].y=y2+bevel_width;
  XSetBevelColor(display,window_info,(unsigned int) !triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  /*
    Draw left bevel.
  */
  points[0].x=x1;
  points[0].y=y1;
  points[1].x=x2;
  points[1].y=y2;
  points[2].x=x2;
  points[2].y=y2+bevel_width;
  points[3].x=x1-bevel_width;
  points[3].y=y1-bevel_width;
  XSetBevelColor(display,window_info,triangle_info->raised);
  XFillPolygon(display,window_info->id,window_info->widget_context,points,4,
    Complex,CoordModeOrigin);
  XSetFillStyle(display,window_info->widget_context,FillSolid);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D r a w W i d g e t T e x t                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDrawWidgetText first clears the widget and draws a text string
%  justifed left in the x-direction and centered within the y-direction.
%
%  The format of the XDrawWidgetText function is:
%
%      XDrawWidgetText(display,window_info,text_info)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a XWindowText structure.
%
%    o text_info: Specifies a pointer to XWidgetInfo structure.
%
%
*/
static void XDrawWidgetText(display,window_info,text_info)
Display
  *display;

XWindowInfo
  *window_info;

XWidgetInfo
  *text_info;
{
  GC
    widget_context;

  int
    x,
    y;

  unsigned int
    height,
    width;

  XFontStruct
    *font_info;

  XRectangle
    clip_info;

  /*
    Clear the text area.
  */
  widget_context=window_info->annotate_context;
  if (text_info->raised)
    XClearArea(display,window_info->id,text_info->x,text_info->y,
      text_info->width,text_info->height,False);
  else
    {
      XFillRectangle(display,window_info->id,widget_context,text_info->x,
        text_info->y,text_info->width,text_info->height);
      widget_context=window_info->highlight_context;
    }
  if (text_info->text == (char *) NULL)
    return;
  if (*text_info->text == '\0')
    return;
  /*
    Set clipping region.
  */
  font_info=window_info->font_info;
  clip_info.width=text_info->width;
  clip_info.height=text_info->height;
  clip_info.x=text_info->x;
  clip_info.y=text_info->y;
  /*
    Draw text.
  */
  width=XTextWidth(font_info,text_info->text,strlen(text_info->text));
  x=text_info->x+(font_info->max_bounds.width >> 1);
  if (width > (text_info->width-font_info->max_bounds.width))
    x+=(text_info->width-font_info->max_bounds.width-width);
  height=font_info->ascent+font_info->descent;
  y=text_info->y+((text_info->height-height) >> 1)+font_info->ascent;
  XSetClipRectangles(display,widget_context,0,0,&clip_info,1,Unsorted);
  XDrawString(display,window_info->id,widget_context,x,y,
    text_info->text,strlen(text_info->text));
  XSetClipMask(display,widget_context,None);
  if (x < text_info->x)
    XDrawLine(display,window_info->id,window_info->annotate_context,
      text_info->x,text_info->y,text_info->x,text_info->y+text_info->height-1);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X E d i t T e x t                                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XEditText edits a text string as indicated by the key symbol.
%
%  The format of the XEditText function is:
%
%      XEditText(display,text_info,key_symbol,text,state)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o text_info: Specifies a pointer to a XWidgetInfo structure.  It
%      contains the extents of the text.
%
%    o key_symbol:  A X11 KeySym that indicates what editing function to
%      perform to the text.
%
%    o text: A character string to insert into the text.
%
%    o state:  An unsigned long that indicates whether the key symbol is a
%      control character or not.
%
%
*/
static void XEditText(display,text_info,key_symbol,text,state)
Display
  *display;

XWidgetInfo
  *text_info;

KeySym
  key_symbol;

char
  *text;

unsigned long
  state;
{
  switch (key_symbol)
  {
    case XK_BackSpace:
    {
      /*
        Erase one character.
      */
      if (text_info->cursor == text_info->text)
        break;
      text_info->cursor--;
      (void) strcpy(text_info->cursor,text_info->cursor+1);
      text_info->highlight=False;
      break;
    }
    case XK_Delete:
    {
      /*
        Erase the entire line of text.
      */
      *text_info->text='\0';
      text_info->cursor=text_info->text;
      text_info->highlight=False;
      break;
    }
    case XK_Left:
    case XK_KP_Left:
    {
      /*
        Move cursor one position left.
      */
      if (text_info->cursor == text_info->text)
        break;
      text_info->cursor--;
      break;
    }
    case XK_Right:
    case XK_KP_Right:
    {
      /*
        Move cursor one position right.
      */
      if (text_info->cursor == (text_info->text+strlen(text_info->text)))
        break;
      text_info->cursor++;
      break;
    }
    default:
    {
      register char
        *p,
        *q;

      register int
        i;

      if (state & ControlState)
        break;
      if (*text == '\0')
        break;
      if (((int) strlen(text_info->text)+1) >= MaxTextLength)
        XBell(display,0);
      else
        {
          /*
            Insert a string into the text.
          */
          q=text_info->text+strlen(text_info->text)+strlen(text);
          for (i=0; i <= (int) strlen(text_info->cursor); i++)
          {
            *q=(*(q-strlen(text)));
            q--;
          }
          p=text;
          for (i=0; i < (int) strlen(text); i++)
            *text_info->cursor++=(*p++);
          text_info->highlight=False;
        }
      break;
    }
  }
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X G e t W i d g e t I n f o                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XGetWidgetInfo initializes the XWidgetInfo structure.
%
%  The format of the XGetWidgetInfo function is:
%
%      XGetWidgetInfo(text,widget_info)
%
%  A description of each parameter follows:
%
%    o text: A string of characters associated with the widget.
%
%    o widget_info: Specifies a pointer to a X11 XWidgetInfo structure.
%
%
*/
static void XGetWidgetInfo(text,widget_info)
char
  *text;

XWidgetInfo
  *widget_info;
{
  /*
    Initialize widget info.
  */
   widget_info->id=(~0);
   widget_info->bevel_width=3;
   widget_info->width=1;
   widget_info->height=1;
   widget_info->x=0;
   widget_info->y=0;
   widget_info->min_y=0;
   widget_info->max_y=0;
   widget_info->raised=True;
   widget_info->active=False;
   widget_info->trough=False;
   widget_info->highlight=False;
   widget_info->text=text;
   widget_info->cursor=text;
   if (text != (char *) NULL)
     widget_info->cursor+=strlen(text);
   widget_info->marker=text;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X H i g h l i g h t W i d g e t                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XHighlightWidget draws a highlighted border around a window.
%
%  The format of the XHighlightWidget function is:
%
%      XHighlightWidget(display,window_info,x,y)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o x: Specifies an integer representing the rectangle offset in the
%      x-direction.
%
%    o y: Specifies an integer representing the rectangle offset in the
%      y-direction.
%
%
*/
static void XHighlightWidget(display,window_info,x,y)
Display
  *display;

XWindowInfo
  *window_info;

int
  x,
  y;
{
  /*
    Draw the widget highlighting rectangle.
  */
  XSetBevelColor(display,window_info,True);
  XDrawRectangle(display,window_info->id,window_info->widget_context,x,y,
    window_info->width-(x << 1),window_info->height-(y << 1));
  XDrawRectangle(display,window_info->id,window_info->widget_context,x-1,y-1,
    window_info->width-(x << 1)+1,window_info->height-(y << 1)+1);
  XSetBevelColor(display,window_info,False);
  XDrawRectangle(display,window_info->id,window_info->widget_context,x-1,y-1,
    window_info->width-(x << 1),window_info->height-(y << 1));
  XSetFillStyle(display,window_info->widget_context,FillSolid);
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
%  Function XScreenEvent returns True if the any event on the X server queue
%  is associated with the widget window.
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
  XWindows
    *windows;

  windows=(XWindows *) data;
  if (event->xany.window == windows->popup.id)
    return(True);
  if (event->xany.window == windows->command.id)
    return(True);
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
      return(True);
    }
    case Expose:
    {
      if (event->xexpose.window == windows->image.id)
        {
          XRefreshWindow(display,&windows->image,event);
          break;
        }
      if (event->xexpose.window == windows->magnify.id)
        if (event->xexpose.count == 0)
          if (windows->magnify.mapped)
            {
              XMakeMagnifyImage(display,windows);
              break;
            }
      break;
    }
    case FocusOut:
    {
      /*
        Set input focus for backdrop window.
      */
      if (event->xfocus.window == windows->image.id)
        XSetInputFocus(display,windows->image.id,RevertToNone,CurrentTime);
      return(True);
    }
    case ButtonRelease:
    case KeyPress:
    case KeyRelease:
    case MotionNotify:
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
%   X S e t B e v e l C o l o r                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XSetBevelColor sets the graphic context for drawing a beveled
%  border.
%
%  The format of the XSetBevelColor function is:
%
%      XSetBevelColor(display,window_info,raised)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o raised: A value other than zero indicates the color show be a
%      "highlight" color, otherwise the "shadow" color is set.
%
%
*/
static void XSetBevelColor(display,window_info,raised)
Display
  *display;

XWindowInfo
  *window_info;

unsigned int
  raised;
{
  if (window_info->depth == 1)
    {
      Pixmap
        stipple;

      /*
        Monochrome window.
      */
      XSetBackground(display,window_info->widget_context,
        XBlackPixel(display,window_info->screen));
      XSetForeground(display,window_info->widget_context,
        XWhitePixel(display,window_info->screen));
      XSetFillStyle(display,window_info->widget_context,FillOpaqueStippled);
      stipple=window_info->highlight_stipple;
      if (!raised)
        stipple=window_info->shadow_stipple;
      XSetStipple(display,window_info->widget_context,stipple);
    }
  else
    if (raised)
      XSetForeground(display,window_info->widget_context,
        window_info->pixel_info->highlight_color.pixel);
    else
      XSetForeground(display,window_info->widget_context,
        window_info->pixel_info->shadow_color.pixel);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S e t M a t t e C o l o r                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XSetMatteColor sets the graphic context for drawing the matte.
%
%  The format of the XSetMatteColor function is:
%
%      XSetMatteColor(display,window_info,raised)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o raised: A value other than zero indicates the matte is active.
%
%
*/
static void XSetMatteColor(display,window_info,raised)
Display
  *display;

XWindowInfo
  *window_info;

unsigned int
  raised;
{
  if (window_info->depth == 1)
    {
      /*
        Monochrome window.
      */
      if (raised)
        XSetForeground(display,window_info->widget_context,
          XWhitePixel(display,window_info->screen));
      else
        XSetForeground(display,window_info->widget_context,
          XBlackPixel(display,window_info->screen));
    }
  else
    if (raised)
      XSetForeground(display,window_info->widget_context,
        window_info->pixel_info->matte_color.pixel);
    else
      XSetForeground(display,window_info->widget_context,
        window_info->pixel_info->depth_color.pixel);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X S e t T e x t C o l o r                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XSetTextColor sets the graphic context for drawing text on a
%  matte.
%
%  The format of the XSetTextColor function is:
%
%      XSetTextColor(display,window_info,raised)
%
%  A description of each parameter follows:
%
%    o display: Specifies a pointer to the Display structure;  returned from
%      XOpenDisplay.
%
%    o window_info: Specifies a pointer to a X11 XWindowInfo structure.
%
%    o raised: A value other than zero indicates the color show be a
%      "highlight" color, otherwise the "shadow" color is set.
%
%
*/
static void XSetTextColor(display,window_info,raised)
Display
  *display;

XWindowInfo
  *window_info;

unsigned int
  raised;
{
  if (window_info->depth == 1)
    {
      /*
        Monochrome window.
      */
      if (raised)
        XSetForeground(display,window_info->widget_context,
          XBlackPixel(display,window_info->screen));
      else
        XSetForeground(display,window_info->widget_context,
          XWhitePixel(display,window_info->screen));
    }
  else
    XSetForeground(display,window_info->widget_context,
      window_info->pixel_info->foreground_color.pixel);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o l o r B r o w s e r W i d g e t                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XColorBrowserWidget displays a popup window with a color query to
%  the user.  The user keys a reply and presses the Action or Cancel button
%  to exit.  The typed text is returned as the reply function parameter.
%
%  The format of the XColorBrowserWidget routine is:
%
%    XColorBrowserWidget(display,windows,action,reply)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o action: Specifies a pointer to the action of this widget.
%
%    o reply: The response from the user is returned in this parameter.
%
%
*/
void XColorBrowserWidget(display,windows,action,reply)
Display
  *display;

XWindows
  *windows;

char
  *action,
  *reply;
{
#define CancelButtonText  "Cancel"
#define ColornameText  "Name:"
#define ColorPatternText  "Pattern:"
#define GrabButtonText  "Grab"
#define ResetButtonText  "Reset"

  char
    **colorlist,
    primary_selection[MaxTextLength],
    reset_pattern[MaxTextLength],
    text[MaxTextLength];

  int
    colors,
    x,
    y;

  register int
    i;

  static char
    glob_pattern[MaxTextLength] = "*";

  unsigned int
    height,
    limit,
    mask,
    status,
    text_width,
    visible_colors,
    width;

  unsigned long
    delay,
    state;

  Window
    root_window;

  XColor
    color;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    action_info,
    cancel_info,
    expose_info,
    grab_info,
    list_info,
    mode_info,
    north_info,
    reply_info,
    reset_info,
    scroll_info,
    selection_info,
    slider_info,
    south_info,
    text_info;

  XWindowChanges
    window_changes;

  /*
    Get color list and sort in ascending order.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  (void) strcpy(reset_pattern,"*");
  colorlist=XListColors(glob_pattern,&colors);
  if (colorlist == (char **) NULL)
    {
      /*
        Pattern failed, obtain all the colors.
      */
      XNoticeWidget(display,windows,"Unable to obtain colors names",
        glob_pattern);
      (void) strcpy(glob_pattern,"*");
      colorlist=XListColors(glob_pattern,&colors);
      if (colorlist == (char **) NULL)
        {
          XNoticeWidget(display,windows,"Unable to obtain colors names",
            glob_pattern);
          return;
        }
    }
  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  text_width=0;
  for (i=0; i < colors; i++)
    if (XTextWidth(font_info,colorlist[i],strlen(colorlist[i])) > text_width)
      text_width=XTextWidth(font_info,colorlist[i],strlen(colorlist[i]));
  width=XTextWidth(font_info,action,strlen(action));
  if (XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText)) > width)
    width=XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText));
  if (XTextWidth(font_info,ResetButtonText,strlen(ResetButtonText)) > width)
    width=XTextWidth(font_info,ResetButtonText,strlen(ResetButtonText));
  if (XTextWidth(font_info,GrabButtonText,strlen(GrabButtonText)) > width)
    width=XTextWidth(font_info,GrabButtonText,strlen(GrabButtonText));
  width+=font_info->max_bounds.width;
  if (XTextWidth(font_info,ColorPatternText,strlen(ColorPatternText)) > width)
    width=XTextWidth(font_info,ColorPatternText,strlen(ColorPatternText));
  if (XTextWidth(font_info,ColornameText,strlen(ColornameText)) > width)
    width=XTextWidth(font_info,ColornameText,strlen(ColornameText));
  height=font_info->ascent+font_info->descent;
  windows->popup.width=width+text_width+6*font_info->max_bounds.width;
  windows->popup.height=
    ((81*height) >> 2)+((13*font_info->max_bounds.width) >> 1)+4;
  windows->popup.min_width=width+25*XTextWidth(font_info,"#",1)+
    4*font_info->max_bounds.width;
  windows->popup.min_height=
    ((23*height) >> 1)+((13*font_info->max_bounds.width) >> 1)+4;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=((3*windows->popup.width) >> 2);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=(windows->popup.height >> 1);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,"Browse and Select a Color");
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  XGetWidgetInfo((char *) NULL,&slider_info);
  XGetWidgetInfo((char *) NULL,&north_info);
  XGetWidgetInfo((char *) NULL,&south_info);
  visible_colors=0;
  delay=SuspendTime << 2;
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        int
          id;

        /*
          Initialize button information.
        */
        XGetWidgetInfo(CancelButtonText,&cancel_info);
        cancel_info.width=width;
        cancel_info.height=(3*height) >> 1;
        cancel_info.x=windows->popup.width-cancel_info.width-
          font_info->max_bounds.width-2;
        cancel_info.y=windows->popup.height-cancel_info.height-
          font_info->max_bounds.width;
        XGetWidgetInfo(action,&action_info);
        action_info.width=width;
        action_info.height=(3*height) >> 1;
        action_info.x=cancel_info.x-(cancel_info.width+
          (font_info->max_bounds.width >> 1)+(action_info.bevel_width << 1));
        action_info.y=cancel_info.y;
        XGetWidgetInfo(GrabButtonText,&grab_info);
        grab_info.width=width;
        grab_info.height=(3*height) >> 1;
        grab_info.x=font_info->max_bounds.width;
        grab_info.y=((5*font_info->max_bounds.width) >> 1)+height;
        XGetWidgetInfo(ResetButtonText,&reset_info);
        reset_info.width=width;
        reset_info.height=(3*height) >> 1;
        reset_info.x=font_info->max_bounds.width;
        reset_info.y=grab_info.y+grab_info.height+font_info->max_bounds.width;
        /*
          Initialize reply information.
        */
        XGetWidgetInfo(reply,&reply_info);
        reply_info.raised=False;
        reply_info.bevel_width--;
        reply_info.width=windows->popup.width-width-
          ((6*font_info->max_bounds.width) >> 1);
        reply_info.height=height << 1;
        reply_info.x=width+(font_info->max_bounds.width << 1);
        reply_info.y=
          action_info.y-reply_info.height-font_info->max_bounds.width;
        /*
          Initialize mode information.
        */
        XGetWidgetInfo((char *) NULL,&mode_info);
        mode_info.bevel_width=0;
        mode_info.width=action_info.x-reply_info.x-font_info->max_bounds.width;
        mode_info.height=action_info.height;
        mode_info.x=reply_info.x;
        mode_info.y=action_info.y;
        /*
          Initialize scroll information.
        */
        XGetWidgetInfo((char *) NULL,&scroll_info);
        scroll_info.bevel_width--;
        scroll_info.width=height;
        scroll_info.height=
          reply_info.y-grab_info.y-(font_info->max_bounds.width >> 1);
        scroll_info.x=reply_info.x+(reply_info.width-scroll_info.width);
        scroll_info.y=grab_info.y-reply_info.bevel_width;
        scroll_info.raised=False;
        scroll_info.trough=True;
        north_info=scroll_info;
        north_info.raised=True;
        north_info.width-=(north_info.bevel_width << 1);
        north_info.height=north_info.width-1;
        north_info.x+=north_info.bevel_width;
        north_info.y+=north_info.bevel_width;
        south_info=north_info;
        south_info.y=scroll_info.y+scroll_info.height-scroll_info.bevel_width-
          south_info.height;
        id=slider_info.id;
        slider_info=north_info;
        slider_info.id=id;
        slider_info.width-=2;
        slider_info.min_y=north_info.y+north_info.height+north_info.bevel_width+
          slider_info.bevel_width+2;
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+2;
        visible_colors=
          (scroll_info.height-(height >> 3)-4)/((9*height) >> 3);
        if (colors > visible_colors)
          slider_info.height=(visible_colors*slider_info.height)/colors;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.x=scroll_info.x+slider_info.bevel_width+1;
        slider_info.y=slider_info.min_y;
        expose_info=scroll_info;
        expose_info.y=slider_info.y;
        /*
          Initialize list information.
        */
        XGetWidgetInfo((char *) NULL,&list_info);
        list_info.raised=False;
        list_info.bevel_width--;
        list_info.width=
          scroll_info.x-reply_info.x-(font_info->max_bounds.width >> 1);
        list_info.height=scroll_info.height;
        list_info.x=reply_info.x;
        list_info.y=scroll_info.y;
        /*
          Initialize text information.
        */
        XGetWidgetInfo(text,&text_info);
        text_info.width=reply_info.width;
        text_info.height=height;
        text_info.x=list_info.x-(font_info->max_bounds.width >> 1);
        text_info.y=font_info->max_bounds.width;
        /*
          Initialize selection information.
        */
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=list_info.width;
        selection_info.height=(9*height) >> 3;
        selection_info.x=list_info.x;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw color browser window.
        */
        XClearWindow(display,windows->popup.id);
        x=font_info->max_bounds.width;
        y=text_info.y+((text_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,ColorPatternText,strlen(ColorPatternText));
        (void) sprintf(text_info.text,"%s",glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawBeveledButton(display,&windows->popup,&grab_info);
        XDrawBeveledButton(display,&windows->popup,&reset_info);
        XDrawBeveledMatte(display,&windows->popup,&list_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        x=font_info->max_bounds.width;
        y=reply_info.y+((reply_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,ColornameText,strlen(ColornameText));
        XDrawBeveledMatte(display,&windows->popup,&reply_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledButton(display,&windows->popup,&action_info);
        XDrawBeveledButton(display,&windows->popup,&cancel_info);
        XHighlightWidget(display,&windows->popup,4,4);
        selection_info.id=(~0);
        state|=RedrawActionState;
        state|=RedrawListState;
        state&=(~RedrawWidgetState);
      }
    if (state & UpdateListState)
      {
        char
          **checklist;

        int
          number_colors;

        status=XParseColor(display,windows->popup.map_info->colormap,
          glob_pattern,&color);
        if (status != 0)
          {
            /*
              Reply is a single color name-- exit.
            */
            (void) strcpy(reply,glob_pattern);
            (void) strcpy(glob_pattern,reset_pattern);
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        /*
          Update color list.
        */
        checklist=XListColors(glob_pattern,&number_colors);
        if (number_colors == 0)
          {
            (void) strcpy(glob_pattern,reset_pattern);
            XBell(display,0);
          }
        else
          {
            for (i=0; i < colors; i++)
              (void) free((char *) colorlist[i]);
            if (colorlist != (char **) NULL)
              (void) free((char *) colorlist);
            colorlist=checklist;
            colors=number_colors;
          }
        /*
          Sort color list in ascending order.
        */
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+1;
        if (colors > visible_colors)
          slider_info.height=(visible_colors*slider_info.height)/colors;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.id=0;
        slider_info.y=slider_info.min_y;
        expose_info.y=slider_info.y;
        selection_info.id=(~0);
        list_info.id=(~0);
        state|=RedrawListState;
        /*
          Redraw color name & reply.
        */
        *reply_info.text='\0';
        reply_info.cursor=reply_info.text;
        (void) sprintf(text_info.text,"%s",glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        XHighlightWidget(display,&windows->popup,4,4);
        state&=(~UpdateListState);
      }
    if (state & RedrawListState)
      {
        /*
          Determine slider id and position.
        */
        if (slider_info.id >= (int) (colors-visible_colors))
          slider_info.id=colors-visible_colors;
        if ((slider_info.id < 0) || (colors <= visible_colors))
          slider_info.id=0;
        slider_info.y=slider_info.min_y;
        if (colors > 0)
          slider_info.y+=
            slider_info.id*(slider_info.max_y-slider_info.min_y+1)/colors;
        if (slider_info.id != selection_info.id)
          {
            /*
              Redraw scroll bar and file names.
            */
            selection_info.id=slider_info.id;
            selection_info.y=list_info.y+(height >> 3)+2;
            for (i=0; i < visible_colors; i++)
            {
              selection_info.raised=(slider_info.id+i) != list_info.id;
              selection_info.text=(char *) NULL;
              if ((slider_info.id+i) < colors)
                selection_info.text=colorlist[slider_info.id+i];
              XDrawWidgetText(display,&windows->popup,&selection_info);
              selection_info.y+=(int) selection_info.height;
            }
            /*
              Update slider.
            */
            if (slider_info.y > expose_info.y)
              {
                expose_info.height=slider_info.y-expose_info.y;
                expose_info.y=slider_info.y-expose_info.height-
                  slider_info.bevel_width-1;
              }
            else
              {
                expose_info.height=expose_info.y-slider_info.y;
                expose_info.y=slider_info.y+slider_info.height+
                  slider_info.bevel_width+1;
              }
            XDrawTriangleNorth(display,&windows->popup,&north_info);
            XDrawMatte(display,&windows->popup,&expose_info);
            XDrawBeveledButton(display,&windows->popup,&slider_info);
            XDrawTriangleSouth(display,&windows->popup,&south_info);
            expose_info.y=slider_info.y;
          }
        state&=(~RedrawListState);
      }
    if (state & RedrawActionState)
      {
        /*
          Display the selected color in a drawing area.
        */
        color=windows->popup.pixel_info->matte_color;
        (void) XParseColor(display,windows->popup.map_info->colormap,
          reply_info.text,&windows->popup.pixel_info->matte_color);
        XBestPixel(display,windows->popup.map_info->colormap,(XColor *) NULL,
          (unsigned int) windows->popup.visual_info->colormap_size,
          &windows->popup.pixel_info->matte_color);
        XDrawBeveledButton(display,&windows->popup,&mode_info);
        windows->popup.pixel_info->matte_color=color;
        state&=(~RedrawActionState);
      }
    /*
      Wait for next event.
    */
    if (north_info.raised && south_info.raised)
      XIfEvent(display,&event,XScreenEvent,(char *) windows);
    else
      {
        /*
          Brief delay before advancing scroll bar.
        */
        XDelay(display,delay);
        XCheckMaskEvent(display,ButtonReleaseMask,&event);
        delay=SuspendTime;
      }
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(slider_info,event.xbutton))
          {
            /*
              Track slider.
            */
            slider_info.active=True;
            break;
          }
        if (MatteIsActive(north_info,event.xbutton))
          if (slider_info.id > 0)
            {
              /*
                Move slider up.
              */
              north_info.raised=False;
              slider_info.id--;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(south_info,event.xbutton))
          if (slider_info.id < colors)
            {
              /*
                Move slider down.
              */
              south_info.raised=False;
              slider_info.id++;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(scroll_info,event.xbutton))
          {
            /*
              Move slider.
            */
            if (event.xbutton.y < slider_info.y)
              slider_info.id-=(visible_colors-1);
            else
              slider_info.id+=(visible_colors-1);
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(list_info,event.xbutton))
          {
            unsigned int
              id;

            /*
              User pressed list matte.
            */
            id=slider_info.id+(event.xbutton.y-(list_info.y+(height >> 1))+1)/
              selection_info.height;
            if (id >= colors)
              break;
            (void) strcpy(reply_info.text,colorlist[id]);
            reply_info.highlight=False;
            reply_info.marker=reply_info.text;
            reply_info.cursor=reply_info.text+strlen(reply_info.text);
            XDrawMatteText(display,&windows->popup,&reply_info);
            state|=RedrawActionState;
            if (id == list_info.id)
              {
                (void) strcpy(glob_pattern,reply_info.text);
                state|=UpdateListState;
              }
            selection_info.id=(~0);
            list_info.id=id;
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(grab_info,event.xbutton))
          {
            /*
              User pressed Grab button.
            */
            grab_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&grab_info);
            break;
          }
        if (MatteIsActive(reset_info,event.xbutton))
          {
            /*
              User pressed Reset button.
            */
            reset_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
            break;
          }
        if (MatteIsActive(action_info,event.xbutton))
          {
            /*
              User pressed action button.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (MatteIsActive(cancel_info,event.xbutton))
          {
            /*
              User pressed Cancel button.
            */
            cancel_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        if (!MatteIsActive(reply_info,event.xbutton))
          break;
        if (event.xbutton.button != Button2)
          {
            static Time
              click_time;

            /*
              Move text cursor to position of button press.
            */
            x=event.xbutton.x-reply_info.x-(font_info->max_bounds.width >> 2);
            for (i=1; i <= (int) strlen(reply_info.marker); i++)
              if (XTextWidth(font_info,reply_info.marker,i) > x)
                break;
            reply_info.cursor=reply_info.marker+i-1;
            if (event.xbutton.time < (click_time+DoubleClick))
              {
                /*
                  Become the XA_PRIMARY selection owner.
                */
                (void) strcpy(primary_selection,reply_info.text);
                XSetSelectionOwner(display,XA_PRIMARY,windows->popup.id,
                  event.xbutton.time);
                reply_info.highlight=XGetSelectionOwner(display,XA_PRIMARY) ==
                  windows->popup.id;
              }
            XDrawMatteText(display,&windows->popup,&reply_info);
            click_time=event.xbutton.time;
            break;
          }
        /*
          Request primary selection.
        */
        XConvertSelection(display,XA_PRIMARY,XA_STRING,XA_STRING,
          windows->popup.id,event.xbutton.time);
        break;
      }
      case ButtonRelease:
      {
        if (!north_info.raised)
          {
            /*
              User released up button.
            */
            delay=SuspendTime << 2;
            north_info.raised=True;
            XDrawTriangleNorth(display,&windows->popup,&north_info);
          }
        if (!south_info.raised)
          {
            /*
              User released down button.
            */
            delay=SuspendTime << 2;
            south_info.raised=True;
            XDrawTriangleSouth(display,&windows->popup,&south_info);
          }
        if (slider_info.active)
          {
            /*
              Stop tracking slider.
            */
            slider_info.active=False;
            break;
          }
        if (!grab_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(grab_info,event.xbutton))
                {
                  unsigned int
                    status;

                  XColor
                    color;

                  /*
                    Select a pen color from the X server.
                  */
                  status=XGetWindowColor(display,&color);
                  if (status != False)
                    {
                      (void) sprintf(reply_info.text,"#%02x%02x%02x",
                        color.red >> 8,color.green >> 8,color.blue >> 8);
                      reply_info.marker=reply_info.text;
                      reply_info.cursor=reply_info.text+strlen(reply_info.text);
                      XDrawMatteText(display,&windows->popup,&reply_info);
                      state|=RedrawActionState;
                    }
                }
            grab_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&grab_info);
          }
        if (!reset_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(reset_info,event.xbutton))
                {
                  (void) strcpy(glob_pattern,reset_pattern);
                  state|=UpdateListState;
                }
            reset_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
          }
        if (!action_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(action_info,event.xbutton))
                if (*reply_info.text == '\0')
                  XBell(display,0);
                else
                  state|=ExitState;
            action_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&action_info);
          }
        if (!cancel_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(cancel_info,event.xbutton))
                {
                  *reply_info.text='\0';
                  state|=ExitState;
                }
            cancel_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
          }
        if (!MatteIsActive(reply_info,event.xbutton))
          break;
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
        if (event.xclient.window == windows->popup.id)
          {
            *reply_info.text='\0';
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
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

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (MatteIsActive(scroll_info,event.xkey))
          {
            /*
              Move slider.
            */
            switch (key_symbol)
            {
              case XK_Home:
              case XK_KP_Home:
              {
                slider_info.id=0;
                break;
              }
              case XK_Up:
              case XK_KP_Up:
              {
                slider_info.id--;
                break;
              }
              case XK_Down:
              case XK_KP_Down:
              {
                slider_info.id++;
                break;
              }
              case XK_Prior:
              case XK_KP_Prior:
              {
                slider_info.id-=visible_colors;
                break;
              }
              case XK_Next:
              case XK_KP_Next:
              {
                slider_info.id+=visible_colors;
                break;
              }
              case XK_End:
              case XK_KP_End:
              {
                slider_info.id=colors;
                break;
              }
            }
            state|=RedrawListState;
            break;
          }
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            /*
              Read new color or glob patterm.
            */
            if (*reply_info.text == '\0')
              break;
            (void) strcpy(glob_pattern,reply_info.text);
            state|=UpdateListState;
            break;
          }
        if (!MatteIsActive(reply_info,event.xkey))
          break;
        if (key_symbol == XK_Control_L)
          {
            state|=ControlState;
            break;
          }
        if (state & ControlState)
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
        XEditText(display,&reply_info,key_symbol,command,state);
        XDrawMatteText(display,&windows->popup,&reply_info);
        status=XParseColor(display,windows->popup.map_info->colormap,
          reply_info.text,&color);
        if (status != 0)
          state|=RedrawActionState;
        break;
      }
      case KeyRelease:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if (key_symbol == XK_Control_L)
          state&=(~ControlState);
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (slider_info.active)
          {
            /*
              Move slider matte.
            */
            slider_info.y=event.xmotion.y-
              ((slider_info.height+slider_info.bevel_width) >> 1)+1;
            if (slider_info.y < slider_info.min_y)
              slider_info.y=slider_info.min_y;
            if (slider_info.y > slider_info.max_y)
              slider_info.y=slider_info.max_y;
            slider_info.id=(colors*(slider_info.y-slider_info.min_y+1))/
              (slider_info.max_y-slider_info.min_y+1);
            state|=RedrawListState;
            break;
          }
        if (state & InactiveWidgetState)
          break;
        if (grab_info.raised == MatteIsActive(grab_info,event.xmotion))
          {
            /*
              Grab button status changed.
            */
            grab_info.raised=!grab_info.raised;
            XDrawBeveledButton(display,&windows->popup,&grab_info);
            break;
          }
        if (reset_info.raised == MatteIsActive(reset_info,event.xmotion))
          {
            /*
              Reset button status changed.
            */
            reset_info.raised=!reset_info.raised;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
            break;
          }
        if (action_info.raised == MatteIsActive(action_info,event.xmotion))
          {
            /*
              Action button status changed.
            */
            action_info.raised=!action_info.raised;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (cancel_info.raised == MatteIsActive(cancel_info,event.xmotion))
          {
            /*
              Cancel button status changed.
            */
            cancel_info.raised=!cancel_info.raised;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        break;
      }
      case SelectionClear:
      {
        reply_info.highlight=False;
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case SelectionNotify:
      {
        Atom
          type;

        int
          format,
          status;

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
        if ((strlen(reply_info.text)+length) >= MaxTextLength)
          XBell(display,0);
        else
          {
            /*
              Insert primary selection in reply text.
            */
            *(data+length)='\0';
            XEditText(display,&reply_info,(KeySym) XK_Insert,(char *) data,
              state);
            XDrawMatteText(display,&windows->popup,&reply_info);
            state|=RedrawActionState;
          }
        XFree((void *) data);
        break;
      }
      case SelectionRequest:
      {
        XSelectionEvent
          notify;

        XSelectionRequestEvent
          *request;

        if (!reply_info.highlight)
          break;
        /*
          Set primary selection.
        */
        request=(&(event.xselectionrequest));
        XChangeProperty(request->display,request->requestor,request->property,
          request->target,8,PropModeReplace,(unsigned char *) primary_selection,
          strlen(primary_selection));
        notify.type=SelectionNotify;
        notify.send_event=True;
        notify.display=request->display;
        notify.requestor=request->requestor;
        notify.selection=request->selection;
        notify.target=request->target;
        notify.time=request->time;
        if (request->property == None)
          notify.property=request->target;
        else
          notify.property=request->property;
        (void) XSendEvent(request->display,request->requestor,False,NoEventMask,
          (XEvent *) &notify);
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
  /*
    Free color list.
  */
  for (i=0; i < colors; i++)
    (void) free((char *) colorlist[i]);
  if (colorlist != (char **) NULL)
    (void) free((char *) colorlist);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X C o m m a n d W i d g e t                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XCommandWidget maps a menu and returns the command pointed to by
%  the user when the button is released.
%
%  The format of the XCommandWidget routine is:
%
%    selection_number=XCommandWidget(display,windows,item)
%
%  A description of each parameter follows:
%
%    o selection_number: Specifies the number of the selection that the
%      user choose.
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o windows: Specifies a pointer to a XWindows structure.
%
%    o item: Specifies a character array.  The item selected from the menu
%      is returned here.
%
%
*/
KeySym XCommandWidget(display,windows,item)
Display
  *display;

XWindows
  *windows;

char
  *item;
{
#define MenuTitle  "Commands"
#define NumberMenus  8
#define XDrawSubmenuMarker(display,window_info)  \
  if (selection_info.raised) \
    XDrawString(display,window_info.id,window_info.annotate_context, \
      submenu_info.x,selection_info.y+window_info.font_info->ascent,">",1); \
  else \
    XDrawString(display,window_info.id,window_info.highlight_context, \
      submenu_info.x,selection_info.y+window_info.font_info->ascent,">",1);

  static char
    *CommandMenu[]=
    {
      "File",
      "Edit",
      "View",
      "Transform",
      "Enhance",
      "Effects",
      "Image Edit",
      "Help",
      "Quit",
      (char *) NULL
    },
    *FileMenu[]=
    {
      "Image Info",
      "Write...",
      "Print...",
      "Delete...",
      "Load...",
      "Next",
      "Former",
      (char *) NULL
    },
    *EditMenu[]=
    {
      "Undo",
      "Restore",
      "Refresh",
      (char *) NULL
    },
    *ViewMenu[]=
    {
      "Half Size",
      "Original Size",
      "Double Size",
      "Resize...",
      (char *) NULL
    },
    *TransformMenu[]=
    {
      "Trim Edges",
      "Clip",
      "Cut",
      "Flop",
      "Flip",
      "Rotate Right",
      "Rotate Left",
      "Rotate...",
      "Shear...",
      (char *) NULL
    },
    *EnhanceMenu[]=
    {
      "Negate",
      "Equalize",
      "Normalize",
      "Gamma...",
      (char *) NULL
    },
    *EffectsMenu[]=
    {
      "Despeckle",
      "Peak Noise",
      "Sharpen",
      "Blur",
      "Edge Detect",
      "Grayscale",
      "Quantize...",
      (char *) NULL
    },
    *ImageEditMenu[]=
    {
      "Annotate",
      "Add Border...",
      "Composite...",
      "Color",
      "Matte",
      "Comment",
      (char *) NULL
    },
    *HelpMenu[]=
    {
      "About IM",
      "Version...",
      (char *) NULL
    };

  static char
    **Menus[NumberMenus]=
    {
      FileMenu,
      EditMenu,
      ViewMenu,
      TransformMenu,
      EnhanceMenu,
      EffectsMenu,
      ImageEditMenu,
      HelpMenu
    };

  static KeySym
    CommandKeys[]=
    {
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_VoidSymbol,
      XK_h,
      XK_q
    },
    FileKeys[]=
    {
      XK_i,
      XK_w,
      XK_p,
      XK_d,
      XK_l,
      XK_n,
      XK_f
    },
    EditKeys[]=
    {
      XK_u,
      XK_r,
      XK_at
    },
    ViewKeys[]=
    {
      XK_less,
      XK_o,
      XK_greater,
      XK_percent,
    },
    TransformKeys[]=
    {
      XK_t,
      XK_bracketleft,
      XK_bracketright,
      XK_bar,
      XK_minus,
      XK_slash,
      XK_backslash,
      XK_asterisk,
      XK_s
    },
    EnhanceKeys[]=
    {
      XK_asciitilde,
      XK_equal,
      XK_N,
      XK_g
    },
    EffectsKeys[]=
    {
      XK_D,
      XK_P,
      XK_S,
      XK_B,
      XK_E,
      XK_G,
      XK_Q
    },
    ImageEditKeys[]=
    {
      XK_a,
      XK_b,
      XK_x,
      XK_c,
      XK_m,
      XK_numbersign
    },
    HelpKeys[]=
    {
      XK_h,
      XK_v
    };

  static KeySym
    *Keys[NumberMenus]=
    {
      FileKeys,
      EditKeys,
      ViewKeys,
      TransformKeys,
      EnhanceKeys,
      EffectsKeys,
      ImageEditKeys,
      HelpKeys,
    };

  Cursor
    cursor;

  int
    id,
    x,
    y;

  KeySym
    key_symbol;

  unsigned int
    height,
    limit,
    mask,
    number_selections,
    title_height,
    width;

  unsigned long
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XWidgetInfo
    menu_info,
    selection_info,
    submenu_info;

  XWindowChanges
    window_changes;

  /*
    Determine command window attributes.
  */
  font_info=windows->command.font_info;
  windows->command.width=XTextWidth(font_info,MenuTitle,strlen(MenuTitle));
  for (id=0; CommandMenu[id] != (char *) NULL; id++)
  {
    width=XTextWidth(font_info,CommandMenu[id],strlen(CommandMenu[id]))+
      XTextWidth(font_info," -",2);
    if (width > windows->command.width)
      windows->command.width=width;
  }
  number_selections=id;
  windows->command.width+=font_info->max_bounds.width+6;
  title_height=(font_info->descent+font_info->ascent) << 1;
  width=XTextWidth(font_info,MenuTitle,strlen(MenuTitle));
  height=(9*(font_info->ascent+font_info->descent)) >> 3;
  XGetWidgetInfo((char *) NULL,&menu_info);
  windows->command.height=
    title_height+number_selections*height+(menu_info.bevel_width << 1);
  windows->command.min_width=windows->command.width;
  windows->command.min_height=windows->command.height;
  /*
    Position command window.
  */
  XQueryPointer(display,XRootWindow(display,windows->command.screen),
    &root_window,&root_window,&x,&y,&windows->command.x,&windows->command.y,
    &mask);
  windows->command.x=x-(font_info->max_bounds.width >> 1);
  limit=XDisplayWidth(display,windows->command.screen)-windows->command.width;
  if (windows->command.x < 0)
    windows->command.x=0;
  else
    if (windows->command.x > limit)
      windows->command.x=limit;
  windows->command.y=y-((3*title_height) >> 2);
  limit=XDisplayHeight(display,windows->command.screen)-windows->command.height;
  if (windows->command.y < 0)
    windows->command.y=0;
  else
    if (windows->command.y > limit)
      windows->command.y=limit;
  /*
    Map command window.
  */
  window_changes.width=windows->command.width;
  window_changes.height=windows->command.height;
  window_changes.x=windows->command.x;
  window_changes.y=windows->command.y;
  XReconfigureWMWindow(display,windows->command.id,windows->command.screen,
    CWWidth | CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->command.id);
  /*
    Respond to X events.
  */
  *item='\0';
  key_symbol=XK_VoidSymbol;
  selection_info.height=height;
  cursor=XCreateFontCursor(display,XC_right_ptr);
  XDefineCursor(display,windows->image.id,cursor);
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        /*
          Initialize selection information.
        */
        XClearWindow(display,windows->command.id);
        XGetWidgetInfo((char *) NULL,&menu_info);
        menu_info.bevel_width--;
        menu_info.width=windows->command.width-((menu_info.bevel_width) << 1);
        menu_info.height=windows->command.height-((menu_info.bevel_width) << 1);
        menu_info.x=menu_info.bevel_width;
        menu_info.y=menu_info.bevel_width;
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=menu_info.width;
        selection_info.height=height;
        selection_info.x=menu_info.x;
        XGetWidgetInfo((char *) NULL,&submenu_info);
        submenu_info.width=font_info->max_bounds.width;
        submenu_info.height=height;
        submenu_info.x=selection_info.width-font_info->max_bounds.width;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw Command widget.
        */
        XDrawBevel(display,&windows->command,&menu_info);
        y=title_height-1;
        XSetBevelColor(display,&windows->command,False);
        XDrawLine(display,windows->command.id,windows->command.widget_context,
          selection_info.x,y-1,(int) selection_info.width,y-1);
        XSetBevelColor(display,&windows->command,True);
        XDrawLine(display,windows->command.id,windows->command.widget_context,
          selection_info.x,y,(int) selection_info.width,y);
        XSetFillStyle(display,windows->command.widget_context,FillSolid);
        /*
          Draw menu selections.
        */
        selection_info.raised=True;
        selection_info.y=title_height >> 2;
        selection_info.text=MenuTitle;
        XDrawWidgetText(display,&windows->command,&selection_info);
        selection_info.y=title_height;
        for (id=0; id < number_selections; id++)
        {
          selection_info.raised=(id != selection_info.id);
          selection_info.text=CommandMenu[id];
          XDrawWidgetText(display,&windows->command,&selection_info);
          if ((id >= 0) && (id < NumberMenus))
            XDrawSubmenuMarker(display,windows->command);
          selection_info.y+=(int) selection_info.height;
        }
        state&=(~RedrawWidgetState);
      }
    if (number_selections > 2)
      {
        /*
          Redraw menu line.
        */
        y=title_height+selection_info.height*(number_selections-1);
        XSetBevelColor(display,&windows->command,False);
        XDrawLine(display,windows->command.id,windows->command.widget_context,
          selection_info.x,y-1,(int) selection_info.width,y-1);
        XSetBevelColor(display,&windows->command,True);
        XDrawLine(display,windows->command.id,windows->command.widget_context,
          selection_info.x,y,(int) selection_info.width,y);
        XSetFillStyle(display,windows->command.widget_context,FillSolid);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
        break;
      case ButtonRelease:
      {
        /*
          Exit menu.
        */
        if ((selection_info.id >= 0) && (selection_info.id < number_selections))
          {
            (void) strcpy(item,CommandMenu[selection_info.id]);
            key_symbol=CommandKeys[selection_info.id];
          }
        state|=ExitState;
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->command.id)
          break;
        if ((event.xconfigure.width == windows->command.width) &&
            (event.xconfigure.height == windows->command.height))
          break;
        windows->command.width=
          Max(event.xconfigure.width,windows->command.min_width);
        windows->command.height=
          Max(event.xconfigure.height,windows->command.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->command.id)
          break;
        state&=(~InactiveWidgetState);
        id=((event.xcrossing.y-title_height)/(int) selection_info.height);
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Highlight this selection.
        */
        selection_info.id=id;
        selection_info.y=title_height+selection_info.id*selection_info.height;
        selection_info.raised=False;
        selection_info.text=CommandMenu[selection_info.id];
        XDrawWidgetText(display,&windows->command,&selection_info);
        if ((id >= 0) && (id < NumberMenus))
          XDrawSubmenuMarker(display,windows->command);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->command.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->command.id)
          break;
        state|=InactiveWidgetState;
        id=selection_info.id;
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Unhighlight last selection.
        */
        selection_info.id=(~0);
        selection_info.y=title_height+id*selection_info.height;
        selection_info.raised=True;
        selection_info.text=CommandMenu[id];
        XDrawWidgetText(display,&windows->command,&selection_info);
        if ((id >= 0) && (id < NumberMenus))
          XDrawSubmenuMarker(display,windows->command);
        break;
      }
      case MotionNotify:
      {
        int
          entry;

        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (state & InactiveWidgetState)
          break;
        id=(event.xmotion.y-title_height)/(int) selection_info.height;
        if ((selection_info.id >= 0) && (selection_info.id < number_selections))
          {
            if (id == selection_info.id)
              if (event.xmotion.x <= submenu_info.x)
                break;
              else
                if ((id < 0) || (id >= NumberMenus))
                  break;
                else
                  {
                    /*
                      Select item from sub-menu.
                    */
                    entry=
                      XMenuWidget(display,windows,(char *) NULL,Menus[id],item);
                    if (entry >= 0)
                      {
                        (void) strcpy(item,Menus[id][entry]);
                        key_symbol=Keys[id][entry];
                        state|=ExitState;
                        break;
                      }
                  }
            /*
              Unhighlight last selection.
            */
            selection_info.y=
              title_height+selection_info.id*selection_info.height;
            selection_info.raised=True;
            selection_info.text=CommandMenu[selection_info.id];
            XDrawWidgetText(display,&windows->command,&selection_info);
            if ((selection_info.id >= 0) && (selection_info.id < NumberMenus))
              XDrawSubmenuMarker(display,windows->command);
            if (id == selection_info.id)
              break;
          }
        selection_info.id=id;
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Highlight this selection.
        */
        XFlush(display);
        selection_info.y=title_height+id*selection_info.height;
        selection_info.raised=False;
        selection_info.text=CommandMenu[id];
        XDrawWidgetText(display,&windows->command,&selection_info);
        if ((id >= 0) && (id < NumberMenus))
          XDrawSubmenuMarker(display,windows->command);
        break;
      }
      case UnmapNotify:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        XDrawBevel(display,&windows->command,&menu_info);
        XCheckRefreshWindow(display,&windows->image);
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->command.id,windows->command.screen);
  return(key_symbol);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X D i a l o g W i d g e t                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XDialogWidget displays a popup window with a query to the user.
%  The user keys a reply and presses the Ok or Cancel button to exit.  The
%  typed text is returned as the reply function parameter.
%
%  The format of the XDialogWidget routine is:
%
%    XDialogWidget(display,windows,action,query,reply)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o action: Specifies a pointer to the action of this widget.
%
%    o query: Specifies a pointer to the query to present to the user.
%
%    o reply: The response from the user is returned in this parameter.
%
%
*/
void XDialogWidget(display,windows,action,query,reply)
Display
  *display;

XWindows
  *windows;

char
  *action,
  *query,
  *reply;
{
#define CancelButtonText  "Cancel"

  char
    primary_selection[MaxTextLength];

  int
    x,
    y;

  register int
    i;

  unsigned int
    height,
    limit,
    mask,
    width;

  unsigned long
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    action_info,
    cancel_info,
    reply_info,
    text_info;

  XWindowChanges
    window_changes;

  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  width=XTextWidth(font_info,action,strlen(action));
  if (XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText)) > width)
    width=XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText));
  width+=(3*font_info->max_bounds.width) >> 1;
  height=font_info->ascent+font_info->descent;
  windows->popup.width=Max(width,XTextWidth(font_info,query,strlen(query)))+
    6*font_info->max_bounds.width;
  windows->popup.height=7*height+(font_info->max_bounds.width << 1);
  windows->popup.min_width=width+25*XTextWidth(font_info,"#",1)+
    4*font_info->max_bounds.width;
  windows->popup.min_height=windows->popup.height;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=(windows->popup.width >> 1);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=(windows->popup.height >> 1);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,"Dialog");
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  state=RedrawWidgetState | UpdateConfigurationState;
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  do
  {
    if (state & UpdateConfigurationState)
      {
        /*
          Initialize button information.
        */
        XGetWidgetInfo(CancelButtonText,&cancel_info);
        cancel_info.width=width;
        cancel_info.height=(3*height) >> 1;
        cancel_info.x=windows->popup.width-cancel_info.width-
          ((3*font_info->max_bounds.width) >> 1);
        cancel_info.y=windows->popup.height-cancel_info.height-
          ((3*font_info->max_bounds.width) >> 1);
        XGetWidgetInfo(action,&action_info);
        action_info.width=width;
        action_info.height=(3*height) >> 1;
        action_info.x=cancel_info.x-(cancel_info.width+
          font_info->max_bounds.width+(action_info.bevel_width << 1));
        action_info.y=cancel_info.y;
        /*
          Initialize reply information.
        */
        XGetWidgetInfo(reply,&reply_info);
        reply_info.raised=False;
        reply_info.bevel_width--;
        reply_info.width=windows->popup.width-(3*font_info->max_bounds.width);
        reply_info.height=height << 1;
        reply_info.x=(3*font_info->max_bounds.width) >> 1;
        reply_info.y=action_info.y-reply_info.height-
          font_info->max_bounds.width;
        /*
          Initialize text information.
        */
        XGetWidgetInfo(query,&text_info);
        text_info.width=reply_info.width;
        text_info.height=height;
        text_info.x=reply_info.x-(font_info->max_bounds.width >> 1);
        text_info.y=font_info->max_bounds.width;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw Dialog widget.
        */
        XClearWindow(display,windows->popup.id);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawBeveledMatte(display,&windows->popup,&reply_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledButton(display,&windows->popup,&action_info);
        XDrawBeveledButton(display,&windows->popup,&cancel_info);
        XHighlightWidget(display,&windows->popup,4,4);
        state&=(~RedrawWidgetState);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(action_info,event.xbutton))
          {
            /*
              User pressed Action button.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (MatteIsActive(cancel_info,event.xbutton))
          {
            /*
              User pressed Cancel button.
            */
            cancel_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        if (!MatteIsActive(reply_info,event.xbutton))
          break;
        if (event.xbutton.button != Button2)
          {
            static Time
              click_time;

            /*
              Move text cursor to position of button press.
            */
            x=event.xbutton.x-reply_info.x-(font_info->max_bounds.width >> 2);
            for (i=1; i <= (int) strlen(reply_info.marker); i++)
              if (XTextWidth(font_info,reply_info.marker,i) > x)
                break;
            reply_info.cursor=reply_info.marker+i-1;
            if (event.xbutton.time < (click_time+DoubleClick))
              {
                /*
                  Become the XA_PRIMARY selection owner.
                */
                (void) strcpy(primary_selection,reply_info.text);
                XSetSelectionOwner(display,XA_PRIMARY,windows->popup.id,
                  event.xbutton.time);
                reply_info.highlight=XGetSelectionOwner(display,XA_PRIMARY) ==
                  windows->popup.id;
              }
            XDrawMatteText(display,&windows->popup,&reply_info);
            click_time=event.xbutton.time;
            break;
          }
        /*
          Request primary selection.
        */
        XConvertSelection(display,XA_PRIMARY,XA_STRING,XA_STRING,
          windows->popup.id,event.xbutton.time);
        break;
      }
      case ButtonRelease:
      {
        if (!action_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(action_info,event.xbutton))
                if (*reply_info.text == '\0')
                  XBell(display,0);
                else
                  state|=ExitState;
            action_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&action_info);
          }
        if (!cancel_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(cancel_info,event.xbutton))
                {
                  *reply_info.text='\0';
                  state|=ExitState;
                }
            cancel_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
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
        if (event.xclient.window == windows->popup.id)
          {
            *reply_info.text='\0';
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
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

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            if (*reply_info.text != '\0')
              {
                action_info.raised=False;
                XDrawBeveledButton(display,&windows->popup,&action_info);
                state|=ExitState;
              }
            break;
          }
        if (!MatteIsActive(reply_info,event.xkey))
          break;
        if (key_symbol == XK_Control_L)
          {
            state|=ControlState;
            break;
          }
        if (state & ControlState)
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
        XEditText(display,&reply_info,key_symbol,command,state);
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case KeyRelease:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if (key_symbol == XK_Control_L)
          state&=(~ControlState);
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (state & InactiveWidgetState)
          break;
        if (action_info.raised == MatteIsActive(action_info,event.xmotion))
          {
            /*
              Action button status changed.
            */
            action_info.raised=!action_info.raised;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (cancel_info.raised == MatteIsActive(cancel_info,event.xmotion))
          {
            /*
              Cancel button status changed.
            */
            cancel_info.raised=!cancel_info.raised;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        break;
      }
      case SelectionClear:
      {
        reply_info.highlight=False;
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case SelectionNotify:
      {
        Atom
          type;

        int
          format,
          status;

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
        if ((strlen(reply_info.text)+length) >= MaxTextLength)
          XBell(display,0);
        else
          {
            /*
              Insert primary selection in reply text.
            */
            *(data+length)='\0';
            XEditText(display,&reply_info,(KeySym) XK_Insert,(char *) data,
              state);
            XDrawMatteText(display,&windows->popup,&reply_info);
          }
        XFree((void *) data);
        break;
      }
      case SelectionRequest:
      {
        XSelectionEvent
          notify;

        XSelectionRequestEvent
          *request;

        if (!reply_info.highlight)
          break;
        /*
          Set primary selection.
        */
        request=(&(event.xselectionrequest));
        XChangeProperty(request->display,request->requestor,request->property,
          request->target,8,PropModeReplace,(unsigned char *) primary_selection,
          strlen(primary_selection));
        notify.type=SelectionNotify;
        notify.display=request->display;
        notify.requestor=request->requestor;
        notify.selection=request->selection;
        notify.target=request->target;
        notify.time=request->time;
        if (request->property == None)
          notify.property=request->target;
        else
          notify.property=request->property;
        (void) XSendEvent(request->display,request->requestor,False,0,
          (XEvent *) &notify);
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X F i l e B r o w s e r W i d g e t                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XFileBrowserWidget displays a popup window with a file query to
%  the user.  The user keys a reply and presses the Action or Cancel button
%  to exit.  The typed text is returned as the reply function parameter.
%
%  The format of the XFileBrowserWidget routine is:
%
%    XFileBrowserWidget(display,windows,action,reply)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o action: Specifies a pointer to the action of this widget.
%
%    o reply: The response from the user is returned in this parameter.
%
%
*/
void XFileBrowserWidget(display,windows,action,reply)
Display
  *display;

XWindows
  *windows;

char
  *action,
  *reply;
{
#define CancelButtonText  "Cancel"
#define DirectoryText  "Directory:"
#define FilenameText  "File name:"
#define HomeButtonText  "Home"
#define UpButtonText  "Up"

  char
    **filelist,
    home_directory[MaxTextLength],
    primary_selection[MaxTextLength],
    text[MaxTextLength],
    working_directory[MaxTextLength];

  int
    files,
    x,
    y;

  register int
    i;

  static char
    glob_pattern[MaxTextLength] = "*";

  unsigned int
    height,
    limit,
    mask,
    text_width,
    visible_files,
    width;

  unsigned long
    delay,
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    action_info,
    cancel_info,
    expose_info,
    list_info,
    home_info,
    north_info,
    reply_info,
    scroll_info,
    selection_info,
    slider_info,
    south_info,
    text_info,
    up_info;

  XWindowChanges
    window_changes;

  /*
    Read filelist from current directory.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  (void) getcwd(home_directory,MaxTextLength-1);
  (void) strcpy(working_directory,home_directory);
  filelist=ListFiles(working_directory,glob_pattern,&files);
  if (filelist == (char **) NULL)
    {
      /*
        Directory read failed.
      */
      XNoticeWidget(display,windows,"Unable to read directory",
        working_directory);
      return;
    }
  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  text_width=0;
  for (i=0; i < files; i++)
    if (XTextWidth(font_info,filelist[i],strlen(filelist[i])) > text_width)
      text_width=XTextWidth(font_info,filelist[i],strlen(filelist[i]));
  width=XTextWidth(font_info,action,strlen(action));
  if (XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText)) > width)
    width=XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText));
  if (XTextWidth(font_info,HomeButtonText,strlen(HomeButtonText)) > width)
    width=XTextWidth(font_info,HomeButtonText,strlen(HomeButtonText));
  if (XTextWidth(font_info,UpButtonText,strlen(UpButtonText)) > width)
    width=XTextWidth(font_info,UpButtonText,strlen(UpButtonText));
  width+=font_info->max_bounds.width;
  if (XTextWidth(font_info,DirectoryText,strlen(DirectoryText)) > width)
    width=XTextWidth(font_info,DirectoryText,strlen(DirectoryText));
  if (XTextWidth(font_info,FilenameText,strlen(FilenameText)) > width)
    width=XTextWidth(font_info,FilenameText,strlen(FilenameText));
  height=font_info->ascent+font_info->descent;
  windows->popup.width=width+text_width+6*font_info->max_bounds.width;
  windows->popup.height=
    ((81*height) >> 2)+((13*font_info->max_bounds.width) >> 1)+4;
  windows->popup.min_width=width+25*XTextWidth(font_info,"#",1)+
    4*font_info->max_bounds.width;
  windows->popup.min_height=
    ((23*height) >> 1)+((13*font_info->max_bounds.width) >> 1)+4;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=((3*windows->popup.width) >> 2);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=(windows->popup.height >> 1);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,"Browse and Select a File");
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  XGetWidgetInfo((char *) NULL,&slider_info);
  XGetWidgetInfo((char *) NULL,&north_info);
  XGetWidgetInfo((char *) NULL,&south_info);
  visible_files=0;
  delay=SuspendTime << 2;
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        int
          id;

        /*
          Initialize button information.
        */
        XGetWidgetInfo(CancelButtonText,&cancel_info);
        cancel_info.width=width;
        cancel_info.height=(3*height) >> 1;
        cancel_info.x=windows->popup.width-cancel_info.width-
          font_info->max_bounds.width-2;
        cancel_info.y=windows->popup.height-cancel_info.height-
          font_info->max_bounds.width;
        XGetWidgetInfo(action,&action_info);
        action_info.width=width;
        action_info.height=(3*height) >> 1;
        action_info.x=cancel_info.x-(cancel_info.width+
          (font_info->max_bounds.width >> 1)+(action_info.bevel_width << 1));
        action_info.y=cancel_info.y;
        XGetWidgetInfo(UpButtonText,&up_info);
        up_info.width=width;
        up_info.height=(3*height) >> 1;
        up_info.x=font_info->max_bounds.width;
        up_info.y=((5*font_info->max_bounds.width) >> 1)+height;
        XGetWidgetInfo(HomeButtonText,&home_info);
        home_info.width=width;
        home_info.height=(3*height) >> 1;
        home_info.x=font_info->max_bounds.width;
        home_info.y=up_info.y+up_info.height+font_info->max_bounds.width;
        /*
          Initialize reply information.
        */
        XGetWidgetInfo(reply,&reply_info);
        reply_info.raised=False;
        reply_info.bevel_width--;
        reply_info.width=windows->popup.width-width-
          ((6*font_info->max_bounds.width) >> 1);
        reply_info.height=height << 1;
        reply_info.x=width+(font_info->max_bounds.width << 1);
        reply_info.y=
          action_info.y-reply_info.height-font_info->max_bounds.width;
        /*
          Initialize scroll information.
        */
        XGetWidgetInfo((char *) NULL,&scroll_info);
        scroll_info.bevel_width--;
        scroll_info.width=height;
        scroll_info.height=
          reply_info.y-up_info.y-(font_info->max_bounds.width >> 1);
        scroll_info.x=reply_info.x+(reply_info.width-scroll_info.width);
        scroll_info.y=up_info.y-reply_info.bevel_width;
        scroll_info.raised=False;
        scroll_info.trough=True;
        north_info=scroll_info;
        north_info.raised=True;
        north_info.width-=(north_info.bevel_width << 1);
        north_info.height=north_info.width-1;
        north_info.x+=north_info.bevel_width;
        north_info.y+=north_info.bevel_width;
        south_info=north_info;
        south_info.y=scroll_info.y+scroll_info.height-scroll_info.bevel_width-
          south_info.height;
        id=slider_info.id;
        slider_info=north_info;
        slider_info.id=id;
        slider_info.width-=2;
        slider_info.min_y=north_info.y+north_info.height+north_info.bevel_width+
          slider_info.bevel_width+2;
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+2;
        visible_files=(scroll_info.height-(height >> 3)-4)/((9*height) >> 3);
        if (files > visible_files)
          slider_info.height=(visible_files*slider_info.height)/files;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.x=scroll_info.x+slider_info.bevel_width+1;
        slider_info.y=slider_info.min_y;
        expose_info=scroll_info;
        expose_info.y=slider_info.y;
        /*
          Initialize list information.
        */
        XGetWidgetInfo((char *) NULL,&list_info);
        list_info.raised=False;
        list_info.bevel_width--;
        list_info.width=
          scroll_info.x-reply_info.x-(font_info->max_bounds.width >> 1);
        list_info.height=scroll_info.height;
        list_info.x=reply_info.x;
        list_info.y=scroll_info.y;
        /*
          Initialize text information.
        */
        XGetWidgetInfo(text,&text_info);
        text_info.width=reply_info.width;
        text_info.height=height;
        text_info.x=list_info.x-(font_info->max_bounds.width >> 1);
        text_info.y=font_info->max_bounds.width;
        /*
          Initialize selection information.
        */
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=list_info.width;
        selection_info.height=(9*height) >> 3;
        selection_info.x=list_info.x;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw file browser window.
        */
        XClearWindow(display,windows->popup.id);
        x=font_info->max_bounds.width;
        y=text_info.y+((text_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,DirectoryText,strlen(DirectoryText));
        (void) sprintf(text_info.text,"%s/%s",working_directory,glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawBeveledButton(display,&windows->popup,&up_info);
        XDrawBeveledButton(display,&windows->popup,&home_info);
        XDrawBeveledMatte(display,&windows->popup,&list_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        x=font_info->max_bounds.width;
        y=reply_info.y+((reply_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,FilenameText,strlen(FilenameText));
        XDrawBeveledMatte(display,&windows->popup,&reply_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledButton(display,&windows->popup,&action_info);
        XDrawBeveledButton(display,&windows->popup,&cancel_info);
        XHighlightWidget(display,&windows->popup,4,4);
        selection_info.id=(~0);
        state|=RedrawListState;
        state&=(~RedrawWidgetState);
      }
    if (state & UpdateListState)
      {
        char
          **checklist;

        int
          number_files;

        /*
          Update file list.
        */
        checklist=ListFiles(working_directory,glob_pattern,&number_files);
        if (checklist == (char **) NULL)
          {
            /*
              Reply is a filename, exit.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        for (i=0; i < files; i++)
          (void) free((char *) filelist[i]);
        if (filelist != (char **) NULL)
          (void) free((char *) filelist);
        filelist=checklist;
        files=number_files;
        /*
          Update file list.
        */
        if (filelist == (char **) NULL)
          {
            /*
              Reply is a filename, exit.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+1;
        if (files > visible_files)
          slider_info.height=(visible_files*slider_info.height)/files;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.id=0;
        slider_info.y=slider_info.min_y;
        expose_info.y=slider_info.y;
        selection_info.id=(~0);
        list_info.id=(~0);
        state|=RedrawListState;
        /*
          Redraw directory name & reply.
        */
        *reply_info.text='\0';
        reply_info.cursor=reply_info.text;
        (void) sprintf(text_info.text,"%s/%s",working_directory,glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        XHighlightWidget(display,&windows->popup,4,4);
        state&=(~UpdateListState);
      }
    if (state & RedrawListState)
      {
        /*
          Determine slider id and position.
        */
        if (slider_info.id >= (int) (files-visible_files))
          slider_info.id=files-visible_files;
        if ((slider_info.id < 0) || (files <= visible_files))
          slider_info.id=0;
        slider_info.y=slider_info.min_y;
        if (files > 0)
          slider_info.y+=
            slider_info.id*(slider_info.max_y-slider_info.min_y+1)/files;
        if (slider_info.id != selection_info.id)
          {
            /*
              Redraw scroll bar and file names.
            */
            selection_info.id=slider_info.id;
            selection_info.y=list_info.y+(height >> 3)+2;
            for (i=0; i < visible_files; i++)
            {
              selection_info.raised=(slider_info.id+i) != list_info.id;
              selection_info.text=(char *) NULL;
              if ((slider_info.id+i) < files)
                selection_info.text=filelist[slider_info.id+i];
              XDrawWidgetText(display,&windows->popup,&selection_info);
              selection_info.y+=(int) selection_info.height;
            }
            /*
              Update slider.
            */
            if (slider_info.y > expose_info.y)
              {
                expose_info.height=slider_info.y-expose_info.y;
                expose_info.y=slider_info.y-expose_info.height-
                  slider_info.bevel_width-1;
              }
            else
              {
                expose_info.height=expose_info.y-slider_info.y;
                expose_info.y=slider_info.y+slider_info.height+
                  slider_info.bevel_width+1;
              }
            XDrawTriangleNorth(display,&windows->popup,&north_info);
            XDrawMatte(display,&windows->popup,&expose_info);
            XDrawBeveledButton(display,&windows->popup,&slider_info);
            XDrawTriangleSouth(display,&windows->popup,&south_info);
            expose_info.y=slider_info.y;
          }
        state&=(~RedrawListState);
      }
    /*
      Wait for next event.
    */
    if (north_info.raised && south_info.raised)
      XIfEvent(display,&event,XScreenEvent,(char *) windows);
    else
      {
        /*
          Brief delay before advancing scroll bar.
        */
        XDelay(display,delay);
        XCheckMaskEvent(display,ButtonReleaseMask,&event);
        delay=SuspendTime;
      }
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(slider_info,event.xbutton))
          {
            /*
              Track slider.
            */
            slider_info.active=True;
            break;
          }
        if (MatteIsActive(north_info,event.xbutton))
          if (slider_info.id > 0)
            {
              /*
                Move slider up.
              */
              north_info.raised=False;
              slider_info.id--;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(south_info,event.xbutton))
          if (slider_info.id < files)
            {
              /*
                Move slider down.
              */
              south_info.raised=False;
              slider_info.id++;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(scroll_info,event.xbutton))
          {
            /*
              Move slider.
            */
            if (event.xbutton.y < slider_info.y)
              slider_info.id-=(visible_files-1);
            else
              slider_info.id+=(visible_files-1);
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(list_info,event.xbutton))
          {
            unsigned int
              id;

            /*
              User pressed file matte.
            */
            id=slider_info.id+(event.xbutton.y-(list_info.y+(height >> 1))+1)/
              selection_info.height;
            if (id >= files)
              break;
            (void) strcpy(reply_info.text,filelist[id]);
            reply_info.highlight=False;
            reply_info.marker=reply_info.text;
            reply_info.cursor=reply_info.text+strlen(reply_info.text);
            XDrawMatteText(display,&windows->popup,&reply_info);
            if (id == list_info.id)
              {
                (void) strcpy(working_directory,reply_info.text);
                state|=UpdateListState;
              }
            selection_info.id=(~0);
            list_info.id=id;
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(up_info,event.xbutton))
          {
            /*
              User pressed Up button.
            */
            up_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&up_info);
            break;
          }
        if (MatteIsActive(home_info,event.xbutton))
          {
            /*
              User pressed Home button.
            */
            home_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&home_info);
            break;
          }
        if (MatteIsActive(action_info,event.xbutton))
          {
            /*
              User pressed action button.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (MatteIsActive(cancel_info,event.xbutton))
          {
            /*
              User pressed Cancel button.
            */
            cancel_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        if (!MatteIsActive(reply_info,event.xbutton))
          break;
        if (event.xbutton.button != Button2)
          {
            static Time
              click_time;

            /*
              Move text cursor to position of button press.
            */
            x=event.xbutton.x-reply_info.x-(font_info->max_bounds.width >> 2);
            for (i=1; i <= (int) strlen(reply_info.marker); i++)
              if (XTextWidth(font_info,reply_info.marker,i) > x)
                break;
            reply_info.cursor=reply_info.marker+i-1;
            if (event.xbutton.time < (click_time+DoubleClick))
              {
                /*
                  Become the XA_PRIMARY selection owner.
                */
                (void) strcpy(primary_selection,reply_info.text);
                XSetSelectionOwner(display,XA_PRIMARY,windows->popup.id,
                  event.xbutton.time);
                reply_info.highlight=XGetSelectionOwner(display,XA_PRIMARY) ==
                  windows->popup.id;
              }
            XDrawMatteText(display,&windows->popup,&reply_info);
            click_time=event.xbutton.time;
            break;
          }
        /*
          Request primary selection.
        */
        XConvertSelection(display,XA_PRIMARY,XA_STRING,XA_STRING,
          windows->popup.id,event.xbutton.time);
        break;
      }
      case ButtonRelease:
      {
        if (!north_info.raised)
          {
            /*
              User released up button.
            */
            delay=SuspendTime << 2;
            north_info.raised=True;
            XDrawTriangleNorth(display,&windows->popup,&north_info);
          }
        if (!south_info.raised)
          {
            /*
              User released down button.
            */
            delay=SuspendTime << 2;
            south_info.raised=True;
            XDrawTriangleSouth(display,&windows->popup,&south_info);
          }
        if (slider_info.active)
          {
            /*
              Stop tracking slider.
            */
            slider_info.active=False;
            break;
          }
        if (!up_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(up_info,event.xbutton))
                {
                  (void) strcpy(working_directory,"..");
                  state|=UpdateListState;
                }
            up_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&up_info);
          }
        if (!home_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(home_info,event.xbutton))
                {
                  (void) strcpy(working_directory,home_directory);
                  state|=UpdateListState;
                }
            home_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&home_info);
          }
        if (!action_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(action_info,event.xbutton))
                if (*reply_info.text == '\0')
                  XBell(display,0);
                else
                  state|=ExitState;
            action_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&action_info);
          }
        if (!cancel_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(cancel_info,event.xbutton))
                {
                  *reply_info.text='\0';
                  state|=ExitState;
                }
            cancel_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
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
        if (event.xclient.window == windows->popup.id)
          {
            *reply_info.text='\0';
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
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

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (MatteIsActive(scroll_info,event.xkey))
          {
            /*
              Move slider.
            */
            switch (key_symbol)
            {
              case XK_Home:
              case XK_KP_Home:
              {
                slider_info.id=0;
                break;
              }
              case XK_Up:
              case XK_KP_Up:
              {
                slider_info.id--;
                break;
              }
              case XK_Down:
              case XK_KP_Down:
              {
                slider_info.id++;
                break;
              }
              case XK_Prior:
              case XK_KP_Prior:
              {
                slider_info.id-=visible_files;
                break;
              }
              case XK_Next:
              case XK_KP_Next:
              {
                slider_info.id+=visible_files;
                break;
              }
              case XK_End:
              case XK_KP_End:
              {
                slider_info.id=files;
                break;
              }
            }
            state|=RedrawListState;
            break;
          }
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            /*
              Read new directory or glob patterm.
            */
            if (*reply_info.text == '\0')
              break;
            if ((strchr(reply_info.text,'*') != (char *) NULL) ||
                (strchr(reply_info.text,'?') != (char *) NULL) ||
                (strchr(reply_info.text,'{') != (char *) NULL) ||
                (strchr(reply_info.text,'}') != (char *) NULL) ||
                (strchr(reply_info.text,'[') != (char *) NULL) ||
                (strchr(reply_info.text,']') != (char *) NULL))
              (void) strcpy(glob_pattern,reply_info.text);
            else
              (void) strcpy(working_directory,reply_info.text);
            state|=UpdateListState;
            break;
          }
        if (!MatteIsActive(reply_info,event.xkey))
          break;
        if (key_symbol == XK_Control_L)
          {
            state|=ControlState;
            break;
          }
        if (state & ControlState)
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
        XEditText(display,&reply_info,key_symbol,command,state);
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case KeyRelease:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if (key_symbol == XK_Control_L)
          state&=(~ControlState);
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (slider_info.active)
          {
            /*
              Move slider matte.
            */
            slider_info.y=event.xmotion.y-
              ((slider_info.height+slider_info.bevel_width) >> 1)+1;
            if (slider_info.y < slider_info.min_y)
              slider_info.y=slider_info.min_y;
            if (slider_info.y > slider_info.max_y)
              slider_info.y=slider_info.max_y;
            slider_info.id=(files*(slider_info.y-slider_info.min_y+1))/
              (slider_info.max_y-slider_info.min_y+1);
            state|=RedrawListState;
            break;
          }
        if (state & InactiveWidgetState)
          break;
        if (up_info.raised == MatteIsActive(up_info,event.xmotion))
          {
            /*
              Up button status changed.
            */
            up_info.raised=!up_info.raised;
            XDrawBeveledButton(display,&windows->popup,&up_info);
            break;
          }
        if (home_info.raised == MatteIsActive(home_info,event.xmotion))
          {
            /*
              Home button status changed.
            */
            home_info.raised=!home_info.raised;
            XDrawBeveledButton(display,&windows->popup,&home_info);
            break;
          }
        if (action_info.raised == MatteIsActive(action_info,event.xmotion))
          {
            /*
              Action button status changed.
            */
            action_info.raised=!action_info.raised;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (cancel_info.raised == MatteIsActive(cancel_info,event.xmotion))
          {
            /*
              Cancel button status changed.
            */
            cancel_info.raised=!cancel_info.raised;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        break;
      }
      case SelectionClear:
      {
        reply_info.highlight=False;
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case SelectionNotify:
      {
        Atom
          type;

        int
          format,
          status;

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
        if ((strlen(reply_info.text)+length) >= MaxTextLength)
          XBell(display,0);
        else
          {
            /*
              Insert primary selection in reply text.
            */
            *(data+length)='\0';
            XEditText(display,&reply_info,(KeySym) XK_Insert,(char *) data,
              state);
            XDrawMatteText(display,&windows->popup,&reply_info);
          }
        XFree((void *) data);
        break;
      }
      case SelectionRequest:
      {
        XSelectionEvent
          notify;

        XSelectionRequestEvent
          *request;

        if (!reply_info.highlight)
          break;
        /*
          Set primary selection.
        */
        request=(&(event.xselectionrequest));
        XChangeProperty(request->display,request->requestor,request->property,
          request->target,8,PropModeReplace,(unsigned char *) primary_selection,
          strlen(primary_selection));
        notify.type=SelectionNotify;
        notify.display=request->display;
        notify.requestor=request->requestor;
        notify.selection=request->selection;
        notify.target=request->target;
        notify.time=request->time;
        if (request->property == None)
          notify.property=request->target;
        else
          notify.property=request->property;
        (void) XSendEvent(request->display,request->requestor,False,0,
          (XEvent *) &notify);
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
  /*
    Free file list.
  */
  for (i=0; i < files; i++)
    (void) free((char *) filelist[i]);
  if (filelist != (char **) NULL)
    (void) free((char *) filelist);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X F o n t B r o w s e r W i d g e t                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XFontBrowserWidget displays a popup window with a font query to
%  the user.  The user keys a reply and presses the Action or Cancel button
%  to exit.  The typed text is returned as the reply function parameter.
%
%  The format of the XFontBrowserWidget routine is:
%
%    XFontBrowserWidget(display,windows,action,reply)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o action: Specifies a pointer to the action of this widget.
%
%    o reply: The response from the user is returned in this parameter.
%
%
*/
static int FontCompare(x,y)
const void
  *x,
  *y;
{
  register char
    *p,
    *q;

  p=(char *) *((char **) x);
  q=(char *) *((char **) y);
  while ((*p != '\0') && (*q != '\0') && (*p == *q))
  {
    p++;
    q++;
  }
  return(*p-(*q));
}

void XFontBrowserWidget(display,windows,action,reply)
Display
  *display;

XWindows
  *windows;

char
  *action,
  *reply;
{
#define BackButtonText  "Back"
#define CancelButtonText  "Cancel"
#define FontnameText  "Name:"
#define FontPatternText  "Pattern:"
#define ResetButtonText  "Reset"

  char
    back_pattern[MaxTextLength],
    **fontlist,
    **listhead,
    primary_selection[MaxTextLength],
    reset_pattern[MaxTextLength],
    text[MaxTextLength];

  int
    fonts,
    x,
    y;

  register int
    i;

  static char
    glob_pattern[MaxTextLength] = "*";

  unsigned int
    height,
    limit,
    mask,
    text_width,
    visible_fonts,
    width;

  unsigned long
    delay,
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    action_info,
    back_info,
    cancel_info,
    expose_info,
    list_info,
    mode_info,
    north_info,
    reply_info,
    reset_info,
    scroll_info,
    selection_info,
    slider_info,
    south_info,
    text_info;

  XWindowChanges
    window_changes;

  /*
    Get font list and sort in ascending order.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  (void) strcpy(back_pattern,glob_pattern);
  (void) strcpy(reset_pattern,"*");
  fontlist=XListFonts(display,glob_pattern,32767,&fonts);
  if (fonts == 0)
    {
      /*
        Pattern failed, obtain all the fonts.
      */
      XNoticeWidget(display,windows,"Unable to obtain fonts names",
        glob_pattern);
      (void) strcpy(glob_pattern,"*");
      fontlist=XListFonts(display,glob_pattern,32767,&fonts);
      if (fontlist == (char **) NULL)
        {
          XNoticeWidget(display,windows,"Unable to obtain fonts names",
            glob_pattern);
          return;
        }
    }
  /*
    Sort font list in ascending order.
  */
  listhead=fontlist;
  fontlist=(char **) malloc(fonts*sizeof(char **));
  if (fontlist == (char **) NULL)
    {
      XNoticeWidget(display,windows,"Unable to view fonts",
        "Memory allocation failed");
      return;
    }
  for (i=0; i < fonts; i++)
    fontlist[i]=listhead[i];
  (void) qsort((void *) fontlist,fonts,sizeof(char **),FontCompare);
  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  text_width=0;
  for (i=0; i < fonts; i++)
    if (XTextWidth(font_info,fontlist[i],strlen(fontlist[i])) > text_width)
      text_width=XTextWidth(font_info,fontlist[i],strlen(fontlist[i]));
  width=XTextWidth(font_info,action,strlen(action));
  if (XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText)) > width)
    width=XTextWidth(font_info,CancelButtonText,strlen(CancelButtonText));
  if (XTextWidth(font_info,ResetButtonText,strlen(ResetButtonText)) > width)
    width=XTextWidth(font_info,ResetButtonText,strlen(ResetButtonText));
  if (XTextWidth(font_info,BackButtonText,strlen(BackButtonText)) > width)
    width=XTextWidth(font_info,BackButtonText,strlen(BackButtonText));
  width+=font_info->max_bounds.width;
  if (XTextWidth(font_info,FontPatternText,strlen(FontPatternText)) > width)
    width=XTextWidth(font_info,FontPatternText,strlen(FontPatternText));
  if (XTextWidth(font_info,FontnameText,strlen(FontnameText)) > width)
    width=XTextWidth(font_info,FontnameText,strlen(FontnameText));
  height=font_info->ascent+font_info->descent;
  windows->popup.width=width+text_width+6*font_info->max_bounds.width;
  windows->popup.height=
    ((85*height) >> 2)+((13*font_info->max_bounds.width) >> 1)+4;
  windows->popup.min_width=width+25*XTextWidth(font_info,"#",1)+
    4*font_info->max_bounds.width;
  windows->popup.min_height=
    ((27*height) >> 1)+((13*font_info->max_bounds.width) >> 1)+4;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=((3*windows->popup.width) >> 2);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=(windows->popup.height >> 1);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,"Browse and Select a Font");
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  XGetWidgetInfo((char *) NULL,&slider_info);
  XGetWidgetInfo((char *) NULL,&north_info);
  XGetWidgetInfo((char *) NULL,&south_info);
  visible_fonts=0;
  delay=SuspendTime << 2;
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        int
          id;

        /*
          Initialize button information.
        */
        XGetWidgetInfo(CancelButtonText,&cancel_info);
        cancel_info.width=width;
        cancel_info.height=(3*height) >> 1;
        cancel_info.x=windows->popup.width-cancel_info.width-
          font_info->max_bounds.width-2;
        cancel_info.y=windows->popup.height-cancel_info.height-
          font_info->max_bounds.width;
        XGetWidgetInfo(action,&action_info);
        action_info.width=width;
        action_info.height=(3*height) >> 1;
        action_info.x=cancel_info.x-(cancel_info.width+
          (font_info->max_bounds.width >> 1)+(action_info.bevel_width << 1));
        action_info.y=cancel_info.y;
        XGetWidgetInfo(BackButtonText,&back_info);
        back_info.width=width;
        back_info.height=(3*height) >> 1;
        back_info.x=font_info->max_bounds.width;
        back_info.y=((5*font_info->max_bounds.width) >> 1)+height;
        XGetWidgetInfo(ResetButtonText,&reset_info);
        reset_info.width=width;
        reset_info.height=(3*height) >> 1;
        reset_info.x=font_info->max_bounds.width;
        reset_info.y=back_info.y+back_info.height+font_info->max_bounds.width;
        /*
          Initialize reply information.
        */
        XGetWidgetInfo(reply,&reply_info);
        reply_info.raised=False;
        reply_info.bevel_width--;
        reply_info.width=windows->popup.width-width-
          ((6*font_info->max_bounds.width) >> 1);
        reply_info.height=height << 1;
        reply_info.x=width+(font_info->max_bounds.width << 1);
        reply_info.y=
          action_info.y-(action_info.height << 1)-font_info->max_bounds.width;
        /*
          Initialize mode information.
        */
        XGetWidgetInfo(reply,&mode_info);
        mode_info.bevel_width=0;
        mode_info.width=action_info.x-reply_info.x-font_info->max_bounds.width;
        mode_info.height=action_info.height << 1;
        mode_info.x=reply_info.x;
        mode_info.y=action_info.y-action_info.height+action_info.bevel_width;
        /*
          Initialize scroll information.
        */
        XGetWidgetInfo((char *) NULL,&scroll_info);
        scroll_info.bevel_width--;
        scroll_info.width=height;
        scroll_info.height=
          reply_info.y-back_info.y-(font_info->max_bounds.width >> 1);
        scroll_info.x=reply_info.x+(reply_info.width-scroll_info.width);
        scroll_info.y=back_info.y-reply_info.bevel_width;
        scroll_info.raised=False;
        scroll_info.trough=True;
        north_info=scroll_info;
        north_info.raised=True;
        north_info.width-=(north_info.bevel_width << 1);
        north_info.height=north_info.width-1;
        north_info.x+=north_info.bevel_width;
        north_info.y+=north_info.bevel_width;
        south_info=north_info;
        south_info.y=scroll_info.y+scroll_info.height-scroll_info.bevel_width-
          south_info.height;
        id=slider_info.id;
        slider_info=north_info;
        slider_info.id=id;
        slider_info.width-=2;
        slider_info.min_y=north_info.y+north_info.height+north_info.bevel_width+
          slider_info.bevel_width+2;
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+2;
        visible_fonts=
          (scroll_info.height-(height >> 3)-4)/((9*height) >> 3);
        if (fonts > visible_fonts)
          slider_info.height=(visible_fonts*slider_info.height)/fonts;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.x=scroll_info.x+slider_info.bevel_width+1;
        slider_info.y=slider_info.min_y;
        expose_info=scroll_info;
        expose_info.y=slider_info.y;
        /*
          Initialize list information.
        */
        XGetWidgetInfo((char *) NULL,&list_info);
        list_info.raised=False;
        list_info.bevel_width--;
        list_info.width=
          scroll_info.x-reply_info.x-(font_info->max_bounds.width >> 1);
        list_info.height=scroll_info.height;
        list_info.x=reply_info.x;
        list_info.y=scroll_info.y;
        /*
          Initialize text information.
        */
        XGetWidgetInfo(text,&text_info);
        text_info.width=reply_info.width;
        text_info.height=height;
        text_info.x=list_info.x-(font_info->max_bounds.width >> 1);
        text_info.y=font_info->max_bounds.width;
        /*
          Initialize selection information.
        */
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=list_info.width;
        selection_info.height=(9*height) >> 3;
        selection_info.x=list_info.x;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw file browser window.
        */
        XClearWindow(display,windows->popup.id);
        x=font_info->max_bounds.width;
        y=text_info.y+((text_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,FontPatternText,strlen(FontPatternText));
        (void) sprintf(text_info.text,"%s",glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawBeveledButton(display,&windows->popup,&back_info);
        XDrawBeveledButton(display,&windows->popup,&reset_info);
        XDrawBeveledMatte(display,&windows->popup,&list_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        x=font_info->max_bounds.width;
        y=reply_info.y+((reply_info.height-height) >> 1)+font_info->ascent;
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,FontnameText,strlen(FontnameText));
        XDrawBeveledMatte(display,&windows->popup,&reply_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledButton(display,&windows->popup,&action_info);
        XDrawBeveledButton(display,&windows->popup,&cancel_info);
        XHighlightWidget(display,&windows->popup,4,4);
        selection_info.id=(~0);
        state|=RedrawActionState;
        state|=RedrawListState;
        state&=(~RedrawWidgetState);
      }
    if (state & UpdateListState)
      {
        char
          **checklist;

        int
          number_fonts;

        /*
          Update font list.
        */
        checklist=XListFonts(display,glob_pattern,32767,&number_fonts);
        if (checklist == (char **) NULL)
          {
            if ((strchr(glob_pattern,'*') == (char *) NULL) &&
                (strchr(glob_pattern,'?') == (char *) NULL))
              {
                /*
                  Might be a scaleable font-- exit.
                */
                (void) strcpy(reply,glob_pattern);
                (void) strcpy(glob_pattern,back_pattern);
                action_info.raised=False;
                XDrawBeveledButton(display,&windows->popup,&action_info);
                break;
              }
            (void) strcpy(glob_pattern,back_pattern);
            XBell(display,0);
          }
        else
          if (number_fonts == 1)
            {
              /*
                Reply is a single font name-- exit.
              */
              (void) strcpy(reply,checklist[0]);
              (void) strcpy(glob_pattern,back_pattern);
              XFreeFontNames(checklist);
              action_info.raised=False;
              XDrawBeveledButton(display,&windows->popup,&action_info);
              break;
            }
          else
            {
              XFreeFontNames(listhead);
              (void) free((char *) fontlist);
              fontlist=checklist;
              fonts=number_fonts;
            }
        /*
          Sort font list in ascending order.
        */
        listhead=fontlist;
        fontlist=(char **) malloc(fonts*sizeof(char **));
        if (fontlist == (char **) NULL)
          {
            XNoticeWidget(display,windows,"Unable to view fonts",
              "Memory allocation failed");
            return;
          }
        for (i=0; i < fonts; i++)
          fontlist[i]=listhead[i];
        (void) qsort((void *) fontlist,fonts,sizeof(char **),FontCompare);
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+1;
        if (fonts > visible_fonts)
          slider_info.height=(visible_fonts*slider_info.height)/fonts;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.id=0;
        slider_info.y=slider_info.min_y;
        expose_info.y=slider_info.y;
        selection_info.id=(~0);
        list_info.id=(~0);
        state|=RedrawListState;
        /*
          Redraw font name & reply.
        */
        *reply_info.text='\0';
        reply_info.cursor=reply_info.text;
        (void) sprintf(text_info.text,"%s",glob_pattern);
        XDrawWidgetText(display,&windows->popup,&text_info);
        XDrawMatteText(display,&windows->popup,&reply_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        XHighlightWidget(display,&windows->popup,4,4);
        state&=(~UpdateListState);
      }
    if (state & RedrawListState)
      {
        /*
          Determine slider id and position.
        */
        if (slider_info.id >= (int) (fonts-visible_fonts))
          slider_info.id=fonts-visible_fonts;
        if ((slider_info.id < 0) || (fonts <= visible_fonts))
          slider_info.id=0;
        slider_info.y=slider_info.min_y;
        if (fonts > 0)
          slider_info.y+=
            slider_info.id*(slider_info.max_y-slider_info.min_y+1)/fonts;
        if (slider_info.id != selection_info.id)
          {
            /*
              Redraw scroll bar and file names.
            */
            selection_info.id=slider_info.id;
            selection_info.y=list_info.y+(height >> 3)+2;
            for (i=0; i < visible_fonts; i++)
            {
              selection_info.raised=(slider_info.id+i) != list_info.id;
              selection_info.text=(char *) NULL;
              if ((slider_info.id+i) < fonts)
                selection_info.text=fontlist[slider_info.id+i];
              XDrawWidgetText(display,&windows->popup,&selection_info);
              selection_info.y+=(int) selection_info.height;
            }
            /*
              Update slider.
            */
            if (slider_info.y > expose_info.y)
              {
                expose_info.height=slider_info.y-expose_info.y;
                expose_info.y=slider_info.y-expose_info.height-
                  slider_info.bevel_width-1;
              }
            else
              {
                expose_info.height=expose_info.y-slider_info.y;
                expose_info.y=slider_info.y+slider_info.height+
                  slider_info.bevel_width+1;
              }
            XDrawTriangleNorth(display,&windows->popup,&north_info);
            XDrawMatte(display,&windows->popup,&expose_info);
            XDrawBeveledButton(display,&windows->popup,&slider_info);
            XDrawTriangleSouth(display,&windows->popup,&south_info);
            expose_info.y=slider_info.y;
          }
        state&=(~RedrawListState);
      }
    if (state & RedrawActionState)
      {
        XFontStruct
          *font_info,
          *save_info;

        /*
          Display the selected font in a drawing area.
        */
        save_info=windows->popup.font_info;
        font_info=XLoadQueryFont(display,reply_info.text);
        if (font_info != (XFontStruct *) NULL)
          {
            windows->popup.font_info=font_info;;
            XSetFont(display,windows->popup.widget_context,font_info->fid);
          }
        XDrawBeveledButton(display,&windows->popup,&mode_info);
        windows->popup.font_info=save_info;
        if (font_info != (XFontStruct *) NULL)
          {
            XSetFont(display,windows->popup.widget_context,
              windows->popup.font_info->fid);
            XFreeFont(display,font_info);
          }
        XHighlightWidget(display,&windows->popup,4,4);
        XDrawMatteText(display,&windows->popup,&reply_info);
        state&=(~RedrawActionState);
      }
    /*
      Wait for next event.
    */
    if (north_info.raised && south_info.raised)
      XIfEvent(display,&event,XScreenEvent,(char *) windows);
    else
      {
        /*
          Brief delay before advancing scroll bar.
        */
        XDelay(display,delay);
        XCheckMaskEvent(display,ButtonReleaseMask,&event);
        delay=SuspendTime;
      }
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(slider_info,event.xbutton))
          {
            /*
              Track slider.
            */
            slider_info.active=True;
            break;
          }
        if (MatteIsActive(north_info,event.xbutton))
          if (slider_info.id > 0)
            {
              /*
                Move slider up.
              */
              north_info.raised=False;
              slider_info.id--;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(south_info,event.xbutton))
          if (slider_info.id < fonts)
            {
              /*
                Move slider down.
              */
              south_info.raised=False;
              slider_info.id++;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(scroll_info,event.xbutton))
          {
            /*
              Move slider.
            */
            if (event.xbutton.y < slider_info.y)
              slider_info.id-=(visible_fonts-1);
            else
              slider_info.id+=(visible_fonts-1);
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(list_info,event.xbutton))
          {
            unsigned int
              id;

            /*
              User pressed list matte.
            */
            id=slider_info.id+(event.xbutton.y-(list_info.y+(height >> 1))+1)/
              selection_info.height;
            if (id >= fonts)
              break;
            (void) strcpy(reply_info.text,fontlist[id]);
            reply_info.highlight=False;
            reply_info.marker=reply_info.text;
            reply_info.cursor=reply_info.text+strlen(reply_info.text);
            XDrawMatteText(display,&windows->popup,&reply_info);
            state|=RedrawActionState;
            if (id == list_info.id)
              {
                (void) strcpy(glob_pattern,reply_info.text);
                state|=UpdateListState;
              }
            selection_info.id=(~0);
            list_info.id=id;
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(back_info,event.xbutton))
          {
            /*
              User pressed Back button.
            */
            back_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&back_info);
            break;
          }
        if (MatteIsActive(reset_info,event.xbutton))
          {
            /*
              User pressed Reset button.
            */
            reset_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
            break;
          }
        if (MatteIsActive(action_info,event.xbutton))
          {
            /*
              User pressed action button.
            */
            action_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (MatteIsActive(cancel_info,event.xbutton))
          {
            /*
              User pressed Cancel button.
            */
            cancel_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        if (!MatteIsActive(reply_info,event.xbutton))
          break;
        if (event.xbutton.button != Button2)
          {
            static Time
              click_time;

            /*
              Move text cursor to position of button press.
            */
            x=event.xbutton.x-reply_info.x-(font_info->max_bounds.width >> 2);
            for (i=1; i <= (int) strlen(reply_info.marker); i++)
              if (XTextWidth(font_info,reply_info.marker,i) > x)
                break;
            reply_info.cursor=reply_info.marker+i-1;
            if (event.xbutton.time < (click_time+DoubleClick))
              {
                /*
                  Become the XA_PRIMARY selection owner.
                */
                (void) strcpy(primary_selection,reply_info.text);
                XSetSelectionOwner(display,XA_PRIMARY,windows->popup.id,
                  event.xbutton.time);
                reply_info.highlight=XGetSelectionOwner(display,XA_PRIMARY) ==
                  windows->popup.id;
              }
            XDrawMatteText(display,&windows->popup,&reply_info);
            click_time=event.xbutton.time;
            break;
          }
        /*
          Request primary selection.
        */
        XConvertSelection(display,XA_PRIMARY,XA_STRING,XA_STRING,
          windows->popup.id,event.xbutton.time);
        break;
      }
      case ButtonRelease:
      {
        if (!north_info.raised)
          {
            /*
              User released up button.
            */
            delay=SuspendTime << 2;
            north_info.raised=True;
            XDrawTriangleNorth(display,&windows->popup,&north_info);
          }
        if (!south_info.raised)
          {
            /*
              User released down button.
            */
            delay=SuspendTime << 2;
            south_info.raised=True;
            XDrawTriangleSouth(display,&windows->popup,&south_info);
          }
        if (slider_info.active)
          {
            /*
              Stop tracking slider.
            */
            slider_info.active=False;
            break;
          }
        if (!back_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(back_info,event.xbutton))
                {
                  (void) strcpy(glob_pattern,back_pattern);
                  state|=UpdateListState;
                }
            back_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&back_info);
          }
        if (!reset_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(reset_info,event.xbutton))
                {
                  (void) strcpy(back_pattern,glob_pattern);
                  (void) strcpy(glob_pattern,reset_pattern);
                  state|=UpdateListState;
                }
            reset_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
          }
        if (!action_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(action_info,event.xbutton))
                if (*reply_info.text == '\0')
                  XBell(display,0);
                else
                  state|=ExitState;
            action_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&action_info);
          }
        if (!cancel_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(cancel_info,event.xbutton))
                {
                  *reply_info.text='\0';
                  state|=ExitState;
                }
            cancel_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
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
        if (event.xclient.window == windows->popup.id)
          {
            *reply_info.text='\0';
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
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

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (MatteIsActive(scroll_info,event.xkey))
          {
            /*
              Move slider.
            */
            switch (key_symbol)
            {
              case XK_Home:
              case XK_KP_Home:
              {
                slider_info.id=0;
                break;
              }
              case XK_Up:
              case XK_KP_Up:
              {
                slider_info.id--;
                break;
              }
              case XK_Down:
              case XK_KP_Down:
              {
                slider_info.id++;
                break;
              }
              case XK_Prior:
              case XK_KP_Prior:
              {
                slider_info.id-=visible_fonts;
                break;
              }
              case XK_Next:
              case XK_KP_Next:
              {
                slider_info.id+=visible_fonts;
                break;
              }
              case XK_End:
              case XK_KP_End:
              {
                slider_info.id=fonts;
                break;
              }
            }
            state|=RedrawListState;
            break;
          }
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            /*
              Read new font or glob patterm.
            */
            if (*reply_info.text == '\0')
              break;
            (void) strcpy(back_pattern,glob_pattern);
            (void) strcpy(glob_pattern,reply_info.text);
            state|=UpdateListState;
            break;
          }
        if (!MatteIsActive(reply_info,event.xkey))
          break;
        if (key_symbol == XK_Control_L)
          {
            state|=ControlState;
            break;
          }
        if (state & ControlState)
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
        XEditText(display,&reply_info,key_symbol,command,state);
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case KeyRelease:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key release.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if (key_symbol == XK_Control_L)
          state&=(~ControlState);
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (slider_info.active)
          {
            /*
              Move slider matte.
            */
            slider_info.y=event.xmotion.y-
              ((slider_info.height+slider_info.bevel_width) >> 1)+1;
            if (slider_info.y < slider_info.min_y)
              slider_info.y=slider_info.min_y;
            if (slider_info.y > slider_info.max_y)
              slider_info.y=slider_info.max_y;
            slider_info.id=(fonts*(slider_info.y-slider_info.min_y+1))/
              (slider_info.max_y-slider_info.min_y+1);
            state|=RedrawListState;
            break;
          }
        if (state & InactiveWidgetState)
          break;
        if (back_info.raised == MatteIsActive(back_info,event.xmotion))
          {
            /*
              Back button status changed.
            */
            back_info.raised=!back_info.raised;
            XDrawBeveledButton(display,&windows->popup,&back_info);
            break;
          }
        if (reset_info.raised == MatteIsActive(reset_info,event.xmotion))
          {
            /*
              Reset button status changed.
            */
            reset_info.raised=!reset_info.raised;
            XDrawBeveledButton(display,&windows->popup,&reset_info);
            break;
          }
        if (action_info.raised == MatteIsActive(action_info,event.xmotion))
          {
            /*
              Action button status changed.
            */
            action_info.raised=!action_info.raised;
            XDrawBeveledButton(display,&windows->popup,&action_info);
            break;
          }
        if (cancel_info.raised == MatteIsActive(cancel_info,event.xmotion))
          {
            /*
              Cancel button status changed.
            */
            cancel_info.raised=!cancel_info.raised;
            XDrawBeveledButton(display,&windows->popup,&cancel_info);
            break;
          }
        break;
      }
      case SelectionClear:
      {
        reply_info.highlight=False;
        XDrawMatteText(display,&windows->popup,&reply_info);
        break;
      }
      case SelectionNotify:
      {
        Atom
          type;

        int
          format,
          status;

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
        if ((strlen(reply_info.text)+length) >= MaxTextLength)
          XBell(display,0);
        else
          {
            /*
              Insert primary selection in reply text.
            */
            *(data+length)='\0';
            XEditText(display,&reply_info,(KeySym) XK_Insert,(char *) data,
              state);
            XDrawMatteText(display,&windows->popup,&reply_info);
            state|=RedrawActionState;
          }
        XFree((void *) data);
        break;
      }
      case SelectionRequest:
      {
        XSelectionEvent
          notify;

        XSelectionRequestEvent
          *request;

        /*
          Set XA_PRIMARY selection.
        */
        request=(&(event.xselectionrequest));
        XChangeProperty(request->display,request->requestor,request->property,
          request->target,8,PropModeReplace,(unsigned char *) primary_selection,
          strlen(primary_selection));
        notify.type=SelectionNotify;
        notify.display=request->display;
        notify.requestor=request->requestor;
        notify.selection=request->selection;
        notify.target=request->target;
        notify.time=request->time;
        if (request->property == None)
          notify.property=request->target;
        else
          notify.property=request->property;
        (void) XSendEvent(request->display,request->requestor,False,0,
          (XEvent *) &notify);
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
  /*
    Free font list.
  */
  XFreeFontNames(listhead);
  (void) free((char *) fontlist);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X M e n u W i d g e t                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XMenuWidget maps a menu and returns the command pointed to by the
%  user when the button is released.
%
%  The format of the XMenuWidget routine is:
%
%    selection_number=XMenuWidget(display,windows,title,selections,item)
%
%  A description of each parameter follows:
%
%    o selection_number: Specifies the number of the selection that the
%      user choose.
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o title: Specifies a character string that describes the menu selections.
%
%    o selections: Specifies a pointer to one or more strings that comprise
%      the choices in the menu.
%
%    o item: Specifies a character array.  The item selected from the menu
%      is returned here.
%
%
*/
int XMenuWidget(display,windows,title,selections,item)
Display
  *display;

XWindows
  *windows;

char
  *title,
  **selections;

char
  *item;
{
  Cursor
    cursor;

  int
    id,
    x,
    y;

  unsigned int
    height,
    limit,
    mask,
    number_selections,
    title_height,
    width;

  unsigned long
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XSetWindowAttributes
    window_attributes;

  XWidgetInfo
    menu_info,
    selection_info;

  XWindowChanges
    window_changes;

  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  windows->popup.width=0;
  if (title != (char *) NULL)
    windows->popup.width=XTextWidth(font_info,title,strlen(title));
  for (id=0; selections[id] != (char *) NULL; id++)
  {
    width=XTextWidth(font_info,selections[id],strlen(selections[id]));
    if (width > windows->popup.width)
      windows->popup.width=width;
  }
  number_selections=id;
  windows->popup.width+=font_info->max_bounds.width+6;
  XGetWidgetInfo((char *) NULL,&menu_info);
  title_height=menu_info.bevel_width << 1;
  width=0;
  if (title != (char *) NULL)
    {
      title_height=(font_info->descent+font_info->ascent) << 1;
      width=XTextWidth(font_info,title,strlen(title));
    }
  height=(9*(font_info->ascent+font_info->descent)) >> 3;
  windows->popup.height=
    title_height+number_selections*height+(menu_info.bevel_width << 1);
  windows->popup.min_width=windows->popup.width;
  windows->popup.min_height=windows->popup.height;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  windows->popup.x=x-(font_info->max_bounds.width >> 1);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  windows->popup.y=y-((3*title_height) >> 2);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  window_attributes.override_redirect=True;
  XChangeWindowAttributes(display,windows->popup.id,CWOverrideRedirect,
    &window_attributes);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  selection_info.height=height;
  cursor=XCreateFontCursor(display,XC_right_ptr);
  XDefineCursor(display,windows->image.id,cursor);
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        /*
          Initialize selection information.
        */
        XClearWindow(display,windows->popup.id);
        XGetWidgetInfo((char *) NULL,&menu_info);
        menu_info.bevel_width--;
        menu_info.width=windows->popup.width-((menu_info.bevel_width) << 1);
        menu_info.height=windows->popup.height-((menu_info.bevel_width) << 1);
        menu_info.x=menu_info.bevel_width;
        menu_info.y=menu_info.bevel_width;
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=menu_info.width;
        selection_info.height=height;
        selection_info.x=menu_info.x;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw Menu widget.
        */
        XDrawBevel(display,&windows->popup,&menu_info);
        if (title != (char *) NULL)
          {
            y=title_height-1;
            XSetBevelColor(display,&windows->popup,False);
            XDrawLine(display,windows->popup.id,windows->popup.widget_context,
              selection_info.x,y-1,(int) selection_info.width,y-1);
            XSetBevelColor(display,&windows->popup,True);
            XDrawLine(display,windows->popup.id,windows->popup.widget_context,
              selection_info.x,y,(int) selection_info.width,y);
            XSetFillStyle(display,windows->popup.widget_context,FillSolid);
          }
        /*
          Draw menu selections.
        */
        selection_info.raised=True;
        selection_info.y=title_height >> 2;
        selection_info.text=title;
        if (title != (char *) NULL)
          XDrawWidgetText(display,&windows->popup,&selection_info);
        selection_info.y=title_height;
        for (id=0; id < number_selections; id++)
        {
          selection_info.raised=(id != selection_info.id);
          selection_info.text=selections[id];
          XDrawWidgetText(display,&windows->popup,&selection_info);
          selection_info.y+=(int) selection_info.height;
        }
        state&=(~RedrawWidgetState);
      }
    if ((title != (char *) NULL) && (number_selections > 2))
      {
        /*
          Redraw Menu line.
        */
        y=title_height+selection_info.height*(number_selections-1);
        XSetBevelColor(display,&windows->popup,False);
        XDrawLine(display,windows->popup.id,windows->popup.widget_context,
          selection_info.x,y-1,(int) selection_info.width,y-1);
        XSetBevelColor(display,&windows->popup,True);
        XDrawLine(display,windows->popup.id,windows->popup.widget_context,
          selection_info.x,y,(int) selection_info.width,y);
        XSetFillStyle(display,windows->popup.widget_context,FillSolid);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
        break;
      case ButtonRelease:
      {
        /*
          Exit menu.
        */
        *item='\0';
        state|=ExitState;
        XPutBackEvent(display,&event);
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window == windows->command.id)
          {
            selection_info.id=(-1);
            state|=ExitState;
            XPutBackEvent(display,&event);
            break;
          }
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        id=((event.xcrossing.y-title_height)/(int) selection_info.height);
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Highlight this selection.
        */
        selection_info.id=id;
        selection_info.y=title_height+selection_info.id*selection_info.height;
        selection_info.raised=False;
        selection_info.text=selections[selection_info.id];
        XDrawWidgetText(display,&windows->popup,&selection_info);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        id=selection_info.id;
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Unhighlight last selection.
        */
        selection_info.y=title_height+id*selection_info.height;
        selection_info.id=(~0);
        selection_info.raised=True;
        selection_info.text=selections[id];
        XDrawWidgetText(display,&windows->popup,&selection_info);
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (state & InactiveWidgetState)
          break;
        id=(event.xmotion.y-title_height)/(int) selection_info.height;
        if ((selection_info.id >= 0) && (selection_info.id < number_selections))
          {
            /*
              Unhighlight last selection.
            */
            if (id == selection_info.id)
              break;
            selection_info.y=
              title_height+selection_info.id*selection_info.height;
            selection_info.raised=True;
            selection_info.text=selections[selection_info.id];
            XDrawWidgetText(display,&windows->popup,&selection_info);
          }
        selection_info.id=id;
        if ((id < 0) || (id >= number_selections))
          break;
        /*
          Highlight this selection.
        */
        selection_info.y=title_height+id*selection_info.height;
        selection_info.raised=False;
        selection_info.text=selections[id];
        XDrawWidgetText(display,&windows->popup,&selection_info);
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  window_attributes.override_redirect=False;
  XChangeWindowAttributes(display,windows->popup.id,CWOverrideRedirect,
    &window_attributes);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
  if ((selection_info.id < 0) || (selection_info.id >= number_selections))
    return(~0);
  (void) strcpy(item,selections[selection_info.id]);
  return(selection_info.id);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X N o t i c e W i d g e t                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XNoticeWidget displays a popup window with a notice to the user.
%  The function returns when the user presses the "Dismiss" button.
%
%  The format of the XNoticeWidget routine is:
%
%    XNoticeWidget(display,windows,message,qualifier)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o message: Specifies the message to display before terminating the
%      program.
%
%    o qualifier: Specifies any qualifier to the message.
%
%
*/
void XNoticeWidget(display,windows,message,qualifier)
Display
  *display;

XWindows
  *windows;

char
  *message,
  *qualifier;
{
#define DismissButtonText  "Dismiss"

  int
    limit,
    x,
    y;

  unsigned int
    height,
    mask,
    width;

  unsigned long
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    dismiss_info;

  XWindowChanges
    window_changes;

  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  width=XTextWidth(font_info,DismissButtonText,strlen(DismissButtonText));
  if (message != (char *) NULL)
    if (XTextWidth(font_info,message,strlen(message)) > width)
      width=XTextWidth(font_info,message,strlen(message));
  if (qualifier != (char *) NULL)
    if (XTextWidth(font_info,qualifier,strlen(qualifier)) > width)
      width=XTextWidth(font_info,qualifier,strlen(qualifier));
  height=(font_info->ascent+font_info->descent);
  windows->popup.width=width+4*font_info->max_bounds.width;
  windows->popup.height=12*height;
  windows->popup.min_width=width+font_info->max_bounds.width;
  windows->popup.min_height=7*height;
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=(windows->popup.width >> 1);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=((5*windows->popup.height) >> 3);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,"Notice");
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  XBell(display,0);
  /*
    Respond to X events.
  */
  state=RedrawWidgetState | UpdateConfigurationState;
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  do
  {
    if (state & UpdateConfigurationState)
      {
        /*
          Initialize Dismiss button information.
        */
        XGetWidgetInfo(DismissButtonText,&dismiss_info);
        dismiss_info.width=font_info->max_bounds.width+
          XTextWidth(font_info,DismissButtonText,strlen(DismissButtonText));
        dismiss_info.height=(3*height) >> 1;
        dismiss_info.x=(windows->popup.width >> 1)-(dismiss_info.width >> 1);
        dismiss_info.y=windows->popup.height-(dismiss_info.height << 1);
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw Notice widget.
        */
        XClearWindow(display,windows->popup.id);
        width=XTextWidth(font_info,message,strlen(message));
        x=(windows->popup.width >> 1)-(width >> 1);
        y=(windows->popup.height >> 1)-(height << 1);
        XDrawString(display,windows->popup.id,windows->popup.annotate_context,
          x,y,message,strlen(message));
        if (qualifier != (char *) NULL)
          {
            width=XTextWidth(font_info,qualifier,strlen(qualifier));
            x=(windows->popup.width >> 1)-(width >> 1);
            y+=height;
            XDrawString(display,windows->popup.id,
              windows->popup.annotate_context,x,y,qualifier,strlen(qualifier));
          }
        XDrawBeveledButton(display,&windows->popup,&dismiss_info);
        XHighlightWidget(display,&windows->popup,4,4);
        state&=(~RedrawWidgetState);
      }
    /*
      Wait for next event.
    */
    XIfEvent(display,&event,XScreenEvent,(char *) windows);
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(dismiss_info,event.xbutton))
          {
            /*
              User pressed Dismiss button.
            */
            dismiss_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            break;
          }
        break;
      }
      case ButtonRelease:
      {
        if (!dismiss_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(dismiss_info,event.xbutton))
                state|=ExitState;
            dismiss_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
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
        if (event.xclient.window == windows->popup.id)
          {
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
        break;
      }
      case KeyPress:
      {
        static char
          command[MaxTextLength];

        static KeySym
          key_symbol;

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        (void) XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            dismiss_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            state|=ExitState;
            break;
          }
        break;
      }
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (state & InactiveWidgetState)
          break;
        if (dismiss_info.raised == MatteIsActive(dismiss_info,event.xmotion))
          {
            /*
              Dismiss button status changed.
            */
            dismiss_info.raised=!dismiss_info.raised;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            break;
          }
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   X T e x t V i e w W i d g e t                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Function XTextViewWidget displays text in a popup window.
%
%  The format of the XTextViewWidget routine is:
%
%    XTextViewWidget(display,windows,title,textlist)
%
%  A description of each parameter follows:
%
%    o display: Specifies a connection to an X server;  returned from
%      XOpenDisplay.
%
%    o window: Specifies a pointer to a XWindows structure.
%
%    o title: This character string is displayed at the top of the popup
%      window.
%
%    o textlist: This string list is displayed within the popup window.
%
%
*/
void XTextViewWidget(display,windows,title,textlist)
Display
  *display;

XWindows
  *windows;

char
  *title,
  **textlist;
{
#define DismissButtonText  "Dismiss"

  int
    x,
    y;

  register int
    i;

  unsigned int
    height,
    limit,
    lines,
    mask,
    text_width,
    visible_lines,
    width;

  unsigned long
    delay,
    state;

  Window
    root_window;

  XEvent
    event;

  XFontStruct
    *font_info;

  XTextProperty
    window_name;

  XWidgetInfo
    dismiss_info,
    expose_info,
    list_info,
    north_info,
    scroll_info,
    selection_info,
    slider_info,
    south_info;

  XWindowChanges
    window_changes;

  /*
    Convert text string to a text list.
  */
  XDefineCursor(display,windows->image.id,windows->image.busy_cursor);
  XFlush(display);
  if (textlist == (char **) NULL)
    {
      XNoticeWidget(display,windows,"No text to view",(char *) NULL);
      return;
    }
  /*
    Determine popup window attributes.
  */
  font_info=windows->popup.font_info;
  text_width=0;
  for (i=0; textlist[i] != (char *) NULL; i++)
    if (XTextWidth(font_info,textlist[i],strlen(textlist[i])) > text_width)
      text_width=XTextWidth(font_info,textlist[i],strlen(textlist[i]));
  lines=i;
  width=XTextWidth(font_info,DismissButtonText,strlen(DismissButtonText));
  width+=font_info->max_bounds.width;
  height=font_info->ascent+font_info->descent;
  windows->popup.width=text_width+5*font_info->max_bounds.width;
  windows->popup.height=Min(Max(lines,3),24)*height+((13*height) >> 1)+
    ((9*font_info->max_bounds.width) >> 1);
  windows->popup.min_width=25*XTextWidth(font_info,"#",1)+
    4*font_info->max_bounds.width;
  windows->popup.min_height=3*height+((13*height) >> 1)+
    ((9*font_info->max_bounds.width) >> 1);
  /*
    Position popup window.
  */
  XQueryPointer(display,XRootWindow(display,windows->popup.screen),&root_window,
    &root_window,&x,&y,&windows->popup.x,&windows->popup.y,&mask);
  if (windows->popup.width < windows->popup.min_width)
    windows->popup.width=windows->popup.min_width;
  windows->popup.x-=((3*windows->popup.width) >> 2);
  limit=XDisplayWidth(display,windows->popup.screen)-windows->popup.width;
  if (windows->popup.x < 0)
    windows->popup.x=0;
  else
    if (windows->popup.x > limit)
      windows->popup.x=limit;
  if (windows->popup.height < windows->popup.min_height)
    windows->popup.height=windows->popup.min_height;
  windows->popup.y-=(windows->popup.height >> 1);
  limit=XDisplayHeight(display,windows->popup.screen)-windows->popup.height;
  if (windows->popup.y < 0)
    windows->popup.y=0;
  else
    if (windows->popup.y > limit)
      windows->popup.y=limit;
  /*
    Map popup window.
  */
  (void) sprintf(windows->popup.name,title);
  (void) XStringListToTextProperty(&windows->popup.name,1,&window_name);
  XSetWMName(display,windows->popup.id,&window_name);
  window_changes.width=windows->popup.width;
  window_changes.height=windows->popup.height;
  window_changes.x=windows->popup.x;
  window_changes.y=windows->popup.y;
  XReconfigureWMWindow(display,windows->popup.id,windows->popup.screen,CWWidth |
    CWHeight | CWX | CWY,&window_changes);
  XMapRaised(display,windows->popup.id);
  /*
    Respond to X events.
  */
  XGetWidgetInfo((char *) NULL,&slider_info);
  XGetWidgetInfo((char *) NULL,&north_info);
  XGetWidgetInfo((char *) NULL,&south_info);
  visible_lines=0;
  delay=SuspendTime << 2;
  state=RedrawWidgetState | UpdateConfigurationState;
  do
  {
    if (state & UpdateConfigurationState)
      {
        int
          id;

        /*
          Initialize button information.
        */
        XGetWidgetInfo(DismissButtonText,&dismiss_info);
        dismiss_info.width=width;
        dismiss_info.height=(3*height) >> 1;
        dismiss_info.x=windows->popup.width-dismiss_info.width-
          font_info->max_bounds.width-2;
        dismiss_info.y=windows->popup.height-dismiss_info.height-
          font_info->max_bounds.width;
        /*
          Initialize scroll information.
        */
        XGetWidgetInfo((char *) NULL,&scroll_info);
        scroll_info.bevel_width--;
        scroll_info.width=height;
        scroll_info.height=dismiss_info.y-
          ((5*font_info->max_bounds.width) >> 1);
        scroll_info.x=windows->popup.width-font_info->max_bounds.width-
          scroll_info.width;
        scroll_info.y=(3*font_info->max_bounds.width) >> 1;
        scroll_info.raised=False;
        scroll_info.trough=True;
        north_info=scroll_info;
        north_info.raised=True;
        north_info.width-=(north_info.bevel_width << 1);
        north_info.height=north_info.width-1;
        north_info.x+=north_info.bevel_width;
        north_info.y+=north_info.bevel_width;
        south_info=north_info;
        south_info.y=scroll_info.y+scroll_info.height-scroll_info.bevel_width-
          south_info.height;
        id=slider_info.id;
        slider_info=north_info;
        slider_info.id=id;
        slider_info.width-=2;
        slider_info.min_y=north_info.y+north_info.height+north_info.bevel_width+
          slider_info.bevel_width+2;
        slider_info.height=
          scroll_info.height-((slider_info.min_y-scroll_info.y+1) << 1)+2;
        visible_lines=(scroll_info.height-(height >> 3)-4)/((9*height) >> 3);
        if (lines > visible_lines)
          slider_info.height=(visible_lines*slider_info.height)/lines;
        slider_info.max_y=south_info.y-south_info.bevel_width-
          slider_info.bevel_width-2;
        slider_info.x=scroll_info.x+slider_info.bevel_width+1;
        slider_info.y=slider_info.min_y;
        expose_info=scroll_info;
        expose_info.y=slider_info.y;
        /*
          Initialize list information.
        */
        XGetWidgetInfo((char *) NULL,&list_info);
        list_info.raised=False;
        list_info.bevel_width--;
        list_info.width=scroll_info.x-((3*font_info->max_bounds.width) >> 1);
        list_info.height=scroll_info.height;
        list_info.x=font_info->max_bounds.width;
        list_info.y=scroll_info.y;
        /*
          Initialize selection information.
        */
        XGetWidgetInfo((char *) NULL,&selection_info);
        selection_info.width=list_info.width;
        selection_info.height=(9*height) >> 3;
        selection_info.x=list_info.x;
        state&=(~UpdateConfigurationState);
      }
    if (state & RedrawWidgetState)
      {
        /*
          Redraw file browser window.
        */
        XClearWindow(display,windows->popup.id);
        XDrawBeveledMatte(display,&windows->popup,&list_info);
        XDrawBeveledMatte(display,&windows->popup,&scroll_info);
        XDrawTriangleNorth(display,&windows->popup,&north_info);
        XDrawBeveledButton(display,&windows->popup,&slider_info);
        XDrawTriangleSouth(display,&windows->popup,&south_info);
        XDrawBeveledButton(display,&windows->popup,&dismiss_info);
        XHighlightWidget(display,&windows->popup,4,4);
        selection_info.id=(~0);
        state|=RedrawListState;
        state&=(~RedrawWidgetState);
      }
    if (state & RedrawListState)
      {
        /*
          Determine slider id and position.
        */
        if (slider_info.id >= (int) (lines-visible_lines))
          slider_info.id=lines-visible_lines;
        if ((slider_info.id < 0) || (lines <= visible_lines))
          slider_info.id=0;
        slider_info.y=slider_info.min_y;
        if (lines != 0)
          slider_info.y+=
            slider_info.id*(slider_info.max_y-slider_info.min_y+1)/lines;
        if (slider_info.id != selection_info.id)
          {
            /*
              Redraw scroll bar and file names.
            */
            selection_info.id=slider_info.id;
            selection_info.y=list_info.y+(height >> 3)+2;
            for (i=0; i < visible_lines; i++)
            {
              selection_info.raised=(slider_info.id+i) != list_info.id;
              selection_info.text=(char *) NULL;
              if ((slider_info.id+i) < lines)
                selection_info.text=textlist[slider_info.id+i];
              XDrawWidgetText(display,&windows->popup,&selection_info);
              selection_info.y+=(int) selection_info.height;
            }
            /*
              Update slider.
            */
            if (slider_info.y > expose_info.y)
              {
                expose_info.height=slider_info.y-expose_info.y;
                expose_info.y=slider_info.y-expose_info.height-
                  slider_info.bevel_width-1;
              }
            else
              {
                expose_info.height=expose_info.y-slider_info.y;
                expose_info.y=slider_info.y+slider_info.height+
                  slider_info.bevel_width+1;
              }
            XDrawTriangleNorth(display,&windows->popup,&north_info);
            XDrawMatte(display,&windows->popup,&expose_info);
            XDrawBeveledButton(display,&windows->popup,&slider_info);
            XDrawTriangleSouth(display,&windows->popup,&south_info);
            expose_info.y=slider_info.y;
          }
        state&=(~RedrawListState);
      }
    /*
      Wait for next event.
    */
    if (north_info.raised && south_info.raised)
      XIfEvent(display,&event,XScreenEvent,(char *) windows);
    else
      {
        /*
          Brief delay before advancing scroll bar.
        */
        XDelay(display,delay);
        XCheckMaskEvent(display,ButtonReleaseMask,&event);
        delay=SuspendTime;
      }
    switch (event.type)
    {
      case ButtonPress:
      {
        if (MatteIsActive(slider_info,event.xbutton))
          {
            /*
              Track slider.
            */
            slider_info.active=True;
            break;
          }
        if (MatteIsActive(north_info,event.xbutton))
          if (slider_info.id > 0)
            {
              /*
                Move slider up.
              */
              north_info.raised=False;
              slider_info.id--;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(south_info,event.xbutton))
          if (slider_info.id < lines)
            {
              /*
                Move slider down.
              */
              south_info.raised=False;
              slider_info.id++;
              state|=RedrawListState;
              break;
            }
        if (MatteIsActive(scroll_info,event.xbutton))
          {
            /*
              Move slider.
            */
            if (event.xbutton.y < slider_info.y)
              slider_info.id-=(visible_lines-1);
            else
              slider_info.id+=(visible_lines-1);
            state|=RedrawListState;
            break;
          }
        if (MatteIsActive(dismiss_info,event.xbutton))
          {
            /*
              User pressed Dismiss button.
            */
            dismiss_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            break;
          }
        break;
      }
      case ButtonRelease:
      {
        if (!north_info.raised)
          {
            /*
              User released up button.
            */
            delay=SuspendTime << 2;
            north_info.raised=True;
            XDrawTriangleNorth(display,&windows->popup,&north_info);
          }
        if (!south_info.raised)
          {
            /*
              User released down button.
            */
            delay=SuspendTime << 2;
            south_info.raised=True;
            XDrawTriangleSouth(display,&windows->popup,&south_info);
          }
        if (slider_info.active)
          {
            /*
              Stop tracking slider.
            */
            slider_info.active=False;
            break;
          }
        if (!dismiss_info.raised)
          {
            if (event.xbutton.window == windows->popup.id)
              if (MatteIsActive(dismiss_info,event.xbutton))
                state|=ExitState;
            dismiss_info.raised=True;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
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
        if (event.xclient.window == windows->popup.id)
          {
            state|=ExitState;
            break;
          }
        break;
      }
      case ConfigureNotify:
      {
        /*
          Update widget configuration.
        */
        if (event.xconfigure.window != windows->popup.id)
          break;
        if ((event.xconfigure.width == windows->popup.width) &&
            (event.xconfigure.height == windows->popup.height))
          break;
        windows->popup.width=
          Max(event.xconfigure.width,windows->popup.min_width);
        windows->popup.height=
          Max(event.xconfigure.height,windows->popup.min_height);
        state|=UpdateConfigurationState;
        break;
      }
      case EnterNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state&=(~InactiveWidgetState);
        break;
      }
      case Expose:
      {
        if (event.xexpose.window != windows->popup.id)
          break;
        if (event.xexpose.count != 0)
          break;
        state|=RedrawWidgetState;
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

        if (event.xkey.window != windows->popup.id)
          break;
        /*
          Respond to a user key press.
        */
        length=XLookupString((XKeyEvent *) &event.xkey,command,sizeof(command),
          &key_symbol,(XComposeStatus *) NULL);
        *(command+length)='\0';
        if (MatteIsActive(scroll_info,event.xkey))
          {
            /*
              Move slider.
            */
            switch (key_symbol)
            {
              case XK_Home:
              case XK_KP_Home:
              {
                slider_info.id=0;
                break;
              }
              case XK_Up:
              case XK_KP_Up:
              {
                slider_info.id--;
                break;
              }
              case XK_Down:
              case XK_KP_Down:
              {
                slider_info.id++;
                break;
              }
              case XK_Prior:
              case XK_KP_Prior:
              {
                slider_info.id-=visible_lines;
                break;
              }
              case XK_Next:
              case XK_KP_Next:
              {
                slider_info.id+=visible_lines;
                break;
              }
              case XK_End:
              case XK_KP_End:
              {
                slider_info.id=lines;
                break;
              }
            }
            state|=RedrawListState;
            break;
          }
        if ((key_symbol == XK_Return) || (key_symbol == XK_KP_Enter))
          {
            dismiss_info.raised=False;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            state|=ExitState;
            break;
          }
        break;
      }
      case KeyRelease:
        break;
      case LeaveNotify:
      {
        if (event.xcrossing.window != windows->popup.id)
          break;
        state|=InactiveWidgetState;
        break;
      }
      case MotionNotify:
      {
        /*
          Discard pending button motion events.
        */
        while (XCheckMaskEvent(display,ButtonMotionMask,&event));
        if (slider_info.active)
          {
            /*
              Move slider matte.
            */
            slider_info.y=event.xmotion.y-
              ((slider_info.height+slider_info.bevel_width) >> 1)+1;
            if (slider_info.y < slider_info.min_y)
              slider_info.y=slider_info.min_y;
            if (slider_info.y > slider_info.max_y)
              slider_info.y=slider_info.max_y;
            slider_info.id=(lines*(slider_info.y-slider_info.min_y+1))/
              (slider_info.max_y-slider_info.min_y+1);
            state|=RedrawListState;
            break;
          }
        if (state & InactiveWidgetState)
          break;
        if (dismiss_info.raised == MatteIsActive(dismiss_info,event.xmotion))
          {
            /*
              Dismiss button status changed.
            */
            dismiss_info.raised=!dismiss_info.raised;
            XDrawBeveledButton(display,&windows->popup,&dismiss_info);
            break;
          }
        break;
      }
      default:
        break;
    }
  } while (!(state & ExitState));
  XDefineCursor(display,windows->image.id,windows->image.cursor);
  XWithdrawWindow(display,windows->popup.id,windows->popup.screen);
}
