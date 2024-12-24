/*
 *	$Source: /u1/Sx.new/code/RCS/list.c,v $
 *	$Header: list.c,v 1.1 86/12/03 16:09:17 swick Exp $
 */

#ifndef lint
static char *rcsid_list_c = "$Header: list.c,v 1.1 86/12/03 16:09:17 swick Exp $";
#endif	lint

/* list.c -
 *
 * This file contains procedures for manipulating lists.
 * Structures may be inserted into or deleted from lists, and
 * they may be moved from one place in a list to another.
 *
 * The header file contains macros to help in determining the destination
 * locations for List_Insert and List_Move.  See list.h for details.
 *
 * Copyright (C) 1985 Regents of the University of California
 * All rights reserved.
 */

#ifndef lint
static char rcsid[] = "$Header: list.c,v 1.1 86/12/03 16:09:17 swick Exp $ SPRITE (Berkeley)";
#endif not lint

#include "sprite.h"
#include "list.h"


/*
 * ----------------------------------------------------------------------------
 *
 * List_Insert --
 *
 *	Insert the list element pointed to by itemPtr into a List after 
 *	destPtr.
 *
 * Results:
 *	If neither List_Links structure is NIL, they are assumed to be valid
 *	and SUCCESS is returned.  If either one is NIL then FAILURE is
 *	returned.
 *
 * Side effects:
 *	The list containing destPtr is modified to contain itemPtr.
 *
 * ----------------------------------------------------------------------------
 */
ReturnStatus
List_Insert(itemPtr, destPtr)
    register	List_Links *itemPtr;	/* structure to insert */
    register	List_Links *destPtr;	/* structure after which to insert it */
{
    if (itemPtr == (List_Links *) NIL || destPtr == (List_Links *) NIL
	    || !itemPtr || !destPtr) {
	return(FAILURE);
    }
    itemPtr->nextPtr = destPtr->nextPtr;
    itemPtr->prevPtr = destPtr;
    destPtr->nextPtr->prevPtr = itemPtr;
    destPtr->nextPtr = itemPtr;
    return(SUCCESS);
}


/*
 * ----------------------------------------------------------------------------
 *
 * List_Remove --
 *
 *	Remove a list element from the list in which it is contained.
 *
 * Results:
 *	If the list element is invalid or is the list header, FAILURE is 
 *	returned.  Otherwise SUCCESS is returned.
 *
 * Side effects:
 *	The given structure is removed from its containing list.
 *
 * ----------------------------------------------------------------------------
 */
ReturnStatus
List_Remove(itemPtr)
    register	List_Links *itemPtr;	/* list element to remove */
{
    if (itemPtr == (List_Links *) NIL || itemPtr == itemPtr->nextPtr
	    || !itemPtr) {
	return(FAILURE);
    }
    itemPtr->prevPtr->nextPtr = itemPtr->nextPtr;
    itemPtr->nextPtr->prevPtr = itemPtr->prevPtr;
    return(SUCCESS);
}


/*
 * ----------------------------------------------------------------------------
 *
 * List_Move --
 *
 *	Move the list element referenced by itemPtr to follow destPtr.
 *
 * Results:
 *	If either list element is invalid, FAILURE is returned.
 *	Otherwise SUCCESS is returned.
 *
 * Side effects:
 *	List ordering is modified.
 *
 * ----------------------------------------------------------------------------
 */
ReturnStatus
List_Move(itemPtr, destPtr)
    register List_Links *itemPtr; /* list element to be moved */
    register List_Links *destPtr; /* element after which it is to be placed */
{
    ReturnStatus status;

    if (itemPtr == (List_Links *) NIL || destPtr == (List_Links *) NIL
	    || !itemPtr || !destPtr) {
	return(FAILURE);
    }
    /*
     * It is conceivable that someone will try to move a list element to
     * be after itself.
     */
    if (itemPtr != destPtr) {
	status = List_Remove(itemPtr);
	if (status != SUCCESS) {
	    return(status);
	}
	status = List_Insert(itemPtr, destPtr);
	if (status != SUCCESS) {
	    return(status);
	}
    }    
    return(SUCCESS);
}


/*
 * ----------------------------------------------------------------------------
 *
 * List_Init --
 *
 *	Initialize a header pointer to point to an empty list.  The List_Links
 *	structure must already be allocated.
 *
 * Results:
 *	No value is returned.
 *
 * Side effects:
 *	The header's pointers are modified to point to itself.
 *
 * ----------------------------------------------------------------------------
 */
ReturnStatus
List_Init(headerPtr)
    register List_Links *headerPtr;  /* Pointer to a List_Links structure 
					to be header */
{
    if (headerPtr == (List_Links *) NIL || !headerPtr) {
	return(FAILURE);
    }
    headerPtr->nextPtr = headerPtr;
    headerPtr->prevPtr = headerPtr;
    return(SUCCESS);
}
