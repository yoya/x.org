/* $XConsortium: css_plain.c,v 5.4 91/12/26 18:15:31 hersh Exp $ */
/***********************************************************
Copyright (c) 1989, 1990, 1991 by Sun Microsystems, Inc. and the X Consortium.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Sun Microsystems,
the X Consortium, and MIT not be used in advertising or publicity
pertaining to distribution of the software without specific, written
prior permission.

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/*
 * this file contains the element handling procedures for elements
 * which are stored in the format they are received.  This includes
 * PEX elements stored in PEX format and any imp. dep. elements which
 * are not defined by PEX.
 */

#include "ddpex.h"
#include "miStrMacro.h"

extern ocTableType ParseOCTable[];
extern destroyTableType DestroyOCTable[];
extern ocTableType CopyOCTable[];
extern ocTableType InquireOCTable[];
extern ocTableType ReplaceOCTable[];

#define SET_STR_HEADER(pStruct, pheader)                                \
    register miStructPtr pheader = (miStructPtr) pStruct->deviceData

#define PEX_EL_TYPE(POC) ((ddElementInfo *)(POC))->elementType


ddpex4rtn
createCSS_Plain(pStruct, pPEXOC, ppCSSElement)
/* in */
	diStructHandle  pStruct;
	ddPointer       pPEXOC;
/* out */
	miGenericElementStr **ppCSSElement;
{
	ddpex4rtn       err = Success;
	SET_STR_HEADER(pStruct, pheader);

	*ppCSSElement = (miGenericElementPtr) NULL;

	/*
	 * Parse into server native format
	 * If we make it here OC is either proprietary or valid PEXOC
	 * still need to check proprietary to avoid Null Function Ptrs
	 */
	if (MI_HIGHBIT_ON(PEX_EL_TYPE(pPEXOC)))
		err = (*ParseOCTable[MI_OC_PROP]) (pPEXOC, ppCSSElement);
	else
		err = (*ParseOCTable[PEX_EL_TYPE(pPEXOC)])
			(pPEXOC, ppCSSElement);

	if (err != Success)
		return (PEXERR(PEXOutputCommandError));

	(*ppCSSElement)->pStruct = pStruct;
	(*ppCSSElement)->element.pexOClength =
		((ddElementInfo *) pPEXOC)->length;	/* protocol size, not
							 * parsed size */
	(*ppCSSElement)->element.elementType =
		((ddElementInfo *) pPEXOC)->elementType;

	MISTR_NUM_EL(pheader)++;
	MISTR_LENGTH(pheader) += ((ddElementInfo *) pPEXOC)->length;

	return (Success);
}

ddpex4rtn
destroyCSS_Plain(pStruct, pCSSElement)
/* in */
	diStructHandle  pStruct;
	miGenericElementPtr pCSSElement;
/* out */
{
	ddpex4rtn       err = Success;
	SET_STR_HEADER(pStruct, pheader);

	MISTR_NUM_EL(pheader)--;
	MISTR_LENGTH(pheader) -= MISTR_EL_LENGTH(pCSSElement);

	/*
	 * Free the parsed format
	 * If we make it here OC is either proprietary or valid PEXOC
	 * still need to check proprietary to avoid Null Function Ptrs
	 * even though we use the same destroy routine
	 */

	if (MI_HIGHBIT_ON(pCSSElement->element.elementType))
	    (*DestroyOCTable[MI_OC_PROP]) (pCSSElement);
	else 
	    (*DestroyOCTable[(int) (pCSSElement->element.elementType)])
		(pCSSElement);

	return (err);
}

ddpex4rtn
copyCSS_Plain(pSrcCSSElement, pDestStruct, ppDestCSSElement)
/* in */
	miGenericElementPtr pSrcCSSElement;
	diStructHandle  pDestStruct;
/* out */
	miGenericElementStr **ppDestCSSElement;
{
	ddpex4rtn       err = Success;
	SET_STR_HEADER(pDestStruct, pheader);

	*ppDestCSSElement = (miGenericElementPtr) NULL;

	/*
	 * If we make it here OC is either proprietary or valid PEXOC
	 * still need to check proprietary to avoid Null Function Ptrs
	*/
	if (MI_HIGHBIT_ON(pSrcCSSElement->element.elementType))
	    err = (*CopyOCTable[MI_OC_PROP])
		    (pSrcCSSElement, ppDestCSSElement);
	else 
	    err = (*CopyOCTable[(int) (pSrcCSSElement->element.elementType)])
		    (pSrcCSSElement, ppDestCSSElement);

	if (err != Success)
		return (err);

	(*ppDestCSSElement)->pStruct = pDestStruct;
	(*ppDestCSSElement)->element.pexOClength =
		pSrcCSSElement->element.pexOClength;
	(*ppDestCSSElement)->element.elementType =
		pSrcCSSElement->element.elementType;

	MISTR_NUM_EL(pheader)++;
	MISTR_LENGTH(pheader) += MISTR_EL_LENGTH(*ppDestCSSElement);

	return (Success);
}

ddpex4rtn
replaceCSS_Plain(pStruct, pCSSElement, pPEXOC)
	diStructHandle  pStruct;
	miGenericElementPtr pCSSElement;
	ddElementInfo  *pPEXOC;
{
	ddpex4rtn       err = Success;

	/*
	 * If we make it here OC is either proprietary or valid PEXOC
	 * still need to check proprietary to avoid Null Function Ptrs
	*/
	if (MI_HIGHBIT_ON(pCSSElement->element.elementType))
	    err = (*ReplaceOCTable[MI_OC_PROP]) (pPEXOC, &pCSSElement);
	else 
	    err = (*ReplaceOCTable[(int) (pCSSElement->element.elementType)])
		    (pPEXOC, &pCSSElement);

	if (err == Success) {
		pCSSElement->pStruct = pStruct;
		pCSSElement->element.elementType = pPEXOC->elementType;
		pCSSElement->element.pexOClength = pPEXOC->length;
	}
	return (err);
}

ddpex4rtn
inquireCSS_Plain(pCSSElement, pBuf, ppPEXOC)
	miGenericElementPtr pCSSElement;
	ddBuffer       *pBuf;
	ddElementInfo  **ppPEXOC;
{
	ddpex4rtn       err = Success;

	/*
	 * If we make it here OC is either proprietary or valid PEXOC
	 * still need to check proprietary to avoid Null Function Ptrs
	*/
	if (MI_HIGHBIT_ON(pCSSElement->element.elementType))
	    err = (*InquireOCTable[MI_OC_PROP]) (pCSSElement, pBuf, ppPEXOC);
	else 
	    err = (*InquireOCTable[(int) (pCSSElement->element.elementType)])
		    (pCSSElement, pBuf, ppPEXOC);
	return (err);
}
