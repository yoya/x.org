/* Copyright 1991 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of NEC Corporation
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  NEC 
 * Corporation makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef _MFDEF_H_

/* @(#) 102.1 $Header: /work/nk.R3.1/iroha/RCS/mfdef.h,v 3.1 91/09/02 10:40:37 kon Exp $ */

/*
  このヘッダファイルではモードに割り振られた番号や、関数に割り振られた
  番号を管理する。

  */

/* 
 * カナ漢字変換のための様々なキーマップテーブル 
 * キーマップテーブルは処理関数へのポインタの配列となっている。
 */

typedef struct {
  int (*func)();
  unsigned char *keytbl;
  int flags;			/* 下を見よ */
  int (**ftbl)();
} *KanjiMode, KanjiModeRec;

/* flags の情報 */
#define IROHA_KANJIMODE_TABLE_SHARED	01
#define IROHA_KANJIMODE_EMPTY_MODE	02

extern unsigned char default_kmap[];
extern int  simplefunc();
extern int  multiSequenceFunc();

/* モード */

/* real modes */
/* 実モード(real mode)はキーマップの実体を持っているモード */

#define IROHA_MODE_AlphaMode		0	/* アルファベットモード */
#define IROHA_MODE_EmptyMode		1	/* 読み入力がない状態 */
#define IROHA_MODE_KigoMode		2	/* 記号一覧表示状態 */
#define IROHA_MODE_ZenHiraKakuteiMode	3	/* 確定入力モード */
#define IROHA_MODE_ZenKataKakuteiMode	4	/* 確定入力モード */
#define IROHA_MODE_HanKataKakuteiMode	5	/* 確定入力モード */
#define IROHA_MODE_ZenAlphaKakuteiMode	6	/* 確定入力モード */
#define IROHA_MODE_HanAlphaKakuteiMode	7	/* 確定入力モード */
#define IROHA_MODE_YomiMode		8	/* 読み入力している状態 */
#define IROHA_MODE_JishuMode		9	/* 文字種変換している状態 */
#define IROHA_MODE_TankouhoMode		10	/* 単一候補表示状態 */
#define IROHA_MODE_IchiranMode		11	/* 候補一覧表示状態 */
#define IROHA_MODE_YesNoMode		12	/* 単語登録の例文表示状態 */
#define IROHA_MODE_OnOffMode		13	/* 単語登録の例文表示状態 */
#define IROHA_MODE_AdjustBunsetsuMode   14	/* 文節伸縮モード */

#define IROHA_MODE_MAX_REAL_MODE	IROHA_MODE_AdjustBunsetsuMode + 1

/* imaginary modes */
/* 虚モード(imaginary mode)はキーマップの実体を持っていないモード */

#define IROHA_MODE_HenkanMode		IROHA_MODE_EmptyMode
#define IROHA_MODE_ZenHiraHenkanMode	IROHA_MODE_EmptyMode
#define IROHA_MODE_ZenKataHenkanMode	15	/* 変換入力モード */
#define IROHA_MODE_HanKataHenkanMode	16	/* 変換入力モード */
#define IROHA_MODE_ZenAlphaHenkanMode	17	/* 変換入力モード */
#define IROHA_MODE_HanAlphaHenkanMode	18	/* 変換入力モード */
#define IROHA_MODE_HexMode		19	/* １６進コード入力モード */
#define IROHA_MODE_BushuMode		20	/* 部首の読みの入力状態 */
#define IROHA_MODE_ExtendMode		21	/* 拡張機能選択 */
#define IROHA_MODE_RussianMode		22	/* ロシア文字選択 */
#define IROHA_MODE_GreekMode		23	/* ギリシア文字選択 */
#define IROHA_MODE_LineMode		24	/* 罫線選択 */
#define IROHA_MODE_ChangingServerMode	25	/* サーバ変更 */
#define IROHA_MODE_HenkanMethodMode	26	/* 変換方式選択 */
#define IROHA_MODE_DeleteDicMode	27	/* 単語削除 */
#define IROHA_MODE_TourokuMode		28	/* 単語登録モード */
#define IROHA_MODE_TourokuEmptyMode	IROHA_MODE_TourokuMode
#define IROHA_MODE_TourokuHinshiMode	29	/* 単語登録の品詞選択状態 */
#define IROHA_MODE_TourokuDicMode	30	/* 単語登録の辞書選択状態 */
#define IROHA_MODE_QuotedInsertMode	31	/* 引用入力モード */
#define IROHA_MODE_BubunMuhenkanMode	32	/* 部分無変換状態 */
#define IROHA_MODE_MountDicMode   	33	/* 辞書のmount,unmount状態 */

#define IROHA_MODE_MAX_IMAGINARY_MODE	IROHA_MODE_MountDicMode + 1


/* キー関数 */

/* 未定義にする */

#define IROHA_FN_Undefined		0 /* 何もしない or キーをスルーで通す */

/* 文字挿入 */

#define IROHA_FN_SelfInsert		1 /* 一文字挿入する */
#define IROHA_FN_FunctionalInsert	2 /* ローマ字かな変換など */
#define IROHA_FN_QuotedInsert		3 /* 引用挿入 */

/* モード切り換え */

#define IROHA_FN_JapaneseMode		4 /* 日本語入力モード */
#define IROHA_FN_AlphaMode		5 /* アルファベット入力モード */
#define IROHA_FN_HenkanNyuryokuMode	6 /* 変換入力モード */
#define IROHA_FN_ZenHiraKakuteiMode	7 /* 全角ひらがな確定入力モード */
#define IROHA_FN_ZenKataKakuteiMode	8 /* 全角カタカナ確定入力モード */
#define IROHA_FN_HanKataKakuteiMode	9 /* 半角カタカナ確定入力モード */
#define IROHA_FN_ZenAlphaKakuteiMode	10 /* 全角アルファベット確定入力モ */
#define IROHA_FN_HanAlphaKakuteiMode	11 /* 半角アルファベット確定入力モ */

#define IROHA_FN_HexMode		12 /* １６進入力モード */
#define IROHA_FN_BushuMode		13 /* 部首入力モード */
#define IROHA_FN_KigouMode		14 /* 記号入力モード */

/* 編集 */

#define IROHA_FN_Forward		15 /* 右へ */
#define IROHA_FN_Backward		16 /* 左へ */
#define IROHA_FN_Next			17 /* 次の行 */
#define IROHA_FN_Prev			18 /* 前の行 */
#define IROHA_FN_BeginningOfLine	19 /* 行頭 */
#define IROHA_FN_EndOfLine		20 /* 行末 */
#define IROHA_FN_DeleteNext		21 /* 削除 */
#define IROHA_FN_DeletePrevious		22 /* 削除 */
#define IROHA_FN_KillToEndOfLine	23 /* 行末まで削除 */

/* カナ漢字変換機能 */

#define IROHA_FN_Henkan			24 /* 変換 */
#define IROHA_FN_Kakutei		25 /* 確定 */
#define IROHA_FN_Extend			26 /* 伸ばし */
#define IROHA_FN_Shrink			27 /* 縮め */
#define IROHA_FN_AdjustBunsetsu		28 /* 文節伸縮モードに入る */
#define IROHA_FN_Quit			29 /* 取りやめ */
#define IROHA_FN_ExtendMode		30 /* 拡張モード */
#define IROHA_FN_Touroku                IROHA_FN_ExtendMode /* 登録 */
#define IROHA_FN_ConvertAsHex		31 /* １６進コードとして変換 */
#define IROHA_FN_ConvertAsBushu		32 /* 部首名として変換 */
#define IROHA_FN_KouhoIchiran		33 /* 候補一覧 */
#define IROHA_FN_BubunMuhenkan		34 /* 部分無変換 */

/* 文字種変換だけに使われる機能 */

#define IROHA_FN_Zenkaku		35 /* 全角への変換 */
#define IROHA_FN_Hankaku		36 /* 半角への変換 */
#define IROHA_FN_ToUpper		37 /* 大文字への変換 */
#define IROHA_FN_Capitalize		38 /* 先頭だけ大文字にする */
#define IROHA_FN_ToLower		39 /* 小文字への変換 */
#define IROHA_FN_Hiragana		40 /* ひらがな変換 */
#define IROHA_FN_Katakana		41 /* カタカナ変換 */
#define IROHA_FN_Romaji			42 /* ローマ字変換 */

/* 複雑な機能 */

#define IROHA_FN_FuncSequence		43 /* 複数の機能の割り当て */
#define IROHA_FN_UseOtherKeymap		44 /* キーシーケンスの対応 */

#define IROHA_FN_MAX_FUNC		IROHA_FN_UseOtherKeymap + 1

#define _MFDEF_H_
#endif /* _MFDEF_H_ */
