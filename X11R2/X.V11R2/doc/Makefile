SUBDIRS= Xlib

all:
	for i in ${SUBDIRS} ; \
	do \
		(cd $$i ; make ${MFLAGS} all ) ; \
	done

clean:
	rm -f core *~ \#* 
	for i in ${SUBDIRS} ; \
	do \
		(cd $$i ; make ${MFLAGS} clean ) ; \
	done

get:
	@echo "Already done"

depend:
	@echo " skipping depend on docs "

build: 
	echo "Skipping docs\n\n\n"

buildtree:
	-(cd ${WHEREAMI} ; rdist -R )
