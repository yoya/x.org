;;; -*- Mode: LISP; Syntax: Common-lisp; Package: XLIB; Base: 10; Lowercase: Yes -*-

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

;; Trace works by substituting trace functions for the display-write/input functions.
;; The trace functions maintain a database of requests sent to the server in the
;; trace-history display property.  This is an alist of (id . byte-vector) where
;; id is the request number for writes, :reply for replies, :event for events and
;; :error for errors.  The alist is kept in reverse order (most recent first)

;;; Created 09/14/87 by LaMott G. OREN

(in-package "XLIB" :use '("LISP"))

(EXPORT '(trace-display
	   untrace-display
	   display-trace
	   describe-request
	   describe-event
	   describe-reply
	   describe-error
	   describe-trace))

(DEFUN trace-display (display)
  "Start a trace on DISPLAY.
 If display is already being traced, this discards previous history.
 See display-trace and describe-trace."  
  (DECLARE (type display display))
  (UNLESS (GETF (display-plist display) 'write-function)
    (LET ((write-function (display-write-function display))
	  (input-function (display-input-function display)))
      ;; Save origional write/input functions so we can untrace
      (SETF (GETF (display-plist display) 'write-function) write-function)
      (SETF (GETF (display-plist display) 'input-function) input-function)
      ;; Set new write/input functions that will record what's sent to the server
      (SETF (display-write-function display)
	    #'(lambda (vector display start end)
		(trace-write-hook vector display start end)
		(FUNCALL write-function vector display start end)))
      (SETF (display-input-function display)
	    #'(lambda (display vector start end timeout)
		(LET ((result (FUNCALL input-function display vector start end timeout)))
		  (UNLESS result
		    (trace-read-hook display vector start end))
		  result)))))
  (SETF (display-trace-history display) nil)
  t)

(DEFUN untrace-display (display)
  "Stop tracing DISPLAY."
  (DECLARE (type display display))
  (LET ((write-function (GETF (display-plist display) 'write-function))
	(input-function (GETF (display-plist display) 'input-function)))
    (WHEN write-function (SETF (display-write-function display) write-function))
    (WHEN input-function (SETF (display-input-function display) input-function))
    (remF (display-plist display) 'write-function)
    (remF (display-plist display) 'input-function)
    (SETF (display-trace-history display) nil)))


(DEFUN byte-ref16 (VECTOR index)
  #+clx-little-endian
   (logior (the card16
		(ash (the card8 (aref vector (index+ index 1))) 8))
	   (the card8
		(aref vector index)))
  #-clx-little-endian
   (logior (the card16
		(ash (the card8 (aref vector index))) 8)
	   (the card8
		(aref vector (index+ index 1)))))

(defun byte-ref32 (a i)
  (declare (type buffer-bytes a)
	   (type array-index i))
  (declare-values card32)
  (declare-buffun)
  #+clx-little-endian
  (the card32
       (logior (the card32
		    (ash (the card8 (aref a (index+ i 3))) 24))
	       (the card29
		    (ash (the card8 (aref a (index+ i 2))) 16))
	       (the card16
		    (ash (the card8 (aref a (index+ i 1))) 8))
	       (the card8
		    (aref a i))))
  #-clx-little-endian
  (the card32
       (logior (the card32
		    (ash (the card8 (aref a i)) 24))
	       (the card29
		    (ash (the card8 (aref a (index+ i 1))) 16))
	       (the card16
		    (ash (the card8 (aref a (index+ i 2))) 8))
	       (the card8
		    (aref a (index+ i 3))))))

(DEFUN trace-write-hook (vector display start end)
  ;; Called only by buffer-flush.  Start should always be 0
  (UNLESS (ZEROP start) (FORMAT *debug-io* "write-called with non-zero start: ~d" start))
  (LET* ((history (display-trace-history display))
	 (request-number (display-request-number display))
	 (last-history (CAR history)))
    ;; There may be several requests in the buffer, and the last one may be incomplete.
    ;; The first one may be the completion of a previous request.
    ;; We can detect incomplete requests by comparing the expected length of the last request
    ;; with the actual length.
    (WHEN (AND last-history (NUMBERP (CAR last-history)))
      (LET* ((last-length (index* 4 (byte-ref16 (CDR last-history) 2)))
	     (append-length (MIN (- last-length (LENGTH (CDR last-history)))
				 (- end start))))
	(WHEN (PLUSP append-length)
	  ;; Last history incomplete - append to last
	  (SETF (CDR last-history) (CONCATENATE '(VECTOR card8) (CDR last-history)
					      (SUBSEQ vector start (+ start append-length))))
	  (index-incf start append-length))))
    ;; Copy new requests into the history
    (do* ((new-history nil)
	  (i start (+ i length))
	  request
	  length)
	 ((>= i end)
	  ;; add in sequence numbers
	  (DOLIST (entry new-history)
	    (SETF (CAR entry) request-number)
	    (DECF request-number))
	  (SETF (display-trace-history display)
		(NCONC new-history history)))
      (setq request (AREF vector i))
      (setq length (index* 4 (byte-ref16 vector (+ i 2))))
      (when (zerop length)
	(si:fsignal "Zero length in buffer")
	(return nil))
      (PUSH (CONS 0 (SUBSEQ vector i (MIN (+ i length) end))) new-history)
      (when (zerop request)
	(si:fsignal "Zero request in buffer")
	(return nil))
      )))

(DEFUN trace-read-hook (display vector start end)
  ;; Reading is done with an initial length of 32 (with start = 0)
  ;; This may be followed by several other reads for long replies.
  (LET* ((history (display-trace-history display))
	 (last-history (CAR history))
	 (LENGTH (- end start)))
    (WHEN (AND history (EQ (CAR last-history) :reply))
      (LET* ((last-length (index+ 32 (index* 4 (byte-ref32 (CDR last-history) 4))))
	     (append-length (MIN (- last-length (LENGTH (CDR last-history)))
				 (- end start))))
	(WHEN (PLUSP append-length)
	  (SETF (CDR last-history) (CONCATENATE '(VECTOR card8) (CDR last-history)
						(SUBSEQ vector start (+ start append-length))))
	  (index-incf start append-length)
	  (index-decf length append-length))))
    
    ;; Copy new requests into the history
    (WHEN (PLUSP length)
      (PUSH (CONS (CASE (AREF vector start)
		    (0 :error)
		    (1 :reply)
		    (otherwise :event))
		  (SUBSEQ vector start (+ start length)))
	    (display-trace-history display)))))

(defun display-trace (display &optional length)
  "Display the trace history for DISPLAY.
 The default is to show ALL history entries.
 When the LENGTH parameter is used, only the last LENGTH entries are
 displayed."
  (DECLARE (type display display))
  (DOLIST (hist (REVERSE (SUBSEQ (display-trace-history display)
				 0 length)))
    (LET* ((id (CAR hist))
	   (VECTOR (CDR hist))
	   (LENGTH (LENGTH vector))
	   (request (AREF vector 0)))
      (format t "~%~5d " id)
      (CASE id
	(:error
	 (trace-error-print display vector))
	(:event
	 (FORMAT t "~a (~d) Sequence ~d"
		 (IF (< request (LENGTH *event-key-vector*))
		     (AREF *event-key-vector* request)
		   "Unknown")
		 request
		 (byte-ref16 vector 2)))
	(:reply
	 (FORMAT t "To ~d length ~d"
		 (byte-ref16 vector 2) length)
	 (LET ((actual-length (index+ 32 (index* 4 (byte-ref32 vector 4)))))
	   (UNLESS (= length actual-length)
	     (FORMAT t " Should be ~d **************" actual-length))))
	(otherwise
	 (FORMAT t "~a (~d) length ~d"
		 (request-name request) request length))))))

(DEFUN find-trace (display type sequence)
  (DOLIST (history (display-trace-history display))
    (WHEN (AND (SYMBOLP (CAR history))
	       (= (AREF (CDR history) 0) type)
	       (= (byte-ref16 (CDR history) 2) sequence))
      (RETURN (CDR history)))))

(defun describe-error (display sequence)
  "Describe the error associated with request SEQUENCE."
  (LET ((VECTOR (find-trace display 0 sequence)))
    (IF vector
	(PROGN
	  (TERPRI)
	  (trace-error-print display vector))
      (FORMAT t "Error with sequence ~d not found." sequence))))

(DEFUN trace-error-print (display VECTOR &optional (STREAM *standard-output*))
  (let ((event (allocate-event)))
    ;; Copy into event from reply buffer
    (buffer-replace (reply-ibuf8 event)
		    vector
		    0
		    *replysize*)
    (reading-event (event)
      (let* ((type (read-card8 0))
	     (error-code (read-card8 1))
	     (sequence (read-card16 2))
	     (resource-id (read-card32 4))
	     (minor-code (read-card16 8))
	     (major-code (read-card8 10))
	     (current-sequence (ldb (byte 16 0) (buffer-request-number display)))
	     (error-key
	       (if (< error-code (length *xerror-vector*))
		   (aref *xerror-vector* error-code)
		 'unknown-error))
	     (params
	       (case error-key
		 ((colormap-error cursor-error drawable-error font-error gcontext-error
				  id-choice-error pixmap-error window-error)
		  (list :resource-id resource-id))
		 (atom-error 
		  (list :atom-id resource-id))
		 (value-error
		  (list :value resource-id))
		 (unknown-error
		  ;; Prevent errors when handler is a sequence
		  (setq error-code 0)
		  (list :error-code error-code)))))
	type
	(LET ((condition 
		(APPLY #+lispm #'si:MAKE-CONDITION
		       #-lispm #'make-condition
		       error-key
		       :error-key error-key
		       :display display
		       :major major-code
		       :minor minor-code
		       :sequence sequence
		       :current-sequence current-sequence
		       params)))
	  (PRINC condition stream)
	  (deallocate-event event)
	  condition)))))

(DEFUN describe-request (display sequence)
  "Describe the request with sequence number SEQUENCE"
  #+ti (si:load-if "clx:debug;describe")
  (LET ((request (ASSOC sequence (display-trace-history display))))
    (IF (NULL request)
	(FORMAT t "~%Request number ~d not found in trace history" sequence)
      (let* ((VECTOR (CDR request))
	     (len (LENGTH vector))
	     (hist (make-reply-buffer len)))
	(buffer-replace (reply-ibuf8 hist) vector 0 len)
	(print-history-description hist)))))

(DEFUN describe-reply (display sequence)
  "Print the reply to request SEQUENCE.
 (The current implementation doesn't print very pretty)"
  (LET ((VECTOR (find-trace display 1 sequence))
	(*print-array* t))
    (IF vector
	(PRINT vector)
      (FORMAT T "~%Reply not found"))))

(DEFUN event-number (name)
  (IF (AND (TYPEP name '(integer 0 63))
	   (AREF *event-key-vector* name))
      name
    (POSITION (STRING name) *event-key-vector* :test #'EQUALP :key #'STRING)))

(DEFUN describe-event (display name sequence)
  "Describe the event with event-name NAME and sequence number SEQUENCE."
  (DECLARE (type display display)
	   (type (OR stringable (integer 0 63)) name)
	   (type (integer 0) sequence))
  (LET* ((number (event-number name))
	 (vector (AND number (find-trace display number sequence))))
    (IF (NOT number)
	(FORMAT T "~%~s isn't an event name" name)
      (IF (NOT vector)
	  (FORMAT T "~%Event ~s not found"
		  (AREF *event-key-vector* number))
	(trace-event-print display vector)))))

(DEFUN trace-event-print (display vector)
  (let* ((event (allocate-event))
	 (event-code (LDB (BYTE 7 0) (AREF vector 0)))
	 (event-decoder (aref *event-handler-vector* event-code)))
    ;; Copy into event from reply buffer
    (SETF (event-code event) event-code)
    (buffer-replace (reply-ibuf8 event)
		    vector
		    0
		    *replysize*)
    (PROG1 (funcall event-decoder display event
		    #'(lambda (&rest args &key send-event-p &allow-other-keys)
			(SETQ args (COPY-LIST args))
			(REMF args :display)
			(REMF args :event-code)
			(UNLESS send-event-p (REMF args :send-event-p))
			args))
	   (deallocate-event event))))

(defun describe-trace (display &optional length)
  "Display the trace history for DISPLAY.
 The default is to show ALL history entries.
 When the LENGTH parameter is used, only the last LENGTH entries are
 displayed."
  (DECLARE (type display display))
  #+ti (si:load-if "clx:debug;describe")
  (DOLIST (hist (REVERSE (SUBSEQ (display-trace-history display)
				 0 length)))
    (LET* ((id (CAR hist))
	   (VECTOR (CDR hist))
	   (LENGTH (LENGTH vector)))
      (format t "~%~5d " id)
      (CASE id
	(:error
	 (trace-error-print display vector))
	(:event
	 (LET ((event (trace-event-print display vector)))
	   (WHEN event (FORMAT t "~{ ~s~}" event))))
	(:reply
	 (FORMAT t "To ~d length ~d"
		 (byte-ref16 vector 2) length)
	 (LET ((actual-length (index+ 32 (index* 4 (byte-ref32 vector 4)))))
	   (UNLESS (= length actual-length)
	     (FORMAT t " Should be ~d **************" actual-length)))
	 (LET ((*print-array* t)
	       (*print-base* 16.))
	   (PRINC " ")
	   (PRINC vector)))
	(otherwise
	  (let* ((len (LENGTH vector))
		 (hist (make-reply-buffer len)))
	    (buffer-replace (reply-ibuf8 hist) vector 0 len)
	    (print-history-description hist)))))))

;; End of file
