
	lw	r1, 0b0110101010101010
	rw	r1, 110
	rz	111
	rz	112
	rz	113

	fi 110
	ki 111
	fi 112
	ki 113
	hlt 077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT [110] : 0b0110101010101010
; XPCT [111] : 0b0110101010101010
; XPCT [112] : 0b0000000000000000
; XPCT [113] : 0b0000000000000000
