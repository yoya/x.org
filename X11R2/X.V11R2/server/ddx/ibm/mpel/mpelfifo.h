#ifndef MPEL_FIFO_SEEN
#define	MPEL_FIFO_SEEN 1
/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $Header:mpelfifo.h 5.1$ */
/* $ACIS:mpelfifo.h 5.1$ */
/* $Source: /vice/X11/src/server/ddx/ibm/mpel/RCS/mpelfifo.h,v $ */

#ifndef lint
static char *rcsidmpelfifo = "$Header:mpelfifo.h 5.1$";
#endif

   /*
    * Definitions for megapel hardware fifo 
    */

#define	MPEL_FIFO_LEN	2048
#define	MPEL_MAXRETRY	500

#define MPELWaitFifo()   	{while (MPEL_PSR&PSR_FIFO_NOT_EMPTY);}

   /*
    * structure elements (instructions)
    */

#define	fifocmd(cmd)		mpelWFifo(0,(cmd),NULL,NULL)
#define	fifocmd1(cmd,a)		mpelWFifo(1,(cmd),(short *)(a),NULL)
#define	fifocmd2(cmd,a1,a2)	mpelWFifo(2,(cmd),(short *)(a1),(short *)(a2))
#define	fifocmdn(cmd,n,a)	mpelWFifo((n),(cmd),(short *)(a),NULL)
#define	fifocmdv(cmd,n,a,dl)	mpelWFifo((n),(cmd),(short *)(a),(short *)(dl))

#define	MPELSetLineType(t)			fifocmd1(0x5040,(t))
#define	MPELSetPolylineColor(i)			fifocmd1(0x5042,(i))
#define	MPELSetMarkerType(t)			fifocmd1(0x5043,(t))
#define	MPELSetMarkerSizeScaleFactor(f)		fifocmd1(0x5044,(f))
#define	MPELSetPolymarkerColor(i)		fifocmd1(0x5045,(i))
#define	MPELSetTextFont(f)			fifocmd1(0x5046,(f))
#define	MPELSetTextPrecision(p)			fifocmd1(0x5047,(p))
#define	MPELSetCharExpansionFactor(x)		fifocmd1(0x5048,(x))
#define	MPELSetCharSpacing(s)			fifocmd1(0x5049,(s))
#define	MPELSetTextColor(i)			fifocmd1(0x504a,(i))
#define	MPELSetCharHeight(h)			fifocmd1(0x504b,(h))
#define	MPELSetCharUpVector(x,y)		fifocmd2(0x504c,(x),(y))
#define	MPELSetTextPath(p)			fifocmd1(0x504d,(p))
#define	MPELSetTextAlignment(h,v)		fifocmd2(0x5050,(h),(v))
#define	MPELSetPgonInteriorStyle(i)		fifocmd1(0x5051,(i))
#define	MPELSetPgonInteriorStyleIndex(i)	fifocmd1(0x5052,(i))
#define	MPELSetPgonInteriorColor(i)		fifocmd1(0x5053,(i))
#define	MPELSetPgonEdgeFlag(f)			fifocmd1(0x5054,(f))
#define	MPELSetPgonEdgeLinetype(t)		fifocmd1(0x5055,(t))
#define	MPELSetPgonEdgeColor(i)			fifocmd1(0x5056,(i))
#define	MPELSetLogicalOp(o)			fifocmd1(0x5060,(o))
#define	MPELSetPlaneMask(m)			fifocmd1(0x5061,(m))
#define	MPELSetTextCodePage(p)			fifocmd1(0x5062,(p))
#define	MPELSetHighlightingColor(i)		fifocmd1(0x5063,(i))
#define	MPELSetBlinkControlParameters(p)	fifocmdn(0x5064,10,(p))
#define	MPELSetLineOnLineBg(i)			fifocmd1(0x5065,(i))
#define	MPELSetUserLineStyle(s)			fifocmd2(0x5066,(s))
#define	MPELSetLineOnLineMask(m)		fifocmd1(0x5067,(m))
#define	MPELBeginPgon()				fifocmd(0x500a)
#define	MPELCircle(c)				fifocmdn(0x5006,10,(c))
#define	MPELCircularArc(a)			fifocmdn(0x5007,14,(a))
#define	MPELClose()				fifocmd(0x500c)
#define	MPELEllipse(e)				fifocmdn(0x5008,14,(e))
#define	MPELEllipticalArc(a)			fifocmdn(0x5009,18,(a))
#define	MPELEndPgon()				fifocmd(0x500b)
#define	MPELFillCircle(c)			fifocmdn(0x5005,10,(c))
#define	MPELFillRectangle(r)			fifocmdn(0x5004,12,(r))
#define	MPELCircularArcWConnectivity(a)		fifocmdn(0x5026,20,(a))
#define	MPELEllipticalArcWConnectivity(a)	fifocmdn(0x5027,23,(a))
#define	MPELBeginHighlight(h)			fifocmd1(0x501e,(h))
#define	MPELEndHighlight()			fifocmd(0x501f)
#define	MPELEnablePick()			fifocmd(0x501a)
#define	MPELDisablePick()			fifocmd(0x501b)
#define	MPELSynchronize(s)			fifocmdn(0x5020,10,(s))
#define	MPELSetScissor(b)			fifocmdn(0x5017,12,(b))
#define	MPELSetTransformAndClipModes(m)		fifocmdn(0x5028,14,(m))
#define	MPELSetClipping(b)			fifocmdn(0x5014,16,(b))
#define	MPELSetTransformationMatrix(m)		fifocmdn(0x5015,26,(m))
#define	MPELSetViewport(v)			fifocmdn(0x5016,12,(v))
#define	MPELVPMCopy(r)				fifocmdn(0x500e,18,(r))
#define	MPELVPMBLTDest(r)			fifocmdn(0x5011,18,(r))
#define	MPELSrcBLTVPM(r)			fifocmdn(0x5012,20,(r))
#define	MPELSrcBLTVPMWColorExpansion(b)		fifocmdn(0x5013,18,(b))
#define	MPELSrcBLTVPMWColorExpansionROP(b)	fifocmdn(0x5013,20,(b))
#define	MPELSrcBLTVPMNoMask(s)			fifocmdn(0x5021,30,(s))
#define	MPELSrcBLTVPMWMask(s)			fifocmdn(0x5021,40,(s))
#define	MPELVPMBLTDestNoMask(s)			fifocmdn(0x5022,30,(s))
#define	MPELVPMBLTDestWMask(s)			fifocmdn(0x5022,40,(s))
#define	MPELVPMtoVPMCopyNoMask(s)		fifocmdn(0x5023,30,(s))
#define	MPELVPMtoVPMCopyWMask(s)		fifocmdn(0x5023,40,(s))
#define	MPELTileBLTVPMNoMask(s)			fifocmdn(0x5024,22,(s))
#define	MPELTileBLTVPMWMask(s)			fifocmdn(0x5024,32,(s))

#define	MPELDisjointPolyline(n,l) fifocmdn(0x501d,4+(n)*sizeof(mpelPoint),(l))
#define	MPELMultiline(n,l)  	  fifocmdn(0x5003,4+(n)*sizeof(mpelPoint),(l))
#define	MPELPolyline(n,l) 	  fifocmdn(0x5000,4+(n)*sizeof(mpelPoint),(l))
#define	MPELPolymarker(n,m)	  fifocmdn(0x5001,4+(n)*sizeof(mpelPoint),(m))
#define	MPELNoOp(n,d)		  fifocmdn(0x501c,4+(n),(d))

#define	MPELAnnotationText(n,t)	  fifocmdv(0x5002,12,(t),((CARD32)(n)))
#define	MPELText(n,t)		  fifocmdv(0x500d,10,(t),((CARD32)(n)))
#define	MPELBLTImmed(n,s)	  fifocmdv(0x5018,16,(s),((CARD32)(n)))
#define	MPELBLTImmedWColorExpansion(n,s) \
				  fifocmdv(0x5019,16,(s),((CARD32)(n)))
#define	MPELBLTImmed4x4WColorExpansion(n,s) \
				  fifocmdv(0x5025,14,(s),((CARD32)(n)))

#define	MPELSendData(n,d)			fifocmdn(0,(n),(d))

	/*
	 * arguments to structure elements
	 */

typedef struct {
	INT16	x;
	INT16	y;
} mpelPoint;

typedef mpelPoint	*mpelPointList;

typedef struct {
	CARD16	rate;
	CARD32	color;
} mpelBlinkControlParameters;

typedef struct {
	mpelPoint	center;
	CARD16		radius;
} mpelCircle;

typedef struct {
	mpelCircle	circle;
	CARD16		start;
	CARD16		end;
} mpelCircularArc;

typedef struct {
	mpelPoint	center;
	INT16		majax;
	INT16		minax;
	INT16		angle;
} mpelEllipse;

typedef struct {
	mpelEllipse	ellipse;
	CARD16		start;
	CARD16		end;
} mpelEllipticalArc;

typedef struct {
	mpelPoint	lleft;
	mpelPoint	uright;
} mpelRectangle;

typedef struct {
	mpelPoint	point;
	CARD16		reserved;
	CARD16		length;
	char		*string;
} mpelAnnotationText;

typedef struct {
	mpelPoint	point;
	CARD16		length;
	char		*string;
} mpelText;

typedef struct {
	mpelCircle	circle;
	mpelPoint	startpt;
	mpelPoint	endpt;
	INT16		drawdir;
} mpelCircularArcWConnectivity;

typedef struct {
	mpelEllipse	ellipse;
	mpelPoint	startpt;
	mpelPoint	endpt;
	INT16		drawdir;
} mpelEllipticalArcWConnectivity;

typedef struct {
	INT16		funct;
	INT16		inter;
	CARD16		data;
} mpelSynchronize;

typedef struct {
	INT16		tran;
	INT16		clip;
	INT16		map;
	INT16		reserved;
} mpelTransformClipModes;

typedef struct {
	INT16		xleft;
	INT16		xright;
	INT16		yleft;
	INT16		yright;
} mpelClippingRectangle;

typedef struct {
	CARD16		ctype;
	INT16		scalef;
	INT16		matrix[3][3];
} mpelTransformationMatrix;

typedef struct {
	mpelRectangle	source;
	mpelPoint	dlleft;
	CARD16		alu;
} mpelVPMCopy;

typedef struct {
	CARD32		destaddr;
	mpelRectangle	source;
	INT16		comp;
} mpelVPMBLTDestination;

typedef struct {
	CARD32		srcaddr;
	mpelRectangle	dest;
	CARD16		bpixel;
	CARD16		alu;
} mpelSrcBLTVPM;

typedef struct {
	mpelRectangle	dest;
	CARD16		bpixel;
	CARD16		alu;
	char		*data;
} mpelBLTImmed;

typedef struct {
	mpelRectangle	dest;
	CARD16		color;
	CARD16		alu;
	char		*data;
} mpelBLTImmedWColorExpansion;

typedef struct {
	mpelRectangle	dest;
	CARD16		color;
	char		*data;
} mpelBLTImmed4x4WColorExpansion;

typedef struct {
	CARD32		srcaddr;
	mpelRectangle	dest;
	CARD16		color;
	CARD16		alu;
} mpelSrcBLTVPMWColorExpansion;

typedef struct {
	CARD32		srcaddr;
	CARD16		swidth;
	mpelPoint	suleft;
	CARD16		pwidth;
	CARD16		pheight;
	CARD32		dstaddr;
	CARD16		dwidth;
	mpelPoint	duleft;
	CARD16		alu;
	CARD32		mskaddr;
	CARD16		mwidth;
	mpelPoint	muleft;
} mpelBLTVPMWMask;

typedef struct {
	CARD32		tileaddr;
	CARD16		twidth;
	CARD16		theight;
	mpelRectangle	dest;
	CARD16		alu;
	CARD32		mskaddr;
	CARD16		mwidth;
	CARD16		muleft;
} mpelTileBLTVPMWMask;

	/*
	 * Default values for some registers
	 */
extern	CARD16		mpelMaskXlate[];
extern	mpelRectangle	mpelDfltScissor;

	/*
	 * Macros to set to corresponding X values or reset
	 * defaults
	 */

#define	mpelSetPlaneMask(m) MPELSetPlaneMask((CARD32)mpelMaskXlate[(m)&0x3ff])
#define	mpelSetALU(a)		MPELSetLogicalOp((a)+1)

#define	MPELResetScissor()	MPELSetScissor(&mpelDfltScissor)
#define	MPELResetPlaneMask()	MPELSetPlaneMask(0)
#define	MPELResetALU()		MPELSetLogicalOp(4)

#endif /* ndef MPEL_FIFO_SEEN */
