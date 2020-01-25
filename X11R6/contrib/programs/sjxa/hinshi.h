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

#define	MEISI_1		1  /* 他の分類に含まれない、複合名詞を作りやすい名詞 */
#define	MEISI_2		2  /* 接頭語 "お"			*/
#define	MEISI_3		3  /* 接頭語 "ご"			*/
#define	MEISI_4		4  /* 接尾語 "的" "化"			*/
#define	MEISI_5		5  /* 名詞＋サ変動詞、接頭語 "お"	*/
#define	MEISI_6		6  /* 名詞＋サ変動詞			*/
#define	MEISI_7		7  /* 名詞＋サ変動詞、接頭語 "ご"	*/
#define	MEISI_8		8  /* 名詞＋形容動詞			*/
#define	MEISI_9		9  /* 名詞＋形容動詞、接頭語 "お"	*/
#define	MEISI_10	10 /* 名詞＋形容動詞、接頭語 "ご"	*/
#define MEISI_11	11 /* 名詞＋副詞			*/
#define D_MEISI_1	12 /* 他の分類に含まれない、複合名詞を作りにくい名詞 */
#define D_MEISI_2	13 /* 接尾語 "さん"			*/
#define D_MEISI_3	14 /* "〜をする"			*/
#define D_MEISI_4	15 /* 名詞＋サ変動詞			*/
#define D_MEISI_5	16 /* 名詞＋形容動詞			*/
#define D_MEISI_6	17 /* 名詞＋副詞			*/
#define	MEISI_20	18 /* 接頭語はひらがなのみ		*/
/* 19 20 は RESERVE */
#define MYOUJI		21 /* 苗字				*/
#define NAMAE		22 /* 名前				*/
#define KIGYOU		23 /* 企業名				*/
#define TIMEI		24 /* 都道府県・区郡・市町村 および主要都市の駅名 */
#define KEN_KU		25 /* 都道府県・区の・・県(都,府,区)付き */
#define RENTAISI	26 /* 連体詞				*/
#define SETUZOKU	27 /* 接続詞				*/
#define KANDOUSI	28 /* 感動詞				*/
#define JOSUUSI1	29 /* "第〜" が接続できる助数詞		*/
#define SUUSI		30 /* 数詞				*/
#define SETTOU_1	31 /* 名詞(サ変名詞・刑動名詞含む)に接続 */
#define SETTOU_2	32 /* 地名に接続(北、南、西、東 等)	*/
#define SETTOU_3	33 /* 人名に接続(故 など)		*/
#define SETTOU_4	34 /* 数詞に接続(第、計 等)		*/
#define SETTOU_5	35 /* 接頭１より接続が強い		*/
#define SETUBI_1	36 /* 名詞に接続、名詞扱い		*/
#define SETUBI_2	37 /* 名詞に接続、名詞扱い。ただし連濁形 */
#define SETUBI_3	38 /* 接尾１より接続が強い		*/
#define SETUBI_4	39 /* 地名に接続、名詞扱い		*/
#define SETUBI_5	40 /* 名詞に接続、形容詞扱い		*/
#define SETUBI_6	41 /* 名詞に接続、形容動詞扱い		*/
#define SETUBI_7	42 /* 名詞に接続、サ変名詞扱い		*/
#define SETUBI_8	43 /* 名詞に接続、一段動詞扱い		*/
#define SETUBI_9	44 /* 接尾語＋副詞			*/
#define FUKUSI_1	45 /* 接続なし				*/
#define FUKUSI_2	46 /* "〜と" 接続			*/
#define FUKUSI_3	47 /* "〜と"、サ変動詞語尾が接続	*/
#define FUKUSI_4	48 /* サ変動詞語尾が接続		*/
#define FUKUSI_5	49 /* "〜に" 接続			*/
#define FUKUSI_6	50 /* "〜も" 接続			*/
#define FUKUSI_7	51 /* "〜の" 接続			*/
#define FUKUSI_8	52 /* "〜と"、"〜です、〜だ" 接続	*/
#define FUKUSI_9	53 /* "〜と"、サ変動詞語尾、"〜です、〜だ" 接続 */
#define JOSUUSI2	54 /* "第〜"が接続できない助数詞	*/
/* 55 - 59 は RESERVE */

/* [さ]		"さ" が接続して名詞化する	*/
/* [げ]		"げ" が接続して形容動詞化する	*/
/* [め]		"め" が接続して形容動詞化する	*/
/* [み]		"み" が接続して名詞化する	*/
/* [がる]	"がる" が接続して動詞化する	*/
#define KEIYOUSI_1	60 /* 上記付属語接続なし		*/
#define KEIYOUSI_2	61 /* [さ]				*/
#define KEIYOUSI_3	62 /* [さ、げ]				*/
#define KEIYOUSI_4	63 /* [さ、がる]			*/
#define KEIYOUSI_5	64 /* [さ、げ、がる]			*/
#define KEIYOUSI_6	65 /* [さ、め、げ]			*/
#define KEIYOUSI_7	66 /* [さ、め、げ、がる]		*/
#define KEIYOUSI_8	67 /* [さ、み、め、げ、がる]		*/
#define KEIYOUSI_9	68 /* 接頭語 "お", [さ、げ、がる]	*/
#define KEIYOUSI_10	69 /* 接頭語 "お", [さ、め、げ、がる]	*/
#define KEIYOUSI_11	70 /* [さ、み、め]			*/
#define KE_DOUSI_1	71 /* ダ活用				*/
#define KE_DOUSI_2	72 /* タリ活用				*/
#define KE_DOUSI_3	73 /* ダ活用、語幹で連体修飾		*/
#define KE_DOUSI_4	74 /* ダ活用、"〜さ"			*/
#define KE_DOUSI_5	75 /* ダ活用、"〜さ"、語幹で連体修飾	*/
#define KE_DOUSI_6	76 /* ダ活用、接頭語 "お" が接続	*/
#define KE_DOUSI_7	77 /* ダ活用、"〜さ"、接頭語 "お" が接続*/
#define KE_DOUSI_8	78 /* ダ活用、接頭語 "ご" が接続	*/
#define KE_DOUSI_9	79 /* ダ活用、"〜の"			*/

#define DO_SAHEN	80 /* サ変動詞、連用名詞扱いできない	*/
#define DO_ZAHEN	81 /* ザ変動詞。連用名詞扱いできない	*/
/* 82 - 89 は RESERVE 	*/

/* 連用名詞扱いできない、接頭語 "お" */
#define DO_1DAN_1	90 /* 動・一段	*/
#define DO_KAGO_1	91 /* 動・カ五	*/
#define DO_GAGO_1	92 /* 動・ガ五	*/
#define	DO_SAGO_1	93 /* 動・サ五	*/
#define	DO_TAGO_1	94 /* 動・タ五	*/
#define	DO_NAGO_1	95 /* 動・ナ五	*/
#define	DO_BAGO_1	96 /* 動・バ五	*/
#define	DO_MAGO_1	97 /* 動・マ五	*/
#define	DO_RAGO_1	98 /* 動・ラ五	*/
#define	DO_WAGO_1	99 /* 動・ワ五	*/

/* 連用名詞扱いできない、接頭語 "お" */
#define DO_1DAN_2	100 /* 動・一段 */
#define DO_KAGO_2	101 /* 動・カ五 */
#define DO_GAGO_2	102 /* 動・ガ五 */
#define	DO_SAGO_2	103 /* 動・サ五 */
#define	DO_TAGO_2	104 /* 動・タ五 */
#define	DO_BAGO_2	105 /* 動・バ五 */
#define	DO_MAGO_2	106 /* 動・マ五 */
#define	DO_RAGO_2	107 /* 動・ラ五 */
#define	DO_WAGO_2	108 /* 動・ワ五 */
/* 109, 110 は RESERVE	*/

/* 連用名詞扱いできない、[け、ける、けれ] */
#define DO_KAGO_5	111 /* 動・カ五 */
#define DO_GAGO_5	112 /* 動・ガ五 */
#define	DO_SAGO_5	113 /* 動・サ五 */
#define	DO_TAGO_5	114 /* 動・タ五 */
#define	DO_BAGO_5	115 /* 動・バ五 */
#define	DO_MAGO_5	116 /* 動・マ五 */
#define	DO_RAGO_5	117 /* 動・ラ五 */
#define	DO_WAGO_5	118 /* 動・ワ五 */
/* 119, 120 は RESERVE	*/

/* 連用名詞扱いできない、[け、ける、けれ]、接頭語 "お" */
#define DO_KAGO_6	121 /* 動・カ五 */
#define DO_GAGO_6	122 /* 動・ガ五 */
#define	DO_SAGO_6	123 /* 動・サ五 */
#define	DO_TAGO_6	124 /* 動・タ五 */
#define	DO_BAGO_6	125 /* 動・バ五 */
#define	DO_MAGO_6	126 /* 動・マ五 */
#define	DO_RAGO_6	127 /* 動・ラ五 */
#define	DO_WAGO_6	128 /* 動・ワ五 */
/* 129 は RESERVE */

/* 連用名詞扱いできる */
#define DO_1DAN_3	130 /* 動・一段 */
#define DO_KAGO_3	131 /* 動・カ五 */
#define DO_GAGO_3	132 /* 動・ガ五 */
#define	DO_SAGO_3	133 /* 動・サ五 */
#define	DO_TAGO_3	134 /* 動・タ五 */
#define	DO_BAGO_3	135 /* 動・バ五 */
#define	DO_MAGO_3	136 /* 動・マ五 */
#define	DO_RAGO_3	137 /* 動・ラ五 */
#define	DO_WAGO_3	138 /* 動・ワ五 */
/* 139 は RESERVE */

/* 連用名詞扱いできる、接頭語 "お" */
#define DO_1DAN_4	140 /* 動・一段４ */
#define DO_KAGO_4	141 /* 動・カ五４ */
#define DO_GAGO_4	142 /* 動・ガ五４ */
#define	DO_SAGO_4	143 /* 動・サ五４ */
#define	DO_TAGO_4	144 /* 動・タ五４ */
#define	DO_BAGO_4	145 /* 動・バ五４ */
#define	DO_MAGO_4	146 /* 動・マ五４ */
#define	DO_RAGO_4	147 /* 動・ラ五４ */
#define	DO_WAGO_4	148 /* 動・ワ五４ */
/* 149, 150 は RESERVE	*/

/* 連用名詞扱いできる、[け、 ける、けれ] */
#define DO_KAGO_7	151 /* 動・カ五 */
#define DO_GAGO_7	152 /* 動・ガ五 */
#define	DO_SAGO_7	153 /* 動・サ五 */
#define	DO_TAGO_7	154 /* 動・タ五 */
#define	DO_BAGO_7	155 /* 動・バ五 */
#define	DO_MAGO_7	156 /* 動・マ五 */
#define	DO_RAGO_7	157 /* 動・ラ五 */
#define	DO_WAGO_7	158 /* 動・ワ五 */
/* 159, 160 は RESERVE	*/

/* 連用名詞扱いできる、[け、 ける、けれ]、接頭語 "お" */
#define DO_KAGO_8	161 /* 動・カ五	*/
#define DO_GAGO_8	162 /* 動・ガ五 */
#define	DO_SAGO_8	163 /* 動・サ五 */
#define	DO_TAGO_8	164 /* 動・タ五 */
#define	DO_BAGO_8	165 /* 動・バ五 */
#define	DO_MAGO_8	166 /* 動・マ五 */
#define	DO_RAGO_8	167 /* 動・ラ五 */
#define	DO_WAGO_8	168 /* 動・ワ五 */
/* 169, 170 は RESERVE	*/

#define	SP_SA_MI1	171 /* サ変・未１  さ			*/
#define	SP_SA_MI2	172 /* サ変・未２  せ			*/
#define	SP_SA_YOU	173 /* サ変・未用  し			*/
#define	SP_SA_SYU	174 /* サ変・終体  する			*/
#define	SP_SA_KAT	175 /* サ変・仮	   すれ			*/
#define SP_SA_ME1	176 /* サ変・命１  しろ			*/
#define	SP_SA_ME2	177 /* サ変・命２  せよ			*/
#define DO_KAHEN	178
#define SP_KA_MIZ	178 /* カ変・未	   こ			*/
#define	SP_KA_YOU	179 /* カ変・用	   き			*/
#define	SP_KA_SYU	180 /* カ変・終体  くる			*/
#define	SP_KA_KAT	181 /* カ変・仮	   くれ			*/
#define	SP_KA_MEI	182 /* カ変・命	   こい			*/
#define	TEINEI1		183 /* 丁寧動詞１  いらっしゃい	おっしゃい	*/
			    /*		   ください なさい	*/
#define	TEINEI2		184 /* 丁寧動詞２  ござい		*/
#define	SP_KA_ONB	185 /* カ五・音便  いっ			*/
#define	SP_FUKUSI	186 /* 特殊副詞:   こう	そう どう	*/
#define	AISATU		187 /* 挨拶語	   ありがとう おめでとう おはよう*/
#define	SP_KEIYOUSI	188 /* 特殊形容詞  いい			*/
#define	TANKANJI	189 /* 単漢字				*/

#define	IKKATU		200 /* 一括（文節変換では使用しない熟語） */

#endif /* _Hinshi_H */
