/* $XConsortium: widgetdemo.c,v 1.0 93/11/22 12:24:31 rws Exp $ */

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DrawingA.h>
#include "PEXSimple.h"

#define NumPoints	4

void
ExposeCB(WidgetInstance, ClientData, CallData)
	Widget		WidgetInstance;
	XtPointer	ClientData;
	XmDrawingAreaCallbackStruct
			* CallData;

{
	static PEXCoord	Points[] =	{ {0.2, 0.2, 0.0},
					  {0.4, 0.7, 0.0},
					  {0.6, 0.3, 0.0},
					  {0.8, 0.8, 0.0} };
	PEXColor		Color;
	PEXRenderer		Renderer;
	PEXRendererAttributes	RendererAttrs;
	unsigned long		Mask;


	XClearWindow( XtDisplay(WidgetInstance), XtWindow(WidgetInstance) );
	XtVaGetValues( WidgetInstance, XgNrenderer, &Renderer, NULL );

	/*
	** Make the renderer background black, and ensure that the PEX image
	** gets cleared.
	*/
	Mask = PEXRABackgroundColor | PEXRAClearImage;
	RendererAttrs.clear_image = 1;
	RendererAttrs.background_color.type = PEXColorTypeRGB;
	RendererAttrs.background_color.value.rgb.red = 0.0;
	RendererAttrs.background_color.value.rgb.green = 0.0;
	RendererAttrs.background_color.value.rgb.blue = 0.0;
	PEXChangeRenderer( XtDisplay(WidgetInstance),
			   Renderer,
			   Mask,
			   &RendererAttrs );

	/*
	** Render the PEXlib image.
	*/
	PEXBeginRendering( XtDisplay(WidgetInstance),
			   XtWindow(WidgetInstance),
			   Renderer ); {

	Color.rgb.red = 1.0;
	Color.rgb.green = 0.0;
	Color.rgb.blue = 0.0;

	PEXSetLineColor( XtDisplay(WidgetInstance),
			 Renderer,
			 PEXOCRender,
			 PEXColorTypeRGB,
			 &Color );
	PEXSetLineWidth( XtDisplay(WidgetInstance),
			 Renderer,
			 PEXOCRender,
			 2.0 );
	PEXSetLineType( XtDisplay(WidgetInstance),
			Renderer,
			PEXOCRender,
			PEXLineTypeDashed );

	PEXPolyline( XtDisplay(WidgetInstance),
		     Renderer,
		     PEXOCRender,
		     NumPoints,
		     Points );
	}
	PEXEndRendering( XtDisplay(WidgetInstance), Renderer, True );
	XFlush( XtDisplay(WidgetInstance) );
}

main(argc, argv)
int argc;
char *argv[];
{
	XtAppContext 	ApplContext; 	/* application context */
	Widget 		TopLevel;	/* top level widget */
	Widget		Graphics;	/* PEX widget */

	TopLevel = XtVaAppInitialize( &ApplContext,
				      "PEXWidgtdemo",
				      NULL, 0,
				      &argc, argv,
				      NULL, NULL );
	Graphics = XtVaCreateManagedWidget( "PEXSimple1",
			 		    xgPEXSimpleWidgetClass,
					    TopLevel,
						XmNwidth, 500,
						XmNheight, 500,
					    NULL );
	XtAddCallback( Graphics,
		       XmNexposeCallback,
		       (XtCallbackProc) ExposeCB,
		       NULL);
	XtRealizeWidget(TopLevel);
	XtAppMainLoop(ApplContext);
	return(0);
}
