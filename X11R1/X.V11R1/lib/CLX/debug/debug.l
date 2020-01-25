;;; -*- Mode:Lisp; Package:XLIB; Syntax:COMMON-LISP; Base:10; Lowercase:YES; Patch-file:T -*-
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

;; XLIB Test code

;;
;; Debug Patches for collecting input and output to the server
;;

; From file BUFFER.LISP

(defvar *history* nil)				;debug******************
(defvar *input* nil)				;debug******************
(defparameter *debug* t)			;debug******************

(defun buffer-write (vector buffer start end)
  ;; Write out VECTOR from START to END into BUFFER
  ;; Internal function, MUST BE CALLED FROM WITHIN WITH-BUFFER
  (declare (type buffer buffer)
	   (type integer start end))
  (when (buffer-dead buffer)
    (x-error 'closed-display :display buffer))
  (when *debug*					; debug**************************
    (let ((hist (make-reply-buffer (index- end start))))
      (replace (the buffer-bytes (reply-ibuf8 hist)) (the buffer-bytes vector)
	       :start2 start :end2 end)
      (push hist *history*)))			; end debug

  (wrap-buf-output buffer
    (funcall (buffer-write-function buffer) vector buffer start end))
  nil)

(defun buffer-flush (buffer)
  ;; Write the buffer contents to the server stream - doesn't force-output the stream
  ;; Internal function, MUST BE CALLED FROM WITHIN WITH-BUFFER
  (declare (type buffer buffer))
  (let ((boffset (buffer-boffset buffer)))
    (declare (type array-index boffset))
    (when (index-plusp boffset)
      (buffer-write (buffer-obuf8 buffer) buffer 0 boffset)
	; *DEBUG*****************
      (setf (buffer-boffset buffer) 0)
      (setf (buffer-last-request buffer) nil)))
  nil)

(defun buffer-input  (buffer vector start end &optional timeout)
  ;; Read into VECTOR from the buffer stream
  ;; Timeout, when non-nil, is in seconds
  ;; Returns non-nil if EOF encountered
  ;; Returns :TIMEOUT when timeout exceeded
  (declare (type buffer buffer)
	   (type vector vector)
	   (type array-index start end)
	   (type (or null number) timeout)
	   (values eof-p))
  (when (buffer-dead buffer)
    (x-error 'closed-display :display buffer))
  (unless (= start end)
    (prog1 ;; ****** DEBUG
      (funcall (buffer-input-function buffer) buffer vector start end timeout)
      
      (when *debug*				; debug**************************
	(let* ((hist (make-reply-buffer (index- end start))))
	  (replace (the buffer-bytes (reply-ibuf8 hist)) (the buffer-bytes vector)
		   :start2 start :end2 end)
	  (push hist *input*)))		; end debug
      )))


;;-----------------------------------------------------------------------------

(defun buffer-string (reply-buffer)
  (make-array (reply-size reply-buffer) :element-type 'string-char :displaced-to (reply-ibuf8 reply-buffer)))

(defun display-listen (display)
  (listen (display-input-stream display)))

(defun readflush (display)
  ;; Flushes Display's input stream, returning what was there
  (let ((stream (display-input-stream display)))
    (loop while (listen stream) collect (read-byte stream))))

;;-----------------------------------------------------------------------------

(defun show-history (&optional (buffer (car *history*)))
  ;; Display an output history
  (reading-event (buffer :sizes (8 16))
    (do* ((i 0 (+ i length))
	  (size (length buffer-bbuf))
	  request
	  length)
	 ((>= i size)
	  (unless (= i size)
	    (format t "~%Buffer size ~d  Requests end at ~d" size i)))
      (setq request (read-card8 i))
      (setq length (* 4 (read-card16 (+ i 2))))
      (format t "~%~3d ~a (~d) length ~d"
	      i (request-name request) request length))))

(defun show-histories (&optional n (hist *history*))
  ;; Display the last N output histories
  (dolist (h (reverse (if n (butlast hist (max 0 (- (length hist) n))) hist)))
    (show-history h)))

(defun clear-all-history ()
  (setq *history* nil)
  (setq *input* nil))

(defun show-buffer (display)
  ;; Show whats in the output buffer
  (with-buffer-output (display :length :none :sizes (8 16))
    (do* ((i 0 (+ i length))
	  request
	  length)
	 ((>= i buffer-boffset)
	  (unless (= i buffer-boffset)
	    (format t "~%Buffer size ~d  Requests end at ~d" buffer-boffset i)))
      (setq request (read-card8 i))
      (setq length (* 4 (read-card16 (+ i 2))))
      (when (zerop length)
	(format t "~%Zero length in buffer")
	(return nil))
      (format t "~%~3d ~a (~d) length ~d"
	      i (request-name request) request length))))

;;-----------------------------------------------------------------------------
;; The following are useful display-after functions

(defparameter *requests-used* (make-array (length *request-names*) :initial-value 0))

(defun which-used (display)
  (let* ((last-request (or (display-last-request display) 0))
	 (request (aref (buffer-obuf8 display) last-request)))
    (incf (aref *requests-used* request))))

(defun not-used (&aux result)
  (dotimes (i (length *requests-used*))
    (when (zerop (aref *requests-used* i))
      (push (aref *request-names* i) result)))
  (nreverse result))

(defun check-buffer (display)
  ;; Ensure the output buffer in display is correct
  (with-buffer-output (display :length :none :sizes (8 16))
    (do* ((i 0 (+ i length))
	  request
	  length)
	 ((>= i buffer-boffset)
	  (unless (= i buffer-boffset)
	    (si:fsignal "Buffer size ~d  Requests end at ~d" buffer-boffset i)))
      (setq request (read-card8 i))
      (setq length (* 4 (read-card16 (+ i 2))))
      (when (zerop request)
	(si:fsignal "Zero request in buffer")
	(return nil))
      (when (zerop length)
	(si:fsignal "Zero length in buffer")
	(return nil)))))

(defun check-finish (display)
  (check-buffer display)
  (display-finish-output display))

(defun check-force (display)
  (check-buffer display)
  (display-force-output display))

(defun clear-next (display)
  ;; Never append requests
  (setf (display-last-request display) nil))

(defvar *request-number* 0)
(defun clear-n (display)
  ;; Never append more than 40 requests together
  ;; This was necessary to get around bugs in the alpha server
  (check-buffer display)
  (when (zerop (mod (incf *request-number*) 40))
    (setf (display-last-request display) nil)))
