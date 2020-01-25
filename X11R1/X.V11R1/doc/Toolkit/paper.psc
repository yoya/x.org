%!PS-Adobe-1.0
%%Creator: orpheus:mento (Al Mento,,,,)
%%Title: stdin (ditroff)
%%CreationDate: Mon Sep 14 23:06:09 1987
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

%%Page: 1 1
10 s 0 xH 0 xS 1 f
11 s
10 s
32(--)Y
4323(--)X
3 f
13 s
1352 1536(X)N
1453(Toolkit)X
1803(Library)X
2181(-)X
2242(C)X
2343(Language)X
2808(Interface)X
12 s
1784 1728(Toolkit)N
2107(Beta)X
2318(Version)X
2662(0.1)X
1830 1920(X)N
1923(Protocol)X
2295(Version)X
2639(11)X

2 p
%%Page: 2 2
12 s 0 xH 0 xS 3 f
11 s
1 f
10 s
0 32(--)N
4323(--)X
9 s
555 1564(The)N
685(X)X
755(Window)X
1021(System)X
1251(is)X
1317(a)X
1367(trademark)X
1677(of)X
1755(MIT.)X
555 1688(Copyright)N
9 f
869(\323)X
1 f
948(1985,)X
1131(1986,)X
1314(1987,)X
1497(Massachusetts)X
1934(Institute)X
2191(of)X
2272(Technology,)X
2655(Cambridge,)X
3014(Massachusetts,)X
3469(and)X
3594(Digital)X
3815(Equip-)X
555 1784(ment)N
717(Corporation,)X
1101(Maynard,)X
1397(Massachusetts.)X
555 1908(Permission)N
898(to)X
977(use,)X
1114(copy,)X
1295(modify)X
1526(and)X
1653(distribute)X
1948(this)X
2075(documentation)X
2526(for)X
2633(any)X
2760(purpose)X
3011(and)X
3137(without)X
3379(fee)X
3489(is)X
3559(hereby)X
3777(granted,)X
555 2004(provided)N
835(that)X
967(the)X
1079(above)X
1275(copyright)X
1575(notice)X
1775(appears)X
2019(in)X
2099(all)X
2195(copies)X
2403(and)X
2530(that)X
2661(both)X
2812(that)X
2943(copyright)X
3242(notice)X
3441(and)X
3568(this)X
3695(permission)X
555 2100(notice)N
749(appear)X
959(in)X
1033(supporting)X
1359(documentation,)X
1823(and)X
1945(that)X
2071(the)X
2177(name)X
2351(of)X
2429(M.I.T.)X
2633(or)X
2711(Digital)X
2929(not)X
3039(be)X
3125(used)X
3275(in)X
3349(in)X
3423(advertising)X
3761(or)X
3839(publi-)X
555 2196(city)N
688(pertaining)X
1005(to)X
1086(distribution)X
1443(of)X
1528(the)X
1641(software)X
1913(without)X
2157(speci\256c,)X
2419(written)X
2647(prior)X
2811(permission.)X
3187(M.I.T)X
3379(and)X
3507(Digital)X
3731(makes)X
3939(no)X
555 2292(representations)N
1015(about)X
1199(the)X
1311(suitability)X
1623(of)X
1707(the)X
1819(software)X
2091(described)X
2391(herein)X
2595(for)X
2703(any)X
2831(purpose.)X
3119(It)X
3187(is)X
3259(provided)X
3539(``as)X
3671(is'')X
3791(without)X
555 2388(express)N
789(or)X
867(implied)X
1105(warranty.)X

1 p
%%Page: 1 3
9 s 0 xH 0 xS 1 f
11 s
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 672(1.)N
665(Overview)X
1 f
555 796(The)N
726(X)X
822(Toolkit)X
1116(provides)X
1452(the)X
1593(base)X
1782(functionality)X
2267(necessary)X
2641(to)X
2743(build)X
2958(a)X
3030(wide)X
3234(variety)X
3512(of)X
3618(application)X
555 892(environments.)N
1106(The)X
1270(X)X
1360(Toolkit)X
1648(is)X
1734(fully)X
1928(extensible)X
2313(and)X
2467(supportive)X
2865(of)X
2964(the)X
3098(independent)X
3555(development)X
555 988(of)N
656(new)X
830(or)X
931(extended)X
1277(components.)X
1775(This)X
1960(is)X
2047(accomplished)X
2560(by)X
2676(de\256ning)X
2993(a)X
3060(few,)X
3241(easy-to-use)X
3669(interfaces)X
555 1084(that)N
714(mask)X
926 0.2885(implementation)AX
1508(details)X
1765(from)X
1962(both)X
2145(applications)X
2598(and)X
2751(common)X
3086(component)X
3503(implementors.)X
555 1180(By)N
681(following)X
1046(a)X
1107(small)X
1321(set)X
1441(of)X
1536(conventions,)X
2006(a)X
2067(programmer)X
2524(can)X
2668(extend)X
2925(the)X
3055(X)X
3140(Toolkit)X
3423(in)X
3514(new)X
3682(ways)X
3884(and)X
555 1276(have)N
743(these)X
946(extensions)X
1340(function)X
1656(smoothly)X
2007(with)X
2186(the)X
2316(existing)X
2618(facilities.)X
555 1400(The)N
720(X)X
811(Toolkit)X
1099(is)X
1185(a)X
1251(library)X
1513(package)X
1828(layered)X
2114(on)X
2229(top)X
2369(of)X
2469(the)X
2604(X)X
2694(Window)X
3024(System.)X
3355(This)X
3539(layer)X
3742(extends)X
555 1496(the)N
686(basic)X
890(abstractions)X
1334(provided)X
1670(by)X
1781(X)X
1867(and,)X
2039(thus,)X
2231(provides)X
2557(the)X
2688(``next)X
2921(layer'')X
3178(of)X
3274(functionality)X
3749(by)X
3860(sup-)X
555 1592(plying)N
807(mechanisms)X
1269(for)X
1396(intercomponent)X
1975(and)X
2127(intracomponent)X
2706(interactions.)X
3187(As)X
3309(time)X
3492(progresses,)X
3909(we)X
555 1688(expect)N
815(to)X
914(supply)X
1179(a)X
1248(reasonably)X
1659(complete)X
2013(and)X
2170(cohesive)X
2508(set)X
2636(of)X
2739(sample)X
3019(widgets)X
3323(that)X
3485(can)X
3636(be)X
3748(used)X
3938(as)X
555 1784(the)N
691(basis)X
895(for)X
1025(quick)X
1249(applications)X
1704(development.)X
2232(In)X
2333(the)X
2468(X)X
2558(Toolkit,)X
2868(a)X
2934(``widget'')X
3317(is)X
3403(a)X
3469(combination)X
3938(of)X
555 1880(an)N
660(X)X
745(window)X
1050(\(or)X
1174(subwindow\))X
1630(and)X
1779(its)X
1885(associated)X
2269(semantics.)X
555 2004(The)N
726(X)X
823(Toolkit)X
1118(provides)X
1455(\257exibility)X
1834(and)X
1995(extensibility.)X
2511(To)X
2643(the)X
2785(extent)X
3035(possible,)X
3380(it)X
3464(also)X
3640(is)X
3732(``policy)X
555 2100(free''.)N
815(The)X
974(application)X
1389(environment,)X
1879(not)X
2014(the)X
2144(X)X
2229(Toolkit,)X
2534(de\256nes,)X
2827(implements,)X
3284(and)X
3433(enforces:)X
10 f
555 2224(g)N
1 f
775(Policy)X
10 f
555 2348(g)N
1 f
775(Consistency)X
10 f
555 2472(g)N
1 f
775(Style)X
555 2596(Each)N
759(individual)X
1146(widget)X
1413 0.2885(implementation)AX
1996(de\256nes)X
2272(its)X
2383(own)X
2561(policy.)X
2853(The)X
3017(X)X
3107(Toolkit)X
3395(design)X
3652(allows)X
3909(for)X
555 2692(the)N
685(development)X
1163(of)X
1258(radically)X
1589(differing)X
1919(widget)X
2181 0.2333(implementations.)AX
3 f
555 2884(2.)N
665(Introduction)X
1 f
555 3008(The)N
716(X)X
803(Toolkit)X
1088(provides)X
1415(tools)X
1611(that)X
1768(simplify)X
2087(the)X
2219(design)X
2473(of)X
2570(application)X
2987(user)X
3156(interfaces)X
3521(in)X
3613(the)X
3744(X)X
3830(Win-)X
555 3104(dow)N
732(System)X
1018(programming)X
1524(environment.)X
2040(It)X
2120(assists)X
2371(application)X
2790(programmers)X
3285(by)X
3398(providing)X
3766(a)X
3830(com-)X
555 3200(monly)N
803(used)X
986(set)X
1106(of)X
1201(underlying)X
1605(user-interface)X
2110(functions)X
2460(that)X
2615(enable)X
2867(you)X
3021(to)X
3112(manage:)X
10 f
555 3324(g)N
1 f
775(Widgets)X
10 f
555 3448(g)N
1 f
775(Standard)X
1110(operations)X
10 f
555 3572(g)N
1 f
775(Toolkit)X
1058 0.3317(initialization)AX
10 f
555 3696(g)N
1 f
775(Memory)X
10 f
555 3820(g)N
1 f
775(Input)X
983(and)X
1132(events)X
10 f
555 3944(g)N
1 f
775(Widget)X
1057(geometry)X
10 f
555 4068(g)N
1 f
775(Input)X
983(focus)X
10 f
555 4192(g)N
1 f
775(Selections)X
10 f
555 4316(g)N
1 f
775(Resources)X
1158(and)X
1307(resource)X
1626(conversion)X
10 f
555 4440(g)N
1 f
775(Translation)X
1199(of)X
1294(events)X
10 f
555 4564(g)N
1 f
775(Graphics)X
1114(contexts)X
10 f
555 4688(g)N
1 f
775(Pixmaps)X
10 f
555 4812(g)N
1 f
775(Errors)X
555 4936(At)N
665(present,)X
963(the)X
1093(X)X
1178(Toolkit)X
1461(consists)X
1762(of:)X
10 f
555 5060(g)N
1 f
775(A)X
860(set)X
980(of)X
1075(intrinsic)X
1387(mechanisms)X
1845(for)X
1969(building)X
2286(widgets)X
10 f
555 5184(g)N
1 f
775(An)X
904(architectural)X
1367(model)X
1610(for)X
1734(constructing)X
2192(and)X
2341(composing)X
2750(widgets)X
10 f
555 5308(g)N
1 f
775(A)X
860(consistent)X
1235(interface)X
1565(for)X
1689(programming)X
555 5432(The)N
717(intrinsics)X
1066(mechanisms)X
1527(are)X
1659(intended)X
1988(for)X
2115(the)X
2248(widget)X
2513(programmer.)X
3017(The)X
3178(architectural)X
3643(model)X
3888(lets)X
555 5528(the)N
691(widget)X
959(programmer)X
1422(design)X
1680(new)X
1854(widgets)X
2156(by)X
2272(using)X
2491(the)X
2627(intrinsics)X
2978(or)X
3078(by)X
3193(combining)X
3598(other)X
3806(widg-)X
555 5624(ets.)N
729(The)X
898(application)X
1323(interface)X
1663(layers)X
1905(built)X
2100(on)X
2220(top)X
2365(of)X
2470(the)X
2610(X)X
2705(Toolkit)X
2998(include)X
3290(a)X
3361(coordinated)X
3809(set)X
3938(of)X
555 5720(widgets)N
863(and)X
1024(composition)X
1495(policies.)X
1848(Some)X
2083(of)X
2190(these)X
2405(widgets)X
2712(and)X
2872(policies)X
3180(are)X
3320(application)X
3746(domain)X
555 5816(speci\256c,)N
868(while)X
1086(others)X
1323(are)X
1452(common)X
1783(across)X
2024(a)X
2085(number)X
2376(of)X
2471(application)X
2886(domains.)X
3 f
2261 6144(1)N

2 p
%%Page: 2 4
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
718(X)X
807(Toolkit)X
1094(provides)X
1423(an)X
1532(architectural)X
1999(model)X
2246(that)X
2405(is)X
2490(\257exible)X
2781(enough)X
3066(to)X
3161(accommodate)X
3677(a)X
3742(number)X
555 768(of)N
650(different)X
975(application)X
1390(interface)X
1720(layers.)X
1996(In)X
2091(addition,)X
2425(the)X
2555(supplied)X
2876(set)X
2996(of)X
3091(toolkit)X
3345(functions)X
3695(are:)X
10 f
555 892(g)N
1 f
775(Functionally)X
1244(complete)X
1590(and)X
1739(policy)X
1982(free)X
10 f
555 1016(g)N
1 f
775 0.3021(Stylistically)AX
1221(and)X
1370(functionally)X
1819(consistent)X
2194(with)X
2373(the)X
2503(X)X
2588(Window)X
2913(System)X
3195(primitives)X
10 f
555 1140(g)N
1 f
775(Portable)X
1091(across)X
1332(a)X
1393(wide)X
1586(variety)X
1853(of)X
1948(languages,)X
2344(computer)X
2699(architectures,)X
3193(and)X
3342(operating)X
3697(systems)X
555 1264(Applications)N
1028(that)X
1183(use)X
1322(the)X
1452(X)X
1537(Toolkit)X
1820(must)X
2014(include)X
2296(the)X
2426(following)X
2791(header)X
3047(\256les:)X
10 f
555 1388(g)N
1 f
5 f
790(<X/Xlib.h>)X
1 f
10 f
555 1512(g)N
1 f
5 f
790(<X/Intrinsic.h>)X
1 f
10 f
555 1636(g)N
1 f
5 f
790(<X/Xatoms.h>)X
1 f
10 f
555 1760(g)N
1 f
5 f
790(<X/Atoms.h>)X
1 f
10 f
555 1884(g)N
1 f
5 f
790(<X/TopLevel.h>)X
1 f
555 2008(The)N
718(applications)X
1171(should)X
1432(also)X
1600(include)X
1886(the)X
2020(additional)X
2400(headers)X
2694(for)X
2822(each)X
3009(widget)X
3275(class)X
3472(that)X
3631(they)X
3809(are)X
3942(to)X
555 2104(use)N
706(\(for)X
871(example,)X
5 f
1244(<X/Label.h>)X
1 f
1768(or)X
5 f
1893(<X/Scroll.h>)X
1 f
2387(\).)X
2494(The)X
2665(object)X
2915(library)X
3183(\256le)X
3329(is)X
3421(named)X
5 f
3707(libXtk.a)X
1 f
555 2200(and,)N
726(on)X
836(a)X
897(UNIX-based)X
1366(system,)X
1655(is)X
1736(normally)X
2077(referenced)X
2469(as)X
2564('-lXtk'.)X
3 f
555 2392(3.)N
665(Terminology)X
1 f
555 2516(The)N
714(following)X
1079(terms)X
1297(are)X
1426(used)X
1609(throughout)X
2018(this)X
2168(manual:)X
555 2640(Object)N
1707(A)X
1794(software)X
2120(data)X
2291(abstraction)X
2702(that)X
2859(consists)X
3162(of)X
3259(private)X
3528(data)X
3699(and)X
3850(pub-)X
1707 2736(lic)N
1820(and)X
1971(private)X
2240(routines)X
2548(that)X
2705(operate)X
2988(on)X
3100(the)X
3232(private)X
3501(data.)X
3716(Users)X
3938(of)X
1707 2832(the)N
1843(abstraction)X
2258(can)X
2408(interact)X
2701(with)X
2886(the)X
3022(object)X
3266(only)X
3451(through)X
3753(calls)X
3942(to)X
1707 2928(the)N
1863(object's)X
2190(public)X
2459(routines.)X
2835(In)X
2955(the)X
3110(X)X
3220(Toolkit,)X
3550(some)X
3783(of)X
3903(the)X
1707 3024(object's)N
2027(public)X
2288(routines)X
2612(are)X
2759(called)X
3010(directly)X
3320(by)X
3448(the)X
3596(application,)X
1707 3120(while)N
1934(others)X
2180(are)X
2318(called)X
2560(indirectly)X
2930(when)X
3150(the)X
3288(application)X
3711(calls)X
3903(the)X
1707 3216(common)N
2056(routines.)X
2424(In)X
2537(general,)X
2858(if)X
2952(a)X
3031(function)X
3365(is)X
3464(common)X
3813(to)X
3922(all)X
1707 3312(widgets,)N
2035(an)X
2150(application)X
2575(uses)X
2757(a)X
2827(single)X
3069(intrinsic)X
3390(routine)X
3671(to)X
3771(invoke)X
1707 3408(the)N
1841(function)X
2161(for)X
2289(all)X
2404(types)X
2616(of)X
2715(widgets.)X
3059(If)X
3143(a)X
3208(function)X
3528(is)X
3613(unique)X
3878(to)X
3972(a)X
1707 3504(single)N
1955(widget)X
2232(type,)X
2443(the)X
2588(widget)X
2865(exports)X
3161(the)X
3306(function)X
3637(as)X
3747(another)X
1707 3600(``Xt'')N
1933(routine.)X
555 3724(Widget)N
1707(An)X
1844(object)X
2090(providing)X
2463(a)X
2532(user-interface)X
3045(abstraction)X
3462(\(for)X
3622(example,)X
3972(a)X
1707 3820(scroll)N
1925(bar\).)X
555 3944(Toolkit)N
1707(The)X
1874(name)X
2095(of)X
2198(the)X
2336(user-interface)X
2849(library)X
3114(that)X
3277(is)X
3365(the)X
3502(subject)X
3781(of)X
3883(this)X
1707 4040(manual.)N
555 4164(Application)N
996(Programmer)X
1709(A)X
1796(programmer)X
2255(that)X
2412(uses)X
2587(the)X
2719(X)X
2805(Toolkit)X
3089(to)X
3181(produce)X
3487(the)X
3618(application)X
1707 4260(user-interface.)N
555 4384(Widget)N
843(Programmer)X
1713(A)X
1804(programmer)X
2267(that)X
2428(modi\256es)X
2760(the)X
2896(X)X
2987(Toolkit)X
3276(or)X
3376(adds)X
3564(new)X
3737(widgets)X
1707 4480(to)N
1798(it.)X
555 4604(Client)N
1707(A)X
1800(routine)X
2080(that)X
2242(uses)X
2422(a)X
2490(widget)X
2759(in)X
2857(an)X
2969(application)X
3391(or)X
3493(for)X
3624(composing)X
1707 4700(another)N
1993(widget.)X
555 4824(Name)N
1707(The)X
1871(name)X
2089(that)X
2249(is)X
2335(speci\256c)X
2631(to)X
2727(a)X
2793(given)X
3016(widget)X
3283(instance)X
3599(in)X
3695(an)X
3805(appli-)X
1707 4920(cation.)N
555 5044(Class)N
1707(A)X
1792(general)X
2073(group)X
2300(that)X
2455(a)X
2516(widget)X
2778(belongs)X
3074(to.)X
3 f
555 5236(4.)N
665(Widgets)X
1 f
555 5360(The)N
726(fundamental)X
1201(data)X
1381(type)X
1566(of)X
1672(the)X
1813(toolkit)X
2078(is)X
2170(the)X
2311(``widget,'')X
2722(which)X
2970(is)X
3062(dynamically)X
3532(allocated)X
3884(and)X
555 5456(contains)N
874(state)X
1061(information.)X
1547(Every)X
1782(widget)X
2047(belongs)X
2346(to)X
2440(exactly)X
2720(one)X
2872(``widget)X
3195(class'')X
3449(that)X
3607(is)X
3691(statically)X
555 5552(allocated)N
896(and)X
1045 0.3187(initialized)AX
1422(and)X
1571(that)X
1726(contains)X
2042(the)X
2172(operations)X
2561(allowable)X
2926(on)X
3036(widgets)X
3332(of)X
3427(that)X
3582(class.)X
555 5676(Physically,)N
974(a)X
1042(widget)X
1311(is)X
1399(a)X
1467(pointer)X
1746(to)X
1844(a)X
1911(structure)X
2247(which)X
2490(contains)X
2812(\256elds)X
3031(common)X
3368(to)X
3465(all)X
3582(widgets)X
3884(and)X
555 5772(some)N
764(\256elds)X
978(that)X
1134(are)X
1264(speci\256c)X
1556(to)X
1648(widgets)X
1945(of)X
2040(a)X
2101(particular)X
2461(widget)X
2723(class.)X
2960(In)X
3055(general,)X
3358(the)X
3488(values)X
3735(of)X
3830(these)X
3 f
2261 6144(2)N

3 p
%%Page: 3 5
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(\256elds)N
776(may)X
957(vary)X
1142(from)X
1342(widget)X
1611(to)X
1709(widget,)X
2000(even)X
2195(for)X
2326(widgets)X
2629(of)X
2731(the)X
2868(same)X
3078(widget)X
3347(class.)X
3591(A)X
3683(widget)X
3952(is)X
555 768(allocated)N
896(and)X
1045 0.3187(initialized)AX
1422(by)X
5 f
1550(XtCreateWidget)X
1 f
2187(.)X
2253(See)X
2402(``Creating)X
2786(Widgets'')X
3160(for)X
3284(further)X
3545(information.)X
555 892(Physically,)N
973(a)X
1040(widget)X
1308(class)X
1507(is)X
1594(a)X
1661(pointer)X
1939(to)X
2036(a)X
2103(structure)X
2439(which)X
2682(contains)X
3004(\256elds)X
3223(common)X
3559(to)X
3655(all)X
3771(widget)X
555 988(classes)N
825(and)X
978(some)X
1190(\256elds)X
1406(that)X
1564(are)X
1696(particular)X
2059(to)X
2153(the)X
2286(widget)X
2551(class.)X
2791(The)X
2953(values)X
3203(of)X
3301(these)X
3507(\256elds)X
3723(are)X
3855(con-)X
555 1084(stant)N
748(for)X
876(all)X
991(widgets)X
1291(of)X
1390(the)X
1524(widget)X
1790(class,)X
2009(though)X
2279(the)X
2412(values)X
2662(may)X
2839(vary)X
3020(from)X
3216(widget)X
3481(class)X
3677(to)X
3771(widget)X
555 1180(class.)N
802(\(``Constant'')X
1293(means)X
1550(the)X
1690(class)X
1893(structure)X
2233(is)X
2324 0.3187(initialized)AX
2711(at)X
2807(compile-time)X
3311(and)X
3470(never)X
3696(changed,)X
555 1276(except)N
807(for)X
931(a)X
992(one-shot)X
1317(class)X
1510 0.3317(initialization)AX
1981(and)X
2130(in-place)X
2436(compilation)X
2881(of)X
2976(resource)X
3295(lists.\))X
555 1400(The)N
718(organization)X
1185(of)X
1284(the)X
1418(declarations)X
1870(and)X
2023(code)X
2215(for)X
2343(a)X
2408(new)X
2580(widget)X
2845(class)X
3041(between)X
3359(a)X
3423(public)X
3669(``.h'')X
3876(\256le,)X
555 1496(a)N
618(private)X
887(``.h'')X
1093(\256le,)X
1252(and)X
1403(the)X
1535 0.2885(implementation)AX
2115(``.c'')X
2316(\256le)X
2453(is)X
2535(described)X
2895(in)X
2987(``Widget)X
3328(Subclassing''.)X
3874(The)X
555 1592(prede\256ned)N
948(widget)X
1210(classes)X
1476(below)X
1713(adhere)X
1969(to)X
2060(these)X
2263(conventions.)X
3 f
555 1784(4.1.)N
731(Core)X
938(Widget)X
1234(De\256nitions)X
1 f
555 1908(The)N
727(Core)X
933(widget)X
1208(contains)X
1537(the)X
1679(generic)X
1972(widget)X
2246(de\256nitions.)X
2697(All)X
2844(actual)X
3089(widgets)X
3397(are)X
3538(subclasses)X
3938(of)X
555 2004(Core.)N
794(The)X
955(structure)X
1287(de\256nitions)X
1684(for)X
1810(Core,)X
2027(Composite)X
2433(and)X
2583(Constraint)X
2974(currently)X
3315(are)X
3445(contained)X
3811(in)X
3903(the)X
555 2100(Intrinsic.h)N
937(\256le.)X
3 f
555 2292(4.1.1.)N
797(CorePart)X
1170(De\256nition)X
1 f
555 2416(The)N
714(common)X
1045(\256elds)X
1258(for)X
1382(all)X
1493(widgets)X
1789(are)X
1918(de\256ned)X
2199(in)X
2290(the)X
5 f
2438(CorePart)X
1 f
2827(structure:)X
555 2540(typedef)N
841(struct)X
1059({)X
843 2636(WidgetClass)N
2 f
1316(widget_class)X
1 f
1772(;)X
2283(See)X
2432(``Widget)X
2772(Classes'')X
843 2732(Widget)N
2 f
1125(parent)X
1 f
1355(;)X
2283(See)X
2432(``Widget)X
2772(Tree'')X
843 2828(String)N
2 f
1081(name)X
1 f
1271(;)X
2283(See)X
2432(``Resource)X
2839(Management'')X
843 2924(XrmName)N
2 f
1236(xrm_name)X
1 f
1606(;)X
2283(Private)X
2555(to)X
2646(``Resource)X
3053(Management'')X
843 3020(Screen)N
1104(*)X
2 f
(screen)S
1 f
1377(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 3116(Window)N
2 f
1168(window)X
1 f
1443(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 3212(Position)N
2 f
1155(x)X
1 f
1194(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 3308(Position)N
2 f
1155(y)X
1 f
1194(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 3404(Dimension)N
2 f
1252(width)X
1 f
1449(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 3500(Dimension)N
2 f
1252(height)X
1 f
1473(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 3596(Cardinal)N
2 f
1169(depth)X
1 f
1365(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 3692(Dimension)N
2 f
1252(border_width)X
1 f
1732(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 3788(Pixel)N
2 f
1047(border_pixel)X
1 f
1502(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 3884(Pixmap)N
2 f
1135(border_pixmap)X
1 f
1677(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 3980(Pixel)N
2 f
1047(background_pixel)X
1 f
1683(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 4076(Pixmap)N
2 f
1135(background_pixmap)X
1 f
1858(;)X
2283(See)X
2432(``Window)X
2815(Attributes'')X
843 4172(_XtEventTable)N
2 f
1404(event_table)X
1 f
1811(;)X
2283(Private)X
2555(to)X
2646(``Event)X
2932(Management'')X
843 4268(_XtTranslations)N
2 f
1433(translations)X
1 f
1855(;)X
2283(Private)X
2555(to)X
2646(``Translation)X
3128(Management'')X
843 4364(Boolean)N
2 f
1159(visible)X
1 f
1390(;)X
2283(See)X
2432(``Widget)X
2772(Visibility)X
3129(and)X
3278(Exposure'')X
843 4460(Boolean)N
2 f
1159(sensitive)X
1 f
1463(;)X
2283(See)X
2432(``XtSetSensitive'')X
843 4556(Boolean)N
2 f
1159(ancestor_sensitive)X
1 f
1810(;)X
2283(See)X
2432(``XtSetSensitive'')X
843 4652(Boolean)N
2 f
1159(managed)X
1 f
1481(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 4748(Boolean)N
2 f
1159(mapped_when_managed)X
1 f
2033(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 4844(Boolean)N
2 f
1159(being_destroyed)X
1 f
1741(;)X
2283(See)X
2432(``Destroying)X
2903(Widgets'')X
843 4940(XtCallbackList)N
2 f
1405(destroy_callbacks)X
1 f
2041(;)X
2283(See)X
2432(``Destroying)X
2903(Widgets'')X
555 5036(})N
619(CorePart;)X
555 5208(All)N
709(widgets)X
1024(have)X
1231(the)X
1379(core)X
1570(\256elds)X
1801(as)X
1914(their)X
2116(\256rst)X
2293(component.)X
2769(The)X
2946(prototypical)X
3413(type)X
5 f
3623(Widget)X
1 f
3952(is)X
555 5304(de\256ned)N
847(with)X
1037(only)X
1227(this)X
1388(set)X
1519(of)X
1625(\256elds:)X
1873(various)X
2164(routines)X
2480(may)X
2664(cast)X
2833(these)X
3046(to)X
3147(speci\256c)X
3448(widget)X
3720(types)X
3938(as)X
555 5400(needed.)N
555 5524(typedef)N
841(struct)X
1059({)X
843 5620(CorePart)N
2 f
1178(core)X
1 f
1334(;)X
555 5716(})N
619(WidgetRec,)X
1060(*Widget;)X
3 f
2261 6144(3)N

4 p
%%Page: 4 6
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(4.1.2.)N
797(CorePart)X
1170(Default)X
1470(Values)X
1 f
555 796(The)N
723(default)X
999(values)X
1255(for)X
1388(the)X
1527(core)X
1709(\256elds)X
1931(\(\256lled)X
2173(in)X
2273(by)X
2392(the)X
2531(Core)X
2733(resource)X
3061(list)X
3201(and)X
3359(the)X
3498(Core)X
3700 0.3542(initialize)AX
555 892(procedure\))N
957(are:)X
10 f
555 976(i)N
563(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1120(Field)N
1435(Default)X
1721(Value)X
10 f
555 1176(i)N
563(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1320(widget_class)N
1435(widget_class)X
1912(argument)X
2267(to)X
2358(XtCreateWidget)X
555 1416(parent)N
1435(parent)X
1677(argument)X
2032(to)X
2123(XtCreateWidget)X
555 1512(name)N
1435(name)X
1648(argument)X
2003(to)X
2094(XtCreateWidget)X
555 1608(screen)N
1435(parent's)X
1740(screen)X
1986(\256eld)X
2165(\(top-level)X
2530(widget)X
2792(from)X
2985(display)X
3262(speci\256er\))X
555 1704(window)N
1435(NULL)X
555 1800(x)N
1435(0)X
555 1896(y)N
1435(0)X
555 1992(width)N
1435(0)X
555 2088(height)N
1435(0)X
555 2184(depth)N
1435(parent's)X
1740(depth)X
1958(\256eld)X
2137(\(top-level)X
2502(widget)X
2764(from)X
2957(root)X
3121(window\))X
555 2280(border_width)N
1435(1)X
555 2376(border_pixel)N
1435(black)X
555 2472(border_pixmap)N
1435(NULL)X
555 2568(background_pixel)N
1435(white)X
555 2664(background_pixmap)N
1435(NULL)X
555 2760(visible)N
1435(TRUE)X
555 2856(sensitive)N
1435(TRUE)X
555 2952(ancestor_sensitive)N
1435(parent's)X
1740(\(sensitive)X
2100(&)X
2191(ancestor_sensitive\))X
555 3048(managed)N
1435(FALSE)X
555 3144(map_when_managed)N
1435(TRUE)X
555 3240(being_destroyed)N
1435(parent's)X
1740(being_destroyed)X
555 3336(destroy_callbacks)N
1435(NULL)X
10 f
555 3392(i)N
563(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
3 f
555 3632(4.1.3.)N
797(CoreClassPart)X
1370(De\256nitions)X
1 f
555 3756(The)N
714(common)X
1045(\256elds)X
1258(for)X
1382(all)X
1493(widget)X
1755(classes)X
2021(are)X
2150(de\256ned)X
2431(in)X
2522(the)X
5 f
2670(CoreClassPart)X
1 f
3278(structure:)X
3 f
2261 6144(4)N

5 p
%%Page: 5 7
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(typedef)N
841(void)X
1020(\(*)X
2 f
1093(WidgetProc)X
1 f
1514(\)\(\);)X
731 768(Widget)N
2 f
1013(w)X
1 f
1072(;)X
555 960(typedef)N
841(struct)X
1059({)X
843 1056(WidgetClass)N
2 f
1316(superclass)X
1 f
1687(;)X
2283(See)X
2432(``Widget)X
2772(Subclassing'')X
843 1152(String)N
2 f
1081(class_name)X
1 f
1491(;)X
2283(See)X
2432(``Widget)X
2772(Subclassing'')X
843 1248(Cardinal)N
2 f
1169(widget_size)X
1 f
1581(;)X
2283(See)X
2432(``Creating)X
2816(Widgets'')X
843 1344(XtProc)N
2 f
1114 0.2042(class_initialize)AX
1 f
1645(;)X
2283(See)X
2432(``Widget)X
2772(Subclassing'')X
843 1440(Boolean)N
2 f
1159(class_inited)X
1 f
1581(;)X
2283(Private)X
2555(to)X
2646(``XtCreateWidget'')X
843 1536(XtInitProc)N
2 f
1237 0.3403(initialize)AX
1 f
1548(;)X
2283(See)X
2432(``Creating)X
2816(Widgets'')X
843 1632(XtRealizeProc)N
2 f
1379(realize)X
1 f
1619(;)X
2283(See)X
2432(``Creating)X
2816(Widgets'')X
843 1728(XtActionList)N
2 f
1331(actions)X
1 f
1586(;)X
2283(See)X
2432(``Translation)X
2914(Management'')X
843 1824(Cardinal)N
2 f
1169(num_actions)X
1 f
1619(;)X
2283(See)X
2432(``Translation)X
2914(Management'')X
843 1920(XtResourceList)N
2 f
1418(resources)X
1 f
1759(;)X
2283(See)X
2432(``Resource)X
2839(Management'')X
843 2016(Cardinal)N
2 f
1169(num_resources)X
1 f
1705(;)X
2283(See)X
2432(``Resource)X
2839(Management'')X
843 2112(XrmClass)N
2 f
1217(xrm_class)X
1 f
1573(;)X
2283(Private)X
2555(to)X
2646(``Resource)X
3053(Management'')X
843 2208(Boolean)N
2 f
1159(compress_motion)X
1 f
1779(;)X
2283(See)X
2432(``Mouse)X
2751(Motion)X
3033(Compression'')X
843 2304(Boolean)N
2 f
1159(compress_exposure)X
1 f
1851(;)X
2283(See)X
2432(``Exposure)X
2844(Compression'')X
843 2400(Boolean)N
2 f
1159(visible_interest)X
1 f
1699(;)X
2283(See)X
2432(``Widget)X
2772(Visibility)X
3129(and)X
3278(Exposure'')X
843 2496(XtWidgetProc)N
2 f
1374(destroy)X
1 f
1633(;)X
2283(See)X
2432(``Destroying)X
2903(Widgets'')X
843 2592(XtWidgetProc)N
2 f
1374(resize)X
1 f
1579(;)X
2283(See)X
2432(``Geometry)X
2864(Management'')X
843 2688(XtExposeProc)N
2 f
1373(expose)X
1 f
1612(;)X
2283(See)X
2432(``Widget)X
2772(Visibility)X
3129(and)X
3278(Exposure'')X
843 2784(XtSetValuesProc)N
2 f
1471(set_values)X
1 f
1838(;)X
2283(See)X
2432(``Reading)X
2806(and)X
2955(Writing)X
3252(Widget)X
3534(State'')X
843 2880(XtWidgetProc)N
2 f
1374(accept_focus)X
1 f
1834(;)X
2283(See)X
2432(``Focus)X
2722(Management'')X
555 2976(})N
619(CoreClassPart;)X
555 3148(All)N
708(widget)X
988(classes)X
1271(have)X
1476(the)X
1623(core)X
1813(class)X
2023(\256elds)X
2253(as)X
2365(their)X
2566(\256rst)X
2742(component.)X
3217(The)X
3393(prototypical)X
3859(type)X
5 f
570 3244(WidgetClass)N
1 f
1106(is)X
1193(de\256ned)X
1480(with)X
1665(only)X
1850(this)X
2006(set)X
2132(of)X
2233(\256elds.)X
2496(That)X
2686(is,)X
2794(various)X
3080(routines)X
3391(may)X
3570(cast)X
3734(these)X
3942(to)X
555 3340(speci\256c)N
846(widget)X
1108(class)X
1301(types)X
1509(as)X
1604(needed:)X
555 3464(typedef)N
841(struct)X
1059({)X
843 3560(CoreClassPart)N
2 f
1369(core_class)X
1 f
1745(;)X
555 3656(})N
619(WidgetClassRec,)X
1251(*WidgetClass;)X
555 3828(The)N
714(prede\256ned)X
1107(class)X
1300(record)X
1546(and)X
1695(pointer)X
1967(for)X
5 f
2109(WidgetClassRec)X
1 f
2795(are:)X
555 3952(extern)N
797(WidgetClassRec)X
2 f
1407(widgetClassRec)X
1 f
1971(;)X
555 4096(extern)N
797(WidgetClass)X
2 f
1270(widgetClass)X
1 f
1702(;)X
555 4268(Applications)N
1033(that)X
1193(do)X
1308(not)X
1448(de\256ne)X
1690(new)X
1863(widget)X
2130(classes)X
2401(usually)X
2683(do)X
2798(not)X
2937(need)X
3129(to)X
3224(access)X
3474(the)X
3608(internals)X
3938(of)X
555 4364(widgets.)N
883(The)X
1052(opaque)X
1338(types)X
5 f
1573(Widget)X
1 f
1893(and)X
5 f
2069(WidgetClass)X
1 f
2608(and)X
2766(the)X
2905(opaque)X
3190(variable)X
5 f
3523(widgetClass)X
1 f
555 4460(are)N
687(de\256ned)X
971(for)X
1098(generic)X
1382(actions)X
1657(on)X
1770(widgets.)X
2112(Applications)X
2587(can)X
2733(use)X
2874(these)X
3079(types)X
3289(for)X
3415(generality)X
3792(or)X
3889(can)X
555 4556(use)N
694(the)X
824(widget-speci\256c)X
1384(types)X
1592(to)X
1683(make)X
1896(the)X
2026(code)X
2214(indicate)X
2516(external)X
2822(representation.)X
3 f
555 4748(4.2.)N
731(Composite)X
1153(Widget)X
1449(De\256nitions)X
1 f
555 4872(Composite)N
961(widgets)X
1258(are)X
1388(a)X
1450(subclass)X
1766(of)X
1862(the)X
1993(Core)X
2187(widget)X
2450(and)X
2599(are)X
2728(more)X
2931(fully)X
3120(described)X
3479(in)X
3570(``Composite)X
555 4968(Widgets''.)N
3 f
555 5160(4.2.1.)N
797(CompositePart)X
1385(De\256nition)X
1 f
555 5284(Composite)N
960(widgets)X
1256(have)X
1444(the)X
1574(following)X
1939(additional)X
2315(\256elds)X
2528(de\256ned)X
2809(in)X
2900(the)X
5 f
3048(CompositePart)X
1 f
3667(structure:)X
3 f
2261 6144(5)N

6 p
%%Page: 6 8
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(typedef)N
841(struct)X
1059({)X
843 768(WidgetList)N
2 f
1263(children)X
1 f
1557(;)X
2283(See)X
2432(``Widget)X
2772(Tree'')X
843 864(Cardinal)N
2 f
1169(num_children)X
1 f
1658(;)X
2283(See)X
2432(``Widget)X
2772(Tree'')X
843 960(Cardinal)N
2 f
1169(num_slots)X
1 f
1526(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 1056(Cardinal)N
2 f
1169(num_mapped_children)X
1 f
1980(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 1152(XtOrderProc)N
2 f
1318(insert_position)X
1 f
1848(;)X
2283(See)X
2432(``Creating)X
2816(Widgets'')X
555 1248(})N
619(CompositePart;)X
555 1420(Composite)N
960(widgets)X
1256(have)X
1444(the)X
1574(composite)X
1959(\256elds)X
2172 0.3187(immediately)AX
2637(following)X
3002(the)X
3132(core)X
3305(\256elds:)X
555 1544(typedef)N
841(struct)X
1059({)X
843 1640(CorePart)N
2 f
1178(core)X
1 f
1334(;)X
843 1736(CompositePart)N
2 f
1390(composite)X
1 f
1747(;)X
555 1832(})N
619(CompositeRec,)X
1183 0.2031(*CompositeWidget;)AX
3 f
555 2072(4.2.2.)N
797(CompositePart)X
1385(Default)X
1685(Values)X
1 f
555 2196(The)N
716(default)X
985(values)X
1234(for)X
1360(the)X
1492(composite)X
1879(\256elds)X
2094(\(\256lled)X
2329(in)X
2422(by)X
2534(the)X
2666(Composite)X
3073(resource)X
3394(list)X
3527(and)X
3678(the)X
3810(Com-)X
555 2292(posite)N
788 0.3542(initialize)AX
1121(procedure)X
1494(are:)X
10 f
555 2376(i)N
580(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 2520(Field)N
1500(Default)X
1786(Value)X
10 f
555 2576(i)N
580(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 2720(children)N
1500(NULL)X
555 2816(num_children)N
1500(0)X
555 2912(num_slots)N
1500(0)X
555 3008(num_mapped_children)N
1500(0)X
555 3104(insert_position)N
1500(internal)X
1792(function)X
2108(InsertAtEnd)X
10 f
555 3160(i)N
580(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
3 f
555 3400(4.2.3.)N
797(CompositeClassPart)X
1585(De\256nitions)X
1 f
555 3524(Composite)N
960(widget)X
1222(classes)X
1488(have)X
1676(the)X
1806(following)X
2171(additional)X
2547(\256elds:)X
555 3648(typedef)N
841(struct)X
1059({)X
843 3744(XtGeometryHandler)N
2 f
1588(geometry_manager)X
1 f
2271(;See)X
2445(``Geometry)X
2877(Management'')X
843 3840(XtWidgetProc)N
2 f
1374(change_managed)X
1 f
1994(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 3936(XtArgsProc)N
2 f
1284(insert_child)X
1 f
1706(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 4032(XtWidgetProc)N
2 f
1374(delete_child)X
1 f
1806(;)X
2283(See)X
2432(``Composite)X
2895(Widgets'')X
843 4128(XtWidgetProc)N
2 f
1374(move_focus_to_next)X
1 f
2093(;)X
2283(See)X
2432(``Focus)X
2722(Management'')X
843 4224(XtWidgetProc)N
2 f
1374(move_focus_to_prev)X
1 f
2102(;)X
2283(See)X
2432(``Focus)X
2722(Management'')X
555 4320(})N
619(CompositeClassPart;)X
555 4492(Composite)N
960(widget)X
1222(classes)X
1488(have)X
1676(the)X
1806(composite)X
2191(\256elds)X
2404 0.3187(immediately)AX
2869(following)X
3234(the)X
3364(core)X
3537(\256elds:)X
555 4616(typedef)N
841(struct)X
1059({)X
843 4712(CoreClassPart)N
2 f
1369(core_class)X
1 f
1745(;)X
843 4808(CompositeClassPart)N
2 f
1581(composite_class)X
1 f
2158(;)X
555 4904(})N
619(CompositeClassRec,)X
1374 0.1696(*CompositeWidgetClass;)AX
555 5076(The)N
714(prede\256ned)X
1107(class)X
1300(record)X
1546(and)X
1695(pointer)X
1967(for)X
5 f
2109(CompositeClassRec)X
1 f
2941(are:)X
555 5200(extern)N
797(compositeClassRec)X
2 f
1510(compositeClassRec)X
1 f
2195(;)X
555 5344(extern)N
797(CompositeWidgetClass)X
2 f
1653(compositeWidgetClass)X
1 f
2456(;)X
555 5516(The)N
726(opaque)X
1014(types)X
5 f
1252(CompositeWidget)X
1 f
1995(and)X
5 f
2174(CompositeWidgetClass)X
1 f
3136(and)X
3297(the)X
3439(opaque)X
3727(variable)X
5 f
570 5612(compositeWidgetClass)N
1 f
1515(are)X
1658(de\256ned)X
1953(for)X
2091(generic)X
2386(operations)X
2789(on)X
2913(widgets)X
3223(that)X
3392(are)X
3535(a)X
3610(subclass)X
3938(of)X
5 f
570 5708(CompositeWidget)N
1 f
1279(.)X
3 f
2261 6144(6)N

7 p
%%Page: 7 9
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(To)N
681(determine)X
1062(if)X
1144(a)X
1211(widget)X
2 f
1479(w)X
1 f
1565(is)X
1651(composite)X
2041(\(or)X
2170(a)X
2236(subclass\),)X
2607(use)X
2751(the)X
2886(Boolean)X
3207(function)X
5 f
3546(XtIsCompo-)X
555 768(site)N
1 f
707(.)X
555 988(Boolean)N
871(XtIsComposite)X
1412(\()X
2 f
1448(w)X
1 f
1514(\))X
687 1084(Widget)N
2 f
969(w)X
1 f
1035(;)X
3 f
555 1400(4.3.)N
731(Constraint)X
1158(Widget)X
1454(De\256nitions)X
1 f
555 1524(Constraint)N
964(widgets)X
1278(are)X
1425(a)X
1504(subclass)X
1837(of)X
1950(the)X
2098(Composite)X
2521(widget,)X
2823(and)X
2990(are)X
3137(more)X
3358(fully)X
3565(described)X
3942(in)X
555 1620(``Constrained)N
1061(Composite)X
1466(Widgets''.)X
3 f
555 1812(4.3.1.)N
797(ConstraintPart)X
1390(De\256nition)X
1 f
555 1936(Constraint)N
945(widgets)X
1241(have)X
1429(the)X
1559(following)X
1924(additional)X
2300(\256elds)X
2513(de\256ned)X
2794(in)X
2885(the)X
5 f
3033(ConstraintPart)X
1 f
3633(structure:)X
555 2060(typedef)N
841(struct)X
1059({)X
843 2156(caddr_t)N
2 f
1129(mumble)X
1 f
1407(;)X
2283(No)X
2412(new)X
2580(\256elds,)X
2815(keep)X
3003(C)X
3084(compiler)X
3420(happy)X
555 2252(})N
619(ConstraintPart;)X
555 2424(Constraint)N
945(widgets)X
1241(have)X
1429(the)X
1559(constraint)X
1929(\256elds)X
2142 0.3187(immediately)AX
2607(following)X
2972(the)X
3102(composite)X
3487(\256elds:)X
555 2548(typedef)N
841(struct)X
1059({)X
843 2644(CorePart)N
2 f
1178(core)X
1 f
1334(;)X
843 2740(CompositePart)N
2 f
1390(composite)X
1 f
1747(;)X
843 2836(ConstraintPart)N
2 f
1375(constraint)X
1 f
1733(;)X
555 2932(})N
619(ConstraintRec,)X
1168(*ConstraintWidget;)X
3 f
555 3172(4.3.2.)N
797(ConstraintClassPart)X
1590(De\256nitions)X
1 f
555 3296(Constraint)N
945(widget)X
1207(classes)X
1473(have)X
1661(the)X
1791(following)X
2156(additional)X
2532(\256elds:)X
555 3420(typedef)N
841(struct)X
1059({)X
843 3516(XtResourceList)N
2 f
1418(constraints)X
1 f
1810(;)X
2283(See)X
2432(``Constrained)X
2938(Composite)X
3343(Widgets'')X
843 3612(Cardinal)N
2 f
1169(num_constraints)X
1 f
1756(;)X
2283(See)X
2432(``Constrained)X
2938(Composite)X
3343(Widgets'')X
555 3708(})N
619(ConstraintClassPart;)X
555 3880(Constraint)N
945(widget)X
1207(classes)X
1473(have)X
1661(the)X
1791(constraint)X
2161(\256elds)X
2374 0.3187(immediately)AX
2839(following)X
3204(the)X
3334(composite)X
3719(\256elds:)X
555 4004(typedef)N
841(struct)X
1059({)X
843 4100(CoreClassPart)N
2 f
1369(core_class)X
1 f
1745(;)X
843 4196(CompositeClassPart)N
2 f
1581(composite_class)X
1 f
2158(;)X
843 4292(ConstraintClassPart)N
2 f
1566(constraint_class)X
1 f
2144(;)X
555 4388(})N
619(ConstraintClassRec,)X
1359 0.1477(*ConstraintWidgetClass;)AX
555 4560(The)N
714(prede\256ned)X
1107(class)X
1300(record)X
1546(and)X
1695(pointer)X
1967(for)X
5 f
2109(ConstraintClassRec)X
1 f
2922(are:)X
555 4684(extern)N
797(ConstraintClassRec)X
2 f
1515(constraintClassRec)X
1 f
2201(;)X
555 4828(extern)N
797(ConstraintWidgetClass)X
2 f
1638(constraintWidgetClass)X
1 f
2442(;)X
555 5000(The)N
733(opaque)X
1028(types)X
5 f
1272(ConstraintWidget)X
1 f
5 f
2020(ConstraintWidget)X
1 f
2750(and)X
5 f
2935(ConstraintWidgetClass)X
1 f
3884(and)X
555 5096(the)N
692(opaque)X
975(variable)X
5 f
1306(constraintWidgetClass)X
1 f
2225(are)X
2361(de\256ned)X
2648(for)X
2778(generic)X
3065(operations)X
3460(on)X
3576(widgets)X
3878(that)X
555 5192(are)N
684(a)X
745(subclass)X
1060(of)X
5 f
1173(ConstraintWidgetClass)X
1 f
2082(.)X
3 f
2261 6144(7)N

8 p
%%Page: 8 10
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(5.)N
665(Widget)X
961(Classes)X
1 f
555 796(The)N
717(widget_class)X
1197(\256eld)X
1379(of)X
1477(a)X
1541(widget)X
1806(points)X
2047(to)X
2140(its)X
2248(widget)X
2512(class)X
2707(structure.)X
3083(The)X
3244(widget)X
3508(class)X
3703(structure)X
555 892(contains)N
871(information)X
1310(that)X
1465(is)X
1546(constant)X
1862(across)X
2103(all)X
2214(widgets)X
2510(of)X
2605(that)X
2760(class.)X
555 1016(This)N
736(class-oriented)X
1249(structure)X
1581(means)X
1830(that)X
1986(widget)X
2249(classes)X
2516(do)X
2627(not)X
2763(usually)X
3041(implement)X
3443(directly)X
3736(callable)X
555 1112(procedures.)N
1003(That)X
1206(is,)X
1328(they)X
1521(implement)X
1940(procedures)X
2365(that)X
2538(are)X
2685(available)X
3044(through)X
3358(their)X
3560(widget)X
3840(class)X
555 1208(structure.)N
939(These)X
1181(class)X
1384(procedures)X
1801(are)X
1940(invoked)X
2256(by)X
2376(generic)X
2667(procedures,)X
3106(which)X
3353(envelope)X
3702(common)X
555 1304(actions)N
833(around)X
1104(the)X
1239(procedures)X
1651(implemented)X
2140(by)X
2255(the)X
2390(widget)X
2657(class.)X
2899(Such)X
3102(procedures)X
3514(are)X
3648(applicable)X
555 1400(to)N
646(all)X
757(widgets)X
1053(of)X
1148(that)X
1303(class)X
1496(and)X
1645(also)X
1809(to)X
1900(widgets)X
2196(that)X
2351(are)X
2480(subclasses)X
2868(of)X
2963(that)X
3118(class.)X
2197 1576(Note)N
775 1712(In)N
883(the)X
1026(alpha)X
1252(release)X
1531(toolkit,)X
1820(each)X
2016(widget)X
2291(class)X
2496(implemented)X
2992(an)X
3109(Xt<)X
2 f
3254(Widget)X
1 f
3511(>Create)X
775 1808(\(for)N
941(example,)X
5 f
1315(XtLabelCreate)X
1 f
1893(\))X
1957(function,)X
2308(in)X
2412(which)X
2661(most)X
2867(of)X
2974(the)X
3116(code)X
3316(was)X
3486(identical)X
775 1904(from)N
975(widget)X
1244(to)X
1342(widget.)X
1655(In)X
1757(this)X
1914(toolkit,)X
2197(a)X
2265(single)X
2504(generic)X
5 f
2809(XtCreateWidget)X
1 f
3474(performs)X
775 2000(most)N
970(of)X
1066(the)X
1197(common)X
1529(work)X
1732(and)X
1881(then)X
2055(calls)X
2239(the)X
2369 0.3542(initialize)AX
2702(procedure)X
3075(implemented)X
3559(for)X
3683(the)X
775 2096(particular)N
1135(widget)X
1397(class.)X
775 2220(Each)N
989(composite)X
1390(widget)X
1668(class)X
1877(also)X
2057(implemented)X
2557(the)X
2702(procedures)X
3124(Xt<)X
2 f
3269(Widget)X
1 f
3526(>Add)X
3764(and)X
3928(an)X
775 2316(Xt<)N
2 f
920(Widget)X
1 f
1177(>Delete)X
1480(\(for)X
1633(example,)X
5 f
1994 0.2599(XtButtonBoxAddButton)AX
1 f
2936(and)X
5 f
3103 0.1563(XtButtonBoxDeleteBut-)AX
775 2412(ton)N
1 f
913(\).)X
1019(In)X
1125(the)X
1266(beta)X
1445(release)X
1721(toolkit,)X
2007(the)X
2147(composite)X
2542(generic)X
2833(procedures)X
5 f
3268(XtManageChildren)X
1 f
775 2508(and)N
5 f
947(XtUnmanageChildren)X
1 f
1828(perform)X
2137(error-checking)X
2680(and)X
2833(screening)X
3196(out)X
3335(of)X
3434(certain)X
3700(children.)X
775 2604(Then,)N
1009(they)X
1191(call)X
1349(the)X
1487(changed_managed)X
2172(procedure)X
2553(implemented)X
3045(for)X
3177(the)X
3315(widget's)X
3648(composite)X
775 2700(class.)N
775 2824(Note)N
970(that)X
1127(the)X
1259(new)X
1429(toolkit)X
1684(can)X
1829(be)X
1935(used)X
2119(to)X
2211(implement)X
2613(old-style)X
2945(calls)X
3130(by)X
3241(de\256ning)X
3553(one-line)X
3865(pro-)X
775 2920(cedures)N
1075(or)X
1180(macros)X
1465(that)X
1629(invoke)X
1900(a)X
1970(generic)X
2260(routine.)X
2585(For)X
2738(example,)X
3090(you)X
3253(can)X
3406(de\256ne)X
3652(the)X
3791(macro)X
5 f
790 3016(XtCreateLabel)N
1 f
1368(:)X
775 3140(#de\256ne)N
1056(XtCreateLabel\()X
2 f
1604(name)X
1 f
1794(,)X
2 f
1838(parent)X
1 f
2068(,)X
2 f
2112(args)X
1 f
2268(,)X
2 f
2312(num_args)X
1 f
2663(\))X
2714(\\)X
1063 3236(\(\(LabelWidget\))N
1633(XtCreateWidget\()X
2 f
2240(name)X
1 f
2430(,)X
2 f
2474(labelWidgetClass)X
1 f
3097(,)X
3207(\\)X
2 f
1063 3332(parent)N
1 f
1293(,)X
2 f
1337(args)X
1 f
1493(,)X
2 f
1537(num_args)X
1 f
1888(\)\))X
3 f
555 3652(6.)N
665(Widget)X
961(Subclassing)X
1 f
555 3776(All)N
696(widget)X
963(classes)X
1234(are)X
1368(a)X
1434(subclass)X
1754(of)X
1854(the)X
1989(Core)X
2187(class)X
2385(and)X
2539(may)X
2718(be)X
2828(subclassed)X
3231(further.)X
3541(For)X
3690(example,)X
555 3872(Label)N
785(is)X
873(a)X
941(direct)X
1171(subclass)X
1493(of)X
1594(Core.)X
1815(That)X
2005(is,)X
2114(it)X
2192(adds)X
2381(a)X
2448(text)X
2609(\256eld.)X
2838(Command)X
3234(is)X
3321(a)X
3388(direct)X
3617(subclass)X
3938(of)X
555 3968(Label.)N
811(In)X
917(addition)X
1240(to)X
1342(a)X
1414(text)X
1580(\256eld,)X
1792(it)X
1875(has)X
2025(a)X
2096(callback)X
2422(procedure)X
2805(that)X
2970(can)X
3124(be)X
3239(invoked.)X
3599(Toggle)X
3881(is)X
3972(a)X
555 4064(direct)N
784(subclass)X
1105(of)X
1206(Command.)X
1624(In)X
1725(addition)X
2043(to)X
2140(the)X
2275(text)X
2435(and)X
2589(callback,)X
2932(it)X
3009(has)X
3153(a)X
3219(Boolean)X
3540(state)X
3729(that)X
3889(can)X
555 4160(be)N
660(toggled.)X
555 4284(Subclassing)N
1004(reduces)X
1300(the)X
1436(amount)X
1729(of)X
1830(code)X
2024(and)X
2178(declarations)X
2631(you)X
2790(write)X
2998(to)X
3094(make)X
3312(a)X
3378(new)X
3551(widget)X
3818(class.)X
555 4380(For)N
700(example,)X
1044(you)X
1199(do)X
1310(not)X
1446(have)X
1635(to)X
1727(include)X
2009(every)X
2226(resource)X
2545(your)X
2728(widget)X
2990(uses)X
3163(in)X
3254(an)X
5 f
3377(XtResourceList)X
1 f
3989(,)X
555 4476(but)N
690(you)X
844(do)X
954(have)X
1142(to)X
1233(include)X
1515(the)X
1645(resources)X
1998(your)X
2181(widget)X
2443(has)X
2582(that)X
2737(its)X
2843(superclass)X
3226(does)X
3409(not.)X
555 4600(In)N
653(order)X
863(to)X
957(make)X
1173(good)X
1374(use)X
1516(of)X
1614(subclassing,)X
2066(widget)X
2330(declarations)X
2780(are)X
2911(highly)X
3161(stylized.)X
3504(A)X
3591(widget)X
3855(con-)X
555 4696(sists)N
729(of)X
824(three)X
1022(parts:)X
10 f
555 4820(g)N
1 f
775(A)X
860(public)X
1103(``.h'')X
1307(\256le)X
1442(that)X
1597(is)X
1678(used)X
1861(by)X
1971(client)X
2190(widgets)X
2486(or)X
2581(applications)X
10 f
555 4944(g)N
1 f
775(A)X
860(private)X
1127(``.h'')X
1331(\256le)X
1466(used)X
1649(by)X
1759(widgets)X
2055(that)X
2210(are)X
2339(subclasses)X
2727(of)X
2822(the)X
2952(widget)X
10 f
555 5068(g)N
1 f
775(A)X
860(``.c'')X
1059(\256le)X
1194(that)X
1349(implements)X
1784(the)X
1914(widget)X
2176(class)X
3 f
555 5260(6.1.)N
731(Widget)X
1027(Subclassing)X
1490(in)X
1586(Public)X
1849(``.h'')X
2058(Files)X
1 f
555 5384(The)N
714(public)X
957(``.h'')X
1161(\256le)X
1296(for)X
1420(a)X
1481(widget)X
1743(class)X
1936(is)X
2017(imported)X
2358(by)X
2468(clients)X
2721(and)X
2870(contains:)X
10 f
555 5508(g)N
1 f
775(The)X
934(atom)X
1133(names)X
1380(\(strings\))X
1695(of)X
1790(the)X
1920(new)X
2088(resources)X
2441(that)X
2596(this)X
2746(widget)X
3008(adds)X
3191(to)X
3282(its)X
3388(superclass)X
10 f
555 5632(g)N
1 f
775(The)X
934(class)X
1127(record)X
1373(pointer)X
1645(for)X
1769(the)X
1899(widget)X
3 f
2261 6144(8)N

9 p
%%Page: 9 11
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
10 f
555 672(g)N
1 f
775(The)X
934(C)X
1015(type)X
1189(you)X
1343(use)X
1482(to)X
1573(declare)X
1849(widgets)X
2145(of)X
2240(this)X
2390(class)X
555 796(For)N
699(example,)X
1042(here)X
1215(is)X
1296(the)X
1426(public)X
1669(``.h'')X
1873(\256le)X
2008(for)X
2132(the)X
2262(Label)X
2485(widget:)X
555 920(/*)N
646(New)X
833(resources)X
1186(*/)X
555 1016(#de\256ne)N
836(XtNjustify)X
1419("justify")X
555 1112(#de\256ne)N
836(XtNforeground)X
1419("foreground")X
555 1208(#de\256ne)N
836(XtNlabel)X
1419("label")X
555 1304(#de\256ne)N
836(XtNfont)X
1419("font")X
555 1400(#de\256ne)N
836(XtNinternalWidth)X
2283("internalWidth")X
555 1496(#de\256ne)N
836(XtNinternalHeight)X
2283("internalHeight")X
555 1688(/*)N
646(Class)X
859(record)X
1105(pointer)X
1377(*/)X
555 1784(extern)N
797(WidgetClass)X
1270(labelWidgetClass;)X
555 1976(/*)N
646(C)X
727(Widget)X
1009(type)X
1183(de\256nition)X
1544(*/)X
555 2072(typedef)N
841(struct)X
1059(_LabelRec)X
1573(*LabelWidget;)X
3 f
555 2312(6.2.)N
731(Widget)X
1027(Subclassing)X
1490(in)X
1586(Private)X
1882(``.h'')X
2091(Files)X
1 f
555 2436(The)N
717(private)X
987(``.h'')X
1194(\256le)X
1332(for)X
1459(a)X
1523(widget)X
1788(is)X
1872(imported)X
2216(by)X
2329(widget)X
2594(classes)X
2863(that)X
3021(are)X
3152(subclasses)X
3542(of)X
3639(the)X
3771(widget)X
555 2532(and)N
704(contains:)X
10 f
555 2656(g)N
1 f
775(The)X
934(new)X
1102(\256elds)X
1315(that)X
1470(the)X
1600(widget)X
1862(instance)X
2173(adds)X
2356(to)X
2447(its)X
2553(superclass's)X
2999(widget)X
3261(structure.)X
10 f
555 2780(g)N
1 f
775(The)X
934(complete)X
1280(widget)X
1542(instance)X
1853(structure)X
2183(for)X
2307(this)X
2457(widget.)X
10 f
555 2904(g)N
1 f
775(The)X
934(new)X
1102(\256elds)X
1315(that)X
1470(this)X
1620(widget)X
1882(class)X
2075(adds)X
2258(to)X
2349(its)X
2455(superclass's)X
2901(widget)X
3163(class)X
3356(structure.)X
10 f
555 3028(g)N
1 f
775(The)X
934(complete)X
1280(widget)X
1542(class)X
1735(structure)X
2065(for)X
2189(this)X
2339(widget.)X
10 f
555 3152(g)N
1 f
775(A)X
860(``constant'')X
1292(of)X
1387(the)X
1517(generic)X
1798(widget)X
2060(class)X
2253(structure.)X
555 3276(For)N
699(example,)X
1042(here)X
1215(is)X
1296(the)X
1426(private)X
1693(``.h'')X
1897(\256le)X
2032(for)X
2156(the)X
2286(Label)X
2509(widget:)X
555 3400(/*)N
646(New)X
833(\256elds)X
1046(for)X
1170(the)X
1300(Label)X
1523(widget)X
1785(record)X
2031(*/)X
555 3496(typedef)N
841(struct)X
1059({)X
555 3592(/*)N
646(Settable)X
953(resources)X
1306(*/)X
843 3688(Pixel)N
1047(foreground;)X
843 3784(XFontStruct)N
1301(*font;)X
843 3880(String)N
1081(label;)X
843 3976(XtJustify)N
1188(justify;)X
843 4072(Dimension)N
1252(internal_width;)X
843 4168(Dimension)N
1252(internal_height;)X
555 4360(/*)N
646(Data)X
834(derived)X
1120(from)X
1313(resources)X
1666(*/)X
843 4456(GC)N
987(normal_GC;)X
843 4552(GC)N
987(gray_GC;)X
843 4648(Pixmap)N
1135(gray_pixmap;)X
843 4744(Position)N
1155(label_x;)X
843 4840(Position)N
1155(label_y;)X
843 4936(Dimension)N
1252(label_width;)X
843 5032(Dimension)N
1252(label_height;)X
843 5128(Cardinal)N
1169(label_len;)X
843 5224(Boolean)N
1159(display_sensitive;)X
555 5320(})N
619(LabelPart;)X
3 f
2261 6144(9)N

10 p
%%Page: 10 12
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(/*)N
646(Full)X
811(instance)X
1122(record)X
1368(declaration)X
1782(*/)X
555 768(typedef)N
841(struct)X
1059(_LabelRec)X
1463({)X
843 864(CorePart)N
1178(core;)X
843 960(LabelPart)N
1208(label;)X
555 1056(})N
619(LabelRec;)X
555 1248(/*)N
646(New)X
833(\256elds)X
1046(for)X
1170(the)X
1300(Label)X
1523(widget)X
1785(class)X
1978(record)X
2224(*/)X
555 1344(typedef)N
841(struct)X
1059({)X
843 1440(int)N
959(mumble;)X
2283(/*)X
2374(No)X
2503(new)X
2671(\256elds,)X
2906(but)X
3041(keep)X
3229(C)X
3310(compiler)X
3646(happy)X
3883(*/)X
555 1536(})N
619(LabelClassPart;)X
555 1728(/*)N
646(Full)X
811(class)X
1004(record)X
1250(declaration)X
1664(*/)X
555 1824(typedef)N
841(struct)X
1059(_LabelClassRec)X
1654({)X
843 1920(CoreClassPart)N
1369(core_class;)X
843 2016(LabelClassPart)N
1399(label_class;)X
555 2112(})N
619(LabelClassRec;)X
555 2304(/*)N
646(Class)X
859(record)X
1105(variable)X
1411(*/)X
555 2400(extern)N
797(LabelClassRec)X
1348(labelClassRec;)X
3 f
555 2668(6.3.)N
731(Widget)X
1027(Subclassing)X
1490(in)X
1586(``.c'')X
1785(Files)X
1 f
555 2792(The)N
715(``.c'')X
915(\256le)X
1050(for)X
1174(a)X
1235(widget)X
1497(contains)X
1813(the)X
1943(initializer)X
2305(for)X
2429(the)X
2559(class)X
2752(record)X
2998(variable.)X
3348(This)X
3527(initializer)X
3889(can)X
555 2888(be)N
660(broken)X
926(up)X
1036(into)X
1196(several)X
1467(parts:)X
10 f
555 3012(g)N
1 f
775(Class)X
988(information)X
1427(\(superclass,)X
1861(class_name,)X
2311(widget_size,)X
2776 0.1953(class_initialize,)AX
3346(class_inited\))X
10 f
555 3136(g)N
1 f
775(Data)X
975(Constants)X
1357(\(resources)X
1751(and)X
1912(num_resources,)X
2500(actions)X
2783(and)X
2943(num_actions,)X
3449(visible_interest,)X
775 3232(compress_motion,)N
1446(compress_expose\))X
10 f
555 3356(g)N
1 f
775(Widget)X
1057(Operations)X
1465(\(initialize,)X
1849(realize,)X
2128(destroy,)X
2431(resize,)X
2680(expose,)X
2968(set_values,)X
3379(accept_focus\))X
555 3480(The)N
725(superclass)X
1119(\256eld)X
1309(points)X
1557(to)X
1658(the)X
1798(superclass)X
2191(record.)X
2491(For)X
2645(all)X
2766(direct)X
2999(subclass)X
3324(of)X
3429(the)X
3569(generic)X
3860(core)X
555 3576(widget)N
817(this)X
967(means)X
1214(that)X
1369(superclass)X
1752(is)X
1833 0.3187(initialized)AX
2210(to)X
2301(the)X
2431(address)X
2716(of)X
2811(the)X
5 f
2959(widgetClassRec)X
1 f
3625(structure.)X
555 3700(The)N
717(class_name)X
1148(\256eld)X
1330(contains)X
1649(the)X
1782(text)X
1940(name)X
2156(for)X
2283(this)X
2436(class)X
2632(\(that)X
2819(is)X
2903(used)X
3089(by)X
3202(the)X
3335(resource)X
3657(manager\).)X
555 3796(For)N
707(example,)X
1058(the)X
1196(Label)X
1427(widget)X
1697(has)X
1844(the)X
1982(string)X
2213(``Label''.)X
2604(The)X
2771(widget_size)X
3222(\256eld)X
3409(is)X
3498(the)X
3635(size)X
3801(of)X
3903(the)X
555 3892(corresponding)N
1084(Widget)X
1370(structure,)X
1725(not)X
1863(the)X
1996(size)X
2158(of)X
2256(the)X
2389(Class)X
2605(structure.)X
2982(All)X
3120(other)X
3326(\256elds)X
3542(are)X
3674(described)X
555 3988(in)N
646(their)X
830(respective)X
1209(sections.)X
555 4112(Here)N
747(is)X
828(a)X
889(somewhat)X
1268(compressed)X
1705(version)X
1986(of)X
2081(``.h'')X
2285(\256le)X
2420(for)X
2544(the)X
2674(Label)X
2897(widget:)X
3 f
2239 6144(10)N

11 p
%%Page: 11 13
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(/*)N
646(Resources)X
1029(speci\256c)X
1320(to)X
1411(Label)X
1634(*/)X
555 768(#de\256ne)N
836(XtRjustify)X
2283("Justify")X
555 864(static)N
764(XtResource)X
1201(resources[])X
1612(=)X
1684({)X
843 960({XtNforeground,)N
1470(XtCForeground,)X
2071(XrmRPixel,)X
2517(sizeof\(Pixel\),)X
909 1056(XtOffset\(LabelWidget,)N
1750(label.foreground\),)X
2407(XrmRString,)X
2887("Black"},)X
843 1152({XtNfont,)N
1244(XtCFont,)X
1597(XrmRFontStruct,)X
2234(sizeof\(XFontStruct)X
2931(*\),)X
909 1248(XtOffset\(LabelWidget,)N
1750(label.font\),XrmRString,)X
2617("Fixed"},)X
843 1344({XtNlabel,)N
1274(XtCLabel,)X
1666(XrmRString,)X
2146(sizeof\(String\),)X
909 1440(XtOffset\(LabelWidget,)N
1750(label.label\),)X
2189(XrmRString,)X
2669(NULL},)X
2283 1536(.)N
2283 1632(.)N
2283 1728(.)N
555 1824(})N
555 2016(/*)N
646(Forward)X
965(declarations)X
1413(of)X
1508(procedures)X
1915(*/)X
555 2112(static)N
764(void)X
943(ClassInitialize\(\);)X
555 2208(static)N
764(void)X
943(Initialize\(\);)X
555 2304(static)N
764(void)X
943(Realize\(\);)X
843 2400(.)N
843 2496(.)N
843 2592(.)N
3 f
2239 6144(11)N

12 p
%%Page: 12 14
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(/*)N
646(Class)X
859(record)X
1105(constant)X
1421(*/)X
555 768(LabelClassRec)N
1106(labelClassRec)X
1628(=)X
1700({)X
599 864({)N
555 960(/*)N
646(core_class)X
1034(\256elds)X
1247(*/)X
843 1056(/*)N
934(superclass)X
1471(*/)X
2283(\(WidgetClass\))X
2814(&widgetClassRec,)X
843 1152(/*)N
934(class_name)X
1516(*/)X
2283("Label",)X
843 1248(/*)N
934(widget_size)X
1509(*/)X
2283(sizeof\(LabelRec\),)X
843 1344(/*)N
934 0.2083(class_initialize)AX
1504(*/)X
2283(ClassInitialize,)X
843 1440(/*)N
934(class_inited)X
1483(*/)X
2283(FALSE,)X
843 1536(/*)N
934 0.3542(initialize)AX
1421(*/)X
2283(Initialize,)X
843 1632(/*)N
934(realize)X
1411(*/)X
2283(Realize,)X
843 1728(/*)N
934(actions)X
1426(*/)X
2283(NULL,)X
843 1824(/*)N
934(num_actions)X
1539(*/)X
2283(0,)X
843 1920(/*)N
934(resources)X
1463(*/)X
2283(resources,)X
843 2016(/*)N
934(num_resources)X
1576(*/)X
2283(XtNumber\(resources\),)X
843 2112(/*)N
934(xrm_class)X
1489(*/)X
2283(NULLQUARK,)X
843 2208(/*)N
934(compress_motion)X
1627(*/)X
2283(TRUE,)X
843 2304(/*)N
934(compress_exposure)X
1649(*/)X
2283(TRUE,)X
843 2400(/*)N
934(visible_interest)X
1518(*/)X
2283(FALSE,)X
843 2496(/*)N
934(destroy)X
1435(*/)X
2283(NULL,)X
843 2592(/*)N
934(resize)X
1403(*/)X
2283(Resize,)X
843 2688(/*)N
934(expose)X
1442(*/)X
2283(Redisplay,)X
843 2784(/*)N
934(set_values)X
1477(*/)X
2283(SetValues,)X
843 2880(/*)N
934(accept_focus)X
1525(*/)X
2283(NULL,)X
599 2976(})N
555 3072(};)N
555 3264(/*)N
646(Class)X
859(record)X
1105(pointer)X
1377(*/)X
555 3360(WidgetClass)N
1028(labelWidgetClass)X
1673(=)X
1745(\(WidgetClass\))X
2276(&labelClassRec;)X
555 3552(/*)N
646(Private)X
918(procedures)X
1325(*/)X
843 3648(.)N
843 3744(.)N
843 3840(.)N
3 f
555 4080(6.4.)N
731(Class)X
953(Initialization:)X
1490(the)X
1629(class_initialize)X
2196(procedure)X
1 f
555 4204(Many)N
785(class)X
981(records)X
1264(can)X
1411(be)X
1518 0.3187(initialized)AX
1897(completely)X
2314(at)X
2402(compile)X
2711(time.)X
2937(But)X
3089(in)X
3182(some)X
3392(cases)X
3601(a)X
3664(class)X
3859(may)X
555 4300(want)N
753(to)X
849(register)X
1140(type)X
1319(converters,)X
1734(inherit)X
1992(procedures)X
2404(from)X
2602(its)X
2713(superclass)X
3101(by)X
3216(copying)X
3527(them)X
3730(from)X
3927(its)X
555 4396(superclass's)N
1006(record,)X
1279(register)X
1570(unique)X
1837(callback)X
2158(kinds,)X
2398(or)X
2497(perform)X
2806(other)X
3013(sorts)X
3205(of)X
3304(``one-shot'')X
3749(initiali-)X
555 4492(zation.)N
555 4616(Because)N
882(the)X
1024(C)X
1116(language)X
1467(does)X
1661(not)X
1807(have)X
2006 0.3317(initialization)AX
2488(procedures)X
2906(that)X
3072(are)X
3212(invoked)X
3529 0.2604(automatically)AX
555 4712(when)N
781(a)X
856(program)X
1190(starts)X
1412(up,)X
1558(a)X
1633(widget)X
1909(class)X
2116(can)X
2274(declare)X
2564(a)X
2639 0.2083(class_initialize)AX
3201(procedure)X
3587(that)X
3755(will)X
3928(be)X
555 4808(called)N
791(exactly)X
1071(once)X
1262 0.2604(automatically)AX
1769(by)X
1882(the)X
2015(toolkit.)X
2315(A)X
2402(class)X
2597 0.3317(initialization)AX
3070(procedure)X
3445(takes)X
3650(no)X
3762(param-)X
555 4904(eters)N
743(and)X
892(returns)X
1158(no)X
1268(result:)X
555 5028(typedef)N
841(void)X
1020(\(*XtProc\)\(\);)X
555 5200(All)N
693(widget)X
958(classes,)X
1249(whether)X
1557(they)X
1734(have)X
1925(a)X
1989(class)X
2185 0.3317(initialization)AX
2659(procedure)X
3035(or)X
3133(not,)X
3293(should)X
3552(start)X
3728(off)X
3854(with)X
555 5296(their)N
739(class_inited)X
1178(\256eld)X
1357(FALSE.)X
555 5420(The)N
723(\256rst)X
891(time)X
1080(a)X
1150(widget)X
1421(of)X
1525(that)X
1689(class)X
1891(is)X
1981(created,)X
5 f
2306(XtCreateWidget)X
1 f
2974(ensures)X
3268(that)X
3432(the)X
3570(widget)X
3840(class)X
555 5516(\(and)N
768(all)X
914(superclasses\))X
1434(are)X
1598 0.3187(initialized)AX
2010(\(in)X
2165(superclass)X
2583(to)X
2709(subclass)X
3059(order\))X
3330(by)X
3475(checking)X
3850(each)X
555 5612(class_inited)N
1004(\256eld)X
1193(and)X
1352(calling)X
1625(the)X
1765 0.2237(class_initialization)AX
2461(procedure)X
2844(if)X
2930(it)X
3012(is)X
3103(FALSE.)X
3426(The)X
3594(class_inited)X
555 5708(\256eld)N
734(is)X
815(then)X
989(set)X
1109(to)X
1200(TRUE.)X
1474(After)X
1681(the)X
1811(one-time)X
2147 0.3080(initialization,)AX
2640(a)X
2701(class)X
2894(structure)X
3224(is)X
3305(constant.)X
3 f
2239 6144(12)N

13 p
%%Page: 13 15
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(Here)N
760(is)X
854(the)X
997(class)X
1203 0.3317(initialization)AX
1687(procedure)X
2073(for)X
2210(ButtonBox,)X
2655(a)X
2728(subclass)X
3055(of)X
3162(Composite.)X
3623(ButtonBox)X
555 768(inherits)N
850(its)X
964(superclass's)X
1418(insert_child)X
1864(and)X
2020(delete_child)X
2481(routines.)X
2816(That)X
3007(is,)X
3117(it)X
3196(uses)X
3376(whatever)X
3727(routines)X
555 864(Composite)N
960(has)X
1099(de\256ned)X
1380(for)X
1504(these)X
1707(\256elds:)X
555 988(static)N
764(void)X
943(ClassInitialize\(\))X
555 1084({)N
843 1180(CompositeWidgetClass)N
1699(superclass;)X
843 1276 0.1612(ButtonBoxWidgetClass)AN
1704(myclass;)X
843 1468(myclass)N
1149(=)X
1221(\(ButtonBoxWidgetClass\))X
2140 0.1656(buttonBoxWidgetClass;)AX
843 1564(superclass)N
1226(=)X
1298(\(CompositeWidgetClass\))X
2212(myclass->core_class.superclass;)X
555 1756(/*)N
646(Inherit)X
903(insert_child)X
1342(and)X
1491(delete_child)X
1945(from)X
2138(Composite)X
2543(*/)X
843 1852 0.0938(myclass->composite_class.insert_child)AN
2245(=)X
909 1948(superclass->composite_class.insert_child;)N
843 2044 0.1059(myclass->composite_class.delete_child)AN
2260(=)X
909 2140 0.0797(superclass->composite_class.delete_child;)AN
555 2236(})N
3 f
555 2476(6.5.)N
731(XtClass)X
1 f
555 2696(WidgetClass)N
1028(XtClass\()X
2 f
(w)S
1 f
1395(\))X
687 2792(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 3040(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
5 f
570 3164(XtClass)N
1 f
910(returns)X
1176(a)X
1237(pointer)X
1509(to)X
1600(the)X
1730(widget)X
1992(class)X
2185(structure.)X
2559(It)X
2635(is)X
2716(simply)X
2979(an)X
3084(abbreviation)X
3547(for:)X
2 f
843 3260(w)N
1 f
902(->)X
2 f
981(core)X
1 f
1137(.)X
2 f
(widget_class)S
3 f
555 3500(6.6.)N
731(XtSuperclass)X
1 f
555 3720(WidgetClass)N
1028(XtSuperclass\()X
2 f
1521(w)X
1 f
1580(\))X
687 3816(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 4064(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
5 f
570 4188(XtSuperclass)N
1 f
1129(returns)X
1398(a)X
1462(pointer)X
1737(to)X
1831(the)X
1964(widget's)X
2292(superclass)X
2678(class)X
2874(structure.)X
3251(It)X
3330(is)X
3414(simply)X
3680(an)X
3787(abbre-)X
555 4284(viation)N
823(for:)X
2 f
843 4380(w)N
1 f
902(->)X
2 f
981(core)X
1 f
1137(.)X
2 f
(widget_class)S
1 f
1615(->)X
2 f
1694(core_class)X
1 f
2070(.)X
2 f
(superclass)S
3 f
555 4620(6.7.)N
731(XtIsSubclass)X
1 f
555 4840(Boolean)N
871(XtIsSubclass\()X
2 f
1359(w)X
1 f
1418(,)X
2 f
1462(widget_class)X
1 f
1918(\))X
687 4936(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 5032(WidgetClass)N
2 f
1160(widget_class)X
1 f
1616(;)X
2 f
555 5280(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(under)X
2085(question.)X
2 f
555 5404(widget_class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(class)X
2056(to)X
2147(test)X
2292(against.)X
5 f
570 5528(XtIsSubclass)N
1 f
1122(returns)X
1393(TRUE)X
1650(if)X
1731(the)X
1866(speci\256ed)X
2206(w)X
2296(has)X
2440(a)X
2506(class,)X
2726(which)X
2968(is)X
3054(equal)X
3272(to)X
3368(or)X
3468(is)X
3554(a)X
3619(subclass)X
3938(of)X
555 5624(widget_class.)N
1080(Composite)X
1489(widgets)X
1789(that)X
1948(wish)X
2140(to)X
2235(restrict)X
2506(the)X
2640(class)X
2837(of)X
2935(the)X
3068(items)X
3285(they)X
3462(contain)X
3747(can)X
3894(use)X
5 f
570 5720(XtIsSubclass)N
1 f
1117(to)X
1208(\256nd)X
1367(out)X
1502(if)X
1578(a)X
1639(widget)X
1901(belongs)X
2197(to)X
2288(the)X
2418(desired)X
2694(class)X
2887(of)X
2982(objects.)X
3 f
2239 6144(13)N

14 p
%%Page: 14 16
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(Note)N
748(that)X
903(the)X
1033(Boolean)X
1349(function)X
5 f
1683(XtIsComposite)X
1 f
2293(is)X
2374(simply)X
2637(an)X
2742(abbreviation)X
3205(for)X
5 f
3347(XtIsSubclass)X
1 f
3872(.)X
555 892(XtIsSubclass\()N
2 f
1043(w)X
1 f
1102(,)X
2 f
1139(compositeWidgetClass)X
1 f
1942(\))X
3 f
555 1208(6.8.)N
731(XtCheckSubclass)X
1 f
555 1428(void)N
734(XtCheckSubclass\()X
2 f
1384(w)X
1 f
1443(,)X
2 f
1487(widget_class)X
1 f
1943(\))X
687 1524(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 1620(WidgetClass)N
2 f
1160(widget_class)X
1 f
1616(;)X
2 f
555 1868(parent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(under)X
2085(question.)X
2 f
555 1992(widget_class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(class)X
2056(to)X
2147(test)X
2292(against.)X
5 f
570 2116(XtCheckSubclass)N
1 f
1305(determines)X
1722(if)X
1805(the)X
1942(speci\256ed)X
2284(w)X
2376(has)X
2522(a)X
2590(class)X
2790(that)X
2952(is)X
3040(equal)X
3260(to)X
3358(or)X
3460(is)X
3548(a)X
3616(subclass)X
3938(of)X
555 2212(widget_class.)N
1086(If)X
1176(not,)X
5 f
1361(XtCheckSubclass)X
1 f
2098(issues)X
2340(an)X
2455(error)X
2657(and)X
2816(aborts)X
3063(the)X
3203(program.)X
3577(It)X
3662(should)X
3928(be)X
555 2308(used)N
751(at)X
850(the)X
993(entry-point)X
1420(of)X
1528(routines)X
1847(that)X
2015(can)X
2172(be)X
2290(called)X
2536(by)X
2659(clients)X
2924(in)X
3027(order)X
3246(to)X
3349(determine)X
3736(that)X
3903(the)X
555 2404(client)N
774(has)X
913(passed)X
1169(in)X
1260(a)X
1321(valid)X
1520(widget)X
1782(class)X
1975(for)X
2099(the)X
2229(exported)X
2559(operation.)X
5 f
570 2528(XtCheckSubclass)N
1 f
1304(is)X
1392(only)X
1578(executed)X
1920(when)X
2139(including)X
2502(and)X
2658(linking)X
2938(against)X
3216(the)X
3352(debugging)X
3752(version)X
555 2624(of)N
650(the)X
780(intrinsics.)X
1170(Otherwise,)X
1575(it)X
1647(is)X
1728(de\256ned)X
2009(as)X
2104(the)X
2234(empty)X
2477(string)X
2700(and)X
2849(so)X
2949(generates)X
3303(no)X
3413(code.)X
2197 2800(Note)N
775 2936(This)N
954(routine)X
1226(currently)X
1566(is)X
1647(not)X
1782(yet)X
1912(implemented.)X
3 f
555 3208(6.9.)N
731(Superclass)X
1154(Chaining)X
1 f
555 3332(Some)N
789(\256elds)X
1013(de\256ned)X
1305(in)X
1407(the)X
1548(widget)X
1821(class)X
2025(structure)X
2366(are)X
2506(self-contained)X
3037(and)X
3196(are)X
3335(independent)X
3798(of)X
3903(the)X
555 3428(values)N
802(for)X
926(these)X
1129(\256elds)X
1342(de\256ned)X
1623(in)X
1714(superclasses.)X
2214(These)X
2446(are:)X
10 f
555 3552(g)N
1 f
775(class_name)X
10 f
555 3676(g)N
1 f
775(widget_size)X
10 f
555 3800(g)N
1 f
775(realize)X
10 f
555 3924(g)N
1 f
775(actions)X
10 f
555 4048(g)N
1 f
775(visible_interest)X
10 f
555 4172(g)N
1 f
775(resize)X
10 f
555 4296(g)N
1 f
775(expose)X
10 f
555 4420(g)N
1 f
775(accept_focus)X
555 4544(Some)N
783(\256elds)X
1001(de\256ned)X
1287(in)X
1383(the)X
1518(widget)X
1785(class)X
1983(structure)X
2318(make)X
2536(sense)X
2753(only)X
2937(after)X
3125(their)X
3314(superclass)X
3702(has)X
3845(been)X
555 4640(operated)N
887(on.)X
1048(In)X
1150(this)X
1307(case,)X
1509(invocation)X
1911(of)X
2013(a)X
2081(single)X
2321(operation)X
2683(actually)X
2992(\256rst)X
3158(accesses)X
3483(the)X
3619(Core)X
3818(class,)X
555 4736(then)N
754(the)X
909(subclass,)X
1270(and)X
1443(on)X
1577(down)X
1818(the)X
1972(class)X
2189(chain)X
2426(to)X
2541(the)X
2695(widget)X
2981(class)X
3198(of)X
3317(the)X
3471(widget.)X
3801(These)X
555 4832(superclass-to-subclass)N
1358(\256elds)X
1571(are:)X
10 f
555 4956(g)N
1 f
775 0.3542(initialize)AX
10 f
555 5080(g)N
1 f
775(set_values)X
10 f
555 5204(g)N
1 f
775(resources)X
1131(\(For)X
1307(subclasses)X
1698(of)X
1796(Constraint,)X
2211(the)X
2344(constraints)X
2751(\256eld)X
2933(is)X
3017(chained)X
3316(from)X
3511(the)X
3643(Constraint)X
775 5300(class)N
968(down)X
1185(to)X
1276(the)X
1406(subclass.\))X
555 5424(Some)N
788(\256elds)X
1011(de\256ned)X
1302(in)X
1403(the)X
1543(widget)X
1815(class)X
2018(structure)X
2357(make)X
2579(sense)X
2800(only)X
2988(after)X
3180(their)X
3373(subclass)X
3697(has)X
3845(been)X
555 5520(operated)N
888(on.)X
1050(In)X
1153(this)X
1311(case,)X
1514(the)X
1652(invocation)X
2055(of)X
2158(a)X
2227(single)X
2468(operation)X
2831(actually)X
3141(\256rst)X
3308(accesses)X
3634(the)X
3771(widget)X
555 5616(class,)N
786(then)X
976(its)X
1097(superclass,)X
1517(and)X
1681(on)X
1806(up)X
1931(the)X
2076(class)X
2284(chain)X
2512(to)X
2618(the)X
2763(Core)X
2971(class.)X
3223(The)X
3397(only)X
3591(subclass-to-)X
555 5712(superclass)N
938(\256eld)X
1117(is)X
1198(``destroy''.)X
3 f
2239 6144(14)N

15 p
%%Page: 15 17
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(6.10.)N
775(Inheriting)X
1179(Superclass)X
1602(Operations)X
1 f
555 796(A)N
648(widget)X
918(class)X
1119(is)X
1208(free)X
1373(to)X
1471(use)X
1617(any)X
1773(of)X
1875(its)X
1988(superclass's)X
2441(self-contained)X
2969(operations)X
3365(rather)X
3599(than)X
3780(imple-)X
555 892(menting)N
867(its)X
973(own)X
1146(code.)X
1378(The)X
1537(most)X
1731(frequently)X
2115(inherited)X
2451(operations)X
2840(are:)X
10 f
555 1016(g)N
1 f
775(expose)X
10 f
555 1140(g)N
1 f
775(realize)X
10 f
555 1264(g)N
1 f
775(insert_child)X
10 f
555 1388(g)N
1 f
775(delete_child)X
555 1512(To)N
694(inherit)X
966(an)X
1090(operation,)X
1486(you)X
1659(simply)X
1941(write)X
2163(code)X
2369(in)X
2478(the)X
2626 0.2083(class_initialize)AX
3192(procedure)X
3583(to)X
3692(copy)X
3903(the)X
555 1608(superclass's)N
1011(procedure)X
1394(into)X
1564(the)X
1704(subclass)X
2029(structure.)X
2413(See)X
2572(the)X
2712(example)X
3043(above)X
3285(in)X
3386(``Class)X
3667(Initializa-)X
555 1704(tion''.)N
3 f
555 1896(6.11.)N
775(Calling)X
1072(Superclass)X
1495(Operations)X
1 f
555 2048(A)N
655(widget)X
932(class)X
1140(sometimes)X
1555(wants)X
1797(to)X
1903(explicitly)X
2274(call)X
2438(a)X
2513(superclass)X
2910(operation)X
3279(that)X
3448(is)X
3543(not)X
3692(normally)X
555 2144(chained.)N
904(For)X
1057(example,)X
1408(a)X
1477(widget's)X
1810(expose)X
2084(procedure)X
2465(might)X
2702(call)X
2860(its)X
2974(superclass's)X
3428(expose)X
3702(and)X
3859(then)X
555 2240(perform)N
878(a)X
957(little)X
1161(more)X
1382(work)X
1602(of)X
1715(its)X
1839(own.)X
2074(Composite)X
2497(classes)X
2781(with)X
2977(\256xed)X
3192(children)X
3520(\(for)X
3690(example,)X
555 2336(Scrollbar)N
921(and)X
1091(the)X
1242(top-level)X
1599(widget\))X
1911(implement)X
2332(insert_child)X
2791(by)X
2921(\256rst)X
3100(calling)X
3383(their)X
3587(superclass's)X
555 2432(insert_child)N
994(procedure)X
1367(and)X
1516(then)X
1690(calling)X
5 f
1971(XtManageChild)X
1 f
2609(to)X
2700(add)X
2849(the)X
2979(child)X
3178(to)X
3269(the)X
3399(managed)X
3739(list.)X
555 2556(Here)N
747(is)X
828(the)X
958(code)X
1146(for)X
1270(the)X
1400(top-level)X
1736(widget's)X
2 f
2061(insert_child)X
1 f
2483(:)X
555 2680(static)N
764(void)X
943(InsertChild\(w\))X
643 2776(Widget)N
925(w;)X
555 2872({)N
843 2968(\(\(CompositeWidgetClass\))N
1786(XtSuperclass\(w\)\)->composite_class.insert_child\(w\);)X
843 3064(XtManageChild\(w\);)N
2283(/*)X
2374(Add)X
2547(to)X
2638(managed)X
2978(set)X
3098(now)X
3271(*/)X
555 3160(})N
3 f
555 3400(7.)N
665(The)X
834(Widget)X
1130(Tree)X
1 f
555 3524(Widgets)N
874(are)X
1006(either)X
1232(``primitive'')X
1698(or)X
1795(``composite''.)X
2320(A)X
2407(primitive)X
2756(widget)X
3020(has)X
3161(no)X
3273(children)X
3586(widgets)X
3884(and)X
555 3620(can)N
706(never)X
930(have)X
1125(them.)X
1352(A)X
1443(composite)X
1834(widget)X
2102(may)X
2282(in)X
2379(unusual)X
2681(circumstances)X
3208(have)X
3402(zero)X
3581(children)X
3898(but)X
555 3716(usually)N
840(has)X
987(at)X
1081(least)X
1273(one.)X
1451(Primitive)X
1810(widgets)X
2113(are)X
2249(leaves)X
2498(of)X
2600(the)X
2737(widget)X
3006(tree.)X
3189(Composite)X
3601(widgets)X
3904(are)X
555 3812(intermediate)N
1024(nodes)X
1256(with)X
1440(children)X
1756(and)X
1910(the)X
2045(top-level)X
2386(widget)X
2653(returned)X
2973(by)X
5 f
3106(XtInitialize)X
1 f
3549(is)X
3635(the)X
3770(root)X
3938(of)X
555 3908(the)N
685(widget)X
947(tree.)X
555 4032(Except)N
830(for)X
962(pop-up)X
1241(widget)X
1511(windows)X
1858(\(which)X
2132(are)X
2269(children)X
2588(of)X
2691(the)X
2829(root)X
3000(window)X
3312(so)X
3419(that)X
3581(they)X
3762(are)X
3898(not)X
555 4128(clipped\),)N
888(the)X
1018(widget)X
1280(tree)X
1434(exactly)X
1711(duplicates)X
2091(the)X
2221(associated)X
2605(window)X
2910(tree.)X
555 4252(The)N
727(widget)X
1002(tree)X
1169(is)X
1263(manipulated)X
1735(by)X
1858(several)X
2142(toolkit)X
2409(functions.)X
2816(For)X
2973(example,)X
5 f
3346(XtRealizeWidget)X
1 f
555 4348(traverses)N
894(the)X
1029(tree)X
1188(downward)X
1585(to)X
1681(recursively)X
2099(realize)X
2361(widgets.)X
5 f
2702(XtDestroyWidget)X
1 f
3405(traverses)X
3744(the)X
3879(tree)X
555 4444(downward)N
964(to)X
1072(destroy)X
1369(all)X
1496(children.)X
1845(The)X
2020(internal)X
2328(routine)X
5 f
2634(_XtGetResources)X
1 f
3383(traverses)X
3733(the)X
3879(tree)X
555 4540(upward)N
881(to)X
1012(determine)X
1427(the)X
1597(inheritance)X
2051(of)X
2186(resources)X
2579(from)X
2812(a)X
2913(widget's)X
3278(ancestors.)X
5 f
3732(XtMak-)X
555 4636(eGeometryRequest)N
1 f
1361(traverses)X
1700(the)X
1835(tree)X
1994(one)X
2148(level)X
2347(upward)X
2637(to)X
2733(get)X
2867(the)X
3001(geometry)X
3360(manager)X
3689(responsi-)X
555 4732(ble)N
685(for)X
809(the)X
939(widget's)X
1264(geometry.)X
555 4856(To)N
680(facilitate)X
1017(up-traversal)X
1464(of)X
1564(the)X
1699(widget)X
1966(tree,)X
2147(each)X
2334(widget)X
2600(has)X
2743(a)X
2808(pointer)X
3084(to)X
3179(its)X
3289(parent)X
3535(widget.)X
3845(\(The)X
555 4952(top-level)N
891(widget)X
1153(returned)X
1468(by)X
5 f
1596(XtInitialize)X
1 f
2034(has)X
2173(a)X
2234(parent)X
2476(pointer)X
2748(of)X
2843(NULL.\))X
555 5076(To)N
677(facilitate)X
1011(down-traversal)X
1562(of)X
1658(the)X
1789(widget)X
2052(tree,)X
2229(each)X
2413(composite)X
2799(widget)X
3062(has)X
3202(a)X
3264(pointer)X
3537(to)X
3629(an)X
3735(array)X
3938(of)X
555 5172(children)N
871(widgets.)X
1216(This)X
1400(array)X
1606(includes)X
1926(all)X
2041(children)X
2356(created,)X
2658(not)X
2797(just)X
2951(the)X
3085(subset)X
3331(of)X
3430(children)X
3745(that)X
3904(are)X
555 5268(managed)N
895(by)X
1005(the)X
1135(composite)X
1520(widget's)X
1845(geometry)X
2200(manager.)X
3 f
555 5460(8.)N
665(Window)X
1005(Attributes)X
1 f
555 5584(The)N
717(Core)X
913(widget)X
1178(de\256nition)X
1542(contains)X
1861(screen)X
2110(and)X
2262(window.)X
2592(Therefore,)X
2985(widgets)X
3284(and)X
3436(intrinsics)X
3785(do)X
3898(not)X
555 5680(have)N
752(to)X
852(pass)X
1034(them.)X
1286(The)X
1453(window)X
1766(\256eld)X
1953(may)X
2135(be)X
2248(NULL)X
2512(for)X
2644(a)X
2713(while)X
2939(\(see)X
3110(``Creating)X
3502(Widgets'')X
3884(and)X
555 5776(``XtRealizeWidget''\).)N
3 f
2239 6144(15)N

16 p
%%Page: 16 18
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
720(display)X
1003(pointer,)X
1303(screen)X
1555(pointer,)X
1855(and)X
2010(window)X
2321(of)X
2422(a)X
2489(widget)X
2757(are)X
2892(returned)X
3213(by)X
3329(the)X
3465(following)X
3835(mac-)X
555 768(ros:)N
555 988(Display)N
851(*XtDisplay\()X
2 f
1286(w)X
1 f
1345(\))X
687 1084(Widget)N
2 f
969(w)X
1 f
1028(;)X
555 1428(Screen)N
816(*XtScreen\()X
2 f
1216(w)X
1 f
1275(\))X
687 1524(Widget)N
2 f
969(w)X
1 f
1028(;)X
555 1868(Window)N
880(XtWindow\()X
2 f
1300(w)X
1 f
1359(\))X
687 1964(Widget)N
2 f
969(w)X
1 f
1028(;)X
555 2184(They)N
758(all)X
869(take)X
1038(a)X
1099(widget)X
1361(and)X
1510(return)X
1742(the)X
1872(speci\256ed)X
2207(value.)X
555 2308(Several)N
852(window)X
1168(attributes)X
1530(are)X
1670(locally)X
1944(cached)X
2221(in)X
2323(the)X
2464(widget,)X
2759(so)X
2870(they)X
3055(can)X
3209(be)X
3324(set)X
3454(by)X
3574(the)X
3714(resource)X
555 2404(manager)N
893(and)X
1055(by)X
5 f
1196(XtSetValues)X
1 f
1697(,)X
1754(and)X
1916(can)X
2073(be)X
2191(used)X
2387(by)X
2510(routines)X
2828(that)X
2995(derive)X
3249(structures)X
3625(from)X
3830(these)X
555 2500(values)N
804(\(for)X
959(example,)X
1304(depth)X
1524(for)X
1650(deriving)X
1968(pixmaps,)X
2313(background_pixel)X
2973(for)X
3099(deriving)X
3417(GCs,)X
3619(and)X
3770(so)X
3872(on\),)X
555 2596(as)N
650(well)X
824(as)X
919(used)X
1102(in)X
1193(the)X
5 f
1341(XtCreateWindow)X
1 f
2038(call.)X
555 2720(The)N
717(x,)X
808(y,)X
899(width,)X
1147(height,)X
1415(and)X
1567(border_width)X
2066(window)X
2374(attributes)X
2728(are)X
2859(available)X
3202(to)X
3295(geometry)X
3652(managers.)X
555 2816(These)N
794(\256elds)X
1014(are)X
1149(maintained)X
1570(synchronously)X
2111(inside)X
2350(the)X
2486(toolkit.)X
2790(When)X
3028(an)X
5 f
3157(XCon\256gureWindow)X
1 f
3952(is)X
555 2912(issued)N
802(on)X
916(the)X
1050(widget's)X
1379(window,)X
1710(these)X
1917(values)X
2168(are)X
2301(updated)X
2606 0.3187(immediately)AX
3075(rather)X
3306(than)X
3484(sometime)X
3854(later)X
555 3008(when)N
775(the)X
913(server)X
1157(gets)X
1329(around)X
1603(to)X
1702(generating)X
2104(a)X
5 f
2191(Con\256gureNotify)X
1 f
2841(event.)X
3105(\(In)X
3236(fact,)X
3419(most)X
3620(widgets)X
3923(do)X
555 3104(not)N
696(have)X
5 f
908(SubstructureNotify)X
1 f
1676(turned)X
1929(on.\))X
2118(This)X
2302(ensures)X
2592(that)X
2752(all)X
2868(geometry)X
3228(calculations)X
3677(are)X
3811(based)X
555 3200(on)N
665(the)X
795(internally)X
1156(consistent)X
1531(toolkit)X
1785(world,)X
2034(rather)X
2261(than)X
2435(on)X
2545(either)X
2768(of)X
2863(the)X
2993(following:)X
10 f
555 3324(g)N
1 f
775(An)X
904(inconsistent)X
1348(world)X
1575(updated)X
1876(by)X
1986(asynchronous)X
5 f
2509(Con\256gureNotify)X
1 f
3152(events)X
10 f
555 3448(g)N
1 f
775(A)X
866(consistent)X
1247(but)X
1388(slow)X
1582(world)X
1815(in)X
1912(which)X
2155(geometry)X
2516(managers)X
2881(ask)X
3025(the)X
3160(server)X
3401(for)X
3530(window)X
3840(sizes)X
775 3544(whenever)N
1149(they)X
1334(need)X
1533(to)X
1635(layout)X
1889(their)X
2084(managed)X
2435(children)X
2757(See)X
2917(``Geometry)X
3359(Management'')X
3909(for)X
775 3640(further)N
1036(information.)X
3 f
555 3832(9.)N
665(Creating)X
1019(Widgets)X
1 f
555 3956(The)N
714(creation)X
1020(of)X
1115(widget)X
1377(instances)X
1722(is)X
1803(a)X
1864(two-phase)X
2247(process:)X
555 4080(1.)N
775(The)X
947(widgets)X
1256(are)X
1398(allocated)X
1752(and)X
1914 0.3187(initialized)AX
2304(with)X
2496(resources)X
2861(and)X
3022(are)X
3163(optionally)X
3556(added)X
3800(to)X
3903(the)X
775 4176(managed)N
1115(subset)X
1357(of)X
1452(their)X
1636(parent,)X
1900(with)X
2079(whom)X
2321(they)X
2495(negotiate)X
2841(geometry.)X
555 4300(2.)N
775(The)X
934(widgets)X
1230(create)X
1462(X)X
1547(windows)X
1886(and)X
2035(get)X
2165(mapped.)X
555 4424(To)N
681(start)X
861(the)X
997(\256rst)X
1162(phase,)X
1412(the)X
1548(application)X
1969(calls)X
5 f
2176(XtCreateWidget)X
1 f
2840(on)X
2955(all)X
3071(its)X
3182(widgets)X
3483(and)X
3637(adds)X
3825(some)X
555 4520(\(usually,)N
885(most)X
1080(or)X
1176(all\))X
1317(of)X
1413(its)X
1520(widgets)X
1817(to)X
1909(their)X
2094(respective)X
2474(parents')X
2780(managed)X
3121(set.)X
3286(Adding)X
3573(a)X
3635(child)X
3835(to)X
3927(its)X
555 4616(parent's)N
870(managed)X
1219(set)X
1348(involves)X
1678(geometry)X
2042(negotiation.)X
2493(That)X
2686(is,)X
2798(the)X
2937(parent)X
3188(may)X
3371(force)X
3582(some)X
3799(of)X
3903(the)X
555 4712(child's)N
828(siblings)X
1136(to)X
1238(change)X
1520(size)X
1690(and)X
1850(position,)X
2190(and)X
2350(the)X
2491(parent)X
2744(may)X
2929(issue)X
3138(geometry)X
3504(requests)X
3825(to)X
3927(its)X
555 4808(own)N
734(parent)X
982(in)X
1079(order)X
1292(to)X
1389(better)X
1618(accommodate)X
2136(all)X
2253(its)X
2365(children.)X
2726(You)X
2905(do)X
3021(not)X
3162(really)X
3391(know)X
3614(where)X
3855(any-)X
555 4904(thing)N
776(should)X
1050(go)X
1177(on)X
1304(the)X
1451(screen)X
1714(until)X
1916(all)X
2044(widgets)X
2357(have)X
2561(been)X
2765(created)X
3057(and)X
3222(are)X
3367(managed)X
3723(by)X
3849(their)X
555 5000(respective)N
941(parents.)X
1268(Consequently,)X
1804(in)X
1902(the)X
2039(\256rst)X
2205(phase,)X
2456(no)X
2572(X)X
2663(windows)X
3008(are)X
3143(actually)X
3451(created)X
3733(because)X
555 5096(chances)N
855(are)X
984(they)X
1158(would)X
1400(just)X
1550(get)X
1680(moved)X
1942(around)X
2208(after)X
2391(creation,)X
2719(which)X
2956(is)X
3037(slow.)X
555 5220(After)N
777(all)X
903(widgets)X
1214(have)X
1417(been)X
1620(created,)X
1933(the)X
2078(application)X
2508(calls)X
5 f
2725(XtRealizeWidget)X
1 f
3427(on)X
3552(the)X
3697(top-level)X
555 5316(widget)N
826(to)X
926(start)X
1109(the)X
1248(second)X
1523(phase.)X
5 f
1794(XtRealizeWidget)X
1 f
2490(recursively)X
2912(traverses)X
3255(the)X
3394(widget)X
3665(tree,)X
3849(allo-)X
555 5412(cates)N
753(an)X
858(X)X
943(window)X
1248(to)X
1339(each)X
1522(widget,)X
1806(and)X
1955(maps)X
2163(the)X
2293(widgets.)X
3 f
2239 6144(16)N

17 p
%%Page: 17 19
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(9.1.)N
731(XtCreateWidget)X
1 f
555 892(Widget)N
837(XtCreateWidget\()X
2 f
1444(name)X
1 f
1634(,)X
2 f
1678(widget_class)X
1 f
2134(,)X
2 f
2178(parent)X
1 f
2408(,)X
2 f
2452(args)X
1 f
2608(,)X
2 f
2652(num_args)X
1 f
3003(\))X
687 988(String)N
2 f
925(name)X
1 f
1115(;)X
687 1084(WidgetClass)N
2 f
1160(widget_class)X
1 f
1616(;)X
687 1180(Widget)N
2 f
969(parent)X
1 f
1199(;)X
687 1276(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 1372(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
2 f
555 1620(name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(text)X
1756(name)X
1969(for)X
2093(the)X
2223(created)X
2499(widget.)X
2 f
555 1744(widget_class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(class)X
2056(pointer)X
2328(for)X
2452(the)X
2582(created)X
2858(widget.)X
2 f
555 1868(parent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(parent)X
1843(widget.)X
2 f
555 1992(args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(argument)X
1956(list)X
2087(to)X
2178(override)X
2493(the)X
2623(resource)X
2942(defaults.)X
2 f
555 2116(num_args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arguments)X
2376(in)X
2467(args.)X
5 f
570 2240(XtCreateWidget)N
1 f
1229(performs)X
1568(much)X
1786(of)X
1881(the)X
2011(``boiler-plate'')X
2556(operations)X
2945(of)X
3040(widget)X
3302(creation.)X
3652(It)X
10 f
555 2364(g)N
1 f
775(Checks)X
1056(that)X
1211(the)X
1341(parent)X
1583(is)X
1664(a)X
1725(subclass)X
2040(of)X
5 f
2153(compositeWidgetClass)X
1 f
10 f
555 2488(g)N
1 f
775(Allocates)X
1130(memory)X
1446(for)X
1570(the)X
1700(widget)X
1962(instance)X
10 f
555 2612(g)N
1 f
775(Initializes)X
1146(the)X
1276(core)X
1449(non-resource)X
1929(data)X
2098(\256elds)X
2311(\(for)X
2464(example,)X
2807(parent)X
3049(and)X
3198(visible\))X
10 f
555 2736(g)N
1 f
775(Initializes)X
1156(the)X
1296(resource)X
1625(\256elds)X
1848(\(for)X
2011(example,)X
2364(background_pixel\))X
3060(by)X
3179(using)X
3401(the)X
3540(resource)X
3868(lists)X
775 2832(speci\256ed)N
1110(for)X
1234(this)X
1384(class)X
1577(and)X
1726(all)X
1837(superclasses)X
10 f
555 2956(g)N
1 f
775(Calls)X
990(the)X
1131 0.3542(initialize)AX
1475(procedures)X
1893(for)X
2027(the)X
2167(widget,)X
2461(starting)X
2758(at)X
2854(the)X
2994(Core)X
3197 0.3542(initialize)AX
3540(procedure)X
3923(on)X
775 3052(down)N
992(to)X
1083(the)X
1213(widget's)X
1538 0.3542(initialize)AX
1871(procedure.)X
10 f
555 3176(g)N
1 f
775(Puts)X
949(the)X
1079(widget)X
1341(into)X
1501(its)X
1607(parent's)X
1912(children)X
2223(list)X
2354(by)X
2464(calling)X
2727(its)X
2833(parent's)X
3138(insert_child)X
3577(procedure)X
555 3300(\(See)N
733(``Adding)X
1077(New)X
1264(Children:)X
1620(the)X
1750(insert_child)X
2189(procedure''.)X
3 f
555 3492(9.1.1.)N
797(Widget)X
1093(Initialization:)X
1630(the)X
1769(initialize)X
2116(procedure)X
1 f
555 3616(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtInitProc)X
1 f
1466(\))X
1517(\(\);)X
687 3712(Widget)N
2 f
969(request)X
1 f
1228(;)X
687 3808(Widget)N
2 f
969(new)X
1 f
1111(;)X
687 3904(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 4000(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
555 4124(The)N
717(main)X
919(job)X
1057(of)X
1155(an)X
1263 0.3317(initialization)AX
1737(procedure)X
2112(is)X
2195(to)X
2288(\256ll)X
2411(in)X
2504(\256elds)X
2719(in)X
2812(the)X
2944(new)X
3114(widget)X
3378(record.)X
3670(These)X
3904(are)X
555 4220(generally)N
927(nonresource)X
1399(\256elds)X
1633(\(for)X
1807(example,)X
2171(GCs)X
2370(that)X
2546(the)X
2697(widget)X
2980(uses\))X
3203(or)X
3319(resource)X
3659(\256elds)X
3893(left)X
555 4316(unspeci\256ed)N
989(\(for)X
1153(example,)X
1507(if)X
1594(width)X
1828(and)X
1988(height)X
2242(are)X
2381(zero)X
2564(\(0\),)X
2720(the)X
2860(widget)X
3132(\256lls)X
3297(in)X
3398(width)X
3631(and)X
3790(height)X
555 4412(with)N
734(appropriate)X
1157(values)X
1404(based)X
1626(on)X
1736(other)X
1939(resources\).)X
555 4536 0.2644(Initialization)AN
1038(procedures)X
1453(are)X
1590(called)X
1831(in)X
1929(``superclass-to-subclass)X
2797(order''.)X
3113(Most)X
3323(of)X
3425(the)X
3562 0.3317(initialization)AX
555 4632(code)N
749(for)X
879(a)X
945(speci\256c)X
1241(widget)X
1508(class)X
1706(deals)X
1914(with)X
2098(\256elds)X
2316(de\256ned)X
2602(in)X
2698(that)X
2858(class)X
3056(and)X
3210(not)X
3350(with)X
3534(\256elds)X
3752(de\256ned)X
555 4728(in)N
646(its)X
752(superclasses.)X
555 4852(However,)N
926(sometimes)X
1332(a)X
1398(subclass)X
1718(may)X
1897(want)X
2095(to)X
2191(overwrite)X
2555(values)X
2807(\256lled)X
3016(in)X
3112(by)X
3227(its)X
3338(superclass.)X
3770(In)X
3870(par-)X
555 4948(ticular,)N
834(size)X
1002(calculations)X
1455(of)X
1559(a)X
1629(superclass)X
2021(are)X
2158(often)X
2369(incorrect)X
2712(for)X
2844(a)X
2913(subclass.)X
3280(In)X
3383(this)X
3541(case)X
3722(the)X
3860(sub-)X
555 5044(class)N
755(must)X
956(modify)X
1240(or)X
1342(recalculate)X
1753(\256elds)X
1973(declared)X
2300(and)X
2456(computed)X
2833(by)X
2950(its)X
3062(superclass.)X
3473(For)X
3623(example,)X
3972(a)X
555 5140(subclass)N
870(may)X
1044(visually)X
1346(surround)X
1680(its)X
1786(superclass)X
2169(display.)X
2468(In)X
2563(this)X
2713(case,)X
2908(the)X
3038(width)X
3261(and)X
3410(height)X
3653(calculated)X
555 5236(by)N
669(the)X
803(superclass)X
1190 0.3542(initialize)AX
1527(procedure)X
1904(are)X
2036(too)X
2174(small)X
2391(and)X
2543(need)X
2734(to)X
2828(be)X
2936(incremented)X
3397(by)X
3510(the)X
3643(size)X
3805(of)X
3903(the)X
555 5332(surround.)N
555 5456(The)N
733(subclass)X
1067(needs)X
1308(to)X
1417(know)X
1652(if)X
1746(its)X
1870(superclass's)X
2334(size)X
2511(was)X
2687(calculated)X
3085(by)X
3213(the)X
3361(superclass)X
3762(or)X
3875(was)X
555 5552(speci\256ed)N
896(explicitly.)X
1303(A)X
1394(typical)X
1663(widget)X
1931(should)X
2194(place)X
2408(itself)X
2613(into)X
2779(whatever)X
3129(size)X
3294(is)X
3381(speci\256ed)X
3722(\(if)X
3833(any\),)X
555 5648(but)N
693(it)X
768(computes)X
1131(a)X
1195(reasonable)X
1596(size)X
1758(for)X
1885(itself)X
2086(if)X
2164(no)X
2276(size)X
2437(is)X
2520(requested.)X
2925(How)X
3119(does)X
3304(a)X
3367(subclass)X
3684(know)X
3903(the)X
555 5744(difference)N
933(between)X
1248(a)X
1309(speci\256ed)X
1644(size,)X
1825(and)X
1974(a)X
2035(size)X
2194(computed)X
2564(by)X
2674(a)X
2735(superclass?)X
3 f
2239 6144(17)N

18 p
%%Page: 18 20
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
725(request)X
1012(and)X
1172(new)X
1351(parameters)X
1770(provide)X
2072(the)X
2213(necessary)X
2587(information.)X
3081(The)X
3251(request)X
3538(widget)X
3811(is)X
3903(the)X
555 768(widget)N
828(as)X
934(originally)X
1311(requested.)X
1725(The)X
1895(``new'')X
2190(widget)X
2462(starts)X
2680(with)X
2869(the)X
3009(values)X
3266(in)X
3367(the)X
3507(request)X
3793(and)X
3952(is)X
555 864(updated)N
870(by)X
994(all)X
1118(superclasses)X
1587(so)X
1700(far.)X
1876(A)X
1974(subclass)X
2302 0.3542(initialize)AX
2648(procedure)X
3034(may)X
3221(compare)X
3559(these)X
3775(two)X
3942(to)X
555 960(resolve)N
838(any)X
994(potential)X
1333(con\257icts.)X
1705(In)X
1807(the)X
1944(above)X
2183(example,)X
2532(the)X
2668(subclass)X
2989(with)X
3174(the)X
3310(visual)X
3549(surround)X
3889(can)X
555 1056(see)N
696(if)X
779(the)X
916(width)X
1146(and)X
1302(height)X
1552(in)X
1650(the)X
1787(request)X
2070(widget)X
2339(are)X
2475(zero.)X
2699(If)X
2786(this)X
2943(is)X
3031(so,)X
3160(it)X
3239(just)X
3396(adds)X
3586(its)X
3699(surround)X
555 1152(size)N
720(to)X
817(the)X
953(width)X
1182(and)X
1336(height)X
1584(\256elds)X
1802(in)X
1898(the)X
2033(new)X
2206(widget.)X
2517(If)X
2602(this)X
2757(is)X
2843(not)X
2983(so,)X
3110(it)X
3187(must)X
3386(make)X
3604(do)X
3719(with)X
3903(the)X
555 1248(size)N
728(originally)X
1108(speci\256ed.)X
1501(The)X
1674(``new'')X
1972(widget)X
2248(will)X
2422(become)X
2732(the)X
2876(instance)X
3200(record.)X
3481(Therefore,)X
3884(any)X
555 1344(callback)N
874(lists)X
1042(or)X
1140(similar)X
1411(elements)X
1750(that)X
1908(the)X
2041 0.3317(initialization)AX
2515(procedure)X
2890(registers)X
3212(should)X
3471(specify)X
3749(``new'')X
555 1440(as)N
650(the)X
780(widget)X
1042(instance.)X
555 1564(The)N
718(arguments)X
1111(speci\256ed)X
1450(by)X
1564(args)X
1736(and)X
1889(num_args)X
2262(in)X
2357(the)X
5 f
2509(XtCreateWidget)X
1 f
3171(call)X
3324(are)X
3456(passed)X
3715(in)X
3809(to)X
3903(the)X
555 1660 0.3542(initialize)AN
897(procedure.)X
1322(These)X
1562(are)X
1699(are)X
1836(ignored)X
2135(by)X
2253(most)X
2455(widgets.)X
2781(However,)X
3154(these)X
3365(are)X
3502(used)X
3693(by)X
3811(a)X
3880(few)X
555 1756(widgets)N
866(that)X
1036(have)X
1239(subparts)X
1569(that)X
1739(are)X
1883(not)X
2032(widgets)X
2342(but,)X
2513(nonetheless,)X
2982(have)X
3184(resources)X
3551(that)X
3720(must)X
3928(be)X
555 1852(\256lled)N
759(in.)X
872(See)X
1021(``XtGetSubresources'')X
1842(for)X
1966(further)X
2227(information.)X
3 f
555 2044(9.2.)N
731(XtRealizeWidget)X
1 f
555 2264(void)N
734(XtRealizeWidget\()X
2 f
1376(w)X
1 f
1435(\))X
687 2360(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 2608(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
555 2732(If)N
635(the)X
765(widget)X
1027(is)X
1108(already)X
1389(realized,)X
5 f
1730(XtRealizeWidget)X
1 f
2417(simply)X
2680(returns.)X
555 2856(Otherwise,)N
5 f
997(XtRealizeWidget)X
1 f
1703(constructs)X
2101(an)X
5 f
2243(XSetWindowAttributes)X
1 f
3175(structure)X
3523(\256lled)X
3745(in)X
3854(with)X
555 2952(information)N
1004(derived)X
1300(from)X
1503(the)X
1643(Core)X
1846(widget)X
2117(\256elds)X
2339(and)X
2497(calls)X
2690(the)X
2829(realize)X
3095(procedure)X
3477(for)X
3610(the)X
3749(widget,)X
555 3048(which)N
796(adds)X
983(any)X
1136(widget-speci\256c)X
1700(attributes)X
2055(and)X
2208(creates)X
2478(the)X
2612(X)X
2701(window.)X
3054(If)X
3138(the)X
3272(widget)X
3538(is)X
3622(a)X
3686(primitive)X
555 3144(widget,)N
875(nothing)X
1203(else)X
1398(need)X
1622(be)X
1763(done,)X
2013(and)X
5 f
2215(XtRealizeWidget)X
1 f
2937(returns.)X
3282(Otherwise)X
5 f
3718(XtReal-)X
555 3240(izeWidget)N
1 f
986(recursively)X
1407(calls)X
1599(itself)X
1806(on)X
1924(each)X
2115(of)X
2218(the)X
2356(widget's)X
2689(managed)X
3036(children.)X
3398(Finally,)X
5 f
3718(XtReal-)X
555 3336(izeWidget)N
1 f
979(maps)X
1188(all)X
1300(of)X
1396(the)X
1527(managed)X
1868(children)X
2180(windows)X
2519(that)X
2674(have)X
2862(mapped_when_managed)X
3759(TRUE.)X
555 3432(If)N
641(num_children)X
1159(equals)X
1412(num_mapped_children,)X
2274(it)X
2351(calls)X
5 f
2558(XMapSubwindows)X
1 f
3323(to)X
3419(map)X
3598(all)X
3714(the)X
3849(chil-)X
555 3528(dren)N
744(at)X
841(once.)X
1062(Otherwise,)X
1478(it)X
1561(maps)X
1780(each)X
1974(child)X
2183(individually.)X
2687(If)X
2777(the)X
2917(widget)X
3189(is)X
3280(the)X
3420(special)X
3697(top-level)X
555 3624(widget)N
817(\(that)X
1001(is,)X
1104(its)X
1210(parent)X
1452(is)X
1533(NULL\),)X
5 f
1858(XtRealizeWidget)X
1 f
2545(maps)X
2753(the)X
2883(widget)X
3145(window.)X
5 f
570 3748(XtCreateWidget)N
1 f
1207(,)X
5 f
1318(XtRealizeWidget)X
1 f
1983(,)X
5 f
2094(XtAdd/RemoveChildren)X
1 f
3028(,)X
3120(and)X
5 f
3335(XtDestroyWidget)X
1 f
555 3844(maintain)N
887(the)X
1017(following)X
1382(invariants:)X
10 f
555 3968(g)N
1 f
775(If)X
855(w)X
940(is)X
1021(realized,)X
1344(then)X
1518(all)X
1629(managed)X
1969(children)X
2280(of)X
2375(w)X
2460(are)X
2589(realized.)X
10 f
555 4092(g)N
1 f
775(If)X
863(w)X
955(is)X
1043(realized,)X
1373(then)X
1554(all)X
1672(managed)X
2019(children)X
2337(of)X
2439(w)X
2531(that)X
2693(are)X
2829(also)X
3000(mapped_when_managed)X
3904(are)X
775 4188(mapped.)N
555 4312(All)N
692(intrinsic)X
1006(routines)X
1314(and)X
1465(all)X
1578(widget)X
1842(routines)X
2150(should)X
2409(work)X
2613(with)X
2794(either)X
3018(realized)X
3320(or)X
3416(unrealized)X
3806(widg-)X
555 4408(ets.)N
3 f
555 4600(9.3.)N
731(Window)X
1071(Creation:)X
1454(the)X
1593(realize)X
1865(procedure)X
1 f
555 4820(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtRealizeProc)X
1 f
1603(\)\(\);)X
687 4916(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 5012(XtValueMask)N
2 f
1202(value_mask)X
1 f
1617(;)X
687 5108(XSetWindowAttributes)N
1541(*)X
2 f
(attributes)S
1 f
1924(;)X
2 f
555 5356(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 5480(value_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(\256elds)X
1921(in)X
2012(attributes)X
2363(to)X
2454(use.)X
2 f
555 5604(attributes)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(attributes)X
2257(to)X
2348(use)X
2487(in)X
2578(the)X
2708(XCreateWindows)X
3360(call.)X
555 5728(The)N
714(realize)X
971(procedure)X
1344(must)X
1538(make)X
1751(the)X
1881(window)X
2186(a)X
2247(reality.)X
3 f
2239 6144(18)N

19 p
%%Page: 19 21
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
719(generic)X
1005(procedure)X
5 f
1401(XtRealizeWidget)X
1 f
2093(\256lls)X
2253(in)X
2349(a)X
2415(mask)X
2628(and)X
2782(a)X
2847(corresponding)X
5 f
3394(XSetWindowAt-)X
555 768(tributes)N
1 f
890(structure.)X
1273(It)X
1358(sets)X
1521(the)X
1660(following)X
2034(\256elds)X
2256(based)X
2486(on)X
2604(information)X
3051(in)X
3150(the)X
3288(widget)X
5 f
3576(Core)X
1 f
3811(struc-)X
555 864(ture:)N
10 f
555 988(g)N
1 f
775(background_pixmap)X
1540(\(or)X
1683(background_pixel)X
2360(if)X
2455(background_pixmap)X
3219(is)X
3318(NULL\))X
3621(is)X
3720(\256lled)X
3942(in)X
775 1084(from)N
968(the)X
1098(corresponding)X
1623(\256eld.)X
10 f
555 1208(g)N
1 f
775(border_pixmap)X
1372(\(or)X
1533(border_pixel)X
2042(if)X
2155(border_pixmap)X
2752(is)X
2869(NULL\))X
3190(is)X
3307(\256lled)X
3547(in)X
3674(from)X
3903(the)X
775 1304(corresponding)N
1300(\256eld.)X
10 f
555 1428(g)N
1 f
775(event_mask)X
1242(is)X
1347(\256lled)X
1575(in)X
1690(based)X
1936(on)X
2069(the)X
2222(event)X
2458(handlers)X
2801(registered,)X
3215(the)X
3368(event)X
3604(translations)X
775 1524(speci\256ed,)N
1132(whether)X
1437(expose)X
1703(is)X
1784(non-NULL,)X
2223(and)X
2372(whether)X
2677(visible_interest)X
3239(is)X
3320(TRUE.)X
10 f
555 1648(g)N
1 f
775(do_not_propagate_mask)X
1668(is)X
1754(set)X
1879(to)X
1975(propagate)X
2349(all)X
2465(mouse)X
2722(and)X
2876(keyboard)X
3229(events)X
3480(up)X
3594(the)X
3728(window)X
775 1744(tree.)N
978(A)X
1068(composite)X
1458(widget)X
1725(can)X
1874(implement)X
2280(functionality)X
2759(caused)X
3025(by)X
3140(event)X
3357(anywhere)X
3724(inside)X
3961(it)X
775 1840(\(including)N
1168(on)X
1286(top)X
1429(of)X
1532(children)X
1851(widgets\))X
2184(as)X
2287(long)X
2474(as)X
2577(children)X
2896(do)X
3014(not)X
3156(specify)X
3439(a)X
3507(translation)X
3909(for)X
775 1936(the)N
905(event.)X
555 2060(All)N
695(other)X
903(\256elds)X
1121(in)X
1217(attributes)X
1573(\(and)X
1756(the)X
1891(corresponding)X
2421(bits)X
2576(in)X
2671(value_mask\))X
3147(can)X
3295(be)X
3404(set)X
3528(by)X
3642(the)X
3776(realize)X
555 2156(procedure.)N
555 2280(A)N
641(widget)X
904(class)X
1098(can)X
1243(inherit)X
1497(its)X
1604(realize)X
1862(procedure)X
2236(from)X
2430(its)X
2536(superclass.)X
2963(The)X
3122(realize)X
3379(procedure)X
3752(de\256ned)X
555 2376(for)N
5 f
698(Core)X
1 f
926(simply)X
1190(calls)X
5 f
1393(XtCreateWindows)X
1 f
2135(with)X
2315(the)X
2446(passed)X
2703(value_mask)X
3147(and)X
3296(attributes,)X
3669(as)X
3764(well)X
3938(as)X
555 2472(with)N
749(windowClass)X
1260(and)X
1424(visual)X
1672(set)X
1806(to)X
5 f
1929(CopyFromParent)X
1 f
2614(.)X
2694(Both)X
5 f
2920(CompositeWidgetClass)X
1 f
3884(and)X
5 f
570 2568(ContraintWidgetClass)N
1 f
1467(inherit)X
1730(this)X
1890(realize)X
2157(procedure,)X
2562(and)X
2721(most)X
2925(new)X
3102(widget)X
3373(subclasses)X
3770(can)X
3923(do)X
555 2664(the)N
685(same.)X
910(See)X
1059(``Inheriting)X
1487(Superclass)X
1885(Operations'')X
2351(for)X
2475(further)X
2736(information.)X
555 2788(The)N
725(most)X
930(common)X
1272(noninherited)X
1751(realize)X
2019(procedures)X
2437(set)X
2567(bit_gravity)X
2987(to)X
3088(the)X
3228(appropriate)X
3661(value)X
3884(and)X
555 2884(then)N
737(create)X
977(the)X
1115(window.)X
1472(For)X
1624(example,)X
1975(Label)X
2206(sets)X
2367(bit_gravity)X
2784(to)X
2882(WestGravity,)X
3383(CenterGravity,)X
3938(or)X
555 2980(EastGravity.)N
1050(Consequently,)X
1587(shrinking)X
1950(a)X
2018(Label)X
2248(just)X
2405(moves)X
2664(the)X
2801(bits)X
2958(appropriately,)X
3479(and)X
3635(no)X
3752(Expose)X
555 3076(event)N
768(is)X
849(needed)X
1120(for)X
1244(repainting.)X
3 f
555 3364(9.3.1.)N
797(XtCreateWindow)X
1 f
555 3488(Rather)N
823(than)X
1008(call)X
1169(the)X
5 f
1328(XCreateWindow)X
1 f
2010(X)X
2105(function)X
2431(directly,)X
2755(a)X
2826(realize)X
3093(procedure)X
3476(should)X
3743(call)X
3903(the)X
555 3584(toolkit)N
817(analog)X
5 f
1100(XtCreateWindow)X
1 f
1775(.)X
1848(This)X
2034(routine)X
2313(simpli\256es)X
2686(the)X
2823(creation)X
3136(of)X
3238(windows)X
3584(for)X
3715(widgets.)X
555 3680(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 3900(void)N
734(XtCreateWindow\()X
2 f
(w)S
1 f
1443(,)X
2 f
1487(window_class)X
1 f
1982(,)X
2 f
2026(visual)X
1 f
2237(,)X
2 f
2281(value_mask)X
1 f
2696(,)X
2 f
2740(attributes)X
1 f
3079(\))X
687 3996(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 4092(unsigned)N
1027(int)X
2 f
1143(window_class)X
1 f
1638(;)X
687 4188(Visual)N
939(*)X
2 f
(visual)S
1 f
1194(;)X
687 4284(XtValueMask)N
2 f
1202(value_mask)X
1 f
1617(;)X
687 4380(XSetWindowAttributes)N
1541(*)X
2 f
(attributes)S
1 f
1924(;)X
2 f
555 4628(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(used)X
2046(to)X
2137(set)X
2257(x,)X
2345(y,)X
2433(and)X
2582(so)X
2682(on)X
2 f
555 4752(window_class)N
1 f
1131(Speci\256es)X
1476(the)X
1610(Xlib)X
1793(window)X
2102(class)X
2299(\(for)X
2456(example,)X
2803(InputOutput,)X
3282(InputOnly,)X
3692(or)X
3791(Copy-)X
1131 4848(FromParent)N
1569(\).)X
2 f
555 4972(visual)N
1 f
1131(Speci\256es)X
1471(the)X
1601(visual)X
1834(type.)X
2 f
555 5096(value_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(\256elds)X
1921(in)X
2012(attributes)X
2363(to)X
2454(use.)X
2 f
555 5220(attributes)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(attributes)X
2257(to)X
2348(use)X
2487(in)X
2578(the)X
5 f
2726(XCreateWindows)X
1 f
3442(call.)X
5 f
570 5344(XtCreateWindow)N
1 f
1269(calls)X
5 f
1473(XCreateWindow)X
1 f
2147(with)X
2328(values)X
2577(from)X
2771(the)X
2902(widget)X
3165(structure)X
3496(and)X
3646(the)X
3777(passed)X
555 5440(parameters,)N
985(as)X
1080(well)X
1254(as)X
1349(assigns)X
1625(the)X
1755(created)X
2031(window)X
2336(into)X
2496(the)X
2626(widget's)X
2951(window)X
3256(\256eld.)X
3 f
2239 6144(19)N

20 p
%%Page: 20 22
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(9.4.)N
731(XtIsRealized)X
1 f
555 796(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 1016(Boolean)N
871(XtIsRealized\()X
2 f
1360(w)X
1 f
1419(\))X
687 1112(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 1360(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
5 f
570 1484(XtIsRealized)N
1 f
1101(returns)X
1367(TRUE)X
1619(if)X
1695(the)X
1825(widget)X
2087(has)X
2226(been)X
2414(realized.)X
2737(That)X
2921(is,)X
3024(if)X
3100(it)X
3172(has)X
3311(a)X
3372(valid)X
3571(X)X
3656(window.)X
3 f
555 1676(10.)N
709(Destroying)X
1141(Widgets)X
1 f
555 1800(Destroying)N
968(widgets)X
1264(is)X
1345(simple.)X
1647(The)X
1806(toolkit)X
2060(provides)X
2385(support)X
2671(to:)X
10 f
555 1924(g)N
1 f
775(Destroy)X
1075(all)X
1186(the)X
1316(children)X
1627(of)X
1722(the)X
1852(widget)X
2114(being)X
2332(destroyed)X
10 f
555 2048(g)N
1 f
775(Remove)X
1091(\(and)X
1269(unmap\))X
1560(the)X
1690(widget)X
1952(from)X
2145(its)X
2251(parent)X
10 f
555 2172(g)N
1 f
775(Call)X
945(procedures)X
1352(that)X
1507(have)X
1695(been)X
1883(registered)X
2252(to)X
2343(trigger)X
2600(when)X
2812(the)X
2942(widget)X
3204(is)X
3285(destroyed)X
10 f
555 2296(g)N
1 f
775(Minimize)X
1141(the)X
1271(number)X
1562(of)X
1657(things)X
1895(a)X
1956(widget)X
2218(has)X
2357(to)X
2448(deallocate)X
2828(when)X
3040(destroyed)X
10 f
555 2420(g)N
1 f
775(Minimize)X
1141(the)X
1271(number)X
1562(of)X
1657(XDestroyWindow)X
2323(calls)X
3 f
555 2612(10.1.)N
775(Adding)X
1076(and)X
1240(Deleting)X
1575(Destroy)X
1889(Callbacks)X
1 f
555 2736(The)N
744(destroy)X
1055(callback)X
1400(uses)X
1602(the)X
1761(mechanism)X
2214(described)X
2602(in)X
2722(``Callbacks''.)X
3259(The)X
3447(callback)X
3792(list)X
3952(is)X
555 2832(identi\256ed)N
918(by)X
1035(XtCallbackKind)X
1642(destroyCallback.)X
2288(To)X
2415(add)X
2571(a)X
2639(destroy)X
2927(callback)X
3250(procedure)X
3629(ClientDes-)X
555 2928(troy)N
739(with)X
937(closure)X
2 f
1232(closure)X
1 f
1532(to)X
1642(Widget)X
1943(w,)X
2069(call)X
5 f
2256(XtAddCallback)X
1 f
2848(.)X
2933(To)X
3072(remove)X
3377(the)X
3526(callback,)X
3883(call)X
5 f
570 3024(XtRemoveCallback)N
1 f
1332(.)X
1398(Both)X
1592(take)X
1761(the)X
1891(following)X
2256(parameter)X
2630(list:)X
555 3148(\()N
2 f
584(w)X
1 f
643(,)X
2 f
687(destroyCallback)X
1 f
1265(,)X
2 f
1309(ClientDestroy)X
1 f
1804(,)X
2 f
1848(closure)X
1 f
2107(\))X
3 f
555 3388(10.2.)N
775(XtDestroyWidget)X
1 f
555 3512(To)N
675(destroy)X
956(a)X
1017(widget)X
1279(instance,)X
1612(use)X
5 f
1769(XtDestroyWidget)X
1 f
2445(.)X
2511(The)X
2670(de\256nition)X
3031(for)X
3155(this)X
3305(function)X
3621(is:)X
555 3732(void)N
734(XtDestroyWidget\()X
2 f
1389(w)X
1 f
1448(\))X
687 3828(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 4076(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
555 4200(This)N
744(routine)X
1026(is)X
1117(the)X
1257(only)X
1446(method)X
1743(of)X
1848(destroying)X
2252(a)X
2323(widget,)X
2617(including)X
2983(widgets)X
3289(that)X
3454(wish)X
3652(to)X
3752(destroy)X
555 4296(themselves.)N
1000(It)X
1085(may)X
1268(be)X
1382(called)X
1624(at)X
1719(any)X
1877(time,)X
2088(including)X
2453(from)X
2655(an)X
2769(application)X
3193(callback)X
3518(routine)X
3799(of)X
3903(the)X
555 4392(widget)N
839(being)X
1079(destroyed.)X
1509(This)X
1709(requires)X
2035(a)X
2117(two-phase)X
2521(destroy)X
2823(process.)X
3173(In)X
3289(phase)X
3532(one,)X
5 f
3742(XtDes-)X
555 4488(troyWidget)N
1 f
1013(performs)X
1352(the)X
1482(following)X
1847(actions:)X
10 f
555 4612(g)N
1 f
775(If)X
855(being_destroyed)X
1459(is)X
1540(TRUE,)X
5 f
1832(XtDestroyWidget)X
1 f
2530(returns)X
2796 0.2898(immediately.)AX
10 f
555 4736(g)N
1 f
775(Removes)X
1132(the)X
1269(widget)X
1538(from)X
1738(its)X
1851(parent's)X
2163(managed)X
2510(set,)X
2659(which)X
2903(in)X
3000(turn)X
3170(causes)X
3427(the)X
3563(widget)X
3831(to)X
3928(be)X
775 4832(unmapped.)N
10 f
555 4956(g)N
1 f
775(Sets)X
946(the)X
1078(being_destroyed)X
1684(\256eld)X
1865(to)X
1958(TRUE)X
2212(and)X
2362(the)X
2493(visible)X
2752(bit)X
2869(to)X
2961(FALSE)X
3253(for)X
3378(the)X
3509(widget)X
3772(and)X
3922(all)X
775 5052(descendants.)N
10 f
555 5176(g)N
1 f
775(Adds)X
991(the)X
1130(widget)X
1401(to)X
1500(a)X
1569(list)X
1708(of)X
1811(widgets)X
2115(\(the)X
2282(destroy)X
2571(list\))X
2739(that)X
2902(should)X
3167(be)X
3280(destroyed)X
3652(when)X
3872(it)X
3952(is)X
775 5272(safe)N
938(to)X
1029(do)X
1139(so.)X
555 5396(Entries)N
827(on)X
937(the)X
1067(destroy)X
1348(list)X
1479(satisfy)X
1731(the)X
1861(invariant:)X
10 f
555 5520(g)N
1 f
775(If)X
872(w1)X
1018(occurs)X
1286(before)X
1549(w2)X
1695(on)X
1822(the)X
1968(destroy)X
2265(list,)X
2434(then)X
2624(there)X
2838(is)X
2935(no)X
3061(ancestor/child)X
3594(relationship)X
775 5616(between)N
1090(the)X
1220(two,)X
1396(or)X
1491(w1)X
1620(is)X
1701(a)X
1762(descendant)X
2175(of)X
2270(w2.)X
555 5740(Phase)N
791(two)X
954(occurs)X
1214(when)X
1435(all)X
1555(procedures)X
1971(that)X
2135(should)X
2401(execute)X
2701(as)X
2805(a)X
2875(result)X
3102(of)X
3206(the)X
3345(current)X
3624(event)X
3845(have)X
555 5836(been)N
749(called)X
988(\(including)X
1379(all)X
1496(procedures)X
1909(registered)X
2284(with)X
2469(the)X
2605(Event)X
2839(and)X
2994(Translation)X
3424(Managers\).)X
3849(That)X
3 f
2239 6144(20)N

21 p
%%Page: 21 23
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(is,)N
658(phase)X
880(two)X
1034(occurs)X
1285(when)X
5 f
1515(XtNextEvent)X
1 f
2043(is)X
2124(called.)X
555 796(In)N
663(phase)X
898(two,)X
5 f
1105(XtDestroyWidget)X
1 f
1816(destroy)X
2110(performs)X
2462(the)X
2605(following)X
2983(actions)X
3268(on)X
3390(each)X
3585(entry)X
3800(in)X
3903(the)X
555 892(destroy)N
836(list:)X
10 f
555 1016(g)N
1 f
775(Calls)X
993(the)X
1137(destroy)X
1432(callbacks)X
1796(registered)X
2179(on)X
2302(the)X
2445(widget)X
2720(\(and)X
2911(all)X
3035(descendants\))X
3524(in)X
3628(post-order.)X
775 1112(That)N
959(is,)X
1062(it)X
1134(calls)X
1318(children)X
1629(callbacks)X
1979(before)X
2225(parent)X
2467(callbacks.)X
10 f
555 1236(g)N
1 f
775(Calls)X
1024(the)X
1198(widget's)X
1567(parent's)X
1916(delete_child)X
2414(procedure.)X
2853(\(See)X
3075(``Deleting)X
3503(Children:)X
3903(the)X
775 1332(delete_child)N
1229(procedure''.\))X
10 f
555 1456(g)N
1 f
775(Calls)X
988(the)X
1127(destroy)X
1417(procedures)X
1833(for)X
1966(the)X
2105(widget)X
2375(\(and)X
2561(all)X
2680(descendants\))X
3164(in)X
3263(post-order.)X
3698(For)X
3850(each)X
775 1552(such)N
962(widget,)X
1250(it)X
1326(calls)X
1514(the)X
1648(destroy)X
1933(procedure)X
2310(for)X
2437(the)X
2570(widget,)X
2879(then)X
3056(calls)X
3243(the)X
3376(destroy)X
3660(procedure)X
775 1648(for)N
899(its)X
1005(superclass,)X
1410(until)X
1595(\256nally)X
1843(it)X
1915(calls)X
2099(the)X
5 f
2247(Core)X
1 f
2474(destroy)X
2755(routine.)X
10 f
555 1772(g)N
1 f
775(Calls)X
5 f
1007(XDestroyWindow)X
1 f
1728(if)X
1814(the)X
1953(widget)X
2224(is)X
2314(realized)X
2624(\(that)X
2817(is,)X
2929(has)X
3077(an)X
3191(X)X
3285(window\))X
3628(Otherwise,)X
775 1868(recursively)N
1193(descends)X
1537(the)X
1672(tree)X
1831(and)X
1985(calls)X
5 f
2192(XDestroyWindow)X
1 f
2908(on)X
3023(the)X
3158(top)X
3298(widget)X
3564(of)X
3663(each)X
3850(real-)X
775 1964(ized)N
944(subtree.)X
1264(The)X
1423(server)X
1659(recursively)X
2072(destroys)X
2387(all)X
2498(descendant)X
2911(windows.)X
3 f
555 2156(10.3.)N
775(Deallocating)X
1267(Dynamic)X
1626(Data:)X
1857(the)X
1996(destroy)X
2296(procedure)X
1 f
555 2280(The)N
723(destroy)X
1012(procedures)X
1427(are)X
1564(called)X
1805(in)X
1904(subclass-to-superclass)X
2715(order.)X
2952(Therefore,)X
3350(a)X
3419(widget's)X
3752(destroy)X
555 2376(procedure)N
940(should)X
1209(only)X
1400(deallocate)X
1792(storage)X
2080(that)X
2247(is)X
2340(speci\256c)X
2643(to)X
2745(the)X
2886(subclass)X
3212(and)X
3372(should)X
3640(not)X
3786(bother)X
555 2472(with)N
738(the)X
872(storage)X
1152(allocated)X
1497(by)X
1611(any)X
1764(of)X
1863(its)X
1973(superclasses.)X
2477(If)X
2561(a)X
2626(widget)X
2892(does)X
3078(not)X
3216(need)X
3407(to)X
3501(deallocate)X
3884(any)X
555 2568(storage,)N
853(the)X
983(destroy)X
1264(procedure)X
1637(entry)X
1840(in)X
1931(its)X
2037(WidgetClass)X
2510(should)X
2767(be)X
2872(NULL.)X
555 2692(Deallocating)N
1028(storage)X
1304(includes,)X
1642(but)X
1777(is)X
1858(not)X
1993(limited)X
2267(to:)X
10 f
555 2816(g)N
1 f
775(Calling)X
5 f
1076(XtFree)X
1 f
1378(on)X
1488(dynamic)X
1814(storage)X
2090(allocated)X
2431(with)X
5 f
2628(XtMalloc)X
1 f
2980(,)X
5 f
3042(XtCalloc)X
1 f
3384(,)X
3428(and)X
3577(so)X
3677(on.)X
10 f
555 2940(g)N
1 f
775(Calling)X
5 f
1076(XtRemoveAllCallbacks)X
1 f
2001(on)X
2111(callback)X
2427(lists.)X
10 f
555 3064(g)N
1 f
775(Calling)X
5 f
1076(XtDestroyPixmap)X
1 f
1793(on)X
1903(pixmaps)X
2224(allocated)X
2565(with)X
5 f
2762(XtGetPixmap)X
1 f
3297(.)X
10 f
555 3188(g)N
1 f
775(Calling)X
5 f
1076(XFreePixmap)X
1 f
1646(on)X
1756(pixmaps)X
2077(created)X
2353(with)X
2532(direct)X
2755(X)X
2840(calls.)X
10 f
555 3312(g)N
1 f
775(Calling)X
5 f
1076(XtDestroyGC)X
1 f
1632(on)X
1742(GCs)X
1920(allocated)X
2261(with)X
5 f
2458(XtGetGC)X
1 f
2832(.)X
10 f
555 3436(g)N
1 f
775(Calling)X
5 f
1076(XFreeGC)X
1 f
1485(on)X
1595(GCs)X
1773(allocated)X
2114(with)X
2293(direct)X
2516(X)X
2601(calls.)X
10 f
555 3560(g)N
1 f
775(Calling)X
5 f
1076(XtRemoveEventHandler)X
1 f
2057(on)X
2167(event)X
2380(handlers)X
2700(added)X
2932(with)X
5 f
3129(XtAddEventHandler)X
1 f
3918(.)X
10 f
555 3684(g)N
1 f
775(Calling)X
5 f
1076(XtRemoveTimeOut)X
1 f
1862(on)X
1972(timers)X
2215(created)X
2491(with)X
5 f
2688(XtAddTimeOut)X
1 f
3282(.)X
3 f
555 3876(11.)N
709(Callbacks)X
1 f
555 4000(Applications)N
1030(and)X
1181(other)X
1386(widgets)X
1683(\(clients\))X
1995(often)X
2199(want)X
2393(to)X
2485(register)X
2772(a)X
2834(procedure)X
3208(with)X
3388(a)X
3450(widget)X
3713(that)X
3869(gets)X
555 4096(called)N
791(under)X
1016(certain)X
1280(conditions.)X
1716(For)X
1862(example,)X
2207(when)X
2421(a)X
2484(widget)X
2748(is)X
2831(destroyed)X
3197(every)X
3416(procedure)X
3791(on)X
3903(the)X
555 4192(widget's)N
880(destroy_callbacks)X
1533(list)X
1664(is)X
1745(called)X
1978(to)X
2069(notify)X
2302(clients)X
2555(of)X
2650(the)X
2780(widget's)X
3105(impending)X
3505(doom.)X
555 4316(Every)N
795(widget)X
1065(has)X
1212(a)X
1281(destroy_callbacks)X
1942(list.)X
2125(Widgets)X
2449(may)X
2631(de\256ne)X
2876(additional)X
3260(callback)X
3584(lists)X
3757(as)X
3859(they)X
555 4412(see)N
690(\256t.)X
831(For)X
976(example)X
1298(the)X
1428(Command)X
1818(widget)X
2080(has)X
2219(a)X
2280(callback)X
2596(list)X
2727(to)X
2818(notify)X
3051(clients)X
3304(when)X
3516(the)X
3646(button)X
3894(has)X
555 4508(been)N
743(activated.)X
3 f
555 4700(11.1.)N
775(Callback)X
1135(Procedure)X
1548(and)X
1712(CallbackList)X
2219(De\256nitions)X
1 f
555 4920(typedef)N
841(Opaque)X
1136(XtCallbackList;)X
1723(/*)X
1814(List)X
1974(of)X
2069(Callback)X
2405(procedures)X
2812(*/)X
555 5064(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtCallbackProc)X
1 f
1662(\)\(\);)X
687 5160(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 5256(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
687 5352(caddr_t)N
2 f
973(call_data)X
1 f
1307(;)X
2 f
555 5600(w)N
1 f
1131(Speci\256es)X
1471(widget)X
1733(for)X
1857(which)X
2094(the)X
2224(callback)X
2540(is)X
2621(registered.)X
2 f
555 5724(closure)N
1 f
1131(Speci\256es)X
1478(the)X
1615(client)X
1841(data)X
2017(that)X
2179(the)X
2316(widget)X
2585(should)X
2849(pass)X
3029(back)X
3224(to)X
3322(the)X
3459(client)X
3685(when)X
3903(the)X
1131 5820(widget)N
1411(executes)X
1754(the)X
1902(client's)X
2202(callback)X
2535(procedure.)X
2969(This)X
3165(is)X
3263(a)X
3341(way)X
3526(for)X
3667(the)X
3814(client)X
3 f
2239 6144(21)N

22 p
%%Page: 22 24
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
1131 672(registering)N
1531(the)X
1662(callback)X
1979(to)X
2071(also)X
2236(register)X
2522(client-speci\256c)X
3039(data:)X
3233(a)X
3294(pointer)X
3566(to)X
3657(additional)X
1131 768(information)N
1573(about)X
1794(the)X
1927(widget,)X
2213(a)X
2276(reason)X
2529(for)X
2655(invoking)X
2993(the)X
3125(callback,)X
3465(and)X
3616(so)X
3718(on.)X
3874(It)X
3952(is)X
1131 864(perfectly)N
1469(normal)X
1744(to)X
1838(have)X
2029(a)X
2093(closure)X
2372(of)X
2470(NULL)X
2729(if)X
2808(all)X
2921(necessary)X
3286(information)X
3727(is)X
3810(in)X
3903(the)X
1131 960(widget.)N
2 f
555 1084(call_data)N
1 f
1131(Speci\256es)X
1481(any)X
1640(callback-speci\256c)X
2264(data)X
2443(the)X
2583(widget)X
2855(wants)X
3092(to)X
3193(pass)X
3376(to)X
3477(the)X
3617(client.)X
3889(For)X
1131 1180(example,)N
1504(when)X
1723(Scrollbar)X
2075(executes)X
2407(its)X
2520(thumbChanged)X
3088(callback)X
3411(list,)X
3571(it)X
3650(passes)X
3903(the)X
1131 1276(address)N
1416(of)X
1511(a)X
1572(struct)X
1790(containing)X
2185(the)X
2315(position)X
2622(and)X
2771(height)X
3014(of)X
3109(the)X
3239(thumb.)X
3 f
555 1468(11.2.)N
775(Registering)X
1227(CallbackKinds)X
1 f
555 1592(Whenever)N
939(a)X
1001(widget)X
1264(contains)X
1581(a)X
1642(callback)X
1958(list)X
2089(for)X
2213(use)X
2352(by)X
2462(clients,)X
2737(it)X
2809(also)X
2973(exports)X
3254(in)X
3345(its)X
3451(public)X
3694(``.h'')X
3898(\256le)X
555 1688(a)N
620(name)X
837(of)X
936(type)X
5 f
1132(XtCallbackKind)X
1 f
1769(to)X
1864(identify)X
2165(the)X
2299(callback)X
2619(list.)X
2798(These)X
3034(names)X
3285(are)X
3418(registered)X
3790(by)X
3903(the)X
555 1784 0.2083(class_initialize)AN
1120(procedure,)X
1532(which)X
1785(calls)X
5 f
2003(XtNewCallbackKind)X
1 f
2827(to)X
2934(get)X
3080(a)X
3157(unique)X
3435(identi\256er)X
3792(of)X
3903(the)X
555 1880(callback.)N
915(The)X
1074(de\256nitions)X
1469(for)X
5 f
1611(XtCallbackKind)X
1 f
2244(and)X
2393(this)X
2543(function)X
2859(are:)X
555 2100(typedef)N
841(Opaque)X
1136(XtCallbackKind;)X
555 2244(XtCallbackKind)N
1155(XtNewCallbackKind\()X
2 f
1927(widget_class)X
1 f
2383(,)X
2 f
2427(offset)X
1 f
2619(\))X
687 2340(WidgetClass)N
2 f
1160(widget_class)X
1 f
1616(;)X
687 2436(Cardinal)N
2 f
1013(offset)X
1 f
1205(;)X
2 f
555 2684(widget_class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(class)X
2056(which)X
2293(exports)X
2574(the)X
2704(callback)X
3020(list.)X
2 f
555 2808(offset)N
1 f
1131(Speci\256es)X
1478(the)X
1615(offset)X
1844(of)X
1946(the)X
2083(callback)X
2406(list)X
2544(in)X
2642(the)X
2778(widget)X
3046(structure.)X
3404(\(See)X
3588(``XtOffset'')X
1131 2904(for)N
1255(further)X
1516(information.\))X
555 3028(Clients)N
829(and)X
979(widgets)X
1276(never)X
1494(access)X
1741(widget)X
2003(callback)X
2319(\256elds)X
2532(directly.)X
2868(Instead,)X
3166(they)X
3340(always)X
3606(identify)X
3903(the)X
555 3124(desired)N
834(callback)X
1153(list)X
1286(using)X
1501(the)X
1633(exported)X
1965(XtCallbackKind)X
2567(variable.)X
2919(All)X
3056(callback)X
3374(manipulation)X
3865(rou-)X
555 3220(tines)N
744(described)X
1103(below)X
1340(check)X
1567(that)X
1722(the)X
1852(requested)X
2211(callback)X
2527(list)X
2658(is)X
2739(indeed)X
2996(implemented)X
3480(by)X
3590(the)X
3720(widget.)X
555 3344(Because)N
875(callback)X
1196(kinds)X
1414(are)X
1548(registered)X
1922(by)X
2037(the)X
2172 0.2083(class_initialize)AX
2725(procedure,)X
3124(clients)X
3381(should)X
3642(not)X
3781(expect)X
555 3440(the)N
685(exported)X
1015(callback)X
1331(identi\256ers)X
1706(to)X
1797(be)X
1902(valid)X
2101(before)X
2347(creating)X
2653(a)X
2714(widget)X
2976(of)X
3071(that)X
3226(class.)X
3 f
555 3632(11.3.)N
775(XtAddCallback)X
1 f
555 3756(To)N
677(add)X
828(a)X
891(callback)X
1209(procedure)X
1584(to)X
1677(a)X
1740(callback)X
2057(list,)X
2211(use)X
5 f
2369(XtAddCallback)X
1 f
2961(.)X
3028(The)X
3188(de\256nition)X
3550(for)X
3675(this)X
3826(func-)X
555 3852(tion)N
715(is:)X
555 4072(void)N
734(XtAddCallback\()X
2 f
1316(w)X
1 f
1375(,)X
2 f
1419(callback_kind,)X
1958(callback)X
1 f
2257(,)X
2 f
2301(closure)X
1 f
2560(\))X
687 4168(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 4264(XtCallbackKind)N
2 f
1287(callback_kind)X
1 f
1782(;)X
687 4360(XtCallbackProc)N
2 f
1272(callback)X
1 f
1571(;)X
687 4456(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
2 f
555 4704(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(add)X
2103(the)X
2233(callback)X
2549(to.)X
2 f
555 4828(callback_kind)N
1 f
1131(Speci\256es)X
1471(the)X
1601(list)X
1732(of)X
1827(callbacks)X
2177(within)X
2425(the)X
2555(widget)X
2817(to)X
2908(append)X
3184(to.)X
2 f
555 4952(callback)N
1 f
1131(Speci\256es)X
1471(the)X
1601(callback)X
1917(procedure)X
2290(to)X
2381(add.)X
2 f
555 5076(closure)N
1 f
1131(Speci\256es)X
1477(the)X
1613(closure)X
1895(to)X
1992(be)X
2103(passed)X
2365(to)X
2462(the)X
2598(callback)X
2920(when)X
3138(it)X
3216(is)X
3303(invoked)X
3615(by)X
5 f
3748(XtCall-)X
1131 5172(Callbacks)N
1 f
1548(\(which)X
1814(may)X
1988(often)X
2191(be)X
2296(NULL\).)X
3 f
555 5364(11.4.)N
775(XtRemoveCallback)X
1 f
555 5488(To)N
683(remove)X
977(a)X
1046(callback)X
1370(procedure)X
1751(from)X
1952(a)X
2021(callback)X
2345(list,)X
2506(use)X
5 f
2671(XtRemoveCallback)X
1 f
3433(.)X
3506(The)X
3672(de\256nition)X
555 5584(for)N
679(this)X
829(function)X
1145(is:)X
3 f
2239 6144(22)N

23 p
%%Page: 23 25
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(void)N
734(XtRemoveCallback\()X
2 f
1459(w)X
1 f
1518(,)X
2 f
1562(callback_kind)X
1 f
2057(,)X
2 f
2101(callback)X
1 f
2400(,)X
2 f
2444(closure)X
1 f
2703(\))X
687 864(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 960(XtCallbackKind)N
2 f
1287(callback_kind)X
1 f
1782(;)X
687 1056(XtCallbackProc)N
2 f
1272(callback)X
1 f
1571(;)X
687 1152(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
2 f
555 1400(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(delete)X
2187(the)X
2317(callback)X
2633(from.)X
2 f
555 1524(callback_kind)N
1 f
1131(Speci\256es)X
1471(the)X
1601(list)X
1732(of)X
1827(callbacks)X
2177(within)X
2425(the)X
2555(widget)X
2817(to)X
2908(remove)X
3194(the)X
3324(callback)X
3640(from.)X
2 f
555 1648(callback)N
1 f
1131(Speci\256es)X
1471(the)X
1601(callback)X
1917(procedure)X
2290(to)X
2381(delete.)X
2 f
555 1772(closure)N
1 f
1131(Speci\256es)X
1503(the)X
1665(closure)X
1973(to)X
2096(match)X
2366(on)X
2508(the)X
2670(registered)X
3071(callback)X
3419(procedure.)X
3845(\(The)X
5 f
1146 1868(XtRemoveCallback)N
1 f
1935(routine)X
2212(removes)X
2537(a)X
2603(callback)X
2924(only)X
3108(if)X
3189(both)X
3373(the)X
3507(procedure)X
3884(and)X
1131 1964(the)N
1261(closure)X
1537(match\).)X
3 f
555 2156(11.5.)N
775(XtRemoveAllCallbacks)X
1 f
555 2280(To)N
678(remove)X
967(all)X
1081(callback)X
1400(procedures)X
1810(from)X
2006(a)X
2070(callback)X
2389(list)X
2522(\(and,)X
2724(thus,)X
2917(free)X
3077(all)X
3190(storage)X
3468(associated)X
3854(with)X
555 2376(the)N
685(callback)X
1001(list\),)X
1183(use)X
5 f
1340(XtRemoveAllCallbacks)X
1 f
2243(.)X
2309(The)X
2468(de\256nition)X
2829(for)X
2953(this)X
3103(function)X
3419(is:)X
555 2596(void)N
734(XtRemoveAllCallbacks\()X
2 f
1606(w)X
1 f
1665(,)X
2 f
1709(callback_kind)X
1 f
2204(\))X
687 2692(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 2788(XtCallbackKind)N
2 f
1287(callback_kind)X
1 f
1782(;)X
2 f
555 3036(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(remove)X
2240(the)X
2370(callback)X
2686(from.)X
2 f
555 3160(callbacks)N
1 f
1131(Speci\256es)X
1471(the)X
1601(callback)X
1917(list)X
2048(within)X
2296(the)X
2426(widget)X
2688(to)X
2779(remove.)X
3 f
555 3352(11.6.)N
775(XtCallCallbacks)X
1 f
555 3476(To)N
683(execute)X
982(the)X
1119(procedures)X
1533(in)X
1631(a)X
1699(callback)X
2022(list,)X
2182(use)X
5 f
2346(XtCallCallback)X
1 f
2931(.)X
3004(The)X
3170(de\256nition)X
3538(for)X
3669(this)X
3826(func-)X
555 3572(tion)N
715(is:)X
555 3792(void)N
734(XtCallCallbacks\()X
2 f
1347(w)X
1 f
1406(,)X
2 f
1450(callback_kind)X
1 f
1945(,)X
2 f
1989(call_data)X
1 f
2323(\))X
687 3888(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3984(XtCallbackKind)N
2 f
1287(callback_kind)X
1 f
1782(;)X
687 4080(caddr_t)N
2 f
973(call_data)X
1 f
1307(;)X
2 f
555 4328(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(execute)X
2245(the)X
2375(callbacks)X
2725(of.)X
2 f
555 4452(callback_kind)N
1 f
1131(Speci\256es)X
1471(the)X
1601(list)X
1732(of)X
1827(callbacks)X
2177(within)X
2425(the)X
2555(widget)X
2817(to)X
2908(execute.)X
2 f
555 4576(call_data)N
1 f
1131(Speci\256es)X
1481(a)X
1552(callback-list)X
2016(speci\256c)X
2317(data)X
2496(value)X
2719(to)X
2820(pass)X
3003(to)X
3104(each)X
3297(of)X
3401(the)X
3540(callback)X
3865(pro-)X
1131 4672(cedure)N
1393(in)X
1490(the)X
1626(list.)X
1807(The)X
1972(call_data)X
2318(is)X
2404(NULL)X
2665(if)X
2746(no)X
2861(data)X
3035(is)X
3121(needed)X
3397(\(for)X
3555(example,)X
3903(the)X
1131 4768(commandActivated)N
1846(callback)X
2164(list)X
2297(in)X
2390(Command)X
2782(needs)X
3006(only)X
3187(to)X
3280(notify)X
3515(its)X
3623(clients)X
3878(that)X
1131 4864(the)N
1263(button)X
1513(has)X
1654(been)X
1844(activated\);)X
2241(the)X
2373(call_data)X
2716(is)X
2799(the)X
2931(actual)X
3165(data)X
3335(if)X
3412(only)X
3592(one)X
3742(\(32-bit\))X
1131 4960(word)N
1338(is)X
1424(needed;)X
1725(the)X
1860(call_data)X
2206(is)X
2292(the)X
2427(address)X
2717(of)X
2817(the)X
2952(data)X
3126(if)X
3207(more)X
3415(than)X
3593(one)X
3746(word)X
3952(is)X
1131 5056(needed)N
1407(\(for)X
1565(example,)X
1913(Scrollbar)X
2263(needs)X
2490(to)X
2586(pass)X
2764(back)X
2957(the)X
3092(height)X
3340(and)X
3493(position)X
3804(of)X
3903(the)X
1131 5152(scroll)N
1349(thumb\).)X
3 f
555 5344(12.)N
709(Mapping)X
1074(Strings)X
1365(to)X
1460(Widget)X
1756(Instances)X
2129(and)X
2293(Classes)X
555 5536(12.1.)N
775(XtStringToWidget)X
1 f
5 f
570 5660(XtStringToWidget)N
1 f
1303(maps)X
1516(a)X
1582(fully)X
1776(quali\256ed)X
2112(widget)X
2379(name)X
2596(to)X
2691(a)X
2756(widget)X
3022(instance.)X
3381(The)X
3544(de\256nition)X
3909(for)X
555 5756(this)N
705(function)X
1021(is:)X
3 f
2239 6144(23)N

24 p
%%Page: 24 26
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(Widget)N
837(XtStringToWidget\()X
2 f
1528(name)X
1 f
1718(\);)X
687 864(String)N
2 f
925(name)X
1 f
1115(;)X
2 f
555 1112(name)N
1 f
1131(The)X
1290(fully)X
1479(quali\256ed)X
1810(name)X
2023(of)X
2118(the)X
2248(desired)X
2524(widget.)X
2485 1288(Note)N
1351 1424(This)N
1530(routine)X
1802(currently)X
2142(is)X
2223(not)X
2358(yet)X
2488(implemented.)X
3 f
555 1696(12.2.)N
775(XtStringToClass)X
1 f
5 f
570 1820(XtStringToClass)N
1 f
1249(maps)X
1463(a)X
1530(fully)X
1725(quali\256ed)X
2062(widget)X
2330(name)X
2549(to)X
2646(a)X
2713(widget)X
2981(class.)X
3224(The)X
3388(de\256nition)X
3754(for)X
3883(this)X
555 1916(function)N
871(is:)X
555 2136(WidgetClass)N
1028(XtStringToClass\()X
2 f
1650(name)X
1 f
1840(\);)X
687 2232(String)N
2 f
925(name)X
1 f
1115(;)X
2 f
555 2480(name)N
1 f
1131(The)X
1290(name)X
1503(of)X
1598(the)X
1728(desired)X
2004(widget)X
2266(class.)X
2485 2656(Note)N
1351 2792(This)N
1530(routine)X
1802(currently)X
2142(is)X
2223(not)X
2358(yet)X
2488(implemented.)X
3 f
555 3064(13.)N
709(Composite)X
1131(Widgets)X
1 f
555 3188(Composite)N
970(widgets)X
1276(can)X
1430(have)X
1628(children.)X
1993(Consequently,)X
2532(they)X
2716(are)X
2855(responsible)X
3287(for)X
3420(much)X
3647(more)X
3859(than)X
555 3284(primitive)N
921(widgets.)X
1280(Their)X
1512(responsibilities)X
2088(\(either)X
2359(implemented)X
2861(directly)X
3171(by)X
3299(the)X
3447(widget)X
3727(class)X
3938(or)X
555 3380(indirectly)N
916(by)X
1026(generic)X
1307(procedures\))X
1743(include:)X
10 f
555 3504(g)N
1 f
775(Overall)X
1061(management)X
1534(of)X
1629(children)X
1940(from)X
2133(creation)X
2439(to)X
2530(destruction)X
10 f
555 3628(g)N
1 f
775(Destruction)X
1208(of)X
1303(descendants)X
1750(when)X
1962(the)X
2092(composite)X
2477(widget)X
2739(is)X
2820(destroyed)X
10 f
555 3752(g)N
1 f
775(Physical)X
1103(arrangement)X
1572(\(geometry)X
1963(management\))X
2472(of)X
2574(a)X
2642(displayable)X
3073(subset)X
3322(of)X
3423(children)X
3740(\(that)X
3930(is,)X
775 3848(the)N
905(``managed'')X
1361(children\))X
10 f
555 3972(g)N
1 f
775(Mapping)X
1115(and)X
1264(unmapping)X
1683(of)X
1778(a)X
1839(subset)X
2081(of)X
2176(the)X
2306(managed)X
2646(children)X
10 f
555 4096(g)N
1 f
775(Focus)X
1007(management)X
1480(for)X
1604(the)X
1734(displayable)X
2158(children)X
555 4220(Overall)N
869(management)X
1370(is)X
1479(handled)X
1808(by)X
1945(the)X
2102(generic)X
2410(procedures)X
5 f
2862(XtCreateWidget)X
1 f
3548(and)X
5 f
3742(XtDes-)X
555 4316(troyWidget)N
1 f
991(.)X
5 f
1082(XtCreateWidget)X
1 f
1745(adds)X
1932(children)X
2247(to)X
2342(their)X
2529(parent)X
2774(by)X
2887(calling)X
3153(the)X
3286(parent's)X
3594(insert_child)X
555 4412(procedure.)N
5 f
999(XtDestroyWidget)X
1 f
1728(removes)X
2079(children)X
2421(from)X
2645(their)X
2860(parent)X
3133(by)X
3274(calling)X
3568(the)X
3728(parent's)X
555 4508(delete_child)N
1009(procedure)X
1382(and)X
1531(ensures)X
1816(all)X
1927(children)X
2238(of)X
2333(a)X
2394(destroyed)X
2758(widget)X
3020(also)X
3184(get)X
3314(destroyed.)X
555 4632(Only)N
754(a)X
816(subset)X
1059(of)X
1155(the)X
1286(total)X
1467(number)X
1759(of)X
1855(children)X
2167(are)X
2297(actually)X
2600(managed)X
2941(by)X
3052(the)X
3182(geometry)X
3537(manager)X
3862(and,)X
555 4728(hence,)N
810(possibly)X
1132(visible.)X
1440(For)X
1590(example,)X
1939(an)X
2049(emacs-like)X
2458(composite)X
2848(editor)X
3081(widget)X
3348(might)X
3582(allocate)X
3884(one)X
555 4824(child)N
762(widget)X
1032(per)X
1174(buffer,)X
1440(but)X
1583(it)X
1663(might)X
1900(only)X
2087(display)X
2372(a)X
2441(small)X
2663(number)X
2961(of)X
3063(the)X
3200(existing)X
3509(buffers.)X
3830(Win-)X
555 4920(dows)N
767(that)X
927(are)X
1061(in)X
1157(this)X
1312(displayable)X
1740(subset)X
1986(are)X
2119(called)X
2356(``managed'')X
2816(windows)X
3159(and)X
3312(enter)X
3514(into)X
3678(geometry)X
555 5016(manager)N
885(calculations.)X
1378(The)X
1542(other)X
1750(children)X
2065(are)X
2198(``unmanaged'')X
2746(windows)X
3089(and,)X
3264(by)X
3378(de\256nition,)X
3765(are)X
3898(not)X
555 5112(mapped.)N
555 5236(Children)N
900(are)X
1043(added)X
1289(to)X
1394(and)X
1557(removed)X
1901(from)X
2108(the)X
2252(managed)X
2606(set)X
2739(by)X
2862(using)X
5 f
3106(XtManageChildren)X
1 f
3884(and)X
5 f
570 5332(XtUnmanageChildren)N
1 f
1425(,)X
1472(which)X
1712(notify)X
1947(the)X
2079(parent)X
2323(to)X
2416(recalculate)X
2822(the)X
2954(physical)X
3272(layout)X
3517(of)X
3614(its)X
3722(children)X
555 5428(by)N
665(calling)X
928(the)X
1058(parent's)X
1363(change_managed)X
1996(procedure.)X
555 5552(Most)N
761(managed)X
1104(children)X
1418(are)X
1550(mapped,)X
1876(but)X
2014(some)X
2225(widgets)X
2524(may)X
2701(be)X
2809(in)X
2903(a)X
2967(state)X
3153(where)X
3391(they)X
3567(take)X
3738(up)X
3850(phy-)X
555 5648(sical)N
776(space)X
1030(but)X
1202(do)X
1349(not)X
1521(show)X
1764(anything.)X
2175(Managed)X
2560(widgets)X
2892(will)X
3088(not)X
3259(be)X
3400(mapped)X
3737(if)X
3849(their)X
555 5744(map_when_managed)N
1327(\256eld)X
1508(is)X
1591(FALSE.)X
1906(This)X
2087(\256eld)X
2268(default)X
2537(is)X
2620(TRUE)X
2874(and)X
3024(is)X
3106(changed)X
3422(by)X
3533(using)X
5 f
3765(XtSet-)X
555 5840(MappedWhenManaged)N
1 f
1485(.)X
3 f
2239 6144(24)N

25 p
%%Page: 25 27
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(Each)N
754(composite)X
1139(widget)X
1401(class)X
1594(has)X
1733(a)X
1794(geometry)X
2149(manager,)X
2496(which)X
2733(is)X
2814(responsible)X
3237(for)X
3361(\256guring)X
3662(out)X
3797(where)X
555 768(the)N
690(managed)X
1035(children)X
1351(should)X
1613(appear)X
1873(within)X
2125(the)X
2259(composite)X
2648(widget's)X
2977(window.)X
3330(Geometry)X
3708(manage-)X
555 864(ment)N
754(techniques)X
1153(fall)X
1293(into)X
1453(three)X
1651(classes:)X
10 f
555 988(g)N
1 f
775(Fixed)X
1008(boxes)X
1244(have)X
1441(a)X
1511(\256xed)X
1718(number)X
2018(of)X
2122(children)X
2442(that)X
2606(are)X
2744(created)X
3029(by)X
3148(the)X
3287(parent.)X
3582(All)X
3726(of)X
3830(these)X
775 1084(children)N
1089(are)X
1221(managed,)X
1586(and)X
1737(none)X
1932(of)X
2029(them)X
2230(ever)X
2405(make)X
2620(geometry)X
2977(manager)X
3304(requests.)X
3660(An)X
3791(exam-)X
775 1180(ple)N
905(is)X
986(Scrollbar.)X
10 f
555 1304(g)N
1 f
775(Homogeneous)X
1311(boxes)X
1544(treat)X
1729(all)X
1846(children)X
2163(equally)X
2451(and)X
2606(apply)X
2830(the)X
2965(same)X
3173(geometry)X
3533(constraints)X
3942(to)X
775 1400(each)N
963(child.)X
1211(Many)X
1443(clients)X
1701(insert)X
1924(and)X
2078(delete)X
2316(widgets)X
2617(freely)X
2849(\(for)X
3007(instance,)X
3345(xmh's)X
3592(folder)X
3828(box\).)X
775 1496(Examples)N
1145(are)X
1274(Menu,)X
1523(ButtonBox,)X
1955(and)X
2104(Pane.)X
10 f
555 1620(g)N
1 f
775(Heterogeneous)X
1330(boxes)X
1563(have)X
1757(a)X
1824(speci\256c)X
2121(location)X
2434(where)X
2676(each)X
2865(child)X
3069(is)X
3155(placed.)X
3456(This)X
3640(location)X
3952(is)X
775 1716(usually)N
1067(not)X
1217(speci\256ed)X
1567(in)X
1673(pixels,)X
1943(because)X
2258(the)X
2403(window)X
2723(may)X
2912(be)X
3031(resized)X
3316(but)X
3465(is)X
3560(expressed)X
3942(in)X
775 1812(terms)N
1002(of)X
1106(the)X
1245(relationship)X
1692(between)X
2015(a)X
2084(child)X
2291(and)X
2448(the)X
2586(parent's)X
2899(borders)X
3192(as)X
3295(well)X
3477(as)X
3580(between)X
3903(the)X
775 1908(child)N
989(and)X
1153(other,)X
1393(speci\256c)X
1699(children.)X
2069(Most)X
2287(clients)X
2555(insert)X
2787(a)X
2862(\256xed)X
3074(number)X
3379(of)X
3488(widgets)X
3798(into)X
3972(a)X
775 2004(homogeneous)N
1291(box.)X
1472(Heterogeneous)X
2026(boxes)X
2258(are)X
2391(usually)X
2672(subclasses)X
3064(of)X
3163(Constraint.)X
3579(An)X
3712(example)X
775 2100(is)N
856(Form.)X
555 2224(Each)N
755(composite)X
1142(widget,)X
1427(especially)X
1803(those)X
2012(that)X
2168(are)X
2298(heterogeneous,)X
2851(may)X
3026(de\256ne)X
3264(ways)X
3467(for)X
3592(one)X
3742(child)X
3942(to)X
555 2320(change)N
841(focus)X
1068(to)X
1174(another)X
1475(child)X
1689(by)X
1814(means)X
2076(of)X
2186(the)X
2330(move_focus_to_next)X
3105(and)X
3268(move_focus_to_prev)X
555 2416(procedures.)N
1017(For)X
1172(example,)X
1526(typing)X
1785(carriage)X
2101(return)X
2344(in)X
2446(one)X
2606(child)X
2816(widget)X
3089(may)X
3274(move)X
3502(to)X
3603(the)X
3743(``next'')X
555 2512(child)N
759(widget,)X
1048(while)X
1271(typing)X
1524(a)X
1590(number)X
1886(in)X
1982(one)X
2136(child)X
2340(widget)X
2607(may)X
2786(move)X
3009(focus)X
3226(to)X
3322(any)X
3476(of)X
3576(a)X
3642(number)X
3938(of)X
555 2608(children)N
866(widgets.)X
3 f
555 2800(13.1.)N
775(Composite)X
1197(Widget)X
1493(Type)X
1706(De\256nitions)X
1 f
555 2924(typedef)N
841(Widget)X
1123(*)X
2 f
(WidgetList)S
1 f
1550(;)X
3 f
555 3164(13.2.)N
775(XtIsComposite)X
1 f
555 3288(To)N
677(test)X
824(if)X
902(a)X
965(widget)X
1229(is)X
1312(a)X
1375(subclass)X
1691(of)X
1787(Composite,)X
2215(use)X
5 f
2373(XtIsComposite)X
1 f
2961(.)X
3028(The)X
3188(de\256nition)X
3550(for)X
3675(this)X
3826(func-)X
555 3384(tion)N
715(is:)X
555 3604(void)N
734(XtIsComposite\()X
2 f
1297(w)X
1 f
1356(\))X
687 3700(Widget)N
2 f
969(w)X
1 f
1028(;)X
2 f
555 3948(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(under)X
2085(question.)X
5 f
570 4072(XtIsComposite)N
1 f
1180(is)X
1261(just)X
1411(an)X
1516(abbreviation)X
1979(for)X
5 f
2121(XtIsSubclass)X
1 f
2668(with)X
2847(the)X
2977(following)X
3342(parameters)X
3750(list:)X
555 4196(\()N
2 f
584(w)X
1 f
643(,)X
2 f
687(compositeWidgetClass)X
1 f
1490(\).)X
3 f
555 4436(13.3.)N
775(Adding)X
1076(New)X
1263(Children:)X
1652(the)X
1791(insert_child)X
2259(procedure)X
1 f
555 4560(To)N
680(add)X
834(the)X
968(child)X
1171(to)X
1266(the)X
1400(parent's)X
1709(children)X
2024(array,)X
5 f
2270(XtCreateWidget)X
1 f
2933(calls)X
3121(the)X
3255(parent's)X
3564(class)X
3761(routine)X
555 4656(insert_child.)N
555 4780(Fixed)N
781(boxes)X
1011(usually)X
1291(insert)X
1512(the)X
1645(child)X
1846(\256rst)X
2007(by)X
2119(calling)X
2384(Composite's)X
2854(insert_child)X
3295(routine,)X
3591(then)X
3767(by)X
3879(cal-)X
555 4876(ling)N
5 f
742(XtManageChild)X
1 f
1389(to)X
1489(add)X
1647(the)X
1786(child)X
1994(to)X
2094(the)X
2233(box's)X
2459(managed)X
2808(set.)X
2981(Homogeneous)X
3520(boxes)X
3756(usually)X
555 4972(just)N
709(inherit)X
966(Composite's)X
1438(insert_child)X
1881(routine.)X
2201(Heterogeneous)X
2753(boxes)X
2983(usually)X
3263(have)X
3454(a)X
3518(separate)X
3831(array)X
555 5068(of)N
650(constraint)X
1020(information)X
1459(on)X
1569(the)X
1699(side)X
1863(and,)X
2034(so,)X
2156(perform)X
2461(the)X
2591(insertion)X
2922(with)X
3101(custom)X
3378(code.)X
555 5192(An)N
688(insertion)X
1023(procedure)X
1400(takes)X
1607(the)X
1741(widget)X
2007(to)X
2102(insert)X
2324(and)X
2476(the)X
2609(argument)X
2967(list)X
3101(used)X
3287(to)X
3381(create)X
3616(the)X
3749(widget.)X
555 5288(This)N
734(argument)X
1089(list)X
1220(is)X
1301(usually)X
1578(used)X
1761(only)X
1940(by)X
2050(homogeneous)X
2561(boxes)X
2788(\(subclasses)X
3205(of)X
3300(Constraint\).)X
3 f
2239 6144(25)N

26 p
%%Page: 26 28
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtArgsProc)X
1 f
1509(\)\(\);)X
687 864(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 960(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 1056(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
2 f
555 1304(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(insert.)X
2 f
555 1428(args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(argument)X
1956(list)X
2087(passed)X
2343(to)X
5 f
2452(XtCreateWidget)X
1 f
3089(.)X
2 f
555 1552(num_args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arguments)X
2376(in)X
2467(argument)X
2822(list.)X
3 f
555 1744(13.3.1.)N
841(Insertion)X
1205(Order)X
1 f
555 1964(typedef)N
841(Cardinal)X
1167(\(*)X
2 f
1240(XtOrderProc)X
1 f
1704(\)\(\);)X
687 2060(Widget)N
2 f
969(w)X
1 f
1028(;)X
555 2280(Instances)N
914(of)X
1019(composite)X
1414(widgets)X
1720(may)X
1904(care)X
2082(about)X
2310(the)X
2449(order)X
2665(in)X
2765(which)X
3011(their)X
3204(children)X
3524(are)X
3662(kept.)X
3889(For)X
555 2376(example,)N
907(an)X
1021(application)X
1445(may)X
1628(want)X
1830(a)X
1900(set)X
2029(of)X
2133(command)X
2512(buttons)X
2803(in)X
2903(some)X
3119(logical)X
3390(order)X
3605(grouped)X
3923(by)X
555 2472(function,)N
906(while)X
1136(it)X
1220(may)X
1406(want)X
1611(buttons)X
1905(that)X
2072(represent)X
2428(\256le)X
2575(names)X
2834(to)X
2937(be)X
3054(kept)X
3240(in)X
3343(alphabetical)X
3804(order.)X
555 2568(Composite)N
972(widgets)X
1280(that)X
1447(allow)X
1677(clients)X
1942(to)X
2045(order)X
2264(their)X
2460(children)X
2783(\(usually)X
3100(homogeneous)X
3622(boxes\))X
3889(can)X
555 2664(call)N
720(their)X
919(widget)X
1196(instance's)X
1585(insert_position)X
2147(function)X
2478(from)X
2686(the)X
2831(class's)X
3102(insert_child)X
3555(procedure)X
3942(to)X
555 2760(determine)N
930(where)X
1166(a)X
1227(new)X
1395(child)X
1594(should)X
1851(go)X
1961(in)X
2052(its)X
2158(children)X
2469(array.)X
555 2884(Using)N
796(zero)X
978(\(0\))X
1111(means)X
1367(before)X
1622(all)X
1742(other)X
1954(children,)X
2296(while)X
2523(using)X
2745(num_children)X
3265(means)X
3520(after)X
3711(all)X
3830(other)X
555 2980(children.)N
889(The)X
1049(default)X
1317(insert_position)X
1865(function)X
2182(returns)X
2449(num_children.)X
3028(This)X
3208(can)X
3353(be)X
3459(overridden)X
3862(by)X
3972(a)X
555 3076(speci\256c)N
858(composite)X
1254(widget's)X
1590(resource)X
1920(list)X
2062(or)X
2168(by)X
2289(the)X
2430(argument)X
2796(list)X
2938(provided)X
3284(when)X
3507(the)X
3648(composite)X
555 3172(widget)N
817(is)X
898(created.)X
3 f
555 3364(13.4.)N
775(Deleting)X
1110(Children:)X
1499(the)X
1638(delete_child)X
2111(procedure)X
1 f
5 f
570 3488(XtDestroyWidget)N
1 f
1275(eventually)X
1671(causes)X
1928(a)X
1995(call)X
2151(to)X
2248(the)X
2384(parent's)X
2695(class)X
2894(routine)X
3172(delete_child)X
3632(in)X
3729(order)X
3942(to)X
555 3584(remove)N
841(the)X
971(child)X
1170(from)X
1363(the)X
1493(parent's)X
1798(children)X
2109(array.)X
555 3708(Fixed)N
780(boxes)X
1009(usually)X
1288(do)X
1400(not)X
1537(need)X
1727(a)X
1790(delete_children)X
2358(routine)X
2632(because)X
2933(they)X
3108(create)X
3341(their)X
3526(own)X
3700(children.)X
555 3804(Hence,)N
831(they)X
1035(need)X
1231(not)X
1374(worry)X
1613(about)X
1839(a)X
1908(client)X
2135(calling)X
5 f
2424(XtDestroyWidget)X
1 f
3130(on)X
3248(one)X
3405(of)X
3508(their)X
3700(children.)X
555 3900(Homogeneous)N
1088(boxes)X
1318(usually)X
1598(just)X
1751(inherit)X
2007(Composite's)X
2478(delete_child)X
2935(routine.)X
3254(Heterogeneous)X
3806(boxes)X
555 3996(usually)N
839(have)X
1034(a)X
1102(separate)X
1419(array)X
1628(of)X
1730(constraint)X
2107(information)X
2552(on)X
2668(the)X
2804(side)X
2974(and,)X
3151(so,)X
3279(perform)X
3590(the)X
3726(deletion)X
555 4092(with)N
734(custom)X
1011(code.)X
555 4216(A)N
640(deletion)X
947(procedure)X
1320(is)X
1401(of)X
1496(type)X
5 f
1688(XtWidgetProc)X
1 f
2242(,)X
2286(and)X
2435(it)X
2507(merely)X
2774(takes)X
2977(the)X
3107(widget)X
3369(to)X
3460(delete.)X
3 f
555 4408(13.5.)N
775(Adding)X
1076(and)X
1240(Removing)X
1643(Children)X
2003(from)X
2210(the)X
2349(Managed)X
2723(Set)X
1 f
555 4532(The)N
727(X)X
825(Toolkit)X
1121(provides)X
1459(a)X
1532(set)X
1664(of)X
1771(generic)X
2064(routines)X
2382(to)X
2485(permit,)X
2772(without)X
3076(explicit)X
3376(knowledge)X
3796(of)X
3903(the)X
555 4628(composite)N
947(widget)X
1216(type.)X
1441(The)X
1607(addition)X
1925(of)X
2026(widgets)X
2328(to)X
2425(or)X
2526(the)X
2662(removal)X
2979(of)X
3080(widgets)X
3382(from)X
3581(a)X
3648(composite)X
555 4724(widget's)N
903(managed)X
1266(set.)X
1453(These)X
1708(generic)X
2012(routines)X
2340(eventually)X
2752(call)X
2924(the)X
3076(widget's)X
3423(class)X
3638(procedure,)X
555 4820(change_managed,)N
1210(which)X
1447(is)X
1528(of)X
1623(type)X
5 f
1815(XtWidgetProc)X
1 f
2369(.)X
3 f
555 5012(13.5.1.)N
841(XtManageChildren)X
1 f
555 5136(To)N
678(add)X
830(a)X
894(list)X
1028(of)X
1126(widgets)X
1425(to)X
1519(the)X
1652(geometry-managed)X
2356(\(and,)X
2558(hence,)X
2809(displayable\))X
3264(subset)X
3508(of)X
3605(their)X
3791(parent)X
555 5232(widget,)N
852(the)X
995(application)X
1423(must)X
1630(\256rst)X
1802(create)X
2047(the)X
2190(widgets)X
2499(by)X
2622(using)X
5 f
2865(XtCreateWidget)X
1 f
3536(and)X
3697(then)X
3883(call)X
5 f
570 5328(XtManageChildren)N
1 f
1313(.)X
1423(\(Note)X
1689(that)X
1888(some)X
2140(composite)X
2569(widgets,)X
2931(especially)X
3350(\256xed)X
3591(boxes,)X
3883(call)X
5 f
570 5424(XtManageChild)N
1 f
1208(from)X
1401(their)X
1585(insert_child)X
2024(procedure.\))X
2448(The)X
2607(de\256nition)X
2968(for)X
5 f
3110(XtManageChildren)X
1 f
3875(is:)X
3 f
2239 6144(26)N

27 p
%%Page: 27 29
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(void)N
734(XtManageChildren\()X
2 f
1443(children)X
1 f
1737(,)X
2 f
1781(num_children)X
1 f
2270(\))X
687 864(WidgetList)N
2 f
1107(children)X
1 f
1401(;)X
687 960(Cardinal)N
2 f
1013(num_children)X
1 f
1502(;)X
2 f
555 1208(children)N
1 f
1131(Speci\256es)X
1471(a)X
1532(list)X
1663(of)X
1758(children)X
2069(to)X
2160(add.)X
2 f
555 1332(num_children)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(children)X
2298(to)X
2389(add.)X
5 f
570 1456(XtManageChildren)N
1 f
1335(performs)X
1674(the)X
1804(following)X
2169(actions:)X
10 f
555 1580(g)N
1 f
775(Issues)X
1011(an)X
1116(error)X
1308(if)X
1384(the)X
1514(children)X
1825(do)X
1935(not)X
2070(all)X
2181(have)X
2369(the)X
2499(same)X
2702(parent.)X
10 f
555 1704(g)N
1 f
775(Returns)X
1071 0.3187(immediately)AX
1536(if)X
1612(the)X
1742(common)X
2073(parent)X
2315(is)X
2396(being)X
2614(destroyed.)X
10 f
555 1828(g)N
1 f
775(Otherwise,)X
1180(it)X
1252(processes)X
1610(the)X
1740(unique,)X
2024(unmanaged)X
2452(children:)X
9 f
775 1952(-)N
1 f
995(A)X
1080(child)X
1279(is)X
1360(ignored)X
1651(if)X
1727(it)X
1799(is)X
1880(already)X
2161(managed)X
2501(or)X
2596(being)X
2814(destroyed.)X
9 f
775 2076(-)N
1 f
995(Otherwise,)X
1407(the)X
1543(child)X
1748(is)X
1835(marked)X
2127(as)X
2228(managed,)X
2596(and)X
2751(the)X
2887(parent's)X
3198(num_mapped_children)X
995 2172(\256eld)N
1174(is)X
1255(incremented)X
1713(if)X
1789(the)X
1919(child)X
2118(has)X
2257(map_when_managed)X
3027(TRUE.)X
10 f
555 2296(g)N
1 f
775(Calls)X
979(the)X
1109(change_managed)X
1742(routine)X
2014(of)X
2109(the)X
2239(widgets')X
2564(parent.)X
10 f
555 2420(g)N
1 f
775(If)X
865(the)X
1005(parent)X
1257(is)X
1348(realized,)X
5 f
1699(XtManageChildren)X
1 f
2474(makes)X
2731(\(some)X
2978(of\))X
3112(the)X
3252(newly)X
3499(managed)X
3849(chil-)X
775 2516(dren)N
953(visible:)X
9 f
775 2640(-)N
1 f
995(Calls)X
5 f
1217(XtRealizeWidget)X
1 f
1904(on)X
2014(each)X
2197(previously)X
2591(unmanaged)X
3019(child)X
3218(that)X
3373(is)X
3454(unrealized.)X
9 f
775 2764(-)N
1 f
995(Maps)X
1212(each)X
1395(previously)X
1789(unmanaged)X
2217(child)X
2416(that)X
2571(has)X
2710(map_when_managed)X
3480(TRUE.)X
555 2888(The)N
723(composite)X
1117(widget's)X
1451(change_managed)X
2093(procedure)X
2475(is)X
2564(called)X
2805(to)X
2904(notify)X
3145(the)X
3283(parent)X
3533(that)X
3696(its)X
3810(set)X
3938(of)X
555 2984(managed)N
900(children)X
1216(has)X
1360(changed.)X
1724(It)X
1804(may,)X
2004(as)X
2103(it)X
2179(sees)X
2351(\256t,)X
2473(reposition)X
2852(and)X
3005(resize)X
3236(any)X
3389(of)X
3488(its)X
3598(children.)X
3957(It)X
555 3080(moves)N
816(each)X
1008(child)X
1216(as)X
1320(needed)X
1600(by)X
1719(using)X
1941(the)X
5 f
2097(XtMoveWidget)X
1 f
2715(procedure,)X
3118(which)X
3363(\256rst)X
3530(updates)X
3829(the)X
3967(x)X
555 3176(and)N
704(y)X
770(\256elds.)X
1027(Then,)X
1252(it)X
1324(calls)X
5 f
1526(XMoveWindow)X
1 f
2149(if)X
2225(the)X
2355(widget)X
2617(is)X
2698(realized.)X
555 3300(If)N
641(the)X
777(composite)X
1168(widget)X
1436(wishes)X
1703(to)X
1800(change)X
2077(the)X
2213(size)X
2378(or)X
2479(border)X
2736(width)X
2965(of)X
3066(any)X
3221(of)X
3322(its)X
3434(children,)X
3772(it)X
3849(calls)X
555 3396(the)N
5 f
709(XtResizeWidget)X
1 f
1378(procedure,)X
1779(which)X
2022(\256rst)X
2187(updates)X
2484(the)X
2620(width,)X
2871(height,)X
3142(and)X
3297(border_width)X
3798(\256elds.)X
555 3492(Then,)N
780(it)X
852(calls)X
5 f
1054(XCon\256gureWindow)X
1 f
1843(if)X
1919(the)X
2049(widget)X
2311(is)X
2392(realized.)X
3 f
555 3684(13.5.2.)N
841(XtManageChild)X
1 f
555 3808(To)N
689(add)X
852(a)X
927(single)X
1174(child)X
1387(to)X
1492(the)X
1636(managed)X
1990(children)X
2315(of)X
2424(its)X
2544(parent)X
2799(widget,)X
3096(the)X
3239(application)X
3667(must)X
3874(\256rst)X
555 3904(create)N
791(the)X
924(widget)X
1189(by)X
1302(using)X
5 f
1536(XtCreateWidget)X
1 f
2198(and)X
2350(then)X
2527(call)X
5 f
2698(XtManageChild)X
1 f
3314(.)X
3383(The)X
3545(de\256nition)X
3909(for)X
555 4000(this)N
705(function)X
1021(is:)X
555 4220(void)N
734(XtManageChild\()X
2 f
1331(child)X
1 f
1508(\))X
687 4316(Widget)N
2 f
969(child)X
1 f
1146(;)X
2 f
555 4564(child)N
1 f
1131(Speci\256es)X
1471(the)X
1601(child)X
1800(to)X
1891(add.)X
5 f
570 4688(XtManageChild)N
1 f
1208(constructs)X
1587(a)X
1648(WidgetList)X
2068(of)X
2163(length)X
2406(one)X
2555(\(1\))X
2679(and)X
2828(calls)X
5 f
3030(XtManageChildren)X
1 f
3773(.)X
3 f
555 4880(13.5.3.)N
841(XtUnmanageChildren)X
1 f
555 5004(To)N
681(remove)X
973(a)X
1040(list)X
1177(of)X
1278(children)X
1595(from)X
1794(the)X
1930(managed)X
2276(list)X
2413(of)X
2514(their)X
2703(parent,)X
2972(use)X
5 f
3134(XtUnmanageChildren)X
1 f
3989(.)X
555 5100(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 5320(void)N
734(XtUnmanageChildren\()X
2 f
1541(children)X
1 f
1835(,)X
2 f
1879(num_children)X
1 f
2368(\))X
687 5416(WidgetList)N
2 f
1107(children)X
1 f
1401(;)X
687 5512(Cardinal)N
2 f
1013(num_children)X
1 f
1502(;)X
3 f
2239 6144(27)N

28 p
%%Page: 28 30
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
2 f
555 672(children)N
1 f
1131(Speci\256es)X
1471(the)X
1601(children)X
1912(to)X
2003(remove.)X
2 f
555 796(num_children)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(children)X
2298(to)X
2389(remove.)X
5 f
570 920(XtUnmanageChildren)N
1 f
1447(performs)X
1786(the)X
1916(following:)X
10 f
555 1044(g)N
1 f
775(Issues)X
1011(an)X
1116(error)X
1308(if)X
1384(the)X
1514(children)X
1825(do)X
1935(not)X
2070(all)X
2181(have)X
2369(the)X
2499(same)X
2702(parent.)X
10 f
555 1168(g)N
1 f
775(Returns)X
1071 0.3187(immediately)AX
1536(if)X
1612(the)X
1742(common)X
2073(parent)X
2315(is)X
2396(being)X
2614(destroyed.)X
10 f
555 1292(g)N
1 f
775(Otherwise,)X
1180(it)X
1252(processes)X
1610(the)X
1740(unique,)X
2024(managed)X
2364(children:)X
9 f
775 1416(-)N
1 f
995(A)X
1080(child)X
1279(is)X
1360(ignored)X
1651(if)X
1727(it)X
1799(is)X
1880(already)X
2161(unmanaged)X
2589(or)X
2684(being)X
2902(destroyed.)X
9 f
775 1540(-)N
1 f
995(Otherwise,)X
5 f
1418(XtUnmanagedChildren)X
1 f
2344(marks)X
2581(the)X
2711(child)X
2910(as)X
3005(unmanaged.)X
9 f
775 1664(-)N
1 f
995(If)X
1075(the)X
1205(parent)X
1447(is)X
1528(realized,)X
1851(makes)X
2098(child)X
2297(non-visible)X
2716(by)X
2826(unmapping)X
3245(it.)X
9 f
775 1788(-)N
1 f
995(Decrements)X
1505(the)X
1703(parent's)X
2076(num_mapped_children)X
2979(\256eld)X
3225(if)X
3368(the)X
3565(widget)X
3894(has)X
995 1884(map_when_managed)N
1765(TRUE.)X
10 f
555 2008(g)N
1 f
775(Calls)X
979(the)X
1109(change_managed)X
1742(routine)X
2014(of)X
2109(the)X
2239(widgets')X
2564(parent.)X
5 f
570 2132(XtUnmanageChildren)N
1 f
1474(does)X
1684(not)X
1846(destroy)X
2154(the)X
2311(children)X
2649(widgets.)X
3015(Removing)X
3431(widgets)X
3753(from)X
3972(a)X
555 2228(parent's)N
862(managed)X
1204(set)X
1326(is)X
1409(often)X
1614(a)X
1677(temporary)X
2063(banishment,)X
2516(and)X
2667(some)X
2877(time)X
3058(later)X
3238(you)X
3393(may)X
3568(add)X
3718(the)X
3849(chil-)X
555 2324(dren)N
733(again.)X
990(To)X
1110(entirely)X
1402(destroy)X
1683(widgets,)X
2001(see)X
2135(``XtDestroyWidget''.)X
3 f
555 2516(13.5.4.)N
841(XtUnmanageChild)X
1 f
555 2640(To)N
677(remove)X
965(a)X
1028(single)X
1263(child)X
1464(from)X
1659(its)X
1767(parent's)X
2074(managed)X
2416(set,)X
2559(use)X
5 f
2717(XtUnmanageChild)X
1 f
3445(.)X
3512(The)X
3672(de\256nition)X
555 2736(for)N
679(this)X
829(function)X
1145(is:)X
555 2956(void)N
734(XtUnmanageChild\()X
2 f
1429(child)X
1 f
1606(\))X
687 3052(Widget)N
2 f
969(child)X
1 f
1146(;)X
2 f
555 3300(child)N
1 f
1131(Speci\256es)X
1471(the)X
1601(child)X
1800(to)X
1891(remove.)X
5 f
570 3424(XtUnmanageChild)N
1 f
1338(constructs)X
1735(a)X
1814(WidgetList)X
2252(of)X
2365(length)X
2626(one)X
2792(\(1\))X
2933(and)X
3099(calls)X
5 f
3318(XtUnmanageChil-)X
555 3520(dren)N
1 f
746(.)X
555 3644(These)N
791(generic)X
1076(routines)X
1386(are)X
1519(low-level)X
1878(routines)X
2188(used)X
2375(by)X
2489(``generic'')X
2890(composite)X
3279(widget)X
3545(building)X
3865(rou-)X
555 3740(tines.)N
777(Composite)X
1193(widgets)X
1500(may)X
1685(in)X
1787(addition)X
2110(provide)X
2411(widget-speci\256c,)X
3003(high-level)X
3393(convenience)X
3865(rou-)X
555 3836(tines)N
744(to)X
835(allow)X
1053(applications)X
1502(to)X
1593(create)X
1825(and)X
1974(manage)X
2270(children)X
2581(more)X
2784(easily.)X
3 f
555 4028(13.6.)N
775(XtSetMappedWhenManaged)X
1 f
555 4152(A)N
649(widget)X
920(is)X
1010(normally)X
1360(mapped)X
1670(if)X
1755(it)X
1836(is)X
1926(managed.)X
2318(This)X
2505(behavior)X
2843(can)X
2995(be)X
3108(overridden)X
3519(by)X
3637(setting)X
3903(the)X
555 4248(map_when_managed)N
1340(\256eld)X
1534(FALSE.)X
1861(To)X
1995(change)X
2280(the)X
2424(map_when_managed)X
3208(\256eld,)X
3423(use)X
5 f
3594(XtSetMap-)X
555 4344(pedWhenManaged)N
1 f
1314(.)X
1380(The)X
1539(de\256nition)X
1900(for)X
2024(this)X
2174(function)X
2490(is:)X
555 4564(void)N
734(XtSetMappedWhenManaged\()X
2 f
1789(w)X
1 f
1848(,)X
2 f
1892(map_when_managed)X
1 f
2639(\))X
687 4660(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 4756(Boolean)N
2 f
1003(map_when_managed)X
1 f
1750(;)X
2 f
555 5004(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 5128(map_when_managed)N
1 f
1131 5224(Speci\256es)N
1471(the)X
1601(new)X
1769(value.)X
555 5348(If)N
637(the)X
769(widget)X
1032(is)X
1114(realized)X
1416(and)X
1566(managed,)X
5 f
1947(XtSetMappedWhenManaged)X
1 f
3117(maps)X
3326(or)X
3422(unmaps)X
3719(the)X
3850(win-)X
555 5444(dow)N
728(as)X
823(appropriate.)X
3 f
2239 6144(28)N

29 p
%%Page: 29 31
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(14.)N
709(Toolkit)X
1006(Initialization)X
1 f
555 796(To)N
675 0.3542(initialize)AX
1008(the)X
1138(X)X
1223(Toolkit,)X
1528(call)X
1678(the)X
5 f
1826(XtInitialize)X
1 f
2264(function.)X
2624(The)X
2783(de\256nition)X
3144(of)X
3239(the)X
3369(functions)X
3719(is;)X
555 1016(Widget)N
837(XtInitialize\()X
2 f
1269(name)X
1 f
1459(,)X
2 f
1503(class_name)X
1 f
1913(,)X
2 f
1957(options)X
1 f
2217(,)X
2 f
2261(num_options)X
1 f
2716(,)X
2 f
2760(argc)X
1 f
2921(,)X
2 f
2965(argv)X
1 f
3126(\))X
687 1112(String)N
2 f
925(name)X
1 f
1115(;)X
687 1208(String)N
2 f
925(class_name)X
1 f
1335(;)X
687 1304(XrmOptionDescRec)N
2 f
1427(options)X
1 f
1687(;)X
687 1400(Cardinal)N
2 f
1013(num_options)X
1 f
1468(;)X
687 1496(Cardinal)N
1013(*)X
2 f
(argc)S
1 f
1218(;)X
687 1592(char)N
860(*)X
2 f
(argv)S
1 f
1065([];)X
2 f
555 1840(name)N
1 f
1131(Name)X
1363(of)X
1458(this)X
1608(instance)X
1919(of)X
2014(the)X
2144(application.)X
2 f
555 1964(class)N
1 f
1131(Class)X
1348(name)X
1565(of)X
1664(this)X
1818(application.)X
2259(Usually)X
2559(the)X
2693(generic)X
2978(name)X
3195(for)X
3323(all)X
3437(instances)X
3785(of)X
3883(this)X
1131 2060(application.)N
2 f
555 2184(options)N
1 f
1131(Speci\256es)X
1479(how)X
1660(to)X
1759(parse)X
1974(the)X
2112(command)X
2489(line)X
2651(for)X
2782(any)X
2938(application-speci\256c)X
3658(resources.)X
1131 2280(The)N
1301(options)X
1594(is)X
1686(passed)X
1953(as)X
2059(a)X
2131(parameter)X
2516(to)X
5 f
2636(XrmParseCommand)X
1 f
3447(.)X
3524(\(See)X
3713(the)X
3854(Xlib)X
1131 2376(manual)N
1413(for)X
1537(further)X
1798(information.\))X
2 f
555 2500(num_options)N
1 f
1131(The)X
1290(number)X
1581(of)X
1676(entries)X
1933(in)X
2024(options)X
2306(list.)X
2 f
555 2624(argc)N
1 f
1131(Address)X
1440(of)X
1535(the)X
1665(number)X
1956(of)X
2051(command)X
2421(line)X
2576(parameters.)X
2 f
555 2748(argv)N
1 f
1131(Command)X
1521(line)X
1676(parameters.)X
5 f
570 2872(XtInitialize)N
1 f
1012(must)X
1210(be)X
1319(called)X
1556(before)X
1806(calling)X
2073(any)X
2226(other)X
2433(X)X
2522(Toolkit)X
2809(functions.)X
3207(It)X
3286(parses)X
3530(the)X
3663(command)X
555 2968(line,)N
738(opens)X
971(the)X
1106(display,)X
1410(and)X
1564(initializes)X
1936(the)X
2071(toolkit.)X
2352(It)X
2433(returns)X
2704(the)X
2839(``top-level'')X
3296(widget)X
3563(to)X
3659(use)X
3803(as)X
3903(the)X
555 3064(parent)N
797(of)X
892(the)X
1022(application's)X
1500(root)X
1664(widget.)X
555 3188(It)N
644(modi\256es)X
983(argc)X
1169(and)X
1331(argv)X
1522(to)X
1626(contain)X
1921(just)X
2084(the)X
2227(parameters)X
2647(that)X
2814(were)X
3018(not)X
3165(a)X
3238(display,)X
3549(geometry,)X
3938(or)X
555 3284(resource)N
878(speci\256cation.)X
1372(If)X
1456(the)X
1590(modi\256ed)X
1930(argc)X
2107(is)X
2192(not)X
2331(zero)X
2508(\(0\),)X
2658(most)X
2855(applications)X
3307(simply)X
3573(print)X
3765(out)X
3903(the)X
555 3380(modi\256ed)N
891(argv)X
1069(along)X
1287(with)X
1466(a)X
1527(message)X
1847(about)X
2065(the)X
2195(allowable)X
2560(options.)X
3 f
555 3572(14.1.)N
775(Setting)X
1061(Resources)X
1463(from)X
1670(the)X
1809(Command)X
2226(Line)X
1 f
5 f
570 3696(XtInitialize)N
1 f
1018(has)X
1167(a)X
1238(table)X
1442(of)X
1546(standard)X
1875(command)X
2254(line)X
2418(options)X
2709(for)X
2842(adding)X
3113(resources)X
3475(to)X
3575(the)X
3714(resource)X
555 3792(database,)N
909(and)X
1065(it)X
1144(takes)X
1353(as)X
1454(a)X
1521(parameter)X
1901(additional)X
2283(application-speci\256c)X
3002(resource)X
3327(abbreviations.)X
3874(The)X
555 3888(format)N
812(of)X
907(this)X
1057(table)X
1251(is)X
1332(described)X
1691(in)X
1782(the)X
1912(Xlib)X
2091(documentation.)X
555 4012(The)N
714(standard)X
1034(table)X
1228(contains)X
1544(the)X
1674(following)X
2039(entries:)X
10 f
555 4068(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 4212(Option)N
822(string)X
1155(Resource)X
1504(name)X
1827(Argument)X
2206(Kind)X
2514(Resource)X
2863(value)X
10 f
555 4268(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 4360(h)N
583(hhhhhhhhhh)X
555 4376(h)N
583(hhhhhhhhhh)X
1 f
1155 4412(geometry)N
1827(IsArg)X
555 4508(-bd)N
1155(border)X
1827(SepArg)X
555 4604(-bordercolor)N
1155(border)X
1827(SepArg)X
555 4700(-bg)N
1155(background)X
1827(SepArg)X
555 4796(-background)N
1155(background)X
1827(SepArg)X
555 4892(-bw)N
1155(borderWidth)X
1827(SepArg)X
555 4988(-border)N
1155(borderWidth)X
1827(SepArg)X
555 5084(-fg)N
1155(foreground)X
1827(SepArg)X
555 5180(-foreground)N
1155(foreground)X
1827(SepArg)X
555 5276(-fn)N
1155(font)X
1827(SepArg)X
555 5372(-font)N
1155(font)X
1827(SepArg)X
555 5468(-rv)N
1155(reverseVideo)X
1827(NoArg)X
2514(on)X
2624([not)X
2788(implemented])X
555 5564(-reverse)N
1155(reverseVideo)X
1827(NoArg)X
2514(on)X
2624([not)X
2788(implemented])X
555 5660(+rv)N
1155(reverseVideo)X
1827(NoArg)X
2514(off)X
2638([not)X
2802(implemented])X
555 5756(-n)N
1155(name)X
1827(SepArg)X
555 5852(-name)N
1155(name)X
1827(SepArg)X
3 f
2239 6144(29)N

30 p
%%Page: 30 32
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(-title)N
1155(title)X
1827(SepArg)X
555 768(-t)N
1155(title)X
1827(SepArg)X
10 f
555 824(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
3 f
555 1064(15.)N
709(Memory)X
1053(Management)X
1 f
555 1188(The)N
728(X)X
827(Toolkit)X
1123(memory)X
1452(management)X
1938(routines)X
2257(provide)X
2561(uniform)X
2880(checking)X
3233(for)X
3370(null)X
3543(pointers,)X
3884(and)X
555 1284(error)N
755(reporting)X
1108(on)X
1226(memory)X
1550(allocation)X
1929(errors.)X
2185(These)X
2424(routines)X
2737(are)X
2873(completely)X
3295(compatible)X
3717(with)X
3903(the)X
555 1380(standard)N
880(C)X
966(language)X
1311(runtime)X
1613(routines)X
5 f
1942(malloc)X
1 f
2210(,)X
5 f
2277(calloc)X
1 f
2516(,)X
5 f
2583(realloc)X
1 f
2856(,)X
2905(and)X
5 f
3077(free)X
1 f
3271(with)X
3455(the)X
3590(added)X
3826(func-)X
555 1476 0.3681(tionality:)AN
10 f
555 1600(g)N
1 f
5 f
790(XtMalloc)X
1 f
1142(,)X
5 f
1204(XtCalloc)X
1 f
1546(,)X
1590(and)X
5 f
1757(XtRealloc)X
1 f
2170(give)X
2344(an)X
2449(error)X
2641(if)X
2717(there)X
2915(is)X
2996(not)X
3131(enough)X
3412(memory.)X
10 f
555 1724(g)N
1 f
5 f
790(XtFree)X
1 f
1092(simply)X
1355(returns)X
1621(if)X
1697(passed)X
1953(a)X
2014(NULL)X
2270(pointer.)X
10 f
555 1848(g)N
1 f
5 f
790(XtRealloc)X
1 f
1203(simply)X
1466(allocates)X
1797(new)X
1965(storage)X
2241(if)X
2317(passed)X
2573(a)X
2634(NULL)X
2890(pointer.)X
555 1972(See)N
712(the)X
850(C)X
939(library)X
1203(documentation)X
1757(on)X
5 f
1892(malloc)X
1 f
2160(,)X
5 f
2229(calloc)X
1 f
2468(,)X
5 f
2537(realloc)X
1 f
2810(,)X
2861(and)X
5 f
3035(free)X
1 f
3231(for)X
3362(more)X
3572(information.)X
555 2068(The)N
714(de\256nitions)X
1109(for)X
1233(the)X
1363(X)X
1448(Toolkit)X
1731(memory)X
2047(management)X
2520(routines)X
2826(are:)X
555 2288(char)N
728(*XtMalloc\()X
2 f
1139(size)X
1 f
(\);)S
687 2384(Cardinal)N
2 f
1013(size)X
1 f
(;)S
2 f
555 2632(size)N
1 f
1131(Number)X
1441(of)X
1536(bytes)X
1744(desired.)X
5 f
570 2756(XtMalloc)N
1 f
944(returns)X
1210(a)X
1271(pointer)X
1543(to)X
1634(a)X
1695(block)X
1913(of)X
2008(storage)X
2284(of)X
2379(at)X
2465(least)X
2649(the)X
2779(speci\256ed)X
3114(size)X
3273(bytes.)X
555 2976(char)N
728(*XtCalloc\()X
2 f
1120(num)X
1 f
1271(,)X
2 f
1315(size)X
1 f
(\);)S
687 3072(Cardinal)N
2 f
1013(num)X
1 f
1164(;)X
687 3168(Cardinal)N
2 f
1013(size)X
1 f
(;)S
2 f
555 3416(num)N
1 f
1131(Number)X
1441(of)X
1536(array)X
1738(elements)X
2074(to)X
2165(allocate.)X
2 f
555 3540(size)N
1 f
1131(Size)X
1305(in)X
1396(bytes)X
1604(of)X
1699(an)X
1804(array)X
2006(element.)X
5 f
570 3664(XtCalloc)N
1 f
934(allocates)X
1265(space)X
1482(for)X
1606(the)X
1736(speci\256ed)X
2071(number)X
2362(of)X
2457(array)X
2659(elements)X
2995(of)X
3090(the)X
3220(speci\256ed)X
3555(size)X
3714(bytes.)X
555 3884(char)N
728(*XtRealloc\()X
2 f
1159(ptr)X
1 f
1262(,)X
2 f
1306(num)X
1 f
1457(\);)X
687 3980(char)N
860(*)X
2 f
(ptr)S
1 f
1007(;)X
687 4076(Cardinal)N
2 f
1013(num)X
1 f
1164(;)X
2 f
555 4324(ptr)N
1 f
1131(Pointer)X
1408(to)X
1499(old)X
1634(storage.)X
2 f
555 4448(num)N
1 f
1131(Number)X
1441(of)X
1536(bytes)X
1744(desired)X
2020(in)X
2111(new)X
2279(storage.)X
5 f
570 4572(XtRealloc)N
1 f
991(changes)X
1304(the)X
1442(size)X
1609(of)X
1712(a)X
1781(block)X
2007(of)X
2110(storage)X
2393(\(possibly)X
2745(moving)X
3044(it\).)X
3196(Then,)X
3428(it)X
3507(copies)X
3761(the)X
3898(old)X
555 4668(contents)N
871(into)X
1031(the)X
1161(new)X
1329(block)X
1547(\(as)X
1671(much)X
1889(as)X
1984(will)X
2144(\256t\).)X
555 4888(void)N
734(XtFree\()X
2 f
1007(ptr)X
1 f
1110(\);)X
687 4984(char)N
860(*)X
2 f
(ptr)S
1 f
1007(;)X
2 f
555 5232(ptr)N
1 f
1131(Pointer)X
1408(to)X
1499(block)X
1717(of)X
1812(storage)X
2088(to)X
2179(free.)X
555 5356(Free)N
733(returns)X
999(storage)X
1275(and)X
1424(allows)X
1676(it)X
1748(to)X
1839(be)X
1944(reused.)X
3 f
555 5548(16.)N
709(Input)X
941(and)X
1105(Events)X
1 f
555 5672(While)N
793(X)X
878(allows)X
1130(the)X
1260(reading)X
1546(and)X
1695(processing)X
2093(of)X
2188(events)X
2435(anywhere)X
2798(in)X
2889(an)X
2994(application,)X
3431(widgets)X
3727(in)X
3818(the)X
3948(X)X
555 5768(Toolkit)N
843(neither)X
1115(read)X
1293(events)X
1545(nor)X
1689(grab)X
1872(the)X
2007(server)X
2248(or)X
2348(mouse.)X
2649(Widgets)X
2970(merely)X
3241(register)X
3531(procedures)X
3942(to)X
3 f
2239 6144(30)N

31 p
%%Page: 31 33
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(be)N
660(called)X
893(when)X
1105(an)X
1210(event)X
1423(or)X
1518(class)X
1711(of)X
1806(events)X
2053(occurs)X
2304(in)X
2395(that)X
2550(widget.)X
555 796(A)N
649(typical)X
921(application)X
1345(consists)X
1655(of)X
1759(startup)X
2030(code)X
2227(followed)X
2571(by)X
2690(an)X
2804(event)X
3026(loop)X
3213(\(see)X
3384(``XtMainLoop''\),)X
555 892(which)N
792(reads)X
999(events)X
1246(and)X
1395(dispatches)X
1784(events)X
2031(by)X
2141(calling)X
2404(the)X
2534(procedures)X
2941(that)X
3096(widgets)X
3392(have)X
3580(registered.)X
555 1016(The)N
714(event)X
927(manager)X
1252(is)X
1333(a)X
1394(collection)X
1765(of)X
1860(routines)X
2166(to:)X
10 f
555 1140(g)N
1 f
775(Add)X
959(or)X
1065(remove)X
1362(event)X
1586(sources)X
1882(other)X
2096(that)X
2262(X)X
2358(window)X
2674(events)X
2932(\(in)X
3062(particular,)X
3454(timer)X
3673(interrupts)X
775 1236(and)N
924(\256le)X
1059(input\).)X
10 f
555 1360(g)N
1 f
775(Query)X
1016(the)X
1146(status)X
1369(of)X
1464(event)X
1677(sources.)X
10 f
555 1484(g)N
1 f
775(Add)X
948(or)X
1043(remove)X
1329(procedures)X
1736(to)X
1827(be)X
1932(called)X
2165(when)X
2377(an)X
2482(event)X
2695(occurs)X
2946(for)X
3070(a)X
3131(particular)X
3491(widget.)X
10 f
555 1608(g)N
1 f
775(Disable)X
1070(the)X
1204(dispatching)X
1637(of)X
1736(user-initiated)X
2228(events)X
2479(\(keyboard)X
2860(and)X
3012(mouse)X
3267(events\))X
3546(for)X
3673(a)X
3737(particu-)X
775 1704(lar)N
890(widget.)X
10 f
555 1828(g)N
1 f
775(Constrain)X
1140(the)X
1270(dispatching)X
1699(of)X
1794(events)X
2041(to)X
2132(a)X
2193(cascade)X
2488(of)X
2583(``pop-up'')X
2970(widgets.)X
10 f
555 1952(g)N
1 f
775(Call)X
945(the)X
1075(appropriate)X
1498(set)X
1618(of)X
1713(procedures)X
2120(currently)X
2460(registered)X
2829(when)X
3041(an)X
3146(event)X
3359(is)X
3440(read.)X
555 2076(In)N
652(general,)X
957(widgets)X
1255(do)X
1367(not)X
1504(use)X
1645(any)X
1796(of)X
1893(the)X
2025(event)X
2240(manager)X
2567(routines.)X
2918(That)X
3103(is,)X
3207(the)X
3338(normal)X
3611(interface)X
3942(to)X
555 2172(X)N
653(events)X
913(is)X
1007(through)X
1316(the)X
1459(higher-level)X
1919(Translation)X
2355(Manager,)X
2723(which)X
2972(maps)X
3192(sequences)X
3582(of)X
3689(X)X
3786(events)X
555 2268(\(with)N
785(modi\256ers\))X
1191(into)X
1373(procedure)X
1768(calls.)X
2018(In)X
2135(general,)X
2460(applications)X
2931(do)X
3063(not)X
3220(use)X
3381(any)X
3552(of)X
3669(the)X
3820(event)X
555 2364(manager)N
880(routines,)X
1208(except)X
5 f
1478(XtMainLoop)X
1 f
1963(.)X
3 f
555 2556(16.1.)N
775(Adding)X
1076(and)X
1240(Deleting)X
1575(Event)X
1817(Sources)X
1 f
555 2680(While)N
799(most)X
999(applications)X
1454(are)X
1589(driven)X
1842(only)X
2027(by)X
2143(X)X
2234(events,)X
2509(some)X
2723(need)X
2916(to)X
3012(incorporate)X
3440(other)X
3648(sources)X
3938(of)X
555 2776(input)N
774(into)X
949(the)X
1094(X)X
1194(Toolkit)X
1492(event)X
1720(handling)X
2066(philosophy.)X
2539(The)X
2713(event)X
2941(manager)X
3281(provides)X
3621(routines)X
3942(to)X
555 2872(integrate)N
886(timer)X
1095(events)X
1342(and)X
1491(\256le)X
1626(data)X
1795(pending)X
2101(events)X
2348(into)X
2508(this)X
2658(mechanism.)X
555 2996(The)N
715(next)X
890(two)X
1045(functions)X
1396(provide)X
1688(input)X
1893(gathering)X
2249(from)X
2443(\256les.)X
2657(The)X
2817(application)X
3233(registers)X
3554(the)X
3685(\256les)X
3854(with)X
555 3092(the)N
689(read)X
866(routine.)X
1186(When)X
1422(input)X
1630(is)X
1715(pending)X
2025(on)X
2139(one)X
2291(of)X
2389(the)X
2522(\256les,)X
2716(a)X
2780(message)X
3103(event)X
3319(containing)X
3717(the)X
3850(win-)X
555 3188(dow)N
728(that)X
883(is)X
964(to)X
1055(receive)X
1331(the)X
1461(input)X
1665(is)X
1746(generated.)X
3 f
555 3380(16.1.1.)N
841(XtAddInput)X
1 f
555 3504(To)N
675(register)X
961(a)X
1022(new)X
1190(\256le)X
1325(for)X
1449(input,)X
1675(use)X
5 f
1832(XtAddInput)X
1 f
2285(.)X
2351(The)X
2510(de\256nition)X
2871(for)X
2995(this)X
3145(function)X
3461(is:)X
555 3724(void)N
734(XtAddInput\()X
2 f
1188(w)X
1 f
1247(,)X
2 f
1291(source)X
1 f
1525(,)X
2 f
1569(condition)X
1 f
1903(\))X
687 3820(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3916(int)N
2 f
803(source)X
1 f
1037(;)X
687 4012(caddr_t)N
2 f
973(condition)X
1 f
1307(;)X
2 f
555 4260(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 4384(source)N
1 f
1131(Speci\256es)X
1475(the)X
1609(source)X
1864(\256le)X
2003(descriptor)X
2380(in)X
2474(UNIX)X
2717(or)X
2815(other)X
3021(operating)X
3379(system)X
3649(dependent)X
1131 4480(device)N
1383(speci\256cation.)X
2 f
555 4604(condition)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mask)X
1809(that)X
1964(indicates)X
2300(a)X
2361(read,)X
2556(write,)X
2781(or)X
2876(exception)X
3241(condition.)X
555 4728(The)N
5 f
748(XtAddInput)X
1 f
1239(function)X
1571(registers)X
1907(with)X
2102(the)X
2248(X)X
2349(Toolkit)X
2648(read)X
2837(routine)X
3125(a)X
3202(new)X
3386(source)X
3653(of)X
3764(events,)X
555 4824(which)N
797(is)X
883(usually)X
1165(\256le)X
1305(input)X
1514(but)X
1654(can)X
1803(also)X
1972(be)X
2081(\256le)X
2220(output.)X
2516(\(The)X
2708(word)X
2914(``\256le'')X
3169(should)X
3430(be)X
3539(loosely)X
3820(inter-)X
555 4920(preted)N
814(to)X
922(mean)X
1152(any)X
1318(sink)X
1504(or)X
1615(source)X
1882(of)X
1993(data.\))X
5 f
2247(XtAddInput)X
1 f
2738(also)X
2918(speci\256es)X
3259(the)X
3405(conditions)X
3811(under)X
555 5016(which)N
796(the)X
930(source)X
1185(can)X
1333(generate)X
1657(events.)X
1952(When)X
2188(input)X
2396(is)X
2481(pending)X
2791(on)X
2904(this)X
3057(source,)X
3333(a)X
5 f
3415(ClientMessage)X
1 f
555 5112(event)N
768(that)X
923(contains)X
1239(the)X
1369(window,)X
1696(the)X
1826(source,)X
2099(and)X
2248(the)X
2378(condition)X
2734(is)X
2815(returned)X
3130(to)X
3221(the)X
3351(application.)X
3 f
555 5304(16.1.2.)N
841(XtRemoveInput)X
1 f
555 5428(To)N
675(discontinue)X
1104(a)X
1165(source)X
1416(of)X
1511(input,)X
1737(use)X
5 f
1894(XtRemoveInput)X
1 f
2517(.)X
2583(The)X
2742(de\256nition)X
3103(for)X
3227(this)X
3377(function)X
3693(is:)X
3 f
2239 6144(31)N

32 p
%%Page: 32 34
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(void)N
734(XtRemoveInput\()X
2 f
1331(w)X
1 f
1390(,)X
2 f
1434(source)X
1 f
1668(,)X
2 f
1712(condition)X
1 f
2046(\))X
687 864(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 960(int)N
2 f
803(source)X
1 f
1037(;)X
687 1056(caddr_t)N
2 f
973(condition)X
1 f
1307(;)X
2 f
555 1304(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 1428(source)N
1 f
1131(Speci\256es)X
1475(the)X
1609(source)X
1864(\256le)X
2003(descriptor)X
2380(in)X
2474(UNIX)X
2717(or)X
2815(other)X
3021(operating)X
3379(system)X
3649(dependent)X
1131 1524(device)N
1383(speci\256cation.)X
2 f
555 1648(condition)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mask)X
1809(that)X
1964(indicates)X
2300(a)X
2361(read,)X
2556(write,)X
2781(or)X
2876(exception)X
3241(condition.)X
555 1772(The)N
5 f
738(XtRemoveInput)X
1 f
1389(function)X
1711(causes)X
1968(the)X
2104(X)X
2195(read)X
2374(routine)X
2652(to)X
2749(stop)X
2924(gathering)X
3285(input)X
3495(from)X
3694(the)X
3829(input)X
555 1868(source.)N
850(The)X
1009(condition)X
1365(parameter)X
1739(allows)X
1991(the)X
2121(disabling)X
2467(of)X
2562(input)X
2766(only)X
2945(for)X
3069(certain)X
3331(conditions.)X
3 f
555 2060(16.1.3.)N
841(XtAddTimeOut)X
1 f
555 2184(The)N
721(timeout)X
1021(facility)X
1301(noti\256es)X
1590(the)X
1727(application)X
2149(or)X
2251(the)X
2388(widget)X
2657(writer)X
2896(through)X
3199(a)X
3266(message)X
3592(event)X
3811(that)X
3972(a)X
555 2280(speci\256ed)N
898(time)X
1086(interval)X
1386(has)X
1533(elapsed.)X
1849(Timeout)X
2179(values)X
2434(are)X
2571(set)X
2699(for)X
2831(a)X
2900(single)X
3141(widget)X
3410(and)X
3566(are)X
3702(uniquely)X
555 2376(identi\256ed)N
915(by)X
1029(an)X
1138(interval)X
1434(ID.)X
1596(This)X
1778(allows)X
2033(for)X
2160(more)X
2366(than)X
2543(one)X
2695(widget)X
2960(to)X
3054(specify)X
3333(a)X
3397(timeout)X
3693(and)X
3845(for)X
3972(a)X
555 2472(single)N
788(widget)X
1050(to)X
1141(have)X
1329(multiple)X
1647(timeouts.)X
555 2596(To)N
675(create)X
907(a)X
968(timeout)X
1261(value)X
1474(for)X
1598(a)X
1659(widget,)X
1943(use)X
5 f
2100(XtAddTimeOut)X
1 f
2694(.)X
2760(The)X
2919(de\256nition)X
3280(for)X
3404(this)X
3554(function)X
3870(is:)X
555 2816(XtIntervalId)N
1012(XtAddTimeOut\()X
2 f
1599(w)X
1 f
1658(,)X
2 f
1702(interval)X
1 f
1977(\))X
687 2912(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3008(unsigned)N
1027(long)X
2 f
1206(interval)X
1 f
1481(;)X
2 f
555 3256(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 3380(interval)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time)X
1781(interval)X
2073(in)X
2164(milliseconds.)X
555 3504(The)N
5 f
746(XtAddTimeOut)X
1 f
1376(function)X
1706(creates)X
1986(a)X
2061(timeout)X
2368(value)X
2594(for)X
2731(the)X
2874(widget)X
3149(that)X
3317(is)X
3411(identi\256ed)X
3780(by)X
3903(the)X
555 3600(widget)N
832(ID)X
961(and)X
1125(returns)X
1406(an)X
1526(identi\256er)X
1882(for)X
2021(that)X
2191(timer.)X
2459(The)X
2633(timeout)X
2941(value)X
3169(is)X
3265(set)X
3399(to)X
3504(interval.)X
3854(This)X
555 3696(causes)N
809(a)X
872(message)X
1194(event)X
1409(to)X
1502(be)X
1609(generated)X
1975(for)X
2101(the)X
2233(speci\256ed)X
2570(widget)X
2834(window)X
3141(whenever)X
3506(the)X
3638(time)X
3820(inter-)X
555 3792(val)N
685(elapses.)X
3 f
555 3984(16.1.4.)N
841(XtRemoveTimeOut)X
1 f
555 4108(To)N
675(clear)X
868(a)X
929(timeout)X
1222(value,)X
1457(use)X
5 f
1614(XtRemoveTimeOut)X
1 f
2378(.)X
2444(The)X
2603(de\256nition)X
2964(for)X
3088(this)X
3238(function)X
3554(is:)X
555 4328(void)N
734(XtRemoveTimeOut\()X
2 f
1464(timer)X
1 f
1650(\))X
687 4424(XtIntervalId)N
2 f
1144(timer)X
1 f
1330(;)X
2 f
555 4672(timer)N
1 f
1131(Speci\256es)X
1471(the)X
1601(unique)X
1863(identi\256er)X
2204(for)X
2328(the)X
2458(timeout)X
2751(request)X
3027(to)X
3118(be)X
3223(destroyed.)X
5 f
570 4796(XtRemoveTimeOut)N
1 f
1359(removes)X
1682(the)X
1815(timeout,)X
2133(even)X
2324(if)X
2403(it)X
2478(has)X
2620(triggered)X
2963(but)X
3101(has)X
3243(not)X
3381(been)X
3571(fetched)X
3854(with)X
5 f
570 4892(XtNextEvent)N
1 f
1076(.)X
3 f
555 5084(16.2.)N
775(Filtering)X
1126(X)X
1211(Events)X
1 f
555 5208(The)N
718(event)X
935(manager)X
1264(provides)X
1593(two)X
1751(\256lters)X
1978(that)X
2137(can)X
2285(be)X
2394(applied)X
2679(to)X
2773(X)X
2861(user)X
3032(events.)X
3326(These)X
3561(\256lters)X
3787(screen)X
555 5304(out)N
690(events)X
937(that)X
1092(are)X
1221(redundant)X
1595(or)X
1690(that)X
1845(are)X
1974(temporarily)X
2408(unwanted.)X
3 f
555 5496(16.2.1.)N
841(Mouse)X
1112(Motion)X
1408(Compression)X
1 f
555 5620(Widgets)N
879(may)X
1061(have)X
1257(a)X
1326(hard)X
1512(time)X
1700(keeping)X
2008(up)X
2125(with)X
2311(mouse)X
2570(motion)X
2850(events.)X
3148(Further,)X
3458(they)X
3639(usually)X
3923(do)X
555 5716(not)N
699(actually)X
1010(care)X
1187(about)X
1414(every)X
1640(motion)X
1922(event.)X
2187(To)X
2315(throw)X
2550(out)X
2693(redundant)X
3075(motion)X
3356(events,)X
3633(the)X
3771(widget)X
555 5812(class)N
766(\256eld)X
963(compress_motion)X
1630(should)X
1905(be)X
2028(TRUE.)X
2320(All)X
2473(but)X
2626(the)X
2774(last)X
2937(motion)X
3228(event)X
3458(is)X
3556(ignored)X
3864(in)X
3972(a)X
3 f
2239 6144(32)N

33 p
%%Page: 33 35
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(consecutive)N
994(sequence)X
1339(of)X
1435(motion)X
1709(events)X
1957(queued)X
2234(for)X
2359(the)X
2490(widget's)X
2816(window,)X
3144(and)X
3293(only)X
3472(the)X
3602(most)X
3796(recent)X
555 768(motion)N
828(event)X
1041(is)X
1122(delivered.)X
3 f
555 960(16.2.2.)N
841(XtSetSensitive)X
1 f
555 1180(void)N
734(XtSetSensitive\()X
2 f
1288(w)X
1 f
1347(,)X
2 f
1391(sensitive)X
1 f
1695(\))X
687 1276(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 1372(Boolean)N
2 f
1003(sensitive)X
1 f
1307(;)X
2 f
555 1620(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 1744(sensitive)N
1 f
1131(Speci\256es)X
1471(whether)X
1776(or)X
1871(not)X
2006(the)X
2136(widget)X
2398(should)X
2655(receive)X
2931(keyboard)X
3280(and)X
3429(mouse)X
3681(events.)X
555 1868(Many)N
793(widgets,)X
1122(especially)X
1508(those)X
1727(with)X
1917(callback)X
2244(lists)X
2420(that)X
2586(get)X
2727(executed)X
3073(in)X
3175(response)X
3515(to)X
3617(some)X
3836(user-)X
555 1964(initiated)N
869(action)X
1108(\(for)X
1262(example,)X
1606(clicking)X
1914(down)X
2132(or)X
2228(up\),)X
2390(have)X
2579(a)X
2641(mode)X
2860(in)X
2952(which)X
3190(they)X
3365(take)X
3535(on)X
3646(a)X
3708(different)X
555 2060(appearance)N
972(\(for)X
1125(example,)X
1468(greyed)X
1729(out)X
1864(or)X
1959(stippled\))X
2290(and)X
2439(do)X
2549(not)X
2684(respond)X
2984(any)X
3133(more.)X
555 2184(This)N
753(dormant)X
1088(state)X
1291(means)X
1556(the)X
1704(widget)X
1984(is)X
2083(``insensitive''.)X
2661(If)X
2759(a)X
2838(widget)X
3118(is)X
3217(insensitive,)X
3657(the)X
3805(Event)X
555 2280(Manager)N
919(does)X
1132(not)X
1297(dispatch)X
1643(any)X
1822(events)X
2098(to)X
2218(the)X
2377(widget)X
2668(with)X
2876(an)X
3010(event)X
3252(type)X
3455(of)X
5 f
3597(KeyPress)X
1 f
3989(,)X
5 f
570 2376(KeyRelease)N
1 f
1059(,)X
5 f
1141(ButtonPress)X
1 f
1637(,)X
5 f
1718(ButtonRelease)X
1 f
2311(,)X
5 f
2392(MotionNotify)X
1 f
2896(,)X
5 f
2977(EnterNotify)X
1 f
3428(,)X
5 f
3509(LeaveNotify)X
1 f
3989(,)X
5 f
570 2472(FocusIn)N
1 f
899(,)X
943(or)X
5 f
1056(FocusOut)X
1 f
1454(.)X
555 2596(A)N
644(widget)X
910(may)X
1088(be)X
1197(insensitive)X
1601(because)X
1905(its)X
2015(sensitive)X
2350(\256eld)X
2533(is)X
2618(FALSE)X
2913(or)X
3012(one)X
3165(of)X
3264(its)X
3374(parents)X
3653(is)X
3737(insensi-)X
555 2692(tive,)N
739(and,)X
917(thus,)X
1115(the)X
1252(widget's)X
1584(ancestor_sensitive)X
2259(\256eld)X
2445(also)X
2616(is)X
2704(FALSE.)X
3024(A)X
3116(widget)X
3385(may)X
3566(but)X
3708(does)X
3898(not)X
555 2788(nned)N
748(to)X
839(distinguish)X
1249(these)X
1452(two)X
1606(cases)X
1813(visually.)X
5 f
570 2912(XtSetSensitive)N
1 f
1184(\256rst)X
1346(calls)X
5 f
1551(XtSetValues)X
1 f
2077(on)X
2190(the)X
2323(current)X
2597(widget)X
2862(with)X
3044(an)X
3152(argument)X
3510(list)X
3644(specifying)X
555 3008(that)N
720(the)X
860(sensitive)X
1201(\256eld)X
1390(should)X
1657(change)X
1938(to)X
2039(the)X
2179(new)X
2357(value.)X
2624(It)X
2709(then)X
2892(recursively)X
3314(propagates)X
3726(the)X
3865(new)X
555 3104(value)N
795(down)X
1039(the)X
1195(managed)X
1561(children)X
1898(tree)X
2078(by)X
2214(calling)X
5 f
2521(XtSetValues)X
1 f
3070(on)X
3206(each)X
3415(child)X
3640(to)X
3757(set)X
3903(the)X
555 3200(ancestor_sensitive)N
1227(to)X
1322(the)X
1456(new)X
1628(value)X
1845(if)X
1925(the)X
2059(new)X
2231(values)X
2482(for)X
2610(sensitive)X
2945(and)X
3098(ancestor_sensitive)X
3770(are)X
3903(the)X
555 3296(same.)N
5 f
868(XtSetSensitive)X
1 f
1524(thus)X
1738(maintains)X
2149(the)X
2324(invariant)X
2705(if)X
2826(parent)X
3113(has)X
3296(either)X
3563(sensitive)X
3938(or)X
555 3392(ancestor_sensitive)N
1223(FALSE.)X
1536(Then,)X
1761(all)X
1872(children)X
2183(have)X
2371(ancestor_sensitive)X
3039(FALSE.)X
5 f
570 3516(XtSetSensitive)N
1 f
1187(calls)X
5 f
1395(XtSetValues)X
1 f
1924(to)X
2021(change)X
2298(sensitive)X
2635(and)X
2790(ancestor_sensitive.)X
3508(Therefore,)X
3903(the)X
555 3612(widget's)N
887(set_values)X
1283(procedure)X
1663(should)X
1927(take)X
2103(whatever)X
2454(display)X
2738(actions)X
3017(are)X
3153(needed,)X
3452(such)X
3641(as)X
3742(greying)X
555 3708(out)N
690(or)X
785(stippling)X
1117(the)X
1247(widget,)X
1531(when)X
1743(one)X
1892(of)X
1987(these)X
2190(changes.)X
3 f
555 3900(16.3.)N
775(Querying)X
1155(Event)X
1397(Sources)X
1 f
555 4024(The)N
716(event)X
931(manager)X
1258(provides)X
1585(several)X
1858(routines)X
2166(to)X
2258(examine)X
2580(and)X
2730(read)X
2904(events)X
3152(\(including)X
3538(\256le)X
3674(and)X
3824(timer)X
555 4120(events\))N
831(that)X
986(are)X
1115(in)X
1206(the)X
1336(queue.)X
555 4244(The)N
729(next)X
918(three)X
1131(functions)X
1496(handle)X
1768(X)X
1868(Toolkit)X
2165(equivalents)X
2603(of)X
2712(the)X
5 f
2874(XPending)X
1 f
3271(,)X
5 f
3347(XPeekEvent)X
1 f
3884(and)X
5 f
570 4340(XNextEvent)N
1 f
1051(,)X
1095(Xlib)X
1274(calls.)X
3 f
555 4532(16.3.1.)N
841(XtPending)X
1 f
555 4656(To)N
678(determine)X
1056(if)X
1135(there)X
1336(are)X
1468(any)X
1620(events)X
1870(on)X
1983(the)X
2116(input)X
2323(queue,)X
2580(use)X
5 f
2740(XtPending)X
1 f
3162(.)X
3231(The)X
3393(de\256nition)X
3757(for)X
3883(this)X
555 4752(function)N
871(is:)X
555 4972(Boolean)N
871(XtPending\(\))X
555 5192(The)N
5 f
744(XtPending)X
1 f
1200(function)X
1528(\257ushes)X
1806(the)X
1948(X)X
2045(output)X
2305(buffer)X
2553(and)X
2714(returns)X
2992(a)X
3065(nonzero)X
3382(value)X
3607(if)X
3695(there)X
3904(are)X
555 5288(events)N
813(pending)X
1130(from)X
1334(X)X
1430(or)X
1535(other)X
1748(input)X
1962(sources.)X
2279(If)X
2369(there)X
2577(are)X
2716(no)X
2836(events)X
3093(pending,)X
3431(it)X
3513(returns)X
3789(a)X
3860(zero)X
555 5384(value.)N
3 f
2239 6144(33)N

34 p
%%Page: 34 36
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(16.3.2.)N
841(XtPeekEvent)X
1 f
555 796(To)N
676(\257ush)X
870(the)X
1000(X)X
1085(output)X
1333(buffer)X
1569(and)X
1718(return)X
1950(the)X
2080(value)X
2293(from)X
2486(the)X
2616(head)X
2804(of)X
2899(the)X
3029(input)X
3233(queue)X
3465(without)X
3757(remov-)X
555 892(ing)N
690(input)X
894(from)X
1087(the)X
1217(queue,)X
1471(use)X
5 f
1628(XtPeekEvent)X
1 f
2154(.)X
2220(The)X
2379(de\256nition)X
2740(for)X
2864(this)X
3014(function)X
3330(is:)X
555 1112(void)N
734(XtPeekEvent\()X
2 f
1228(event)X
1 f
1414(\))X
687 1208(XEvent)N
978(*)X
2 f
(event)S
1 f
1208(;)X
2 f
555 1456(event)N
1 f
1131(Returns)X
1427(the)X
1557(event)X
1770(information)X
2209(to)X
2300(the)X
2430(speci\256ed)X
2765(event)X
2978(structure.)X
555 1580(If)N
638(no)X
751(X)X
839(input)X
1046(is)X
1130(on)X
1243(the)X
1376(queue,)X
5 f
1651(XtPeekEvent)X
1 f
2202(looks)X
2418(at)X
2507(all)X
2621(other)X
2827(input)X
3034(sources)X
3322(and)X
3474(returns)X
3742(the)X
3874(\256rst)X
555 1676(source)N
810(that)X
969(has)X
1112(input)X
1319(pending.)X
1650(If)X
1733(no)X
1846(input)X
2053(is)X
2137(pending,)X
2468(however,)X
5 f
2835(XtPeekEvent)X
1 f
3386(blocks)X
3641(until)X
3829(input)X
555 1772(is)N
636(available.)X
3 f
555 1964(16.3.3.)N
841(XtNextEvent)X
1 f
555 2088(To)N
681(\257ush)X
880(the)X
1016(X)X
1107(output)X
1361(buffer)X
1603(and)X
1758(return)X
1996(the)X
2132(value)X
2351(from)X
2549(the)X
2684(head)X
2877(of)X
2977(the)X
3112(input)X
3321(queue,)X
3580(use)X
5 f
3742(XtNex-)X
555 2184(tEvent)N
1 f
821(.)X
887(The)X
1046(de\256nition)X
1407(for)X
1531(this)X
1681(function)X
1997(is:)X
555 2404(void)N
734(XtNextEvent\()X
2 f
1228(event)X
1 f
1414(\))X
687 2500(XEvent)N
978(*)X
2 f
(event)S
1 f
1208(;)X
2 f
555 2748(event)N
1 f
1131(Returns)X
1427(the)X
1557(event)X
1770(information)X
2209(to)X
2300(the)X
2430(speci\256ed)X
2765(event)X
2978(structure.)X
555 2872(If)N
645(no)X
765(input)X
979(is)X
1070(on)X
1190(the)X
1330(X)X
1425(input)X
1639(queue,)X
5 f
1921(XtNextEvent)X
1 f
2459(looks)X
2682(at)X
2778(the)X
2917(other)X
3129(input)X
3342(sources)X
3636(or)X
3740(timeout)X
555 2968(values)N
810(and)X
967(returns)X
1241(input)X
1453(generated)X
1825(by)X
1943(them.)X
2194(If)X
2282(no)X
2399(input)X
2610(of)X
2712(any)X
2868(sort)X
3029(is)X
3117(available,)X
5 f
3505(XtNextEvent)X
1 f
555 3064(blocks)N
807(until)X
992(an)X
1097(event)X
1310(can)X
1454(be)X
1559(returned.)X
555 3188(Because)N
871(the)X
1002(toolkit)X
1257(event)X
1471(processing)X
1870(gives)X
2078(preference)X
2470(to)X
2561(X)X
2646(events)X
2893(over)X
3071(source)X
3322(and)X
3471(timeout)X
3764(events,)X
555 3284(a)N
626(call)X
786(to)X
5 f
905(XtPeekEvent)X
1 f
1463(followed)X
1808(by)X
1928(a)X
1999(call)X
2159(to)X
5 f
2278(XtNextEvent)X
1 f
2816(may)X
2999(not)X
3143(return)X
3384(the)X
3523(same)X
3735(event)X
3957(if)X
5 f
570 3380(XtPeekEvent)N
1 f
1118(returns)X
1384(a)X
1445(source)X
1696(or)X
1791(timeout)X
2084(event.)X
3 f
555 3572(16.4.)N
775(Adding)X
1076(and)X
1240(Removing)X
1643(Event)X
1885(Handlers)X
1 f
555 3696(Event)N
783(handlers)X
1103(are)X
1232(procedures)X
1639(that)X
1794(are)X
1923(called)X
2156(when)X
2368(a)X
2429(speci\256ed)X
2764(set)X
2884(of)X
2979(events)X
3226(occurs)X
3477(in)X
3568(a)X
3629(widget.)X
555 3916(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtEventHandler)X
1 f
1666(\(\)\);)X
687 4012(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 4108(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
687 4204(XEvent)N
978(*)X
2 f
(event)S
1 f
1208(;)X
2 f
555 4452(w)N
1 f
1131(The)X
1290(widget)X
1552(that)X
1707(this)X
1857(event)X
2070(handler)X
2356(was)X
2514(registered)X
2883(with.)X
2 f
555 4576(closure)N
1 f
1131(Client)X
1393(speci\256c)X
1707(information)X
2169(registered)X
2561(with)X
2763(the)X
2916(event)X
3151(handler,)X
3481(this)X
3653(is)X
3756(usually)X
1131 4672(NULL)N
1387(if)X
1463(the)X
1593(event)X
1806(handler)X
2092(is)X
2173(registered)X
2542(by)X
2652(the)X
2782(widget)X
3044(itself.)X
2 f
555 4796(event)N
1 f
1131(The)X
1290(triggering)X
1660(event.)X
3 f
555 4988(16.4.1.)N
841(XtAddEventHandler)X
1 f
555 5112(To)N
679(register)X
968(an)X
1076(event)X
1292(handler)X
1581(procedure)X
1957(with)X
2139(the)X
2272(dispatch)X
2591(mechanism,)X
3040(use)X
5 f
3200(XtAddEventHandler)X
1 f
3989(.)X
555 5208(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
3 f
2239 6144(34)N

35 p
%%Page: 35 37
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 768(void)N
734(XtAddEventHandler\()X
2 f
1491(w)X
1 f
1550(,)X
2 f
1594(event_mask)X
1 f
2004(,)X
2 f
2048(nonmaskable)X
1 f
2512(,)X
2 f
2556(proc)X
1 f
2717(,)X
2 f
2761(closure)X
1 f
3020(\))X
687 864(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 960(XtEventMask)N
2 f
1198(event_mask)X
1 f
1608(;)X
687 1056(Boolean)N
2 f
1003(nonmaskable)X
1 f
1467(;)X
687 1152(XtEventHandler)N
2 f
1286(proc)X
1 f
1447(;)X
687 1248(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
2 f
555 1496(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(for)X
1987(which)X
2224(this)X
2374(event)X
2587(handler)X
2873(is)X
2954(being)X
3172(registered.)X
2 f
555 1620(event_mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(event)X
1814(mask)X
2022(to)X
2113(call)X
2263(this)X
2413(procedure)X
2786(for.)X
2 f
555 1744(nonmaskable)N
1 f
1131(Speci\256es)X
1486(whether)X
1806(this)X
1971(procedure)X
2358(should)X
2629(be)X
2748(called)X
2995(on)X
3119(the)X
3263(nonmaskable)X
3764(events.)X
1131 1840(These)N
1404(are)X
1574(event)X
1828(of)X
1964(type)X
5 f
2197(GraphicsExpose)X
1 f
2858(,)X
5 f
2961(NoExpose)X
1 f
3382(,)X
5 f
3485(CreateNotify)X
1 f
3989(,)X
5 f
1146 1936(MapRequest)N
1 f
1660(,)X
5 f
1762(Con\256gureRequest)X
1 f
2486(,)X
5 f
2588(ResizeRequest)X
1 f
3199(,)X
5 f
3300(CirculateRequest)X
1 f
3989(,)X
5 f
1146 2032(SelectionClear)N
1 f
1732(,)X
5 f
1794(SelectionRequest)X
1 f
2499(,)X
5 f
2561(ClientMessage)X
1 f
3157(,)X
3201(and)X
5 f
3368(MappingNotify)X
1 f
3945(.)X
2 f
555 2156(proc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(client)X
1820(event)X
2033(handler)X
2319(procedure.)X
2 f
555 2280(closure)N
1 f
1131(Speci\256es)X
1471(additional)X
1847(data)X
2016(to)X
2107(be)X
2212(passed)X
2468(to)X
2559(the)X
2689(client's)X
2971(event)X
3184(handler.)X
555 2404(The)N
5 f
736(XtAddEventHandler)X
1 f
1551(function)X
1871(registers)X
2195(a)X
2260(procedure)X
2637(with)X
2820(the)X
2954(dispatch)X
3273(mechanism)X
3700(that)X
3858(is)X
3942(to)X
555 2500(be)N
661(called)X
895(when)X
1108(an)X
1214(event)X
1428(that)X
1584(matches)X
1896(the)X
2027(mask)X
2236(occurs)X
2488(on)X
2598(the)X
2728(speci\256ed)X
3063(widget.)X
3369(If)X
3449(the)X
3579(procedure)X
3952(is)X
555 2596(already)N
843(registered,)X
1241(the)X
1378(speci\256ed)X
1720(mask)X
1935(is)X
2023(OR'ed)X
2286(into)X
2453(the)X
2589(existing)X
2897(mask.)X
3155(If)X
3241(the)X
3377(widget)X
3645(is)X
3732(realized)X
5 f
570 2692(XtAddEventHandler)N
1 f
1381(calls)X
5 f
1583(XSelectInput)X
1 f
2121(if)X
2197(necessary.)X
3 f
555 2884(16.4.2.)N
841(XtRemoveEventHandler)X
1 f
555 3008(To)N
678(remove)X
967(a)X
1031(previously)X
1428(registered)X
1800(event)X
2015(handler,)X
2325(use)X
5 f
2484(XtRemoveEventHandler)X
1 f
3443(.)X
3511(The)X
3672(de\256nition)X
555 3104(for)N
679(this)X
829(function)X
1145(is:)X
555 3324(void)N
734(XtRemoveEventHandler\()X
2 f
1634(w)X
1 f
1693(,)X
2 f
1737(event_mask)X
1 f
2147(,)X
2 f
2191(nonmaskable)X
1 f
2655(,)X
2 f
2699(proc)X
1 f
2860(,)X
2 f
2904(closure)X
1 f
3163(\))X
687 3420(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3516(XtEventMask)N
2 f
1198(event_mask)X
1 f
1608(;)X
687 3612(Boolean)N
2 f
1003(nonmaskable)X
1 f
1467(;)X
687 3708(XtEventHandler)N
2 f
1286(proc)X
1 f
1447(;)X
687 3804(caddr_t)N
2 f
973(closure)X
1 f
1232(;)X
2 f
555 4052(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(for)X
1987(which)X
2224(this)X
2374(procedure)X
2747(is)X
2828(registered.)X
2 f
555 4176(event_mask)N
555 4300(nonmaskable)N
1 f
1131(Speci\256es)X
1471(the)X
1601(events)X
1848(to)X
1939(unregister)X
2313(this)X
2463(procedure)X
2836(for)X
2 f
555 4424(proc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(event)X
1814(handler)X
2100(procedure)X
2473(registered.)X
2 f
555 4548(closure)N
1 f
1131(Speci\256es)X
1471(the)X
1601(closure)X
1877(registered.)X
5 f
570 4672(XtRemoveEventHandler)N
1 f
1558(stops)X
1768(the)X
1904(speci\256ed)X
2245(procedure)X
2624(from)X
2823(receiving)X
3179(the)X
3315(speci\256ed)X
3656(events.)X
3953(If)X
555 4768(the)N
685(widget)X
947(is)X
1028(realized)X
5 f
1347(XtRemoveEventHandler)X
1 f
2328(calls)X
5 f
2530(XSelectInput)X
1 f
3068(if)X
3144(necessary.)X
555 4892(To)N
703(stop)X
900(a)X
989(procedure)X
1390(from)X
1611(receiving)X
1989(any)X
2165(events)X
2439(\(which)X
2732(will)X
2919(remove)X
3232(it)X
3331(from)X
3551(the)X
3708(widget's)X
555 4988(event_table)N
1003(entirely\),)X
1365(call)X
5 f
1551(XtRemoveEventHandler)X
1 f
2550(with)X
2747(an)X
2870(event_mask)X
3331(of)X
5 f
3462(AllEvents)X
1 f
3884(and)X
555 5084(with)N
734(nonmaskable)X
1221(TRUE.)X
3 f
555 5276(16.5.)N
775(Constraining)X
1291(Events)X
1567(to)X
1662(a)X
1728(Cascade)X
2062(of)X
2157(Widgets)X
1 f
555 5400(Some)N
786(widgets)X
1090(lock)X
1272(out)X
1415(any)X
1572(user)X
1748(input)X
1960(to)X
2059(the)X
2197(application)X
2620(except)X
2880(input)X
3092(to)X
3190(that)X
3352(widget.)X
3665(These)X
3904(are)X
555 5496(called)N
788(``modal'')X
1147(widgets.)X
555 5620(When)N
800(a)X
874(modal)X
1130(menu)X
1361(or)X
1469(modal)X
1725(dialog)X
1981(box)X
2148(is)X
2242(``popped-up'',)X
2791(user)X
2971(events)X
3230(\(keyboard)X
3620(and)X
3781(mouse)X
555 5716(events\))N
836(that)X
996(occur)X
1218(outside)X
1500(the)X
1635(modal)X
1883(widget)X
2150(should)X
2412(be)X
2522(delivered)X
2877(to)X
2973(the)X
3108(modal)X
3355(widget)X
3621(or)X
3720(ignored.)X
555 5812(In)N
656(no)X
772(case)X
951(should)X
1214(user)X
1388(events)X
1641(be)X
1752(delivered)X
2108(to)X
2204(widgets)X
2505(outside)X
2787(of)X
2887(the)X
3022(modal)X
3270(widgets.)X
3593(Menus)X
3859(may)X
3 f
2239 6144(35)N

36 p
%%Page: 36 38
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(pop-up)N
836(submenus)X
1220(and)X
1379(dialog)X
1632(boxes)X
1869(may)X
2053(pop-up)X
2334(further)X
2605(dialog)X
2858(dialog)X
3111(boxes)X
3348(to)X
3449(create)X
3691(a)X
3762(pop-up)X
555 768(``cascade''.)N
995(In)X
1097(this)X
1254(case,)X
1456(user)X
1631(events)X
1885(should)X
2149(be)X
2261(delivered)X
2618(to)X
2716(one)X
2872(of)X
2974(several)X
3252(modal)X
3502(widgets)X
3805(in)X
3903(the)X
555 864(cascade.)N
555 988(Display-related)N
1128(events)X
1382(should)X
1646(be)X
1758(delivered)X
2115(outside)X
2399(the)X
2536(modal)X
2786(cascade)X
3088(so)X
3195(that)X
3357(expose)X
3630(events)X
3884(and)X
555 1084(the)N
696(like)X
862(keep)X
1061(the)X
1202(application's)X
1691(display)X
1979(up-to-date.)X
2417(Any)X
2600(event)X
2823(that)X
2988(occurs)X
3249(within)X
3507(the)X
3647(cascade)X
3952(is)X
555 1180(delivered)N
908(normally.)X
1296(Events)X
1561(that)X
1719(are)X
1851(delivered)X
2204(to)X
2298(the)X
2431(most)X
2628(recent)X
2868(modal)X
3114(widget)X
3379(in)X
3473(the)X
3606(cascade)X
3904(are)X
555 1276(if)N
642(they)X
827(occur)X
1055(outside)X
1343(the)X
1484(cascade)X
1790(are:)X
5 f
1973(KeyPress)X
1 f
2365(,)X
5 f
2438(KeyRelease)X
1 f
2927(,)X
5 f
2999(ButtonPress)X
1 f
3495(,)X
3549(and)X
5 f
3726(Button-)X
555 1372(Release)N
1 f
892(.)X
555 1496(Events)N
837(that)X
1012(are)X
1161(ignored)X
1472(if)X
1568(they)X
1762(occur)X
1999(outside)X
2296(the)X
2446(cascade)X
2761(are:)X
5 f
2953(MotionNotify)X
1 f
3457(,)X
5 f
3538(EnterNotify)X
1 f
3989(,)X
5 f
570 1592(LeaveNotify)N
1 f
1050(,)X
5 f
1148(FocusIn)X
1 f
1477(,)X
5 f
1575(FocusOut)X
1 f
1973(,)X
5 f
2071(KeymapNotify)X
1 f
2634(,)X
5 f
2732(PropertyNotify)X
1 f
3305(,)X
5 f
3403(SelectionClear)X
1 f
3989(,)X
5 f
570 1688(SelectionRequest)N
1 f
1275(,)X
5 f
1339(ColormapNotify)X
1 f
1959(,)X
5 f
2023(ClientMessage)X
1 f
2619(,)X
2665(and)X
5 f
2834(MappingNotify)X
1 f
3411(.)X
3479(All)X
3616(other)X
3820(event)X
555 1784(are)N
684(delivered)X
1034(normally.)X
555 1908(Modal)N
808(widgets)X
1105(use)X
1245(the)X
1376(procedures)X
5 f
1802(XtAddGrab)X
1 f
2277(and)X
5 f
2445(XtRemoveGrab)X
1 f
3090(to)X
3182(constrain)X
3527(user)X
3695(events)X
3942(to)X
555 2004(a)N
616(modal)X
859(cascade)X
1154(and)X
1303(to)X
1394(subsequently)X
1876(remove)X
2162(a)X
2223(grab)X
2401(when)X
2613(the)X
2743(modal)X
2986(widget)X
3248(goes)X
3431(away.)X
3 f
555 2196(16.5.1.)N
841(XtAddGrab)X
1 f
555 2320(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 2540(void)N
734(XtAddGrab\()X
2 f
1177(w)X
1 f
1236(,)X
2 f
1280(exclusive)X
1 f
1603(\))X
687 2636(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 2732(Boolean)N
2 f
1003(exclusive)X
1 f
1326(;)X
2 f
555 2980(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(add)X
2103(to)X
2194(the)X
2324(modal)X
2567(cascade.)X
2 f
555 3104(exclusive)N
1 f
1131(Speci\256es)X
1474(if)X
1553(user)X
1724(events)X
1974(should)X
2234(be)X
2342(dispatched)X
2744(exclusively)X
3170(to)X
3263(this)X
3415(widget)X
3679(or)X
3776(also)X
3942(to)X
1131 3200(previous)N
1456(widgets)X
1752(in)X
1843(the)X
1973(cascade.)X
5 f
570 3324(XtAddGrab)N
1 f
1044(appends)X
1354(the)X
1484(widget)X
1746(to)X
1837(the)X
1967(modal)X
2210(cascade.)X
555 3448(When)N
5 f
809(XtDispatchEvent)X
1 f
1502(tries)X
1680(to)X
1775(dispatch)X
2095(a)X
2160(user)X
2332(event)X
2549(and)X
2702(when)X
2918(at)X
3008(least)X
3195(one)X
3347(modal)X
3593(widget)X
3858(is)X
3942(in)X
555 3544(the)N
693(widget)X
963(cascade,)X
1288(it)X
1368(\256rst)X
1535(determines)X
1952(if)X
2036(the)X
2174(the)X
2312(event)X
2533(should)X
2798(be)X
2910(delivered.)X
3311(It)X
3394(starts)X
3609(at)X
3702(the)X
3839(most)X
555 3640(recent)N
800(cascade)X
1103(entry)X
1314(and)X
1471(follows)X
1765(the)X
1903(cascade)X
2206(up)X
2324(to)X
2423(and)X
2580(including)X
2944(the)X
3082(most)X
3284(recent)X
3528(cascade)X
3830(entry)X
555 3736(added)N
787(with)X
966(exclusive)X
1321(TRUE.)X
555 3860(This)N
738(subset)X
984(of)X
1083(the)X
1217(modal)X
1464(cascade)X
1763(is)X
1848(the)X
1981(active)X
2217(subset.)X
2506(User)X
2696(events)X
2946(that)X
3104(occur)X
3324(outside)X
3604(the)X
3737(widgets)X
555 3956(in)N
659(this)X
822(subset)X
1077(are)X
1219(ignored.)X
1545(Modal)X
1810(menus)X
2075(generally)X
2438(add)X
2600(a)X
2674(submenu)X
3027(widget)X
3302(to)X
3405(the)X
3547(cascade)X
3854(with)X
555 4052(exclusive)N
915(FALSE.)X
1255(Modal)X
1512(dialog)X
1760(boxes)X
1992(that)X
2152(wish)X
2345(to)X
2441(restrict)X
2713(user)X
2886(input)X
3095(to)X
3191(the)X
3326(most)X
3525(deeply)X
3786(nested)X
555 4148(dialog)N
798(box)X
952(add)X
1101(a)X
1162(subdialog)X
1527(widget)X
1789(to)X
1880(the)X
2010(cascade)X
2305(with)X
2484(exclusive)X
2839(TRUE.)X
555 4272(User)N
749(events)X
1003(that)X
1165(occur)X
1389(within)X
1643(the)X
1779(active)X
2018(subset)X
2266(are)X
2401(delivered)X
2757(to)X
2854(the)X
2990(appropriate)X
3419(widget,)X
3709(which)X
3952(is)X
555 4368(usually)N
832(a)X
893(child)X
1092(or)X
1187(further)X
1448(descendant)X
1861(of)X
1956(the)X
2086(modal)X
2329(widget.)X
555 4492(Regardless)N
973(of)X
1078(where)X
1324(on)X
1444(the)X
1584(screen)X
1840(they)X
2024(occur,)X
5 f
2290(ButtonRelease)X
1 f
2914(events)X
3170(are)X
3308(always)X
3583(delivered)X
3942(to)X
555 4588(the)N
685(most)X
879(recent)X
1116(widget)X
1378(in)X
1469(the)X
1599(cascade.)X
2197 4764(Note)N
775 4900(This)N
954(routine)X
1226(currently)X
1566(is)X
1647(not)X
1782(yet)X
1912(implemented.)X
3 f
555 5172(16.5.2.)N
841(XtRemoveGrab)X
1 f
555 5296(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 5516(void)N
734(XtRemoveGrab\()X
2 f
1320(w)X
1 f
1379(\))X
687 5612(Widget)N
2 f
969(w)X
1 f
1028(;)X
3 f
2239 6144(36)N

37 p
%%Page: 37 39
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
2 f
555 672(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(to)X
1954(remove)X
2240(from)X
2433(the)X
2563(modal)X
2806(cascade.)X
5 f
570 796(XtRemoveGrab)N
1 f
1217(removes)X
1540(widgets)X
1839(from)X
2035(the)X
2168(modal)X
2414(cascade)X
2712(starting)X
3002(at)X
3091(the)X
3224(most)X
3420(recent)X
3659(widget)X
3923(up)X
555 892(to)N
646(and)X
795(including)X
1151(the)X
1281(speci\256ed)X
1616(widget.)X
1922(It)X
1998(issues)X
2230(an)X
2335(error)X
2527(if)X
2603(w)X
2688(is)X
2769(not)X
2904(on)X
3014(the)X
3144(modal)X
3387(cascade.)X
2197 1068(Note)N
775 1204(This)N
954(routine)X
1226(currently)X
1566(is)X
1647(not)X
1782(yet)X
1912(implemented.)X
3 f
555 1476(16.6.)N
775(Dispatching)X
1247(Events)X
1 f
555 1600(The)N
716(X)X
803(Toolkit)X
1087(provides)X
1413(functions)X
1764(that)X
1920(dispatch)X
2237(events)X
2485(to)X
2577(widgets)X
2874(or)X
2970(other)X
3174(application)X
3590(code.)X
3801(Every)X
555 1696(client)N
782(interested)X
1155(in)X
1254(events)X
1509(on)X
1627(a)X
1696(widget)X
1966(uses)X
5 f
2165(XtAddEventHandler)X
1 f
2984(to)X
3082(register)X
3375(which)X
3619(events)X
3873(it)X
3952(is)X
555 1792(interested)N
927(in)X
1025(and)X
1181(a)X
1249(procedure)X
1628(\(event)X
1876(handler\))X
2197(that)X
2358(is)X
2445(to)X
2542(be)X
2653(called)X
2892(when)X
3110(the)X
3246(event)X
3465(happens)X
3781(in)X
3878(that)X
555 1888(window.)N
555 2012(When)N
803(an)X
924(event)X
1153(is)X
1250(receeved,)X
1622(it)X
1710(is)X
1807(passed)X
2079(to)X
2186(a)X
2263(dispatching)X
2708(procedure.)X
3119(This)X
3314(procedure)X
3703(calls)X
3903(the)X
555 2108(appropriate)N
1003(event)X
1241(handlers)X
1586(and)X
1760(passes)X
2031(them)X
2255(the)X
2410(widget,)X
2719(the)X
2874(event,)X
3133(and)X
3306(client)X
3549(speci\256c)X
3864(data)X
555 2204(registered)N
932(with)X
1118(each)X
1308(procedure)X
1688(\(a)X
1785(``closure''\).)X
2257(If)X
2344(there)X
2549(are)X
2685(no)X
2802(handlers)X
3129(for)X
3260(that)X
3422(event)X
3642(registered,)X
555 2300(the)N
685(event)X
898(is)X
979(ignored)X
1270(and)X
1419(the)X
1549(dispatcher)X
1933(simply)X
2196(returns.)X
555 2424(The)N
714(order)X
921(in)X
1012(which)X
1249(the)X
1379(handlers)X
1699(are)X
1828(called)X
2061(is)X
2142(not)X
2277(de\256ned.)X
3 f
555 2616(16.6.1.)N
841(XtDispatchEvent)X
1 f
555 2740(To)N
681(send)X
869(events)X
1121(to)X
1217(registered)X
1591(functions)X
1946(and)X
2100(widgets,)X
2423(use)X
5 f
2585(XtDispatchEvent)X
1 f
3252(.)X
3323(Normally,)X
3710(this)X
3865(pro-)X
555 2836(cedure)N
815(is)X
900(not)X
1039(called)X
1276(by)X
1390(client)X
1613(applications)X
2066(\(see)X
2233(``XtMainLoop''\).)X
2908(The)X
3071(de\256nition)X
3436(for)X
3564(this)X
3717(function)X
555 2932(is:)N
555 3152(void)N
734(XtDispatchEvent\()X
2 f
1370(event)X
1 f
1556(\))X
687 3248(XEvent)N
978(*)X
2 f
(event)S
1 f
1208(;)X
2 f
555 3496(event)N
1 f
1131(Speci\256es)X
1476(a)X
1542(pointer)X
1819(to)X
1915(the)X
2050(event)X
2268(structure)X
2603(that)X
2763(is)X
2848(to)X
2943(be)X
3052(dispatched)X
3455(to)X
3550(the)X
3684(appropri-)X
1131 3592(ate)N
1256(event)X
1469(handler.)X
555 3716(The)N
5 f
744(XtDispatchEvent)X
1 f
1444(function)X
1771(sends)X
1999(those)X
2218(events)X
2476(to)X
2578(those)X
2797(event)X
3021(handler)X
3318(functions)X
3679(that)X
3845(have)X
555 3812(been)N
804(previously)X
1259(registered)X
1689(with)X
1929(the)X
2120(dispatch)X
2497(routine.)X
2874(The)X
3094(most)X
3348(common)X
3739(use)X
3938(of)X
5 f
570 3908(XtDispatchEvent)N
1 f
1262(is)X
1346(to)X
1440(dispatch)X
1759(events)X
2009(acquired)X
2337(with)X
2519(the)X
5 f
2670(XtNextEvent)X
1 f
3200(or)X
5 f
3315(XtPeekEvent)X
1 f
3865(pro-)X
555 4004(cedure.)N
833(However,)X
1198(it)X
1270(can)X
1414(also)X
1578(be)X
1683(used)X
1866(to)X
1957(dispatch)X
2273(user-constructed)X
2876(events.)X
3 f
555 4196(16.6.2.)N
841(XtMainLoop)X
1 f
555 4320(Applications)N
1028(normally)X
1369(process)X
1654(input)X
1858(within)X
2106(the)X
2236(X)X
2321(Toolkit)X
2604(by)X
2714(calling)X
2977(the)X
5 f
3125(XtMainLoop)X
1 f
3632(routine.)X
555 4444(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 4664(void)N
734(XtMainLoop\(\))X
5 f
570 4884(XtMainLoop)N
1 f
1091(reads)X
1312(the)X
1456(next)X
1644(incoming)X
2014(\256le,)X
2185(timer,)X
2430(or)X
2539(X)X
2638(event.)X
2909(Then,)X
3148(it)X
3234(dispatches)X
3636(this)X
3799(to)X
3903(the)X
555 4980(appropriate)N
980(registered)X
1351(procedure.)X
1770(This)X
1951(is)X
2034(the)X
2166(main)X
2367(loop)X
2548(of)X
2645(X)X
2731(Toolkit)X
3015(applications,)X
3487(and,)X
3659(as)X
3755(such,)X
3961(it)X
555 5076(does)N
738(not)X
873(return.)X
1149(Applications)X
1622(are)X
1751(expected)X
2086(to)X
2177(exit)X
2332(directly)X
2624(in)X
2715(response)X
3044(to)X
3135(some)X
3343(user)X
3511(action.)X
3 f
555 5268(17.)N
709(Widget)X
1005(Visibility)X
1371(and)X
1535(Exposure)X
1 f
555 5392(Every)N
796(primitive)X
1152(widget)X
1423(\(and)X
1610(some)X
1827(composite)X
2221(widgets\))X
2555(displays)X
2875(data)X
3053(on)X
3172(the)X
3311(screen)X
3565(by)X
3683(means)X
3938(of)X
555 5488(raw)N
717(X)X
811(calls.)X
1048(Widgets)X
1373(cannot)X
1639(simply)X
1911(write)X
2123(to)X
2223(the)X
2362(screen)X
2617(and)X
2775(forget)X
3016(what)X
3218(they've)X
3513(done.)X
3737(That)X
3930(is,)X
555 5584(widgets)N
858(must)X
1059(keep)X
1254(enough)X
1542(state)X
1733(to)X
1831(redisplay)X
2183(the)X
2320(window)X
2632(\(or)X
2763(parts)X
2963(of)X
3065(it\))X
3173(if)X
3256(a)X
3324(portion)X
3607(is)X
3694(obscured)X
555 5680(and)N
704(then)X
878(re-exposed.)X
3 f
2239 6144(37)N

38 p
%%Page: 38 40
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(17.1.)N
775(Widget)X
1071(Redisplay:)X
1494(the)X
1633(expose)X
1904(procedure)X
1 f
555 796(typedef)N
841(void)X
1020(\(*)X
2 f
1093(XtExposeProc)X
1 f
1597(\)\(\);)X
687 892(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 988(XEvent)N
978(*)X
2 f
(event)S
1 f
1208(;)X
555 1112(Redisplay)N
932(of)X
1029(a)X
1092(widget)X
1356(upon)X
1556(exposure)X
1897(is)X
1980(the)X
2112(responsibility)X
2617(of)X
2713(the)X
2844(expose)X
3111(procedure)X
3485(in)X
3577(the)X
3708(widget's)X
555 1208(class)N
751(record.)X
1044(A)X
1132(small)X
1349(simple)X
1610(widget)X
1875(\(for)X
2031(example,)X
2377(Label\))X
2632(may)X
2809(ignore)X
3058(the)X
3190(bounding)X
3547(box)X
3703(informa-)X
555 1304(tion)N
724(in)X
824(the)X
962(Expose)X
1251(event)X
1472(and)X
1629(just)X
1787(redisplay)X
2140(the)X
2278(entire)X
2509(window.)X
2844(A)X
2937(more)X
3148(complicated)X
3610(widget)X
3880(\(for)X
555 1400(example,)N
899(Text\))X
1113(may)X
1288(use)X
1428(the)X
1559(bounding)X
1915(box)X
2070(information)X
2510(to)X
2602(minimize)X
2960(the)X
3091(amount)X
3379(of)X
3474(calculation)X
3884(and)X
555 1496(redisplay)N
903(it)X
978(does.)X
1208(The)X
1370(expose)X
1639(procedure)X
2015(is)X
2099(responsible)X
2525(for)X
2652(exposure)X
2994(of)X
3092(all)X
3206(superclass)X
3591(data)X
3762(as)X
3859(well)X
555 1592(as)N
650(its)X
756(own,)X
951(because,)X
1273(in)X
1364(general,)X
1667(this)X
1817(operation)X
2172(cannot)X
2429(be)X
2534(cleanly)X
2811(broken)X
3077(up.)X
555 1716(However,)N
930(it)X
1012(often)X
1225(possible)X
1546(to)X
1647(anticipate)X
2023(the)X
2163(display)X
2450(needs)X
2682(of)X
2787(several)X
3067(levels)X
3304(of)X
3408(subclassing.)X
3889(For)X
555 1812(example,)N
906(rather)X
1141(than)X
1323(separate)X
1641(display)X
1926(procedures)X
2341(for)X
2472(the)X
2609(widgets)X
2912(Label,)X
3164(Command,)X
3583(and)X
3739(Toggle,)X
555 1908(you)N
709(could)X
927(write)X
1130(a)X
1191(single)X
1424(display)X
1701(routine)X
1973(in)X
2064(Label)X
2287(that)X
2442(uses)X
2615(``display)X
2950(state'')X
3192(\256elds)X
3405(like:)X
843 2080(Boolean)N
1159(invert)X
843 2176(Boolean)N
1159(highlight)X
843 2272(Dimension)N
1252(highlight_width)X
555 2416(Label)N
788(would)X
1040(have)X
1238(invert)X
1476(and)X
1635(highlight)X
1986(always)X
2261(FALSE)X
2561(and)X
2719(highlight_width)X
3337(zero\(0\).)X
3643(Command)X
555 2512(would)N
805(dynamically)X
1272(set)X
1400(highlight)X
1750(and)X
1907(highlight_width,)X
2524(but)X
2667(it)X
2747(would)X
2997(leave)X
3212(invert)X
3447(always)X
3720(FALSE.)X
555 2608(Finally,)N
850(Toggle)X
1122(would)X
1364(dynamically)X
1823(set)X
1943(all)X
2054(three.)X
555 2732(In)N
662(this)X
824(case,)X
1031(the)X
1173(expose)X
1451(procedures)X
1870(for)X
2006(Command)X
2408(and)X
2568(Toggle)X
2851(inherit)X
3115(their)X
3310(superclass's)X
3767(expose)X
555 2828(procedure.)N
950(See)X
1099(``Inheriting)X
1527(Superclass)X
1925(Operations''.)X
3 f
555 3020(17.2.)N
775(Exposure)X
1154(Compression)X
1 f
555 3144(Many)N
796(widgets)X
1106(are)X
1249(so)X
1363(simple)X
1635(that)X
1804(they)X
1992(always)X
2272(redisplay)X
2631(the)X
2774(entire)X
3010(widget)X
3285(in)X
3389(their)X
3586(expose)X
3865(pro-)X
555 3240(cedure.)N
863(These)X
1103(widgets)X
1407(do)X
1524(not)X
1666(care)X
1841(about)X
2066(getting)X
2341(partial)X
2596(expose)X
2869(events.)X
3167(If)X
3254(the)X
3391(compress_expose)X
555 3336(\256eld)N
746(in)X
849(the)X
991(widget)X
1265(class)X
1470(structure)X
1812(is)X
1905(TRUE,)X
2191(the)X
2332(Event)X
2571(Manager)X
2916(calls)X
3111(the)X
3252(widget's)X
3588(expose)X
3865(pro-)X
555 3432(cedure)N
811(only)X
990(for)X
1114(Expose)X
1395(events)X
1642(in)X
1733(which)X
1970(the)X
2100(expose)X
2366(count)X
2584(is)X
2665(zero)X
2838(\(0\).)X
3 f
555 3624(17.3.)N
775(Widget)X
1071(Visibility)X
1 f
555 3748(Some)N
786(widgets)X
1090(may)X
1272(use)X
1419(substantial)X
1827(computing)X
2235(resources)X
2596(to)X
2695(display)X
2979(data.)X
3199(However,)X
3571(this)X
3728(effort)X
3952(is)X
555 3844(wasted)N
823(if)X
901(the)X
1033(widget)X
1297(is)X
1380(not)X
1517(actually)X
1821(visible)X
2081(on)X
2193(the)X
2325(screen.)X
2617(That)X
2803(is,)X
2908(the)X
3039(widget)X
3302(may)X
3477(be)X
3583(obscured)X
3923(by)X
555 3940(another)N
841(application)X
1256(or)X
1351(iconi\256ed.)X
555 4064(The)N
716(visible)X
976(\256eld)X
1156(in)X
1248(the)X
1379(Core)X
1573(widget)X
1836(structure)X
2167(provides)X
2493(a)X
2555(hint)X
2716(to)X
2808(the)X
2939(widget)X
3202(that)X
3358(it)X
3431(need)X
3620(not)X
3756(display)X
555 4160(data.)N
782(This)X
975(\256eld)X
1168(is)X
1263(guaranteed)X
1685(TRUE)X
1951(\(by)X
2104(the)X
2248(time)X
2442(an)X
2560(ExposeNotify)X
3084(event)X
3310(is)X
3404(processed\))X
3814(if)X
3903(the)X
555 4256(widget)N
817(is)X
898(visible)X
1156(and)X
1305(is)X
1386(usually)X
1663(FALSE)X
1954(if)X
2030(the)X
2160(widget)X
2422(is)X
2503(not)X
2638(visible.)X
555 4380(Widgets)N
888(can)X
1049(use)X
1204(or)X
1315(ignore)X
1578(the)X
1724(visible)X
1998(hint)X
2174(as)X
2285(they)X
2475(wish.)X
2723(If)X
2819(they)X
3009(ignore)X
3272(it,)X
3382(they)X
3572(should)X
3845(have)X
555 4476(visible_interest)N
1124(in)X
1222(their)X
1413(widget)X
1681(class)X
1880(record)X
2132(set)X
2258(FALSE.)X
2577(In)X
2678(such)X
2867(cases,)X
3102(the)X
3238(visible)X
3502(\256eld)X
3687(is)X
3774(initial-)X
555 4572(ized)N
727(TRUE)X
982(and)X
1134(never)X
1354(changes.)X
1706(If)X
1789(visible_interest)X
2354(is)X
2438(TRUE,)X
2715(the)X
2848(Event)X
3079(Manager)X
3416(asks)X
3591(for)X
5 f
3735(Visibili-)X
555 4668(tyNotify)N
1 f
886(events)X
1133(for)X
1257(the)X
1387(widget,)X
1671(and)X
1820(updates)X
2111(the)X
2 f
2241(visible)X
1 f
2494(\256eld)X
2673(accordingly.)X
3 f
555 4860(18.)N
709(Size)X
883(and)X
1047(Position)X
1373(of)X
1468(Widgets)X
1798(\320)X
1908(Geometry)X
2306(Management)X
1 f
555 4984(A)N
643(widget)X
908(does)X
1094(not)X
1232(directly)X
1527(control)X
1802(its)X
1911(size)X
2073(and)X
2225(location.)X
2557(This)X
2739(is)X
2823(the)X
2956(responsibility)X
3462(of)X
3560(the)X
3693(parent)X
3938(of)X
555 5080(that)N
718(widget.)X
1032(In)X
1135(general,)X
1446(the)X
1584(position)X
1899(of)X
2002(children)X
2321(is)X
2410(left)X
2558(up)X
2676(to)X
2775(their)X
2967(parent.)X
3261(However,)X
3634(the)X
3771(widget)X
555 5176(itself)N
754(often)X
957(has)X
1096(the)X
1226(best)X
1390(idea)X
1559(of)X
1654(its)X
1760(optimal)X
2053(size)X
2212(and)X
2361(may)X
2535(also)X
2699(have)X
2887(a)X
2948(preferred)X
3291(location.)X
555 5300(To)N
677(resolve)X
955(physical)X
1273(layout)X
1518(con\257icts)X
1841(between)X
2158(sibling)X
2423(widgets)X
2721(and)X
2871(between)X
3187(a)X
3249(widget)X
3512(and)X
3662(its)X
3769(parent,)X
555 5396(the)N
699(X)X
798(Toolkit)X
1095(provides)X
1434(the)X
1578(Geometry)X
1966(Management)X
2461(mechanism.)X
2920(Almost)X
3215(all)X
3339(composite)X
3737(widgets)X
555 5492(have)N
746(a)X
810(geometry)X
1168(manager)X
1495(\(geometry_manager)X
2228(\256eld)X
2409(in)X
2502(the)X
2634(widget)X
2898(class)X
3093(record\))X
3370(that)X
3527(is)X
3610(responsible)X
555 5588(for)N
686(the)X
823(size,)X
1011(position,)X
1347(and)X
1503(stacking)X
1826(depth)X
2050(of)X
2151(the)X
2287(widget's)X
2618(children.)X
2979(The)X
3144(only)X
3329(exception)X
3700(are)X
3835(\256xed)X
555 5684(boxes)N
795(like)X
962(Scrollbar,)X
1341(which)X
1590(create)X
1834(their)X
2030(children)X
2353(themselves)X
2779(and)X
2940(can)X
3096(ensure)X
3359(that)X
3526(their)X
3722(children)X
555 5780(will)N
715(never)X
932(make)X
1145(a)X
1206(geometry)X
1561(request.)X
3 f
2239 6144(38)N

39 p
%%Page: 39 41
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(Widgets)N
880(that)X
1044(wish)X
1241(to)X
1341(change)X
1621(their)X
1814(size,)X
2004(position,)X
2341(border)X
2600(width,)X
2853(or)X
2956(stacking)X
3280(depth)X
3506(can)X
3658(not)X
3801(use)X
3948(X)X
555 768(calls)N
756(directly.)X
1087(Instead,)X
1402(they)X
1592(must)X
1802(ask)X
1957(their)X
2157(parent's)X
2478(geometry)X
2849(manager)X
3190(to)X
3297(change)X
3584(it)X
3672(for)X
3812(them.)X
555 864(When)N
795(a)X
863(child)X
1069(makes)X
1323(a)X
1391(request)X
1674(of)X
1776(the)X
1913(parent's)X
2225(geometry)X
2587(manager,)X
2941(the)X
3078(geometry)X
3440(manager)X
3772(can)X
3923(do)X
555 960(one)N
704(of)X
799(the)X
929(following:)X
10 f
555 1084(g)N
1 f
775(Allow)X
1017(the)X
1147(request)X
10 f
555 1208(g)N
1 f
775(Disallow)X
1115(the)X
1245(request)X
10 f
555 1332(g)N
1 f
775(Suggest)X
1076(a)X
1137(compromise)X
555 1456(Geometry)N
934(requests)X
1249(are)X
1383(always)X
1653(made)X
1870(by)X
1984(the)X
2118(child)X
2321(itself.)X
2546(Clients)X
2823(that)X
2982(wish)X
3174(to)X
3269(change)X
3544(the)X
3678(geometry)X
555 1552(of)N
650(a)X
711(widget)X
973(should)X
1230(call)X
5 f
1398(XtSetValues)X
1 f
1921(on)X
2031(the)X
2161(appropriate)X
2584(geometry)X
2939(\256elds.)X
555 1676(When)N
792(the)X
927(geometry)X
1287(manager)X
1617(is)X
1702(asked)X
1928(to)X
2023(change)X
2298(the)X
2432(geometry)X
2791(of)X
2890(a)X
2955(particular)X
3319(child,)X
3544(the)X
3678(geometry)X
555 1772(manager)N
884(may)X
1062(also)X
1230(rearrange)X
1587(and)X
1740(resize)X
1971(any)X
2124(or)X
2223(all)X
2337(of)X
2435(the)X
2568(other)X
2774(children)X
3088(it)X
3163(controls.)X
3516(The)X
3678(geometry)X
555 1868(manager)N
888(can)X
1040(move)X
1265(children)X
1583(around)X
1856(freely)X
2090(using)X
5 f
2328(XtMoveWidget)X
1 f
2916(.)X
2989(When)X
3228(it)X
3307(resizes)X
3575(a)X
3643(child)X
3849(\(that)X
555 1964(is,)N
679(changes)X
1005(width,)X
1271(height,)X
1557(or)X
1673(border_width\),)X
2241(it)X
2334(should)X
2611(inform)X
2893(each)X
3096(affected)X
3421(child)X
3640(by)X
3770(calling)X
5 f
570 2060(XtResizeWidget)N
1 f
1211(.)X
555 2184(Often,)N
816(geometry)X
1188(managers)X
1564(\256nd)X
1739(that)X
1910(they)X
2100(can)X
2260(satisfy)X
2528(a)X
2605(request)X
2897(only)X
3092(if)X
3184(they)X
3374(can)X
3534(recon\256gure)X
3972(a)X
555 2280(widget)N
823(that)X
984(they)X
1164(are)X
1299(not)X
1440(in)X
1536(control)X
1813(of)X
1913(\(in)X
2038(particular,)X
2425(when)X
2642(the)X
2777(composite)X
3167(widget)X
3434(wants)X
3666(to)X
3762(change)X
555 2376(its)N
680(own)X
872(size\).)X
1123(In)X
1237(this)X
1406(case,)X
1620(the)X
1768(geometry)X
2141(manager)X
2484(makes)X
2749(a)X
2828(request)X
3122(to)X
3231(its)X
3355(parent's)X
3678(geometry)X
555 2472(manager.)N
924(Geometry)X
1298(requests)X
1608(can)X
1752(cascade)X
2047(this)X
2197(way)X
2365(to)X
2456(arbitrary)X
2781(depth.)X
555 2596(Because)N
881(such)X
1075(cascaded)X
1425(arbitration)X
1825(of)X
1930(widget)X
2202(geometry)X
2567(may)X
2751(involve)X
3048(extended)X
3398(negotiation,)X
3850(win-)X
555 2692(dows)N
763(are)X
893(not)X
1029(actually)X
1331(allocated)X
1672(to)X
1763(widgets)X
2059(at)X
2145(application)X
2560(startup)X
2822(until)X
3007(all)X
3118(widgets)X
3414(are)X
3543(satis\256ed)X
3854(with)X
555 2788(their)N
739(geometry.)X
1138(See)X
1287(``XtRealizeWidget'')X
2038(and)X
2187(``Creating)X
2571(Widgets'')X
2945(for)X
3069(more)X
3272(details.)X
3 f
555 2980(18.1.)N
775(Making)X
1091(Geometry)X
1489(Manager)X
1853(Requests)X
1 f
555 3104(To)N
681(make)X
900(a)X
967(general)X
1253(geometry)X
1613(manager)X
1943(request,)X
2246(use)X
5 f
2408(XtMakeGeometryRequest)X
1 f
3437(.)X
3508(The)X
3672(de\256nition)X
555 3200(for)N
679(this)X
829(function)X
1145(is:)X
555 3420(XtGeometryResult)N
1243(XtMakeGeometryRequest\()X
2 f
2196(w)X
1 f
2255(,)X
2 f
2299(request)X
1 f
2558(,)X
2 f
(reply)S
1 f
2761(\))X
687 3516(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3612(XtWidgetGeometry)N
1409(*)X
2 f
(request)S
1 f
1712(;)X
687 3708(XtWidgetGeometry)N
1409(*)X
2 f
(reply)S
1 f
1634(;)X
2 f
555 3956(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 4080(request)N
1 f
1131(Speci\256es)X
1475(the)X
1609(desired)X
1889(widget)X
2155(geometry)X
2514(\(size,)X
2728(position,)X
3061(border)X
3316(width,)X
3565(and)X
3717(stacking)X
1131 4176(order\).)N
2 f
555 4300(reply)N
1 f
1131(Speci\256es)X
1478(an)X
1589(argument)X
1950(in)X
2047(which)X
2290(the)X
2426(allowed)X
2733(widget)X
3001(size)X
3166(is)X
3253(returned.)X
3618(If)X
3704(a)X
3771(widget)X
1131 4396(is)N
1225(not)X
1373(interested)X
1751(in)X
1855(handling)X
5 f
2217(XtGeometryAlmost)X
1 f
2972(,)X
3029(the)X
3171(reply)X
3386(parameter)X
3772(can)X
3928(be)X
1131 4492(NULL.)N
555 4616(The)N
714(return)X
946(codes)X
1168(from)X
1361(geometry)X
1716(managers)X
2075(are:)X
555 4740(typedef)N
841(enum)X
1059(_XtGeometryResult)X
1791({)X
843 4836(XtGeometryYes,)N
843 4932(XtGeometryNo,)N
843 5028(XtGeometryAlmost,)N
555 5124(})N
619(XtGeometryResult;)X
555 5296(The)N
5 f
742(XtWidgetGeometry)X
1 f
1534(structure)X
1874(is)X
1965(quite)X
2174(similar)X
2452(but)X
2597(not)X
2742(identical)X
3079(to)X
3180(the)X
3320(corresponding)X
3854(Xlib)X
555 5392(structure:)N
3 f
2239 6144(39)N

40 p
%%Page: 40 42
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(typedef)N
841(unsigned)X
1181(long)X
1360(XtGeometryMask;)X
555 864(typedef)N
841(struct)X
1059({)X
843 960(XtGeometryMask)N
2 f
1500(request_mask)X
1 f
1983(;)X
843 1056(Position)N
2 f
1155(x)X
1 f
1194(,)X
2 f
1238(y)X
1 f
1277(;)X
843 1152(Dimension)N
2 f
1252(width)X
1 f
1449(,)X
2 f
1493(height)X
1 f
1714(;)X
843 1248(Dimension)N
2 f
1252(border_width)X
1 f
1732(;)X
843 1344(Widget)N
2 f
1125(sibling)X
1 f
1366(;)X
843 1440(int)N
2 f
959(stack_mode)X
1 f
1374(;)X
555 1536(})N
619(XtWidgetGeometry;)X
555 1708(The)N
714(request_mask)X
1220(de\256nitions)X
1615(are)X
1744(from)X
5 f
1955(<X/X.h>)X
1 f
2289(:)X
843 1832(#de\256ne)N
1124(CWX)X
1707(\(1<<0\))X
843 1928(#de\256ne)N
1124(CWY)X
1707(\(1<<1\))X
843 2024(#de\256ne)N
1124(CWWidth)X
1707(\(1<<2\))X
843 2120(#de\256ne)N
1124(CWHeight)X
1707(\(1<<3\))X
843 2216(#de\256ne)N
1124(CWBorderWidth\(1<<4\))X
843 2312(#de\256ne)N
1124(CWSibling)X
1707(\(1<<5\))X
843 2408(#de\256ne)N
1124(CWStackMode)X
1707(\(1<<6\))X
5 f
570 2580(XtMakeGeometryRequest)N
1 f
1599(,)X
1724(in)X
1896(exactly)X
2254(the)X
2465(same)X
2749(manner)X
3116(as)X
3292(the)X
3502(Xlib)X
3761(routine)X
5 f
570 2676(XCon\256gureWindow)N
1 f
1337(,)X
1409(uses)X
1610(the)X
1768(request_mask)X
2302(to)X
2421(determine)X
2824(which)X
3089(\256elds)X
3330(in)X
3449(the)X
5 f
3624(XtWidget-)X
555 2772(Geometry)N
1 f
979(structure)X
1309(you)X
1463(want)X
1656(to)X
1747(specify.)X
555 2896(The)N
714(stack_mode)X
1157(de\256nitions)X
1552(are)X
1681(from)X
5 f
1892(<X/X.h>)X
1 f
2226(:)X
843 3020(#de\256ne)N
1124(Above)X
1995(0)X
843 3116(#de\256ne)N
1124(Below)X
1995(1)X
843 3212(#de\256ne)N
1124(TopIf)X
1995(2)X
843 3308(#de\256ne)N
1124(BottomIf)X
1995(3)X
843 3404(#de\256ne)N
1124(Opposite)X
1995(4)X
843 3500(#de\256ne)N
1124(XtDontChange)X
1995(5)X
555 3672(\(See)N
747(the)X
891(Xlib)X
1084(manual)X
1379(for)X
1516(de\256nition)X
1890(and)X
2052(behavior)X
2395(of)X
5 f
2521(Above)X
1 f
2786(,)X
5 f
2861(Below)X
1 f
3115(,)X
5 f
3190(TopIf)X
1 f
3407(,)X
5 f
3482(BottomIf)X
1 f
3827(,)X
3884(and)X
5 f
570 3768(Opposite)N
1 f
938(.\))X
555 3892(The)N
5 f
738(XtMakeGeometryRequest)X
1 f
1795(function)X
2117(is)X
2204(a)X
2271(simple)X
2535(routine.)X
2857(If)X
2943(the)X
3079(parent)X
3326(is)X
3412(not)X
3552(a)X
3618(subclass)X
3938(of)X
555 3988(Composite,)N
5 f
1013(XtMakeGeometryRequest)X
1 f
2077(issues)X
2322(an)X
2439(error.)X
2687(If)X
2779(the)X
2921(parent's)X
3238(geometry_manager)X
3952(is)X
555 4084(NULL,)N
5 f
874(XtMakeGeometryRequest)X
1 f
1948(issues)X
2202(an)X
2329(error.)X
2587(If)X
2689(being_destroyed)X
3315(is)X
3418(TRUE,)X
5 f
3732(XtMak-)X
555 4180(eGeometryRequest)N
1 f
1363(returns)X
5 f
1654(XtGeometryNo)X
1 f
2252(.)X
2325(If)X
2412(managed)X
2759(is)X
2847(FALSE,)X
5 f
3184(XtMakeGeometryRe-)X
555 4276(quest)N
1 f
815(issues)X
1054(a)X
1122(warning)X
1438(and)X
1593(returns)X
5 f
1883(XtGeometryNo)X
1 f
2481(.)X
2553(Otherwise,)X
5 f
2982(XtMakeGeometryRequest)X
1 f
555 4372(calls)N
739(the)X
869(parent's)X
1174(geometry_manager)X
1876(procedure)X
2249(with)X
2428(the)X
2558(given)X
2776(parameters.)X
555 4496(If)N
642(the)X
779(parent's)X
1091(geometry)X
1452(manager)X
1783(returns)X
5 f
2073(XtGeometryYes)X
1 f
2739(and)X
2894(the)X
3030(widget)X
3298(is)X
3385(realized,)X
5 f
3732(XtMak-)X
555 4592(eGeometryRequest)N
1 f
1356(recon\256gures)X
1812(the)X
1942(widget's)X
2267(window)X
2572(by)X
2682(calling)X
5 f
2963(XResizeWindow)X
1 f
3617(.)X
3 f
555 4784(18.2.)N
775(Making)X
1091(Resize)X
1352(Requests)X
1 f
555 4908(To)N
680(make)X
898(a)X
964(simple)X
1227(resize)X
1459(request,)X
1762(use)X
5 f
1924(XtMakeResizeRequest)X
1 f
2834(.)X
2905(The)X
3069(de\256nition)X
3435(for)X
3563(this)X
3717(function)X
555 5004(is:)N
555 5224(XtGeometryResult)N
1243(XtMakeResizeRequest\()X
2 f
2079(w)X
1 f
2138(,)X
2 f
2182(width)X
1 f
2379(,)X
2 f
2423(height)X
1 f
2644(,)X
2 f
2688(replyWidth)X
1 f
3080(,)X
2 f
3124(replyHeight)X
1 f
3545(\))X
687 5320(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 5416(Dimension)N
2 f
1096(width)X
1 f
1293(,)X
2 f
1337(height)X
1 f
1558(;)X
687 5512(Dimension)N
1096(*)X
2 f
(replyWidth)S
1 f
1532(,)X
1576(*)X
2 f
(replyHeight)S
3 f
2239 6144(40)N

41 p
%%Page: 41 43
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
2 f
555 672(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 796(width)N
1 f
1131(Speci\256es)X
1471(the)X
1601(desired)X
1877(widget)X
2139(width.)X
2 f
555 920(height)N
1 f
1131(Speci\256es)X
1471(the)X
1601(desired)X
1877(widget)X
2139(height.)X
2 f
555 1044(replyWidth)N
1 f
1131(Speci\256es)X
1471(an)X
1576(argument)X
1931(in)X
2022(which)X
2259(the)X
2389(allowed)X
2690(widget)X
2952(width)X
3175(is)X
3256(returned.)X
2 f
555 1168(replyHeight)N
1 f
1131(Speci\256es)X
1471(an)X
1576(argument)X
1931(in)X
2022(which)X
2259(the)X
2389(allowed)X
2690(widget)X
2952(height)X
3195(is)X
3276(returned.)X
5 f
570 1292(XtMakeResizeRequest)N
1 f
1524(is)X
1627(a)X
1710(simple)X
1990(interface)X
2342(to)X
5 f
2472(XtMakeGeometryRequest)X
1 f
3501(.)X
3588(It)X
3685(creates)X
3972(a)X
5 f
570 1388(XtWidgetGeometry)N
1 f
1356(structure)X
1690(and)X
1843(speci\256es)X
2172(that)X
2331(width)X
2558(and)X
2711(height)X
2958(should)X
3219(change.)X
3516(The)X
3678(geometry)X
555 1484(manager)N
882(is)X
965(free)X
1125(to)X
1218(modify)X
1497(any)X
1648(of)X
1745(the)X
1877(other)X
2082(window)X
2389(attributes)X
2742(\(position)X
3080(or)X
3177(stacking)X
3495(order\))X
3733(in)X
3826(order)X
555 1580(to)N
676(satisfy)X
958(the)X
1118(resize)X
1375(request.)X
1703(If)X
1813(the)X
1972(return)X
2233(value)X
2475(is)X
5 f
2603(XtGeometryAlmost)X
1 f
3358(,)X
3431(replyWidth)X
3884(and)X
555 1676(replyHeight)N
1000(contain)X
1284(a)X
1347(``compromise'')X
1923(width)X
2148(and)X
2299(height.)X
2566(If)X
2648(these)X
2853(are)X
2984(acceptable)X
3380(the)X
3512(widget)X
3776(should)X
555 1772 0.3187(immediately)AN
1020(make)X
1233(an)X
5 f
1356(XtResizeRequest)X
1 f
2073(requesting)X
2462(the)X
2592(compromise)X
3050(width)X
3273(and)X
3422(height.)X
555 1896(If)N
640(the)X
775(widget)X
1042(is)X
1128(not)X
1268(interested)X
1638(in)X
5 f
1752(XtGeometryAlmost)X
1 f
2534(replies,)X
2818(it)X
2895(can)X
3044(pass)X
3221(NULL)X
3481(for)X
3609(replyWidth)X
555 1992(and)N
704(replyHeight.)X
3 f
555 2184(18.3.)N
775(Geometry)X
1173(Manager)X
1537(Responsibilities:)X
2176(the)X
2315(geometry_manager)X
3064(procedure)X
1 f
555 2308(Speci\256c)N
861(geometry)X
1216(managers)X
1575(are)X
1704(of)X
1799(type)X
1973(XtGeometryHandler:)X
555 2528(typedef)N
841(XtGeometryResult)X
1529(\(*)X
2 f
1602(XtGeometryHandler)X
1 f
2320(\)\(\);)X
687 2624(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 2720(XtWidgetGeometry)N
1409(*)X
2 f
(request)S
1 f
1712(;)X
687 2816(XtWidgetGeometry)N
1409(*)X
2 f
(reply)S
1 f
1634(;)X
555 3036(A)N
645(zero)X
823(\(0\))X
952(bit)X
1072(in)X
1167(the)X
1301(request)X
1581(mask)X
1793(means)X
2044(that)X
2203(you)X
2361(do)X
2475(not)X
2614(care)X
2786(about)X
3008(the)X
3142(value)X
3359(of)X
3458(the)X
3592(correspond-)X
555 3132(ing)N
698(\256eld.)X
907(Then,)X
1140(the)X
1278(geometry)X
1641(manager)X
1974(can)X
2126(change)X
2405(it)X
2485(as)X
2588(it)X
2668(wishes.)X
2959(A)X
3052(one)X
3209(\(1\))X
3340(bit)X
3463(means)X
3717(that)X
3879(you)X
555 3228(want)N
748(that)X
903(geometry)X
1258(element)X
1560(changed)X
1875(to)X
1966(the)X
2096(value)X
2309(in)X
2400(the)X
2530(corresponding)X
3055(\256eld.)X
555 3352(If)N
640(the)X
774(geometry)X
1133(manager)X
1462(can)X
1610(satisfy)X
1866(all)X
1981(changes)X
2290(requested,)X
2675(it)X
2751(updates)X
3046(the)X
3180(widget's)X
3509(position)X
3820(\256elds)X
555 3448(\(x,)N
690(y\))X
802(with)X
5 f
1016(XtMoveWidget)X
1 f
1604(,)X
1665(and)X
1831(updates)X
2139(width,)X
2401(height,)X
2683(and)X
2849(border_width)X
3362(directly,)X
3693(and)X
3859(then)X
555 3544(returns)N
5 f
842(XtGeometryYes)X
1 f
1480(.)X
1549(The)X
1710(value)X
1925(of)X
2022(the)X
2154(reply)X
2359(parameter)X
2735(is)X
2818(unde\256ned.)X
3211(\(The)X
3401(widget's)X
3728(window)X
555 3640(is)N
636(resized)X
907 0.2604(automatically)AX
1411(by)X
5 f
1539(XtMakeGeometryRequest)X
1 f
2568(.\))X
555 3764(Note)N
776(that)X
959(the)X
1117(geometry)X
1500(manager)X
1853(does)X
2063(not)X
2225(call)X
2402(the)X
2559(widget's)X
2911(resize)X
3165(procedure)X
3565(if)X
3668(it)X
3767(returns)X
5 f
570 3860(XtGeometryYes)N
1 f
1208(.)X
1290(Then,)X
1531(the)X
1677(widget)X
1955(must)X
2164(perform)X
2484(whatever)X
2843(resizing)X
3159(calculations)X
3618(are)X
3762(needed)X
555 3956(explicitly.)N
959(This)X
1141(costs)X
1342(primitive)X
1692(widgets)X
1991(an)X
2098(extra)X
2298(line,)X
2477(but)X
2614(it)X
2688(substantially)X
3159(simpli\256es)X
3527(the)X
3659(writing)X
3938(of)X
555 4052(nice)N
724(composite)X
1109(widgets.)X
555 4176(If)N
641(the)X
777(geometry)X
1138(manager)X
1469(chooses)X
1775(to)X
1872(disallow)X
2199(the)X
2335(request,)X
2639(the)X
2774(widget)X
3041(cannot)X
3303(change)X
3579(it)X
3656(geometry.)X
555 4272(The)N
765(value)X
1029(of)X
1175(the)X
1356(reply)X
1610(parameter)X
2035(is)X
2167(unde\256ned,)X
2608(and)X
2807(the)X
2987(geometry)X
3392(manager)X
3767(returns)X
5 f
570 4368(XtGeometryNo)N
1 f
1168(.)X
555 4492(Sometimes)N
973(the)X
1106(geometry)X
1464(manager)X
1792(cannot)X
2052(satisfy)X
2307(the)X
2440(request)X
2719(exactly,)X
3021(but)X
3159(it)X
3234(may)X
3410(be)X
3517(able)X
3688(to)X
3781(satisfy)X
555 4588(what)N
762(it)X
848(considers)X
1216(a)X
1291(similar)X
1573(request.)X
1885(That)X
2083(is,)X
2200(it)X
2286(could)X
2518(satisfy)X
2784(only)X
2977(a)X
3051(subset)X
3306(of)X
3414(the)X
3557(requests)X
3880(\(for)X
555 4684(example,)N
907(size)X
1075(but)X
1219(not)X
1362(position\),)X
1728(satisfy)X
1988(a)X
2057(request)X
2341(only)X
2528(if)X
2612(other)X
2823(changes)X
3136(take)X
3313(place)X
3529(\(for)X
3690(example,)X
555 4780(size)N
715(can)X
860(be)X
966(changed)X
1282(if)X
1359(the)X
1490(widget)X
1753(is)X
1835(moved)X
2098(to)X
2190(a)X
2252(particular)X
2613(location\),)X
2972(or)X
3068(satisfy)X
3321(a)X
3382(lesser)X
3604(request)X
3880(\(for)X
555 4876(example,)N
908(it)X
989(can't)X
1196(make)X
1418(the)X
1557(child)X
1765(as)X
1869(big)X
2013(as)X
2117(the)X
2256(request)X
2541(but)X
2685(it)X
2766(can)X
2919(make)X
3141(the)X
3280(child)X
3488(bigger)X
3744(than)X
3927(its)X
555 4972(current)N
826(size\).)X
555 5096(In)N
658(such)X
849(cases,)X
1086(the)X
1224(geometry)X
1587(manager)X
1920(\256lls)X
2083(in)X
2182(reply)X
2393(with)X
2580(the)X
2718(actual)X
2959(changes)X
3272(it)X
3352(is)X
3441(willing)X
3722(to)X
3820(make)X
555 5192(and)N
728(returns)X
5 f
1036(XtGeometryAlmost)X
1 f
1791(.)X
1881(If)X
1985(a)X
2070(bit)X
2210(in)X
2324(reply.request_mask)X
3056(is)X
3160(zero)X
3356(\(0\),)X
3525(the)X
3678(geometry)X
555 5288(manager)N
885(will)X
1050(not)X
1190(change)X
1466(the)X
1601(corresponding)X
2131(value.)X
2393(If)X
2478(a)X
2543(bit)X
2663(is)X
2748(one)X
2901(\(1\),)X
3051(the)X
3185(geometry)X
3544(manager)X
3873(will)X
555 5384(change)N
826(that)X
981(element)X
1283(to)X
1374(the)X
1504(corresponding)X
2029(value)X
2242(in)X
2333(the)X
2463(reply)X
2666(parameter.)X
555 5508(When)N
5 f
811(XtGeometryAlmost)X
1 f
1594(is)X
1681(returned,)X
2024(the)X
2160(widget)X
2428(must)X
2628(decide)X
2886(if)X
2968(the)X
3104(compromise)X
3568(suggested)X
3942(in)X
555 5604(the)N
689(reply)X
895(is)X
979(acceptable.)X
1398(If)X
1481(so,)X
1606(the)X
1739(widget)X
2004(must)X
2201(not)X
2339(change)X
2613(its)X
2722(geometry)X
3080(directly.)X
3419(Rather,)X
3701(it)X
3776(should)X
555 5700(make)N
768(another)X
1054(call)X
1204(to)X
5 f
1313(XtMakeGeometryRequest)X
1 f
2342(.)X
3 f
2239 6144(41)N

42 p
%%Page: 42 44
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(If)N
642(the)X
779(next)X
960(geometry)X
1322(request)X
1605(on)X
1722(this)X
1879(window)X
2191(uses)X
2371(the)X
2508(reply)X
2718(box)X
2879(\256lled)X
3090(in)X
3188(by)X
3304(an)X
5 f
3433(XtGeometryAl-)X
555 768(most)N
1 f
783(return)X
1015(and)X
1164(if)X
1240(there)X
1438(have)X
1626(been)X
1814(no)X
1924(intervening)X
2348(geometry)X
2703(requests)X
3013(on)X
3123(either)X
3346(its)X
3452(parent)X
3694(or)X
3789(any)X
3938(of)X
555 864(its)N
671(children,)X
1014(the)X
1154(request)X
1440(is)X
1530(guaranteed)X
1947(to)X
2047(succeed.)X
2378(That)X
2571(is,)X
2683(if)X
2768(you)X
2931(ask)X
3079(again)X
3301(right)X
3499(away)X
3715(with)X
3903(the)X
555 960(returned)N
870(geometry,)X
1247(you)X
1401(will)X
1561(get)X
1691(an)X
1796(answer)X
2066(of)X
5 f
2179(XtGeometryYes)X
1 f
2817(.)X
555 1084(To)N
682(return)X
921(an)X
5 f
1051(XtGeometryYes)X
1 f
1689(,)X
1740(the)X
1877(geometry)X
2239(manager)X
2571(will)X
2738(frequently)X
3129(rearrange)X
3489(the)X
3625(position)X
3938(of)X
555 1180(other)N
778(managed)X
1138(children.)X
1491(To)X
1630(do)X
1759(this,)X
1950(it)X
2041(should)X
2317(call)X
2486(the)X
2635(procedure)X
5 f
3045(XtMoveWidget)X
1 f
3674(described)X
555 1276(below.)N
847(However,)X
1223(a)X
1294(few)X
1457(geometry)X
1822(managers)X
2191(may)X
2375(sometimes)X
2785(change)X
3066(the)X
3206(size)X
3375(of)X
3480(other)X
3693(managed)X
555 1372(children.)N
888(To)X
1008(do)X
1118(this,)X
1290(they)X
1464(should)X
1721(call)X
1871(the)X
2001(procedure)X
5 f
2392(XtResizeWidget)X
1 f
3055(described)X
3414(below.)X
3 f
555 1564(18.3.1.)N
841(XtMoveWidget)X
1 f
555 1688(Geometry)N
945(managers)X
1320(should)X
1593(call)X
5 f
1777(XtMoveWidget)X
1 f
2403(to)X
2510(move)X
2744(their)X
2944(children)X
3271(widget)X
3549(around.)X
3874(The)X
555 1784(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 2004(void)N
734(XtMoveWidget\()X
2 f
1316(w)X
1 f
1375(,)X
2 f
1419(x)X
1 f
1458(,)X
2 f
1502(y)X
1 f
1541(\))X
687 2100(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 2196(Position)N
2 f
999(x)X
1 f
1038(;)X
687 2292(Position)N
2 f
999(y)X
1 f
1038(;)X
2 f
555 2540(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 2664(x)N
555 2760(y)N
1 f
1131(Speci\256es)X
1471(the)X
1601(new)X
1769(widget)X
2031(coordinates.)X
5 f
570 2884(XtMoveWidget)N
1 f
1193(writes)X
1443(the)X
1586(new)X
1766(x)X
1844(and)X
2005(y)X
2083(values)X
2342(into)X
2514(the)X
2656(widget)X
2930(and,)X
3113(if)X
3201(the)X
3343(widget)X
3617(is)X
3710(realized,)X
555 2980(issues)N
787(an)X
5 f
910(XMoveWindow)X
1 f
1533(call)X
1683(on)X
1793(the)X
1923(widget's)X
2248(window.)X
3 f
555 3172(18.3.2.)N
841(XtResizeWidget)X
1 f
555 3296(Geometry)N
941(managers)X
1311(should)X
1579(call)X
5 f
1758(XtResizeWidget)X
1 f
2432(to)X
2534(resize)X
2772(children)X
3094(widgets)X
3401(\(except)X
3693(the)X
3834(child)X
555 3392(making)N
842(the)X
972(geometry)X
1327(request\).)X
1676(The)X
1835(de\256nition)X
2196(for)X
2320(this)X
2470(function)X
2786(is:)X
555 3612(void)N
734(XtResizeWidget\()X
2 f
1346(w)X
1 f
1405(,)X
2 f
1449(width)X
1 f
1646(,)X
2 f
1690(height)X
1 f
1911(,)X
2 f
1955(border_width)X
1 f
2435(\))X
687 3708(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3804(Dimension)N
2 f
1096(width)X
1 f
1293(;)X
687 3900(Dimension)N
2 f
1096(height)X
1 f
1317(;)X
687 3996(Dimension)N
2 f
1096(border_width)X
1 f
1576(;)X
2 f
555 4244(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 4368(width)N
555 4464(height)N
555 4560(border_width)N
1 f
1131(Specify)X
1422(the)X
1552(new)X
1720(widget)X
1982(size.)X
5 f
570 4684(XtResizeWidget)N
1 f
1238(writes)X
1480(the)X
1615(new)X
1788(width,)X
2038(height,)X
2308(and)X
2462(border_width)X
2963(values)X
3215(into)X
3380(the)X
3515(widget)X
3782(and,)X
3957(if)X
555 4780(the)N
685(widget)X
947(is)X
1028(realized,)X
1351(issues)X
1583(an)X
5 f
1706(XCon\256gureWindow)X
1 f
2495(call)X
2645(on)X
2755(the)X
2885(widget's)X
3210(window.)X
555 4904(If)N
638(the)X
771(new)X
942(width)X
1168(or)X
1266(height)X
1512(are)X
1644(different)X
1972(from)X
2167(the)X
2299(old)X
2436(values,)X
5 f
2725(XtResizeWidget)X
1 f
3390(calls)X
3576(the)X
3708(widget's)X
555 5000(resize)N
782(procedure)X
1155(to)X
1246(notify)X
1479(it)X
1551(of)X
1646(the)X
1776(size)X
1935(change.)X
3 f
555 5192(18.4.)N
775(Widget)X
1071(Resizing)X
1411(Responsibilities:)X
2050(the)X
2189(resize)X
2426(procedure)X
1 f
555 5316(A)N
652(child)X
863(can)X
1019(be)X
1136(involuntarily)X
1627(resized)X
1910(by)X
2032(its)X
2150(parent)X
2403(at)X
2500(any)X
2660(time.)X
2895(Widgets)X
3222(usually)X
3510(want)X
3714(to)X
3816(know)X
555 5412(when)N
776(they)X
958(have)X
1154(changed)X
1477(size,)X
1666(so)X
1774(they)X
1956(can)X
2108(re-layout)X
2456(their)X
2648(displayed)X
3016(data)X
3193(to)X
3292(match)X
3538(the)X
3676(new)X
3852(size.)X
555 5508(When)N
788(a)X
850(geometry)X
1206(manager)X
1532(resizes)X
1794(a)X
1856(child)X
2056(it)X
2129(calls)X
5 f
2332(XtResizeWidget)X
1 f
2973(,)X
3018(which)X
3256(updates)X
3548(the)X
3678(geometry)X
555 5604(\256elds)N
776(in)X
874(the)X
1011(widget,)X
1302(con\256gures)X
1697(the)X
1834(window)X
2146(if)X
2229(the)X
2366(widget)X
2635(is)X
2723(realized,)X
3053(and)X
3209(calls)X
3400(the)X
3537(child's)X
3806(resize)X
555 5700(procedure)N
928(to)X
1019(notify)X
1252(the)X
1382(child.)X
3 f
2239 6144(42)N

43 p
%%Page: 43 45
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
725(resize)X
963(procedure)X
1346(should)X
1613(recalculate)X
2027(the)X
2167(layout)X
2420(of)X
2525(internal)X
2827(data)X
3006(as)X
3111(needed.)X
3436(\(For)X
3619(example,)X
3972(a)X
555 768(centered)N
878(Label)X
1104(in)X
1198(a)X
1262(window)X
1570(that)X
1728(changes)X
2036(size)X
2198(should)X
2458(recalculate)X
2865(the)X
2998(starting)X
3288(position)X
3598(of)X
3695(the)X
3827(text.\))X
555 864(The)N
720(widget)X
988(must)X
1188(obey)X
1387(resize)X
1620(as)X
1721(a)X
1788(command)X
2164(and)X
2319(must)X
2519(not)X
2660(treat)X
2845(it)X
2923(as)X
3024(a)X
3091(request.)X
3417(A)X
3508(widget)X
3776(cannot)X
555 960(issue)N
753(an)X
5 f
876(XtMakeGeometryRequest)X
1 f
1927(call)X
2077(from)X
2270(its)X
2376(resize)X
2603(procedure.)X
3 f
555 1152(19.)N
709(Input)X
941(Focus)X
1 f
555 1276(This)N
734(section)X
1006(discusses)X
1355(the)X
1485(underlying)X
1889(philosophy)X
2303(of)X
2398(the)X
2528(X)X
2613(Toolkit)X
2896(input)X
3100(focus)X
3312(management.)X
555 1400(Widgets)N
874(that)X
1032(want)X
1227(the)X
1359(input)X
1565(focus)X
1779(can)X
1925(call)X
5 f
2095(XSetInputFocus)X
1 f
2763(directly.)X
3079(To)X
3201(allow)X
3421(others)X
3660(to)X
3753(cause)X
3972(a)X
555 1496(widget)N
846(to)X
966(get)X
1125(the)X
1284(input)X
1517(focus,)X
1780(every)X
2026(widget)X
2317(exports)X
2627(an)X
2761(accept_focus)X
3271(procedure.)X
3717(Widgets)X
555 1592(interested)N
944(in)X
1058(knowing)X
1411(when)X
1646(they)X
1843(lose)X
2030(the)X
2183(input)X
2410(focus)X
2645(must)X
2862(use)X
3024(the)X
3177(X11)X
3373(focus)X
3608(noti\256cation)X
555 1688(mechanism)N
986(directly.)X
1329(Widgets)X
1652(that)X
1813(never)X
2036(want)X
2235(the)X
2371(input)X
2581(focus)X
2799(should)X
3062(set)X
3188(their)X
3378(accept_focus)X
3865(pro-)X
555 1784(cedure)N
811(to)X
902(NULL.)X
555 1908(Composite)N
978(widgets)X
1292(are)X
1439(required)X
1772(to)X
1881(export)X
2146(two)X
2318(additional)X
2712(functions:)X
3105(move_focus_to_next)X
3884(and)X
555 2004(move_focus_to_prev.)N
1367(These)X
1602(procedures)X
2011(move)X
2231(the)X
2363(input)X
2569(focus)X
2783(to)X
2876(the)X
3008(next)X
3184(child)X
3385(widget)X
3649(that)X
3806(wants)X
555 2100(it)N
630(and)X
782(to)X
876(the)X
1009(previous)X
1337(child)X
1539(widget)X
1804(that)X
1962(wants)X
2192(it,)X
2288(respectively.)X
2760(The)X
2921(de\256nition)X
3284(of)X
3381(next)X
3557(and)X
3708(previous)X
555 2196(is)N
640(left)X
784(to)X
879(each)X
1066(individual)X
1451(widget.)X
1739(In)X
1838(addition,)X
2176(composite)X
2565(widgets)X
2865(are)X
2998(free)X
3160(to)X
3255(implement)X
3659(other)X
3865(pro-)X
555 2292(cedures)N
880(to)X
1005(move)X
1257(the)X
1421(input)X
1659(focus)X
1905(between)X
2254(their)X
2472(children.)X
2861(Both)X
3089(move_focus_to_next)X
3884(and)X
555 2388(move_focus_to_prev)N
1323(should)X
1583(be)X
1691(entered)X
1975(in)X
2069(the)X
2202(widget)X
2467(class)X
2663(action)X
2904(table,)X
3123(so)X
3226(that)X
3384(they)X
3561(are)X
3692(available)X
555 2484(to)N
646(translation)X
1041(speci\256cations.)X
3 f
555 2676(20.)N
709(Selections)X
1 f
555 2800(Arbitrary)N
920(widgets)X
1232(\(possibly)X
1593(not)X
1744(all)X
1871(in)X
1978(the)X
2123(same)X
2341(application\))X
2800(communicate)X
3313(with)X
3507(each)X
3705(other)X
3923(by)X
555 2896(means)N
802(of)X
897(the)X
1027(selection)X
1363(mechanism)X
1787(de\256ned)X
2068(by)X
2178(the)X
2308(server)X
2544(and)X
2693(Xlib.)X
2894(See)X
3043(the)X
3173(Xlib)X
3352(manual)X
3634(for)X
3758(details.)X
3 f
555 3088(21.)N
709(Resource)X
1077(Management)X
1 f
555 3212(Writers)N
849(of)X
952(widgets)X
1256(need)X
1452(to)X
1551(obtain)X
1802(a)X
1871(large)X
2076(set)X
2203(of)X
2305(resources)X
2665(at)X
2758(widget)X
3027(creation)X
3340(time.)X
3571(Some)X
3801(of)X
3903(the)X
555 3308(resources)N
909(come)X
1123(from)X
1317(the)X
1448(resource)X
1768(database,)X
2116(some)X
2325(from)X
2519(the)X
2650(argument)X
3006(list)X
3138(supplied,)X
3482(and)X
3632(some)X
3840(from)X
555 3404(the)N
689(internal)X
985(defaults)X
1290(speci\256ed)X
1629(for)X
1757(the)X
1891(widget.)X
2179(Resources)X
2566(are)X
2699(obtained)X
3029(\256rst)X
3192(from)X
3389(the)X
3522(argument)X
3880(list,)X
555 3500(then)N
742(from)X
948(the)X
1091(resource)X
1423(database)X
1761(for)X
1898(all)X
2022(resources)X
2387(not)X
2534(speci\256ed)X
2881(in)X
2984(the)X
3126(argument)X
3493(list,)X
3658(and)X
3819(lastly)X
555 3596(from)N
748(the)X
878(internal)X
1170(default,)X
1459(if)X
1535(needed.)X
555 3720(A)N
652(resource)X
983(is)X
1076(a)X
1149(\256eld)X
1340(in)X
1443(the)X
1585(widget)X
1859(record)X
2117(with)X
2308(a)X
2381(corresponding)X
2918(resource)X
3249(entry)X
3464(in)X
3567(the)X
3708(widget's)X
555 3816(resource)N
902(list)X
1061(\(or)X
1213(in)X
1332(a)X
1421(superclass's)X
1894(resource)X
2240(list\).)X
2471(This)X
2677(means)X
2951(that)X
3133(the)X
3290(\256eld)X
3496(is)X
3604(settable)X
3923(by)X
5 f
570 3912(XtCreateWidget)N
1 f
1238(\(by)X
1386(naming)X
1682(the)X
1820(\256eld)X
2007(in)X
2106(the)X
2244(argument)X
2607(list\),)X
2797(by)X
2915(an)X
3028(entry)X
3239(in)X
3338(the)X
3476(default)X
3751(\256le)X
3894(\(by)X
555 4008(using)N
782(either)X
1019(the)X
1163(name)X
1390(or)X
1499(class\),)X
1757(and)X
1920(by)X
5 f
2062(XtSetValues)X
1 f
2563(.)X
2643(In)X
2752(addition,)X
3100(it)X
3186(is)X
3281(readable)X
3614(by)X
5 f
3755(XtGet-)X
555 4104(Values)N
1 f
839(.)X
555 4228(Not)N
716(all)X
834(\256elds)X
1054(in)X
1152(a)X
1220(widget)X
1489(record)X
1742(are)X
1878(resources.)X
2260(Some)X
2490(are)X
2626(for)X
2756(``bookkeeping'')X
3355(use)X
3500(by)X
3616(the)X
3752(generic)X
555 4324(routines)N
866(\(like)X
1055(managed)X
1400(and)X
1554(being_destroyed\).)X
2214(Other)X
2440(may)X
2618(be)X
2727(for)X
2855(local)X
3053(bookkeeping,)X
3556(and)X
3709(still)X
3869(oth-)X
555 4420(ers)N
679(are)X
808(derived)X
1094(from)X
1287(resources)X
1640(\(many)X
1887(GCs)X
2065(and)X
2214(Pixmaps\).)X
3 f
555 4612(21.1.)N
775(XtOffset)X
1 f
555 4736(To)N
676(determine)X
1052(the)X
1183(byte)X
1358(offset)X
1581(of)X
1677(a)X
1739(\256eld)X
1919(within)X
2168(a)X
2230(structure,)X
2582(use)X
2721(the)X
2851(macro)X
5 f
3111 0.4554(XtOffset)AX
1 f
3447(.)X
3513(The)X
3672(de\256nition)X
555 4832(for)N
679(this)X
829(macro)X
1071(is:)X
555 5052(Cardinal)N
881(XtOffset\()X
2 f
1217(pointer_type)X
1 f
1663(,)X
2 f
1707(\256eld_name)X
1 f
2093(\))X
687 5148(Type)N
2 f
890(pointer_type)X
1 f
1336(;)X
687 5244(Field)N
2 f
891(\256eld_name)X
1 f
1277(;)X
2 f
555 5492(pointer_type)N
1 f
1131(Speci\256es)X
1471(a)X
1532(type)X
1706(that)X
1861(points)X
2099(to)X
2190(the)X
2320(structure.)X
2 f
555 5616(\256eld_name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(of)X
1909(the)X
2039(\256eld)X
2218(to)X
2309(calculate)X
2645(the)X
2775(byte)X
2949(offset)X
3171(from.)X
5 f
570 5740 0.4554(XtOffset)AN
1 f
931(is)X
1015(usually)X
1295(used)X
1481(to)X
1575(determine)X
1953(the)X
2086(offset)X
2311(of)X
2409(various)X
2693(resource)X
3015(\256elds)X
3231(from)X
3427(the)X
3560(beginning)X
3938(of)X
555 5836(a)N
616(widget.)X
3 f
2239 6144(43)N

44 p
%%Page: 44 46
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(21.2.)N
775(XtNumber)X
1 f
555 796(To)N
692(determine)X
1084(the)X
1230(number)X
1537(of)X
1648(elements)X
2000(in)X
2107(a)X
2184(\256xed-size)X
2564(array,)X
2804(use)X
2959(the)X
3105(macro)X
5 f
3381(XtNumber)X
1 f
3792(.)X
3874(The)X
555 892(de\256nition)N
916(for)X
1040(this)X
1190(macro)X
1432(is:)X
555 1112(Cardinal)N
881(XtNumber\()X
2 f
1286(array)X
1 f
1481(\))X
687 1208(ArrayVariable)N
2 f
1216(array)X
1 f
1411(;)X
2 f
555 1456(array)N
1 f
1131(Speci\256es)X
1471(a)X
1532(\256xed-size)X
1896(array.)X
5 f
570 1580(XtNumber)N
1 f
1012(is)X
1102(used)X
1294(to)X
1394(pass)X
1576(the)X
1714(number)X
2013(of)X
2116(elements)X
2460(in)X
2559(argument)X
2922(lists,)X
3117(resources)X
3478(lists,)X
3673(and)X
3830(other)X
555 1676(counted)N
856(arrays.)X
3 f
555 1868(21.3.)N
775(Resource)X
1143(Lists)X
1 f
555 1992(A)N
645(resource)X
969(entry)X
1177(speci\256es)X
1507(a)X
1573(\256eld)X
1757(in)X
1853(the)X
1988(widget,)X
2277(the)X
2412(textual)X
2680(name)X
2898(and)X
3052(class)X
3250(of)X
3350(the)X
3484(\256eld)X
3667(that)X
3826(argu-)X
555 2088(ment)N
755(lists)X
921(and)X
1071(external)X
1378(default)X
1646(\256le)X
1782(use)X
1922(to)X
2013(refer)X
2200(to)X
2291(the)X
2421(\256eld,)X
2622(and)X
2771(a)X
2832(default)X
3099(value)X
3312(that)X
3467(the)X
3597(\256eld)X
3776(should)X
555 2184(get)N
685(if)X
761(no)X
871(value)X
1084(is)X
1165(speci\256ed.)X
1544(The)X
1703(declaration)X
2117(for)X
2241(the)X
5 f
2389(XtResource)X
1 f
2886(structure)X
3216(is:)X
555 2308(typedef)N
841(struct)X
1059({)X
843 2404(String)N
2 f
1081(resource_name)X
1 f
1622(;)X
843 2500(String)N
2 f
1081(resource_class)X
1 f
1608(;)X
843 2596(String)N
2 f
1081(resource_type)X
1 f
1579(;)X
843 2692(Cardinal)N
2 f
1169(resource_size)X
1 f
1652(;)X
843 2788(Cardinal)N
2 f
1169(resource_offset)X
1 f
1712(;)X
843 2884(String)N
2 f
1081(default_type)X
1 f
1518(;)X
843 2980(caddr_t)N
2 f
1129(default_address)X
1 f
1692(;)X
555 3076(})N
619(XtResource,)X
1078(*XtResourceList;)X
555 3248(The)N
720(resource_name)X
1280(\256eld)X
1465(contains)X
1786(the)X
1921(name)X
2139(used)X
2327(by)X
2442(clients)X
2700(to)X
2796(access)X
3047(the)X
3182(\256eld)X
3366(in)X
3462(the)X
3597(widget.)X
3908(By)X
555 3344(convention,)N
1000(it)X
1081(starts)X
1298(with)X
1486(a)X
1556(lower-case)X
1966(letter)X
2178(and)X
2335(is)X
2424(spelled)X
2704(almost)X
2970(identically)X
3374(to)X
3473(the)X
3611(\256eld)X
3798(name,)X
555 3440(except)N
808(\(underbar,)X
1194(character\))X
1567(is)X
1648(replaced)X
1968(by)X
2078(\(capital)X
2365(character\).)X
2760(For)X
2904(example,)X
3247(the)X
3377(resource)X
3696(name)X
3909(for)X
555 3536(background_pixel)N
1213(is)X
1294(backgroundPixel.)X
555 3660(The)N
717(resource_class)X
1254(\256eld)X
1436(contains)X
1755(the)X
1888(class)X
2084(of)X
2182(the)X
2315(resource.)X
2681(A)X
2769(resource)X
3091(is)X
3175(rarely)X
3405(a)X
3469(widget.)X
3777(There-)X
555 3756(fore,)N
755(a)X
831(resource)X
1165(class)X
1373(is)X
1469(not)X
1618(the)X
1762(same)X
1979(as)X
2088(a)X
2163(widget)X
2439(class,)X
2668(but)X
2817(there)X
3029(are)X
3172(some)X
3394(resemblances.)X
3948(A)X
555 3852(resource)N
874(class)X
1067(offers)X
1293(two)X
1447(functions:)X
10 f
555 3976(g)N
1 f
775(It)X
851(isolates)X
1138(you)X
1292(from)X
1485(different)X
1810(representations)X
2365(that)X
2520(widgets)X
2816(may)X
2990(use)X
3129(for)X
3253(a)X
3314(similar)X
3582(resource)X
10 f
555 4100(g)N
1 f
775(It)X
856(lets)X
1005(you)X
1163(specify)X
1443(several)X
1718(actual)X
1955(resources)X
2312(with)X
2495(a)X
2560(single)X
2797(name.)X
3058(A)X
3147(resource)X
3470(class)X
3667(should)X
3928(be)X
775 4196(chosen)N
1041(to)X
1132(span)X
1315(a)X
1376(group)X
1603(of)X
1698(closely-related)X
2239(\256elds.)X
555 4320(For)N
706(example,)X
1056(a)X
1123(widget)X
1391(may)X
1571(have)X
1765(several)X
2042(resource)X
2367(pixels:)X
2631(background,)X
3096(foreground,)X
3536(border,)X
3815(block)X
555 4416(cursor,)N
824(mouse)X
1082(cursor,)X
1351(and)X
1506(so)X
1612(on.)X
1772(Typically,)X
2161(the)X
2297(background)X
2739(defaults)X
3045(to)X
3141(``white'')X
3480(and)X
3634(everything)X
555 4512(else)N
718(to)X
813(``black''.)X
1190(The)X
1353(resource)X
1676(class)X
1873(for)X
2001(each)X
2188(of)X
2287(these)X
2494(resources)X
2851(should)X
3112(be)X
3221(chosen)X
3491(so)X
3595(that)X
3754(it)X
3830(takes)X
555 4608(the)N
701(minimal)X
1034(number)X
1340(of)X
1450(entries)X
1722(in)X
1828(the)X
1973(resource)X
2307(database)X
2647(to)X
2753(make)X
2981(background)X
3433(``offwhite'')X
3884(and)X
555 4704(everything)N
954(else)X
1113(``darkblue''.)X
555 4828(In)N
654(this)X
808(case,)X
1006(giving)X
1257(background)X
1697(a)X
1761(resource)X
2083(class)X
2279(of)X
5 f
2395(Background)X
1 f
2905(and)X
3057(all)X
3171(the)X
3304(other)X
3510(pixel)X
3712(entries)X
3972(a)X
555 4924(resource)N
891(class)X
1101(of)X
5 f
1231(Foreground)X
1 f
1740(is)X
1838(the)X
1985(right)X
2190(thing)X
2410(to)X
2517(do.)X
2665(\(By)X
2835(convention,)X
3287(resource)X
3622(classes)X
3904(are)X
555 5020(always)N
821(spelled)X
1093(starting)X
1380(with)X
1559(a)X
1620(capital)X
1878(letter.\))X
555 5144(The)N
728(resource_type)X
1257(\256eld)X
1450(is)X
1545(the)X
1689(physical)X
2019(representation)X
2554(type)X
2742(of)X
2851(the)X
2995(resource.)X
3372(By)X
3511(convention,)X
3961(it)X
555 5240(starts)N
774(with)X
964(an)X
1080(upper-case)X
1493(letter)X
1708(and)X
1868(is)X
1960(usually)X
2248(spelled)X
2531(identically)X
2938(to)X
3040(the)X
3180(type)X
3364(name)X
3587(of)X
3692(the)X
3832(\256eld.)X
555 5336(The)N
719(resource)X
1043(type)X
1222(is)X
1308(used)X
1496(when)X
1713(resources)X
2071(are)X
2205(fetched,)X
2512(to)X
2607(convert)X
2897(from)X
3094(the)X
3228(resource)X
3551(database)X
3880(for-)X
555 5432(mat)N
718(\(usually)X
1032(String\))X
1307(or)X
1410(the)X
1548(default)X
1823(resource)X
2150(format)X
2415(\(just)X
2602(about)X
2828(anything\))X
3196(to)X
3295(the)X
3433(desired)X
3717(physical)X
555 5528(representation)N
1076(\(see)X
1239(``Resource)X
1646(Conversions''\).)X
555 5652(The)N
719(resource_size)X
1224(\256eld)X
1408(is)X
1494(the)X
1629(size)X
1793(of)X
1892(the)X
2026(physical)X
2346(representation)X
2871(in)X
2966(bytes.)X
3222(The)X
3385(resource_offset)X
3952(is)X
555 5748(the)N
693(offset)X
923(in)X
1022(bytes)X
1238(of)X
1341(the)X
1479(\256eld)X
1666(within)X
1922(the)X
2060(widget.)X
2374(The)X
2541(macro)X
5 f
2809 0.4554(XtOffset)AX
1 f
3175(in)X
3274(Intrinsic.h)X
3664(should)X
3928(be)X
555 5844(used)N
738(to)X
829(retrieve)X
1120(this)X
1270(value.)X
3 f
2239 6144(44)N

45 p
%%Page: 45 47
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(The)N
720(default_type)X
1189(\256eld)X
1374(is)X
1461(the)X
1597(representation)X
2124(type)X
2304(of)X
2405(the)X
2541(default)X
2813(resource)X
3137(value.)X
3399(If)X
3484(default_type)X
3952(is)X
555 768(different)N
897(from)X
1107(resource_type)X
1639(and)X
1805(the)X
1952(default_type)X
2432(is)X
2529(needed,)X
2838(the)X
2984(resource)X
3319(manager)X
3660(invokes)X
3972(a)X
555 864(conversion)N
963(procedure)X
1336(from)X
1529(default_type)X
1992(to)X
2083(resource_type.)X
555 988(The)N
718(default_address)X
1296(\256eld)X
1479(is)X
1564(the)X
1698(address)X
1987(of)X
2086(the)X
2220(default)X
2491(resource)X
2813(value.)X
3073(The)X
3235(default)X
3505(is)X
3589(used)X
3775(only)X
3957(if)X
555 1084(a)N
616(resource)X
935(is)X
1016(not)X
1151(speci\256ed)X
1486(in)X
1577(the)X
1707(argument)X
2062(list)X
2193(or)X
2288(in)X
2379(the)X
2509(resource)X
2828(database.)X
555 1208(The)N
724(routines)X
5 f
1058(XtSetValues)X
1 f
1591(and)X
5 f
1768(XtGetValues)X
1 f
2311(also)X
2485(use)X
2634(the)X
2774(resource)X
3103(list)X
3244(to)X
3345(set)X
3474(and)X
3632(get)X
3771(widget)X
555 1304(state.)N
783(See)X
932(sections)X
1238(``XtGetValues'')X
1835(and)X
1984(``XtSetValues''.)X
3 f
555 1496(21.4.)N
775(Superclass)X
1198(to)X
1293(Subclass)X
1638(Chaining)X
2008(of)X
2103(Resource)X
2471(Lists)X
1 f
555 1620(The)N
725(procedure)X
5 f
1127(XtCreateWidget)X
1 f
1797(gets)X
1972(resources)X
2335(as)X
2440(a)X
2511(``superclass-to-subclass'')X
3440(operation.)X
3849(That)X
555 1716(is,)N
667(the)X
805(resources)X
1166(speci\256ed)X
1509(in)X
1608(Core's)X
1872(resource)X
2199(list)X
2338(are)X
2475(fetched,)X
2786(then)X
2968(those)X
3184(in)X
3283(the)X
3421(subclass,)X
3766(and)X
3923(on)X
555 1812(down)N
772(to)X
863(the)X
993(resources)X
1346(speci\256ed)X
1681(for)X
1805(this)X
1955(widget's)X
2280(class.)X
555 1936(In)N
658(general,)X
969(if)X
1053(a)X
1122(widget)X
1392(\256eld)X
1579(is)X
1668(declared)X
1996(in)X
2095(a)X
2164(superclass,)X
2577(that)X
2740(\256eld)X
2927(is)X
3016(included)X
3350(in)X
3449(the)X
3587(superclass's)X
555 2032(resource)N
876(list)X
1009(and)X
1160(need)X
1350(not)X
1487(be)X
1594(included)X
1922(in)X
2015(the)X
2147(subclass's)X
2527(resource)X
2848(list.)X
3025(For)X
3171(example,)X
3515(the)X
3646(Core)X
3840(class)X
555 2128(contains)N
882(a)X
954(resource)X
1284(entry)X
1498(for)X
1632(background_pixel.)X
2322(Consequently,)X
2861(the)X
3001 0.2885(implementation)AX
3589(of)X
3694(``Label'')X
555 2224(need)N
748(not)X
888(also)X
1057(have)X
1250(a)X
1316(resource)X
1640(entry)X
1848(for)X
1977(background_pixel.)X
2684(However,)X
3054(a)X
3119(subclass,)X
3460(just)X
3614(by)X
3728(specify-)X
555 2320(ing)N
695(a)X
761(resource)X
1085(entry)X
1293(for)X
1421(that)X
1580(\256eld)X
1763(in)X
1858(its)X
1968(own)X
2145(resource)X
2468(list,)X
2625(can)X
2773(override)X
3092(the)X
3226(resource)X
3549(entry)X
3756(for)X
3884(any)X
555 2416(\256eld)N
734(declared)X
1054(in)X
1145(a)X
1206(superclass.)X
1611(The)X
1770(translations)X
2199(\256eld)X
2378(is)X
2459(frequently)X
2843(overridden)X
3246(by)X
3356(subclasses.)X
3 f
555 2608(21.5.)N
775(XtGetSubresources)X
1 f
555 2732(A)N
643(widget)X
908(needs)X
1133(to)X
1227(do)X
1339(nothing)X
1633(to)X
1726(get)X
1858(its)X
1966(resources.)X
2365(That)X
2551(is,)X
5 f
2674(XtCreateWidget)X
1 f
3335(does)X
3520(this)X
3672(automati-)X
555 2828(cally)N
752(before)X
1001(calling)X
1267(the)X
1400 0.3542(initialize)AX
1736(procedure.)X
2156(Some)X
2382(widgets)X
2681(have)X
2872(subparts)X
3190(which)X
3430(are)X
3562(not)X
3700(children,)X
555 2924(but)N
691(for)X
816(which)X
1054(the)X
1185(widget)X
1447(would)X
1689(like)X
1844(to)X
1935(fetch)X
2133(resources.)X
2530(These)X
2762(widgets)X
3058(call)X
5 f
3226(XtGetSubresources)X
1 f
555 3020(to)N
646(accomplish)X
1070(this.)X
1264(The)X
1423(de\256nition)X
1784(for)X
1908(this)X
2058(function)X
2374(is:)X
555 3240(void)N
734(XtGetSubresources\()X
2 f
1446(w)X
1 f
1505(,)X
2 f
1549(base)X
1 f
1710(,)X
2 f
1754(name)X
1 f
1944(,)X
2 f
1988(class)X
1 f
(,)S
2 f
2208(resources)X
1 f
2549(,)X
2 f
2593(num_resources)X
1 f
3129(,)X
2 f
3173(args)X
1 f
3329(,)X
2 f
3373(num_args)X
1 f
3724(\))X
687 3336(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3432(caddr_t)N
2 f
973(base)X
1 f
1134(;)X
687 3528(String)N
2 f
925(name)X
1 f
1115(;)X
687 3624(String)N
2 f
925(class)X
1 f
(;)S
687 3720(XtResourceList)N
2 f
1262(resources)X
1 f
1603(;)X
687 3816(Cardinal)N
2 f
1013(num_resources)X
1 f
1549(;)X
687 3912(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 4008(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
2 f
555 4256(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(that)X
2018(wants)X
2245(resources)X
2598(for)X
2722(a)X
2783(subpart.)X
2 f
555 4380(base)N
1 f
1131(Speci\256es)X
1494(the)X
1647(base)X
1848(address)X
2155(of)X
2272(the)X
2424(subpart)X
2727(data)X
2918(structure)X
3270(where)X
3528(the)X
3680(resources)X
1131 4476(should)N
1388(be)X
1493(written.)X
2 f
555 4600(name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(of)X
1909(the)X
2039(subpart.)X
2 f
555 4724(class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(class)X
1794(of)X
1889(the)X
2019(subpart.)X
2 f
555 4848(resources)N
1 f
1131(Speci\256es)X
1471(the)X
1601(resource)X
1920(list)X
2051(for)X
2175(the)X
2305(subpart.)X
2 f
555 4972(num_resources)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(resources)X
2340(in)X
2 f
2431(resources)X
1 f
2772(.)X
2 f
555 5096(args)N
1 f
1131(Speci\256es)X
1473(the)X
1605(argument)X
1962(list)X
2095(to)X
2188(override)X
2505(resources)X
2860(obtained)X
3188(from)X
3383(the)X
3515(resource)X
3835(data-)X
1131 5192(base.)N
2 f
555 5316(num_args)N
1 f
1131(Speci\256es)X
1482(the)X
1623(number)X
1925(of)X
2031(arguments)X
2430(in)X
2531(the)X
2671(argument)X
3036(list.)X
3199(If)X
3289(the)X
3429(speci\256ed)X
3774(args)X
3952(is)X
1131 5412(NULL,)N
1413(num_args)X
1786(must)X
1984(be)X
2093(zero)X
2270(\(0\).)X
2442(However,)X
2811(if)X
2891(num_args)X
3264(is)X
3349(zero)X
3525(\(0\),)X
3674(args)X
3845(need)X
1131 5508(not)N
1266(be)X
1371(NULL.)X
5 f
570 5632(XtGetSubresources)N
1 f
1386(constructs)X
1774(a)X
1844(name/class)X
2262(list)X
2402(from)X
2603(the)X
2741(widget)X
3011(\(and)X
3197(all)X
3316(ancestors\))X
3702(and)X
3859(then)X
555 5728(appends)N
875(the)X
1015(name/class)X
1433(pair)X
1601(passed)X
1866(in.)X
2010(The)X
2178(resources)X
2540(are)X
2678(fetched)X
2968(from)X
3170(the)X
3309(argument)X
3673(list,)X
3835(data-)X
555 5824(base,)N
755(or)X
850(default)X
1117(in)X
1208(the)X
1338(resource)X
1657(list)X
1788(and)X
1937(are)X
2066(copied)X
2323(into)X
2483(the)X
2613(subpart)X
2894(record.)X
3 f
2239 6144(45)N

46 p
%%Page: 46 48
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(21.6.)N
775(Resource)X
1143(Conversions)X
1 f
555 796(The)N
716(X)X
802(Toolkit)X
1086(de\256nes)X
1358(all)X
1470(the)X
1601(resources)X
1955(and)X
2105(representations)X
2661(used)X
2845(in)X
2937(the)X
3068(Core)X
3262(widget.)X
3569(It)X
3646(also)X
3811(regis-)X
555 892(ters)N
710(some)X
924(resource)X
1249(conversions)X
1697(in)X
1794(addition)X
2112(to)X
2209(the)X
2345(standard)X
2671(resource)X
2996(conversions)X
3443(described)X
3807(in)X
3903(the)X
555 988(Xlib)N
734(manual.)X
3 f
555 1180(22.)N
709(Reading)X
1044(and)X
1208(Writing)X
1529(Widget)X
1825(State)X
1 f
555 1304(Any)N
733(resource)X
1056(\256eld)X
1239(in)X
1334(a)X
1399(widget)X
1665(can)X
1813(be)X
1922(read)X
2099(or)X
2198(written)X
2474(by)X
2588(a)X
2653(client.)X
2920(On)X
3053(a)X
3118(write,)X
3347(the)X
3481(widget)X
3747(decides)X
555 1400(what)N
748(changes)X
1053(it)X
1125(will)X
1285(actually)X
1587(allow)X
1805(and)X
1954(updates)X
2245(all)X
2356(derived)X
2642(\256elds)X
2855(appropriately.)X
3 f
555 1592(22.1.)N
775(XtGetValues)X
1 f
555 1716(To)N
679(retrieve)X
974(the)X
1108(current)X
1383(value)X
1600(of)X
1698(a)X
1762(resource)X
2084(associated)X
2471(with)X
2653(a)X
2717(widget)X
2982(instance,)X
3318(use)X
5 f
3478(XtGetValues)X
1 f
3989(.)X
555 1812(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 2032(void)N
734(XtGetValues\()X
2 f
1222(w)X
1 f
1281(,)X
2 f
1325(args)X
1 f
1481(,)X
2 f
1525(num_args)X
1 f
1876(\))X
687 2128(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 2224(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 2320(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
2 f
555 2568(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 2692(args)N
1 f
1131(Speci\256es)X
1474(a)X
1538(variable)X
1847(length)X
2093(argument)X
2451(list)X
2585(of)X
2682(name/address)X
3185(pairs)X
3380(which)X
3619(contain)X
3903(the)X
1131 2788(resource)N
1459(name)X
1681(and)X
1839(the)X
1978(address)X
2272(to)X
2372(store)X
2574(the)X
2713(resource)X
3041(value)X
3263(into.)X
3476(The)X
3644(arguments)X
1131 2884(and)N
1284(values)X
1534(passed)X
1793(in)X
1887(args)X
2058(are)X
2190(dependent)X
2577(on)X
2690(the)X
2823(widget.)X
3110(The)X
3272(storage)X
3551(for)X
3678(argument)X
1131 2980(values)N
1382(which)X
1623(are)X
1756(pointed)X
2047(to)X
2141(by)X
2254(args)X
2425(must)X
2622(be)X
2730(deallocated)X
3157(by)X
3270(the)X
3403(application)X
3821(when)X
1131 3076(no)N
1241(longer)X
1488(needed.)X
2 f
555 3200(num_args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arguments)X
2376(in)X
2467(argument)X
2822(list.)X
5 f
570 3324(XtGetValues)N
1 f
1113(starts)X
1331(with)X
1520(the)X
1660(resources)X
2023(speci\256ed)X
2368(for)X
2501(the)X
2640(core)X
2822(widget)X
3093(\256elds)X
3315(and)X
3473(proceeds)X
3816(down)X
555 3420(the)N
685(subclass)X
1000(chain)X
1213(to)X
1304(the)X
1434(widget.)X
3 f
555 3612(22.2.)N
775(XtSetValues)X
1 f
555 3736(To)N
695(modify)X
992(the)X
1141(current)X
1431(value)X
1663(of)X
1777(an)X
1901(argument)X
2275(associated)X
2678(with)X
2876(a)X
2956(widget)X
3237(instance,)X
3589(use)X
5 f
3765(XtSet-)X
555 3832(Values)N
1 f
839(.)X
905(The)X
1064(de\256nition)X
1425(for)X
1549(this)X
1699(function)X
2015(is:)X
555 4052(void)N
734(XtSetValues\()X
2 f
1208(w)X
1 f
1267(,)X
2 f
1311(args)X
1 f
1467(,)X
2 f
1511(num_args)X
1 f
1862(\))X
687 4148(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 4244(ArgList)N
2 f
983(args)X
1 f
1139(;)X
687 4340(Cardinal)N
2 f
1013(num_args)X
1 f
1364(;)X
2 f
555 4588(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 4712(args)N
1 f
1131(Speci\256es)X
1481(a)X
1551(variable)X
1866(length)X
2118(argument)X
2482(list)X
2622(of)X
2726(name/value)X
3164(pairs)X
3366(which)X
3612(contain)X
3903(the)X
1131 4808(arguments)N
1543(to)X
1657(be)X
1785(modi\256ed)X
2143(and)X
2314(their)X
2520(new)X
2710(values.)X
3023(The)X
3204(arguments)X
3615(and)X
3786(values)X
1131 4904(passed)N
1387(are)X
1516(dependent)X
1900(on)X
2010(the)X
2140(widget)X
2402(being)X
2620(modi\256ed.)X
2 f
555 5028(num_args)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arguments)X
2376(in)X
2467(the)X
2597(argument)X
2952(list.)X
5 f
570 5152(XtSetValues)N
1 f
1094(starts)X
1303(with)X
1483(the)X
1613(resources)X
1966(speci\256ed)X
2301(for)X
2425(the)X
2555(core)X
2728(widget)X
2990(\256elds)X
3203(and)X
3352(proceeds)X
3686(down)X
3903(the)X
555 5248(subclass)N
879(chain)X
1101(to)X
1201(the)X
1340(widget.)X
1655(At)X
1774(each)X
1966(stage,)X
2200(it)X
2281(writes)X
2527(the)X
2666(new)X
2843(value)X
3065(\(if)X
3179(speci\256ed)X
3523(by)X
3642(one)X
3800(of)X
3903(the)X
555 5344(arguments\))N
973(or)X
1068(the)X
1198(existing)X
1500(value)X
1713(\(if)X
1818(no)X
1928(new)X
2096(value)X
2309(is)X
2390(speci\256ed\))X
2754(to)X
2845(a)X
2906(new)X
3074(widget)X
3336(data)X
3505(record.)X
5 f
570 5468(XtSetValues)N
1 f
1106(then)X
1293(calls)X
1490(the)X
1633(set_values)X
2035(procedures)X
2455(for)X
2592(the)X
2735(widget)X
3010(in)X
3114(``superclass-to-subclass'')X
555 5564(order.)N
3 f
2239 6144(46)N

47 p
%%Page: 47 49
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(22.2.1.)N
841(Setting)X
1127(Widget)X
1423(State:)X
1664(the)X
1803(set_values)X
2206(procedure)X
1 f
555 892(typedef)N
841(Boolean)X
1157(\(*XtSetValuesProc\)\(\);)X
687 988(Widget)N
2 f
969(current)X
1 f
1228(;)X
687 1084(Widget)N
2 f
969(request)X
1 f
1228(;)X
687 1180(Widget)N
2 f
969(new)X
1 f
1111(;)X
687 1276(Boolean)N
2 f
1003(last)X
1 f
1131(;)X
2 f
555 1524(current)N
1 f
1131(Speci\256es)X
1471(the)X
1601(existing)X
1903(widget.)X
2 f
555 1648(request)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(asked)X
2085(for)X
2209(by)X
2319(the)X
5 f
2467(XtSetValues)X
1 f
2990(call.)X
2 f
555 1772(new)N
1 f
1131(Speci\256es)X
1471(a)X
1532(widget)X
1794(with)X
1973(the)X
2103(new)X
2271(values)X
2518(that)X
2673(are)X
2802(actually)X
3104(allowed.)X
2 f
555 1896(last)N
1 f
1131(Speci\256es)X
1483(if)X
1571(this)X
1733(is)X
1826(the)X
1968(last)X
2125(set_values)X
2526(procedure)X
2911(in)X
3014(the)X
3156(chain)X
3381(called)X
3626(by)X
5 f
3765(XtSet-)X
1131 1992(Values)N
1 f
1415(.)X
555 2116(The)N
726(set_values)X
1127(procedure)X
1512(should)X
1780(construct)X
2136(any)X
2296(composite)X
2692(resources)X
3056(that)X
3222(depend)X
3509(upon)X
3718(changed)X
555 2212(values)N
815(\(for)X
981(example,)X
1337(many)X
1567(GCs)X
1757(depend)X
2045(upon)X
2255(foreground)X
2679(and)X
2840(background\).)X
3362(Like)X
3558(the)X
3700 0.3542(initialize)AX
555 2308(procedure,)N
967(set_values)X
1373(mostly)X
1653(deals)X
1873(only)X
2069(with)X
2265(the)X
2412(\256elds)X
2642(de\256ned)X
2940(in)X
3048(the)X
3195(subclass,)X
3548(but)X
3699(it)X
3787(has)X
3942(to)X
555 2404(resolve)N
834(con\257icts)X
1158(with)X
1340(superclass,)X
1748(especially)X
2126(con\257icts)X
2450(over)X
2631(width)X
2857(and)X
3009(height.)X
3298(In)X
3395(this)X
3547(case,)X
3744(though,)X
555 2500(the)N
685(``reference'')X
1149(widget)X
1411(is)X
1492(``current'',)X
1901(not)X
2036(``request''.)X
555 2624(A)N
647(``new'')X
938(starts)X
1153(with)X
1339(the)X
1476(values)X
1730(of)X
1832(``request'')X
2231(and)X
2387(can)X
2538(be)X
2650(modi\256ed)X
2993(by)X
3110(the)X
3247(superclass.)X
3680(A)X
3771(widget)X
555 2720(need)N
754(not)X
900(refer)X
1098(to)X
1200(request)X
1486(unless)X
1738(it)X
1820(must)X
2024(resolve)X
2310(con\257icts)X
2641(between)X
2966(``current'')X
3363(and)X
3522(``new''.)X
3860(Any)X
555 2816(changes)N
863(that)X
1021(the)X
1154(widget)X
1419(wishes)X
1683(to)X
1777(make)X
1993(should)X
2253(be)X
2361(made)X
2577(in)X
2670(``new''.)X
5 f
3021(XtSetValues)X
1 f
3546(will)X
3708(copy)X
3903(the)X
555 2912(``new'')N
850(values)X
1108(back)X
1307(into)X
1478(the)X
1619(``current'')X
2016(widget)X
2288(instance)X
2609(record)X
2865(after)X
3058(class)X
3261(SetValuesProcs)X
3845(have)X
555 3008(been)N
743(called.)X
555 3132(If)N
651(last)X
812(is)X
909(TRUE,)X
1199(the)X
1345(set_values)X
1750(procedure)X
2139(knows)X
2406(that)X
2577(it)X
2665(is)X
2761(last)X
2921(in)X
3027(the)X
3172(``superclass-to-subclass)X
555 3228(chain''.)N
852(In)X
951(this)X
1105(case,)X
1304(it)X
1380(should)X
1641(perform)X
1949(any)X
2101(\256nal)X
2283(actions.)X
2580(In)X
2678(particular,)X
3063(it)X
3138(should)X
3398(make)X
3614(a)X
3678(geometry)X
555 3324(request,)N
853(if)X
929(needed.)X
555 3448(Finally,)N
856(the)X
992(set_values)X
1387(procedure)X
1766(must)X
1966(return)X
2204(a)X
2271(Boolean)X
2593(that)X
2754(indicates)X
3096(whether)X
3407(the)X
3543(widget)X
3811(needs)X
555 3544(to)N
653(be)X
765(redisplayed.)X
1244(Note)X
1444(that)X
1606(a)X
1674(change)X
1952(in)X
2050(the)X
2187(geometry)X
2549(\256elds)X
2769(alone)X
2989(does)X
3179(not)X
3321(require)X
3599(a)X
3666(redisplay.)X
555 3640(The)N
737(X)X
845(server)X
1104(will)X
1287(eventually)X
1700(generate)X
2042(an)X
2169(Expose)X
2472(event,)X
2729(if)X
2827(necessary.)X
3256(After)X
3485(calling)X
3770(all)X
3903(the)X
555 3736(set_values)N
952(procedures,)X
5 f
1407(XtSetValues)X
1 f
1938(will)X
2106(force)X
2316(a)X
2385(redisplay)X
2738(\(by)X
2884(calling)X
5 f
3172(XClearArea)X
1 f
3641(\))X
3699(if)X
3782(any)X
3938(of)X
555 3832(the)N
687(set_values)X
1078(procedures)X
1486(returned)X
1802(TRUE.)X
2099(Therefore,)X
2490(a)X
2552(set_values)X
2942(procedure)X
3316(should)X
3574(not)X
3710(try)X
3831(to)X
3923(do)X
555 3928(its)N
661(own)X
834(redisplaying.)X
3 f
555 4120(22.2.2.)N
841(XtSetValuesGeometryRequest)X
1 f
555 4244(To)N
689(make)X
916(a)X
991(geometry)X
1360(request)X
1650(from)X
1857(a)X
1932(set_values)X
2334(procedure,)X
2742(you)X
2909(can)X
3066(use)X
3218(the)X
3361(procedure)X
5 f
3765(XtSet-)X
555 4340(ValuesGeometryRequest)N
1 f
1554(.)X
1620(The)X
1779(de\256nition)X
2140(for)X
2264(this)X
2414(function)X
2730(is:)X
555 4560(XtGeometryResult)N
1243(XtSetValuesGeometryRequest\()X
2 f
2353(current)X
1 f
2612(,)X
2 f
2656(new)X
1 f
2798(,)X
2 f
2842(reply)X
1 f
3023(\))X
687 4656(Widget)N
2 f
969(current)X
1 f
1228(;)X
687 4752(Widget)N
2 f
969(new)X
1 f
1111(;)X
687 4848(XtWidgetGeometry)N
1409(*)X
2 f
(reply)S
1 f
1634(;)X
2 f
555 5096(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(as)X
1958(it)X
2030(exists.)X
2 f
555 5220(new)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget)X
1863(as)X
1958(it)X
2030(should)X
2287(be.)X
2 f
555 5344(reply)N
1 f
1131(Speci\256es)X
1471(the)X
1601(geometry)X
1956(allowed)X
2257(if)X
2333(result)X
2551(is)X
5 f
2650(XtGeometryAlmost)X
1 f
3405(.)X
5 f
570 5468(XtSetValuesGeometryRequest)N
1 f
1825(makes)X
2088(a)X
2165(geometry)X
2536(request)X
2828(if)X
2920(one)X
3085(of)X
3196(the)X
3342(geometry)X
3713(\256elds)X
3942(in)X
555 5564(current)N
843(is)X
941(different)X
1283(from)X
1493(the)X
1640(\256elds)X
1870(in)X
1978(new.)X
2206(\(If)X
2331(no)X
2457(geometry)X
2828(request)X
3120(is)X
3217(needed)X
3504(is)X
3601(just)X
3767(returns)X
5 f
570 5660(XtGeometryNo)N
1 f
1168(.\))X
3 f
2239 6144(47)N

48 p
%%Page: 48 50
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(Note)N
753(that)X
913(only)X
1097(the)X
1232(current)X
1508(widget)X
1775(values)X
2027(will)X
2192(be)X
2302(updated.)X
2652(Do)X
2786(not)X
2926(worry)X
3162(about)X
3385(this.)X
3584(The)X
5 f
3765(XtSet-)X
555 768(Values)N
1 f
861(procedure)X
1234(knows)X
1485(that)X
1640(it)X
1712(should)X
1969(never)X
2186(just)X
2336(overwrite)X
2695(the)X
2825(widget's)X
3150(geometry)X
3505(\256elds.)X
2197 944(Note)N
775 1080(This)N
954(routine)X
1226(currently)X
1566(is)X
1647(not)X
1782(yet)X
1912(implemented.)X
3 f
555 1352(23.)N
709(Translation)X
1172(of)X
1267(X)X
1352(Events)X
1628(to)X
1723(Program/Widget)X
2381(functionality)X
1 f
555 1476(Except)N
834(under)X
1067(unusual)X
1374(circumstances,)X
1928(widgets)X
2235(do)X
2356(not)X
2502(hardwire)X
2847(the)X
2988(mapping)X
3330(of)X
3436(user)X
3615(events)X
3873(into)X
555 1572(widget)N
829(behavior)X
1171(by)X
1293(using)X
1518(the)X
1660(Event)X
1900(Manager.)X
2289(Instead,)X
2598(they)X
2783(provide)X
3085(a)X
3157(user-overridable)X
3766(default)X
555 1668(mapping)N
886(of)X
981(events)X
1228(into)X
1388(behavior.)X
555 1792(The)N
726(translation)X
1133(manager)X
1470(provides)X
1807(an)X
1923(interface)X
2264(to)X
2366(specify)X
2653(and)X
2813(manage)X
3120(the)X
3261(mapping)X
3603(of)X
3709(X)X
3805(Event)X
555 1888(sequences)N
942(into)X
1111(widget-supplied)X
1710(functionality.)X
2215(The)X
2383(simplest)X
2709(example)X
3039(would)X
3290(be)X
3403(to)X
3502(call)X
3660(procedure)X
555 1984(Foo)N
714(when)X
926(key)X
1075(``y'')X
1257(is)X
1338(pressed.)X
555 2108(The)N
717(translation)X
1115(manager)X
1443(uses)X
1619(two)X
1776(tables)X
2007(to)X
2101(perform)X
2409(translations.)X
2885(The)X
3046(``action)X
3344(table'',)X
3620(which)X
3859(is)X
3942(in)X
555 2204(the)N
686(widget)X
949(class)X
1143(structure,)X
1496(speci\256es)X
1822(the)X
1953(mapping)X
2285(of)X
2381(externally)X
2757(available)X
3099(procedure)X
3472(name)X
3685(strings)X
3942(to)X
555 2300(the)N
687(corresponding)X
1214(procedure)X
1589(implemented)X
2075(by)X
2187(the)X
2319(widget)X
2583(class.)X
2822(The)X
2983(``translation)X
3438(table'',)X
3714(which)X
3952(is)X
555 2396(in)N
646(the)X
776(widget)X
1038(instance)X
1349(structure,)X
1701(speci\256es)X
2026(the)X
2156(mapping)X
2487(of)X
2582(event)X
2795(sequence)X
3139(to)X
3230(procedure)X
3603(names.)X
555 2520(For)N
699(example,)X
1042(the)X
1172(action)X
1410(table)X
1604(for)X
1728(the)X
1858(Command)X
2248(widget)X
2510(class)X
2703(is)X
2784(de\256ned)X
3065(as:)X
555 2644(XtActionsRec)N
1076(actionTable[])X
1573(=)X
1645({)X
843 2740({"Set",)N
1419(\(caddr_t\))X
1763(Set},)X
843 2836({"Unset",)N
1419(\(caddr_t\))X
1763(Unset},)X
843 2932({"Highlight",)N
1419(\(caddr_t\))X
1763(Highlight},)X
843 3028({"Unhighlight",)N
1419(\(caddr_t\))X
1763(Unhighlight})X
843 3124({"Notify",)N
1419(\(caddr_t\))X
1763(Notify},)X
555 3220(};)N
555 3392(The)N
714(default)X
981(behavior)X
1311(of)X
1406(Command)X
1796(is:)X
10 f
555 3516(g)N
1 f
775(Higlighted)X
1175(on)X
1285(enter)X
1483(window)X
10 f
555 3640(g)N
1 f
775(Unhighlighted)X
1307(on)X
1417(exit)X
1572(window)X
10 f
555 3764(g)N
1 f
775(Invert)X
1007(on)X
1117(left)X
1257(button)X
1505(down)X
10 f
555 3888(g)N
1 f
775(Call)X
945(callbacks)X
1295(and)X
1444(reinvert)X
1740(on)X
1850(left)X
1990(button)X
2238(up)X
555 4012(Command's)N
1008(default)X
1275(translation)X
1670(table)X
1864(is:)X
555 4136(static)N
764(char)X
937(*defaultTranslations[])X
1742(=)X
1814({)X
843 4232("<EnterWindow>:Highlight\(\)",)N
843 4328("<LeaveWindow>:Unhighlight\(\)",)N
843 4424("<Btn1Down>:)N
1419(Set\(\)",)X
843 4520("<Btn1Up>:)N
1419(Notify\(\))X
1729(Unset\(\)",)X
843 4616(NULL)N
555 4712(};)N
555 4884(For)N
699(details)X
952(on)X
1062(the)X
1192(syntax)X
1444(of)X
1539(translation)X
1934(tables,)X
2184(see)X
2318(Appendix)X
2687(B.)X
5 f
570 5008(XtCreateWidget)N
1 f
1230(calls)X
1415(an)X
1521(internal)X
1814(routine)X
2087(to)X
2179(compile)X
2487(the)X
2618(translation)X
3014(speci\256cation)X
3482(into)X
3642(a)X
3703(structure)X
555 5104(that)N
719(ef\256ciently)X
1107(maps)X
1323(incoming)X
1687(events)X
1942(into)X
2110(the)X
2248(speci\256ed)X
2591(procedure)X
2972(calls)X
3164(and)X
3321(registers)X
3649(the)X
3787(neces-)X
555 5200(sary)N
730(events)X
984(with)X
1170(the)X
1307(Event)X
1541(Manager.)X
1903(Widgets)X
2225(need)X
2419(do)X
2535(nothing)X
2833(other)X
3042(than)X
3222(specify)X
3504(the)X
3640(action)X
3884(and)X
555 5296(translation)N
950(tables.)X
3 f
555 5488(23.0.1.)N
841(Translation)X
1304(Tables)X
1 f
555 5612(Mapping)N
913(the)X
1061(X)X
1164(Event)X
1410(to)X
1519(procedure)X
1910(calls)X
2112(is)X
2211(accomplished)X
2736(by)X
2863(specifying)X
3269(a)X
3347(Translation)X
3788(Table.)X
555 5708(Translation)N
982(tables)X
1213(exist)X
1405(in)X
1499(textual)X
1765(\(external\))X
2132(and)X
2284(compiled)X
2638(\(internal\))X
2991(forms.)X
3243(The)X
3404(compiled)X
3757(form)X
3952(is)X
555 5804(private)N
831(to)X
931(the)X
1069(translation)X
1472(manager)X
1805(and)X
1962(contains)X
2286(the)X
2424(state)X
2616(tables,)X
2874(the)X
3012(current)X
3291(state,)X
3505(and)X
3662(actions)X
3942(to)X
3 f
2239 6144(48)N

49 p
%%Page: 49 51
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(perform)N
860(for)X
984(each)X
1167(state.)X
1395(The)X
1554(textual)X
1817(form's)X
2073(syntax)X
2325(is)X
2406(described)X
2765(in)X
2856(Appendix)X
3225(B.)X
3 f
555 864(23.0.2.)N
841(Function)X
1201(table)X
1 f
555 988(Clients)N
834(of)X
935(the)X
1071(translation)X
1472(manager)X
1803(must)X
2003(provide)X
2300(a)X
2367(table)X
2567(\(array\))X
2833(of)X
2934(procedure)X
3312(names)X
3564(to)X
3660(procedure)X
555 1084 0.2500(implementations)AN
1167(using)X
1380(the)X
1510(following)X
1875(format.)X
555 1208(typedef)N
841(struct)X
1059(_XtActionsRec)X
1624({)X
843 1304(char)N
1016(*string;)X
843 1400(caddr_t)N
1129(value;)X
555 1496(})N
619(XtActionsRec,)X
1162(*XtActionList;)X
555 1640(The)N
715(string)X
939(\256eld)X
1119(is)X
1200(the)X
1330(name)X
1543(of)X
1638(the)X
1768(exported)X
2098(function.)X
2436(The)X
2595(value)X
2808(\256eld)X
2987(is)X
3068(a)X
3129(pointer)X
3401(to)X
3492(any)X
3641(client)X
3860(sup-)X
555 1736(plied)N
770(data.)X
999(The)X
1174(most)X
1384(common)X
1731(use)X
1886(of)X
1997(this)X
2163(is)X
2260(to)X
2367(supply)X
2640(a)X
2717(procedure)X
3106(adress)X
3363(for)X
3503(the)X
3648(name,)X
3898(but)X
555 1832(Actions)N
851(can)X
995(map)X
1169(onto)X
1348(strings,)X
1627(chars,)X
1856(procedures,)X
2285(atoms,)X
2540(or)X
2635(numbers.)X
555 1956(Each)N
755(widget)X
1019(class)X
1214(has)X
1355(a)X
1418(table)X
1614(of)X
1711(name-to-procedure)X
2403(bindings)X
2730(for)X
2855(that)X
3011(class.)X
3249(In)X
3345(addition,)X
3680(the)X
3811(trans-)X
555 2052(lation)N
781(manager)X
1107(provides)X
1433(a)X
1495(global)X
1739(name)X
1953(to)X
2045(function)X
2362(table)X
2557(containing)X
2953(the)X
3084(de\256nition)X
3446(of)X
3542(standard)X
3863(util-)X
555 2148(ity)N
684(procedures)X
1104(for)X
1241(setting)X
1512(timers,)X
1790(and)X
1952(the)X
2095(current)X
2379(values)X
2639(of)X
2747(global)X
3002(values)X
3261(like)X
3428(the)X
3570(double-click)X
555 2244(time.)N
3 f
555 2436(24.)N
709(Graphics)X
1079(Contexts)X
1 f
555 2560(The)N
721(X)X
813(Toolkit)X
1103(provides)X
1435(a)X
1503(mechanism)X
1934(whereby)X
2264(cooperating)X
2708(clients)X
2967(can)X
3117(share)X
3330(Graphics)X
3675(Contexts,)X
555 2656(thus,)N
750(reducing)X
1083(both)X
1265(the)X
1398(number)X
1692(of)X
1790(Graphics)X
2132(Contexts)X
2471(created)X
2750(and)X
2902(the)X
3035(total)X
3218(number)X
3512(of)X
3610(server)X
3849(calls)X
555 2752(in)N
650(any)X
803(give)X
981(application.)X
1422(The)X
1585(mechanism)X
2013(implemented)X
2500(is)X
2584(a)X
2648(simple)X
2909(caching)X
3208(scheme)X
3497(and)X
3649(all)X
3763(Graph-)X
555 2848(ics)N
680(Contexts)X
1021(obtained)X
1352(by)X
1467(means)X
1719(of)X
1819(this)X
1974(mechanism)X
2403(must)X
2602(be)X
2712(treated)X
2979(as)X
3079(read-only.)X
3465(If)X
3550(a)X
3615(changeable)X
555 2944(Graphics)N
894(Context)X
1196(is)X
1277(needed,)X
1570(the)X
5 f
1718(XCreateGC)X
1 f
2210(Xlib)X
2389(function)X
2705(should)X
2962(be)X
3067(used)X
3250(instead.)X
555 3068(To)N
675(obtain)X
918(shared)X
1169(GCs,)X
1369(use)X
5 f
1526(XtGetGC)X
1 f
1900(.)X
1966(The)X
2125(de\256nition)X
2486(of)X
2581(this)X
2731(function)X
3047(is:)X
555 3288(GC)N
699(XtGetGC\()X
2 f
1065(w)X
1 f
1124(,)X
2 f
1168(value_mask)X
1 f
1583(,)X
2 f
1627(values)X
1 f
1852(\))X
687 3384(Widget)N
2 f
969(w)X
1 f
1028(;)X
687 3480(XtGCMask)N
2 f
1114(value_mask)X
1 f
1529(;)X
687 3576(XGCValues)N
1138(*)X
2 f
(values)S
1 f
1407(;)X
2 f
555 3824(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(widget.)X
2 f
555 3948(value_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(\256elds)X
1921(of)X
2016(the)X
2146(values)X
2393(are)X
2522(speci\256ed.)X
2879(\(See)X
5 f
3075(XCreateGC)X
1 f
3545(.\))X
2 f
555 4072(values)N
1 f
1131(Speci\256es)X
1471(the)X
1601(actual)X
1834(values)X
2081(for)X
2205(this)X
2355(GC.)X
2521(\(See)X
5 f
2717(XCreateGC)X
1 f
3187(.\))X
5 f
570 4196(XtGetGC)N
1 f
971(returns)X
1242(a)X
1308(Graphics)X
1652(Context.)X
1980(The)X
2143(parameters)X
2555(to)X
2650(this)X
2804(function)X
3124(are)X
3257(the)X
3391(same)X
3598(as)X
3697(those)X
3909(for)X
5 f
570 4292(XCreateGC)N
1 f
1040(,)X
1084(except)X
1336(that)X
1491(a)X
1552(widget)X
1814(is)X
1895(passed)X
2151(instead)X
2423(of)X
2518(a)X
2579(Display.)X
5 f
570 4416(XtGetGC)N
1 f
981(shares)X
1237(only)X
1431(GCs)X
1624(in)X
1730(which)X
1982(all)X
2108(values)X
2370(in)X
2476(the)X
2621(GC)X
2780(returned)X
3110(by)X
5 f
3253(XCreateGC)X
1 f
3760(are)X
3903(the)X
555 4512(same.)N
784(In)X
883(particular,)X
1269(it)X
1345(does)X
1532(not)X
1671(use)X
1813(the)X
1946(value_mask)X
2392(provided)X
2730(to)X
2824(determine)X
3202(which)X
3442(\256elds)X
3658(of)X
3756(the)X
3889(GC)X
555 4608(a)N
617(widget)X
879(considers)X
1233(relevant.)X
1583(The)X
1742(value_mask)X
2185(is)X
2266(used)X
2449(only)X
2628(to)X
2719(tell)X
2855(the)X
2985(server)X
3221(which)X
3458(\256elds)X
3671(should)X
3928(be)X
555 4704(\256lled)N
759(in)X
850(with)X
1029(widget)X
1291(data)X
1460(and)X
1609(which)X
1846(it)X
1918(should)X
2175(\256ll)X
2296(in)X
2387(with)X
2566(default)X
2833(values.)X
555 4828(When)N
792(a)X
858(Graphics)X
1202(Context)X
1509(is)X
1594(no)X
1708(longer)X
1959(needed)X
2234(it)X
2310(should)X
2571(be)X
2680(deallocated)X
3108(using)X
3325(the)X
5 f
3477(XtDestroyGC)X
1 f
555 4924(procedure.)N
950(The)X
1109(de\256nition)X
1470(for)X
1594(this)X
1744(procedure)X
2117(is:)X
555 5144(void)N
734(XtDestroyGC\()X
2 f
1251(gc)X
1 f
1334(\))X
687 5240(GC)N
2 f
831(gc)X
1 f
914(;)X
2 f
555 5488(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(gc)X
1706(to)X
1797(be)X
1902(deallocated.)X
555 5612(References)N
969(to)X
1062(sharable)X
1378(GCs)X
1557(are)X
1687(counted)X
1989(and)X
2139(a)X
2201(free)X
2360(request)X
2637(is)X
2719(generated)X
3084(to)X
3176(the)X
3307(server)X
3544(when)X
3757(the)X
3888(last)X
555 5708(user)N
723(of)X
818(a)X
879(GC)X
1023(destroys)X
1338(it.)X
3 f
2239 6144(49)N

50 p
%%Page: 50 52
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(25.)N
709(Errors)X
1 f
555 796(The)N
715(X)X
801(Toolkit)X
1085(uses)X
1259(the)X
1389(Xlib)X
1568(error)X
1760(handling)X
2091(interface.)X
2443(This)X
2622(facility)X
2895(allows)X
3147(a)X
3208(client)X
3427(to)X
3518(register)X
3804(a)X
3865(pro-)X
555 892(cedure)N
819(to)X
918(be)X
1031(called)X
1272(whenever)X
1643(an)X
1756(error)X
1956(occurs.)X
2237(This)X
2424(facility)X
2705(is)X
2794(intended)X
3127(for)X
3258(error)X
3457(logging)X
3756(but)X
3898(not)X
555 988(for)N
679(error)X
871(correction)X
1250(or)X
1345(recovery.)X
3 f
2239 6144(50)N

51 p
%%Page: 51 53
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(Appendix)N
944(A:)X
1058(.Xdefaults)X
1463(\256le)X
1598(format)X
1 f
555 796(The)N
727(.Xdefaults)X
1126(\256le)X
1274(is)X
1368(an)X
1486(ASCII)X
1750(text)X
1917(\256le)X
2064(that)X
2231(consists)X
2544(of)X
2651(a)X
2724(number)X
3027(of)X
3134(lines)X
3335(with)X
3526(the)X
3668(following)X
555 892(EBNF)N
802(syntax:)X
555 1036(Xdefault)N
1175(=)X
1247({line)X
1444('\\n'}.)X
555 1132(line)N
1175(=)X
1247(\(comment)X
1627(|)X
1667(include)X
1949(|)X
1989(production\).)X
555 1228(comment)N
1175(=)X
1247("!")X
1370(string.)X
555 1324(include)N
1175(=)X
1247("#INCLUDE")X
1770(\256lename.)X
555 1420(production)N
1175(=)X
1247([hostspec])X
1630(resourcename)X
2140(":")X
2259(string.)X
555 1516(resourcename)N
1175(=)X
1247(["*"])X
1443(name)X
1656({\(".")X
1843(|)X
1883("*"\))X
2050(name}.)X
555 1612(string)N
1175(=)X
1247({<any)X
1488(character)X
1832(not)X
1967(including)X
2323(eol>}.)X
555 1708(\256lename)N
1175(=)X
1247(<operating)X
1652(system)X
1919(speci\256c>.)X
555 1804(hostspec)N
1175(=)X
1247(<operating)X
1652(system)X
1919(speci\256c,)X
2232(same)X
2435(as)X
2530(X)X
2615(host)X
2784(spec>.)X
555 1900(name)N
1175(=)X
1247({"A"-"Z")X
1601(|)X
1641("a"-"z")X
1914(|)X
1954("0"-"9"}.)X
555 2044(The)N
724(\256lename)X
1060(is)X
1151(an)X
1266(operating)X
1631(system)X
1908(speci\256c)X
2209(name)X
2432(of)X
2536(a)X
2606(\256le)X
2750(to)X
2850(include)X
3141(textually)X
3482(in)X
3582(place)X
3799(of)X
3903(the)X
555 2140(include.)N
870(The)X
1040(hostspec)X
1376(is)X
1468(a)X
1540(system)X
1818(dependent)X
2213(host)X
2393(speci\256cation.)X
2894(The)X
3064(intent)X
3298(is)X
3389(to)X
3490(omit)X
3685(any)X
3844(lines)X
555 2236(containing)N
955(a)X
1021(hostspec)X
1351(not)X
1491(matching)X
1847(the)X
1982(host)X
2156(the)X
2291(application)X
2711(is)X
2796(running)X
3096(on.)X
3232(This)X
3415(is)X
3500(determined)X
3923(by)X
555 2332(parsing)N
848(the)X
990(display_name)X
1514(\256eld)X
1704(of)X
1810(the)X
1951(display)X
2239(variable.)X
2578(Because)X
2904(hostspecs)X
3274(are)X
3414(likely)X
3649(to)X
3751(include)X
555 2428(colon)N
787(characters)X
1179(\(``:''\))X
1414(to)X
1519(avoid)X
1751(ambiguity,)X
2168(the)X
2312(hostspec)X
2651(must)X
2859(not)X
3008(be)X
3127(followed)X
3475(by)X
3598(whitespace.)X
555 2524(However,)N
926(the)X
1062(colon)X
1286(that)X
1446(separates)X
1795(the)X
1930(resource)X
2254(name)X
2472(from)X
2670(the)X
2805(resource)X
3129(value)X
3347(string)X
3575(must)X
3774(be)X
3884(fol-)X
555 2620(lowed)N
792(by)X
902(whitespace.)X
3 f
2239 6144(51)N

52 p
%%Page: 52 54
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(Appendix)N
944(B:)X
1054(Translation)X
1517(table)X
1725(\256le)X
1860(syntax)X
1 f
555 796(A)N
640(translation)X
1035(table)X
1229(\256le)X
1364(is)X
1445(an)X
1550(ASCII)X
1801(text)X
1956(\256le.)X
3 f
555 988(Notation)N
1 f
555 1112(Syntax)N
822(is)X
903(speci\256ed)X
1238(in)X
1329(an)X
1434(extended)X
1774(BNF)X
1967(notation)X
2279(with)X
2458(the)X
2588(following)X
2953(abbreviations:)X
775 1236(l)N
822(::=)X
944([)X
995(a)X
1056(])X
1107(.)X
555 1360(is)N
636(equivalent)X
1026(to)X
775 1484(l)N
822(::=)X
944(a)X
1005(|)X
1045(NIL)X
1213(.)X
775 1608(l)N
822(::=)X
944({)X
1008(a)X
1069(})X
1133(.)X
555 1732(is)N
636(equivalent)X
1026(to)X
775 1856(l)N
822(::=)X
944(a)X
1005(l)X
1052(|)X
1092(NIL)X
1260(.)X
555 1980(All)N
708(terminals)X
1077(are)X
1224(enclosed)X
1572(in)X
1680('single')X
1988(quotation)X
2361(marks.)X
2637(Informal)X
2984(descriptions)X
3449(are)X
3595(enclosed)X
3942(in)X
555 2076(<angle>)N
868(brackets.)X
3 f
555 2268(Syntax)N
1 f
555 2392(The)N
714(syntax)X
966(of)X
1061(the)X
1191(translation)X
1586(table)X
1780(\256le)X
1915(is:)X
555 2536(translationTable)N
1261(::=)X
1383({)X
1447(production)X
1851('\\n')X
2000(})X
555 2632(production)N
1261(::=)X
1383(lhs':')X
1591(rhs)X
555 2728(lhs)N
1261(::=)X
1383(\()X
1434(event)X
1647(|)X
1687(keyseq)X
1953(\))X
2004([ifstate])X
2300([setstate])X
555 2824(keyseq)N
1261(::=)X
1383('"')X
1499(keychar)X
1799({keychar})X
2183('"')X
555 2920(keychar)N
1261(::=)X
1383([)X
1434('\303')X
1543(|)X
1583('$')X
1707(])X
1758(<ascii)X
1992(character>)X
555 3016(event)N
1261(::=)X
1383(mode'<'eventtype'>'detail)X
555 3112(ifstate)N
1261(::=)X
1383(',')X
1485('IF')X
1643(state)X
555 3208(setstate)N
1261(::=)X
1383(',')X
1485('SET')X
1722(state)X
555 3304(mode)N
1261(::=)X
1383(<see)X
1567(``Modes''>)X
555 3400(eventtype)N
1261(::=)X
1383(<see)X
1567(``Event)X
1853(Types''>)X
555 3496(detail)N
1261(::=)X
1383(<eventtype)X
1798(speci\256c>)X
555 3592(rhs)N
1261(::=)X
1383({)X
1447(action)X
1685('\(')X
1794(params)X
2070('\)')X
2179(})X
555 3688(action)N
1261(::=)X
1405(atom)X
555 3784(params)N
1261(::=)X
1383({)X
1447(atom)X
1646(|)X
1686("string")X
1981(|)X
2021(`char')X
2252(|)X
2292(number)X
2583(})X
555 3880(atom)N
1261(::=)X
1383(atomchar)X
1733({)X
1797(atomchar)X
2147(})X
555 3976(atomchar)N
1261(::=)X
1383({)X
1447(alpha)X
1660(|)X
1700(digit)X
1885(|)X
1925('$')X
2049(|)X
2089('_')X
2213(})X
555 4072(alpha)N
1261(::=)X
1383(<alphabetic)X
1818(character>)X
555 4168(digit)N
1261(::=)X
1383(<decimal)X
1735(digit)X
1920(0-9>)X
555 4264(number)N
1261(::=)X
1383(digit)X
1568({)X
1632(digit)X
1817(})X
555 4408(Informally,)N
982(the)X
1118(productions)X
1562(are)X
1697(an)X
1808(event)X
2026(speci\256er)X
2351(on)X
2466(the)X
2601(left)X
2746(\(terminated)X
3180(with)X
3364(a)X
3430(colon\))X
3682(and)X
3836(a)X
3902(list)X
555 4504(of)N
650(action)X
888(speci\256cations)X
1390(on)X
1500(the)X
1630(right)X
1819(\(terminated)X
2248(with)X
2427(a)X
2488(newline\).)X
555 4628(The)N
715(information)X
1155(on)X
1266(the)X
1397(left)X
1538(speci\256es)X
1864(the)X
1995(X)X
2081(Event,)X
2332(complete)X
2678(with)X
2857(modi\256ers)X
3212(and)X
3361(detail)X
3580(\256elds,)X
3815(while)X
555 4724(that)N
720(on)X
840(the)X
980(right)X
1179(speci\256es)X
1514(what)X
1717(to)X
1818(do)X
1938(when)X
2160(that)X
2325(event)X
2548(is)X
2639(detected.)X
2987(An)X
3126(action)X
3373(is)X
3463(the)X
3602(name)X
3824(of)X
3928(an)X
555 4820(exported)N
885(function.)X
1223(The)X
1382(atoms,)X
1637(strings,)X
1916(chars,)X
2145(and)X
2294(numbers)X
2619(are)X
2748(parameters)X
3156(to)X
3247(the)X
3377(function.)X
3 f
555 5012(Modes)N
1 f
555 5136(The)N
714(Mode)X
941(\256eld)X
1120(is)X
1201(used)X
1384(to)X
1475(specify)X
1751(normal)X
2023(X)X
2108(keyboard)X
2457(and)X
2606(button)X
2854(modi\256er)X
3175(mask)X
3383(bits:)X
10 f
555 5192(i)N
587(iiiiiiiiiiiiiiiiiii)X
555 5200(i)N
587(iiiiiiiiiiiiiiiiiii)X
1 f
599 5344(Mode)N
941(Meaning)X
10 f
555 5400(i)N
587(iiiiiiiiiiiiiiiiiii)X
555 5448(c)N
5376(c)Y
5288(c)Y
1423 5448(c)N
5376(c)Y
5288(c)Y
1 f
599 5544(c)N
941(Control)X
1233(Key)X
599 5640(Ctrl)N
941(Control)X
1233(Key)X
599 5736(s)N
941(Shift)X
1135(Key)X
599 5832(Shift)N
941(Shift)X
1135(Key)X
10 f
555(c)X
5808(c)Y
5720(c)Y
5632(c)Y
5544(c)Y
1423 5832(c)N
5808(c)Y
5720(c)Y
5632(c)Y
5544(c)Y
3 f
2239 6144(52)N

53 p
%%Page: 53 55
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
10 f
555 584(i)N
587(iiiiiiiiiiiiiiiiiii)X
555 592(i)N
587(iiiiiiiiiiiiiiiiiii)X
1 f
599 736(Mode)N
941(Meaning)X
10 f
555 792(i)N
587(iiiiiiiiiiiiiiiiiii)X
555 840(c)N
768(c)Y
680(c)Y
1423 840(c)N
768(c)Y
680(c)Y
1 f
599 936(m)N
941(Meta)X
1144(Key)X
599 1032(Meta)N
941(Meta)X
1144(Key)X
599 1128(l)N
941(Lock)X
1144(Key)X
599 1224(Lock)N
941(Lock)X
1144(Key)X
599 1320(1)N
941(Modi\256er)X
1271(1)X
599 1416(Mod1)N
941(Modi\256er)X
1271(1)X
599 1512(2)N
941(Modi\256er)X
1271(2)X
599 1608(Mod2)N
941(Modi\256er)X
1271(2)X
599 1704(3)N
941(Modi\256er)X
1271(3)X
599 1800(Mod3)N
941(Modi\256er)X
1271(3)X
599 1896(4)N
941(Modi\256er)X
1271(4)X
599 1992(Mod4)N
941(Modi\256er)X
1271(4)X
599 2088(5)N
941(Modi\256er)X
1271(5)X
599 2184(Mod5)N
941(Modi\256er)X
1271(5)X
10 f
555 2240(i)N
587(iiiiiiiiiiiiiiiiiii)X
555(c)X
2168(c)Y
2080(c)Y
1992(c)Y
1904(c)Y
1816(c)Y
1728(c)Y
1640(c)Y
1552(c)Y
1464(c)Y
1376(c)Y
1288(c)Y
1200(c)Y
1112(c)Y
1024(c)Y
936(c)Y
1423 2240(c)N
2168(c)Y
2080(c)Y
1992(c)Y
1904(c)Y
1816(c)Y
1728(c)Y
1640(c)Y
1552(c)Y
1464(c)Y
1376(c)Y
1288(c)Y
1200(c)Y
1112(c)Y
1024(c)Y
936(c)Y
1 f
555 2412(The)N
716(character)X
1062(\304)X
1115(when)X
1329(used)X
1513(as)X
1609(a)X
1671(pre\256x)X
1899(to)X
1991(the)X
2122(Mode)X
2350(speci\256es)X
2676(that)X
2832(the)X
2963(modi\256er)X
3285(key)X
3435(must)X
3630(be)X
3736(up.)X
3891(If)X
3972(a)X
555 2508(modi\256er)N
881(key)X
1035(is)X
1120(omitted)X
1417(from)X
1614(a)X
1679(mode)X
1901(\256eld)X
2084(speci\256cation,)X
2578(then)X
2756(it)X
2832(is)X
2917(assumed)X
3246(that)X
3405(the)X
3539(user)X
3711(does)X
3898(not)X
555 2604(care)N
723(whether)X
1028(the)X
1158(key)X
1307(is)X
1388(up)X
1498(or)X
1593(down.)X
3 f
555 2796(Event)N
797(Types)X
1 f
555 2920(The)N
718(EventType)X
1131(\256eld)X
1314(describes)X
1667(XEvent)X
1962(types.)X
2196(The)X
2359(following)X
2728(are)X
2861(the)X
2995(currently)X
3339(de\256ned)X
3624(EventType)X
555 3016(values:)N
10 f
555 3072(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 3080(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
599 3224(Type)N
1153(Meaning)X
10 f
555 3280(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 3328(c)N
3256(c)Y
3168(c)Y
2253 3328(c)N
3256(c)Y
3168(c)Y
1 f
599 3424(Key)N
1153(KeyPress)X
599 3520(KeyDown)N
1153(KeyPress)X
599 3616(KeyUp)N
1153(KeyRelease)X
599 3712(BtnDown)N
1153(ButtonPress)X
599 3808(BtnUp)N
1153(ButtonRelease)X
599 3904(Motion)N
1153(MotionNotify)X
599 4000(Enter)N
1153(EnterNotify)X
599 4096(Leave)N
1153(LeaveNotify)X
599 4192(FocusIn)N
1153(FocusIn)X
599 4288(FocusOut)N
1153(FocusOut)X
599 4384(Keymap)N
1153(KeymapNotify)X
599 4480(Expose)N
1153(Expose)X
599 4576(GrExp)N
1153(GraphicsExpose)X
599 4672(NoExp)N
1153(NoExpose)X
599 4768(Visible)N
1153(VisibilityNotify)X
599 4864(Create)N
1153(CreateNotify)X
599 4960(Destroy)N
1153(DestroyNotify)X
599 5056(Unmap)N
1153(UnmapNotify)X
599 5152(Map)N
1153(MapNotify)X
599 5248(MapReq)N
1153(MapRequest)X
599 5344(Reparent)N
1153(ReparentNotify)X
599 5440(Con\256gure)N
1153(Con\256gureNotify)X
599 5536(ConfReq)N
1153(Con\256gureRequest)X
599 5632(Grav)N
1153(GravityNotify)X
599 5728(ResReq)N
1153(ResizeRequest)X
599 5824(Circ)N
1153(CirculateNotify)X
10 f
555(c)X
5800(c)Y
5712(c)Y
5624(c)Y
5536(c)Y
5448(c)Y
5360(c)Y
5272(c)Y
5184(c)Y
5096(c)Y
5008(c)Y
4920(c)Y
4832(c)Y
4744(c)Y
4656(c)Y
4568(c)Y
4480(c)Y
4392(c)Y
4304(c)Y
4216(c)Y
4128(c)Y
4040(c)Y
3952(c)Y
3864(c)Y
3776(c)Y
3688(c)Y
3600(c)Y
3512(c)Y
3424(c)Y
2253 5824(c)N
5800(c)Y
5712(c)Y
5624(c)Y
5536(c)Y
5448(c)Y
5360(c)Y
5272(c)Y
5184(c)Y
5096(c)Y
5008(c)Y
4920(c)Y
4832(c)Y
4744(c)Y
4656(c)Y
4568(c)Y
4480(c)Y
4392(c)Y
4304(c)Y
4216(c)Y
4128(c)Y
4040(c)Y
3952(c)Y
3864(c)Y
3776(c)Y
3688(c)Y
3600(c)Y
3512(c)Y
3424(c)Y
3 f
2239 6144(53)N

54 p
%%Page: 54 56
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
10 f
555 584(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 592(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
599 736(Type)N
1153(Meaning)X
10 f
555 792(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 840(c)N
768(c)Y
680(c)Y
2253 840(c)N
768(c)Y
680(c)Y
1 f
599 936(CircReq)N
1153(CirculateRequest)X
599 1032(Prop)N
1153(PropertyNotify)X
599 1128(SelClr)N
1153(SelectionClear)X
599 1224(SelReq)N
1153(SelectionRequest)X
599 1320(Select)N
1153(SelectionNotify)X
599 1416(Clrmap)N
1153(ColormapNotify)X
599 1512(Message)N
1153(ClientMessage)X
599 1608(Mapping)N
1153(MappingNotify)X
599 1704(Timer)N
1153(Toolkit)X
1436(Timer)X
1674(Event)X
599 1800(EventTimer)N
1153(Toolkit)X
1436(``Event)X
1722(Timer'')X
2018(event)X
10 f
555 1856(i)N
581(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555(c)X
1816(c)Y
1728(c)Y
1640(c)Y
1552(c)Y
1464(c)Y
1376(c)Y
1288(c)Y
1200(c)Y
1112(c)Y
1024(c)Y
936(c)Y
2253 1856(c)N
1816(c)Y
1728(c)Y
1640(c)Y
1552(c)Y
1464(c)Y
1376(c)Y
1288(c)Y
1200(c)Y
1112(c)Y
1024(c)Y
936(c)Y
1 f
555 2000(Supported)N
939(Abbreviations:)X
10 f
555 2056(i)N
561(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 2064(i)N
561(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
599 2208(Abbreviation)N
1196(Meaning)X
10 f
555 2264(i)N
561(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555 2312(c)N
2240(c)Y
2152(c)Y
2453 2312(c)N
2240(c)Y
2152(c)Y
1 f
599 2408(Ctrl)N
1196(KeyPressed)X
1632(with)X
1811(control)X
2083(modi\256er.)X
599 2504(Meta)N
1196(KeyPressed)X
1632(with)X
1811(meta)X
2005(modi\256er.)X
599 2600(Shift)N
1196(KeyPressed)X
1632(with)X
1811(shift)X
1990(modi\256er.)X
599 2696(Btn1Down)N
1196(ButtonPress)X
1644(with)X
1823(Btn1)X
2017(detail)X
599 2792(Btn1Up)N
1196(ButtonRelease)X
1733(with)X
1912(Btn1)X
2106(detail)X
599 2888(Btn2Down)N
1196(ButtonPress)X
1644(with)X
1823(Btn2)X
2017(detail)X
599 2984(Btn2Up)N
1196(ButtonRelease)X
1733(with)X
1912(Btn2)X
2106(detail)X
599 3080(Btn3Down)N
1196(ButtonPress)X
1644(with)X
1823(Btn3)X
2017(detail)X
599 3176(Btn3Up)N
1196(ButtonRelease)X
1733(with)X
1912(Btn3)X
2106(detail)X
599 3272(Btn4Down)N
1196(ButtonPress)X
1644(with)X
1823(Btn4)X
2017(detail)X
599 3368(Btn4Up)N
1196(ButtonRelease)X
1733(with)X
1912(Btn4)X
2106(detail)X
599 3464(Btn5Down)N
1196(ButtonPress)X
1644(with)X
1823(Btn5)X
2017(detail)X
599 3560(Btn5Up)N
1196(ButtonRelease)X
1733(with)X
1912(Btn5)X
2106(detail)X
10 f
555 3616(i)N
561(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
555(c)X
3552(c)Y
3464(c)Y
3376(c)Y
3288(c)Y
3200(c)Y
3112(c)Y
3024(c)Y
2936(c)Y
2848(c)Y
2760(c)Y
2672(c)Y
2584(c)Y
2496(c)Y
2408(c)Y
2453 3616(c)N
3552(c)Y
3464(c)Y
3376(c)Y
3288(c)Y
3200(c)Y
3112(c)Y
3024(c)Y
2936(c)Y
2848(c)Y
2760(c)Y
2672(c)Y
2584(c)Y
2496(c)Y
2408(c)Y
1 f
555 3788(The)N
721(Detail)X
965(\256eld)X
1150(is)X
1237(event)X
1456(speci\256c)X
1753(and)X
1908(normally)X
2255(corresponds)X
2707(to)X
2804(the)X
2940(detail)X
3165(\256eld)X
3350(of)X
3451(an)X
3562(X)X
3653(Event,)X
3909(for)X
555 3884(example,)N
904(<Key>A.)X
1285(In)X
1385(the)X
1520(event)X
1738(that)X
1898(no)X
2013(event)X
2231(bindings)X
2562(are)X
2696(speci\256ed)X
3036(externally,)X
3438(the)X
3573(widget)X
3840(class)X
555 3980(speci\256es)N
880(the)X
1010(default)X
1277(bindings)X
1603(in)X
1694(textual)X
1957(form.)X
3 f
2239 6144(54)N

55 p
%%Page: 55 57
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
555 672(Appendix)N
944(C:)X
1058(Intrinsic.h)X
2239 6144(55)N

56 p
%%Page: 56 58
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 720(/*)N
843 816(*)N
909($Header:)X
1253(Intrinsic.h,v)X
1701(1.32)X
1877(87/09/13)X
2213(20:36:19)X
2549(newman)X
2869(Exp)X
3033($)X
843 912(*/)N
843 1104(/*)N
865 1200(*)N
931(Copyright)X
1311(1987)X
1509(by)X
1619(Digital)X
1887(Equipment)X
2297(Corporation,)X
2767(Maynard,)X
3128(Massachusetts.)X
865 1296(*)N
865 1392(*)N
1459(All)X
1594(Rights)X
1847(Reserved)X
865 1488(*)N
865 1584(*)N
931(Permission)X
1345(to)X
1436(use,)X
1597(copy,)X
1812(modify,)X
2111(and)X
2260(distribute)X
2616(this)X
2766(software)X
3090(and)X
3239(its)X
865 1680(*)N
931(documentation)X
1478(for)X
1602(any)X
1751(purpose)X
2051(and)X
2200(without)X
2492(fee)X
2621(is)X
2702(hereby)X
2963(granted,)X
865 1776(*)N
931(provided)X
1266(that)X
1421(the)X
1551(above)X
1783(copyright)X
2143(notice)X
2381(appear)X
2637(in)X
2728(all)X
2839(copies)X
3086(and)X
3235(that)X
865 1872(*)N
931(both)X
1110(that)X
1265(copyright)X
1625(notice)X
1863(and)X
2012(this)X
2162(permission)X
2571(notice)X
2809(appear)X
3065(in)X
865 1968(*)N
931(supporting)X
1330(documentation,)X
1899(and)X
2048(that)X
2203(the)X
2333(name)X
2546(of)X
2641(Digital)X
2909(Equipment)X
865 2064(*)N
931(Corporation)X
1379(not)X
1514(be)X
1619(used)X
1802(in)X
1893(advertising)X
2307(or)X
2402(publicity)X
2739(pertaining)X
3119(to)X
865 2160(*)N
931(distribution)X
1361(of)X
1456(the)X
1586(software)X
1910(without)X
2202(speci\256c,)X
2515(written)X
2787(prior)X
2980(permission.)X
865 2256(*)N
865 2352(*)N
865 2448(*)N
931(DIGITAL)X
1308(DISCLAIMS)X
1803(ALL)X
1996(WARRANTIES)X
2594(WITH)X
2845(REGARD)X
3228(TO)X
3367(THIS)X
3584(SOFTWARE,)X
4102(INCLUDING)X
865 2544(*)N
931(ALL)X
1124(IMPLIED)X
1502(WARRANTIES)X
2100(OF)X
2234(MERCHANTABILITY)X
3100(AND)X
3311(FITNESS,)X
3702(IN)X
3816(NO)X
3964(EVENT)X
4274(SHALL)X
865 2640(*)N
931(DIGITAL)X
1308(BE)X
1443(LIABLE)X
1778(FOR)X
1971(ANY)X
2182(SPECIAL,)X
2583(INDIRECT)X
3015(OR)X
3159(CONSEQUENTIAL)X
3912(DAMAGES)X
4367(OR)X
865 2736(*)N
931(ANY)X
1142(DAMAGES)X
1597(WHATSOEVER)X
2224(RESULTING)X
2734(FROM)X
3005(LOSS)X
3242(OF)X
3376(USE,)X
3586(DATA)X
3851(OR)X
3995(PROFITS,)X
865 2832(*)N
931(WHETHER)X
1383(IN)X
1497(AN)X
1645(ACTION)X
1998(OF)X
2132(CONTRACT,)X
2650(NEGLIGENCE)X
3228(OR)X
3372(OTHER)X
3687(TORTIOUS)X
4143(ACTION,)X
865 2928(*)N
931(ARISING)X
1308(OUT)X
1510(OF)X
1644(OR)X
1788(IN)X
1902(CONNECTION)X
2494(WITH)X
2745(THE)X
2938(USE)X
3126(OR)X
3270(PERFORMANCE)X
3942(OF)X
4076(THIS)X
865 3024(*)N
931(SOFTWARE.)X
865 3120(*/)N
843 3216(#ifndef)N
1119(_Xtintrinsic_h)X
843 3312(#de\256ne)N
1124(_Xtintrinsic_h)X
843 3600(/****************************************************************)N
865 3696(****************************************************************)N
865 3792(***)N
2273(***)X
865 3888(***)N
2273(***)X
865 3984(***)N
1415(X)X
1500(Toolkit)X
1783(Intrinsics)X
2529(***)X
865 4080(***)N
2273(***)X
865 4176(***)N
2273(***)X
865 4272(****************************************************************)N
865 4368(****************************************************************/)N
843 4464(/****************************************************************)N
865 4560(*)N
865 4656(*)N
931(Miscellaneous)X
1462(de\256nitions)X
865 4752(*)N
865 4848(****************************************************************/)N
843 5136(#include)N
1283(<X11/Xlib.h>)X
843 5232(#include)N
1283(<X11/Xresource.h>)X
843 5328(#include)N
1283(<sys/types.h>)X
843 5520(#ifndef)N
1119(NULL)X
843 5616(#de\256ne)N
1124(NULL)X
1380(0)X
843 5712(#endif)N
3 f
2239 6144(56)N

57 p
%%Page: 57 59
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(#ifndef)N
1119(FALSE)X
843 768(#de\256ne)N
1124(FALSE)X
1415(0)X
843 864(#de\256ne)N
1124(TRUE)X
1398(1)X
843 960(#endif)N
843 1152(#de\256ne)N
1124(XtNumber\(arr\))X
1943(\(\(Cardinal\))X
2356(\(sizeof\(arr\))X
2772(/)X
2819(sizeof\(arr[0]\)\)\))X
843 1248(#de\256ne)N
1124(XtOffset\(type,\256eld\))X
1908(\(\(unsigned)X
2306(int\)&\(\(\(type\)NULL\)->\256eld\)\))X
843 1344(typedef)N
1129(char)X
1302(*String;)X
843 1440(typedef)N
1129(struct)X
1347(_WidgetRec)X
1810(*Widget;)X
843 1536(typedef)N
1129(struct)X
1347(_WidgetClassRec)X
2001(*WidgetClass;)X
843 1632(typedef)N
1129(struct)X
1347(_CompositeRec)X
1933 0.2031(*CompositeWidget;)AX
843 1728(typedef)N
1129(struct)X
1347(_XtEventRec)X
1844(*_XtEventTable;)X
843 1824(typedef)N
1129(struct)X
1347(_XtActionsRec)X
1912(*XtActionList;)X
843 1920(typedef)N
1129(struct)X
1347(_XtResource)X
1828(*XtResourceList;)X
843 2016(typedef)N
1129(struct)X
1347(_GrabRec)X
1747(*GrabList;)X
843 2112(typedef)N
1129(unsigned)X
1469(int)X
1629(Cardinal;)X
843 2208(typedef)N
1129 0.3523(charBoolean;)AX
843 2304(typedef)N
1129(unsigned)X
1469(long)X
1723(*Opaque;)X
843 2400(typedef)N
1129(struct)X
1347(_TranslationData)X
2163(*_XtTranslations;)X
843 2496(typedef)N
1129(struct)X
1347(_XtCallbackRec*)X
2062(XtCallbackList;)X
843 2592(typedef)N
1129(unsigned)X
1469(long)X
1692(XtCallbackKind;)X
843 2688(typedef)N
1129(unsigned)X
1469(long)X
1692(XtValueMask;)X
843 2784(typedef)N
1129(unsigned)X
1469(long)X
1692(XtIntervalId;)X
843 2880(typedef)N
1129(unsigned)X
1469(int)X
1651(XtGeometryMask;)X
843 2976(typedef)N
1129(unsigned)X
1469(long)X
1692(XtGCMask;)X
2188(/*)X
2279(Mask)X
2496(of)X
2591(values)X
2838(that)X
2993(are)X
3122(used)X
3305(by)X
3415(widget*/)X
843 3072(typedef)N
1129(unsigned)X
1469(long)X
1692(Pixel;)X
2031(/*)X
2122(Index)X
2344(into)X
2504(colormap)X
3219(*/)X
843 3168(typedef)N
1129(int)X
1503(Position;)X
1884(/*)X
1975(Offset)X
2216(from)X
2409(0)X
2475(coordinate)X
3219(*/)X
843 3264(typedef)N
1129(unsigned)X
1469(int)X
1723(Dimension;)X
2179(/*)X
2270(Size)X
2444(in)X
2535(pixels)X
3219(*/)X
1811 3360(/*)N
1902(should)X
2159(be)X
2264(unsigned,)X
2626(but)X
2761(pcc)X
3015(*/)X
1811 3456(/*)N
1902(generates)X
2256(bad)X
2405(code)X
2593(for)X
2717(unsigned?)X
3096(*/)X
843 3648(typedef)N
1129(void)X
1308(\(*XtProc\)\(\);)X
931 3744(/*)N
1022(takes)X
1225(no)X
1335(arguments)X
1724(*/)X
843 3936(typedef)N
1129(void)X
1308(\(*XtWidgetProc\)\(\);)X
931 4032(/*)N
1022(Widget)X
1304(widget)X
1566(*/)X
843 4224(typedef)N
1129(void)X
1308(\(*XtArgsProc\)\(\);)X
931 4320(/*)N
1022(Widget)X
1304(widget)X
1566(*/)X
931 4416(/*)N
1022(ArgList)X
1318(args)X
1486(*/)X
931 4512(/*)N
1022(Cardinal)X
1348(num_args)X
1717(*/)X
843 4704(typedef)N
1129(void)X
1308(\(*XtInitProc\)\(\);)X
931 4800(/*)N
1022(Widget)X
1304(requestWidget;)X
1865(*/)X
931 4896(/*)N
1022(Widget)X
1304(newWidget;)X
1757(*/)X
931 4992(/*)N
1022(ArgList)X
1318(args)X
1486(*/)X
931 5088(/*)N
1022(Cardinal)X
1348(num_args)X
1717(*/)X
843 5280(typedef)N
1129(Boolean)X
1445(\(*XtSetValuesProc\)\(\);)X
2280(/*)X
2371(returns)X
2637(TRUE)X
2889(if)X
2965(redisplay)X
3310(needed)X
3581(*/)X
931 5376(/*)N
1022(Widget)X
1304(widget;)X
1679(*/)X
931 5472(/*)N
1022(Widget)X
1304(request;)X
1767(*/)X
931 5568(/*)N
1022(Widget)X
1304(new;)X
1651(*/)X
931 5664(/*)N
1022(Boolean)X
1338(last;)X
1618(*/)X
3 f
2239 6144(57)N

58 p
%%Page: 58 60
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(typedef)N
1129(void)X
1308(\(*XtExposeProc\)\(\);)X
931 768(/*)N
1022(Widget)X
1370(widget;)X
1657(*/)X
931 864(/*)N
1022(XEvent)X
1379(*event;)X
1661(*/)X
843 1056(typedef)N
1129(void)X
1308(\(*XtRealizeProc\))X
1946(\(\);)X
931 1152(/*)N
1022 0.2552(Widgetwidget;)AX
2251(*/)X
931 1248(/*)N
1022(XtValueMask)X
1537(mask;)X
2471(*/)X
931 1344(/*)N
1022(XSetWindowAttributes)X
1876(*attributes;)X
2362(*/)X
843 1536(typedef)N
1129(enum)X
1369({)X
931 1632(XtGeometryYes,)N
1705(/*)X
1796(Request)X
2102(accepted.)X
2454(*/)X
931 1728(XtGeometryNo,)N
1698(/*)X
1789(Request)X
2095(denied.)X
2374(*/)X
931 1824(XtGeometryAlmost,)N
1763(/*)X
1854(Request)X
2160(denied,)X
2439(but)X
2574(willing)X
2847(to)X
2938(take)X
3107(replyBox.)X
3479(*/)X
843 1920(})N
907(XtGeometryResult;)X
843 2112(typedef)N
1129(XtGeometryResult)X
1817(\(*XtGeometryHandler\)\(\);)X
931 2208(/*)N
1022(Widget)X
1591(widget)X
1853(*/)X
931 2304(/*)N
1022(XtWidgetGeometry)X
1876(*request)X
2196(*/)X
931 2400(/*)N
1022(XtWidgetGeometry)X
1811(*reply)X
2102(*/)X
843 2592(/****************************************************************)N
865 2688(*)N
865 2784(*)N
931(System)X
1213(Dependent)X
1616(De\256nitions)X
865 2880(*)N
865 2976(*)N
865 3072(*)N
931(XtArgVal)X
1304(ought)X
1527(to)X
1618(be)X
1723(a)X
1784(union)X
2007(of)X
2102(caddr_t,)X
2410(char)X
2583(*,)X
2671(long,)X
2872(int)X
2988(*,)X
3076(and)X
3225(proc)X
3403(*)X
865 3168(*)N
931(but)X
1066(casting)X
1338(to)X
1429(union)X
1652(types)X
1860(is)X
1941(not)X
2076(really)X
2299(supported.)X
865 3264(*)N
865 3360(*)N
931(So)X
1046(the)X
1176(typedef)X
1462(for)X
1586(XtArgVal)X
1959(should)X
2216(be)X
2321(chosen)X
2587(such)X
2770(that)X
865 3456(*)N
865 3552(*)N
1041(sizeof)X
1273(\(XtArgVal\))X
1704(>=)X
1943(sizeof\(caddr_t\))X
865 3648(*)N
1723(sizeof\(char)X
2135(*\))X
865 3744(*)N
1723(sizeof\(long\))X
865 3840(*)N
1723(sizeof\(int)X
2078(*\))X
865 3936(*)N
1723(sizeof\(proc)X
2140(*\))X
865 4032(*)N
865 4128(*)N
931(ArgLists)X
1261(rely)X
1420(heavily)X
1702(on)X
1812(the)X
1942(above)X
2174(typedef.)X
865 4224(*)N
865 4320(****************************************************************/)N
843 4416(typedef)N
1129(long)X
1308(XtArgVal;)X
843 4608(/***************************************************************)N
865 4704(*)N
931(Widget)X
1213(Core)X
1406(Data)X
1594(Structures)X
865 4800(*)N
865 4896(*)N
865 4992(**************************************************************/)N
843 5184(typedef)N
1129(struct)X
1347(_CorePart)X
1726({)X
931 5280(WidgetClass)N
1591(widget_class;)X
2163(/*)X
2254(pointer)X
2526(to)X
2617(Widget's)X
2962(ClassRec)X
3593(*/)X
931 5376(Widget)N
1371(parent;)X
1943(/*)X
2034(parent)X
2276(widget)X
2933(*/)X
931 5472(String)N
1367(name;)X
1943(/*)X
2034(widget)X
2296(resource)X
2615(name)X
3153(*/)X
931 5568(XrmName)N
1500(xrm_name;)X
2163(/*)X
2254(widget)X
2516(resource)X
2835(name)X
3048(quarki\256ed)X
3471(*/)X
931 5664(Screen)N
1371(*screen;)X
1943(/*)X
2034(window's)X
2402(screen)X
3153(*/)X
931 5760(Window)N
1371(window;)X
1943(/*)X
2034(window)X
2339(ID)X
3153(*/)X
3 f
2239 6144(58)N

59 p
%%Page: 59 61
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
931 672(Position)N
1397(x,)X
1485(y;)X
1943(/*)X
2034(window)X
2339(position)X
3153(*/)X
931 768(Dimension)N
1472(width,)X
1717(height;)X
2163(/*)X
2254(window)X
2559(dimensions)X
3373(*/)X
931 864(Cardinal)N
1411(depth;)X
1943(/*)X
2034(number)X
2325(of)X
2420(planes)X
2667(in)X
2758(window)X
3217(*/)X
931 960(Dimension)N
1472(border_width;)X
2163(/*)X
2254(window)X
2559(border)X
2810(width)X
3373(*/)X
931 1056(Pixel)N
1371(border_pixel;)X
1943(/*)X
2034(window)X
2339(border)X
2590(pixel)X
3153(*/)X
931 1152(Pixmap)N
1421(border_pixmap;)X
2163(/*)X
2254(window)X
2559(border)X
2810(pixmap)X
3097(or)X
3192(NULL)X
3558(*/)X
931 1248(Pixel)N
1371(background_pixel;)X
2163(/*)X
2254(window)X
2559(background)X
2996(pixel)X
3373(*/)X
931 1344(Pixmap)N
1421(background_pixmap;)X
2383(/*)X
2474(window)X
2779(background)X
3216(pixmap)X
3503(or)X
3598(NULL)X
3876(*/)X
931 1440(_XtEventTable)N
1536(event_table;)X
2163(/*)X
2254(private)X
2521(to)X
2612(event)X
2825(dispatcher)X
3341(*/)X
931 1536(_XtTranslations)N
1521(translations;)X
2163(/*)X
2254(private)X
2521(to)X
2612(Translation)X
3036(Manager)X
3436(*/)X
931 1632(Boolean)N
1423(visible;)X
1943(/*)X
2034(is)X
2115(window)X
2420(mapped)X
2721(and)X
2870(not)X
3005(occluded?*/)X
931 1728(Boolean)N
1371(sensitive;)X
1943(/*)X
2034(is)X
2115(widget)X
2377(sensitive)X
2708(to)X
2799(user)X
2967(events*/)X
931 1824(Boolean)N
1423(ancestor_sensitive;)X
2163(/*)X
2254(are)X
2383(all)X
2494(ancestors)X
2843(sensitive?)X
3323(*/)X
931 1920(Boolean)N
1423(managed;)X
2163(/*)X
2254(is)X
2335(widget)X
2597(geometry)X
2952(managed?)X
3593(*/)X
931 2016(Boolean)N
1371(mapped_when_managed;/*)X
2362(map)X
2536(window)X
2841(if)X
2917(it's)X
3052(managed?)X
3563(*/)X
931 2112(Boolean)N
1423(being_destroyed;)X
2163(/*)X
2254(marked)X
2540(for)X
2664(destroy)X
3373(*/)X
931 2208(XtCallbackList)N
1515(destroy_callbacks;)X
2383(/*)X
2474(who)X
2647(to)X
2738(call)X
2888(when)X
3100(widget)X
3362(destroyed)X
3726(*/)X
843 2304(})N
907(CorePart;)X
843 2496(typedef)N
1129(struct)X
1347(_WidgetRec)X
1810({)X
931 2592(CorePart)N
1332(core;)X
865 2688(})N
929(WidgetRec;)X
843 2880(typedef)N
1129(Widget)X
1411(*WidgetList;)X
843 3072(/******************************************************************)N
865 3168(*)N
865 3264(*)N
931(Core)X
1124(Class)X
1337(Structure.)X
1704(Widgets,)X
2042(regardless)X
2420(of)X
2515(their)X
2699(class,)X
2914(will)X
3074(have)X
865 3360(*)N
931(these)X
1134(\256elds.)X
1391(All)X
1526(widgets)X
1822(of)X
1917(a)X
1978(given)X
2196(class)X
2389(will)X
2549(have)X
2737(the)X
2867(same)X
3070(values)X
865 3456(*)N
931(for)X
1055(these)X
1258(\256elds.)X
1515(Widgets)X
1831(of)X
1926(a)X
1987(given)X
2205(class)X
2398(may)X
2572(also)X
2736(have)X
2924(additional)X
865 3552(*)N
931(common)X
1262(\256elds.)X
1519(These)X
1751(additional)X
2127(\256elds)X
2340(are)X
2469(included)X
2795(in)X
2886(incremental)X
865 3648(*)N
931(class)X
1124(structures,)X
1510(such)X
1693(as)X
1788(CommandClass.)X
865 3744(*)N
865 3840(*)N
931(The)X
1090(\256elds)X
1303(that)X
1458(are)X
1587(speci\256c)X
1878(to)X
1969(this)X
2119(subclass,)X
2456(as)X
2551(opposed)X
2866(to)X
2957(\256elds)X
3170(that)X
865 3936(*)N
931(are)X
1060(part)X
1219(of)X
1314(the)X
1444(superclass,)X
1849(are)X
1978(called)X
2211("subclass)X
2562(\256elds")X
2811(below.)X
3092(Many)X
865 4032(*)N
931(procedures)X
1338(are)X
1467(responsible)X
1890(only)X
2069(for)X
2193(the)X
2323(subclass)X
2638(\256elds,)X
2873(and)X
3022(not)X
3157(for)X
865 4128(*)N
931(any)X
1080(superclass)X
1463(\256elds.)X
865 4224(*)N
865 4320(********************************************************************/)N
843 4512(typedef)N
1129(struct)X
1347(_CoreClassPart)X
1917({)X
931 4608(WidgetClass)N
1591(superclass;)X
2317(/*)X
2408(pointer)X
2680(to)X
2771(superclass)X
3154(ClassRec)X
3592(*/)X
931 4704(String)N
1367(class_name;)X
2097(/*)X
2188(widget)X
2450(resource)X
2769(class)X
2962(name)X
3351(*/)X
931 4800(Cardinal)N
1411(widget_size;)X
2011(/*)X
2102(size)X
2261(in)X
2352(bytes)X
2560(of)X
2655(widget)X
2917(record)X
3251(*/)X
931 4896(XtProc)N
1371 0.2305(class_initialize;)AX
1966(/*)X
2057(class)X
2250 0.3317(initialization)AX
2721(proc)X
3153(*/)X
931 4992(Boolean)N
1423(class_inited;)X
1997(/*)X
2088(has)X
2227(class)X
2420(been)X
2608 0.2841(initialized?)AX
3178(*/)X
931 5088(XtInitProc)N
1435 0.3750(initialize;)AX
2097(/*)X
2188 0.3542(initialize)AX
2521(subclass)X
2836(\256elds)X
3225(*/)X
931 5184(XtRealizeProc)N
1511(realize;)X
2097(/*)X
2188(XCreateWindow)X
2806(for)X
2930(widget)X
3373(*/)X
931 5280(XtActionList)N
1485(actions;)X
2097(/*)X
2188(widget)X
2450(semantics)X
2820(name)X
3033(to)X
3124(proc)X
3302(map)X
3498(*/)X
931 5376(Cardinal)N
1371(num_actions;)X
2001(/*)X
2092(number)X
2383(of)X
2478(entries)X
2735(in)X
2826(actions)X
3230(*/)X
931 5472(XtResourceList)N
1528(resources;)X
2097(/*)X
2188(resources)X
2541(for)X
2665(subclass)X
2980(\256elds)X
3303(*/)X
931 5568(Cardinal)N
1411(num_resources;)X
2078(/*)X
2169(number)X
2460(of)X
2555(entries)X
2812(in)X
2903(resources)X
3344(*/)X
931 5664(XrmClass)N
1459(xrm_class;)X
2097(/*)X
2188(resource)X
2507(class)X
2700(quarki\256ed)X
3373(*/)X
931 5760(Boolean)N
1423(compress_motion;)X
2141(/*)X
2232(compress)X
2586(MotionNotify)X
3098(for)X
3222(widget)X
3528(*/)X
3 f
2239 6144(59)N

60 p
%%Page: 60 62
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
931 672(Boolean)N
1423(compress_exposure;)X
2163(/*)X
2254(compress)X
2608(Expose)X
2889(events)X
3136(for)X
3260(widget)X
3544(*/)X
931 768(Boolean)N
1423(visible_interest;)X
2032(/*)X
2123(select)X
2346(for)X
2470(VisibilityNotify)X
3211(*/)X
931 864(XtWidgetProc)N
1528(destroy;)X
2097(/*)X
2188(free)X
2346(data)X
2515(for)X
2639(subclass)X
2954(pointers)X
3326(*/)X
931 960(XtWidgetProc)N
1528(resize;)X
2097(/*)X
2188(geom)X
2406(manager)X
2731(changed)X
3046(widget)X
3308(size)X
3511(*/)X
931 1056(XtExposeProc)N
1527(expose;)X
2097(/*)X
2188(rediplay)X
2499(window)X
3153(*/)X
931 1152(XtSetValuesProc)N
1559(set_values;)X
2317(/*)X
2408(set)X
2528(subclass)X
2843(resource)X
3162(values)X
3541(*/)X
931 1248(XtWidgetProc)N
1528(accept_focus;)X
2144(/*)X
2235(assign)X
2477(input)X
2681(focus)X
2893(to)X
2984(widget)X
3378(*/)X
887 1344(})N
951(CoreClassPart;)X
843 1536(typedef)N
1129(struct)X
1347(_WidgetClassRec)X
2001({)X
931 1632(CoreClassPart)N
1457(core_class;)X
843 1728(})N
907(WidgetClassRec;)X
843 1920(extern)N
1085(WidgetClassRec)X
1695(widgetClassRec;)X
843 2016(extern)N
1085(WidgetClass)X
1558(widgetClass;)X
843 2208(/************************************************************************)N
865 2304(*)N
865 2400(*)N
931(Additional)X
1331(instance)X
1642(\256elds)X
1855(for)X
1979(widgets)X
2275(of)X
2370(\(sub\)class)X
2743('Composite')X
865 2496(*)N
865 2592(************************************************************************/)N
843 2784(typedef)N
1129(Cardinal)X
1455(\(*XtOrderProc\)\(\);)X
931 2880(/*)N
1022(Widget)X
1304(child;)X
1528(*/)X
843 3168(typedef)N
1129(struct)X
1347(_CompositePart)X
1938({)X
931 3264(WidgetList)N
1373(children;)X
1833(/*)X
1924(array)X
2126(of)X
2221(ALL)X
2414(widget)X
2676(children)X
3153(*/)X
931 3360(Cardinal)N
1323(num_children;)X
2053(/*)X
2144(total)X
2324(number)X
2615(of)X
2710(widget)X
2972(children)X
3373(*/)X
931 3456(Cardinal)N
1323(num_slots;)X
1953(/*)X
2044(number)X
2335(of)X
2430(slots)X
2614(in)X
2705(children)X
3016(array)X
3284(*/)X
931 3552(Cardinal)N
1323(num_mapped_children;)X
2183(/*)X
2274(count)X
2492(of)X
2587(managed)X
2927(and)X
3076(mapped)X
3377(children)X
3688(*/)X
931 3648(XtOrderProc)N
1406(insert_position;)X
2066(/*)X
2157(compute)X
2483(position)X
2790(of)X
2885(new)X
3053(child)X
3373(*/)X
843 3744(})N
907(CompositePart;)X
843 3936(typedef)N
1129(struct)X
1347(_CompositeRec)X
1933({)X
931 4032(CorePart)N
1376(core;)X
931 4128(CompositePart)N
1478(composite;)X
843 4224(})N
907(CompositeRec;)X
843 4416(typedef)N
1129(struct)X
1347(_ConstraintPart)X
1923({)X
931 4512(caddr_t)N
1305(mumble;)X
1943(/*)X
2034(No)X
2163(new)X
2331(\256elds,)X
2566(keep)X
2754(C)X
2835(compiler)X
3171(happy)X
3408(*/)X
843 4608(})N
907(ConstraintPart;)X
843 4800(typedef)N
1129(struct)X
1347(_ConstraintRec)X
1918({)X
931 4896(CorePart)N
1371(core;)X
931 4992(CompositePart)N
1522(composite;)X
931 5088(ConstraintPart)N
1485(constraint;)X
843 5184(})N
907(ConstraintRec,)X
1456(*ConstraintWidget;)X
843 5376(/*********************************************************************)N
865 5472(*)N
865 5568(*)N
953(Additional)X
1353(class)X
1546(\256elds)X
1759(for)X
1883(widgets)X
2179(of)X
2274(\(sub\)class)X
2647('Composite')X
865 5664(*)N
865 5760(********************************************************************/)N
3 f
2239 6144(60)N

61 p
%%Page: 61 63
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(typedef)N
1129(struct)X
1347(_CompositeClassPart)X
2129({)X
931 768(XtGeometryHandler)N
1676(geometry_manager;)X
2427(/*)X
2518(geometry)X
2873(manager)X
3198(for)X
3322(children)X
3677(*/)X
931 864(XtWidgetProc)N
1572(change_managed;)X
2427(/*)X
2518(change)X
2789(managed)X
3129(state)X
3313(of)X
3408(child)X
3651(*/)X
931 960(XtArgsProc)N
1635(insert_child;)X
2207(/*)X
2298(physically)X
2683(add)X
2832(child)X
3031(to)X
3122(parent)X
3386(*/)X
931 1056(XtWidgetProc)N
1572(delete_child;)X
2207(/*)X
2298(physically)X
2683(remove)X
2969(child)X
3373(*/)X
931 1152(XtWidgetProc)N
1572(move_focus_to_next;)X
2358(/*)X
2449(move)X
2667(Focus)X
2899(to)X
2990(next)X
3164(child)X
3593(*/)X
931 1248(XtWidgetProc)N
1572(move_focus_to_prev;)X
2362(/*)X
2453(move)X
2671(Focus)X
2903(to)X
2994(previous)X
3319(child)X
3584(*/)X
843 1344(})N
907(CompositeClassPart;)X
843 1536(typedef)N
1129(struct)X
1347(_CompositeClassRec)X
2124({)X
953 1632(CoreClassPart)N
1589(core_class;)X
953 1728(CompositeClassPart)N
1691(composite_class;)X
843 1824(})N
907(CompositeClassRec,)X
1662 0.1696(*CompositeWidgetClass;)AX
843 2016(extern)N
1085(CompositeClassRec)X
1818(compositeClassRec;)X
843 2112(extern)N
1085(CompositeWidgetClass)X
1941 0.1594(compositeWidgetClass;)AX
843 2400(typedef)N
1129(struct)X
1347(_ConstraintClassPart)X
2114({)X
931 2496(XtResourceList)N
1506(constraints;)X
2075(/*)X
2166(constraint)X
2536(resource)X
2855(list)X
3153(*/)X
931 2592(Cardinal)N
1301(num_constraints;)X
2063(/*)X
2154(number)X
2445(of)X
2540(constraints)X
2944(in)X
3035(list)X
3298(*/)X
843 2688(})N
907(ConstraintClassPart;)X
843 2880(typedef)N
1129(struct)X
1347(_ConstraintClassRec)X
2109({)X
931 2976(CoreClassPart)N
1589(core_class;)X
931 3072(CompositeClassPart)N
1691(composite_class;)X
931 3168(ConstraintClassPart)N
1654(constraint_class;)X
843 3264(})N
907(ConstraintClassRec,)X
1647 0.1477(*ConstraintWidgetClass;)AX
843 3456(extern)N
1085(ConstraintClassRec)X
1803(constraintClassRec;)X
843 3552(extern)N
1085(ConstraintWidgetClass)X
1926(constraintWidgetClass;)X
843 3744(/*************************************************************************)N
865 3840(*)N
865 3936(*)N
931(Generic)X
1231(Procedures)X
865 4032(*)N
865 4128(*************************************************************************/)N
843 4416(extern)N
1085(Boolean)X
1401(XtIsSubclass)X
1882(\(\);)X
931 4512(/*)N
1022(Widget)X
1436(widget;)X
1811(*/)X
931 4608(/*)N
1022(WidgetClass)X
1517(widgetClass;)X
2061(*/)X
843 4800(/*)N
934(Some)X
1157(macros)X
1433(to)X
1524(get)X
1654(frequently)X
2038(used)X
2221(components)X
2669(of)X
2764(a)X
2825(widget)X
3087(*/)X
843 4992(#de\256ne)N
1124(XtDisplay\(widget\))X
1943(\(\(widget\)->core.screen->display\))X
843 5088(#de\256ne)N
1124(XtScreen\(widget\))X
1943(\(\(widget\)->core.screen\))X
843 5184(#de\256ne)N
1124(XtWindow\(widget\))X
1943(\(\(widget\)->core.window\))X
843 5280(#de\256ne)N
1124(XtMapWidget\(widget\))X
1943(XMapWindow\(XtDisplay\(widget\),)X
3203(XtWindow\(widget\)\))X
843 5376(#de\256ne)N
1124(XtUnmapWidget\(widget\))X
2603(XUnmapWindow\(XtDisplay\(widget\),)X
3961(XtWindow\(widget\)\))X
843 5472(#de\256ne)N
1124(XtIsComposite\(widget\))X
2603(XtIsSubclass\(widget,)X
3375(\(WidgetClass\)compositeWidgetClass\))X
843 5568(#de\256ne)N
1124(XtClass\(widget\))X
1943(\(\(widget\)->core.widget_class\))X
843 5664(#de\256ne)N
1124(XtSuperclass\(widget\))X
1943(\(XtClass\(widget\)->core_class.superclass\))X
3 f
2239 6144(61)N

62 p
%%Page: 62 64
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(extern)N
1085(Widget)X
1367(XtCreateWidget)X
1967(\(\);)X
931 768(/*)N
1022(String)X
1371(name;)X
1811(*/)X
931 864(/*)N
1022(WidgetClass)X
1517(widgetClass;)X
2061(*/)X
931 960(/*)N
1022(Widget)X
1436(parent;)X
1811(*/)X
931 1056(/*)N
1022(ArgList)X
1428(args;)X
1811(*/)X
931 1152(/*)N
1022(Cardinal)X
1436(num_args;)X
1962(*/)X
843 1344(extern)N
1085(Widget)X
1367(TopLevelCreate)X
1962(\(\);)X
2067(/*hack)X
2324(for)X
2448(now*/)X
931 1440(/*)N
1022(String)X
1349(name;)X
1587(*/)X
931 1536(/*)N
1022(WidgetClass)X
1495(widgetClass;)X
1973(*/)X
931 1632(/*)N
1022(Screen)X
1393(*screen;*/)X
931 1728(/*)N
1022(ArgList)X
1406(args;)X
1599(*/)X
931 1824(/*)N
1022(Cardinal)X
1414(num_args;)X
1808(*/)X
843 2208(extern)N
1085(void)X
1264(XtRealizeWidget)X
1899(\(\);)X
931 2304(/*)N
1022(Widget)X
1370(widget)X
1742(*/)X
843 2496(extern)N
1085(Boolean)X
1401(XtIsRealized)X
1883(\(\);)X
931 2592(/*)N
1022(Widget)X
1370(widget;)X
1657(*/)X
843 2784(extern)N
1085(void)X
1264(XtDestroyWidget)X
1912(\(\);)X
931 2880(/*)N
1022(Widget)X
1304(widget)X
1566(*/)X
843 3072(extern)N
1085(void)X
1264(XtSetSensitive)X
1811(\(\);)X
931 3168(/*)N
1022(Widget)X
1370(widget;)X
1723(*/)X
931 3264(/*)N
1022(Boolean)X
1382(sensitive;)X
1738(*/)X
843 3456(extern)N
1085(void)X
1264(XtSetMappedWhenManaged)X
2312(\(\);)X
931 3552(/*)N
1022(Widget)X
1370(widget;)X
1723(*/)X
931 3648(/*)N
1022(Boolean)X
1382(mappedWhenManaged;)X
2245(*/)X
843 3840(/**********************************************************)N
865 3936(*)N
865 4032(*)N
931(Composite)X
1336(widget)X
1598(Procedures)X
865 4128(*)N
865 4224(**********************************************************)N
843 4416(extern)N
1085(void)X
1264(XtManageChildren)X
1966(\(\);)X
931 4512(/*)N
1022(WidgetList)X
1442(children;)X
1778(*/)X
931 4608(/*)N
1022(Cardinal)X
1392(num_children;)X
1929(*/)X
843 4800(extern)N
1085(void)X
1264(XtManageChild)X
1854(\(\);)X
931 4896(/*)N
1022(Widget)X
1370(child;)X
1594(*/)X
843 5088(extern)N
1085(void)X
1264(XtUnmanageChildren)X
2064(\(\);)X
931 5184(/*)N
1022(WidgetList)X
1442(children;)X
1778(*/)X
931 5280(/*)N
1022(Cardinal)X
1392(num_children;)X
1929(*/)X
843 5472(extern)N
1085(void)X
1264(XtUnmanageChild)X
1952(\(\);)X
931 5568(/*)N
1022(Widget)X
1304(child;)X
1528(*/)X
3 f
2239 6144(62)N

63 p
%%Page: 63 65
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(/*************************************************************)N
865 768(*)N
865 864(*)N
953(Callbacks)X
865 960(*)N
865 1056(**************************************************************/)N
843 1248(typedef)N
1129(void)X
1308(\(*XtCallbackProc\)\(\);)X
931 1344(/*)N
1022(Widget)X
1304(widget;)X
1591(*/)X
931 1440(/*)N
1022(caddr_t)X
1308(closure;)X
1631(data)X
1800(the)X
1930(application)X
2345(registered)X
2714(*/)X
931 1536(/*)N
1022(caddr_t)X
1308(callData;)X
1649(widget)X
1911(instance)X
2222(speci\256c)X
2513(data)X
2682(passed)X
2938(to)X
3029(application*/)X
843 1728(typedef)N
1129(struct)X
1347(_XtCallbackRec)X
1952({)X
931 1824(XtCallbackList)N
1493(next;)X
931 1920(Widget)N
1257(widget;)X
931 2016(XtCallbackProc)N
1516(callback;)X
931 2112(Opaque)N
1248(closure;)X
843 2208(}XtCallbackRec;)N
843 2400(extern)N
1085(XtCallbackKind)X
1685(XtNewCallbackKind\(\);)X
931 2496(/*)N
1022(WidegtClass)X
1495(widgetClass;)X
1973(*/)X
931 2592(/*)N
1022(Cardinal)X
1348(offset;)X
1595(*/)X
843 2784(extern)N
1085(void)X
1264(XtAddCallback)X
1839(\(\);)X
931 2880(/*)N
1022(Widget)X
1436(widget;)X
1767(*/)X
931 2976(/*)N
1022(XtCallbackKind)X
1622(callbackKind;)X
2139(*/)X
931 3072(/*)N
1022(XtCallbackProc)X
1607(callback;)X
1948(*/)X
931 3168(/*)N
1022(caddr_t)X
1418(closure;)X
1741(*/)X
843 3360(extern)N
1085(void)X
1264(XtRemoveCallback)X
1982(\(\);)X
931 3456(/*)N
1022(Widget)X
1436(widget;)X
1767(*/)X
931 3552(/*)N
1022(XtCallbackKind)X
1622(callbackKind;)X
2139(*/)X
931 3648(/*)N
1022(XtCallbackProc)X
1607(callback;)X
1948(*/)X
931 3744(/*)N
1022(caddr_t)X
1418(closure;)X
1741(*/)X
843 4032(extern)N
1085(void)X
1264(XtRemoveAllCallbacks)X
2129(\(\);)X
931 4128(/*)N
1022(Widget)X
1304(widget;)X
1591(*/)X
931 4224(/*)N
1022(XtCallbackKind)X
1622(callbackKind;)X
2139(*/)X
843 4416(extern)N
1085(void)X
1264(XtCallCallbacks)X
1870(\(\);)X
931 4512(/*)N
1022(Widget)X
1326(widget;)X
1613(*/)X
931 4608(/*)N
1022(XtCallbackKind)X
1622(callbackKind;)X
2139(*/)X
931 4704(/*)N
1022(caddr_t)X
1308(callData)X
1624(*/)X
843 4896(/****************************************************************)N
865 4992(*)N
865 5088(*)N
931(Toolkit)X
1214 0.3317(initialization)AX
865 5184(*)N
865 5280(****************************************************************/)N
843 5472(extern)N
1085(Widget)X
1367(XtInitialize\(\);)X
931 5568(/*)N
1022(XtAtom)X
1811(name;)X
2181(*/)X
931 5664(/*)N
1022(XtAtom)X
1811(class;)X
2139(*/)X
931 5760(/*)N
1022(XrmOptionsDescRec)X
1862(options;)X
2235(*/)X
3 f
2239 6144(63)N

64 p
%%Page: 64 66
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
931 672(/*)N
1022(Cardinal)X
1612(num_options;)X
2142(*/)X
931 768(/*)N
1022(Cardinal)X
1811(*argc;)X
2053(*/)X
2144(/*)X
2235(returns)X
2501(count)X
2719(of)X
2814(args)X
2982(not)X
3117(processed)X
3485(*/)X
931 864(/*)N
1022(char)X
1591(**argv;)X
1970(*/)X
843 1056(/****************************************************************)N
865 1152(*)N
865 1248(*)N
931(Memory)X
1256(Management)X
865 1344(*)N
865 1440(****************************************************************/)N
843 1632(extern)N
1085(char)X
1258(*XtMalloc\(\);)X
1745(/*)X
1836(size)X
1995(*/)X
931 1728(/*)N
1022(Cardinal)X
1348(size;)X
1532(*/)X
843 1920(extern)N
1085(char)X
1258(*XtCalloc\(\);)X
1726(/*)X
1817(num,)X
2018(size)X
2177(*/)X
931 2016(/*)N
1022(Cardinal)X
1348(num,)X
1549(size;)X
1733(*/)X
843 2208(extern)N
1085(char)X
1258(*XtRealloc\(\);)X
1765(/*)X
1856(ptr,)X
1998(num)X
2177(*/)X
931 2304(/*)N
1022(char)X
1283(*ptr;)X
1472(*/)X
931 2400(/*)N
1022(Cardinal)X
1348(num;)X
1552(*/)X
843 2592(extern)N
1085(void)X
1264(XtFree\(\);)X
1613(/*)X
1704(ptr)X
1824(*/)X
1063 2688(/*)N
1154(char)X
1349(*ptr)X
1513(*/)X
843 2976(/****************************************************************)N
865 3072(*)N
865 3168(*)N
931(Arg)X
1089(lists)X
865 3264(*)N
865 3360(****************************************************************/)N
843 3552(typedef)N
1129(struct)X
1347({)X
931 3648(String)N
1283(name;)X
931 3744(XtArgValvalue;)N
843 3840(})N
907(Arg,)X
1087(*ArgList;)X
843 4032(#de\256ne)N
1124(XtSetArg\(arg,)X
1646(n,)X
1734(d\))X
1917(\()X
1968(\(arg\).name)X
2373(=)X
2445(\(n\),)X
2591(\(arg\).value)X
2996(=)X
3068(\(XtArgVal\)\(d\))X
3601(\))X
843 4224(extern)N
1085(ArgList)X
1381(XtMergeArgLists\(\);)X
2111(/*)X
2202(args1,)X
2436(num_args1,)X
2871(args2,)X
3105(num_args2)X
3518(*/)X
931 4320(/*)N
1022(ArgList)X
1318(args1;)X
1687(*/)X
931 4416(/*)N
1022(int)X
1226(num_args1;)X
1708(*/)X
931 4512(/*)N
1022(ArgList)X
1318(args2;)X
1687(*/)X
931 4608(/*)N
1022(int)X
1226(num_args2;)X
1708(*/)X
843 4992(/****************************************************************)N
865 5088(*)N
865 5184(*)N
931(Event)X
1159(Management)X
865 5280(*)N
865 5376(****************************************************************/)N
843 5568(/*)N
934(|||)X
1010(Much)X
1237(of)X
1332(this)X
1482(should)X
1739(be)X
1844(private)X
2111(*/)X
843 5664(XtCallbackList)N
1405(DestroyList;)X
843 5760(Display)N
1139(*toplevelDisplay;)X
3 f
2239 6144(64)N

65 p
%%Page: 65 67
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(typedef)N
1129(unsigned)X
1469(long)X
1648(EventMask;)X
843 864(typedef)N
1129(enum)X
1347({pass,ignore,remap})X
2093(GrabType;)X
843 960(typedef)N
1129(void)X
1308(\(*XtEventHandler\)\(\);)X
2092(/*)X
2183(widget,)X
2467(closure,)X
2765(event)X
2978(*/)X
931 1056(/*)N
1022(Widget)X
1326(widget)X
1632(*/)X
931 1152(/*)N
1022(caddr_t)X
1308(closure)X
1606(*/)X
931 1248(/*)N
1022(XEvent)X
1335(*event;)X
1639(*/)X
843 1440(typedef)N
1129(struct)X
1347(_XtEventRec)X
1844({)X
953 1536(_XtEventTable)N
1514(next;)X
953 1632(EventMask)N
1420(mask;)X
953 1728(Boolean)N
1357(non_\256lter;)X
953 1824(XtEventHandler)N
1552(proc;)X
953 1920(Opaque)N
1248(closure;)X
843 2016(}XtEventRec;)N
843 2208(typedef)N
1129(struct)X
1347(_GrabRec)X
1725({)X
931 2304(GrabList)N
1266(next;)X
931 2400(Widget)N
1235(widget;)X
931 2496(Boolean)N
1269(exclusive;)X
843 2592(}GrabRec;)N
843 2784(typedef)N
1129(struct)X
1347(_MaskRec)X
1745({)X
931 2880(EventMask)N
1398(mask;)X
931 2976(GrabType)N
1375(grabType;)X
931 3072(Boolean)N
1335(sensitive;)X
843 3168(}MaskRec;)N
843 3264(#de\256ne)N
1124(is_sensitive)X
1558(TRUE)X
843 3360(#de\256ne)N
1124(not_sensitive)X
1612(FALSE)X
843 3456(GrabRec)N
1177(*grabList;)X
843 3648(extern)N
1085(EventMask)X
1508(_XtBuildEventMask\(\);)X
2343(/*)X
2434(widget)X
2696(*/)X
931 3744(/*)N
1022(Widget)X
1304(widget;)X
1591(*/)X
843 3936(extern)N
1085(void)X
1264(XtAddEventHandler\(\);)X
2097(/*)X
2188(widget,)X
2472(eventMask,)X
2902(other,)X
3127(proc,)X
3327(closure)X
3603(*/)X
931 4032(/*)N
1022(Widget)X
1503(widget)X
1875(*/)X
931 4128(/*)N
1022(EventMask)X
1599(eventMask;)X
2054(*/)X
931 4224(/*)N
1022(Boolean)X
1536(other;)X
1874(*/)X
931 4320(/*)N
1022(XtEventHandler)X
1665(proc;)X
2000(*/)X
931 4416(/*)N
1022(caddr_t)X
1723(closure)X
1999(;)X
2090(*/)X
843 4704(extern)N
1085(void)X
1264(XtRemoveEventHandler\(\);)X
2240(/*)X
2331(widget,eventMask,other,proc,closure)X
3658(*/)X
931 4800(/*)N
1022(Widget)X
1503(widget)X
1875(*/)X
931 4896(/*)N
1022(EventMask)X
1599(eventMask;)X
2054(*/)X
931 4992(/*)N
1022(Boolean)X
1536(other;)X
1874(*/)X
931 5088(/*)N
1022(XtEventHandler)X
1665(proc;)X
2000(*/)X
931 5184(/*)N
1022(caddr_t)X
1723(closure)X
1999(;)X
2090(*/)X
843 5472(extern)N
1085(void)X
1264(XtDispatchEvent\(\);)X
1976(/*)X
2067(event)X
2280(*/)X
931 5568(/*)N
1022(XEvent)X
1503(*event;)X
1785(*/)X
843 5760(extern)N
1085(void)X
1264(XtMainLoop\(\);)X
3 f
2239 6144(65)N

66 p
%%Page: 66 68
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(/****************************************************************)N
865 768(*)N
865 864(*)N
931(Event)X
1159(Gathering)X
1533(Routines)X
865 960(*)N
865 1056(****************************************************************/)N
843 1248(typedef)N
1129(unsigned)X
1469(long)X
1723(XtInputMask;)X
843 1440(#de\256ne)N
1124(XtInputNoneMask)X
2163(0L)X
843 1536(#de\256ne)N
1124(XtInputReadMask)X
2163(\(1L<<0\))X
843 1632(#de\256ne)N
1124(XtInputWriteMask)X
1943(\(1L<<1\))X
843 1728(#de\256ne)N
1124(XtInputExceptMask)X
1943(\(1L<<2\))X
843 1920(extern)N
1085(Atom)X
1308(XtHasInput;)X
843 2016(extern)N
1085(Atom)X
1308(XtTimerExpired;)X
843 2208(extern)N
1085(XtIntervalId)X
1542(XtAddTimeOut\(\);)X
931 2304(/*)N
1022(Widget)X
1304(widget;)X
1943(*/)X
931 2400(/*)N
1022(unsigned)X
1362(long)X
1541(interval;)X
1880(*/)X
843 2592(extern)N
1085(void)X
1264(XtRemoveTimeOut\(\);)X
931 2688(/*)N
1022(XtIntervalId)X
1479(timer;)X
1823(*/)X
843 2880(extern)N
1085(unsigned)X
1425(long)X
1604(XtGetTimeOut\(\);)X
931 2976(/*)N
1022(XtIntervalId)X
1523(timer;)X
1823(*/)X
843 3168(extern)N
1085(void)X
1264(XtAddInput\(\);)X
1794(/*)X
1885(widget,)X
2169(source,)X
2442(condition)X
2798(*/)X
931 3264(/*)N
1022(Widget)X
1304(widget)X
1943(*/)X
931 3360(/*)N
1022(int)X
1138(source;)X
1723(*/)X
931 3456(/*)N
1022(XtInputMask)X
1513(inputMask;)X
1943(*/)X
843 3648(extern)N
1085(void)X
1264(XtRemoveInput\(\);)X
1937(/*)X
2028(widget,)X
2312(source,)X
2585(condition)X
2941(*/)X
931 3744(/*)N
1022(Widget)X
1304(widget)X
1943(*/)X
931 3840(/*)N
1022(int)X
1138(source;)X
1723(*/)X
931 3936(/*)N
1022(XtInputMask)X
1513(inputMask;)X
1943(*/)X
843 4128(extern)N
1085(void)X
1264(XtNextEvent\(\);)X
1834(/*)X
1925(event)X
2138(*/)X
931 4224(/*)N
1022(XtEvent)X
1338(*event;)X
1943(*/)X
843 4416(extern)N
1085(XtPeekEvent\(\);)X
1655(/*)X
1746(event)X
1959(*/)X
931 4512(/*)N
1022(XtEvent)X
1338(*event;)X
1943(*/)X
843 4704(extern)N
1085(Boolean)X
1401(XtPending)X
1800(\(\);)X
843 4992(/****************************************************************)N
865 5088(*)N
865 5184(*)N
931(Geometry)X
1305(Management)X
865 5280(*)N
865 5376(****************************************************************/)N
843 5568(#de\256ne)N
1124(XtDontChange)X
1723(5)X
1789(/*)X
1880(don't)X
2088(change)X
2359(the)X
2489(stacking)X
2805(order)X
3012(stack_mode)X
3455(*/)X
843 5760(typedef)N
1129(struct)X
1347({)X
3 f
2239 6144(66)N

67 p
%%Page: 67 69
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
931 672(XtGeometryMask)N
1588(request_mode;)X
931 768(Position)N
1243(x,)X
1331(y;)X
931 864(Dimension)N
1340(width,)X
1585(height,)X
1850(border_width;)X
931 960(Widget)N
1213(sibling;)X
931 1056(int)N
1047(stack_mode;)X
1503(/*)X
1594(Above,)X
1872(Below,)X
2146(TopIf,)X
2390(BottomIf,)X
2758(Opposite)X
3098(*/)X
843 1152(})N
907(XtWidgetGeometry;)X
843 1440(extern)N
1085(XtGeometryResult)X
1773(XtMakeGeometryRequest\(\);)X
931 1536(/*)N
1044(widget,)X
1328(request,)X
1626(reply)X
2251(*/)X
931 1632(/*)N
1022 0.2552(Widgetwidget;)AX
2031(*/)X
931 1728(/*)N
1022(XtWidgetGeometry)X
1810(*request;)X
2251(*/)X
931 1824(/*)N
1022(XtWidgetGeometry)X
1745(*reply;)X
2039(/*)X
2130(RETURN)X
2504(*/)X
843 2016(extern)N
1085(XtGeometryResult)X
1773(XtMakeResizeRequest)X
2602(\(\);)X
931 2112(/*)N
1022(Widget)X
1370(widget;)X
1723(*/)X
931 2208(/*)N
1022(Dimension)X
1431(width,)X
1676(height;)X
1944(*/)X
931 2304(/*)N
1022(Dimension)X
1431(*replyWidth,)X
1921(*replyHeight;)X
2433(*/)X
843 2496(extern)N
1085(void)X
1264(XtResizeWindow\(\);)X
1995(/*)X
2086(widget)X
2348(*/)X
931 2592(/*)N
1022(Widget)X
1304(widget;)X
1591(*/)X
843 2784(extern)N
1085(void)X
1264(XtResizeWidget\(\);)X
1952(/*)X
2043(widget,)X
2327(width,)X
2572(height,)X
2837(borderWidth)X
3309(*/)X
931 2880(/*)N
1022(Widget)X
1326(widget)X
1588(*/)X
931 2976(/*)N
1022(Dimension)X
1431(width,)X
1676(height,)X
1941(borderWidth;)X
2438(*/)X
843 3168(extern)N
1085(void)X
1264(XtMoveWidget\(\);)X
1922(/*)X
2013(widget,)X
2297(x,)X
2385(y)X
2451(*/)X
931 3264(/*)N
1022(Widget)X
1326(widget)X
1588(*/)X
931 3360(/*)N
1022(Position)X
1334(x,)X
1422(y)X
1510(*/)X
843 3648(/****************************************************************)N
865 3744(*)N
865 3840(*)N
931(Graphic)X
1236(Context)X
1538(Management)X
865 3936(*****************************************************************/)N
843 4128(extern)N
1085(GC)X
1229(XtGetGC\(\);)X
1671(/*)X
1762(widget,)X
2046(valueMask,)X
2476(values)X
2723(*/)X
931 4224(/*)N
1022(Widget)X
1370(widget)X
1632(*/)X
931 4320(/*)N
1022(XtGCMask)X
1449(valueMask;)X
1882(*/)X
931 4416(/*)N
1022(XGCValues)X
1473(*values;)X
1789(*/)X
843 4608(extern)N
1085(void)X
1264(XtDestroyGC)X
1774(\(\);)X
931 4704(/*)N
1022(GC)X
1166(gc;)X
1296(*/)X
843 4896(/****************************************************************)N
865 4992(*)N
865 5088(*)N
931(Resources)X
865 5184(*)N
865 5280(****************************************************************/)N
843 5472(#de\256ne)N
1124(StringToQuark\(string\))X
1938(XrmAtomToQuark\(string\))X
843 5568(#de\256ne)N
1124(StringToName\(string\))X
1929(XrmAtomToName\(string\))X
843 5664(#de\256ne)N
1124(StringToClass\(string\))X
1910(XrmAtomToClass\(string\))X
3 f
2239 6144(67)N

68 p
%%Page: 68 70
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(typedef)N
1129(struct)X
1347(_XtResource)X
1828({)X
931 768(String)N
1257(resource_name;)X
1943(/*)X
2034(Resource)X
2383(name)X
3131(*/)X
931 864(String)N
1257(resource_class;)X
1943(/*)X
2034(Resource)X
2383(class)X
3131(*/)X
931 960(String)N
1257(resource_type;)X
1943(/*)X
2034(Representation)X
2585(type)X
2759(desired)X
3351(*/)X
931 1056(Cardinal)N
1323(resource_size;)X
1943(/*)X
2034(Size)X
2208(in)X
2299(bytes)X
2507(of)X
2602(representation)X
3351(*/)X
931 1152(Cardinal)N
1323(resource_offset;/*)X
1980(Offset)X
2221(from)X
2414(base)X
2592(to)X
2683(put)X
2818(resource)X
3137(value)X
3394(*/)X
931 1248(String)N
1257(default_type;)X
1943(/*)X
2034(representation)X
2555(type)X
2729(of)X
2824(speci\256ed)X
3159(default)X
3426(*/)X
931 1344(caddr_t)N
1305(default_addr;)X
1841(/*)X
1932(Address)X
2241(of)X
2336(default)X
2603(resource)X
3351(*/)X
843 1440(})N
907(XtResource;)X
843 1728(extern)N
1085(void)X
1264(XtGetResources\(\);)X
931 1824(/*)N
1022(Widget)X
1436(widget;)X
1987(*/)X
931 1920(/*)N
1022(ArgList)X
1591(args;)X
2163(*/)X
931 2016(/*)N
1022(int)X
1371(num_args;)X
2163(*/)X
843 2208(extern)N
1085(void)X
1264(XtReadBinaryDatabase)X
2117(\(\);)X
931 2304(/*)N
1022(FILE)X
1296(*f;)X
1943(*/)X
931 2400(/*)N
1022(ResourceDatabase)X
1693(*db;)X
2163(*/)X
843 2592(extern)N
1085(void)X
1264(XtWriteBinaryDatabase)X
2137(\(\);)X
931 2688(/*)N
1022(FILE)X
1296(*f;)X
1943(*/)X
931 2784(/*)N
1022(ResourceDatabase)X
1693(db;)X
2163(*/)X
843 2976(extern)N
1085(void)X
1264(XtSetValues\(\);)X
931 3072(/*)N
1022(Widget)X
1524(widget;)X
1987(*/)X
931 3168(/*)N
1022(ArgList)X
1723(args;)X
2163(*/)X
931 3264(/*)N
1022(int)X
1503(num_args;)X
2029(*/)X
843 3456(extern)N
1085(void)X
1264(XtGetValues\(\);)X
931 3552(/*)N
1022(Widget)X
1524(widget;)X
1987(*/)X
931 3648(/*)N
1022(ArgList)X
1723(args;)X
2163(*/)X
931 3744(/*)N
1022(Cardinal)X
1503(num_args;)X
2029(*/)X
843 3936(extern)N
1085(Widget)X
1367(XtStringToWidget)X
2051(\(\);)X
931 4032(/*)N
1022(String)X
1260(s;)X
1341(*/)X
843 4224(extern)N
1085(WidgetClass)X
1558(XtStringToClass)X
2173(\(\);)X
931 4320(/*)N
1022(String)X
1260(s;)X
1341(*/)X
843 4704(/****************************************************************)N
865 4800(*)N
865 4896(*)N
931(Translation)X
1355(Management)X
865 4992(*)N
865 5088(****************************************************************/)N
843 5280(typedef)N
1129(struct)X
1347(_XtActionsRec{)X
931 5376(char)N
1170(*string;)X
931 5472(caddr_t)N
1217(value;)X
843 5568(})N
907(XtActionsRec;)X
843 5760(/*)N
934(|||)X
1010(Should)X
1282(be)X
1387(private)X
1654(*/)X
3 f
2239 6144(68)N

69 p
%%Page: 69 71
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
843 672(extern)N
1085(void)X
1264(XtDe\256neTranslation)X
2010(\(\);)X
931 768(/*)N
1022(Widget)X
1304(widget)X
1566(*/)X
843 960(/*************************************************************)N
865 1056(*)N
865 1152(*)N
931(Error)X
1138(Handling)X
865 1248(*)N
865 1344(************************************************************/)N
843 1632(extern)N
1085(void)X
1264(XtSetErrorHandler\(\);)X
2038(/*)X
2129(errorProc)X
2482(*/)X
887 1728(/*)N
978(\(*errorProc\)\(String\);)X
1732(*/)X
843 1920(extern)N
1085(void)X
1264(XtError\(\);)X
1664(/*)X
1755(message)X
2075(*/)X
931 2016(/*)N
1022(String)X
1260(message)X
1580(*/)X
843 2208(extern)N
1085(void)X
1264(XtSetWarningHandler\(\);)X
2161(/*)X
2252(errorProc)X
2605(*/)X
887 2304(/*)N
978(\(*errorProc\)\(String\);)X
1732(*/)X
843 2496(extern)N
1085(void)X
1264(XtWarning\(\);)X
1787(/*)X
1878(message)X
2198(*/)X
931 2592(/*)N
1022(String)X
1260(message)X
1580(*/)X
843 2880(#endif)N
1090(_Xtintrinsic_h)X
843 2976(/*)N
934(DON'T)X
1228(ADD)X
1439(STUFF)X
1725(AFTER)X
2026(THIS)X
2243(#endif)X
2490(*/)X
3 f
2239 6144(69)N

1 p
%%Page: 1 72
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
13 s
1886 960(Table)N
2166(of)X
2279(Contents)X
1 f
11 s
555 1372(1.)N
643(Overview)X
1019(..............................................................................................................................)X
3967(1)X
555 1496(2.)N
643(Introduction)X
1107(..........................................................................................................................)X
3967(1)X
555 1620(4.)N
643(Widgets)X
975(................................................................................................................................)X
3967(2)X
555 1744(4.1.)N
709(Core)X
902(Widget)X
1184(De\256nitions)X
1613(...................................................................................................)X
3967(3)X
555 1868(4.1.1.)N
775(CorePart)X
1110(De\256nition)X
1503(........................................................................................................)X
3967(3)X
555 1992(4.1.2.)N
775(CorePart)X
1110(Default)X
1396(Values)X
1679(................................................................................................)X
3967(4)X
555 2116(4.1.3.)N
775(CoreClassPart)X
1301(De\256nitions)X
1723(..............................................................................................)X
3967(4)X
555 2240(4.2.)N
709(Composite)X
1114(Widget)X
1396(De\256nitions)X
1811(..........................................................................................)X
3967(5)X
555 2364(4.2.1.)N
775(CompositePart)X
1322(De\256nition)X
1723(..............................................................................................)X
3967(5)X
555 2488(4.2.2.)N
775(CompositePart)X
1322(Default)X
1608(Values)X
1877(.......................................................................................)X
3967(6)X
555 2612(4.2.3.)N
775(CompositeClassPart)X
1513(De\256nitions)X
1943(....................................................................................)X
3967(6)X
555 2736(4.3.)N
709(Constraint)X
1099(Widget)X
1381(De\256nitions)X
1811(..........................................................................................)X
3967(7)X
555 2860(4.3.1.)N
775(ConstraintPart)X
1307(De\256nition)X
1701(...............................................................................................)X
3967(7)X
555 2984(4.3.2.)N
775(ConstraintClassPart)X
1498(De\256nitions)X
1921(.....................................................................................)X
3967(7)X
555 3108(5.)N
643(Widget)X
925(Classes)X
1217(.....................................................................................................................)X
3967(8)X
555 3232(6.)N
643(Widget)X
925(Subclassing)X
1371(..............................................................................................................)X
3967(8)X
555 3356(6.1.)N
709(Widget)X
991(Subclassing)X
1434(in)X
1525(Public)X
1773(``.h'')X
1977(Files)X
2185(.........................................................................)X
3967(8)X
555 3480(6.2.)N
709(Widget)X
991(Subclassing)X
1434(in)X
1525(Private)X
1797(``.h'')X
2001(Files)X
2207(........................................................................)X
3967(9)X
555 3604(6.3.)N
709(Widget)X
991(Subclassing)X
1434(in)X
1525(``.c'')X
1724(Files)X
1921(.....................................................................................)X
3923(10)X
555 3728(6.4.)N
709(Class)X
922 0.2857(Initialization:)AX
1422(the)X
1552 0.2083(class_initialize)AX
2100(procedure)X
2493(...........................................................)X
3923(12)X
555 3852(6.5.)N
709(XtClass)X
1019(..............................................................................................................................)X
3923(13)X
555 3976(6.6.)N
709(XtSuperclass)X
1195(......................................................................................................................)X
3923(13)X
555 4100(6.7.)N
709(XtIsSubclass)X
1195(......................................................................................................................)X
3923(13)X
555 4224(6.8.)N
709(XtCheckSubclass)X
1371(..............................................................................................................)X
3923(14)X
555 4348(6.9.)N
709(Superclass)X
1107(Chaining)X
1459(..........................................................................................................)X
3923(14)X
555 4472(6.10.)N
753(Inheriting)X
1123(Superclass)X
1521(Operations)X
1943(....................................................................................)X
3923(15)X
555 4596(6.11.)N
753(Calling)X
1036(Superclass)X
1434(Operations)X
1855(........................................................................................)X
3923(15)X
555 4720(7.)N
643(The)X
802(Widget)X
1084(Tree)X
1283(..................................................................................................................)X
3923(15)X
555 4844(8.)N
643(Window)X
968(Attributes)X
1349(...............................................................................................................)X
3923(15)X
555 4968(9.)N
643(Creating)X
969(Widgets)X
1305(.................................................................................................................)X
3923(16)X
555 5092(9.1.)N
709(XtCreateWidget)X
1327(................................................................................................................)X
3923(17)X
555 5216(9.1.1.)N
775(Widget)X
1057 0.2857(Initialization:)AX
1557(the)X
1687 0.3542(initialize)AX
2020(procedure)X
2405(...............................................................)X
3923(17)X
555 5340(9.2.)N
709(XtRealizeWidget)X
1349(...............................................................................................................)X
3923(18)X
555 5464(9.3.)N
709(Window)X
1034(Creation:)X
1385(the)X
1515(realize)X
1772(procedure)X
2163(..........................................................................)X
3923(18)X
555 5588(9.3.1.)N
775(XtCreateWindow)X
1437(...........................................................................................................)X
3923(19)X
555 5712(9.4.)N
709(XtIsRealized)X
1195(......................................................................................................................)X
3923(20)X
555 5836(10.)N
687(Destroying)X
1100(Widgets)X
1437(...........................................................................................................)X
3923(20)X
3 f
2261 6144(1)N

2 p
%%Page: 2 73
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(10.1.)N
753(Adding)X
1039(and)X
1188(Deleting)X
1514(Destroy)X
1814(Callbacks)X
2185(.........................................................................)X
3923(20)X
555 796(10.2.)N
753(XtDestroyWidget)X
1415(............................................................................................................)X
3923(20)X
555 920(10.3.)N
753(Deallocating)X
1226(Dynamic)X
1571(Data:)X
1784(the)X
1914(destroy)X
2195(procedure)X
2581(.......................................................)X
3923(21)X
555 1044(11.)N
687(Callbacks)X
1063(............................................................................................................................)X
3923(21)X
555 1168(11.1.)N
753(Callback)X
1089(Procedure)X
1467(and)X
1616(CallbackList)X
2090(De\256nitions)X
2515(..........................................................)X
3923(21)X
555 1292(11.2.)N
753(Registering)X
1182(CallbackKinds)X
1745(.............................................................................................)X
3923(22)X
555 1416(11.3.)N
753(XtAddCallback)X
1349(...............................................................................................................)X
3923(22)X
555 1540(11.4.)N
753(XtRemoveCallback)X
1481(.........................................................................................................)X
3923(22)X
555 1664(11.5.)N
753(XtRemoveAllCallbacks)X
1635(..................................................................................................)X
3923(23)X
555 1788(11.6.)N
753(XtCallCallbacks)X
1371(..............................................................................................................)X
3923(23)X
555 1912(12.)N
687(Mapping)X
1027(Strings)X
1299(to)X
1390(Widget)X
1672(Instances)X
2021(and)X
2170(Classes)X
2471(............................................................)X
3923(23)X
555 2036(12.1.)N
753(XtStringToWidget)X
1437(...........................................................................................................)X
3923(23)X
555 2160(12.2.)N
753(XtStringToClass)X
1371(..............................................................................................................)X
3923(24)X
555 2284(13.)N
687(Composite)X
1092(Widgets)X
1415(............................................................................................................)X
3923(24)X
555 2408(13.1.)N
753(Composite)X
1158(Widget)X
1440(Type)X
1643(De\256nitions)X
2075(..............................................................................)X
3923(25)X
555 2532(13.2.)N
753(XtIsComposite)X
1327(................................................................................................................)X
3923(25)X
555 2656(13.3.)N
753(Adding)X
1039(New)X
1226(Children:)X
1582(the)X
1712(insert_child)X
2151(procedure)X
2537(.........................................................)X
3923(25)X
555 2780(13.3.1.)N
819(Insertion)X
1154(Order)X
1393(.............................................................................................................)X
3923(26)X
555 2904(13.4.)N
753(Deleting)X
1079(New)X
1266(Children:)X
1622(the)X
1752(delete_child)X
2206(procedure)X
2581(.......................................................)X
3923(26)X
555 3028(13.5.)N
753(Adding)X
1039(and)X
1188(Removing)X
1578(Children)X
1909(from)X
2102(the)X
2232(Managed)X
2581(Set)X
2735(................................................)X
3923(26)X
555 3152(13.5.1.)N
819(XtManageChildren)X
1525(.......................................................................................................)X
3923(26)X
555 3276(13.5.2.)N
819(XtManageChild)X
1415(............................................................................................................)X
3923(27)X
555 3400(13.5.3.)N
819(XtUnmanageChildren)X
1635(..................................................................................................)X
3923(27)X
555 3524(13.5.4.)N
819(XtUnmanageChild)X
1525(.......................................................................................................)X
3923(28)X
555 3648(13.6.)N
753(XtSetMappedWhenManaged)X
1811(..........................................................................................)X
3923(28)X
555 3772(14.)N
687(Toolkit)X
970 0.2644(Initialization)AX
1459(..........................................................................................................)X
3923(29)X
555 3896(14.1.)N
753(Setting)X
1026(Resources)X
1409(from)X
1602(the)X
1732(Commmand)X
2191(Line)X
2383(................................................................)X
3923(29)X
555 4020(15.)N
687(Memory)X
1012(Management)X
1503(........................................................................................................)X
3923(30)X
555 4144(16.)N
687(Input)X
895(and)X
1044(Events)X
1327(................................................................................................................)X
3923(30)X
555 4268(16.1.)N
753(Adding)X
1039(and)X
1188(Deleting)X
1514(Event)X
1742(Sources)X
2053(...............................................................................)X
3923(31)X
555 4392(16.1.1.)N
819(XtAddInput)X
1283(..................................................................................................................)X
3923(31)X
555 4516(16.1.2.)N
819(XtRemoveInput)X
1415(............................................................................................................)X
3923(31)X
555 4640(16.1.3.)N
819(XtAddTimeOut)X
1415(............................................................................................................)X
3923(32)X
555 4764(16.1.4.)N
819(XtRemoveTimeOut)X
1547(......................................................................................................)X
3923(32)X
555 4888(16.2.)N
753(Filtering)X
1080(X)X
1165(Events)X
1437(...........................................................................................................)X
3923(32)X
555 5012(16.2.1.)N
819(Mouse)X
1080(Motion)X
1362(Compression)X
1855(........................................................................................)X
3923(32)X
555 5136(16.2.2.)N
819(XtSetSensitive)X
1371(..............................................................................................................)X
3923(33)X
555 5260(16.3.)N
753(Querying)X
1107(Event)X
1335(Sources)X
1635(..................................................................................................)X
3923(33)X
555 5384(16.3.1.)N
819(XtPending)X
1239(....................................................................................................................)X
3923(33)X
555 5508(16.3.2.)N
819(XtPeekEvent)X
1327(................................................................................................................)X
3923(34)X
555 5632(16.3.3.)N
819(XtNextEvent)X
1327(................................................................................................................)X
3923(34)X
555 5756(16.4.)N
753(Adding)X
1039(and)X
1188(Removing)X
1578(Event)X
1806(Handlers)X
2163(..........................................................................)X
3923(34)X
3 f
2261 6144(2)N

3 p
%%Page: 3 74
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(16.4.1.)N
819(XtAddEventHandler)X
1569(.....................................................................................................)X
3923(34)X
555 796(16.4.2.)N
819(XtRemoveEventHandler)X
1723(..............................................................................................)X
3923(35)X
555 920(16.5.)N
753(Constraining)X
1231(Events)X
1493(to)X
1584(a)X
1645(Cascade)X
1960(of)X
2055(Widgets)X
2383(................................................................)X
3923(35)X
555 1044(16.5.1.)N
819(XtAddGrab)X
1261(...................................................................................................................)X
3923(36)X
555 1168(16.5.2.)N
819(XtRemoveGrab)X
1415(............................................................................................................)X
3923(36)X
555 1292(16.6.)N
753(Dispatching)X
1201(Events)X
1481(.........................................................................................................)X
3923(37)X
555 1416(16.6.1.)N
819(XtDispatchEvent)X
1459(..........................................................................................................)X
3923(37)X
555 1540(16.6.2.)N
819(XtMainLoop)X
1305(.................................................................................................................)X
3923(37)X
555 1664(17.)N
687(Widget)X
969(Visibility)X
1326(and)X
1475(Exposure)X
1833(.........................................................................................)X
3923(37)X
555 1788(17.1.)N
753(Widget)X
1035(Redisplay:)X
1435(the)X
1565(expose)X
1831(procedure)X
2207(........................................................................)X
3923(38)X
555 1912(17.2.)N
753(Exposure)X
1107(Compression)X
1613(...................................................................................................)X
3923(38)X
555 2036(17.3.)N
753(Widget)X
1035(Visibility)X
1393(.............................................................................................................)X
3923(38)X
555 2160(18.)N
687(Size)X
861(and)X
1010(Position)X
1322(of)X
1417(Widgets)X
1733(\320)X
1843(Geometry)X
2217(Management)X
2713(.................................................)X
3923(38)X
555 2284(18.1.)N
753(Making)X
1049(Geometry)X
1423(Manager)X
1757(Requests)X
2097(.............................................................................)X
3923(39)X
555 2408(18.2.)N
753(Making)X
1049(Resize)X
1306(Requests)X
1657(.................................................................................................)X
3923(40)X
555 2532(18.3.)N
753(Geometry)X
1127(Manager)X
1461 0.2109(Responsibilities:)AX
2073(the)X
2203(geometry_manager)X
2905(procedure)X
3285(.......................)X
3923(41)X
555 2656(18.3.1.)N
819(XtMoveWidget)X
1415(............................................................................................................)X
3923(42)X
555 2780(18.3.2.)N
819(XtResizeWidget)X
1437(...........................................................................................................)X
3923(42)X
555 2904(18.4.)N
753(Widget)X
1035(Resizing)X
1366 0.2109(Responsibilities:)AX
1978(the)X
2108(resize)X
2335(procedure)X
2713(.................................................)X
3923(42)X
555 3028(19.)N
687(Input)X
895(Focus)X
1129(.........................................................................................................................)X
3923(43)X
555 3152(20.)N
687(Selections)X
1085(...........................................................................................................................)X
3923(43)X
555 3276(21.)N
687(Resource)X
1036(Management)X
1525(.......................................................................................................)X
3923(43)X
555 3400(21.1.)N
753(XtOffset)X
1085(...........................................................................................................................)X
3923(43)X
555 3524(21.2.)N
753(XtNumber)X
1151(........................................................................................................................)X
3923(44)X
555 3648(21.3.)N
753(Resource)X
1102(Lists)X
1305(.................................................................................................................)X
3923(44)X
555 3772(21.4.)N
753(Superclass)X
1151(to)X
1242(Subclass)X
1572(Chaining)X
1918(of)X
2013(Resource)X
2362(Lists)X
2559(........................................................)X
3923(45)X
555 3896(21.5.)N
753(XtGetSubresources)X
1459(..........................................................................................................)X
3923(45)X
555 4020(21.6.)N
753(Resource)X
1102(Conversions)X
1569(.....................................................................................................)X
3923(46)X
555 4144(22.)N
687(Reading)X
1003(and)X
1152(Writing)X
1449(Widget)X
1731(State)X
1943(....................................................................................)X
3923(46)X
555 4268(22.1.)N
753(XtGetValues)X
1239(....................................................................................................................)X
3923(46)X
555 4392(22.2.)N
753(XtSetValues)X
1239(....................................................................................................................)X
3923(46)X
555 4516(22.2.1.)N
819(Setting)X
1092(Widget)X
1374(State:)X
1598(the)X
1728(set_values)X
2117(procedure)X
2493(...........................................................)X
3923(47)X
555 4640(22.2.2.)N
819(XtSetValuesGeometryRequest)X
1943(....................................................................................)X
3923(47)X
555 4764(23.)N
687(Translation)X
1111(of)X
1206(X)X
1291(Events)X
1553(to)X
1644(Program/Widget)X
2254(functionality)X
2735(................................................)X
3923(48)X
555 4888(23.0.1.)N
819(Translation)X
1243(Tables)X
1503(........................................................................................................)X
3923(48)X
555 5012(23.0.2.)N
819(Function)X
1155(table)X
1349(...............................................................................................................)X
3923(49)X
555 5136(24.)N
687(Graphics)X
1026(Contexts)X
1371(..............................................................................................................)X
3923(49)X
555 5260(25.)N
687(Errors)X
931(..................................................................................................................................)X
3923(50)X
555 5384(25.)N
687(Appendix)X
1056(A:)X
1166(.Xdefaults)X
1552(\256le)X
1687(format)X
1965(...................................................................................)X
3923(51)X
555 5508(25.)N
687(Appendix)X
1056(B:)X
1162(Translation)X
1586(table)X
1780(\256le)X
1915(syntax)X
2185(.........................................................................)X
3923(52)X
555 5632(25.)N
687(Notation)X
1019(..............................................................................................................................)X
3923(52)X
555 5756(25.)N
687(Syntax)X
975(................................................................................................................................)X
3923(52)X
3 f
2261 6144(3)N

4 p
%%Page: 4 75
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(X)N
640(Toolkit)X
3393(Beta)X
3586(Version)X
3901(0.1)X
1 f
555 672(25.)N
687(Modes)X
953(.................................................................................................................................)X
3923(52)X
555 796(25.)N
687(Event)X
915(Types)X
1173(.......................................................................................................................)X
3923(53)X
555 920(25.)N
687(Appendix)X
1056(C:)X
1162(Intrinsic.h)X
1547(......................................................................................................)X
3923(55)X
3 f
2261 6144(4)N
1 f
10 s
0 6360(--)N
4323(--)X

75 p
%%Trailer
xt

xs
