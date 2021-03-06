;;; -*- Mode: LISP; Syntax: Common-lisp; Package: XLIB; Base: 10; Lowercase: Yes -*-

;; Version 4

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

;; Primary Implementation Author:
;;	LaMott Oren, Texas Instruments

;; Implementation Contributors:
;;	Chris Lindblad, MIT
;;	Robert Scheifler, MIT

;;;
;;; Change history:
;;;
;;;  Date	Author		Description
;;; -------------------------------------------------------------------------------------
;;; 04/07/87	R.Scheifler	Created code stubs
;;; 04/08/87	L.Oren		Started Implementation
;;; 05/11/87	L.Oren		Included draft 3 revisions
;;; 07/07/87	L.Oren		Untested alpha release to MIT
;;; 07/17/87	L.Oren		Alpha release
;;; 08/**/87	C.Lindblad	Rewrite of buffer code
;;; 08/**/87	et al		Various random bug fixes
;;; 08/**/87	R.Scheifler	General syntactic and portability cleanups
;;; 08/**/87	R.Scheifler	Rewrite of gcontext caching and shadowing
;;; 09/02/87	L.Oren		Change events from resource-ids to objects
;;; 12/24/87	R.Budzianowski	KCL support
;;; 12/**/87	J.Irwin		ExCL 2.0 support
;;; 01/20/88	L.Oren		Add server extension mechanisms
;;; 01/20/88	L.Oren		Only force output when blocking on input
;;; 01/20/88	L.Oren		Uniform support for :event-window on events
;;; 01/28/88	L.Oren		Add window manager property functions
;;; 01/28/88	L.Oren		Add character translation facility
;;; 02/**/87	J.Irwin		Allegro 2.2 support

;;; This is considered a somewhat changeable interface.  Discussion of better
;;; integration with CLOS, support for user-specified subclassess of basic
;;; objects, and the additional functionality to match the C Xlib is still in
;;; progress.  Bug reports should be addressed to bug-clx@expo.lcs.mit.edu.

;; Note: all of the following is in the package XLIB.

(in-package 'xlib :use '(lisp))

(export '(
	  card32
	  card29
	  int32
	  card16
	  int16
	  card8
	  int8
	  rgb-val
	  angle
	  mask32
	  mask16
	  array-index
	  pixel
	  image-depth
	  display
	  display-p
	  display-display
	  display-after-function
	  display-protocol-major-version
	  display-protocol-minor-version
	  display-vendor-name
	  display-resource-id-base
	  display-resource-id-mask
	  display-xid
	  display-byte-order
	  display-version-number
	  display-release-number
	  display-max-request-length
	  display-squish
	  display-default-screen
	  display-nscreens
	  display-roots
	  display-motion-buffer-size
	  display-xdefaults
	  display-image-lsb-first-p
	  display-bitmap-format
	  display-pixmap-formats
	  display-min-keycode
	  display-max-keycode
	  display-error-handler
	  display-authorization-name
	  display-authorization-data
	  display-plist
	  color
	  color-p
	  color-red
	  color-green
	  color-blue
	  make-color
	  color-rgb
	  resource-id
	  drawable
	  drawable-p
	  drawable-equal
	  drawable-id
	  drawable-display
	  drawable-plist
	  window
	  window-p
	  window-equal
	  window-id
	  window-display
	  window-plist
	  pixmap
	  pixmap-p
	  pixmap-equal
	  pixmap-id
	  pixmap-display
	  pixmap-plist
	  colormap
	  colormap-p
	  colormap-equal
	  colormap-id
	  colormap-display
	  cursor
	  cursor-p
	  cursor-equal
	  cursor-id
	  cursor-display
	  xatom
	  stringable
	  fontable
	  timestamp
	  bit-gravity
	  win-gravity
	  grab-status
	  boolean
	  alist
	  repeat-seq
	  point-seq
	  seg-seq
	  rect-seq
	  arc-seq
	  gcontext
	  gcontext-p
	  gcontext-equal
	  gcontext-id
	  gcontext-display
	  gcontext-plist
	  event-mask-class
	  event-mask
	  pointer-event-mask-class
	  pointer-event-mask
	  device-event-mask-class
	  device-event-mask
	  modifier-key
	  modifier-mask
	  state-mask-key
	  gcontext-key
	  event-key
	  error-key
	  draw-direction
	  boole-constant
	  bitmap-format
	  bitmap-format-p
	  bitmap-format-unit
	  bitmap-format-pad
	  bitmap-format-lsb-first-p
	  pixmap-format
	  pixmap-format-p
	  pixmap-format-depth
	  pixmap-format-bits-per-pixel
	  pixmap-format-scanline-pad
	  visual-info
	  visual-info-p
	  visual-info-id
	  visual-info-class
	  visual-info-red-mask
	  visual-info-green-mask
	  visual-info-blue-mask
	  visual-info-bits-per-rgb
	  visual-info-colormap-entries
	  visual-info-plist
	  screen
	  screen-p
	  screen-root
	  screen-width
	  screen-height
	  screen-width-in-millimeters
	  screen-height-in-millimeters
	  screen-depths
	  screen-root-depth
	  screen-root-visual
	  screen-default-colormap
	  screen-white-pixel
	  screen-black-pixel
	  screen-min-installed-maps
	  screen-max-installed-maps
	  screen-backing-stores
	  screen-save-unders-p
	  screen-event-mask-at-open
	  screen-plist
	  font
	  font-p
	  font-equal
	  font-id
	  font-display
	  font-name
	  font-direction
	  font-min-char
	  font-max-char
	  font-min-byte1
	  font-max-byte1
	  font-min-byte2
	  font-max-byte2
	  font-all-chars-exist-p
	  font-default-char
	  font-min-bounds
	  font-max-bounds
	  font-ascent
	  font-descent
	  font-properties
	  font-property
	  font-plist
	  char-left-bearing
	  max-char-left-bearing
	  min-char-left-bearing
	  char-right-bearing
	  max-char-right-bearing
	  min-char-right-bearing
	  char-width
	  max-char-width
	  min-char-width
	  char-ascent
	  max-char-ascent
	  min-char-ascent
	  char-descent
	  max-char-descent
	  min-char-descent
	  char-attributes
	  max-char-attributes
	  min-char-attributes
	  make-event-mask
	  make-event-keys
	  make-state-mask
	  make-state-keys
	  ))

(defparameter *protocol-major-version* 11.)
(defparameter *protocol-minor-version* 0)

(defparameter *x-tcp-port* 6000) ;; add display number

; Note: various perversions of the CL type system are used below.
; Examples: (list elt-type) (sequence elt-type)

;; Note: if you have read the Version 11 protocol document or C Xlib manual, most of
;; the relationships should be fairly obvious.  We have no intention of writing yet
;; another moby document for this interface.

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

;(defun make-<mumble> (display resource-id)
;  ;; This function should almost never be called by applications, except in handling
;  ;; events.  To minimize consing in some implementations, this may use a cache in
;  ;; the display.  Make-gcontext creates with :cache-p nil.  Make-font creates with
;  ;; cache-p true.
;  (declare (type display display)
;	   (type integer resource-id)
;	   (values <mumble>)))

;(defun <mumble>-display (<mumble>)
;  (declare (type <mumble> <mumble>)
;	   (values display)))

;(defun <mumble>-id (<mumble>)
;  (declare (type <mumble> <mumble>)
;	   (values integer)))

;(defun <mumble>-equal (<mumble>-1 <mumble>-2)
;  (declare (type <mumble> <mumble>-1 <mumble>-2)))

;(defun <mumble>-p (<mumble>-1 <mumble>-2)
;  (declare (type <mumble> <mumble>-1 <mumble>-2)
;	   (values boolean)))

(deftype boolean () '(or null (not null)))

(deftype card32 () '(unsigned-byte 32))

(deftype card29 () '(unsigned-byte 29))

(deftype int32 () '(signed-byte 32))

(deftype card16 () '(unsigned-byte 16))

(deftype int16 () '(signed-byte 16))

(deftype card8 () '(unsigned-byte 8))

(deftype int8 () '(signed-byte 8))

; Note that we are explicitly using a different rgb representation than what
; is actually transmitted in the protocol.

(deftype rgb-val () '(float 0.0 1.0))

; Note that we are explicitly using a different angle representation than what
; is actually transmitted in the protocol.

(deftype angle () 'number)

(deftype mask32 () 'card32)

(deftype mask16 () 'card16)

(deftype pixel () '(unsigned-byte 32))
(deftype image-depth () '(integer 0 32))

(deftype resource-id () 'card29)

(deftype keysym () 'card32)

; The following functions are provided by color objects:

; The intention is that IHS and YIQ and CYM interfaces will also exist.
; Note that we are explicitly using a different spectrum representation
; than what is actually transmitted in the protocol.

(defstruct (color (:constructor make-color-internal (red green blue)))
  (red 0.0 :type rgb-val)
  (green 0.0 :type rgb-val)
  (blue 0.0 :type rgb-val))

(defun make-color (&key (red 1.0) (green 1.0) (blue 1.0) &allow-other-keys)
  (declare (type rgb-val red green blue))
  (declare-values color)
  (make-color-internal red green blue))

(defun color-rgb (color)
  (declare (type color color))
  (declare-values red green blue)
  (values (color-red color) (color-green color) (color-blue color)))

(defstruct bitmap-format
  (unit 8 :type (member 8 16 32))
  (pad 8 :type (member 8 16 32))
  (lsb-first-p nil :type boolean))

(defstruct pixmap-format
  (depth 0 :type image-depth)
  (bits-per-pixel 8 :type (member 1 4 8 16 24 32))
  (scanline-pad 8 :type (member 8 16 32)))

(defparameter *atom-cache-size* 200)
(defparameter *resource-id-map-size* 500)

(defstruct (display (:include buffer)
		    (:constructor make-display-internal)
		    (:print-function display-print))
  host						; Server Host
  (display 0 :type integer)			; Display number on host
  (after-function nil)				; Function to call after every request
  (waiting-reply-p nil)				; non-nil when waiting for a reply
  (input-lock (make-process-lock))		; Lock over reading from the input stream
  (event-lock (make-process-lock))		; with-event-queue lock
  (event-queue-lock (make-process-lock))	; new-events/event-queue lock
  (new-events nil)				; unprocessed events (a cons of event-queue)
  (event-queue nil)				; processed and unprocessed events
  (atom-cache (make-hash-table :test #'eq :size *atom-cache-size*)
	      :type hash-table)			; EQ Hash table relating atoms keywords
						; to resource id's
  (font-cache nil)				;list of font
  (protocol-major-version 0 :type card16)	; Major version of server's X protocol
  (protocol-minor-version 0 :type card16)	; minor version of servers X protocol
  (vendor-name "" :type string)			; vendor of the server hardware
  (resource-id-base 0 :type resource-id)	; resouce ID base
  (resource-id-mask 0 :type resource-id)	; resource ID mask bits
  (resource-id-byte nil)			; resource ID mask field (used with DPB & LDB)
  (resource-id-count 0 :type resource-id)	; resource ID mask count
						; (used for allocating ID's)
  (resource-id-map (make-hash-table :test #+explorer #'eql #-explorer #'eq
				    :size *resource-id-map-size*)
		   :type hash-table)		; hash table maps resource-id's to
						; objects (used in lookup functions)
  (xid 'resourcealloc)				; allocator function
  (byte-order :lsbfirst)			; screen byte order, LSBFirst, MSBFirst
  (version-number 11 :type card16)		; X protocol version number.
  (release-number 0 :type card32)		; release of the server
  (max-request-length 0 :type card16)		; maximum number 32 bit words in request
  (squish nil :type boolean)			; Squish MouseMoved events?
  (default-screen)				; default screen for operations
  (nscreens 1 :type card8)			; number of screens on this server
  (roots nil :type list)			; List of screens
  (motion-buffer-size 0 :type card32)		; size of motion buffer
  (xdefaults)					; contents of defaults from server
  (image-lsb-first-p nil :type boolean)
  (bitmap-format (make-bitmap-format)		; Screen image info
		 :type bitmap-format)
  (pixmap-formats nil :type sequence)		; list of pixmap formats
  (min-keycode 0 :type card8)			; minimum key-code
  (max-keycode 0 :type card8)			; maximum key-code
  (error-handler 'default-error-handler)	; Error handler function
  (close-down-mode :destroy)  			; Close down mode saved by Set-Close-Down-Mode
  (authorization-name "" :type string)
  (authorization-data "" :type string)
  (last-width nil :type (or null card29))	; Accumulated width of last string
  (keysym-mapping nil				; Keysym mapping cached from server
		  :type (or null (array * (* *))))
  (modifier-mapping nil :type list)		; ALIST of (keysym . state-mask) for all modifier keysyms
  (keysym-translation nil :type list)		; An alist of (keysym object function)
						; for display-local keysyms
  (extension-alist nil :type list)		; extension alist, which has elements:
						; (name major-opcode first-event first-error)
  (event-extensions #() :type vector)		; Vector mapping X event-codes to event keys
  (performance-info)				; Hook for gathering performance info
  (trace-history)				; Hook for debug trace
  (plist)					; hook for extension to hang data
  )

;;(deftype drawable () '(or window pixmap))

(defstruct drawable
  (id 0 :type resource-id)
  (display nil :type (or null display))
  (plist nil :type list)			; Extension hook
  )

(defstruct (window (:include drawable))
  )

(defstruct (pixmap (:include drawable))
  )

(defstruct colormap
  (id 0 :type resource-id)
  (display nil :type (or null display))
  )

(defstruct cursor
  (id 0 :type resource-id)
  (display nil :type (or null display))
  )

; Atoms are accepted as strings or symbols, and are always returned as keywords.
; Protocol-level integer atom ids are hidden, using a cache in the display object.

(deftype xatom () '(or string symbol))

(defconstant *predefined-atoms*
	     '#(nil :primary :secondary :arc :atom :bitmap
		    :cardinal :colormap :cursor
		    :cut_buffer0 :cut_buffer1 :cut_buffer2 :cut_buffer3
		    :cut_buffer4 :cut_buffer5 :cut_buffer6 :cut_buffer7
		    :drawable :font :integer :pixmap :point :rectangle
		    :resource_manager :rgb_color_map :rgb_best_map
		    :rgb_blue_map :rgb_default_map
		    :rgb_gray_map :rgb_green_map :rgb_red_map :string
		    :visualid :window :wm_command :wm_hints
		    :wm_client_machine :wm_icon_name :wm_icon_size
		    :wm_name :wm_normal_hints :wm_size_hints
		    :wm_zoom_hints :min_space :norm_space :max_space
		    :end_space :superscript_x :superscript_y
		    :subscript_x :subscript_y
		    :underline_position :underline_thickness
		    :strikeout_ascent :strikeout_descent
		    :italic_angle :x_height :quad_width :weight
		    :point_size :resolution :copyright :notice
		    :font_name :family_name :full_name :cap_height
		    :wm_class :wm_transient_for))

(deftype stringable () '(or string symbol))

(deftype fontable () '(or stringable font))

; Nil stands for CurrentTime.

(deftype timestamp () '(or null card32))

(defconstant *bit-gravity-vector*
	     '#(:forget :north-west :north :north-east :west
		:center :east :south-west :south
		:south-east :static))

(deftype bit-gravity ()
  '(member :forget :north-west :north :north-east :west
	   :center :east :south-west :south :south-east :static))

(defconstant *win-gravity-vector*
	     '#(:unmap :north-west :north :north-east :west
		:center :east :south-west :south :south-east
		:static))

(deftype win-gravity ()
  '(member :unmap :north-west :north :north-east :west
	   :center :east :south-west :south :south-east :static))

(deftype grab-status ()
  '(member :success :already-grabbed :invalid-time :not-viewable))

; An association list.

(deftype alist (key-type-and-name datum-type-and-name)
  key-type-and-name datum-type-and-name 'list)

; A sequence, containing zero or more repetitions of the given elements,
; with the elements expressed as (type name).

(deftype repeat-seq (&rest elts) elts 'sequence)

(deftype point-seq () '(repeat-seq (int16 x) (int16 y)))

(deftype seg-seq () '(repeat-seq (int16 x1) (int16 y1) (int16 x2) (int16 y2)))

(deftype rect-seq () '(repeat-seq (int16 x) (int16 y) (card16 width) (card16 height)))

(deftype arc-seq ()
  '(repeat-seq (int16 x) (int16 y) (card16 width) (card16 height)
	       (angle angle1) (angle angle2)))

(deftype gcontext-state () 'simple-vector)

(defstruct (gcontext (:copier nil))
  ;; The accessors convert to CLX data types.
  (id 0 :type resource-id)
  (display nil :type (or null display))
  (drawable nil :type (or null drawable))
  (cache-p t :type boolean)
  (server-state (allocate-gcontext-state) :type gcontext-state)
  (local-state (allocate-gcontext-state) :type gcontext-state)
  (plist nil :type list)			; Extension hook
  )

(defconstant *event-mask-vector*
	     '#(:key-press :key-release :button-press :button-release
		:enter-window :leave-window :pointer-motion :pointer-motion-hint
		:button-1-motion :button-2-motion :button-3-motion :button-4-motion
		:button-5-motion :button-motion :keymap-state :exposure :visibility-change
		:structure-notify :resize-redirect :substructure-notify :substructure-redirect
		:focus-change :property-change :colormap-change :owner-grab-button))

(deftype event-mask-class ()
  '(member :key-press :key-release :owner-grab-button :button-press :button-release
	   :enter-window :leave-window :pointer-motion :pointer-motion-hint
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion :exposure :visibility-change
	   :structure-notify :resize-redirect :substructure-notify :substructure-redirect
	   :focus-change :property-change :colormap-change :keymap-state))

(deftype event-mask ()
  '(or mask32 list)) ;; (OR integer (LIST event-mask-class))

(defconstant *pointer-event-mask-vector*
	     '#(%error %error :button-press :button-release
		:enter-window :leave-window :pointer-motion :pointer-motion-hint
		:button-1-motion :button-2-motion :button-3-motion :button-4-motion
		:button-5-motion :button-motion :keymap-state))

(deftype pointer-event-mask-class ()
  '(member :button-press :button-release
	   :enter-window :leave-window :pointer-motion :pointer-motion-hint
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion :keymap-state))

(deftype pointer-event-mask ()
  '(or mask32 list)) ;;  '(or integer (list pointer-event-mask-class)))

(defconstant *device-event-mask-vector*
	     '#(:key-press :key-release :button-press :button-release :pointer-motion
		:button-1-motion :button-2-motion :button-3-motion :button-4-motion
		:button-5-motion :button-motion))

(deftype device-event-mask-class ()
  '(member :key-press :key-release :button-press :button-release :pointer-motion
	   :button-1-motion :button-2-motion :button-3-motion :button-4-motion
	   :button-5-motion :button-motion))

(deftype device-event-mask ()
  '(or mask32 list)) ;;  '(or integer (list device-event-mask-class)))

(defconstant *state-mask-vector*
	     '#(:shift :lock :control :mod-1 :mod-2 :mod-3 :mod-4 :mod-5
		:button-1 :button-2 :button-3 :button-4 :button-5))

(deftype modifier-key ()
  '(member :shift :lock :control :mod-1 :mod-2 :mod-3 :mod-4 :mod-5))

(deftype modifier-mask ()
  '(or (member :any) mask16 list)) ;;  '(or (member :any) integer (list modifier-key)))

(deftype state-mask-key ()
  '(or modifier-key (member :button-1 :button-2 :button-3 :button-4 :button-5)))

(defconstant *gcontext-components*
	     '(:function :plane-mask :foreground :background
	       :line-width :line-style :cap-style :join-style :fill-style
	       :fill-rule :tile :stipple :ts-x :ts-y :font :subwindow-mode
	       :exposures :clip-x :clip-y :clip-mask :dash-offset :dashes
	       :arc-mode))

(deftype gcontext-key ()
  '(member :function :plane-mask :foreground :background
	   :line-width :line-style :cap-style :join-style :fill-style
	   :fill-rule :tile :stipple :ts-x :ts-y :font :subwindow-mode
	   :exposures :clip-x :clip-y :clip-mask :dash-offset :dashes
	   :arc-mode))

(deftype event-key ()
  '(member :key-press :key-release :button-press :button-release :motion-notify
	   :enter-notify :leave-notify :focus-in :focus-out :keymap-notify
	   :exposure :graphics-exposure :no-exposure :visibility-notify
	   :create-notify :destroy-notify :unmap-notify :map-notify :map-request
	   :reparent-notify :configure-notify :gravity-notify :resize-request
	   :configure-request :circulate-notify :circulate-request :property-notify
	   :selection-clear :selection-request :selection-notify
	   :colormap-notify :client-message :mapping-notify))

(deftype error-key ()
  '(member :access :alloc :atom :colormap :cursor :drawable :font :gcontext :id-choice
	   :illegal-request :implementation :length :match :name :pixmap :value :window))

(deftype draw-direction ()
  '(member :left-to-right :right-to-left))

(defconstant *boole-vector*
	     '#(#.boole-clr #.boole-and #.boole-andc2 #.boole-1
		#.boole-andc1 #.boole-2 #.boole-xor #.boole-ior
		#.boole-nor #.boole-eqv #.boole-c2 #.boole-orc2
		#.boole-c1 #.boole-orc1 #.boole-nand #.boole-set))

(deftype boole-constant ()
  `(member ,boole-clr ,boole-and ,boole-andc2 ,boole-1
	   ,boole-andc1 ,boole-2 ,boole-xor ,boole-ior
	   ,boole-nor ,boole-eqv ,boole-c2 ,boole-orc2
	   ,boole-c1 ,boole-orc1 ,boole-nand ,boole-set))

(defstruct visual-info
  (id 0 :type card29)
  (class :static-gray :type (member :static-gray :static-color :true-color
				    :gray-scale :pseudo-color :direct-color))
  (red-mask 0 :type pixel)
  (green-mask 0 :type pixel)
  (blue-mask 0 :type pixel)
  (bits-per-rgb 1 :type card8)
  (colormap-entries 0 :type card16)
  (plist nil :type list)			; Extension hook
  )

(defstruct screen
  (root nil :type (or null window))
  (width 0 :type card16)
  (height 0 :type card16)
  (width-in-millimeters 0 :type card16)
  (height-in-millimeters 0 :type card16)
  (depths nil :type (alist (image-depth depth) ((list visual-info) visuals)))
  (root-depth 1 :type image-depth)
  (root-visual 0 :type card29)
  (default-colormap nil :type (or null colormap))
  (white-pixel 0 :type pixel)
  (black-pixel 1 :type pixel)
  (min-installed-maps 1 :type card16)
  (max-installed-maps 1 :type card16)
  (backing-stores :never :type (member :never :when-mapped :always))
  (save-unders-p nil :type boolean)
  (event-mask-at-open 0 :type mask32)
  (plist nil :type list)			; Extension hook
  )

;; The list contains alternating keywords and integers.
(deftype font-props () 'list)

(defstruct font-info
  (direction :left-to-right :type draw-direction)
  (min-char 0 :type card16)   ;; First character in font
  (max-char 0 :type card16)   ;; Last character in font
  (min-byte1 0 :type card8)   ;; The following are for 16 bit fonts
  (max-byte1 0 :type card8)   ;; and specify min&max values for
  (min-byte2 0 :type card8)   ;; the two character bytes
  (max-byte2 0 :type card8)
  (all-chars-exist-p nil :type boolean)
  (default-char 0 :type card16)
  (min-bounds nil :type (or null vector))
  (max-bounds nil :type (or null vector))
  (ascent 0 :type int16)
  (descent 0 :type int16)
  (properties nil :type font-props))

(defstruct (font (:constructor make-font-internal))
  (id-internal nil :type (or null resource-id)) ;; NIL when not opened
  (display nil :type (or null display))
  (reference-count 0 :type fixnum)
  (name "" :type (or null string)) ;; NIL when ID is for a GContext
  (font-info-internal nil :type (or null font-info))
  (char-infos-internal nil :type (or null vector))
  (local-only-p t :type boolean) ;; When T, always calculate text extents locally
  (plist nil :type list)			; Extension hook
  )

(proclaim '(inline font-id font-font-info font-char-infos make-font))

(defun font-id (font)
  ;; Get font-id, opening font if needed
  (or (font-id-internal font)
      (open-font-internal font)))

(defun font-font-info (font)
  (or (font-font-info-internal font)
      (query-font font)))

(defun font-char-infos (font)
  (or (font-char-infos-internal font)
      (progn (query-font font)
	     (font-char-infos-internal font))))

(defun make-font (&key id
		  display
		  (reference-count 0)
		  (name "")
		  (local-only-p t)
		  font-info-internal)
  (make-font-internal :id-internal id
		      :display display
		      :reference-count reference-count
		      :name name
		      :local-only-p local-only-p
		      :font-info-internal font-info-internal))

; For each component (<name> <unspec> :type <type>) of font-info,
; there is a corresponding function:

;(defun font-<name> (font)
;  (declare (type font font)
;	   (values <type>)))

(eval-when (eval compile) ;; I'd rather use macrolet, but Symbolics doesn't like it...

(defmacro make-font-info-accessors (useless-name &body fields)
  `(within-definition (,useless-name make-font-info-accessors)
     ,@(mapcan
	 #'(lambda (field)
	     (let* ((type (second field))
		    (n (string (first field)))
		    (name (xintern 'font- n))
		    (accessor (xintern 'font-info- n)))
	       `((proclaim '(inline ,name))
		 (defun ,name (font)
		   (declare (type font font))
		   (declare-values ,type)
		   (,accessor (font-font-info font))))))
	 fields)))

) ;; End eval-when

(make-font-info-accessors ignore
  (direction draw-direction)
  (min-char card16)
  (max-char card16)
  (min-byte1 card8)
  (max-byte1 card8)
  (min-byte2 card8)
  (max-byte2 card8)
  (all-chars-exist-p boolean)
  (default-char card16)
  (min-bounds vector)
  (max-bounds vector)
  (ascent int16)
  (descent int16)
  (properties font-props))

(defun font-property (font name)
  (declare (type font font)
	   (type keyword name))
  (declare-values (or null int32))
  (getf (font-properties font) name))

(eval-when (eval compile) ;; I'd rather use macrolet, but Symbolics doesn't like it...

(defmacro make-mumble-equal (type)
  ;; When cached, EQ works fine, otherwise test resource id's and displays
  (let ((predicate (xintern type '-equal))
	(id (xintern type '-id))
	(dpy (xintern type '-display)))
    (if (member type *clx-cached-types*)
	`(within-definition (,type make-mumble-equal)
	   (proclaim '(inline ,predicate))
	   (defun ,predicate (a b) (eq a b)))
      `(within-definition (,type make-mumble-equal)
	 (defun ,predicate (a b)
	   (declare (type ,type a b))
	   (and (= (,id a) (,id b))
		(eq (,dpy a) (,dpy b))))))))

) ;; End eval-when

(make-mumble-equal window)
(make-mumble-equal pixmap)
(make-mumble-equal cursor)
(make-mumble-equal font)
(make-mumble-equal gcontext)
(make-mumble-equal colormap)
(make-mumble-equal drawable)

;;;
;;; Event-mask encode/decode functions
;;;    Converts from keyword-lists to integer and back
;;;
(defun encode-mask (key-vector key-list key-type)
  ;; KEY-VECTOR is a vector containg bit-position keywords.  The position of the
  ;; keyword in the vector indicates its bit position in the resulting mask
  ;; KEY-LIST is either a mask or a list of KEY-TYPE
  ;; Returns NIL when KEY-LIST is not a list or mask.
  (declare (type vector key-vector)
	   (type (or mask32 list) key-list))
  (declare-values (or mask32 nil))
  (typecase key-list
    (mask32 key-list)
    (list (let ((mask 0))
	    (dolist (key key-list mask)
	      (let ((bit (position key (the vector key-vector) :test #'eq)))
		(unless bit
		  (x-type-error key key-type))
		(setq mask (logior mask (ash 1 bit)))))))))

(defun decode-mask (key-vector mask)
  (declare (type vector key-vector)
	   (type mask32 mask))
  (declare-values list)
  (do ((m mask (ash m -1))
       (bit 0 (1+ bit))
       (len (length key-vector))
       (result nil))       
      ((or (zerop m) (>= bit len)) result)
    (when (oddp m)
      (push (aref key-vector bit) result))))

(defun encode-event-mask (event-mask)
  (declare (type event-mask event-mask))
  (declare-values mask32)
  (or (encode-mask *event-mask-vector* event-mask 'event-mask-class)
      (x-type-error event-mask 'event-mask)))

(defun make-event-mask (&rest keys)
  ;; This is only defined for core events.
  ;; Useful for constructing event-mask, pointer-event-mask, device-event-mask.
  (declare (type list keys)) ;; (list event-mask-class)
  (declare-values mask32)
  (encode-mask *event-mask-vector* keys 'event-mask-class))

(defun make-event-keys (event-mask)
  ;; This is only defined for core events.
  (declare (type mask32 event-mask))
  (declare-values (list event-mask-class))
  (decode-mask *event-mask-vector* event-mask))

(defun encode-device-event-mask (device-event-mask)
  (declare (type device-event-mask device-event-mask))
  (declare-values mask32)
  (or (encode-mask *device-event-mask-vector* device-event-mask
		   'device-event-mask-class)
      (x-type-error device-event-mask 'device-event-mask)))

(defun encode-modifier-mask (modifier-mask)
  (declare (type modifier-mask modifier-mask)) ;; (list state-mask-key)
  (declare-values mask16)
  (or (encode-mask *state-mask-vector* modifier-mask 'modifier-key)
      (and (eq modifier-mask :any) #x8000)
      (x-type-error modifier-mask 'modifier-mask)))

(defun encode-state-mask (state-mask)
  (declare (type (or mask16 list) state-mask)) ;; (list state-mask-key)
  (declare-values mask16)
  (or (encode-mask *state-mask-vector* state-mask 'state-mask-key)
      (x-type-error state-mask '(or mask16 (list state-mask-key)))))

(defun make-state-mask (&rest keys)
  ;; Useful for constructing modifier-mask, state-mask.
  (declare (type list keys)) ;; (list state-mask-key)
  (declare-values mask16)
  (encode-mask *state-mask-vector* keys 'state-mask-key))

(defun make-state-keys (state-mask)
  (declare (type mask16 state-mask))
  (declare-values (list state-mask-key))
  (decode-mask *state-mask-vector* state-mask))

(defun encode-pointer-event-mask (pointer-event-mask)
  (declare (type pointer-event-mask pointer-event-mask))
  (declare-values mask32)
  (or (encode-mask *pointer-event-mask-vector* pointer-event-mask
		   'pointer-event-mask-class)
      (x-type-error pointer-event-mask 'pointer-event-mask)))
