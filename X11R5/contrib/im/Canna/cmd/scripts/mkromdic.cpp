
/**/#!/bin/sh
/**/# Copyright 1991 NEC Corporation, Tokyo, Japan.
/**/#
/**/# Permission to use, copy, modify, and distribute this software and its
/**/# documentation for any purpose and without fee is hereby granted,
/**/# provided that the above copyright notice appear in all copies and that
/**/# both that copyright notice and this permission notice appear in
/**/# supporting documentation, and that the name of NEC Corporation
/**/# not be used in advertising or publicity pertaining to distribution
/**/# of the software without specific, written prior permission.  NEC 
/**/# Corporation makes no representations about the suitability of this
/**/# software for any purpose.  It is provided "as is" without express
/**/# or implied warranty.
/**/#
/**/# NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
/**/# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
/**/# NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
/**/# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
/**/# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
/**/# OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
/**/# PERFORMANCE OF THIS SOFTWARE. 

PATH=$PATH:/bin:/usr/bin:/etc:/usr/etc:/usr/nec/bin:/usr/ucb
export PATH

if [ $# -eq 0 ]; then
  echo "usage: mkromdic [...options] <file name>"
  exit 1
fi

N=`expr $# - 1`

while [ $N -gt 0 ]
do
	args="$args $1"
	shift
	N=`expr $N - 1`
done

if [ ! -r $1 ]; then
  echo "mkbindic: cannot open $1"
  exit 1
fi			

echo "forcpp -7 < $1 | /lib/cpp $args |forcpp -8 | crrdic > `basename $1 '\..*'`.rdic"

forcpp -7 < $1 | /lib/cpp $args | forcpp -8 | crrdic > `basename $1 '\..*'`.rdic
