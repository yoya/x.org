%!PS-Adobe-1.0
%%Creator: orpheus:toddb (Todd Brunhoff,,,,)
%%Title: stdin (ditroff)
%%CreationDate: Tue Sep 15 10:30:15 1987
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
8 s
10 s
32(--)Y
4323(--)X
3 f
12 s
1611 984(X)N
1704(Window)X
2074(System)X
2391(Release)X
2725(Notes)X
2063 1104(Version)N
2407(11)X
2092 1224(Release)N
2426(1)X
2 f
10 s
2049 1416(Todd)N
2233(Brunhoff)X
1 f
2046 1560(Tektronix,)N
2402(inc.)X
1950 1656(MIT,)N
2137(Project)X
2384(Athena)X
2 f
2084 1848(Ralph)N
2295(Swick)X
1 f
1783 1992(Digital)N
2025(Equipment)X
2396(Corporation)X
1950 2088(MIT,)N
2137(Project)X
2384(Athena)X
2 f
2068 2280(Bob)N
2217(Schei\257er)X
1 f
1649 2424(MIT,)N
1836(Laboratory)X
2213(for)X
2327(Computer)X
2667(Science)X
2 f
2068 2616(Ron)N
2217(Newman)X
1 f
1950 2760(MIT,)N
2137(Project)X
2384(Athena)X
2 f
2112 2952(Jim)N
2248(Gettys)X
1 f
1783 3096(Digital)N
2025(Equipment)X
2396(Corporation)X
1875 3192(Systems)N
2161(Research)X
2476(Center)X
2 f
2094 3480(ABSTRACT)N
1 f
1043 3672(X)N
1127(Window)X
1428(System)X
1688(version)X
1949(11)X
2054(is)X
2132(a)X
2193(network)X
2481(transparent)X
2863(window)X
3146(system)X
3393(developed)X
843 3768(at)N
938(MIT)X
1122(and)X
1275(is)X
1365(intended)X
1678(to)X
1777(be)X
1890(able)X
2060(to)X
2158(support)X
2434(virtually)X
2741(all)X
2857(known)X
3111(instances)X
3441(of)X
3544(raster)X
843 3864(display)N
1096(hardware.)X
1458(This)X
1622(document)X
1960(is)X
2035(a)X
2093(collection)X
2431(of)X
2520(notes)X
2710(on)X
2811(the)X
2930(system's)X
3231(installation)X
3607(and)X
843 3960(use.)N
3 f
555 4248(1.)N
655(Overview)X
1 f
555 4372(Welcome)N
895(to)X
989(the)X
1119(\256rst)X
1275(release)X
1531(of)X
1630(version)X
1898(11)X
2010(of)X
2109(The)X
2266(X)X
2356(Window)X
2664(System.)X
2971(This)X
3145(release)X
3400(represents)X
3757(a)X
3824(major)X
555 4468(redesign)N
855(and)X
999(enhancement)X
1451(of)X
1546(X)X
1631(and)X
1774(signals)X
2023(its)X
2125(graduation)X
2495(from)X
2678(the)X
2803(research)X
3099(community)X
3490(into)X
3641(the)X
3766(product)X
555 4564(engineering)N
955(and)X
1092(development)X
1527(community.)X
1952(The)X
2098(X)X
2177(Window)X
2474(System)X
2730(version)X
2987(11)X
3088(is)X
3162(intended)X
3459(to)X
3541(be)X
3637(able)X
3791(to)X
3873(sup-)X
555 4660(port)N
716(virtually)X
1019(all)X
1131(known)X
1381(instances)X
1707(of)X
1806(raster)X
2017(display)X
2280(hardware)X
2612(and)X
2760(reasonable)X
3135(future)X
3358(hardware,)X
3709(including)X
555 4756(hardware)N
893(supporting)X
1273(deep)X
1463(frame)X
1689(buffers,)X
1974(multiple)X
2277(colormaps)X
2648(and)X
2801(various)X
3074(levels)X
3298(of)X
3402(hardware)X
3739(graphics)X
555 4852(assist.)N
555 4976(A)N
638(note)X
801(about)X
1004(terminology)X
1419(is)X
1496(in)X
1582(order)X
1776(here.)X
1979(First,)X
2169(the)X
2291(document)X
2631("X)X
2746(Window)X
3046(System)X
3305(Protocol,)X
3620(Version)X
3898(11")X
555 5072(is)N
630(the)X
750(de\256nitive)X
1073(description)X
1450(of)X
1538(what)X
1715(is)X
1789(\(and)X
1953(what)X
2130(is)X
2204(not\))X
2354(part)X
2500(of)X
2588(the)X
2707(core)X
2867(X)X
2946(Window)X
3243(System.)X
3539(This)X
3702(is)X
3776(distinct)X
555 5168(from)N
733(the)X
853(sample)X
1102(server)X
1321(implementation)X
1845(that)X
1987(forms)X
2195(the)X
2314(foundation)X
2682(of)X
2770(this)X
2906(release.)X
3191(The)X
3337(sample)X
3585(server)X
3803(is)X
3877(pro-)X
555 5264(vided)N
755(here)X
916(for)X
1032(the)X
1152(bene\256t)X
1392(of)X
1481(developers)X
1851(who)X
2010(wish)X
2182(to)X
2265(quickly)X
2526(produce)X
2806(an)X
2903(implementation)X
3426(for)X
3541(a)X
3598(new)X
3753(piece)X
3944(of)X
555 5360(hardware.)N
923(The)X
1076(sample)X
1331(server)X
1556(is)X
1636(not)X
1765(necessarily)X
2149(intended)X
2452(to)X
2541(be)X
2644(a)X
2707 0.4531(reference)AX
3035(implementation,)X
3584(although)X
3891(that)X
555 5456(remains)N
832(a)X
890(possibility)X
1244(for)X
1360(the)X
1480(future.)X
1734(Instead,)X
2008(it)X
2074(is)X
2149(our)X
2278(intention)X
2584(that)X
2726(future)X
2940(implementors)X
3402(will)X
3548(be)X
3646(able)X
3802(to)X
3886(rely)X
555 5552(on)N
665(the)X
793(protocol)X
1090(speci\256cation)X
1525(and)X
1671(related)X
1920(documents)X
2297(and)X
2443(not)X
2575(be)X
2681(forced)X
2917(to)X
3009(refer)X
3191(to)X
3282(the)X
3409(code.)X
3630(The)X
3784(sample)X
555 5648(server,)N
793(with)X
956(device-dependent)X
1544(and)X
1681(device-independent)X
2331(implementations)X
2885(as)X
2973(contributed)X
3358(from)X
3534(various)X
3790(places,)X
555 5744(will)N
699(remain)X
942(the)X
1060(foundation)X
1427(of)X
1514(the)X
1632(M.I.T.)X
1879(X)X
1957(Window)X
2253(System)X
2508(distribution.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

2 p
%%Page: 2 2
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(2)X
2323(-)X
1 f
555 672(The)N
705(C)X
783(language)X
1098(interface)X
1405(de\256ned)X
1666(by)X
1771(Xlib)X
1938(is)X
2016(only)X
2183(one)X
2324(of)X
2415(many)X
2617(possible)X
2903(language)X
3217(bindings)X
3516(to)X
3602(the)X
3724(protocol.)X
555 768(While)N
773(we)X
889(certainly)X
1192(encourage)X
1545(standardization)X
2057(of)X
2146(the)X
2266(Xlib)X
2430(interface)X
2734(\(and)X
2898(at)X
2977(least)X
3145(one)X
3282(international)X
3708(standards)X
555 864(organization)N
986(is)X
1069(considering)X
1473(this\),)X
1664(we)X
1787(are)X
1915(not)X
2046(dogmatic)X
2373(on)X
2482(this)X
2626(point.)X
2859(Xlib)X
3030(has)X
3166(two)X
3315(somewhat)X
3669(con\257icting)X
555 960(requirements;)N
1018(\256rst,)X
1184(it)X
1250(is)X
1325(the)X
1445(lowest)X
1676(level)X
1853(procedural)X
2218(interface)X
2521(to)X
2604(the)X
2723(X)X
2802(protocol.)X
3130(As)X
3240(such,)X
3428(it)X
3493(must)X
3669(implement)X
555 1056(all)N
657(the)X
777(nuances)X
1058(possible)X
1341(without)X
1606(encroaching)X
2020(on)X
2121(the)X
2240(vendors')X
2542(and)X
2679(application)X
3056(developers')X
3452(prerogative)X
3839(to)X
3922(set)X
555 1152(conventions)N
966(for)X
1084(style.)X
1298(Xlib's)X
1521(second)X
1767(mission)X
2038(is)X
2114(to)X
2199(be)X
2298(the)X
2419(\256rst)X
2566(level)X
2745(interface)X
3050(for)X
3167(applications)X
3577(development,)X
555 1248(in)N
651(anticipation)X
1062(of)X
1162(future)X
1387(work)X
1585(\(such)X
1792(as)X
1892(the)X
2023(toolkit\))X
2291(on)X
2404(common)X
2717(higher-level)X
3138(applications)X
3558(interfaces.)X
3944(In)X
555 1344(ful\256llment)N
918(of)X
1011(this)X
1152(objective,)X
1492(Xlib)X
1660(includes)X
1953(many)X
2157("convenience)X
2618(routines")X
2935(that)X
3081(are)X
3205(a)X
3266(re-packaging)X
3711(of)X
3803(one)X
3944(or)X
555 1440(more)N
745(protocol)X
1037(operations)X
1396(to)X
1483(simplify)X
1774(common)X
2079(cases,)X
2294(and)X
2435(it)X
2504(includes)X
2796(several)X
3049(additional)X
3394(utility)X
3609(routines)X
3891(that)X
555 1536(have)N
732(no)X
837(direct)X
1045(relationship)X
1448(to)X
1535(the)X
1658(protocol)X
1950(but)X
2077(that)X
2222(are)X
2346(required)X
2639(when)X
2838(building)X
3129("real")X
3341(applications,)X
3773(such)X
3944(as)X
555 1632(window)N
833(managers.)X
555 1756(The)N
701(toolkit)X
930(is)X
1004(a)X
1061('beta)X
1243(test')X
1402(release.)X
1687(The)X
1833(design)X
2063(here)X
2223(should)X
2457(look)X
2620(familiar)X
2895(to)X
2978(those)X
3168(of)X
3256(you)X
3397(who)X
3556(used)X
3723(our)X
3850(early)X
555 1852(prototypes)N
920(under)X
1130(version)X
1393(10,)X
1520(but)X
1649(we)X
1770(have)X
1949(changed)X
2244(most)X
2426(of)X
2520(the)X
2645(interfaces)X
2985(in)X
3074(ways)X
3265(that)X
3411(we)X
3531(hope)X
3713(make)X
3913(the)X
555 1948(task)N
706(of)X
795(writing)X
1048(widgets)X
1319(much)X
1519(easier)X
1729(to)X
1813(accomplish.)X
2240(The)X
2387(earlier)X
2614(prototype)X
2942(toolkit)X
3171(is)X
3245(included)X
3542(on)X
3643(the)X
3762(tape)X
3917(for)X
555 2044(those)N
744(of)X
831(you)X
971(who)X
1129(need)X
1301(it)X
1365(to)X
1447(assist)X
1640(your)X
1807(conversion.)X
2219(You)X
2377(should)X
2610(not)X
2732(expect)X
2962(it)X
3026(to)X
3108(appear)X
3343(on)X
3443(the)X
3561(next)X
3719(release.)X
555 2168(The)N
704(clients)X
937(provided)X
1246(on)X
1350(this)X
1489(release)X
1737(should)X
1974(not)X
2100(be)X
2200(construed)X
2536(as)X
2627(the)X
2749(correct)X
2997(way)X
3155(for)X
3272(them)X
3455(to)X
3540(be)X
3639(written,)X
3909(but)X
555 2264(should)N
790(be)X
888(regarded)X
1192(as)X
1281(what)X
1459(most)X
1636(of)X
1725(them)X
1907(really)X
2112(are:)X
2255(straight-forward)X
2799(ports)X
2981(of)X
3070(existing)X
3345(X)X
3424(version)X
3681(10)X
3782(clients.)X
555 2360(Only)N
738(a)X
797(few)X
941(use)X
1071(the)X
1192(toolkit)X
1423(and)X
1562(only)X
1727(the)X
1848(old)X
1973(version)X
2232(at)X
2313(that.)X
2496(Those)X
2715(of)X
2805(you)X
2948(who)X
3108(used)X
3277(previous)X
3575(versions)X
3864(of)X
3953(X)X
555 2456(will)N
707(\256nd)X
859(several)X
1115(differences)X
1501(in)X
1591(their)X
1766(input,)X
1978(output)X
2209(and)X
2352(options,)X
2634(not)X
2763(all)X
2870(of)X
2964(them)X
3151(documented.)X
3610(Some)X
3819(of)X
3913(the)X
555 2552(differences)N
937(are)X
1060(intentional)X
1426(and)X
1566(some)X
1759(not.)X
1925(Feel)X
2087(free)X
2237(to)X
2323(let)X
2427(us)X
2522(know)X
2724(if)X
2797(you)X
2941(really)X
3148(think)X
3336(something)X
3692(is)X
3768(broken,)X
555 2648(but)N
677(we)X
791(will)X
935(also)X
1084(feel)X
1225(free)X
1371(to)X
1453(respond)X
1727(with)X
1889(an)X
1985(alternative)X
2344(to)X
2426(your)X
2593(proposal.)X
555 2772(By)N
678(default,)X
951(X)X
1038(is)X
1120(distributed)X
1491(to)X
1582(install)X
1806(on)X
1915(a)X
1980(stock)X
2178(4.3BSD)X
2462(system;)X
2735(installation)X
3119(under)X
3331(Ultrix-32)X
3658(release)X
3911(2.0)X
555 2868(should)N
794(essentially)X
1158(be)X
1260(the)X
1384(same.)X
1614(It)X
1688(has)X
1820(been)X
1997(known)X
2240(to)X
2327(compile)X
2610(and)X
2751(run)X
2883(on)X
2988(ATT)X
3169(System)X
3429(V)X
3512(systems)X
3790(but)X
3917(we)X
555 2964(haven't)N
816(had)X
952(access)X
1178(to)X
1260(such)X
1427(a)X
1483(system)X
1725(to)X
1807(verify)X
2019(this.)X
3 f
555 3156(2.)N
655(Installation)X
1 f
555 3280(Given)N
771(an)X
867(X)X
945(distribution)X
1333(tape,)X
1507(the)X
1625(following)X
1956(basic)X
2141(steps)X
2321(need)X
2493(to)X
2575(be)X
2671(performed:)X
555 3404(1\))N
755(Read)X
940(in)X
1022(the)X
1140(tape.)X
555 3528(2\))N
755(Edit)X
915(the)X
1040(con\256guration)X
1494(\256les)X
1654(to)X
1743(your)X
1917(taste.)X
2131(This)X
2299(includes)X
2592(selecting)X
2903(debugging)X
3267(compilations,)X
3726(selecting)X
755 3624(installation)N
1130(directories,)X
1509(etc.)X
555 3748(3\))N
755(Build)X
952(all)X
1052(software.)X
555 3872(4\))N
755(Install)X
975(the)X
1093(software)X
1390(into)X
1534(the)X
1652(directories.)X
555 3996(The)N
700(following)X
1031(steps)X
1211(may)X
1369(not)X
1491(be)X
1587(needed)X
1835(\(or)X
1949(possible\))X
2258(on)X
2358(some)X
2547(systems.)X
555 4120(5\))N
755(Add)X
913(a)X
969(device)X
1199(driver)X
1411(or)X
1498(con\256gure)X
1821(the)X
1939(device)X
2169(driver)X
2381(into)X
2525(your)X
2692(kernel.)X
555 4244(6\))N
755(Rename)X
1038(pseudo-teletypes)X
1597(for)X
1711(your)X
1878(login)X
2062(window.)X
555 4368(7\))N
755(Test)X
913(that)X
1053(everything)X
1416(is)X
1489(working.)X
555 4492(8\))N
755(Con\256gure)X
1095(your)X
1262(/etc/ttys)X
1535(\256le)X
1657(for)X
1771(the)X
1889(display.)X
3 f
555 4684(2.1.)N
715(Installation)X
1125(Steps)X
1 f
555 4808(This)N
717(distribution)X
1105(was)X
1250(tested)X
1457(under)X
1660(the)X
1778(following)X
2109(versions)X
2396(of)X
2483(different)X
2780(vendors)X
3054(systems.)X
8 s
10 f
555 5104(hhhhhhhhhhhhhhhhhh)N
1 f
555 5184(\263)N
603(Ultrix)X
772(and)X
880(Ultrix-32)X
1134(are)X
1227(trademarks)X
1526(of)X
1595(Digital)X
1789(Equipment)X
2086(Corporation)X
555 5264(Copyright)N
831(1985,)X
991(1986,)X
1151(1987)X
1295(by)X
1375(the)X
1469(Massachusetts)X
1854(Institute)X
2080(of)X
2149(Technology)X
555 5344(Permission)N
857(to)X
924(use,)X
1041(copy,)X
1197(modify,)X
1414(and)X
1522(distribute)X
1780(this)X
1889(software)X
2124(and)X
2232(its)X
2309(documentation)X
2705(for)X
2795(any)X
2903(purpose)X
3121(and)X
3229(without)X
3441(fee)X
3534(is)X
3593(here-)X
555 5424(by)N
640(granted,)X
868(provided)X
1116(that)X
1233(the)X
1332(above)X
1505(copyright)X
1771(notice)X
1948(appear)X
2138(in)X
2209(all)X
2294(copies)X
2477(and)X
2589(that)X
2705(both)X
2839(that)X
2955(copyright)X
3220(notice)X
3396(and)X
3508(this)X
3621(per-)X
555 5504(mission)N
773(notice)X
947(appear)X
1133(in)X
1200(supporting)X
1491(documentation,)X
1904(and)X
2013(that)X
2126(the)X
2221(name)X
2376(of)X
2446(M.I.T.)X
2628(not)X
2727(be)X
2804(used)X
2938(in)X
3005(advertising)X
3306(or)X
3376(publicity)X
3621(per-)X
555 5584(taining)N
750(to)X
817(distribution)X
1130(of)X
1200(the)X
1295(software)X
1531(without)X
1744(speci\256c,)X
1972(written)X
2170(prior)X
2311(permission.)X
2641(M.I.T.)X
2823(makes)X
3003(no)X
3084(representations)X
3487(about)X
3645(the)X
555 5664(suitability)N
828(of)X
897(this)X
1006(software)X
1241(for)X
1331(any)X
1439(purpose.)X
1689(It)X
1744(is)X
1803(provided)X
2046("as)X
2141(is")X
2226(without)X
2438(express)X
2645(or)X
2714(implied)X
2926(warranty.)X
3 f
10 s
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

3 p
%%Page: 3 3
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(3)X
2323(-)X
1 f
1043 720(4.3BSD)N
1043 816(Ultrix)N
1254(2.0)X
1043 912(Sun)N
1187(3.2)X
1307(and)X
1443(3.4)X
1043 1008(Apollo)N
1285(9.5)X
1043 1104(IBM)N
1214(4.3)X
1334(Release)X
1604(1.)X
555 1248(You)N
720(are)X
846(on)X
953(your)X
1127(own)X
1292(if)X
1368(you)X
1515(are)X
1641(using)X
1840(versions)X
2133(prior)X
2315(to)X
2403(those)X
2598(listed)X
2797(above.)X
3055(We)X
3193(recommend)X
3598(upgrading)X
3949(to)X
555 1344(the)N
675(later)X
840(releases.)X
1157(In)X
1246(particular,)X
1596(the)X
1716(server)X
1935(will)X
2081(not)X
2205(run)X
2334(on)X
2436(IBM)X
2609(4.2A)X
2789(release)X
3034(2,)X
3115(and)X
3252(previous)X
3549(releases)X
3825(of)X
3913(the)X
555 1440(other)N
745(systems)X
1023(may)X
1186(or)X
1278(may)X
1441(not)X
1568(work)X
1758(properly.)X
2095(It)X
2168(is)X
2245(impossible)X
2615(for)X
2733(us)X
2828(to)X
2914(keep)X
3090(straight)X
3354(all)X
3458(the)X
3580(different)X
3881(ver-)X
555 1536(sions)N
739(of)X
826(different)X
1123(vendor's)X
1424(systems.)X
3 f
555 1756(2.2.)N
715(Prelude)X
1 f
555 1880(NOTE:)N
815(As)X
927(X)X
1008(is)X
1084(a)X
1143(network)X
1429(transparent)X
1809(window)X
2090(system,)X
2355(client)X
2556(programs)X
2882(use)X
3012(network)X
3298(facilities)X
3597(to)X
3682(communi-)X
555 1976(cate)N
706(with)X
869(the)X
988(window)X
1267(system.)X
1549(Make)X
1752(sure)X
1906(your)X
2073(network)X
2356(code)X
2528(works)X
2744(properly)X
3036(BEFORE)X
3362(attempting)X
3724(to)X
3806(use)X
3933(X.)X
555 2072(For)N
687(example,)X
2 f
1000(talk\(1\))X
1 f
1235(should)X
1469(be)X
1566(working)X
1854(properly)X
2147(between)X
2436(normal)X
2684(terminals,)X
3023(or)X
3111(you)X
3251(should)X
3484(be)X
3580(able)X
3734(to)X
2 f
3816(rlogin)X
1 f
555 2168(to)N
637(either)X
840(the)X
958(local)X
1134(or)X
1221(to)X
1303(another)X
1564(machine)X
1856(if)X
1925(you)X
2065(have)X
2237(a)X
2293(network.)X
3 f
555 2360(2.3.)N
715(Reading)X
1019(the)X
1146(distribution.)X
1 f
555 2484(The)N
703(software)X
1003(comes)X
1231(on)X
1334(a)X
1393(single)X
1607(1600)X
1790(BPI)X
1937(magnetic)X
2254(tape)X
2411(in)X
2 f
2496(tar)X
1 f
2589(\(1\))X
2706(format.)X
2983(All)X
3108(\256les)X
3264(will)X
3411(be)X
3510(extracted)X
3828(into)X
3975(a)X
555 2580(directory)N
865(named)X
3 f
1099(X)X
1 f
1157(.)X
1217(An)X
1335(example)X
1627(command)X
1963(would)X
2183(be:)X
843 2724(%)N
930(tar)X
1035(xf)X
1122(/dev/rmt8)X
555 2896(If)N
633(you)X
777(move)X
979(the)X
1101(distribution)X
1493(to)X
1579(a)X
1639(different)X
1940(machine)X
2236(than)X
2398(the)X
2520(one)X
2659(you)X
2802(read)X
2964(the)X
3085(tape)X
3242(on,)X
3365(use)X
3495(care)X
3653(to)X
3738(preserve)X
555 2992(the)N
678(symbolic)X
996(links,)X
1196(either)X
1404(by)X
1509(moving)X
1778(the)X
1901(distribution)X
2294(as)X
2386(a)X
2447(single)X
2663(\256le)X
2790(or)X
2882(by)X
2987(using)X
2 f
3185(tar)X
1 f
3303(across)X
3529(the)X
3651(net.)X
3813(If)X
3891(you)X
555 3088(don't,)N
769(you)X
914(will)X
1063(end)X
1204(up)X
1309(with)X
1475(more)X
1664(than)X
1826(one)X
1966(copy)X
2146(of)X
2237(certain)X
2480(key)X
2620(include)X
2880(\256les,)X
3057(and)X
3197(may)X
3359(get)X
3481(confused)X
3795(later)X
3962(if)X
555 3184(you)N
695(make)X
889(changes.)X
3 f
555 3376(2.4.)N
715(Con\256guring)X
1147(the)X
1274(Release)X
1 f
555 3500(Much)N
785(of)X
895(the)X
1036(software)X
1355(compilation)X
1779(for)X
1915(this)X
2072(release)X
2338(can)X
2492(be)X
2610(controlled)X
2977(by)X
3099(three)X
3302(\256les)X
3477(in)X
3581(the)X
3721(directory)X
555 3596(util/imake.includes:)N
1213(Imake.tmpl,)X
1624(Imake.rules)X
2025(and)X
2165(especially,)X
2530(one)X
2670(of)X
2761(the)X
2883(*.macros)X
3199(\256les)X
3356(that)X
3500(pertains)X
3778(to)X
3864(your)X
555 3692(machine.)N
888(If)X
963(you)X
1104(have)X
1277(one)X
1414(of)X
1502(the)X
1621(machines)X
1945(that)X
2086(has)X
2214(been)X
2387(tested)X
2595(for)X
2709(this)X
2844(release,)X
3108(you)X
3248(will)X
3392(probably)X
3697(only)X
3859(need)X
555 3788(to)N
639(consider)X
933(the)X
1053(appropriate)X
1441(*.macros)X
1755(\256le;)X
1901(e.g.)X
2039(on)X
2141(a)X
2199(vax,)X
2357(you)X
2498(should)X
2732(look)X
2895(at)X
2974(Vax.macros.)X
3401(If)X
3476(you)X
3617(are)X
3737(trying)X
3949(to)X
555 3884(bring)N
749(up)X
853(a)X
913(machine)X
1209(for)X
1327(which)X
1547(MIT)X
1718(has)X
1849(not)X
1975(done)X
2155(any)X
2295(testing,)X
2552(we)X
2670(suggest)X
2934(you)X
3078(read)X
3241(the)X
3363(man)X
3525(page)X
3701(for)X
2 f
3819(imake)X
1 f
555 3980(very)N
718(carefully,)X
1044(and)X
1180(start)X
1338(by)X
1438(copying)X
1716(only)X
1878(of)X
1965(these)X
2150(*.macros)X
2462(\256les)X
2615(as)X
2702(a)X
2758(starting)X
3018(place.)X
555 4104(Each)N
740(of)X
831(the)X
953(*.macros)X
1269(\256les)X
1426(has)X
1557(in)X
1643(it)X
1711(every)X
1914(parameter)X
2260(that)X
2403(can)X
2538(affect)X
2745(the)X
2866(con\256guration)X
3316(of)X
3406(the)X
3527(software)X
3827(build,)X
555 4200(either)N
758(commented)X
1152(out)X
1274(or)X
1361(appearing)X
1698(with)X
1860(an)X
1956(appropriate)X
2342(value.)X
2576(Let)X
2703(us)X
2794(consider)X
3086(each.)X
555 4324(TOP)N
726(=)X
791(TOPDIR)X
755 4420(This)N
917(should)X
1150(remain)X
1393(unchanged.)X
555 4544(AS)N
677(=)X
742(??,)X
854(CC)X
980(=)X
1045(??,)X
1157(CPP)X
1318(=)X
1383(??,)X
1495(LD)X
1622(=)X
1687(??,)X
1799(LINT)X
2002(=)X
2067(??,)X
2179(MAKE)X
2435(=)X
2500(??)X
755 4640(These)N
970(should)X
1206(be)X
1305(selected)X
1587(as)X
1677(you)X
1820(would)X
2043(normally)X
2355(for)X
2472(the)X
2 f
2593(make)X
1 f
2786(program)X
3081(on)X
3183(your)X
3352(machine.)X
3686(Use)X
3833(exist-)X
755 4736(ing)N
877(values)X
1102(as)X
1189(a)X
1245(guide.)X
555 4860(CDEBUGFLAGS)N
1157(=)X
1222(??)X
755 4956(These)N
970(\257ags)X
1144(are)X
1266(used)X
1436(in)X
1521(every)X
1723(make\256le)X
2022(\(except)X
2282(for)X
2399(the)X
2520(server\))X
2767(to)X
2852(determine)X
3196(what)X
3375(kind)X
3540(of)X
3629(compilation)X
755 5052(takes)N
940(place.)X
1170(Usually)X
1439(this)X
1574(is)X
1647("CDEBUGFLAGS)X
2282(=)X
2347(-O".)X
555 5176(DESTDIR)N
913(=)X
978(??)X
755 5272(We)N
891(recommend)X
1294(leaving)X
1554(the)X
1676(value)X
1874(blank.)X
2116(It)X
2189(is)X
2265(only)X
2430(used)X
2600(by)X
2703("install")X
2987(targets)X
3224(in)X
3309(the)X
3430(make\256les)X
3760(and)X
3899(can)X
755 5368(be)N
851(overridden)X
1219(throughout)X
1590(an)X
1686(entire)X
1889(subtree)X
2141(of)X
2228(the)X
2346(source)X
2576(with)X
2738(a)X
2794(command)X
3130(like)X
1043 5512(%)N
1130(make)X
1324(DESTDIR=/foo)X
555 5684(#de\256ne)N
811(X???Server)X
755 5780(This)N
921(should)X
1158(be)X
1258(de\256ned)X
1518(to)X
1604(be)X
1704(the)X
1826(name)X
2024(of)X
2115(whatever)X
2433(server)X
2653(you)X
2796(want)X
2975(to)X
3060(build;)X
3269(e.g.,)X
3428(at)X
3509(Project)X
3759(Athena,)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

4 p
%%Page: 4 4
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(4)X
2323(-)X
1 f
755 672(we)N
873(have)X
1049(both)X
1215(QVSS)X
1443(and)X
1583(QDSS)X
1811(displays)X
2097(for)X
2215(our)X
2346(DEC)X
2530(MicroVaxes,)X
2971(so)X
3066(we)X
3184(have)X
3359(both)X
3524(servers)X
3775(de\256ned)X
755 768(here:)N
1043 912(#de\256ne)N
1299(XqvssServer)X
1729(Xqvss)X
1043 1008(#de\256ne)N
1299(XqdssServer)X
1729(Xqdss)X
755 1152(You)N
915(should)X
1150(only)X
1314(chose)X
1519(X???Server)X
1917(values)X
2144(already)X
2403(found)X
2612(in)X
2696(one)X
2834(of)X
2923(the)X
3042(existing)X
3316(*.macros)X
3629(\256les,)X
3803(or)X
3891(you)X
755 1248(should)N
988(be)X
1084(quite)X
1264(sure)X
1418(you)X
1558(understand)X
2 f
1930(imake)X
1 f
2122(,)X
2162(util/imake.includes/Imake.rules)X
3192(and)X
3328(server/Imake\256le.)X
555 1372(#de\256ne)N
811(ExtensionDe\256nes)X
755 1468(The)N
901(value)X
1096(for)X
1211(ExtensionDe\256nes)X
1797(is)X
1871(used)X
2039(to)X
2122(determine)X
2464(which)X
2681(extensions)X
3040(are)X
3160(pulled)X
3380(into)X
3524(the)X
3642(server.)X
3899(We)X
755 1564(distribute)N
1082(the)X
1205(system)X
1452(with)X
1619(the)X
1742(value)X
1941(set)X
2055(to)X
2142(nil)X
2251(because)X
2531(neither)X
2779(of)X
2871(the)X
2993(two)X
3137(working)X
3428(extensions)X
3790(are)X
3913(yet)X
755 1660(considered)N
1136(standard,)X
1461(but)X
1596(you)X
1749(may)X
1920(want)X
2109(to)X
2203(try)X
2324(them.)X
2556(For)X
2699(example,)X
3023(you)X
3175(may)X
3345(turn)X
3506(on)X
3618(the)X
3748("bezier")X
755 1756(extension)N
1082(with)X
1043 1900(#de\256ne)N
1299(ExtensionDe\256nes)X
1884(-DBEZIER)X
755 2044(In)N
863(any)X
1020(case,)X
1220(ExtensionDe\256nes)X
1826(must)X
2022(be)X
2139(de\256ned)X
2416(to)X
2519(be)X
2636(something)X
3010(or)X
3118(nothing.)X
3443(It)X
3533(cannot)X
3788(be)X
3904(left)X
755 2140(unde\256ned.)N
555 2264(#de\256ne)N
811(DebugLib??)X
1228(YES)X
555 2388(#de\256ne)N
811(Pro\256leLib??)X
1227(NO)X
555 2512(#de\256ne)N
811(DebugOldLib??)X
1348(YES)X
555 2636(#de\256ne)N
811(Pro\256leOldLib??)X
1347(NO)X
755 2732(All)N
882(of)X
974(the)X
1097(de\256nes)X
1349(of)X
1441(this)X
1580(form)X
1760(must)X
1939(be)X
2039(de\256ned)X
2299(to)X
2385(be)X
2485(either)X
2692(YES)X
2867(or)X
2958(NO.)X
3138(De\256ning)X
3442(DebugLibX)X
3849(to)X
3935(be)X
755 2828(YES)N
927(will)X
1072(mean)X
1267(that)X
1408(you)X
1549(will)X
1694(get)X
1813(a)X
1869(debug)X
2085(\(compiled)X
2430(with)X
2592(-g\))X
2706(version)X
2 f
2962(in)X
3044(addition)X
1 f
3330(to)X
3412(the)X
3530(normal)X
3777(library.)X
755 2924(De\256ning)N
1058(Pro\256leOldXMenu)X
1659(to)X
1744(be)X
1842(NO,)X
2000(will)X
2146(mean)X
2342(that)X
2484(you)X
2626(will)X
2772(not)X
2896(get)X
3016(a)X
3074(pro\256led)X
3345(library)X
3581(in)X
3665(addition)X
3949(to)X
755 3020(the)N
873(normal)X
1120(library.)X
555 3144(#de\256ne)N
811(BigEndianArchitecture)X
1575(??)X
755 3240(This)N
920(must)X
1098(be)X
1197(de\256ned.)X
1496(If)X
1573(you)X
1716(are)X
1838(on)X
1941(a)X
2000(machine)X
2295(that)X
2438(has)X
2567(Vax-like)X
2870(architecture,)X
3292(it)X
3358(should)X
3593(be)X
3691(de\256ned)X
3949(to)X
755 3336(be)N
852(NO.)X
1029(A)X
1108(68000-like)X
1475(architure)X
1781(should)X
2014(get)X
2132(a)X
2188(YES.)X
2399(This)X
2561(is)X
2634(only)X
2796(used)X
2963(by)X
3063(the)X
3181(toolkit)X
3409(and)X
3545(may)X
3703(disappear)X
755 3432(someday.)N
555 3556(#de\256ne)N
811(CppSourcesPresent)X
755 3652(This)N
920(should)X
1155(only)X
1319(be)X
1417(de\256ned)X
1675(if)X
1746(you)X
1888(have)X
2062(sources)X
2325(to)X
2409(the)X
2529(Berkeley)X
2841(version)X
3099(of)X
3188(cpp)X
3326(\(although)X
3655(some)X
3846(other)X
755 3748(versions)N
1049(may)X
1214(work\).)X
1473(Its)X
1580(value)X
1781(should)X
2021(be)X
2124(the)X
2248(pathname)X
2586(of)X
2679(the)X
2803(directory)X
3119(containing)X
3483(the)X
3607(source.)X
3883(It)X
3958(is)X
755 3844(only)N
917(used)X
1084(by)X
1184(the)X
1302(sources)X
1563(in)X
1645(util/makedepend.)X
555 3968(#de\256ne)N
811(ManSuf\256x)X
1173(n)X
755 4064(Currently)N
1092(this)X
1237(is)X
1320(a)X
1386(poor)X
1563(solution)X
1850(for)X
1974(installing)X
2301(manpages.)X
2691(This)X
2862(should)X
3104(probably)X
3418(always)X
3670(be)X
3775(de\256ned)X
755 4160(this)N
890(way.)X
555 4284(#de\256ne)N
811(ConstructMFLAGS)X
755 4380(This)N
919(should)X
1154(only)X
1318(be)X
1416(de\256ned)X
1674(if)X
1745(your)X
2 f
1914(make)X
1 f
2105(program)X
2398(internally)X
2726(maintains)X
3058(the)X
3177(macro)X
3399(MAKEFLAGS)X
3909(but)X
755 4476(not)N
877(MFLAGS.)X
3 f
555 4668(2.5.)N
715(Building)X
1026(the)X
1153(Release)X
1 f
555 4792(Compiling)N
919(all)X
1022(software)X
1322(takes)X
1510(quite)X
1693(a)X
1752(while.)X
1973(Building)X
2275(the)X
2396(X)X
2477(library)X
2713(takes)X
2900(the)X
3020(longest,)X
3293(as)X
3382(there)X
3565(are)X
3686(more)X
3873(than)X
555 4888(two)N
700(hundred)X
988(modules.)X
1324(This)X
1491(should)X
1728(complete)X
2046(without)X
2314(error)X
2495(on)X
2599(most)X
2778(machines.)X
3145(After)X
3339(having)X
3581(selected)X
3864(your)X
555 4984(con\256guration,)N
1022(simply)X
1259(type)X
843 5128(%)N
930(make)X
1124(-k)X
1211(>&)X
1338(make.log)X
555 5272(in)N
645(the)X
771(top)X
901(level)X
1085(directory.)X
1443(This)X
1613(will)X
1765(take)X
1926(from)X
2109(two)X
2256(to)X
2345(twelve)X
2586(hours)X
2791(or)X
2885(more)X
3077(depending)X
3438(on)X
3545(your)X
3719(machine.)X
555 5368(The)N
700(-k)X
787(option)X
1011(will)X
1155(allow)X
1353(it)X
1417(to)X
1499(continue)X
1795(if)X
1864(it)X
1928(encounters)X
2296(any)X
2432(errors.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

5 p
%%Page: 5 5
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(5)X
2323(-)X
555 672(2.6.)N
715(Installing)X
1058(the)X
1185(X)X
1263(Executables)X
1 f
555 796(At)N
659(this)X
798(point,)X
1006(you)X
1150(should)X
1387(check)X
1599(the)X
1721(\256le)X
1847(util/imake.include/Imake.tmpl)X
2840(to)X
2925(be)X
3024(sure)X
3181(that)X
3324(the)X
3445(installation)X
3823(direc-)X
555 892(tories,)N
783(those)X
982(macro)X
1213(names)X
1448(ending)X
1696(in)X
1788("...DIR")X
2082(are)X
2211(right)X
2392(for)X
2516(your)X
2693(machine.)X
3035(We)X
3177(believe)X
3438(that)X
3587(/usr/bin/X11,)X
555 988(/usr/lib/X11,)N
981(etc)X
1095(are)X
1214(the)X
1332(right)X
1503(values,)X
1748(but)X
1870(you)X
2010(may)X
2168(disagree.)X
555 1112(Then,)N
760(as)X
847(super)X
1041(user,)X
1215(execute)X
1481(the)X
1599(command)X
843 1256(#)N
903(make)X
1097(install)X
555 1400(This)N
717(should)X
950(complete)X
1264(without)X
1528(error.)X
555 1524(This)N
737(also)X
886(copies)X
1111(all)X
1211(necessary)X
1544(\256les)X
1697(for)X
1811(users)X
1996(to)X
2078(program)X
2370(using)X
2563(X)X
2641(into)X
2 f
2785(/usr/include/X)X
1 f
3238(.)X
555 1648(You)N
720(should)X
960(also)X
1116(install)X
1338(the)X
1463(correct)X
1714(termcap)X
2000(and)X
2143(terminfo)X
2446(\(if)X
2549(applicable\))X
2932(\256les)X
3091(out)X
3219(of)X
2 f
3312(clients/xterm/termcap)X
1 f
555 1744(and)N
2 f
691(clients/xterm/terminfo)X
1 f
1418(into)X
2 f
1562(/etc/termcap)X
1 f
1983(and)X
2119(your)X
2286(terminfo)X
2582(database)X
2879(if)X
2948(they)X
3106(are)X
3225(not)X
3347(already)X
3604(there.)X
555 1868(The)N
2 f
708(xinit)X
1 f
878(program,)X
1197(useful)X
1420(on)X
1527(machines)X
1857(without)X
2128(the)X
2253(proper)X
2490(init)X
2623(support)X
2890(for)X
3011(login,)X
3222(expects)X
3490(that)X
3637(the)X
3762(running)X
555 1964(server)N
777(to)X
864(be)X
965(called)X
1182("X".)X
1371(You)X
1534(can)X
1671(either)X
1879(rename)X
2141(the)X
2264(appropriate)X
2655(server)X
2877(for)X
2996(your)X
3168(display,)X
3443(or)X
3534(use)X
3665(the)X
3787(correct)X
555 2060(arguments)N
909(to)X
2 f
991(xinit)X
1 f
1133(.)X
3 f
555 2252(2.7.)N
715(Building)X
1026(a)X
1086(Kernel)X
1342(With)X
1535(the)X
1662(Device)X
1910(Driver)X
1 f
555 2376(On)N
682(some)X
880(machines,)X
1232(the)X
1359(display)X
1618(driver)X
1838(or)X
1933(other)X
2126(auxilary)X
2417(\256le)X
2547(may)X
2713(not)X
2843(come)X
3045(con\256gured)X
3416(into)X
3568(the)X
3694(system)X
3944(or)X
555 2472(other)N
744(device)X
978(\256les)X
1135(may)X
1297(not)X
1423(exist.)X
1638(You)X
1800(must)X
1979(add)X
2119(a)X
2179(line)X
2323(to)X
2409(your)X
2579(con\256guration)X
3029(\256le)X
3154(for)X
3271(each)X
3442(display)X
3696(you)X
3839(have.)X
555 2568(Make)N
760(sure)X
916(the)X
1036(CSR)X
1208(address)X
1471(matches)X
1756(between)X
2046(your)X
2215(con\256guration)X
2664(\256le)X
2788(and)X
2926(your)X
3095(hardware.)X
3456(Con\256gure,)X
3817(make,)X
555 2664(and)N
701(install)X
925(the)X
1052(kernel)X
1282(containing)X
1649(the)X
1776(display)X
2036(driver.)X
2297(When)X
2518(you)X
2667(reboot)X
2901(the)X
3028(machine,)X
3349(make)X
3552(sure)X
3715(that)X
3864(your)X
555 2760(display)N
806(auto)X
964(con\256gures)X
1318(during)X
1547(boot.)X
555 2884(As)N
669(the)X
792(superuser,)X
1145(you)X
1290(should)X
1528(also)X
1682(make)X
1881(a)X
1941(device)X
2175(entry)X
2364(for)X
2482(each)X
2654(display.)X
2949(Some)X
3155(examples)X
3482(of)X
3573(the)X
3695(command)X
555 2980(would)N
775(be)X
843 3124(#)N
903(/etc/mknod)X
1283(/dev/mouse)X
1672(c)X
1728(35)X
1828(2)X
2043(#)X
2103(if)X
2172(/dev/mouse)X
2561(does)X
2728(not)X
2850(exist)X
3021(on)X
3121(a)X
3177(VS-2.)X
843 3220(#)N
903(/etc/mknod)X
1283(/dev/bell)X
1583(c)X
1639(12)X
1739(2)X
1843(#)X
1903(for)X
2017(bell)X
2157(to)X
2239(ring)X
2388(on)X
2488(a)X
2544(Sun.)X
843 3316(#)N
903(MAKEDEV)X
1324(displays)X
1843(#)X
1903(for)X
2017(displays)X
2299(on)X
2399(the)X
2517(RT/PC)X
555 3460(Normally,)N
904(the)X
1024(protection)X
1371(on)X
1473(the)X
1593(device)X
1825(would)X
2047(be)X
2144(only)X
2307(user)X
2462(read/write,)X
2829(but)X
2952(for)X
3067(debugging)X
3426(purposes)X
3732(you)X
3873(may)X
555 3556(want)N
732(to)X
815(temporarily)X
1210(change)X
1459(it.)X
1564(On)X
1683(a)X
1740(DEC)X
1921(VS-2,)X
2131(you)X
2272(should)X
2506(also)X
2656(adb)X
2793(/sys/vaxuba/qv.o)X
3354(and)X
3491(change)X
3740(the)X
3859(vari-)X
555 3652(able)N
713("qv_def_scrn")X
1200(to)X
1286(2)X
1349(and)X
1488(rebuild)X
1738(your)X
1908(system.)X
2193(This)X
2358(will)X
2505(cause)X
2707(the)X
2828(correct)X
3075(screen)X
3304(parameters)X
3680(to)X
3765(be)X
3864(used)X
555 3748(on)N
655(the)X
773(VR-260)X
1051(monitor.)X
3 f
555 3940(2.8.)N
715(Testing)X
988(and)X
1136(Trouble)X
1431(Shooting)X
1 f
555 4064(We)N
689(highly)X
915(recommend)X
1316(testing)X
1551(your)X
1719(newly)X
1936(built)X
2103(server)X
2321(from)X
2498(a)X
2555(separate)X
2840(terminal)X
3128(before)X
3355(attempting)X
3718(to)X
3801(enable)X
555 4160(login)N
744(on)X
849(the)X
972(display.)X
1268(In)X
1360(this)X
1500(way,)X
1679(error)X
1861(messages)X
2189(will)X
2337(go)X
2441(to)X
2527(your)X
2698(terminal,)X
3009(rather)X
3221(than)X
3383(being)X
3585(logged)X
3827(in)X
3913(the)X
555 4256(\256le)N
2 f
677(/usr/adm/X?msgs)X
1 f
1232(.)X
1292(Testing)X
1552(is)X
1625(most)X
1800(easily)X
2007(performed)X
2362(from)X
2538(terminal)X
2825(or)X
2912(from)X
3088(across)X
3309(the)X
3427(network.)X
555 4380(To)N
664(test)X
795(a)X
851(Sun,)X
1015(for)X
1129(example,)X
1441(you)X
1581(would)X
1801(change)X
2049(directory)X
2359(to)X
2441(/usr/bin/X11)X
2865(\(or)X
2979(wherever)X
3299(you)X
3439(decided)X
3709(to)X
3791(put)X
3913(the)X
555 4476(executable)N
930(programs\),)X
1310(and)X
1456(would)X
1686(type)X
1854(``Xsun)X
2107(&''.)X
2293(\(For)X
2461(a)X
2527(MicroVAX,)X
2947(the)X
3075(command)X
3421(would)X
3651(be)X
3757(``Xqvss)X
555 4572(&''\).)N
764(There)X
978(can)X
1116(only)X
1284(be)X
1385(one)X
1526(display)X
1782(on)X
1887(a)X
1948(MicroVAX.)X
2383(See)X
2524(the)X
2 f
2647(X\(8c\))X
1 f
2851(and)X
2992(the)X
3115(manual)X
3376(pages)X
3584(for)X
3703(particular)X
555 4668(servers)N
803(manual)X
1059(page)X
1231(for)X
1345(other)X
1530(options.)X
555 4792(If)N
635(everything)X
1004(succeeds,)X
1336(you)X
1482(should)X
1721(get)X
1845(a)X
1907(grey)X
2076(background)X
2481(and)X
2623(a)X
2685(large)X
2872(X)X
2956(cursor)X
3183(on)X
3289(the)X
3413(screen.)X
3685(If)X
3765(not,)X
3913(the)X
555 4888(most)N
731(common)X
1032(mistake)X
1302(is)X
1376(for)X
1491(the)X
1610(fonts)X
1791(or)X
1878(\256rmware)X
2188(to)X
2270(be)X
2366(in)X
2448(the)X
2566(wrong)X
2791(location)X
3069(or)X
3156(a)X
3212(directory)X
3522(or)X
3609(\256le)X
3731(to)X
3813(not)X
3935(be)X
555 4984(readable.)N
555 5108(You)N
714(should)X
948(now)X
1107(be)X
1204(able)X
1358(to)X
1440(run)X
1567(any)X
1703(of)X
1790(the)X
1908(X)X
1986(programs)X
2309(from)X
2485(the)X
2603(separate)X
2887(terminal)X
3174(or)X
3261(network)X
3544(login.)X
3768(A)X
3846(com-)X
555 5204(mon)N
721(beginning)X
1065(mistake)X
1338(is)X
1415(to)X
1501(forget)X
1717(to)X
1802(set)X
1914(the)X
2035("DISPLAY")X
2462(environment)X
2890(variable.)X
3212(Most)X
3399(programs)X
3725(also)X
3877(take)X
555 5300(arguments)N
916(to)X
1005(specify)X
1264(the)X
1389(host)X
1549(and)X
1692(display)X
1950(number.)X
2262(So,)X
2393(for)X
2514(convenience)X
2943(while)X
3148(testing,)X
3407(you)X
3553(might)X
3765(execute)X
555 5396(the)N
675(command)X
1012(``setenv)X
1292(DISPLAY)X
2 f
1651(yourhost)X
1 f
(:0'')S
2068(where)X
2 f
2286(yourhost)X
1 f
2587(is)X
2661(the)X
2780(name)X
2975(of)X
3063(your)X
3231(machine)X
3524(when)X
3719(using)X
3913(the)X
555 5492(C-shell.)N
850(You)X
1012(should)X
1249(edit)X
1393(your)X
1564(.login)X
1772(or)X
1863(.pro\256le)X
2116(so)X
2211(that)X
2355(his)X
2472(variable)X
2755(will)X
2903(be)X
3003(set)X
3116(for)X
3234(you)X
3378(automatically)X
3837(when)X
555 5588(you)N
695(log)X
817(in)X
899(in)X
981(the)X
1099(future.)X
2 f
555 5712(Xterm)N
1 f
779(may)X
945(also)X
1102(fail)X
1237(if)X
1313(the)X
1438(\256les)X
1598(described)X
1933(in)X
2022(the)X
2147(previous)X
2450(section)X
2704(are)X
2830(not)X
2959(consistent.)X
3346(If)X
3427(the)X
3552(xterm)X
3766(is)X
3846(to)X
3935(be)X
555 5808(started)N
789(as)X
876(a)X
932(login)X
1116(xterm)X
1323(\(the)X
1468(-L)X
1564(option\),)X
1835(it)X
1899(must)X
2074(be)X
2170(setid)X
2341(root)X
2490(or)X
2577(started)X
2811(by)X
2911(a)X
2967(root)X
3116(process,)X
3397(such)X
3564(as)X
3651(/etc/init.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

6 p
%%Page: 6 6
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(6)X
2323(-)X
1 f
555 672(If)N
629(everything)X
992(is)X
1065(working,)X
1372(you)X
1512(should)X
1745(be)X
1841(ready)X
2040(to)X
2122(enable)X
2352(the)X
2470(line)X
2610(in)X
2692(/etc/ttys)X
2965(for)X
3079(login.)X
3 f
555 864(2.9.)N
715(Automatic)X
1096(Login)X
1315(Support)X
1 f
555 988(Some)N
761(systems)X
1038(are)X
1161(capable)X
1431(of)X
1522(starting)X
1786(the)X
1908(server)X
2129(and)X
2269(a)X
2329(login)X
2 f
2516(xterm)X
1 f
2722(automatically,)X
3201(in)X
3286(particular)X
3617(4.3BSD)X
3895(and)X
555 1084(Ultrix)N
769(1.2)X
892(and)X
1031(later.)X
1237(If)X
1314(your)X
1484(system)X
1729(does)X
1899(not)X
2024(support)X
2287(the)X
2408(new)X
2564(/etc/ttys)X
2839(format,)X
3095(you)X
3237(should)X
3472(skip)X
3627(this)X
3764(section.)X
555 1180(On)N
687(other)X
886(systems,)X
1193(if)X
1275(you)X
1428(have)X
1613(source)X
1856(for)X
1983(4.3BSD)X
2271(you)X
2424(may)X
2595(want)X
2784(to)X
2879(install)X
3107(this)X
3255(support.)X
3568(Mere)X
3771(mortals)X
555 1276(should)N
805(probably)X
1127(think)X
1328(long)X
1507(and)X
1660(hard)X
1839(before)X
2081(risking)X
2339(such)X
2522(installation,)X
2933(but)X
3071(wizards)X
3357(may)X
3531(\256nd)X
3691(it)X
3771(not)X
3909(too)X
555 1372(dif\256cult.)N
887(Affected)X
1208(programs)X
1550(include)X
2 f
1825(/etc/init)X
1 f
2069(,)X
2 f
2128(/etc/getty)X
1 f
2422(,)X
2 f
2481(/bin/login)X
1 f
2791(,)X
2850(and)X
3004(C)X
3095(library)X
3347(routines)X
2 f
3643(ttyname)X
1 f
3897(\(3\),)X
2 f
555 1468(isatty)N
1 f
728(\(3\),)X
2 f
864(ttyslot)X
1 f
1059(\(3\))X
1175(and)X
1313(all)X
1415(programs)X
1740(that)X
1882(depend)X
2136(on)X
2238(the)X
2358(format)X
2594(of)X
2 f
2683(/etc/ttys)X
1 f
2932(,)X
2 f
2973(/etc/gettytab)X
1 f
3390(and)X
2 f
3527(/etc/utmp)X
1 f
3825(.)X
3886(The)X
555 1564(programs)N
878(need)X
1050(to)X
1132(be)X
1228(installed)X
1519(as)X
1606(a)X
1662(set.)X
555 1688(To)N
669(avoid)X
872(a)X
933(possible)X
1220(race)X
1380(condition,)X
1727(and)X
1868(to)X
1955(allow)X
2158(consistent)X
2503(Unix)X
2688(program)X
2985(behavior,)X
3311(we)X
3430(dedicate)X
3723(a)X
3784(pseudo)X
555 1784(teletype)N
834(for)X
953(each)X
1126(display's)X
1440(login)X
1629(window.)X
1952(All)X
2079(other)X
2269(pty's)X
2454(are)X
2578(allocated)X
2893(dynamically)X
3314(when)X
3513(used.)X
3725(You)X
3887(will)X
555 1880(use)N
685(many)X
886(pty's,)X
1089(so)X
1183(make)X
1380(as)X
1470(many)X
1671(as)X
1761(possible.)X
2086(Pseudo)X
2340(TTY's)X
2577(come)X
2774(in)X
2859(pairs,)X
3058(the)X
3178(master)X
3414(and)X
3552(the)X
3672(slave.)X
3899(We)X
555 1976(rename)N
812(them)X
992(to)X
1074(be)X
1170(``ttyv?'')X
1458(where)X
1675(`?')X
1785(is)X
1858(the)X
1976(number)X
2241(of)X
2328(the)X
2446(display.)X
555 2100(So)N
659(for)X
773(example,)X
1085(we)X
1199(might)X
1405(execute)X
1671(the)X
1789(commands:)X
843 2244(#)N
903(cd)X
999(/dev)X
843 2340(#)N
903(MAKEDEV)X
1324(pty0)X
843 2436(#)N
903(MAKEDEV)X
1324(pty1)X
843 2532(#)N
903(mv)X
1025(ttyqf)X
1196(ttyv0)X
1643(#)X
1703(may)X
1861(already)X
2118(exist)X
2289(on)X
2389(some)X
2578(machines)X
843 2628(#)N
903(mv)X
1025(ptyqf)X
1214(ptyv0)X
555 2772(and)N
693(similarly)X
999(for)X
1115(any)X
1253(other)X
1440(displays.)X
1764(When)X
1977(logged)X
2216(in,)X
2319(you)X
2460(would)X
2681(appear)X
2917(to)X
3000(be)X
3097(logged)X
3336(in)X
3419(on)X
3520(``ttyv0'')X
3813(in)X
3896(this)X
555 2868(case.)N
764(We)X
906(use)X
1043(the)X
1171(last)X
1312(pseudo)X
1569(teletypes)X
1884(since)X
2078(all)X
2187(other)X
2381(utilities)X
2649(start)X
2816(searching)X
3153(from)X
3338(lower)X
3550(to)X
3641(higher,)X
3895(and)X
555 2964(we'd)N
736(just)X
871(as)X
958(soon)X
1129(have)X
1301(them)X
1481(\256nd)X
1625(a)X
1681(pty)X
1803(as)X
1890(soon)X
2061(as)X
2148(they)X
2306(can.)X
3 f
555 3156(2.10.)N
755(Con\256guring)X
1187(Lines)X
1393(in)X
1479(/etc/ttys)X
1 f
555 3280(If)N
643(you)X
797(started)X
1045(X)X
1137(in)X
1233(the)X
1364(previous)X
1673(step,)X
1855(you)X
2008(will)X
2165(want)X
2354(to)X
2449(abort)X
2647(it)X
2724(now.)X
2935(For)X
3079(each)X
3260(display)X
3524(you)X
3677(have)X
3862(on)X
3975(a)X
555 3376(machine,)N
868(you)X
1009(must)X
1185(have)X
1358(a)X
1415(line)X
1556(in)X
2 f
1639(/etc/ttys)X
1 f
1908(to)X
1990(drive)X
2175(the)X
2293(terminal)X
2580(emulator)X
2885(used)X
3052(for)X
3166(login)X
3350(and)X
3486(the)X
3604(window)X
3882(sys-)X
555 3472(tem)N
698(server.)X
958(NOTE:)X
1216(The)X
1363(format)X
1599(of)X
1688(the)X
2 f
1808(/etc/ttys)X
1 f
2079(\256le)X
2203(has)X
2332(changed)X
2622(radically)X
2925(between)X
3215(4.2)X
3337(and)X
3475(4.3.)X
3637(You)X
3797(cannot)X
555 3568(set)N
672(up)X
780(a)X
844(display)X
1103(for)X
1225(login)X
1417(on)X
1525(a)X
1588(4.2BSD)X
1870(system)X
2119(without)X
2390(installing)X
2714(new)X
2875(versions)X
3169(of)X
2 f
3263(/etc/init)X
1 f
3507(,)X
2 f
3554(/etc/getty)X
1 f
3848(,)X
3895(and)X
2 f
555 3664(/bin/login)N
1 f
885(from)X
1061(4.3.)X
555 3788(An)N
677(example)X
973(line)X
1117(in)X
2 f
1203(/etc/ttys)X
1 f
1476(is)X
1553(given)X
1755(in)X
1841(the)X
2 f
1963(X\(8c\))X
1 f
2166(manual)X
2425(page)X
2600(\(though)X
2872(you)X
3015(will)X
3162(have)X
3337(to)X
3422(customize)X
3770(the)X
3891(line)X
555 3884(for)N
679(the)X
807(location)X
1095(and)X
1241(names)X
1476(of)X
1573(the)X
1701(executable)X
2074(programs\).)X
2473(An)X
2600(example)X
2901(for)X
3024(a)X
3089(QVSS)X
3322(installed)X
3622(without)X
3895(any)X
555 3980(recon\256guration)N
1065(on)X
1165(4.3BSD)X
1440(might)X
1646(be:)X
755 4172(ttyv0)N
939("/etc/xterm)X
1317(-L)X
1413(=80x64+0+0)X
1848(:0")X
1963(xterm)X
2170(off)X
2284(window="/etc/Xqvss)X
2978(:0")X
555 4364(You)N
713(can)X
845(customize)X
1190(these)X
1375(commands)X
1742(to)X
1824(taste.)X
555 4488(You)N
713(can)X
845(tell)X
2 f
967(init\(8\))X
1 f
1187(to)X
1269(re-read)X
1518(the)X
2 f
1636(/etc/ttys)X
1 f
1905(\256le)X
2027(by)X
2127(the)X
2245(command)X
2581(``kill)X
2761(-1)X
2848(1'')X
2962(as)X
3049(super)X
3243(user.)X
3 f
555 4680(3.)N
655(Contents)X
982(of)X
1069(this)X
1213(Release)X
1 f
555 4804(The)N
700(enclosed)X
1001(tape)X
1155(contains)X
1442(nineteen)X
1734(megabytes)X
2097(of)X
2184(source)X
2414(\256les)X
2567(and)X
2703(documentation)X
3199(including:)X
10 f
555 4928(g)N
1 f
610(A)X
695(sample)X
949(server)X
1173(with)X
1342(device-dependent)X
1936(layers)X
2154(\(DDX\))X
2408(for)X
2528(several)X
2782(displays)X
3070(and)X
3212(vendors,)X
3512(including)X
3840(IBM,)X
555 5024(DEC,)N
755(Apollo)X
997(and)X
1133(Sun)X
1277(workstations.)X
10 f
555 5120(g)N
1 f
608(Several)X
874(libraries)X
1162(including)X
1489(a)X
1550(toolkit)X
1783(and)X
1924(both)X
2091(C)X
2169(and)X
2310(Common-Lisp)X
2800(language)X
3114(bindings)X
3413(for)X
3531(the)X
3653(X11)X
3815(proto-)X
555 5216(col.)N
10 f
555 5312(g)N
1 f
603(Some)X
805(bitmaps)X
1078(in)X
1160(include/bitmaps)X
1691(for)X
1805(use)X
1932(with)X
2094(xsetroot,)X
2392(et.)X
2490(al.)X
10 f
555 5408(g)N
1 f
607(Several)X
872(clients,)X
1125(including)X
1451(a)X
1511(terminal)X
1801(emulator)X
2109(and)X
2248(a)X
2307(toolkit-based)X
2748(client)X
2949(for)X
3066(browsing)X
3387(mh-style)X
3690(mail)X
3855(fold-)X
555 5504(ers.)N
10 f
555 5600(g)N
1 f
603(Some)X
805(demo)X
1003(clients)X
10 f
555 5696(g)N
1 f
603(Some)X
805(random)X
1070(utilities)X
1329(to)X
1411(help)X
1569(you)X
1709(build)X
1893(this)X
2028(release)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

7 p
%%Page: 7 7
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(7)X
2323(-)X
1 f
555 672(There)N
774(are)X
904(many)X
1113(aspects)X
1376(to)X
1469(the)X
1598(software)X
1906(in)X
1999(this)X
2145(release,)X
2420(so)X
2522(in)X
2615(the)X
2744(following)X
3086(we)X
3211(provide)X
3487(a)X
3554(perusal)X
3816(of)X
3913(the)X
555 768(release)N
800(software)X
1098(by)X
1199(directory)X
1510(name,)X
1725(calling)X
1964(attention)X
2265(to)X
2348(salient)X
2578(features)X
2854(or)X
2942(notable)X
3199(bugs.)X
3411(To)X
3521(be)X
3617(brief,)X
3809(not)X
3931(all)X
555 864(directories)N
919(are)X
1043(present)X
1300(in)X
1387(this)X
1527(discussion.)X
1925(If)X
2004(you)X
2149(\256nd)X
2298(problems)X
2621(not)X
2748(mentioned)X
3111(here,)X
3294(they)X
3456(should)X
3693(be)X
3793(mailed)X
555 960(to)N
637(xbugs@athena.mit.edu)X
1394(using)X
1587(the)X
1705(template)X
2001(bug)X
2141(report)X
2353(found)X
2560(in)X
2642(doc/bugs.)X
555 1084(clients/bitmap)N
755 1180(This)N
918(is)X
992(a)X
1049(very)X
1213(nice)X
1368(program)X
1661(for)X
1776(editing)X
2019(small)X
2212(bitmaps)X
2485(such)X
2652(as)X
2739(cursors)X
2991(or)X
3078(icons.)X
3307(However,)X
3642(it)X
3706(comes)X
3931(up)X
755 1276(a)N
831(silly)X
1008(size)X
1173(under)X
1396(the)X
1534("wm")X
1760(window)X
2058(manager,)X
2395(and)X
2550(puts)X
2722(dialog)X
2961(box)X
3120(in)X
3221(wrong)X
3465(place.)X
3714(With)X
3913(the)X
755 1372("woman")N
1087(bitmap,)X
1359(the)X
1487(small)X
1690(images)X
1947(are)X
2075(corrupt)X
2336(on)X
2445(color)X
2639(screens)X
2905(\(server)X
3158(bug)X
3307(likely\).)X
3585(The)X
3739(program)X
755 1468(dumps)N
988(core)X
1147(when)X
1341(colors)X
1557(are)X
1676(speci\256ed)X
1981(in)X
2063(.Xdefaults.)X
555 1592(clients/emacs)N
755 1688(All)N
880(that)X
1023(is)X
1099(provided)X
1407(here)X
1569(are)X
1691(the)X
1812(\256les)X
1968(that)X
2111(needed)X
2362(to)X
2446(change)X
2696(in)X
2780(gnuemacs)X
3123(so)X
3216(that)X
3358(it)X
3424(could)X
3624(use)X
3753(the)X
3873(X11)X
755 1784(window)N
1044(system.)X
1337(You)X
1506(must)X
1692(obtain)X
1923(the)X
2052(complete)X
2377(source)X
2618(for)X
2743(this)X
2889(separately.)X
3286(Consult)X
3564(the)X
3692(\256les)X
3855(DIS-)X
755 1880(TRIB)N
968(and)X
1115(README)X
1484(in)X
1576(this)X
1721(directory)X
2041(for)X
2165(more)X
2360(information.)X
2808(This)X
2980(implementation)X
3512(is)X
3595(known)X
3843(to)X
3935(be)X
755 1976(slow)N
949(and)X
1108(to)X
1213(have)X
1408(screen-updating)X
1964(problems,)X
2325(especially)X
2689(when)X
2906(doing)X
3131(incremental)X
3553(searches)X
3869(with)X
755 2072(control-S.)N
555 2196(clients/gnuplot)N
755 2292(This)N
921(is)X
998(a)X
1058(plotting)X
1330(program)X
1626(converted)X
1967(by)X
2071(an)X
2171(undergraduate)X
2655(summer)X
2937(student)X
3192(at)X
3274(MIT)X
3445(and)X
3585(he)X
3684(was)X
3832(never)X
755 2388(able)N
912(to)X
997(test)X
1131(it)X
1198(on)X
1301(anything)X
1604(but)X
1729(a)X
1788(microvax.)X
2153(The)X
2300(display)X
2553(window)X
2833(is)X
2908(totally)X
3134(white)X
3334(on)X
3436(Sun,)X
3602(for)X
3718(both)X
3882(BW)X
755 2484(and)N
891(color.)X
1116(The)X
1261(program)X
1553(fails)X
1711(to)X
1793(link)X
1937(on)X
2037(Ultrix)X
2248(2.0)X
2368(because)X
2643(it)X
2707(is)X
2780(missing)X
3048(-lgamma)X
3353(on)X
3453(the)X
3571(load)X
3729(line.)X
555 2608(clients/puzzle)N
755 2704(A)N
840(very)X
1010(simple)X
1250(program)X
1549(that)X
1696(animates)X
2008(a)X
2071(game)X
2272(you)X
2419(probably)X
2731(carried)X
2982(in)X
3071(your)X
3245(pocket)X
3486(as)X
3580(a)X
3643(child.)X
3869(This)X
755 2800(really)N
970(belongs)X
1251(in)X
1345(the)X
1475(hacks)X
1690(directory,)X
2031(but)X
2164(we)X
2289(noticed)X
2556(it)X
2631(too)X
2764(late.)X
2951(It)X
3031(doesn't)X
3298(center)X
3526(text)X
3677(in)X
3770(squares)X
755 2896(correctly)N
1066(when)X
1265(its)X
1365(window)X
1648(is)X
1726(created)X
1984(with)X
2151(the)X
2274(default)X
2521(height)X
2745(and)X
2885(width.)X
3131(The)X
3280(window)X
3562(never)X
3765(appears)X
755 2992(when)N
949(run)X
1076(under)X
1279(wm.)X
555 3116(clients/uwm)N
755 3212(This)N
935(is)X
1026(the)X
1162(most)X
1355(mature)X
1616(window)X
1911(manager)X
2225(available)X
2552(on)X
2669(the)X
2804(release.)X
3105(Resizing)X
3422(with)X
3601(this)X
3753(window)X
755 3308(manager)N
1066(causes)X
1310(double)X
1562(exposures)X
1917(because)X
2206(of)X
2307(the)X
2439(tiny)X
2597(XxY)X
2786(display)X
3050(box)X
3203(placed)X
3446(over)X
3622(the)X
3753(window)X
755 3404(being)N
956(resized)X
1207(and)X
1346(because)X
1624(of)X
1714(lack)X
1871(of)X
1961(save-unders.)X
2408(The)X
2555(server)X
2774(should)X
3009(do)X
3111(save-unders)X
3517(\(the)X
3664(saving)X
3895(and)X
755 3500(restoring)N
1061(the)X
1180(contents)X
1468(of)X
1556(the)X
1675(screen)X
1902(under)X
2106(a)X
2163(very)X
2327(short-lived)X
2695(window\),)X
3021(but)X
3144(in)X
3227(the)X
3346(interim,)X
3618(uwm)X
3798(should)X
755 3596(either)N
961(place)X
1154(the)X
1275(display)X
1529(box)X
1672(outside)X
1926(the)X
2047(bounds)X
2300(of)X
2389(the)X
2509(resized)X
2759(window,)X
3059(or)X
3148(perhaps)X
3420(unmap)X
3660(the)X
3780(display)X
755 3692(box)N
897(after)X
1067(resize,)X
1297(so)X
1390(that)X
1532(least)X
1701(both)X
1865(repaints)X
2141(are)X
2262(at)X
2342(the)X
2461(new)X
2616(size.)X
2802(The)X
2948(program)X
3241(doesn't)X
3498(deal)X
3653(with)X
3816(multi-)X
755 3788(ple)N
873(screens.)X
555 3912(clients/xcalc)N
755 4008(This)N
918(is)X
992(a)X
1049(clever)X
1267(tool)X
1412(that)X
1553(implements)X
1947(a)X
2004(hand-held)X
2345(calculator)X
2682(or)X
2769(a)X
2825(slide-rule)X
3148(on)X
3248(the)X
3366(screen.)X
3632(Unfortunat-)X
755 4104(ley,)N
894(it)X
959(dies)X
1109(with)X
1272(a)X
1329(parameter)X
1672(mismatch)X
2004(on)X
2105(ChangeWindowAttributes)X
2967(when)X
3162(the)X
3281(cursor)X
3502(leaves)X
3723(the)X
3841(xcalc)X
755 4200(window)N
1041(on)X
1148(color)X
1340(screen,)X
1593(and)X
1736(it)X
1807(dies)X
1963(immediately)X
2390(in)X
2479(-analog)X
2747(mode)X
2952(on)X
3059(a)X
3122(color)X
3314(screen.)X
3587(The)X
3739(program)X
755 4296(dumps)N
992(core)X
1155(when)X
1353(run)X
1484(without)X
1751(a)X
1810(window)X
2091(manager)X
2391(in)X
2476(-analog)X
2740(mode,)X
2961(and)X
3100(has)X
3230(a)X
3289(strange)X
3544(initial)X
3753(window)X
755 4392(size)N
900(when)X
1094(started)X
1328(under)X
1531(wm.)X
555 4516(clients/xclock)N
755 4612(This)N
931(displays)X
1227(digital)X
1465(or)X
1566(analog)X
1813(time.)X
2028(The)X
2186("-update)X
2493(N")X
2617(option)X
2854(has)X
2994(an)X
3103(old)X
3238(bug)X
3391(of)X
3491(not)X
3626(keeping)X
3913(the)X
755 4708(second)N
1010(hand)X
1198(pointing)X
1495(to)X
1588(a)X
1655(multiple)X
1952(of)X
2050(N)X
2139(seconds.)X
2464(It)X
2544(also)X
2704(doesn't)X
2971(center)X
3199(the)X
3328(time)X
3501(string)X
3714(in)X
3807(digital)X
755 4804(mode.)N
555 4928(clients/xedit)N
755 5024(This)N
920(is)X
995(a)X
1053(nice)X
1209(attempt)X
1471(at)X
1551(implementing)X
2017(an)X
2115(editor)X
2324(under)X
2529(X11)X
2689(using)X
2884(the)X
3004(toolkit.)X
3274(But)X
3411(it)X
3477(dumps)X
3712(core)X
3873(on)X
3975(a)X
755 5120(black-and-white)N
1312(Sun.)X
1511(It)X
1595(gets)X
1759(parameter)X
2116(mismatch)X
2462(for)X
2591(CreateGC)X
2947(when)X
3156(run)X
3298(on)X
3413(a)X
3483(color)X
3682(screen.)X
3962(It)X
755 5216(makes)N
989(the)X
1116(assumption)X
1509(that)X
1658(background=white.)X
2329(Resizing)X
2638(this)X
2781(client)X
2987(sometimes)X
3357(seems)X
3581(to)X
3671(do)X
3779(strange)X
755 5312(things.)N
555 5436(clients/xfd)N
755 5532(This)N
920(program)X
1215(will)X
1362(display)X
1616(the)X
1737(X11)X
1898(fonts.)X
2121(Pressing)X
2415(the)X
2536(right)X
2710(button)X
2937(should)X
3173(page)X
3347(through)X
3618(the)X
3738(font,)X
3909(but)X
755 5628(continues)N
1095(to)X
1190(wrap)X
1384(around)X
1640(and)X
1789(continue)X
2098(on)X
2211(forever.)X
2517(The)X
2675(-icon)X
2873(option)X
3110(doesn't)X
3379(seem)X
3577(to)X
3672(work)X
3869(with)X
755 5724(uwm.)N
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

8 p
%%Page: 8 8
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(8)X
2323(-)X
1 f
555 672(clients/xhost)N
755 768(This)N
920(provides)X
1219(a)X
1278(way)X
1435(to)X
1520(allow)X
1721(or)X
1811(deny)X
1990(access)X
2219(to)X
2304(speci\256c)X
2572(hosts.)X
2799(Regretably,)X
3194(it)X
3261(does)X
3431(not)X
3556(allow)X
3757(the)X
3877(user)X
755 864(to)N
837(enable/disable)X
1316(access)X
1542(control.)X
555 988(clients/xload)N
755 1084(This)N
919(client)X
1119(displays)X
1403(the)X
1523("load)X
1716(average")X
2022(in)X
2106(a)X
2164(histogram)X
2506(plot,)X
2672(but)X
2796(it)X
2862(may)X
3022(need)X
3196(to)X
3280(be)X
3378(set-userid)X
3711(to)X
3794(root)X
3944(or)X
755 1180(set-groupid)N
1147(to)X
1236(kmem)X
1463(if)X
1539(the)X
1664(permissions)X
2073(to)X
2161(/dev/kmem)X
2547(don't)X
2742(allow)X
2946(normal)X
3199(users)X
3390(to)X
3478(read)X
3643(it.)X
3753(In)X
3846(addi-)X
755 1276(tion,)N
919(the)X
1037(toolkit)X
1265(widget)X
1503(may)X
1661(need)X
1833(to)X
1915(be)X
2011(reimplemented)X
2512(for)X
2626(your)X
2793(kernel.)X
555 1400(clients/xlsfonts)N
755 1496(This)N
925(client)X
1131(will)X
1283(list)X
1408(the)X
1534(fonts)X
1722(known)X
1968(to)X
2058(the)X
2184(server,)X
2429(but)X
2559(the)X
2685(undocumented)X
3185(-l)X
3262(option)X
3494(\(long)X
3691(listing)X
3917(ala)X
755 1592(QueryFontsWithInfo\))N
1478(brings)X
1701(out)X
1826(a)X
1885(shortcoming)X
2308(in)X
2393(the)X
2514(sample)X
2764(server:)X
3006(it)X
3073(will)X
3220(freeze)X
3441(your)X
3611(display)X
3865(until)X
755 1688(the)N
873(request)X
1125(is)X
1198(complete,)X
1532(which)X
1748(may)X
1906(take)X
2060(several)X
2308(seconds.)X
555 1812(clients/xmh)N
755 1908(This)N
927(is)X
1010(potentially)X
1382(a)X
1448(very)X
1621(nice)X
1785(interface)X
2097(to)X
2189(the)X
2317(mh-style)X
2627(tools)X
2812(used)X
2989(to)X
3081(read)X
3250(one's)X
3454(mail.)X
3665(But)X
3809(it)X
3882(gets)X
755 2004(parameter)N
1097(mismatch)X
1428(on)X
1528(CreateGC)X
1869(on)X
1969(a)X
2025(color)X
2210(screen.)X
2476(It,)X
2565(too,)X
2707(has)X
2834(the)X
2952(background=white)X
3574(assumption.)X
555 2128(clients/xmodmap)N
755 2224(This)N
921(client)X
1122(will)X
1269(allow)X
1470(you)X
1613(to)X
1698(change)X
1949(the)X
2070(modi\256er)X
2364(mappings)X
2698(of)X
2788(your)X
2958(keyboard.)X
3320(Sometimes,)X
3718(additions)X
755 2320(will)N
899(fail)X
1026(because)X
1301(it)X
1365(doesn't)X
1621(know)X
1819(how)X
1977(to)X
2059("stretch")X
2359(the)X
2477(modi\256er)X
2768(map,)X
2946(but)X
3068(success)X
3329(is)X
3402(still)X
3541(indicated.)X
555 2444(clients/xpr)N
755 2540(This)N
924(program)X
1223(prints)X
1432(out)X
1561(a)X
1624(screendump)X
2038(created)X
2297(with)X
2465(xwd.)X
2669(It)X
2744(seems)X
2966(to)X
3054(have)X
3232(VAX)X
3432(byte)X
3596(order)X
3792(depen-)X
755 2636(dencies)N
1016(for)X
1130(-dev)X
1293(ps)X
1384(\(postscript)X
1742(output\).)X
2033(Postscript)X
2368(output)X
2592(generates)X
2916(printer)X
3150(errors)X
3358(sometimes.)X
555 2760(clients/xprop)N
755 2856(It)N
827(displays)X
1112(properties)X
1456(of)X
1546(windows)X
1858(and)X
1997(fonts,)X
2200(but)X
2325(it)X
2392(should)X
2627(have)X
2801(a)X
2859(way)X
3015(to)X
3099(get)X
3219(at)X
3299(subwindows.)X
3761(As)X
3872(is,)X
3967(it)X
755 2952(is)N
828(useless)X
1075(under)X
1278(wm)X
1418(because)X
1693(it)X
1757(will)X
1901(only)X
2063(give)X
2221(information)X
2619(about)X
2817(the)X
2935("frame")X
3209(window.)X
555 3076(clients/xrdb)N
755 3172(This)N
921(client)X
1123(will)X
1271(take)X
1429(an)X
1529(Xdefaults)X
1865(\256le)X
1991(and)X
2131(load)X
2293(it)X
2361(into)X
2509(the)X
2631(server)X
2852(as)X
2943(a)X
3003(property)X
3299(for)X
3417(use)X
3547(by)X
3650(clients)X
3882(glo-)X
755 3268(bally.)N
555 3392(clients/xsetroot)N
755 3488(Among)N
1015(other)X
1200(things,)X
1435(try)X
1544(the)X
1662(command)X
1998("xsetroot)X
2309(-bitmap)X
2578(include/bitmaps/woman".)X
555 3612(clients/xterm)N
755 3708(This)N
929(is)X
1014(the)X
1144(one)X
1292(and)X
1440(only)X
1614(terminal)X
1913(emulator)X
2230(for)X
2356(X11)X
2526(available)X
2848(publicly.)X
3182(If)X
3267(it)X
3342(is)X
3426(resized)X
3685(by)X
3796(wm,)X
3967(it)X
755 3804(paints)N
981(twice;)X
1212(once)X
1399(at)X
1492(the)X
1625(old)X
1762(size)X
1922(and)X
2073(once)X
2260(at)X
2353(the)X
2486(new.)X
2695(Selecting)X
3028(reverse)X
3296(video)X
3509(from)X
3700(the)X
3833(menu)X
755 3900(doesn't)N
1011(invert)X
1218(scroll)X
1416(bar.)X
1579(The)X
1724(-ms)X
1864(and)X
2000(-cr)X
2110(options)X
2365(don't)X
2554(work.)X
555 4024(clients/xwd)N
755 4120(Dumping)N
1080(a)X
1139(window)X
1420(or)X
1510(a)X
1569(portion)X
1822(thereof)X
2072(is)X
2147(the)X
2267(purpose)X
2543(of)X
2632(this)X
2769(client.)X
3009(The)X
3156(-xy)X
3285(mode)X
3485(on)X
3587(color)X
3774(screens)X
755 4216(is)N
837(broken)X
1089(\(server)X
1342(bug)X
1491(likely\).)X
1769(It)X
1847(doesn't)X
2111(retain)X
2322(button)X
2554(release)X
2806(during)X
3043(grab,)X
3234(resulting)X
3542(in)X
3632(spurious)X
3931(up)X
755 4312(event)N
953(to)X
1039(the)X
1161(window)X
1443(you)X
1587(wanted)X
1843(dumped.)X
2165(If)X
2243(the)X
2365(output)X
2593(is)X
2670(desired)X
2925(in)X
3010(z)X
3069(format)X
3306(from)X
3485(a)X
3544(1-bit)X
3718(screen,)X
3967(it)X
755 4408(duplicates)N
1108(a)X
1172(top)X
1302(stripe)X
1508(of)X
1603(the)X
1729(selected)X
2016(window)X
2302(instead)X
2557(of)X
2652(dumping)X
2964(the)X
3090(window)X
3376(as)X
3470(seen)X
3640(\(server)X
3891(bug)X
755 4504(likely\).)N
1030(It)X
1105(should)X
1344(check)X
1558(for)X
1678(the)X
1802(selected)X
2087(window)X
2371(being)X
2575(on-screen.)X
2954(Large)X
3168(windows)X
3483(cause)X
3688(the)X
3811(Xqdss)X
755 4600(server)N
972(to)X
1054(dump)X
1256(core)X
1415(\(server)X
1659(bug\).)X
555 4724(clients/xwininfo)N
755 4820(Like)N
928(xprop,)X
1161(the)X
1285(program)X
1583(doesn't)X
1845(retain)X
2054(button)X
2284(release)X
2534(during)X
2769(grab,)X
2957(resulting)X
3262(in)X
3349(spurious)X
3645(up)X
3750(event)X
3949(to)X
755 4916(another)N
1016(window.)X
555 5040(clients/xwud)N
755 5136(This)N
923(client)X
1127(will)X
1277(dump)X
1485(to)X
1573(the)X
1696(screen,)X
1947(the)X
2070(\256le)X
2197(created)X
2455(by)X
2560(xwd,)X
2743(but)X
2870(a)X
2931(partial)X
3161(refresh)X
3410(of)X
3502(Z)X
3576(format)X
3815(image)X
755 5232(produces)N
1068(garbage)X
1346(\(library)X
1610(bug\).)X
1820(A)X
1901(1-bit)X
2075(image)X
2293(on)X
2395(N-bit)X
2586(screen)X
2814(is)X
2889(garbled)X
3152(\(server)X
3398(bug)X
3540(likely\).)X
3811(It)X
3882(dies)X
755 5328(in)N
837(strange)X
1089(ways)X
1274(on)X
1374(large)X
1555(color)X
1740(images.)X
555 5452(contrib/InterViews)N
755 5548(This)N
921(is)X
998(the)X
1120(only)X
1286(software)X
1587(found)X
1798(in)X
1884(the)X
2006("contributed)X
2428(software")X
2762(directory:)X
3098(i.e.)X
3239(untested)X
3529(software.)X
3869(This)X
755 5644(is)N
828(a)X
884(C++)X
1047(implementation)X
1569(of)X
1656(a)X
1712(toolkit)X
1940(and)X
2076(some)X
2265(clients.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

9 p
%%Page: 9 9
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2216 416(-)N
2263(9)X
2323(-)X
1 f
555 672(demos/ico,)N
924(muncher,)X
1245(plaid)X
755 768(These)N
973(are)X
1098(cute)X
1258(programs)X
1587(that)X
1732(do)X
1837(nothing)X
2106(but)X
2233(paint)X
2418(somewhat)X
2768(pretty)X
2980(pictures.)X
3299(They)X
3489(don't)X
3683(obey)X
3864(win-)X
755 864(dow)N
929(resizing)X
1219(or)X
1322(exposure)X
1648(correctly.)X
2010(The)X
2171("plaid")X
2433(program)X
2741(needs)X
2960(to)X
3057(throttle)X
3323(output)X
3562(to)X
3659(keep)X
3846(other)X
755 960(clients)N
984(happy,)X
1220(but)X
1342(this)X
1477(may)X
1635(suggest)X
1895(that)X
2035(the)X
2153(server)X
2370(isn't)X
2532(round-robining)X
3037(properly.)X
555 1084(demos/wm)N
755 1180(This)N
927(is)X
1010(the)X
1138("other")X
1399(window)X
1687(manager;)X
2016(it)X
2090(is)X
2173(pretty)X
2390(simple)X
2632(minded,)X
2921(but)X
3052(it)X
3125(implements)X
3527(title)X
3680(bars,)X
3863(real-)X
755 1276(estate)N
960(style)X
1133(window)X
1413(management)X
1845(and)X
1983(understands)X
2388(multiple)X
2676(screens.)X
2975(It)X
3045(seems)X
3262(to)X
3345(cause)X
3545(various)X
3802(clients)X
755 1372(grief,)N
949(particularly)X
1341(with)X
1505(respect)X
1755(to)X
1839(initial)X
2047(size.)X
2234(It)X
2305(should)X
2540(do)X
2642(something)X
2997(about)X
3196(borders)X
3458(so)X
3550(that)X
3691(they)X
3850(arent)X
755 1468(always)N
998(white,)X
1216(and)X
1352(when)X
1546(it)X
1610(closes)X
1826(down,)X
2044(it)X
2108(should)X
2341(restore)X
2580(rational)X
2845(borders.)X
555 1592(doc)N
755 1688(As)N
870(might)X
1082(be)X
1184(expected,)X
1516(this)X
1656(contains)X
1948(all)X
2053(the)X
2176(documentation.)X
2717(Any)X
2880(\256le)X
3007(in)X
3094(this)X
3234(directory)X
3549(named)X
3788(*.ps)X
3944(or)X
755 1784(*.psc)N
946(can)X
1082(be)X
1182(handed)X
1438(directly)X
1707(to)X
1793(a)X
1853(postscript)X
2187(printer,)X
2444(as)X
2534(it)X
2601(is)X
2677(the)X
2798(postscript)X
3132(output)X
3359(from)X
3538(programs)X
3864(such)X
755 1880(as)N
842(troff.)X
555 2004(doc/Protocol)N
755 2100(Here)N
944(you)X
1096(will)X
1252(\256nd)X
1408(the)X
1538(formal)X
1784(description)X
2172(of)X
2271(the)X
2401(protocol,)X
2720(the)X
2850(numeric)X
3145(encoding)X
3471(of)X
3570(same)X
3766(and)X
3913(the)X
755 2196(keysym)N
1024(description.)X
555 2320(doc/Server)N
755 2416(There)N
969(are)X
1094(several)X
1348(documents)X
1721(in)X
1809(this)X
1950(directory)X
2266(dealing)X
2528(with)X
2696(the)X
2820(implementation)X
3347(of)X
3439(the)X
3562(sample)X
3814(server)X
755 2512(and)N
891(helpful)X
1138(hints)X
1313(for)X
1427(porting.)X
555 2636(doc/Xlib)N
755 2732(The)N
905(C)X
983(language)X
1297(X)X
1379(interface)X
1685(is)X
1762(documented)X
2178(here.)X
2381(The)X
2530(printed)X
2781(document)X
3121(is)X
3198(just)X
3337(under)X
3544(300)X
3688(pages)X
3895(and)X
755 2828(is)N
828(quite)X
1008(complete.)X
555 2952(doc/Xlib/Xman)N
755 3048(The)N
902(section)X
1151(3)X
1213(man)X
1373(pages)X
1578(can)X
1712(be)X
1810(found)X
2018(here.)X
2218(If)X
2293(you)X
2434(can)X
2567(tolerate)X
2829(\256le)X
2952(names)X
3178(longer)X
3404(than)X
3563(14)X
3664(characters,)X
755 3144(you)N
901(should)X
1140(consider)X
1438(running)X
1713(the)X
1836(\(largely)X
2111(untested\))X
2430(shell)X
2606(script)X
2809(named)X
3048("expand.names")X
3596(in)X
3683(this)X
3823(direc-)X
755 3240(tory)N
905(that)X
1046(will)X
1191(change)X
1439(the)X
1557(pre\256x)X
1764(of)X
1851(the)X
1969(.3X)X
2107(\256les)X
2260(to)X
2342(names)X
2567(matching)X
2885(the)X
3003(names)X
3228(of)X
3315(the)X
3433(Xlib)X
3595(routines)X
3873(they)X
755 3336(describe.)N
555 3460(doc/bugs)N
755 3556(A)N
835(bug)X
976(report)X
1189(template)X
1486(appears)X
1753(in)X
1836(this)X
1972(directory.)X
2323(Bug)X
2477(reports)X
2721(should)X
2955(be)X
3052(submitted)X
3388(using)X
3582(this)X
3718(form)X
3895(and)X
755 3652(should)N
988(be)X
1084(mailed)X
1322(to)X
1404(xbugs@athena.mit.edu.)X
555 3776(doc/releasenotes)N
755 3872(You)N
913(will)X
1057(\256nd)X
1201(the)X
1319(troff)X
1482(source)X
1712(for)X
1826(this)X
1961(document)X
2297(here.)X
555 3996(extensions)N
755 4092(This)N
918(directory)X
1229(contains)X
1517(a)X
1574(lib,)X
1699(server,)X
1937(test)X
2069(and)X
2206(include)X
2463(directory)X
2774(for)X
2889(two)X
3029(extensions:)X
3409(a)X
3465(useless)X
3712(trapezoid)X
755 4188(\256lling)N
980(extensioin)X
1343(called)X
1569("zoid")X
1807(and)X
1957(a)X
2027(rather)X
2249(interesting)X
2621(one)X
2771(for)X
2899(implementing)X
3377(bezier)X
3608(curve)X
3821(\256tting)X
755 4284(called)N
967("bezier".)X
1290(The)X
1435(beziertest)X
1763(program)X
2055(repaints)X
2329(on)X
2429(the)X
2547(wrong)X
2772(kind)X
2934(of)X
3021(events.)X
555 4408(fonts/bdf)N
755 4504(The)N
902(ascii)X
1070(representation)X
1546(of)X
1634(the)X
1753(fonts)X
1934(live)X
2075(in)X
2158(this)X
2294(directory.)X
2645(You)X
2804(should)X
3038(try)X
3148(to)X
3231(remember)X
3578(that)X
3719(the)X
3838("bdf")X
755 4600(stands)N
979(for)X
1097(Bitmap)X
1356(Distribution)X
1766(Format.)X
2061(This)X
2227(format)X
2465(is)X
2542(an)X
2642(extension)X
2973(of)X
3064(Adobe,)X
3322(Inc's)X
3507(standard)X
3802(ASCII)X
755 4696(format)N
990(for)X
1105(font)X
1255(distribution.)X
1684(It)X
1754(should)X
1988(be)X
2085(noted)X
2284(that)X
2425(you)X
2566(can)X
2698(purchase)X
3004(fonts)X
3184(from)X
3360(Adobe,)X
3614(inc.)X
3752(in)X
3834(a)X
3890(for-)X
755 4792(mat)N
909(close)X
1108(to)X
1204(this.)X
1393(When)X
1619(doing)X
1835(so,)X
1960(you)X
2114(should)X
2361(ask)X
2502(for)X
2630(both)X
2806(the)X
2938(bitmap)X
3194(and)X
3344(the)X
3476(font)X
3639(metric)X
3878(\256les)X
755 4888(\('afm'\),)N
1028(which)X
1244(are)X
1363(combined)X
1699(together)X
1982(for)X
2096(an)X
2192(X11)X
2350(.bdf)X
2497(\256le.)X
2639(Please)X
2864(do)X
2964(not)X
3086(contact)X
3338(MIT)X
3505(to)X
3587(buy)X
3727(fonts.)X
555 5012(fonts/snf)N
755 5108(This)N
925(directory)X
1243(holds)X
1444(the)X
1570(compiled)X
1896(fonts.)X
2124(The)X
2277("snf")X
2469(stands)X
2697(for)X
2819(Server)X
3057(Natural)X
3326(Format.)X
3625(snf)X
3751(\256les)X
3912(are)X
755 5204(not)N
877(portable)X
1160(between)X
1448(servers)X
1696(except)X
1926(in)X
2008(rare)X
2154(cases.)X
555 5328(include/bitmaps)N
755 5424(Bitmap)N
1016(\256les)X
1175(found)X
1388(in)X
1476(this)X
1617(directory)X
1933(have)X
2111(a)X
2173(different)X
2476(format)X
2716(in)X
2804(X11)X
2968(than)X
3132(they)X
3296(did)X
3424(in)X
3512(X10.)X
3716(The)X
3867("bit-)X
755 5520(map")N
955(program)X
1256(can)X
1397(read)X
1565(\256les)X
1727(in)X
1818(either)X
2029(format,)X
2291(but)X
2421(always)X
2672(writes)X
2896(out)X
3026(X11-format)X
3433(\256les.)X
3634(In)X
3729(addition,)X
755 5616(there)N
936(is)X
1009(a)X
1065(utility)X
1275(program)X
1567(called)X
1779("bm-convert")X
2235(which)X
2451(converts)X
2743(X10-format)X
3142(bitmap)X
3384(\256les)X
3537(to)X
3619(X11)X
3777(format.)X
755 5712(For)N
886(more)X
1071(information,)X
1489(see)X
1612(clients/bitmap/bitmap.man)X
2487(and)X
2623(util/bm-convert/bm-convert.doc.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

10 p
%%Page: 10 10
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2196 416(-)N
2243(10)X
2343(-)X
1 f
555 672(lib/CLX)N
755 768(This)N
923(contains)X
1215(the)X
1338(Common)X
1660(-)X
1712(Lisp)X
1879(language)X
2194(interface)X
2501(to)X
2588(the)X
2711(X11)X
2874(protocol)X
3166(and)X
3307(has)X
3439(been)X
3616(tested)X
3828(under)X
755 864(TI,)N
871(Symbolics,)X
1248(and)X
1384(Lucid.)X
555 988(lib/X)N
755 1084(Here)N
936(you)X
1080(can)X
1216(\256nd)X
1364(the)X
1486(source)X
1720(for)X
1837(the)X
1958(C)X
2034(-)X
2084(language)X
2397(interface)X
2702(to)X
2787(the)X
2908(X11)X
3069(protocol.)X
3399(The)X
3547(next)X
3708(section)X
3958(is)X
755 1180(devoted)N
1029(to)X
1111(its)X
1206(discussion.)X
555 1304(lib/Xrm)N
755 1400(This)N
929(is)X
1014(the)X
1144(directory)X
1466(containing)X
1836(the)X
1966(implementation)X
2500(of)X
2598(the)X
2727("new")X
2958(resource)X
3262(manager.)X
3610(None)X
3815(of)X
3913(the)X
755 1496(clients)N
987(use)X
1117(it)X
1184(and)X
1323(neither)X
1569(the)X
1690(old)X
1815(nor)X
1945(new)X
2102(toolkits)X
2364(use)X
2494(it.)X
2601(However,)X
2938(this)X
3075(will)X
3221(be)X
3319(the)X
3439(resource)X
3734(manager)X
755 1592(used)N
929(in)X
1018(future)X
1237(releases.)X
1558(As)X
1673(soon)X
1850(as)X
1943(the)X
2067(new)X
2227(toolkit)X
2461(is)X
2540(stable)X
2753(and)X
2895(has)X
3028(been)X
3206(converted)X
3549(to)X
3637(use)X
3770(this,)X
3931(all)X
755 1688(clients)N
984(will)X
1128(be)X
1224(converted)X
1561(as)X
1648(well.)X
1846(If)X
1920(you)X
2060(write)X
2245(a)X
2301(new)X
2455(client,)X
2673(it)X
2737(should)X
2970(use)X
3097(this)X
3232(resource)X
3525(manager.)X
755 1812(Note)N
936(that)X
1081(both)X
1248(the)X
1371(new)X
1530(and)X
1671(the)X
1794(old)X
1921(resource)X
2219(manager)X
2521(are)X
2645(case-sensitive)X
3116(now.)X
3319(You)X
3482(should)X
3720(keep)X
3896(this)X
755 1908(in)N
837(mind)X
1021(when)X
1215(you)X
1355(build)X
1539(your)X
1706(.Xdefaults)X
2058(\256le.)X
555 2032(lib/Xtk)N
755 2128(The)N
909(Xtk)X
1058(library)X
1301(\("new)X
1524(toolkit"\))X
1821(is)X
1902(still)X
2049(in)X
2139(a)X
2203(very)X
2374(un\256nished)X
2735(state.)X
2950(We've)X
3193(identi\256ed)X
3523(three)X
3712(bugs)X
3891(that)X
755 2224(cause)N
954(problems)X
1272(on)X
1372(the)X
1490(Sun,)X
1654(but)X
1776(not)X
1898(on)X
1998(the)X
2116(Vax:)X
955 2348(Resources.c)N
1368(fails)X
1533(to)X
1622(correctly)X
1935(handle)X
2175(resources)X
2505(whose)X
2736(size)X
2887(is)X
2966(1;)X
3054(in)X
3142(particular,)X
3496(it)X
3566(gets)X
3721(confused)X
955 2444(on)N
1076(Boolean)X
1384(resources)X
1729(such)X
1917(as)X
2025(the)X
2164("analog/digital")X
2711(option)X
2956(to)X
3059(the)X
3198(Clock)X
3430(widget.)X
3729(There)X
3958(is)X
955 2540(currently)N
1271(special)X
1520(case)X
1685(code,)X
1883(surrounded)X
2270(by)X
2376("#ifdef)X
2627(BIGENDIAN",)X
3151(to)X
3238(handle)X
3477(resources)X
3806(whose)X
955 2636(size)N
1122(is)X
1217(2)X
1299(\(=)X
1413(sizeof\(short\)\);)X
1910(this)X
2067(code)X
2261(probably)X
2588(needs)X
2813(to)X
2917(be)X
3035(extended)X
3367(to)X
3471(similarly)X
3797(handle)X
955 2732(resources)N
1279(whose)X
1504(size)X
1649(is)X
1722(1.)X
955 2856(Text.c)N
1195(incorrectly)X
1580(calls)X
1764(XtGetSubresources)X
2425(with)X
2604(a)X
2677(Display)X
2962(argument)X
3301(instead)X
3564(of)X
3667(the)X
3801(proper)X
955 2952(Widget)N
1211(argument.)X
1554(This)X
1716(core-dumps)X
2115(the)X
2233("xtext")X
2479(application)X
2855(instantly)X
3150(on)X
3250(a)X
3306(Sun.)X
955 3076(AsciiSink.c)N
1352(contains)X
1645(bitmap)X
1893(constants)X
2217(declared)X
2516(as)X
2609(arrays)X
2832(of)X
2925(shorts,)X
3162(rather)X
3375(than)X
3538(arrays)X
3760(of)X
3852(char,)X
955 3172(and)N
1099(assumes)X
1394(that)X
1542(the)X
1667(shorts)X
1885(are)X
2011(stored)X
2234(in)X
2323(LSBFirst)X
2642(\(little-endian\))X
3110(order.)X
3347(The)X
3499(result)X
3704(is)X
3784(that)X
3931(on)X
955 3268(big-endian)N
1318(machines)X
1641(such)X
1808(as)X
1895(the)X
2013(Sun,)X
2177(Text)X
2344(widgets)X
2613(don't)X
2802(display)X
3053(a)X
3109(caret)X
3286(at)X
3364(the)X
3482(insertion)X
3782(point.)X
755 3392(All)N
881(of)X
972(the)X
1094(widgets)X
1367(should)X
1604(be)X
1704(using)X
1901(XtCreateWindow)X
2490(instead)X
2740(of)X
2830(XCreateWindow,)X
3417(but)X
3542(currently)X
3855(none)X
755 3488(of)N
848(them)X
1034(do.)X
1180(The)X
1331(SetValues)X
1682(routines)X
1966(in)X
2054(most)X
2235(of)X
2328(the)X
2452(widgets)X
2726(are)X
2850(either)X
3058(incorrect)X
3369(or)X
3461(are)X
3585(complete)X
3904(no-)X
755 3584(ops.)N
930(None)X
1128(of)X
1219(the)X
1341(widgets')X
1641(SetValues)X
1990(routines)X
2272(have)X
2448(been)X
2624(exercised)X
2952(by)X
3055(any)X
3194(code)X
3369(yet,)X
3510(with)X
3675(the)X
3796(excep-)X
755 3680(tion)N
899(of)X
986(the)X
1104("top-level")X
1475(widget)X
1713(that)X
1853(is)X
1926(returned)X
2214(by)X
2314(XtInitialize.)X
755 3804(The)N
903(IBM)X
1077(hc)X
1176(compiler)X
1484(will)X
1631(not)X
1756(compile)X
2037(lib/Xtk)X
2286(directory,)X
2619(but)X
2744(the)X
2865(pcc)X
3000(compiler)X
3308(will.)X
3495(We)X
3629(suggest)X
3891(you)X
755 3900(build)N
939(this)X
1074(directory)X
1384(by)X
1484(hand)X
1660(with)X
1822(pcc)X
1954(before)X
2180(doing)X
2382(a)X
2438(full)X
2569(make.)X
555 4024(lib/Xtk/clients)N
755 4120(With)N
938(the)X
1059(exception)X
1394(of)X
1484("xboxes",)X
1820(all)X
1923(the)X
2044(sample)X
2293(clients)X
2524(in)X
2608(this)X
2745(subdirectory)X
3168(run)X
3297(more-or-less)X
3725(correctly)X
755 4216(on)N
869(a)X
939(Vax.)X
1147(However,)X
1496(various)X
1766(bugs)X
1951(become)X
2235(evident)X
2505(when)X
2713(you)X
2867(run)X
3008(them)X
3202(on)X
3316(a)X
3386(Sun.)X
3584(For)X
3728(instance,)X
755 4312("xtext")N
1013(immediately)X
1445(core-dumps,)X
1876(and)X
2024("xclock")X
2336(comes)X
2573(up)X
2685(in)X
2779(digital)X
3015(mode)X
3224(regardless)X
3581(of)X
3679(.Xdefaults)X
755 4408(options)N
1010(or)X
1097(command)X
1433(line)X
1573(arguments.)X
555 4532(lib/oldX)N
755 4628(This)N
919(library)X
1155(contains)X
1444(code)X
1618(that)X
1760(existed)X
2009(in)X
2093(X10)X
2252(such)X
2420(as)X
2508(the)X
2627("XAssocTable")X
3151(routines)X
3430(and)X
3567(an)X
3664(implemen-)X
755 4724(tation)N
962(of)X
1054(the)X
1177(X10)X
1340(XDraw)X
1602(and)X
1743(XDrawFilled)X
2191(requests)X
2479(that)X
2624(turn)X
2778(spline)X
2994(descriptions)X
3406(into)X
3555(X11)X
3718(PolyLine)X
755 4820(requests.)N
1091(You)X
1262(should)X
1508(not)X
1643(assume)X
1912(that)X
2065(these)X
2263(routines)X
2554(will)X
2711(continue)X
3019(to)X
3113(exist)X
3296(in)X
3390(this)X
3537(form)X
3725(in)X
3819(future)X
755 4916(releases.)N
555 5040(lib/oldXMenu)N
755 5136(This)N
919(is)X
994(a)X
1052 0.2813(deck-of-cards)AX
1517(menu)X
1717(facility)X
1966(that)X
2108(has)X
2237(simply)X
2476(been)X
2649(ported)X
2875(to)X
2958(X11)X
3117(and)X
3254(is)X
3328(known)X
3567(to)X
3650(run)X
3778(slowly.)X
755 5232(Eventually,)N
1142(this)X
1277(will)X
1421(be)X
1517(subsumed)X
1857(by)X
1957(a)X
2013(menu)X
2211(widget)X
2449(in)X
2531(the)X
2649(toolkit.)X
555 5356(lib/oldXrm)N
755 5452(This)N
925(is)X
1006(the)X
1132("old")X
1328(resource)X
1629(manager)X
1934(as)X
2029(it)X
2101(appeared)X
2420(on)X
2528(the)X
2654(Beta)X
2829(release.)X
3121(It)X
3198(will)X
3350(remain)X
3601(a)X
3665(part)X
3818(of)X
3913(the)X
755 5548(release)N
1016(only)X
1195(as)X
1299(long)X
1478(as)X
1582(there)X
1780(remains)X
2071(a)X
2144(client)X
2358(that)X
2514(has)X
2657(not)X
2795(been)X
2983(converted)X
3336(to)X
3434(the)X
3568(new)X
3738(resource)X
755 5644(manager.)N
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

11 p
%%Page: 11 11
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2196 416(-)N
2243(11)X
2343(-)X
1 f
555 672(lib/oldXtk)N
755 768(This)N
920(contains)X
1210(the)X
1331("old")X
1522(toolkit,)X
1773(with)X
1938(the)X
2059(old)X
2184(intrinsics)X
2500(interface.)X
2845(As)X
2957(soon)X
3131(as)X
3221(the)X
3342(new)X
3499(toolkit)X
3730(becomes)X
755 864(stable)N
969(all)X
1076(clients)X
1312(will)X
1463(be)X
1566(converted)X
1910(to)X
1999(it)X
2069(and)X
2211(this)X
2352(will)X
2502(not)X
2630(appear)X
2871(on)X
2977(future)X
3195(releases.)X
3516(Currently,)X
3869(only)X
755 960(xload,)N
973(xmh,)X
1155(xclock,)X
1409(xedit)X
1589(and)X
1725(xterm)X
1932(use)X
2059(this)X
2194(toolkit.)X
555 1084(server/ddx/apollo)N
755 1180(This)N
921(holds)X
1118(the)X
1240(Apollo)X
1486(device-dependent)X
2077(code)X
2253(\(ddx\))X
2450(for)X
2567(the)X
2688(sample)X
2938(server.)X
3198(It)X
3270(will)X
3417(run)X
3547(on)X
3650(any)X
3789(Apollo)X
755 1276(display,)N
1032(color)X
1223(or)X
1316(monochrome,)X
1785(but)X
1913(it)X
1983(will)X
2133(only)X
2301(do)X
2407(1-bit)X
2583(black-and-white)X
3130(graphics.)X
3467(You)X
3630(should)X
3868(con-)X
755 1372(sult)N
891(the)X
1010(README)X
1368(\256le)X
1490(in)X
1572(this)X
1707(directory)X
2017(before)X
2243(trying)X
2454(to)X
2536(build)X
2720(an)X
2816(Apollo)X
3058(server.)X
3315(The)X
3460(only)X
3622(problem)X
3909(not)X
755 1468(mentioned)N
1118(there)X
1304(is)X
1382(that)X
1526(the)X
1648(server)X
1869(builds)X
2088(with)X
2254(about)X
2456(a)X
2516(half-dozen)X
2884(unde\256ned)X
3224(variables,)X
3558(but)X
3684(since)X
3873(they)X
755 1564(are)N
874(never)X
1073 0.3625(referenced,)AX
1454(the)X
1572(server)X
1789(runs.)X
555 1688(server/ddx/cfb)N
755 1784(In)N
859(here,)X
1055(Sun)X
1216(Microsystems)X
1702(has)X
1845(placed)X
2091(a)X
2163("generic")X
2502(analogue)X
2828(of)X
2931("mfb")X
3162(for)X
3292(memory)X
3595(color)X
3796(frame-)X
755 1880(buffers.)N
1047(The)X
1196(code)X
1372(has)X
1503(been)X
1679(tested)X
1890(only)X
2056(on)X
2159(displays)X
2444(with)X
2609(a)X
2668(pixel)X
2851(size)X
2999(of)X
3089(8)X
3152(bits,)X
3310(both)X
3475(with)X
3640(all)X
3743(8)X
3806(planes)X
755 1976(implemented)N
1201(\(Sun\))X
1407(and)X
1551(less)X
1699(than)X
1865(8)X
1933(planes)X
2166(implemented)X
2612(\(HP\).)X
2836(As)X
2953(distributed,)X
3363(it)X
3435(is)X
3516(set)X
3632(up)X
3739(for)X
3860(8-bit)X
755 2072(pixels)N
978(with)X
1152(all)X
1264(8)X
1336(planes)X
1573(implemented.)X
2063(The)X
2220(code)X
2404(is)X
2489(intended)X
2797(to)X
2890(be)X
2997(highly)X
3232(portable,)X
3566(and)X
3713(to)X
3806(use)X
3944(as)X
755 2168(much)N
957(as)X
1048(possible)X
1334(of)X
1425(the)X
1547("mi")X
1721(code.)X
1937(As)X
2050(a)X
2110(result,)X
2352(it)X
2420(is)X
2497(very)X
2664(slow,)X
2858(but)X
2983(very)X
3149(portable.)X
3475(You)X
3636(should)X
3872(read)X
755 2264(the)N
873(README)X
1231(\256le)X
1353(in)X
1435(this)X
1570(directory)X
1880(before)X
2106(using)X
2299(it)X
2363(for)X
2477(a)X
2533(new)X
2687(port.)X
555 2388(server/ddx/ibm)N
755 2484(IBM)N
927(has)X
1055(provided)X
1361(us)X
1453(with)X
1616(ddx)X
1757(layers)X
1970(for)X
2085(IBM)X
2257(PC/RT)X
2499(with)X
2662(AED)X
2848(and/or)X
3073(APA16/APA19)X
3595(display.)X
3886(The)X
755 2580(hc)N
855(compiler)X
1164(available)X
1478(on)X
1582(this)X
1721(machine)X
2017(is)X
2094(known)X
2336(to)X
2421(have)X
2596(problems)X
2917(with)X
3082(*foo++)X
3342(=)X
3410(*bar++)X
3666(constructs.)X
755 2676(To)N
868(get)X
990(around)X
1237(this,)X
1395(you)X
1538(should)X
1774(run)X
1904(the)X
2025(command)X
2364("RTSETUP)X
2766(init")X
2928(in)X
3013(this)X
3151(directory)X
3464(before)X
3693(the)X
3814(server)X
755 2772(is)N
833(built.)X
1044(This)X
1210(will)X
1358(build)X
1546(patch,)X
1764(makedepend)X
2194(and)X
2334(imake,)X
2574(then)X
2736(it)X
2804(will)X
2952(patch)X
3150(sources)X
3415(for)X
3533(the)X
3655(sake)X
3822(of)X
3913(the)X
755 2868(RT)N
908(compiler)X
1244(\(miregion.c)X
1667(and)X
1833(../../../lib/Xtk/Imake\256le\).)X
2687(Finally,)X
2983(it)X
3077(will)X
3251(build)X
3465(and)X
3631(depend)X
3913(the)X
755 2964(make\256les.)N
1123(After)X
1314(this,)X
1470(a)X
1527(top-level)X
1833(make)X
2028(should)X
2262(complete)X
2577(without)X
2842(errors.)X
3091(Note)X
3268(that)X
3408(source)X
3638(\256les)X
3791(will)X
3935(be)X
755 3060(patched)N
1025(only)X
1187(if)X
1256(the)X
1374("init")X
1566(argument)X
1889(is)X
1962(speci\256ed)X
2267(on)X
2367(the)X
2485(command)X
2821(line.)X
555 3184(server/ddx/lk201)N
755 3280(This)N
917(is)X
990(the)X
1108(ddx)X
1248(driver)X
1460(code)X
1632(for)X
1746(the)X
1864(keyboard)X
2183(used)X
2350(by)X
2450(most)X
2625(DEC)X
2805(displays.)X
555 3404(server/ddx/mfb)N
755 3500(This)N
923(is)X
1002(the)X
1126("monochrome)X
1608 0.3229(frame-buffer")AX
2079(ddx)X
2225(code.)X
2443(If)X
2523(you)X
2669(are)X
2794(beginning)X
3139(a)X
3200(new)X
3359(port)X
3513(and)X
3654(plan)X
3817(to)X
3904(use)X
755 3596(this,)N
964(you)X
1158(should)X
1445(read)X
1658(the)X
1830(code)X
2056(very)X
2273(carefully)X
2633(in)X
2769(maskbits.h)X
3187(and)X
3376(the)X
3547(comments)X
3949(in)X
755 3692(../../include/servermd.h,)N
1576(and)X
1752(then)X
1950(choose)X
2233(appropriate)X
2658(constants)X
3015(for)X
3168(IMAGE_BYTE_ORDER,)X
755 3788(BITMAP_BIT_ORDER,)N
1577(GETLEFTBITS_ALIGNMENT)X
2634(and)X
2770(GLYPHPADBYTES.)X
555 3912(server/ddx/mi)N
755 4008(This)N
917(is)X
990(the)X
1108 0.1563("machine-independent")AX
1885(ddx)X
2025(layer.)X
555 4132(server/ddx/qdss)N
755 4228(The)N
900(pre-compiled)X
1348(libraries)X
1631(found)X
1838(here)X
1997(are)X
2116(are)X
2235(for)X
2349(DEC's)X
2587(GPX)X
2767(display.)X
3058(No)X
3176(source)X
3406(code)X
3578(is)X
3651(available.)X
555 4352(server/ddx/qvss)N
755 4448(This)N
917(holds)X
1110(the)X
1228(DEC)X
1408(version)X
1664(of)X
1751(the)X
1869(QVSS)X
2093(display)X
2344(ddx)X
2484(code.)X
555 4572(server/ddx/sun)N
755 4668(This)N
918(is)X
991(the)X
1109(directory)X
1419(containing)X
1777(general)X
2034(Sun)X
2178(ddx)X
2318(code.)X
2530(It)X
2599(expects)X
2860(to)X
2942(be)X
3038(linked)X
3258(with)X
3420(the)X
3538(cfb)X
3661(code.)X
3873(You)X
755 4764(should)N
988(consult)X
1239(the)X
1357(README)X
1715(\256le)X
1837(in)X
1919(this)X
2054(directory)X
2364(before)X
2590(using)X
2783(this)X
2918(server.)X
555 4888(server/dix)N
755 4984(In)N
842(here)X
1001(are)X
1120(the)X
1238(sources)X
1499(for)X
1613(the)X
1731(device-independent)X
2380(layer)X
2561(of)X
2648(the)X
2766(sample)X
3013(server.)X
555 5108(util/bm-convert)N
755 5204(This)N
922(is)X
1000(a)X
1061(special)X
1309(utility)X
1524(for)X
1643(converting)X
2011(X10-format)X
2414(bitmaps)X
2691(to)X
2777(X11-style.)X
3157(Alternatively,)X
3624(you)X
3768(can)X
3904(run)X
755 5300(the)N
875(bitmap)X
1119(program,)X
1433(which)X
1651(can)X
1785(read)X
1946(X10-format)X
2347(bitmaps)X
2622(and)X
2760(will)X
2906(always)X
3151(write)X
3338(X11-format)X
3738(bitmaps.)X
755 5396(This)N
917(program)X
1209(will)X
1353(never)X
1552(be)X
1648(built)X
1814(unless)X
2034(you)X
2174(explicitly)X
2496(do)X
2596(so)X
2687(in)X
2769(this)X
2904(directory.)X
555 5520(util/imake)N
755 5616(This)N
921(facility)X
1172(has)X
1303(become)X
1577(crucial)X
1820(to)X
1906(the)X
2028(maintenance)X
2458(of)X
2549(the)X
2671(X11)X
2833(source)X
3067(tree.)X
3252(If)X
3330(you)X
3474(have)X
3649(a)X
3708(machines)X
755 5712(other)N
946(that)X
1092(what)X
1274(this)X
1415(release)X
1665(has)X
1798(been)X
1976(tested)X
2189(for,)X
2329(I)X
2382(suggest)X
2648(that)X
2794(you)X
2940(read)X
3105(the)X
3229(man)X
3393(page)X
3570(\(found)X
3809(in)X
3896(this)X
755 5808(directory\))N
1098(very)X
1267(carefully.)X
1619(This)X
1787(is)X
1866(the)X
1990(program)X
2288(used)X
2461(to)X
2548(create)X
2766(all)X
2871(the)X
2994(Make\256les)X
3335(from)X
3516(the)X
3639(\256les)X
3797(named)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

12 p
%%Page: 12 12
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2196 416(-)N
2243(12)X
2343(-)X
1 f
755 672("Imake\256le")N
1144(throughout)X
1515(the)X
1633(source)X
1863(tree.)X
555 796(util/imake.includes)N
755 892(This)N
930(is)X
1016(where)X
1246(the)X
1377(header)X
1625(\256les)X
1790(and)X
1938(template)X
2246(are)X
2377(kept)X
2547(for)X
2673(the)X
2803(imake)X
3031(utility.)X
3293(You)X
3463(should)X
3708(study)X
3913(the)X
755 988(*.macros)N
1074(\256les)X
1234(carefully)X
1547(to)X
1636(decide)X
1873(how)X
2037(to)X
2125(con\256gure)X
2454(your)X
2627(machine.)X
2965(There)X
3179(is)X
3258(more)X
3449(on)X
3555(this)X
3696(subject)X
3949(in)X
755 1084(section)N
1002(2.)X
555 1208(util/makedepend)N
755 1304(This)N
923(utility)X
1139(is)X
1218(used)X
1391(to)X
1479(generate)X
2 f
1778(make)X
1 f
1948(\(1\))X
2068(dependencies)X
2527(for)X
2647(all)X
2753(the)X
2876(.o)X
2961(\256les.)X
3159(It)X
3233(runs)X
3396(an)X
3497(order)X
3692(of)X
3784(magni-)X
755 1400(tude)N
916(faster)X
1117(than)X
1277(any)X
1415(other)X
1602(implementation)X
2126(of)X
2215(a)X
2273 0.2303(dependency-generator)AX
3010(by)X
3112(caching)X
3384(the)X
3504(include)X
3762(\256les)X
3917(for)X
755 1496(each)N
923(directory.)X
1273(There)X
1481(is)X
1554(more)X
1739(on)X
1839(this)X
1974(subject)X
2221(in)X
2303(section)X
2550(2.)X
555 1620(util/patch)N
755 1716(This)N
930(is)X
1015(the)X
1145(source-code)X
1566(patching)X
1874(utility)X
2096(written)X
2355(by)X
2467(Larry)X
2678(Wall)X
2866(and)X
3014(distributed)X
3388(in)X
3482(mod.sources.)X
3957(If)X
755 1812(there)N
946(are)X
1075(patches)X
1346(sent)X
1505(out)X
1637(from)X
1823(MIT,)X
2020(this)X
2165(program)X
2466(can)X
2607(be)X
2712(used)X
2888(to)X
2979(apply)X
3186(them.)X
3415(This)X
3586(program)X
3887(will)X
755 1908(never)N
954(be)X
1050(built)X
1216(unless)X
1436(you)X
1576(explicitly)X
1898(do)X
1998(so)X
2089(in)X
2171(this)X
2306(directory.)X
555 2032(util/rgb)N
755 2128(This)N
923(the)X
1047(the)X
1171(unchanged)X
1544(X10)X
1707(version)X
1968(of)X
2060(program)X
2357(by)X
2462(the)X
2585(same)X
2775(name.)X
3014(It)X
3088(reads)X
3283(a)X
3344(text)X
3489(\256le)X
3616(describing)X
3975(a)X
755 2224(set)N
864(of)X
951(colors,)X
1187(and)X
1323(creates)X
1567(a)X
1623(database)X
1920(for)X
2034(the)X
2152(server)X
2369(to)X
2451(consult.)X
555 2348(util/vtobdf)N
755 2444(This)N
917(program)X
1209(reads)X
1399(a)X
1455(Sun)X
1599(vfont)X
1788(\256le)X
1910(and)X
2046(creates)X
2290(a)X
2346(character)X
2662(bitmap)X
2904(distribution)X
3292(format)X
3526(\256le.)X
3 f
555 2636(3.1.)N
715(Your)X
913(.Xdefaults)X
1282(\256le)X
1 f
555 2760(In)N
653(previous)X
960(releases)X
1246(of)X
1344(X,)X
1453(the)X
1582(speci\256cation)X
2018(of)X
2116(con\256guration)X
2574(options)X
2840(in)X
2933(the)X
3062(.Xdefaults)X
3425(\256le)X
3558(was)X
3713(not)X
3845(case-)X
555 2856(sensitive,)N
875(although)X
1175(this)X
1310(fact)X
1451(was)X
1596(never)X
1795(explicitly)X
2117(mentioned)X
2475(in)X
2557(any)X
2693(documentation.)X
555 2980(For)N
694(version)X
958(11,)X
1086(we)X
1208(have)X
1388(chosen)X
1639(to)X
1729(de\256ne)X
1953(the)X
2079(contents)X
2374(of)X
2469(.Xdefaults)X
2829(to)X
2919(be)X
3022(case-sensitive.)X
3535(This)X
3704(is)X
3784(not)X
3913(yet)X
555 3076(fully)N
726(implemented;)X
1186(some)X
1375(clients)X
1604(may)X
1762(still)X
1901(allow)X
2099(non-matching)X
2564(case)X
2723(in)X
2805(their)X
2972(options.)X
555 3200(The)N
709(principal)X
1023(reason)X
1262(for)X
1385(this)X
1529(change)X
1786(is)X
1868(the)X
1995(addition)X
2286(of)X
2382(option)X
2615("classes")X
2933(to)X
3024(the)X
3151(Resource)X
3479(Manager,)X
3814(where)X
555 3296(before)N
782(everything)X
1146(was)X
1292(only)X
1455(an)X
1552(option)X
1777("instance".)X
2167(The)X
2313(new)X
2468(naming)X
2729(and)X
2866(classing)X
3145(hierarchy)X
3470(will)X
3615(enable)X
3846(users)X
555 3392(to)N
640(customize)X
988(large)X
1172(parts)X
1351(of)X
1441(client)X
1642(interfaces)X
1978(with)X
2143(much)X
2344(fewer)X
2551(lines)X
2725(than)X
2886(before,)X
3135(at)X
3216(the)X
3337(expense)X
3619(of)X
3709(having)X
3949(to)X
555 3488(read)N
714(the)X
832(man)X
990(pages)X
1193(somewhat)X
1538(more)X
1723(carefully)X
2029(when)X
2223(customizing.)X
3 f
555 3680(4.)N
655(Xlib)X
821(-)X
868(C)X
946(Language)X
1303(X)X
1381(Interface)X
1 f
555 3804(These)N
771(notes)X
964(document)X
1304(the)X
1426(known)X
1667(problems)X
1988(in)X
2073(the)X
2194(base)X
2360(X)X
2441(library)X
2678(as)X
2768(of)X
2858(the)X
2979(\256rst)X
3126(release)X
3373(of)X
3463(X)X
3544(Version)X
3821(11,)X
3944(as)X
555 3900(well)N
718(as)X
810(some)X
1004(comments)X
1358(of)X
1450(what)X
1631(has)X
1763(changed)X
2055(during)X
2288(the)X
2410(last)X
2545(phases)X
2783(of)X
2874(beta)X
3032(test,)X
3187(since)X
3376(the)X
3498(beta)X
3656(test)X
3791(was)X
3940(so)X
555 3996(large.)N
777(Bugs)X
961(reported)X
1249(after)X
1417(early)X
1598(September)X
1961(are)X
2080(unlikely)X
2362(to)X
2444(have)X
2616(been)X
2788(\256xed)X
2968(unless)X
3188(extremely)X
3529(important.)X
3900(My)X
555 4092(thanks)N
784(to)X
866(the)X
984(beta)X
1138(testers)X
1363(for)X
1477(their)X
1644(efforts)X
1874(and)X
2010(comments.)X
3 f
555 4216(XPutImage)N
1 f
973(was)X
1124(extensively)X
1515(reworked)X
1845(at)X
1929(the)X
2053(last)X
2190(moment;)X
2500(see)X
2629(the)X
2753(comments)X
3108(at)X
3192(the)X
3316(beginning)X
3662(of)X
3754(the)X
3877(rou-)X
555 4312(tine.)N
744(There)X
961(are)X
1089(a)X
1153(few)X
1302(remaining)X
1655(nits)X
1798(with)X
1968(nibble)X
2196(swapping.)X
3 f
2571(XGetImage)X
1 f
2996(semantics)X
3340(are)X
3467(slightly)X
3734(different)X
555 4408(than)N
718(during)X
952(most)X
1132(of)X
1224(beta)X
1383(test.)X
1559(If)X
1638(you)X
1783(only)X
1950(ask)X
2082(for)X
2200(certain)X
2443(planes)X
2672(in)X
2758(x-y)X
2889(mode,)X
3111(the)X
3233(image)X
3453(returned)X
3745(is)X
3822(of)X
3913(the)X
555 4504(depth)N
753(of)X
840(the)X
958(number)X
1223(of)X
1310(planes)X
1535(requested.)X
3 f
555 4628(XPutSubImage)N
1 f
1099(in)X
1181(Z)X
1250(format)X
1484(does)X
1651(not)X
1773(work)X
1958(properly.)X
3 f
555 4752(XPending)N
1 f
916(still)X
1059(uses)X
1221(FIONREAD,)X
1670(which)X
1890(means)X
2119(that)X
2263(it)X
2331(does)X
2502(not)X
2627(detect)X
2842(dropped)X
3128(connections)X
3534(properly.)X
3869(This)X
555 4848(was)N
702(the)X
821(cause)X
1021(of)X
1109(many)X
1308(client)X
1507(looping)X
1772(bugs)X
1944(when)X
2139(connections)X
2543(failed)X
2747(on)X
2848(V10.)X
3047(It)X
3117(should)X
3351(be)X
3448(be)X
3545(recoded)X
3821(to)X
3904(use)X
555 4944(a)N
611(non-blocking)X
1058(read)X
1217(of)X
1304(0)X
1364(bytes)X
1553(to)X
1635(get)X
1753(status)X
1955(back.)X
3 f
555 5068(XGetDefault)N
1 f
1016(now)X
1178(has)X
1309(the)X
1431(correct)X
1679(interface)X
1985(with)X
2151(a)X
2210(\(Display)X
2509(*\))X
2599(as)X
2689(the)X
2810(\256rst)X
2957(argument.)X
3323(Note)X
3502(that)X
3645(during)X
3877(beta)X
555 5164(test,)N
715(the)X
842(document)X
1187(was)X
1341(correct,)X
1614(and)X
1759(the)X
1885(code)X
2065(was)X
2218(wrong.)X
2491(Your)X
2684(code)X
2864(which)X
3088(you)X
3236(may)X
3402(have)X
3582(been)X
3762(running)X
555 5260(during)N
784(beta)X
938(test)X
1069(using)X
1262(this)X
1397(routine)X
1644(will)X
1788(have)X
1960(to)X
2042(be)X
2138(converted.)X
555 5384(The)N
701(types)X
891(of)X
979(members)X
1294(of)X
1382(some)X
1572(structures)X
1905(in)X
1988(Xlib.h)X
2211(were)X
2389(\256xed)X
2570(so)X
2662(that)X
2803(machines)X
3127(on)X
3228(which)X
3445(int=16)X
3675(bits)X
3811(would)X
555 5480(work)N
750(correctly.)X
1105(Some)X
1316(members)X
1639(were)X
1825(declared)X
2127(int)X
2240(but)X
2371(were)X
2557(plane)X
2760(masks)X
2989(and)X
3134(should)X
3376(have)X
3557(been)X
3738(declared)X
555 5576(unsigned)N
865(long.)X
1068(Some)X
1271(other)X
1457(members)X
1772(were)X
1950(renamed)X
2248(to)X
2331(type)X
3 f
2490(Bool)X
1 f
2666(and)X
3 f
2803(Time)X
1 f
2981(,)X
3022(though)X
3265(the)X
3384(underlying)X
3751(type)X
3909(did)X
555 5672(not)N
677(change.)X
3 f
555 6144(X)N
633(Window)X
941(System)X
1206(Release)X
1485(Notes)X
3566(X)X
3644(Version)X
3931(11)X

13 p
%%Page: 13 13
10 s 0 xH 0 xS 3 f
1 f
0 32(--)N
4323(--)X
3 f
2196 416(-)N
2243(13)X
2343(-)X
1 f
555 672(The)N
702(region)X
929(code)X
1103(in)X
1187(the)X
1307(library)X
1543(has)X
1671(been)X
1844(regenerated)X
2241(after)X
2410(extraction)X
2752(from)X
2929(the)X
3048(server.)X
3306(Since)X
3505(no)X
3606(one)X
3743(reported)X
555 768(a)N
616(single)X
832(bug)X
977(in)X
1064(the)X
1187(region)X
1417(code)X
1594(during)X
1828(beta)X
1987(test)X
2123(and)X
2264(we)X
2383(know)X
2586(there)X
2772(were)X
2954(many)X
3157(bugs)X
3333(in)X
3420(it)X
3489(from)X
3670(server)X
3891(bug)X
555 864(reports,)N
818(it)X
882(clearly)X
1121(was)X
1266(not)X
1388(heavily)X
1644(exercised)X
1968(\(if)X
2064(at)X
2142(all\))X
2269(then.)X
555 988(There)N
767(ought)X
972(to)X
1057(be)X
1156(some)X
1348(way)X
1505(to)X
1590(generate)X
1886(a)X
1945(region)X
2173(from)X
2352(a)X
2411(series)X
2617(of)X
2707(exposure)X
3020(events,)X
3268(to)X
3353(make)X
3550(it)X
3617(very)X
3783(easy)X
3949(to)X
555 1084(do)N
660(clever)X
882(repaint)X
1130(on)X
1235(exposure)X
1550(of)X
1642(only)X
1808(exposed)X
2095(areas,)X
2305(using)X
2502(the)X
2624(GC)X
2759(region.)X
3028(No)X
3150(one)X
3290(volunteered)X
3693(to)X
3779(specify)X
555 1180(it/write)N
813(it,)X
904(so)X
1002(there)X
1190(still)X
1336(is)X
1416(no)X
1523(way.)X
1724(The)X
1876(region)X
2108(code)X
2287(was)X
2439(known)X
2683(to)X
2771(be)X
2873(buggy)X
3099(during)X
3334(beta)X
3494(test,)X
3651(which)X
3873(may)X
555 1276(have)N
727(reduced)X
1002(the)X
1120(inclination.)X
555 1400(V10)N
719(conversion)X
1097(aid)X
1221(code)X
1399(documented)X
1817(in)X
1905(the)X
2029(X)X
2113(manual)X
2375(appendix)X
2695(is)X
2774(in)X
2862(a)X
2924(separate)X
3214(library;)X
3476(to)X
3564(link)X
3714(against)X
3967(it)X
555 1496(under)N
758(UNIX\262)X
1019(add)X
1155(a)X
2 f
1211(-loldX)X
1 f
1431(to)X
1513(your)X
1680(link)X
1824(command.)X
11 s
555 1620(The)N
719(resource)X
1043(manager)X
1373(is)X
1459(currently)X
1804(split)X
1984(out)X
2124(into)X
2289(a)X
2355(separate)X
2670(library)X
2932(called)X
2 f
3170(Xrm)X
1 f
3321(.)X
3392(It)X
3473(will)X
3638(be)X
3747(merged)X
555 1716(in)N
650(a)X
715(future)X
951(release.)X
1265(This)X
1448(occurred)X
1780(since)X
1986(the)X
2119(tool)X
2282(kit)X
2401(did)X
2539(not)X
2677(get)X
2810(converted)X
3182(to)X
3276(use)X
3418(the)X
3551(correct)X
3820(inter-)X
555 1812(faces.)N
802(The)X
962(old)X
1098(\(beta)X
1297(version\))X
1608(is)X
1690(in)X
1782(oldXrm)X
2079(and)X
2229(is)X
2310(still)X
2466(used)X
2649(by)X
2759(the)X
2889(tool)X
3049(kits.)X
3243(Since)X
3461(the)X
3591(X)X
3676(tool)X
3836(kit)X
3952(is)X
555 1908(the)N
691(primary)X
998(user)X
1172(of)X
1273(the)X
1409(resource)X
1734(manager,)X
2087(this)X
2243(also)X
2413(means)X
2666(that)X
2827(the)X
2963(current)X
3240(resource)X
3564(manager)X
3894(has)X
555 2004(not)N
692(had)X
843(the)X
975(extensive)X
1332(exercise)X
1644(of)X
1741(the)X
1873(rest)X
2024(of)X
2121(the)X
2252(library)X
2510(\(or)X
2635(even)X
2824(of)X
2920(the)X
3051(old)X
3187(resource)X
3507(manager\),)X
3884(and)X
555 2100(will)N
735(undergo)X
1065(some)X
1293(\256nal)X
1492(changes)X
1817(before)X
2083(being)X
2320(incorporated,)X
2828(though)X
3114(the)X
3263(interface)X
3612(is)X
3712(believed)X
555 2196(correct)N
826(except)X
1083(for)X
2 f
1212(XrmPutResource)X
1 f
1813(,)X
1862(which)X
2104(should)X
2366(take)X
2540(a)X
2606(string)X
2834(rather)X
3066(than)X
3244(a)X
3309(quark.)X
3579(This)X
3762(change)X
555 2292(was)N
716(dropped)X
1029(on)X
1142(the)X
1275(\257oor)X
1466(until)X
1654(after)X
1840(the)X
1973(code)X
2164(had)X
2316(been)X
2507(frozen)X
2756(for)X
2882(release.)X
3194(Note)X
3389(that)X
3546(only)X
3727(applica-)X
555 2388(tions)N
761(linked)X
1016(against)X
1300(one)X
1461(of)X
1568(the)X
1710(resource)X
2041(manager)X
2378(libraries)X
2701(will)X
2873(use)X
3024(the)X
3166(error)X
3369(database)X
3705(as)X
3811(docu-)X
555 2484(mented)N
840(in)X
934(the)X
1067(manual.)X
1396(Clients)X
1672(linking)X
1948(only)X
2130(against)X
2405(Xlib)X
2587(will)X
2750(only)X
2932(print)X
3124(the)X
3257(default)X
3526(strings.)X
3829(Until)X
555 2580(the)N
695(next)X
879(MIT)X
1072(release,)X
1370(you)X
1533(should)X
1799(therefore)X
2147(understand)X
2564(that)X
2728(it)X
2809(is)X
2899(conceivable)X
3351(that)X
3515(these)X
3727(routines)X
555 2676(may)N
733(change)X
1008(interfaces;)X
1401(somewhat)X
1784(more)X
1991(likely)X
2219(is)X
2304(that)X
2463(more)X
2669(sophisticated)X
3155(matching)X
3509(will)X
3672(be)X
3780(imple-)X
555 2772(mented)N
840(without)X
1135(interface)X
1468(changes.)X
1820(The)X
1982(most)X
2179(unfortunate)X
2609(part)X
2770(of)X
2867(this)X
3019(is)X
3102(that)X
3259(only)X
3 f
3440(XGetDefault)X
1 f
3942(in)X
555 2868(the)N
692(new)X
867(resource)X
1193(manager)X
1525(knows)X
1783(how)X
1963(to)X
2061(deal)X
2237(with)X
2423(the)X
2559(server)X
2801(resource)X
3126(data)X
3301(base)X
3485(property.)X
3855(\(See)X
555 2964(the)N
688(xrdb)X
874(utility\).)X
1185(\(One)X
1385(of)X
1483(the)X
1616(real)X
1773(pains)X
1983(to)X
2076(life)X
2218(in)X
2311(V10)X
2486(was)X
2646(keeping)X
2949(your)X
3134(.Xdefaults)X
3522(\256le)X
3659(up)X
3771(to)X
3864(date)X
555 3060(on)N
665(all)X
776(the)X
906(machines)X
1261(you)X
1415(used\).)X
555 3184(The)N
729(library)X
1001(is)X
1097(not)X
1247(as)X
1357(portable)X
1683(to)X
1789(some)X
2012(machine)X
2348(architectures)X
2835(as)X
2945(we)X
3084(would)X
3341(like.)X
3555(The)X
3728(changes)X
555 3280(required)N
877(to)X
975(deal)X
1151(with)X
1337(the)X
1474(Cray)X
1674(for)X
1805(example,)X
2155(were)X
2354(more)X
2564(extensive)X
2926(than)X
3106(we)X
3236(wished)X
3513(to)X
3610(incorporate)X
555 3376(during)N
811(beta)X
984(test,)X
1155(affecting)X
1494(too)X
1633(many)X
1855(\256les)X
2028(with)X
2211(insuf\256cient)X
2633(time)X
2816(for)X
2943(shake)X
3168(down.)X
3432(My)X
3579(apologies)X
3942(to)X
555 3472(them,)N
791(since)X
1009(it)X
1096(will)X
1270(have)X
1472(cost)X
1650(them)X
1863(additional)X
2253(work)X
2469(until)X
2668(the)X
2812(changes)X
3131(are)X
3274(made.)X
3545(Machines)X
3923(on)X
555 3568(which)N
792(short!=16)X
1157(bits)X
1307(may)X
1481(have)X
1669(dif\256culty)X
2015(in)X
2106(a)X
2167(port)X
2331(at)X
2417(the)X
2547(moment.)X
555 3692(The)N
716(V10)X
891(functionality)X
1367(that)X
1523(allowed)X
1825(remapping)X
2225(keys)X
2409(to)X
2501(strings)X
2759(read)X
2933(in)X
3025(from)X
3219(a)X
3281(\256le)X
3417(did)X
3553(not)X
3689(get)X
3820(reim-)X
555 3788(plemented,)N
978(due)X
1138(to)X
1240(upheavals)X
1625(during)X
1888(both)X
2078(alpha)X
2301(and)X
2460(beta)X
2639(tests)X
2828(in)X
2929(keyboard)X
3288(handling)X
3629(in)X
3730(general.)X
555 3884(This)N
737(is)X
821(less)X
978(needed)X
1252(in)X
1346(V11,)X
1544(where)X
1783(there)X
1984(is)X
2068(global)X
2314(renaming)X
2672(of)X
2770(keysyms,)X
3125(but)X
3262(would)X
3506(still)X
3664(be)X
3771(nice)X
3942(to)X
555 3980(have.)N
787(The)X
946(compose)X
1281(processing)X
1679(\(yuk\))X
1891(in)X
3 f
1982(XLookupString)X
1 f
2596(is)X
2677(not)X
2812(implemented.)X
555 4104(There)N
792(are)X
931(an)X
1046(extensive)X
1411(set)X
1541(of)X
1646(routines/macros)X
2241(for)X
2375(extracting)X
2759(screen)X
3014(structure)X
3353(information)X
3801(added)X
555 4200(late)N
712(in)X
810(beta)X
985(test.)X
1180(The)X
1345(tool)X
1511(kit)X
1633(is)X
1720(being)X
1944(reorganized)X
2387(to)X
2484(deal)X
2659(with)X
2844(multiple)X
3168(screen)X
3420(applications)X
3875(in)X
3972(a)X
555 4296(graceful)N
865(way,)X
1055(and)X
1204(these)X
1407(additions)X
1753(were)X
1945(clearly)X
2207(useful.)X
555 4420(Manual)N
846(pages)X
1068(for)X
1192(Xlib)X
1371(now)X
1544(exist,)X
1755(though)X
2022(are)X
2151(quite)X
2350(terse.)X
555 4544(Extensive)N
927(work)X
1131(on)X
1243(the)X
1375(Xlib)X
1556(manual)X
1840(took)X
2021(place;)X
2256(the)X
2388(most)X
2584(glaring)X
2858(de\256ciencies)X
3298(are)X
3429(the)X
3561(lack)X
3732(of)X
3829(illus-)X
555 4640(trations)N
852(in)X
953(the)X
1093(right)X
1292(places.)X
1588(There)X
1825(will)X
1995(be)X
2110(some)X
2328(additional)X
2714(work)X
2926(on)X
3045(the)X
3184(manual)X
3475(until)X
3669(it)X
3750(goes)X
3942(to)X
555 4736(Digital)N
823(Press)X
1030(for)X
1154(printing)X
1456(as)X
1551(a)X
1612(book.)X
8 s
10 f
555 4816(hhhhhhhhhhhhhhhhhh)N
9 s
1 f
555 4924(The)N
685(X)X
755(Window)X
1021(System)X
9 f
1251(\324)X
1 f
1333(is)X
1399(a)X
1449(trademark)X
1759(of)X
1837(the)X
1943(Massachusetts)X
2377(Institute)X
2631(of)X
2709(Technology.)X
555 5032(\262)N
609(UNIX)X
807(is)X
873(a)X
923(trademark)X
1233(of)X
1311(AT&T)X
1525(Bell)X
1663(Laboratories.)X
555 5140(Copyright)N
9 f
866(\323)X
1 f
942(1987,)X
1123(Massachusetts)X
1557(Institute)X
1811(of)X
1889(Technology,)X
2269(Cambridge,)X
2625(Massachusetts,)X
3077(and)X
3199(Digital)X
3417(Equipment)X
3751(Corpora-)X
555 5220(tion,)N
703(Maynard,)X
999(Massachusetts.)X
555 5328(Permission)N
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
555 5408(provided)N
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
555 5488(notice)N
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
555 5568(city)N
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
555 5648(representations)N
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
555 5728(express)N
789(or)X
867(implied)X
1105(warranty.)X
3 f
11 s
555 6144(X)N
640(Window)X
980(System)X
1270(Release)X
1575(Notes)X
3523(X)X
3608(Version)X
3923(11)X

14 p
%%Page: 14 14
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
2188 416(-)N
2239(14)X
2349(-)X
1 f
555 672(We)N
699(still)X
855(don't)X
1063(have)X
1251(an)X
1356(introductory)X
1814(Xlib)X
1993(Primer.)X
2299(Any)X
2472(volunteers)X
2861(out)X
2996(there?)X
555 796(Locking)N
871(for)X
995(multi-threading)X
1567(is)X
1648(still)X
1804(untested.)X
2164(Unix)X
2362(needs)X
2584(a)X
2645(new)X
2813(system)X
3080(call)X
3230(interface!)X
3 f
555 988(5.)N
665(Credits)X
1 f
555 1112(Whereas)N
890(the)X
1025(design)X
1282(and)X
1436 0.2885(implementation)AX
2019(of)X
2119(the)X
2254(\256rst)X
2418(10)X
2533(versions)X
2853(of)X
2953(X)X
3043(were)X
3240(primarily)X
3596(the)X
3731(work)X
3938(of)X
555 1208(three)N
758(individuals,)X
1200(Robert)X
1467(Schei\257er)X
1807(of)X
1907(the)X
2042(M.I.T.)X
2318(Laboratory)X
2736(for)X
2865(Computer)X
3245(Science)X
3546(and)X
3700(Jim)X
3855(Get-)X
555 1304(tys)N
700(of)X
815(Digital)X
1103(Equipment)X
1533(Corporation)X
2001(and)X
2170(Ron)X
2359(Newman)X
2718(of)X
2833(M.I.T.,)X
3124(both)X
3322(working)X
3656(at)X
3761(Project)X
555 1400(Athena,)N
865(X)X
962(version)X
1255(11)X
1377(is)X
1469(the)X
1610(result)X
1839(of)X
1945(the)X
2086(efforts)X
2348(of)X
2454(literally)X
2763(dozens)X
3040(of)X
3146(individuals)X
3572(at)X
3669(almost)X
3938(as)X
555 1496(many)N
786(locations)X
1140(and)X
1302(organizations.)X
1856(At)X
1979(the)X
2122(risk)X
2289(of)X
2396(offending)X
2772(some)X
2992(of)X
3099(the)X
3241(players)X
3529(by)X
3651(exclusion,)X
555 1592(several)N
826(people)X
1083(deserve)X
1373(special)X
1640(credit)X
1863(and)X
2012(recognition.)X
555 1716(Managerial)N
985(Support)X
1293(\(people)X
1585(without)X
1883(whose)X
2135(support)X
2427(and)X
2582(political)X
2901(battles)X
3160(the)X
3296(release)X
3568(would)X
3816(never)X
555 1812(have)N
743(materialized\):)X
843 1956(Carol)N
1061(Berry,)X
1305(IBM)X
2371(Steve)X
2589(Lerman,)X
2907(MIT)X
843 2052(Larry)N
1060(Breed,)X
1314(IBM)X
2371(Al)X
2481(Lopez,)X
2745(Apollo)X
843 2148(Michael)N
1154(Dertouzos,)X
1559(MIT)X
2371(Scott)X
2575(McGregor,)X
2984(Digital)X
843 2244(Sam)N
1022(Fuller,)X
1277(Digital)X
2371(Jerry)X
2568(Saltzer,)X
2857(MIT)X
843 2340(Dan)N
1011(Geer,)X
1225(MIT)X
2371(Warren)X
2656(Teitelman,)X
3059(Sun)X
843 2436(Jeff)N
996(Lane,)X
1216(Digital)X
2371(Dave)X
2578(Verhoven,)X
2968(Tektronix)X
843 2532(Rick)N
1032(LeFaivre,)X
1394(Tektronix)X
2371(Smokey)X
2682(Wallace,)X
3015(Digital)X
555 2676(Thanks)N
836(also)X
1000(to)X
1091(DARPA,)X
1432(for)X
1556(supporting)X
1955(the)X
2085(research)X
2399(side)X
2563(of)X
2658(the)X
2788(X)X
2873(protocol.)X
555 2800(Our)N
726(undying)X
1050(gratitude)X
1399(to)X
1503(Todd)X
1724(Brunhoff)X
2080(\(and)X
2270(to)X
2373(Tektronix)X
2755(for)X
2891(sending)X
3199(him\);)X
3425(Todd)X
3645(performed)X
555 2896(wonders)N
876(in)X
969(making)X
1257(the)X
1388(alpha)X
1602(and)X
1752(beta)X
1922(tests)X
2102(happen,)X
2401(\256ltering)X
2704(bug)X
2859(reports,)X
3148(integrating)X
3554(sources)X
3840(from)X
555 2992(everywhere,)N
1008(putting)X
1281(the)X
1411(releases)X
1711(and)X
1860(updates)X
2151(together,)X
2484(and)X
2633(generally)X
2983(hacking)X
3284(up)X
3394(a)X
3455(storm.)X
555 3116(In\256nite)N
845(thanks)X
1110(to)X
1214(Ken)X
1395(Goldman,)X
1780(MIT,)X
1997(for)X
2133(accepting)X
2505(thousands)X
2891(of)X
2998(phone)X
3247(calls,)X
3465(mail)X
3657(messages,)X
555 3212(being)N
785(patient,)X
1082(sending)X
1389(out)X
1535(documentation,)X
2115(and)X
2275(generally)X
2636(keeping)X
2948(us)X
3059(insulated)X
3411(from)X
3615(the)X
3756(outside)X
555 3308(world.)N
555 3432(Great)N
775(appreciation)X
1236(goes)X
1422(to)X
1516(Phil)X
1684(Karlton)X
1978(and)X
2130(Scott)X
2337(McGregor,)X
2749(both)X
2931(of)X
3029(DEC,)X
3252(for)X
3379(their)X
3566(considerable)X
555 3528(contributions)N
1053(to)X
1154(the)X
1294(speci\256cation)X
1772(of)X
1877(the)X
2017(version)X
2308(11)X
2427(protocol.)X
2796(Also)X
2993(Sue)X
3156(Angebranndt,)X
3668(Raymond)X
555 3624(Drewry)N
847(and)X
999(Todd)X
1210(Newman)X
1552(of)X
1649(DEC's)X
1912(Western)X
2229(Software)X
2570(Laboratory,)X
3007(with)X
3188(Phil,)X
3377(have)X
3567(worked)X
3854(long)X
555 3720(and)N
704(hard)X
882(to)X
973(produce)X
1278(the)X
1408(sample)X
1680(server)X
1916 0.2679(implementation.)AX
555 3844(Special)N
838(thanks)X
1091(must)X
1286(also)X
1451(go)X
1562(to)X
1654(Sam)X
1833(Fuller,)X
2088(Vice)X
2276(President)X
2626(of)X
2721(Research)X
3065(at)X
3151(DEC,)X
3371(who)X
3544(has)X
3683(remained)X
555 3940(committed)N
961(to)X
1057(the)X
1192(widest)X
1449(public)X
1697 0.2898(availability)AX
2123(of)X
2223(X)X
2313(and)X
2467(who)X
2645(foresaw)X
2949(a)X
3015(year)X
3192(ago)X
3345(the)X
3479(need)X
3671(to)X
3766(greatly)X
555 4036(supplement)N
984(Project)X
1256(Athena's)X
1595(resources)X
1948(with)X
2127(the)X
2257(WSL)X
2465(staff)X
2643(in)X
2734(order)X
2941(to)X
3032(make)X
3245(version)X
3526(11)X
3636(a)X
3697(reality.)X
555 4160(Many)N
784(thanks)X
1038(from)X
1233(us)X
1335(and)X
1486(the)X
1618(several)X
1891(vendors)X
2193(who)X
2368(have)X
2558(a)X
2621(head)X
2811(start)X
2987(on)X
3099(porting)X
3378(X11)X
3553(to)X
3645(their)X
3830(color)X
555 4256(frambuffers)N
994(goes)X
1179(to)X
1272(Dave)X
1481(Rosenthal)X
1858(of)X
1955(Sun)X
2116(Microsystems.)X
2678(He)X
2804(provided)X
3141(the)X
3273(sample)X
3547(generic)X
3830(color)X
555 4352(frame)N
782(buffer)X
1018(ddx)X
1172(code)X
1360(and)X
1509(also)X
1673(contributed)X
2097(to)X
2188(the)X
2318(protocol.)X
555 4476(Many)N
788(other)X
997(people)X
1260(made)X
1478(it)X
1555(happen,)X
1858(either)X
2086(with)X
2270(code)X
2463(or)X
2563(documentation;)X
3140(so)X
3245(many)X
3468(of)X
3568(these)X
3776(people)X
555 4572(worked)N
864(on)X
998(more)X
1225(than)X
1423(one)X
1595(major)X
1846(component)X
2283(of)X
2401(the)X
2554(system)X
2844(\(toolkits,)X
3206(libraries,)X
3562(applications,)X
555 4668(servers\))N
854(that)X
1009(we)X
1133(have)X
1321(not)X
1456(attempted)X
1827(to)X
1918(list)X
2049(people)X
2306(by)X
2416(component:)X
3 f
555 6144(X)N
640(Window)X
980(System)X
1270(Release)X
1575(Notes)X
3523(X)X
3608(Version)X
3923(11)X

15 p
%%Page: 15 15
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
2188 416(-)N
2239(15)X
2349(-)X
1 f
843 720(Allen)N
1061(Akin,)X
1281(Digital)X
2371(Geoff)X
2597(Lee,)X
2773(Sun)X
843 816(Susan)N
1075(Angebranndt,)X
1578(Digital)X
2371(Ed)X
2491(Lee,)X
2667(HP)X
843 912(Tom)N
1032(Benson,)X
1340(Digital)X
2371(Bob)X
2540(Leichner,)X
2897(HP)X
843 1008(Adam)N
1080(de)X
1185(Boor,)X
1405(UC)X
1549(Berkeley)X
2371(Mark)X
2583(Manasse,)X
2934(Digital)X
843 1104(Todd)N
1051(Brunhoff,)X
1417(Tektronix)X
2371(Stuart)X
2604(Marks,)X
2872(Sun)X
843 1200(Richard)N
1144(Carling,)X
1453(Masscomp)X
2371(Joel)X
2535(McCormack,)X
3019(Digital)X
843 1296(Colyn)N
1081(Case,)X
1296(Digital)X
2371(Rick)X
2560(McKay,)X
2867(HP)X
843 1392(Ann)N
1016(Mei)X
1180(Chang,)X
1454(Digital)X
2371(Al)X
2481(Mento,)X
2755(Digital)X
843 1488(Mike)N
1051(Chow,)X
1305(Digital)X
2371(Don)X
2544(Mullis,)X
2819(Digital)X
843 1584(Larry)N
1060(Church,)X
1363(Digital)X
2371(Hal)X
2520(Murray,)X
2827(Digital)X
843 1680(Jeff)N
996(Dike,)X
1211(Digital)X
2371(Todd)X
2579(Newman,)X
2940(Digital)X
843 1776(Raymond)N
1208(Drewry,)X
1519(Digital)X
2371(Ron)X
2540(Newman,)X
2901(MIT)X
843 1872(Jerry)N
1040(Farrell,)X
1319(Sun)X
2371(Mark)X
2583(Opperman,)X
2998(Sun)X
843 1968(Burns)N
1075(Fisher,)X
1339(Digital)X
2371(Vince)X
2603(Orgovan,)X
2954(Digital)X
843 2064(Erik)N
1017(Fortune,)X
1335(IBM)X
2371(Jack)X
2549(Palevich,)X
2897(HP)X
843 2160(Hania)N
1075(Gajewska,)X
1465(Digital)X
2371(Matt)X
2560(Pappas,)X
2853(Digital)X
843 2256(Mike)N
1051(Gancarz,)X
1387(Digital)X
2371(Ram)X
2560(Rao,)X
2746(Digital)X
843 2352(Peter)N
1046(George,)X
1348(Digital)X
2371(Loretta)X
2648(Guarino)X
2958(Reid,)X
3169(Digital)X
843 2448(Jim)N
993(Gettys,)X
1267(Digital)X
2371(David)X
2608(Rosenthal,)X
3005(Sun)X
843 2544(Dave)N
1050(Gorgen,)X
1357(Apollo)X
2371(Paul)X
2550(Rubin,)X
2810(MIT)X
843 2640(Jackie)N
1085(Grand\256eld,)X
1505(Digital)X
2371(Robin)X
2609(Schau\257er,)X
2985(Sun)X
843 2736(Joel)N
1007(Gringorten,)X
1437(Digital)X
2371(Bob)X
2540(Schei\257er,)X
2897(MIT)X
843 2832(Phil)N
1008(Gust,)X
1218(HP)X
2371(Mike)X
2579(Schwartz,)X
2950(Sun)X
843 2928(Frank)N
1070(Hall,)X
1266(HP)X
2371(Alok)X
2569(Singhania,)X
2966(Sun)X
843 3024(Peter)N
1046(Ham,)X
1261(Digital)X
2371(Ralph)X
2604(Swick,)X
2868(MIT,)X
3073(Digital)X
843 3120(Charles)N
1134(Haynes,)X
1441(Digital)X
2371(Al)X
2481(Tabayoyon,)X
2921(Apollo)X
843 3216(Scott)N
1047(Hennes,)X
1354(Apollo)X
2371(Fred)X
2554(Taft,)X
2745(HP)X
843 3312(Harry)N
1069(Hersh,)X
1322(Digital)X
2371(Leo)X
2530(Treggiari,)X
2902(Digital)X
843 3408(Frances)N
1138(Ho,)X
1289(Sun)X
2371(Jake)X
2549(VanNoy,)X
2890(Digital)X
843 3504(Tom)N
1032(Houser,)X
1329(HP)X
2371(Pam)X
2550(Verno,)X
2813(Digital)X
843 3600(Rich)N
1032(Hyde,)X
1266(Digital)X
2371(Smokey)X
2682(Wallace,)X
3015(Digital)X
843 3696(Richard)N
1144(Johnsson,)X
1510(Digital)X
2371(Jeff)X
2524(Weinstein,)X
2926(IBM)X
843 3792(Phil)N
1008(Karlton,)X
1321(Digital)X
2371(Terry)X
2588(Weissman,)X
2999(Digital)X
843 3888(Kathy)N
1080(Langone,)X
1432(Digital)X
2371(Ted)X
2530(Wilson,)X
2829(HP)X
843 3984(Mary)N
1055(Larson,)X
1343(Digital)X
2371(Al)X
2481(Wojtas,)X
2775(Digital)X
555 4156(Thanks)N
836(to)X
927(the)X
1057(alpha)X
1270(and)X
1419(beta)X
1588(sites,)X
1789(who)X
1962(contributed)X
2386(much)X
2604(in)X
2695(the)X
2825(way)X
2993(of)X
3088(bug)X
3242(reports)X
3508(and)X
3657(\256xes.)X
555 4280(Thanks)N
836(to)X
927(the)X
1057(MIT)X
1240(Statistics)X
1582(Center,)X
1861(for)X
1985(the)X
2115(use)X
2254(of)X
2349(their)X
2533(Suns)X
2726(and)X
2875(for)X
2999(putting)X
3272(up)X
3382(with)X
3561(us.)X
555 4404(Thanks)N
836(to)X
927(John)X
1115(Guttag)X
1377(of)X
1472(MIT)X
1655(LCS)X
1839(for)X
1963(the)X
2093(use)X
2232(of)X
2327(an)X
2432(8600)X
2630(for)X
2754(\256nal)X
2933(build)X
3137(test.)X
3 f
555 6144(X)N
640(Window)X
980(System)X
1270(Release)X
1575(Notes)X
3523(X)X
3608(Version)X
3923(11)X
1 f
10 s
0 6360(--)N
4323(--)X

15 p
%%Trailer
xt

xs
