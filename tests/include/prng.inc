; ------------------------------------------------------------------------
; Extract random bit from interrupted program address
; in: r7 - bit counter
; out: [r1, r2] - generated seed
; temp: r3
prngtimer:
	MD	[STACKP]	; load IC of the interrupted program
	LW	r3, [-4]

	SRZ	r3
	SLY	r1
	SRZ	r3
	SLY	r1

	CWT	r7, -16
	BLC	?E
	LW	r2, r1
	AWT	r7, 2

	LIP

; ------------------------------------------------------------------------
; Seed PRNG with a reasonably good 32-bit random number
; It is obtained by interrupting a tight loop 32 times with timer interrupt,
; each time storing least significant bit of the return address
; temp: r1, r2, r4, r7
otimv:	.res	1
seed:	.dword	1
prngseed:
	.res	1
	LWT	r1, 0
	LWT	r2, 0
	LWT	r7, -32
	LW	r1, [IV_TIMER]
	RW	r1, otimv
	LW	r1, prngtimer
	RW	r1, IV_TIMER
rngloop:
	NOP
	NOP
	NOP
	NOP
	NOP
	CWT	r7, 0
	JES	prngseed_fin
	UJS	rngloop
prngseed_fin:
	RD	seed
	LW	r4, [otimv]
	RW	r4, IV_TIMER
	UJ	[prngseed]

; ------------------------------------------------------------------------
; Park-Miller-Carta Pseudo-Random Number Generator implementation for MERA-400
; out: [r1, r2] - random 32-bit number
; temp: r3
	.const	constapmc 16807
lo:	.res	2
hi:	.res	2
tmp_r3:	.res	1

rand:	.res	1
	rw	r3, tmp_r3
	; lo = self.constapmc * (self.seed & 0xFFFF)
	lw	r2, constapmc
	mw	seed+1
	rd	lo
	; hi = self.constapmc * (self.seed >> 16)
	lw	r2, constapmc
	mw	seed
	rd	hi
	; lo += (hi & 0x7FFF) << 16
	lw	r1, [hi+1]
	er	r1, 1\0
	lwt	r2, 0
	ad	lo
	rd	lo
	; lo += hi >> 15
	lwt	r1, 0
	lw	r2, [hi]
	lw	r3, [hi+1]
	SLZ	r3
	sly	r2
	sly	r1
	ad	lo
	; lo = (lo & 0x7FFFFFFF) + (lo >> 31);
	SXU	r1
	er	r1, 1\0
	RD	lo
	lwt	r1, 0
	lwt	r2, 0
	SLX	r2
	AD	lo
	; self.seed = lo
	rd	seed
	; return lo
	lw	r3, [tmp_r3]
	uj	[rand]

