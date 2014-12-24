.text
.code 32

.global vectors_start
.global vectors_end

.EQU USR_MODE, 0x10
.EQU SYS_MODE, 0x1F
.EQU SVC_MODE, 0x13
.EQU IRQ_MODE, 0x12
.EQU INT_OFF, 0xC0

vectors_start:
	LDR PC, reset_handler_addr
	LDR PC, undef_handler_addr
	LDR PC, swi_handler_addr
	LDR PC, prefetch_abort_handler_addr
	LDR PC, data_abort_handler_addr
	B .
	LDR PC, irq_handler_addr
	LDR PC, fiq_handler_addr

reset_handler_addr: .word reset_handler
undef_handler_addr: .word undef_handler
swi_handler_addr: .word swi_handler
prefetch_abort_handler_addr: .word prefetch_abort_handler
data_abort_handler_addr: .word data_abort_handler
irq_handler_addr: .word irq_handler
fiq_handler_addr: .word fiq_handler

vectors_end:

reset_handler:
	/* set Supervisor stack */
	MOV sp, #0x80000
	
	MSR CPSR_c, #INT_OFF|SYS_MODE
	MOV sp, #0x40000
	
	MSR CPSR_c, #INT_OFF|IRQ_MODE
	MOV sp, #0x10000
	
	MSR CPSR_c,#INT_OFF|SVC_MODE
	
	/* Copy table vector to address 0 */
	BL copy_vectors

	/* jump to main */
	B c_entry
	
irq_handler:
	/* Save the return value */
	SUB lr,lr,#4
	STMFD sp!, {r0-ip, lr}^ /* Save the user task context */
	MOV r0, sp
	BL saveTaskContext

	BL event_irq_handler

	/* Load kernel state */
	MSR CPSR_c,SVC_MODE
	LDMFD sp!,{r1-r12,pc}
	NOP
	
swi_handler:
	MOV ip, r0

	/* Save user state */
	MSR CPSR_c, #INT_OFF|SYS_MODE
	STMFD sp!,{r0-r12,lr}
	NOP
	mov r0, sp
	BL saveTaskContext
	NOP

	MOV r0, ip
	BL event_swi_handler

	/* Load kernel state */
	MSR CPSR_c,SVC_MODE
	LDMFD sp!,{r1-r12,pc}
	NOP

.end
