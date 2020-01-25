; -*-Lisp-*-
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; File:         wcls-gauge.lsp
; RCS:          $Header: /users/npm/src/winterp/examples/xtango/RCS/wcls-gauge.lsp,v 2.1 1994/06/06 15:01:47 npm Exp $
; Description:  GAUGE-WIDGET-CLASS -- use xtango to define the graphics of a
;		new motif widget class without having to use low-level xt
;		widget subclassing uglyness.
; Author:       Niels P. Mayer
; Created:      1993
; Modified:     Mon Jun  6 04:26:36 1994 (Niels Mayer) npm@indeed
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

(require "xtango/icls-plrar")		;define POLAR-ARROW-IMAGE-CLASS and method :SET-PHASE

;;;
;;; GAUGE-WIDGET-CLASS -- a subclass of XM_FRAME_WIDGET_CLASS
;;; containing a TANGO:WIDGET_CLASS with meter semantics
;;;
(setq GAUGE-WIDGET-CLASS		;name of the new subclass
      (send Class :new
	    '(				;new ivars for this subclass
	      ivar_needle_ti		;POLAR-LINE-IMAGE-CLASS -- subclass of TANGO:LINE_IMAGE_CLASS displaying line at a location of given size and magnitude
	      )				
	    '()                         ;no class variables for subclass
	    TANGO:WIDGET_CLASS		;name of the superclass
 	    )) 

;;;
;;; override TANGO:WIDGET_CLASS instance initializer (method :isnew)...
;;;
;;; (send GAUGE-WIDGET-CLASS [:managed] <widget_name> <widget_parent>
;;;       ...)
;;;
(let* (					;def initialization constants...
       (needle_origin #C(0.5 0.5))
       (needle_size   0.45)
       (bezel_size    (+ needle_size 0.005))
       (scale_size    (+ bezel_size 0.02))
;;     (fgcolor_str   "black")
;;     (bgcolor_str   "white")
       )

  (send GAUGE-WIDGET-CLASS :answer :ISNEW
	'(managed_k widget_name widget_parent
		    &rest args)
	'(
	  ;; create 'self', an instance of XM_FRAME_WIDGET_CLASS
	  (apply #'send-super :isnew	;call superclass's init to create widget
		 managed_k widget_name widget_parent
		 args
		 )

	  ;; Set up expose callback to draw the button-image once
	  ;; the window is created... Subsequent exposes will refresh
	  ;; the drawing.
	  (let ((init_p NIL))
	    (send-super :add_callback :XMN_EXPOSE_CALLBACK
		  '()
		  '(
		    (if init_p
			(send-super :refresh)
		      (progn
			(send-super :begin_drawing)

;;			(send-super :SET_BGCOLOR bgcolor_str)

			(send-super :SET_COORD ;shift the window coordinates so that only the top half of circle is visible...
			      0.0 (+ (imagpart needle_origin) 0.03)
			      1.0 0.0)

			;; create a small circle at the origin of the "dial needle"
			(send TANGO:CIRCLE_IMAGE_CLASS :new self
			      needle_origin ;location_coord
			      (/ bezel_size 2.5) ;radius_float
;;			      fgcolor_str ;tango_color
			      TANGO_COLOR_RED
			      1.0	;fill_float
			      )

			;; create a circle, the "bezel" onto which we put the scale lines
			(send TANGO:CIRCLE_IMAGE_CLASS :new self
			      needle_origin ;location_coord
			      bezel_size ;radius_float
;;			      fgcolor_str ;tango_color
                              TANGO_COLOR_BLACK
			      0.0	;fill_float
			      )

			;; draw 10 equally spaced scale lines around the "bezel"
			(do ((i 0 (1+ i)))
			    ((> i 10))
			    (let* ((x (* (/ i 10.0) pi))
				   (cis (exp (* #C(0.0 1.0) (- x)))) ;cis == cos(x) + i*sin(x) = e^(i*x)
				   (origin (+ (* bezel_size cis) needle_origin))
				   (endpoint (+ (* scale_size cis) needle_origin))
				   (size (- endpoint origin))
				   )
			      (send TANGO:LINE_IMAGE_CLASS :new self
				    origin ;location_coord
				    size ;size_coord
;;				    fgcolor_str ;tango_color
                                    TANGO_COLOR_BLACK
				    0.5 ;width_float
				    1.0 ;style_float
				    :no_arrow ;arrow_int
				    )
			      ))

			;; create the needle, make it accecible through instance var 'ivar_needle_ti'
			(setq ivar_needle_ti
			      (send POLAR-ARROW-IMAGE-CLASS :new self
				    #C(0.5 0.5)	;<location_coord>
				    needle_size ;<magnitude_float>
				    0.0 ;<phase_float>
;;				    fgcolor_str ;<tango_color>
                                    TANGO_COLOR_GREEN
				    1.0	;<fill_float>
				    ))

			(send self :set-value 0.0) ;intialize the label...

			;; make all the newly drawn images visible
			(send-super :refresh)
			(setq init_p t)
			)
		      )
		    ))
	    )

	  (send-super :add_event_handler BUTTON_PRESS_MASK
		'(EVHANDLER_XEVENT EVHANDLER_BUTTON)
		'(
		  (case EVHANDLER_BUTTON
			(1
			 (let ((pha
				(- (phase (- (send-super :get_event_coord EVHANDLER_XEVENT)
					     needle_origin)))
				))
			   (if (< pha 0.0)
			       (if (< pha (/ pi -2.0))
				   (setq pha pi)
				 (setq pha 0.0)))

			   (send ivar_needle_ti :set-phase pha)

			   )
			 ))
		  ))

	  (send-super :add_event_handler BUTTON1_MOTION_MASK
		'(EVHANDLER_XEVENT)
		'(
		  (let ((pha
			 (- (phase (- (send-super :get_event_coord EVHANDLER_XEVENT)
				      needle_origin)))
			 ))
		    (if (< pha 0.0)
			(if (< pha (/ pi -2.0))
			    (setq pha pi)
			  (setq pha 0.0)))

		    (send ivar_needle_ti :set-phase pha)
		    )
		  ))
	  ))
  )

;;;
;;; value ranges from 0.0 to +1.0.
;;;
(send GAUGE-WIDGET-CLASS :answer :SET-VALUE 
      '(value)
      '(
	(if (< value 0.0) (setq value 0.0))
	(if (> value 1.0) (setq value 1.0))
	(send ivar_needle_ti :set-phase (* pi (- 1.0 value)))
	))

;;;
;;; returns value between 0.0 and +1.0
;;;
(send GAUGE-WIDGET-CLASS :answer :GET-VALUE 
      '()
      '(
	(- 1.0 (/ (send ivar_needle_ti :get-phase) pi))
	))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide "xtango/wcls-gauge")
