.global syscall
syscall:
	/* r0 contains the task number */
	swi 0
	bx lr
