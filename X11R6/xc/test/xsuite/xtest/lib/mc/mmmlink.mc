# $XConsortium: mmmlink.mc,v 1.2 92/06/29 19:42:16 rws Exp $
# A version of the test that can be combined with all the other tests for
# the macro version of the function.
#
mlink.c: $(SOURCES)
	$(CODEMAKER) -m -l -o mlink.c $(SOURCES)

linkexec:: m$(LINKEXEC) $(AUXFILES) ;

m$(LINKEXEC): ../Tests
	$(RM) m$(LINKEXEC)
	$(LN) ../Tests m$(LINKEXEC)
