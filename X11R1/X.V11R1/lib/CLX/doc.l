;;; -*- Mode: LISP; Syntax: Common-lisp; Package: (XLIB (CL)); Base: 10; Lowercase: Yes -*-

;; Version 4

;; This is considered a beta interface.  Discussion of better
;; integration with CLOS, and support for user-specified subclassess of
;; basic objects is still in progress.

;; Primary Interface Author:
;;	Robert W. Scheifler
;;	MIT Laboratory for Computer Science
;;	545 Technology Square, Room 418
;;	Cambridge, MA 02139
;;	rws@zermatt.lcs.mit.edu

;; Design Contributors:
;;	Dan Cerys, Texas Instruments
;;	Scott Fahlman, CMU
;;	Kerry Kimbrough, Texas Instruments
;;	Chris Lindblad, MIT
;;	Rob MacLachlan, CMU
;;	Mike McMahon, Symbolics
;;	David Moon, Symbolics
;;	LaMott Oren, Texas Instruments
;;	Daniel Weinreb, Symbolics
;;	John Wroclawski, MIT
;;	Richard Zippel, Symbolics

;; Note: all of the following is in the package XLIB.

;; Note: various perversions of the CL type system are used below.
;; Examples: (list elt-type) (sequence elt-type)

(proclaim '(declaration arglist values))

;; Note: if you have read the Version 11 protocol document or C Xlib manual, most of
;; the relationships should be fairly obvious.  We have no intention of writing yet
;; another moby document for this interface.

(deftype card32 () '(unsigned-byte 32))

(deftype card29 () '(unsigned-byte 29))

(deftype int32 () '(signed-byte 32))

(deftype card16 () '(unsigned-byte 16))

(deftype int16 () '(signed-byte 16))

(deftype card8 () '(unsigned-byte 8))

(deftype int8 () '(signed-byte 8))

(deftype mask32 () 'card32)

(deftype mask16 () 'card16)

(deftype resource-id () 'card29)

;; Types employed: display, window, pixmap, cursor, font, gcontext, colormap, color.
;; These types are defined solely by a functional interface; we do not specify
;; whether they are implemented as structures or flavors or ...  Although functions
;; below are written using DEFUN, this is not an implementation requirement (although
;; it is a requirement that they be functions as opposed to macros or special forms).
;; It is unclear whether with-slots in the Common Lisp Object System must work on
;; them.

;; Windows, pixmaps, cursors, fonts, gcontexts, and colormaps are all represented as
;; compound objects, rather than as integer resource-ids.  This allows applications
;; to deal with multiple displays without having an explicit display argument in the
;; most common functions.  Every function uses the display object indicated by the
;; first argument that is or contains a display; it is an error if arguments contain
;; different displays, and predictable results are not guaranteed.

;; Each of window, pixmap, cursor, font, gcontext, and colormap have the following
;; five functions:

(defun make-<mumble> (display resource-id)
  ;; This function should almost never be called by applications, except in handling
  ;; events.  To minimize consing in some implementations, this may use a cache in
  ;; the display.  Make-gcontext creates with :cache-p nil.
  (declare (type display display)
	   (type resource-id resource-id)
	   (values <mumble>)))

(defun <mumble>-display (<mumble>)
  (declare (type <mumble> <mumble>)
	   (values display)))

(defun <mumble>-id (<mumble>)
  (declare (type <mumble> <mumble>)
	   (values resource-id)))

(defun <mumble>-equal (<mumble>-1 <mumble>-2)
  (declare (type <mumble> <mumble>-1 <mumble>-2)))

(defun <mumble>-p (<mumble>)
  (declare (type <mumble> <mumble>)
	   (values boolean)))

;; The following functions are provided by color objects:

;; The intention is that IHS and YIQ and CYM interfaces will also exist.  Note that
;; we are explicitly using a different spectrum representation than what is actually
;; transmitted in the protocol.

(deftype rgb-val () '(float 0.0 1.0))

(defun make-color (&key red green blue &allow-other-keys)	; for expansion
  (declare (type rgb-val red green blue)
	   (values color)))

(defun color-rgb (color)
  (declare (type color color)
	   (values red green blue)))

(defun color-red (color)
  ;; setf'able
  (declare (type color color)
	   (values rgb-val)))

(defun color-green (color)
  ;; setf'able
  (declare (type color color)
	   (values rgb-val)))

(defun color-blue (color)
  ;; setf'able
  (declare (type color color)
	   (values rgb-val)))

(deftype drawable () '(or window pixmap))

;; Atoms are accepted as strings or symbols, and are always returned as keywords.
;; Protocol-level integer atom ids are hidden, using a cache in the display object.

(deftype xatom () '(or string symbol))

(deftype stringable () '(or string symbol))

(deftype fontable () '(or stringable font))

;; Nil stands for CurrentTime.

(deftype timestamp () '(or null card32))

(deftype bit-gravity () '(member :forget :static :north-west :north :north-east
				 :west :center :east :south-west :south :south-east))

(deftype win-gravity () '(member :unmap :static :north-west :north :north-east
				 :west :center :east :south-west :south :south-east))

(deftype grab-status ()
  '(member :success :already-grabbed :frozen :invalid-time :not-viewable))

(deftype boolean () '(or null (not null)))

(deftype pixel () '(unsigned-byte 32))
(deftype image-depth () '(integer 0 32))

(deftype keysym () 'card32)

(deftype array-index () `(integer 0 ,array-dimension-limit))

;; An association list.

(deftype alist (key-type-and-name datum-type-and-name) 'list)

;; A sequence, containing zero or more repetitions of the given elements,
;; with the elements expressed as (type name).

(deftype repeat-seq (&rest elts) 'sequence)

(deftype point-seq () '(repeat-seq (int16 x) (int16 y)))

(deftype seg-seq () '(repeat-seq (int16 x1) (int16 y1) (int16 x2) (int16 y2)))

(deftype rect-seq () '(repeat-seq (int16 x) (int16 y) (card16 width) (card16 height)))

;; Note that we are explicitly using a different angle representation than what
;; is actually transmitted in the protocol.

(deftype angle () `(number ,(* -2 pi) ,(* 2 pi)))

(deftype arc-seq () '(repeat-seq (int16 x) (int16 y) (card16 width) (card16 height)
				 (angle angle1) (angle angle2)))

(deftype event-mask-class ()
  '(member :key-press :key-release :owner-grab-button :button-press :button-release
	   :enter-window :leave-window :pointer-motion :pointer-motion-hint
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion :exposure :visibility-change
	   :structure-notify :resize-redirect :substructure-notify :substructure-redirect
	   :focus-change :property-change :colormap-change :keymap-state))

(deftype event-mask ()
  '(or mask32 (list event-mask-class)))

(deftype pointer-event-mask-class ()
  '(member :button-press :button-release
	   :enter-window :leave-window :pointer-motion :pointer-motion-hint
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion :keymap-state))

(deftype pointer-event-mask ()
  '(or mask32 (list pointer-event-mask-class)))

(deftype device-event-mask-class ()
  '(member :key-press :key-release :button-press :button-release :pointer-motion
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion))

(deftype device-event-mask ()
  '(or mask32 (list device-event-mask-class)))

(deftype modifier-key ()
  '(member :shift :lock :control :mod-1 :mod-2 :mod-3 :mod-4 :mod-5))

(deftype modifier-mask ()
  '(or (member :any) mask16 (list modifier-key)))

(deftype state-mask-key ()
  '(or modifier-key (member :button-1 :button-2 :button-3 :button-4 :button-5)))

(deftype gcontext-key ()
  '(member :function :plane-mask :foreground :background
	   :line-width :line-style :cap-style :join-style :fill-style :fill-rule
	   :arc-mode :tile :stipple :ts-x :ts-y :font :subwindow-mode
	   :exposures :clip-x :clip-y :clip-mask :dash-offset :dashes))

(deftype event-key ()
  '(member :key-press :key-release :button-press :button-release :motion-notify
	   :enter-notify :leave-notify :focus-in :focus-out :keymap-notify
	   :exposure :graphics-exposure :no-exposure :visibility-notify
	   :create-notify :destroy-notify :unmap-notify :map-notify :map-request
	   :reparent-notify :configure-notify :gravity-notify :resize-request
	   :configure-request :circulate-notify :circulate-request :property-notify
	   :selection-clear :selection-request :selection-notify
	   :colormap-notify :client-message))

(deftype error-key ()
  '(member :access :alloc :atom :colormap :cursor :drawable :font :gcontext :id-choice
	   :illegal-request :implementation :length :match :name :pixmap :value :window))

(deftype draw-direction ()
  '(member :left-to-right :right-to-left))

(defstruct bitmap-format
  (unit <unspec> :type (member 8 16 32))
  (pad <unspec> :type (member 8 16 32))
  (lsb-first-p <unspec> :type boolean))

(defstruct pixmap-format
  (depth <unspec> :type image-depth)
  (bits-per-pixel <unspec> :type (member 4 8 16 32))
  (pad <unspec> :type (member 8 16 32)))

(defstruct visual-info
  (id <unspec> :type card29)
  (class <unspec> :type (member :static-gray :static-color :true-color
				:gray-scale :pseudo-color :direct-color))
  (red-mask <unspec> :type pixel)
  (green-mask <unspec> :type pixel)
  (blue-mask <unspec> :type pixel)
  (bits-per-rgb <unspec> :type card8)
  (colormap-entries <unspec> :type card16))

(defstruct screen
  (root <unspec> :type window)
  (width <unspec> :type card16)
  (height <unspec> :type card16)
  (width-in-millimeters <unspec> :type card16)
  (height-in-millimeters <unspec> :type card16)
  (depths <unspec> :type (alist (image-depth depth) ((list visual-info) visuals)))
  (root-depth <unspec> :type image-depth)
  (root-visual <unspec> :type card29)
  (default-colormap <unspec> :type colormap)
  (white-pixel <unspec> :type pixel)
  (black-pixel <unspec> :type pixel)
  (min-installed-maps <unspec> :type card16)
  (max-installed-maps <unspec> :type card16)
  (backing-stores <unspec> :type (member :never :when-mapped :always))
  (save-unders-p <unspec> :type boolean)
  (event-mask-at-open <unspec> :type mask32))

;; The list contains alternating keywords and integers.

(deftype font-props () 'list)

(defun open-display (host &key (display 0) protocol)
  ;; A string must be acceptable as a host, but otherwise the possible types for host
  ;; and protocol are not constrained, and will likely be very system dependent.  The
  ;; default protocol is system specific.  Authorization, if any, is assumed to come
  ;; from the environment somehow.
  (declare (type integer display)
	   (values display)))

(defun display-protocol-major-version (display)
  (declare (type display display)
	   (values card16)))

(defun display-protocol-minor-version (display)
  (declare (type display display)
	   (values card16)))

(defun display-vendor-name (display)
  (declare (type display display)
	   (values string)))

(defun display-release-number (display)
  (declare (type display display)
	   (values card32)))

(defun display-image-lsb-first-p (display)
  (declare (type display display)
	   (values boolean)))

(defun display-bitmap-formap (display)
  (declare (type display display)
	   (values bitmap-format)))

(defun display-pixmap-formats (display)
  (declare (type display display)
	   (values (list pixmap-formats))))

(defun display-roots (display)
  (declare (type display display)
	   (values (list screen))))

(defun display-motion-buffer-size (display)
  (declare (type display display)
	   (values card32)))

(defun display-max-request-length (display)
  (declare (type display display)
	   (values card16)))

(defun display-min-keycode (display)
  (declare (type display display)
	   (values card8)))

(defun display-max-keycode (display)
  (declare (type display display)
	   (values card8)))

(defun close-display (display)
  (declare (type display display)))

(defun display-error-handler (display)
  (declare (type display display)
	   (values handler)))

(defsetf display-error-handler (display) (handler)
  ;; All errors (synchronous and asynchronous) are processed by calling an error
  ;; handler in the display.  If handler is a sequence it is expected to contain
  ;; handler functions specific to each error; the error code is used to index the
  ;; sequence, fetching the appropriate handler.  Any results returned by the handler
  ;; are ignored; it is assumed the handler either takes care of the error
  ;; completely, or else signals. For all core errors, the keyword/value argument
  ;; pairs are:
  ;;    :display display
  ;;    :error-key error-key
  ;;    :major card8
  ;;    :minor card16
  ;;    :sequence card16
  ;;    :current-sequence card16
  ;; For :colormap, :cursor, :drawable, :font, :gcontext, :id-choice, :pixmap, and
  ;; :window errors another pair is:
  ;;    :resource-id card32
  ;; For :atom errors, another pair is:
  ;;    :atom-id card32
  ;; For :value errors, another pair is:
  ;;    :value card32
  (declare (type display display)
	   (type (or (sequence (function (&rest key-vals)))
		     (function (&rest key-vals)))
		 handler)))

(defmacro define-condition (name base &body items)
  ;; just a place-holder here for the real thing
  )

(define-condition request-error error
  display
  major
  minor
  sequence
  current-sequence)

(defun default-error-handler (&rest key-vals)
  ;; The default display-error-handler.
  ;; It signals the conditions listed below.
  )

(define-condition resource-error request-error
  resource-id)

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

(define-condition implementation-error request-error)

(define-condition length-error request-error)

(define-condition match-error request-error)

(define-condition name-error request-error)

(define-condition pixmap-error resource-error)

(define-condition value-error request-error
  value)

(define-condition window-error resource-error)

(defmacro with-display ((display) &body body)
  ;; This macro is for use in a multi-process environment.  It provides exclusive
  ;; access to the local display object for multiple request generation.  It need not
  ;; provide immediate exclusive access for replies; that is, if another process is
  ;; waiting for a reply (while not in a with-display), then synchronization need not
  ;; (but can) occur immediately.  Except where noted, all routines effectively
  ;; contain an implicit with-display where needed, so that correct synchronization
  ;; is always provided at the interface level on a per-call basis.  Nested uses of
  ;; this macro will work correctly.  This macro does not prevent concurrent event
  ;; processing; see with-event-queue.
  )

(defun display-force-output (display)
  ;; Output is normally buffered; this forces any buffered output.
  (declare (type display display)))

(defun display-finish-output (display)
  ;; Forces output, then causes a round-trip to ensure that all possible errors and
  ;; events have been received.
  (declare (type display display)))

(defun display-after-function (display)
  ;; setf'able
  ;; If defined, called after every protocol request is generated, even those inside
  ;; explicit with-display's, but never called from inside the after-function itself.
  ;; The function is called inside the effective with-display for the associated
  ;; request.  Default value is nil.  Can be set, for example, to
  ;; #'display-force-output or #'display-finish-output.
  (declare (type display display)
	   (values (or null (function (display))))))

(defun create-window (&key parent x y width height (depth 0) (border-width 0)
		      (class :copy) (visual :copy)
		      background border gravity bit-gravity
		      backing-store backing-planes backing-pixel save-under
		      event-mask do-not-propagate-mask override-redirect
		      colormap cursor)
  ;; Display is obtained from parent.  Only non-nil attributes are passed on in the
  ;; request: the function makes no assumptions about what the actual protocol
  ;; defaults are.  Width and height are the inside size, excluding border.
  (declare (type window parent)
	   (type int16 x y)
	   (type card16 width height depth border-width)
	   (type (member :copy :input-output :input-only) class)
	   (type (or (member :copy) visual) visual)
	   (type (or null (member :none :parent-relative) pixel pixmap) background)
	   (type (or null (member :copy) pixel pixmap) border)
	   (type (or null win-gravity) gravity)
	   (type (or null bit-gravity) bit-gravity)
	   (type (or null (member :not-useful :when-mapped :always) backing-store))
	   (type (or null pixel) backing-planes backing-pixel)
	   (type (or null event-mask) event-mask)
	   (type (or null device-event-mask) do-not-propagate-mask)
	   (type (or null (member :on :off)) save-under override-redirect)
	   (type (or null (member :copy) colormap) colormap)
	   (type (or null (member :none) cursor) cursor)
	   (values window)))

(defun window-class (window)
  (declare (type window window)
	   (values (member :input-output :input-only))))

(defun window-visual (window)
  (declare (type window window)
	   (values card29)))

(defsetf window-background (window) (background)
  (declare (type window window)
	   (type (or (member :none :parent-relative) pixel pixmap) background)))

(defsetf window-border (window) (border)
  (declare (type window window)
	   (type (or (member :copy) pixel pixmap) border)))

(defun window-gravity (window)
  ;; setf'able
  (declare (type window window)
	   (values win-gravity)))

(defun window-bit-gravity (window)
  ;; setf'able
  (declare (type window window)
	   (values bit-gravity)))

(defun window-backing-store (window)
  ;; setf'able
  (declare (type window window)
	   (values (member :not-useful :when-mapped :always))))

(defun window-backing-planes (window)
  ;; setf'able
  (declare (type window window)
	   (values pixel)))

(defun window-backing-pixel (window)
  ;; setf'able
  (declare (type window window)
	   (values pixel)))

(defun window-save-under (window)
  ;; setf'able
  (declare (type window window)
	   (values (member :on :off))))

(defun window-event-mask (window)
  ;; setf'able
  (declare (type window window)
	   (values mask32)))

(defun window-do-not-propagate-mask (window)
  ;; setf'able
  (declare (type window window)
	   (values mask32)))

(defun window-override-redirect (window)
  ;; setf'able
  (declare (type window window)
	   (values (member :on :off))))

(defun window-colormap (window)
  (declare (type window window)
	   (values (or null colormap))))

(defsetf window-colormap (window) (colormap)
  (declare (type window window)
	   (type (or (member :copy) colormap) colormap)))

(defsetf window-cursor (window) (cursor)
  (declare (type window window)
	   (type (or (member :none) cursor) cursor)))

(defun window-colormap-installed-p (window)
  (declare (type window window)
	   (values boolean)))

(defun window-all-event-masks (window)
  (declare (type window window)
	   (values mask32)))

(defun window-map-state (window)
  (declare (type window window)
	   (values (member :unmapped :unviewable :viewable))))

(defsetf drawable-x (window) (x)
  (declare (type window window)
	   (type int16 x)))

(defsetf drawable-y (window) (y)
  (declare (type window window)
	   (type int16 y)))

(defsetf drawable-width (window) (width)
  ;; Inside width, excluding border.
  (declare (type window window)
	   (type card16 width)))

(defsetf drawable-height (window) (height)
  ;; Inside height, excluding border.
  (declare (type window window)
	   (type card16 height)))

(defsetf drawable-border-width (window) (border-width)
  (declare (type window window)
	   (type card16 border-width)))

(defsetf window-priority (window &optional sibling) (mode)
  ;; A bit strange, but retains setf form.
  (declare (type window window)
	   (type (or null window) sibling)
	   (type (member :above :below :top-if :bottom-if :opposite) mode)))

(defmacro with-state ((drawable) &body body)
  ;; Allows a consistent view to be obtained of data returned by GetWindowAttributes
  ;; and GetGeometry, and allows a coherent update using ChangeWindowAttributes and
  ;; ConfigureWindow.  The body is not surrounded by a with-display.  Within the
  ;; indefinite scope of the body, on a per-process basis in a multi-process
  ;; environment, the first call within an Accessor Group on the specified drawable
  ;; (the object, not just the variable) causes the complete results of the protocol
  ;; request to be retained, and returned in any subsequent accessor calls.  Calls
  ;; within a Setf Group are delayed, and executed in a single request on exit from
  ;; the body.  In addition, if a call on a function within an Accessor Group follows
  ;; a call on a function in the corresponding Setf Group, then all delayed setfs for
  ;; that group are executed, any retained accessor information for that group is
  ;; discarded, the corresponding protocol request is (re)issued, and the results are
  ;; (again) retained, and returned in any subsequent accessor calls.

  ;; Accessor Group A (for GetWindowAttributes):
  ;; window-visual, window-class, window-gravity, window-bit-gravity,
  ;; window-backing-store, window-backing-planes, window-backing-pixel,
  ;; window-save-under, window-colormap, window-colormap-installed-p,
  ;; window-map-state, window-all-event-masks, window-event-mask,
  ;; window-do-not-propagate-mask, window-override-redirect

  ;; Setf Group A (for ChangeWindowAttributes):
  ;; window-gravity, window-bit-gravity, window-backing-store, window-backing-planes,
  ;; window-backing-pixel, window-save-under, window-event-mask,
  ;; window-do-not-propagate-mask, window-override-redirect, window-colormap,
  ;; window-cursor

  ;; Accessor Group G (for GetGeometry):
  ;; drawable-root, drawable-depth, drawable-x, drawable-y, drawable-width,
  ;; drawable-height, drawable-border-width

  ;; Setf Group G (for ConfigureWindow):
  ;; drawable-x, drawable-y, drawable-width, drawable-height, drawable-border-width,
  ;; window-priority
  )

(defun destroy-window (window)
  (declare (type window window)))

(defun destroy-subwindows (window)
  (declare (type window window)))

(defun add-to-save-set (window)
  (declare (type window window)))

(defun remove-from-save-set (window)
  (declare (type window window)))

(defun reparent-window (window parent x y)
  (declare (type window window parent)
	   (type int16 x y)))

(defun map-window (window)
  (declare (type window window)))

(defun map-subwindows (window)
  (declare (type window window)))

(defun unmap-window (window)
  (declare (type window window)))

(defun unmap-subwindows (window)
  (declare (type window window)))

(defun circulate-window-up (window)
  (declare (type window window)))

(defun circulate-window-down (window)
  (declare (type window window)))

(defun drawable-root (drawable)
  (declare (type drawable drawable)
	   (values drawable)))

(defun drawable-depth (drawable)
  (declare (type drawable drawable)
	   (values card8)))

(defun drawable-x (drawable)
  (declare (type drawable drawable)
	   (values int16)))

(defun drawable-y (drawable)
  (declare (type drawable drawable)
	   (values int16)))

(defun drawable-width (drawable)
  ;; For windows, inside width, excluding border.
  (declare (type drawable drawable)
	   (values card16)))

(defun drawable-height (drawable)
  ;; For windows, inside height, excluding border.
  (declare (type drawable drawable)
	   (values card16)))

(defun drawable-border-width (drawable)
  (declare (type drawable drawable)
	   (values card16)))

(defun query-tree (window &key (result-type 'list))
  (declare (type window window)
	   (type type result-type)
	   (values (sequence window) parent root)))

(defun change-property (window property data type format
			&key (mode :replace) (start 0) end transform)
  ;; Start and end affect sub-sequence extracted from data.
  ;; Transform is applied to each extracted element.
  (declare (type window window)
	   (type xatom property type)
	   (type (member 8 16 32) format)
	   (type sequence data)
	   (type (member :replace :prepend :append) mode)
	   (type array-index start)
	   (type (or null array-index) end)
	   (type (or null (function (t) integer)) transform)))

(defun delete-property (window property)
  (declare (type window window)
	   (type xatom property)))

(defun get-property (window property
		     &key type (start 0) end delete-p (result-type 'list) transform)
  ;; Transform is applied to each integer retrieved.
  ;; Nil is returned for type when the protocol returns None.
  (declare (type window window)
	   (type xatom property)
	   (type (or null xatom) type)
	   (type array-index start)
	   (type (or null array-index) end)
	   (type boolean delete-p)
	   (type type result-type)
	   (type (or null (function (integer) t)) transform)
	   (values data type format bytes-after)))

(defun rotate-properties (window properties &optional (delta 1))
  ;; Postive rotates left, negative rotates right (opposite of actual protocol request).
  (declare (type window window)
	   (type (sequence xatom) properties)
	   (type int16 delta)))

(defun list-properties (window &key (result-type 'list))
  (declare (type window window)
	   (type type result-type)
	   (values (sequence keyword))))

;; Although atom-ids are not visible in the normal user interface, atom-ids might
;; appear in window properties and other user data, so conversion hooks are needed.

(defun intern-atom (display name)
  (declare (type display display)
	   (type xatom name)
	   (values card29)))

(defun find-atom (display name)
  (declare (type display display)
	   (type xatom name)
	   (values (or null card29))))

(defun atom-name (display atom-id)
  (declare (type display display)
	   (type card29 atom-id)
	   (values keyword)))

(defun selection-owner (display selection)
  (declare (type display display)
	   (type xatom selection)
	   (values (or null window))))

(defsetf selection-owner (display selection &optional time) (owner)
  ;; A bit strange, but retains setf form.
  (declare (type display display)
	   (type xatom selection)
	   (type (or null window) owner)
	   (type timestamp time)))

(defun convert-selection (selection type requestor &optional property time)
  (declare (type xatom selection type)
	   (type window requestor)
	   (type (or null xatom) property)
	   (type timestamp time)))

(defun send-event (window event-key event-mask &rest args
		   &key propagate-p display &allow-other-keys)
  ;; Additional arguments depend on event-key, and are as specified further below
  ;; with declare-event, except that both resource-ids and resource objects are
  ;; accepted in the event components.  The display argument is only required if the
  ;; window is :pointer-window or :input-focus.
  (declare (type (or window (member :pointer-window :input-focus)) window)
	   (type (or null event-key) event-key)
	   (type event-mask event-mask)
	   (type boolean propagate-p)
	   (type (or null display) display)))

(defun grab-pointer (window event-mask
		     &key owner-p sync-pointer-p sync-keyboard-p confine-to cursor time)
  (declare (type window window)
	   (type pointer-event-mask event-mask)
	   (type boolean owner-p sync-pointer-p sync-keyboard-p)
	   (type (or null window) confine-to)
	   (type (or null cursor) cursor)
	   (type timestamp time)
	   (values grab-status)))

(defun ungrab-pointer (display &key time)
  (declare (type display display)
	   (type timestamp time)))

(defun grab-button (window button event-mask
		    &key (modifiers 0)
			 owner-p sync-pointer-p sync-keyboard-p confine-to cursor)
  (declare (type window window)
	   (type (or (member :any) card8) button)
	   (type modifier-mask modifiers)
	   (type pointer-event-mask event-mask)
	   (type boolean owner-p sync-pointer-p sync-keyboard-p)
	   (type (or null window) confine-to)
	   (type (or null cursor) cursor)))

(defun ungrab-button (window button &key (modifiers 0))
  (declare (type window window)
	   (type (or (member :any) card8) button)
	   (type modifier-mask modifiers)))

(defun change-active-pointer-grab (display event-mask &optional cursor time)
  (declare (type display display)
	   (type pointer-event-mask event-mask)
	   (type (or null cursor) cursor)
	   (type timestamp time)))

(defun grab-keyboard (window &key owner-p sync-pointer-p sync-keyboard-p time)
  (declare (type window window)
	   (type boolean owner-p sync-pointer-p sync-keyboard-p)
	   (type timestamp time)
	   (values grab-status)))

(defun ungrab-keyboard (display &key time)
  (declare (type display display)
	   (type timestamp time)))

(defun grab-key (window key &key (modifiers 0) owner-p sync-pointer-p sync-keyboard-p)
  (declare (type window window)
	   (type boolean owner-p sync-pointer-p sync-keyboard-p)
	   (type (or (member :any) card8) key)
	   (type modifier-mask modifiers)))

(defun ungrab-key (window key &key (modifiers 0))
  (declare (type window window)
	   (type (or (member :any) card8) key)
	   (type modifier-mask modifiers)))

(defun allow-events (display mode &optional time)
  (declare (type display display)
	   (type (member :async-pointer :sync-pointer :reply-pointer
			 :async-keyboard :sync-keyboard :replay-keyboard
			 :async-both :sync-both)
		 mode)
	   (type timestamp time)))

(defun grab-server (display)
  (declare (type display display)))

(defun ungrab-server (display)
  (declare (type display display)))

(defmacro with-server-grabbed ((display) &body body)
  ;; The body is not surrounded by a with-display.
  )

(defun query-pointer (window)
  (declare (type window window)
	   (values x y same-screen-p child mask root-x root-y root)))

(defun pointer-position (window)
  (declare (type window window)
	   (values x y same-screen-p)))

(defun global-pointer-position (display)
  (declare (type display display)
	   (values root-x root-y root)))

(defun motion-events (window &key start stop (result-type 'list))
  (declare (type window window)
	   (type timestamp start stop)
	   (type type result-type)
	   (values (repeat-seq (int16 x) (int16 y) (timestamp time)))))

(defun translate-coordinates (src src-x src-y dst)
  ;; If src and dst are not on the same screen, nil is returned.
  (declare (type window src)
	   (type int16 src-x src-y)
	   (type window dst)
	   (values dst-x dst-y child)))

(defun warp-pointer (dst dst-x dst-y)
  (declare (type window dst)
	   (type int16 dst-x dst-y)))

(defun warp-pointer-relative (display x-off y-off)
  (declare (type display display)
	   (type int16 x-off y-off)))

(defun warp-pointer-if-inside (dst dst-x dst-y src src-x src-y
			       &optional src-width src-height)
  ;; Passing in a zero src-width or src-height is a no-op.  A null src-width or
  ;; src-height translates into a zero value in the protocol request.
  (declare (type window dst src)
	   (type int16 dst-x dst-y src-x src-y)
	   (type (or null card16) src-width src-height)))

(defun warp-pointer-relative-if-inside (x-off y-off src src-x src-y
					&optional src-width src-height)
  ;; Passing in a zero src-width or src-height is a no-op.  A null src-width or
  ;; src-height translates into a zero value in the protocol request.
  (declare (type window src)
	   (type int16 x-off y-off src-x src-y)
	   (type (or null card16) src-width src-height)))

(defun set-input-focus (display focus revert-to &optional time)
  ;; Setf ought to allow multiple values.
  (declare (type display display)
	   (type (or (member :none :pointer-root) window) focus)
	   (type (member :none :parent :pointer-root) revert-to)
	   (type timestamp time)))

(defun input-focus (display)
  (declare (type display display)
	   (values focus revert-to)))

(defun query-keymap (display)
  (declare (type display display)
	   (values (bit-vector 256))))

(defun open-font (display name)
  ;; Font objects may be cached and reference counted locally within the display
  ;; object.  This function might not execute a with-display if the font is cached.
  ;; The protocol QueryFont request happens on-demand under the covers.
  (declare (type display display)
	   (type stringable name)
	   (values font)))

;; We probably want a per-font bit to indicate whether caching on
;; text-extents/width calls is desirable.  But what to name it?

(defun discard-font-info (font)
  ;; Discards any state that can be re-obtained with QueryFont.  This is simply
  ;; a performance hint for memory-limited systems.
  (declare (type font font)))

;; This can be signalled anywhere a pseudo font access fails.

(define-condition invalid-font error
  font)

;; Note: font-font-info removed.

(defun font-name (font)
  ;; Returns nil for a pseudo font returned by gcontext-font.
  (declare (type font font)
	   (values (or null string))))

(defun font-direction (font)
  (declare (type font font)
	   (values draw-direction)))

(defun font-min-char (font)
  (declare (type font font)
	   (values card16)))

(defun font-max-char (font)
  (declare (type font font)
	   (values card16)))

(defun font-min-byte1 (font)
  (declare (type font font)
	   (values card8)))

(defun font-max-byte1 (font)
  (declare (type font font)
	   (values card8)))

(defun font-min-byte2 (font)
  (declare (type font font)
	   (values card8)))

(defun font-max-byte2 (font)
  (declare (type font font)
	   (values card8)))

(defun font-all-chars-exist-p (font)
  (declare (type font font)
	   (values boolean)))

(defun font-default-char (font)
  (declare (type font font)
	   (values card16)))

(defun font-ascent (font)
  (declare (type font font)
	   (values int16)))

(defun font-descent (font)
  (declare (type font font)
	   (values int16)))

;; The list contains alternating keywords and int32s.

(deftype font-props () 'list)

(defun font-properties (font)
  (declare (type font font)
	   (values font-props)))

(defun font-property (font name)
  (declare (type font font)
	   (type keyword name)
	   (values (or null int32))))

;; For each of left-bearing, right-bearing, width, ascent, descent, attributes:

(defun char-<metric> (font index)
  ;; Note: I have tentatively chosen to return nil for an out-of-bounds index
  ;; (or an in-bounds index on a pseudo font), although returning zero or
  ;; signalling might be better.
  (declare (type font font)
	   (type card16 index)
	   (values (or null int16))))

(defun max-char-<metric> (font)
  ;; Note: I have tentatively chosen separate accessors over allowing :min and
  ;; :max as an index above.
  (declare (type font font)
	   (values int16)))

(defun min-char-<metric> (font)
  (declare (type font font)
	   (values int16)))

;; Note: char16-<metric> accessors could be defined to accept two-byte indexes.

(defun close-font (font)
  ;; This might not generate a protocol request if the font is reference
  ;; counted locally or if it is a pseudo font.
  (declare (type font font)))

(defun list-font-names (display pattern &key (max-fonts 65535) (result-type 'list))
  (declare (type display display)
	   (type string pattern)
	   (type card16 max-fonts)
	   (type type result-type)
	   (values (sequence string))))

(defun list-fonts (display pattern &key (max-fonts 65535) (result-type 'list))
  ;; Returns "pseudo" fonts that contain basic font metrics and properties, but
  ;; no per-character metrics and no resource-ids.  These pseudo fonts will be
  ;; converted (internally) to real fonts dynamically as needed, by issuing an
  ;; OpenFont request.  However, the OpenFont might fail, in which case the
  ;; invalid-font error can arise.
  (declare (type display display)
	   (type string pattern)
	   (type card16 max-fonts)
	   (type type result-type)
	   (values (sequence font))))

(defun font-path (display &key (result-type 'list))
  (declare (type display display)
	   (type type result-type)
	   (values (sequence (or string pathname)))))

(defsetf font-path (display) (paths)
  (declare (type display display)
	   (type (sequence (or string pathname)) paths)))

(defun create-pixmap (&key width height depth drawable)
  (declare (type card16 width height)
	   (type card8 depth)
	   (type drawable drawable)
	   (values pixmap)))

(defun free-pixmap (pixmap)
  (declare (type pixmap pixmap)))

(defun create-gcontext (&key drawable function plane-mask foreground background
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
  (declare (type drawable drawable)
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
	   (type (or null (or card8 (sequence card8))) dashes)
	   (type boolean cache)
	   (values gcontext)))

;; For each argument to create-gcontext (except font, clip-mask and
;; clip-ordering) declared as (type <type> <name>), there is an accessor:

(defun gcontext-<name> (gcontext)
  ;; The value will be nil if the last value stored is unknown (e.g., the cache was
  ;; off, or the component was copied from a gcontext with unknown state).
  (declare (type gcontext gcontext)
	   (values <type>)))

;; For each argument to create-gcontext (except clip-mask and clip-ordering) declared
;; as (type (or null <type>) <name>), there is a setf for the corresponding accessor:

(defsetf gcontext-<name> (gcontext) (value)
  (declare (type gcontext gcontext)
	   (type <type> value)))

(defun gcontext-font (gcontext &optional metrics-p)
  ;; If the stored font is known, it is returned.  If it is not known and
  ;; metrics-p is false, then nil is returned.  If it is not known and
  ;; metrics-p is true, then a pseudo font is returned.  Full metric and
  ;; property information can be obtained, but the font does not have a name or
  ;; a resource-id, and attempts to use it where a resource-id is required will
  ;; result in an invalid-font error.
  (declare (type gcontext gcontext)
	   (type boolean metrics-p)
	   (values (or null font))))

(defun gcontext-clip-mask (gcontext)
  (declare (type gcontext gcontext)
	   (values (or null (member :none) pixmap rect-seq)
		   (or null (member :unsorted :y-sorted :yx-sorted :yx-banded)))))

(defsetf gcontext-clip-mask (gcontext &optional ordering) (clip-mask)
  ;; Is nil illegal here, or is it transformed to a vector?
  ;; A bit strange, but retains setf form.
  (declare (type gcontext gcontext)
	   (type (or null (member :unsorted :y-sorted :yx-sorted :yx-banded)) clip-ordering)
	   (type (or (member :none) pixmap rect-seq) clip-mask)))

(defun force-gcontext-changes (gcontext)
  ;; Force any delayed changes.
  (declare (type gcontext gcontext)))

(defmacro with-gcontext ((gcontext &key
			  function plane-mask foreground background
			  line-width line-style cap-style join-style fill-style fill-rule
			  arc-mode tile stipple ts-x ts-y font subwindow-mode
			  exposures clip-x clip-y clip-mask clip-ordering
			  dashes dash-offset)
			 &body body)
  ;; Changes gcontext components within the dynamic scope of the body (i.e.,
  ;; indefinite scope and dynamic extent), on a per-process basis in a multi-process
  ;; environment.  The body is not surrounded by a with-display.  If cache-p is nil
  ;; or the some component states are unknown, this will implement save/restore by
  ;; creating a temporary gcontext and doing copy-gcontext-components to and from it.
  )

(defun copy-gcontext-components (src dst &rest keys)
  (declare (type gcontext src dst)
	   (type (list gcontext-key) keys)))

(defun copy-gcontext (src dst)
  (declare (type gcontext src dst))
  ;; Copies all components.
  )
	   
(defun free-gcontext (gcontext)
  (declare (type gcontext gcontext)))

(defun clear-area (window &key (x 0) (y 0) width height exposures-p)
  ;; Passing in a zero width or height is a no-op.  A null width or height translates
  ;; into a zero value in the protocol request.
  (declare (type window window)
	   (type int16 x y)
	   (type (or null card16) width height)
	   (type boolean exposures-p)))

(defun copy-area (src gcontext src-x src-y width height dst dst-x dst-y)
  (declare (type drawable src dst)
	   (type gcontext gcontext)
	   (type int16 src-x src-y dst-x dst-y)
	   (type card16 width height)))

(defun copy-plane (src gcontext plane src-x src-y width height dst dst-x dst-y)
  (declare (type drawable src dst)
	   (type gcontext gcontext)
	   (type pixel plane)
	   (type int16 src-x src-y dst-x dst-y)
	   (type card16 width height)))

(defun draw-point (drawable gcontext x y)
  ;; Should be clever about appending to existing buffered protocol request, provided
  ;; gcontext has not been modified.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)))

(defun draw-points (drawable gcontext points &optional relative-p)
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type point-seq points)
	   (type boolean relative-p)))

(defun draw-line (drawable gcontext x1 y1 x2 y2 &optional relative-p)
  ;; Should be clever about appending to existing buffered protocol request, provided
  ;; gcontext has not been modified.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x1 y1 x2 y2)
	   (type boolean relative-p)))

(defun draw-lines (drawable gcontext points &key relative-p fill-p (shape :complex))
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type point-seq points)
	   (type boolean relative-p fill-p)
	   (type (member :complex :non-convex :convex) shape)))

(defun draw-segments (drawable gcontext segments)
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type seg-seq segments)))

(defun draw-rectangle (drawable gcontext x y width height &optional fill-p)
  ;; Should be clever about appending to existing buffered protocol request, provided
  ;; gcontext has not been modified.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type card16 width height)
	   (type boolean fill-p)))

(defun draw-rectangles (drawable gcontext rectangles &optional fill-p)
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type rect-seq rectangles)
	   (type boolean fill-p)))

(defun draw-arc (drawable gcontext x y width height angle1 angle2 &optional fill-p)
  ;; Should be clever about appending to existing buffered protocol request, provided
  ;; gcontext has not been modified.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type card16 width height)
	   (type angle angle1 angle2)
	   (type boolean fill-p)))

(defun draw-arcs (drawable gcontext arcs &optional fill-p)
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type arc-seq arcs)
	   (type boolean fill-p)))

;; The following image routines are bare minimum.  It may be useful to define some
;; form of "image" object to hide representation details and format conversions.  It
;; also may be useful to provide stream-oriented interfaces for reading and writing
;; the data.

(defun put-raw-image (drawable gcontext data
		      &key (start 0) depth x y width height (left-pad 0) format)
  ;; Data must be a sequence of 8-bit quantities, already in the appropriate format
  ;; for transmission; the caller is responsible for all byte and bit swapping and
  ;; compaction.  Start is the starting index in data; the end is computed from the
  ;; other arguments.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type (sequence card8) data)
	   (type array-index start)
	   (type card8 depth left-pad)
	   (type int16 x y)
	   (type card16 width height)
	   (type (member :bitmap :xy-pixmap :z-pixmap) format)))

(defun get-raw-image (drawable &key data (start 0) x y width height
				    (plane-mask 0xffffffff) format
				    (result-type '(vector (unsigned-byte 8))))
  ;; If data is given, it is modified in place (and returned), otherwise a new
  ;; sequence is created and returned, with a size computed from the other arguments
  ;; and the returned depth.  The sequence is filled with 8-bit quantities, in
  ;; transmission format; the caller is responsible for any byte and bit swapping and
  ;; compaction required for further local use.
  (declare (type drawable drawable)
	   (type (or null (sequence card8)) data)
	   (type array-index start)
	   (type int16 x y)
	   (type card16 width height)
	   (type pixel plane-mask)
	   (type (member :xy-pixmap :z-pixmap) format)
	   (values (sequence card8) depth visual)))

(defun translate-default (src src-start src-end font dst dst-start)
  ;; dst is guaranteed to have room for (- src-end src-start) integer elements,
  ;; starting at dst-start; whether dst holds 8-bit or 16-bit elements depends
  ;; on context.  font is the current font, if known.  The function should
  ;; translate as many elements of src as possible into indexes in the current
  ;; font, and store them into dst.  The first return value should be the src
  ;; index of the first untranslated element.  If no further elements need to
  ;; be translated, the second return value should be nil.  If a horizontal
  ;; motion is required before further translation, the second return value
  ;; should be the delta in x coordinate.  If a font change is required for
  ;; further translation, the second return value should be the new font.  If
  ;; known, the pixel width of the translated text can be returned as the third
  ;; value; this can allow for appending of subsequent output to the same
  ;; protocol request, if no overall width has been specified at the higher
  ;; level.
  (declare (type sequence src)
	   (type array-index src-start src-end dst-start)
	   (type (or null font) font)
	   (type vector dst)
	   (values array-index (or null int16 font) (or null int32))))

;; There is a question below of whether translate should always be required, or
;; if not, what the default should be or where it should come from.  For
;; example, the default could be something that expected a string as src and
;; translated the CL standard character set to ASCII indexes, and ignored fonts
;; and bits.  Or the default could expect a string but otherwise be "system
;; dependent".  Or the default could be something that expected a vector of
;; integers and did no translation.  Or the default could come from the
;; gcontext (but what about text-extents and text-width?).

(defun text-extents (sequence &key (start 0) end font translate)
  ;; If multiple fonts are involved, font-ascent and font-descent will be the
  ;; maximums.  If multiple directions are involved, the direction will be nil.
  ;; Translate will always be called with a 16-bit dst buffer.
  (declare (type sequence sequence)
	   (type (or null font gcontext) font)
	   (type translate translate)
	   (values width ascent descent left right font-ascent font-descent direction
		   (or null array-index))))

(defun text-width (sequence &key (start 0) end font translate)
  ;; Translate will always be called with a 16-bit dst buffer.
  (declare (type sequence sequence)
	   (type (or null font gcontext) font)
	   (type translate translate)
	   (values int32 (or null array-index))))

;; This controls the element size of the dst buffer given to translate.  If
;; :default is specified, the size will be based on the current font, if known,
;; and otherwise 16 will be used.  [An alternative would be to pass the buffer
;; size to translate, and allow it to return the desired size if it doesn't
;; like the current size.  The problem is that the protocol doesn't allow
;; switching within a single request, so to allow switching would require
;; knowing the width of text, which isn't necessarily known.  We could call
;; text-width to compute it, but perhaps that is doing too many favors?]  [An
;; additional possibility is to allow an index-size of :two-byte, in which case
;; translate would be given a double-length 8-bit array, and translate would be
;; expected to store first-byte/second-byte instead of 16-bit integers.]

(deftype index-size () '(member :default 8 16))

;; In the glyph functions below, if width is specified, it is assumed to be the
;; total pixel width of whatever string of glyphs is actually drawn.
;; Specifying width will allow for appending the output of subsequent calls to
;; the same protocol request, provided gcontext has not been modified in the
;; interim.  If width is not specified, appending of subsequent output might
;; not occur (unless translate returns the width).  Specifying width is simply
;; a hint, for performance.

(defun draw-glyph (drawable gcontext x y elt
		   &key translate width (size :default))
  ;; Returns true if elt is output, nil if translate refuses to output it.
  ;; Second result is width, if known.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type translate translate)
	   (type (or null int32) width)
	   (type index-size size)
	   (values boolean (or null int32))))

(defun draw-glyphs (drawable gcontext x y sequence
		    &key (start 0) end translate width (size :default))
  ;; First result is new start, if end was not reached.  Second result is
  ;; overall width, if known.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type sequence sequence)
	   (type array-index start)
	   (type (or null array-index) end)
	   (type (or null int32) width)
	   (type translate translate)
	   (type index-size size)
	   (values (or null array-index) (or null int32))))

(defun draw-image-glyph (drawable gcontext x y elt
			 &key translate width (size :default))
  ;; Returns true if elt is output, nil if translate refuses to output it.
  ;; Second result is overall width, if known.  An initial font change is
  ;; allowed from translate.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type translate translate)
	   (type (or null int32) width)
	   (type index-size size)
	   (values boolean (or null int32))))

(defun draw-image-glyphs (drawable gcontext x y sequence
			  &key (start 0) end width translate (size :default))
  ;; An initial font change is allowed from translate, but any subsequent font
  ;; change or horizontal motion will cause termination (because the protocol
  ;; doesn't support chaining).  [Alternatively, font changes could be accepted
  ;; as long as they are accompanied with a width return value, or always
  ;; accept font changes and call text-width as required.  However, horizontal
  ;; motion can't really be accepted, due to semantics.]  First result is new
  ;; start, if end was not reached.  Second result is overall width, if known.
  (declare (type drawable drawable)
	   (type gcontext gcontext)
	   (type int16 x y)
	   (type sequence sequence)
	   (type array-index start)
	   (type (or null array-index) end)
	   (type (or null int32) width)
	   (type translate translate)
	   (type index-size size)
	   (values (or null array-index) (or null int32))))

(defun create-colormap (visual window &optional alloc-p)
  (declare (type card29 visual)
	   (type window window)
	   (type boolean alloc-p)
	   (values colormap)))

(defun free-colormap (colormap)
  (declare (type colormap colormap)))

(defun copy-colormap-and-free (colormap)
  (declare (type colormap colormap)
	   (values colormap)))

(defun install-colormap (colormap)
  (declare (type colormap colormap)))

(defun uninstall-colormap (colormap)
  (declare (type colormap colormap)))

(defun installed-colormaps (window &key (result-type 'list))
  (declare (type window window)
	   (type type result-type)
	   (values (sequence colormap))))

(defun alloc-color (colormap color)
  (declare (type colormap colormap)
	   (type (or stringable color) color)
	   (values pixel screen-color exact-color)))

(defun alloc-color-cells (colormap colors &key (planes 0) contiguous-p (result-type 'list))
  (declare (type colormap colormap)
	   (type card16 colors planes)
	   (type boolean contiguous-p)
	   (type type result-type)
	   (values (sequence pixel) (sequence mask))))

(defun alloc-color-planes (colormap colors
			   &key (reds 0) (greens 0) (blues 0)
				contiguous-p (result-type 'list))
  (declare (type colormap colormap)
	   (type card16 colors reds greens blues)
	   (type boolean contiguous-p)
	   (type type result-type)
	   (values (sequence pixel) red-mask green-mask blue-mask)))

(defun free-colors (colormap pixels &optional (plane-mask 0))
  (declare (type colormap colormap)
	   (type (sequence pixel) pixels)
	   (type pixel plane-mask)))

(defun store-color (colormap pixel spec &key (red-p t) (green-p t) (blue-p t))
  (declare (type colormap colormap)
	   (type pixel pixel)
	   (type (or stringable color) spec)
	   (type boolean red-p green-p blue-p)))

(defun store-colors (colormap specs &key (red-p t) (green-p t) (blue-p t))
  ;; If stringables are specified for colors, it is unspecified whether all
  ;; stringables are first resolved and then a single StoreColors protocol request is
  ;; issued, or whether multiple StoreColors protocol requests are issued.
  (declare (type colormap colormap)
	   (type (repeat-seq (pixel pixel) ((or stringable color) color)) specs)
	   (type boolean red-p green-p blue-p)))

(defun query-colors (colormap pixels &key (result-type 'list))
  (declare (type colormap colormap)
	   (type (sequence pixel) pixels)
	   (type type result-type)
	   (values (sequence color))))

(defun lookup-color (colormap name)
  (declare (type colormap colormap)
	   (type stringable name)
	   (values screen-color true-color)))

(defun create-cursor (&key source mask x y foreground background)
  (declare (type pixmap source)
	   (type (or null pixmap) mask)
	   (type card16 x y)
	   (type color foreground background)
	   (values cursor)))

(defun create-glyph-cursor (&key source-font source-char mask-font mask-char
				 foreground background)
  (declare (type font source-font)
	   (type card16 source-char)
	   (type (or null font) mask-font)
	   (type (or null card16) mask-char)
	   (type color foreground background)
	   (values cursor)))

(defun free-cursor (cursor)
  (declare (type cursor cursor)))

(defun recolor-cursor (cursor foreground background)
  (declare (type cursor cursor)
	   (type color foreground background)))

(defun query-best-cursor (width height display)
  (declare (type card16 width height)
	   (type display display)
	   (values width height)))

(defun query-best-tile (width height drawable)
  (declare (type card16 width height)
	   (type drawable drawable)
	   (values width height)))

(defun query-best-stipple (width height drawable)
  (declare (type card16 width height)
	   (type drawable drawable)
	   (values width height)))

(defun query-extension (display name)
  (declare (type display display)
	   (type stringable name)
	   (values major-opcode first-event first-error)))

(defun list-extensions (display &key (result-type 'list))
  (declare (type display display)
	   (type type result-type)
	   (values (sequence string))))

;; Should pointer-mapping setf be changed to set-pointer-mapping?

(defun set-modifier-mapping (display &key shift lock control mod1 mod2 mod3 mod4 mod5)
  ;; Can signal device-busy.
  ;; Setf ought to allow multiple values.
  ;; Returns true for success, nil for failure
  (declare (type display display)
	   (type (sequence card8) shift lock control mod1 mod2 mod3 mod4 mod5)
	   (values (member :success :busy :failed))))

(defun modifier-mapping (display)
  ;; each value is a list of card8s
  (declare (type display display)
	   (values shift lock control mod1 mod2 mod3 mod4 mod5)))

;; Either we will want lots of defconstants for well-known values, or perhaps
;; an integer-to-keyword translation function for well-known values.

(defun change-keyboard-mapping (display keysyms
				&key (start 0) end (first-keycode start))
  ;; start/end give subrange of keysyms
  ;; first-keycode is the first-keycode to store at
  (declare (type display display)
	   (type (array * (* *)) keysyms)
	   (type array-index start)
	   (type (or null array-index) end)
	   (type card8 first-keycode)))

(defun keyboard-mapping (display &key first-keycode start end data)
  ;; First-keycode specifies which keycode to start at (defaults to
  ;; min-keycode).  Start specifies where (in result) to put first-keycode
  ;; (defaults to first-keycode).  (- end start) is the number of keycodes to
  ;; get (end defaults to (1+ max-keycode)).  If data is specified, the results
  ;; are put there.
  (declare (type display display)
	   (type (or null card8) first-keycode)
	   (type (or null array-index) start end)
	   (type (or null (array * (* *))) data)
	   (values (array * (* *)))))

(defun change-keyboard-control (display &key key-click-percent
				bell-percent bell-pitch bell-duration
				led led-mode key auto-repeat-mode)
  (declare (type display display)
	   (type (or null (member :default) int16) key-click-percent
						   bell-percent bell-pitch bell-duration)
	   (type (or null card8) led key)
	   (type (or null (member :on :off)) led-mode)
	   (type (or null (member :on :off :default)) auto-repeat-mode)))

(defun keyboard-control (display)
  (declare (type display display)
	   (values key-click-percent bell-percent bell-pitch bell-duration
		   led-mask global-auto-repeat auto-repeats)))

(defun bell (display &optional (percent-from-normal 0))
  ;; It is assumed that an eventual audio extension to X will provide more complete
  ;; control.
  (declare (type display display)
	   (type int8 percent-from-normal)))

(defun pointer-mapping (display &key (result-type 'list))
  (declare (type display display)
	   (type type result-type)
	   (values (sequence card8))))

(defsetf pointer-mapping (display) (map)
  ;; Can signal device-busy.
  (declare (type display display)
	   (type (sequence card8) map)))

(defun change-pointer-control (display &key acceleration threshold)
  ;; Acceleration is rationalized if necessary.
  (declare (type display display)
	   (type (or null (member :default) number) acceleration)
	   (type (or null (member :default) integer) threshold)))

(defun pointer-control (display)
  (declare (type display display)
	   (values acceleration threshold)))

(defun set-screen-saver (display timeout interval blanking exposures)
  ;; Setf ought to allow multiple values.
  ;; Timeout and interval are in seconds, will be rounded to minutes.
  (declare (type display display)
	   (type (or (member :default) int16) timeout interval)
	   (type (member :yes :no :default) blanking exposures)))

(defun screen-saver (display)
  ;; Returns timeout and interval in seconds.
  (declare (type display display)
	   (values timeout interval blanking exposures)))

(defun activate-screen-saver (display)
  (declare (type display display)))

(defun reset-screen-saver (display)
  (declare (type display display)))

(defun add-access-host (display host)
  ;; A string must be acceptable as a host, but otherwise the possible types for host
  ;; are not constrained, and will likely be very system dependent.
  (declare (type display display)))

(defun remove-access-host (display host)
  ;; A string must be acceptable as a host, but otherwise the possible types for host
  ;; are not constrained, and will likely be very system dependent.
  (declare (type display display)))

(defun access-hosts (display &key (result-type 'list))
  ;; The type of host objects returned is not constrained, except that the hosts must
  ;; be acceptable to add-access-host and remove-access-host.
  (declare (type display display)
	   (type type result-type)
	   (values (sequence host) enabled-p)))

(defun access-control (display)
  ;; setf'able
  (declare (type display display)
	   (values boolean)))

(defun close-down-mode (display)
  ;; setf'able
  ;; Cached locally in display object.
  (declare (type display display)
	   (values (member :destroy :retain-permanent :retain-temporary))))

(defun kill-client (display resource-id)
  (declare (type display display)
	   (type resource-id resource-id)))

(defun kill-temporary-clients (display)
  (declare (type display display)))

(defun make-event-mask (&rest keys)
  ;; This is only defined for core events.
  ;; Useful for constructing event-mask, pointer-event-mask, device-event-mask.
  (declare (type (list event-mask-class) keys)
	   (values mask32)))

(defun make-event-keys (event-mask)
  ;; This is only defined for core events.
  (declare (type mask32 event-mask)
	   (values (list event-mask-class))))

(defun make-state-mask (&rest keys)
  ;; Useful for constructing modifier-mask, state-mask.
  (declare (type (list state-mask-key) keys)
	   (values mask16)))

(defun make-state-keys (state-mask)
  (declare (type mask16 mask)
	   (values (list state-mask-key))))

(defmacro with-event-queue ((display) &body body)
  ;; Grants exclusive access to event queue.
  )

(defun event-listen (display &optional (timeout 0))
  (declare (type display display)
	   (type (or null number) timeout))
  ;; Returns the number of events queued locally, if any, else nil.  Hangs waiting
  ;; for events, forever if timeout is nil, else for the specified number of seconds.
  )

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
  ;; fetching the appropriate handler.  The arguments to the handler are described
  ;; further below using declare-event.
  (declare (type display display)
	   (type (or (sequence (function (&rest key-vals) t))
		     (function (&rest key-vals) t))
		 handler)
	   (type (or null number) timeout)
	   (type boolean peek-p)))

(defmacro event-case ((display &key timeout peek-p discard-p force-output-p)
		      &body clauses)
  (declare (arglist (display &key timeout peek-p discard-p force-output-p)
		    (event-or-events ((&rest args) |...|) &body body) |...|))
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
  )

(defmacro declare-event (event-codes &rest declares)
  ;; Used to indicate the keyword arguments for handler functions in process-event
  ;; and event-case.  All process-event handlers can have (display display) and
  ;; (event-key event-key) as keyword arguments, and an event-case clause can also
  ;; have event-key as an argument.
  (declare (arglist event-key-or-keys &rest (type &rest keywords))))

(declare-event (:key-press :key-release :button-press :button-release)
	       (window window root)
	       ((or null window) child)
	       (boolean same-screen-p)
	       (int16 x y root-x root-y)
	       (card16 state)
	       (card32 time)
	       ;; for key-press and key-release, code is the keycode
	       ;; for button-press and button-release, code is the button number
	       (card8 code))

(declare-event :motion-notify
	       (window window root)
	       ((or null window) child)
	       (boolean same-screen-p)
	       (int16 x y root-x root-y)
	       (card16 state)
	       (card32 time)
	       (boolean hint-p))

(declare-event (:enter-notify :leave-notify)
	       (window window root)
	       ((or null window) child)
	       (boolean same-screen-p)
	       (int16 x y root-x root-y)
	       (card16 state)
	       (card32 time)
	       ((member :normal :grab :ungrab) mode)
	       ((member :ancestor :virtual :inferior :nonlinear :nonlinear-virtual) kind)
	       (boolean focus-p))

(declare-event (:focus-in :focus-out)
	       (window window)
	       ((member :normal :while-grabbed :grab :ungrab) mode)
	       ((member :ancestor :virtual :inferior :nonlinear :nonlinear-virtual
			:pointer :pointer-root :none)
		kind))

(declare-event :keymap-notify
	       (window window)
	       ((bit-vector 256) keymap))

(declare-event :exposure
	       (window window)
	       (card16 x y width height count))

(declare-event :graphics-exposure
	       (drawable drawable)
	       (card16 x y width height count)
	       (card8 major)
	       (card16 minor))

(declare-event :no-exposure
	       (drawable drawable)
	       (card8 major)
	       (card16 minor))

(declare-event :visibility-notify
	       (window window)
	       ((member :unobscured :partially-obscured :fully-obscured) state))

(declare-event :create-notify
	       (window window parent)
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
	       (int16 x y)
	       (card16 width height border-width)
	       ((or null window) above-sibling)
	       (boolean override-redirect-p))

(declare-event :gravity-notify
	       (window event-window window)
	       (int16 x y))

(declare-event :resize-request
	       (window window)
	       (card16 width height))

(declare-event :configure-request
	       (window parent window)
	       (int16 x y)
	       (card16 width height border-width)
	       ((member :above :below :top-if :bottom-if :opposite) stack-mode)
	       ((or null window) above-sibling)
	       (mask16 value-mask))

(declare-event :circulate-notify
	       (window event-window window)
	       ((member :top :bottom) place))

(declare-event :circulate-request
	       (window parent window)
	       ((member :top :bottom) place))

(declare-event :property-notify
	       (window window)
	       (keyword atom)
	       ((member :new-value :deleted) state)
	       (card32 time))

(declare-event :selection-clear
	       (window window)
	       (keyword selection)
	       (card32 time))

(declare-event :selection-request
	       (window window requestor)
	       (keyword selection target)
	       ((or null keyword) property)
	       (card32 time))

(declare-event :selection-notify
	       (window window)
	       (keyword selection target)
	       ((or null keyword) property)
	       (card32 time))

(declare-event :colormap-notify
	       (window window)
	       ((or null colormap) colormap)
	       (boolean new-p installed-p))

(declare-event :mapping-notify
	       ((member :modifier :keyboard :pointer) request)
	       (card8 start count))

(declare-event :client-message
	       (window window)
	       ((member 8 16 32) format)
	       ((sequence integer) data))

(defun queue-event (display event-key &rest args &key append-p &allow-other-keys)
  ;; The event is put at the head of the queue if append-p is nil, else the tail.
  ;; Additional arguments depend on event-key, and are as specified above with
  ;; declare-event, except that both resource-ids and resource objects are accepted
  ;; in the event components.
  (declare (type display display)
	   (type event-key event-key)
	   (type boolean append-p)))
