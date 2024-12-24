CFLAGS = -O
NOBSD_CFLAGS = +X -Wc,-Nd2000
#
all:	intrinsic2 editor2 dialog2
#
NOBSD:	intrinsic editor dialog
#
intrinsic:
	cd Intrinsic; make -f INTRINSIC.mk "OS=NOBSD" "CFLAGS=$(NOBSD_CFLAGS)"; cd ..
#
intrinsic2:
	cd Intrinsic; make -f INTRINSIC.mk "OS=BSD" "CFLAGS=$(CFLAGS)"; cd ..
#
editor:
	cd Editor; make -f EDITOR.mk "OS=NOBSD" "CFLAGS=$(NOBSD_CFLAGS)"; cd ..
#
editor2:
	cd Editor; make -f EDITOR.mk "OS=BSD" "CFLAGS=$(CFLAGS)"; cd ..
#
dialog:
	cd Dialog; make -f DIALOG.mk "OS=NOBSD" "CFLAGS=$(NOBSD_CFLAGS)"; cd ..
#
dialog2:
	cd Dialog; make -f DIALOG.mk "OS=BSD" "CFLAGS=$(CFLAGS)"; cd ..
#
clean:
	cd Intrinsic; make -f INTRINSIC.mk clean
	cd Editor; make -f EDITOR.mk clean
	cd Dialog; make -f DIALOG.mk clean
#
clobber:
	cd Intrinsic; make -f INTRINSIC.mk clobber
	cd Editor; make -f EDITOR.mk clobber
	cd Dialog; make -f DIALOG.mk clobber

