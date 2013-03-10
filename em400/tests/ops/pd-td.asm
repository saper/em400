.program "op/PD+TD"

; PRE r1 = 50
; PRE r2 = 60

	lw r3, 0b0000000000000001
	ou r3, 0b0000000000000011
	.data   err, err, ok, err
ok:
	mb blk

	pf 20

	lwt r1, 0
	lwt r2, 0

	tf 20

	hlt 077

data:	.res 7
blk:	.data 1

err:	hlt 077

.endprog

; XPCT int(rz(6)) : 0
; XPCT int(sr) : 1

; XPCT int(r1): 50
; XPCT int(r2): 60
; XPCT int(ic): 17