	/* $Header: /u/04c/mnt/integ/sxt/include/RCS/xvp.h,v 1.3 89/02/17 03:25:29 ejh Exp $ */

/*--------------------------------------------------------------------------*\
|
|  Name:
|	xvp.h
|
|  Description:
|	The interface to the various routines needed to interact with the
|	viewport widget.
|
|  Component of:
|	SerpentX
|
|  Related files:
|	xvp.c
|
|  Project:
|	SERPENT System
|	User Interface Prototyping
|	Software Engineering Institute
|	Carnegie Mellon University
|	Pittsburgh, PA  15213
|
|  Version:
|	1.0
|
|  Owner:  
|	Len Bass
|	ljb@sei.cmu.edu
|
|  Authors:
|	Kurt W. Hoyt
|	kwh@sei.cmu.edu
|
|	Erik J. Hardy
|	erik@sei.cmu.edu
|
|  Change History:
$Log:	xvp.h,v $
 * Revision 1.3  89/02/17  03:25:29  ejh
 * fixed x includes
 * 
 * Revision 1.2  89/02/09  20:34:10  ejh
 * added copyright notice
 * 
 * Revision 1.1  88/11/14  17:04:34  ejh
 * Initial revision
 * 
|	 2 Nov 88	project standards imposed	erik
|	 2 Jun 88	created		kwh
|
\*--------------------------------------------------------------------------*\
|
|   Distribution: Approved for public release; distribution is unlimited.
|
|   Copyright (c) 1989  by  Carnegie  Mellon  University, Pittsburgh, PA.  The
|  Software Engineering  Institute  (SEI) is a federally  funded  research and
|  development center established  and  operated by Carnegie Mellon University
|  (CMU).  Sponsored  by  the  U.S.  Department  of   Defense  under  contract
|  F19628-85-C-0003,  the  SEI  is  supported  by  the  services  and  defense
|  agencies, with the U.S. Air Force as the executive contracting agent.
|
|    Permission to use,  copy,  modify, or  distribute  this  software and its
|  documentation for  any purpose and without fee  is hereby granted, provided
|  that  the above copyright notice appear  in  all copies and that both  that
|  copyright  notice  and   this  permission  notice  appear   in   supporting
|  documentation.   Further,  the  names  Software  Engineering  Institute  or
|  Carnegie  Mellon  University  may  not be used in  advertising or publicity
|  pertaining to distribution of the software without specific, written  prior
|  permission.  CMU makes no claims  or representations  about the suitability
|  of this software for any purpose.  This software is provided "as is" and no
|  warranty,  express  or  implied,  is  made  by  the  SEI  or CMU, as to the
|  accuracy  and  functioning of the program and related program material, nor
|  shall   the  fact of  distribution   constitute  any   such  warranty.   No
|  responsibility is assumed by the SEI or CMU in connection herewith.
|
\*--------------------------------------------------------------------------*/

#ifndef XVP_H
#define XVP_H

#include	<X11/Intrinsic.h>
#include	"xht.h"

/*--------------------------------------------------------------------------*\
|  Name: xvp_fill_table
|
|  Description:
|	xvp_fill_table inserts into a hash table with the nodes necessary to
|	match a viewport widget part component name, which is a string, with
|	the actual data for the component.
|
|  Syntax:
\*                                                                          */
	void	xvp_fill_table (
	/*	node:		inout	*xht_record_node,
		widget_name:	in	*char
	*/
	);
/*                                                                          *\
|	node		the node into which to insert the data
|	widget_name	the name of the widget whose table is being filled.
|
|  Returns: Modifies node.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
|  Name: xvp_make_table
|
|  Description:
|	xvp_make_table creates a hash table with the nodes necessary to match
|	a viewport widget component name, which is a string, with the actual
|	data for the component.
|
|  Syntax:
\*                                                                          */
	xht_record_node	*xvp_make_table (
	/*	NONE
	*/
	);
/*                                                                          *\
|  Parameters: None.
|
|  Returns:
|	a record suitable for insertion into the toplevel hash table for all
|	objects and objects.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
|  Name: xvp_process
|
|  Description:
|	xvp_process generates the argument list values for the viewport
|	widget
|
|  Syntax:
\*                                                                          */
	Boolean xvp_process (
	/*	node:		in	*xht_comp_node,
		data:		in	caddr_t,
		args:		inout	ArgList,
		num_args:	inout	*Cardinal
	*/
	);
/*                                                                          *\
|  Parameters:
|	node		the node retrived for the SD component.
|	data		the element values from the SD.
|	args		the X Toolkit argument list.
|	num_args	the length of args.
|
|  Returns:
|	args is modified to reflect the new value inserted; num_args is
|	incremented; returns true if the component is used, false if not.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
|  Name: xvp_process_changes
|
|  Description:
|	xvp_process_changes processes viewport widget records from the
|	dialogue manager.
|
|  Syntax:
\*                                                                          */
	void	xvp_process_changes (
	/*	change:		in	isd_change_type,
		transaction:	in	isd_trans,
		id:		in	iid_id_type
	*/
	);
/*                                                                          *\
|  Parameters:
|	change		the change type from the dialogue manager.
|	transaction	the transaction that was sent.
|	id		the id of the record in shared data to process.
|
|  Returns: Nothing.
\*--------------------------------------------------------------------------*/

#endif
