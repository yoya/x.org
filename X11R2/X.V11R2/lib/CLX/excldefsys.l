;;; -*- Mode: common-lisp; Package: user; Base: 10; Lowercase: Yes -*-
;;;
;;;			 TEXAS INSTRUMENTS INCORPORATED
;;;				  P.O. BOX 2909
;;;			       AUSTIN, TEXAS 78769
;;;
;;; Copyright (C) 1987 Texas Instruments Incorporated.
;;;
;;; Permission is granted to any individual or institution to use, copy, modify,
;;; and distribute this software, provided that this complete copyright and
;;; permission notice is maintained, intact, in all copies and supporting
;;; documentation.
;;;
;;; Texas Instruments Incorporated provides this software "as is" without
;;; express or implied warranty.
;;;

;;; NOTE: This is beta code.  There are various known problems (e.g.,
;;; various cases of gcontext shadowing, bullet-proof abort handling),
;;; and various features (e.g., less restrictive locking) being thought
;;; about.  Bug reports should be addressed to
;;; bug-clx@zermatt.lcs.mit.edu.

;; Note: File mode-lines don't have:
;;    PACKAGE: (XLIB :USE (CL))
;;  because the TI Explorer doesn't fully support it.
;;  Define the XLIB package here instead.

#+ignore
(eval-when (compile) (proclaim '(optimize (speed 1) (safety 3))))

;;;
;;; NOTE: Enable the following if you will be using CLX and getting a SIGIO
;;;       signal when there is input on the X socket.  Remember to recompile
;;;       when you switch the setting.

#+delete_me_if_you_want_sigio_blocked
(eval-when (load compile)
  (setq *features* (union *features* '(:clx-blocksigio))))


(in-package :user)

(require :foreign)
(require :mdproc)
(require :process)

(eval-when (load)
  (require :clxexcldep "excldep")
  (provide :clx))


;;
;; The following is a suggestion.  If you turn this off be prepared for
;; possible deadlock, since no interrupts will be recognized while
;; reading from the X socket.
;;
(setq compiler::generate-interrupt-checks-switch
  (compile nil '(lambda (safety size speed)
		 (declare (ignore safety size speed)) t)))


;;
;; Now the stuff that really belongs here
;;
(in-package 'xlib :use '(foreign-functions lisp))

#+allegro
(excl:defsystem clx
;;              (:default-pathname "/usr/src/local/X11/lib/CLX/")
                ()
  (|depdefs|)
  (|clx| :load-before-compile |depdefs|
	 :funcall-after sys:gsgc-step-generation
	 :recompile-on (|depdefs|))
  (|dependent| :load-before-compile |clx|
	       :funcall-after sys:gsgc-step-generation
	       :recompile-on (|clx|))
  (|macros| :load-before-compile |dependent|
	    :funcall-after sys:gsgc-step-generation
	    :compile-satisfies-load t
	    :recompile-on (|dependent|))
  (|bufmac| :load-before-compile |macros|
	    :funcall-after sys:gsgc-step-generation
	    :compile-satisfies-load t
	    :recompile-on (|macros|))
  (|buffer| :load-before-compile |bufmac|
	    :funcall-after sys:gsgc-step-generation
	    :recompile-on (|bufmac|))
  (|display| :load-before-compile |buffer|
	     :funcall-after sys:gsgc-step-generation
	     :recompile-on (|buffer|))
  (|gcontext| :load-before-compile |display|
	      :funcall-after sys:gsgc-step-generation
	      :recompile-on (|display|))
  (|requests| :load-before-compile |display|
	      :funcall-after sys:gsgc-step-generation
	      :recompile-on (|display|))
  (|input| :load-before-compile |display|
	   :funcall-after sys:gsgc-step-generation
	   :recompile-on (|display|))
  (|fonts| :load-before-compile |display|
	   :funcall-after sys:gsgc-step-generation
	   :recompile-on (|display|))
  (|graphics| :load-before-compile |fonts|
	      :funcall-after sys:gsgc-step-generation
	      :recompile-on (|fonts|))
  (|text| :load-before-compile (|gcontext| |fonts|)
	  :funcall-after sys:gsgc-step-generation
	  :recompile-on (|gcontext| |fonts|)
	  :load-after (|translate|))
  ;; This above line gets around a compiler macro expansion bug.
  (|attributes| :load-before-compile |display|
		:funcall-after sys:gsgc-step-generation
		:recompile-on (|display|))
  (|translate| :load-before-compile |text|
	     :funcall-after sys:gsgc-step-generation
	     :recompile-on (|display|))
  (|keysyms| :load-before-compile |translate|
	     :funcall-after sys:gsgc-step-generation
	     :recompile-on (|translate|))
  (|manager| :load-before-compile |display|
	     :funcall-after sys:gsgc-step-generation
	     :recompile-on (|display|))
  (|image| :load-before-compile |display|
	   :funcall-after sys:gsgc-step-generation
	   :recompile-on (|display|))
  )


#-lispm
(defun compile-clx (&optional pathname-defaults)
  (let ((*default-pathname-defaults*
	  (or pathname-defaults *default-pathname-defaults*)))
    (declare (special *default-pathname-defaults*))
    (compile-file "depdefs")
    (load "depdefs")
    (compile-file "clx")
    (load "clx")
    (compile-file "dependent")
    (load "dependent")
    (compile-file "macros")
    (load "macros")
    (compile-file "bufmac")
    (load "bufmac")
    (compile-file "buffer")
    (load "buffer")
    (compile-file "display")
    (load "display")
    (compile-file "gcontext")
    (load "gcontext")
    (compile-file "requests")
    (load "requests")
    (compile-file "input")
    (load "input")
    (compile-file "fonts")
    (load "fonts")
    (compile-file "graphics")
    (load "graphics")
    (compile-file "text")
    (load "text")
    (compile-file "attributes")
    (load "attributes")
    (load "translate")
    (compile-file "translate")			; work-around bug in 2.0 and 2.2
    (load "translate")
    (compile-file "keysyms")
    (load "keysyms")
    (compile-file "manager")
    (load "manager")
    (compile-file "image")
    (load "image")
    ))

#-lispm
(defun load-clx (&optional pathname-defaults (macros-p t))
  (let ((*default-pathname-defaults*
	  (or pathname-defaults *default-pathname-defaults*)))
    (declare (special *default-pathname-defaults*))
    (load "depdefs")
    (load "clx")
    (load "dependent")
    (when macros-p
      (load "macros")
      (load "bufmac"))
    (load "buffer")
    (load "display")
    (load "gcontext")
    (load "requests")
    (load "input")
    (load "fonts")
    (load "graphics")
    (load "text")
    (load "attributes")
    (load "translate")
    (load "keysyms")
    (load "manager")
    (load "image")
    ))


(ff:remove-entry-point (ff:convert-to-lang "connect_to_server" :language :c))
(ff:remove-entry-point (ff:convert-to-lang "c_check_bytes" :language :c))
(ff:remove-entry-point (ff:convert-to-lang "c_read_bytes" :language :c))
(ff:remove-entry-point (ff:convert-to-lang "c_read_bytes_interruptible"
					   :language :c))
(ff:remove-entry-point (ff:convert-to-lang "c_write_bytes" :language :c))
(ff:remove-entry-point (ff:convert-to-lang "c_flush_bytes" :language :c))
(load "socket.o")
(load "excldep.o")

(ff:defforeign-list `((xlib::connect-to-server
		       :entry-point
		       ,(ff:convert-to-lang "connect_to_server" :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (string fixnum))
		      (xlib::c-check-bytes
		       :entry-point
		       ,(ff:convert-to-lang "c_check_bytes" :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (fixnum fixnum))
		      (xlib::c-read-bytes
		       :entry-point
		       ,(ff:convert-to-lang "c_read_bytes" :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (fixnum (simple-array (unsigned-byte 8))
				   fixnum fixnum))
		      (xlib::c-read-bytes-interruptible
		       :entry-point
		       ,(ff:convert-to-lang "c_read_bytes_interruptible"
			 :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (fixnum (simple-array (unsigned-byte 8))
				   fixnum fixnum))
		      (xlib::c-write-bytes
		       :entry-point
		       ,(ff:convert-to-lang "c_write_bytes" :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (fixnum (simple-array (unsigned-byte 8))
				   fixnum fixnum))
		      (xlib::c-flush-bytes
		       :entry-point
		       ,(ff:convert-to-lang "c_flush_bytes" :language :c)
		       :return-type :fixnum
		       :arg-checking nil
		       :arguments (fixnum))
		      #+clx-blocksigio
		      (xlib::sigblock
		       :entry-point
		       ,(ff:convert-to-lang "sigblock" :language :c)
		       :return-type :integer
		       :arg-checking nil
		       :arguments (integer))
		      #+clx-blocksigio
		      (xlib::sigsetmask
		       :entry-point
		       ,(ff:convert-to-lang "sigsetmask" :language :c)
		       :return-type :integer
		       :arg-checking nil
		       :arguments (integer))
		      ))
