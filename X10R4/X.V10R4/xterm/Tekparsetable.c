/*
 *	$Source: /u1/X/xterm/RCS/Tekparsetable.c,v $
 *	$Header: Tekparsetable.c,v 10.101 86/12/01 21:50:13 jg Rel $
 */

#ifndef lint
static char *rcsid_Tekparsetable_c = "$Header: Tekparsetable.c,v 10.101 86/12/01 21:50:13 jg Rel $";
#endif	lint

#include "Tekparse.h"
#ifndef lint
static char sccs_id[] = "@(#)Tekparsetable.c\tX10/6.6\t11/7/86";
#endif	lint

int Talptable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_ESC_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_SP,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	$		%		&		'	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	(		)		*		+	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	,		-		.		/	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	0		1		2		3	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	4		5		6		7	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	8		9		:		;	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	<		=		>		?	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	@		A		B		C	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	D		E		F		G	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	H		I		J		K	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	L		M		N		O	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	P		Q		R		S	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	T		U		V		W	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	X		Y		Z		[	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	\		]		^		_	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	`		a		b		c	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	d		e		f		g	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	h		i		j		k	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	l		m		n		o	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	p		q		r		s	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	t		u		v		w	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	x		y		z		{	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
/*	|		}		~		DEL	*/
CASE_PRINT,
CASE_PRINT,
CASE_PRINT,
CASE_IGNORE,
};

int Tbestable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_VT_MODE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_BYP_STATE,
CASE_REPORT,
CASE_BYP_STATE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_IGNORE,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_PAGE,
CASE_IGNORE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	DLE		DC1		DC2		DC3	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	DC4		NAK		SYN		ETB	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_COPY,
/*	CAN		EM		SUB		ESC	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_GIN,
CASE_IGNORE,
/*	FS		GS		RS		US	*/
CASE_SPT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	$		%		&		'	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	(		)		*		+	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	,		-		.		/	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	0		1		2		3	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	4		5		6		7	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	8		9		:		;	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	<		=		>		?	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	@		A		B		C	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	D		E		F		G	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	H		I		J		K	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	L		M		N		O	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	P		Q		R		S	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	T		U		V		W	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	X		Y		Z		[	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	\		]		^		_	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	`		a		b		c	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	d		e		f		g	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	h		i		j		k	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	l		m		n		o	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	p		q		r		s	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	t		u		v		w	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	x		y		z		{	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_BYP_STATE,
/*	|		}		~		DEL	*/
CASE_BYP_STATE,
CASE_BYP_STATE,
CASE_IGNORE,
CASE_BYP_STATE,
};

int Tbyptable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BES_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	$		%		&		'	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	(		)		*		+	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	,		-		.		/	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	0		1		2		3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	4		5		6		7	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	8		9		:		;	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	<		=		>		?	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	@		A		B		C	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	D		E		F		G	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	H		I		J		K	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	L		M		N		O	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	P		Q		R		S	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	T		U		V		W	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	X		Y		Z		[	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	\		]		^		_	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	`		a		b		c	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	d		e		f		g	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	h		i		j		k	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	l		m		n		o	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	p		q		r		s	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	t		u		v		w	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	x		y		z		{	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	|		}		~		DEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
};

int Tesctable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_VT_MODE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_CURSTATE,
CASE_REPORT,
CASE_CURSTATE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_IGNORE,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_PAGE,
CASE_IGNORE,
CASE_APL,
CASE_ASCII,
/*	DLE		DC1		DC2		DC3	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	DC4		NAK		SYN		ETB	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_COPY,
/*	CAN		EM		SUB		ESC	*/
CASE_BYP_STATE,
CASE_CURSTATE,
CASE_GIN,
CASE_IGNORE,
/*	FS		GS		RS		US	*/
CASE_SPT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	$		%		&		'	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	(		)		*		+	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	,		-		.		/	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	0		1		2		3	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	4		5		6		7	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	8		9		:		;	*/
CASE_CHAR_SIZE,
CASE_CHAR_SIZE,
CASE_CHAR_SIZE,
CASE_CHAR_SIZE,
/*	<		=		>		?	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	@		A		B		C	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	D		E		F		G	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	H		I		J		K	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	L		M		N		O	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	P		Q		R		S	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	T		U		V		W	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	X		Y		Z		[	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	\		]		^		_	*/
CASE_CURSTATE,
CASE_OSC,
CASE_CURSTATE,
CASE_CURSTATE,
/*	`		a		b		c	*/
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
/*	d		e		f		g	*/
CASE_BEAM_VEC,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_BEAM_VEC,
/*	h		i		j		k	*/
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
/*	l		m		n		o	*/
CASE_BEAM_VEC,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_BEAM_VEC,
/*	p		q		r		s	*/
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
CASE_BEAM_VEC,
/*	t		u		v		w	*/
CASE_BEAM_VEC,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_BEAM_VEC,
/*	x		y		z		{	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
CASE_CURSTATE,
/*	|		}		~		DEL	*/
CASE_CURSTATE,
CASE_CURSTATE,
CASE_IGNORE,
CASE_CURSTATE,
};

int Tipltable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_ESC_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_PENUP,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	$		%		&		'	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	(		)		*		+	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	,		-		.		/	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	0		1		2		3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	4		5		6		7	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	8		9		:		;	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	<		=		>		?	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	@		A		B		C	*/
CASE_IGNORE,
CASE_IPL_POINT,
CASE_IPL_POINT,
CASE_IGNORE,
/*	D		E		F		G	*/
CASE_IPL_POINT,
CASE_IPL_POINT,
CASE_IPL_POINT,
CASE_IGNORE,
/*	H		I		J		K	*/
CASE_IPL_POINT,
CASE_IPL_POINT,
CASE_IPL_POINT,
CASE_IGNORE,
/*	L		M		N		O	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	P		Q		R		S	*/
CASE_PENDOWN,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	T		U		V		W	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	X		Y		Z		[	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	\		]		^		_	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	`		a		b		c	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	d		e		f		g	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	h		i		j		k	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	l		m		n		o	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	p		q		r		s	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	t		u		v		w	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	x		y		z		{	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	|		}		~		DEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
};

int Tplttable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_ESC_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	$		%		&		'	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	(		)		*		+	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	,		-		.		/	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	0		1		2		3	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	4		5		6		7	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	8		9		:		;	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	<		=		>		?	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	@		A		B		C	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	D		E		F		G	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	H		I		J		K	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	L		M		N		O	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	P		Q		R		S	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	T		U		V		W	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	X		Y		Z		[	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	\		]		^		_	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	`		a		b		c	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	d		e		f		g	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	h		i		j		k	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	l		m		n		o	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	p		q		r		s	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	t		u		v		w	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	x		y		z		{	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
/*	|		}		~		DEL	*/
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
CASE_PLT_VEC,
};

int Tpttable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_ESC_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	$		%		&		'	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	(		)		*		+	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	,		-		.		/	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	0		1		2		3	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	4		5		6		7	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	8		9		:		;	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	<		=		>		?	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	@		A		B		C	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	D		E		F		G	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	H		I		J		K	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	L		M		N		O	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	P		Q		R		S	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	T		U		V		W	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	X		Y		Z		[	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	\		]		^		_	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	`		a		b		c	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	d		e		f		g	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	h		i		j		k	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	l		m		n		o	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	p		q		r		s	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	t		u		v		w	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	x		y		z		{	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
/*	|		}		~		DEL	*/
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
CASE_PT_POINT,
};

int Tspttable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BEL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_LF,
CASE_UP,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_CR,
CASE_IGNORE,
CASE_IGNORE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_ESC_STATE,
/*	FS		GS		RS		US	*/
CASE_PT_STATE,
CASE_PLT_STATE,
CASE_IPL_STATE,
CASE_ALP_STATE,
/*	SP		!		"		#	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	$		%		&		'	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	(		)		*		+	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	,		-		.		/	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	0		1		2		3	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	4		5		6		7	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	8		9		:		;	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	<		=		>		?	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	@		A		B		C	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	D		E		F		G	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	H		I		J		K	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	L		M		N		O	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	P		Q		R		S	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	T		U		V		W	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	X		Y		Z		[	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	\		]		^		_	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	`		a		b		c	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	d		e		f		g	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	h		i		j		k	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	l		m		n		o	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	p		q		r		s	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	t		u		v		w	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	x		y		z		{	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
/*	|		}		~		DEL	*/
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
CASE_SPT_POINT,
};
