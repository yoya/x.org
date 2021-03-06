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
 * $SonyRCSfile: cnvhinsi.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:33 $
 */


#include <stdio.h>
#include <sys/types.h>



struct gram_code {
	char	*name;
	int	code;
} gramtbl[] = {


#include "GramTable"

};

#define	GramMax	(sizeof(gramtbl)/sizeof(struct gram_code) - 1)



int	cnvhinsi(buf)
u_char	*buf;
{
	register int	min;
	register int	max;
	register int	mid;
	register int	i;

	min = 0;
	max = GramMax;
	while (min <= max) {
		
		mid = (min + max) / 2;

		
		i = strcmp(buf, gramtbl[mid].name);

		
		if (i < 0) {
			max = mid - 1;
		}
		
		else if (i > 0) {
			min = mid + 1;
		}
		
		else {
			return(gramtbl[mid].code);
		}
	}

	
	return 0;
}



char	*hinsi_str(code)
int	code;
{
	int	i;

	for (i = 0 ; i <= GramMax ; i++) {
		if (gramtbl[i].code == code) return gramtbl[i].name;
	}

	return "Err";
}
