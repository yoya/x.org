!
*showGrip:		False
*calendar*internalBorderWidth: 0
*input:		True
*resize: False
*resizable: True
*title: XCalendar v.4.0
*dayEditor.title: Day Editor
*helpWindow.title: XCalendar Help
*Font: 8x13
*FontList: 8x13

*helpFile:	%%XCALENDAR_LIBDIR%%/xcalendar.hlp
*holidayFile:	%%XCALENDAR_LIBDIR%%/xcalendar.hol

*firstDay:		7

! bitmaps

*prev*bitmap: %%XCALENDAR_LIBDIR%%/larrow.xbm
*succ*bitmap: %%XCALENDAR_LIBDIR%%/rarrow.xbm
*prevday*bitmap: %%XCALENDAR_LIBDIR%%/larrow.xbm
*succday*bitmap: %%XCALENDAR_LIBDIR%%/rarrow.xbm
*helpButton*bitmap: %%XCALENDAR_LIBDIR%%/qmark.xbm
*quitButton*bitmap: %%XCALENDAR_LIBDIR%%/quit.xbm
*prev*labelType: XmPIXMAP
*prev*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/larrow.xbm
*succ*labelType: XmPIXMAP
*succ*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/rarrow.xbm
*prevday*labelType: XmPIXMAP
*prevday*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/larrow.xbm
*succday*labelType: XmPIXMAP
*succday*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/rarrow.xbm
*quitButton*labelType: XmPIXMAP
*quitButton*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/quit.xbm
*helpButton*labelType: XmPIXMAP
*helpButton*labelPixmap: /usr/local/X11R5/lib/X11/xcalendar/qmark.xbm


! colors
! Note: These may cause problems using the -rv option on monochrome displays

*Background: lightgray
*markBackground: gray
*daynames*Background: lightgray
*daynames.SUN*Foreground: Red
*daynames.SAT*Foreground: Black
*daynumbers*Foreground:   Black
*daynumbers.1*Foreground: Red
*daynumbers.8*Foreground: Red
*daynumbers.15*Foreground: Red
*daynumbers.22*Foreground: Red
*daynumbers.29*Foreground: Red
*daynumbers.36*Foreground: Red
*helpButton*Background: slategray
*helpButton*Foreground: White
*quitButton*Background: slategray
*quitButton*Foreground: White
*editorTitle*Background: lightgray
*editorTitle*Foreground: Black
*editorControls*Background: lightgray
*editorControls*Command.Background: slategray
*editorControls*Command.Foreground: White
*editorControls*XmPushButton.Background: slategray
*editorControls*XmPushButton.Foreground: White

*setMarkBackground: True
*markOnStartup: True
*markCurrent: True
*currentForeground: Blue
*markHoliday: True
*holidayForeground: Red
*holiday*fontList: 6x13

*BorderWidth:		2
*calendar.borderWidth:	1
*borderWidth: 0
*date*borderWidth: 0
*date*vertDistance: 5

*date*font:				8x13bold
*daynames*font:			8x13bold
*dayEditor*editorTitle*font: 		8x13bold
*helpWindow*editorTitle*font: 		8x13bold
*date*fontList:			8x13bold
*daynames*fontList:			8x13bold
*dayEditor*editorTitle*fontList: 	8x13bold
*helpWindow*editorTitle*fontList: 	8x13bold

*helpButton*vertDistance: 12
*quitButton*vertDistance: 12
*editorTitle*vertDistance: 5

*editorControls*doneButton*label: Done
*editorControls*saveButton*label: Save
*editorControls*clearEntry*label: Clear Entry
*doneButton*labelString: Done
*saveButton*labelString: Save
*clearEntry*labelString: Clear Entry

*helpWindow*editorTitle*label: Help
*helpWindow*editorTitle*labelString: Help
*helpWindow*rows: 15
*helpWindow*columns: 80
*rows:7
*columns: 30

*dayEditor*Paned*editor.width: 300
*dayEditor*Paned*editor.height: 150
*helpWindow*Paned*editor.width: 600
*helpWindow*Paned*editor.height: 350

*bcontrols*borderWidth: 0
*prev*highlightThickness: 0
*succ*highlightThickness: 0

*prevday*highlightThickness: 0
*succday*highlightThickness: 0
*daybuttons*borderWidth: 0

*Scrollbar.borderWidth: 1
*Text*scrollVertical: whenNeeded
*scrollHorizontal: False
*helpWindow*scrollHorizontal: True
*holiday*cursorPositionVisible: False
*holiday*displayCaret: False
*helpWindow*cursorPositionVisible: False
*helpWindow*displayCaret: False

!! Keyboard accelerators for Athena version

*Box*doneButton*accelerators: #override \n\
	Meta<Key>q: set() notify() reset() \n
*Box*saveButton*accelerators: #override \n\
	Meta<Key>s: set() notify() reset() \n
*Box*clearEntry*accelerators: #override \n\
	Meta<Key>c: set() notify() reset() \n

*Form*prevday*accelerators: #override \n\
	Meta<Key>p: set() notify() reset() \n
*Form*succday*accelerators: #override \n\
	Meta<Key>n: set() notify() reset() \n

*Form*prev*accelerators: #override \n\
	<Key>p: set() notify() reset() \n
*Form*succ*accelerators: #override \n\
	<Key>n: set() notify() reset() \n
*Form*quitButton*accelerators: #override \n\
	<Key>q: set() notify() \n


! Motif text translations to make it work like emacs

*XmText.translations: #override\n\
        Ctrl <Key>b:            backward-character()\n\
        Alt <Key>b:             backward-word()\n\
        Meta <Key>b:            backward-word()\n\
        Shift Alt <Key>b:       backward-word(extend)\n\
        Shift Meta <Key>b:      backward-word(extend)\n\
        Alt <Key>[:             backward-paragraph()\n\
        Meta <Key>[:            backward-paragraph()\n\
        Shift Alt <Key>[:       backward-paragraph(extend)\n\
        Shift Meta <Key>[:      backward-paragraph(extend)\n\
        Alt <Key><:             beginning-of-file()\n\
        Meta <Key><:            beginning-of-file()\n\
        Ctrl <Key>a:            beginning-of-line()\n\
        Shift Ctrl <Key>a:      beginning-of-line(extend)\n\
        Ctrl <Key>osfInsert:    copy-clipboard()\n\
        Shift <Key>osfDelete:   cut-clipboard()\n\
        Shift <Key>osfInsert:   paste-clipboard()\n\
        Alt <Key>>:             end-of-file()\n\
        Meta <Key>>:            end-of-file()\n\
        Ctrl <Key>e:            end-of-line()\n\
        Shift Ctrl <Key>e:      end-of-line(extend)\n\
        Ctrl <Key>f:            forward-character()\n\
        Alt <Key>]:             forward-paragraph()\n\
        Meta <Key>]:            forward-paragraph()\n\
        Shift Alt <Key>]:       forward-paragraph(extend)\n\
        Shift Meta <Key>]:      forward-paragraph(extend)\n\
        Ctrl Alt <Key>f:        forward-word()\n\
        Ctrl Meta <Key>f:       forward-word()\n\
        Ctrl <Key>d:            kill-next-character()\n\
	Ctrl <Key>h:		kill-previous-character()\n\
        Alt <Key>BackSpace:     kill-previous-word()\n\
        Meta <Key>BackSpace:    kill-previous-word()\n\
        Ctrl <Key>w:            key-select() kill-selection()\n\
        Ctrl <Key>y:            unkill()\n\
        Ctrl <Key>k:            kill-to-end-of-line()\n\
        Alt <Key>Delete:        kill-to-start-of-line()\n\
        Meta <Key>Delete:       kill-to-start-of-line()\n\
        Ctrl <Key>o:            newline-and-backup()\n\
        Ctrl <Key>j:            newline-and-indent()\n\
        Ctrl <Key>n:            next-line()\n\
        Ctrl <Key>osfLeft:      page-left()\n\
        Ctrl <Key>osfRight:     page-right()\n\
        Ctrl <Key>p:            previous-line()\n\
        Ctrl <Key>g:            process-cancel()\n\
        Ctrl <Key>l:            redraw-display()\n\
	Ctrl <Key>v:		next-page()\n\
	Meta <Key>v:		previous-page()\n\
        Ctrl <Key>osfDown:      next-page()\n\
        Ctrl <Key>osfUp:        previous-page()\n\
        Ctrl <Key>space:        set-anchor()\n


