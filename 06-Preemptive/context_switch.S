.EQU USR_MODE, 0x10
.EQU SYS_MODE, 0x1F
.EQU SVC_MODE, 0x13
.EQU IRQ_MODE, 0x12
.EQU INT_OFF, 0xC0

.global activate
activate:
	STMFD sp!, {r1-r12, lr}
	NOP
	msr CPSR_c, SYS_MODE /* Sys mode with IRQ enabled and FIQ disabled*/
	mov sp, r0 /* MOVE TO THE STACK USER
	/* Load task's context */
	mov ip, r0
	LDMFD sp!, {r0-ip,lr}
	NOP
	mov pc, lr
