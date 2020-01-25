;;; -*- Mode: LISP; Syntax: Common-lisp; Package: XLIB; Base: 10; Lowercase: Yes -*-

;;; This file contains definitions for the DISPLAY object for Common-Lisp X windows version 11

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

(in-package "XLIB" :use '("LISP"))

;; Event Resource
(defvar *event-free-list* nil) ;; List of unused (processed) events

(eval-when (eval compile load)
(defconstant *max-events* 64) ;; Maximum number of events supported (the X11 alpha release only has 34)
(defvar *event-key-vector* (make-array *max-events*)
  "Vector of event keys - See define-event")
)
(defvar *event-macro-vector* (make-array *max-events*)
  "Vector of event handler functions - See declare-event")
(defvar *event-handler-vector* (make-array *max-events*)
  "Vector of event handler functions - See declare-event")
(defvar *event-send-vector* (make-array *max-events*)
  "Vector of event sending functions - See declare-event")

(defun allocate-event ()
  (or (atomic-pop *event-free-list*)
      (make-reply-buffer *replysize*)))

(defun deallocate-event (event)
  (atomic-push event *event-free-list*))

;;
;; Reply handlers
;;

(defun wait-for-reply (display expected-size)
  ;; Wait for a reply to a request.
  ;; Expected-size is the length in BYTES,
  ;;   or NIL to only read the first 32 bytes,
  ;;   or T to read in the whole thing.
  ;; Returns with REPLY in the reply-buffer in DISPLAY
  ;; Handle error and event packets that are encountered
  ;; Internal function, MUST BE CALLED FROM WITHIN WITH-BUFFER
  (declare (type display display)
	   (type (or integer (member nil t)) expected-size))
  (declare-values length-in-bytes)
  (buffer-force-output display)
  (do ((sequence-error nil)
       (req-sequence (ldb (byte 16 0) (buffer-request-number display))))
      (nil) ;; forever
    (setf (display-waiting-reply-p display) t) ;; indicate awaiting reply
    (with-input-lock (display)
      (reading-buffer-reply (display)
	;; If waiting-reply-p is :in-buffer then the reply was read in while
	;; waiting for the input lock (see wait-for-event)
	(unless (eq (display-waiting-reply-p display) :in-buffer)
	  ;; Loop for reply-timeout condition return
	  (do ((timeout *reply-timeout* (floor timeout 0.666s0))
	       eofp)
	      (()) ;; forever
	    (setq eofp (buffer-input display buffer-bbuf 0 *replysize* timeout))
	    (if eofp
		(if (eq eofp :timeout)
		    (if sequence-error
			(progn
			  (x-cerror "Proceed, altering display sequence number"
				    'sequence-error
				    :display display
				    :req-sequence req-sequence
				    :msg-sequence sequence-error)
			  (setf (buffer-request-number display) sequence-error
				req-sequence sequence-error)
			  (return nil))
		      (x-cerror "Retry with longer timeout"
				'reply-timeout
				:display display
				:timeout timeout))
		  (progn
		    (setf (display-dead display) t)
		    (x-error 'server-disconnect :display display)))
	      (return nil))))
	(case (read-card8 0) ;; Type
	  (0					; Error
	   (report-error display (buffer-reply-buffer display)))
	  (1					; Normal reply
	    (let ((msg-sequence (read-card16 2))      ;; Message sequence number
		  (length (+ (* (read-card32 4) 4) *replysize*))) ;; Length in bytes
	      (if (= msg-sequence req-sequence)	; Check for reply out of sequence
		  (progn 
		    (when expected-size
		      (if (and (not (eq expected-size t))
			       (> expected-size length))
			  (x-error 'reply-length-error
				   :display display
				   :expected-length expected-size
				   :reply-length length)
			(progn
			  (if (<= length (reply-size (buffer-reply-buffer display)))
			      (buffer-input display buffer-bbuf *replysize* length)
			    (progn
			      ;; reply buffer too small (should never happen!)
			      (cerror "Grow the reply buffer"
				      "Reply buffer too small, increase to ~d bytes"
				      length)
			      ;; Grow it
			      (let* ((new-buffer (make-reply-buffer (+ length 64)))
				     (new-bbuf (reply-ibuf8 new-buffer)))
				(buffer-replace new-bbuf buffer-bbuf 0 *replysize*)
				(buffer-input display new-bbuf *replysize* length)
				(setf (buffer-reply-buffer display) new-buffer)))))))
		    (setf (display-waiting-reply-p display) nil)
		    (return length))
		;; Reply out of sequence
		(if (> msg-sequence req-sequence)
		    (progn
		      (x-cerror "Proceed, altering display sequence number"
				'sequence-error
				:display display
				:req-sequence req-sequence
				:msg-sequence msg-sequence)
		      (setf (buffer-request-number display) msg-sequence))
		  ;; We usually get here because of an abort while waiting for a reply
		  ;; Loop back around and read another reply (hopefully the one we want)
		  (progn
		    ;; **** debug *****
		    (format t "~%wait-for-reply recovering from sequence-error. ~
			         Expected ~d Got ~d" req-sequence msg-sequence)
		    ;; Flush input
		    (buffer-input display buffer-bbuf *replysize* length *reply-timeout*)
		    (setq sequence-error msg-sequence))))))
	  (otherwise				; Event
	    ;; Push the event in the input buffer on the display's event queue
	    (let ((event (allocate-event)))
	      ;; Copy into event from reply buffer
	      (buffer-replace (reply-ibuf8 event)
			      (reply-ibuf8 (buffer-reply-buffer display))
			      0
			      *replysize*)
	      (setf (event-code event) (logand #x7f (read-card8 0)))
	      (enqueue-event event display))))))))

;; Its necessary to have a lock on input operations, to prevent two
;; process from reading from the server at the same time.  Its also
;; necessary to allow one process to be hung inside wait-for-event
;; waiting for an event while another process makes requests and waits
;; for replies.  With this situation, its possible that the process
;; that reads the reply is different from the process waiting for the
;; reply.  The process waiting for the reply must not hang because
;; there's another high priority process that's always waiting for
;; events, and has the input lock.  Because of this, wait-for-event
;; will wait for, and immediately give-up the display lock, when the
;; waiting-reply-p flag is set in the display.  This is sufficient to
;; break the deadlock.

(defun wait-for-event (display timeout)
  ;; Wait for an event.
  ;; Handle error and event packets that are encountered
  ;; Returns :TIMEOUT on timeout, T on eof, else NIL.
  (declare (type display display)
	   (type (or null number) timeout))
  (declare-values eof-or-timeout)
  (let ((event (allocate-event)))
    (loop
      (when (display-waiting-reply-p display) ;; See comments above
	(with-display (display)
	  ;; Flag whould be already NIL.  Reset it here incase wait-for-reply aborted.
	  (setf (display-waiting-reply-p display) nil)))
      (with-input-lock (display)
	;; Return if events read while waiting for locks
	(when (display-new-events display)
	  (deallocate-event event)
	  (return nil))
	(unless (display-waiting-reply-p display) ;; Give up input-lock if waiting event
	  (reading-event (event)
	    ;; Hang waiting for an event
	    (let ((eofp (buffer-input display buffer-bbuf 0 *replysize* timeout)))
	      (when eofp
		(deallocate-event event)
		(if (eq eofp :timeout)
		    (return eofp)
		  (progn
		    (x-error 'server-disconnect :display display)
		    (setf (display-dead display) t)))))
	    ;; Check for replies and errors
	    (let ((type (read-card8 0)))
	      (case type
		(0				; Error
		 (if (display-lock display)
		     (buffer-replace (buffer-obuf8 display)
				     buffer-bbuf
				     0
				     *replysize*)
		   (report-error display event)))
		(1				; Normal reply
		 (if (display-waiting-reply-p display)
		     (progn
		       (buffer-replace (reply-ibuf8 (display-reply-buffer display))
				       buffer-bbuf
				       0
				       *replysize*)
		       (setf (display-waiting-reply-p display) :in-buffer))
		   (x-cerror "Ignore"
			     'unexpected-reply
			     :display display
			     :req-sequence (ldb (byte 16 0) (buffer-request-number display))
			     :msg-sequence (read-card16 2)
			     :length (+ (* (read-card32 4) 4) *replysize*))))
		(otherwise			; Event
		 (setf (event-code event) (logand #x7f (read-card8 0)))
		 (enqueue-event event display)
		 (return nil))))))))))

(export 'event-listen)
(defun event-listen (display &optional (timeout 0))
  (declare (type display display)
	   (type (or null number) timeout))
  ;; Returns the number of events queued locally, if any, else nil.  Hangs waiting
  ;; for events, forever if timeout is nil, else for the specified number of seconds.
  (let ((queue (display-event-queue display)))
    (if queue
	(length queue)
      (progn
	(wait-for-event display timeout)
	(and (setq queue (display-event-queue display))
	     (length queue))))))

(export 'queue-event)
(defun queue-event (display event-key &rest args &key append-p send-event-p &allow-other-keys)
  ;; The event is put at the head of the queue if append-p is nil, else the tail.
  ;; Additional arguments depend on event-key, and are as specified above with
  ;; declare-event, except that both resource-ids and resource objects are accepted
  ;; in the event components.
  (declare (type display display)
	   (type event-key event-key)
	   (type boolean append-p send-event-p))
  (unless (get event-key 'event-code)
    (x-type-error event-key 'event-key))
  (let* ((event (allocate-event))
	 (buffer (reply-ibuf8 event))
	 (event-code (get event-key 'event-code)))
    (unless event-code (x-type-error event-key 'event-key))
    (setf (event-code event) event-code)
    (with-display (display)
      (apply (aref *event-send-vector* event-code) display args)
      (buffer-replace buffer
		      (display-obuf8 display)
		      0
		      *replysize*
		      (index+ 12 (buffer-boffset display)))
      (setf (aref buffer 0) (if send-event-p (logior event-code #x80) event-code)
	    (aref buffer 2) 0
	    (aref buffer 3) 0))
    (with-event-queue (display)
      (if append-p
	  (enqueue-event event display)
	  (with-event-queue-internal (display)
	    (push event (display-event-queue display)))))))

(defun enqueue-event (new-event display)
  ;; Place EVENT at the end of the event queue for DISPLAY
  (let ((event (list new-event)))
    (declare (type list event))
    (let* ((event-code (event-code new-event))
	   (event-key (and (< event-code (length *event-key-vector*))
			   (aref *event-key-vector* event-code))))
      (if (null event-key)
	  (cerror "Ignore this event" "No handler for ~s event" event-key)
	(with-event-queue-internal (display)
	  (let ((new (display-new-events display)))
	    (declare (type list new))
	    (if new
		(setf (display-new-events display) (nconc new event))
	      (progn
		(setf (display-new-events display) event)
		(let ((old (display-event-queue display)))
		  (if old
		      (setf (display-event-queue display) (nconc old event))
		    (setf (display-event-queue display) event)))))))))))


(defmacro define-event (name code)
  `(eval-when (eval compile load)
     (setf (aref *event-key-vector* ,code) ',name)
     (setf (get ',name 'event-code) ,code)))

;; Event names.  Used in "type" field in XEvent structures.  Not to be
;; confused with event masks above.  They start from 2 because 0 and 1
;; are reserved in the protocol for errors and replies. */

(define-event :key-press 2)
(define-event :key-release 3)
(define-event :button-press 4)
(define-event :button-release 5)
(define-event :motion-notify 6)
(define-event :enter-notify 7)
(define-event :leave-notify 8)
(define-event :focus-in 9)
(define-event :focus-out 10)
(define-event :keymap-notify 11)
(define-event :exposure 12)
(define-event :graphics-exposure 13)
(define-event :no-exposure 14)
(define-event :visibility-notify 15)
(define-event :create-notify 16)
(define-event :destroy-notify 17)
(define-event :unmap-notify 18)
(define-event :map-notify 19)
(define-event :map-request 20)
(define-event :reparent-notify 21)
(define-event :configure-notify 22)
(define-event :gravity-notify 23)
(define-event :resize-request 24)
(define-event :configure-request 25)
(define-event :circulate-notify 26)
(define-event :circulate-request 27)
(define-event :property-notify 28)
(define-event :selection-clear 29)
(define-event :selection-request 30)
(define-event :selection-notify 31)
(define-event :colormap-notify 32)
(define-event :client-message 33)
(define-event :mapping-notify 34)


(defmacro declare-event (event-codes &body declares)
  ;; Used to indicate the keyword arguments for handler functions in next-event.
  ;; Generates functions used in SEND-EVENT.
  (declare-arglist event-key-or-keys &rest (type &rest keywords))
  (when (atom event-codes) (setq event-codes (list event-codes)))
  (let* (get-code get-index get-sizes
	 put-code put-index put-sizes keywords
	 (name (first event-codes))
	 (get-macro (xintern name "-EVENT-GET-MACRO"))
	 (get-function (xintern name "-EVENT-GET"))
	 (put-function (xintern name "-EVENT-PUT"))
	 (*buffer* (gensym)))
    (multiple-value-setq (get-code get-index get-sizes)
      (get-put-items 4 declares nil
		     #'(lambda (type index item args)
			 (unless (member type '(pad8 pad16))
			   `(,(intern (string item) "KEYWORD")
			     (,(getify type) ,index ,@args))))))
    (multiple-value-setq (put-code put-index put-sizes)
      (get-put-items 4 declares t
		     #'(lambda (type index item args)
			 (unless (member type '(pad8 pad16))
			   (push item keywords)
			   `((,(putify type) ,index ,item ,@args))))))

    (ignore get-index put-index)
    `(within-definition (,(first event-codes) declare-event)
       (defun ,get-macro (display event-code variable)
	 ;; Note: we take pains to macroexpand the get-code here to enable application
	 ;; code to be compiled without having the CLX macros file loaded.
	 (subst display ',*buffer*
		(getf `(:display (the display ,display)
				 :event-key (the keyword (aref *event-key-vector*
							       ,event-code))
				 :event-code (the integer ,event-code)
				 :send-event-p (the boolean (logbitp 7 (read-card8 0)))
				 ,@',(mapcar #'macroexpand get-code))
		      variable)))

       (defun ,get-function (display event handler)
	 (compiler-let ((*buffer* 'display))
	   (reading-event (event :sizes (8 ,@get-sizes))
	     (let ((event-code (logand #x7f (card8-get 0))))
	       (funcall handler :display display
			:event-key (aref *event-key-vector* event-code)
			:event-code event-code
			:send-event-p (logbitp 7 (card8-get 0))
			,@get-code)))))

       (defun ,put-function (display &key ,@(nreverse keywords) &allow-other-keys)
	 (writing-buffer-send (display :sizes ,put-sizes
				       :index (index+ (buffer-boffset display) 12))
	   ,@put-code))
       
       ,@(mapcan #'(lambda (name)
		     (let ((event-code (get name 'event-code) ))
		       (unless event-code
			 (error "~s isn't a defined event-key" name))
		       `((setf (aref *event-macro-vector* ,event-code)
			       (function ,get-macro))
			 (setf (aref *event-handler-vector* ,event-code)
			       (function ,get-function))
			 (setf (aref *event-send-vector* ,event-code)
			       (function ,put-function)))))
		 event-codes))))

(declare-event (:key-press :key-release :button-press :button-release)
  ;; for key-press and key-release, code is the keycode
  ;; for button-press and button-release, code is the button number
  (data code)
  (card32 time)
  (window root window)
  ((or null window) child)
  (int16 root-x root-y x y)
  (card16 state)
  (boolean same-screen-p)
  )

(declare-event :motion-notify
  ((data boolean) hint-p)
  (card32 time)
  (window root window)
  ((or null window) child)
  (int16 root-x root-y x y)
  (card16 state)
  (boolean same-screen-p))

(declare-event (:enter-notify :leave-notify)
  ((data (member8 :ancestor :virtual :inferior :nonlinear :nonlinear-virtual)) kind)
  (card32 time)
  (window root window)
  ((or null window) child)
  (int16 root-x root-y x y)
  (card16 state)
  ((member8 :normal :grab :ungrab) mode)
  ((bit 0) focus-p)
  ((bit 1) same-screen-p))

(declare-event (:focus-in :focus-out)
  ((data (member8 :ancestor :virtual :inferior :nonlinear :nonlinear-virtual
		  :pointer :pointer-root :none))
   kind)
  (window window)
  ((member8 :normal :while-grabbed :grab :ungrab) mode))

(declare-event :keymap-notify
  (window window)
  ((bit-vector256 0) keymap))

(declare-event :exposure
  (window window)
  (card16 x y width height count))

(declare-event :graphics-exposure
  (drawable drawable)
  (card16 x y width height)
  (card16 minor)  ;; Minor opcode
  (card16 count)
  (card8 major))  ;; Major opcode

(declare-event :no-exposure
  (drawable drawable)
  (card16 minor)
  (card8  major))

(declare-event :visibility-notify
  (window window)
  ((member8 :unobscured :partially-obscured :fully-obscured) state))

(declare-event :create-notify
  (window parent window)
  (int16 x y)
  (card16 width height border-width)
  (boolean override-redirect-p))

(declare-event :destroy-notify
  (window event-window window))

(declare-event :unmap-notify
  (window event-window window)
  (boolean configure-p))

(declare-event :map-notify
  (window event-window window)
  (boolean override-redirect-p))

(declare-event :map-request
  (window parent window))

(declare-event :reparent-notify
  (window event-window window parent)
  (int16 x y)
  (boolean override-redirect-p))

(declare-event :configure-notify
  (window event-window window)
  ((or null window) above-sibling)
  (int16 x y)
  (card16 width height border-width)
  (boolean override-redirect-p))

(declare-event :configure-request
  ((data (member :above :below :top-if :bottom-if :opposite)) stack-mode)
  (window parent window)
  ((or null window) above-sibling)
  (int16 x y)
  (card16 width height border-width value-mask))

(declare-event :gravity-notify
  (window event-window window)
  (int16 x y))

(declare-event :resize-request
  (window window)
  (card16 width height))

(declare-event :circulate-notify
  (window event-window window parent)
  ((member16 :top :bottom) place))

(declare-event :circulate-request
  (window parent window)
  (pad16 1 2)
  ((member16 :top :bottom) place))

(declare-event :property-notify
  (window window)
  (keyword atom) ;; keyword
  (card32 time)
  ((member16 :new-value :deleted) state))

(declare-event :selection-clear
  (card32 time)
  (window window) 
  (keyword selection) ;; keyword
  )

(declare-event :selection-request
  (card32 time)
  (window window requestor)
  (keyword selection target)
  ((or null keyword) property)
  )

(declare-event :selection-notify
  (card32 time)
  (window window)
  (keyword selection target)
  ((or null keyword) property)
  )

(declare-event :colormap-notify
  (window window)
  ((or null colormap) colormap)
  (boolean new-p installed-p))

(declare-event :client-message
  (data format)
  (window window)
  (keyword type)
  ((client-message-sequence format) data))

(declare-event :mapping-notify
  ((member8 :modifier :keyboard :pointer) request)
  (card8 start) ;; first key-code
  (card8 count))


;;
;; EVENT-LOOP
;;
(defmacro event-loop ((display event timeout peek-p discard-p force-output-p) &body body)
  ;; Bind EVENT to the events for DISPLAY.
  ;; This is the "GUTS" of process-event and event-case.
  (let ((previous (gensym))
	(events (gensym))
	(result (gensym))
	(new (gensym)))
    `(with-event-queue (,display)
       (do* ((,previous nil)
	     (,events (display-event-queue ,display) (cdr ,events))
	     (,result nil))
	    ((or ,result
		 (and (endp ,events) ,timeout (zerop ,timeout)))
	     ,result)
						; Read events when queue empty
	 (unless ,events
	   ,(when force-output-p
	      `(when ,force-output-p (display-force-output ,display)))
	   (when (wait-for-event ,display ,timeout)
	     (return nil)) ;; return when timeout exceeded
	   (setq ,events (display-event-queue ,display)
		 ,previous nil))
						; Keep the new-event list updated
	 (with-event-queue-internal (,display)
	   ;; When event is new, Pop the new-event list
	   (let ((,new (display-new-events ,display)))
	     (when (eq ,events ,new)
	       (setf (display-new-events ,display) (cdr ,new)))))
						; Execute the body
	 (let ((,event (car ,events)))
	   (setq ,result (progn ,@body))
						; Delete entry when ,result was NIL
	   (if (if ,result
		   (not ,peek-p)
		   ,discard-p)
	       (progn				;Delete event
		 (if ,previous			; When Event in middle of queue,
		     (setf (cdr ,previous) (cdr ,events)) ;; Destructively splice it out.
		     (pop (display-event-queue ,display)))
		 (deallocate-event ,event))
	       (setq ,previous ,events))	; Don't delete event - update previous
	   )))))
;;
;; PROCESS-EVENT
;;
(export 'process-event)
(defun process-event (display &key handler timeout peek-p discard-p force-output-p)
  ;; If force-output-p is true, first invokes display-force-output.  Invokes handler
  ;; on each queued event until handler returns non-nil, and that returned object is
  ;; then returned by process-event.  If peek-p is true, then the event is not
  ;; removed from the queue.  If discard-p is true, then events for which handler
  ;; returns nil are removed from the queue, otherwise they are left in place.  Hangs
  ;; until non-nil is generated for some event, or for the specified timeout (in
  ;; seconds, if given); however, it is acceptable for an implementation to wait only
  ;; once on network data, and therefore timeout prematurely.  Returns nil on
  ;; timeout.  If handler is a sequence, it is expected to contain handler functions
  ;; specific to each event class; the event code is used to index the sequence,
  ;; fetching the appropriate handler.  Handler is called with raw resource-ids, not
  ;; with resource objects.  The arguments to the handler are described using declare-event.
  ;;
  ;; T for peek-p means the event (for which the handler returns non-nil) is not removed
  ;; from the queue (it is left in place), NIL means the event is removed.
  
  (declare (type display display)
	   (type t handler) ;; (or (sequence (function (display &rest key-vals) t))
	   ;;	   (function (display event-key &rest key-vals) t))
	   (type (or null number) timeout)
	   (type boolean peek-p))
  (event-loop (display event timeout peek-p discard-p force-output-p)
    (let* ((event-code (event-code event)) ;; Event decoder defined by DECLARE-EVENT
	   (event-decoder (and (< event-code (length *event-handler-vector*))
			       (aref *event-handler-vector* event-code))))
      (if event-decoder
	  (let ((event-handler (if (type? handler 'sequence)
				   (and (< event-code (length handler))
					(elt handler event-code))
				 handler)))
	    (if event-handler
		(funcall event-decoder display event event-handler)
	      (cerror "Ignore this event"
		      "No handler for ~s event"
		      (aref *event-key-vector* event-code))))
	(cerror "Ignore this event"
		"Server Error: event with unknown event code ~d received."
		event-code)))))

;;
;; EVENT-CASE
;; 
;; This version sets-up a vector of event functions at run time, which
;; is passed into PROCESS-EVENT.  Would it be better to somehow construct
;; (a constant) event-vector at compile-time?  Would it be even better
;; to construct code that would pull parameters directly out of the event
;; and pass them into a lambda expression, rather than using lexical closures?
;; (i.e. expand the code generated for the declare-event get-function inside
;; the event-case call)
(export 'event-case)
(defmacro event-case ((display &key timeout peek-p discard-p force-output-p)
		      &body clauses)
  (declare-arglist (display &key timeout peek-p discard-p force-output-p)
		   (event-or-events ((&rest args) |...|) &body body) |...|)
  ;; If force-output-p is true, first invokes display-force-output.  Executes the
  ;; matching clause for each queued event until a clause returns non-nil, and that
  ;; returned object is then returned by event-case.  If peek-p is true, then the
  ;; event is not removed from the queue.  If discard-p is true, then events for
  ;; which the clause returns nil are removed from the queue, otherwise they are left
  ;; in place.  Hangs until non-nil is generated for some event, or for the specified
  ;; timeout (in seconds, if given); however, it is acceptable for an implementation
  ;; to wait only once on network data, and therefore timeout prematurely.  Returns
  ;; nil on timeout.  In each clause, event-or-events is an event-key or a list of
  ;; event-keys (but they need not be typed as keywords) or the symbol t or otherwise
  ;; (but only in the last clause).  The keys are not evaluated, and it is an error
  ;; for the same key to appear in more than one clause.  Args is the list of event
  ;; components of interest; corresponding values (if any) are bound to variables
  ;; with these names (i.e., the args are variable names, not keywords, the keywords
  ;; are derived from the variable names).  An arg can also be a (keyword var) form,
  ;; as for keyword args in a lambda lists.  If no t/otherwise clause appears, it is
  ;; equivalent to having one that returns nil.
  (let ((event (gensym))
	(disp (gensym)))
    `(let ((,disp ,display))
       (event-loop (,disp ,event ,timeout ,peek-p ,discard-p ,force-output-p)
	 (event-dispatch (,disp ,event) ,@clauses)))))

(defun get-event-code (event)
  ;; Returns the event code given an event-key
  (declare (type event-key event))
  (declare-values card8)
  (or (get (find-symbol (symbol-name event) "KEYWORD") 'event-code)
      (x-type-error event 'event-key)))

(defmacro event-dispatch ((display event) &body clauses)
  ;; Helper macro for event-case
  (let ((event-code (gensym))
	(all-events 0))
    `(reading-event (,event)
       (let ((,event-code (event-code ,event)))
	 (case ,event-code
	   ,@(mapcar
	      #'(lambda (clause)
		  (let* ((events (car clause))
			 (arglist (cadr clause))
			 (body (cddr clause)))
		    (if (member events '(otherwise t))
			;; code for OTHERWISE clause.  Find all events NOT
			;; used by other clauses
			(let ((keys (do ((i 0 (1+ i))
					 (key nil)
					 (result nil))
					((>= i *max-events*) result)
				      (setq key (aref *event-key-vector* i))
				      (when (and key (not (logbitp i all-events)))
					(push key result)))))
			  `(otherwise (binding-event-values
					(,display ,event-code ,keys ,@arglist)
					,@body)))
		      ;; Code for normal clauses
		      (let (codes) ;; Convert event-key's to event-code's
			(if (consp events)
			    (setq codes (mapcar #'get-event-code events)
				  all-events (apply #'logior all-events codes))
			  (setq codes (get-event-code events)
				all-events (logior all-events codes)))
			`(,codes (binding-event-values
				   (,display ,event-code ,events ,@arglist) ,@body))))))
		     clauses))))))

(defmacro binding-event-values ((display event-code event-keys &rest value-list) &body body)
  ;; Execute BODY with the variables in VALUE-LIST bound to components of the
  ;; EVENT-KEYS events.
  (unless (consp event-keys) (setq event-keys (list event-keys)))
  (flet ((var-key (var) (intern (symbol-name (if (consp var) (first var) var)) "KEYWORD"))
	 (var-symbol (var) (if (consp var) (second var) var)))
    ;; VARS is an alist of:
    ;;  (component-key ((event-key event-key ...) . extraction-code)
    ;;		       ((event-key event-key ...) . extraction-code) ...)
    ;; There should probably be accessor macros for this, instead of things like cdadr.
    (let ((vars (mapcar #'(lambda (var) (list var)) value-list))
	  (multiple-p nil))
      ;; Fill in the VARS alist with event-keys and extraction-code
      (do ((keys event-keys (cdr keys))
	   (temp nil))
	  ((endp keys))
	(let* ((key (car keys))
	       (binder (aref *event-macro-vector* (get-event-code key))))
	  (dolist (var vars)
	    (let ((code (funcall binder display event-code (var-key (car var)))))
	      (unless code (format t "~%Warning: ~a isn't a component of the ~s event"
				   (var-key (car var)) key))
	      (if (setq temp (member code (cdr var) :key #'cdr :test #'equal))
		  (push key (caar temp))
		(push `((,key) . ,code) (cdr var)))))))
      ;; Bind all the values
      `(let ,(mapcar #'(lambda (var)
			 (if (cddr var) ;; if more than one binding form
			     (progn (setq multiple-p t)
				    (var-symbol (car var)))
			   (list (var-symbol (car var)) (cdadr var))))
		     vars)
	 ;; When some values come from different places, generate code to set them
	 ,(when multiple-p
	    `(case ,event-code
	       ,@(do ((keys event-keys (cdr keys))
		      (result nil) ;; alist of (binding-code . event-keys))
		      event-key
		      temp)
		     ((endp keys)
		      ;; Reverse result into a case clause
		      (do ((clause (nreverse result) (cdr clause))
			   (code nil))
			  ((endp clause) code)
			(push `(,(if (cddar clause)
				     (mapcar #'get-event-code (cdar clause))
				   (get-event-code (cadar clause)))
				,(caar clause))
			      code)))
		   (setq event-key (car keys))
		   (dolist (var vars)
		     (when (cddr var) ;; more than one binding form
		       (dolist (code (cdr var))
			 (let* ((var-name (var-symbol (car var)))
				(var-code `(setq ,var-name ,(cdr code))))
			   (when (member event-key (car code))
			     (if (setq temp (assoc var-code result :test #'equal))
				 (push event-key (cdr temp))
			       (push `(,var-code ,event-key) result))))))))))
	 ,@body))))


;;;-----------------------------------------------------------------------------
;;; Error Handling
;;;-----------------------------------------------------------------------------

(eval-when (eval compile load)
(defparameter
  *xerror-vector*
  '#(unknown-error
     request-error				; 1  bad request code
     value-error				; 2  integer parameter out of range
     window-error				; 3  parameter not a Window
     pixmap-error				; 4  parameter not a Pixmap
     atom-error					; 5  parameter not an Atom
     cursor-error				; 6  parameter not a Cursor
     font-error					; 7  parameter not a Font
     match-error				; 8  parameter mismatch
     drawable-error				; 9  parameter not a Pixmap or Window
     access-error				; 10 attempt to access private resource"
     alloc-error				; 11 insufficient resources
     colormap-error				; 12 no such colormap
     gcontext-error				; 13 parameter not a GContext
     id-choice-error				; 14 invalid resource ID for this connection
     name-error					; 15 font or color name does not exist
     length-error				; 16 request length incorrect;
						;    internal Xlib error
     implementation-error			; 17 server is defective
     ))
)

(export (cdr (coerce *xerror-vector* 'list)))

(defun report-error (display event)
  ;; All errors (synchronous and asynchronous) are processed by calling
  ;; an error handler in the display.  The handler is called with the display
  ;; as the first argument and the error-key as its second argument. If handler is
  ;; an array it is expected to contain handler functions specific to
  ;; each error; the error code is used to index the array, fetching the
  ;; appropriate handler. Any results returned by the handler are ignored;;
  ;; it is assumed the handler either takes care of the error completely,
  ;; or else signals. For all core errors, additional keyword/value argument
  ;; pairs are:
  ;;    :major integer
  ;;    :minor integer
  ;;    :sequence integer
  ;;    :current-sequence integer
  ;; For :colormap, :cursor, :drawable, :font, :GContext, :id-choice, :pixmap, and :window
  ;; errors another pair is:
  ;;    :resource-id integer
  ;; For :atom errors, another pair is:
  ;;    :atom-id integer
  ;; For :value errors, another pair is:
  ;;    :value integer
  (reading-event (event)
    (let* ((type (read-card8 0))
	   (error-code (read-card8 1))
	   (sequence (read-card16 2))
	   (resource-id (read-card32 4))
	   (minor-code (read-card16 8))
	   (major-code (read-card8 10))
	   (handler (display-error-handler display))
	   (current-sequence (ldb (byte 16 0) (buffer-request-number display)))
	   (handler-function
	     (if (type? handler 'sequence)
		 (elt handler error-code)
	       handler))
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
      (unwind-protect
	  (apply handler-function
		 display error-key
		 :major major-code
		 :minor minor-code
		 :sequence sequence
		 :current-sequence current-sequence
		 params)
	;; Eat up any remaining server information
	(do ()
	    ((or (>= sequence current-sequence)
		 (buffer-input display buffer-bbuf 0 *replysize* 0)))
	  (case (read-card8 0) ;; type
	    (0					; Another error
	      (report-error display event))
	    (1					; Reply
	      (return t))
	    (otherwise				; Event
	      ;; Push the event in the input buffer on the display's event queue
	      (let ((event (allocate-event)))
		;; Copy into event from reply buffer
		(buffer-replace (reply-ibuf8 event)
				(reply-ibuf8 (buffer-reply-buffer display))
				0
				*replysize*)
		(setf (event-code event) (logand #x7f (read-card8 0)))
		(enqueue-event event display)))))))))

(define-condition request-error error
  :report
  (format t
	  "~a in ~:[request ~d (last request was ~d) ~;current request~2* ~] Code ~d.~d [~a]"
	  error-key (= sequence current-sequence) sequence current-sequence major minor
	  (request-name major))
  display
  error-key
  major
  minor
  sequence
  current-sequence)

(define-condition resource-error request-error
  :report
  (format t
	  "~a in ~:[request ~d (last request was ~d) ~;current request~2* ~] Code ~d.~d [~a] ID #x~x"
	  error-key (= sequence current-sequence) sequence current-sequence major minor
	  (request-name major) resource-id)
  resource-id)

(define-condition unknown-error request-error
  error-code)

(define-condition access-error request-error)

(define-condition alloc-error request-error)

(define-condition atom-error request-error
  atom-id)

(define-condition colormap-error resource-error)

(define-condition cursor-error resource-error)

(define-condition drawable-error resource-error)

(define-condition font-error resource-error)

(define-condition gcontext-error resource-error)

(define-condition id-choice-error resource-error)

(define-condition illegal-request-error request-error)

(define-condition length-error request-error)

(define-condition match-error request-error)

(define-condition name-error request-error)

(define-condition pixmap-error resource-error)

(define-condition value-error request-error
  value)

(define-condition window-error resource-error)

(define-condition implementation-error request-error)

;;-----------------------------------------------------------------------------
;; Internal error conditions signaled by CLX

(define-condition type-error error
  :report (format t "~s isn't ~@[a ~] ~s" object type-string type)
  object
  type
  type-string)

(define-condition closed-display error
  :report (format t "Attempt to use closed display ~s" display)
  display)

(define-condition lookup-error error
  :report (format t "ID ~d from display ~s should have been a ~s, but was ~s"
		  id display type object)
  id display type object)

(define-condition connection-failure error
  :report (format t "Connection failure to X~d.~d server ~a display ~d: ~a"
		  major-version minor-version
		  host display reason)
  major-version
  minor-version
  host
  display
  reason)

(define-condition reply-length-error error
  :report (format t "Reply length was ~d when ~d words were expected for display ~s"
		  reply-length expected-length display)
  reply-length
  expected-length
  display)

(define-condition reply-timeout error
  :report (format t "Timeout after waiting ~d seconds for a reply for display ~s"
		  timeout display)
  timeout
  display)

(define-condition server-disconnect error
  :report (format t "Server disconnect for display ~s" display)
  display)

(define-condition sequence-error error
  :report (format t  "Reply out of sequence for display ~s.~%  Expected ~d, Got ~d"
		  display req-sequence msg-sequence)
  display
  req-sequence
  msg-sequence)

(define-condition unexpected-reply error
  :report (format t "Display ~s received a server reply when none was expected.~@
		     Last request sequence ~d Reply Sequence ~d Reply Length ~d bytes."
		  display req-sequence msg-sequence length)
  display
  msg-sequence
  req-sequence
  length)

(define-condition missing-parameter error
  :report (if (consp parameter)
	      (format t "One or more of the required parameters ~a is missing."
		      parameter)
	    (format t "Required parameter ~a is missing or null." parameter))
  parameter)

;; This can be signalled anywhere a pseudo font access fails.
(define-condition invalid-font error
  :report (format t "Can't access font ~s" font)
  font)

;;-----------------------------------------------------------------------------

(export '(request-error
	  resource-error
	  unknown-error
	  access-error
	  alloc-error
	  atom-error
	  colormap-error
	  cursor-error
	  drawable-error
	  font-error
	  gcontext-error
	  id-choice-error
	  illegal-request-error
	  length-error
	  match-error
	  name-error
	  pixmap-error
	  value-error
	  window-error
	  implementation-error
	  type-error
	  closed-display
	  lookup-error
	  connection-failure
	  reply-length-error
	  reply-timeout
	  server-disconnect
	  sequence-error
	  unexpected-reply
	  missing-parameter
	  invalid-font
	  ))

