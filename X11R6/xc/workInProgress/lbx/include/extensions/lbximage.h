/* $XConsortium: lbximage.h,v 1.2 94/11/29 19:16:42 mor Exp $ */

/******************************************************************************

Copyright (c) 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

******************************************************************************/

extern int LbxImageEncodePackBits (
#if NeedFunctionPrototypes
char *			/* inbuf */,
char *			/* outbuf */,
int			/* outbufSize */,
int			/* num_scan_lines */,
int			/* scan_line_size */,
int *			/* bytesCompressed */
#endif
);

extern int LbxImageEncodeFaxG42D (
#if NeedFunctionPrototypes
unsigned char *		/* inbuf */,
unsigned char *		/* outbuf */,
int			/* outbufSize */,
int			/* image_bytes */,
int			/* pixels_per_line */,
int			/* reverse_bits */,
int *			/* bytesCompressed */
#endif
);

extern int LbxImageDecodePackBits (
#if NeedFunctionPrototypes
char *			/* inbuf */,
char *			/* outbuf */,
int			/* num_scan_lines */,
int			/* scan_line_size */
#endif
);

extern int LbxImageDecodeFaxG42D (
#if NeedFunctionPrototypes
unsigned char *		/* inbuf */,
unsigned char *		/* outbuf */,
int			/* image_bytes */,
int			/* pixels_per_line */,
int			/* reverse_bits */
#endif
);


#define LBX_IMAGE_COMPRESS_SUCCESS		0
#define LBX_IMAGE_COMPRESS_UNSUPPORTED_FORMAT	1
#define LBX_IMAGE_COMPRESS_BAD_MALLOC		2
#define LBX_IMAGE_COMPRESS_NOT_WORTH_IT		3
