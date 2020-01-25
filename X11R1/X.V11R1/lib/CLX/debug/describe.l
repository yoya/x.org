;;; -*- Mode: Lisp; Package: XLIB; Syntax: COMMON-LISP; Base: 10; Lowercase: Yes; -*-
;;;===============================================================================
;;;                                    RESTRICTED RIGHTS LEGEND 
;;; Use,  duplication, or  disclosure  by  the  Government is subject to restrictions
;;; as set forth in subdivision (b)(3)(ii) of the Rights in Technical Data and
;;; Computer Software clause at 52.227-7013. 
;;;
;;; TEXAS INSTRUMENTS INCORPORATED, P.O. BOX 2909 AUSTIN, TEXAS 78769  
;;; Copyright (C) 1986, Texas Instruments Incorporated. All rights reserved.
;;;===============================================================================

;;;
;;; Change history:
;;;
;;;  Date	Author	Description
;;; -------------------------------------------------------------------------------------
;;; 07/15/87	LGO	Created

(defparameter *request-parameters* (make-array (length *request-names*)))

(defmacro x-request (name &rest fields)
  (unless (zerop (mod (length fields) 3))
    (format t "~%Field length not a multiple of 3 for ~a" name))
  (let ((request (position name *request-names* :test #'zetalisp:string-equal)))
     (if request
	 `(setf (aref *request-parameters* ,request) ',fields)
       `(format t "~%~s isn't an X11 request name" ',name))))

(defun describe-history (&optional (buffer (car *history*)))
  ;; Display an output history
  (reading-event (buffer :sizes (8 16))
    (do* ((i 0 (+ i length))
	  (size (length buffer-bbuf))
	  request
	  length)
	 ((>= i size)
	  (unless (= i size)
	    (format t "~%Buffer size ~d  Requests end at ~d" size i)))
      (setq request (read8 i))
      (setq length (* 4 (read16 (+ i 2))))
      (format t "~2%~a (~d) length ~d"
	      (request-name request) request length)
      (do ((parms (aref *request-parameters* request) (cdddr parms))
	   (j i))	  
	  ((endp parms))
	(let ((len (first parms))
	      (type (second parms))
	      (doc (third parms))
	      value)
	  (setq value (case len
			(1 (card8-get j))
			(2 (card16-get j))
			(4 (card32-get j))))
	  (if value
	      (progn (print-value j value type doc)
		     (incf j len))
	    (format t "~%  ~2d ~10a ~a"
		    j type doc)
	    (case type
	      (listofvalue
	       (format t " Words:~%")
	       (dotimes (k (floor (- length (- j i)) 4))
		 (format t " ~d" (card32-get j))
		 (incf j 4)))
	      (listofrectangle
	       (format t " Half-Words:~%")
	       (dotimes (k (floor (- length (- j i)) 2))
		 (format t " ~d" (card16-get j))
		 (incf j 2)))
	      (otherwise
	       (format t " Bytes:~%")
	       (dotimes (k (- length (- j i)))
		 (format t " ~d" (card8-get j))
		 (incf j))))))))))

(defun print-value (i value type doc &aux temp)
  (format t "~%  ~2d ~3d "
	  i value value)
  (if (consp type)
      (case (first type)
	(bitmask (format t "~a" (decode-mask (symbol-value (second type)) value))
		 (setq type (car type)))
	(member (if (null (setq temp (nth value (cdr type))))
		    (format t "*****ERROR*****")
		  (format t "~a" temp)
		  (setq type (car type)))))
    (case type
      ((window pixmap drawable cursor font gcontext colormap atom)
       (format t "[#x~x]" value)
       #+comment
       (let ((temp (lookup-resource-id display value)))
	 (when (eq (first type) 'atom)
	   (setq temp (lookup-xatom display value)))
	 (when temp (format t " (~s)" (type-of temp)))))
      (int16 (setq temp (signed-16 value))
	     (when (minusp temp) (format t "~d" temp)))
      (otherwise
       (when (and (numberp type) (not (= type value)))
	 (format t "*****ERROR*****")))))
  (format t "~30,10t ~10a ~a" type doc))

(defun describe-histories (n)
  ;; Display the last N output histories
  (dolist (h (nreverse (zetalisp:firstn (min n (length *history*)) *history*)))
    (describe-history h)))

(x-request createwindow
   1	1			opcode
   1	card8			depth
   2	8+n			request-length
   4	window			wid
   4	window			parent
   2	int16			x
   2	int16			y
   2	card16			width
   2	card16			height
   2	card16			border-width
   2	(member copyfromparent inputoutput inputonly) class
   4	(or (member copyfromparent) visualid)		visual
   4	(bitmask *create-bitmask*) value-mask
  4n	listofvalue		value-list
  )

(defparameter *create-bitmask*
	      #(background-pixmap background-pixel border-pixmap border-pixel bit-gravity
		win-gravity backing-store backing-planes backing-pixel override-redirect
		save-under event-mask do-not-propagate-mask colormap cursor))

(x-request changewindowattributes
   1	2			opcode
   1	x			unused
   2	3+n			request-length
   4	window			window
   4	(bitmask *create-bitmask*) value-mask
  4n	listofvalue		value-list
  )

(x-request getwindowattributes
   1	3			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request destroywindow
   1	4			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request destroysubwindows
   1	5			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request changesaveset
   1	6			opcode
   1	(member insert delete)	mode
   2	2			request-length
   4	window			window
)

(x-request reparentwindow
  1	7			opcode
   1	x			unused
   2	4			request-length
   4	window			window
   4	window			parent
   2	int16			x
   2	int16			y
)

(x-request mapwindow
   1	8			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request mapsubwindows
   1	9			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request unmapwindow
   1	10			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request unmapsubwindows
   1	11			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request configurewindow
   1	12			opcode
   1	x			unused
   2	3+n			request-length
   4	window			window
   2	bitmask			value-mask
   2	x			unused
  4n	listofvalue		value-list
)

(x-request circulatewindow
   1	13			opcode
   1	(member raiselowest lowerhighest) direction
   2	2			request-length
   4	window			window
)

(x-request getgeometry
   1	14			opcode
   1	x			unused
   2	2			request-length
   4	drawable		drawable
)

(x-request querytree
   1	15			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request internatom
   1	16			opcode
   1	bool			only-if-exists
   2	|2+(n+p)/4|		request-length
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request getatomname
   1	17			opcode
   1	x			unused
   2	2			request-length
   4	atom			atom
)

(x-request changeproperty
   1	18			opcode
   1	(member replace prepend append)	mode
   2	|6+(n+p)/4|		request-length
   4	window			window
   4	atom			property
   4	atom			type
   1	card8			format
   3	x			unused
   4	card32			length-of-data-in-format-units
   n	listofbyte		data
   p	x			unused
)

(x-request deleteproperty
   1	19			opcode
   1	x			unused
   2	3			request-length
   4	window			window
   4	atom			property
)

(x-request getproperty
   1	20			opcode
   1	bool			delete
   2	6			request-length
   4	window			window
   4	atom			property
   4	(or (member anypropertytype) atom) type
   4	card32			long-offset
   4	card32			long-length
)

(x-request listproperties
   1	21			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request setselectionowner
   1	22			opcode
   1	x			unused
   2	4			request-length
   4	(or (member none) window) owner
   4	atom			selection
   4	(or (member currenttime) timestamp) time
)

(x-request getselectionowner
   1	23			opcode
   1	x			unused
   2	2			request-length
   4	atom			selection
)

(x-request convertselection
   1	24			opcode
   1	x			unused
   2	6			request-length
   4	window			requestor
   4	atom			selection
   4	atom			target
   4	(or (member none) atom)	property
   4	(or (member currenttime) timestamp) time
)

(x-request sendevent
   1	25			opcode
   1	bool			propagate
   2	11			request-length
   4	(or (member pointerwindow inputfocus) window) destination
   4	setofevent		event-mask
  32	x			event
)

(x-request grabpointer
   1	26			opcode
   1	bool			owner-events
   2	6			request-length
   4	window			grab-window
   2	setofpointerevent	event-mask
   1	(member synchronous asynchronous) pointer-mode
   1	(member synchronous asynchronous) keyboard-mode
   4	(or (member none) window) confine-to
   4	(or (member none) cursor) cursor
   4	(or (member currenttime) timestamp) timestamp
)

(x-request ungrabpointer
   1	27			opcode
   1	x			unused
   2	2			request-length
   4	(or (member currenttime) timestamp) time
)

(x-request grabbutton
   1	28			opcode
   1	bool			owner-events
   2	6			request-length
   4	window			grab-window
   2	setofpointerevent	event-mask
   1	(member synchronous asynchronous) pointer-mode
   1	(member synchronous asynchronous) keyboard-mode
   4	(or (member none) window) confine-to
   4	(or (member none) cursor) cursor
   1	(or (member anybutton) button)button
   1	x			unused
   2	setofkeymask		modifiers
)

(x-request ungrabbutton
   1	29			opcode
   1	(or (member anybutton) button) button
   2	3			request-length
   4	window			grab-window
   2	setofkeymask		modifiers
   2	x			unused
)

(x-request changeactivepointergrab
   1	30			opcode
   1	x			unused
   2	4			request-length
   4	(or (member none) cursor) cursor
   4	(or (member currenttime) timestamp) time
   2	setofpointerevent	event-mask
   2	x			unused
)

(x-request grabkeyboard
   1	31			opcode
   1	bool			owner-events
   2	4			request-length
   4	window			grab-window
   4	(or (member currenttime) timestamp) time
   1	(member synchronous asynchronous) pointer-mode
   1	(member synchronous asynchronous) keyboard-mode
   2	x			unused
)

(x-request ungrabkeyboard
   1	32			opcode
   1	x			unused
   2	2			request-length
   4	(or (member currenttime) timestamp) time
)

(x-request grabkey
   1	33			opcode
   1	bool			owner-events
   2	4			request-length
   4	window			grab-window
   2	setofkeymask		modifiers
   1	(or (member anykey) keycode) key
   1	(member synchronous asynchronous) pointer-mode
   1	(member synchronous asynchronous) keyboard-mode
   3	x			unused
)

(x-request ungrabkey
   1	34			opcode
   1	(or (member anykey) keycode) key
   2	3			request-length
   4	window			grab-window
   2	setofkeymask		modifiers
   2	x			unused
)

(x-request allowevents
   1	35			opcode
   1	(member asyncpointer syncpointer replaypointer asynckeyboard synckeyboard replaykeyboard) mode
   2	2			request-length
   4	(or (member currenttime) timestamp) time
)

(x-request grabserver
   1	36			opcode
   1	x			unused
   2	1			request-length
)

(x-request ungrabserver
   1	37			opcode
   1	x			unused
   2	1			request-length
)

(x-request querypointer
   1	38			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request getmotionevents
   1	39			opcode
   1	x			unused
   2	4			request-length
   4	window			window
   4	(or (member currenttime) timestamp)		start
   4	(or (member currenttime) timestamp)		stop
)

(x-request translatecoords
   1	40			opcode
   1	x			unused
   2	4			request-length
   4	window			src-window
   4	window			dst-window
   2	int16			src-x
   2	int16			src-y
)

(x-request warppointer
   1	41			opcode
   1	x			unused
   2	6			request-length
   4	(or (member none) window)			src-window
   4	window			dst-window
   2	int16			src-x
   2	int16			src-y
   2	card16			src-width
   2	card16			src-height
   2	int16			dst-x
   2	int16			dst-y
)

(x-request setinputfocus
   1	42			opcode
   1	(member none pointerroot parent) revert-to
   2	3			request-length
   4	(or (member none pointerroot) window) focus
   4	(or (member currenttime) timestamp)		time
)

(x-request getinputfocus
   1	43			opcode
   1	x			unused
   2	1			request-length
)

(x-request querykeymap
   1	44			opcode
   1	x			unused
   2	1			request-length
)

(x-request openfont
   1	45			opcode
   1	x			unused
   2	|3+(n+p)/4|		request-length
   4	font			fid
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request closefont
   1	46			opcode
   1	x			unused
   2	2			request-length
   4	font			font
)

(x-request queryfont
   1	47			opcode
   1	x			unused
   2	2			request-length
   4	fontable		font
)

(x-request querytextextents
   1	48			opcode
   1	bool			odd-length-p
   2	|2+(2n+p)/4|		request-length
   4	fontable		font
  2n	string16		string
   p	x			unused
)

(x-request listfonts
   1	49			opcode
   1	x			unused
   2	|2+(n+p)/4|		request-length
   2	card16			max-names
   2	n			length-of-pattern
   n	string8			pattern
   p	x			unused
)

(x-request listfontswithinfo
   1	50			opcode
   1	x			unused
   2	|2+(n+p)/4|		request-length
   2	card16			max-names
   2	n			length-of-pattern
   n	string8			pattern
   p	x			unused
)

(x-request setfontpath
   1	51			opcode
   1	x			unused
   2	|2+(n+p)/4|		request-length
   2	card16			number-of-strs-in-path
   2	x			unused
   n	listofstr		path
   p	x			unused
)

(x-request getfontpath
   1	52			opcode
   1	x			unused
   2	1			request-list
)

(x-request createpixmap
   1	53			opcode
   1	card8			depth
   2	4			request-length
   4	pixmap			pid
   4	drawable		drawable
   2	card16			width
   2	card16			height
)

(x-request freepixmap
   1	54			opcode
   1	x			unused
   2	2			request-length
   4	pixmap			pixmap
)

(x-request creategc
   1	55			opcode
   1	x			unused
   2	4+n			request-length
   4	gcontext		cid
   4	drawable		drawable
   4	(bitmask *gc-bitmask*)	value-mask
  4n	listofvalue		value-list
)

(defconstant *gc-bitmask*
	     #(function plane-mask foreground
	       background line-width line-style cap-style join-style
	       fill-style fill-rule tile stipple tile-stipple-x-origin
	       tile-stipple-y-origin font subwindow-mode graphics-exposures clip-x-origin
	       clip-y-origin clip-mask dash-offset dashes arc-mode))


(x-request changegc
   1	56			opcode
   1	x			unused
   2	3+n			request-length
   4	gcontext		gc
   4	(bitmask *gc-bitmask*)	value-mask
  4n	listofvalue		value-list
)

(x-request copygc
   1	57			opcode
   1	x			unused
   2	4			request-length
   4	gcontext		src-gc
   4	gcontext		dst-gc
   4	(bitmask *gc-bitmask*)	value-mask
)

(x-request setdashes
   1	58			opcode
   1	x			unused
   2	|3+(n+p)/4|		request-length
   4	gcontext		gc
   2	card16			dash-offset
   2	n			length-of-dashes
   n	listofcard8		dashes
   p	x			unused
)

(x-request setcliprectangles
   1	59			opcode
   1	(member unsorted ysorted yxsorted yxbanded) ordering
   2	3+2n			request-length
   4	gcontext		gc
   2	int16			clip-x-origin
   2	int16			clip-y-origin
  8n	listofrectangle		rectangles
)

(x-request freegc
   1	60			opcode
   1	x			unused
   2	2			request-length
   4	gcontext		gc
)

(x-request cleartobackground
   1	61			opcode
   1	bool			exposures
   2	4			request-length
   4	window			window
   2	int16			x
   2	int16			y
   2	card16			width
   2	card16			height
)

(x-request copyarea
   1	62			opcode
   1	x			unused
   2	7			request-length
   4	drawable		src-drawable
   4	drawable		dst-drawable
   4	gcontext		gc
   2	int16			src-x
   2	int16			src-y
   2	int16			dst-x
   2	int16			dst-y
   2	card16			width
   2	card16			height
)

(x-request copyplane
   1	63			opcode
   1	x			unused
   2	8			request-length
   4	drawable		src-drawable
   4	drawable		dst-drawable
   4	gcontext		gc
   2	int16			src-x
   2	int16			src-y
   2	int16			dst-x
   2	int16			dst-y
   2	card16			width
   2	card16			height
   4	card32			bit-plane
)

(x-request polypoint
   1	64			opcode
   1	(member origin previous) coordinate-mode
   2	3+n			request-length
   4	drawable		drawable
   4	gcontext		gc
  4n	listofpoint		points
)

(x-request polyline
   1	65			opcode
   1	(member origin previous) coordinate-mode
   2	3+n			request-length
   4	drawable		drawable
   4	gcontext		gc
  4n	listofpoint		points
)

(x-request polysegment
   1	66			opcode
   1	x			unused
   2	3+2n			request-length
   4	drawable		drawable
   4	gcontext		gc
  8n	listofsegment		segments
)

(x-request polyrectangle
   1	67			opcode
   1	x			unused
   2	3+2n			request-length
   4	drawable		drawable
   4	gcontext		gc
  8n	listofrectangle		rectangles
)

(x-request polyarc
   1	68			opcode
   1	x			unused
   2	3+3n			request-length
   4	drawable		drawable
   4	gcontext		gc
 12n	listofarc		arcs
)

(x-request fillpoly
   1	69			opcode
   1	x			unused
   2	4+n			request-length
   4	drawable		drawable
   4	gcontext		gc
   1	(member complex nonconvex convex) shape
   1	(member origin previous) coordinate-mode
   2	x			unused
  4n	listofpoint		points
)

(x-request polyfillrectangle
   1	70			opcode
   1	x			unused
   2	3+2n			request-length
   4	drawable		drawable
   4	gcontext		gc
  8n	listofrectangle		rectangles
)

(x-request polyfillarc
   1	71			opcode
   1	x			unused
   2	3+3n			request-length
   4	drawable		drawable
   4	gcontext		gc
 12n	listofarc		arcs
)

(x-request putimage
   1	72			opcode
   1	(bitmap xypixmap zpixmap) format
   2	|6+(n+p)/4|		request-length
   4	drawable		drawable
   4	gcontext		gc
   2	card16			width
   2	card16			height
   2	int16			dst-x
   2	int16			dst-y
   1	card8			left-pad
   1	card8			depth
   2	x			unused
   n	listofbyte		data
   p	x			unused
)

(x-request getimage
   1	73			opcode
   1	(member error xypixmap zpixmap)	format
   2	5			request-length
   4	drawable		drawable
   2	int16			x
   2	int16			y
   2	card16			width
   2	card16			height
   4	card32			plane-mask
)

(x-request polytext8
   1	74			opcode
   1	x			unused
   2	|4+(n+p)/4|		request-length
   4	drawable		drawable
   4	gcontext		gc
   2	int16			x
   2	int16			y
   n	listoftextitem8		items
   p	x			unused
)

(x-request polytext16
   1	75			opcode
   1	x			unused
   2	|4+(n+p)/4|		request-length
   4	drawable		drawable
   4	gcontext		gc
   2	int16			x
   2	int16			y
   n	listoftextitem16	items
   p	x			unused
)

(x-request imagetext8
   1	76			opcode
   1	n			length-of-string
   2	|4+(n+p)/4|		request-length
   4	drawable		drawable
   4	gcontext		gc
   2	int16			x
   2	int16			y
   n	string8			string
   p	x			unused
)

(x-request imagetext16
   1	77			opcode
   1	n			number-of-char2bs-in-string
   2	|4+(2n+p)/4|		request-length
   4	drawable		drawable
   4	gcontext		gc
   2	int16			x
   2	int16			y
  2n	string16		string
   p	x			unused
)

(x-request createcolormap
   1	78			opcode
   1	(member none all)	alloc
   2	4			request-length
   4	colormap		mid
   4	window			window
   4	visualid		visual
)

(x-request freecolormap
   1	79			opcode
   1	x			unused
   2	2			request-length
   4	colormap		cmap
)

(x-request copycolormapandfree
   1	80			opcode
   1	x			unused
   2	3			request-length
   4	colormap		mid
   4	colormap		src-cmap
)

(x-request installcolormap
   1	81			opcode
   1	x			unused
   2	2			request-length
   4	colormap		cmap
)

(x-request uninstallcolormap
   1	82			opcode
   1	x			unused
   2	2			request-length
   4	colormap		cmap
)

(x-request listinstalledcolormaps
   1	83			opcode
   1	x			unused
   2	2			request-length
   4	window			window
)

(x-request listinstalledcolormaps reply
   1	1			reply
   1	x			unused
   2	card16			sequence number
   4	n			reply length
   2	n			number-of-colormaps-in-cmaps
  22	x			unused
  4n	listofcolormap		cmaps
)

(x-request alloccolor
   1	84			opcode
   1	x			unused
   2	4			request-length
   4	colormap		cmap
   2	card16			red
   2	card16			green
   2	card16			blue
   2	x			unused
)

(x-request allocnamedcolor
   1	85			opcode
   1	x			unused
   2	|3+(n+p)/4|		request-length
   4	colormap		cmap
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request alloccolorcells
   1	86			opcode
   1	bool			contiguous
   2	3			request-length
   4	colormap		cmap
   2	card16			colors
   2	card16			planes
)

(x-request alloccolorplanes
   1	87			opcode
   1	bool			contiguous
   2	4			request-length
   4	colormap		cmap
   2	card16			colors
   2	card16			reds
   2	card16			greens
   2	card16			blues
)

(x-request freecolors
   1	88			opcode
   1	x			unused
   2	3+n			request-length
   4	colormap		cmap
   4	card32			plane-mask
  4n	listofcard32		pixels
)

(x-request storecolors
   1	89			opcode
   1	x			unused
   2	2+3n			request-length
   4	colormap		cmap
 12n	listofcoloritem		items
)

(x-request storenamedcolor
   1	90			opcode
   1	color-mask		do-red_do-green_do-blue
   2	|4+(n+p)/4|		request-length
   4	colormap		cmap
   4	card32			pixel
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request querycolors
   1	91			opcode
   1	x			unused
   2	2+n			request-length
   4	colormap		cmap
  4n	listofcard32		pixels
)

(x-request lookupcolor
   1	92			opcode
   1	x			unused
   2	|3+(n+p)/4|		request-length
   4	colormap		cmap
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request createcursor
   1	93			opcode
   1	x			unused
   2	8			request-length
   4	cursor			cid
   4	pixmap			source
   4	(or (member none) pixmap) mask
   2	card16			fore-red
   2	card16			fore-green
   2	card16			fore-blue
   2	card16			back-red
   2	card16			back-green
   2	card16			back-blue
   2	card16			x
   2	card16			y
)

(x-request createglyphcursor
   1	94			createglyphcursor
   1	x			unused
   2	8			request-length
   4	cursor			cid
   4	font			source-font
   4	(or (member none) font)	mask-font
   2	card16			source-char
   2	card16			mask-char
   2	card16			fore-red
   2	card16			fore-green
   2	card16			fore-blue
   2	card16			back-red
   2	card16			back-green
   2	card16			back-blue
)

(x-request freecursor
   1	95			opcode
   1	x			unused
   2	2			request-length
   4	cursor			cursor
)

(x-request recolorcursor
   1	96			opcode
   1	x			unused
   2	5			request-length
   4	cursor			cursor
   2	card16			fore-red
   2	card16			fore-green
   2	card16			fore-blue
   2	card16			back-red
   2	card16			back-green
   2	card16			back-blue
)

(x-request querybestsize
   1	97			opcode
   1	(member cursor tile stipple) class
   2	3			request-length
   4	drawable		drawable
   2	card16			width
   2	card16			height
)

(x-request queryextension
   1	98			opcode
   1	x			unused
   2	|2+(n+p)/4|		request-length
   2	n			length-of-name
   2	x			unused
   n	string8			name
   p	x			unused
)

(x-request listextensions
   1	99			opcode
   1	x			unused
   2	1			request-length
)

(x-request setkeyboardmapping
   1	100			opcode
   1	n			keycode-count
   2	2+nm			request-length
   1	keycode			first-keycode
   1	m			keysyms-per-keycode
   2	x			unused
 4nm	listofkeysym		keysyms
)

(x-request getkeyboardmapping
   1	101			opcode
   1	x			unused
   2	2			request-length
   1	keycode			first-keycode
   1	card8			count
   2	x			unused
)

(x-request changekeyboardcontrol
   1	102			opcode
   1	x			unused
   2	2+n			request-length
   4	bitmask			value-mask
  4n	listofvalue		value-list
)

(x-request getkeyboardcontrol
   1	103			opcode
   1	x			unused
   2	1			request-length
)

(x-request bell
   1	104			opcode
   1	int8			percent
   2	1			request-length
)

(x-request changepointercontrol
   1	105			opcode
   1	x			unused
   2	3			request-length
   2	int16			acceleration-numerator
   2	int16			acceleration-denominator
   2	int16			threshold
   1	bool			do-acceleration
   1	bool			do-threshold
)

(x-request getpointercontrol
   1	106			getpointercontrol
   1	x			unused
   2	1			request-length
)

(x-request setscreensaver
   1	107			opcode
   1	x			unused
   2	3			request-length
   2	int16			timeout
   2	int16			interval
   1	(member no yes default)	prefer-blanking
   1	(member no yes default)	allow-exposures
   2	x			unused
)

(x-request getscreensaver
   1	108			opcode
   1	x			unused
   2	1			request-length
)

(x-request changehosts
   1	109			opcode
   1	(member insert delete)	mode
   2	|2+(n+p)/4|		request-length
   1	(member internet decnet chaos) family
   1	x			unused
   2	card16			length-of-address
   n	listofcard8		address
   p	x			unused
)

(x-request listhosts
   1	110			opcode
   1	x			unused
   2	1			request-length
)

(x-request changeaccesscontrol
   1	111			opcode
   1	(member disable enable)	mode
   2	1			request-length
)

(x-request changeclosedownmode
   1	112			opcode
   1	(member destroy retainpermanent retaintemporary) mode
   2	1			request-length
)

(x-request killclient
   1	113			opcode
   1	x			unused
   2	2			request-length
   4	(member alltemporary card32) resource
)

(x-request rotateproperties
   1	114			opcode
   1	x			unused
   2	3+n			request-length
   4	window			window
   2	n			number-of-properties
   2	int16			delta
  4n	listofatom		properties
)

(x-request forcescreensaver
   1	115			forcescreensaver
   1	(member reset activate)	mode
   2	1			request-length
)

(x-request setpointermapping
   1	116			opcode
   1	n			length-of-map
   2	|1+(n+p)/4|		request-length
   n	listofcard8		map
   p	x			unused
)

(x-request getpointermapping
   1	117			opcode
   1	x			unused
   2	1			request-length
)

(x-request setmodifiermapping
   1	118			opcode
   1	keycode			lock
   2	5			request-length
   1	keycode			shift_a
   1	keycode			shift_b
   1	keycode			control_a
   1	keycode			control_b
   1	keycode			mod1_a
   1	keycode			mod1_b
   1	keycode			mod2_a
   1	keycode			mod2_b
   1	keycode			mod3_a
   1	keycode			mod3_b
   1	keycode			mod4_a
   1	keycode			mod4_b
   1	keycode			mod5_a
   1	keycode			mod5_b
   2	x			unused
)

(x-request getmodifiermapping
   1	119			opcode
   1	x			unused
   2	1			request-length
)

#+comment
(x-request nooperation
   1	127			opcode
   1	x			unused
   2	1			request-length
)

