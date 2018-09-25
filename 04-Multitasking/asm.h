#ifndef __ASM_H_
#define __ASM_H_

unsigned int *activate(unsigned int *stack);
void task_init_env(unsigned int *stack);
void syscall(void);

#endif
