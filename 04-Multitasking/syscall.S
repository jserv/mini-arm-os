.syntax unified

.global syscall
syscall:
	nop
	svc 0
	nop
	bx lr
