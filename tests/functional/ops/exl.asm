
	uj	start

	.res	128
stack:	.res	16
mask:	.word	0b0000001111000000

start:
	lw	r0, 0xfafa
	lw	r1, exlp
	rw	r1, 96

	lw	r1, stack
	rw	r1, 97

	im	mask

	exl	23
	hlt	040

exlp:
	hlt	077

; XPCT rz[6] : 0
; XPCT ir&0x3f : 0o77

; new process vector

; XPCT sr : 0b0000001110000000
; XPCT r0 : 0

; new stack pointer

; XPCT [97] : 134

; stack contents

; XPCT [130] : 160
; XPCT [131] : 0xfafa
; XPCT [132] : 0b0000001111000000
; XPCT [133] : 23
