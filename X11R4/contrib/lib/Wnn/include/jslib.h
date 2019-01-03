/*
	Copyright
		Kyoto University Research Institute for Mathematical Sciences
		1987, 1988, 1989
	Copyright OMRON TATEISI ELECTRONICS CO. 1987, 1988, 1989
	Copyright ASTEC, Inc. 1987, 1988, 1989


    Permission to use, copy, modify, and distribute this software
    and its documentation for any purpose and without any fee is
    hereby granted, subject to the following restrictions:

    The above copyright notice and this permission notice must appear
    in all versions of this software;

    The name of "Wnn" may not be changed; 

    All documentations of software based on "Wnn" must contain the wording
    "This software is based on the original version of Wnn developed by
    Kyoto University Research Institute for Mathematical Sciences (KURIMS),
    OMRON TATEISI ELECTRONICS CO. and
    ASTEC, Inc.", followed by the above copyright notice;

    The name of KURIMS, OMRON and ASTEC may not be used
    for any purpose related to the marketing or advertising
    of any product based on this software.

    KURIMS, OMRON and ASTEC make no representations about
    the suitability of this software for any purpose.
    It is provided "as is" without express or implied warranty.

    KURIMS, OMRON and ASTEC DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
    SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
    IN NO EVENT SHALL KURIMS, OMRON AND ASTEC BE LIABLE FOR ANY SPECIAL,
    INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
    LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
    OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
    PERFORMANCE OF THIS SOFTWARE.

    Author: Hideki Tsuiki	Kyoto University
				tsuiki%kaba.or.jp@uunet.uu.net

	    Hiroshi Kuribayashi	OMRON TATEISI ELECTRONICS CO.
				kuri@frf.omron.co.jp
				uunet!nff.ncl.omron.co.jp!kuri

	    Naouki Nide		Kyoto University
				nide%kaba.or.jp@uunet.uu.net

	    Shozo Takeoka	ASTEC, Inc.
				take%astec.co.jp@uunet.uu.net

	    Takashi Suzuki	Advanced Software Technology & Mechatronics
				Research Institute of KYOTO
				suzuki%astem.or.jp@uunet.uu.net

*/
/*	Version 4.0
 */
/*
	Nihongo	Henkan	Library Header File
*/
#ifndef	JSLIB
#ifndef	_WNN_SETJMP
#define	_WNN_SETJMP
#include <setjmp.h>
#endif

#define	JLIB_VERSION	0x4003

#ifndef NEC
#ifndef WCHAR_T
typedef unsigned short	wchar_t;
#define	WCHAR_T
#endif
#endif /* NEC */

#ifndef	wchar
#define	wchar	wchar_t
#endif	wchar

#define JSLIB

/* 疑似自立語の品詞名の定義 */
#define	WNN_SENTOU_MEI	"先頭"		/* 文節先頭に成れるもの */
#define	WNN_SUUJI_MEI	"数字"		/* 数詞相当 */
#define	WNN_KANA_MEI	"カナ"		/* 外来語など 名詞相当 */
#define	WNN_EISUU_MEI	"英数"		/* 半角カナ 濁点 半濁点 全/半角英数 */
#define	WNN_KIGOU_MEI	"記号"
#define	WNN_TOJIKAKKO_MEI	"閉括弧"
#define	WNN_FUZOKUGO_MEI	"付属語"
#define	WNN_KAIKKAKO_MEI	"開括弧"
#define	WNN_GIJI_MEI	"疑似"
/* 前端ベクタ(品詞番号)の定義 */
#define	WNN_BUN_SENTOU	-1	/* 大文節の先頭になれる奴 */
#define	WNN_ALL_HINSI	-2	/* なんでもええよ */
/* 終端ベクタの定義 */
#define	WNN_VECT_KANREN	0	/* 連文節変換の終端ベクター */
#define	WNN_VECT_KANTAN	1	/* 単文節変換の終端ベクター */
#define	WNN_VECT_KANZEN	1	/* 全候補取出しの終端ベクター 
				   単文節変換時のものと等しい。*/ 
#define WNN_VECT_BUNSETSU 2     /* 連文節変換時の、各文節の終端ベクター*/
#define	WNN_VECT_NO	-1	/* 終端ベクタ1 nasi */
/* 文節の終端の接続情報 */
#define	WNN_CONNECT_BK	1	/* 指定された終端ベクターに接続できた。*/
#define	WNN_NOT_CONNECT_BK	0	/* */
/* 文節の前端の接続情報 */
#define	WNN_CONNECT	1	/* 指定された(品詞、付属語)に接続できた。*/
#define	WNN_SENTOU	2	/* 大文節の先頭 */
#define	WNN_NOT_CONNECT	3	/* 先頭になれないし、前に接続できない */
#define	WNN_GIJI	4	/* 疑似文節を作った。*/

/* 疑似文節の直前に選択した候補 */
#define WNN_HIRAGANA	-1	/* ひらがな *//* 読みのまま */
#define WNN_KATAKANA	-11	/* カタカナ */
/* 数字 */
#define	WNN_NUM_HAN	-2	/* 半角数字 *//* 読みのまま */
#define	WNN_NUM_ZEN	-12	/* 全角数字 *//* １２３ */
#define	WNN_NUM_KAN	-13	/* 漢数字 *//* 一二三 */
#define	WNN_NUM_KANSUUJI -15	/* 漢数字 *//* 百二十三 */
#define	WNN_NUM_KANOLD	-16	/* 漢数字 *//* 壱百弐拾参 */
#define	WNN_NUM_HANCAN	-17	/* 半角数字 *//* 1,234 */
#define	WNN_NUM_ZENCAN	-18	/* 全角数字 *//* １，２３４ */
/* 英数 */
#define	WNN_ALP_HAN	-4	/* 半角 *//* 読みのまま */
#define	WNN_ALP_ZEN	-30	/* 全角 */
/* 記号 */
#define	WNN_KIG_HAN	-5	/* 半角 *//* 読みのまま */
#define	WNN_KIG_JIS	-40	/* 全角(JIS) */
#define	WNN_KIG_ASC	-41	/* 全角(ASC) */

#define WNN_FT_DICT_FILE 1
#define WNN_FT_HINDO_FILE 2
#define WNN_FT_FUZOKUGO_FILE 3

#define WNN_STATIC_DICT 1		/* static dict */
#define WNN_UD_DICT 2			/* updatable dict */
#define WNN_REV_DICT 3		/* Updatable Dictonary with Reverse Index */
#define WNN_REG_DICT 4		/* Regular Expressino Dictonary */

#define	WNN_DIC_RDONLY	1
#define	WNN_DIC_RW	0

#define	WNN_DIC_ADD_REV 1	/* 逆変換 */
#define	WNN_DIC_ADD_NOR 0	/* 通常の変換 */

/* header file for dic_syurui */
#define WNN_FILE_NOT_EXIST -1
#define WNN_NOT_A_JISHO 0
/* KURI
#define WNN_USER_DIC 1
#define WNN_SYSTEM_DIC 3
*/
#define WNN_HINDO_FILE 4
#define WNN_FUZOKUGO_FILE 5

/* set_hindo operation */
/* 取り敢えず、こういう名前で define しておきます。*/
#define WNN_HINDO_NOP -2
#define WNN_IMA_ON -3
#define WNN_IMA_OFF -4
#define WNN_HINDO_INC -3
#define WNN_HINDO_DECL -4
#define WNN_ENTRY_NO_USE -1

/*	Wnn constant
*/
#define WNN_HOSTLEN 16
#define WNN_ENVNAME_LEN 32

/*	js_who	*/
#define WNN_MAX_ENV_OF_A_CLIENT 32

struct wnn_jwho {
	int sd;   /* jserver 内のソケットディスクリプタ*/
	char user_name[64];   /* ユーザ名 */
	char host_name[64];   /* ホスト名 */
	int  env[WNN_MAX_ENV_OF_A_CLIENT];  /* このクライアントが使用している
				環境番号の列。空いているところには、-1 が入る */
};
typedef struct wnn_jwho WNN_JWHO;

/*	js_env_list	*/
#define WNN_MAX_JISHO_OF_AN_ENV 30
#define WNN_MAX_FILE_OF_AN_ENV 60

 struct wnn_env_info {
        int	env_id;		/* 環境番号 */
	char	env_name[WNN_ENVNAME_LEN]; /* 環境名 */
	int	ref_count;	/* 参照数 */
	/* struct wnn_param; */
	int	fzk_fid;	/* 付属語のファイル番号 */
	int jishomax;		/* 使用している辞書の個数 */
	int	jisho[WNN_MAX_JISHO_OF_AN_ENV];	/* 使用している辞書の辞書番号。
					   最初のjishomax個だけ意味がある */
	int	file[WNN_MAX_FILE_OF_AN_ENV]; /* この環境につながっている
				 ファイル番号(空いているところには、-1 が入る) */
};
typedef struct wnn_env_info WNN_ENV_INFO;

#define WNN_COMMENT_LEN 512     /* jisho no comment no nagasa */
#define WNN_F_NAMELEN 100
#define WNN_PASSWD_LEN 16  /* File Passwd Length */


struct wnn_dic_info {
	int dic_no;		/* 辞書番号 */
	int body;		/* 辞書本体のファイル番号 */
	int hindo;		/* 頻度のファイル番号 */
	int rw;			/* 辞書が登録可能かどうか
				   (WNN_DIC_RW, WNN_DIC_RDONLY) */
	int hindo_rw;		/* 頻度が更新可能かどうか
				   (WNN_DIC_RW, WNN_DIC_RDONLY) */
	int enablef;		/* 辞書が使用中かどうか
				   (1 = 使用中, 0 = 使用中断) */
	int nice;		/* 辞書の変換時の優先度 */
	int rev;		/* 逆変換か、どうか(1 = 逆変換, 0 = 正変換) */
/* added H.T */
	wchar comment[WNN_COMMENT_LEN];	/* 辞書のコメント */
	char fname[WNN_F_NAMELEN]; /* 辞書のファイル名 */
	char hfname[WNN_F_NAMELEN]; /* 頻度のファイル名 */
	char passwd[WNN_PASSWD_LEN]; /* 辞書のパスワード */
	char hpasswd[WNN_PASSWD_LEN]; /* 頻度のパスワード */
	int type;		/* 辞書の種類(WNN_UD_DICT,WNN_STATIC_DICT) */
	int gosuu;		/* 辞書の語数 */
	int localf;
	int hlocalf;
};

typedef struct wnn_dic_info WNN_DIC_INFO;

struct wnn_file_stat {
    int type;			/* ファイルの種類
		WNN_STATIC_DICT		固定形式辞書
		WNN_UD_DICT		登録可能形式辞書
		WNN_HINDO_FILE		頻度ファイル
		WNN_FUZOKUGO_FILE	付属語ファイル
		WNN_NOT_A_JISHO		その他   */
};

typedef struct wnn_file_stat WNN_FILE_STAT;

/*	*/
extern	int	wnn_errorno;		/* Wnnのエラーはこの変数に報告される */

extern char *wnn_dic_types[];	/* "固定","登録","逆変換","正規" */

#define FILE_ALREADY_READ -2

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_jserver_id {
	int	sd;
	char	js_name[40];
	int	js_dead;
	jmp_buf js_dead_env;	/* サーバが死んだ時に飛んでいくenv */
	int	js_dead_env_flg; /* jd_server_dead_envが有効か否か  */
};

typedef struct wnn_jserver_id WNN_JSERVER_ID;

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_env {
	int		env_id;
	WNN_JSERVER_ID	*js_id;
};

typedef struct wnn_env WNN_ENV;

struct wnn_param {
	int	n;	/* Ｎ(大)文節解析のＮ */
	int	nsho;	/* 大文節中の小文節の最大数 */
	int 	p1;	/* 自立語の頻度のパラメータ */
	int 	p2;	/* 小文節長のパラメータ */
	int 	p3;	/* 自立語長のパラメータ */
	int 	p4;	/* 今使ったよビットのパラメータ */
	int 	p5;	/* 辞書のパラメータ */
	int	p6;	/* 小文節の評価値のパラメータ */
	int	p7;	/* 大文節長のパラメータ */
	int	p8;	/* 小文節数のパラメータ */
	int 	p9;	/* 疑似品詞 数字の頻度 */
	int 	p10;	/* 疑似品詞 カナの頻度 */
	int 	p11;	/* 疑似品詞 英数の頻度 */
	int 	p12;	/* 疑似品詞 記号の頻度 */
	int 	p13;	/* 疑似品詞 閉括弧の頻度 */
	int 	p14;	/* 疑似品詞 付属語の頻度 */
	int 	p15;	/* 疑似品詞 開括弧の頻度 */
};

#ifdef nodef
struct wnn_dic_info_struct {
	int	body_fid;
	int	hindo_fid;
	int	enablef;
	int	rw;
	int     hindo_rw;
	int	nice;
};

typedef struct wnn_dic_info_struct WNN_DIC_INFO_STRUCT;
#endif

struct wnn_file_info_struct {
	int	fid;		/* ファイル番号 */
	char	name[WNN_F_NAMELEN]; /* ファイル名 */
	int	localf;		/* サーバ・サイトのファイルかどうか
				   1: サーバ・サイト
				   0: クライアント・サイト   */
	int	type;		/* ファイルの種類 */
	int	ref_count;	/* (環境からの)参照数 */
};

typedef struct wnn_file_info_struct WNN_FILE_INFO_STRUCT;

#define WNN_VECT_L	((256+8*4-1)/(8*4) + 5)	/***** !!!!! ****/

struct	wnn_sho_bunsetsu {
	int	end;		/* 候補文節の end char index */
	int	start;		/* 候補文節の top char index */
	int	jiriend;	/* 候補文節の自立語 end char index */
	int	dic_no;		/* 自立語の辞書内のエントリ番号 */
	int	entry;		/* 候補文節の自立語辞書 entry */

	int	hinsi;		/* 自立語品詞 */
	int     status;		/* 大文節の先頭か */
	int	status_bkwd;	/* usiro の文節に接続できるか */
	int 	hindo;		/* 候補自立語の頻度 */ 
	int 	ima;		/* 候補自立語の今使ったよビット */ 
	int	kangovect;	/* 接続ベクトルテーブルへのポインタ */
	int 	hyoka;		/* 小文節評価値 */
	wchar	*kanji;		/* 自立語文字列 */
	wchar	*yomi;		/* 自立語の読み文字列 */
	wchar	*fuzoku;	/* 付属語文字列 */
/*
 *頻度については、頻度ファイルが指定されている時は、
 *hindo = 頻度ファイルの(実)頻度値 + 辞書内の(実)頻度値
 *ima = 頻度ファイルの今使ったよビット
 *
 *頻度ファイルが指定されていない時には、
 *hindo = 辞書内の(実)頻度値、ima = 辞書内の今使ったよビット
 *である。ここで、実頻度値とは、計算機内で7ビットに圧縮された値である。
 *仮想頻度値ではなく実頻度値を返すのは、変換結果のデバッグのためである。
 */
};

struct	wnn_dai_bunsetsu {
	int	end;		/* 候補文節の end char index */
	int	start;		/* 候補文節の top char index */
	struct	wnn_sho_bunsetsu	*sbn;	/* 小文節解析結果へのポインタ */
	int 	hyoka;		/* 大文節評価値 */
	int	sbncnt;		/* 小文節数 (次候補の場合は、次候補数)
				   DSD_SBNは、*sbn から sbncnt だけある */
};

struct	wnn_jdata {
	int	dic_no;		/* 辞書番号 */
	int	serial;		/* 辞書内のエントリ番号 */
	int	hinshi;		/* 品詞番号(品詞番号と品詞名の対応は、manual/etc
				   の下を参照) */
	int	hindo;		/* 頻度 */
	int	ima;		/* 今使ったよビット */
	int	int_hindo;	/* 辞書内頻度 */
	int	int_ima;	/* 辞書内、今使ったよビット */
	wchar   *yomi;		/* 読みの文字列 */
	wchar	*kanji;		/* 漢字文字列 */
	wchar	*com;		/* エントリのコメント文字列 */
/*
 *頻度については、頻度ファイルが指定されている時は、
 *hindo = 頻度ファイルの(仮想)頻度値、ima = 頻度ファイルの今使ったよビット
 *int_hindo = 辞書内の(仮想)頻度値、int_ima = 辞書内の今使ったよビット
 *
 *頻度ファイルが指定されていない時には、
 *hindo = 辞書内の(仮想)頻度値、ima = 辞書内の今使ったよビット
 *int_hindo = -1、int_ima = -1
 *
 *ただし、どちらの場合でも、エントリが使用中止の状態の時には、
 *hindo = -1, ima = 0 あるいは、
 *int_hindo = -1, int_ima = 0 となる。
 *ここで、(仮想)頻度値とは、計算機内で7ビットに圧縮された頻度値から、
 *実際の値を想定した値である。
 */
};

struct	wnn_ret_buf {
	int	size;		/* buf から alloc されている大きさ */
	char	*buf;		/* malloc などでとられた領域 */
};


#define	WNN_F_UNIQ_LEN	(sizeof(struct wnn_file_uniq))

/* この構造体は、ライブラリ内部で用いられる */
struct wnn_file_head {
  struct wnn_file_uniq{
    int time;
    int dev;
    int inode;
    char createhost[WNN_HOSTLEN];
  } file_uniq;
  struct wnn_file_uniq file_uniq_org;
  int file_type;
  char file_passwd[WNN_PASSWD_LEN];
};

#define WNN_HINSI_NAME_LEN 64

#define	WNN_FILE_HEADER_LEN	(WNN_PASSWD_LEN + 8 + WNN_FILE_STRING_LEN + \
				 sizeof(struct wnn_file_uniq) * 2 + 4)
				/*
				  8 is for future use 
				  4 is for file_type.
				 */

#ifndef	DEMON
/*
  JSLIB function declaration
*/

extern	WNN_JSERVER_ID *js_open();
extern	int		js_close();
extern	WNN_JSERVER_ID *js_change_current_jserver();
extern	struct wnn_env *js_connect();
extern	int		js_disconnect();
extern	int		js_env_list();
extern	int		js_param_set();
extern	int		js_param_get();

#endif
#endif	JSLIB

/*
  Local Variables:
  kanji-flag: t
  End:
*/

