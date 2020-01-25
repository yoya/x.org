; $XConsortium: cr16.s,v 1.1 93/08/08 12:58:34 rws Exp $
        .SPACE  $TEXT$
        .SUBSPA $CODE$
        .export cr16
        .PROC
        .CALLINFO
        .ENTRY
cr16
        bv      (%rp)
	mfctl	16,%ret0
        .PROCEND
