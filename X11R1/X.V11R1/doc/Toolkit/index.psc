%!PS-Adobe-1.0
%%Creator: orpheus:mento (Al Mento,,,,)
%%Title: stdin (ditroff)
%%CreationDate: Mon Sep 14 23:29:07 1987
%%EndComments
% lib/psdit.pro -- prolog for psdit (ditroff) files
% Copyright (c) 1984, 1985 Adobe Systems Incorporated. All Rights Reserved.
% last edit: shore Sat Nov 23 20:28:03 1985
% RCSID: $Header: psdit.pro,v 2.1 85/11/24 12:19:43 shore Rel $

/$DITroff 140 dict def $DITroff begin
/fontnum 1 def /fontsize 10 def /fontheight 10 def /fontslant 0 def
/xi {0 72 11 mul translate 72 resolution div dup neg scale 0 0 moveto
  /fontnum 1 def /fontsize 10 def /fontheight 10 def /fontslant 0 def F
  /pagesave save def}def
/PB{save /psv exch def currentpoint translate 
  resolution 72 div dup neg scale 0 0 moveto}def
/PE{psv restore}def
/arctoobig 90 def /arctoosmall .05 def
/m1 matrix def /m2 matrix def /m3 matrix def /oldmat matrix def
/tan{dup sin exch cos div}def
/point{resolution 72 div mul}def
/dround	{transform round exch round exch itransform}def
/xT{/devname exch def}def
/xr{/mh exch def /my exch def /resolution exch def}def
/xp{}def
/xs{docsave restore end}def
/xt{}def
/xf{/fontname exch def /slotno exch def fontnames slotno get fontname eq not
 {fonts slotno fontname findfont put fontnames slotno fontname put}if}def
/xH{/fontheight exch def F}def
/xS{/fontslant exch def F}def
/s{/fontsize exch def /fontheight fontsize def F}def
/f{/fontnum exch def F}def
/F{fontheight 0 le {/fontheight fontsize def}if
   fonts fontnum get fontsize point 0 0 fontheight point neg 0 0 m1 astore
   fontslant 0 ne{1 0 fontslant tan 1 0 0 m2 astore m3 concatmatrix}if
   makefont setfont .04 fontsize point mul 0 dround pop setlinewidth}def
/X{exch currentpoint exch pop moveto show}def
/N{3 1 roll moveto show}def
/Y{exch currentpoint pop exch moveto show}def
/S{show}def
/ditpush{}def/ditpop{}def
/AX{3 -1 roll currentpoint exch pop moveto 0 exch ashow}def
/AN{4 2 roll moveto 0 exch ashow}def
/AY{3 -1 roll currentpoint pop exch moveto 0 exch ashow}def
/AS{0 exch ashow}def
/MX{currentpoint exch pop moveto}def
/MY{currentpoint pop exch moveto}def
/MXY{moveto}def
/cb{pop}def	% action on unknown char -- nothing for now
/n{}def/w{}def
/p{pop showpage pagesave restore /pagesave save def}def
/abspoint{currentpoint exch pop add exch currentpoint pop add exch}def
/distance{dup mul exch dup mul add sqrt}def
/dstroke{currentpoint stroke moveto}def
/Dl{2 copy gsave rlineto stroke grestore rmoveto}def
/arcellipse{/diamv exch def /diamh exch def oldmat currentmatrix pop
 currentpoint translate 1 diamv diamh div scale /rad diamh 2 div def
 currentpoint exch rad add exch rad -180 180 arc oldmat setmatrix}def
/Dc{dup arcellipse dstroke}def
/De{arcellipse dstroke}def
/Da{/endv exch def /endh exch def /centerv exch def /centerh exch def
 /cradius centerv centerv mul centerh centerh mul add sqrt def
 /eradius endv endv mul endh endh mul add sqrt def
 /endang endv endh atan def
 /startang centerv neg centerh neg atan def
 /sweep startang endang sub dup 0 lt{360 add}if def
 sweep arctoobig gt
 {/midang startang sweep 2 div sub def /midrad cradius eradius add 2 div def
  /midh midang cos midrad mul def /midv midang sin midrad mul def
  midh neg midv neg endh endv centerh centerv midh midv Da
  currentpoint moveto Da}
 {sweep arctoosmall ge
  {/controldelt 1 sweep 2 div cos sub 3 sweep 2 div sin mul div 4 mul def
  centerv neg controldelt mul centerh controldelt mul
  endv neg controldelt mul centerh add endh add
  endh controldelt mul centerv add endv add
  centerh endh add centerv endv add rcurveto dstroke}
 {centerh endh add centerv endv add rlineto dstroke}ifelse}ifelse}def

/Barray 200 array def % 200 values in a wiggle
/D~{mark}def
/D~~{counttomark Barray exch 0 exch getinterval astore /Bcontrol exch def pop
 /Blen Bcontrol length def Blen 4 ge Blen 2 mod 0 eq and
 {Bcontrol 0 get Bcontrol 1 get abspoint /Ycont exch def /Xcont exch def
  Bcontrol 0 2 copy get 2 mul put Bcontrol 1 2 copy get 2 mul put
  Bcontrol Blen 2 sub 2 copy get 2 mul put
  Bcontrol Blen 1 sub 2 copy get 2 mul put
  /Ybi /Xbi currentpoint 3 1 roll def def 0 2 Blen 4 sub
  {/i exch def
   Bcontrol i get 3 div Bcontrol i 1 add get 3 div
   Bcontrol i get 3 mul Bcontrol i 2 add get add 6 div
   Bcontrol i 1 add get 3 mul Bcontrol i 3 add get add 6 div
   /Xbi Xcont Bcontrol i 2 add get 2 div add def
   /Ybi Ycont Bcontrol i 3 add get 2 div add def
   /Xcont Xcont Bcontrol i 2 add get add def
   /Ycont Ycont Bcontrol i 3 add get add def
   Xbi currentpoint pop sub Ybi currentpoint exch pop sub rcurveto
  }for dstroke}if}def
end
/ditstart{$DITroff begin
 /nfonts 60 def			% NFONTS makedev/ditroff dependent!
 /fonts[nfonts{0}repeat]def
 /fontnames[nfonts{()}repeat]def
/docsave save def
}def

% character outcalls
/oc {/pswid exch def /cc exch def /name exch def
   /ditwid pswid fontsize mul resolution mul 72000 div def
   /ditsiz fontsize resolution mul 72 div def
   ocprocs name known{ocprocs name get exec}{name cb}
   ifelse}def
/fractm [.65 0 0 .6 0 0] def
/fraction
 {/fden exch def /fnum exch def gsave /cf currentfont def
  cf fractm makefont setfont 0 .3 dm 2 copy neg rmoveto
  fnum show rmoveto currentfont cf setfont(\244)show setfont fden show 
  grestore ditwid 0 rmoveto} def
/oce {grestore ditwid 0 rmoveto}def
/dm {ditsiz mul}def
/ocprocs 50 dict def ocprocs begin
(14){(1)(4)fraction}def
(12){(1)(2)fraction}def
(34){(3)(4)fraction}def
(13){(1)(3)fraction}def
(23){(2)(3)fraction}def
(18){(1)(8)fraction}def
(38){(3)(8)fraction}def
(58){(5)(8)fraction}def
(78){(7)(8)fraction}def
(sr){gsave 0 .06 dm rmoveto(\326)show oce}def
(is){gsave 0 .15 dm rmoveto(\362)show oce}def
(->){gsave 0 .02 dm rmoveto(\256)show oce}def
(<-){gsave 0 .02 dm rmoveto(\254)show oce}def
(==){gsave 0 .05 dm rmoveto(\272)show oce}def
end

% an attempt at a PostScript FONT to implement ditroff special chars
% this will enable us to 
%	cache the little buggers
%	generate faster, more compact PS out of psdit
%	confuse everyone (including myself)!
50 dict dup begin
/FontType 3 def
/FontName /DIThacks def
/FontMatrix [.001 0 0 .001 0 0] def
/FontBBox [-260 -260 900 900] def% a lie but ...
/Encoding 256 array def
0 1 255{Encoding exch /.notdef put}for
Encoding
 dup 8#040/space put %space
 dup 8#110/rc put %right ceil
 dup 8#111/lt put %left  top curl
 dup 8#112/bv put %bold vert
 dup 8#113/lk put %left  mid curl
 dup 8#114/lb put %left  bot curl
 dup 8#115/rt put %right top curl
 dup 8#116/rk put %right mid curl
 dup 8#117/rb put %right bot curl
 dup 8#120/rf put %right floor
 dup 8#121/lf put %left  floor
 dup 8#122/lc put %left  ceil
 dup 8#140/sq put %square
 dup 8#141/bx put %box
 dup 8#142/ci put %circle
 dup 8#143/br put %box rule
 dup 8#144/rn put %root extender
 dup 8#145/vr put %vertical rule
 dup 8#146/ob put %outline bullet
 dup 8#147/bu put %bullet
 dup 8#150/ru put %rule
 dup 8#151/ul put %underline
 pop
/DITfd 100 dict def
/BuildChar{0 begin
 /cc exch def /fd exch def
 /charname fd /Encoding get cc get def
 /charwid fd /Metrics get charname get def
 /charproc fd /CharProcs get charname get def
 charwid 0 fd /FontBBox get aload pop setcachedevice
 2 setlinejoin 40 setlinewidth
 newpath 0 0 moveto gsave charproc grestore
 end}def
/BuildChar load 0 DITfd put
%/UniqueID 5 def
/CharProcs 50 dict def
CharProcs begin
/space{}def
/.notdef{}def
/ru{500 0 rls}def
/rn{0 840 moveto 500 0 rls}def
/vr{0 800 moveto 0 -770 rls}def
/bv{0 800 moveto 0 -1000 rls}def
/br{0 750 moveto 0 -1000 rls}def
/ul{0 -140 moveto 500 0 rls}def
/ob{200 250 rmoveto currentpoint newpath 200 0 360 arc closepath stroke}def
/bu{200 250 rmoveto currentpoint newpath 200 0 360 arc closepath fill}def
/sq{80 0 rmoveto currentpoint dround newpath moveto
    640 0 rlineto 0 640 rlineto -640 0 rlineto closepath stroke}def
/bx{80 0 rmoveto currentpoint dround newpath moveto
    640 0 rlineto 0 640 rlineto -640 0 rlineto closepath fill}def
/ci{500 360 rmoveto currentpoint newpath 333 0 360 arc
    50 setlinewidth stroke}def

/lt{0 -200 moveto 0 550 rlineto currx 800 2cx s4 add exch s4 a4p stroke}def
/lb{0 800 moveto 0 -550 rlineto currx -200 2cx s4 add exch s4 a4p stroke}def
/rt{0 -200 moveto 0 550 rlineto currx 800 2cx s4 sub exch s4 a4p stroke}def
/rb{0 800 moveto 0 -500 rlineto currx -200 2cx s4 sub exch s4 a4p stroke}def
/lk{0 800 moveto 0 300 -300 300 s4 arcto pop pop 1000 sub
    0 300 4 2 roll s4 a4p 0 -200 lineto stroke}def
/rk{0 800 moveto 0 300 s2 300 s4 arcto pop pop 1000 sub
    0 300 4 2 roll s4 a4p 0 -200 lineto stroke}def
/lf{0 800 moveto 0 -1000 rlineto s4 0 rls}def
/rf{0 800 moveto 0 -1000 rlineto s4 neg 0 rls}def
/lc{0 -200 moveto 0 1000 rlineto s4 0 rls}def
/rc{0 -200 moveto 0 1000 rlineto s4 neg 0 rls}def
end

/Metrics 50 dict def Metrics begin
/.notdef 0 def
/space 500 def
/ru 500 def
/br 0 def
/lt 416 def
/lb 416 def
/rt 416 def
/rb 416 def
/lk 416 def
/rk 416 def
/rc 416 def
/lc 416 def
/rf 416 def
/lf 416 def
/bv 416 def
/ob 350 def
/bu 350 def
/ci 750 def
/bx 750 def
/sq 750 def
/rn 500 def
/ul 500 def
/vr 0 def
end

DITfd begin
/s2 500 def /s4 250 def /s3 333 def
/a4p{arcto pop pop pop pop}def
/2cx{2 copy exch}def
/rls{rlineto stroke}def
/currx{currentpoint pop}def
/dround{transform round exch round exch itransform} def
end
end
/DIThacks exch definefont pop
ditstart
(psc)xT
576 1 1 xr
1(Times-Roman)xf 1 f
2(Times-Italic)xf 2 f
3(Times-Bold)xf 3 f
4(Times-BoldItalic)xf 4 f
5(Helvetica)xf 5 f
6(Helvetica-Bold)xf 6 f
7(Courier)xf 7 f
8(Courier-Bold)xf 8 f
9(Symbol)xf 9 f
10(DIThacks)xf 10 f
10 s
1 f
xi
%%EndProlog

%%Page: 69 1
10 s 0 xH 0 xS 1 f
3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
18 s
2140 834(Index)N
3 f
12 s
576 1086(<)N
1 f
10 s
576 1266(<X/Atoms.h>,)N
1059(2)X
576 1356(<X/Intrinsic.h>,)N
1113(2)X
576 1446(<X/TopLevel.h>,)N
1158(2)X
576 1536(<X/X.h>,)N
904(40)X
576 1626(<X/Xatoms.h>,)N
1095(2)X
576 1716(<X/Xlib.h>,)N
988(2)X
3 f
12 s
576 1896(A)N
1 f
10 s
576 2076(Action)N
814(Table,)X
1037(48)X
576 2166(Application)N
974(Programmer,)X
1415(2)X
3 f
12 s
576 2346(B)N
1 f
10 s
576 2526(Background,)N
1008(44)X
3 f
12 s
576 2706(C)N
1 f
10 s
576 2886(Chaining,)N
909(14,)X
1029(17,)X
1149(45)X
576 2976(Class,)N
789(2)X
576 3066(Class)N
769(Initialization,)X
1218(12)X
576 3156(Client,)N
811(2)X
576 3246(ClientMessage,)N
1092(31)X
576 3336(CompositeClassRec,)N
1260(6)X
576 3426(CompositePart,)N
1091(5)X
576 3516(CompositeWidget,)N
1198(6)X
576 3606(CompositeWidgetClass,)N
1371(6,)X
1451(19)X
576 3696(Con\256gure)N
916(Window,)X
1232(38)X
576 3786(Con\256gureNotify,)N
1145(16)X
576 3876(ConstraintClassRec,)N
1247(7)X
576 3966(ConstraintPart,)N
1078(7)X
576 4056(ConstraintWidgetClass,)N
1358(7)X
576 4146(ContraintWidgetClass,)N
1327(19)X
576 4236(Core,)N
772(19,)X
892(21)X
576 4326(CoreClassPart,)N
1074(4)X
576 4416(CorePart,)N
901(3)X
3 f
12 s
576 4596(D)N
1 f
10 s
576 4776(Destroy)N
850(Callbacks,)X
1206(20,)X
1326(21)X
3 f
12 s
576 4956(E)N
1 f
10 s
576 5136(Events,)N
834(33)X
3 f
12 s
576 5316(F)N
1 f
10 s
2384 1086(Foreground,)N
2798(44)X
3 f
12 s
2384 1266(G)N
1 f
10 s
2384 1446(Geometry)N
2725(Management,)X
3184(38,)X
3304(47)X
2384 1536(Grabbing)N
2707(Input,)X
2916(35)X
3 f
12 s
2384 1716(I)N
1 f
10 s
2384 1896(Inheritance,)N
2786(14,)X
2906(17,)X
3026(19,)X
3146(45)X
2384 1986(Initialization,)N
2833(12,)X
2953(17)X
2384 2076(Input)N
2573(Grabbing,)X
2916(35)X
3 f
12 s
2384 2256(M)N
1 f
10 s
2384 2436(Macros:)N
2444 2526(XtDisplay,)N
2813(16)X
2444 2616(XtScreen,)N
2783(16)X
2444 2706(XtWindow,)N
2840(16)X
3 f
12 s
2384 2886(N)N
1 f
10 s
2384 3066(Name,)N
2616(2)X
3 f
12 s
2384 3246(R)N
1 f
10 s
2384 3426(Resource)N
2703(Management,)X
3162(44)X
3 f
12 s
2384 3606(S)N
1 f
10 s
2384 3786(SubstructureNotify,)N
3038(16)X
2384 3876(Superclass)N
2747(Chaining,)X
3080(14,)X
3200(17,)X
3320(45)X
3 f
12 s
2384 4056(T)N
1 f
10 s
2384 4236(Toolkit,)N
2659(2)X
2384 4326(Translation)N
2769(Table,)X
2992(48)X
3 f
12 s
2384 4506(V)N
1 f
10 s
2384 4686(Visibility,)N
2725(38)X
2384 4776(Visible,)N
2655(38)X
3 f
12 s
2384 4956(W)N
1 f
10 s
2384 5136(Widget,)N
2660(2,)X
2740(3,)X
2820(5)X
2384 5226(Widget)N
2640(Classes,)X
2920(8)X
2384 5316(Widget)N
2640(Programmer,)X
3081(2)X
2384 5406(WidgetClass,)N
2833(5)X
3 f
2264 6048(69)N

70 p
%%Page: 70 2
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(WidgetClassRec,)N
1150(5)X
3 f
12 s
576 942(X)N
1 f
10 s
576 1122(XCon\256gureWindow,)N
1270(16,)X
1390(27,)X
1510(40,)X
1630(42)X
576 1212(XCreateGC,)N
995(49)X
576 1302(XCreateWindow,)N
1160(19)X
576 1392(XDestroyWindow,)N
1204(21)X
576 1482(XFreeGC,)N
928(21)X
576 1572(XFreePixmap,)N
1061(21)X
576 1662(XMapSubwindows,)N
1234(18)X
576 1752(XMoveWindow,)N
1137(42)X
576 1842(XNextEvent,)N
1017(33)X
576 1932(XPeekEvent,)N
1017(33)X
576 2022(XPending,)N
936(33)X
576 2112(XResizeWindow,)N
1164(40)X
576 2202(XSelectInput,)N
1039(35)X
576 2292(XSetInput,)N
945(43)X
576 2382(XSetWindowAttributes,)N
1372(18,)X
1492(19)X
576 2472(XrmParseCommand,)N
1270(29)X
576 2562(XtActionList,)N
1038(49)X
576 2652(XtActionsRec,)N
1070(49)X
576 2742(XtAdd/RemoveChildren,)N
1403(18)X
576 2832(XtAddCallback,)N
1119(20,)X
1239(22)X
576 2922(XtAddEventHandler,)N
1280(21,)X
1400(34,)X
1520(35,)X
1640(37)X
576 3012(XtAddGrab,)N
995(36)X
576 3102(XtAddInput,)N
1003(31)X
576 3192(XtAddTimeOut,)N
1123(21,)X
1243(32)X
576 3282(XtArgsProc,)N
999(25)X
576 3372(XtButtonBoxAddButton,)N
1401(8)X
576 3462(XtButtonBoxDeleteButton,)N
1473(8)X
576 3552(XtCallCallback,)N
1114(23)X
576 3642(XtCallCallbacks,)N
1145(22)X
576 3732(XtCallbackKind,)N
1141(22)X
576 3822(XtCallbackList,)N
1105(21)X
576 3912(XtCallbackProc,)N
1128(21)X
576 4002(XtCalloc,)N
905(21,)X
1025(30)X
576 4092(XtCheckSubclass,)N
1181(14)X
576 4182(XtClass,)N
869(13)X
576 4272(XtCreateLabel,)N
1089(8)X
576 4362(XtCreateWidget,)N
1154(3,)X
1246(8,)X
1338(12,)X
1469(16,)X
1600(17,)X
1731(18,)X
1862(24,)X
1993(25,)X
2124(26,)X
576 4452(27,)N
696(43,)X
816(45,)X
936(48)X
576 4542(XtCreateWindow,)N
1182(16,)X
1302(19)X
576 4632(XtCreateWindows,)N
1213(19)X
576 4722(XtDestroyGC,)N
1061(21,)X
1181(49)X
576 4812(XtDestroyPixmap,)N
1194(21)X
576 4902(XtDestroyWidget,)N
1186(15,)X
1306(18,)X
1426(20,)X
1546(21,)X
1666(24,)X
1786(26)X
576 4992(XtDispatchEvent,)N
1168(36,)X
1288(37)X
576 5082(XtDisplay,)N
945(16)X
576 5172(XtEventHandler,)N
1142(34)X
576 5262(XtExposeProc,)N
1079(38)X
576 5352(XtFree,)N
839(21,)X
959(30)X
576 5442(XtGeometryAlmost,)N
1252(39,)X
1372(41,)X
1492(42,)X
1612(47)X
576 5532(XtGeometryHandler,)N
1276(41)X
576 5622(XtGeometryMask,)N
1195(39)X
576 5712(XtGeometryNo,)N
1115(40,)X
1235(41,)X
1355(47)X
576 5802(XtGeometryResult,)N
1221(39)X
2384 762(XtGeometryYes,)N
2950(40,)X
3070(41,)X
3190(42)X
2384 852(XtGetGC,)N
2731(21,)X
2851(49)X
2384 942(XtGetPixmap,)N
2864(21)X
2384 1032(XtGetSubresources,)N
3048(45)X
2384 1122(XtGetValues,)N
2843(43,)X
2963(45,)X
3083(46)X
2384 1212(XtInitProc,)N
2762(17)X
2384 1302(XtInitialize,)N
2789(15)X
2404 1662(XtInitialize,)N
2809(29)X
3 f
12 s
2384 1842(X)N
1 f
10 s
2384 2022(XtInitialize,)N
2789(29)X
2384 2112(XtIsComposite,)N
2908(14,)X
3028(25)X
2384 2202(XtIsRealized,)N
2843(20)X
2384 2292(XtIsSubclass,)N
2842(13,)X
2962(25)X
2384 2382(XtLabelCreate,)N
2897(8)X
2384 2472(XtMainLoop,)N
2842(31,)X
2962(37)X
2384 2562(XtMakeGeometryRequest,)N
3266(15,)X
3386(39,)X
3506(40,)X
3626(41,)X
3746(43)X
2384 2652(XtMakeResizeRequest,)N
3159(40)X
2384 2742(XtMalloc,)N
2731(21,)X
2851(30)X
2384 2832(XtManageChild,)N
2940(15,)X
3060(25,)X
3180(26,)X
3300(27)X
2384 2922(XtManageChildren,)N
3043(8,)X
3123(24,)X
3243(26,)X
3363(27)X
2384 3012(XtMoveWidget,)N
2927(27,)X
3047(39,)X
3167(41,)X
3287(42)X
2384 3102(XtNewCallbackKind,)N
3101(22)X
2384 3192(XtNextEvent,)N
2847(21,)X
2967(34,)X
3087(37)X
2384 3282(XtNumber,)N
2767(44)X
2384 3372(XtOffset,)N
2705(43,)X
2825(44)X
2384 3462(XtOrderProc,)N
2839(26)X
2384 3552(XtPeekEvent,)N
2847(34,)X
2967(37)X
2384 3642(XtPending,)N
2766(33)X
2384 3732(XtProc,)N
2651(12)X
2384 3822(XtRealizeProc,)N
2892(18)X
2384 3912(XtRealizeWidget,)N
2981(15,)X
3101(16,)X
3221(18,)X
3341(19,)X
3461(27)X
2404 4272(XtRealloc,)N
2769(30)X
3 f
12 s
2384 4452(X)N
1 f
10 s
2384 4632(XtRealloc,)N
2749(30)X
2384 4722(XtRemoveAllCallbacks,)N
3189(21,)X
3309(23)X
2384 4812(XtRemoveCallback,)N
3056(20,)X
3176(22,)X
3296(23)X
2384 4902(XtRemoveEventHandler,)N
3217(21,)X
3337(35)X
2384 4992(XtRemoveGrab,)N
2932(37)X
2384 5082(XtRemoveInput,)N
2940(31)X
2384 5172(XtRemoveTimeOut,)N
3060(21,)X
3180(32)X
2384 5262(XtResizeRequest,)N
2976(41)X
2384 5352(XtResizeWidget,)N
2954(27,)X
3074(39,)X
3194(42)X
2384 5442(XtResource,)N
2803(44)X
2384 5532(XtResourceList,)N
2927(8,)X
3007(44)X
2384 5622(XtScreen,)N
2723(16)X
2384 5712(XtSetMappedWhenManaged,)N
3360(24,)X
3480(28)X
2384 5802(XtSetSensitive,)N
2899(33)X
3 f
2264 6048(70)N

71 p
%%Page: 71 3
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(XtSetValues,)N
1021(16,)X
1141(33,)X
1261(39,)X
1381(43,)X
1501(45,)X
1621(46,)X
1741(47,)X
1861(48)X
576 852(XtSetValuesGeometryRequest,)N
1600(47)X
576 942(XtSetValuesProc,)N
1168(47)X
576 1032(XtStringToClass,)N
1153(24)X
576 1122(XtStringToWidget,)N
1216(23)X
576 1212(XtSuperclass,)N
1039(13)X
576 1302(XtUnmanageChild,)N
1221(28)X
576 1392(XtUnmanageChildren,)N
1324(8,)X
1404(24,)X
1524(27,)X
1644(28)X
576 1482(XtWidgetGeometry,)N
1253(39,)X
1373(40)X
576 1572(XtWidgetProc,)N
1079(26)X
576 1662(XtWindow,)N
972(16)X
3 f
12 s
576 1842(_)N
1 f
10 s
576 2022(_XtGetResources,)N
1182(15)X
3 f
12 s
576 2202(a)N
1 f
10 s
576 2382(accept_focus)N
1016(procedure,)X
1378(43)X
3 f
12 s
576 2562(c)N
1 f
10 s
576 2742(calloc,)N
808(30)X
576 2832(change_managed)N
1154(procedure,)X
1516(26)X
576 2922(class_initialize)N
1072(procedure,)X
1434(12)X
576 3012(compositeWidgetClass,)N
1354(6)X
576 3102(compress_expose)N
1162(\256eld,)X
1344(38)X
576 3192(compress_motion,)N
1185(32)X
576 3282(constraintWidgetClass,)N
1341(7)X
3 f
12 s
576 3462(d)N
1 f
10 s
576 3642(delete_child)N
988(procedure,)X
1350(26)X
3 f
12 s
576 3822(e)N
1 f
10 s
576 4002(expose)N
819(procedure,)X
1181(38)X
3 f
12 s
576 4182(f)N
1 f
10 s
576 4362(free,)N
742(30)X
3 f
12 s
576 4542(g)N
1 f
10 s
576 4722(geometry_manager)N
1216(\256eld,)X
1398(38)X
576 4812(geometry_manager)N
1216(procedure,)X
1578(41)X
3 f
12 s
576 4992(i)N
1 f
10 s
576 5172(initialize)N
876(procedure,)X
1238(17)X
576 5262(insert_child)N
974(procedure,)X
1336(15,)X
1456(25)X
3 f
12 s
576 5442(m)N
1 f
10 s
576 5622(malloc,)N
834(30)X
576 5712(move_focus_to_next)N
1268(procedure,)X
1630(43)X
2384 762(move_focus_to_prev)N
3081(procedure,)X
3443(43)X
3 f
12 s
2384 942(r)N
1 f
10 s
2384 1122(realize)N
2619(procedure,)X
2981(19)X
2384 1212(realloc,)N
2643(30)X
3 f
12 s
2384 1392(s)N
1 f
10 s
2384 1572(set_values)N
2738(procedure,)X
3100(47)X
3 f
12 s
2384 1752(w)N
1 f
10 s
2384 1932(widgetClass,)N
2815(5)X
2384 2022(widgetClassRec,)N
2940(10)X
3 f
2264 6048(71)N

3 p
%%Trailer
xt

xs
