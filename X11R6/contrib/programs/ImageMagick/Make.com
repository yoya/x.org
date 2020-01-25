$!
$! Make ImageMagick X image utilities for VMS.
$!
$
$define/nolog X11 decw$include:
$define/nolog sys sys$library:
$link_options="/nodebug/notraceback"
$define/nolog lnk$library sys$library:vaxcrtl
$
$write sys$output "Making in [.magick]"
$set default [.magick]
$@make
$set default [-]

$if ((p1.nes."").and.(p1.nes."display")) then goto SkipDisplay
$write sys$output "Making Display..."
$call Make display
$
$link'link_options' display,[.magick]widget,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$display:==$'f$environment("default")'display
$write sys$output "..symbol DISPLAY defined."
$
$SkipDisplay:
$if ((p1.nes."").and.(p1.nes."import")) then goto SkipImport
$write sys$output "Making Import..."
$call Make import
$
$link'link_options' import,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$import:==$'f$environment("default")'import
$write sys$output "..symbol IMPORT defined."
$SkipImport:
$
$if ((p1.nes."").and.(p1.nes."animate")) then goto SkipAnimate
$write sys$output "Making Animate..."
$call Make animate
$
$link'link_options' animate,[.magick]widget,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$animate:==$'f$environment("default")'animate
$write sys$output "..symbol ANIMATE defined."
$
$SkipAnimate:
$if ((p1.nes."").and.(p1.nes."montage")) then goto SkipMontage
$write sys$output "Making Montage..."
$call Make montage
$
$link'link_options' montage,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$montage:==$'f$environment("default")'montage
$write sys$output "..symbol MONTAGE defined."
$
$SkipMontage:
$if ((p1.nes."").and.(p1.nes."mogrify")) then goto SkipMogrify
$write sys$output "Making Mogrify..."
$call Make mogrify
$
$link'link_options' mogrify,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$mogrify:==$'f$environment("default")'mogrify
$write sys$output "..symbol MOGRIFY defined."
$
$SkipMogrify:
$if ((p1.nes."").and.(p1.nes."convert")) then goto SkipConvert
$write sys$output "Making Convert..."
$call Make convert
$
$link'link_options' convert,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$convert:==$'f$environment("default")'convert
$write sys$output "..symbol CONVERT defined."
$SkipConvert:
$if ((p1.nes."").and.(p1.nes."combine")) then goto SkipCombine
$write sys$output "Making Combine..."
$call Make combine
$
$link'link_options' combine,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$combine:==$'f$environment("default")'combine
$write sys$output "..symbol COMBINE defined."
$SkipCombine:
$if ((p1.nes."").and.(p1.nes."segment")) then goto SkipSegment
$write sys$output "Making Segment..."
$call Make segment
$
$link'link_options' segment,[.magick]X,[.magick]image, -
  [.magick]shear,[.magick]quantize,[.magick]colors,[.magick]signature, -
  [.magick]decode,[.magick]encode,[.magick]compress,[.magick]utility, -
  [.magick]error,[.magick]PreRvIcccm,sys$input:/opt
sys$share:decw$xlibshr.exe/share
$
$segment:==$'f$environment("default")'segment
$write sys$output "..symbol SEGMENT defined."
$SkipSegment:
$type sys$input

Use this command to specify which X server to contact:

  $set display/create/node=node_name::

This can be done automatically from your LOGIN.COM with the following
command:

  $if (f$trnlmn("sys$rem_node").nes."") then -
  $  set display/create/node='f$trnlmn("sys$rem_node")'
$exit
$
$Make: subroutine
$!
$! A very primitive "make" (or MMS) hack for DCL.
$!
$if (p1.eqs."") then exit
$source_file=f$search(f$parse(p1,".c"))
$if (source_file.nes."")
$  then
$    object_file=f$parse(source_file,,,"name")+".obj"
$    object_file=f$search( object_file )
$    if (object_file.nes."")
$      then
$        object_time=f$file_attribute(object_file,"cdt")
$        source_time=f$file_attribute(source_file,"cdt")
$        if (f$cvtime(object_time).lts.f$cvtime(source_time)) then -
$          object_file=""
$      endif
$    if (object_file.eqs."")
$      then
$        write sys$output "Compiling ",p1
$        cc/nodebug/optimize/include_directory=[.magick] 'source_file'
$      endif
$  endif
$exit
$endsubroutine
