/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* ************************************************************ *\
	btr2.c
	More library routines for reading B-trees.
	Include file ``bt.h'' declares the procedures for clients.
	Include file ``btint.h'' declares common structures for the implementation modules.
\* ************************************************************ */

static char rcsid[] = "$Header: /afs/.andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/overhead/util/lib/RCS/btr2.c,v 2.8 89/04/04 09:45:26 cfe Exp $";

#include <andyenv.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <stdio.h>
#include <andrewos.h>	/* file, time, strings */
#include <ctype.h>
#include <errno.h>
#include <truth.h> /* itc.h -> truth.h DMT */
#include <sys/stat.h>
#include <netinet/in.h>
#include <util.h>
#ifdef WHITEPAGES_ENV  /* avoid makedepend "errors" */
#include <btint.h>
#endif /* WHITEPAGES_ENV   */

extern int errno;

extern char *malloc();
extern char *realloc();

extern int br_Debugging;

/*
Get status of (the root file of) an open b-tree.  Returns as much information as will fit in longwords in the caller's area.  If the callee's BTHead structure is smaller than the caller's, the remaining words will be set to -1.
Declaration:
	extern bt_ErrorCode bt_GetFixedHead(bt, hdrloc, (int) sizeof(BTHead));
	struct BTree *bt;
	struct BTHead *hdrloc;
*/
bt_ErrorCode bt_GetFixedHead(bt, hdrloc, siz)
struct BTree *bt;
struct BTHead *hdrloc;
int siz;
{
    int MinSize, Scanner;
    unsigned long *Scn;
    struct BTr *bp = (struct BTr *) bt;

    if (bp->Tag != BTrTag) return bterr_NotABTree;
    siz = siz / (sizeof(unsigned long));
    MinSize = MIN(siz, (sizeof(struct BTHead) / sizeof(unsigned long)));
    /* the use for MinSize will be when a new BTHead structure is defined and parts of it can be stored into only if they are within MinSize of hdrloc. */
    if (MinSize < (sizeof(struct BTHead) / sizeof(unsigned long))) return bterr_NoSuchMode;
    Scn = &hdrloc->bthVersion;
    for (Scanner = 0; Scanner < siz; Scanner++) *Scn++ = (unsigned long) -1;
    hdrloc->bthVersion = bp->Root->Head.BTVersion;
    hdrloc->bthMaxFileSize = bp->Root->Head.BTMaxFileSize;
    hdrloc->bthDepth = bp->Root->Head.BTDepth;
    hdrloc->bthFixedHeadSize = bp->Root->Head.BTFixedHeadSize;
    hdrloc->bthCTime1 = bp->Root->Head.BTCTime1;
    hdrloc->bthCTime2 = bp->Root->Head.BTCTime2;
    hdrloc->bthMTime1 = bp->Root->Head.BTMTime1;
    hdrloc->bthMTime2 = bp->Root->Head.BTMTime2;
    hdrloc->bthMWhere = bp->Root->Head.BTMWhere;
    hdrloc->bthMWho = bp->Root->Head.BTMWho;
    hdrloc->bthLockStyle = bp->Root->Head.BTLockStyle;
    return bterr_NoError;
}

/* Get attributes of the current cursor location--the ID1, ID2, and filename of the current file, a pointer to a stat(2) buffer of that file, and the offset within that file.  Pointers returned are pointers to areas owned by the b-tree package: do not change them, and copy the contents if you want to save them.
Declaration:
	extern bt_ErrorCode bt_GetCursorAttributes(curs, ID1, ID2, StatPtr,
						Filename, Offset);
	struct btCursor *curs;
	unsigned long *ID1, *ID2, *Offset;
	struct stat **StatPtr;
	char **Filename;
*/
bt_ErrorCode bt_GetCursorAttributes(curs, ID1, ID2, StatPtr, Filename, Offset)
struct btCursor *curs;
unsigned long *ID1, *ID2, *Offset;
struct stat **StatPtr;
char **Filename;
{
    struct btC *bC = (struct btC *) curs;

    if (bC == NULL || bC->Tag != btCTag
	 || bC->Tree == NULL || bC->Tree->Tag != BTrTag)
	return bterr_CursorTreeDamaged;
    if (bC->State != AtKey) return bterr_NotAtKey;
    if (bC->FP == NULL || bC->FP->Tag != btFileTag) return bterr_CursorTreeDamaged;
    *ID1 = bC->FP->Head.BTID1;
    *ID2 = bC->FP->Head.BTID2;
    *StatPtr = &bC->FP->FileStat;
    *Filename = bC->FP->FileName;
    *Offset = bC->FP->Index[bC->IndexPos];
    return bterr_NoError;
}
