/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/RCS/sliderv.ch,v 2.7 1991/09/30 18:26:45 susan Exp $ */
/* $ACIS: $ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/RCS/sliderv.ch,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsid_sliderV_H = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/RCS/sliderv.ch,v 2.7 1991/09/30 18:26:45 susan Exp $ ";
#endif /* !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS) */

#include <fontdesc.ih>
#include <rect.h>

class sliderV[sliderv] : valueview[valuev]
{
 overrides:
  DrawFromScratch(long x,y,width,height);
  DrawHighlight();
  DrawDehighlight();
  DrawNewValue();
  DoHit(enum view_MouseAction type, long x,y,hits)
    returns struct sliderV  *;
  LookupParameters();
methods:
  GetValueString() returns char *;
  Drawslider(boolean fullupdate,struct rectangle *rr);
macromethods:
  GetTmpVal() (self->tmpval)
classprocedures:
  InitializeClass() returns boolean;
  InitializeObject() returns boolean;
  FinalizeObject(); 
data:
  char * label;
  char * fontname;
  char buf[32];
  short fontsize;
  struct fontdesc * normalfont, * boldfont, * activefont, *valuefont;
  long maxval, minval , rv;
  long increment, tmpval,lasty;
  struct rectangle wheelrec,labelrec,valrec,inwheelrec,lastdrawn;
  long x,y,width,height,granular,gran;
  struct sbutton_prefs *prefs;
  boolean doclip;
  long sliderwidth;
  boolean immediate, immediatedefault,readonly,readonlydefault;
  int mono;
};

/* parameters to spst
   label		string		Defaults to NULL
   bodyfont		string		Defaults to "andytype"
   bodyfont-size	long		Defaults to 10
   max-value		long		defaults to 100
  min-value		long		dafaults to 0
  increment		float		defaults to 1.0
*/
