; -*-Lisp-*-
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; File:         prov-req.lsp
; RCS:          $Header: prov-req.lsp,v 1.2 91/10/05 18:46:00 mayer Exp $
; Description:  Pseudo version of common lisp's provide/require functionality
;		Note that this uses the X11r4 routine XT_RESOLVE_PATHNAME
;		XtResolvePathname(), therefore you can only use this w/
;		Motif 1.1.
; Author:       Eric Blossom, HP Response Center Lab; Niels Mayer, HP Labs.
; Created:      Mon Feb 12 19:05:25 1990
; Modified:     Sat Oct  5 18:45:39 1991 (Niels Mayer) mayer@hplnpm
; Language:     Lisp
; Package:      N/A
; Status:       X11r5 contrib tape release
;
; WINTERP Copyright 1989, 1990, 1991 Hewlett-Packard Company (by Niels Mayer).
; XLISP version 2.1, Copyright (c) 1989, by David Betz.
;
; Permission to use, copy, modify, distribute, and sell this software and its
; documentation for any purpose is hereby granted without fee, provided that
; the above copyright notice appear in all copies and that both that
; copyright notice and this permission notice appear in supporting
; documentation, and that the name of Hewlett-Packard and Niels Mayer not be
; used in advertising or publicity pertaining to distribution of the software
; without specific, written prior permission.  Hewlett-Packard and Niels Mayer
; makes no representations about the suitability of this software for any
; purpose.  It is provided "as is" without express or implied warranty.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; I use prov-req.lsp to set up a load-path for WINTERP.
;;;
;;; Put 
;;; (PROVIDE <module-name>) 
;;; at the end of each file/module/class/<program-unit>
;;; PROVIDE indicates that a file <module-name> has been
;;; loaded. Once a file is LOADed and PROVIDE'd,
;;; (REQUIRE <module-name>) will not re-LOAD
;;; <module-name> again. <module-name> can be a string
;;; or a symbol.
;;;
;;; (REQUIRE <module-name> [<path>])
;;; will load <module-name> if it hasn't been PROVIDE'd
;;; already. <module-name> is a string or a symbol. If
;;; <path> isn't provided, *default-load-path*=="%N%S:%N"
;;; is used in it's place. See documentation for
;;; XtResolvePathname() for an explanation of load-paths.
;;; If <module-name>=="file", then 
;;; by default, the default load path will load
;;; "file.lsp" out of the directory specified by resource
;;; "Winterp.lispLibDir" or by the command line arg -lisp_lib_dir.
;;; ... 
;;;
;;; I do the following in my winterp startup file ~/.winterp,
;;; which I specify thru Xdefault "Winterp.lispInitFile: /users/mayer/.winterp"
;;;
;;; (setq *default-load-path*
;;;       (strcat
;;;        "/users/mayer/%N%S:"
;;;        "/users/mayer/%N:"
;;;        "/users/mayer/src/widgit/examples/%N%S:"
;;;        "/users/mayer/src/widgit/examples/%N:"
;;;        "/tmp/%N%S:"
;;;        "/tmp/%N"
;;;        ))
;;;
;;; (load "/users/mayer/src/widgit/examples/prov-req.lsp")

(defvar *default-load-path* "%N%S:%N")	;load-path, see XtResolvePathname()
(defvar *modules-size* 20)		;size of hashtable.
(setq *modules* (make-array *modules-size*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun provide (module-name)
  (let ((file-str (cond ((symbolp module-name)
			 (symbol-name module-name))
			((stringp module-name)
			 module-name)
			(t
			 (error "Wrong type argument to PROVIDE" module-name)))
		  ))
    
    (let ((hashbucket (aref *modules* (hash file-str *modules-size*))))
      (if (member file-str (aref *modules* (hash file-str *modules-size*))
		  :test #'equal)
	  t
	(setf (aref *modules* (hash file-str *modules-size*))
	      (cons file-str hashbucket))
	))
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun require (module-name &optional pathname)
  (let ((file-str (cond ((symbolp module-name)
			 (symbol-name module-name))
			((stringp module-name)
			 module-name)
			(t
			 (error "Wrong type argument to REQUIRE" module-name)))))

    (if (member file-str (aref *modules* (hash file-str *modules-size*))
		:test #'equal)
	t		
      (let (
	    (filepath-str (XT_RESOLVE_PATHNAME nil ;ignore all %T substitutions
					       file-str	;substitute for %N
					       ".lsp" ;substitute for %S
					       (if pathname pathname *default-load-path*))))
	(if (null filepath-str)
	    (error (format nil "Can't find module ~A in path ~A" file-str (if pathname pathname *default-load-path*))))

	(if (not (load filepath-str :verbose T :print NIL))
	    (error "Can't load required module" filepath-str)
	  t
	  ))
      )))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide "prov-req")
