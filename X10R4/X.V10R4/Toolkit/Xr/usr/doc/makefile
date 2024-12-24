#
#      Makefile for 'X' documentation and associated support files
#
#
FORMATTER=eroff -Tlj+
MANDIR=/usr/man/man3

all:
	@touch pr01
	@touch pr02
	@touch pr03
	@touch pr04
	@touch pr05
	@touch pr0a
	@touch pr0b
	@make pr01.idx
	@make pr02.idx
	@make pr03.idx
	@make pr04.idx
	@make pr05.idx
	@make pr0a.idx
	@make manpages
	@make pr0b.idx
	@make pr00 				# make contents
	@make prii				# make index
	@echo ""
	@echo "Finished Making 'Programming With the X Window System'"
	@echo ""

new:  pr01.idx pr02.idx pr03.idx pr04.idx pr05.idx pr0a.idx pr0b.idx
	@make pr00 				# make contents
	@make prii				# make index
	@echo ""
	@echo "Finished Making changed Chap/App from"\
			" 'Programming With the X Window System'"
	@echo ""
	@touch new

pr01.idx:	pr01
	@echo ""
	@echo "making Chap. 1 --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr01 | $(FORMATTER) -mm
	@mv con.con pr01.con
	@-cat idx.idx >>pr01.idx

pr02.idx:	pr02
	@echo ""
	@echo "making Chap. 2 --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr02 | $(FORMATTER) -mm
	@mv con.con pr02.con
	@-cat idx.idx >>pr02.idx

pr03.idx:	pr03
	@echo ""
	@echo "making Chap. 3 --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr03 | $(FORMATTER) -mm
	@mv con.con pr03.con
	@-cat idx.idx >>pr03.idx

pr04.idx:	pr04
	@echo ""
	@echo "making Chap. 4 --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr04 | $(FORMATTER) -mm
	@mv con.con pr04.con
	@-cat idx.idx >>pr04.idx

pr05.idx:	pr05
	@echo ""
	@echo "making Chap. 5 --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr05 | $(FORMATTER) -mm
	@mv con.con pr05.con
	@-cat idx.idx >>pr05.idx

pr0a.idx:	pr0a
	@echo ""
	@echo "making App. A --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr0a | $(FORMATTER) -mm
	@mv con.con pr0a.con
	@-cat idx.idx >>pr0a.idx

manpages:	pr0a	
	@echo ""
	@echo "making App. A Man Pages"
	tbl $(MANDIR)/Xlib.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XMenu.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrCheckBox.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrEditor.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrEditorGro.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrErrno.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrGlobals.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInit.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInput.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInputConf.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInputInit.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInputMap.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrInputMute.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrMenu.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrMessageBo.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrPageEdit.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrPanel.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrPoint.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrPushButto.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrRadioButt.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrRasterEdi.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrRasterSel.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrRectangle.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrResource.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrScrollBar.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrStaticRas.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrStaticTex.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrTextEdit.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrTitleBar.3x | $(FORMATTER) -man
	tbl $(MANDIR)/XrUtilities.3x | $(FORMATTER) -man
	tbl $(MANDIR)/xtext.3x | $(FORMATTER) -man
	tbl $(MANDIR)/xtty.3x | $(FORMATTER) -man

pr0b.idx:	pr0b
	@echo ""
	@echo "making App. B --updating contents and index"
	@cp blankfile con.con	# just in case no toc entries found
	@cp blankfile idx.idx	# just in case no index entries found
	@tbl pr0b | $(FORMATTER) -mm
	@mv con.con pr0b.con
	@cat idx.idx >>pr0b.idx

pr00:	pr01.con pr02.con pr03.con pr04.con pr05.con pr0a.con pr0b.con
	@echo ""
	@echo "making Table of Contents"
	@cp pr000 pr00
	@cat pr01.con >>pr00
	@cat pr02.con >>pr00
	@cat pr03.con >>pr00
	@cat pr04.con >>pr00
	@cat pr05.con >>pr00
	@cat pr0a.con >>pr00
	@cat pr0b.con >>pr00
	@tbl pr00 | $(FORMATTER) -mm

prii:	pr01.idx pr02.idx pr03.idx pr04.idx pr05.idx pr0a.idx pr0b.idx index
	@echo ""
	@echo "making Index"
	@cat pr01.idx >>prii
	@cat pr02.idx >>prii
	@cat pr03.idx >>prii
	@cat pr04.idx >>prii
	@cat pr05.idx >>prii
	@cat pr0a.idx >>prii
	@cat pr0b.idx >>prii
	@makeindex prii		# script using the 'index' program
	@mv prii.out prii.tmp
	@cp priii prii.out
	@cat prii.tmp >>prii.out
	@rm prii.tmp
	@$(FORMATTER) -mm prii.out

index:	idx.lex
	@echo ""
	@echo "Making 'index' filter"
	@cc -o index lex.yy.c -ll
	@rm lex.yy.c

idx.lex:		
	@lex idx.lex

