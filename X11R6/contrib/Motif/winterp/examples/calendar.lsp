; -*-Lisp-*-
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; File:         calendar.lsp
; RCS:          $Header: /users/npm/src/winterp/examples/RCS/calendar.lsp,v 2.1 1994/06/06 14:35:02 npm Exp $
; Description:  Simple and Stupid Calendar UI generated by Unix 'cal' command.
;		To start up this application "standalone", do
;		"env WINTERP_STANDALONE_APP=TRUE winterp -init_file cal.lsp -no_stdin_serv -no_unix_serv"
;		doing so will cause WINTERP to terminate when the calendar window
;		is closed (via wm's f.close), rather than just deleting the window.
; Author:       Niels P. Mayer
; Created:      Tue Oct 12 11:10:44 1993
; Modified:     Sun Jun  5 18:28:10 1994 (Niels Mayer) npm@indeed
; Language:     Lisp
; Package:      N/A
; Status:       X11r6 contrib release
;
; Copyright (C) 1994, Enterprise Integration Technologies Corp. and Niels Mayer.
; WINTERP 1.15-1.99, Copyright (c) 1993, Niels P. Mayer.
; WINTERP 1.0-1.14, Copyright (c) 1989-1992 Hewlett-Packard Co. and Niels Mayer.
; 
; Permission to use, copy, modify, distribute, and sell this software and its
; documentation for any purpose is hereby granted without fee, provided that
; the above copyright notice appear in all copies and that both that
; copyright notice and this permission notice appear in supporting
; documentation, and that the name of Enterprise Integration Technologies,
; Hewlett-Packard Company, or Niels Mayer not be used in advertising or
; publicity pertaining to distribution of the software without specific,
; written prior permission. Enterprise Integration Technologies, Hewlett-Packard
; Company, and Niels Mayer makes no representations about the suitability of
; this software for any purpose.  It is provided "as is" without express or
; implied warranty.
; 
; ENTERPRISE INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD COMPANY AND NIELS MAYER
; DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL ENTERPRISE
; INTEGRATION TECHNOLOGIES, HEWLETT-PACKARD COMPANY OR NIELS MAYER BE LIABLE
; FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
; RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
; CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
; CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(require "lib-utils/initialize")	;defines :GET method on WIDGET_CLASS
(require "lib-utils/unixstuf")		;define read-exec-cmd, decide-winterp-top-level-shell and other unixisms...
(require "xlisp-2.1d/common")		;define FILL
(require "lib-utils/redir-err")		;pops up dialog box showing stderr output
(require "lib-utils/show-busy")		;define WINTERP-SHOW-BUSY-PROGN
(require "lib-widgets/clock-disp")	;define Clock_Display_Widget_Class

;; IF WINTERP started w/ "env WINTERP_STANDALONE_APP=TRUE winterp -init_file ..."
(if (winterp-standalone-p)
    ;; THEN LOAD redir-out so that users get warned about XLISP errors occuring (e.g. from trying
    ;; browse a deleted file). Users using WINTERP interactively and loading this will probably 
    ;; not want their stdout suddenly appearing in a dialog box, so that's why we only load this
    ;; for a WINTERP application started standalone via "env WINTERP_STANDALONE_APP=TRUE ..."
    (require "lib-utils/redir-out")	;pops up dialog box showing stdout output
  )

(defvar *CALENDAR-MONTHS-STRING-ARRAY*
	#("January" "February" "March" "April" "May" "June" "July" "August" "September" "October" "November" "December")
	)
(defvar	*CALENDAR-MONTHS-MNEMONIC-ARRAY*
	#(#\J      #\F        #\M     #\A       #\y #\J     #\u     #\g    #\S         #\O       #\N        #\D)
	)
(defvar	*CALENDAR-YEARS-STRING-ARRAY*
	#("1990" "1991" "1992" "1993" "1994" "1995" "1996" "1997" "1998" "1999")
	)
(defvar *CALENDAR-YEARS-MNEMONIC-ARRAY*
	#(  #\0    #\1    #\2    #\3    #\4    #\5    #\6    #\7    #\8    #\9)
	)

(defvar *CALENDAR-DIRECTORY-STR*
  (concatenate 'string *HOME-DIRECTORY-STR* "/.winterp-calendar"))

;;
;; Make sure directory named by *CALENDAR-DIRECTORY-STR* exists...
;;
(let ((str (read-exec-cmd (concatenate 'string "ls -ld " *CALENDAR-DIRECTORY-STR*))))
  ;; if the directory named by *CALENDAR-DIRECTORY-STR* doesn't exist
  (if (or (null str)			;no such file
	  (string/= "drwx" str :start1 0 :end1 4 :start2 0 :end2 4)) ;not a directory
      ;; THEN delete file (if any), create DIRECTORY ....
      (system (format nil "/bin/rm -f ~A ; mkdir ~A"
		      *CALENDAR-DIRECTORY-STR*
		      *CALENDAR-DIRECTORY-STR*
		      ))
    ))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun create-day-browser (parent_w year-str month-str day-str)
  (let*
      (;; loc vars
       (fname
	(format NIL "~A-~2,,,'0@A-~2,,,'0@A"
		year-str
		(1+ (position-if #'(lambda (x) (string= x month-str))
				 *CALENDAR-MONTHS-STRING-ARRAY*))
		day-str))
       (filename
	(concatenate 'string *CALENDAR-DIRECTORY-STR* "/" fname)
	)
       (top_w
	(send XM_FORM_WIDGET_CLASS :new :unmanaged :dialog
	      "calendar-editor" parent_w
	      :XMN_DIALOG_TITLE		(format NIL "WINTERP:Calendar for ~A ~A, ~A" month-str day-str year-str)
	      :XMN_DELETE_RESPONSE	:destroy
	      :XMN_AUTO_UNMANAGE	NIL
	      :XMN_FRACTION_BASE	5
	      ))
       (te_frame_w
	(send XM_FRAME_WIDGET_CLASS :new :managed
	      "frame" top_w
	      :XMN_TOP_ATTACHMENT	:attach_form
	      :XMN_LEFT_ATTACHMENT	:attach_form
	      :XMN_RIGHT_ATTACHMENT	:attach_form
	      ))
       (te_w
	(send XM_TEXT_WIDGET_CLASS :new :managed :scrolled
	      "text" te_frame_w
	      :XMN_ROWS			24
	      :XMN_COLUMNS		80
	      :XMN_EDIT_MODE		:multi_line_edit
	      ))
       (save_btn_w
	(send XM_PUSH_BUTTON_GADGET_CLASS :new :managed
	      "save" top_w
	      :XMN_LABEL_STRING		"Save"
	      :XMN_TOP_ATTACHMENT	:attach_widget
	      :XMN_TOP_WIDGET		te_frame_w
	      :XMN_BOTTOM_ATTACHMENT	:attach_form
	      :XMN_LEFT_ATTACHMENT	:attach_position
	      :XMN_LEFT_POSITION	1
	      :XMN_RIGHT_ATTACHMENT	:attach_position
	      :XMN_RIGHT_POSITION	2
	      ))
       (cancel_btn_w
	(send XM_PUSH_BUTTON_GADGET_CLASS :new :managed
	      "cancel" top_w
	      :XMN_LABEL_STRING		"Close"
	      :XMN_TOP_ATTACHMENT	:attach_widget
	      :XMN_TOP_WIDGET		te_frame_w
	      :XMN_BOTTOM_ATTACHMENT	:attach_form
	      :XMN_LEFT_ATTACHMENT	:attach_position
	      :XMN_LEFT_POSITION	3
	      :XMN_RIGHT_ATTACHMENT	:attach_position
	      :XMN_RIGHT_POSITION	4
	      ))
       )

    (send top_w :set_values 
	  :XMN_DEFAULT_BUTTON save_btn_w
	  :XMN_CANCEL_BUTTON  cancel_btn_w)

    (open filename :direction :probe :if-does-not-exist :create)
    (send te_w :read_file filename)
    (send top_w :manage)

    (send save_btn_w :add_callback :XMN_ACTIVATE_CALLBACK
	  '()
	  '(
	    (send te_w :write_file filename)
	    ))

    (send cancel_btn_w :add_callback :XMN_ACTIVATE_CALLBACK
	  '()
	  '(
	    ;; TODO -- don't allow destroy until changes have been saved.
	    (send top_w :destroy)
	    ))
    ))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun create-calendar-widget (parent_w &optional month-num year-str)
  (let ((cal_w		(send XM_FORM_WIDGET_CLASS :new :unmanaged
			      "cal" parent_w
			      :XMN_FRACTION_BASE	7
			      ))
	(cur-month-str	NIL)
	(cur-year-str	NIL)
	(i		0)
	(y		0)
	(fp		NIL)
	)
    (progv '(*breakenable*) '(nil)
	   (unwind-protect		;unwind protect s.t. we close 'fp' no matter what...
	       (progn
		 (setq fp
		       (if (null month-num) ;do current month if optional month-num arg not passed
			   (popen "cal" :direction :input)
			 (popen (format nil "cal ~A ~A" month-num year-str)
				:direction :input)
			 )
		       cur-month-str (fscanf-string fp " %s ")
		       cur-year-str  (fscanf-string fp " %s ")
		       )

		 (dolist
		  (date-str
		   (list (fscanf-string fp " %s ") ;sunday
			 (fscanf-string fp " %s ") ;monday
			 (fscanf-string fp " %s ") ;tuesday
			 (fscanf-string fp " %s ") ;wednesday
			 (fscanf-string fp " %s ") ;thursday
			 (fscanf-string fp " %s ") ;friday
			 (fscanf-string fp " %s%*c") ;saturday -- %*c reads over newline
			 ))

		  (send XM_LABEL_GADGET_CLASS :new :managed
			"date-label" cal_w
			:XMN_LABEL_STRING		date-str
			:XMN_LEFT_ATTACHMENT		:attach_position
			:XMN_LEFT_POSITION		(rem i 7)
			:XMN_RIGHT_ATTACHMENT		:attach_position
			:XMN_RIGHT_POSITION		(1+ (rem i 7))
			:XMN_TOP_ATTACHMENT		:attach_position
			:XMN_TOP_POSITION		y
			:XMN_BOTTOM_ATTACHMENT		:attach_position
			:XMN_BOTTOM_POSITION		(1+ y)
			)
		  (setq i (1+ i)
			y (truncate i 7))
		  )

		 ;; create blank space for the "blank days" at the beginning of month
		 ;; figure out how many blank days by getting the number of whitespace
		 ;; chars up till the first numeric date, divide that by 3
		 ;; (3 == 2 chars per day column + 1 space).
		 (dotimes (x (truncate	;truncate/2 == integer division
			      (1- (length (fscanf-string fp "%[^0-9]")))
			      3))
			  (setq i (1+ i)
				y (truncate i 7))
			  )

		 (do ((day-str (fscanf-string fp " %s ")
			       (fscanf-string fp " %s "))
		      )
		     ((null day-str)
		      )
		     (send
		      (send XM_PUSH_BUTTON_GADGET_CLASS :new :managed
			    "day-btn" cal_w
			    :XMN_LABEL_STRING		day-str
			    :XMN_LEFT_ATTACHMENT	:attach_position
			    :XMN_LEFT_POSITION		(rem i 7)
			    :XMN_RIGHT_ATTACHMENT	:attach_position
			    :XMN_RIGHT_POSITION		(1+ (rem i 7))
			    :XMN_TOP_ATTACHMENT		:attach_position
			    :XMN_TOP_POSITION		y
			    :XMN_BOTTOM_ATTACHMENT	:attach_position
			    :XMN_BOTTOM_POSITION	(1+ y)
			    )
		      :add_callback :XMN_ACTIVATE_CALLBACK '()
		      `(
			(create-day-browser ,parent_w ,cur-year-str
					    ,cur-month-str ,day-str)
			))
		     (setq i (1+ i)
			   y (truncate i 7)
			   )
		     )

		 (send cal_w :manage)
		 )

	     ;; unwind always
	     (if fp
		 (pclose fp))
	     ))
    cal_w				;RETURN value
    ))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;; CREATE THE CALENDAR GUI ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(let (toplevel_w form_w clock_w sep0_w month_opt_w
		 year_opt_w cal_frame_w calendar_w)
  (setq toplevel_w
	(send (decide-winterp-top-level-shell) :new 
	      "winterpCalendar"		;app-instance name
	      "WinterpCalendar"		;app-class name
	      :XMN_TITLE	"WINTERP: Calendar"
	      :XMN_ICON_NAME	"W:calendar"
	      ))
  (setq form_w
	(send XM_FORM_WIDGET_CLASS :new :managed
	      "form" toplevel_w
	      ))
  (setq clock_w
	(send Clock_Display_Widget_Class :new :managed
	      "clock" form_w
	      :XMN_TOP_ATTACHMENT	:attach_form
	      :XMN_LEFT_ATTACHMENT	:attach_form
	      :XMN_RIGHT_ATTACHMENT	:attach_form
	      ))
  (setq sep0_w
	(send XM_SEPARATOR_GADGET_CLASS :new :managed
	      "sep0" form_w
	      :XMN_ORIENTATION		:horizontal
	      :XMN_LEFT_ATTACHMENT	:attach_form
	      :XMN_RIGHT_ATTACHMENT	:attach_form
	      :XMN_TOP_ATTACHMENT	:attach_widget
	      :XMN_TOP_WIDGET		clock_w
	      ))

  ;; This is a quick and dirty way to get current month and year in order to
  ;; set the initially displayed month for the month_opt_w and year_opt_w
  ;; below. It might be better to just get it from clock_w, except that clock_w
  ;; hasn't been realized yet, and we need the values right now, before
  ;; any widgetry is created.
  (let ((fp NIL) cur-month-str cur-year-str)
    (progv '(*breakenable*) '(nil)
	   (unwind-protect		;unwind protect s.t. we close 'fp' no matter what...
	       (progn
		 (setq fp (popen "cal" :direction :input))
		 (setq cur-month-str (fscanf-string fp " %s "))
		 (setq cur-year-str  (fscanf-string fp " %s "))
		 )
	     ;; unwind always
	     (if fp
		 (pclose fp))
	     ))
    (setq month_opt_w
	  (send XM_ROW_COLUMN_WIDGET_CLASS :new :managed :simple_option_menu
		"month-opt" form_w
		:XMN_OPTION_LABEL	"Mo:"
		:XMN_OPTION_MNEMONIC	#\M
		:XMN_BUTTON_COUNT	(length *CALENDAR-MONTHS-STRING-ARRAY*)
		:XMN_BUTTON_TYPE	(fill (make-array (length *CALENDAR-MONTHS-STRING-ARRAY*)) :PUSHBUTTON)
		:XMN_BUTTONS		*CALENDAR-MONTHS-STRING-ARRAY*		
		:XMN_BUTTON_MNEMONICS 	*CALENDAR-MONTHS-MNEMONIC-ARRAY*
		:XMN_BUTTON_SET		(position-if #'(lambda (x) (string= x cur-month-str))
						     *CALENDAR-MONTHS-STRING-ARRAY*)
		:XMN_TOP_ATTACHMENT	:attach_widget
		:XMN_TOP_WIDGET		sep0_w
		:XMN_LEFT_ATTACHMENT	:attach_form
		))
    (setq year_opt_w
	  (send XM_ROW_COLUMN_WIDGET_CLASS :new :managed :simple_option_menu
		"month-opt" form_w
		:XMN_OPTION_LABEL	"Yr:"
		:XMN_OPTION_MNEMONIC	#\Y
		:XMN_BUTTON_COUNT	(length *CALENDAR-YEARS-STRING-ARRAY*)
		:XMN_BUTTON_TYPE	(fill (make-array (length *CALENDAR-MONTHS-STRING-ARRAY*)) :PUSHBUTTON)
		:XMN_BUTTONS		*CALENDAR-YEARS-STRING-ARRAY*		
		:XMN_BUTTON_MNEMONICS 	*CALENDAR-YEARS-MNEMONIC-ARRAY*
		:XMN_BUTTON_SET		(position-if #'(lambda (x) (string= x cur-year-str))
						     *CALENDAR-YEARS-STRING-ARRAY*)
		:XMN_TOP_ATTACHMENT	:attach_widget
		:XMN_TOP_WIDGET		sep0_w
		:XMN_LEFT_ATTACHMENT	:attach_widget
		:XMN_LEFT_WIDGET	month_opt_w
		:XMN_RIGHT_ATTACHMENT	:attach_form
		))
    )
  (setq cal_frame_w
	(send XM_FRAME_WIDGET_CLASS :new :managed
	      "cal_frame" form_w
	      :XMN_TOP_ATTACHMENT	:attach_widget
	      :XMN_TOP_WIDGET		year_opt_w
	      :XMN_LEFT_ATTACHMENT	:attach_form
	      :XMN_RIGHT_ATTACHMENT	:attach_form
	      :XMN_BOTTOM_ATTACHMENT	:attach_form
	      ))

  (setq calendar_w (create-calendar-widget cal_frame_w))

  (send toplevel_w :realize)

  (send (send month_opt_w :GET_SUB_MENU_WIDGET)	:set_callback
	:XMN_ENTRY_CALLBACK '(CALLBACK_ENTRY_WIDGET)
	'(
	  (winterp-show-busy-progn
	   (send calendar_w :destroy)
	   (setq calendar_w
		 (create-calendar-widget
		  cal_frame_w
		  ;; retrieve numeric month from current setting of month_opt_w
		  (1+ (read (make-string-input-stream (send CALLBACK_ENTRY_WIDGET :name) 7)))
		  ;; retrieve string year from the year_opt_w
		  (xm_string_get_l_to_r (send (send (send year_opt_w :GET_SUB_MENU_WIDGET) :get :XMN_MENU_HISTORY)
					      :get :XMN_LABEL_STRING))
		  ))
	   )
	  ))
  (send (send year_opt_w :GET_SUB_MENU_WIDGET) :set_callback
	:XMN_ENTRY_CALLBACK '(CALLBACK_ENTRY_WIDGET)
	'(
	  (winterp-show-busy-progn
	   (send calendar_w :destroy)
	   (setq calendar_w
		 (create-calendar-widget
		  cal_frame_w
		  ;; retrieve numeric month from current setting of month_opt_w
		  (1+ (read (make-string-input-stream
			     (send (send (send month_opt_w :GET_SUB_MENU_WIDGET) :get :XMN_MENU_HISTORY) :name)
			     7)))
		  ;; retrieve string year from the year_opt_w
		  (xm_string_get_l_to_r (send CALLBACK_ENTRY_WIDGET :get :XMN_LABEL_STRING))
		  ))
	   )
	  ))
  )
