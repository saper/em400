
	lw	r0, ?Y
	lw	r1, 0b1000000000000001
	sry	r1
	rpc	r2
	lw	r3, r1

	sry	r1
	rpc	r4
	lw	r5, r1

	sry	r1

	hlt	077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT r3 : 0b1100000000000000
; XPCT r2 : 0b0000000100000000

; XPCT r5 : 0b1110000000000000
; XPCT r4 : 0b0000000000000000

; XPCT r1 : 0b0111000000000000
; XPCT r0 : 0b0000000000000000
