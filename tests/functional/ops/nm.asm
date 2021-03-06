
	.equ int_nomem 0x40 + 2
	.equ stackp 0x61

	lw r3, stack
	rw r3, stackp
	lw r3, nomem_proc
	rw r3, int_nomem

	lw r1, 0b0000000000000001
	ou r1, 0b0000000000000011
	.word   err, err, ok, err
ok:
	mb blk
	im blk

	lwt r1, 0
	pw r1, 10
	pw r1, 11

	lw r1, 0b1111111100000000
	om r1, 10
	lw r1, 0b0101010101010101
	nm r1, 10

	lw r2, 0
	nm r2, 11

	hlt 077

err:	hlt 040
blk:	.word 0b0100000000000001

nomem_proc:
	hlt 040
stack:

; XPCT rz[6] : 0
; XPCT sr : 0b0100000000000001
; XPCT ir&0x3f : 0o77

; XPCT [1:10] : 0b0101010100000000
; XPCT [1:11] : 0
; XPCT r0 : 0b1000000000000000
