#ifndef __ASM_H_
#define __ASM_H_

unsigned int *activate(unsigned int *stack);
void *init_activate_env(unsigned int *stack);
void syscall(void);

#endif
