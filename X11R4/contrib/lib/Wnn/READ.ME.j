
��	�ף�̾�����Ѵ������ƥ� �С������4.0

�ף�̾�����Ѵ������ƥ�ϡ�������ؿ������ϸ���ꡢΩ���ŵ�������ҡ�
������ҥ����ƥå��Σ��Ԥˤ�äƳ�ȯ���줿�ͥåȥ����̾�����Ѵ�����
�ƥ�Ǥ���

����ϡ��ף�̾�����Ѵ������ƥ�ΥС������4.0�Ǥ����С�����󣳰���
�Υ����ƥ�Ȥϡ������˰ۤʤäƤ��ޤ������ۤξܺ٤ˤĤ��Ƥϡ�manual/intro
���ɤ�ǲ�������

����Υ�꡼���Ǥϡ��ʲ��ε�����ư���ǧ����Ƥ��ޤ���

SUN3 SUN4 �� OS3.4, OS4.0

OMRON SX9100 �� UniOS-B (4.3BSD)
OMRON SX9100 �� UniOS-U (SystemV R2.1) (DT DS M90)




��	���󥹥ȡ���

 �������ե�����ˤϡ���ƥ��ʸ���󡢤���ӡ������Ȥ���ECU�����ɤǽ񤫤��
 ���ޤ�������ѥ���κݤˤϡ�8�ӥåȤ��̤륳��ѥ���ǡ�make ���Ʋ�������

 ��Makefile �ǡ�make

 ����ѥ�����ˤϡ��ȥåץ�٥�� MAKEFILE ��񤭴����Ʋ�������

 SUN �Ѥˤϡ�(Makefile.sun)
 DEFS = DEFS='-O -SUB'
 LIBS = LIB_TERM='-ltermcap'
 GLIB = GETOPTLIB=

 LUNA BSD �Ѥˤϡ�(Makefile.luna)
 DEFS = DEFS='-O -20 -f -DSX9100BSD'
 LIBS = LIB_TERM='-ltermcap'
 GLIB = GETOPTLIB='../jlib/libgetopt.a'

 LUNA SYSV �Ѥˤϡ�(Makefile.sx)
 DEFS = DEFS='-O -Zs -DSX8870'
 LIBS = LIB_TERM='-lcurses'
 GLIB = GETOPTLIB=
 �Ȥ��Ʋ�������

 �ޤ���wchar_t �� typedef ����Ƥ���ޥ���Ǥϡ�DEFS �ˡ�-DWCHAR_T ��ä��Ƥ���������(NEC��EWS4800, Toshiba AS3000�ʤ�)

 
 �ȥåץ�٥�� Makefile �� BINDIR, LIBDIR, PUBDIC_DIR �Ȥ�������ޤ�������ϡ�
 jserver, uum, ����桼�ƥ���ƥ��ʤɤΥ��ޥ��(BINDIR)������ե�����(LIBDIR)��
 �У������(PUBDIC_DIR)�������󥹥ȡ��뤵���ǥ��쥯�ȥ꡼�Ǥ���
 ��������LIBDIR �ϡ�include/config.h �ˡ��ѥ�̾���񤫤�Ƥ��ޤ��Τǡ����������
 ���Ƥ�ư���ޤ���
 �ǥե���ȤǤϡ����줾�졢/usr/local/bin/Wnn4, /usr/local/lib/wnn,
 /usr/local/lib/wnn/dic/pubdic �ˤʤäƤ��ޤ���

 jserver/Makefile �ˤϡ�JSERVER_OWNER �Ȥ����ѿ�������ޤ�������ϡ�
 jserver ��ɤΥ桼���θ��¤�ư���������ꤷ�ޤ����С������3�ޤǤǤϡ�
 jserver �ϡ�root �θ��¤�ư�����Ƥ����ΤǤ�����root ��ư�����Τϴ�����
 ���⤤���ᡢwnn �ѤΥ桼�����äơ����θ��¤� jserver ������ư������
 �Ȥ�侩���ޤ���
 �ǥե���ȤǤϡ�wnn �ˤʤäƤ��ޤ���

 jserver �ϡ�JSERVER_OWNER �Υ����ʤǡ�s�ӥåȤ�Ω�äƤ���ɬ�פ�����ޤ���
 make ����� make install �ϡ�JSERVER_OWNER �ǡ��¹Ԥ���Τ������Ǥ��礦��

 �ޤ���jslib �β��ˡ�WNNLIBDIR��WNNINCDIR �Ȥ��ä��ѿ�������ޤ�������
 ��ϡ����줾�졢libwnn.a �����󥯥롼�ɥե�����򥤥󥹥ȡ��뤹��ǥ�
 �쥯�ȥ����ꤷ�ޤ������Τ褦�ˡ����󥹥ȡ����������ϡ����줾��Υ�
 �٥��Makefile �˻���ФäƤ��ޤ��Τǡ��ܤ��̤��Ʋ�������

 uum �ϡ������ʤ� root �ǡ� s�ӥåȤ�Ω�äƤ���ɬ�פ�����ޤ���

 make install �򡢼¹Ԥ���ȡ����󥹥ȡ����ɬ�פʥǥ��쥯�ȥ꤬���ʤ����ϡ�
 �ǥ��쥯�ȥ����褦�˥�å�������Ф��ƽ�λ���ޤ��Τǡ���äƲ�������
 ��������¾���ǥ��쥯�ȥ�ˤϡ�write �ѡ��ߥå����ɬ�פǤ���

 �ޤ���uum �Υ����ʤ�root �ˤ��뤿��ˡ�root �Υѥ���ɤ�ʹ���Ƥ��ޤ��Τǡ�
 ���Ϥ��Ʋ�������

 make install �򤹤�ȡ�jserver, uum, ����桼�ƥ���ƥ��ʤɤΥ��ޥ�ɤϡ�
 /usr/local/bin/Wnn4/ �ˡ�
 ����ե�����ϡ�/usr/local/bin/wnn �ˡ�
 ����ϡ�/usr/local/lib/wnn/dic �ˡ����줾��Ǥ��ޤ���
 ���ޤǡ��ȤäƤ�����/usr/local/lib/dic, /usr/local/lib/jd �ϡ����Ѥ��ޤ���

 jserver �μ¹Ի��ˡ�/usr/local/lib/wnn/dic �� read/write �ѡ��ߥå����ɬ�פǤ���

 ����Ǥϡ��������ѥ��ˡ�/usr/local/bin/Wnn4 ���ɲä��ơ�jserver ����ӡ�uum 
 ��ȤäƤߤƲ�������

 ��Imakefile �ǡ�make

 X-Window �� Imake ���Ȥ���Ķ��Υ����ȤǤϡ���ǰ��դ���θ湥�դǡ�
 Imakefile �����Ѱդ���Ƥ��ޤ���
	    Special thanks to Mr.Inaba @ Toshiba

 Imake ��Ȥä� make ���뤿��ˤϡ��ǥ��쥯�ȥ� Wnn ��
 contrib/lib/Wnn ���֤��Ʋ�������

    �ȥåץ�٥�� WnnImake.defs �� �񤭴����Ʋ�������
    SUN �Ѥˤ�
    #defined WNNSUN 1

    LUNA BSD �Ѥˤ�
    #defined WNNSX9100BSD 1

    LUNA SYSV �Ѥˤ�
    #defined WNNSX8870 1 (�вٻ��Ϥ���ˤʤäƤ��ޤ���)
    
    ���θ塢�ȥåץ�٥��
    make Makefile Makefiles
    ��ԤʤäƲ�������

    LUNA SYSV���뤤��LUNA BSD �ξ�硢imake �δĶ����Ѱդ��Ƥ��ޤ��Τǡ�imake ��
    �¹Ԥ������˰ʲ��Τ��Ȥ�ԤʤäƲ�������
   (1)�ǥ��쥯�ȥ� Wnn/imake�β��ˡ�imake.patch ������ޤ��Τǡ����ꥸ�ʥ�� 
      imake.c �� �ѥå������ƤƲ����������θ塢imake ��᡼�����Ʋ�������
      �᡼�������硢LUNA SYSV �ʤ� Makefile.unU ��LUNA BSD �ʤ� Makefile.unB
      ��ȤäƲ�������
   (2)�ޤ���tmpl.patch �Ȥ����ե����뤬����ޤ��Τǡ����ꥸ�ʥ�� imake.tmpl ��
      �ѥå������ƤƲ�������
   (3)OMRON.macros �Ȥ����ե����뤬����ޤ��Τǡ�util/imake.includes �β��ˤ���
      �Ʋ�������

��	uum

uum �δ��ܥ��ڥ졼�����ϡ� �С������3�� wnn �Τ�Τ��礭���Ѥ�ä�
���ޤ��󡣤��������ޤ� wnnrc, wnnkey �ȸƤФ줿��Τϡ�uumrc, uumkey��
����̾�����ѹ��ˤʤ�ޤ������ޤ������ޤޤ� wnnrc �˽񤫤�Ƥ��������
�ɤ�����ϡ��Ѵ��˲�̾�����Ѵ��ʳ��˴�����̾�Ѵ�����̾�����޻��Ѵ��δ�
�����Ѥ��뤿��ˡ��̤Υե�����˽񤫤졢���Υե�����̾��uumrc ��
���ꤹ��褦�ˤʤäƤ��ޤ���(manual �򸫤Ʋ�������)

 �����������Х���ɤ��ѹ�������ޤ���

 �ǥե���ȤǤϡ�^W ���Ѵ�(�����ޤǤ�Ʊ��)�θ�(�Ѵ���⡼��)�ǡ�
 ^W �Ȥ���ȡ����ޤǤϡ������䤬�Фޤ�������V4 ����ϡ�
 ������Ȥʤ�ޤ���

 V3 ��������ϡ�^G �Ǥ�������^G �ˡ��Х���ɤ���Ƥ���Τϡ���������Ȥʤ�ޤ���

 �ɲä��줿��ǽ��

    ��ʸ��Υ��ݡ���

    ��ʸ��ϡ���ʸ�᤬�����Ĥ��Ĥʤ��ä���Τǡ�
    ��ʸ��ϡ����ޤǤ�Ʊ�ͤǤ���
    �㤨�С���Ƭ��+̾��+������ǡ���Ĥ���ʸ��ˤʤ�ޤ���

    ��ʸ��ϡ��ܡ����(�ܡ���ɤ򥵥ݡ��Ȥ��Ƥ��ʤ�ü�� gmw_shell, gmwterm
    �ʤɤǤϡ���������饤��)�ǡ�ɽ������ޤ���

    ��ʸ��ϡ����ޤ�Ʊ�͡���С���ɽ���Ǥ���

    ���ϥ⡼�ɤ��顢^W �Ȥ���ȡ����Ϥ��줿ʸ�����Ϣʸ���Ѵ����ޤ���
    ^Z �Ȥ���ȡ����Ϥ��줿ʸ�����1��ʸ��Ȥߤʤ����Ѵ����ޤ���
    ���Ϥ��줿ʸ�����1��ʸ��Ȥߤʤ����Ѵ����륭���ϡ��Х���ɤ��Ƥ��ޤ���

    �Ѵ���⡼�ɤǡ�

    ��ʸ���٥�����򥵥ݡ��Ȥ��Ƥ��Ĥ�Τˡ�
    Ϣʸ���Ѵ�(^W), ������(^N)/������(^P)/������(^W)��
    ʸ�῭�Ф������߸���Ѵ�(^W)�Ǥ���

    ��ʸ���٥���Ѵ��ϡ�
    ������(^Z), ʸ�῭�Ф������߸���Ѵ�(^Z)�Ǥ���
    ������/������ϡ�ɸ��Ǥϡ��Х���ɤ��Ƥ��ޤ���

    ���Ȥϡ����󥹥ڥ��Ȥ�����ޤ���
    �Ѵ���ˡ�^V �Ȥ���ȡ��Ѵ���̤ξܺ٤ʾ����Ф��ޤ���
    (�ʻ졢�������١�ɾ���ͤʤ�)
    ���θ�(���󥹥ڥ��ȥ⡼��)�ǡ�
    ^N, ^P ��ʸ��μ����䡢������
    ^U, ^D ����μ�Ω����Ѵ��˻��Ѥ��ʤ�/������Ǥ��ޤ���

    ^U, ^D �ϡ�ñ�측���Υ⡼�ɤ����Ȥ��ޤ���

    �ޤ����������/������󤫤�⡢����λ���/�Ի��ѡ�������Ǥ��ޤ���

    ������̾�Ѵ���
    ���ϥ⡼�ɤǡ�^C �Ȥ���ȡ����Ϥ��줿ʸ����������̾�Ѵ����ޤ���
    ���������Ϥϡ����åȥ���ɥڡ����Ȥ���uum �Υҥ��ȥ꤫����äƤ��ޤ���

    �Ѵ��⡼�ɤǡ�^C �ϡ����ޤ��̤ꡢ���ϥ⡼�ɤؤΰܹԤǤ���

    ��̾�����޻��Ѵ�
    ���ϥ⡼�ɤ��顢^R �Ȥ���ȡ�
    ���Ϥ��줿ʿ��̾���������ʤ򡢥����޻������뤤�ϡ�Ⱦ�ѥ������ʤ��Ѵ����ޤ���
    ����ϡ�ʸ�῭�Ф������ߤλ��ˤ�Ȥ��ޤ���

    ���줫��⤦��ġ�
    Ⱦ�����Ϥ��줿��ʸ�������Ѥ��Ѵ��������˴ؤ��Ƥϡ��������ˤ��Ѵ����ޤ���
    ���ɤ��С�1234 ���������Ȥ�ȡ�
    1234	1,234	���󻰻�
    ��������	����������
    ������ɴ������	��������������
    ������Ȥ��ƽФƤ��ޤ���

    ����ե��٥åȡ�����������λ�������ˡ�ľ���ˤɤ�����򤷤�����Ф��Ƥ��ơ�
    �����������Ȥ��ޤ���

    �ܤ����ϡ��ޥ˥奢��򸫤Ʋ�������



��	manual �ʳ��Υǥ��쥯�ȥ�ϡ��ʲ����̤�Ǥ���

conv
	����С��ȡ�����(�ե��󥯥���󥭡����Ǥ����������Ŭ���ʥ���
	�ɤ��Ѵ�����)�Υ�������¸�ߤ��ޤ���
etc
	ʣ���Υǥ��쥯�ȥ�Ƕ�ͭ����륽�����ե����뤬¸�ߤ��ޤ���
fuzokugo
	��°��ե���������ץ������(atof)������ӡ�pubdic ����°��ե����뤬
	¸�ߤ��ޤ���
include 
	�إå����ե����뤬¸�ߤ��ޤ���
jd
	uum, jserver, �����޻���̾�Ѵ��ʤɤν�����ե����롢
	�ʻ�ե����뤬¸�ߤ��ޤ������Υǥ��쥯�ȥ�����Ƥϡ�
	/usr/local/lib/wnn �Ȥ��ä��ǥ��쥯�ȥ�˥��󥹥ȡ��뤵��ޤ���

	    jserverrc - jserver ��Ω��������ե�����Ǥ���
	    uumrc	uum ��Ω��������ե�����Ǥ���
	    uumkey	uum �Υ����Х��������ե�����Ǥ���
	    wnnenvrc	uum ���ѻ��μ�����������ե�����Ǥ���
	jd �β��ˤϡ���������������Τ�Τ��Ѱդ���Ƥ��ޤ���
	���ߤ˹�碌�Ƹ����Ѳ�������

jlib
	�饤�֥��Υ�������¸�ߤ��ޤ���
jlib.V3
	�С������3���Ф���ߴ��饤�֥��Υ�������¸�ߤ��ޤ���
jserver
	jserver �Υ�������¸�ߤ��ޤ���
jutil
	����桼�ƥ���ƥ��Υ�������¸�ߤ��ޤ���
		atod	 ����κ���
		dtoa	 ����Υƥ����Ȥؤ��ѹ�
		oldatonewa �С�����󣳤ޤǤμ���Υƥ����ȷ�����
			�С�����󣴤Υƥ����ȷ������ѹ�
		wnnstat	 jserver �ξ���(�桼�����Ķ����ե����롢����)��Ĵ�٤롣
		wnnkill	 jserver ��λ�����롣
		wnntouch ����ե�����ʤɤΥե������FID �����ե�����Υإå���
			���äƤ����ΤȰۤʤ���˰��פ����롣
		wddel	ñ������Хå�Ū�˹Ԥʤ�
		wdreg	ñ����Ͽ��Хå�Ū�˹Ԥʤ�
pubdic 			
	pubdic �μ���¸�ߤ��ޤ���
	����ϡ� �հ��������Ǻ�äƤ���ޤ���
romkan
	�����޻���̾�Ѵ��Υ�������¸�ߤ��ޤ���
uum
	�ե���ȥ���ɡ��ץ����å� uum �Υ�������¸�ߤ��ޤ���
