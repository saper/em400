
	lw r1, 14
	sw r1, 19
	jm fin
	hlt 040
fin:	hlt 077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT ir&0x3f : 0o77
