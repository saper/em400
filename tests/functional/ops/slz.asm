
	lw r1, 0b1000000000000001
	slz r1
	lw r3, r1
	rpc r2

	slz r1

	hlt 077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT r3 : 0b0000000000000010
; XPCT r2 : 0b0000000100000000

; XPCT r1 : 0b0000000000000100
; XPCT r0 : 0b0000000000000000
