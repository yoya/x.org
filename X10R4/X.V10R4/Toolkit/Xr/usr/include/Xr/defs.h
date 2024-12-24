#include <Xr/xr-copyright.h>

/* $Header: defs.h,v 7.0 86/11/13 08:32:20 rick Exp $ */
/* Copyright 1986, Hewlett-Packard Company */
/* Copyright 1986, Massachussetts Institute of Technology */

/*************************************<+>*************************************
 *****************************************************************************
 **
 **   File:        defs.h
 **
 **   Project:     X-ray Toolbox
 **
 **   Description: 
 **
 **
 **   ------------------------ MODIFICATION RECORD   ------------------------
 *
 * $Log:	defs.h,v $
 * Revision 7.0  86/11/13  08:32:20  08:32:20  rick ()
 * Final QA release
 * 
 * Revision 6.0  86/11/10  15:19:21  15:19:21  rick ()
 * QA #2 release
 * 
 * Revision 5.5  86/11/07  13:58:34  13:58:34  rick ()
 * Added the copyright message.
 * 
 * Revision 5.4  86/11/07  11:23:04  11:23:04  rick ()
 * Changed the value of MAX_ALARM.
 * 
 * Revision 5.3  86/11/06  12:05:03  12:05:03  ed ()
 * Changed some dialog manager define values.
 * 
 * Revision 5.2  86/11/06  09:43:26  09:43:26  rick ()
 * Changed the defines for the bitmap resource id's to negatives.
 * 
 * Revision 5.1  86/11/03  13:19:06  13:19:06  rick ()
 * Added the error defines XrINVALIDTYPE and XrINVALIDSTATE.
 * 
 * Revision 5.0  86/10/28  08:25:18  08:25:18  rick ()
 * QA #1.1 release
 * 
 * Revision 4.1  86/10/23  16:17:32  16:17:32  ed ()
 * panel manager changes
 * 
 * Revision 4.0  86/10/20  12:11:00  12:11:00  rick ()
 * QA 1 release
 * 
 * Revision 3.7  86/10/20  10:21:56  10:21:56  rick ()
 * *** empty log message ***
 * 
 * Revision 3.6  86/10/20  08:45:18  08:45:18  rick ()
 * *** empty log message ***
 * 
 * Revision 3.5  86/10/20  08:36:23  08:36:23  fred ()
 * Reorganized the input defines.
 * 
 *
 *****************************************************************************
 *************************************<+>*************************************/


/*
 *  Some X Release 11 defines
 */

#define Solid		0
#define Tiled		1
#define Stippled	2
#define ForeDash	3
#define ForeBackDash	4
#define TiledDash	5
#define StippleDash	6
#define FBStippledDash	7

#define NotLast		0
#define Butt		1
#define Round		2
#define Projecting	3

#define Miter	4
#define Bevel	5

#define ClipByChildren		0
#define IncludeInferiors	1


/*
 * X-ray error codes
 *
 * These are returned by means of the global
 * variable 'xrErrno', anytime an X-ray function
 * fails.
 */

#define XrOUTOFMEM         1
#define XrINVALIDID        2
#define XrINVALIDOPTION    3
#define XrINVALIDRECT      4
#define XrINVALIDPTR       5
#define XrPARMOUTOFRANGE   6
#define XrINVALIDDEPTH     7
#define XrINVALIDMSG       8
#define XrXCALLFAILED      9
#define XrINVALIDPARM     10
#define XrFILEERROR       11
#define XrINVALIDTYPE     12
#define XrINVALIDSTATE    13



/********************************/
/*                              */
/*      X-ray Messages          */
/*                              */
/********************************/


/***************************/
/* Initialization messages */
/***************************/
#define MSG_INIT		0x0001

/******************/
/* Input messages */
/******************/
#define MSG_BLKREAD		0x0010
#define MSG_NONBLKREAD		0x0011
#define MSG_BLKHOTREAD		0x0012
#define MSG_NONBLKHOTREAD	0x0013
#define MSG_PUSHEVENT		0x0014
#define MSG_PEEKEVENT		0x0015
#define MSG_CLEAR		0x0016
#define MSG_ADDINPUT		0x0017
#define MSG_REMOVEINPUT		0x0018
#define MSG_SETTIMEOUT		0x0019
#define MSG_GETTIMEOUT		0x001A
#define MSG_CATCHEVENT		0x001B
#define MSG_IGNOREEVENT		0x001C
#define MSG_ADDWINDOW		0x001D
#define MSG_REMOVEWINDOW	0x001E
#define MSG_GETWINDOWDATA       0x001F
#define MSG_SETWINDOWDATA       0x0020
#define MSG_ADDWINDOWFUNCT      0x0021
#define MSG_REMOVEWINDOWFUNCT   0x0022
#define MSG_SETPROCESSFLAG      0x0023
#define MSG_CLEARPROCESSFLAG    0x0024

/******************************/
/* Pixmap Extraction Messages */
/******************************/
#define MSG_LOCATION		0x0040
#define MSG_GETVALUE		0x0041
#define MSG_COPYTO		0x0042
#define MSG_COPYFROM		0x0043
#define MSG_SETVALUE		0x0044

/*****************************/
/* Resource manager messages */
/*****************************/
#define MSG_OPEN		0x0050
#define MSG_CLOSE		0x0051
#define MSG_UPDATEFILE		0x0052
#define MSG_UPDATEALLFILES	0x0053
#define MSG_FIND		0x0054
#define MSG_UPDATE		0x0055
#define MSG_NEWTYPE		0x0056
#define MSG_FREETYPE		0x0057

/********************************/
/* Editor manipulation messages */
/********************************/
#define MSG_ADD			0x0070
#define MSG_REMOVE		0x0071
#define MSG_PROCESSKEY		0x0072
#define MSG_REDRAWALL		0x0073

/**************************/
/*  Field editor messages */
/**************************/
#define MSG_NEW  		0x0080
#define MSG_FREE		0x0081
#define MSG_SIZE		0x0082
#define MSG_MOVE		0x0083
#define MSG_REDRAW      	0x0084
#define MSG_GETSTATE		0x0085
#define MSG_SETSTATE		0x0086
#define MSG_MINSIZE		0x0087
#define MSG_SETCOLOR		0x0088
#define MSG_EDIT        	0x0089
#define MSG_GETITEMCOUNT	0x008A
#define MSG_GETITEMRECTS	0x008B
#define MSG_GETITEMSTATES       0x008C
#define MSG_SETITEMSTATES       0x008D
#define MSG_RESIZE              0x008E
#define MSG_GETPARAMETERS       0x008F
#define MSG_SETPARAMETERS       0x0090
#define MSG_POSITION            0x0091
#define MSG_CHANGERASTER        0x0092
#define MSG_DEACTIVATE          0x0093
#define MSG_INSERTMODE          0x0094
#define MSG_ACTIVATE            0x0095
#define MSG_GETBUFINFO          0x0096
#define MSG_SETBUFINFO          0x0097

/***************************/
/*  Editor Group messages. */
/***************************/
#define MSG_GETDEFAULTGROUP	0x00B0
#define MSG_CLEARRECT		0x00B1
#define MSG_ADDTOGROUP		0x00B2
#define MSG_GETGROUPRECT	0x00B3
#define MSG_SETGROUPRECT	0x00B4
#define MSG_ADJUSTGROUPRECT     0x00B5

/******************/
/* Panel messages */
/******************/
#define MSG_CURRENTEDITOR       0x00C0
#define MSG_SHOW                0x00C1
#define MSG_HIDE                0x00C2
#define MSG_GETPANELCONTEXT     0x00C3
#define MSG_NEWSUBPANEL         0x00C4
#define MSG_FREESUBPANEL        0x00C5
#define MSG_SHOWSUBPANEL        0x00C6
#define MSG_HIDESUBPANEL        0x00C7
#define MSG_GETCONTEXTDEFAULTS  0x00C8

/*****************/
/* Menu messages */
/*****************/
/* #define MSG_CURRENT             0x00D0 */
#define MSG_ADDSUBMENU          0x00D1
#define MSG_REMOVESUBMENU       0x00D2
#define MSG_ACTIVATEITEM        0x00D3
#define MSG_DEACTIVATEITEM      0x00D4
#define MSG_RESET               0x00D5
#define MSG_KEYBDEQUIV          0x00D6
#define MSG_ACTIVATEMENU        0x00D7
#define MSG_DEACTIVATEMENU      0x00D8
#define MSG_SETITEMEVENT        0x00D9
#define MSG_SETITEMFUNCT        0x00DA



/**********************************/
/*                                */
/*    X-ray Input Event Defines   */
/*                                */
/**********************************/


/**************************/
/* Xray input type define */
/**************************/
#define XrXRAY		0x80000000

/********************************/
/* Xray input inputType defines */
/********************************/
#define XrEDITOR	0x0001
#define XrMENU		0x0002
#define XrPANEL		0x0003
#define XrMESSAGEBOX	0x0004
#define XrFILE		0x0005

/*********************************/
/* Xray input inputCode defines. */
/*********************************/
#define XrSELECT		0x0001
#define XrSELECTUP		0x0002
#define XrMENUPOST		0x0003
#define XrMENUITEMSELECT	0x0004
#define XrPANELEDITOR		0x0005
#define XrPANELTIMER		0x0006
#define XrPANELINPUT		0x0007
#define XrRASTEREDIT		0x0008
#define XrRASTERSELECT		0x0009
#define XrSTATICRASTER		0x000A
#define XrSCROLLBAR		0x000B
#define XrTITLEBAR		0x000C
#define XrCHECKBOX		0x000D
#define XrPUSHBUTTON		0x000E
#define XrRADIOBUTTON		0x000F
#define XrTEXTEDIT		0x0010
#define XrSTATICTEXT		0x0011
#define XrMENUEDIT              0x0012
#define XrPAGEEDIT              0x0013

/*******************************/
/* Field editor input  defines */
/*******************************/
#define XrSCROLL_LEFT		0x0001
#define XrSCROLL_RIGHT		0x0002
#define XrSCROLL_UP		0x0003
#define XrSCROLL_DOWN		0x0004
#define XrSCROLL_SLIDE		0x0005
#define XrTSCROLL_SLIDE		0x0006
#define XrSCROLL_LESS		0x0007
#define XrSCROLL_MORE		0x0008
#define XrTEDIT_FIRST		0x0009
#define XrTEDIT_FULL		0x000A
#define XrTEDIT_BREAK		0x000B
#define XrTEDIT_EMPTY		0x000C
#define XrGADGET_BOX1		0x000D
#define XrGADGET_BOX2		0x000E
#define XrTITLE_REGION		0x000F
#define XrUNKNOWN_EVENT         0x0010
#define XrNEXT                  0x0011
#define XrPREVIOUS              0x0012
#define XrPEDIT_BREAK           0x0013
#define XrBUFFER_FULL           0x0014
#define XrBUFEXPANDFAILED       0x0015
#define XrBUFEXPANDED           0x0016


/***********************************/
/*                                 */
/*       X-ray Input Defines       */
/*                                 */
/***********************************/


/*********************************************/
/*  File descritor input source return types */
/*********************************************/
#define XrREAD		0x01
#define XrWRITE		0x02
#define XrEXCEPTION	0x04
#define XrXSELECT       0x08

/******************/
/* Button defines */
/******************/
#define XrLEFTBUTTONDOWN	0
#define XrLEFTBUTTONUP		1
#define XrMIDDLEBUTTONDOWN	2
#define XrMIDDLEBUTTONUP	3
#define XrRIGHTBUTTONDOWN	4
#define XrRIGHTBUTTONUP		5



/**********************************/
/*                                */
/*    Resource Manager Defines    */
/*                                */
/**********************************/


/*******************************************/
/* Resource manager resource state defines */
/*******************************************/
#define XrLOCK		0x01
#define XrUPDATE	0x02
#define XrPROTECT	0x04
#define XrAUTOADD	0x08

/***********************************************/
/*  Resource define for memory based resources */
/***********************************************/
#define XrMEMORY	0

/*******************************************************/
/*  Resource defines for X-ray built in resource types */
/*******************************************************/
#define XrTYPE_STRING8		64000
#define XrTYPE_PIXMAP		64001
#define XrTYPE_PIXMAPID		64002
#define XrTYPE_BITMAP		64003
#define XrTYPE_BITMAPID		64004
#define XrTYPE_CURSOR		64005
#define XrTYPE_CURSORID		64006
#define XrTYPE_FONTINFO		64007
#define XrTYPE_FONTID		64008
#define XrTYPE_FUNCTION		64009
#define XrTYPE_REGWINDOW	64010



/**************************************/
/*                                    */
/*   Misc. field editor defines       */
/*                                    */
/**************************************/
 

/**********************/
/* Editor state flags */
/**********************/
#define XrVISIBLE    		0x0001
#define XrSENSITIVE  		0x0002
#define XrTRANSPARENT           0x0004

/*****************************/
/* Field editor redraw modes */
/*****************************/
#define XrREDRAW_ALL		0x0001
#define XrREDRAW_SLIDE		0x0002
#define XrREDRAW_ACTIVE		0x0003
#define XrREDRAW_TEXT   	0x0004

/*********************************/
/* Text editor insert mode flags */
/*********************************/
#define XrALWAYS_ON        1
#define XrALWAYS_OFF       2
#define XrINTERACTIVE      3

/*********************************/
/* Text editor cell size defines */
/*********************************/
#define XrMAXWIDTH         -1
#define XrAVGWIDTH         -2

/*************************************/
/* Static text justification defines */
/*************************************/
#define XrCENTER_ALIGNED	1
#define XrLEFT_ALIGNED		2
#define XrRIGHT_ALIGNED		3
#define XrNO_ALIGNMENT          4

/*******************************/
/* Scrollbar component defines */
/*******************************/
#define XrSLIDEBOX              0x01
#define XrSCROLLARROWS          0x02



/************************************/
/*                                  */
/*       Misc. X-ray defines        */
/*                                  */
/************************************/


/************************/
/* Tile pattern defines */
/************************/
#define XrWHITE			-1
#define XrBLACK			-2
#define XrPERCENT25		-3
#define XrPERCENT50		-4
#define XrPERCENT75		-5
#define XrVERTICAL  		-6
#define XrHORIZONTAL  		-7
#define XrSLANTLEFT		-8
#define XrSLANTRIGHT		-9

/***********************/
/* Pixel depth defines */
/***********************/
#define XrBIT1	1
#define XrBYTE1	8
#define XrBYTE2	16
#define XrBYTE3	24
#define XrBYTE4	32

/*************************/
/* Miscellaneous defines */
/*************************/
#define XrNULLTERMINATED	-1
#define XrDEFAULT		-1
#define XrALLBUTTONS            -1
#define XrMIN_SCROLLBAR_HT      11
#define XrMIN_SCROLLBAR_WID     XrMIN_SCROLLBAR_HT
#define XrTB_MIN_HEIGHT         XrMIN_SCROLLBAR_HT
#define TRUE  		        1
#define FALSE 		        0

#ifndef NULL
#define NULL			0
#endif

#define NULLPTR 	        (char*)NULL
#define XrMAX_ALARM             0xE00000


/************************************/
/*                                  */
/*     Graphics Context defines     */
/*                                  */
/************************************/


/**************************************************/
/* Defines uses for constructing changeMask value */
/**************************************************/
#define XrALU			0x00000001
#define XrPLANEMASK		0x00000002
#define XrFOREGROUND		0x00000004
#define XrBACKGROUND		0x00000008
#define XrLINEWIDTH		0x00000010
#define XrLINESTYLE		0x00000020
#define XrCAPSTYLE		0x00000040
#define XrJOINSTYLE		0x00000080
#define XrTILE			0x00000100
#define XrSTIPPLE		0x00000200
#define XrTILEXORIGIN		0x00000400
#define XrTILEYORIGIN		0x00000800
#define XrFILLSTYLE		0x00001000
#define XrFONT			0x00002000
#define XrSUBWINMODE		0x00004000
#define XrGRAPHICSEXPOSE	0x00008000
#define XrCLIPXORIGIN		0x00010000
#define XrCLIPYORIGIN		0x00020000
#define XrCLIPMASK		0x00040000
#define XrDASHOFFSET		0x00080000
#define XrDASHLIST		0x00100000

/********************************************/
/* Defines uses for filling the value array */
/********************************************/
#define XrALUVAL		0
#define XrPLANEMASKVAL		1
#define XrFOREGROUNDVAL		2
#define XrBACKGROUNDVAL		3
#define XrLINEWIDTHVAL		4
#define XrLINESTYLEVAL		5
#define XrCAPSTYLEVAL		6
#define XrJOINSTYLEVAL		7
#define XrTILEVAL		8
#define XrSTIPPLEVAL		9
#define XrTILEXORIGINVAL	10
#define XrTILEYORIGINVAL	11
#define XrFILLSTYLEVAL		12
#define XrFONTVAL		13
#define XrSUBWINMODEVAL		14
#define XrGRAPHICSEXPOSEVAL	15	
#define XrCLIPXORIGINVAL	16
#define XrCLIPYORIGINVAL	17
#define XrCLIPMASKVAL		18
#define XrDASHOFFSETVAL		19
#define XrDASHLISTVAL		20


/**************************************/
/*                                    */
/*       Menu and Panel Defines       */
/*                                    */
/**************************************/


#define XrMAXMENU 35
#define XrMAXMENUWINDOWS 40
#define XrMAXSUBPANELS 40


#define XrITEMAREA     0x0001
#define XrUITEMAREA    0x0002
#define XrPOPUPAREA    0x0003
#define XrEXITPOPUP    0x0004
#define XrNULLAREA     0x0005
#define XrOUTSIDEMENU  0x0006

#define XrNOCHANGE     0x0000
#define XrITEMCHANGE   0x0001
#define XrAREACHANGE   0x0002
#define XrBOTHCHANGE   0x0003

#define XrSTRING      0x0001
#define XrUSTRING     0x0002
#define XrICON        0x0003
#define XrUICON       0x0004
#define XrLINE        0x0005
#define XrDBLINE      0x0006

#define XrCONTROL     0x0000
#define XrESCAPE      0x0001
#define XrMETA        0x0002
