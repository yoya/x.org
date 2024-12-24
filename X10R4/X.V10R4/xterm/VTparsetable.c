/*
 *	$Source: /u1/X/xterm/RCS/VTparsetable.c,v $
 *	$Header: VTparsetable.c,v 10.101 86/12/01 21:49:52 jg Rel $
 */

#ifndef lint
static char *rcsid_VTparsetable_c = "$Header: VTparsetable.c,v 10.101 86/12/01 21:49:52 jg Rel $";
#endif	lint

#include "VTparse.h"
#ifndef lint
static char sccs_id[] = "@(#)VTparsetable.c\tX10/6.6\t11/6/86";
#endif	lint
/*
 * Stupid Apollo C preprocessor can't handle long lines.  So... To keep
 * it happy, we put each onto a seperate line....  Sigh...
 */

int groundtable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_PRINT,
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
CASE_GROUND_STATE, };  

int csitable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	$		%		&		'	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	(		)		*		+	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	,		-		.		/	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	0		1		2		3	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
/*	4		5		6		7	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
/*	8		9		:		;	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_IGNORE,
CASE_ESC_SEMI,
/*	<		=		>		?	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_DEC_STATE,
/*	@		A		B		C	*/
CASE_ICH,
CASE_CUU,
CASE_CUD,
CASE_CUF,
/*	D		E		F		G	*/
CASE_CUB,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_CUP,
CASE_GROUND_STATE,
CASE_ED,
CASE_EL,
/*	L		M		N		O	*/
CASE_IL,
CASE_DL,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	P		Q		R		S	*/
CASE_DCH,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	T		U		V		W	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_DA1,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_CUP,
CASE_TBC,
/*	h		i		j		k	*/
CASE_SET,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_RST,
CASE_SGR,
CASE_CPR,
CASE_GROUND_STATE,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_DECSTBM,
CASE_GROUND_STATE,
/*	t		u		v		w	*/
CASE_SUN_EMU,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_DECREQTPARM,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
};

int dectable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	$		%		&		'	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	(		)		*		+	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	,		-		.		/	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	0		1		2		3	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
/*	4		5		6		7	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
/*	8		9		:		;	*/
CASE_ESC_DIGIT,
CASE_ESC_DIGIT,
CASE_IGNORE,
CASE_ESC_SEMI,
/*	<		=		>		?	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	@		A		B		C	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	D		E		F		G	*/
CASE_GROUND_STATE,
CASE_ERASE_STATUS,
CASE_FROM_STATUS,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_HIDE_STATUS,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	L		M		N		O	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	P		Q		R		S	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_SHOW_STATUS,
/*	T		U		V		W	*/
CASE_TO_STATUS,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	h		i		j		k	*/
CASE_DECSET,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_DECRST,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_XTERM_RESTORE,
CASE_XTERM_SAVE,
/*	t		u		v		w	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
};

int eigtable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
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
/*	,
	-		.		/	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	0		1		2		3	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	4		5		6		7	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	8		9		:		;	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	<		=		>		?	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	@		A		B		C	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	D		E		F		G	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	L		M		N		O	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	P		Q		R		S	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	T		U		V		W	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	h		i		j		k	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	t		u		v		w	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
};

int esctable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_SCR_STATE,
/*	$		%		&		'	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	(		)		*		+	*/
CASE_SCS0_STATE,
CASE_SCS1_STATE,
CASE_SCS2_STATE,
CASE_SCS3_STATE,
/*	,		-		.		/	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	0		1		2		3	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	4		5		6		7	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_DECSC,
/*	8		9		:		;	*/
CASE_DECRC,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	<		=		>		?	*/
CASE_GROUND_STATE,
CASE_DECKPAM,
CASE_DECKPNM,
CASE_GROUND_STATE,
/*	@		A		B		C	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	D		E		F		G	*/
CASE_IND,
CASE_NEL,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_HTS,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	L		M		N		O	*/
CASE_GROUND_STATE,
CASE_RI,
CASE_SS2,
CASE_SS3,
/*	P		Q		R		S	*/
CASE_IGNORE_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	T		U		V		W	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_CSI_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_OSC,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_RIS,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	h		i		j		k	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_LS2,
CASE_LS3,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	t		u		v		w	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_LS3R,
CASE_LS2R,
CASE_LS1R,
CASE_GROUND_STATE,
};

int iestable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	BS		HT		NL		VT	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	NP		CR		SO		SI	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	DLE		DC1		DC2		DC3	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	DC4		NAK		SYN		ETB	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	CAN		EM		SUB		ESC	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	FS		GS		RS		US	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	SP		!		"		#	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	$		%		&		'	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	(		)		*		+	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	,		-		.		/	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	0		1		2		3	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	4		5		6		7	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	8		9		:		;	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	<		=		>		?	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	@		A		B		C	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	D		E		F		G	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	H		I		J		K	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	L		M		N		O	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	P		Q		R		S	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	T		U		V		W	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	X		Y		Z		[	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	`		a		b		c	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	d		e		f		g	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	h		i		j		k	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	l		m		n		o	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	p		q		r		s	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	t		u		v		w	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	x		y		z		{	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
/*	|		}		~		DEL	*/
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
CASE_IGNORE_STATE,
};

int igntable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	BS		HT		NL		VT	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	NP		CR		SO		SI	*/
CASE_IGNORE,
CASE_IGNORE,
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
CASE_GROUND_STATE, 
CASE_IGNORE,
CASE_GROUND_STATE,
CASE_IGNORE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
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

int scrtable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	$		%		&		'	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	(		)		*		+	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	,		-		.		/	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	0		1		2		3	*/
CASE_GROUND_STATE,
CASE_HIDDEN,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	4		5		6		7	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	8		9		:		;	*/
CASE_DECALN,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	<		=		>		?	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	@		A		B		C	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	D		E		F		G	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	L		M		N		O	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	P		Q		R		S	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	T		U		V		W	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	h		i		j		k	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	t		u		v		w	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
};

int scstable[] = {
/*	NUL		SOH		STX		ETX	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	EOT		ENQ		ACK		BEL	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_BELL,
/*	BS		HT		NL		VT	*/
CASE_BS,
CASE_TAB,
CASE_VMOT,
CASE_VMOT,
/*	NP		CR		SO		SI	*/
CASE_VMOT,
CASE_CR,
CASE_SO,
CASE_SI,
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
CASE_ESC,
/*	FS		GS		RS		US	*/
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
CASE_IGNORE,
/*	SP		!		"		#	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	$		%		&		'	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	(		)		*		+	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	,		-		.		/	*/
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
CASE_ESC_IGNORE,
/*	0		1		2		3	*/
CASE_GSETS,
CASE_GSETS,
CASE_GSETS,
CASE_GROUND_STATE,
/*	4		5		6		7	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	8		9		:		;	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	<		=		>		?	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	@		A		B		C	*/
CASE_GROUND_STATE,
CASE_GSETS,
CASE_GSETS,
CASE_GROUND_STATE,
/*	D		E		F		G	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	H		I		J		K	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	L		M		N		O	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	P		Q		R		S	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	T		U		V		W	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	X		Y		Z		[	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	\		]		^		_	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	`		a		b		c	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	d		e		f		g	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	h		i		j		k	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	l		m		n		o	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	p		q		r		s	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	t		u		v		w	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	x		y		z		{	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
/*	|		}		~		DEL	*/
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
CASE_GROUND_STATE,
};
