XFEOAK(1)

名称
    xfeoak - Ｘウィンドウ・インプットサーバ（かな漢字変換フロントエンド）

記述形式
    xfeoak [-display <displayname>] [-geometry <geom>] [-nobeep] [-v[erbose]]
    [{-none|-jis|-oya[yubi]}] [-fg <color>] [-bg <color>] [-bd <color>]
    [-bw <number>] [-bp <bitmapfile>] [-fn <fontname>] [-fk <kanjifontname>]
    [{-serverSync|-serverSyncNone}] [{-clientSync|-clientSyncNone}]
    [{-Ext|-noExt}] [{-ExtSetEventMask|-noExtSetEventMask}]
    [{-ExtForwardKey|-noExtForwardKey}] [{-ExtMove|-noExtMove}]
    [{-static|-dynamic}] [{-backend|-frontend}] [-proto <name>] [-locale <name>]
    [{-title [<string>]|-notitle}] [-name <string>] [-iconic]
    [-maxClient <number>] [-workSize <number>]
    [-transVersion <number/number>] [-divideSize <number>]

機能説明

    xfeoak は，X Window System用の日本語インプットサーバです．
　　xfeoak と Input method library 間の通信は，XIMプロトコルを使用します．
　　Transportレイアは，X Protocol と TCP/IP をサポートしています．

　　xfeoak は，富士通のＸウィンドウのＸサーバと辞書と接続して，ＯＡＫと
    互換性のある操作性を提供します．

オプション

    -display ディスプレイ
	使用するディスプレイを指定する．

    -geometry ジオメトリ
	ウィンドウのサイズと位置を指定する．

    -nobeep
	ベルを鳴らさない時に指定する．省略値は鳴らす．

    -v[erbose]
	XLIBのエラーメッセージの表示をするか指定する．省略値は表示しない．

    -none | -jis | -oya[yubi]
	使用するキーボードを指定する．
	  -none      - 一般のＸサーバで使用するときに指定．
	  -jis       - 富士通のＸサーバでＪＩＳキーボードを使用するときに指定．
	  -oya[yubi] - 富士通のＸサーバで親指キーボードを使用するときに指定．
	省略値は，-noneである．

    -fg 色
	ウィンドウの前面色を指定する．省略値は，白である．

    -bg 色
	ウィンドウの背景色を指定する．省略値は，黒である．

    -bd 色
	ウィンドウを囲む枠の色を指定する．省略値は，白である．

    -bw 数字
	ウィンドウを囲む枠の幅をピクセル単位で指定する．

    -bp ビットマップファイル名
	ウィンドウの背景ピックスマップを指定する．
	ファイル名に"."を指定すると"X11/bitmaps/1x1"同様な背景を表示する．

    -fn フォント
	英数カナ文字用フォントを指定する． 省略値は，
	"-*-fixed-medium-r-normal--14-*-*-*-*-*-jisx0201.1976-*"である．

    -fk フォント
	漢字用フォントを指定する．省略値は，
	"-*-fixed-medium-r-normal--14-*-*-*-*-*-jisx0208.1983-*"である．

    -serverSync | -serverSyncNone
	フロントエンド側のXIMプロトコルの同期指定する．
	省略値はServerSyncである．

    -clientSync | -clientSyncNone
	クライアント側のXIMプロトコルの同期指定する．
	省略値はclientSyncNoneである．

    -Ext | -noExt
	Extensionをすべて使用する/しない.
	省略値は下記オプションに従う.

    -ExtSetEventMask | -noExtSetEventMask
	Extension SetEventMaskを使用する/しない．
	-noExtSetEventMaskを指定した場合，FrontEnd は使用できない．
	省略値は使用する.

    -ExtForwardKey | -noExtForwardKey
	Extension ForwwardKeyEventを使用する/しない．
	省略値は使用しない.

    -ExtMove | -noExtMove
	Extension Moveを使用する/しない．
	省略値は使用しない.

    -static | -dynamic
	使用するXIMプロトコルの Static Event Flow と Dynamic Event Flow
	の指定する．省略値は Static Event Flow である．

    -backend | -frontend
	使用するXIMプロトコルの BackEnd と FrontEnd の指定する．
	省略値は BackEnd である．

    -proto トランスポート名
	サポートするXIMプロトコルのトランスポート名を指定する．
	トランスポート名には以下の名前を指定できる．
	  X	- X
	  local - TCP/IP internal domain
	  tcp	- TCP/IP Internet domain
	複数のトランスポートを指定する場合は，各トランスポート名を","で
	区切ること． 省略値は X である．

    -locale ロケール名
	サポートするロケール名を指定する.
	複数のロケールを指定する場合は，各ロケール名を","で区切ること．
	省略値は ja_JP である.

    -title [タイトル名] | -notitle
	ウィンドウタイトルの表示を指定する．
	表示する場合は，そのタイトル名も指定できる．
	省略値はタイトル表示でタイトル名が xfeoak である．

    -name リソース名
	リソースの名前を指定する．
	ex.) *xfeoak.Title : ON
	      ~~~~~~
	省略値はリソース名なしである．

    -iconic
	当サーバ起動時にアイコン表示するかを指定する．

    -maxClient 最大クライアント接続数
	接続可能なクライアントの最大数を指定する．
	省略値は 32 である．

    -workSize 作業領域のサイズ
	初期化時に確保する作業用領域の大きさを指定する．
	省略値は 4096 である．

    -transVersion majorコード/minorコード
	X Transport の major/minor-transport-version を指定する．
	major-code と minor-code を "/" で区切ること．
	省略値は 0/1 である．

	 major | minor |              method
	-------+-------+---------------------------------------
	   0   |   0   | only-CM & Property-with-CM
	       |   1   | only-CM & multi-CM
	       |   2   | only-CM & multi-CM & Property-with-CM
	-------+-------+---------------------------------------
	   1   |   0   | PropertyNotify
	-------+-------+---------------------------------------
	   2   |   0   | only-CM & PropertyNotify
	       |   1   | only-CM & multi-CM & PropertyNotify

    -divideSize 境界サイズ
	上記 TransportVersion が '0/2' '2/1' の場合の method 切り分けサイズ
	を指定する．
	省略値は 400 である．


リソース

    xfeoak は、標準的なリソースのほかに，以下のものを受け付ける．

    geometry (Geometry クラス)
	ウィンドウサイズと位置を指定する．省略値は "0x0+0+0" である．

    beep (Beep クラス)
	ベルを鳴らすかどうかを（on/off)で指定する．省略値は "on"(鳴らす)である．

    verbose (Verbose クラス)
	XLIBのエラーメッセージの表示をするかどうかを (on/off) で指定する．
	省略値は "off"(表示しない)である．

    keyBord (KeyBord クラス)
	使用するキーボードの種別を指定する．省略値は "none" である．
    	none    : 一般のＸサーバ で使用するときに指定する．
    	jis     : 富士通のＸサーバでＪＩＳキーボードを使用するときに指定する．
    	oyayubi : 富士通のＸサーバで親指キーボードを使用するときに指定する．
    
    foreground (Foreground クラス)
	ウィンドウの前面色を指定する．  省略値は "white" である．

    background (Background クラス)
	ウィンドウの背景色を指定する．  省略値は "black" である．

    borderColor (BorderColor クラス)
	ウィンドウを囲む枠の色を指定する．  省略値は "white" である．

    borderWidth (BorderWidth クラス)
	ウィンドウを囲む枠の幅をピクセル単位で指定する．

    backgroundPixmap (BackgroundPixmap クラス)
	ウィンドウの背景ピックスマップを指定する．
	ファイル名に"."を指定すると"X11/bitmaps/1x1"同様な背景を表示する．

    font (Font クラス)
	英数カナ文字用フォントを指定する．省略値は，
	"-*-fixed-medium-r-normal--14-*-*-*-*-*-jisx0201.1976-*"である．

    kanjiFont (KanjiFont クラス)
	漢字用フォントを指定する．省略値は，
	"-*-fixed-medium-r-normal--14-*-*-*-*-*-jisx0208.1983-*"である．

    serverSync (ServerSync クラス)
	フロントエンド側のXIMプロトコルの同期を (on/off) で指定する．
	省略値は "on" である．

    clientSync (ClientSync クラス）
	クライアント側のXIMプロトコルの同期を (on/off) で指定する．
	省略値は "off" である．

    extension (Extension クラス)
	Extension のすべての使用を (on/off) で指定する．
	省略値は下記リソースに従う.

    extSetEventMask (ExtSetEventMask クラス)
	Extension SetEventMask の使用を (on/off) で指定する．
	off を指定した場合，FrontEnd は使用できない．
	省略値は "on" である．

    extForwardKey (ExtForwardKey クラス)
	Extension ForwwardKeyEvent の使用を (on/off) で指定する．
	省略値は "off" である．

    extMove (ExtMove クラス)
	Extension Move の使用を (on/off) で指定する．
	省略値は "off" である．

    eventFlow (EventFlow クラス)
	使用するXIMプロトコルの Event Flow の種別を指定する．
	省略値は "Static" である．
	  static  : Static  Event Flow
	  Dynamic : Dynamic Event Flow

    eventMethod (EventMethod クラス)
	使用するXIMプロトコルの Event Method の種別を指定する．
	省略値は "BackEnd" である．
	  backend  : BackEnd  Event Method
	  frontend : FrontEnd Event Method

    protocol (Protocol クラス)
	サポートするXIMプロトコルのトランスポート名を指定する．
	トランスポート名には以下の名前を指定できる．
	  X     - X
	  local - TCP/IP internal domain
	  tcp   - TCP/IP Internet domain
	複数のトランスポートを指定する場合は，各トランスポート名を","で
	区切ること． 省略値は "X" である．

    locale (Locale クラス)
	サポートするロケール名を指定する.
	複数のロケールを指定する場合は，各ロケール名を","で区切ること．
	省略値は "ja_JP" である.

    title (Title クラス)
	タイトルを表示するかを (on/off) で指定する．
	省略値は "on" である．

    titleName (TitleName クラス)
	Title クラスが on の場合表示するタイトル名を指定する．
	省略値は "xfeoak" である．

    iconic (Iconic クラス)
	当サーバ起動時にアイコン表示するかを (on/off) で指定する．
	省略値は "off" である．

    maxClient (MaxClient クラス)
	接続可能なアプリケーション数を指定する．省略値は "32" である．

    workSize (WorkSize クラス)
	初期化時に確保する作業用領域の大きさを指定する．省略値は "4096" である．

    transVersion (TransVersion クラス)
	X Transport の major/minor-transport-version を指定する．
	省略値は "0/1" である．

	   major | minor |              method
	  -------+-------+---------------------------------------
	     0   |   0   | only-CM & Property-with-CM
	         |   1   | only-CM & multi-CM
	         |   2   | only-CM & multi-CM & Property-with-CM
	  -------+-------+---------------------------------------
	     1   |   0   | PropertyNotify
	  -------+-------+---------------------------------------
	     2   |   0   | only-CM & PropertyNotify
	         |   1   | only-CM & multi-CM & PropertyNotify

    divideSize (DivideSize クラス)
	上記 TransportVersion が '0/2' '2/1' の場合の method 切り分けサイズ
	を指定する．
	省略値は "400" である．


環境

    DISPLAY  : 省略時のＸサーバのホスト名と番号を得る．

操作方法

  1) 一般的な操作方法

      一般のＸサーバ で使用する場合の操作方法は，ＪＩＳキーボードを使用する
    場合の操作に準じます．
    物理的なカナ/英字キーの切替えは論理的な入力モードより優先します．

    基本動作方法を以下に示します．
       1. 起動方法
             $ xfeoak &
             画面の下部にウィンドウが表示されます．
       2. 日本語入力開始
            Control + スペース キーを押下すると開始します．
            下部ウィンドウの表示が「日本語     Ｒかな」に変更されます．
       3. 入力例
            入力キーで 'a','i'を押下すると，Preeditウィンドウに"あい"と
            表示されます．
            Control + h を押下すると Preeditウィンドウ内の文字がかな漢字変換
            します．その結果"愛"と表示します．
            （再度 Control + H で次候補が表示されます）
            Control + l を押下すると確定しアプリケーションに文字列を通知します．
       4. 日本語入力終了
            Control + スペース　キーを押下すると終了します． 

    その他の詳細の操作方法を以下に示す．
       
     - 入力モード（ステータス表示）については，表１を参照してください．
     - 入力モードの遷移方法は，表２を参照してください．
     - 各種変換方法については，表３を参照してください．

  2) ＯＡＫの操作方法

      富士通製のＸサーバ でＪＩＳキーボードまたは親指キーボードを使用する場合
    の操作方法は，ＯＡＫ日本語入力と同様に操作できます．
    ただし，いくつかの機能はサポートされていません．

その他

    かな漢字変換（辞書）がなくても動作します．この時には，ローマ字変換，
    ひらがな，カタカナ変換のみになります．


付録

  表１　入力モード

    --------------------+---------------------------------
    ステータス表示文字	| 入力状態
    --------------------+---------------------------------
    	       英数文字 | フロントエンドと接続していない状態
    --------------------+---------------------------------
    日本語     Ｒかな 	| 全角ローマ字かな変換入力状態
    日本語     Ｒカナ	| 全角ローマ字カナ変換入力状態
    日本語     ひらがな	| 全角ひらがな入力状態
    日本語     カタカナ	| 全角カタカナ入力状態
    日本語     英小文字	| 全角英小文字入力状態
    日本語     英大文字	| 全角英大文字入力状態
    日本語 半  Ｒかな 	| 半角ローマ字かな変換入力状態
    日本語 半  Ｒカナ	| 半角ローマ字カナ変換入力状態
    日本語 半  ひらがな	| 半角ひらがな入力状態
    日本語 半  カタカナ	| 半角カタカナ入力状態
    日本語 半  英小文字	| 半角英小文字入力状態
    日本語 半  英大文字	| 半角英大文字入力状態
    --------------------+---------------------------------

  表２　入力モード遷移

     ANK入力(フロントエンドと接続しない）
        ↑
      Control + Space
        ↓
    +--------------------- 日本語入力モード ------------------------+
    | +---------------------- 全角入力 --------------------------+  |
    | | +------------------------------------------------------+ |  |
    | | | +-ローマ字入力状態-+          +- ひらがな入力状態 -+ | |  |
    | | | |                  |          |                    | | |  |
    | | | | ひらがな変換入力 |          |  ひらがな入力      | | |  |
    | | | |     ↑           |          |      ↑            | | |  |
    | | | |     F1           | <- F3 -> |      F1            | | |  |
    | | | |     ↓           |          |      ↓            | | |  |
    | | | | カタカナ変換入力 |          |  カタカナ入力      | | |  |
    | | | +------------------+          +--------------------+ | |  |
    | | +------------------------------------------------------+ |  |
    | |                        F4   ↑                           |  |
    | |                        ↓   F1                           |  |
    | | +------------------------------------------------------+ |  |
    | | | +------------------+          +------------------+   | |  |
    | | | | 英小文字入力状態 | <- F4 -> | 英大文字入力状態 |   | |  |
    | | | +------------------+          +------------------+   | |  |
    | | +------------------------------------------------------+ |  |
    | +----------------------------------------------------------+  |
    |                            ↑                                 |
    |                            F2                                 |
    |                            ↓                                 |
    | +----------------------------------------------------------+  |
    | |                       半角入力                           |  |
    | +----------------------------------------------------------+  |
    +---------------------------------------------------------------+

  表３　かな漢字変換中（Preeditウィンドウ表示中）のキーと機能の対応表

    --------------------+---------------------------------------
       キーシム		|		機能
    --------------------+---------------------------------------
     Control + "h"	| 変換／次候補
     Control + "p"	| 前候補
     Control + "m"	| 無変換
     Control + "l"	| 確定
     Control + "w"	| 漢字選択
     Control + "u"	| 取消
     Control + "d"	| カーソル位置の一字削除
     Control + "i"	| カーソル位置に一字挿入
     Control + "b"	| カーソルを左に移動へ
     Control + "f"	| カーソルを右に移動へ
     Control + "a"	| カーソルを文の先頭
     Control + "e"	| カーソルを文の最後
    --------------------+---------------------------------------
     Kanji		| 変換／次候補
     Muhenkan		| 無変換
     Control + Kanji	| 漢字選択
     Control + Muhenkan	| 前候補
     Cancel		| 取消
     Escape		| 取消
     Delete		| カーソル位置の一文字削除
     F28		| カーソル位置の一文字削除
     Insert		| カーソル位置に一文字挿入
     Execute		| 確定
     Linefeed		| 確定し，改行(Linefeed)
     Return		| 確定し，改行(Return)
     BackSpace		| カーソルを左に移動へ
     Left		| カーソルを左に移動へ
     Right		| カーソルを右に移動へ
     Up			| カーソルを文の先頭
     Down		| カーソルを文の最後
     Tab		| カーソルを文の最後
    --------------------+----------------------------------------
