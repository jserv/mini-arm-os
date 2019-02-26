#include <stdint.h>
#include "threads.h"
#include "config.h"
#include "malloc.h"
#include "reg.h"
#include "stream.h"
#include "stdlib.h"

#define THREAD_PSP	0xFFFFFFFD

/* Thread Control Block */
typedef struct {
	void *stack;
	void *orig_stack;
	uint8_t in_use;
	int32_t pid;
} tcb_t;

static tcb_t tasks[MAX_TASKS];
static int lastTask;

// Calling convention: https://stackoverflow.com/questions/261419/what-registers-to-save-in-the-arm-c-calling-convention
// Exception entry and return: in Cortex-M user guide
// Stacking: in Cortex-M technical Manual
/*
 *	This is automatical stacking after exception
 *	Note LR is indirect
 *
 *         -------
 *        |  prev | <- old SP
 *         -------
 *        |  xPSR |
 *         -------
 *        |   PC  |
 *         -------
 *        |   LR  |
 *         -------
 *        |  r12  |
 *         -------
 *        |   r3  |
 *         -------
 *        |   r2  |
 *         -------
 *        |   r1  |
 *         -------
 *        |   r0  | <- SP
 *         -------
 * 		For EXC_RETURN
 *
 * 		0xFFFFFFF1	0001
 *		Return to Handler mode.
 *
 *		Exception return gets state from the main stack.
 *
 *		Execution uses MSP after return.
 *
 *		0xFFFFFFF9	1001
 *		Return to Thread mode.
 *
 *		Exception return gets state from MSP.
 *
 *		Execution uses MSP after return.
 *
 *		0xFFFFFFFD	1101
 *		Return to Thread mode.
 *
 *		Exception return gets state from PSP.
 *
 *		Execution uses PSP after return.
 */


void  svc_handler()
{
	// get svc number, which is next to PC
	int svc = -1;
	asm volatile(
	    "tst	lr, 0x3\n"
	    "ite	eq\n"
	    "mrseq	r0, msp\n"
	    "mrsne	r0, psp\n"
	    "ldr	r1, [r0, #24]\n"
	    "ldrb	%[svc], [r1, -2]"
	    : [svc] "+rm"(svc)
	);
	puts("received svc :");
	char s[10];
	itoa(svc, s);
	puts(s);
	puts("\r\n");
	return;

}

/* Caution: Without naked attribute, GCC will normally push r7 which is used
 * for stack pointer. If so, after restoring the tasks' context, we will get
 * wrong stack pointer. PendSV handles calling convention itself.
 */
void __attribute__((naked)) pendsv_handler()
{
	/* Save the old task's context[1] */
	asm volatile("mrs   r0, psp\n"
	             "stmdb r0!, {r4-r11, lr}\n");
	/* To save the last task's sp from r0 into its tcb*/
	asm volatile("mov   %0, r0\n" : "=r"(tasks[lastTask].stack));

	/* Find a new task to run */
	while (1) {
		lastTask++;
		if (lastTask == MAX_TASKS)
			lastTask = 0;
		if (tasks[lastTask].in_use) {
			/* Move the task's stack pointer address into r0 */
			asm volatile("mov r0, %0\n" : : "r"(tasks[lastTask].stack));
			/* Restore the new task's context and jump to the task */
			asm volatile("ldmia r0!, {r4-r11, lr}\n"
			             "msr psp, r0\n"
			             "bx lr\n");
		}
	}
}


/* As PendSV does, this function also switches context and
 * maintain assembly on its own. This avoids the compiler
 * modifying register at prologue/epilogue sequences and
 * corrupts inline-assembly usage.
 */
void __attribute__((naked)) thread_start()
{
	lastTask = 0;
	CONTROL_Type user_ctx = {
		.b.nPRIV = 1,
		.b.SPSEL = 1
	};

	/* Reset APSR before context switch
	 * Make sure we have a _clean_ PSR for the task.
	 */
	asm volatile("mov r0, #0\n"
	             "msr APSR_nzcvq, r0\n");
	/* Load user task's context and jump to the task */
	__set_PSP((uint32_t) tasks[lastTask].stack);
	__set_CONTROL(user_ctx.w);
	__ISB();

	/* This is how we simulate stack handling that pendsv_handler
	 * does. Thread_create sets 17 entries in stack, and the 9
	 * entries we pop here will be pushed back in pendsv_handler
	 * in the same order.
	 *
	 *
	 *                      pop {r4-r11, lr}
	 *                      ldr r0, [sp]
	 *          stack
	 *  offset -------
	 *        |   16  | <- Reset value of PSR
	 *         -------
	 *        |   15  | <- Task entry
	 *         -------
	 *        |   14  | <- LR for task
	 *         -------
	 *        |  ...  |                             register
	 *         -------                              -------
	 *        |   9   | <- Task argument ---->     |   r0  |
	 * psp after pop--<                             -------
	 *        |   8   | <- EXC_RETURN    ---->     |   lr  |
	 *         -------                              -------
	 *        |   7   |                            |  r11  |
	 *         -------                              -------
	 *        |  ...  |                            |  ...  |
	 *         -------                              -------
	 *        |   0   |                            |   r4  |
	 * psp ->  -------                              -------
	 *
	 * Instead of "pop {r0}", use "ldr r0, [sp]" to ensure consistent
	 * with the way how PendSV saves _old_ context[1].
	 */
	asm volatile("pop {r4-r11, lr}\n"
	             "ldr r0, [sp]\n");
	/* Okay, we are ready to run first task, get address from
	 * stack[15]. We just pop 10 register so #24 comes from
	 * (15 - 9) * sizeof(entry of sp) = 6 * 4.
	 */
	asm volatile("ldr pc, [sp, #24]\n");

	/* Never reach here */
	while (1);

}

int thread_create(void (*run)(void *), void *userdata)
{
	/* Find a free thing */
	int threadId = 0;
	uint32_t *stack;

	for (threadId = 0; threadId < MAX_TASKS; threadId++) {
		if (tasks[threadId].in_use == 0)
			break;
	}

	if (threadId == MAX_TASKS)
		return -1;

	/* Create the stack */
	stack = (uint32_t *) malloc(STACK_SIZE * sizeof(uint32_t));
	tasks[threadId].orig_stack = stack;
	if (stack == 0)
		return -1;

	stack += STACK_SIZE - 17; /* End of stack, minus what we are about to push */
	stack[8] = (unsigned int) THREAD_PSP;
	stack[9] = (unsigned int) userdata;
	stack[14] = (unsigned) &thread_self_terminal;
	stack[15] = (unsigned int) run;
	stack[16] = (unsigned int) 0x01000000; /* PSR Thumb bit */

	/* Construct the control block */
	tasks[threadId].stack = stack;
	tasks[threadId].in_use = 1;

	return threadId;
}

void thread_kill(int thread_id)
{
	tasks[thread_id].in_use = 0;

	/* Free the stack */
	free(tasks[thread_id].orig_stack);
}

void thread_self_terminal()
{
	/* This will kill the stack.
	 * For now, disable context switches to save ourselves.
	 */
	__disable_irq();
	thread_kill(lastTask);
	__enable_irq();

	/* And now wait for death to kick in */
	while (1);
}

/* [1] Exception entry only saves R0-R4, R12, LR, PC and xPSR,
 *     which means PendSV has to save other context (R4-R11) itself to
 *     complete context switch.
 *     Also, pushing LR (=EXC_RETURN) into thread stack before switching
 *     task is needed in order to perform exception return next time the task
 *     is selected to run.
 *     Reference:
 *         Cortex-M4 Devices: Generic User Guide (ARM DUI 0553A):
 *         2.3.7 Exception entry and return
 */
