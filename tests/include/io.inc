
; channel numbers for I/O commands

	.const	IO_CHAN_0	0\14
	.const	IO_CHAN_1	1\14
	.const	IO_CHAN_2	2\14
	.const	IO_CHAN_3	3\14
	.const	IO_CHAN_4	4\14
	.const	IO_CHAN_5	5\14
	.const	IO_CHAN_6	6\14
	.const	IO_CHAN_7	7\14
	.const	IO_CHAN_8	8\14
	.const	IO_CHAN_9	9\14
	.const	IO_CHAN_10	10\14
	.const	IO_CHAN_11	11\14
	.const	IO_CHAN_12	12\14
	.const	IO_CHAN_13	13\14
	.const	IO_CHAN_14	14\14
	.const	IO_CHAN_15	15\14

; general channel I/O commands

	.const	IO_RESET	0b000\2 + 0b00\4	; IN
	.const	IO_INTSPEC	0b000\2 + 0b01\4	; IN
	.const	IO_EXISTS	0b000\2 + 0b10\4	; IN

