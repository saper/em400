
	lw r1, 14
	cw r1, 15
	jls fin
	hlt 040
fin:	hlt 077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT ir&0x3f : 0o77
