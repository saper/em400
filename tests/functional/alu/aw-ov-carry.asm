; -1 + -32768

	lw r1, 0b1111111111111111
	aw r1, 0b1000000000000000
	; =    1 0111111111111111
	hlt 077

; XPCT rz[6] : 0
; XPCT sr : 0

; XPCT r1 : 32767
; XPCT Z : 0
; XPCT M : 1
; XPCT V : 1
; XPCT C : 1
