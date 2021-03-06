;;; -*- Mode: LISP; Syntax: Common-lisp; Package: XLIB; Base: 10; Lowercase: Yes -*-

;;; GContext

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

;;;	GContext values are usually cached locally in the GContext object.
;;;	This is required because the X.11 server doesn't have any requests
;;;	for getting GContext values back.
;;;
;;;	GContext changes are cached until force-GContext-changes is called.
;;;	All the requests that use GContext (including the GContext accessors,
;;;	but not the SETF's) call force-GContext-changes.
;;;	In addition, the macro WITH-GCONTEXT may be used to provide a 
;;;	local view if a GContext.
;;;
;;;	Each GContext keeps a copy of the values the server has seen, and
;;;	a copy altered by SETF, called the LOCAL-STATE (bad name...).
;;;	The SETF accessors increment a timestamp in the GContext.
;;;	When the timestamp in a GContext isn't equal to the timestamp in
;;;	the local-state, changes have been made, and force-GContext-changes
;;;	loops through the GContext and local-state, sending differences to
;;;	the server, and updating GContext.
;;;
;;;	WITH-GCONTEXT works by BINDING the local-state slot in a GContext to
;;;	a private copy.  This is easy (and fast) for lisp machines, but other
;;;	lisps will have problems.  Fortunately, most other lisps don't care,
;;;	because they don't run in a multi-processing shared-address space
;;;	environment.

(in-package "XLIB" :use '("LISP"))

;; GContext state accessors
;;	The state vector contains all card32s to speed server updating

(eval-when (eval compile load)

(defconstant *gcontext-fast-change-length* #.(length *gcontext-components*))

)

(eval-when (eval compile)

(defmacro def-gc-internals (name &rest extras)
  (let ((macros nil)
	(indexes nil)
	(masks nil)
	(index 0))
    (dolist (name *gcontext-components*)
      (push `(defmacro ,(xintern "GCONTEXT-INTERNAL-" name) (state)
	       `(svref ,state ,,index))
	    macros)
      (setf (getf indexes name) index)
      (push (ash 1 index) masks)
      (incf index))
    (dolist (extra extras)
      (push `(defmacro ,(xintern "GCONTEXT-INTERNAL-" (first extra)) (state)
	       `(svref ,state ,,index))
	    macros)
      (setf (getf indexes (or (second extra) (first extra))) index)
      (push (logior (ash 1 index)
		    (if (second extra)
			(ash 1 (position (second extra) *gcontext-components*))
			0))
	    masks)
      (incf index))
    `(within-definition (def-gc-internals ,name)
       ,@(nreverse macros)
       (eval-when (eval compile load)
	 (defconstant *gcontext-data-length* ,index)
	 (defconstant *gcontext-indexes* ',indexes)
	 (defconstant *gcontext-masks* ',(coerce (nreverse masks) 'simple-vector))))))

)

(def-gc-internals ignore
		  (:clip :clip-mask) (:dash :dashes) (:font-obj :font) (:timestamp))

(deftype gcmask () '(unsigned-byte #.*gcontext-fast-change-length*))

(deftype xgcmask () '(unsigned-byte #.*gcontext-data-length*))

;; Gcontext state Resource
(defvar *gcontext-local-state-cache* nil) ;; List of unused gcontext local states

(defun allocate-gcontext-state ()
  (let ((state (or (atomic-pop *gcontext-local-state-cache*)
		   (make-array *gcontext-data-length*))))
    (declare-array gcontext-state state)
    (fill (the gcontext-state state) nil)
    state))

(defun deallocate-gcontext-state (state)
  (atomic-push state *gcontext-local-state-cache*))

;; For each argument to create-gcontext (except clip-mask and clip-ordering) declared
;; as (type <type> <name>), there is an accessor:

;(defun gcontext-<name> (gcontext)
;  ;; The value will be nil if the last value stored is unknown (e.g., the cache was
;  ;; off, or the component was copied from a gcontext with unknown state).
;  (declare (type gcontext gcontext)
;	   (values <type>)))

;; For each argument to create-gcontext (except clip-mask and clip-ordering) declared
;; as (type (or null <type>) <name>), there is a setf for the corresponding accessor:

;(defsetf gcontext-<name> (gcontext) (value)
;  )

;; Generate all the accessors and defsetf's for GContext

(eval-when (eval compile)  ;; I'd rather use macrolet, but Symbolics doesn't like it...

(defmacro xgcmask->gcmask (mask)
  `(logand ,mask #.(1- (ash 1 *gcontext-fast-change-length*))))

(defmacro access-gcontext ((gcontext local-state) &body body)
  `(let ((,local-state (gcontext-local-state ,gcontext)))
     (declare (type gcontext-state ,local-state))
     ,@body))

(defmacro modify-gcontext ((gcontext local-state) &body body)
  ;; The timestamp must be altered after the modification
  `(let ((,local-state (gcontext-local-state ,gcontext)))
     (declare (type gcontext-state ,local-state))
     (prog1
	 (progn ,@body)
       (setf (gcontext-internal-timestamp ,local-state) 0))))

(defmacro def-gc-accessor (name type)
  (let* ((gcontext-name (xintern "GCONTEXT-" name))
	 (internal-accessor (xintern "GCONTEXT-INTERNAL-" name))
	 (internal-setfer (xintern "SET-" gcontext-name)))
    `(within-definition (,name def-gc-accessor)
       (compiler-let ((*buffer* '(gcontext-display gcontext)))

	 (defun ,gcontext-name (gcontext)
	   (declare (type gcontext gcontext))
	   (declare-values (or null ,type))
	   (let ((value (,internal-accessor (gcontext-local-state gcontext))))
	     (declare (type (or null card32) value))
	     (when value ;; Don't do anything when value isn't known
	       (decode-type ,type value))))
       
	 (defun ,internal-setfer (gcontext value)
	   (declare (type gcontext gcontext)
		    (type ,type value))
	   (modify-gcontext (gcontext local-state)
	     (setf (,internal-accessor local-state) (encode-type ,type value))
	     value))
       
	 (defsetf ,gcontext-name ,internal-setfer)))))

(defmacro incf-internal-timestamp (state)
  (let ((ts (gensym)))
    `(let ((,ts (the fixnum (gcontext-internal-timestamp ,state))))
       (declare (type fixnum ,ts))
       ;; the probability seems low enough
       (setq ,ts (if (= ,ts most-positive-fixnum)
		     1
		     (the fixnum (1+ ,ts))))
       (setf (gcontext-internal-timestamp ,state) ,ts))))

) ;; End eval-when

(def-gc-accessor function boole-constant)
(def-gc-accessor plane-mask card32)
(def-gc-accessor foreground card32)
(def-gc-accessor background card32)
(def-gc-accessor line-width card16)
(def-gc-accessor line-style (member :solid :dash :double-dash))
(def-gc-accessor cap-style (member :not-last :butt :round :projecting))
(def-gc-accessor join-style (member :miter :round :bevel))
(def-gc-accessor fill-style (member :solid :tiled :stippled :opaque-stippled))
(def-gc-accessor fill-rule (member :even-odd :winding))
(def-gc-accessor tile pixmap)
(def-gc-accessor stipple pixmap)
(def-gc-accessor ts-x int16) ;; Tile-Stipple-X-origin
(def-gc-accessor ts-y int16) ;; Tile-Stipple-Y-origin
;; (def-GC-accessor font font) ;; See below
(def-gc-accessor subwindow-mode (member :clip-by-children :include-inferiors))
(def-gc-accessor exposures (member :off :on))
(def-gc-accessor clip-x int16)
(def-gc-accessor clip-y int16)
;; (def-GC-accessor clip-mask) ;; see below
(def-gc-accessor dash-offset card16)
;; (def-GC-accessor dashes)  ;; see below
(def-gc-accessor arc-mode (member :chord :pie-slice))


(defun gcontext-clip-mask (gcontext)
  (declare (type gcontext gcontext))
  (declare-values (or null (member :none) pixmap rect-seq)
		  (or null (member :unsorted :y-sorted :yx-sorted :yx-banded)))
  (access-gcontext (gcontext local-state)
    (compiler-let ((*buffer* '(gcontext-display gcontext)))
      (let ((clip (gcontext-internal-clip local-state)))
	(values (or (third clip)
		    (decode-type (or (member :none) pixmap) (second clip)))
		(decode-type (or null (member :unsorted :y-sorted :yx-sorted :yx-banded))
			     (first clip)))))))

(defsetf gcontext-clip-mask (gcontext &optional ordering) (clip-mask)
  ;; A bit strange, but retains setf form.
  ;; a nil clip-mask is transformed to an empty vector
  `(set-gcontext-clip-mask ,gcontext ,ordering ,clip-mask))

(defun set-gcontext-clip-mask (gcontext ordering clip-mask)
  ;; a nil clip-mask is transformed to an empty vector
  (declare (type gcontext gcontext)
	   (type (or null (member :unsorted :y-sorted :yx-sorted :yx-banded)) ordering)
	   (type (or (member :none) pixmap rect-seq) clip-mask))
  (modify-gcontext (gcontext local-state)
    ;; need single setf for atomicity
    (setf (gcontext-internal-clip local-state)
	  (cons (encode-type (or null (member :unsorted :y-sorted :yx-sorted :yx-banded))
			     ordering)
		(if (type? clip-mask 'sequence)
		    (list nil (or clip-mask (vector)))
		    (list (encode-type (or (member :none) pixmap) clip-mask)))))
    clip-mask))

(defun gcontext-dashes (gcontext)
  (declare (type gcontext gcontext))
  (declare-values (or null card8 sequence))
  (access-gcontext (gcontext local-state)
    (let ((dash (gcontext-internal-dash local-state)))
      (or (first dash) (second dash)))))

(defsetf gcontext-dashes set-gcontext-dashes)

(defun set-gcontext-dashes (gcontext dashes)
  (declare (type gcontext gcontext)
	   (type (or card8 sequence) dashes))
  (modify-gcontext (gcontext local-state)
    ;; need single setf for atomicity
    (setf (gcontext-internal-dash local-state)
	  (if (type? dashes 'sequence)
	      (list nil (or dashes (vector)))
	      (list (encode-type card8 dashes))))
    dashes))

(defun gcontext-font (gcontext &optional metrics-p)
  ;; If the stored font is known, it is returned.  If it is not known and
  ;; metrics-p is false, then nil is returned.  If it is not known and
  ;; metrics-p is true, then a pseudo font is returned.  Full metric and
  ;; property information can be obtained, but the font does not have a name or
  ;; a resource-id, and attempts to use it where a resource-id is required will
  ;; result in an invalid-font error.
  (declare (type gcontext gcontext)
	   (type boolean metrics-p))
  (declare-values (or null font))
  (access-gcontext (gcontext local-state)
    (let ((font (gcontext-internal-font-obj local-state)))
      (or font
	  (when metrics-p
	    ;; XXX this isn't correct
	    (make-font :display (gcontext-display gcontext)
		       :id (gcontext-id gcontext)
		       :name nil))))))

(defsetf gcontext-font set-gcontext-font)

(defun set-gcontext-font (gcontext font)
  (declare (type gcontext gcontext)
	   (type fontable font))
    (let ((font-object (if (font-p font) font (open-font (gcontext-display gcontext) font))))
      ;; XXX need to check font has id (and name?)
      (modify-gcontext (gcontext local-state)
	(setf (gcontext-internal-font-obj local-state) font-object)
	font)))

(defun force-gcontext-changes (gcontext)
  ;; Force any delayed changes.
  (declare (type gcontext gcontext))
  (let ((display (gcontext-display gcontext))
	(server-state (gcontext-server-state gcontext))
	(local-state (gcontext-local-state gcontext))
	local-clip local-dash)
    (declare-array gcontext-state server-state local-state)
      ;; Update server when timestamps don't match
    (unless (= (the fixnum (gcontext-internal-timestamp local-state))
	       (the fixnum (gcontext-internal-timestamp server-state)))
      (with-display (display)
	;; first unpack a few things
	(setq local-clip (gcontext-internal-clip local-state))
	(if (setf (gcontext-internal-clip-mask local-state) (second local-clip))
	    (setf (gcontext-internal-clip server-state) local-clip)
	    (setf (gcontext-internal-clip-mask server-state) nil))
	(setq local-dash (gcontext-internal-dash local-state))
	(if (setf (gcontext-internal-dashes local-state) (first local-dash))
	    (setf (gcontext-internal-dash server-state) local-dash)
	    (setf (gcontext-internal-dashes server-state) nil))
	(let ((local-font (gcontext-internal-font-obj local-state)))
	  (if local-font
	      (progn
		(setf (gcontext-internal-font local-state) (font-id local-font))
		(setf (gcontext-internal-font-obj server-state) local-font))
	      (setf (gcontext-internal-font server-state) nil)))
	  
	;; Because there is no locking on the local state we have to
	;; assume that state will change and set timestamps up front,
	;; otherwise by the time we figured out there were no changes
	;; and tried to store the server stamp as the local stamp, the
	;; local stamp might have since been modified.
	(setf (gcontext-internal-timestamp local-state)
	      (incf-internal-timestamp server-state))

	(block no-changes
	  (let ((last-request (buffer-last-request display)))
	    (with-buffer-request (display *x-changegc*)
	      (gcontext gcontext)
	      (progn (do ((i 0 (index+ i 1))
			  (bit 1 (the gcmask (ash bit 1)))
			  (nbyte 12)
			  (mask 0)
			  (local 0))
			 ((index>= i *gcontext-fast-change-length*)
			  (when (zerop mask)
			    ;; If nothing changed, restore last-request and quit
			    (setf (buffer-last-request display)
				  (if (zerop (buffer-last-request display))
				      nil
				      last-request))
			    (return-from no-changes nil))
			  (card32-put 8 mask)
			  (card16-put 2 (index-ash nbyte -2))
			  (index-incf (buffer-boffset display) nbyte))
		       (declare (type array-index i nbyte)
				(type gcmask bit mask)
				(type (or null card32) local))
		       (unless (eql (the (or null card32) (svref server-state i))
				    (setq local (the (or null card32) (svref local-state i))))
			 (setf (svref server-state i) local)
			 (card32-put nbyte local)
			 (setq mask (the gcmask (logior mask bit)))
			 (index-incf nbyte 4)))))))
	(unless (eq (third local-clip)
		    (third (gcontext-internal-clip server-state)))
	  (setf (gcontext-internal-clip server-state) local-clip)
	  (with-buffer-request (display *x-setcliprectangles*)
	    (data (first local-clip))
	    (gcontext gcontext)
	    ;; XXX treat nil correctly
	    (card16 (or (gcontext-internal-clip-x local-state) 0)
		    (or (gcontext-internal-clip-y local-state) 0))
	    ;; XXX this has both int16 and card16 values
	    ((sequence :format int16) (third local-clip))))
	(unless (eq (second local-dash)
		    (second (gcontext-internal-dash server-state)))
	  (setf (gcontext-internal-dash server-state) local-dash)
	  (with-buffer-request (display *x-setdashes*)
	    (gcontext gcontext)
	    ;; XXX treat nil correctly
	    (card16 (or (gcontext-internal-dash-offset local-state) 0)
		    (length (second local-dash)))
	    ((sequence :format card8) (second local-dash))))))))

;;; WARNING: WITH-GCONTEXT WORKS MUCH MORE EFFICIENTLY WHEN THE OPTIONS BEING "BOUND" ARE
;;;	     SET IN THE GCONTEXT ON ENTRY.  BECAUSE THERE'S NO WAY TO GET THE VALUE OF AN
;;;	     UNKNOWN GC COMPONENT, WITH-GCONTEXT MUST CREATE A TEMPORARY GC, COPY THE UNKNOWN
;;;	     COMPONENTS TO THE TEMPORARY GC, ALTER THE GC BEING USED, THEN COPY COMPOMENTS
;;;          BACK.

(defmacro with-gcontext ((gcontext &rest options &key
			  function plane-mask foreground background
			  line-width line-style cap-style join-style fill-style fill-rule
			  arc-mode tile stipple ts-x ts-y font subwindow-mode
			  exposures clip-x clip-y clip-mask clip-ordering
			  dash-offset dashes)
			 &body body)
  ;; "Binds" the gcontext components specified by options within the
  ;; dynamic scope of the body (i.e., indefinite scope and dynamic
  ;; extent), on a per-process basis in a multi-process environment.
  ;; The body is not surrounded by a with-display.  If cache-p is nil or
  ;; the some component states are unknown, this will implement
  ;; save/restore by creating a temporary gcontext and doing
  ;; copy-gcontext-components to and from it.

  (declare (ignore function plane-mask foreground background
		   line-width line-style cap-style join-style fill-style fill-rule
		   arc-mode tile stipple ts-x ts-y font subwindow-mode
		   exposures clip-x clip-y clip-mask dash-offset dashes))

  (remf options :clip-ordering)

  (let ((gc (gensym))
	(saved-state (gensym))
	(temp-gc (gensym))
	(temp-mask (gensym))
	(setfs nil)
	(indexes nil)
	(ts-index (getf *gcontext-indexes* :timestamp)))

    (do* ((option options (cddr option))
	  (name (car option) (car option)))
	 ((endp option) (setq setfs (nreverse setfs)))
      (push `(setf (,(xintern "GCONTEXT-" name) ,gc ,@(when (eq name :clip-mask)
							`(,clip-ordering)))
		   ,(cadr option))
	    setfs)
      (push (getf *gcontext-indexes* name) indexes))
    
    (if setfs
	`(multiple-value-bind (,gc ,saved-state ,temp-mask ,temp-gc)
	     (copy-gcontext-local-state ,gcontext ',indexes)
	   (declare (type gcontext ,gc)
		    (type gcontext-state ,saved-state)
		    (type xgcmask ,temp-mask)
		    (type (or null resource-id) ,temp-gc))
	   (with-gcontext-bindings (,gc ,saved-state ,indexes ,ts-index ,temp-mask ,temp-gc)
	     ,@setfs
	     ,@body))
      `(progn ,@body))))

(defun copy-gcontext-local-state (gcontext indexes)
  ;; Called from WITH-GCONTEXT to save the fields in GCONTEXT indicated by MASK
  (declare (type gcontext gcontext)
	   (type list indexes))
  (let ((local-state (gcontext-local-state gcontext))
	(saved-state (allocate-gcontext-state)))
    (declare-array gcontext-state local-state saved-state)
    (do ((indexes indexes (cdr indexes))
	 (i 0)
	 (temp-gc nil)
	 (temp-mask 0))
	((endp indexes)
	 (when (or (plusp temp-mask) (not (gcontext-cache-p gcontext)))
	   ;; Copy to temporary GC when field unknown or cache-p false
	   (let ((display (gcontext-display gcontext)))
	     (declare (type display display))
	     (with-display (display)
	       (setq temp-gc (allocate-resource-id display gcontext 'gcontext))
	       ;; Create a new (temporary) gcontext
	       (with-buffer-request (display *x-creategc*)
		 (resource-id temp-gc)
		 (drawable (gcontext-drawable gcontext))
		 (card32 0))
	       ;; Copy changed components to the temporary gcontext
	       (with-buffer-request (display *x-copygc*)
		 (gcontext gcontext)
		 (resource-id temp-gc)
		 (card32 (xgcmask->gcmask temp-mask))))))
	 (values gcontext saved-state temp-mask temp-gc))
      (declare (type xgcmask temp-mask)
	       (type list indexes)
	       (type array-index i)
	       (type (or null resource-id) temp-gc))
      (setq i (car indexes))
      (unless (setf (svref saved-state i) (svref local-state i))
	(setq temp-mask
	      (the xgcmask (logior temp-mask
				   (the xgcmask (svref *gcontext-masks* i)))))))))

(defun restore-gcontext-temp-state (gcontext temp-mask temp-gc)
  (declare (type gcontext gcontext)
	   (type xgcmask temp-mask)
	   (type resource-id temp-gc))
  (let ((display (gcontext-display gcontext)))
    (declare (type display display))
    (with-display (display)
      (with-buffer-request (display *x-copygc*)
	(resource-id temp-gc)
	(gcontext gcontext)
	(card32 (xgcmask->gcmask temp-mask)))
      (with-buffer-request (display *x-freegc*) ;; free gcontext
	(resource-id temp-gc))
      (deallocate-resource-id display temp-gc 'gcontext)
      (do ((server-state (gcontext-server-state gcontext))
	   (bit temp-mask (the xgcmask (ash bit -1)))
	   (i 0 (index+ i 1)))
	  ((zerop bit)
	   (incf-internal-timestamp server-state))
	(declare (type gcontext-state server-state)
		 (type xgcmask bit)
		 (type array-index i))
	(when (oddp bit)
	  (setf (svref server-state i) nil))))))

(defun create-gcontext (&key (drawable (required-arg drawable))
			function plane-mask foreground background
			line-width line-style cap-style join-style fill-style fill-rule
			arc-mode tile stipple ts-x ts-y font subwindow-mode
			exposures clip-x clip-y clip-mask clip-ordering
			dash-offset dashes
			(cache-p t))
  ;; Only non-nil components are passed on in the request, but for effective caching
  ;; assumptions have to be made about what the actual protocol defaults are.  For
  ;; all gcontext components, a value of nil causes the default gcontext value to be
  ;; used.  For clip-mask, this implies that an empty rect-seq cannot be represented
  ;; as a list.  Note:  use of stringable as font will cause an implicit open-font.
  ;; Note:  papers over protocol SetClipRectangles and SetDashes special cases.  If
  ;; cache-p is true, then gcontext state is cached locally, and changing a gcontext
  ;; component will have no effect unless the new value differs from the cached
  ;; value.  Component changes (setfs and with-gcontext) are always deferred
  ;; regardless of the cache mode, and sent over the protocol only when required by a
  ;; local operation or by an explicit call to force-gcontext-changes.
  (declare (type drawable drawable) ; Required to be non-null
	   (type (or null boole-constant) function)
	   (type (or null pixel) plane-mask foreground background)
	   (type (or null card16) line-width dash-offset)
	   (type (or null int16) ts-x ts-y clip-x clip-y)
	   (type (or null (member :solid :dash :double-dash)) line-style)
	   (type (or null (member :not-last :butt :round :projecting)) cap-style)
	   (type (or null (member :miter :round :bevel)) join-style)
	   (type (or null (member :solid :tiled :opaque-stippled :stippled)) fill-style)
	   (type (or null (member :even-odd :winding)) fill-rule)
	   (type (or null (member :chord :pie-slice)) arc-mode)
	   (type (or null pixmap) tile stipple)
	   (type (or null fontable) font)
	   (type (or null (member :clip-by-children :include-inferiors)) subwindow-mode)
	   (type (or null (member :on :off)) exposures)
	   (type (or null (member :none) pixmap rect-seq) clip-mask)
	   (type (or null (member :unsorted :y-sorted :yx-sorted :yx-banded)) clip-ordering)
	   (type (or null card8 sequence) dashes)
	   (type boolean cache-p))
  (declare-values gcontext)
  (let* ((display (drawable-display drawable))
	 (gcontext (make-gcontext :display display :drawable drawable :cache-p cache-p))
	 (local-state (gcontext-local-state gcontext))
	 (server-state (gcontext-server-state gcontext))
	 (gcontextid (allocate-resource-id display gcontext 'gcontext)))
    (declare (type display display)
	     (type gcontext gcontext)
	     (type resource-id gcontextid))
    (declare-array gcontext-state local-state server-state)
    (setf (gcontext-id gcontext) gcontextid)

    (unless function (setf (gcontext-function gcontext) boole-1))
    ;; using the depth of the drawable would be better, but ...
    (unless plane-mask (setf (gcontext-plane-mask gcontext) #xffffffff))
    (unless foreground (setf (gcontext-foreground gcontext) 0))
    (unless background (setf (gcontext-background gcontext) 1))
    (unless line-width (setf (gcontext-line-width gcontext) 0))
    (unless line-style (setf (gcontext-line-style gcontext) :solid))
    (unless cap-style (setf (gcontext-cap-style gcontext) :butt))
    (unless join-style (setf (gcontext-join-style gcontext) :miter))
    (unless fill-style (setf (gcontext-fill-style gcontext) :solid))
    (unless fill-rule (setf (gcontext-fill-rule gcontext) :even-odd))
    (unless arc-mode (setf (gcontext-arc-mode gcontext) :pie-slice))
    (unless ts-x (setf (gcontext-ts-x gcontext) 0))
    (unless ts-y (setf (gcontext-ts-y gcontext) 0))
    (unless subwindow-mode (setf (gcontext-subwindow-mode gcontext)
				 :clip-by-children))
    (unless exposures (setf (gcontext-exposures gcontext) :on))
    (unless clip-mask (setf (gcontext-clip-mask gcontext) :none))
    (unless clip-x (setf (gcontext-clip-x gcontext) 0))
    (unless clip-y (setf (gcontext-clip-y gcontext) 0))
    (unless dashes (setf (gcontext-dashes gcontext) 4))
    (unless dash-offset (setf (gcontext-dash-offset gcontext) 0))
    ;; a bit kludgy, but ...
    (replace server-state local-state)

    (when function (setf (gcontext-function gcontext) function))
    (when plane-mask (setf (gcontext-plane-mask gcontext) plane-mask))
    (when foreground (setf (gcontext-foreground gcontext) foreground))
    (when background (setf (gcontext-background gcontext) background))
    (when line-width (setf (gcontext-line-width gcontext) line-width))
    (when line-style (setf (gcontext-line-style gcontext) line-style))
    (when cap-style (setf (gcontext-cap-style gcontext) cap-style))
    (when join-style (setf (gcontext-join-style gcontext) join-style))
    (when fill-style (setf (gcontext-fill-style gcontext) fill-style))
    (when fill-rule (setf (gcontext-fill-rule gcontext) fill-rule))
    (when arc-mode (setf (gcontext-arc-mode gcontext) arc-mode))
    (when tile (setf (gcontext-tile gcontext) tile))
    (when stipple (setf (gcontext-stipple gcontext) stipple))
    (when ts-x (setf (gcontext-ts-x gcontext) ts-x))
    (when ts-y (setf (gcontext-ts-y gcontext) ts-y))
    (when font (setf (gcontext-font gcontext) font))
    (when subwindow-mode (setf (gcontext-subwindow-mode gcontext) subwindow-mode))
    (when exposures (setf (gcontext-exposures gcontext) exposures))
    (when clip-x (setf (gcontext-clip-x gcontext) clip-x))
    (when clip-y (setf (gcontext-clip-y gcontext) clip-y))
    (when clip-mask (setf (gcontext-clip-mask gcontext clip-ordering) clip-mask))
    (when dash-offset (setf (gcontext-dash-offset gcontext) dash-offset))
    (when dashes (setf (gcontext-dashes gcontext) dashes))
    
    (setf (gcontext-internal-timestamp server-state) 1)
    (setf (gcontext-internal-timestamp local-state) 1)
    
    (let ((local-clip (gcontext-internal-clip local-state)))
      (if (setf (gcontext-internal-clip-mask local-state) (second local-clip))
	  (setf (gcontext-internal-clip server-state) local-clip)
	  (setf (gcontext-internal-timestamp local-state) 0)))
    (let ((local-dash (gcontext-internal-dash local-state)))
      (if (setf (gcontext-internal-dashes local-state) (first local-dash))
	  (setf (gcontext-internal-dash server-state) local-dash)
	  (setf (gcontext-internal-timestamp local-state) 0)))
    (let ((local-font (gcontext-internal-font-obj local-state)))
      (when local-font
	(setf (gcontext-internal-font local-state) (font-id local-font))
	(setf (gcontext-internal-font-obj server-state) local-font)))

    (with-buffer-request (display *x-creategc*)
      (card32 gcontextid)
      (drawable drawable)
      (progn (do* ((i 0 (index+ i 1))
		   (bit 1 (the gcmask (ash bit 1)))
		   (nbyte 16)
		   (mask 0)
		   (local (svref local-state i) (svref local-state i)))
		 ((index>= i *gcontext-fast-change-length*)
		  (card32-put 12 mask)
		  (card16-put 2 (index-ash nbyte -2))
		  (index-incf (buffer-boffset display) nbyte))
	       (declare (type array-index i nbyte)
			(type gcmask bit mask)
			(type (or null card32) local))
	       (unless (eql local (the (or null card32) (svref server-state i)))
		 (setf (svref server-state i) local)
		 (card32-put nbyte local)
		 (setq mask (the gcmask (logior mask bit)))
		 (index-incf nbyte 4)))))
    gcontext))

(defun copy-gcontext-components (src dst &rest keys)
  (declare (type gcontext src dst)
	   (type list keys)) ;; list of GContext-key
  ;; you might ask why this isn't just a bunch of
  ;;   (setf (gcontext-<mumble> dst) (gcontext-<mumble> src))
  ;; the answer is that you can do that yourself if you want, what we are
  ;; providing here is access to the protocol request, which will generally
  ;; be more efficient (particularly for things like clip and dash lists).
  (when keys
    (let ((display (gcontext-display src))
	  (mask 0))
      (declare (type xgcmask mask))
      ;; collect entire mask first, so that an error doesn't leave us inconsistent
      (dolist (key keys)
	(let ((i (getf *gcontext-indexes* key)))
	  (declare (type (or null array-index) i))
	  (unless i
	    (x-type-error key 'gcontext-key))
	  (setq mask (the xgcmask (logior mask
					  (the xgcmask (svref *gcontext-masks* i)))))))
      (with-display (display)
	(force-gcontext-changes src)
	(force-gcontext-changes dst)
	(do ((src-server-state (gcontext-server-state src))
	     (dst-server-state (gcontext-server-state dst))
	     (dst-local-state (gcontext-local-state dst))
	     (bit mask (the xgcmask (ash bit -1)))
	     (i 0 (index+ i 1)))
	    ((zerop bit)
	     (incf-internal-timestamp dst-server-state)
	     (setf (gcontext-internal-timestamp dst-local-state) 0))
	  (declare (type gcontext-state src-server-state dst-server-state dst-local-state)
		   (type xgcmask bit)
		   (type array-index i))
	  (when (oddp bit)
	    (setf (svref dst-local-state i)
		  (setf (svref dst-server-state i) (svref src-server-state i)))))
	(with-buffer-request (display *x-copygc*)
	  (gcontext src dst)
	  (card32 (xgcmask->gcmask mask)))))))

(defun copy-gcontext (src dst)
  (declare (type gcontext src dst))
  ;; Copies all components.
  (apply #'copy-gcontext-components src dst *gcontext-components*))
	   
(defun free-gcontext (gcontext)
  (declare (type gcontext gcontext))
  (let ((display (gcontext-display gcontext)))
    (with-buffer-request (display *x-freegc*)
      (gcontext gcontext))
    (deallocate-resource-id display (gcontext-id gcontext) 'gcontext)
    (deallocate-gcontext-state (gcontext-server-state gcontext))
    (deallocate-gcontext-state (gcontext-local-state gcontext))
    nil))

(export '(force-gcontext-changes
	  with-gcontext
	  create-gcontext
	  copy-gcontext-components
	  copy-gcontext
	  free-gcontext
	  
	  gcontext-function
	  gcontext-plane-mask
	  gcontext-foreground
	  gcontext-background
	  gcontext-line-width
	  gcontext-line-style
	  gcontext-cap-style
	  gcontext-join-style
	  gcontext-fill-style
	  gcontext-fill-rule
	  gcontext-tile
	  gcontext-stipple
	  gcontext-ts-x
	  gcontext-ts-y
	  gcontext-font
	  gcontext-subwindow-mode
	  gcontext-exposures
	  gcontext-clip-x
	  gcontext-clip-y
	  gcontext-clip-mask
	  gcontext-dashes
	  gcontext-arc-mode
	  ))
