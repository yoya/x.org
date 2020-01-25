/*
 * xexit - X logout manager
 * 
 * $Id: xexit.h,v 1.2 1994/06/06 14:27:13 stumpf Exp $
 * 
 * Copyright (c) 1992-1994   Markus Stumpf
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE X CONSORTIUM OR THE AUTHOR BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X Consortium shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from the X Consortium.
 *
 * Author:  Markus Stumpf, Technische Universitaet Muenchen
 *          <stumpf@informatik.tu-muenchen.de>
 * 
 * Filename: xexit.h
 * Date:     Thu, 08 Jan 1992  15:39:47
 * Creator:  Markus Stumpf <Markus.Stumpf@Informatik.TU-Muenchen.DE>
 * 
 * $Source: /usr/wiss/stumpf/src/CVS/xexit/xexit.h,v $
 */

#include <X11/Xos.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>

#include <stdio.h>

#ifdef linux
# include <sys/unistd.h>
#endif /* linux */

#include "malloc.h"

#include "patchlevel.h"

#define XAPPL_NAME	"xexit"
#define XAPPL_CLASS	"Xexit"
#define AUTHOR_INFO	" Markus Stumpf\n(stumpf@informatik.tu-muenchen.de)"

#define DoDelete	0
#define DoKill		1

#define XtNconfirmExit		"confirmExit"
#define XtCConfirmExit		"ConfirmExit"
#define XtOnoConfirm		"-noConfirm"
#define XtORnoConfirm		".confirmExit"

#define XtNshowVersion		"showVersion"
#define XtCShowVersion		"ShowVersion"
#define XtOshowVersion		"-showVersion"
#define XtORshowVersion		".showVersion"

#if NeedFunctionPrototypes
#define _P(x) x
#else
#define _P(x) ()
#endif                          /* NeedFunctionPrototypes */

typedef struct {
    Boolean	confirm_exit;
    Boolean	print_version;
} xexit_resourceRec, *xexit_resourcePtr;
