/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
/*
 * File: linkl.h
 *
 */

#ifndef LINKL_H
#define LINKL_H

#include	"common.h"

typedef	struct _LinkLeaf
{
    struct	_LinkLeaf*	next;
    struct	_LinkLeaf*	prev;
    Pointer					contents;
}
	LinkLeaf;

typedef	struct
{
    LinkLeaf				*top;
    LinkLeaf				*bottom;
    LinkLeaf				*current;
    LinkLeaf				*current2;
}
	LinkList;


#define	ListIsEmpty(list)		((list)->top == (LinkLeaf *)(list))
#define	TopOfList(list)			(list)->top->contents
#define	BottomOfList(list)		(list)->bottom->contents
#define	CurrentContentsOfList(list)	(list)->current->contents
#define	CurrentContentsOfList2(list)	(list)->current2->contents


#define	ForAllInList(list)	for(((list)->current) = ((list)->top);((list)->current) != (LinkLeaf *)(list);((list)->current) = ((list)->current)->next)

#define	ForAllInList2(list)	for(((list)->current2) = ((list)->top);((list)->current2) != (LinkLeaf *)(list);((list)->current2) = ((list)->current2)->next)

#define	ScanList(list, current, dest, type) for((current) = ((list)->top),(dest) = (type)(current->contents);(current) != (LinkLeaf *)(list);(current) = (current)->next,(dest) = (type)(current->contents))

/* function prototypes: */
/* linkl.c: */
Global void initList P((LinkList *list ));
Global void prependToList P((LinkList *list , Pointer contents ));
Global void appendToList P((LinkList *list , Pointer contents ));
Global Pointer deleteFirst P((LinkList *list ));
Global Pointer deleteLast P((LinkList *list ));
Global Pointer deleteCurrent P((LinkList *list ));
Global void AppendToCurrent P((LinkList *list , Pointer contents ));
Global void PrependToCurrent P((LinkList *list , Pointer contents ));
Global void deleteList P((LinkList *list ));
Global void freeList P((LinkList *list ));
Global void freeCurrent P((LinkList *list ));
Global Bool freeMatchingLeaf P((LinkList *list , Pointer contents ));
Global LinkLeaf *findMatchingLeaf P((LinkList *list , Pointer contents ));
Global int indexOfContents P((LinkList *list , Pointer contents ));
Global Pointer getIndexedContents P((LinkList *list , int index ));
Global void freeLeaf P((LinkList *list , LinkLeaf *leaf ));
Global Pointer deleteLeaf P((LinkList *list , LinkLeaf *leaf ));

/* end function prototypes */

#endif  /* LINKL_H */
