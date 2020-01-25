.\"
.\"Copyright (c) 1991-1994  Sony Corporation
.\"
.\"Permission is hereby granted, free of charge, to any person obtaining
.\"a copy of this software and associated documentation files (the
.\""Software"), to deal in the Software without restriction, including
.\"without limitation the rights to use, copy, modify, merge, publish,
.\"distribute, sublicense, and/or sell copies of the Software, and to
.\"permit persons to whom the Software is furnished to do so, subject to
.\"the following conditions:
.\"
.\"The above copyright notice and this permission notice shall be
.\"included in all copies or substantial portions of the Software.
.\"
.\"THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
.\"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\"IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
.\"DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
.\"OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
.\"THE USE OR OTHER DEALINGS IN THE SOFTWARE.
.\"
.\"Except as contained in this notice, the name of Sony Corporation
.\"shall not be used in advertising or otherwise to promote the sale, use
.\"or other dealings in this Software without prior written authorization
.\"from Sony Corporation.
.\"
.\"Author: Noriyuki Hosoe, Sony Corporation
.\"Modifier: Nobuyuki Tanaka, Sony Corporation
.\"
.\"$SonyRCSfile: sj3lib.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:48 $
.\"
.TL
sj3lib �饤�֥��
.AB
.LP
\fBsj3lib\fP �ϡ����ˡ��ʳ��ˤǳ�ȯ���줿��̾�����Ѵ������� \fIsj3serv\fP ��
�̿�����̾�����Ѵ���Ԥ������饤������ѥ饤�֥��Ǥ���
.sp
���ץꥱ�������ץ������ϡ�\fBsj3lib\fP ����Ѥ��뤳�Ȥˤ��
\&  \fIsj3serv\fP ����Ѥ�����̾�����Ѵ���Ԥ����Ȥ���ǽ�Ȥʤ�ޤ���
.AE
.SH
�Ϥ����
.RS
.IP ��
\fBsj3lib\fP �Ǥϡ��ؿ����ѿ��� sj3_ �ǻϤޤ�̾������Ѥ��Ƥ��ޤ��Τǡ�
sj3_ �ǻϤޤ�ؿ�̾���ѿ�̾�λ��ѤϤ��򤱤���������
.IP ��
\fBsj3lib\fP �ǰ���ʸ�������ɤϡ��ؿ�̾�κǸ�� "_euc" ���Ĥ��Ƥ���Ȥ��ϡ�
EUC �Ǥ���"_mb" ���Ĥ��Ƥ�����ϡ�������ʸ�������ɤ��ץ������¹Ի���
��������˰�¸���ޤ����ʤˤ��դ��Ƥʤ��Ȥ��ˤϡ�������ʸ�������ɤ˥��ե� 
JIS ����Ѥ��ޤ���
.IP ��
�ᥤ�󼭽񡢥桼�����񡢳ؽ��ե�����ϡ���³���륵���Ф�ư��Ƥ���ޥ����Τ�Τ����Ѥ���ޤ���
.br
�����Υե�����ϡ��ǥե���ȤǤϥǥ��쥯�ȥ� \fB/usr/dict/sj3\fP �β��ˤ�����ޤ����������ФΥ��åȥ��åץե�����ˤ���ѹ�����ǽ�Ǥ���
.IP ��
�桼������ȳؽ��ե�����ϡ��ؿ� sj3_open �ǻ��ꤵ�줿�桼��̾�� USER ��
����С�\fBuser/USER/private.dic\fP �� \fBuser/USER/study.dat\fP �Ǥ���
.RE
.NH
�����Фλ��ѳ���
.RS
.LP
.DS
sj3_open(hname, uname)
char	*hname;		/* �ۥ���̾�ؤΥݥ��� */
char	*uname;		/* �桼��̾�ؤΥݥ��� */
.DE
.sp
ʸ����ؤΥݥ��� \fBhname\fP �ǻ��ꤵ�줿�ۥ��Ⱦ�Υ����� \fIsj3serv\fP ��
�Ф����桼��̾�Ȥ��ƥݥ��� \fBuname\fP �ǻ��ꤵ���ʸ�������Ѥ���
��³���ߤޤ���
.sp
��³�ˤϡ��ۥ���̾�ؤΥݥ��� \fBhname\fP ���ʥ�ݥ��󥿡����뤤�ϡ�
����ʸ����ޤ���ʸ���� unix �ؤΥݥ��󥿤λ��ˤ� AF_UNIX �����åȤ�
����ʳ��ΤȤ��ˤ� AF_INET �����åȤ���Ѥ��ޤ���
.sp
��³�Ǥ����Ȥ��ˤϡ������Ф�Ω���夲���˻��ꤵ�줿����ǥ��쥯�ȥ���ˤ���
�ᥤ�󼭽� sj3main.dic���桼������ user/\fBuname\fP/private.dic��
�ؽ��ե����� user/\fBuname\fP/study.dat �򥪡��ץ󤷤ޤ���
.br
���ꤵ�줿�桼���ѤΥǥ��쥯�ȥꡢ�桼�����񡢳ؽ��ե����뤬
¸�ߤ��ʤ��Ȥ��ˤϡ��������������ߤޤ���
.sp
���δؿ��ϡ����ʴ����Ѵ��롼�������Ѥ��������ɬ���ƤФʤ���Фʤ�ޤ���
.sp
�����Ф��������³�Ǥ����Ȥ��ˤ� 0 �򡢲������顼�Τ��ä��Ȥ��ˤ� 0 �ʳ���
���顼���ơ��������֤��ޤ���
.sp
���顼���ơ������ϥإå��ե����� \fB/usr/include/sj3lib.h\fP ��
��������ӥåȥ������󤵤줿�ͤǡ��ܺ٤ϰʲ��ΤȤ���Ǥ���
.sp
.IP "\fBSJ3_SERVER_DEAD\fP
��³������˥����Ф������󤷤ޤ�����
.IP "\fBSJ3_CONNECT_ERROR\fP
�����Ф��ʤ������뤤����³�Ǥ��ޤ���
.IP "\fBSJ3_ALREADY_CONNECTED\fP
���Ǥ˥����Ф���³�ѤǤ���
.IP "\fBSJ3_CANNOT_OPEN_MDICT\fP
�ᥤ�󼭽񤬥����ץ�Ǥ��ޤ���
.IP "\fBSJ3_CANNOT_OPEN_UDICT\fP
�桼�����񤬥����ץ�Ǥ��ޤ���
.IP "\fBSJ3_CANNOT_OPEN_STUDY\fP
�ؽ��ե����뤬�����ץ�Ǥ��ޤ���
.IP "\fBSJ3_CANNOT_MAKE_UDIR\fP
�Ŀͥե�����Τ���Υǥ��쥯�ȥ꤬�����Ǥ��ޤ���
.IP "\fBSJ3_CANNOT_MAKE_UDICT\fP
�桼�����񤬺����Ǥ��ޤ���
.IP "\fBSJ3_CANNOT_MAKE_STUDY\fP
�ؽ��ե����뤬�����Ǥ��ޤ���
.RE
.sp
���顼���ơ������ˤϡ������Τ������ʤ��Ȥ� 1 �ӥåȤ�Ω�äƤ��ޤ���
.br
���顼���ơ������Τ��� \fBSJ3_SERVER_DEAD\fP ���뤤�� \fBSJ3_CONNECT_ERROR\fP ��
�֤��줿�Ȥ��ʳ��ϡ������ФȤ���³����Ω����Ƥ��ޤ���
.RE

.NH
�����Фλ��ѽ�λ
.RS
.LP
.DS
sj3_close()
.DE
.sp
\fIsj3_open\fP �ˤ�äƳ�Ω���줿�����ФȤ���³���ڤ������ޤ���
.br
�ᥤ�󼭽񡢥桼�����񡢳ؽ��ե�����Τ��٤Ƥ������ץ󤵤�Ƥ����ΤȤ��ơ�
�����Υ����������ߤޤ���
.sp
���δؿ��ϡ����ʴ����Ѵ��롼����λ��Ѥ򽪤���Ȥ��˸ƤФʤ���Фʤ�ޤ���
.sp
�����ФȤλ��Ѥ�����˽�λ�Ǥ����Ȥ��ˤ� 0 ��
�������顼�Τ��ä��Ȥ��ˤ� 0 �ʳ��Υ��顼���ơ��������֤��ޤ���
.sp
���顼���ơ������ϥإå��ե����� \fB/usr/include/sj3lib.h\fP ��
��������ӥåȥ������󤵤줿�ͤǡ��ܺ٤ϰʲ��ΤȤ���Ǥ���
.sp
.IP "\fBSJ3_SERVER_DEAD\fP
�ڤ�����������˥����Ф������󤷤ޤ�����
.IP "\fBSJ3_DISCONNECT_ERROR\fP
�����Ф��������顼������ޤ�����
.IP "\fBSJ3_NOT_CONNECTED\fP
�����Ф���³����Ƥ��ޤ���
.IP "\fBSJ3_NOT_OPENED_MDICT\fP
�ᥤ�󼭽񤬥����ץ󤵤�Ƥ��ޤ���
.IP "\fBSJ3_NOT_OPENED_UDICT\fP
�桼�����񤬥����ץ󤵤�Ƥ��ޤ���
.IP "\fBSJ3_NOT_OPENED_STUDY\fP
�ؽ��ե����뤬�����ץ󤵤�Ƥ��ޤ���
.IP "\fBSJ3_CLOSE_MDICT_ERROR\fP
�ᥤ�󼭽񤬥��������Ǥ��ޤ���
.IP "\fBSJ3_CLOSE_UDICT_ERROR\fP
�桼�����񤬥��������Ǥ��ޤ���
.IP "\fBSJ3_CLOSE_STUDY_ERROR\fP
�ؽ��ե����뤬���������Ǥ��ޤ���
.RE
.sp
���顼���ơ������ˤϡ������Τ������ʤ��Ȥ� 1 �ӥåȤ�Ω�äƤ��ޤ���
.br
�ɤΥ��顼���ơ��������֤äƤ⡢�����ФȤ���³���ڤ�Ƥ��ޤ���
.RE

.NH
ʸ�ϰ���Ѵ�
.RS
.LP
.DS
sj3_getkan(yomi, bun, knjbuf, knjsiz)
sj3_getkan_euc(yomi, bun, knjbuf, knjsiz)
sj3_getkan_mb(yomi, bun, knjbuf, knjsiz)
unsigned char	*yomi;		/* �ɤ�ʸ����ؤΥݥ��� */
struct bunsetu	bun[];		/* ʸ�ṽ¤�ΤؤΥݥ��� */
unsigned char	*knjbuf;	/* ����ʸ�����Ǽ�ΰ�ؤΥݥ��� */
int		knjsiz;		/* ����ʸ�����Ǽ�ΰ���礭�� */
.DE
.sp
Ϳ����줿�ɤߤ�ʸ�ϰ���Ѵ��������η�̤��֤��ޤ���
.br
�ݥ��� \fByomi\fP �ǻ��ꤵ�줿�ɤ�ʸ����򤫤ʴ����Ѵ�����
�ݥ��� \fBknjbuf\fP �Ǽ�������礭�� \fBknjsiz\fP ���ΰ��
�Ѵ���̤δ���ʸ��������� \fBstruct bunsetu bun[]\fP ��
��ʸ��˴ؤ������򥻥åȤ��ޤ���
.br
\fBstruct bunsetu\fP �ϥإå��ե����� \fB/usr/include/sj3lib.h\fP ��
�������Ƥ���ʲ��Τ褦�ʹ�¤�ΤǤ���
.sp
.RS
.DS
struct bunsetu {
    int             srclen;         /* �ɤߤ�Ĺ��         */
    int             destlen;        /* ������Ĺ��         */
    unsigned char   *srcstr;        /* �ɤߤؤΥݥ���   */
    unsigned char   *deststr;       /* �����ؤΥݥ���   */
    struct studyrec dcid;           /* �ؽ��Τ���Υǡ��� */
};
.DE
.RE
.sp
�ɤߤ�����ʸ������Ѵ��Ǥ����Ȥ��ˤϡ����������ǤϤ���������ꤵ��ޤ�����
�ݥ��� \fBknjbuf\fP �Ǽ�������ΰ���礭�� \fBbufsiz\fP ���������ʤɤ�
�����Ǵ���ʸ������֤��ʤ�ʸ�᤬�������Ȥ��ˤϡ������ɤ�ʸ����ؤ�
�ݥ��󥿤� \fBsrcstr\fP �ˡ��ޤ�����Ĺ���� \fBsrclen\fP �˻�����
\fBdestlen\fP �� 0 �� \fBdeststr\fP ���ʥ�ݥ��󥿤ι�¤�Τ���������ޤ���
.sp
�ɤߤȤ���Ϳ������ʸ����ϡ�
���ե� JIS �ޤ��� EUC �� 2 �Х��ȥ����ɤǹ�������Ƥ��뤳�Ȥ���§�Ǥ�����
1 �Х��ȥ����ɤ������äƤ��Ƥ⤫�ޤ��ޤ���
���������κݤˤϡ�1 �Х��ȥ����ɤϴ������Ѵ�����뤳�ȤϤ���ޤ���
.br
�ɤ�ʸ����ϥʥ�ǽ�λ���Ƥ��ʤ���Фʤ餺��
����Ǥ� 256 ʸ����ۤ��ƤϤʤ�ޤ���
.br
���� \fBstruct bunsetu bun[]\fP �μ��Τϡ��桼���ץ������¦��
���ʤ���Фʤ�ޤ���
�����礭���Ϻ�����ɤ�ʸ����ʬɬ�פȤʤ�ޤ���
.br
�Ѵ�����ɬ�פȤ����ݥ��� \fBknjbuf\fP �Ǽ�������ΰ���礭���ϡ�
�Ѵ�����ʸ��������Ͽ����Ƥ���ñ���Ĺ���ʤɤˤ�ä�
�Ѳ����ޤ�������Ǥ� 1024 �Х������٤Ϥ��ä��ۤ����ɤ��Ǥ��礦��
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
�ɤߤ��ʥ�ʸ������ä������뤤���ɤߤ�Ĺ�������Ȥ��ˤ� 0 ���֤���ޤ���
.br
���ｪλ�����Ȥ��ˤϡ����� \fBstruct sj3bunsetu bun[]\fP �˥��åȤ��줿
ʸ������֤���ޤ���
.RE

.NH
Ʊ���յ�����μ���
.RS
.LP
.DS
sj3_douoncnt(yomi)
sj3_douoncnt_euc(yomi)
sj3_douoncnt_mb(yomi)
unsigned char	*yomi;		/* �ɤ�ʸ����ؤΥݥ��� */
.DE
.sp
Ϳ����줿�ɤߤ�ʸ���Ѵ������꥿�����ͤȤ���Ʊ���յ�������֤��ޤ���
.sp
�ݥ��� \fByomi\fP �ˤ�ä��ɤߤȤ���Ϳ������ʸ����ϡ�
���ե� JIS �ޤ��� EUC �� 2 �Х��ȥ����ɤǹ�������Ƥ��뤳�Ȥ���§�Ǥ�����
1 �Х��ȥ����ɤ������äƤ��Ƥ⤫�ޤ��ޤ���
���������κݤˤϡ�1 �Х��ȥ����ɤϴ������Ѵ�����뤳�ȤϤ���ޤ���
.br
�ɤ�ʸ����ϥʥ�ǽ�λ���Ƥ��ʤ���Фʤ餺��
����Ǥ� 64 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
�ɤߤ��ʥ�ʸ������ä������뤤���ɤߤ�Ĺ���������ޤ���Ϳ����줿
�ɤߤ� 1 ʸ�᤬��������ʤ��ä��Ȥ��ˤ� 0 ���֤���ޤ���
.br
���ｪλ�����Ȥ��ˤ�Ʊ��������֤���ޤ���
.RE

.NH
Ʊ���յ���μ���
.RS
.LP
.DS
sj3_getdouon(yomi, dou)
sj3_getdouon_euc(yomi, dou)
sj3_getdouon_mb(yomi, dou)
unsigned char	*yomi;		/* �ɤ�ʸ����ؤΥݥ��� */
struct douon	*dou;		/* Ʊ���յ��칽¤�ΤؤΥݥ��� */
.DE
.sp
Ϳ����줿�ɤߤ�ʸ���Ѵ�����Ʊ���յ����Ϳ����줿��¤�Τ˥��åȤ�
��ȤȤ�ˡ��꥿�����ͤȤ���Ʊ���յ�������֤��ޤ���
.sp
�ݥ��� \fByomi\fP �ˤ�ä��ɤߤȤ���Ϳ������ʸ����ϡ�
���ե� JIS �ޤ��� EUC �� 2 �Х��ȥ����ɤǹ�������Ƥ��뤳�Ȥ���§�Ǥ�����
1 �Х��ȥ����ɤ������äƤ��Ƥ⤫�ޤ��ޤ���
���������κݤˤϡ�1 �Х��ȥ����ɤϴ������Ѵ�����뤳�ȤϤ���ޤ���
.br
�ɤ�ʸ����ϥʥ�ǽ�λ���Ƥ��ʤ���Фʤ餺��
����Ǥ� 64 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
���� \fBstruct douon dou[]\fP �ˤ�Ʊ���յ���˴ؤ�����󤬥��åȤ���ޤ���
��¤�� \fBstruct douon\fP �� \fBsj3lib.h\fP ���������Ƥ���
�ʲ��Τ褦�ʹ�¤�ΤǤ���
.sp
.RS
.DS
struct douon {
    unsigned char   ddata[512];	/* Ʊ���յ���Υǡ��� */
    int             dlen;		/* Ʊ���յ����Ĺ��   */
    struct studyrec dcid;		/* �ؽ��Τ���Υǡ��� */
};
.DE
.RE
.sp
���� \fBstruct douon dou[]\fP �μ��Τϥ桼���ץ������¦��
���ʤ���Фʤ�ޤ���
.sp
��¤�� \fBstruct douon\fP �� \fBddata\fP �ϡ��ʥ�ǽ�λ����ʸ����Ǥ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
�ɤߤ��ʥ�ʸ������ä������뤤���ɤߤ�Ĺ���������ޤ���Ϳ����줿
�ɤߤ� 1 ʸ�᤬��������ʤ��ä��Ȥ��ˤ� 0 ���֤���ޤ���
.br
���ｪλ�����Ȥ��ˤϡ����� \fBstruct douon dou[]\fP ��
���åȤ��줿Ʊ��������֤���ޤ���
.RE

.NH
ʸ��ؽ�
.RS
.LP
.DS
sj_gakusyuu(dcid)
struct studyrec	*dcid;		/* �ؽ��ǡ����ؤΥݥ��� */
.DE
.sp
�ؽ��ǡ����ؤΥݥ��� \fBdcid\fP �μ�����
\fIsj3_getkan\fP �� \fIsj_getdouon\fP �ˤ������줿�ؽ��ǡ���
����Ѥ��ơ�ʸ��ؽ���Ԥ��ޤ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
����˳ؽ��Ǥ����Ȥ��ˤ� 0 �����ؽ��Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ����ͤ�
�֤���ޤ���
.RE

.NH
ʸ��Ĺ�ؽ�
.RS
.LP
.DS
sj_gakusyuu2(yomi1, yomi2, dcid)
sj_gakusyuu2_euc(yomi1, yomi2, dcid)
sj_gakusyuu2_mb(yomi1, yomi2, dcid)
unsigned char	*yomi1;		/* �裱ʸ����ɤ�ʸ����ؤΥݥ��� */
unsigned char	*yomi2;		/* �裲ʸ����ɤ�ʸ����ؤΥݥ��� */
struct studyrec	*dcid;		/* �裲ʸ��γؽ��ǡ����ؤΥݥ��� */
.DE
.sp
ʸ��Ĺ�ؽ���Ԥ��ޤ���
.sp
��ʸ��Ȥ����Ѵ����줿ʸ���󤬣�ʸ���ʬ����줿�褦�ʾ�硢
���뤤�ϣ�ʸ��δ֤Ǥ��ζ������ѹ����줿�褦�ʤȤ��ˤϡ�
\fByomi1\fP �˿������裱ʸ����ɤߤؤΥݥ��󥿤�
\fByomi2\fP �˿������裲ʸ����ɤߤؤΥݥ��󥿤�
\fBdcid\fP �˿������裲ʸ��γؽ��ǡ�����Ϳ���뤳�Ȥˤ�ꡢ
ʸ��Ĺ�ؽ���Ԥ��ޤ���
.sp
��ʸ��Ȥ����Ѵ����줿ʸ���󤬣�ʸ��ˤʤä����ˤϡ�
\fByomi1\fP �˿����ˤǤ�����ʸ����ɤߤؤΥݥ��󥿤�
\fByomi2\fP �ˤϥʥ�ݥ��󥿤ޤ��ϥʥ�ʸ����ؤΥݥ��󥿤�Ϳ���뤳�Ȥˤ�ꡢ
ʸ��Ĺ�ؽ���Ԥ��ޤ������ΤȤ���\fBdcid\fP �ˤ����ꤹ�٤��ǡ�����
�äˤ���ޤ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
����˳ؽ��Ǥ����Ȥ��ˤ� 0 �����ؽ��Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ����ͤ�
�֤���ޤ���
.RE

.NH
ñ����Ͽ
.RS
.LP
.DS
sj3_touroku(yomi, kanji, hinsi)
sj3_touroku_euc(yomi, kanji, hinsi)
sj3_touroku_mb(yomi, kanji, hinsi)
unsigned char	*yomi;		/* �ɤ�ʸ����ؤΥݥ��� */
unsigned char	*kanji;		/* ����ʸ����ؤΥݥ��� */
int	hinsi;			/* �ʻ쥳����             */
.DE
.sp
�ɤ�ʸ����ؤΥݥ��� \fByomi\fP��
����ʸ����ؤΥݥ��� \fBkanji\fP��
�ʻ쥳���� \fBhinsi\fP �ǻ��ꤵ�줿���桼���������Ͽ���ޤ���
.sp
�ɤߤϰʲ��Υ��ե� JIS �ޤ��� EUC �����ɤǹ������졢
���ĥʥ�ǽ�λ���Ƥ��ʤ���Фʤ�ޤ���
�ޤ�����Ĺ���ϡ�����Ǥ� 31 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
.RS
.DS
��������
���������� ����������
���£ãģ� �ƣǣȣɣ� �ˣ̣ͣΣ� �Уѣңӣ� �գ֣ףأ� ��
������ ������ ������� ������ ���������� ��
���������� ���������� ���������� �����ĤƤ� �ʤˤ̤ͤ�
�ϤҤդؤ� �ޤߤ��� �䡡�桡�� ������ ��𡡤�� ��
���������� ���������� ���¤ŤǤ� �ФӤ֤٤� �ѤԤפڤ�
���������� �����á��� �㡡�塡�� ��
������
.DE
.RE
.sp
�������ʲ���ʸ�����ɤߤ���Ƭ�ˤʤ뤳�Ȥ�����ޤ���
.sp
.RS
.DS
�� �� ���������� �� ���� �� ����
.DE
.RE
.sp
�����ϥ��ե� JIS �ޤ��� EUC �����ɤǹ������졢���ĥʥ�ǽ�λ���Ƥ��ʤ����
�ʤ�ޤ���
�ޤ�����Ĺ���ϡ�����Ǥ� 32 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
�ʻ쥳���ɤϥإå��ե����� \fB/usr/include/sj3lib.h\fP ��
��������ʲ����ʻ�Τ����ΰ�ĤǤ���
.sp
.RS
.DS
SJ3_H_NRMNOUN   ����̾��        SJ3_H_ADJVERB   ����ư��
SJ3_H_PRONOUN   ��̾��          SJ3_H_SILVERB   ����ư��
SJ3_H_LNAME     �Ļ�            SJ3_H_ZILVERB   ����ư��
SJ3_H_FNAME     ̾��            SJ3_H_ONEVERB   ����ư��
SJ3_H_LOCNAME   ��̾            SJ3_H_KAVERB    ���Ը���ư��
SJ3_H_PREFIC    ������̾        SJ3_H_GAVERB    ���Ը���ư��
SJ3_H_RENTAI    Ϣ�λ�          SJ3_H_SAVERB    ���Ը���ư��
SJ3_H_CONJUNC   ��³��          SJ3_H_TAVERB    ���Ը���ư��
SJ3_H_SUBNUM    ������          SJ3_H_NAVERB    �ʹԸ���ư��
SJ3_H_NUMERAL   ����            SJ3_H_BAVERB    �йԸ���ư��
SJ3_H_PREFIX    ��Ƭ��          SJ3_H_MAVERB    �޹Ը���ư��
SJ3_H_POSTFIX   ������          SJ3_H_RAVERB    ��Ը���ư��
SJ3_H_ADVERB    ����            SJ3_H_WAVERB    ��Ը���ư��
SJ3_H_ADJECT    ���ƻ�          SJ3_H_SINGLE    ñ����
.DE
.RE
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
��Ͽ�Ǥ����Ȥ��ˤ� 0 ������Ͽ�Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ���
���顼���ơ��������֤���ޤ���
.sp
���顼���ơ������ϥإå��ե����� \fB/usr/include/sj3lib.h\fP ��
������졢�ܺ٤ϰʲ��ΤȤ���Ǥ���
.sp
.IP "\fBSJ3_DICT_ERROR\fP
����¸�ߤ��ʤ������뤤����Ͽ�Ǥ��ʤ�����Ǥ���
.IP "\fBSJ3_DICT_LOCKED\fP
�����ɤ߽Ф��Τ���˥��å�����Ƥ��ޤ���
.IP "\fBSJ3_BAD_YOMI_STR\fP
�ɤ�ʸ���������Ǥ���
.IP "\fBSJ3_BAD_KANJI_STR\fP
����ʸ���������Ǥ���
.IP "\fBSJ3_BAD_HINSI_CODE\fP
�ʻ쥳���ɤ������Ǥ���
.IP "\fBSJ3_WORD_EXIST\fP
���Ǥ���Ͽ�ѤǤ���
.IP "\fBSJ3_DOUON_FULL\fP
����ʾ�Ʊ�������Ͽ�Ǥ��ޤ���
.IP "\fBSJ3_DICT_FULL\fP
����ʾ弭�����Ͽ�Ǥ��ޤ���
.IP "\fBSJ3_INDEX_FULL\fP
����ʾ奤��ǥå�������Ͽ�Ǥ��ޤ���
.IP "\fBSJ3_TOUROKU_FAILED\fP
¾�θ����Ǽ���ؤ���Ͽ���Ԥ��ޤ�����
.RE
.RE

.NH
ñ����
.RS
.LP
.DS
sj3_syoukyo(yomi, kanji, hinsi)
sj3_syoukyo_euc(yomi, kanji, hinsi)
sj3_syoukyo_mb(yomi, kanji, hinsi)
unsigned char	*yomi;		/* �ɤ�ʸ����ؤΥݥ��� */
unsigned char	*kanji;		/* ����ʸ����ؤΥݥ��� */
int	hinsi;			/* �ʻ쥳���� */
.DE
.sp
�桼���������Ͽ����Ƥ���
�ɤ�ʸ����ؤΥݥ��� \fByomi\fP��
����ʸ����ؤΥݥ��� \fBkanji\fP��
�ʻ쥳���� \fBhinsi\fP �ǻ��ꤵ�줿���õ�ޤ���
.sp
�ɤߤϰʲ��Υ��ե� JIS �ޤ��� EUC �����ɤǹ������졢
���ĥʥ�ǽ�λ���Ƥ��ʤ���Фʤ�ޤ���
�ޤ�����Ĺ���ϡ�����Ǥ� 31 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
.RS
.DS
��������
���������� ����������
���£ãģ� �ƣǣȣɣ� �ˣ̣ͣΣ� �Уѣңӣ� �գ֣ףأ� ��
������ ������ ������� ������ ���������� ��
���������� ���������� ���������� �����ĤƤ� �ʤˤ̤ͤ�
�ϤҤդؤ� �ޤߤ��� �䡡�桡�� ������ ��𡡤�� ��
���������� ���������� ���¤ŤǤ� �ФӤ֤٤� �ѤԤפڤ�
���������� �����á��� �㡡�塡�� ��
������
.DE
.RE
.sp
�������ʲ���ʸ�����ɤߤ���Ƭ�ˤʤ뤳�Ȥ�����ޤ���
.sp
.RS
.DS
�� �� ���������� �� ���� �� ����
.DE
.RE
.sp
�����ϥ��ե� JIS �ޤ��� EUC �����ɤǹ������졢���ĥʥ�ǽ�λ���Ƥ��ʤ����
�ʤ�ޤ���
�ޤ�����Ĺ���ϡ�����Ǥ� 32 ʸ����ۤ��ƤϤʤ�ޤ���
.sp
�ʻ쥳���ɤ� \fB/usr/include/sj3lib.h\fP ����������ʲ����ʻ��
�����ΰ�ĤǤ���
.sp
.RS
.DS
SJ3_H_NRMNOUN   ����̾��        SJ3_H_ADJVERB   ����ư��
SJ3_H_PRONOUN   ��̾��          SJ3_H_SILVERB   ����ư��
SJ3_H_LNAME     �Ļ�            SJ3_H_ZILVERB   ����ư��
SJ3_H_FNAME     ̾��            SJ3_H_ONEVERB   ����ư��
SJ3_H_LOCNAME   ��̾            SJ3_H_KAVERB    ���Ը���ư��
SJ3_H_PREFIC    ������̾        SJ3_H_GAVERB    ���Ը���ư��
SJ3_H_RENTAI    Ϣ�λ�          SJ3_H_SAVERB    ���Ը���ư��
SJ3_H_CONJUNC   ��³��          SJ3_H_TAVERB    ���Ը���ư��
SJ3_H_SUBNUM    ������          SJ3_H_NAVERB    �ʹԸ���ư��
SJ3_H_NUMERAL   ����            SJ3_H_BAVERB    �йԸ���ư��
SJ3_H_PREFIX    ��Ƭ��          SJ3_H_MAVERB    �޹Ը���ư��
SJ3_H_POSTFIX   ������          SJ3_H_RAVERB    ��Ը���ư��
SJ3_H_ADVERB    ����            SJ3_H_WAVERB    ��Ը���ư��
SJ3_H_ADJECT    ���ƻ�          SJ3_H_SINGLE    ñ����
.DE
.RE
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
����Ǥ����Ȥ��ˤ� 0 ��������Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ���
���顼���ơ��������֤���ޤ���
.sp
���顼���ơ������� \fB/usr/include/sj3lib.h\fP ��������졢
�ܺ٤ϰʲ��ΤȤ���Ǥ���
.sp
.IP "\fBSJ3_DICT_ERROR\fP
����¸�ߤ��ʤ������뤤����Ͽ�Ǥ��ʤ�����Ǥ���
.IP "\fBSJ3_DICT_LOCKED\fP
�����ɤ߽Ф��Τ���˥��å�����Ƥ��ޤ���
.IP "\fBSJ3_BAD_YOMI_STR\fP
�ɤ�ʸ���������Ǥ���
.IP "\fBSJ3_BAD_KANJI_STR\fP
����ʸ���������Ǥ���
.IP "\fBSJ3_BAD_HINSI_CODE\fP
�ʻ쥳���ɤ������Ǥ���
.IP "\fBSJ3_WORD_NOT_EXIST\fP
���ꤵ�줿�����Ͽ����Ƥ��ޤ���
.IP "\fBSJ3_SYOUKYO_FAILED\fP
¾�θ����Ǽ��񤫤�ξõ���Ԥ��ޤ�����
.RE
.RE

.NH
����Υ��å�
.RS
.LP
.DS
sj3_lockserv()
.DE
.sp
���ߥ����ץ󤵤�Ƥ��뼭���ñ�����Ͽ������Ǥ��ʤ��褦�˥��å����ޤ���
.sp
�ؽ��ɣĤ�ñ��μ�����Ǥΰ��֤򼨤��Ƥ��ޤ������Τ��ᡢ���饤����Ȥ�
�ؽ��ɣĤ��ݻ����Ƥ���֤˼�����Ͽ��������Ԥ���ȡ����饤����Ȥ�
�ݻ����Ƥ���ؽ��ɣĤȼ���δ֤�������򵯤�����ǽ��������ޤ���
������򤱤뤿��ˡ����饤����Ȥ��ؽ��ɣĤ��ݻ����Ƥ���֤ϼ���ե������
���å�����ɬ�פ�����ޤ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
���å��Ǥ����Ȥ��ˤ� 0 ����
���å��Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ����ͤ��֤���ޤ���
.RE

.NH
����Υ�����å�
.RS
.LP
.DS
sj3_unlockserv()
.DE
.sp
sj3_lockserv �ؿ��ǥ��å�����Ƥ��뼭���ñ�����Ͽ������Ǥ���褦��
������å����ޤ���
.sp
�ؽ��ɣĤ�ñ��μ�����Ǥΰ��֤򼨤��Ƥ��ޤ������Τ��ᡢ���饤����Ȥ�
�ؽ��ɣĤ��ݻ����Ƥ���֤˼�����Ͽ��������Ԥ���ȡ����饤����Ȥ�
�ݻ����Ƥ���ؽ��ɣĤȼ���δ֤�������򵯤�����ǽ��������ޤ���
������򤱤뤿��ˡ����饤����Ȥ��ؽ��ɣĤ��ݻ����Ƥ���֤ϼ���ե������
���å�����ɬ�פ�����ޤ���
.br
���饤����Ȥ��������å����Ƥ���֤ϡ�������Ͽ��������뤳�Ȥ��Ǥ��ޤ���
���饤����Ȥϳؽ��ɣĤ��ݻ�����ɬ�פ��ʤ��ʤä��Ȥ��ˤϡ��������˼����
������å�����٤��Ǥ���
.sp
�����Ф���³����Ƥ��ʤ��Ȥ������뤤�ϥ����Ф������󤷤��Ȥ��ˤ� \-1 ��
�֤���ޤ���
.br
������å��Ǥ����Ȥ��ˤ� 0 ����
������å��Ǥ��ʤ��ä��Ȥ��ˤϤ���ʳ����ͤ��֤���ޤ���
.RE