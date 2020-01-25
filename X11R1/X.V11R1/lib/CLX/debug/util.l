;;; -*- Mode:Lisp; Package:XLIB; Syntax:COMMON-LISP; Base:10; Lowercase:YES; -*-
;;;===============================================================================
;;;                                    RESTRICTED RIGHTS LEGEND 
;;; Use,  duplication, or  disclosure  by  the  Government is subject to restrictions
;;; as set forth in subdivision (b)(3)(ii) of the Rights in Technical Data and
;;; Computer Software clause at 52.227-7013. 
;;;
;;; TEXAS INSTRUMENTS INCORPORATED, P.O. BOX 2909 AUSTIN, TEXAS 78769  
;;; Copyright (C) 1986, Texas Instruments Incorporated. All rights reserved.
;;;===============================================================================

;;; Created 04/09/87 14:30:41 by LaMott G. OREN

;; XLIB Test utilities

#+ti
(defun make-sun-host (host)
  ;; On The sun, hosts are the IP address
  (let ((addr (ip:get-ip-address host))
	(result (make-string 4)))
    (setf (char result 0) (int-char (ldb (byte 8 24) addr)))
    (setf (char result 1) (int-char (ldb (byte 8 16) addr)))
    (setf (char result 2) (int-char (ldb (byte 8 8) addr)))
    (setf (char result 3) (int-char (ldb (byte 8 0) addr)))
    (list result 2)))

#+ti
(defun get-sun-host (host)
  (let* ((string (first host))
	 (address (dpb (char-int (char string 0)) (byte 8 24)
		       (dpb (char-int (char string 1)) (byte 8 16)
			    (dpb (char-int (char string 2)) (byte 8 8)
				 (char-int (char string 3))))))
	 (host-object (ip:ip-get-host-from-address address)))
    (and host-object (funcall host-object :name))))

(defun display-root (display) (screen-root (display-default-screen display)))
(defun display-black (display) (screen-black-pixel (display-default-screen display)))
(defun display-white (display) (screen-white-pixel (display-default-screen display)))

(defun report-events (display)
  (process-event display :handler #'(lambda (&rest args) (print args)) :discard-p t))

(defun describe-window (window)
  (macrolet ((da (attribute &key (transform 'progn) (format "~s"))
	       (let ((func (intern (concatenate 'string "WINDOW-" (string attribute)) 'xlib)))
		 `(format t "~%~22a ~?" ',attribute ,format (list (,transform (,func window))))))
	     (dg (attribute &key (transform 'progn) (format "~s"))
	       (let ((func (intern (concatenate 'string "DRAWABLE-" (string attribute)) 'xlib)))
		 `(format t "~%~22a ~?" ',attribute ,format (list (,transform (,func window)))))))
    (with-state (window)
      (when (window-p window)
	(da visual :format "#x~x")
	(da class)
	(da gravity)
	(da bit-gravity)
	(da backing-store)
	(da backing-planes :format "#x~x")
	(da backing-pixel)
	(da save-under)
	(da colormap)
	(da colormap-installed-p)
	(da map-state)
	(da all-event-masks :transform make-event-keys :format "~~{~<~%~1:;~s ~>~}~")
	(da event-mask :transform make-event-keys :format "~~{~<~%~1:;~s ~>~}~")
	(da do-not-propagate-mask :transform make-event-keys :format "~~{~<~%~1:;~s ~>~}~")
	(da override-redirect)
	)
      (dg root)
      (dg depth)
      (dg x)
      (dg y)
      (dg width)
      (dg height)
      (dg border-width)

      )))
      
(defun describe-gc (gc)
  (macrolet ((dgc (name &key (transform 'progn) (format "~s"))
	       (let ((func (intern (concatenate 'string "GCONTEXT-" (string name)) 'xlib)))
		 `(format t "~%~22a ~?" ',name ,format (list (,transform (,func gc)))))))
    (dgc function)
    (dgc plane-mask)
    (dgc foreground)
    (dgc background)
    (dgc line-width)
    (dgc line-style)
    (dgc cap-style)
    (dgc join-style)
    (dgc fill-style)
    (dgc fill-rule)
    (dgc tile)
    (dgc stipple)
    (dgc ts-x)
    (dgc ts-y)
    (dgc font) ;; See below
    (dgc subwindow-mode)
    (dgc exposures)
    (dgc clip-x)
    (dgc clip-y)
    (dgc clip-ordering)
    (dgc clip-mask)
    (dgc dash-offset)
    (dgc dashes)
    (dgc arc-mode)
    ))

(defun degree (degrees)
  (* degrees (/ pi 180)))

(defun radian (radians)
  (round (* radians (/ 180 pi))))

(defun window-tree (window &optional (depth 0))
  ;; Print the window tree from WINDOW
  (format t "~%~v@t~s" depth window)
  (dolist (w (query-tree window))
    (window-tree w (+ depth 2))))


;--------------------------------------------------------------------------------
;;; Graying stuff

(defun make-gray (width &rest patterns)
  "Create a byte array containg pattern"
  (let* ((height (length patterns))
	 (nbits (* height width))
	 (result (make-array (ceiling nbits 8) :element-type '(unsigned-byte 8)))
	 (bits (make-array nbits :displaced-to result :element-type 'bit))
	 (n -1))
    (dolist (p patterns)
      (do ((b p (ash b -1))
	   (i width (1- i)))
	  ((zerop i))
	(setf (aref bits (+ i n)) (logand b 1)))
      (incf n width))
    result))

;;; Some common grays
;;; These are used to draw gray onto the screen. They are
;;; initialized in this strange manner to make them work
;;; properly with BITBLT -- the width must be a multiple of 32.
;;; The way to think of these grays is as a 2 dimensional array
;;; with enough bits on to corresponds to the proper gray level.
;;; When the pattern is copied onto the screen, it should be
;;; such that the underlying gray pattern is not easy to see
;;; (i.e. no streaking and no splotches).  All of the gray
;;; patterns are shown below to help visualize how they are
;;; implemented.  See the book "Fundamentals of Interactive
;;; Computer Graphics" by Foley and Van Dam, pages 599 and 600,
;;; published by Addison-Wesley, for more discussion in this
;;; area.
;;;
;;;         88%              75%              66%              50%
;;;    .-----------.    .-----------.     .--------.         .-----.
;;;    |  |XX|XX|XX|    |  |XX|XX|XX|     |  |XX|XX|         |  |XX|
;;;    |--+--+--+--|    |--+--+--+--|     |--+--+--|         |--+--|    
;;;    |XX|XX|XX|XX|    |XX|XX|  |XX|     |XX|  |XX|         |XX|  |    
;;;    |--+--+--+--|    |--+--+--+--|     |--+--+--|         `-----'
;;;    |XX|XX|  |XX|    |XX|  |XX|XX|     |XX|XX|  |
;;;    |--+--+--+--|    |--+--+--+--|     `--------'
;;;    |XX|XX|XX|XX|    |XX|XX|XX|  |
;;;    `-----------'    `-----------'
;;;
;;;         33%              25%              12%
;;;     .--------.      .-----------.    .-----------.
;;;     |XX|  |  |      |XX|  |  |  |    |XX|  |  |  |
;;;     |--+--+--|      |--+--+--+--|    |--+--+--+--|
;;;     |  |XX|  |      |  |  |XX|  |    |  |  |  |  |
;;;     |--+--+--|      |--+--+--+--|    |--+--+--+--|
;;;     |  |  |XX|      |  |XX|  |  |    |  |  |XX|  |
;;;     `--------'      |--+--+--+--|    |--+--+--+--|
;;;                     |  |  |  |XX|    |  |  |  |  |
;;;                     `-----------'    `-----------'

(defvar 88%-gray (make-gray 4 4 #b0111 #b1111 #b1101 #b1111) "Gray pattern")
(defvar 75%-gray (make-gray 4 4 #b0111 #b1101 #b1011 #b1110) "Gray pattern")
(defvar 66%-gray (make-gray 3 3 #b011 #b101 #b110          ) "Gray pattern")
(defvar 50%-gray (make-gray 2 2 #b01 #b10                  ) "Gray pattern")
(defvar 33%-gray (make-gray 3 3 #b100 #b010 #b001          ) "Gray pattern")
(defvar 25%-gray (make-gray 4 4 #b1000 #b0010 #b0100 #b0001) "Gray pattern")
(defvar 12%-gray (make-gray 4 4 #b1000 #b0000 #b0010 #b0000) "Gray pattern")

