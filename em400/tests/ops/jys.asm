.prog "op/JYS"

	lw r1, 0b1000000000000000
	slz r1
	jys fin
	hlt 077
fin:	hlt 077


.finprog

; XPCT int(rz(6)) : 0
; XPCT int(sr) : 0

; XPCT int(ic) : 6

