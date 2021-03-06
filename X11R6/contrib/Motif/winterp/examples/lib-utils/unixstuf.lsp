;; -*-Lisp-*-
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; File:         unixstuf.lsp
; RCS:          $Header: /users/npm/src/winterp/examples/lib-utils/RCS/unixstuf.lsp,v 2.1 1994/06/06 14:54:46 npm Exp $
; Description:  Various and Sundry UN*X interfaces.
; Author:       Niels P. Mayer
; Created:      1993
; Modified:     Mon Jun  6 00:57:47 1994 (Niels Mayer) npm@indeed
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; READ-EXEC-CMD-MULTILINE: returns a string representing all lines of 
;; output to stdout generated by executing string parameter <cmd> in a shell
;; (sh(1)). If <cmd> outputs error output to stderr, it will be appear on
;; stderr; use lib-utils/redir-err.lsp to pop up a warning dialog box on 
;; stderr output.
;; If the command succeeds, a STRING result is returned; if the command fails,
;; NIL is returned.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun read-exec-cmd-multiline (cmd)
  (let ((cmd-status NIL)
	(line-list  NIL)
	(fp         NIL))
    (progv '(*breakenable*) '(nil)
	   (unwind-protect		;unwind protect s.t. we close 'fp' no matter what...
	       (progn 
		 (setq fp (popen cmd :direction :input))
		 (do* ((line (read-line fp) (read-line fp))) ;READ LINES...
		      ((null line))	;UNTIL NO MORE OUTPUT TO READ
		      (setq line-list (cons line (cons "\n" line-list)))
		      ))
	     ;; unwind always
	     (if fp
		 (setq cmd-status (pclose fp)))
	     ))
    (if (eq cmd-status T)		;RETURN a STRING only on successful cmd execution
	(apply #'concatenate 'string (cdr (reverse line-list))) ;RETURN VALUE
      NIL				;RETURN NIL if command failed...
      )
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; READ-EXEC-CMD: returns a string representing the first line of output to
;; stdout generated by executing string parameter <cmd> in a shell (sh(1).)
;; If the command succeeds, a STRING result is returned; if the command fails,
;; NIL is returned.
;;
;; If <cmd> outputs error output to stderr, it will be discarded. Note,
;; however, that if the command itself doesn't exist or isn't on the path,
;; you'll get an error message to stdout, e.g. (read-exec-cmd "foo")
;; will cause Irix to return the following to stderr
;; "UX:sh (sh): ERROR: ll: Not found".
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun read-exec-cmd (cmd)
  (let ((cmd-status NIL)
	(fp         NIL)
	(result     NIL)
	)
    (progv '(*breakenable*) '(nil)
	   (unwind-protect
	       (progn
		 (setq fp (popen (concatenate 'string cmd " 2>/dev/null")))
		 (setq result (read-line fp))
		 )
	     ;; unwind always
	     (if fp
		 (setq cmd-status (pclose fp)))
	     ))
    (if (eq cmd-status T)		;RETURN a STRING only on successful cmd execution
	result				;RETURN VALUE
      NIL				;RETURN NIL if command failed...
      )
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; READ-FIXNUM-EXEC-CMD: just like READ-EXEC-CMD, except that a FIXNUM is
;; returned representing the output of executing string parameter <cmd> in a
;; shell (sh(1).) If the command succeeds, a FIXNUM result is returned; if
;; the command fails, NIL is returned.
;;
;; If <cmd> outputs error output to stderr, it will be discarded. Note,
;; however, that if the command itself doesn't exist or isn't on the path,
;; you'll get an error message to stdout, e.g. (read-exec-cmd "foo")
;; will cause Irix to return the following to stderr
;; "UX:sh (sh): ERROR: ll: Not found".
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun read-fixnum-exec-cmd (cmd)
  (let ((cmd-status NIL)
	(fp	    NIL)
	(result	    NIL)
	)
    (progv '(*breakenable*) '(nil)
	   (unwind-protect
	       (progn
		 (setq fp (popen (concatenate 'string cmd " 2>/dev/null")))
		 (setq result (FSCANF-FIXNUM fp "%d"))
		 )
	     ;; unwind always
	     (if fp
		 (setq cmd-status (pclose fp)))
	     ))
    (if (eq cmd-status T)		;RETURN a STRING only on successful cmd execution
	result				;RETURN VALUE
      NIL				;RETURN NIL if command failed...
      )
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (UNIX:GET-UNAME)
;; calls uname(1) to return a STRING representing the name of the Unix
;; variant WINTERP is running under. Examples include "IRIX", "HP-UX", 
;; "OSF1", "SunOS", "ULTRIX", etc.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defvar *WINTERP-UNIX-NAME-INFO* NIL)
(defun UNIX:GET-UNAME ()
  (if (arrayp *WINTERP-UNIX-NAME-INFO*)	;previous UNIX:GET-UNAME succeeded
      (aref *WINTERP-UNIX-NAME-INFO* 0)	;RETURN unix name STRING
    ;; else, call "uname -a" to retrieve info from system
    (let ((cmd-status	NIL)
	  (fp		NIL)
	  (result	(make-array 3))
	  )
      (progv '(*breakenable*) '(nil)
	     (unwind-protect
		 (progn
		   (setq fp (popen "uname -a 2> /dev/null"))
		   (setf (aref result 0) (FSCANF-STRING fp "%s %*s"))
		   (setf (aref result 1) (FSCANF-FIXNUM fp " %d."))
		   (setf (aref result 2) (FSCANF-FIXNUM fp "%d"))
		   )
	       ;; unwind always
	       (if fp
		   (setq cmd-status (pclose fp)))
	       ))
      (cond
       ((eq cmd-status T)		;successful cmd execution
	(setq *WINTERP-UNIX-NAME-INFO* result)
	(aref *WINTERP-UNIX-NAME-INFO* 0) ;RETURN unix name STRING
	)
       (T				;command failed
	(setq *WINTERP-UNIX-NAME-INFO* NIL)
	NIL				;RETURN NIL indicating failure.
	)
       )
      )))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (UNIX:GET-UNAME-REV)
;; calls uname(1) to return a FIXNUM representing the revision number of
;; the Unix variant WINTERP is running under. For example,
;; if uname(1) returns "IRIX indeed 5.2 02282016 IP22 mips"
;; GET-UNIX-REVISION will return 5
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun UNIX:GET-UNAME-REV ()
  (if (arrayp *WINTERP-UNIX-NAME-INFO*)
      (aref *WINTERP-UNIX-NAME-INFO* 1)	;RETURN unix revision FIXNUM
    (error "UNIX:GET-UNAME not called, or returned invalid result" *WINTERP-UNIX-NAME-INFO*)
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (UNIX:GET-UNAME-SUBREV)
;; calls uname(1) to return a FIXNUM representing the sub-revision number
;; of the Unix variant WINTERP is running under. For example if uname(1)
;; returns "IRIX indeed 5.2 02282016 IP22 mips" GET-UNIX-REVISION will
;; return 2.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun UNIX:GET-UNAME-SUBREV ()
  (if (arrayp *WINTERP-UNIX-NAME-INFO*)	
      (aref *WINTERP-UNIX-NAME-INFO* 2)	;RETURN unix sub-revision FIXNUM
    (error "UNIX:GET-UNAME not called, or returned invalid result" *WINTERP-UNIX-NAME-INFO*)
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; GETENV: return the value of the environment variable string <envar-str>.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun getenv (envar-str)
  (read-exec-cmd (concatenate 'string "echo $" envar-str))
  )

;; Home directory used by some other applications in winterp/examples/*.lsp
(defvar *HOME-DIRECTORY-STR* (getenv "HOME"))

;; Sockets directory used by some other applications in winterp/examples/*.lsp
(defvar *SOCKETS-DIRECTORY-STR* (concatenate 'string *HOME-DIRECTORY-STR* "/.sockets"))

(defvar *WINTERP-RUNNING-STANDALONE* NIL)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; WINTERP-STANDALONE-P: returns non-NIL if WINTERP was started with
;; environment variable WINTERP_STANDALONE_APP set.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun winterp-standalone-p ()
  (if (null *WINTERP-RUNNING-STANDALONE*)
      (progn
	(if (string/=			;TRUE when envvar WINTERP_STANDALONE_APP is set
	     (read-exec-cmd "echo $WINTERP_STANDALONE_APP")
	     "")
	    (setq *WINTERP-RUNNING-STANDALONE* :stand-alone)
	  (setq *WINTERP-RUNNING-STANDALONE* :monolithic)
	  )
	))
  (eq *WINTERP-RUNNING-STANDALONE* :stand-alone) ;RETURN VALUE
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; DECIDE-WINTERP-TOP-LEVEL-SHELL: returns the APPLICATION_SHELL_WIDGET_CLASS
;; if WINTERP was started with; environment variable WINTERP_STANDALONE_APP
;; set. Returns the TOP_LEVEL_SHELL_WIDGET_CLASS if environment variable
;; WINTERP_STANDALONE_APP was not set. A standalone application with a top-level
;; shell instance of APPLICATION_SHELL_WIDGET_CLASS will exit (terminate
;; WINTERP) when it receives an f.close'd window-manager event from an
;; ICCCM-compliant window manager (e.g. mwm).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun decide-winterp-top-level-shell ()
  (if (winterp-standalone-p)		;TRUE when envvar WINTERP_STANDALONE_APP is set
      APPLICATION_SHELL_WIDGET_CLASS	;for stand-alone apps --> f.close kills WINTERP
    TOP_LEVEL_SHELL_WIDGET_CLASS)	;for monolithic apps --> f.close only destroys shell
  )


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun file:get-path (filepath-str)
  (do
   ((i (1- (length filepath-str)) (1- i))
    )
   ((or (< i 0) (char= (char filepath-str i) #\/))
    (if (< i 0)
        filepath-str
      (subseq filepath-str 0 i))
    )
   )
  )

(defun file:remove-path (filepath-str)
  (do
   ((i (1- (length filepath-str)) (1- i))
    )
   ((or (< i 0) (char= (char filepath-str i) #\/))
    (if (< i 0)
        filepath-str
      (subseq filepath-str (1+ i)))
    )
   )
  )

(defun file:remove-extension (filepath-str)
  (do
   ((i (1- (length filepath-str)) (1- i))
    )
   ((or (< i 0) (char= (char filepath-str i) #\.))
    (if (< i 0)
        filepath-str
      (subseq filepath-str 0 i))
    )
   )
  )

(defun file:get-extension (filepath-str)
  (do
   ((i (1- (length filepath-str)) (1- i))
    )
   ((or (< i 0) (char= (char filepath-str i) #\.))
    (if (< i 0)
        NIL
      (subseq filepath-str i nil))
    )
   )
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide "lib-utils/unixstuf")
