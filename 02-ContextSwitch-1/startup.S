.global _Reset
_Reset:
	ldr sp, =0x07FFFFFF
	bl c_entry
	b .
