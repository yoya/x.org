/* -*-C-*-
********************************************************************************
*
* File:         tic_Line.c
* RCS:          $Header: /users/npm/src/winterp/src-server/RCS/tic_Line.c,v 2.1 1994/06/06 15:41:07 npm Exp $
* Description:  TANGO:LINE_IMAGE_CLASS (subclass of TANGO:IMAGE_CLASS).
* Author:       Niels P. Mayer
* Created:      Sat May 15 21:39:46 1993
* Modified:     Sun Jun  5 14:27:38 1994 (Niels Mayer) npm@indeed
* Language:     C
* Package:      N/A
* Status:       X11r6 contrib release
*
* Copyright (C) 1994, Enterprise Integration Technologies Corp. and Niels Mayer.
* WINTERP 1.15-1.99, Copyright (c) 1993, Niels P. Mayer.
* WINTERP 1.0-1.14, Copyright (c) 1989-1992 Hewlett-Packard Co. and Niels Mayer.
* 
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Enterprise Integration Technologies,
* Hewlett-Packard Company, or Niels Mayer not be used in advertising or
* publicity pertaining to distribution of the software without specific,
* written prior permission. Enterprise Integration Technologies, Hewlett-Packard
* Company, and Niels Mayer makes no representations about the suitability of
* this software for any purpose.  It is provided "as is" without express or
* implied warranty.
* 
* ENTERPRISE INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD COMPANY AND NIELS MAYER
* DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL ENTERPRISE
* INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD COMPANY OR NIELS MAYER BE LIABLE
* FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
* CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
********************************************************************************
*/
static char rcs_identity[] = "@(#)$Header: /users/npm/src/winterp/src-server/RCS/tic_Line.c,v 2.1 1994/06/06 15:41:07 npm Exp $";

/*
*------------------------------------------------------------------------------
* See ./winterp/COPYRIGHT for information on contacting the authors.
* Please e-mail comments, modifications, questions, improvements and
* bugfixes to the WINTERP mailing list winterp@netcom.com. Please send 
* mailing list subscribe/unsubscribe notices to winterp-request@netcom.com .
* Post XLISP-specific questions/information to the USENET newsgroup
* comp.lang.lisp.x.
*------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <Xm/Xm.h>
#include "winterp.h"
#include "w_funtab.h"
#include "tango.h"


LVAL k_NO_ARROW, k_FORW_ARROW, k_BACK_ARROW, k_BOTH_ARROW; /* Init'd by Tic_Line_Init() */

/*******************************************************************************
 * Special routine to get args from image description list. This 
 * is used both by Tango_Ellispe_Image_Class_Method_ISNEW() and
 * Xtango_Create_Composite_Image()
 *******************************************************************************/
Boolean Xtango_Get_Line_Image_Args(err_string, err_lval, loc_x_float, loc_y_float, lval_color,
				   size_x_float, size_y_float, line_width_float, line_style, arrow_type)
     char*  *err_string;	/* If routine returns FALSE, returns a string w/ error message */
     LVAL   *err_lval;		/* If routine returns FALSE, returns an LVAL w/ eroneous value */
     double *loc_x_float;	/* If routine returns TRUE, returns x_location */
     double *loc_y_float;	/* If routine returns TRUE, returns y_location */
     LVAL   *lval_color;	/* If routine returns TRUE, returns color LVAL (STRING/FIXNUM) */
     double *size_x_float;      /* If routine returns TRUE, returns x-size */
     double *size_y_float;      /* If routine returns TRUE, returns y-size */
     double *line_width_float;	/* If routine returns TRUE, returns line_width */
     double *line_style;	/* If routine returns TRUE, returns line_style */
     int    *arrow_type;	/* If routine returns TRUE, returns arrow_type */
{
  extern LVAL s_unbound;
  LVAL lval_arg;

  /*
   * get <location_coord> from argument stack
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (complexp(lval_arg) && floatp(getelement(lval_arg, 0)) && floatp(getelement(lval_arg, 1))) {
      *loc_x_float = (double) getflonum(getelement(lval_arg, 0));
      *loc_y_float = (double) getflonum(getelement(lval_arg, 1));
    }
    else {
      *err_string = string_err_msg_bad_loc_coord;
      if (!complexp(lval_arg))
	*err_lval = lval_arg;
      else if (!floatp(getelement(lval_arg, 0)))
	*err_lval = getelement(lval_arg, 0);
      else if (!floatp(getelement(lval_arg, 1)))
	*err_lval = getelement(lval_arg, 1);
      else
	*err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_loc_coord;
    *err_lval = s_unbound;
    return (FALSE);
  }

  /*
   * get <size_coord> from argument stack (COMPLEX)
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (complexp(lval_arg) && floatp(getelement(lval_arg, 0)) && floatp(getelement(lval_arg, 1))) {
      *size_x_float = (double) getflonum(getelement(lval_arg, 0));
      *size_y_float = (double) getflonum(getelement(lval_arg, 1));
    }
    else {
      *err_string = string_err_msg_bad_size_coord;
      if (!complexp(lval_arg))
	*err_lval = lval_arg;
      else if (!floatp(getelement(lval_arg, 0)))
	*err_lval = getelement(lval_arg, 0);
      else if (!floatp(getelement(lval_arg, 1)))
	*err_lval = getelement(lval_arg, 1);
      else
	*err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_size_coord;
    *err_lval = s_unbound;
    return (FALSE);
  }

  /*
   * get <tango_color> from argument stack
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (stringp(lval_arg) || fixp(lval_arg))
      *lval_color = lval_arg;
    else {
      *err_string = string_err_msg_bad_color;
      *err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_color;
    *err_lval = s_unbound;
    return (FALSE);
  }
  
  /*
   * get <line_width_float> from argument stack
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (floatp(lval_arg))
      *line_width_float = (double) getflonum(lval_arg);
    else {
      *err_string = string_err_msg_bad_line_width;
      *err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_line_width;
    *err_lval = s_unbound;
    return (FALSE);
  }

  /*
   * get <line_style> from argument stack
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (floatp(lval_arg))
      *line_style = (double) getflonum(lval_arg);
    else {
      *err_string = string_err_msg_bad_line_style;
      *err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_line_style;
    *err_lval = s_unbound;
    return (FALSE);
  }

  /*
   * get <arrow-kwd> from argument stack
   */
  if (moreargs()) {
    lval_arg = nextarg();
    if (lval_arg == k_NO_ARROW)
      *arrow_type = 0;
    else if (lval_arg == k_FORW_ARROW)
      *arrow_type = 1;
    else if (lval_arg == k_BACK_ARROW)
      *arrow_type = 2;
    else if (lval_arg == k_BOTH_ARROW)
      *arrow_type = 3;
    else {
      *err_string = string_err_msg_bad_line_arrow;
      *err_lval = lval_arg;
      return (FALSE);
    }
  }
  else {
    *err_string = string_err_msg_bad_line_arrow;
    *err_lval = s_unbound;
    return (FALSE);
  }

  return (TRUE);
}


/*****************************************************************************
 * (send TANGO:LINE_IMAGE_CLASS :new
 *       [:show] [<visible_kwd>] <tango_widget>
 *       <location_coord>
 *       <size_coord>
 *       <tango_color>
 *       <line_width_float>
 *       <line_style>
 *       <arrow_kwd>)
 * 	==> RETURNS an <tango_image> object.
 * 
 * [:show] -- OPTIONAL :show keyword. If present, displays image
 * immediately, else the image will be displayed along with the next
 * animation frame. See :TAP_SHOW :TX_DELAY.
 * 
 * [<visible_kwd>] -- OPTIONAL :VISIBLE or :INVISIBLE keyword. If
 * omitted, :VISIBLE is assumed. See also :TX_VISIBLE, :TAP_VIS_TOGGLE.
 * 
 * <tango_widget> -- an instance of TANGO:WIDGET_CLASS.
 * 
 * <location_coord> -- the location for placing the image. A
 * COMPLEX number, of form #C(<loc_x> <loc_y>), where <loc_x> is a
 * FLONUM, typically [0.0 - 1.0] representing the X-axis location;
 * <loc_y> is a FLONUM, typically [0.0 - 1.0] representing the Y-axis
 * location.
 * 
 * <size_coord> -- a COMPLEX number of form #C(<size-x> #<size-y>)
 * where <size-x> represents the x-offset and <size-y> represents
 * the y-offset of line/arrow destination; the source being at
 * <location_coor>. Line sizes can be either positive or negative.
 * 
 * <tango_color> -- The color of the image; it can either be a string
 * color name, or a FIXNUM representing the tango pixel value. If given
 * a string, it is looked up in the X11 file /usr/lib/X11/rgb.txt.
 * 
 * <line_width_float> -- defines the width of the line; it can range
 * from 0.0 to 1.0 (corresponding roughly to percentages).  0.0
 * corresponds to a thin line, and 1.0 corresponds to the thickest line.
 * Currently, we have implemented three thicknesses that are achieved by
 * values in the range 0.0 to 0.333, 0.333 to 0.667, and 0.667 to 1.0,
 * respectively.
 * 
 * <line_style> -- defines the line's style.  It ranges from 0.0 to 1.0
 * also.  We have implemented three styles: 0.0 to 0.333 defines a dotted
 * line, 0.333 to 0.667 defines a dashed line, and 0.667 to 1.0 defines a
 * solid line.
 * 
 * <arrow_kwd> -- one of the following keywords :NO_ARROW, :FORW_ARROW,
 * :BACK_ARROW, or :BOTH_ARROW .
 * ==========================================================================
 * TANGO_IMAGE
 * TANGOimage_create(TANGO_IMAGE_TYPE_LINE,lx,ly,vis,color,sx,sy,wid,sty,arrow)
 *	double lx,ly;
 *	int vis
 *	TANGO_COLOR color;
 *	double sx,sy,wid,sty;
 *	int arrow;
 ****************************************************************************/
LVAL Tango_Line_Image_Class_Method_ISNEW()
{
  TANGO_IMAGE image;
  WINTERP_TANGO_CONTEXT context;
  LVAL o_widget;
  char*  err_string;
  LVAL   err_lval;
  double loc_x_float;
  double loc_y_float;
  LVAL   lval_color;
  double size_x_float;
  double size_y_float;
  double line_width_float;
  double line_style;
  int	 arrow_type;

  /* get <self> from argument stack */
  LVAL self
    = Tcls_Get_OBJECT_Arg_Returning_TANGOIMAGEOBJ();

  /* get OPTIONAL [:show] from argument stack */
  int show_p
    = (moreargs() && (*xlargv == k_SHOW))
      ? (nextarg(), TRUE)
	: FALSE;

  /* get OPTIONAL [<visible_kwd>] from argument stack */
  int visible_p
    = (moreargs() && ((*xlargv == k_VISIBLE) || (*xlargv == k_INVISIBLE)))
      ? ((nextarg() == k_VISIBLE) ? TRUE : FALSE)
	: TRUE;

  /* get <tango_widget> from argument stack */
  Widget widget_id
    = Xtango_Get_TANGO_WIDGETOBJ_Returning_Validated_WidgetID(&o_widget);

  /* get image-specific arguments */
  if (!Xtango_Get_Line_Image_Args(&err_string, &err_lval,
				  &loc_x_float, &loc_y_float,
				  &lval_color,
				  &size_x_float, &size_y_float,
				  &line_width_float, &line_style,
				  &arrow_type))
    xlerror(err_string, err_lval);

  xllastarg();

  context = Xtango_Find_Context_From_WidgetID(widget_id);
  Xtango_Save_Set_Context(context);
  Xtango_Check_Initialized();

  image = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
			    loc_x_float,
			    loc_y_float,
			    visible_p,
			    Xtango_Cvt_LVAL_To_Valid_TANGO_COLOR_Else_Error(lval_color),
			    size_x_float,
			    size_y_float,
			    line_width_float,
			    line_style,
			    arrow_type);

  Tcls_Initialize_TANGOIMAGEOBJ(self, image, context);

  if (show_p)
    TAPshow(image);

  Xtango_Restore_Context();

  return (self);
}


/*****************************************************************************
 *
 ****************************************************************************/
LVAL Tango_Line_Image_Get_Values(image)
     TANGO_IMAGE image;
{
  LVAL lval_location, lval_size, lval_color, lval_width, lval_style, lval_arrow, lval_result;
  TANGO_LINE_PTR line = (TANGO_LINE_PTR) image->object;

  /* protect some pointers */
  xlstkcheck(7);
  xlsave(lval_location);
  xlsave(lval_size);
  xlsave(lval_color);
  xlsave(lval_width);
  xlsave(lval_style);
  xlsave(lval_arrow);
  xlsave(lval_result);

  switch (line->arrow) {
  case 0:
    lval_arrow = k_NO_ARROW;
    break;
  case 1:
    lval_arrow = k_FORW_ARROW;
    break;
  case 2:
    lval_arrow = k_BACK_ARROW;
    break;
  case 3:
    lval_arrow = k_BOTH_ARROW;
    break;
  default:
    lval_arrow = k_NO_ARROW;
    break;
  }
  lval_result = cons(lval_arrow, lval_result);

  lval_style = cvflonum((FLOTYPE) line->style);
  lval_result = cons(lval_style, lval_result);

  lval_width = cvflonum((FLOTYPE) line->width);
  lval_result = cons(lval_width, lval_result);

  lval_color = cvfixnum((FIXTYPE) line->color);
  lval_result = cons(lval_color, lval_result);

  lval_size = newdcomplex((double) line->size[0], (double) line->size[1]);
  lval_result = cons(lval_size, lval_result);

  lval_location = newdcomplex((double) image->loc[0], (double) image->loc[1]);
  lval_result = cons(lval_location, lval_result);

  /* restore the stack */
  xlpopn(7);

  return(lval_result);
}


/*****************************************************************************
 * (send <line-image> :storeon)
 *	==> returns list (send TANGO:LINE_IMAGE_CLASS :new <visibility_kwd>
 *				*TANGO_WIDGET*
 *				#C(<location-x> <location-y)
 *				#C(<size-x> <size-y>)
 *				<tango_color>
 *				<line_width_float>
 *				<line_style>
 *				<arrow-kwd>)
 ****************************************************************************/
LVAL Tango_Line_Image_Class_Method_STOREON()
{
  extern LVAL s_SEND, k_NEW, s_TANGO_W; /* wc_Xtango.c */
  LVAL o_image, lval_result;
  TANGO_IMAGE image;

  xlsave1(lval_result);		/* protect from gc */

  image = Tcls_Get_TANGOIMAGEOBJ_Arg_Returning_Validated_TangoImageID(&o_image);
  xllastarg();

  lval_result = Tango_Line_Image_Get_Values(image);

  lval_result = cons(s_TANGO_W,
		     lval_result);
  lval_result = cons((image->visible) ? k_VISIBLE : k_INVISIBLE,
		     lval_result);
  lval_result = cons(k_NEW,
		     lval_result);
  lval_result = cons(Tcls_Get_TANGOIMAGECLASS_Symbol_From_TANGOIMAGEOBJ(o_image),
		     lval_result);
  lval_result = cons(s_SEND,
		     lval_result);

  xlpop();
  return (lval_result);
}


/*****************************************************************************
 *
 ****************************************************************************/
Tic_Line_Init()
{
  LVAL o_TANGO_LINE_IMAGE_CLASS;

  k_NO_ARROW    = xlenter(":NO_ARROW");
  k_FORW_ARROW  = xlenter(":FORW_ARROW");
  k_BACK_ARROW  = xlenter(":BACK_ARROW");
  k_BOTH_ARROW  = xlenter(":BOTH_ARROW");

  /*--------------- create 'Class' instance 'TANGO:LINE_IMAGE_CLASS' -----------------*/
  o_TANGO_LINE_IMAGE_CLASS =
    Tcls_Create_Subclass_Of_TANGOIMAGE_CLASS("TANGO:LINE_IMAGE_CLASS",
					     TANGO_IMAGE_TYPE_LINE);
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":ISNEW",
	   FTAB_Tango_Line_Image_Class_Method_ISNEW);
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":STOREON",
	   FTAB_Tango_Line_Image_Class_Method_STOREON);

  /* :IMAGE_LOC methods differ among subclasses of TANGO:IMAGE_CLASS; most
   * use Tango_Non_Poly_Image_Class_Method_IMAGE_LOC(), but
   * TANGO:POLYLINE_IMAGE_CLASS, TANGO:POLYGON_IMAGE_CLASS, and
   * TANGO:SPLINE_IMAGE_CLASS use their own methods.
   * Thus can't add it to TANGO:IMAGE_CLASS metaclass... */
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":IMAGE_LOC",
	   FTAB_Tango_Non_Poly_Image_Class_Method_IMAGE_LOC);

  /* Message :TX_COLOR applies to almost all subclasses of TANGO:IMAGE_CLASS, with
   * the exception of TANGO:BITMAP_IMAGE_CLASS, thus can't add it to
   * TANGO:IMAGE_CLASS metaclass... */
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":TX_COLOR",
	   FTAB_Tango_Image_Class_Method_TX_COLOR);

  /* Message :TX_FILL applies to almost all subclasses of TANGO:IMAGE_CLASS, with
   * the exception of TANGO:TEXT_IMAGE_CLASS and TANGO:BITMAP_IMAGE_CLASS,
   * thus can't add it to TANGO:IMAGE_CLASS metaclass... */
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":TX_FILL",
	   FTAB_Tango_Image_Class_Method_TX_FILL);

  /* Message :TX_RESIZE doesn't apply to all subclasses of TANGO:IMAGE_CLASS, 
   * therefore can't add it to TANGO:IMAGE_CLASS metaclass. In particular,
   * :TX_RESIZE message doesn't exist on TANGO:POLYLINE_IMAGE_CLASS,
   * TANGO:POLYGON_IMAGE_CLASS, TANGO:SPLINE_IMAGE_CLASS, 
   * TANGO:BITMAP_IMAGE_CLASS, TANGO:TEXT_IMAGE_CLASS. */
  xladdmsg(o_TANGO_LINE_IMAGE_CLASS, ":TX_RESIZE",
	   FTAB_Tango_Non_Poly_Image_Class_Method_TX_RESIZE);
}
