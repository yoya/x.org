$!
$! Make ImageMagick X image utilities for VMS.
$!
$
$define/nolog X11 decw$include:
$define/nolog sys sys$library:
$link_options="/nodebug/notraceback"
$define/nolog lnk$library sys$library:vaxcrtl
$
$write sys$output "Making Magick..."
$call Make widget
$call Make X
$call Make image
$call Make shear
$call Make quantize
$call Make colors
$call Make signature
$call Make decode
$call Make encode
$call Make compress
$call Make utility
$call Make error
$call Make PreRvIcccm
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
$        cc/nodebug/optimize/include_directory=[-] 'source_file'
$      endif
$  endif
$exit
$endsubroutine

