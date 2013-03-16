.prog "alu/AD"

; PRE [910] = 0b0000000000001000
; PRE [911] =                   0b1100000000000111
; PRE [912] = 0b0000000000001001
; PRE [913] =                   0b1100000000000011

; PRE [914] = 0b0100000000000000
; PRE [915] =                   0b0000000000000000
; PRE [916] = 0b0100000000000000
; PRE [917] =                   0b0000000000000000

; PRE [918] = 0b1100000000000000
; PRE [919] =                   0b0000000000000000
; PRE [920] = 0b1100000000000000
; PRE [921] =                   0b0000000000000000

; PRE [922] = 0b1111111111111111
; PRE [923] =                   0b1111111111111111
; PRE [924] = 0b0000000000000000
; PRE [925] =                   0b0000000000000001

	ld 910
	ad 912
	rd 510
	rw r0, 512

	ld 914
	ad 916
	rd 514
	rw r0, 516

	ld 918
	ad 920
	rd 518
	rw r0, 520

	ld 922
	ad 924
	rd 522
	rw r0, 524

	hlt 077

.finprog

; XPCT int(rz(6)) : 0
; XPCT int(sr) : 0

; XPCT bin([510]) : 0b0000000000010010
; XPCT bin([511]) :                   0b1000000000001010
; XPCT bin([512]) : 0b0000000000000000

; XPCT bin([514]) : 0b1000000000000000
; XPCT bin([515]) :                   0b0000000000000000
; XPCT bin([516]) : 0b0110000000000000

; XPCT bin([518]) : 0b1000000000000000
; XPCT bin([519]) :                   0b0000000000000000
; XPCT bin([520]) : 0b0101000000000000

; XPCT bin([522]) : 0b0000000000000000
; XPCT bin([523]) :                   0b0000000000000000
; XPCT bin([524]) : 0b1001000000000000

