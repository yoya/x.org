/*
 * Copyright 1993 by Digital Equipment Corporation, Maynard, Massachusetts.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of Digital not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  Digital makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Alibint.h"

int AFQueryInputGain(AC ac, int *min_input_gain, int *max_input_gain)
{
    register AFAudioConn *aud = ac->connection; 
    register aQueryInputGainReq *req;
    aQueryGainReply reply;

    LockConnection(aud);
    GetReq(QueryInputGain, req);
    req->ac = ac->acontext;
    _AReply(aud, (aReply *) &reply, 0, aTrue);
    UnlockConnection(aud);
    SyncHandle();
    *min_input_gain = reply.minGain;
    *max_input_gain = reply.maxGain;
    return(reply.gain);
}

int AFQueryOutputGain(AC ac, int *min_output_gain, int *max_output_gain)
{
    register AFAudioConn *aud = ac->connection; 
    register aQueryOutputGainReq *req;
    aQueryGainReply reply;

    LockConnection(aud);
    GetReq(QueryOutputGain, req);
    req->ac = ac->acontext;
    _AReply(aud, (aReply *) &reply, 0, aTrue);
    UnlockConnection(aud);
    SyncHandle();
    *min_output_gain = reply.minGain;
    *max_output_gain = reply.maxGain;
    return(reply.gain);
}
