; -*-Lisp-*-
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; File:         .winterp
; RCS:          $Header: $
; Description:  WINTERP INITIALIZATION FILE
; Author:       Niels Mayer, HPLabs
; Created:      Mon Nov 20 18:13:23 1989
; Modified:     Sat Nov 25 07:17:38 1989 (Niels Mayer) mayer@hplnpm
; Language:     Lisp
; Package:      N/A
; Status:       Experimental (Do Not Distribute)
;
; (c) Copyright 1989, Hewlett-Packard Company, all rights reserved.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; initialization file for XLISP 2.0 & WINTERP

; define some macros
(defmacro defvar (sym &optional val)
  `(if (boundp ',sym) ,sym (setq ,sym ,val)))
(defmacro defparameter (sym val)
  `(setq ,sym ,val))
(defmacro defconstant (sym val)
  `(setq ,sym ,val))

; (makunbound sym) - make a symbol value be unbound
(defun makunbound (sym) (setf (symbol-value sym) '*unbound*) sym)

; (fmakunbound sym) - make a symbol function be unbound
(defun fmakunbound (sym) (setf (symbol-function sym) '*unbound*) sym)

; (mapcan fun list [ list ]...)
(defmacro mapcan (&rest args) `(apply #'nconc (mapcar ,@args)))

; (mapcon fun list [ list ]...)
(defmacro mapcon (&rest args) `(apply #'nconc (maplist ,@args)))

; (set-macro-character ch fun [ tflag ])
(defun set-macro-character (ch fun &optional tflag)
    (setf (aref *readtable* (char-int ch))
          (cons (if tflag :tmacro :nmacro) fun))
    t)

; (get-macro-character ch)
(defun get-macro-character (ch)
  (if (consp (aref *readtable* (char-int ch)))
    (cdr (aref *readtable* (char-int ch)))
    nil))

; (savefun fun) - save a function definition to a file
(defmacro savefun (fun)
  `(let* ((fname (strcat (symbol-name ',fun) ".lsp"))
          (fval (get-lambda-expression (symbol-function ',fun)))
          (fp (open fname :direction :output)))
     (cond (fp (print (cons (if (eq (car fval) 'lambda)
                                'defun
                                'defmacro)
                            (cons ',fun (cdr fval))) fp)
               (close fp)
               fname)
           (t nil))))

; (debug) - enable debug breaks
(defun debug ()
       (setq *breakenable* t))

; (nodebug) - disable debug breaks
(defun nodebug ()
       (setq *breakenable* nil))

(setq *breakenable* nil)		; T allows entry into breakloop
(setq *tracenable* t)			; set this to T if you want to see a
					; backtrace on error.
(setq *gc-flag* t)			; we want to see garbage collection messages

(setq *lisp-lib-dir*
      (let*
	  ((pipe (popen "/bin/echo $HOME" "r"))
	   (home (read-line pipe))
	   )
	(pclose pipe)
	(strcat home "/src/widgit/examples/")
	)
      )
      
(if (not (load (strcat *lisp-lib-dir* "rc-shell.lsp")))
    (error (strcat "Couldn't load " *lisp-lib-dir* "rc-shell.lsp"))
  )
