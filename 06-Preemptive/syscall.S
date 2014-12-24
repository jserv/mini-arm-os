.global syscall
syscall:
	/* r0 contains the task number */
	svc 0
	bx lr
