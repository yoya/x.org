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
 * File: linkl.c
 *
 * Description: Functions implementing general linked lists
 */

#include "xmove.h"

/* function prototypes: */
/* linkl.c: */
static void linkl_panic P((char *s ));

/* end function prototypes */

Global void
initList(list)
	 LinkList	*list;
{
	list->top = list->bottom = list->current = list->current2 =
	  (LinkLeaf *)list;
}

Global void
prependToList(list, contents)
	 LinkList	*list;
	 Pointer		contents;
{
	LinkLeaf		*leaf;
	
	if ((leaf = Tmalloc(LinkLeaf)) == NULL)
		linkl_panic("prependList: malloc failed\n");
	else
	{
		leaf->prev = (LinkLeaf *)list;
		leaf->next = list->top;
		list->top = list->top->prev = leaf;
		leaf->contents = contents;
	}
}

Global void
appendToList(list, contents)
	 LinkList	*list;
	 Pointer		contents;
{
	LinkLeaf		*leaf;
	
	if ((leaf = Tmalloc(LinkLeaf)) == NULL)
		linkl_panic("appendToList: malloc failed\n");
	else
	{
		leaf->next = (LinkLeaf *)list;
		leaf->prev = list->bottom;
		list->bottom = list->bottom->next = leaf;
		leaf->contents = contents;
	}
}

Global Pointer
deleteFirst(list)
	 LinkList	*list;
{
	LinkLeaf		*leaf = list->top;
	Pointer			contents;
	
	if (leaf == (LinkLeaf *)list)
		return ((Pointer)NULL);
	list->top = leaf->next;
	leaf->next->prev = (LinkLeaf *)list;
	if (list->current == leaf)
		list->current = leaf->next;
	contents = leaf->contents;
	Tfree(leaf);
	return (contents);
}

Global Pointer
deleteLast(list)
	 LinkList	*list;
{
	LinkLeaf		*leaf = list->bottom;
	Pointer			contents;
	
	if (leaf == (LinkLeaf *)list)
		return ((Pointer)NULL);
	list->bottom = leaf->prev;
	leaf->prev->next = (LinkLeaf *)list;
	if (list->current == leaf)
		list->current = leaf->prev;
	contents = leaf->contents;
	Tfree(leaf);
	return(contents);
}

Global Pointer
deleteCurrent(list)
	 LinkList	*list;
{
	LinkLeaf		*leaf = list->current;
	Pointer			contents;
	
	if (leaf == (LinkLeaf *)list)
		return ((Pointer)NULL);
	leaf->prev->next = leaf->next;
	list->current = leaf->next->prev = leaf->prev;
	contents = leaf->contents;
	Tfree(leaf);
	return(contents);
}

Global void
AppendToCurrent(list, contents)
	 LinkList	*list;
	 Pointer		contents;
{
	LinkLeaf		*leaf;
	
	if ((leaf = Tmalloc(LinkLeaf)) == NULL)
		linkl_panic("AppendToCurrent: malloc failed\n");
	else
	{
		leaf->prev = list->current;
		leaf->next = list->current->next;
		leaf->contents = contents;
		list->current->next = list->current->next->prev = leaf;
	}
}

Global void
PrependToCurrent(list, contents)
	 LinkList	*list;
	 Pointer		contents;
{
	LinkLeaf		*leaf;
	
	if ((leaf = Tmalloc(LinkLeaf)) == NULL)
		linkl_panic("PrependToCurrent: malloc failed\n");
	else
	{
		leaf->next = list->current;
		leaf->prev = list->current->prev;
		leaf->contents = contents;
		list->current->prev = list->current->prev->next = leaf;
	}
}

Global void
deleteList(list)
	LinkList  *list;
{
	while (!ListIsEmpty(list))
		deleteFirst(list);
}

Global void
freeList(list)
	LinkList  *list;
{
	while (!ListIsEmpty(list))
		Tfree(deleteFirst(list));
}

Global void
freeCurrent(list)
	LinkList  *list;
{
	Tfree(deleteCurrent(list));
}

Global Bool
freeMatchingLeaf(list, contents)
	LinkList		*list;
	Pointer			contents;
{
	LinkLeaf		*leaf;

	for(leaf = list->top; leaf != (LinkLeaf *)list; leaf = leaf->next)
		if (leaf->contents == contents)
		{
			freeLeaf(list, leaf);
			return True;
		}
	return False;
}

Global LinkLeaf*
findMatchingLeaf(list, contents)
	LinkList		*list;
	Pointer			contents;
{
	LinkLeaf		*leaf;

	for(leaf = list->top; leaf != (LinkLeaf *)list; leaf = leaf->next)
		if (leaf->contents == contents)
			return leaf;
	return (LinkLeaf *)NULL;
}

Global int
indexOfContents(list, contents)
	LinkList		*list;
	Pointer			contents;
{
	LinkLeaf		*leaf;
	int				index = 0;

	for(leaf = list->top; leaf != (LinkLeaf *)list; leaf = leaf->next, index++)
		if (leaf->contents == contents)
			return index;
	return -1;
}

Global Pointer
getIndexedContents(list, index)
	LinkList		*list;
	int				index;
{
	LinkLeaf		*leaf;
	int				count = 0;

	for(leaf = list->top; leaf != (LinkLeaf *)list; leaf = leaf->next, count++)
		if (count == index)
			return leaf->contents;
	return (Pointer)-1;
}

Global void
freeLeaf(list, leaf)
	LinkList		*list;
	LinkLeaf		*leaf;
{
	Tfree(deleteLeaf(list, leaf));
}

Global Pointer
deleteLeaf(list, leaf)
	LinkList		*list;
	LinkLeaf		*leaf;
{
	Pointer			contents;
	
	if (leaf == (LinkLeaf *)list)
		return ((Pointer)NULL);
	else if (leaf == list->current)
		return deleteCurrent(list);
	else
	{
		leaf->prev->next = leaf->next;
		leaf->next->prev = leaf->prev;
		contents = leaf->contents;
		Tfree(leaf);
		return(contents);
	}
}

static void
linkl_panic(s)
	char   *s;
{
	fprintf(stderr, "%s\n", s);
	exit(1);
}

