.\"
.\" �Ķ�����
.\"
.bd K 3 3
.nr PI 2n
.nr PO 2c
.\"
.nr LL 16.5c
.nr PS 10
.nr VS 14p
.\"
.nr PD 10p
.ds CH
.ds RH
.ds LH
.ds CF
.ds RF
.ds LF
.BG
.\"
.\" ��ʸ
.\"
.B
.ce
.ps 16p
�أ����Ǥδ����ޤ���ʸ����ν��ϤˤĤ���
.vs 14p
.ps 8p
.ce 3
��������ǯ����
�����ŵ��ʳ��˴��ܥ��եȥ�������ȯ����
��������
.PP
�أ����Ǥϣ��Х��ȥ����ɤ�ʸ����ɽ���Ǥ���褦�ˤʤäƤ��ꡢ��������Ϥ�
�뤳�Ȥ���ǽ�Ǥ��롣�������ѿ������ɤȴ��������ɤ����ߤ��Ƥ�����ˤϡ�ɽ
���ΤȤ��ˤ��줾�졢XDrawString��XDrawString16�Ȥ��ä��褦���̸Ĥδؿ��ʤ�
��ӡ��ե���ȡ��ǣáˤ��Ѥ��ʤ���Фʤ�ʤ���
.PP
���������̾�Υץ�����ߥ󥰤ˤ����Ƥϥ��������ס��������󥹤��Ѥ����ꡢ
���뤤�ϣţգäΤ褦��MSB���Ѥ����ꤷ�ơ����Х��ȥ����ɤȣ��Х��ȥ����ɤ�
���ߤ�����Τ����̤Ǥ��롣���ΤȤ���Ϳ����줿ʸ���󤫤飱�Х��ȥ����ɤȣ�
�Х��ȥ����ɤ��ڤ�ʬ���Ƥ���XDrawString���뤤��XDrawString16��Ϳ����Ȥ���
�������򤷤ʤ���Фʤ餺���������ݤǤ��롣
.PP
�ʲ��δؿ����Ϥ��ν�����Ԥ��ʤ��饹�ȥ�󥰽�����Ԥ��饤�֥��ؿ�����
���롣�����δؿ��ϰ����Ȥ��ơ����Х��ȥ����ɤȣ��Х��ȥ����ɤκ��ߤ���ʸ
���������դ��롣
.PP
�������оݤȤʤ�ؿ��ϰʲ��Σ��ĤǤ��롣
.RS
(1) XKanjiTextWidth
(2) XKanjiTextExtents
(3) XQueryKanjiTextExtents
(4) XDrawKanjiText
(5) XDrawKanjiString
(6) XDrawImageKanjiString
.RE
.br
�����Υ饤�֥��ؿ����Ѥ������ܸ���б�����ˤ�
.RS
.IP (1) 4
�ؿ�̾���ءߡߤΤȤ�����ء�Kanji�ߤ��Ѥ��롣
.IP (2) 4
����Ϳ����Ȥ������������ؤΥݥ��󥿤�Ϳ���롣
.IP (3) 4
�ե���ȣɣĤ�Ϳ����Ȥ�����ե���ȣɣĤ�����ؤΥݥ��󥿤�Ϳ���롣
.IP (4) 4
�ե���ȹ�¤�ΤΥݥ��󥿤�����ؤΥݥ��󥿤�Ϳ���롣
�ʤ�Ȥ�ȥե���ȹ�¤�ΤΥݥ��󥿤�Ϳ���Ƥ������ˤϤ�����ʬ���������
�ե���ȹ�¤�ΤΥݥ��󥿤�����ؤΥݥ��󥿤�Ϳ����褦�ˤ��롣��
.IP (5) 4
�ǽ������Ȥ��ƴ����Υ⡼�ɡʥե�����ڤ��ؤ��Υ⡼�ɡˤ�Ϳ���롣
.RE
.br
�Ȥ��ä����ˤʤ롣����ե���Ȥ�����ˤϡ����ӥåȥ����ɤ��б������Τ�
��ӡ������ӥåȥ����ɤ��б������Τ��Ǽ���롣
.PP
�ǽ������Ȥ���Ϳ��������⡼�ɤϣ��Х��ȥ����ɤȣ��Х��ȥ����ɤκ��ߤ��ɤΤ�
���ˤ���Ƥ��뤫�򼨤���Τǡ����߰ʲ��Υ⡼�ɤ���������Ƥ��롣
.RS
.IP fs_euc_mode 20
�ţգå����ɤǤκ��ߤΤߤ��᤹�롣
.IP fs_simple_jis_mode 20
���������ס��������󥹤Ǥκ��ߤΤߤ��᤹��
.IP fs_jis_mode 20
���������ס��������󥹤Ǥκ��ߤΤߤ��᤹��
.IP fs_jis_euc_mode 20
�ţգ�,���������ס��������󥹡���ξ�Ԥ��᤹�롣
.RE
.PP
���줾��Υ⡼�ɻ��˥ե���Ȥ�ǣä�����ϼ��Τ褦�˳�Ǽ���롣
.RS
.IP fs_euc_mode�ʣţգá� 4
.nf
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
.fi
.IP fs_simple_jis_mode�ʣʣɣӡ� 4
.nf
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
.fi
.IP fs_jis_mode�ʣʣɣӡ� 4
.nf
����Σ����ܤˡ�����������ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ����գ�ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ����ʣɣӥ�����ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ����ʣɣӣ��ӥåȥ���ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ�����ʣɣӥ�����ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ����ʣɣӴ���ʸ���ե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ�����ʣɣӴ���ʸ���ե���Ȥ��б�������Τ�����롣
.fi
.IP fs_jis_euc_mode�ʣʣɣӤ���ӣţգá� 4
.nf
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
����Σ����ܤˡ��������ѤΥե���Ȥ��б�������Τ�����롣
.fi
.RE
.PP
�����ա�
.PP
�ǥե���Ȥδ����ե���ȤȤ��Ƥ�
.RS
.IP kanji 8
���б�����ank�ե���Ȥ�romankana�ˡ��ʣţףӤǤϣ����ߣ����ɥåȡˡ�
.br
���뤤��
.IP k14 8
���б�����ank�ե���Ȥ�a14�ˡ��������ʣ����ߣ����ɥåȡˡ���������
.RE
.br
���Ѥ���Τ�˾�ޤ�����
.PP
���줾��δؿ��ˤ�����ʸ��Ĺ��Ϳ���뤬������Ͻ��ʥХ���Ĺ��Ϳ����Ф�
�����㤨�д��������ɤϣ��Х��ȤȤʤꡢ�ޤ����������ס��������󥹤��פ���ʸ
������������롣
.PP
���
.nf
�ɴ����ɤ�
                              EUC �ʤ�  ���Х���
                              JIS �ʤ顡�����Х���
.fi
.PP
������ˤ���string�ΥХ���Ĺ��strlen(string)�ˤ������롣
.PP
�������ߤΥ⡼�ɤϥ桼�����ȼ��Τ�Τ��뤳�Ȥ���ǽ�Ǥ��롣�����κ��ߥ�
���ɤ�ɽ��fs_mode���ϼ��Τ褦���������Ƥ��롣
.br
.nf
typedef enum {single_byte_code, double_byte_code} code_len;
typedef struct {
        code_len nbyte;
        int (*func)();
} fs_mode;
.fi
.PP
�㤨�С�fs_euc_mode�ϣ��ӥåȥ����ɤ����򽦤�������ؿ�pickupEUC8�ؤΥ�
���󥿤ȣ����ӥåȥ����ɤ����򽦤�������ؿ�pickupEUC16 �ؤΥݥ��󥿤�ޤ�
�Ǥ��롣pickup�ؿ��ϼ��Τ褦�ʰ����ȤȤ�˸ƤӽФ��졢����ͤȤ��ƽ�������
��ʸ�������֤���
.br
.nf
pickup(from, to, len, next_return, remain_return, 
       nextIndex_return)
    char *from, *to,  **next_return;
    int len, *remain_return, *nextIndex_return;
.fi
.PP
from�ϣ��ӥåȥ����ɤȣ����ӥåȥ����ɤκ��ߤ���ʸ����ؤΥݥ��󥿤Ǥ���
��len �Ϥ���ʸ����ΥХ���Ĺ��ɽ�������ӥå�ʸ�����뤤�ϣ����ӥå�ʸ���Τ�
�������������to�Ǽ����줿�ΰ�س�Ǽ����롣pickup������ͤ�to�س�Ǽ���줿
ʸ����ʸ������ɽ����next_return�ˤϤ��δؿ����ɤ߻Ĥ���from���ʸ����ؤ�
�ݥ��󥿤��֤��졢remain_return�ˤϻĤä�ʸ����ΥХ��ȿ����֤���롣next
Index_return�ˤ��ɤ߻Ĥ���ʸ����Υ����ɷϤ����Ǥ��뤫�򼨤����ͤ��֤����
��
.PP
�ʾ�Τ褦�ʵ���˽����п������⡼�ɤ�������뤳�Ȥ��Ǥ��롣�ܤ����ϥ���
�������ɤ򻲾Ȥ��뤳�ȡ�
.bp
.SH
�ƥ饤�֥��ؿ�������
.SH
1. XKanjiTextWidth
.br
ʸ����������֤��ؿ���
.nf
int
XKanjiTextWidth(font_structs, string, count, mode) 

����XFontStruct **font_structs; 
����char        *string; 
����int         count;  /*���Х���ñ�̡�*/
��  fs_mode     *mode; 
.fi
.SH
2. XKanjiTextExtents
.nf
int 
XKanjiTextExtents(font_structs, string, nchars,
              direction_return, ascent_return, descent_return, 
������������  overall_return, mode)

    XFontStruct **font_structs; 
    char        *string; 
    int         nchars; /*���Х���ñ�̡�*/
����int         *direction_return, *ascent_return, *descent_return; 
    XCharStruct *overall_return;
    fs_mode     *mode;
.fi
.SH
3. XQueryKanjiTextExtents
.nf
int 
XQueryKanjiTextExtents(display, font_IDs, font_structs, string, nchars,
                   direction_return, ascent_return, descent_return,
                   overall_return, mode) 

    Display     *display; 
    XID         font_IDs; 
    XFontStruct **font_structs; 
    char        *string; 
    int         nchars; /*���Х���ñ�̡�*/
����int         *direction_return, *ascent_return, *descent_return; 
    XCharStruct *overall_return;
    fs_mode     *mode;
.fi
.SH
4. XDrawKanjiText
.nf
int 
XDrawKanjiText(display, d, gcs, font_structs,
           x, y, items, nitems, mode)

    Display     *display; 
    Drawable    d; 
    GC          *gc; 
    XFontStruct **font_structs; 
    int         x, y; 
    XTextItem   *items; 
    int         nitems; /*���Х���ñ�̡�*/
    fs_mode     *mode;
.fi
.SH
5. XDrawKanjiString
.nf
int 
XDrawKanjiString(display, d, gcs, font_structs,
             x, y, string, length, mode);

    Display     *display; 
    Drawable    d; 
    GC          *gcs;
    XFontStruct **font_structs; 
    int         x, y; 
    char        *string, 
    int         length; /*���Х���ñ�̡�*/
��  fs_mode     *mode; 
.fi
.SH
6. XDrawImageKanjiString
.nf
int 
XDrawImageKanjiString(display, d, gcs, font_structs, 
                  x, y, string, length, mode);

    Display     *display; 
    Drawable    d; 
    GC          *gcs;
    XFontStruct **font_structs; 
    int         x, y; 
    char        *string, 
    int         length; /*���Х���ñ�̡�*/
��  fs_mode     mode; 
.fi
.bp
.SH
���δؿ����ξ���Ū�ʥ���ѥ��ӥ�ƥ�
.SH
�������ѹ�������ǽ���Τ�����ʬ
.PP
���δؿ����ϻ��ͤ��Ϥä���ȷ�ޤä��櫓�ǤϤʤ��ΤǾ���Ū�˼��������ѹ�
�ˤʤ��礬����ޤ���
.IP (1)�ؿ�̾ 2
.PP
�ʲ��δؿ��δؿ�̾���ѹ�������뤫�⤷��ޤ���
�㤨��XKanjiTextWidth�ǤϤʤ���XnTextWidth�Τ褦��̾���ˤʤ뤫�⤷��ޤ���
.br
.nf
XKanjiTextWidth
XKanjiTextExtents
XQueryKanjiTextExtents
XDrawKanjiText
XDrawKanjiString
XDrawImageKanjiString
.fi
.IP (2)�⡼��̾ 2
.PP
�ʲ��Υ⡼��̾���ѹ�������뤫�⤷��ޤ���fs_euc_mode�ǤϤʤ��ơ�
Xfs_euc_mode�Τ褦��̾���ˤʤ뤫�⤷��ޤ���
.br
.nf
fs_euc_mode
fs_simple_jis_mode
fs_jis_mode
fs_jis_euc_mode
.fi
.IP (3)�⡼�ɤη�����̾ 2
.PP
�⡼�ɤη����ѹ�������뤫�⤷��ޤ���
�����ȼ�äƥ⡼�ɤη�̾���ѹ�������뤫�⤷��ޤ���
�㤨�С��⡼�ɤη��ϡ��ԥå����å״ؿ��Υݥ��󥿤����������ɽ�����褦
�ˤʤ뤫�⤷��ޤ���
�ޤ��⡼�ɤη�̾��fs_mode�ǤϤʤ���Xfs_mode�Τ褦�ˤʤ뤫�⤷��ޤ���
.br
.nf
typedef enum {single_byte_code, double_byte_code} code_len;
typedef struct {
        code_len nbyte;
        int (*func)();
} fs_mode;
.fi
.IP (4)�ԥå����å״ؿ� 2
.PP
�⡼�ɤ�������Ѥ�����ԥå����å״ؿ��λ��ͤ��ѹ�������뤫�⤷��ޤ�
�󡣰����λ��ͤ��ѹ����ͤ����ޤ���
.br
.nf
pickup(from, to, len, next_return, remain_return, 
       nextIndex_return)
    char *from, *to,  **next_return;
    int len, *remain_return, *nextIndex_return;
.fi
.SH
�������ѹ�������ʤ���ʬ
.PP
�ʲ�����ʬ�Ͼ���ˤ錄�ä��ѹ���̵���Ȼפ��ޤ���
.IP (1)���� 2
.PP
�ʲ��δؿ��ΰ����λ��ͤ��ѹ�������ʤ��Ȼפ��ޤ���
.br
.nf
XKanjiTextWidth
XKanjiTextExtents
XQueryKanjiTextExtents
XDrawKanjiText
XDrawKanjiString
XDrawImageKanjiString
.fi
.IP (2)�⡼�ɤλ��� 2
.PP
fs_euc_mode�Ǥ�����Σ����ܤ˱ѻ��Ѥξ�������졢�����ܤ˴����Ѥξ����
�����Ȥ��ä����ͤ��ѹ�������ʤ��Ȼפ��ޤ���