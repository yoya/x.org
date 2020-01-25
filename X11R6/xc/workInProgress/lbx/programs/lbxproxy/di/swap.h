/*
 * $XConsortium: swap.h,v 1.3 94/12/01 20:41:41 mor Exp $
 *
 * Copyright 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)swap.h,v 1.3 1994/11/18 20:36:17 lemke Exp $
 */

#ifndef _SWAP_H_
#define _SWAP_H_

extern void SwapConnClientPrefix();
extern void SwapNewClient();
extern void SwapCloseClient();
extern void SwapModifySequence();
extern void SwapIncrementPixel();
extern void SwapGetModifierMapping();
extern void SwapGetKeyboardMapping();
extern void SwapQueryFont();
extern void SwapChangeProperty();
extern void SwapGetProperty();
extern void SwapInvalidateTag();
extern void SwapQueryTag();
extern void SwapQueryExtension();
extern void SwapTagData();
extern void SwapLbxConnSetupPrefix();
extern void SwapConnSetupPrefix();
extern void SwapConnectionInfo();
extern void SwapPutImage();
extern void SwapGetImage();

extern void SwapGetPropertyReply();
extern void SwapInternAtomReply();
extern void SwapGetAtomNameReply();
extern void SwapLookupColorReply();
extern void SwapAllocColorReply();
extern void SwapAllocNamedColorReply();
extern void SwapModmapReply();
extern void SwapKeymapReply();
extern void SwapFont();
extern void SwapGetImageReply();

extern void WriteSConnectionInfo();

#endif				/* _SWAP_H_ */
