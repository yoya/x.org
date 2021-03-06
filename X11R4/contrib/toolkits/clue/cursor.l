;;; -*- Mode:Lisp; Package:CLUEI; Syntax:COMMON-LISP; Base:10; Lowercase:T -*-

;;;
;;;			 TEXAS INSTRUMENTS INCORPORATED
;;;				  P.O. BOX 149149
;;;			       AUSTIN, TEXAS 78714-9149
;;;
;;; Copyright (C)1987-1990 Texas Instruments Incorporated.
;;;
;;; Permission is granted to any individual or institution to use, copy, modify,
;;; and distribute this software, provided that this complete copyright and
;;; permission notice is maintained, intact, in all copies and supporting
;;; documentation.
;;;
;;; Texas Instruments Incorporated provides this software "as is" without
;;; express or implied warranty.
;;;

(in-package "CLUEI")

(defparameter *cursor-names*
	      '(("X_cursor"		0)
		("arrow"		2)
		("based_arrow_down"	4)
		("based_arrow_up"	6)
		("boat"			8)
		("bogosity"		10)
		("bottom_left_corner"	12)
		("bottom_right_corner"	14)
		("bottom_side"		16)
		("bottom_tee"		18)
		("box_spiral"		20)
		("center_ptr"		22)
		("circle"		24)
		("clock"		26)
		("coffee_mug"		28)
		("cross"		30)
		("cross_reverse"	32)
		("crosshair"		34)
		("diamond_cross"	36)
		("dot"			38)
		("dotbox"		40)
		("double_arrow"		42)
		("draft_large"		44)
		("draft_small"		46)
		("draped_box"		48)
		("exchange"		50)
		("fleur"		52)
		("gobbler"		54)
		("gumby"		56)
		("hand1"		58)
		("hand2"		60)
		("heart"		62)
		("icon"			64)
		("iron_cross"		66)
		("left_ptr"		68)
		("left_side"		70)
		("left_tee"		72)
		("leftbutton"		74)
		("ll_angle"		76)
		("lr_angle"		78)
		("man"			80)
		("middlebutton"		82)
		("mouse"		84)
		("pencil"		86)
		("pirate"		88)
		("plus"			90)
		("question_arrow"	92)
		("right_ptr"		94)
		("right_side"		96)
		("right_tee"		98)
		("rightbutton"		100)
		("rtl_logo"		102)
		("RTL_logo"		102)
		("sailboat"		104)
		("sb_down_arrow"	106)
		("sb_h_double_arrow"	108)
		("sb_left_arrow"	110)
		("sb_right_arrow"	112)
		("sb_up_arrow"		114)
		("sb_v_double_arrow"	116)
		("shuttle"		118)
		("sizing"		120)
		("spider"		122)
		("spraycan"		124)
		("star"			126)
		("target"		128)
		("tcross"		130)
		("top_left_arrow"	132)
		("top_left_corner"	134)
		("top_right_corner"	136)
		("top_side"		138)
		("top_tee"		140)
		("trek"			142)
		("ul_angle"		144)
		("umbrella"		146)
		("ur_angle"		148)
		("watch"		150)
		("xterm"		152)
		))
