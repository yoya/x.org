%!PS-Adobe-1.0
%%Creator: orpheus:swick (Ralph R. Swick,,E40-327,3-1506,)
%%Title: stdin (ditroff)
%%CreationDate: Wed Sep 16 15:51:54 1987
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
1608 1536(Xlib)N
1825(-)X
1886(C)X
1987(Language)X
2452(X)X
2553(Interface)X
12 s
1877 1728(Protocol)N
2249(Version)X
2593(11)X
1 f
10 s
2111 2400(Jim)N
2246(Gettys)X
1783 2544(Digital)N
2025(Equipment)X
2396(Corporation)X
1875 2640(Systems)N
2161(Research)X
2476(Center)X
1960 2736(MIT)N
2127(Project)X
2374(Athena)X
2061 3024(Ron)N
2214(Newman)X
1665 3168(Massachusetts)N
2148(Institute)X
2430(of)X
2517(Technology)X
1960 3264(MIT)N
2127(Project)X
2374(Athena)X
1963 3552(Robert)N
2201(W.)X
2317(Schei\257er)X
1665 3696(Massachusetts)N
2148(Institute)X
2430(of)X
2517(Technology)X
1742 3792(Laboratory)N
2119(for)X
2233(Computer)X
2573(Science)X

2 p
%%Page: 2 2
10 s 0 xH 0 xS 1 f
11 s
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
555 1688(ULTRIX,)N
855(ULTRIX-32,)X
1251(ULTRIX-32m,)X
1703(ULTRIX-32w,)X
2151(and)X
2273(VAX/VMS)X
2623(are)X
2729(trademarks)X
3067(of)X
3145(Digital)X
3363(Equipment)X
3697(Corpora-)X
555 1784(tion.)N
555 1908(UNIX)N
753(is)X
819(a)X
869(trademark)X
1179(of)X
1257(AT&T)X
1471(Bell)X
1609(Laboratories.)X
555 2032(Copyright)N
9 f
865(\323)X
1 f
940(1985,)X
1120(1986,)X
1300(1987,)X
1480(Massachusetts)X
1914(Institute)X
2168(of)X
2246(Technology,)X
2626(Cambridge,)X
2982(Massachusetts,)X
3434(and)X
3556(Digital)X
3774(Equip-)X
555 2128(ment)N
717(Corporation,)X
1101(Maynard,)X
1397(Massachusetts.)X
555 2252(Permission)N
893(to)X
967(use,)X
1099(copy,)X
1275(modify)X
1501(and)X
1623(distribute)X
1913(this)X
2035(documentation)X
2481(for)X
2583(any)X
2705(purpose)X
2951(and)X
3073(without)X
3311(fee)X
3417(is)X
3483(hereby)X
3697(granted,)X
555 2348(provided)N
829(that)X
955(the)X
1061(above)X
1251(copyright)X
1545(notice)X
1739(appears)X
1977(in)X
2051(all)X
2141(copies)X
2343(and)X
2465(that)X
2591(both)X
2737(that)X
2863(copyright)X
3157(notice)X
3351(and)X
3473(this)X
3595(permission)X
555 2444(notice)N
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
555 2540(city)N
681(pertaining)X
991(to)X
1065(distribution)X
1415(of)X
1493(the)X
1599(software)X
1865(without)X
2103(speci\256c,)X
2359(written)X
2581(prior)X
2739(permission.)X
3109(M.I.T)X
3295(and)X
3417(Digital)X
3635(makes)X
3837(no)X
555 2636(representations)N
1009(about)X
1187(the)X
1293(suitability)X
1599(of)X
1677(the)X
1783(software)X
2049(described)X
2343(herein)X
2541(for)X
2643(any)X
2765(purpose.)X
3047(It)X
3109(is)X
3175(provided)X
3449(``as)X
3575(is'')X
3689(without)X
555 2732(express)N
789(or)X
867(implied)X
1105(warranty.)X

3 p
%%Page: 3 3
9 s 0 xH 0 xS 1 f
11 s
10 s
0 32(--)N
4323(--)X
3 f
12 s
2127 864(Preface)N
1 f
11 s
555 1180(Whereas)N
884(the)X
1014(design)X
1266(and)X
1415 0.2885(implementation)AX
1993(of)X
2088(the)X
2218(\256rst)X
2377(10)X
2487(versions)X
2802(of)X
2897(X)X
2982(were)X
3174(primarily)X
3525(the)X
3655(work)X
3857(of)X
555 1276(three)N
753(individuals,)X
1190(Robert)X
1452(Schei\257er)X
1787(of)X
1882(the)X
2012(MIT)X
2217(Laboratory)X
2630(for)X
2754(Computer)X
3129(Science)X
3425(and)X
3574(Jim)X
3724(Gettys)X
555 1372(of)N
650(Digital)X
918(Equipment)X
1328(Corporation)X
1776(and)X
1925(Ron)X
2094(Newman)X
2433(of)X
2528(MIT,)X
2733(both)X
2912(at)X
2998(MIT)X
3181(Project)X
3453(Athena,)X
3751(X)X
3836(ver-)X
555 1468(sion)N
724(11)X
834(is)X
915(the)X
1045(result)X
1263(of)X
1358(the)X
1488(efforts)X
1739(of)X
1834(literally)X
2132(dozens)X
2398(of)X
2493(individuals)X
2908(at)X
2994(almost)X
3252(as)X
3347(many)X
3565(locations)X
555 1564(and)N
704(organizations.)X
1245(At)X
1355(the)X
1485(risk)X
1639(of)X
1734(offending)X
2098(some)X
2306(of)X
2401(the)X
2531(players)X
2807(by)X
2917(exclusion,)X
3299(many)X
3517(people)X
555 1660(deserve)N
845(special)X
1112(credit)X
1335(and)X
1484(recognition.)X
1952(Our)X
2110(apologies)X
2470(to)X
2561(anyone)X
2837(inadvertently)X
3325(overlooked.)X
555 1784(First,)N
761(Phil)X
926(Karlton)X
1217(and)X
1366(Scott)X
1570(McGregor,)X
1979(both)X
2158(of)X
2253(Digital,)X
2543(for)X
2667(their)X
2851(considerable)X
3318(contributions)X
3806(to)X
3897(the)X
555 1880(speci\256cation)N
1023(of)X
1118(the)X
1248(version)X
1529(11)X
1639(protocol.)X
1999(Sue)X
2153(Angebranndt,)X
2656(Raymond)X
3021(Drewry,)X
3332(Todd)X
3540(Newman)X
3879(and)X
555 1976(Phil)N
720(Karlton)X
1011(of)X
1106(Digital)X
1374(have)X
1562(worked)X
1847(long)X
2026(and)X
2175(hard)X
2353(to)X
2444(produce)X
2749(the)X
2879(sample)X
3151(server)X
3387 0.2679(implementation.)AX
555 2100(Todd)N
763(Brunhoff)X
1107(of)X
1202(Tektronix)X
1572(was)X
1730(``loaned'')X
2103(to)X
2194(Project)X
2466(Athena)X
2742(at)X
2828(exactly)X
3105(the)X
3235(right)X
3424(moment)X
3736(to)X
3827(pro-)X
555 2196(vide)N
729(very)X
907(capable)X
1198(and)X
1347(much)X
1565(needed)X
1836(assistance)X
2210(during)X
2462(the)X
2592(alpha)X
2805(and)X
2954(beta)X
3123(releases.)X
3467(He)X
3591(is)X
3672(totally)X
555 2292(responsible)N
978(for)X
1102(the)X
1232(successfull)X
1640(integration)X
2045(of)X
2140(sources)X
2425(from)X
2618(multiple)X
2936(sites;)X
3140(we)X
3264(simply)X
3527(wouldn't)X
555 2388(have)N
743(a)X
804(release)X
1070(without)X
1362(him.)X
555 2512(Ralph)N
788(Swick)X
1030(of)X
1125(Project)X
1397(Athena)X
1673(and)X
1822(Digital)X
2090(kept)X
2264(it)X
2336(all)X
2447(together)X
2758(for)X
2882(us.)X
3026(He)X
3150(has)X
3289(handled)X
3590(literally)X
555 2608(thousands)N
929(of)X
1024(requests)X
1334(for)X
1458(people)X
1715(everywhere)X
2146(and)X
2295(saved)X
2517(the)X
2647(sanity)X
2880(of)X
2975(one)X
3124(of)X
3219(us)X
3319(\(JG\).)X
3540(His)X
3684(calm)X
555 2704(good)N
753(cheer)X
965(has)X
1104(been)X
1292(a)X
1353(foundation)X
1757(on)X
1867(which)X
2104(we)X
2228(could)X
2446(build.)X
555 2828(Special)N
837(thanks)X
1089(must)X
1283(also)X
1447(go)X
1557(to)X
1648(Sam)X
1827(Fuller,)X
2082(Vice)X
2270(President)X
2620(of)X
2715(Corporate)X
3089(Research)X
3433(at)X
3519(Digital,)X
3809(who)X
555 2924(has)N
694(remained)X
1044(committed)X
1445(to)X
1536(the)X
1666(widest)X
1918(public)X
2161 0.2898(availability)AX
2582(of)X
2677(X)X
2762(and)X
2911(who)X
3084(made)X
3297(it)X
3369(possible)X
3680(to)X
555 3020(greatly)N
822(supplement)X
1251(MIT's)X
1497(resources)X
1850(with)X
2029(the)X
2159(Digital)X
2427(staff)X
2605(in)X
2696(order)X
2903(to)X
2994(make)X
3207(version)X
3488(11)X
3598(a)X
3659(reality.)X
555 3116(Many)N
782(of)X
877(the)X
1007(people)X
1264(mentioned)X
1659(here)X
1832(are)X
1961(part)X
2120(of)X
2215(the)X
2345(Western)X
2660(Software)X
2999(Laboratory)X
3412(\(Digital)X
3709(UEG-)X
555 3212(WSL\))N
792(of)X
887(the)X
1017(ULTRIX)X
1361(Engineering)X
1814(group,)X
2063(who)X
2236(work)X
2438(for)X
2562(Smokey)X
2873(Wallace,)X
3206(who)X
3379(has)X
3518(been)X
3706(vital)X
3886(to)X
555 3308(the)N
685(project's)X
1015(success;)X
1325(additional)X
1701(others)X
1938(have)X
2126(worked)X
2411(on)X
2521(the)X
2651(toolkit,)X
2927(and)X
3076(are)X
3205(acknowledged)X
3735(there.)X
555 3432(Our)N
713(thanks)X
965(also)X
1129(go)X
1239(to)X
1330(Al)X
1440(Mento)X
1692(and)X
1841(Al)X
1951(Wojtas)X
2223(of)X
2318(Digital's)X
2649(ULTRIX)X
2993(Documentation)X
3559(Group.)X
555 3528(With)N
754(good)X
952(humor)X
1204(and)X
1353(cheer,)X
1587(they)X
1761(took)X
1940(a)X
2001(rough)X
2228(draft)X
2416(of)X
2511(the)X
2641(V11)X
2814(Xlib)X
2993(manual)X
3275(and)X
3424(made)X
3637(it)X
3709(an)X
555 3624(in\256nitely)N
897(better)X
1120(and)X
1269(more)X
1472(useful)X
1709(document.)X
2123(The)X
2282(work)X
2484(they)X
2658(have)X
2846(done)X
3039(will)X
3199(help)X
3373(many)X
3591(every-)X
555 3720(where.)N
835(We)X
979(also)X
1143(would)X
1385(like)X
1540(to)X
1631(thank)X
1849(Hal)X
1998(Murray)X
2283(\(Digital)X
2580(SRC\))X
2798(and)X
2947(Peter)X
3150(George)X
3430(\(Digital)X
3727(VMS\))X
555 3816(who)N
728(contributed)X
1152(much)X
1370(by)X
1480(proof)X
1692(reading)X
1978(this)X
2128(manual.)X
555 3940(We)N
699(would)X
941(like)X
1096(to)X
1187(thank)X
1405(Jeff)X
1558(Dike)X
1751(\(Digital)X
2048(UEG\),)X
2301(Tom)X
2490(Benson,)X
2798(Jackie)X
3040(Gran\256eld,)X
3416(and)X
3565(Vince)X
3797(Orgo-)X
555 4036(van)N
704(\(Digital)X
1001(VMS\))X
1242(who)X
1415(helped)X
1672(with)X
1851(the)X
1981(library)X
2238 0.3828(utilities)AX
2527 0.2679(implementation.)AX
555 4160(Hania)N
787(Gajewska)X
1155(\(Digital)X
1452(UEG-WSL\))X
1898(was)X
2056(instrumental)X
2520(in)X
2611(the)X
2741(semantic)X
3077(design)X
3329(of)X
3424(the)X
3554(window)X
555 4256(manager)N
880(properties)X
1254(along)X
1472(with)X
1651(Ellis)X
1836(Cohen)X
2088(of)X
2183(CMU)X
2405(and)X
2554(Siemens.)X
555 4380(Dave)N
762(Rosenthal)X
1137(of)X
1232(Sun)X
1391(Microsystems)X
1907(also)X
2071(contributed)X
2495(to)X
2586(the)X
2716(protocol)X
3032(and)X
3181(provided)X
3516(the)X
3646(sample)X
555 4476(generic)N
836(color)X
1039(frame)X
1266(buffer)X
1502(device)X
1754(dependent)X
2138(code.)X
555 4600(The)N
714(alpha)X
927(and)X
1076(beta)X
1245(test)X
1390(participants)X
1824(deserve)X
2114(recognition)X
2538(as)X
2633(well.)X
2851(It)X
2927(is)X
3008(signi\256cant)X
3398(that)X
3553(the)X
3683(bug)X
555 4696(reports)N
821(\(and)X
999(many)X
1217(\256xes\))X
1434(during)X
1686(alpha)X
1899(and)X
2048(beta)X
2217(test)X
2362(came)X
2570(almost)X
2828(exclusively)X
3252(from)X
3445(just)X
3595(a)X
3656(few)X
3809(of)X
555 4792(the)N
685(alpha)X
898(testers,)X
1167(mostly)X
1430(hardware)X
1778(vendors)X
2078(working)X
2393(on)X
2503(product)X
2794 0.2500(implementations)AX
3406(of)X
3501(X.)X
3608(The)X
3767(con-)X
555 4888(tinued)N
798(public)X
1041(contribution)X
1495(of)X
1590(vendors)X
1890(and)X
2039(universities)X
2468(is)X
2549(certainly)X
2880(to)X
2971(the)X
3101(bene\256t)X
3363(of)X
3458(the)X
3588(entire)X
3811(X)X
555 4984(community.)N
555 5108(The)N
714(Version)X
1014(11)X
1124(protocol)X
1440(was)X
1598(authored)X
1928(by)X
2038(Bob)X
2207(Schei\257er)X
2542(of)X
2637(MIT's)X
2883(Laboratory)X
3296(for)X
3420(computer)X
3775(sci-)X
555 5204(ence.)N
782(Contributers)X
1245(to)X
1336(the)X
1466(design)X
1718(were:)X

4 p
%%Page: 4 4
11 s 0 xH 0 xS 1 f
10 s
0 32(--)N
4323(--)X
11 s
843 720(Dave)N
1050(Carver)X
1311(\(Digital)X
1608(HPW\))X
843 816(Branko)N
1124(Gerovac)X
1443(\(Digital)X
1740(HPW\))X
843 912(Jim)N
993(Gettys)X
1245(\(MIT/Project)X
1732(Athena,)X
2030(Digital\))X
843 1008(Phil)N
1008(Karlton)X
1299(\(Digital)X
1596(WSL\))X
843 1104(Scott)N
1047(McGregor)X
1434(\(Digital)X
1731(SSG\))X
843 1200(Ram)N
1032(Rao)X
1196(\(Digital)X
1493(UEG\))X
843 1296(David)N
1080(Rosenthal)X
1455(\(Sun\))X
843 1392(Dave)N
1050(Winchell)X
1396(\(Digital)X
1693(UEG\))X
555 1564(Invited)N
827(reviewers)X
1190(who)X
1363(provided)X
1698(useful)X
1935(input:)X
843 1708(Andrew)N
1147(Cherenson)X
1545(\(Berkeley\))X
843 1804(Burns)N
1075(Fisher)X
1317(\(Digital\))X
843 1900(Dan)N
1011(Gar\256nkel)X
1365(\(HP\))X
843 1996(Leo)N
1002(Hourvitz)X
1337(\(Next\))X
843 2092(Brock)N
1080(Krizan)X
1341(\(HP\))X
843 2188(David)N
1080(Laidlaw)X
1391(\(Stellar\))X
843 2284(Dave)N
1050(Mellinger)X
1420(\(Interleaf\))X
843 2380(Ron)N
1012(Newman)X
1351(\(MIT\))X
843 2476(John)N
1031(Ousterhout)X
1444(\(Berkeley\))X
843 2572(Andrew)N
1147(Palay)X
1365(\(ITC)X
1558(CMU\))X
843 2668(Ralph)N
1076(Swick)X
1318(\(MIT/Project)X
1805(Athena,)X
2103(Digital\))X
843 2764(Craig)N
1061(Taylor)X
1318(\(Sun\))X
843 2860(Jeffery)N
1108(Vroom)X
1379(\(Stellar\))X
555 3004(And)N
728(of)X
823(course,)X
1096(we)X
1220(must)X
1414(acknowledge)X
1900(Paul)X
2079(Asente,)X
2367(of)X
2462(Stanford)X
2787(University,)X
3203(who)X
3376(wrote)X
3598(W,)X
3725(the)X
555 3100(predecessor)N
991(to)X
1082(X.)X
555 3224(And)N
728(thanks)X
980(must)X
1174(also)X
1338(go)X
1448(to)X
1539(MIT,)X
1766(Digital)X
2034(Equipment)X
2444(Corporation,)X
2914(and)X
3063(IBM)X
3251(for)X
3375(providing)X
3740(the)X
555 3320(environment)N
1023(where)X
1259(it)X
1331(could)X
1549(happen.)X
555 3540(Jim)N
705(Gettys)X
555 3636(Systems)N
871(Research)X
1215(Center)X
555 3732(MIT)N
738(/)X
785(Project)X
1057(Athena)X
555 3828(Digital)N
823(Equipment)X
1233(Corporation)X
555 3972(Ron)N
724(Newman)X
555 4068(Project)N
827(Athena)X
555 4164(Massachusetts)N
1085(Institute)X
1397(of)X
1492(Technology)X
555 4356(Robert)N
817(W.)X
944(Schei\257er)X
555 4452(Laboratory)N
968(for)X
1092(Computer)X
1467(Science)X
555 4548(Massachusetts)N
1085(Institute)X
1397(of)X
1492(Technology)X
555 4740(September)N
954(15,)X
1086(1987)X

1 p
%%Page: 1 5
11 s 0 xH 0 xS 1 f
10 s
0 32(--)N
4323(--)X
3 f
12 s
2076 864(Chapter)N
2441(1)X
1870 1056(Introduction)N
2416(to)X
2520(Xlib)X
1 f
11 s
555 1372(The)N
714(X)X
799(Window)X
1124(System)X
1406(is)X
1487(a)X
1548(network)X
1858(transparent)X
2271(windowing)X
2689(system)X
2956(that)X
3111(was)X
3269(designed)X
3604(at)X
3690(MIT)X
3873(and)X
555 1468(that)N
710(runs)X
883(under)X
1105(4.3BSD)X
1408(UNIX,)X
1670(ULTRIX-32,)X
2153(many)X
2371(other)X
2574(UNIX)X
2814(variants,)X
3137(VAX/VMS,)X
3585(as)X
3680(well)X
3854(as)X
555 1564(several)N
826(other)X
1029(operating)X
1384(systems.)X
555 1688(X)N
640(display)X
917(servers)X
1187(run)X
1326(on)X
1436(computers)X
1825(with)X
2004(either)X
2227(monochrome)X
2714(or)X
2809(color)X
3012(bitmap)X
3280(terminals.)X
3675(The)X
555 1784(server)N
791(distributes)X
1181(user)X
1349(input)X
1553(to)X
1644(and)X
1793(accepts)X
2074(output)X
2322(requests)X
2632(from)X
2825(various)X
3106(client)X
3325(programs)X
3679(located)X
555 1880(either)N
778(on)X
888(the)X
1018(same)X
1221(machine)X
1542(or)X
1637(elsewhere)X
2010(in)X
2101(your)X
2284(network.)X
2638(Xlib)X
2817(is)X
2898(a)X
2959(C)X
3040(subroutine)X
3434(library)X
3691(that)X
555 1976(application)N
970(programs)X
1324(\(clients\))X
1635(use)X
1774(to)X
1865(interface)X
2195(with)X
2374(the)X
2504(window)X
2809(system)X
3076(by)X
3186(means)X
3433(of)X
3528(a)X
3589(stream)X
3846(con-)X
555 2072(nection.)N
881(Although)X
1236(a)X
1297(client)X
1516(usually)X
1793(runs)X
1966(on)X
2076(the)X
2206(same)X
2409(machine)X
2730(as)X
2825(the)X
2955(X)X
3040(server)X
3276(it)X
3348(is)X
3429(talking)X
3697(to,)X
3810(this)X
555 2168(need)N
743(not)X
878(be)X
983(the)X
1113(case.)X
555 2292(This)N
734(manual)X
1016(is)X
1097(a)X
1158(descriptive)X
1567(guide)X
1785(to)X
1876(the)X
2006(low-level)X
2361(C)X
2442(language)X
2782(interface)X
3112(to)X
3203(the)X
3333(X)X
3418(Window)X
3743(System)X
555 2388(protocol.)N
915(It)X
991(is)X
1072(neither)X
1339(a)X
1400(tutorial)X
1678(nor)X
1817(a)X
1878(user)X
2046(guide)X
2264(to)X
2355(programming)X
2857(to)X
2948(the)X
3078(X)X
3163(Window)X
3488(System.)X
555 2484(Other)N
777(high-level)X
1157(abstractions)X
1600(\(for)X
1753(example,)X
2096(those)X
2304(provided)X
2639(by)X
2749(the)X
2879(toolkits)X
3167(for)X
3291(X\))X
3405(are)X
3534(built)X
3719(on)X
3829(top)X
555 2580(of)N
650(the)X
780(Xlib)X
959(library.)X
1260(For)X
1404(further)X
1665(information)X
2104(about)X
2322(these)X
2525(high-level)X
2905(libraries,)X
3238(see)X
3372(the)X
3502(appropriate)X
555 2676(toolkit)N
809(documentation.)X
1400(For)X
1544(general)X
1825(information)X
2264(about)X
2482(the)X
2612(design)X
2864(of)X
2959(X,)X
3066(see)X
3200(``The)X
3417(X)X
3502(Window)X
3827(Sys-)X
555 2772(tem.'')N
555 2896(This)N
734(manual)X
1016(assumes)X
1331(a)X
1392(basic)X
1595(understanding)X
2116(of)X
2211(a)X
2272(graphics)X
2592(window)X
2897(system)X
3164(and)X
3313(of)X
3408(the)X
3538(C)X
3619(program-)X
555 2992(ming)N
759(language.)X
1143(To)X
1263(provide)X
1554(an)X
1659(introduction)X
2113(to)X
2204(X)X
2289(programming,)X
2813(this)X
2963(chapter)X
3244(discusses:)X
10 f
555 3116(g)N
1 f
775(Overview)X
1143(of)X
1238(the)X
1368(X)X
1453(Window)X
1778(System)X
10 f
555 3240(g)N
1 f
775(Naming)X
1081(and)X
1230(argument)X
1585(conventions)X
10 f
555 3364(g)N
1 f
775(Programming)X
1282(considerations)X
3 f
555 3556(1.1.)N
731(Overview)X
1115(of)X
1210(the)X
1349(X)X
1434(Window)X
1774(System)X
1 f
555 3680(Some)N
778(of)X
873(the)X
1003(terms)X
1221(used)X
1404(in)X
1495(this)X
1645(book)X
1843(are)X
1972(unique)X
2234(to)X
2325(X,)X
2432(while)X
2650(other)X
2853(terms)X
3071(that)X
3226(are)X
3355(common)X
3686(to)X
3777(other)X
555 3776(window)N
860(systems)X
1161(have)X
1349(different)X
1674(meanings)X
2034(in)X
2125(X.)X
2254(You)X
2427(may)X
2601(\256nd)X
2760(it)X
2832(helpful)X
3104(to)X
3195(refer)X
3382(to)X
3473(the)X
3603(glossary,)X
555 3872(located)N
832(at)X
918(the)X
1048(end)X
1197(of)X
1292(the)X
1422(book,)X
1642(when)X
1854(you)X
2008(are)X
2137(uncertain)X
2487(of)X
2582(a)X
2643(term's)X
2890(meaning)X
3216(in)X
3307(the)X
3437(context)X
3719(of)X
3814(the)X
3944(X)X
555 3968(Window)N
880(System.)X
555 4092(The)N
714(X)X
799(Window)X
1124(System)X
1406(supports)X
1726(one)X
1875(or)X
1970(more)X
2173(screens)X
2453(containing)X
2848(overlapping)X
3291(windows)X
3630(or)X
3725(subwin-)X
555 4188(dows.)N
806(A)X
891(screen)X
1137(is)X
1218(a)X
1279(physical)X
1595(monitor)X
1897(and)X
2046(hardware,)X
2416(which)X
2653(may)X
2827(be)X
2932(either)X
3155(color)X
3358(or)X
3453(black)X
3666(and)X
555 4284(white.)N
817(There)X
1044(can)X
1188(be)X
1293(multiple)X
1611(screens)X
1891(per)X
2025(display)X
2302(or)X
2397(workstation.)X
555 4408(All)N
690(the)X
820(windows)X
1159(in)X
1250(an)X
1355(X)X
1440(server)X
1676(are)X
1805(arranged)X
2134(in)X
2225(a)X
2286(strict)X
2485(hierarchy.)X
2883(At)X
2993(the)X
3123(top)X
3258(of)X
3353(the)X
3483(hierarchy)X
3837(are)X
555 4504(the)N
685(root)X
849(windows,)X
1210(which)X
1447(cover)X
1664(each)X
1847(of)X
1942(the)X
2072(display)X
2349(screens.)X
2673(Each)X
2871(root)X
3035(window)X
3340(is)X
3421(partially)X
3738(or)X
555 4600(completely)N
970(covered)X
1270(by)X
1380(child)X
1579(windows.)X
1962(All)X
2097(windows,)X
2458(except)X
2710(for)X
2834(root)X
2998(windows,)X
3359(have)X
3547(parents.)X
555 4696(There)N
782(is)X
863(usually)X
1140(at)X
1226(least)X
1410(one)X
1559(window)X
1864(per)X
1998(application)X
2413(program.)X
2777(Child)X
2996(windows)X
3357(may)X
3531(in)X
3622(turn)X
3786(have)X
555 4792(their)N
739(own)X
912(children.)X
1267(In)X
1362(this)X
1512(way,)X
1702(an)X
1807(application)X
2222(program)X
2542(can)X
2686(create)X
2918(a)X
2979(tree)X
3133(of)X
3228(arbitrary)X
3553(depth)X
3771(on)X
555 4888(each)N
738(screen.)X
555 5012(A)N
640(child)X
839(window)X
1144(may)X
1318(be)X
1423(larger)X
1650(than)X
1824(its)X
1930(parent.)X
2194(That)X
2378(is,)X
2481(part)X
2640(or)X
2735(all)X
2846(of)X
2941(the)X
3071(child)X
3270(window)X
3575(may)X
3749(extend)X
555 5108(beyond)N
836(the)X
966(boundaries)X
1374(of)X
1469(the)X
1599(parent.)X
1885(However,)X
2250(all)X
2361(output)X
2609(to)X
2700(a)X
2761(window)X
3066(is)X
3147(clipped)X
3429(by)X
3539(the)X
3669(boun-)X
555 5204(daries)N
787(of)X
882(its)X
988(parent)X
1230(window.)X
1579(If)X
1659(several)X
1930(children)X
2241(of)X
2336(a)X
2397(window)X
2702(have)X
2890(overlapping)X
3333(locations,)X
3696(one)X
3845(of)X
555 5300(the)N
685(children)X
996(is)X
1077(considered)X
1480(to)X
1571(be)X
1676(on)X
1786(top)X
1921(of)X
2016(or)X
2111(raised)X
2343(over)X
2521(the)X
2651(others,)X
2910(obscuring)X
3279(them.)X
3522(Output)X
3789(to)X
555 5396(areas)N
757(covered)X
1057(by)X
1167(other)X
1370(windows)X
1709(will)X
1869(be)X
1974(suppressed)X
2381(by)X
2491(the)X
2621(window)X
2926(system.)X
3237(If)X
3317(a)X
3378(window)X
3705(is)X
555 5492(obscured)N
894(by)X
1004(a)X
1065(second)X
1331(window,)X
1658(the)X
1788(window)X
2093(will)X
2253(obscure)X
2548(only)X
2727(those)X
2935(ancestors)X
3284(of)X
3379(the)X
3509(second)X
3775(win-)X
555 5588(dow,)N
750(which)X
987(are)X
1116(also)X
1280(ancestors)X
1629(of)X
1724(the)X
1854(\256rst)X
2013(window.)X
555 5712(A)N
640(window)X
945(has)X
1084(a)X
1145(border)X
1396(of)X
1491(zero)X
1664(or)X
1759(more)X
1962(pixels)X
2195(in)X
2286(width,)X
2531(which)X
2768(can)X
2912(be)X
3017(any)X
3166(pattern)X
3433(\(pixmap\))X
3778(or)X
555 5808(solid)N
749(color)X
952(you)X
1106(like.)X
1305(A)X
1390(window)X
1695(usually)X
1972(but)X
2107(not)X
2242(always)X
2508(has)X
2647(a)X
2708(background)X
3145(pattern)X
3412(which)X
3649(will)X
3809(be)X
3 f
2261 6144(1)N

2 p
%%Page: 2 6
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(repainted)N
905(by)X
1015(the)X
1145(window)X
1450(system)X
1717(when)X
1929(uncovered.)X
2361(Each)X
2559(window)X
2864(has)X
3003(its)X
3109(own)X
3282(coordinate)X
3676(system.)X
555 768(Child)N
774(windows)X
1113(obscure)X
1408(their)X
1592(parents)X
1868(unless)X
2110(the)X
2240(child)X
2439(windows)X
2778(have)X
2966(no)X
3076(b)X
3142(ackground,)X
3557(and)X
3706(graphic)X
555 864(operations)N
944(in)X
1035(the)X
1165(parent)X
1407(window)X
1712(usually)X
1989(are)X
2118(clipped)X
2400(by)X
2510(the)X
2640(children.)X
555 988(Input)N
763(from)X
956(X)X
1041(takes)X
1244(the)X
1374(form)X
1567(of)X
1662(events.)X
1953(Events)X
2215(may)X
2389(either)X
2612(be)X
2717(side)X
2881(effects)X
3137(of)X
3232(a)X
3293(command)X
3663(\(for)X
555 1084(example,)N
898(restacking)X
1282(windows)X
1621(generates)X
1975(exposure)X
2314(events\))X
2590(or)X
2685(completely)X
3100(asynchronous)X
3605(\(for)X
3758(exam-)X
555 1180(ple,)N
707(the)X
837(keyboard\).)X
1259(A)X
1344(client)X
1563(program)X
1883(asks)X
2056(to)X
2147(be)X
2252(informed)X
2597(of)X
2692(events.)X
2983(X)X
3068(never)X
3285(sends)X
3502(events)X
3749(a)X
3810(pro-)X
555 1276(gram)N
758(did)X
893(not)X
1028(ask)X
1167(for.)X
555 1400(X)N
640(does)X
823(not)X
958(take)X
1127(responsibility)X
1630(for)X
1754(the)X
1884(contents)X
2200(of)X
2295(windows.)X
2656(When)X
2888(part)X
3047(or)X
3142(all)X
3253(of)X
3348(a)X
3409(window)X
3714(is)X
3795(hid-)X
555 1496(den)N
704(and)X
853(then)X
1027(brought)X
1323(back)X
1511(onto)X
1690(the)X
1820(screen,)X
2088(its)X
2194(contents)X
2510(may)X
2684(be)X
2789(lost,)X
2961(and)X
3110(the)X
3240(client)X
3459(program)X
3779(is)X
555 1592(noti\256ed)N
847(\(by)X
986(an)X
1091(exposure)X
1430(event\))X
1672(that)X
1827(part)X
1986(or)X
2081(all)X
2192(of)X
2287(the)X
2417(window)X
2722(needs)X
2944(to)X
3035(be)X
3140(repainted.)X
3534(Programs)X
555 1688(must)N
749(be)X
854(prepared)X
1183(to)X
1274(regenerate)X
1662(the)X
1792(contents)X
2108(of)X
2203(windows)X
2542(on)X
2652(demand.)X
555 1812(X)N
640(also)X
804(provides)X
1129(off)X
1253(screen)X
1499(storage)X
1775(of)X
1870(graphics)X
2190(objects,)X
2484(called)X
2717(``pixmaps.'')X
3176(Single)X
3424(plane)X
3637(\(depth)X
3884(1\))X
555 1908(pixmaps)N
876(are)X
1005(sometimes)X
1405(referred)X
1704(to)X
1795(as)X
1890(``bitmaps.'')X
2330(These)X
2562(can)X
2706(be)X
2811(used)X
2994(in)X
3085(most)X
3279(graphics)X
3599(functions)X
555 2004(interchangably)N
1101(with)X
1280(windows,)X
1641(and)X
1790(are)X
1919(used)X
2102(in)X
2193(various)X
2474(graphics)X
2794(operations)X
3183(to)X
3274(de\256ne)X
3511(patterns)X
3812(also)X
555 2100(called)N
788(``tiles.'')X
1096(Windows)X
1455(and)X
1604(pixmaps)X
1925(together)X
2236(are)X
2365(referred)X
2664(to)X
2755(as)X
2850(``drawables.'')X
555 2224(Most)N
758(of)X
853(the)X
983(functions)X
1333(in)X
1424(Xlib)X
1603(just)X
1753(add)X
1902(requests)X
2212(to)X
2303(an)X
2408(output)X
2656(buffer.)X
2936(These)X
3168(requests)X
3478(later)X
3657(execute)X
555 2320(asynchronously)N
1129(on)X
1239(the)X
1369(X)X
1454(server,)X
1712(often)X
1915(referred)X
2214(to)X
2305(as)X
2400(display)X
2677(server.)X
2957(Functions)X
3327(that)X
3482(return)X
3714(values)X
555 2416(of)N
650(information)X
1089(stored)X
1326(in)X
1417(the)X
1547(server)X
1783(do)X
1893(not)X
2028(return)X
2260(\(that)X
2444(is,)X
2547(they)X
2721(``block''\))X
3084(until)X
3269(an)X
3374(explicit)X
3662(reply)X
3865(is)X
555 2512(received)N
875(or)X
970(an)X
1075(error)X
1267(occurs.)X
1562(If)X
1642(a)X
1703(nonblocking)X
2188(call)X
2338(results)X
2590(in)X
2681(an)X
2786(error,)X
3000(the)X
3130(error)X
3322(will)X
3482(generally)X
3832(not)X
555 2608(be)N
660(reported)X
975(by)X
1085(a)X
1146(call)X
1296(to)X
1387(an)X
1492(optional)X
1804(error)X
1996(handler)X
2282(until)X
2467(some)X
2675(later,)X
2876(blocking)X
3207(call)X
3357(is)X
3438(made.)X
555 2732(If)N
635(it)X
707(does)X
890(not)X
1025(want)X
1218(a)X
1279(request)X
1555(to)X
1646(execute)X
1937(asynchronously,)X
2533(a)X
2594(client)X
2813(can)X
2957(follow)X
3209(it)X
3281(with)X
3460(a)X
3521(call)X
3671(to)X
5 f
570 2828(XSync)N
1 f
840(,)X
884(which)X
1121(will)X
1281(block)X
1499(until)X
1684(all)X
1795(previously)X
2189(buffered)X
2508(asynchronous)X
3013(events)X
3260(have)X
3448(been)X
3636(sent)X
3800(and)X
555 2924(acted)N
763(on.)X
917(As)X
1036(an)X
1141(important)X
1507(side)X
1671(effect,)X
1915(the)X
2045(output)X
2293(buffer)X
2529(is)X
2610(always)X
2876(\257ushed)X
3152(by)X
3262(a)X
3323(call)X
3473(to)X
3564(any)X
3713(function)X
555 3020(which)N
792(returns)X
1058(a)X
1119(value)X
1332(or)X
1427(waits)X
1635(for)X
1759(input)X
1963(\(for)X
2116(example,)X
5 f
2477(XPending)X
1 f
2874(,)X
5 f
2936(XNextEvent)X
1 f
3417(,)X
5 f
3479(XWin-)X
555 3116(dowEvent)N
1 f
957(,)X
5 f
1019(XFlush)X
1 f
1308(,)X
1352(or)X
5 f
1465(XSync)X
1 f
1735(\).)X
555 3240(Many)N
782(Xlib)X
961(functions)X
1311(will)X
1471(return)X
1703(an)X
1808(integer)X
2075(resource)X
2394(ID,)X
2530(which)X
2767(allows)X
3019(you)X
3173(to)X
3264(refer)X
3451(to)X
3542(objects)X
555 3336(stored)N
792(on)X
902(the)X
1032(X)X
1117(server.)X
1397(These)X
1629(can)X
1773(be)X
1878(of)X
1973(type)X
2147(Window,)X
2494(Font,)X
2700(Pixmap,)X
3014(Bitmap,)X
3319(Cursor,)X
3602(and)X
3751(GCon-)X
555 3432(text,)N
732(as)X
827(de\256ned)X
1108(in)X
1199(the)X
1329(\256le)X
5 f
1482(<X11/X.h>)X
1 f
1914(.)X
1980(These)X
2212(resources)X
2565(are)X
2694(created)X
2970(by)X
3080(user)X
3248(requests,)X
3580(and)X
3729(des-)X
555 3528(troyed)N
802(\(or)X
926(freed\))X
1157(by)X
1267(user)X
1435(requests)X
1745(or)X
1840(when)X
2052(connections)X
2495(closed.)X
2786(Most)X
2989(of)X
3084(these)X
3287(resources)X
3640(are)X
3769(poten-)X
555 3624(tially)N
760(sharable)X
1075(between)X
1390(applications,)X
1861(and)X
2010(in)X
2101(fact,)X
2277(windows)X
2616(are)X
2745(manipulated)X
3204(explicitly)X
3561(by)X
3671(window)X
555 3720(manager)N
880(programs.)X
1278(Fonts)X
1496(and)X
1645(cursors)X
1920(are)X
2049(typically)X
2381(shared)X
2632 0.2604(automatically)AX
3136(since)X
3339(the)X
3469(X)X
3554(server)X
3790(treats)X
555 3816(fonts)N
753(specially,)X
1111(loading)X
1398(and)X
1547(unloading)X
1922(font)X
2086(storage)X
2362(as)X
2457(needed.)X
2772(GContexts)X
3171(should)X
3428(not)X
3563(be)X
3668(shared)X
555 3912(between)N
870(applications.)X
555 4036(Some)N
778(functions)X
1128(return)X
1360(Status,)X
1620(an)X
1725(integer)X
1992(error)X
2184(indication.)X
2604(If)X
2684(the)X
2814(function)X
3130(fails,)X
3326(it)X
3398(will)X
3558(return)X
3790(a)X
3851(zero)X
555 4132(\(0\).)N
723(If)X
803(the)X
933(function)X
1249(returns)X
1515(a)X
1576(status)X
1799(of)X
1894(zero)X
2067(\(0\),)X
2213(it)X
2285(has)X
2424(not)X
2559(updated)X
2860(the)X
2990(return)X
3222(parameters.)X
3674(Because)X
555 4228(C)N
636(does)X
819(not)X
954(provide)X
1245(multiple)X
1563(return)X
1795(values,)X
2064(many)X
2282(function)X
2598(must)X
2792(return)X
3024(their)X
3208(results)X
3460(by)X
3570(writing)X
3847(into)X
555 4324(client-passed)N
1037(storage.)X
1335(Any)X
1508(pointer)X
1780(that)X
1935(is)X
2016(used)X
2199(to)X
2290(return)X
2522(a)X
2583(value)X
2796(is)X
2877(designated)X
3276(as)X
3371(such)X
3554(by)X
3664(the)X
2 f
555 4420(_return)N
1 f
841(suf\256x)X
1063(as)X
1158(part)X
1317(of)X
1412(its)X
1518(name.)X
1775(All)X
1910(other)X
2113(pointers)X
2419(passed)X
2675(to)X
2766(these)X
2969(functions)X
3319(are)X
3448(used)X
3631(for)X
3755(read-)X
555 4516(ing)N
690(only.)X
913(By)X
1038(default,)X
1327(errors)X
1553(are)X
1682(handled)X
1983(either)X
2206(by)X
2316(a)X
2377(standard)X
2697(library)X
2954(function)X
3270(or)X
3365(by)X
3475(one)X
3624(that)X
3779(you)X
555 4612(provide.)N
890(Functions)X
1260(that)X
1415(return)X
1647(pointers)X
1953(to)X
2044(strings)X
2301(will)X
2461(return)X
2693(NULL)X
2949(pointers)X
3255(if)X
3331(the)X
3461(string)X
3684(does)X
3867(not)X
555 4708(exist.)N
555 4832(Input)N
763(events)X
1010(\(for)X
1163(example,)X
1506(key)X
1655(pressed)X
1940(or)X
2035(mouse)X
2287(moved\))X
2578(arrive)X
2805(asynchronously)X
3379(from)X
3572(the)X
3702(server)X
555 4928(and)N
704(are)X
833(queued)X
1109(until)X
1294(they)X
1468(are)X
1597(requested)X
1956(by)X
2066(a)X
2127(call)X
2277(to)X
5 f
2386(XNextEvent)X
1 f
2889(or)X
5 f
3002(XWindowEvent)X
1 f
3614(.)X
3680(In)X
3775(addi-)X
555 5024(tion,)N
737(some)X
945(of)X
1040(the)X
1170(library)X
1427(functions)X
1777(\(for)X
1930(example,)X
5 f
2291(XResizeWindow)X
1 f
2967(and)X
5 f
3134(XRaiseWindow)X
1 f
3744(\))X
3795(gen-)X
555 5120(erate)N
748(exposure)X
1087(events)X
1334(\(that)X
1518(is,)X
1621(requests)X
1931(to)X
2022(repaint)X
2289(sections)X
2595(of)X
2690(a)X
2751(window)X
3056(that)X
3211(do)X
3321(not)X
3456(have)X
3644(valid)X
3843(con-)X
555 5216(tents\).)N
795(These)X
1027(events)X
1274(also)X
1438(arrive)X
1665(asynchronously,)X
2261(but)X
2396(the)X
2526(client)X
2745(may)X
2919(wish)X
3107(to)X
3198(explicitly)X
3555(wait)X
3729(for)X
555 5312(them)N
754(by)X
864(calling)X
5 f
1145(XSync)X
1 f
1437(after)X
1620(calling)X
1883(a)X
1944(function)X
2260(which)X
2497(may)X
2671(generate)X
2991(exposure)X
3330(events.)X
7 s
10 f
555 5520(h)N
571(hhhhhhhhhhhhhhhhhhhh)X
1 f
555 5592(The)N
660(<)X
701(>)X
751(has)X
844(the)X
931(meaning)X
1143(de\256ned)X
1326(by)X
1400(the)X
1487(#)X
1533(include)X
1717(statement)X
1952(of)X
2016(the)X
2102(C)X
2156(compiler)X
2374(and)X
2472(is)X
2527(a)X
2569(\256le)X
2658(relative)X
2845(to)X
2906(a)X
2948(well)X
3062(known)X
3231(directory.)X
3480(On)X
3565(UNIX-)X
555 5664(based)N
697(systems,)X
904(this)X
1000(is)X
5 f
1063(/usr/include)X
1 f
1358(.)X
3 f
11 s
2261 6144(2)N

3 p
%%Page: 3 7
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(1.2.)N
731(Naming)X
1051(and)X
1215(Argument)X
1622(Conventions)X
2113(within)X
2375(Xlib)X
1 f
555 796(Throughout)N
993(Xlib,)X
1194(a)X
1255(number)X
1546(of)X
1641(conventions)X
2089(for)X
2213(naming)X
2500(and)X
2649(syntax)X
2901(of)X
2996(the)X
3126(Xlib)X
3305(functions)X
3655(have)X
3843(been)X
555 892(followed.)N
934(These)X
1166(conventions)X
1614(are)X
1743(intended)X
2069(to)X
2160(make)X
2373(the)X
2503(syntax)X
2755(of)X
2850(the)X
2980(functions)X
3330(more)X
3533(predictable,)X
555 988(given)N
773(that)X
928(you)X
1082(remember)X
1461(what)X
1654(information)X
2093(the)X
2223(routine)X
2495(may)X
2669(require.)X
555 1112(The)N
714(major)X
942(naming)X
1229(conventions)X
1677(are:)X
10 f
555 1236(g)N
1 f
775(To)X
895(better)X
1118(differentiate)X
1571(the)X
1701(X)X
1786(symbols)X
2102(from)X
2295(the)X
2425(user)X
2593(symbols,)X
2931(the)X
3061(library)X
3318(uses)X
3491(mixed)X
3734(case)X
3907(for)X
775 1332(external)N
1081(symbols,)X
1419(and)X
1568(leaves)X
1810(lower)X
2032(case)X
2205(for)X
2329(variables)X
2669(and)X
2818(all)X
2929(upper)X
3151(case)X
3324(for)X
3448(user)X
3616(macros,)X
775 1428(per)N
909(existing)X
1211(convention.)X
10 f
555 1552(g)N
1 f
775(All)X
910(Xlib)X
1089(functions)X
1439(begin)X
1657(with)X
1836(a)X
1897(capital)X
2155(X.)X
10 f
555 1676(g)N
1 f
775(The)X
934(beginnings)X
1343(of)X
1438(all)X
1549(procedure)X
1922(names)X
2169(and)X
2318(symbols)X
2634(are)X
2763(capitalized.)X
10 f
555 1800(g)N
1 f
775(All)X
910(user-visible)X
1343(data)X
1512(structures)X
1876(begin)X
2094(with)X
2273(a)X
2334(capital)X
2592(X.)X
2721(More)X
2933(generally,)X
3305(anything)X
3636(that)X
3791(a)X
3852(user)X
775 1896(might)N
1004(dereference)X
1435(begins)X
1687(with)X
1866(an)X
1971(capital)X
2229(X.)X
10 f
555 2020(g)N
1 f
775(Macros)X
1060(and)X
1209(other)X
1412(symbols)X
1728(do)X
1838(not)X
1973(begin)X
2191(with)X
2370(a)X
2431(capital)X
2689(X.)X
2818(To)X
2938(distinguish)X
3348(them)X
3547(from)X
3740(all)X
3851(user)X
775 2116(symbols,)N
1113(each)X
1296(word)X
1498(in)X
1589(the)X
1719(macro)X
1961(is)X
2042(capitalized.)X
10 f
555 2240(g)N
1 f
775(All)X
910(elements)X
1268(of)X
1363(or)X
1458(variables)X
1798(in)X
1889(a)X
1950(data)X
2119(structure)X
2449(are)X
2578(in)X
2669(lower)X
2891(case.)X
3108(Compound)X
3522(words,)X
3780(where)X
775 2336(needed,)N
1068(are)X
1197(constructed)X
1625(with)X
1804(underscores)X
2245(\(_\).)X
10 f
555 2460(g)N
1 f
775(The)X
934(display)X
1211(argument,)X
1588(where)X
1824(used,)X
2029(is)X
2110(always)X
2376(\256rst)X
2535(in)X
2626(the)X
2756(argument)X
3111(list.)X
10 f
555 2584(g)N
1 f
775(All)X
910(resource)X
1229(objects,)X
1523(where)X
1759(used,)X
1964(occur)X
2181(at)X
2267(the)X
2397(beginning)X
2772(of)X
2867(the)X
2997(argument)X
3352(list,)X
3505 0.3187(immediately)AX
775 2680(after)N
958(the)X
1088(display)X
1365(variable.)X
10 f
555 2804(g)N
1 f
775(When)X
1007(a)X
1090(graphics)X
1410(context)X
1692(is)X
1773(present)X
2049(together)X
2360(with)X
2539(another)X
2825(type)X
2999(of)X
3094(resource)X
3413(\(most)X
3636(com-)X
775 2900(monly,)N
1045(a)X
1106(drawable\),)X
1501(the)X
1631(graphics)X
1951(context)X
2233(occurs)X
2484(in)X
2575(the)X
2705(argument)X
3060(list)X
3191(after)X
3374(the)X
3504(other)X
775 2996(resource.)N
1138(Drawables)X
1535(outrank)X
1826(all)X
1937(other)X
2140(resources.)X
10 f
555 3120(g)N
1 f
775(Source)X
1041(arguments)X
1430(always)X
1696(precede)X
1991(the)X
2121(destination)X
2531(arguments)X
2920(in)X
3011(the)X
3141(argument)X
3496(list.)X
10 f
555 3244(g)N
1 f
775(The)X
934(x)X
1000(argument)X
1355(always)X
1621(precedes)X
1950(the)X
2080(y)X
2146(argument)X
2501(in)X
2592(the)X
2722(argument)X
3077(list.)X
10 f
555 3368(g)N
1 f
775(The)X
934(width)X
1157(argument)X
1512(always)X
1778(precedes)X
2107(the)X
2237(height)X
2480(argument)X
2835(in)X
2926(the)X
3056(argument)X
3411(list.)X
10 f
555 3492(g)N
1 f
775(Where)X
1031(the)X
1161(x,)X
1249(y,)X
1337(width)X
1560(and)X
1709(height)X
1952(arguments)X
2341(are)X
2470(used)X
2653(together,)X
2986(the)X
3116(x)X
3182(and)X
3331(y)X
3397(arguments)X
775 3588(always)N
1041(precede)X
1336(the)X
1466(width)X
1689(and)X
1838(height)X
2081(arguments.)X
10 f
555 3712(g)N
1 f
775(Where)X
1031(an)X
1136(array)X
1338(occurs)X
1589(in)X
1680(an)X
1785(argument)X
2140(list)X
2271(accompanied)X
2758(with)X
2937(a)X
2998(count)X
3216(\(number)X
3536(of)X
3631(elements)X
775 3808(in)N
866(the)X
996(array\),)X
1249(the)X
1379(array)X
1581(always)X
1847(precedes)X
2176(the)X
2306(count)X
2524(in)X
2615(the)X
2745(argument)X
3100(list.)X
10 f
555 3932(g)N
1 f
775(Where)X
1031(a)X
1092(mask)X
1300(is)X
1381(accompanied)X
1868(with)X
2047(a)X
2108(structure,)X
2460(the)X
2590(mask)X
2798(always)X
3064(precedes)X
3393(the)X
3523(pointer)X
3795(to)X
3886(the)X
775 4028(structure)N
1105(in)X
1196(the)X
1326(argument)X
1681(list.)X
3 f
555 4220(1.3.)N
731(Programming)X
1281(Considerations)X
1 f
555 4344(The)N
714(major)X
942(considerations)X
1473(are:)X
10 f
555 4468(g)N
1 f
775(Keyboards)X
1177(are)X
1306(the)X
1436(greatest)X
1732(variable)X
2038(between)X
2353(different)X
2678(manufacturer's)X
3232(workstations.)X
3748(If)X
3828(you)X
775 4564(want)N
968(your)X
1151(program)X
1471(to)X
1562(be)X
1667(portable,)X
2000(you)X
2154(should)X
2411(be)X
2516(particularly)X
2945(conservative)X
3412(here.)X
10 f
555 4688(g)N
1 f
775(Many)X
1002(display)X
1279(systems)X
1580(have)X
1768(limited)X
2042(amounts)X
2363(of)X
2458(off-screen)X
2835(memory.)X
3195(If)X
3275(you)X
3429(can,)X
3595(you)X
3749(should)X
775 4784(minimize)N
1132(use)X
1271(of)X
1366(pixmaps)X
1687(and)X
1836(backing)X
2137(store.)X
10 f
555 4908(g)N
1 f
775(The)X
934(user)X
1102(should)X
1359(have)X
1547(control)X
1819(of)X
1914(his)X
2039(screen)X
2285(real-estate.)X
2713(Therefore,)X
3103(you)X
3257(should)X
3514(write)X
3717(your)X
775 5004(applications)N
1224(to)X
1315(react)X
1508(to)X
1599(window)X
1904(management,)X
2399(rather)X
2626(than)X
2800(presume)X
3120(control)X
3392(of)X
3487(the)X
3617(entire)X
775 5100(screen.)N
1065(What)X
1278(you)X
1432(do)X
1542(inside)X
1775(of)X
1870(your)X
2053(top-level)X
2389(window,)X
2716(however,)X
3062(is)X
3143(up)X
3253(to)X
3344(your)X
3527(application.)X
775 5196(There)N
1002(is)X
1083(more)X
1286(on)X
1396(this)X
1546(topic)X
1745(elsewhere)X
2118(in)X
2209(the)X
2339(book.)X
3 f
555 5388(1.4.)N
731(Conventions)X
1222(Used)X
1429(in)X
1525(this)X
1684(Manual)X
1 f
555 5512(This)N
734(manual)X
1016(is)X
1097(practical,)X
1445(descriptive)X
1854(guide)X
2072(to)X
2163(all)X
2274(the)X
2404(C)X
2485(language)X
2825(functions)X
3175(in)X
3266(the)X
3396(Xlib)X
3575(library.)X
555 5608(That)N
739(is,)X
842(it)X
914(is)X
995(organized)X
1364(by)X
1474(practical)X
1800(X)X
1885(programming)X
2387(tasks)X
2585(and)X
2734(describes)X
3083(each)X
3266(function)X
3582(in)X
3673(the)X
555 5704(library.)N
856(All)X
991(functions)X
1341(that)X
1496(are)X
1625(related)X
1887(to)X
1978(a)X
2039(given)X
2257(programming)X
2759(task)X
2923(are)X
3052(discussed)X
3411(in)X
3502(the)X
3632(same)X
555 5800(chapter)N
836(or)X
931(section,)X
1225(and)X
1374(each)X
1557(provides)X
1882(all)X
1993(the)X
2123(related)X
2385(background)X
2822(information)X
3261(that)X
3416(is)X
3497(needed)X
3768(for)X
3 f
2261 6144(3)N

4 p
%%Page: 4 8
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(that)N
710(discussion.)X
1143(Although)X
1498(you)X
1652(can)X
1796(read)X
1969(the)X
2099(chapters)X
2414(of)X
2509(this)X
2659(manual)X
2941(in)X
3032(any)X
3181(order,)X
3410(you)X
3564(should)X
3821(start)X
555 768(by)N
665(reading)X
951(Chapter)X
1252(2,)X
1340(which)X
1577(explains)X
1893(how)X
2066(to)X
2157(connect)X
2453(your)X
2636(display)X
2913(to)X
3004(the)X
3134(X)X
3219(server.)X
555 892(The)N
714(major)X
942(documentation)X
1489(conventions)X
1937(are:)X
10 f
555 1016(g)N
1 f
775(Global)X
1037(symbols)X
1353(in)X
1444(this)X
1594(manual)X
1876(are)X
2005(printed)X
2277(in)X
2368(this)X
5 f
2536(special)X
1 f
2846(font.)X
3054(These)X
3286(can)X
3430(be)X
3535(either)X
3758(pro-)X
775 1112(cedure)N
1031(names,)X
1300(symbols)X
1616(de\256ned)X
1897(in)X
1988(include)X
2270(\256les,)X
2461(or)X
2556(structure)X
2886(names.)X
3177(Arguments,)X
3612(user-)X
775 1208(supplied)N
1096(variables,)X
1458(are)X
1587(printed)X
1859(in)X
2 f
1950(italics)X
1 f
2167(.)X
10 f
555 1332(g)N
1 f
775(Most)X
978(procedures)X
1385(are)X
1514(introduced)X
1913(by)X
2023(a)X
2084(general)X
2365(discussion)X
2754(that)X
2909(may)X
3083(be)X
3188(used)X
3371(to)X
3462(distinguish)X
3872(this)X
775 1428(procedure)N
1148(from)X
1341(other)X
1544(procedures)X
1951(and)X
2100(are)X
2229(followed)X
2564(by)X
2674(the)X
2804(procedure)X
3177(declaration)X
3591(itself.)X
3834(Each)X
775 1524(argument)N
1130(is)X
1211(then)X
1385(speci\256cally)X
1809(explained.)X
2218(General)X
2518(discussion)X
2907(of)X
3002(the)X
3132(procedure,)X
3527(if)X
3603(any)X
3752(is)X
775 1620(required,)N
1112(follows)X
1398(the)X
1528(arguments.)X
1961(Where)X
2217(applicable,)X
2624(the)X
2754(last)X
2899(paragraph)X
3272(of)X
3367(given)X
3585(explanation)X
775 1716(lists)N
940(the)X
1070(possible)X
1381(Xlib)X
1560(error)X
1752(codes)X
1974(that)X
2129(can)X
2273(be)X
2378(generated)X
2742(by)X
2852(that)X
3007(function.)X
3367(See)X
3516(Section)X
775 1812(8.10.2)N
1017(for)X
1141(a)X
1202(complete)X
1548(discussion)X
1937(of)X
2032(the)X
2162(Xlib)X
2341(error)X
2533(codes.)X
10 f
555 1936(g)N
1 f
775(To)X
895(eliminate)X
1247(any)X
1396(ambiguity)X
1777(between)X
2092(those)X
2300(arguments)X
2689(that)X
2844(you)X
2998(pass)X
3171(and)X
3320(those)X
3528(that)X
3683(a)X
3744(func-)X
775 2032(tion)N
935(returns)X
1201(to)X
1292(you,)X
1468(the)X
1598(explanations)X
2066(for)X
2190(all)X
2301(arguments)X
2690(that)X
2845(you)X
2999(pass)X
3172(start)X
3346(with)X
3525(the)X
3655(word)X
775 2128(``speci\256es''.)N
1260(By)X
1385(contrast,)X
1708(the)X
1838(explanations)X
2306(for)X
2430(all)X
2541(arguments)X
2930(that)X
3085(are)X
3214(returned)X
3529(to)X
3620(you)X
3774(start)X
775 2224(with)N
954(the)X
1084(word)X
1286(``returns''.)X
3 f
2261 6144(4)N

5 p
%%Page: 5 9
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2186 864(Chapter)N
2551(2)X
2024 1056(Display)N
2357(Functions)X
1 f
11 s
555 1372(Before)N
816(your)X
999(program)X
1319(can)X
1463(use)X
1602(a)X
1663(display,)X
1962(you)X
2116(must)X
2310(establish)X
2641(a)X
2702(connection)X
3111(to)X
3202(the)X
3332(X)X
3417(server)X
3653(driving)X
555 1468(your)N
738(display.)X
1059(Once)X
1266(you)X
1420(have)X
1608(established)X
2022(a)X
2083(connection,)X
2514(you)X
2668(then)X
2842(can)X
2986(use)X
3125(the)X
3255(Xlib)X
3434(macros)X
3710(and)X
555 1564(functions)N
905(discussed)X
1264(in)X
1355(this)X
1505(chapter)X
1786(to)X
1877(return)X
2109(information)X
2548(about)X
2766(the)X
2896(display.)X
3217(This)X
3396(chapter)X
3677(discusses)X
555 1660(how)N
728(to:)X
10 f
555 1784(g)N
1 f
775(Open)X
987(\(connect\))X
1341(the)X
1471(display)X
10 f
555 1908(g)N
1 f
775(Obtain)X
1037(information)X
1476(about)X
1694(the)X
1824(display,)X
2123(image)X
2361(formats,)X
2674(or)X
2769(a)X
2830(screen)X
10 f
555 2032(g)N
1 f
775(Generate)X
1114(a)X
1175(NoOperation)X
1656(protocol)X
1972(request)X
10 f
555 2156(g)N
1 f
775(Free)X
953(client-created)X
1455(data)X
10 f
555 2280(g)N
1 f
775(Close)X
998(\(disconnect\))X
1455(a)X
1516(display)X
555 2404(Finally,)N
850(the)X
980(chapter)X
1261(concludes)X
1635(with)X
1814(a)X
1875(section)X
2147(that)X
2302(describes)X
2651(the)X
2781(operations)X
3170(that)X
3325(occur)X
3542(when)X
3754(the)X
555 2500(connection)N
964(to)X
1055(the)X
1185(X)X
1270(server)X
1506(is)X
1587(closed.)X
3 f
555 2692(2.1.)N
731(Opening)X
1077(the)X
1216(Display)X
1 f
555 2816(Use)N
5 f
731(XOpenDisplay)X
1 f
1330(to)X
1421(open)X
1614(a)X
1675(connection)X
2084(to)X
2175(the)X
2305(X)X
2390(server)X
2626(controlling)X
3036(the)X
3166(speci\256ed)X
3501(display.)X
3822(X)X
555 2912(servers)N
825(may)X
999(implement)X
1400(various)X
1681(types)X
1889(of)X
1984(access)X
2230(control)X
2502(mechanisms.)X
3004(See)X
3153(section)X
3425(7.11)X
3601(for)X
3725(infor-)X
555 3008(mation)N
823(about)X
1041(access)X
1287(control.)X
1603(The)X
1762(de\256nition)X
2123(for)X
2247(this)X
2397(function)X
2713(is:)X
555 3160(Display)N
851(*XOpenDisplay)X
1429(\()X
2 f
1465(display_name)X
1 f
1961(\))X
687 3256(char)N
860(*)X
2 f
(display_name)S
1 f
1400(;)X
2 f
555 3408(display_name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(hardware)X
1949(display)X
2226(name,)X
2461(which)X
2698(determines)X
3107(the)X
3237(display)X
3514(and)X
3663(commun-)X
1131 3504(ications)N
1428(domain)X
1715(to)X
1806(be)X
1911(used.)X
2138(On)X
2267(a)X
2328(UNIX-based)X
2797(system,)X
3086(if)X
3162(the)X
3292(display_name)X
3804(is)X
1131 3600(NULL,)N
1409(it)X
1481(defaults)X
1782(to)X
1873(the)X
2003(DISPLAY)X
2395(environment)X
2863(variable.)X
3191(On)X
3320(all)X
3431(non-UNIX)X
3832(sys-)X
1131 3696(tems,)N
1342(see)X
1476(the)X
1606(Xlib)X
1785(manual)X
2067(associated)X
2451(with)X
2630(your)X
2813(operating)X
3168(system)X
3435(to)X
3526(determine)X
3901(the)X
1131 3792(default)N
1398(for)X
1522(this)X
1672(argument.)X
555 3916(On)N
684(UNIX-based)X
1153(systems,)X
1476(the)X
1606(display)X
1883(name)X
2096(or)X
2191(DISPLAY)X
2583(environment)X
3051(variable)X
3357(is)X
3438(a)X
3499(string)X
3722(that)X
3877(has)X
555 4012(the)N
685(format:)X
2 f
1131 4164(hostname)N
1 f
1475(:)X
2 f
1507(number)X
1 f
1782(.)X
2 f
1811(screen)X
555 4288(hostname)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(of)X
1909(the)X
2039(host)X
2208(machine)X
2529(on)X
2639(which)X
2876(the)X
3006(display)X
3283(is)X
3364(physically)X
1131 4384(attached.)N
1491(You)X
1664(follow)X
1916(the)X
2046(hostname)X
2406(with)X
2585(either)X
2808(a)X
2869(single)X
3102(colon)X
3320(\(:\))X
3425(or)X
3520(a)X
3581(double)X
1131 4480(colon)N
1349(\(::\).)X
1523(Each)X
1721(is)X
1802(discussed)X
2161(in)X
2252(the)X
2382(following)X
2747(paragraph.)X
2 f
555 4604(number)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(the)X
2117(display)X
2394(server)X
2630(on)X
2740(that)X
2895(host)X
3064(machine.)X
3429(You)X
3602(may)X
1131 4700(optionally)N
1512(follow)X
1764(this)X
1914(display)X
2191(number)X
2482(with)X
2661(a)X
2722(period)X
2969(\(.\).)X
2 f
555 4824(screen)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(the)X
2117(screen)X
2363(on)X
2473(that)X
2628(host)X
2797(server.)X
3077(Multiple)X
3404(screens)X
3684(can)X
3828(be)X
1131 4920(connected)N
1510(to)X
1601(or)X
1696(controlled)X
2076(by)X
2186(a)X
2247(single)X
2480(X)X
2565(server.)X
2845(The)X
3004(screen)X
3250(sets)X
3404(an)X
3509(internal)X
3801(vari-)X
1131 5016(able)N
1300(that)X
1455(can)X
1599(be)X
1704(accessed)X
2033(by)X
2143(using)X
2356(the)X
5 f
2504(DefaultScreen)X
1 f
3099(macro)X
3341(\(or)X
3465(the)X
5 f
3613(XDe-)X
1131 5112(faultScreen)N
1 f
1614(function)X
1930(if)X
2006(you)X
2160(are)X
2289(using)X
2502(other)X
2705(languages\).)X
3152(See)X
3301(Section)X
3588(2.2.1)X
3786(for)X
1131 5208(further)N
1392(information.)X
555 5332(For)N
699(example,)X
1042(the)X
1172(following)X
1537(would)X
1779(specify)X
2055(screen)X
2301(2)X
2367(of)X
2462(display)X
2739(0)X
2805(on)X
2915(the)X
3045(machine)X
3366(named)X
3623(mit-)X
555 5428(athena:)N
555 5552(mit-athena:0.2)N
555 5724(The)N
5 f
732(XOpenDisplay)X
1 f
1331(function)X
1647(returns)X
1913(a)X
1974(Display)X
2270(structure)X
2600(that)X
2755(serves)X
2996(as)X
3091(the)X
3221(connection)X
3630(to)X
3721(the)X
3851(X)X
555 5820(server)N
791(and)X
940(that)X
1095(contains)X
1411(all)X
1522(the)X
1652(information)X
2091(about)X
2309(that)X
2464(X)X
2549(server.)X
5 f
2850(XOpenDisplay)X
1 f
3449(connects)X
3779(the)X
3 f
2261 6144(5)N

6 p
%%Page: 6 10
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(speci\256ed)N
890(hardware)X
1238(display)X
1515(to)X
1606(the)X
1736(server)X
1972(through)X
2268(TCP,)X
2474(UNIX)X
2714(domain,)X
3023(or)X
3118(DECnet)X
3424(stream)X
3681(com-)X
555 768(munications)N
1009(protocols.)X
1403(If)X
1483(the)X
1613(hostname)X
1973(is)X
2054(a)X
2115(host)X
2284(machine)X
2605(name)X
2818(and)X
2967(a)X
3028(single)X
3261(colon)X
3479(\(:\))X
3584(separates)X
555 864(the)N
685(hostname)X
1045(and)X
1194(display)X
1471(number,)X
5 f
1802(XOpenDisplay)X
1 f
2401(connects)X
2731(using)X
2944(TCP)X
3128(streams.)X
3463(If)X
3543(the)X
3673(hostname)X
555 960(is)N
2 f
636(unix)X
1 f
810(and)X
959(a)X
1020(single)X
1253(colon)X
1471(\(:\))X
1576(separates)X
1920(it)X
1992(from)X
2185(the)X
2315(display)X
2592(number,)X
5 f
2923(XOpenDisplay)X
1 f
3522(connects)X
555 1056(using)N
768(UNIX)X
1008(domain)X
1295(IPC)X
1454(streams.)X
1789(If)X
1869(the)X
1999(hostname)X
2359(is)X
2440(a)X
2501(host)X
2670(machine)X
2991(name)X
3204(and)X
3353(a)X
3414(double)X
3676(colon)X
3894(\(::\))X
555 1152(separates)N
899(the)X
1029(hostname)X
1389(and)X
1538(display)X
1815(number,)X
5 f
2146(XOpenDisplay)X
1 f
2745(connects)X
3075(using)X
3288(DECnet)X
3594(streams.)X
555 1248(To)N
675(use)X
814(DECnet,)X
1142(however,)X
1488(the)X
1618(X)X
1703(library)X
1960 0.2885(implementation)AX
2538(must)X
2732(have)X
2920(been)X
3108(built)X
3293(to)X
3384(support)X
3670(it.)X
3786(A)X
3871(sin-)X
555 1344(gle)N
685(server)X
921(can)X
1065(support)X
1351(any)X
1500(or)X
1595(all)X
1706(of)X
1801(these)X
2004(transport)X
2339(mechanisms)X
2797(simultaneously.)X
555 1468(If)N
635(successful,)X
5 f
1058(XOpenDisplay)X
1 f
1657(returns)X
1923(a)X
1984(pointer)X
2256(to)X
2347(a)X
5 f
2426(Display)X
1 f
2750(structure)X
3080(that)X
3235(is)X
3316(de\256ned)X
3597(in)X
5 f
570 1564(<X11/Xlib.h>)N
1 f
1089(.)X
1155(See)X
1304(Section)X
1591(2.2.1,)X
1811(for)X
1935(information)X
2374(about)X
2592(using)X
2805(macros)X
3081(and)X
3230(functions)X
3580(to)X
3671(obtain)X
555 1660(information)N
994(from)X
1187(the)X
5 f
1335(Display)X
1 f
1659(structure.)X
2033(If)X
5 f
2131(XOpenDisplay)X
1 f
2730(does)X
2913(not)X
3048(succeed,)X
3370(it)X
3442(returns)X
3708(a)X
555 1756(NULL.)N
855(After)X
1062(a)X
1123(successful)X
1506(call)X
1656(to)X
5 f
1765(XOpenDisplay)X
1 f
2342(,)X
2386(all)X
2497(of)X
2592(the)X
2722(screens)X
3002(in)X
3093(the)X
3223(display)X
3500(may)X
3674(be)X
3779(used)X
555 1852(by)N
665(the)X
795(client)X
1014(application.)X
1473(The)X
1632(screen)X
1878(number)X
2169(speci\256ed)X
2504(in)X
2595(the)X
2725(display_name)X
3237(argument)X
3592(serves)X
3833(only)X
555 1948(to)N
646(specify)X
922(the)X
1052(value)X
1265(that)X
1420(will)X
1580(be)X
1685(returned)X
2000(by)X
2110(the)X
5 f
2258(DefaultScreen)X
1 f
2853(macro)X
3095(or)X
3190(the)X
5 f
3338(XDefaultScreen)X
1 f
555 2044(function.)N
915(You)X
1088(can)X
1232(access)X
1478(elements)X
1814(of)X
1909(the)X
5 f
2057(Display)X
1 f
2381(and)X
5 f
2548(Screen)X
1 f
2864(structures)X
3228(by)X
3338(using)X
3551(the)X
3681(informa-)X
555 2140(tion)N
715(macros)X
991(or)X
1086(functions.)X
3 f
555 2332(2.2.)N
731(Obtaining)X
1136(Information)X
1617(About)X
1873(the)X
2012(Display,)X
2340(Image)X
2596(Formats,)X
2957(or)X
3062(Screens)X
1 f
555 2456(The)N
714(Xlib)X
893(library)X
1150(provides)X
1475(a)X
1536(number)X
1827(of)X
1922(useful)X
2159(macros)X
2435(and)X
2584(corresponding)X
3109(functions)X
3459(that)X
3614(return)X
3846(data)X
555 2552(from)N
748(the)X
5 f
896(Display)X
1 f
1220(structure.)X
1594(The)X
1753(macros)X
2029(are)X
2158(to)X
2249(be)X
2354(used)X
2537(for)X
2661(C)X
2742(programming,)X
3266(while)X
3484(their)X
555 2648(corresponding)N
1080(function)X
1396(equivalents)X
1820(are)X
1949(for)X
2073(other)X
2276(language)X
2616(bindings.)X
2986(This)X
3165(section)X
3437(discusses)X
3786(the:)X
10 f
555 2772(g)N
1 f
775(Display)X
1071(macros)X
10 f
555 2896(g)N
1 f
775(Image)X
1017(format)X
1274(macros)X
10 f
555 3020(g)N
1 f
775(Screen)X
1036(macros)X
555 3144(All)N
690(other)X
893(members)X
1238(of)X
1333(the)X
5 f
1481(Display)X
1 f
1805(structure)X
2135(\(that)X
2319(is,)X
2422(those)X
2630(for)X
2754(which)X
2991(no)X
3101(macros)X
3377(are)X
3506(de\256ned\))X
3816(are)X
555 3240(private)N
822(to)X
913(Xlib)X
1092(and)X
1241(must)X
1435(not)X
1570(be)X
1675(used.)X
1902(That)X
2086(is,)X
2189(applications)X
2638(must)X
2832(never)X
3049(directly)X
3341(modify)X
3618(or)X
3713(inspect)X
555 3336(these)N
758(private)X
1025(elements)X
1361(in)X
1452(the)X
5 f
1600(Display)X
1 f
1924(structure.)X
3 f
555 3528(2.2.1.)N
797(Display)X
1103(Macros)X
1 f
555 3652(Once)N
762(you)X
916(have)X
1104(successfully)X
1556(connected)X
1935(to)X
2026(the)X
2156(X)X
2241(server)X
2477(that)X
2632(is)X
2713(driving)X
2990(a)X
3051(screen)X
3297(on)X
3407(your)X
3590(display,)X
555 3748(you)N
709(can)X
853(use)X
992(the)X
1122(Xlib)X
1301(display)X
1578(macros)X
1854(or)X
1949(corresponding)X
2474(functions)X
2824(to)X
2915(obtain)X
3158(information)X
3597(about)X
3815(that)X
555 3844(display.)N
854(Applications)X
1327(should)X
1584(not)X
1719(directly)X
2011(modify)X
2288(any)X
2437(part)X
2596(of)X
2691(the)X
5 f
2839(Display)X
1 f
3163(and)X
5 f
3330(Screen)X
1 f
3646(structures.)X
555 3940(The)N
714(members)X
1059(should)X
1316(be)X
1421(considered)X
1824(read-only,)X
2205(although)X
2536(they)X
2710(may)X
2884(change)X
3155(as)X
3250(the)X
3380(result)X
3598(of)X
3693(other)X
555 4036(operations)N
944(on)X
1054(the)X
1184(display.)X
555 4160(The)N
714(following)X
1079(lists)X
1244(the)X
1374(C)X
1455(language)X
1795(macros,)X
2093(their)X
2277(corresponding)X
2802(function)X
3118(equivalents)X
3542(that)X
3697(are)X
3826(for)X
555 4256(other)N
758(language)X
1098(bindings,)X
1446(and)X
1595(what)X
1788(data)X
1957(they)X
2131(both)X
2310(can)X
2454(return.)X
555 4504(AllPlanes\()N
934(\))X
555 4696(unsigned)N
895(long)X
1074(XAllPlanes\()X
1516(\))X
555 4848(Both)N
749(return)X
981(a)X
1042(value)X
1255(with)X
1434(all)X
1545(bits)X
1695(set)X
1815(on)X
1925(suitable)X
2222(for)X
2346(use)X
2485(in)X
2576(a)X
2637(plane)X
2850(argument)X
3205(to)X
3296(a)X
3357(procedure.)X
555 5096(BlackPixel)N
950(\()X
2 f
986(display)X
1 f
1248(,)X
2 f
1299(screen)X
1 f
1535(\))X
555 5288(unsigned)N
895(long)X
1074(XBlackPixel)X
1532(\()X
2 f
1568(display)X
1 f
1830(,)X
2 f
1881(screen)X
1 f
2117(\))X
687 5384(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5480(int)N
2 f
803(screen)X
1 f
1039(;)X
555 5632(Both)N
749(return)X
981(the)X
1111(black)X
1324(pixel)X
1523(value)X
1736(for)X
1860(the)X
1990(speci\256ed)X
2325(screen.)X
3 f
2261 6144(6)N

7 p
%%Page: 7 11
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(ConnectionNumber)N
1257(\()X
2 f
1293(display)X
1 f
1555(\))X
555 864(int)N
671(XConnectionNumber)X
1436(\()X
2 f
1472(display)X
1 f
1734(\))X
687 960(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 1112(Both)N
749(return)X
981(a)X
1042(connection)X
1451(number)X
1742(for)X
1866(the)X
1996(speci\256ed)X
2331(display.)X
2652(On)X
2781(a)X
2842(UNIX-based)X
3311(system,)X
3600(this)X
3750(is)X
3831(the)X
555 1208(\256le)N
690(descriptor)X
1064(of)X
1159(the)X
1289(connection.)X
555 1456(DefaultColormap)N
1179(\()X
2 f
1215(display)X
1 f
1477(,)X
2 f
1528(screen)X
1 f
1764(\))X
555 1648(Colormap)N
930(XDefaultColormap)X
1617(\()X
2 f
1653(display)X
1 f
1915(,)X
2 f
1966(screen)X
1 f
2202(\))X
687 1744(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1840(int)N
2 f
803(screen)X
1 f
1039(;)X
555 1992(Both)N
749(return)X
981(the)X
1111(default)X
1378(Colormap)X
1753(ID)X
1867(for)X
1991(allocation)X
2362(on)X
2472(the)X
2602(speci\256ed)X
2937(screen.)X
3227(Most)X
3430(routine)X
3702(alloca-)X
555 2088(tions)N
749(of)X
844(color)X
1047(should)X
1304(be)X
1409(made)X
1622(out)X
1757(of)X
1852(this)X
2002(color)X
2205(map.)X
555 2336(DefaultDepth)N
1041(\()X
2 f
1077(display)X
1 f
1339(,)X
2 f
1390(screen)X
1 f
1626(\))X
555 2528(int)N
671(XDefaultDepth)X
1220(\()X
2 f
1256(display)X
1 f
1518(,)X
2 f
1569(screen)X
1 f
1805(\))X
687 2624(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2720(int)N
2 f
803(screen)X
1 f
1039(;)X
555 2872(Both)N
749(return)X
981(the)X
1111(depth)X
1329(\(number)X
1649(of)X
1744(planes\))X
2020(of)X
2115(the)X
2245(default)X
2512(root)X
2676(window)X
2981(for)X
3105(the)X
3235(speci\256ed)X
3570(screen.)X
555 2968(Other)N
777(depths)X
1029(may)X
1203(also)X
1367(be)X
1472(supported)X
1841(on)X
1951(this)X
2101(screen.)X
2391(See)X
5 f
2558(XMatchVisualInfo)X
1 f
3281(in)X
3372(Chapter)X
3673(10)X
3783(to)X
3874(\256nd)X
555 3064(out)N
690(how)X
863(to)X
954(determine)X
1329(what)X
1522(depths)X
1774(may)X
1948(be)X
2053(available.)X
555 3312(DefaultGC)N
948(\()X
2 f
984(display)X
1 f
1246(,)X
2 f
1297(screen)X
1 f
1533(\))X
555 3504(GC)N
699(XDefaultGC)X
1155(\()X
2 f
1191(display)X
1 f
1453(,)X
2 f
1504(screen)X
1 f
1740(\))X
687 3600(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3696(int)N
2 f
803(screen)X
1 f
1039(;)X
555 3848(Both)N
749(return)X
981(the)X
1111(default)X
1378(graphics)X
1698(context)X
1980(for)X
2104(the)X
2234(root)X
2398(window)X
2703(of)X
2798(the)X
2928(speci\256ed)X
3263(screen.)X
3553(This)X
3732(GC)X
3876(is)X
555 3944(created)N
831(for)X
955(the)X
1085(convenience)X
1547(of)X
1642(simple)X
1900(applications)X
2349(and)X
2498(contains)X
2814(the)X
2944(default)X
3211(GC)X
3355(components)X
3803(with)X
555 4040(the)N
685(foreground)X
1097(and)X
1246(background)X
1683(pixel)X
1882(values)X
2129 0.3187(initialized)AX
2506(to)X
2597(the)X
2727(black)X
2940(and)X
3089(white)X
3307(pixels,)X
3562(respectively,)X
555 4136(for)N
679(the)X
809(screen.)X
1099(You)X
1272(can)X
1416(modify)X
1693(its)X
1799(contents)X
2115(freely)X
2342(because)X
2642(it)X
2714(is)X
2795(not)X
2930(used)X
3113(in)X
3204(any)X
3353(Xlib)X
3532(function.)X
555 4384(DefaultRootWindow)N
1301(\()X
2 f
1337(display)X
1 f
1599(\))X
555 4576(Window)N
880(XDefaultRootWindow)X
1689(\()X
2 f
1725(display)X
1 f
1987(\))X
687 4672(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 4824(Both)N
749(return)X
981(the)X
1111(root)X
1275(window)X
1580(for)X
1704(the)X
1834(default)X
2101(screen.)X
555 5072(DefaultScreen)N
1065(\()X
2 f
1101(display)X
1 f
1363(\))X
555 5264(int)N
671(XDefaultScreen)X
1244(\()X
2 f
1280(display)X
1 f
1542(\))X
687 5360(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 5512(Both)N
749(return)X
981(the)X
1111(default)X
1378(screen)X
1624(referenced)X
2016(in)X
2107(the)X
5 f
2255(XOpenDisplay)X
1 f
2854(routine.)X
3148(This)X
3327(macro)X
3569(and)X
3718(func-)X
555 5608(tion)N
715(should)X
972(be)X
1077(used)X
1260(to)X
1351(retrieve)X
1642(the)X
1772(screen)X
2018(number)X
2309(in)X
2400(applications)X
2849(that)X
3004(will)X
3164(use)X
3303(only)X
3482(a)X
3543(single)X
555 5704(screen.)N
3 f
2261 6144(7)N

8 p
%%Page: 8 12
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(DefaultVisual)N
1056(\()X
2 f
1092(display)X
1 f
1354(,)X
2 f
1405(screen)X
1 f
1641(\))X
555 864(Visual)N
807(*XDefaultVisual)X
1415(\()X
2 f
1451(display)X
1 f
1713(,)X
2 f
1764(screen)X
1 f
2000(\))X
687 960(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1056(int)N
2 f
803(screen)X
1 f
1039(;)X
555 1208(Both)N
749(return)X
981(the)X
1111(default)X
1378(visual)X
1611(type)X
1785(for)X
1909(the)X
2039(speci\256ed)X
2374(screen.)X
2664(See)X
2813(section)X
3085(3.1)X
3217(for)X
3341(further)X
3602(informa-)X
555 1304(tion)N
715(about)X
933(visual)X
1166(types.)X
555 1552(DisplayCells)N
1018(\()X
2 f
1054(display)X
1 f
1316(,)X
2 f
1367(screen)X
1 f
1603(\))X
555 1744(int)N
671(XDisplayCells)X
1197(\()X
2 f
1233(display)X
1 f
1495(,)X
2 f
1546(screen)X
1 f
1782(\))X
687 1840(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1936(int)N
2 f
803(screen)X
1 f
1039(;)X
555 2088(Both)N
749(return)X
981(the)X
1111(number)X
1402(of)X
1497(entries)X
1754(in)X
1845(the)X
1975(default)X
2242(color)X
2445(map.)X
555 2336(DisplayPlanes)N
1066(\()X
2 f
1102(display)X
1 f
1364(,)X
2 f
1415(screen)X
1 f
1651(\))X
555 2528(int)N
671(XDisplayPlanes)X
1245(\()X
2 f
1281(display)X
1 f
1543(,)X
2 f
1594(screen)X
1 f
1830(\))X
687 2624(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2720(int)N
2 f
803(screen)X
1 f
1039(;)X
555 2872(Both)N
749(return)X
981(the)X
1111(depth)X
1329(of)X
1424(the)X
1554(root)X
1718(window)X
2023(of)X
2118(the)X
2248(speci\256ed)X
2583(screen.)X
2873(See)X
3022(the)X
3152(glossary)X
3467(for)X
3591(a)X
3652(discus-)X
555 2968(sion)N
724(of)X
819(depth.)X
555 3216(DisplayString)N
1052(\()X
2 f
1088(display)X
1 f
1350(\))X
555 3408(char)N
728(*XDisplayString)X
1332(\()X
2 f
1368(display)X
1 f
1630(\))X
687 3504(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 3656(Both)N
749(return)X
981(the)X
1111(string)X
1334(that)X
1489(was)X
1647(passed)X
1903(to)X
5 f
2012(XOpenDisplay)X
1 f
2611(when)X
2823(the)X
2953(current)X
3224(display)X
3501(was)X
3659(opened.)X
555 3752(On)N
684(UNIX-based)X
1153(systems,)X
1476(if)X
1552(the)X
1682(passed)X
1938(string)X
2161(was)X
2319(NULL,)X
2597(this)X
2747(macro)X
2989(returns)X
3255(the)X
3385(value)X
3598(of)X
3693(the)X
555 3848(DISPLAY)N
947(environment)X
1415(variable)X
1721(when)X
1933(the)X
2063(current)X
2334(display)X
2611(was)X
2769(opened.)X
3089(This)X
3268(macro)X
3510(is)X
3591(useful)X
3828(to)X
555 3944(applications)N
1004(that)X
1159(invoke)X
1421(the)X
5 f
1569(fork)X
1 f
1753(system)X
2020(call)X
2170(and)X
2319(want)X
2512(to)X
2603(open)X
2796(a)X
2857(new)X
3025(connection)X
3434(to)X
3525(the)X
3655(same)X
555 4040(display)N
832(from)X
1025(the)X
1155(child)X
1354(process.)X
555 4288(ImageByteOrder)N
1153(\()X
2 f
1189(display)X
1 f
1451(\))X
555 4480(int)N
671(XImageByteOrder)X
1332(\()X
2 f
1368(display)X
1 f
1630(\))X
687 4576(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 4728(Both)N
749(specify)X
1025(the)X
1155(required)X
1470(byte)X
1644(order)X
1851(for)X
1975(images)X
2247(for)X
2371(each)X
2554(scanline)X
2865(unit)X
3025(in)X
3116(XYFormat)X
3519(\(bitmap\))X
3845(or)X
555 4824(for)N
679(each)X
862(pixel)X
1061(value)X
1274(in)X
1365(ZFormat.)X
1740(The)X
1899(macro)X
2141(and)X
2290(function)X
2606(can)X
2750(return)X
2982(one)X
3131(of)X
3226(the)X
3356(constants)X
5 f
570 4920(LSBFirst)N
1 f
945(or)X
5 f
1058(MSBFirst)X
1 f
1435(.)X
555 5168(ProtocolRevision)N
1175(\()X
2 f
1211(display)X
1 f
1473(\))X
555 5360(int)N
671(XProtocolRevision)X
1354(\()X
2 f
1390(display)X
1 f
1652(\))X
687 5456(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 5608(Both)N
749(return)X
981(the)X
1111(minor)X
1344(protocol)X
1660(revision)X
1966(number)X
2257(of)X
2352(the)X
2482(X)X
2567(server.)X
3 f
2261 6144(8)N

9 p
%%Page: 9 13
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(ProtocolVersion)N
1139(\()X
2 f
1175(display)X
1 f
1437(\))X
555 864(int)N
671(XProtocolVersion)X
1318(\()X
2 f
1354(display)X
1 f
1616(\))X
687 960(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 1112(Both)N
749(return)X
981(the)X
1111(major)X
1339(version)X
1620(number)X
1911(\(11\))X
2079(of)X
2174(the)X
2304(X)X
2389(protocol)X
2705(associated)X
3089(with)X
3268(the)X
3398(connected)X
555 1208(display.)N
555 1456(QLength)N
875(\()X
2 f
911(display)X
1 f
1173(\))X
555 1648(int)N
671(XQLength)X
1054(\()X
2 f
1090(display)X
1 f
1352(\))X
687 1744(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 1896(Both)N
749(return)X
981(the)X
1111(length)X
1354(of)X
1449(the)X
1579(event)X
1792(queue)X
2024(for)X
2148(the)X
2278(connected)X
2657(display.)X
2978(Note)X
3171(that)X
3326(there)X
3524(may)X
3698(be)X
3803(more)X
555 1992(events)N
802(that)X
957(have)X
1145(not)X
1280(been)X
1468(read)X
1641(into)X
1801(the)X
1931(queue)X
2163(yet.)X
555 2240(RootWindow)N
1037(\()X
2 f
1073(display)X
1 f
1335(,)X
2 f
1386(screen)X
1 f
1622(\))X
555 2432(Window)N
880(XRootWindow)X
1425(\()X
2 f
1461(display)X
1 f
1723(,)X
2 f
1774(screen)X
1 f
2010(\))X
687 2528(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2624(int)N
2 f
803(screen)X
1 f
1039(;)X
555 2776(Both)N
749(return)X
981(the)X
1111(root)X
1275(window.)X
1624(This)X
1803(macro)X
2045(and)X
2194(function)X
2510(are)X
2639(useful)X
2876(with)X
3055(functions)X
3405(that)X
3560(take)X
3729(a)X
3790(parent)X
555 2872(window)N
860(as)X
955(an)X
1060(argument.)X
555 3120(ScreenCount)N
1017(\()X
2 f
1053(display)X
1 f
1315(\))X
555 3312(int)N
671(XScreenCount)X
1196(\()X
2 f
1232(display)X
1 f
1494(\))X
687 3408(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 3560(Both)N
749(return)X
981(the)X
1111(number)X
1402(of)X
1497(available)X
1838(screens.)X
555 3808(ServerVendor)N
1054(\()X
2 f
1090(display)X
1 f
1352(\))X
555 4000(char)N
728(*XServerVendor)X
1334(\()X
2 f
1370(display)X
1 f
1632(\))X
687 4096(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 4248(Both)N
749(return)X
981(a)X
1042(pointer)X
1314(to)X
1405(a)X
1466(null-terminated)X
2033(string)X
2256(that)X
2411(provides)X
2736(some)X
2944(identi\256cation)X
3433(of)X
3528(the)X
3658(owner)X
3899(of)X
555 4344(the)N
685(X)X
770(server)X
1006 0.2679(implementation.)AX
555 4592(VendorRelease)N
1099(\()X
2 f
1135(display)X
1 f
1397(\))X
555 4784(int)N
671(XVendorRelease)X
1278(\()X
2 f
1314(display)X
1 f
1576(\))X
687 4880(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 5032(Both)N
749(return)X
981(a)X
1042(number)X
1333(related)X
1595(to)X
1686(a)X
1747(vendor's)X
2076(release)X
2342(of)X
2437(the)X
2567(X)X
2652(server.)X
555 5280(WhitePixel)N
960(\()X
2 f
996(display)X
1 f
1258(,)X
2 f
1309(screen)X
1 f
1545(\))X
555 5472(unsigned)N
895(long)X
1074(XWhitePixel)X
1542(\()X
2 f
1578(display)X
1 f
1840(,)X
2 f
1891(screen)X
1 f
2127(\))X
687 5568(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5664(int)N
2 f
803(screen)X
1 f
1039(;)X
3 f
2261 6144(9)N

10 p
%%Page: 10 14
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Both)N
749(return)X
981(the)X
1111(white)X
1329(pixel)X
1528(value)X
1741(for)X
1865(the)X
1995(speci\256ed)X
2330(screen.)X
3 f
555 864(2.2.2.)N
797(Image)X
1053(Format)X
1358(Macros)X
1 f
555 988(Once)N
762(you)X
916(have)X
1104(successfully)X
1556(connected)X
1935(to)X
2026(the)X
2156(X)X
2241(server)X
2477(that)X
2632(is)X
2713(driving)X
2990(a)X
3051(screen)X
3297(on)X
3407(your)X
3590(display,)X
555 1084(you)N
709(can)X
853(use)X
992(the)X
1122(Xlib)X
1301(image)X
1539(format)X
1796(macros)X
2072(to)X
2163(obtain)X
2406(information)X
2845(about)X
3063(image)X
3301(formats.)X
555 1208(The)N
714(following)X
1079(lists)X
1244(the)X
1374(C)X
1455(language)X
1795(macros,)X
2093(their)X
2277(corresponding)X
2802(function)X
3118(equivalents)X
3542(that)X
3697(are)X
3826(for)X
555 1304(other)N
758(language)X
1098(bindings,)X
1446(and)X
1595(what)X
1788(data)X
1957(they)X
2131(both)X
2310(return.)X
2586(These)X
2818(are)X
2947(often)X
3150(used)X
3333(by)X
3443(toolkits)X
3731(as)X
3826(well)X
555 1400(as)N
650(simple)X
908(applications.)X
555 1648(BitmapBitOrder)N
1136(\()X
2 f
1172(display)X
1 f
1434(\))X
555 1840(int)N
671(XBitmapBitOrder)X
1315(\()X
2 f
1351(display)X
1 f
1613(\))X
687 1936(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 2088(Within)N
823(each)X
1006(bitmap)X
1274(unit,)X
1456(the)X
1586(leftmost)X
1898(bit)X
2014(in)X
2105(the)X
2235(bitmap)X
2503(as)X
2598(displayed)X
2958(on)X
3068(the)X
3198(screen)X
3444(is)X
3525(either)X
3748(the)X
555 2184(least)N
739(or)X
834(most)X
1028(signi\256cant)X
1418(bit)X
1534(in)X
1625(the)X
1755(unit.)X
1959(This)X
2138(macro)X
2380(and)X
2529(function)X
2845(can)X
2989(return)X
3221(one)X
3370(of)X
3465(the)X
3595(constants)X
5 f
570 2280(LSBFirst)N
1 f
945(or)X
5 f
1058(MSBFirst)X
1 f
1435(.)X
555 2528(BitmapPad)N
955(\()X
2 f
991(display)X
1 f
1253(\))X
555 2720(int)N
671(XBitmapPad)X
1134(\()X
2 f
1170(display)X
1 f
1432(\))X
687 2816(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 2968(Each)N
753(scanline)X
1064(must)X
1258(be)X
1363(padded)X
1639(to)X
1730(a)X
1791(multiple)X
2109(of)X
2204(bits)X
2354(returned)X
2669(by)X
2779(this)X
2929(macro)X
3171(or)X
3266(function.)X
555 3216(BitmapUnit)N
980(\()X
2 f
1016(display)X
1 f
1278(\))X
555 3408(int)N
671(XBitmapUnit)X
1159(\()X
2 f
1195(display)X
1 f
1457(\))X
687 3504(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 3656(Both)N
749(return)X
981(the)X
1111(size)X
1270(of)X
1365(a)X
1426(bitmap's)X
1757(unit)X
1917(in)X
2008(bits.)X
2202(The)X
2361(scanline)X
2672(is)X
2753(calculated)X
3133(in)X
3224(multiples)X
3576(of)X
3671(this)X
555 3752(value.)N
555 4000(DisplayHeight)N
1076(\()X
2 f
1112(display)X
1 f
1374(,)X
2 f
1425(screen)X
1 f
1661(\))X
555 4192(int)N
671(XDisplayHeightMM)X
1411(\()X
2 f
1447(display)X
1 f
1709(,)X
2 f
1760(screen)X
1 f
1996(\))X
687 4288(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4384(int)N
2 f
803(screen)X
1 f
1039(;)X
555 4536(Both)N
749(return)X
981(an)X
1086(integer)X
1353(that)X
1508(describes)X
1857(the)X
1987(height)X
2230(of)X
2325(the)X
2455(screen)X
2701(in)X
2792(pixels.)X
555 4784(DisplayHeightMM)N
1232(\()X
2 f
1268(display)X
1 f
1530(,)X
2 f
1581(screen)X
1 f
1817(\))X
555 4976(int)N
671(XDisplayHeightMM)X
1411(\()X
2 f
1447(display)X
1 f
1709(,)X
2 f
1760(screen)X
1 f
1996(\))X
687 5072(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5168(int)N
2 f
803(screen)X
1 f
1039(;)X
555 5320(Both)N
749(return)X
981(the)X
1111(height)X
1354(of)X
1449(the)X
1579(speci\256ed)X
1914(screen)X
2160(in)X
2251 0.2955(millimeters.)AX
3 f
2239 6144(10)N

11 p
%%Page: 11 15
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(DisplayWidth)N
1057(\()X
2 f
1093(display)X
1 f
1355(,)X
2 f
1406(screen)X
1 f
1642(\))X
555 864(int)N
671(XDisplayWidth)X
1236(\()X
2 f
1272(display)X
1 f
1534(,)X
2 f
1585(screen)X
1 f
1821(\))X
687 960(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1056(int)N
2 f
803(screen)X
1 f
1039(;)X
555 1208(Both)N
749(return)X
981(an)X
1086(integer)X
1353(that)X
1508(describes)X
1857(the)X
1987(width)X
2210(of)X
2305(the)X
2435(screen)X
2681(in)X
2772(pixels.)X
555 1456(DisplayWidthMM)N
1213(\()X
2 f
1249(display)X
1 f
1511(,)X
2 f
1562(screen)X
1 f
1798(\))X
555 1648(int)N
671(XDisplayWidthMM)X
1392(\()X
2 f
1428(display)X
1 f
1690(,)X
2 f
1741(screen)X
1 f
1977(\))X
687 1744(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1840(int)N
2 f
803(screen)X
1 f
1039(;)X
555 1992(Both)N
749(return)X
981(the)X
1111(width)X
1334(of)X
1429(the)X
1559(speci\256ed)X
1894(screen)X
2140(in)X
2231 0.2955(millimeters.)AX
3 f
555 2184(2.2.3.)N
797(Screen)X
1073(Information)X
1554(Macros)X
1 f
555 2308(Once)N
762(you)X
916(have)X
1104(successfully)X
1556(connected)X
1935(to)X
2026(the)X
2156(X)X
2241(server)X
2477(that)X
2632(is)X
2713(driving)X
2990(a)X
3051(screen)X
3297(on)X
3407(your)X
3590(display,)X
555 2404(you)N
709(can)X
853(use)X
992(the)X
1122(Xlib)X
1301(screen)X
1547(information)X
1986(macros)X
2262(to)X
2353(obtain)X
2596(information)X
3035(about)X
3253(screens.)X
555 2528(The)N
714(following)X
1079(lists)X
1244(the)X
1374(C)X
1455(language)X
1795(macros,)X
2093(their)X
2277(corresponding)X
2802(function)X
3118(equivalents)X
3542(that)X
3697(are)X
3826(for)X
555 2624(other)N
758(language)X
1098(bindings,)X
1446(and)X
1595(what)X
1788(data)X
1957(they)X
2131(both)X
2310(can)X
2454(return.)X
555 2872(BlackPixelOfScreen)N
1281(\()X
2 f
1317(screen)X
1 f
1553(\))X
555 3064(unsigned)N
895(long)X
1074(XBlackPixelOfScreen)X
1863(\()X
2 f
1899(screen)X
1 f
2135(\))X
687 3160(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 3312(Both)N
749(return)X
981(the)X
1111(black)X
1324(pixel)X
1523(value)X
1736(of)X
1831(the)X
1961(speci\256ed)X
2296(screen.)X
555 3560(CellsOfScreen)N
1075(\()X
2 f
1111(screen)X
1 f
1347(\))X
555 3752(int)N
671(XCellsOfScreen)X
1254(\()X
2 f
1290(screen)X
1 f
1526(\))X
687 3848(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 4000(Both)N
749(return)X
981(the)X
1111(number)X
1402(of)X
1497(colormap)X
1852(cells)X
2036(of)X
2131(the)X
2261(speci\256ed)X
2596(screen.)X
555 4248(DefaultColormapOfScreen)N
1510(\()X
2 f
1546(screen)X
1 f
1782(\))X
555 4440(Colormap)N
930(XDefaultColormapOfScreen)X
1948(\()X
2 f
1984(screen)X
1 f
2220(\))X
687 4536(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 4688(Both)N
749(return)X
981(the)X
1111(default)X
1378(colormap)X
1733(of)X
1828(the)X
1958(speci\256ed)X
2293(screen.)X
555 4936(DefaultDepthOfScreen)N
1372(\()X
2 f
1408(screen)X
1 f
1644(\))X
555 5128(int)N
671(XDefaultDepthOfScreen)X
1551(\()X
2 f
1587(screen)X
1 f
1823(\))X
687 5224(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 5376(Both)N
749(return)X
981(the)X
1111(default)X
1378(depth)X
1596(of)X
1691(the)X
1821(speci\256ed)X
2156(screen.)X
3 f
2239 6144(11)N

12 p
%%Page: 12 16
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(DefaultGCOfScreen)N
1279(\()X
2 f
1315(screen)X
1 f
1551(\))X
555 864(GC)N
699(XDefaultGCOfScreen)X
1486(\()X
2 f
1522(screen)X
1 f
1758(\))X
687 960(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 1112(Both)N
749(return)X
981(the)X
1111(default)X
1378(graphics)X
1698(context)X
1980(\(GC\))X
2182(of)X
2277(the)X
2407(speci\256ed)X
2742(screen.)X
555 1360(DefaultScreenOfDisplay)N
1431(\()X
2 f
1467(display)X
1 f
1729(\))X
555 1552(Screen)N
816(*XDefaultScreenOfDisplay)X
1799(\()X
2 f
1835(display)X
1 f
2097(\))X
687 1648(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
555 1800(Both)N
749(return)X
981(the)X
1111(default)X
1378(screen)X
1624(of)X
1719(the)X
1849(speci\256ed)X
2184(display.)X
555 2048(DefaultVisualOfScreen)N
1387(\()X
2 f
1423(screen)X
1 f
1659(\))X
555 2240(Visual)N
807(*XDefaultVisualOfScreen)X
1746(\()X
2 f
1782(screen)X
1 f
2018(\))X
687 2336(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 2488(Both)N
749(return)X
981(the)X
1111(default)X
1378(visual)X
1611(of)X
1706(the)X
1836(speci\256ed)X
2171(screen.)X
2461(See)X
2610(Section)X
2897(3.1)X
3029(for)X
3153(information)X
3592(on)X
3702(visual)X
555 2584(types.)N
555 2832(DoesBackingStore)N
1222(\()X
2 f
1258(screen)X
1 f
1494(\))X
555 3024(int)N
671(XDoesBackingStore)X
1401(\()X
2 f
1437(screen)X
1 f
1673(\))X
687 3120(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 3272(Both)N
749(return)X
981(a)X
1042(value)X
1255(indicating)X
1631(whether)X
1936(the)X
2066(screen)X
2312(supports)X
2632(backing)X
2933(stores.)X
3204(The)X
3363(value)X
3576(returned)X
555 3368(can)N
699(be)X
804(one)X
953(of)X
1048(the)X
1178(constants)X
5 f
1546(WhenMapped)X
1 f
2109(,)X
5 f
2171(NotUseful)X
1 f
2572(,)X
2616(or)X
5 f
2729(Always)X
1 f
3022(.)X
3088(See)X
3237(section)X
3509(3.2.4)X
3707(for)X
3831(a)X
555 3464(discussion)N
944(of)X
1039(the)X
1169(backing)X
1470(store.)X
555 3712(DoesSaveUnders)N
1166(\()X
2 f
1202(screen)X
1 f
1438(\))X
555 3904(Bool)N
749(XDoesSaveUnders)X
1423(\()X
2 f
1459(screen)X
1 f
1695(\))X
687 4000(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 4152(Both)N
749(return)X
981(a)X
1042(boolean)X
1343(value)X
1556(indicating)X
1932(whether)X
2237(the)X
2367(screen)X
2613(supports)X
2933(save)X
3111(unders.)X
3411(If)X
5 f
3509(True)X
1 f
3705(,)X
3749(the)X
555 4248(screen)N
801(supports)X
1121(save)X
1299(unders.)X
1599(If)X
5 f
1697(False)X
1 f
1927(,)X
1971(the)X
2101(screen)X
2347(does)X
2530(not)X
2665(support)X
2951(save)X
3129(unders.)X
3429(See)X
3578(section)X
555 4344(3.2.6)N
753(for)X
877(a)X
938(discussion)X
1327(of)X
1422(the)X
1552(save)X
1730(under.)X
555 4592(DisplayOfScreen)N
1167(\()X
2 f
1203(screen)X
1 f
1439(\))X
555 4784(Display)N
851(*XDisplayOfScreen)X
1570(\()X
2 f
1606(screen)X
1 f
1842(\))X
687 4880(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 5032(Both)N
749(return)X
981(the)X
1111(display)X
1388(of)X
1483(the)X
1613(speci\256ed)X
1948(screen.)X
555 5280(EventMaskOfScreen)N
1294(\()X
2 f
1330(screen)X
1 f
1566(\))X
555 5472(long)N
734(XEventMaskOfScreen)X
1536(\()X
2 f
1572(screen)X
1 f
1808(\))X
687 5568(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 5720(Both)N
749(return)X
981(the)X
1111(initial)X
1341(root)X
1505(event)X
1718(mask)X
1926(for)X
2050(the)X
2180(speci\256ed)X
2515(screen.)X
3 f
2239 6144(12)N

13 p
%%Page: 13 17
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(HeightOfScreen)N
1133(\()X
2 f
1169(screen)X
1 f
1405(\))X
555 864(int)N
671(XHeightOfScreen)X
1312(\()X
2 f
1348(screen)X
1 f
1584(\))X
687 960(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 1112(Both)N
749(return)X
981(the)X
1111(height)X
1354(of)X
1449(the)X
1579(speci\256ed)X
1914(screen.)X
555 1360(HeightMMOfScreen)N
1289(\()X
2 f
1325(screen)X
1 f
1561(\))X
555 1552(int)N
671(XHeightMMOfScreen)X
1468(\()X
2 f
1504(screen)X
1 f
1740(\))X
687 1648(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 1800(Both)N
749(return)X
981(the)X
1111(height)X
1354(of)X
1449(the)X
1579(speci\256ed)X
1914(screen)X
2160(in)X
2251 0.2955(millimeters.)AX
555 2048(MaxCmapsOfScreen)N
1299(\()X
2 f
1335(screen)X
1 f
1571(\))X
555 2240(int)N
671(XMaxCmapsOfScreen)X
1478(\()X
2 f
1514(screen)X
1 f
1750(\))X
687 2336(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 2488(Both)N
749(return)X
981(the)X
1111(maximum)X
1492(number)X
1783(of)X
1878(color)X
2081(maps)X
2289(supported)X
2658(by)X
2768(the)X
2898(speci\256ed)X
3233(screen.)X
555 2736(MinCmapsOfScreen)N
1285(\()X
2 f
1321(screen)X
1 f
1557(\))X
555 2928(int)N
671(XMinCmapsOfScreen)X
1464(\()X
2 f
1500(screen)X
1 f
1736(\))X
687 3024(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 3176(Both)N
749(return)X
981(the)X
1111(minimum)X
1478(number)X
1769(of)X
1864(color)X
2067(maps)X
2275(supported)X
2644(by)X
2754(the)X
2884(speci\256ed)X
3219(screen.)X
555 3424(PlanesOfScreen)N
1123(\()X
2 f
1159(screen)X
1 f
1395(\))X
555 3616(int)N
671(XPlanesOfScreen)X
1302(\()X
2 f
1338(screen)X
1 f
1574(\))X
687 3712(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 3864(Both)N
749(return)X
981(the)X
1111(number)X
1402(of)X
1497(planes)X
1744(in)X
1835(the)X
1965(speci\256ed)X
2300(screen.)X
555 4112(RootWindowOfScreen)N
1368(\()X
2 f
1404(screen)X
1 f
1640(\))X
555 4304(Window)N
880(XRootWindowOfScreen)X
1756(\()X
2 f
1792(screen)X
1 f
2028(\))X
687 4400(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 4552(Both)N
749(return)X
981(the)X
1111(root)X
1275(window)X
1580(of)X
1675(the)X
1805(speci\256ed)X
2140(screen.)X
555 4800(ScreenOfDisplay)N
1167(\()X
2 f
1203(display)X
1 f
1458(,)X
2 f
1502(screen_number)X
1 f
2050(\))X
555 4992(Screen)N
816(*XScreenOfDisplay)X
1535(\()X
2 f
1571(display)X
1 f
1826(,)X
2 f
1870(screen_number)X
1 f
2418(\))X
687 5088(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5184(int)N
2 f
803(screen_number)X
1 f
1351(;)X
555 5336(Both)N
749(return)X
981(a)X
1042(pointer)X
1314(to)X
1405(the)X
1535(screen)X
1781(of)X
1876(the)X
2006(speci\256ed)X
2341(display.)X
3 f
2239 6144(13)N

14 p
%%Page: 14 18
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(WhitePixelOfScreen)N
1291(\()X
2 f
1327(screen)X
1 f
1563(\))X
555 864(unsigned)N
895(long)X
1074(XWhitePixelOfScreen)X
1873(\()X
2 f
1909(screen)X
1 f
2145(\))X
687 960(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 1112(Both)N
749(return)X
981(the)X
1111(white)X
1329(pixel)X
1528(value)X
1741(of)X
1836(the)X
1966(speci\256ed)X
2301(screen.)X
555 1360(WidthOfScreen)N
1114(\()X
2 f
1150(screen)X
1 f
1386(\))X
555 1552(int)N
671(XWidthOfScreen)X
1293(\()X
2 f
1329(screen)X
1 f
1565(\))X
687 1648(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 1800(Both)N
749(return)X
981(the)X
1111(width)X
1334(of)X
1429(the)X
1559(speci\256ed)X
1894(screen.)X
555 2048(WidthMMOfScreen)N
1270(\()X
2 f
1306(screen)X
1 f
1542(\))X
555 2240(int)N
671(XWidthMMOfScreen)X
1449(\()X
2 f
1485(screen)X
1 f
1721(\))X
687 2336(Screen)N
948(*)X
2 f
(screen)S
1 f
1228(;)X
555 2488(Both)N
749(return)X
981(the)X
1111(width)X
1334(of)X
1429(the)X
1559(speci\256ed)X
1894(screen)X
2140(in)X
2231 0.2955(millimeters.)AX
3 f
555 2680(2.3.)N
731(Generating)X
1179(a)X
1245(NoOperation)X
1761(Protocol)X
2101(Request)X
1 f
555 2804(Use)N
5 f
731(XNoOp)X
1 f
1057(to)X
1148(execute)X
1439(a)X
1500(NoOperation)X
1981(protocol)X
2297(request.)X
2617(The)X
2776(de\256nition)X
3137(for)X
3261(this)X
3411(function)X
3727(is:)X
555 2928(XNoOp)N
839(\()X
2 f
875(display)X
1 f
1137(\))X
687 3024(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 3176(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 3300(The)N
5 f
732(XNoOp)X
1 f
1058(function)X
1374(sends)X
1591(a)X
1652(NoOperation)X
2133(request)X
2409(to)X
2500(the)X
2630(X)X
2715(server,)X
2973(thereby)X
3259(exercising)X
3643(the)X
3773(con-)X
555 3396(nection.)N
881(It)X
957(does)X
1140(not)X
1275(\257ush)X
1468(the)X
1598(output)X
1846(buffer.)X
3 f
555 3588(2.4.)N
731(Freeing)X
1042(Client-Created)X
1625(Data)X
1 f
555 3712(Use)N
5 f
731(XFree)X
1 f
1008(to)X
1099(free)X
1257(any)X
1406(in-memory)X
1820(data)X
1989(that)X
2144(was)X
2302(created)X
2578(by)X
2688(an)X
2793(Xlib)X
2972(function.)X
3332(The)X
3491(de\256nition)X
3852(for)X
555 3808(this)N
705(function)X
1021(is:)X
555 3932(XFree)N
781(\()X
2 f
817(data)X
1 f
981(\))X
665 4028(char)N
838(*)X
2 f
(data)S
1 f
1046(;)X
2 f
555 4180(data)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(data)X
2194(that)X
2349(is)X
2430(to)X
2521(be)X
2626(freed.)X
555 4304(The)N
5 f
732(XFree)X
1 f
1009(function)X
1325(is)X
1406(a)X
1467(general)X
1748(purpose)X
2048(Xlib)X
2227(routine)X
2499(that)X
2654(frees)X
2846(the)X
2976(speci\256ed)X
3311(data.)X
3 f
555 4496(2.5.)N
731(Closing)X
1037(the)X
1176(Display)X
1 f
555 4620(Use)N
5 f
731(XCloseDisplay)X
1 f
1338(to)X
1429(close)X
1632(or)X
1727(disconnect)X
2126(a)X
2187(display)X
2464(from)X
2657(the)X
2787(X)X
2872(server.)X
3152(The)X
3311(de\256nition)X
3672(for)X
3796(this)X
555 4716(function)N
871(is:)X
555 4868(XCloseDisplay)N
1100(\()X
2 f
1129(display)X
1 f
1391(\))X
687 4964(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 5116(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 5240(The)N
5 f
732(XCloseDisplay)X
1 f
1339(function)X
1655(closes)X
1892(the)X
2022(connection)X
2431(to)X
2522(the)X
2652(X)X
2737(server)X
2973(for)X
3097(the)X
3227(display)X
3504(speci\256ed)X
3839(in)X
555 5336(the)N
5 f
703(Display)X
1 f
1027(structure.)X
1401(The)X
5 f
1578(XCloseDisplay)X
1 f
2185(function)X
2501(destroys)X
2816(all)X
2927(windows,)X
3288(resource)X
3607(IDs)X
3755(\(Win-)X
555 5432(dow,)N
750(Font,)X
956(Pixmap,)X
1270(Colormap,)X
1667(Cursor,)X
1950(and)X
2099(GContext\),)X
2515(or)X
2610(other)X
2813(resources)X
3166(\(GCs\))X
3402(that)X
3557(the)X
3687(client)X
555 5528(application)N
970(has)X
1109(created)X
1385(on)X
1495(this)X
1645(display,)X
1944(unless)X
2186(the)X
2316(closedown)X
2714(mode)X
2932(of)X
3027(the)X
3157(resource)X
3476(has)X
3615(been)X
555 5624(changed.)N
914(Therefore,)X
1304(these)X
1507(windows,)X
1868(resource)X
2187(IDs,)X
2357(and)X
2506(other)X
2709(resources)X
3062(should)X
3319(never)X
3536(be)X
3641(referenced)X
555 5720(again.)N
812(In)X
907(addition,)X
1241(this)X
1391(function)X
1707(discards)X
2017(any)X
2166(output)X
2414(requests)X
2724(that)X
2879(have)X
3067(been)X
3255(buffered)X
3574(but)X
3709(have)X
3897(not)X
555 5816(yet)N
685(been)X
873(sent.)X
1059(Because)X
1374(these)X
1577(operations)X
1966 0.2604(automatically)AX
2470 0.2955(\(implicitly\))AX
2896(occur)X
3113(if)X
3189(a)X
3250(process)X
3535(exits,)X
3746(you)X
3 f
2239 6144(14)N

15 p
%%Page: 15 19
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(normally)N
896(do)X
1006(not)X
1141(have)X
1329(to)X
1420(call)X
5 f
1588(XCloseDisplay)X
1 f
2195(explicitly.)X
3 f
555 864(2.6.)N
731(X)X
816(Server)X
1087(Connection)X
1539(Close)X
1766(Operations)X
1 f
555 988(When)N
787(the)X
917(X)X
1002(server's)X
1301(connection)X
1710(to)X
1801(a)X
1862(client)X
2081(is)X
2162(closed,)X
2431(either)X
2654(by)X
2764(an)X
2869(explicit)X
3157(call)X
3307(to)X
5 f
3416(XCloseDisplay)X
1 f
555 1084(or)N
650(by)X
760(a)X
821(process)X
1106(that)X
1261(exits,)X
1472(the)X
1602(X)X
1687(server)X
1923(performs)X
2262(these)X
2465(automatic)X
2836(operations:)X
10 f
555 1208(g)N
1 f
775(All)X
910(selections)X
1280(\(see)X
5 f
1461(XSetSelectionOwner)X
1 f
2289(\))X
2340(owned)X
2596(by)X
2706(the)X
2836(client)X
3055(are)X
3184(disowned.)X
10 f
555 1332(g)N
1 f
775(Performs)X
1119(an)X
5 f
1242(XUngrabPointer)X
1 f
1905(and)X
5 f
2072(XUngrabKeyboard)X
1 f
2833(if)X
2909(the)X
3039(client)X
3258(application)X
3673(has)X
775 1428(actively)N
1077(grabbed)X
1382(the)X
1512(pointer)X
1784(or)X
1879(the)X
2009(keyboard.)X
2380(These)X
2612(functions)X
2962(are)X
3091(described)X
3450(in)X
3541(Chapter)X
3842(7.)X
10 f
555 1552(g)N
1 f
775(Performs)X
1119(an)X
5 f
1242(XUngrabServer)X
1 f
1885(if)X
1961(the)X
2091(client)X
2310(has)X
2449(grabbed)X
2754(the)X
2884(server.)X
3164(This)X
3343(function)X
3659(is)X
775 1648(described)N
1134(in)X
1225(Chapter)X
1526(7.)X
10 f
555 1772(g)N
1 f
775(Releases)X
1105(all)X
1216(passive)X
1497(grabs)X
1709(made)X
1922(by)X
2032(the)X
2162(client)X
2381(application.)X
10 f
555 1896(g)N
1 f
775(Marks)X
1021(all)X
1132(resources)X
1485(\(including)X
1870(color)X
2073(map)X
2247(entries\))X
2533(allocated)X
2874(by)X
2984(the)X
3114(client)X
3333(application)X
3748(either)X
775 1992(as)N
870(permanent)X
1264(or)X
1359(temporary,)X
1765(depending)X
2154(on)X
2264(whether)X
2569(the)X
2699(close_mode)X
3142(argument)X
3497(is)X
3578(one)X
3727(of)X
3822(the)X
775 2088(constants)N
5 f
1143(RetainPermanent)X
1 f
1865(or)X
5 f
1978(RetainTemporary)X
1 f
2672(.)X
2738(However,)X
3103(this)X
3253(does)X
3436(not)X
3571(prevent)X
775 2184(other)N
978(client)X
1197(applications)X
1646(from)X
1839(explicitly)X
2196(destroying)X
2590(the)X
2720(resources.)X
3117(\(See)X
3295(below)X
3532(and)X
5 f
790 2280(XSetCloseDownMode)N
1 f
1687(in)X
1778(Chapter)X
2079(7)X
2145(for)X
2269(further)X
2530(information.\))X
555 2404(When)N
787(the)X
917(mode)X
1135(is)X
5 f
1234(DestroyAll)X
1 f
1649(,)X
1693(the)X
1823(X)X
1908(server)X
2144(destroys)X
2459(all)X
2570(of)X
2665(a)X
2726(client)X
2945(application's)X
3423(resources)X
3776(as)X
3871(fol-)X
555 2500(lows:)N
10 f
555 2624(g)N
1 f
775(Examines)X
1145(each)X
1328(window)X
1633(in)X
1724(the)X
1854(client's)X
2136(save-set)X
2441(to)X
2532(determine)X
2907(if)X
2983(it)X
3055(is)X
3136(an)X
3241(inferior)X
3527(\(subwindow\))X
775 2720(of)N
870(a)X
931(window)X
1236(created)X
1512(by)X
1622(the)X
1752(client.)X
2015(\(The)X
2203(save-set)X
2508(is)X
2589(a)X
2650(list)X
2781(of)X
2876(other)X
3079(clients)X
3332(windows,)X
3693(and)X
3842(win-)X
775 2816(dows)N
982(in)X
1073(this)X
1223(list)X
1354(are)X
1483(referred)X
1782(to)X
1873(as)X
1968(a)X
2029(save-set)X
2334(window.\))X
2690(If)X
2770(so,)X
2892(the)X
3022(X)X
3107(server)X
3343(reparents)X
3687(the)X
775 2912(save-set)N
1080(window)X
1385(to)X
1476(the)X
1606(closest)X
1868(ancestor)X
2183(such)X
2366(that)X
2521(the)X
2651(save-set)X
2956(window)X
3261(is)X
3342(not)X
3477(an)X
3582(inferior)X
3868(of)X
3963(a)X
775 3008(window)N
1080(created)X
1356(by)X
1466(the)X
1596(client.)X
10 f
555 3132(g)N
1 f
775(Performs)X
1119(a)X
5 f
1198(MapWindow)X
1 f
1718(request)X
1994(on)X
2104(the)X
2234(save-set)X
2539(window)X
2844(if)X
2920(the)X
3050(save-set)X
3355(window)X
3660(is)X
775 3228(unmapped.)N
1208(The)X
1367(X)X
1452(server)X
1688(does)X
1871(this)X
2021(even)X
2209(if)X
2285(the)X
2415(save-set)X
2720(window)X
3025(was)X
3183(not)X
3318(an)X
3423(inferior)X
3709(of)X
3804(a)X
775 3324(window)N
1080(created)X
1356(by)X
1466(the)X
1596(client.)X
10 f
555 3448(g)N
1 f
775(Examines)X
1145(each)X
1328(window)X
1633(in)X
1724(the)X
1854(client's)X
2136(save-set,)X
2463(and)X
2612(destroys)X
2927(all)X
3038(windows)X
3377(created)X
3653(by)X
3763(the)X
775 3544(client.)N
10 f
555 3668(g)N
1 f
775(Performs)X
1119(the)X
1249(appropriate)X
1672(free)X
1830(request)X
2106(on)X
2216(each)X
2399(nonwindow)X
2836(resource)X
3155(created)X
3431(by)X
3541(the)X
3671(client)X
3890(in)X
775 3764(the)N
905(server)X
1141(\(for)X
1294(example,)X
1637(Font,)X
1843(Pixmap,)X
2157(Cursor,)X
2440(Colormap,)X
2837(and)X
2986(GContext\).)X
10 f
555 3888(g)N
1 f
775(Frees)X
987(all)X
1098(colors)X
1335(and)X
1484(color)X
1687(map)X
1861(entries)X
2118(allocated)X
2459(by)X
2569(a)X
2630(client)X
2849(application.)X
555 4012(Additional)N
955(processing)X
1353(occurs)X
1604(when)X
1816(the)X
1946(last)X
2091(connection)X
2500(to)X
2591(the)X
2721(X)X
2806(server)X
3042(closes.)X
3323(An)X
3452(X)X
3537(server)X
3773(goes)X
555 4108(through)N
851(a)X
912(cycle)X
1120(of)X
1215(having)X
1477(no)X
1587(connections)X
2030(and)X
2179(having)X
2441(some)X
2649(connections.)X
3136(When)X
3368(the)X
3498(last)X
3643(display)X
555 4204(connection)N
964(to)X
1055(the)X
1185(X)X
1270(server)X
1506(closes)X
1743(as)X
1838(a)X
1899(result)X
2117(of)X
2212(a)X
2273(connection)X
2682(closing)X
2959(with)X
3138(the)X
3268(close_mode)X
3711(argu-)X
555 4300(ment)N
5 f
772(DestroyAll)X
1 f
1209(\(that)X
1393(is,)X
1496(the)X
1626(X)X
1711(server)X
1947(reverts)X
2208(to)X
2299(the)X
2429(state)X
2613(of)X
2708(having)X
2970(no)X
3080(connections\),)X
3574(the)X
3704(X)X
555 4396(server:)N
10 f
555 4520(g)N
1 f
775(Resets)X
1027(its)X
1133(state,)X
1339(as)X
1434(if)X
1510(it)X
1582(had)X
1731(just)X
1881(been)X
2069(started.)X
2348(The)X
2507(X)X
2592(server)X
2828(begins)X
3080(by)X
3190(destroying)X
3584(all)X
3695(linger-)X
775 4616(ing)N
910(resources)X
1263(from)X
1456(clients)X
1709(that)X
1864(have)X
2052(terminated)X
2452(in)X
5 f
2561(RetainPermanent)X
1 f
3283(or)X
5 f
3396(RetainTem-)X
775 4712(porary)N
1 f
1061(mode.)X
10 f
555 4836(g)N
1 f
775(Deletes)X
1061(all)X
1172(but)X
1307(the)X
1437(prede\256ned)X
1830(atom)X
2029(identi\256ers.)X
10 f
555 4960(g)N
1 f
775(Deletes)X
1061(all)X
1172(properties)X
1546(on)X
1656(all)X
1767(root)X
1931(windows.)X
2314(See)X
2463(Chapter)X
2764(4)X
2830(for)X
2954(information)X
3393(about)X
3611(properties.)X
10 f
555 5084(g)N
1 f
775(Resets)X
1027(all)X
1138(device)X
1390(maps)X
1598(and)X
1747(attributes)X
2098(\(for)X
2251(example,)X
2594(key)X
2743(click,)X
2959(bell)X
3114(volume,)X
3423(and)X
3572(accelera-)X
775 5180(tion\))N
964(and)X
1113(the)X
1243(access)X
1489(control)X
1761(list.)X
10 f
555 5304(g)N
1 f
775(Restores)X
1100(the)X
1230(standard)X
1550(root)X
1714(tiles)X
1884(and)X
2033(cursors.)X
10 f
555 5428(g)N
1 f
775(Restores)X
1100(the)X
1230(default)X
1497(font)X
1661(path.)X
10 f
555 5552(g)N
1 f
775(Restores)X
1100(the)X
1230(input)X
1434(focus)X
1646(to)X
1737(state)X
5 f
1939(PointerRoot)X
1 f
2419(.)X
555 5676(However,)N
920(the)X
1050(X)X
1135(server)X
1371(does)X
1554(not)X
1689(reset)X
1877(if)X
1953(you)X
2107(close)X
2310(a)X
2371(connection)X
2780(with)X
2959(a)X
3020(close_down)X
3462(mode)X
3680(argu-)X
555 5772(ment)N
754(set)X
874(to)X
5 f
983(RetainPermanent)X
1 f
1705(or)X
5 f
1818(RetainTemporary)X
1 f
2512(.)X
3 f
2239 6144(15)N

16 p
%%Page: 16 20
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(3)X
1895 1056(Window)N
2265(Functions)X
1 f
11 s
555 1372(In)N
650(the)X
780(X)X
865(window)X
1170(system,)X
1459(a)X
1520(window)X
1825(is)X
1906(a)X
1967(rectangular)X
2385(area)X
2553(on)X
2663(the)X
2793(screen)X
3039(that)X
3194(lets)X
3339(you)X
3493(view)X
3686(graphi-)X
555 1468(cal)N
680(output.)X
972(Client)X
1211(applications)X
1660(can)X
1804(display)X
2081(overlapping)X
2524(and)X
2673(nested)X
2920(windows)X
3259(on)X
3369(one)X
3518(or)X
3613(more)X
555 1564(screens)N
835(that)X
990(are)X
1119(driven)X
1366(by)X
1476(X)X
1561(servers)X
1831(on)X
1941(one)X
2090(or)X
2185(more)X
2388(machines.)X
2787(Clients)X
3060(who)X
3233(want)X
3426(to)X
3517(create)X
3749(win-)X
555 1660(dows)N
762(must)X
956(\256rst)X
1115(connect)X
1411(their)X
1595(program)X
1915(to)X
2006(the)X
2136(X)X
2221(server)X
2457(by)X
2567(calling)X
2830(the)X
2960(Xlib)X
3139(function)X
5 f
3473(XOpen-)X
555 1756(Display)N
1 f
857(.)X
923(This)X
1102(chapter)X
1383(begins)X
1635(with)X
1814(a)X
1875(discussion)X
2264(of)X
2359(visual)X
2592(types)X
2800(and)X
2949(window)X
3254(attributes.)X
3649(The)X
555 1852(chapter)N
836(continues)X
1196(with)X
1375(a)X
1436(discussion)X
1825(of)X
1920(the)X
2050(Xlib)X
2229(functions)X
2579(you)X
2733(can)X
2877(use)X
3016(to:)X
10 f
555 1976(g)N
1 f
775(Create)X
1027(windows)X
10 f
555 2100(g)N
1 f
775(Destroy)X
1075(windows)X
10 f
555 2224(g)N
1 f
775(Map)X
958(windows)X
10 f
555 2348(g)N
1 f
775(Unmap)X
1056(windows)X
10 f
555 2472(g)N
1 f
775(Con\256gure)X
1149(windows)X
10 f
555 2596(g)N
1 f
775(Change)X
1066(the)X
1196(stacking)X
1512(order)X
10 f
555 2720(g)N
1 f
775(Change)X
1066(window)X
1371(attributes)X
10 f
555 2844(g)N
1 f
775(Translate)X
1125(window)X
1430(coordinates)X
2197 3020(Note)N
775 3156(Your)N
977(application)X
1392(should)X
1649(seldom)X
1926(directly)X
2218(create)X
2450(windows,)X
2811(particularly)X
3240(top-level)X
3576(win-)X
775 3252(dows.)N
1026(If)X
1106(you)X
1260(use)X
1399(a)X
1460(toolkit,)X
1736(the)X
1866(toolkit)X
2120(will)X
2280(do)X
2390(most)X
2584(of)X
2679(your)X
2862(window-creating)X
3480(for)X
3604(you.)X
775 3348(See)N
924(the)X
1054(documentation)X
1601(for)X
1725(your)X
1908(toolkit.)X
555 3552(This)N
734(chapter)X
1015(also)X
1179(identi\256es)X
1525(the)X
1655(window)X
1960(actions)X
2232(that)X
2387(may)X
2561(generate)X
2881(events.)X
3172(See)X
3321(Chapter)X
3622(8)X
3688(for)X
3812(a)X
555 3648(complete)N
901(discussion)X
1290(of)X
1385(events.)X
3 f
555 3840(3.1.)N
731(Visual)X
993(Types)X
1 f
555 3964(On)N
684(some)X
892(high)X
1071(end)X
1220(displays,)X
1553(it)X
1625(may)X
1799(be)X
1904(possible)X
2215(to)X
2306(deal)X
2475(with)X
2654(color)X
2857(resources)X
3210(in)X
3301(more)X
3504(than)X
3678(one)X
3827(way.)X
555 4060(For)N
699(example,)X
1042(you)X
1196(may)X
1370(be)X
1475(able)X
1644(to)X
1735(deal)X
1904(with)X
2083(the)X
2213(display)X
2490(either)X
2713(as)X
2808(a)X
2869(12-bit)X
3102(display)X
3379(with)X
3558(arbitrary)X
555 4156(mapping)N
886(of)X
981(pixel)X
1180(to)X
1271(color)X
1474(\(pseudo-color\))X
2013(or)X
2108(as)X
2203(a)X
2264(24-bit)X
2497(display)X
2774(with)X
2953(8)X
3019(bits)X
3169(of)X
3264(the)X
3394(pixel)X
3593(dedicated)X
555 4252(for)N
679(red,)X
835(green,)X
1074(and)X
1223(blue.)X
1441(These)X
1673(different)X
1998(ways)X
2200(of)X
2295(dealing)X
2577(with)X
2756(the)X
2886(visual)X
3119(aspects)X
3395(of)X
3490(the)X
3620(display)X
3897(are)X
555 4348(called)N
788(Visuals.)X
1118(For)X
1262(each)X
1445(screen)X
1691(of)X
1786(the)X
1916(display,)X
2215(there)X
2413(may)X
2587(be)X
2692(a)X
2753(list)X
2884(of)X
2979(valid)X
3178(visual)X
3411(types)X
3619(supported)X
555 4444(at)N
641(different)X
966(depths)X
1218(of)X
1313(the)X
1443(display.)X
1764(Because)X
2079(there)X
2277(are)X
2406(default)X
2673(windows)X
3012(and)X
3161(visual)X
3394(types)X
3602(de\256ned)X
3883(for)X
555 4540(each)N
738(screen,)X
1006(most)X
1200(simple)X
1458(applications)X
1907(need)X
2095(not)X
2230(deal)X
2399(with)X
2578(this)X
2728(complexity.)X
3192(Xlib)X
3371(provides)X
3696(macros)X
555 4636(and)N
704(functions)X
1054(that)X
1209(return)X
1441(the)X
1571(default)X
1838(root)X
2002(window,)X
2329(the)X
2459(default)X
2726(depth)X
2944(of)X
3039(the)X
3169(default)X
3436(root)X
3600(window,)X
555 4732(and)N
704(the)X
834(default)X
1101(visual)X
1334(type.)X
1552(See)X
1701(Chapter)X
2002(2)X
2068(for)X
2192(information)X
2631(on)X
2741(these)X
2944(macros)X
3220(and)X
3369(functions.)X
3763(See)X
5 f
570 4828(XMatchVisualInfo)N
1 f
1293(in)X
1384(Chapter)X
1685(10)X
1795(for)X
1919(information)X
2358(about)X
2576(how)X
2749(to)X
2840(\256nd)X
2999(the)X
3129(visual)X
3362(type)X
3536(you)X
3690(need.)X
555 4952(Xlib)N
734(uses)X
907(a)X
5 f
986(Visual)X
1 f
1262(structure)X
1592(that)X
1747(contains)X
2063(information)X
2502(about)X
2720(the)X
2850(possible)X
3161(color)X
3364(mapping.)X
3739(The)X
555 5048(members)N
900(of)X
995(this)X
1145(structure)X
1475(pertinent)X
1811(to)X
1902(this)X
2052(discussion)X
2441(are)X
2570(class,)X
2785(red_mask,)X
3171(green_mask,)X
555 5144(blue_mask,)N
981(bits_per_rgb,)X
1470(and)X
1619(map_entries.)X
2116(The)X
2275(class)X
2468(member)X
2779(speci\256es)X
3104(the)X
3234(possible)X
3545(visual)X
555 5240(classes)N
821(of)X
916(the)X
1046(screen.)X
1314(It)X
1390(can)X
1534(be)X
1639(one)X
1788(of)X
1883(the)X
2013(constants)X
5 f
2381(StaticGray)X
1 f
2808(,)X
5 f
2870(StaticColor)X
1 f
3315(,)X
5 f
3377(TrueColor)X
1 f
3782(,)X
5 f
570 5336(GrayScale)N
1 f
996(,)X
5 f
1058(PseudoColor)X
1 f
1581(,)X
1625(or)X
5 f
1738(DirectColor)X
1 f
2191(.)X
555 5460(Conceptually,)N
1070(as)X
1165(each)X
1348(pixel)X
1547(is)X
1628(read)X
1801(out)X
1936(of)X
2031(memory,)X
2369(it)X
2441(goes)X
2624(through)X
2920(a)X
2981(lookup)X
3248(stage)X
3451(by)X
3561(indexing)X
555 5556(into)N
715(a)X
776(colormap.)X
1175(Colormaps)X
1584(can)X
1728(be)X
1833(manipulated)X
2292(arbitrarily)X
2667(on)X
2777(some)X
2985(hardware,)X
3355(in)X
3446(limited)X
3720(way)X
3888(on)X
555 5652(other)N
758(hardware,)X
1128(and)X
1277(not)X
1412(at)X
1498(all)X
1609(on)X
1719(yet)X
1849(other)X
2052(hardware.)X
2422(The)X
2581(visual)X
2814(types)X
3022(affect)X
3244(the)X
3374(color)X
3577(map)X
3751(and)X
3900(the)X
555 5748(RGB)N
758(values)X
1005(in)X
1096(the)X
1226(following)X
1591(ways:)X
3 f
2239 6144(16)N

17 p
%%Page: 17 21
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(For)X
5 f
937(PseudoColor)X
1 f
1460(,)X
1504(a)X
1565(pixel)X
1764(value)X
1977(indexes)X
2268(a)X
2329(color)X
2532(map)X
2706(to)X
2797(produce)X
3102(independent)X
3555(RGB)X
3758(values,)X
775 768(and)N
924(the)X
1054(RGB)X
1257(values)X
1504(can)X
1648(be)X
1753(changed)X
2068(dynamically.)X
10 f
555 892(g)N
1 f
5 f
790(GrayScale)X
1 f
1238(is)X
1319(treated)X
1581(the)X
1711(same)X
1914(as)X
5 f
2027(PseudoColor)X
1 f
2550(,)X
2594(except)X
2846(the)X
2976(primary)X
3277(which)X
3514(drives)X
3751(the)X
775 988(screen)N
1021(is)X
1102(unde\256ned.)X
1493(Thus,)X
1713(the)X
1843(client)X
2062(should)X
2319(always)X
2585(store)X
2778(the)X
2908(same)X
3111(value)X
3324(for)X
3448(red,)X
3604(green,)X
3843(and)X
775 1084(blue)N
949(in)X
1040(the)X
1170(color)X
1373(maps.)X
10 f
555 1208(g)N
1 f
775(For)X
5 f
937(DirectColor)X
1 f
1390(,)X
1434(a)X
1495(pixel)X
1694(value)X
1907(is)X
1988(decomposed)X
2450(into)X
2610(separate)X
2920(RGB)X
3123(sub\256elds,)X
3480(and)X
3629(each)X
775 1304(sub\256eld)N
1076(separately)X
1455(indexes)X
1746(the)X
1876(color)X
2079(map)X
2253(for)X
2377(the)X
2507(corresponding)X
3032(value.)X
3289(The)X
3448(RGB)X
3651(values)X
775 1400(can)N
919(be)X
1024(changed)X
1339(dynamically.)X
10 f
555 1524(g)N
1 f
5 f
790(TrueColor)X
1 f
1217(is)X
1298(treated)X
1560(the)X
1690(same)X
1893(as)X
5 f
2006(DirectColor)X
1 f
2459(,)X
2503(except)X
2755(the)X
2885(color)X
3088(map)X
3262(has)X
3401(prede\256ned)X
3794(read-)X
775 1620(only)N
954(RGB)X
1157(values.)X
1448(These)X
1680(RGB)X
1883(values)X
2130(are)X
2259(server-dependent,)X
2908(but)X
3043(provide)X
3334(\(near-\)linear)X
3795(ramps)X
775 1716(in)N
866(each)X
1049(primary.)X
10 f
555 1840(g)N
1 f
5 f
790(StaticColor)X
1 f
1257(is)X
1338(treated)X
1600(the)X
1730(same)X
1933(as)X
5 f
2046(PseudoColor)X
1 f
2569(,)X
2613(except)X
2865(the)X
2995(color)X
3198(map)X
3372(has)X
3511(prede\256ned)X
775 1936(read-only)N
1134(server-dependent)X
1761(RGB)X
1964(values.)X
10 f
555 2060(g)N
1 f
5 f
790(StaticGray)X
1 f
1239(is)X
1320(treated)X
1582(the)X
1712(same)X
1915(as)X
5 f
2028(StaticColor)X
1 f
2473(,)X
2517(except)X
2769(the)X
2899(red,)X
3055(green,)X
3294(and)X
3443(blue)X
3617(values)X
3864(are)X
775 2156(equal)N
988(for)X
1112(any)X
1261(single)X
1494(pixel)X
1693(value,)X
1928(thus)X
2097(resulting)X
2428(in)X
2519(shades)X
2775(of)X
2870(gray.)X
5 f
3088(StaticGray)X
1 f
3537(with)X
3716(a)X
3777(two-)X
775 2252(entry)N
978(colormap)X
1333(can)X
1477(be)X
1582(thought)X
1874(of)X
1969(as)X
2064(monochrome.)X
555 2376(The)N
714(red_mask,)X
1100(green_mask,)X
1569(and)X
1718(blue_mask)X
2122(members)X
2467(are)X
2596(only)X
2775(de\256ned)X
3056(for)X
5 f
3198(DirectColor)X
1 f
3673(and)X
5 f
570 2472(TrueColor)N
1 f
975(.)X
1041(Each)X
1239(has)X
1378(one)X
1527(contiguous)X
1936(set)X
2056(of)X
2151(bits)X
2301(with)X
2480(no)X
2590(intersections.)X
3102(The)X
3261(bits_per_rgb)X
555 2568(member)N
866(speci\256es)X
1191(the)X
1321(log)X
1456(base)X
1634(2)X
1700(of)X
1795(the)X
1925(approximate)X
2388(number)X
2679(of)X
2774(distinct)X
3057(color)X
3260(values)X
3507(\(individually\))X
555 2664(of)N
650(red,)X
806(green,)X
1045(and)X
1194(blue.)X
1412(Actual)X
1669(RGB)X
1872(values)X
2119(are)X
2248(unsigned)X
2588(16)X
2698(bit)X
2814(numbers.)X
3183(The)X
3342(map_entries)X
555 2760(member)N
866(de\256nes)X
1137(the)X
1267(number)X
1558(of)X
1653(available)X
1994(color)X
2197(map)X
2371(entries)X
2628(in)X
2719(a)X
2780(newly)X
3017(created)X
3293(color)X
3496(map.)X
3692(For)X
5 f
570 2856(DirectColor)N
1 f
1045(and)X
5 f
1212(TrueColor)X
1 f
1617(,)X
1661(this)X
1811(will)X
1971(be)X
2076(the)X
2206(size)X
2365(of)X
2460(an)X
2565(individual)X
2946(pixel)X
3145(sub\256eld.)X
3490(The)X
3649(following)X
555 2952(concepts)N
885(may)X
1059(serve)X
1266(to)X
1357(make)X
1570(the)X
1700(explanation)X
2134(of)X
2229(Visual)X
2481(types)X
2689(clearer.)X
2972(The)X
3131(screen)X
3377(can)X
3521(be)X
3626(color)X
3829(or)X
555 3048(grayscale.)N
953(The)X
1112(screen)X
1358(can)X
1502(have)X
1690(a)X
1751(colormap)X
2106(that)X
2261(is)X
2342(writable)X
2653(or)X
2748(read-only.)X
3129(A)X
3214(screen)X
3460(can)X
3604(also)X
3768(have)X
3956(a)X
555 3144(colormap)N
910(whose)X
1156(indices)X
1428(are)X
1557(decomposed)X
2019(into)X
2179(separate)X
2489(RGB)X
2692(pieces,)X
2956(provided)X
3291(one)X
3440(is)X
3521(not)X
3656(on)X
3766(a)X
555 3240(grayscale)N
909(screen.)X
1177(This)X
1356(leads)X
1559(to)X
1650(the)X
1780(following)X
2145(diagram:)X
1419 3364(Color)N
1840(GrayScale)X
1275 3460(R/O)N
1554(R/W)X
1831(R/O)X
2066(R/W)X
1275 3556 -0.2793(+-------------------------------+)AN
555 3652(Undecomposed)N
1275 0.2062(|Static|Pseudo|Static)AX
2009(|Gray)X
2224(|)X
599 3748(Colormap)N
1275(|Color)X
1501(|Color)X
1749(|Gray)X
2008(|Scale|)X
1275 3844 -0.2793(+-------------------------------+)AN
577 3940(Decomposed)N
1275(|True)X
1503(|Direct|)X
599 4036(Colormap)N
1275(|Color)X
1509(|Color)X
1743(|)X
1275 4132 -0.2461(+---------------+)AN
3 f
555 4372(3.2.)N
731(Window)X
1071(Attributes)X
1 f
555 4496(All)N
690(windows)X
1029(have)X
1217(a)X
1278(border)X
1529(width)X
1752(of)X
1847(zero)X
2020(or)X
2115(more)X
2318(pixels,)X
2573(an)X
2678(optional)X
2990(background,)X
3449(an)X
3554(input)X
3758(mask,)X
555 4592(an)N
660(event)X
873(suppression)X
1310(mask,)X
1540(and)X
1689(a)X
1750(property)X
2070(list.)X
2245(The)X
2404(window)X
2709(border)X
2960(and)X
3109(background)X
3546(can)X
3690(be)X
3795(a)X
555 4688(solid)N
749(color)X
952(or)X
1047(a)X
1108(pattern,)X
1397(called)X
1630(a)X
1691(tile.)X
1871(All)X
2006(windows)X
2345(except)X
2597(the)X
2727(root)X
2891(have)X
3079(a)X
3140(parent)X
3382(and)X
3531(are)X
3660(clipped)X
555 4784(by)N
665(their)X
849(parent.)X
1135(If)X
1215(a)X
1276(window)X
1581(is)X
1662(stacked)X
1948(on)X
2058(top)X
2193(of)X
2288(another)X
2574(window,)X
2901(it)X
2973(obscures)X
3302(that)X
3457(other)X
3660(window)X
555 4880(for)N
679(the)X
809(purpose)X
1109(of)X
1204(input.)X
1452(If)X
1532(a)X
1593(window)X
1898(has)X
2037(a)X
2098(background)X
2535(\(almost)X
2822(all)X
2933(do\),)X
3094(it)X
3166(obscures)X
3495(the)X
3625(other)X
3828(win-)X
555 4976(dow)N
728(for)X
852(purposes)X
1186(of)X
1281(output.)X
1573(Attempts)X
1919(to)X
2010(output)X
2258(to)X
2349(the)X
2479(obscured)X
2818(area)X
2986(will)X
3146(do)X
3256(nothing,)X
3570(and)X
3719(no)X
3829(input)X
555 5072(events)N
802(\(for)X
955(example,)X
1298(pointer)X
1570(motion\))X
1872(will)X
2032(be)X
2137(generated)X
2501(for)X
2625(the)X
2755(obscured)X
3094(area.)X
555 5196(Windows)N
914(classi\256ed)X
1264(as)X
5 f
1377(InputOnly)X
1 f
1792(have)X
1980(only)X
2159(the)X
2289(following)X
2654(attributes)X
3005(de\256ned:)X
10 f
555 5320(g)N
1 f
775(win_gravity)X
10 f
555 5444(g)N
1 f
775(event_mask)X
10 f
555 5568(g)N
1 f
775(do_not_propagate_mask)X
10 f
555 5692(g)N
1 f
775(override_redirect)X
3 f
2239 6144(17)N

18 p
%%Page: 18 22
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(cursor)X
555 796(A)N
5 f
658(BadMatch)X
1 f
1092(error)X
1284(is)X
1365(generated)X
1729(if)X
1805(you)X
1959(specify)X
2235(any)X
2384(other)X
2587(attributes)X
2938(for)X
3062(an)X
5 f
3185(InputOnly)X
1 f
3600(window.)X
555 920(Windows)N
914(have)X
1102(borders)X
1387(of)X
1482(a)X
1543(programmable)X
2079(width)X
2302(and)X
2451(pattern)X
2718(as)X
2813(well)X
2987(as)X
3082(a)X
3143(background)X
3580(pattern)X
3847(or)X
555 1016(tile.)N
735(Pixels)X
973(can)X
1117(be)X
1222(used)X
1405(for)X
1529(solid)X
1723(colors.)X
2004(In)X
2099(a)X
2160(program,)X
2502(you)X
2656(refer)X
2843(to)X
2934(the)X
3064(window)X
3369(using)X
3582(its)X
3688(resource)X
555 1112(ID)N
669(of)X
764(type)X
5 f
956(Window)X
1 f
1283(.)X
1349(The)X
1508(background)X
1945(and)X
2094(border)X
2345(pixmaps)X
2666(may)X
2840(be)X
2945(destroyed)X
3309 0.3187(immediately)AX
3774(after)X
555 1208(creating)N
861(the)X
991(window)X
1296(if)X
1372(no)X
1482(further)X
1743(explicit)X
2031(references)X
2413(to)X
2504(them)X
2703(are)X
2832(to)X
2923(be)X
3028(made.)X
555 1332(A)N
640(window's)X
1008(background)X
1445(pattern)X
1712(can)X
1856(be)X
1961(either)X
2184(a)X
2245(solid)X
2439(color)X
2642(or)X
2737(a)X
2798(pattern.)X
3109(The)X
3268(pattern)X
3535(can)X
3679(either)X
3902(be)X
555 1428(relative)N
842(to)X
933(the)X
1063(parent)X
1305(or)X
1400(absolute.)X
1760(If)X
1840(relative)X
2127(to)X
2218(the)X
2348(parent,)X
2612(the)X
2742(pattern)X
3009(will)X
3169(be)X
3274(shifted)X
3536(appropriately)X
555 1524(to)N
646(match)X
884(the)X
1014(parent)X
1256(window.)X
1583(If)X
1663(absolute,)X
2001(the)X
2131(pattern)X
2398(will)X
2558(be)X
2663(positioned)X
3053(in)X
3144(the)X
3274(window)X
3579(indepen-)X
555 1620(dently)N
798(of)X
893(the)X
1023(parent)X
1265(window.)X
555 1744(When)N
787(windows)X
1126(are)X
1255(\256rst)X
1414(created,)X
1712(they)X
1886(are)X
2015(not)X
2150(visible)X
2408(on)X
2518(the)X
2648(screen.)X
2938(Any)X
3111(output)X
3359(to)X
3450(a)X
3511(window)X
3816(not)X
555 1840(visible)N
813(\(not)X
977(mapped\))X
1307(on)X
1417(the)X
1547(screen)X
1793(will)X
1953(be)X
2058(discarded.)X
2461(An)X
2590(application)X
3005(may)X
3179(wish)X
3367(to)X
3458(create)X
3690(a)X
3751(win-)X
555 1936(dow)N
728(long)X
907(before)X
1153(it)X
1225(is)X
1306(mapped)X
1607(to)X
1698(the)X
1828(screen.)X
2118(When)X
2350(a)X
2411(window)X
2716(is)X
2797(eventually)X
3187(mapped)X
3488(to)X
3579(the)X
3709(screen)X
555 2032(\(using)N
5 f
815(XMapWindow)X
1 f
1372(\),)X
1445(the)X
1575(X)X
1660(server)X
1896(will)X
2056(generate)X
2376(an)X
2481(exposure)X
2820(event)X
3033(for)X
3157(the)X
3287(window.)X
555 2156(A)N
640(window)X
945(manager)X
1270(may)X
1444(override)X
1759(your)X
1942(choice)X
2194(as)X
2289(to)X
2380(size,)X
2561(border)X
2812(width,)X
3057(and)X
3206(style)X
3395(for)X
3519(a)X
3580(window.)X
555 2252(Your)N
757(program)X
1077(must)X
1271(be)X
1376(prepared)X
1705(to)X
1796(use)X
1935(the)X
2065(actual)X
2298(size)X
2457(and)X
2606(position)X
2913(of)X
3008(the)X
3138(top)X
3273(window,)X
3600(which)X
3837(is)X
555 2348(reported)N
870(when)X
1082(the)X
1212(window)X
1517(is)X
1598(\256rst)X
1757(mapped.)X
2102(It)X
2178(is)X
2259(not)X
2394(acceptable)X
2788(for)X
2912(a)X
2973(client)X
3192(application)X
3607(to)X
3698(resize)X
555 2444(itself)N
754(unless)X
996(in)X
1087(direct)X
1310(response)X
1639(to)X
1730(a)X
1791(human)X
2053(command)X
2423(to)X
2514(do)X
2624(so.)X
2768(Instead,)X
3066(your)X
3249(program)X
3569(should)X
555 2540(either)N
778(use)X
917(the)X
1047(space)X
1264(given)X
1482(to)X
1573(it,)X
1667(or,)X
1784(if)X
1860(the)X
1990(space)X
2207(is)X
2288(too)X
2423(small)X
2637(for)X
2761(any)X
2910(useful)X
3147(work,)X
3371(your)X
3554(program)X
555 2636(might)N
784(ask)X
923(the)X
1053(user)X
1221(to)X
1312(resize)X
1539(the)X
1669(window.)X
2018(The)X
2177(border)X
2428(of)X
2523(your)X
2706(top-level)X
3042(windows)X
3381(are)X
3510(considered)X
555 2732(fair)N
699(game)X
912(for)X
1036(window)X
1341(managers.)X
555 2856(The)N
714(following)X
1079(symbols)X
1395(and)X
1544(the)X
5 f
1692(XSetWindowAttributes)X
1 f
2606(structure)X
2936(are)X
3065(used)X
3248(in)X
3339(the)X
3469(functions)X
3819(that)X
555 2952(follow.)N
555 3076(#de\256ne)N
836(CWBackPixmap)X
2283(\(1L<<0\))X
555 3172(#de\256ne)N
836(CWBackPixel)X
2283(\(1L<<1\))X
555 3268(#de\256ne)N
836(CWBorderPixmap)X
2283(\(1L<<2\))X
555 3364(#de\256ne)N
836(CWBorderPixel)X
2283(\(1L<<3\))X
555 3460(#de\256ne)N
836(CWBitGravity)X
2283(\(1L<<4\))X
555 3556(#de\256ne)N
836(CWWinGravity)X
2283(\(1L<<5\))X
555 3652(#de\256ne)N
836(CWBackingStore)X
2283(\(1L<<6\))X
555 3748(#de\256ne)N
836(CWBackingPlanes)X
2283(\(1L<<7\))X
555 3844(#de\256ne)N
836(CWBackingPixel)X
2283(\(1L<<8\))X
555 3940(#de\256ne)N
836(CWOverrideRedirect)X
2283(\(1L<<9\))X
555 4036(#de\256ne)N
836(CWSaveUnder)X
2283(\(1L<<10\))X
555 4132(#de\256ne)N
836(CWEventMask)X
2283(\(1L<<11\))X
555 4228(#de\256ne)N
836(CWDontPropagate)X
2283(\(1L<<12\))X
555 4324(#de\256ne)N
836(CWColormap)X
2283(\(1L<<13\))X
555 4420(#de\256ne)N
836(CWCursor)X
2283(\(1L<<14\))X
3 f
2239 6144(18)N

19 p
%%Page: 19 23
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 768(typedef)N
841(struct)X
1059({)X
843 864(Pixmap)N
1135(background_pixmap;)X
2283(/*)X
2374(background,)X
2833(None,)X
3067(or)X
3162(ParentRelative)X
3704(*/)X
843 960(unsigned)N
1183(long)X
1362(background_pixel;)X
2283(/*)X
2374(background)X
2811(pixel)X
3010(*/)X
843 1056(Pixmap)N
1135(border_pixmap;)X
2283(/*)X
2374(border)X
2625(of)X
2720(the)X
2850(window)X
3155(*/)X
843 1152(unsigned)N
1183(long)X
1362(border_pixel;)X
2283(/*)X
2374(border)X
2625(pixel)X
2824(value)X
3037(*/)X
843 1248(int)N
959(bit_gravity;)X
2283(/*)X
2374(one)X
2523(of)X
2618(bit)X
2734(gravity)X
3006(values)X
3253(*/)X
843 1344(int)N
959(win_gravity;)X
2283(/*)X
2374(one)X
2523(of)X
2618(the)X
2748(window)X
3053(gravity)X
3325(values)X
3572(*/)X
843 1440(int)N
959(backing_store;)X
2283(/*)X
2374(NotUseful,)X
2784(WhenMapped,)X
3326(Always)X
3616(*/)X
843 1536(unsigned)N
1183(long)X
1362(backing_planes;)X
2283(/*)X
2374(planes)X
2621(to)X
2712(be)X
2817(preserved)X
3180(if)X
3256(possible)X
3567(*/)X
843 1632(unsigned)N
1183(long)X
1362(backing_pixel;)X
2283(/*)X
2374(value)X
2587(to)X
2678(use)X
2817(in)X
2908(restoring)X
3243(planes)X
3490(*/)X
843 1728(Bool)N
1037(save_under;)X
2283(/*)X
2374(should)X
2631(bits)X
2781(under)X
3003(be)X
3108(saved?)X
3369(\(popups\))X
3703(*/)X
843 1824(long)N
1022(event_mask;)X
2283(/*)X
2374(set)X
2494(of)X
2589(events)X
2836(that)X
2991(should)X
3248(be)X
3353(saved)X
3575(*/)X
843 1920(long)N
1022(do_not_propagate_mask;)X
2283(/*)X
2374(set)X
2494(of)X
2589(events)X
2836(that)X
2991(should)X
3248(not)X
3383(propagate)X
3752(*/)X
843 2016(Bool)N
1037(override_redirect;)X
2283(/*)X
2374(boolean)X
2675(value)X
2888(for)X
3012(override_redirect)X
3640(*/)X
843 2112(Colormap)N
1218(colormap;)X
2283(/*)X
2374(color)X
2577(map)X
2751(to)X
2842(be)X
2947(associated)X
3331(with)X
3510(window)X
3815(*/)X
843 2208(Cursor)N
1104(cursor;)X
2283(/*)X
2374(cursor)X
2615(to)X
2706(be)X
2811(displayed)X
3171(\(or)X
3295(None\))X
3536(*/)X
555 2304(})N
619(XSetWindowAttributes;)X
555 2476(The)N
714(XSetWindowAttributes)X
1568(structure)X
1898(members)X
2243(are)X
2372(discussed)X
2731(in)X
2822(the)X
2952(following)X
3317(sections.)X
3 f
555 2668(3.2.1.)N
797(The)X
966(background_pixmap)X
1771(and)X
1935(background_pixel)X
2638(Members)X
1 f
555 2792(The)N
714(background_pixmap)X
1460(member)X
1771(speci\256es)X
2096(the)X
2226(pixmap)X
2513(to)X
2604(be)X
2709(used)X
2892(for)X
3016(a)X
3077(window's)X
3445(background.)X
555 2888(This)N
734(pixmap)X
1021(can)X
1165(be)X
1270(of)X
1365(any)X
1514(size,)X
1695(although)X
2026(some)X
2234(sizes)X
2427(may)X
2601(be)X
2706(faster)X
2923(than)X
3097(others.)X
3378(The)X
555 2984(background_pixel)N
1213(member)X
1524(speci\256es)X
1849(a)X
1910(pixel)X
2109(value)X
2322(used)X
2505(to)X
2596(paint)X
2795(a)X
2856(window's)X
3224(background)X
3661(in)X
3752(a)X
3813(sin-)X
555 3080(gle)N
685(color.)X
555 3204(You)N
728(can)X
872(set)X
992(the)X
1144(background_pixmap)X
1890(member)X
2201(to)X
2292(a)X
2353(pixmap,)X
2684(the)X
2814(constant)X
5 f
3148(None)X
1 f
3373(,)X
3417(or)X
3512(the)X
3642(constant)X
5 f
570 3300(ParentRelative)N
1 f
1162(.)X
1228(The)X
1387(default)X
1654(value)X
1867(is)X
1948(the)X
2078(constant)X
5 f
2412(None)X
1 f
2637(.)X
2703(You)X
2876(can)X
3020(set)X
3140(the)X
3270(background_pixel)X
3928(to)X
555 3396(any)N
704(pixel)X
903(value.)X
1160(The)X
1319(default)X
1586(value)X
1799(is)X
1880(unde\256ned.)X
2293(If)X
2373(you)X
2527(specify)X
2803(a)X
2864(background_pixel,)X
3544(it)X
3616(overrides)X
555 3492(either)N
778(the)X
908(default)X
1175(background_pixmap)X
1921(or)X
2016(any)X
2165(value)X
2378(you)X
2532(may)X
2706(have)X
2894(set)X
3014(in)X
3105(the)X
3235(background_pixmap)X
555 3588(member.)N
910(All)X
1045(pixels)X
1278(in)X
1369(the)X
1499(background)X
1936(of)X
2031(the)X
2161(window)X
2466(will)X
2626(be)X
2731(set)X
2851(to)X
2942(this)X
3092(value.)X
555 3712(If)N
635(you)X
789(set)X
909(the)X
1039(background_pixmap,)X
1807(it)X
1879(overrides)X
2228(the)X
2358(default)X
2625(background_pixmap.)X
3415(The)X
555 3808(background_pixmap)N
1301(and)X
1450(the)X
1580(window)X
1885(must)X
2079(have)X
2267(the)X
2397(same)X
2600(depth.)X
2862(Otherwise,)X
3267(a)X
5 f
3346(BadMatch)X
1 f
3780(error)X
555 3904(is)N
636(returned.)X
995(If)X
1075(you)X
1229(set)X
1349(background_pixmap)X
2095(to)X
5 f
2204(None)X
1 f
2429(,)X
2473(the)X
2603(window)X
2908(has)X
3047(no)X
3157(de\256ned)X
3438(background.)X
3897(If)X
555 4000(the)N
685(parent)X
927(window)X
1232(has)X
1371(a)X
1432(background_pixmap)X
2178(of)X
5 f
2291(None)X
1 f
2516(,)X
2560(the)X
2690(window)X
2995(will)X
3155(also)X
3319(have)X
3507(a)X
555 4096(background_pixmap)N
1301(of)X
5 f
1414(None)X
1 f
1639(.)X
1705(If)X
1785(you)X
1939(set)X
2059(the)X
2189(background_pixmap)X
2935(to)X
5 f
3044(ParentRelative)X
1 f
3636(:)X
10 f
555 4220(g)N
1 f
775(The)X
934(parent)X
1176(window's)X
1544(background_pixmap)X
2290(is)X
2371(used,)X
2576(but)X
2711(the)X
2841(child)X
3040(window)X
3345(must)X
3539(have)X
3727(the)X
775 4316(same)N
978(depth)X
1196(as)X
1291(its)X
1397(parent.)X
1683(Otherwise,)X
2088(a)X
5 f
2167(BadMatch)X
1 f
2601(error)X
2793(is)X
2874(returned.)X
10 f
555 4440(g)N
1 f
775(A)X
860(copy)X
1053(of)X
1148(the)X
1278(parent)X
1520(window's)X
1888(background_pixmap)X
2634(is)X
2715(not)X
2850(made.)X
3107(The)X
3266(parent's)X
775 4536(background_pixmap)N
1521(is)X
1602(examined)X
1967(each)X
2150(time)X
2330(the)X
2460(child)X
2659(window)X
2964(background_pixmap)X
3710(is)X
775 4632(required.)N
10 f
555 4756(g)N
1 f
775(The)X
934(background)X
1371(tile)X
1507(origin)X
1740(always)X
2006(aligns)X
2239(with)X
2418(the)X
2548(parent)X
2790(window's)X
3158(background)X
3595(tile)X
3731(origin.)X
775 4852(Otherwise,)N
1180(the)X
1310(background)X
1747(tile)X
1883(origin)X
2116(is)X
2197(always)X
2463(the)X
2593(child)X
2792(window)X
3097(origin.)X
555 4976(Setting)N
828(a)X
889(new)X
1057(background,)X
1516(whether)X
1821(by)X
1931(setting)X
2189(background_pixmap)X
2935(or)X
3030(background_pixel,)X
3710(over-)X
555 5072(rides)N
748(any)X
897(previous)X
1222(border.)X
1517(The)X
1676(background_pixmap)X
2422(can)X
2566(be)X
2671(freed)X
2873 0.3187(immediately)AX
3338(if)X
3414(no)X
3524(further)X
3785(expli-)X
555 5168(cit)N
666(reference)X
1014(is)X
1095(made)X
1308(to)X
1399(it)X
1471(\(the)X
1630(X)X
1715(server)X
1951(will)X
2111(keep)X
2299(a)X
2360(copy)X
2553(to)X
2644(use)X
2783(when)X
2995(needed\).)X
3339(If)X
3419(you)X
3573(later)X
3752(draw)X
555 5264(into)N
715(the)X
845(pixmap)X
1132(used)X
1315(for)X
1439(the)X
1569(background,)X
2028(X)X
2113(does)X
2296(not)X
2431(predict)X
2698(what)X
2891(happens)X
3201(because)X
3501(the)X
3631(X)X
3716(imple-)X
555 5360(mentation)N
931(is)X
1012(free)X
1170(to)X
1261(either)X
1484(make)X
1697(a)X
1758(copy)X
1951(of)X
2046(the)X
2176(pixmap)X
2463(or)X
2558(just)X
2708(use)X
2847(the)X
2977(same)X
3180(pixmap.)X
555 5484(When)N
787(regions)X
1068(of)X
1163(the)X
1293(window)X
1598(are)X
1727(exposed)X
2037(and)X
2186(the)X
2316(X)X
2401(server)X
2637(has)X
2776(not)X
2911(retained)X
3217(the)X
3347(contents,)X
3685(it)X
555 5580 0.2604(automatically)AN
1059(tiles)X
1229(the)X
1359(regions)X
1640(with)X
1819(the)X
1949(window's)X
2317(background)X
2754(as)X
2849(long)X
3028(as)X
3123(the)X
3253(background_pixmap)X
555 5676(is)N
636(not)X
5 f
789(None)X
1 f
1014(.)X
1080(If)X
1160(the)X
1290(background_pixmap)X
2036(is)X
5 f
2135(None)X
1 f
2360(,)X
2404(the)X
2534(previous)X
2859(screen)X
3105(contents)X
3421(are)X
3550(left)X
3690(in)X
3781(place)X
555 5772(if)N
631(the)X
761(window)X
1066(and)X
1215(its)X
1321(parent)X
1563(are)X
1692(the)X
1822(same)X
2025(depth.)X
2265(Otherwise,)X
2670(the)X
2800(initial)X
3030(contents)X
3346(of)X
3441(the)X
3571(exposed)X
3 f
2239 6144(19)N

20 p
%%Page: 20 24
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(regions)N
836(are)X
965(unde\256ned.)X
1378(Exposure)X
1732(events)X
1979(are)X
2108(then)X
2282(generated)X
2646(for)X
2770(the)X
2900(regions,)X
3203(even)X
3391(if)X
3467(the)X
555 768(background_pixmap)N
1301(is)X
5 f
1400(None)X
1 f
1625(.)X
1691(See)X
1840(Chapter)X
2141(8)X
2207(for)X
2331(a)X
2392(discussion)X
2781(of)X
2876(exposure)X
3215(event)X
3428(processing.)X
3 f
555 960(3.2.2.)N
797(The)X
966(border_pixmap)X
1575(and)X
1739(border_pixel)X
2246(Members)X
1 f
555 1084(The)N
714(border_pixmap)X
1274(member)X
1585(speci\256es)X
1910(the)X
2040(pixmap)X
2327(to)X
2418(be)X
2523(used)X
2706(for)X
2830(a)X
2891(window's)X
3259(border.)X
3554(This)X
3733(pixmap)X
555 1180(can)N
699(be)X
804(of)X
899(any)X
1048(size,)X
1229(although)X
1560(some)X
1768(sizes)X
1961(may)X
2135(be)X
2240(faster)X
2457(than)X
2631(others.)X
2912(The)X
3071(border_pixel)X
3543(member)X
555 1276(speci\256es)N
880(a)X
941(pixmap)X
1228(of)X
1323(unde\256ned)X
1692(size)X
1851(be)X
1956(used)X
2139(for)X
2263(a)X
2324(window's)X
2692(border.)X
2965(The)X
3124(border)X
3375(tile)X
3511(origin)X
3744(is)X
555 1372(always)N
821(the)X
951(same)X
1154(as)X
1249(the)X
1379(background)X
1816(tile)X
1952(origin.)X
555 1496(You)N
728(can)X
872(also)X
1036(set)X
1156(border_pixmap)X
1716(to)X
5 f
1825(CopyFromParent)X
1 f
2510(.)X
2576(In)X
2671(this)X
2821(case,)X
3016(the)X
3146(pixmap)X
3433(used)X
3616(for)X
3740(the)X
555 1592(border)N
806(will)X
966(be)X
1071(a)X
1132(copy)X
1325(of)X
1420(the)X
1550(parent)X
1792(window's)X
2160(border)X
2411(pixmap.)X
2742(The)X
2901(default)X
3168(value)X
3381(is)X
3462(the)X
3592(constant)X
5 f
570 1688(CopyFromParent)N
1 f
1255(.)X
1321(You)X
1494(can)X
1638(set)X
1758(the)X
1888(border_pixel)X
2360(to)X
2451(any)X
2600(pixel)X
2799(value.)X
3056(The)X
3215(default)X
3482(value)X
3695(is)X
555 1784(unde\256ned.)N
968(If)X
1048(you)X
1202(specify)X
1478(a)X
1539(border_pixel,)X
2033(it)X
2105(overrides)X
2454(either)X
2677(the)X
2807(default)X
3074(border_pixmap)X
3634(or)X
3729(any)X
555 1880(value)N
768(you)X
922(may)X
1096(have)X
1284(set)X
1404(in)X
1495(the)X
1625(border_pixmap)X
2185(member.)X
2540(All)X
2675(pixels)X
2908(in)X
2999(the)X
3129(window's)X
3497(border)X
3748(will)X
3908(be)X
555 1976(set)N
675(to)X
766(the)X
896(border_pixel)X
1368(value.)X
1625(Setting)X
1898(a)X
1959(new)X
2127(border,)X
2400(whether)X
2705(by)X
2815(setting)X
3073(border_pixel)X
3545(or)X
3640(by)X
3750(setting)X
555 2072(border_pixmap)N
1115(overrides)X
1464(any)X
1613(previous)X
1938(border.)X
555 2196(If)N
635(you)X
789(set)X
909(a)X
970(border_pixmap)X
1530(value,)X
1765(it)X
1837(overrides)X
2186(the)X
2316(default)X
2583(border_pixmap.)X
3187(The)X
3346(border_pixmap)X
555 2292(and)N
704(the)X
834(window)X
1139(must)X
1333(have)X
1521(the)X
1651(same)X
1854(depth.)X
2116(Otherwise,)X
2521(a)X
5 f
2600(BadMatch)X
1 f
3034(error)X
3226(is)X
3307(returned.)X
3666(If)X
3746(you)X
3900(set)X
555 2388(the)N
685(border_pixmap)X
1245(to)X
1336(the)X
1466(constant)X
5 f
1800(CopyFromParent)X
1 f
2485(,)X
2529(the)X
2659(parent)X
2901(window's)X
3269(border_pixmap)X
3829(is)X
555 2484(copied.)N
856(Subsequent)X
1284(changes)X
1589(to)X
1680(the)X
1810(parent)X
2052(window)X
2357(do)X
2467(not)X
2602(affect)X
2824(the)X
2954(child)X
3153(window.)X
3502(However,)X
3867(the)X
555 2580(child)N
754(window)X
1059(must)X
1253(have)X
1441(the)X
1571(same)X
1774(depth)X
1992(as)X
2087(the)X
2217(parent)X
2459(window.)X
2808(Otherwise,)X
3213(a)X
5 f
3292(BadMatch)X
1 f
3726(error)X
3918(is)X
555 2676(returned.)N
555 2800(The)N
714(border_pixmap)X
1274(can)X
1418(be)X
1523(freed)X
1725 0.3187(immediately)AX
2190(if)X
2266(no)X
2376(further)X
2637(explicit)X
2925(reference)X
3273(is)X
3354(made)X
3567(to)X
3658(it.)X
3774(If)X
3854(you)X
555 2896(later)N
734(draw)X
931(into)X
1091(the)X
1221(pixmap)X
1508(used)X
1691(for)X
1815(the)X
1945(border,)X
2218(X)X
2303(does)X
2486(not)X
2621(predict)X
2888(what)X
3081(happens)X
3391(because)X
3691(the)X
3821(X)X
555 2992 0.2885(implementation)AN
1133(is)X
1214(free)X
1372(to)X
1463(either)X
1686(make)X
1899(a)X
1960(copy)X
2153(of)X
2248(the)X
2378(pixmap)X
2665(or)X
2760(use)X
2899(the)X
3029(same)X
3232(pixmap)X
3519(each)X
3702(time)X
3882(the)X
555 3088(window)N
860(border)X
1111(is)X
1192(repainted.)X
555 3212(Output)N
822(to)X
913(a)X
974(window)X
1279(is)X
1360(always)X
1626(clipped)X
1908(to)X
1999(the)X
2129(inside)X
2362(of)X
2457(the)X
2587(window.)X
2914(Therefore,)X
3304(graphics)X
3624(operations)X
555 3308(never)N
772(affect)X
994(the)X
1124(window)X
1429(border.)X
1724(Borders)X
2024(are)X
2153(added)X
2385(to)X
2476(the)X
2606(window)X
2911(size)X
3070(speci\256ed.)X
3 f
555 3500(3.2.3.)N
797(The)X
966(bit_gravity)X
1404(and)X
1568(win_gravity)X
2040(Members)X
1 f
555 3624(Bit)N
686(gravity)X
958(de\256nes)X
1229(which)X
1466(region)X
1713(of)X
1808(the)X
1938(window)X
2243(should)X
2500(be)X
2605(retained)X
2911(when)X
3123(the)X
3253(window)X
3558(is)X
3639(resized.)X
555 3720(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(bit_gravity)X
1858(member)X
2169(is)X
2250(the)X
2380(constant)X
5 f
2714(ForgetGravity)X
1 f
3263(.)X
3329(Window)X
3654(gravity)X
555 3816(allows)N
807(you)X
961(to)X
1074(de\256ne)X
1311(how)X
1484(the)X
1614(window)X
1919(should)X
2176(be)X
2281(repositioned)X
2739(if)X
2815(its)X
2921(parent)X
3163(is)X
3244(resized.)X
3537(The)X
3696(default)X
555 3912(value)N
768(for)X
892(the)X
1022(win_gravity)X
1470(member)X
1781(is)X
1862(the)X
1992(constant)X
5 f
2326(NorthWestGravity)X
1 f
3036(.)X
3102(See)X
5 f
3269(XGetWindowAttri-)X
555 4008(butes)N
1 f
808(in)X
899(Section)X
1186(4.1)X
1318(for)X
1442(a)X
1503(description)X
1917(of)X
2012(the)X
2142(possible)X
2453(values)X
2700(you)X
2854(can)X
2998(set)X
3118(these)X
3321(to.)X
3 f
555 4200(3.2.4.)N
797(The)X
966(backing_store)X
1516(Member)X
1 f
555 4324(Some)N
778 0.2500(implementations)AX
1390(of)X
1485(the)X
1615(X)X
1700(server)X
1936(may)X
2110(choose)X
2376(to)X
2467(maintain)X
2799(the)X
2929(contents)X
3245(of)X
3340(windows.)X
3723(If)X
3803(the)X
3933(X)X
555 4420(server)N
791(maintains)X
1157(the)X
1287(contents)X
1603(of)X
1698(a)X
1759(window,)X
2086(the)X
2216(off-screen)X
2593(saved)X
2815(pixels)X
3048(are)X
3177(known)X
3438(as)X
3533(backing)X
555 4516(store.)N
792(The)X
951(backing_store)X
1467(member)X
1778(advises)X
2059(the)X
2189(X)X
2274(server)X
2510(on)X
2620(what)X
2813(to)X
2904(do)X
3014(with)X
3193(the)X
3323(contents)X
3639(of)X
3734(a)X
3795(win-)X
555 4612(dow.)N
772(You)X
945(can)X
1089(set)X
1209(this)X
1359(member)X
1670(to)X
5 f
1779(NotUseful)X
1 f
2180(,)X
5 f
2242(WhenMapped)X
1 f
2805(,)X
2849(or)X
5 f
2962(Always)X
1 f
3255(.)X
3321(The)X
3480(default)X
3747(value)X
555 4708(is)N
5 f
654(NotUseful)X
1 f
1055(.)X
555 4832(A)N
640(backing_store)X
1156(of)X
5 f
1269(WhenMapped)X
1 f
1854(advises)X
2135(the)X
2265(X)X
2350(server)X
2586(that)X
2741(maintaining)X
3186(contents)X
3502(of)X
3597(obscured)X
555 4928(regions)N
836(when)X
1048(the)X
1178(window)X
1483(is)X
1564(mapped)X
1865(would)X
2107(be)X
2212(bene\256cial.)X
2621(A)X
2706(backing_store)X
3222(of)X
5 f
3335(Always)X
1 f
3650(advises)X
555 5024(the)N
685(X)X
770(server)X
1006(that)X
1161(maintaining)X
1606(contents)X
1922(even)X
2110(when)X
2322(the)X
2452(window)X
2757(is)X
2838(unmapped)X
3227(would)X
3469(be)X
3574(bene\256cial.)X
555 5120(Even)N
758(if)X
834(the)X
964(window)X
1269(is)X
1350(larger)X
1577(than)X
1751(its)X
1857(parent,)X
2121(this)X
2271(is)X
2352(a)X
2413(request)X
2689(to)X
2780(the)X
2910(X)X
2995(server)X
3231(to)X
3344(maintain)X
3676(complete)X
555 5216(contents,)N
893(not)X
1028(just)X
1178(the)X
1308(region)X
1555(within)X
1803(the)X
1933(parent)X
2175(window)X
2480(boundaries.)X
2910(If)X
2990(the)X
3120(X)X
3205(server)X
3441(maintains)X
3807(con-)X
555 5312(tents,)N
766(exposure)X
1105(events)X
1352(will)X
1512(not)X
1647(be)X
1752(generated,)X
2138(but)X
2273(the)X
2403(X)X
2488(server)X
2724(may)X
2898(stop)X
3067(maintaining)X
3512(contents)X
3828(at)X
555 5408(any)N
704(time.)X
906(A)X
991(backing_store)X
1507(of)X
5 f
1620(NotUseful)X
1 f
2043(advises)X
2324(the)X
2454(X)X
2539(server)X
2775(that)X
2930(maintaining)X
3375(contents)X
3691(is)X
555 5504(unnecessary,)N
1028(although)X
1359(some)X
1567(X)X
1652 0.2500(implementations)AX
2264(may)X
2438(still)X
2594(choose)X
2860(to)X
2951(maintain)X
3283(contents)X
3599(and,)X
3770(there-)X
555 5600(fore,)N
740(not)X
875(generate)X
1195(exposure)X
1534(events.)X
3 f
2239 6144(20)N

21 p
%%Page: 21 25
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(3.2.5.)N
797(The)X
966(backing_planes)X
1571(and)X
1735(backing_pixel)X
2282(Members)X
1 f
555 796(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(backing_planes)X
2018(member)X
2329(is)X
2410(all)X
2521(ones.)X
2748(It)X
2824(indicates)X
3160(\(with)X
3368(one)X
3517(bits\))X
3696(which)X
555 892(bit)N
671(planes)X
918(of)X
1013(the)X
1143(window)X
1448(hold)X
1627(dynamic)X
1953(data)X
2122(that)X
2277(must)X
2471(be)X
2576(preserved)X
2939(in)X
3030(backing_stores)X
3580(and)X
3729(during)X
555 988(save_unders.)N
1055(The)X
1214(default)X
1481(value)X
1694(for)X
1818(the)X
1948(backing_pixel)X
2470(member)X
2781(is)X
2862(zero)X
3035(\(0\).)X
3203(It)X
3279(speci\256es)X
3604(what)X
555 1084(values)N
802(to)X
893(use)X
1032(in)X
1123(planes)X
1370(not)X
1505(covered)X
1805(by)X
1915(backing_planes.)X
2507(The)X
2666(X)X
2751(server)X
2987(is)X
3068(free)X
3226(to)X
3317(save)X
3495(only)X
3674(the)X
555 1180(speci\256ed)N
890(bit)X
1006(planes)X
1253(in)X
1344(the)X
1474(backing_store)X
1990(or)X
2085(the)X
2215(save_under)X
2637(and)X
2786(is)X
2867(free)X
3025(to)X
3116(regenerate)X
3504(the)X
3634(remaining)X
555 1276(planes)N
802(with)X
981(the)X
1111(speci\256ed)X
1446(pixel)X
1645(value.)X
1902(Any)X
2075(extraneous)X
2478(bits)X
2628(\(that)X
2812(is,)X
2915(those)X
3123(beyond)X
3404(the)X
3534(speci\256ed)X
555 1372(depth)N
773(of)X
868(the)X
998(window\))X
1332(in)X
1423(these)X
1626(values)X
1873(may)X
2047(be)X
2152(simply)X
2415(ignored.)X
3 f
555 1564(3.2.6.)N
797(The)X
966(save_under)X
1418(Member)X
1 f
555 1688(Some)N
778(server)X
1014 0.2500(implementations)AX
1626(may)X
1800(preserve)X
2119(bits)X
2269(of)X
2364(windows)X
2703(under)X
2925(other)X
3128(windows.)X
3511(This)X
3690(is)X
3771(not)X
555 1784(the)N
685(same)X
888(as)X
983(preserving)X
1376(the)X
1506(contents)X
1822(of)X
1917(a)X
1978(window)X
2283(for)X
2407(you.)X
2605(You)X
2778(may)X
2952(get)X
3082(better)X
3305(visual)X
3538(appeal)X
3790(if)X
555 1880(transient)N
881(windows)X
1220(\(for)X
1373(example,)X
1716(pop-up)X
1987(menus\))X
2268(request)X
2544(that)X
2699(the)X
2829(system)X
3096(preserve)X
3415(the)X
3545(bits)X
3695(under)X
555 1976(them,)N
776(so)X
876(the)X
1006(temporarily)X
1440(obscured)X
1779(applications)X
2228(do)X
2338(not)X
2473(have)X
2661(to)X
2752(repaint.)X
555 2100(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(save_under)X
1870(member)X
2181(is)X
5 f
2280(False)X
1 f
2510(.)X
2576(If)X
2656(save_under)X
3078(is)X
5 f
3177(True)X
1 f
3373(,)X
3417(the)X
3547(X)X
3632(server)X
3868(is)X
555 2196(advised)N
846(that,)X
1023(when)X
1235(this)X
1385(window)X
1690(is)X
1771(mapped,)X
2094(saving)X
2346(the)X
2476(contents)X
2792(of)X
2887(windows)X
3226(it)X
3298(obscures)X
3627(would)X
3869(be)X
555 2292(bene\256cial.)N
3 f
555 2484(3.2.7.)N
797(The)X
966(event_mask)X
1432(and)X
1596(do_not_propagate_mask)X
2546(Members)X
1 f
555 2608(The)N
714(event_mask)X
1157(de\256nes)X
1428(which)X
1665(events)X
1912(the)X
2042(client)X
2261(is)X
2342(interested)X
2707(in)X
2798(for)X
2922(this)X
3072(window)X
3377(\(or,)X
3523(for)X
3647(some)X
555 2704(event)N
768(types,)X
998(inferiors)X
1318(of)X
1413(the)X
1543(window\).)X
1899(The)X
2058(do_not_propagate_mask)X
2946(de\256nes)X
3217(which)X
3454(events)X
3701(should)X
555 2800(not)N
690(be)X
795(propagated)X
1208(to)X
1299(ancestor)X
1614(windows)X
1953(when)X
2165(no)X
2275(client)X
2494(has)X
2633(the)X
2763(event)X
2976(type)X
3150(selected)X
3456(in)X
3547(this)X
3697(window.)X
555 2896(These)N
787(masks)X
1029(are)X
1158(the)X
1288(bitwise)X
1565(inclusive)X
1906(OR)X
2050(of)X
2145(one)X
2294(or)X
2389(more)X
2592(of)X
2687(the)X
2817(valid)X
3016(event)X
3229(mask)X
3437(bits.)X
3631(You)X
3804(can)X
555 2992(specify)N
831(that)X
986(no)X
1096(maskable)X
1451(events)X
1698(are)X
1827(reported)X
2142(by)X
2252(passing)X
2538(the)X
2668(constant)X
5 f
3002(NoEventMask)X
1 f
3565(.)X
3631(The)X
555 3088(default)N
822(value)X
1035(for)X
1159(these)X
1362(members)X
1707(is)X
1788(the)X
1918(empty)X
2161(set.)X
2325(See)X
2474(Section)X
2761(8.3)X
2893(for)X
3017(information)X
3456(on)X
3566(the)X
3696(event)X
555 3184(mask)N
763(and)X
912(events.)X
3 f
555 3376(3.2.8.)N
797(The)X
966(override_redirect)X
1648(Member)X
1 f
555 3500(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(override_redirect)X
2076(member)X
2387(is)X
5 f
2486(False)X
1 f
2716(.)X
2782(Override_redirect)X
3429(speci\256es)X
555 3596(whether)N
860(map)X
1034(and)X
1183(con\256gure)X
1537(requests)X
1847(on)X
1957(this)X
2107(window)X
2412(should)X
2669(override)X
2984(a)X
5 f
3063(Substruc-)X
555 3692(tureRedirectMask)N
1 f
1281(on)X
1391(the)X
1521(parent.)X
1785(Window)X
2110(manages)X
2440(use)X
2579(this)X
2729(information)X
3168(to)X
3259(avoid)X
3477(tampering)X
555 3788(with)N
734(pop-up)X
1005(windows.)X
3 f
555 3980(3.2.9.)N
797(The)X
966(colormap)X
1345(Member)X
1 f
555 4104(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(colormap)X
1803(member)X
2114(is)X
5 f
2213(CopyFromParent)X
1 f
2898(.)X
2964(The)X
3123(colormap)X
3478(member)X
555 4200(speci\256es)N
880(which)X
1117(color)X
1320(map,)X
1516(if)X
1592(any,)X
1763(best)X
1927(re\257ects)X
2203(the)X
2333(true)X
2492(colors)X
2729(of)X
2824(the)X
2954(window.)X
3281(The)X
3440(colormap)X
3795(must)X
555 4296(have)N
743(the)X
873(same)X
1076(visual)X
1309(type)X
1483(as)X
1578(the)X
1708(window.)X
2057(Otherwise,)X
2462(a)X
5 f
2541(BadMatch)X
1 f
2975(error)X
3167(is)X
3248(returned.)X
3585(X)X
3670(servers)X
555 4392(capable)N
846(of)X
941(supporting)X
1340(multiple)X
1658(hardware)X
2006(color)X
2209(maps)X
2417(may)X
2591(use)X
2730(this)X
2880(information,)X
3341(and)X
3490(window)X
555 4488(managers)N
914(may)X
1088(use)X
1227(it)X
1299(for)X
5 f
1441(InstallColormap)X
1 f
2088(requests.)X
2442(If)X
2522(you)X
2676(set)X
2796(the)X
2926(colormap)X
3281(member)X
3592(to)X
5 f
3701(Copy-)X
555 4584(FromParent)N
1 f
1035(,)X
1079(the)X
1209(parent)X
1451(window's)X
1819(color)X
2022(map)X
2196(is)X
2277(copied)X
2534(and)X
2683(used)X
2866(by)X
2976(its)X
3082(child.)X
3325(Subsequent)X
555 4680(changes)N
860(to)X
951(the)X
1081(parent)X
1323(window)X
1628(do)X
1738(not)X
1873(affect)X
2095(the)X
2225(child)X
2424(window.)X
2773(However,)X
3138(the)X
3268(child)X
3467(window)X
3772(must)X
555 4776(have)N
743(the)X
873(same)X
1076(visual)X
1309(type)X
1483(as)X
1578(the)X
1708(parent.)X
1972(Otherwise,)X
2377(a)X
5 f
2456(BadMatch)X
1 f
2890(error)X
3082(is)X
3163(returned.)X
3500(The)X
3659(parent)X
555 4872(window)N
860(must)X
1054(not)X
1189(have)X
1377(a)X
1438(color)X
1641(map)X
1815(of)X
5 f
1928(None)X
1 f
2153(.)X
2219(Otherwise,)X
2624(a)X
5 f
2703(BadMatch)X
1 f
3137(error)X
3329(is)X
3410(returned.)X
3 f
555 5064(3.2.10.)N
841(The)X
1010(cursor)X
1276(Member)X
1 f
555 5188(The)N
714(default)X
981(value)X
1194(for)X
1318(the)X
1448(cursor)X
1689(member)X
2000(is)X
2081(the)X
2211(constant)X
5 f
2545(None)X
1 f
2770(.)X
2836(If)X
2916(a)X
2977(cursor)X
3218(is)X
3299(speci\256ed,)X
3656(it)X
3728(will)X
3888(be)X
555 5284(used)N
738(whenever)X
1101(the)X
1231(pointer)X
1503(is)X
1584(in)X
1675(the)X
1805(window.)X
2132(If)X
5 f
2230(None)X
1 f
2477(is)X
2558(speci\256ed,)X
2915(the)X
3045(parent's)X
3350(cursor)X
3591(will)X
3751(be)X
555 5380(used)N
738(when)X
950(the)X
1080(pointer)X
1352(is)X
1433(in)X
1524(the)X
1654(window,)X
1981(and)X
2130(any)X
2279(change)X
2550(in)X
2641(the)X
2771(parent's)X
3076(cursor)X
3317(will)X
3477(cause)X
3694(an)X
555 5476(immediate)N
951(change)X
1222(in)X
1313(the)X
1443(displayed)X
1803(cursor.)X
2088(The)X
2247(cursor)X
2488(may)X
2662(be)X
2767(freed)X
2969 0.3187(immediately)AX
3434(if)X
3510(no)X
3620(further)X
555 5572(explicit)N
843(reference)X
1191(to)X
1282(it)X
1354(is)X
1435(made)X
1648(by)X
1758(calling)X
5 f
2039(XFreeCursor)X
1 f
2557(.)X
2623(See)X
2772(Section)X
3059(6.8.2)X
3257(for)X
3381(further)X
3642(informa-)X
555 5668(tion.)N
3 f
2239 6144(21)N

22 p
%%Page: 22 26
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(3.2.11.)N
841(Default)X
1141(Values)X
1417(for)X
1551(XSetWindowAttributes)X
2456(Members)X
1 f
555 796(The)N
714(following)X
1079(table)X
1273(lists)X
1438(the)X
1568(default)X
1835(values)X
2082(for)X
2206(each)X
2389(member)X
2700(in)X
2791(the)X
5 f
2939(XSetWindowAttributes)X
1 f
555 892(structure.)N
10 f
555 948(i)N
585(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1092(Member)N
1553(Default)X
1839(Value)X
10 f
555 1148(i)N
585(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1292(background_pixmap)N
1553(None)X
555 1388(background_pixel)N
1553(Unde\256ned)X
555 1484(border_pixmap)N
1553(CopyFromParent)X
555 1580(border_pixel)N
1553(Unde\256ned)X
555 1676(bit_gravity)N
1553(ForgetGravity)X
555 1772(win_gravity)N
1553(NorthWestGravity)X
555 1868(backing_store)N
1553(NotUseful)X
555 1964(backing_planes)N
1553(All)X
1688(ones)X
555 2060(backing_pixel)N
1553(0)X
1619(\(zero\))X
555 2156(save_under)N
1553(False)X
555 2252(event_mask)N
1553(empty)X
1796(set)X
555 2348(do_not_propagate_mask)N
1553(empty)X
1796(set)X
555 2444(override_redirect)N
1553(False)X
555 2540(colormap)N
1553(CopyFromParent)X
555 2636(cursor)N
1553(None)X
10 f
555 2644(i)N
585(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
3 f
555 2884(3.3.)N
731(Creating)X
1085(Windows)X
1 f
555 3008(Xlib)N
734(provides)X
1059(basic)X
1262(ways)X
1464(of)X
1559(creating)X
1865(windows.)X
2248(See)X
2397(the)X
2527(X)X
2612(toolkit)X
2866(documentation)X
3413(for)X
3537(more)X
3740(infor-)X
555 3104(mation.)N
867(If)X
947(you)X
1101(create)X
1333(your)X
1516(own)X
1689(top)X
1824(level)X
2018(windows)X
2357(\(direct)X
2609(children)X
2920(of)X
3015(the)X
3145(root)X
3309(window\))X
3643(the)X
3773(rules)X
555 3200(enumerated)N
988(below)X
1225(must)X
1419(be)X
1524(observed)X
1863(for)X
1987(applications)X
2436(to)X
2527(interact)X
2814(reasonably)X
3217(across)X
3458(differing)X
3788(styles)X
555 3296(of)N
650(window)X
955(management.)X
555 3420(You)N
728(should)X
985(never)X
1202(\256ght)X
1386(with)X
1565(a)X
1626(window)X
1931(manager)X
2256(for)X
2380(size)X
2539(or)X
2634(placement)X
3019(of)X
3114(your)X
3297(top-level)X
555 3516(window\(s\).)N
996(Toolkits)X
1313(often)X
1516(supply)X
1773(routines)X
2079(speci\256cally)X
2503(for)X
2627(creating)X
2933(and)X
3082(placing)X
3364(top)X
3499(level)X
3693(win-)X
555 3612(dows.)N
806(If)X
886(you)X
1040(do)X
1150(not)X
1285(use)X
1424(a)X
1485(toolkit,)X
1761(you)X
1915(should)X
2172(provide)X
2463(some)X
2671(standard)X
2991(information)X
3430(or)X
3525(``hints'')X
3835(to)X
555 3708(the)N
685(window)X
990(manager)X
1315(by)X
1425(using)X
1638(the)X
1768(utility)X
2003(functions)X
2353(described)X
2712(in)X
2803(Chapter)X
3104(10.)X
555 3832(The)N
714(policy)X
957(guidelines)X
1342(for)X
1466(window)X
1771(creation)X
2077(are:)X
555 3956(1.)N
775(An)X
904(application,)X
1341(by)X
1451(listening)X
1778(to)X
1869(the)X
1999(\256rst)X
2158(exposure)X
2497(event,)X
2732(must)X
2926(be)X
3031(able)X
3200(to)X
3291(deal)X
3460(with)X
3639(whatever)X
775 4052(size)N
934(window)X
1239(it)X
1311(gets,)X
1497(even)X
1685(if)X
1761(this)X
1911(means)X
2158(that)X
2313(the)X
2443(application)X
2858(just)X
3008(prints)X
3231(a)X
3292(message,)X
3634(like)X
775 4148(``Please)N
1080(make)X
1293(me)X
1423(bigger,'')X
1750(in)X
1841(its)X
1947(window.)X
555 4272(2.)N
775(An)X
904(application)X
1319(should)X
1576(only)X
1755(attempt)X
2043(to)X
2134(resize)X
2361(or)X
2456(move)X
2674(its)X
2780(top-level)X
3116(window)X
3421(in)X
3512(direct)X
775 4368(response)N
1104(to)X
1195(a)X
1256(user)X
1424(request.)X
1744(An)X
1873(application)X
2288(is)X
2369(free)X
2527(to)X
2618(resize)X
2845(or)X
2940(move)X
3158(the)X
3288(children)X
3599(of)X
3694(its)X
3800(top-)X
775 4464(level)N
969(window)X
1274(as)X
1369(necessary.)X
1776(\(Toolkits)X
2122(often)X
2325(have)X
2513(facilities)X
2840(for)X
2964(automatic)X
3335(relayout.\))X
3697(If)X
3777(a)X
775 4560(request)N
1051(to)X
1142(change)X
1413(the)X
1543(size)X
1702(of)X
1797(its)X
1903(top-level)X
2239(window)X
2544(fails,)X
2740(the)X
2870(application)X
3285(must)X
3479(not)X
3614(\256ght)X
3798(with)X
775 4656(the)N
905(window)X
1210(manager.)X
555 4780(3.)N
775(If)X
855(an)X
960(application)X
1375(does)X
1558(not)X
1693(use)X
1832(a)X
1893(toolkit)X
2147(that)X
2302 0.2604(automatically)AX
2806(sets)X
2960(standard)X
3280(window)X
3585(properties,)X
775 4876(that)N
930(application)X
1345(should)X
1602(set)X
1722(these)X
1925(properties)X
2299(for)X
2423(the)X
2553(top-level)X
2889(window)X
3194(before)X
3440(mapping)X
3771(it.)X
3887(To)X
775 4972(set)N
895(standard)X
1215(window)X
1520(properties)X
1894(for)X
2018(a)X
2079(top-level)X
2415(window,)X
2742(use)X
5 f
2899(XSetStandardProperties)X
1 f
3865(.)X
775 5068(See)N
924(Chapter)X
1225(9)X
1291(for)X
1415(further)X
1676(information.)X
555 5192(The)N
714(low-level)X
1069(functions)X
1419(provided)X
1754(by)X
1864(Xlib)X
2043(to)X
2134(create)X
2366(an)X
2471(unmapped)X
2860(subwindow)X
3287(for)X
3411(a)X
3472(speci\256ed)X
555 5288(parent)N
797(window)X
1102(are)X
5 f
1249(XCreateWindow)X
1 f
1921(and)X
5 f
2088(XCreateSimpleWindow)X
1 f
3006(.)X
5 f
3093(XCreateWindow)X
1 f
3765(is)X
3846(a)X
555 5384(more)N
758(general)X
1039(function)X
1355(that)X
1510(allows)X
1762(you)X
1916(to)X
2007(set)X
2127(speci\256c)X
2418(window)X
2723(attributes)X
3074(when)X
3286(you)X
3440(create)X
3672(it.)X
5 f
570 5480(XCreateSimpleWindow)N
1 f
1510(creates)X
1776(a)X
1837(window)X
2142(that)X
2297(inherits)X
2584(its)X
2690(attributes)X
3041(from)X
3234(its)X
3340(parent)X
3582(window.)X
555 5576(That)N
739(is,)X
842(you)X
996(do)X
1106(not)X
1241(set)X
1361(speci\256c)X
1652(attributes)X
2003(when)X
2215(you)X
2369(create)X
2601(a)X
2684(simple)X
2942(window.)X
555 5700(The)N
714(X)X
799(server)X
1035(acts)X
1194(as)X
1289(if)X
5 f
1383(InputOnly)X
1 f
1798(windows)X
2137(do)X
2247(not)X
2382(exist)X
2571(for)X
2695(the)X
2825(purposes)X
3159(of)X
3254(graphics)X
3574(requests,)X
555 5796(exposure)N
894(processing,)X
1314(and)X
5 f
1481(VisibilityNotify)X
1 f
2059(events.)X
2350(An)X
5 f
2497(InputOnly)X
1 f
2912(window)X
3217(cannot)X
3474(be)X
3579(used)X
3762(as)X
3857(a)X
3 f
2239 6144(22)N

23 p
%%Page: 23 27
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(drawable)N
899(\(that)X
1083(is,)X
1186(as)X
1281(a)X
1342(source)X
1593(or)X
1688(destination)X
2098(for)X
2222(graphics)X
2542(requests\).)X
5 f
2946(InputOnly)X
1 f
3361(and)X
5 f
3528 0.3187(InputOutput)AX
1 f
555 768(windows)N
894(act)X
1019(identically)X
1415(in)X
1506(other)X
1709(respects)X
2014(\(properties,)X
2439(grabs,)X
2673(input)X
2877(control,)X
3171(and)X
3320(so)X
3420(on\).)X
3603(Extension)X
555 864(packages)N
899(may)X
1073(de\256ne)X
1310(other)X
1513(classes)X
1779(of)X
1874(windows.)X
555 988(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XCreateWindow)X
1 f
1889(is:)X
555 1112(Window)N
880(XCreateWindow)X
1483(\()X
2 f
1519(display)X
1 f
1774(,)X
2 f
1818(parent)X
1 f
2055(,)X
2 f
2099(x)X
1 f
2145(,)X
2 f
2189(y)X
1 f
2235(,)X
2 f
2279(width)X
1 f
2483(,)X
2 f
2527(height)X
1 f
2755(,)X
2 f
2799(border_width)X
1 f
3286(,)X
2 f
3330(depth)X
1 f
3533(,)X
2 f
1061 1208(class)N
1 f
1244(,)X
2 f
1288(visual)X
1 f
1506(,)X
2 f
1550(valuemask)X
1 f
1928(,)X
2 f
1972(attributes)X
1 f
2318(\))X
687 1304(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1400(Window)N
2 f
1012(parent)X
1 f
1249(;)X
687 1496(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 1592(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 1688(unsigned)N
1027(int)X
2 f
1143(border_width)X
1 f
1630(;)X
687 1784(int)N
2 f
803(depth)X
1 f
1006(;)X
687 1880(unsigned)N
1027(int)X
2 f
1143(class)X
1 f
1326(;)X
687 1976(Visual)N
939(*)X
2 f
(visual)S
1 f
687 2072(unsigned)N
1027(long)X
2 f
1206(valuemask)X
1 f
1584(;)X
687 2168(XSetWindowAttributes)N
1541(*)X
2 f
(attributes)S
1 f
1931(;)X
2 f
555 2320(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2444(parent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(parent)X
1843(window)X
2148(ID.)X
2 f
555 2568(x)N
555 2664(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(are)X
3094(the)X
3224(top)X
3359(left)X
3499(outside)X
3776(corner)X
1131 2760(of)N
1226(the)X
1356(created)X
1632(window's)X
2000(borders)X
2285(and)X
2434(are)X
2563(relative)X
2850(to)X
2941(the)X
3071(inside)X
3304(of)X
3399(the)X
3529(parent)X
1131 2856(window's)N
1499(borders.)X
2 f
555 2980(width)N
555 3076(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(created)X
2978(window's)X
3346(inside)X
3579(dimensions.)X
1131 3172(These)N
1363(dimensions)X
1787(do)X
1897(not)X
2032(include)X
2314(the)X
2444(created)X
2720(window's)X
3088(borders,)X
3395(which)X
3632(are)X
1131 3268(entirely)N
1423(outside)X
1700(of)X
1795(the)X
1925(window.)X
2274(The)X
2433(dimensions)X
2857(must)X
3051(be)X
3156(nonzero.)X
3483(Otherwise,)X
3888(a)X
5 f
1146 3364(BadValue)N
1 f
1565(error)X
1757(is)X
1838(returned.)X
2 f
555 3488(border_width)N
1 f
1131(Speci\256es,)X
1493(in)X
1584(pixels,)X
1839(the)X
1969(width)X
2192(of)X
2287(the)X
2417(created)X
2693(window's)X
3061(border.)X
3356(The)X
3515(border_width)X
1131 3584(for)N
1255(an)X
5 f
1378(InputOnly)X
1 f
1793(window)X
2098(must)X
2292(be)X
2397(zero)X
2570(\(0\).)X
2738(Otherwise,)X
3143(a)X
5 f
3222(BadMatch)X
1 f
3656(error)X
3848(is)X
1131 3680(returned.)N
2 f
555 3804(depth)N
1 f
1131(A)X
1216(depth)X
1434(of)X
1529(zero)X
1702(for)X
1826(class)X
5 f
2037 0.3187(InputOutput)AX
1 f
2537(or)X
5 f
2650(CopyFromParent)X
1 f
3357(means)X
3604(the)X
3734(depth)X
3952(is)X
1131 3900(taken)N
1344(from)X
1537(the)X
1667(parent.)X
2 f
555 4024(class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(created)X
1877(window's)X
2245(class.)X
2482(You)X
2655(can)X
2799(pass)X
2972(one)X
3121(of)X
3216(these)X
3419(constants:)X
5 f
1146 4120 0.3187(InputOutput)AN
1 f
1624(,)X
5 f
1686(InputOnly)X
1 f
2079(,)X
2123(or)X
5 f
2236(CopyFromParent)X
1 f
2921(.)X
2987(A)X
3072(class)X
3265(of)X
5 f
3378(CopyFrom-)X
1131 4216(Parent)N
1 f
1428(means)X
1675(the)X
1805(class)X
1998(is)X
2079(taken)X
2292(from)X
2485(the)X
2615(parent.)X
2 f
555 4340(visual)N
1 f
1131(Speci\256es)X
1471(the)X
1601(visual)X
1834(type.)X
2052(A)X
2137(visual)X
2370(of)X
5 f
2483(CopyFromParent)X
1 f
3190(means)X
3437(the)X
3567(visual)X
3800(type)X
1131 4436(is)N
1212(taken)X
1425(from)X
1618(the)X
1748(parent.)X
2 f
555 4560(valuemask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(window)X
2013(attributes)X
2364(are)X
2493(de\256ned)X
2774(in)X
2865(the)X
2995(attributes)X
3346(argument.)X
3745(This)X
1131 4656(mask)N
1339(is)X
1420(the)X
1550(bitwise)X
1827(inclusive)X
2168(OR)X
2312(of)X
2407(the)X
2537(valid)X
2736(attribute)X
3053(mask)X
3261(bits.)X
3455(If)X
3535(valuemask)X
3934(is)X
1131 4752(zero)N
1304(\(0\),)X
1450(the)X
1580(rest)X
1729(is)X
1810(ignored,)X
2123(and)X
2272(the)X
2402(attributes)X
2753(are)X
2882(not)X
3017(referenced.)X
2 f
555 4876(attributes)N
1 f
1131(Attributes)X
1506(of)X
1601(the)X
1731(window)X
2036(to)X
2127(be)X
2232(set)X
2352(at)X
2438(creation)X
2744(time)X
2924(should)X
3181(be)X
3286(set)X
3406(in)X
3497(this)X
3647(structure.)X
1131 4972(The)N
1290(valuemask)X
1689(should)X
1946(have)X
2134(the)X
2264(appropriate)X
2687(bits)X
2837(set)X
2957(to)X
3048(indicate)X
3350(which)X
3587(attributes)X
1131 5068(have)N
1319(been)X
1507(set)X
1627(in)X
1718(the)X
1848(structure.)X
2222(See)X
2371(Section)X
2658(3.2)X
2790(for)X
2914(further)X
3175(information.)X
555 5192(The)N
5 f
732(XCreateWindow)X
1 f
1404(function)X
1720(creates)X
1986(an)X
2091(unmapped)X
2480(subwindow)X
2907(for)X
3031(a)X
3092(speci\256ed)X
3427(parent)X
3669(window,)X
555 5288(returns)N
821(the)X
951(window)X
1256(ID)X
1370(of)X
1465(the)X
1595(created)X
1871(window,)X
2198(and)X
2347(causes)X
2598(the)X
2728(X)X
2813(server)X
3049(to)X
3140(generate)X
3460(a)X
5 f
3539(CreateNo-)X
555 5384(tify)N
1 f
705(event.)X
962(The)X
1121(created)X
1397(window)X
1702(is)X
1783(placed)X
2035(on)X
2145(top)X
2280(in)X
2371(the)X
2501(stacking)X
2817(order)X
3024(with)X
3203(respect)X
3474(to)X
3565(siblings.)X
555 5508(For)N
699(class)X
5 f
910 0.3187(InputOutput)AX
1 f
1388(,)X
1432(the)X
1562(visual)X
1795(type)X
1969(and)X
2118(depth)X
2336(must)X
2530(be)X
2635(a)X
2696(combination)X
3160(supported)X
3529(for)X
3653(the)X
555 5604(screen.)N
845(Otherwise,)X
1250(a)X
5 f
1329(BadMatch)X
1 f
1763(error)X
1955(is)X
2036(returned.)X
2395(The)X
2554(depth)X
2772(need)X
2960(not)X
3095(be)X
3200(the)X
3330(same)X
3533(as)X
3628(the)X
3758(parent,)X
555 5700(but)N
690(the)X
820(parent)X
1062(must)X
1256(not)X
1391(be)X
1496(a)X
1557(window)X
1862(of)X
1957(class)X
5 f
2168(InputOnly)X
1 f
2561(.)X
2627(Otherwise,)X
3032(a)X
5 f
3111(BadMatch)X
1 f
3545(error)X
3737(is)X
3818(gen-)X
555 5796(erated.)N
836(For)X
980(an)X
5 f
1103(InputOnly)X
1 f
1518(window)X
1823(the)X
1953(depth)X
2171(must)X
2365(be)X
2470(zero)X
2643(\(0\),)X
2789(and)X
2938(the)X
3068(visual)X
3301(must)X
3495(be)X
3600(one)X
3 f
2239 6144(23)N

24 p
%%Page: 24 28
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(supported)N
924(by)X
1034(the)X
1164(screen.)X
1454(If)X
1534(either)X
1757(of)X
1852(these)X
2055(conditions)X
2445(are)X
2574(not)X
2709(met,)X
2886(a)X
5 f
2965(BadMatch)X
1 f
3399(error)X
3591(is)X
3672(gen-)X
555 768(erated.)N
836(The)X
995(parent)X
1237(window,)X
1564(however,)X
1910(may)X
2084(have)X
2272(any)X
2421(depth)X
2639(and)X
2788(class.)X
3025(The)X
3184(only)X
3363(window)X
3668(attributes)X
555 864(de\256ned)N
836(for)X
5 f
978(InputOnly)X
1 f
1393(windows)X
1732(are)X
1861(win_gravity,)X
2331(event_mask,)X
2796(do_not_propagate_mask,)X
555 960(override_redirect,)N
1205(and)X
1354(cursor.)X
1639(If)X
1719(you)X
1873(specify)X
2149(any)X
2298(other)X
2501(window)X
2806(attribute)X
3123(for)X
3247(an)X
5 f
3370(InputOnly)X
1 f
3785(win-)X
555 1056(dow,)N
750(a)X
5 f
829(BadMatch)X
1 f
1263(error)X
1455(is)X
1536(returned.)X
555 1180(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCreateWindow)X
1 f
2508(are)X
5 f
2655(BadAlloc)X
1 f
5 f
3057(BadColor)X
1 f
3438(,)X
5 f
3500(BadCursor)X
1 f
3935(,)X
5 f
570 1276(BadMatch)N
1 f
982(,)X
5 f
1044(BadPixmap)X
1 f
1509(,)X
5 f
1571(BadValue)X
1 f
1968(,)X
2012(and)X
5 f
2179(BadWindow)X
1 f
2663(.)X
555 1496(Use)N
5 f
731(XCreateSimpleWindow)X
1 f
1671(to)X
1762(create)X
1994(an)X
2099(unmapped)X
5 f
2506 0.3187(InputOutput)AX
1 f
3006(subwindow)X
3433(of)X
3528(the)X
3658(speci\256ed)X
555 1592(parent)N
797(window.)X
1146(The)X
1305(de\256nition)X
1666(for)X
1790(this)X
1940(function)X
2256(is:)X
555 1716(Window)N
880(XCreateSimpleWindow)X
1734(\()X
2 f
1770(display)X
1 f
2025(,)X
2 f
2069(parent)X
1 f
2306(,)X
2 f
2350(x)X
1 f
2396(,)X
2 f
2440(y)X
1 f
2486(,)X
2 f
2530(width)X
1 f
2734(,)X
2 f
2778(height)X
1 f
3006(,)X
2 f
3050(border_width)X
1 f
3537(,)X
2 f
1193 1812(border)N
1 f
1439(,)X
2 f
1483(background)X
1 f
1910(\))X
687 1908(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2004(Window)N
2 f
1012(parent)X
1 f
1249(;)X
687 2100(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 2196(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(,)X
2 f
1663(border_width)X
1 f
2150(;)X
687 2292(unsigned)N
1027(long)X
2 f
1206(border)X
1 f
1452(;)X
687 2388(unsigned)N
1027(long)X
2 f
1206(background)X
1 f
1633(;)X
2 f
555 2540(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2664(parent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(parent)X
1843(window)X
2148(ID.)X
2 f
555 2788(x)N
555 2884(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(are)X
3094(the)X
3224(top)X
3359(left)X
3499(outside)X
3776(corner)X
1131 2980(of)N
1226(the)X
1356(new)X
1524(window's)X
1892(borders)X
2177(and)X
2326(are)X
2455(relative)X
2742(to)X
2833(the)X
2963(inside)X
3196(of)X
3291(the)X
3421(parent)X
3663(window's)X
1131 3076(borders.)N
2 f
555 3200(width)N
555 3296(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(created)X
2978(window's)X
3346(inside)X
3579(dimensions.)X
1131 3392(These)N
1363(dimensions)X
1787(do)X
1897(not)X
2032(include)X
2314(the)X
2444(created)X
2720(window's)X
3088(borders,)X
3395(which)X
3632(are)X
1131 3488(entirely)N
1423(outside)X
1700(of)X
1795(the)X
1925(window.)X
2274(The)X
2433(dimensions)X
2857(must)X
3051(be)X
3156(nonzero.)X
3505(Otherwise,)X
3910(a)X
5 f
1146 3584(BadValue)N
1 f
1565(error)X
1757(is)X
1838(returned.)X
2 f
555 3708(border_width)N
1 f
1131(Speci\256es,)X
1493(in)X
1584(pixels,)X
1839(the)X
1969(width)X
2192(of)X
2287(the)X
2417(created)X
2693(window's)X
3061(border.)X
3356(The)X
3515(border_width)X
1131 3804(for)N
1255(an)X
5 f
1378(InputOnly)X
1 f
1793(window)X
2098(must)X
2292(be)X
2397(zero)X
2570(\(0\).)X
2738(Otherwise,)X
3143(a)X
5 f
3222(BadMatch)X
1 f
3656(error)X
3848(is)X
1131 3900(returned.)N
2 f
555 4024(border)N
1 f
1131(Speci\256es)X
1471(the)X
1601(border)X
1852(pixel)X
2051(of)X
2146(the)X
2276(window.)X
2 f
555 4148(background)N
1 f
1131(Speci\256es)X
1471(the)X
1601(pixel)X
1800(value)X
2013(that)X
2168(you)X
2322(want)X
2515(to)X
2606(set)X
2726(for)X
2850(the)X
2980(speci\256ed)X
3315(window's)X
3683(back-)X
1131 4244(ground.)N
555 4464(The)N
5 f
732(XCreateSimpleWindow)X
1 f
1672(function)X
1988(creates)X
2254(an)X
2359(unmapped)X
5 f
2766 0.3187(InputOutput)AX
1 f
3266(subwindow)X
3693(for)X
3817(a)X
555 4560(speci\256ed)N
890(parent)X
1132(window,)X
1459(returns)X
1725(the)X
1855(window)X
2160(ID)X
2274(of)X
2369(the)X
2499(created)X
2775(window,)X
3102(and)X
3251(causes)X
3502(the)X
3632(X)X
3717(server)X
555 4656(to)N
646(generate)X
966(a)X
5 f
1045(CreateNotify)X
1 f
1571(event.)X
1828(The)X
1987(created)X
2263(window)X
2568(is)X
2649(placed)X
2901(on)X
3011(top)X
3146(in)X
3237(the)X
3367(stacking)X
3683(order)X
555 4752(with)N
734(respect)X
1005(to)X
1096(siblings.)X
1437(Any)X
1610(part)X
1769(of)X
1864(the)X
1994(window)X
2299(that)X
2454(extends)X
2745(outside)X
3022(its)X
3128(parent)X
3370(window)X
3675(will)X
3835(be)X
555 4848(clipped.)N
5 f
902(XCreateSimpleWindow)X
1 f
1842(inherits)X
2129(its)X
2235(depth,)X
2475(class,)X
2690(and)X
2839(visual)X
3072(from)X
3265(its)X
3371(parent.)X
3657(All)X
3792(other)X
555 4944(window)N
860(attributes)X
1211(have)X
1399(their)X
1583(default)X
1850(values.)X
555 5068(The)N
714(created)X
990(window)X
1295(is)X
1376(not)X
1511(yet)X
1641(displayed)X
2001(\(mapped\))X
2360(on)X
2470(the)X
2600(user's)X
2831(display.)X
3152(To)X
3272(display)X
3549(the)X
3679(window,)X
555 5164(call)N
5 f
723(XMapWindow)X
1 f
1280(.)X
1346(The)X
1505(new)X
1673(window)X
1978(will)X
2138 0.4141(initially)AX
2437(use)X
2576(the)X
2706(same)X
2909(cursor)X
3150(as)X
3245(its)X
3351(parent.)X
3615(A)X
3700(new)X
3868(cur-)X
555 5260(sor)N
684(may,)X
880(of)X
975(course,)X
1248(be)X
1353(de\256ned)X
1634(for)X
1758(the)X
1888(new)X
2056(window)X
2361(by)X
2471(calling)X
5 f
2752(XDe\256neCursor)X
1 f
3343(.)X
3409(The)X
3568(window)X
3873(will)X
555 5356(not)N
690(be)X
795(visible)X
1053(on)X
1163(the)X
1293(screen)X
1539(unless)X
1781(it)X
1853(and)X
2002(all)X
2113(of)X
2208(its)X
2314(ancestors)X
2663(are)X
2792(mapped,)X
3115(and)X
3264(it)X
3336(is)X
3417(not)X
3552(obscured)X
3891(by)X
555 5452(any)N
704(of)X
799(its)X
905(ancestors.)X
1298(The)X
1457(window)X
1762(is)X
1843(placed)X
2095(on)X
2205(top)X
2340(of)X
2435(the)X
2565(stacking)X
2881(order)X
3088(with)X
3267(respect)X
3538(to)X
3629(its)X
555 5548(siblings.)N
896(In)X
991(addition,)X
1325(the)X
1455(new)X
1623(window's)X
1991(cursor)X
2232(will)X
2392(be)X
5 f
2515(None)X
1 f
2740(.)X
555 5672(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCreateSimpleWindow)X
1 f
2776(are)X
5 f
2923(BadAlloc)X
1 f
3285(,)X
5 f
3347(BadValue)X
1 f
3744(,)X
3788(and)X
5 f
570 5768(BadWindow)N
1 f
1054(.)X
3 f
2239 6144(24)N

25 p
%%Page: 25 29
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(3.4.)N
731(Destroying)X
1163(Windows)X
1 f
555 796(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(destroy)X
2404(a)X
2465(window)X
2770(or)X
2887(destroy)X
3168(all)X
3279(subwindows)X
3740(of)X
3835(a)X
555 892(window.)N
555 1112(Use)N
5 f
731(XDestroyWindow)X
1 f
1442(to)X
1533(destroy)X
1814(a)X
1875(window)X
2180(and)X
2329(all)X
2440(of)X
2535(its)X
2641(subwindows.)X
3124(The)X
3283(de\256nition)X
3644(for)X
3768(this)X
555 1208(function)N
871(is:)X
555 1332(XDestroyWindow)N
1206(\()X
2 f
1242(display)X
1 f
1497(,)X
2 f
1541(w)X
1 f
1607(\))X
687 1428(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1524(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 1676(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1800(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 1924(The)N
5 f
732(XDestroyWindow)X
1 f
1443(function)X
1759(destroys)X
2074(the)X
2204(speci\256ed)X
2539(window)X
2844(as)X
2939(well)X
3113(as)X
3208(all)X
3319(of)X
3414(its)X
3520(subwindows)X
555 2020(and)N
704(causes)X
955(the)X
1085(X)X
1170(server)X
1406(to)X
1497(generate)X
1817(a)X
5 f
1896(DestroyNotify)X
1 f
2461(event)X
2674(for)X
2798(each)X
2981(window.)X
3330(The)X
3489(window)X
555 2116(should)N
812(never)X
1029(again)X
1242(be)X
1347(referenced.)X
1783(If)X
1863(the)X
1993(window)X
2298(speci\256ed)X
2633(by)X
2743(the)X
2873(w)X
2958(argument)X
3313(is)X
3394(mapped,)X
3717(it)X
3789(is)X
555 2212(unmapped)N
944 0.2404(automatically.)AX
1492(The)X
1651(window)X
1956(and)X
2105(all)X
2216(of)X
2311(its)X
2417(inferiors)X
2737(are)X
2866(then)X
3040(destroyed,)X
3426(and)X
3575(a)X
5 f
3654(Destroy-)X
555 2308(Notify)N
1 f
817(event)X
1030(is)X
1111(generated)X
1475(for)X
1599(each)X
1782(window.)X
2131(The)X
2290(ordering)X
2610(of)X
2705(the)X
5 f
2853(DestroyNotify)X
1 f
3418(events)X
3665(is)X
3746(such)X
555 2404(that)N
710(for)X
834(any)X
983(given)X
1201(window)X
1506(being)X
1724(destroyed,)X
5 f
2128(DestroyNotify)X
1 f
2693(is)X
2774(generated)X
3138(on)X
3248(any)X
3397(inferiors)X
3717(of)X
3812(the)X
555 2500(window)N
860(before)X
1106(being)X
1324(generated)X
1688(on)X
1798(the)X
1928(window)X
2233(itself.)X
2476(The)X
2635(ordering)X
2955(among)X
3217(siblings)X
3514(and)X
3663(across)X
555 2596(subhierarchies)N
1085(is)X
1166(not)X
1301(otherwise)X
1665(constrained.)X
2137(If)X
2217(the)X
2347(window)X
2652(you)X
2806(speci\256ed)X
3141(is)X
3222(a)X
3283(root)X
3447(window,)X
3774(no)X
555 2692(windows)N
894(are)X
1023(destroyed.)X
1431(Destroying)X
1844(a)X
1905(mapped)X
2228(window)X
2533(will)X
2693(generate)X
3013(exposure)X
3352(events)X
3599(on)X
3709(other)X
555 2788(windows)N
894(that)X
1049(were)X
1241(obscured)X
1580(by)X
1690(the)X
1820(window)X
2125(being)X
2343(destroyed.)X
555 2912(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XDestroyWindow)X
1 f
2513(is)X
5 f
2612(BadWindow)X
1 f
3096(.)X
555 3132(Use)N
5 f
731(XDestroySubwindows)X
1 f
1623(to)X
1714(destroy)X
1995(all)X
2106(subwindows)X
2567(of)X
2662(a)X
2723(speci\256ed)X
3058(window.)X
3385(The)X
3544(de\256nition)X
3905(for)X
555 3228(this)N
705(function)X
1021(is:)X
555 3352(XDestroySubwindows)N
1357(\()X
2 f
1393(display)X
1 f
1648(,)X
2 f
1692(w)X
1 f
1758(\))X
687 3448(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3544(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 3696(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3820(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 3944(The)N
5 f
732(XDestroySubwindows)X
1 f
1624(function)X
1940(destroys)X
2255(all)X
2366(inferior)X
2652(windows)X
2991(of)X
3086(the)X
3216(speci\256ed)X
3551(window,)X
3878(in)X
555 4040(bottom)N
828(to)X
919(top)X
1054(stacking)X
1370(order.)X
1621(It)X
1697(causes)X
1948(the)X
2078(X)X
2163(server)X
2399(to)X
2490(generate)X
2810(a)X
5 f
2889(DestroyNotify)X
1 f
3454(event)X
3667(for)X
3791(each)X
555 4136(window.)N
904(If)X
984(any)X
1133(mapped)X
1434(subwindows)X
1895(were)X
2087(actually)X
2389(destroyed,)X
5 f
2793(XDestroySubwindows)X
1 f
3685(causes)X
555 4232(the)N
685(X)X
770(sever)X
977(to)X
1068(generate)X
1388(exposure)X
1727(events)X
1974(on)X
2084(the)X
2214(speci\256ed)X
2549(window.)X
2898(This)X
3077(is)X
3158(much)X
3376(more)X
3579(ef\256cient)X
555 4328(than)N
729(deleting)X
1036(many)X
1254(windows)X
1593(one)X
1742(at)X
1828(a)X
1889(time)X
2069(because)X
2369(much)X
2587(of)X
2682(the)X
2812(work)X
3014(need)X
3202(only)X
3381(be)X
3486(performed)X
555 4424(once)N
743(for)X
867(all)X
978(of)X
1073(the)X
1203(windows)X
1542(rather)X
1769(than)X
1943(for)X
2067(each)X
2250(window.)X
2599(The)X
2758(subwindows)X
3219(should)X
3476(never)X
3693(again)X
3906(be)X
555 4520(referenced.)N
555 4644(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XDestroySubwindows)X
1 f
2694(is)X
5 f
2793(BadWindow)X
1 f
3277(.)X
2197 4820(Note)N
5 f
790 4956(XCloseDisplay)N
1 f
1397 0.2604(automatically)AX
1901(destroys)X
2216(all)X
2327(windows)X
2666(that)X
2821(have)X
3009(been)X
3197(created)X
3473(on)X
3583(that)X
775 5052(server)N
1011(if)X
1087(it)X
1159(is)X
1240(the)X
1370(last)X
1515(copy)X
1708(of)X
1803(the)X
1933(connection.)X
2386(\(Remember)X
2824(that)X
2979(under)X
3201(UNIX,)X
5 f
3481(fork)X
1 f
775 5148(will)N
935(duplicate)X
1281(open)X
1474(connections\).)X
3 f
555 5420(3.5.)N
731(Mapping)X
1096(Windows)X
1 f
555 5544(A)N
640(window)X
945(manager)X
1270(may)X
1444(want)X
1637(to)X
1728(control)X
2000(the)X
2130(placement)X
2515(of)X
2610(subwindows.)X
3115(If)X
5 f
3213(Substruc-)X
555 5640(tureRedirectMask)N
1 f
1281(has)X
1420(been)X
1608(selected)X
1914(by)X
2024(a)X
2085(window)X
2390(manager)X
2715(on)X
2825(a)X
2886(parent)X
3128(window)X
3433(\(usually)X
3739(a)X
3800(root)X
555 5736(window\),)N
911(a)X
972(map)X
1146(request)X
1422(initiated)X
1735(by)X
1845(other)X
2048(clients)X
2301(on)X
2411(a)X
2472(child)X
2671(window)X
2976(is)X
3057(not)X
3192(performed,)X
3602(and)X
3751(the)X
555 5832(window)N
860(manager)X
1185(would)X
1427(be)X
1532(sent)X
1696(a)X
5 f
1775(MapRequest)X
1 f
2311(event.)X
2568(However,)X
2933(if)X
3009(the)X
3139(override_redirect)X
3767(\257ag)X
3921(on)X
3 f
2239 6144(25)N

26 p
%%Page: 26 30
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(the)N
685(child)X
884(had)X
1033(been)X
1221(set)X
1341(to)X
5 f
1450(True)X
1 f
1668(\(usually)X
1974(only)X
2153(on)X
2263(pop-up)X
2534(menus\),)X
2837(the)X
2967(map)X
3141(request)X
3417(would)X
3659(be)X
3764(per-)X
555 768(formed.)N
555 892(A)N
640(tiling)X
850(window)X
1155(manager)X
1480(might)X
1709(decide)X
1961(to)X
2052(reposition)X
2427(and)X
2576(resize)X
2803(other)X
3006(client's)X
3288(windows)X
3627(and)X
3776(then)X
555 988(decide)N
807(to)X
898(map)X
1072(the)X
1202(window)X
1507(at)X
1593(its)X
1699(\256nal)X
1878(location.)X
2229(A)X
2314(window)X
2619(manager)X
2944(that)X
3099(wants)X
3326(to)X
3417(provide)X
3708(decora-)X
555 1084(tion)N
715(might)X
944(reparent)X
1254(the)X
1384(child)X
1583(into)X
1743(a)X
1804(frame)X
2031(\256rst.)X
2234(See)X
2383(Section)X
2670(3.2.7)X
2868(and)X
3017(Chapter)X
3318(8)X
3384(for)X
3508(further)X
3769(infor-)X
555 1180(mation.)N
867(Only)X
1065(a)X
1126(single)X
1359(client)X
1578(at)X
1664(a)X
1725(time)X
1905(can)X
2049(select)X
2272(for)X
5 f
2414(SubstructureRedirectMask)X
1 f
3466(.)X
555 1304(Similarly,)N
929(a)X
990(single)X
1223(client)X
1442(can)X
1586(select)X
1809(for)X
5 f
1951(ResizeRedirectMask)X
1 f
2793(on)X
2903(a)X
2964(parent)X
3206(window.)X
3555(Then,)X
3780(any)X
555 1400(attempt)N
843(to)X
934(resize)X
1161(the)X
1291(window)X
1596(is)X
1677(suppressed,)X
2106(and)X
2255(the)X
2385(client)X
2604(\(usually,)X
2932(a)X
2993(window)X
3298(manager\))X
3652(receives)X
3962(a)X
5 f
570 1496(ResizeRequest)N
1 f
1203(event.)X
1460(These)X
1692(mechanisms)X
2150(allow)X
2368(arbitrary)X
2693(placement)X
3078(policy)X
3321(to)X
3412(be)X
3517(enforced)X
3846(by)X
555 1592(an)N
660(external)X
966(window)X
1271(manager.)X
555 1716(A)N
640(window)X
945(is)X
1026(considered)X
1429(mapped)X
1730(if)X
1806(a)X
5 f
1885(XMapWindow)X
1 f
2464(call)X
2614(has)X
2753(been)X
2941(made)X
3154(on)X
3264(it.)X
3380(It)X
3456(may)X
3630(not)X
3765(be)X
555 1812(visible)N
813(on)X
923(the)X
1053(screen)X
1299(for)X
1423(one)X
1572(of)X
1667(the)X
1797(following)X
2162(reasons:)X
10 f
555 1936(g)N
1 f
775(It)X
851(is)X
932(obscured)X
1271(by)X
1381(another)X
1667(opaque)X
1943(sibling)X
2206(window.)X
10 f
555 2060(g)N
1 f
775(One)X
943(of)X
1038(its)X
1144(ancestors)X
1493(is)X
1574(not)X
1709(mapped.)X
10 f
555 2184(g)N
1 f
775(It)X
851(is)X
932(entirely)X
1224(clipped)X
1506(by)X
1616(an)X
1721(ancestor.)X
555 2308(Exposure)N
909(events)X
1156(will)X
1316(be)X
1421(generated)X
1785(for)X
1909(the)X
2039(window)X
2344(when)X
2556(part)X
2715(or)X
2810(all)X
2921(of)X
3016(it)X
3088(becomes)X
3418(visible)X
3676(on)X
3786(the)X
555 2404(screen.)N
823(A)X
908(client)X
1127(will)X
1287(only)X
1466(receive)X
1742(the)X
1872(exposure)X
2211(events)X
2458(if)X
2534(it)X
2606(has)X
2745(asked)X
2967(for)X
3091(them)X
3290(using)X
5 f
3521(XSelectIn-)X
555 2500(put)N
1 f
693(.)X
759(Windows)X
1118(retain)X
1341(their)X
1525(position)X
1832(in)X
1923(the)X
2053(stacking)X
2369(order)X
2576(when)X
2788(unmapped.)X
555 2720(Use)N
5 f
731(XMapWindow)X
1 f
1310(to)X
1401(map)X
1575(the)X
1705(speci\256ed)X
2040(window.)X
2367(The)X
2526(de\256nition)X
2887(for)X
3011(this)X
3161(function)X
3477(is:)X
555 2844(XMapWindow)N
1089(\()X
2 f
1125(display)X
1 f
1380(,)X
2 f
1424(w)X
1 f
1490(\))X
687 2940(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3036(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 3188(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3312(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 3436(The)N
5 f
732(XMapWindow)X
1 f
1311(function)X
1627(maps)X
1835(the)X
1965(window)X
2270(and)X
2419(all)X
2530(of)X
2625(its)X
2731(subwindows)X
3192(which)X
3429(have)X
3617(had)X
3766(map)X
555 3532(requests.)N
909(A)X
994(subwindow)X
1421(will)X
1581(appear)X
1837(on)X
1947(the)X
2077(screen)X
2323(as)X
2418(long)X
2597(as)X
2692(all)X
2803(of)X
2898(its)X
3004(ancestors)X
3353(are)X
3482(mapped)X
3783(and)X
555 3628(not)N
690(obscured)X
1029(by)X
1139(a)X
1200(sibling)X
1463(or)X
1558(are)X
1687(not)X
1822(clipped)X
2104(by)X
2214(an)X
2319(ancestor.)X
2678(Mapping)X
3018(a)X
3079(window)X
3384(that)X
3539(has)X
3678(an)X
555 3724(unmapped)N
944(ancestor)X
1259(does)X
1442(not)X
1577(display)X
1854(the)X
1984(window)X
2289(but)X
2424(marks)X
2661(it)X
2733(as)X
2828(eligible)X
3116(for)X
3240(display)X
3517(when)X
3729(the)X
555 3820(ancestor)N
870(becomes)X
1200(mapped.)X
1545(Such)X
1743(a)X
1804(window)X
2109(is)X
2190(called)X
2423(unviewable.)X
2895(When)X
3127(all)X
3238(its)X
3344(ancestors)X
3693(are)X
555 3916(mapped,)N
878(the)X
1008(window)X
1313(becomes)X
1643(viewable)X
1983(and)X
2132(will)X
2292(be)X
2397(visible)X
2655(on)X
2765(the)X
2895(screen)X
3141(if)X
3217(it)X
3289(is)X
3370(not)X
3505(obscured)X
3844(by)X
555 4012(any)N
704(sibling)X
967(or)X
1062(ancestor.)X
1421(This)X
1600(function)X
1916(has)X
2055(no)X
2165(effect)X
2387(if)X
2463(the)X
2593(window)X
2898(is)X
2979(already)X
3260(mapped.)X
555 4136(If)N
635(the)X
765(override_redirect)X
1393(member)X
1704(of)X
1799(the)X
5 f
1947(XSetWindowAttributes)X
1 f
2861(structure)X
3191(is)X
5 f
3290(False)X
1 f
3520(,)X
3564(and)X
3713(if)X
3789(some)X
555 4232(other)N
758(client)X
977(has)X
1116(selected)X
5 f
1440(SubstructureRedirectMask)X
1 f
2514(on)X
2624(the)X
2754(parent)X
2996(window,)X
3323(then)X
3497(the)X
3627(X)X
3712(server)X
555 4328(generates)N
909(a)X
5 f
988(MapRequest)X
1 f
1524(event,)X
1759(and)X
1908(the)X
5 f
2056(XMapWindow)X
1 f
2635(function)X
2951(does)X
3134(not)X
3269(map)X
3443(the)X
3573(window.)X
555 4424(Otherwise,)N
960(the)X
1090(window)X
1395(is)X
1476(mapped,)X
1799(and)X
1948(the)X
2078(X)X
2163(server)X
2399(generates)X
2753(a)X
5 f
2832(MapNotify)X
1 f
3265(event.)X
555 4548(If)N
635(the)X
765(window)X
1070(becomes)X
1400(viewable)X
1740(and)X
1889(no)X
1999(earlier)X
2246(contents)X
2562(for)X
2686(it)X
2758(are)X
2887(remembered,)X
5 f
3389(XMapWindow)X
1 f
555 4644(tiles)N
725(the)X
855(window)X
1160(with)X
1339(its)X
1445(background.)X
1926(If)X
2006(no)X
2116(background)X
2553(was)X
2711(de\256ned)X
2992(for)X
3116(the)X
3246(window,)X
3573(the)X
3703(existing)X
555 4740(screen)N
801(contents)X
1117(are)X
1246(not)X
1381(altered,)X
1665(and)X
1814(the)X
1944(X)X
2029(server)X
2265(generates)X
2619(one)X
2768(or)X
2863(more)X
5 f
3084(Expose)X
1 f
3415(events.)X
3706(If)X
3786(a)X
555 4836(backing_store)N
1071(was)X
1229(maintained)X
1644(while)X
1862(the)X
1992(window)X
2297(was)X
2455(unmapped,)X
2866(no)X
5 f
2994(Expose)X
1 f
3325(events)X
3572(are)X
3701(gen-)X
555 4932(erated.)N
836(If)X
916(a)X
977(backing_store)X
1493(will)X
1653(now)X
1826(be)X
1931(maintained,)X
2368(a)X
2429(full-window)X
2886(exposure)X
3225(is)X
3306(always)X
3572(generated.)X
555 5028(Otherwise,)N
960(only)X
1139(visible)X
1397(regions)X
1678(may)X
1852(be)X
1957(reported.)X
2316(Similar)X
2599(tiling)X
2809(and)X
2958(exposure)X
3297(take)X
3466(place)X
3674(for)X
3798(any)X
555 5124(newly)N
792(viewable)X
1132(inferiors.)X
555 5248(If)N
635(the)X
765(window)X
1070(is)X
1151(an)X
5 f
1274 0.3187(InputOutput)AX
1 f
1774(window,)X
5 f
2119(XMapWindow)X
1 f
2698(generates)X
5 f
3070(Expose)X
1 f
3401(events)X
3648(on)X
3758(each)X
5 f
570 5344 0.3187(InputOutput)AN
1 f
1070(window)X
1375(that)X
1530(it)X
1602(causes)X
1853(to)X
1944(become)X
2240(displayed.)X
2644(If)X
2724(the)X
2854(client)X
3073(maps)X
3281(and)X
3430(paints)X
3663(the)X
3793(win-)X
555 5440(dow,)N
750(and)X
899(if)X
975(the)X
1105(client)X
1324(begins)X
1576(processing)X
1974(events,)X
2243(the)X
2373(window)X
2678(will)X
2838(be)X
2943(painted)X
3225(twice.)X
3482(To)X
3602(avoid)X
3820(this,)X
555 5536(the)N
685(client)X
904(should)X
1161(call)X
5 f
1329(XSelectInput)X
1 f
1867(for)X
1991(exposure)X
2330(events,)X
2599(and)X
2748(map)X
2922(the)X
3052(window.)X
3379(Then,)X
3604(the)X
3734(client)X
555 5632(should)N
812(process)X
1097(input)X
1301(events)X
1548(normally.)X
1933(The)X
2092(event)X
2305(list)X
2436(will)X
2596(include)X
5 f
2896(Expose)X
1 f
3227(for)X
3351(each)X
3534(window)X
3839(that)X
555 5728(has)N
694(appeared)X
1033(on)X
1143(the)X
1273(screen.)X
1541(The)X
1700(client's)X
1982(normal)X
2254(response)X
2583(to)X
2674(an)X
5 f
2797(Expose)X
1 f
3128(event)X
3341(should)X
3598(be)X
3703(to)X
555 5824(repaint)N
822(the)X
952(window.)X
1301(This)X
1480(method)X
1767(usually)X
2044(leads)X
2247(to)X
2338(simpler)X
2625(programs)X
2979(and)X
3128(to)X
3219(proper)X
3470(interaction)X
3 f
2239 6144(26)N

27 p
%%Page: 27 31
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(with)N
734(window)X
1039(managers.)X
555 796(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XMapWindow)X
1 f
2381(is)X
5 f
2480(BadWindow)X
1 f
2964(.)X
555 1016(Use)N
5 f
731(XMapRaised)X
1 f
1271(to)X
1362(map)X
1536(and)X
1685(raise)X
1873(a)X
1934(window.)X
2283(The)X
2442(de\256nition)X
2803(for)X
2927(this)X
3077(function)X
3393(is:)X
555 1140(XMapRaised)N
1026(\()X
2 f
1062(display)X
1 f
1317(,)X
2 f
1361(w)X
1 f
1427(\))X
687 1236(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1332(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 1484(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1608(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 1732(The)N
5 f
732(XMapRaised)X
1 f
1272(function)X
1588(essentially)X
1983(is)X
2064(similar)X
2332(to)X
5 f
2441(XMapWindow)X
1 f
3020(in)X
3111(that)X
3266(it)X
3338(maps)X
3546(the)X
3676(window)X
555 1828(and)N
704(all)X
815(of)X
910(its)X
1016(subwindows)X
1477(which)X
1714(have)X
1902(had)X
2051(map)X
2225(requests.)X
2579(However,)X
2944(it)X
3016(also)X
3180(raises)X
3402(the)X
3532(speci\256ed)X
555 1924(window)N
860(to)X
951(the)X
1081(top)X
1216(of)X
1311(the)X
1441(stack.)X
1688(See)X
1837(the)X
5 f
1985(XMapWindow)X
1 f
2564(description)X
2978(for)X
3102(additional)X
3478(information.)X
555 2048(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XMapRaised)X
1 f
2342(is)X
5 f
2441(BadWindow)X
1 f
2925(.)X
555 2268(Use)N
5 f
731(XMapSubwindows)X
1 f
1491(to)X
1582(map)X
1756(all)X
1867(subwindows)X
2328(for)X
2452(a)X
2513(speci\256ed)X
2848(window.)X
3175(The)X
3334(de\256nition)X
3695(for)X
3819(this)X
555 2364(function)N
871(is:)X
555 2488(XMapSubwindows)N
1240(\()X
2 f
1276(display)X
1 f
1531(,)X
2 f
1575(w)X
1 f
1641(\))X
687 2584(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2680(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 2832(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2956(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 3080(The)N
5 f
732(XMapSubwindows)X
1 f
1492(function)X
1808(maps)X
2016(all)X
2127(subwindows)X
2588(for)X
2712(a)X
2773(speci\256ed)X
3108(window)X
3413(in)X
3504(top-to-bottom)X
555 3176(stacking)N
871(order.)X
1122(The)X
1281(X)X
1366(server)X
1602(to)X
1693(generate)X
2013(an)X
5 f
2136(Expose)X
1 f
2467(event)X
2680(on)X
2790(each)X
2973(newly)X
3210(displayed)X
3570(window.)X
555 3272(This)N
734(is)X
815(much)X
1033(more)X
1236(ef\256cient)X
1547(than)X
1721(mapping)X
2052(many)X
2270(windows)X
2609(one)X
2758(at)X
2844(a)X
2905(time,)X
3107(because)X
3407(the)X
3537(server)X
3773(needs)X
555 3368(only)N
734(perform)X
1039(much)X
1257(of)X
1352(the)X
1482(work)X
1684(once)X
1872(for)X
1996(all)X
2107(of)X
2202(the)X
2332(windows)X
2671(rather)X
2898(than)X
3072(for)X
3196(each)X
3379(window.)X
555 3492(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XMapSubwindows)X
1 f
2562(is)X
5 f
2661(BadWindow)X
1 f
3145(.)X
3 f
555 3684(3.6.)N
731(Unmapping)X
1198(Windows)X
1 f
555 3808(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(unmap)X
2385(a)X
2446(window)X
2751(or)X
2846(all)X
2957(subwindows.)X
555 4028(Use)N
5 f
731(XUnmapWindow)X
1 f
1422(to)X
1513(unmap)X
1775(a)X
1836(window.)X
2163(The)X
2322(de\256nition)X
2683(for)X
2807(this)X
2957(function)X
3273(is:)X
555 4152(XUnmapWindow)N
1187(\()X
2 f
1223(display)X
1 f
1478(,)X
2 f
1522(w)X
1 f
1588(\))X
687 4248(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4344(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 4496(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4620(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 4744(The)N
5 f
732(XUnmapWindow)X
1 f
1423(function)X
1739(unmaps)X
2035(the)X
2165(speci\256ed)X
2500(window)X
2805(and)X
2954(causes)X
3205(the)X
3335(X)X
3420(server)X
3656(to)X
3747(gen-)X
555 4840(erate)N
748(an)X
5 f
871(UnmapNotify)X
1 f
1416(event.)X
1673(If)X
1753(the)X
1883(speci\256ed)X
2218(window)X
2523(is)X
2604(already)X
2885(unmapped,)X
5 f
3314(XUnmapWindow)X
1 f
555 4936(has)N
694(no)X
804(effect.)X
1070(Normal)X
1361(exposure)X
1700(processing)X
2098(on)X
2208(formerly)X
2538(obscured)X
2877(windows)X
3216(is)X
3297(performed.)X
3729(Any)X
555 5032(child)N
754(window)X
1059(will)X
1219(no)X
1329(longer)X
1576(be)X
1681(visible)X
1939(until)X
2124(another)X
2410(map)X
2584(call)X
2734(is)X
2815(made)X
3028(on)X
3138(the)X
3268(parent.)X
3554(In)X
3649(other)X
555 5128(words,)N
813(the)X
943(subwindows)X
1404(are)X
1533(still)X
1689(mapped)X
1990(but)X
2125(not)X
2260(visible)X
2518(until)X
2703(the)X
2833(parent)X
3075(is)X
3156(mapped.)X
3501(Unmapping)X
3939(a)X
555 5224(window)N
860(will)X
1020(generate)X
1340(exposure)X
1679(events)X
1926(on)X
2036(windows)X
2375(that)X
2530(were)X
2722(formerly)X
3052(obscured)X
3391(by)X
3501(it)X
3573(and)X
3722(its)X
3828(chil-)X
555 5320(dren.)N
555 5444(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUnmapWindow)X
1 f
2493(is)X
5 f
2592(BadWindow)X
1 f
3076(.)X
555 5664(Use)N
5 f
731(XUnmapSubwindows)X
1 f
1603(to)X
1694(unmap)X
1956(all)X
2067(subwindows)X
2528(for)X
2652(a)X
2713(speci\256ed)X
3048(window.)X
3375(The)X
3534(de\256nition)X
3895(for)X
555 5760(this)N
705(function)X
1021(is:)X
3 f
2239 6144(27)N

28 p
%%Page: 28 32
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XUnmapSubwindows)N
1338(\()X
2 f
1374(display)X
1 f
1629(,)X
2 f
1673(w)X
1 f
1739(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 1016(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1140(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 1264(The)N
5 f
732(XUnmapSubwindows)X
1 f
1604(function)X
1920(unmaps)X
2216(all)X
2327(subwindows)X
2788(for)X
2912(the)X
3042(speci\256ed)X
3377(window)X
3682(in)X
3773(bot-)X
555 1360(tom)N
715(to)X
806(top)X
941(stacking)X
1257(order.)X
1508(It)X
1584(causes)X
1835(the)X
1965(X)X
2050(server)X
2286(to)X
2377(generate)X
2697(an)X
5 f
2820(UnmapNotify)X
1 f
3365(event)X
3578(on)X
3688(each)X
555 1456(subwindow)N
982(and)X
1131(exposure)X
1470(events)X
1717(on)X
1827(formerly)X
2157(obscured)X
2496(windows.)X
2879(Using)X
3111(this)X
3261(function)X
3577(is)X
3658(much)X
555 1552(more)N
758(ef\256cient)X
1069(than)X
1243(unmapping)X
1662(multiple)X
1980(windows)X
2319(one)X
2468(at)X
2554(a)X
2615(time,)X
2817(because)X
3117(the)X
3247(server)X
3483(needs)X
3705(only)X
555 1648(perform)N
860(much)X
1078(of)X
1173(the)X
1303(work)X
1505(once)X
1693(for)X
1817(all)X
1928(of)X
2023(the)X
2153(windows)X
2492(rather)X
2719(than)X
2893(for)X
3017(each)X
3200(window.)X
555 1772(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUnmapSubwindows)X
1 f
2674(is)X
5 f
2773(BadWindow)X
1 f
3257(.)X
3 f
555 1964(3.7.)N
731(Con\256guring)X
1208(Windows)X
1 f
555 2088(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(move)X
2341(a)X
2402(window,)X
2729(resize)X
2956(a)X
3017(window,)X
3344(move)X
3562(and)X
3711(resize)X
3938(a)X
555 2184(window,)N
882(or)X
977(change)X
1248(a)X
1309(window's)X
1677(border)X
1928(width.)X
2195(The)X
2354(most)X
2548(general)X
2829(interface)X
3159(to)X
3250(con\256guring)X
3678(win-)X
555 2280(dows,)N
5 f
802(XCon\256gureWindow)X
1 f
1569(,)X
1613(uses)X
1786(the)X
5 f
1934(XWindowChanges)X
1 f
2694(structure,)X
3046(which)X
3283(contains:)X
555 2404(#de\256ne)N
836(CWX)X
2283(\(1<<0\))X
555 2500(#de\256ne)N
836(CWY)X
2283(\(1<<1\))X
555 2596(#de\256ne)N
836(CWWidth)X
2283(\(1<<2\))X
555 2692(#de\256ne)N
836(CWHeight)X
2283(\(1<<3\))X
555 2788(#de\256ne)N
836(CWBorderWidth)X
2283(\(1<<4\))X
555 2884(#de\256ne)N
836(CWSibling)X
2283(\(1<<5\))X
555 2980(#de\256ne)N
836(CWStackMode)X
2283(\(1<<6\))X
555 3124(typedef)N
841(struct)X
1059({)X
843 3220(int)N
959(x,)X
1047(y;)X
843 3316(int)N
959(width,)X
1204(height;)X
843 3412(int)N
959(border_width;)X
843 3508(Window)N
1168(sibling;)X
843 3604(int)N
959(stack_mode;)X
555 3700(})N
619(XWindowChanges;)X
555 3872(The)N
714(following)X
1079(paragraphs)X
1486(discuss)X
1762(the)X
1892(members)X
2237(of)X
2332(this)X
2482(structure)X
2812(and)X
2961(the)X
3091(processing)X
3489(that)X
3644(occurs)X
3895(for)X
5 f
570 3968(XCon\256gureWindow)N
1 f
1337(.)X
555 4092(The)N
714(x)X
780(and)X
929(y)X
995(members)X
1340(specify)X
1616(the)X
1746(x)X
1812(and)X
1961(y)X
2027(coordinates)X
2455(relative)X
2742(to)X
2833(the)X
2963(parent's)X
3268(origin)X
3501(and)X
3650(indicate)X
555 4188(the)N
685(position)X
992(of)X
1087(the)X
1217(upper)X
1439(left)X
1579(outer)X
1782(corner)X
2028(of)X
2123(the)X
2253(window.)X
2602(The)X
2761(width)X
2984(and)X
3133(height)X
3376(members)X
3721(specify)X
555 4284(the)N
685(inside)X
918(size)X
1077(of)X
1172(the)X
1302(window,)X
1629(not)X
1764(including)X
2120(the)X
2250(border.)X
2545(These)X
2777(arguments)X
3166(must)X
3360(be)X
3465(nonzero.)X
3814(Oth-)X
555 4380(erwise,)N
828(a)X
5 f
907(BadValue)X
1 f
1326(error)X
1518(is)X
1599(generated.)X
2007(Attempts)X
2353(to)X
2444(con\256gure)X
2798(a)X
2859(root)X
3023(window)X
3328(have)X
3516(no)X
3626(effect.)X
555 4504(The)N
714(border_width)X
1210(member)X
1521(speci\256es)X
1846(the)X
1976(width)X
2199(of)X
2294(the)X
2424(border)X
2675(in)X
2766(pixels.)X
3043(Note)X
3236(that)X
3391(changing)X
3736(just)X
3886(the)X
555 4600(border_width)N
1051(leaves)X
1293(the)X
1423(outer-left)X
1773(corner)X
2019(of)X
2114(the)X
2244(window)X
2549(in)X
2640(a)X
2701(\256xed)X
2899(position,)X
3228(but)X
3363(moves)X
3615(the)X
3745(abso-)X
555 4696(lute)N
710(position)X
1017(of)X
1112(the)X
1242(window's)X
1610(origin.)X
1887(A)X
5 f
1990(BadMatch)X
1 f
2424(error)X
2616(is)X
2697(generated)X
3061(if)X
3137(you)X
3291(attempt)X
3579(to)X
3670(make)X
3883(the)X
555 4792(border_width)N
1051(of)X
1146(an)X
5 f
1269(InputOnly)X
1 f
1684(window)X
1989(nonzero.)X
555 4916(The)N
714(sibling)X
977(member)X
1288(speci\256es)X
1613(the)X
1743(sibling)X
2006(window)X
2311(for)X
2435(stacking)X
2751(operations.)X
3184(The)X
3343(stack_mode)X
555 5012(member)N
866(can)X
1010(be)X
1115(one)X
1264(of)X
1359(these)X
1562(constants:)X
5 f
1955(Above)X
1 f
2220(,)X
5 f
2282(Below)X
1 f
2536(,)X
5 f
2598(TopIf)X
1 f
2815(,)X
5 f
2877(BottomIf)X
1 f
3222(,)X
3266(or)X
5 f
3379(Opposite)X
1 f
3747(.)X
555 5136(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2569(,)X
2613(and)X
2762(if)X
2838(some)X
3046(other)X
3249(client)X
3468(has)X
3607(selected)X
5 f
570 5232(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(then)X
2322(the)X
2452(X)X
2537(server)X
2773(generates)X
3127(a)X
5 f
3206(Con\256gureRequest)X
1 f
555 5328(event,)N
790(and)X
939(no)X
1049(further)X
1310(processing)X
1708(is)X
1789(performed.)X
2221(Otherwise,)X
2626(if)X
2702(some)X
2910(other)X
3113(client)X
3332(has)X
3471(selected)X
5 f
570 5424(ResizeRedirectMask)N
1 f
1412(on)X
1522(the)X
1652(window)X
1957(and)X
2106(the)X
2236(inside)X
2469(width)X
2692(or)X
2787(height)X
3030(of)X
3125(the)X
3255(window)X
3560(is)X
3641(being)X
555 5520(changed,)N
892(a)X
5 f
971(ResizeRequest)X
1 f
1604(event)X
1817(is)X
1898(generated,)X
2284(and)X
2433(the)X
2563(current)X
2834(inside)X
3067(width)X
3290(and)X
3439(height)X
3682(are)X
3811(used)X
555 5616(instead)N
827(in)X
918(the)X
1048(following.)X
1457(Note)X
1650(that)X
1805(the)X
1935(override_redirect)X
2563(attribute)X
2880(of)X
2975(the)X
3105(window)X
3410(has)X
3549(no)X
3659(effect)X
3881(on)X
5 f
570 5712(ResizeRedirectMask)N
1 f
1412(and)X
1561(that)X
5 f
1734(SubstructureRedirectMask)X
1 f
2808(on)X
2918(the)X
3048(parent)X
3290(has)X
3429(precedence)X
3846(over)X
5 f
570 5808(ResizeRedirectMask)N
1 f
1412(on)X
1522(the)X
1652(window.)X
3 f
2239 6144(28)N

29 p
%%Page: 29 33
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(geometry)X
1069(of)X
1164(the)X
1294(window)X
1599(is)X
1680(changed)X
1995(as)X
2090(speci\256ed,)X
2447(the)X
2577(window)X
2882(is)X
2963(restacked)X
3317(among)X
3579(siblings,)X
555 768(and)N
704(a)X
5 f
783(Con\256gureNotify)X
1 f
1426(event)X
1639(is)X
1720(generated.)X
2128(X)X
2213(generates)X
5 f
2585(GravityNotify)X
1 f
3126(events)X
3373(after)X
3556(generating)X
5 f
570 864(Con\256gureNotify)N
1 f
1213(events.)X
1504(If)X
1584(the)X
1714(inside)X
1947(width)X
2170(or)X
2265(height)X
2508(of)X
2603(the)X
2733(window)X
3038(has)X
3177(actually)X
3479(changed,)X
3816(then)X
555 960(children)N
866(of)X
961(the)X
1091(window)X
1396(are)X
1525(affected)X
1830(as)X
1925(described)X
2284(below.)X
555 1084(Exposure)N
909(processing)X
1307(is)X
1388(performed)X
1776(on)X
1886(formerly)X
2216(obscured)X
2555(windows,)X
2916(including)X
3272(the)X
3402(window)X
3707(itself)X
555 1180(and)N
704(its)X
810(inferiors,)X
1152(if)X
1228(regions)X
1509(of)X
1604(them)X
1803(were)X
1995(obscured)X
2334(but)X
2469(now)X
2642(are)X
2771(not.)X
2950(As)X
3069(a)X
3130(result)X
3348(of)X
3443(increasing)X
3827(the)X
555 1276(width)N
778(or)X
873(height,)X
1138(exposure)X
1477(processing)X
1875(is)X
1956(also)X
2120(performed)X
2508(on)X
2618(any)X
2767(new)X
2935(regions)X
3216(of)X
3311(the)X
3441(window)X
3746(and)X
555 1372(any)N
704(regions)X
985(where)X
1221(window)X
1526(contents)X
1842(are)X
1971(lost.)X
555 1496(If)N
635(the)X
765(inside)X
998(width)X
1221(or)X
1316(height)X
1559(of)X
1654(a)X
1715(window)X
2020(is)X
2101(not)X
2236(changed,)X
2573(and)X
2722(if)X
2798(the)X
2928(window)X
3233(is)X
3314(moved)X
3576(or)X
3671(its)X
3777(border)X
555 1592(is)N
636(changed,)X
973(then)X
1147(the)X
1277(contents)X
1593(of)X
1688(the)X
1818(window)X
2123(are)X
2252(not)X
2387(lost)X
2537(but)X
2672(move)X
2890(with)X
3069(the)X
3199(window.)X
3548(Changing)X
555 1688(the)N
685(inside)X
918(width)X
1141(or)X
1236(height)X
1479(of)X
1574(the)X
1704(window)X
2009(causes)X
2260(its)X
2366(contents)X
2682(to)X
2773(be)X
2878(moved)X
3140(or)X
3235(lost,)X
3407(depending)X
3796(on)X
555 1784(the)N
685(bit_gravity)X
1095(of)X
1190(the)X
1320(window,)X
1647(and)X
1796(causes)X
2047(children)X
2358(to)X
2449(be)X
2554(recon\256gured,)X
3042(depending)X
3431(on)X
3541(their)X
555 1880(win_gravity.)N
1025(For)X
1169(a)X
1230(change)X
1501(of)X
1596(width)X
1819(and)X
1968(height,)X
2233(the)X
2363(\(x,)X
2480(y\))X
2575(pairs)X
2768(are)X
2897(de\256ned:)X
10 f
555 1964(i)N
589(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 2108(Gravity)N
846(Direction)X
1428(Coordinates)X
10 f
555 2164(i)N
589(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
5 f
570 2308(NorthWestGravity)N
1 f
1428(\(0,)X
1545(0\))X
5 f
570 2404(NorthGravity)N
1 f
1428(\(Width/2,)X
1791(0\))X
5 f
570 2500(NorthEastGravity)N
1 f
1428(\(Width,)X
1722(0\))X
5 f
570 2596(WestGravity)N
1 f
1428(\(0,)X
1545(Height/2\))X
5 f
570 2692(CenterGravity)N
1 f
1428(\(Width/2,)X
1791(Height/2\))X
5 f
570 2788(EastGravity)N
1 f
1428(\(Width,)X
1722(Height/2\))X
5 f
570 2884(SouthWestGravity)N
1 f
1428(\(0,)X
1545(Height\))X
5 f
570 2980(SouthGravity)N
1 f
1428(\(Width/2,)X
1791(Height\))X
5 f
570 3076(SouthEastGravity)N
1 f
1428(\(Width,)X
1722(Height\))X
10 f
555 3132(i)N
589(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 3304(When)N
787(a)X
848(window)X
1153(with)X
1332(one)X
1481(of)X
1576(these)X
1779(bit_gravities)X
2243(is)X
2324(resized,)X
2617(the)X
2747(corresponding)X
3272(pair)X
3431(de\256nes)X
3702(the)X
555 3400(change)N
826(in)X
917(position)X
1224(of)X
1319(each)X
1502(pixel)X
1701(in)X
1792(the)X
1922(window.)X
2271(When)X
2503(a)X
2564(window)X
2869(with)X
3048(one)X
3197(of)X
3292(these)X
3495(win_gravities)X
555 3496(has)N
694(its)X
800(parent)X
1042(window)X
1347(resized,)X
1640(the)X
1770(corresponding)X
2295(pair)X
2454(de\256nes)X
2725(the)X
2855(change)X
3126(in)X
3217(position)X
3524(of)X
3619(the)X
3749(win-)X
555 3592(dow)N
728(within)X
976(the)X
1106(parent.)X
1392(When)X
1624(a)X
1685(window)X
1990(is)X
2071(so)X
2171(repositioned,)X
2651(a)X
5 f
2730(GravityNotify)X
1 f
3271(event)X
3484(is)X
3565(generated.)X
555 3716(A)N
640(bit_gravity)X
1050(of)X
5 f
1163(StaticGravity)X
1 f
1700(indicates)X
2036(that)X
2191(the)X
2321(contents)X
2637(or)X
2732(origin)X
2965(should)X
3222(not)X
3357(move)X
3575(relative)X
3862(to)X
555 3812(the)N
685(origin)X
918(of)X
1013(the)X
1143(root)X
1307(window.)X
1656(If)X
1736(the)X
1866(change)X
2137(in)X
2228(size)X
2387(of)X
2482(the)X
2612(window)X
2917(is)X
2998(coupled)X
3299(with)X
3478(a)X
3539(change)X
3810(in)X
555 3908(position)N
862(\(x,)X
979(y\),)X
1096(then)X
1270(for)X
1394(bit_gravity)X
1804(the)X
1934(change)X
2205(in)X
2296(position)X
2603(of)X
2698(each)X
2881(pixel)X
3080(is)X
3161(\(-x,)X
3307(-y\),)X
3453(and)X
3602(for)X
555 4004(win_gravity)N
1003(the)X
1133(change)X
1404(in)X
1495(position)X
1802(of)X
1897(a)X
1958(child)X
2157(when)X
2369(its)X
2475(parent)X
2717(is)X
2798(so)X
2898(resized)X
3169(is)X
3250(\(-x,)X
3396(-y\).)X
3564(Note)X
3757(that)X
5 f
570 4100(StaticGravity)N
1 f
1107(still)X
1263(only)X
1442(takes)X
1645(effect)X
1867(when)X
2079(the)X
2209(width)X
2432(or)X
2527(height)X
2770(of)X
2865(the)X
2995(window)X
3300(is)X
3381(changed,)X
3718(not)X
555 4196(when)N
767(the)X
897(window)X
1202(is)X
1283(moved.)X
555 4320(A)N
640(bit_gravity)X
1050(of)X
5 f
1163(ForgetGravity)X
1 f
1734(indicates)X
2070(that)X
2225(the)X
2355(window's)X
2723(contents)X
3039(are)X
3168(always)X
3434(discarded)X
3793(after)X
555 4416(a)N
616(size)X
775(change,)X
1068(even)X
1256(if)X
1332(a)X
1393(backing_store)X
1909(or)X
2004(save_under)X
2426(has)X
2565(been)X
2753(requested.)X
3156(The)X
3315(window)X
3620(is)X
3701(tiled)X
555 4512(with)N
734(its)X
840(background)X
1277(and)X
1426(one)X
1575(or)X
1670(more)X
1873(exposure)X
2212(events)X
2459(are)X
2588(generated.)X
2974(If)X
3054(no)X
3164(background)X
3601(is)X
3682(de\256ned,)X
555 4608(the)N
685(existing)X
987(screen)X
1233(contents)X
1549(are)X
1678(not)X
1813(altered.)X
2119(Some)X
2342(X)X
2427(servers)X
2697(may)X
2871(also)X
3035(ignore)X
3282(the)X
3412(speci\256ed)X
555 4704(bit_gravity)N
965(and)X
1114(always)X
1380(generate)X
1700(exposure)X
2039(events.)X
555 4828(A)N
640(win_gravity)X
1088(of)X
5 f
1201(UnmapGravity)X
1 f
1800(is)X
1881(like)X
2036(NorthWest)X
2444(\(the)X
2603(window)X
2908(is)X
2989(not)X
3124(moved\),)X
3437(but)X
3572(the)X
3702(child)X
3901(is)X
555 4924(also)N
719(unmapped)X
1108(when)X
1320(the)X
1450(parent)X
1692(is)X
1773(resized,)X
2066(and)X
2215(an)X
5 f
2338(UnmapNotify)X
1 f
2883(event)X
3096(is)X
3177(generated.)X
3585(X)X
3670(generates)X
5 f
570 5020(GravityNotify)N
1 f
1111(notify)X
1344(events)X
1591(after)X
1774(generating)X
5 f
2186(Con\256gureNotify)X
1 f
2829(events.)X
3120(A)X
3205(win_gravity)X
3653(of)X
5 f
570 5116(AntiGravity)N
1 f
1038(indicates)X
1374(all)X
1485(pixels)X
1718(should)X
1975(move)X
2193(radically)X
2524(outward)X
2834(from)X
3027(the)X
3157(center)X
3394(of)X
3489(the)X
3619(window.)X
9 f
3924(\261)X
1 f
555 5240(The)N
714(restack)X
985(check)X
1212(\(speci\256cally,)X
1687(the)X
1817(computation)X
2281(for)X
5 f
2423(BottomIf)X
1 f
2768(,)X
5 f
2830(TopIf)X
1 f
3047(,)X
3091(and)X
5 f
3258(Opposite)X
1 f
3626(\))X
3677(is)X
3758(per-)X
555 5336(formed)N
831(with)X
1010(respect)X
1281(to)X
1372(the)X
1502(window's)X
1870(\256nal)X
2049(size)X
2208(and)X
2357(position)X
2664(\(as)X
2788(controlled)X
3168(by)X
3278(the)X
3408(other)X
3611(arguments)X
555 5432(to)N
646(the)X
776(request\),)X
1103(not)X
1238(its)X
1344(initial)X
1574(position.)X
1925(It)X
2001(is)X
2082(an)X
2187(error)X
2379(if)X
2455(a)X
2516(sibling)X
2779(is)X
2860(speci\256ed)X
3195(without)X
3487(a)X
3548(stack_mode.)X
7 s
10 f
555 5592(h)N
571(hhhhhhhhhhhhhhhhhhhh)X
9 f
555 5664(\261)N
1 f
600(\(If)X
671(you)X
769(believe)X
946(this)X
1042(statement,)X
1287(there)X
1414(is)X
1466(a)X
1505(bridge)X
1663(for)X
1743(sale.\))X
3 f
11 s
2239 6144(29)N

30 p
%%Page: 30 34
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(If)N
635(a)X
696(sibling)X
959(and)X
1108(a)X
1169(stack_mode)X
1612(are)X
1741(speci\256ed,)X
2098(the)X
2228(window)X
2533(is)X
2614(restacked)X
2968(as)X
3063(follows:)X
5 f
570 816(Above)N
1 f
1263(The)X
1422(window)X
1727(is)X
1808(placed)X
2060(just)X
2210(above)X
2442(the)X
2572(sibling.)X
5 f
570 960(Below)N
1 f
1263(The)X
1422(window)X
1727(is)X
1808(placed)X
2060(just)X
2210(below)X
2447(the)X
2577(sibling.)X
5 f
570 1104(TopIf)N
1 f
1263(If)X
1343(the)X
1473(sibling)X
1736(occludes)X
2066(the)X
2196(window,)X
2523(the)X
2653(window)X
2958(is)X
3039(placed)X
3291(at)X
3377(the)X
3507(top)X
3642(of)X
3737(the)X
3867(stack.)X
5 f
570 1248(BottomIf)N
1 f
1263(If)X
1343(the)X
1473(window)X
1778(occludes)X
2108(the)X
2238(sibling,)X
2523(the)X
2653(window)X
2958(is)X
3039(placed)X
3291(at)X
3377(the)X
3507(bottom)X
3780(of)X
3875(the)X
1263 1344(stack.)N
5 f
570 1488(Opposite)N
1 f
1263(If)X
1343(the)X
1473(sibling)X
1736(occludes)X
2066(the)X
2196(window,)X
2523(the)X
2653(window)X
2958(is)X
3039(placed)X
3291(at)X
3377(the)X
3507(top)X
3642(of)X
3737(the)X
3867(stack.)X
1263 1584(Otherwise,)N
1668(if)X
1744(the)X
1874(window)X
2179(occludes)X
2509(the)X
2639(sibling,)X
2924(the)X
3054(window)X
3359(is)X
3440(placed)X
3692(at)X
3778(the)X
3908(bot-)X
1263 1680(tom)N
1423(of)X
1518(the)X
1648(stack.)X
555 1852(If)N
635(a)X
696(stack_mode)X
1139(is)X
1220(speci\256ed)X
1555(but)X
1690(no)X
1800(sibling)X
2063(is)X
2144(speci\256ed,)X
2501(the)X
2631(window)X
2936(is)X
3017(restacked)X
3371(as)X
3466(follows:)X
5 f
570 1996(Above)N
1 f
1263(The)X
1422(window)X
1727(is)X
1808(placed)X
2060(at)X
2146(the)X
2276(top)X
2411(of)X
2506(the)X
2636(stack.)X
5 f
570 2140(Below)N
1 f
1263(The)X
1422(window)X
1727(is)X
1808(placed)X
2060(at)X
2146(the)X
2276(bottom)X
2549(of)X
2644(the)X
2774(stack.)X
5 f
570 2284(TopIf)N
1 f
1263(If)X
1343(any)X
1492(sibling)X
1755(occludes)X
2085(the)X
2215(window,)X
2542(the)X
2672(window)X
2977(is)X
3058(placed)X
3310(at)X
3396(the)X
3526(top)X
3661(of)X
3756(the)X
3886(stack.)X
5 f
570 2428(BottomIf)N
1 f
1263(If)X
1343(the)X
1473(window)X
1778(occludes)X
2108(any)X
2257(sibling,)X
2542(the)X
2672(window)X
2977(is)X
3058(placed)X
3310(at)X
3396(the)X
3526(bottom)X
3799(of)X
3894(the)X
1263 2524(stack.)N
5 f
570 2668(Opposite)N
1 f
1263(If)X
1343(any)X
1492(sibling)X
1755(occludes)X
2085(the)X
2215(window,)X
2542(the)X
2672(window)X
2977(is)X
3058(placed)X
3310(at)X
3396(the)X
3526(top)X
3661(of)X
3756(the)X
3886(stack.)X
1263 2764(Otherwise,)N
1668(if)X
1744(the)X
1874(window)X
2179(occludes)X
2509(any)X
2658(sibling,)X
2943(the)X
3073(window)X
3378(is)X
3459(placed)X
3711(at)X
3797(the)X
3927(bot-)X
1263 2860(tom)N
1423(of)X
1518(the)X
1648(stack.)X
555 3128(Use)N
5 f
731(XCon\256gureWindow)X
1 f
1520(to)X
1611(recon\256gure)X
2033(a)X
2094(window's)X
2462(size,)X
2643(position,)X
2972(border,)X
3245(and)X
3394(stacking)X
3710(order.)X
555 3224(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 3348(XCon\256gureWindow)N
1280(\()X
2 f
1316(display)X
1 f
1571(,)X
2 f
1615(w)X
1 f
1681(,)X
2 f
1725(value_mask)X
1 f
2147(,)X
2 f
2191(values)X
1 f
2423(\))X
687 3444(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3540(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3636(unsigned)N
1027(int)X
2 f
1143(value_mask)X
1 f
1565(;)X
687 3732(XWindowChanges)N
1378(*)X
2 f
(values)S
1 f
1654(;)X
2 f
555 3884(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4008(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(to)X
2850(be)X
2955(recon\256gured.)X
2 f
555 4132(value_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(values)X
1955(are)X
2084(to)X
2175(be)X
2280(set)X
2400(using)X
2613(information)X
3052(in)X
3143(the)X
3273(values)X
3520(structure.)X
1131 4228(This)N
1310(mask)X
1518(is)X
1599(the)X
1729(bitwise)X
2006(inclusive)X
2347(OR)X
2491(of)X
2586(the)X
2716(valid)X
2915(change)X
3186(window)X
3491(values)X
3738(bits.)X
2 f
555 4352(values)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(structure)X
5 f
2373(XWindowChanges)X
1 f
3111(.)X
555 4476(The)N
5 f
732(XCon\256gureWindow)X
1 f
1521(function)X
1837(uses)X
2010(the)X
2140(values)X
2387(speci\256ed)X
2722(in)X
2813(the)X
5 f
2961(XWindowChanges)X
1 f
3721(struc-)X
555 4572(ture)N
714(to)X
805(recon\256gure)X
1227(a)X
1288(window's)X
1656(size,)X
1837(position,)X
2166(border,)X
2439(and)X
2588(stacking)X
2904(order.)X
3155(The)X
3314(stacking)X
3630(order)X
3837(of)X
555 4668(the)N
685(window)X
990(is)X
1071(controlled)X
1451(by)X
1561(the)X
1691(function's)X
2070(arguments.)X
2503(Values)X
2769(not)X
2904(speci\256ed)X
3239(are)X
3368(taken)X
3581(from)X
3774(the)X
555 4764(existing)N
857(geometry)X
1212(of)X
1307(the)X
1437(window.)X
555 4888(A)N
5 f
658(BadMatch)X
1 f
1092(error)X
1284(is)X
1365(generated)X
1729(if)X
1805(a)X
1866(sibling)X
2129(is)X
2210(speci\256ed)X
2545(without)X
2837(a)X
2898(stack_mode)X
3341(or)X
3436(if)X
3512(the)X
3642(window)X
3947(is)X
555 4984(not)N
690(actually)X
992(a)X
1053(sibling.)X
1360(Note)X
1553(that)X
1708(the)X
1838(computations)X
2336(for)X
5 f
2478(BottomIf)X
1 f
2823(,)X
5 f
2885(TopIf)X
1 f
3102(,)X
3146(and)X
5 f
3313(Opposite)X
1 f
3703(are)X
3832(per-)X
555 5080(formed)N
831(with)X
1010(respect)X
1281(to)X
1372(the)X
1502(window's)X
1870(\256nal)X
2049(geometry)X
2404(\(as)X
2528(controlled)X
2908(by)X
3018(the)X
3148(other)X
3351(arguments)X
3740(passed)X
555 5176(to)N
5 f
664(XCon\256gureWindow)X
1 f
1431(\),)X
1504(not)X
1639(its)X
1745(initial)X
1975(geometry.)X
2374(Any)X
2547(backing-store)X
3048(contents)X
3364(of)X
3459(the)X
3589(window,)X
3916(its)X
555 5272(inferiors,)N
897(and)X
1046(other)X
1249(newly)X
1486(visible)X
1744(windows)X
2083(are)X
2212(either)X
2435(discarded)X
2794(or)X
2889(changed)X
3204(to)X
3295(re\257ect)X
3537(the)X
3667(current)X
555 5368(screen)N
801(contents)X
1117 0.2455(\(implementation)AX
1724(dependent\).)X
555 5492(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCon\256gureWindow)X
1 f
2625(are)X
5 f
2772(BadWindow)X
1 f
3256(,)X
5 f
3318(BadMatch)X
1 f
3730(,)X
3774(and)X
5 f
570 5588(BadValue)N
1 f
967(.)X
555 5808(Use)N
5 f
731(XMoveWindow)X
1 f
1354(to)X
1445(move)X
1663(a)X
1724(window)X
2029(without)X
2321(changing)X
2666(its)X
2772(size.)X
2953(The)X
3112(de\256nition)X
3473(for)X
3597(this)X
3 f
2239 6144(30)N

31 p
%%Page: 31 35
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(function)N
871(is:)X
555 796(XMoveWindow)N
1133(\()X
2 f
1169(display)X
1 f
1424(,)X
2 f
1468(w)X
1 f
1534(,)X
2 f
1578(x)X
1 f
1624(,)X
2 f
1668(y)X
1 f
1714(\))X
687 892(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 988(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 1084(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
2 f
555 1236(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1360(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(to)X
2850(be)X
2955(moved.)X
2 f
555 1484(x)N
555 1580(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(new)X
3500(location)X
3807(of)X
3902(the)X
1131 1676(top)N
1266(left)X
1406(pixel)X
1605(of)X
1700(the)X
1830(window's)X
2198(border)X
2449(or)X
2544(the)X
2674(window)X
2979(itself,)X
3200(if)X
3276(it)X
3348(has)X
3487(no)X
3597(border.)X
555 1800(The)N
5 f
732(XMoveWindow)X
1 f
1355(function)X
1671(moves)X
1923(the)X
2053(speci\256ed)X
2388(window)X
2693(to)X
2784(the)X
2914(speci\256ed)X
3249(x)X
3315(and)X
3464(y)X
3530(coordinates.)X
555 1896(This)N
734(function)X
1050(does)X
1233(not)X
1368(change)X
1639(the)X
1769(window's)X
2137(size,)X
2318(does)X
2501(not)X
2636(raise)X
2824(the)X
2954(window,)X
3281(and)X
3430(does)X
3613(not)X
3748(change)X
555 1992(the)N
685(mapping)X
1016(state)X
1200(of)X
1295(the)X
1425(window.)X
1774(Moving)X
2075(a)X
2136(mapped)X
2437(window)X
2742(may)X
2916(or)X
3011(may)X
3185(not)X
3320(lose)X
3484(its)X
3590(contents)X
555 2088(depending)N
944(on:)X
10 f
555 2212(g)N
1 f
775(If)X
855(its)X
961(tile)X
1097(mode)X
1315(is)X
1396(relative)X
10 f
555 2336(g)N
1 f
775(If)X
855(the)X
985(window)X
1290(is)X
1371(obscured)X
1710(by)X
1820(nonchildren)X
555 2460(If)N
635(the)X
765(contents)X
1081(of)X
1176(the)X
1306(window)X
1611(are)X
1740(lost,)X
1912(exposure)X
2251(events)X
2498(will)X
2658(be)X
2763(generated)X
3127(for)X
3251(the)X
3381(window)X
3686(and)X
3835(any)X
555 2556(mapped)N
856(subwindows.)X
1361(Moving)X
1662(a)X
1723(mapped)X
2024(window)X
2329(will)X
2489(generate)X
2809(exposure)X
3148(events)X
3395(on)X
3505(any)X
3654(formerly)X
555 2652(obscured)N
916(windows.)X
555 2776(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2591(and)X
2740(some)X
2948(other)X
3151(client)X
3370(has)X
3509(selected)X
5 f
570 2872(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(a)X
5 f
2227(Con\256gureRequest)X
1 f
2973(event)X
3186(is)X
3267(generated,)X
3653(and)X
3802(no)X
555 2968(further)N
816(processing)X
1214(is)X
1295(performed.)X
1705(Otherwise,)X
2110(the)X
2240(window)X
2545(is)X
2626(moved.)X
555 3092(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XMoveWindow)X
1 f
2425(is)X
5 f
2524(BadWindow)X
1 f
3008(.)X
555 3312(Use)N
5 f
731(XResizeWindow)X
1 f
1407(to)X
1498(change)X
1769(a)X
1830(window's)X
2198(size)X
2357(without)X
2649(changing)X
2994(the)X
3124(upper)X
3346(left)X
3486(coordinate.)X
555 3408(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XResizeWindow)X
1 f
1893(is:)X
555 3532(XResizeWindow)N
1163(\()X
2 f
1199(display)X
1 f
1454(,)X
2 f
1498(w)X
1 f
1564(,)X
2 f
1608(width)X
1 f
1812(,)X
2 f
1856(height)X
1 f
2084(\))X
687 3628(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3724(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3820(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
2 f
555 3972(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4096(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 4220(width)N
555 4316(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(dimensions)X
3126(of)X
3221(the)X
3351(window)X
3656(after)X
3839(the)X
1131 4412(call)N
1281(completes.)X
555 4536(The)N
5 f
732(XResizeWindow)X
1 f
1408(function)X
1724(changes)X
2029(the)X
2159(inside)X
2392(dimensions)X
2816(of)X
2911(the)X
3041(speci\256ed)X
3376(window,)X
3703(not)X
555 4632(including)N
911(its)X
1017(borders.)X
1346(This)X
1525(function)X
1841(does)X
2024(not)X
2159(change)X
2430(the)X
2560(window's)X
2928(upper-left)X
3297(coordinate)X
3691(or)X
3786(the)X
555 4728(origin)N
788(and)X
937(does)X
1120(not)X
1255(raise)X
1443(the)X
1573(window.)X
1922(Changing)X
2287(the)X
2417(size)X
2576(of)X
2671(a)X
2732(mapped)X
3033(window)X
3338(may)X
3512(lose)X
3676(its)X
3782(con-)X
555 4824(tents)N
744(and)X
893(generate)X
1213(an)X
5 f
1336(Expose)X
1 f
1667(event.)X
1924(If)X
2004(a)X
2065(mapped)X
2366(window)X
2671(is)X
2752(made)X
2965(smaller,)X
3269(exposure)X
3608(events)X
3855(will)X
555 4920(be)N
660(generated)X
1024(on)X
1134(windows)X
1473(that)X
1628(it)X
1700(formerly)X
2030(obscured.)X
555 5044(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2591(and)X
2740(some)X
2948(other)X
3151(client)X
3370(has)X
3509(selected)X
5 f
570 5140(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(a)X
5 f
2227(Con\256gureRequest)X
1 f
2973(event)X
3186(is)X
3267(generated,)X
3653(and)X
3802(no)X
555 5236(further)N
816(processing)X
1214(is)X
1295(performed.)X
555 5360(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XResizeWindow)X
1 f
2478(is)X
5 f
2577(BadWindow)X
1 f
3061(.)X
555 5580(Use)N
5 f
731(XMoveResizeWindow)X
1 f
1622(to)X
1713(change)X
1984(the)X
2114(size)X
2273(and)X
2422(location)X
2729(of)X
2824(a)X
2885(window.)X
3212(The)X
3371(de\256nition)X
3732(for)X
3856(this)X
555 5676(function)N
871(is:)X
3 f
2239 6144(31)N

32 p
%%Page: 32 36
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XMoveResizeWindow)N
1368(\()X
2 f
1404(display)X
1 f
1659(,)X
2 f
1703(w)X
1 f
1769(,)X
2 f
1813(x)X
1 f
1859(,)X
2 f
1903(y)X
1 f
1949(,)X
2 f
1993(width)X
1 f
2197(,)X
2 f
2241(height)X
1 f
2469(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 960(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 1056(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
2 f
555 1208(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1332(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(to)X
2850(be)X
2955(recon\256gured.)X
2 f
555 1456(x)N
555 1552(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(new)X
3500(position)X
3807(of)X
3902(the)X
1131 1648(window)N
1436(relative)X
1723(to)X
1814(its)X
1920(parent.)X
2 f
555 1772(width)N
555 1868(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(arguments)X
2832(de\256ne)X
3069(the)X
3199(interior)X
3481(size)X
3640(of)X
3735(the)X
1131 1964(window.)N
555 2088(The)N
5 f
732(XMoveResizeWindow)X
1 f
1623(function)X
1939(changes)X
2244(the)X
2374(size)X
2533(and)X
2682(location)X
2989(of)X
3084(the)X
3214(speci\256ed)X
3549(window)X
555 2184(without)N
847(raising)X
1109(it.)X
1225(Moving)X
1526(and)X
1675(resizing)X
1976(a)X
2037(mapped)X
2338(window)X
2643(may)X
2817(generate)X
3137(an)X
5 f
3260(Expose)X
1 f
3591(event)X
3804(on)X
555 2280(the)N
685(window.)X
1034(Depending)X
1442(on)X
1552(the)X
1682(new)X
1850(size)X
2009(and)X
2158(location)X
2465(parameters,)X
2895(moving)X
3187(and)X
3336(resizing)X
3637(a)X
3698(window)X
555 2376(may)N
729(generate)X
1049(exposure)X
1388(events)X
1635(on)X
1745(windows)X
2084(that)X
2239(the)X
2369(window)X
2674(formerly)X
3004(obscured.)X
555 2500(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2591(and)X
2740(some)X
2948(other)X
3151(client)X
3370(has)X
3509(selected)X
5 f
570 2596(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(then)X
2322(a)X
5 f
2401(Con\256gureRequest)X
1 f
3147(event)X
3360(is)X
3441(generated,)X
3827(and)X
555 2692(no)N
665(further)X
926(processing)X
1324(is)X
1405(performed.)X
1815(Otherwise,)X
2220(the)X
2350(window)X
2655(size)X
2814(and)X
2963(location)X
3270(is)X
3351(changed.)X
555 2816(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XMoveResizeWindow)X
1 f
2727(are)X
5 f
2874(BadWindow)X
1 f
3358(,)X
5 f
3420(BadMatch)X
1 f
3832(,)X
3876(and)X
5 f
570 2912(BadValue)N
1 f
967(.)X
555 3132(Use)N
5 f
731(XSetWindowBorderWidth)X
1 f
1761(to)X
1852(change)X
2123(the)X
2253(border)X
2504(width)X
2727(of)X
2822(a)X
2883(window.)X
3232(The)X
3391(de\256nition)X
3752(for)X
3876(this)X
555 3228(function)N
871(is:)X
555 3352(XSetWindowBorderWidth)N
1506(\()X
2 f
1542(display)X
1 f
1797(,)X
2 f
1841(w)X
1 f
1900(,)X
2 f
1944(width)X
1 f
2148(\))X
687 3448(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3544(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3640(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(;)X
2 f
555 3792(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3916(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 4040(width)N
1 f
1131(Speci\256es)X
1471(the)X
1601(width)X
1824(of)X
1919(the)X
2049(window)X
2354(border.)X
555 4164(The)N
5 f
732(XSetWindowBorderWidth)X
1 f
1762(function)X
2078(sets)X
2232(the)X
2362(speci\256ed)X
2697(window's)X
3065(border)X
3316(width)X
3539(to)X
3630(the)X
555 4260(speci\256ed)N
890(width.)X
555 4384(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowBorderWidth)X
1 f
2866(are)X
5 f
3013(BadWindow)X
1 f
3519(and)X
5 f
3686(Bad-)X
555 4480(Value)N
1 f
795(.)X
3 f
555 4672(3.8.)N
731(Changing)X
1120(Window)X
1460(Stacking)X
1810(Order)X
1 f
555 4796(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(raise,)X
2333(lower,)X
2577(circulate,)X
2925(or)X
3020(restack)X
3291(windows.)X
555 5016(Use)N
5 f
731(XRaiseWindow)X
1 f
1363(to)X
1454(raise)X
1642(a)X
1703(window)X
2008(so)X
2108(that)X
2263(no)X
2373(sibling)X
2636(window)X
2941(obscures)X
3270(it.)X
3386(The)X
3545(de\256nition)X
3906(for)X
555 5112(this)N
705(function)X
1021(is:)X
555 5236(XRaiseWindow)N
1124(\()X
2 f
1160(display)X
1 f
1415(,)X
2 f
1459(w)X
1 f
1525(\))X
687 5332(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5428(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 5580(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5704(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
3 f
2239 6144(32)N

33 p
%%Page: 33 37
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
5 f
732(XRaiseWindow)X
1 f
1364(function)X
1680(raises)X
1902(the)X
2032(speci\256ed)X
2367(window)X
2672(to)X
2763(the)X
2893(top)X
3028(of)X
3123(the)X
3253(stack)X
3456(so)X
3556(that)X
3711(no)X
555 768(sibling)N
818(window)X
1123(obscures)X
1452(it.)X
1568(If)X
1648(the)X
1778(windows)X
2117(are)X
2246(regarded)X
2575(as)X
2670(overlapping)X
3113(sheets)X
3350(of)X
3445(paper)X
3662(stacked)X
555 864(on)N
665(a)X
726(desk,)X
931(then)X
1105(raising)X
1367(a)X
1428(window)X
1733(is)X
1814(analogous)X
2193(to)X
2284(moving)X
2576(the)X
2706(sheet)X
2909(to)X
3000(the)X
3130(top)X
3265(of)X
3360(the)X
3490(stack)X
3693(but)X
3828(leav-)X
555 960(ing)N
690(its)X
796(x)X
862(and)X
1011(y)X
1077(location)X
1384(on)X
1494(the)X
1624(desk)X
1807(constant.)X
2167(Raising)X
2459(a)X
2520(mapped)X
2821(window)X
3126(may)X
3300(generate)X
3620(exposure)X
555 1056(events)N
802(for)X
926(the)X
1056(window)X
1361(and)X
1510(any)X
1659(mapped)X
1960(subwindows)X
2421(that)X
2576(were)X
2768(formerly)X
3098(obscured.)X
555 1180(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2591(and)X
2740(some)X
2948(other)X
3151(client)X
3370(has)X
3509(selected)X
5 f
570 1276(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(a)X
5 f
2227(Con\256gureRequest)X
1 f
2973(event)X
3186(is)X
3267(generated,)X
3653(and)X
3802(no)X
555 1372(processing)N
953(is)X
1034(performed.)X
1466(Otherwise,)X
1871(the)X
2001(window)X
2306(is)X
2387(raised.)X
555 1496(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XRaiseWindow)X
1 f
2434(is)X
5 f
2533(BadWindow)X
1 f
3017(.)X
555 1716(Use)N
5 f
731(XLowerWindow)X
1 f
1378(to)X
1469(lower)X
1691(a)X
1752(window)X
2057(so)X
2157(that)X
2312(it)X
2384(does)X
2567(not)X
2702(obscure)X
2997(any)X
3146(sibling)X
3409(windows.)X
3770(The)X
555 1812(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 1936(XLowerWindow)N
1157(\()X
2 f
1193(display)X
1 f
1448(,)X
2 f
1492(w)X
1 f
1558(\))X
687 2032(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2128(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 2280(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2404(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 2528(The)N
5 f
732(XLowerWindow)X
1 f
1379(function)X
1695(lowers)X
1951(the)X
2081(speci\256ed)X
2416(window)X
2721(to)X
2812(the)X
2942(bottom)X
3215(of)X
3310(the)X
3440(stack)X
3643(so)X
3743(that)X
3898(it)X
555 2624(does)N
738(not)X
873(obscure)X
1168(any)X
1317(sibling)X
1580(windows.)X
1963(If)X
2043(the)X
2173(windows)X
2512(are)X
2641(regarded)X
2970(as)X
3065(overlapping)X
3508(sheets)X
3745(of)X
555 2720(paper)N
772(stacked)X
1058(on)X
1168(a)X
1229(desk,)X
1434(then)X
1608(lowering)X
1943(a)X
2004(window)X
2309(is)X
2390(analogous)X
2769(to)X
2860(moving)X
3152(the)X
3282(sheet)X
3485(to)X
3576(the)X
3706(bottom)X
555 2816(of)N
650(the)X
780(stack)X
983(but)X
1118(leaving)X
1400(its)X
1506(x)X
1572(and)X
1721(y)X
1787(location)X
2094(on)X
2204(the)X
2334(desk)X
2517(constant.)X
2877(Lowering)X
3241(a)X
3302(mapped)X
3603(window)X
555 2912(will)N
715(generate)X
1035(exposure)X
1374(events)X
1621(on)X
1731(any)X
1880(windows)X
2219(it)X
2291(formerly)X
2621(obscured.)X
555 3036(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(the)X
1935(window)X
2240(is)X
5 f
2339(False)X
1 f
2591(and)X
2740(some)X
2948(other)X
3151(client)X
3370(has)X
3509(selected)X
5 f
570 3132(SubstructureRedirectMask)N
1 f
1644(on)X
1754(the)X
1884(parent,)X
2148(a)X
5 f
2227(Con\256gureRequest)X
1 f
2973(event)X
3186(is)X
3267(generated,)X
3653(and)X
3802(no)X
555 3228(processing)N
953(is)X
1034(performed.)X
1444(Otherwise,)X
1849(the)X
1979(window)X
2284(is)X
2365(lowered)X
2670(to)X
2761(the)X
2891(bottom)X
3164(of)X
3259(the)X
3389(stack.)X
555 3352(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XLowerWindow)X
1 f
2449(is)X
5 f
2548(BadWindow)X
1 f
3032(.)X
555 3572(Use)N
5 f
731(XCirculateSubwindows)X
1 f
1666(to)X
1757(circulate)X
2083(a)X
2144(subwindow)X
2571(up)X
2681(or)X
2776(down.)X
3037(The)X
3196(de\256nition)X
3557(for)X
3681(this)X
555 3668(function)N
871(is:)X
555 3792(XCirculateSubwindows)N
1403(\()X
2 f
1439(display)X
1 f
1694(,)X
2 f
1738(w)X
1 f
1804(,)X
2 f
1848(direction)X
1 f
2174(\))X
687 3888(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3984(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 4080(int)N
2 f
803(direction)X
1 f
1129(;)X
2 f
555 4232(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4356(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 4480(direction)N
1 f
1131(Speci\256es)X
1471(the)X
1601(direction)X
1937(\(up)X
2076(or)X
2171(down\))X
2417(that)X
2572(you)X
2726(want)X
2919(to)X
3010(circulate)X
3336(the)X
3466(window.)X
3793(You)X
1131 4576(can)N
1275(pass)X
1448(one)X
1597(of)X
1692(these)X
1895(constants:)X
5 f
2288(RaiseLowest)X
1 f
2828(or)X
5 f
2941(LowerHighest)X
1 f
3493(.)X
555 4700(The)N
5 f
732(XCirculateSubwindows)X
1 f
1667(function)X
1983(circulates)X
2343(the)X
2473(speci\256ed)X
2808(subwindow)X
3235(in)X
3326(the)X
3456(speci\256ed)X
3791(direc-)X
555 4796(tion:)N
5 f
758(RaiseLowest)X
1 f
1298(or)X
5 f
1411(LowerHighest)X
1 f
1963(.)X
2029(If)X
2109(some)X
2317(other)X
2520(client)X
2739(has)X
2878(selected)X
5 f
3202(Substruc-)X
555 4892(tureRedirectMask)N
1 f
1281(on)X
1391(the)X
1521(window,)X
1848(a)X
5 f
1927(CirculateRequest)X
1 f
2638(event)X
2851(is)X
2932(generated,)X
3318(and)X
3467(no)X
3577(further)X
3838(pro-)X
555 4988(cessing)N
836(is)X
917(performed.)X
1349(Otherwise,)X
1754(the)X
1884(processing)X
2282(described)X
2641(in)X
2732(the)X
2862(following)X
3227(paragraph)X
3600(is)X
3681(per-)X
555 5084(formed,)N
853(and)X
1002(if)X
1078(the)X
1208(window)X
1513(is)X
1594(actually)X
1896(restacked,)X
2272(the)X
2402(X)X
2487(server)X
2723(generates)X
3077(a)X
5 f
3156(CirculateNotify)X
1 f
3764(event.)X
555 5208(If)N
635(you)X
789(specify)X
5 f
1083(RaiseLowest)X
1 f
1601(,)X
1645(this)X
1795(function)X
2111(raises)X
2333(the)X
2463(lowest)X
2715(mapped)X
3016(child)X
3215(\(if)X
3320(any\))X
3498(that)X
3653(is)X
555 5304(occluded)N
895(by)X
1005(another)X
1291(child)X
1490(to)X
1581(the)X
1711(top)X
1846(of)X
1941(the)X
2071(stack.)X
2318(If)X
2398(you)X
2552(specify)X
5 f
2846(LowerHighest)X
1 f
3398(,)X
3442(this)X
3592(function)X
555 5400(lowers)N
811(the)X
941(highest)X
1218(mapped)X
1519(child)X
1718(\(if)X
1823(any\))X
2001(that)X
2156(occludes)X
2486(another)X
2772(child)X
2971(to)X
3062(the)X
3192(bottom)X
3465(of)X
3560(the)X
3690(stack.)X
555 5496(Exposure)N
909(processing)X
1307(is)X
1388(performed)X
1776(on)X
1886(formerly)X
2216(obscured)X
2555(windows.)X
555 5620(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCirculateSubwindows)X
1 f
2771(are)X
5 f
2918(BadWindow)X
1 f
3424(and)X
5 f
3591(BadValue)X
1 f
3988(.)X
555 5840(Use)N
5 f
731(XCirculateSubwindowsUp)X
1 f
1778(to)X
1869(raise)X
2057(the)X
2187(lowest)X
2439(mapped)X
2740(child)X
2939(of)X
3034(an)X
3139(occluded)X
3479(window.)X
3806(The)X
3 f
2239 6144(33)N

34 p
%%Page: 34 38
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 796(XCirculateSubwindowsUp)N
1510(\()X
2 f
1546(display)X
1 f
1801(,)X
2 f
1845(w)X
1 f
1911(\))X
687 892(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 988(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 1140(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1264(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 1388(The)N
5 f
732(XCirculateSubwindowsUp)X
1 f
1779(function)X
2095(raises)X
2317(the)X
2447(lowest)X
2699(mapped)X
3000(child)X
3199(of)X
3294(the)X
3424(speci\256ed)X
3759(win-)X
555 1484(dow)N
728(that)X
883(is)X
964(partially)X
1281(or)X
1376(completely)X
1791(occluded)X
2131(by)X
2241(another)X
2527(child.)X
2770(Completely)X
3205(unobscured)X
3632(children)X
555 1580(are)N
684(not)X
819(affected.)X
1168(This)X
1347(is)X
1428(a)X
1489(convenience)X
1951(routine)X
2223(equivalent)X
2613(to)X
5 f
2722(XCirculateWindow)X
1 f
3476(\(display,)X
3804(win-)X
555 1676(dow,)N
5 f
768(RaiseLowest)X
1 f
1286(\).)X
555 1800(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XCirculateSubwindowsUp)X
1 f
2849(is)X
5 f
2948(BadWindow)X
1 f
3432(.)X
555 2020(Use)N
5 f
731(XCirculateSubwindowsDown)X
1 f
1890(to)X
1981(lower)X
2203(the)X
2333(highest)X
2610(mapped)X
2911(child)X
3110(of)X
3205(a)X
3266(window)X
3571(that)X
3726(par-)X
555 2116(tially)N
760(or)X
855(completely)X
1270(occludes)X
1600(another)X
1886(child.)X
2107(The)X
2266(de\256nition)X
2627(for)X
2751(this)X
2901(function)X
3217(is:)X
555 2240(XCirculateSubwindowsDown)N
1617(\()X
2 f
1653(display)X
1 f
1908(,)X
2 f
1952(w)X
1 f
2018(\))X
687 2336(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2432(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 2584(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2708(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 2832(The)N
5 f
732(XCirculateSubwindowsDown)X
1 f
1891(function)X
2207(lowers)X
2463(the)X
2593(highest)X
2870(mapped)X
3171(child)X
3370(of)X
3465(the)X
3595(speci\256ed)X
555 2928(window)N
860(that)X
1015(partially)X
1332(or)X
1427(completely)X
1842(occludes)X
2172(another)X
2458(child.)X
2701(Completely)X
3136(unobscured)X
3563(children)X
3874(are)X
555 3024(not)N
690(affected.)X
1039(This)X
1218(is)X
1299(a)X
1360(convenience)X
1822(routine)X
2094(equivalent)X
2484(to)X
5 f
2593(XCirculateWindow)X
1 f
3347(\(display,)X
3675(window,)X
5 f
570 3120(LowerHighest)N
1 f
1122(\).)X
555 3244(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XCirculateSubwindowsDown)X
1 f
2961(is)X
5 f
3060(BadWindow)X
1 f
3544(.)X
555 3464(Use)N
5 f
731(XRestackWindows)X
1 f
1501(to)X
1592(restack)X
1863(a)X
1924(set)X
2044(of)X
2139(windows)X
2478(from)X
2671(top)X
2806(to)X
2897(bottom.)X
3192(The)X
3351(de\256nition)X
3712(for)X
3836(this)X
555 3560(function)N
871(is:)X
555 3684(XRestackWindows)N
1241(\()X
2 f
1277(display)X
1 f
1532(,)X
2 f
1576(windows)X
1 f
1892(,)X
2 f
1943(nwindows)X
1 f
2303(\);)X
687 3780(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3876(Window)N
2 f
1012(windows)X
1 f
1328([];)X
687 3972(int)N
2 f
803(nwindows)X
1 f
1163(;)X
2 f
555 4124(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4248(windows)N
1 f
1131(Speci\256es)X
1471(an)X
1576(array)X
1778(containing)X
2173(the)X
2303(windows)X
2642(to)X
2733(be)X
2838(restacked.)X
3236(All)X
3371(the)X
3501(windows)X
1131 4344(must)N
1325(have)X
1513(the)X
1643(same)X
1846(parent.)X
2 f
555 4468(nwindows)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(windows)X
2326(to)X
2417(be)X
2522(restacked.)X
555 4592(The)N
5 f
732(XRestackWindows)X
1 f
1502(function)X
1818(restacks)X
2123(the)X
2253(windows)X
2592(in)X
2683(the)X
2813(order)X
3020(speci\256ed,)X
3377(from)X
3570(top)X
3705(to)X
3796(bot-)X
555 4688(tom.)N
759(The)X
918(stacking)X
1234(order)X
1441(of)X
1536(the)X
1666(\256rst)X
1825(window)X
2130(in)X
2221(the)X
2351(windows)X
2690(array)X
2892(will)X
3052(be)X
3157(unaffected,)X
3572(but)X
3707(the)X
555 4784(other)N
758(windows)X
1097(in)X
1188(the)X
1318(array)X
1520(will)X
1680(be)X
1785(stacked)X
2071(underneath)X
2484(the)X
2614(\256rst)X
2773(window)X
3078(in)X
3169(the)X
3299(order)X
3506(of)X
3601(the)X
3731(array.)X
555 4880(The)N
714(stacking)X
1030(order)X
1237(of)X
1332(the)X
1462(other)X
1665(windows)X
2004(is)X
2085(not)X
2220(affected.)X
555 5004(If)N
635(the)X
765(override_redirect)X
1393(attribute)X
1710(of)X
1805(a)X
1866(window)X
2171(is)X
5 f
2270(False)X
1 f
2522(and)X
2671(some)X
2879(other)X
3082(client)X
3301(has)X
3440(selected)X
5 f
3764(Sub-)X
555 5100(structureRedirectMask)N
1 f
1472(on)X
1582(the)X
1712(parent,)X
5 f
1994(Con\256gureRequest)X
1 f
2740(events)X
2987(are)X
3116(generated)X
3480(for)X
3604(each)X
3787(win-)X
555 5196(dow)N
728(whose)X
974(override_redirect)X
1602(is)X
1683(not)X
1818(set,)X
1960(and)X
2109(no)X
2219(further)X
2480(processing)X
2878(is)X
2959(performed.)X
3391(Otherwise,)X
3796(the)X
555 5292(windows)N
894(will)X
1054(be)X
1159(restacked)X
1513(in)X
1604(top)X
1739(to)X
1830(bottom)X
2103(order.)X
555 5416(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XRestackWindows)X
1 f
2572(is)X
5 f
2671(BadWindow)X
1 f
3155(.)X
3 f
2239 6144(34)N

35 p
%%Page: 35 39
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(3.9.)N
731(Changing)X
1120(Window)X
1460(Attributes)X
1 f
555 796(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set)X
2243(window)X
2548(attributes.)X
5 f
2964(XChangeWindowAttri-)X
555 892(butes)N
1 f
808(is)X
889(the)X
1019(more)X
1222(general)X
1503(function)X
1819(that)X
1974(allows)X
2226(you)X
2380(to)X
2471(set)X
2591(one)X
2740(or)X
2835(more)X
3038(window)X
3343(attributes)X
3694(provided)X
555 988(by)N
665(the)X
5 f
813(XSetWindowAttributes)X
1 f
1727(structure.)X
2101(See)X
2250(Section)X
2537(3.2)X
2669(for)X
2793(descriptions)X
3241(of)X
3336(these)X
3539(window)X
555 1084(attributes.)N
950(The)X
1109(other)X
1312(functions)X
1662(described)X
2021(in)X
2112(this)X
2262(section)X
2534(allow)X
2752(you)X
2906(to)X
2997(set)X
3117(one)X
3266(speci\256c)X
3557(window)X
555 1180(attribute,)N
894(such)X
1077(as)X
1172(a)X
1233(window's)X
1601(background.)X
555 1304(Use)N
5 f
731(XChangeWindowAttributes)X
1 f
1820(to)X
1911(change)X
2182(one)X
2331(or)X
2426(more)X
2629(window)X
2934(attributes.)X
3329(The)X
3488(de\256nition)X
3849(for)X
555 1400(this)N
705(function)X
1021(is:)X
555 1524(XChangeWindowAttributes)N
1550(\()X
2 f
1586(display)X
1 f
1841(,)X
2 f
1885(w)X
1 f
1951(,)X
2 f
1995(valuemask)X
1 f
2373(,)X
2 f
2417(attributes)X
1 f
2763(\))X
687 1620(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1716(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 1812(unsigned)N
1027(long)X
2 f
1206(valuemask)X
1 f
1584(;)X
687 1908(XSetWindowAttributes)N
1541(*)X
2 f
(attributes)S
1 f
1931(;)X
2 f
555 2060(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2184(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 2308(valuemask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(window)X
2013(attributes)X
2364(are)X
2493(de\256ned)X
2774(in)X
2865(the)X
2995(attributes)X
3346(argument.)X
3745(This)X
1131 2404(mask)N
1339(is)X
1420(the)X
1550(bitwise)X
1827(inclusive)X
2168(OR)X
2312(of)X
2407(the)X
2537(valid)X
2736(attribute)X
3053(mask)X
3261(bits.)X
3455(If)X
3535(valuemask)X
3934(is)X
1131 2500(zero)N
1304(\(0\),)X
1450(the)X
1580(rest)X
1729(is)X
1810(ignored,)X
2123(and)X
2272(the)X
2402(attributes)X
2753(are)X
2882(not)X
3017(referenced.)X
3453(The)X
3612(values)X
3859(and)X
1131 2596(restrictions)N
1545(are)X
1674(the)X
1804(same)X
2007(as)X
2102(for)X
5 f
2244(XCreateSimpleWindow)X
1 f
3184(and)X
5 f
3351(XCreateWin-)X
1131 2692(dow)N
1 f
1307(.)X
2 f
555 2844(attributes)N
1 f
1131(Attributes)X
1506(of)X
1601(the)X
1731(window)X
2036(to)X
2127(be)X
2232(set)X
2352(at)X
2438(creation)X
2744(time)X
2924(should)X
3181(be)X
3286(set)X
3406(in)X
3497(this)X
3647(structure.)X
1131 2940(The)N
1290(valuemask)X
1689(should)X
1946(have)X
2134(the)X
2264(appropriate)X
2687(bits)X
2837(set)X
2957(to)X
3048(indicate)X
3350(which)X
3587(attributes)X
1131 3036(have)N
1319(been)X
1507(set)X
1627(in)X
1718(the)X
1848(structure.)X
555 3160(Depending)N
963(on)X
1073(the)X
1203(valuemask,)X
1624(the)X
5 f
1772(XChangeWindowAttributes)X
1 f
2861(function)X
3177(uses)X
3350(the)X
3480(window)X
3785(attri-)X
555 3256(butes)N
763(in)X
854(the)X
5 f
1002(XSetWindowAttributes)X
1 f
1916(structure)X
2246(to)X
2337(change)X
2608(the)X
2738(speci\256ed)X
3073(window)X
3378(attributes.)X
555 3352(Changing)N
920(the)X
1050(background)X
1487(does)X
1670(not)X
1805(cause)X
2022(the)X
2152(window)X
2457(contents)X
2773(to)X
2864(be)X
2969(changed.)X
3328(Use)X
5 f
3504(XClearWin-)X
555 3448(dow)N
1 f
753(to)X
844(repaint)X
1111(the)X
1241(window)X
1546(and)X
1695(its)X
1801(background.)X
2282(\(See)X
2460(Section)X
2747(6.2)X
2879(for)X
3003(further)X
3264(information.\))X
3754(Setting)X
555 3544(the)N
685(border)X
936(or)X
1031(changing)X
1376(the)X
1506(background)X
1943(such)X
2126(that)X
2281(the)X
2411(border)X
2662(tile)X
2798(origin)X
3031(changes)X
3336(causes)X
3587(the)X
3717(border)X
555 3640(to)N
646(be)X
751(repainted.)X
1145(Changing)X
1510(the)X
1640(background)X
2077(of)X
2172(a)X
2233(root)X
2397(window)X
2702(to)X
5 f
2811(None)X
1 f
3058(or)X
5 f
3171(ParentRelative)X
1 f
555 3736(restores)N
850(the)X
980(default)X
1247(background)X
1684(pixmap.)X
2015(Changing)X
2380(the)X
2510(border)X
2761(of)X
2856(a)X
2917(root)X
3081(window)X
3386(to)X
5 f
3495(CopyFrom-)X
555 3832(Parent)N
1 f
852(restores)X
1147(the)X
1277(default)X
1544(border)X
1795(pixmap.)X
2126(Changing)X
2491(the)X
2621(win_gravity)X
3069(does)X
3252(not)X
3387(affect)X
3609(the)X
3739(current)X
555 3928(position)N
862(of)X
957(the)X
1087(window.)X
1436(Either)X
1674(changing)X
2019(the)X
2149(backing_store)X
2665(of)X
2760(an)X
2865(obscured)X
3204(window)X
3509(to)X
5 f
3618(When-)X
555 4024(Mapped)N
1 f
910(or)X
5 f
1023(Always)X
1 f
1316(,)X
1360(or)X
1455(changing)X
1800(the)X
1930(backing_planes,)X
2522(backing_pixel,)X
3066(or)X
3161(save_under)X
3583(of)X
3678(a)X
555 4120(mapped)N
856(window)X
1161(may)X
1335(have)X
1523(no)X
1633(immediate)X
2029(effect.)X
555 4244(Multiple)N
882(clients)X
1135(can)X
1279(select)X
1502(input)X
1706(on)X
1816(the)X
1946(same)X
2149(window.)X
2476(If)X
2556(this)X
2706(is)X
2787(the)X
2917(case,)X
3112(their)X
3296(event)X
3509(masks)X
3751(are)X
555 4340(maintained)N
970(separately.)X
1393(When)X
1625(an)X
1730(event)X
1943(is)X
2024(generated,)X
2410(it)X
2482(will)X
2642(be)X
2747(reported)X
3062(to)X
3153(all)X
3264(interested)X
3629(clients.)X
555 4436(However,)N
920(at)X
1006(most,)X
1222(one)X
1371(client)X
1590(at)X
1676(a)X
1737(time)X
1917(can)X
2061(select)X
2284(for)X
5 f
2426(SubstructureRedirectMask)X
1 f
3478(,)X
5 f
3540(Resiz-)X
555 4532(eRedirectMask)N
1 f
1156(,)X
1200(and)X
5 f
1367(ButtonPressMask)X
1 f
2073(.)X
2139(If)X
2219(a)X
2280(client)X
2499(attempts)X
2821(to)X
2912(select)X
3135(any)X
3284(of)X
3379(these)X
3582(event)X
555 4628(masks)N
797(and)X
946(some)X
1154(other)X
1357(client)X
1576(has)X
1715(already)X
1996(selected)X
2302(it,)X
2396(the)X
2526(X)X
2611(server)X
2847(generates)X
3201(a)X
5 f
3280(BadAccess)X
1 f
3758(error.)X
555 4724(There)N
782(is)X
863(only)X
1042(one)X
1191(do_not_propagate_mask)X
2079(for)X
2203(a)X
2264(window,)X
2591(not)X
2726(one)X
2875(per)X
3009(client.)X
555 4848(Changing)N
920(the)X
1050(color)X
1253(map)X
1427(of)X
1522(a)X
1583(window)X
1888(\(that)X
2072(is,)X
2175(de\256ning)X
2486(a)X
2547(new)X
2715(map,)X
2911(not)X
3046(changing)X
3391(the)X
3521(contents)X
3837(of)X
555 4944(the)N
685(existing)X
987(map\))X
1190(generates)X
1544(a)X
5 f
1623(ColormapNotify)X
1 f
2265(event.)X
2500(Changing)X
2865(the)X
2995(color)X
3198(map)X
3372(of)X
3467(a)X
3528(visible)X
3786(win-)X
555 5040(dow)N
728(may)X
902(have)X
1090(no)X
1200(immediate)X
1596(effect)X
1818(on)X
1928(the)X
2058(screen)X
2304(because)X
2604(the)X
2734(map)X
2908(may)X
3082(not)X
3217(be)X
3322(installed.)X
3688(See)X
5 f
570 5136(XInstallColormap)N
1 f
1276(in)X
1367(Chapter)X
1668(7.)X
1778(Changing)X
2143(the)X
2273(cursor)X
2514(of)X
2609(a)X
2670(root)X
2834(window)X
3139(to)X
5 f
3248(None)X
1 f
3495(restores)X
3790(the)X
555 5232(default)N
822(cursor.)X
1107(Whenever)X
1490(possible,)X
1823(you)X
1977(are)X
2106(encouraged)X
2533(to)X
2624(share)X
2831(color)X
3034(maps.)X
555 5356(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XChangeWindowAttributes)X
1 f
3341(are)X
5 f
3488(BadWindow)X
1 f
3972(,)X
5 f
570 5452(BadPixmap)N
1 f
1035(,)X
5 f
1097(BadColor)X
1 f
1478(,)X
5 f
1540(BadCursor)X
1 f
1975(,)X
5 f
2037(BadMatch)X
1 f
2449(,)X
5 f
2511(BadAccess)X
1 f
2967(,)X
3011(and)X
5 f
3178(BadValue)X
1 f
3575(.)X
555 5672(Use)N
5 f
731(XSetWindowBackground)X
1 f
1742(to)X
1833(set)X
1953(the)X
2083(background)X
2520(of)X
2615(a)X
2676(speci\256ed)X
3011(window)X
3316(to)X
3407(the)X
3537(speci\256ed)X
555 5768(pixel.)N
776(The)X
935(de\256nition)X
1296(for)X
1420(this)X
1570(function)X
1886(is:)X
3 f
2239 6144(35)N

36 p
%%Page: 36 40
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XSetWindowBackground)N
1471(\()X
2 f
1507(display)X
1 f
1762(,)X
2 f
1806(w)X
1 f
1872(,)X
2 f
1916(background_pixel)X
1 f
2559(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 960(unsigned)N
1027(long)X
2 f
1206(background_pixel)X
1 f
1849(;)X
2 f
555 1112(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1236(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 1360(background_pixel)N
1 f
1131 1456(Speci\256es)N
1471(the)X
1601(pixel)X
1800(of)X
1895(the)X
2025(background.)X
2506(This)X
2685(pixel)X
2884(value)X
3097(determines)X
3506(which)X
3743(entry)X
1131 1552(in)N
1222(the)X
1352(colormap)X
1707(is)X
1788(used.)X
555 1676(The)N
5 f
732(XSetWindowBackground)X
1 f
1743(function)X
2059(sets)X
2213(the)X
2343(background)X
2780(pixel)X
2979(of)X
3074(the)X
3204(window)X
3509(to)X
3600(the)X
3730(pixel)X
555 1772(value)N
768(you)X
922(specify.)X
1242(Changing)X
1607(the)X
1737(background)X
2174(does)X
2357(not)X
2492(cause)X
2709(the)X
2839(window)X
3144(contents)X
3460(to)X
3551(be)X
3656(changed.)X
5 f
570 1868(XSetWindowBackground)N
1 f
1581(uses)X
1754(a)X
1815(pixmap)X
2102(of)X
2197(unde\256ned)X
2566(size)X
2725(\256lled)X
2929(with)X
3108(the)X
3238(color)X
3441(associated)X
3825(with)X
555 1964(the)N
685(pixel)X
884(value)X
1097(you)X
1251(passed)X
1507(to)X
1598(the)X
1728(background_pixel)X
2386(argument.)X
2785(This)X
2964(can)X
3108(not)X
3243(be)X
3348(performed)X
3736(on)X
3846(an)X
5 f
570 2060(InputOnly)N
1 f
985(window.)X
1334(An)X
1463(error)X
1655(will)X
1815(result)X
2033(if)X
2109(you)X
2263(try)X
2383(to)X
2474(change)X
2745(the)X
2875(background)X
3312(of)X
3407(an)X
5 f
3530(InputOnly)X
1 f
555 2156(window.)N
555 2280(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowBackground)X
1 f
2847(are)X
5 f
2994(BadWindow)X
1 f
3500(and)X
5 f
3667(Bad-)X
555 2376(Match)N
1 f
810(.)X
555 2624(Use)N
5 f
731(XSetWindowBackgroundPixmap)X
1 f
2035(to)X
2126(set)X
2246(the)X
2376(background)X
2813(of)X
2908(a)X
2969(speci\256ed)X
3304(window)X
3609(to)X
3700(the)X
555 2720(speci\256ed)N
890(pixmap.)X
1221(The)X
1380(de\256nition)X
1741(for)X
5 f
1883(XSetWindowBackgroundPixmap)X
1 f
3187(is:)X
555 2872(XSetWindowBackgroundPixmap)N
1741(\()X
2 f
1777(display)X
1 f
2032(,)X
2 f
2076(w)X
1 f
2142(,)X
2 f
2186(background_pixmap)X
1 f
2916(\))X
687 2968(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3064(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3160(Pixmap)N
2 f
979(background_pixmap)X
1 f
1709(;)X
2 f
555 3312(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3436(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 3560(background_pixmap)N
1 f
1131 3656(Speci\256es)N
1471(the)X
1601(background)X
2038(pixmap.)X
2369(If)X
2449(a)X
2510(Pixmap)X
2802(ID)X
2916(is)X
2997(speci\256ed,)X
3354(the)X
3484(background)X
3921(is)X
1131 3752(painted)N
1413(with)X
1592(this)X
1742(pixmap.)X
2073(If)X
5 f
2171(None)X
1 f
2396(,)X
2440(no)X
2550(background)X
2987(is)X
3068(painted.)X
3394(If)X
5 f
3492(ParentRela-)X
1131 3848(tive)N
1 f
1283(,)X
1327(the)X
1457(parent's)X
1762(pixmap)X
2049(is)X
2130(used.)X
555 3972(The)N
5 f
732(XSetWindowBackgroundPixmap)X
1 f
2036(function)X
2352(sets)X
2506(the)X
2636(background)X
3073(pixmap)X
3360(of)X
3455(the)X
3585(window)X
3890(to)X
555 4068(the)N
685(pixmap)X
972(you)X
1126(specify.)X
1446(If)X
1526(no)X
1636(background)X
2073(Pixmap)X
2365(is)X
2446(speci\256ed,)X
2803(the)X
2933(background)X
3370(pixmap)X
3657(of)X
3752(the)X
555 4164(window's)N
923(parent)X
1165(is)X
1246(used.)X
1473(On)X
1602(the)X
1732(root)X
1896(window,)X
2223(the)X
2353(default)X
2620(background)X
3057(will)X
3217(be)X
3322(restored.)X
3671(The)X
555 4260(background)N
992(Pixmap)X
1284(can)X
1428 0.3187(immediately)AX
1893(be)X
1998(freed)X
2200(if)X
2276(no)X
2386(further)X
2647(explicit)X
2935(references)X
3317(to)X
3408(it)X
3480(are)X
3609(to)X
3700(be)X
555 4356(made.)N
812(This)X
991(can)X
1135(not)X
1270(be)X
1375(performed)X
1763(on)X
1873(an)X
5 f
1996(InputOnly)X
1 f
2411(window.)X
2760(An)X
2889(error)X
3081(will)X
3241(result)X
3459(if)X
3535(you)X
3689(try)X
3809(to)X
555 4452(change)N
826(the)X
956(background)X
1393(of)X
1488(an)X
5 f
1611(InputOnly)X
1 f
2026(window.)X
555 4576(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowBackgroundPixmap)X
1 f
3140(are)X
5 f
3287(BadWindow)X
1 f
3771(,)X
5 f
570 4672(BadPixmap)N
1 f
1035(,)X
5 f
1097(BadColor)X
1 f
1478(,)X
1522(and)X
5 f
1689(BadMatch)X
1 f
2101(.)X
2197 4848(Note)N
5 f
790 4984(XSetWindowBackground)N
1 f
1801(and)X
5 f
1968(XSetWindowBackgroundPixmap)X
1 f
3272(do)X
3382(not)X
3517(change)X
775 5080(the)N
905(current)X
1176(contents)X
1492(of)X
1587(the)X
1717(window,)X
2044(and)X
2193(you)X
2347(may)X
2521(wish)X
2709(to)X
2800(clear)X
2993(and)X
3142(repaint)X
3409(the)X
3539(screen)X
775 5176(after)N
958(these)X
1161(functions)X
1511(because)X
1811(they)X
1985(will)X
2145(not)X
2280(repaint)X
2547(the)X
2677(background)X
3114(you)X
3268(just)X
3418(set.)X
555 5476(Use)N
5 f
731(XSetWindowBorder)X
1 f
1536(to)X
1627(change)X
1898(and)X
2047(repaint)X
2314(a)X
2375(window's)X
2743(border)X
2994(to)X
3085(the)X
3215(speci\256ed)X
3550(pixel.)X
3771(The)X
555 5572(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
3 f
2239 6144(36)N

37 p
%%Page: 37 41
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XSetWindowBorder)N
1285(\()X
2 f
1321(display)X
1 f
1576(,)X
2 f
1620(w)X
1 f
1686(,)X
2 f
1730(border_pixel)X
1 f
2192(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 960(unsigned)N
1027(long)X
2 f
1206(border_pixel)X
1 f
1668(;)X
2 f
555 1112(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1236(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 1360(border_pixel)N
1 f
1131(Speci\256es)X
1471(the)X
1601(entry)X
1804(in)X
1895(the)X
2025(color)X
2228(map.)X
555 1484(The)N
5 f
732(XSetWindowBorder)X
1 f
1537(function)X
1853(sets)X
2007(the)X
2137(border)X
2388(pixel)X
2587(of)X
2682(the)X
2812(window)X
3117(to)X
3208(the)X
3338(pixel)X
3537(value)X
3750(you)X
555 1580(specify.)N
875(It)X
951(uses)X
1124(this)X
1274(value)X
1487(as)X
1582(an)X
1687(entry)X
1890(into)X
2050(the)X
2180(color)X
2383(map)X
2557(to)X
2648(determine)X
3023(which)X
3260(color)X
3463(is)X
3544(to)X
3635(be)X
3740(used)X
3923(to)X
555 1676(paint)N
754(the)X
884(border.)X
1179(This)X
1358(can)X
1502(not)X
1637(be)X
1742(performed)X
2130(on)X
2240(an)X
5 f
2363(InputOnly)X
1 f
2778(window.)X
3127(It)X
3203(will)X
3363(cause)X
3580(an)X
3685(error)X
3877(to)X
555 1772(perform)N
860(this)X
1010(on)X
1120(an)X
5 f
1243(InputOnly)X
1 f
1658(window.)X
555 1896(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowBorder)X
1 f
2641(are)X
5 f
2788(BadWindow)X
1 f
3272(,)X
5 f
3334(BadPixmap)X
1 f
3799(,)X
5 f
570 1992(BadMatch)N
1 f
982(,)X
1026(and)X
5 f
1193(BadValue)X
1 f
1590(.)X
555 2212(Use)N
5 f
731(XSetWindowBorderPixmap)X
1 f
1829(to)X
1920(change)X
2191(and)X
2340(repaint)X
2607(a)X
2668(window's)X
3036(border)X
3287(tile.)X
3445(The)X
3604(de\256nition)X
555 2308(for)N
679(this)X
829(function)X
1145(is:)X
555 2432(XSetWindowBorderPixmap)N
1555(\()X
2 f
1591(display)X
1 f
1846(,)X
2 f
1890(w)X
1 f
1956(,)X
2 f
2000(border_pixmap)X
1 f
2549(\))X
687 2528(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2624(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 2720(Pixmap)N
2 f
979(border_pixmap)X
1 f
1528(;)X
2 f
555 2872(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2996(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 3120(border_pixmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(border)X
1852(pixmap.)X
2183(If)X
2263(you)X
2417(specify)X
2693(a)X
2754(pixmap)X
3041(ID,)X
3177(the)X
3307(associated)X
3691(pixmap)X
1131 3216(is)N
1212(used)X
1395(for)X
1519(the)X
1649(border.)X
1944(If)X
5 f
2042(CopyFromParent)X
1 f
2749(is)X
2830(speci\256ed,)X
3187(a)X
3248(copy)X
3441(of)X
3536(the)X
3666(parent)X
1131 3312(window's)N
1499(border)X
1750(pixmap)X
2037(is)X
2118(used.)X
555 3436(The)N
5 f
732(XSetWindowBorderPixmap)X
1 f
1830(function)X
2146(sets)X
2300(the)X
2430(border)X
2681(pixmap)X
2968(of)X
3063(the)X
3193(window)X
3498(to)X
3589(the)X
3719(pixmap)X
555 3532(you)N
709(specify.)X
1029(It)X
1105(uses)X
1278(this)X
1428(entry)X
1631(for)X
1755(the)X
1885(border.)X
2180(The)X
2339(border)X
2590(Pixmap)X
2882(can)X
3026(be)X
3131(freed)X
3333 0.3187(immediately)AX
3798(if)X
3874(no)X
555 3628(further)N
816(explicit)X
1104(references)X
1486(to)X
1577(it)X
1649(are)X
1778(to)X
1869(be)X
1974(made.)X
2231(This)X
2410(can)X
2554(not)X
2689(be)X
2794(performed)X
3182(on)X
3292(an)X
5 f
3415(InputOnly)X
1 f
3830(win-)X
555 3724(dow.)N
772(It)X
848(will)X
1008(cause)X
1225(an)X
1330(error)X
1522(to)X
1613(perform)X
1918(this)X
2068(on)X
2178(an)X
5 f
2301(InputOnly)X
1 f
2716(window.)X
555 3848(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowBorderPixmap)X
1 f
2934(are)X
5 f
3081(BadWindow)X
1 f
3565(,)X
5 f
3627(BadPix-)X
555 3944(map)N
1 f
741(,)X
5 f
803(BadMatch)X
1 f
1215(,)X
1259(and)X
5 f
1426(BadValue)X
1 f
1823(.)X
3 f
555 4136(3.10.)N
775(Translating)X
1238(Window)X
1578(Coordinates)X
1 f
555 4260(Applications,)N
1050(mostly)X
1313(window)X
1618(managers,)X
1999(often)X
2202(need)X
2390(to)X
2481(perform)X
2786(a)X
2847(coordinate)X
3241(transformation)X
3782(from)X
555 4356(the)N
685(coordinate)X
1079(space)X
1296(of)X
1391(one)X
1540(window)X
1845(to)X
1936(another)X
2222(window)X
2527(or)X
2622(need)X
2810(to)X
2901(determine)X
3276(which)X
3513(subwindow)X
3940(a)X
555 4452(coordinate)N
949(lies)X
1094(in.)X
5 f
1250(XTranslateCoordinates)X
1 f
2187(ful\256lls)X
2440(these)X
2643(needs)X
2865(and)X
3014(avoids)X
3266(any)X
3415(race)X
3583(conditions)X
555 4548(by)N
665(asking)X
917(the)X
1047(X)X
1132(server)X
1368(to)X
1459(perform)X
1764(this)X
1914(operation.)X
2313(The)X
2472(de\256nition)X
2833(for)X
2957(this)X
3107(function)X
3423(is:)X
555 4672(int)N
671(XTranslateCoordinates)X
1495(\()X
2 f
1531(display)X
1 f
1786(,)X
2 f
1830(src_w)X
1 f
2047(,)X
2 f
2091(dest_w)X
1 f
2343(,)X
2 f
2387(src_x)X
1 f
2584(,)X
2 f
2628(src_y)X
1 f
2825(,)X
2 f
2869(dest_x_return)X
1 f
3365(,)X
2 f
1171 4768(dest_y_return)N
1 f
1667(,)X
2 f
1711(child_return)X
1 f
2159(\))X
687 4864(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4960(Window)N
2 f
1012(src_w)X
1 f
1229(,)X
2 f
1273(dest_w)X
1 f
1525(;)X
687 5056(int)N
2 f
803(src_x)X
1 f
1000(,)X
2 f
1044(src_y)X
1 f
1241(;)X
687 5152(int)N
803(*)X
2 f
(dest_x_return)S
1 f
1343(,)X
1387(*)X
2 f
(dest_y_return)S
1 f
1927(;)X
687 5248(Window)N
1012(*)X
2 f
(child_return)S
1 f
1504(;)X
2 f
555 5400(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5524(src_w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(source)X
2496(window.)X
2 f
555 5648(dest_w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(destination)X
2655(window.)X
3 f
2239 6144(37)N

38 p
%%Page: 38 42
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(src_x)N
555 768(src_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(within)X
2509(the)X
2639(source)X
2890(window.)X
2 f
555 892(dest_x_return)N
555 988(dest_y_return)N
1 f
1131(Returns)X
1427(the)X
1557(x)X
1623(and)X
1772(y)X
1838(coordinates)X
2266(within)X
2514(the)X
2644(destination)X
3054(window.)X
2 f
555 1112(child_return)N
1 f
1131(Returns)X
1427(the)X
1557(child)X
1756(if)X
1832(the)X
1962(coordinates)X
2390(are)X
2519(contained)X
2884(in)X
2975(a)X
3036(mapped)X
3337(child)X
3536(of)X
3631(the)X
3761(desti-)X
1131 1208(nation)N
1374(window.)X
555 1332(The)N
5 f
732(XTranslateCoordinates)X
1 f
1669(function)X
1985(takes)X
2188(the)X
2318(src_x)X
2530(and)X
2679(src_y)X
2891(coordinates)X
3319(within)X
3567(the)X
3697(source)X
555 1428(window)N
860(relative)X
1147(to)X
1238(the)X
1368(source)X
1619(window's)X
1987(origin)X
2220(and)X
2369(returns)X
2635(these)X
2838(coordinates)X
3266(to)X
3357(dest_x_return)X
3863(and)X
555 1524(dest_y_return)N
1061(relative)X
1348(to)X
1439(the)X
1569(destination)X
1979(window's)X
2347(origin.)X
2624(If)X
5 f
2722(XTranslateCoordinates)X
1 f
3659(returns)X
555 1620(zero)N
728(\(0\),)X
874(src_w)X
1105(and)X
1254(dest_w)X
1525(are)X
1654(on)X
1764(different)X
2089(screens,)X
2391(and)X
2540(dest_x_return)X
3046(and)X
3195(dest_y_return)X
3701(are)X
3830(zero)X
555 1716(\(0\).)N
723(If)X
803(the)X
933(coordinates)X
1361(are)X
1490(contained)X
1855(in)X
1946(a)X
2007(mapped)X
2308(child)X
2507(of)X
2602(dest_w,)X
2895(that)X
3050(child)X
3249(is)X
3330(returned)X
3645(to)X
3736(the)X
555 1812(child)N
754(argument.)X
555 1936(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XTranslateCoordinates)X
1 f
2739(is)X
5 f
2838(BadWindow)X
1 f
3322(.)X
3 f
2239 6144(38)N

39 p
%%Page: 39 43
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(4)X
1633 1056(Window)N
2003(Information)X
2528(Functions)X
1 f
11 s
555 1372(After)N
762(you)X
916(connect)X
1212(the)X
1342(display)X
1619(to)X
1710(the)X
1840(X)X
1925(server)X
2161(and)X
2310(create)X
2542(a)X
2603(window,)X
2930(you)X
3084(can)X
3228(use)X
3367(the)X
3497(Xlib)X
3676(window)X
555 1468(information)N
994(functions)X
1344(to:)X
10 f
555 1592(g)N
1 f
775(Obtain)X
1037(information)X
1476(about)X
1694(a)X
1755(window)X
10 f
555 1716(g)N
1 f
775(Manipulate)X
1199(property)X
1519(lists)X
10 f
555 1840(g)N
1 f
775(Obtain)X
1037(and)X
1186(change)X
1457(window)X
1762(properties)X
10 f
555 1964(g)N
1 f
775(Manipulate)X
1199(window)X
1504(selection)X
3 f
555 2156(4.1.)N
731(Obtaining)X
1136(Window)X
1476(Information)X
1 f
555 2280(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(obtain)X
2366(information)X
2805(about)X
3023(the)X
3153(window)X
3458(tree,)X
3634(the)X
555 2376(current)N
826(attributes)X
1177(of)X
1272(a)X
1333(window,)X
1660(its)X
1766(current)X
2037(geometry,)X
2414(or)X
2509(the)X
2639(current)X
2910(pointer)X
3182(coordinates.)X
3654(Because)X
555 2472(they)N
729(are)X
858(most)X
1052(frequently)X
1436(used)X
1619(by)X
1729(window)X
2034(managers,)X
2415(these)X
2618(functions)X
2968(all)X
3079(return)X
3311(a)X
3372(status)X
3595(to)X
3686(indicate)X
555 2568(whether)N
860(the)X
990(window)X
1295(still)X
1451(exists.)X
555 2788(Use)N
5 f
731(XQueryTree)X
1 f
1248(to)X
1339(obtain)X
1582(a)X
1643(list)X
1774(of)X
1869(children,)X
2202(the)X
2332(parent,)X
2596(and)X
2745(number)X
3036(of)X
3131(children)X
3442(for)X
3566(a)X
3627(speci\256ed)X
555 2884(window.)N
882(The)X
1041(de\256nition)X
1402(for)X
1526(this)X
1676(function)X
1992(is:)X
555 3008(Status)N
793(XQueryTree)X
1243(\()X
2 f
1279(display)X
1 f
1534(,)X
2 f
1578(w)X
1 f
1644(,)X
2 f
1688(root_return)X
1 f
2106(,)X
2 f
2150(parent_return)X
1 f
2651(,)X
2 f
2695(children_return)X
1 f
3260(,)X
2 f
3304(nchildren_return)X
1 f
3913(\))X
687 3104(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3200(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3296(Window)N
1012(*)X
2 f
(root_return)S
1 f
1474(;)X
687 3392(Window)N
1012(*)X
2 f
(parent_return)S
1 f
1557(;)X
687 3488(Window)N
1012(**)X
2 f
(children_return)S
1 f
1665(;)X
687 3584(unsigned)N
1027(int)X
1143(*)X
2 f
(nchildren_return)S
1 f
1796(;)X
2 f
555 3736(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3860(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(For)X
2208(this)X
2358(window,)X
2685(you)X
2839(obtain)X
3082(the)X
3212(list)X
3343(of)X
3438(its)X
3544(children,)X
3877(its)X
1131 3956(root,)N
1317(its)X
1423(parent,)X
1687(and)X
1836(the)X
1966(number)X
2257(of)X
2352(children.)X
2 f
555 4080(root_return)N
1 f
1131(Returns)X
1427(the)X
1557(root)X
1721(window)X
2026(ID)X
2140(for)X
2264(the)X
2394(speci\256ed)X
2729(window.)X
2 f
555 4204(parent_return)N
1 f
1131(Returns)X
1427(the)X
1557(parent)X
1799(window)X
2104(ID)X
2218(for)X
2342(the)X
2472(speci\256ed)X
2807(window.)X
2 f
555 4328(children_return)N
1 f
1131(Returns)X
1427(a)X
1488(pointer)X
1760(to)X
1851(the)X
1981(list)X
2112(of)X
2207(children)X
2518(for)X
2642(the)X
2772(speci\256ed)X
3107(window.)X
2 f
555 4452(nchildren_return)N
1 f
1157(Returns)X
1453(the)X
1583(number)X
1874(of)X
1969(children)X
2280(for)X
2404(the)X
2534(speci\256ed)X
2869(window.)X
555 4576(The)N
5 f
732(XQueryTree)X
1 f
1249(function)X
1565(returns)X
1831(the)X
1961(root)X
2125(ID,)X
2261(the)X
2391(parent)X
2633(window)X
2938(ID,)X
3074(a)X
3135(pointer)X
3407(to)X
3498(the)X
3628(list)X
3759(of)X
555 4672(children)N
866(windows,)X
1227(and)X
1376(the)X
1506(number)X
1797(of)X
1892(children)X
2203(in)X
2294(the)X
2424(list)X
2555(for)X
2679(the)X
2809(speci\256ed)X
3144(window.)X
3493(Use)X
5 f
3669(XFree)X
1 f
555 4768(to)N
646(free)X
804(this)X
954(list)X
1085(when)X
1297(it)X
1369(is)X
1450(no)X
1560(longer)X
1807(needed.)X
2122(\(See)X
2300(Section)X
2587(2.4)X
2719(for)X
2843(further)X
3104(information.\))X
3594(The)X
3753(chil-)X
555 4864(dren)N
733(are)X
862(listed)X
1076(in)X
1167(current)X
1438(stacking)X
1754(order,)X
1983(from)X
2176(bottom-most)X
2650(\(\256rst\))X
2867(to)X
2958(top-most)X
3294(\(last\).)X
5 f
570 4960(XQueryTree)N
1 f
1087(returns)X
1353(zero)X
1526(\(0\))X
1650(if)X
1726(it)X
1798(fails)X
1972(and)X
2121(nonzero)X
2426(if)X
2502(it)X
2574(succeeds.)X
555 5084(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XQueryTree)X
1 f
2319(is)X
5 f
2418(BadWindow)X
1 f
2902(.)X
555 5304(Use)N
5 f
731(XGetWindowAttributes)X
1 f
1655(to)X
1746(obtain)X
1989(the)X
2119(current)X
2390(attributes)X
2741(for)X
2865(a)X
2926(speci\256ed)X
3261(window.)X
3588(The)X
555 5400(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
3 f
2239 6144(39)N

40 p
%%Page: 40 44
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Status)N
793(XGetWindowAttributes)X
1646(\()X
2 f
1682(display)X
1 f
1937(,)X
2 f
1981(w)X
1 f
2047(,)X
2 f
2091(window_attributes_return)X
1 f
3020(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 960(XWindowAttributes)N
1428(*)X
2 f
(window_attributes_return)S
1 f
2401(;)X
2 f
555 1112(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1236(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(current)X
3276(attributes)X
3627(you)X
3781(want)X
1131 1332(to)N
1222(obtain.)X
2 f
555 1456(window_attributes_return)N
1 f
1131 1552(Rturns)N
1388(the)X
1518(speci\256ed)X
1853(window's)X
2221(attributes)X
2572(in)X
2663(the)X
5 f
2811(XWindowAttributes)X
1 f
3592(structure.)X
555 1676(The)N
5 f
732(XGetWindowAttributes)X
1 f
1656(function)X
1972(returns)X
2238(the)X
2368(current)X
2639(attributes)X
2990(for)X
3114(the)X
3244(speci\256ed)X
3579(window)X
3884(to)X
555 1772(an)N
5 f
678(XWindowAttributes)X
1 f
1459(structure.)X
1833(This)X
2012(structure)X
2342(is)X
2423(de\256ned)X
2704(as)X
2799(follows:)X
555 1964(typedef)N
841(struct)X
1059({)X
843 2060(int)N
959(x,)X
1047(y;)X
2283(/*)X
2374(location)X
2681(of)X
2776(window)X
3081(*/)X
843 2156(int)N
959(width,)X
1204(height;)X
2283(/*)X
2374(width)X
2597(and)X
2746(height)X
2989(of)X
3084(window)X
3389(*/)X
843 2252(int)N
959(border_width;)X
2283(/*)X
2374(border)X
2625(width)X
2848(of)X
2943(window)X
3248(*/)X
843 2348(int)N
959(depth;)X
2283(/*)X
2374(depth)X
2592(of)X
2687(window)X
2992(*/)X
843 2444(Visual)N
1095(*visual;)X
2283(/*)X
2374(the)X
2504(associated)X
2888(visual)X
3121(structure)X
3451(*/)X
843 2540(Window)N
1168(root;)X
2283(/*)X
2374(root)X
2538(of)X
2633(screen)X
2879(containing)X
3274(window)X
3579(*/)X
843 2636(int)N
959(class;)X
2283(/*)X
2374(InputOutput,)X
2849(InputOnly*/)X
843 2732(int)N
959(bit_gravity;)X
2283(/*)X
2374(one)X
2523(of)X
2618(bit)X
2734(gravity)X
3006(values)X
3253(*/)X
843 2828(int)N
959(win_gravity;)X
2283(/*)X
2374(one)X
2523(of)X
2618(the)X
2748(window)X
3053(gravity)X
3325(values)X
3572(*/)X
843 2924(int)N
959(backing_store;)X
2283(/*)X
2374(NotUseful,)X
2784(WhenMapped,)X
3326(Always)X
3616(*/)X
843 3020(unsigned)N
1183(long)X
1362(backing_planes;)X
2283(/*)X
2374(planes)X
2621(to)X
2712(be)X
2817(preserved)X
3180(if)X
3256(possible)X
3567(*/)X
843 3116(unsigned)N
1183(long)X
1362(backing_pixel;)X
2283(/*)X
2374(value)X
2587(to)X
2678(be)X
2783(used)X
2966(when)X
3178(restoring)X
3513(planes)X
3760(*/)X
843 3212(Bool)N
1037(save_under;)X
2283(/*)X
2374(boolean,)X
2697(should)X
2954(bits)X
3104(under)X
3326(be)X
3431(saved?)X
3692(*/)X
843 3308(Colormap)N
1218(colormap;)X
2283(/*)X
2374(color)X
2577(map)X
2751(to)X
2842(be)X
2947(associated)X
3331(with)X
3510(window)X
3815(*/)X
843 3404(Bool)N
1037(map_installed;)X
2283(/*)X
2374(boolean,)X
2697(is)X
2778(color)X
2981(map)X
3155(currently)X
3495(installed*/)X
843 3500(int)N
959(map_state;)X
2283(/*)X
2374(IsUnmapped,)X
2867(IsUnviewable,)X
3399(IsViewable)X
3821(*/)X
843 3596(long)N
1022(all_event_masks;)X
2283(/*)X
2374(set)X
2494(of)X
2589(events)X
2836(all)X
2947(people)X
3204(have)X
3392(interest)X
3674(in*/)X
843 3692(long)N
1022(your_event_mask;)X
2283(/*)X
2374(my)X
2509(event)X
2722(mask)X
2930(*/)X
843 3788(long)N
1022(do_not_propagate_mask;)X
2283(/*)X
2374(set)X
2494(of)X
2589(events)X
2836(that)X
2991(should)X
3248(not)X
3383(propagate)X
3752(*/)X
843 3884(Bool)N
1037(override_redirect;)X
2283(/*)X
2374(boolean)X
2675(value)X
2888(for)X
3012(override-redirect)X
3625(*/)X
843 3980(Screen)N
1104(*screen;)X
2283(/*)X
2374(back)X
2562(pointer)X
2834(to)X
2925(correct)X
3191(screen)X
3437(*/)X
555 4076(})N
619(XWindowAttributes;)X
555 4248(The)N
714(x)X
780(and)X
929(y)X
995(members)X
1340(are)X
1469(set)X
1589(to)X
1680(the)X
1810(coordinates)X
2238(that)X
2393(de\256ne)X
2630(the)X
2760(location)X
3067(of)X
3162(the)X
3292(drawable.)X
3680(If)X
3760(the)X
555 4344(drawable)N
899(is)X
980(a)X
1041(window,)X
1368(these)X
1571(coordinates)X
1999(specify)X
2275(the)X
2405(upper)X
2627(left)X
2767(outer)X
2970(corner)X
3216(relative)X
3503(to)X
3594(the)X
3724(parent)X
555 4440(window's)N
923(origin.)X
1200(If)X
1280(the)X
1410(drawable)X
1754(is)X
1835(a)X
1896(pixmap,)X
2205(these)X
2408(members)X
2753(are)X
2882(set)X
3002(to)X
3093(zero)X
3266(\(0\).)X
3434(The)X
3593(width)X
3816(and)X
555 4536(height)N
798(members)X
1143(are)X
1272(set)X
1392(to)X
1483(the)X
1613(drawable's)X
2020(dimensions.)X
2488(For)X
2632(a)X
2693(window,)X
3020(these)X
3223(dimensions)X
3647(specify)X
555 4632(the)N
685(inside)X
918(size)X
1077(of)X
1172(the)X
1302(window,)X
1629(not)X
1764(including)X
2120(the)X
2250(border.)X
555 4756(The)N
714(border_width)X
1210(member)X
1521(is)X
1602(set)X
1722(to)X
1813(the)X
1943(window's)X
2311(border)X
2562(width)X
2785(in)X
2876(pixels.)X
3153(If)X
3233(the)X
3363(drawable)X
3707(is)X
3788(a)X
3849(pix-)X
555 4852(map,)N
751(this)X
901(member)X
1212(is)X
1293(set)X
1413(to)X
1504(zero)X
1677(\(0\).)X
1845(The)X
2004(depth)X
2222(member)X
2533(is)X
2614(set)X
2734(to)X
2825(the)X
2955(depth)X
3173(of)X
3268(the)X
3398(pixmap)X
3685(\(that)X
3869(is,)X
555 4948(bits)N
705(per)X
839(pixel)X
1038(for)X
1162(the)X
1292(object\).)X
1603(The)X
1762(depth)X
1980(must)X
2174(be)X
2279(supported)X
2648(by)X
2758(the)X
2888(root)X
3052(of)X
3147(the)X
3277(speci\256ed)X
3612(drawable.)X
555 5072(The)N
714(visual)X
947(member)X
1258(is)X
1339(a)X
1400(pointer)X
1672(to)X
1763(the)X
1893(screen's)X
2202(associated)X
5 f
2604(Visual)X
1 f
2880(structure.)X
3254(The)X
3413(root)X
3577(member)X
3888(is)X
555 5168(set)N
675(to)X
766(the)X
896(root)X
1060(ID)X
1174(of)X
1269(the)X
1399(screen)X
1645(containing)X
2040(the)X
2170(window.)X
2519(The)X
2678(class)X
2871(member)X
3182(is)X
3263(set)X
3383(to)X
3474(the)X
3604(window's)X
555 5264(class)N
748(and)X
897(can)X
1041(be)X
1146(one)X
1295(of)X
1390(the)X
1520(constants)X
5 f
1888 0.3187(InputOutput)AX
1 f
2388(or)X
5 f
2501(InputOnly)X
1 f
2894(.)X
555 5388(The)N
714(bit_gravity)X
1124(member)X
1435(is)X
1516(set)X
1636(to)X
1727(the)X
1857(window's)X
2225(bit)X
2341(gravity)X
2613(and)X
2762(can)X
2906(be)X
3011(one)X
3160(of)X
3255(these)X
3458(constants:)X
3 f
2239 6144(40)N

41 p
%%Page: 41 45
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(ForgetGravity)N
1419(EastGravity)X
555 768(NorthWestGravity)N
1419(SouthWestGravity)X
555 864(NorthGravity)N
1419(SouthGravity)X
555 960(NorthEastGravity)N
1419(SouthEastGravity)X
555 1056(WestGravity)N
1419(StaticGravity)X
555 1152(CenterGravity)N
555 1324(See)N
704(the)X
834(Con\256guring)X
1282(Windows)X
1641(section)X
1913(in)X
2004(Chapter)X
2305(3)X
2371(for)X
2495(additional)X
2871(information)X
3310(on)X
3420(bit)X
3536(gravity.)X
555 1448(The)N
714(win_gravity)X
1162(member)X
1473(is)X
1554(set)X
1674(to)X
1765(the)X
1895(window's)X
2263(window)X
2568(gravity)X
2840(and)X
2989(can)X
3133(be)X
3238(one)X
3387(of)X
3482(these)X
3685(con-)X
555 1544(stants:)N
555 1668(UnmapGravity)N
1419(EastGravity)X
555 1764(NorthWestGravity)N
1419(SouthWestGravity)X
555 1860(NorthGravity)N
1419(SouthGravity)X
555 1956(NorthEastGravity)N
1419(SouthEastGravity)X
555 2052(WestGravity)N
1419(StaticGravity)X
555 2148(CenterGravity)N
555 2320(The)N
714(backing_store)X
1230(member)X
1541(is)X
1622(set)X
1742(to)X
1833(indicate)X
2135(how)X
2308(the)X
2438(X)X
2523(server)X
2759(should)X
3016(maintain)X
3348(the)X
3478(contents)X
3794(of)X
3889(a)X
555 2416(window.)N
904(It)X
980(can)X
1124(be)X
1229(set)X
1349(to)X
1440(one)X
1589(of)X
1684(the)X
1814(constants)X
5 f
2182(WhenMapped)X
1 f
2745(,)X
5 f
2807(Always)X
1 f
3100(,)X
3144(or)X
5 f
3257(NotUseful)X
1 f
3658(.)X
3724(The)X
555 2512(backing_planes)N
1125(member)X
1436(is)X
1517(set)X
1637(to)X
1728(indicate)X
2030(\(with)X
2238(one)X
2387(bits\))X
2566(which)X
2803(bit)X
2919(planes)X
3166(of)X
3261(the)X
3391(window)X
3696(hold)X
555 2608(dynamic)N
881(data)X
1050(that)X
1205(must)X
1399(be)X
1504(preserved)X
1867(in)X
1958(backing_stores)X
2508(and)X
2657(during)X
2909(save_unders.)X
3409(The)X
555 2704(backing_pixel)N
1077(member)X
1388(is)X
1469(set)X
1589(to)X
1680(indicate)X
1982(what)X
2175(values)X
2422(to)X
2513(use)X
2652(when)X
2864(restoring)X
3199(planes)X
3446(from)X
3639(a)X
3700(partial)X
555 2800(backing)N
856(store.)X
555 2924(The)N
714(save_under)X
1136(member)X
1447(is)X
1528(set)X
1648(to)X
1739(one)X
1888(of)X
1983(the)X
2113(constants)X
5 f
2481(True)X
1 f
2699(or)X
5 f
2812(False)X
1 f
3042(.)X
3108(The)X
3267(colormap)X
3622(member)X
3933(is)X
555 3020(set)N
675(to)X
766(the)X
896(color)X
1099(map)X
1273(for)X
1397(the)X
1527(speci\256ed)X
1862(window)X
2167(and)X
2316(can)X
2460(be)X
2565(a)X
2626(colormap)X
2981(ID)X
3095(or)X
3190(the)X
3320(constant)X
5 f
3654(None)X
1 f
3879(.)X
555 3116(The)N
714(map_installed)X
1232(member)X
1543(is)X
1624(set)X
1744(to)X
1835(indicate)X
2137(whether)X
2442(the)X
2572(color)X
2775(map)X
2949(is)X
3030(currently)X
3370(installed.)X
3736(It)X
3812(can)X
555 3212(be)N
660(one)X
809(of)X
904(the)X
1034(constants)X
5 f
1402(True)X
1 f
1620(or)X
5 f
1733(False)X
1 f
1963(.)X
2029(The)X
2188(map_state)X
2568(member)X
2879(is)X
2960(set)X
3080(to)X
3171(indicate)X
3473(the)X
3603(state)X
3787(of)X
3882(the)X
555 3308(window)N
860(and)X
1009(can)X
1153(be)X
1258(one)X
1407(of)X
1502(the)X
1632(constants)X
5 f
2000(IsUnmapped)X
1 f
2514(,)X
5 f
2576(IsUnviewable)X
1 f
3113(,)X
3157(or)X
5 f
3270(IsViewable)X
1 f
3710(.)X
3776(This)X
555 3404(member)N
866(gets)X
1030(set)X
1150(to)X
5 f
1259(IsUnviewable)X
1 f
1818(if)X
1894(the)X
2024(window)X
2329(is)X
2410(mapped)X
2711(but)X
2846(some)X
3054(ancestor)X
3369(is)X
3450(unmapped.)X
555 3528(The)N
714(all_event_masks)X
1324(member)X
1635(is)X
1716(set)X
1836(to)X
1927(the)X
2057(bitwise)X
2334(inclusive)X
2675(OR)X
2819(of)X
2914(all)X
3025(event)X
3238(masks)X
3480(selected)X
3786(on)X
3896(the)X
555 3624(window)N
860(by)X
970(interested)X
1335(clients.)X
1632(The)X
1791(your_event_mask)X
2439(member)X
2750(is)X
2831(set)X
2951(to)X
3042(the)X
3172(bitwise)X
3449(inclusive)X
3790(OR)X
3934(of)X
555 3720(all)N
666(event)X
879(masks)X
1121(selected)X
1427(by)X
1537(the)X
1667(querying)X
2002(client.)X
2265(The)X
2424(do_not_propagate_mask)X
3312(member)X
3623(is)X
3704(set)X
3824(to)X
555 3816(the)N
685(bitwise)X
962(inclusive)X
1303(OR)X
1447(of)X
1542(the)X
1672(set)X
1792(of)X
1887(events)X
2134(that)X
2289(should)X
2546(not)X
2681(propagate.)X
3094(See)X
3243(Section)X
3530(8.3)X
3662(for)X
3786(a)X
3847(dis-)X
555 3912(cussion)N
841(of)X
936(events)X
1183(and)X
1332(the)X
1462(event)X
1675(mask.)X
555 4036(The)N
714(override_redirect)X
1342(member)X
1653(is)X
1734(set)X
1854(to)X
1945(indicate)X
2247(whether)X
2552(this)X
2702(window)X
3007(overrides)X
3356(structure)X
3686(control)X
555 4132(facilities.)N
926(It)X
1002(can)X
1146(be)X
1251(one)X
1400(of)X
1495(the)X
1625(constants)X
5 f
1993(True)X
1 f
2211(or)X
5 f
2324(False)X
1 f
2554(.)X
2620(Window)X
2945(manager)X
3270(clients)X
3523(usually)X
555 4228(should)N
812(ignore)X
1059(the)X
1189(window)X
1494(if)X
1570(this)X
1720(member)X
2031(is)X
5 f
2130(True)X
1 f
2326(.)X
2392(Transient)X
2747(windows)X
3086(should)X
3343(mark)X
3546(which)X
3783(win-)X
555 4324(dows)N
762(they)X
936(are)X
1065(associated)X
1449(with.)X
1672(See)X
1821(Section)X
2108(9.1.9)X
2306(for)X
2430(further)X
2691(information.)X
555 4448(The)N
714(screen)X
960(member)X
1271(is)X
1352(set)X
1472(to)X
1563(a)X
1624(screen)X
1870(pointer)X
2142(that)X
2297(gives)X
2505(you)X
2659(a)X
2720(back)X
2908(pointer)X
3180(to)X
3271(the)X
3401(correct)X
3667(screen.)X
555 4544(This)N
734(makes)X
981(it)X
1053(easier)X
1280(to)X
1371(obtain)X
1614(the)X
1744(screen)X
1990(information)X
2429(without)X
2721(having)X
2983(to)X
3074(loop)X
3253(over)X
3431(the)X
3561(root)X
3725(window)X
555 4640(\256elds)N
768(to)X
859(see)X
993(which)X
1230(matches.)X
555 4764(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XGetWindowAttributes)X
1 f
2726(is)X
5 f
2825(BadWindow)X
1 f
3309(.)X
555 4984(Use)N
5 f
731(XGetGeometry)X
1 f
1357(to)X
1448(obtain)X
1691(the)X
1821(current)X
2092(geometry)X
2447(of)X
2542(the)X
2672(speci\256ed)X
3007(drawable.)X
3373(The)X
3532(de\256nition)X
3893(for)X
555 5080(this)N
705(function)X
1021(is:)X
3 f
2239 6144(41)N

42 p
%%Page: 42 46
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Status)N
793(XGetGeometry)X
1342(\()X
2 f
1378(display)X
1 f
1633(,)X
2 f
1677(d)X
1 f
1728(,)X
2 f
1779(root_return)X
1 f
2197(,)X
2 f
2241(x_return)X
1 f
2551(,)X
2 f
2595(y_return)X
1 f
2905(,)X
2 f
2949(width_return)X
1 f
3417(,)X
2 f
1039 768(height_return)N
1 f
1531(,)X
2 f
1575(border_width_return)X
1 f
2326(,)X
2 f
2370(depth_return)X
1 f
2837(\))X
731 864(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 960(Drawable)N
2 f
1094(d)X
1 f
1145(;)X
731 1056(Window)N
1056(*)X
2 f
(root_return)S
1 f
1518(;)X
731 1152(int)N
847(*)X
2 f
(x_return)S
1 f
1201(,)X
1245(*)X
2 f
(y_return)S
1 f
1599(;)X
731 1248(unsigned)N
1071(int)X
1187(*)X
2 f
(width_return)S
1 f
1699(,)X
1743(*)X
2 f
(height_return)S
1 f
2279(;)X
731 1344(unsigned)N
1071(int)X
1187(*)X
2 f
(border_width_return)S
1 f
1982(;)X
731 1440(unsigned)N
1071(int)X
1187(*)X
2 f
(depth_return)S
1 f
1698(;)X
2 f
555 1592(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1716(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
1967(The)X
2126(drawable)X
2470(can)X
2614(be)X
2719(either)X
2942(a)X
3003(window)X
3308(or)X
3403(a)X
3464(pixmap.)X
2 f
555 1840(root_return)N
1 f
1131(Returns)X
1427(the)X
1557(root)X
1721(window)X
2026(ID)X
2140(for)X
2264(the)X
2394(speci\256ed)X
2729(window.)X
2 f
555 1964(x_return)N
555 2088(y_return)N
1 f
1131(Returns)X
1427(the)X
1557(x)X
1623(and)X
1772(y)X
1838(coordinates)X
2266(of)X
2361(the)X
2491(drawable.)X
2879(These)X
3111(coordinates)X
3539(de\256ne)X
3776(the)X
1131 2184(location)N
1438(of)X
1533(the)X
1663(drawable.)X
2051(For)X
2195(a)X
2256(window,)X
2583(these)X
2786(coordinates)X
3214(specify)X
3490(the)X
3620(upper)X
3842(left)X
1131 2280(outer)N
1334(corner)X
1580(relative)X
1867(to)X
1958(its)X
2064(parent's)X
2369(origin.)X
2646(For)X
2790(pixmaps,)X
3133(these)X
3336(coordinates)X
3764(are)X
1131 2376(always)N
1397(zero)X
1570(\(0\).)X
2 f
555 2500(width_return)N
555 2596(height_return)N
1 f
1131(Returns)X
1427(the)X
1557(drawable's)X
1964(dimensions)X
2388(\(width)X
2640(and)X
2789(height\).)X
3105(For)X
3249(a)X
3310(window,)X
3637(these)X
1131 2692(dimensions)N
1555(specify)X
1831(the)X
1961(inside)X
2194(size,)X
2375(not)X
2510(including)X
2866(the)X
2996(border.)X
2 f
555 2816(border_width_return)N
1 f
1131 2912(Returns)N
1427(the)X
1557(border)X
1808(width)X
2031(in)X
2122(pixels.)X
2377(The)X
2536(function)X
2852(returns)X
3118(the)X
3248(border)X
3499(width)X
3722(only)X
3901(if)X
1131 3008(the)N
1261(drawable)X
1605(is)X
1686(a)X
1747(window.)X
2096(It)X
2172(returns)X
2438(zero)X
2611(\(0\))X
2735(if)X
2811(the)X
2941(drawable)X
3285(is)X
3366(a)X
3427(pixmap.)X
2 f
555 3132(depth_return)N
1 f
1131(Returns)X
1427(the)X
1557(depth)X
1775(of)X
1870(the)X
2000(pixmap)X
2287(\(bits)X
2466(per)X
2600(pixel)X
2799(for)X
2923(the)X
3053(object\).)X
555 3256(The)N
5 f
732(XGetGeometry)X
1 f
1358(function)X
1674(returns)X
1940(the)X
2070(root)X
2234(ID)X
2348(and)X
2497(the)X
2627(current)X
2898(geometry)X
3253(of)X
3348(the)X
3478(drawable.)X
3866(The)X
555 3352(geometry)N
910(of)X
1005(the)X
1135(drawable)X
1479(includes)X
1795(the)X
1925(x)X
1991(and)X
2140(y)X
2206(coordinates,)X
2656(width)X
2879(and)X
3028(height,)X
3293(border)X
3544(width,)X
3789(and)X
555 3448(depth.)N
817(These)X
1049(are)X
1178(described)X
1537(in)X
1628(the)X
1758(argument)X
2113(list.)X
2288(It)X
2364(is)X
2445(legal)X
2639(to)X
2730(pass)X
2903(to)X
2994(this)X
3144(function)X
3460(a)X
3521(window)X
555 3544(whose)N
801(class)X
994(is)X
5 f
1093(InputOnly)X
1 f
1486(.)X
555 3668(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XGetGeometry)X
1 f
2428(is)X
5 f
2527(BadDrawable)X
1 f
3069(.)X
555 3888(Use)N
5 f
731(XQueryPointer)X
1 f
1346(to)X
1437(obtain)X
1680(the)X
1810(root)X
1974(window)X
2279(the)X
2409(pointer)X
2681(is)X
2762(currently)X
3102(on)X
3212(and)X
3361(the)X
3491(pointer)X
3763(coor-)X
555 3984(dinates)N
827(relative)X
1114(to)X
1205(the)X
1335(root's)X
1562(origin.)X
1839(The)X
1998(de\256nition)X
2359(for)X
2483(this)X
2633(function)X
2949(is:)X
555 4108(Bool)N
749(XQueryPointer)X
1293(\()X
2 f
1329(display)X
1 f
1584(,)X
2 f
1628(w)X
1 f
1694(,)X
2 f
1738(root_return)X
1 f
2156(,)X
2 f
2200(child_return)X
1 f
2648(,)X
2 f
2692(root_x_return)X
1 f
3193(,)X
2 f
3237(root_y_return)X
1 f
3738(,)X
2 f
1017 4204(win_x_return)N
1 f
1499(,)X
2 f
1543(win_y_return)X
1 f
2025(,)X
2 f
2069(mask_return)X
1 f
2520(\))X
687 4300(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4396(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 4492(Window)N
1012(*)X
2 f
(root_return)S
1 f
1474(,)X
1518(*)X
2 f
(child_return)S
1 f
2010(;)X
687 4588(int)N
803(*)X
2 f
(root_x_return)S
1 f
1348(,)X
1392(*)X
2 f
(root_y_return)S
1 f
1937(;)X
687 4684(int)N
803(*)X
2 f
(win_x_return)S
1 f
1329(,)X
1373(*)X
2 f
(win_y_return)S
1 f
1899(;)X
687 4780(unsigned)N
1027(int)X
1143(*)X
2 f
(mask_return)S
1 f
1638(;)X
2 f
555 4932(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5056(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 5180(root_return)N
1 f
1131(Returns)X
1427(the)X
1557(root)X
1721(window)X
2026(ID)X
2140(for)X
2264(the)X
2394(speci\256ed)X
2729(window.)X
3078(This)X
3257(root)X
3421(ID)X
3535(identi\256es)X
3881(the)X
1131 5276(root)N
1295(window)X
1600(the)X
1730(pointer)X
2002(is)X
2083(currently)X
2423(on.)X
2 f
555 5400(child_return)N
1 f
1131(Returns)X
1427(the)X
1557(child)X
1756(window)X
2061(ID)X
2175(that)X
2330(the)X
2460(pointer)X
2732(is)X
2813(located)X
3090(in,)X
3203(if)X
3279(any.)X
2 f
555 5524(root_x_return)N
555 5620(root_y_return)N
1 f
1131(Returns)X
1427(the)X
1557(pointer)X
1829(coordinates)X
2257(relative)X
2544(to)X
2635(the)X
2765(root)X
2929(window's)X
3297(origin.)X
2 f
555 5744(win_x_return)N
3 f
2239 6144(42)N

43 p
%%Page: 43 47
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(win_y_return)N
1 f
1131(Returns)X
1427(the)X
1557(pointer)X
1829(coordinates)X
2257(relative)X
2544(to)X
2635(the)X
2765(speci\256ed)X
3100(window.)X
2 f
555 796(mask_return)N
1 f
1131(Returns)X
1427(the)X
1557(current)X
1828(state)X
2012(of)X
2107(the)X
2237(modi\256er)X
2558(keys)X
2741(and)X
2890(pointer)X
3162(buttons.)X
555 920(The)N
5 f
732(XQueryPointer)X
1 f
1347(function)X
1663(returns)X
1929(the)X
2059(root)X
2223(window)X
2528(the)X
2658(pointer)X
2930(is)X
3011(currently)X
3351(on)X
3461(and)X
3610(the)X
3740(pointer)X
555 1016(coordinates)N
983(relative)X
1270(to)X
1361(the)X
1491(root)X
1655(window's)X
2023(origin.)X
2300(If)X
5 f
2398(XQueryPointer)X
1 f
3013(returns)X
5 f
3297(False)X
1 f
3527(,)X
3571(the)X
3701(pointer)X
555 1112(is)N
636(not)X
771(on)X
881(the)X
1011(same)X
1214(screen)X
1460(as)X
1555(the)X
1685(window)X
1990(associated)X
2374(with)X
2553(the)X
2683(window)X
2988(you)X
3142(passed)X
3398(to)X
3489(the)X
3619(w)X
3704(argu-)X
555 1208(ment.)N
798(In)X
893(this)X
1043(case,)X
1238(the)X
1368(function)X
1684(returns)X
1950(the)X
2080(constant)X
5 f
2414(None)X
1 f
2661(to)X
2752(child_return)X
3205(and)X
3354(zero)X
3527(\(0\))X
3651(to)X
555 1304(win_x_return)N
1051(and)X
1200(win_y_return.)X
1740(If)X
5 f
1838(XQueryPointer)X
1 f
2453(returns)X
5 f
2737(True)X
1 f
2933(,)X
2977(the)X
3107(pointer)X
3379(coordinates)X
555 1400(returned)N
870(to)X
961(win_x_return)X
1457(and)X
1606(win_y_return)X
2102(are)X
2231(relative)X
2518(to)X
2609(the)X
2739(origin)X
2972(of)X
3067(the)X
3197(window)X
3502(identi\256ed)X
3858(by)X
555 1496(the)N
685(w)X
770(argument.)X
1169(In)X
1264(this)X
1414(case,)X
1609(the)X
1739(function)X
2055(returns)X
2321(the)X
2451(ID)X
2565(of)X
2660(the)X
2790(child)X
2989(containing)X
3384(the)X
3514(pointer,)X
3808(if)X
555 1592(any.)N
555 1716(The)N
714(state)X
898(of)X
993(the)X
1123(keyboard)X
1472(buttons)X
1754(and)X
1903(the)X
2033(modi\256er)X
2354(keys)X
2537(are)X
2666(returned)X
2981(in)X
3072(the)X
3202(mask_return)X
3664(argu-)X
555 1812(ment.)N
798(Depending)X
1206(on)X
1316(the)X
1446(current)X
1717(state)X
1901(of)X
1996(the)X
2126(mouse)X
2378(buttons)X
2660(and)X
2809(the)X
2939(modi\256er)X
3260(keys,)X
5 f
570 1908(XQueryPointer)N
1 f
1185(can)X
1329(set)X
1449(this)X
1599(argument)X
1954(to)X
2045(the)X
2175(bitwise)X
2452(inclusive)X
2793(OR)X
2937(of)X
3032(one)X
3181(or)X
3276(more)X
3479(of)X
3574(the)X
3704(button)X
555 2004(or)N
650(modi\256er)X
971(key)X
1120(bitmasks:)X
5 f
1499(Button1Mask)X
1 f
2029(,)X
5 f
2091(Button2Mask)X
1 f
2621(,)X
5 f
2683(Button3Mask)X
1 f
3213(,)X
5 f
3275(Button4Mask)X
1 f
3805(,)X
5 f
570 2100(Button5Mask)N
1 f
1100(,)X
5 f
1162(ShiftMask)X
1 f
1564(,)X
5 f
1626(LockMask)X
1 f
2037(,)X
5 f
2099(ControlMask)X
1 f
2607(,)X
5 f
2669(Mod1Mask)X
1 f
3114(,)X
5 f
3176(Mod2Mask)X
1 f
3621(,)X
5 f
570 2196(Mod3Mask)N
1 f
1015(,)X
5 f
1077(Mod4Mask)X
1 f
1522(,)X
5 f
1584(Mod5Mask)X
1 f
2029(.)X
555 2320(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XQueryPointer)X
1 f
2417(is)X
5 f
2516(BadWindow)X
1 f
3000(.)X
3 f
555 2512(4.2.)N
731(Properties)X
1144(and)X
1308(Atoms)X
1 f
555 2636(A)N
640(property)X
960(is)X
1041(a)X
1102(collection)X
1473(of)X
1568(named)X
1825(typed)X
2043(data.)X
2256(The)X
2415(type)X
2589(of)X
2684(a)X
2745(property)X
3065(is)X
3146(de\256ned)X
3427(by)X
3537(other)X
3740(proper-)X
555 2732(ties,)N
722(which)X
959(allows)X
1211(for)X
1335(arbitrary)X
1660(extension)X
2020(in)X
2111(this)X
2261(type)X
2435(scheme.)X
2765(Data)X
2953(of)X
3048(more)X
3251(than)X
3425(one)X
3574(type)X
3748(may)X
3922(be)X
555 2828(associated)N
939(with)X
1118(a)X
1179(single)X
1412(property.)X
1754(An)X
1883(Atom)X
2106(is)X
2187(just)X
2337(a)X
2398(nickname)X
2763(for)X
2887(a)X
2948(property,)X
3290(so)X
3390(that)X
3545(the)X
3675(protocol)X
555 2924(does)N
738(not)X
873(have)X
1061(to)X
1152(send)X
1335(arbitrary)X
1660(length)X
1903(property)X
2223(names)X
2470(back)X
2658(and)X
2807(forth.)X
555 3048(A)N
640(property)X
960(is)X
1041(also)X
1205(stored)X
1442(in)X
1533(one)X
1682(of)X
1777(several)X
2048(possible)X
2359(formats.)X
2694(The)X
2853(X)X
2938(server)X
3174(can)X
3318(store)X
3511(the)X
3641(informa-)X
555 3144(tion)N
715(as)X
810(8-bit)X
999(quantities,)X
1387(16-bit)X
1620(quantities,)X
2008(or)X
2103(32-bit)X
2336(quantities.)X
2746(This)X
2925(permits)X
3212(the)X
3342(X)X
3427(server)X
3663(to)X
3754(present)X
555 3240(the)N
685(data)X
854(in)X
945(the)X
1075(byte)X
1249(order)X
1456(that)X
1611(the)X
1741(client)X
1960(expects.)X
2197 3416(Note)N
775 3552(If)N
855(you)X
1009(de\256ne)X
1246(further)X
1507(properties)X
1881(of)X
1976(complex)X
2302(type,)X
2498(you)X
2652(must)X
2846(encode)X
3117(and)X
3266(decode)X
3537(them)X
775 3648(yourself)N
1085(into)X
1245(one)X
1394(of)X
1489(the)X
1619(possible)X
1930(formats.)X
555 3852(The)N
714(window)X
1019(system)X
1286(has)X
1425(a)X
1486(set)X
1606(of)X
1701(prede\256ned)X
2094(properties)X
2468(\(for)X
2621(example,)X
2964(the)X
3094(name)X
3307(of)X
3402(a)X
3463(window,)X
3790(size)X
555 3948(hints,)N
771(and)X
920(so)X
1020(on\),)X
1181(and)X
1330(users)X
1532(can)X
1676(de\256ne)X
1913(any)X
2062(other)X
2265(arbitrary)X
2590(information)X
3029(and)X
3178(can)X
3322(associate)X
3662(them)X
555 4044(with)N
734(windows.)X
1117(Each)X
1315(property)X
1635(has)X
1774(a)X
1835(name,)X
2070(which)X
2307(is)X
2388(an)X
2493(ISO)X
2656(Latin-1)X
2938(string.)X
3205(For)X
3349(each)X
3532(named)X
3789(pro-)X
555 4140(perty)N
758(there)X
956(is)X
1037(a)X
1098(unique)X
1360(identi\256er)X
1701(\(atom\))X
1958(associated)X
2342(with)X
2521(it.)X
2615(A)X
2700(property)X
3020(also)X
3184(has)X
3323(a)X
3384(type,)X
3580(for)X
3704(exam-)X
555 4236(ple,)N
707(string)X
930(or)X
1025(integer.)X
1336(These)X
1568(types)X
1776(are)X
1905(also)X
2069(indicated)X
2415(using)X
2628(atoms,)X
2883(so)X
2983(arbitrary)X
3308(new)X
3476(types)X
3684(can)X
3828(be)X
555 4332(de\256ned.)N
555 4456(Certain)N
837(properties)X
1211(are)X
1340(prede\256ned)X
1733(in)X
1824(the)X
1954(server)X
2190(for)X
2314(commonly)X
2714(used)X
2897(functions.)X
3291(The)X
3450(atoms)X
3683(for)X
3807(these)X
555 4552(properties)N
929(are)X
1058(de\256ned)X
1339(in)X
5 f
1448(<X11/Xatom.h>)X
1 f
2076(.)X
2142(To)X
2262(avoid)X
2480(name)X
2693(clashes)X
2969(with)X
3148(user)X
3316(symbols,)X
3654(the)X
555 4648(#de\256ne)N
836(name)X
1049(for)X
1173(each)X
1356(atom)X
1555(has)X
1694(the)X
1824(``XA_'')X
2132(pre\256x)X
2359(added.)X
2635(See)X
2784(Section)X
3071(4.3)X
3203(for)X
3327(de\256nitions)X
3722(of)X
3817(these)X
555 4744(properties.)N
973(See)X
1122(Chapter)X
1423(9)X
1489(for)X
1613(an)X
1718(explanation)X
2152(of)X
2247(the)X
2377(functions)X
2727(that)X
2882(let)X
2993(you)X
3147(get)X
3277(and)X
3426(set)X
3546(much)X
3764(of)X
3859(the)X
555 4840(information)N
994(stored)X
1231(in)X
1322(these)X
1525(prede\256ned)X
1918(properties.)X
555 4964(You)N
728(can)X
872(use)X
1011(this)X
1161(mechanism)X
1585(to)X
1676(communicate)X
2174(other)X
2377(information)X
2816(between)X
3131(applications.)X
3624(The)X
3783(func-)X
555 5060(tions)N
749(described)X
1108(in)X
1199(this)X
1349(section)X
1621(let)X
1732(you)X
1886(de\256ne)X
2123(new)X
2291(properties)X
2665(and)X
2814(get)X
2944(the)X
3074(unique)X
3336(Atom)X
3559(IDs)X
3707(in)X
3798(your)X
555 5156(applications.)N
555 5280(Atoms)N
812(occur)X
1029(in)X
1120(\256ve)X
1274(distinct)X
1557(name)X
1770(spaces)X
2021(within)X
2269(the)X
2399(protocol.)X
10 f
555 5404(g)N
1 f
775(Selections)X
10 f
555 5528(g)N
1 f
775(Property)X
1100(names)X
10 f
555 5652(g)N
1 f
775(Property)X
1100(types)X
3 f
2239 6144(43)N

44 p
%%Page: 44 48
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(Font)X
959(properties)X
10 f
555 796(g)N
1 f
775(Type)X
978(of)X
1073(a)X
5 f
1152(ClientMessage)X
1 f
1770(event)X
1983(\(none)X
2205(are)X
2334(built)X
2519(into)X
2679(the)X
2809(X)X
2894(server\))X
555 920(Any)N
728(particular)X
1088(atom)X
1287(can)X
1431(have)X
1619(some)X
1827(client)X
2046(interpretation)X
2544(within)X
2792(each)X
2975(of)X
3070(the)X
3200(name)X
3413(spaces.)X
555 1044(The)N
714(built-in)X
997(selection)X
1333(properties,)X
1729(which)X
1966(name)X
2179(properties,)X
2575(are:)X
555 1168(PRIMARY)N
555 1264(SECONDARY)N
555 1436(The)N
714(built-in)X
997(property)X
1317(names)X
1564(are:)X
555 1628(CUT_BUFFER0)N
1839(RGB_GREEN_MAP)X
555 1724(CUT_BUFFER1)N
1839(RGB_RED_MAP)X
555 1820(CUT_BUFFER2)N
1839(RESOURCE_MANAGER)X
555 1916(CUT_BUFFER3)N
1839(WM_CLASS)X
555 2012(CUT_BUFFER4)N
1839(WM_CLIENT_MACHINE)X
555 2108(CUT_BUFFER5)N
1839(WM_COMMAND)X
555 2204(CUT_BUFFER6)N
1839(WM_HINTS)X
555 2300(CUT_BUFFER7)N
1839(WM_ICON_NAME)X
555 2396(RGB_BEST_MAP)N
1839(WM_ICON_SIZE)X
555 2492(RGB_BLUE_MAP)N
1839(WM_NAME)X
555 2588(RGB_DEFAULT_MAP)N
1839(WM_NORMAL_HINTS)X
555 2684(RGB_GRAY_MAP)N
1839(WM_ZOOM_HINTS)X
1839 2780(WM_TRANSIENT_FOR)N
555 3000(The)N
714(built-in)X
997(property)X
1317(types)X
1525(are:)X
555 3220(ARC)N
1839(POINT)X
555 3316(ATOM)N
1839(RGB_COLOR_MAP)X
555 3412(BITMAP)N
1839(RECTANGLE)X
555 3508(CARDINAL)N
1839(STRING)X
555 3604(COLORMAP)N
1839(VISUALID)X
555 3700(CURSOR)N
1839(WINDOW)X
555 3796(DRAWABLE)N
1839(WM_HINTS)X
555 3892(FONT)N
1839(WM_SIZE_HINTS)X
555 3988(INTEGER)N
555 4084(PIXMAP)N
555 4304(The)N
714(built-in)X
997(font)X
1161(property)X
1481(types)X
1689(are:)X
555 4496(MIN_SPACE)N
1839(STRIKEOUT_DESCENT)X
555 4592(NORM_SPACE)N
1839(STRIKEOUT_ASCENT)X
555 4688(MAX_SPACE)N
1839(ITALIC_ANGLE)X
555 4784(END_SPACE)N
1839(X_HEIGHT)X
555 4880(SUPERSCRIPT_X)N
1839 -0.3889(QUAD_WIDTH)AX
555 4976(SUPERSCRIPT_Y)N
1839(WEIGHT)X
555 5072(SUBSCRIPT_X)N
1839(POINT_SIZE)X
555 5168(SUBSCRIPT_Y)N
1839(RESOLUTION)X
555 5264 -0.2022(UNDERLINE_POSITION)AN
1839(COPYRIGHT)X
555 5360(UNDERLINE_THICKNESS)N
1839(NOTICE)X
555 5456(FONT_NAME)N
1839(FAMILY_NAME)X
555 5552(FULL_NAME)N
1839(CAP_HEIGHT)X
3 f
2239 6144(44)N

45 p
%%Page: 45 49
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
2197 672(Note)N
775 808(See)N
924(Chapter)X
1225(6)X
1291(for)X
1415(further)X
1676(information)X
2115(about)X
2333(font)X
2497(property)X
2817(atoms.)X
555 1108(Use)N
5 f
731(XInternAtom)X
1 f
1259(to)X
1350(return)X
1582(an)X
1687(atom)X
1886(for)X
2010(a)X
2071(speci\256ed)X
2406(name.)X
2641(The)X
2800(de\256nition)X
3161(for)X
3285(this)X
3435(function)X
3751(is:)X
555 1232(Atom)N
778(XInternAtom)X
1259(\()X
2 f
1295(display)X
1 f
1550(,)X
2 f
1594(atom_name)X
1 f
2011(,)X
2 f
2055(only_if_exists)X
1 f
2548(\))X
687 1328(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1424(char)N
860(*)X
2 f
(atom_name)S
1 f
1321(;)X
687 1520(Bool)N
2 f
881(only_if_exists)X
1 f
1374(;)X
2 f
555 1672(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1796(atom_name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(associated)X
2198(with)X
2377(the)X
2507(atom)X
2706(you)X
2860(want)X
3053(returned.)X
2 f
555 1920(only_if_exists)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(that)X
2201(indicates)X
2537(whether)X
5 f
2860(XInternAtom)X
1 f
3388(creates)X
3654(the)X
3784(atom.)X
1131 2016(You)N
1304(can)X
1448(pass)X
1621(one)X
1770(of)X
1865(the)X
1995(constants)X
5 f
2363(True)X
1 f
2581(or)X
5 f
2694(False)X
1 f
2924(.)X
555 2140(The)N
5 f
732(XInternAtom)X
1 f
1260(function)X
1576(returns)X
1842(the)X
1972(atom)X
2171(identi\256er)X
2512(associated)X
2896(with)X
3075(the)X
3205(string)X
3428(you)X
3582(passed)X
3838(to)X
555 2236(the)N
685(atom_name)X
1119(argument.)X
5 f
1539(XInternAtom)X
1 f
2067(returns)X
2333(the)X
2463(atom)X
2662(for)X
2786(the)X
2916(speci\256ed)X
3251(atom_name)X
3685(if)X
555 2332(only_if_exists)N
1077(is)X
5 f
1176(True)X
1 f
1372(.)X
1438(If)X
1518(only_if_exists)X
2040(is)X
5 f
2139(False)X
1 f
2369(,)X
2413(the)X
2543(atom)X
2742(is)X
2823(created)X
3099(if)X
3175(it)X
3247(does)X
3430(not)X
3565(exist.)X
555 2428(Therefore,)N
5 f
963(XInternAtom)X
1 f
1491(can)X
1635(return)X
5 f
1885(None)X
1 f
2110(.)X
2176(You)X
2349(should)X
2606(use)X
2745(a)X
2806(null-terminated)X
3373(ISO)X
3536(Latin-1)X
555 2524(string)N
778(for)X
902(atom_name.)X
1380(Case)X
1573(matters:)X
1880(the)X
2010(strings)X
2267("thing",)X
2565("Thing",)X
2892(and)X
3041("thinG")X
3336(all)X
3447(designate)X
3802(dif-)X
555 2620(ferent)N
782(atoms.)X
1037(The)X
1196(atom)X
1395(will)X
1555(remain)X
1822(de\256ned)X
2103(even)X
2291(after)X
2474(the)X
2604(client)X
2823(who)X
2996(de\256ned)X
3277(it)X
3349(has)X
3488(gone)X
3681(away.)X
3910(It)X
555 2716(will)N
715(become)X
1011(unde\256ned)X
1380(only)X
1559(when)X
1771(the)X
1901(last)X
2046(connection)X
2455(to)X
2546(the)X
2676(X)X
2761(server)X
2997(closes.)X
555 2840(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XInternAtom)X
1 f
2364(are)X
5 f
2511(BadAlloc)X
1 f
2895(and)X
5 f
3062(BadValue)X
1 f
3459(.)X
555 3060(Use)N
5 f
731(XGetAtomName)X
1 f
1410(to)X
1501(return)X
1733(a)X
1794(name)X
2007(for)X
2131(the)X
2261(speci\256ed)X
2596(atom)X
2795(identi\256er.)X
3180(The)X
3339(de\256nition)X
3700(for)X
3824(this)X
555 3156(function)N
871(is:)X
555 3280(char)N
728(*XGetAtomName)X
1380(\()X
2 f
1416(display)X
1 f
1671(,)X
2 f
1715(atom)X
1 f
1898(\))X
687 3376(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3472(Atom)N
2 f
910(atom)X
1 f
1093(;)X
2 f
555 3624(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3748(atom)N
1 f
1131(Speci\256es)X
1471(the)X
1601(atom)X
1800(associated)X
2184(with)X
2363(the)X
2493(string)X
2716(name)X
2929(you)X
3083(want)X
3276(returned.)X
555 3872(The)N
5 f
732(XGetAtomName)X
1 f
1411(function)X
1727(returns)X
1993(the)X
2123(name)X
2336(associated)X
2720(with)X
2899(the)X
3029(atom)X
3228(identi\256er)X
3569(you)X
3723(passed)X
555 3968(to)N
646(the)X
776(atom)X
975(argument.)X
1374(You)X
1547(previously)X
1941(obtained)X
2267(the)X
2397(atom)X
2596(identi\256er)X
2937(by)X
3047(calling)X
5 f
3328(XInternAtom)X
1 f
3834(.)X
555 4092(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XGetAtomName)X
1 f
2481(is)X
5 f
2580(BadAtom)X
1 f
2958(.)X
3 f
555 4284(4.3.)N
731(Obtaining)X
1136(and)X
1300(Changing)X
1689(Window)X
2029(Properties)X
1 f
555 4408(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(obtain,)X
2388(rotate,)X
2633(or)X
2728(change)X
2999(a)X
3060(window)X
3365(property.)X
3729(In)X
3824(addi-)X
555 4504(tion,)N
737(Xlib)X
916(provides)X
1241(other)X
1444(utility)X
1679(functions)X
2029(for)X
2153(prede\256ned)X
2546(property)X
2866(operations.)X
3299(See)X
3448(Chapter)X
3749(9)X
3815(for)X
555 4600(further)N
816(information)X
1255(about)X
1473(prede\256ned)X
1866(property)X
2186(functions.)X
555 4820(Use)N
5 f
731(XGetWindowProperty)X
1 f
1615(to)X
1706(obtain)X
1949(the)X
2079(atom)X
2278(type)X
2452(and)X
2601(property)X
2921(format)X
3178(for)X
3302(a)X
3363(speci\256ed)X
3698(window.)X
555 4916(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
3 f
2239 6144(45)N

46 p
%%Page: 46 50
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(int)N
671(XGetWindowProperty)X
1474(\()X
2 f
1510(display)X
1 f
1765(,)X
2 f
1809(w)X
1 f
1875(,)X
2 f
1919(property)X
1 f
2229(,)X
2 f
2273(long_offset)X
1 f
2673(,)X
2 f
2717(long_length)X
1 f
3146(,)X
2 f
3190(delete)X
1 f
3408(,)X
2 f
3452(req_type)X
1 f
3767(,)X
2 f
1083 768(actual_type_return)N
1 f
1766(,)X
2 f
1810(actual_format_return)X
1 f
2581(,)X
2 f
2625(nitems_return)X
1 f
3126(,)X
2 f
3170(bytes_after_return)X
1 f
3833(,)X
2 f
1083 864(prop_return)N
1 f
1520(\))X
687 960(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1056(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 1152(Atom)N
2 f
910(property)X
1 f
1220(;)X
687 1248(long)N
2 f
866(long_offset)X
1 f
1266(,)X
2 f
1310(long_length)X
1 f
1739(;)X
687 1344(Bool)N
2 f
881(delete)X
1 f
1099(;)X
687 1440(Atom)N
2 f
910(req_type)X
1 f
1225(;)X
687 1536(Atom)N
910(*)X
2 f
(actual_type_return)S
1 f
1637(;)X
687 1632(int)N
803(*)X
2 f
(actual_format_return)S
1 f
1618(;)X
687 1728(unsigned)N
1027(long)X
1206(*)X
2 f
(nitems_return)S
1 f
1751(;)X
687 1824(long)N
866(*)X
2 f
(bytes_after_return)S
1 f
1573(;)X
687 1920(unsigned)N
1027(char)X
1200(**)X
2 f
(prop_return)S
1 f
1725(;)X
2 f
555 2072(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2196(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(atom)X
3204(type)X
3378(and)X
3527(property)X
3847(for-)X
1131 2292(mat)N
1286(you)X
1440(want)X
1633(to)X
1724(obtain.)X
2 f
555 2416(property)N
1 f
1131(Speci\256es)X
1471(the)X
1601(property)X
1921(atom.)X
2 f
555 2540(long_offset)N
1 f
1131(Speci\256es)X
1471(the)X
1601(offset)X
1823(in)X
1914(the)X
2044(speci\256ed)X
2379(property)X
2699(\(in)X
2819(32-bit)X
3052(quantities\))X
3447(where)X
3683(data)X
3852(will)X
1131 2636(be)N
1236(retrieved.)X
2 f
555 2760(long_length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(length)X
1844(in)X
1935(32-bit)X
2168(multiples)X
2520(of)X
2615(the)X
2745(data)X
2914(to)X
3005(be)X
3110(retrieved.)X
2 f
555 2884(delete)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(that)X
2201(determines)X
2610(whether)X
2915(the)X
3045(property)X
3365(is)X
3446(deleted)X
3723(from)X
1131 2980(the)N
1261(window.)X
1610(You)X
1783(can)X
1927(pass)X
2100(one)X
2249(of)X
2344(these)X
2547(constants:)X
5 f
2940(True)X
1 f
3158(or)X
5 f
3271(False)X
1 f
3501(.)X
2 f
555 3104(req_type)N
1 f
1131(Speci\256es)X
1471(the)X
1601(atom)X
1800(identi\256er)X
2141(associated)X
2525(with)X
2704(the)X
2834(property)X
3154(type.)X
3372(You)X
3545(can)X
3689(pass)X
3862(an)X
1131 3200(atom)N
1330(identi\256er)X
1671(or)X
1766(the)X
1896(constant)X
5 f
2230(AnyPropertyType)X
1 f
2926(.)X
2 f
555 3324(actual_type_return)N
1 f
1131 3420(Returns)N
1427(the)X
1557(atom)X
1756(identi\256er)X
2119(that)X
2274(de\256nes)X
2545(the)X
2675(actual)X
2908(type)X
3082(of)X
3177(the)X
3307(property.)X
2 f
555 3544(actual_format_return)N
1 f
1131 3640(Returns)N
1427(the)X
1557(actual)X
1790(format)X
2047(of)X
2142(the)X
2272(property.)X
2 f
555 3764(nitems_return)N
1 f
1131(Returns)X
1427(the)X
1557(actual)X
1790(number)X
2081(of)X
2176(8-bit,)X
2387(16-bit,)X
2642(or)X
2737(32-bit)X
2970(items)X
3184(transferred.)X
2 f
555 3888(bytes_after_return)N
1 f
1131 3984(Returns)N
1427(the)X
1557(number)X
1848(of)X
1943(bytes)X
2151(remaining.)X
2575(This)X
2754(is)X
2835(the)X
2965(number)X
3256(of)X
3351(bytes)X
3559(remaining)X
3939(to)X
1131 4080(be)N
1236(read)X
1409(in)X
1500(the)X
1630(property)X
1950(if)X
2026(a)X
2087(partial)X
2335(read)X
2508(was)X
2666(performed.)X
2 f
555 4204(prop_return)N
1 f
1131(Returns)X
1427(a)X
1488(pointer)X
1760(to)X
1851(the)X
1981(data,)X
2172(in)X
2263(the)X
2393(speci\256ed)X
2728(format.)X
555 4328(The)N
5 f
732(XGetWindowProperty)X
1 f
1616(function)X
1932(returns)X
2198(the)X
2328(actual)X
2561(type)X
2735(of)X
2830(the)X
2960(property;)X
3305(the)X
3435(actual)X
3668(format)X
3925(of)X
555 4424(the)N
685(property;)X
1030(the)X
1160(number)X
1451(of)X
1546(8-bit,)X
1757(16-bit,)X
2012(or)X
2107(32-bit)X
2340(items)X
2554(transferred;)X
2981(the)X
3111(number)X
3402(of)X
3497(bytes)X
3705(remain-)X
555 4520(ing)N
690(to)X
781(be)X
886(read)X
1059(in)X
1150(the)X
1280(property;)X
1625(and)X
1774(a)X
1835(pointer)X
2107(to)X
2198(the)X
2328(data)X
2497(actually)X
2799(returned.)X
3158(This)X
3337(function)X
3653(sets)X
3807(the)X
555 4616(return)N
787(arguments)X
1176(according)X
1545(to)X
1636(the)X
1766(following:)X
10 f
555 4740(g)N
1 f
775(If)X
855(the)X
985(speci\256ed)X
1320(property)X
1640(does)X
1823(not)X
1958(exist)X
2147(for)X
2271(the)X
2401(speci\256ed)X
2736(window,)X
5 f
3081(XGetWindowProperty)X
1 f
775 4836(returns)N
1041(the)X
1171(constant)X
5 f
1505(None)X
1 f
1752(to)X
1843(the)X
1973(actual_type_return)X
2656(argument)X
3011(and)X
3160(the)X
3290(value)X
3503(zero)X
3676(\(0\))X
3800(to)X
3891(the)X
775 4932(actual_format_return)N
1541(and)X
1690(bytes_after_return)X
2357(arguments.)X
2790(The)X
2949(nitems_return)X
3461(argument)X
3816(is)X
775 5028(empty.)N
1062(In)X
1157(this)X
1307(case,)X
1502(the)X
1632(delete)X
1865(argument)X
2220(is)X
2301(ignored.)X
10 f
555 5152(g)N
1 f
775(If)X
855(the)X
985(speci\256ed)X
1320(property)X
1640(exists,)X
1885(but)X
2020(its)X
2126(type)X
2300(does)X
2483(not)X
2618(match)X
2856(the)X
2986(speci\256ed)X
3321(type,)X
5 f
3535(XGetWin-)X
775 5248(dowProperty)N
1 f
1306(returns)X
1572(the)X
1702(actual)X
1935(property)X
2255(type)X
2429(to)X
2520(the)X
2650(actual_type_return)X
3333(argument;)X
3713(the)X
775 5344(actual)N
1008(property)X
1328(format)X
1585(\(never)X
1831(zero\))X
2033(to)X
2124(the)X
2254(actual_format_return)X
3020(argument;)X
3400(and)X
3549(the)X
3679(property)X
775 5440(length)N
1018(in)X
1109(bytes)X
1317(\(even)X
1534(if)X
1610(the)X
1740(actual_format_return)X
2506(is)X
2587(16)X
2697(or)X
2792(32\))X
2931(to)X
3022(the)X
3152(bytes_after_return)X
3819(argu-)X
775 5536(ment.)N
1018(It)X
1094(also)X
1258(ignores)X
1539(the)X
1669(delete)X
1902(argument.)X
2301(The)X
2460(nitems_return)X
2972(argument)X
3327(is)X
3408(empty.)X
10 f
555 5660(g)N
1 f
775(If)X
855(the)X
985(speci\256ed)X
1320(property)X
1640(exists,)X
1885(and)X
2034(either)X
2257(you)X
2411(assign)X
2653(the)X
2783(constant)X
5 f
3117(AnyPropertyType)X
1 f
3835(to)X
775 5756(the)N
905(req_type)X
1235(argument)X
1590(or)X
1685(the)X
1815(speci\256ed)X
2150(type)X
2324(matches)X
2635(the)X
2765(actual)X
2998(property)X
3318(type,)X
5 f
3532(XGetWin-)X
775 5852(dowProperty)N
1 f
1306(returns)X
1572(the)X
1702(the)X
1832(actual)X
2065(property)X
2385(type)X
2559(to)X
2650(the)X
2780(actual_type_return)X
3463(argument)X
3818(and)X
3 f
2239 6144(46)N

47 p
%%Page: 47 51
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
775 672(the)N
905(actual)X
1138(property)X
1458(format)X
1715(\(never)X
1961(zero\))X
2163(to)X
2254(the)X
2384(actual_format_return)X
3150(argument.)X
3527(It)X
3603(also)X
3767(returns)X
775 768(a)N
836(value)X
1049(to)X
1140(the)X
1270(bytes_after_return)X
1937(and)X
2086(nitems_return)X
2598(arguments,)X
3009(by)X
3119(de\256ning)X
3430(the)X
3560(following)X
775 864(values:)N
995 988(N)N
1080(=)X
1152(actual)X
1385(length)X
1628(of)X
1723(the)X
1853(stored)X
2090(property)X
2410(in)X
2501(bytes)X
1083 1084(\(even)N
1300(if)X
1376(the)X
1506(format)X
1763(is)X
1844(16)X
1954(or)X
2049(32\))X
995 1180(I)N
1046(=)X
1118(4)X
1184(*)X
1250(long_offset)X
995 1276(T)N
1071(=)X
1143(N)X
1228(-)X
1279(I)X
995 1372(L)N
1071(=)X
1143(MINIMUM\(T,)X
1688(4)X
1754(*)X
1820(long_length\))X
995 1468(A)N
1080(=)X
1152(N)X
1237(-)X
1288(\(I)X
1368(+)X
1440(L\))X
775 1592(The)N
934(returned)X
1249(value)X
1462(starts)X
1670(at)X
1756(byte)X
1930(index)X
2148(I)X
2199(in)X
2290(the)X
2420(property)X
2740(\(indexing)X
3100(from)X
3293(zero\),)X
3517(and)X
3666(its)X
3772(length)X
775 1688(in)N
866(bytes)X
1074(is)X
1155(L.)X
1275(A)X
5 f
1378(BadValue)X
1 f
1797(error)X
1989(is)X
2070(returned)X
2385(if)X
2461(the)X
2591(value)X
2804(for)X
2928(long_offset)X
3351(causes)X
3602(L)X
3678(to)X
3769(be)X
775 1784(negative.)N
1118(The)X
1277(value)X
1490(of)X
1585(bytes_after_return)X
2252(is)X
2333(A,)X
2440(giving)X
2688(the)X
2818(number)X
3109(of)X
3204(trailing)X
3482(unread)X
3743(bytes)X
775 1880(in)N
866(the)X
996(stored)X
1233(property.)X
775 2004(If)N
855(delete)X
1088(is)X
5 f
1187(True)X
1 f
1405(and)X
1554(bytes_after_return)X
2221(is)X
2302(zero)X
2475(\(0\))X
2599(the)X
2729(function)X
3045(deletes)X
3312(the)X
3442(property)X
3762(from)X
775 2100(the)N
905(window)X
1210(and)X
1359(generates)X
1713(a)X
5 f
1792(PropertyNotify)X
1 f
2387(event)X
2600(on)X
2710(the)X
2840(window.)X
5 f
570 2224(XGetWindowProperty)N
1 f
1454(allocates)X
1785(one)X
1934(extra)X
2132(byte)X
2306(and)X
2455(sets)X
2609(it)X
2681(to)X
2772(ASCII)X
3023(null,)X
3205(so)X
3305(that)X
3460(simple)X
3718(proper-)X
555 2320(ties)N
700(consisting)X
1080(of)X
1175(characters)X
1553(do)X
1663(not)X
1798(have)X
1986(to)X
2077(be)X
2182(copied)X
2439(into)X
2599(yet)X
2729(another)X
3015(string)X
3238(before)X
3484(use.)X
3667(The)X
3826(func-)X
555 2416(tion)N
715(returns)X
5 f
999(Success)X
1 f
1369(if)X
1445(it)X
1517(executes)X
1842(successfully.)X
555 2540(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGetWindowProperty)X
1 f
2720(are)X
5 f
2867(BadAtom)X
1 f
3245(,)X
5 f
3307(BadValue)X
1 f
3704(,)X
3748(and)X
5 f
570 2636(BadWindow)N
1 f
1054(.)X
555 2856(Use)N
5 f
731(XListProperties)X
1 f
1365(to)X
1456(obtain)X
1699(a)X
1760(property)X
2080(list)X
2211(for)X
2335(a)X
2396(speci\256ed)X
2731(window.)X
3058(The)X
3217(de\256nition)X
3578(for)X
3702(this)X
555 2952(function)N
871(is:)X
555 3076(Atom)N
778(*XListProperties)X
1387(\()X
2 f
1423(display)X
1 f
1678(,)X
2 f
1722(w)X
1 f
1788(,)X
2 f
1832(num_prop_return)X
1 f
2464(\))X
687 3172(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3268(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3364(int)N
803(*)X
2 f
(num_prop_return)S
1 f
1479(;)X
2 f
555 3516(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3640(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(property)X
3325(list)X
3456(you)X
3610(want)X
3803(to)X
1131 3736(obtain.)N
2 f
555 3860(num_prop_return)N
1 f
1131 3956(Returns)N
1427(the)X
1557(length)X
1800(of)X
1895(the)X
2025(properties)X
2399(array.)X
555 4080(The)N
5 f
732(XListProperties)X
1 f
1366(function)X
1682(returns)X
1948(a)X
2009(pointer)X
2281(to)X
2372(an)X
2477(array)X
2679(of)X
2774(atom)X
2973(properties)X
3347(that)X
3502(are)X
3631(de\256ned)X
555 4176(for)N
679(the)X
809(speci\256ed)X
1144(window.)X
1493(Use)X
5 f
1669(XFree)X
1 f
1946(to)X
2037(free)X
2195(the)X
2325(memory)X
2641(allocated)X
2982(by)X
3092(this)X
3242(function.)X
3602(\(See)X
3780(Sec-)X
555 4272(tion)N
715(2.4)X
847(for)X
971(further)X
1232(information.\))X
555 4396(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XListProperties)X
1 f
2436(is)X
5 f
2535(BadWindow)X
1 f
3019(.)X
555 4616(Use)N
5 f
731(XChangeProperty)X
1 f
1468(to)X
1559(change)X
1830(the)X
1960(property)X
2280(for)X
2404(a)X
2465(speci\256ed)X
2800(window.)X
3127(The)X
3286(de\256nition)X
3647(for)X
3771(this)X
555 4712(function)N
871(is:)X
555 4836(XChangeProperty)N
1197(\()X
2 f
1233(display)X
1 f
1488(,)X
2 f
1532(w)X
1 f
1598(,)X
2 f
1642(property)X
1 f
1952(,)X
2 f
1996(type)X
1 f
2150(,)X
2 f
2194(format)X
1 f
2436(,)X
2 f
2480(mode)X
1 f
2677(,)X
2 f
2721(data)X
1 f
2885(,)X
2 f
2929(nelements)X
1 f
3288(\))X
687 4932(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5028(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 5124(Atom)N
2 f
910(property)X
1 f
1220(,)X
2 f
1264(type)X
1 f
1418(;)X
687 5220(int)N
2 f
803(format)X
1 f
1045(;)X
687 5316(int)N
2 f
803(mode)X
1 f
1000(;)X
687 5412(unsigned)N
1027(char)X
1200(*)X
2 f
(data)S
1 f
1408(;)X
687 5508(int)N
2 f
803(nelements)X
1 f
1162(;)X
2 f
555 5660(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2239 6144(47)N

48 p
%%Page: 48 52
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(property)X
3325(you)X
3479(want)X
3672(to)X
1131 768(change.)N
2 f
555 892(property)N
1 f
1131(Speci\256es)X
1471(the)X
1601(property)X
1921(atom.)X
2164(The)X
2323(property)X
2643(remains)X
2944(de\256ned)X
3225(even)X
3413(after)X
3596(the)X
3726(client)X
1131 988(who)N
1304(de\256ned)X
1585(it)X
1657(goes)X
1840(away.)X
2091(The)X
2250(property)X
2570(becomes)X
2900(unde\256ned)X
3269(if)X
3345(the)X
3475(application)X
1131 1084(calls)N
5 f
1333(XDeleteProperty)X
1 f
1994(,)X
2038(if)X
2114(the)X
2244(application)X
2659(destroys)X
2974(the)X
3104(speci\256ed)X
3439(window,)X
3766(or)X
3861(if)X
1131 1180(the)N
1261(application)X
1676(closes)X
1913(the)X
2043(last)X
2188(connection)X
2597(to)X
2688(the)X
2818(X)X
2903(server.)X
2 f
555 1304(type)N
1 f
1131(Speci\256es)X
1471(the)X
1601(type)X
1775(of)X
1870(the)X
2000(property.)X
2364(The)X
2523(X)X
2608(server)X
2844(does)X
3027(not)X
3162(interpret)X
3483(the)X
3613(type)X
3787(but)X
1131 1400(simply)N
1394(passes)X
1640(it)X
1712(back)X
1900(to)X
1991(an)X
2096(application)X
2511(that)X
2666(later)X
2845(calls)X
5 f
3047(XGetProperty)X
1 f
3597(.)X
2 f
555 1524(format)N
1 f
1131(Speci\256es)X
1471(whether)X
1776(the)X
1906(data)X
2075(should)X
2332(be)X
2437(viewed)X
2713(as)X
2808(a)X
2869(list)X
3000(of)X
3095(8-bit,)X
3306(16-bit,)X
3561(or)X
3656(32-bit)X
1131 1620(quantities.)N
1541(This)X
1720(information)X
2159(allows)X
2411(the)X
2541(X)X
2626(server)X
2862(to)X
2953(correctly)X
3288(perform)X
3593(byte-swap)X
1131 1716(operations)N
1520(as)X
1615(necessary.)X
2022(If)X
2102(the)X
2232(format)X
2489(is)X
2570(16-bit)X
2803(or)X
2898(32-bit,)X
3153(you)X
3307(must)X
3501(explicitly)X
3858(cast)X
1131 1812(your)N
1314(data)X
1483(pointer)X
1755(to)X
1846(a)X
1907(\(char)X
2109(*\))X
2204(in)X
2295(the)X
2425(call)X
2575(to)X
5 f
2684(XChangeProperty)X
1 f
3399(.)X
3465(Possible)X
3781(values)X
1131 1908(are)N
1260(8,)X
1348(16,)X
1480(and)X
1629(32.)X
2 f
555 2032(mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mode)X
1819(of)X
1914(the)X
2044(operation.)X
2443(You)X
2616(can)X
2760(pass)X
2933(one)X
3082(of)X
3177(these)X
3380(constants:)X
5 f
3773(Prop-)X
1131 2128(ModeReplace)N
1 f
1688(,)X
5 f
1750(PropModePrepend)X
1 f
2504(,)X
2548(or)X
5 f
2661(PropModeAppend)X
1 f
3386(.)X
2 f
555 2252(data)N
1 f
1131(Speci\256es)X
1471(the)X
1601(property)X
1921(data.)X
2 f
555 2376(nelements)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(elements)X
2323(of)X
2418(the)X
2548(speci\256ed)X
2883(data)X
3052(format)X
3309(\(8-bit,)X
3549(16-bit,)X
3804(or)X
1131 2472(32-bit\).)N
555 2596(The)N
5 f
732(XChangeProperty)X
1 f
1469(function)X
1785(alters)X
1998(the)X
2128(property)X
2448(for)X
2572(the)X
2702(speci\256ed)X
3037(window)X
3342(and)X
3491(causes)X
3742(the)X
3872(X)X
555 2692(server)N
791(to)X
882(generate)X
1202(a)X
5 f
1281(PropertyNotify)X
1 f
1876(event)X
2089(on)X
2199(that)X
2354(window.)X
5 f
2724(XChangeProperty)X
1 f
3461(does)X
3644(the)X
3774(fol-)X
555 2788(lowing)N
822(according)X
1191(to)X
1282(the)X
1412(value)X
1625(you)X
1779(assign)X
2021(to)X
2112(the)X
2242(mode)X
2460(argument:)X
10 f
555 2912(g)N
1 f
775(If)X
855(the)X
985(mode)X
1203(argument)X
1558(is)X
5 f
1657(PropModeReplace)X
1 f
2400(,)X
5 f
2462(XChangeProperty)X
1 f
3199(discards)X
3509(the)X
3639(previous)X
775 3008(property)N
1095(value.)X
10 f
555 3132(g)N
1 f
775(If)X
855(the)X
985(mode)X
1203(argument)X
1558(is)X
5 f
1657(PropModePrepend)X
1 f
2433(or)X
5 f
2546(PropModeAppend)X
1 f
3271(,)X
3315(the)X
3445(type)X
3619(and)X
3768(format)X
775 3228(must)N
969(match)X
1207(the)X
1337(existing)X
1639(property)X
1959(value.)X
2194(Otherwise,)X
2599(a)X
5 f
2678(BadMatch)X
1 f
3112(error)X
3304(is)X
3385(returned.)X
3744(If)X
3824(the)X
775 3324(property)N
1095(is)X
1176(unde\256ned,)X
1567(it)X
1639(is)X
1720(treated)X
1982(as)X
2077(de\256ned)X
2358(with)X
2537(the)X
2667(correct)X
2933(type)X
3107(and)X
3256(format)X
3513(with)X
3692(zero-)X
775 3420(length)N
1018(data.)X
775 3544(For)N
5 f
937(PropModePrepend)X
1 f
1691(,)X
1735(the)X
1865(function)X
2181(inserts)X
2433(the)X
2563(data)X
2732(before)X
2978(the)X
3108(beginning)X
3483(of)X
3578(the)X
3708(existing)X
775 3640(data.)N
988(For)X
5 f
1150(PropModeAppend)X
1 f
1875(,)X
1919(the)X
2049(function)X
2365(appends)X
2675(the)X
2805(data)X
2974(onto)X
3153(the)X
3283(end)X
3432(of)X
3527(the)X
3657(existing)X
775 3736(data.)N
555 3860(The)N
714(lifetime)X
1012(of)X
1107(a)X
1168(property)X
1488(is)X
1569(not)X
1704(tied)X
1859(to)X
1950(the)X
2080(storing)X
2347(client.)X
2610(Properties)X
2989(remain)X
3256(until)X
3441(explicitly)X
555 3956(deleted,)N
854(or)X
949(the)X
1079(window)X
1384(is)X
1465(destroyed,)X
1851(or)X
1946(until)X
2131(the)X
2261(server)X
2497(resets.)X
2763(See)X
2912(Section)X
3199(2.5)X
3331(for)X
3455(a)X
3516(discussion)X
3905(of)X
555 4052(what)N
748(happens)X
1058(when)X
1270(the)X
1400(connection)X
1809(to)X
1900(the)X
2030(X)X
2115(server)X
2351(is)X
2432(closed.)X
2723(The)X
2882(maximum)X
3263(size)X
3422(of)X
3517(a)X
3578(property)X
3898(is)X
555 4148(server)N
791(dependent)X
1175(and)X
1324(may)X
1498(vary)X
1676(dynamically,)X
2157(depending)X
2546(on)X
2656(the)X
2786(amount)X
3073(of)X
3168(memory)X
3484(the)X
3614(server)X
3850(has)X
555 4244(available.)N
555 4368(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XChangeProperty)X
1 f
2989(are)X
5 f
3136(BadWindow)X
1 f
3620(,)X
5 f
3682(BadA-)X
555 4464(tom)N
1 f
717(,)X
5 f
779(BadAlloc)X
1 f
1141(,)X
1185(and)X
5 f
1352(BadValue)X
1 f
1749(.)X
555 4684(Use)N
5 f
731(XRotateWindowProperties)X
1 f
1800(to)X
1891(rotate)X
2114(properties)X
2488(in)X
2579(the)X
2709(properties)X
3083(array.)X
3329(The)X
3488(de\256nition)X
3849(for)X
555 4780(this)N
705(function)X
1021(is:)X
555 4932(XRotateWindowProperties)N
1516(\()X
2 f
1552(display)X
1 f
1807(,)X
2 f
1851(w)X
1 f
1910(,)X
2 f
1954(properties)X
1 f
2316(,)X
2 f
2360(num_prop)X
1 f
2721(,)X
2 f
2765(npositions)X
1 f
3135(\))X
687 5028(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5124(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 5220(Atom)N
2 f
910(properties)X
1 f
1279([])X
1344(;)X
687 5316(int)N
2 f
803(num_prop)X
1 f
1171(;)X
687 5412(int)N
2 f
803(npositions)X
1 f
1173(;)X
2 f
555 5564(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5688(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
3 f
2239 6144(48)N

49 p
%%Page: 49 53
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(properties)N
1 f
1131(Speci\256es)X
1471(the)X
1601(array)X
1803(of)X
1898(properties)X
2272(that)X
2427(are)X
2556(to)X
2647(be)X
2752(rotated.)X
2 f
555 796(num_prop)N
1 f
1131(Speci\256es)X
1471(the)X
1601(length)X
1844(of)X
1939(the)X
2069(properties)X
2443(array.)X
2 f
555 920(npositions)N
1 f
1131(Speci\256es)X
1471(the)X
1601(rotation)X
1898(amount.)X
555 1044(The)N
5 f
732(XRotateWindowProperties)X
1 f
1801(function)X
2117(allows)X
2369(you)X
2523(to)X
2614(rotate)X
2837(properties)X
3211(in)X
3302(the)X
3432(properties)X
3806(array)X
555 1140(and)N
704(causes)X
955(the)X
1085(X)X
1170(server)X
1406(to)X
1497(generate)X
1817(a)X
5 f
1896(PropertyNotify)X
1 f
2491(event.)X
2748(If)X
2828(the)X
2958(property)X
3278(names)X
3525(in)X
3616(the)X
3746(proper-)X
555 1236(ties)N
700(array)X
902(are)X
1031(viewed)X
1307(as)X
1402(being)X
1620(numbered)X
1994(starting)X
2281(from)X
2474(zero)X
2647(and)X
2796(if)X
2872(there)X
3070(are)X
3199(num_prop)X
3583(property)X
555 1332(names)N
802(in)X
893(the)X
1023(list,)X
1176(then)X
1350(the)X
1480(value)X
1693(associated)X
2077(with)X
2256(property)X
2576(name)X
2789(I)X
2840(becomes)X
3170(the)X
3300(value)X
3513(associated)X
555 1428(with)N
734(property)X
1054(name)X
1267(\(I)X
1347(+)X
1419(npositions\))X
1833(mod)X
2012(N,)X
2119(for)X
2243(all)X
2354(I)X
2405(from)X
2598(zero)X
2771(to)X
2862(N)X
2947(-)X
2998(1.)X
3108(The)X
3267(effect)X
3489(is)X
3570(to)X
3661(rotate)X
3884(the)X
555 1524(states)N
773(by)X
883(npositions)X
1268(places)X
1510(around)X
1776(the)X
1906(virtual)X
2159(ring)X
2323(of)X
2418(property)X
2738(names)X
2985(\(right)X
3203(for)X
3327(positive)X
3629(nposi-)X
555 1620(tions,)N
771(left)X
911(for)X
1035(negative)X
1356(npositions\).)X
1814(If)X
1894(npositions)X
2279(mod)X
2458(N)X
2543(is)X
2624(nonzero,)X
2951(the)X
3081(X)X
3166(server)X
3402(generates)X
3756(a)X
5 f
3835(Pro-)X
555 1716(pertyNotify)N
1 f
1013(event)X
1226(for)X
1350(each)X
1533(property)X
1853(in)X
1944(the)X
2074(order)X
2281(that)X
2436(they)X
2610(are)X
2739(listed)X
2953(in)X
3044(the)X
3174(array.)X
3420(If)X
3500(an)X
3605(atom)X
555 1812(occurs)N
806(more)X
1009(than)X
1183(once)X
1371(in)X
1462(the)X
1592(list)X
1723(or)X
1818(no)X
1928(property)X
2248(with)X
2427(that)X
2582(name)X
2795(is)X
2876(de\256ned)X
3157(for)X
3281(the)X
3411(window,)X
3738(a)X
5 f
3817(Bad-)X
555 1908(Match)N
1 f
832(error)X
1024(is)X
1105(generated.)X
1513(If)X
1593(a)X
5 f
1672(BadAtom)X
1 f
2072(or)X
5 f
2185(BadMatch)X
1 f
2619(error)X
2811(is)X
2892(generated,)X
3278(no)X
3388(properties)X
3762(are)X
555 2004(changed.)N
555 2128(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XRotateWindowProperties)X
1 f
3074(is)X
5 f
3173(BadWindow)X
1 f
3657(.)X
555 2348(Use)N
5 f
731(XDeleteProperty)X
1 f
1414(to)X
1505(delete)X
1738(a)X
1799(property)X
2119(for)X
2243(the)X
2373(speci\256ed)X
2708(window.)X
3035(The)X
3194(de\256nition)X
3555(for)X
3679(this)X
555 2444(function)N
871(is:)X
555 2568(XDeleteProperty)N
1158(\()X
2 f
1194(display)X
1 f
1449(,)X
2 f
1493(w)X
1 f
1559(,)X
2 f
1603(property)X
1 f
1913(\))X
687 2664(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2760(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 2856(Atom)N
2 f
910(property)X
1 f
1220(;)X
2 f
555 3008(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3132(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(property)X
3325(you)X
3479(want)X
3672(to)X
3763(delete.)X
2 f
555 3256(property)N
1 f
1131(Speci\256es)X
1471(the)X
1601(property)X
1921(atom.)X
555 3380(The)N
5 f
732(XDeleteProperty)X
1 f
1415(function)X
1731(deletes)X
1998(the)X
2128(speci\256ed)X
2463(property)X
2783(only)X
2962(if)X
3038(the)X
3168(property)X
3488(was)X
3646(de\256ned)X
555 3476(on)N
665(the)X
795(speci\256ed)X
1130(window.)X
5 f
1500(XDeleteProperty)X
1 f
2183(causes)X
2434(the)X
2564(X)X
2649(server)X
2885(to)X
2976(generate)X
3296(a)X
5 f
3375(PropertyNotify)X
1 f
555 3572(event)N
768(on)X
878(the)X
1008(window,)X
1335(unless)X
1577(the)X
1707(property)X
2027(does)X
2210(not)X
2345(exist.)X
555 3696(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDeleteProperty)X
1 f
2519(are)X
5 f
2666(BadWindow)X
1 f
3172(and)X
5 f
3339(BadAtom)X
1 f
3717(.)X
3 f
555 3888(4.4.)N
731(Manipulating)X
1268(Window)X
1608(Selection)X
1 f
555 4012(A)N
640(selection)X
976(can)X
1120(be)X
1225(thought)X
1517(of)X
1612(as)X
1707(an)X
1812(indirect)X
2104(property)X
2424(with)X
2603(a)X
2664(dynamic)X
2990(type.)X
3208(That)X
3392(is,)X
3495(rather)X
3722(than)X
555 4108(having)N
817(the)X
947(property)X
1267(stored)X
1504(in)X
1595(the)X
1725(X)X
1810(server,)X
2068(the)X
2198(property)X
2518(is)X
2599(maintained)X
3014(by)X
3124(some)X
3332(client)X
3551(\(the)X
3710(owner\).)X
555 4204(A)N
640(selection)X
976(is)X
1057(global)X
1300(in)X
1391(nature,)X
1655(being)X
1873(thought)X
2165(of)X
2260(as)X
2355(belonging)X
2730(to)X
2821(the)X
2951(user)X
3119(but)X
3254(maintained)X
3669(by)X
555 4300(clients,)N
830(rather)X
1057(than)X
1231(being)X
1449(private)X
1716(to)X
1807(a)X
1868(particular)X
2228(window)X
2533(subhierarchy)X
3009(or)X
3104(a)X
3165(particular)X
3525(set)X
3645(of)X
3740(clients.)X
555 4396(When)N
787(a)X
848(client)X
1067(asks)X
1240(for)X
1364(the)X
1494(contents)X
1810(of)X
1905(a)X
1966(selection,)X
2324(it)X
2396(speci\256es)X
2721(a)X
2782(selection)X
3118(target)X
3341(type.)X
3559(This)X
3738(target)X
555 4492(type)N
729(can)X
873(be)X
978(used)X
1161(to)X
1252(control)X
1524(the)X
1654(transmitted)X
2074(representation)X
2595(of)X
2690(the)X
2820(contents.)X
3180(For)X
3324(example,)X
3667(if)X
3743(the)X
555 4588(selection)N
891(is)X
972(``the)X
1160(last)X
1305(thing)X
1509(the)X
1639(user)X
1807(clicked)X
2084(on,'')X
2274(and)X
2423(that)X
2578(is)X
2659(currently)X
2999(an)X
3104(image,)X
3364(then)X
3538(the)X
3668(target)X
555 4684(type)N
729(might)X
958(specify)X
1234(whether)X
1539(the)X
1669(contents)X
1985(of)X
2080(the)X
2210(image)X
2448(should)X
2705(be)X
2810(sent)X
2974(in)X
3065(XYFormat)X
3468(or)X
3563(ZFormat.)X
555 4808(The)N
714(target)X
937(type)X
1111(can)X
1255(also)X
1419(be)X
1524(used)X
1707(to)X
1798(control)X
2070(the)X
2200(class)X
2393(of)X
2488(contents)X
2804(transmitted,)X
3246(for)X
3370(example,)X
3713(asking)X
555 4904(for)N
679(the)X
809(``looks'')X
1138(\(fonts,)X
1387(line)X
1542(spacing,)X
1855(indentation,)X
2297(and)X
2446(so)X
2546(forth\))X
2768(of)X
2863(a)X
2924(paragraph)X
3297(selection,)X
3655(not)X
3790(the)X
555 5000(text)N
710(of)X
805(the)X
935(paragraph.)X
1352(The)X
1511(target)X
1734(type)X
1908(can)X
2052(also)X
2216(be)X
2321(used)X
2504(for)X
2628(other)X
2831(purposes.)X
3209(The)X
3368(semantics)X
3738(are)X
3867(not)X
555 5096(constrained)N
983(by)X
1093(the)X
1223(protocol.)X
555 5220(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set,)X
2265(get,)X
2417(or)X
2512(convert)X
2798(window)X
3103(selection.)X
3483(This)X
3662(allows)X
555 5316(applications)N
1004(to)X
1095(implement)X
1496(the)X
1626(notion)X
1874(of)X
1969(current)X
2240(selection,)X
2598(which)X
2835(requires)X
3140(noti\256cation)X
3565(be)X
3670(sent)X
3834(to)X
555 5412(applications)N
1004(when)X
1216(they)X
1390(no)X
1500(longer)X
1747(own)X
1920(the)X
2050(selection.)X
2430(Applications)X
2903(that)X
3058(support)X
3344(selection)X
3680(often)X
555 5508(highlight)N
897(the)X
1027(current)X
1298(selection)X
1634(and)X
1783(need)X
1971(to)X
2062(be)X
2167(able)X
2336(to)X
2427(be)X
2532(informed)X
2877(when)X
3089(some)X
3297(other)X
3500(application)X
555 5604(has)N
694(acquired)X
1019(the)X
1149(selection)X
1485(in)X
1576(order)X
1783(to)X
1874(be)X
1979(able)X
2148(to)X
2239(unhighlight)X
2669(the)X
2799(selection.)X
555 5824(Use)N
5 f
731(XSetSelectionOwner)X
1 f
1581(to)X
1672(set)X
1792(the)X
1922(selection)X
2258(owner.)X
2543(The)X
2702(de\256nition)X
3063(for)X
3187(this)X
3337(function)X
3653(is:)X
3 f
2239 6144(49)N

50 p
%%Page: 50 54
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XSetSelectionOwner)N
1305(\()X
2 f
1341(display)X
1 f
1596(,)X
2 f
1640(selection)X
1 f
1961(,)X
2 f
2005(owner)X
1 f
2232(,)X
2 f
2276(time)X
1 f
2435(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Atom)N
2 f
910(selection)X
1 f
1231(;)X
687 960(Window)N
2 f
1012(owner)X
1 f
1239(;)X
687 1056(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 1208(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1332(selection)N
1 f
1131(Speci\256es)X
1471(the)X
1601(selection)X
1937(atom.)X
2 f
555 1456(owner)N
1 f
1131(Speci\256es)X
1471(the)X
1601(owner)X
1842(of)X
1937(the)X
2067(speci\256ed)X
2402(selection)X
2738(atom.)X
2981(You)X
3154(can)X
3298(pass)X
3471(a)X
3532(window)X
3837(ID)X
1131 1552(or)N
1226(the)X
1356(constant)X
5 f
1690(None)X
1 f
1915(.)X
2 f
555 1676(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 1772(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 1896(The)N
5 f
732(XSetSelectionOwner)X
1 f
1582(function)X
1898(changes)X
2203(the)X
2333(owner)X
2574(and)X
2723(last)X
2868(change)X
3139(time)X
3319(for)X
3443(the)X
3573(speci\256ed)X
555 1992(selection.)N
935(The)X
1094(function)X
1410(has)X
1549(no)X
1659(effect)X
1881(if)X
1957(the)X
2087(value)X
2300(you)X
2454(pass)X
2627(to)X
2718(the)X
2848(time)X
3028(argument)X
3383(is)X
3464(earlier)X
3711(than)X
3885(the)X
555 2088(current)N
826(last-change)X
1249(time)X
1429(of)X
1524(the)X
1654(speci\256ed)X
1989(selection)X
2325(or)X
2420(is)X
2501(later)X
2680(than)X
2854(the)X
2984(current)X
3255(X)X
3340(server)X
3576(time.)X
3800(Oth-)X
555 2184(erwise,)N
828(the)X
958(last-change)X
1381(time)X
1561(is)X
1642(set)X
1762(to)X
1853(the)X
1983(speci\256ed)X
2318(time,)X
2520(with)X
5 f
2717(CurrentTime)X
1 f
3242(replaced)X
3562(by)X
3672(the)X
555 2280(current)N
826(server)X
1062(time.)X
1286(If)X
1366(the)X
1496(owner)X
1737(window)X
2042(is)X
2123(speci\256ed)X
2458(as)X
5 f
2571(None)X
1 f
2796(,)X
2840(then)X
3014(the)X
3144(owner)X
3385(of)X
3480(the)X
3610(selection)X
555 2376(becomes)N
885(None)X
1097(\(that)X
1281(is,)X
1384(no)X
1494(owner\).)X
1808(Otherwise)X
2191(the)X
2321(owner)X
2562(of)X
2657(the)X
2787(selection)X
3123(becomes)X
3453(the)X
3583(client)X
3802(exe-)X
555 2472(cuting)N
798(the)X
928(request.)X
555 2596(If)N
635(the)X
765(new)X
933(owner)X
1174(\(whether)X
1508(a)X
1569(client)X
1788(or)X
5 f
1901(None)X
1 f
2126(\))X
2177(is)X
2258(not)X
2393(the)X
2523(same)X
2726(as)X
2821(the)X
2951(current)X
3222(owner)X
3463(of)X
3558(the)X
3688(selec-)X
555 2692(tion,)N
737(and)X
886(the)X
1016(current)X
1287(owner)X
1528(is)X
1609(not)X
5 f
1762(None)X
1 f
1987(,)X
2031(the)X
2161(current)X
2432(owner)X
2673(is)X
2754(sent)X
2918(a)X
5 f
2997(SelectionClear)X
1 f
3605(event.)X
3862(If)X
555 2788(the)N
685(client)X
904(that)X
1059(is)X
1140(the)X
1270(owner)X
1511(of)X
1606(a)X
1667(selection)X
2003(is)X
2084(later)X
2263(terminated)X
2663(\(that)X
2847(is,)X
2950(its)X
3056(connection)X
3465(is)X
3546(closed\))X
3822(or)X
3917(if)X
555 2884(the)N
685(owner)X
926(window)X
1231(it)X
1303(has)X
1442(speci\256ed)X
1777(in)X
1868(the)X
1998(request)X
2274(is)X
2355(later)X
2534(destroyed,)X
2920(the)X
3050(owner)X
3291(of)X
3386(the)X
3516(selection)X
555 2980 0.2604(automatically)AN
1059(reverts)X
1320(to)X
5 f
1429(None)X
1 f
1654(,)X
1698(but)X
1833(the)X
1963(last-change)X
2386(time)X
2566(is)X
2647(not)X
2782(affected.)X
3131(The)X
3290(selection)X
3626(atom)X
3825(is)X
555 3076(uninterpreted)N
1047(by)X
1157(the)X
1287(X)X
1372(server.)X
1652(The)X
1811(owner)X
2052(window)X
2357(is)X
2438(returned)X
2753(by)X
2863(the)X
5 f
3011(XGetSelectionOwner)X
1 f
555 3172(function)N
871(and)X
1020(is)X
1101(reported)X
1416(in)X
5 f
1525(SelectionRequest)X
1 f
2252(and)X
5 f
2419(SelectionClear)X
1 f
3027(events.)X
3318(Selections)X
3703(are)X
3832(glo-)X
555 3268(bal)N
685(to)X
776(the)X
906(X)X
991(server.)X
555 3392(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetSelectionOwner)X
1 f
2686(are)X
5 f
2833(BadWindow)X
1 f
3339(and)X
5 f
3506(BadAtom)X
1 f
3884(.)X
555 3612(Use)N
5 f
731(XGetSelectionOwner)X
1 f
1591(to)X
1682(return)X
1914(selection)X
2250(owner.)X
2535(The)X
2694(de\256nition)X
3055(for)X
3179(this)X
3329(function)X
3645(is:)X
555 3736(Window)N
880(XGetSelectionOwner)X
1644(\()X
2 f
1680(display)X
1 f
1935(,)X
2 f
1979(selection)X
1 f
2300(\))X
687 3832(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3928(Atom)N
2 f
910(selection)X
1 f
1231(;)X
2 f
555 4080(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4204(selection)N
1 f
1131(Speci\256es)X
1471(the)X
1601(selection)X
1937(atom.)X
2180(This)X
2359(is)X
2440(the)X
2570(atom)X
2769(whose)X
3015(owner)X
3256(you)X
3410(want)X
3603(returned.)X
555 4328(The)N
5 f
732(XGetSelectionOwner)X
1 f
1592(function)X
1908(returns)X
2174(the)X
2304(window)X
2609(ID)X
2723(associated)X
3107(with)X
3286(the)X
3416(window)X
3721(that)X
555 4424(currently)N
895(owns)X
1102(the)X
1232(speci\256ed)X
1567(selection.)X
1947(If)X
2027(no)X
2137(selection)X
2473(was)X
2631(speci\256ed,)X
2988(the)X
3118(function)X
3434(returns)X
3700(the)X
3830(con-)X
555 4520(stant)N
5 f
762(None)X
1 f
987(.)X
1053(If)X
5 f
1151(None)X
1 f
1398(is)X
1479(returned,)X
1816(there)X
2014(is)X
2095(no)X
2205(owner)X
2446(for)X
2570(the)X
2700(selection.)X
555 4644(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XGetSelectionOwner)X
1 f
2662(is)X
5 f
2761(BadAtom)X
1 f
3139(.)X
555 4864(Use)N
5 f
731(XConvertSelection)X
1 f
1497(to)X
1588(request)X
1864(a)X
1925(selection)X
2261(conversion.)X
2713(The)X
2872(de\256nition)X
3233(for)X
3357(this)X
3507(function)X
3823(is:)X
555 4988(XConvertSelection)N
1238(\()X
2 f
1274(display)X
1 f
1529(,)X
2 f
1573(selection)X
1 f
1894(,)X
2 f
1938(target)X
1 f
2156(,)X
2 f
2200(property)X
1 f
2510(,)X
2 f
2554(requestor)X
1 f
2898(,)X
2 f
2942(time)X
1 f
3101(\))X
687 5084(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5180(Atom)N
2 f
910(selection)X
1 f
1231(,)X
2 f
1275(target)X
1 f
1493(;)X
687 5276(Atom)N
2 f
910(property)X
1 f
1220(;)X
687 5372(Window)N
2 f
1012(requestor)X
1 f
1356(;)X
687 5468(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 5620(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5744(selection)N
1 f
1131(Speci\256es)X
1471(the)X
1601(selection)X
1937(atom.)X
3 f
2239 6144(50)N

51 p
%%Page: 51 55
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(target)N
1 f
1131(Speci\256es)X
1471(the)X
1601(target)X
1824(atom.)X
2 f
555 796(property)N
1 f
1131(Speci\256es)X
1471(the)X
1601(property)X
1921(atom.)X
2164(You)X
2337(also)X
2501(can)X
2645(pass)X
2818(the)X
2948(constant)X
5 f
3282(None)X
1 f
3507(.)X
2 f
555 920(requestor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(requestor.)X
2 f
555 1044(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 1140(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
5 f
570 1264(XConvertSelection)N
1 f
1336(requests)X
1646(that)X
1801(the)X
1931(speci\256ed)X
2266(selection)X
2602(be)X
2707(converted)X
3076(to)X
3167(the)X
3297(speci\256ed)X
3632(target)X
555 1360(type:)N
10 f
555 1484(g)N
1 f
775(If)X
855(the)X
985(speci\256ed)X
1320(selection)X
1656(has)X
1795(an)X
1900(owner,)X
2163(the)X
2293(X)X
2378(server)X
2614(sends)X
2831(a)X
5 f
2910(SelectionRequest)X
1 f
3637(event)X
3850(to)X
775 1580(that)N
930(owner.)X
10 f
555 1704(g)N
1 f
775(If)X
855(no)X
965(owner)X
1206(for)X
1330(the)X
1460(speci\256ed)X
1795(selection)X
2131(exists,)X
2376(the)X
2506(X)X
2591(server)X
2827(generates)X
3181(a)X
5 f
3260(SelectionNotify)X
1 f
775 1800(event)N
988(to)X
1079(the)X
1209(requestor)X
1558(with)X
1737(property)X
5 f
2075(None)X
1 f
2300(.)X
2366(The)X
2525(arguments)X
2914(are)X
3043(passed)X
3299(on)X
3409(unchanged)X
3812(in)X
775 1896(either)N
998(event.)X
555 2020(There)N
782(are)X
911(two)X
1065(prede\256ned)X
1458(selection)X
1794(atoms:)X
2052(``PRIMARY'')X
2590(and)X
2739 -0.2404(``SECONDARY''.)AX
3457(See)X
3606(Chapter)X
3907(8)X
555 2116(for)N
679(more)X
882(information)X
1321(on)X
1431(events,)X
1700(and)X
1849(in)X
1940(particular)X
2300(the)X
5 f
2448(SelectionNotify)X
1 f
3072(event.)X
555 2240(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XConvertSelection)X
1 f
2602(are)X
5 f
2749(BadWindow)X
1 f
3255(and)X
5 f
3422(BadAtom)X
1 f
3800(.)X
3 f
2239 6144(51)N

52 p
%%Page: 52 56
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(5)X
1677 1056(Graphics)N
2080(Resource)X
2483(Functions)X
1 f
11 s
555 1372(After)N
762(you)X
916(connect)X
1212(your)X
1395(program)X
1715(to)X
1806(the)X
1936(X)X
2021(server)X
2257(by)X
2367(calling)X
5 f
2648(XOpenDisplay)X
1 f
3225(,)X
3269(you)X
3423(can)X
3567(use)X
3706(the)X
3836(Xlib)X
555 1468(graphics)N
875(resource)X
1194(functions)X
1544(to:)X
10 f
555 1592(g)N
1 f
775(Manipulate)X
1199(the)X
1329(color)X
1532(map)X
10 f
555 1716(g)N
1 f
775(Manipulate)X
1199(pixmaps)X
10 f
555 1840(g)N
1 f
775(Manipulate)X
1199(graphics)X
1519(context/state)X
10 f
555 1964(g)N
1 f
775(Use)X
933(GC)X
1077(convenience)X
1539(routines)X
555 2088(There)N
782(are)X
911(a)X
972(number)X
1263(of)X
1358(resources)X
1711(used)X
1894(when)X
2106(performing)X
2524(graphics)X
2844(operations)X
3233(in)X
3324(X.)X
3453(Most)X
3656(informa-)X
555 2184(tion)N
715(about)X
933(performing)X
1351(graphics)X
1671(\(for)X
1824(example,)X
2167(foreground)X
2579(color,)X
2804(background)X
3241(color,)X
3466(line)X
3621(style,)X
3832(and)X
555 2280(so)N
655(on\))X
794(are)X
923(stored)X
1160(in)X
1251(resources)X
1604(called)X
1837(graphics)X
2157(contexts.)X
2517(Most)X
2720(graphics)X
3040(operations)X
3429(\(see)X
3592(chapter)X
3873(6\))X
555 2376(take)N
724(a)X
785(graphics)X
1105(context)X
1387(or)X
1482(``GC'')X
1742(as)X
1837(an)X
1942(argument.)X
2341(While)X
2579(it,)X
2673(in)X
2764(theory,)X
3033(is)X
3114(possible)X
3425(to)X
3516(share)X
3723(GCs)X
555 2472(between)N
870(applications,)X
1341(it)X
1413(is)X
1494(expected)X
1829(that)X
1984(applications)X
2433(will)X
2593(use)X
2732(their)X
2916(own)X
3089(GCs)X
3267(when)X
3479(performing)X
555 2568(operations,)N
966(and)X
1115(such)X
1298(use)X
1437(is)X
1518(highly)X
1766(discouraged)X
2213(because)X
2513(the)X
2643(library)X
2900(may)X
3074(cache)X
3296(GC)X
3440(state.)X
555 2692(Windows)N
914(in)X
1005(X)X
1090(always)X
1356(have)X
1544(an)X
1649(associated)X
2033(color)X
2236(map)X
2410(that)X
2565(provides)X
2890(a)X
2951(level)X
3145(of)X
3240(indirection)X
3645(between)X
555 2788(pixel)N
754(values)X
1001(and)X
1150(color)X
1353(displayed)X
1713(on)X
1823(the)X
1953(screen.)X
2243(Many)X
2470(of)X
2565(the)X
2695(hardware)X
3043(displays)X
3354(built)X
3539(today)X
3757(have)X
3945(a)X
555 2884(single)N
788(color)X
991(map,)X
1187(so)X
1287(the)X
1417(primitives)X
1798(are)X
1927(written)X
2199(to)X
2290(encourage)X
2673(sharing)X
2954(of)X
3049(color)X
3252(map)X
3426(entries)X
3683(between)X
555 2980(applications.)N
1048(Because)X
1363(color)X
1566(maps)X
1774(are)X
1903(associated)X
2287(with)X
2466(windows,)X
2827(X)X
2912(will)X
3072(support)X
3358(displays)X
3669(with)X
555 3076(multiple)N
873(color)X
1076(maps)X
1284(and,)X
1455(indeed,)X
1734(different)X
2059(types)X
2267(of)X
2362(color)X
2565(maps.)X
2817(If)X
2897(there)X
3095(are)X
3224(not)X
3359(suf\256cient)X
3709(color)X
555 3172(map)N
729(resources)X
1082(in)X
1173(the)X
1303(display,)X
1602(some)X
1810(windows)X
2149(may)X
2323(not)X
2458(be)X
2563(displayed)X
2923(in)X
3014(their)X
3198(true)X
3357(colors.)X
3638(A)X
3723(window)X
555 3268(manager)N
880(can)X
1024(set)X
1144(which)X
1381(windows)X
1720(are)X
1849(displayed)X
2209(in)X
2300(their)X
2484(true)X
2643(colors)X
2880(if)X
2956(more)X
3159(than)X
3333(one)X
3482(color)X
3685(map)X
3859(is)X
555 3364(required)N
870(for)X
994(the)X
1124(color)X
1327(resources)X
1680(the)X
1810(applications)X
2259(are)X
2388(using.)X
555 3488(Off)N
698(screen)X
944(memory)X
1260(or)X
1355(pixmaps)X
1676(are)X
1805(often)X
2008(used)X
2191(to)X
2282(de\256ne)X
2519(frequently)X
2903(used)X
3086(images)X
3358(for)X
3482(later)X
3661(use)X
3800(in)X
555 3584(graphics)N
875(operations.)X
1308(Pixmaps)X
1634(are)X
1763(also)X
1927(used)X
2110(to)X
2201(de\256ne)X
2438(tiles)X
2608(or)X
2703(patterns)X
3004(for)X
3128(use)X
3267(as)X
3362(window)X
3667(back-)X
555 3680(grounds,)N
882(borders,)X
1189(or)X
1284(cursors.)X
1603(A)X
1688(single)X
1921(bit)X
2037(plane)X
2250(pixmap)X
2537(is)X
2618(sometimes)X
3018(referred)X
3317(to)X
3408(as)X
3503(a)X
3564(bitmap.)X
555 3776(There)N
782(may)X
956(not)X
1091(be)X
1196(an)X
1301(unlimited)X
1663(amount)X
1950(of)X
2045(off)X
2169(screen)X
2415(memory,)X
2753(so)X
2853(it)X
2925(should)X
3182(be)X
3287(regarded)X
3616(as)X
3711(a)X
3772(pre-)X
555 3872(cious)N
763(resource.)X
555 3996(Graphics)N
894(operations)X
1283(can)X
1427(be)X
1532(performed)X
1920(to)X
2011(either)X
2234(windows)X
2573(or)X
2668(pixmaps,)X
3011(also)X
3175(called)X
3408(drawables,)X
3808(in)X
3899(the)X
555 4092(discussion)N
944(below)X
1181(and)X
1330(in)X
1421(the)X
1551(next)X
1725(chapter.)X
2050(Each)X
2248(drawable)X
2592(exists)X
2815(on)X
2925(a)X
2986(single)X
3219(screen)X
3465(or)X
3560(root)X
3724(and)X
3873(can)X
555 4188(only)N
734(be)X
839(used)X
1022(on)X
1132(that)X
1287(root.)X
1495(GCs)X
1673(can)X
1817(also)X
1981(only)X
2160(be)X
2265(used)X
2448(with)X
2627(drawables)X
3005(of)X
3100(matching)X
3451(roots)X
3649(and)X
555 4284(depths.)N
3 f
555 4476(5.1.)N
731(Manipulating)X
1268(the)X
1407(Color)X
1644(Map)X
1 f
555 4600(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(manipulate)X
2538(a)X
2599(color)X
2802(map.)X
3020(This)X
3199(section)X
3471(discusses)X
3820(how)X
555 4696(to:)N
10 f
555 4820(g)N
1 f
775(Create,)X
1049(copy,)X
1264(and)X
1413(destroy)X
1694(the)X
1824(color)X
2027(map)X
10 f
555 4944(g)N
1 f
775(Allocate)X
1096(and)X
1245(deallocate)X
1625(colors)X
3 f
555 5136(5.1.1.)N
797(Creating,)X
1173(Copying,)X
1535(and)X
1699(Destroying)X
2131(Color)X
2368(Maps)X
1 f
555 5260(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(create,)X
2377(copy,)X
2592(free,)X
2772(or)X
2867(set)X
2987(a)X
3048(color)X
3251(map.)X
555 5384(The)N
714(following)X
1079(functions)X
1429(manipulate)X
1844(the)X
1974(representation)X
2495(of)X
2590(color)X
2793(on)X
2903(the)X
3033(screen.)X
3323(For)X
3467(each)X
3650(possible)X
555 5480(value)N
768(a)X
829(pixel)X
1028(may)X
1202(take)X
1371(on)X
1481(a)X
1542(display,)X
1841(there)X
2039(is)X
2120(a)X
2181(color)X
2384(cell)X
2534(in)X
2625(the)X
2755(color)X
2958(map.)X
3176(For)X
3320(example,)X
3663(if)X
3739(a)X
555 5576(display)N
832(is)X
913(4)X
979(bits)X
1129(deep,)X
1339(pixel)X
1538(values)X
1785(0)X
1851(through)X
2147(15)X
2257(are)X
2386(de\256ned.)X
2689(A)X
2774(color)X
2977(map)X
3151(is)X
3232(the)X
3362(collection)X
3733(of)X
3828(the)X
555 5672(color)N
758(cells.)X
986(A)X
1071(color)X
1274(cell)X
1424(consists)X
1725(of)X
1820(a)X
1881(triple)X
2090(of)X
2185(red,)X
2341(green,)X
2580(and)X
2729(blue.)X
2947(As)X
3066(each)X
3249(pixel)X
3448(is)X
3529(read)X
3702(out)X
3837(of)X
555 5768(display)N
832(memory,)X
1170(its)X
1276(value)X
1489(is)X
1570(taken)X
1783(and)X
1932(looked)X
2194(up)X
2304(in)X
2395(the)X
2525(color)X
2728(map.)X
2946(The)X
3105(values)X
3352(of)X
3447(the)X
3577(cell)X
3 f
2239 6144(52)N

53 p
%%Page: 53 57
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(determine)N
930(what)X
1123(color)X
1326(is)X
1407(displayed)X
1767(on)X
1877(the)X
2007(screen.)X
2297(On)X
2426(a)X
2487(multiplane)X
2910(display)X
3187(with)X
3366(a)X
3427(black)X
3640(and)X
3789(white)X
555 768(monitor)N
857(\(grayscale,)X
1262(but)X
1397(not)X
1532(color\),)X
1786(these)X
1989(values)X
2236(may)X
2410(or)X
2505(may)X
2679(not)X
2814(be)X
2919(combined)X
3289(to)X
3380(determine)X
3755(the)X
555 864(brightness)N
939(on)X
1049(the)X
1179(screen.)X
555 988(Screens)N
850(always)X
1116(have)X
1304(a)X
1365(color)X
1568(map.)X
1786(Programs)X
2145(will)X
2305(typically)X
2637(allocate)X
2934(cells)X
3118(out)X
3253(of)X
3348(a)X
3409(common)X
3740(map.)X
555 1084(It)N
631(is)X
712(highly)X
960(discouraged)X
1407(to)X
1498(write)X
1701(applications)X
2150(which)X
2387(monopolize)X
2826(color)X
3029(resources.)X
3426(On)X
3555(a)X
3616(screen)X
3862(that)X
555 1180(either)N
778(cannot)X
1035(load)X
1209(the)X
1339(color)X
1542(map)X
1716(or)X
1811(cannot)X
2068(have)X
2256(a)X
2317(fully)X
2506(independent)X
2959(color)X
3162(map,)X
3358(only)X
3537(certain)X
3799(kinds)X
555 1276(of)N
650(allocations)X
1055(may)X
1229(work.)X
1475(One)X
1643(or)X
1738(more)X
1941(\(on)X
2080(certain)X
2342(hardware\))X
2719(color)X
2922(maps)X
3130(may)X
3304(be)X
3409(resident)X
3710(at)X
3796(one)X
555 1372(time.)N
779(The)X
5 f
956(XInstallColormap)X
1 f
1662(function)X
1978(\(see)X
2141(Chapter)X
2442(7\))X
2537(is)X
2618(used)X
2801(to)X
2892(install)X
3131(a)X
3192(color)X
3395(map.)X
3613(The)X
5 f
570 1468(DefaultVisual)N
1 f
1125(macro)X
1367(returns)X
1633(the)X
1763(default)X
2030(visual)X
2263(type)X
2437(for)X
2561(the)X
2691(speci\256ed)X
3026(screen.)X
3316(Color)X
3539(maps)X
3747(are)X
555 1564(local)N
749(to)X
840(a)X
901(particular)X
1261(screen.)X
1551(The)X
5 f
1728(DefaultColormap)X
1 f
2424(macro)X
2666(returns)X
2932(the)X
3062(type)X
3236(of)X
3331(color)X
3534(map.)X
3752(Possi-)X
555 1660(ble)N
685(types)X
893(are)X
1022(represented)X
1449(by)X
1559(these)X
1762(constants:)X
5 f
2155(StaticGray)X
1 f
2582(,)X
5 f
2644(GrayScale)X
1 f
3070(,)X
5 f
3132(StaticColor)X
1 f
3577(,)X
5 f
3639(Pseu-)X
555 1756(doColor)N
1 f
877(,)X
5 f
939(TrueColor)X
1 f
1344(,)X
1388(or)X
5 f
1501(DirectColor)X
1 f
1954(.)X
2020(These)X
2252(types)X
2460(are)X
2589(more)X
2792(fully)X
2981(discussed)X
3340(in)X
3431(the)X
3561(section)X
3833(on)X
555 1852(visual)N
788(types)X
996(in)X
1087(Chapter)X
1388(3.)X
2197 2028(Note)N
775 2164(The)N
934(introduction)X
1388(of)X
1483(color)X
1686(changes)X
1991(the)X
2121(view)X
2314(a)X
2375(programmer)X
2832(should)X
3089(take)X
3258(when)X
3470(dealing)X
775 2260(with)N
954(a)X
1015(bitmap)X
1283(display.)X
1604(For)X
1748(example,)X
2091(when)X
2303(printing)X
2605(text,)X
2782(you)X
2936(write)X
3139(in)X
3230(a)X
3291(color)X
3494(\(pixel)X
775 2356(value\))N
1017(rather)X
1244(than)X
1418(setting)X
1676(or)X
1771(clearing)X
2077(bits.)X
2271(Hardware)X
2638(will)X
2798(impose)X
3075(limits)X
3300(\(number)X
3620(of)X
775 2452(signi\256cant)N
1165(bits,)X
1337(for)X
1461(example\))X
1811(on)X
1921(these)X
2124(values.)X
2415(Typically,)X
2798(one)X
2947(allocates)X
3278(particular)X
775 2548(pixel)N
974(values)X
1221(or)X
1316(sets)X
1470(of)X
1565(values.)X
1856(If)X
1936(read)X
2109(only,)X
2310(the)X
2440(pixel)X
2639(values)X
2886(may)X
3060(be)X
3165(shared)X
3416(among)X
775 2644(multiple)N
1093(applications.)X
1586(If)X
1666(read/write,)X
2067(they)X
2241(are)X
2370(exclusively)X
2794(owned)X
3050(by)X
3160(the)X
3290(program,)X
3632(and)X
775 2740(the)N
905(color)X
1108(cell)X
1258(associated)X
1642(with)X
1821(the)X
1951(pixel)X
2150(value)X
2363(may)X
2537(be)X
2642(changed)X
2957(at)X
3043(will.)X
555 2944(The)N
714(functions)X
1064(in)X
1155(this)X
1305(section)X
1577(operate)X
1858(on)X
1968(an)X
5 f
2091(XColor)X
1 f
2396(structure:)X
555 3136(typedef)N
841(struct)X
1059({)X
843 3232(unsigned)N
1183(long)X
1362(pixel;)X
1995(/*)X
2086(pixel)X
2285(value)X
2498(*/)X
843 3328(unsigned)N
1183(short)X
1381(red,)X
1537(green,)X
1776(blue;)X
1995(/*)X
2086(rgb)X
2225(values)X
2472(*/)X
843 3424(char)N
1016(\257ags;)X
1995(/*)X
2086(DoRed,)X
2379(DoGreen,)X
2744(DoBlue)X
3040(*/)X
843 3520(char)N
1016(pad;)X
555 3616(})N
619(XColor;)X
555 3788(The)N
714(red,)X
870(green)X
1087(and)X
1236(blue)X
1410(values)X
1657(are)X
1786(scaled)X
2028(between)X
2343(0)X
2409(and)X
2558(65535.)X
2822(That)X
3006(is,)X
3109(on)X
3219(full)X
3364(in)X
3455(a)X
3516(color)X
3719(is)X
3800(a)X
555 3884(value)N
768(of)X
863(65535)X
1105(independent)X
1558(of)X
1653(the)X
1783(number)X
2074(of)X
2169(bit)X
2285(planes)X
2532(of)X
2627(the)X
2757(display.)X
3078(Half)X
3256(brightness)X
3640(in)X
3731(a)X
3792(color)X
555 3980(would)N
797(be)X
902(a)X
963(value)X
1176(of)X
1271(32767)X
1513(and)X
1662(off)X
1786(would)X
2028(be)X
2133(0.)X
2243(This)X
2422(representation)X
2943(gives)X
3151(uniform)X
3457(results)X
3709(for)X
555 4076(color)N
758(values)X
1005(across)X
1246(displays)X
1557(with)X
1736(different)X
2061(number)X
2352(of)X
2447(bit)X
2563(planes.)X
555 4200(The)N
714(\257ags)X
902(member,)X
1235(which)X
1472(can)X
1616(be)X
1721(one)X
1870(or)X
1965(more)X
2168(of)X
5 f
2281(DoRed)X
1 f
2569(,)X
5 f
2631(DoGreen)X
1 f
3003(,)X
3047(and)X
5 f
3214(DoBlue)X
1 f
3517(,)X
3561(is)X
3642(used)X
3825(in)X
555 4296(some)N
763(functions)X
1113(to)X
1204(control)X
1476(which)X
1713(members)X
2058(will)X
2218(be)X
2323(set.)X
555 4516(Use)N
5 f
731(XCreateColormap)X
1 f
1471(to)X
1562(create)X
1794(a)X
1855(color)X
2058(map)X
2232(for)X
2356(the)X
2486(screen)X
2732(on)X
2842(which)X
3079(the)X
3209(window)X
3514(resides)X
3780(and)X
3929(to)X
555 4612(return)N
787(the)X
917(Colormap)X
1292(ID.)X
1428(The)X
1587(de\256nition)X
1948(for)X
2072(this)X
2222(function)X
2538(is:)X
555 4736(Colormap)N
930(XCreateColormap)X
1583(\()X
2 f
1619(display)X
1 f
1874(,)X
2 f
1918(w)X
1 f
1984(,)X
2 f
2028(visual)X
1 f
2246(,)X
2 f
2290(alloc)X
1 f
2474(\))X
687 4832(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4928(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 5024(Visual)N
939(*)X
2 f
(visual)S
1 f
1201(;)X
687 5120(int)N
2 f
803(alloc)X
1 f
987(;)X
2 f
555 5272(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5396(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(on)X
2869(whose)X
3115(screen)X
3361(you)X
3515(want)X
3708(to)X
3799(create)X
1131 5492(a)N
1192(color)X
1395(map.)X
2 f
555 5616(visual)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(a)X
1956(visual)X
2189(type)X
2363(supported)X
2732(on)X
2842(the)X
2972(screen.)X
3262(If)X
3342(the)X
3472(visual)X
3705(type)X
3879(is)X
1131 5712(not)N
1266(one)X
1415(supported)X
1784(by)X
1894(the)X
2024(screen,)X
2292(the)X
2422(function)X
2738(returns)X
3004(a)X
5 f
3083(BadMatch)X
1 f
3517(error.)X
3 f
2239 6144(53)N

54 p
%%Page: 54 58
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(alloc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(entries)X
2235(to)X
2326(be)X
2431(allocated.)X
2816(You)X
2989(can)X
3133(pass)X
3306(one)X
3455(of)X
3550(these)X
3753(con-)X
1131 768(stants:)N
5 f
1397(AllocNone)X
1 f
1834(or)X
5 f
1947(AllocAll)X
1 f
2249(.)X
555 892(The)N
5 f
732(XCreateColormap)X
1 f
1472(function)X
1788(creates)X
2054(a)X
2115(color)X
2318(map)X
2492(of)X
2587(the)X
2717(speci\256ed)X
3052(visual)X
3285(type)X
3459(for)X
3583(the)X
3713(screen)X
555 988(on)N
665(which)X
902(the)X
1032(window)X
1337(resides)X
1603(and)X
1752(associates)X
2126(the)X
5 f
2274(Colormap)X
1 f
2691(ID)X
2805(with)X
2984(it.)X
5 f
3121(XCreateColormap)X
1 f
555 1084(operates)N
870(on)X
980(a)X
5 f
1059(Visual)X
1 f
1335(structure,)X
1687(whose)X
1933(members)X
2278(contain)X
2560(information)X
2999(about)X
3217(the)X
3347(color)X
3550(mapping)X
555 1180(that)N
710(is)X
791(possible.)X
555 1304(The)N
714(members)X
1059(of)X
1154(this)X
1304(structure)X
1634(pertinent)X
1970(to)X
2061(the)X
2191(discussion)X
2580(of)X
5 f
2693(XCreateColormap)X
1 f
3433(are)X
3562(class,)X
555 1400(red_mask,)N
941(green_mask,)X
1410(blue_mask,)X
1836(and)X
1985(map_entries.)X
2482(The)X
2641(class)X
2834(member)X
3145(speci\256es)X
3470(the)X
3600(screen)X
555 1496(class)N
748(and)X
897(can)X
1041(be)X
1146(one)X
1295(of)X
1390(these)X
1593(constants:)X
5 f
1986(GrayScale)X
1 f
2412(,)X
5 f
2474(PseudoColor)X
1 f
2997(,)X
5 f
3059(DirectColor)X
1 f
3512(,)X
5 f
3574(Sta-)X
555 1592(ticColor)N
1 f
867(,)X
5 f
929(StaticGray)X
1 f
1356(,)X
1400(or)X
5 f
1513(TrueColor)X
1 f
1918(.)X
1984(The)X
2143(red_mask,)X
2529(green_mask,)X
2998(and)X
3147(blue_mask)X
3551(members)X
555 1688(specify)N
831(the)X
961(color)X
1164(mask)X
1372(values.)X
1663(The)X
1822(map_entries)X
2275(member)X
2586(speci\256es)X
2911(the)X
3041(color)X
3244(map)X
3418(entries.)X
3719(The)X
555 1784(class)N
748(member)X
1059(constant)X
1375(determines)X
1784(whether)X
2089(the)X
2219(initial)X
2449(values)X
2696(for)X
2820(map_entries)X
3273(are)X
3402(de\256ned.)X
3727(If)X
3807(the)X
555 1880(class)N
748(member)X
1059(is)X
5 f
1158(GrayScale)X
1 f
1584(,)X
5 f
1646(PseudoColor)X
1 f
2169(,)X
2213(or)X
5 f
2326(DirectColor)X
1 f
2779(,)X
2823(the)X
2953(initial)X
3183(values)X
3430(for)X
3554(map_entries)X
555 1976(are)N
684(unde\256ned.)X
1097(However,)X
1462(if)X
1538(the)X
1668(class)X
1861(member)X
2172(is)X
5 f
2271(StaticColor)X
1 f
2716(,)X
5 f
2778(StaticGray)X
1 f
3205(,)X
3249(or)X
5 f
3362(TrueColor)X
1 f
3767(,)X
555 2072(map_entries)N
1008(has)X
1147(initial)X
1377(values)X
1624(that)X
1779(are)X
1908(de\256ned.)X
2233(These)X
2465(values)X
2712(are)X
2841(speci\256c)X
3132(to)X
3223(the)X
3353(visual)X
3586(type)X
3760(and)X
555 2168(are)N
684(not)X
819(de\256ned)X
1100(by)X
1210(the)X
1340(X)X
1425(server.)X
555 2292(The)N
714(class)X
907(member)X
1218(constant)X
1534(also)X
1698(determines)X
2107(the)X
2237(constant)X
2553(you)X
2707(should)X
2964(pass)X
3137(to)X
3228(the)X
3358(alloc)X
3552(argument:)X
10 f
555 2416(g)N
1 f
775(If)X
855(the)X
985(class)X
1178(member)X
1489(is)X
5 f
1588(StaticGray)X
1 f
2015(,)X
5 f
2077(StaticColor)X
1 f
2522(,)X
2566(or)X
5 f
2679(TrueColor)X
1 f
3106(you)X
3260(must)X
3454(pass)X
5 f
3645(Alloc-)X
775 2512(None)N
1 f
1000(.)X
1066(Otherwise,)X
1471(the)X
1601(function)X
1917(returns)X
2183(a)X
5 f
2262(BadMatch)X
1 f
2696(error.)X
10 f
555 2636(g)N
1 f
775(If)X
855(the)X
985(class)X
1178(member)X
1489(is)X
1570(any)X
1719(other)X
1922(class,)X
2137(you)X
2291(can)X
2435(pass)X
5 f
2626(AllocNone)X
1 f
3041(.)X
3107(In)X
3202(this)X
3352(case,)X
3547(the)X
3677(color)X
775 2732(map)N
949(has)X
1088(no)X
1198(values)X
1445(de\256ned)X
1726(for)X
1850(map_entries.)X
2347(This)X
2526(allows)X
2778(your)X
2961(client)X
3180(programs)X
3534(to)X
3625(allocate)X
775 2828(the)N
905(entries)X
1162(in)X
1253(the)X
1383(color)X
1586(map.)X
1804(You)X
1977(can)X
2121(also)X
2285(pass)X
5 f
2476(AllocAll)X
1 f
2778(.)X
2844(In)X
2939(this)X
3089(case,)X
5 f
3302(XCreateColor-)X
775 2924(map)N
1 f
983(allocates)X
1314(the)X
1444(entire)X
1667(color)X
1870(map)X
2044(as)X
2139(writable.)X
2494(The)X
2653(initial)X
2883(values)X
3130(of)X
3225(all)X
3336(map_entries)X
3789(are)X
775 3020(unde\256ned.)N
1188(You)X
1361(cannot)X
1618(free)X
1776(any)X
1925(of)X
2020(these)X
2223(map_entries)X
2676(with)X
2855(a)X
2916(call)X
3066(to)X
3157(the)X
3287(function)X
5 f
790 3116(XFreeColors)N
1 f
1298(.)X
775 3240(For)N
919(a)X
980(color)X
1183(map)X
1357(class)X
1550(of)X
5 f
1663(GrayScale)X
1 f
2111(or)X
5 f
2224(PseudoColor)X
1 f
2747(,)X
2791(the)X
2921(processing)X
3319(simulates)X
3675(a)X
3736(call)X
3886(to)X
775 3336(the)N
905(function)X
5 f
1239(XAllocColor)X
1 f
1712(,)X
1756(where)X
5 f
2010(XAllocColor)X
1 f
2505(returns)X
2771(all)X
2882(pixel)X
3081(values)X
3328(from)X
3521(zero)X
3694(to)X
3785(N)X
3870(-)X
3921(1.)X
775 3432(The)N
934(value)X
1147(N)X
1232(represents)X
1610(the)X
1740(map_entries)X
2193(value)X
2406(in)X
2497(the)X
2627(speci\256ed)X
5 f
2980(Visual)X
1 f
3256(structure.)X
3630(For)X
3774(a)X
775 3528(color)N
978(map)X
1152(class)X
1345(of)X
5 f
1458(DirectColor)X
1 f
1911(,)X
1955(the)X
2085(processing)X
2483(simulates)X
2839(a)X
2900(call)X
3050(to)X
3141(the)X
3271(function)X
5 f
3605(XAlloc-)X
775 3624(ColorPlanes)N
1 f
1268(,)X
1312(where)X
5 f
1566(XAllocColorPlanes)X
1 f
2330(returns)X
2596(a)X
2657(pixel)X
2856(value)X
3069(of)X
3164(zero)X
3337(and)X
3486(rmask,)X
3745(gmask,)X
775 3720(and)N
924(bmask)X
1176(values)X
1423(containing)X
1818(the)X
1948(same)X
2151(bits)X
2301(as)X
2396(the)X
2526(red_mask,)X
2912(green_mask,)X
3381(and)X
3530(blue_mask)X
775 3816(members)N
1120(in)X
1211(the)X
1341(speci\256ed)X
5 f
1694(Visual)X
1 f
1970(structure.)X
555 3940(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCreateColormap)X
1 f
2992(are)X
5 f
3139(BadWindow)X
1 f
3623(,)X
5 f
3685(BadAl-)X
555 4036(loc)N
1 f
682(,)X
5 f
744(BadMatch)X
1 f
1156(,)X
1200(and)X
5 f
1367(BadValue)X
1 f
1764(.)X
555 4284(Use)N
5 f
731(XCopyColormapAndFree)X
1 f
1750(to)X
1841(obtain)X
2084(a)X
2145(new)X
2313(color)X
2516(map)X
2690(when)X
2902(allocating)X
3273(out)X
3408(of)X
3503(a)X
3564(previously)X
555 4380(shared)N
806(color)X
1009(map)X
1183(has)X
1322(failed)X
1545(due)X
1694(to)X
1785(resource)X
2104(exhaustion.)X
2530(That)X
2714(is,)X
2817(too)X
2952(many)X
3170(cells)X
3354(or)X
3449(planes)X
3696(were)X
3888(in)X
555 4476(use)N
694(in)X
785(the)X
915(original)X
1212(color)X
1415(map.)X
1633(The)X
1792(de\256nition)X
2153(for)X
2277(this)X
2427(function)X
2743(is:)X
555 4600(Colormap)N
930(XCopyColormapAndFree)X
1851(\()X
2 f
1887(display)X
1 f
2142(,)X
2 f
2186(cmap)X
1 f
2383(\))X
687 4696(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4792(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
2 f
555 4944(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5068(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
5 f
570 5192(XCopyColormapAndFree)N
1 f
1567(:)X
10 f
555 5316(g)N
1 f
775(Creates)X
1061(a)X
1122(colormap)X
1477(of)X
1572(the)X
1702(same)X
1905(visual)X
2138(type)X
2312(and)X
2461(for)X
2585(the)X
2715(same)X
2918(screen)X
3164(as)X
3259(the)X
3389(cmap)X
3602(argument)X
775 5412(and)N
924(returns)X
1212(the)X
1342(new)X
1510(colormap)X
1865(ID.)X
10 f
555 5536(g)N
1 f
775(Moves)X
1036(all)X
1147(of)X
1242(the)X
1372(client's)X
1654(existing)X
1956(allocation)X
2327(from)X
2520(cmap)X
2733(to)X
2824(the)X
2954(new)X
3122(colormap)X
3477(with)X
3656(their)X
775 5632(color)N
978(values)X
1225(intact)X
1444(and)X
1593(their)X
1777(read-only)X
2136(or)X
2231(writable)X
2542(characteristics)X
3068(intact)X
3287(and)X
3436(frees)X
3628(those)X
775 5728(entries.)N
1076(Color)X
1299(values)X
1546(in)X
1637(other)X
1840(entries)X
2097(in)X
2188(the)X
2318(new)X
2486(colormap)X
2841(are)X
2970(unde\256ned.)X
3 f
2239 6144(54)N

55 p
%%Page: 55 59
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(If)X
855(cmap)X
1068(was)X
1226(created)X
1502(by)X
1612(the)X
1742(client)X
1961(with)X
2140(the)X
2270(alloc)X
2464(argument)X
2819(set)X
2939(to)X
5 f
3048(AllocAll)X
1 f
3350(,)X
3394(the)X
3524(new)X
3692(color-)X
775 768(map)N
949(is)X
1030(also)X
1194(created)X
1470(with)X
5 f
1667(AllocAll)X
1 f
1991(all)X
2102(color)X
2305(values)X
2552(for)X
2676(all)X
2787(entries)X
3044(are)X
3173(copied)X
3430(from)X
3623(cmap,)X
3858(and)X
775 864(then)N
949(all)X
1060(entries)X
1317(in)X
1408(cmap)X
1621(are)X
1750(freed.)X
10 f
555 988(g)N
1 f
775(If)X
855(cmap)X
1068(was)X
1226(not)X
1361(created)X
1637(by)X
1747(those)X
1955(clients)X
2208(with)X
5 f
2405(AllocAll)X
1 f
2707(,)X
2751(the)X
2881(allocations)X
3286(to)X
3377(be)X
3482(moved)X
3744(are)X
3873(all)X
775 1084(those)N
983(pixels)X
1216(and)X
1365(planes)X
1612(that)X
1767(have)X
1955(been)X
2143(allocated)X
2484(by)X
2594(the)X
2724(client)X
2943(using)X
5 f
3174(XAllocColor)X
1 f
3647(,)X
5 f
3709(XAlloc-)X
775 1180(NamedColor)N
1 f
1282(,)X
5 f
1344 -0.2250(XAllocColorCells)AX
1 f
2011(,)X
2055(or)X
5 f
2168(XAllocColorPlanes)X
1 f
2932(and)X
3081(which)X
3318(have)X
3506(not)X
3641(been)X
3829(freed)X
775 1276(since)N
978(they)X
1152(were)X
1344(allocated.)X
555 1400(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCopyColormapAndFree)X
1 f
2855(are)X
5 f
3002(BadAlloc)X
1 f
3386(and)X
5 f
3553(BadColor)X
1 f
3934(.)X
555 1620(Use)N
5 f
731(XFreeColormap)X
1 f
1388(to)X
1479(delete)X
1712(the)X
1842(association)X
2256(between)X
2571(the)X
2701(Colormap)X
3076(resource)X
3395(ID)X
3509(and)X
3658(the)X
3788(color)X
555 1716(map.)N
751(The)X
910(de\256nition)X
1271(for)X
1395(this)X
1545(function)X
1861(is:)X
555 1840(XFreeColormap)N
1134(\()X
2 f
1170(display)X
1 f
1425(,)X
2 f
1469(cmap)X
1 f
1666(\))X
687 1936(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2032(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
2 f
555 2184(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2308(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2136(This)X
2315(is)X
2396(the)X
2526(color)X
2729(map)X
2903(associated)X
3287(with)X
3466(the)X
3596(resource)X
3915(ID)X
1131 2404(you)N
1285(want)X
1478(to)X
1569(delete.)X
555 2528(The)N
5 f
732(XFreeColormap)X
1 f
1389(function)X
1705(deletes)X
1972(the)X
2102(association)X
2516(between)X
2831(the)X
2961(colormap)X
3316(resource)X
3635(ID)X
3749(and)X
3898(the)X
555 2624(color)N
758(map.)X
976(However,)X
1341(this)X
1491(function)X
1807(has)X
1946(no)X
2056(effect)X
2278(on)X
2388(a)X
2449(default)X
2716(color)X
2919(map)X
3093(for)X
3217(a)X
3278(screen.)X
3568(If)X
3648(cmap)X
3861(is)X
555 2720(an)N
660(installed)X
982(map)X
1156(for)X
1280(a)X
1341(screen)X
1587(it)X
1659(is)X
1740(uninstalled.)X
2194(See)X
5 f
2361(XUninstallColormap)X
1 f
3151(.)X
3217(If)X
3297(cmap)X
3510(is)X
3591(de\256ned)X
3872(as)X
555 2816(the)N
685(color)X
888(map)X
1062(for)X
1186(a)X
1247(window)X
1552(\(by)X
5 f
1709(XCreateWindow)X
1 f
2381(or)X
5 f
2494(XChangeWindowAttributes\))X
1 f
5 f
570 2912(XFreeColormap)N
1 f
1227(changes)X
1532(the)X
1662(color)X
1865(map)X
2039(associated)X
2423(with)X
2602(the)X
2732(window)X
3037(to)X
5 f
3146(None)X
1 f
3393(and)X
3542(generates)X
3896(a)X
5 f
570 3008(ColormapNotify)N
1 f
1212(event.)X
1469(The)X
1628(colors)X
1865(displayed)X
2225(for)X
2349(a)X
2410(window)X
2715(with)X
2894(a)X
2955(colormap)X
3310(of)X
5 f
3423(None)X
1 f
3670(are)X
3799(not)X
555 3104(de\256ned)N
836(by)X
946(X.)X
555 3228(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XFreeColormap)X
1 f
2459(is)X
5 f
2558(BadColor)X
1 f
2939(.)X
555 3448(Use)N
5 f
731(XSetWindowColormap)X
1 f
1652(to)X
1743(set)X
1863(the)X
1993(color)X
2196(map)X
2370(for)X
2494(a)X
2555(speci\256ed)X
2890(window.)X
3217(The)X
3376(de\256nition)X
3737(for)X
3861(this)X
555 3544(function)N
871(is:)X
555 3668(XSetWindowColormap)N
1394(\()X
2 f
1430(display)X
1 f
1685(,)X
2 f
1729(w)X
1 f
1795(,)X
2 f
1839(cmap)X
1 f
2036(\))X
687 3764(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3860(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 3956(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
2 f
555 4108(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4232(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(to)X
2850(which)X
3087(you)X
3241(want)X
3434(to)X
3525(set)X
3645(the)X
3775(color)X
1131 4328(map.)N
2 f
555 4452(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
555 4576(The)N
5 f
732(XSetWindowColormap)X
1 f
1653(function)X
1969(sets)X
2123(the)X
2253(speci\256ed)X
2588(color)X
2791(map)X
2965(of)X
3060(the)X
3190(speci\256ed)X
3525(window.)X
555 4700(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetWindowColormap)X
1 f
2757(are)X
5 f
2904(BadWindow)X
1 f
3388(,)X
5 f
3450(BadColor)X
1 f
3831(,)X
3875(and)X
5 f
570 4796(BadMatch)N
1 f
982(.)X
3 f
555 4988(5.1.2.)N
797(Allocating)X
1206(and)X
1370(Freeing)X
1681(Colors)X
1 f
555 5112(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(allocate)X
2420(or)X
2515(deallocate)X
2895(colors.)X
3176(There)X
3403(are)X
3532(two)X
3686(ways)X
3888(of)X
555 5208(allocating)N
926(color)X
1129(cells:)X
1338(explicitly)X
1695(as)X
1790(read)X
1963(only)X
2142(entries)X
2399(by)X
2509(pixel)X
2708(value)X
2921(or)X
3016(read/write,)X
3417(where)X
3653(you)X
3807(can)X
555 5304(allocate)N
852(N)X
937(colors)X
1174(and)X
1323(planes)X
1570(simultaneously.)X
2171(The)X
2330(read/write)X
2709(cells)X
2893(you)X
3047(allocate)X
3344(do)X
3454(not)X
3589(have)X
555 5400(de\256ned)N
836(colors)X
1073(until)X
1258(set)X
1378(with)X
5 f
1575(XStoreColors)X
1 f
2113(.)X
555 5524(To)N
675(determine)X
1050(the)X
1180(color)X
1383(names,)X
1652(the)X
1782(X)X
1867(server)X
2103(uses)X
2276(a)X
2337(color)X
2540(data)X
2709(base.)X
2931(On)X
3060(a)X
3121(UNIX-based)X
3590(system,)X
3879(this)X
555 5620(data)N
724(base)X
902(is)X
5 f
1001(/usr/lib/rgb)X
1 f
1427(,)X
1471(and)X
1620(a)X
1681(printable)X
2017(copy)X
2210(of)X
2305(it)X
2377(is)X
2458(stored)X
2695(in)X
5 f
2804(/usr/lib/rgb.txt)X
1 f
3349(.)X
3415(The)X
3574(name)X
3787(and)X
555 5716(contents)N
871(of)X
966(this)X
1116(\256le)X
1251(are)X
1380(operating)X
1735(system)X
2002(speci\256c.)X
3 f
2239 6144(55)N

56 p
%%Page: 56 60
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Use)N
5 f
731(XAllocColor)X
1 f
1226(to)X
1317(obtain)X
1560(the)X
1690(closest)X
1952(color)X
2155(provided)X
2490(by)X
2600(the)X
2730(hardware.)X
3100(The)X
3259(de\256nition)X
3620(for)X
3744(this)X
555 768(function)N
871(is:)X
555 892(Status)N
793(XAllocColor)X
1260(\()X
2 f
1296(display)X
1 f
1551(,)X
2 f
1595(cmap)X
1 f
1792(,)X
2 f
1836(screen_def_return)X
1 f
2488(\))X
687 988(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1084(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 1180(XColor)N
973(*)X
2 f
(screen_def_return)S
1 f
1669(;)X
2 f
555 1332(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1456(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 1580(screen_def_return)N
1 f
1131 1676(Returns)N
1427(the)X
1557(values)X
1804(actually)X
2106(used)X
2289(in)X
2380(the)X
2510(color)X
2713(map.)X
555 1800(The)N
5 f
732(XAllocColor)X
1 f
1227(function)X
1543(returns)X
1809(the)X
1939(pixel)X
2138(value)X
2351(indicating)X
2727(the)X
2857(closest)X
3119(color)X
3322(supported)X
3691(by)X
3801(the)X
555 1896(hardware.)N
947(It)X
1023(also)X
1187(returns)X
1453(the)X
1583(red,)X
1739(green,)X
1978(and)X
2127(blue)X
2301(values)X
2548(actually)X
2850(used.)X
5 f
3098(XAllocColor)X
1 f
3593(allocates)X
3924(a)X
555 1992(read-only)N
914(color)X
1117(map)X
1291(entry)X
1494(corresponding)X
2019(to)X
2110(the)X
2240(closest)X
2502(red,)X
2658(green,)X
2897(and)X
3046(blue)X
3220(values)X
3467(supported)X
3836(by)X
555 2088(the)N
685(hardware.)X
1077(The)X
1236(corresponding)X
1761(color)X
1964(map)X
2138(cell)X
2288(is)X
2369(read-only.)X
2772(In)X
2867(addition,)X
5 f
3219(XAllocColor)X
1 f
3714(returns)X
555 2184(zero)N
728(\(0\))X
852(if)X
928(there)X
1126(were)X
1318(some)X
1526(problem)X
1842(\(typically)X
2203(lack)X
2372(of)X
2467(resources\))X
2849(or)X
2944(nonzero)X
3249(if)X
3325(it)X
3397(succeeded.)X
555 2280(Read-only)N
944(color)X
1147(map)X
1321(cells)X
1505(are)X
1634(shared)X
1885(among)X
2147(clients.)X
2444(When)X
2676(the)X
2806(last)X
2951(client)X
3170(deallocates)X
3584(a)X
3645(shared)X
555 2376(cell,)N
727(it)X
799(is)X
880(deallocated.)X
555 2500(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XAllocColor)X
1 f
2331(are)X
5 f
2478(BadAlloc)X
1 f
2862(and)X
5 f
3029(BadColor)X
1 f
3410(.)X
555 2720(Use)N
5 f
731(XAllocNamedColor)X
1 f
1509(to)X
1600(obtain)X
1843(the)X
1973(color)X
2176(de\256nition)X
2537(structure)X
2867(for)X
2991(a)X
3052(speci\256ed)X
3387(color)X
3590(and)X
3739(the)X
555 2816(closest)N
817(color)X
1020(supported)X
1389(by)X
1499(the)X
1629(hardware.)X
2021(The)X
2180(de\256nition)X
2541(for)X
2665(this)X
2815(function)X
3131(is:)X
555 2940(Status)N
793(XAllocNamedColor)X
1514(\()X
2 f
1550(display)X
1 f
1805(,)X
2 f
1849(cmap)X
1 f
2046(,)X
2 f
2090(colorname)X
1 f
2473(,)X
2 f
2517(screen_def_return)X
1 f
3169(,)X
2 f
3213(exact_def_return)X
1 f
3822(\))X
687 3036(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3132(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 3228(char)N
860(*)X
2 f
(colorname)S
1 f
1287(;)X
687 3324(XColor)N
973(*)X
2 f
(screen_def_return)S
1 f
1669(,)X
1713(*)X
2 f
(exact_def_return)S
1 f
2366(;)X
2 f
555 3476(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3600(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 3724(colorname)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(name)X
2017(string)X
2240(\(for)X
2393(example,)X
2736("red"\))X
2971(whose)X
3217(color)X
3420(de\256nition)X
3781(struc-)X
1131 3820(ture)N
1290(you)X
1444(want)X
1637(returned.)X
1996(You)X
2169(should)X
2426(use)X
2565(the)X
2695(ISO)X
2858(Latin-1)X
3140(encoding,)X
3507(and)X
1131 3916(upper/lower)N
1578(case)X
1751(does)X
1934(not)X
2069(matter.)X
2 f
555 4040(screen_def_return)N
1 f
1131 4136(Returns)N
1427(the)X
1557(values)X
1804(actually)X
2106(used)X
2289(in)X
2380(the)X
2510(color)X
2713(map.)X
2 f
555 4260(exact_def_return)N
1 f
1157(Returns)X
1453(the)X
1583(true)X
1742(pixel)X
1941(values)X
2188(that)X
2343(indicate)X
2645(the)X
2775(closest)X
3037(color)X
3240(provided)X
3575(by)X
3685(the)X
1131 4356(hardware)N
1479(for)X
1603(the)X
1733(speci\256ed)X
2068(color)X
2271(name.)X
555 4480(The)N
5 f
732(XAllocNamedColor)X
1 f
1510(function)X
1826(determines)X
2235(the)X
2365(correct)X
2631(color)X
2834(\(shade\))X
3114(for)X
3238(the)X
3368(given)X
3586(screen.)X
5 f
570 4576(XAllocNamedColor)N
1 f
1348(returns)X
1614(zero)X
1787(\(0\))X
1911(when)X
2123(it)X
2195(encounters)X
2598(an)X
2703(error)X
2895(or)X
2990(a)X
3051(nonzero)X
3356(when)X
3568(it)X
3640(succeeds.)X
555 4672(Both)N
749(the)X
879(exact)X
1087(data)X
1256(base)X
1434(de\256nition)X
1795(and)X
1944(the)X
2074(closest)X
2336(color)X
2539(supported)X
2908(by)X
3018(the)X
3148(hardware)X
3496(are)X
3625(returned.)X
555 4796(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XAllocNamedColor)X
1 f
2614(are)X
5 f
2761(BadColor)X
1 f
3142(,)X
5 f
3204(BadAlloc)X
1 f
3566(,)X
3610(and)X
5 f
3777(Bad-)X
555 4892(Name)N
1 f
804(.)X
555 5112(Use)N
5 f
731(XLookupColor)X
1 f
1325(to)X
1416(look)X
1595(up)X
1705(the)X
1835(name)X
2048(of)X
2143(a)X
2204(color.)X
2451(The)X
2610(de\256nition)X
2971(for)X
3095(this)X
3245(function)X
3561(is:)X
555 5236(Status)N
793(XLookupColor)X
1338(\()X
2 f
1374(display)X
1 f
1629(,)X
2 f
1673(cmap)X
1 f
1863(,)X
2 f
1907(colorname)X
1 f
2283(,)X
2 f
2327(screen_def_return)X
1 f
2972(,)X
2 f
3016(exact_def_return)X
1 f
3625(\))X
687 5332(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5428(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 5524(char)N
860(*)X
2 f
(colorname)S
1 f
1287(;)X
687 5620(XColor)N
973(*)X
2 f
(screen_def_return)S
1 f
1662(,)X
1706(*)X
2 f
(exact_def_return)S
1 f
2359(;)X
3 f
2239 6144(56)N

57 p
%%Page: 57 61
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 796(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 920(colorname)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(name)X
2017(string)X
2240(\(for)X
2393(example,)X
2736("red"\))X
2971(whose)X
3217(color)X
3420(de\256nition)X
3781(struc-)X
1131 1016(ture)N
1290(you)X
1444(want)X
1637(returned.)X
1996(You)X
2169(should)X
2426(use)X
2565(the)X
2695(ISO)X
2858(Latin-1)X
3140(encoding,)X
3507(and)X
1131 1112(upper/lower)N
1578(case)X
1751(does)X
1934(not)X
2069(matter.)X
2 f
555 1236(screen_def_return)N
1 f
1131 1332(Returns)N
1427(the)X
1557(values)X
1804(actually)X
2106(used)X
2289(in)X
2380(the)X
2510(color)X
2713(map.)X
2 f
555 1456(exact_def_return)N
1 f
1157(Returns)X
1453(the)X
1583(true)X
1742(pixel)X
1941(values)X
2188(that)X
2343(indicate)X
2645(the)X
2775(closest)X
3037(color)X
3240(provided)X
3575(by)X
3685(the)X
1131 1552(hardware)N
1479(for)X
1603(the)X
1733(speci\256ed)X
2068(color)X
2271(name.)X
555 1676(The)N
5 f
732(XLookupColor)X
1 f
1326(function)X
1642(looks)X
1855(up)X
1965(the)X
2095(string)X
2318(name)X
2531(of)X
2626(a)X
2687(color)X
2890(with)X
3069(respect)X
3340(to)X
3431(the)X
3561(screen)X
3807(asso-)X
555 1772(ciated)N
788(with)X
967(the)X
1097(speci\256ed)X
1432(cmap)X
1645(and)X
1794(returns)X
2060(both)X
2239(the)X
2369(exact)X
2577(color)X
2780(values)X
3027(and)X
3176(the)X
3306(closest)X
3568(values)X
3815(pro-)X
555 1868(vided)N
773(by)X
883(the)X
1013(hardware)X
1361(with)X
1540(respect)X
1811(to)X
1902(the)X
2032(visual)X
2265(type)X
2439(of)X
2534(cmap.)X
2791(You)X
2964(should)X
3221(use)X
3360(the)X
3490(ISO)X
3653(Latin-1)X
555 1964(encoding)N
900(for)X
1024(the)X
1154(name,)X
1389(and)X
1538(upper/lower)X
1985(case)X
2158(does)X
2341(not)X
2476(matter.)X
2768(In)X
2863(addition,)X
5 f
3215(XLookupColor)X
1 f
555 2060(returns)N
821(nonzero)X
1126(if)X
1202(the)X
1332(spec)X
1510(existed)X
1782(in)X
1873(the)X
2003(RGB)X
2206(data)X
2375(base)X
2553(or)X
2648(zero)X
2821(\(0\))X
2945(if)X
3021(it)X
3093(did)X
3228(not)X
3363(exist.)X
555 2184(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XLookupColor)X
1 f
2430(are)X
5 f
2577(BadColor)X
1 f
2980(and)X
5 f
3147(BadName)X
1 f
3553(.)X
555 2404(Use)N
5 f
731(XStoreColors)X
1 f
1291(to)X
1382(set)X
1502(the)X
1632(colors)X
1869(of)X
1964(the)X
2094(speci\256ed)X
2429(pixel)X
2628(values)X
2875(to)X
2966(the)X
3096(closest)X
3358(available)X
555 2500(hardware)N
903(colors.)X
1184(The)X
1343(de\256nition)X
1704(for)X
1828(this)X
1978(function)X
2294(is:)X
555 2624(XStoreColors)N
1046(\()X
2 f
1082(display)X
1 f
1337(,)X
2 f
1381(cmap)X
1 f
1578(,)X
2 f
1622(defs)X
1 f
1771(,)X
2 f
1815(ncolors)X
1 f
2086(\))X
687 2720(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2816(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 2912(XColor)N
2 f
973(defs)X
1 f
1122([)X
1158(])X
1194(;)X
687 3008(int)N
2 f
803(ncolors)X
1 f
1074(;)X
2 f
555 3160(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3284(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 3408(defs)N
1 f
1131(Speci\256es)X
1471(an)X
1576(array)X
1778(of)X
1873(color)X
2076(de\256nition)X
2437(structures.)X
2 f
555 3532(ncolors)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
5 f
2005(XColor)X
1 f
2310(structures)X
2674(in)X
2765(the)X
2895(color)X
3098(de\256nition)X
3459(array.)X
555 3656(The)N
5 f
732(XStoreColors)X
1 f
1292(function)X
1608(changes)X
1913(the)X
2043(color)X
2246(map)X
2420(entries)X
2677(of)X
2772(the)X
2902(pixel)X
3101(values)X
3348(speci\256ed)X
3683(in)X
3774(the)X
555 3752(pixel)N
754(members)X
1099(of)X
1194(the)X
5 f
1342(XColor)X
1 f
1647(structures.)X
2055(You)X
2228(specify)X
2504(which)X
2741(color)X
2944(components)X
3392(to)X
3483(be)X
3588(changed)X
3903(by)X
555 3848(passing)N
841(the)X
971(constants)X
5 f
1339(DoRed)X
1 f
1627(,)X
5 f
1689(DoGreen)X
1 f
2061(,)X
2105(and/or)X
5 f
2370(DoBlue)X
1 f
2695(to)X
2786(the)X
2916(\257ags)X
3104(members)X
3449(of)X
3544(the)X
5 f
3692(XColor)X
1 f
555 3944(structures.)N
963(If)X
1043(the)X
1173(colormap)X
1528(is)X
1609(an)X
1714(installed)X
2036(map)X
2210(for)X
2334(its)X
2440(screen,)X
2708(the)X
2838(changes)X
3143(are)X
3272(visible)X
3530 0.2898(immediately.)AX
5 f
570 4040(XStoreColors)N
1 f
1130(changes)X
1435(the)X
1565(speci\256ed)X
1900(pixels)X
2133(if)X
2209(they)X
2383(are)X
2512(allocated)X
2853(writable)X
3164(in)X
3255(cmap)X
3468(by)X
3578(any)X
3727(client,)X
555 4136(even)N
743(if)X
819(one)X
968(or)X
1063(more)X
1266(pixels)X
1499(generates)X
1853(an)X
1958(error.)X
2194(A)X
5 f
2297(BadValue)X
1 f
2716(error)X
2908(is)X
2989(generated)X
3353(if)X
3429(a)X
3490(speci\256ed)X
3825(pixel)X
555 4232(is)N
636(not)X
771(a)X
832(valid)X
1031(index)X
1249(into)X
1409(cmap)X
1622(and)X
1771(a)X
5 f
1850(BadAccess)X
1 f
2328(error)X
2520(is)X
2601(generated)X
2965(if)X
3041(a)X
3102(speci\256ed)X
3437(pixel)X
3636(either)X
3859(is)X
555 4328(unallocated)N
984(or)X
1079(is)X
1160(allocated)X
1501(read-only.)X
1904(If)X
1984(more)X
2187(than)X
2361(one)X
2510(pixel)X
2709(is)X
2790(in)X
2881(error,)X
3095(which)X
3332(one)X
3481(is)X
3562(reported)X
3877(is)X
555 4424(arbitrary.)N
555 4548(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XStoreColors)X
1 f
2812(are)X
5 f
2959(BadAccess)X
1 f
3437(and)X
5 f
3604(BadColor)X
1 f
3985(.)X
555 4768(Use)N
5 f
731(XStoreColor)X
1 f
1247(to)X
1338(set)X
1458(the)X
1588(color)X
1791(of)X
1886(the)X
2016(speci\256ed)X
2351(pixel)X
2550(value)X
2763(to)X
2854(the)X
2984(closest)X
3246(available)X
3587(hardware)X
555 4864(color.)N
780(The)X
939(de\256nition)X
1300(for)X
1424(this)X
1574(function)X
1890(is:)X
555 4988(XStoreColor)N
1012(\()X
2 f
1048(display)X
1 f
1303(,)X
2 f
1347(cmap)X
1 f
1544(,)X
2 f
1588(screen_def_return)X
1 f
2240(\))X
687 5084(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5180(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 5276(XColor)N
973(*)X
2 f
(screen_def_return)S
1 f
1669(;)X
2 f
555 5428(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5552(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 5676(screen_def_return)N
1 f
1131 5772(Returns)N
1427(the)X
1557(values)X
1804(actually)X
2106(used)X
2289(in)X
2380(the)X
2510(color)X
2713(map.)X
3 f
2239 6144(57)N

58 p
%%Page: 58 62
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
5 f
732(XStoreColor)X
1 f
1248(function)X
1564(changes)X
1869(the)X
1999(color)X
2202(map)X
2376(entry)X
2579(of)X
2674(the)X
2804(pixel)X
3003(value)X
3216(speci\256ed)X
3551(in)X
3642(the)X
3772(pixel)X
555 768(member)N
866(of)X
961(the)X
5 f
1109(XColor)X
1 f
1414(structure.)X
5 f
1809(XStoreColor)X
1 f
2325(changes)X
2630(the:)X
10 f
555 892(g)N
1 f
775(Color)X
998(map)X
1172(entry)X
1375(of)X
1470(the)X
1600(pixel)X
1799(value.)X
2034(You)X
2207(speci\256ed)X
2542(this)X
2692(value)X
2905(in)X
2996(the)X
3126(pixel)X
3325(member)X
3636(of)X
3731(the)X
5 f
790 988(XColor)N
1 f
1095(structure.)X
1469(This)X
1648(pixel)X
1847(value)X
2060(must)X
2254(be)X
2359(a)X
2420(read/write)X
2799(cell)X
2949(and)X
3098(a)X
3159(valid)X
3358(index)X
3576(into)X
3736(cmap.)X
775 1084(A)N
5 f
878(BadValue)X
1 f
1297(error)X
1489(is)X
1570(generated)X
1934(if)X
2010(a)X
2071(speci\256ed)X
2406(pixel)X
2605(is)X
2686(not)X
2821(a)X
2882(valid)X
3081(index)X
3299(into)X
3459(cmap.)X
10 f
555 1208(g)N
1 f
775(Red,)X
961(green,)X
1200(and/or)X
1447(blue)X
1621(color)X
1824(components.)X
2316(You)X
2489(specify)X
2765(which)X
3002(color)X
3205(components)X
3653(to)X
3744(be)X
775 1304(changed)N
1090(by)X
1200(passing)X
1486(the)X
1616(constants)X
5 f
1984(DoRed)X
1 f
2272(,)X
5 f
2334(DoGreen)X
1 f
2706(,)X
2750(and/or)X
5 f
3015(DoBlue)X
1 f
3340(to)X
3431(the)X
3561(\257ags)X
775 1400(member)N
1086(of)X
1181(the)X
5 f
1329(XColor)X
1 f
1634(structure.)X
2008(If)X
2088(the)X
2218(color)X
2421(map)X
2595(is)X
2676(an)X
2781(installed)X
3103(map)X
3277(for)X
3401(its)X
3507(screen,)X
3775(the)X
775 1496(changes)N
1080(are)X
1209(visible)X
1467 0.2898(immediately.)AX
10 f
555 1620(g)N
1 f
775(Speci\256ed)X
1125(pixel)X
1324(if)X
1400(it)X
1472(is)X
1553(allocated)X
1894(writable)X
2205(in)X
2296(cmap)X
2509(by)X
2619(any)X
2768(client,)X
3009(even)X
3197(if)X
3273(the)X
3403(pixel)X
3602(generates)X
775 1716(an)N
880(error.)X
555 1840(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XStoreColor)X
1 f
2521(is)X
5 f
2620(BadColor)X
1 f
3001(.)X
555 2060(Use)N
5 f
731 -0.2250(XAllocColorCells)AX
1 f
1420(to)X
1511(allocate)X
1808(color)X
2011(cells.)X
2217(The)X
2376(de\256nition)X
2737(for)X
2861(this)X
3011(function)X
3327(is:)X
555 2184(Status)N
793(XAllocColorCells)X
1442(\()X
2 f
1478(display)X
1 f
1733(,)X
2 f
1777(cmap)X
1 f
1974(,)X
2 f
2018(contig)X
1 f
2246(,)X
2 f
2290(plane_masks_return)X
1 f
3015(,)X
2 f
3059(nplanes)X
1 f
3340(,)X
2 f
1127 2280(pixels_return)N
1 f
1604(,)X
2 f
1648(ncolors)X
1 f
1919(\))X
687 2376(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2472(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 2568(Bool)N
2 f
881(contig)X
1 f
1109(;)X
687 2664(unsigned)N
1027(long)X
2 f
1206(plane_masks_return)X
1 f
1924([)X
1960(])X
1996(;)X
687 2760(unsigned)N
1027(int)X
2 f
1143(nplanes)X
1 f
1424(;)X
687 2856(unsigned)N
1027(long)X
2 f
1206(pixels_return)X
1 f
1676([)X
1712(])X
1748(;)X
687 2952(unsigned)N
1027(int)X
2 f
1143(ncolors)X
1 f
1414(;)X
2 f
555 3104(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3228(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 3352(contig)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value.)X
2090(You)X
2263(pass)X
2436(the)X
2566(value)X
2779(1)X
2845(if)X
2921(the)X
3051(planes)X
3298(must)X
3492(be)X
3597(contiguous)X
1131 3448(or)N
1226(the)X
1356(value)X
1569(0)X
1635(if)X
1711(the)X
1841(planes)X
2088(do)X
2198(not)X
2333(need)X
2521(to)X
2612(be)X
2717(contiguous.)X
2 f
555 3572(plane_mask_return)N
1 f
1131 3668(Returns)N
1427(an)X
1532(array)X
1734(of)X
1829(plane)X
2042(masks.)X
2 f
555 3792(nplanes)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(plane)X
2200(masks)X
2442(that)X
2597(are)X
2726(to)X
2817(be)X
2922(returned)X
3237(in)X
3328(the)X
3458(plane)X
3671(masks)X
1131 3888(array.)N
2 f
555 4012(pixels_return)N
1 f
1131(Returns)X
1427(an)X
1532(array)X
1734(of)X
1829(pixel)X
2028(values.)X
2 f
555 4136(ncolors)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(pixel)X
2186(values)X
2433(that)X
2588(are)X
2717(to)X
2808(be)X
2913(returned)X
3228(in)X
3319(the)X
3449(pixels_return)X
1131 4232(array.)N
555 4356(The)N
5 f
732 -0.2250(XAllocColorCells)AX
1 f
1421(function)X
1737(allocates)X
2068(color)X
2271(cells.)X
2499(The)X
2658(number)X
2949(of)X
3044(colors)X
3281(must)X
3475(be)X
3580(positive)X
3882(and)X
555 4452(the)N
685(number)X
976(of)X
1071(planes)X
1318(nonnegative.)X
1815(Otherwise,)X
2220(a)X
5 f
2299(BadValue)X
1 f
2718(error)X
2910(is)X
2991(generated.)X
3399(If)X
3479(ncolors)X
3760(and)X
555 4548(nplanes)N
846(are)X
975(requested,)X
1356(then)X
1530(ncolors)X
1811(pixels)X
2044(and)X
2193(nplane)X
2450(plane)X
2663(masks)X
2905(are)X
3034(returned.)X
3393(No)X
3522(mask)X
3730(will)X
555 4644(have)N
743(any)X
892(bits)X
1042(in)X
1133(common)X
1464(with)X
1643(any)X
1792(other)X
1995(mask)X
2203(or)X
2298(with)X
2477(any)X
2626(of)X
2721(the)X
2851(pixels.)X
3128(By)X
3253(ORing)X
3510(together)X
555 4740(masks)N
797(and)X
946(pixels,)X
1201(ncolors*)X
10 s
1526(2)X
2 f
7 s
4686(nplanes)Y
10 s
1 f
11 s
1776 4740(distinct)N
2059(pixels)X
2292(can)X
2436(be)X
2541(produced.)X
2934(All)X
3069(of)X
3164(these)X
3367(are)X
3496(allocated)X
3837(writ-)X
555 4836(able)N
724(by)X
834(the)X
964(request.)X
1284(For)X
5 f
1446(GrayScale)X
1 f
1894(or)X
5 f
2007(PseudoColor)X
1 f
2530(,)X
2574(each)X
2757(mask)X
2965(will)X
3125(have)X
3313(exactly)X
3590(one)X
3739(bit,)X
555 4932(and,)N
726(for)X
5 f
868(DirectColor)X
1 f
1321(,)X
1365(each)X
1548(will)X
1708(have)X
1896(exactly)X
2173(three)X
2371(bits.)X
2565(If)X
2645(contigs)X
2922(is)X
5 f
3021(True)X
1 f
3217(,)X
3261(and)X
3410(if)X
3486(all)X
3597(masks)X
3839(are)X
555 5028(ORed)N
782(together,)X
1115(a)X
1176(single)X
1409(contiguous)X
1818(set)X
1938(of)X
2033(bits)X
2183(will)X
2343(be)X
2448(formed)X
2724(for)X
5 f
2866(GrayScale)X
1 f
3314(or)X
5 f
3427(PseudoColor)X
1 f
555 5124(and)N
704(three)X
902(contiguous)X
1311(sets)X
1465(of)X
1560(bits)X
1710(\(one)X
1888(within)X
2136(each)X
2319(pixel)X
2518(sub\256eld\))X
2848(for)X
5 f
2990(DirectColor)X
1 f
3443(.)X
3509(The)X
3668(RGB)X
555 5220(values)N
802(of)X
897(the)X
1027(allocated)X
1368(entries)X
1625(are)X
1754(unde\256ned.)X
555 5344(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252 -0.2250(XAllocColorCells)AX
1 f
2941(are)X
5 f
3088(BadColor)X
1 f
3469(,)X
5 f
3531(BadValue)X
1 f
3928(,)X
555 5440(and)N
5 f
722(BadAlloc)X
1 f
1084(.)X
555 5660(Use)N
5 f
731(XAllocColorPlanes)X
1 f
1495(to)X
1586(allocate)X
1883(color)X
2086(planes.)X
2377(The)X
2536(de\256nition)X
2897(for)X
3021(this)X
3171(function)X
3487(is:)X
3 f
2239 6144(58)N

59 p
%%Page: 59 63
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Status)N
793(XAllocColorPlanes)X
1490(\()X
2 f
1526(display)X
1 f
1781(,)X
2 f
1825(cmap)X
1 f
2022(,)X
2 f
2066(contig)X
1 f
2294(,)X
2 f
2338(pixels_return)X
1 f
2815(,)X
2 f
2859(ncolors)X
1 f
3130(,)X
2 f
3174(nreds)X
1 f
3376(,)X
2 f
3420(ngreens)X
1 f
3705(,)X
2 f
1149 768(nblues)N
1 f
1386(,)X
2 f
1430(rmask_return)X
1 f
1915(,)X
2 f
1959(gmask_return)X
1 f
2454(,)X
2 f
2498(bmask_return)X
1 f
2993(\))X
687 864(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 960(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 1056(Bool)N
2 f
881(contig)X
1 f
1109(;)X
687 1152(unsigned)N
1027(long)X
2 f
1206(pixels_return)X
1 f
1676([)X
1712(])X
1748(;)X
687 1248(int)N
2 f
803(ncolors)X
1 f
1074(;)X
687 1344(int)N
2 f
803(nreds)X
1 f
1005(,)X
2 f
1049(ngreens)X
1 f
1334(,)X
2 f
1378(nblues)X
1 f
1615(;)X
687 1440(unsigned)N
1027(long)X
1206(*)X
2 f
(rmask_return)S
1 f
1735(,)X
1779(*)X
2 f
(gmask_return)S
1 f
2318(,)X
2362(*)X
2 f
(bmask_return)S
1 f
2901(;)X
2 f
555 1592(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1716(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 1840(contig)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value.)X
2090(You)X
2263(pass)X
2436(the)X
2566(value)X
2779(1)X
2845(if)X
2921(the)X
3051(planes)X
3298(must)X
3492(be)X
3597(contiguous)X
1131 1936(or)N
1226(the)X
1356(value)X
1569(0)X
1635(if)X
1711(the)X
1841(planes)X
2088(do)X
2198(not)X
2333(need)X
2521(to)X
2612(be)X
2717(contiguous.)X
2 f
555 2060(pixels_return)N
1 f
1131(Returns)X
1427(an)X
1532(array)X
1734(of)X
1829(pixel)X
2028(values.)X
5 f
2315(XAllocColorPlanes)X
1 f
3079(returns)X
3345(the)X
3475(pixel)X
3674(values)X
3921(in)X
1131 2156(this)N
1281(array.)X
2 f
555 2280(ncolors)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(pixel)X
2186(values)X
2433(that)X
2588(are)X
2717(to)X
2808(be)X
2913(returned)X
3228(in)X
3319(the)X
3449(pixels_return)X
1131 2376(array.)N
2 f
555 2500(nreds)N
555 2596(ngreens)N
555 2692(nblues)N
1 f
1131 2788(Specify)N
1422(the)X
1552(number)X
1843(of)X
1938(red,)X
2094(green,)X
2333(and)X
2482(blue)X
2656(colors)X
2893(\(shades\).)X
3251(The)X
3410(value)X
3623(you)X
3777(pass)X
1131 2884(must)N
1325(be)X
1430(non-negative.)X
2 f
555 3008(rmask_return)N
555 3104(gmask_return)N
555 3200(bmask_return)N
1 f
1131(Return)X
1393(bit)X
1509(masks)X
1751(for)X
1875(the)X
2005(red,)X
2161(green,)X
2400(and)X
2549(blue)X
2723(planes.)X
555 3324(The)N
5 f
732(XAllocColorPlanes)X
1 f
1496(function)X
1812(allocates)X
2143(color)X
2346(planes.)X
2637(The)X
2796(speci\256ed)X
3131(ncolors)X
3412(must)X
3606(be)X
3711(posi-)X
555 3420(tive,)N
732(and)X
881(nreds,)X
1115(ngreens,)X
1432(and)X
1581(nblues)X
1833(must)X
2027(be)X
2132(nonnegative.)X
2629(Otherwise,)X
3034(a)X
5 f
3113(BadValue)X
1 f
3532(error)X
3724(is)X
3805(gen-)X
555 3516(erated.)N
836(If)X
916(ncolors)X
1197(colors,)X
1456(nreds)X
1668(reds,)X
1858(ngreens)X
2153(greens,)X
2426(and)X
2575(nblues)X
2827(blues)X
3035(are)X
3164(requested,)X
3545(ncolors)X
3826(pix-)X
555 3612(els)N
675(are)X
804(returned,)X
1141(and)X
1290(the)X
1420(masks)X
1662(have)X
1850(nreds,)X
2084(ngreens,)X
2401(and)X
2550(nblues)X
2802(bits)X
2952(set)X
3072(respectively.)X
3564(If)X
3644(contigu-)X
555 3708(ous)N
699(is)X
5 f
798(True)X
1 f
994(,)X
1038(each)X
1221(mask)X
1429(will)X
1589(have)X
1777(a)X
1838(contiguous)X
2247(set)X
2367(of)X
2462(bits.)X
2656(No)X
2785(mask)X
2993(will)X
3153(have)X
3341(any)X
3490(bits)X
3640(in)X
3731(com-)X
555 3804(mon)N
734(with)X
913(any)X
1062(other)X
1265(mask)X
1473(or)X
1568(with)X
1747(any)X
1896(of)X
1991(the)X
2121(pixels.)X
2398(For)X
5 f
2560(DirectColor)X
1 f
3013(,)X
3057(each)X
3240(mask)X
3448(will)X
3608(lie)X
3719(within)X
555 3900(the)N
685(corresponding)X
1210(pixel)X
1409(sub\256eld.)X
1754(By)X
1879(ORing)X
2136(together)X
2447(subsets)X
2723(of)X
2818(masks)X
3060(with)X
3239(pixels,)X
555 3996(ncolors*)N
10 s
858(2)X
7 s
3942(\()Y
2 f
917(nreds)X
9 f
1051(+)X
2 f
1082(ngreens)X
9 f
1269(+)X
2 f
1300(nblues)X
1 f
1456(\))X
11 s
1497 3996(distinct)N
1780(pixels)X
2013(can)X
2157(be)X
2262(produced.)X
2633(All)X
2768(of)X
2863(these)X
3066(are)X
3195(allocated)X
3536(by)X
3646(the)X
555 4092(request.)N
875(However,)X
1240(in)X
1331(the)X
1461(colormap,)X
1838(there)X
2036(are)X
2165(only)X
2344(ncolors*)X
10 s
2647(2)X
2 f
7 s
4038(nreds)Y
10 s
1 f
11 s
2847 4092(independent)N
3300(red)X
3434(entries,)X
555 4188(ncolors*)N
10 s
858(2)X
2 f
7 s
4134(ngreens)Y
10 s
1 f
11 s
1111 4188(independent)N
1564(green)X
1781(entries,)X
2060(and)X
2209(ncolors*)X
10 s
2512(2)X
2 f
7 s
4134(nblues)Y
10 s
1 f
11 s
2734 4188(independent)N
3187(blue)X
3361(entries.)X
3640(This)X
3819(is)X
555 4284(true)N
714(even)X
902(for)X
5 f
1044(PseudoColor)X
1 f
1567(.)X
1633(When)X
1865(the)X
1995(colormap)X
2350(entry)X
2553(for)X
2677(a)X
2738(pixel)X
2937(value)X
3150(is)X
3231(changed)X
3546(\(using)X
5 f
570 4380(XStoreColors)N
1 f
1130(or)X
5 f
1243(XStoreNamedColor)X
1 f
2020(\),)X
2093(the)X
2223(pixel)X
2422(is)X
2503(decomposed)X
2965(according)X
3334(to)X
3425(the)X
3555(masks,)X
3819(and)X
555 4476(the)N
685(corresponding)X
1210(independent)X
1663(entries)X
1920(are)X
2049(updated.)X
555 4600(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XAllocColorPlanes)X
1 f
3016(are)X
5 f
3163(BadAlloc)X
1 f
3525(,)X
5 f
3587(BadValue)X
1 f
3984(,)X
555 4696(and)N
5 f
722(BadColor)X
1 f
1103(.)X
555 4916(Use)N
5 f
731(XStoreNamedColor)X
1 f
1530(to)X
1621(allocate)X
1918(a)X
1979(color)X
2182(cell)X
2332(by)X
2442(name.)X
2677(The)X
2836(de\256nition)X
3197(for)X
3321(this)X
3471(function)X
3787(is:)X
555 5040(XStoreNamedColor)N
1266(\()X
2 f
1302(display)X
1 f
1557(,)X
2 f
1601(cmap)X
1 f
1798(,)X
2 f
1842(color)X
1 f
2035(,)X
2 f
2079(pixel)X
1 f
2258(,)X
2 f
2302(\257ags)X
1 f
2475(\))X
687 5136(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5232(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 5328(char)N
860(*)X
2 f
911(color)X
1 f
1104(;)X
687 5424(unsigned)N
1027(long)X
2 f
1206(pixel)X
1 f
1385(;)X
687 5520(int)N
2 f
803(\257ags)X
1 f
976(;)X
2 f
555 5672(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2239 6144(59)N

60 p
%%Page: 60 64
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 796(color)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(name)X
2017(string)X
2240(\(for)X
2393(example,)X
2736(``red''\).)X
3037(The)X
3196(function)X
3512(then)X
3686(allocates)X
1131 892(this)N
1281(color)X
1484(cell.)X
1678(You)X
1851(should)X
2108(use)X
2247(the)X
2377(ISO)X
2540(Latin-1)X
2822(encoding,)X
3189(and)X
3338(upper/lower)X
3785(case)X
1131 988(does)N
1314(not)X
1449(matter.)X
2 f
555 1112(pixel)N
1 f
1131(Speci\256es)X
1471(the)X
1601(entry)X
1804(in)X
1895(the)X
2025(color)X
2228(map.)X
2 f
555 1236(\257ags)N
1 f
1131(Speci\256es)X
1471(which)X
1708(red,)X
1864(green,)X
2103(and)X
2252(blue)X
2426(indexes)X
2717(are)X
2846(set.)X
555 1360(The)N
5 f
732(XStoreNamedColor)X
1 f
1531(function)X
1847(looks)X
2060(up)X
2170(the)X
2300(named)X
2557(color)X
2760(with)X
2939(respect)X
3210(to)X
3301(the)X
3431(screen)X
3677(associ-)X
555 1456(ated)N
724(with)X
903(cmap)X
1116(and)X
1265(stores)X
1492(the)X
1622(result)X
1840(in)X
1931(cmap.)X
2188(The)X
2347(pixel)X
2546(argument)X
2901(determines)X
3310(the)X
3440(entry)X
3643(in)X
3734(the)X
555 1552(color)N
758(map.)X
976(The)X
1135(\257ags)X
1323(argument)X
1678(determines)X
2087(which)X
2324(of)X
2419(the)X
2549(red,)X
2705(green,)X
2944(and)X
3093(blue)X
3267(indexes)X
3558(are)X
3687(set.)X
3829(You)X
555 1648(can)N
699(set)X
819(this)X
969(member)X
1280(to)X
1371(the)X
1501(bitwise)X
1778(inclusive)X
2119(OR)X
2263(of)X
2358(the)X
2488(bits)X
2638(from)X
2831(the)X
2961(constant)X
3277(set)X
5 f
3415(DoRed)X
1 f
3703(,)X
5 f
570 1744(DoGreen)N
1 f
942(,)X
986(and)X
5 f
1153(DoBlue)X
1 f
1456(.)X
1522(A)X
5 f
1625(BadValue)X
1 f
2044(error)X
2236(is)X
2317(generated)X
2681(if)X
2757(a)X
2818(speci\256ed)X
3153(pixel)X
3352(is)X
3433(not)X
3568(a)X
3629(valid)X
555 1840(index)N
773(into)X
933(cmap)X
1146(and)X
1295(a)X
5 f
1374(BadAccess)X
1 f
1852(error)X
2044(is)X
2125(generated)X
2489(if)X
2565(a)X
2626(speci\256ed)X
2961(pixel)X
3160(either)X
3383(is)X
3464(unallocated)X
3893(or)X
555 1936(is)N
636(allocated)X
977(read-only.)X
1380(If)X
1460(more)X
1663(than)X
1837(one)X
1986(pixel)X
2185(is)X
2266(in)X
2357(error,)X
2571(which)X
2808(one)X
2957(is)X
3038(reported)X
3353(is)X
3434(arbitrary.)X
555 2060(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XStoreNamedColor)X
1 f
2635(are)X
5 f
2782(BadColor)X
1 f
3163(,)X
5 f
3225(BadAccess)X
1 f
3681(,)X
5 f
3743(Bad-)X
555 2156(Name)N
1 f
804(,)X
848(and)X
5 f
1015(BadValue)X
1 f
1412(.)X
555 2376(Use)N
5 f
731(XFreeColors)X
1 f
1261(to)X
1352(free)X
1510(color)X
1713(map)X
1887(cells.)X
2093(The)X
2252(de\256nition)X
2613(for)X
2737(this)X
2887(function)X
3203(is:)X
555 2500(XFreeColors)N
1016(\()X
2 f
1052(display)X
1 f
1307(,)X
2 f
1351(cmap)X
1 f
1548(,)X
2 f
1592(pixels)X
1 f
1805(,)X
2 f
1849(npixels)X
1 f
2106(,)X
2 f
2150(planes)X
1 f
2387(\))X
687 2596(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2692(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 2788(unsigned)N
1027(long)X
2 f
1206(pixels[])X
1 f
1487(;)X
687 2884(int)N
2 f
803(npixels)X
1 f
1060(;)X
687 2980(unsigned)N
1027(long)X
2 f
1206(planes)X
1 f
1443(;)X
2 f
555 3132(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3256(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 3380(pixels)N
1 f
1131(Speci\256es)X
1471(an)X
1576(array)X
1778(of)X
1873(pixel)X
2072(values.)X
2341(These)X
2573(pixel)X
2772(values)X
3019(map)X
3193(to)X
3284(the)X
3414(cells)X
3598(in)X
3689(the)X
1131 3476(speci\256ed)N
1466(color)X
1669(map.)X
2 f
555 3600(npixels)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(pixels.)X
2 f
555 3724(planes)N
1 f
1131(Speci\256es)X
1471(the)X
1601(planes)X
1848(you)X
2002(want)X
2195(to)X
2286(free.)X
555 3848(The)N
5 f
732(XFreeColors)X
1 f
1262(function)X
1578(frees)X
1770(the)X
1900(cells)X
2084(represented)X
2511(by)X
2621(pixels)X
2854(whose)X
3100(values)X
3347(are)X
3476(in)X
3567(the)X
3697(pixels)X
555 3944(array.)N
801(The)X
960(planes)X
1207(argument)X
1562(should)X
1819(not)X
1954(have)X
2142(any)X
2291(bits)X
2441(in)X
2532(common)X
2863(with)X
3042(any)X
3191(of)X
3286(the)X
3416(pixels.)X
3671(The)X
3830(set)X
555 4040(of)N
650(all)X
761(pixels)X
994(is)X
1075(produced)X
1424(by)X
1534(ORing)X
1791(together)X
2102(subsets)X
2378(of)X
2473(the)X
2603(planes)X
2850(argument)X
3205(with)X
3384(the)X
3514(pixels.)X
3791(The)X
555 4136(request)N
831(frees)X
1023(all)X
1134(of)X
1229(these)X
1432(pixels)X
1665(that)X
1820(were)X
2012(allocated)X
2353(by)X
2463(the)X
2593(client)X
2812(\(using)X
5 f
3072(XAllocColor)X
1 f
3545(,)X
5 f
3607(XAlloc-)X
555 4232(NamedColor)N
1 f
1062(,)X
5 f
1124 -0.2250(XAllocColorCells)AX
1 f
1791(,)X
1835(and)X
5 f
2002(XAllocColorPlanes)X
1 f
2744(\).)X
2839(Note)X
3032(that)X
3187(freeing)X
3458(an)X
3563(individual)X
555 4328(pixel)N
754(obtained)X
1080(from)X
5 f
1291(XAllocColorPlanes)X
1 f
2055(may)X
2229(not)X
2364(actually)X
2666(allow)X
2884(it)X
2956(to)X
3047(be)X
3152(reused)X
3403(until)X
3588(all)X
3699(of)X
3794(its)X
555 4424(related)N
817(pixels)X
1050(are)X
1179(also)X
1343(freed.)X
555 4548(All)N
690(speci\256ed)X
1025(pixels)X
1258(that)X
1413(are)X
1542(allocated)X
1883(by)X
1993(the)X
2123(client)X
2342(in)X
2433(cmap)X
2646(are)X
2775(freed,)X
2999(even)X
3187(if)X
3263(one)X
3412(or)X
3507(more)X
3710(pixels)X
555 4644(produce)N
860(an)X
965(error.)X
1179(A)X
5 f
1282(BadValue)X
1 f
1701(error)X
1893(is)X
1974(generated)X
2338(if)X
2414(a)X
2475(speci\256ed)X
2810(pixel)X
3009(is)X
3090(not)X
3225(a)X
3286(valid)X
3485(index)X
3703(into)X
555 4740(cmap.)N
812(A)X
5 f
915(BadAccess)X
1 f
1393(error)X
1585(is)X
1666(generated)X
2030(if)X
2106(a)X
2167(speci\256ed)X
2502(pixel)X
2701(is)X
2782(not)X
2917(allocated)X
3258(by)X
3368(the)X
3498(client)X
3717(\(that)X
3901(is,)X
555 4836(is)N
636(unallocated)X
1065(or)X
1160(is)X
1241(only)X
1420(allocated)X
1761(by)X
1871(another)X
2157(client\).)X
2449(If)X
2529(more)X
2732(than)X
2906(one)X
3055(pixel)X
3254(is)X
3335(in)X
3426(error,)X
3640(the)X
3770(one)X
555 4932(reported)N
870(is)X
951(arbitrary.)X
555 5056(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XFreeColors)X
1 f
2366(are)X
5 f
2513(BadColor)X
1 f
2894(,)X
5 f
2956(BadAccess)X
1 f
3412(,)X
3456(and)X
5 f
3623(Bad-)X
555 5152(Value)N
1 f
795(.)X
3 f
555 5344(5.1.3.)N
797(Reading)X
1132(Entries)X
1428(in)X
1524(a)X
1590(Colormap)X
1 f
555 5468(The)N
5 f
732(XQueryColor)X
1 f
1277(function)X
1593(returns)X
1859(the)X
1989(red,)X
2145(green,)X
2384(and)X
2533(blue)X
2707(color)X
2910(values)X
3157(stored)X
3394(in)X
3485(cmap)X
3698(for)X
3822(the)X
555 5564(pixel)N
754(value)X
967(you)X
1121(passed)X
1377(to)X
1468(the)X
1598(pixel)X
1797(member)X
2108(of)X
2203(the)X
5 f
2351(XColor)X
1 f
2656(structure.)X
3030(The)X
3189(values)X
3436(returned)X
3751(for)X
3875(an)X
555 5660(unallocated)N
984(entry)X
1187(are)X
1316(unde\256ned.)X
1729(The)X
5 f
1906(XQueryColors)X
1 f
2495(function)X
2811(does)X
2994(the)X
3124(same)X
3327(thing,)X
3553(except)X
3805(that)X
3960(it)X
555 5756(operates)N
870(on)X
980(an)X
1085(array)X
1287(of)X
5 f
1400(XColor)X
1 f
1705(structures.)X
2113(They)X
2316(also)X
2480(set)X
2600(the)X
2730(\257ags)X
2918(member)X
3229(in)X
3320(the)X
5 f
3468(XColor)X
1 f
3773(struc-)X
555 5852(ture)N
714(to)X
805(all)X
916(three)X
1114(colors.)X
1395(A)X
5 f
1498(BadValue)X
1 f
1917(error)X
2109(is)X
2190(generated)X
2554(if)X
2630(a)X
2691(pixel)X
2890(is)X
2971(not)X
3106(a)X
3167(valid)X
3366(index)X
3584(into)X
3744(cmap.)X
3 f
2239 6144(60)N

61 p
%%Page: 61 65
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(If)N
635(more)X
838(than)X
1012(one)X
1161(pixel)X
1360(is)X
1441(in)X
1532(error,)X
1746(which)X
1983(one)X
2132(is)X
2213(reported)X
2528(is)X
2609(arbitrary.)X
555 892(Use)N
5 f
731(XQueryColor)X
1 f
1276(to)X
1367(obtain)X
1610(the)X
1740(color)X
1943(values)X
2190(for)X
2314(a)X
2375(single)X
2608(speci\256ed)X
2943(pixel)X
3142(value.)X
3377(The)X
3536(de\256nition)X
3897(for)X
555 988(this)N
705(function)X
1021(is:)X
555 1112(XQueryColor)N
1045(\()X
2 f
1081(display)X
1 f
1336(,)X
2 f
1380(cmap)X
1 f
1577(,)X
2 f
1621(def_return)X
1 f
2000(\))X
687 1208(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1304(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 1400(XColor)N
973(*)X
2 f
(def_return)S
1 f
1396(;)X
2 f
555 1552(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1676(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 1800(def_return)N
1 f
1131(Returns)X
1427(the)X
1557(RGB)X
1760(values)X
2007(for)X
2131(the)X
2261(pixel)X
2460(speci\256ed)X
2795(in)X
2886(the)X
3016(structure.)X
555 1924(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XQueryColor)X
1 f
2797(are)X
5 f
2944(BadValue)X
1 f
3363(and)X
5 f
3530(BadColor)X
1 f
3911(.)X
555 2144(Use)N
5 f
731(XQueryColors)X
1 f
1320(to)X
1411(obtain)X
1654(color)X
1857(values)X
2104(for)X
2228(an)X
2333(array)X
2535(of)X
2630(pixels)X
2863(stored)X
3100(in)X
3191(color)X
3394(structures.)X
3802(The)X
555 2240(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 2364(XQueryColors)N
1079(\()X
2 f
1115(display)X
1 f
1370(,)X
2 f
1414(cmap)X
1 f
1611(,)X
2 f
1655(defs_return)X
1 f
2068(,)X
2 f
2112(ncolors)X
1 f
2383(\))X
687 2460(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2556(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
687 2652(XColor)N
2 f
973(defs_return)X
1 f
1379([)X
1415(])X
1451(;)X
687 2748(int)N
2 f
803(ncolors)X
1 f
1074(;)X
2 f
555 2900(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3024(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
2 f
555 3148(defs_return)N
1 f
1131(Returns)X
1427(an)X
1532(array)X
1734(of)X
1829(color)X
2032(de\256nition)X
2393(structures.)X
2 f
555 3272(ncolors)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
5 f
2005(XColor)X
1 f
2310(structures)X
2674(in)X
2765(the)X
2895(color)X
3098(de\256nition)X
3459(array.)X
555 3396(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XQueryColors)X
1 f
2841(are)X
5 f
2988(BadValue)X
1 f
3407(and)X
5 f
3574(BadColor)X
1 f
3955(.)X
3 f
555 3588(5.2.)N
731(Manipulating)X
1268(Pixmaps)X
1 f
555 3712(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(create)X
2355(or)X
2450(free)X
2608(a)X
2669(pixmap.)X
3000(A)X
3085(few)X
3260(programs)X
3614(may)X
3788(want)X
555 3808(to)N
646(manipulate)X
1061(pixels)X
1294(that)X
1449(they)X
1623(later)X
1802(display)X
2079(on)X
2189(the)X
2319(screen.)X
2609(This)X
2788(section)X
3060(describes)X
3409(functions)X
3759(that)X
555 3904(will)N
715(move)X
933(pixels)X
1166(from)X
1359(the)X
1489(program)X
1809(to)X
1900(the)X
2030(window)X
2335(system,)X
2624(or)X
2719(from)X
2912(the)X
3042(window)X
3347(system)X
3614(to)X
3705(the)X
3835(pro-)X
555 4000(gram.)N
802(You)X
975(must)X
1169(be)X
1274(careful)X
1540(to)X
1631(adhere)X
1887(to)X
1978(the)X
2108(data)X
2277(representation)X
2798(described)X
3157(at)X
3243(the)X
3373(beginning)X
3748(of)X
3843(this)X
555 4096(document)N
925(to)X
1016(keep)X
1204(programs)X
1558(portable)X
1869(between)X
2184(machine)X
2505(architectures.)X
3021(Pixmaps)X
3347(can)X
3491(only)X
3670(be)X
3775(used)X
555 4192(on)N
665(the)X
795(screen)X
1041(on)X
1151(which)X
1388(they)X
1562(were)X
1754(created.)X
555 4316(Pixmaps)N
881(are)X
1010(off-screen)X
1387(resources)X
1740(that)X
1895(are)X
2024(used)X
2207(for)X
2331(a)X
2392(number)X
2683(of)X
2778(operations.)X
3211(A)X
3296(bitmap)X
3564(is)X
3645(a)X
3706(single)X
555 4412(bit)N
671(pixmap.)X
1002(These)X
1234(include)X
1516(de\256ning)X
1827(cursors)X
2102(or)X
2197(temporarily)X
2631(saving)X
2883(some)X
3091(part)X
3250(of)X
3345(the)X
3475(screen)X
3721(for)X
555 4508(later,)N
756(either)X
979(as)X
1074(tiling)X
1284(patterns)X
1585(or)X
1680(as)X
1775(the)X
1905(source)X
2156(for)X
2280(certain)X
2542(raster)X
2759(operations.)X
555 4728(Use)N
5 f
731(XCreatePixmap)X
1 f
1384(to)X
1475(create)X
1707(a)X
1768(pixmap)X
2055(of)X
2150(a)X
2211(speci\256ed)X
2546(size.)X
2727(The)X
2886(de\256nition)X
3247(for)X
3371(this)X
3521(function)X
3837(is:)X
555 4852(Pixmap)N
847(XCreatePixmap)X
1417(\()X
2 f
1453(display)X
1 f
1708(,)X
2 f
1752(d)X
1 f
1803(,)X
2 f
1847(width)X
1 f
2051(,)X
2 f
2095(height)X
1 f
2323(,)X
2 f
2367(depth)X
1 f
2570(\))X
687 4948(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5044(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5140(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 5236(unsigned)N
1027(int)X
2 f
1143(depth)X
1 f
1346(;)X
2 f
555 5388(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5512(d)N
1 f
1131(Speci\256es)X
1471(which)X
1708(screen)X
1954(the)X
2084(pixmap)X
2371(is)X
2452(created)X
2728(on.)X
2 f
555 5636(width)N
555 5732(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(dimensions)X
2867(de\256ne)X
3104(the)X
3234(width)X
3457(and)X
3606(height)X
3849(of)X
1131 5828(the)N
1261(pixmap.)X
1592(The)X
1751(values)X
1998(you)X
2152(pass)X
2325(must)X
2519(be)X
2624(nonzero.)X
3 f
2239 6144(61)N

62 p
%%Page: 62 66
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(depth)N
1 f
1131(Speci\256es)X
1471(the)X
1601(depth)X
1819(of)X
1914(the)X
2044(pixmap.)X
2375(The)X
2534(depth)X
2752(must)X
2946(be)X
3051(supported)X
3420(by)X
3530(the)X
3660(root)X
3824(of)X
1131 768(the)N
1261(speci\256ed)X
1596(drawable.)X
555 892(The)N
5 f
732(XCreatePixmap)X
1 f
1385(function)X
1701(creates)X
1967(a)X
2028(pixmap)X
2315(of)X
2410(the)X
2540(width,)X
2785(height,)X
3050(and)X
3199(depth)X
3417(you)X
3571(speci\256ed.)X
3950(It)X
555 988(also)N
719(assigns)X
995(the)X
1125(pixmap)X
1412(ID)X
1526(to)X
1617(it.)X
1733(It)X
1809(is)X
1890(valid)X
2089(to)X
2180(pass)X
2353(a)X
2414(window)X
2719(whose)X
2965(class)X
3158(is)X
5 f
3257(InputOnly)X
1 f
3672(to)X
3763(the)X
555 1084(drawable)N
899(argument.)X
1298(The)X
1457(width)X
1680(and)X
1829(height)X
2072(arguments)X
2461(must)X
2655(be)X
2760(nonzero.)X
3109(Otherwise,)X
3514(a)X
5 f
3593(BadValue)X
1 f
555 1180(error)N
747(is)X
828(generated.)X
1236(The)X
1395(depth)X
1613(argument)X
1968(must)X
2162(be)X
2267(one)X
2416(of)X
2511(the)X
2641(depths)X
2893(supported)X
3262(by)X
3372(the)X
3502(root)X
3666(of)X
3761(the)X
555 1276(speci\256ed)N
890(drawable.)X
1278(Otherwise,)X
1683(a)X
5 f
1762(BadValue)X
1 f
2181(error)X
2373(is)X
2454(generated.)X
555 1400(The)N
714(server)X
950(uses)X
1123(the)X
1253(drawable)X
1597(argument)X
1952(to)X
2043(determine)X
2418(which)X
2655(screen)X
2901(the)X
3031(pixmap)X
3318(is)X
3399(stored)X
3636(on.)X
3790(The)X
555 1496(pixmap)N
842(can)X
986(only)X
1165(be)X
1270(used)X
1453(on)X
1563(this)X
1713(screen)X
1959(and)X
2108(only)X
2287(with)X
2466(other)X
2669(drawables)X
3047(windows)X
3386(of)X
3481(the)X
3611(same)X
555 1592(depth.)N
817(\(See)X
5 f
1013(XCopyPlane)X
1 f
1539(for)X
1663(an)X
1768(exception)X
2133(to)X
2224(this)X
2374(rule\).)X
2606(The)X
2765(initial)X
2995(contents)X
3311(of)X
3406(the)X
3536(pixmap)X
3823(are)X
555 1688(unde\256ned.)N
968(If)X
1048(this)X
1198(routine)X
1470(returns)X
1736(zero)X
1909(\(0\),)X
2055(there)X
2253(was)X
2411(insuf\256cient)X
2830(space)X
3047(for)X
3171(the)X
3301(pixmap.)X
555 1812(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCreatePixmap)X
1 f
2489(are)X
5 f
2636(BadAlloc)X
1 f
2998(,)X
5 f
3060(BadDrawable)X
1 f
3602(,)X
3646(and)X
5 f
3813(Bad-)X
555 1908(Value)N
1 f
795(.)X
555 2128(Use)N
5 f
731(XFreePixmap)X
1 f
1301(to)X
1392(free)X
1550(all)X
1661(storage)X
1937(associated)X
2321(with)X
2500(a)X
2561(speci\256ed)X
2896(pixmap.)X
3205(The)X
3364(de\256nition)X
3725(for)X
3849(this)X
555 2224(function)N
871(is:)X
555 2348(XFreePixmap)N
1051(\()X
2 f
1087(display)X
1 f
1342(,)X
2 f
1386(pixmap)X
1 f
1652(\))X
687 2444(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2540(Pixmap)N
2 f
979(pixmap)X
1 f
1245(;)X
2 f
555 2692(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2816(pixmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(pixmap.)X
555 2940(The)N
5 f
732(XFreePixmap)X
1 f
1302(function)X
1618(\256rst)X
1777(deletes)X
2044(the)X
2174(association)X
2588(between)X
2903(the)X
3033(pixmap)X
3320(ID)X
3434(and)X
3583(the)X
3713(pixmap.)X
555 3036(Then,)N
780(the)X
910(X)X
995(server)X
1231(frees)X
1423(the)X
1553(pixmap)X
1840(storage)X
2116(when)X
2328(no)X
2438(other)X
2641(resources)X
2994(reference)X
3342(it.)X
3458(The)X
3617(pixmap)X
555 3132(should)N
812(never)X
1029(be)X
1134(referenced)X
1526(again.)X
555 3256(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XFreePixmap)X
1 f
2372(is)X
5 f
2471(BadPixmap)X
1 f
2936(.)X
3 f
555 3448(5.3.)N
731(Manipulating)X
1268(Graphics)X
1638(Context/State)X
1 f
555 3572(Most)N
758(attributes)X
1109(of)X
1204(graphics)X
1524(operations)X
1913(are)X
2042(stored)X
2279(in)X
2370(Graphic)X
2675(Contexts)X
3011(or)X
3106(GCs.)X
3328(These)X
3560(include)X
3842(line)X
555 3668(width,)N
800(line)X
955(style,)X
1166(plane)X
1379(mask,)X
1609(foreground,)X
2043(background,)X
2502(tile,)X
2660(stipple,)X
2940(clipping)X
3252(region,)X
3521(end)X
3670(style,)X
555 3764(join)N
715(style,)X
926(and)X
1075(so)X
1175(on.)X
1329(Graphics)X
1668(operations)X
2057(\(for)X
2210(example,)X
2553(drawing)X
2863(lines\))X
3081(use)X
3220(these)X
3423(values)X
3670(to)X
3761(deter-)X
555 3860(mine)N
754(the)X
884(actual)X
1117(drawing)X
1427(operation.)X
1826(Extensions)X
2235(to)X
2326(X)X
2411(may)X
2585(add)X
2734(additional)X
3110(components)X
3558(to)X
3649(GCs.)X
555 3956(Xlib)N
734(provides)X
1059(calls)X
1243(for)X
1367(changing)X
1712(the)X
1842(state)X
2026(of)X
2121(GCs.)X
555 4080(Xlib)N
734(implements)X
1169(a)X
1230(write-back)X
1628(cache)X
1850(for)X
1974(all)X
2085(elements)X
2421(of)X
2516(a)X
2577(GC)X
2721(that)X
2876(are)X
3005(not)X
3140(resource)X
3459(IDs)X
3607(to)X
3698(allow)X
3916(it)X
555 4176(to)N
646(implement)X
1047(the)X
1177(transparent)X
1590(coalescing)X
1984(changes)X
2289(to)X
2380(GCs.)X
2602(GCs)X
2780(are)X
2909(neither)X
3176(expected)X
3511(nor)X
555 4272(encouraged)N
982(to)X
1073(be)X
1178(shared)X
1429(between)X
1744(client)X
1963(applications,)X
2434(so)X
2534(this)X
2684(write-back)X
3082(caching)X
3378(should)X
3635(present)X
3911(no)X
555 4368(problems.)N
949(Applications)X
1422(cannot)X
1679(share)X
1886(GCs)X
2064(without)X
2356(external)X
2662(synchronization.)X
3291(Therefore,)X
3681(sharing)X
555 4464(GCs)N
733(between)X
1048(applications)X
1497(is)X
1578(highly)X
1826(discouraged.)X
555 4588(You)N
728(use)X
867(display)X
1144(functions)X
1494(when)X
1706(you)X
1860(update)X
2139(a)X
2200(section)X
2472(of)X
2567(the)X
2697(screen)X
2943(\(destination\))X
3411(with)X
3590(bits)X
3740(from)X
555 4684(somewhere)N
977(else)X
1136(\(source\).)X
1467(Many)X
1694(GC)X
1838(functions)X
2188(take)X
2357(one)X
2506(of)X
2601(these)X
2804(display)X
3081(functions)X
3431(as)X
3526(an)X
3631(argument.)X
555 4780(The)N
714(function)X
1030(de\256nes)X
1301(how)X
1474(the)X
1604(new)X
1772(destination)X
2182(bits)X
2332(are)X
2461(to)X
2552(be)X
2657(computed)X
3027(from)X
3220(the)X
3350(source)X
3601(bits)X
3751(and)X
3900(the)X
555 4876(old)N
690(destination)X
1100(bits.)X
5 f
1315(GXcopy)X
1 f
1666(is)X
1747(typically)X
2079(the)X
2209(most)X
2403(useful)X
2640(because)X
2940(it)X
3012(will)X
3172(work)X
3374(on)X
3484(a)X
3545(color)X
555 4972(display,)N
854(but)X
989(special)X
1256(applications)X
1705(may)X
1879(use)X
2018(other)X
2221(functions,)X
2593(particularly)X
3022(in)X
3113(concert)X
3394(with)X
3573(particular)X
555 5068(planes)N
802(of)X
897(a)X
958(color)X
1161(display.)X
1482(The)X
1641(16)X
1751(such)X
1934(functions,)X
2306(de\256ned)X
2587(in)X
5 f
2696(<X11/X.h>)X
1 f
3128(,)X
3172(are:)X
10 f
555 5124(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 5268(Function)N
891(Name)X
1277(Hex)X
1445(Code)X
2066(Operation)X
10 f
555 5324(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 5468(GXclear)N
1388(0x0)X
1763(0)X
555 5564(GXand)N
1388(0x1)X
1763(src)X
1887(AND)X
2098(dst)X
555 5660(GXandReverse)N
1388(0x2)X
1763(src)X
1887(AND)X
2098(NOT)X
2300(dst)X
555 5756(GXcopy)N
1388(0x3)X
1763(src)X
555 5852(GXandInverted)N
1388(0x4)X
1763(\(NOT)X
1994(src\))X
2147(AND)X
2358(dst)X
3 f
2239 6144(62)N

63 p
%%Page: 63 67
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 584(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 728(Function)N
891(Name)X
1277(Hex)X
1445(Code)X
2066(Operation)X
10 f
555 784(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 928(GXnoop)N
1388(0x5)X
1763(dst)X
555 1024(GXxor)N
1388(0x6)X
1763(src)X
1887(XOR)X
2094(dst)X
555 1120(GXor)N
1388(0x7)X
1763(src)X
1887(OR)X
2031(dst)X
555 1216(GXnor)N
1388(0x8)X
1763(\(NOT)X
1994(src\))X
2147(AND)X
2358(\(NOT)X
2589(dst\))X
555 1312(GXequiv)N
1388(0x9)X
1763(\(NOT)X
1994(src\))X
2147(XOR)X
2354(dst)X
555 1408(GXinvert)N
1390(0xa)X
1763(NOT)X
1965(dst)X
555 1504(GXorReverse)N
1388(0xb)X
1763(src)X
1887(OR)X
2031(\(NOT)X
2262(dst\))X
555 1600(GXcopyInverted)N
1390(0xc)X
1763(NOT)X
1965(src)X
555 1696(GXorInverted)N
1388(0xd)X
1763(\(NOT)X
1994(src\))X
2147(OR)X
2291(dst)X
555 1792(GXnand)N
1390(0xe)X
1763(\(NOT)X
1994(src\))X
2147(OR)X
2291(\(NOT)X
2522(dst\))X
555 1888(GXset)N
1395(0xf)X
1763(1)X
10 f
555 1944(i)N
565(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 2116(Many)N
782(of)X
877(the)X
1007(color)X
1210(functions)X
1560(below)X
1797(take)X
1966(either)X
2189(pixel)X
2388(values)X
2635(or)X
2730(planes)X
2977(as)X
3072(an)X
3177(argument.)X
3576(The)X
3735(planes)X
555 2212(argument)N
910(is)X
991(of)X
1086(type)X
1260(long)X
1439(and)X
1588(it)X
1660(speci\256es)X
1985(which)X
2222(planes)X
2469(of)X
2564(the)X
2694(display)X
2971(are)X
3100(to)X
3191(be)X
3296(modi\256ed,)X
3654(one)X
3803(bit)X
555 2308(per)N
689(plane.)X
946(A)X
1031(monochrome)X
1518(display)X
1795(has)X
1934(only)X
2113(one)X
2262(plane)X
2475(and)X
2624(will)X
2784(be)X
2889(the)X
3019(least)X
3203(signi\256cant)X
3593(bit)X
3709(of)X
3804(the)X
555 2404(word.)N
801(As)X
920(planes)X
1167(are)X
1296(added)X
1528(to)X
1619(the)X
1749(display)X
2026(hardware,)X
2396(they)X
2570(will)X
2730(occupy)X
3006(more)X
3209(signi\256cant)X
3599(bits)X
3749(in)X
3840(the)X
555 2500(plane)N
768(mask.)X
555 2624(A)N
640(macro)X
882(constant)X
5 f
1216(AllPlanes)X
1 f
1619(can)X
1763(be)X
1868(used)X
2051(to)X
2142(refer)X
2329(to)X
2420(all)X
2531(planes)X
2778(of)X
2873(a)X
2934(display)X
3211(simultaneously)X
3768(\(\3040\).)X
555 2720(Most)N
758(operations)X
1147(use)X
1286(an)X
1391(object)X
1629(called)X
1862(a)X
1923(GC,)X
2089(which)X
2326(is)X
2407(short)X
2605(for)X
2729(Graphics)X
3068(Context.)X
3392(The)X
3551(contents)X
3867(of)X
555 2816(the)N
685(GC)X
829(object)X
1067(are)X
1196(private)X
1463(to)X
1554(the)X
1684(library.)X
1985(Several)X
2271(procedures)X
2678(take)X
2847(structures)X
3211(of)X
3306(type)X
3480(GCValues.)X
555 2912(The)N
714(following)X
1079(lists)X
1244(each)X
1427(entry)X
1630(by)X
1740(its)X
1846(de\256ned)X
2127(value,)X
2362(not)X
2497(by)X
2607(its)X
2713(position)X
3020(in)X
3111(the)X
5 f
3259(XGCValues)X
1 f
3756(struc-)X
555 3008(ture:)N
555 3104(#de\256ne)N
836(GCFunction)X
2283(\(1L<<0\))X
555 3200(#de\256ne)N
836(GCPlaneMask)X
2283(\(1L<<1\))X
555 3296(#de\256ne)N
836(GCForeground)X
2283(\(1L<<2\))X
555 3392(#de\256ne)N
836(GCBackground)X
2283(\(1L<<3\))X
555 3488(#de\256ne)N
836(GCLineWidth)X
2283(\(1L<<4\))X
555 3584(#de\256ne)N
836(GCLineStyle)X
2283(\(1L<<5\))X
555 3680(#de\256ne)N
836(GCCapStyle)X
2283(\(1L<<6\))X
555 3776(#de\256ne)N
836(GCJoinStyle)X
2283(\(1L<<7\))X
555 3872(#de\256ne)N
836(GCFillStyle)X
2283(\(1L<<8\))X
555 3968(#de\256ne)N
836(GCFillRule)X
2283(\(1L<<9\))X
555 4064(#de\256ne)N
836(GCTile)X
2283(\(1L<<10\))X
555 4160(#de\256ne)N
836(GCStipple)X
2283(\(1L<<11\))X
555 4256(#de\256ne)N
836(GCTileStipXOrigin)X
2283(\(1L<<12\))X
555 4352(#de\256ne)N
836(GCTileStipYOrigin)X
2283(\(1L<<13\))X
555 4448(#de\256ne)N
836(GCFont)X
2283(\(1L<<14\))X
555 4544(#de\256ne)N
836(GCSubwindowMode)X
2283(\(1L<<15\))X
555 4640(#de\256ne)N
836(GCGraphicsExposures)X
2283(\(1L<<16\))X
555 4736(#de\256ne)N
836(GCClipXOrigin)X
2283(\(1L<<17\))X
555 4832(#de\256ne)N
836(GCClipYOrigin)X
2283(\(1L<<18\))X
555 4928(#de\256ne)N
836(GCClipMask)X
2283(\(1L<<19\))X
555 5024(#de\256ne)N
836(GCDashOffset)X
2283(\(1L<<20\))X
555 5120(#de\256ne)N
836(GCDashList)X
2283(\(1L<<21\))X
555 5216(#de\256ne)N
836(GCArcMode)X
2283(\(1L<<22\))X
3 f
2239 6144(63)N

64 p
%%Page: 64 68
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(typedef)N
841(struct)X
1059({)X
843 768(int)N
959(function;)X
2283(/*)X
2374(logical)X
2637(operation)X
2992(*/)X
843 864(unsigned)N
1183(long)X
1362(plane_mask;)X
2283(/*)X
2374(plane)X
2587(mask)X
2795(*/)X
843 960(unsigned)N
1183(long)X
1362(foreground;)X
2283(/*)X
2374(foreground)X
2786(pixel)X
2985(*/)X
843 1056(unsigned)N
1183(long)X
1362(background;)X
2283(/*)X
2374(background)X
2811(pixel)X
3010(*/)X
843 1152(int)N
959(line_width;)X
2283(/*)X
2374(line)X
2529(width)X
2752(\(in)X
2872(pixels\))X
3134(*/)X
843 1248(int)N
959(line_style;)X
2283(/*)X
2374(LineSolid,)X
2767(LineOnOffDash,)X
3381(LineDoubleDash)X
4004(*/)X
843 1344(int)N
959(cap_style;)X
2283(/*)X
2374(CapNotLast,)X
2844(CapButt,)X
3183(CapRound,)X
3604(CapProjecting)X
4131(*/)X
843 1440(int)N
959(join_style;)X
2283(/*)X
2374(JoinMiter,)X
2761(JoinRound,)X
3187(JoinBevel)X
3562(*/)X
843 1536(int)N
959(\256ll_style;)X
2283(/*)X
2374(FillSolid,)X
2729 0.3403(FillTiled,)AX
3084 0.3125(FillStippled)AX
3525 0.1743(FillOpaqueStippled*/)AX
843 1632(int)N
959(\256ll_rule;)X
2283(/*)X
2374(EvenOddRule,)X
2917(WindingRule)X
3415(*/)X
843 1728(int)N
959(arc_mode;)X
2283(/*)X
2374(ArcChord,)X
2769(ArcPieSlice)X
3212(*/)X
843 1824(Pixmap)N
1135(tile;)X
2283(/*)X
2374(tile)X
2510(pixmap)X
2797(for)X
2921(tiling)X
3131(operations)X
3520(*/)X
843 1920(Pixmap)N
1135(stipple;)X
2283(/*)X
2374(stipple)X
2632(1)X
2698(plane)X
2911(pixmap)X
3198(for)X
3322(stippling)X
3654(*/)X
843 2016(int)N
959(ts_x_origin;)X
2283(/*)X
2374(offset)X
2596(for)X
2720(tile)X
2856(or)X
2951(stipple)X
3209(operations)X
3598(*/)X
843 2112(int)N
959(ts_y_origin;)X
843 2208(Font)N
1027(font;)X
2283(/*)X
2374(default)X
2641(text)X
2796(font)X
2960(for)X
3084(text)X
3239(operations)X
3628(*/)X
843 2304(int)N
959(subwindow_mode;)X
2283(/*)X
2374(ClipByChildren,)X
2983(IncludeInferiors)X
3571(*/)X
843 2400(Bool)N
1037(graphics_exposures;)X
2283(/*)X
2374(boolean,)X
2697(should)X
2954(exposures)X
3327(be)X
3432(generated)X
3796(*/)X
843 2496(int)N
959(clip_x_origin;)X
2283(/*)X
2374(origin)X
2607(for)X
2731(clipping)X
3043(*/)X
843 2592(int)N
959(clip_y_origin;)X
843 2688(Pixmap)N
1135(clip_mask;)X
2283(/*)X
2374(bitmap)X
2642(clipping;)X
2979(other)X
3182(calls)X
3366(for)X
3490(rects)X
3678(*/)X
843 2784(int)N
959(dash_offset;)X
2283(/*)X
2374(patterned/dashed)X
2993(line)X
3148(information)X
3587(*/)X
843 2880(char)N
1016(dashes;)X
555 2976(})N
619(XGCValues;)X
555 3244(Use)N
5 f
731(XCreateGC)X
1 f
1223(to)X
1314(create)X
1546(a)X
1607(new)X
1775(graphics)X
2095(context)X
2377(for)X
2501(the)X
2631(speci\256ed)X
2966(drawable.)X
3332(The)X
3491(de\256nition)X
3852(for)X
555 3340(this)N
705(function)X
1021(is:)X
555 3464(GC)N
699(XCreateGC)X
1121(\()X
2 f
1157(display)X
1 f
1412(,)X
2 f
1456(d)X
1 f
1507(,)X
2 f
1551(valuemask_create)X
1 f
2193(,)X
2 f
2237(values)X
1 f
2469(\))X
687 3560(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3656(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 3752(unsigned)N
1027(long)X
2 f
1206(valuemask_create)X
1 f
1848(;)X
687 3848(XGCValues)N
1138(*)X
2 f
1189(values)X
1 f
1421(;)X
2 f
555 4000(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4124(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 4248(valuemask_create)N
1 f
1131 4344(Speci\256es)N
1471(which)X
1708(components)X
2156(in)X
2247(the)X
2377(graphics)X
2697(context)X
2979(are)X
3108(to)X
3199(be)X
3304(set)X
3424(using)X
3637(informa-)X
1131 4440(tion)N
1291(in)X
1382(the)X
5 f
1530(XGCValues)X
1 f
2027(structure.)X
2401(This)X
2580(argument)X
2935(is)X
3016(the)X
3146(bitwise)X
3423(inclusive)X
3764(OR)X
3908(of)X
1131 4536(one)N
1280(or)X
1375(more)X
1578(of)X
1673(the)X
1803(valid)X
2002(GC)X
2146(component)X
2560(masks.)X
2 f
555 4660(values)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XGCValues)X
1 f
2540(structure.)X
555 4784(The)N
5 f
732(XCreateGC)X
1 f
1224(function)X
1540(creates)X
1806(a)X
1867(graphics)X
2187(context)X
2469(and)X
2618(returns)X
2884(a)X
2945(GC.)X
3133(The)X
3292(graphics)X
3612(context)X
555 4880(can)N
699(be)X
804(used)X
987(with)X
1166(any)X
1315(destination)X
1725(drawable)X
2069(having)X
2331(the)X
2461(same)X
2664(root)X
2828(and)X
2977(depth)X
3195(as)X
3290(the)X
3420(speci\256ed)X
3755(draw-)X
555 4976(able.)N
768(Use)X
926(with)X
1105(other)X
1308(drawables)X
1686(results)X
1938(in)X
2029(a)X
5 f
2108(BadMatch)X
1 f
2542(error.)X
555 5100(The)N
714(speci\256ed)X
1049(components)X
1497(of)X
1592(the)X
1722(new)X
1890(graphics)X
2210(context)X
2492(in)X
2583(valuemask_create)X
3236(are)X
3365(set)X
3485(to)X
3576(the)X
3706(values)X
555 5196(passed)N
811(in)X
902(the)X
1032(values)X
1279(argument.)X
1678(The)X
1837(other)X
2040(values)X
2287(default)X
2554(to)X
2645(the)X
2775(following)X
3140(values:)X
10 f
555 5252(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
708 5396(Component)N
1405(Value)X
10 f
555 5452(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 5596(function:)N
1405(GXcopy)X
555 5692(plane_mask:)N
1405(All)X
1540(ones)X
555 5788(foreground:)N
1405(0)X
3 f
2239 6144(64)N

65 p
%%Page: 65 69
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 584(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
708 728(Component)N
1405(Value)X
10 f
555 784(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 928(background:)N
1405(1)X
555 1024(line_width:)N
1405(0)X
555 1120(line_style:)N
1405(LineSolid)X
555 1216(cap_style:)N
1405(CapButt)X
555 1312(join_style:)N
1405(JoinMiter)X
555 1408(\256ll_style:)N
1405(FillSolid)X
555 1504(\256ll_rule:)N
1405(EvenOddRule)X
555 1600(arc_mode:)N
1405(ArcPieSlice)X
555 1696(tile:)N
1405(Pixmap)X
1697(of)X
1792(unspeci\256ed)X
2215(size)X
2374(\256lled)X
2578(with)X
2757(foreground)X
3169(pixel)X
1405 1792(\(that)N
1589(is,)X
1692(client)X
1911(speci\256ed)X
2246(pixel)X
2445(if)X
2521(any,)X
2692(else)X
2851(0\))X
1405 1888(\(subsequent)N
1847(changes)X
2152(to)X
2243(foreground)X
2655(do)X
2765(not)X
2900(affect)X
3122(this)X
3272(pixmap\))X
555 1984(stipple:)N
1405(Pixmap)X
1697(of)X
1792(unspeci\256ed)X
2215(size)X
2374(\256lled)X
2578(with)X
2757(ones)X
555 2080(ts_x_origin:)N
1405(0)X
555 2176(ts_y_origin:)N
1405(0)X
555 2272(font:)N
1405 0.2946(<implementation)AX
2033(dependent>)X
555 2368(subwindow_mode:)N
1405(ClipByChildren)X
555 2464(graphics_exposures:)N
1405(True)X
555 2560(clip_x_origin:)N
1405(0)X
555 2656(clip_y_origin:)N
1405(0)X
555 2752(clip_mask:)N
1405(None)X
555 2848(dash_offset:)N
1405(0)X
555 2944(dashes:)N
1405(4)X
1471(\(that)X
1655(is,)X
1758(the)X
1888(list)X
2019([4,)X
2136(4]\))X
10 f
555 3000(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 3172(Note)N
748(that)X
903(foreground)X
1315(and)X
1464(background)X
1901(are)X
2030(not)X
2165(set)X
2285(to)X
2376(any)X
2525(values)X
2772(likely)X
2996(to)X
3087(be)X
3192(useful)X
3429(on)X
3539(a)X
3600(color)X
555 3268(display.)N
555 3392(In)N
650(graphics)X
970(operations,)X
1381(given)X
1599(a)X
1660(source)X
1911(and)X
2060(destination)X
2470(pixel,)X
2691(the)X
2821(result)X
3039(is)X
3120(computed)X
3490(bitwise)X
3767(on)X
555 3488(corresponding)N
1080(bits)X
1230(of)X
1325(the)X
1455(pixels.)X
1732(That)X
1916(is,)X
2019(a)X
2080(boolean)X
2381(operation)X
2736(is)X
2817(performed)X
3205(in)X
3296(each)X
3479(bit)X
3595(plane.)X
3830(The)X
555 3584(plane_mask)N
998(restricts)X
1299(the)X
1429(operation)X
1784(to)X
1875(a)X
1936(subset)X
2178(of)X
2273(planes.)X
2564(That)X
2748(is,)X
2851(the)X
2981(result)X
3199(is)X
3280(computed)X
3650(by)X
3760(the)X
555 3680(following:)N
843 3852(\(\(src)N
1025(FUNC)X
1281(dst\))X
1435(AND)X
1646(plane-mask\))X
2103(OR)X
2247(\(dst)X
2401(AND)X
2612(\(NOT)X
2843(plane-mask\)\))X
555 3996(Range)N
802(checking)X
1142(is)X
1223(not)X
1358(performed)X
1746(on)X
1856(the)X
1986(values)X
2233(for)X
2357(foreground,)X
2791(background,)X
3250(or)X
3345(plane_mask.)X
555 4092(They)N
758(are)X
887(simply)X
1150(truncated)X
1500(to)X
1591(the)X
1721(appropriate)X
2144(number)X
2435(of)X
2530(bits.)X
2724(The)X
2883(line_width)X
3283(is)X
3364(measured)X
3723(in)X
3814(pix-)X
555 4188(els)N
675(and)X
824(either)X
1047(can)X
1191(be)X
1296(greater)X
1562(than)X
1736(or)X
1831(equal)X
2044(to)X
2135(one)X
2284(\(wide)X
2506(line\))X
2690(or)X
2785(can)X
2929(be)X
3034(the)X
3164(special)X
3431(value)X
3644(zero)X
3817(\(thin)X
555 4284(line\).)N
555 4408(Wide)N
768(lines)X
957(are)X
1086(drawn)X
1327(centered)X
1647(on)X
1757(the)X
1887(path)X
2061(described)X
2420(by)X
2530(the)X
2660(graphics)X
2980(request.)X
3300(Unless)X
3561(otherwise)X
555 4504(speci\256ed)N
890(by)X
1000(the)X
1130(join)X
1290(or)X
1385(cap)X
1529(style,)X
1740(the)X
1870(bounding)X
2225(box)X
2379(of)X
2474(a)X
2535(wide)X
2728(line)X
2883(with)X
3062(endpoints)X
3427([x1,)X
3588(y1],)X
3749([x2,)X
555 4600(y2],)N
716(and)X
865(width)X
1088(w)X
1173(is)X
1254(a)X
1315(rectangle)X
1660(with)X
1839(vertices)X
2135(at)X
2221(the)X
2351(following)X
2716(real)X
2870(coordinates:)X
843 4772([x1-\(w*sn/2\),)N
1345(y1+\(w*cs/2\)],)X
1863([x1+\(w*sn/2\),)X
2386(y1-\(w*cs/2\)],)X
843 4868([x2-\(w*sn/2\),)N
1345(y2+\(w*cs/2\)],)X
1863([x2+\(w*sn/2\),)X
2386(y2-\(w*cs/2\)])X
555 5012(The)N
714(sn)X
814(is)X
895(the)X
1025(sine)X
1189(of)X
1284(the)X
1414(angle)X
1627(of)X
1722(the)X
1852(line)X
2007(and)X
2156(cs)X
2251(is)X
2332(the)X
2462(cosine)X
2709(of)X
2804(the)X
2934(angle)X
3147(of)X
3242(the)X
3372(line.)X
3571(A)X
3656(pixel)X
3855(is)X
555 5108(part)N
714(of)X
809(the)X
939(line)X
1094(and,)X
1265(hence,)X
1514(is)X
1595(drawn,)X
1858(if)X
1934(the)X
2064(center)X
2301(of)X
2396(the)X
2526(pixel)X
2725(is)X
2806(fully)X
2995(inside)X
3228(the)X
3358(bounding)X
3713(box)X
555 5204(\(which)N
821(is)X
902(viewed)X
1178(as)X
1273(having)X
1535(in\256nitely)X
1877(thin)X
2037(edges\).)X
2332(If)X
2412(the)X
2542(center)X
2779(of)X
2874(the)X
3004(pixel)X
3203(is)X
3284(exactly)X
3561(on)X
3671(the)X
555 5300(bounding)N
910(box,)X
1086(it)X
1158(is)X
1239(part)X
1398(of)X
1493(the)X
1623(line)X
1778(if)X
1854(and)X
2003(only)X
2182(if)X
2258(the)X
2388(interior)X
2670(is)X
2751 0.3187(immediately)AX
3216(to)X
3307(its)X
3413(right)X
3602(\(x)X
3697(increas-)X
555 5396(ing)N
690(direction\).)X
1099(Pixels)X
1337(with)X
1516(centers)X
1787(on)X
1897(a)X
1958(horizontal)X
2338(edge)X
2526(are)X
2655(a)X
2716(special)X
2983(case)X
3156(and)X
3305(are)X
3434(part)X
3593(of)X
3688(the)X
3818(line)X
555 5492(if)N
631(and)X
780(only)X
959(if)X
1035(the)X
1165(interior)X
1447(is)X
1528 0.3187(immediately)AX
1993(below)X
2230(\(y)X
2325(increasing)X
2709(direction\).)X
555 5616(Thin)N
744(lines)X
933(\(zero)X
1135(line_width\))X
1564(are)X
1693(one)X
1842(pixel)X
2041(wide)X
2234(lines)X
2423(drawn)X
2664(using)X
2877(an)X
2982(unspeci\256ed,)X
3427(device)X
3679(depen-)X
555 5712(dent)N
729(algorithm.)X
1139(There)X
1366(are)X
1495(only)X
1674(two)X
1828(constraints)X
2232(on)X
2342(this)X
2492(algorithm.)X
3 f
2239 6144(65)N

66 p
%%Page: 66 70
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(1.)N
775(If)X
855(a)X
916(line)X
1071(is)X
1152(drawn)X
1393(unclipped)X
1763(from)X
1956([x1,y1])X
2234(to)X
2325([x2,y2])X
2603(and)X
2752(if)X
2828(another)X
3114(line)X
3269(is)X
3350(drawn)X
3591(unclipped)X
775 768(from)N
968([x1+dx,y1+dy])X
1522(to)X
1613([x2+dx,y2+dy],)X
2189(a)X
2250(point)X
2454([x,y])X
2644(is)X
2725(touched)X
3026(by)X
3136(drawing)X
3446(the)X
3576(\256rst)X
3735(line)X
3890(if)X
775 864(and)N
924(only)X
1103(if)X
1179(the)X
1309(point)X
1513([x+dx,y+dy])X
1979(is)X
2060(touched)X
2361(by)X
2471(drawing)X
2781(the)X
2911(second)X
3177(line.)X
555 988(2.)N
775(The)X
934(effective)X
1264(set)X
1384(of)X
1479(points)X
1717(comprising)X
2136(a)X
2197(line)X
2352(cannot)X
2609(be)X
2714(affected)X
3019(by)X
3129(clipping.)X
3485(That)X
3669(is,)X
3772(a)X
775 1084(point)N
979(is)X
1060(touched)X
1361(in)X
1452(a)X
1513(clipped)X
1795(line)X
1950(if)X
2026(and)X
2175(only)X
2354(if)X
2430(the)X
2560(point)X
2764(lies)X
2909(inside)X
3142(the)X
3272(clipping)X
3584(region)X
3831(and)X
775 1180(the)N
905(point)X
1109(would)X
1351(be)X
1456(touched)X
1757(by)X
1867(the)X
1997(line)X
2152(when)X
2364(drawn)X
2605(unclipped.)X
555 1304(A)N
640(wide)X
833(line)X
988(drawn)X
1229(from)X
1422([x1,y1])X
1700(to)X
1791([x2,y2])X
2069(always)X
2335(draws)X
2566(the)X
2696(same)X
2899(pixels)X
3132(as)X
3227(a)X
3288(wide)X
3481(line)X
3636(drawn)X
555 1400(from)N
748([x2,y2])X
1026(to)X
1117([x1,y1],)X
1417(not)X
1552(counting)X
1883(cap)X
2027(and)X
2176(join)X
2336(styles.)X
2603(Implementors)X
3115(are)X
3244(encouraged)X
3671(to)X
3762(make)X
555 1496(this)N
705(property)X
1025(true)X
1184(for)X
1308(thin)X
1468(lines,)X
1679(but)X
1814(it)X
1886(is)X
1967(not)X
2102(required.)X
2461(A)X
2546(line_width)X
2946(of)X
3041(zero)X
3214(may)X
3388(differ)X
3605(from)X
3798(a)X
555 1592(line_width)N
955(of)X
1050(one)X
1199(in)X
1290(which)X
1527(pixels)X
1760(are)X
1889(drawn.)X
555 1716(In)N
650(general,)X
953(drawing)X
1263(a)X
1324(thin)X
1484(line)X
1639(will)X
1799(be)X
1904(faster)X
2121(than)X
2295(drawing)X
2605(a)X
2666(wide)X
2859(line)X
3014(of)X
3109(width)X
3332(one.)X
3525(However,)X
555 1812(because)N
855(of)X
950(their)X
1134(different)X
1459(drawing)X
1769(algorithms,)X
2191(thin)X
2351(lines)X
2540(may)X
2714(not)X
2849(mix)X
3009(well,)X
3205(aesthetically)X
3669(speaking,)X
555 1908(with)N
734(wide)X
927(lines.)X
1160(If)X
1240(it)X
1312(is)X
1393(desirable)X
1733(to)X
1824(obtain)X
2067(precise)X
2338(and)X
2487(uniform)X
2793(results)X
3045(across)X
3286(all)X
3397(displays,)X
3730(a)X
3791(client)X
555 2004(should)N
812(always)X
1078(use)X
1217(a)X
1278(line_width)X
1678(of)X
1773(one,)X
1944(rather)X
2171(than)X
2345(a)X
2406(line_width)X
2806(of)X
2901(zero.)X
555 2128(The)N
714(line-style)X
1065(de\256nes)X
1336(which)X
1573(sections)X
1879(of)X
1974(a)X
2035(line)X
2190(are)X
2319(drawn:)X
5 f
570 2272(LineSolid)N
1 f
1407(The)X
1566(full)X
1711(path)X
1885(of)X
1980(the)X
2110(line)X
2265(is)X
2346(drawn.)X
5 f
570 2416(LineDoubleDash)N
1 f
1407(The)X
1566(full)X
1711(path)X
1885(of)X
1980(the)X
2110(line)X
2265(is)X
2346(drawn,)X
2609(but)X
2744(the)X
2874(even)X
3062(dashes)X
3318(are)X
3447(\256lled)X
3651(dif-)X
1407 2512(ferently)N
1703(than)X
1877(the)X
2007(odd)X
2161(dashes)X
2417(\(see)X
2580(\256ll-style\))X
2926(with)X
3105(CapButt)X
3422(style)X
3611(used)X
1407 2608(where)N
1643(even)X
1831(and)X
1980(odd)X
2134(dashes)X
2390(meet.)X
5 f
570 2752(LineOnOffDash)N
1 f
1407(Only)X
1605(the)X
1735(even)X
1923(dashes)X
2179(are)X
2308(drawn,)X
2571(and)X
2720(cap-style)X
3060(applies)X
3332(to)X
3423(all)X
3534(internal)X
3826(ends)X
1407 2848(of)N
1502(the)X
1632(individual)X
2013(dashes,)X
2291(except)X
2543(CapNotLast)X
2991(is)X
3072(treated)X
3334(as)X
3429(CapButt.)X
555 3020(The)N
714(cap_style)X
1069(de\256nes)X
1340(how)X
1513(the)X
1643(endpoints)X
2008(of)X
2103(a)X
2164(path)X
2338(are)X
2467(drawn:)X
5 f
570 3164(CapNotLast)N
1 f
1407(Equivalent)X
1812(to)X
1903(CapButt,)X
2242(except)X
2494(that)X
2649(for)X
2773(a)X
2834(line_width)X
3234(of)X
3329(zero)X
3502(or)X
3597(one)X
3746(the)X
1407 3260(\256nal)N
1586(endpoint)X
1917(is)X
1998(not)X
2133(drawn.)X
5 f
570 3404(CapButt)N
1 f
1407(Square)X
1673(at)X
1759(the)X
1889(endpoint)X
2220(\(perpendicular)X
2755(to)X
2846(the)X
2976(slope)X
3184(of)X
3279(the)X
3409(line\))X
3593(with)X
3772(no)X
1407 3500(projection)N
1787(beyond.)X
5 f
570 3644(CapRound)N
1 f
1407(A)X
1492(circular)X
1783(arc)X
1912(with)X
2091(the)X
2221(diameter)X
2552(equal)X
2765(to)X
2856(the)X
2986(line_width,)X
3408(centered)X
3728(on)X
3838(the)X
1407 3740(endpoint)N
1738(\(equivalent)X
2157(to)X
2248(CapButt)X
2565(for)X
2689(line_width)X
3089(zero)X
3262(or)X
3357(one\).)X
5 f
570 3884(CapProjecting)N
1 f
1407(Square)X
1673(at)X
1759(the)X
1889(end,)X
2060(but)X
2195(the)X
2325(path)X
2499(continues)X
2859(beyond)X
3140(the)X
3270(endpoint)X
3601(for)X
3725(a)X
3786(dis-)X
1407 3980(tance)N
1615(equal)X
1828(to)X
1919(half)X
2078(the)X
2208(line_width)X
2608(\(equivalent)X
3027(to)X
3118(CapButt)X
3435(for)X
3559(line_width)X
1407 4076(zero)N
1580(or)X
1675(one\).)X
555 4248(The)N
714(join_style)X
1085(de\256nes)X
1356(how)X
1529(corners)X
1809(are)X
1938(drawn)X
2179(for)X
2303(wide)X
2496(lines:)X
5 f
570 4392(JoinMiter)N
1 f
1407(The)X
1566(outer)X
1769(edges)X
1991(of)X
2086(two)X
2240(lines)X
2429(extend)X
2686(to)X
2777(meet)X
2971(at)X
3057(an)X
3162(angle.)X
5 f
570 4536(JoinRound)N
1 f
1407(A)X
1492(circular)X
1783(arc)X
1912(with)X
2091(diameter)X
2422(equal)X
2635(to)X
2726(the)X
2856(line_width,)X
3278(centered)X
3598(on)X
3708(the)X
1407 4632(joinpoint.)N
5 f
570 4776(JoinBevel)N
1 f
1407(CapButt)X
1724(endpoint)X
2055(styles,)X
2300(and)X
2449(then)X
2623(the)X
2753(triangular)X
3118(notch)X
3336(\256lled.)X
555 4948(For)N
699(a)X
760(line)X
915(with)X
1094(coincident)X
1484(endpoints)X
1849(\(x1=x2,)X
2148(y1=y2\),)X
2447(when)X
2659(the)X
2789(cap_style)X
3144(is)X
3225(applied)X
3507(to)X
3598(both)X
3777(end-)X
555 5044(points,)N
815(the)X
945(semantics)X
1315(depends)X
1625(on)X
1735(the)X
1865(line_width)X
2265(and)X
2414(the)X
2544(cap_style:)X
5 f
570 5188(CapNotLast)N
1 f
1407(thin)X
1827(Device)X
2098(dependent,)X
2504(but)X
2639(the)X
2769(desired)X
3045(effect)X
3267(is)X
3348(that)X
3503(nothing)X
3795(is)X
3876(drawn.)X
5 f
570 5332(CapButt)N
1 f
1407(thin)X
1827(Device)X
2098(dependent,)X
2504(but)X
2639(the)X
2769(desired)X
3045(effect)X
3267(is)X
3348(that)X
3503(a)X
3564(single)X
3797(pixel)X
3996(is)X
1827 5428(drawn.)N
5 f
570 5572(CapRound)N
1 f
1407(thin)X
1827(Same)X
2045(as)X
2140 0.2813(CapButt/thin.)AX
5 f
570 5716(CapProjecting)N
1 f
1407(thin)X
1827(Same)X
2045(as)X
2140 0.3472(Butt/thin.)AX
3 f
2239 6144(66)N

67 p
%%Page: 67 71
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
570 672(CapButt)N
1 f
1407(wide)X
1827(Nothing)X
2138(is)X
2219(drawn.)X
5 f
570 816(CapRound)N
1 f
1407(wide)X
1827(The)X
1986(closed)X
2233(path)X
2407(is)X
2488(a)X
2549(circle,)X
2789(centered)X
3109(at)X
3195(the)X
3325(endpoint,)X
3678(with)X
3857(diame-)X
1827 912(ter)N
1942(equal)X
2155(to)X
2246(the)X
2376(line_width.)X
5 f
570 1056(CapRound)N
1 f
1407(thin)X
1827(Device)X
2098(dependent,)X
2504(but)X
2639(the)X
2769(desired)X
3045(effect)X
3267(is)X
3348(that)X
3503(a)X
3564(single)X
3797(pixel)X
3996(is)X
1827 1152(drawn)N
5 f
570 1296(CapProjecting)N
1 f
1407(wide)X
1827(The)X
1986(closed)X
2233(path)X
2407(is)X
2488(a)X
2549(square4,)X
2866(aligned)X
3148(with)X
3327(the)X
3457(coordinate)X
3851(axes,)X
1827 1392(centered)N
2147(at)X
2233(the)X
2363(endpoint,)X
2716(with)X
2895(sides)X
3093(equal)X
3306(to)X
3397(the)X
3527(line_width)X
5 f
570 1536(CapProjecting)N
1 f
1407(thin)X
1827(Device)X
2098(dependent,)X
2504(but)X
2639(the)X
2769(desired)X
3045(effect)X
3267(is)X
3348(that)X
3503(a)X
3564(single)X
3797(pixel)X
3996(is)X
1827 1632(drawn)N
555 1804(For)N
699(a)X
760(line)X
915(with)X
1094(coincident)X
1484(endpoints)X
1849(\(x1=x2,)X
2148(y1=y2\),)X
2447(when)X
2659(the)X
2789(join_style)X
3160(is)X
3241(applied)X
3523(at)X
3609(one)X
3758(or)X
3853(both)X
555 1900(endpoints,)N
942(the)X
1072(effect)X
1294(is)X
1375(as)X
1470(if)X
1546(the)X
1676(line)X
1831(was)X
1989(removed)X
2319(from)X
2512(the)X
2642(overall)X
2909(path.)X
3127(However,)X
3492(if)X
3568(the)X
3698(total)X
555 1996(path)N
729(consists)X
1030(of)X
1125(or)X
1220(is)X
1301(reduced)X
1601(to)X
1692(a)X
1753(single)X
1986(point)X
2190(joined)X
2433(with)X
2612(itself,)X
2833(the)X
2963(effect)X
3185(is)X
3266(the)X
3396(same)X
3599(as)X
3694(when)X
555 2092(the)N
685(cap_style)X
1040(is)X
1121(applied)X
1403(at)X
1489(both)X
1668(endpoints.)X
555 2216(The)N
714 0.3239(tile/stipple)AX
1111(and)X
1260(clip)X
1415(origins)X
1682(are)X
1811(interpreted)X
2215(relative)X
2502(to)X
2593(the)X
2723(origin)X
2956(of)X
3051(whatever)X
3395(destination)X
3805(draw-)X
555 2312(able)N
724(is)X
805(speci\256ed)X
1140(in)X
1231(a)X
1292(graphics)X
1612(request.)X
1932(The)X
2091(tile)X
2227(pixmap)X
2514(must)X
2708(have)X
2896(the)X
3026(same)X
3229(root)X
3393(and)X
3542(depth)X
3760(as)X
3855(the)X
555 2408(graphics)N
875(context.)X
1201(Otherwise,)X
1606(a)X
5 f
1685(BadMatch)X
1 f
2119(error)X
2311(is)X
2392(generated.)X
2800(The)X
2959(stipple)X
3217(pixmap)X
3504(must)X
3698(have)X
555 2504(depth)N
773(one)X
922(and)X
1071(must)X
1265(have)X
1453(the)X
1583(same)X
1786(root)X
1950(as)X
2045(the)X
2175(graphics)X
2495(context)X
2777(\(else)X
2965(a)X
5 f
3044(BadMatch)X
1 f
3478(error\).)X
3721(For)X
555 2600(stipple)N
813(operations)X
1202(where)X
1438(the)X
1568(\256ll_style)X
1900(is)X
5 f
1999(FillStippled)X
1 f
2465(but)X
2600(not)X
5 f
2753(FillOpaqueStippled)X
1 f
3511(,)X
3555(the)X
3685(stipple)X
555 2696(pattern)N
822(is)X
903(tiled)X
1083(in)X
1174(a)X
1235(single)X
1468(plane)X
1681(and)X
1830(acts)X
1989(as)X
2084(an)X
2189(additional)X
2565(clip)X
2720(mask)X
2928(to)X
3019(be)X
3124(ANDed)X
3418(with)X
3597(the)X
555 2792(clip_mask.)N
962(Any)X
1135(size)X
1294(pixmap)X
1581(can)X
1725(be)X
1830(used)X
2013(for)X
2137(tiling)X
2347(or)X
2442(stippling,)X
2796(although)X
3127(some)X
3335(sizes)X
3528(may)X
3702(be)X
3807(faster)X
555 2888(to)N
646(use)X
785(than)X
959(others.)X
555 3012(The)N
714(\256ll_style)X
1046(de\256nes)X
1317(the)X
1447(contents)X
1763(of)X
1858(the)X
1988(source)X
2239(for)X
2363(line,)X
2540(text,)X
2717(and)X
2866(\256ll)X
2987(requests.)X
3319(For)X
3463(all)X
3574(text)X
3729(and)X
3878(\256ll)X
555 3108(requests)N
865(\(for)X
1018(example,)X
5 f
1379(XDrawText)X
1 f
1829(,)X
5 f
1891(XDrawText16)X
1 f
2439(,)X
5 f
2501(XFillRectangle)X
1 f
3082(,)X
5 f
3144(XFillPolygon)X
1 f
3647(,)X
3691(and)X
5 f
570 3204(XFillArc)N
1 f
887(\);)X
963(for)X
1087(line)X
1242(requests)X
1552(\(for)X
1705(example,)X
5 f
2066(XDrawLine)X
1 f
2510(,)X
5 f
2572(XDrawSegments)X
1 f
3247(,)X
5 f
3309(XDrawRectangle,)X
1 f
5 f
570 3300(XDrawArc)N
1 f
980(\))X
1031(with)X
1210(line_style)X
5 f
1594(LineSolid)X
1 f
1970(;)X
2017(and)X
2166(for)X
2290(the)X
2420(even)X
2608(dashes)X
2864(for)X
2988(line)X
3143(requests)X
3453(with)X
3632(line_style)X
5 f
570 3396(LineOnOffDash)N
1 f
1215(or)X
5 f
1328(LineDoubleDash)X
1 f
2014(the)X
2144(following)X
2509(apply:)X
5 f
570 3540(FillSolid)N
1 f
1695(Foreground.)X
5 f
570 3684(FillTiled)N
1 f
1695(Tile.)X
5 f
570 3828(FillOpaqueStippled)N
1 f
1695(A)X
1780(tile)X
1916(with)X
2095(the)X
2225(same)X
2428(width)X
2651(and)X
2800(height)X
3043(as)X
3138(stipple,)X
3418(but)X
3553(with)X
3732(back-)X
1695 3924(ground)N
1966(everywhere)X
2397(stipple)X
2655(has)X
2794(a)X
2855(zero)X
3028(and)X
3177(with)X
3356(foreground)X
3768(every-)X
1695 4020(where)N
1931(stipple)X
2189(has)X
2328(a)X
2389(one.)X
5 f
570 4164(FillStippled)N
1 f
1695(Foreground)X
2127(masked)X
2418(by)X
2528(stipple.)X
555 4336(When)N
787(drawing)X
1097(lines)X
1286(with)X
1465(line_style)X
5 f
1849(LineDoubleDash)X
1 f
2513(,)X
2557(the)X
2687(odd)X
2841(dashes)X
3097(are)X
3226(controlled)X
3606(by)X
3716(the)X
555 4432(\256ll_style)N
887(in)X
978(the)X
1108(following)X
1473(manner:)X
5 f
570 4576(FillSolid)N
1 f
1695(Background.)X
5 f
570 4720(FillTiled)N
1 f
1695(Same)X
1913(as)X
2008(for)X
2132(even)X
2320(dashes.)X
5 f
570 4864(FillOpaqueStippled)N
1 f
1695(Same)X
1913(as)X
2008(for)X
2132(even)X
2320(dashes.)X
5 f
570 5008(FillStippled)N
1 f
1695(Background)X
2147(masked)X
2438(by)X
2548(stipple.)X
555 5180(Storing)N
837(a)X
898(pixmap)X
1185(in)X
1276(a)X
1337(graphics)X
1657(context)X
1939(might)X
2168(or)X
2263(might)X
2492(not)X
2627(result)X
2845(in)X
2936(a)X
2997(copy)X
3190(being)X
3408(made.)X
3665(If)X
3745(the)X
555 5276(pixmap)N
842(is)X
923(later)X
1102(used)X
1285(as)X
1380(the)X
1510(destination)X
1920(for)X
2044(a)X
2105(graphics)X
2425(request,)X
2723(the)X
2853(change)X
3124(might)X
3353(or)X
3448(might)X
3677(not)X
3812(be)X
555 5372(re\257ected)N
880(in)X
971(the)X
1101(graphics)X
1421(context.)X
1747(If)X
1827(the)X
1957(pixmap)X
2244(is)X
2325(used)X
2508(simultaneously)X
3065(in)X
3156(a)X
3217(graphics)X
3537(request)X
3813(both)X
555 5468(as)N
650(a)X
711(destination)X
1121(and)X
1270(as)X
1365(a)X
1426(tile)X
1562(or)X
1657(stipple,)X
1937(the)X
2067(results)X
2319(are)X
2448(not)X
2583(de\256ned.)X
555 5592(It)N
631(is)X
712(quite)X
911(likely)X
1135(that)X
1290(some)X
1498(amount)X
1785(of)X
1880(graphics)X
2200(context)X
2482(information)X
2921(will)X
3081(be)X
3186(cached)X
3452(in)X
3543(display)X
555 5688(hardware)N
903(and)X
1052(that)X
1207(such)X
1390(hardware)X
1738(can)X
1882(only)X
2061(cache)X
2283(a)X
2344(small)X
2558(number)X
2849(of)X
2944(graphics)X
3264(contexts.)X
3624(Given)X
3861(the)X
555 5784(number)N
846(and)X
995(complexity)X
1415(of)X
1510(components,)X
1980(clients)X
2233(should)X
2490(view)X
2683(switching)X
3048(between)X
3363(graphics)X
3683(contexts)X
3 f
2239 6144(67)N

68 p
%%Page: 68 72
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(with)N
734(nearly)X
976(identical)X
1303(state)X
1487(as)X
1582(signi\256cantly)X
2041(more)X
2244(expensive)X
2618(than)X
2792(making)X
3079(minor)X
3312(changes)X
3617(to)X
3708(a)X
3769(single)X
555 768(graphics)N
875(context.)X
555 892(The)N
714(dash_list)X
1050(value)X
1263(allowed)X
1564(here)X
1737(is)X
1818(actually)X
2120(a)X
2181(simpli\256ed)X
2557(form)X
2750(of)X
2845(the)X
2975(more)X
3178(general)X
3459(patterns)X
3760(that)X
555 988(can)N
699(be)X
804(set)X
924(with)X
5 f
1121(XSetDashes)X
1 f
1626(.)X
1692(Specifying)X
2096(a)X
2157(value)X
2370(of)X
2465(N)X
2550(here)X
2723(is)X
2804(equivalent)X
3194(to)X
3285(specifying)X
3674(the)X
3804(two)X
555 1084(element)N
857(list)X
988([N,)X
1124(N])X
1238(in)X
5 f
1347(XSetDashes)X
1 f
1852(.)X
1918(The)X
2077(value)X
2290(must)X
2484(be)X
2589(nonzero.)X
2916(Otherwise,)X
3321(a)X
5 f
3400(BadValue)X
1 f
3819(error)X
555 1180(is)N
636(generated.)X
1044(The)X
1203(meaning)X
1529(of)X
1624(dash_offset)X
2051(and)X
2200(dash_list)X
2536(are)X
2665(explained)X
3030(for)X
3154(the)X
5 f
3302(XSetDashes)X
1 f
555 1276(function.)N
555 1400(The)N
714(clip_mask)X
1099(restricts)X
1400(writes)X
1637(to)X
1728(the)X
1858(destination)X
2268(drawable.)X
2634(If)X
2714(a)X
2775(pixmap)X
3062(is)X
3143(speci\256ed)X
3478(as)X
3573(the)X
555 1496(clip_mask,)N
962(it)X
1034(must)X
1228(have)X
1416(depth)X
1634(one)X
1783(and)X
1932(have)X
2120(the)X
2250(same)X
2453(root)X
2617(as)X
2712(the)X
2842(graphics)X
3162(context.)X
3488(Otherwise,)X
3893(a)X
5 f
570 1592(BadMatch)N
1 f
1004(error)X
1196(is)X
1277(generated.)X
1685(If)X
1765(clip_mask)X
2150(is)X
5 f
2249(None)X
1 f
2474(,)X
2518(the)X
2648(pixels)X
2881(are)X
3010(always)X
3276(drawn,)X
3539(regardless)X
3917(of)X
555 1688(the)N
685(clip)X
840(origin.)X
1117(The)X
1276(clip_mask)X
1661(can)X
1805(also)X
1969(be)X
2074(set)X
2194(with)X
2373(the)X
5 f
2521(XSetClipRectangles)X
1 f
3340(request.)X
3660(Only)X
3858(pix-)X
555 1784(els)N
675(where)X
911(the)X
1041(clip-mask)X
1411(has)X
1550(a)X
1611(one)X
1760(bit)X
1876(are)X
2005(drawn.)X
2268(Pixels)X
2506(are)X
2635(not)X
2770(drawn)X
3011(outside)X
3288(the)X
3418(area)X
3586(covered)X
3886(by)X
555 1880(the)N
685(clip_mask)X
1070(or)X
1165(where)X
1401(the)X
1531(clip_mask)X
1916(has)X
2055(a)X
2116(zero)X
2289(bit.)X
2449(It)X
2525(affects)X
2781(all)X
2892(graphics)X
3212(requests.)X
3566(The)X
555 1976(clip_mask)N
940(does)X
1123(not)X
1258(clip)X
1413(sources.)X
1742(The)X
1901(clip_mask)X
2286(origin)X
2519(is)X
2600(interpreted)X
3004(relative)X
3291(to)X
3382(the)X
3512(origin)X
3745(of)X
555 2072(whatever)N
899(destination)X
1309(drawable)X
1653(is)X
1734(speci\256ed)X
2069(in)X
2160(a)X
2221(graphics)X
2541(request.)X
555 2196(For)N
5 f
717 -0.2356(ClipByChildren)AX
1 f
1311(,)X
1355(both)X
1534(source)X
1785(and)X
1934(destination)X
2344(windows)X
2683(are)X
2812(additionally)X
3257(clipped)X
3539(by)X
3649(all)X
3760(view-)X
555 2292(able)N
5 f
742 0.3187(InputOutput)AX
1 f
1242(children.)X
1575(For)X
5 f
1737(IncludeInferiors)X
1 f
2354(,)X
2398(neither)X
2665(source)X
2916(nor)X
3055(destination)X
3465(window)X
3770(is)X
555 2388(clipped)N
837(by)X
947(inferiors.)X
1289(This)X
1468(will)X
1628(result)X
1846(in)X
1937(drawing)X
2247(through)X
2543(subwindow)X
2970(boundaries.)X
3422(The)X
3581(use)X
3720(of)X
5 f
570 2484(IncludeInferiors)N
1 f
1209(on)X
1319(a)X
1380(window)X
1685(of)X
1780(one)X
1929(depth)X
2147(with)X
2326(mapped)X
2627(inferiors)X
2947(of)X
3042(differing)X
3372(depth)X
3590(is)X
3671(not)X
3806(ille-)X
555 2580(gal,)N
707(but)X
842(the)X
972(semantics)X
1342(are)X
1471(unde\256ned)X
1840(by)X
1950(the)X
2080(core)X
2253(protocol.)X
555 2704(The)N
714(\256ll_rule)X
1016(de\256nes)X
1287(what)X
1480(pixels)X
1713(are)X
1842(inside)X
2075(\(drawn\))X
2374(for)X
2498(paths)X
2706(given)X
2924(in)X
5 f
3033(XFillPolygon)X
1 f
3558(requests.)X
5 f
570 2800(EvenOddRule)N
1 f
1155(means)X
1402(a)X
1463(point)X
1667(is)X
1748(inside)X
1981(if)X
2057(an)X
2162(in\256nite)X
2435(ray)X
2569(with)X
2748(the)X
2878(point)X
3082(as)X
3177(origin)X
3410(crosses)X
3685(the)X
3815(path)X
555 2896(an)N
660(odd)X
814(number)X
1105(of)X
1200(times.)X
555 3020(For)N
5 f
717(WindingRule)X
1 f
1229(,)X
1273(a)X
1334(point)X
1538(is)X
1619(inside)X
1852(if)X
1928(an)X
2033(in\256nite)X
2306(ray)X
2440(with)X
2619(the)X
2749(point)X
2953(as)X
3048(origin)X
3281(crosses)X
3556(an)X
3661(unequal)X
555 3116(number)N
846(of)X
941(clockwise)X
1315(and)X
1464(counterclockwise)X
2102(directed)X
2408(path)X
2582(segments.)X
2976(A)X
3061(clockwise)X
3435(directed)X
3741(path)X
555 3212(segment)N
871(is)X
952(one)X
1101(which)X
1338(crosses)X
1613(the)X
1743(ray)X
1877(from)X
2070(left)X
2210(to)X
2301(right)X
2490(as)X
2585(observed)X
2924(from)X
3117(the)X
3247(point.)X
3495(A)X
3580(counter-)X
555 3308(clockwise)N
929(segment)X
1245(is)X
1326(one)X
1475(which)X
1712(crosses)X
1987(the)X
2117(ray)X
2251(from)X
2444(right)X
2633(to)X
2724(left)X
2864(as)X
2959(observed)X
3298(from)X
3491(the)X
3621(point.)X
3869(The)X
555 3404(case)N
728(where)X
964(a)X
1025(directed)X
1331(line)X
1486(segment)X
1802(is)X
1883(coincident)X
2273(with)X
2452(the)X
2582(ray)X
2716(is)X
2797(uninteresting)X
3280(because)X
3580(you)X
3734(can)X
555 3500(simply)N
818(choose)X
1084(a)X
1145(different)X
1470(ray)X
1604(that)X
1759(is)X
1840(not)X
1975(coincident)X
2365(with)X
2544(a)X
2605(segment.)X
555 3624(For)N
699(both)X
5 f
896(EvenOddRule)X
1 f
1481(and)X
5 f
1648(WindingRule)X
1 f
2160(,)X
2204(a)X
2265(point)X
2469(is)X
2550(in\256nitely)X
2892(small,)X
3128(and)X
3277(the)X
3407(path)X
3581(is)X
3662(an)X
555 3720(in\256nitely)N
897(thin)X
1057(line.)X
1234(A)X
1319(pixel)X
1518(is)X
1599(inside)X
1832(if)X
1908(the)X
2038(center)X
2275(point)X
2479(of)X
2574(the)X
2704(pixel)X
2903(is)X
2984(inside,)X
3239(and)X
3388(the)X
3518(center)X
3755(point)X
555 3816(is)N
636(not)X
771(on)X
881(the)X
1011(boundary.)X
1387(If)X
1467(the)X
1597(center)X
1834(point)X
2038(is)X
2119(on)X
2229(the)X
2359(boundary,)X
2735(the)X
2865(pixel)X
3064(is)X
3145(inside)X
3378(if)X
3454(and)X
3603(only)X
3782(if)X
3858(the)X
555 3912(polygon)N
866(interior)X
1148(is)X
1229 0.3187(immediately)AX
1694(to)X
1785(its)X
1891(right)X
2080(\(x)X
2175(increasing)X
2559(direction\).)X
2946(Pixels)X
3184(with)X
3363(centers)X
3634(along)X
3852(a)X
555 4008(horizontal)N
935(edge)X
1123(are)X
1252(a)X
1313(special)X
1580(case)X
1753(and)X
1902(are)X
2031(inside)X
2264(if)X
2340(and)X
2489(only)X
2668(if)X
2744(the)X
2874(polygon)X
3185(interior)X
3467(is)X
3548 0.3187(immediately)AX
555 4104(below)N
792(\(y)X
887(increasing)X
1271(direction\).)X
555 4228(The)N
714(arc_mode)X
1083(controls)X
1389(\256lling)X
1623(in)X
1714(the)X
5 f
1862(XFillArcs)X
1 f
2245(function)X
2561(and)X
2710(can)X
2854(be)X
2959(one)X
3108(of)X
3203(the)X
3333(constants)X
5 f
570 4324(ArcPieSlice)N
1 f
1056(or)X
5 f
1169(ArcChord)X
1 f
1555(.)X
1621(The)X
1780(graphics_exposure)X
2461(\257ag)X
2615(controls)X
5 f
2939(GraphicsExpose)X
1 f
3622(event)X
3835(gen-)X
555 4420(eration)N
822(for)X
5 f
964(XCopyArea)X
1 f
1451(and)X
5 f
1618(XCopyPlane)X
1 f
2144(requests)X
2454(\(and)X
2632(any)X
2781(similar)X
3049(requests)X
3359(de\256ned)X
3640(by)X
3750(exten-)X
555 4516(sions\).)N
555 4640(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCreateGC)X
1 f
2744(are)X
5 f
2891(BadDrawable)X
1 f
3433(,)X
5 f
3495(BadPixmap)X
1 f
3960(,)X
5 f
570 4736(BadAlloc)N
1 f
932(,)X
5 f
994(BadMatch)X
1 f
1406(,)X
1450(and)X
5 f
1617(BadFont)X
1 f
1966(.)X
555 4956(Use)N
5 f
731(XCopyGC)X
1 f
1164(to)X
1255(copy)X
1448(components)X
1896(from)X
2089(a)X
2150(source)X
2401(graphics)X
2721(context)X
3003(to)X
3094(a)X
3155(destination)X
3565(graphics)X
555 5052(context.)N
859(The)X
1018(de\256nition)X
1379(for)X
1503(this)X
1653(function)X
1969(is:)X
555 5176(XCopyGC)N
938(\()X
2 f
974(display)X
1 f
1229(,)X
2 f
1273(src)X
1 f
1387(,)X
2 f
1431(valuemask_copy)X
1 f
2019(,)X
2 f
2063(dest)X
1 f
2212(\))X
687 5272(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5368(GC)N
2 f
831(src)X
1 f
945(,)X
2 f
989(dest)X
1 f
1138(;)X
687 5464(unsigned)N
1027(long)X
2 f
1206(valuemask_copy)X
1 f
1794(;)X
2 f
555 5616(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5740(src)N
1 f
1131(Speci\256es)X
1471(the)X
1601(components)X
2049(of)X
2144(the)X
2274(source)X
2525(graphics)X
2845(context.)X
3 f
2239 6144(68)N

69 p
%%Page: 69 73
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(valuemask_copy)N
1 f
1136(Speci\256es)X
1476(which)X
1713(components)X
2161(in)X
2252(the)X
2382(source)X
2633(graphics)X
2953(context)X
3235(are)X
3364(to)X
3455(be)X
3560(copied)X
3817(to)X
1131 768(the)N
1261(destination)X
1671(graphics)X
1991(context.)X
2317(This)X
2496(argument)X
2851(is)X
2932(the)X
3062(bitwise)X
3339(inclusive)X
3680(OR)X
3824(of)X
1131 864(one)N
1280(or)X
1375(more)X
1578(of)X
1673(the)X
1803(valid)X
2002(GC)X
2146(component)X
2560(masks.)X
2 f
555 988(dest)N
1 f
1131(Speci\256es)X
1471(the)X
1601(destination)X
2011(graphics)X
2331(context.)X
555 1112(The)N
5 f
732(XCopyGC)X
1 f
1165(function)X
1481(copies)X
1728(the)X
1858(speci\256ed)X
2193(components)X
2641(from)X
2834(the)X
2964(source)X
3215(graphics)X
3535(context)X
3817(to)X
555 1208(the)N
685(destination)X
1095(graphics)X
1415(context.)X
1741(The)X
1900(source)X
2151(and)X
2300(destination)X
2710(graphics)X
3030(contexts)X
3346(must)X
3540(have)X
3728(the)X
555 1304(same)N
758(root)X
922(and)X
1071(depth.)X
1333(Otherwise,)X
1738(a)X
5 f
1817(BadMatch)X
1 f
2251(error)X
2443(is)X
2524(generated.)X
2932(The)X
3091(valuemask_copy)X
3705(speci\256es)X
555 1400(which)N
792(component)X
1206(to)X
1297(copy,)X
1512(as)X
1607(for)X
5 f
1749(XCreateGC)X
1 f
2219(.)X
555 1524(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCopyGC)X
1 f
2685(are)X
5 f
2832(BadGC)X
1 f
3136(,)X
5 f
3198(BadAlloc)X
1 f
3560(,)X
3604(and)X
5 f
3771(Bad-)X
555 1620(Value)N
1 f
795(.)X
555 1840(Use)N
5 f
731(XChangeGC)X
1 f
1267(to)X
1358(change)X
1629(the)X
1759(components)X
2207(in)X
2298(the)X
2428(speci\256ed)X
2763(graphics)X
3083(context.)X
3387(The)X
3546(de\256nition)X
3907(for)X
555 1936(this)N
705(function)X
1021(is:)X
555 2060(XChangeGC)N
1016(\()X
2 f
1052(display)X
1 f
1307(,)X
2 f
1351(gc)X
1 f
1441(,)X
2 f
1485(valuemask_change)X
1 f
2161(,)X
2 f
2205(values)X
1 f
2437(\))X
687 2156(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2252(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2348(unsigned)N
1027(long)X
2 f
1206(valuemask_change)X
1 f
1882(;)X
687 2444(XGCValues)N
1138(*)X
2 f
1189(values)X
1 f
1421(;)X
2 f
555 2596(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2720(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2844(valuemask_change)N
1 f
1131 2940(Speci\256es)N
1471(which)X
1708(components)X
2156(in)X
2247(the)X
2377(graphics)X
2697(context)X
2979(are)X
3108(to)X
3199(be)X
3304(changed)X
3619(using)X
1131 3036(information)N
1570(in)X
1661(the)X
5 f
1809(XGCValues)X
1 f
2306(structure.)X
2680(This)X
2859(argument)X
3214(is)X
3295(the)X
3425(bitwise)X
1131 3132(inclusive)N
1472(OR)X
1616(of)X
1711(one)X
1860(or)X
1955(more)X
2158(of)X
2253(the)X
2383(valid)X
2582(GC)X
2726(component)X
3140(masks.)X
2 f
555 3256(values)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XGCValues)X
1 f
2540(structure.)X
555 3380(The)N
5 f
732(XChangeGC)X
1 f
1268(function)X
1584(changes)X
1889(the)X
2019(components)X
2467(speci\256ed)X
2802(by)X
2912(the)X
3042(valuemask_change)X
3734(argu-)X
555 3476(ment)N
754(for)X
878(the)X
1008(speci\256ed)X
1343(graphics)X
1663(context.)X
1989(The)X
2148(values)X
2395(argument)X
2750(contains)X
3066(the)X
3196(values)X
3443(to)X
3534(be)X
3639(set.)X
3803(The)X
555 3572(values)N
802(and)X
951(restrictions)X
1365(are)X
1494(the)X
1624(same)X
1827(as)X
1922(for)X
5 f
2064(XCreateGC)X
1 f
2534(.)X
2600(Changing)X
2965(the)X
3095(clip_mask)X
3480(also)X
3644(overrides)X
555 3668(any)N
704(previous)X
5 f
1047(XSetClipRectangles)X
1 f
1866(request)X
2142(on)X
2252(the)X
2382(context.)X
2686(Changing)X
3051(the)X
3181(dash_offset)X
3608(or)X
555 3764(dash_list)N
891(overrides)X
1240(any)X
1389(previous)X
5 f
1732(XSetDashes)X
1 f
2259(request)X
2535(on)X
2645(the)X
2775(context.)X
3101(The)X
3260(order)X
3467(in)X
3558(which)X
3795(com-)X
555 3860(ponents)N
851(are)X
980(veri\256ed)X
1271(and)X
1420(altered)X
1682(is)X
1763(sever-dependent.)X
2405(If)X
2485(an)X
2590(error)X
2782(is)X
2863(generated,)X
3249(a)X
3310(subset)X
3552(of)X
3647(the)X
3777(com-)X
555 3956(ponents)N
851(may)X
1025(have)X
1213(been)X
1401(altered.)X
555 4080(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XChangeGC)X
1 f
2372(are)X
5 f
2519(BadGC)X
1 f
2823(,)X
5 f
2885(BadPixmap)X
1 f
3350(,)X
5 f
3412(BadFont)X
1 f
3761(,)X
5 f
3823(Bad-)X
555 4176(Match)N
1 f
810(,)X
5 f
872(BadAlloc)X
1 f
1234(,)X
1278(and)X
5 f
1445(BadValue.)X
1 f
555 4396(Use)N
5 f
731(XFreeGC)X
1 f
1140(to)X
1231(free)X
1389(the)X
1519(speci\256ed)X
1854(graphics)X
2174(context.)X
2478(The)X
2637(de\256nition)X
2998(for)X
3122(this)X
3272(function)X
3588(is:)X
555 4520(XFreeGC)N
903(\()X
2 f
939(display)X
1 f
1194(,)X
2 f
1238(gc)X
1 f
1328(\))X
687 4616(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4712(GC)N
2 f
831(gc)X
1 f
921(;)X
2 f
555 4864(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4988(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
555 5112(The)N
5 f
732(XFreeGC)X
1 f
1141(function)X
1457(destroys)X
1772(the)X
1902(speci\256ed)X
2237(graphics)X
2557(context)X
2839(as)X
2934(well)X
3108(as)X
3203(the)X
3333(shadow)X
3623(copy.)X
555 5236(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XFreeGC)X
1 f
2211(is)X
5 f
2310(BadGC)X
1 f
2614(.)X
3 f
555 5428(5.4.)N
731(Using)X
968(GC)X
1122(Convenience)X
1623(Routines)X
1 f
555 5552(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set)X
2243(a)X
2304(graphics)X
2624(context)X
2906(component.)X
3364(This)X
3543(section)X
555 5648(discusses)N
904(how)X
1077(to)X
1168(set)X
1288(the:)X
3 f
2239 6144(69)N

70 p
%%Page: 70 74
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(Foreground,)X
1229(background,)X
1688(plane)X
1901(mask,)X
2131(or)X
2226(function)X
2542(components)X
10 f
555 796(g)N
1 f
775(Line)X
959(attributes)X
1310(and)X
1459(dashes)X
1715(components)X
10 f
555 920(g)N
1 f
775(Fill)X
921(style)X
1110(and)X
1259(\256ll)X
1380(rule)X
1539(components)X
10 f
555 1044(g)N
1 f
775(Fill)X
921(tile)X
1057(and)X
1206(stipple)X
1464(components)X
10 f
555 1168(g)N
1 f
775(Font)X
959(component)X
10 f
555 1292(g)N
1 f
775(Clip)X
950(region)X
1197(component)X
10 f
555 1416(g)N
1 f
775(Arc)X
928(mode,)X
1168(subwindow)X
1595(mode,)X
1835(and)X
1984(graphics)X
2304(exposure)X
2643(components)X
3 f
555 1608(5.4.1.)N
797(Setting)X
1083(Foreground,)X
1577(Background,)X
2086(Plane)X
2319(Mask,)X
2573(or)X
2678(Function)X
1 f
555 1732(Use)N
5 f
731(XSetState)X
1 f
1167(to)X
1258(set)X
1378(the)X
1508(foreground,)X
1942(background,)X
2401(plane)X
2614(mask,)X
2844(and)X
2993(function)X
3309(components)X
3757(for)X
3881(the)X
555 1828(speci\256ed)N
890(graphics)X
1210(context.)X
1514(The)X
1673(de\256nition)X
2034(for)X
2158(this)X
2308(function)X
2624(is:)X
555 1952(XSetState)N
915(\()X
2 f
951(display)X
1 f
1206(,)X
2 f
1250(gc)X
1 f
1340(,)X
2 f
1384(foreground)X
1 f
1787(,)X
2 f
1831(background)X
1 f
2258(,)X
2 f
2302(function)X
1 f
2599(,)X
2 f
2643(plane_mask)X
1 f
3070(\))X
687 2048(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2144(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2240(unsigned)N
1027(long)X
2 f
1206(foreground)X
1 f
1609(,)X
2 f
1653(background)X
1 f
2080(;)X
687 2336(int)N
2 f
803(function)X
1 f
1100(;)X
687 2432(unsigned)N
1027(long)X
2 f
1206(plane_mask)X
1 f
1633(;)X
2 f
555 2584(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2708(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2832(foreground)N
1 f
1131(Speci\256es)X
1471(the)X
1601(foreground)X
2013(you)X
2167(want)X
2360(to)X
2451(set)X
2571(for)X
2695(the)X
2825(speci\256ed)X
3160(graphics)X
3480(context.)X
2 f
555 2956(background)N
1 f
1131(Speci\256es)X
1471(the)X
1601(background)X
2038(you)X
2192(want)X
2385(to)X
2476(set)X
2596(for)X
2720(the)X
2850(speci\256ed)X
3185(graphics)X
3505(context.)X
2 f
555 3080(function)N
1 f
1131(Speci\256es)X
1471(the)X
1601(function)X
1917(you)X
2071(want)X
2264(to)X
2355(set)X
2475(for)X
2599(the)X
2729(speci\256ed)X
3064(graphics)X
3384(context.)X
2 f
555 3204(plane_mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(plane)X
1814(mask.)X
555 3356(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetState)X
1 f
2272(are)X
5 f
2419(BadGC)X
1 f
2745(and)X
5 f
2912(BadValue)X
1 f
3309(.)X
555 3576(Use)N
5 f
731(XSetFunction)X
1 f
1298(to)X
1389(set)X
1509(a)X
1570(speci\256ed)X
1905(value)X
2118(in)X
2209(the)X
2339(speci\256ed)X
2674(graphics)X
2994(context.)X
3298(The)X
3457(de\256nition)X
3818(for)X
555 3672(this)N
705(function)X
1021(is:)X
555 3796(XSetFunction)N
1052(\()X
2 f
1088(display)X
1 f
1343(,)X
2 f
1387(gc)X
1 f
1477(,)X
2 f
1521(function)X
1 f
1818(\))X
687 3892(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3988(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4084(int)N
2 f
803(function)X
1 f
1100(;)X
2 f
555 4236(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4360(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 4484(function)N
1 f
1131(Speci\256es)X
1471(the)X
1601(function)X
1917(you)X
2071(want)X
2264(to)X
2355(set)X
2475(for)X
2599(the)X
2729(speci\256ed)X
3064(graphics)X
3384(context.)X
555 4608(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetFunction)X
1 f
2403(are)X
5 f
2550(BadGC)X
1 f
2876(and)X
5 f
3043(BadValue)X
1 f
3440(.)X
555 4828(Use)N
5 f
731(XSetPlaneMask)X
1 f
1395(to)X
1486(set)X
1606(the)X
1736(plane)X
1949(mask)X
2157(in)X
2248(the)X
2378(speci\256ed)X
2713(graphics)X
3033(context.)X
3337(The)X
3496(de\256nition)X
3857(for)X
555 4924(this)N
705(function)X
1021(is:)X
555 5048(XSetPlaneMask)N
1129(\()X
2 f
1165(display)X
1 f
1420(,)X
2 f
1464(gc)X
1 f
1554(,)X
2 f
1598(plane_mask)X
1 f
2025(\))X
687 5144(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5240(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5336(unsigned)N
1027(long)X
2 f
1206(plane_mask)X
1 f
1633(;)X
2 f
555 5488(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5612(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5736(plane_mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(plane)X
1814(mask.)X
3 f
2239 6144(70)N

71 p
%%Page: 71 75
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetPlaneMask)X
1 f
2466(is)X
5 f
2565(BadGC)X
1 f
2869(.)X
555 892(Use)N
5 f
731(XSetForeground)X
1 f
1415(to)X
1506(set)X
1626(the)X
1756(foreground)X
2168(in)X
2259(the)X
2389(speci\256ed)X
2724(graphics)X
3044(context.)X
3348(The)X
3507(de\256nition)X
3868(for)X
555 988(this)N
705(function)X
1021(is:)X
555 1112(XSetForeground)N
1148(\()X
2 f
1184(display)X
1 f
1439(,)X
2 f
1483(gc)X
1 f
1573(,)X
2 f
1617(foreground)X
1 f
2020(\))X
687 1208(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1304(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1400(unsigned)N
1027(long)X
2 f
1206(foreground)X
1 f
1609(;)X
2 f
555 1552(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1676(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1800(foreground)N
1 f
1131(Speci\256es)X
1471(the)X
1601(foreground)X
2013(you)X
2167(want)X
2360(to)X
2451(set)X
2571(for)X
2695(the)X
2825(speci\256ed)X
3160(graphics)X
3480(context.)X
555 1924(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetForeground)X
1 f
2486(is)X
5 f
2585(BadGC)X
1 f
2889(.)X
555 2144(Use)N
5 f
731(XSetBackground)X
1 f
1430(to)X
1521(set)X
1641(the)X
1771(background)X
2208(in)X
2299(the)X
2429(speci\256ed)X
2764(graphics)X
3084(context.)X
3388(The)X
3547(de\256nition)X
3908(for)X
555 2240(this)N
705(function)X
1021(is:)X
555 2364(XSetBackground)N
1168(\()X
2 f
1204(display)X
1 f
1459(,)X
2 f
1503(gc)X
1 f
1593(,)X
2 f
1637(background)X
1 f
2064(\))X
687 2460(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2556(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2652(unsigned)N
1027(long)X
2 f
1206(background)X
1 f
1633(;)X
2 f
555 2804(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2928(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3052(background)N
1 f
1131(Speci\256es)X
1471(the)X
1601(background)X
2038(you)X
2192(want)X
2385(to)X
2476(set)X
2596(for)X
2720(the)X
2850(speci\256ed)X
3185(graphics)X
3505(context.)X
555 3176(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetBackground)X
1 f
2501(is)X
5 f
2600(BadGC)X
1 f
2904(.)X
3 f
555 3368(5.4.2.)N
797(Setting)X
1083(Line)X
1277(Attributes)X
1684(and)X
1848(Dashes)X
1 f
555 3492(Use)N
5 f
731(XSetLineAttributes)X
1 f
1499(to)X
1590(set)X
1710(the)X
1840(line)X
1995(drawing)X
2305(components)X
2753(in)X
2844(the)X
2974(speci\256ed)X
3309(graphics)X
3629(context.)X
555 3588(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 3712(XSetLineAttributes)N
1253(\()X
2 f
1289(display)X
1 f
1544(,)X
2 f
1588(gc)X
1 f
1678(,)X
2 f
1722(line_width)X
1 f
2103(,)X
2 f
2147(line_style)X
1 f
2493(,)X
2 f
2537(cap_style)X
1 f
2877(,)X
2 f
2921(join_style)X
1 f
3272(\))X
687 3808(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3904(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4000(unsigned)N
1027(int)X
2 f
1143(line_width)X
1 f
1524(;)X
687 4096(int)N
2 f
803(line_style)X
1 f
1149(;)X
687 4192(int)N
2 f
803(cap_style)X
1 f
1143(;)X
687 4288(int)N
2 f
803(join_style)X
1 f
1154(;)X
2 f
555 4440(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4564(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 4688(line_width)N
1 f
1131(Speci\256es)X
1471(the)X
1601(line)X
1756(width)X
1979(you)X
2133(want)X
2326(to)X
2417(set)X
2537(for)X
2661(the)X
2791(speci\256ed)X
3126(graphics)X
3446(context.)X
2 f
555 4812(line_style)N
1 f
1131(Speci\256es)X
1471(the)X
1601(line)X
1756(style)X
1945(you)X
2099(want)X
2292(to)X
2383(set)X
2503(for)X
2627(the)X
2757(speci\256ed)X
3092(graphics)X
3412(context.)X
3738(Possi-)X
1131 4908(ble)N
1261(values)X
1508(are)X
5 f
1655(LineSolid)X
1 f
2053(\(solid\),)X
5 f
2345(LineOnOffDash)X
1 f
2990(\(on-off)X
3260(dash\),)X
3494(or)X
5 f
1146 5004(LineDoubleDash)N
1 f
1832(\(double)X
2123(dash\).)X
2 f
555 5128(cap_style)N
1 f
1131(Speci\256es)X
1471(the)X
1601(line)X
1756(and)X
1905(cap)X
2049(style)X
2238(you)X
2392(want)X
2585(to)X
2676(set)X
2796(for)X
2920(the)X
3050(speci\256ed)X
3385(graphics)X
3705(context.)X
1131 5224(Possible)N
1447(values)X
1694(are)X
5 f
1841(CapNotLast)X
1 f
2321(,)X
5 f
2383(CapButt)X
1 f
2717(,)X
5 f
2779(CapRound)X
1 f
3214(,)X
3258(or)X
5 f
3371(CapProjecting)X
1 f
3938(.)X
2 f
555 5348(join_style)N
1 f
1131(Speci\256es)X
1471(the)X
1601(line-join)X
1923(style)X
2112(you)X
2266(want)X
2459(to)X
2550(set)X
2670(for)X
2794(the)X
2924(speci\256ed)X
3259(graphics)X
3579(context.)X
1131 5444(Possible)N
1447(values)X
1694(are)X
5 f
1841(JoinMiter)X
1 f
2212(,)X
5 f
2274(JoinRound)X
1 f
2709(,)X
2753(or)X
5 f
2866(JoinBevel)X
1 f
3262(.)X
555 5568(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetLineAttributes)X
1 f
2604(are)X
5 f
2751(BadGC)X
1 f
3077(and)X
5 f
3244(BadValue)X
1 f
3641(.)X
555 5788(Use)N
5 f
731(XSetDashes)X
1 f
1258(to)X
1349(set)X
1469(the)X
1599(dash_offset)X
2026(and)X
2175(dash_list)X
2511(for)X
2635(dashed)X
2901(line)X
3056(styles)X
3279(in)X
3370(the)X
3500(speci\256ed)X
3 f
2239 6144(71)N

72 p
%%Page: 72 76
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(graphics)N
875(context.)X
1179(The)X
1338(de\256nition)X
1699(for)X
1823(this)X
1973(function)X
2289(is:)X
555 796(XSetDashes)N
991(\()X
2 f
1027(display)X
1 f
1282(,)X
2 f
1326(gc)X
1 f
1416(,)X
2 f
1460(dash_offset)X
1 f
1869(,)X
2 f
1913(dash_list)X
1 f
2239(,)X
2 f
2283(n)X
1 f
2334(\))X
731 892(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 988(GC)N
2 f
875(gc)X
1 f
965(;)X
731 1084(int)N
2 f
847(dash_offset)X
1 f
1256(;)X
731 1180(char)N
2 f
904(dash_list)X
1 f
1223([])X
1288(;)X
731 1276(int)N
2 f
847(n)X
1 f
898(;)X
2 f
555 1428(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1552(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1676(dash_offset)N
1 f
1131(Speci\256es)X
1471(the)X
1601(phase)X
1823(of)X
1918(the)X
2048(pattern)X
2315(for)X
2439(the)X
2569(dashed)X
2835(line)X
2990(style)X
3179(you)X
3333(want)X
3526(to)X
3617(set)X
3737(for)X
3861(the)X
1131 1772(speci\256ed)N
1466(graphics)X
1786(context.)X
2 f
555 1896(dash_list)N
1 f
1131(Speci\256es)X
1471(the)X
1601(dash)X
1784(list)X
1915(for)X
2039(the)X
2169(dashed)X
2435(line)X
2590(style)X
2779(you)X
2933(want)X
3126(to)X
3217(set)X
3337(for)X
3461(the)X
3591(speci\256ed)X
1131 1992(graphics)N
1451(context.)X
2 f
555 2116(n)N
1 f
1131(Speci\256es)X
1471(the)X
1601(length)X
1844(of)X
1939(the)X
2069(dash)X
2252(list)X
2383(argument.)X
555 2240(The)N
5 f
732(XSetDashes)X
1 f
1259(function)X
1575(sets)X
1729(the)X
1859(dash_offset)X
2286(and)X
2435(dash_list)X
2771(in)X
2862(the)X
2992(speci\256ed)X
3327(GC)X
3471(for)X
3595(dashed)X
3861(line)X
555 2336(styles.)N
822(Dashes)X
1097(cannot)X
1354(be)X
1459(empty.)X
1724(Otherwise,)X
2129(a)X
5 f
2208(BadValue)X
1 f
2627(error)X
2819(is)X
2900(generated.)X
3308(The)X
3467(initial)X
3697(and)X
555 2432(alternating)N
955(elements)X
1291(of)X
1386(the)X
1516(dash_list)X
1852(are)X
1981(the)X
2111(even)X
2299(dashes,)X
2577(the)X
2707(others)X
2944(are)X
3073(the)X
3203(odd)X
3357(dashes.)X
3657(All)X
3792(of)X
3887(the)X
555 2528(elements)N
891(must)X
1085(be)X
1190(nonzero.)X
1539(Otherwise,)X
1944(a)X
5 f
2023(BadValue)X
1 f
2442(error)X
2634(is)X
2715(generated.)X
3123(Specifying)X
3527(an)X
3632(odd-)X
555 2624(length)N
798(list)X
929(is)X
1010(equivalent)X
1400(to)X
1491(specifying)X
1880(the)X
2010(same)X
2213(list)X
2344(concatenated)X
2826(with)X
3005(itself)X
3204(to)X
3295(produce)X
3600(an)X
3705(even-)X
555 2720(length)N
798(list.)X
555 2844(You)N
728(probably)X
1063(will)X
1223(\256nd)X
1382(it)X
1454(easiest)X
1711(to)X
1802(specify)X
2078(the)X
2208(elements)X
2544(of)X
2639(the)X
2769(dash_list)X
3105(as)X
3200(octal)X
3394(or)X
3489(hex)X
3638(values.)X
555 2940(The)N
714(dash_offset)X
1141(de\256nes)X
1412(the)X
1542(phase)X
1764(of)X
1859(the)X
1989(pattern,)X
2278(specifying)X
2667(how)X
2840(many)X
3058(elements)X
3394(into)X
3554(the)X
3684(dash_list)X
555 3036(the)N
685(pattern)X
952(should)X
1209(actually)X
1511(begin)X
1729(in)X
1820(any)X
1969(single)X
2202(graphics)X
2522(request.)X
2842(N)X
2927(speci\256es)X
3252(the)X
3382(length)X
3625(of)X
3720(the)X
555 3132(dash_list.)N
935(Dashing)X
1250(is)X
1331(continuous)X
1740(through)X
2036(path)X
2210(elements)X
2546(combined)X
2916(with)X
3095(a)X
3156(join_style,)X
3549(but)X
3684(is)X
3765(reset)X
555 3228(to)N
646(the)X
776(dash_offset)X
1203(each)X
1386(time)X
1566(a)X
1627(cap_style)X
1982(is)X
2063(applied)X
2345(at)X
2431(a)X
2492(line)X
2647(endpoint.)X
555 3352(The)N
714(unit)X
874(of)X
969(measure)X
1284(for)X
1408(dashes)X
1664(is)X
1745(the)X
1875(same)X
2078(as)X
2173(in)X
2264(the)X
2394(ordinary)X
2714(coordinate)X
3108(system.)X
3419(Ideally,)X
3708(a)X
3769(dash)X
555 3448(length)N
798(is)X
879(measured)X
1238(along)X
1456(the)X
1586(slope)X
1794(of)X
1889(the)X
2019(line,)X
2196(but)X
2331 0.2500(implementations)AX
2943(are)X
3072(only)X
3251(required)X
3566(to)X
3657(match)X
555 3544(this)N
705(ideal)X
899(for)X
1023(horizontal)X
1403(and)X
1552(vertical)X
1839(lines.)X
2072(Failing)X
2345(the)X
2475(ideal)X
2669(semantics,)X
3061(it)X
3133(is)X
3214(suggested)X
3583(that)X
3738(the)X
555 3640(length)N
798(be)X
903(measured)X
1262(along)X
1480(the)X
1610(major)X
1838(axis)X
2002(of)X
2097(the)X
2227(line.)X
2426(The)X
2585(major)X
2813(axis)X
2977(is)X
3058(de\256ned)X
3339(as)X
3434(the)X
3564(x)X
3630(axis)X
3794(for)X
555 3736(lines)N
744(drawn)X
985(at)X
1071(an)X
1176(angle)X
1389(of)X
1484(between)X
1799(-45)X
1938(and)X
2087(+45)X
2247(degrees)X
2537(or)X
2632(between)X
2947(315)X
3101(and)X
3250(225)X
3404(degrees)X
3694(from)X
3887(the)X
555 3832(x)N
621(axis.)X
829(For)X
973(all)X
1084(other)X
1287(lines,)X
1498(the)X
1628(major)X
1856(axis)X
2020(is)X
2101(the)X
2231(y)X
2297(axis.)X
2505(The)X
2664(default)X
2931(dash_list)X
3267(in)X
3358(a)X
3419(newly)X
3656(created)X
555 3928(GC)N
699(is)X
780(equivalent)X
1170(to)X
1261(``)X
1363(04)X
1495(04''.)X
555 4052(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XSetDashes)X
1 f
2779(are)X
5 f
2926(BadAlloc)X
1 f
3310(and)X
5 f
3477(BadGC)X
1 f
3781(.)X
3 f
555 4244(5.4.3.)N
797(Setting)X
1083(the)X
1222(Fill)X
1373(Style)X
1581(and)X
1745(File)X
1910(Rule)X
2108(Components)X
1 f
555 4368(Use)N
5 f
731(XSetFillStyle)X
1 f
1267(to)X
1358(set)X
1478(the)X
1608(\256ll)X
1729(style)X
1918(in)X
2009(the)X
2139(speci\256ed)X
2474(graphics)X
2794(context.)X
3098(The)X
3257(de\256nition)X
3618(for)X
3742(this)X
555 4464(function)N
871(is:)X
555 4588(XSetFillStyle)N
1044(\()X
2 f
1080(display)X
1 f
1335(,)X
2 f
1379(gc)X
1 f
1469(,)X
2 f
1513(\256ll_style)X
1 f
1820(\))X
687 4684(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4780(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4876(int)N
2 f
803(\256ll_style)X
1 f
1110(;)X
2 f
555 5028(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5152(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5276(\256ll_style)N
1 f
1131(Speci\256es)X
1471(the)X
1601(\256ll)X
1722(style)X
1911(you)X
2065(want)X
2258(to)X
2349(set)X
2469(for)X
2593(the)X
2723(speci\256ed)X
3058(graphics)X
3378(context.)X
3704(Possible)X
1131 5372(values)N
1378(are)X
5 f
1525(FillSolid)X
1 f
1846(,)X
5 f
1908(FillTiled)X
1 f
2224(,)X
5 f
2286(FillStippled)X
1 f
2730(,)X
2774(or)X
5 f
2887(FillOpaqueStippled)X
1 f
3645(.)X
555 5496(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetFillStyle)X
1 f
2372(are)X
5 f
2519(BadGC)X
1 f
2845(and)X
5 f
3012(BadValue)X
1 f
3409(.)X
555 5716(Use)N
5 f
731(XSetFillRule)X
1 f
1251(to)X
1342(set)X
1462(the)X
1592(\256ll)X
1713(rule)X
1872(in)X
1963(the)X
2093(speci\256ed)X
2428(graphics)X
2748(context.)X
3052(The)X
3211(de\256nition)X
3572(for)X
3696(this)X
555 5812(function)N
871(is:)X
3 f
2239 6144(72)N

73 p
%%Page: 73 77
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XSetFillRule)N
1029(\()X
2 f
1065(display)X
1 f
1320(,)X
2 f
1364(gc)X
1 f
1454(,)X
2 f
1498(\256ll_rule)X
1 f
1785(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(GC)N
2 f
831(gc)X
1 f
921(;)X
687 960(int)N
2 f
803(\256ll_rule)X
1 f
1090(;)X
2 f
555 1112(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1236(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1360(\256ll_rule)N
1 f
1131(Speci\256es)X
1471(the)X
1601(\256ll)X
1722(rule)X
1881(you)X
2035(want)X
2228(to)X
2319(set)X
2439(for)X
2563(the)X
2693(speci\256ed)X
3028(graphics)X
3348(context.)X
3674(You)X
3847(can)X
1131 1456(pass)N
1304(one)X
1453(of)X
1548(these)X
1751(constants:)X
5 f
2144(EvenOddRule)X
1 f
2729(or)X
5 f
2842(WindingRule)X
1 f
3354(.)X
555 1580(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetFillRule)X
1 f
2356(are)X
5 f
2503(BadGC)X
1 f
2829(and)X
5 f
2996(BadValue)X
1 f
3393(.)X
3 f
555 1772(5.4.4.)N
797(Setting)X
1083(the)X
1222(Fill)X
1373(Tile)X
1543(and)X
1707(Stipple)X
1994(Components)X
1 f
555 1896(Some)N
778(hardware)X
1126(can)X
1270(support)X
1556(speci\256c)X
1847(size)X
2006(patterns)X
2307(for)X
2431(tiling)X
2641(or)X
2736(stippling)X
3068(that)X
3223(runs)X
3396(much)X
3614(faster)X
3831(than)X
555 1992(arbitrary)N
880(sizes.)X
1117(Xlib)X
1296(provides)X
1621(functions)X
1971(with)X
2150(which)X
2387(you)X
2541(can)X
2685(determine)X
3060(the)X
3190(best)X
3354(size,)X
3535(tile,)X
3693(or)X
3788(stip-)X
555 2088(ple)N
685(for)X
809(the)X
939(display)X
1216(as)X
1311(well)X
1485(as)X
1580(set)X
1700(the)X
1830(tile)X
1966(or)X
2061(stipple)X
2319(shape)X
2541(and)X
2690(the)X
2820 0.3239(tile/stipple)AX
3217(origin.)X
555 2308(Use)N
5 f
731(XQueryBestSize)X
1 f
1415(to)X
1506(obtain)X
1749(the)X
1879(best)X
2043(size.)X
2246(The)X
2405(de\256nition)X
2766(for)X
2890(this)X
3040(function)X
3356(is:)X
555 2432(Status)N
793(XQueryBestSize)X
1391(\()X
2 f
1427(display)X
1 f
1682(,)X
2 f
1726(class)X
1 f
(,)S
2 f
1946(which_screen)X
1 f
2430(,)X
2 f
2474(width)X
1 f
2671(,)X
2 f
2715(height)X
1 f
2936(,)X
2 f
2980(width_return)X
1 f
3441(,)X
2 f
3485(height_return)X
1 f
3977(\))X
687 2528(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2624(int)N
2 f
803(class)X
1 f
986(;)X
687 2720(Drawable)N
2 f
1050(which_screen)X
1 f
1541(;)X
687 2816(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1340(,)X
2 f
1384(height)X
1 f
1612(;)X
687 2912(unsigned)N
1027(int)X
1143(*)X
2 f
(width_return)S
1 f
1648(,)X
1692(*)X
2 f
(height_return)S
1 f
2228(;)X
2 f
555 3064(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3188(class)N
1 f
1131(Speci\256es)X
1471(the)X
1601(class)X
1794(that)X
1949(you)X
2103(are)X
2232(interested)X
2597(in.)X
2732(You)X
2905(can)X
3049(pass)X
3222(one)X
3371(of)X
3466(these)X
3669(con-)X
1131 3284(stants:)N
5 f
1397(TileShape)X
1 f
1808(,)X
5 f
1870(CursorShape)X
1 f
2403(,)X
2447(or)X
5 f
2560(StippleShape)X
1 f
3099(.)X
2 f
555 3408(which_screen)N
1 f
1131(Speci\256es)X
1471(any)X
1620(drawable)X
1964(on)X
2074(a)X
2135(screen.)X
2 f
555 3532(width)N
555 3628(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2 f
555 3752(width_return)N
555 3848(height_return)N
1 f
1131(Returns)X
1427(the)X
1557(width)X
1780(and)X
1929(height)X
2172(of)X
2267(the)X
2397(object)X
2635(best)X
2799(supported)X
3168(by)X
3278(the)X
3408(display)X
1131 3944(hardware.)N
555 4068(The)N
5 f
732(XQueryBestSize)X
1 f
1416(function)X
1732(returns)X
1998(the)X
2128(best)X
2292(or)X
2387(closest)X
2649(size)X
2808(to)X
2899(the)X
3029(speci\256ed)X
3364(size.)X
3567(For)X
5 f
3729(Cur-)X
555 4164(sorShape)N
1 f
947(,)X
991(this)X
1141(is)X
1222(the)X
1352(largest)X
1609(size)X
1768(that)X
1923(can)X
2067(be)X
2172(fully)X
2361(displayed)X
2721(on)X
2831(the)X
2961(screen)X
3207(speci\256ed)X
3542(by)X
555 4260(which_screen.)N
1104(For)X
5 f
1266(TileShape)X
1 f
1677(,)X
1721(this)X
1871(is)X
1952(the)X
2082(size)X
2241(that)X
2396(can)X
2540(be)X
2645(tiled)X
2825(fastest.)X
3116(For)X
5 f
3278(StippleShape)X
1 f
3817(,)X
3861(this)X
555 4356(is)N
636(the)X
766(size)X
925(that)X
1080(can)X
1224(be)X
1329(stippled)X
1631(fastest.)X
1922(For)X
5 f
2084(CursorShape)X
1 f
2617(,)X
2661(the)X
2791(drawable)X
3135(indicates)X
3471(the)X
3601(desired)X
555 4452(screen.)N
845(For)X
5 f
1007(TileShape)X
1 f
1440(and)X
5 f
1607(StippleShape)X
1 f
2146(,)X
2190(the)X
2320(drawable)X
2664(indicates)X
3000(the)X
3130(screen)X
3376(and)X
3525(possibly)X
3841(the)X
555 4548(window)N
860(class)X
1053(and)X
1202(depth.)X
1464(An)X
5 f
1611(InputOnly)X
1 f
2026(window)X
2331(cannot)X
2588(be)X
2693(used)X
2876(as)X
2971(the)X
3101(drawable)X
3445(for)X
5 f
3587(TileShape)X
1 f
555 4644(or)N
5 f
668(StippleShape)X
1 f
1207(.)X
1273(Otherwise,)X
1678(a)X
5 f
1757(BadMatch)X
1 f
2191(error)X
2383(is)X
2464(generated.)X
555 4768(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XQueryBestSize)X
1 f
2936(are)X
5 f
3083(BadDrawable)X
1 f
3625(,)X
5 f
3687(Bad-)X
555 4864(Match)N
1 f
810(,)X
854(and)X
5 f
1021(BadValue)X
1 f
1418(.)X
555 5084(Use)N
5 f
731(XQueryBestTile)X
1 f
1385(to)X
1476(obtain)X
1719(the)X
1849(best)X
2013(\256ll)X
2134(tile)X
2270(shape.)X
2536(The)X
2695(de\256nition)X
3056(for)X
3180(this)X
3330(function)X
3646(is:)X
555 5208(Status)N
793(XQueryBestTile)X
1382(\()X
2 f
1418(display)X
1 f
1673(,)X
2 f
1717(which_screen)X
1 f
2201(,)X
2 f
2245(width)X
1 f
2442(,)X
2 f
2486(height)X
1 f
2707(,)X
2 f
2751(width_return)X
1 f
3212(,)X
2 f
3256(height_return)X
1 f
3748(\))X
687 5304(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5400(Drawable)N
2 f
1050(which_screen)X
1 f
1541(;)X
687 5496(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1340(,)X
2 f
1384(height)X
1 f
1612(;)X
687 5592(unsigned)N
1027(int)X
1143(*)X
2 f
(width_return)S
1 f
1648(,)X
1692(*)X
2 f
(height_return)S
1 f
2228(;)X
2 f
555 5744(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2239 6144(73)N

74 p
%%Page: 74 78
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(which_screen)N
1 f
1131(Speci\256es)X
1471(any)X
1620(drawable)X
1964(on)X
2074(a)X
2135(screen.)X
2 f
555 796(width)N
555 892(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2 f
555 1016(width_return)N
555 1112(height_return)N
1 f
1131(Returns)X
1427(the)X
1557(width)X
1780(and)X
1929(height)X
2172(of)X
2267(the)X
2397(object)X
2635(best)X
2799(supported)X
3168(by)X
3278(the)X
3408(display)X
1131 1208(hardware.)N
555 1332(The)N
5 f
732(XQueryBestTile)X
1 f
1386(function)X
1702(returns)X
1968(the)X
2098(best)X
2262(or)X
2357(closest)X
2619(size,)X
2800(that)X
2955(is,)X
3058(the)X
3188(size)X
3347(that)X
3502(can)X
3646(be)X
3751(tiled)X
555 1428(fastest)N
802(on)X
912(the)X
1042(screen)X
1288(speci\256ed)X
1623(by)X
1733(which_screen.)X
2282(The)X
2441(drawable)X
2785(indicates)X
3121(the)X
3251(screen)X
3497(and)X
3646(possibly)X
555 1524(the)N
685(window)X
990(class)X
1183(and)X
1332(depth.)X
1594(An)X
5 f
1741(InputOnly)X
1 f
2156(window)X
2461(cannot)X
2718(be)X
2823(used)X
3006(as)X
3101(the)X
3231(drawable)X
3575(for)X
5 f
3717(XQuer-)X
555 1620(yBestTile)N
1 f
932(.)X
998(Otherwise,)X
1403(a)X
5 f
1482(BadMatch)X
1 f
1916(error)X
2108(is)X
2189(generated.)X
555 1744(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XQueryBestTile)X
1 f
2659(is)X
5 f
2758(BadDrawable)X
1 f
3300(.)X
555 1964(Use)N
5 f
731(XQueryBestStipple)X
1 f
1513(to)X
1604(obtain)X
1847(the)X
1977(best)X
2141(stipple)X
2399(shape.)X
2665(The)X
2824(de\256nition)X
3185(for)X
3309(this)X
3459(function)X
3775(is:)X
555 2088(Status)N
793(XQueryBestStipple)X
1490(\()X
2 f
1526(display)X
1 f
1781(,)X
2 f
1825(which_screen)X
1 f
2309(,)X
2 f
2353(width)X
1 f
2550(,)X
2 f
2594(height)X
1 f
2815(,)X
2 f
2859(width_return)X
1 f
3320(,)X
2 f
3364(height_return)X
1 f
3856(\))X
687 2184(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2280(Drawable)N
2 f
1050(which_screen)X
1 f
1541(;)X
687 2376(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1340(,)X
2 f
1384(height)X
1 f
1612(;)X
687 2472(unsigned)N
1027(int)X
1143(*)X
2 f
(width_return)S
1 f
1648(,)X
1692(*)X
2 f
(height_return)S
1 f
2228(;)X
2 f
555 2624(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2748(which_screen)N
1 f
1131(Speci\256es)X
1471(any)X
1620(drawable)X
1964(on)X
2074(a)X
2135(screen.)X
2 f
555 2872(width)N
555 2968(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2 f
555 3092(width_return)N
555 3188(height_return)N
1 f
1131(Returns)X
1427(the)X
1557(width)X
1780(and)X
1929(height)X
2172(of)X
2267(the)X
2397(object)X
2635(best)X
2799(supported)X
3168(by)X
3278(the)X
3408(display)X
1131 3284(hardware.)N
555 3408(The)N
5 f
732(XQueryBestStipple)X
1 f
1514(function)X
1830(returns)X
2096(the)X
2226(best)X
2390(or)X
2485(closest)X
2747(size,)X
2928(that)X
3083(is,)X
3186(the)X
3316(size)X
3475(that)X
3630(can)X
3774(be)X
555 3504(stippled)N
857(fastest)X
1104(on)X
1214(the)X
1344(screen)X
1590(speci\256ed)X
1925(by)X
2035(which_screen.)X
2584(The)X
2743(drawable)X
3087(indicates)X
3423(the)X
3553(screen)X
3799(and)X
555 3600(possibly)N
871(the)X
1001(window)X
1306(class)X
1499(and)X
1648(depth.)X
1910(An)X
5 f
2057(InputOnly)X
1 f
2472(window)X
2777(cannot)X
3034(be)X
3139(used)X
3322(as)X
3417(the)X
3547(drawable)X
3891(for)X
5 f
570 3696(XQueryBestStipple)N
1 f
1330(.)X
1396(Otherwise,)X
1801(a)X
5 f
1880(BadMatch)X
1 f
2314(error)X
2506(is)X
2587(generated.)X
555 3820(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XQueryBestStipple)X
1 f
2787(is)X
5 f
2886(BadDrawable)X
1 f
3428(.)X
555 4040(Use)N
5 f
731(XSetTile)X
1 f
1101(to)X
1192(set)X
1312(the)X
1442(\256ll)X
1563(tile)X
1699(in)X
1790(the)X
1920(speci\256ed)X
2255(graphics)X
2575(context.)X
2879(The)X
3038(de\256nition)X
3399(for)X
3523(this)X
3673(function)X
555 4136(is:)N
555 4260(XSetTile)N
881(\()X
2 f
917(display)X
1 f
1172(,)X
2 f
1216(gc)X
1 f
1306(,)X
2 f
1350(tile)X
1 f
1471(\))X
687 4356(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4452(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4548(Pixmap)N
2 f
979(tile)X
1 f
1100(;)X
2 f
555 4700(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4824(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 4948(tile)N
1 f
1131(Speci\256es)X
1471(the)X
1601(\256ll)X
1722(tile)X
1858(you)X
2012(want)X
2205(to)X
2296(set)X
2416(for)X
2540(the)X
2670(speci\256ed)X
3005(graphics)X
3325(context.)X
555 5072(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetTile)X
1 f
2206(are)X
5 f
2353(BadGC)X
1 f
2657(,)X
5 f
2719(BadPixmap)X
1 f
3184(,)X
5 f
3246(BadMatch)X
1 f
3658(,)X
3702(and)X
5 f
570 5168(BadAlloc)N
1 f
932(.)X
555 5388(Use)N
5 f
731(XSetStipple)X
1 f
1229(to)X
1320(set)X
1440(the)X
1570(stipple)X
1828(in)X
1919(the)X
2049(speci\256ed)X
2384(graphics)X
2704(context.)X
3008(The)X
3167(de\256nition)X
3528(for)X
3652(this)X
3802(func-)X
555 5484(tion)N
715(is:)X
3 f
2239 6144(74)N

75 p
%%Page: 75 79
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XSetStipple)N
989(\()X
2 f
1025(display)X
1 f
1280(,)X
2 f
1324(gc)X
1 f
1414(,)X
2 f
1458(stipple)X
1 f
1701(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(GC)N
2 f
831(gc)X
1 f
921(;)X
687 960(Pixmap)N
2 f
979(stipple)X
1 f
1222(;)X
2 f
555 1112(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1236(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1360(stipple)N
1 f
1131(Speci\256es)X
1471(the)X
1601(stipple)X
1859(you)X
2013(want)X
2206(to)X
2297(set)X
2417(for)X
2541(the)X
2671(speci\256ed)X
3006(graphics)X
3326(context.)X
555 1484(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetStipple)X
1 f
2334(are)X
5 f
2481(BadGC)X
1 f
2785(,)X
5 f
2847(BadPixmap)X
1 f
3312(,)X
5 f
3374(BadMatch)X
1 f
3786(,)X
3830(and)X
5 f
570 1580(BadAlloc)N
1 f
932(.)X
555 1800(Use)N
5 f
731(XSetTSOrigin)X
1 f
1307(to)X
1398(set)X
1518(the)X
1648(tile)X
1784(or)X
1879(stipple)X
2137(origin)X
2370(in)X
2461(the)X
2591(speci\256ed)X
2926(graphics)X
3246(context.)X
3572(The)X
555 1896(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 2020(XSetTSOrigin)N
1071(\()X
2 f
1107(display)X
1 f
1362(,)X
2 f
1406(gc)X
1 f
1496(,)X
2 f
1540(ts_x_origin)X
1 f
1949(,)X
2 f
1993(ts_y_origin)X
1 f
2402(\))X
687 2116(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2212(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2308(int)N
2 f
803(ts_x_origin)X
1 f
1212(,)X
2 f
1256(ts_y_origin)X
1 f
1665(;)X
2 f
555 2460(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2584(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2708(ts_x_origin)N
555 2804(ts_y_origin)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(tile)X
2622(or)X
2717(stipple)X
2975(origin.)X
555 2928(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetTSOrigin)X
1 f
2378(is)X
5 f
2477(BadGC)X
1 f
2781(.)X
3 f
555 3120(5.4.5.)N
797(Setting)X
1083(the)X
1222(Current)X
1551(Font)X
1 f
555 3244(Use)N
5 f
731(XSetFont)X
1 f
1137(to)X
1228(set)X
1348(the)X
1478(current)X
1749(font)X
1913(in)X
2004(the)X
2134(speci\256ed)X
2469(graphics)X
2789(context.)X
3093(The)X
3252(de\256nition)X
3613(for)X
3737(this)X
555 3340(function)N
871(is:)X
555 3464(XSetFont)N
900(\()X
2 f
936(display)X
1 f
1191(,)X
2 f
1235(gc)X
1 f
1325(,)X
2 f
1369(font)X
1 f
1514(\))X
687 3560(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3656(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3752(Font)N
2 f
871(font)X
1 f
1016(;)X
2 f
555 3904(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4028(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 4152(font)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(ID.)X
555 4276(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetFont)X
1 f
2242(are)X
5 f
2389(BadGC)X
1 f
2693(,)X
5 f
2755(BadFont)X
1 f
3104(,)X
3148(and)X
5 f
3315(BadAlloc)X
1 f
3677(.)X
3 f
555 4468(5.4.6.)N
797(Setting)X
1083(Clip)X
1267(Region)X
1 f
555 4592(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set)X
2243(the)X
2373(clip)X
2528(origin)X
2761(or)X
2856(the)X
2986(clip)X
3141(mask)X
3349(as)X
3444(well)X
3618(as)X
3713(with)X
555 4688(which)N
792(you)X
946(can)X
1090(set)X
1210(the)X
1340(clip)X
1495(mask)X
1703(to)X
1794(a)X
1855(list)X
1986(of)X
2081(rectangles.)X
555 4908(Use)N
5 f
731(XSetClipOrigin)X
1 f
1344(to)X
1435(set)X
1555(the)X
1685(clip)X
1840(origin)X
2073(in)X
2164(the)X
2294(speci\256ed)X
2629(graphics)X
2949(context.)X
3253(The)X
3412(de\256nition)X
3773(for)X
555 5004(this)N
705(function)X
1021(is:)X
555 5128(XSetClipOrigin)N
1121(\()X
2 f
1157(display)X
1 f
1412(,)X
2 f
1456(gc)X
1 f
1546(,)X
2 f
1590(clip_x_origin)X
1 f
2073(,)X
2 f
2117(clip_y_origin)X
1 f
2600(\))X
687 5224(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5320(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5416(int)N
2 f
803(clip_x_origin)X
1 f
1286(,)X
2 f
1330(clip_y_origin)X
1 f
1813(;)X
2 f
555 5568(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5692(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
3 f
2239 6144(75)N

76 p
%%Page: 76 80
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(clip_x_origin)N
555 768(clip_y_origin)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(clip)X
2641(origin.)X
555 892(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetClipOrigin)X
1 f
2415(is)X
5 f
2514(BadGC)X
1 f
2818(.)X
555 1112(Use)N
5 f
731(XSetClipMask)X
1 f
1320(to)X
1411(set)X
1531(the)X
1661(clip_mask)X
2046(in)X
2137(the)X
2267(speci\256ed)X
2602(graphics)X
2922(context)X
3204(to)X
3295(the)X
3425(speci\256ed)X
3760(pix-)X
555 1208(map.)N
751(The)X
910(de\256nition)X
1271(for)X
1395(this)X
1545(function)X
1861(is:)X
555 1332(XSetClipMask)N
1086(\()X
2 f
1122(display)X
1 f
1377(,)X
2 f
1421(gc)X
1 f
1504(,)X
2 f
1548(pixmap)X
1 f
1814(\))X
687 1428(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1524(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1620(Pixmap)N
2 f
979(pixmap)X
1 f
1245(;)X
2 f
555 1772(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1896(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2020(pixmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(pixmap.)X
555 2144(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetClipMask)X
1 f
2425(are)X
5 f
2572(BadGC)X
1 f
2876(,)X
5 f
2938(BadMatch)X
1 f
3350(,)X
3394(and)X
5 f
3561(BadValue)X
1 f
3958(.)X
555 2364(Use)N
5 f
731(XSetClipRectangles)X
1 f
1550(to)X
1641(change)X
1912(the)X
2042(clip_mask)X
2427(in)X
2518(the)X
2648(speci\256ed)X
2983(graphics)X
3303(context)X
3585(to)X
3676(the)X
555 2460(speci\256ed)N
890(list)X
1021(of)X
1116(rectangles.)X
1517(The)X
1676(de\256nition)X
2037(for)X
2161(this)X
2311(function)X
2627(is:)X
555 2584(XSetClipRectangles)N
1278(\()X
2 f
1314(display)X
1 f
1569(,)X
2 f
1613(gc)X
1 f
1703(,)X
2 f
1747(clip_x_origin)X
1 f
2230(,)X
2 f
2274(clip_y_origin)X
1 f
2757(,)X
2 f
2801(rectangles)X
1 f
3175(,)X
2 f
3219(n)X
1 f
3270(,)X
2 f
3314(ordering)X
1 f
3629(\))X
687 2680(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2776(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2872(int)N
2 f
803(clip_x_origin)X
1 f
1286(,)X
2 f
1330(clip_y_origin)X
1 f
1813(;)X
687 2968(XRectangle)N
2 f
1125(rectangles)X
1 f
1492([])X
1557(;)X
687 3064(int)N
2 f
803(n)X
1 f
854(;)X
687 3160(int)N
2 f
803(ordering)X
1 f
1118(;)X
2 f
555 3312(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3436(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3560(clip_x_origin)N
555 3656(clip_y_origin)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(clip)X
2641(origin.)X
2 f
555 3780(rectangles)N
1 f
1131(Speci\256es)X
1471(an)X
1576(array)X
1778(of)X
1873(rectangles.)X
2296(These)X
2528(are)X
2657(the)X
2787(rectangles)X
3166(you)X
3320(want)X
3513(to)X
3604(specify)X
3880(in)X
1131 3876(the)N
1261(graphics)X
1581(context.)X
2 f
555 4000(n)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(rectangles.)X
2 f
555 4124(ordering)N
1 f
1131(Speci\256es)X
1471(the)X
1601(ordering)X
1921(relations)X
2247(on)X
2357(the)X
2487(rectangles.)X
2910(Possible)X
3226(values)X
3473(are)X
5 f
1146 4220(Unsorted)N
1 f
1518(,)X
5 f
1580(YSorted)X
1 f
1914(,)X
5 f
1976(YXSorted)X
1 f
2369(,)X
2413(or)X
5 f
2526(YXBanded)X
1 f
2963(.)X
555 4344(The)N
5 f
732(XSetClipRectangles)X
1 f
1551(function)X
1867(changes)X
2172(the)X
2302(clip_mask)X
2687(in)X
2778(the)X
2908(speci\256ed)X
3243(graphics)X
3563(context)X
3845(to)X
555 4440(the)N
685(speci\256ed)X
1020(list)X
1151(of)X
1246(rectangles)X
1625(and)X
1774(sets)X
1928(the)X
2058(clip)X
2213(origin.)X
2490(The)X
2649(output)X
2897(is)X
2978(clipped)X
3260(to)X
3351(remain)X
3618(contained)X
555 4536(within)N
803(the)X
933(rectangles.)X
1356(The)X
1515(number)X
1806(of)X
1901(rectangles)X
2280(are)X
2409(speci\256ed)X
2744(with)X
2923(the)X
3053(n)X
3119(argument.)X
3496(The)X
3655(clip)X
3810(ori-)X
555 4632(gin)N
690(is)X
771(interpreted)X
1175(relative)X
1462(to)X
1553(the)X
1683(origin)X
1916(of)X
2011(whatever)X
2355(destination)X
2765(drawable)X
3109(is)X
3190(speci\256ed)X
3525(in)X
3616(a)X
3677(graphics)X
555 4728(request.)N
853(The)X
1012(rectangle)X
1357(coordinates)X
1785(are)X
1914(interpreted)X
2318(relative)X
2605(to)X
2696(the)X
2826(clip)X
2981(origin.)X
3236(The)X
3395(rectangles)X
3774(should)X
555 4824(be)N
660(nonintersecting,)X
1248(or)X
1343(the)X
1473(graphics)X
1793(results)X
2045(will)X
2205(be)X
2310(unde\256ned.)X
2723(Note)X
2916(that)X
3071(the)X
3201(list)X
3332(of)X
3427(rectangles)X
3806(can)X
555 4920(be)N
660(empty,)X
925(which)X
1162(effectively)X
1561(disables)X
1867(output.)X
2159(This)X
2338(is)X
2419(the)X
2549(opposite)X
2870(of)X
2965(passing)X
5 f
3269(None)X
1 f
3516(as)X
3611(the)X
555 5016(clip_mask)N
940(in)X
5 f
1049(XCreateGC)X
1 f
1541(and)X
5 f
1708(XChangeGC)X
1 f
2222(.)X
555 5140(If)N
635(known)X
896(by)X
1006(the)X
1136(client,)X
1377(ordering)X
1697(relations)X
2023(on)X
2133(the)X
2263(rectangles)X
2642(can)X
2786(be)X
2891(speci\256ed)X
3226(with)X
3405(the)X
3535(ordering)X
555 5236(argument.)N
932(This)X
1111(may)X
1285(provide)X
1576(faster)X
1793(operation)X
2148(by)X
2258(the)X
2388(server.)X
2646(If)X
2726(an)X
2831(incorrect)X
3166(ordering)X
3486(is)X
3567(speci\256ed,)X
555 5332(the)N
685(X)X
770(server)X
1006(may)X
1180(generate)X
1500(a)X
5 f
1579(BadMatch)X
1 f
2013(error,)X
2227(but)X
2362(it)X
2434(is)X
2515(not)X
2650(required)X
2965(to)X
3056(do)X
3166(so.)X
3310(If)X
3390(no)X
3500(error)X
3692(is)X
3773(gen-)X
555 5428(erated,)N
814(the)X
944(graphics)X
1264(results)X
1516(are)X
1645(unde\256ned.)X
5 f
2079(Unsorted)X
1 f
2473(means)X
2720(the)X
2850(rectangles)X
3229(are)X
3358(in)X
3449(arbitrary)X
3774(order.)X
5 f
570 5524(YSorted)N
1 f
926(means)X
1173(that)X
1328(the)X
1458(rectangles)X
1837(are)X
1966(nondecreasing)X
2496(in)X
2587(their)X
2771(Y)X
2856(origin.)X
5 f
3154(YXSorted)X
1 f
3569(additionally)X
555 5620(constrains)N
5 f
952(YSorted)X
1 f
1308(order)X
1515(in)X
1606(that)X
1761(all)X
1872(rectangles)X
2251(with)X
2430(an)X
2535(equal)X
2748(Y)X
2833(origin)X
3066(are)X
3195(nondecreasing)X
3725(in)X
3816(their)X
555 5716(X)N
640(origin.)X
5 f
913(YXBanded)X
1 f
1372(additionally)X
1817(constrains)X
5 f
2214(YXSorted)X
1 f
2629(by)X
2739(requiring)X
3084(that,)X
3261(for)X
3385(every)X
3602(possible)X
3913(Y)X
555 5812(scan)N
733(line,)X
910(all)X
1021(rectangles)X
1400(that)X
1555(include)X
1837(that)X
1992(scan)X
2170(line)X
2325(have)X
2513(an)X
2618(identical)X
2945(Y)X
3030(origins)X
3297(and)X
3446(Y)X
3531(extents.)X
3 f
2239 6144(76)N

77 p
%%Page: 77 81
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XSetClipRectangles)X
1 f
3071(are)X
5 f
3218(BadGC)X
1 f
3522(,)X
5 f
3584(BadAlloc)X
1 f
3946(,)X
555 768(and)N
5 f
722(BadValue)X
1 f
1119(.)X
2197 944(Note)N
775 1080(The)N
934(Xlib)X
1113(library)X
1370(provides)X
1695(a)X
1756(set)X
1876(of)X
1971(basic)X
2174(functions)X
2524(for)X
2648(performing)X
3066(region)X
3313(arithmetic.)X
775 1176(For)N
919(information)X
1358(about)X
1576(these)X
1779(functions,)X
2151(see)X
2285(Chapter)X
2586(10.)X
3 f
555 1448(5.4.7.)N
797(Setting)X
1083(the)X
1222(Arc)X
1385(Mode,)X
1644(Subwindow)X
2106(Mode,)X
2365(and)X
2529(Graphics)X
2899(Exposure)X
3278(components)X
1 f
555 1572(Use)N
5 f
731(XSetArcMode)X
1 f
1312(to)X
1403(set)X
1523(the)X
1653(arc)X
1782(mode)X
2000(in)X
2091(the)X
2221(speci\256ed)X
2556(graphics)X
2876(context.)X
3180(The)X
3339(de\256nition)X
3700(for)X
3824(this)X
555 1668(function)N
871(is:)X
555 1792(XSetArcMode)N
1074(\()X
2 f
1110(display)X
1 f
1365(,)X
2 f
1409(gc)X
1 f
1499(,)X
2 f
1543(arc_mode)X
1 f
1901(\))X
687 1888(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1984(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2080(int)N
2 f
803(arc_mode)X
1 f
1161(;)X
2 f
555 2232(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2356(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2480(arc_mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(arc)X
1730(mode:)X
5 f
1991(ArcChord)X
1 f
2399(speci\256es)X
2724(that)X
2879(arcs)X
3042(will)X
3202(be)X
3307(chord)X
3529(\256lled,)X
3755(while)X
5 f
1146 2576(ArcPieSlice)N
1 f
1632(speci\256es)X
1957(that)X
2112(arcs)X
2275(will)X
2435(be)X
2540(pie)X
2670(slice)X
2854(\256lled.)X
555 2700(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetArcMode)X
1 f
2417(are)X
5 f
2564(BadGC)X
1 f
2890(and)X
5 f
3057(BadValue)X
1 f
3454(.)X
555 2920(Use)N
5 f
731(XSetSubwindowMode)X
1 f
1629(to)X
1720(set)X
1840(the)X
1970(subwindow)X
2397(mode)X
2615(in)X
2706(the)X
2836(speci\256ed)X
3171(graphics)X
3491(context.)X
3795(The)X
555 3016(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 3140(XSetSubwindowMode)N
1363(\()X
2 f
1399(display)X
1 f
1654(,)X
2 f
1698(gc)X
1 f
1788(,)X
2 f
1832(subwindow_mode)X
1 f
2470(\))X
687 3236(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3332(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3428(int)N
2 f
803(subwindow_mode)X
1 f
1441(;)X
2 f
555 3580(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3704(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3828(subwindow_mode)N
1 f
1131 3924(Speci\256es)N
1471(the)X
1601(subwindow)X
2028(mode:)X
5 f
2289 -0.2356(ClipByChildren)AX
1 f
2905(clips)X
3094(source)X
3345(and)X
3494(destination)X
3904(by)X
1131 4020(all)N
1242(viewable)X
1582(children,)X
1915(while)X
5 f
2151(IncludeInferiors)X
1 f
2790(draws)X
3021(through)X
3317(all)X
3428(subwindows,)X
1131 4116(that)N
1286(is,)X
1389(does)X
1572(not)X
1707(clip)X
1862(by)X
1972(inferiors.)X
555 4240(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetSubwindowMode)X
1 f
2734(are)X
5 f
2881(BadGC)X
1 f
3207(and)X
5 f
3374(BadValue)X
1 f
3771(.)X
555 4460(Use)N
5 f
731(XSetGraphicsExposures)X
1 f
1728(to)X
1819(set)X
1939(the)X
2069(graphics-exposures)X
2769(\257ag)X
2923(in)X
3014(the)X
3144(speci\256ed)X
3479(graphics)X
3799(con-)X
555 4556(text.)N
732(The)X
891(de\256nition)X
1252(for)X
1376(this)X
1526(function)X
1842(is:)X
555 4680(XSetGraphicsExposures)N
1421(\()X
2 f
1457(display)X
1 f
1712(,)X
2 f
1756(gc)X
1 f
1846(,)X
2 f
1890(graphics_exposures)X
1 f
2600(\))X
687 4776(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4872(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4968(Boolean)N
2 f
1003(graphics_exposures)X
1 f
1713(;)X
2 f
555 5120(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5244(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5368(graphics_exposures)N
1 f
1131 5464(Speci\256es)N
1471(whether)X
1776(you)X
1930(want)X
5 f
2141(GraphicsExpose)X
1 f
2824(events)X
3071(to)X
3162(be)X
3267(reported)X
3582(when)X
3794(cal-)X
1131 5560(ling)N
5 f
1309(XCopyArea)X
1 f
1796(and)X
5 f
1963(XCopyPlane)X
1 f
2489(with)X
2668(this)X
2818(graphics)X
3138(context.)X
3464(If)X
5 f
3562(True)X
1 f
3758(,)X
5 f
3820(Gra-)X
1131 5656(phicsExpose)N
1 f
1667(events)X
1914(are)X
2043(reported.)X
2402(If)X
5 f
2500(False)X
1 f
2730(,)X
5 f
2792(GraphicsExpose)X
1 f
3475(events)X
3722(are)X
3851(not)X
1131 5752(reported.)N
3 f
2239 6144(77)N

78 p
%%Page: 78 82
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetGraphicsExposures)X
1 f
2833(are)X
5 f
2980(BadGC)X
1 f
3306(and)X
5 f
3473(BadValue)X
1 f
3870(.)X
3 f
2239 6144(78)N

79 p
%%Page: 79 83
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(6)X
1879 1056(Graphics)N
2282(Functions)X
1 f
11 s
555 1372(Once)N
762(you)X
916(have)X
1104(connected)X
1483(the)X
1613(display)X
1890(to)X
1981(the)X
2111(X)X
2196(server,)X
2454(you)X
2608(can)X
2752(use)X
2891(the)X
3021(Xlib)X
3200(graphics)X
3520(functions)X
3870(to:)X
10 f
555 1496(g)N
1 f
775(Clear)X
988(and)X
1137(copy)X
1330(areas)X
10 f
555 1620(g)N
1 f
775(Draw)X
991(points,)X
1251(lines,)X
1462(rectangles,)X
1863(and)X
2012(arcs)X
10 f
555 1744(g)N
1 f
775(Fill)X
921(areas)X
10 f
555 1868(g)N
1 f
775(Manipulate)X
1199(fonts)X
10 f
555 1992(g)N
1 f
775(Draw)X
991(text)X
1146(characters)X
10 f
555 2116(g)N
1 f
775(Transfer)X
1094(images)X
1366(between)X
1681(clients)X
1934(and)X
2083(server)X
10 f
555 2240(g)N
1 f
775(Manipulate)X
1199(cursors)X
2197 2416(Note)N
775 2552(It)N
851(is)X
932(generally)X
1282(more)X
1485(ef\256cient)X
1796(to)X
1887(call)X
2037(those)X
2245(graphics)X
2565(functions)X
2915(that)X
3070(take)X
3239(an)X
3344(array)X
3546(of)X
775 2648(arguments)N
1164(\(for)X
1317(example,)X
5 f
1678(XDrawPoints)X
1 f
2201(\))X
2252(once)X
2440(than)X
2614(it)X
2686(is)X
2767(to)X
2858(call)X
3008(those)X
3216(graphics)X
3536(func-)X
775 2744(tions)N
969(that)X
1124(do)X
1234(not)X
1369(\(for)X
1522(example,)X
5 f
1883(XDrawPoint)X
1 f
2362(\))X
2413(a)X
2474(number)X
2765(of)X
2860(times.)X
3 f
555 3016(6.1.)N
731(Clearing)X
1081(Areas)X
1 f
555 3140(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(clear)X
2316(an)X
2421(area)X
2589(or)X
2684(the)X
2814(entire)X
3037(drawable.)X
3425(Because)X
3740(pix-)X
555 3236(maps)N
763(do)X
873(not)X
1008(have)X
1196(de\256ned)X
1477(backgrounds,)X
1970(they)X
2144(cannot)X
2401(be)X
2506(\256lled)X
2710(by)X
2820(using)X
3033(the)X
3163(functions)X
3513(described)X
3872(in)X
555 3332(this)N
705(section.)X
1021(Instead,)X
1319(to)X
1410(accomplish)X
1834(an)X
1939(analogous)X
2318(operation)X
2673(on)X
2783(a)X
2844(pixmap,)X
3153(you)X
3307(should)X
3564(use)X
5 f
570 3428(XFillRectangle)N
1 f
1151(,)X
1195(which)X
1432(sets)X
1586(the)X
1716(pixmap)X
2003(to)X
2094(a)X
2155(known)X
2416(value.)X
2673(See)X
2822(Section)X
3109(6.4.1)X
3307(for)X
3431(information)X
3870(on)X
5 f
570 3524(XFillRectangle)N
1 f
1151(.)X
555 3744(Use)N
5 f
731(XClearArea)X
1 f
1222(to)X
1313(clear)X
1506(a)X
1567(speci\256ed)X
1902(rectangular)X
2320(area)X
2488(in)X
2579(the)X
2709(speci\256ed)X
3044(window.)X
3393(The)X
3552(de\256nition)X
555 3840(for)N
679(this)X
829(function)X
1145(is:)X
555 3964(XClearArea)N
986(\()X
2 f
1022(display)X
1 f
1277(,)X
2 f
1321(w)X
1 f
1387(,)X
2 f
1431(x)X
1 f
1477(,)X
2 f
1521(y)X
1 f
1567(,)X
2 f
1611(width)X
1 f
1815(,)X
2 f
1859(height)X
1 f
2087(,)X
2 f
2131(exposures)X
1 f
2489(\))X
687 4060(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4156(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 4252(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 4348(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 4444(Bool)N
2 f
881(exposures)X
1 f
1239(;)X
2 f
555 4596(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4720(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 4844(x)N
555 4940(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(are)X
3094(relative)X
3381(to)X
3472(the)X
3602(origin)X
3835(of)X
1131 5036(the)N
1261(window)X
1566(and)X
1715(specify)X
1991(the)X
2121(upper)X
2343(left)X
2483(corner)X
2729(of)X
2824(the)X
2954(rectangle.)X
2 f
555 5160(width)N
555 5256(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(dimensions)X
3126(of)X
3221(the)X
3351(rectangle)X
3696(to)X
3787(be)X
1131 5352(cleared.)N
2 f
555 5476(exposures)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(of)X
5 f
2159(True)X
1 f
2377(or)X
5 f
2490(False)X
1 f
2720(.)X
555 5600(The)N
5 f
732(XClearArea)X
1 f
1223(function)X
1539(clears)X
1766(a)X
1827(rectangular)X
2245(area)X
2413(in)X
2504(the)X
2634(speci\256ed)X
2969(window)X
3274(according)X
3643(to)X
3734(the)X
555 5696(speci\256ed)N
890(dimensions.)X
1358(If)X
1438(width)X
1661(is)X
1742(zero,)X
1937(it)X
2009(is)X
2090(replaced)X
2410(with)X
2589(the)X
2719(current)X
2990(width)X
3213(of)X
3308(the)X
3438(window)X
3743(minus)X
555 5792(x.)N
665(If)X
745(height)X
988(is)X
1069(zero,)X
1264(it)X
1336(is)X
1417(replaced)X
1737(with)X
1916(the)X
2046(current)X
2317(height)X
2560(of)X
2655(the)X
2785(window)X
3090(minus)X
3328(y.)X
3438(If)X
3518(the)X
3648(window)X
3 f
2239 6144(79)N

80 p
%%Page: 80 84
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(has)N
694(a)X
755(de\256ned)X
1036(background)X
1473(tile,)X
1631(the)X
1761(rectangle)X
2106(is)X
2187(\256lled)X
2391(with)X
2570(this)X
2720(tile.)X
2900(If)X
2980(the)X
3110(window)X
3415(has)X
3554(background)X
5 f
570 768(None)N
1 f
795(,)X
839(the)X
969(contents)X
1285(of)X
1380(the)X
1510(window)X
1815(are)X
1944(not)X
2079(changed.)X
2416(In)X
2511(either)X
2734(case,)X
2929(if)X
3005(exposures)X
3378(is)X
5 f
3477(True)X
1 f
3673(,)X
3717(one)X
3866(or)X
555 864(more)N
758(exposure)X
1097(events)X
1344(are)X
1473(generated)X
1837(for)X
1961(regions)X
2242(of)X
2337(the)X
2467(rectangle)X
2812(that)X
2967(are)X
3096(either)X
3319(visible)X
3577(or)X
3672(are)X
3801(being)X
555 960(retained)N
861(in)X
952(a)X
1013(backing)X
1314(store.)X
1551(A)X
5 f
1654(BadMatch)X
1 f
2088(error)X
2280(is)X
2361(generated)X
2725(if)X
2801(you)X
2955(specify)X
3231(a)X
3292(window)X
3597(whose)X
555 1056(class)N
748(is)X
5 f
847(InputOnly)X
1 f
1240(.)X
555 1180(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XClearArea)X
1 f
2743(are)X
5 f
2890(BadWindow)X
1 f
3374(,)X
5 f
3436(BadMatch)X
1 f
3848(,)X
555 1276(and)N
5 f
722(BadValue)X
1 f
1119(.)X
555 1496(Use)N
5 f
731(XClearWindow)X
1 f
1348(to)X
1439(clear)X
1632(the)X
1762(entire)X
1985(area)X
2153(in)X
2244(the)X
2374(speci\256ed)X
2709(window.)X
3058(The)X
3217(de\256nition)X
3578(for)X
3702(this)X
555 1592(function)N
871(is:)X
555 1716(XClearWindow)N
1119(\()X
2 f
1155(display)X
1 f
1410(,)X
2 f
1454(w)X
1 f
1520(\))X
687 1812(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1908(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 2060(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2184(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 2308(The)N
5 f
732(XClearWindow)X
1 f
1349(function)X
1665(clears)X
1892(the)X
2022(entire)X
2245(area)X
2413(in)X
2504(the)X
2634(speci\256ed)X
2969(window)X
3274(and)X
3423(is)X
3504(equivalent)X
3894(to)X
5 f
570 2404(XClearArea)N
1 f
1061(\(display,)X
1389(w,)X
1496(0,)X
1584(0,)X
1672(0,)X
1760(0,)X
5 f
1866(False)X
1 f
2096(\).)X
2191(If)X
2271(the)X
2401(window)X
2706(has)X
2845(a)X
2906(de\256ned)X
3187(background)X
3624(tile,)X
3782(the)X
555 2500(rectangle)N
900(is)X
981(tiled)X
1161(with)X
1340(a)X
1401(plane-mask)X
1829(of)X
1924(all)X
2035(ones)X
2218(and)X
2367(function)X
2683(of)X
5 f
2796(GXcopy)X
1 f
3125(.)X
3191(If)X
3271(the)X
3401(window)X
3706(has)X
555 2596(background)N
5 f
1010(None)X
1 f
1235(,)X
1279(the)X
1409(contents)X
1725(of)X
1820(the)X
1950(window)X
2255(are)X
2384(not)X
2519(changed.)X
2856(In)X
2951(either)X
3174(case,)X
3369(if)X
3445(exposures)X
3818(is)X
5 f
570 2692(True)N
1 f
766(,)X
810(one)X
959(or)X
1054(more)X
1257(exposure)X
1596(events)X
1843(are)X
1972(generated)X
2336(for)X
2460(regions)X
2741(of)X
2836(the)X
2966(rectangle)X
3311(that)X
3466(are)X
3595(either)X
3818(visi-)X
555 2788(ble)N
685(or)X
780(are)X
909(being)X
1127(retained)X
1433(in)X
1524(a)X
1585(backing)X
1886(store.)X
2123(A)X
5 f
2226(BadMatch)X
1 f
2660(error)X
2852(is)X
2933(generated)X
3297(if)X
3373(you)X
3527(specify)X
3803(a)X
555 2884(window)N
860(whose)X
1106(class)X
1299(is)X
5 f
1398(InputOnly)X
1 f
1791(.)X
555 3008(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XClearWindow)X
1 f
2869(are)X
5 f
3016(BadWindow)X
1 f
3500(,)X
5 f
3562(BadMatch)X
1 f
3974(,)X
555 3104(and)N
5 f
722(BadValue)X
1 f
1119(.)X
3 f
555 3296(6.2.)N
731(Copying)X
1071(Areas)X
1 f
555 3420(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(copy)X
2316(an)X
2421(area)X
2589(or)X
2684(a)X
2745(bit-plane.)X
555 3640(Use)N
5 f
731(XCopyArea)X
1 f
1218(to)X
1309(copy)X
1502(an)X
1607(area)X
1775(of)X
1870(the)X
2000(speci\256ed)X
2335(drawable)X
2679(between)X
2994(drawables)X
3372(of)X
3467(the)X
3597(same)X
3800(root)X
555 3736(and)N
704(depth.)X
966(The)X
1125(de\256nition)X
1486(for)X
1610(this)X
1760(function)X
2076(is:)X
555 3860(XCopyArea)N
986(\()X
2 f
1022(display)X
1 f
1277(,)X
2 f
1321(src)X
1 f
1435(,)X
2 f
1479(dest)X
1 f
1628(,)X
2 f
1672(gc)X
1 f
1762(,)X
2 f
1806(src_x)X
1 f
2003(,)X
2 f
2047(src_y)X
1 f
2244(,)X
2 f
2288(width)X
1 f
2492(,)X
2 f
2536(height)X
1 f
2764(,)X
2 f
2830(dest_x)X
1 f
3062(,)X
2 f
3106(dest_y)X
1 f
3338(\))X
687 3956(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4052(Drawable)N
2 f
1050(src)X
1 f
1164(,)X
2 f
1208(dest)X
1 f
1357(;)X
687 4148(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4244(int)N
2 f
803(src_x)X
1 f
1000(,)X
2 f
1044(src_y)X
1 f
1241(;)X
687 4340(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 4436(int)N
2 f
803(dest_x)X
1 f
1035(,)X
2 f
1079(dest_y)X
1 f
1311(;)X
2 f
555 4588(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4712(src)N
555 4808(dest)N
1 f
1131(Specify)X
1422(the)X
1552(source)X
1803(and)X
1952(destination)X
2362(rectangles)X
2741(to)X
2832(be)X
2937(combined.)X
2 f
555 4932(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5056(src_x)N
555 5152(src_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(source)X
2737(rectangle)X
3082(relative)X
3369(to)X
3460(its)X
3566(origin.)X
1131 5248(These)N
1363(coordinates)X
1791(specify)X
2067(the)X
2197(upper)X
2419(left)X
2559(corner)X
2805(of)X
2900(the)X
3030(source)X
3281(rectangle.)X
2 f
555 5372(width)N
555 5468(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(dimensions)X
3126(of)X
3221(both)X
3400(the)X
3530(source)X
3781(and)X
1131 5564(destination)N
1541(rectangles.)X
2 f
555 5688(dest_x)N
3 f
2239 6144(80)N

81 p
%%Page: 81 85
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(dest_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(destination)X
2896(rectangle)X
3241(relative)X
3528(to)X
3619(its)X
3725(origin.)X
1131 768(These)N
1363(coordinates)X
1791(specify)X
2067(the)X
2197(upper)X
2419(left)X
2559(corner)X
2805(of)X
2900(the)X
3030(destination)X
3440(rectangle.)X
555 892(The)N
5 f
732(XCopyArea)X
1 f
1219(function)X
1535(combines)X
1895(the)X
2025(speci\256ed)X
2360(rectangle)X
2705(of)X
2800(src)X
2924(with)X
3103(the)X
3233(speci\256ed)X
3568(rectangle)X
3913(of)X
555 988(dest.)N
763(The)X
922(rectangles)X
1301(speci\256ed)X
1636(by)X
1746(these)X
1949(two)X
2103(arguments)X
2492(must)X
2686(have)X
2874(the)X
3004(same)X
3207(root)X
3371(and)X
3520(depth.)X
3782(Other-)X
555 1084(wise,)N
760(a)X
5 f
839(BadMatch)X
1 f
1273(error)X
1465(is)X
1546(generated.)X
5 f
1975(XCopyArea)X
1 f
2462(uses)X
2635(these)X
2838(graphics)X
3158(context)X
3440(components:)X
555 1180(function,)N
893(plane_mask,)X
1358(subwindow_mode,)X
2047(graphics_exposures,)X
2784(clip_x_origin,)X
3304(clip_y_origin,)X
3824(and)X
555 1276(clip_mask.)N
555 1400(If)N
635(either)X
858(the)X
988(regions)X
1269(of)X
1364(the)X
1494(source)X
1745(rectangle)X
2090(are)X
2219(obscured)X
2558(and)X
2707(have)X
2895(not)X
3030(been)X
3218(retained)X
3524(in)X
555 1496(backing_store)N
1071(or)X
1166(the)X
1296(regions)X
1577(outside)X
1854(the)X
1984(boundaries)X
2392(of)X
2487(the)X
2617(source)X
2868(drawable)X
3212(are)X
3341(speci\256ed,)X
3698(those)X
555 1592(regions)N
836(are)X
965(not)X
1100(copied.)X
1379(Instead,)X
1677(the)X
1807(following)X
2172(occurs)X
2423(on)X
2533(all)X
2644(corresponding)X
3169(destination)X
3579(regions)X
3860(that)X
555 1688(are)N
684(either)X
907(visible)X
1165(or)X
1260(are)X
1389(retained)X
1695(in)X
1786(backing_store.)X
2324(If)X
2404(the)X
2534(destination)X
2944(rectangle)X
3289(is)X
3370(a)X
3431(window)X
3736(with)X
3915(a)X
555 1784(background)N
992(other)X
1195(than)X
5 f
1387(None)X
1 f
1612(,)X
1656(these)X
1859(corresponding)X
2384(regions)X
2665(of)X
2760(the)X
2890(destination)X
3300(are)X
3429(tiled)X
3609(\(with)X
555 1880(plane_mask)N
998(of)X
1093(all)X
1204(ones)X
1387(and)X
1536(function)X
5 f
1870(GXcopy)X
1 f
2199(\))X
2250(with)X
2429(that)X
2584(background.)X
3065(Regardless)X
3473(of)X
3568(tiling)X
3778(or)X
555 1976(whether)N
860(the)X
990(destination)X
1400(is)X
1481(a)X
1542(window)X
1847(or)X
1942(a)X
2003(pixmap,)X
2312(if)X
2388(graphics_exposures)X
3103(in)X
3194(GC)X
3338(is)X
5 f
3437(True)X
1 f
3633(,)X
3677(then)X
5 f
570 2072(GraphicsExpose)N
1 f
1253(events)X
1500(for)X
1624(all)X
1735(corresponding)X
2260(destination)X
2670(regions)X
2951(are)X
3080(generated.)X
3488(If)X
555 2168(graphics_exposures)N
1270(is)X
5 f
1369(True)X
1 f
1587(but)X
1722(no)X
1832(regions)X
2113(are)X
2242(exposed,)X
2574(a)X
5 f
2653(NoExpose)X
1 f
3096(event)X
3309(is)X
3390(generated.)X
3798(Note)X
555 2264(that)N
710(by)X
820(default,)X
1109(graphics_exposures)X
1824(is)X
5 f
1923(True)X
1 f
2141(on)X
2251(in)X
2342(new)X
2510(GCs.)X
2732(See)X
2881(Chapter)X
3182(8)X
3248(for)X
3372(further)X
3633(informa-)X
555 2360(tion.)N
555 2484(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCopyArea)X
1 f
2323(are)X
5 f
2470(BadDrawable)X
1 f
3012(,)X
5 f
3074(BadGC)X
1 f
3378(,)X
3422(and)X
5 f
3589(Bad-)X
555 2580(Match)N
1 f
810(.)X
555 2800(Use)N
5 f
731(XCopyPlane)X
1 f
1257(to)X
1348(copy)X
1541(a)X
1602(single)X
1835(bit-plane)X
2171(of)X
2266(the)X
2396(speci\256ed)X
2731(drawable.)X
3119(The)X
3278(drawable)X
3622(may)X
3796(have)X
555 2896(different)N
880(depths.)X
1176(The)X
1335(de\256nition)X
1696(for)X
5 f
1838(XCopyPlane)X
1 f
2364(is:)X
555 3020(XCopyPlane)N
1012(\()X
2 f
1048(display)X
1 f
1303(,)X
2 f
1347(src)X
1 f
1461(,)X
2 f
1505(dest)X
1 f
1654(,)X
2 f
1698(gc)X
1 f
1788(,)X
2 f
1832(src_x)X
1 f
2029(,)X
2 f
2073(src_y)X
1 f
2270(,)X
2 f
2314(width)X
1 f
2518(,)X
2 f
2562(height)X
1 f
2790(,)X
2 f
2834(dest_x)X
1 f
3066(,)X
2 f
3110(dest_y)X
1 f
3342(,)X
2 f
3386(plane)X
1 f
3589(\))X
687 3116(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3212(Drawable)N
2 f
1050(src)X
1 f
1164(,)X
2 f
1208(dest)X
1 f
1357(;)X
687 3308(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3404(int)N
2 f
803(src_x)X
1 f
1000(,)X
2 f
1044(src_y)X
1 f
1241(;)X
687 3500(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 3596(int)N
2 f
803(dest_x)X
1 f
1035(,)X
2 f
1079(dest_y)X
1 f
1311(;)X
687 3692(unsigned)N
1027(long)X
2 f
1206(plane)X
1 f
1409(;)X
2 f
555 3844(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3968(src)N
555 4064(dest)N
1 f
1131(Specify)X
1422(the)X
1552(source)X
1803(and)X
1952(destination)X
2362(rectangles)X
2741(to)X
2832(be)X
2937(combined.)X
2 f
555 4188(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 4312(src_x)N
555 4408(src_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(source)X
2737(rectangle)X
3082(relative)X
3369(to)X
3460(its)X
3566(origin.)X
1131 4504(These)N
1363(coordinates)X
1791(specify)X
2067(the)X
2197(upper)X
2419(left)X
2559(corner)X
2805(of)X
2900(the)X
3030(source)X
3281(rectangle.)X
2 f
555 4628(width)N
555 4724(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(dimensions)X
3126(of)X
3221(both)X
3400(the)X
3530(source)X
3781(and)X
1131 4820(destination)N
1541(rectangles.)X
2 f
555 4944(dest_x)N
555 5040(dest_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(destination)X
2896(rectangle)X
3241(relative)X
3528(to)X
3619(its)X
3725(origin.)X
1131 5136(These)N
1363(coordinates)X
1791(specify)X
2067(the)X
2197(upper)X
2419(left)X
2559(corner)X
2805(of)X
2900(the)X
3030(destination)X
3440(rectangle.)X
2 f
555 5260(plane)N
1 f
1131(Speci\256es)X
1471(the)X
1601(bit-plane.)X
1981(You)X
2154(must)X
2348(set)X
2468(exactly)X
2745(one)X
2894(bit.)X
555 5384(The)N
5 f
732(XCopyPlane)X
1 f
1258(function)X
1574(combines)X
1934(a)X
1995(single)X
2228(bit)X
2344(plane)X
2557(of)X
2652(the)X
2782(speci\256ed)X
3117(rectangle)X
3462(of)X
3557(src)X
3681(with)X
3860(the)X
555 5480(speci\256ed)N
890(rectangle)X
1235(of)X
1330(dest.)X
1538(The)X
1697(rectangles)X
2076(speci\256ed)X
2411(by)X
2521(these)X
2724(two)X
2878(arguments)X
3267(must)X
3461(have)X
3649(the)X
3779(same)X
555 5576(root)N
719(but)X
854(need)X
1042(not)X
1177(have)X
1365(the)X
1495(same)X
1698(depth.)X
1960(If)X
2040(the)X
2170(rectangles)X
2549(do)X
2659(not)X
2794(have)X
2982(the)X
3112(same)X
3315(root,)X
3501(a)X
5 f
3580(BadMatch)X
1 f
555 5672(error)N
747(is)X
828(generated.)X
1236(This)X
1415(function)X
1731(is)X
1812(identical)X
2139(to)X
5 f
2248(XCopyArea)X
1 f
2713(,)X
2757(except)X
3009(that)X
3164(only)X
3343(one)X
3492(bit-plane)X
3828(is)X
555 5768(copied.)N
856(If)X
936(the)X
1066(bit-plane)X
1402(does)X
1585(not)X
1720(have)X
1908(exactly)X
2185(one)X
2334(bit)X
2450(set,)X
2592(a)X
5 f
2671(BadValue)X
1 f
3090(error)X
3282(is)X
3363(generated.)X
3 f
2239 6144(81)N

82 p
%%Page: 82 86
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Effectively,)N
991(the)X
1121(function)X
1437(forms)X
1664(a)X
1725(pixmap)X
2012(of)X
2107(the)X
2237(same)X
2440(depth)X
2658(as)X
2753(the)X
2883(rectangle)X
3228(of)X
3323(dest)X
3487(and)X
3636(with)X
3815(a)X
555 768(size)N
714(speci\256ed)X
1049(by)X
1159(the)X
1289(source)X
1540(region.)X
1809(The)X
1968(function)X
2284(uses)X
2457(the)X
2587(foreground/background)X
3439(pixels)X
3672(in)X
3763(the)X
555 864(graphics)N
875(context)X
1157(\(foreground)X
1598(everywhere)X
2029(the)X
2159(bit-plane)X
2495(in)X
2586(src)X
2710(contains)X
3026(a)X
3087(one)X
3236(bit,)X
3374(background)X
555 960(everywhere)N
986(the)X
1116(bit-plane)X
1452(in)X
1543(src)X
1667(contains)X
1983(a)X
2044(zero)X
2217(bit\))X
2362(and)X
2511(the)X
2641(equivalent)X
3031(of)X
3126(a)X
5 f
3205(XCopyArea)X
1 f
3692(request)X
555 1056(is)N
636(performed)X
1024(with)X
1203(all)X
1314(the)X
1444(same)X
1647(exposure)X
1986(semantics.)X
2400(This)X
2579(can)X
2723(also)X
2887(be)X
2992(thought)X
3284(of)X
3379(as)X
3474(using)X
3687(the)X
555 1152(speci\256ed)N
890(region)X
1137(of)X
1232(the)X
1362(source)X
1613(bit-plane)X
1949(as)X
2044(a)X
2105(stipple)X
2363(with)X
2542(a)X
2603(\256ll_style)X
2935(of)X
5 f
3048(FillOpaqueStippled)X
1 f
3828(for)X
555 1248(\256lling)N
789(a)X
850(rectangular)X
1268(area)X
1436(of)X
1531(the)X
1661(destination.)X
555 1372(Regardless)N
963(of)X
1058(tiling)X
1268(or)X
1363(whether)X
1668(the)X
1798(destination)X
2208(is)X
2289(a)X
2350(window)X
2655(or)X
2750(a)X
2811(pixmap,)X
3120(if)X
3196(graphics_exposures)X
3911(in)X
555 1468(GC)N
699(is)X
5 f
798(True)X
1 f
994(,)X
1038(the)X
5 f
1186(GraphicsExpose)X
1 f
1869(events)X
2116(for)X
2240(all)X
2351(corresponding)X
2876(destination)X
3286(regions)X
3567(are)X
3696(gen-)X
555 1564(erated.)N
836(If)X
916(graphics_exposures)X
1631(is)X
5 f
1730(True)X
1 f
1948(but)X
2083(no)X
2193(regions)X
2474(are)X
2603(exposed,)X
2935(a)X
5 f
3014(NoExpose)X
1 f
3457(event)X
3670(is)X
3751(gen-)X
555 1660(erated.)N
836(Note)X
1029(that)X
1184(by)X
1294(default,)X
1583(graphics_exposures)X
2298(is)X
5 f
2397(True)X
1 f
2615(on)X
2725(in)X
2816(new)X
2984(GCs.)X
3206(See)X
3355(Chapter)X
3656(8)X
3722(for)X
555 1756(further)N
816(information.)X
5 f
570 1880(XCopyPlane)N
1 f
1096(uses)X
1269(these)X
1472(graphics)X
1792(context)X
2074(components:)X
2547(function,)X
2885(plane_mask,)X
3350(foreground,)X
3784(back-)X
555 1976(ground,)N
848(subwindow_mode,)X
1537(graphics_exposures,)X
2274(clip_x_origin,)X
2794(clip_y_origin,)X
3314(and)X
3463(clip_mask.)X
555 2100(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCopyPlane)X
1 f
2778(are)X
5 f
2925(BadDrawable)X
1 f
3467(,)X
5 f
3529(BadMatch)X
1 f
3941(,)X
5 f
570 2196(BadValue)N
1 f
967(,)X
1011(and)X
5 f
1178(BadGC)X
1 f
1482(.)X
3 f
555 2388(6.3.)N
731(Drawing)X
1080(Points,)X
1359(Lines,)X
1609(Rectangles,)X
2058(and)X
2222(Arcs)X
1 f
555 2512(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(draw:)X
10 f
555 2636(g)N
1 f
775(A)X
860(single)X
1093(point)X
1297(or)X
1392(multiple)X
1710(points)X
10 f
555 2760(g)N
1 f
775(A)X
860(single)X
1093(line)X
1248(or)X
1343(multiple)X
1661(lines)X
10 f
555 2884(g)N
1 f
775(A)X
860(single)X
1093(rectangle)X
1438(or)X
1533(multiple)X
1851(rectangles)X
10 f
555 3008(g)N
1 f
775(A)X
860(single)X
1093(arc)X
1222(or)X
1317(multiple)X
1635(arcs)X
555 3132(Some)N
778(of)X
873(the)X
1003(functions)X
1353(described)X
1712(in)X
1803(the)X
1933(following)X
2298(subsections)X
2726(use)X
2865(these)X
3068(structures:)X
555 3256(typedef)N
841(struct)X
1059({)X
643 3352(short)N
841(x1,)X
973(y1,)X
1105(x2,)X
1237(y2;)X
643 3448(})N
707(XSegment;)X
555 3592(typedef)N
841(struct)X
1059({)X
643 3688(short)N
841(x,)X
929(y;)X
643 3784(})N
707(XPoint;)X
555 3928(typedef)N
841(struct)X
1059({)X
643 4024(short)N
841(x,)X
929(y;)X
643 4120(unsigned)N
983(short)X
1181(width,)X
1426(height;)X
643 4216(})N
707(XRectangle;)X
555 4360(typedef)N
841(struct)X
1059({)X
643 4456(short)N
841(x,)X
929(y;)X
643 4552(unsigned)N
983(short)X
1181(width,)X
1426(height;)X
643 4648(short)N
841(angle1,)X
1120(angle2;)X
1666(/*)X
1757(Degrees)X
2066(*)X
2132(64)X
2242(*/)X
643 4744(})N
707(XArc;)X
555 4984(All)N
690(x)X
756(and)X
905(y)X
971(members)X
1316(are)X
1445(signed)X
1697(integers.)X
2042(The)X
2201(width)X
2424(and)X
2573(height)X
2816(members)X
3161(are)X
3290(16-bit)X
3523(unsigned)X
555 5080(integers.)N
900(Applications)X
1373(should)X
1630(be)X
1735(careful)X
2001(not)X
2136(to)X
2227(generate)X
2547(coordinates)X
2975(and)X
3124(sizes)X
3317(out)X
3452(of)X
3547(the)X
3677(16-bit)X
555 5176(ranges,)N
828(as)X
923(the)X
1053(protocol)X
1369(only)X
1548(has)X
1687(16)X
1797(bit)X
1913(\256elds)X
2126(for)X
2250(these)X
2453(values.)X
3 f
555 5368(6.3.1.)N
797(Drawing)X
1146(Single)X
1399(and)X
1563(Multiple)X
1909(Points)X
1 f
555 5492(Use)N
5 f
731(XDrawPoint)X
1 f
1232(to)X
1323(draw)X
1520(a)X
1581(single)X
1814(point)X
2018(or)X
5 f
2131(XDrawPoints)X
1 f
2676(to)X
2767(draw)X
2964(multiple)X
3282(points)X
3520(in)X
3611(the)X
555 5588(speci\256ed)N
890(drawable.)X
555 5712(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawPoint)X
1 f
1718(is:)X
3 f
2239 6144(82)N

83 p
%%Page: 83 87
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XDrawPoint)N
1006(\()X
2 f
1042(display)X
1 f
1297(,)X
2 f
1341(d)X
1 f
1392(,)X
2 f
1436(gc)X
1 f
1526(,)X
2 f
1570(x)X
1 f
1616(,)X
2 f
1660(y)X
1 f
1706(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 960(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1056(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
2 f
555 1208(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1332(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1456(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1580(x)N
555 1676(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(where)X
2497(you)X
2651(want)X
2844(the)X
2974(point)X
3178(drawn.)X
555 1896(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawPoints)X
1 f
1762(is:)X
555 2020(XDrawPoints)N
1040(\()X
2 f
1076(display)X
1 f
1331(,)X
2 f
1375(d)X
1 f
1426(,)X
2 f
1470(gc)X
1 f
1560(,)X
2 f
1604(points)X
1 f
1827(,)X
2 f
1871(npoints)X
1 f
2138(,)X
2 f
2182(mode)X
1 f
2379(\))X
687 2116(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2212(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 2308(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2404(XPoint)N
959(*)X
2 f
(points)S
1 f
1226(;)X
687 2500(int)N
2 f
803(npoints)X
1 f
1070(;)X
687 2596(int)N
2 f
803(mode)X
1 f
1000(;)X
2 f
555 2748(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2872(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2996(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3120(points)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(points.)X
2 f
555 3244(npoints)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(points)X
2225(in)X
2316(the)X
2446(array.)X
2 f
555 3368(mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(coordinate)X
1995(mode.)X
5 f
2253(CoordModeOrigin)X
1 f
2983(treats)X
3196(a)X
3257(coordinates)X
3685(as)X
1131 3464(related)N
1393(to)X
1484(the)X
1614(origin,)X
1869(while)X
5 f
2105(CoordModePrevious)X
1 f
2943(treates)X
3195(all)X
3306(coordinates)X
3734(after)X
1131 3560(the)N
1261(\256rst)X
1420(as)X
1515(relative)X
1802(to)X
1893(the)X
2023(previous)X
2348(point.)X
555 3684(The)N
5 f
732(XDrawPoint)X
1 f
1233(and)X
5 f
1400(XDrawPoints)X
1 f
1945(functions)X
2295(use)X
2434(these)X
2637(graphics)X
2957(context)X
3239(components:)X
3712(func-)X
555 3780(tion,)N
737(plane_mask,)X
1202(foreground,)X
1636(subwindow_mode,)X
2325(clip_x_origin,)X
2845(clip_y_origin,)X
3365(and)X
3514(clip_mask.)X
555 3876(The)N
5 f
732(XDrawPoint)X
1 f
1233(function)X
1549(uses)X
1722(the)X
1852(foreground)X
2264(pixel)X
2463(and)X
2612(function)X
2928(components)X
3376(of)X
3471(the)X
3601(graphics)X
555 3972(context)N
837(to)X
928(draw)X
1125(a)X
1186(single)X
1419(point)X
1623(into)X
1783(the)X
1913(speci\256ed)X
2248(drawable,)X
2614(while)X
5 f
2850(XDrawPoints)X
1 f
3395(draws)X
3626(multiple)X
555 4068(points)N
793(into)X
953(the)X
1083(speci\256ed)X
1418(drawable.)X
1806(These)X
2038(functions)X
2388(are)X
2517(not)X
2652(affected)X
2957(by)X
3067(the)X
3197(tile)X
3333(or)X
3428(stipple)X
3686(in)X
3777(the)X
555 4164(graphics)N
875(context.)X
555 4288(When)N
787(using)X
5 f
1018(XDrawPoint)X
1 f
1497(,)X
1541(you)X
1695(simply)X
1958(specify)X
2234(the)X
2364(x)X
2430(and)X
2579(y)X
2645(coordinates)X
3073(where)X
3309(you)X
3463(want)X
3656(the)X
3786(point)X
555 4384(drawn.)N
840(For)X
5 f
1002(XDrawPoints)X
1 f
1525(,)X
1569(you)X
1723(specify)X
1999(an)X
2104(array)X
2306(of)X
5 f
2419(XPoint)X
1 f
2716(structures,)X
3102(each)X
3285(of)X
3380(which)X
3617(contains)X
555 4480(an)N
660(x)X
726(and)X
875(y)X
941(coordinate.)X
5 f
1400(XDrawPoints)X
1 f
1945(draws)X
2176(the)X
2306(points)X
2544(in)X
2635(the)X
2765(order)X
2972(listed)X
3186(in)X
3277(the)X
3407(array.)X
5 f
570 4604(XDrawPoints)N
1 f
1115(requires)X
1420(a)X
1481(mode)X
1699(argument)X
2054(that)X
2209(indicates)X
2545(whether)X
2850(the)X
2980(points)X
3218(are)X
3347(relative)X
3634(to)X
3725(the)X
555 4700(drawable's)N
962(origin)X
1195(or)X
1290(to)X
1381(the)X
1511(previous)X
1836(point:)X
10 f
555 4824(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModeOrigin)X
1 f
1880(,)X
1924(all)X
2035(points)X
2273(after)X
2456(the)X
2586(\256rst)X
2745(are)X
2874(relative)X
3161(to)X
3252(the)X
3382(drawable's)X
3789(ori-)X
775 4920(gin.)N
932(\(The)X
1120(\256rst)X
1279(point)X
1483(is)X
1564(always)X
1830(relative)X
2117(to)X
2208(the)X
2338(drawable's)X
2745(origin.\))X
10 f
555 5044(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModePrevious)X
1 f
1988(,)X
2032(all)X
2143(points)X
2381(after)X
2564(the)X
2694(\256rst)X
2853(are)X
2982(relative)X
3269(to)X
3360(the)X
3490(previous)X
775 5140(point.)N
555 5264(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawPoint)X
1 f
2337(are)X
5 f
2484(BadDrawable)X
1 f
3026(,)X
5 f
3088(BadGC)X
1 f
3392(,)X
3436(and)X
5 f
3603(Bad-)X
555 5360(Match)N
1 f
810(.)X
876(The)X
1035(errors)X
1261(that)X
1416(can)X
1560(be)X
1665(generated)X
2029(by)X
5 f
2157(XDrawPoint)X
1 f
2658(are)X
5 f
2805(BadDrawable)X
1 f
3347(,)X
5 f
3409(BadGC)X
1 f
3713(,)X
5 f
3775(Bad-)X
555 5456(Value)N
1 f
795(,)X
839(and)X
5 f
1006(BadMatch)X
1 f
1418(.)X
3 f
2239 6144(83)N

84 p
%%Page: 84 88
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(6.3.2.)N
797(Drawing)X
1146(Single)X
1399(and)X
1563(Multiple)X
1909(Lines)X
1 f
555 796(Use)N
5 f
731(XDrawLine)X
1 f
1197(to)X
1288(draw)X
1485(a)X
1546(single)X
1779(line)X
1934(between)X
2249(two)X
2403(points)X
2641(in)X
2732(the)X
2862(speci\256ed)X
3197(drawable,)X
5 f
3581(XDraw-)X
555 892(Lines)N
1 f
802(to)X
893(draw)X
1090(multiple)X
1408(lines)X
1597(in)X
1688(the)X
1818(speci\256ed)X
2153(drawable,)X
2519(or)X
5 f
2632(XDrawSegments)X
1 f
3329(to)X
3420(draw)X
3617(multiple,)X
555 988(but)N
690(not)X
825(necessarily)X
1238(connected)X
1617(lines)X
1806(in)X
1897(the)X
2027(speci\256ed)X
2362(drawable.)X
555 1112(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawLine)X
1 f
1683(is:)X
555 1236(XDrawLine)N
981(\()X
2 f
1017(display)X
1 f
1272(,)X
2 f
1316(d)X
1 f
1367(,)X
2 f
1411(gc)X
1 f
1501(,)X
2 f
1545(x1)X
1 f
1635(,)X
2 f
1679(y1)X
1 f
1769(,)X
2 f
1813(x2)X
1 f
1903(,)X
2 f
1947(y2)X
1 f
2037(\))X
687 1332(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1428(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 1524(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1620(int)N
2 f
803(x1)X
1 f
893(,)X
2 f
937(y1)X
1 f
1027(,)X
2 f
1071(x2)X
1 f
1161(,)X
2 f
1205(y2)X
1 f
1295(;)X
2 f
555 1772(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1896(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2020(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2144(x1)N
555 2240(y1)N
555 2336(x2)N
555 2432(y2)N
1 f
1131(Specify)X
1422(the)X
1552(points)X
1790(used)X
1973(to)X
2064(connect)X
2360(the)X
2490(line.)X
2689(Thus,)X
5 f
2927(XDrawLine)X
1 f
3393(draws)X
3624(a)X
3685(line)X
3840(con-)X
1131 2528(necting)N
1413(point)X
1617(x1,)X
1749(y1)X
1859(to)X
1950(point)X
2154(x2,)X
2286(y2.)X
555 2748(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawLines)X
1 f
1727(is:)X
555 2872(XDrawLines)N
1015(\()X
2 f
1051(display)X
1 f
1306(,)X
2 f
1350(d)X
1 f
1401(,)X
2 f
1445(gc)X
1 f
1535(,)X
2 f
1579(points)X
1 f
1802(,)X
2 f
1846(npoints)X
1 f
2113(,)X
2 f
2157(mode)X
1 f
2354(\))X
687 2968(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3064(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 3160(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3256(XPoint)N
959(*)X
2 f
(points)S
1 f
1226(;)X
687 3352(int)N
2 f
803(npoints)X
1 f
1070(;)X
687 3448(int)N
2 f
803(mode)X
1 f
1000(;)X
2 f
555 3600(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3724(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 3848(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3972(points)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(points.)X
2 f
555 4096(npoints)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(points)X
2225(in)X
2316(the)X
2446(array.)X
2 f
555 4220(mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(coordinate)X
1995(mode.)X
5 f
2253(CoordModeOrigin)X
1 f
2983(treats)X
3196(a)X
3257(coordinates)X
3685(as)X
1131 4316(related)N
1393(to)X
1484(the)X
1614(origin,)X
1869(while)X
5 f
2105(CoordModePrevious)X
1 f
2943(treates)X
3195(all)X
3306(coordinates)X
3734(after)X
1131 4412(the)N
1261(\256rst)X
1420(as)X
1515(relative)X
1802(to)X
1893(the)X
2023(previous)X
2348(point.)X
555 4632(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawSegments)X
1 f
1914(is:)X
555 4756(XDrawSegments)N
1162(\()X
2 f
1198(display)X
1 f
1453(,)X
2 f
1497(d)X
1 f
1548(,)X
2 f
1592(gc)X
1 f
1682(,)X
2 f
1726(segments)X
1 f
2055(,)X
2 f
2099(nsegments)X
1 f
2472(\))X
687 4852(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4948(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5044(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5140(XSegment)N
1081(*)X
2 f
(segments)S
1 f
1454(;)X
687 5236(int)N
2 f
803(nsegments)X
1 f
1176(;)X
2 f
555 5388(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5512(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 5636(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
3 f
2239 6144(84)N

85 p
%%Page: 85 89
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(segments)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(segments.)X
2 f
555 796(nsegments)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(segments)X
2337(in)X
2428(the)X
2558(array.)X
555 920(The)N
5 f
732(XDrawLine)X
1 f
1198(function)X
1514(uses)X
1687(the)X
1817(components)X
2265(of)X
2360(the)X
2490(speci\256ed)X
2825(graphics)X
3145(context)X
3427(to)X
3518(draw)X
3715(a)X
3776(line)X
555 1016(between)N
870(the)X
1000(speci\256ed)X
1335(set)X
1455(of)X
1550(points)X
1788(\(x1,)X
1949(y1)X
2059(and)X
2208(x2,)X
2340(y2\).)X
2523(No)X
2652(joining)X
2925(is)X
3006(performed)X
3394(at)X
3480(coincident)X
3870(end)X
555 1112(points.)N
837(For)X
981(any)X
1130(given)X
1348(line,)X
1525(no)X
1635(pixel)X
1834(is)X
1915(drawn)X
2156(more)X
2359(than)X
2533(once.)X
2765(If)X
2845(lines)X
3034(intersect,)X
3377(the)X
3507(intersecting)X
555 1208(pixels)N
788(are)X
917(drawn)X
1158(multiple)X
1476(times.)X
555 1332(The)N
5 f
732(XDrawLines)X
1 f
1242(function)X
1558(uses)X
1731(the)X
1861(components)X
2309(of)X
2404(the)X
2534(speci\256ed)X
2869(graphics)X
3189(context)X
3471(to)X
3562(draw)X
555 1428(npoints-1)N
910(lines)X
1099(between)X
1414(each)X
1597(pair)X
1756(of)X
1851(points)X
2089(\(point[i],)X
2427(point[i+1]\))X
2837(in)X
2928(the)X
3058(array)X
3260(of)X
5 f
3373(XPoint)X
1 f
3670(struc-)X
555 1524(tures.)N
792(The)X
951(lines)X
1140(are)X
1269(drawn)X
1510(in)X
1601(the)X
1731(order)X
1938(listed)X
2152(in)X
2243(the)X
2373(array.)X
2619(The)X
2778(lines)X
2967(join)X
3127(correctly)X
3462(at)X
3548(all)X
3659(inter-)X
555 1620(mediate)N
857(points,)X
1117(and)X
1266(if)X
1342(the)X
1472(\256rst)X
1631(and)X
1780(last)X
1925(points)X
2163(coincide,)X
2506(the)X
2636(\256rst)X
2795(and)X
2944(last)X
3089(lines)X
3278(also)X
3442(join)X
3602(correctly.)X
555 1716(For)N
699(any)X
848(given)X
1066(line,)X
1243(no)X
1353(pixel)X
1552(is)X
1633(drawn)X
1874(more)X
2077(than)X
2251(once.)X
2483(If)X
2563(thin)X
2723(\(zero)X
2925(line)X
3080(width\))X
3332(lines)X
3521(intersect,)X
3864(the)X
555 1812(intersecting)N
989(pixels)X
1222(will)X
1382(be)X
1487(drawn)X
1728(multiple)X
2046(times.)X
2304(If)X
2384(wide)X
2577(lines)X
2766(intersect,)X
3109(the)X
3239(intersecting)X
3673(pixels)X
555 1908(are)N
684(drawn)X
925(only)X
1104(once,)X
1314(as)X
1409(though)X
1676(the)X
1806(entire)X
2029(PolyLine)X
2375(were)X
2567(a)X
2628(single)X
2861(\256lled)X
3065(shape.)X
5 f
3352(XDrawLines)X
1 f
555 2004(requires)N
860(a)X
921(mode)X
1139(argument)X
1494(to)X
1585(determine)X
1960(whether)X
2265(the)X
2395(points)X
2633(are)X
2762(relative)X
3049(to)X
3140(the)X
3270(drawable's)X
3677(origin)X
3910(or)X
555 2100(to)N
646(the)X
776(previous)X
1101(point:)X
10 f
555 2224(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModeOrigin)X
1 f
1880(,)X
1924(all)X
2035(points)X
2273(after)X
2456(the)X
2586(\256rst)X
2745(are)X
2874(relative)X
3161(to)X
3252(the)X
3382(drawable's)X
3789(ori-)X
775 2320(gin.)N
954(\(The)X
1142(\256rst)X
1301(point)X
1505(is)X
1586(always)X
1852(relative)X
2139(to)X
2230(the)X
2360(drawable's)X
2767(origin.\))X
10 f
555 2444(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModePrevious)X
1 f
1988(,)X
2032(all)X
2143(points)X
2381(after)X
2564(the)X
2694(\256rst)X
2853(are)X
2982(relative)X
3269(to)X
3360(the)X
3490(previous)X
775 2540(point.)N
555 2664(The)N
5 f
732(XDrawSegments)X
1 f
1429(function)X
1745(draws)X
1976(multiple,)X
2316(but)X
2451(not)X
2586(necessarily)X
2999(connected)X
3378(lines.)X
3589(For)X
3733(each)X
555 2760(segment,)N
5 f
911(XDrawSegments)X
1 f
1608(draws)X
1839(a)X
1900(line)X
2055(between)X
2370(\(x1,)X
2531(y1\))X
2670(and)X
2819(\(x2,)X
2980(y2\).)X
3163(The)X
3322(lines)X
3511(are)X
3640(drawn)X
3881(in)X
555 2856(the)N
685(order)X
892(listed)X
1106(in)X
1197(the)X
1327(array)X
1529(of)X
5 f
1642(XSegment)X
1 f
2091(structures.)X
2477(No)X
2606(joining)X
2879(is)X
2960(performed)X
3348(at)X
3434(coincident)X
3824(end)X
555 2952(points.)N
837(For)X
981(any)X
1130(given)X
1348(line,)X
1525(no)X
1635(pixel)X
1834(is)X
1915(drawn)X
2156(more)X
2359(than)X
2533(once.)X
2743(If)X
2823(lines)X
3012(intersect,)X
3355(the)X
3485(intersecting)X
555 3048(pixels)N
788(are)X
917(drawn)X
1158(multiple)X
1476(times.)X
555 3172(All)N
690(three)X
888(functions)X
1238(use)X
1377(these)X
1580(graphics)X
1900(context)X
2182(components:)X
2655(function,)X
2993(plane_mask,)X
3458(line_width,)X
555 3268(line_style,)N
943(cap_style,)X
1320(\256ll_style,)X
1674(subwindow_mode,)X
2363(clip_x_origin,)X
2883(clip_y_origin,)X
3403(and)X
3552(clip_mask.)X
555 3364(The)N
5 f
732(XDrawLines)X
1 f
1242(function)X
1558(also)X
1722(uses)X
1895(the)X
2025(join_style)X
2396(graphics)X
2716(context)X
2998(component.)X
3456(Additionally,)X
555 3460(all)N
666(three)X
864(functions)X
1214(use)X
1353(these)X
1556(graphics)X
1876(context)X
2158(mode-dependent)X
2767(components:)X
3240(foreground,)X
3674(back-)X
555 3556(ground,)N
848(tile,)X
1006(stipple,)X
1286(ts_x_origin,)X
1732(ts_y_origin,)X
2178(dash_offset,)X
2627(and)X
2776(dash_list.)X
3156(See)X
3305(the)X
3435(general)X
3716(discus-)X
555 3652(sion)N
724(under)X
5 f
964(XCreateGC)X
1 f
1456(in)X
1547(Chapter)X
1848(5.)X
555 3776(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawLine)X
1 f
2280(,)X
5 f
2342(XDrawLines)X
1 f
2830(,)X
2874(and)X
5 f
3041(XDrawSegments)X
1 f
3738(are)X
5 f
570 3872(BadDrawable)N
1 f
1112(,)X
5 f
1174(BadGC)X
1 f
1478(,)X
1522(and)X
5 f
1689(BadMatch)X
1 f
2101(.)X
5 f
2188(XDrawLines)X
1 f
2698(can)X
2842(also)X
3006(return)X
5 f
3256(BadValue)X
1 f
3653(.)X
3 f
555 4064(6.3.3.)N
797(Drawing)X
1146(Single)X
1399(and)X
1563(Multiple)X
1909(Rectangles)X
1 f
555 4188(Use)N
5 f
731(XDrawRectangle)X
1 f
1427(to)X
1518(draw)X
1715(the)X
1845(outline)X
2113(of)X
2208(a)X
2269(single)X
2502(rectangle)X
2847(or)X
5 f
2960(XDrawRectangles)X
1 f
3700(to)X
3791(draw)X
555 4284(the)N
685(outline)X
953(of)X
1048(multiple)X
1366(rectangles)X
1745(in)X
1836(the)X
1966(speci\256ed)X
2301(drawable.)X
555 4408(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawRectangle)X
1 f
1913(is:)X
555 4532(XDrawRectangle)N
1172(\()X
2 f
1208(display)X
1 f
1463(,)X
2 f
1507(d)X
1 f
1558(,)X
2 f
1602(gc)X
1 f
1692(,)X
2 f
1736(x)X
1 f
1782(,)X
2 f
1826(y)X
1 f
1872(,)X
2 f
1916(width)X
1 f
2120(,)X
2 f
2164(height)X
1 f
2392(\))X
687 4628(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4724(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 4820(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4916(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 5012(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
2 f
555 5164(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5288(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 5412(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5536(x)N
555 5632(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(that)X
2416(de\256ne)X
2653(the)X
2783(upper)X
3005(left)X
3145(corner)X
3391(of)X
3486(the)X
3616(rectangle.)X
3 f
2239 6144(85)N

86 p
%%Page: 86 90
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(width)N
555 768(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height)X
2167(that)X
2322(de\256ne)X
2559(the)X
2689(outline)X
2957(of)X
3052(the)X
3182(rectangle.)X
555 988(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawRectangles)X
1 f
1957(is:)X
555 1112(XDrawRectangles)N
1206(\()X
2 f
1242(display)X
1 f
1497(,)X
2 f
1541(d)X
1 f
1592(,)X
2 f
1636(gc)X
1 f
1726(,)X
2 f
1770(rectangles)X
1 f
2144(,)X
2 f
2188(nrectangles)X
1 f
2606(\))X
687 1208(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1304(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 1400(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1496(XRectangle)N
2 f
1125(rectangles)X
1 f
1499([)X
1535(])X
1571(;)X
687 1592(int)N
2 f
803(nrectangles)X
1 f
1221(;)X
2 f
555 1744(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1868(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1992(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2116(rectangles)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(rectangles.)X
2 f
555 2240(nrectangles)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(rectangles)X
2366(in)X
2457(the)X
2587(array.)X
555 2364(The)N
5 f
732(XDrawRectangle)X
1 f
1428(and)X
5 f
1595(XDrawRectangles)X
1 f
2335(functions)X
2685(draw)X
2882(the)X
3012(outlines)X
3314(of)X
3409(the)X
3539(speci\256ed)X
3874(rec-)X
555 2460(tangle)N
793(or)X
888(rectangles)X
1267(as)X
1362(if)X
1438(a)X
1499(\256ve-point)X
1864(PolyLine)X
2210(were)X
2402(speci\256ed)X
2737(for)X
2861(each)X
3044(rectangle:)X
755 2584([x,y,])N
967([x+width,y])X
1408([x+width,y+height])X
2120([x,y+height])X
2581([x,y])X
555 2708(For)N
699(the)X
829(speci\256ed)X
1164(rectangle)X
1509(or)X
1604(rectangles,)X
2005(no)X
2115(pixel)X
2314(is)X
2395(drawn)X
2636(more)X
2839(than)X
3013(once.)X
3245(The)X
3404(x)X
3470(and)X
3619(y)X
3685(coordi-)X
555 2804(nates)N
758(of)X
853(each)X
1036(rectangle)X
1381(are)X
1510(relative)X
1797(to)X
1888(the)X
2018(drawable's)X
2425(origin)X
2658(and)X
2807(de\256ne)X
3044(the)X
3174(upper)X
3396(left)X
3536(corner)X
3782(of)X
3877(the)X
555 2900(rectangle.)N
5 f
940(XDrawRectangles)X
1 f
1680(draws)X
1911(the)X
2041(rectangles)X
2420(in)X
2511(the)X
2641(order)X
2848(listed)X
3062(in)X
3153(the)X
3283(array.)X
3529(If)X
3609(rectangles)X
555 2996(intersect,)N
898(the)X
1028(intersecting)X
1462(pixels)X
1695(are)X
1824(drawn)X
2065(multiple)X
2383(times.)X
5 f
570 3120(XDrawRectangle)N
1 f
1266(and)X
5 f
1433(XDrawRectangles)X
1 f
2173(use)X
2312(these)X
2515(graphics)X
2835(context)X
3117(components:)X
3590(function,)X
555 3216(plane_mask,)N
1020(line_width,)X
1442(line_style,)X
1830(join_style,)X
2223(\256ll_style,)X
2577(subwindow_mode,)X
3266(clip_x_origin,)X
555 3312(clip_y_origin,)N
1075(and)X
1224(clip_mask.)X
1653(Both)X
1847(functions)X
2197(also)X
2361(use)X
2500(these)X
2703(graphics)X
3023(context)X
3305(mode-dependent)X
555 3408(components:)N
1028(foreground,)X
1462(background,)X
1921(tile,)X
2079(stipple,)X
2359(ts_x_origin,)X
2805(ts_y_origin,)X
3251(dash_offset,)X
3700(and)X
555 3504(dash_list.)N
935(See)X
1084(the)X
1214(general)X
1495(discussion)X
1884(under)X
5 f
2124(XCreateGC)X
1 f
2616(in)X
2707(Chapter)X
3008(5.)X
555 3628(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawRectangle)X
1 f
2532(and)X
5 f
2699(XDrawRectangles)X
1 f
3439(are)X
5 f
3586(BadDraw-)X
555 3724(able)N
1 f
736(,)X
5 f
798(BadGC)X
1 f
1102(,)X
1146(and)X
5 f
1313(BadMatch)X
1 f
1725(.)X
3 f
555 3916(6.3.4.)N
797(Drawing)X
1146(Single)X
1399(and)X
1563(Multiple)X
1909(Arcs)X
1 f
555 4040(Use)N
5 f
731(XDrawArc)X
1 f
1163(to)X
1254(draw)X
1451(a)X
1512(single)X
1745(arc)X
1874(or)X
5 f
1987(XDrawArcs)X
1 f
2463(to)X
2554(draw)X
2751(multiple)X
3069(arcs)X
3232(in)X
3323(the)X
3453(speci\256ed)X
3788(draw-)X
555 4136(able.)N
555 4356(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawArc)X
1 f
1649(is:)X
555 4480(XDrawArc)N
950(\()X
2 f
986(display)X
1 f
1241(,)X
2 f
1285(d)X
1 f
1336(,)X
2 f
1380(gc)X
1 f
1470(,)X
2 f
1514(x)X
1 f
1560(,)X
2 f
1604(y)X
1 f
1650(,)X
2 f
1694(width)X
1 f
1898(,)X
2 f
1942(height)X
1 f
2170(,)X
2 f
2214(angle1)X
1 f
2461(,)X
2 f
2505(angle2)X
1 f
2752(\))X
687 4576(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4672(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 4768(GC)N
2 f
831(gc)X
1 f
921(;)X
687 4864(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 4960(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 5056(int)N
2 f
803(angle1)X
1 f
1050(,)X
2 f
1094(angle2)X
1 f
1341(;)X
2 f
555 5208(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5332(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 5456(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 5580(x)N
555 5676(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(are)X
2666(the)X
2796(coordinates)X
3224(of)X
3319(the)X
3449(arc)X
3578(and)X
3727(are)X
1131 5772(relative)N
1418(to)X
1509(the)X
1639(origin)X
1872(of)X
1967(the)X
2097(drawable.)X
2485(These)X
2717(coordinates)X
3145(also)X
3309(specify)X
3585(the)X
3715(upper)X
3 f
2239 6144(86)N

87 p
%%Page: 87 91
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
1131 672(left)N
1271(corner)X
1517(of)X
1612(the)X
1742(rectangle.)X
2 f
555 796(width)N
555 892(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(major)X
2930(and)X
3079(minor)X
3312(axes)X
3490(of)X
3585(the)X
3715(arc.)X
2 f
555 1016(angle1)N
1 f
1131(Speci\256es)X
1471(the)X
1601(start)X
1775(of)X
1870(the)X
2000(arc)X
2129(relative)X
2416(to)X
2507(the)X
2637(three-o-clock)X
3128(position)X
3435(from)X
3628(the)X
3758(center,)X
1131 1112(in)N
1222(units)X
1416(of)X
1511(degrees)X
1801(*)X
1867(64.)X
2 f
555 1236(angle2)N
1 f
1131(Speci\256es)X
1471(the)X
1601(path)X
1775(and)X
1924(extent)X
2162(of)X
2257(the)X
2387(arc)X
2516(relative)X
2803(to)X
2894(the)X
3024(start)X
3198(of)X
3293(the)X
3423(arc,)X
3574(in)X
3665(units)X
3859(of)X
1131 1332(degrees)N
1421(*)X
1487(64.)X
555 1552(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawArcs)X
1 f
1693(is:)X
555 1676 -0.4063(XDrawArcs)AN
984(\()X
2 f
1020(display)X
1 f
1275(,)X
2 f
1319(d)X
1 f
1370(,)X
2 f
1414(gc)X
1 f
1504(,)X
2 f
1548(arcs)X
1 f
1706(,)X
2 f
1750(narcs)X
1 f
1952(\))X
687 1772(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1868(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 1964(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2060(XArc)N
903(*)X
2 f
(arcs)S
1 f
1105(;)X
687 2156(int)N
2 f
803(narcs)X
1 f
1005(;)X
2 f
555 2308(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2432(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2556(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2680(arcs)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(arcs.)X
2 f
555 2804(narcs)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arcs)X
2150(in)X
2241(the)X
2371(array.)X
5 f
570 2928(XDrawArc)N
1 f
1002(draws)X
1233(a)X
1294(single)X
1527(circular)X
1818(or)X
1913 0.3542(elliptical)AX
2246(arc,)X
2397(while)X
5 f
2633(XDrawArcs)X
1 f
3109(draws)X
3340(multiple)X
3658(circular)X
555 3024(or)N
650 0.3542(elliptical)AX
983(arcs.)X
1190(Each)X
1388(arc)X
1517(is)X
1598(speci\256ed)X
1933(by)X
2043(a)X
2104(rectangle)X
2449(and)X
2598(two)X
2752(angles.)X
3021(The)X
3180(x)X
3246(and)X
3395(y)X
3461(coordinates)X
3889(are)X
555 3120(relative)N
842(to)X
933(the)X
1063(origin)X
1296(of)X
1391(the)X
1521(drawable)X
1865(and)X
2014(de\256ne)X
2251(the)X
2381(upper)X
2603(left)X
2743(corner)X
2989(of)X
3084(the)X
3214(rectangle.)X
3581(The)X
3740(center)X
555 3216(of)N
650(the)X
780(circle)X
998(or)X
1093(ellipse)X
1346(is)X
1427(the)X
1557(center)X
1794(of)X
1889(the)X
2019(rectangle,)X
2386(and)X
2535(the)X
2665(major)X
2893(and)X
3042(minor)X
3275(axes)X
3453(are)X
3582(speci\256ed)X
3917(by)X
555 3312(the)N
685(width)X
908(and)X
1057(height,)X
1322(respectively.)X
1792(The)X
1951(angles)X
2198(are)X
2327(signed)X
2579(integers)X
2880(in)X
2971(degrees)X
3261(scaled)X
3503(by)X
3613(64)X
3723(with)X
555 3408(positive)N
857(indicating)X
1233(counterclockwise)X
1871(motion)X
2144(and)X
2293(negative)X
2614(indicating)X
2990(clockwise)X
3364(motion.)X
3659(The)X
3818(start)X
555 3504(of)N
650(the)X
780(arc)X
909(is)X
990(speci\256ed)X
1325(by)X
1435(angle1)X
1692(relative)X
1979(to)X
2070(the)X
2200(three-o-clock)X
2691(position)X
2998(from)X
3191(the)X
3321(center)X
3558(of)X
3653(the)X
3783(rec-)X
555 3600(tangle,)N
815(and)X
964(the)X
1094(path)X
1268(and)X
1417(extent)X
1655(of)X
1750(the)X
1880(arc)X
2009(are)X
2138(speci\256ed)X
2473(by)X
2583(angle2)X
2840(relative)X
3127(to)X
3218(the)X
3348(start)X
3522(of)X
3617(the)X
3747(arc.)X
3920(If)X
555 3696(the)N
685(magnitude)X
1080(of)X
1175(angle2)X
1432(is)X
1513(greater)X
1779(than)X
1953(360)X
2107(degrees,)X
5 f
2437(XDrawArc)X
1 f
2869(or)X
5 f
2982(XDrawArcs)X
1 f
3458(truncates)X
3798(it)X
3870(to)X
555 3792(360)N
709(degrees.)X
555 3916(The)N
714(x)X
780(and)X
929(y)X
995(coordinates)X
1423(of)X
1518(the)X
1648(rectangle)X
1993(are)X
2122(relative)X
2409(to)X
2500(the)X
2630(origin)X
2863(of)X
2958(the)X
3088(drawable.)X
3476(For)X
3620(an)X
3725(arc)X
555 4012(speci\256ed)N
890(as)X
985([x,y,w,h,a1,a2],)X
1558(the)X
1688(origin)X
1921(of)X
2016(the)X
2146(major)X
2374(and)X
2523(minor)X
2756(axes)X
2934(is)X
3015(at)X
3101([x+\(w/2\),y+\(h/2\)],)X
3774(and)X
555 4108(the)N
685(in\256nitely)X
1027(thin)X
1187(path)X
1361(describing)X
1750(the)X
1880(entire)X
2103(circle)X
2321(or)X
2416(ellipse)X
2669(intersects)X
3024(the)X
3154(horizontal)X
3534(axis)X
3698(at)X
555 4204([x,y+\(h/2\)])N
966(and)X
1115([x+w,y+\(h/2\)])X
1639(and)X
1788(intersects)X
2143(the)X
2273(vertical)X
2560(axis)X
2724(at)X
2810([x+\(w/2\),y])X
3240(and)X
3389([x+\(w/2\),y+h].)X
555 4300(These)N
787(coordinates)X
1215(can)X
1359(be)X
1464(fractional.)X
1846(That)X
2030(is,)X
2133(they)X
2307(are)X
2436(not)X
2571(truncated)X
2921(to)X
3012(discrete)X
3308(coordinates.)X
3780(The)X
555 4396(path)N
729(should)X
986(be)X
1091(de\256ned)X
1372(by)X
1482(the)X
1612(ideal)X
1806 0.2784(mathematical)AX
2305(path.)X
2501(For)X
2645(a)X
2706(wide)X
2899(line)X
3054(with)X
3233(line-width)X
3618(lw,)X
3750(the)X
555 4492(bounding)N
910(outlines)X
1212(for)X
1336(\256lling)X
1570(are)X
1699(given)X
1917(by)X
2027(the)X
2157(in\256nitely)X
2499(thin)X
2659(paths)X
2867(describing)X
3256(the)X
3386(arcs:)X
843 4664([x+dx/2,y+dy/2,w-dx,h-dy,a1,a2])N
1019 4808(and)N
843 4952([x-lw/2,y-lw/2,w+lw,h+lw,a1,a2])N
1019 5096(where)N
843 5240(dx=min\(lw,w\))N
843 5336(dy=min\(lw,h\))N
555 5508(For)N
699(an)X
804(arc)X
933(speci\256ed)X
1268(as)X
1363([x,y,w,h,a1,a2],)X
1936(the)X
2066(angles)X
2313(must)X
2507(be)X
2612(speci\256ed)X
2947(in)X
3038(the)X
3168(effectively)X
3567(skewed)X
555 5604(coordinate)N
949(system)X
1216(of)X
1311(the)X
1441(ellipse)X
1694(\(for)X
1847(a)X
1908(circle,)X
2148(the)X
2278(angles)X
2525(and)X
2674(coordinate)X
3068(systems)X
3369(are)X
3498(identical\).)X
555 5700(The)N
714(relationship)X
1153(between)X
1468(these)X
1671(angles)X
1918(and)X
2067(angles)X
2314(expressed)X
2682(in)X
2773(the)X
2903(normal)X
3175(coordinate)X
3569(system)X
3836(of)X
555 5796(the)N
685(screen)X
931(\(as)X
1055(measured)X
1414(with)X
1593(a)X
1654(protractor\))X
2052(is)X
2133(as)X
2228(follows:)X
3 f
2239 6144(87)N

88 p
%%Page: 88 92
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
843 720(skewed-angle)N
1348(=)X
1420(atan\(tan\(normal-angle\))X
2254(*)X
2320(w/h\))X
2503(+)X
2575(adjust)X
555 864(The)N
714(skewed-angle)X
1219(and)X
1368(normal-angle)X
1860(are)X
1989(expressed)X
2357(in)X
2448(radians)X
2724(\(rather)X
2980(than)X
3154(in)X
3245(degrees)X
3535(scaled)X
3777(by)X
3887(64\))X
555 960(in)N
646(the)X
776(range)X
993([0,2*PI\),)X
1327(and)X
1476(where)X
1712(atan)X
1881(returns)X
2147(a)X
2208(value)X
2421(in)X
2512(the)X
2642(range)X
2859([-PI/2,PI/2],)X
3306(and)X
3455(where)X
3691(adjust)X
3924(is:)X
843 1132(0)N
1063(for)X
1187(normal-angle)X
1679(in)X
1770(the)X
1900(range)X
2117([0,PI/2\))X
843 1228(PI)N
1075(for)X
1199(normal-angle)X
1691(in)X
1782(the)X
1912(range)X
2129([PI/2,\(3*PI\)/2\))X
843 1324(2*PI)N
1119(for)X
1243(normal-angle)X
1735(in)X
1826(the)X
1956(range)X
2173([\(3*PI\)/2,2*PI\))X
555 1496(In)N
650(the)X
780(case)X
953(of)X
5 f
1066(XDrawArc)X
1 f
1476(,)X
1520(you)X
1674(simply)X
1937(specify)X
2213(a)X
2274(single)X
2507(arc.)X
2680(For)X
5 f
2842(XDrawArcs)X
1 f
3296(,)X
3340(you)X
3494(specify)X
3770(an)X
555 1592(array)N
757(of)X
5 f
870(XArc)X
1 f
1098(structures,)X
1484(each)X
1667(of)X
1762(which)X
1999(contains)X
2315(an)X
2420(arc's)X
2612(x)X
2678(and)X
2827(y)X
2893(coordinates,)X
3343(width)X
3566(and)X
3715(height,)X
555 1688(start)N
729(position,)X
1058(and)X
1207(path)X
1381(and)X
1530(extent.)X
5 f
1833(XDrawArcs)X
1 f
2309(draws)X
2540(the)X
2670(arcs)X
2833(in)X
2924(the)X
3054(order)X
3261(listed)X
3475(in)X
3566(the)X
3696(array.)X
555 1784(For)N
699(any)X
848(given)X
1066(arc,)X
1217(no)X
1327(pixel)X
1526(is)X
1607(drawn)X
1848(more)X
2051(than)X
2225(once.)X
2435(If)X
2515(two)X
2669(arcs)X
2832(join)X
2992(correctly)X
3327(and)X
3476(if)X
3552(the)X
555 1880(line_width)N
955(is)X
1036(greater)X
1302(than)X
1476(zero)X
1649(and)X
1798(the)X
1928(arcs)X
2091(intersect,)X
2434(no)X
2544(pixel)X
2743(is)X
2824(drawn)X
3065(more)X
3268(than)X
3442(once.)X
3674(Other-)X
555 1976(wise,)N
760(the)X
890(intersecting)X
1324(pixels)X
1557(of)X
1652(intersecting)X
2086(arcs)X
2249(are)X
2378(drawn)X
2619(multiple)X
2937(times.)X
3195(Specifying)X
3599(an)X
3704(arc)X
3833(with)X
555 2072(one)N
704(endpoint)X
1035(and)X
1184(a)X
1245(clockwise)X
1619(extent)X
1857(draws)X
2088(the)X
2218(same)X
2421(pixels)X
2654(as)X
2749(specifying)X
3138(the)X
3268(other)X
3471(endpoint)X
3802(and)X
555 2168(an)N
660(equivalent)X
1050(counterclockwise)X
1688(extent,)X
1948(except)X
2200(as)X
2295(it)X
2367(affects)X
2623(joins.)X
555 2292(If)N
635(the)X
765(last)X
910(point)X
1114(in)X
1205(one)X
1354(arc)X
1483(coincides)X
1838(with)X
2017(the)X
2147(\256rst)X
2306(point)X
2510(in)X
2601(the)X
2731(following)X
3096(arc,)X
3247(the)X
3377(two)X
3531(arcs)X
3694(will)X
3854(join)X
555 2388(correctly.)N
912(If)X
992(the)X
1122(\256rst)X
1281(point)X
1485(in)X
1576(the)X
1706(\256rst)X
1865(arc)X
1994(coincides)X
2349(with)X
2528(the)X
2658(last)X
2803(point)X
3007(in)X
3098(the)X
3228(last)X
3373(arc,)X
3524(the)X
3654(two)X
3808(arcs)X
555 2484(will)N
715(join)X
875(correctly.)X
1254(By)X
1379(specifying)X
1768(one)X
1917(axis)X
2081(to)X
2172(be)X
2277(zero,)X
2472(a)X
2533(horizontal)X
2913(or)X
3008(vertical)X
3295(line)X
3450(can)X
3594(be)X
3699(drawn.)X
555 2580(Angles)N
826(are)X
955(computed)X
1325(based)X
1547(solely)X
1780(on)X
1890(the)X
2020(coordinate)X
2414(system)X
2681(and)X
2830(ignore)X
3077(the)X
3207(aspect)X
3449(ratio.)X
5 f
570 2704(XDrawArc)N
1 f
1002(and)X
5 f
1169(XDrawArcs)X
1 f
1645(use)X
1784(these)X
1987(graphics)X
2307(context)X
2589(components:)X
3062(function,)X
3400(plane_mask,)X
555 2800(line_width,)N
977(line_style,)X
1365(cap_style,)X
1742(join_style,)X
2135(\256ll_style,)X
2489(subwindow_mode,)X
3178(clip_x_origin,)X
555 2896(clip_y_origin,)N
1075(and)X
1224(clip_mask.)X
1653(Both)X
1847(functions)X
2197(also)X
2361(use)X
2500(these)X
2703(graphics)X
3023(context)X
3305(mode-dependent)X
555 2992(components:)N
1028(foreground,)X
1462(background,)X
1921(tile,)X
2079(stipple,)X
2359(ts_x_origin,)X
2805(ts_y_origin,)X
3251(dash_offset,)X
3700(and)X
555 3088(dash_list.)N
555 3212(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawArc)X
1 f
2268(and)X
5 f
2435(XDrawArcs)X
1 f
2911(are)X
5 f
3058(BadDrawable)X
1 f
3600(,)X
5 f
3662(BadGC)X
1 f
3966(,)X
555 3308(and)N
5 f
722(BadMatch)X
1 f
1134(.)X
3 f
555 3500(6.4.)N
731(Filling)X
1000(Areas)X
1 f
555 3624(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(\256ll:)X
10 f
555 3748(g)N
1 f
775(A)X
860(single)X
1093(rectangle)X
1438(or)X
1533(multiple)X
1851(rectangles)X
10 f
555 3872(g)N
1 f
775(A)X
860(single)X
1093(polygon)X
10 f
555 3996(g)N
1 f
775(A)X
860(single)X
1093(arc)X
1222(or)X
1317(multiple)X
1635(arcs)X
3 f
555 4188(6.4.1.)N
797(Filling)X
1066(Single)X
1319(and)X
1483(Multiple)X
1829(Rectangles)X
1 f
555 4312(Use)N
5 f
731(XFillRectangle)X
1 f
1334(to)X
1425(\256ll)X
1546(a)X
1607(single)X
1840(rectangular)X
2258(area)X
2426(or)X
5 f
2539(XFillRectangles)X
1 f
3186(to)X
3277(\256ll)X
3398(multiple)X
3716(rec-)X
555 4408(tangular)N
866(areas)X
1068(in)X
1159(the)X
1289(speci\256ed)X
1624(drawable.)X
555 4628(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XFillRectangle)X
1 f
1820(is:)X
555 4752(XFillRectangle)N
1102(\()X
2 f
1138(display)X
1 f
1393(,)X
2 f
1437(d)X
1 f
1488(,)X
2 f
1532(gc)X
1 f
1622(,)X
2 f
1666(x)X
1 f
1712(,)X
2 f
1756(y)X
1 f
1802(,)X
2 f
1846(width)X
1 f
2050(,)X
2 f
2094(height)X
1 f
2322(\))X
687 4848(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4944(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5040(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5136(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 5232(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
2 f
555 5384(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5508(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 5632(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
3 f
2239 6144(88)N

89 p
%%Page: 89 93
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(x)N
555 768(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(are)X
3094(relative)X
3381(to)X
3472(the)X
3602(origin)X
3835(of)X
1131 864(the)N
1261(drawable)X
1605(and)X
1754(specify)X
2030(the)X
2160(upper)X
2382(left)X
2522(corner)X
2768(of)X
2863(the)X
2993(rectangle.)X
2 f
555 988(width)N
555 1084(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(dimensions)X
3126(of)X
3221(the)X
3351(rectangle)X
3696(to)X
3787(be)X
1131 1180(\256lled.)N
555 1400(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XFillRectangles)X
1 f
1864(is:)X
555 1524(XFillRectangles)N
1136(\()X
2 f
1172(display)X
1 f
1427(,)X
2 f
1471(d)X
1 f
1522(,)X
2 f
1566(gc)X
1 f
1656(,)X
2 f
1700(rectangles)X
1 f
2074(,)X
2 f
2118(nrectangles)X
1 f
2536(\))X
687 1620(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1716(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 1812(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1908(XRectangle)N
1125(*)X
2 f
(rectangles)S
1 f
1543(;)X
687 2004(int)N
2 f
803(nrectangles)X
1 f
1221(;)X
2 f
555 2156(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2280(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2404(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2528(rectangles)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(rectangles.)X
2 f
555 2652(nrectangles)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(rectangles)X
2366(in)X
2457(the)X
2587(array.)X
555 2776(The)N
5 f
732(XFillRectangle)X
1 f
1335(and)X
5 f
1502(XFillRectangles)X
1 f
2149(functions)X
2499(\256ll)X
2620(the)X
2750(speci\256ed)X
3085(rectangle)X
3430(or)X
3525(rectangles)X
3904(as)X
555 2872(if)N
631(a)X
692(four-point)X
5 f
1089(XFillPolygon)X
1 f
1614(were)X
1806(speci\256ed)X
2141(for)X
2265(each)X
2448(rectangle:)X
755 2996([x,y])N
945([x+width,y])X
1386([x+width,y+height])X
2098([x,y+height])X
555 3120(Each)N
753(function)X
1069(uses)X
1242(the)X
1372(x)X
1438(and)X
1587(y)X
1653(coordinates,)X
2103(width)X
2326(and)X
2475(height)X
2718(dimensions,)X
3164(and)X
3313(graphics)X
3633(context)X
555 3216(you)N
709(specify.)X
1029(Both)X
1223(functions)X
1573(use)X
1712(these)X
1915(graphics)X
2235(context)X
2517(components:)X
2990(function,)X
3328(plane_mask,)X
555 3312(\256ll_style,)N
909(\256ll_rule,)X
1233(subwindow_mode,)X
1922(clip_x_origin,)X
2442(clip_y_origin,)X
2962(and)X
3111(clip_mask.)X
3540(The)X
3699(two)X
555 3408(functions)N
905(also)X
1069(use)X
1208(these)X
1411(graphics)X
1731(context)X
2013(mode_dependent)X
2637(components:)X
3110(foreground,)X
3544(background,)X
555 3504(tile,)N
713(stipple,)X
993(ts_x_origin,)X
1439(and)X
1588(ts_y_origin.)X
555 3628(In)N
650(the)X
780(case)X
953(of)X
5 f
1066(XFillRectangle)X
1 f
1647(,)X
1691(you)X
1845(simply)X
2108(specify)X
2384(a)X
2445(single)X
2678(rectangle)X
3023(to)X
3114(be)X
3219(\256lled.)X
3467(For)X
5 f
3629(XFillRec-)X
555 3724(tangles)N
1 f
854(,)X
898(you)X
1052(specify)X
1328(an)X
1433(array)X
1635(of)X
5 f
1748(XRectangle)X
1 f
2240(structures,)X
2626(each)X
2809(of)X
2904(which)X
3141(contains)X
3457(a)X
3518(rectangle's)X
3926(x)X
555 3820(and)N
704(y)X
770(coordinates)X
1198(and)X
1347(width)X
1570(and)X
1719(height.)X
5 f
2002(XFillRectangles)X
1 f
2649(\256lls)X
2804(the)X
2934(rectangles)X
3313(in)X
3404(the)X
3534(order)X
3741(listed)X
555 3916(in)N
646(the)X
776(array.)X
1000(The)X
1159(x)X
1225(and)X
1374(y)X
1440(coordinates)X
1868(of)X
1963(each)X
2146(rectangle)X
2491(are)X
2620(relative)X
2907(to)X
2998(the)X
3128(drawable's)X
3535(origin)X
3768(and)X
555 4012(de\256ne)N
792(the)X
922(upper)X
1144(left)X
1284(corner)X
1530(of)X
1625(the)X
1755(rectangle.)X
2144(For)X
2288(any)X
2437(given)X
2655(rectangle,)X
3022(no)X
3132(pixel)X
3331(is)X
3412(drawn)X
3653(more)X
3856(than)X
555 4108(once.)N
765(If)X
845(rectangles)X
1224(intersect,)X
1567(the)X
1697(intersecting)X
2131(pixels)X
2364(are)X
2493(drawn)X
2734(multiple)X
3052(times.)X
555 4232(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XFillRectangle)X
1 f
2439(and)X
5 f
2606(XFillRectangles)X
1 f
3253(are)X
5 f
3400(BadDrawable)X
1 f
3942(,)X
5 f
570 4328(BadGC)N
1 f
874(,)X
918(and)X
5 f
1085(BadMatch)X
1 f
1497(.)X
3 f
555 4520(6.4.2.)N
797(Filling)X
1066(a)X
1132(Single)X
1385(Polygon)X
1 f
555 4740(Use)N
5 f
731(XFillPolygon)X
1 f
1256(to)X
1347(\256ll)X
1468(a)X
1529(polygon)X
1840(area)X
2008(in)X
2099(the)X
2229(speci\256ed)X
2564(drawable.)X
2952(The)X
3111(de\256nition)X
3472(for)X
3596(this)X
3746(func-)X
555 4836(tion)N
715(is:)X
555 4960(XFillPolygon)N
1043(\()X
2 f
1079(display)X
1 f
1334(,)X
2 f
1378(d)X
1 f
1429(,)X
2 f
1473(gc)X
1 f
1563(,)X
2 f
1607(points)X
1 f
1830(,)X
2 f
1874(npoints)X
1 f
2141(,)X
2 f
2185(shape)X
1 f
2397(,)X
2 f
2441(mode)X
1 f
2638(\))X
687 5056(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5152(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5248(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5344(XPoint)N
959(*)X
2 f
(points)S
1 f
1226(;)X
687 5440(int)N
2 f
803(npoints)X
1 f
1070(;)X
687 5536(int)N
2 f
803(shape)X
1 f
1015(;)X
687 5632(int)N
2 f
803(mode)X
1 f
1000(;)X
3 f
2239 6144(89)N

90 p
%%Page: 90 94
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 796(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 920(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1044(points)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(points.)X
2 f
555 1168(npoints)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(points)X
2225(in)X
2316(the)X
2446(array.)X
2 f
555 1292(shape)N
1 f
1131(Speci\256es)X
1471(an)X
1576(argument)X
1931(that)X
2086(helps)X
2294(the)X
2424(server)X
2660(to)X
2751(improve)X
3067(performance.)X
3577(You)X
3750(can)X
1131 1388(pass)N
1304(one)X
1453(of)X
1548(these)X
1751(constants:)X
5 f
2144(Complex)X
1 f
2505(,)X
5 f
2567(Convex)X
1 f
2880(,)X
2924(or)X
5 f
3037(Nonconvex)X
1 f
3492(.)X
2 f
555 1512(mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(coordinate)X
1995(mode.)X
5 f
2253(CoordModeOrigin)X
1 f
2983(treats)X
3196(a)X
3257(coordinates)X
3685(as)X
1131 1608(related)N
1393(to)X
1484(the)X
1614(origin,)X
1869(while)X
5 f
2105(CoordModePrevious)X
1 f
2943(treates)X
3195(all)X
3306(coordinates)X
3734(after)X
1131 1704(the)N
1261(\256rst)X
1420(as)X
1515(relative)X
1802(to)X
1893(the)X
2023(previous)X
2348(point.)X
555 1828(The)N
5 f
732(XFillPolygon)X
1 f
1257(function)X
1573(uses)X
1746(these)X
1949(graphics)X
2269(context)X
2551(components)X
2999(when)X
3211(\256lling)X
3445(the)X
3575(polygon)X
555 1924(area:)N
748(function,)X
1086(plane_mask,)X
1551(\256ll_style,)X
1905(\256ll_rule,)X
2229(subwindow_mode,)X
2918(clip_x_origin,)X
3438(clip_y_origin,)X
555 2020(and)N
704(clip_mask.)X
1133(This)X
1312(function)X
1628(also)X
1792(uses)X
1965(these)X
2168(graphics)X
2488(context)X
2770(mode_dependent)X
3394(components:)X
555 2116(foreground,)N
989(tile,)X
1147(stipple,)X
1427(ts_x_origin,)X
1873(and)X
2022(ts_y_origin.)X
5 f
2511(XFillPolygon)X
1 f
3036(\256lls)X
3191(the)X
3321(region)X
3568(closed)X
3815(by)X
555 2212(the)N
685(speci\256ed)X
1020(path.)X
1238(The)X
1397(path)X
1571(is)X
1652(closed)X
1899 0.2604(automatically)AX
2403(if)X
2479(the)X
2609(last)X
2754(point)X
2958(in)X
3049(the)X
3179(list)X
3310(does)X
3493(not)X
3628(coincide)X
555 2308(with)N
734(the)X
864(\256rst)X
1023(point.)X
1271(No)X
1400(pixel)X
1599(of)X
1694(the)X
1824(region)X
2071(is)X
2152(drawn)X
2393(more)X
2596(than)X
2770(once.)X
5 f
570 2432(XFillPolygon)N
1 f
1095(requires)X
1400(a)X
1461(mode)X
1679(argument)X
2034(to)X
2125(determine)X
2500(whether)X
2805(the)X
2935(points)X
3173(are)X
3302(relative)X
3589(to)X
3680(the)X
555 2528(drawable's)N
962(origin)X
1195(or)X
1290(to)X
1381(the)X
1511(previous)X
1836(point:)X
10 f
555 2652(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModeOrigin)X
1 f
1880(,)X
1924(all)X
2035(points)X
2273(after)X
2456(the)X
2586(\256rst)X
2745(are)X
2874(relative)X
3161(to)X
3252(the)X
3382(drawable's)X
3789(ori-)X
775 2748(gin.)N
954(\(The)X
1142(\256rst)X
1301(point)X
1505(is)X
1586(always)X
1852(relative)X
2139(to)X
2230(the)X
2360(drawable's)X
2767(origin.\))X
10 f
555 2872(g)N
1 f
775(If)X
855(mode)X
1073(is)X
5 f
1172(CoordModePrevious)X
1 f
1988(,)X
2032(all)X
2143(points)X
2381(after)X
2564(the)X
2694(\256rst)X
2853(are)X
2982(relative)X
3269(to)X
3360(the)X
3490(previous)X
775 2968(point.)N
5 f
570 3092(XFillPolygon)N
1 f
1095(also)X
1259(requires)X
1564(a)X
1625(shape)X
1847(argument:)X
10 f
555 3216(g)N
1 f
775(If)X
855(shape)X
1077(is)X
5 f
1176(Complex)X
1 f
1537(,)X
1581(the)X
1711(path)X
1885(may)X
2059(self-intersect.)X
10 f
555 3340(g)N
1 f
775(If)X
855(shape)X
1077(is)X
5 f
1176(Nonconvex)X
1 f
1631(,)X
1675(the)X
1805(path)X
1979(does)X
2162(not)X
2297(self-intersect,)X
2796(but)X
2931(the)X
3061(shape)X
3283(is)X
3364(not)X
3499(wholly)X
3766(con-)X
775 3436(vex.)N
946(If)X
1026(known)X
1287(by)X
1397(the)X
1527(client,)X
1768(specifying)X
5 f
2175(Nonconvex)X
1 f
2652(instead)X
2924(of)X
5 f
3037(Complex)X
1 f
3420(may)X
3594(improve)X
775 3532(performance.)N
1263(If)X
1343(you)X
1497(specify)X
5 f
1791(Nonconvex)X
1 f
2268(for)X
2392(a)X
2453(self-intersecting)X
3043(path,)X
3239(the)X
3369(graphics)X
3689(results)X
775 3628(are)N
904(unde\256ned.)X
10 f
555 3752(g)N
1 f
775(If)X
855(shape)X
1077(is)X
5 f
1176(Convex)X
1 f
1489(,)X
1533(the)X
1663(path)X
1837(is)X
1918(wholly)X
2185(convex.)X
2483(If)X
2563(known)X
2824(by)X
2934(the)X
3064(client,)X
3305(specifying)X
5 f
3712(Con-)X
775 3848(vex)N
1 f
949(can)X
1093(improve)X
1409(performance.)X
1897(If)X
1977(you)X
2131(specify)X
5 f
2425(Convex)X
1 f
2760(for)X
2884(a)X
2945(path)X
3119(that)X
3274(is)X
3355(not)X
3490(convex,)X
3788(the)X
775 3944(graphics)N
1095(results)X
1347(are)X
1476(unde\256ned.)X
555 4068(The)N
714(\256ll_rule)X
1016(member)X
1327(of)X
1422(the)X
1552(GC)X
1696(controls)X
2002(the)X
2132(\256lling)X
2366(behavior)X
2696(of)X
2791(self-intersecting)X
3381(polygons.)X
555 4192(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XFillPolygon)X
1 f
2361(are)X
5 f
2508(BadDrawable)X
1 f
3050(,)X
5 f
3112(BadGC)X
1 f
3416(,)X
5 f
3478(BadMatch)X
1 f
3890(,)X
555 4288(and)N
5 f
722(BadValue)X
1 f
1119(.)X
3 f
555 4480(6.4.3.)N
797(Filling)X
1066(Single)X
1319(and)X
1483(Multiple)X
1829(Arcs)X
1 f
555 4604(Use)N
5 f
731(XFillArc)X
1 f
1070(to)X
1161(\256ll)X
1282(a)X
1343(single)X
1576(arc)X
1705(or)X
5 f
1818(XFillArcs)X
1 f
2201(to)X
2292(\256ll)X
2413(multiple)X
2731(arcs)X
2894(in)X
2985(the)X
3115(speci\256ed)X
3450(drawable.)X
3838(The)X
555 4700(de\256nition)N
916(for)X
5 f
1058(XFillArc)X
1 f
1397(is:)X
555 4824(XFillArc)N
880(\()X
2 f
916(display)X
1 f
1171(,)X
2 f
1215(d)X
1 f
1266(,)X
2 f
1310(gc)X
1 f
1400(,)X
2 f
1466(x)X
1 f
1512(,)X
2 f
1556(y)X
1 f
1602(,)X
2 f
1646(width)X
1 f
1850(,)X
2 f
1894(height)X
1 f
2122(,)X
2 f
2166(angle1)X
1 f
2413(,)X
2 f
2457(angle2)X
1 f
2704(\))X
687 4920(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5016(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5112(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5208(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 5304(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 5400(int)N
2 f
803(angle1)X
1 f
1050(,)X
2 f
1094(angle2)X
1 f
1341(;)X
2 f
555 5552(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5676(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
3 f
2239 6144(90)N

91 p
%%Page: 91 95
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 796(x)N
555 892(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(are)X
3094(relative)X
3381(to)X
3472(the)X
3602(origin)X
3835(of)X
1131 988(the)N
1261(drawable)X
1605(and)X
1754(specify)X
2030(the)X
2160(upper)X
2382(left)X
2522(corner)X
2768(of)X
2863(the)X
2993(rectangle.)X
2 f
555 1112(width)N
555 1208(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(the)X
2702(major)X
2930(and)X
3079(minor)X
3312(axes)X
3490(of)X
3585(the)X
3715(arc.)X
2 f
555 1332(angle1)N
1 f
1131(Speci\256es)X
1471(the)X
1601(start)X
1775(of)X
1870(the)X
2000(arc)X
2129(relative)X
2416(to)X
2507(the)X
2637(three-o-clock)X
3128(position)X
3435(from)X
3628(the)X
3758(center,)X
1131 1428(in)N
1222(units)X
1416(of)X
1511(degrees)X
1801(*)X
1867(64.)X
2 f
555 1552(angle2)N
1 f
1131(Speci\256es)X
1471(the)X
1601(path)X
1775(and)X
1924(extent)X
2162(of)X
2257(the)X
2387(arc)X
2516(relative)X
2803(to)X
2894(the)X
3024(start)X
3198(of)X
3293(the)X
3423(arc,)X
3574(in)X
3665(units)X
3859(of)X
1131 1648(degrees)N
1421(*)X
1487(64.)X
555 1868(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XFillArcs)X
1 f
1600(is:)X
555 1992(XFillArcs)N
914(\()X
2 f
950(display)X
1 f
1205(,)X
2 f
1249(d)X
1 f
1300(,)X
2 f
1344(gc)X
1 f
1434(,)X
2 f
1478(arcs)X
1 f
1636(,)X
2 f
1680(narcs)X
1 f
1882(\))X
687 2088(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2184(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 2280(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2376(XArc)N
903(*)X
2 f
(arcs)S
1 f
1105(;)X
687 2472(int)N
2 f
803(narcs)X
1 f
1005(;)X
2 f
555 2624(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2748(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2872(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2996(arcs)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(arcs.)X
2 f
555 3120(narcs)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(arcs)X
2150(in)X
2241(the)X
2371(array.)X
555 3244(For)N
699(each)X
882(arc,)X
5 f
1051(XFillArc)X
1 f
1390(or)X
5 f
1503(XFillArcs)X
1 f
1886(\256lls)X
2041(the)X
2171(region)X
2418(closed)X
2665(by)X
2775(the)X
2905(in\256nitely)X
3247(thin)X
3407(path)X
3581(described)X
555 3340(by)N
665(the)X
795(speci\256ed)X
1130(arc)X
1259(and,)X
1430(depending)X
1819(on)X
1929(the)X
2059(arc_mode)X
2428(speci\256ed)X
2763(in)X
2854(the)X
2984(graphics)X
3304(context)X
3586(one)X
3735(or)X
3830(two)X
555 3436(line)N
710(segments.)X
1082(For)X
5 f
1244(ArcChord)X
1 f
1630(,)X
1674(the)X
1804(single)X
2037(line)X
2192(segment)X
2508(joining)X
2781(the)X
2911(endpoints)X
3276(of)X
3371(the)X
3501(arc)X
3630(is)X
3711(used.)X
555 3532(For)N
5 f
717(ArcPieSlice)X
1 f
1181(,)X
1225(the)X
1355(two)X
1509(line)X
1664(segments)X
2014(joining)X
2287(the)X
2417(endpoints)X
2782(of)X
2877(the)X
3007(arc)X
3136(with)X
3315(the)X
3445(center)X
3682(point)X
3886(are)X
555 3628(used.)N
5 f
570 3752(XFillArc)N
1 f
909(and)X
5 f
1076(XFillArcs)X
1 f
1459(use)X
1598(these)X
1801(graphics)X
2121(context)X
2403(components)X
2851(when)X
3063(\256lling)X
3297(the)X
3427(arc)X
3556(or)X
3651(arcs:)X
555 3848(function,)N
893(plane_mask,)X
1358(\256ll_style,)X
1712(arc_mode,)X
2103(subwindow_mode,)X
2792(clip_x_origin,)X
3312(clip_y_origin,)X
3832(and)X
555 3944(clip_mask.)N
984(The)X
1143(two)X
1297(functions)X
1647(also)X
1811(use)X
1950(these)X
2153(graphics)X
2473(context)X
2755(mode-dependent)X
3364(components:)X
3837(fore-)X
555 4040(ground,)N
848(background,)X
1307(tile,)X
1465(stipple,)X
1745(ts_x_origin,)X
2191(and)X
2340(ts_y_origin.)X
555 4164(In)N
650(the)X
780(case)X
953(of)X
5 f
1066(XFillArc)X
1 f
1383(,)X
1427(you)X
1581(simply)X
1844(specify)X
2120(a)X
2181(single)X
2414(arc)X
2543(to)X
2634(be)X
2739(\256lled.)X
2987(For)X
5 f
3149(XFillArcs)X
1 f
3510(,)X
3554(you)X
3708(specify)X
555 4260(an)N
660(array)X
862(of)X
5 f
975(XArc)X
1 f
1203(structures,)X
1589(each)X
1772(of)X
1867(which)X
2104(contains)X
2420(an)X
2525(arc's)X
2717(x)X
2783(and)X
2932(y)X
2998(coordinates,)X
3448(width)X
3671(and)X
555 4356(height,)N
820(start)X
994(position,)X
1323(and)X
1472(path)X
1646(and)X
1795(extent.)X
5 f
2098(XFillArcs)X
1 f
2481(\256lls)X
2636(the)X
2766(arcs)X
2929(in)X
3020(the)X
3150(order)X
3357(listed)X
3571(in)X
3662(the)X
3792(array.)X
555 4452(For)N
699(any)X
848(given)X
1066(arc,)X
1217(no)X
1327(pixel)X
1526(is)X
1607(drawn)X
1848(more)X
2051(than)X
2225(once.)X
2435(If)X
2515(regions)X
2796(intersect,)X
3139(the)X
3269(intersecting)X
3703(pixels)X
555 4548(are)N
684(drawn)X
925(multiple)X
1243(times.)X
555 4672(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XFillArc)X
1 f
2175(and)X
5 f
2342(XFillArcs)X
1 f
2725(are)X
5 f
2872(BadDrawable)X
1 f
3414(,)X
5 f
3476(BadGC)X
1 f
3780(,)X
3824(and)X
5 f
570 4768(BadMatch)N
1 f
982(.)X
3 f
555 4960(6.5.)N
731(Manipulating)X
1268(Fonts)X
1 f
555 5084(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(manipulate)X
2538(fonts.)X
2780(The)X
2939(following)X
3304(sections)X
3610(discuss)X
555 5180(how)N
728(to:)X
10 f
555 5304(g)N
1 f
775(Load)X
978(and)X
1127(free)X
1285(fonts)X
10 f
555 5428(g)N
1 f
775(Obtain)X
1037(and)X
1186(free)X
1344(font)X
1508(names)X
10 f
555 5552(g)N
1 f
775(Set)X
910(and)X
1059(return)X
1291(the)X
1421(font)X
1585(search)X
1831(path)X
10 f
555 5676(g)N
1 f
775(Compute)X
1121(character)X
1465(string)X
1688(sizes)X
3 f
2239 6144(91)N

92 p
%%Page: 92 96
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(Return)X
1037(logical)X
1300(extents)X
10 f
555 796(g)N
1 f
775(Query)X
1016(character)X
1360(string)X
1583(sizes)X
555 920(The)N
714(X)X
799(server)X
1035(loads)X
1243(fonts)X
1441(whenever)X
1804(a)X
1865(program)X
2185(requests)X
2495(a)X
2556(new)X
2724(font.)X
2932(Fonts)X
3150(are)X
3279(unloaded)X
3624(when)X
3836(the)X
555 1016(last)N
700(font)X
864(is)X
945(no)X
1055(longer)X
1302(referenced.)X
1738(There)X
1965(is)X
2046(never)X
2263(more)X
2466(than)X
2640(one)X
2789(copy)X
2982(of)X
3077(a)X
3138(font)X
3302(stored)X
3539(in)X
3630(the)X
3760(server)X
555 1112(at)N
641(one)X
790(time.)X
1014(Fonts)X
1232(are)X
1361(global)X
1604(across)X
1845(all)X
1956(screens)X
2236(in)X
2327(a)X
2388(server.)X
2668(There)X
2895(are)X
3024(several)X
3295(levels)X
3523(one)X
3672(can)X
3816(deal)X
555 1208(with)N
734(fonts.)X
976(Most)X
1179(applications)X
1628(will)X
1788(simply)X
2051(use)X
5 f
2208(XLoadQueryFont)X
1 f
2917(to)X
3008(load)X
3182(a)X
3243(font)X
3407(and)X
3556(query)X
3778(the)X
555 1304(font)N
719(metrics.)X
555 1428(Characters)N
953(in)X
1044(fonts)X
1242(are)X
1371(regarded)X
1700(as)X
1795(masks.)X
2081(Except)X
2348(for)X
2472(image)X
2710(text)X
2865(requests,)X
3197(the)X
3327(only)X
3506(pixels)X
555 1524(modi\256ed)N
891(are)X
1020(those)X
1228(in)X
1319(which)X
1556(bits)X
1706(are)X
1835(on)X
1945(in)X
2036(the)X
2166(character.)X
2554(This)X
2733(means)X
2980(that)X
3135(it)X
3207(makes)X
3454(sense)X
3666(to)X
3757(draw)X
555 1620(text)N
710(using)X
923(stipples)X
1215(or)X
1310(tiles)X
1480(\(for)X
1633(example,)X
1976(many)X
2194(menus)X
2446(gray-out)X
2766(unusable)X
3101(entries\).)X
3 f
555 1812(6.5.1.)N
797(Loading)X
1133(and)X
1297(Freeing)X
1608(Fonts)X
1 f
555 1936(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(load)X
2297(fonts,)X
2517(get)X
2647(font)X
2811(information,)X
3272(unload)X
3534(fonts,)X
3754(and)X
555 2032(free)N
713(font)X
877(information.)X
1360(A)X
1445(few)X
1598(font)X
1762(functions)X
2112(use)X
2251(a)X
2312(GContext)X
2677(in)X
2768(order)X
2975(to)X
3066(specify)X
3342(the)X
3472(Graphics)X
3811(con-)X
555 2128(text)N
710(for)X
834(a)X
895(font)X
1059(query.)X
555 2348(Use)N
5 f
731(XLoadFont)X
1 f
1200(to)X
1291(load)X
1465(the)X
1595(speci\256ed)X
1930(font.)X
2138(The)X
2297(de\256nition)X
2658(for)X
2782(this)X
2932(function)X
3248(is:)X
555 2472(Font)N
739(XLoadFont)X
1152(\()X
2 f
1188(display)X
1 f
1443(,)X
2 f
1487(name)X
1 f
1684(\))X
687 2568(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2664(char)N
860(*)X
2 f
(name)S
1 f
1101(;)X
2 f
555 2816(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2940(name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(of)X
1909(the)X
2039(font.)X
2247(This)X
2426(name)X
2639(is)X
2720(a)X
2781(null)X
2941(terminated)X
3341(string.)X
3608(You)X
1131 3036(should)N
1388(use)X
1527(the)X
1657(ISO)X
1820(Latin-1)X
2102(encoding,)X
2469(and)X
2618(upper/lower)X
3065(case)X
3238(does)X
3421(not)X
3556(matter.)X
555 3160(The)N
5 f
732(XLoadFont)X
1 f
1201(function)X
1517(loads)X
1725(the)X
1855(speci\256ed)X
2190(font)X
2354(and)X
2503(returns)X
2769(its)X
2875(associated)X
3259(font)X
3423(ID.)X
3581(If)X
3661(the)X
3791(func-)X
555 3256(tion)N
715(was)X
873(unsuccessful)X
1344(at)X
1430(loading)X
1717(the)X
1847(speci\256ed)X
2182(font,)X
2368(it)X
2440(generates)X
2794(a)X
5 f
2873(BadName)X
1 f
3301(error.)X
3537(When)X
3769(the)X
555 3352(font)N
719(is)X
800(no)X
910(longer)X
1157(needed,)X
1450(the)X
1580(client)X
1799(should)X
2056(call)X
5 f
2224(XUnloadFont)X
1 f
2753(.)X
2819(Fonts)X
3037(are)X
3166(not)X
3301(associated)X
3685(with)X
3864(a)X
555 3448(particular)N
915(screen)X
1161(and)X
1310(can)X
1454(be)X
1559(stored)X
1796(as)X
1891(a)X
1952(component)X
2366(of)X
2461(any)X
2610(graphics)X
2930(context.)X
555 3572(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XLoadFont)X
1 f
2305(are)X
5 f
2452(BadAlloc)X
1 f
2836(and)X
5 f
3003(BadName)X
1 f
3409(.)X
555 3820(Use)N
5 f
731(XGContextFromGC)X
1 f
1537(to)X
1628(obtain)X
1871(the)X
2001(GContext)X
2366(for)X
2490(the)X
2620(speci\256ed)X
2955(GC.)X
3143(The)X
3302(de\256nition)X
3663(for)X
3787(this)X
555 3916(function)N
871(is:)X
555 4040(GContext)N
920(XGContextFromGC)X
1646(\()X
2 f
1682(gc)X
1 f
1772(\))X
687 4136(GC)N
2 f
831(gc)X
1 f
921(;)X
2 f
555 4288(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context)X
2203(that)X
2358(you)X
2512(want)X
2705(the)X
2835(resource)X
3154(for.)X
555 4508(Use)N
5 f
731(XQueryFont)X
1 f
1244(to)X
1335(return)X
1567(information)X
2006(about)X
2224(a)X
2285(loaded)X
2542(font.)X
2750(The)X
2909(de\256nition)X
3270(for)X
3394(this)X
3544(function)X
3860(is:)X
555 4632(XFontStruct)N
1013(*XQueryFont)X
1508(\()X
2 f
1544(display)X
1 f
1799(,)X
2 f
1843(font_ID)X
1 f
2124(\))X
687 4728(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4824(XID)N
2 f
864(font_ID)X
1 f
1145(;)X
2 f
555 4976(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5100(font_ID)N
1 f
1131(Speci\256es)X
1471(the)X
1601(ID)X
1715(of)X
1810(the)X
1940(font)X
2104(or)X
2199(the)X
2329(graphics)X
2649(context)X
2931(whose)X
3177(current)X
3448(font)X
3612(you)X
3766(want)X
1131 5196(font)N
1295(information)X
1734(about.)X
555 5320(The)N
5 f
732(XQueryFont)X
1 f
1245(function)X
1561(returns)X
1827(a)X
1888(pointer)X
2160(to)X
2251(the)X
5 f
2399(XFontStruct)X
1 f
2903(structure,)X
3255(which)X
3492(contains)X
3808(infor-)X
555 5416(mation)N
823(associated)X
1207(with)X
1386(the)X
1516(font.)X
1724(You)X
1897(can)X
2041(either)X
2264(query)X
2486(fonts)X
2684(or)X
2779(the)X
2909(fonts)X
3107(stored)X
3344(in)X
3435(the)X
3565(GC.)X
3753(Use)X
5 f
570 5512(XFreeFontInfo)N
1 f
1172(to)X
1263(free)X
1421(this)X
1571(data.)X
1784(In)X
1879(this)X
2029(case,)X
2224(the)X
2354(font)X
2518(ID)X
2632(stored)X
2869(in)X
2960(the)X
5 f
3108(XFontstruct)X
1 f
3597(will)X
3757(be)X
3862(the)X
555 5608(ID)N
669(of)X
764(the)X
894(GC,)X
1060(and)X
1209(you)X
1363(need)X
1551(to)X
1642(be)X
1747(careful)X
2013(when)X
2225(using)X
2438(this)X
2588(ID)X
2702(in)X
2793(other)X
2996(functions.)X
3390(For)X
3534(example,)X
3877(the)X
555 5704(ID)N
669(of)X
764(the)X
894(GC)X
1038(is)X
1119(not)X
1254(valid)X
1453(as)X
1548(a)X
1609(font)X
1773(ID)X
1887(in)X
1978(a)X
2039(call)X
2189(to)X
2280(a)X
2341(Set)X
2476(or)X
2571(Get)X
2720(font)X
2884(function.)X
3 f
2239 6144(92)N

93 p
%%Page: 93 97
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Use)N
5 f
731(XListFontsWithInfo)X
1 f
1509(to)X
1600(obtain)X
1843(the)X
1973(names)X
2220(and)X
2369(information)X
2808(about)X
3026(loaded)X
3283(fonts.)X
3525(The)X
555 768(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 892(char)N
728(**XListFontsWithInfo)X
1543(\()X
2 f
1579(display)X
1 f
1834(,)X
2 f
1878(pattern)X
1 f
2133(,)X
2 f
2177(maxnames)X
1 f
2547(,)X
2 f
2591(count_return)X
1 f
3051(,)X
2 f
3095(info_return)X
1 f
3504(\))X
687 988(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1084(char)N
860(*)X
2 f
(pattern)S
1 f
1166(;)X
687 1180(int)N
2 f
803(maxnames)X
1 f
1180(;)X
687 1276(int)N
803(*)X
2 f
(count_return)S
1 f
1314(;)X
687 1372(XFontStruct)N
1145(**)X
2 f
(info_return)S
1 f
1642(;)X
2 f
555 1524(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1648(pattern)N
1 f
1131(Speci\256es)X
1471(the)X
1601(null-terminated)X
2168(string)X
2391(associated)X
2775(with)X
2954(the)X
3084(font)X
3248(names)X
3495(that)X
3650(you)X
3804(want)X
1131 1744(returned.)N
1490(You)X
1663(can)X
1807(specify)X
2083(any)X
2232(string,)X
2477(an)X
2582(asterisk)X
2873(\(*\),)X
3019(or)X
3114(a)X
3175(question)X
3496(mark)X
3699(\(?\).)X
3840(The)X
1131 1840(asterisk)N
1422(indicates)X
1758(a)X
1819(wildcard)X
2149(on)X
2259(any)X
2408(number)X
2699(of)X
2794(characters,)X
3194(and)X
3343(the)X
3473(question)X
3794(mark)X
1131 1936(indicates)N
1467(a)X
1528(wildcard)X
1858(on)X
1968(a)X
2029(single)X
2262(character.)X
2 f
555 2060(maxnames)N
1 f
1131(Speci\256es)X
1471(the)X
1601(maximum)X
1982(number)X
2273(of)X
2368(names)X
2615(that)X
2770(are)X
2899(to)X
2990(be)X
3095(in)X
3186(the)X
3316(returned)X
3631(list.)X
2 f
555 2184(count_return)N
1 f
1131(Returns)X
1427(the)X
1557(actual)X
1790(number)X
2081(of)X
2176(matched)X
2497(font)X
2661(names.)X
2 f
555 2308(info_return)N
1 f
1131(Returns)X
1427(a)X
1488(pointer)X
1760(to)X
1851(the)X
1981(font)X
2145(information.)X
555 2432(The)N
5 f
732(XListFontsWithInfo)X
1 f
1510(function)X
1826(returns)X
2092(a)X
2153(list)X
2284(of)X
2379(names)X
2626(of)X
2721(fonts)X
2919(that)X
3074(match)X
3312(the)X
3442(speci\256ed)X
3777(pat-)X
555 2528(tern)N
714(and)X
863(their)X
1047(associated)X
1431(font)X
1595(information.)X
2078(The)X
2237(list)X
2368(of)X
2463(names)X
2710(is)X
2791(limited)X
3065(to)X
3156(size)X
3315(speci\256ed)X
3650(by)X
3760(the)X
555 2624(maxnames)N
954(argument.)X
1353(The)X
1512(information)X
1951(returned)X
2266(for)X
2390(each)X
2573(font)X
2737(is)X
2818(identical)X
3145(to)X
3236(what)X
5 f
3447(XLoad-)X
555 2720(QueryFont)N
1 f
1009(would)X
1251(return,)X
1505(except)X
1757(that)X
1912(the)X
2042(per-character)X
2527(metrics)X
2809(are)X
2938(not)X
3073(returned.)X
3432(To)X
3552(free)X
3710(the)X
3840(allo-)X
555 2816(cated)N
763(name)X
976(array,)X
1200(the)X
1330(client)X
1549(should)X
1806(call)X
5 f
1974(XFreeFontNames)X
1 f
2684(.)X
2750(To)X
2870(free)X
3028(the)X
3158(the)X
3288(font)X
3452(information)X
555 2912(array,)N
779(the)X
909(client)X
1128(should)X
1385(call)X
5 f
1553(XFreeFontInfo)X
1 f
2133(.)X
555 3132(Use)N
5 f
731(XFreeFontInfo)X
1 f
1333(to)X
1424(free)X
1582(the)X
1712(the)X
1842(font)X
2006(information)X
2445(array.)X
2669(The)X
2828(de\256nition)X
3189(for)X
3313(this)X
3463(function)X
3779(is:)X
555 3256(XFreeFontInfo\()N
2 f
1118(names)X
1 f
1342(,)X
2 f
1386(free_info)X
1 f
1705(,)X
2 f
1749(actual_count)X
1 f
2217(\))X
687 3352(char)N
860(**)X
2 f
(names)S
1 f
1179(;)X
687 3448(XFontStruct)N
1145(*)X
2 f
(free_info)S
1 f
1508(;)X
687 3544(int)N
2 f
803(actual_count)X
1 f
1271(;)X
2 f
555 3696(names)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(list)X
2156(of)X
2251(font)X
2415(names)X
2662(that)X
2817(were)X
3009(returned)X
3324(by)X
5 f
3452(XList-)X
1131 3792(FontsWithInfo)N
1 f
1691(.)X
2 f
555 3916(free_info)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(font)X
2189(information)X
2628(that)X
2783(was)X
2941(returned)X
3256(by)X
5 f
3384(Xlist-)X
1131 4012(FontWithInfo)N
1 f
1647(.)X
2 f
555 4136(actual_count)N
1 f
1131(Speci\256es)X
1471(the)X
1601(actual)X
1834(number)X
2125(of)X
2220(matched)X
2541(font)X
2705(names)X
2952(that)X
3107(were)X
3299(returned)X
3614(by)X
5 f
3742(Xlist-)X
1131 4232(FontsWithInfo)N
1 f
1691(.)X
555 4452(Use)N
5 f
731(XLoadQueryFont)X
1 f
1440(to)X
1531(perform)X
1836(a)X
5 f
1915(XLoadFont)X
1 f
2384(and)X
5 f
2551(XQueryFont)X
1 f
3064(in)X
3155(a)X
3216(single)X
3449(operation.)X
3848(The)X
555 4548(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 4672(XFontStruct)N
1013(*XLoadQueryFont)X
1689(\()X
2 f
1725(display)X
1 f
1980(,)X
2 f
2024(name)X
1 f
2221(\))X
687 4768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4864(char)N
860(*)X
2 f
(name)S
1 f
1101(;)X
2 f
555 5016(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5140(name)N
1 f
1131(Speci\256es)X
1471(the)X
1601(name)X
1814(of)X
1909(the)X
2039(font.)X
2247(This)X
2426(name)X
2639(is)X
2720(a)X
2781(null)X
2941(terminated)X
3341(string.)X
555 5264(The)N
5 f
732(XLoadQueryFont)X
1 f
1441(function)X
1757(provides)X
2082(the)X
2212(most)X
2406(common)X
2737(way)X
2905(for)X
3029(accessing)X
3388(a)X
3449(font.)X
3657(That)X
3841(is,)X
5 f
570 5360(XLoadQueryFont)N
1 f
1279(both)X
1458(opens)X
1685(\(loads\))X
1951(the)X
2081(speci\256ed)X
2416(font)X
2580(and)X
2729(returns)X
2995(a)X
3056(pointer)X
3328(to)X
3419(the)X
3549(appropriate)X
5 f
570 5456(XFontStruct)N
1 f
1074(structure.)X
1448(If)X
1528(the)X
1658(font)X
1822(does)X
2005(not)X
2140(exist,)X
5 f
2369(XLoadQueryFont)X
1 f
3078(returns)X
3344(NULL.)X
555 5580(The)N
5 f
732(XFontStruct)X
1 f
1236(structure)X
1566(contains)X
1882(all)X
1993(of)X
2088(the)X
2218(information)X
2657(for)X
2781(the)X
2911(font)X
3075(and)X
3224(consists)X
3525(of)X
3620(the)X
3750(font)X
555 5676(speci\256c)N
846(information)X
1285(as)X
1380(well)X
1554(as)X
1649(a)X
1710(pointer)X
1982(to)X
2073(an)X
2178(array)X
2380(of)X
5 f
2493(XCharStruct)X
1 f
3010(structures)X
3374(for)X
3498(the)X
3628(characters)X
555 5772(contained)N
920(in)X
1011(the)X
1141(font.)X
1349(The)X
1508(information)X
1947(in)X
2038(the)X
5 f
2186(XFontStruct)X
1 f
2668(,)X
5 f
2730(XFontProp)X
1 f
3167(,)X
3211(and)X
5 f
3378(XCharStruct)X
3 f
2239 6144(93)N

94 p
%%Page: 94 98
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(structures)N
919(is:)X
555 864(typedef)N
841(struct)X
1059({)X
843 960(short)N
1041(lbearing;)X
2283(/*)X
2374(origin)X
2607(to)X
2698(left)X
2838(edge)X
3026(of)X
3121(raster)X
3338(*/)X
843 1056(short)N
1041(rbearing;)X
2283(/*)X
2374(origin)X
2607(to)X
2698(right)X
2887(edge)X
3075(of)X
3170(raster)X
3387(*/)X
843 1152(short)N
1041(width;)X
2283(/*)X
2374(advance)X
2684(to)X
2775(next)X
2949(char's)X
3185(origin)X
3418(*/)X
843 1248(short)N
1041(ascent;)X
2283(/*)X
2374(baseline)X
2685(to)X
2776(top)X
2911(edge)X
3099(of)X
3194(raster)X
3411(*/)X
843 1344(short)N
1041(descent;)X
2283(/*)X
2374(baseline)X
2685(to)X
2776(bottom)X
3049(edge)X
3237(of)X
3332(raster)X
3549(*/)X
843 1440(unsigned)N
1183(short)X
1381(attributes;)X
2283(/*)X
2374(per)X
2508(char)X
2681(\257ags)X
2869(\(not)X
3033(prede\256ned\))X
3455(*/)X
555 1536(})N
619(XCharStruct;)X
555 1680(typedef)N
841(struct)X
1059({)X
843 1776(Atom)N
1131(name;)X
843 1872(unsigned)N
1183(long)X
1362(card32;)X
555 1968(})N
619(XFontProp;)X
555 2208(typedef)N
841(struct)X
1059({)X
2283(/*)X
2374(normal)X
2646(16)X
2756(bit)X
2872(characters)X
3250(are)X
3379(two)X
3533(bytes)X
3741(*/)X
643 2304(unsigned)N
983(char)X
1156(byte1;)X
643 2400(unsigned)N
983(char)X
1156(byte2;)X
555 2496(})N
619(XChar2b;)X
555 2640(typedef)N
841(struct)X
1059({)X
843 2736(XExtData)N
1534(*ext_data;)X
2283(/*)X
2374(hook)X
2572(for)X
2696(extension)X
3056(to)X
3147(hang)X
3340(data)X
3509(*/)X
843 2832(Font)N
1534(\256d;)X
2283(/*)X
2374(Font)X
2558(id)X
2649(for)X
2773(this)X
2923(font)X
3087(*/)X
843 2928(unsigned)N
1534(direction;)X
2283(/*)X
2374(hint)X
2534(about)X
2752(the)X
2882(direction)X
3218(font)X
3382(is)X
3463(painted)X
3745(*/)X
843 3024(unsigned)N
1534(min_char_or_byte2;)X
2283(/*)X
2374(\256rst)X
2533(character)X
2877(*/)X
843 3120(unsigned)N
1534(max_char_or_byte2;)X
2283(/*)X
2374(last)X
2519(character)X
2863(*/)X
843 3216(unsigned)N
1534(min_byte1;)X
2283(/*)X
2374(\256rst)X
2533(row)X
2691(that)X
2846(exists)X
3069(*/)X
843 3312(unsigned)N
1534(max_byte1;)X
2283(/*)X
2374(last)X
2519(row)X
2677(that)X
2832(exists)X
3055(*/)X
843 3408(Bool)N
1534(all_chars_exist;)X
2283(/*)X
2374(\257ag)X
2528(if)X
2604(all)X
2715(characters)X
2283 3504(have)N
2471(nonzero)X
2776(size*/)X
843 3600(unsigned)N
1534(default_char;)X
2283(/*)X
2374(char)X
2547(to)X
2638(print)X
2827(for)X
2951(unde\256ned)X
3320(character)X
3664(*/)X
843 3696(int)N
1534(n_properties;)X
2283(/*)X
2374(how)X
2547(many)X
2765(properties)X
3139(there)X
3337(are)X
3466(*/)X
843 3792(XFontProp)N
1534(*properties;)X
2283(/*)X
2374(pointer)X
2646(to)X
2737(array)X
2939(of)X
2283 3888(additional)N
2659(properties*/)X
843 3984(XCharStruct)N
1534(min_bounds;)X
2283(/*)X
2374(minimum)X
2741(bounds)X
3017(over)X
3195(all)X
3306(existing)X
3608(char*/)X
843 4080(XCharStruct)N
1534(max_bounds;)X
2283(/*)X
2374(maximum)X
2755(bounds)X
3031(over)X
3209(all)X
3320(existing)X
3622(char*/)X
843 4176(XCharStruct)N
1534(*per_char;)X
2283(/*)X
2374(\256rst_char)X
2728(to)X
2819(last_char)X
3159(information)X
3598(*/)X
843 4272(int)N
1534(ascent;)X
2283(/*)X
2374(logical)X
2637(extent)X
2875(above)X
3107(baseline)X
3418(for)X
3542(spacing)X
3833(*/)X
843 4368(int)N
1534(descent;)X
2283(/*)X
2374(logical)X
2637(decent)X
2889(below)X
3126(baseline)X
3437(for)X
3561(spacing)X
3852(*/)X
555 4464(})N
619(XFontStruct;)X
555 4636(X)N
640(supports)X
960(both)X
1139(single)X
1372(byte/character)X
1893(and)X
2042(two)X
2196(bytes/character)X
2751(text)X
2906(operations.)X
3339(Note)X
3532(that)X
3687(either)X
555 4732(form)N
748(can)X
892(be)X
997(used)X
1180(with)X
1359(a)X
1420(font,)X
1606(but)X
1741(a)X
1802(single)X
2035(byte/character)X
2556(text)X
2711(requests)X
3021(can)X
3165(only)X
3344(specify)X
3620(a)X
3681(single)X
555 4828(byte)N
729(\(that)X
913(is,)X
1016(the)X
1146(\256rst)X
1305(row)X
1463(of)X
1558(a)X
1619(two)X
1773(byte)X
1947(font\).)X
2184(You)X
2357(should)X
2614(view)X
2807(two)X
2961(byte)X
3135(fonts)X
3333(as)X
3428(a)X
3489(two)X
3643(dimen-)X
555 4924(sional)N
788(matrix)X
1041(of)X
1136(de\256ned)X
1417(characters:)X
1820(byte1)X
2038(speci\256es)X
2363(the)X
2493(range)X
2710(of)X
2805(de\256ned)X
3086(rows)X
3278(and)X
3427(byte2)X
3645(de\256nes)X
555 5020(the)N
685(range)X
902(of)X
997(de\256ned)X
1278(columns)X
1599(of)X
1694(the)X
1824(font.)X
2032(Single)X
2280(byte/character)X
2801(fonts)X
2999(have)X
3187(no)X
3297(rows)X
3489(de\256ned,)X
3792(and)X
555 5116(the)N
685(byte2)X
903(range)X
1120(speci\256ed)X
1455(in)X
1546(the)X
1676(structure)X
2006(de\256nes)X
2277(a)X
2338(range)X
2555(of)X
2650(characters.)X
555 5240(The)N
714(bounding)X
1069(box)X
1223(of)X
1318(a)X
1379(character)X
1723(is)X
1804(de\256ned)X
2085(by)X
2195(the)X
5 f
2343(XCharStruct)X
1 f
2860(of)X
2955(that)X
3110(character)X
3454(\(see)X
3617(below\).)X
555 5336(However,)N
920(characters)X
1298(may)X
1472(be)X
1577(absent)X
1824(from)X
2017(a)X
2078(font.)X
2286(In)X
2381(this)X
2531(case,)X
2726(the)X
2856(default)X
3123(font)X
3287(is)X
3368(used.)X
3595(Some)X
3818(fonts)X
555 5432(may)N
729(have)X
917(characters)X
1295(all)X
1406(the)X
1536(same)X
1739(size.)X
1942(In)X
2037(this)X
2187(case,)X
2382(only)X
2561(the)X
2691(information)X
3130(in)X
3221(the)X
5 f
3369(XFontStruct)X
1 f
555 5528(characters)N
933(are)X
1062(used.)X
555 5652(The)N
714(members)X
1059(of)X
1154(the)X
5 f
1302(XFontStruct)X
1 f
1806(structure)X
2136(are:)X
3 f
2239 6144(94)N

95 p
%%Page: 95 99
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(The)X
934(direction)X
1270(member)X
1581(can)X
1725(be)X
1830(either)X
5 f
2071(FontLeftToRight)X
1 f
2741(or)X
5 f
2854(FontRightToLeft)X
1 f
3502(.)X
3568(Essentially,)X
775 768(it)N
847(is)X
928(just)X
1078(a)X
1139(hint)X
1299(as)X
1394(to)X
1485(whether)X
1790(most)X
5 f
2002(XCharStruct)X
1 f
2519(elements)X
2855(have)X
3043(a)X
3104(positive)X
3406(\()X
5 f
3475(FontLeft-)X
775 864(ToRight)N
1 f
1098(\))X
1149(or)X
1244(a)X
1305(negative)X
1648(\()X
5 f
1717(FontRightToLeft)X
1 f
2365(\))X
2416(character-width)X
2990(metric.)X
3282(The)X
3441(core)X
3614(protocol)X
775 960(de\256nes)N
1046(no)X
1156(support)X
1442(for)X
1566(vertical)X
1853(text.)X
10 f
555 1084(g)N
1 f
775(If)X
855(the)X
985(min_byte1)X
1385(and)X
1534(max_byte1)X
1948(members)X
2293(are)X
2422(both)X
2601(zero,)X
2796(min_char_or_byte2)X
3508(speci\256es)X
3833(the)X
775 1180(linear)N
998(character)X
1342(index)X
1560(corresponding)X
2085(to)X
2176(the)X
2306(\256rst)X
2465(element)X
2767(of)X
2862(the)X
2992(per_char)X
3321(array,)X
3545(and)X
775 1276(max_char_or_byte2)N
1501(speci\256es)X
1826(the)X
1956(linear)X
2179(character)X
2523(index)X
2741(of)X
2836(the)X
2966(last)X
3111(element.)X
775 1400(If)N
855(either)X
1078(min_byte1)X
1478(or)X
1573(max_byte1)X
1987(are)X
2116(nonzero,)X
2443(both)X
2622(min_char_or_byte2)X
3334(and)X
775 1496(max_char_or_byte2)N
1501(will)X
1661(be)X
1766(less)X
1920(than)X
2094(256,)X
2270(and)X
2419(the)X
2549(two-byte)X
2884(character)X
3228(index)X
3446(values)X
775 1592(corresponding)N
1300(to)X
1391(the)X
1521(per_char)X
1850(array)X
2052(element)X
2354(N)X
2439(\(counting)X
2799(from)X
2992(0\))X
3087(are:)X
995 1716(byte1)N
1213(=)X
1285(N/D)X
1458(+)X
1530(min_byte1)X
995 1812(byte2)N
1213(=)X
1285(N\\D)X
1458(+)X
1530(min_char_or_byte2)X
775 1936(where:)N
951 2060(D)N
1036(=)X
1108(max_char_or_byte2)X
1834(-)X
1885(min_char_or_byte2)X
2597(+)X
2669(1)X
951 2156(/)N
998(=)X
1070(integer)X
1337(division)X
951 2252(\\)N
998(=)X
1070(integer)X
1337(modulus)X
10 f
555 2376(g)N
1 f
775(If)X
855(the)X
985(per_char)X
1314(pointer)X
1586(is)X
1667(NULL,)X
1945(all)X
2056(glyphs)X
2313(between)X
2628(the)X
2758(\256rst)X
2917(and)X
3066(last)X
3211(character)X
3555(indexes)X
775 2472(inclusive)N
1116(have)X
1304(the)X
1434(same)X
1637(information,)X
2098(as)X
2193(given)X
2411(by)X
2521(both)X
2700(min_bounds)X
3158(and)X
3307(max_bounds.)X
10 f
555 2596(g)N
1 f
775(If)X
855(all_chars_exist)X
1406(is)X
5 f
1505(True)X
1 f
1701(,)X
1745(all)X
1856(characters)X
2234(in)X
2325(the)X
2455(per_char)X
2784(array)X
2986(have)X
3174(nonzero)X
3479(bounding)X
775 2692(boxes.)N
10 f
555 2816(g)N
1 f
775(The)X
934(default_char)X
1396(member)X
1707(speci\256es)X
2032(the)X
2162(character)X
2506(that)X
2661(will)X
2821(be)X
2926(used)X
3109(when)X
3321(an)X
3426(unde\256ned)X
3795(or)X
775 2912(nonexistent)N
1204(character)X
1548(is)X
1629(used.)X
1834(The)X
1993(default_char)X
2455(is)X
2536(a)X
2597(16)X
2707(bit)X
2823(character)X
3167(\(not)X
3331(a)X
3392(two)X
3546(byte)X
3720(charac-)X
775 3008(ter\).)N
963(For)X
1107(a)X
1168(font)X
1332(using)X
1545(two)X
1699(byte)X
1873(matrix)X
2126(format,)X
2405(the)X
2535(default_char)X
2997(has)X
3136(byte1)X
3354(in)X
3445(the)X
3575(most)X
775 3104(signi\256cant)N
1165(byte,)X
1361(and)X
1510(byte2)X
1728(in)X
1819(the)X
1949(least)X
2133(signi\256cant)X
2523(byte.)X
2741(If)X
2821(the)X
2951(default_char)X
3413(itself)X
3612(speci\256es)X
775 3200(an)N
880(unde\256ned)X
1249(or)X
1344(nonexistent)X
1773(character,)X
2139(no)X
2249(printing)X
2551(is)X
2632(performed)X
3020(for)X
3144(an)X
3249(unde\256ned)X
3618(or)X
3713(nonex-)X
775 3296(istent)N
989(character.)X
10 f
555 3420(g)N
1 f
775(The)X
934(min_bounds)X
1392(and)X
1541(max_bounds)X
2013(members)X
2358(contain)X
2640(the)X
2770(most)X
2964(extreme)X
3270(values)X
3517(of)X
3612(each)X
3795(indi-)X
775 3516(vidual)N
5 f
1036(XCharStruct)X
1 f
1553(component)X
1967(over)X
2145(all)X
2256(elements)X
2592(of)X
2687(this)X
2837(array)X
3039(\(ignores)X
3349(nonexistent)X
3778(char-)X
775 3612(acters\).)N
1075(The)X
1234(bounding)X
1589(box)X
1743(of)X
1838(the)X
1968(font)X
2132(\(the)X
2291(smallest)X
2603(rectangle)X
2948(enclosing)X
3308(the)X
3438(shape)X
3660(obtained)X
775 3708(by)N
885(superimposing)X
1426(all)X
1537(of)X
1632(the)X
1762(characters)X
2140(at)X
2226(the)X
2356(same)X
2559(origin)X
2792([x,y]\))X
3011(has)X
3150(its)X
3256(upper)X
3478(left)X
3618(coordinate)X
775 3804(at:)N
995 3928([x)N
1090(+)X
1162(min_bounds.lbearing,)X
1953(y)X
2019(-)X
2070(max_bounds.ascent])X
775 4052(Its)N
885(width)X
1108(is:)X
995 4176(max_bounds.rbearing)N
1782(-)X
1833(min_bounds.lbearing)X
775 4300(Its)N
885(height)X
1128(is:)X
995 4396(max_bounds.ascent)N
1709(+)X
1781(max_bounds.descent)X
775 4520(The)N
934(x)X
1000(and)X
1149(y)X
1215(are)X
1344(the)X
1474(lower)X
1696(left)X
1836(corner)X
2082(of)X
2177(the)X
2307(box)X
2461(that)X
2616(is)X
2697(relative)X
2984(to)X
3075(the)X
3205(origin.)X
10 f
555 4644(g)N
1 f
775(The)X
934(ascent)X
1176(member)X
1487(is)X
1568(the)X
1698(logical)X
1961(extent)X
2199(of)X
2294(the)X
2424(font)X
2588(above)X
2820(the)X
2950(baseline)X
3261(that)X
3416(is)X
3497(used)X
3680(for)X
3804(deter-)X
775 4740(mining)N
1048(line)X
1203(spacing.)X
1538(Speci\256c)X
1844(characters)X
2222(may)X
2396(extend)X
2653(beyond)X
2934(this.)X
10 f
555 4864(g)N
1 f
775(The)X
934(descent)X
1220(member)X
1531(is)X
1612(the)X
1742(logical)X
2005(extent)X
2243(of)X
2338(the)X
2468(font)X
2632(at)X
2718(or)X
2813(below)X
3050(the)X
3180(baseline)X
3491(that)X
3646(is)X
3727(used)X
775 4960(for)N
899(determining)X
1348(line)X
1503(spacing.)X
1838(Speci\256c)X
2144(characters)X
2522(may)X
2696(extend)X
2953(beyond)X
3234(this.)X
3428(If)X
3508(the)X
3638(baseline)X
3949(is)X
775 5056(at)N
861(Y-coordinate)X
1347(y,)X
1435(the)X
1565(logical)X
1828(extent)X
2066(of)X
2161(the)X
2291(font)X
2455(is)X
2536(inclusive)X
2877(between)X
3192(the)X
3322(Y-coordinate)X
775 5152(values)N
1022(\(y-font.ascent\))X
1559(and)X
1708(\(y)X
1803(+)X
1875(font.descent)X
2325(-)X
2376(1\).)X
555 5276(A)N
640(font)X
804(is)X
885(not)X
1020(guaranteed)X
1428(to)X
1519(have)X
1707(any)X
1856(properties.)X
2274(The)X
2433(interpretation)X
2931(of)X
3026(the)X
3156(property)X
3476(value)X
3689(\(for)X
555 5372(example,)N
898(int32,)X
1124(card32\))X
1414(must)X
1608(be)X
1713(derived)X
1999(from)X
2192(a)X
2253(priori)X
2471(knowledge)X
2879(of)X
2974(the)X
3104(property.)X
3446(When)X
3678(possible,)X
555 5468(fonts)N
753(should)X
1010(have)X
1198(at)X
1284(least)X
1468(the)X
1598(properties)X
1972(listed)X
2186(in)X
2277(the)X
2407(following)X
2772(table.)X
3010(With)X
3209(atom)X
3408(names,)X
555 5564(upper/lower)N
1002(case)X
1175(matters.)X
1501(The)X
1660(following)X
2025(builtin)X
2279(property)X
2599(atoms)X
2832(can)X
2976(be)X
3081(found)X
3308(in)X
5 f
570 5660(<X11/Xatom.h>)N
1 f
1198(.)X
3 f
2239 6144(95)N

96 p
%%Page: 96 100
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 584(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 728(Property)N
880(Name)X
1716(Type)X
2166(Description)X
10 f
555 784(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 928(MIN_SPACE)N
1716(unsigned)X
2166(The)X
2325(minimum)X
2692(interword)X
3056(spacing)X
3347(\(in)X
3467(pixels\).)X
555 1072(NORM_SPACE)N
1716(unsigned)X
2166(The)X
2325(normal)X
2597(interword)X
2961(spacing)X
3252(\(in)X
3372(pixels\).)X
555 1216(MAX_SPACE)N
1716(unsigned)X
2166(The)X
2325(maximum)X
2706(interword)X
3070(spacing)X
3361(\(in)X
3481(pixels\).)X
555 1360(END_SPACE)N
1716(unsigned)X
2166(The)X
2325(additional)X
2701(spacing)X
2992(\(in)X
3112(pixels\))X
3374(at)X
3460(the)X
3590(end)X
3739(of)X
3834(sen-)X
2166 1456(tences.)N
555 1600(SUPERSCRIPT_X)N
1716(int)X
2166(Offset)X
2407(from)X
2600(the)X
2730(character)X
3074(origin)X
3307(where)X
3543(superscripts)X
2166 1696(should)N
2423(begin)X
2641(\(in)X
2761(pixels\).)X
3067(If)X
3147(the)X
3277(origin)X
3510(is)X
3591(at)X
3677([x,y],)X
3889(then)X
2166 1792(superscripts)N
2608(should)X
2865(begin)X
3083(at)X
2166 1888([x)N
2261(+)X
2333(SUPERSCRIPT_X,)X
3057(y)X
3123(-)X
3174(SUPERSCRIPT_Y].)X
555 2032(SUPERSCRIPT_Y)N
1716(int)X
2166(Offset)X
2407(from)X
2600(the)X
2730(character)X
3074(origin)X
3307(where)X
3543(superscripts)X
2166 2128(should)N
2423(begin)X
2641(\(in)X
2761(pixels\).)X
3067(If)X
3147(the)X
3277(origin)X
3510(is)X
3591(at)X
3677([x,y],)X
3889(then)X
2166 2224(superscripts)N
2608(should)X
2865(begin)X
3083(at)X
2166 2320([x)N
2261(+)X
2333(SUPERSCRIPT_X,)X
3057(y)X
3123(-)X
3174(SUPERSCRIPT_Y].)X
555 2464(SUBSCRIPT_X)N
1716(int)X
2166(Offset)X
2407(from)X
2600(the)X
2730(character)X
3074(origin)X
3307(where)X
3543(subscripts)X
3917(should)X
2166 2560(begin)N
2384(\(in)X
2504(pixels\).)X
2810(If)X
2890(the)X
3020(origin)X
3253(is)X
3334(at)X
3420([x,y],)X
3632(then)X
3806(subscripts)X
2166 2656(should)N
2423(begin)X
2641(at)X
2166 2752([x)N
2261(+)X
2333(SUPERSCRIPT_X,)X
3057(y)X
3123(+)X
3195(SUPERSCRIPT_Y].)X
555 2896(SUBSCRIPT_Y)N
1716(int)X
2166(Offset)X
2407(from)X
2600(the)X
2730(character)X
3074(origin)X
3307(where)X
3543(subscripts)X
3917(should)X
2166 2992(begin)N
2384(\(in)X
2504(pixels\).)X
2810(If)X
2890(the)X
3020(origin)X
3253(is)X
3334(at)X
3420([x,y],)X
3632(then)X
3806(subscripts)X
2166 3088(should)N
2423(begin)X
2641(at)X
2166 3184([x)N
2261(+)X
2333(SUPERSCRIPT_X,)X
3057(y)X
3123(+)X
3195(SUPERSCRIPT_Y].)X
555 3328 -0.2022(UNDERLINE_POSITION)AN
1716(int)X
2166(Y)X
2251(offset)X
2473(from)X
2666(the)X
2796(baseline)X
3107(to)X
3198(the)X
3328(top)X
3463(of)X
3558(an)X
3663(underline)X
4018(\(in)X
2166 3424(pixels\).)N
2472(If)X
2552(the)X
2682(baseline)X
2993(is)X
3074(Y-coordinate)X
3560(y,)X
3648(then)X
3822(the)X
3952(top)X
4087(of)X
2166 3520(the)N
2296(underline)X
2651(is)X
2732(at)X
2818(\(y)X
2913(+)X
2985 -0.1974(UNDERLINE_POSITION\).)AX
555 3664(UNDERLINE_THICKNESS)N
1716(unsigned)X
2166(Thickness)X
2545(of)X
2640(the)X
2770(underline)X
3125(\(in)X
3245(pixels\))X
555 3808(STRIKEOUT_ASCENT)N
1716(int)X
2166(Vertical)X
2472(extents)X
2744(for)X
2868(boxing)X
3135(or)X
3230(voiding)X
3522(characters)X
3900(\(in)X
4020(pix-)X
2166 3904(els\).)N
2359(If)X
2439(the)X
2569(baseline)X
2880(is)X
2961(at)X
3047(Y-coordinate)X
3533(y,)X
3621(then)X
3795(the)X
3925(top)X
4060(of)X
2166 4000(the)N
2296(strikeout)X
2627(box)X
2781(is)X
2862(at)X
2948(\(y)X
3043(-)X
3094(STRIKEOUT_ASCENT\),)X
4041(and)X
2166 4096(the)N
2296(height)X
2539(of)X
2634(the)X
2764(box)X
2918(is)X
2166 4192(\(STRIKEOUT_ASCENT)N
3091(+)X
3163(STRIKEOUT_DESCENT\).)X
555 4336(STRIKEOUT_DESCENT)N
1716(int)X
2166(Vertical)X
2472(extents)X
2744(for)X
2868(boxing)X
3135(or)X
3230(voiding)X
3522(characters)X
3900(\(in)X
4020(pix-)X
2166 4432(els\).)N
2359(If)X
2439(the)X
2569(baseline)X
2880(is)X
2961(at)X
3047(Y-coordinate)X
3533(y,)X
3621(then)X
3795(the)X
3925(top)X
4060(of)X
2166 4528(the)N
2296(strikeout)X
2627(box)X
2781(is)X
2862(at)X
2948(\(y)X
3043(-)X
3094(STRIKEOUT_ASCENT\),)X
4041(and)X
2166 4624(the)N
2296(height)X
2539(of)X
2634(the)X
2764(box)X
2918(is)X
2166 4720(\(STRIKEOUT_ASCENT)N
3091(+)X
3163(STRIKEOUT_DESCENT\).)X
555 4864(ITALIC_ANGLE)N
1716(int)X
2166(The)X
2325(angle)X
2538(of)X
2633(the)X
2763(dominant)X
3119(staffs)X
3331(of)X
3426(characters)X
3804(in)X
3895(the)X
2166 4960(font,)N
2352(in)X
2443(degrees)X
2733(scaled)X
2975(by)X
3085(64,)X
3217(relative)X
3504(to)X
3595(the)X
3725(three-o-)X
2166 5056(clock)N
2379(position)X
2686(from)X
2879(the)X
3009(character)X
3353(origin,)X
3608(with)X
3787(positive)X
2166 5152(indicating)N
2542(counterclockwise)X
3180(motion)X
3453(\(as)X
3577(in)X
3668(the)X
2166 5248(XDrawArc)N
2576(functions\).)X
555 5392(X_HEIGHT)N
1716(int)X
2166(``1)X
2290(ex'')X
2453(as)X
2548(in)X
2639(TeX,)X
2839(but)X
2974(expressed)X
3342(in)X
3433(units)X
3627(of)X
3722(pixels.)X
2166 5488(Often)N
2388(the)X
2518(height)X
2761(of)X
2856(lowercase)X
3229(x.)X
555 5632 -0.3889(QUAD_WIDTH)AN
1716(int)X
2166(``1)X
2290(em'')X
2478(as)X
2573(in)X
2664(TeX,)X
2864(but)X
2999(expressed)X
3367(in)X
3458(units)X
3652(of)X
3747(pixels.)X
2166 5728(Often)N
2388(the)X
2518(width)X
2741(of)X
2836(the)X
2966(digits)X
3185(0-9.)X
3 f
2239 6144(96)N

97 p
%%Page: 97 101
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 584(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 728(Property)N
880(Name)X
1716(Type)X
2166(Description)X
10 f
555 784(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 928(WEIGHT)N
1716(unsigned)X
2166(The)X
2325(weight)X
2587(or)X
2682(boldness)X
3012(of)X
3107(the)X
3237(font,)X
3423(expressed)X
3791(as)X
3886(a)X
3947(value)X
2166 1024(between)N
2481(0)X
2547(and)X
2696(1000.)X
555 1168(POINT_SIZE)N
1716(unsigned)X
2166(The)X
2325(point)X
2529(size)X
2688(of)X
2783(this)X
2933(font)X
3097(at)X
3183(the)X
3313(ideal)X
3507(resolution,)X
2166 1264(expressed)N
2534(in)X
2625(1/10ths)X
2907(of)X
3002(points.)X
3262(There)X
3489(are)X
3618(72.27)X
3838(points)X
4076(to)X
2166 1360(the)N
2296(inch.)X
555 1504(RESOLUTION)N
1716(unsigned)X
2166(The)X
2325(number)X
2616(of)X
2711(pixels)X
2944(per)X
3078(point,)X
3304(expressed)X
3672(in)X
3763(1/100ths,)X
4111(at)X
2166 1600(which)N
2403(this)X
2553(font)X
2717(was)X
2875(created.)X
555 1744(CAP_HEIGHT)N
1716(int)X
2166(Y)X
2251(offset)X
2473(from)X
2666(the)X
2796(baseline)X
3107(to)X
3198(the)X
3328(top)X
3463(of)X
3558(the)X
3688(capital)X
2166 1840(letters,)N
2426(ignoring)X
2747(accents,)X
3050(in)X
3141(pixels.)X
3418(If)X
3498(the)X
3628(baseline)X
3939(is)X
4020(at)X
2166 1936(Y-coordinate)N
2652(y,)X
2740(then)X
2914(the)X
3044(top)X
3179(of)X
3274(the)X
3404(capitals)X
3696(is)X
3777(at)X
2166 2032(\(y)N
2261(-)X
2312(CAP_HEIGHT\).)X
10 f
555 2088(i)N
574(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 2260(For)N
699(a)X
760(character)X
1104(origin)X
1337(at)X
1423([x,y],)X
1635(the)X
1765(bounding)X
2120(box)X
2274(of)X
2369(a)X
2430(character)X
2774(\(the)X
2933(smallest)X
3245(rectangle,)X
3612(described)X
555 2356(in)N
646(terms)X
864(of)X
5 f
977(XCharStruct)X
1 f
1494(components,)X
1964(enclosing)X
2324(the)X
2454(character's)X
2861(shape\))X
3112(is)X
3193(a)X
3254(rectangle)X
3599(with)X
3778(its)X
555 2452(upper)N
777(left)X
917(corner)X
1163(at:)X
775 2576([x)N
870(+)X
942(lbearing,)X
1275(y)X
1341(-)X
1392(ascent])X
555 2700(Its)N
665(width)X
888(is:)X
775 2824(rbearing)N
1090(-)X
1141(lbearing)X
555 2948(Its)N
665(height)X
908(is:)X
775 3072(ascent)N
1017(+)X
1089(descent)X
555 3196(The)N
714(origin)X
947(for)X
1071(the)X
1201(next)X
1375(character)X
1719(is)X
1800(de\256ned)X
2081(to)X
2172(be:)X
775 3320([x)N
870(+)X
942(character-width,)X
1538(y])X
555 3444(Note)N
748(that)X
903(the)X
1033(baseline)X
1344(is)X
1425(logically)X
1757(viewed)X
2033(as)X
2128(being)X
2346(just)X
2496(below)X
2733(nondescending)X
3283(characters.)X
3683(When)X
555 3540(descent)N
841(is)X
922(zero,)X
1117(only)X
1296(pixels)X
1529(with)X
1708(Y-coordinates)X
2228(less)X
2382(than)X
2556(y)X
2622(are)X
2751(drawn.)X
3036(And,)X
3231(the)X
3361(origin)X
3594(is)X
3675(logically)X
555 3636(viewed)N
831(as)X
926(being)X
1144(coincident)X
1534(with)X
1713(the)X
1843(left)X
1983(edge)X
2171(of)X
2266(a)X
2327(nonkerned)X
2720(character.)X
3086(When)X
3318(lbearing)X
3629(is)X
3710(zero)X
3883(\(0\),)X
555 3732(no)N
665(pixels)X
898(with)X
1077(X-coordinate)X
1563(less)X
1717(than)X
1891(x)X
1957(are)X
2086(drawn\).)X
2400(Any)X
2573(of)X
2668(these)X
2871(values)X
3118(could)X
3336(be)X
3441(negative.)X
555 3856(The)N
714(interpretation)X
1212(of)X
1307(the)X
1437(attributes)X
1788(member)X
2099(in)X
2190(the)X
5 f
2338(XCharStruct)X
1 f
2855(structure)X
3185(is)X
3266(not)X
3401(de\256ned)X
3682(by)X
3792(the)X
555 3952(core)N
728(X)X
813(protocol.)X
1173(A)X
1258(nonexistent)X
1687(character)X
2031(is)X
2112(represented)X
2539(with)X
2718(all)X
2829(members)X
3174(of)X
5 f
3287(XCharStruct)X
1 f
3804(set)X
3924(to)X
555 4048(zero)N
728(\(0\).)X
555 4172(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XLoadQueryFont)X
1 f
2511(is)X
5 f
2610(BadAlloc)X
1 f
2972(.)X
555 4392(Use)N
5 f
731(XFreeFont)X
1 f
1185(to)X
1276(unload)X
1538(the)X
1668(font)X
1832(and)X
1981(free)X
2139(the)X
2269(storage)X
2545(used)X
2728(by)X
2838(the)X
2968(font)X
3132(structure)X
3462(that)X
3617(was)X
3775(allo-)X
555 4488(cated)N
763(by)X
5 f
891(XQueryFont)X
1 f
1404(or)X
5 f
1517(XLoadQueryFont)X
1 f
2204(.)X
2270(The)X
2429(de\256nition)X
2790(for)X
2914(this)X
3064(function)X
3380(is:)X
555 4612(XFreeFont)N
943(\()X
2 f
979(display)X
1 f
1234(,)X
2 f
1278(font_struct)X
1 f
1668(\))X
687 4708(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4804(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
2 f
555 4956(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5080(font_struct)N
1 f
1131(Speci\256es)X
1471(the)X
1601(storage)X
1877(associated)X
2261(with)X
2440(the)X
2570(font.)X
555 5204(The)N
5 f
732(XFreeFont)X
1 f
1186(function)X
1502(deletes)X
1769(the)X
1899(association)X
2313(between)X
2628(the)X
2758(font)X
2922(resource)X
3241(ID)X
3355(and)X
3504(the)X
3634(speci\256ed)X
555 5300(font.)N
763(The)X
922(font)X
1086(itself)X
1285(will)X
1445(be)X
1550(freed)X
1752(when)X
1964(no)X
2074(other)X
2277(resource)X
2596(references)X
2978(it.)X
3094(The)X
3253(data)X
3422(and)X
3571(the)X
3701(font)X
555 5396(should)N
812(not)X
947(be)X
1052(referenced)X
1444(again.)X
555 5520(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XFreeFont)X
1 f
2256(is)X
5 f
2355(BadFont)X
1 f
2704(.)X
555 5740(Use)N
5 f
731(XGetFontProperty)X
1 f
1480(to)X
1571(return)X
1803(the)X
1933(speci\256ed)X
2268(font)X
2432(property.)X
2796(The)X
2955(de\256nition)X
3316(for)X
3440(this)X
3590(function)X
3906(is:)X
3 f
2239 6144(97)N

98 p
%%Page: 98 102
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Bool)N
749(XGetFontProperty)X
1411(\()X
2 f
1447(font_struct)X
1 f
1837(,)X
2 f
1888(atom)X
1 f
2071(,)X
2 f
2122(value_return)X
1 f
2584(\))X
687 768(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
687 864(Atom)N
2 f
910(atom)X
1 f
1093(;)X
687 960(unsigned)N
1027(long)X
1206(*)X
2 f
(value_return)S
1 f
1712(;)X
2 f
555 1112(font_struct)N
1 f
1131(Speci\256es)X
1471(the)X
1601(storage)X
1877(associated)X
2261(with)X
2440(the)X
2570(font.)X
2 f
555 1236(atom)N
1 f
1131(Speci\256es)X
1471(the)X
1601(atom)X
1800(associated)X
2184(with)X
2363(the)X
2493(string)X
2716(name)X
2929(you)X
3083(want)X
3276(returned.)X
2 f
555 1456(value_return)N
1 f
1131(Returns)X
1427(the)X
1557(value)X
1770(of)X
1865(the)X
1995(font)X
2159(property.)X
555 1580(Given)N
792(the)X
922(atom)X
1121(for)X
1245(that)X
1400(property,)X
1742(the)X
5 f
1890(XGetFontProperty)X
1 f
2639(function)X
2955(returns)X
3221(the)X
3351(value)X
3564(of)X
3659(the)X
555 1676(speci\256ed)N
890(font)X
1054(property.)X
1396(The)X
1555(function)X
1871(returns)X
2137(zero)X
2310(\(0\))X
2434(if)X
2510(the)X
2640(atom)X
2839(was)X
2997(not)X
3132(de\256ned)X
3413(or)X
3508(one)X
3657(\(1\))X
3781(if)X
3857(it)X
555 1772(was)N
713(de\256ned.)X
1038(There)X
1265(are)X
1394(a)X
1455(set)X
1575(of)X
1670(prede\256ned)X
2063(atoms)X
2296(for)X
2420(font)X
2584(properties)X
2958(which)X
3195(can)X
3339(be)X
3444(found)X
3671(in)X
5 f
570 1868(<X11/Xatom.h>)N
1 f
1198(.)X
1264(This)X
1443(set)X
1563(contains)X
1879(the)X
2009(standard)X
2329(properties)X
2703(associated)X
3087(with)X
3266(a)X
3327(font.)X
3535(While)X
3773(not)X
555 1964(guaranteed)N
963(to)X
1054(be)X
1159(present,)X
1457(it)X
1529(it)X
1601(is)X
1682(very)X
1860(likely)X
2084(that)X
2239(the)X
2369(prede\256ned)X
2762(font)X
2926(properties)X
3300(will)X
3460(be)X
3565(present.)X
555 2184(Use)N
5 f
731(XUnloadFont)X
1 f
1282(to)X
1373(unload)X
1635(the)X
1765(speci\256ed)X
2100(font)X
2264(that)X
2419(was)X
2577(loaded)X
2834(by)X
5 f
2962(XLoadFont)X
1 f
3409(.)X
3475(The)X
3634(de\256nition)X
555 2280(for)N
679(this)X
829(function)X
1145(is:)X
555 2404(XUnloadFont)N
1046(\()X
2 f
1082(display)X
1 f
1337(,)X
2 f
1381(font)X
1 f
1526(\))X
687 2500(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2596(Font)N
2 f
871(font)X
1 f
1016(;)X
2 f
555 2748(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2872(font)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(ID.)X
555 2996(The)N
5 f
732(XUnloadFont)X
1 f
1283(function)X
1599(deletes)X
1866(the)X
1996(association)X
2410(between)X
2725(the)X
2855(font)X
3019(resource)X
3338(ID)X
3452(and)X
3601(the)X
555 3092(speci\256ed)N
890(font.)X
1098(The)X
1257(font)X
1421(itself)X
1620(will)X
1780(be)X
1885(freed)X
2087(when)X
2299(no)X
2409(other)X
2612(resource)X
2931(references)X
3313(it.)X
3407(The)X
3566(font)X
3730(may)X
3904(be)X
555 3188(unloaded)N
900(on)X
1010(the)X
1140(X)X
1225(server)X
1461(if)X
1537(this)X
1687(is)X
1768(the)X
1898(last)X
2043(reference)X
2391(to)X
2482(the)X
2612(font.)X
2820(In)X
2915(any)X
3064(case,)X
3259(the)X
3389(font)X
3553(should)X
3810(never)X
555 3284(again)N
768(be)X
873(referenced)X
1265(because)X
1565(the)X
1695(resource)X
2014(ID)X
2128(is)X
2209(destroyed.)X
555 3408(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUnloadFont)X
1 f
2353(is)X
5 f
2452(BadFont)X
1 f
2801(.)X
3 f
555 3600(6.5.2.)N
797(Obtaining)X
1202(and)X
1366(Freeing)X
1677(Font)X
1875(Names)X
1 f
555 3724(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(obtain)X
2366(and)X
2515(free)X
2673(font)X
2837(names.)X
3128(Fonts)X
3346(have)X
3534(font)X
3698(names)X
555 3820(that)N
710(usually)X
987(are)X
1116(related)X
1378(or)X
1473(obvious.)X
1818(These)X
2050(functions)X
2400(let)X
2511(you)X
2665(obtain)X
2908(font)X
3072(names)X
3319(by)X
3429(matching)X
3780(a)X
3841(wild)X
555 3916(card)N
728(speci\256cation)X
1196(by)X
1306(querying)X
1641(a)X
1702(font)X
1866(type)X
2040(for)X
2164(a)X
2225(list)X
2356(of)X
2451(available)X
2792(sizes,)X
3007(and)X
3156(so)X
3256(on.)X
555 4136(Use)N
5 f
731(XListFonts)X
1 f
1185(to)X
1276(return)X
1508(a)X
1569(list)X
1700(of)X
1795(the)X
1925(available)X
2266(font)X
2430(names.)X
2721(The)X
2880(de\256nition)X
3241(for)X
3365(this)X
3515(function)X
3831(is:)X
555 4260(char)N
728(**XListFonts)X
1220(\()X
2 f
1256(display)X
1 f
1511(,)X
2 f
1555(pattern)X
1 f
1817(,)X
2 f
1861(maxnames)X
1 f
2231(,)X
2 f
2275(actual_count_return)X
1 f
3007(\))X
687 4356(Display)N
983(*)X
2 f
1034(display)X
1 f
1296(;)X
687 4452(char)N
860(*)X
2 f
911(pattern)X
1 f
1173(;)X
687 4548(int)N
2 f
803(maxnames)X
1 f
1180(;)X
687 4644(int)N
803(*)X
2 f
854(actual_count_return)X
1 f
1586(;)X
2 f
555 4796(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4920(pattern)N
1 f
1131(Speci\256es)X
1471(the)X
1601(null-terminated)X
2168(string)X
2391(associated)X
2775(with)X
2954(the)X
3084(font)X
3248(names)X
3495(that)X
3650(you)X
3804(want)X
1131 5016(returned.)N
1490(You)X
1663(can)X
1807(specify)X
2083(any)X
2232(string,)X
2477(an)X
2582(asterisk)X
2873(\(*\),)X
3019(or)X
3114(a)X
3175(question)X
3496(mark)X
3699(\(?\).)X
3840(The)X
1131 5112(asterisk)N
1422(indicates)X
1758(a)X
1819(wildcard)X
2149(on)X
2259(any)X
2408(number)X
2699(of)X
2794(characters,)X
3194(and)X
3343(the)X
3473(question)X
3794(mark)X
1131 5208(indicates)N
1467(a)X
1528(wildcard)X
1858(on)X
1968(a)X
2029(single)X
2262(character.)X
2 f
555 5332(maxnames)N
1 f
1131(Speci\256es)X
1471(the)X
1601(maximum)X
1982(number)X
2273(of)X
2368(names)X
2615(that)X
2770(are)X
2899(to)X
2990(be)X
3095(in)X
3186(the)X
3316(returned)X
3631(list.)X
2 f
555 5456(actual_count_return)N
1 f
1131 5552(Returns)N
1427(the)X
1557(actual)X
1790(number)X
2081(of)X
2176(font)X
2340(names.)X
555 5676(The)N
5 f
732(XListFonts)X
1 f
1186(function)X
1502(returns)X
1768(an)X
1873(array)X
2075(of)X
2170(font)X
2334(names)X
2581(that)X
2736(match)X
2974(the)X
3104(string)X
3327(you)X
3481(passed)X
3737(to)X
3828(the)X
555 5772(pattern)N
822(argument.)X
1221(The)X
1380(string)X
1603(should)X
1860(be)X
1965(ISO)X
2128(Latin-1,)X
2432(and)X
2581(upper/lower)X
3028(case)X
3201(does)X
3384(not)X
3519(matter.)X
3811(Each)X
3 f
2239 6144(98)N

99 p
%%Page: 99 103
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(string)N
778(is)X
859(terminated)X
1259(by)X
1369(NULL.)X
1669(The)X
1828(maximum)X
2209(number)X
2500(of)X
2595(names)X
2842(returned)X
3157(in)X
3248(the)X
3378(list)X
3509(depends)X
3819(on)X
555 768(the)N
685(value)X
898(you)X
1052(passed)X
1308(to)X
1399(the)X
1529(maxnames)X
1928(argument.)X
2327(The)X
2486(function)X
2802(places)X
3044(the)X
3174(actual)X
3407(number)X
3698(of)X
3793(font)X
555 864(names)N
802(returned)X
1117(in)X
1208(the)X
1338(actual_count_return)X
2065(argument.)X
2464(The)X
2623(client)X
2842(should)X
3099(call)X
5 f
3267(XFreeFontNames)X
1 f
555 960(when)N
767(done)X
960(with)X
1139(the)X
1269(result)X
1487(to)X
1578(free)X
1736(the)X
1866(memory.)X
555 1180(Use)N
5 f
731(XFreeFontNames)X
1 f
1463(to)X
1554(free)X
1712(a)X
1773(font)X
1937(name)X
2150(array.)X
2396(The)X
2555(de\256nition)X
2916(for)X
3040(this)X
3190(function)X
3506(is:)X
555 1304(XFreeFontNames)N
1187(\()X
2 f
1223(list)X
1 f
1339(\))X
687 1400(char)N
860(*)X
2 f
(list)S
1 f
1020([)X
1056(])X
1092(;)X
2 f
555 1552(list)N
1 f
1131(Speci\256es)X
1471(the)X
1601(array)X
1803(of)X
1898(strings)X
2155(you)X
2309(want)X
2502(to)X
2593(free.)X
555 1676(The)N
5 f
732(XFreeFontNames)X
1 f
1464(function)X
1780(frees)X
1972(the)X
2102(array)X
2304(and)X
2453(strings)X
2710(returned)X
3025(by)X
5 f
3153(XListFonts)X
1 f
3585(.)X
3 f
555 1868(6.5.3.)N
797(Setting)X
1083(and)X
1247(Retrieving)X
1665(the)X
1804(Font)X
2002(Search)X
2283(Path)X
1 f
555 1992(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set)X
2243(and)X
2392(retrieve)X
2683(the)X
2813(font)X
2977(search)X
3223(path.)X
555 2212(Use)N
5 f
731 0.2898(XSetFontPath)AX
1 f
1319(to)X
1410(set)X
1530(the)X
1660(font)X
1824(search)X
2070(path.)X
2288(The)X
2447(de\256nition)X
2808(for)X
2932(this)X
3082(function)X
3398(is:)X
555 2336(XSetFontPath)N
1057(\()X
2 f
1093(display)X
1 f
1348(,)X
2 f
1392(directories)X
1 f
1781(,)X
2 f
1825(ndirs)X
1 f
2013(\))X
687 2432(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2528(char)N
860(**)X
2 f
(directories)S
1 f
1337(;)X
687 2624(int)N
2 f
803(ndirs)X
1 f
991(;)X
2 f
555 2776(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2900(directories)N
1 f
1131(Speci\256es)X
1471(the)X
1601(directory)X
1941(path)X
2115(used)X
2298(to)X
2389(look)X
2568(for)X
2692(a)X
2753(font.)X
2961(Setting)X
3234(the)X
3364(path)X
3538(to)X
3629(the)X
3759(empty)X
1131 2996(list)N
1262(restores)X
1557(the)X
1687(default)X
1954(path)X
2128(de\256ned)X
2409(for)X
2533(the)X
2663(X)X
2748(server.)X
2 f
555 3120(ndirs)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(directories)X
2381(in)X
2472(the)X
2602(path.)X
555 3244(The)N
5 f
732 0.2898(XSetFontPath)AX
1 f
1320(function)X
1636(de\256nes)X
1907(the)X
2037(directory)X
2377(search)X
2623(path)X
2797(for)X
2921(font)X
3085(lookup.)X
3396(There)X
3623(is)X
3704(only)X
3883(one)X
555 3340(search)N
801(path)X
975(per)X
1109(X)X
1194(server,)X
1452(not)X
1587(one)X
1736(per)X
1870(client.)X
2133(The)X
2292(interpretation)X
2790(of)X
2885(the)X
3015(strings)X
3272(is)X
3353(operating)X
3708(system)X
555 3436(dependent,)N
961(but)X
1096(they)X
1270(are)X
1399(intended)X
1725(to)X
1816(specify)X
2092(directories)X
2486(to)X
2577(be)X
2682(searched)X
3011(in)X
3102(the)X
3232(order)X
3439(listed.)X
3697(Also,)X
555 3532(the)N
685(contents)X
1001(of)X
1096(these)X
1299(strings)X
1556(are)X
1685(operating)X
2040(system)X
2307(speci\256c)X
2598(and)X
2747(are)X
2876(not)X
3011(intended)X
3337(to)X
3428(be)X
3533(used)X
3716(by)X
555 3628(client)N
774(applications.)X
1267(Usually,)X
1585(the)X
1715(X)X
1800(server)X
2036(is)X
2117(free)X
2275(to)X
2366(cache)X
2588(font)X
2752(information)X
3191(internally)X
3552(rather)X
3779(than)X
555 3724(having)N
817(to)X
908(read)X
1081(fonts)X
1279(from)X
1472(\256les.)X
1685(As)X
1804(a)X
1865(side-effect)X
2258(of)X
2353(executing)X
2718(this)X
2868(function,)X
3206(the)X
3336(X)X
3421(server)X
3657(is)X
555 3820(guaranteed)N
963(to)X
1054(\257ush)X
1247(all)X
1358(cached)X
1624(information)X
2063(about)X
2281(fonts)X
2479(for)X
2603(which)X
2840(there)X
3038(currently)X
3378(are)X
3507(no)X
3617(explicit)X
555 3916(resource)N
874(IDs)X
1022(allocated.)X
1407(The)X
1566(meaning)X
1892(of)X
1987(an)X
2092(error)X
2284(from)X
2477(this)X
2627(request)X
2903(is)X
2984(system)X
3251(speci\256c.)X
555 4040(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802 0.2898(XSetFontPath)AX
1 f
2390(is)X
5 f
2489(BadValue)X
1 f
2886(.)X
555 4260(Use)N
5 f
731 0.3125(XGetFontPath)AX
1 f
1329(to)X
1420(get)X
1550(the)X
1680(current)X
1951(font)X
2115(search)X
2361(path.)X
2579(The)X
2738(de\256nition)X
3099(for)X
3223(this)X
3373(function)X
3689(is:)X
555 4384(char)N
728(**XGetFontPath)X
1332(\()X
2 f
1368(display)X
1 f
1623(,)X
2 f
1667(npaths_return)X
1 f
2173(\))X
687 4480(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4576(int)N
803(*)X
2 f
(npaths_return)S
1 f
1353(;)X
2 f
555 4824(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4948(npaths_return)N
1 f
1131(Returns)X
1427(the)X
1557(number)X
1848(of)X
1943(strings)X
2200(in)X
2291(the)X
2421(font)X
2585(path)X
2759(array.)X
555 5072(The)N
5 f
732 0.3125(XGetFontPath)AX
1 f
1330(function)X
1646(allocates)X
1977(and)X
2126(returns)X
2392(an)X
2497(array)X
2699(of)X
2794(strings)X
3051(containing)X
3446(the)X
3576(search)X
3822(path.)X
555 5168(The)N
714(data)X
883(in)X
974(the)X
1104(font)X
1268(path)X
1442(should)X
1699(be)X
1804(freed)X
2006(when)X
2218(no)X
2328(longer)X
2575(needed.)X
555 5388(Use)N
5 f
731(XFreeFontPath)X
1 f
1367(to)X
1458(free)X
1616(data)X
1785(returned)X
2100(by)X
5 f
2228 0.3125(XGetFontPath)AX
1 f
2804(.)X
2870(The)X
3029(de\256nition)X
3390(for)X
3514(this)X
3664(function)X
555 5484(is:)N
3 f
2239 6144(99)N

100 p
%%Page: 100 104
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XFreeFontPath)N
1100(\()X
2 f
1136(list)X
1 f
1252(\))X
687 768(char)N
860(**)X
2 f
(list)S
1 f
1064(;)X
2 f
555 1016(list)N
1 f
1131(Speci\256es)X
1471(the)X
1601(array)X
1803(of)X
1898(strings)X
2155(you)X
2309(want)X
2502(to)X
2593(free.)X
555 1140(The)N
5 f
732(XFreeFontPath)X
1 f
1368(function,)X
1706(when)X
1918(presented)X
2277(the)X
2407(data)X
2576(from)X
5 f
2787 0.3125(XGetFontPath)AX
1 f
3363(,)X
3407(frees)X
3599(the)X
3729(data)X
555 1236(used)N
738(by)X
848(the)X
978(array.)X
3 f
555 1428(6.5.4.)N
797(Computing)X
1244(Character)X
1651(String)X
1908(Sizes)X
1 f
555 1552(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(compute)X
2449(the)X
2579(width,)X
2824(the)X
2954(logical)X
3217(extents,)X
3511(and)X
3660(server)X
555 1648(information)N
994(about)X
1212(8-bit)X
1401(and)X
1550(16-bit)X
1783(text)X
1938(strings.)X
555 1868(Use)N
5 f
731(XTextWidth)X
1 f
1224(to)X
1315(determine)X
1690(the)X
1820(width)X
2043(of)X
2138(an)X
2243(8-bit)X
2432(character)X
2776(string)X
2999(or)X
5 f
3112(XTextWidth16)X
1 f
3703(to)X
3794(deter-)X
555 1964(mine)N
754(the)X
884(width)X
1107(of)X
1202(a)X
1263(16-bit)X
1496(character.)X
1884(Width)X
2127(is)X
2208(computed)X
2578(by)X
2688(adding)X
2950(the)X
3080(character)X
3424(widths)X
3681(of)X
3776(all)X
3887(of)X
555 2060(the)N
685(characters.)X
1107(It)X
1183(does)X
1366(not)X
1501(matter)X
1749(if)X
1825(the)X
1955(font)X
2119(is)X
2200(an)X
2305(8-bit)X
2494(or)X
2589(16-bit)X
2822(font.)X
3030(These)X
3262(functions)X
3612(return)X
3844(the)X
555 2156(sum)N
724(of)X
819(the)X
949(character)X
1293(metrics,)X
1597(in)X
1688(pixels.)X
1965(The)X
2124(de\256nition)X
2485(for)X
5 f
2627(XTextWidth)X
1 f
3120(is:)X
555 2280(int)N
671(XTextWidth)X
1124(\()X
2 f
1160(font_struct)X
1 f
1550(,)X
2 f
1594(string)X
1 f
1800(,)X
2 f
1844(count)X
1 f
2047(\))X
687 2376(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
687 2472(char)N
860(*)X
2 f
(string)S
1 f
1117(;)X
687 2568(int)N
2 f
803(count)X
1 f
1006(;)X
2 f
555 2720(font_struct)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(used)X
1948(for)X
2072(the)X
2202(width)X
2425(computation.)X
2 f
555 2844(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 2968(count)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(count)X
2163(in)X
2254(the)X
2384(named)X
2641(string.)X
555 3188(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XTextWidth16)X
1 f
1808(is:)X
555 3312(int)N
671(XTextWidth16)X
1212(\()X
2 f
1248(font_struct)X
1 f
1638(,)X
2 f
1682(string)X
1 f
1888(,)X
2 f
1932(count)X
1 f
2135(\))X
687 3408(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
687 3504(XChar2b)N
1031(*)X
2 f
(string)S
1 f
1288(;)X
687 3600(int)N
2 f
803(count)X
1 f
1006(;)X
2 f
555 3752(font_struct)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(used)X
1948(for)X
2072(the)X
2202(width)X
2425(computation.)X
2 f
555 3876(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 4000(count)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(count)X
2163(in)X
2254(the)X
2384(named)X
2641(string.)X
3 f
555 4192(6.5.5.)N
797(Returning)X
1205(Logical)X
1507(Extents)X
1 f
555 4316(Use)N
5 f
731(XTextExtents)X
1 f
1294(to)X
1385(determine)X
1760(the)X
1890(logical)X
2153(extents)X
2425(of)X
2520(the)X
2650(speci\256ed)X
2985(8-bit)X
3174(character)X
3518(string)X
3741(or)X
5 f
570 4412(XTextExtents16)N
1 f
1231(to)X
1322(return)X
1554(the)X
1684(logical)X
1947(extents)X
2219(of)X
2314(the)X
2444(speci\256ed)X
2779(16-bit)X
3012(character)X
3356(string.)X
3623(The)X
3782(logi-)X
555 4508(cal)N
680(extents)X
952(of)X
1047(a)X
1108(string)X
1331(is)X
1412(the)X
1542(width)X
1765(and)X
1914(height)X
2157(of)X
2252(the)X
2382(bounding)X
2737(box)X
2891(occupied)X
3231(by)X
3341(the)X
3471(string)X
3694(in)X
3785(the)X
555 4604(speci\256ed)N
890(font.)X
1098(The)X
1257(de\256nition)X
1618(for)X
5 f
1760(XTextExtents)X
1 f
2323(is:)X
555 4728(XTextExtents)N
1052(\()X
2 f
1088(font_struct)X
1 f
1478(,)X
2 f
1522(string)X
1 f
1735(,)X
2 f
1779(nchars)X
1 f
2025(,)X
2 f
2069(direction_return)X
1 f
2652(,)X
2 f
2696(ascent_return)X
1 f
3185(,)X
2 f
3229(descent_return)X
1 f
3757(,)X
2 f
885 4824(overall_return)N
1 f
1406(\))X
687 4920(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
687 5016(char)N
860(*)X
2 f
(string)S
1 f
1117(;)X
687 5112(int)N
2 f
803(nchars)X
1 f
1049(;)X
687 5208(int)N
803(*)X
2 f
(direction_return)S
1 f
1437(;)X
687 5304(int)N
803(*)X
2 f
(ascent_return)S
1 f
1336(,)X
1380(*)X
2 f
(descent_return)S
1 f
1959(;)X
687 5400(XCharStruct)N
1154(*)X
2 f
(overall_return)S
1 f
1719(;)X
2 f
555 5648(font_struct)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XFontStruct)X
1 f
2547(structure.)X
3 f
2217 6144(100)N

101 p
%%Page: 101 105
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 796(nchars)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(character)X
2930(string.)X
2 f
555 920(direction_return)N
1 f
1138(Returns)X
1434(the)X
1564(value)X
1777(of)X
1872(the)X
2002(direction)X
2338(hint)X
2498(member:)X
5 f
2852(FontLeftToRight)X
1 f
3522(or)X
5 f
3635(Fon-)X
1131 1016(tRightToLeft)N
1 f
1627(.)X
2 f
555 1140(ascent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(ascent)X
1963(member.)X
2 f
555 1264(descent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(descent)X
2007(member.)X
2 f
555 1388(overall_return)N
1 f
1131(Returns)X
1427(the)X
1557(overall)X
1824(size)X
1983(in)X
2074(the)X
2204(speci\256ed)X
5 f
2557(XCharStruct)X
1 f
3074(structure.)X
555 1608(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XTextExtents16)X
1 f
1878(is:)X
555 1732(XTextExtents16)N
1140(\()X
2 f
1176(font_struct)X
1 f
1566(,)X
2 f
1610(string)X
1 f
1823(,)X
2 f
1867(nchars)X
1 f
2113(,)X
2 f
2157(direction_return)X
1 f
2740(,)X
2 f
2784(ascent_return)X
1 f
3273(,)X
2 f
3317(descent_return)X
1 f
3845(,)X
2 f
929 1828(overall_return)N
1 f
1450(\))X
687 1924(XFontStruct)N
1145(*)X
2 f
(font_struct)S
1 f
1579(;)X
687 2020(XChar2b)N
1031(*)X
2 f
(string)S
1 f
1288(;)X
687 2116(int)N
2 f
803(nchars)X
1 f
1049(;)X
687 2212(int)N
803(*)X
2 f
(direction_return)S
1 f
1437(;)X
687 2308(int)N
803(*)X
2 f
(ascent_return)S
1 f
1336(,)X
1380(*)X
2 f
(descent_return)S
1 f
1959(;)X
687 2404(XCharStruct)N
1154(*)X
2 f
(overall_return)S
1 f
1719(;)X
2 f
555 2652(font_struct)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XFontStruct)X
1 f
2547(structure.)X
2 f
555 2776(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 2900(nchars)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(character)X
2930(string.)X
2 f
555 3024(direction_return)N
1 f
1138(Returns)X
1434(the)X
1564(value)X
1777(of)X
1872(the)X
2002(direction)X
2338(hint)X
2498(member:)X
5 f
2852(FontLeftToRight)X
1 f
3522(or)X
5 f
3635(Fon-)X
1131 3120(tRightToLeft)N
1 f
1627(.)X
2 f
555 3244(ascent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(ascent)X
1963(member.)X
2 f
555 3368(descent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(descent)X
2007(member.)X
2 f
555 3492(overall_return)N
1 f
1131(Returns)X
1427(the)X
1557(overall)X
1824(size)X
1983(in)X
2074(the)X
2204(speci\256ed)X
5 f
2557(XCharStruct)X
1 f
3074(structure.)X
555 3616(The)N
5 f
732(XTextExtent)X
1 f
1251(and)X
5 f
1418(XTextExtents16)X
1 f
2079(functions)X
2429(perform)X
2734(the)X
2864(size)X
3023(computation)X
3487(locally)X
3750(and,)X
555 3712(thereby,)N
863(avoid)X
1081(the)X
1211(roundtrip)X
1561(overhead)X
1905(of)X
5 f
2018 0.1953(XQueryTextExtents)AX
1 f
2821(and)X
5 f
2988 0.1806(XQueryTextExtents16)AX
1 f
3867(.)X
555 3808(For)N
699(the)X
829(elements)X
1165(returned)X
1480(from)X
1673(the)X
1803(ascent)X
2045(is)X
2126(the)X
2256(maximum)X
2637(of)X
2732(the)X
2862(ascent)X
3104(metrics)X
3386(of)X
3481(all)X
3592(characters)X
555 3904(in)N
646(the)X
776(string.)X
1043(The)X
1202(descent)X
1488(is)X
1569(the)X
1699(maximum)X
2080(of)X
2175(the)X
2305(descent)X
2591(metrics.)X
2917(The)X
3076(width)X
3299(is)X
3380(the)X
3510(sum)X
3679(of)X
3774(the)X
555 4000(character-width)N
1129(metrics)X
1411(of)X
1506(all)X
1617(characters)X
1995(in)X
2086(the)X
2216(string.)X
2483(For)X
2627(each)X
2810(character)X
3154(in)X
3245(the)X
3375(string,)X
3620(let)X
3731(W)X
3836(be)X
555 4096(the)N
685(sum)X
854(of)X
949(the)X
1079(character-width)X
1653(metrics)X
1935(of)X
2030(all)X
2141(characters)X
2519(preceding)X
2888(it)X
2960(in)X
3051(the)X
3181(string.)X
3448(Let)X
3588(R)X
3669(be)X
3774(the)X
555 4192(right-side-bearing)N
1208(metric)X
1456(of)X
1551(the)X
1681(character)X
2025(plus)X
2194(W.)X
2343(The)X
2502(lbearing)X
2813(member)X
3124(is)X
3205(the)X
3335(minimum)X
3702(L)X
3778(of)X
3873(all)X
555 4288(characters)N
933(in)X
1024(the)X
1154(string.)X
1421(The)X
1580(rbearing)X
1895(member)X
2206(is)X
2287(the)X
2417(maximum)X
2798(R.)X
555 4412(For)N
699(fonts)X
897(de\256ned)X
1178(with)X
1357(linear)X
1580(indexing)X
1911(rather)X
2138(than)X
2312(two-byte)X
2647(matrix)X
2900(indexing,)X
3253(the)X
3383(X)X
3468(server)X
3704(inter-)X
555 4508(prets)N
748(each)X
931(XChar2b)X
1275(as)X
1370(a)X
1431(16-bit)X
1664(number)X
1955(that)X
2110(has)X
2249(been)X
2437(transmitted)X
2857(with)X
3036(the)X
3166(most)X
3360(signi\256cant)X
3750(byte)X
555 4604(\256rst.)N
758(That)X
942(is,)X
1045(byte1)X
1263(of)X
1358(the)X
1488(XChar2b)X
1832(is)X
1913(taken)X
2126(as)X
2221(the)X
2351(most)X
2545(signi\256cant)X
2935(byte.)X
3153(If)X
3233(the)X
3363(font)X
3527(has)X
3666(no)X
555 4700(de\256ned)N
836(default)X
1103(character,)X
1469(unde\256ned)X
1838(characters)X
2216(in)X
2307(the)X
2437(string)X
2660(are)X
2789(taken)X
3002(to)X
3093(have)X
3281(all)X
3392(zero)X
3565(metrics.)X
3 f
555 4892(6.5.6.)N
797(Querying)X
1177(Character)X
1584(String)X
1841(Sizes)X
1 f
555 5016(Use)N
5 f
731 0.1953(XQueryTextExtents)AX
1 f
1534(to)X
1625(query)X
1847(the)X
1977(server)X
2213(for)X
2337(the)X
2467(sizes)X
2660(of)X
2755(an)X
2860(8-bit)X
3049(character)X
3393(string)X
3616(or)X
5 f
570 5112 0.1806(XQueryTextExtents16)AN
1 f
1471(to)X
1562(query)X
1784(the)X
1914(server)X
2150(for)X
2274(the)X
2404(sizes)X
2597(of)X
2692(a)X
2753(16-bit)X
2986(character)X
3330(string.)X
3597(The)X
555 5208(de\256nition)N
916(for)X
5 f
1058 0.1953(XQueryTextExtents)AX
1 f
1861(is:)X
3 f
2217 6144(101)N

102 p
%%Page: 102 106
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XQueryTextExtents)N
1271(\()X
2 f
1307(display)X
1 f
1562(,)X
2 f
1606(font_ID)X
1 f
1880(,)X
2 f
1924(string)X
1 f
2130(,)X
2 f
2174(nchars)X
1 f
2413(,)X
2 f
(direction_return)S
1 f
3018(,)X
2 f
3062(ascent_return)X
1 f
3551(,)X
2 f
995 768(descent_return)N
1 f
1523(,)X
2 f
1567(overall_return)X
1 f
2088(\))X
687 864(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 960(XID)N
2 f
864(font_ID)X
1 f
1145(;)X
687 1056(char)N
860(*)X
2 f
(string)S
1 f
1117(;)X
687 1152(int)N
2 f
803(nchars)X
1 f
1049(;)X
687 1248(int)N
803(*)X
2 f
(direction_return)S
1 f
1437(;)X
687 1344(int)N
803(*)X
2 f
(ascent_return)S
1 f
1336(,)X
1380(*)X
2 f
(descent_return)S
1 f
1959(;)X
687 1440(XCharStruct)N
1154(*)X
2 f
(overall_return)S
1 f
1719(;)X
2 f
555 1592(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1716(font_ID)N
1 f
1131(Speci\256es)X
1471(either)X
1694(the)X
1824(font)X
1988(ID)X
2102(or)X
2197(the)X
2327(graphics)X
2647(context)X
2929(that)X
3084(contains)X
3400(the)X
3530(font.)X
2 f
555 1840(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 1964(nchars)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(character)X
2930(string.)X
2 f
555 2088(direction_return)N
1 f
1138(Returns)X
1434(the)X
1564(value)X
1777(of)X
1872(the)X
2002(direction)X
2338(hint)X
2498(member:)X
5 f
2852(FontLeftToRight)X
1 f
3522(or)X
5 f
3635(Fon-)X
1131 2184(tRightToLeft)N
1 f
1627(.)X
2 f
555 2308(ascent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(ascent)X
1963(member.)X
2 f
555 2432(descent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(descent)X
2007(member.)X
2 f
555 2556(overall_return)N
1 f
1131(Returns)X
1427(the)X
1557(overall)X
1824(size)X
1983(in)X
2074(the)X
2204(speci\256ed)X
5 f
2557(XCharStruct)X
1 f
3074(structure.)X
555 2776(The)N
714(de\256nition)X
1075(for)X
5 f
1217 0.1806(XQueryTextExtents16)AX
1 f
2118(is:)X
555 2900(XQueryTextExtents16)N
1359(\()X
2 f
1395(display)X
1 f
1650(,)X
2 f
1694(font_ID)X
1 f
1968(,)X
2 f
2012(string)X
1 f
2218(,)X
2 f
2262(nchars)X
1 f
2501(,)X
2 f
2545(direction_return)X
1 f
3128(,)X
2 f
3172(ascent_return)X
1 f
3661(,)X
2 f
1083 2996(descent_return)N
1 f
1611(,)X
2 f
1655(overall_return)X
1 f
2176(\))X
687 3092(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3188(XID)N
2 f
864(font_ID)X
1 f
1145(;)X
687 3284(XChar2b)N
1031(*)X
2 f
(string)S
1 f
1288(;)X
687 3380(int)N
2 f
803(nchars)X
1 f
1049(;)X
687 3476(int)N
803(*)X
2 f
(direction_return)S
1 f
1437(;)X
687 3572(int)N
803(*)X
2 f
(ascent_return)S
1 f
1336(,)X
1380(*)X
2 f
(descent_return)S
1 f
1959(;)X
687 3668(XCharStruct)N
1154(*)X
2 f
(overall_return)S
1 f
1719(;)X
2 f
555 3820(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3944(font_ID)N
1 f
1131(Speci\256es)X
1471(either)X
1694(the)X
1824(font)X
1988(ID)X
2102(or)X
2197(the)X
2327(graphics)X
2647(context)X
2929(that)X
3084(contains)X
3400(the)X
3530(font.)X
2 f
555 4068(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 4192(nchars)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(character)X
2930(string.)X
2 f
555 4316(direction_return)N
1 f
1138(Returns)X
1434(the)X
1564(value)X
1777(of)X
1872(the)X
2002(direction)X
2338(hint)X
2498(member:)X
5 f
2852(FontLeftToRight)X
1 f
3522(or)X
5 f
3635(Fon-)X
1131 4412(tRightToLeft)N
1 f
1627(.)X
2 f
555 4536(ascent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(ascent)X
1963(member.)X
2 f
555 4660(descent_return)N
1 f
1131(Returns)X
1427(the)X
1557(font)X
1721(descent)X
2007(member.)X
2 f
555 4784(overall_return)N
1 f
1131(Returns)X
1427(the)X
1557(overall)X
1824(size)X
1983(in)X
2074(the)X
2204(speci\256ed)X
5 f
2557(XCharStruct)X
1 f
3074(structure.)X
555 4908(The)N
5 f
732 0.1953(XQueryTextExtents)AX
1 f
1535(and)X
5 f
1702 0.1806(XQueryTextExtents16)AX
1 f
2603(functions)X
2953(return)X
3185(the)X
3315(logical)X
3578(extents)X
3850(of)X
555 5004(the)N
685(speci\256ed)X
1020(8-bit)X
1209(and)X
1358(16-bit)X
1591(character)X
1935(string)X
2158(in)X
2249(the)X
2379(speci\256ed)X
2714(font)X
2878(or)X
2973(the)X
3103(font)X
3267(contained)X
3632(in)X
3723(the)X
555 5100(speci\256ed)N
890(GC.)X
1078(These)X
1310(functions)X
1660(query)X
1882(the)X
2012(X)X
2097(server)X
2333(and,)X
2504(therefore,)X
2865(suffer)X
3091(the)X
3221(round)X
3448(trip)X
3593(overhead)X
555 5196(that)N
710(is)X
791(avoided)X
1092(by)X
5 f
1220(XTextExtents)X
1 f
1783(and)X
5 f
1950(XTextExtents16)X
1 f
2589(.)X
2655(Both)X
2849(functions)X
3199(return)X
3431(a)X
5 f
3510(XCharStruct)X
1 f
555 5292(structure)N
885(whose)X
1131(members)X
1476(are)X
1605(set)X
1725(to)X
1816(the)X
1946(values)X
2193(discussed)X
2552(in)X
2643(the)X
2773(following)X
3138(paragraphs.)X
555 5416(The)N
714(ascent)X
956(member)X
1267(is)X
1348(set)X
1468(to)X
1559(the)X
1689(maximum)X
2070(of)X
2165(the)X
2295(ascent)X
2537(metrics)X
2819(of)X
2914(all)X
3025(characters)X
3403(in)X
3494(the)X
3624(string.)X
555 5512(The)N
714(descent)X
1000(member)X
1311(is)X
1392(set)X
1512(to)X
1603(the)X
1733(maximum)X
2114(of)X
2209(the)X
2339(descent)X
2625(metrics.)X
2951(The)X
3110(width)X
3333(member)X
3644(is)X
3725(set)X
3845(to)X
555 5608(the)N
685(sum)X
854(of)X
949(the)X
1079(character-width)X
1653(metrics)X
1935(of)X
2030(all)X
2141(characters)X
2519(in)X
2610(the)X
2740(string.)X
3007(For)X
3151(each)X
3334(character)X
3678(in)X
3769(the)X
555 5704(string,)N
800(let)X
911(W)X
1016(be)X
1121(the)X
1251(sum)X
1420(of)X
1515(the)X
1645(character-width)X
2219(metrics)X
2501(of)X
2596(all)X
2707(characters)X
3085(preceding)X
3454(it)X
3526(in)X
3617(the)X
3747(string.)X
555 5800(Let)N
695(L)X
771(be)X
876(the)X
1006(left-side-bearing)X
1610(metric)X
1858(of)X
1953(the)X
2083(character)X
2427(plus)X
2596(W.)X
2745(Let)X
2885(R)X
2966(be)X
3071(the)X
3201(right-side-bearing)X
3 f
2217 6144(102)N

103 p
%%Page: 103 107
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(metric)N
803(of)X
898(the)X
1028(character)X
1372(plus)X
1541(W.)X
1690(The)X
1849(lbearing)X
2160(member)X
2471(is)X
2552(set)X
2672(to)X
2763(the)X
2893(minimum)X
3260(L)X
3336(of)X
3431(all)X
3542(characters)X
3920(in)X
555 768(the)N
685(string.)X
952(The)X
1111(rbearing)X
1426(member)X
1737(is)X
1818(the)X
1948(maximum)X
2329(R.)X
555 892(Note)N
748(that)X
903(the)X
1033(string)X
1256(formal)X
1513(parameter)X
1887(for)X
5 f
2029 0.1806(XQueryTextExtents16)AX
1 f
2930(is)X
3011(of)X
3106(type)X
3280(XChar2b,)X
3646(rather)X
555 988(than)N
729(of)X
824(type)X
998(char)X
1171(as)X
1266(in)X
5 f
1375 0.1953(XQueryTextExtents)AX
1 f
2156(.)X
2222(For)X
2366(fonts)X
2564(de\256ned)X
2845(with)X
3024(linear)X
3247(indexing)X
3578(rather)X
3805(than)X
555 1084(two-byte)N
890(matrix)X
1143(indexing,)X
1496(the)X
1626(X)X
1711(server)X
1947(interprets)X
2302(each)X
2485(XChar2b)X
2829(as)X
2924(a)X
2985(16-bit)X
3218(number)X
3509(that)X
3664(has)X
3803(been)X
555 1180(transmitted)N
975(with)X
1154(the)X
1284(most)X
1478(signi\256cant)X
1868(byte)X
2042(\256rst.)X
2245(That)X
2429(is,)X
2532(byte1)X
2750(of)X
2845(the)X
2975(XChar2b)X
3319(is)X
3400(taken)X
3613(as)X
3708(the)X
3838(most)X
555 1276(signi\256cant)N
945(byte.)X
1163(If)X
1243(the)X
1373(font)X
1537(has)X
1676(no)X
1786(de\256ned)X
2067(default)X
2334(character,)X
2700(unde\256ned)X
3069(characters)X
3447(in)X
3538(the)X
3668(string)X
3891(are)X
555 1372(taken)N
768(to)X
859(have)X
1047(all)X
1158(zero)X
1331(metrics.)X
555 1496(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836 0.1953(XQueryTextExtents)AX
1 f
2639(and)X
5 f
2806 0.1806(XQueryTextExtents16)AX
1 f
3707(are)X
5 f
570 1592(BadGC)N
1 f
896(and)X
5 f
1063(BadFont)X
1 f
1412(.)X
3 f
555 1784(6.6.)N
731(Drawing)X
1080(Text)X
1273(Characters)X
1 f
555 1908(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(draw)X
2320(text)X
2475(characters.)X
2897(This)X
3076(section)X
3348(discusses)X
3697(how)X
3870(to)X
555 2004(draw:)N
10 f
555 2128(g)N
1 f
775(Complex)X
1121(text)X
10 f
555 2252(g)N
1 f
775(Text)X
959(characters)X
10 f
555 2376(g)N
1 f
775(Image)X
1017(text)X
1172(characters)X
555 2500(All)N
690(of)X
785(the)X
915(functions)X
1265(discussed)X
1624(in)X
1715(the)X
1845(following)X
2210(subsections)X
2638(make)X
2851(use)X
2990(of)X
3085(fonts.)X
3327(A)X
3412(font)X
3576(is)X
3657(a)X
3718(graphi-)X
555 2596(cal)N
680(description)X
1094(of)X
1189(a)X
1250(set)X
1370(of)X
1465(characters)X
1843(that)X
1998(are)X
2127(used)X
2310(to)X
2401(increase)X
2711(ef\256ciency)X
3080(whenever)X
3443(a)X
3504(set)X
3624(of)X
3719(small,)X
555 2692(similar)N
823(sized)X
1026(patterns)X
1327(are)X
1456(repeatedly)X
1845(used.)X
555 2816(typedef)N
841(struct)X
1059({)X
843 2912(char)N
1016(*chars;)X
2283(/*)X
2374(pointer)X
2646(to)X
2737(string)X
2960(*/)X
843 3008(int)N
959(nchars;)X
2283(/*)X
2374(number)X
2665(of)X
2760(characters)X
3138(*/)X
843 3104(int)N
959(delta;)X
2283(/*)X
2374(delta)X
2568(between)X
2883(strings)X
3140(*/)X
843 3200(Font)N
1027(font;)X
2283(/*)X
2374(Font)X
2558(to)X
2649(print)X
2838(it)X
2910(in,)X
3023(None)X
3235(don't)X
3443(change)X
3714(*/)X
555 3296(})N
619(XTextItem;)X
555 3488(typedef)N
841(struct)X
1059({)X
843 3584(XChar2b)N
1187(*chars;)X
2283(/*)X
2374(pointer)X
2646(to)X
2737(two)X
2891(byte)X
3065(characters)X
3443(*/)X
843 3680(int)N
959(nchars;)X
2283(/*)X
2374(number)X
2665(of)X
2760(characters)X
3138(*/)X
843 3776(int)N
959(delta;)X
2283(/*)X
2374(delta)X
2568(between)X
2883(strings)X
3140(*/)X
843 3872(Font)N
1027(font;)X
2283(/*)X
2374(font)X
2538(to)X
2629(print)X
2818(it)X
2890(in,)X
3003(None)X
3215(don't)X
3423(change)X
3694(*/)X
555 3968(})N
619(XTextItem16;)X
555 4140(The)N
714(fundamental)X
1177(text)X
1332(functions)X
5 f
1700(XDrawText)X
1 f
2172(and)X
5 f
2339(XDrawText16)X
1 f
2909(use)X
3048(these)X
3251(structures.)X
3659(If)X
3739(the)X
3869(font)X
555 4236(member)N
866(is)X
5 f
965(None)X
1 f
1190(,)X
1234(the)X
1364(font)X
1528(is)X
1609(changed)X
1924(before)X
2170(printing)X
2472(and)X
2621(also)X
2785(is)X
2866(stored)X
3103(in)X
3194(the)X
3324(GC.)X
3512(If)X
3592(an)X
3697(error)X
555 4332(was)N
713(generated)X
1077(during)X
1329(text)X
1484(drawing,)X
1816(the)X
1946(font)X
2110(in)X
2201(the)X
2331(GC)X
2475(is)X
2556(unde\256ned.)X
3 f
555 4524(6.6.1.)N
797(Drawing)X
1146(Complex)X
1505(Text)X
1 f
555 4648(Use)N
5 f
731(XDrawText)X
1 f
1203(to)X
1294(draw)X
1491(8-bit)X
1680(polytext)X
1992(characters)X
2370(or)X
5 f
2483(XDrawText16)X
1 f
3053(to)X
3144(draw)X
3341(16-bit)X
3574(polytext)X
555 4744(characters)N
933(in)X
1024(the)X
1154(speci\256ed)X
1489(drawable.)X
1877(The)X
2036(de\256nition)X
2397(for)X
5 f
2539(XDrawText)X
1 f
3011(is:)X
555 4868(XDrawText)N
981(\()X
2 f
1017(display)X
1 f
1272(,)X
2 f
1316(d)X
1 f
1367(,)X
2 f
1411(gc)X
1 f
1501(,)X
2 f
1545(x)X
1 f
1591(,)X
2 f
1635(y)X
1 f
1681(,)X
2 f
1725(items)X
1 f
1918(,)X
2 f
1962(nitems)X
1 f
2199(\))X
687 4964(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5060(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5156(GC)N
2 f
831(gc)X
1 f
921(;)X
687 5252(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 5348(XTextItem)N
1096(*)X
2 f
(items)S
1 f
1333(;)X
687 5444(int)N
2 f
803(nitems)X
1 f
1040(;)X
2 f
555 5596(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5720(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
3 f
2217 6144(103)N

104 p
%%Page: 104 108
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 796(x)N
555 892(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 988(position)N
1438(for)X
1562(the)X
1692(character)X
2036(\(initial)X
2295(character)X
2639(origin\))X
2901(and)X
3050(are)X
3179(relative)X
3466(to)X
3557(the)X
3687(origin)X
3920(of)X
1131 1084(the)N
1261(speci\256ed)X
1596(drawable.)X
2 f
555 1208(items)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(text)X
2452(items.)X
2 f
555 1332(nitems)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(text)X
2142(items)X
2356(in)X
2447(the)X
2577(array.)X
555 1552(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawText16)X
1 f
1787(is:)X
555 1676(XDrawText16)N
1069(\()X
2 f
1105(display)X
1 f
1360(,)X
2 f
1404(d)X
1 f
1455(,)X
2 f
1499(gc)X
1 f
1589(,)X
2 f
1633(x)X
1 f
1679(,)X
2 f
1723(y)X
1 f
1769(,)X
2 f
1813(items)X
1 f
2006(,)X
2 f
2050(nitems)X
1 f
2287(\))X
687 1772(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1868(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 1964(GC)N
2 f
831(gc)X
1 f
921(;)X
687 2060(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 2156(XTextItem16)N
1184(*)X
2 f
(items)S
1 f
1421(;)X
687 2252(int)N
2 f
803(nitems)X
1 f
1040(;)X
2 f
555 2404(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2528(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2652(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2776(x)N
555 2872(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 2968(position)N
1438(for)X
1562(the)X
1692(character)X
2036(\(the)X
2195(initial)X
2425(character)X
2769(origin\),)X
3053(and)X
3202(are)X
3331(relative)X
3618(to)X
3709(the)X
3839(ori-)X
1131 3064(gin)N
1266(of)X
1361(the)X
1491(speci\256ed)X
1826(drawable.)X
2 f
555 3188(items)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(text)X
2452(items.)X
2 f
555 3312(nitems)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(text)X
2142(items)X
2356(in)X
2447(the)X
2577(array.)X
555 3436(The)N
5 f
732(XDrawText16)X
1 f
1302(function)X
1618(is)X
1699(like)X
5 f
1872(XDrawText)X
1 f
2322(,)X
2366(except)X
2618(two-byte)X
2953(or)X
3048(16-bit)X
3281(characters)X
3659(are)X
3788(used.)X
555 3532(The)N
5 f
732(XDrawText)X
1 f
1204(and)X
5 f
1371(XDrawText16)X
1 f
1941(functions)X
2291(use)X
2430(the)X
2560(following)X
2925(elements)X
3261(in)X
3352(the)X
3482(speci\256ed)X
3817(GC:)X
555 3628(function,)N
893(plane_mask,)X
1358(\256ll_style,)X
1712(font,)X
1898(subwindow_mode,)X
2587(clip_x_origin,)X
3107(clip_y_origin,)X
3627(and)X
555 3724(clip_mask.)N
984(Both)X
1178(functions)X
1528(also)X
1692(use)X
1831(these)X
2034(graphics)X
2354(context)X
2636(mode-dependent)X
3245(components:)X
3718(fore-)X
555 3820(ground,)N
848(background,)X
1307(tile,)X
1465(stipple,)X
1745(ts_x_origin,)X
2191(and)X
2340(ts_y_origin.)X
555 3944(Each)N
753(text)X
908(item)X
1088(is)X
1169(processed)X
1537(in)X
1628(turn.)X
1836(A)X
1921(font)X
2085(member)X
2396(other)X
2599(than)X
5 f
2791(None)X
1 f
3038(in)X
3129(an)X
3234(item)X
3414(causes)X
3665(the)X
3795(font)X
555 4040(to)N
646(be)X
751(stored)X
988(in)X
1079(GC)X
1223(and)X
1372(used)X
1555(for)X
1679(subsequent)X
2092(text.)X
2269(A)X
2354(text)X
2509(element)X
2811(delta)X
3005(speci\256es)X
3330(an)X
3435(additional)X
555 4136(change)N
826(in)X
917(the)X
1047(position)X
1354(along)X
1572(the)X
1702(x)X
1768(axis)X
1932(before)X
2178(the)X
2308(string)X
2531(is)X
2612(drawn.)X
2875(The)X
3034(delta)X
3228(is)X
3309(always)X
3575(added)X
3807(to)X
3898(the)X
555 4232(character)N
899(origin,)X
1154(and)X
1303(is)X
1384(not)X
1519(dependent)X
1903(on)X
2013(any)X
2162(characteristics)X
2688(of)X
2783(the)X
2913(font.)X
3121(Each)X
3319(character)X
3663(image,)X
3923(as)X
555 4328(de\256ned)N
836(by)X
946(the)X
1076(font)X
1240(in)X
1331(the)X
1461(GC,)X
1627(is)X
1708(treated)X
1970(as)X
2065(an)X
2170(additional)X
2546(mask)X
2754(for)X
2878(a)X
2939(\256ll)X
3060(operation)X
3415(on)X
3525(the)X
3655(drawable.)X
555 4424(If)N
635(a)X
696(text)X
851(item)X
1031(generates)X
1385(a)X
5 f
1464(BadFont)X
1 f
1835(error,)X
2049(the)X
2179(previous)X
2504(text)X
2659(items)X
2873(may)X
3047(have)X
3235(been)X
3423(drawn.)X
555 4548(Note)N
748(that)X
903(the)X
1033(chars)X
1240(member)X
1551(of)X
1646(the)X
5 f
1794(XTextItem16)X
1 f
2332(structure)X
2662(is)X
2743(of)X
2838(type)X
3012(XChar2b,)X
3378(rather)X
3605(than)X
3779(of)X
555 4644(type)N
729(char)X
902(as)X
997(it)X
1069(is)X
1150(in)X
1241(the)X
5 f
1389(XTextItem)X
1 f
1829(structure.)X
2203(For)X
2347(fonts)X
2545(de\256ned)X
2826(with)X
3005(linear)X
3228(indexing)X
3559(rather)X
3786(than)X
555 4740(two-byte)N
890(matrix)X
1143(indexing,)X
1496(the)X
1626(X)X
1711(server)X
1947(will)X
2107(interpret)X
2428(each)X
2611(member)X
2922(of)X
3017(the)X
5 f
3165(XChar2b)X
1 f
3549(structure)X
3879(as)X
555 4836(a)N
616(16-bit)X
849(number)X
1140(that)X
1295(has)X
1434(been)X
1622(transmitted)X
2042(most)X
2236(signi\256cant)X
2626(byte)X
2800(\256rst.)X
3003(In)X
3098(other)X
3301(words,)X
3559(the)X
3689(byte1)X
555 4932(member)N
866(of)X
961(the)X
5 f
1109(XChar2b)X
1 f
1493(structure)X
1823(is)X
1904(taken)X
2117(as)X
2212(the)X
2342(most)X
2536(signi\256cant)X
2926(byte.)X
555 5056(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawText)X
1 f
2308(and)X
5 f
2475(XDrawText16)X
1 f
3045(are)X
5 f
3192(BadDrawable)X
1 f
3734(,)X
5 f
570 5152(BadGC)N
1 f
874(,)X
5 f
936(BadFont)X
1 f
1285(,)X
1329(and)X
5 f
1496(BadMatch)X
1 f
1908(.)X
3 f
555 5344(6.6.2.)N
797(Drawing)X
1146(Text)X
1339(Characters)X
1 f
555 5468(Use)N
5 f
731(XDrawString)X
1 f
1261(to)X
1352(draw)X
1549(8-bit)X
1738(text)X
1893(characters)X
2271(or)X
5 f
2384(XDrawString16)X
1 f
3012(to)X
3103(draw)X
3300(16-bit)X
3533(characters)X
3911(in)X
555 5564(the)N
685(speci\256ed)X
1020(drawable.)X
1408(The)X
1567(de\256nition)X
1928(for)X
5 f
2070(XDrawString)X
1 f
2600(is:)X
3 f
2217 6144(104)N

105 p
%%Page: 105 109
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XDrawString)N
1035(\()X
2 f
1071(display)X
1 f
1326(,)X
2 f
1370(d)X
1 f
1421(,)X
2 f
1465(gc)X
1 f
1555(,)X
2 f
1599(x)X
1 f
1645(,)X
2 f
1689(y)X
1 f
1735(,)X
2 f
1779(string)X
1 f
1992(,)X
2 f
2036(length)X
1 f
2264(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 960(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1056(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 1152(char)N
860(*)X
2 f
(string)S
1 f
1117(;)X
687 1248(int)N
2 f
803(length)X
1 f
1031(;)X
2 f
555 1400(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1524(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1648(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1772(x)N
555 1868(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 1964(position)N
1438(for)X
1562(the)X
1692(character)X
2036(\(initial)X
2295(character)X
2639(origin\))X
2901(and)X
3050(are)X
3179(relative)X
3466(to)X
3557(the)X
3687(origin)X
3920(of)X
1131 2060(the)N
1261(speci\256ed)X
1596(drawable.)X
2 f
555 2184(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 2308(length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(string)X
2809(argument.)X
555 2528(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawString16)X
1 f
1845(is:)X
555 2652(XDrawString16)N
1123(\()X
2 f
1159(display)X
1 f
1414(,)X
2 f
1458(d)X
1 f
1509(,)X
2 f
1553(gc)X
1 f
1643(,)X
2 f
1687(x)X
1 f
1733(,)X
2 f
1777(y)X
1 f
1823(,)X
2 f
1867(string)X
1 f
2080(,)X
2 f
2124(length)X
1 f
2352(\))X
687 2748(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2844(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 2940(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3036(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 3132(XChar2b)N
1031(*)X
2 f
(string)S
1 f
1288(;)X
687 3228(int)N
2 f
803(length)X
1 f
1031(;)X
2 f
555 3380(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3504(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 3628(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3752(x)N
555 3848(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 3944(position)N
1438(for)X
1562(the)X
1692(character)X
2036(\(the)X
2195(initial)X
2425(character)X
2769(origin\),)X
3053(and)X
3202(are)X
3331(relative)X
3618(to)X
3709(the)X
3839(ori-)X
1131 4040(gin)N
1266(of)X
1361(the)X
1491(speci\256ed)X
1826(drawable.)X
2 f
555 4164(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 4288(length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(string)X
2809(argument.)X
555 4440(The)N
5 f
732(XDrawString)X
1 f
1262(and)X
5 f
1429(XDrawString16)X
1 f
2057(functions)X
2407(use)X
2546(these)X
2749(graphics)X
3069(context)X
3351(components:)X
3824(func-)X
555 4536(tion,)N
737(plane_mask,)X
1202(\256ll_style,)X
1556(font,)X
1742(subwindow_mode,)X
2431(clip_x_origin,)X
2951(clip_y_origin,)X
3471(and)X
3620(clip_mask.)X
555 4632(Both)N
749(functions)X
1099(also)X
1263(use)X
1402(these)X
1605(graphics)X
1925(context)X
2207(mode-dependent)X
2816(components:)X
3289(foreground,)X
3723(back-)X
555 4728(ground,)N
848(tile,)X
1006(stipple,)X
1286(ts_x_origin,)X
1732(and)X
1881(ts_y_origin.)X
2349(Each)X
2547(character)X
2891(image,)X
3151(as)X
3246(de\256ned)X
3527(by)X
3637(the)X
3767(font)X
3931(in)X
555 4824(the)N
685(GC,)X
851(is)X
932(treated)X
1194(as)X
1289(an)X
1394(additional)X
1770(mask)X
1978(for)X
2102(a)X
2163(\256ll)X
2284(operation)X
2639(on)X
2749(the)X
2879(drawable.)X
555 4948(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawString)X
1 f
2366(and)X
5 f
2533(XDrawString16)X
1 f
3161(are)X
5 f
3308(BadDrawable)X
1 f
3850(,)X
5 f
570 5044(BadGC)N
1 f
874(,)X
5 f
936(BadFont)X
1 f
1285(,)X
1329(and)X
5 f
1496(BadMatch)X
1 f
1908(.)X
3 f
555 5236(6.6.3.)N
797(Drawing)X
1146(Image)X
1402(Text)X
1595(Characters)X
1 f
555 5360(Some)N
778(applications,)X
1249(in)X
1340(particular)X
1700(terminal)X
2017(emulators,)X
2409(need)X
2597(to)X
2688(print)X
2877(image)X
3115(text)X
3270(in)X
3361(which)X
3598(both)X
3777(the)X
555 5456(foreground)N
967(and)X
1116(background)X
1553(bits)X
1703(of)X
1798(each)X
1981(character)X
2325(are)X
2454(painted.)X
2780(This)X
2959(avoids)X
3211(annoying)X
3561(\257icker)X
3808(on)X
555 5552(many)N
773(displays.)X
1128(Use)X
5 f
1304(XDrawImageString)X
1 f
2079(to)X
2170(draw)X
2367(8-bit)X
2556(or)X
5 f
2669(XDrawImageString16)X
1 f
3542(to)X
3633(draw)X
3830(16-)X
555 5648(bit)N
671(image)X
909(text)X
1064(characters)X
1442(in)X
1533(the)X
1663(speci\256ed)X
1998(drawable.)X
2386(These)X
2618(functions)X
2968(also)X
3132(modify)X
3409(both)X
3588(the)X
3718(fore-)X
555 5744(ground)N
826(and)X
975(background)X
1412(pixels)X
1645(in)X
1736(the)X
1866(characters.)X
2288(The)X
2447(de\256nition)X
2808(for)X
5 f
2950(XDrawImageString)X
1 f
3725(is:)X
3 f
2217 6144(105)N

106 p
%%Page: 106 110
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XDrawImageString)N
1255(\()X
2 f
1291(display)X
1 f
1546(,)X
2 f
1590(d)X
1 f
1641(,)X
2 f
1685(gc)X
1 f
1775(,)X
2 f
1819(x)X
1 f
1865(,)X
2 f
1909(y)X
1 f
1955(,)X
2 f
1999(string)X
1 f
2212(,)X
2 f
2256(length)X
1 f
2484(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 960(GC)N
2 f
831(gc)X
1 f
921(;)X
687 1056(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 1152(char)N
860(*)X
2 f
(string)S
1 f
1117(;)X
687 1248(int)N
2 f
803(length)X
1 f
1031(;)X
2 f
555 1400(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1524(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1648(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 1772(x)N
555 1868(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 1964(position)N
1438(for)X
1562(the)X
1692(image)X
1930(text)X
2085(character)X
2429(\(initial)X
2688(character)X
3032(origin\))X
3294(and)X
3443(are)X
3572(relative)X
3859(to)X
1131 2060(the)N
1261(origin)X
1494(of)X
1589(the)X
1719(speci\256ed)X
2054(drawable.)X
2 f
555 2184(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 2308(length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(string)X
2809(argument.)X
555 2528(The)N
714(de\256nition)X
1075(for)X
5 f
1217(XDrawImageString16)X
1 f
2090(is:)X
555 2652(XDrawImageString16)N
1343(\()X
2 f
1379(display)X
1 f
1634(,)X
2 f
1678(d)X
1 f
1729(,)X
2 f
1773(gc)X
1 f
1863(,)X
2 f
1907(x)X
1 f
1953(,)X
2 f
1997(y)X
1 f
2043(,)X
2 f
2087(string)X
1 f
2300(,)X
2 f
2344(length)X
1 f
2572(\))X
687 2748(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2844(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 2940(GC)N
2 f
831(gc)X
1 f
921(;)X
687 3036(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 3132(XChar2b)N
1031(*)X
2 f
(string)S
1 f
1288(;)X
687 3228(int)N
2 f
803(length)X
1 f
1031(;)X
2 f
555 3380(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3504(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 3628(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 3752(x)N
555 3848(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(baseline)X
3643(starting)X
1131 3944(position)N
1438(for)X
1562(the)X
1692(image)X
1930(text)X
2085(character)X
2429(and)X
2578(are)X
2707(relative)X
2994(to)X
3085(the)X
3215(origin)X
3448(of)X
3543(the)X
3673(speci\256ed)X
1131 4040(drawable.)N
2 f
555 4164(string)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(string.)X
2 f
555 4288(length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(characters)X
2365(in)X
2456(the)X
2586(string)X
2809(argument.)X
555 4412(The)N
5 f
732(XDrawImageText16)X
1 f
1547(function)X
1863(is)X
1944(like)X
5 f
2117(XDrawImageText,)X
1 f
2859(except)X
3111(two-byte)X
3446(or)X
3541(16-bit)X
3774(char-)X
555 4508(acters)N
782(are)X
911(used.)X
5 f
1159(XDrawImageString)X
1 f
1934(and)X
5 f
2101(XDrawImageString16)X
1 f
2974(use)X
3113(these)X
3316(graphics)X
3636(context)X
555 4604(components:)N
1028(plane_mask,)X
1493(foreground,)X
1927(background,)X
2386(font,)X
2572(subwindow_mode,)X
3261(clip_x_origin,)X
555 4700(clip_y_origin,)N
1075(and)X
1224(clip_mask.)X
1653(The)X
1812(effect)X
2034(is)X
2115(\256rst)X
2274(to)X
2365(\256ll)X
2486(a)X
2547(destination)X
2957(rectangle)X
3302(with)X
3481(the)X
3611(back-)X
555 4796(ground)N
826(pixel)X
1025(de\256ned)X
1306(in)X
1397(the)X
1527(GC)X
1671(and)X
1820(then)X
1994(to)X
2085(paint)X
2284(the)X
2414(text)X
2569(with)X
2748(the)X
2878(foreground)X
3290(pixel.)X
3533(The)X
3692(upper)X
555 4892(left)N
695(corner)X
941(of)X
1036(the)X
1166(\256lled)X
1370(rectangle)X
1715(is)X
1796(at:)X
775 5016([x)N
870(+)X
942(overall->lbearing,)X
1599(y)X
1665(-)X
1716(font_ascent])X
555 5140(The)N
714(width)X
937(is)X
1018(at:)X
775 5264(overall->rbearing)N
1414(-)X
1465(overall->lbearing)X
555 5388(The)N
714(height)X
957(is)X
1038(at:)X
775 5512(font_ascent)N
1203(+)X
1275(font_descent)X
555 5636(The)N
714(overall->lbearing,)X
1371(overall->rbearing,)X
2032(font_ascent,)X
2482(and)X
2631(font_descent)X
3103(are)X
3232(as)X
3327(would)X
3569(be)X
3674(returned)X
555 5732(by)N
5 f
683 0.1953(XQueryTextExtents)AX
1 f
1486(using)X
1699(gc)X
1804(and)X
1953(string.)X
2220(The)X
2379(function)X
2695(and)X
2844(\256ll_style)X
3176(de\256ned)X
3457(in)X
3548(the)X
3678(GC)X
3822(are)X
555 5828(ignored)N
846(for)X
970(these)X
1173(functions.)X
1545(The)X
1704(effective)X
2034(function)X
2350(is)X
5 f
2449(GXcopy)X
1 f
2778(,)X
2822(and)X
2971(the)X
3101(effective)X
3431(\256ll_style)X
3763(is)X
3 f
2217 6144(106)N

107 p
%%Page: 107 111
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
570 672(FillSolid)N
1 f
891(.)X
555 796(Note)N
748(that)X
903(the)X
1033(string)X
1256(formal)X
1513(parameter)X
1887(for)X
5 f
2029(XDrawImageString16)X
1 f
2902(is)X
2983(of)X
3078(type)X
3252(XChar2b,)X
3618(rather)X
3845(than)X
555 892(of)N
650(type)X
824(char)X
997(as)X
1092(in)X
5 f
1201(XDrawImageString)X
1 f
1954(.)X
2020(For)X
2164(fonts)X
2362(de\256ned)X
2643(with)X
2822(linear)X
3045(indexing)X
3376(rather)X
3603(than)X
3777(two-)X
555 988(byte)N
729(matrix)X
982(indexing,)X
1335(the)X
1465(X)X
1550(server)X
1786(interprets)X
2141(each)X
2324(XChar2b)X
2668(as)X
2763(a)X
2824(16-bit)X
3057(number)X
3348(that)X
3503(has)X
3642(been)X
555 1084(transmitted)N
975(with)X
1154(the)X
1284(most)X
1478(signi\256cant)X
1868(byte)X
2042(\256rst.)X
2245(In)X
2340(other)X
2543(words,)X
2801(the)X
2931(byte1)X
3149(member)X
3460(of)X
3555(the)X
5 f
570 1180(XChar2b)N
1 f
954(structure)X
1284(is)X
1365(taken)X
1578(as)X
1673(the)X
1803(most)X
1997(signi\256cant)X
2387(byte.)X
555 1304(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDrawImageString)X
1 f
2611(and)X
5 f
2778(XDrawImageString16)X
1 f
3651(are)X
5 f
3798(Bad-)X
555 1400(Drawable)N
1 f
940(,)X
5 f
1002(BadGC)X
1 f
1306(,)X
1350(and)X
5 f
1517(BadMatch)X
1 f
1929(.)X
3 f
555 1620(6.7.)N
731(Transferring)X
1242(Images)X
1532(Between)X
1871(Client)X
2123(and)X
2287(Server)X
1 f
555 1744(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(transfer)X
2413(images)X
2685(between)X
3000(a)X
3061(client)X
3280(and)X
3429(the)X
3559(server.)X
555 1840(Because)N
870(the)X
1000(server)X
1236(may)X
1410(require)X
1681(diverse)X
1957(data)X
2126(formats,)X
2439(Xlib)X
2618(provides)X
2943(an)X
3048(image)X
3286(object)X
3524(that)X
3679(fully)X
555 1936(describes)N
904(the)X
1034(data)X
1203(in)X
1294(memory)X
1610(and)X
1759(that)X
1914(provides)X
2239(for)X
2363(basic)X
2566(operations)X
2955(on)X
3065(that)X
3220(data.)X
3411(You)X
3584(should)X
555 2032(reference)N
903(the)X
1033(data)X
1202(through)X
1498(the)X
1628(image)X
1866(object,)X
2126(not)X
2261(the)X
2391(data)X
2560(directly.)X
2896(However,)X
3261(some)X
3469(implementa-)X
555 2128(tions)N
749(of)X
844(the)X
974(Xlib)X
1153(library)X
1410(may)X
1584(ef\256ciently)X
1964(deal)X
2133(with)X
2312(frequently)X
2696(used)X
2879(data)X
3048(formats)X
3339(by)X
3449(replacing)X
3799(rou-)X
555 2224(tines)N
744(in)X
835(the)X
965(procedure)X
1338(vector)X
1580(with)X
1759(special)X
2026(case)X
2199(routines.)X
2549(Supported)X
2933(operations)X
3322(include)X
3604(destroying)X
555 2320(the)N
685(image,)X
945(getting)X
1213(a)X
1274(pixel,)X
1495(storing)X
1762(a)X
1823(pixel,)X
2044(extracting)X
2419(a)X
2480(sub)X
2624(image)X
2862(of)X
2957(an)X
3062(image,)X
3322(and)X
3471(adding)X
3733(a)X
3794(con-)X
555 2416(stant)N
744(to)X
835(an)X
940(image.)X
2197 2592(Note)N
775 2728(See)N
924(Chapter)X
1225(10)X
1335(for)X
1459(further)X
1720(information)X
2159(about)X
2377(these)X
2580(utility)X
2815(functions.)X
3187(Chapter)X
3488(10)X
3598(also)X
775 2824(has)N
914(information)X
1353(about)X
1571(additional)X
1947(library)X
2204(functions)X
2554(used)X
2737(to)X
2828(perform)X
3133(basic)X
3336(operations)X
775 2920(on)N
885(an)X
990(image.)X
555 3124(All)N
690(the)X
820(image)X
1058(manipulation)X
1547(functions)X
1897(discussed)X
2256(in)X
2347(this)X
2497(section)X
2769(make)X
2982(use)X
3121(of)X
3216(the)X
5 f
3364(XImage)X
1 f
3705(data)X
555 3220(structure.)N
929(The)X
1088(members)X
1433(in)X
1524(the)X
5 f
1672(XImage)X
1 f
2013(structure)X
2343(are:)X
555 3344(typedef)N
841(struct)X
1059(_XImage)X
1408({)X
665 3440(int)N
781(width,)X
1026(height;)X
2283(/*)X
2374(size)X
2533(of)X
2628(image)X
2866(*/)X
665 3536(int)N
781(xoffset;)X
2283(/*)X
2374(number)X
2665(of)X
2760(pixels)X
2993(offset)X
3215(in)X
3306(X)X
3391(direction)X
3727(*/)X
665 3632(int)N
781(format;)X
2283(/*)X
2374(XYBitmap,)X
2805(XYPixmap,)X
3245(ZPixmap)X
3591(*/)X
665 3728(char)N
838(*data;)X
2283(/*)X
2374(pointer)X
2646(to)X
2737(image)X
2975(data)X
3144(*/)X
665 3824(int)N
781(byte_order;)X
2283(/*)X
2374(data)X
2543(byte)X
2717(order,)X
2946(LSBFirst,)X
3314(MSBFirst)X
3684(*/)X
665 3920(int)N
781 0.3011(bitmap_unit;)AX
2283(/*)X
2374(quant.)X
2614(of)X
2709(scanline)X
3020(8,)X
3108(16,)X
3240(32)X
3350(*/)X
665 4016(int)N
781(bitmap_bit_order;)X
2283(/*)X
2374(LSBFirst,)X
2742(MSBFirst)X
3112(*/)X
665 4112(int)N
781(bitmap_pad;)X
2283(/*)X
2374(8,)X
2462(16,)X
2594(32)X
2704(either)X
2927(XY)X
3075(or)X
3170(ZPixmap)X
3516(*/)X
665 4208(int)N
781(depth;)X
2283(/*)X
2374(depth)X
2592(of)X
2687(image)X
2925(*/)X
665 4304(int)N
781(bytes_per_line;)X
2283(/*)X
2374(accelerator)X
2782(to)X
2873(next)X
3047(line)X
3202(*/)X
665 4400(int)N
781(bits_per_pixel;)X
2283(/*)X
2374(bits)X
2524(per)X
2658(pixel)X
2857(\(ZPixmap\))X
3261(*/)X
665 4496(unsigned)N
1005(long)X
1184(red_mask;)X
2283(/*)X
2374(bits)X
2524(in)X
2615(z)X
2676(arrangement)X
3138(*/)X
665 4592(unsigned)N
1005(long)X
1184(green_mask;)X
665 4688(unsigned)N
1005(long)X
1184(blue_mask;)X
665 4784(char)N
838(*obdata;)X
2283(/*)X
2374(hook)X
2572(for)X
2696(the)X
2826(object)X
3064(routines)X
3370(to)X
3461(hang)X
3654(on)X
3764(*/)X
665 4880(struct)N
883(funcs)X
1095({)X
2283(/*)X
2374(image)X
2612(manipulation)X
3101(routines)X
3407(*/)X
775 4976(struct)N
993(_XImage)X
1342(*\(*create_image\)\(\);)X
775 5072(int)N
891(\(*destroy_image\)\(\);)X
775 5168(unsigned)N
1115(long)X
1294(\(*get_pixel\)\(\);)X
775 5264(int)N
891(\(*put_pixel\)\(\);)X
775 5360(struct)N
993(_XImage)X
1342(*\(*sub_image\)\(\);)X
775 5456(int)N
891(\(*add_pixel\)\(\);)X
775 5552(})N
839(f;)X
555 5648(})N
619(XImage;)X
3 f
2217 6144(107)N

108 p
%%Page: 108 112
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
5 f
732(XImage)X
1 f
1073(structure)X
1403(describes)X
1752(an)X
1857(image)X
2095(as)X
2190(it)X
2262(exists)X
2485(in)X
2576(the)X
2706(client's)X
2988(memory.)X
3326(The)X
3485(user)X
3653(may)X
555 768(request)N
831(that)X
986(some)X
1194(of)X
1289(the)X
1419(members)X
1764(\(for)X
1917(example,)X
2260(height,)X
2525(width,)X
2770(and)X
2919(xoffset\))X
3214(be)X
3319(changed)X
3634(when)X
3846(the)X
555 864(image)N
793(is)X
874(sent)X
1038(to)X
1129(the)X
1259(server.)X
1517(That)X
1701(is,)X
1804(the)X
1934(user)X
2102(may)X
2276(send)X
2459(a)X
2520(subset)X
2762(of)X
2857(the)X
2987(image.)X
3269(Other)X
3491(members)X
3836(\(for)X
555 960(example,)N
898(byte_order,)X
1323(bitmap_unit,)X
1795(and)X
1944(so)X
2044(forth\))X
2266(are)X
2395(characteristics)X
2921(of)X
3016(both)X
3195(the)X
3325(image)X
3563(and)X
3712(of)X
3807(the)X
555 1056(server.)N
813(If)X
893(these)X
1096(members)X
1441(differ)X
1658(between)X
1973(the)X
2103(image)X
2341(and)X
2490(the)X
2620(server,)X
5 f
2896(XPutImage)X
1 f
3370(makes)X
3617(the)X
555 1152(appropriate)N
978(conversions.)X
1464(If)X
1544(the)X
1674(image)X
1912(is)X
1993(formatted)X
2358(as)X
2453(an)X
2558(XYPixmap,)X
2998(\(that)X
3182(is,)X
3285(the)X
3415(format)X
3672(member)X
555 1248(is)N
636(set)X
756(to)X
847(the)X
977(constant)X
5 f
1311(XYPixmap)X
1 f
1737(\),)X
1810(the)X
1940(the)X
2070(\256rst)X
2229(byte)X
2403(of)X
2498(the)X
2628(\256rst)X
2787(line)X
2942(of)X
3037(plane)X
3250(n)X
3316(is)X
3397(located)X
3674(at)X
3760(the)X
555 1344(address)N
840(\(data)X
1038(+)X
1110(\(n)X
1205(*)X
1271(height)X
1514(*)X
1580(bytes_per_line\)\).)X
555 1564(Use)N
5 f
731(XPutImage)X
1 f
1205(to)X
1296(combine)X
1622(an)X
1727(image)X
1965(in)X
2056(memory)X
2372(with)X
2551(a)X
2612(rectangle)X
2957(of)X
3052(a)X
3113(drawable)X
3457(on)X
3567(your)X
555 1660(display.)N
876(The)X
1035(de\256nition)X
1396(for)X
1520(this)X
1670(function)X
1986(is:)X
555 1784(XPutImage)N
963(\()X
2 f
999(display)X
1 f
1254(,)X
2 f
1298(d)X
1 f
1349(,)X
2 f
1393(gc)X
1 f
1483(,)X
2 f
1527(image)X
1 f
1749(,)X
2 f
1793(src_x)X
1 f
1983(,)X
2 f
2027(src_y)X
1 f
2217(,)X
2 f
2261(dst_x)X
1 f
2454(,)X
2 f
2498(dst_y)X
1 f
2691(,)X
2 f
2735(width)X
1 f
2939(,)X
2 f
2983(height)X
1 f
3211(\))X
731 1880(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 1976(Drawable)N
2 f
1094(d)X
1 f
1145(;)X
731 2072(GC)N
2 f
875(gc)X
1 f
965(;)X
731 2168(XImage)N
1036(*)X
2 f
(image)S
1 f
1302(;)X
731 2264(int)N
2 f
847(src_x)X
1 f
1044(,)X
2 f
1088(src_y)X
1 f
1285(;)X
731 2360(int)N
2 f
847(dst_x)X
1 f
1040(,)X
2 f
1084(dst_y)X
1 f
1277(;)X
731 2456(unsigned)N
1071(int)X
2 f
1187(width)X
1 f
1391(,)X
2 f
1435(height)X
1 f
1663(;)X
2 f
555 2608(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2732(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 2856(gc)N
1 f
1131(Speci\256es)X
1471(the)X
1601(graphics)X
1921(context.)X
2 f
555 2980(image)N
1 f
1131(Speci\256es)X
1471(the)X
1601(image)X
1839(you)X
1993(want)X
2186(combined)X
2556(with)X
2735(the)X
2865(rectangle.)X
2 f
555 3104(src_x)N
1 f
1131(Speci\256es)X
1471(the)X
1601(offset)X
1823(in)X
1914(X)X
1999(from)X
2192(the)X
2322(left)X
2462(edge)X
2650(of)X
2745(the)X
2875(image)X
3113(de\256ned)X
3394(by)X
3504(the)X
5 f
3652(XImage)X
1 f
1131 3200(data)N
1300(structure.)X
2 f
555 3324(src_y)N
1 f
1131(Speci\256es)X
1471(the)X
1601(offset)X
1823(in)X
1914(from)X
2107(the)X
2237(top)X
2372(edge)X
2560(of)X
2655(the)X
2785(image)X
3023(de\256ned)X
3304(by)X
3414(the)X
5 f
3562(XImage)X
1 f
1131 3420(data)N
1300(structure.)X
2 f
555 3544(dst_x)N
555 3640(dst_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(are)X
2666(the)X
2796(coordinates)X
3224(of)X
3319(the)X
3449(subimage)X
3809(and)X
1131 3736(are)N
1260(relative)X
1547(to)X
1638(the)X
1768(origin)X
2001(of)X
2096(the)X
2226(drawable)X
2570(where)X
2806(the)X
2936(image)X
3174(will)X
3334(be)X
3439(drawn.)X
2 f
555 3860(width)N
555 3956(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height.)X
2211(These)X
2443(are)X
2572(of)X
2667(the)X
2797(subimage.)X
3201(These)X
3433(arguments)X
1131 4052(de\256ne)N
1368(the)X
1498(dimensions)X
1922(of)X
2017(the)X
2147(rectangle.)X
555 4176(The)N
5 f
732(XPutImage)X
1 f
1206(function)X
1522(combines)X
1882(an)X
1987(image)X
2225(in)X
2316(memory)X
2632(with)X
2811(a)X
2872(rectangle)X
3217(of)X
3312(the)X
3442(speci\256ed)X
3777(draw-)X
555 4272(able.)N
768(The)X
927(function)X
1243(uses)X
1416(these)X
1619(graphics)X
1939(context)X
2221(components:)X
2694(function,)X
3032(plane_mask,)X
555 4368(subwindow_mode,)N
1244(clip_x_origin,)X
1764(clip_y_origin,)X
2284(and)X
2433(clip_mask.)X
2862(This)X
3041(function)X
3357(also)X
3521(uses)X
3694(the)X
555 4464(graphics)N
875(context)X
1157(mode-dependent)X
1766(components)X
2214(foreground)X
2626(and)X
2775(background.)X
3256(If)X
5 f
3354(XYBitmap)X
1 f
3783(for-)X
555 4560(mat)N
710(is)X
791(used,)X
996(the)X
1126(depth)X
1344(must)X
1538(be)X
1643(one,)X
1814(and)X
1963(the)X
2093(image)X
2331(must)X
2525(be)X
2630(XYFormat.)X
3077(Otherwise,)X
3482(a)X
5 f
3561(BadMatch)X
1 f
555 4656(error)N
747(is)X
828(generated.)X
1236(The)X
1395(foreground)X
1807(pixel)X
2006(in)X
2097(the)X
2227(GC)X
2371(de\256nes)X
2642(the)X
2772(source)X
3023(for)X
3147(one)X
3296(bits)X
3446(in)X
3537(the)X
3667(image,)X
555 4752(and)N
704(the)X
834(background)X
1271(pixel)X
1470(de\256nes)X
1741(the)X
1871(source)X
2122(for)X
2246(the)X
2376(zero)X
2549(bits.)X
2743(For)X
5 f
2905(XYPixmap)X
1 f
3353(and)X
5 f
3520(ZPixmap)X
1 f
3882(,)X
555 4848(the)N
685(depth)X
903(must)X
1097(match)X
1335(the)X
1465(depth)X
1683(of)X
1778(drawable.)X
2144(Otherwise,)X
2549(a)X
5 f
2628(BadMatch)X
1 f
3062(error)X
3254(is)X
3335(generated.)X
3743(For)X
5 f
570 4944(XYPixmap)N
1 f
996(,)X
1040(the)X
1170(image)X
1408(must)X
1602(be)X
1707(sent)X
1871(in)X
1962(XYFormat.)X
2409(For)X
5 f
2571(ZPixmap)X
1 f
2933(,)X
2977(the)X
3107(image)X
3345(must)X
3539(be)X
3644(sent)X
3808(in)X
3899(the)X
555 5040(ZFormat)N
886(de\256ned)X
1167(for)X
1291(the)X
1421(given)X
1639(depth.)X
1901(The)X
2060(section)X
2332(of)X
2427(the)X
2557(image)X
2795(de\256ned)X
3076(by)X
3186(the)X
3316(src_x,)X
3550(src_y,)X
3784(width,)X
555 5136(and)N
704(height)X
947(arguments)X
1336(are)X
1465(drawn)X
1706(on)X
1816(the)X
1946(speci\256ed)X
2281(part)X
2440(of)X
2535(the)X
2665(drawable.)X
555 5260(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XPutImage)X
1 f
2310(are)X
5 f
2457(BadDrawable)X
1 f
2999(,)X
5 f
3061(BadGC)X
1 f
3365(,)X
5 f
3427(BadValue)X
1 f
3824(,)X
3868(and)X
5 f
570 5356(BadMatch)N
1 f
982(.)X
555 5576(Use)N
5 f
731(XGetImage)X
1 f
1215(to)X
1306(return)X
1538(the)X
1668(contents)X
1984(of)X
2079(a)X
2140(rectangle)X
2485(in)X
2576(the)X
2706(speci\256ed)X
3041(drawable)X
3385(on)X
3495(the)X
3625(display.)X
555 5672(This)N
734(function)X
1050(is)X
1131(intended)X
1457(speci\256cally)X
1881(for)X
2005(rudimentary)X
2458(hardcopy)X
2807(support.)X
3137(The)X
3296(de\256nition)X
3657(for)X
3781(this)X
555 5768(function)N
871(is:)X
3 f
2217 6144(108)N

109 p
%%Page: 109 113
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XImage)N
860(*XGetImage)X
1321(\()X
2 f
1357(display)X
1 f
1612(,)X
2 f
1656(d)X
1 f
1707(,)X
2 f
1751(x)X
1 f
1797(,)X
2 f
1841(y)X
1 f
1887(,)X
2 f
1931(width)X
1 f
2135(,)X
2 f
2179(height)X
1 f
2407(,)X
2 f
2451(plane_mask)X
1 f
2871(,)X
2 f
2915(format)X
1 f
3157(\))X
731 768(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 864(Drawable)N
2 f
1094(d)X
1 f
1145(;)X
731 960(int)N
2 f
847(x)X
1 f
893(,)X
2 f
937(y)X
1 f
983(;)X
731 1056(unsigned)N
1071(int)X
2 f
1187(width)X
1 f
1391(,)X
2 f
1435(height)X
1 f
1663(;)X
731 1152(long)N
2 f
910(plane_mask)X
1 f
1337(;)X
731 1248(int)N
2 f
847(format)X
1 f
1089(;)X
2 f
555 1400(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1524(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1648(x)N
555 1744(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(upper)X
3554(left)X
3694(corner)X
1131 1840(of)N
1226(the)X
1356(rectangle)X
1701(and)X
1850(are)X
1979(relative)X
2266(to)X
2357(the)X
2487(origin)X
2720(of)X
2815(the)X
2945(drawable.)X
2 f
555 1964(width)N
555 2060(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height)X
2167(of)X
2262(the)X
2392(subimage.)X
2796(These)X
3028(arguments)X
3417(de\256ne)X
3654(the)X
1131 2156(dimensions)N
1555(of)X
1650(the)X
1780(rectangle.)X
2 f
555 2280(plane_mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(plane)X
1814(mask.)X
2 f
555 2404(format)N
1 f
1131(Speci\256es)X
1471(the)X
1601(format)X
1858(for)X
1982(the)X
2112(image.)X
2394(You)X
2567(can)X
2711(pass)X
2884(one)X
3033(of)X
3128(these)X
3331(constants:)X
5 f
3724(XYPix-)X
1131 2500(map)N
1 f
1339(or)X
5 f
1452(ZPixmap)X
1 f
1814(.)X
555 2624(The)N
5 f
732(XGetImage)X
1 f
1216(function)X
1532(returns)X
1798(the)X
5 f
1946(XImage)X
1 f
2287(structure.)X
2661(This)X
2840(structure)X
3170(provides)X
3495(you)X
3649(with)X
3828(the)X
555 2720(contents)N
871(of)X
966(the)X
1096(speci\256ed)X
1431(rectangle)X
1776(of)X
1871(the)X
2001(drawable)X
2345(in)X
2436(the)X
2566(format)X
2823(you)X
2977(specify.)X
3297(If)X
3377(the)X
3507(format)X
3764(argu-)X
555 2816(ment)N
754(is)X
5 f
853(XYPixmap)X
1 f
1279(,)X
1323(the)X
1453(function)X
1769(transmits)X
2115(only)X
2294(the)X
2424(bit)X
2540(planes)X
2787(you)X
2941(passed)X
3197(to)X
3288(the)X
3418(plane_mask)X
555 2912(argument.)N
954(If)X
1034(the)X
1164(plane_mask)X
1607(argument)X
1962(only)X
2141(requests)X
2451(a)X
2512(subset)X
2754(of)X
2849(the)X
2979(planes)X
3226(of)X
3321(the)X
3451(display,)X
3750(the)X
555 3008(depth)N
773(of)X
868(the)X
998(returned)X
1313(image)X
1551(will)X
1711(be)X
1816(the)X
1946(number)X
2237(of)X
2332(planes)X
2579(requested.)X
2982(If)X
3062(the)X
3192(format)X
3449(argument)X
3804(is)X
5 f
570 3104(ZPixmap)N
1 f
932(,)X
976(the)X
1106(function)X
1422(transmits)X
1768(as)X
1863(zero)X
2036(the)X
2166(bits)X
2316(in)X
2407(all)X
2518(planes)X
2765(not)X
2900(speci\256ed)X
3235(in)X
3326(the)X
3456(plane_mask)X
555 3200(argument.)N
954(The)X
1113(function)X
1429(performs)X
1768(no)X
1878(range)X
2095(checking)X
2435(on)X
2545(the)X
2675(values)X
2922(in)X
3013(plane_mask)X
3456(and)X
3605(ignores)X
555 3296(extraneous)N
958(bits.)X
5 f
570 3420(XGetImage)N
1 f
1054(returns)X
1320(the)X
1450(depth)X
1668(of)X
1763(the)X
1893(image)X
2131(to)X
2222(the)X
2352(depth)X
2570(member)X
2881(of)X
2976(the)X
5 f
3124(XImage)X
1 f
3465(structure.)X
3839(The)X
555 3516(depth)N
773(of)X
868(the)X
998(image)X
1236(is)X
1317(as)X
1412(speci\256ed)X
1747(when)X
1959(the)X
2089(drawable)X
2433(was)X
2591(created.)X
555 3640(If)N
635(the)X
765(drawable)X
1109(is)X
1190(a)X
1251(pixmap,)X
1560(then)X
1734(the)X
1864(given)X
2082(rectangle)X
2427(must)X
2621(be)X
2726(wholly)X
2993(contained)X
3358(within)X
3606(the)X
3736(pix-)X
555 3736(map,)N
751(otherwise)X
1115(the)X
1245(X)X
1330(server)X
1566(will)X
1726(generate)X
2046(a)X
5 f
2125(BadMatch)X
1 f
2559(error.)X
2795(If)X
2875(the)X
3005(drawable)X
3349(is)X
3430(a)X
3491(window,)X
3818(the)X
555 3832(window)N
860(must)X
1054(be)X
1159(mapped,)X
1482(and)X
1631(if)X
1707(there)X
1905(were)X
2097(no)X
2207(inferiors)X
2527(or)X
2622(overlapping)X
3065(windows,)X
3426(the)X
3556(speci\256ed)X
555 3928(rectangle)N
900(of)X
995(the)X
1125(window)X
1430(would)X
1672(be)X
1777(fully)X
1966(visible)X
2224(on)X
2334(the)X
2464(screen)X
2710(and)X
2859(wholly)X
3126(contained)X
3491(within)X
3739(the)X
3869(out-)X
555 4024(side)N
719(edges)X
941(of)X
1036(the)X
1166(window.)X
1515(Otherwise,)X
1920(a)X
5 f
1999(BadMatch)X
1 f
2433(error)X
2625(is)X
2706(generated.)X
3114(Note)X
3307(that)X
3462(the)X
3592(borders)X
3877(of)X
555 4120(the)N
685(window)X
990(can)X
1134(be)X
1239(included)X
1565(and)X
1714(read)X
1887(with)X
2066(this)X
2216(request.)X
2536(If)X
2616(the)X
2746(window)X
3051(has)X
3190(a)X
3251(backing-store,)X
3774(the)X
555 4216(backing-store)N
1056(contents)X
1372(are)X
1501(returned)X
1816(for)X
1940(regions)X
2221(of)X
2316(the)X
2446(window)X
2751(that)X
2906(are)X
3035(obscured)X
3374(by)X
3484(noninferior)X
555 4312(windows.)N
916(Otherwise,)X
1321(the)X
1451(return)X
1683(contents)X
1999(of)X
2094(such)X
2277(obscured)X
2616(regions)X
2897(are)X
3026(unde\256ned.)X
3439(Also)X
3627(unde\256ned)X
555 4408(are)N
684(the)X
814(returned)X
1129(contents)X
1445(of)X
1540(visible)X
1798(regions)X
2079(of)X
2174(inferiors)X
2494(of)X
2589(different)X
2914(depth)X
3132(than)X
3306(the)X
3436(speci\256ed)X
3771(win-)X
555 4504(dow.)N
555 4628(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGetImage)X
1 f
2320(are)X
5 f
2467(BadDrawable)X
1 f
3009(,)X
5 f
3071(BadValue)X
1 f
3468(,)X
3512(and)X
5 f
3679(Bad-)X
555 4724(Match)N
1 f
810(.)X
555 4916(Use)N
5 f
731(XGetSubImage)X
1 f
1372(to)X
1463(copy)X
1656(the)X
1786(contents)X
2102(of)X
2197(a)X
2258(rectangle)X
2603(in)X
2694(the)X
2824(speci\256ed)X
3159(drawable)X
3503(on)X
3613(the)X
3743(display)X
555 5012(to)N
646(the)X
776(speci\256ed)X
1111(location)X
1418(within)X
1666(a)X
1727(pre-existing)X
2170(image)X
2408(structure.)X
2782(The)X
2941(de\256nition)X
3302(for)X
3426(this)X
3576(function)X
3892(is:)X
3 f
2217 6144(109)N

110 p
%%Page: 110 114
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XImage)N
860(*XGetSubImage)X
1458(\()X
2 f
1494(display)X
1 f
1749(,)X
2 f
1793(d)X
1 f
1844(,)X
2 f
1888(x)X
1 f
1934(,)X
2 f
1978(y)X
1 f
2024(,)X
2 f
2068(width)X
1 f
2272(,)X
2 f
2316(height)X
1 f
2544(,)X
2 f
2588(plane_mask)X
1 f
3008(,)X
2 f
3052(format)X
1 f
3294(,)X
2 f
3338(dest_image)X
1 f
3746(,)X
2 f
3790(dest_x)X
1 f
4022(,)X
2 f
4066(dest_y)X
1 f
4298(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 960(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
687 1056(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 1152(unsigned)N
1027(long)X
2 f
1206(plane_mask)X
1 f
1633(;)X
687 1248(int)N
2 f
803(format)X
1 f
1045(;)X
687 1344(XImage)N
992(*)X
2 f
(dest_image)S
1 f
1444(;)X
687 1440(int)N
2 f
803(dest_x)X
1 f
1035(,)X
2 f
1079(dest_y)X
1 f
1311(;)X
2 f
555 1592(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1716(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
2 f
555 1840(x)N
555 1936(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(de\256ne)X
3202(the)X
3332(upper)X
3554(left)X
3694(corner)X
1131 2032(of)N
1226(the)X
1356(rectangle)X
1701(and)X
1850(are)X
1979(relative)X
2266(to)X
2357(the)X
2487(origin)X
2720(of)X
2815(the)X
2945(drawable.)X
2 f
555 2156(width)N
555 2252(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height)X
2167(of)X
2262(the)X
2392(subimage.)X
2796(These)X
3028(arguments)X
3417(de\256ne)X
3654(the)X
1131 2348(dimensions)N
1555(of)X
1650(the)X
1780(rectangle.)X
2 f
555 2472(plane_mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(plane)X
1814(mask.)X
2 f
555 2596(format)N
1 f
1131(Speci\256es)X
1471(the)X
1601(format)X
1858(for)X
1982(the)X
2112(image.)X
2394(You)X
2567(can)X
2711(pass)X
2884(one)X
3033(of)X
3128(these)X
3331(constants:)X
5 f
3724(XYPix-)X
1131 2692(map)N
1 f
1339(or)X
5 f
1452(ZPixmap)X
1 f
1814(.)X
2 f
555 2816(dest_image)N
1 f
1131(Specify)X
1422(the)X
1552(the)X
1682(destination)X
2092(image.)X
2 f
555 2940(dest_x)N
555 3036(dest_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(destination)X
2896(rectangle)X
3241(relative)X
3528(to)X
3619(its)X
3725(origin.)X
1131 3132(These)N
1363(coordinates)X
1791(specify)X
2067(the)X
2197(upper)X
2419(left)X
2559(corner)X
2805(of)X
2900(the)X
3030(destination)X
3440(rectangle.)X
1131 3228(These)N
1363(coordinates)X
1791(determine)X
2166(where)X
2402(the)X
2532(subimage)X
2892(will)X
3052(be)X
3157(placed)X
3409(within)X
3657(the)X
3787(desti-)X
1131 3324(nation)N
1374(image.)X
555 3448(The)N
5 f
732(XGetSubImage)X
1 f
1373(function)X
1689(updates)X
1980(the)X
2110(Image)X
2352(with)X
2531(the)X
2661(speci\256ed)X
2996(subimage)X
3356(in)X
3447(the)X
3577(same)X
555 3544(manner)N
841(as)X
5 f
954(XGetImage)X
1 f
1416(.)X
1482(If)X
1562(the)X
1692(format)X
1949(argument)X
2304(is)X
5 f
2403(XYPixmap)X
1 f
2829(,)X
2873(the)X
3003(function)X
3319(transmits)X
3665(only)X
3844(the)X
555 3640(bit)N
671(planes)X
918(you)X
1072(passed)X
1328(to)X
1419(the)X
1549(plane_mask)X
1992(argument.)X
2391(If)X
2471(the)X
2601(format)X
2858(argument)X
3213(is)X
5 f
3312(ZPixmap)X
1 f
3674(,)X
3718(the)X
555 3736(function)N
871(transmits)X
1217(as)X
1312(zero)X
1485(the)X
1615(bits)X
1765(in)X
1856(all)X
1967(planes)X
2214(not)X
2349(speci\256ed)X
2684(in)X
2775(the)X
2905(plane_mask)X
3348(argument.)X
3747(The)X
555 3832(function)N
871(performs)X
1210(no)X
1320(range)X
1537(checking)X
1877(on)X
1987(the)X
2117(values)X
2364(in)X
2455(plane_mask)X
2898(and)X
3047(ignores)X
3328(extraneous)X
3731(bits.)X
5 f
570 3956(XGetSubImage)N
1 f
1211(does)X
1394(not)X
1529(update)X
1786(any)X
1935(\256elds)X
2148(in)X
2239(the)X
2369(destination)X
5 f
2797(XImage)X
1 f
3138(structure.)X
3512(The)X
3671(depth)X
3889(of)X
555 4052(the)N
685(destination)X
5 f
1113(XImage)X
1 f
1454(structure)X
1784(must)X
1978(be)X
2083(the)X
2213(same)X
2416(as)X
2511(that)X
2666(of)X
2761(the)X
2891(drawable.)X
3279(Otherwise,)X
3684(a)X
5 f
570 4148(BadImage)N
1 f
1009(error)X
1201(is)X
1282(generated.)X
1690(If)X
1770(the)X
1900(speci\256ed)X
2235(subimage)X
2595(does)X
2778(not)X
2913(\256t)X
3009(at)X
3095(the)X
3225(speci\256ed)X
3560(location)X
3867(on)X
555 4244(the)N
685(destination)X
1095(image,)X
1355(the)X
1485(right)X
1674(and)X
1823(bottom)X
2096(edges)X
2318(are)X
2447(clipped.)X
2773(If)X
2853(the)X
2983(drawable)X
3327(is)X
3408(a)X
3469(window,)X
3796(the)X
555 4340(window)N
860(must)X
1054(be)X
1159(mapped,)X
1482(and)X
1631(it)X
1703(must)X
1897(be)X
2002(the)X
2132(case)X
2305(that,)X
2482(if)X
2558(there)X
2756(were)X
2948(no)X
3058(inferiors)X
3378(or)X
3473(overlapping)X
555 4436(windows,)N
916(the)X
1046(speci\256ed)X
1381(rectangle)X
1726(of)X
1821(the)X
1951(window)X
2256(would)X
2498(be)X
2603(fully)X
2792(visible)X
3050(on)X
3160(the)X
3290(screen.)X
3580(Otherwise,)X
555 4532(a)N
5 f
634(BadMatch)X
1 f
1068(error)X
1260(is)X
1341(generated.)X
1749(If)X
1829(the)X
1959(window)X
2264(has)X
2403(a)X
2464(backing-store,)X
2987(then)X
3161(the)X
3291(backing-store)X
3792(con-)X
555 4628(tents)N
744(are)X
873(returned)X
1188(for)X
1312(regions)X
1593(of)X
1688(the)X
1818(window)X
2123(that)X
2278(are)X
2407(obscured)X
2746(by)X
2856(noninferior)X
3274(windows.)X
3635(Other-)X
555 4724(wise,)N
760(the)X
890(return)X
1122(contents)X
1438(of)X
1533(such)X
1716(obscured)X
2055(regions)X
2336(are)X
2465(unde\256ned.)X
2878(Also)X
3066(unde\256ned)X
3435(are)X
3564(the)X
3694(returned)X
555 4820(contents)N
871(of)X
966(visible)X
1224(regions)X
1505(of)X
1600(inferiors)X
1920(of)X
2015(different)X
2340(depth)X
2558(than)X
2732(the)X
2862(speci\256ed)X
3197(window.)X
555 4944(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGetSubImage)X
1 f
2477(are)X
5 f
2624(BadDrawable)X
1 f
3166(,)X
5 f
3228(BadGC)X
1 f
3532(,)X
5 f
3594(Bad-)X
555 5040(Value)N
1 f
795(,)X
839(and)X
5 f
1006(BadMatch)X
1 f
1418(.)X
3 f
555 5232(6.8.)N
731(Manipulating)X
1268(Cursors)X
1 f
555 5356(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(manipulate)X
2538(the)X
2668(cursor.)X
2953(This)X
3132(section)X
3404(discusses)X
3753(how)X
555 5452(to:)N
10 f
555 5576(g)N
1 f
775(Create)X
1027(a)X
1088(cursor)X
10 f
555 5700(g)N
1 f
775(Change)X
1066(or)X
1161(destroy)X
1442(a)X
1503(cursor)X
3 f
2217 6144(110)N

111 p
%%Page: 111 115
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 672(g)N
1 f
775(De\256ne)X
1031(the)X
1161(cursor)X
1402(for)X
1526(a)X
1587(window)X
555 796(These)N
787(functions)X
1137(allow)X
1355(you)X
1509(to)X
1600(load)X
1774(and)X
1923(change)X
2194(cursors)X
2469(associated)X
2853(with)X
3032(windows.)X
3415(Each)X
3613(window)X
555 892(can)N
699(have)X
887(a)X
948(different)X
1273(cursor)X
1514(de\256ned)X
1795(for)X
1919(it.)X
2035(Whenever)X
2418(the)X
2548(pointer)X
2820(is)X
2901(in)X
2992(a)X
3053(visible)X
3311(window,)X
3638(it)X
3710(will)X
3870(be)X
555 988(set)N
675(to)X
766(the)X
896(cursor)X
1137(de\256ned)X
1418(for)X
1542(that)X
1697(window.)X
2046(If)X
2126(no)X
2236(cursor)X
2477(was)X
2635(de\256ned)X
2916(for)X
3040(that)X
3195(window,)X
3522(the)X
3652(cursor)X
555 1084(will)N
715(be)X
820(the)X
950(that)X
1105(de\256ned)X
1386(for)X
1510(the)X
1640(parent)X
1882(window.)X
555 1208(From)N
768(X's)X
916(perspective,)X
1361(a)X
1422(cursor)X
1663(consists)X
1964(of)X
2059(a)X
2120(cursor)X
2361(shape,)X
2605(mask,)X
2835(colors)X
3072(for)X
3196(the)X
3326(shape)X
3548(and)X
3697(mask,)X
555 1304(and)N
704(``hot)X
897(spot''.)X
1146(The)X
1305(cursor)X
1546(pixmap)X
1833(determines)X
2242(the)X
2372(shape)X
2594(of)X
2689(the)X
2819(cursor)X
3060(and)X
3209(must)X
3403(be)X
3508(a)X
3569(depth)X
3787(of)X
3882(one)X
555 1400(\(1\).)N
723(The)X
882(mask)X
1090(pixmap)X
1377(determines)X
1786(the)X
1916(bits)X
2066(which)X
2303(will)X
2463(be)X
2568(modi\256ed)X
2904(by)X
3014(the)X
3144(cursor.)X
3429(The)X
3588(colors)X
555 1496(determine)N
930(the)X
1060(colors)X
1297(of)X
1392(the)X
1522(shape)X
1744(and)X
1893(mask.)X
2145(The)X
2304(hot)X
2439(spot)X
2608(de\256nes)X
2879(the)X
3009(point)X
3213(on)X
3323(the)X
3453(cursor)X
3694(which)X
555 1592(will)N
715(be)X
820(reported)X
1135(when)X
1347(a)X
1408(pointer)X
1680(event)X
1893(occurs.)X
2188(There)X
2415(may)X
2589(be)X
2694(and)X
2843(probably)X
3178(are)X
3307 0.3625(limitations)AX
3709(imposed)X
555 1688(by)N
665(the)X
795(hardware)X
1143(on)X
1253(cursors)X
1528(as)X
1623(to)X
1714(size)X
1873(and)X
2022(whether)X
2327(a)X
2388(mask)X
2596(is)X
2677(implemented.)X
5 f
3201(XQueryBestCursor)X
1 f
555 1784(can)N
699(be)X
804(used)X
987(to)X
1078(\256nd)X
1237(out)X
1372(what)X
1565(sizes)X
1758(are)X
1887(possible.)X
2242(In)X
2337(the)X
2467(future,)X
2721(it)X
2793(is)X
2874(intended)X
3200(that)X
3355(most)X
3549(standard)X
3869(cur-)X
555 1880(sors)N
718(will)X
878(be)X
983(stored)X
1220(as)X
1315(a)X
1376(special)X
1643(font.)X
1851(Client)X
2090(programs)X
2444(refer)X
2631(to)X
2722(cursors)X
2997(by)X
3107(using)X
3320(Cursor)X
3581(resource)X
555 1976(IDs.)N
3 f
555 2168(6.8.1.)N
797(Creating)X
1151(a)X
1217(Cursor)X
1 f
555 2292(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(create)X
2355(a)X
2416(font,)X
2602(bitmap,)X
2892(or)X
2987(glyph)X
3210(cursor.)X
555 2512(Use)N
5 f
731(XCreateFontCursor)X
1 f
1531(to)X
1622(create)X
1854(a)X
1915(cursor)X
2156(from)X
2349(a)X
2410(standard)X
2730(font.)X
5 f
2959(XRecolorCursor)X
1 f
3620(de\256ned)X
3901(in)X
555 2608(the)N
685(next)X
859(section)X
1131(can)X
1275(be)X
1380(used)X
1563(to)X
1654(change)X
1925(the)X
2055(color)X
2258(of)X
2353(the)X
2483(cursor)X
2724(to)X
2815(the)X
2945(desired)X
3221(colors.)X
3502(The)X
3661(de\256nition)X
555 2704(for)N
679(this)X
829(function)X
1145(is:)X
555 2828(#include)N
881(<X11/cursorfont.h>)X
555 2972(Cursor)N
816(XCreateFontCursor)X
1517(\()X
2 f
1553(display)X
1 f
1808(,)X
2 f
1852(shape)X
1 f
2064(\))X
687 3068(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3164(unsigned)N
1027(int)X
2 f
1143(shape)X
1 f
1355(;)X
2 f
555 3316(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3440(shape)N
1 f
1131(Speci\256es)X
1471(the)X
1601(shape)X
1823(in)X
1914(which)X
2151(you)X
2305(want)X
2498(to)X
2589(create)X
2821(the)X
2951(standard)X
3271(cursor.)X
555 3564(X)N
640(provides)X
965(a)X
1026(set)X
1146(of)X
1241(standard)X
1561(cursor)X
1802(shapes)X
2058(in)X
2149(a)X
2210(special)X
2477(font)X
2641(named)X
2898(cursorfont.)X
3325(Applications)X
3798(are)X
555 3660(encouraged)N
982(to)X
1073(use)X
1212(this)X
1362(interface)X
1692(for)X
1816(their)X
2000(cursors)X
2275(because)X
2575(the)X
2705(font)X
2869(can)X
3013(be)X
3118(customized)X
3542(for)X
3666(the)X
3796(indi-)X
555 3756(vidual)N
798(display)X
1075(type.)X
1293(The)X
1452(shape)X
1674(argument)X
2029(speci\256es)X
2354(which)X
2591(glyph)X
2814(of)X
2909(the)X
3039(standard)X
3359(fonts)X
3557(to)X
3648(use.)X
555 3880(The)N
714(hotspot)X
996(comes)X
1243(from)X
1436(the)X
1566(information)X
2005(stored)X
2242(in)X
2333(the)X
2463(cursor)X
2704(font.)X
2912(The)X
3071(initial)X
3301(colors)X
3538(of)X
3633(a)X
3694(cursor)X
555 3976(are)N
684(a)X
745(black)X
958(foreground)X
1370(and)X
1519(a)X
1580(white)X
1798(background.)X
2279(See)X
2428(Appendix)X
2797(C)X
2878(for)X
3002(further)X
3263(information)X
3702(about)X
555 4072(cursors)N
830(and)X
979(their)X
1163(shapes)X
1419(in)X
1510(fonts.)X
555 4196(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XCreateFontCursor)X
1 f
2636(are)X
5 f
2783(BadAlloc)X
1 f
3145(,)X
5 f
3207(BadMatch)X
1 f
3619(,)X
3663(and)X
5 f
570 4292(BadValue)N
1 f
967(.)X
555 4512(Use)N
5 f
731(XCreatePixmapCursor)X
1 f
1647(to)X
1738(create)X
1970(a)X
2031(cursor)X
2272(from)X
2465(two)X
2619(bitmaps.)X
2965(The)X
3124(de\256nition)X
3485(for)X
3609(this)X
3759(func-)X
555 4608(tion)N
715(is:)X
555 4732(Cursor)N
816(XCreatePixmapCursor)X
1625(\()X
2 f
1661(display)X
1 f
1916(,)X
2 f
1960(source)X
1 f
2201(,)X
2 f
2245(mask)X
1 f
2432(,)X
2 f
2476(foreground_color)X
1 f
3109(,)X
2 f
3153(background_color)X
1 f
3810(,)X
2 f
3854(x)X
1 f
3900(,)X
2 f
3944(y)X
1 f
3990(\))X
687 4828(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4924(Pixmap)N
2 f
979(source)X
1 f
1220(;)X
687 5020(Pixmap)N
2 f
979(mask)X
1 f
1166(;)X
687 5116(XColor)N
973(*)X
2 f
(foreground_color)S
1 f
1650(;)X
687 5212(XColor)N
973(*)X
2 f
(background_color)S
1 f
1674(;)X
687 5308(unsigned)N
1027(int)X
2 f
1143(x)X
1 f
1189(,)X
2 f
1233(y)X
1 f
1279(;)X
2 f
555 5460(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5584(source)N
1 f
1131(Speci\256es)X
1471(the)X
1601(shape)X
1823(of)X
1918(the)X
2048(source)X
2299(cursor.)X
2 f
555 5708(mask)N
1 f
1131(Speci\256es)X
1471(the)X
1601(source)X
1852(bits)X
2002(of)X
2097(the)X
2227(cursor)X
2468(that)X
2623(are)X
2752(to)X
2843(be)X
2948(displayed.)X
3352(You)X
3525(can)X
3669(pass)X
3842(the)X
1131 5804(constant)N
5 f
1465(None)X
1 f
1690(.)X
3 f
2217 6144(111)N

112 p
%%Page: 112 116
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(foreground_color)N
1 f
1131 768(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(foreground)X
3493(of)X
3588(the)X
3718(source.)X
2 f
555 892(background_color)N
1 f
1131 988(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(background)X
3518(of)X
3613(the)X
3743(source.)X
2 f
555 1112(x)N
555 1208(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates.)X
2305(These)X
2537(coordinates)X
2965(indicate)X
3267(the)X
3397(hot)X
3532(spot)X
3701(relative)X
1131 1304(to)N
1222(the)X
1352(source's)X
1666(origin)X
1899(and)X
2048(must)X
2242(be)X
2347(a)X
2408(point)X
2612(within)X
2860(the)X
2990(source.)X
3263(Otherwise,)X
3668(a)X
5 f
3747(Bad-)X
1131 1400(Match)N
1 f
1408(error)X
1600(is)X
1681(generated.)X
555 1524(The)N
5 f
732(XCreatePixmapCursor)X
1 f
1648(function)X
1964(creates)X
2230(a)X
2291(cursor)X
2532(and)X
2681(returns)X
2947(the)X
3077(cursor)X
3318(ID)X
3432(associated)X
3816(with)X
555 1620(it.)N
671(The)X
830(foreground)X
1242(and)X
1391(background)X
1828(RGB)X
2031(values)X
2278(must)X
2472(be)X
2577(speci\256ed)X
2912(using)X
3125(foreground_color)X
3762(and)X
555 1716(background_color,)N
1239(even)X
1427(if)X
1503(the)X
1633(X)X
1718(server)X
1954(only)X
2133(has)X
2272(a)X
5 f
2351(StaticGray)X
1 f
2800(or)X
5 f
2913(GrayScale)X
1 f
3361(screen.)X
3651(The)X
3810(fore-)X
555 1812(ground)N
826(color)X
1029(is)X
1110(used)X
1293(for)X
1417(the)X
1547(one)X
1696(bits)X
1846(in)X
1937(the)X
2067(source,)X
2340(and)X
2489(the)X
2619(background)X
3056(color)X
3259(is)X
3340(used)X
3523(for)X
3647(the)X
3777(zero)X
555 1908(bits.)N
749(Both)X
943(source)X
1194(and)X
1343(mask,)X
1573(if)X
1649(speci\256ed,)X
2006(must)X
2200(have)X
2388(depth)X
2606(one)X
2755(but)X
2890(can)X
3034(have)X
3222(any)X
3371(root.)X
3579(Otherwise,)X
555 2004(a)N
5 f
634(BadMatch)X
1 f
1068(error)X
1260(is)X
1341(generated.)X
1727(The)X
1886(mask)X
2094(argument)X
2449(de\256nes)X
2720(the)X
2850(shape)X
3072(of)X
3167(the)X
3297(cursor;)X
3563(that)X
3718(is,)X
3821(the)X
555 2100(one)N
704(bits)X
854(in)X
945(the)X
1075(mask)X
1283(de\256ne)X
1520(which)X
1757(source)X
2008(pixels)X
2241(will)X
2401(be)X
2506(displayed,)X
2888(where)X
3124(the)X
3254(mask)X
3462(has)X
3601(zero)X
3774(bits,)X
555 2196(the)N
685(corresponding)X
1210(bits)X
1360(of)X
1455(the)X
1585(source)X
1836(pixmap)X
2123(are)X
2252(ignored.)X
2587(If)X
2667(no)X
2777(mask)X
2985(is)X
3066(given,)X
3306(all)X
3417(pixels)X
3650(of)X
3745(the)X
555 2292(source)N
806(are)X
935(displayed.)X
1339(The)X
1498(mask,)X
1728(if)X
1804(present,)X
2102(must)X
2296(be)X
2401(the)X
2531(same)X
2734(size)X
2893(as)X
2988(the)X
3118(pixmap)X
3405(de\256ned)X
3686(by)X
3796(the)X
555 2388(source)N
806(argument.)X
1183(Otherwise,)X
1588(a)X
5 f
1667(BadMatch)X
1 f
2101(error)X
2293(is)X
2374(generated.)X
555 2512(The)N
714(components)X
1162(of)X
1257(the)X
1387(cursor)X
1628(may)X
1802(be)X
1907(transformed)X
2354(arbitrarily)X
2729(to)X
2820(meet)X
3014(display)X
3291 0.3295(limitations.)AX
3737(The)X
555 2608(pixmaps)N
876(can)X
1020(be)X
1125(freed)X
1327 0.3187(immediately)AX
1792(if)X
1868(no)X
1978(further)X
2239(explicit)X
2527(references)X
2909(to)X
3000(them)X
3199(are)X
3328(to)X
3419(be)X
3524(made.)X
3781(Sub-)X
555 2704(sequent)N
846(drawing)X
1156(in)X
1247(the)X
1377(source)X
1628(or)X
1723(mask)X
1931(pixmap)X
2218(has)X
2357(an)X
2462(unde\256ned)X
2831(effect)X
3053(on)X
3163(the)X
3293(cursor.)X
3578(The)X
3737(X)X
555 2800(server)N
791(might)X
1020(or)X
1115(might)X
1344(not)X
1479(make)X
1692(a)X
1753(copy)X
1946(of)X
2041(the)X
2171(pixmap.)X
555 2924(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCreatePixmapCursor)X
1 f
3168(are)X
5 f
3315(BadAlloc)X
1 f
3699(and)X
5 f
570 3020(BadPixmap)N
1 f
1035(.)X
555 3240(Use)N
5 f
731(XCreateGlyphCursor)X
1 f
1584(to)X
1675(create)X
1907(a)X
1968(cursor)X
2209(from)X
2402(font)X
2566(glyphs.)X
2867(This)X
3046(is)X
3127(a)X
3188(fundamental)X
3651(request)X
555 3336(used)N
738(by)X
5 f
866(XCreateFontCursor)X
1 f
1644(.)X
1710(The)X
1869(de\256nition)X
2230(for)X
5 f
2372(XCreateGlyphCursor)X
1 f
3225(is:)X
555 3460(Cursor)N
816(XCreateGlyphCursor)X
1575(\()X
2 f
1611(display)X
1 f
1866(,)X
2 f
1910(source_font)X
1 f
2333(,)X
2 f
2377(mask_font)X
1 f
2746(,)X
2 f
2790(source_char)X
1 f
3236(,)X
2 f
3280(mask_char)X
1 f
3672(,)X
2 f
3716(foreground_color)X
1 f
4349(,)X
2 f
555 3556(background_color)N
1 f
1212(\))X
687 3652(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3748(Font)N
2 f
871(source_font)X
1 f
1294(,)X
2 f
1338(mask_font)X
1 f
1707(;)X
687 3844(unsigned)N
1027(int)X
2 f
1143(source_char)X
1 f
1589(,)X
2 f
1633(mask_char)X
1 f
2025(;)X
687 3940(XColor)N
973(*)X
2 f
(foreground_color)S
1 f
1650(;)X
687 4036(XColor)N
973(*)X
2 f
(background_color)S
1 f
1674(;)X
2 f
555 4188(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4312(source_font)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(for)X
1889(the)X
2019(source)X
2270(glyph.)X
2 f
555 4436(mask_font)N
1 f
1131(Speci\256es)X
1471(the)X
1601(font)X
1765(for)X
1889(the)X
2019(mask)X
2227(glyph.)X
2472(You)X
2645(can)X
2789(pass)X
2962(the)X
3092(constant)X
5 f
3426(None)X
1 f
3651(.)X
2 f
555 4560(source_char)N
1 f
1131(Speci\256es)X
1471(the)X
1601(character)X
1945(glyph)X
2168(for)X
2292(the)X
2422(source.)X
2 f
555 4684(mask_char)N
1 f
1131(Speci\256es)X
1471(the)X
1601(glyph)X
1824(character)X
2168(for)X
2292(the)X
2422(mask.)X
2 f
555 4808(foreground_color)N
1 f
1131 4904(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(foreground)X
3493(of)X
3588(the)X
3718(source.)X
2 f
555 5028(background_color)N
1 f
1131 5124(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(background)X
3518(of)X
3613(the)X
3743(source.)X
555 5248(The)N
5 f
732(XCreateGlyphCursor)X
1 f
1585(function)X
1901(is)X
1982(similar)X
2250(to)X
5 f
2359(XCreatePixmapCursor)X
1 f
3275(and)X
3424(creates)X
3690(a)X
3751(cursor)X
555 5344(from)N
748(font)X
912(glyphs.)X
1213(For)X
5 f
1375(XCreateGlyphCursor)X
1 f
2206(,)X
2250(however,)X
2596(the)X
2726(source)X
2977(and)X
3126(mask)X
3334(bitmaps)X
3636(are)X
555 5440(obtained)N
881(from)X
1074(the)X
1204(speci\256ed)X
1539(font)X
1703(glyphs.)X
2004(The)X
2163(source_char)X
2609(must)X
2803(be)X
2908(a)X
2969(de\256ned)X
3250(glyph)X
3473(in)X
3564(source_font,)X
555 5536(and,)N
726(if)X
802(mask_font)X
1196(is)X
1277(given,)X
1517(mask_char)X
1920(must)X
2114(be)X
2219(a)X
2280(de\256ned)X
2561(glyph)X
2784(in)X
2875(mask_font.)X
3291(Otherwise,)X
3696(a)X
5 f
3775(Bad-)X
555 5632(Value)N
1 f
817(error)X
1009(is)X
1090(generated.)X
1498(The)X
1657(mask_font)X
2051(and)X
2200(character)X
2544(are)X
2673(optional.)X
3029(The)X
3188(origins)X
3455(of)X
3550(the)X
555 5728(source_char)N
1001(and)X
1150(mask_char)X
1553(\(if)X
1658(de\256ned\))X
1968(glyphs)X
2225(are)X
2354(positioned)X
2744(coincidently)X
3203(and)X
3352(de\256ne)X
3589(the)X
3719(hotspot.)X
555 5824(The)N
714(source_char)X
1160(and)X
1309(mask_char)X
1712(need)X
1900(not)X
2035(have)X
2223(the)X
2353(same)X
2556(bounding)X
2911(box)X
3065(metrics,)X
3369(and)X
3518(there)X
3716(is)X
3797(no)X
3 f
2217 6144(112)N

113 p
%%Page: 113 117
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(restriction)N
935(on)X
1045(the)X
1175(placement)X
1560(of)X
1655(the)X
1785(hotspot)X
2067(relative)X
2354(to)X
2445(the)X
2575(bounding)X
2930(boxes.)X
3179(If)X
3259(no)X
3369(mask_char)X
3772(is)X
555 768(given,)N
795(all)X
906(pixels)X
1139(of)X
1234(the)X
1364(source)X
1615(are)X
1744(displayed.)X
2148(You)X
2321(can)X
2465(free)X
2623(the)X
2753(fonts)X
2951 0.3187(immediately)AX
3416(by)X
3526(calling)X
5 f
570 864(XFreeFont)N
1 f
1024(if)X
1100(no)X
1210(further)X
1471(explicit)X
1759(references)X
2141(to)X
2232(them)X
2431(are)X
2560(to)X
2651(be)X
2756(made.)X
555 988(Note)N
748(that)X
903(schar)X
1110(and)X
1259(mchar)X
1501(are)X
1630(of)X
1725(type)X
1899(unsigned)X
2239(int,)X
2377(not)X
2512(of)X
2607(type)X
2781(XChar2b.)X
3169(For)X
3313(two-byte)X
3648(matrix)X
555 1084(fonts,)N
775(the)X
905(16-bit)X
1138(value)X
1351(should)X
1608(be)X
1713(formed)X
1989(with)X
2168(the)X
2298(byte1)X
2516(member)X
2827(in)X
2918(the)X
3048(most)X
3242(signi\256cant)X
3632(byte)X
3806(and)X
555 1180(the)N
685(byte2)X
903(member)X
1214(in)X
1305(the)X
1435(least)X
1619(signi\256cant)X
2009(byte.)X
555 1304(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XCreateGlyphCursor)X
1 f
3105(are)X
5 f
3252(BadAlloc)X
1 f
3614(,)X
5 f
3676(Bad-)X
555 1400(Font)N
1 f
747(,)X
791(and)X
5 f
958(BadValue)X
1 f
1355(.)X
3 f
555 1592(6.8.2.)N
797(Changing)X
1186(and)X
1350(Destroying)X
1782(Cursors)X
1 f
555 1716(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(change)X
2394(the)X
2524(cursor)X
2765(color,)X
2990(destroy)X
3271(the)X
3401(cursor,)X
3664(and)X
555 1812(determine)N
930(the)X
1060(cursor)X
1301(size.)X
555 2032(Use)N
5 f
731(XRecolorCursor)X
1 f
1392(to)X
1483(change)X
1754(the)X
1884(color)X
2087(of)X
2182(the)X
2312(speci\256ed)X
2647(cursor.)X
2932(The)X
3091(de\256nition)X
3452(for)X
3576(this)X
3726(func-)X
555 2128(tion)N
715(is:)X
555 2252(XRecolorCursor)N
1143(\()X
2 f
1179(display)X
1 f
1434(,)X
2 f
1478(cursor)X
1 f
1714(,)X
2 f
1758(foreground_color)X
1 f
2391(,)X
2 f
2435(background_color)X
1 f
3092(\))X
687 2348(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2444(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
687 2540(XColor)N
973(*)X
2 f
(foreground_color)S
1 f
1650(,)X
1694(*)X
2 f
(background_color)S
1 f
2395(;)X
2 f
555 2692(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2816(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor.)X
2 f
555 2940(foreground_color)N
1 f
1131 3036(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(foreground)X
3493(of)X
3588(the)X
3718(source.)X
2 f
555 3160(background_color)N
1 f
1131 3256(Speci\256es)N
1471(the)X
1601(red,)X
1757(green,)X
1996(and)X
2145(blue)X
2319(\(RGB\))X
2580(values)X
2827(for)X
2951(the)X
3081(background)X
3518(of)X
3613(the)X
3743(source.)X
555 3380(The)N
5 f
732(XRecolorCursor)X
1 f
1393(function)X
1709(changes)X
2014(the)X
2144(color)X
2347(of)X
2442(the)X
2572(speci\256ed)X
2907(cursor,)X
3170(and,)X
3341(if)X
3417(the)X
3547(cursor)X
3788(is)X
555 3476(being)N
773(displayed)X
1133(on)X
1243(a)X
1304(screen,)X
1572(the)X
1702(change)X
1973(is)X
2054(visible)X
2312 0.2898(immediately.)AX
555 3600(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XRecolorCursor)X
1 f
2463(is)X
5 f
2562(BadCursor)X
1 f
2997(.)X
555 3820(Use)N
5 f
731(XFreeCursor)X
1 f
1271(to)X
1362(free)X
1520(\(destroy\))X
1859(the)X
1989(speci\256ed)X
2324(cursor.)X
2609(The)X
2768(de\256nition)X
3129(for)X
3253(this)X
3403(function)X
3719(is:)X
555 3944(XFreeCursor)N
1020(\()X
2 f
1056(display)X
1 f
1311(,)X
2 f
1355(cursor)X
1 f
1591(\))X
687 4040(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4136(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
2 f
555 4288(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4412(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor.)X
555 4536(The)N
5 f
732(XFreeCursor)X
1 f
1272(function)X
1588(deletes)X
1855(the)X
1985(association)X
2399(between)X
2714(the)X
2844(cursor)X
3085(resource)X
3404(ID)X
3518(and)X
3667(the)X
555 4632(speci\256ed)N
890(cursor.)X
1175(The)X
1334(cursor)X
1575(storage)X
1851(is)X
1932(freed)X
2134(when)X
2346(no)X
2456(other)X
2659(resource)X
2978(references)X
3360(it.)X
3476(The)X
3635(speci\256ed)X
555 4728(cursor)N
796(ID)X
910(should)X
1167(not)X
1302(be)X
1407(referred)X
1706(to)X
1797(again)X
2010(or)X
2105(an)X
2210(error)X
2402(will)X
2562(be)X
2667(generated.)X
555 4852(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XFreeCursor)X
1 f
2342(is)X
5 f
2441(BadCursor)X
1 f
2876(.)X
555 5072(Use)N
5 f
731(XQueryBestCursor)X
1 f
1507(to)X
1598(determine)X
1973(useful)X
2210(cursor)X
2451(sizes.)X
2688(The)X
2847(de\256nition)X
3208(for)X
3332(this)X
3482(function)X
3798(is:)X
555 5196(Status)N
793(XQueryBestCursor)X
1478(\()X
2 f
1514(display)X
1 f
1769(,)X
2 f
1813(d)X
1 f
(,)S
2 f
1901(width)X
1 f
2105(,)X
2 f
2149(height)X
1 f
2377(,)X
2 f
2421(width_return)X
1 f
2889(,)X
2 f
2933(height_return)X
1 f
3425(\))X
687 5292(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5388(Drawable)N
2 f
1050(d)X
1 f
1101(;)X
687 5484(unsigned)N
1027(int)X
2 f
1143(width)X
1 f
1347(,)X
2 f
1391(height)X
1 f
1619(;)X
687 5580(unsigned)N
1027(int)X
1143(*)X
2 f
(width_return)S
1 f
1655(,)X
1699(*)X
2 f
(height_return)S
1 f
2235(;)X
2 f
555 5732(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2217 6144(113)N

114 p
%%Page: 114 118
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(d)N
1 f
1131(Speci\256es)X
1471(the)X
1601(drawable.)X
1967(The)X
2126(drawable)X
2470(indicates)X
2806(the)X
2936(desired)X
3212(screen.)X
2 f
555 796(width)N
555 892(height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height)X
2167(of)X
2262(the)X
2392(cursor)X
2633(that)X
2788(you)X
2942(want)X
3135(the)X
3265(size)X
3424(information)X
3863(for.)X
2 f
555 1016(width_return)N
555 1112(height_return)N
1 f
1131(Returns)X
1427(the)X
1557(best)X
1721(width)X
1944(and)X
2093(height)X
2336(\(that)X
2520(is,)X
2623(closest)X
2885(to)X
2976(the)X
3106(speci\256ed)X
3441(width)X
3664(and)X
1131 1208(height\).)N
555 1332(Some)N
778(displays)X
1089(allow)X
1307(larger)X
1534(cursors)X
1809(than)X
1983(other)X
2186(displays.)X
5 f
2562(XQueryBestCursor)X
1 f
3338(provides)X
3663(a)X
3724(way)X
3892(to)X
555 1428(\256nd)N
714(out)X
849(what)X
1042(size)X
1201(cursors)X
1476(are)X
1605(actually)X
1907(possible)X
2218(on)X
2328(the)X
2458(display.)X
2779(This)X
2958(function)X
3274(returns)X
3540(the)X
3670(largest)X
555 1524(size)N
714(that)X
869(can)X
1013(be)X
1118(displayed.)X
1522(For)X
1666(a)X
1727(cursor)X
1968(shape,)X
2212(it)X
2284(returns)X
2550(a)X
2611(bitmap)X
2879(shape)X
3101(acceptable)X
3495(for)X
5 f
570 1620(XCreatePixmapCursor)N
1 f
1464(.)X
1530(Applications)X
2003(should)X
2260(be)X
2365(prepared)X
2694(to)X
2785(use)X
2924(smaller)X
3206(cursors)X
3481(on)X
3591(displays)X
555 1716(which)N
792(cannot)X
1049(support)X
1335(large)X
1533(ones.)X
555 1840(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XQueryBestCursor)X
1 f
2578(is)X
5 f
2677(BadDrawable)X
1 f
3219(.)X
3 f
555 2032(6.8.3.)N
797(De\256ning)X
1137(the)X
1276(Cursor)X
1 f
555 2156(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(de\256ne)X
2360(or)X
2455(unde\256ne)X
2780(the)X
2910(cursor)X
3151(in)X
3242(a)X
3303(window.)X
555 2376(Use)N
5 f
731(XDe\256neCursor)X
1 f
1344(to)X
1435(de\256ne)X
1672(which)X
1909(cursor)X
2150(will)X
2310(be)X
2415(used)X
2598(in)X
2689(a)X
2750(window.)X
3099(The)X
3258(de\256nition)X
3619(for)X
3743(this)X
555 2472(function)N
871(is:)X
555 2596(XDe\256neCursor)N
1098(\()X
2 f
1134(display)X
1 f
1389(,)X
2 f
1433(w)X
1 f
1499(,)X
2 f
1543(cursor)X
1 f
1779(\))X
687 2692(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2788(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 2884(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
2 f
555 3036(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3160(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 3284(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor)X
1842(that)X
1997(is)X
2078(to)X
2169(be)X
2274(displayed)X
2634(when)X
2846(the)X
2976(pointer)X
3248(is)X
3329(in)X
3420(the)X
3550(speci\256ed)X
1131 3380(window.)N
1480(You)X
1653(can)X
1797(pass)X
5 f
1988(None)X
1 f
2235(if)X
2311(no)X
2421(cursor)X
2662(is)X
2743(to)X
2834(be)X
2939(displayed.)X
555 3504(If)N
635(a)X
696(cursor)X
937(is)X
1018(set,)X
1160(it)X
1232(will)X
1392(be)X
1497(used)X
1680(when)X
1892(the)X
2022(pointer)X
2294(is)X
2375(in)X
2466(the)X
2596(window.)X
555 3628(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XDe\256neCursor)X
1 f
2449(are)X
5 f
2596(BadWindow)X
1 f
3080(,)X
5 f
3142(BadAlloc)X
1 f
3504(,)X
3548(and)X
5 f
3715(Bad-)X
555 3724(Cursor)N
1 f
833(.)X
555 3944(Use)N
5 f
731(XUnde\256neCursor)X
1 f
1442(to)X
1533(unde\256ne)X
1858(the)X
1988(mouse)X
2240(cursor)X
2481(in)X
2572(the)X
2702(speci\256ed)X
3037(window.)X
3386(The)X
3545(de\256nition)X
3906(for)X
555 4040(this)N
705(function)X
1021(is:)X
555 4164(XUnde\256neCursor)N
1186(\()X
2 f
1222(display)X
1 f
1477(,)X
2 f
1521(w)X
1 f
1587(\))X
687 4260(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4356(Window)N
2 f
1012(w)X
1 f
1078(;)X
2 f
555 4508(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4632(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
555 4756(The)N
5 f
732(XUnde\256neCursor)X
1 f
1443(undoes)X
1714(the)X
1844(effect)X
2066(of)X
2161(a)X
2222(previous)X
5 f
2565(XDe\256neCursor)X
1 f
3178(for)X
3302(this)X
3452(window.)X
3801(When)X
555 4852(the)N
685(mouse)X
937(is)X
1018(in)X
1109(the)X
1239(window,)X
1566(the)X
1696(parent's)X
2001(cursor)X
2242(will)X
2402(now)X
2575(be)X
2680(used.)X
2907(On)X
3036(the)X
3166(root)X
3330(window,)X
3657(with)X
3836(no)X
555 4948(cursor)N
796(speci\256ed,)X
1153(the)X
1283(default)X
1550(cursor)X
1791(is)X
1872(restored.)X
555 5072(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUnde\256neCursor)X
1 f
2513(is)X
5 f
2612(BadWindow)X
1 f
3096(.)X
3 f
2217 6144(114)N

115 p
%%Page: 115 119
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(7)X
1696 1056(Window)N
2066(Manager)X
2464(Functions)X
1 f
11 s
555 1372(Once)N
762(you)X
916(have)X
1104(connected)X
1483(the)X
1613(display)X
1890(to)X
1981(the)X
2111(X)X
2196(server)X
2432(and)X
2581(want)X
2774(to)X
2865(create)X
3097(a)X
3158(window)X
3463(manager)X
3788(appli-)X
555 1468(cation,)N
815(you)X
969(can)X
1113(use)X
1252(the)X
1382(Xlib)X
1561(window)X
1866(manager)X
2191(functions)X
2541(to:)X
10 f
555 1592(g)N
1 f
775(Change)X
1066(the)X
1196(parent)X
1438(of)X
1533(a)X
1594(window)X
10 f
555 1716(g)N
1 f
775(Control)X
1067(the)X
1197(lifetime)X
1495(of)X
1590(a)X
1651(window)X
10 f
555 1840(g)N
1 f
775(Manipulate)X
1199(color)X
1402(maps)X
10 f
555 1964(g)N
1 f
775(Manipulate)X
1199(the)X
1329(pointer)X
10 f
555 2088(g)N
1 f
775(Manipulate)X
1199(the)X
1329(keyboard)X
10 f
555 2212(g)N
1 f
775(Grab)X
972(the)X
1102(server)X
10 f
555 2336(g)N
1 f
775(Control)X
1067(processing)X
10 f
555 2460(g)N
1 f
775(Manipulate)X
1199(the)X
1329(keyboard)X
1678(encoding)X
10 f
555 2584(g)N
1 f
775(Manipulate)X
1199(the)X
1329(screen)X
1575(saver)X
10 f
555 2708(g)N
1 f
775(Control)X
1067(host)X
1236(access)X
2197 2884(Note)N
775 3020(The)N
934(functions)X
1284(in)X
1375(this)X
1525(chapter)X
1806(are)X
1935(most)X
2129(often)X
2332(used)X
2515(by)X
2625(window)X
2930(managers,)X
3311(though)X
3578(it)X
3650(can)X
775 3116(be)N
880(dif\256cult)X
1182(to)X
1273(de\256nitively)X
1698(categorize)X
2082(functions)X
2432(as)X
2527(application)X
2942(only)X
3121(or)X
3216(window)X
775 3212(manager)N
1100(only.)X
1323(It)X
1399(is)X
1480(not)X
1615(expected)X
1950(that)X
2105(these)X
2308(functions)X
2658(will)X
2818(be)X
2923(used)X
3106(by)X
3216(most)X
3410(normal)X
775 3308(application)N
1190(programs.)X
3 f
555 3580(7.1.)N
731(Changing)X
1120(the)X
1259(Parent)X
1535(of)X
1630(a)X
1696(Window)X
1 f
555 3704(Use)N
5 f
731(XReparentWindow)X
1 f
1501(to)X
1592(change)X
1863(a)X
1924(window's)X
2292(parent)X
2534(within)X
2782(a)X
2843(single)X
3076(screen.)X
3366(There)X
3593(is)X
3674(no)X
3784(way)X
555 3800(to)N
646(move)X
864(a)X
925(window)X
1230(between)X
1545(screens.)X
1869(The)X
2028(de\256nition)X
2389(for)X
2513(this)X
2663(function)X
2979(is:)X
555 3924(XReparentWindow)N
1246(\()X
2 f
1282(display)X
1 f
1537(,)X
2 f
1581(w)X
1 f
1647(,)X
2 f
1691(parent)X
1 f
1928(,)X
2 f
1972(x)X
1 f
2018(,)X
2 f
2062(y)X
1 f
2108(\))X
687 4020(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4116(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 4212(Window)N
2 f
1012(parent)X
1 f
1249(;)X
687 4308(int)N
2 f
803(x)X
1 f
849(,)X
2 f
893(y)X
1 f
939(;)X
2 f
555 4460(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4584(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2 f
555 4708(parent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(parent)X
1843(window)X
2148(ID.)X
2 f
555 4832(x)N
555 4928(y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(of)X
2356(the)X
2486(position)X
2793(in)X
2884(the)X
3014(new)X
3182(parent)X
3424(window)X
3729(of)X
3824(the)X
1131 5024(speci\256ed)N
1466(window.)X
555 5148(The)N
5 f
732(XReparentWindow)X
1 f
1502(function)X
1818(reparents)X
2162(the)X
2292(speci\256ed)X
2627(window)X
2932(by)X
3042(inserting)X
3373(it)X
3445(as)X
3540(the)X
3670(child)X
3869(of)X
555 5244(the)N
685(speci\256ed)X
1020(parent.)X
1306(If)X
1386(the)X
1516(window)X
1821(is)X
1902(mapped,)X
5 f
2243(XReparentWindow)X
1 f
3013 0.2604(automatically)AX
3517(performs)X
3856(an)X
5 f
570 5340(XUnmapWindow)N
1 f
1261(request)X
1537(on)X
1647(the)X
1777(speci\256ed)X
2112(window.)X
2461(The)X
2620(function)X
2936(then)X
3110(removes)X
3430(the)X
3560(speci\256ed)X
555 5436(window)N
860(from)X
1053(its)X
1159(current)X
1430(position)X
1737(in)X
1828(the)X
1958(hierarchy)X
2312(and)X
2461(inserts)X
2713(it)X
2785(as)X
2880(the)X
3010(child)X
3209(of)X
3304(the)X
3434(speci\256ed)X
3769(parent.)X
555 5532(The)N
714(window)X
1019(is)X
1100(placed)X
1352(on)X
1462(top)X
1597(in)X
1688(the)X
1818(stacking)X
2134(order)X
2341(with)X
2520(respect)X
2791(to)X
2882(sibling)X
3145(windows.)X
555 5656(After)N
762(reparenting)X
1185(the)X
1315(speci\256ed)X
1650(window,)X
5 f
1995(XReparentWindow)X
1 f
2765(causes)X
3016(the)X
3146(X)X
3231(server)X
3467(to)X
3558(generate)X
3878(a)X
5 f
570 5752(ReparentNotify)N
1 f
1194(event.)X
1451(The)X
1610(override_redirect)X
2238(member)X
2549(of)X
2644(the)X
2774(structure)X
3104(returned)X
3419(by)X
3529(this)X
3679(event)X
3892(is)X
555 5848(set)N
675(to)X
766(either)X
5 f
1007(True)X
1 f
1225(or)X
5 f
1338(False)X
1 f
1568(.)X
1634(Window)X
1959(manager)X
2284(clients)X
2537(normally)X
2878(should)X
3135(ignore)X
3382(this)X
3532(event)X
3745(if)X
3821(this)X
3 f
2217 6144(115)N

116 p
%%Page: 116 120
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(member)N
866(is)X
947(set)X
1067(to)X
5 f
1176(True)X
1 f
1372(.)X
1438(See)X
1587(Chapter)X
1888(8)X
1954(for)X
2078(more)X
2281(information)X
2720(on)X
5 f
2848(ReparentNotify)X
1 f
3472(event)X
3685(process-)X
555 768(ing.)N
734(Finally,)X
1029(if)X
1105(the)X
1235(speci\256ed)X
1570(window)X
1875(was)X
2033(originally)X
2399(mapped,)X
2722(this)X
2872(function)X
3188 0.2604(automatically)AX
3692(performs)X
555 864(a)N
5 f
634(XMapWindow)X
1 f
1213(request)X
1489(on)X
1599(it.)X
555 988(The)N
714(X)X
799(server)X
1035(performs)X
1374(normal)X
1646(exposure)X
1985(processing)X
2383(on)X
2493(formerly)X
2823(obscured)X
3162(windows.)X
3545(The)X
3704(X)X
3789(server)X
555 1084(might)N
784(not)X
919(generate)X
1239(exposure)X
1578(events)X
1825(for)X
1949(regions)X
2230(from)X
2423(the)X
2553(initial)X
5 f
2801(XUnmapWindow)X
1 f
3492(request)X
3768(that)X
555 1180(are)N
684 0.3187(immediately)AX
1149(obscured)X
1488(by)X
1598(the)X
1728(\256nal)X
5 f
1925(XMapWindow)X
1 f
2504(request.)X
2824(A)X
5 f
2927(BadMatch)X
1 f
3361(error)X
3553(is)X
3634(generated:)X
10 f
555 1304(g)N
1 f
775(If)X
855(the)X
985(new)X
1153(parent)X
1395(window)X
1700(is)X
1781(not)X
1916(on)X
2026(the)X
2156(same)X
2359(screen)X
2605(as)X
2700(the)X
2830(old)X
2965(parent)X
3207(window)X
10 f
555 1428(g)N
1 f
775(If)X
855(the)X
985(new)X
1153(parent)X
1395(window)X
1700(is)X
1781(the)X
1911(speci\256ed)X
2246(window)X
2551(or)X
2646(an)X
2751(inferior)X
3037(of)X
3132(the)X
3262(speci\256ed)X
3597(window)X
10 f
555 1552(g)N
1 f
775(If)X
855(the)X
985(speci\256ed)X
1320(window)X
1625(has)X
1764(a)X
5 f
1843(ParentRelative)X
1 f
2457(background)X
2894(and)X
3043(if)X
3119(the)X
3249(new)X
3417(parent)X
3659(window)X
775 1648(is)N
856(not)X
991(the)X
1121(same)X
1324(depth)X
1542(as)X
1637(the)X
1767(speci\256ed)X
2102(window.)X
555 1772(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XReparentWindow)X
1 f
2775(is)X
5 f
2874(BadWindow)X
1 f
3358(.)X
3 f
555 1964(7.2.)N
731(Controlling)X
1189(the)X
1328(Lifetime)X
1668(of)X
1763(a)X
1829(Window)X
1 f
555 2088(The)N
714(save-set)X
1019(of)X
1114(a)X
1175(client)X
1394(is)X
1475(a)X
1536(list)X
1667(of)X
1762(other)X
1965(client's)X
2247(windows)X
2586(which,)X
2845(if)X
2921(they)X
3095(are)X
3224(inferiors)X
3544(of)X
3639(one)X
3788(of)X
3883(the)X
555 2184(client's)N
837(windows)X
1176(at)X
1262(connection)X
1671(close,)X
1896(should)X
2153(not)X
2288(be)X
2393(destroyed)X
2757(and)X
2906(should)X
3163(be)X
3268(remapped)X
3637(if)X
3713(they)X
3887(are)X
555 2280(unmapped.)N
988(To)X
1108(allow)X
1326(an)X
1431(application's)X
1909(window)X
2214(to)X
2305(survive)X
2586(when)X
2798(a)X
2859(window)X
3164(manager)X
3489(that)X
3644(has)X
555 2376(reparented)N
948(a)X
1009(window)X
1314(fails,)X
1510(Xlib)X
1689(provides)X
2014(the)X
2144(save-set)X
2449(functions)X
2799(with)X
2978(which)X
3215(you)X
3369(can)X
3513(change)X
3784(a)X
555 2472(client's)N
837(save-set,)X
1164(add)X
1313(a)X
1374(subwindow)X
1801(to)X
1892(a)X
1953(client's)X
2235(save-set,)X
2562(or)X
2657(remove)X
2943(a)X
3004(subwindow)X
3431(from)X
3624(a)X
3685(client's)X
555 2568(save-set.)N
555 2692(The)N
714(functions)X
1064(described)X
1423(in)X
1514(this)X
1664(section)X
1936(are)X
2065(used)X
2248(to)X
2339(control)X
2611(the)X
2741(longevity)X
3097(of)X
3192(subwindows)X
3653(that)X
3808(are)X
555 2788(normally)N
896(destroyed)X
1260(when)X
1472(the)X
1602(parent)X
1844(is)X
1925(destroyed.)X
2333(For)X
2477(example,)X
2820(a)X
2881(window)X
3186(manager)X
3511(that)X
3666(wants)X
3893(to)X
555 2884(add)N
704(decoration)X
1098(to)X
1189(a)X
1250(window)X
1555(by)X
1665(adding)X
1927(a)X
1988(frame)X
2215(might)X
2444(reparent)X
2754(an)X
2859(application's)X
3337(window.)X
3664(When)X
3896(the)X
555 2980(frame)N
782(is)X
863(destroyed,)X
1249(the)X
1379(application's)X
1857(window)X
2162(should)X
2419(not)X
2554(be)X
2659(destroyed,)X
3045(but)X
3180(returned)X
3495(to)X
3586(its)X
3692(previous)X
555 3076(place)N
763(in)X
854(the)X
984(window)X
1289(hierarchy.)X
555 3200(Windows)N
914(are)X
1043(removed)X
1373 0.2604(automatically)AX
1877(from)X
2070(the)X
2200(save-set)X
2505(by)X
2615(the)X
2745(X)X
2830(server)X
3066(when)X
3278(they)X
3452(are)X
3581(destroyed.)X
555 3296(For)N
699(each)X
882(window)X
1187(in)X
1278(the)X
1408(client's)X
1690(save-set,)X
2017(if)X
2093(the)X
2223(window)X
2528(is)X
2609(an)X
2714(inferior)X
3000(of)X
3095(a)X
3156(window)X
3461(created)X
3737(by)X
3847(the)X
555 3392(client,)N
796(the)X
926(save-set)X
1231(window)X
1536(is)X
1617(reparented)X
2010(to)X
2101(the)X
2231(closest)X
2493(ancestor)X
2808(such)X
2991(that)X
3146(the)X
3276(save-set)X
3581(window)X
3886(is)X
555 3488(not)N
690(an)X
795(inferior)X
1081(of)X
1176(a)X
1237(window)X
1542(created)X
1818(by)X
1928(the)X
2058(client.)X
2299(If)X
2379(the)X
2509(save-set)X
2814(window)X
3119(is)X
3200(unmapped,)X
3611(a)X
5 f
570 3584(MapWindow)N
1 f
1090(request)X
1366(is)X
1447(performed)X
1835(on)X
1945(it.)X
2039(After)X
2246(save-set)X
2551(processing,)X
2971(all)X
3082(windows)X
3421(created)X
3697(by)X
3807(the)X
555 3680(client)N
774(are)X
903(destroyed.)X
1289(For)X
1433(each)X
1616(nonwindow)X
2053(resource)X
2372(created)X
2648(by)X
2758(the)X
2888(client,)X
3129(the)X
3259(appropriate)X
3682(Free)X
555 3776(request)N
831(is)X
912(performed.)X
1322(All)X
1457(colors)X
1694(and)X
1843(color)X
2046(map)X
2220(entries)X
2477(allocated)X
2818(by)X
2928(the)X
3058(client)X
3277(are)X
3406(freed.)X
555 3996(Use)N
5 f
731(XChangeSaveSet)X
1 f
1469(to)X
1560(add)X
1709(or)X
1804(remove)X
2090(a)X
2151(window)X
2456(from)X
2649(the)X
2779(client's)X
3061(save-set.)X
3388(The)X
3547(de\256nition)X
3908(for)X
555 4092(this)N
705(function)X
1021(is:)X
555 4216(XChangeSaveSet)N
1178(\()X
2 f
1214(display)X
1 f
1469(,)X
2 f
1513(w)X
1 f
1579(,)X
2 f
1623(change_mode)X
1 f
2118(\))X
687 4312(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4408(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 4504(int)N
2 f
803(change_mode)X
1 f
1298(;)X
2 f
555 4656(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4780(w_add)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(whose)X
2796(children)X
3107(you)X
3261(want)X
3454(to)X
3545(add)X
3694(to)X
3785(the)X
1131 4876(client's)N
1413(save-set.)X
2 f
555 5000(change_mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mode.)X
1863(You)X
2036(can)X
2180(pass)X
2353(one)X
2502(of)X
2597(the)X
2727(constants)X
5 f
3095(SetModeInsert)X
1 f
3706(or)X
5 f
3819(Set-)X
1131 5096(ModeDelete)N
1 f
1620(.)X
1686(If)X
5 f
1784(SetModeInsert)X
1 f
2373(,)X
5 f
2435(XChangeSaveSet)X
1 f
3173(adds)X
3356(the)X
3486(window)X
3791(to)X
3882(this)X
1131 5192(client's)N
1413(save-set.)X
1740(If)X
5 f
1838(SetModeDelete)X
1 f
2460(,)X
5 f
2522(XChangeSaveSet)X
1 f
3260(deletes)X
3527(the)X
3657(window)X
1131 5288(from)N
1324(this)X
1474(client's)X
1756(save-set.)X
555 5412(Depending)N
963(on)X
1073(the)X
1203(constant)X
1519(you)X
1673(passed)X
1929(to)X
2020(the)X
2150(change_mode)X
2661(argument,)X
3038(the)X
5 f
3186(XChangeSaveSet)X
1 f
555 5508(function)N
871(either)X
1094(adds)X
1277(or)X
1372(removes)X
1692(a)X
1753(subwindow)X
2180(from)X
2373(the)X
2503(client's)X
2785(save-set.)X
3112(The)X
3271(speci\256ed)X
3606(window)X
555 5604(must)N
749(have)X
937(been)X
1125(created)X
1401(by)X
1511(some)X
1719(other)X
1922(client.)X
2163(Otherwise,)X
2568(a)X
5 f
2647(BadMatch)X
1 f
3081(error)X
3273(is)X
3354(generated.)X
3762(See)X
555 5700(Section)N
842(2.6)X
974(for)X
1098(information)X
1537(on)X
1647(what)X
1840(happens)X
2150(to)X
2241(the)X
2371(save-set)X
2676(during)X
2928(connection)X
3337(close.)X
3584(The)X
3743(X)X
555 5796(server)N
791 0.2604(automatically)AX
1295(removes)X
1615(windows)X
1954(from)X
2147(the)X
2277(save-set)X
2582(when)X
2794(they)X
2968(are)X
3097(destroyed.)X
3 f
2217 6144(116)N

117 p
%%Page: 117 121
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XChangeSaveSet)X
1 f
2990(are)X
5 f
3137(BadWindow)X
1 f
3621(,)X
5 f
3683(Bad-)X
555 768(Match)N
1 f
810(,)X
854(and)X
5 f
1021(BadValue)X
1 f
1418(.)X
555 988(Use)N
5 f
731(XAddToSaveSet)X
1 f
1421(to)X
1512(add)X
1661(a)X
1722(window)X
2027(to)X
2118(the)X
2248(client's)X
2530(save-set.)X
2879(The)X
3038(de\256nition)X
3399(for)X
3523(this)X
3673(function)X
555 1084(is:)N
555 1208(XAddToSaveSet)N
1158(\()X
2 f
1194(display)X
1 f
1449(,)X
2 f
1493(w_add)X
1 f
1735(\))X
687 1304(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1400(Window)N
2 f
1012(w_add)X
1 f
1254(;)X
2 f
555 1552(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1676(w_add)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(whose)X
2796(children)X
3107(you)X
3261(want)X
3454(to)X
3545(add)X
3694(to)X
3785(the)X
1131 1772(client's)N
1413(save-set.)X
555 1896(The)N
5 f
732(XAddToSaveSet)X
1 f
1422(function)X
1738(adds)X
1921(the)X
2051(children)X
2362(of)X
2457(the)X
2587(speci\256ed)X
2922(window)X
3227(to)X
3318(the)X
3448(client's)X
3730(save-)X
555 1992(set.)N
719(The)X
878(speci\256ed)X
1213(window)X
1518(must)X
1712(have)X
1900(been)X
2088(created)X
2364(by)X
2474(some)X
2682(other)X
2885(client.)X
3126(Otherwise,)X
3531(a)X
5 f
3610(Bad-)X
555 2088(Match)N
1 f
832(error)X
1024(is)X
1105(generated.)X
1513(See)X
1662(Section)X
1949(2.6)X
2081(for)X
2205(information)X
2644(on)X
2754(what)X
2947(happens)X
3257(to)X
3348(the)X
3478(save-set)X
3783(dur-)X
555 2184(ing)N
690(connection)X
1099(close.)X
1346(The)X
1505(X)X
1590(server)X
1826 0.2604(automatically)AX
2330(removes)X
2650(windows)X
2989(from)X
3182(the)X
3312(save-set)X
3617(when)X
3829(they)X
555 2280(are)N
684(destroyed.)X
555 2404(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XAddToSaveSet)X
1 f
2942(are)X
5 f
3089(BadWindow)X
1 f
3595(and)X
5 f
3762(Bad-)X
555 2500(Match)N
1 f
810(.)X
555 2720(Use)N
5 f
731(XRemoveFromSaveSet)X
1 f
1693(to)X
1784(remove)X
2070(a)X
2131(window)X
2436(from)X
2629(the)X
2759(client's)X
3041(save-set.)X
3390(The)X
3549(de\256nition)X
555 2816(for)N
679(this)X
829(function)X
1145(is:)X
555 2940(XRemoveFromSaveSet)N
1394(\()X
2 f
1430(display)X
1 f
1685(,)X
2 f
1729(w_remove)X
1 f
2097(\))X
687 3036(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3132(Window)N
2 f
1012(w_remove)X
1 f
1380(;)X
2 f
555 3284(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3408(w_remove)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(whose)X
2796(children)X
3107(you)X
3261(want)X
3454(to)X
3545(remove)X
3831(frem)X
1131 3504(the)N
1261(client's)X
1543(save-set.)X
555 3628(The)N
5 f
732(XRemoveFromSaveSet)X
1 f
1694(function)X
2010(removes)X
2330(the)X
2460(children)X
2771(of)X
2866(the)X
2996(speci\256ed)X
3331(window)X
3636(from)X
3829(the)X
555 3724(client's)N
837(save-set.)X
1186(The)X
1345(speci\256ed)X
1680(window)X
1985(must)X
2179(have)X
2367(been)X
2555(created)X
2831(by)X
2941(some)X
3149(other)X
3352(client.)X
3593(Otherwise,)X
555 3820(a)N
5 f
634(BadMatch)X
1 f
1068(error)X
1260(is)X
1341(generated.)X
1749(See)X
1898(Section)X
2185(2.6)X
2317(for)X
2441(information)X
2880(on)X
2990(what)X
3183(happens)X
3493(to)X
3584(the)X
3714(save-set)X
555 3916(during)N
807(connection)X
1216(close.)X
1463(The)X
1622(X)X
1707(server)X
1943 0.2604(automatically)AX
2447(removes)X
2767(windows)X
3106(from)X
3299(the)X
3429(save-set)X
3734(when)X
555 4012(they)N
729(are)X
858(destroyed.)X
555 4136(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XRemoveFromSaveSet)X
1 f
3214(are)X
5 f
3361(BadWindow)X
1 f
3867(and)X
5 f
570 4232(BadMatch)N
1 f
982(.)X
3 f
555 4424(7.3.)N
731(Manipulating)X
1268(Color)X
1505(Maps)X
1 f
555 4548(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(install)X
2362(a)X
2423(color)X
2626(map,)X
2822(uninstall)X
3149(a)X
3210(color)X
3413(map,)X
3609(and)X
3758(obtain)X
555 4644(a)N
616(list)X
747(of)X
842(installed)X
1164(color)X
1367(maps.)X
555 4768(Window)N
880(manager)X
1205(applications)X
1654(usually)X
1931(install)X
2170(and)X
2319(uninstall)X
2646(color)X
2849(maps,)X
3079(and,)X
3250(thus,)X
3441(these)X
3644(tasks)X
555 4864(should)N
812(not)X
947(be)X
1052(performed)X
1440(by)X
1550(normal)X
1822(client)X
2041(applications.)X
2534(The)X
2693(X)X
2778(server)X
3014(always)X
3280(maintains)X
3646(a)X
3707(subset)X
555 4960(of)N
650(the)X
780(installed)X
1102(color)X
1305(maps)X
1513(in)X
1604(an)X
1709(ordered)X
1999(list)X
2130(called)X
2363(the)X
2493(``required)X
2866(list.'')X
3077(The)X
3236(length)X
3479(of)X
3574(the)X
3704(required)X
555 5056(list)N
686(is)X
767(at)X
853(most)X
1047(the)X
1177(minimum)X
1544(installed)X
1888(maps)X
2096(speci\256ed)X
2431(for)X
2555(the)X
2685(screen)X
2931(when)X
3143(the)X
3273(connection)X
3682(is)X
555 5152(opened)N
831(to)X
922(the)X
1052(server.)X
1332(Initially,)X
1657(only)X
1836(the)X
1966(default)X
2233(color)X
2436(map)X
2610(for)X
2734(a)X
2795(screen)X
3041(is)X
3122(installed,)X
3466(but)X
3601(it)X
3673(is)X
3754(not)X
3889(in)X
555 5248(the)N
685(required)X
1000(list.)X
1175(The)X
1334(X)X
1419(server)X
1655(maintains)X
2021(the)X
2151(required)X
2466(list)X
2597(as)X
2692(follows:)X
10 f
555 5372(g)N
1 f
775(If)X
855(you)X
1009(pass)X
1182(a)X
1243(color)X
1446(map)X
1620(resource)X
1939(ID)X
2053(to)X
2144(the)X
2274(cmap)X
2487(argument,)X
5 f
2882(XInstallColormap)X
1 f
3588(adds)X
3771(the)X
775 5468(color)N
978(map)X
1152(to)X
1243(the)X
1373(top)X
1508(of)X
1603(the)X
1733(list)X
1864(and)X
2013(truncates)X
2353(a)X
2414(color)X
2617(map)X
2791(at)X
2877(the)X
3007(bottom)X
3280(of)X
3375(the)X
3505(list,)X
3658(if)X
3734(neces-)X
775 5564(sary,)N
965(so)X
1065(as)X
1160(not)X
1295(to)X
1386(exceed)X
1652(the)X
1782(maximum)X
2163(length)X
2406(of)X
2501(the)X
2631(list.)X
10 f
555 5688(g)N
1 f
775(If)X
855(you)X
1009(pass)X
1182(a)X
1243(color)X
1446(map)X
1620(resource)X
1939(ID)X
2053(to)X
2144(the)X
2274(cmap)X
2487(argument)X
2842(and)X
2991(that)X
3146(color)X
3349(map)X
3523(is)X
3604(in)X
3695(the)X
775 5784(required)N
1090(list,)X
5 f
1261(XUninstallColormap)X
1 f
2073(removes)X
2393(the)X
2523(color)X
2726(map)X
2900(from)X
3093(the)X
3223(required)X
3538(list.)X
3713(A)X
3 f
2217 6144(117)N

118 p
%%Page: 118 122
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
775 672(colormap)N
1130(is)X
1211(not)X
1346(added)X
1578(to)X
1669(the)X
1799(required)X
2114(list)X
2245(when)X
2457(it)X
2529(is)X
2610(installed)X
2932 0.4306(implicitly)AX
3300(by)X
3410(the)X
3540(server,)X
3798(and)X
775 768(the)N
905(X)X
990(server)X
1226(cannot)X
1483 0.4306(implicitly)AX
1851(uninstall)X
2178(a)X
2239(colormap)X
2594(that)X
2749(is)X
2830(in)X
2921(the)X
3051(required)X
3366(list.)X
555 988(Use)N
5 f
731(XInstallColormap)X
1 f
1437(to)X
1528(install)X
1767(a)X
1828(color)X
2031(map.)X
2227(The)X
2386(de\256nition)X
2747(for)X
2871(this)X
3021(function)X
3337(is:)X
555 1112(XInstallColormap)N
1199(\()X
2 f
1235(display)X
1 f
1490(,)X
2 f
1534(cmap)X
1 f
1731(\))X
687 1208(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1304(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
2 f
555 1456(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1580(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
555 1704(The)N
5 f
732(XInstallColormap)X
1 f
1438(function)X
1754(installs)X
2027(the)X
2157(speci\256ed)X
2492(color)X
2695(map)X
2869(for)X
2993(its)X
3099(associated)X
3483(screen.)X
3773(All)X
555 1800(windows)N
894(associated)X
1278(with)X
1457(this)X
1607(color)X
1810(map)X
1984 0.3187(immediately)AX
2449(display)X
2726(with)X
2905(true)X
3064(colors.)X
3345(You)X
3518(associated)X
3902(the)X
555 1896(windows)N
894(with)X
1073(this)X
1223(color)X
1426(map)X
1600(when)X
1812(you)X
1966(created)X
2242(them)X
2441(by)X
2551(calling)X
2814(the)X
2944(functions)X
5 f
3312(XCreateWindow)X
1 f
555 1992(or)N
5 f
668(XCreateSimpleWindow)X
1 f
1586(.)X
1652(See)X
1801(Chapter)X
2102(3)X
2168(for)X
2292(a)X
2353(discussion)X
2742(of)X
2837(how)X
3010(to)X
3101(associate)X
3441(a)X
3502(window)X
3807(with)X
555 2088(a)N
616(color)X
819(map)X
993(using)X
1206(these)X
1409(functions.)X
555 2212(The)N
714(X)X
799(server)X
1035(obtains)X
1312(the)X
1442(color)X
1645(map)X
1819(from)X
2012(a)X
2073(required)X
2388(list,)X
2541(which)X
2778(is)X
2859(an)X
2964(ordered)X
3254(list)X
3385(containing)X
3780(a)X
3841(sub-)X
555 2308(set)N
675(of)X
770(the)X
900(installed)X
1222(color)X
1425(maps.)X
1677(The)X
1836(X)X
1921(server)X
2157(might)X
2386 0.4306(implicitly)AX
2754(install)X
2993(or)X
3088(uninstall)X
3415(additional)X
3791(color)X
555 2404(maps.)N
807(Which)X
1064(other)X
1267(color)X
1470(maps)X
1678(get)X
1808(installed)X
2130(or)X
2225(uninstalled)X
2635(is)X
2716(sever-dependent,)X
3336(except)X
3588(that)X
3743(the)X
555 2500(required)N
870(list)X
1001(must)X
1195(remain)X
1462(installed.)X
1828(If)X
1908(the)X
2038(speci\256ed)X
2373(color)X
2576(map)X
2750(is)X
2831(not)X
2966(already)X
3247(an)X
3352(installed)X
3674(color)X
555 2596(map,)N
751(the)X
881(X)X
966(server)X
1202(generates)X
1556(a)X
5 f
1635(ColormapNotify)X
1 f
2277(event)X
2490(on)X
2600(every)X
2817(window)X
3122(having)X
3384(the)X
3514(cmap)X
3727(as)X
3822(the)X
555 2692(color)N
758(map)X
932(resource)X
1251(ID.)X
1409(In)X
1504(addition,)X
1838(for)X
1962(every)X
2179(other)X
2382(color)X
2585(map)X
2759(that)X
2914(is)X
2995(installed)X
3317(or)X
3412(uninstalled)X
3822(as)X
3917(a)X
555 2788(result)N
773(of)X
868(a)X
929(call)X
1079(to)X
1170(this)X
1320(function,)X
1658(the)X
1788(X)X
1873(server)X
2109(generates)X
2463(a)X
5 f
2542(ColormapNotify)X
1 f
3184(event)X
3397(on)X
3507(every)X
3724(window)X
555 2884(having)N
817(that)X
972(cmap)X
1185(as)X
1280(the)X
1410(color)X
1613(map)X
1787(resource)X
2106(ID.)X
555 3008(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XInstallColormap)X
1 f
2508(is)X
5 f
2607(BadColor)X
1 f
2988(.)X
555 3228(Use)N
5 f
731(XUninstallColormap)X
1 f
1543(to)X
1634(uninstall)X
1961(a)X
2022(color)X
2225(map.)X
2421(The)X
2580(de\256nition)X
2941(for)X
3065(this)X
3215(function)X
3531(is:)X
555 3352(XUninstallColormap)N
1302(\()X
2 f
1338(display)X
1 f
1593(,)X
2 f
1637(cmap)X
1 f
1834(\))X
687 3448(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3544(Colormap)N
2 f
1062(cmap)X
1 f
1259(;)X
2 f
555 3696(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3820(cmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(color)X
1804(map)X
1978(ID.)X
555 3944(The)N
5 f
732(XUninstallColormap)X
1 f
1544(function)X
1860(removes)X
2180(the)X
2310(speci\256ed)X
2645(color)X
2848(map)X
3022(from)X
3215(the)X
3345(required)X
3660(list)X
3791(for)X
3915(its)X
555 4040(screen.)N
845(As)X
964(a)X
1025(result,)X
1265(the)X
1395(speci\256ed)X
1730(cmap)X
1943(might)X
2172(be)X
2277(uninstalled,)X
2709(and)X
2858(the)X
2988(X)X
3073(server)X
3309(might)X
3538 0.4306(implicitly)AX
555 4136(install)N
794(or)X
889(uninstall)X
1216(additional)X
1592(color)X
1795(maps.)X
2047(Which)X
2304(color)X
2507(maps)X
2715(get)X
2845(installed)X
3167(or)X
3262(uninstalled)X
3672(is)X
3753(sever-)X
555 4232(dependent,)N
961(except)X
1213(that)X
1368(the)X
1498(required)X
1813(list)X
1944(must)X
2138(remain)X
2405(installed.)X
555 4356(If)N
635(the)X
765(speci\256ed)X
1100(color)X
1303(map)X
1477(becomes)X
1807(uninstalled,)X
2239(the)X
2369(X)X
2454(server)X
2690(generates)X
3044(a)X
5 f
3123(ColormapNotify)X
1 f
3765(event)X
555 4452(on)N
665(every)X
882(window)X
1187(having)X
1449(the)X
1579(cmap)X
1792(as)X
1887(the)X
2017(color)X
2220(map)X
2394(resource)X
2713(ID.)X
2871(In)X
2966(addition,)X
3300(for)X
3424(every)X
3641(other)X
555 4548(color)N
758(map)X
932(that)X
1087(is)X
1168(uninstalled)X
1578(as)X
1673(a)X
1734(result)X
1952(of)X
2047(a)X
2108(call)X
2258(to)X
2349(this)X
2499(function,)X
2837(the)X
2967(X)X
3052(server)X
3288(generates)X
3642(a)X
5 f
3721(Color-)X
555 4644(mapNotify)N
1 f
988(event)X
1201(on)X
1311(every)X
1528(window)X
1833(having)X
2095(that)X
2250(cmap)X
2463(as)X
2558(the)X
2688(color)X
2891(map)X
3065(resource)X
3384(ID.)X
555 4768(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUninstallColormap)X
1 f
2614(is)X
5 f
2713(BadColor)X
1 f
3094(.)X
555 4988(Use)N
5 f
731(XListInstalledColormaps)X
1 f
1716(to)X
1807(obtain)X
2050(a)X
2111(list)X
2242(of)X
2337(the)X
2467(currently)X
2807(installed)X
3129(color)X
3332(maps.)X
3562(The)X
555 5084(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 5208(Colormap)N
930(*XListInstalledColormaps)X
1873(\()X
2 f
1909(display)X
1 f
2164(,)X
2 f
2208(w)X
1 f
2267(,)X
2 f
2311(num_return)X
1 f
2733(\))X
687 5304(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5400(Window)N
2 f
1012(w)X
1 f
1078(;)X
687 5496(int)N
803(*)X
2 f
(num_return)S
1 f
1269(;)X
2 f
555 5648(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2217 6144(118)N

119 p
%%Page: 119 123
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(whose)X
3005(screen)X
3251(you)X
3405(want)X
3598(to)X
3689(obtain)X
1131 768(the)N
1261(list)X
1392(of)X
1487(currently)X
1827(installed)X
2149(color)X
2352(maps.)X
2 f
555 892(num_return)N
1 f
1131(Returns)X
1427(the)X
1557(list)X
1688(of)X
1783(currently)X
2123(installed)X
2445(color)X
2648(maps.)X
555 1016(The)N
5 f
732(XListInstalledColormaps)X
1 f
1717(function)X
2033(returns)X
2299(a)X
2360(list)X
2491(of)X
2586(the)X
2716(currently)X
3056(installed)X
3378(color)X
3581(maps)X
3789(for)X
555 1112(the)N
685(screen)X
931(of)X
1026(the)X
1156(speci\256ed)X
1491(window.)X
1840(The)X
1999(order)X
2206(in)X
2297(which)X
2534(the)X
2664(color)X
2867(maps)X
3075(appear)X
3331(in)X
3422(the)X
3552(list)X
3683(is)X
3764(not)X
555 1208(signi\256cant,)N
967(and)X
1116(there)X
1314(is)X
1395(no)X
1505(explicit)X
1793(indication)X
2169(of)X
2264(the)X
2394(required)X
2709(list.)X
2884(You)X
3057(should)X
3314(free)X
3472(the)X
3602(allocated)X
555 1304(list)N
686(by)X
796(using)X
5 f
1027(XFree)X
1 f
1282(,)X
1326(when)X
1538(it)X
1610(no)X
1720(longer)X
1967(is)X
2048(needed.)X
2363(\(See)X
2541(Section)X
2828(2.4)X
2960(for)X
3084(further)X
3345(information.\))X
555 1428(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XListInstalledColormaps)X
1 f
2787(is)X
5 f
2886(BadWindow)X
1 f
3370(.)X
3 f
555 1620(7.4.)N
731(Manipulating)X
1268(the)X
1407(Pointer)X
1 f
555 1744(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(control)X
2395(input)X
2599(from)X
2792(the)X
2922(pointer,)X
3216(which)X
3453(usually)X
3730(is)X
3811(a)X
555 1840(mouse.)N
2197 2016(Note)N
775 2152(Window)N
1100(managers)X
1459(most)X
1653(often)X
1856(use)X
1995(these)X
2198(facilities)X
2525(to)X
2616(implement)X
3017(certain)X
3279(styles)X
3502(of)X
3597(user)X
775 2248(interfaces.)N
1183(Some)X
1406(applications)X
1855(may)X
2029(use)X
2168(these)X
2371(facilities)X
2698(for)X
2822(special)X
3089(purposes.)X
3467(How-)X
775 2344(ever,)N
970(most)X
1164(applications)X
1613(will)X
1773(not)X
1908(use)X
2047(these)X
2250(functions.)X
555 2548(Usually,)N
873(the)X
1003(X)X
1088(server)X
1324(delivers)X
1625(keyboard)X
1974(and)X
2123(mouse)X
2375(events)X
2622(as)X
2717(soon)X
2905(as)X
3000(they)X
3174(occur)X
3391(to)X
3482(the)X
3612(appropri-)X
555 2644(ate)N
680(client)X
899(depending)X
1288(upon)X
1486(the)X
1616(window)X
1921(and)X
2070(input)X
2274(focus.)X
2530(The)X
2689(X)X
2774(server)X
3010(provides)X
3335(suf\256cient)X
3685(control)X
555 2740(over)N
733(event)X
946(delivery)X
1257(to)X
1348(allow)X
1566(window)X
1871(managers)X
2230(to)X
2321(support)X
2607(mouse)X
2859(ahead)X
3086(and)X
3235(various)X
3516(other)X
3719(styles)X
555 2836(of)N
650(user)X
818(interface.)X
1192(Many)X
1419(of)X
1514(these)X
1717(depend)X
1993(upon)X
2191(synchronous)X
2657(delivery)X
2968(of)X
3063(events.)X
3354(The)X
3513(delivery)X
3824(of)X
555 2932(pointer)N
827(and)X
976(keyboard)X
1325(events)X
1572(can)X
1716(be)X
1821(controlled)X
2201(independently.)X
555 3056(When)N
787(mouse)X
1039(buttons)X
1321(or)X
1416(keyboard)X
1765(keys)X
1948(are)X
2077(grabbed,)X
2404(events)X
2651(will)X
2811(be)X
2916(sent)X
3080(to)X
3171(the)X
3301(grabbing)X
3636(client)X
555 3152(rather)N
782(than)X
956(the)X
1086(normal)X
1358(client)X
1577(who)X
1750(would)X
1992(have)X
2180(received)X
2500(the)X
2630(event.)X
2887(If)X
2967(the)X
3097(keyboard)X
3446(or)X
3541(pointer)X
3813(is)X
3894(in)X
555 3248(asynchronous)N
1060(mode,)X
1300(further)X
1561(mouse)X
1813(and)X
1962(keyboard)X
2311(events)X
2558(will)X
2718(continue)X
3044(to)X
3135(be)X
3240(processed.)X
3652(If)X
3732(the)X
555 3344(keyboard)N
904(or)X
999(pointer)X
1271(is)X
1352(in)X
1443(synchronous)X
1909(mode,)X
2149(no)X
2259(further)X
2520(events)X
2767(will)X
2927(be)X
3032(processed)X
3400(until)X
3585(the)X
3715(grab-)X
555 3440(bing)N
734(client)X
953(allows)X
1205(them.)X
1448(\(See)X
5 f
1644(XAllowEvents)X
1 f
2197(.\))X
2270(The)X
2429(keyboard)X
2778(or)X
2873(pointer)X
3145(is)X
3226(considered)X
3629(frozen)X
555 3536(during)N
807(this)X
957(interval.)X
1293(The)X
1452(triggering)X
1822(event)X
2035(can)X
2179(also)X
2343(be)X
2448(replayed.)X
555 3660(There)N
782(are)X
911(two)X
1065(kinds)X
1278(of)X
1373(grabs:)X
1610(active)X
1843(and)X
1992(passive.)X
2317(An)X
2446(active)X
2679(grab)X
2857(occurs)X
3108(when)X
3320(a)X
3381(single)X
3614(client)X
555 3756(grabs)N
767(the)X
897(keyboard)X
1246(and/or)X
1493(pointer)X
1765(explicitly.)X
2166(\(See)X
5 f
2362(XGrabPointer)X
1 f
2933(and)X
5 f
3100(XGrabKeyboard)X
1 f
3747(.\))X
555 3852(Clients)N
828(can)X
972(also)X
1136(grab)X
1314(a)X
1375(particular)X
1735(keyboard)X
2084(key)X
2233(or)X
2328(pointer)X
2600(button)X
2848(in)X
2939(a)X
3000(window.)X
3349(The)X
3508(grab)X
3686(activates)X
555 3948(when)N
767(the)X
897(key)X
1046(or)X
1141(button)X
1389(is)X
1470(actually)X
1772(pressed,)X
2079(and)X
2228(is)X
2309(called)X
2542(a)X
2603(``passive)X
2942(grab''.)X
3222(Passive)X
3508(grabs)X
3720(can)X
3864(be)X
555 4044(very)N
733(convenient)X
1142(for)X
1266 0.2955(implementing)AX
1780(reliable)X
2067(pop-up)X
2338(menus.)X
2634(For)X
2778(example,)X
3121(you)X
3275(can)X
3419(arrange)X
3704(to)X
555 4140(guarantee)N
919(that)X
1074(the)X
1204(pop-up)X
1475(will)X
1635(be)X
1740(mapped)X
2041(before)X
2287(the)X
2417(up)X
2527(pointer)X
2799(button)X
3047(event)X
3260(occurs)X
3511(by)X
3621(grabbing)X
3956(a)X
555 4236(button)N
803(requesting)X
1192(synchronous)X
1658(behavior.)X
2032(The)X
2191(down)X
2408(event)X
2621(will)X
2781(trigger)X
3038(the)X
3168(grab)X
3346(and)X
3495(freeze)X
3731(further)X
555 4332(processing)N
953(of)X
1048(pointer)X
1320(events)X
1567(until)X
1752(you)X
1906(have)X
2094(the)X
2224(chance)X
2490(to)X
2581(map)X
2755(the)X
2885(pop)X
3039(up)X
3149(window.)X
3498(You)X
3671(can)X
3815(then)X
555 4428(allow)N
773(further)X
1034(event)X
1247(processing.)X
1689(The)X
1848(up)X
1958(event)X
2171(will)X
2331(then)X
2505(be)X
2610(correctly)X
2945(processed)X
3313(relative)X
3600(to)X
3691(the)X
3821(pop-)X
555 4524(up)N
665(window.)X
555 4648(For)N
699(many)X
917(operations,)X
1328(there)X
1526(are)X
1655(functions)X
2005(which)X
2242(take)X
2411(a)X
2472(time)X
2652(argument.)X
3051(The)X
3210(X)X
3295(server)X
3531(includes)X
3847(a)X
555 4744(time)N
735(stamp)X
968(in)X
1059(various)X
1340(events.)X
1631(One)X
1799(special)X
2066(time)X
2246(called)X
5 f
2497(CurrentTime)X
1 f
3022(represents)X
3400(the)X
3530(current)X
555 4840(server)N
791(time.)X
1015(The)X
1174(X)X
1259(server)X
1495(maintains)X
1861(the)X
1991(time)X
2171(when)X
2383(the)X
2513(input)X
2717(focus)X
2929(was)X
3087(last)X
3232(changed)X
3547(and)X
3696(the)X
3826(time)X
555 4936(of)N
650(the)X
780(server)X
1016(itself)X
1215(when)X
1427(the)X
1557(client)X
1776(last)X
1921(performed)X
2309(an)X
2414(active)X
2647(grab)X
2825(\(discussed)X
3213(below\),)X
3501(when)X
3713(the)X
3843(key-)X
555 5032(board)N
777(was)X
935(last)X
1080(grabbed,)X
1407(when)X
1619(the)X
1749(pointer)X
2021(was)X
2179(last)X
2324(grabbed,)X
2651(or)X
2746(when)X
2958(a)X
3019(selection)X
3355(was)X
3513(last)X
3658(changed.)X
555 5128(Your)N
757(application)X
1172(may)X
1346(be)X
1451(slow)X
1639(reacting)X
1945(to)X
2036(an)X
2141(event.)X
2398(You)X
2571(often)X
2774(need)X
2962(some)X
3170(way)X
3338(to)X
3429(specify)X
3705(that)X
555 5224(your)N
738(request)X
1014(not)X
1149(occur)X
1366(if)X
1442(some)X
1650(other)X
1853(application)X
2268(has)X
2407(in)X
2498(the)X
2628(meanwhile)X
3037(taken)X
3250(control)X
3522(of)X
3617(the)X
3747(key-)X
555 5320(board,)N
799(pointer,)X
1093(or)X
1188(selection.)X
1568(By)X
1693(providing)X
2058(the)X
2188(timestamp)X
2579(from)X
2772(the)X
2902(event)X
3115(in)X
3206(the)X
3336(request,)X
3634(you)X
3788(can)X
555 5416(arrange)N
840(that)X
995(the)X
1125(operation)X
1480(not)X
1615(take)X
1784(effect)X
2006(if)X
2082(someone)X
2417(else)X
2576(has)X
2715(in)X
2806(the)X
2936(meanwhile)X
3345(performed)X
3733(an)X
555 5512(operation.)N
555 5732(Use)N
5 f
731(XGrabPointer)X
1 f
1302(to)X
1393(grab)X
1571(the)X
1701(pointer.)X
1995(The)X
2154(de\256nition)X
2515(for)X
2639(this)X
2789(function)X
3105(is:)X
3 f
2217 6144(119)N

120 p
%%Page: 120 124
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(int)N
671(XGrabPointer)X
1171(\()X
2 f
1207(display)X
1 f
1462(,)X
2 f
1506(grab_window)X
1 f
1998(,)X
2 f
2042(owner_events)X
1 f
2533(,)X
2 f
2577(event_mask)X
1 f
2994(,)X
2 f
3038(pointer_mode)X
1 f
3534(,)X
2 f
885 768(keyboard_mode)N
1 f
1453(,)X
2 f
1497(con\256ne_to)X
1 f
1871(,)X
2 f
1915(cursor)X
1 f
2151(,)X
2 f
2195(time)X
1 f
2354(\))X
687 864(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 960(Window)N
2 f
1012(grab_window)X
1 f
1504(;)X
687 1056(Bool)N
2 f
881(owner_events)X
1 f
1372(;)X
687 1152(unsigned)N
1027(int)X
2 f
1143(event_mask)X
1 f
1560(;)X
687 1248(int)N
2 f
803(pointer_mode)X
1 f
1299(,)X
2 f
1343(keyboard_mode)X
1 f
1911(;)X
687 1344(Window)N
2 f
1012(con\256ne_to)X
1 f
1386(;)X
687 1440(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
687 1536(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 1688(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1812(grab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(relative)X
2837(to)X
2928(which)X
3165(events)X
3412(are)X
3541(reported)X
1131 1908(while)N
1349(it)X
1421(is)X
1502(grabbed.)X
2 f
555 2032(owner_events)N
1 f
1131(Speci\256es)X
1471(if)X
1547(the)X
1677(pointer)X
1949(events)X
2196(are)X
2325(to)X
2416(be)X
2521(reported)X
2836(normally)X
3177(\(pass)X
5 f
3397(True)X
1 f
3593(\))X
3644(or)X
3739(with)X
1131 2128(respect)N
1402(to)X
1493(the)X
1623(grab)X
1801(window)X
2106(if)X
2182(selected)X
2488(by)X
2598(the)X
2728(event)X
2941(mask)X
3149(\(pass)X
5 f
3369(False)X
1 f
3599(\).)X
2 f
555 2252(event_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(pointer)X
1980(events)X
2227(are)X
2356(reported)X
2671(to)X
2762(the)X
2892(client.)X
3155(They)X
3358(can)X
3502(be)X
3607(the)X
3737(bitwise)X
1131 2348(inclusive)N
1472(OR)X
1616(of)X
1711(these)X
1914(pointer)X
2186(event)X
2399(mask)X
2607(bits:)X
5 f
2800(ButtonPressMask)X
1 f
3506(,)X
5 f
3568(Button-)X
1131 2444(ReleaseMask)N
1 f
1678(,)X
5 f
1740(EnterWindowMask)X
1 f
2488(,)X
5 f
2550(LeaveWindowMask)X
1 f
3327(,)X
5 f
3389(PointerMotion-)X
1131 2540(Mask)N
1 f
1356(,)X
5 f
1418(PointerMotionHintMask)X
1 f
2342(,)X
5 f
2404(Button1MotionMask)X
1 f
3198(,)X
5 f
1146 2636(Button2MotionMask)N
1 f
1940(,)X
5 f
2002(Button3MotionMask)X
1 f
2796(,)X
5 f
2858(Button4MotionMask)X
1 f
3652(,)X
5 f
1146 2732(Button5MotionMask)N
1 f
1940(,)X
5 f
2002(ButtonMotionMask)X
1 f
2747(,)X
5 f
2809(KeyMapStateMask)X
1 f
3564(.)X
2 f
555 2856(pointer_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(pointer)X
2483(events.)X
2774(You)X
2947(can)X
3091(pass)X
3264(one)X
3413(of)X
3508(these)X
3711(con-)X
1131 2952(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 3076(keyboard_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(keyboard)X
2560(events.)X
2851(You)X
3024(can)X
3168(pass)X
3341(one)X
3490(of)X
3585(these)X
3788(con-)X
1131 3172(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 3296(con\256ne_to)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(to)X
1997(con\256ne)X
2278(the)X
2408(pointer)X
2680(in)X
2771(or)X
5 f
2884(None)X
1 f
3131(if)X
3207(it)X
3279(is)X
3360(not)X
3495(to)X
3586(be)X
1131 3392(con\256ned.)N
2 f
555 3516(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor)X
1842(that)X
1997(is)X
2078(to)X
2169(be)X
2274(displayed)X
2634(during)X
2886(the)X
3016(grab.)X
2 f
555 3640(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 3736(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 3860(The)N
5 f
732(XGrabPointer)X
1 f
1303(function)X
1619(actively)X
1921(grabs)X
2133(control)X
2405(of)X
2500(the)X
2630(pointer)X
2902(and)X
3051(returns)X
5 f
3335(GrabSuccess)X
1 f
3901(if)X
555 3956(the)N
685(grab)X
863(was)X
1021(successful.)X
1448(Further)X
1729(pointer)X
2001(events)X
2248(are)X
2377(only)X
2556(reported)X
2871(to)X
2962(the)X
3092(grabbing)X
3427(client.)X
3690(This)X
555 4052(function)N
871(overrides)X
1220(any)X
1369(active)X
1602(pointer)X
1874(grab)X
2052(by)X
2162(this)X
2312(client.)X
2575(If)X
2655(owner_events)X
3165(is)X
5 f
3264(False)X
1 f
3494(,)X
3538(all)X
3649(generated)X
555 4148(pointer)N
827(events)X
1074(are)X
1203(reported)X
1518(with)X
1697(respect)X
1968(to)X
2059(grab_window)X
2564(and)X
2713(are)X
2842(only)X
3021(reported)X
3336(if)X
3412(selected)X
3718(by)X
555 4244(event_mask.)N
1042(If)X
1122(owner_events)X
1632(is)X
5 f
1731(True)X
1 f
1927(,)X
1971(and)X
2120(if)X
2196(a)X
2257(generated)X
2621(pointer)X
2893(event)X
3106(would)X
3348(normally)X
3689(be)X
555 4340(reported)N
870(to)X
961(this)X
1111(client,)X
1352(it)X
1424(is)X
1505(reported)X
1820(normally.)X
2183(Otherwise,)X
2588(the)X
2718(event)X
2931(is)X
3012(reported)X
3327(with)X
3506(respect)X
3777(to)X
3868(the)X
555 4436(grab_window)N
1060(and)X
1209(is)X
1290(only)X
1469(reported)X
1784(if)X
1860(selected)X
2166(by)X
2276(event_mask.)X
2763(For)X
2907(either)X
3130(value)X
3343(of)X
3438(owner_events,)X
555 4532(unreported)N
958(events)X
1205(are)X
1334(discarded.)X
555 4656(Pointer_mode)N
1072(controls)X
1378(further)X
1639(processing)X
2037(of)X
2132(pointer)X
2404(events,)X
2673(and)X
2822(keyboard_mode)X
3411(controls)X
3717(further)X
555 4752(processing)N
953(of)X
1048(main)X
1247(keyboard)X
1596(events.)X
1865(If)X
1945(the)X
2075(pointer_mode)X
2587(is)X
5 f
2686(GrabModeAsync)X
1 f
3357(,)X
3401(pointer)X
3673(event)X
555 4848(processing)N
953(continues)X
1313(normally.)X
1698(If)X
1778(the)X
1908(pointer)X
2180(is)X
2261(currently)X
2601(frozen)X
2847(by)X
2957(this)X
3107(client,)X
3348(the)X
3478(processing)X
3876(of)X
555 4944(events)N
802(for)X
926(the)X
1056(pointer)X
1328(is)X
1409(resumed.)X
1773(If)X
1853(the)X
1983(pointer_mode)X
2495(is)X
5 f
2594(GrabModeSync)X
1 f
3221(,)X
3265(the)X
3395(the)X
3525(pointer,)X
3819(as)X
555 5040(seen)N
733(by)X
843(client)X
1062(applications,)X
1533(appears)X
1823(to)X
1914(freeze,)X
2172(and)X
2321(no)X
2431(further)X
2692(pointer)X
2964(events)X
3211(are)X
3340(generated)X
3704(by)X
3814(the)X
3944(X)X
555 5136(server)N
791(until)X
976(the)X
1106(grabbing)X
1441(client)X
1660(calls)X
1844(the)X
5 f
1992(XAllowEvents)X
1 f
2567(function.)X
2927(Actual)X
3184(pointer)X
3456(changes)X
3761(are)X
3890(not)X
555 5232(lost)N
705(while)X
923(the)X
1053(pointer)X
1325(is)X
1406(frozen;)X
1677(they)X
1851(are)X
1980(simply)X
2243(queued)X
2519(for)X
2643(later)X
2822(processing.)X
555 5356(If)N
635(the)X
765(keyboard_mode)X
1354(is)X
5 f
1453(GrabModeAsync)X
1 f
2124(,)X
2168(keyboard)X
2517(event)X
2730(processing)X
3128(is)X
3209(unaffected)X
3602(by)X
3712(activa-)X
555 5452(tion)N
715(of)X
810(the)X
940(grab.)X
1162(If)X
1242(the)X
1372(keyboard_mode)X
1961(is)X
5 f
2060(GrabModeSync)X
1 f
2687(,)X
2731(the)X
2861(the)X
2991(keyboard,)X
3362(as)X
3457(seen)X
3635(by)X
3745(client)X
555 5548(applications,)N
1026(appears)X
1316(to)X
1407(freeze,)X
1665(and)X
1814(no)X
1924(further)X
2185(keyboard)X
2534(events)X
2781(are)X
2910(generated)X
3274(by)X
3384(the)X
3514(X)X
3599(server)X
3835(until)X
555 5644(the)N
685(grabbing)X
1020(client)X
1239(calls)X
1423(the)X
5 f
1571(XAllowEvents)X
1 f
2146(function.)X
2506(Actual)X
2763(keyboard)X
3112(changes)X
3417(are)X
3546(not)X
3681(lost)X
555 5740(while)N
773(the)X
903(pointer)X
1175(is)X
1256(frozen;)X
1527(they)X
1701(are)X
1830(simply)X
2093(queued)X
2369(for)X
2493(later)X
2672(processing.)X
3 f
2217 6144(120)N

121 p
%%Page: 121 125
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(If)N
635(a)X
696(cursor)X
937(is)X
1018(speci\256ed,)X
1375(it)X
1447(is)X
1528(displayed)X
1888(regardless)X
2266(of)X
2361(what)X
2554(window)X
2859(the)X
2989(pointer)X
3261(is)X
3342(in.)X
3455(If)X
3535(no)X
3645(cursor)X
3886(is)X
555 768(speci\256ed,)N
912(the)X
1042(normal)X
1314(cursor)X
1555(for)X
1679(that)X
1834(window)X
2139(is)X
2220(displayed)X
2580(when)X
2792(the)X
2922(pointer)X
3194(is)X
3275(in)X
3366(grab_window)X
3871(or)X
555 864(one)N
704(of)X
799(its)X
905(subwindows;)X
1391(otherwise,)X
1777(the)X
1907(cursor)X
2148(for)X
2272(grab_window)X
2777(is)X
2858(displayed.)X
555 988(If)N
635(a)X
696(con\256ne_to)X
1090(window)X
1395(is)X
1476(speci\256ed,)X
1833(the)X
1963(pointer)X
2235(will)X
2395(be)X
2500(restricted)X
2850(to)X
2941(stay)X
3105(contained)X
3470(in)X
3561(that)X
3716(win-)X
555 1084(dow.)N
772(The)X
931(con\256ne_to)X
1325(window)X
1630(need)X
1818(have)X
2006(no)X
2116(relationship)X
2555(to)X
2646(the)X
2776(grab_window.)X
3325(If)X
3405(the)X
3535(pointer)X
3807(is)X
3888(not)X
555 1180 0.4141(initially)AN
854(in)X
945(the)X
1075(con\256ne_to)X
1469(window,)X
1796(it)X
1868(is)X
1949(warped)X
2229 0.2604(automatically)AX
2733(to)X
2824(the)X
2954(closest)X
3216(edge)X
3404(just)X
3554(before)X
3800(the)X
555 1276(grab)N
733(activates)X
1064(and)X
1213(enter/leave)X
1622(events)X
1869(are)X
1998(generated)X
2362(normally.)X
2725(If)X
2805(the)X
2935(con\256ne_to)X
3329(window)X
3634(is)X
3715(subse-)X
555 1372(quently)N
842(recon\256gured,)X
1330(the)X
1460(pointer)X
1732(will)X
1892(be)X
1997(warped)X
2277 0.2604(automatically)AX
2781(as)X
2876(necessary)X
3239(to)X
3330(keep)X
3518(it)X
3590(contained)X
555 1468(in)N
646(the)X
776(window.)X
555 1592(The)N
714(time)X
894(argument)X
1249(allows)X
1501(you)X
1655(to)X
1746(avoid)X
1964(certain)X
2226(circumstances)X
2747(that)X
2902(come)X
3115(up)X
3225(if)X
3301(applications)X
3750(take)X
3919(a)X
555 1688(long)N
734(while)X
952(to)X
1043(respond)X
1343(or)X
1438(if)X
1514(there)X
1712(are)X
1841(long)X
2020(network)X
2330(delays.)X
2621(Consider)X
2961(a)X
3022(situation)X
3349(where)X
3585(you)X
3739(have)X
555 1784(two)N
709(applications,)X
1180(both)X
1359(of)X
1454(which)X
1691(normally)X
2032(grab)X
2210(the)X
2340(pointer)X
2612(when)X
2824(clicked)X
3101(on.)X
3255(If)X
3335(both)X
3514(applications)X
555 1880(specify)N
831(the)X
961(timestamp)X
1352(from)X
1545(the)X
1675(event,)X
1910(the)X
2040(second)X
2306(application)X
2721(may)X
2895(successfully)X
3347(grab)X
3525(the)X
3655(pointer,)X
555 1976(while)N
773(the)X
903(\256rst)X
1062(gets)X
1226(an)X
1331(indication)X
1707(the)X
1837(other)X
2040(application)X
2455(grabbed)X
2760(the)X
2890(pointer)X
3162(before)X
3408(its)X
3514(request)X
3790(was)X
555 2072(processed.)N
5 f
570 2196(XGrabPointer)N
1 f
1141(generates)X
5 f
1513(EnterNotify)X
1 f
1986(and)X
5 f
2153(LeaveNotify)X
1 f
2655(events.)X
555 2320(The)N
5 f
732(XGrabPointer)X
1 f
1303(function)X
1619(fails)X
1793(and)X
1942(returns:)X
5 f
570 2464(GrabNotViewable)N
1 f
1551(If)X
1631(grab_window)X
2136(or)X
2231(con\256ne_to)X
2625(window)X
2930(is)X
3011(not)X
3146(viewable.)X
5 f
570 2608(AlreadyGrabbed)N
1 f
1551(If)X
1631(the)X
1761(pointer)X
2033(is)X
2114(actively)X
2416(grabbed)X
2721(by)X
2831(some)X
3039(other)X
3242(client.)X
5 f
570 2752(GrabFrozen)N
1 f
1551(If)X
1631(the)X
1761(pointer)X
2033(is)X
2114(frozen)X
2360(by)X
2470(an)X
2575(active)X
2808(grab)X
2986(of)X
3081(another)X
3367(client.)X
5 f
570 2896(GrabInvalidTime)N
1 f
1551(If)X
1631(the)X
1761(speci\256ed)X
2096(time)X
2276(is)X
2357(earlier)X
2604(than)X
2778(the)X
2908(last-pointer-grab)X
3517(time)X
3697(or)X
3792(later)X
1551 2992(than)N
1725(the)X
1855(current)X
2126(X)X
2211(server)X
2447(time.)X
2671(Otherwise,)X
3076(the)X
3206(last-pointer-grab)X
3815(time)X
1551 3088(is)N
1632(set)X
1752(to)X
1843(the)X
1973(speci\256ed)X
2308(time)X
2488(and)X
2637(the)X
2767(constant)X
5 f
3101(CurrentTime)X
1 f
3626(is)X
1551 3184(replaced)N
1871(by)X
1981(the)X
2111(current)X
2382(X)X
2467(server)X
2703(time.)X
555 3356(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGrabPointer)X
1 f
2407(are)X
5 f
2554(BadWindow)X
1 f
3038(,)X
5 f
3100(BadCursor)X
1 f
3535(,)X
3579(and)X
5 f
3746(Bad-)X
555 3452(Value)N
1 f
795(.)X
555 3672(Use)N
5 f
731(XGrabButton)X
1 f
1279(to)X
1370(grab)X
1548(a)X
1609(mouse)X
1861(button.)X
2131(The)X
2290(de\256nition)X
2651(for)X
2775(this)X
2925(function)X
3241(is:)X
555 3796(XGrabButton)N
1041(\()X
2 f
1077(display)X
1 f
1332(,)X
2 f
1376(button_grab)X
1 f
1819(,)X
2 f
1863(modi\256ers)X
1 f
2197(,)X
2 f
2241(grab_window)X
1 f
2733(,)X
2 f
2777(owner_events)X
1 f
3268(,)X
2 f
3312(event_mask)X
1 f
3729(,)X
2 f
907 3892(pointer_mode)N
1 f
1403(,)X
2 f
1447(keyboard_mode)X
1 f
2015(,)X
2 f
2059(con\256ne_to)X
1 f
2433(,)X
2 f
2477(cursor)X
1 f
2713(\))X
687 3988(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4084(unsigned)N
1027(int)X
2 f
1143(button_grab)X
1 f
1586(;)X
687 4180(unsigned)N
1027(int)X
2 f
1143(modi\256ers)X
1 f
1477(;)X
687 4276(Window)N
2 f
1012(grab_window)X
1 f
1504(;)X
687 4372(Bool)N
2 f
881(owner_events)X
1 f
1372(;)X
687 4468(unsigned)N
1027(int)X
2 f
1143(event_mask)X
1 f
1560(;)X
687 4564(int)N
2 f
803(pointer_mode)X
1 f
1299(,)X
2 f
1343(keyboard_mode)X
1 f
1911(;)X
687 4660(Window)N
2 f
1012(con\256ne_to)X
1 f
1386(;)X
687 4756(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
2 f
555 4908(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5032(button_grab)N
1 f
1131(Speci\256es)X
1471(the)X
1601(pointer)X
1873(button)X
2121(that)X
2276(is)X
2357(to)X
2448(be)X
2553(grabbed)X
2858(when)X
3070(the)X
3200(speci\256ed)X
3535(modi\256er)X
1131 5128(keys)N
1314(are)X
1443(down.)X
1704(You)X
1877(can)X
2021(pass)X
2194(the)X
2324(constant)X
5 f
2658(AnyButton)X
1 f
3081(,)X
3125(which)X
3362(is)X
3443(equivalent)X
3833(to)X
1131 5224(issuing)N
1403(the)X
1533(grab)X
1711(request)X
1987(for)X
2111(all)X
2222(possible)X
2533(buttons.)X
2 f
555 5348(modi\256ers)N
1 f
1131(Speci\256es)X
1471(the)X
1601(set)X
1721(of)X
1816(keymasks.)X
2229(This)X
2408(mask)X
2616(is)X
2697(the)X
2827(bitwise)X
3104(inclusive)X
3445(OR)X
3589(of)X
3684(these)X
1131 5444(keymask)N
1466(bits:)X
5 f
1659(ShiftMask)X
1 f
2061(,)X
5 f
2123(LockMask)X
1 f
2534(,)X
5 f
2596(ControlMask)X
1 f
3104(,)X
5 f
3166(Mod1Mask)X
1 f
3611(,)X
5 f
1146 5540(Mod2Mask)N
1 f
1591(,)X
5 f
1653(Mod3Mask)X
1 f
2098(,)X
5 f
2160(Mod4Mask)X
1 f
2605(,)X
5 f
2667(Mod5Mask.)X
1 f
3159(You)X
3332(can)X
3476(also)X
3640(pass)X
3813(the)X
1131 5636(constant)N
5 f
1465(AnyModi\256er)X
1 f
1944(,)X
1988(which)X
2225(is)X
2306(equivalent)X
2696(to)X
2787(issuing)X
3059(the)X
3189(grab)X
3367(request)X
3643(for)X
3767(all)X
1131 5732(possible)N
1442(modi\256er)X
1763(combinations)X
2261(\(including)X
2646(the)X
2776(combination)X
3240(of)X
3335(no)X
3445(modi\256ers\).)X
3 f
2217 6144(121)N

122 p
%%Page: 122 126
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(grab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(you)X
2704(want)X
2897(to)X
2988(grab.)X
2 f
555 796(owner_events)N
1 f
1131(Speci\256es)X
1471(if)X
1547(the)X
1677(pointer)X
1949(events)X
2196(are)X
2325(to)X
2416(be)X
2521(reported)X
2836(normally)X
3177(\(pass)X
5 f
3397(True)X
1 f
3593(\))X
3644(or)X
3739(with)X
1131 892(respect)N
1402(to)X
1493(the)X
1623(grab)X
1801(window)X
2106(if)X
2182(selected)X
2488(by)X
2598(the)X
2728(event)X
2941(mask)X
3149(\(pass)X
5 f
3369(False)X
1 f
3599(\).)X
2 f
555 1016(event_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(pointer)X
1980(events)X
2227(are)X
2356(reported)X
2671(to)X
2762(the)X
2892(client.)X
3155(They)X
3358(can)X
3502(be)X
3607(the)X
3737(bitwise)X
1131 1112(inclusive)N
1472(OR)X
1616(of)X
1711(these)X
1914(pointer)X
2186(event)X
2399(mask)X
2607(bits:)X
5 f
2800(ButtonPressMask)X
1 f
3506(,)X
5 f
3568(Button-)X
1131 1208(ReleaseMask)N
1 f
1678(,)X
5 f
1740(EnterWindowMask)X
1 f
2488(,)X
5 f
2550(LeaveWindowMask)X
1 f
3327(,)X
5 f
3389(PointerMotion-)X
1131 1304(Mask)N
1 f
1356(,)X
5 f
1418(PointerMotionHintMask)X
1 f
2342(,)X
5 f
2404(Button1MotionMask)X
1 f
3198(,)X
5 f
1146 1400(Button2MotionMask)N
1 f
1940(,)X
5 f
2002(Button3MotionMask)X
1 f
2796(,)X
5 f
2858(Button4MotionMask)X
1 f
3652(,)X
5 f
1146 1496(Button5MotionMask)N
1 f
1940(,)X
5 f
2002(ButtonMotionMask)X
1 f
2747(,)X
5 f
2809(KeyMapStateMask)X
1 f
3564(.)X
2 f
555 1620(pointer_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(pointer)X
2483(events.)X
2774(You)X
2947(can)X
3091(pass)X
3264(one)X
3413(of)X
3508(these)X
3711(con-)X
1131 1716(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 1840(keyboard_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(keyboard)X
2560(events.)X
2851(You)X
3024(can)X
3168(pass)X
3341(one)X
3490(of)X
3585(these)X
3788(con-)X
1131 1936(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 2060(con\256ne_to)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(to)X
1997(con\256ne)X
2278(the)X
2408(pointer)X
2680(in)X
2771(or)X
5 f
2884(None)X
1 f
3131(if)X
3207(it)X
3279(is)X
3360(not)X
3495(to)X
3586(be)X
1131 2156(con\256ned.)N
2 f
555 2280(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor)X
1842(that)X
1997(is)X
2078(to)X
2169(be)X
2274(displayed)X
2634(during)X
2886(the)X
3016(grab.)X
555 2404(The)N
5 f
732(XGrabButton)X
1 f
1280(function)X
1596(establishes)X
2000(a)X
2061(passive)X
2342(grab.)X
2564(Consequently,)X
3093(in)X
3184(the)X
3314(future:)X
10 f
555 2528(g)N
1 f
775(IF)X
875(the)X
1005(speci\256ed)X
1340(button)X
1588(is)X
1669(pressed)X
1954(when)X
2166(the)X
2296(speci\256ed)X
2631(modi\256er)X
2952(keys)X
3135(are)X
3264(down)X
3481(\(and)X
3659(no)X
3769(other)X
775 2624(buttons)N
1057(or)X
1152(modi\256er)X
1473(keys)X
1656(are)X
1785(down\))X
10 f
555 2748(g)N
1 f
775(AND)X
986(if)X
1062(grab_window)X
1567(contains)X
1883(the)X
2013(pointer)X
10 f
555 2872(g)N
1 f
775(AND)X
986(if)X
1062(the)X
1192(con\256ne_to)X
1586(window)X
1891(\(if)X
1996(any\))X
2174(is)X
2255(viewable)X
10 f
555 2996(g)N
1 f
775(AND)X
986(if)X
1062(these)X
1265(constraints)X
1669(are)X
1798(not)X
1933(satis\256ed)X
2244(for)X
2368(any)X
2517(ancestor)X
10 f
555 3120(g)N
1 f
775(THEN)X
1031(the)X
1161(pointer)X
1433(is)X
1514(actively)X
1816(grabbed,)X
2143(as)X
2238(for)X
5 f
2380(XGrabPointer)X
1 f
2929(,)X
2973(the)X
3103(last-pointer-grab)X
3712(time)X
3892(is)X
775 3216(set)N
895(to)X
986(the)X
1116(time)X
1296(at)X
1382(which)X
1619(the)X
1749(button)X
1997(was)X
2155(pressed)X
2440(\(as)X
2564(transmitted)X
2984(in)X
3075(the)X
5 f
3223(ButtonPress)X
1 f
3741(event\),)X
775 3312(and)N
924(the)X
5 f
1072(ButtonPress)X
1 f
1590(events)X
1837(is)X
1918(reported.)X
555 3436(The)N
714(interpretation)X
1212(of)X
1307(the)X
1437(remaining)X
1817(arguments)X
2206(is)X
2287(as)X
2382(for)X
5 f
2524(XGrabPointer)X
1 f
3073(.)X
3139(The)X
3298(active)X
3531(grab)X
3709(is)X
3790(ter-)X
555 3532(minated)N
862 0.2604(automatically)AX
1366(when)X
1578(all)X
1689(buttons)X
1971(are)X
2100(released)X
2410(\(independent)X
2892(of)X
2987(the)X
3117(state)X
3301(of)X
3396(the)X
3526(modi\256er)X
555 3628(keys\).)N
555 3752(A)N
640(modi\256ers)X
995(of)X
5 f
1108(AnyModi\256er)X
1 f
1609(is)X
1690(equivalent)X
2080(to)X
2171(issuing)X
2443(the)X
2573(grab)X
2751(request)X
3027(for)X
3151(all)X
3262(possible)X
3573(modi\256er)X
555 3848(combinations)N
1053(\(including)X
1438(the)X
1568(combination)X
2032(of)X
2127(no)X
2237(modi\256ers\).)X
2643(It)X
2719(is)X
2800(not)X
2935(required)X
3250(that)X
3405(all)X
3516(modi\256ers)X
555 3944(speci\256ed)N
890(have)X
1078(currently)X
1418(assigned)X
1743(keycodes.)X
2136(A)X
2221(button)X
2469(of)X
5 f
2582(AnyButton)X
1 f
3027(is)X
3108(equivalent)X
3498(to)X
3589(issuing)X
3861(the)X
555 4040(request)N
831(for)X
955(all)X
1066(possible)X
1377(buttons.)X
1703(Otherwise,)X
2108(it)X
2180(is)X
2261(not)X
2396(required)X
2711(that)X
2866(the)X
2996(speci\256ed)X
3331(button)X
3579(currently)X
3919(be)X
555 4136(assigned)N
880(to)X
971(a)X
1032(physical)X
1348(button.)X
555 4260(The)N
714(request)X
990(fails)X
1164(and)X
1313(the)X
1443(X)X
1528(server)X
1764(generates)X
2118(a)X
5 f
2197(BadAccess)X
1 f
2675(error)X
2867(if)X
2943(some)X
3151(other)X
3354(client)X
3573(has)X
3712(already)X
555 4356(issued)N
797(a)X
5 f
876(XGrabButton)X
1 f
1424(with)X
1603(the)X
1733(same)X
1936(button/key)X
2336(combination)X
2800(on)X
2910(the)X
3040(same)X
3243(window.)X
3592(When)X
555 4452(using)N
5 f
786(AnyModi\256er)X
1 f
1287(or)X
5 f
1400(AnyButton)X
1 f
1823(,)X
1867(the)X
1997(request)X
2273(fails)X
2447(completely)X
2862(generating)X
3256(an)X
5 f
3379(BadAccess)X
1 f
555 4548(error)N
747(\(no)X
886(grabs)X
1098(are)X
1227(established\))X
1670(if)X
1746(there)X
1944(is)X
2025(a)X
2086(con\257icting)X
2486(grab)X
2664(for)X
2788(any)X
2937(combination.)X
3445(The)X
3604(request)X
3880(has)X
555 4644(no)N
665(effect)X
887(on)X
997(an)X
1102(active)X
1335(grab.)X
555 4768(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XGrabButton)X
1 f
2800(are)X
5 f
2947(BadWindow)X
1 f
3431(,)X
5 f
3493(BadCursor)X
1 f
3928(,)X
5 f
570 4864(BadAccess)N
1 f
1026(,)X
1070(and)X
5 f
1237(BadValue)X
1 f
1634(.)X
555 5084(Use)N
5 f
731(XUngrabButton)X
1 f
1371(to)X
1462(ungrab)X
1728(a)X
1789(mouse)X
2041(button.)X
2311(The)X
2470(de\256nition)X
2831(for)X
2955(this)X
3105(function)X
3421(is:)X
555 5208(XUngrabButton)N
1129(\()X
2 f
1165(display)X
1 f
1420(,)X
2 f
1464(button_ungrab)X
1 f
1995(,)X
2 f
2039(modi\256ers)X
1 f
2373(,)X
2 f
2417(ungrab_window)X
1 f
2997(\))X
687 5304(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5400(unsigned)N
1027(int)X
2 f
1143(button_ungrab)X
1 f
1674(;)X
687 5496(unsigned)N
1027(int)X
2 f
1143(modi\256ers)X
1 f
1477(;)X
687 5592(Window)N
2 f
1012(ungrab_window)X
1 f
1592(;)X
2 f
555 5744(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
3 f
2217 6144(122)N

123 p
%%Page: 123 127
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(button_ungrab)N
1 f
1131(Speci\256es)X
1471(the)X
1601(pointer)X
1873(button)X
2121(that)X
2276(is)X
2357(to)X
2448(be)X
2553(released)X
2863(in)X
2954(combination)X
3418(with)X
3597(the)X
1131 768(modi\256er)N
1452(keys.)X
1679(You)X
1852(can)X
1996(pass)X
2169(the)X
2299(constant)X
5 f
2633(AnyButton)X
1 f
3056(,)X
3100(which)X
3337(is)X
3418(equivalent)X
3808(to)X
1131 864(issuing)N
1403(the)X
1533(ungrab)X
1799(request)X
2075(for)X
2199(all)X
2310(possible)X
2621(buttons.)X
2 f
555 988(modi\256ers)N
1 f
1131(Speci\256es)X
1471(the)X
1601(set)X
1721(of)X
1816(keymasks.)X
2229(This)X
2408(mask)X
2616(is)X
2697(the)X
2827(bitwise)X
3104(inclusive)X
3445(OR)X
3589(of)X
3684(these)X
1131 1084(keymask)N
1466(bits:)X
5 f
1659(ShiftMask)X
1 f
2061(,)X
5 f
2123(LockMask)X
1 f
2534(,)X
5 f
2596(ControlMask)X
1 f
3104(,)X
5 f
3166(Mod1Mask)X
1 f
3611(,)X
5 f
1146 1180(Mod2Mask)N
1 f
1591(,)X
5 f
1653(Mod3Mask)X
1 f
2098(,)X
5 f
2160(Mod4Mask)X
1 f
2605(,)X
5 f
2667(Mod5Mask.)X
1 f
3159(You)X
3332(can)X
3476(also)X
3640(pass)X
3813(the)X
1131 1276(constant)N
5 f
1465(AnyModi\256er)X
1 f
1944(,)X
1988(which)X
2225(is)X
2306(equivalent)X
2696(to)X
2787(issuing)X
3059(the)X
3189(ungrab)X
3455(request)X
3731(for)X
3855(all)X
1131 1372(possible)N
1442(modi\256er)X
1763(combinations)X
2261(\(including)X
2646(the)X
2776(combination)X
3240(of)X
3335(no)X
3445(modi\256ers\).)X
2 f
555 1496(ungrab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(you)X
2704(want)X
2897(to)X
2988(ungrab.)X
555 1620(The)N
5 f
732(XUngrabButton)X
1 f
1372(function)X
1688(releases)X
1988(the)X
2118(passive)X
2399(button/key)X
2799(combination)X
3263(on)X
3373(the)X
3503(speci\256ed)X
3838(win-)X
555 1716(dow)N
728(if)X
804(it)X
876(was)X
1034(grabbed)X
1339(by)X
1449(this)X
1599(client.)X
1862(A)X
1947(modi\256ers)X
2302(of)X
5 f
2415(AnyModi\256er)X
1 f
2916(is)X
2997(equivalent)X
3387(to)X
3478(issuing)X
3750(the)X
555 1812(ungrab)N
821(request)X
1097(for)X
1221(all)X
1332(possible)X
1643(modi\256er)X
1964(combinations,)X
2484(including)X
2840(the)X
2970(combination)X
3434(of)X
3529(no)X
3639(modi\256ers.)X
555 1908(A)N
640(button)X
888(of)X
5 f
1001(AnyButton)X
1 f
1446(is)X
1527(equivalent)X
1917(to)X
2008(issuing)X
2280(the)X
2410(request)X
2686(for)X
2810(all)X
2921(possible)X
3232(buttons.)X
3558(This)X
3737(func-)X
555 2004(tion)N
715(has)X
854(no)X
964(effect)X
1186(on)X
1296(an)X
1401(active)X
1634(grab.)X
555 2128(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUngrabButton)X
1 f
2442(is)X
5 f
2541(BadWindow)X
1 f
3025(.)X
555 2348(Use)N
5 f
731(XUngrabPointer)X
1 f
1394(to)X
1485(ungrab)X
1751(the)X
1881(pointer.)X
2175(The)X
2334(de\256nition)X
2695(for)X
2819(this)X
2969(function)X
3285(is:)X
555 2472(XUngrabPointer)N
1143(\()X
2 f
1179(display)X
1 f
1434(,)X
2 f
1478(time)X
1 f
1637(\))X
687 2568(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2664(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 2816(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2940(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 3036(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 3160(The)N
5 f
732(XUngrabPointer)X
1 f
1395(function)X
1711(releases)X
2011(the)X
2141(pointer)X
2413(and)X
2562(any)X
2711(queued)X
2987(events,)X
3256(if)X
3332(this)X
3482(client)X
3701(has)X
555 3256(actively)N
857(grabbed)X
1162(the)X
1292(pointer)X
1564(from)X
5 f
1775(XGrabPointer)X
1 f
2324(,)X
5 f
2386(XGrabButton)X
1 f
2912(,)X
2956(or)X
3051(from)X
3244(a)X
3305(normal)X
3577(button)X
555 3352(press.)N
801(The)X
960(function)X
1276(does)X
1459(not)X
1594(release)X
1860(the)X
1990(pointer)X
2262(if)X
2338(the)X
2468(speci\256ed)X
2803(time)X
2983(is)X
3064(earlier)X
3311(than)X
3485(the)X
3615(last-)X
555 3448(pointer-grab)N
1012(time)X
1192(or)X
1287(is)X
1368(later)X
1547(than)X
1721(the)X
1851(current)X
2122(X)X
2207(server)X
2443(time.)X
2667(It)X
2743(also)X
2907(generates)X
5 f
3279(EnterNotify)X
1 f
3752(and)X
5 f
570 3544(LeaveNotify)N
1 f
1072(events.)X
1363(The)X
1522(X)X
1607(server)X
1843(performs)X
2182(an)X
5 f
2305(XUngrabPointer)X
1 f
2968 0.2604(automatically)AX
3472(if)X
3548(the)X
3678(event)X
555 3640(window)N
860(or)X
955(con\256ne-to)X
1334(window)X
1639(for)X
1763(an)X
1868(active)X
2101(pointer)X
2373(grab)X
2551(becomes)X
2881(not)X
3016(viewable.)X
555 3860(Use)N
5 f
731(XChangeActivePointerGrab)X
1 f
1850(to)X
1941(change)X
2212(the)X
2342(active)X
2575(pointer.)X
2869(The)X
3028(de\256nition)X
3389(for)X
3513(this)X
3663(function)X
555 3956(is:)N
555 4080(XChangeActivePointerGrab)N
1559(\()X
2 f
1595(display)X
1 f
1850(,)X
2 f
1894(event_mask)X
1 f
2311(,)X
2 f
2355(cursor)X
1 f
2591(,)X
2 f
2635(time)X
1 f
2794(\))X
687 4176(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4272(unsigned)N
1027(int)X
2 f
1143(event_mask)X
1 f
1560(;)X
687 4368(Cursor)N
2 f
948(cursor)X
1 f
1184(;)X
687 4464(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 4616(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4740(event_mask)N
1 f
1131(Speci\256es)X
1471(which)X
1708(pointer)X
1980(events)X
2227(are)X
2356(reported)X
2671(to)X
2762(the)X
2892(client.)X
3155(They)X
3358(can)X
3502(be)X
3607(the)X
3737(bitwise)X
1131 4836(inclusive)N
1472(OR)X
1616(of)X
1711(these)X
1914(pointer)X
2186(event)X
2399(mask)X
2607(bits:)X
5 f
2800(ButtonPressMask)X
1 f
3506(,)X
5 f
3568(Button-)X
1131 4932(ReleaseMask)N
1 f
1678(,)X
5 f
1740(EnterWindowMask)X
1 f
2488(,)X
5 f
2550(LeaveWindowMask)X
1 f
3327(,)X
5 f
3389(PointerMotion-)X
1131 5028(Mask)N
1 f
1356(,)X
5 f
1418(PointerMotionHintMask)X
1 f
2342(,)X
5 f
2404(Button1MotionMask)X
1 f
3198(,)X
5 f
1146 5124(Button2MotionMask)N
1 f
1940(,)X
5 f
2002(Button3MotionMask)X
1 f
2796(,)X
5 f
2858(Button4MotionMask)X
1 f
3652(,)X
5 f
1146 5220(Button5MotionMask)N
1 f
1940(,)X
5 f
2002(ButtonMotionMask)X
1 f
2747(,)X
5 f
2809(KeyMapStateMask)X
1 f
3564(.)X
2 f
555 5344(cursor)N
1 f
1131(Speci\256es)X
1471(the)X
1601(cursor.)X
1864(This)X
2043(is)X
2124(the)X
2254(cursor)X
2495(that)X
2650(is)X
2731(displayed.)X
3135(A)X
3220(possible)X
3531(value)X
3744(you)X
1131 5440(can)N
1275(pass)X
1448(is)X
5 f
1547(None)X
1 f
1772(.)X
2 f
555 5564(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 5660(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
3 f
2217 6144(123)N

124 p
%%Page: 124 128
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
5 f
732(XChangeActivePointerGrab)X
1 f
1851(function)X
2167(changes)X
2472(the)X
2602(speci\256ed)X
2937(dynamic)X
3263(parameters)X
3671(if)X
3747(the)X
555 768(pointer)N
827(is)X
908(actively)X
1210(grabbed)X
1515(by)X
1625(the)X
1755(client)X
1974(and)X
2123(if)X
2199(the)X
2329(speci\256ed)X
2664(time)X
2844(is)X
2925(no)X
3035(earlier)X
3282(than)X
3456(the)X
3586(last-)X
555 864(pointer-grab)N
1012(time)X
1192(and)X
1341(no)X
1451(later)X
1630(than)X
1804(the)X
1934(current)X
2205(X)X
2290(server)X
2526(time.)X
2750(This)X
2929(function)X
3245(has)X
3384(no)X
3494(effect)X
3716(on)X
3826(the)X
555 960(passive)N
836(parameters)X
1244(of)X
1339(a)X
5 f
1418(XGrabButton)X
1 f
1944(.)X
2010(The)X
2169(event-mask)X
2597(is)X
2678(always)X
2944(augmented)X
3353(to)X
3444(include)X
5 f
3744(But-)X
555 1056(tonPress)N
1 f
940(mask)X
1148(and)X
5 f
1315(ButtonRelease)X
1 f
1930(mask.)X
2182(The)X
2341(interpretation)X
2839(of)X
2934(event_mask)X
3377(and)X
3526(cursor)X
3767(is)X
3848(the)X
555 1152(same)N
758(as)X
853(described)X
1212(in)X
5 f
1321(XGrabPointer)X
1 f
1870(.)X
555 1276(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XChangeActivePointerGrab)X
1 f
2921(is)X
5 f
3020(BadCursor)X
1 f
3455(.)X
3 f
555 1468(7.5.)N
731(Manipulating)X
1268(the)X
1407(Keyboard)X
1 f
555 1592(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(grab)X
2301(or)X
2396(ungrab)X
2662(the)X
2792(keyboard)X
3141(as)X
3236(well)X
3410(as)X
3505(allow)X
3723(events.)X
555 1812(Use)N
5 f
731(XGrabKeyboard)X
1 f
1400(to)X
1491(grab)X
1669(the)X
1799(keyboard.)X
2170(The)X
2329(de\256nition)X
2690(for)X
2814(this)X
2964(function)X
3280(is:)X
555 1936(int)N
671(XGrabKeyboard)X
1262(\()X
2 f
1298(display)X
1 f
1553(,)X
2 f
1597(grab_window)X
1 f
2089(,)X
2 f
2133(owner_events)X
1 f
2624(,)X
2 f
2668(pointer_mode)X
1 f
3164(,)X
2 f
3208(keyboard_mode)X
1 f
3776(,)X
2 f
3820(time)X
1 f
3979(\))X
687 2032(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2128(Window)N
2 f
1012(grab_window)X
1 f
1504(;)X
687 2224(Bool)N
2 f
881(owner_events)X
1 f
1372(;)X
687 2320(int)N
2 f
803(pointer_mode)X
1 f
1299(,)X
2 f
1343(keyboard_mode)X
1 f
1911(;)X
687 2416(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 2568(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2692(grab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(associated)X
2934(with)X
3113(the)X
3243(keyboard)X
3592(you)X
3746(want)X
3939(to)X
1131 2788(grab.)N
2 f
555 2912(owner_events)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(of)X
2141(either)X
5 f
2382(True)X
1 f
2600(or)X
5 f
2713(False)X
1 f
2943(.)X
2 f
555 3036(pointer_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(pointer)X
2483(events.)X
2774(You)X
2947(can)X
3091(pass)X
3264(one)X
3413(of)X
3508(these)X
3711(con-)X
1131 3132(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 3256(keyboard_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(keyboard)X
2560(events.)X
2851(You)X
3024(can)X
3168(pass)X
3341(one)X
3490(of)X
3585(these)X
3788(con-)X
1131 3352(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 3476(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 3572(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 3696(The)N
5 f
732(XGrabKeyboard)X
1 f
1401(function)X
1717(actively)X
2019(grabs)X
2231(control)X
2503(of)X
2598(the)X
2728(main)X
2927(keyboard)X
3276(and)X
3425(generates)X
5 f
570 3792(FocusIn)N
1 f
921(and)X
5 f
1088(FocusOut)X
1 f
1508(events.)X
1799(Further)X
2080(key)X
2229(events)X
2476(are)X
2605(reported)X
2920(only)X
3099(to)X
3190(the)X
3320(grabbing)X
3655(client.)X
555 3888(This)N
734(function)X
1050(overrides)X
1399(any)X
1548(active)X
1781(keyboard)X
2130(grab)X
2308(by)X
2418(this)X
2568(client.)X
2831(If)X
2911(owner_events)X
3421(is)X
5 f
3520(False)X
1 f
3750(,)X
3794(all)X
555 3984(generated)N
919(key)X
1068(events)X
1315(are)X
1444(reported)X
1759(with)X
1938(respect)X
2209(to)X
2300(grab_window.)X
2827(If)X
2907(owner_events)X
3417(is)X
5 f
3516(True)X
1 f
3712(,)X
3756(then)X
3930(if)X
555 4080(a)N
616(generated)X
980(key)X
1129(event)X
1342(would)X
1584(normally)X
1925(be)X
2030(reported)X
2345(to)X
2436(this)X
2586(client,)X
2827(it)X
2899(is)X
2980(reported)X
3295(normally;)X
3661(otherwise)X
555 4176(the)N
685(event)X
898(is)X
979(reported)X
1294(with)X
1473(respect)X
1744(to)X
1835(the)X
1965(grab_window.)X
2492(Both)X
5 f
2704(KeyPress)X
1 f
3118(and)X
5 f
3285(KeyRelease)X
1 f
555 4272(events)N
802(are)X
931(always)X
1197(reported,)X
1534(independent)X
1987(of)X
2082(any)X
2231(event)X
2444(selection)X
2780(made)X
2993(by)X
3103(the)X
3233(client.)X
555 4396(The)N
714(pointer_mode)X
1226(argument)X
1581(controls)X
1887(the)X
2017(further)X
2278(processing)X
2676(of)X
2771(pointer)X
3043(events,)X
3312(and)X
3461(the)X
555 4492(keyboard_mode)N
1144(argument)X
1499(controls)X
1805(the)X
1935(further)X
2196(processing)X
2594(of)X
2689(the)X
2819(keyboard)X
3168(events.)X
10 f
555 4616(g)N
1 f
775(If)X
855(the)X
985(keyboard_mode)X
1574(argument)X
1929(is)X
5 f
2028(GrabModeAsync)X
1 f
2699(,)X
2743(keyboard)X
3092(event)X
3305(processing)X
3703(contin-)X
775 4712(ues)N
914(normally;)X
1280(if)X
1356(the)X
1486(keyboard)X
1835(is)X
1916(currently)X
2256(frozen)X
2502(by)X
2612(this)X
2762(client,)X
3003(then)X
3177(processing)X
3575(of)X
3670(keyboard)X
775 4808(events)N
1022(is)X
1103(resumed.)X
1467(If)X
1547(the)X
1677(keyboard_mode)X
2288(argument)X
2643(is)X
5 f
2742(GrabModeSync)X
1 f
3369(,)X
3413(the)X
3543(keyboard)X
3892(\(as)X
775 4904(seen)N
953(by)X
1063(client)X
1282(applications\))X
1760(appears)X
2050(to)X
2141(freeze,)X
2399(and)X
2548(no)X
2658(further)X
2919(keyboard)X
3268(events)X
3515(are)X
3644(generated)X
775 5000(by)N
885(the)X
1015(server)X
1251(until)X
1436(the)X
1566(grabbing)X
1901(client)X
2120(issues)X
2352(a)X
2413(releasing)X
5 f
2771(XAllowEvents)X
1 f
3346(call.)X
3540(Actual)X
3797(key-)X
775 5096(board)N
997(changes)X
1302(are)X
1431(not)X
1566(lost)X
1716(while)X
1934(the)X
2064(keyboard)X
2413(is)X
2494(frozen;)X
2765(they)X
2939(are)X
3068(simply)X
3331(queued)X
3607(for)X
3731(later)X
775 5192(processing.)N
10 f
555 5316(g)N
1 f
775(If)X
855(pointer_mode)X
1367(is)X
5 f
1466(GrabModeAsync)X
1 f
2137(,)X
2181(pointer)X
2453(event)X
2666(processing)X
3064(is)X
3145(unaffected)X
3538(by)X
3648(activation)X
775 5412(of)N
870(the)X
1000(grab.)X
1200(If)X
1280(pointer_mode)X
1792(is)X
5 f
1891(GrabModeSync)X
1 f
2518(,)X
2562(the)X
2692(pointer)X
2964(\(as)X
3088(seen)X
3266(by)X
3376(client)X
3595(applica-)X
775 5508(tions\))N
998(appears)X
1288(to)X
1379(freeze,)X
1637(and)X
1786(no)X
1896(further)X
2157(pointer)X
2429(events)X
2676(are)X
2805(generated)X
3169(by)X
3279(the)X
3409(server)X
3645(until)X
3830(the)X
775 5604(grabbing)N
1110(client)X
1329(issues)X
1561(a)X
1622(releasing)X
5 f
1980(XAllowEvents)X
1 f
2555(call.)X
2749(Actual)X
3006(pointer)X
3278(changes)X
3583(are)X
3712(not)X
3847(lost)X
775 5700(while)N
993(the)X
1123(pointer)X
1395(is)X
1476(frozen;)X
1747(they)X
1921(are)X
2050(simply)X
2313(queued)X
2589(for)X
2713(later)X
2892(processing.)X
3 f
2217 6144(124)N

125 p
%%Page: 125 129
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
570 672(XGrabKeyboard)N
1 f
1239(fails)X
1413(and)X
1562(returns:)X
5 f
570 816(AlreadyGrabbed)N
1 f
1551(If)X
1631(the)X
1761(keyboard)X
2110(is)X
2191(actively)X
2493(grabbed)X
2798(by)X
2908(some)X
3116(other)X
3319(client.)X
5 f
570 960(GrabNotViewable)N
1 f
1551(If)X
1631(grab_window)X
2136(is)X
2217(not)X
2352(viewable.)X
5 f
570 1104(GrabInvalidTime)N
1 f
1551(If)X
1631(the)X
1761(speci\256ed)X
2096(time)X
2276(is)X
2357(earlier)X
2604(than)X
2778(the)X
2908(last-keyboard-grab)X
3594(time)X
3774(or)X
1551 1200(later)N
1730(than)X
1904(the)X
2034(current)X
2305(X)X
2390(server)X
2626(time.)X
2850(Otherwise,)X
3255(the)X
3385(last-keyboard-)X
1551 1296(grab)N
1729(time)X
1909(is)X
1990(set)X
2110(to)X
2201(the)X
2331(speci\256ed)X
2666(time)X
2846(and)X
5 f
3013(CurrentTime)X
1 f
3538(is)X
3619(replaced)X
1551 1392(by)N
1661(the)X
1791(current)X
2062(X)X
2147(server)X
2383(time.)X
5 f
570 1536(GrabFrozen)N
1 f
1551(If)X
1631(the)X
1761(keyboard)X
2110(is)X
2191(frozen)X
2437(by)X
2547(an)X
2652(active)X
2885(grab)X
3063(of)X
3158(another)X
3444(client.)X
555 1708(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGrabKeyboard)X
1 f
2505(are)X
5 f
2652(BadWindow)X
1 f
3158(and)X
5 f
3325(BadValue)X
1 f
3722(.)X
555 1928(Use)N
5 f
731(XUngrabKeyboard)X
1 f
1492(to)X
1583(ungrab)X
1849(the)X
1979(keyboard.)X
2350(The)X
2509(de\256nition)X
2870(for)X
2994(this)X
3144(function)X
3460(is:)X
555 2052(XUngrabKeyboard)N
1234(\()X
2 f
1270(display)X
1 f
1525(,)X
2 f
1569(time)X
1 f
1728(\))X
687 2148(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2244(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 2396(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2520(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 2616(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 2740(The)N
5 f
732(XUngrabKeyboard)X
1 f
1493(function)X
1809(releases)X
2109(the)X
2239(keyboard)X
2588(and)X
2737(any)X
2886(queued)X
3162(events)X
3409(if)X
3485(this)X
3635(client)X
3854(has)X
555 2836(it)N
627(actively)X
929(grabbed)X
1234(from)X
1427(either)X
5 f
1668(XGrabKeyboard)X
1 f
2337(or)X
5 f
2450(XGrabKey)X
1 f
2872(.)X
2938(The)X
3097(function)X
3413(does)X
3596(not)X
3731(release)X
555 2932(the)N
685(keyboard)X
1034(and)X
1183(any)X
1332(queued)X
1608(events)X
1855(if)X
1931(the)X
2061(speci\256ed)X
2396(time)X
2576(is)X
2657(earlier)X
2904(than)X
3078(the)X
3208(last-keyboard-grab)X
555 3028(time)N
735(or)X
830(is)X
911(later)X
1090(than)X
1264(the)X
1394(current)X
1665(X)X
1750(server)X
1986(time.)X
2210(It)X
2286(also)X
2450(generates)X
5 f
2822(FocusIn)X
1 f
3173(and)X
5 f
3340(FocusOut)X
1 f
3760(events.)X
555 3124(The)N
714(X)X
799(server)X
1035 0.2604(automatically)AX
1539(performs)X
1878(an)X
5 f
2001(XUngrabKeyboard)X
1 f
2762(if)X
2838(the)X
2968(event)X
3181(window)X
3486(for)X
3610(an)X
3715(active)X
555 3220(keyboard)N
904(grab)X
1082(becomes)X
1412(not)X
1547(viewable.)X
555 3440(Use)N
5 f
731(XGrabKey)X
1 f
1175(to)X
1266(passively)X
1616(grab)X
1794(a)X
1855(single)X
2088(key)X
2237(of)X
2332(the)X
2462(keyboard)X
2811(key.)X
2982(The)X
3141(de\256nition)X
3502(for)X
3626(this)X
3776(func-)X
555 3536(tion)N
715(is:)X
555 3660(XGrabKey)N
946(\()X
2 f
982(display)X
1 f
1237(,)X
2 f
1281(keycode)X
1 f
1571(,)X
2 f
1615(modi\256ers)X
1 f
1949(,)X
2 f
1993(grab_window)X
1 f
2485(,)X
2 f
2529(owner_events)X
1 f
3020(,)X
2 f
3064(pointer_mode)X
1 f
3560(,)X
2 f
841 3756(keyboard_mode)N
1 f
1409(\))X
687 3852(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3948(int)N
2 f
803(keycode)X
1 f
1093(;)X
687 4044(unsigned)N
1027(int)X
2 f
1143(modi\256ers)X
1 f
1477(;)X
687 4140(Window)N
2 f
1012(grab_window)X
1 f
1504(;)X
687 4236(Bool)N
2 f
881(owner_events)X
1 f
1372(;)X
687 4332(int)N
2 f
803(pointer_mode)X
1 f
1299(,)X
2 f
1343(keyboard_mode)X
1 f
1911(;)X
2 f
555 4484(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4608(keycode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(keycode.)X
1960(This)X
2139(keycode)X
2454(maps)X
2662(to)X
2753(the)X
2883(speci\256c)X
3174(key)X
3323(you)X
3477(want)X
3670(to)X
3761(grab.)X
1131 4704(You)N
1304(can)X
1448(pass)X
1621(either)X
1844(the)X
1974(keycode)X
2289(or)X
2384(the)X
2514(constant)X
5 f
2848(AnyKey)X
1 f
3167(.)X
2 f
555 4828(modi\256ers)N
1 f
1131(Speci\256es)X
1471(the)X
1601(set)X
1721(of)X
1816(keymasks.)X
2229(This)X
2408(mask)X
2616(is)X
2697(the)X
2827(bitwise)X
3104(inclusive)X
3445(OR)X
3589(of)X
3684(these)X
1131 4924(keymask)N
1466(bits:)X
5 f
1659(ShiftMask)X
1 f
2061(,)X
5 f
2123(LockMask)X
1 f
2534(,)X
5 f
2596(ControlMask)X
1 f
3104(,)X
5 f
3166(Mod1Mask)X
1 f
3611(,)X
5 f
1146 5020(Mod2Mask)N
1 f
1591(,)X
5 f
1653(Mod3Mask)X
1 f
2098(,)X
5 f
2160(Mod4Mask)X
1 f
2605(,)X
5 f
2667(Mod5Mask.)X
1 f
1131 5144(You)N
1304(can)X
1448(also)X
1612(pass)X
1785(the)X
1915(constant)X
5 f
2249(AnyModi\256er)X
1 f
2728(,)X
2772(which)X
3009(is)X
3090(equivalent)X
3480(to)X
3571(issuing)X
3843(the)X
1131 5240(grab)N
1309(key)X
1458(request)X
1734(for)X
1858(all)X
1969(possible)X
2280(modi\256er)X
2601(combinations)X
3099(\(including)X
3484(the)X
3614(combina-)X
1131 5336(tion)N
1291(of)X
1386(no)X
1496(modi\256ers\).)X
2 f
555 5460(grab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(associated)X
2934(with)X
3113(the)X
3243(keys)X
3426(you)X
3580(want)X
3773(to)X
1131 5556(grab.)N
2 f
555 5680(owner_events)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(of)X
2141(either)X
5 f
2382(True)X
1 f
2600(or)X
5 f
2713(False)X
1 f
2943(.)X
3 f
2217 6144(125)N

126 p
%%Page: 126 130
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(pointer_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(pointer)X
2483(events.)X
2774(You)X
2947(can)X
3091(pass)X
3264(one)X
3413(of)X
3508(these)X
3711(con-)X
1131 768(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
2 f
555 892(keyboard_mode)N
1 f
1131(Controls)X
1457(further)X
1718(processing)X
2116(of)X
2211(keyboard)X
2560(events.)X
2851(You)X
3024(can)X
3168(pass)X
3341(one)X
3490(of)X
3585(these)X
3788(con-)X
1131 988(stants:)N
5 f
1397(GrabModeSync)X
1 f
2046(or)X
5 f
2159(GrabModeAsync)X
1 f
2830(.)X
555 1112(The)N
5 f
732(XGrabKey)X
1 f
1176(function)X
1492(establishes)X
1896(a)X
1957(passive)X
2238(grab)X
2416(on)X
2526(the)X
2656(keyboard.)X
3049(Consequently,)X
3578(in)X
3669(the)X
555 1208(future:)N
10 f
555 1332(g)N
1 f
775(IF)X
875(the)X
1005(speci\256ed)X
1340(key,)X
1511(which)X
1748(itself)X
1947(can)X
2091(be)X
2196(a)X
2257(modi\256er)X
2578(key,)X
2749(is)X
2830(pressed)X
3115(when)X
3327(the)X
3457(speci\256ed)X
775 1428(modi\256er)N
1096(keys)X
1279(are)X
1408(down)X
1625(\(and)X
1803(no)X
1913(other)X
2116(keys)X
2299(are)X
2428(down\),)X
10 f
555 1552(g)N
1 f
775(AND)X
986(EITHER)X
1321(the)X
1451(grab)X
1629(window)X
1934(is)X
2015(an)X
2120(ancestor)X
2435(of)X
2530(\(or)X
2654(is\))X
2764(the)X
2894(focus)X
3106(window)X
3411(OR)X
3555(the)X
3685(grab)X
775 1648(window)N
1080(is)X
1161(a)X
1222(descendent)X
1635(of)X
1730(the)X
1860(focus)X
2072(window)X
2377(and)X
2526(contains)X
2842(the)X
2972(pointer,)X
10 f
555 1772(g)N
1 f
775(AND)X
986(these)X
1189(constraints)X
1593(are)X
1722(not)X
1857(satis\256ed)X
2168(for)X
2292(any)X
2441(ancestor)X
2756(of)X
2851(grab_window,)X
10 f
555 1896(g)N
1 f
775(THEN)X
1031(the)X
1161(keyboard)X
1510(is)X
1591(actively)X
1893(grabbed,)X
2220(as)X
2315(for)X
5 f
2457(XGrabKeyboard)X
1 f
3104(,)X
3148(the)X
3278(last-keyboard-grab)X
775 1992(time)N
955(is)X
1036(set)X
1156(to)X
1247(the)X
1377(time)X
1557(at)X
1643(which)X
1880(the)X
2010(key)X
2159(was)X
2317(pressed)X
2602(\(as)X
2726(transmitted)X
3146(in)X
3237(the)X
5 f
3385(KeyPress)X
1 f
775 2088(event\),)N
1039(and)X
1188(the)X
5 f
1336(KeyPress)X
1 f
1750(event)X
1963(is)X
2044(reported.)X
555 2212(The)N
714(interpretation)X
1212(of)X
1307(the)X
1437(remaining)X
1817(arguments)X
2206(is)X
2287(as)X
2382(for)X
5 f
2524(XGrabKeyboard)X
1 f
3171(.)X
3237(The)X
3396(active)X
3629(grab)X
3807(is)X
3888(ter-)X
555 2308(minated)N
862 0.2604(automatically)AX
1366(when)X
1578(the)X
1708(speci\256ed)X
2043(key)X
2192(has)X
2331(been)X
2519(released)X
2829(\(independent)X
3311(of)X
3406(the)X
3536(state)X
3720(of)X
3815(the)X
555 2404(modi\256er)N
876(keys\).)X
555 2528(A)N
640(modi\256ers)X
995(of)X
5 f
1108(AnyModi\256er)X
1 f
1609(is)X
1690(equivalent)X
2080(to)X
2171(issuing)X
2443(the)X
2573(request)X
2849(for)X
2973(all)X
3084(possible)X
3395(modi\256er)X
3716(combi-)X
555 2624(nations)N
832(\(including)X
1217(the)X
1347(combination)X
1811(of)X
1906(no)X
2016(modi\256ers\).)X
2422(It)X
2498(is)X
2579(not)X
2714(required)X
3029(that)X
3184(all)X
3295(modi\256ers)X
3650(speci\256ed)X
555 2720(have)N
743(currently)X
1083(assigned)X
1408(keycodes.)X
1801(A)X
1886(key)X
2035(of)X
5 f
2148(AnyKey)X
1 f
2489(is)X
2570(equivalent)X
2960(to)X
3051(issuing)X
3323(the)X
3453(request)X
3729(for)X
3853(all)X
555 2816(possible)N
866(keycodes.)X
1259(Otherwise,)X
1664(the)X
1794(key)X
1943(must)X
2137(be)X
2242(in)X
2333(the)X
2463(range)X
2680(speci\256ed)X
3015(by)X
3125(min_keycode)X
3622(and)X
555 2912(max_keycode)N
1066(in)X
1157(the)X
1287(connection)X
1696(setup.)X
1926(Otherwise,)X
2331(a)X
5 f
2410(BadValue)X
1 f
2829(error)X
3021(is)X
3102(generated.)X
555 3036(A)N
5 f
658(BadAccess)X
1 f
1136(error)X
1328(is)X
1409(generated)X
1773(if)X
1849(some)X
2057(other)X
2260(client)X
2479(has)X
2618(issued)X
2860(a)X
5 f
2939(XGrabKey)X
1 f
3383(with)X
3562(the)X
3692(same)X
555 3132(key)N
704(combination)X
1168(on)X
1278(the)X
1408(same)X
1611(window.)X
1938(When)X
2170(using)X
5 f
2401(AnyModi\256er)X
1 f
2902(or)X
5 f
3015(AnyKey)X
1 f
3334(,)X
3378(the)X
3508(request)X
3784(fails)X
555 3228(completely)N
970(and)X
1119(the)X
1249(X)X
1334(server)X
1570(generates)X
1924(a)X
5 f
2003(BadAccess)X
1 f
2481(error)X
2673(and)X
2822(no)X
2932(grabs)X
3144(are)X
3273(established)X
3687(if)X
3763(there)X
555 3324(is)N
636(a)X
697(con\257icting)X
1097(grab)X
1275(for)X
1399(any)X
1548(combination.)X
555 3448(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XGrabKey)X
1 f
2280(are)X
5 f
2427(BadAccess)X
1 f
2883(,)X
5 f
2945(BadValue)X
1 f
3342(,)X
3386(and)X
5 f
3553(BadWin-)X
555 3544(dow)N
1 f
731(.)X
555 3764(Use)N
5 f
731(XUngrabKey)X
1 f
1267(to)X
1358(ungrab)X
1624(a)X
1685(key.)X
1856(The)X
2015(de\256nition)X
2376(for)X
2500(this)X
2650(function)X
2966(is:)X
555 3888(XUngrabKey)N
1034(\()X
2 f
1070(display)X
1 f
1325(,)X
2 f
1369(keycode)X
1 f
1659(,)X
2 f
1703(modi\256ers)X
1 f
2037(,)X
2 f
2081(ungrab_window)X
1 f
2661(\))X
687 3984(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4080(int)N
2 f
803(keycode)X
1 f
1093(;)X
687 4176(unsigned)N
1027(int)X
2 f
1143(modi\256ers)X
1 f
1477(;)X
687 4272(Window)N
2 f
1012(ungrab_window)X
1 f
1592(;)X
2 f
555 4424(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4548(keycode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(keycode.)X
1960(This)X
2139(keycode)X
2454(maps)X
2662(to)X
2753(the)X
2883(speci\256c)X
3174(key)X
3323(you)X
3477(want)X
3670(to)X
1131 4644(ungrab.)N
1441(You)X
1614(can)X
1758(pass)X
1931(either)X
2154(the)X
2284(keycode)X
2599(or)X
2694(the)X
2824(constant)X
5 f
3158(AnyKey)X
1 f
3477(.)X
2 f
555 4768(modi\256ers)N
1 f
1131(Speci\256es)X
1471(the)X
1601(set)X
1721(of)X
1816(keymasks.)X
2229(This)X
2408(mask)X
2616(is)X
2697(the)X
2827(bitwise)X
3104(inclusive)X
3445(OR)X
3589(of)X
3684(these)X
1131 4864(keymask)N
1466(bits:)X
5 f
1659(ShiftMask)X
1 f
2061(,)X
5 f
2123(LockMask)X
1 f
2534(,)X
5 f
2596(ControlMask)X
1 f
3104(,)X
5 f
3166(Mod1Mask)X
1 f
3611(,)X
5 f
1146 4960(Mod2Mask)N
1 f
1591(,)X
5 f
1653(Mod3Mask)X
1 f
2098(,)X
5 f
2160(Mod4Mask)X
1 f
2605(,)X
5 f
2667(Mod5Mask.)X
1 f
3159(You)X
3332(can)X
3476(also)X
3640(pass)X
3813(the)X
1131 5056(constant)N
5 f
1465(AnyModi\256er)X
1 f
1944(,)X
1988(which)X
2225(is)X
2306(equivalent)X
2696(to)X
2787(issuing)X
3059(the)X
3189(ungrab)X
3455(key)X
3604(request)X
3880(for)X
1131 5152(all)N
1242(possible)X
1553(modi\256er)X
1874(combinations)X
2372(\(including)X
2757(the)X
2887(combination)X
3351(of)X
3446(no)X
3556(modi\256ers\).)X
2 f
555 5276(ungrab_window)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(window)X
2550(associated)X
2934(with)X
3113(the)X
3243(keys)X
3426(you)X
3580(want)X
3773(to)X
1131 5372(ungrab.)N
555 5496(The)N
5 f
732(XUngrabKey)X
1 f
1268(function)X
1584(releases)X
1884(the)X
2014(key)X
2163(combination)X
2627(on)X
2737(the)X
2867(speci\256ed)X
3202(window)X
3507(if)X
3583(it)X
3655(was)X
555 5592(grabbed)N
860(by)X
970(this)X
1120(client.)X
1383(This)X
1562(function)X
1878(has)X
2017(no)X
2127(effect)X
2349(on)X
2459(an)X
2564(active)X
2797(grab.)X
3019(A)X
3104(modi\256ers)X
3459(of)X
5 f
570 5688(AnyModi\256er)N
1 f
1071(is)X
1152(equivalent)X
1542(to)X
1633(issuing)X
1905(the)X
2035(request)X
2311(for)X
2435(all)X
2546(possible)X
2857(modi\256er)X
3178(combinations)X
3676(\(includ-)X
555 5784(ing)N
690(the)X
820(combination)X
1284(of)X
1379(no)X
1489(modi\256ers\).)X
1917(A)X
2002(key)X
2151(of)X
5 f
2264(AnyKey)X
1 f
2605(is)X
2686(equivalent)X
3076(to)X
3167(issuing)X
3439(the)X
3569(request)X
3845(for)X
3 f
2217 6144(126)N

127 p
%%Page: 127 131
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(all)N
666(possible)X
977(nonmodi\256er)X
1430(key)X
1579(codes.)X
555 796(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XUngrabKey)X
1 f
2338(is)X
5 f
2437(BadWindow)X
1 f
2921(.)X
555 1016(Use)N
5 f
731(XAllowEvents)X
1 f
1306(to)X
1397(allow)X
1615(further)X
1876(events)X
2123(to)X
2214(be)X
2319(processed)X
2687(when)X
2899(the)X
3029(device)X
3281(has)X
3420(been)X
3608(frozen.)X
555 1112(The)N
714(de\256nition)X
1075(for)X
1199(this)X
1349(function)X
1665(is:)X
555 1236(XAllowEvents)N
1085(\()X
2 f
1121(display)X
1 f
1376(,)X
2 f
1420(event_mode)X
1 f
1847(,)X
2 f
1891(time)X
1 f
2050(\))X
687 1332(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1428(int)N
2 f
803(event_mode)X
1 f
1230(;)X
687 1524(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 1676(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1800(event_mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(event)X
1814(mode.)X
2076(You)X
2249(can)X
2393(pass)X
2566(one)X
2715(of)X
2810(these)X
3013(constants:)X
5 f
3406(AsyncPointer)X
1 f
3940(,)X
5 f
1146 1896(SyncPointer)N
1 f
1636(,)X
5 f
1698(AsyncKeyboard)X
1 f
2330(,)X
5 f
2392(SyncKeyboard)X
1 f
2980(,)X
5 f
3042(ReplayPointer)X
1 f
3609(,)X
5 f
3671(Replay-)X
1131 1992(Keyboard)N
1 f
1523(,)X
5 f
1585(AsyncBoth)X
1 f
2022(,)X
2066(or)X
5 f
2179(SyncBoth)X
1 f
2572(.)X
2 f
555 2116(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 2212(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 2336(The)N
5 f
732(XAllowEvents)X
1 f
1307(function)X
1623(releases)X
1923(some)X
2131(queued)X
2407(events)X
2654(if)X
2730(the)X
2860(client)X
3079(has)X
3218(caused)X
3479(a)X
3540(device)X
3792(to)X
555 2432(freeze.)N
835(The)X
994(function)X
1310(has)X
1449(no)X
1559(effect)X
1781(if)X
1857(the)X
1987(speci\256ed)X
2322(time)X
2502(is)X
2583(earlier)X
2830(than)X
3004(the)X
3134(last-grab)X
3464(time)X
3644(of)X
3739(the)X
555 2528(most)N
749(recent)X
986(active)X
1219(grab)X
1397(for)X
1521(the)X
1651(client,)X
1892(or)X
1987(if)X
2063(the)X
2193(speci\256ed)X
2528(time)X
2708(is)X
2789(later)X
2968(than)X
3142(the)X
3272(current)X
3543(X)X
3628(server)X
555 2624(time.)N
779(The)X
938(following)X
1303(describes)X
1652(the)X
1782(processing)X
2180(that)X
2335(occurs)X
2586(depending)X
2975(on)X
3085(what)X
3278(constant)X
3594(you)X
3748(pass)X
3921(to)X
555 2720(the)N
685(event_mode)X
1138(argument:)X
5 f
570 2864(AsyncPointer)N
1 f
1695(If)X
1775(the)X
1905(pointer)X
2177(is)X
2258(frozen)X
2504(by)X
2614(the)X
2744(client)X
2963(the)X
3093(pointer)X
3365(event)X
3578(processing)X
1695 2960(continues)N
2055(as)X
2150(usual.)X
2402(If)X
2482(the)X
2612(pointer)X
2884(is)X
2965(frozen)X
3211(twice)X
3424(by)X
3534(the)X
3664(client)X
3883(on)X
1695 3056(behalf)N
1937(of)X
2032(two)X
2186(separate)X
2496(grabs,)X
2730(AsyncPointer)X
3231(thaws)X
3458(for)X
3582(both.)X
1695 3152(AsyncPointer)N
2196(has)X
2335(no)X
2445(effect)X
2667(if)X
2743(the)X
2873(pointer)X
3145(is)X
3226(not)X
3361(frozen)X
3607(by)X
3717(the)X
1695 3248(client,)N
1936(but)X
2071(the)X
2201(pointer)X
2473(need)X
2661(not)X
2796(be)X
2901(grabbed)X
3206(by)X
3316(the)X
3446(client.)X
5 f
570 3392(SyncPointer)N
1 f
1695(If)X
1775(the)X
1905(pointer)X
2177(is)X
2258(frozen)X
2504(and)X
2653(actively)X
2955(grabbed)X
3260(by)X
3370(the)X
3500(client,)X
3741(pointer)X
1695 3488(event)N
1908(processing)X
2306(continues)X
2666(normally)X
3007(until)X
3192(the)X
3322(next)X
3496(ButtonPress)X
1695 3584(or)N
1790(ButtonRelease)X
2327(event)X
2540(is)X
2621(reported)X
2936(to)X
3027(the)X
3157(client.)X
3420(At)X
3530(this)X
3680(time,)X
3882(the)X
1695 3680(pointer)N
1967(again)X
2180(appears)X
2470(to)X
2561(freeze.)X
2841(However,)X
3206(if)X
3282(the)X
3412(reported)X
3727(event)X
1695 3776(causes)N
1946(the)X
2076(pointer)X
2348(grab)X
2526(to)X
2617(be)X
2722(released,)X
3054(the)X
3184(pointer)X
3456(does)X
3639(not)X
1695 3872(freeze.)N
1975(SyncPointer)X
2428(has)X
2567(no)X
2677(effect)X
2899(if)X
2975(the)X
3105(pointer)X
3377(is)X
3458(not)X
3593(frozen)X
3839(by)X
1695 3968(the)N
1825(client)X
2044(or)X
2139(if)X
2215(the)X
2345(pointer)X
2617(is)X
2698(not)X
2833(grabbed)X
3138(by)X
3248(the)X
3378(client.)X
5 f
570 4112(ReplayPointer)N
1 f
1695(If)X
1775(the)X
1905(pointer)X
2177(is)X
2258(actively)X
2560(grabbed)X
2865(by)X
2975(the)X
3105(client)X
3324(and)X
3473(is)X
3554(frozen)X
3800(as)X
1695 4208(the)N
1825(result)X
2043(of)X
2138(an)X
2243(event)X
2456(having)X
2718(been)X
2906(sent)X
3070(to)X
3161(the)X
3291(client)X
3510(\(either)X
3762(from)X
1695 4304(the)N
1825(activation)X
2196(of)X
2291(a)X
2352(XGrabButton)X
2853(or)X
2948(from)X
3141(a)X
3202(previous)X
3527(XAl-)X
1695 4400(lowEvents)N
2089(with)X
2268(mode)X
2486(\(SyncPointer,)X
2990(but)X
3125(not)X
3260(from)X
3453(a)X
3514(XGrab-)X
1695 4496(Pointer\),)N
2023(the)X
2153(pointer)X
2425(grab)X
2603(is)X
2684(released)X
2994(and)X
3143(that)X
3298(event)X
3511(is)X
3592(completely)X
1695 4592(reprocessed.)N
2175(This)X
2354(time,)X
2556(however,)X
2902(the)X
3032(function)X
3348(ignores)X
3629(any)X
3778(pas-)X
1695 4688(sive)N
1859(grabs)X
2071(at)X
2157(or)X
2252(above)X
2484(\(towards)X
2813(the)X
2943(root\))X
3136(the)X
3266(grab_window)X
3771(of)X
3866(the)X
1695 4784(grab)N
1873(just)X
2023(released.)X
2377(The)X
2536(request)X
2812(has)X
2951(no)X
3061(effect)X
3283(if)X
3359(the)X
3489(pointer)X
3761(is)X
3842(not)X
1695 4880(grabbed)N
2000(by)X
2110(the)X
2240(client)X
2459(or)X
2554(if)X
2630(the)X
2760(pointer)X
3032(is)X
3113(not)X
3248(frozen)X
3494(as)X
3589(the)X
3719(result)X
1695 4976(of)N
1790(an)X
1895(event.)X
5 f
570 5120(AsyncKeyboard)N
1 f
1695(If)X
1775(the)X
1905(keyboard)X
2254(is)X
2335(frozen)X
2581(by)X
2691(the)X
2821(client,)X
3062(the)X
3192(keyboard)X
3541(event)X
3754(pro-)X
1695 5216(cessing)N
1976(continues)X
2336(as)X
2431(usual.)X
2683(If)X
2763(the)X
2893(keyboard)X
3242(is)X
3323(frozen)X
3569(twice)X
3782(by)X
1695 5312(the)N
1825(client)X
2044(on)X
2154(behalf)X
2396(of)X
2491(two)X
2645(separate)X
2955(grabs,)X
3189(AsyncKeyboard)X
1695 5408(``thaws'')N
2038(for)X
2162(both.)X
2385(AsyncKeyboard)X
2977(has)X
3116(no)X
3226(effect)X
3448(if)X
3524(the)X
3654(keyboard)X
1695 5504(is)N
1776(not)X
1911(frozen)X
2157(by)X
2267(the)X
2397(client,)X
2638(but)X
2773(the)X
2903(keyboard)X
3252(need)X
3440(not)X
3575(be)X
3680(grabbed)X
1695 5600(by)N
1805(the)X
1935(client.)X
3 f
2217 6144(127)N

128 p
%%Page: 128 132
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
570 672(SyncKeyboard)N
1 f
1695(If)X
1775(the)X
1905(keyboard)X
2254(is)X
2335(frozen)X
2581(and)X
2730(actively)X
3032(grabbed)X
3337(by)X
3447(the)X
3577(client,)X
3818(key-)X
1695 768(board)N
1917(event)X
2130(processing)X
2528(continues)X
2888(as)X
2983(usual)X
3191(until)X
3376(the)X
3506(next)X
1695 864(KeyPress)N
2048(or)X
2143(KeyRelease)X
2585(event)X
2798(is)X
2879(reported)X
3194(to)X
3285(the)X
3415(client.)X
3678(At)X
3788(this)X
1695 960(time,)N
1897(the)X
2027(keyboard)X
2376(again)X
2589(appears)X
2879(to)X
2970(freeze.)X
3250(However,)X
3615(if)X
3691(the)X
1695 1056(reported)N
2010(event)X
2223(causes)X
2474(the)X
2604(keyboard)X
2953(grab)X
3131(to)X
3222(be)X
3327(released,)X
3659(the)X
3789(key-)X
1695 1152(board)N
1917(does)X
2100(not)X
2235(freeze.)X
2515(SyncKeyboard)X
3059(has)X
3198(no)X
3308(effect)X
3530(if)X
3606(the)X
3736(key-)X
1695 1248(board)N
1917(is)X
1998(not)X
2133(frozen)X
2379(by)X
2489(the)X
2619(client)X
2838(or)X
2933(if)X
3009(the)X
3139(keyboard)X
3488(is)X
3569(not)X
3704(grabbed)X
1695 1344(by)N
1805(the)X
1935(client.)X
5 f
570 1488(ReplayKeyboard)N
1 f
1695(If)X
1775(the)X
1905(keyboard)X
2254(is)X
2335(actively)X
2637(grabbed)X
2942(by)X
3052(the)X
3182(client)X
3401(and)X
3550(is)X
3631(frozen)X
3877(as)X
1695 1584(the)N
1825(result)X
2043(of)X
2138(an)X
2243(event)X
2456(having)X
2718(been)X
2906(sent)X
3070(to)X
3161(the)X
3291(client)X
3510(\(either)X
3762(from)X
1695 1680(the)N
1825(activation)X
2196(of)X
2291(a)X
2352(XGrabKey)X
2758(or)X
2853(from)X
3046(a)X
3107(previous)X
3432(XAllowEvents)X
1695 1776(with)N
1874(mode)X
2092(\(SyncKeyboard,)X
2687(but)X
2822(not)X
2957(from)X
3150(a)X
3211(XGrabKeyboard\),)X
3868(the)X
1695 1872(keyboard)N
2044(grab)X
2222(is)X
2303(released)X
2613(and)X
2762(that)X
2917(event)X
3130(is)X
3211(completely)X
3626(repro-)X
1695 1968(cessed.)N
1990(This)X
2169(time,)X
2371(however,)X
2717(the)X
2847(function)X
3163(ignores)X
3444(any)X
3593(passive)X
1695 2064(grabs)N
1907(at)X
1993(or)X
2088(above)X
2320(\(towards)X
2649(the)X
2779(root\))X
2972(the)X
3102(grab_window)X
3607(of)X
3702(the)X
3832(grab)X
1695 2160(just)N
1845(released.)X
2199(The)X
2358(request)X
2634(has)X
2773(no)X
2883(effect)X
3105(if)X
3181(the)X
3311(keyboard)X
3660(is)X
3741(not)X
1695 2256(grabbed)N
2000(by)X
2110(the)X
2240(client)X
2459(or)X
2554(if)X
2630(the)X
2760(keyboard)X
3109(is)X
3190(not)X
3325(frozen)X
3571(as)X
3666(the)X
3796(result)X
1695 2352(of)N
1790(an)X
1895(event.)X
5 f
570 2496(SyncBoth)N
1 f
1695(If)X
1775(both)X
1954(pointer)X
2226(and)X
2375(keyboard)X
2724(are)X
2853(frozen)X
3099(by)X
3209(the)X
3339(client,)X
3580(event)X
3793(pro-)X
1695 2592(cessing)N
1976(\(for)X
2129(both)X
2308(devices\))X
2623(continues)X
2983(normally)X
3324(until)X
3509(the)X
3639(next)X
3813(But-)X
1695 2688(tonPress,)N
2037(ButtonRelease,)X
2596(KeyPress,)X
2971(or)X
3066(KeyRelease)X
3508(event)X
3721(is)X
1695 2784(reported)N
2010(to)X
2101(the)X
2231(client)X
2450(for)X
2574(a)X
2635(grabbed)X
2940(device)X
3192(\(button)X
3469(event)X
3682(for)X
3806(the)X
1695 2880(pointer,)N
1989(key)X
2138(event)X
2351(for)X
2475(the)X
2605(keyboard\),)X
3005(at)X
3091(which)X
3328(time)X
3508(the)X
3638(devices)X
1695 2976(again)N
1908(appear)X
2164(to)X
2255(freeze.)X
2535(However,)X
2900(if)X
2976(the)X
3106(reported)X
3421(event)X
3634(causes)X
3885(the)X
1695 3072(grab)N
1873(to)X
1964(be)X
2069(released,)X
2401(then)X
2575(the)X
2705(devices)X
2991(do)X
3101(not)X
3236(freeze)X
3472(\(but)X
3636(if)X
3712(the)X
1695 3168(other)N
1898(device)X
2150(is)X
2231(still)X
2387(grabbed,)X
2714(then)X
2888(a)X
2949(subsequent)X
3362(event)X
3575(for)X
3699(it)X
3771(will)X
1695 3264(still)N
1851(cause)X
2068(both)X
2247(devices)X
2533(to)X
2624(freeze\).)X
2911(SyncBoth)X
3281(has)X
3420(no)X
3530(effect)X
3752(unless)X
1695 3360(both)N
1874(pointer)X
2146(and)X
2295(keyboard)X
2644(are)X
2773(frozen)X
3019(by)X
3129(the)X
3259(client.)X
3522(If)X
3602(the)X
3732(pointer)X
1695 3456(or)N
1790(keyboard)X
2139(is)X
2220(frozen)X
2466(twice)X
2679(by)X
2789(the)X
2919(client)X
3138(on)X
3248(behalf)X
3490(of)X
3585(two)X
1695 3552(separate)N
2005(grabs,)X
2239(SyncBoth)X
2609("thaws")X
2908(for)X
3032(both)X
3211(\(but)X
3375(a)X
3436(subsequent)X
1695 3648(freeze)N
1931(for)X
2055(SyncBoth)X
2425(will)X
2585(only)X
2764(freeze)X
3000(each)X
3183(device)X
3435(once\).)X
5 f
570 3792(AsyncBoth)N
1 f
1695(If)X
1775(the)X
1905(pointer)X
2177(and)X
2326(the)X
2456(keyboard)X
2805(are)X
2934(frozen)X
3180(by)X
3290(the)X
3420(client,)X
3661(event)X
1695 3888(processing)N
2093(\(for)X
2246(both)X
2425(devices\))X
2740(continues)X
3100(normally.)X
3485(If)X
3565(a)X
3626(device)X
3878(is)X
1695 3984(frozen)N
1941(twice)X
2154(by)X
2264(the)X
2394(client)X
2613(on)X
2723(behalf)X
2965(of)X
3060(two)X
3214(separate)X
3524(grabs,)X
1695 4080(AsyncBoth)N
2113("thaws")X
2412(for)X
2536(both.)X
2759(AsyncBoth)X
3177(has)X
3316(no)X
3426(effect)X
3648(unless)X
1695 4176(both)N
1874(pointer)X
2146(and)X
2295(keyboard)X
2644(are)X
2773(frozen)X
3019(by)X
3129(the)X
3259(client.)X
5 f
570 4348(AsyncPointer)N
1 f
1104(,)X
5 f
1166(SyncPointer)X
1 f
1656(,)X
1700(and)X
5 f
1867(ReplayPointer)X
1 f
2456(have)X
2644(no)X
2754(effect)X
2976(on)X
3086(the)X
3216(processing)X
3614(of)X
3709(key-)X
555 4444(board)N
777(events.)X
5 f
1089(AsyncKeyboard)X
1 f
1721(,)X
5 f
1783(SyncKeyboard)X
1 f
2371(,)X
2415(and)X
5 f
2582(ReplayKeyboard)X
1 f
3269(have)X
3457(no)X
3567(effect)X
3789(on)X
3899(the)X
555 4540(processing)N
953(of)X
1048(pointer)X
1320(events.)X
1611(It)X
1687(is)X
1768(possible)X
2079(for)X
2203(both)X
2382(a)X
2443(pointer)X
2715(grab)X
2893(and)X
3042(a)X
3103(keyboard)X
3452(grab)X
3630(\(by)X
3769(the)X
555 4636(same)N
758(or)X
853(different)X
1178(clients\))X
1460(to)X
1551(be)X
1656(active)X
1889(simultaneously.)X
2490(If)X
2570(a)X
2631(device)X
2883(is)X
2964(frozen)X
3210(on)X
3320(behalf)X
3562(of)X
3657(either)X
555 4732(grab,)N
755(no)X
865(event)X
1078(processing)X
1476(is)X
1557(performed)X
1945(for)X
2069(the)X
2199(device.)X
2495(It)X
2571(is)X
2652(possible)X
2963(for)X
3087(a)X
3148(single)X
3381(device)X
3633(to)X
3724(be)X
555 4828(frozen)N
801(because)X
1101(of)X
1196(both)X
1375(grabs.)X
1631(In)X
1726(this)X
1876(case,)X
2071(the)X
2201(freeze)X
2437(must)X
2631(be)X
2736(released)X
3046(on)X
3156(behalf)X
3398(of)X
3493(both)X
3672(grabs)X
555 4924(before)N
801(events)X
1048(can)X
1192(again)X
1405(be)X
1510(processed.)X
555 5048(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XAllowEvents)X
1 f
2377(is)X
5 f
2476(BadValue)X
1 f
2873(.)X
3 f
555 5240(7.6.)N
731(Grabbing)X
1121(the)X
1260(Server)X
1 f
555 5364(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(grab)X
2301(and)X
2450(ungrab)X
2716(the)X
2846(server.)X
3126(These)X
3358(functions)X
3708(can)X
3852(be)X
555 5460(used)N
738(to)X
829(control)X
1101(processing)X
1499(of)X
1594(output)X
1842(on)X
1952(other)X
2155(connections)X
2598(by)X
2708(the)X
2838(window)X
3143(system)X
3410(server.)X
3690(No)X
3819(pro-)X
555 5556(cessing)N
836(of)X
931(requests)X
1241(or)X
1336(close)X
1539(downs)X
1790(on)X
1900(any)X
2049(other)X
2252(connection)X
2661(will)X
2821(occur)X
3038(while)X
3256(the)X
3386(server)X
3622(is)X
3703(grabbed.)X
555 5652(A)N
640(client)X
859(closing)X
1136(its)X
1242(connection)X
1651 0.2604(automatically)AX
2155(ungrabs)X
2455(the)X
2585(server.)X
2865(Although)X
3220(grabbing)X
3555(the)X
3685(server)X
3921(is)X
555 5748(highly)N
803(discouraged,)X
1272(it)X
1344(is)X
1425(sometimes)X
1825(necessary.)X
3 f
2217 6144(128)N

129 p
%%Page: 129 133
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Use)N
5 f
731(XGrabServer)X
1 f
1282(to)X
1373(grab)X
1551(the)X
1681(server.)X
1939(The)X
2098(de\256nition)X
2459(for)X
2583(this)X
2733(function)X
3049(is:)X
555 796(XGrabServer)N
1029(\()X
2 f
1065(display)X
1 f
1327(\))X
687 892(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 1044(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 1168(The)N
5 f
732(XGrabServer)X
1 f
1283(function)X
1599(disables)X
1905(processing)X
2303(of)X
2398(requests)X
2708(and)X
2857(close-downs)X
3318(on)X
3428(all)X
3539(other)X
3742(con-)X
555 1264(nections)N
871(than)X
1045(the)X
1175(one)X
1324(this)X
1474(request)X
1750(arrived)X
2021(on.)X
2175(It)X
2251(is)X
2332(recommended)X
2853(that)X
3008(you)X
3162(not)X
3297(grab)X
3475(the)X
3605(X)X
3690(server)X
555 1360(any)N
704(more)X
907(than)X
1081(is)X
1162(absolutely)X
1547(necessary.)X
555 1580(Use)N
5 f
731(XUngrabServer)X
1 f
1374(to)X
1465(ungrab)X
1731(the)X
1861(server.)X
2119(The)X
2278(de\256nition)X
2639(for)X
2763(this)X
2913(function)X
3229(is:)X
555 1704(XUngrabServer)N
1117(\()X
2 f
1153(display)X
1 f
1415(\))X
687 1800(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 1952(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 2076(The)N
5 f
732(XUngrabServer)X
1 f
1375(function)X
1691(restarts)X
1967(processing)X
2365(of)X
2460(requests)X
2770(and)X
2919(close-downs)X
3380(on)X
3490(other)X
3693(connec-)X
555 2172(tions.)N
793(You)X
966(should)X
1223(avoid)X
1441(grabbing)X
1776(the)X
1906(server)X
2142(as)X
2237(much)X
2455(as)X
2550(possible.)X
3 f
555 2364(7.7.)N
731(Miscellaneous)X
1282(Control)X
1597(Function)X
1 f
555 2488(This)N
734(section)X
1006(discusses)X
1355(how)X
1528(to:)X
10 f
555 2612(g)N
1 f
775(Control)X
1067(the)X
1197(input)X
1401(focus)X
10 f
555 2736(g)N
1 f
775(Control)X
1067(the)X
1197(pointer)X
10 f
555 2860(g)N
1 f
775(Kill)X
935(clients)X
3 f
555 3052(7.7.1.)N
797(Controlling)X
1255(the)X
1394(Input)X
1626(Focus)X
1 f
555 3176(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(move)X
2341(the)X
2471(pointer)X
2743(position)X
3050(as)X
3145(well)X
3319(as)X
3414(set)X
3534(and)X
3683(get)X
3813(the)X
555 3272(input)N
759(focus.)X
555 3492(Use)N
5 f
731(XWarpPointer)X
1 f
1316(to)X
1407(move)X
1625(the)X
1755(pointer)X
2027(to)X
2118(an)X
2223(arbitrary)X
2548(point)X
2752(on)X
2862(the)X
2992(screen.)X
3260(The)X
3419(de\256nition)X
3780(for)X
555 3588(this)N
705(function)X
1021(is:)X
555 3712(XWarpPointer)N
1075(\()X
2 f
1111(display)X
1 f
1366(,)X
2 f
1410(src_w)X
1 f
1627(,)X
2 f
1671(dest_w)X
1 f
1923(,)X
2 f
1967(src_x)X
1 f
2164(,)X
2 f
2208(src_y)X
1 f
2405(,)X
2 f
2449(src_width)X
1 f
2804(,)X
2 f
2848(src_height)X
1 f
3227(,)X
2 f
3271(dest_x)X
1 f
3503(,)X
2 f
907 3808(dest_y)N
1 f
1139(\))X
731 3904(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 4000(Window)N
2 f
1056(src_w)X
1 f
1273(,)X
2 f
1317(dest_w)X
1 f
1569(;)X
731 4096(int)N
2 f
847(src_x)X
1 f
1044(,)X
2 f
1088(src_y)X
1 f
1285(;)X
731 4192(unsigned)N
1071(int)X
2 f
1187(src_width)X
1 f
1542(,)X
2 f
1586(src_height)X
1 f
1965(;)X
731 4288(int)N
2 f
847(dest_x)X
1 f
1079(,)X
2 f
1123(dest_y)X
1 f
1355(;)X
2 f
555 4440(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4564(src_w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(source)X
2496(window.)X
2845(You)X
3018(can)X
3162(pass)X
3335(the)X
3465(window)X
3770(ID)X
3884(or)X
1131 4660(the)N
1261(constant)X
5 f
1595(None)X
1 f
1820(.)X
2 f
555 4784(dest_w)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID)X
2020(of)X
2115(the)X
2245(destination)X
2655(window.)X
3004(You)X
3177(can)X
3321(pass)X
3494(the)X
3624(window)X
1131 4880(ID)N
1245(or)X
1340(the)X
1470(constant)X
5 f
1804(None)X
1 f
2029(.)X
2 f
555 5004(src_x)N
555 5100(src_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(within)X
2509(the)X
2639(source)X
2890(window.)X
2 f
555 5224(src_width)N
555 5320(src_height)N
1 f
1131(Specify)X
1422(the)X
1552(width)X
1775(and)X
1924(height)X
2167(of)X
2262(the)X
2392(source)X
2643(window.)X
2 f
555 5444(dest_x)N
555 5540(dest_y)N
1 f
1131(Specify)X
1422(the)X
1552(x)X
1618(and)X
1767(y)X
1833(coordinates)X
2261(within)X
2509(the)X
2639(destination)X
3049(window.)X
555 5664(The)N
5 f
732(XWarpPointer)X
1 f
1317(function)X
1633(moves)X
1885(the)X
2015(pointer)X
2287(to)X
2378(the)X
2508(coordinates)X
2936(speci\256ed)X
3271(by)X
3381(the)X
3511(dest_x)X
3763(and)X
555 5760(dest_y)N
807(arguments,)X
1218(relative)X
1505(to)X
1596(the)X
1726(destination)X
2136(window's)X
2504(origin.)X
2781(If)X
2861(the)X
2991(destination)X
3401(window)X
3706(is)X
3 f
2217 6144(129)N

130 p
%%Page: 130 134
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
570 672(None)N
1 f
795(,)X
839(the)X
969(pointer)X
1241(is)X
1322(moved)X
1584(by)X
1694(offsets)X
1950(speci\256ed)X
2285(by)X
2395(the)X
2525(dest_x)X
2777(and)X
2926(dest_y)X
3178(coordinates.)X
3650(There)X
3877(is)X
555 768(seldom)N
832(any)X
981(reason)X
1232(for)X
1356(calling)X
1619(this)X
1769(function.)X
2107(The)X
2266(pointer)X
2538(should)X
2795(normally)X
3136(be)X
3241(left)X
3381(to)X
3472(the)X
3602(mouse.)X
3898(If)X
555 864(you)N
709(do)X
819(use)X
958(this)X
1108(function,)X
1446(however,)X
1792(it)X
1864(generates)X
2218(events)X
2465(just)X
2615(as)X
2710(if)X
2786(the)X
2916(user)X
3084(had)X
3233(instantaneously)X
555 960(moved)N
817(the)X
947(pointer)X
1219(from)X
1412(one)X
1561(position)X
1868(to)X
1959(another.)X
2289(Note)X
2482(that)X
2637(you)X
2791(cannot)X
3048(use)X
5 f
3205(XWarpPointer)X
1 f
3790(to)X
555 1056(move)N
773(the)X
903(pointer)X
1175(outside)X
1452(the)X
1582(con\256ne_to)X
1976(window)X
2281(of)X
2376(an)X
2481(active)X
2714(pointer)X
2986(grab.)X
3208(An)X
3337(attempt)X
3625(to)X
3716(do)X
3826(so)X
555 1152(will)N
715(only)X
894(move)X
1112(the)X
1242(pointer)X
1514(as)X
1609(far)X
1728(as)X
1823(the)X
1953(closest)X
2215(edge)X
2403(of)X
2498(the)X
2628(con\256ne_to)X
3022(window.)X
555 1276(If)N
635(the)X
765(src_w)X
996(argument)X
1351(is)X
5 f
1450(None)X
1 f
1675(,)X
1719(the)X
1849(move)X
2067(is)X
2148(independent)X
2601(of)X
2696(the)X
2826(current)X
3097(pointer)X
3369(position.)X
3720(How-)X
555 1372(ever,)N
750(if)X
826(src_w)X
1057(is)X
1138(a)X
1199(window,)X
1526(the)X
1656(move)X
1874(only)X
2053(takes)X
2256(place)X
2464(if)X
2540(the)X
2670(pointer)X
2942(is)X
3023(currently)X
3363(contained)X
3728(in)X
3819(a)X
555 1468(visible)N
813(portion)X
1090(of)X
1185(the)X
1315(speci\256ed)X
1650(rectangle)X
1995(of)X
2090(the)X
2220(src_w.)X
2495(If)X
2575(dest_w)X
2846(is)X
5 f
2945(None)X
1 f
3170(,)X
3214(the)X
3344(function)X
3660(moves)X
555 1564(the)N
685(pointer)X
957(by)X
1067(offsets)X
1323(\(dest_x,)X
1626(dest_y\))X
1907(relative)X
2194(to)X
2285(the)X
2415(current)X
2686(position)X
2993(of)X
3088(the)X
3218(pointer.)X
3534(If)X
3614(dest_w)X
3885(is)X
3966(a)X
555 1660(window,)N
882(the)X
1012(function)X
1328(moves)X
1580(the)X
1710(pointer)X
1982(to)X
2073(\(dest_x,)X
2376(dest_y\))X
2657(relative)X
2944(to)X
3035(the)X
3165(origin)X
3398(of)X
3493(dest_w.)X
3808(How-)X
555 1756(ever,)N
750(if)X
826(src_w)X
1057(is)X
1138(not)X
5 f
1291(None)X
1 f
1516(,)X
1560(the)X
1690(move)X
1908(only)X
2087(takes)X
2290(place)X
2498(if)X
2574(the)X
2704(pointer)X
2976(is)X
3057(currently)X
3397(contained)X
3762(in)X
3853(a)X
555 1852(visible)N
813(portion)X
1090(of)X
1185(the)X
1315(speci\256ed)X
1650(rectangle)X
1995(of)X
2090(the)X
2220(src_w.)X
555 1976(The)N
714(coordinates)X
1142(passed)X
1398(to)X
1489(src_x)X
1701(and)X
1850(src_y)X
2062(are)X
2191(relative)X
2478(to)X
2569(the)X
2699(source)X
2950(window's)X
3318(origin.)X
3595(If)X
555 2072(src_height)N
944(is)X
1025(zero)X
1198(\(0\),)X
1344(the)X
1474(function)X
1790(replaces)X
2100(it)X
2172(with)X
2351(the)X
2481(current)X
2752(height)X
2995(of)X
3090(the)X
3220(source)X
3471(window)X
3776(minus)X
555 2168(src_y.)N
811(If)X
891(src_width)X
1260(is)X
1341(zero)X
1514(\(0\),)X
1660(the)X
1790(function)X
2106(replaces)X
2416(it)X
2488(with)X
2667(the)X
2797(current)X
3068(width)X
3291(of)X
3386(the)X
3516(source)X
3767(win-)X
555 2264(dow)N
728(minus)X
966(src_x.)X
555 2388(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XWarpPointer)X
1 f
2387(is)X
5 f
2486(BadWindow)X
1 f
2970(.)X
555 2608(Use)N
5 f
731(XSetInputFocus)X
1 f
1397(to)X
1488(set)X
1608(the)X
1738(input)X
1942(focus.)X
2176(The)X
2335(de\256nition)X
2696(for)X
2820(this)X
2970(function)X
3286(is:)X
555 2732(XSetInputFocus)N
1134(\()X
2 f
1170(display)X
1 f
1425(,)X
2 f
1469(focus)X
1 f
1662(,)X
2 f
1706(revert_to)X
1 f
2036(,)X
2 f
2080(time)X
1 f
2239(\))X
687 2828(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2924(Window)N
2 f
1012(focus)X
1 f
1205(;)X
687 3020(int)N
2 f
803(revert_to)X
1 f
1133(;)X
687 3116(Time)N
2 f
896(time)X
1 f
1055(;)X
2 f
555 3268(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3392(focus)N
1 f
1131(Speci\256es)X
1471(the)X
1601(window)X
1906(ID.)X
2064(This)X
2243(is)X
2324(the)X
2454(window)X
2759(in)X
2850(which)X
3087(you)X
3241(want)X
3434(to)X
3525(set)X
3645(the)X
3775(input)X
1131 3488(focus.)N
1387(You)X
1560(can)X
1704(pass)X
1877(the)X
2007(window)X
2312(ID)X
2426(or)X
2521(one)X
2670(of)X
2765(the)X
2895(constants)X
5 f
3263(PointerRoot)X
1 f
3765(or)X
5 f
1146 3584(None)N
1 f
1371(.)X
2 f
555 3708(revert_to)N
1 f
1131(Speci\256es)X
1471(which)X
1708(window)X
2013(the)X
2143(input)X
2347(focus)X
2559(reverts)X
2820(to)X
2911(if)X
2987(the)X
3117(window)X
3422(becomes)X
3752(not)X
1131 3804(viewable.)N
1515(You)X
1688(can)X
1832(pass)X
2005(one)X
2154(of)X
2249(these)X
2452(constants:)X
5 f
2845(RevertToParent)X
1 f
3482(,)X
5 f
3544(RevertTo-)X
1131 3900(PointerRoot)N
1 f
1611(,)X
1655(or)X
5 f
1768(RevertToNone)X
1 f
2355(.)X
2 f
555 4024(time)N
1 f
1131(Speci\256es)X
1471(the)X
1601(time.)X
1825(You)X
1998(can)X
2142(pass)X
2315(either)X
2538(a)X
2599(timestamp,)X
3012(expressed)X
3380(in)X
3471(milliseconds,)X
1131 4120(or)N
1226(the)X
1356(constant)X
5 f
1690(CurrentTime)X
1 f
2193(.)X
555 4244(The)N
5 f
732(XSetInputFocus)X
1 f
1398(function)X
1714(changes)X
2019(the)X
2149(input)X
2353(focus)X
2565(and)X
2714(the)X
2844(last-focus-change)X
3486(time.)X
3710(The)X
555 4340(function)N
871(has)X
1010(no)X
1120(effect)X
1342(if)X
1418(the)X
1548(speci\256ed)X
1883(time)X
2063(is)X
2144(earlier)X
2391(than)X
2565(the)X
2695(current)X
2966(last-focus-change)X
3608(time)X
3788(or)X
3883(is)X
555 4436(later)N
734(than)X
908(the)X
1038(current)X
1309(X)X
1394(server)X
1630(time.)X
1854(Otherwise,)X
2259(the)X
2389(last-focus-change)X
3031(time)X
3211(is)X
3292(set)X
3412(to)X
3503(the)X
3633(speci\256ed)X
555 4532(time)N
735(and)X
884(the)X
5 f
1032(CurrentTime)X
1 f
1557(replaced)X
1877(by)X
1987(the)X
2117(current)X
2388(X)X
2473(server)X
2709(time.)X
2933(This)X
3112(function)X
3428(causes)X
3679(the)X
3809(X)X
555 4628(server)N
791(to)X
882(generate)X
5 f
1220(FocusIn)X
1 f
1571(and)X
5 f
1738(FocusOut)X
1 f
2158(events.)X
555 4752(Depending)N
963(on)X
1073(what)X
1266(value)X
1479(you)X
1633(assign)X
1875(to)X
1966(the)X
2096(focus)X
2308(argument,)X
5 f
2703(XSetInputFocus)X
1 f
3369(executes)X
3694(as)X
3789(fol-)X
555 4848(lows:)N
10 f
555 4972(g)N
1 f
775(If)X
855(you)X
1009(assign)X
5 f
1269(None)X
1 f
1516(to)X
1607(the)X
1737(focus)X
1949(argument,)X
2326(all)X
2437(keyboard)X
2786(events)X
3033(are)X
3162(discarded)X
3521(until)X
3706(you)X
3860(set)X
775 5068(a)N
836(new)X
1004(focus)X
1216(window.)X
1543(In)X
1638(this)X
1788(case,)X
1983(the)X
2113(revert_to)X
2453(argument)X
2808(is)X
2889(ignored.)X
10 f
555 5192(g)N
1 f
775(If)X
855(you)X
1009(assign)X
1251(a)X
1312(window)X
1617(ID)X
1731(to)X
1822(the)X
1952(focus)X
2164(argument,)X
2541(it)X
2613(becomes)X
2943(the)X
3073(keyboard's)X
3485(focus)X
3697(window.)X
775 5288(If)N
855(a)X
916(generated)X
1280(keyboard)X
1629(event)X
1842(would)X
2084(normally)X
2425(be)X
2530(reported)X
2845(to)X
2936(this)X
3086(window)X
3391(or)X
3486(one)X
3635(of)X
3730(its)X
3836(infe-)X
775 5384(riors,)N
980(the)X
1110(event)X
1323(is)X
1404(reported)X
1719(normally.)X
2082(Otherwise,)X
2487(the)X
2617(event)X
2830(is)X
2911(reported)X
3226(relative)X
3513(to)X
3604(the)X
3734(focus)X
775 5480(window.)N
10 f
555 5604(g)N
1 f
775(If)X
855(you)X
1009(assign)X
5 f
1269(PointerRoot)X
1 f
1771(to)X
1862(the)X
1992(focus)X
2204(argument,)X
2581(the)X
2711(focus)X
2923(window)X
3228(is)X
3309(dynamically)X
3768(taken)X
775 5700(to)N
866(be)X
971(the)X
1101(root)X
1265(window)X
1570(of)X
1665(whatever)X
2009(screen)X
2255(the)X
2385(pointer)X
2657(is)X
2738(on)X
2848(at)X
2934(each)X
3117(keyboard)X
3466(event.)X
3701(In)X
3796(this)X
775 5796(case,)N
970(the)X
1100(revert_to)X
1440(argument)X
1795(is)X
1876(ignored.)X
3 f
2217 6144(130)N

131 p
%%Page: 131 135
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(speci\256ed)X
1049(focus)X
1261(window)X
1566(must)X
1760(be)X
1865(viewable)X
2205(at)X
2291(the)X
2421(time)X
5 f
2619(XSetInputFocus)X
1 f
3285(is)X
3366(called.)X
3643(Other-)X
555 768(wise,)N
760(a)X
5 f
839(BadMatch)X
1 f
1273(error)X
1465(is)X
1546(generated.)X
1954(If)X
2034(the)X
2164(focus)X
2376(window)X
2681(later)X
2860(becomes)X
3190(not)X
3325(viewable,)X
3687(the)X
3817(X)X
555 864(server)N
791(evaluates)X
1141(the)X
1271(revert_to)X
1611(argument)X
1966(to)X
2057(determine)X
2432(the)X
2562(new)X
2730(focus)X
2942(window:)X
10 f
555 988(g)N
1 f
775(If)X
855(you)X
1009(assign)X
5 f
1269(RevertToParent)X
1 f
1928(to)X
2019(the)X
2149(revert_to)X
2489(argument,)X
2866(the)X
2996(focus)X
3208(reverts)X
3469(to)X
3560(the)X
3690(parent)X
775 1084(\(or)N
899(the)X
1029(closest)X
1291(viewable)X
1631(ancestor\),)X
1997(and)X
2146(the)X
2276(new)X
2444(revert_to)X
2784(value)X
2997(is)X
3078(taken)X
3291(to)X
3382(be)X
5 f
3505(RevertTo-)X
775 1180(None)N
1 f
1000(.)X
10 f
555 1304(g)N
1 f
775(If)X
855(you)X
1009(assign)X
5 f
1269(RevertToPointerRoot)X
1 f
2133(or)X
5 f
2246(RevertToNone)X
1 f
2855(to)X
2946(the)X
3076(revert_to)X
3416(argument,)X
3793(the)X
775 1400(focus)N
987(reverts)X
1248(to)X
1339(that)X
1494(value.)X
1751(When)X
1983(the)X
2113(focus)X
2325(reverts,)X
2608(the)X
2738(X)X
2823(server)X
3059(generates)X
5 f
3431(FocusIn)X
1 f
3782(and)X
5 f
790 1496(FocusOut)N
1 f
1210(events,)X
1479(but)X
1614(the)X
1744(last-focus-change)X
2386(time)X
2566(is)X
2647(not)X
2782(affected.)X
555 1620(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetInputFocus)X
1 f
2502(are)X
5 f
2649(BadWindow)X
1 f
3155(and)X
5 f
3322(BadValue)X
1 f
3719(.)X
555 1840(Use)N
5 f
731 0.2452(XGetInputFocus)AX
1 f
1407(to)X
1498(obtain)X
1741(the)X
1871(current)X
2142(input)X
2346(focus.)X
2580(The)X
2739(de\256nition)X
3100(for)X
3224(this)X
3374(function)X
3690(is:)X
555 1964(XGetInputFocus)N
1148(\()X
2 f
1184(display)X
1 f
1439(,)X
2 f
1483(focus_return)X
1 f
1940(,)X
2 f
1984(revert_to_return)X
1 f
2578(\))X
687 2060(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2156(Window)N
1012(*)X
2 f
(focus_return)S
1 f
1513(;)X
687 2252(int)N
803(*)X
2 f
(revert_to_return)S
1 f
1441(;)X
2 f
555 2404(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2528(focus_return)N
1 f
1131(Returns)X
1427(the)X
1557(focus)X
1769(window)X
2074(ID,)X
2210(the)X
2340(constant)X
5 f
2674(PointerRoot)X
1 f
3154(,)X
3198(or)X
3293(the)X
3423(constant)X
5 f
3757(None)X
1 f
3982(.)X
2 f
555 2652(revert_to_return)N
1 f
1142(Returns)X
1438(the)X
1568(current)X
1839(focus)X
2051(state.)X
2279(The)X
2438(function)X
2754(can)X
2898(return)X
3130(one)X
3279(of)X
3374(these)X
3577(constants:)X
5 f
1146 2748(RevertToParent)N
1 f
1783(,)X
5 f
1845(RevertToPointerRoot)X
1 f
2687(,)X
2731(or)X
5 f
2844(RevertToNone)X
1 f
3431(.)X
555 2872(The)N
5 f
732 0.2452(XGetInputFocus)AX
1 f
1408(function)X
1724(returns)X
1990(the)X
2120(focus)X
2332(window)X
2637(ID)X
2751(and)X
2900(the)X
3030(current)X
3301(focus)X
3513(state.)X
3 f
555 3064(7.7.2.)N
797(Controlling)X
1255(the)X
1394(Pointer)X
1 f
555 3188(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(change)X
2394(the)X
2524(pointer)X
2796(control)X
3068(or)X
3163(can)X
3307(get)X
3437(the)X
3567(current)X
555 3284(pointer)N
827(control)X
1099(parameters.)X
555 3504(Use)N
5 f
731(XChangePointerControl)X
1 f
1697(to)X
1788(control)X
2060(the)X
2190(interactive)X
2585(feel)X
2739(of)X
2834(the)X
2964(pointer)X
3236(device.)X
3532(The)X
555 3600(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 3724(XChangePointerControl)N
1419(\()X
2 f
1455(display)X
1 f
1710(,)X
2 f
1754(do_accel)X
1 f
2079(,)X
2 f
2123(do_threshold)X
1 f
2595(,)X
2 f
2639(accel_numerator)X
1 f
3247(,)X
2 f
1083 3820(accel_denominator)N
1 f
1770(,)X
2 f
1814(threshold)X
1 f
2154(\))X
687 3916(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4012(Bool)N
2 f
881(do_accel)X
1 f
1206(,)X
2 f
1250(do_threshold)X
1 f
1722(;)X
687 4108(int)N
2 f
803(accel_numerator)X
1 f
1411(,)X
2 f
1455(accel_denominator)X
1 f
2142(;)X
687 4204(int)N
2 f
803(threshold)X
1 f
1143(;)X
2 f
555 4356(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4480(do_accel)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(that)X
2201(controls)X
2507(whether)X
2812(the)X
2942(values)X
3189(for)X
3313(the)X
1131 4576(accel_numerator)N
1740(or)X
1835(accel_denominator)X
2528(are)X
2657(set.)X
2821(You)X
2994(can)X
3138(pass)X
3311(one)X
3460(of)X
3555(these)X
3758(con-)X
1131 4672(stants:)N
5 f
1397(True)X
1 f
1615(or)X
5 f
1728(False)X
1 f
1958(.)X
2 f
555 4796(do_threshold)N
1 f
1131(Speci\256es)X
1471(a)X
1532(boolean)X
1833(value)X
2046(that)X
2201(controls)X
2507(whether)X
2812(the)X
2942(value)X
3155(for)X
3279(the)X
3409(accel_numerator)X
1131 4892(or)N
1226(accel_denominator)X
1919(are)X
2048(set.)X
2212(You)X
2385(can)X
2529(pass)X
2702(one)X
2851(of)X
2946(these)X
3149(constants:)X
5 f
3542(True)X
1 f
3760(or)X
5 f
1146 4988(False)N
1 f
1376(.)X
2 f
555 5112(accel_numerator)N
1 f
1156(Speci\256es)X
1496(the)X
1626(numerator)X
2010(for)X
2134(the)X
2264(acceleration)X
2712(multiplier.)X
2 f
555 5236(accel_denominator)N
1 f
1131 5332(Speci\256es)N
1471(the)X
1601(denominator)X
2069(for)X
2193(the)X
2323(acceleration)X
2771(multiplier.)X
2 f
555 5456(threshold)N
1 f
1131(Speci\256es)X
1471(the)X
1601(acceleration)X
2049(threshold.)X
555 5580(The)N
5 f
732(XChangePointerControl)X
1 f
1698(function)X
2014(de\256nes)X
2285(how)X
2458(the)X
2588(pointing)X
2905(device)X
3157(moves.)X
3453(The)X
3612(accelera-)X
555 5676(tion,)N
737(expressed)X
1105(as)X
1200(a)X
1261(fraction,)X
1579(is)X
1660(a)X
1721(multiplier)X
2093(for)X
2217(movement.)X
2634(For)X
2778(example,)X
3121(specifying)X
3510(3/1)X
3645(means)X
3892(the)X
555 5772(pointer)N
827(moves)X
1079(three)X
1277(times)X
1491(as)X
1586(fast)X
1735(as)X
1830(normal.)X
2146(The)X
2305(fraction)X
2601(may)X
2775(be)X
2880(rounded)X
3190(arbitrarily)X
3565(by)X
3675(the)X
3805(X)X
3 f
2217 6144(131)N

132 p
%%Page: 132 136
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(server.)N
813(Acceleration)X
1285(only)X
1464(takes)X
1667(effect)X
1889(if)X
1965(the)X
2095(pointer)X
2367(moves)X
2619(more)X
2822(than)X
2996(threshold)X
3346(pixels)X
3579(at)X
3665(once)X
3853(and)X
555 768(only)N
734(applies)X
1006(to)X
1097(the)X
1227(amount)X
1514(beyond)X
1795(the)X
1925(value)X
2138(in)X
2229(the)X
2359(threshold)X
2709(argument.)X
3108(Setting)X
3381(a)X
3442(value)X
3655(to)X
3746(-1)X
555 864(restores)N
850(the)X
980(default.)X
1291(The)X
1450(values)X
1697(of)X
1792(the)X
1922(do_accel)X
2257(and)X
2406(do_threshold)X
2888(arguments)X
3277(must)X
3471(be)X
3576(nonzero)X
3881(for)X
555 960(the)N
685(pointer)X
957(values)X
1204(to)X
1295(be)X
1400(set.)X
1564(Otherwise,)X
1969(the)X
2099(parameters)X
2507(will)X
2667(be)X
2772(unchanged.)X
3219(Negative)X
3559(values)X
3806(gen-)X
555 1056(erate)N
748(a)X
5 f
827(BadValue)X
1 f
1246(error,)X
1460(as)X
1555(does)X
1738(a)X
1799(zero)X
1972(value)X
2185(for)X
2309(the)X
2439(accel_denominator)X
3132(argument.)X
555 1180(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XChangePointerControl)X
1 f
2768(is)X
5 f
2867(BadValue)X
1 f
3264(.)X
555 1400(Use)N
5 f
731(XGetPointerControl)X
1 f
1532(to)X
1623(get)X
1753(the)X
1883(current)X
2154(parameters)X
2562(set)X
2682(for)X
2806(the)X
2936(pointer.)X
3230(The)X
3389(de\256nition)X
3750(for)X
3874(this)X
555 1496(function)N
871(is:)X
555 1620(XGetPointerControl)N
1277(\()X
2 f
1313(display)X
1 f
1568(,)X
2 f
1612(accel_numerator_return)X
1 f
2484(,)X
2 f
2528(accel_denominator_return)X
1 f
3479(,)X
2 f
1061 1716(threshold_return)N
1 f
1665(\))X
687 1812(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1908(int)N
803(*)X
2 f
(accel_numerator_return)S
1 f
1719(,)X
1763(*)X
2 f
(accel_denominator_return)S
1 f
2758(;)X
687 2004(int)N
803(*)X
2 f
(threshold_return)S
1 f
1451(;)X
2 f
555 2156(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2280(accel_numerator_return)N
1 f
1131 2376(Returns)N
1427(the)X
1557(numerator)X
1941(for)X
2065(the)X
2195(acceleration)X
2643(multiplier.)X
2 f
555 2500(accel_denominator_return)N
1 f
1131 2596(Returns)N
1427(the)X
1557(denominator)X
2025(for)X
2149(the)X
2279(acceleration)X
2727(multiplier.)X
2 f
555 2720(threshold_return)N
1 f
1152(Returns)X
1448(the)X
1578(acceleration)X
2026(threshold.)X
555 2844(The)N
5 f
732(XGetPointerControl)X
1 f
1533(function)X
1849(returns)X
2115(the)X
2245(pointer's)X
2580(current)X
2851(acceleration)X
3299(multiplier)X
3671(and)X
555 2940(acceleration)N
1003(threshold.)X
3 f
555 3132(7.7.3.)N
797(Killing)X
1081(Clients)X
1 f
555 3256(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(control)X
2395(the)X
2525(life)X
2665(time)X
2845(of)X
2940(resources)X
3293(owned)X
3549(by)X
3659(a)X
3720(client)X
555 3352(or)N
650(can)X
794(cause)X
1011(the)X
1141(connection)X
1550(to)X
1641(a)X
1702(client)X
1921(to)X
2012(be)X
2117(destroyed.)X
555 3572(Use)N
5 f
731(XSetCloseDownMode)X
1 f
1628(to)X
1719(change)X
1990(the)X
2120(close)X
2323(down)X
2540(mode)X
2758(of)X
2853(a)X
2914(client.)X
3155(The)X
3314(de\256nition)X
3675(for)X
3799(this)X
555 3668(function)N
871(is:)X
555 3792(XSetCloseDownMode)N
1358(\()X
2 f
1394(display)X
1 f
1649(,)X
2 f
1693(close_mode)X
1 f
2115(\))X
687 3888(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3984(int)N
2 f
803(close_mode)X
1 f
1225(;)X
2 f
555 4136(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4260(close_mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(client)X
1820(close)X
2023(down)X
2240(mode)X
2458(you)X
2612(want)X
2805(to)X
2896(change.)X
3211(You)X
3384(can)X
3528(pass)X
3701(one)X
3850(of)X
1131 4356(these)N
1334(constants:)X
5 f
1727(DestroyAll)X
1 f
2142(,)X
5 f
2204(RetainPermanent)X
1 f
2904(,)X
2948(or)X
5 f
3061(RetainTemporary)X
1 f
3755(.)X
555 4480(The)N
5 f
732(XSetCloseDownMode)X
1 f
1629(de\256nes)X
1900(what)X
2093(will)X
2253(happen)X
2529(to)X
2620(the)X
2750(client's)X
3032(resources)X
3385(at)X
3471(connection)X
555 4576(close.)N
802(A)X
887(connection)X
1296(starts)X
1504(in)X
5 f
1613(DestroyAll)X
1 f
2050(mode.)X
2312(See)X
2461(the)X
2591(Section)X
2878(2.6)X
3010(for)X
3134(information)X
3573(on)X
3683(what)X
555 4672(happens)N
865(to)X
956(the)X
1086(client's)X
1368(resources)X
1721(when)X
1933(the)X
2063(close_mode)X
2506(argument)X
2861(is)X
2942(one)X
3091(of)X
3186(the)X
3316(valid)X
3515(constants.)X
555 4796(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XSetCloseDownMode)X
1 f
2699(is)X
5 f
2798(BadValue)X
1 f
3195(.)X
555 5016(Use)N
5 f
731(XKillClient)X
1 f
1167(to)X
1258(destroy)X
1539(a)X
1600(client.)X
1841(The)X
2000(de\256nition)X
2361(for)X
2485(this)X
2635(function)X
2951(is:)X
555 5140(XKillClient)N
980(\()X
2 f
1016(display)X
1 f
1271(,)X
2 f
1315(resource)X
1 f
1629(\))X
687 5236(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5332(XID)N
2 f
864(resource)X
1 f
1178(;)X
2 f
555 5484(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5608(resource)N
1 f
1131(Speci\256es)X
1471(any)X
1620(resource)X
1939(associated)X
2323(with)X
2502(the)X
2632(client)X
2851(you)X
3005(want)X
3198(to)X
3289(destroy.)X
3614(You)X
3787(can)X
1131 5704(also)N
1295(pass)X
1468(the)X
1598(constant)X
5 f
1932(AllTemporary)X
1 f
2469(.)X
3 f
2217 6144(132)N

133 p
%%Page: 133 137
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
5 f
732(XKillClient)X
1 f
1168(function)X
1484(forces)X
1720(a)X
1781(close-down)X
2208(of)X
2303(the)X
2433(client)X
2652(that)X
2807(created)X
3083(the)X
3213(resource)X
3532(if)X
3608(a)X
3669(valid)X
555 768(resource)N
874(is)X
955(speci\256ed.)X
1334(If)X
1414(the)X
1544(client)X
1763(has)X
1902(already)X
2183(terminated)X
2583(in)X
2674(either)X
5 f
2915(RetainPermanent)X
1 f
3637(or)X
5 f
570 864(RetainTemporary)N
1 f
1286(mode,)X
1526(all)X
1637(of)X
1732(the)X
1862(client's)X
2144(resources)X
2497(are)X
2626(destroyed.)X
3034(If)X
5 f
3132(AllTemporary)X
1 f
3691(is)X
555 960(speci\256ed,)N
912(the)X
1042(resources)X
1395(of)X
1490(all)X
1601(clients)X
1854(that)X
2009(have)X
2197(terminated)X
2597(in)X
5 f
2706(RetainTemporary)X
1 f
3422(are)X
3551(destroyed.)X
555 1056(See)N
704(Section)X
991(2.6)X
1123(for)X
1247(further)X
1508(information.)X
555 1180(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XKillClient)X
1 f
2238(is)X
5 f
2337(BadValue)X
1 f
2734(.)X
3 f
555 1372(7.8.)N
731(Manipulating)X
1268(Keyboard)X
1667(Settings)X
1 f
555 1496(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(change)X
2394(the)X
2524(keyboard)X
2873(control,)X
3167(obtain)X
3410(a)X
3471(list)X
3602(of)X
3697(the)X
555 1592(auto-repeat)N
973(keys,)X
1178(turn)X
1342(keyboard)X
1691(auto-repeat)X
2109(on)X
2219(or)X
2314(off,)X
2460(ring)X
2624(the)X
2754(bell,)X
2931(set)X
3051(or)X
3146(obtain)X
3389(the)X
3519(pointer)X
3791(but-)X
555 1688(ton)N
690(or)X
785(keyboard)X
1134(mapping,)X
1487(and)X
1636(obtain)X
1879(a)X
1940(bit)X
2056(vector)X
2298(for)X
2422(the)X
2552(keyboard.)X
555 1812(This)N
734(section)X
1006(discusses)X
1355(the)X
1485(user)X
1653(preference)X
2045(options)X
2327(of)X
2422(bell,)X
2599(keyclick,)X
2942(mouse)X
3194(behavior,)X
3546(and)X
3695(so)X
3795(on.)X
555 1908(The)N
714(default)X
981(values)X
1228(for)X
1352(many)X
1570(of)X
1665(these)X
1868(functions)X
2218(are)X
2347(determined)X
2766(by)X
2876(command)X
3246(line)X
3401(arguments)X
3790(to)X
3881(the)X
555 2004(X)N
640(server,)X
898(and)X
1047(on)X
1157(UNIX-based)X
1626(systems)X
1927(are)X
2056(typically)X
2388(set)X
2508(in)X
5 f
2617(/etc/ttys)X
1 f
2938(.)X
3004(Not)X
3158(all)X
3269 0.2500(implementations)AX
555 2100(will)N
715(actually)X
1017(be)X
1122(able)X
1291(to)X
1382(control)X
1654(all)X
1765(of)X
1860(these)X
2063(parameters.)X
555 2320(Use)N
5 f
731(XChangeKeyboardControl)X
1 f
1795(to)X
1886(change)X
2157(control)X
2429(from)X
2622(a)X
2683(keyboard.)X
3054(The)X
3213(de\256nition)X
3574(for)X
3698(this)X
555 2416(function)N
871(is:)X
555 2540(XChangeKeyboardControl)N
1510(\()X
2 f
1546(display)X
1 f
1801(,)X
2 f
1845(value_mask)X
1 f
2267(,)X
2 f
2311(values)X
1 f
2543(\))X
687 2636(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2732(unsigned)N
1027(long)X
2 f
1206(value_mask)X
1 f
1628(;)X
687 2828(XKeyboardControl)N
1388(*)X
2 f
(values)S
1 f
1664(;)X
2 f
555 2980(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3104(value_mask)N
1 f
1131(Speci\256es)X
1471(one)X
1620(value)X
1833(for)X
1957(each)X
2140(one)X
2289(bit)X
2405(in)X
2496(the)X
2626(mask)X
2834(\(least)X
3047(to)X
3138(most)X
3332(signi\256cant)X
3722(bit\).)X
1131 3200(The)N
1290(values)X
1537(are)X
1666(associated)X
2050(with)X
2229(the)X
2359(set)X
2479(of)X
2574(keys)X
2757(for)X
2881(the)X
3011(previously)X
3405(speci\256ed)X
3740(key-)X
1131 3296(board.)N
2 f
555 3420(values)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(structure)X
5 f
2373(XKeyboardControl)X
1 f
3107(.)X
555 3544(The)N
5 f
732(XChangeKeyboardControl)X
1 f
1796(function)X
2112(controls)X
2418(the)X
2548(keyboard)X
2897(characteristics)X
3423(de\256ned)X
3704(by)X
3814(the)X
5 f
570 3640(XKeyboardControl)N
1 f
1326(structure.)X
1700(The)X
1859(values)X
2106(argument)X
2461(speci\256es)X
2786(which)X
3023(values)X
3270(are)X
3399(to)X
3490(be)X
3595(changed.)X
555 3736(The)N
714(value_mask)X
1157(contains)X
1473(one)X
1622(value)X
1835(for)X
1959(each)X
2142(one)X
2291(bit)X
2407(in)X
2498(the)X
2628(mask)X
2836(\(least)X
3049(to)X
3140(most)X
3334(signi\256cant)X
3724(bit\).)X
555 3832(This)N
734(function)X
1050(operates)X
1365(on)X
1475(a)X
5 f
1554(XKeyboardControl)X
1 f
2310(structure:)X
3 f
2217 6144(133)N

134 p
%%Page: 134 138
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(/*)N
646(masks)X
888(for)X
1012(ChangeKeyboardControl)X
1919(*/)X
555 864(#de\256ne)N
836(KBKeyClickPercent)X
2283(\(1L<<0\))X
555 960(#de\256ne)N
836(KBBellPercent)X
2283(\(1L<<1\))X
555 1056(#de\256ne)N
836(KBBellPitch)X
2283(\(1L<<2\))X
555 1152(#de\256ne)N
836(KBBellDuration)X
2283(\(1L<<3\))X
555 1248(#de\256ne)N
836(KBLed)X
2283(\(1L<<4\))X
555 1344(#de\256ne)N
836(KBLedMode)X
2283(\(1L<<5\))X
555 1440(#de\256ne)N
836(KBKey)X
2283(\(1L<<6\))X
555 1536(#de\256ne)N
836(KBAutoRepeatMode)X
2283(\(1L<<7\))X
555 1728(typedef)N
841(struct)X
1059({)X
843 1824(int)N
959(key_click_percent;)X
843 1920(int)N
959(bell_percent;)X
843 2016(int)N
959(bell_pitch;)X
843 2112(int)N
959(bell_duration;)X
843 2208(int)N
959(led;)X
843 2304(int)N
959(led_mode)X
843 2400(int)N
959(key;)X
843 2496(int)N
959(auto_repeat_mode;)X
2283(/*)X
2374(AutoRepeatModeOff,)X
3165(AutoRepeatModeOn,)X
2283 2592(AutoRepeatModeDefault)N
3195(*/)X
555 2688(})N
619(XKeyboardControl;)X
555 2832(The)N
714(following)X
1079(describes)X
1428(each)X
1611(of)X
1706(the)X
1836(members)X
2181(of)X
2276(the)X
5 f
2424(XKeyboardControl)X
1 f
3180(structure.)X
555 2956(The)N
714(key_click_percent)X
1382(member)X
1693(sets)X
1847(the)X
1977(volume)X
2264(for)X
2388(key)X
2537(clicks)X
2765(between)X
3080(0)X
3146(\(off\))X
3328(and)X
3477(100)X
3631(\(loud\))X
555 3052(inclusive,)N
918(if)X
994(possible.)X
1327(A)X
1412(setting)X
1670(of)X
1765(-1)X
1860(restores)X
2155(the)X
2285(default.)X
2596(Other)X
2818(negative)X
3139(values)X
3386(generate)X
3706(a)X
5 f
3785(Bad-)X
555 3148(Value)N
1 f
817(error.)X
555 3272(The)N
714(bell_percent)X
1172(sets)X
1326(the)X
1456(base)X
1634(volume)X
1921(for)X
2045(the)X
2175(bell)X
2330(between)X
2645(0)X
2711(\(off\))X
2893(and)X
3042(100)X
3196(\(loud\))X
3433(inclusive,)X
3796(if)X
555 3368(possible.)N
888(A)X
973(setting)X
1231(of)X
1326(-1)X
1421(restores)X
1716(the)X
1846(default.)X
2157(Other)X
2379(negative)X
2700(values)X
2947(generate)X
3267(a)X
5 f
3346(BadValue)X
1 f
3765(error.)X
555 3464(The)N
714(bell_pitch)X
1090(member)X
1401(sets)X
1555(the)X
1685(pitch)X
1884(\(speci\256ed)X
2248(in)X
2339(Hz\))X
2492(of)X
2587(the)X
2717(bell,)X
2894(if)X
2970(possible.)X
3325(A)X
3410(setting)X
3668(to)X
3759(-1)X
555 3560(restores)N
850(the)X
980(default.)X
1291(Other)X
1513(negative)X
1834(values)X
2081(generate)X
2401(a)X
5 f
2480(BadValue)X
1 f
2899(error.)X
3135(The)X
3294(bell_duration)X
555 3656(member)N
866(sets)X
1020(the)X
1150(duration,)X
1488(speci\256ed)X
1823(in)X
1914(milliseconds,)X
2405(of)X
2500(the)X
2630(bell,)X
2807(if)X
2883(possible.)X
3216(A)X
3301(setting)X
3559(to)X
3650(-1)X
555 3752(restores)N
850(the)X
980(default.)X
1291(Other)X
1513(negative)X
1834(values)X
2081(generate)X
2401(a)X
5 f
2480(BadValue)X
1 f
2899(error.)X
555 3876(If)N
635(both)X
814(the)X
944(led_mode)X
1314(and)X
1463(led)X
1593(members)X
1938(are)X
2067(speci\256ed,)X
2424(the)X
2554(state)X
2738(of)X
2833(those)X
3041(LEDs)X
3268(are)X
3397(changed,)X
3734(if)X
3810(pos-)X
555 3972(sible.)N
766(If)X
846(only)X
1025(led_mode)X
1395(is)X
1476(speci\256ed,)X
1833(the)X
1963(state)X
2147(of)X
2242(all)X
2353(LEDs)X
2580(are)X
2709(changed,)X
3046(if)X
3122(possible.)X
3455(At)X
3565(most)X
3759(32)X
555 4068(LEDs)N
782(numbered)X
1156(from)X
1349(one)X
1498(are)X
1627(supported.)X
2018(No)X
2147(standard)X
2467(interpretation)X
2965(of)X
3060(LEDs)X
3287(is)X
3368(de\256ned.)X
3693(A)X
5 f
3796(Bad-)X
555 4164(Match)N
1 f
832(error)X
1024(is)X
1105(generated)X
1469(if)X
1545(an)X
1650(led)X
1780(is)X
1861(speci\256ed)X
2196(without)X
2488(an)X
2593(led_mode.)X
555 4288(If)N
635(both)X
814(the)X
944(auto_repeat_mode)X
1617(and)X
1766(key)X
1915(members)X
2260(are)X
2389(speci\256ed,)X
2746(the)X
2876(auto_repeat_mode)X
3549(of)X
3644(that)X
3799(key)X
3948(is)X
555 4384(changed,)N
892(if)X
968(possible)X
1279(\(where)X
1544(LED)X
1737(is)X
1818(the)X
1948(ordinal)X
2220(number)X
2511(of)X
2606(the)X
2736(LED)X
2929(to)X
3020(be)X
3125(changed,)X
3462(not)X
3597(a)X
3658(mask\).)X
3917(If)X
555 4480(only)N
734(auto_repeat_mode)X
1407(is)X
1488(speci\256ed,)X
1845(the)X
1975(global)X
2218(auto_repeat)X
2651(mode)X
2869(for)X
2993(the)X
3123(entire)X
3346(keyboard)X
3695(is)X
555 4576(changed,)N
892(if)X
968(possible,)X
1301(and)X
1450(does)X
1633(not)X
1768(affect)X
1990(the)X
2120(per_key)X
2425(settings.)X
2761(A)X
5 f
2864(BadMatch)X
1 f
3298(error)X
3490(is)X
3571(generated)X
3935(if)X
555 4672(a)N
616(key)X
765(is)X
846(speci\256ed)X
1181(without)X
1473(an)X
1578(auto_repeat_mode.)X
555 4796(A)N
640(bell)X
795(generator)X
1149(connected)X
1528(with)X
1707(the)X
1837(console)X
2128(but)X
2263(not)X
2398(directly)X
2690(on)X
2800(a)X
2861(keyboard)X
3210(is)X
3291(treated)X
3553(as)X
3648(if)X
3724(it)X
3796(were)X
555 4892(part)N
714(of)X
809(the)X
939(main)X
1138(keyboard.)X
1531(The)X
1690(order)X
1897(in)X
1988(which)X
2225(controls)X
2531(are)X
2660(veri\256ed)X
2951(and)X
3100(altered)X
3362(is)X
3443(server-)X
555 4988(dependent.)N
983(If)X
1063(an)X
1168(error)X
1360(is)X
1441(generated,)X
1827(a)X
1888(subset)X
2130(of)X
2225(the)X
2355(controls)X
2661(may)X
2835(have)X
3023(been)X
3211(altered.)X
555 5112(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XChangeKeyboardControl)X
1 f
2900(are)X
5 f
3047(BadMatch)X
1 f
3481(and)X
5 f
3648(Bad-)X
555 5208(Value)N
1 f
795(.)X
555 5428(Use)N
5 f
731(XGetKeyboardControl)X
1 f
1630(to)X
1721(obtain)X
1964(the)X
2094(current)X
2365(control)X
2637(values)X
2884(for)X
3008(the)X
3138(keyboard.)X
3531(The)X
555 5524(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
3 f
2217 6144(134)N

135 p
%%Page: 135 139
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XGetKeyboardControl)N
1368(\()X
2 f
1404(display)X
1 f
1659(,)X
2 f
1703(values_return)X
1 f
2199(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(XKeyboardState)N
1295(*)X
2 f
(values_return)S
1 f
1835(;)X
2 f
555 1016(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1140(values_return)N
1 f
1131(Returns)X
1427(the)X
1557(current)X
1828(keyboard)X
2177(parameter)X
2551(in)X
2642(the)X
2772(speci\256ed)X
5 f
3125(XKeyboardState)X
1 f
3805(struc-)X
1131 1236(ture.)N
555 1360(The)N
5 f
732(XGetKeyboardControl)X
1 f
1631(function)X
1947(returns)X
2213(the)X
2343(current)X
2614(control)X
2886(values)X
3133(for)X
3257(the)X
3387(keyboard)X
3736(to)X
3827(the)X
5 f
570 1456(XKeyboardState)N
1 f
1250(structure.)X
1624(The)X
1783(members)X
2128(of)X
2223(this)X
2373(structure)X
2703(are:)X
555 1580(typedef)N
841(struct)X
1059({)X
775 1676(int)N
891(key_click_percent;)X
775 1772(int)N
891(bell_percent;)X
775 1868(unsigned)N
1115(int)X
1231(bell_pitch,)X
1629(bell_duration;)X
775 1964(unsigned)N
1115(long)X
1294(led_mask;)X
775 2060(int)N
891(global_auto_repeat;)X
775 2156(char)N
948(auto_repeats[32];)X
775 2252(})N
839(XKeyboardState;)X
555 2424(For)N
699(the)X
829(LEDs,)X
1078(the)X
1208(least)X
1392(signi\256cant)X
1782(bit)X
1898(of)X
1993(led_mask)X
2353(corresponds)X
2799(to)X
2890(LED)X
3083(one,)X
3254(and)X
3403(each)X
3586(one)X
3735(bit)X
3851(in)X
555 2520(led_mask)N
915(indicates)X
1251(an)X
1356(LED)X
1549(that)X
1704(is)X
1785(lit.)X
1926(The)X
2085(auto_repeats)X
2552(member)X
2863(is)X
2944(a)X
3005(bit)X
3121(vector.)X
3385(Each)X
3583(one)X
3732(bit)X
555 2616(indicates)N
891(that)X
1046(auto-repeat)X
1464(is)X
1545(enabled)X
1841(for)X
1965(the)X
2095(corresponding)X
2620(key.)X
2813(The)X
2972(vector)X
3214(is)X
3295(represented)X
3722(as)X
3817(32)X
555 2712(bytes.)N
785(Byte)X
974(N)X
1059(\(from)X
1281(0\))X
1376(contains)X
1692(the)X
1822(bits)X
1972(for)X
2096(keys)X
2279(8N)X
2408(to)X
2499(8N+7,)X
2744(with)X
2923(the)X
3053(least)X
3237(signi\256cant)X
3627(bit)X
3743(in)X
3834(the)X
555 2808(byte)N
729(representing)X
1186(key)X
1335(8N.)X
1508(The)X
1667(global_auto_repeat)X
2365(member)X
2676(can)X
2820(be)X
2925(set)X
3045(to)X
3136(one)X
3285(of)X
3380(the)X
3510(constants)X
5 f
570 2904(AutoRepeatModeOn)N
1 f
1411(or)X
5 f
1524(AutoRepeatModeOff)X
1 f
2344(.)X
555 3124(Use)N
5 f
731(XAutoRepeatOn)X
1 f
1411(to)X
1502(turn)X
1666(on)X
1776(the)X
1906(keyboard)X
2255(auto-repeat.)X
2695(The)X
2854(de\256nition)X
3215(for)X
3339(this)X
3489(function)X
3805(is:)X
555 3248(XAutoRepeatOn)N
1153(\()X
2 f
1189(display)X
1 f
1451(\))X
687 3344(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 3496(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 3620(The)N
5 f
732(XAutoRepeatOn)X
1 f
1412(function)X
1728(turns)X
1926(on)X
2036(auto-repeat)X
2454(for)X
2578(the)X
2708(keyboard)X
3057(on)X
3167(the)X
3297(speci\256ed)X
3632(display.)X
555 3840(Use)N
5 f
731 0.2500(XAutoRepeatOff)AX
1 f
1412(to)X
1503(turn)X
1667(off)X
1791(the)X
1921(keyboard)X
2270(auto-repeat.)X
2710(The)X
2869(de\256nition)X
3230(for)X
3354(this)X
3504(function)X
3820(is:)X
555 3964(XAutoRepeatOff)N
1167(\()X
2 f
1203(display)X
1 f
1465(\))X
687 4060(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 4212(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 4336(The)N
5 f
732 0.2500(XAutoRepeatOff)AX
1 f
1413(function)X
1729(turns)X
1927(off)X
2051(auto-repeat)X
2469(for)X
2593(the)X
2723(keyboard)X
3072(on)X
3182(the)X
3312(speci\256ed)X
3647(display.)X
555 4556(Use)N
5 f
731(XBell)X
1 f
973(to)X
1064(ring)X
1228(the)X
1358(bell.)X
1535(The)X
1694(de\256nition)X
2055(for)X
2179(this)X
2329(function)X
2645(is:)X
555 4680(XBell)N
773(\()X
2 f
809(display)X
1 f
1064(,)X
2 f
1108(percent)X
1 f
1379(\))X
687 4776(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4872(int)N
2 f
803(percent)X
1 f
1074(;)X
2 f
555 5024(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5148(percent)N
1 f
1131(Speci\256es)X
1471(the)X
1601(base)X
1779(volume)X
2066(for)X
2190(the)X
2320(bell,)X
2497(which)X
2734(can)X
2878(range)X
3095(from)X
3288(-100)X
3471(to)X
3562(100)X
1131 5244(inclusive.)N
555 5368(The)N
5 f
732(XBell)X
1 f
974(function)X
1290(rings)X
1488(the)X
1618(bell)X
1773(on)X
1883(the)X
2013(keyboard)X
2362(on)X
2472(the)X
2602(speci\256ed)X
2937(display,)X
3236(if)X
3312(possible.)X
3667(The)X
555 5464(speci\256ed)N
890(volume)X
1177(is)X
1258(relative)X
1545(to)X
1636(the)X
1766(base)X
1944(volume)X
2231(for)X
2355(the)X
2485(keyboard.)X
2878(If)X
2958(the)X
3088(value)X
3301(for)X
3425(the)X
3555(percent)X
555 5560(argument)N
910(is)X
991(not)X
1126(in)X
1217(the)X
1347(range)X
1564(-100)X
1747(to)X
1838(100)X
1992(inclusive,)X
2355(a)X
5 f
2434(BadValue)X
1 f
2853(error)X
3045(is)X
3126(generated.)X
3534(The)X
3693(volume)X
555 5656(at)N
641(which)X
878(the)X
1008(bell)X
1163(is)X
1244(rung)X
1427(when)X
1639(the)X
1769(percent)X
2050(argument)X
2405(is)X
2486(nonnegative)X
2939(is:)X
3 f
2217 6144(135)N

136 p
%%Page: 136 140
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
755 672(base)N
933(-)X
984([\(base)X
1220(*)X
1286(percent\))X
1596(/)X
1643(100])X
1826(+)X
1898(percent)X
555 796(The)N
714(volume)X
1001(at)X
1087(which)X
1324(the)X
1454(bell)X
1609(is)X
1690(rung)X
1873(when)X
2085(the)X
2215(percent)X
2496(argument)X
2851(is)X
2932(negative)X
3253(is:)X
755 920(base)N
933(+)X
1005([\(base)X
1241(*)X
1307(percent\))X
1617(/)X
1664(100])X
555 1044(To)N
675(change)X
946(the)X
1076(base)X
1254(volume)X
1541(of)X
1636(the)X
1766(bell)X
1921(for)X
2045(this)X
2195(keyboard,)X
2566(use)X
5 f
2723(XChangeKeyboardControl)X
1 f
3765(.)X
555 1264(Use)N
5 f
731(XSetPointerMapping)X
1 f
1576(to)X
1667(set)X
1787(the)X
1917(mapping)X
2248(of)X
2343(buttons)X
2625(on)X
2735(the)X
2865(pointer.)X
3181(The)X
3340(de\256nition)X
3701(for)X
3825(this)X
555 1360(function)N
871(is:)X
555 1484(int)N
671(XSetPointerMapping)X
1427(\()X
2 f
1463(display)X
1 f
1718(,)X
2 f
1762(map)X
1 f
1913(,)X
2 f
1957(nmap)X
1 f
2159(\))X
687 1580(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1676(unsigned)N
1027(char)X
2 f
1200(map)X
1 f
1358([])X
1423(;)X
687 1772(int)N
2 f
803(nmap)X
1 f
1005(;)X
2 f
555 1924(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2048(map)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mapping)X
1932(list.)X
2 f
555 2172(nmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(items)X
2201(in)X
2292(mapping)X
2623(list.)X
555 2296(The)N
5 f
732(XSetPointerMapping)X
1 f
1577(function)X
1893(sets)X
2047(the)X
2177(mapping)X
2508(of)X
2603(the)X
2733(pointer)X
3005(and)X
3154(causes)X
3405(the)X
3535(X)X
3620(server)X
3856(to)X
555 2392(generate)N
875(a)X
5 f
954(MappingNotify)X
1 f
1553(event)X
1766(on)X
1876(a)X
1937(status)X
2160(of)X
5 f
2273(MappingSuccess)X
1 f
2958(.)X
3024(Elements)X
3375(of)X
3470(the)X
3600(list)X
3731(are)X
555 2488(indexed)N
856(starting)X
1143(from)X
1336(one.)X
1529(The)X
1688(length)X
1931(of)X
2026(the)X
2156(list)X
2287(must)X
2481(be)X
2586(the)X
2716(same)X
2919(as)X
5 f
3032(XGetPointerMapping)X
1 f
555 2584(would)N
797(return.)X
1073(Otherwise,)X
1478(a)X
5 f
1557(BadValue)X
1 f
1976(error)X
2168(is)X
2249(generated.)X
2657(The)X
2816(index)X
3034(is)X
3115(a)X
3176(core)X
3349(button)X
3597(number,)X
555 2680(and)N
704(the)X
834(element)X
1136(of)X
1231(the)X
1361(list)X
1492(de\256nes)X
1763(the)X
1893(effective)X
2223(number.)X
2558(A)X
2643(zero)X
2816(element)X
3118(disables)X
3424(a)X
3485(button,)X
3755(and)X
555 2776(elements)N
891(are)X
1020(not)X
1155(restricted)X
1505(in)X
1596(value)X
1809(by)X
1919(the)X
2049(number)X
2340(of)X
2435(physical)X
2751(buttons.)X
3077(However,)X
3442(no)X
3552(two)X
3706(ele-)X
555 2872(ments)N
788(can)X
932(have)X
1120(the)X
1250(same)X
1453(nonzero)X
1758(value.)X
2015(Otherwise,)X
2420(a)X
5 f
2499(BadValue)X
1 f
2918(error)X
3110(is)X
3191(generated.)X
3599(If)X
3679(any)X
3828(of)X
555 2968(the)N
685(buttons)X
967(to)X
1058(be)X
1163(altered)X
1425(are)X
1554(currently)X
1894(in)X
1985(the)X
2115(down)X
2332(state,)X
2538(the)X
2668(status)X
2891(reply)X
3094(is)X
5 f
3193(MappingBusy)X
1 f
3763(and)X
555 3064(the)N
685(mapping)X
1016(is)X
1097(not)X
1232(changed.)X
1591(This)X
1770(function)X
2086(returns)X
2352(either)X
5 f
2593(MappingSuccess)X
1 f
3300(or)X
5 f
3413(MappingBusy)X
1 f
3961(.)X
555 3284(Use)N
5 f
731(XGetPointerMapping)X
1 f
1586(to)X
1677(get)X
1807(the)X
1937(pointer)X
2209(mapping.)X
2584(The)X
2743(de\256nition)X
3104(for)X
3228(this)X
3378(function)X
3694(is:)X
555 3408(int)N
671(XGetPointerMapping)X
1441(\()X
2 f
1477(display)X
1 f
1732(,)X
2 f
1776(map)X
1 f
1927(,)X
2 f
1971(nmap)X
1 f
2173(\))X
687 3504(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3600(unsigned)N
1027(char)X
2 f
1200(map)X
1 f
1358([])X
1423(;)X
687 3696(int)N
2 f
803(nmap)X
1 f
1005(;)X
2 f
555 3848(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3972(map)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mapping)X
1932(list.)X
2 f
555 4096(nmap)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(items)X
2201(in)X
2292(mapping)X
2623(list.)X
555 4220(The)N
5 f
732(XGetPointerMapping)X
1 f
1587(function)X
1903(returns)X
2169(the)X
2299(current)X
2570(mapping)X
2901(of)X
2996(the)X
3126(pointer.)X
3442(Elements)X
3793(of)X
3888(the)X
555 4316(list)N
686(are)X
815(indexed)X
1116(starting)X
1403(from)X
1596(one.)X
1789(The)X
1948(length)X
2191(of)X
2286(the)X
2416(list)X
2547(indicates)X
2883(the)X
3013(number)X
3304(of)X
3399(physical)X
3715(buttons.)X
555 4412(The)N
714(nominal)X
1026(mapping)X
1357(for)X
1481(a)X
1542(pointer)X
1814(is)X
1895(the)X
2025(identity)X
2318(mapping:)X
2674(map[i]=i.)X
555 4660(Use)N
5 f
731(XQueryKeymap)X
1 f
1390(to)X
1481(obtain)X
1724(a)X
1785(bit)X
1901(vector)X
2143(for)X
2267(the)X
2397(keyboard.)X
2790(The)X
2949(de\256nition)X
3310(for)X
3434(this)X
3584(function)X
3900(is:)X
555 4784(XQueryKeymap)N
1142(\()X
2 f
1178(display)X
1 f
1433(,)X
2 f
1477(keys_return)X
1 f
1899(\))X
687 4880(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4976(char)N
2 f
860(keys_return)X
1 f
1275([32])X
1428(;)X
2 f
555 5128(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5252(keys_return)N
1 f
1131(Returns)X
1427(an)X
1532(array)X
1734(of)X
1829(bytes)X
2037(that)X
2192(identi\256es)X
2538(which)X
2775(keys)X
2958(are)X
3087(pressed)X
3372(down.)X
3633(Each)X
3831(bit)X
1131 5348(represents)N
1509(one)X
1658(key)X
1807(of)X
1902(the)X
2032(keyboard.)X
555 5472(The)N
5 f
732(XQueryKeymap)X
1 f
1391(function)X
1707(returns)X
1973(a)X
2034(bit)X
2150(vector)X
2392(for)X
2516(the)X
2646(keyboard,)X
3017(where)X
3253(each)X
3436(one)X
3585(bit)X
3701(indi-)X
555 5568(cates)N
753(that)X
908(the)X
1038(corresponding)X
1563(key)X
1712(is)X
1793(currently)X
2133(pressed)X
2418(down.)X
2679(The)X
2838(vector)X
3080(is)X
3161(represented)X
3588(as)X
3683(32)X
3793(bytes.)X
555 5664(Byte)N
744(N)X
829(\(from)X
1051(0\))X
1146(contains)X
1462(the)X
1592(bits)X
1742(for)X
1866(keys)X
2049(8N)X
2178(to)X
2269(8N+7)X
2492(with)X
2671(the)X
2801(least)X
2985(signi\256cant)X
3375(bit)X
3491(in)X
3582(the)X
3712(byte)X
555 5760(representing)N
1012(key)X
1161(8N.)X
3 f
2217 6144(136)N

137 p
%%Page: 137 141
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
555 672(7.9.)N
731(Manipulating)X
1268(the)X
1407(Keyboard)X
1806(Encoding)X
1 f
555 824(A)N
640(KeyCode)X
994(represents)X
1372(a)X
1433(physical)X
1749(\(or)X
1873(logical\))X
2165(key.)X
2336(Keycodes)X
2704(lie)X
2815(in)X
2906(the)X
3036(inclusive)X
3377(range)X
3594([8,255].)X
3894(A)X
555 920(keycode)N
870(value)X
1083(carries)X
1339(no)X
1449(intrinsic)X
1761(information,)X
2222(although)X
2553(server)X
2789(implementors)X
3297(may)X
3471(attempt)X
3759(to)X
555 1016(encode)N
826(geometry)X
1181(\(for)X
1334(example,)X
1677(matrix\))X
1959(information)X
2398(in)X
2489(some)X
2697(fashion)X
2978(so)X
3078(that)X
3233(it)X
3305(can)X
3449(be)X
3554(interpreted)X
555 1112(in)N
646(a)X
707(server-dependent)X
1334(fashion.)X
1659(The)X
1818(mapping)X
2149(between)X
2464(keys)X
2647(and)X
2796(keycodes)X
3145(cannot)X
3402(be)X
3507(changed.)X
555 1236(A)N
640(KeySym)X
970(is)X
1051(an)X
1156(encoding)X
1501(of)X
1596(a)X
1657(symbol)X
1939(on)X
2049(the)X
2179(cap)X
2323(of)X
2418(a)X
2479(key.)X
2650(The)X
2809(set)X
2929(of)X
3024(de\256ned)X
3305(KeySyms)X
3669(include)X
555 1332(the)N
685(ISO)X
848(Latin)X
1057(character)X
1401(sets)X
1555(\(1-4\),)X
1774(Katakana,)X
2150(Arabic,)X
2433(Cyrillic,)X
2748(Greek,)X
3006(Technical,)X
3398(Special,)X
3702(Publish-)X
555 1428(ing,)N
712(APL,)X
922(Hebrew,)X
1243(and)X
1392(a)X
1453(special)X
1720(miscellany)X
2125(of)X
2220(keys)X
2403(found)X
2630(on)X
2740(keyboards)X
3123(\(RETURN,)X
3548(HELP,)X
3812(TAB,)X
555 1524(and)N
704(so)X
804(on\).)X
965(To)X
1085(the)X
1215(extent)X
1453(possible,)X
1786(these)X
1989(sets)X
2143(are)X
2272(derived)X
2558(from)X
2751(international)X
3220(standards.)X
3596(In)X
3691(areas)X
555 1620(where)N
791(no)X
901(standards)X
1255(exist,)X
1466(some)X
1674(of)X
1769(these)X
1972(sets)X
2126(are)X
2255(derived)X
2541(from)X
2734(Digital)X
3002(standards.)X
3400(The)X
3559(list)X
3690(of)X
555 1716(de\256ned)N
836(symbols)X
1152(can)X
1296(be)X
1401(found)X
1628(in)X
1719(the)X
5 f
1867(<X11/keysymdef.h>)X
1 f
2683(header)X
2939(\256le.)X
3118(Unfortunately,)X
3656(some)X
3864(C)X
555 1812(preprocessors)N
1059(have)X
1247(limits)X
1472(on)X
1582(the)X
1712(number)X
2003(of)X
2098(de\256ned)X
2379(symbols.)X
2739(If)X
2819(you)X
2973(must)X
3167(use)X
3306(keysyms)X
3636(not)X
3771(in)X
3862(the)X
555 1908(Latin)N
764(1-4,)X
925(Greek,)X
1183(and)X
1332(miscellany)X
1737(classes,)X
2025(you)X
2179(may)X
2353(have)X
2541(to)X
2632(de\256ne)X
2869(a)X
2930(symbol)X
3212(for)X
3336(those)X
3544(sets.)X
3742(Most)X
555 2004(applications)N
1004(usually)X
1281(only)X
1460(include)X
5 f
1760(<X11/keysym.h>)X
1 f
2431(,)X
2475(which)X
2712(de\256nes)X
2983(symbols)X
3299(for)X
3423(ISO)X
3586(Latin)X
3795(1-4,)X
555 2100(Greek,)N
813(and)X
962(Miscellany.)X
555 2224(A)N
640(list)X
771(of)X
866(KeySyms)X
1230(is)X
1311(associated)X
1695(with)X
1874(each)X
2057(KeyCode.)X
2433(The)X
2592(length)X
2835(of)X
2930(the)X
3060(list)X
3191(can)X
3335(vary)X
3513(with)X
3692(each)X
555 2320(KeyCode.)N
931(The)X
1090(list)X
1221(is)X
1302(intended)X
1628(to)X
1719(convey)X
1995(the)X
2125(set)X
2245(of)X
2340(symbols)X
2656(on)X
2766(the)X
2896(corresponding)X
3421(key.)X
3592(By)X
3717(conven-)X
555 2416(tion,)N
737(if)X
813(the)X
943(list)X
1074(contains)X
1390(a)X
1451(single)X
1684(KeySym)X
2014(and)X
2163(if)X
2239(that)X
2394(KeySym)X
2724(is)X
2805(alphabetic)X
3190(and)X
3339(case)X
3512(distinction)X
3908(is)X
555 2512(relevant)N
861(for)X
985(it,)X
1079(then)X
1253(it)X
1325(should)X
1582(be)X
1687(treated)X
1949(as)X
2044(equivalent)X
2434(to)X
2525(a)X
2586(two-element)X
3049(list)X
3180(of)X
3275(the)X
3405(lowercase)X
3778(and)X
555 2608(uppercase)N
928(KeySyms.)X
1314(For)X
1458(example,)X
1801(if)X
1877(the)X
2007(list)X
2138(contains)X
2454(the)X
2584(single)X
2817(KeySym)X
3147(for)X
3271(uppercase)X
3644(A,)X
3751(the)X
555 2704(client)N
774(should)X
1031(treat)X
1210(it)X
1282(as)X
1377(if)X
1453(it)X
1525(were)X
1717(a)X
1778(pair)X
1937(with)X
2116(lowercase)X
2489(a)X
2550(as)X
2645(the)X
2775(\256rst)X
2934(KeySym)X
3264(and)X
3413(uppercase)X
3786(A)X
3871(as)X
555 2800(the)N
685(second)X
951(KeySym.)X
555 2924(For)N
699(any)X
848(KeyCode,)X
1224(the)X
1354(\256rst)X
1513(KeySym)X
1843(in)X
1934(the)X
2064(list)X
2195(should)X
2452(be)X
2557(chosen)X
2823(as)X
2918(the)X
3048(interpretation)X
3546(of)X
3641(a)X
555 3020(KeyPress)N
908(when)X
1120(no)X
1230(modi\256er)X
1551(keys)X
1734(are)X
1863(down.)X
2124(The)X
2283(second)X
2549(KeySym)X
2879(in)X
2970(the)X
3100(list)X
3231(normally)X
3572(should)X
3829(be)X
555 3116(chosen)N
821(when)X
1033(the)X
1163(Shift)X
1357(modi\256er)X
1678(is)X
1759(on,)X
1891(or)X
1986(when)X
2198(the)X
2328(Lock)X
2531(modi\256er)X
2852(is)X
2933(on)X
3043(and)X
3192(Lock)X
3395(is)X
3476(interpreted)X
3880(as)X
555 3212(ShiftLock.)N
974(When)X
1206(the)X
1336(Lock)X
1539(modi\256er)X
1860(is)X
1941(on)X
2051(and)X
2200(is)X
2281(interpreted)X
2685(as)X
2780(CapsLock,)X
3181(it)X
3253(is)X
3334(suggested)X
3703(that)X
3858(the)X
555 3308(Shift)N
749(modi\256er)X
1070(\256rst)X
1229(be)X
1334(applied)X
1616(to)X
1707(choose)X
1973(a)X
2034(KeySym,)X
2386(but)X
2521(if)X
2597(that)X
2752(KeySym)X
3082(is)X
3163(lowercase)X
3536(alphabetic,)X
555 3404(the)N
685(corresponding)X
1210(uppercase)X
1583(KeySym)X
1913(should)X
2170(be)X
2275(used)X
2458(instead.)X
2752(Other)X
2974(interpretations)X
3506(of)X
3601(CapsLock)X
555 3500(are)N
684(possible;)X
1020(for)X
1144(example,)X
1487(it)X
1559(may)X
1733(be)X
1838(viewed)X
2114(as)X
2209(equivalent)X
2599(to)X
2690(ShiftLock,)X
3087(but)X
3222(only)X
3401(applying)X
3732(when)X
555 3596(the)N
685(\256rst)X
844(KeySym)X
1174(is)X
1255(lowercase)X
1628(alphabetic)X
2013(and)X
2162(the)X
2292(second)X
2558(KeySym)X
2888(is)X
2969(the)X
3099(corresponding)X
3624(uppercase)X
555 3692(alphabetic.)N
962(No)X
1091(interpretation)X
1589(of)X
1684(KeySyms)X
2048(beyond)X
2329(the)X
2459(\256rst)X
2618(two)X
2772(in)X
2863(a)X
2924(list)X
3055(is)X
3136(suggested)X
3505(here.)X
3700(No)X
3829(spa-)X
555 3788(tial)N
691(geometry)X
1046(of)X
1141(the)X
1271(symbols)X
1587(on)X
1697(the)X
1827(key)X
1976(is)X
2057(de\256ned)X
2338(by)X
2448(their)X
2632(order)X
2839(in)X
2930(the)X
3060(KeySym)X
3390(list,)X
3543(although)X
3874(a)X
555 3884(geometry)N
910(might)X
1139(be)X
1244(de\256ned)X
1525(on)X
1635(a)X
1696(vendor-speci\256c)X
2260(basis.)X
2502(The)X
2661(X)X
2746(server)X
2982(does)X
3165(not)X
3300(use)X
3439(the)X
3569(mapping)X
555 3980(between)N
870(KeyCodes)X
1258(and)X
1407(KeySyms.)X
1815(Rather,)X
2094(it)X
2166(stores)X
2393(it)X
2465(merely)X
2732(for)X
2856(reading)X
3142(and)X
3291(writing)X
3568(by)X
3678(clients.)X
2197 4156(Note)N
775 4292(The)N
934(simple)X
1192(interface,)X
5 f
1562(XLookupString)X
1 f
2177(will)X
2337(perform)X
2642(simple)X
2900(translation)X
3295(of)X
3390(a)X
3451(key)X
3600(event)X
775 4388(to)N
866(an)X
971(ASCII)X
1222(string.)X
1467(Keyboard)X
1835(related)X
2097 0.3828(utilities)AX
2386(are)X
2515(discussed)X
2874(in)X
2965(Chapter)X
3266(10.)X
555 4688(Use)N
5 f
731(XGetKeyboardMapping)X
1 f
1684(to)X
1775(obtain)X
2018(the)X
2148(symbols)X
2464(for)X
2588(the)X
2718(speci\256ed)X
3053(number)X
3344(of)X
3439(keycodes.)X
3832(The)X
555 4784(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 4908(KeySym)N
885(*XGetKeyboardMapping\()X
2 f
1819(display)X
1 f
2074(,)X
2 f
2118(\256rst_keycode_wanted)X
1 f
2881(,)X
2 f
2925(keycode_count)X
1 f
3448(,)X
2 f
1171 5004(keysyms_per_keycode_return)N
1 f
2217(\))X
687 5100(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 5196(KeyCode)N
2 f
1041(\256rst_keycode_wanted)X
1 f
1811(;)X
687 5292(int)N
2 f
803(keycode_count)X
1 f
1333(;)X
687 5388(int)N
803(*)X
2 f
(keysyms_per_keycode_return)S
1 f
1893(;)X
2 f
555 5540(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5664(\256rst_keycode_wanted)N
1 f
1131 5760(Speci\256es)N
1471(the)X
1601(\256rst)X
1760(keycode)X
2075(that)X
2230(is)X
2311(to)X
2402(be)X
2507(returned.)X
3 f
2217 6144(137)N

138 p
%%Page: 138 142
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(keycode_count)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(keycodes)X
2336(that)X
2491(are)X
2620(to)X
2711(be)X
2816(returned.)X
2 f
555 796(keysyms_per_keycode_return)N
1 f
1131 892(Returns)N
1427(the)X
1557(number)X
1848(of)X
1943(keysyms)X
2273(per)X
2407(keycode.)X
555 1016(The)N
5 f
732(XGetKeyboardMapping)X
1 f
1685(function,)X
2023(starting)X
2310(with)X
2489(\256rst_keycode,)X
3007(returns)X
3273(the)X
3403(symbols)X
3719(for)X
3843(the)X
555 1112(speci\256ed)N
890(number)X
1181(of)X
1276(keycodes.)X
1669(The)X
1828(value)X
2041(speci\256ed)X
2376(in)X
2467(the)X
2597(\256rst_keycode)X
3093(argument)X
3448(must)X
3642(be)X
3747(greater)X
555 1208(than)N
729(or)X
824(equal)X
1037(to)X
1128(min_keycode)X
1625(as)X
1720(returned)X
2035(in)X
2126(the)X
5 f
2274(Display)X
1 f
2598(structure)X
2928(at)X
3014(connection)X
3423(setup.)X
3653(Other-)X
555 1304(wise,)N
760(a)X
5 f
839(BadValue)X
1 f
1258(error)X
1450(is)X
1531(generated.)X
1939(In)X
2034(addition,)X
2368(the)X
2498(following)X
2863(expression)X
3261(must)X
3455(be)X
3560(less)X
3714(than)X
3888(or)X
555 1400(equal)N
768(to)X
859(max_keycode)X
1370(as)X
1465(returned)X
1780(in)X
1871(the)X
5 f
2019(Display)X
1 f
2343(structure)X
2673(at)X
2759(connection)X
3168(setup.)X
3398(If)X
3478(this)X
3628(is)X
3709(not)X
3844(the)X
555 1496(case,)N
750(a)X
5 f
829(BadValue)X
1 f
1248(error)X
1440(is)X
1521(generated.)X
843 1668(\256rst_keycode)N
1339(+)X
1411(keycode_count)X
1966(-)X
2017(1)X
555 1812(The)N
714(number)X
1005(of)X
1100(elements)X
1436(in)X
1527(the)X
1657(keysyms)X
1987(list)X
2118(is:)X
843 1984(keycode_count)N
1398(*)X
1464(keysyms_per_keycode_return)X
555 2128(Then,)N
780(KeySym)X
1110(number)X
1401(N,)X
1508(counting)X
1839(from)X
2032(zero,)X
2227(for)X
2351(keycode)X
2666(K)X
2751(has)X
2890(an)X
2995(index,)X
3235(counting)X
3566(from)X
3759(zero,)X
555 2224(of)N
650(the)X
780(following)X
1145(in)X
1236(KeySym:)X
843 2368(\(K)N
957(-)X
1008(\256rst_code\))X
1406(*)X
1472(keysyms_per_code)X
2168(+)X
2240(N)X
555 2512(The)N
714(keysyms_per_keycode_return)X
1791(value)X
2004(is)X
2085(chosen)X
2351(arbitrarily)X
2726(by)X
2836(the)X
2966(X)X
3051(server)X
3287(to)X
3378(be)X
3483(large)X
3681(enough)X
555 2608(to)N
646(report)X
878(all)X
989(requested)X
1348(symbols.)X
1686(A)X
1771(special)X
2038(KeySym)X
2368(value)X
2581(of)X
5 f
2694(NoSymbol)X
1 f
3136(is)X
3217(used)X
3400(to)X
3491(\256ll)X
3612(in)X
3703(unused)X
555 2704(elements)N
891(for)X
1015(individual)X
1396(keycodes.)X
555 2828(Use)N
5 f
731(XFree)X
1 f
1008(to)X
1099(free)X
1257(the)X
1387(storage)X
1663(returned)X
1978(by)X
5 f
2106(XGetKeyboardMapping)X
1 f
3037(.)X
3103(\(See)X
3281(Section)X
3568(2.4)X
3700(for)X
555 2924(further)N
816(information.\))X
555 3048(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XGetKeyboardMapping)X
1 f
2958(is)X
5 f
3057(BadValue)X
1 f
3454(.)X
555 3268(Use)N
5 f
731(XChangeKeyboardMapping)X
1 f
1849(to)X
1940(change)X
2211(the)X
2341(keyboard)X
2690(mapping.)X
3043(The)X
3202(de\256nition)X
3563(for)X
3687(this)X
555 3364(function)N
871(is:)X
555 3488(XChangeKeyboardMapping\()N
2 f
1587(display)X
1 f
1842(,)X
2 f
1886(\256rst_keycode)X
1 f
2350(,)X
2 f
2394(keysyms_per_keycode)X
1 f
3169(,)X
2 f
3213(keysyms)X
1 f
3500(,)X
2 f
3544(num_codes)X
1 f
3946(\))X
687 3584(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3680(int)N
2 f
803(\256rst_keycode)X
1 f
1274(;)X
687 3776(int)N
2 f
803(keysyms_per_keycode)X
1 f
1585(;)X
687 3872(KeySym)N
1017(*)X
2 f
(keysyms)S
1 f
1355(;)X
687 3968(int)N
2 f
803(num_codes)X
1 f
1205(;)X
2 f
555 4120(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4244(\256rst_keycode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(\256rst)X
1760(keycode)X
2075(that)X
2230(is)X
2311(to)X
2402(be)X
2507(changed.)X
2 f
555 4368(keysyms_per_keycode)N
1 f
1131 4464(Speci\256es)N
1471(the)X
1601(keysyms)X
1931(that)X
2086(are)X
2215(to)X
2306(be)X
2411(used.)X
2 f
555 4588(keysyms)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(an)X
2000(array)X
2202(of)X
2297(keysyms.)X
2 f
555 4712(num_codes)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(keycodes)X
2336(that)X
2491(are)X
2620(to)X
2711(be)X
2816(changed.)X
555 4836(The)N
5 f
732(XChangeKeyboardMapping)X
1 f
1850(function,)X
2188(starting)X
2475(with)X
2654(\256rst_keycode,)X
3172(de\256nes)X
3443(the)X
3573(symbols)X
3889(for)X
555 4932(the)N
685(speci\256ed)X
1020(number)X
1311(of)X
1406(keycodes.)X
1799(The)X
1958(symbols)X
2274(for)X
2398(keycodes)X
2747(outside)X
3024(this)X
3174(range)X
3391(remained)X
555 5028(unchanged.)N
980(The)X
1139(number)X
1430(of)X
1525(elements)X
1861(in)X
1952(the)X
2082(keysyms)X
2412(list)X
2543(must)X
2737(be)X
2842(a)X
2903(multiple)X
3221(of)X
555 5124(keysyms_per_keycode.)N
1422(Otherwise,)X
1827(a)X
5 f
1906(BadLength)X
1 f
2370(error)X
2562(is)X
2643(generated.)X
3029(The)X
3188(speci\256ed)X
3523(\256rst_keycode)X
555 5220(must)N
749(be)X
854(greater)X
1120(than)X
1294(or)X
1389(equal)X
1602(to)X
1693(min_keycode)X
2190(supplied)X
2511(at)X
2597(connection)X
3006(setup)X
3214(and)X
3363(stored)X
3600(in)X
3691(the)X
5 f
570 5316(Display)N
1 f
894(structure.)X
1268(Otherwise,)X
1673(a)X
5 f
1752(BadValue)X
1 f
2171(error)X
2363(is)X
2444(generated.)X
2852(In)X
2947(addition,)X
3281(the)X
3411(following)X
555 5412(expression)N
953(must)X
1147(be)X
1252(less)X
1406(than)X
1580(or)X
1675(equal)X
1888(to)X
1979(max_keycode)X
2490(as)X
2585(returned)X
2900(in)X
2991(the)X
3121(connection)X
3530(setup)X
3738(\(else)X
3926(a)X
555 5508(BadValue)N
929(error\).)X
843 5680(\256rst_keycode)N
1339(+)X
1411(\(num_codes)X
1863(/)X
1910(keysyms_per_keycode\))X
2762(-)X
2813(1)X
555 5824(The)N
714(KeySym)X
1044(number)X
1335(N,)X
1442(counting)X
1773(from)X
1966(zero,)X
2161(for)X
2285(keycode)X
2600(K)X
2685(has)X
2824(an)X
2929(index,)X
3169(counting)X
3500(from)X
3693(zero,)X
3888(of)X
3 f
2217 6144(138)N

139 p
%%Page: 139 143
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(the)N
685(following)X
1050(in)X
1141(keysyms:)X
1063 844(\(K)N
1177(-)X
1228(\256rst_keycode\))X
1753(*)X
1819(keysyms_per_keycode)X
2642(+)X
2714(N)X
555 988(The)N
714(speci\256ed)X
1049(keysyms_per_keycode)X
1872(can)X
2016(be)X
2121(chosen)X
2387(arbitrarily)X
2762(by)X
2872(the)X
3002(client)X
3221(to)X
3312(be)X
3417(large)X
3615(enough)X
3896(to)X
555 1084(hold)N
734(all)X
845(desired)X
1121(symbols.)X
1459(A)X
1544(special)X
1811(KeySym)X
2141(value)X
2354(of)X
5 f
2467(NoSymbol)X
1 f
2909(should)X
3166(be)X
3271(used)X
3454(to)X
3545(\256ll)X
3666(in)X
3757(unused)X
555 1180(elements)N
891(for)X
1015(individual)X
1396(keycodes.)X
1767(It)X
1843(is)X
1924(legal)X
2118(for)X
5 f
2260(NoSymbol)X
1 f
2702(to)X
2793(appear)X
3049(in)X
3140(nontrailing)X
3550(positions)X
3891(of)X
555 1276(the)N
685(effective)X
1015(list)X
1146(for)X
1270(a)X
1331(keycode.)X
5 f
1711(XChangeKeyboardMapping)X
1 f
2829(generates)X
3183(a)X
5 f
3262(MappingNotify)X
1 f
555 1372(event.)N
555 1496(There)N
782(is)X
863(no)X
973(requirement)X
1421(that)X
1576(the)X
1706(X)X
1791(server)X
2027(interpret)X
2348(this)X
2498(mapping.)X
2851(It)X
2927(is)X
3008(merely)X
3275(stored)X
3512(for)X
3636(reading)X
555 1592(and)N
704(writing)X
981(by)X
1091(clients.)X
555 1716(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XChangeKeyboardMapping)X
1 f
3370(are)X
5 f
3517(BadValue)X
1 f
555 1812(and)N
5 f
722(BadAlloc)X
1 f
1084(.)X
555 1936(The)N
714(next)X
888(two)X
1042(functions)X
1392(make)X
1605(use)X
1744(of)X
1839(the)X
1969(following)X
2334(data)X
2503(structure.)X
555 2060(typedef)N
841(struct)X
1059({)X
665 2156(int)N
781(max_keypermod;)X
1530(/*)X
1621(This)X
1800(server's)X
2099(max)X
2273(number)X
2564(of)X
2659(keys)X
2842(per)X
2976(modi\256er)X
3297(*/)X
665 2252(KeyCode)N
1019(*modi\256ermap;)X
1605(/*)X
1696(An)X
1825(8)X
1891(by)X
2001(max_keypermod)X
2615(array)X
2817(of)X
2912(the)X
3042(modi\256ers)X
3397(*/)X
555 2348(})N
619(XModi\256erKeymap;)X
555 2520(Use)N
5 f
731(XNewModi\256erMapping)X
1 f
1651(to)X
1742(create)X
1974(one)X
2123(of)X
2218(these)X
2421(structures.)X
2807(The)X
2966(de\256nition)X
3327(for)X
3451(this)X
3601(function)X
3917(is:)X
555 2644(XModi\256erKeymap)N
1246(XNewModi\256erMapping\()X
2 f
2136(max_keys_per_mod)X
1 f
2840(\))X
731 2740(int)N
2 f
847(max_keys_per_mod)X
1 f
1551(;)X
2 f
555 2892(max_keys_per_mod)N
1 f
1131 2988(Speci\256es)N
1471(the)X
1601(maximum)X
1982(number)X
2273(of)X
2368(keycodes)X
2717(assigned)X
3042(to)X
3133(any)X
3282(of)X
3377(the)X
3507(modi\256ers)X
3862(in)X
1131 3084(the)N
1261(map.)X
555 3208(The)N
5 f
732(XNewModi\256erMapping)X
1 f
1652(function)X
1968(returns)X
2234(a)X
5 f
2313(XModi\256erKeymap)X
1 f
3044(structure.)X
555 3400(Use)N
5 f
731(XFreeModi\256erMapping)X
1 f
1657(to)X
1748(destroy)X
2029(one)X
2178(of)X
2273(these)X
2476(structures.)X
2862(The)X
3021(de\256nition)X
3382(for)X
3506(this)X
3656(function)X
555 3496(is:)N
555 3620(XFreeModi\256erMapping\()N
2 f
1436(modmap)X
1 f
1745(\))X
731 3716(XModi\256erMapping)N
1442(*)X
2 f
(modmap)S
1 f
1788(;)X
2 f
555 3868(modmap)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XModi\256erKeymap)X
1 f
2774(structure.)X
555 3992(The)N
5 f
732(XFreeModi\256erMapping)X
1 f
1658(function)X
1974(frees)X
2166(the)X
2296(speci\256ed)X
5 f
2649(XModi\256erKeymap)X
1 f
3380(structure.)X
555 4212(Use)N
5 f
731(XSetModi\256erMapping)X
1 f
1609(to)X
1700(set)X
1820(which)X
2057(keycodes)X
2406(are)X
2535(to)X
2626(be)X
2731(used)X
2914(as)X
3009(modi\256ers.)X
3386(The)X
3545(de\256nition)X
3906(for)X
555 4308(this)N
705(function)X
1021(is:)X
555 4432(int)N
671(XSetModi\256erMapping\()X
2 f
1509(display)X
1 f
1764(,)X
2 f
1808(modmap)X
1 f
2117(\))X
731 4528(Display)N
1027(*)X
2 f
(display)S
1 f
1333(;)X
731 4624(XModi\256erKeymap)N
1422(*)X
2 f
(modmap)S
1 f
1775(;)X
2 f
555 4776(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4900(modmap)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
5 f
2043(XModi\256erKeymap)X
1 f
2774(structure.)X
555 5024(The)N
5 f
732(XSetModi\256erMapping)X
1 f
1610(function)X
1926(speci\256es)X
2251(the)X
2381(keycodes)X
2730(of)X
2825(the)X
2955(keys,)X
3160(if)X
3236(any,)X
3407(that)X
3562(are)X
3691(to)X
3782(be)X
555 5120(used)N
738(as)X
833(modi\256ers.)X
1232(A)X
1317(zero)X
1490(value)X
1703(means)X
1950(that)X
2105(no)X
2215(key)X
2364(should)X
2621(be)X
2726(used.)X
2931(No)X
3060(two)X
3214(arguments)X
3603(can)X
3747(have)X
555 5216(the)N
685(same)X
888(nonzero)X
1193(keycode)X
1508(value.)X
1743(Otherwise,)X
2148(a)X
5 f
2227(BadValue)X
1 f
2646(error)X
2838(is)X
2919(generated.)X
555 5340(There)N
782(are)X
911(eight)X
1110(modi\256ers,)X
1487(and)X
1636(the)X
1766(modi\256ermap)X
2239(member)X
2550(of)X
2645(the)X
5 f
2793(XModi\256erKeymap)X
1 f
3524(structure)X
3854(con-)X
555 5436(tains)N
744(eight)X
943(sets)X
1097(of)X
1192(max_keypermod)X
1806(keycodes,)X
2177(one)X
2326(for)X
2450(each)X
2633(modi\256er)X
2954(in)X
3045(the)X
3175(order)X
3382(Shift,)X
3598(Lock,)X
3823(Con-)X
555 5532(trol,)N
722(Mod1,)X
976(Mod2,)X
1230(Mod3,)X
1484(Mod4,)X
1738(and)X
1887(Mod5.)X
2163(Only)X
2361(nonzero)X
2666(keycodes)X
3015(have)X
3203(meaning)X
3529(in)X
3620(each)X
3803(set,)X
555 5628(and)N
704(nonzero)X
1009(keycodes)X
1358(are)X
1487(ignored.)X
1822(In)X
1917(addition,)X
2251(all)X
2362(of)X
2457(the)X
2587(nonzero)X
2892(keycodes)X
3241(must)X
3435(be)X
3540(in)X
3631(the)X
3761(range)X
555 5724(speci\256ed)N
890(by)X
1000(min_keycode)X
1497(and)X
1646(max_keycode)X
2157(in)X
2248(the)X
5 f
2396(Display)X
1 f
2720(structure.)X
3094(Otherwise,)X
3499(a)X
5 f
3578(BadValue)X
1 f
555 5820(error)N
747(is)X
828(generated.)X
1236(No)X
1365(keycode)X
1680(may)X
1854(appear)X
2110(twice)X
2323(in)X
2414(the)X
2544(entire)X
2767(map.)X
2963(Otherwise,)X
3368(a)X
5 f
3447(BadValue)X
3 f
2217 6144(139)N

140 p
%%Page: 140 144
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(error)N
747(is)X
828(generated.)X
555 796(A)N
640(X)X
725(server)X
961(can)X
1105(impose)X
1382(restrictions)X
1796(on)X
1906(how)X
2079(modi\256ers)X
2434(can)X
2578(be)X
2683(changed,)X
3020(for)X
3144(example,)X
3487(if)X
3563(certain)X
3825(keys)X
555 892(do)N
665(not)X
800(generate)X
1120(up)X
1230(transitions)X
1620(in)X
1711(hardware)X
2059(or)X
2154(if)X
2230(multiple)X
2548(modi\256er)X
2869(keys)X
3052(are)X
3181(not)X
3316(supported.)X
3707(If)X
3787(some)X
555 988(such)N
738(restriction)X
1118(is)X
1199(violated,)X
1528(the)X
1658(status)X
1881(reply)X
2084(is)X
5 f
2183(MappingFailed)X
1 f
2774(,)X
2818(and)X
2967(none)X
3160(of)X
3255(the)X
3385(modi\256ers)X
3740(are)X
555 1084(changed.)N
914(If)X
994(the)X
1124(new)X
1292(keycodes)X
1641(speci\256ed)X
1976(for)X
2100(a)X
2161(modi\256er)X
2482(differ)X
2699(from)X
2892(those)X
3100(currently)X
3440(de\256ned)X
3721(and)X
3870(any)X
555 1180(\(current)N
855(or)X
950(new\))X
1147(keys)X
1330(for)X
1454(that)X
1609(modi\256er)X
1930(are)X
2059(in)X
2150(the)X
2280(down)X
2497(state,)X
2703(the)X
2833(status)X
3056(reply)X
3259(is)X
5 f
3358(MappingBusy)X
1 f
3906(,)X
555 1276(and)N
704(none)X
897(of)X
992(the)X
1122(modi\256ers)X
1477(are)X
1606(changed.)X
5 f
1986(XSetModi\256erMapping)X
1 f
2864(generates)X
3218(a)X
5 f
3297(MappingNotify)X
1 f
555 1372(event)N
768(on)X
878(a)X
5 f
957(MappingSuccess)X
1 f
1664(status.)X
555 1496(The)N
714(other)X
917(event)X
1130(errors)X
1356(that)X
1511(can)X
1655(be)X
1760(generated)X
2124(by)X
5 f
2252(XSetModi\256erMapping)X
1 f
3130(are)X
5 f
3277(BadAlloc)X
1 f
3661(and)X
5 f
570 1592(BadValue)N
1 f
967(.)X
555 1812(Use)N
5 f
731(XGetModi\256erMapping)X
1 f
1619(to)X
1710(obtain)X
1953(the)X
2083(keycodes)X
2432(that)X
2587(are)X
2716(to)X
2807(be)X
2912(used)X
3095(as)X
3190(modi\256ers.)X
3567(The)X
555 1908(de\256nition)N
916(for)X
1040(this)X
1190(function)X
1506(is:)X
555 2032(XModi\256erKeymap)N
1246(*XGetModi\256erMapping\()X
2 f
2142(display)X
1 f
2404(\))X
687 2128(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 2376(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 2500(The)N
5 f
732(XGetModi\256erMapping)X
1 f
1620(function)X
1936(returns)X
2202(a)X
2263(newly)X
2500(created)X
5 f
2794(XModi\256erKeymap)X
1 f
3525(structure)X
3855(that)X
555 2596(contains)N
871(the)X
1001(keys)X
1184(being)X
1402(used)X
1585(as)X
1680(modi\256ers.)X
2079(The)X
2238(structure)X
2568(should)X
2825(be)X
2930(freed)X
3132(after)X
3315(use)X
3454(with)X
5 f
570 2692(XFreeModi\256erMapping)N
1 f
1474(.)X
1540(If)X
1620(only)X
1799(zero)X
1972(values)X
2219(appear)X
2475(in)X
2566(the)X
2696(set)X
2816(for)X
2940(any)X
3089(modi\256er,)X
3454(that)X
3609(modi\256er)X
3930(is)X
555 2788(disabled.)N
3 f
555 2980(7.10.)N
775(Manipulating)X
1312(the)X
1451(Screen)X
1727(Saver)X
1 f
555 3104(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(set,)X
2265(force,)X
2489(activate,)X
2808(or)X
2903(reset)X
3091(the)X
3221(screen)X
3467(saver)X
3674(as)X
3769(well)X
555 3200(as)N
650(obtain)X
893(the)X
1023(current)X
1294(screen)X
1540(saver)X
1747(values.)X
555 3420(Use)N
5 f
731(XSetScreenSaver)X
1 f
1469(to)X
1560(set)X
1680(the)X
1810(screen)X
2056(saver.)X
2285(The)X
2444(de\256nition)X
2805(for)X
2929(this)X
3079(function)X
3395(is:)X
555 3544(XSetScreenSaver)N
1177(\()X
2 f
1213(display)X
1 f
1468(,)X
2 f
1512(timeout)X
1 f
1784(,)X
2 f
1828(interval)X
1 f
2110(,)X
2 f
2154(prefer_blanking)X
1 f
2729(,)X
2 f
2773(allow_exposures)X
1 f
3372(\))X
687 3640(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3736(int)N
2 f
803(timeout)X
1 f
1075(,)X
2 f
1119(interval)X
1 f
1401(;)X
687 3832(int)N
2 f
803(prefer_blanking)X
1 f
1378(;)X
687 3928(int)N
2 f
803(allow_exposures)X
1 f
1402(;)X
2 f
555 4080(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 4204(timeout)N
1 f
1131(Speci\256es)X
1471(the)X
1601(timeout,)X
1916(in)X
2007(seconds,)X
2329(until)X
2514(the)X
2644(screen)X
2890(saver)X
3097(turns)X
3295(on.)X
2 f
555 4328(interval)N
1 f
1131(Speci\256es)X
1471(the)X
1601(interval)X
1893(between)X
2208(screen)X
2454(saver)X
2661(invocations.)X
2 f
555 4452(prefer_blanking)N
1 f
1131(Speci\256es)X
1471(whether)X
1776(to)X
1867(enable)X
2119(screen)X
2365(blanking.)X
2740(Possible)X
3056(values)X
3303(are)X
5 f
3450(DontPrefer-)X
1131 4548(Blanking)N
1 f
1483(,)X
5 f
1545(PreferBlanking)X
1 f
2137(,)X
2181(or)X
5 f
2294(DefaultBlanking)X
1 f
2925(.)X
2 f
555 4672(allow_exposures)N
1 f
1147(Speci\256es)X
1487(the)X
1617(current)X
1888(screen)X
2134(save)X
2312(control)X
2584(values.)X
2875(Possible)X
3191(values)X
3438(are)X
5 f
3585(DontAl-)X
1131 4768(lowExposures)N
1 f
1693(,)X
5 f
1755(AllowExposures)X
1 f
2395(,)X
2439(or)X
5 f
2552(DefaultExposures)X
1 f
3262(.)X
555 4892(Timeout)N
877(and)X
1026(interval)X
1318(are)X
1447(speci\256ed)X
1782(in)X
1873(seconds.)X
2195(A)X
2280(timeout)X
2573(of)X
2668(0)X
2734(disables)X
3040(the)X
3170(screen)X
3416(saver,)X
3645(while)X
3863(a)X
555 4988(timeout)N
848(of)X
943(-1)X
1038(restores)X
1333(the)X
1463(default.)X
1774(Other)X
1996(negative)X
2317(values)X
2564(generate)X
2884(a)X
5 f
2963(BadValue)X
1 f
3382(error.)X
3618(If)X
3698(the)X
555 5084(timeout)N
848(value)X
1061(is)X
1142(nonzero,)X
1469(the)X
1599(function)X
1915(enables)X
2201(the)X
2331(screen)X
2577(saver.)X
2828(An)X
2957(interval)X
3249(of)X
3344(0)X
3410(disables)X
3716(the)X
3846(ran-)X
555 5180(dom)N
734(pattern)X
1001(motion.)X
1318(If)X
1398(no)X
1508(input)X
1712(from)X
1905(devices)X
2191(\(keyboard,)X
2591(mouse,)X
2865(and)X
3014(so)X
3114(on\))X
3253(is)X
3334(generated)X
3698(once)X
3886(the)X
555 5276(screen)N
801(saver)X
1008(is)X
1089(enabled,)X
1407(for)X
1531(the)X
1661(speci\256ed)X
1996(number)X
2287(of)X
2382(timeout)X
2675(seconds,)X
2997(the)X
3127(screen)X
3373(saver)X
3580(is)X
3661(activated.)X
555 5400(For)N
699(each)X
882(screen,)X
1150(if)X
1226(blanking)X
1557(is)X
1638(preferred)X
1981(and)X
2130(the)X
2260(hardware)X
2608(supports)X
2928(video)X
3146(blanking,)X
3499(the)X
3629(screen)X
555 5496(will)N
715(simply)X
978(go)X
1088(blank.)X
1328(Otherwise,)X
1733(if)X
1809(either)X
2032(exposures)X
2405(are)X
2534(allowed)X
2835(or)X
2930(the)X
3060(screen)X
3306(can)X
3450(be)X
3555(regenerated)X
555 5592(without)N
847(sending)X
1143(exposure)X
1482(events)X
1729(to)X
1820(clients,)X
2095(the)X
2225(screen)X
2471(is)X
2552(tiled)X
2732(with)X
2911(the)X
3041(root)X
3205(window)X
3510(background)X
555 5688(tile)N
691(at)X
777(randomly)X
1137(re-origined)X
1550(each)X
1733(interval)X
2025(minutes.)X
2371(Otherwise,)X
2776(the)X
2906(state)X
3090(of)X
3185(the)X
3315(screens)X
3595(do)X
3705(not)X
555 5784(change,)N
848(and)X
997(the)X
1127(screen)X
1373(saver)X
1580(is)X
1661(not)X
1796(activated.)X
2181(The)X
2340(screen)X
2586(saver)X
2793(is)X
2874(deactivated)X
3298(and)X
3447(all)X
3558(screen)X
3804(states)X
3 f
2217 6144(140)N

141 p
%%Page: 141 145
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(are)N
684(restored)X
989(at)X
1075(the)X
1205(next)X
1379(keyboard)X
1728(or)X
1823(pointer)X
2095(input)X
2299(or)X
2394(at)X
2480(the)X
2610(next)X
2784(call)X
2934(to)X
5 f
3043(XForceScreenSaver)X
1 f
555 768(with)N
734(mode)X
5 f
970(ScreenSaverReset)X
1 f
1724(.)X
555 892(If)N
635(the)X
765(server-dependent)X
1392(screen)X
1638(saver)X
1845(method)X
2132(supports)X
2452(periodic)X
2763(change,)X
3056(the)X
3186(interval)X
3478(argument)X
555 988(serves)N
796(as)X
891(a)X
952(hint)X
1112(about)X
1330(how)X
1503(long)X
1682(the)X
1812(change)X
2083(period)X
2330(should)X
2587(be,)X
2714(and)X
2863(zero)X
3036(hints)X
3230(that)X
3385(no)X
3495(periodic)X
555 1084(change)N
826(should)X
1083(be)X
1188(made.)X
1445(Examples)X
1815(of)X
1910(ways)X
2112(to)X
2203(change)X
2474(the)X
2604(screen)X
2850(include)X
3132(scrambling)X
3546(the)X
3676(color)X
555 1180(map)N
729(periodically,)X
1195(moving)X
1487(an)X
1592(icon)X
1766(image)X
2004(about)X
2222(the)X
2352(screen)X
2598(periodically,)X
3064(or)X
3159(tiling)X
3369(the)X
3499(screen)X
3745(with)X
555 1276(the)N
685(root)X
849(window)X
1154(background)X
1591(tile,)X
1749(randomly)X
2109(reoriginated)X
2557(periodically.)X
555 1400(The)N
714(other)X
917(error)X
1109(that)X
1264(can)X
1408(be)X
1513(generated)X
1877(by)X
5 f
2005(XSetScreenSaver)X
1 f
2743(is)X
5 f
2842(BadValue)X
1 f
3239(.)X
555 1620(Use)N
5 f
731(XForceScreenSaver)X
1 f
1561(to)X
1652(force)X
1854(the)X
1984(screen)X
2230(saver.)X
2459(The)X
2618(de\256nition)X
2979(for)X
3103(this)X
3253(function)X
3569(is:)X
555 1744(XForceScreenSaver)N
1264(\()X
2 f
1300(display)X
1 f
1562(,)X
2 f
1606(mode)X
1 f
1803(\))X
687 1840(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 1936(int)N
2 f
803(mode)X
1 f
1000(;)X
2 f
555 2088(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2212(mode)N
1 f
1131(Speci\256es)X
1471(the)X
1601(mode)X
1819(that)X
1974(is)X
2055(to)X
2146(be)X
2251(applied.)X
5 f
2598(XForceScreenSaver)X
1 f
3428(applies)X
3700(the)X
1131 2308(speci\256ed)N
1466(mode)X
1684(to)X
1775(the)X
1905(screen)X
2151(saver.)X
2402(The)X
2561(possible)X
2872(modes)X
3124(are)X
5 f
3271(ScreenSaverAc-)X
1131 2404(tive)N
1 f
1305(or)X
5 f
1418(ScreenSaverReset)X
1 f
2172(.)X
555 2528(If)N
635(the)X
765(speci\256ed)X
1100(mode)X
1318(is)X
5 f
1417(ScreenSaverActive)X
1 f
2203(and)X
2352(the)X
2482(screen)X
2728(saver)X
2935(currently)X
3275(is)X
3356(deactivated,)X
3802(the)X
555 2624(screen)N
801(saver)X
1008(is)X
1089(activated,)X
1452(even)X
1640(if)X
1716(the)X
1846(screen)X
2092(saver)X
2299(had)X
2448(been)X
2636(disabled)X
2952(with)X
3131(a)X
3192(timeout)X
3485(of)X
3580(zero)X
3753(\(0\).)X
3921(If)X
555 2720(the)N
685(speci\256ed)X
1020(mode)X
1238(is)X
5 f
1337(ScreenSaverReset)X
1 f
2113(and)X
2262(the)X
2392(screen)X
2638(saver)X
2845(currently)X
3185(is)X
3266(enabled,)X
3584(the)X
3714(screen)X
555 2816(saver)N
762(is)X
843(deactivated)X
1267(\(if)X
1372(it)X
1444(was)X
1602(activated\),)X
1994(and)X
2143(the)X
2273(activation)X
2644(timer)X
2853(is)X
2934(reset)X
3122(to)X
3213(its)X
3319(initial)X
3549(state)X
3733(\(as)X
3857(if)X
555 2912(device)N
807(input)X
1011(had)X
1160(been)X
1348(received\).)X
555 3036(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XForceScreenSaver)X
1 f
2632(is)X
5 f
2731(BadValue)X
1 f
3128(.)X
555 3256(Use)N
5 f
731(XActivateScreenSaver)X
1 f
1650(to)X
1741(activate)X
2038(the)X
2168(screen)X
2414(saver.)X
2643(The)X
2802(de\256nition)X
3163(for)X
3287(this)X
3437(function)X
3753(is:)X
555 3380(XActivateScreenSaver)N
1363(\()X
2 f
1399(display)X
1 f
1661(\))X
687 3476(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 3628(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 3848(Use)N
5 f
731(XResetScreenSaver)X
1 f
1566(to)X
1657(reset)X
1845(the)X
1975(screen)X
2221(saver.)X
2450(The)X
2609(de\256nition)X
2970(for)X
3094(this)X
3244(function)X
3560(is:)X
555 3972(XResetScreenSaver)N
1260(\()X
2 f
1296(display)X
1 f
1558(\))X
687 4068(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 4220(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 4440(Use)N
5 f
731(XGetScreenSaver)X
1 f
1479(to)X
1570(get)X
1700(the)X
1830(current)X
2101(screen)X
2347(saver)X
2554(values.)X
2823(The)X
2982(de\256nition)X
3343(for)X
3467(this)X
3617(function)X
555 4536(is:)N
555 4660(XGetScreenSaver)N
1191(\()X
2 f
1227(display)X
1 f
1482(,)X
2 f
1526(timeout_return)X
1 f
2062(,)X
2 f
2106(interval_return)X
1 f
2652(,)X
2 f
2696(prefer_blanking_return)X
1 f
3535(,)X
2 f
951 4756(allow_exposures_return)N
1 f
1814(\))X
687 4852(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4948(int)N
803(*)X
2 f
(timeout_return)S
1 f
1383(,)X
1427(*)X
2 f
(interval_return)S
1 f
2017(;)X
687 5044(int)N
803(*)X
2 f
(prefer_blanking_return)S
1 f
1686(;)X
687 5140(int)N
803(*)X
2 f
(allow_exposures_return)S
1 f
1710(;)X
2 f
555 5292(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5416(timeout_return)N
1 f
1131(Returns)X
1427(the)X
1557(timeout,)X
1872(in)X
1963(minutes,)X
2287(until)X
2472(the)X
2602(screen)X
2848(saver)X
3055(turns)X
3253(on.)X
2 f
555 5540(interval_return)N
1 f
1131(Returns)X
1427(the)X
1557(interval)X
1849(between)X
2164(screen)X
2410(saver)X
2617(invocations.)X
2 f
555 5664(prefer_blanking_return)N
1 f
1131 5760(Returns)N
1427(the)X
1557(current)X
1828(screen)X
2074(blanking)X
2405(preference:)X
5 f
2840(DontPreferBlanking)X
1 f
3618(,)X
3 f
2217 6144(141)N

142 p
%%Page: 142 146
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
5 f
1146 672(PreferBlanking)N
1 f
1738(,)X
1782(or)X
5 f
1895(DefaultBlanking)X
1 f
2526(.)X
2 f
555 796(allow_exposures_return)N
1 f
1131 892(Returns)N
1427(the)X
1557(current)X
1828(screen)X
2074(save)X
2252(control)X
2524(value:)X
5 f
2780(DontAllowExposures)X
1 f
3606(,)X
5 f
3668(AllowEx-)X
1131 988(posures)N
1 f
1459(,)X
1503(or)X
5 f
1616(DefaultExposures)X
1 f
2326(.)X
3 f
555 1180(7.11.)N
775(Controlling)X
1233(Host)X
1431(Access)X
1 f
555 1304(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(control)X
2395(host)X
2564(access.)X
2854(This)X
3033(section)X
3305(discusses)X
3654(how)X
3827(to:)X
10 f
555 1428(g)N
1 f
775(Add,)X
970(get,)X
1122(or)X
1217(remove)X
1503(hosts)X
1706(from)X
1899(the)X
2029(access)X
2275(control)X
2547(list)X
10 f
555 1552(g)N
1 f
775(Change,)X
1088(enable,)X
1362(or)X
1457(disable)X
1729(access)X
555 1676(X)N
640(does)X
823(not)X
958(provide)X
1249(any)X
1398(protection)X
1778(on)X
1888(a)X
1949(per-window)X
2395(basis.)X
2637(If)X
2717(you)X
2871(\256nd)X
3030(out)X
3165(the)X
3295(resource)X
3614(ID)X
3728(of)X
3823(a)X
555 1772(resource,)N
896(you)X
1050(can)X
1194(manipulate)X
1609(it.)X
1725(To)X
1845(provide)X
2136(some)X
2344(minimal)X
2662(level)X
2856(of)X
2951(protection,)X
3353(however,)X
3699(connec-)X
555 1868(tions)N
749(are)X
878(only)X
1057(permitted)X
1418(from)X
1611(machines)X
1966(you)X
2120(trust.)X
2343(This)X
2522(is)X
2603(adequate)X
2938(on)X
3048(single)X
3281(user)X
3449(workstations,)X
555 1964(but)N
690(obviously)X
1060(breaks)X
1311(down)X
1528(on)X
1638(timesharing)X
2077(machines.)X
2476(While)X
2714(provisions)X
3103(exist)X
3292(in)X
3383(the)X
3513(X)X
3598(protocol)X
555 2060(for)N
679(proper)X
930(connection)X
1339(authentication,)X
1884(the)X
2014(lack)X
2183(of)X
2278(a)X
2339(standard)X
2659(authentication)X
3182(server)X
3418(leaves)X
3660(us)X
3760(with)X
555 2156(only)N
734(host)X
903(level)X
1097(access)X
1343(control.)X
1659(Currently,)X
2041(you)X
2195(can)X
2339(use)X
2478(both)X
2657(DECnet)X
2963(and)X
3112(TCP)X
3296(domains.)X
555 2280(The)N
714(initial)X
944(set)X
1064(of)X
1159(hosts)X
1362(allowed)X
1663(to)X
1754(open)X
1947(connections)X
2390(consists)X
2691(of:)X
10 f
555 2404(g)N
1 f
775(The)X
934(host)X
1103(the)X
1233(window)X
1538(system)X
1805(is)X
1886(running)X
2182(on.)X
10 f
555 2528(g)N
1 f
775(On)X
904(UNIX-based)X
1373(systems,)X
1696(each)X
1879(host)X
2048(is)X
2129(listed)X
2343(in)X
2434(the)X
5 f
2582 0.2813(/etc/X?.hosts)AX
1 f
3131(\256le.)X
3310(The)X
3469(`?')X
3588(indicates)X
775 2624(the)N
905(number)X
1196(of)X
1291(the)X
1421(display.)X
1742(This)X
1921(\256le)X
2056(should)X
2313(consist)X
2580(of)X
2675(host)X
2844(names)X
3091(separated)X
3445(by)X
3555(newlines.)X
775 2720(DECnet)N
1081(nodes)X
1308(must)X
1502(terminate)X
1858(in)X
1949(``::'')X
2137(to)X
2228(distinguish)X
2638(them)X
2837(from)X
3030(internet)X
3322(hosts.)X
555 2844(If)N
635(a)X
696(host)X
865(is)X
946(not)X
1081(in)X
1172(the)X
1302(access)X
1548(control)X
1820(list)X
1951(when)X
2163(the)X
2293(access)X
2539(control)X
2811(mechanism)X
3235(is)X
3316(enabled)X
3612(and)X
3761(if)X
3837(the)X
555 2940(host)N
724(attempts)X
1046(to)X
1137(establish)X
1468(a)X
1529(connection,)X
1960(the)X
2090(server)X
2326(refuses)X
2596(the)X
2726(connection)X
3135(list)X
3266(or)X
3361(to)X
3452(change)X
3723(the)X
555 3036(access)N
801(list.)X
976(The)X
1135(client)X
1354(must)X
1548(reside)X
1780(on)X
1890(the)X
2020(same)X
2223(host)X
2392(as)X
2487(the)X
2617(server)X
2853(and/or)X
3100(must)X
3294(have)X
3482(been)X
3670(granted)X
555 3132(permission)N
964(in)X
1055(the)X
1185(initial)X
1415(authorization)X
1903(at)X
1989(connection)X
2398(setup.)X
2650(The)X
2809(initial)X
3039(access)X
3285(control)X
3557(list)X
3688(can)X
3832(be)X
555 3228(speci\256ed)N
890(by)X
1000(providing)X
1365(a)X
1426(\256le)X
1561(that)X
1716(the)X
1846(server)X
2082(can)X
2226(read)X
2399(at)X
2485(startup)X
2747(and)X
2896(reset)X
3084(time.)X
2197 3404(Note)N
775 3540(Servers)N
1060(also)X
1224(can)X
1368(implement)X
1769(other)X
1972(access)X
2218(control)X
2490(policies)X
2787(in)X
2878(addition)X
3190(to)X
3281(or)X
3376(in)X
3467(place)X
3675(of)X
775 3636(this)N
925(hosts)X
1128(access)X
1374(facility.)X
1691(The)X
1850(name)X
2063(and)X
2212(format)X
2469(of)X
2564(the)X
2694(information)X
3133(in)X
3224(this)X
3374(\256le)X
3509(is)X
775 3732(operating)N
1130(system)X
1397(speci\256c.)X
1732(For)X
1876(further)X
2137(information)X
2576(about)X
2794(other)X
2997(access)X
3243(control)X
3515(imple-)X
775 3828(mentations,)N
1207(see)X
2 f
1341(X)X
1417(Window)X
1728(System)X
1994(Protocol,)X
2347(Version)X
2643(11)X
1 f
(.)S
3 f
555 4100(7.11.1.)N
841(Adding,)X
1164(Getting,)X
1492(or)X
1597(Removing)X
2000(Hosts)X
1 f
555 4224(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(add,)X
2294(get,)X
2446(or)X
2541(remove)X
2827(hosts.)X
3074(All)X
3209(the)X
3339(host)X
3508(access)X
3754(control)X
555 4320(functions)N
905(use)X
1044(the)X
5 f
1192(XHostAddress)X
1 f
1792(structure.)X
2166(The)X
2325(elements)X
2661(in)X
2752(this)X
2902(structure)X
3232(are:)X
555 4444(typedef)N
841(struct)X
1059({)X
843 4540(int)N
959(family;)X
2283(/*)X
2374(for)X
2498(example)X
2819(AF_DNET)X
3231(*/)X
843 4636(int)N
959(length;)X
2283(/*)X
2374(length)X
2617(of)X
2712(address,)X
3019(in)X
3110(bytes)X
3318(*/)X
843 4732(char)N
1016(*address;)X
2283(/*)X
2374(pointer)X
2646(to)X
2737(where)X
2973(to)X
3064(\256nd)X
3223(the)X
3353(bytes)X
3561(*/)X
555 4828(})N
619(XHostAddress;)X
2 f
555 5000(family)N
1 f
1131(Speci\256es)X
1471(which)X
1708(protocol)X
2024(address)X
2309(family)X
2562(to)X
2653(use)X
2792(\(for)X
2945(example,)X
3288(TCP/IP)X
3575(or)X
3670(DECnet\).)X
1131 5096(The)N
1290(family)X
1543(symbols)X
1859(are)X
1988(de\256ned)X
2269(in)X
5 f
2378(<X11/X.h>)X
1 f
2810(.)X
2 f
555 5220(length)N
1 f
1131(Speci\256es)X
1471(the)X
1601(length)X
1844(of)X
1939(the)X
2069(address)X
2354(in)X
2445(bytes.)X
2 f
555 5344(address)N
1 f
1131(Speci\256es)X
1471(a)X
1532(pointer)X
1804(to)X
1895(the)X
2025(address.)X
555 5564(Use)N
5 f
731(XAddHost)X
1 f
1165(to)X
1256(add)X
1405(a)X
1466(single)X
1699(host.)X
1890(The)X
2049(de\256nition)X
2410(for)X
2534(this)X
2684(function)X
3000(is:)X
3 f
2217 6144(142)N

143 p
%%Page: 143 147
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(XAddHost)N
942(\()X
2 f
978(display)X
1 f
1233(,)X
2 f
1277(host)X
1 f
1431(\))X
687 768(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 864(XHostAddress)N
1225(*)X
2 f
(host)S
1 f
1423(;)X
2 f
555 1016(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1140(host)N
1 f
1131(Speci\256es)X
1471(the)X
1601(network)X
1911(address)X
2196(of)X
2291(the)X
2421(host)X
2590(machine.)X
555 1264(The)N
5 f
732(XAddHost)X
1 f
1166(function)X
1482(adds)X
1665(the)X
1795(speci\256ed)X
2130(host)X
2299(to)X
2390(the)X
2520(access)X
2766(control)X
3038(list)X
3169(for)X
3293(that)X
3448(display.)X
3769(The)X
555 1360(display)N
832(hardware)X
1180(must)X
1374(be)X
1479(on)X
1589(the)X
1719(same)X
1922(host)X
2091(as)X
2186(the)X
2316(program)X
2636(issuing)X
2908(the)X
3038(command.)X
555 1484(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XAddHost)X
1 f
2270(are)X
5 f
2417(BadAlloc)X
1 f
2801(and)X
5 f
2968(BadValue)X
1 f
3365(.)X
555 1704(Use)N
5 f
731(XAddHosts)X
1 f
1209(to)X
1300(add)X
1449(multiple)X
1767(hosts)X
1970(at)X
2056(one)X
2205(time.)X
2407(The)X
2566(de\256nition)X
2927(for)X
3051(this)X
3201(function)X
3517(is:)X
555 1828(XAddHosts)N
976(\()X
2 f
1012(display)X
1 f
1267(,)X
2 f
1311(hosts)X
1 f
1492(,)X
2 f
1536(num_hosts)X
1 f
1919(\))X
687 1924(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 2020(XHostAddress)N
1225(*)X
2 f
(hosts)S
1 f
1457(;)X
687 2116(int)N
2 f
803(num_hosts)X
1 f
1186(;)X
2 f
555 2268(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 2392(hosts)N
1 f
1131(Speci\256es)X
1471(each)X
1654(host)X
1823(that)X
1978(is)X
2059(to)X
2150(be)X
2255(added.)X
2 f
555 2516(num_hosts)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(hosts.)X
555 2640(The)N
5 f
732(XAddHosts)X
1 f
1210(function)X
1526(adds)X
1709(each)X
1892(speci\256ed)X
2227(host)X
2396(to)X
2487(the)X
2617(access)X
2863(control)X
3135(list)X
3266(for)X
3390(that)X
3545(display.)X
3866(The)X
555 2736(display)N
832(hardware)X
1180(must)X
1374(be)X
1479(on)X
1589(the)X
1719(same)X
1922(host)X
2091(as)X
2186(the)X
2316(program)X
2636(issuing)X
2908(the)X
3038(command.)X
555 2860(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XAddHosts)X
1 f
2314(are)X
5 f
2461(BadAlloc)X
1 f
2845(and)X
5 f
3012(BadValue)X
1 f
3409(.)X
555 3080(Use)N
5 f
731(XListHosts)X
1 f
1189(to)X
1280(obtain)X
1523(a)X
1584(host)X
1753(list.)X
1906(The)X
2065(de\256nition)X
2426(for)X
2550(this)X
2700(function)X
3016(is:)X
555 3204(XHostAddress)N
1093(*XListHosts)X
1545(\()X
2 f
1581(display)X
1 f
1836(,)X
2 f
1880(nhosts_return)X
1 f
2369(,)X
2 f
2413(state_return)X
1 f
2851(\))X
687 3300(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3396(int)N
803(*)X
2 f
(nhosts_return)S
1 f
1343(;)X
687 3492(Bool)N
881(*)X
2 f
(state_return)S
1 f
1363(;)X
2 f
555 3644(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3768(nhosts_return)N
1 f
1131(Returns)X
1427(the)X
1557(number)X
1848(of)X
1943(hosts)X
2146(currently)X
2486(in)X
2577(the)X
2707(access)X
2953(control)X
3225(list.)X
2 f
555 3892(state_return)N
1 f
1131(Returns)X
1427(the)X
1557(state)X
1741(of)X
1836(the)X
1966(access)X
2212(control)X
2484(\(enabled)X
2809(or)X
2904(disabled\).)X
555 4016(The)N
5 f
732(XListHosts)X
1 f
1190(function)X
1506(returns)X
1772(the)X
1902(current)X
2173(access)X
2419(control)X
2691(list)X
2822(as)X
2917(well)X
3091(as)X
3186(whether)X
3491(the)X
3621(use)X
3760(of)X
3855(the)X
555 4112(list)N
686(at)X
772(connection)X
1181(setup)X
1389(was)X
1547(enabled)X
1843(or)X
1938(disabled.)X
5 f
2319(XListHosts)X
1 f
2777(allows)X
3029(a)X
3090(program)X
3410(to)X
3501(\256nd)X
3660(out)X
3795(what)X
555 4208(machines)N
910(can)X
1054(make)X
1267(connections.)X
1754(It)X
1830(also)X
1994(returns)X
2260(a)X
2321(pointer)X
2593(to)X
2684(a)X
2745(list)X
2876(of)X
2971(host)X
3140(structures)X
3504(that)X
3659(were)X
555 4304(allocated)N
896(by)X
1006(the)X
1136(routine.)X
1430(When)X
1662(it)X
1734(no)X
1844(longer)X
2091(is)X
2172(needed,)X
2465(this)X
2615(memory)X
2931(should)X
3188(be)X
3293(freed)X
3495(by)X
3605(calling)X
5 f
570 4400(XFree)N
1 f
825(.)X
891(\(See)X
1069(Section)X
1356(2.4)X
1488(for)X
1612(further)X
1873(information.\))X
555 4620(Use)N
5 f
731(XRemoveHost)X
1 f
1335(to)X
1426(remove)X
1712(a)X
1773(single)X
2006(host.)X
2197(The)X
2356(de\256nition)X
2717(for)X
2841(this)X
2991(function)X
3307(is:)X
555 4744(XRemoveHost)N
1085(\()X
2 f
1121(display)X
1 f
1376(,)X
2 f
1420(host)X
1 f
1574(\))X
687 4840(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 4936(XHostAddress)N
1225(*)X
2 f
(host)S
1 f
1423(;)X
2 f
555 5088(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 5212(host)N
1 f
1131(Speci\256es)X
1471(the)X
1601(network)X
1911(address)X
2196(of)X
2291(the)X
2421(host)X
2590(machine.)X
555 5336(The)N
5 f
732(XRemoveHost)X
1 f
1336(function)X
1652(removes)X
1972(the)X
2102(speci\256ed)X
2437(host)X
2606(from)X
2799(the)X
2929(access)X
3175(control)X
3447(list)X
3578(for)X
3702(that)X
555 5432(display.)N
876(The)X
1035(display)X
1312(hardware)X
1660(must)X
1854(be)X
1959(on)X
2069(the)X
2199(same)X
2402(host)X
2571(as)X
2666(the)X
2796(client)X
3015(process.)X
3344(If)X
3424(you)X
3578(remove)X
555 5528(your)N
738(machine)X
1059(from)X
1252(the)X
1382(access)X
1628(list,)X
1781(you)X
1935(can)X
2079(no)X
2189(longer)X
2436(connect)X
2732(to)X
2823(that)X
2978(server,)X
3236(and)X
3385(this)X
3535(operation)X
555 5624(cannot)N
812(be)X
917(reversed)X
1236(short)X
1434(of)X
1529(resetting)X
1855(the)X
1985(server.)X
555 5748(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XRemoveHost)X
1 f
2440(are)X
5 f
2587(BadAlloc)X
1 f
2971(and)X
5 f
3138(BadValue)X
1 f
3535(.)X
3 f
2217 6144(143)N

144 p
%%Page: 144 148
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(Use)N
5 f
731(XRemoveHosts)X
1 f
1379(to)X
1470(remove)X
1756(multiple)X
2074(hosts)X
2277(at)X
2363(one)X
2512(time.)X
2714(The)X
2873(de\256nition)X
3234(for)X
3358(this)X
3508(function)X
3824(is:)X
555 796(XRemoveHosts)N
1119(\()X
2 f
1155(display)X
1 f
1410(,)X
2 f
1454(hosts)X
1 f
1635(,)X
2 f
1679(num_hosts)X
1 f
2062(\))X
687 892(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 988(XHostAddress)N
1225(*)X
2 f
(hosts)S
1 f
1457(;)X
687 1084(int)N
2 f
803(num_hosts)X
1 f
1186(;)X
2 f
555 1236(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 1360(hosts)N
1 f
1131(Speci\256es)X
1471(each)X
1654(host)X
1823(that)X
1978(is)X
2059(to)X
2150(be)X
2255(added.)X
2 f
555 1484(num_hosts)N
1 f
1131(Speci\256es)X
1471(the)X
1601(number)X
1892(of)X
1987(hosts.)X
555 1608(The)N
5 f
732(XRemoveHosts)X
1 f
1380(function)X
1696(removes)X
2016(each)X
2199(speci\256ed)X
2534(host)X
2703(from)X
2896(the)X
3026(access)X
3272(control)X
3544(list)X
3675(for)X
3799(that)X
555 1704(display.)N
876(The)X
1035(display)X
1312(hardware)X
1660(must)X
1854(be)X
1959(on)X
2069(the)X
2199(same)X
2402(host)X
2571(as)X
2666(the)X
2796(client)X
3015(process.)X
3344(The)X
5 f
3521(XRemo-)X
555 1800(veHosts)N
1 f
910(function)X
1226(removes)X
1546(each)X
1729(speci\256ed)X
2064(host)X
2233(from)X
2426(the)X
2556(access)X
2802(control)X
3074(list)X
3205(for)X
3329(that)X
3484(display.)X
3783(The)X
555 1896(display)N
832(hardware)X
1180(must)X
1374(be)X
1479(on)X
1589(the)X
1719(same)X
1922(host)X
2091(as)X
2186(the)X
2316(client)X
2535(process.)X
2864(If)X
2944(you)X
3098(remove)X
3384(your)X
3567(machine)X
555 1992(from)N
748(the)X
878(access)X
1124(list,)X
1277(you)X
1431(can)X
1575(no)X
1685(longer)X
1932(connect)X
2228(to)X
2319(that)X
2474(server,)X
2732(and)X
2881(this)X
3031(operation)X
3386(cannot)X
3643(be)X
555 2088(reversed)N
874(short)X
1072(of)X
1167(resetting)X
1493(the)X
1623(server.)X
555 2212(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XRemoveHosts)X
1 f
2484(are)X
5 f
2631(BadAlloc)X
1 f
3015(and)X
5 f
3182(BadValue)X
1 f
3579(.)X
3 f
555 2404(7.11.2.)N
841(Changing,)X
1252(Enabling,)X
1640(or)X
1745(Disabling)X
2125(Access)X
2395(Control)X
1 f
555 2528(Xlib)N
734(provides)X
1059(functions)X
1409(with)X
1588(which)X
1825(you)X
1979(can)X
2123(enable,)X
2397(disable,)X
2691(or)X
2786(change)X
3057(access)X
3303(control.)X
555 2652(For)N
699(these)X
902(functions)X
1252(to)X
1343(execute)X
1634(successfully,)X
2108(the)X
2238(client)X
2457(application)X
2872(must)X
3066(reside)X
3298(on)X
3408(the)X
3538(same)X
3741(host)X
3910(as)X
555 2748(the)N
685(X)X
770(server,)X
1028(and/or)X
1275(have)X
1463(been)X
1651(given)X
1869(permission)X
2278(in)X
2369(the)X
2499(initial)X
2729(authorization)X
3217(at)X
3303(connection)X
3712(setup.)X
555 2968(Use)N
5 f
731(XSetAccessControl)X
1 f
1527(to)X
1618(change)X
1889(access)X
2135(control.)X
2429(The)X
2588(de\256nition)X
2949(for)X
3073(this)X
3223(function)X
3539(is:)X
555 3092(XSetAccessControl)N
1256(\()X
2 f
1292(display)X
1 f
1547(,)X
2 f
1591(mode)X
1 f
1788(\))X
687 3188(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
687 3284(int)N
2 f
803(mode)X
1 f
1000(;)X
2 f
555 3436(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
2 f
555 3560(mode)N
1 f
1131(Speci\256es)X
1471(whether)X
1776(you)X
1930(want)X
2123(to)X
2214(change)X
2485(the)X
2615(access)X
2861(control)X
3133(to)X
3224(enable)X
3476(or)X
3571(disable.)X
5 f
1146 3656(EnableAccess)N
1 f
1741(enables)X
2027(host)X
2196(access)X
2442(control)X
2714(or)X
5 f
2827(DisableAccess)X
1 f
3440(disables)X
3746(host)X
1131 3752(access)N
1377(control.)X
555 3876(The)N
5 f
732(XSetAccessControl)X
1 f
1528(function)X
1844(either)X
2067(enables)X
2353(or)X
2448(disables)X
2754(the)X
2884(use)X
3023(of)X
3118(the)X
3248(access)X
3494(control)X
3766(list)X
3897(at)X
555 3972(connection)N
964(setups.)X
555 4096(The)N
714(errors)X
940(that)X
1095(can)X
1239(be)X
1344(generated)X
1708(by)X
5 f
1836(XSetAccessControl)X
1 f
2632(are)X
5 f
2779(BadAlloc)X
1 f
3163(and)X
5 f
3330(BadAccess)X
1 f
3786(.)X
555 4316(Use)N
5 f
731(XEnableAccessControl)X
1 f
1668(to)X
1759(enable)X
2011(access)X
2257(control.)X
2551(The)X
2710(de\256nition)X
3071(for)X
3195(this)X
3345(function)X
3661(is:)X
555 4440(XEnableAccessControl)N
1388(\()X
2 f
1424(display)X
1 f
1686(\))X
687 4536(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 4688(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 4812(The)N
5 f
732(XEnableAccessControl)X
1 f
1669(function)X
1985(enables)X
2271(the)X
2401(use)X
2540(of)X
2635(the)X
2765(access)X
3011(control)X
3283(list)X
3414(at)X
3500(connection)X
555 4908(setups.)N
555 5032(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XEnableAccessControl)X
1 f
2739(is)X
5 f
2838(BadAccess)X
1 f
3294(.)X
555 5252(Use)N
5 f
731(XDisableAccessControl)X
1 f
1686(to)X
1777(disable)X
2049(access)X
2295(control.)X
2589(The)X
2748(de\256nition)X
3109(for)X
3233(this)X
3383(function)X
3699(is:)X
555 5376(XDisableAccessControl)N
1412(\()X
2 f
1448(display)X
1 f
1710(\))X
687 5472(Display)N
983(*)X
2 f
(display)S
1 f
1289(;)X
2 f
555 5624(display)N
1 f
1131(Speci\256es)X
1471(the)X
1601(connection)X
2010(to)X
2101(the)X
2231(X)X
2316(server.)X
555 5748(The)N
5 f
732(XDisableAccessControl)X
1 f
1687(function)X
2003(disables)X
2309(the)X
2439(use)X
2578(of)X
2673(the)X
2803(access)X
3049(control)X
3321(list)X
3452(at)X
3538(connection)X
555 5844(setups.)N
3 f
2217 6144(144)N

145 p
%%Page: 145 149
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(error)X
906(that)X
1061(can)X
1205(be)X
1310(generated)X
1674(by)X
5 f
1802(XDisableAccessControl)X
1 f
2757(is)X
5 f
2856(BadAccess)X
1 f
3312(.)X
3 f
2217 6144(145)N

146 p
%%Page: 146 150
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
12 s
2076 864(Chapter)N
2441(8)X
1501 1056(Events)N
1802(and)X
1980(Event-Handling)X
2660(Functions)X
1 f
11 s
555 1372(A)N
640(client)X
859(application)X
1274(communicates)X
1806(with)X
1985(the)X
2115(X)X
2200(server)X
2436(through)X
2732(the)X
2862(connection)X
3271(you)X
3425(establish)X
3756(with)X
555 1468(the)N
5 f
703(XOpenDisplay)X
1 f
1302(function.)X
1662(It)X
1738(is)X
1819(over)X
1997(this)X
2147(connection)X
2556(that)X
2711(a)X
2772(client)X
2991(application)X
3406(sends)X
555 1564(``requests'')N
981(to)X
1072(the)X
1202(X)X
1287(server.)X
1567(These)X
1799(requests)X
2109(are)X
2238(made)X
2451(by)X
2561(the)X
2691(Xlib)X
2870(functions)X
3220(that)X
3375(are)X
3504(called)X
3737(in)X
3828(the)X
555 1660(client)N
774(application.)X
1233(The)X
1392(X)X
1477(server)X
1713(sends)X
1930(back)X
2118(to)X
2209(the)X
2339(client)X
2558(application)X
2973(either)X
3196(``replies'')X
3569(or)X
3664(events.)X
555 1756(Most)N
758(requests)X
1068(made)X
1281(by)X
1391(Xlib)X
1570(functions)X
1920(do)X
2030(not)X
2165(generate)X
2485(replies.)X
2786(Some)X
3009(requests)X
3319(generate)X
3639(multiple)X
555 1852(replies.)N
856(Numerous)X
1244(Xlib)X
1423(functions)X
1773(cause)X
1990(the)X
2120(X)X
2205(server)X
2441(to)X
2532(generate)X
2852(events.)X
3143(In)X
3238(addition,)X
3572(the)X
3702(user's)X
555 1948(typing)N
803(or)X
898(moving)X
1190(the)X
1320(pointer)X
1592(can)X
1736(generate)X
2056(events)X
2303(asynchronously.)X
555 2072(This)N
734(chapter)X
1015(begins)X
1267(with)X
1446(a)X
1507(discussion)X
1896(of)X
1991(the)X
2121(following)X
2486(topics)X
2719(associated)X
3103(with)X
3282(events:)X
10 f
555 2196(g)N
1 f
775(Event)X
1003(types)X
10 f
555 2320(g)N
1 f
775(Event)X
1003(structures)X
10 f
555 2444(g)N
1 f
775(Event)X
1003(mask)X
10 f
555 2568(g)N
1 f
775(Event)X
1003(processing)X
555 2692(The)N
714(chapter)X
995(continues)X
1355(with)X
1534(a)X
1595(discussion)X
1984(of)X
2079(the)X
2209(Xlib)X
2388(functions)X
2738(you)X
2892(can)X
3036(use)X
3175(to:)X
10 f
555 2816(g)N
1 f
775(Select)X
1013(events)X
10 f
555 2940(g)N
1 f
775(Handle)X
1051(the)X
1181(output)X
1429(buffer)X
1665(and)X
1814(the)X
1944(event)X
2157(queue)X
10 f
555 3064(g)N
1 f
775(Select)X
1013(events)X
1260(from)X
1453(the)X
1583(event)X
1796(queue)X
10 f
555 3188(g)N
1 f
775(Send)X
973(and)X
1122(get)X
1252(events)X
10 f
555 3312(g)N
1 f
775(Handle)X
1051(error)X
1243(events)X
2197 3488(Note)N
775 3624(Some)N
998(toolkits)X
1286(use)X
1425(their)X
1609(own)X
1782(event-handling)X
2333(routines.)X
2683(Also,)X
2893(some)X
3101(toolkits)X
3389(do)X
3499(not)X
775 3720(allow)N
993(you)X
1147(to)X
1238(interchange)X
1671(these)X
1874(event-handling)X
2425(routines)X
2731(with)X
2910(those)X
3118(in)X
3209(the)X
3339(Xlib)X
3518(library.)X
775 3816(See)N
924(the)X
1054(document)X
1424(supplied)X
1745(with)X
1924(your)X
2107(toolkit)X
2361(for)X
2485(further)X
2746(information.)X
555 4020(Most)N
758(applications)X
1207(simply)X
1470(are)X
1599(event)X
1812(loops.)X
2069(That)X
2253(is,)X
2356(they)X
2530(wait)X
2704(for)X
2828(an)X
2933(event,)X
3168(decide)X
3420(what)X
3613(to)X
3704(do)X
3814(with)X
555 4116(it,)N
649(execute)X
940(some)X
1148(amount)X
1435(of)X
1530(code,)X
1740(which,)X
1999(in)X
2090(turn,)X
2276(results)X
2528(in)X
2619(changes)X
2924(to)X
3015(the)X
3145(display,)X
3444(and)X
3593(then)X
3767(wait)X
555 4212(for)N
679(the)X
809(next)X
983(event.)X
3 f
555 4404(8.1.)N
731(Event)X
973(Types)X
1 f
555 4528(An)N
684(event)X
897(is)X
978(data)X
1147(generated)X
1511(asynchronously)X
2085(by)X
2195(the)X
2325(X)X
2410(server)X
2646(as)X
2741(a)X
2802(result)X
3020(of)X
3115(some)X
3323(device)X
3575(activity,)X
3885(or)X
555 4624(as)N
650(side)X
814(effects)X
1070(of)X
1165(a)X
1226(request)X
1502(sent)X
1666(by)X
1776(an)X
1881(Xlib)X
2060(function.)X
2420(Device-related)X
2960(events)X
3207(propagate)X
3576(from)X
3769(the)X
555 4720(source)N
806(window)X
1111(to)X
1202(ancestor)X
1517(windows)X
1856(until)X
2041(some)X
2249(client)X
2468(application)X
2883(has)X
3022(selected)X
3328(that)X
3483(event)X
3696(type,)X
3892(or)X
555 4816(until)N
740(the)X
870(event)X
1083(is)X
1164(explicitly)X
1521(discarded.)X
1924(The)X
2083(X)X
2168(server)X
2404(never)X
2621(sends)X
2838(an)X
2943(event)X
3156(to)X
3247(a)X
3308(client)X
3527(application,)X
555 4912(unless)N
797(the)X
927(client)X
1146(has)X
1285(speci\256cally)X
1709(asked)X
1931(to)X
2022(be)X
2127(informed)X
2472(of)X
2567(that)X
2722(event)X
2935(type,)X
3131(usually)X
3408(by)X
3518(calling)X
3781(the)X
555 5008(Xlib)N
734(function)X
5 f
1068(XSelectInput)X
1 f
1584(.)X
1650(The)X
1809(mask)X
2017(can)X
2161(also)X
2325(be)X
2430(set)X
2550(when)X
2762(you)X
2916(create)X
3148(a)X
3209(window)X
3514(or)X
3609(by)X
3719(chang-)X
555 5104(ing)N
690(the)X
820(window's)X
1188(event_mask.)X
1675(You)X
1848(can)X
1992(also)X
2156(mask)X
2364(out)X
2499(events)X
2746(that)X
2901(would)X
3143(propagate)X
3512(to)X
3603(outer)X
3806(win-)X
555 5200(dows)N
762(by)X
872(manipulating)X
1361(the)X
1491(do_not_propagate)X
2149(mask)X
2357(of)X
2452(the)X
2582(window's)X
2950(attributes.)X
555 5324(The)N
714(event)X
927(type)X
1101(describes)X
1450(a)X
1511(speci\256c)X
1802(event)X
2015(generated)X
2379(by)X
2489(the)X
2619(X)X
2704(server.)X
2984(For)X
3128(each)X
3311(event)X
3524(type,)X
3720(there)X
3918(is)X
555 5420(a)N
616(corresponding)X
1141(constant)X
1457(name)X
1670(de\256ned)X
1951(in)X
5 f
2060(<X11/X.h>)X
1 f
2492(.)X
2558(When)X
2790(referring)X
3119(to)X
3210(an)X
3315(event)X
3528(type,)X
3724(this)X
555 5516(manual)N
837(uses)X
1010(the)X
1140(constant)X
1456(name)X
1669(de\256ned)X
1950(in)X
2041(this)X
2191(\256le.)X
2370(It)X
2446(is)X
2527(often)X
2730(useful)X
2967(to)X
3058(group)X
3285(one)X
3434(or)X
3529(more)X
3732(event)X
555 5612(types)N
763(into)X
923(logical)X
1186(categories.)X
1609(For)X
1753(example,)X
2096(exposure)X
2435(processing)X
2833(refers)X
3054(to)X
3145(the)X
3275(processing)X
3673(that)X
555 5708(occurs)N
806(for)X
930(the)X
1060(exposure)X
1399(events)X
5 f
1664(Expose)X
1 f
1973(,)X
5 f
2035(GraphicsExpose)X
1 f
2696(,)X
2740(and)X
5 f
2907(NoExpose)X
1 f
3328(.)X
3 f
2217 6144(146)N

147 p
%%Page: 147 151
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(The)N
714(following)X
1079(table)X
1273(lists)X
1438(the)X
1568(event)X
1781(category)X
2106(and)X
2255(its)X
2361(associated)X
2745(event)X
2958(type)X
3132(or)X
3227(types.)X
3457(The)X
3616(processing)X
555 768(associated)N
939(with)X
1118(these)X
1321(events)X
1568(is)X
1649(discussed)X
2008(in)X
2099(Section)X
2386(8.4.)X
10 f
555 880(i)N
567(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1024(Event)N
783(Category)X
1983(Event)X
2211(Type)X
10 f
555 1080(i)N
567(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
1 f
555 1224(Keyboard)N
923(events)X
1983(KeyPress,)X
2358(KeyRelease)X
555 1368(Pointer)N
832(motion)X
1105(events)X
1983(ButtonPress,)X
2453(ButtonRelease,)X
3012(MotionNotify)X
555 1512(Window)N
880(crossing)X
1195(events)X
1983(EnterNotify,)X
2448(LeaveNotify)X
1983 1656(FocusIn,)N
2310(FocusOut)X
555(Input)X
763(focus)X
975(events)X
1983 1800(KeymapNotify)N
555(Key)X
723(map)X
897(state)X
1081(noti\256cation)X
1506(event)X
555 1944(Exposure)N
909(events)X
1983(Expose,)X
2286(GraphicsExpose,)X
2906(NoExpose)X
555 2088(Structure)N
900(control)X
1172(events)X
1983(CirculateRequest,)X
2635(Con\256gureRequest,)X
3315(MapRequest,)X
1983 2184(ResizeRequest)N
555 2328(Window)N
880(state)X
1064(noti\256cation)X
1489(events)X
1983(CirculateNotify,)X
2581(Con\256gureNotify,)X
3207(CreateNotify,)X
3711(Des-)X
1983 2424(troyNotify,)N
2399(GravityNotify,)X
2942(MapNotify,)X
3377(MappingNotify,)X
1983 2520(ReparentNotify,)N
2575(UnmapNotify,)X
3108(VisibilityNotify)X
1983 2664(ColormapNotify)N
555(Color)X
778(map)X
952(state)X
1136(noti\256cation)X
1561(event)X
555 2808(Client)N
794(communication)X
1366(events)X
1983(ClientMessage,)X
2551(PropertyNotify,)X
3128(SelectionClear,)X
3692(Selec-)X
1983 2904(tionNotify,)N
2395(SelectionRequest)X
10 f
555 2960(i)N
567(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)X
3 f
555 3200(8.2.)N
731(Event)X
973(Structures)X
1 f
555 3324(Each)N
753(event)X
966(type)X
1140(has)X
1279(a)X
1340(corresponding)X
1865(structure)X
2195(declared)X
2515(in)X
5 f
2624(<X11/Xlib.h>)X
1 f
3143(.)X
3209(All)X
3344(event)X
3557(structures)X
555 3420(have)N
743(a)X
804(member)X
1115(called)X
1348(type,)X
1544(which)X
1781(the)X
1911(X)X
1996(server)X
2232(sets)X
2386(to)X
2477(the)X
2607(event)X
2820(type)X
2994(constant)X
3310(name)X
3523(that)X
3678(uniquely)X
555 3516(identi\256es)N
901(it.)X
995(For)X
1139(example,)X
1482(when)X
1694(the)X
1824(X)X
1909(server)X
2145(reports)X
2411(a)X
5 f
2490(GraphicsExpose)X
1 f
3173(event)X
3386(to)X
3477(a)X
3538(client)X
3757(appli-)X
555 3612(cation,)N
815(it)X
887(sends)X
1104(an)X
5 f
1227(XGraphicsExposeEvent)X
1 f
2195(structure)X
2525(with)X
2704(the)X
2834(type)X
3008(member)X
3319(set)X
3439(to)X
3530(the)X
3660(constant)X
5 f
570 3708(GraphicsExpose)N
1 f
1231(.)X
1297(Each)X
1495(event)X
1708(structure)X
2038(also)X
2202(has)X
2341(a)X
2402(display)X
2679(member,)X
3012(which)X
3249(Xlib)X
3428(sets)X
3582(to)X
3673(a)X
3734(pointer)X
555 3804(to)N
646(the)X
776(display)X
1053(the)X
1183(event)X
1396(was)X
1554(read)X
1727(on.)X
555 3928(The)N
714(X)X
799(server)X
1035(may)X
1209(send)X
1392(events)X
1639(at)X
1725(any)X
1874(time)X
2054(in)X
2145(the)X
2275(input)X
2479(stream,)X
2758(even)X
2946(between)X
3261(the)X
3391(time)X
3571(your)X
3754(client)X
555 4024(application)N
970(sends)X
1187(a)X
1248(request)X
1524(and)X
1673(receives)X
1983(a)X
2044(reply.)X
2291(Xlib)X
2470(stores)X
2697(in)X
2788(an)X
2893(event)X
3106(queue)X
3338(any)X
3487(events)X
555 4120(received)N
875(while)X
1093(waiting)X
1380(for)X
1504(a)X
1565(reply)X
1768(for)X
1892(later)X
2071(use.)X
2254(Xlib)X
2433(also)X
2597(provides)X
2922(several)X
3193(functions)X
3543(that)X
3698(allow)X
555 4216(you)N
709(to)X
800(check)X
1027(events)X
1274(in)X
1365(the)X
1495(event)X
1708(queue,)X
1962(and)X
2111(these)X
2314(are)X
2443(discussed)X
2802(in)X
2893(Section)X
3180(8.6.)X
555 4340(In)N
650(addition)X
962(to)X
1053(the)X
1183(individual)X
1564(structures)X
1928(declared)X
2248(for)X
2372(each)X
2555(event)X
2768(type,)X
2964(there)X
3162(is)X
3243(also)X
3407(a)X
3468(generic)X
5 f
570 4436(XEvent)N
1 f
892(structure.)X
1244(The)X
5 f
1421(XEvent)X
1 f
1743(structure)X
2073(is)X
2154(a)X
2215(union)X
2438(of)X
2533(the)X
2663(individual)X
3044(structures)X
3408(declared)X
3728(for)X
555 4532(each)N
738(event)X
951(type.)X
1169(Once)X
1376(you)X
1530(determine)X
1905(the)X
2035(event)X
2248(type,)X
2444(use)X
2583(the)X
2713(structures)X
3077(declared)X
3397(in)X
5 f
570 4628(<X11/Xlib.h>)N
1 f
1111(when)X
1323(making)X
1610(references)X
1992(to)X
2083(it)X
2155(in)X
2246(a)X
2307(client)X
2526(application.)X
2985(All)X
3120(events)X
3367(contain)X
3649(a)X
3710(``type'')X
555 4724(member)N
866(that)X
1021(determines)X
1430(the)X
1560(format)X
1817(of)X
1912(the)X
2042(information.)X
2525(Depending)X
2933(on)X
3043(the)X
3173(type,)X
3369(you)X
3523(should)X
3780(access)X
555 4820(elements)N
891(of)X
986(each)X
1169(event)X
1382(by)X
1492(using)X
1705(the)X
5 f
1853(XEvent)X
1 f
2175(union.)X
3 f
555 5012(8.3.)N
731(Event)X
973(Mask)X
1 f
555 5136(Clients)N
828(select)X
1051(event)X
1264(reporting)X
1609(of)X
1704(most)X
1898(events)X
2145(relative)X
2432(to)X
2523(a)X
2584(window.)X
2933(To)X
3053(do)X
3163(this,)X
3335(you)X
3489(pass)X
3662(an)X
3767(event)X
555 5232(mask)N
763(to)X
854(an)X
959(Xlib)X
1138(event-handling)X
1689(function)X
2005(that)X
2160(takes)X
2363(an)X
2468(event_mask)X
2911(argument.)X
3310(The)X
3469(bits)X
3619(of)X
3714(the)X
555 5328(event)N
768(mask)X
976(are)X
1105(de\256ned)X
1386(in)X
5 f
1495(<X11/X.h>)X
1 f
1927(.)X
1993(Each)X
2191(bit)X
2307(in)X
2398(the)X
2528(event)X
2741(mask)X
2949(maps)X
3157(to)X
3248(an)X
3353(event)X
3566(mask)X
3774(name.)X
555 5424(The)N
714(event)X
927(mask)X
1135(name)X
1348(describes)X
1697(the)X
1827(event)X
2040(or)X
2135(events)X
2382(you)X
2536(want)X
2729(the)X
2859(X)X
2944(server)X
3180(to)X
3271(return)X
3503(to)X
3594(a)X
3655(client)X
555 5520(application.)N
1014(When)X
1246(referring)X
1575(to)X
1666(a)X
1727(speci\256c)X
2018(event)X
2231(mask,)X
2461(this)X
2611(manual)X
2893(uses)X
3066(the)X
3196(constant)X
3512(name)X
3725(de\256ned)X
555 5616(in)N
646(this)X
796(\256le.)X
555 5740(Most)N
758(events)X
1005(are)X
1134(not)X
1269(reported)X
1584(to)X
1675(clients)X
1928(when)X
2140(they)X
2314(are)X
2443(generated,)X
2829(unless)X
3071(the)X
3201(client)X
3420(has)X
3559(speci\256cally)X
555 5836(asked)N
777(for)X
901(them.)X
5 f
1165(GraphicsExpose)X
1 f
1848(and)X
5 f
2015(NoExpose)X
1 f
2436(,)X
2480(however,)X
2826(are)X
2955(reported,)X
3292(by)X
3402(default,)X
3691(as)X
3786(a)X
3 f
2217 6144(147)N

148 p
%%Page: 148 152
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(result)N
773(of)X
5 f
886(XCopyPlane)X
1 f
1412(and)X
5 f
1579(XCopyArea)X
1 f
2044(,)X
2088(unless)X
2330(the)X
2460(client)X
2679(suppresses)X
3076(them)X
3275(by)X
3385(setting)X
555 768(graphics_expose)N
1163(in)X
1254(the)X
1384(GC)X
1528(to)X
5 f
1637(False)X
1 f
1867(.)X
1933(See)X
2082(Section)X
2369(6.2)X
2501(for)X
2625(further)X
2886(information.)X
5 f
3390(SelectionClear)X
1 f
3976(,)X
5 f
570 864(SelectionRequest)N
1 f
1275(,)X
5 f
1337(SelectionNotify)X
1 f
1961(or)X
5 f
2074(ClientMessage)X
1 f
2692(cannot)X
2949(be)X
3054(masked,)X
3367(but)X
3502(they)X
3676(generally)X
555 960(are)N
684(only)X
863(sent)X
1027(to)X
1118(clients)X
1371(cooperating)X
1809(with)X
1988(selections.)X
2402(See)X
2551(Section)X
2838(4.4)X
2970(for)X
3094(further)X
3355(information.)X
5 f
570 1056(MappingNotify)N
1 f
1169(is)X
1250(always)X
1516(sent)X
1680(to)X
1771(clients)X
2024(when)X
2236(the)X
2366(keyboard)X
2715(mapping)X
3046(is)X
3127(changed.)X
555 1180(The)N
714(following)X
1079(table)X
1273(lists)X
1438(the)X
1568(event)X
1781(mask)X
1989(constants)X
2339(you)X
2493(can)X
2637(pass)X
2810(to)X
2901(the)X
3031(event_mask)X
3474(argument)X
3829(and)X
555 1276(the)N
685(circumstances)X
1206(in)X
1297(which)X
1534(you)X
1688(would)X
1930(want)X
2123(to)X
2214(specify)X
2490(the)X
2620(event)X
2833(mask.)X
10 f
555 1360(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 1504(Event)N
783(Mask)X
1839(Circumstances)X
10 f
555 1560(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 1704(NoEventMask)N
1839(No)X
1968(events)X
2215(wanted)X
555 1800(KeyPressMask)N
1839(Keyboard)X
2207(down)X
2424(events)X
2671(wanted)X
555 1896(KeyReleaseMask)N
1839(Keyboard)X
2207(up)X
2317(events)X
2564(wanted)X
555 1992(ButtonPressMask)N
1839(Pointer)X
2116(button)X
2364(down)X
2581(events)X
2828(wanted)X
555 2088(ButtonReleaseMask)N
1839(Pointer)X
2116(button)X
2364(up)X
2474(events)X
2721(wanted)X
555 2184(EnterWindowMask)N
1839(Pointer)X
2116(window)X
2421(entry)X
2624(events)X
2871(wanted)X
555 2280(LeaveWindowMask)N
1839(Pointer)X
2116(window)X
2421(leave)X
2629(events)X
2876(wanted)X
555 2376(PointerMotionMask)N
1839(Pointer)X
2116(motion)X
2389(events)X
2636(wanted)X
555 2472(PointerMotionHintMask)N
1839(Pointer)X
2116(motion)X
2389(hints)X
2583(wanted)X
555 2568(Button1MotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(button)X
2855(1)X
2921(down)X
555 2664(Button2MotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(button)X
2855(2)X
2921(down)X
555 2760(Button3MotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(button)X
2855(3)X
2921(down)X
555 2856(Button4MotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(button)X
2855(4)X
2921(down)X
555 2952(Button5MotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(button)X
2855(5)X
2921(down)X
555 3048(ButtonMotionMask)N
1839(Pointer)X
2116(motion)X
2389(while)X
2607(any)X
2756(button)X
3004(down)X
555 3144(KeymapStateMask)N
1839(Any)X
2012(keyboard)X
2361(state)X
2545(change)X
2816(wanted)X
555 3240(ExposureMask)N
1839(Any)X
2012(exposure)X
2351(wanted)X
555 3336(VisibilityChangeMask)N
1839(Any)X
2012(change)X
2283(in)X
2374 0.3472(visibility)AX
2712(wanted)X
555 3432(StructureNotifyMask)N
1839(Any)X
2012(change)X
2283(in)X
2374(window)X
2679(structure)X
3009(wanted)X
555 3528(ResizeRedirectMask)N
1839(Redirect)X
2160(resize)X
2387(of)X
2482(this)X
2632(window)X
555 3624(SubstructureNotifyMask)N
1839(Substructure)X
2306(noti\256cation)X
2731(wanted)X
555 3720(SubstructureRedirectMask)N
1839(Redirect)X
2160(substructure)X
2612(of)X
2707(window)X
555 3816(FocusChangeMask)N
1839(Any)X
2012(change)X
2283(in)X
2374(input)X
2578(focus)X
2790(wanted)X
555 3912(PropertyChangeMask)N
1839(Any)X
2012(change)X
2283(in)X
2374(property)X
2694(wanted)X
555 4008(ColormapChangeMask)N
1839(Any)X
2012(change)X
2283(in)X
2374(Colormap)X
2749(wanted)X
555 4104(OwnerGrabButtonMask)N
1839(Automatic)X
2234(grabs)X
2446(should)X
2703(activate)X
1839 4200(when)N
2051(owner_events)X
2561(is)X
2642(True)X
10 f
555 4256(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
3 f
555 4524(8.4.)N
731(Event)X
973(Processing)X
1 f
555 4648(The)N
714(event)X
927(types)X
1135(reported)X
1450(to)X
1541(a)X
1602(client)X
1821(application)X
2236(during)X
2488(event)X
2701(processing)X
3099(depend)X
3375(on)X
3485(which)X
3722(event)X
555 4744(masks)N
797(you)X
951(pass)X
1124(to)X
1215(the)X
1345(event_mask)X
1788(argument)X
2143(of)X
2238(the)X
5 f
2386(XSelectInput)X
1 f
2924(function)X
3240(\(Section)X
3556(8.5\).)X
3761(For)X
555 4840(some)N
763(event)X
976(masks,)X
1240(there)X
1438(is)X
1519(a)X
1580(one-to-one)X
1983(correspondence)X
2556(between)X
2871(the)X
3001(event)X
3214(mask)X
3422(constant)X
3738(and)X
3887(the)X
555 4936(event)N
768(type)X
942(constant.)X
1302(For)X
1446(example,)X
1789(if)X
1865(you)X
2019(pass)X
2192(the)X
2322(event)X
2535(mask)X
5 f
2761(ButtonPressMask)X
1 f
3467(,)X
3511(the)X
3641(X)X
3726(server)X
555 5032(sends)N
772(back)X
960(only)X
5 f
1157(ButtonPress)X
1 f
1675(events.)X
1966(Most)X
2169(events)X
2416(contain)X
2698(a)X
2759(time)X
2939(member)X
3250(that)X
3405(is)X
3486(the)X
3616(time)X
3796(at)X
555 5128(which)N
792(an)X
897(event)X
1110(occurred.)X
555 5252(In)N
650(other)X
853(cases,)X
1082(one)X
1231(event)X
1444(mask)X
1652(constant)X
1968(can)X
2112(map)X
2286(to)X
2377(several)X
2648(event)X
2861(type)X
3035(constants.)X
3429(For)X
3573(example,)X
3916(if)X
555 5348(you)N
709(pass)X
882(the)X
1012(event)X
1225(mask)X
5 f
1451(SubstructureNotifyMask)X
1 f
2401(,)X
2445(the)X
2575(X)X
2660(server)X
2896(can)X
3040(send)X
3223(back)X
5 f
3429(CirculateNo-)X
555 5444(tify)N
1 f
683(,)X
5 f
745(Con\256gureNotify)X
1 f
1366(,)X
5 f
1428(CreateNotify)X
1 f
1932(,)X
5 f
1994(DestroyNotify)X
1 f
2537(,)X
5 f
2599(GravityNotify)X
1 f
3118(,)X
5 f
3180(MapNotify)X
1 f
3591(,)X
5 f
570 5540(ReparentNotify)N
1 f
1172(,)X
1216(or)X
5 f
1329(UnmapNotify)X
1 f
1874(events.)X
555 5664(In)N
650(another)X
936(case,)X
1131(two)X
1285(event)X
1498(mask)X
1706(constants)X
2056(map)X
2230(to)X
2321(one)X
2470(event)X
2683(type)X
2857(constant.)X
3217(For)X
3361(example,)X
3704(if)X
3780(you)X
555 5760(pass)N
728(the)X
858(event)X
1071(mask)X
5 f
1297(PointerMotionMask)X
1 f
2087(or)X
5 f
2200(PointerMotionHintMask)X
1 f
3146(the)X
3276(X)X
3361(server)X
3597(sends)X
3814(back)X
3 f
2217 6144(148)N

149 p
%%Page: 149 153
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(a)N
5 f
634(MotionNotify)X
1 f
1160(event.)X
555 796(The)N
714(following)X
1079(table)X
1273(lists)X
1438(the)X
1568(event)X
1781(mask,)X
2011(its)X
2117(associated)X
2501(event)X
2714(type)X
2888(or)X
2983(types,)X
3213(and)X
3362(the)X
3492(structure)X
555 892(name)N
768(associated)X
1152(with)X
1331(the)X
1461(event)X
1674(type.)X
1892(Note)X
2085(that)X
2240(the)X
2370(letters)X
2608(N.A.)X
2800(appear)X
3056(in)X
3147(columns)X
3468(for)X
3592(which)X
3829(the)X
555 988(information)N
994(is)X
1075(not)X
1210(applicable.)X
10 f
555 1072(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 1216(Event)N
783(Mask)X
1695(Event)X
1923(Type)X
2835(Structure)X
10 f
555 1272(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 1416(ButtonMotionMask)N
1695(MotionNotify)X
2835(XPointerMovedEvent)X
555 1512(Button1MotionMask)N
555 1608(Button2MotionMask)N
555 1704(Button3MotionMask)N
555 1800(Button4MotionMask)N
555 1896(Button5MotionMask)N
555 2040(ButtonPressMask)N
1695(ButtonPress)X
2835(XButtonPressedEvent)X
555 2184(ButtonReleaseMask)N
1695(ButtonRelease)X
2835(XButtonReleasedEvent)X
555 2328(ColormapChangeMask)N
1695(ColormapNotify)X
2835(XColormapEvent)X
555 2472(EnterWindowMask)N
1695(EnterNotify)X
2835(XEnterWindowEvent)X
555 2616(ExposureMask)N
1695(Expose)X
2835(XExposeEvent)X
1695 2712(GraphicsExpose)N
2835(XGraphicsExposeEvent)X
1695 2808(NoExpose)N
2835(XNoExposeEvent)X
555 2952(LeaveWindowMask)N
1695(LeaveNotify)X
2835(XLeaveWindowEvent)X
555 3096(FocusChangeMask)N
1695(FocusIn)X
2835(XFocusInEvent)X
1695 3192(FocusOut)N
2835(XFocusOutEvent)X
555 3336(KeymapStateMask)N
1695(KeymapNotify)X
2835(XKeymapEvent)X
555 3480(KeyPressMask)N
1695(KeyPress)X
2835(XKeyPressedEvent)X
1695 3576(KeyRelease)N
2835(XKeyReleasedEvent)X
555 3720(OwnerGrabButtonMask)N
1695(N.A.)X
2835(N.A.)X
555 3864(PointerMotionMask)N
1695(MotionNotify)X
2835(XPointerMovedEvent)X
555 3960(PointerMotionHintMask)N
555 4104(PropertyChangeMask)N
1695(PropertyNotify)X
2835(XPropertyEvent)X
555 4248(ResizeRedirectMask)N
1695(ResizeRequest)X
2835(XResizeRequestEvent)X
555 4392(StructureNotifyMask)N
1695(CirculateNotify)X
2835(XCirculateEvent)X
1695 4488(Con\256gureNotify)N
2835(XCon\256gureEvent)X
1695 4584(DestroyNotify)N
2835(XDestroyWindowEvent)X
1695 4680(GravityNotify)N
2835(XGravityEvent)X
1695 4776(MapNotify)N
2835(XMapEvent)X
1695 4872(ReparentNotify)N
2835(XReparentEvent)X
1695 4968(UnmapNotify)N
2835(XUnmapEvent)X
555 5112(SubstructureNotifyMask)N
1695(CirculateNotify)X
2835(XCirculateEvent)X
1695 5208(Con\256gureNotify)N
2835(XCon\256gureEvent)X
1695 5304(CreateNotify)N
2835(XCreateWindowEvent)X
1695 5400(DestroyNotify)N
2835(XDestroyWindowEvent)X
1695 5496(GravityNotify)N
2835(XGravityEvent)X
1695 5592(MapNotify)N
2835(XMapEvent)X
1695 5688(ReparentNotify)N
2835(XReparentEvent)X
1695 5784(UnmapNotify)N
2835(XUnmapEvent)X
3 f
2217 6144(149)N

150 p
%%Page: 150 154
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
10 f
555 584(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 728(Event)N
783(Mask)X
1695(Event)X
1923(Type)X
2835(Structure)X
10 f
555 784(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 928(SubstructureRedirectMask)N
1695(CirculateRequest)X
2835(XCirculateRequestEvent)X
1695 1024(Con\256gureRequest)N
2835(XCon\256gureRequestEvent)X
1695 1120(MapRequest)N
2835(XMapRequestEvent)X
555 1264(N.A.)N
1695(ClientMessage)X
2835(XClientMessageEvent)X
555 1408(N.A.)N
1695(MappingNotify)X
2835(XMappingEvent)X
555 1552(N.A.)N
1695(SelectionClear)X
2835(XSelectionClearEvent)X
555 1696(N.A.)N
1695(SelectionNotify)X
2835(XSelectionEvent)X
555 1840(N.A.)N
1695(SelectionRequest)X
2835(XSelectionRequestEvent)X
555 1984(VisibilityChangeMask)N
1695(VisibilityNotify)X
2835(XVisibilityEvent)X
10 f
555 2040(iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii)N
1 f
555 2212(The)N
714(sections)X
1020(below)X
1257(describe)X
1572(the)X
1702(processing)X
2100(that)X
2255(occurs)X
2506(when)X
2718(you)X
2872(pass)X
3045(the)X
3175(different)X
3500(event)X
3713(masks)X
555 2308(to)N
5 f
664(XSelectInput)X
1 f
1202(\(Section)X
1518(8.5\).)X
1701(The)X
1860(sections)X
2166(are)X
2295(organized)X
2664(according)X
3033(to)X
3124(these)X
3327(processing)X
555 2404(categories:)N
10 f
555 2528(g)N
1 f
775(Keyboard)X
1143(and)X
1292(pointer)X
1564(event)X
1777(processing)X
10 f
555 2652(g)N
1 f
775(Window)X
1100(crossing)X
1415(event)X
1628(processing)X
10 f
555 2776(g)N
1 f
775(Input)X
983(focus)X
1195(event)X
1408(processing)X
10 f
555 2900(g)N
1 f
775(Key)X
943(map)X
1117(state)X
1301(noti\256cation)X
1726(event)X
1939(processing)X
10 f
555 3024(g)N
1 f
775(Exposure)X
1129(event)X
1342(processing)X
10 f
555 3148(g)N
1 f
775(Window)X
1100(state)X
1284(noti\256cation)X
1709(event)X
1922(processing)X
10 f
555 3272(g)N
1 f
775(Structure)X
1120(control)X
1392(event)X
1605(processing)X
10 f
555 3396(g)N
1 f
775(Color)X
998(map)X
1172(state)X
1356(noti\256cation)X
1781(event)X
1994(processing)X
10 f
555 3520(g)N
1 f
775(Client)X
1014(communication)X
1586(event)X
1799(processing)X
555 3644(The)N
714(processing)X
1112(descriptions)X
1560(include)X
1842(explanations)X
2310(of)X
2405(the)X
2535(structure)X
2865(or)X
2960(structures)X
3324(associated)X
3708(with)X
3887(the)X
555 3740(event.)N
812(All)X
947(the)X
1077(event)X
1290(structures)X
1654(contain)X
1936(the)X
2066(members)X
2411(type)X
2585(and)X
2734(display,)X
3033(which)X
3270(were)X
3462(discussed)X
3821(in)X
555 3836(Section)N
842(8.2.)X
1018(Thus,)X
1238(the)X
1368(explanations)X
1836(for)X
1960(these)X
2163(members)X
2508(are)X
2637(not)X
2772(repeated)X
3092(in)X
3183(the)X
3313(following)X
3678(sections.)X
3 f
555 4028(8.4.1.)N
797(Keyboard)X
1196(and)X
1360(Pointer)X
1661(Event)X
1903(Processing)X
1 f
555 4152(This)N
734(section)X
1006(discusses)X
1355(the)X
1485(event)X
1698(processing)X
2096(that)X
2251(occurs)X
2502(when)X
2714(a)X
2775(pointer)X
3047(button)X
3295(is)X
3376(pressed)X
3661(and)X
3810(when)X
555 4248(the)N
685(keyboard)X
1034(events)X
5 f
1299(KeyPress)X
1 f
1713(and)X
5 f
1880(KeyRelease)X
1 f
2391(and)X
2540(the)X
2670(pointer)X
2942(motion)X
3215(events)X
5 f
3480(ButtonPress)X
1 f
3976(,)X
5 f
570 4344(ButtonRelease)N
1 f
1163(,)X
1207(and)X
5 f
1374(MotionNotify)X
1 f
1900(are)X
2029(generated.)X
3 f
555 4536(8.4.1.1.)N
863(Pointer)X
1164(Button)X
1445(Speci\256c)X
1756(Processing)X
1 f
555 4660(The)N
714(following)X
1079(describes)X
1428(the)X
1558(event)X
1771(processing)X
2169(that)X
2324(occurs)X
2575(when)X
2787(a)X
2848(pointer)X
3120(button)X
3368(is)X
3449(pressed)X
3734(with)X
555 4756(the)N
685(pointer)X
957(in)X
1048(some)X
1256(window)X
1561(w)X
1646(and)X
1795(when)X
2007(no)X
2117(active)X
2350(pointer)X
2622(grab)X
2800(is)X
2881(in)X
2972(progress.)X
555 4880(The)N
714(X)X
799(server)X
1035(searches)X
1354(the)X
1484(ancestors)X
1833(of)X
1928(w)X
2013(from)X
2206(the)X
2336(root)X
2500(down,)X
2739(looking)X
3031(for)X
3155(a)X
3216(passive)X
3497(grab)X
3675(to)X
555 4976(activate.)N
896(If)X
976(no)X
1086(matching)X
1437(passive)X
1718(grab)X
1896(on)X
2006(the)X
2136(button)X
2384(exists,)X
2629(the)X
2759(X)X
2844(server)X
3080 0.2604(automatically)AX
3584(starts)X
3792(an)X
555 5072(active)N
788(grab)X
966(for)X
1090(the)X
1220(client)X
1439(receiving)X
1789(the)X
1919(event)X
2132(and)X
2281(sets)X
2435(the)X
2565(last-pointer-grab)X
3174(time)X
3354(to)X
3445(the)X
3575(current)X
555 5168(server)N
791(time.)X
1015(The)X
1174(effect)X
1396(is)X
1477(essentially)X
1872(equivalent)X
2262(to)X
2353(an)X
5 f
2476(XGrabButton)X
1 f
3024(with)X
3203(these)X
3406(client)X
3625(passed)X
555 5264(arguments:)N
2 f
555 5388(w)N
1 f
1131(The)X
1290(event)X
1503(window)X
2 f
555 5512(event_mask)N
1 f
1131(The)X
1290(client's)X
1572(selected)X
1878(pointer)X
2150(motion)X
2423(events)X
2670(on)X
2780(the)X
2910(event)X
3123(window.)X
2 f
555 5636(pointer_mode)N
1 f
5 f
1146(GrabModeAsync)X
1 f
1817(.)X
3 f
2217 6144(150)N

151 p
%%Page: 151 155
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
2 f
555 672(keyboard_mode)N
1 f
5 f
1146(GrabModeAsync)X
1 f
1817(.)X
2 f
555 796(owner_events)N
1 f
5 f
1146(True)X
1 f
1342(,)X
1386(if)X
1462(the)X
1592(client)X
1811(has)X
1950(selected)X
5 f
2274(OwnerGrabButtonMask)X
1 f
3232(on)X
3342(the)X
3472(event)X
3685(window;)X
1131 892(otherwise,)N
5 f
1535(False)X
1 f
1765(.)X
2 f
555 1016(con\256ne_to)N
1 f
5 f
1146(None)X
1 f
1371(.)X
2 f
555 1140(cursor)N
1 f
5 f
1146(None)X
1 f
1371(.)X
555 1264(The)N
714(grab)X
892(is)X
973 0.2604(automatically)AX
1477(terminated)X
1877(when)X
2089(all)X
2200(buttons)X
2482(are)X
2611(released.)X
2965(Clients)X
3238(can)X
3382(modify)X
3659(the)X
3789(active)X
555 1360(grab)N
733(by)X
843(calling)X
5 f
1124(XUngrabPointer)X
1 f
1787(and)X
5 f
1954(XChangeActivePointerGrab)X
1 f
3051(.)X
3 f
555 1552(8.4.1.2.)N
863(Common)X
1231(Keyboard)X
1630(and)X
1794(Pointer)X
2095(Event)X
2337(Processing)X
1 f
555 1676(This)N
734(section)X
1006(discusses)X
1355(the)X
1485(processing)X
1883(that)X
2038(occurs)X
2289(for)X
2413(the)X
2543(keyboard)X
2892(events)X
5 f
3157(KeyPress)X
1 f
3571(and)X
5 f
570 1772(KeyRelease)N
1 f
1081(and)X
1230(the)X
1360(pointer)X
1632(motion)X
1905(events)X
5 f
2170(ButtonPress)X
1 f
2666(,)X
5 f
2728(ButtonRelease)X
1 f
3321(,)X
3365(and)X
5 f
3532(MotionNo-)X
555 1868(tify)N
1 f
683(.)X
749(See)X
898(Chapter)X
1199(10)X
1309(for)X
1433(information)X
1872(about)X
2090(the)X
2220(keyboard)X
2569(event)X
2782(handling)X
3113(utility)X
3348(functions)X
3698(provided)X
555 1964(in)N
646(XLib.)X
555 2088(The)N
714(X)X
799(server)X
1035(can)X
1179(report)X
5 f
1429(KeyPress)X
1 f
1843(events)X
2090(to)X
2181(clients)X
2434(wanting)X
2740(information)X
3179(about)X
3397(when)X
3609(a)X
3670(key)X
3819(is)X
555 2184(pressed)N
840(and)X
5 f
1007(KeyRelease)X
1 f
1518(events)X
1765(to)X
1856(clients)X
2109(wanting)X
2415(information)X
2854(about)X
3072(when)X
3284(a)X
3345(key)X
3494(is)X
3575(released.)X
555 2280(The)N
714(X)X
799(server)X
1035(generates)X
1389(these)X
1592(events)X
1839(whenever)X
2202(a)X
2263(key)X
2412(changes)X
2717(state,)X
2923(that)X
3078(is,)X
3181(whenever)X
3544(the)X
3674(key)X
3823(is)X
555 2376(pressed)N
840(or)X
935(released.)X
1289(Note)X
1482(that)X
1637(these)X
1840(events)X
2087(are)X
2216(generated)X
2580(for)X
2704(all)X
2815(keys,)X
3020(even)X
3208(those)X
3416(mapped)X
3717(to)X
555 2472(modi\256er)N
876(bits.)X
1070(The)X
1229(X)X
1314(server)X
1550(reports)X
5 f
1834(ButtonPress)X
1 f
2352(events)X
2599(to)X
2690(clients)X
2943(wanting)X
3249(information)X
3688(about)X
555 2568(when)N
767(a)X
828(pointer)X
1100(button)X
1348(is)X
1429(pressed)X
1714(and)X
5 f
1881(ButtonRelease)X
1 f
2496(events)X
2743(to)X
2834(clients)X
3087(wanting)X
3393(information)X
555 2664(about)N
773(when)X
985(a)X
1046(pointer)X
1318(button)X
1566(is)X
1647(released.)X
2001(The)X
2160(X)X
2245(server)X
2481(generates)X
2835(these)X
3038(events)X
3285(whenever)X
3648(a)X
3709(pointer)X
555 2760(button)N
803(changes)X
1108(state,)X
1314(that)X
1469(is,)X
1572(whenever)X
1935(the)X
2065(pointer)X
2337(button)X
2585(is)X
2666(pressed)X
2951(or)X
3046(released.)X
555 2884(The)N
714(X)X
799(server)X
1035(reports)X
5 f
1319(MotionNotify)X
1 f
1845(events)X
2092(to)X
2183(clients)X
2436(wanting)X
2742(information)X
3181(about)X
3399(when)X
3611(the)X
3741(pointer)X
555 2980(moves.)N
851(The)X
1010(X)X
1095(server)X
1331(generates)X
1685(this)X
1835(event)X
2048(whenever)X
2411(the)X
2541(pointer)X
2813(changes)X
3118(state,)X
3324(that)X
3479(is,)X
3582(whenever)X
555 3076(the)N
685(pointer)X
957(is)X
1038(moved)X
1300(and)X
1449(the)X
1579(pointer)X
1851(motion)X
2124(begins)X
2376(and)X
2525(ends)X
2708(in)X
2799(the)X
2929(window.)X
3278(The)X
3437(granularity)X
3846(of)X
5 f
570 3172(MotionNotify)N
1 f
1096(events)X
1343(is)X
1424(not)X
1559(guaranteed,)X
1989(but)X
2124(a)X
2185(client)X
2404(that)X
2559(selects)X
2816(this)X
2966(event)X
3179(type)X
3353(is)X
3434(guaranteed)X
3842(to)X
555 3268(receive)N
831(at)X
917(least)X
1101(one)X
1250(event)X
1463(when)X
1675(the)X
1805(pointer)X
2077(moves)X
2329(and)X
2478(then)X
2652(rests.)X
555 3392(To)N
675(receive)X
5 f
969(KeyPress)X
1 f
1361(,)X
5 f
1423(KeyRelease)X
1 f
1912(,)X
5 f
1974(ButtonPress)X
1 f
2470(,)X
2514(and)X
5 f
2681(ButtonRelease)X
1 f
3296(events)X
3543(in)X
3634(a)X
3695(client)X
555 3488(application,)N
992(you)X
1146(pass)X
1319(a)X
1380(window)X
1685(ID)X
1799(and)X
5 f
1966(KeyPressMask)X
1 f
2568(,)X
5 f
2630(KeyReleaseMask)X
1 f
3329(,)X
5 f
3391(But-)X
555 3584(tonPressMask)N
1 f
1128(,)X
1172(and)X
5 f
1339(ButtonReleaseMask)X
1 f
2164(as)X
2259(the)X
2389(event_mask)X
2832(arguments)X
3221(to)X
5 f
3330(XSelectInput)X
1 f
3846(.)X
555 3708(To)N
675(receive)X
5 f
969(MotionNotify)X
1 f
1495(events)X
1742(in)X
1833(a)X
1894(client)X
2113(application,)X
2550(you)X
2704(pass)X
2877(a)X
2938(window)X
3243(ID)X
3357(and)X
3506(one)X
3655(or)X
3750(more)X
555 3804(of)N
650(the)X
780(following)X
1145(event)X
1358(masks)X
1600(as)X
1695(the)X
1825(event_mask)X
2268(argument)X
2623(to)X
5 f
2732(XSelectInput)X
1 f
3248(:)X
10 f
555 3928(g)N
1 f
775(Button1MotionMask)X
9 f
1515(-)X
1 f
1563(Button5MotionMask)X
775 4052(The)N
934(client)X
1153(application)X
1568(receives)X
5 f
1896(MotionNotify)X
1 f
2422(events)X
2669(only)X
2848(when)X
3060(one)X
3209(or)X
3304(more)X
3507(of)X
3602(the)X
775 4148(speci\256ed)N
1110(buttons)X
1392(is)X
1473(pressed.)X
10 f
555 4272(g)N
1 f
775(ButtonMotionMask)X
775 4396(The)N
934(client)X
1153(application)X
1568(receives)X
5 f
1896(MotionNotify)X
1 f
2422(events)X
2669(only)X
2848(when)X
3060(at)X
3146(least)X
3330(one)X
3479(button)X
3727(is)X
775 4492(pressed.)N
10 f
555 4616(g)N
1 f
775(PointerMotionMask)X
775 4740(The)N
934(client)X
1153(application)X
1568(receives)X
5 f
1896(MotionNotify)X
1 f
2422(events)X
2669(independent)X
3122(of)X
3217(the)X
3347(state)X
3531(of)X
3626(the)X
3756(pointer)X
775 4836(buttons.)N
10 f
555 4960(g)N
1 f
775(PointerMotionHint)X
775 5084(If)N
5 f
873(PointerMotionHintMask)X
1 f
1819(is)X
1900(selected,)X
2228(the)X
2358(X)X
2443(server)X
2679(is)X
2760(free)X
2918(to)X
3009(send)X
3192(only)X
3371(one)X
5 f
3538(MotionNo-)X
775 5180(tify)N
1 f
925(event)X
1138(\(with)X
1346(the)X
1476(is_hint)X
1739(member)X
2072(of)X
2167(the)X
5 f
2315(XPointerMovedEvent)X
1 f
3180(structure)X
3510(set)X
3630(to)X
5 f
790 5276(NotifyHint)N
1 f
1186(\))X
1237(to)X
1328(the)X
1458(client)X
1677(for)X
1801(the)X
1931(event)X
2144(window,)X
2471(until)X
2656(either)X
2879(the)X
3009(key)X
3158(or)X
3253(button)X
3501(state)X
3685(changes,)X
775 5372(or)N
870(the)X
1000(pointer)X
1272(leaves)X
1514(the)X
1644(event)X
1857(window,)X
2184(or)X
2279(the)X
2409(client)X
2628(calls)X
2812(the)X
5 f
2960(XQueryPointer)X
1 f
3575(or)X
5 f
3688(XGet-)X
775 5468(MotionEvents)N
1 f
1346(functions.)X
555 5592(The)N
714(source)X
965(of)X
1060(the)X
1190(event)X
1403(is)X
1484(the)X
1614(smallest)X
1926(window)X
2231(containing)X
2626(the)X
2756(pointer.)X
3072(The)X
3231(window)X
3536(used)X
3719(by)X
3829(the)X
555 5688(X)N
640(server)X
876(to)X
967(report)X
1199(these)X
1402(events)X
1649(depends)X
1959(on)X
2069(its)X
2175(position)X
2482(in)X
2573(the)X
2703(window)X
3008(hierarchy)X
3362(and)X
3511(whether)X
3816(any)X
555 5784(intervening)N
979(window)X
1284(prohibits)X
1620(the)X
1750(generation)X
2144(of)X
2239(these)X
2442(events.)X
2733(The)X
2892(X)X
2977(server)X
3213(searches)X
3532(up)X
3642(the)X
3 f
2217 6144(151)N

152 p
%%Page: 152 156
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(window)N
860(hierarchy,)X
1236(starting)X
1523(with)X
1702(the)X
1832(source)X
2083(window,)X
2410(until)X
2595(it)X
2667(locates)X
2934(the)X
3064(\256rst)X
3223(window)X
3528(speci\256ed)X
3863(by)X
555 768(a)N
616(client)X
835(as)X
930(having)X
1192(an)X
1297(interest)X
1579(in)X
1670(these)X
1873(events.)X
2164(If)X
2244(one)X
2393(of)X
2488(the)X
2618(intervening)X
3042(windows)X
3381(has)X
3520(its)X
555 864(do_not_propagate_mask)N
1443(set)X
1563(to)X
1654(prohibit)X
1956(generation)X
2350(of)X
2445(the)X
2575(event)X
2788(type,)X
2984(the)X
3114(event)X
3327(of)X
3422(those)X
3630(types)X
3838(will)X
555 960(be)N
660(suppressed.)X
1111(Clients)X
1384(can)X
1528(modify)X
1805(the)X
1935(actual)X
2168(window)X
2473(used)X
2656(for)X
2780(reporting)X
3125(by)X
3235(performing)X
3653(active)X
555 1056(grabs,)N
789(and,)X
960(in)X
1051(the)X
1181(case)X
1354(of)X
1449(keyboard)X
1798(events,)X
2067(by)X
2177(using)X
2390(the)X
2520(focus)X
2732(window.)X
3081(See)X
3230(Chapter)X
3531(7)X
3597(for)X
3721(a)X
3782(dis-)X
555 1152(cussion)N
841(of)X
936(the)X
5 f
1084(XGrabPointer)X
1 f
1633(,)X
5 f
1695(XGrabKeyboard)X
1 f
2342(,)X
2386(and)X
5 f
2553(XSetInputFocus)X
1 f
3219(functions.)X
555 1276(The)N
714(structures)X
1078(associated)X
1462(with)X
1641(these)X
1844(events)X
2091(are)X
5 f
2238(XKeyPressedEvent)X
1 f
3013(,)X
5 f
3075(XKeyReleasedEvent)X
1 f
3898(,)X
5 f
570 1372 0.1736(XButtonPressedEvent)AN
1 f
1449(,)X
5 f
1511(XButtonReleasedEvent)X
1 f
2438(,)X
2482(and)X
5 f
2649(XPointerMovedEvent)X
1 f
3492(.)X
3558(The)X
3717(window)X
555 1468(the)N
685(event)X
898(is)X
979(reported)X
1294(with)X
1473(respect)X
1744(to)X
1835(is)X
1916(called)X
2149(the)X
2279(event)X
2492(window.)X
2841(These)X
3073(structures)X
3437(have)X
3625(the)X
3755(fol-)X
555 1564(lowing)N
822(common)X
1153(members:)X
1523(window,)X
1850(root,)X
2036(subwindow,)X
2485(time,)X
2687(x,)X
2775(y,)X
2863(x_root,)X
3137(y_root,)X
3411(state,)X
3617(and)X
555 1660(same_screen.)N
555 1784(The)N
714(window)X
1019(member)X
1330(is)X
1411(set)X
1531(to)X
1622(the)X
1752(window)X
2057(ID)X
2171(of)X
2266(the)X
2396(window)X
2701(on)X
2811(which)X
3048(the)X
3178(event)X
3391(was)X
3549(generated)X
555 1880(and)N
704(is)X
785(referred)X
1084(to)X
1175(as)X
1270(the)X
1400(event)X
1613(window.)X
1940(This)X
2119(is)X
2200(the)X
2330(window)X
2635(used)X
2818(by)X
2928(the)X
3058(X)X
3143(server)X
3379(to)X
3470(report)X
3702(the)X
555 1976(event,)N
790(as)X
885(long)X
1064(as)X
1159(the)X
1289(conditions)X
1679(discussed)X
2038(in)X
2129(the)X
2259(previous)X
2584(paragraph)X
2957(are)X
3086(met.)X
3285(The)X
3444(root)X
3608(member)X
3919(is)X
555 2072(set)N
675(to)X
766(the)X
896(window)X
1201(ID)X
1315(of)X
1410(the)X
1540(source)X
1791(window's)X
2159(root)X
2323(window.)X
2672(The)X
2831(x_root)X
3083(and)X
3232(y_root)X
3484(members)X
3829(are)X
555 2168(set)N
675(to)X
766(the)X
896(pointer's)X
1231(coordinates)X
1659(relative)X
1946(to)X
2037(the)X
2167(root)X
2331(window's)X
2699(origin)X
2932(at)X
3018(the)X
3148(time)X
3328(of)X
3423(the)X
3553(event.)X
555 2292(The)N
714(same_screen)X
1185(member)X
1496(is)X
1577(set)X
1697(to)X
1788(indicate)X
2090(whether)X
2395(the)X
2525(event)X
2738(window)X
3043(is)X
3124(on)X
3234(the)X
3364(same)X
3567(screen)X
3813(as)X
555 2388(the)N
685(root)X
849(window)X
1154(and)X
1303(can)X
1447(be)X
1552(one)X
1701(of)X
1796(the)X
1926(constants)X
5 f
2294(True)X
1 f
2512(or)X
5 f
2625(False)X
1 f
2855(.)X
2921(If)X
5 f
3019(True)X
1 f
3215(,)X
3259(the)X
3389(event)X
3602(and)X
3751(root)X
555 2484(windows)N
894(are)X
1023(on)X
1133(the)X
1263(same)X
1466(screen.)X
1756(If)X
5 f
1854(False)X
1 f
2084(,)X
2128(the)X
2258(event)X
2471(and)X
2620(root)X
2784(windows)X
3123(are)X
3252(not)X
3387(on)X
3497(the)X
3627(same)X
555 2580(screen.)N
555 2704(If)N
635(the)X
765(source)X
1016(window)X
1321(is)X
1402(an)X
1507(inferior)X
1793(of)X
1888(the)X
2018(event)X
2231(window,)X
2558(the)X
2688(subwindow)X
3115(member)X
3426(of)X
3521(the)X
3651(structure)X
555 2800(is)N
636(set)X
756(to)X
847(the)X
977(child)X
1176(of)X
1271(the)X
1401(event)X
1614(window)X
1919(that)X
2074(is)X
2155(an)X
2260(ancestor)X
2575(of)X
2670(or)X
2765(is)X
2846(the)X
2976(source)X
3227(member.)X
3582(Otherwise,)X
555 2896(the)N
685(X)X
770(server)X
1006(sets)X
1160(the)X
1290(subwindow)X
1717(member)X
2028(to)X
2119(the)X
2249(constant)X
5 f
2583(None)X
1 f
2808(.)X
2874(The)X
3033(time)X
3213(member)X
3524(is)X
3605(set)X
3725(to)X
3816(the)X
555 2992(time)N
735(when)X
947(the)X
1077(event)X
1290(was)X
1448(generated)X
1812(and)X
1961(is)X
2042(expressed)X
2410(in)X
2501(milliseconds)X
2970(since)X
3173(the)X
3303(server)X
3539(reset.)X
555 3116(If)N
635(the)X
765(event)X
978(window)X
1283(is)X
1364(on)X
1474(the)X
1604(same)X
1807(screen)X
2053(as)X
2148(the)X
2278(root)X
2442(window,)X
2769(the)X
2899(x)X
2965(and)X
3114(y)X
3180(members)X
3525(are)X
3654(set)X
3774(to)X
3865(the)X
555 3212(coordinates)N
983(relative)X
1270(to)X
1361(the)X
1491(event)X
1704(window's)X
2072(origin.)X
2349(Otherwise,)X
2754(these)X
2957(members)X
3302(are)X
3431(set)X
3551(to)X
3642(zero.)X
555 3336(The)N
714(state)X
898(member)X
1209(is)X
1290(set)X
1410(to)X
1501(indicate)X
1803(the)X
1933(state)X
2117(of)X
2212(the)X
2342(pointer)X
2614(buttons)X
2896(and)X
3045(modi\256er)X
3366(keys)X
3549(just)X
3699(prior)X
3892(to)X
555 3432(the)N
685(event.)X
942(For)X
1086(the)X
1216(state)X
1400(of)X
1495(the)X
1625(pointer)X
1897(buttons,)X
2201(the)X
2331(X)X
2416(server)X
2652(can)X
2796(set)X
2916(this)X
3066(member)X
3377(to)X
3468(the)X
3598(bitwise)X
555 3528(inclusive)N
896(OR)X
1040(of)X
1135(one)X
1284(or)X
1379(more)X
1582(of)X
1677(the)X
1807(button)X
2055(or)X
2150(modi\256er)X
2471(key)X
2620(masks:)X
5 f
2905(Button1Mask)X
1 f
3435(,)X
5 f
570 3624(Button2Mask)N
1 f
1100(,)X
5 f
1162(Button3Mask)X
1 f
1692(,)X
5 f
1754(Button4Mask)X
1 f
2284(,)X
5 f
2346(Button5Mask)X
1 f
2876(,)X
5 f
2938(ShiftMask)X
1 f
3340(,)X
5 f
3402(LockMask)X
1 f
3813(,)X
5 f
570 3720(ControlMask)N
1 f
1078(,)X
5 f
1140(Mod1Mask)X
1 f
1585(,)X
5 f
1647(Mod2Mask)X
1 f
2092(,)X
5 f
2154(Mod3Mask)X
1 f
2599(,)X
5 f
2661(Mod4Mask)X
1 f
3106(,)X
5 f
3168(Mod5Mask)X
1 f
3613(.)X
555 3844(Each)N
753(of)X
848(these)X
1051(structures)X
1415(also)X
1579(has)X
1718(a)X
1779(member)X
2090(that)X
2245(indicates)X
2581(the)X
2711(detail.)X
2974(For)X
3118(the)X
5 f
570 3940(XKeyPressedEvent)N
1 f
1367(and)X
5 f
1534(XKeyReleasedEvent)X
1 f
2379(structures,)X
2765(this)X
2915(member)X
3226(is)X
3307(called)X
3540(keycode.)X
3899(It)X
555 4036(is)N
636(set)X
756(to)X
847(a)X
908(number)X
1199(that)X
1354(represents)X
1732(a)X
1793(physical)X
2109(key)X
2258(on)X
2368(the)X
2498(keyboard.)X
2891(The)X
3050(keycode)X
3365(is)X
3446(an)X
3551(arbitrary)X
555 4132(representation)N
1076(for)X
1200(any)X
1349(key)X
1498(on)X
1608(the)X
1738(keyboard.)X
2131(See)X
2280(Chapter)X
2581(10)X
2691(for)X
2815(more)X
3018(information)X
3457(on)X
3567(the)X
3697(key-)X
555 4228(code.)N
555 4352(For)N
699(the)X
5 f
847 0.1736(XButtonPressedEvent)AX
1 f
1748(and)X
5 f
1915(XButtonReleasedEvent)X
1 f
2864(structures,)X
3250(this)X
3400(member)X
3711(is)X
3792(called)X
555 4448(button.)N
847(It)X
923(represents)X
1301(the)X
1431(pointer)X
1703(buttons)X
1985(that)X
2140(changed)X
2455(state)X
2639(and)X
2788(can)X
2932(be)X
3037(set)X
3157(to)X
3248(the)X
3378(bitwise)X
3655(inclusive)X
555 4544(OR)N
699(of)X
794(one)X
943(or)X
1038(more)X
1241(of)X
1336(these)X
1539(button)X
1787(names:)X
5 f
2077(Button1)X
1 f
2397(,)X
5 f
2459(Button2)X
1 f
2779(,)X
5 f
2841(Button3)X
1 f
3161(,)X
5 f
3223(Button4)X
1 f
3543(,)X
5 f
3605(Button5)X
1 f
3925(.)X
555 4640(For)N
699(the)X
5 f
847(XPointerMovedEvent)X
1 f
1712(structure,)X
2064(this)X
2214(member)X
2525(is)X
2606(called)X
2839(is_hint.)X
3146(It)X
3222(can)X
3366(be)X
3471(set)X
3591(to)X
3682(one)X
3831(of)X
555 4736(these)N
758(constants:)X
5 f
1151(NotifyNormal)X
1 f
1695(or)X
5 f
1808(NotifyHint)X
1 f
2204(.)X
3 f
555 4928(8.4.2.)N
797(Window)X
1137(Entry/Exit)X
1561(Event)X
1803(Processing)X
1 f
555 5052(This)N
734(section)X
1006(describes)X
1355(the)X
1485(processing)X
1883(that)X
2038(occurs)X
2289(for)X
2413(the)X
2543(window)X
2848(crossing)X
3163(events)X
5 f
3428(EnterNotify)X
1 f
555 5148(and)N
5 f
722(LeaveNotify)X
1 f
1202(.)X
1268(If)X
1348(a)X
1409(pointer)X
1681(motion)X
1954(or)X
2049(a)X
2110(window)X
2415(hierarchy)X
2769(change)X
3040(causes)X
3291(the)X
3421(pointer)X
3693(to)X
3784(be)X
3889(in)X
555 5244(a)N
616(different)X
941(window)X
1246(than)X
1420(before,)X
1688(the)X
1818(X)X
1903(server)X
2139(reports)X
5 f
2423(EnterNotify)X
1 f
2896(or)X
5 f
3009(LeaveNotify)X
1 f
3511(events)X
3758(to)X
555 5340(clients)N
808(who)X
981(have)X
1169(selected)X
1475(for)X
1599(these)X
1802(events.)X
2093(All)X
5 f
2246(EnterNotify)X
1 f
2719(and)X
5 f
2886(LeaveNotify)X
1 f
3388(events)X
3635(caused)X
3896(by)X
555 5436(a)N
616(hierarchy)X
970(change)X
1241(are)X
1370(generated)X
1734(after)X
1917(any)X
2066(hierarchy)X
2420(event)X
2633(\()X
5 f
2702(UnmapNotify)X
1 f
3225(,)X
5 f
3287(MapNotify)X
1 f
3698(,)X
5 f
570 5532(Con\256gureNotify)N
1 f
1191(,)X
5 f
1253(GravityNotify)X
1 f
1772(,)X
5 f
1834(CirculateNotify)X
1 f
2420(\))X
2471(caused)X
2732(by)X
2842(that)X
2997(change,)X
3290(but)X
3425(the)X
3555(ordering)X
3875(of)X
5 f
570 5628(EnterNotify)N
1 f
1021(and)X
5 f
1188(LeaveNotify)X
1 f
1690(events)X
1937(with)X
2116(respect)X
2387(to)X
5 f
2496(FocusOut)X
1 f
2894(,)X
5 f
2956(VisibilityNotify)X
1 f
3512(,)X
3556(and)X
5 f
570 5724(Expose)N
1 f
901(events)X
1148(is)X
1229(not)X
1364(constrained)X
1792(by)X
1902(the)X
2032(X)X
2117(protocol.)X
3 f
2217 6144(152)N

153 p
%%Page: 153 157
11 s 0 xH 0 xS 3 f
1 f
10 s
0 32(--)N
4323(--)X
3 f
11 s
555 416(C)N
640(Language)X
1034(X)X
1119(Interface)X
3467(V11,)X
3662(Release)X
3967(1)X
1 f
555 672(This)N
734(contrasts)X
1069(with)X
5 f
1266(MotionNotify)X
1 f
1792(events,)X
2061(which)X
2298(are)X
2427(also)X
2591(generated)X
2955(when)X
3167(the)X
3297(pointer)X
3569(moves,)X
3843(but)X
555 768(the)N
685(pointer)X
957(motion)X
1230(begins)X
1482(and)X
1631(ends)X
1814(in)X
1905(a)X
1966(single)X
2199(window.)X
2548(An)X
5 f
2695(EnterNotify)X
1 f
3168(or)X
5 f
3281(LeaveNotify)X
1 f
3783(event)X
555 864(may)N
729(also)X
893(be)X
998(generated)X
1362(when)X
1574(some)X
1782(client)X
2001(application)X
2416(calls)X
5 f
2618(XChangeActivePointerGrab)X
1 f
3715(,)X
5 f
570 960(XGrabKeyboard)N
1 f
1217(,)X
5 f
1279(XGrabPointer)X
1 f
1828(,)X
1872(and)X
5 f
2039(XUngrabPointer)X
1 f
2680(.)X
555 1084(To)N
675(receive)X
5 f
969(EnterNotify)X
1 f
1442(events)X
1689(in)X
1780(a)X
1841(client)X
2060(application,)X
2497(you)X
2651(must)X
2845(pass)X
3018(a)X
3079(window)X
3384(ID)X
3498(and)X
5 f
570 1180(EnterWindowMask)N
1 f
1340(as)X
1435(the)X
1565(event_mask)X
2008(argument)X
2363(to)X
5 f
2472(XSelectInput)X
1 f
2988(.)X
3054(Likewise,)X
3421(to)X
3512(receive)X
5 f
570 1276(LeaveNotify)N
1 f
1072(events,)X
1341(you)X
1495(pass)X
1668(the)X
1798(window)X
2103(ID)X
2217(and)X
5 f
2384(LeaveWindowMask)X
1 f
3161(.)X
555 1400(The)N
714(members)X
1059(of)X
1154(the)X
5 f
1302(XEnterWindowEvent)X
1 f
2147(and)X
5 f
2314(XLeaveWindowEvent)X
1 f
3188(structures)X
3552(associated)X
555 1496(with)N
734(these)X
937(events)X
1184(are)X
1313(window,)X
1640(root,)X
1826(subwindow,)X
2275(time,)X
2477(x,)X
2565(y,)X
2653(x_root,)X
2927(y_root,)X
3201(mode,)X
3441(detail,)X
555 1592(same_screen,)N
1048(focus,)X
1282(and)X
1431(state.)X
1659(The)X
1818(pointer)X
2090(position)X
2397(reported)X
2712(in)X
2803(the)X
2933(event)X
3146(is)X
3227(always)X
3493(the)X
3623(\256nal)X
3802(posi-)X
555 1688(tion,)N
737(not)X
872(the)X
1002(initial)X
1232(position)X
1539(of)X
1634(the)X
1764(pointer.)X
555 1812(The)N
714(window)X
1019(member)X
1330(is)X
1411(set)X
1531(to)X
1622(the)X
1752(window)X
2057(ID)X
2171(of)X
2266(the)X
2396(window)X
2701(on)X
2811(which)X
3048(the)X
5 f
3196(EnterNotify)X
1 f
3669(or)X
5 f
570 1908(LeaveNotify)N
1 f
1072(event)X
1285(was)X
1443(generated)X
1807(and)X
1956(is)X
2037(referred)X
2336(to)X
2427(as)X
2522(the)X
2652(event)X
2865(window.)X
3192(This)X
3371(is)X
3452(the)X
5 f
