*** X.V11R1/server/ddx/sun/Makefile
--- update1/server/ddx/sun/Makefile
***************
*** 80,86 ****
  	sunCG2C.c \
  	sunCG2M.c \
  	sunCG4C.c \
! 	sunKeyMap.c
  
  OFILES = sunInit.o
  
--- 80,87 ----
  	sunCG2C.c \
  	sunCG2M.c \
  	sunCG4C.c \
! 	sunKeyMap.c \
! 	kbd_mode.c
  
  OFILES = sunInit.o
  
***************
*** 114,144 ****
  	ar clq $@ $(OBJS)
  	ranlib $@
  
!            OBJS = kbd_mode.o
!            SRCS = kbd_mode.c
! 
!         PROGRAM = kbd_mode
! 
! all:: kbd_mode
! 
! kbd_mode: $(OBJS) $(LOCAL_LIBRARIES)
  	rm -f $@
! 	$(CC) -o $@ $(OBJS) $(LOCAL_LIBRARIES) $(LDFLAGS)
  
! relink:
! 	rm -f $(PROGRAM)
! 	$(MAKE) $(MFLAGS) $(PROGRAM)
  
  install:: kbd_mode
  	$(INSTALL) -c $(INSTALLFLAGS) kbd_mode $(BINDIR)
  
  install:: kbd_mode.man
  	$(INSTALL) -c -m 0664 kbd_mode.man $(MANDIR)/kbd_mode.n
- 
- depend::
- 	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)
- clean::
- 	rm -f $(PROGRAM)
  
  depend::
  	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)
--- 115,136 ----
  	ar clq $@ $(OBJS)
  	ranlib $@
  
! kbd_mode:  kbd_mode.o
  	rm -f $@
! 	$(CC) -o $@  kbd_mode.o  $(LDFLAGS)
  
! relink::
! 	rm -f kbd_mode
! 	$(MAKE) $(MFLAGS) kbd_mode
  
+ clean::
+ 	rm -f kbd_mode
+ 
  install:: kbd_mode
  	$(INSTALL) -c $(INSTALLFLAGS) kbd_mode $(BINDIR)
  
  install:: kbd_mode.man
  	$(INSTALL) -c -m 0664 kbd_mode.man $(MANDIR)/kbd_mode.n
  
  depend::
  	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)
