!***************************************************************!
!	      Copyright (C) 1987 by Interleaf, Inc.		!
!	      Ten Canal Park, Cambridge, Mass. 02141		!
! See andrew/odatrans/ileaf/ascii-reader/COPYRIGHT		!
!***************************************************************!

CDEFINES = /DEFINE=INTERLEAF_INCLUDE_FILES

.INCLUDE [000000]GLOBAL.MMS

.FIRST
	DEFINE /NOLOG C$INCLUDE [VMS.OS], [INCLUDE]

OBJECT_FILES1 = -
 AR5AUTONUM.OBJ,-
 AR5CVT.OBJ,-
 AR5ERROR.OBJ,-
 AR5IO.OBJ,-
 AR5PRINTF.OBJ,-
 AR5STRINGS.OBJ,-
 AR5BUF.OBJ,-
 AR5DOCREAD.OBJ,-
 AR5FONT.OBJ,-
 AR5LEX.OBJ,-
 AR5READ.OBJ,-
 AR5TOKEN.OBJ

OBJECT_FILES2 = -
 AR5CMPN.OBJ,-
 AR5DUMP.OBJ,-
 AR5FREE.OBJ,-
 AR5FREEDG.OBJ,-
 AR5INCHARMAP.OBJ,-
 AR5INDEX.OBJ,-
 AR5LOADDG.OBJ,-
 AR5REF.OBJ,-
 AR5STUBS.OBJ,-
 AR5CSAVE.OBJ,-
 AR5DUMPDG.OBJ,-
 AR5FRAMEREAD.OBJ,-
 AR5INIT.OBJ,-
 AR5MEMORY.OBJ,-
 AR5SHRUTILS.OBJ,-
 AR5TABLE.OBJ,-
 AR5UNKNOWNS.OBJ


COMPILE_ALL : [LIB]LIBAR5.OLB
	@ ! No actions

[LIB]LIBAR5.OLB : $(OBJECT_FILES1) $(OBJECT_FILES2)
	LIBRARY/CREATE D_USER:$(MMS$TARGET) $(OBJECT_FILES1)
	LIBRARY/INSERT D_USER:$(MMS$TARGET) $(OBJECT_FILES2)
	- PURGE D_USER:$(MMS$TARGET)	