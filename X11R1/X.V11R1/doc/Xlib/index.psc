%!PS-Adobe-1.0
%%Creator: orpheus:swick (Ralph R. Swick,,E40-327,3-1506,)
%%Title: stdin (ditroff)
%%CreationDate: Wed Sep 16 17:45:40 1987
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

%%Page: 263 1
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
576 1086(/)N
1 f
10 s
576 1266(/etc/X?.hosts,)N
1032(142)X
576 1356(/etc/ttys,)N
869(133)X
576 1446(/usr/include,)N
994(2)X
576 1536(/usr/lib/XerrorDB,)N
1190(181)X
576 1626(/usr/lib/rgb,)N
971(55)X
576 1716(/usr/lib/rgb.txt,)N
1075(55)X
3 f
12 s
576 1896(<)N
1 f
10 s
576 2076(<X11/X.h>,)N
984(2,)X
1064(62,)X
1184(142,)X
1344(146,)X
1504(147,)X
1664(178)X
576 2166(<X11/Xatom.h>,)N
1144(43,)X
1264(95,)X
1384(98,)X
1504(182,)X
1664(183)X
576 2256(<X11/Xlib.h>,)N
1068(6,)X
1148(147,)X
1308(211,)X
1468(225,)X
1628(229,)X
1788(238)X
576 2346(<X11/Xproto.h>,)N
1153(160)X
576 2436(<X11/Xresource.h>,)N
1257(216,)X
1417(219)X
576 2526(<X11/Xutil.h>,)N
1090(202,)X
1250(204,)X
1410(209,)X
1570(212,)X
1730(222)X
576 2616(<X11/keysym.h>,)N
1175(137)X
576 2706(<X11/keysymdef.h>,)N
1278(137,)X
1438(200)X
576 2796(<stdio.h>,)N
921(236)X
3 f
12 s
576 2976(A)N
1 f
10 s
576 3156(Above,)N
830(28,)X
950(30,)X
1070(165,)X
1230(30)X
576 3246(Access)N
824(Control,)X
1108(142)X
576 3336(AllPlanes,)N
927(6,)X
1007(63)X
576 3426(AllTemporary,)N
1075(132,)X
1235(133)X
576 3516(AllocAll,)N
896(54,)X
1016(55)X
576 3606(AllocNone,)N
968(54)X
576 3696(Allocation:)N
636 3786(Color)N
838(Map,)X
1025(56)X
576 3876(AllowExposures,)N
1150(140,)X
1310(142)X
576 3966(AlreadyGrabbed,)N
1152(121,)X
1312(125)X
576 4056(Always,)N
861(12,)X
981(20,)X
1101(35,)X
1221(41)X
576 4146(AntiGravity,)N
1003(29)X
576 4236(AnyButton,)N
971(121,)X
1131(122,)X
1291(123)X
576 4326(AnyKey,)N
888(125,)X
1048(126)X
576 4416(AnyModi\256er,)N
1034(121,)X
1194(122,)X
1354(123,)X
1514(125,)X
1674(126)X
576 4506(AnyPropertyType,)N
1195(46)X
576 4596(ArcChord,)N
937(68,)X
1057(77,)X
1177(91)X
576 4686(ArcPieSlice,)N
999(68,)X
1119(77,)X
1239(91)X
576 4776(Arcs:)N
636 4866(Drawing,)N
957(86)X
636 4956(Filling,)N
888(90)X
576 5046(AsyncBoth,)N
976(127)X
576 5136(AsyncKeyboard,)N
1138(127,)X
1298(128)X
576 5226(AsyncPointer,)N
1052(127,)X
1212(128)X
576 5316(Atom,)N
798(43)X
636 5406(Getting)N
896(name,)X
1110(45)X
636 5496(Interning,)N
970(45)X
636 5586(Prede\256ned,)N
1019(182)X
636 5676(Prede\256ned)N
999(properties,)X
1360(43)X
576 5766(Authentication,)N
1092(142)X
2384 1086(AutoRepeatModeOff,)N
3106(135)X
2384 1176(AutoRepeatModeOn,)N
3092(135)X
3 f
12 s
2384 1356(B)N
1 f
10 s
2384 1536(BadAccess,)N
2781(35,)X
2901(57,)X
3021(60,)X
3141(122,)X
3301(126,)X
3461(144,)X
3621(145,)X
3781(178)X
2384 1626(BadAlloc,)N
2732(24,)X
2852(45,)X
2972(48,)X
3092(54,)X
3212(55,)X
3332(56,)X
3452(58,)X
3572(59,)X
3692(62,)X
3812(68,)X
3932(69,)X
2384 1716(72,)N
2519(74,)X
2654(75,)X
2789(77,)X
2924(92,)X
3059(97,)X
3194(111,)X
3369(112,)X
3544(113,)X
3718(114,)X
3892(139,)X
2384 1806(140,)N
2552(143,)X
2720(144,)X
2888(178,)X
3056(184,)X
3224(185,)X
3391(186,)X
3558(188,)X
3725(189,)X
3892(190,)X
2384 1896(191,)N
2544(192,)X
2704(193,)X
2864(197,)X
3024(208)X
2384 1986(BadAtom,)N
2748(45,)X
2881(47,)X
3014(48,)X
3147(49,)X
3280(50,)X
3413(51,)X
3546(178,)X
3719(180,)X
3892(190,)X
2384 2076(191,)N
2544(196,)X
2704(197,)X
2864(208,)X
3024(209)X
2384 2166(BadColor,)N
2747(24,)X
2879(35,)X
3011(36,)X
3143(55,)X
3275(56,)X
3407(57,)X
3539(58,)X
3670(59,)X
3801(60,)X
3932(61,)X
2384 2256(118,)N
2544(178,)X
2704(180,)X
2864(204)X
2384 2346(BadCursor,)N
2782(24,)X
2912(35,)X
3042(113,)X
3212(114,)X
3382(121,)X
3552(122,)X
3722(124,)X
3892(178,)X
2384 2436(180)N
2384 2526(BadDrawable,)N
2878(42,)X
3010(62,)X
3142(68,)X
3274(73,)X
3406(74,)X
3538(81,)X
3670(82,)X
3801(83,)X
3932(85,)X
2384 2616(86,)N
2515(88,)X
2646(89,)X
2777(90,)X
2908(91,)X
3039(104,)X
3210(105,)X
3381(107,)X
3552(108,)X
3722(109,)X
3892(110,)X
2384 2706(114,)N
2544(178,)X
2704(180)X
2384 2796(BadFont,)N
2712(68,)X
2845(69,)X
2978(75,)X
3111(97,)X
3244(98,)X
3376(103,)X
3548(104,)X
3720(105,)X
3892(113,)X
2384 2886(178,)N
2544(180)X
2384 2976(BadGC,)N
2671(69,)X
2798(70,)X
2924(71,)X
3050(72,)X
3176(73,)X
3302(74,)X
3428(75,)X
3554(76,)X
3680(77,)X
3806(78,)X
3932(81,)X
2384 3066(82,)N
2511(83,)X
2638(85,)X
2764(86,)X
2890(88,)X
3016(89,)X
3142(90,)X
3268(91,)X
3394(103,)X
3560(104,)X
3726(105,)X
3892(107,)X
2384 3156(108,)N
2544(110,)X
2704(178,)X
2864(180)X
2384 3246(BadIDChoice,)N
2865(178)X
2384 3336(BadImage,)N
2754(110)X
2384 3426(BadImplementation,)N
3060(178)X
2384 3516(BadLength,)N
2780(138,)X
2940(178,)X
3100(231)X
2384 3606(BadMatch,)N
2768(18,)X
2898(19,)X
3028(20,)X
3158(21,)X
3287(23,)X
3416(24,)X
3545(28,)X
3674(30,)X
3803(32,)X
3932(35,)X
2384 3696(36,)N
2513(37,)X
2642(48,)X
2771(49,)X
2900(53,)X
3029(54,)X
3158(55,)X
3287(64,)X
3416(67,)X
3545(68,)X
3674(69,)X
3803(73,)X
3932(74,)X
2384 3786(75,)N
2510(76,)X
2636(80,)X
2762(81,)X
2888(82,)X
3014(83,)X
3140(85,)X
3266(86,)X
3392(88,)X
3517(89,)X
3642(90,)X
3767(91,)X
3892(104,)X
2384 3876(105,)N
2552(107,)X
2720(108,)X
2888(109,)X
3056(110,)X
3224(111,)X
3391(112,)X
3558(116,)X
3725(117,)X
3892(131,)X
2384 3966(134,)N
2544(178,)X
2704(209)X
2384 4056(BadName,)N
2745(56,)X
2865(57,)X
2985(60,)X
3105(92,)X
3225(178)X
2384 4146(BadPixmap,)N
2799(24,)X
2921(35,)X
3043(36,)X
3165(37,)X
3287(62,)X
3408(68,)X
3529(69,)X
3650(74,)X
3771(75,)X
3892(112,)X
2384 4236(178,)N
2544(180)X
2384 4326(BadRequest,)N
2811(178)X
2384 4416(BadValue,)N
2756(23,)X
2887(24,)X
3018(28,)X
3149(30,)X
3280(32,)X
3411(33,)X
3542(35,)X
3672(37,)X
3802(45,)X
3932(47,)X
2384 4506(48,)N
2513(54,)X
2642(57,)X
2771(58,)X
2900(59,)X
3029(60,)X
3158(61,)X
3287(62,)X
3416(68,)X
3545(69,)X
3674(70,)X
3803(71,)X
3932(72,)X
2384 4596(73,)N
2507(76,)X
2630(77,)X
2753(78,)X
2876(80,)X
2998(81,)X
3120(82,)X
3242(83,)X
3364(85,)X
3486(90,)X
3608(99,)X
3730(108,)X
3892(109,)X
2384 4686(110,)N
2552(111,)X
2720(112,)X
2888(113,)X
3056(117,)X
3224(121,)X
3391(122,)X
3558(125,)X
3725(126,)X
3892(128,)X
2384 4776(131,)N
2552(132,)X
2720(133,)X
2888(134,)X
3056(135,)X
3224(136,)X
3391(138,)X
3558(139,)X
3725(140,)X
3892(141,)X
2384 4866(143,)N
2544(144,)X
2704(169,)X
2864(176,)X
3024(178,)X
3184(209)X
2384 4956(BadValue.,)N
2765(69)X
2384 5046(BadWindow,)N
2832(24,)X
2955(25,)X
3078(27,)X
3200(28,)X
3322(30,)X
3444(31,)X
3566(32,)X
3688(33,)X
3810(34,)X
3932(35,)X
2384 5136(36,)N
2513(37,)X
2642(38,)X
2771(39,)X
2900(41,)X
3029(43,)X
3158(47,)X
3287(48,)X
3416(49,)X
3545(50,)X
3674(51,)X
3803(54,)X
3932(55,)X
2384 5226(80,)N
2516(114,)X
2688(116,)X
2860(117,)X
3032(119,)X
3204(121,)X
3376(122,)X
3548(123,)X
3720(125,)X
3892(126,)X
2384 5316(127,)N
2552(130,)X
2720(131,)X
2888(169,)X
3056(176,)X
3224(177,)X
3391(178,)X
3558(180,)X
3725(184,)X
3892(185,)X
2384 5406(186,)N
2552(188,)X
2720(189,)X
2888(190,)X
3056(191,)X
3224(192,)X
3391(193,)X
3558(196,)X
3725(197,)X
3892(208,)X
2384 5496(209)N
2384 5586(Below,)N
2633(28,)X
2753(30,)X
2873(165,)X
3033(30)X
2384 5676(BitmapBitOrder,)N
2944(10)X
2384 5766(BitmapFileInvalid,)N
3010(214)X
3 f
2244 6048(263)N

264 p
%%Page: 264 2
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(BitmapNoMemory,)N
1225(214,)X
1385(215)X
576 852(BitmapOpenFailed,)N
1225(214,)X
1385(215)X
576 942(BitmapPad,)N
971(10)X
576 1032(BitmapSuccess,)N
1105(214,)X
1265(215)X
576 1122(BitmapUnit,)N
993(10)X
576 1212(BlackPixel,)N
967(6)X
576 1302(BlackPixelOfScreen,)N
1271(11)X
576 1392(BottomIf,)N
909(28,)X
1029(29,)X
1149(30,)X
1269(165,)X
1429(30)X
576 1482(Button1,)N
873(152)X
576 1572(Button1Mask,)N
1051(43,)X
1171(152,)X
1331(153)X
576 1662(Button1MotionMask,)N
1286(120,)X
1446(122,)X
1606(123,)X
1766(169)X
576 1752(Button2,)N
873(152)X
576 1842(Button2Mask,)N
1051(43,)X
1171(152,)X
1331(153)X
576 1932(Button2MotionMask,)N
1286(120,)X
1446(122,)X
1606(123,)X
1766(169)X
576 2022(Button3,)N
873(152)X
576 2112(Button3Mask,)N
1051(43,)X
1171(152,)X
1331(153)X
576 2202(Button3MotionMask,)N
1286(120,)X
1446(122,)X
1606(123,)X
1766(169)X
576 2292(Button4,)N
873(152)X
576 2382(Button4Mask,)N
1051(43,)X
1171(152,)X
1331(153)X
576 2472(Button4MotionMask,)N
1286(120,)X
1446(122,)X
1606(123,)X
1766(169)X
576 2562(Button5,)N
873(152)X
576 2652(Button5Mask,)N
1051(43,)X
1171(152,)X
1331(153)X
576 2742(Button5MotionMask,)N
1286(120,)X
1446(122,)X
1606(123,)X
1766(169)X
576 2832(ButtonMotionMask,)N
1246(120,)X
1406(122,)X
1566(123)X
576 2922(ButtonPress,)N
1002(122,)X
1162(124,)X
1322(148,)X
1482(150,)X
1642(151,)X
1802(155,)X
1962(169)X
576 3012(ButtonPressMask,)N
1180(35,)X
1300(120,)X
1460(122,)X
1620(123,)X
1780(148,)X
1940(151,)X
2100(169)X
576 3102(ButtonRelease,)N
1083(124,)X
1243(150,)X
1403(151,)X
1563(155,)X
1723(169)X
576 3192(ButtonReleaseMask,)N
1261(120,)X
1421(122,)X
1581(123,)X
1741(151,)X
1901(169)X
576 3282(Buttons:)N
636 3372(Grabbing,)N
979(121)X
636 3462(Ungrabbing,)N
1059(122)X
576 3822(,)N
616(troff:)X
801(Can't)X
999(open)X
1175(/usr/lib/font/devpsc/TR.out)X
3 f
12 s
576 4002(C)N
1 f
10 s
576 4182(CapButt,)N
882(66,)X
1002(71)X
576 4272(CapNotLast,)N
1003(66,)X
1123(71)X
576 4362(CapProjecting,)N
1074(66,)X
1194(71)X
576 4452(CapRound,)N
958(66,)X
1078(71)X
576 4542(CellsOfScreen,)N
1084(11)X
576 4632(CenterGravity,)N
1075(29)X
576 4722(Child)N
773(Window,)X
1089(39)X
576 4812(CirculateNotify,)N
1119(33,)X
1239(148,)X
1399(152,)X
1559(160,)X
1719(164)X
576 4902(CirculateRequest,)N
1168(33,)X
1288(164,)X
1448(169)X
576 4992(Clearing:)N
636 5082(Area,)N
833(79)X
636 5172(Window,)N
952(80)X
576 5262(ClientMessage,)N
1092(44,)X
1212(148,)X
1372(166,)X
1532(167)X
576 5352(ClipByChildren,)N
1126(68,)X
1246(77)X
576 5442(Color,)N
798(53)X
636 5532(Data)N
808(Base,)X
1004(55)X
636 5622(Getting)N
896(Values,)X
1159(61)X
636 5712(Naming,)N
934(56)X
636 5802(Parsing)N
896(Command)X
1249(Lines,)X
1467(203)X
2444 762(Setting)N
2690(Cells,)X
2894(57)X
2384 852(Color)N
2586(Allocation,)X
2964(56,)X
3084(58)X
2384 942(Colormap,)N
2744(54,)X
2864(56)X
2384 1032(ColormapChangeMask,)N
3167(166)X
2384 1122(ColormapInstalled,)N
3020(166)X
2384 1212(ColormapNotify,)N
2953(35,)X
3073(55,)X
3193(118,)X
3353(166)X
2384 1302(ColormapUninstalled,)N
3113(166)X
2384 1392(Complex,)N
2717(90)X
2384 1482(Con\256gureNotify,)N
2953(29,)X
3073(148,)X
3233(152,)X
3393(160,)X
3553(161,)X
3713(164)X
2384 1572(Con\256gureRequest,)N
3021(28,)X
3160(31,)X
3299(32,)X
3438(33,)X
3576(34,)X
3714(164,)X
3892(165,)X
2384 1662(169)N
2384 1752(Con\256gureWindow,)N
3020(165)X
2384 1842(ConnectionNumber,)N
3056(7)X
2384 1932(ControlMask,)N
2846(43,)X
2966(121,)X
3126(123,)X
3286(125,)X
3446(126,)X
3606(152,)X
3766(153)X
2384 2022(ConvertSelection,)N
2980(168)X
2384 2112(Convex,)N
2673(90)X
2384 2202(CoordModeOrigin,)N
3020(83,)X
3140(84,)X
3260(85,)X
3380(90)X
2384 2292(CoordModePrevious,)N
3091(83,)X
3211(84,)X
3331(85,)X
3451(90)X
2384 2382(CopyFromParent,)N
2975(20,)X
3095(21,)X
3215(23,)X
3335(35,)X
3455(37)X
2384 2472(Copying:)N
2444 2562(Area,)N
2641(80)X
2444 2652(Plane,)N
2662(81)X
2384 2742(CreateNotify,)N
2843(23,)X
2963(24,)X
3083(148,)X
3243(160,)X
3403(161,)X
3563(170,)X
3723(171)X
2384 2832(CurrentTime,)N
2840(50,)X
2962(51,)X
3084(119,)X
3246(120,)X
3408(121,)X
3570(123,)X
3731(124,)X
3892(125,)X
2384 2922(127,)N
2544(130,)X
2704(148,)X
2864(168,)X
3024(177,)X
3184(261)X
2384 3012(Cursor:)N
2444 3102(Initial)N
2655(State,)X
2855(24)X
2384 3192(CursorShape,)N
2838(73)X
2384 3282(Cut)N
2519(Buffers,)X
2800(208)X
3 f
12 s
2384 3462(D)N
1 f
10 s
2384 3642(DECnet)N
2662(Addresses,)X
3032(142)X
2384 3732(DISPLAY)N
2742(Environment)X
3180(Variable,)X
3497(5)X
2384 3822(Data,)N
2576(234)X
2384 3912(Data)N
2556(Structures:)X
2444 4002(Display,)N
2733(6,)X
2813(229,)X
2973(256,)X
3133(260)X
2444 4092(Screen,)N
2703(260)X
2444 4182(Visual,)N
2693(16)X
2444 4272(XArc,)N
2663(82)X
2444 4362(XChar2b,)N
2778(94)X
2444 4452(XCharStruct,)N
2889(94)X
2444 4542(XColor,)N
2724(53)X
2444 4632(XErrorEvent,)N
2899(178)X
2444 4722(XExtCodes,)N
2853(225)X
2444 4812(XExtData,)N
2805(229)X
2444 4902(XFontProp,)N
2839(94)X
2444 4992(XFontStruct,)N
2879(94)X
2444 5082(XGCValues,)N
2876(64)X
2444 5172(XHostAddress,)N
2956(142)X
2444 5262(XIconSize,)N
2823(191)X
2444 5352(XImage,)N
2743(107)X
2444 5442(XKeyboardControl,)N
3103(134)X
2444 5532(XKeyboardState,)N
3019(135)X
2444 5622(XPoint,)N
2710(82)X
2444 5712(XRectangle,)N
2863(82)X
2444 5802(XSegment,)N
2822(82)X
3 f
2244 6048(264)N

265 p
%%Page: 265 3
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(XSetWindowAttributes,)N
1432(19)X
636 852(XSizeHints,)N
1045(188)X
636 942(XTextItem,)N
1028(103)X
636 1032(XTextItem16,)N
1108(103)X
636 1122(XTimeCoord,)N
1103(177)X
636 1212(XVisualInfo,)N
1077(210)X
636 1302(XWMClass,)N
1054(192)X
636 1392(XWMHints,)N
1054(186)X
636 1482(XWindowAttributes,)N
1330(40)X
636 1572(XWindowChanges,)N
1286(28)X
636 1662(xDoSomethingReply,)N
1351(232)X
636 1752(xDoSomethingReq,)N
1289(232)X
636 1842(xResourceReq,)N
1144(232)X
576 1932(Debugging:)N
636 2022(Error)N
826(Event,)X
1053(178)X
636 2112(Error)N
826(Handlers,)X
1156(178)X
636 2202(Error)N
826(Message)X
1127(Strings,)X
1393(180)X
636 2292(Error)N
826(Numbers,)X
1160(178)X
636 2382(Synchronous)N
1074(Mode,)X
1301(177)X
576 2472(Default)N
837(Options,)X
1130(202)X
576 2562(Default)N
837(Protection,)X
1206(142)X
576 2652(DefaultBlanking,)N
1150(140,)X
1310(142)X
576 2742(DefaultColormap,)N
1177(7,)X
1257(53)X
576 2832 0.1359(DefaultColormapOfScreen,)AN
1481(11)X
576 2922(DefaultDepth,)N
1053(7)X
576 3012 0.1813(DefaultDepthOfScreen,)AN
1357(11)X
576 3102(DefaultExposures,)N
1191(140,)X
1351(142)X
576 3192(DefaultGC,)N
968(7)X
576 3282(DefaultGCOfScreen,)N
1272(12)X
576 3372(DefaultRootWindow,)N
1288(7)X
576 3462(DefaultScreen,)N
1076(5,)X
1156(6,)X
1236(7)X
576 3552 0.1477(DefaultScreenOfDisplay,)AN
1410(12)X
576 3642(DefaultVisual,)N
1066(8,)X
1146(53)X
576 3732 0.1548(DefaultVisualOfScreen,)AN
1370(12)X
576 3822(De\256nitions:)N
636 3912(Active)N
870(Grab,)X
1071(119)X
636 4002(Atom,)N
858(43)X
636 4092(Bitmap,)N
911(2)X
636 4182(Child)N
833(Window,)X
1149(1)X
636 4272(Colormap,)N
996(53)X
636 4362(CurrentTime,)N
1090(119)X
636 4452(Destination,)N
1045(62)X
636 4542(Display,)N
925(5)X
636 4632(Display)N
905(Functions,)X
1260(62)X
636 4722(Drawable,)N
989(2)X
636 4812(Event,)N
863(146)X
636 4902(Event)N
843(Exposure,)X
1186(256)X
636 4992(Events,)N
894(2)X
636 5082(Font,)N
822(103)X
636 5172(GC,)N
787(257)X
636 5262(Graphics)N
946(Context,)X
1239(52,)X
1359(63)X
636 5352(InputOnly)N
985(window,)X
1283(257)X
636 5442(InputOutput)N
1047(window,)X
1345(258)X
636 5532(Parent)N
861(Window,)X
1177(1)X
636 5622(Passive)N
896(Grab,)X
1097(119)X
636 5712(Pixmap,)N
920(2)X
636 5802(Plane)N
834(Masks,)X
1083(63)X
2444 762(RGB)N
2628(values,)X
2873(260)X
2444 852(Reply,)N
2675(146)X
2444 942(Requests,)N
2773(146)X
2444 1032(Resource)N
2763(ID,)X
2888(2)X
2444 1122(Screen,)N
2703(1,)X
2783(5)X
2444 1212(ScreenCount,)N
2898(9)X
2444 1302(Source,)N
2707(62)X
2444 1392(Stacking)N
2744(order,)X
2954(1)X
2444 1482(Status,)N
2679(2)X
2444 1572(Tile)N
2593(Mode,)X
2820(18)X
2444 1662(Tiles,)N
2644(2)X
2444 1752(Visual)N
2673(Type,)X
2878(16)X
2444 1842(Window,)N
2760(1)X
2444 1932 0.1750(XActivateScreenSaver,)AN
3216(141)X
2444 2022(XAddHost,)N
2831(142)X
2444 2112(XAddHosts,)N
2862(143)X
2444 2202(XAddPixel,)N
2844(213)X
2444 2292(XAddToExtensionList,)N
3213(229)X
2444 2382(XAddToSaveSet,)N
3027(117)X
2444 2472(XAllPlanes,)N
2853(6)X
2444 2562(XAllocColor,)N
2902(56)X
2444 2652(XAllocColorCells,)N
3066(58)X
2444 2742(XAllocColorPlanes,)N
3111(58)X
2444 2832(XAllocNamedColor,)N
3134(56)X
2444 2922(XAllowEvents,)N
2960(127)X
2444 3012(XAutoRepeatOff,)N
3037(135)X
2444 3102(XAutoRepeatOn,)N
3023(135)X
2444 3192(XBell,)N
2675(135)X
2444 3282(XBitmapBitOrder,)N
3062(10)X
2444 3372(XBitmapPad,)N
2897(10)X
2444 3462(XBitmapUnit,)N
2919(10)X
2444 3552(XBlackPixel,)N
2893(6)X
2444 3642(XBlackPixelOfScreen,)N
3197(11)X
2444 3732(XCellsOfScreen,)N
3010(11)X
2444 3822(XChangeActivePointerGrab,)N
3393(123)X
2444 3912(XChangeGC,)N
2898(69)X
2444 4002(XChangeKeyboardControl,)N
3348(133)X
2444 4092(XChangeKeyboardMapping,)N
3393(138)X
2444 4182(XChangePointerControl,)N
3262(131)X
2444 4272(XChangeProperty,)N
3063(47)X
2444 4362(XChangeSaveSet,)N
3045(116)X
2444 4452(XChangeWindowAttributes,)N
3383(35)X
2444 4542(XCheckIfEvent,)N
2988(172)X
2444 4632(XCheckMaskEvent,)N
3112(174)X
2444 4722(XCheckTypedEvent,)N
3139(175)X
2444 4812(XCheckTypedWindowEvent,)N
3415(175)X
2444 4902(XCheckWindowEvent,)N
3210(173)X
2444 4992(XCirculateSubwindows,)N
3249(33)X
2444 5082(XCirculateSubwindowsDown,)N
3445(34)X
2444 5172(XCirculateSubwindowsUp,)N
3347(34)X
2444 5262(XClearArea,)N
2873(79)X
2444 5352(XClearWindow,)N
2992(80)X
2444 5442(XClipBox,)N
2812(204)X
2444 5532(XCloseDisplay,)N
2973(14)X
2444 5622(XCon\256gureWindow,)N
3138(30)X
2444 5712(XConnectionNumber,)N
3174(7)X
2444 5802(XConvertSelection,)N
3098(50)X
3 f
2244 6048(265)N

266 p
%%Page: 266 4
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(XCopyArea,)N
1064(80)X
636 852(XCopyColormapAndFree,)N
1508(54)X
636 942(XCopyGC,)N
1018(68)X
636 1032(XCopyPlane,)N
1085(81)X
636 1122(XCreateBitmapFromData,)N
1504(215)X
636 1212(XCreateColormap,)N
1264(53)X
636 1302(XCreateFontCursor,)N
1308(111)X
636 1392(XCreateGC,)N
1055(64)X
636 1482(XCreateGlyphCursor,)N
1362(112)X
636 1572(XCreateImage,)N
1145(211)X
636 1662(XCreatePixmap,)N
1188(61)X
636 1752(XCreatePixmapCursor,)N
1406(111)X
636 1842(XCreateRegion,)N
1175(205)X
636 1932(XCreateSimpleWindow,)N
1446(24)X
636 2022(XCreateWindow,)N
1220(23)X
636 2112(XDefaultColormap,)N
1295(7)X
636 2202 0.1406(XDefaultColormapOfScreen,)AN
1599(11)X
636 2292(XDefaultDepth,)N
1171(7)X
636 2382 0.1845(XDefaultDepthOfScreen,)AN
1475(11)X
636 2472 0.1806(XDefaultGCOfScreen,)AN
1390(12)X
636 2562(XDefaultRootWindow,)N
1406(7)X
636 2652(XDefaultScreen,)N
1194(7)X
636 2742 0.1522(XDefaultScreenOfDisplay,)AN
1528(12)X
636 2832(XDefaultVisual,)N
1184(8)X
636 2922 0.1591(XDefaultVisualOfScreen,)AN
1488(12)X
636 3012(XDe\256neCursor,)N
1166(114)X
636 3102(XDeleteContext,)N
1197(223)X
636 3192(XDeleteProperty,)N
1220(49)X
636 3282(XDestroyImage,)N
1189(214)X
636 3372(XDestroyRegion,)N
1219(205)X
636 3462(XDestroySubwindows,)N
1401(25)X
636 3552(XDestroyWindow,)N
1264(25)X
636 3642(XDisableAccessControl,)N
1451(144)X
636 3732(XDisplayCells,)N
1147(8)X
636 3822(XDisplayHeight,)N
1201(10)X
636 3912(XDisplayName,)N
1175(181)X
636 4002(XDisplayOfScreen,)N
1287(12)X
636 4092(XDisplayPlanes,)N
1192(8)X
636 4182(XDisplayString,)N
1178(8)X
636 4272(XDisplayWidth,)N
1183(11)X
636 4362(XDisplayWidthMM,)N
1325(11)X
636 4452(XDoesBackingStore,)N
1335(12)X
636 4542(XDoesSaveUnders,)N
1287(12)X
636 4632(XDrawArc,)N
1034(86)X
636 4722(XDrawArcs,)N
1065(87)X
636 4812(XDrawImageString,)N
1309(105)X
636 4902(XDrawImageString16,)N
1389(106)X
636 4992(XDrawLine,)N
1060(84)X
636 5082(XDrawLines,)N
1091(84)X
636 5172(XDrawPoint,)N
1081(82)X
636 5262(XDrawPoints,)N
1112(83)X
636 5352(XDrawRectangle,)N
1234(85)X
636 5442(XDrawRectangles,)N
1265(86)X
636 5532(XDrawSegments,)N
1224(84)X
636 5622(XDrawString,)N
1108(104)X
636 5712(XDrawString16,)N
1188(105)X
636 5802(XDrawText,)N
1060(103)X
2444 762(XDrawText16,)N
2948(104)X
2444 852(XESetCloseDisplay,)N
3124(225)X
2444 942(XESetCopyGC,)N
2977(226)X
2444 1032(XESetCreateFont,)N
3049(226)X
2444 1122(XESetCreateGC,)N
3014(225)X
2444 1212(XESetError,)N
2863(228)X
2444 1302(XESetErrorString,)N
3058(228)X
2444 1392(XESetEventToWire,)N
3130(227)X
2444 1482(XESetFlushGC,)N
2981(228)X
2444 1572(XESetFreeFont,)N
2982(226)X
2444 1662(XESetFreeGC,)N
2947(226)X
2444 1752(XESetWireToEvent,)N
3130(227)X
2444 1842(XEmptyRegion,)N
2986(207)X
2444 1932(XEnableAccessControl,)N
3237(144)X
2444 2022(XEqualRegion,)N
2960(207)X
2444 2112(XEvent,)N
2729(147)X
2444 2202(XEventMaskOfScreen,)N
3211(12)X
2444 2292(XFetchBuffer,)N
2930(209)X
2444 2382(XFetchBytes,)N
2902(208)X
2444 2472(XFetchName,)N
2912(185)X
2444 2562(XFillArc,)N
2773(90)X
2444 2652(XFillArcs,)N
2804(91)X
2444 2742(XFillPolygon,)N
2918(89)X
2444 2832(XFillRectangle,)N
2973(88)X
2444 2922(XFillRectangles,)N
3004(89)X
2444 3012(XFindContext,)N
2941(222)X
2444 3102(XFindOnExtensionList,)N
3230(229)X
2444 3192(XFlush,)N
2719(169)X
2444 3282 0.1985(XForceScreenSaver,)AN
3127(141)X
2444 3372(XFree,)N
2685(14)X
2444 3462(XFreeColormap,)N
3005(55)X
2444 3552(XFreeColors,)N
2898(60)X
2444 3642(XFreeCursor,)N
2903(113)X
2444 3732(XFreeExtensionList,)N
3129(224)X
2444 3822(XFreeFont,)N
2831(97)X
2444 3912(XFreeFontInfo,)N
2965(93)X
2444 4002(XFreeFontNames,)N
3054(99)X
2444 4092(XFreeFontPath,)N
2973(99)X
2444 4182(XFreeGC,)N
2796(69)X
2444 4272(XFreeModi\256erMapping,)N
3254(139)X
2444 4362(XFreePixmap,)N
2929(62)X
2444 4452(XGContextFromGC,)N
3137(92)X
2444 4542(XGeometry,)N
2863(203)X
2444 4632(XGetAtomName,)N
3032(45)X
2444 4722(XGetClassHint,)N
2973(192)X
2444 4812(XGetDefault,)N
2899(202)X
2444 4902 0.2024(XGetErrorDatabaseText,)AN
3270(180)X
2444 4992(XGetErrorText,)N
2975(180)X
2444 5082(XGetFontPath,)N
2946(99)X
2444 5172(XGetFontProperty,)N
3080(97)X
2444 5262(XGetGeometry,)N
2979(41)X
2444 5352(XGetIconName,)N
2993(185)X
2444 5442(XGetIconSizes,)N
2970(191)X
2444 5532(XGetImage,)N
2859(108)X
2444 5622(XGetInputFocus,)N
3018(131)X
2444 5712(XGetKeyboardControl,)N
3219(134)X
2444 5802(XGetKeyboardMapping,)N
3264(137)X
3 f
2244 6048(266)N

267 p
%%Page: 267 5
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(XGetModi\256erMapping,)N
1419(140)X
636 852(XGetMotionEvents,)N
1303(176)X
636 942(XGetNormalHints,)N
1268(189)X
636 1032(XGetPixel,)N
1014(212)X
636 1122(XGetPointerControl,)N
1325(132)X
636 1212(XGetPointerMapping,)N
1370(136)X
636 1302(XGetScreenSaver,)N
1252(141)X
636 1392(XGetSelectionOwner,)N
1367(50)X
636 1482(XGetSizeHints,)N
1161(190)X
636 1572(XGetStandardColormap,)N
1455(196)X
636 1662(XGetSubImage,)N
1175(109)X
636 1752(XGetTransientForHint,)N
1406(193)X
636 1842(XGetVisualInfo,)N
1193(210)X
636 1932(XGetWMHints,)N
1170(188)X
636 2022(XGetWindowAttributes,)N
1446(39)X
636 2112(XGetWindowProperty,)N
1402(45)X
636 2202(XGetZoomHints,)N
1214(190)X
636 2292(XGrabButton,)N
1112(121)X
636 2382(XGrabKey,)N
1029(125)X
636 2472(XGrabKeyboard,)N
1212(124)X
636 2562(XGrabPointer,)N
1126(119)X
636 2652(XGrabServer,)N
1105(129)X
636 2742(XHeightMMOfScreen,)N
1398(13)X
636 2832(XHeightOfScreen,)N
1256(13)X
636 2922(XIfEvent,)N
975(171)X
636 3012(XImageByteOrder,)N
1274(8)X
636 3102(XInitExtension,)N
1165(225)X
636 3192(XInstallColormap,)N
1254(118)X
636 3282(XInternAtom,)N
1108(45)X
636 3372(XIntersectRegion,)N
1242(206)X
636 3462(XKeycodeToKeysym,)N
1376(201)X
636 3552(XKeysymToKeycode,)N
1376(201)X
636 3642(XKeysymToString,)N
1285(200)X
636 3732(XKillClient,)N
1053(132)X
636 3822(XListExtensions,)N
1209(224)X
636 3912(XListFonts,)N
1035(98)X
636 4002(XListFontsWithInfo,)N
1329(93)X
636 4092(XListHosts,)N
1040(143)X
636 4182(XListInstalledColormaps,)N
1485(118)X
636 4272(XListProperties,)N
1183(47)X
636 4362(XLoadFont,)N
1045(92)X
636 4452(XLoadQueryFont,)N
1246(93)X
636 4542(XLookupColor,)N
1165(56)X
636 4632(XLookupKeysym,)N
1250(199)X
636 4722(XLookupString,)N
1178(199)X
636 4812(XLowerWindow,)N
1220(33)X
636 4902(XMapRaised,)N
1099(27)X
636 4992(XMapSubwindows,)N
1294(27)X
636 5082(XMapWindow,)N
1157(26)X
636 5172(XMaskEvent,)N
1099(174)X
636 5262(XMatchVisualInfo,)N
1282(210)X
636 5352(XMaxCmapsOfScreen,)N
1407(13)X
636 5442(XMinCmapsOfScreen,)N
1393(13)X
636 5532(XMoveResizeWindow,)N
1411(31)X
636 5622(XMoveWindow,)N
1197(31)X
636 5712(XNewModi\256erMapping,)N
1455(139)X
636 5802(XNextEvent,)N
1077(170)X
2444 762(XNoOp,)N
2738(14)X
2444 852(XOffsetRegion,)N
2974(205)X
2444 942(XOpenDisplay,)N
2965(5)X
2444 1032(XParseColor,)N
2898(203)X
2444 1122(XParseGeometry,)N
3037(202)X
2444 1212(XPeekEvent,)N
2885(171)X
2444 1302(XPeekIfEvent,)N
2939(172)X
2444 1392(XPending,)N
2804(170)X
2444 1482(XPlanesOfScreen,)N
3055(13)X
2444 1572(XPointInRegion,)N
3008(207)X
2444 1662(XPolygonRegion,)N
3039(204)X
2444 1752(XProtocolRevision,)N
3097(8)X
2444 1842(XProtocolVersion,)N
3067(9)X
2444 1932(XPutBackEvent,)N
3000(172)X
2444 2022(XPutImage,)N
2849(108)X
2444 2112(XPutPixel,)N
2812(213)X
2444 2202(XQLength,)N
2827(9)X
2444 2292(XQueryBestCursor,)N
3103(113)X
2444 2382(XQueryBestSize,)N
3023(73)X
2444 2472(XQueryBestStipple,)N
3111(74)X
2444 2562(XQueryBestTile,)N
3014(73)X
2444 2652(XQueryColor,)N
2925(61)X
2444 2742(XQueryColors,)N
2956(61)X
2444 2832(XQueryExtension,)N
3063(224)X
2444 2922(XQueryFont,)N
2889(92)X
2444 3012(XQueryKeymap,)N
3015(136)X
2444 3102(XQueryPointer,)N
2974(42)X
2444 3192(XQueryTextExtents,)N
3130(101)X
2444 3282(XQueryTextExtents16,)N
3210(102)X
2444 3372(XQueryTree,)N
2891(39)X
2444 3462(XRaiseWindow,)N
2996(32)X
2444 3552(XReadBitmapFile,)N
3066(214)X
2444 3642(XRebindKeysym,)N
3040(200)X
2444 3732(XRecInRegion,)N
2965(207)X
2444 3822(XRecolorCursor,)N
3014(113)X
2444 3912 0.1413(XRefreshKeyboardMapping,)AN
3398(199)X
2444 4002(XRemoveFromSaveSet,)N
3240(117)X
2444 4092(XRemoveHost,)N
2960(143)X
2444 4182(XRemoveHosts,)N
2991(144)X
2444 4272(XReparentWindow,)N
3108(115)X
2444 4362(XResetScreenSaver,)N
3122(141)X
2444 4452(XResizeWindow,)N
3032(31)X
2444 4542(XRestackWindows,)N
3103(34)X
2444 4632(XRootWindow,)N
2973(9)X
2444 4722(XRootWindowOfScreen,)N
3277(13)X
2444 4812(XRotateBuffers,)N
2992(209)X
2444 4902(XRotateWindowProperties,)N
3352(48)X
2444 4992(XSaveContext,)N
2951(222)X
2444 5082(XScreenOfDisplay,)N
3095(13)X
2444 5172(XSelectInput,)N
2907(168)X
2444 5262(XSendEvent,)N
2889(175)X
2444 5352(XServerVendor,)N
2993(9)X
2444 5442(XSetAccessControl,)N
3116(144)X
2444 5532(XSetAfterFunction,)N
3098(178)X
2444 5622(XSetArcMode,)N
2952(77)X
2444 5712(XSetBackground,)N
3036(71)X
2444 5802(XSetClassHint,)N
2959(192)X
3 f
2244 6048(267)N

268 p
%%Page: 268 6
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(XSetClipMask,)N
1151(76)X
636 852(XSetClipOrigin,)N
1182(75)X
636 942(XSetClipRectangles,)N
1325(76)X
636 1032(XSetCloseDownMode,)N
1401(132)X
636 1122(XSetCommand,)N
1169(186)X
636 1212(XSetDashes,)N
1068(72)X
636 1302(XSetErrorHandler,)N
1265(178)X
636 1392(XSetFillRule,)N
1097(72)X
636 1482(XSetFillStyle,)N
1110(72)X
636 1572(XSetFont,)N
982(75)X
636 1662(XSetFontPath,)N
1124(99)X
636 1752(XSetForeground,)N
1210(71)X
636 1842(XSetFunction,)N
1120(70)X
636 1932(XSetGraphicsExposures,)N
1460(77)X
636 2022 0.1806(XSetIOErrorHandler,)AN
1350(181)X
636 2112(XSetIconName,)N
1171(185)X
636 2202(XSetIconSizes,)N
1148(191)X
636 2292(XSetInputFocus,)N
1196(130)X
636 2382(XSetLineAttributes,)N
1303(71)X
636 2472(XSetModi\256erMapping,)N
1405(139)X
636 2562(XSetNormalHints,)N
1254(189)X
636 2652(XSetPlaneMask,)N
1192(70)X
636 2742(XSetPointerMapping,)N
1356(136)X
636 2832(XSetRegion,)N
1067(205)X
636 2922(XSetScreenSaver,)N
1238(140)X
636 3012(XSetSelectionOwner,)N
1353(49)X
636 3102(XSetSizeHints,)N
1147(190)X
636 3192(XSetStandardColormap,)N
1441(196)X
636 3282(XSetStandardProperties,)N
1446(184)X
636 3372(XSetState,)N
996(70)X
636 3462(XSetStipple,)N
1062(74)X
636 3552(XSetSubwindowMode,)N
1405(77)X
636 3642(XSetTSOrigin,)N
1138(75)X
636 3732(XSetTile,)N
965(74)X
636 3822(XSetTransientForHint,)N
1392(193)X
636 3912(XSetWMHints,)N
1156(187)X
636 4002(XSetWindowBackground,)N
1504(36)X
636 4092(XSetWindowBackgroundPixmap,)N
1748(36)X
636 4182(XSetWindowBorder,)N
1335(36)X
636 4272(XSetWindowBorderPixmap,)N
1579(37)X
636 4362(XSetWindowBorderWidth,)N
1535(32)X
636 4452(XSetWindowColormap,)N
1432(55)X
636 4542(XSetZoomHints,)N
1200(189)X
636 4632(XShrinkRegion,)N
1178(206)X
636 4722(XStoreBuffer,)N
1113(208)X
636 4812(XStoreBytes,)N
1085(208)X
636 4902(XStoreColor,)N
1085(57)X
636 4992(XStoreColors,)N
1116(57)X
636 5082(XStoreName,)N
1095(184)X
636 5172(XStoreNamedColor,)N
1317(59)X
636 5262(XStringToKeysym,)N
1285(200)X
636 5352(XSubImage,)N
1059(213)X
636 5442(XSubtractRegion,)N
1232(206)X
636 5532(XSync,)N
894(169)X
636 5622(XSynchronize,)N
1135(177)X
636 5712(XTextExtents,)N
1121(100)X
636 5802(XTextExtents16,)N
1201(101)X
2444 762(XTextWidth,)N
2889(100)X
2444 852(XTextWidth16,)N
2969(100)X
2444 942(XTranslateCoordinates,)N
3228(37)X
2444 1032(XUnde\256neCursor,)N
3054(114)X
2444 1122(XUngrabButton,)N
3000(122)X
2444 1212(XUngrabKey,)N
2917(126)X
2444 1302(XUngrabKeyboard,)N
3100(125)X
2444 1392(XUngrabPointer,)N
3014(123)X
2444 1482(XUngrabServer,)N
2993(129)X
2444 1572(XUninstallColormap,)N
3155(118)X
2444 1662(XUnionRegion,)N
2973(206)X
2444 1752(XUniqueContext,)N
3031(223)X
2444 1842(XUnloadFont,)N
2924(98)X
2444 1932(XUnmapSubwindows,)N
3191(27)X
2444 2022(XUnmapWindow,)N
3054(27)X
2444 2112(XVendorRelease,)N
3033(9)X
2444 2202(XWarpPointer,)N
2952(129)X
2444 2292(XWhitePixel,)N
2902(10)X
2444 2382(XWhitePixelOfScreen,)N
3206(14)X
2444 2472(XWidthMMOfScreen,)N
3188(14)X
2444 2562(XWidthOfScreen,)N
3046(14)X
2444 2652(XWindowEvent,)N
3005(173)X
2444 2742(XWriteBitmapFile,)N
3084(214)X
2444 2832(XXorRegion,)N
2898(206)X
2444 2922(XYFormat,)N
2831(262)X
2444 3012(XrmConvert,)N
2889(219)X
2444 3102(XrmGetDataBase,)N
3055(220)X
2444 3192(XrmGetResource,)N
3046(218)X
2444 3282(XrmGetSearchList,)N
3090(218)X
2444 3372 0.1813(XrmGetSearchResource,)AN
3265(219)X
2444 3462(XrmLoadDataBase,)N
3104(221)X
2444 3552(XrmMergeDataBases,)N
3180(220)X
2444 3642(XrmParseCommand,)N
3138(221)X
2444 3732(XrmPutDataBase,)N
3045(221)X
2444 3822(XrmPutResource,)N
3036(218)X
2444 3912(XrmRegisterTypeConverter,)N
3384(220)X
2444 4002(ZFormat,)N
2764(262)X
2444 4092(_XAllocScratch,)N
3001(236)X
2444 4182(_XReply,)N
2773(234)X
2444 4272(access)N
2670(control)X
2917(list,)X
3054(254)X
2444 4362(active)N
2656(grab,)X
2839(254)X
2444 4452(ancestors,)N
2783(254)X
2444 4542(atom,)N
2644(254)X
2444 4632(background,)N
2863(254)X
2444 4722(backing)N
2718(store,)X
2914(254)X
2444 4812(bit)N
2548(gravity,)X
2815(254)X
2444 4902(bit)N
2548(plane,)X
2762(254)X
2444 4992(bitmap,)N
2706(254)X
2444 5082(border,)N
2694(254)X
2444 5172(button)N
2668(grabbing,)X
2993(254)X
2444 5262(byte)N
2602(order,)X
2812(254)X
2444 5352(children,)N
2747(255)X
2444 5442(class,)N
2640(255)X
2444 5532(client,)N
2662(255)X
2444 5622(clipping)N
2726(region,)X
2971(255)X
2444 5712(color)N
2629(map,)X
2807(255)X
2444 5802(connection,)N
2836(255)X
3 f
2244 6048(268)N

269 p
%%Page: 269 7
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(containment,)N
1072(255)X
636 852(coordinate)N
995(system,)X
1257(255)X
636 942(cursor,)N
877(255)X
636 1032(depth,)N
854(255)X
636 1122(device,)N
886(255)X
636 1212(direct)N
839(color,)X
1044(256)X
636 1302(display,)N
907(256)X
636 1392(drawable,)N
971(256)X
636 1482(event,)N
850(256)X
636 1572(event)N
830(mask,)X
1039(147,)X
1199(256)X
636 1662(event)N
830(propagation,)X
1253(256)X
636 1752(event)N
830(source,)X
1080(256)X
636 1842(event)N
830(synchronization,)X
1382(256)X
636 1932(extension,)N
983(256)X
636 2022(font,)N
805(256)X
636 2112(frozen,)N
882(257)X
636 2202(glyph,)N
858(257)X
636 2292(grab,)N
819(257)X
636 2382(graphics)N
928(context,)X
1204(257)X
636 2472(gravity,)N
903(257)X
636 2562(gray)N
799(scale,)X
1000(257)X
636 2652(hotspot,)N
911(257)X
636 2742(identi\256er,)N
965(257)X
636 2832(inferiors,)N
948(257)X
636 2922(input)N
820(focus,)X
1034(257)X
636 3012(input)N
820(manager,)X
1137(257)X
636 3102(key)N
772(grabbing,)X
1097(258)X
636 3192(keyboard)N
955(grabbing,)X
1280(258)X
636 3282(keysym,)N
925(258)X
636 3372(mapped,)N
930(258)X
636 3462(modi\256er)N
927(keys,)X
1114(258)X
636 3552(monochrome,)N
1099(258)X
636 3642(obscure,)N
926(258)X
636 3732(obscures,)N
957(258)X
636 3822(occlude,)N
926(258)X
636 3912(occludes,)N
957(258)X
636 4002(padding,)N
934(258)X
636 4092(parent)N
857(window,)X
1155(258)X
636 4182(passive)N
892(grab,)X
1075(258)X
636 4272(pixel)N
816(value,)X
1030(259)X
636 4362(pixmap,)N
916(259)X
636 4452(plane,)N
850(259)X
636 4542(plane)N
830(mask,)X
1039(259)X
636 4632(pointer,)N
903(259)X
636 4722(pointer)N
883(grabbing,)X
1208(259)X
636 4812(pointing)N
922(device,)X
1172(259)X
636 4902(property,)N
948(259)X
636 4992(property)N
928(list,)X
1065(259)X
636 5082(pseudo)N
883(color,)X
1088(259)X
636 5172(raise,)N
828(259)X
636 5262(rectangle,)N
971(259)X
636 5352(redirecting)N
1004(control,)X
1271(260)X
636 5442(reply,)N
841(260)X
636 5532(request,)N
908(260)X
636 5622(resource,)N
949(260)X
636 5712(root,)N
805(52,)X
925(260)X
636 5802(root)N
785(window,)X
1083(260)X
2444 762(save)N
2607(set,)X
2736(260)X
2444 852(scanline,)N
2747(260)X
2444 942(scanline)N
2727(order,)X
2937(260)X
2444 1032(screen,)N
2690(260)X
2444 1122(selection,)N
2769(49,)X
2889(260)X
2444 1212(server,)N
2681(261)X
2444 1302(server)N
2661(grabbing,)X
2986(261)X
2444 1392(sibling,)N
2701(261)X
2444 1482(stacking)N
2731(order,)X
2941(261)X
2444 1572(static)N
2633(color,)X
2838(261)X
2444 1662(static)N
2633(gray,)X
2816(261)X
2444 1752(status,)N
2666(261)X
2444 1842(stipple,)N
2697(261)X
2444 1932(tile,)N
2586(261)X
2444 2022(time,)N
2626(119)X
2444 2112(timestamp,)N
2817(261)X
2444 2202(true)N
2589(color,)X
2794(262)X
2444 2292(type,)N
2622(262)X
2444 2382(viewable,)N
2774(262)X
2444 2472(visible,)N
2697(262)X
2444 2562(window)N
2722(gravity,)X
2989(262)X
2444 2652(window)N
2722(manager,)X
3039(262)X
2384 2742(DestroyAll,)N
2780(15,)X
2900(132)X
2384 2832(DestroyNotify,)N
2887(25,)X
3007(148,)X
3167(160,)X
3327(161,)X
3487(162)X
2384 2922(DirectColor,)N
2807(16,)X
2927(17,)X
3047(53,)X
3167(54,)X
3287(58,)X
3407(59)X
2384 3012(DisableAccess,)N
2897(144)X
2384 3102(Dislay,)N
2633(260)X
2384 3192(Display,)N
2673(6,)X
2753(14,)X
2873(138,)X
3033(139,)X
3193(229,)X
3353(233,)X
3513(235,)X
3673(256)X
2384 3282(DisplayCells,)N
2837(8)X
2384 3372(DisplayHeight,)N
2891(10)X
2384 3462(DisplayHeightMM,)N
3033(10)X
2384 3552(DisplayOfScreen,)N
2977(12)X
2384 3642(DisplayPlanes,)N
2882(8)X
2384 3732(DisplayString,)N
2868(8)X
2384 3822(DisplayWidth,)N
2873(11)X
2384 3912(DisplayWidthMM,)N
3015(11)X
2384 4002(DoBlue,)N
2673(53,)X
2793(57,)X
2913(58,)X
3033(60)X
2384 4092(DoGreen,)N
2719(53,)X
2839(57,)X
2959(58,)X
3079(60)X
2384 4182(DoRed,)N
2651(53,)X
2771(57,)X
2891(58,)X
3011(60)X
2384 4272(DoesBackingStore,)N
3025(12)X
2384 4362(DoesSaveUnders,)N
2977(12)X
2384 4452(DontAllowExposures,)N
3118(140,)X
3278(142)X
2384 4542(DontPreferBlanking,)N
3074(140,)X
3234(141)X
2384 4632(Drawing:)N
2444 4722(Arcs,)N
2636(86)X
2444 4812(Image)N
2665(Text,)X
2852(105)X
2444 4902(Lines,)N
2662(84)X
2444 4992(Points,)N
2683(82)X
2444 5082(Rectangles,)N
2836(85)X
2444 5172(Strings,)N
2710(104)X
2444 5262(Text)N
2611(Items,)X
2829(103)X
3 f
12 s
2384 5442(E)N
1 f
10 s
2384 5622(EastGravity,)N
2807(29)X
2384 5712(EnableAccess,)N
2875(144)X
2384 5802(EnterNotify,)N
2825(121,)X
3003(123,)X
3181(152,)X
3359(153,)X
3537(154,)X
3715(155,)X
3892(158,)X
3 f
2244 6048(269)N

270 p
%%Page: 270 8
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(164)N
576 852(EnterWindowMask,)N
1244(120,)X
1404(122,)X
1564(123,)X
1724(153)X
576 942(Environment:)N
636 1032(DISPLAY,)N
1014(5)X
576 1122(Error)N
766(Codes,)X
1006(178)X
576 1212(Error)N
766(Handlers,)X
1096(178)X
576 1302(Error)N
766(Handling,)X
1104(2)X
576 1392(EvenOddRule,)N
1070(68,)X
1190(73,)X
1310(204)X
576 1482(Event:)N
636 1572(Categories,)N
1019(147)X
636 1662(Propagation,)N
1063(168)X
636 1752(Types,)N
872(146)X
576 1842(Event)N
783(Processing:)X
636 1932(ButtonPress,)N
1062(151)X
636 2022(ButtonRelease,)N
1143(151)X
636 2112(CirculateNotify,)N
1179(160)X
636 2202(CirculateRequest,)N
1228(164)X
636 2292(ClientMessage,)N
1152(167)X
636 2382(ColormapNotify,)N
1205(166)X
636 2472(Con\256gureNotify,)N
1205(160)X
636 2562(Con\256gureRequest,)N
1254(165)X
636 2652(CreateNotify,)N
1095(161)X
636 2742(DestroyNotify,)N
1139(161)X
636 2832(EnterNotify,)N
1059(152)X
636 2922(Expose,)N
912(159)X
636 3012(FocusIn,)N
934(155)X
636 3102(FocusOut,)N
987(155)X
636 3192(GraphicsExpose,)N
1202(159)X
636 3282(GravityNotify,)N
1130(162)X
636 3372(KeyPress,)N
979(151)X
636 3462(KeyRelease,)N
1060(151)X
636 3552(LeaveNotify,)N
1082(152)X
636 3642(MapNotify,)N
1032(162)X
636 3732(MapRequest,)N
1081(165)X
636 3822(MappingNotify,)N
1174(162)X
636 3912(MotionNotify,)N
1120(151)X
636 4002(NoExpose,)N
1010(159)X
636 4092(PropertyNotify,)N
1161(167)X
636 4182(ReparentNotify,)N
1175(163)X
636 4272(ResizeRequest,)N
1148(166)X
636 4362(UnmapNotify,)N
1121(163)X
636 4452(VisibilityNotify,)N
1186(164)X
576 4542(Event)N
783(Structures,)X
1148(147)X
576 4632(EventMaskOfScreen,)N
1285(12)X
576 4722(Expose,)N
852(26,)X
972(27,)X
1092(31,)X
1212(32,)X
1332(146,)X
1492(152,)X
1652(155,)X
1812(159,)X
1972(164)X
576 4812(Expose)N
832(Event,)X
1059(26,)X
1179(27,)X
1299(32)X
576 4902(Exposure)N
899(Event,)X
1126(18,)X
1246(28)X
576 4992(Exposure)N
899(Events,)X
1157(26,)X
1277(31,)X
1397(33,)X
1517(34)X
576 5082(ExposureMask,)N
1097(159)X
3 f
12 s
576 5262(F)N
1 f
10 s
576 5442(False,)N
787(12,)X
909(21,)X
1031(26,)X
1153(28,)X
1275(31,)X
1397(32,)X
1519(33,)X
1640(34,)X
1761(41,)X
1882(43,)X
2003(45,)X
2124(46,)X
576 5532(77,)N
699(79,)X
822(80,)X
945(115,)X
1108(120,)X
1271(122,)X
1434(124,)X
1597(125,)X
1760(131,)X
1922(148,)X
2084(151,)X
576 5622(152,)N
744(153,)X
912(161,)X
1080(162,)X
1248(163,)X
1416(165,)X
1583(166,)X
1750(171,)X
1917(172,)X
2084(174,)X
576 5712(175,)N
736(176,)X
896(187,)X
1056(211)X
2384 762(Files:)N
2444 852($HOME/.Xdefaults,)N
3114(202)X
2444 942(/etc/X?.hosts,)N
2900(142)X
2444 1032(/etc/ttys,)N
2737(133)X
2444 1122(/usr/lib/XerrorDB,)N
3058(181)X
2444 1212(/usr/lib/rgb.txt,)N
2943(55)X
2444 1302(<X11/X.h>,)N
2852(2,)X
2932(62,)X
3052(146,)X
3212(147,)X
3372(178)X
2444 1392(<X11/Xatom.h>,)N
3012(43,)X
3132(183)X
2444 1482(<X11/Xlib.h>,)N
2936(6,)X
3016(147,)X
3176(229)X
2444 1572(<X11/Xproto.h>,)N
3021(160)X
2444 1662(<X11/Xresource.h>,)N
3125(219)X
2444 1752(<X11/Xutil.h>,)N
2958(202,)X
3118(204,)X
3278(222)X
2444 1842(<X11/keysym.h>,)N
3043(137)X
2444 1932(<X11/keysymdef.h>,)N
3146(137,)X
3306(200)X
2444 2022(<sys/socket.h>,)N
2963(143)X
2384 2112(FillOpaqueStippled,)N
3050(67,)X
3170(72,)X
3290(82)X
2384 2202(FillSolid,)N
2702(67,)X
2822(72,)X
2942(106)X
2384 2292(FillStippled,)N
2800(67,)X
2920(72)X
2384 2382(FillTiled,)N
2703(67,)X
2823(72)X
2384 2472(Filling:)N
2444 2562(Arc,)N
2605(90)X
2444 2652(Arcs,)N
2636(90)X
2444 2742(Polygon,)N
2750(89)X
2444 2832(Rectangle,)N
2805(88)X
2444 2922(Rectangles,)N
2836(88)X
2384 3012(FlushGC,)N
2712(229,)X
2872(231)X
2384 3102(FocusChangeMask,)N
3038(155)X
2384 3192(FocusIn,)N
2682(124,)X
2842(125,)X
3002(130,)X
3162(131,)X
3322(155,)X
3482(156,)X
3642(157,)X
3802(158)X
2384 3282(FocusOut,)N
2740(124,)X
2905(125,)X
3070(130,)X
3235(131,)X
3400(152,)X
3564(155,)X
3728(156,)X
3892(157,)X
2384 3372(158,)N
2544(164)X
2384 3462(FontLeftToRight,)N
2970(95,)X
3090(101,)X
3250(102)X
2384 3552(FontRightToLeft,)N
2970(95,)X
3090(101,)X
3250(102)X
2384 3642(Fonts,)N
2601(103)X
2444 3732(freeing)N
2692(font)X
2841(information,)X
3259(92)X
2444 3822(getting)N
2686(information,)X
3104(92)X
2444 3912(unloading,)N
2804(92)X
2384 4002(ForgetGravity,)N
2878(20,)X
2998(29)X
2384 4092(Freeing:)N
2444 4182(Colors,)N
2697(60)X
2444 4272(Resources,)N
2814(18,)X
2934(36,)X
3054(37)X
3 f
12 s
2384 4452(G)N
1 f
10 s
2384 4632(GC,)N
2535(229)X
2384 4722(GContext,)N
2735(92)X
2384 4812(GXcopy,)N
2696(62,)X
2816(80,)X
2936(81,)X
3056(106)X
2384 4902(GetEmptyReq,)N
2882(233)X
2384 4992(GetKeyboardControl,)N
3101(235)X
2384 5082(GetReq,)N
2669(233,)X
2829(234)X
2384 5172(GetReqExtra,)N
2843(233,)X
3003(234)X
2384 5262(GetResReq,)N
2789(233)X
2384 5352(GetWindowAttributes,)N
3136(235)X
2384 5442(GrabFrozen,)N
2808(121,)X
2968(125)X
2384 5532(GrabInvalidTime,)N
2981(121,)X
3141(125)X
2384 5622(GrabModeAsync,)N
2977(120,)X
3137(122,)X
3297(124,)X
3457(126,)X
3617(150,)X
3777(151)X
2384 5712(GrabModeSync,)N
2932(120,)X
3092(122,)X
3252(124,)X
3412(126)X
2384 5802(GrabNotViewable,)N
3013(121,)X
3173(125)X
3 f
2244 6048(270)N

271 p
%%Page: 271 9
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(GrabSuccess,)N
1031(120)X
576 852(Grabbing:)N
636 942(Buttons,)N
924(121)X
636 1032(Keyboard,)N
993(124)X
636 1122(Keys,)N
841(125)X
636 1212(Pointer,)N
907(119,)X
1067(123)X
636 1302(Server,)N
886(129)X
576 1392(Graphics)N
886(Context,)X
1179(63)X
636 1482(path,)N
814(66)X
576 1572(GraphicsExpose,)N
1142(68,)X
1262(77,)X
1382(81,)X
1502(82,)X
1622(146,)X
1782(147,)X
1942(159,)X
2102(160)X
576 1662(GravityNotify,)N
1070(29,)X
1190(148,)X
1350(152,)X
1510(160,)X
1670(162,)X
1830(164)X
576 1752(GrayScale,)N
951(16,)X
1071(17,)X
1191(53,)X
1311(54,)X
1431(58,)X
1551(112)X
3 f
12 s
576 1932(H)N
1 f
10 s
576 2112(Hash)N
761(Lookup,)X
1050(240)X
576 2202(HeightMMOfScreen,)N
1280(13)X
576 2292(HeightOfScreen,)N
1138(13)X
576 2382(HeightValue,)N
1026(202)X
3 f
12 s
576 2562(I)N
1 f
10 s
576 2742(ImageByteOrder,)N
1156(8)X
576 2832(IncludeInferiors,)N
1134(68,)X
1254(77)X
576 2922(Initializing)N
947(Graphics)X
1257(Contexts,)X
1581(64)X
576 3012(Input)N
765(Control,)X
1049(146)X
576 3102(InputFocus,)N
976(176)X
576 3192(InputOnly,)N
955(17,)X
1085(18,)X
1215(22,)X
1345(23,)X
1475(24,)X
1605(28,)X
1735(36,)X
1865(37,)X
1995(40,)X
2124(42,)X
576 3282(62,)N
696(73,)X
816(74,)X
936(80,)X
1056(159,)X
1216(164,)X
1376(255,)X
1536(256,)X
1696(258)X
576 3372(InputOutput,)N
1007(23,)X
1127(24,)X
1247(26,)X
1367(40,)X
1487(68)X
576 3462(InstallColormap,)N
1136(21)X
576 3552(Internet)N
846(Addresses,)X
1216(142)X
576 3642(IsUnmapped,)N
1026(41)X
576 3732(IsUnviewable,)N
1062(41)X
576 3822(IsViewable,)N
982(41)X
3 f
12 s
576 4002(J)N
1 f
10 s
576 4182(JoinBevel,)N
936(66,)X
1056(71)X
576 4272(JoinMiter,)N
927(66,)X
1047(71)X
576 4362(JoinRound,)N
962(66,)X
1082(71)X
3 f
12 s
576 4542(K)N
1 f
10 s
576 4722(KeyMapStateMask,)N
1235(120,)X
1395(122,)X
1555(123)X
576 4812(KeyPress,)N
919(124,)X
1079(126,)X
1239(150,)X
1399(151,)X
1559(158,)X
1719(199)X
576 4902(KeyPressMask,)N
1097(151)X
576 4992(KeyRelease,)N
1000(124,)X
1160(150,)X
1320(151,)X
1480(158,)X
1640(198,)X
1800(199)X
576 5082(KeyReleaseMask,)N
1178(151)X
576 5172(Keyboard:)N
636 5262(Bell)N
789(Volume,)X
1087(133)X
636 5352(Bit)N
753(Vector,)X
1012(133)X
636 5442(Grabbing,)N
979(124)X
636 5532(Keyclick)N
946(Volume,)X
1244(133)X
636 5622(Ungrabbing,)N
1059(125)X
576 5712(KeymapNotify,)N
1097(158)X
576 5802(KeymapStateMask,)N
1226(158)X
2384 762(Keys:)N
2444 852(Grabbing,)N
2787(125)X
2444 942(Ungrabbing,)N
2867(126)X
3 f
12 s
2384 1122(L)N
1 f
10 s
2384 1302(LSBFirst,)N
2716(8,)X
2796(10)X
2384 1392(LeaveNotify,)N
2830(121,)X
2990(123,)X
3150(152,)X
3310(153,)X
3470(154,)X
3630(155,)X
3790(164)X
2384 1482(LeaveWindowMask,)N
3075(120,)X
3235(122,)X
3395(123,)X
3555(153)X
2384 1572(Limitations:)N
2444 1662(Cursor,)N
2702(114)X
2384 1752(Line:)N
2444 1842(Drawing,)N
2765(84)X
2384 1932(LineDoubleDash,)N
2972(66,)X
3092(67,)X
3212(71)X
2384 2022(LineOnOffDash,)N
2946(66,)X
3066(67,)X
3186(71)X
2384 2112(LineSolid,)N
2739(66,)X
2859(67,)X
2979(71)X
2384 2202(LockDisplay,)N
2838(233)X
2384 2292(LockMask,)N
2767(43,)X
2887(121,)X
3047(123,)X
3207(125,)X
3367(126,)X
3527(152,)X
3687(153)X
2384 2382(LowerHighest,)N
2883(33,)X
3003(34)X
3 f
12 s
2384 2562(M)N
1 f
10 s
2384 2742(MSBFirst,)N
2738(8,)X
2818(10)X
2384 2832(Macros:)N
2444 2922(AllPlanes,)N
2795(6,)X
2875(63)X
2444 3012(BitmapBitOrder,)N
3004(10)X
2444 3102(BitmapPad,)N
2839(10)X
2444 3192(BitmapUnit,)N
2861(10)X
2444 3282(BlackPixel,)N
2835(6)X
2444 3372(BlackPixelOfScreen,)N
3139(11)X
2444 3462(CellsOfScreen,)N
2952(11)X
2444 3552(Colormap,)N
2804(53)X
2444 3642(ConnectionNumber,)N
3116(7)X
2444 3732(DefaultColormap,)N
3045(7)X
2444 3822 0.1359(DefaultColormapOfScreen,)AN
3349(11)X
2444 3912(DefaultDepth,)N
2921(7)X
2444 4002 0.1813(DefaultDepthOfScreen,)AN
3225(11)X
2444 4092(DefaultGC,)N
2836(7)X
2444 4182(DefaultGCOfScreen,)N
3140(12)X
2444 4272(DefaultRootWindow,)N
3156(7)X
2444 4362(DefaultScreen,)N
2944(7)X
2444 4452 0.1477(DefaultScreenOfDisplay,)AN
3278(12)X
2444 4542(DefaultVisual,)N
2934(8)X
2444 4632 0.1548(DefaultVisualOfScreen,)AN
3238(12)X
2444 4722(DisplayCells,)N
2897(8)X
2444 4812(DisplayHeight,)N
2951(10)X
2444 4902(DisplayHeightMM,)N
3093(10)X
2444 4992(DisplayOfScreen,)N
3037(12)X
2444 5082(DisplayPlanes,)N
2942(8)X
2444 5172(DisplayString,)N
2928(8)X
2444 5262(DisplayWidth,)N
2933(11)X
2444 5352(DisplayWidthMM,)N
3075(11)X
2444 5442(DoesBackingStore,)N
3085(12)X
2444 5532(DoesSaveUnders,)N
3037(12)X
2444 5622(EventMaskOfScreen,)N
3153(12)X
2444 5712(HeightMMOfScreen,)N
3148(13)X
2444 5802(HeightOfScreen,)N
3006(13)X
3 f
2244 6048(271)N

272 p
%%Page: 272 10
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
636 762(ImageByteOrder,)N
1216(8)X
636 852(LockDisplay,)N
1090(233)X
636 942(MaxCmapsOfScreen,)N
1349(13)X
636 1032(MinCmapsOfScreen,)N
1335(13)X
636 1122(PlanesOfScreen,)N
1189(13)X
636 1212(ProtocolRevision,)N
1231(8)X
636 1302(ProtocolVersion,)N
1201(9)X
636 1392(QLength,)N
961(9,)X
1041(171)X
636 1482(RootWindow,)N
1107(9)X
636 1572(RootWindowOfScreen,)N
1411(13)X
636 1662(ScreenCount,)N
1090(9)X
636 1752(ScreenOfDisplay,)N
1229(13)X
636 1842(ServerVendor,)N
1127(9)X
636 1932(UnlockDisplay,)N
1161(233)X
636 2022(VendorRelease,)N
1167(9)X
636 2112(WhitePixel,)N
1036(10)X
636 2202(WhitePixelOfScreen,)N
1340(14)X
636 2292(WidthMMOfScreen,)N
1322(14)X
636 2382(WidthOfScreen,)N
1180(14)X
576 2472(MapNotify,)N
972(26,)X
1092(148,)X
1252(152,)X
1412(160,)X
1572(162,)X
1732(164)X
576 2562(MapRequest,)N
1021(25,)X
1141(26,)X
1261(164,)X
1421(165,)X
1581(166,)X
1741(169)X
576 2652(MapWindow,)N
1039(15,)X
1159(116)X
576 2742(MappingBusy,)N
1069(136,)X
1229(140)X
576 2832(MappingFailed,)N
1105(140)X
576 2922(MappingKeyboard,)N
1222(163)X
576 3012(MappingModi\256er,)N
1185(163)X
576 3102(MappingNotify,)N
1114(136,)X
1274(139,)X
1434(140,)X
1594(148,)X
1754(160,)X
1914(162,)X
2074(199)X
576 3192(MappingPointer,)N
1136(163)X
576 3282(MappingSuccess,)N
1159(136,)X
1319(140,)X
1479(162)X
576 3372(MaxCmapsOfScreen,)N
1289(13)X
576 3462(Menus,)N
834(128)X
576 3552(MinCmapsOfScreen,)N
1275(13)X
576 3642(Mod1Mask,)N
985(43,)X
1105(121,)X
1265(123,)X
1425(125,)X
1585(126,)X
1745(152,)X
1905(153)X
576 3732(Mod2Mask,)N
985(43,)X
1105(121,)X
1265(123,)X
1425(125,)X
1585(126,)X
1745(152,)X
1905(153)X
576 3822(Mod3Mask,)N
985(43,)X
1105(121,)X
1265(123,)X
1425(125,)X
1585(126,)X
1745(152,)X
1905(153)X
576 3912(Mod4Mask,)N
985(43,)X
1105(121,)X
1265(123,)X
1425(125,)X
1585(126,)X
1745(152,)X
1905(153)X
576 4002(Mod5Mask,)N
985(43,)X
1105(152,)X
1265(153)X
576 4092(Mod5Mask.,)N
1005(121,)X
1165(123,)X
1325(125,)X
1485(126)X
576 4182(MotionNotify,)N
1060(149,)X
1220(150,)X
1380(151,)X
1540(153,)X
1700(169)X
576 4272(Mouse:)N
636 4362(Programming,)N
1116(133)X
3 f
12 s
576 4542(N)N
1 f
10 s
576 4722(NULL,)N
830(200,)X
990(219)X
576 4812(NoEventMask,)N
1079(21)X
576 4902(NoExpose,)N
950(81,)X
1070(82,)X
1190(146,)X
1350(147,)X
1510(159)X
576 4992(NoSymbol,)N
962(138,)X
1122(139,)X
1282(200)X
576 5082(Nonconvex,)N
986(90)X
576 5172(None,)N
792(19,)X
914(20,)X
1035(21,)X
1156(24,)X
1277(35,)X
1398(36,)X
1519(41,)X
1640(43,)X
1761(45,)X
1882(46,)X
2003(50,)X
2124(51,)X
576 5262(55,)N
707(68,)X
838(76,)X
969(80,)X
1100(81,)X
1231(103,)X
1402(104,)X
1573(111,)X
1744(112,)X
1914(114,)X
2084(120,)X
576 5352(122,)N
744(123,)X
912(129,)X
1080(130,)X
1248(131,)X
1416(151,)X
1583(152,)X
1750(153,)X
1917(157,)X
2084(158,)X
576 5442(161,)N
736(165,)X
896(166,)X
1056(167,)X
1216(168,)X
1376(180,)X
1536(184)X
576 5532(NorthEastGravity,)N
1186(29)X
576 5622(NorthGravity,)N
1048(29)X
576 5712(NorthWestGravity,)N
1213(20,)X
1333(29)X
576 5802(NotUseful,)N
950(12,)X
1070(20,)X
1190(41)X
2384 762(NotifyAncestor,)N
2923(153,)X
3083(154,)X
3243(156)X
2384 852(NotifyDetailNone,)N
3003(156,)X
3163(157,)X
3323(158)X
2384 942(NotifyGrab,)N
2794(153,)X
2954(155,)X
3114(158)X
2384 1032(NotifyHint,)N
2775(151,)X
2935(152)X
2384 1122(NotifyInferior,)N
2879(153,)X
3039(154,)X
3199(156)X
2384 1212(NotifyNonlinear,)N
2954(153,)X
3114(154,)X
3274(156,)X
3434(157,)X
3594(158)X
2384 1302(NotifyNonlinearVirtual,)N
3181(153,)X
3341(154,)X
3501(156,)X
3661(157,)X
3821(158)X
2384 1392(NotifyNormal,)N
2878(152,)X
3038(153,)X
3198(154,)X
3358(155,)X
3518(156)X
2384 1482(NotifyPointer,)N
2864(156,)X
3024(157,)X
3184(158)X
2384 1572(NotifyPointerRoot,)N
3019(156,)X
3179(157,)X
3339(158)X
2384 1662(NotifyUngrab,)N
2874(153,)X
3034(155,)X
3194(158)X
2384 1752(NotifyVirtual,)N
2860(153,)X
3020(154,)X
3180(156)X
2384 1842(NotifyWhileGrabbed,)N
3106(155,)X
3266(156)X
3 f
12 s
2384 2022(O)N
1 f
10 s
2384 2202(Opposite,)N
2713(28,)X
2833(29,)X
2953(30,)X
3073(165,)X
3233(30)X
2384 2292(Output)N
2626(Control,)X
2910(146)X
2384 2382(OwnerGrabButtonMask,)N
3199(151)X
3 f
12 s
2384 2562(P)N
1 f
10 s
2384 2742(PPosition,)N
2729(188)X
2384 2832(PSize,)N
2606(188)X
2384 2922(PackData,)N
2732(234)X
2384 3012(Parent)N
2609(Window,)X
2925(39)X
2384 3102(ParentRelative,)N
2896(19,)X
3016(35,)X
3136(36,)X
3256(116)X
2384 3192(Paste)N
2573(Buffers,)X
2854(208)X
2384 3282(Pixel)N
2568(Values,)X
2831(63)X
2384 3372(PlaceOnBottom,)N
2935(160,)X
3095(165)X
2384 3462(PlaceOnTop,)N
2825(160,)X
2985(165)X
2384 3552(PlanesOfScreen,)N
2937(13)X
2384 3642(Pointer:)N
2444 3732(Grabbing,)N
2787(119,)X
2947(123)X
2444 3822(Ungrabbing,)N
2867(123)X
2384 3912(PointerMotionHintMask,)N
3210(120,)X
3370(122,)X
3530(123,)X
3690(148,)X
3850(151)X
2384 4002(PointerMotionMask,)N
3068(120,)X
3228(122,)X
3388(123,)X
3548(148,)X
3708(169)X
2384 4092(PointerRoot,)N
2810(15,)X
2930(130,)X
3090(131,)X
3250(157,)X
3410(158,)X
3570(180,)X
3730(187)X
2384 4182(PointerWindow,)N
2931(176)X
2384 4272(Polygon:)N
2444 4362(Drawing,)N
2765(84)X
2384 4452(PreferBlanking,)N
2914(140,)X
3074(141)X
2384 4542(PropModeAppend,)N
3016(48)X
2384 4632(PropModePrepend,)N
3025(48)X
2384 4722(PropModeReplace,)N
3021(48)X
2384 4812(Property:)N
2444 4902(Appending,)N
2840(47)X
2444 4992(Changing,)N
2795(47)X
2444 5082(Deleting,)N
2760(46,)X
2880(49)X
2444 5172(Format,)N
2715(47)X
2444 5262(Getting,)N
2724(45)X
2444 5352(Listing,)N
2710(47)X
2444 5442(Prepending,)N
2849(47)X
2444 5532(Replacing,)N
2809(47)X
2444 5622(Type,)N
2649(46,)X
2769(47)X
2384 5712(PropertyChangeMask,)N
3123(167)X
2384 5802(PropertyDelete,)N
2910(167)X
3 f
2244 6048(272)N

273 p
%%Page: 273 11
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(PropertyNewValue,)N
1236(167)X
576 852(PropertyNotify,)N
1101(47,)X
1221(48,)X
1341(49,)X
1461(166,)X
1621(167)X
576 942(Protocol:)N
636 1032(DECnet,)N
934(6)X
636 1122(TCP,)N
822(6)X
576 1212(ProtocolRevision,)N
1171(8)X
576 1302(ProtocolVersion,)N
1141(9)X
576 1392(PseudoColor,)N
1029(16,)X
1149(17,)X
1269(53,)X
1389(54,)X
1509(58,)X
1629(59)X
3 f
12 s
576 1572(Q)N
1 f
10 s
576 1752(QLength,)N
901(9,)X
981(171)X
576 1842(QueryFont,)N
963(235)X
576 1932(QueryKeymap,)N
1089(235)X
3 f
12 s
576 2112(R)N
1 f
10 s
576 2292(RaiseLowest,)N
1030(33,)X
1150(34)X
576 2382(Read-only)N
930(Colormap)X
1270(Cells,)X
1474(56)X
576 2472(Read/Write)N
966(Colormap)X
1306(Cells,)X
1510(55)X
576 2562(Rectangle:)N
636 2652(Drawing,)N
957(85)X
636 2742(Filling,)N
888(88)X
576 2832(RectangleIn,)N
1004(208)X
576 2922(RectangleOut,)N
1057(208)X
576 3012(RectanglePart,)N
1066(208)X
576 3102(Region,)N
847(204)X
576 3192(ReparentNotify,)N
1115(115,)X
1275(116,)X
1435(148,)X
1595(163)X
576 3282(ReparentNotify,,)N
1135(160)X
576 3372(ReplayKeyboard,)N
1160(127,)X
1320(128)X
576 3462(ReplayPointer,)N
1074(127,)X
1234(128)X
576 3552(ResizeRedirect,)N
1102(166)X
576 3642(ResizeRedirectMask,)N
1280(26,)X
1400(28,)X
1520(35,)X
1640(169)X
576 3732(ResizeRequest,)N
1088(26,)X
1208(28,)X
1328(164,)X
1488(166,)X
1648(169)X
576 3822(Resource)N
895(IDs,)X
1051(15,)X
1171(240)X
636 3912(Cursor,)N
894(2)X
636 4002(Font,)N
822(2)X
636 4092(Freeing,)N
921(18,)X
1041(36,)X
1161(37)X
636 4182(GContext,)N
987(2)X
636 4272(Pixmap,)N
920(2)X
636 4362(Window,)N
952(2)X
576 4452(RetainPermanent,)N
1168(15,)X
1288(132,)X
1448(133)X
576 4542(RetainTemporary,)N
1182(15,)X
1302(132,)X
1462(133)X
576 4632(RevertToNone,)N
1093(130,)X
1253(131)X
576 4722(RevertToParent,)N
1124(130,)X
1284(131)X
576 4812(RevertToPointerRoot,)N
1305(130,)X
1465(131)X
576 4902(RootWindow,)N
1047(9)X
576 4992(RootWindowOfScreen,)N
1351(13)X
3 f
12 s
576 5172(S)N
1 f
10 s
576 5352(Save)N
752(Unders,)X
1024(21)X
576 5442(Screen,)N
835(5,)X
915(6,)X
995(229,)X
1155(260)X
576 5532(ScreenCount,)N
1030(9)X
576 5622(ScreenFormat,)N
1066(229)X
576 5712(ScreenOfDisplay,)N
1169(13)X
576 5802(ScreenSaverActive,)N
1232(141)X
2384 762(ScreenSaverReset,)N
3004(141)X
2384 852(Selection:)N
2444 942(Converting,)N
2844(50)X
2444 1032(Getting)N
2704(the)X
2822(owner,)X
3063(50)X
2444 1122(Setting)N
2690(the)X
2808(owner,)X
3049(49)X
2384 1212(SelectionClear,)N
2896(50,)X
3016(148,)X
3176(166,)X
3336(167)X
2384 1302(SelectionNotify,)N
2931(51,)X
3051(148,)X
3211(166,)X
3371(168,)X
3531(175)X
2384 1392(SelectionRequest,)N
2980(50,)X
3100(51,)X
3220(148,)X
3380(166,)X
3540(167)X
2384 1482(Serial)N
2591(Number,)X
2894(178)X
2384 1572(ServerVendor,)N
2875(9)X
2384 1662(SetModeDelete,)N
2923(116)X
2384 1752(SetModeInsert,)N
2896(116)X
2384 1842(ShiftMask,)N
2757(43,)X
2877(121,)X
3037(123,)X
3197(125,)X
3357(126,)X
3517(152,)X
3677(153)X
2384 1932(SouthEastGravity,)N
2993(29)X
2384 2022(SouthGravity,)N
2855(29)X
2384 2112(SouthWestGravity,)N
3020(29)X
2384 2202(StaticColor,)N
2788(16,)X
2908(17,)X
3028(53,)X
3148(54)X
2384 2292(StaticGravity,)N
2851(29)X
2384 2382(StaticGray,)N
2767(16,)X
2887(17,)X
3007(53,)X
3127(54,)X
3247(112)X
2384 2472(StippleShape,)N
2846(73)X
2384 2562(StructureNotifyMask,)N
3105(160,)X
3265(161,)X
3425(162,)X
3585(163)X
2384 2652(SubStructureNotifyMask,)N
3229(163)X
2384 2742(SubstructureNotifyMask,)N
3216(148,)X
3376(160,)X
3536(161,)X
3696(162,)X
3856(163)X
2384 2832(SubstructureRedirectMask,)N
3288(21,)X
3417(25,)X
3546(26,)X
3675(28,)X
3804(31,)X
3932(32,)X
2384 2922(33,)N
2504(34,)X
2624(35,)X
2744(164,)X
2904(165,)X
3064(169)X
2384 3012(Success,)N
2678(47)X
2384 3102(SyncBoth,)N
2739(127)X
2384 3192(SyncHandle\(\),)N
2870(235)X
2384 3282(SyncKeyboard,)N
2901(127,)X
3061(128)X
2384 3372(SyncPointer,)N
2815(127,)X
2975(128)X
3 f
12 s
2384 3552(T)N
1 f
10 s
2384 3732(Tile)N
2533(Pixmaps,)X
2848(18)X
2384 3822(TileShape,)N
2749(73)X
2384 3912(TopIf,)N
2607(28,)X
2727(29,)X
2847(30,)X
2967(165,)X
3127(30)X
2384 4002(True,)N
2579(12,)X
2702(21,)X
2825(26,)X
2948(41,)X
3071(43,)X
3194(45,)X
3317(46,)X
3440(47,)X
3563(58,)X
3686(59,)X
3809(77,)X
3932(79,)X
2384 4092(80,)N
2511(81,)X
2638(82,)X
2765(95,)X
2892(115,)X
3059(116,)X
3226(120,)X
3393(122,)X
3560(124,)X
3726(125,)X
3892(131,)X
2384 4182(151,)N
2552(152,)X
2720(153,)X
2888(159,)X
3056(161,)X
3224(162,)X
3391(163,)X
3558(166,)X
3725(171,)X
3892(172,)X
2384 4272(174,)N
2544(175,)X
2704(176,)X
2864(187,)X
3024(201,)X
3184(211)X
2384 4362(TrueColor,)N
2758(16,)X
2878(17,)X
2998(53,)X
3118(54)X
3 f
12 s
2384 4542(U)N
1 f
10 s
2384 4722(USPosition,)N
2787(188)X
2384 4812(USSize,)N
2664(188)X
2384 4902(Ungrabbing:)N
2444 4992(Buttons,)N
2732(122)X
2444 5082(Keyboard,)N
2801(125)X
2444 5172(Keys,)N
2649(126)X
2444 5262(Pointer,)N
2715(123)X
2384 5352(Unix)N
2564(System)X
2819(Call:)X
2444 5442(fork,)N
2618(8,)X
2698(25)X
2444 5532(select,)N
2667(170)X
2384 5622(UnlockDisplay,)N
2909(233)X
2384 5712(UnmapGravity,)N
2905(29,)X
3025(163)X
2384 5802(UnmapNotify,)N
2872(27,)X
2995(28,)X
3118(29,)X
3241(148,)X
3404(152,)X
3567(155,)X
3730(160,)X
3892(163,)X
3 f
2244 6048(273)N

274 p
%%Page: 274 12
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(164)N
576 852(UnmapNotify)N
1041(Event,)X
1268(27,)X
1388(28)X
576 942(Unsorted,)N
910(76)X
3 f
12 s
576 1122(V)N
1 f
10 s
576 1302(V10)N
734(Compatibility:)X
636 1392(XAssocTable,)N
1113(240)X
636 1482(XCreateAssocTable,)N
1323(240)X
636 1572(XDeleteAssoc,)N
1140(241)X
636 1662(XDestroyAssocTable,)N
1367(241)X
636 1752(XDraw,)N
913(238)X
636 1842(XDrawDashed,)N
1154(238)X
636 1932(XDrawFilled,)N
1099(238,)X
1259(239)X
636 2022(XDrawPatterned,)N
1216(238)X
636 2112(XDrawTiled,)N
1082(238,)X
1242(239)X
636 2202(XLookupAssoc,)N
1179(240)X
636 2292(XMakeAssoc,)N
1113(240)X
576 2382(VendorRelease,)N
1107(9)X
576 2472(VertexCurved,)N
1071(239)X
576 2562(VertexDontDraw,)N
1174(239)X
576 2652(VertexDrawLastPoint,)N
1320(238)X
576 2742(VertexRelative,)N
1102(238,)X
1262(239)X
576 2832(VertexStartClosed,)N
1208(239)X
576 2922(VisibilityChangeMask,)N
1340(164)X
576 3012(VisibilityFullyObscured,)N
1393(164)X
576 3102(VisibilityNotify,)N
1126(22,)X
1246(152,)X
1406(155,)X
1566(160,)X
1726(164)X
576 3192(VisibilityPartiallyObscured,)N
1496(164)X
576 3282(VisibilityUnobscured,)N
1305(164)X
576 3372(VisibilityUnobscured.,)N
1325(164)X
576 3462(Visual,)N
825(16,)X
945(40,)X
1065(54,)X
1185(212,)X
1345(229)X
576 3552(Visual)N
805(Classes:)X
636 3642(GrayScale,)N
1011(16)X
636 3732(PseudoColor,)N
1089(16)X
636 3822(StaticColor,)N
1040(16)X
636 3912(StaticGray,)N
1019(16)X
636 4002(TrueColor,)N
1010(16)X
3 f
12 s
576 4182(W)N
1 f
10 s
576 4362(WestGravity,)N
1026(29)X
576 4452(WhenMapped,)N
1071(12,)X
1191(20,)X
1311(35,)X
1431(41)X
576 4542(WhitePixel,)N
976(10)X
576 4632(WhitePixelOfScreen,)N
1280(14)X
576 4722(WidthMMOfScreen,)N
1262(14)X
576 4812(WidthOfScreen,)N
1120(14)X
576 4902(WidthValue,)N
1008(202)X
576 4992(WindingRule,)N
1047(68,)X
1167(73,)X
1287(204)X
576 5082(Window,)N
892(17,)X
1012(18)X
636 5172(Background,)N
1068(36)X
636 5262(Characteristics,)N
1153(17)X
636 5352(Computing)N
1015(Placement,)X
1389(203)X
636 5442(De\256ne)N
870(Cursor,)X
1128(114)X
636 5532(IDs,)N
792(240)X
636 5622(Initial)N
847(Location,)X
1172(202,)X
1332(203)X
636 5712(Input)N
825(Only,)X
1025(22)X
636 5802(Managers,)N
993(128)X
2444 762(Mapping)N
2753(to)X
2835(screen,)X
3081(18)X
2444 852(Name,)N
2676(184,)X
2836(185)X
2444 942(RootWindow,)N
2915(9)X
2444 1032(Unde\256ne)N
2758(Cursor,)X
3016(114)X
2444 1122(XRootWindow,)N
2973(9)X
3 f
12 s
2384 1302(X)N
1 f
10 s
2384 1482 0.1750(XActivateScreenSaver,)AN
3156(141)X
2384 1572(XAddHost,)N
2771(142,)X
2931(143)X
2384 1662(XAddHosts,)N
2802(143)X
2384 1752(XAddPixel,)N
2784(213)X
2384 1842(XAddToExtensionList,)N
3153(229)X
2384 1932(XAddToSaveSet,)N
2967(117)X
2384 2022(XAllPlanes,)N
2793(6)X
2384 2112(XAllocColor,)N
2842(54,)X
2962(55,)X
3082(56,)X
3202(60,)X
3322(204)X
2384 2202(XAllocColorCells,)N
3006(55,)X
3126(58,)X
3246(60,)X
3366(197)X
2384 2292(XAllocColorPlanes,)N
3051(54,)X
3171(55,)X
3291(58,)X
3411(59,)X
3531(60,)X
3651(195,)X
3811(197)X
2384 2382(XAllocID,)N
2745(229,)X
2905(234)X
2384 2472(XAllocNamedColor,)N
3074(55,)X
3194(56,)X
3314(60,)X
3434(204)X
2384 2562(XAllowEvents,)N
2900(119,)X
3060(120,)X
3220(124,)X
3380(127,)X
3540(128)X
2384 2652(XAppendVertex,)N
2955(238)X
2384 2742(XArc,)N
2603(82,)X
2723(88,)X
2843(91)X
2384 2832(XAssocTable,)N
2861(240,)X
3021(241)X
2384 2922(XAssocTables,)N
2892(240)X
2384 3012(XAutoRepeatOff,)N
2977(135)X
2384 3102(XAutoRepeatOn,)N
2963(135)X
2384 3192(XBell,)N
2615(135)X
2384 3282(XBitmapBitOrder,)N
3002(10)X
2384 3372(XBitmapPad,)N
2837(10)X
2384 3462(XBitmapUnit,)N
2859(10)X
2384 3552(XBlackPixel,)N
2833(6)X
2384 3642(XBlackPixelOfScreen,)N
3137(11)X
2384 3732(XButtonPressedEvent,)N
3131(152)X
2384 3822(XButtonReleasedEvent,)N
3176(152)X
2384 3912(XCNOENT,)N
2807(223)X
2384 4002(XCNOMEM,)N
2842(222)X
2384 4092(XCellsOfScreen,)N
2950(11)X
2384 4182(XChangeActivePointerGrab,)N
3333(123,)X
3493(124,)X
3653(151,)X
3813(153)X
2384 4272(XChangeGC,)N
2838(69,)X
2958(76,)X
3078(234)X
2384 4362(XChangeKeyboardControl,)N
3288(133,)X
3448(134,)X
3608(136)X
2384 4452(XChangeKeyboardMapping,)N
3333(138,)X
3493(139,)X
3653(162)X
2384 4542(XChangePointerControl,)N
3202(131,)X
3362(132)X
2384 4632(XChangeProperty,)N
3003(47,)X
3123(48,)X
3243(167,)X
3403(183)X
2384 4722(XChangeSaveSet,)N
2985(116,)X
3145(117)X
2384 4812(XChangeWindowAttributes,)N
3323(35,)X
3443(166,)X
3603(168)X
2384 4902(XChangeWindowAttributes\),)N
3350(55)X
2384 4992(XChar2b,)N
2718(94,)X
2838(104,)X
2998(107)X
2384 5082(XCharStruct,)N
2829(93,)X
2949(94,)X
3069(95,)X
3189(97,)X
3309(101,)X
3469(102)X
2384 5172(XCheckIfEvent,)N
2928(170,)X
3088(171,)X
3248(172)X
2384 5262(XCheckMaskEvent,)N
3052(174)X
2384 5352(XCheckTypedEvent,)N
3079(175)X
2384 5442(XCheckTypedWindowEvent,)N
3355(175)X
2384 5532(XCheckWindowEvent,)N
3150(173,)X
3310(174)X
2384 5622(XCirculateEvent,)N
2963(160)X
2384 5712(XCirculateRequestEvent,)N
3221(165)X
2384 5802(XCirculateSubwindows,)N
3189(33,)X
3309(160,)X
3469(164)X
3 f
2244 6048(274)N

275 p
%%Page: 275 13
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(XCirculateSubwindowsDown,)N
1577(34,)X
1697(160,)X
1857(164)X
576 852(XCirculateSubwindowsUp,)N
1479(33,)X
1599(34,)X
1719(160,)X
1879(164)X
576 942(XCirculateWindow,)N
1244(34)X
576 1032(XClearArea,)N
1005(79,)X
1125(80)X
576 1122(XClearVertexFlag,)N
1209(238)X
576 1212(XClearWindow,)N
1124(35,)X
1244(80)X
576 1302(XClientMessageEvent,)N
1337(167)X
576 1392(XClipBox,)N
944(204)X
576 1482(XCloseDisplay,)N
1105(14,)X
1225(15,)X
1345(25,)X
1465(225)X
576 1572(XColor,)N
856(53,)X
976(57,)X
1096(58,)X
1216(60,)X
1336(61)X
576 1662(XColormapEvent,)N
1181(166)X
576 1752(XCon\256gureEvent,)N
1181(161)X
576 1842(XCon\256gureRequestEvent,)N
1439(165)X
576 1932(XCon\256gureWindow,)N
1293(21,)X
1436(28,)X
1578(30,)X
1720(160,)X
1902(162,)X
2084(165,)X
576 2022(166)N
576 2112(XConnectionNumber,)N
1306(7)X
576 2202(XConvertSelection,)N
1230(50,)X
1350(51,)X
1470(167,)X
1630(168)X
576 2292(XCopyArea,)N
1004(68,)X
1124(77,)X
1244(80,)X
1364(81,)X
1484(82,)X
1604(148,)X
1764(159,)X
1924(160)X
576 2382(XCopyColormapAndFree,)N
1448(54,)X
1568(55)X
576 2472(XCopyGC,)N
958(68,)X
1078(69)X
576 2562(XCopyPlane,)N
1025(62,)X
1145(68,)X
1265(77,)X
1385(81,)X
1505(82,)X
1625(148,)X
1785(159,)X
1945(160)X
576 2652(XCreateAssocTable,)N
1263(240)X
576 2742(XCreateBitmapFromData,)N
1444(215)X
576 2832(XCreateColormap,)N
1204(53,)X
1324(54,)X
1444(194)X
576 2922(XCreateFontCursor,)N
1248(111,)X
1408(112)X
576 3012(XCreateGC,)N
995(64,)X
1115(68,)X
1235(69,)X
1355(76,)X
1475(85,)X
1595(86,)X
1715(159,)X
1875(204,)X
2035(234)X
576 3102(XCreateGlyphCursor,)N
1302(112,)X
1462(113)X
576 3192(XCreateImage,)N
1085(211,)X
1245(212,)X
1405(214)X
576 3282(XCreatePixmap,)N
1128(61,)X
1248(62,)X
1368(234)X
576 3372(XCreatePixmapCursor,)N
1346(111,)X
1506(112,)X
1666(114)X
576 3462(XCreateRegion,)N
1115(205)X
576 3552(XCreateSimpleWindow,)N
1386(22,)X
1506(24,)X
1626(35,)X
1746(118,)X
1906(161,)X
2066(203)X
576 3642(XCreateWindow,)N
1161(22,)X
1282(23,)X
1403(24,)X
1524(35,)X
1644(55,)X
1764(118,)X
1924(161,)X
2084(168,)X
576 3732(203)N
576 3822(XCreateWindowEvent,)N
1347(161,)X
1507(170,)X
1667(171)X
576 3912(XDefaultColormap,)N
1235(7)X
576 4002 0.1406(XDefaultColormapOfScreen,)AN
1539(11)X
576 4092(XDefaultDepth,)N
1111(7)X
576 4182 0.1845(XDefaultDepthOfScreen,)AN
1415(11)X
576 4272 0.1806(XDefaultGCOfScreen,)AN
1330(12)X
576 4362(XDefaultRootWindow,)N
1346(7)X
576 4452(XDefaultScreen,)N
1134(5,)X
1214(6,)X
1294(7)X
576 4542 0.1522(XDefaultScreenOfDisplay,)AN
1468(12)X
576 4632(XDefaultVisual,)N
1124(8)X
576 4722 0.1591(XDefaultVisualOfScreen,)AN
1428(12)X
576 4812(XDe\256neCursor,)N
1106(24,)X
1226(114)X
576 4902(XDeleteAssoc,)N
1080(241)X
576 4992(XDeleteContext,)N
1137(222,)X
1297(223)X
576 5082(XDeleteProperty,)N
1160(48,)X
1280(49,)X
1400(167)X
576 5172(XDestroyAssocTable,)N
1307(241)X
576 5262(XDestroyImage,)N
1129(214)X
576 5352(XDestroyRegion,)N
1159(205)X
576 5442(XDestroySubwindows,)N
1341(25,)X
1461(161)X
576 5532(XDestroyWindow,)N
1204(25,)X
1324(161)X
576 5622(XDestroyWindowEvent,)N
1391(162)X
576 5712(XDisableAccessControl,)N
1391(144,)X
1551(145)X
576 5802(XDisplayCells,)N
1087(8)X
2384 762(XDisplayHeight,)N
2949(10)X
2384 852(XDisplayName,)N
2923(178,)X
3083(181)X
2384 942(XDisplayOfScreen,)N
3035(12)X
2384 1032(XDisplayPlanes,)N
2940(8)X
2384 1122(XDisplayString,)N
2926(8)X
2384 1212(XDisplayWidth,)N
2931(11)X
2384 1302(XDisplayWidthMM,)N
3073(11)X
2384 1392(XDoesBackingStore,)N
3083(12)X
2384 1482(XDoesSaveUnders,)N
3035(12)X
2384 1572(XDraw,)N
2661(238,)X
2821(239)X
2384 1662(XDrawArc,)N
2782(67,)X
2902(86,)X
3022(87,)X
3142(88)X
2384 1752(XDrawArcs,)N
2813(86,)X
2933(87,)X
3053(88)X
2384 1842(XDrawDashed,)N
2902(238)X
2384 1932(XDrawFilled,)N
2847(238,)X
3007(239)X
2384 2022(XDrawImageString,)N
3057(105,)X
3217(106,)X
3377(107)X
2384 2112(XDrawImageString16,)N
3137(105,)X
3297(106,)X
3457(107)X
2384 2202 0.2083(XDrawImageText,,)AN
3029(106)X
2384 2292 0.1953(XDrawImageText16,)AN
3089(106)X
2384 2382(XDrawLine,)N
2808(67,)X
2928(84,)X
3048(85)X
2384 2472(XDrawLines,)N
2839(84,)X
2959(85,)X
3079(238)X
2384 2562(XDrawPatterned,)N
2964(238)X
2384 2652(XDrawPoint,)N
2829(79,)X
2949(82,)X
3069(83,)X
3189(230)X
2384 2742(XDrawPoints,)N
2860(79,)X
2980(82,)X
3100(83)X
2384 2832(XDrawRectangle,)N
2982(85,)X
3102(86)X
2384 2922(XDrawRectangle,,)N
3002(67)X
2384 3012(XDrawRectangles,)N
3013(85,)X
3133(86)X
2384 3102(XDrawSegments,)N
2972(67,)X
3092(84,)X
3212(85,)X
3332(238)X
2384 3192(XDrawString,)N
2856(104,)X
3016(105)X
2384 3282(XDrawString16,)N
2936(104,)X
3096(105)X
2384 3372(XDrawText,)N
2808(67,)X
2928(103,)X
3088(104)X
2384 3462(XDrawText16,)N
2888(67,)X
3008(103,)X
3168(104)X
2384 3552(XDrawTiled,)N
2830(238,)X
2990(239)X
2384 3642(XESetCloseDisplay,)N
3064(225)X
2384 3732(XESetCopyGC,)N
2917(226)X
2384 3822(XESetCreateFont,)N
2989(226)X
2384 3912(XESetCreateGC,)N
2954(225)X
2384 4002(XESetError,)N
2803(228)X
2384 4092(XESetErrorString,)N
2998(228)X
2384 4182(XESetEventToWire,)N
3070(227)X
2384 4272(XESetFlushGC,)N
2921(228)X
2384 4362(XESetFreeFont,)N
2922(226)X
2384 4452(XESetFreeGC,)N
2887(226)X
2384 4542(XESetWireToEvent,)N
3070(227)X
2384 4632(XEmptyRegion,)N
2926(207)X
2384 4722(XEnableAccessControl,)N
3177(144)X
2384 4812(XEnterWindowEvent,)N
3119(153,)X
3279(154,)X
3439(155)X
2384 4902(XEqualRegion,)N
2900(207)X
2384 4992(XError,)N
2652(170,)X
2812(178,)X
2972(180,)X
3132(228,)X
3292(235)X
2384 5082(XErrorEvent,)N
2839(178)X
2384 5172(XEvent,)N
2682(147,)X
2855(170,)X
3028(171,)X
3201(172,)X
3374(173,)X
3547(174,)X
3720(175,)X
3892(176,)X
2384 5262(227,)N
2544(228)X
2384 5352(XEventMaskOfScreen,)N
3151(12)X
2384 5442(XExposeEvent,)N
2905(159)X
2384 5532(XExtCodes,)N
2793(225,)X
2953(236,)X
3113(237)X
2384 5622(XExtData,)N
2745(229)X
2384 5712(XFetchBuffer,)N
2870(209)X
2384 5802(XFetchBytes,)N
2842(208,)X
3002(209)X
3 f
2244 6048(275)N

276 p
%%Page: 276 14
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(XFetchName,)N
1044(184,)X
1204(185)X
576 852(XFillArc,)N
905(67,)X
1025(90,)X
1145(91)X
576 942(XFillArcs,)N
936(68,)X
1056(90,)X
1176(91)X
576 1032(XFillPolygon,)N
1050(67,)X
1170(68,)X
1290(89,)X
1410(90)X
576 1122(XFillRectangle,)N
1105(67,)X
1225(79,)X
1345(88,)X
1465(89)X
576 1212(XFillRectangles,)N
1136(88,)X
1256(89)X
576 1302(XFindContext,)N
1073(222,)X
1233(223)X
576 1392(XFindOnExtensionList,)N
1362(229)X
576 1482(XFlush,)N
851(2,)X
931(169,)X
1091(170)X
576 1572(XFocusInEvent,)N
1119(155,)X
1279(156,)X
1439(157,)X
1599(158)X
576 1662(XFocusOutEvent,)N
1172(155,)X
1332(156,)X
1492(157,)X
1652(158)X
576 1752(XFontProp,)N
971(93,)X
1091(94)X
576 1842(XFontStruct,)N
1011(92,)X
1131(93,)X
1251(94,)X
1371(100,)X
1531(101,)X
1691(229)X
576 1932(XFontstruct,)N
998(92)X
576 2022 0.1985(XForceScreenSaver,)AN
1259(141)X
576 2112(XFree,)N
829(14,)X
961(39,)X
1093(47,)X
1225(119,)X
1397(138,)X
1569(143,)X
1741(177,)X
1913(188,)X
2084(209,)X
576 2202(210)N
576 2292(XFreeColormap,)N
1137(55,)X
1257(166)X
576 2382(XFreeColors,)N
1030(54,)X
1150(60)X
576 2472(XFreeCursor,)N
1035(21,)X
1155(113)X
576 2562(XFreeExtensionList,)N
1261(224)X
576 2652(XFreeFont,)N
963(97,)X
1083(113,)X
1243(226)X
576 2742(XFreeFontInfo,)N
1097(92,)X
1217(93)X
576 2832(XFreeFontNames,)N
1186(93,)X
1306(99)X
576 2922(XFreeFontPath,)N
1105(99,)X
1225(100)X
576 3012(XFreeGC,)N
928(69)X
576 3102(XFreeModi\256erMapping,)N
1386(139,)X
1546(140)X
576 3192(XFreePixmap,)N
1061(62,)X
1181(214,)X
1341(215)X
576 3282(XGCValues,)N
1008(63,)X
1128(64,)X
1248(69,)X
1368(159)X
576 3372(XGContextFromGC,)N
1269(92)X
576 3462(XGeometry,)N
995(203)X
576 3552(XGetAtomName,)N
1164(45)X
576 3642(XGetClassHint,)N
1105(192,)X
1265(193)X
576 3732(XGetDefault,)N
1031(202,)X
1191(204)X
576 3822 0.2024(XGetErrorDatabaseText,)AN
1402(178,)X
1562(180,)X
1722(181)X
576 3912(XGetErrorText,)N
1107(178,)X
1267(180,)X
1427(228)X
576 4002(XGetFontPath,)N
1078(99,)X
1198(100)X
576 4092(XGetFontProperty,)N
1212(97,)X
1332(98)X
576 4182(XGetGeometry,)N
1111(41,)X
1231(42)X
576 4272(XGetIconName,)N
1125(185,)X
1285(186)X
576 4362(XGetIconSizes,)N
1102(191,)X
1262(192)X
576 4452(XGetImage,)N
991(108,)X
1151(109,)X
1311(110,)X
1471(211)X
576 4542(XGetInputFocus,)N
1150(131)X
576 4632(XGetKeyboardControl,)N
1351(134,)X
1511(135)X
576 4722(XGetKeyboardMapping,)N
1396(137,)X
1556(138)X
576 4812(XGetModi\256erMapping,)N
1359(140)X
576 4902(XGetMotionEvents,)N
1243(151,)X
1403(176,)X
1563(177)X
576 4992(XGetNormalHints,)N
1208(189,)X
1368(191)X
576 5082(XGetPixel,)N
954(212,)X
1114(213)X
576 5172(XGetPointerControl,)N
1265(132)X
576 5262(XGetPointerMapping,)N
1310(136)X
576 5352(XGetProperty,)N
1066(48,)X
1186(167)X
576 5442(XGetScreenSaver,)N
1192(141)X
576 5532(XGetSelectionOwner,)N
1307(50)X
576 5622(XGetSizeHints,)N
1101(190,)X
1261(191)X
576 5712(XGetStandardColormap,)N
1395(196)X
576 5802(XGetSubImage,)N
1115(109,)X
1275(110)X
2384 762(XGetTransientForHint,)N
3154(193)X
2384 852(XGetVisualInfo,)N
2941(210)X
2384 942(XGetWMHints,)N
2918(188)X
2384 1032(XGetWindowAttributes,)N
3194(20,)X
3314(39,)X
3434(40,)X
3554(41)X
2384 1122(XGetWindowProperty,)N
3150(45,)X
3270(46,)X
3390(47,)X
3510(183)X
2384 1212(XGetZoomHints,)N
2962(190,)X
3122(191)X
2384 1302(XGrabButton,)N
2860(121,)X
3020(122,)X
3180(123,)X
3340(124,)X
3500(150)X
2384 1392(XGrabKey,)N
2777(125,)X
2937(126)X
2384 1482(XGrabKeyboard,)N
2982(119,)X
3164(124,)X
3346(125,)X
3528(126,)X
3710(152,)X
3892(153,)X
2384 1572(155,)N
2544(158)X
2384 1662(XGrabPointer,)N
2883(119,)X
3052(120,)X
3220(121,)X
3388(122,)X
3556(123,)X
3724(124,)X
3892(152,)X
2384 1752(153,)N
2544(155,)X
2704(169)X
2384 1842(XGrabServer,)N
2853(129)X
2384 1932(XGraphicsExposeEvent,)N
3195(147,)X
3355(160)X
2384 2022(XGravityEvent,)N
2914(162)X
2384 2112(XHeightMMOfScreen,)N
3146(13)X
2384 2202(XHeightOfScreen,)N
3004(13)X
2384 2292(XHostAddress,)N
2896(142)X
2384 2382(XIconSize,)N
2763(191)X
2384 2472(XId,)N
2549(240,)X
2709(241)X
2384 2562(XIfEvent,)N
2723(170,)X
2883(171)X
2384 2652(XImage,)N
2683(107,)X
2843(108,)X
3003(109,)X
3163(110,)X
3323(211,)X
3483(212,)X
3643(213,)X
3803(214)X
2384 2742(XImageByteOrder,)N
3022(8)X
2384 2832(XInitExtension,)N
2913(225,)X
3073(237)X
2384 2922(XInstallColormap,)N
3002(21,)X
3122(35,)X
3242(53,)X
3362(117,)X
3522(118,)X
3682(166)X
2384 3012(XInternAtom,)N
2856(45)X
2384 3102(XIntersectRegion,)N
2990(206)X
2384 3192(XKeyPressedEvent,)N
3048(152,)X
3208(199)X
2384 3282(XKeyReleasedEvent,)N
3093(152,)X
3253(199)X
2384 3372(XKeyboardControl,)N
3043(133,)X
3203(134)X
2384 3462(XKeyboardState,)N
2959(135)X
2384 3552(XKeycodeToKeysym,)N
3124(201)X
2384 3642(XKeymapEvent,)N
2941(158)X
2384 3732(XKeysymToKeycode,)N
3124(201)X
2384 3822(XKeysymToString,)N
3033(200)X
2384 3912(XKillClient,)N
2801(132,)X
2961(133)X
2384 4002(XLeaveWindowEvent,)N
3142(153,)X
3302(154,)X
3462(155)X
2384 4092(XListExtensions,)N
2957(224)X
2384 4182(XListFonts,)N
2783(98,)X
2903(99)X
2384 4272(XListFontsWithInfo,)N
3077(93)X
2384 4362(XListHosts,)N
2788(143)X
2384 4452(XListInstalledColormaps,)N
3233(118,)X
3393(119)X
2384 4542(XListProperties,)N
2931(47)X
2384 4632(XLoadFont,)N
2793(92,)X
2913(93,)X
3033(98)X
2384 4722(XLoadQueryFont,)N
2994(92,)X
3114(93,)X
3234(97,)X
3354(226)X
2384 4812(XLookUpAssoc,)N
2945(240,)X
3105(241)X
2384 4902(XLookupColor,)N
2913(56,)X
3033(57)X
2384 4992(XLookupKeysym,)N
2998(199)X
2384 5082(XLookupString,)N
2926(137,)X
3086(199,)X
3246(200)X
2384 5172(XLowerWindow,)N
2968(33,)X
3088(160,)X
3248(165)X
2384 5262(XMakeAssoc,)N
2861(240)X
2384 5352(XMapEvent,)N
2816(162)X
2384 5442(XMapRaised,)N
2847(27,)X
2967(161,)X
3127(162,)X
3287(165)X
2384 5532(XMapRequestEvent,)N
3074(166)X
2384 5622(XMapSubwindows,)N
3042(27,)X
3162(162,)X
3322(165)X
2384 5712(XMapWindow,)N
2905(18,)X
3025(24,)X
3145(26,)X
3265(27,)X
3385(116,)X
3545(162,)X
3705(165)X
2384 5802(XMappingEvent,)N
2958(163)X
3 f
2244 6048(276)N

277 p
%%Page: 277 15
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(XMaskEvent,)N
1039(174)X
576 852(XMatchVisualInfo,)N
1222(7,)X
1302(16,)X
1422(210,)X
1582(211)X
576 942(XMaxCmapsOfScreen,)N
1347(13)X
576 1032(XMinCmapsOfScreen,)N
1333(13)X
576 1122(XModi\256erKeymap,)N
1226(139,)X
1386(140)X
576 1212(XMoveResizeWindow,)N
1354(31,)X
1476(32,)X
1598(161,)X
1760(162,)X
1922(165,)X
2084(166,)X
576 1302(234)N
576 1392(XMoveWindow,)N
1137(30,)X
1257(31,)X
1377(160,)X
1537(165,)X
1697(234)X
576 1482(XNegative,)N
964(202)X
576 1572(XNewModi\256erMapping,)N
1395(139)X
576 1662(XNextEvent,)N
1017(2,)X
1097(169,)X
1257(170)X
576 1752(XNoExposeEvent,)N
1195(160)X
576 1842(XNoOp,)N
870(14)X
576 1932(XOffsetRegion,)N
1106(205)X
576 2022(XOpenDisplay,)N
1097(5,)X
1177(6,)X
1257(7,)X
1337(8,)X
1417(16,)X
1537(52,)X
1657(146)X
576 2112(XParseColor,)N
1030(203,)X
1190(204)X
576 2202(XParseGeometry,)N
1169(202,)X
1329(203)X
576 2292(XPeekEvent,)N
1017(170,)X
1177(171)X
576 2382(XPeekIfEvent,)N
1071(170,)X
1231(171,)X
1391(172)X
576 2472(XPending,)N
936(2,)X
1016(169,)X
1176(170)X
576 2562(XPlanesOfScreen,)N
1187(13)X
576 2652(XPoint,)N
842(82,)X
962(83,)X
1082(85)X
576 2742(XPointInRegion,)N
1140(207)X
576 2832(XPointerMovedEvent,)N
1319(151,)X
1479(152)X
576 2922(XPolygonRegion,)N
1171(204)X
576 3012(XPropertyEvent,)N
1137(167)X
576 3102(XProtocolRevision,)N
1229(8)X
576 3192(XProtocolVersion,)N
1199(9)X
576 3282(XPutBackEvent,)N
1132(172,)X
1292(173)X
576 3372(XPutImage,)N
981(108,)X
1141(211)X
576 3462(XPutPixel,)N
944(213)X
576 3552(XQLength,)N
959(9)X
576 3642(XQueryBestCursor,)N
1235(111,)X
1395(113,)X
1555(114)X
576 3732(XQueryBestSize,)N
1155(73)X
576 3822(XQueryBestStipple,)N
1243(74)X
576 3912(XQueryBestTile,)N
1146(73,)X
1266(74)X
576 4002(XQueryColor,)N
1057(60,)X
1177(61)X
576 4092(XQueryColors,)N
1088(60,)X
1208(61)X
576 4182(XQueryExtension,)N
1195(224,)X
1355(225)X
576 4272(XQueryFont,)N
1021(92,)X
1141(93,)X
1261(97)X
576 4362(XQueryKeymap,)N
1147(136)X
576 4452(XQueryPointer,)N
1106(42,)X
1226(43,)X
1346(151)X
576 4542(XQueryTextExtents,)N
1262(101,)X
1422(102,)X
1582(103,)X
1742(106)X
576 4632(XQueryTextExtents16,)N
1342(101,)X
1502(102,)X
1662(103)X
576 4722(XQueryTree,)N
1023(39)X
576 4812(XRaiseWindow,)N
1128(2,)X
1208(32,)X
1328(33,)X
1448(160,)X
1608(165)X
576 4902(XReadBitmapFile,)N
1198(214,)X
1358(215)X
576 4992(XRebindKeysym,)N
1172(200)X
576 5082(XRecolorCursor,)N
1146(111,)X
1306(113)X
576 5172(XRectInRegion,)N
1119(207,)X
1279(208)X
576 5262(XRectangle,)N
995(82,)X
1115(89)X
576 5352 0.1413(XRefreshKeyboardMapping,)AN
1530(163,)X
1690(199)X
576 5442(XRemoveFromSaveSet,)N
1372(117)X
576 5532(XRemoveHost,)N
1092(143)X
576 5622(XRemoveHosts,)N
1123(144)X
576 5712(XReparentEvent,)N
1151(163)X
576 5802(XReparentWindow,)N
1240(115,)X
1400(116,)X
1560(163)X
2384 762(XResetScreenSaver,)N
3062(141)X
2384 852(XResizeRequestEvent,)N
3141(166)X
2384 942(XResizeWindow,)N
2972(2,)X
3052(31,)X
3172(160,)X
3332(162,)X
3492(165,)X
3652(166)X
2384 1032(XRestackWindows,)N
3043(34,)X
3163(160,)X
3323(165)X
2384 1122(XRootWindow,)N
2913(9)X
2384 1212(XRootWindowOfScreen,)N
3217(13)X
2384 1302(XRotateBuffers,)N
2932(209)X
2384 1392(XRotateWindowProperties,)N
3292(48,)X
3412(49,)X
3532(167)X
2384 1482(XSaveContext,)N
2891(222,)X
3051(223)X
2384 1572(XScreenOfDisplay,)N
3035(13)X
2384 1662(XSegment,)N
2762(82,)X
2882(85)X
2384 1752(XSelectInput,)N
2865(26,)X
3003(146,)X
3181(148,)X
3359(150,)X
3537(151,)X
3715(153,)X
3892(155,)X
2384 1842(158,)N
2552(159,)X
2720(160,)X
2888(161,)X
3056(162,)X
3224(163,)X
3391(164,)X
3558(165,)X
3725(166,)X
3892(167,)X
2384 1932(168,)N
2544(169)X
2384 2022(XSelectionClearEvent,)N
3141(167)X
2384 2112(XSelectionEvent,)N
2967(168)X
2384 2202(XSelectionRequestEvent,)N
3225(167)X
2384 2292(XSendEvent,)N
2829(167,)X
2989(168,)X
3149(175,)X
3309(176)X
2384 2382(XServerVendor,)N
2933(9)X
2384 2472(XSetAccessControl,)N
3056(144)X
2384 2562(XSetAfterFunction,)N
3038(177)X
2384 2652(XSetArcMode,)N
2892(77)X
2384 2742(XSetBackground,)N
2976(71)X
2384 2832(XSetBackgroundPixel,)N
3140(234)X
2384 2922(XSetClassHint,)N
2899(192)X
2384 3012(XSetClipMask,)N
2899(76)X
2384 3102(XSetClipOrigin,)N
2930(75,)X
3050(76)X
2384 3192(XSetClipRectangles,)N
3073(68,)X
3193(69,)X
3313(76,)X
3433(77)X
2384 3282(XSetCloseDownMode,)N
3149(15,)X
3269(132)X
2384 3372(XSetCommand,)N
2917(186)X
2384 3462(XSetCopyGC,)N
2868(228)X
2384 3552(XSetDashes,)N
2816(68,)X
2936(69,)X
3056(71,)X
3176(72)X
2384 3642(XSetErrorHandler,)N
3013(178)X
2384 3732(XSetFillRule,)N
2845(72,)X
2965(73)X
2384 3822(XSetFillStyle,)N
2858(72)X
2384 3912(XSetFont,)N
2730(75)X
2384 4002(XSetFontPath,)N
2872(99)X
2384 4092(XSetForeground,)N
2958(71)X
2384 4182(XSetFunction,)N
2868(70)X
2384 4272(XSetGraphicsExposures,)N
3208(77,)X
3328(78,)X
3448(159)X
2384 4362 0.1806(XSetIOErrorHandler,)AN
3098(178,)X
3258(181)X
2384 4452(XSetIconName,)N
2919(185)X
2384 4542(XSetIconSizes,)N
2896(191)X
2384 4632(XSetInputFocus,)N
2944(130,)X
3104(131,)X
3264(152,)X
3424(157)X
2384 4722(XSetLineAttributes,)N
3051(71)X
2384 4812(XSetModi\256erMapping,)N
3153(139,)X
3313(140,)X
3473(162)X
2384 4902(XSetNormalHints,)N
3002(189,)X
3162(190)X
2384 4992(XSetPlaneMask,)N
2940(70,)X
3060(71)X
2384 5082(XSetPointerMapping,)N
3104(136,)X
3264(162)X
2384 5172(XSetRegion,)N
2815(205)X
2384 5262(XSetScreenSaver,)N
2986(140,)X
3146(141)X
2384 5352(XSetSelectionOwner,)N
3101(15,)X
3221(49,)X
3341(50,)X
3461(167,)X
3621(49)X
2384 5442(XSetSizeHints,)N
2895(190)X
2384 5532(XSetStandardColormap,)N
3189(196,)X
3349(197)X
2384 5622(XSetStandardProperties,)N
3194(22,)X
3314(182,)X
3474(184)X
2384 5712(XSetStandatrdColormap,)N
3211(196)X
2384 5802(XSetState,)N
2744(70)X
3 f
2244 6048(277)N

278 p
%%Page: 278 16
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(XSetStipple,)N
1002(74,)X
1122(75)X
576 852(XSetSubwindowMode,)N
1345(77)X
576 942(XSetTSOrigin,)N
1078(75)X
576 1032(XSetTile,)N
905(74)X
576 1122(XSetTransientForHint,)N
1332(193)X
576 1212(XSetWMHints,)N
1096(187,)X
1256(188,)X
1416(192)X
576 1302(XSetWindowAttributes,)N
1391(18,)X
1530(19,)X
1669(22,)X
1808(26,)X
1946(35,)X
2084(161,)X
576 1392(162,)N
736(163,)X
896(165,)X
1056(166,)X
1216(168)X
576 1482(XSetWindowBackground,)N
1444(35,)X
1564(36)X
576 1572(XSetWindowBackgroundPixmap,)N
1688(36)X
576 1662(XSetWindowBorder,)N
1275(36,)X
1395(37)X
576 1752(XSetWindowBorderPixmap,)N
1519(37)X
576 1842(XSetWindowBorderWidth,)N
1475(32,)X
1595(161,)X
1755(165)X
576 1932(XSetWindowColormap,)N
1372(55)X
576 2022(XSetZoomHints,)N
1140(189,)X
1300(190)X
576 2112(XShrinkRegion,)N
1118(206)X
576 2202(XSizeHints,)N
985(184,)X
1145(188)X
576 2292(XStandardColormap,)N
1279(194,)X
1439(195,)X
1599(196)X
576 2382(XStandardColormaps,)N
1310(195)X
576 2472(XStoreBuffer,)N
1053(208)X
576 2562(XStoreBytes,)N
1025(208)X
576 2652(XStoreColor,)N
1025(57,)X
1145(58,)X
1265(204)X
576 2742(XStoreColors,)N
1056(55,)X
1176(57,)X
1296(59,)X
1416(197)X
576 2832(XStoreName,)N
1035(184,)X
1195(185)X
576 2922(XStoreNamedColor,)N
1257(59,)X
1377(60)X
576 3012(XStringToKeysym,)N
1225(200)X
576 3102(XSubImage,)N
999(213)X
576 3192(XSubtractRegion,)N
1172(206)X
576 3282(XSync,)N
834(2,)X
914(169,)X
1074(170)X
576 3372(XSynchronize,)N
1075(177,)X
1235(235)X
576 3462(XTextExtent,)N
1030(101)X
576 3552(XTextExtents,)N
1061(100,)X
1221(102)X
576 3642(XTextExtents16,)N
1141(100,)X
1301(101,)X
1461(102)X
576 3732(XTextItem,)N
968(104)X
576 3822(XTextItem16,)N
1048(104)X
576 3912(XTextWidth,)N
1021(100)X
576 4002(XTextWidth16,)N
1101(100)X
576 4092(XTimeCoord,)N
1043(177)X
576 4182(XTranslateCoordinates,)N
1360(37,)X
1480(38)X
576 4272(XUnde\256neCursor,)N
1186(114)X
576 4362(XUngrabButton,)N
1132(122,)X
1292(123)X
576 4452(XUngrabKey,)N
1049(126,)X
1209(127)X
576 4542(XUngrabKeyboard,)N
1232(15,)X
1352(125,)X
1512(155)X
576 4632(XUngrabPointer,)N
1146(15,)X
1266(123,)X
1426(151,)X
1586(153)X
576 4722(XUngrabServer,)N
1125(15,)X
1245(129)X
576 4812(XUninstallColormap,)N
1287(55,)X
1407(117,)X
1567(118,)X
1727(166)X
576 4902(XUnionRegion,)N
1105(206)X
576 4992(XUniqueContext,)N
1163(223)X
576 5082(XUnloadFont,)N
1056(92,)X
1176(98)X
576 5172(XUnmapEvent,)N
1097(163)X
576 5262(XUnmapSubwindows,)N
1323(27,)X
1443(28,)X
1563(163)X
576 5352(XUnmapWindow,)N
1186(27,)X
1306(115,)X
1466(116,)X
1626(163)X
576 5442(XValue,)N
866(202,)X
1026(203)X
576 5532(XVendorRelease,)N
1165(9)X
576 5622(XVisibilityEvent,)N
1162(164)X
576 5712(XVisualInfo,)N
1017(209,)X
1177(210)X
576 5802(XWMClass,)N
994(192)X
2384 762(XWMClassHint,)N
2944(192)X
2384 852(XWMClassHints,)N
2975(192)X
2384 942(XWMHints,)N
2802(186)X
2384 1032(XWarpPointer,)N
2892(129,)X
3052(130)X
2384 1122(XWhitePixel,)N
2842(10)X
2384 1212(XWhitePixelOfScreen,)N
3146(14)X
2384 1302(XWidthMMOfScreen,)N
3128(14)X
2384 1392(XWidthOfScreen,)N
2986(14)X
2384 1482(XWindowAttributes,)N
3078(40)X
2384 1572(XWindowChanges,)N
3034(28,)X
3154(30)X
2384 1662(XWindowEvent,)N
2945(2,)X
3025(169,)X
3185(170,)X
3345(173)X
2384 1752(XWriteBitmapFile,)N
3024(214,)X
3184(215)X
2384 1842(XXorRegion,)N
2838(206)X
2384 1932(XYBitmap,)N
2775(108)X
2384 2022(XYPixmap,)N
2784(108,)X
2944(109,)X
3104(110,)X
3264(211,)X
3424(212)X
2384 2112(X_CopyArea,)N
2852(160)X
2384 2202(X_CopyPlane,)N
2873(160)X
2384 2292(X_MapWindow,)N
2945(233)X
2384 2382(Xlibint.h,)N
2710(231,)X
2870(233)X
2384 2472(XlistFontWithInfo,)N
3019(93)X
2384 2562(XlistFontsWithInfo,)N
3050(93)X
2384 2652(Xproto.h,)N
2711(231,)X
2871(232,)X
3031(233)X
2384 2742(XrmConvert,)N
2829(217,)X
2989(219)X
2384 2832(XrmGetDataBase,)N
2995(217,)X
3155(220,)X
3315(221)X
2384 2922(XrmGetResource,)N
2986(218,)X
3146(219,)X
3306(220)X
2384 3012(XrmGetSearchList,)N
3030(218,)X
3190(219)X
2384 3102 0.1813(XrmGetSearchResource,)AN
3205(219)X
2384 3192(XrmLoadDataBase,)N
3044(221)X
2384 3282(XrmMergeDataBases,)N
3120(217,)X
3280(218,)X
3440(220)X
2384 3372(XrmParseCommand,)N
3078(221,)X
3238(222)X
2384 3462(XrmPutDataBase,)N
2985(217,)X
3145(221)X
2384 3552(XrmPutResource,)N
2976(218)X
2384 3642(XrmPutResources,)N
3007(218)X
2384 3732(XrmRegisterTypeConverter,)N
3324(217,)X
3484(220)X
3 f
12 s
2384 3912(Y)N
1 f
10 s
2384 4092(YNegative,)N
2772(202)X
2384 4182(YSorted,)N
2691(76)X
2384 4272(YValue,)N
2674(202,)X
2834(203)X
2384 4362(YXBanded,)N
2785(76)X
2384 4452(YXSorted,)N
2749(76)X
3 f
12 s
2384 4632(Z)N
1 f
10 s
2384 4812(ZPixmap,)N
2717(108,)X
2877(109,)X
3037(110,)X
3197(212)X
3 f
12 s
2384 4992(_)N
1 f
10 s
2384 5172(_XAllocScratch,)N
2941(236)X
2384 5262(_XFlushGCCache,)N
3011(229)X
2384 5352(_XRead,)N
2687(235)X
2384 5442(_XReply,)N
2713(228,)X
2873(234,)X
3033(235)X
2384 5532(_XSend,)N
2682(234)X
3 f
2244 6048(278)N

279 p
%%Page: 279 17
10 s 0 xH 0 xS 3 f
576 474(C)N
654(Language)X
1011(X)X
1089(Interface)X
3535(V11,)X
3713(Release)X
3992(1)X
1 f
576 762(_Xdebug,)N
910(177)X
3 f
12 s
576 942(f)N
1 f
10 s
576 1122(fork,)N
750(8,)X
830(25)X
3 f
12 s
576 1302(s)N
1 f
10 s
576 1482(select,)N
799(170)X
576 1572(special,)N
839(4)X
3 f
12 s
576 1752(x)N
1 f
10 s
576 1932(xDoSomethingReply,)N
1291(232)X
576 2022(xDoSomethingReq,)N
1229(232,)X
1389(234)X
576 2112(xEvent,)N
843(227,)X
1003(228)X
576 2202(xFalse,)N
825(235)X
576 2292(xGenericReply,)N
1102(232)X
576 2382(xGetWindowAttributesReply,)N
1559(235)X
576 2472(xReply,)N
847(234,)X
1007(235)X
576 2562(xReq,)N
785(230,)X
945(232)X
576 2652(xResourceReq,)N
1084(232)X
576 2742(xTrue,)N
808(235)X
576 2832(xmail,)N
798(216)X
3 f
12 s
576 3012(\304)N
1 f
10 s
576 3192(\304/.Xdefaults,)N
997(202)X
3 f
2244 6048(279)N

17 p
%%Trailer
xt

xs
