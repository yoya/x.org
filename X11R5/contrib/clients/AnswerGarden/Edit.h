/**********************************************************

  The ANSWER GARDEN PROJECT

Copyright 1989, 1990, 1991 by the Massachusetts Institute of Technology,
Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of MIT not be used in
advertising or publicity pertaining to distribution of the software
without specific, written prior permission.

Answer Garden is a trademark of the Massachusetts Institute of
Technology.  All rights reserved.

  The ANSWER GARDEN:  EDIT.H

     The major editing portions of Answer Garden.

  Mark Ackerman
  MIT/Center for Coordination Science

  ackerman@athena.mit.edu
  ackerman@xv.mit.edu

***********************************************************/
#ifndef _Edit_h
#define _Edit_h

#include "AG.h"

#define MaxTypeToggles 32  /* if you get more than 32 node types in AG,
			      up this number */

#define E_PRELIMINARY 0
#define E_ADD_NODE_IN_NODEINFO  1
#define E_ADD_NODE_IN_FILE_BEFORE_SAVE   2
#define E_ADD_NODE_IN_FILE_AFTER_SAVE    3
#define E_EDIT_FILE 4
#define E_EDIT_NODEINFO 5
#define E_ADD_NODEINFO_FROM_EDITFILE 6

typedef struct _EditFileInfo {
    Widget shell;     
    Widget text;
    Widget abort_button;
    Widget close_button;
    Widget save_button;
  } EditFileInfo;

typedef struct _EditNIPGroup {
  Widget toggle1;
  Widget toggle2;
  Widget text;
} EditNIPGroup;

typedef struct _EditNIPInfo {
    Widget shell;     
    EditNIPGroup name;
    EditNIPGroup label;
    EditNIPGroup storage;
    EditNIPGroup date;
    EditNIPGroup expert;
    Widget type_toggles[MaxTypeToggles];  
    Widget okay_button;
    char *header_values[NFileServHeaderValues];
  } EditNIPInfo;

typedef struct _InputDialogInfo {
    Widget shell;   
    Widget dialog;
    Boolean done;
    Boolean cancel;
    String string;
  } InputDialogInfo;

typedef struct _EditInfo 
{
    int state;   /* What is the state of the state machine? 
		    Some routines can handle more than one state.  */
    Widget menu_shell;
    EditFileInfo editFile_info;
    EditNIPInfo editNIP_info;
    InputDialogInfo  inputDialog_info;
    Boolean new_nodeService;
    char *node_name;
    char *label;
    AGNodeTypes type;
    char *location;
    char **header_values;
    NodeInfoPtr node_info;
};


#define MaxLabelLength 80
#define MaxEditFileSize 1000

Boolean Edit_Check_Write_Permission();

void Edit_EditInfo_Destroy();
void Edit_EditInfo_Destroy();
void Add_Node_Incoming();
void Edit_File();
void Edit_Node_Info();


#endif /*_Edit_h*/
