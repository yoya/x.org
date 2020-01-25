XCOMM! /bin/csh -f
XCOMM 
XCOMM Generate a Makefile for ibuild
XCOMM 

if ($1 == "-m") then
    set name = $2
    set dest = `basename $2`
    set imakefile = $name-imake
    set makefile = $name-make
    set aout = $name.exe

    shift argv
    shift argv

    if (-f $imakefile) then
	rm -f $imakefile.bak
	mv $imakefile $imakefile.bak
    endif

    echo "XCOMM DO NOT EDIT -- generated by ibmkmf" > $imakefile
    echo " " >> $imakefile

    echo "SPECIAL_IMAKEFLAGS = \" >> $imakefile
    echo "    -f $imakefile -s $makefile -DUseInstalled -DTurnOptimizingOn=0" \
	 >> $imakefile
    echo " " >> $imakefile

    echo "SRCS = \" >> $imakefile
    foreach i ($argv)
	echo "    $i."'$(CCSUFFIX) \' >> $imakefile
	echo "    $i-core."'$(CCSUFFIX) \' >> $imakefile
    end
    echo "    $name-main."'$(CCSUFFIX)' >> $imakefile
    echo "OBJS = \" >> $imakefile
    foreach i ($argv)
	echo "    $i.o \" >> $imakefile
	echo "    $i-core.o \" >> $imakefile
    end
    echo "    $name-main.o" >> $imakefile
    echo "AOUT = $aout" >> $imakefile
    echo " " >> $imakefile

    echo "Use_libUnidraw()" >> $imakefile
    echo "Use_2_6()" >> $imakefile
    echo "ComplexProgramTarget($dest)" >> $imakefile
    echo " " >> $imakefile

    echo "MakeObjectRule($name-main,$name-main,NullParameter)" >> $imakefile
    foreach i ($argv)
	echo "MakeObjectRule($i,$i,NullParameter)" >> $imakefile
	echo "MakeObjectRule($i-core,$i-core,NullParameter)" >> $imakefile
    end

else
    set imakefile = $1-imake
    set makefile = $1-make
    imake CONFIGDIRSPEC \
	-f $imakefile -s $makefile -DUseInstalled -DTurnOptimizingOn=0
endif