.equ USR_MODE, 0x10
.equ SYS_MODE, 0x1F
.equ SVC_MODE, 0x13
.equ IRQ_MODE, 0x12
.equ INT_OFF, 0xC0

.global activate
activate:
	stmfd sp!, {r1-r12, lr}
	nop
	msr CPSR_c, SYS_MODE /* Sys mode with IRQ enabled and FIQ disabled*/
	mov sp, r0 /* MOVE TO THE STACK USER
	/* Load task's context */
	mov ip, r0
	ldmfd sp!, {r0-ip,lr}
	nop
	mov pc, lr
