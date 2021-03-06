#ifndef _CODEC_H_
#define _CODEC_H_
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

#include "audio.h"

/* 
 * Definitions from the Rev E Specification for the AMD79C30A CODEC.
 * tml	Thu Sep 20 10:56:10 EDT 1990
 *	some defs stolen from smorg.
 */

/* Codec related definitions */

#define	CODEC_EAR	0
#define	CODEC_SPEAKER	1
#define	CODEC_BOTH	2

/* Indirect Register Addresses, format BLOCK_{SEQ}_REG{_to_REG}, pg. 14	*/
#define	INIT_1			0x21
#define	INIT_2			0x20

#define	LIU_LSR			0xa1
#define	LIU_LPR			0xa2
#define	LIU_LMR1		0xa3
#define	LIU_LMR2		0xa4
#define	LIU_SEQ_LPR_to_LMR2	0xa5
#define	LIU_MF			0xa6
#define	LIU_MFSB		0xa7
#define	LIU_MFQB		0xa8

#define	MUX_MCR1		0x41
#define	MUX_MCR2		0x42
#define	MUX_MCR3		0x43
#define	MUX_MCR4		0x44
#define	MUX_SEQ_MCR1_to_MCR4	0x45

#define	MAP_X			0x61
#define	MAP_R			0x62
#define	MAP_GX			0x63
#define	MAP_GR			0x64
#define	MAP_GER			0x65
#define	MAP_STG			0x66
#define	MAP_SEQ_FTGR1_to_2	0x67
#define	MAP_SEQ_ATGR1_to_2	0x68
#define	MAP_MMR1		0x69
#define	MAP_MMR2		0x6a
#define	MAP_SEQ_X_to_MMR2	0x6b
#define	MAP_MMR3		0x6c
#define	MAP_STRA		0x6d
#define	MAP_STRF		0x6e

#define	DLC_SEQ_FRAR1_to_3	0x81
#define	DLC_SEQ_SRAR1_to_3	0x82
#define	DLC_TAR			0x83
#define	DLC_DRLR		0x84
#define	DLC_DTCR		0x85
#define	DLC_DMR1		0x86
#define	DLC_DMR2		0x87
#define	DLC_SEQ_FRAR1_to_DMR2	0x88
#define	DLC_DRCR		0x89
#define	DLC_RNGR1		0x8a
#define	DLC_RNGR2		0x8b
#define	DLC_FRAR4		0x8c
#define	DLC_SRAR4		0x8d
#define	DLC_DMR3		0x8e
#define	DLC_DMR4		0x8f
#define	DLC_SEQ_FRAR4_to_DMR4	0x90
#define	DLC_ASR			0x91
#define	DLC_EFCR		0x92

#define	PP_PPCR1		0xc0
#define	PP_PPSR			0xc1
#define	PP_PPIER		0xc2
#define	PP_MTDR			0xc3
#define	PP_MRDR			0xc3
#define	PP_CITDR0		0xc4
#define	PP_CIRDR0		0xc4
#define	PP_CITDR1		0xc5
#define	PP_CIRDR1		0xc5
#define	PP_PPCR2		0xc8

/* constants for individual registers */

	/* INIT register */
/* Power Mode Selection */
#define INIT_PM_MASK	(0x3)
#define INIT_PM_IDLE	(0x0)
#define INIT_PM_ACTIVE_VOICE_DATA	(0x1)
#define	INIT_PM_ACTIVE_DATA	(0x2)

/* Interrupt Selection */
#define INIT_IR_MASK	(0x4)
#define	INIT_IR_ENABLE	(0x0)
#define INIT_IR_DISABLE	(0x4)

/* Clock Divider Selection */
#define INIT_CDS_MASK	(0x38)
#define INIT_CDS_BY2	(0x00)
#define INIT_CDS_BY1	(0x08)
#define	INIT_CDS_BY4	(0x10)

#define INIT_CDS_BY3	(0x20)

/* Abort Selection */
#define INIT_AS_MASK	(0xc0)
#define INIT_AS_RA	(0x40)
#define INIT_AS_NRA	(0x00)
#define INIT_AS_TA	(0x80)
#define INIT_AS_NTA	(0x00)

/* Interrupt Register */

/* Microprocessor Interface */


/* MUX constants */
#define MUX_PORT_NONE	(0x0)
#define MUX_PORT_B1	(0x01)
#define MUX_PORT_B2	(0x02)
#define MUX_PORT_BA	(0x03)
#define MUX_PORT_BB	(0x04)
#define MUX_PORT_BC	(0x05)
#define MUX_PORT_BD	(0x06)
#define MUX_PORT_BE	(0x07)
#define MUX_PORT_BF	(0x08)
#define MUX_MCR4_ENABLE	(0x08)
#define MUX_MCR4_BB_REVERSE	(0x10)
#define MUX_MCR4_BC_REVERSE	(0x20)


/* MAP MMR constants */
#define MAP_MMR1_BITS_A-law	(0x1)
#define MAP_MMR1_BITS_GX	(0x2)
#define MAP_MMR1_BITS_GR	(0x4)
#define MAP_MMR1_BITS_GER	(0x8)
#define MAP_MMR1_BITS_X		(0x10)
#define MAP_MMR1_BITS_R		(0x20)
#define MAP_MMR1_BITS_STG	(0x40)
#define MAP_MMR1_BITS_DLOOP	(0x80)


#define MAP_MMR2_BITS_AINB	(0x1)
#define MAP_MMR2_BITS_LS	(0x2)
#define	MAP_MMR2_BITS_DTMF	(0x4)
#define MAP_MMR2_BITS_TGEN	(0x8)
#define MAP_MMR2_BITS_TRING	(0x10)
#define MAP_MMR2_BITS_HIGHPASS	(0x20)
#define MAP_MMR2_BITS_ADC	(0x40)

#define	MAP_MMR3_BITS_BOTH	(0x02)
#define	MAP_MMR3_BITS_MBZ	(0x01)
#define	MAP_MMR3_BITS_GA	(0x70)
#define	MAP_MMR3_BITS_GA0	(0x00)
#define	MAP_MMR3_BITS_GA6	(0x10)
#define	MAP_MMR3_BITS_GA12	(0x20)
#define	MAP_MMR3_BITS_GA18	(0x30)
#define	MAP_MMR3_BITS_GA24	(0x40)
#define	MAP_MMR3_BITS_MUTE	(0x08)
#define	MAP_MMR3_BITS_STR	(0x01)

#define         MIN_GAIN         0.0
#define         MAXGA_GAIN      24.0
#define         MAXGX_GAIN      12.0

#define         MIN_OUTPUT_GAIN   0.0
#define         MAX_OUTPUT_GAIN  41.0
#define         MINGR_GAIN      -12.0
#define         MAXGR_GAIN        0.0
#define         DEFGR_GAIN        0.0
#define         MINGER_GAIN     -10.0
#define         MAXGER_GAIN     +18.0
#define         DEFGER_GAIN      18.0
#define         GR_SCALE         12.0
#define         GER_SCALE        23.0


/* Macros */

#define	PCODEC(reg)  reg->us_reg


/* Tables */

#ifdef __cplusplus
extern "C" {
#endif

float Mcodec_set_ger();
float Mcodec_get_ger();
int Mcodec_get_output();
void Mcodec_set_output();
void Mcodec_set_input();
int Mcodec_get_input();

void Mcodec_init();
ATime bba_get_time();

void Mcodec_stop();
void Mcodec_output();
void Mcodec_input();
void Mcodec_dmw();
void Mcodec_tone();
void Mcodec_sringer(); /* str */
void Mcodec_ftg();
void Mcodec_dtmf_ftg();
void Mcodec_atg();
double Mcodec_stg();
void Mcodec_strg();
void Mcodec_strf();
void Mcodec_ger();
void Mcodec_gr();
double Mcodec_gx();
void Mcodec_mute();
double Mcodec_preamp();
void Mcodec_indirect();

void Mmsleep();
#ifdef __cplusplus
}
#endif

#endif
