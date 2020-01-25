/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: Const.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:54 $
 */



#define	HostNameLen		128			
#define	UserNameLen		128
#define	ProgNameLen		128
#define	PathNameLen		1024			
#define	PassWordLen		16

#define	YomiBufSize		1024
#define	KanjiBufSize		2048

#define	MinIdxLen		512
#define	MinSegLen		512
#define	MinSegNum		1
#define	MinStyNum		512
#define	MaxStyNum		0x7fff
#define	MinClStep		1
#define	MaxClStep		256
#define	MinClLen		512
#define	MaxClLen		0x7fff

#define	RunCmdFile		"/usr/lib/sj3/serverrc"
#define	DebugOutFile		NULL	
#define	DebugLevel		0
#define	ForkFlag		1
#define	MaxClientNum		64
#define	ErrorOutFile		"/dev/console"
#define	DictRootDir		"/usr/dict/sj3"
#define	LogOutFile		NULL	
#define	PortName		"sj3"
#ifdef TLI
#define LocalHost               "localhost"
#define ProtoName               "tcp"
#define	PortNumber		"3000"
#else
#define	PortNumber		3000
#endif
#define	SocketName		"/tmp/sj3sock"
#define	LockFile		"/tmp/SJ3SERV.EXIST"
#define	DirectryMode		0755
#define	DictFileMode		0644
#define	MODE_MASK		0777

#define	MainDictionary		"sj3main.dic"
#define	UserDictionary		"private.dic"
#define	StudyFile		"study.dat"
#define	DefIdxLen		2048
#define	DefSegLen		2048
#define	DefSegNum		256
#define	DefStyNum		2048
#define	DefClStep		1
#define	DefClLen		2048

#define	NotLocked		-1

#define	ERROR			-1

#ifndef	FALSE
#	define	TRUE		-1
#	define	FALSE		(!TRUE)
#endif

