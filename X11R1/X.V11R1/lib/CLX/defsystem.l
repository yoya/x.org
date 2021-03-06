;;; -*- Mode: LISP; Syntax: Common-lisp; Package: USER; Base: 10; Lowercase: Yes -*-

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

(unless (find-package "XLIB")
  (make-package "XLIB" :use '("LISP")))

#+(and lispm (not symbolics))
(defsystem CLX
  (:pathname-default "clx:clx;")
  (:patchable "clx:patch;" clx-ti)
  (:initial-status :experimental)

  (:module depdefs "depdefs")
  (:module clx "clx")
  (:module dependent "dependent")
  (:module macros "macros")
  (:module bufmac "bufmac")
  (:module buffer "buffer")
  (:module display "display")
  (:module gcontext "gcontext")
  (:module requests "requests")
  (:module input "input")
  (:module fonts "fonts")
  (:module graphics "graphics")
  (:module text "text")
  (:module attributes "attributes")
  (:module doc "doc")

  (:compile-load depdefs)
  (:compile-load clx
   (:fasload depdefs))
  (:compile-load dependent
   (:fasload depdefs clx))
  ;; Macros only needed for compilation
  (:skip :compile-load macros
   (:fasload depdefs clx dependent))
  ;; Bufmac only needed for compilation
  (:skip :compile-load bufmac
   (:fasload depdefs clx dependent macros))
  (:compile-load buffer
   (:fasload depdefs clx dependent macros bufmac))
  (:compile-load display
   (:fasload depdefs clx dependent macros bufmac buffer))
  (:compile-load gcontext
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load requests
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load input
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load fonts
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load graphics
   (:fasload depdefs clx dependent macros fonts bufmac buffer display fonts))
  (:compile-load text
   (:fasload depdefs clx dependent macros fonts bufmac buffer display gcontext fonts))
  (:compile-load attributes
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:auxiliary doc)
  )


#+symbolics
(defsystem CLX
    (:default-pathname "CLX:CLX;"
     :default-package "XLIB"
     :pretty-name "CLX"
     :distribute-binaries t
     :bug-reports ("CLX" "Report problems with CLX.")
     :initial-status :experimental)
  (:module doc ("doc")
	   (:type :lisp-example))
  (:module depdefs ("depdefs"))
  (:module clx ("clx")
	   (:uses-definitions-from depdefs))
  (:module dependent ("dependent")
	   (:uses-definitions-from clx))
  (:module macros ("macros")
	   (:root-module nil)
	   (:uses-definitions-from dependent))
  (:module bufmac ("bufmac")
	   (:root-module nil)
	   (:uses-definitions-from macros))
  (:module buffer ("buffer")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac)))
  (:module display ("display")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac)))
  (:module gcontext ("gcontext")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from display))
  (:module requests ("requests")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from display))
  (:module input ("input")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from display))
  (:module fonts ("fonts")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from display))
  (:module graphics ("graphics")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from fonts))
  (:module text ("text")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from gcontext fonts))
  (:module attributes ("attributes")
	   (:in-order-to :compile (:load macros))
	   (:in-order-to :compile (:load bufmac))
	   (:uses-definitions-from display))
  )

#+lucid
(defvar *foreign-libraries* '("-lc")) ; '("-lresolv" "-lc") for some sites

#+lucid
(defun clx-foreign-files ()
  (load "make-sequence-patch")
  (define-c-function (xlib::connect-to-server "_connect_to_server")
		     (host display)
		     :result-type :integer)
  (unintern 'display)
  (load-foreign-files '("socket.o") *foreign-libraries*))

#-lispm
(defun compile-clx (&optional pathname-defaults)
  (let ((*default-pathname-defaults*
	  (or pathname-defaults *default-pathname-defaults*)))
    (declare (special *default-pathname-defaults*))
    #+lucid
    (clx-foreign-files)
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
    (load "attributes")))

#-lispm
(defun load-clx (&optional pathname-defaults macros-p)
  (let ((*default-pathname-defaults*
	  (or pathname-defaults *default-pathname-defaults*)))
    (declare (special *default-pathname-defaults*))
    #+lucid
    (clx-foreign-files)
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
    (load "attributes")))
