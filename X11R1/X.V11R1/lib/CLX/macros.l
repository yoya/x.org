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

;;; CLX basicly implements a very low overhead remote procedure call
;;; to the server.  This file contains macros which generate the code
;;; for both the client AND the server, given a specification of the
;;; interface. This was done to eliminate errors that may occur because
;;; the client and server code get/put bytes in different places, and
;;; it makes it easier to extend the protocol.

;;; This is built on top of BUFFER

(in-package "XLIB" :use '("LISP"))

;;; This variable is used by the required-arg macro just to satisfy compilers.
(defvar *required-arg-dummy*)

;;; An error signalling macro use to specify that keyword arguments are required.
(defmacro required-arg (name)
  `(progn (x-error 'missing-parameter :parameter ',name)
	  *required-arg-dummy*))

(defmacro lround (index)
  ;; Round up to the next 32 bit boundary
  `(the array-index (logand (index+ ,index 3) -4)))

(defmacro wround (index)
  ;; Round up to the next 16 bit boundary
  `(the array-index (logand (index+ ,index 1) -2)))

;;
;; Data-type accessor functions
;;
;;   These functions translate between lisp data-types and the byte,
;;   half-word or word that gets transmitted across the client/server
;;   connection

(defun index-increment (type)
  ;; Given a type, return its field width in bytes
  (let* ((name (if (consp type) (car type) type))
	 (increment (get name 'byte-width :not-found)))
    (when (eq increment :not-found)
      ;; Check for TYPE in a different package
      (when (not (eq (symbol-package name) (find-package "XLIB")))
	(setq name (xintern  name))
	(setq increment (get name 'byte-width :not-found)))
      (when (eq increment :not-found)
	(error "~s isn't a known field accessor" name)))
    increment))

(eval-when (eval compile load)
(defun getify (name)
  (xintern name "-GET"))

(defun putify (name &optional predicate-p)
  (xintern name "-PUT" (if predicate-p "-PREDICATING" "")))

					;; Use &body so zmacs indents properly
(defmacro define-accessor (name (width) &body get-put-macros)
  ;; The first body form defines the get macro
  ;; The second body form defines the put macro
  ;; The third body form is optional, and defines a put macro that does
  ;; type checking and does a put when ok, else NIL when the type is incorrect.
  ;; If no third body form is present, then these macros assume that
  ;; (AND (TYPEP ,thing 'type) (PUT-type ,thing)) can be generated.
  ;; these predicating puts are used by the OR accessor.
  (declare-arglist name (width) get-macro put-macro &optional predicating-put-macro)
  (when (cdddr get-put-macros)
    (error "Too many parameters to define-accessor: ~s" (cdddr get-put-macros)))
  (let ((get-macro (or (first get-put-macros) (error "No GET macro form for ~s" name)))
	(put-macro (or (second get-put-macros) (error "No PUT macro form for ~s" name)))
	(predicating-put (third get-put-macros)))
    `(within-definition (,name define-accessor)
       (setf (get ',name 'byte-width) ,(and width (floor width 8)))
       (defmacro ,(getify name) ,(car get-macro)
	 ,@(cdr get-macro))
       (defmacro ,(putify name) ,(car put-macro)
	 ,@(cdr put-macro))
       ,@(when (and *type-check?* predicating-put)
	   `((setf (get ',name 'predicating-put) t)
	     (defmacro ,(putify name t) ,(car predicating-put)
	       ,@(cdr predicating-put)))))))
) ;; End eval-when

(define-accessor card32 (32)
  ((index) `(read-card32 ,index))
  ((index thing) `(write-card32 ,index ,thing)))

(define-accessor card16 (16)
  ((index) `(read-card16 ,index))
  ((index thing) `(write-card16 ,index ,thing)))

(define-accessor card8 (8)
  ((index) `(read-card8 ,index))
  ((index thing) `(write-card8 ,index ,thing)))

(define-accessor integer (32)
  ((index) `(read-int32 ,index))
  ((index thing) `(write-int32 ,index ,thing)))

(define-accessor int16 (16)
  ((index) `(read-int16 ,index))
  ((index thing) `(write-int16 ,index ,thing)))

(define-accessor rgb-val (16)
  ;; Used for color's
  ((index) `(card16->rgb-val (read-card16 ,index)))
  ((index thing) `(write-card16 ,index (rgb-val->card16 ,thing))))

(define-accessor angle (16)
  ;; Used for drawing arcs
  ((index) `(int16->radians (read-int16 ,index)))
  ((index thing) `(write-int16 ,index (radians->int16 ,thing))))

(define-accessor bit (0)
  ;; Like BOOLEAN, but tests bits
  ;; only used by declare-event (:enter-notify :leave-notify)
  ((index bit)
   `(logbitp ,bit (read-card8 ,index)))
  ((index thing bit)
   (if (zerop bit)
       `(write-card8 ,index (if ,thing 1 0))
     `(write-card8 ,index (dpb (if ,thing 1 0) (byte 1 ,bit) (read-card8 ,index))))))

(define-accessor boolean (8)
  ((index)
   `(plusp (read-card8 ,index)))
  ((index thing) `(write-card8 ,index (if ,thing 1 0))))

(proclaim '(special *buffer*)) ;; Bound with COMPILER-LET inside the buffer macros 

(define-accessor drawable (32)
  ((index &optional (buffer *buffer*))
   `(lookup-drawable ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (drawable-id ,thing))))

(define-accessor window (32)
  ((index &optional (buffer *buffer*))
   `(lookup-window ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (window-id ,thing))))

(define-accessor pixmap (32)
  ((index &optional (buffer *buffer*))
   `(lookup-pixmap ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (pixmap-id ,thing))))

(define-accessor gcontext (32)
  ((index &optional (buffer *buffer*))
   `(lookup-gcontext ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (gcontext-id ,thing))))

(define-accessor cursor (32)
  ((index &optional (buffer *buffer*))
   `(lookup-cursor ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (cursor-id ,thing))))

(define-accessor colormap (32)
  ((index &optional (buffer *buffer*))
   `(lookup-colormap ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (colormap-id ,thing))))

(define-accessor font (32)
  ((index &optional (buffer *buffer*))
   `(lookup-font ,buffer (read-card29 ,index)))
  ((index thing) `(write-card29 ,index (font-id ,thing))))

;; Needed to get and put xatom's in events
(define-accessor keyword (32)
  ((index &optional (buffer *buffer*))
   `(lookup-xatom ,buffer (read-card29 ,index)))
  ((index thing &key (buffer *buffer*))
   `(write-card29 ,index (or (atom-id ,thing ,buffer)
			     (error "CLX implementation error in KEYWORD-PUT")))))

(define-accessor resource-id (32)
  ((index) `(read-card29 ,index))
  ((index thing) `(write-card29 ,index ,thing)))

(define-accessor resource-id-or-nil (32)
  ((index) (let ((id (gensym)))
	     `(let ((,id (read-card29 ,index)))
		(and (plusp ,id) ,id))))
  ((index thing) `(write-card29 ,index (or ,thing 0))))

(defmacro char-info-get (index)
  `(make-char-info
     :left-bearing (int16-get ,index)
     :right-bearing (int16-get ,(+ index 2))
     :width	   (int16-get ,(+ index 4))
     :ascent	   (int16-get ,(+ index 6))
     :descent	   (int16-get ,(+ index 8))
     :attributes   (card16-get ,(+ index 10))))

(define-accessor member8 (8)
  ((index &rest keywords)
   (let ((value (gensym)))
     `(let ((,value (read-card8 ,index)))
	(and (< ,value ,(length keywords))
	     (aref ,(apply #'vector keywords) ,value)))))
  ((index thing &rest keywords)
   `(write-card8 ,index (position ,thing (the simple-vector ,(apply #'vector keywords))
				  :test #'eq)))
  ((index thing &rest keywords)
   (let ((value (gensym)))
     `(let ((,value (position ,thing (the simple-vector ,(apply #'vector keywords))
			      :test #'eq)))
	(and ,value (write-card8 ,index ,value))))))

(define-accessor member16 (16)
  ((index &rest keywords)
   (let ((value (gensym)))
     `(let ((,value (read-card16 ,index)))
	(and (< ,value ,(length keywords))
	     (aref ,(apply #'vector keywords) ,value)))))
  ((index thing &rest keywords)
   `(write-card16 ,index (position ,thing (the simple-vector ,(apply #'vector keywords))
				   :test #'eq)))
  ((index thing &rest keywords)
   (let ((value (gensym)))
     `(let ((,value (position ,thing (the simple-vector ,(apply #'vector keywords))
			      :test #'eq)))
	(and ,value (write-card16 ,index ,value))))))

(define-accessor member (32)
  ((index &rest keywords)
   (let ((value (gensym)))
     `(let ((,value (read-card29 ,index)))
	(and (< ,value ,(length keywords))
	     (aref ,(apply #'vector keywords) ,value)))))
  ((index thing &rest keywords)
   `(write-card29 ,index (position ,thing (the simple-vector ,(apply #'vector keywords))
				   :test #'eq)))
  ((index thing &rest keywords)
   (if (cdr keywords) ;; IF more than one
       (let ((value (gensym)))
	 `(let ((,value (position ,thing (the simple-vector ,(apply #'vector keywords))
				  :test #'eq)))
	    (and ,value (write-card29 ,index ,value))))
     `(and (eq ,thing ,(car keywords)) (write-card29 ,index 0)))))

(deftype member-vector (vector) `(member ,@(coerce (symbol-value vector) 'list)))

(define-accessor member-vector (32)
  ((index membership-vector)
   `(member-get ,index ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member-put ,index ,thing ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member-put ,index ,thing ,@(coerce (eval membership-vector) 'list))))

(define-accessor member16-vector (16)
  ((index membership-vector)
   `(member16-get ,index ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member16-put ,index ,thing ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member16-put ,index ,thing ,@(coerce (eval membership-vector) 'list))))

(define-accessor member8-vector (8)
  ((index membership-vector)
   `(member8-get ,index ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member8-put ,index ,thing ,@(coerce (eval membership-vector) 'list)))
  ((index thing membership-vector)
   `(member8-put ,index ,thing ,@(coerce (eval membership-vector) 'list))))

(define-accessor boole-constant (32)
  ;; this isn't member-vector because we need eql instead of eq
  ((index)
   (let ((value (gensym)))
     `(let ((,value (read-card29 ,index)))
	(and (< ,value ,(length *boole-vector*))
	     (aref *boole-vector* ,value)))))
  ((index thing)
   `(write-card29 ,index (position ,thing (the simple-vector *boole-vector*))))
  ((index thing)
   (let ((value (gensym)))
     `(let ((,value (position ,thing (the simple-vector *boole-vector*))))
	(and ,value (write-card29 ,index ,value))))))

(define-accessor null (32)
  ((index) `(if (zerop (read-card32 ,index)) nil (read-card32 ,index)))
  ((&rest stuff) stuff 0))

(define-accessor pad8 (8)
  ((index) index nil)
  ((index value) index value nil))

(define-accessor pad16 (16)
  ((index) index nil)
  ((index value) index value nil))

(define-accessor bit-vector256 (256)
  ;; used for key-maps
  ((index &optional (buffer *buffer*))
   #+(and clx-overlapping-arrays clx-little-endian)
   `(read-bitvector256 ,buffer ,index)
   #-(and clx-overlapping-arrays clx-little-endian)
   `(read-bitvector256 ,buffer ,index))
  ((index map &optional (buffer *buffer*))
   #+(and clx-overlapping-arrays clx-little-endian)
   `(write-bitvector256 ,buffer ,index ,map)
   #-(and clx-overlapping-arrays clx-little-endian)
   `(write-bitvector256 ,buffer ,index ,map)))
   
(define-accessor string (nil)
  ((length &key buffer)
   `(read-sequence-char ,(or buffer *buffer*) 'string ,length))
  ((index string &key buffer (start 0) end header-length appending)
   (unless buffer (setq buffer *buffer*))
   (unless header-length (setq header-length (lround index)))
   (let* ((real-end (if appending (or end `(length ,string)) (gensym)))
	  (form `(write-sequence-char ,buffer (index+ buffer-boffset ,header-length)
				      ,string ,start ,real-end)))
     (if appending
	 form
	 `(let ((,real-end ,(or end `(length ,string))))
	    (write-card16 2 (index-ceiling (index+ (index- ,real-end ,start)
						   ,header-length)
					   4))
	    ,form)))))

(define-accessor sequence (nil)
  ((&key length (format 'card32) result-type transform buffer data start)
   `(,(ecase format
	(card8 'read-sequence-card8)
	(int8 'read-sequence-int8)
	(card16 'read-sequence-card16)
	(int16 'read-sequence-int16)
	(card32 'read-sequence-card32)
	(int32 'read-sequence-int32))
     ,(or buffer *buffer*) ,result-type ,length ,transform ,data ,@(when start `(,start))))
  ((index data &key (format 'card32) (start 0) end transform buffer appending)
   (unless buffer (setq buffer *buffer*))
   (let* ((real-end (if appending (or end `(length ,data)) (gensym)))
	  (writer (xintern "WRITE-SEQUENCE-" format))
	  (form `(,writer ,buffer (index+ buffer-boffset ,(lround index))
		  ,data ,start ,real-end ,transform)))
     (flet ((maker (size)
	      (if appending
		  form
		  (let ((idx `(index- ,real-end ,start)))
		    (unless (= size 1)
		      (setq idx `(index-ceiling ,idx ,size)))
		    `(let ((,real-end ,(or end `(length ,data))))
		       (write-card16 2 (index+ ,idx ,(index-ceiling index 4)))
		       ,form)))))
       (ecase format
	 ((card8 int8)
	  (maker 4))
	 ((card16 int16)
	  (maker 2))
	 ((card32 int32)
	  (maker 1)))))))

(defmacro client-message-event-get-sequence ()
  '(let* ((format (read-card8 1))
	  (sequence (make-array 20 :data-type `(unsigned-byte ,format))))
     (do ((i 12)
	  (j 0 (1+ j)))
	 ((>= i 32))
       (case format
	 (8 (setf (aref sequence j) (read-card8 i))
	    (incf i))
	 (16 (setf (aref sequence j) (read-card16 i))
	     (incf i 2))
	 (32 (setf (aref sequence j) (read-card32 i))
	     (incf i 4))))
     sequence))

(defmacro client-message-event-put-sequence (format sequence)
  `(ecase ,format
     (8  (sequence-put 12 ,sequence
		       :format card8
		       :end (min (length ,sequence) 20)
		       :appending t))
     (16 (sequence-put 12 ,sequence
		       :format card16
		       :end (min (length ,sequence) 10)
		       :appending t))
     (32 (sequence-put 12 ,sequence
		       :format card32
		       :end (min (length ,sequence) 5)
		       :appending t))))

;; Used only in declare-event
(define-accessor client-message-sequence (160)
  ((index format) index format `(client-message-event-get-sequence))
  ((index value format) index `(client-message-event-put-sequence ,format ,value)))


;;;
;;; Compound accessors
;;;    Accessors that take other accessors as parameters
;;;
(define-accessor code (0)
  ((index) index '(read-card8 0))
  ((index value) index `(write-card8 0 ,value))
  ((index value) index `(write-card8 0 ,value)))

(define-accessor length (0)
  ((index) index '(read-card16 2))
  ((index value) index `(write-card16 2 ,value))
  ((index value) index `(write-card16 2 ,value)))

(deftype data () 'card8)

(define-accessor data (0)
  ;; Put data in byte 1 of the reqeust
  ((index &optional stuff) index
   (if stuff
       (if (consp stuff)
	   `(,(getify (car stuff)) 1 ,@(cdr stuff))
	 `(,(getify stuff) 1))
     `(read-card8 1)))
  ((index thing &optional stuff)
   index
   (if stuff
       (if (consp stuff)
	   `(macrolet ((write-card32 (index value) index value))
	      (write-card8 1 (,(putify (car stuff)) ,index ,thing ,@(cdr stuff))))
	 `(,(putify stuff) 1 ,thing))
     `(write-card8 1 ,thing)))
  ((index thing &optional stuff)
   index
   (if stuff
       `(and (type? ,thing ',stuff)
	     ,(if (consp stuff)
		  `(macrolet ((write-card32 (index value) index value))
		     (write-card8 1 (,(putify (car stuff)) ,index ,thing ,@(cdr stuff))))
		`(,(putify stuff) 1 ,thing)))
     `(and (type? ,thing 'card8) (write-card8 1 ,thing)))))

;; Macroexpand the result of OR-GET to allow the macros file to not be loaded
;; when using event-case.  This is pretty gross.

(defmacro or-expand (&rest forms &environment environment)
  `(or ,@(mapcar #'(lambda (form) (macroexpand form environment)) forms)))

;;
;; the OR type
;;
(define-accessor or (32)
  ;; Select from among several types (usually NULL and something else)
  ((index &rest type-list &environment environment)
   (do ((types type-list (cdr types))
	(value (gensym))
	result)
       ((endp types)
	`(let ((,value (read-card32 ,index)))
	   (macrolet ((read-card32 (index) index ',value)
		      (read-card29 (index) index ',value))
	     ,(macroexpand `(or-expand ,@(nreverse result)) environment))))
     (let ((item (car types))
	   (args nil))
       (when (consp item)
	 (setq args (cdr item)
	       item (car item)))
       (push
	 `(,(getify item) ,index ,@args)
	 result))))

  ((index value &rest type-list)
   (do ((types type-list (cdr types))
	result)
       ((endp types)
	(if (boundp '%mask-bit)
	    `(cond ,@(nreverse result)
		   ,@(when *type-check?*
		       `((t (x-type-error ,value '(or ,@type-list))))))
	  `(write-card32 ,index
			 (macrolet ((write-card32 (index value) index value))
			   (cond ,@(nreverse result)
				 ,@(when *type-check?*
				     `((t (x-type-error ,value '(or ,@type-list))))))))))
     (let* ((type (car types))
	    (type-name type)
	    (args nil))
       (when (consp type)
	 (setq args (cdr type)
	       type-name (car type)))
       (push
	 `(,@(cond ((get type-name 'predicating-put) nil)
		   ((or *type-check?* (cdr types)) `((type? ,value ',type)))
		   (t '(t)))
	   (,(putify type-name (get type-name 'predicating-put)) ,index ,value ,@args))
	 result)))))

;;
;; the MASK type...
;;     is used to specify a subset of a collection of "optional" arguments.
;;     A mask type consists of a 32 bit mask word followed by a word for each one-bit
;;     in the mask.  The MASK type is ALWAYS the LAST item in a request.
;;
(setf (get 'mask 'byte-width) nil)

(defun mask-get (index type-values body-function)
  (declare-funarg function body-function)
  ;; This is a function, because it must return more than one form (called by get-put-items)
  ;; Functions that use this must have a binding for %MASK
  (let* ((bit 0)
	 (result
	   (mapcar
	     #'(lambda (form)
		 (if (atom form)
		     form ;; Hack to allow BODY-FUNCTION to return keyword/value pairs
		   (prog1
		     `(when (logbitp ,bit %mask)
			;; Execute form when bit is set
			,form)
		     (incf bit))))
	     (get-put-items
	       (+ index 4) type-values nil
	       #'(lambda (type index item args)
		   index ;; not used
		   (funcall body-function type '(* (incf %index) 4) item args))))))
    ;; First form must load %MASK
    `(,@(when (atom (car result))
	  (list (pop result)))
      (progn (setq %mask (read-card32 ,index))
	     (setq %index ,(ceiling index 4))
	     ,(car result))
      ,@(cdr result))))

;; MASK-PUT 

(defun mask-put (index type-values body-function)
  (declare-funarg function body-function)
  ;; The MASK type writes a 32 bit mask with 1 bits for each non-nil value in TYPE-VALUES
  ;; A 32 bit value follows for each non-nil value.
  (let ((index-var (gensym)))
    `((let ((%mask 0)
	    (,index-var
	     #+clx-overlapping-arrays
	     (index+ buffer-loffset ,(ceiling index 4)) ;; Index-var is a WORD index
	     #-clx-overlapping-arrays
	     (index+ buffer-boffset ,(lround index)) ;; Index-var is a BYTE index
	     ))
	(macrolet ((write-card32 (index value)
		     `(progn
			(setq %mask (logior %mask ,%mask-bit))
			#+clx-overlapping-arrays
			(setf (aref-card32 buffer-lbuf (incf ,index)) ,value)
			#-clx-overlapping-arrays
			(setf (aref-card32 buffer-bbuf (incf ,index 4)) ,value)))
		   (null-put (index value) index value nil))
	  ,@(let ((%bit 1))
	      (get-put-items index-var type-values t 
			     #'(lambda (type index item args)
				 (prog1 
				   `((compiler-let ((%mask-bit ,%bit))
				       ,@(funcall body-function type index item args)))
				   (setq %bit (ash %bit 1)))))))
	(write-card32 ,index %mask)
	(write-card16 2
		      #+clx-overlapping-arrays
		      (index- (incf ,index-var) buffer-loffset)
		      #-clx-overlapping-arrays
		      (ash (- (incf ,index-var 4) buffer-boffset) -2)
		      )
	(setf (buffer-boffset ,*buffer*)
	      #+clx-overlapping-arrays
	      (* ,index-var 4)
	      #-clx-overlapping-arrays
	      ,index-var
	      )))))

(define-accessor progn (nil)
  ;; Catch-all for inserting random code
  ;; Note that code using this is then responsible for setting the request length
  ((index statement) index statement)
  ((index statement) index statement))


;
; Wrapper macros, for use around the above
;
(defvar *inhibit-type-checking* nil) ;; Bind with compiler-let to inhibit type checking

(defmacro type-check (value type)
  (when *type-check?*
    `(unless (type? ,value ,type)
       (x-type-error ,value ,type))))

(defmacro check-put (index value type &rest args)
  `(,@(if (consp value) ;; Make local binding when value is an expression
	  (let ((val (gensym)))
	    (prog1 `(let ((,val ,value)))
		   (setq value val)))
	'(progn))
    ,(if (or (not *type-check?*) (member type '(or progn pad8 pad16))
	     (constantp value) *inhibit-type-checking*)
	 `(,(putify type) ,index ,value ,@args)
       ;; Do type checking
       (if (get type 'predicating-put)
	   `(or (,(putify type t) ,index ,value ,@args)
		(x-type-error ,value ',(if args `(,type ,@args) type)))
	 `(if (type? ,value ',type)
	      (,(putify type) ,index ,value ,@args)
	    (x-type-error ,value ',(if args `(,type ,@args) type)))))))

(defun get-put-items (index type-args putp &optional body-function)
  (declare-funarg (or null function) body-function)
  ;; Given a lists of the form (type item item ... item)
  ;; Calls body-function with four arguments, a function name,
  ;; index, item name, and optional arguments.
  ;; The results are appended together and retured.
  (unless body-function
    (setq body-function
	  #'(lambda (type index item args)
	      `((check-put ,index ,item ,type ,@args)))))
  (do* ((items type-args (cdr items))
	(type (caar items) (caar items))
	(args nil nil)
	(result nil)
	(sizes nil))
       ((endp items) (values result index sizes))
    (when (consp type)
      (setq args (cdr type)
	    type (car type)))
    (cond ((member type '(return buffer)))
	  ((eq type 'mask) ;; Hack to enable mask-get/put to return multiple values
	   (setq result
		 (append result (if putp
				    (mask-put index (cdar items) body-function)
				  (mask-get index (cdar items) body-function)))
		 index nil))
	  (t (do* ((item (cdar items) (cdr item))
;;		   (FUNCTION (IF putp (putify type) (getify type)))
		   (increment (index-increment type)))
		  ((endp item))
	       (when (constantp index)
		 (case increment		;Round up index when needed
		   (2 (setq index (wround index)))
		   (4 (setq index (lround index)))))
	       (setq result
		     (append result (funcall body-function type index (car item) args)))
	       (when (constantp index)
		 ;; Variable length requests have null length increment.
		 ;; Variable length requests set the request size 
		 ;; & maintain buffer pointers
		 (if (null increment) 
		     (setq index nil)
		   (progn
		     (incf index increment)
		     (when (and increment (zerop increment)) (setq increment 1))
		     (pushnew (* increment 8) sizes)))))))))

(defmacro with-buffer-request ((buffer opcode &rest options) &body type-args)
  (let ((*buffer* (gensym))
	(gc-force (second (member :gc-force options)))
	(length (second (member :length options))))
    (multiple-value-bind (code index sizes)
	(get-put-items 4 type-args t)
      `(let ((,*buffer* ,buffer))
	 (with-buffer (,*buffer*)
	   ,@(when gc-force `((force-gcontext-changes ,gc-force)))
	   (writing-buffer-send (,*buffer* :length ,length
				 :sizes (8 16 ,@sizes ,@(cadr (member :sizes options))))
	     (setf (buffer-last-request ,*buffer*) buffer-boffset)
	     (write-card8 0 ,opcode)	   ;; Stick in the opcode
	     ,@code
	     ,@(when index
		 (setq index (lround index))
		 `((write-card16 2 ,(ceiling index 4))
		   (setf (buffer-boffset ,*buffer*) (+ buffer-boffset ,index))))
	     (incf (buffer-request-number ,*buffer*))))
	 ,@(unless (member :no-after options)
	     `((display-invoke-after-function ,*buffer*)))))))

(defmacro with-buffer-reply ((buffer &optional size &rest options) &body body)
  ;; Generate code for returning values
  (declare-arglist (buffer &optional size &key sizes) &body body)
  (let ((buf (gensym)))
    `(let ((,buf ,buffer))
       (wait-for-reply ,buf ,size)
       (reading-buffer-reply (,buf ,@options)
	 ,@body))))

(defmacro compare-request ((index) &body body)
  `(macrolet ((write-card32 (index item) `(= ,item (read-card32 ,index)))
	      (write-int32 (index item) `(= ,item (read-int32 ,index)))
	      (write-card29 (index item) `(= ,item (read-card29 ,index)))
	      (write-int29 (index item) `(= ,item (read-int29 ,index)))
	      (write-card16 (index item) `(= ,item (read-card16 ,index)))
	      (write-int16 (index item) `(= ,item (read-int16 ,index)))
	      (write-card8 (index item) `(= ,item (read-card8 ,index)))
	      (write-int8 (index item) `(= ,item (read-int8 ,index))))
     (compiler-let ((*inhibit-type-checking* t))
       (and ,@(get-put-items index body t)))))

(defmacro put-items ((index) &body body)
  `(progn ,@(get-put-items index body t)))

(defmacro decode-type (type value)
  ;; Given an integer and type, return the value
  (let ((args nil))
    (when (consp type)
      (setq args (cdr type)
	    type (car type)))
    `(macrolet ((read-card29 (value) value)
		(read-card32 (value) value)
		(read-int32 (value) `(card32->int32 ,value))
		(read-card16 (value) value)
		(read-int16 (value) `(card16->int16 ,value))
		(read-card8 (value) value)
		(read-int8 (value) `(int8->card8 ,value)))
       (,(getify type) ,value ,@args))))

(defmacro encode-type (type value)
  ;; Given a value and type, return an integer
  ;; When check-p, do type checking on value
  (let ((args nil))
    (when (consp type)
      (setq args (cdr type)
	    type (car type)))
    `(macrolet ((write-card29 (index value) index value)
		(write-card32 (index value) index value)
		(write-int32 (index value) index `(int32->card32 ,value))
		(write-card16 (index value) index value)
		(write-int16 (index value) index `(int16->card16 ,value))
		(write-card8 (index value) index value)
		(write-int8 (index value) index `(int8->card8 ,value)))
       (check-put 0 ,value ,type ,@args))))

(defmacro set-decode-type (type accessor value)
  `(setf ,accessor (encode-type ,type ,value)))
(defsetf decode-type set-decode-type)


;;;
;;; Request codes
;;; 

(defconstant *x-createwindow*                  1)
(defconstant *x-changewindowattributes*        2)
(defconstant *x-getwindowattributes*           3)
(defconstant *x-destroywindow*                 4)
(defconstant *x-destroysubwindows*             5)  
(defconstant *x-changesaveset*                 6)
(defconstant *x-reparentwindow*                7)
(defconstant *x-mapwindow*                     8)
(defconstant *x-mapsubwindows*                 9)
(defconstant *x-unmapwindow*                  10)
(defconstant *x-unmapsubwindows*              11) 
(defconstant *x-configurewindow*              12)
(defconstant *x-circulatewindow*              13)
(defconstant *x-getgeometry*                  14)
(defconstant *x-querytree*                    15)
(defconstant *x-internatom*                   16)
(defconstant *x-getatomname*                  17)
(defconstant *x-changeproperty*               18)
(defconstant *x-deleteproperty*               19)
(defconstant *x-getproperty*                  20)
(defconstant *x-listproperties*               21)
(defconstant *x-setselectionowner*            22)  
(defconstant *x-getselectionowner*            23) 
(defconstant *x-convertselection*             24)
(defconstant *x-sendevent*                    25)
(defconstant *x-grabpointer*                  26)
(defconstant *x-ungrabpointer*                27)
(defconstant *x-grabbutton*                   28)
(defconstant *x-ungrabbutton*                 29)
(defconstant *x-changeactivepointergrab*      30)         
(defconstant *x-grabkeyboard*                 31)
(defconstant *x-ungrabkeyboard*               32)
(defconstant *x-grabkey*                      33)
(defconstant *x-ungrabkey*                    34)
(defconstant *x-allowevents*                  35)
(defconstant *x-grabserver*                   36)     
(defconstant *x-ungrabserver*                 37)       
(defconstant *x-querypointer*                 38)       
(defconstant *x-getmotionevents*              39)          
(defconstant *x-translatecoords*              40)               
(defconstant *x-warppointer*                  41)      
(defconstant *x-setinputfocus*                42)        
(defconstant *x-getinputfocus*                43)        
(defconstant *x-querykeymap*                  44)      
(defconstant *x-openfont*                     45)   
(defconstant *x-closefont*                    46)    
(defconstant *x-queryfont*                    47)
(defconstant *x-querytextextents*             48)    
(defconstant *x-listfonts*                    49) 
(defconstant *x-listfontswithinfo*    	      50)
(defconstant *x-setfontpath*                  51)
(defconstant *x-getfontpath*                  52)
(defconstant *x-createpixmap*                 53)      
(defconstant *x-freepixmap*                   54)   
(defconstant *x-creategc*                     55)
(defconstant *x-changegc*                     56)
(defconstant *x-copygc*                       57)
(defconstant *x-setdashes*                    58)  
(defconstant *x-setcliprectangles*            59)         
(defconstant *x-freegc*                       60)
(defconstant *x-cleartobackground*            61)          
(defconstant *x-copyarea*                     62)
(defconstant *x-copyplane*                    63)
(defconstant *x-polypoint*                    64)
(defconstant *x-polyline*                     65)
(defconstant *x-polysegment*                  66)  
(defconstant *x-polyrectangle*                67)   
(defconstant *x-polyarc*                      68)
(defconstant *x-fillpoly*                     69)
(defconstant *x-polyfillrectangle*            70)        
(defconstant *x-polyfillarc*                  71) 
(defconstant *x-putimage*                     72)
(defconstant *x-getimage*                     73)
(defconstant *x-polytext8*                    74)   
(defconstant *x-polytext16*                   75)   
(defconstant *x-imagetext8*                   76)  
(defconstant *x-imagetext16*                  77)  
(defconstant *x-createcolormap*               78)    
(defconstant *x-freecolormap*                 79) 
(defconstant *x-copycolormapandfree*          80)       
(defconstant *x-installcolormap*              81)  
(defconstant *x-uninstallcolormap*            82)   
(defconstant *x-listinstalledcolormaps*       83)       
(defconstant *x-alloccolor*                   84)
(defconstant *x-allocnamedcolor*              85)    
(defconstant *x-alloccolorcells*              86)   
(defconstant *x-alloccolorplanes*             87)   
(defconstant *x-freecolors*                   88)
(defconstant *x-storecolors*                  89)
(defconstant *x-storenamedcolor*              90)   
(defconstant *x-querycolors*                  91)
(defconstant *x-lookupcolor*                  92)
(defconstant *x-createcursor*                 93)
(defconstant *x-createglyphcursor*            94)    
(defconstant *x-freecursor*                   95)
(defconstant *x-recolorcursor*                96)  
(defconstant *x-querybestsize*                97) 
(defconstant *x-queryextension*               98) 
(defconstant *x-listextensions*               99)
(defconstant *x-setkeyboardmapping*           100)
(defconstant *x-getkeyboardmapping*           101)
(defconstant *x-changekeyboardcontrol*        102)               
(defconstant *x-getkeyboardcontrol*           103)           
(defconstant *x-bell*                         104)
(defconstant *x-changepointercontrol*         105)
(defconstant *x-getpointercontrol*            106)
(defconstant *x-setscreensaver*               107)         
(defconstant *x-getscreensaver*               108)        
(defconstant *x-changehosts*                  109)    
(defconstant *x-listhosts*                    110) 
(defconstant *x-changeaccesscontrol*          111)          
(defconstant *x-changeclosedownmode*          112)
(defconstant *x-killclient*                   113)
(defconstant *x-rotateproperties*	      114)
(defconstant *x-forcescreensaver*	      115)
(defconstant *x-setpointermapping*            116)
(defconstant *x-getpointermapping*            117)
(defconstant *x-setmodifiermapping*	      118)
(defconstant *x-getmodifiermapping*	      119)
(defconstant *x-nooperation*                  127)
