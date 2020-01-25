*** X.V11R1/server/Makefile	Mon Sep 14 19:39:21 1987
--- update1/server/Makefile	Wed Sep 16 17:53:35 1987
***************
*** 260,272 ****
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Realdepend \
! 		"SERVERSUBDIRS=$$dirs"
  
  Realdepend:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "depend/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) depend ); \
  	done
  
  install::
--- 260,274 ----
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Realdepend \
! 		"SERVERSUBDIRS=$$dirs" \
! 		LINT=$(LINT) DESTDIR=$(DESTDIR)
  
  Realdepend:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "depend/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) depend \
! 			LINT=$(LINT) DESTDIR=$(DESTDIR) ); \
  	done
  
  install::
***************
*** 274,286 ****
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Realinstall \
! 		"SERVERSUBDIRS=$$dirs"
  
  Realinstall:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "install/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) install ); \
  	done
  
  lintlib::
--- 276,290 ----
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Realinstall \
! 		"SERVERSUBDIRS=$$dirs" \
! 		LINT=$(LINT) DESTDIR=$(DESTDIR)
  
  Realinstall:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "install/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) install \
! 			LINT=$(LINT) DESTDIR=$(DESTDIR) ); \
  	done
  
  lintlib::
***************
*** 288,300 ****
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Reallintlib \
! 		"SERVERSUBDIRS=$$dirs"
  
  Reallintlib:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "lintlib/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) lintlib ); \
  	done
  
  $(EXTDIR) $(SUBDIRS): FRC
--- 292,306 ----
  		dirs="$$dirs \$$($${i}DIRS)"; \
  	done; \
  	$(MAKE) $(MFLAGS) Reallintlib \
! 		"SERVERSUBDIRS=$$dirs" \
! 		LINT=$(LINT) DESTDIR=$(DESTDIR)
  
  Reallintlib:
  	@subdirs=`ls -d $(SERVERSUBDIRS) | sort -u`; \
  	for i in $$subdirs; do \
  		echo "lintlib/**/ing $$i"; \
! 		(cd $$i ; $(MAKE) $(MFLAGS) lintlib \
! 			LINT=$(LINT) DESTDIR=$(DESTDIR) ); \
  	done
  
  $(EXTDIR) $(SUBDIRS): FRC
