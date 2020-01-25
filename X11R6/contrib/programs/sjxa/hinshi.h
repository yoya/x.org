/* $SonyId: hinshi.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#ifndef	_Hinshi_H
#define	_Hinshi_H

#define	MEISI_1		1  /* ¾��ʬ��˴ޤޤ�ʤ���ʣ��̾�����䤹��̾�� */
#define	MEISI_2		2  /* ��Ƭ�� "��"			*/
#define	MEISI_3		3  /* ��Ƭ�� "��"			*/
#define	MEISI_4		4  /* ������ "Ū" "��"			*/
#define	MEISI_5		5  /* ̾��ܥ���ư�졢��Ƭ�� "��"	*/
#define	MEISI_6		6  /* ̾��ܥ���ư��			*/
#define	MEISI_7		7  /* ̾��ܥ���ư�졢��Ƭ�� "��"	*/
#define	MEISI_8		8  /* ̾��ܷ���ư��			*/
#define	MEISI_9		9  /* ̾��ܷ���ư�졢��Ƭ�� "��"	*/
#define	MEISI_10	10 /* ̾��ܷ���ư�졢��Ƭ�� "��"	*/
#define MEISI_11	11 /* ̾�������			*/
#define D_MEISI_1	12 /* ¾��ʬ��˴ޤޤ�ʤ���ʣ��̾�����ˤ���̾�� */
#define D_MEISI_2	13 /* ������ "����"			*/
#define D_MEISI_3	14 /* "���򤹤�"			*/
#define D_MEISI_4	15 /* ̾��ܥ���ư��			*/
#define D_MEISI_5	16 /* ̾��ܷ���ư��			*/
#define D_MEISI_6	17 /* ̾�������			*/
#define	MEISI_20	18 /* ��Ƭ��ϤҤ餬�ʤΤ�		*/
/* 19 20 �� RESERVE */
#define MYOUJI		21 /* �Ļ�				*/
#define NAMAE		22 /* ̾��				*/
#define KIGYOU		23 /* ���̾				*/
#define TIMEI		24 /* ��ƻ�ܸ����跴����Į¼ ����Ӽ����ԻԤα�̾ */
#define KEN_KU		25 /* ��ƻ�ܸ�����Ρ�����(��,��,��)�դ� */
#define RENTAISI	26 /* Ϣ�λ�				*/
#define SETUZOKU	27 /* ��³��				*/
#define KANDOUSI	28 /* ��ư��				*/
#define JOSUUSI1	29 /* "���" ����³�Ǥ��������		*/
#define SUUSI		30 /* ����				*/
#define SETTOU_1	31 /* ̾��(����̾�졦��ư̾��ޤ�)����³ */
#define SETTOU_2	32 /* ��̾����³(�̡�������� ��)	*/
#define SETTOU_3	33 /* ��̾����³(�� �ʤ�)		*/
#define SETTOU_4	34 /* �������³(�衢�� ��)		*/
#define SETTOU_5	35 /* ��Ƭ�������³������		*/
#define SETUBI_1	36 /* ̾�����³��̾�찷��		*/
#define SETUBI_2	37 /* ̾�����³��̾�찷����������Ϣ���� */
#define SETUBI_3	38 /* �����������³������		*/
#define SETUBI_4	39 /* ��̾����³��̾�찷��		*/
#define SETUBI_5	40 /* ̾�����³�����ƻ찷��		*/
#define SETUBI_6	41 /* ̾�����³������ư�찷��		*/
#define SETUBI_7	42 /* ̾�����³������̾�찷��		*/
#define SETUBI_8	43 /* ̾�����³������ư�찷��		*/
#define SETUBI_9	44 /* �����������			*/
#define FUKUSI_1	45 /* ��³�ʤ�				*/
#define FUKUSI_2	46 /* "����" ��³			*/
#define FUKUSI_3	47 /* "����"������ư���������³	*/
#define FUKUSI_4	48 /* ����ư���������³		*/
#define FUKUSI_5	49 /* "����" ��³			*/
#define FUKUSI_6	50 /* "����" ��³			*/
#define FUKUSI_7	51 /* "����" ��³			*/
#define FUKUSI_8	52 /* "����"��"���Ǥ�������" ��³	*/
#define FUKUSI_9	53 /* "����"������ư�������"���Ǥ�������" ��³ */
#define JOSUUSI2	54 /* "���"����³�Ǥ��ʤ�������	*/
/* 55 - 59 �� RESERVE */

/* [��]		"��" ����³����̾�첽����	*/
/* [��]		"��" ����³���Ʒ���ư�첽����	*/
/* [��]		"��" ����³���Ʒ���ư�첽����	*/
/* [��]		"��" ����³����̾�첽����	*/
/* [����]	"����" ����³����ư�첽����	*/
#define KEIYOUSI_1	60 /* �嵭��°����³�ʤ�		*/
#define KEIYOUSI_2	61 /* [��]				*/
#define KEIYOUSI_3	62 /* [������]				*/
#define KEIYOUSI_4	63 /* [��������]			*/
#define KEIYOUSI_5	64 /* [������������]			*/
#define KEIYOUSI_6	65 /* [�����ᡢ��]			*/
#define KEIYOUSI_7	66 /* [�����ᡢ��������]		*/
#define KEIYOUSI_8	67 /* [�����ߡ��ᡢ��������]		*/
#define KEIYOUSI_9	68 /* ��Ƭ�� "��", [������������]	*/
#define KEIYOUSI_10	69 /* ��Ƭ�� "��", [�����ᡢ��������]	*/
#define KEIYOUSI_11	70 /* [�����ߡ���]			*/
#define KE_DOUSI_1	71 /* ������				*/
#define KE_DOUSI_2	72 /* �������				*/
#define KE_DOUSI_3	73 /* �����ѡ��촴��Ϣ�ν���		*/
#define KE_DOUSI_4	74 /* �����ѡ�"����"			*/
#define KE_DOUSI_5	75 /* �����ѡ�"����"���촴��Ϣ�ν���	*/
#define KE_DOUSI_6	76 /* �����ѡ���Ƭ�� "��" ����³	*/
#define KE_DOUSI_7	77 /* �����ѡ�"����"����Ƭ�� "��" ����³*/
#define KE_DOUSI_8	78 /* �����ѡ���Ƭ�� "��" ����³	*/
#define KE_DOUSI_9	79 /* �����ѡ�"����"			*/

#define DO_SAHEN	80 /* ����ư�졢Ϣ��̾�찷���Ǥ��ʤ�	*/
#define DO_ZAHEN	81 /* ����ư�졣Ϣ��̾�찷���Ǥ��ʤ�	*/
/* 82 - 89 �� RESERVE 	*/

/* Ϣ��̾�찷���Ǥ��ʤ�����Ƭ�� "��" */
#define DO_1DAN_1	90 /* ư������	*/
#define DO_KAGO_1	91 /* ư������	*/
#define DO_GAGO_1	92 /* ư������	*/
#define	DO_SAGO_1	93 /* ư������	*/
#define	DO_TAGO_1	94 /* ư������	*/
#define	DO_NAGO_1	95 /* ư���ʸ�	*/
#define	DO_BAGO_1	96 /* ư���и�	*/
#define	DO_MAGO_1	97 /* ư���޸�	*/
#define	DO_RAGO_1	98 /* ư�����	*/
#define	DO_WAGO_1	99 /* ư�����	*/

/* Ϣ��̾�찷���Ǥ��ʤ�����Ƭ�� "��" */
#define DO_1DAN_2	100 /* ư������ */
#define DO_KAGO_2	101 /* ư������ */
#define DO_GAGO_2	102 /* ư������ */
#define	DO_SAGO_2	103 /* ư������ */
#define	DO_TAGO_2	104 /* ư������ */
#define	DO_BAGO_2	105 /* ư���и� */
#define	DO_MAGO_2	106 /* ư���޸� */
#define	DO_RAGO_2	107 /* ư����� */
#define	DO_WAGO_2	108 /* ư����� */
/* 109, 110 �� RESERVE	*/

/* Ϣ��̾�찷���Ǥ��ʤ���[�������롢����] */
#define DO_KAGO_5	111 /* ư������ */
#define DO_GAGO_5	112 /* ư������ */
#define	DO_SAGO_5	113 /* ư������ */
#define	DO_TAGO_5	114 /* ư������ */
#define	DO_BAGO_5	115 /* ư���и� */
#define	DO_MAGO_5	116 /* ư���޸� */
#define	DO_RAGO_5	117 /* ư����� */
#define	DO_WAGO_5	118 /* ư����� */
/* 119, 120 �� RESERVE	*/

/* Ϣ��̾�찷���Ǥ��ʤ���[�������롢����]����Ƭ�� "��" */
#define DO_KAGO_6	121 /* ư������ */
#define DO_GAGO_6	122 /* ư������ */
#define	DO_SAGO_6	123 /* ư������ */
#define	DO_TAGO_6	124 /* ư������ */
#define	DO_BAGO_6	125 /* ư���и� */
#define	DO_MAGO_6	126 /* ư���޸� */
#define	DO_RAGO_6	127 /* ư����� */
#define	DO_WAGO_6	128 /* ư����� */
/* 129 �� RESERVE */

/* Ϣ��̾�찷���Ǥ��� */
#define DO_1DAN_3	130 /* ư������ */
#define DO_KAGO_3	131 /* ư������ */
#define DO_GAGO_3	132 /* ư������ */
#define	DO_SAGO_3	133 /* ư������ */
#define	DO_TAGO_3	134 /* ư������ */
#define	DO_BAGO_3	135 /* ư���и� */
#define	DO_MAGO_3	136 /* ư���޸� */
#define	DO_RAGO_3	137 /* ư����� */
#define	DO_WAGO_3	138 /* ư����� */
/* 139 �� RESERVE */

/* Ϣ��̾�찷���Ǥ��롢��Ƭ�� "��" */
#define DO_1DAN_4	140 /* ư�����ʣ� */
#define DO_KAGO_4	141 /* ư�����ޣ� */
#define DO_GAGO_4	142 /* ư�����ޣ� */
#define	DO_SAGO_4	143 /* ư�����ޣ� */
#define	DO_TAGO_4	144 /* ư�����ޣ� */
#define	DO_BAGO_4	145 /* ư���иޣ� */
#define	DO_MAGO_4	146 /* ư���޸ޣ� */
#define	DO_RAGO_4	147 /* ư����ޣ� */
#define	DO_WAGO_4	148 /* ư����ޣ� */
/* 149, 150 �� RESERVE	*/

/* Ϣ��̾�찷���Ǥ��롢[���� ���롢����] */
#define DO_KAGO_7	151 /* ư������ */
#define DO_GAGO_7	152 /* ư������ */
#define	DO_SAGO_7	153 /* ư������ */
#define	DO_TAGO_7	154 /* ư������ */
#define	DO_BAGO_7	155 /* ư���и� */
#define	DO_MAGO_7	156 /* ư���޸� */
#define	DO_RAGO_7	157 /* ư����� */
#define	DO_WAGO_7	158 /* ư����� */
/* 159, 160 �� RESERVE	*/

/* Ϣ��̾�찷���Ǥ��롢[���� ���롢����]����Ƭ�� "��" */
#define DO_KAGO_8	161 /* ư������	*/
#define DO_GAGO_8	162 /* ư������ */
#define	DO_SAGO_8	163 /* ư������ */
#define	DO_TAGO_8	164 /* ư������ */
#define	DO_BAGO_8	165 /* ư���и� */
#define	DO_MAGO_8	166 /* ư���޸� */
#define	DO_RAGO_8	167 /* ư����� */
#define	DO_WAGO_8	168 /* ư����� */
/* 169, 170 �� RESERVE	*/

#define	SP_SA_MI1	171 /* ���ѡ�̤��  ��			*/
#define	SP_SA_MI2	172 /* ���ѡ�̤��  ��			*/
#define	SP_SA_YOU	173 /* ���ѡ�̤��  ��			*/
#define	SP_SA_SYU	174 /* ���ѡ�����  ����			*/
#define	SP_SA_KAT	175 /* ���ѡ���	   ����			*/
#define SP_SA_ME1	176 /* ���ѡ�̿��  ����			*/
#define	SP_SA_ME2	177 /* ���ѡ�̿��  ����			*/
#define DO_KAHEN	178
#define SP_KA_MIZ	178 /* ���ѡ�̤	   ��			*/
#define	SP_KA_YOU	179 /* ���ѡ���	   ��			*/
#define	SP_KA_SYU	180 /* ���ѡ�����  ����			*/
#define	SP_KA_KAT	181 /* ���ѡ���	   ����			*/
#define	SP_KA_MEI	182 /* ���ѡ�̿	   ����			*/
#define	TEINEI1		183 /* ��ǫư�죱  ����ä��㤤	���ä��㤤	*/
			    /*		   �������� �ʤ���	*/
#define	TEINEI2		184 /* ��ǫư�죲  ������		*/
#define	SP_KA_ONB	185 /* ���ޡ�����  ����			*/
#define	SP_FUKUSI	186 /* �ü�����:   ����	���� �ɤ�	*/
#define	AISATU		187 /* ������	   ���꤬�Ȥ� ����ǤȤ� ���Ϥ褦*/
#define	SP_KEIYOUSI	188 /* �ü���ƻ�  ����			*/
#define	TANKANJI	189 /* ñ����				*/

#define	IKKATU		200 /* ����ʸ���Ѵ��Ǥϻ��Ѥ��ʤ��ϸ�� */

#endif /* _Hinshi_H */
