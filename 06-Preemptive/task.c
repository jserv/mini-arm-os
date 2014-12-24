#include "task.h"

void init_task(taskstruct *task, unsigned int *stack, void (*func)(void))
{
	task->sp = stack;

 	task->sp[0] = task->regs[0];		/* r0 */
	task->sp[1] = task->regs[1];		/* r1 */
	task->sp[2] = task->regs[2];		/* r2 */
	task->sp[3] = task->regs[3];		/* r3 */
	task->sp[4] = task->regs[4];		/* r4 */
	task->sp[5] = task->regs[5];		/* r5 */
	task->sp[6] = task->regs[6];		/* r6 */
	task->sp[7] = task->regs[7];		/* r7 */
	task->sp[8] = task->regs[8];		/* r8 */
	task->sp[9] = task->regs[9];		/* r9 */
	task->sp[10] = task->regs[10];		/* r10 */
	task->sp[11] = task->regs[11];		/* r11 */
	task->sp[12] = task->regs[12];		/* r12 */
	task->sp[13] = (unsigned int) func;	/* lr */
}
