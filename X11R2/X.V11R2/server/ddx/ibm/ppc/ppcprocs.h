#include "colormap.h"
ColormapPtr ppcGetStaticColormap();
void ppcStoreColors();
void ppcDefineDefaultColormapColors();
int ppcListInstalledColormaps();
void ppcSolidFS();
void ppcTileFS();
void ppcStippleFS();
void ppcOpStippleFS();
Bool ppcCreateGC();
void ppcDestroyGC();
void ppcValidateGC();
unsigned int * ppcGetSpans();
PixmapPtr ppcCreatePixmap();
PixmapPtr ppcCopyPixmap();
Bool ppcPadPixmap();
void ppcPaintWindowSolid();
void ppcPaintWindowTile();
void ppcSetSpans();
Bool ppcCreateWindow();
Bool ppcDestroyWindow();
void ppcCopyWindow();
Bool ppcChangeWindowAttributes();
Bool ppcPositionWindow();
Bool ppcUnrealizeWindow();
Bool ppcRealizeWindow();
void ppcScrnZeroLine();
void ibm8514DrawColorImage();
void ibm8514ReadColorImage();
void ppcInstallColormap();
void ppcUninstallColormap();
void ppcCopyArea();
Bool ppcDepthOK();
void ppcTileRect();
void ppcOpaqueStipple();
void ppcStipple();
Bool ppcRealizeCursor();
Bool ppcUnrealizeCursor();
void ppcImageGlyphBlt();
void ppcPolyGlyphBlt();
void ppcResolveColor();
void ppcGetImage();
unsigned long *ppcGetPlane();
void ppcReplicateArea() ;
