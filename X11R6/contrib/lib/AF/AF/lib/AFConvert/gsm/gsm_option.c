/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /crl/audio/AF/lib/AFConvert/gsm/RCS/gsm_option.c,v 1.1 1994/02/01 16:43:15 stewart Exp $ */

#include "private.h"

#include "gsm.h"
#include "proto.h"

int gsm_option P3((r, opt, val), gsm r, int opt, int * val)
{
	int 	result = -1;

	switch (opt) {
	case GSM_OPT_VERBOSE:
#ifndef	NDEBUG
		result = r->verbose;
		if (val) r->verbose = *val;
#endif
		break;

	case GSM_OPT_FAST:

#if	defined(FAST) && defined(USE_FLOAT_MUL)
		result = r->fast;
		if (val) r->fast = !!*val;
#endif
		break;

	default:
		break;
	}
	return result;
}
