/* $XConsortium: pexutdbint.c,v 1.0 93/11/22 12:24:19 rws Exp $ */


/******************************************************************************/
/*  (c) Copyright Hewlett-Packard Company, 1992,  Fort Collins, Colorado      */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted provided  */
/*  that the above copyright notices appear in all copies and that both the   */
/*  copyright notices and this permission notice appear in supporting         */
/*  documentation, and that the name of Hewlett-Packard not be used in        */
/*  advertising or publicity pertaining to distribution of the software       */
/*  without specific, written prior permission.                               */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* Module:  PEXlib Double-Buffer Utilities                                    */
/*                                                                            */
/* File Content:                                                              */
/*   This file contains source code for data management functions used by the */
/*   PEXlib double-buffer utilities.                                          */
/*                                                                            */
/******************************************************************************/


/*
** Include files
*/
#include <X11/PEX5/PEXlib.h>
#include "pexutdbint.h"


/*
** Global internal data
*/
static PEXUtDBData *pexutdb_head = (PEXUtDBData *)NULL;   /* modified by all  */
                                                          /* *_node functions */



/******************************************************************************/
/*                                                                            */
/* lookup_node                                                                */
/*                                                                            */
/******************************************************************************/

PEXUtDBData *lookup_node( win, dpy )
    Window      win;
    Display     *dpy;
{
    PEXUtDBData *ptr;
    
    ptr = pexutdb_head;
    while ( ptr != (PEXUtDBData *)NULL ) {
        if ( ( ptr->window != win ) || ( ptr->display != dpy ) )
            ptr = ptr->next;
        else
            break;
    }

    return( ptr );
}



/******************************************************************************/
/*                                                                            */
/* add_node                                                                   */
/*                                                                            */
/******************************************************************************/

PEXUtDBData *add_node( win, dpy )
    Window      win;
    Display     *dpy;
{
    PEXUtDBData *ptr;
    
    if ( pexutdb_head == (PEXUtDBData *)NULL ) {
        /*
        ** if no data, create a new list of nodes
        */
        ptr = (PEXUtDBData *)malloc( sizeof( PEXUtDBData ) );
        if ( ptr == (PEXUtDBData *)NULL )
            return( ptr );
        ptr->next = (PEXUtDBData *)NULL;
    } else {
        /*
        ** check for existing node with matching search keys
        */
        ptr = pexutdb_head;
        while ( ptr != (PEXUtDBData *)NULL ) {
            if ( ( ptr->window == win ) && ( ptr->display == dpy ) )
                return( (PEXUtDBData *)NULL );
            ptr = ptr->next;
        }
        /*
        ** allocate new node
        */
        ptr = (PEXUtDBData *)malloc( sizeof( PEXUtDBData ) );
        if ( ptr == (PEXUtDBData *)NULL )
            return( ptr );
        /*
        ** add the new data node at the beginning of the list;
        ** this optimizes searches for the most recent additions
        */
        ptr->next = pexutdb_head;
    }

    pexutdb_head = ptr;
    return( ptr );
}



/******************************************************************************/
/*                                                                            */
/* delete_node                                                                */
/*                                                                            */
/******************************************************************************/

void delete_node( win, dpy )
    Window      win;
    Display     *dpy;
{
    PEXUtDBData *ptr, *prev;

    if ( pexutdb_head == (PEXUtDBData *)NULL )
        return;
        
    /*
    ** first, need to find the previous node
    */
    ptr = pexutdb_head;
    prev = pexutdb_head;
    while ( ptr != (PEXUtDBData *)NULL ) {
        if ( ( ptr->window != win ) || ( ptr->display != dpy ) ) {
            prev = ptr;
            ptr = ptr->next;
        } else
            break;
    }

    /*
    ** now, we have the previous node;
    ** point the previous node to the node after the node we are deleting
    */
    if ( ptr == pexutdb_head )
        pexutdb_head = ptr->next;
    else
        prev->next = ptr->next;
        
    /*
    ** now, free the node
    */
    free( ptr );
}
