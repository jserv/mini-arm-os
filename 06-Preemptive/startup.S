.text
.code 32

.global vectors_start
.global vectors_end

.equ USR_MODE, 0x10
.equ SYS_MODE, 0x1F
.equ SVC_MODE, 0x13
.equ IRQ_MODE, 0x12
.equ INT_OFF, 0xC0

vectors_start:
	ldr pc, reset_handler_addr
	ldr pc, undef_handler_addr
	ldr pc, swi_handler_addr
	ldr pc, prefetch_abort_handler_addr
	ldr pc, data_abort_handler_addr
	b .
	ldr pc, irq_handler_addr
	ldr pc, fiq_handler_addr

reset_handler_addr:		.word reset_handler
undef_handler_addr:		.word undef_handler
swi_handler_addr:		.word swi_handler
prefetch_abort_handler_addr:	.word prefetch_abort_handler
data_abort_handler_addr:	.word data_abort_handler
irq_handler_addr:		.word irq_handler
fiq_handler_addr:		.word fiq_handler

vectors_end:

reset_handler:
	/* set Supervisor stack */
	mov sp, #0x80000
	
	msr CPSR_c, #(INT_OFF | SYS_MODE)
	mov sp, #0x40000
	
	msr CPSR_c, #(INT_OFF | IRQ_MODE)
	mov sp, #0x10000
	
	msr CPSR_c,#(INT_OFF | SVC_MODE)
	
	/* Copy table vector to address 0 */
	bl copy_vectors

	/* jump to main */
	b c_entry
	
irq_handler:
	/* Save the return value */
	sub lr, lr, #4
	stmfd sp!, {r0-ip, lr}^ /* Save the user task context */
	mov r0, sp
	bl saveTaskContext

	bl event_irq_handler

	/* Load kernel state */
	msr CPSR_c,SVC_MODE
	ldmfd sp!, {r1-r12, pc}
	nop
	
swi_handler:
	mov ip, r0

	/* Save user state */
	msr CPSR_c, #(INT_OFF | SYS_MODE)
	stmfd sp!, {r0-r12, lr}
	nop
	mov r0, sp
	bl saveTaskContext
	nop

	mov r0, ip
	bl event_swi_handler

	/* Load kernel state */
	msr CPSR_c, SVC_MODE
	ldmfd sp!, {r1-r12, pc}
	nop

.end
