/* $XConsortium: pexutdb.h,v 1.0 93/11/22 12:24:07 rws Exp $ */


/******************************************************************************/
/*  (c) Copyright Hewlett-Packard Company, 1992,  Fort Collins, Colorado      */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted provided  */
/*  that the above copyright notices appear in all copies and that both the   */
/*  copyright notices and this permission notice appear in supporting         */
/*  documentation, and that the name of Hewlett-Packard not be used in        */
/*  advertising or publicity pertaining to distribution of the software       */
/*  without specific, written prior permission.                               */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* Module:  PEXlib Double-Buffer Utilities (see file pexutdb.c for more info) */
/*                                                                            */
/* Module Description:                                                        */
/*   This set of functions provide a portable interface to double-buffering.  */
/*   The utilities support the use of the Multi-Buffering extension to X      */
/*   (MBX), the PEX escapes defined by Hewlett-Packard and Evans & Sutherland */
/*   and the use of pixmaps.                                                  */
/*                                                                            */
/*   The utilities first attempt to use MBX.  If MBX is not available, then   */
/*   the double-buffer escapes are attempted.  Pixmaps are the last option.   */
/*                                                                            */
/* User Functions:                                                            */
/*   PEXUtDBConfig  - configure a window for double-buffering                 */
/*   PEXUtDBSwap    - swap the window buffers and clear the back buffer       */
/*   PEXUtDBClear   - clear the window back buffer                            */
/*   PEXUtDBFront   - reconfigure the window to render in the front buffer    */
/*   PEXUtDBResize  - resize window buffers                                   */
/*                                                                            */
/* File Content:                                                              */
/*   This file contains constants for application use with the PEXlib         */
/*   double-buffer utilities.                                                 */
/*                                                                            */
/*                                                                            */
/* Other Documentation:                                                       */
/*   Title:     "Double-Buffering Utilities for PEXlib"                       */
/*   Author:    Jeff Stevenson; Hewlett-Packard Co.                           */
/*   Dated:     November 6, 1992                                              */
/*                                                                            */
/******************************************************************************/


#ifndef _PEXUTDB_H_ /* [ */
#define _PEXUTDB_H_


#ifndef NeedFunctionPrototypes
#if defined(FUNCPROTO) || defined(__STDC__) || \
    defined(__cplusplus) || defined(c_plusplus)
#define NeedFunctionPrototypes  1
#else
#define NeedFunctionPrototypes  0
#endif /* FUNCPROTO, __STDC__, __cplusplus, c_plusplus */
#endif /* NeedFunctionPrototypes */


#ifdef __cplusplus   /* do not leave open across includes */
extern "C" {         /* for C++ V2.0 */
#endif


/*
** constants for PEXUtDBConfig
*/
#define PEXUTDBOKAY     0               /* PEXUtDBConfig was successful       */
#define PEXUTDBNODB     1               /* double-buffer not supported        */
#define PEXUTDBNODBX    2               /* double-buffer not supported for X  */
#define PEXUTDBNOTPEX5  3               /* PEX extension major version not 5  */
#define PEXUTDBNOMEM    4               /* memory allocation error in utility */

/*
** constants for PEXUtDBFront
*/
/* reuse PEXUTDBOKAY defined above */    /* PEXUtDBFront was successful       */
#define PEXUTDBNOSUPPORT 1               /* render to front buf not supported */
#define PEXUTDBFAIL      2               /* other failure in PEXUtDBFront     */



/*
** define NULL, if necessary
*/
#ifndef NULL /* [ */
#define NULL 0
#endif /* NULL  ] */


/*
** function declarations
*/

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   PEXUtDBConfig  - configure a window for double-buffering                 */
/*                                                                            */
/* Description:                                                               */
/*   The PEXUtDBConfig function configures the specified window for double-   */
/*   buffered rendering (if db_on == True) or configures the specified window */
/*   for single-buffered rendering (if db_on == False). The utilities support */
/*   double-buffered rendering for PEX primitives, and if requested (db_x ==  */
/*   True), for X and PEX primitives together.                                */
/*                                                                            */
/*   If the window is configured for double-buffering, the back buffer is     */
/*   cleared to the specified background color.                               */
/*                                                                            */
/* Returns:                                                                   */
/*   The function returns a value of zero (PEXUTDBOKAY) if it is successful;  */
/*   a non-zero return value indicates failure. The following are the meaning */
/*   of the different non-zero return values:                                 */
/*                                                                            */
/*     * A return value of PEXUTDBNODB means that double-buffering was        */
/*       requested (db_on == True), but double-buffering is not supported for */
/*       this window.                                                         */
/*                                                                            */
/*     * A return value of PEXUTDBNODBX means that double-buffered rendering  */
/*       for both X and PEX was requested (db_x == True), but is not          */
/*       supported for this window. If acceptable, the application should     */
/*       retry with db_x set to False which will double-buffer only PEX       */
/*       rendering.                                                           */
/*                                                                            */
/*     * A return value of PEXUTDBNOT5 means that the PEX server major        */
/*       version was not 5. These utilities are designed only to work with    */
/*       5.0 and 5.1 compatible servers.                                      */
/*                                                                            */
/*     * A return value of PEXUTDBNOMEM means that the utilities ran out of   */
/*       memory.                                                              */
/*                                                                            */
/* Arguments:                                                                 */
/*   dpy, win       - The dpy and win arguments specify the display and       */
/*                    window (respectively) to be configured.                 */
/*                                                                            */
/*   db_on          - db_on indicates whether the window is configured for    */
/*                    double-buffering (db_on == True) or single-buffering    */
/*                    (db_on == False).                                       */
/*                                                                            */
/*   bg_color       - bg_color specifies the background color values used     */
/*                    when clearing and should be the same color as the       */
/*                    window's background. The application must ensure that   */
/*                    both the pixel value and the red, green and blue values */
/*                    are valid in bg_color. These values should have been    */
/*                    set or computed during window creation and              */
/*                    configuration. (One way to obtain the pixel value is    */
/*                    XAllocColor which returns the pixel value for a         */
/*                    specified red, green and blue color triplet. One way to */
/*                    obtain the triplet values is XQueryColor which return   */
/*                    the red, green and blue component values for a          */
/*                    specified pixel value. There are other methods          */
/*                    available in Xlib depending on the application needs.)  */
/*                                                                            */
/*   db_x           - db_x indicates whether the application requires the     */
/*                    double-buffered rendering to support both X and PEX.  A */
/*                    value of True forces the use of a mechanism that will   */
/*                    support both X and PEX double-buffered rendering.       */
/*                                                                            */
/*   drw_id_return  - drw_id_return is a pointer to a drawable identifier.    */
/*                    This is the drawable identifier to be used for          */
/*                    subsequent rendering calls for X and PEX. For example,  */
/*                    this drawable identifier should be used for             */
/*                    PEXBeginRendering or PEXRenderNetwork, as well as for X */
/*                    rendering calls like XDrawLine. The returned value is a */
/*                    Null pointer if the function is unsuccessful.           */
/*                                                                            */
/* Restrictions:                                                              */
/*   If it is necessary to ensure that all actions have completed before      */
/*   proceeding, the application should call XSync after calling              */
/*   PEXUtDBConfig.                                                           */
/*                                                                            */
/*   This function assumes that PEXInitialize has been called for the         */
/*   specified display argument.                                              */
/*                                                                            */
/*   Renderer clip lists do not affect the clear operation. The entire back   */
/*   buffer is cleared.                                                       */
/*                                                                            */
/*   If the PEX server is version 5.0, and MBX is not available, pixmaps are  */
/*   the only other option. However, there is no way to determine if a 5.0    */
/*   PEX server supports rendering into pixmaps. In this case, PEXUtDBConfig  */
/*   will configure the window for double-buffering via pixmap, but the       */
/*   application should be prepared to receive an error from the first        */
/*   PEXBeginRendering or PEXRenderNetwork with the drawable identifier       */
/*   returned from PEXUtDBConfig. If an error does result, that error is the  */
/*   application's indication that double-buffering is not supported for the  */
/*   specified window.                                                        */
/*                                                                            */
/******************************************************************************/
extern int PEXUtDBConfig(
#if NeedFunctionPrototypes
    Display *           /* dpy */,
    Window              /* win */,
    int                 /* db_on */,
    XColor *            /* bg_color */,
    int                 /* db_x */,
    Drawable *          /* drw_id_return */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   PEXUtDBSwap    - swap the window buffers and clear the back buffer       */
/*                                                                            */
/* Description:                                                               */
/*   The PEXUtDBSwap function will swap the buffers of a window which is      */
/*   configured for double-buffering. If the window is not configured for     */
/*   double-buffering, the function will clear the window (if clear == True)  */
/*   and return the specified window identifier.                              */
/*                                                                            */
/*   Swapping the buffers means that the currently displayed buffer (i.e.,    */
/*   the front buffer) is hidden, and the currently non-displayed buffer      */
/*   (i.e., the back buffer) is made visible. After this operation, the front */
/*   and back buffers have swapped positions (i.e., the old front became the  */
/*   new back, and the old back become the new front).                        */
/*                                                                            */
/*   If PEXUtDBFront was called with front == True prior to PEXUtDBSwap, the  */
/*   returned drawable identifier is the identifier of the front buffer.      */
/*   Otherwise, the returned drawable identifier is the identifier of the     */
/*   back buffer.                                                             */
/*                                                                            */
/*   On PEX 5.1 servers, the best way to clear the back buffer is to clear    */
/*   during the next PEXBeginRendering or PEXRenderNetwork with the drawable  */
/*   identifier returned. (This is done by setting the renderer's clear_image */
/*   flag to True.)  In this case, the clear argument to PEXUtDBSwap should   */
/*   be False.                                                                */
/*                                                                            */
/*   On PEX 5.0 servers, the back buffer should be cleared by setting the     */
/*   clear argument to True, unless the application wants to render into the  */
/*   back buffer without clearing.                                            */
/*                                                                            */
/*   PEXUtDBClear is also provided to clear the back buffer independently of  */
/*   swapping the buffers. However, there are some restrictions on when it    */
/*   can be called (see PEXUtDBClear).                                        */
/*                                                                            */
/* Returns:                                                                   */
/*   The function returns a value of None if it is unsuccessful; otherwise,   */
/*   the drawable identifier to be used for subsequent rendering calls is     */
/*   returned. See also the description of the drw_id_return argument for     */
/*   PEXUtDBConfig.                                                           */
/*                                                                            */
/* Arguments:                                                                 */
/*   dpy, win      - The dpy and win arguments specify the display and window */
/*                   for which buffers are swapped.                           */
/*                                                                            */
/*   clear         - The clear argument indicates whether to clear the new    */
/*                   back buffer after the buffers are swapped.               */
/*                                                                            */
/* Restrictions:                                                              */
/*   If it is necessary to ensure that all actions have completed before      */
/*   proceeding, the application should call XSync after calling PEXUtDBSwap. */
/*                                                                            */
/*   This function should not be called with a window into which PEX is       */
/*   rendering. PEXEndRendering should be called prior to PEXUtDBSwap.        */
/*                                                                            */
/*   Renderer clip lists do not affect the clear operation. The entire back   */
/*   buffer is cleared.                                                       */
/*                                                                            */
/******************************************************************************/
extern Drawable PEXUtDBSwap(
#if NeedFunctionPrototypes
    Display *           /* dpy */,
    Window              /* win */,
    int                 /* clear */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   PEXUtDBClear   - clear the window back buffer                            */
/*                                                                            */
/* Description:                                                               */
/*   The PEXUtDBClear function will clear the back buffer associated with the */
/*   specified window. If the window is not configured for double-buffering,  */
/*   the function will clear the window.                                      */
/*                                                                            */
/*   On PEX 5.1 servers, the best way to clear the back buffer is to clear    */
/*   during PEXBeginRendering or PEXRenderNetwork with the drawable           */
/*   identifier of the back buffer. (This is done by setting the renderer's   */
/*   clear_image flag to True.)                                               */
/*                                                                            */
/*   On PEX 5.0 servers, the back buffer can also be cleared by setting the   */
/*   clear argument in PEXUtDBSwap to True.                                   */
/*                                                                            */
/* Returns:                                                                   */
/*   The function returns a value of zero if it is successful; otherwise, a   */
/*   non-zero value is returned.                                              */
/*                                                                            */
/* Arguments:                                                                 */
/*   dpy, win      - The dpy and win arguments specify the display and window */
/*                   for which the back buffer is cleared.                    */
/*                                                                            */
/* Restrictions:                                                              */
/*   TODO: UPDATE PER SPEC                                                    */
/*   If it is necessary to ensure that all actions have completed before      */
/*   proceeding, the application should call XSync after calling PEXUtDBClear.*/
/*                                                                            */
/*   This function should not be called with a window into which PEX is       */
/*   rendering. PEXEndRendering should be called prior to PEXUtDBClear.       */
/*                                                                            */
/*   Renderer clip lists do not affect the clear operation. The entire back   */
/*   buffer is cleared.                                                       */
/*                                                                            */
/******************************************************************************/
extern int PEXUtDBClear(
#if NeedFunctionPrototypes
    Display *           /* dpy */,
    Window              /* win */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   PEXUtDBFront   - reconfigure the window to render in the front buffer    */
/*                                                                            */
/* Description:                                                               */
/*   The PEXUtDBFront function configures a window, which is already          */
/*   configured for double-buffering, to render into the displayed buffer     */
/*   (i.e., the front buffer) if front == True. If the window is not          */
/*   configured for double-buffering, the function will simply return the     */
/*   specified window identifier.                                             */
/*                                                                            */
/* Returns:                                                                   */
/*   The function returns a value of zero (PEXUTDBOKAY) if it is successful;  */
/*   a non-zero return value indicates failure. The following are the meaning */
/*   of the different non-zero return values:                                 */
/*                                                                            */
/*    * A return value of PEXUTDBNOSUPPORT means that rendering to the front  */
/*      buffer is not supported on the specified drawable. The application    */
/*      can still accomplish rendering to the front buffer by turning double- */
/*      buffering off via PEXUtDBConfig. See the Restrictions below.          */
/*                                                                            */
/*    * A return value of PEXUTDBFAIL means that rendering to the front       */
/*      buffer is supported, but the function failed to enable rendering to   */
/*      the front buffer for some reason.                                     */
/*                                                                            */
/* Arguments:                                                                 */
/*   dpy, win      - The dpy and win arguments specify the display and window */
/*                   for which buffers are to be configured.                  */
/*                                                                            */
/*   front         - front indicates whether the window is configured to      */
/*                   render into the front (i.e. the displayed or visible)    */
/*                   buffer. A value of False means the window is configured  */
/*                   to render into the back (i.e. the non-visible buffer).   */
/*                   A value of True means the window is configured to render */
/*                   into the front buffer.                                   */
/*                                                                            */
/*   drw_id_return - drw_id_return is a pointer to a drawable identifier.     */
/*                   This is the drawable identifier to be used for           */
/*                   subsequent rendering calls for X and PEX. For example,   */
/*                   this drawable identifier should be used for              */
/*                   PEXBeginRendering or PEXRenderNetwork, as well as for X  */
/*                   rendering calls like XDrawLine. The returned value is a  */
/*                   Null pointer if the function is unsuccessful.            */
/*                                                                            */
/* Restrictions:                                                              */
/*   If it is necessary to ensure that all actions have completed before      */
/*   proceeding, the application should call XSync after calling PEXUtDBFront.*/
/*                                                                            */
/*   This function should not be called with a window into which PEX is       */
/*   rendering. PEXEndRendering should be called prior to PEXUtDBFront. If    */
/*   the application desires to render into the front buffer with out         */
/*   clearing, and the PEX server is version 5.1, the renderer's clear_image  */
/*   flag should be set to False before calling PEXBeginRendering.            */
/*                                                                            */
/*   PEXUtFront may not succeed on all devices. However, if PEXUtDBFront does */
/*   fail, rendering to the front buffer can still be accomplished by turning */
/*   double-buffering off via PEXUtDBConfig. It is recommended that the       */
/*   application first call PEXUtDBFront since, if it is supported, it is     */
/*   mostly likely a higher performance method than switching double-         */
/*   buffering on and off with PEXUtDBConfig.                                 */
/*                                                                            */
/******************************************************************************/
extern int PEXUtDBFront(
#if NeedFunctionPrototypes
    Display *           /* dpy */,
    Window              /* win */,
    int                 /* front */,
    Drawable *          /* drw_id_return */
#endif
);

/******************************************************************************/
/*                                                                            */
/* Function:                                                                  */
/*   PEXUtDBResize  - resize window buffers                                   */
/*                                                                            */
/* Description:                                                               */
/*   The PEXUtDBResize function will resize buffers associated with a window  */
/*   that is configured for double-buffering. If the window is not configured */
/*   for double-buffering, the function will simply return the specified      */
/*   window identifier.                                                       */
/*                                                                            */
/*   The application must handle window events and call this utility function */
/*   at the appropriate time to handle window resize.                         */
/*                                                                            */
/*   The content of the buffers is not guaranteed to be preserved after the   */
/*   resize. The best practice for interoperability is to redraw the image    */
/*   after a window resize.                                                   */
/*                                                                            */
/*   If PEXUtDBFront was called with front == True prior to PEXUtDBResize,    */
/*   the returned drawable identifier is the identifier of the front buffer.  */
/*   Otherwise, the returned drawable identifier is the identifier of the     */
/*   back buffer.                                                             */
/*                                                                            */
/* Returns:                                                                   */
/*   The function returns a value of None if it is unsuccessful; otherwise,   */
/*   the drawable identifier to be used for subsequent rendering calls is     */
/*   returned. See also the description of the drw_id_return argument for     */
/*   PEXUtDBConfig.                                                           */
/*                                                                            */
/* Arguments:                                                                 */
/*   dpy, win      - The dpy and win arguments specify the display and window */
/*                   for which buffers are resized.                           */
/*                                                                            */
/* Restrictions:                                                              */
/*   If it is necessary to ensure that all actions have completed before      */
/*   proceeding, the application should call XSync after calling              */
/*   PEXUtDBResize.                                                           */
/*                                                                            */
/*   This function should not be called with a window into which PEX is       */
/*   rendering. PEXEndRendering should be called prior to PEXUtDBResize.      */
/*                                                                            */
/******************************************************************************/
extern Drawable PEXUtDBResize(
#if NeedFunctionPrototypes
    Display *           /* dpy */,
    Window              /* win */
#endif
);


#ifdef __cplusplus
}                    /* for C++ V2.0 */
#endif


#endif /* _PEXUTDB_H_ ] */
