#define RUN		0
#define READY		1
#define WAIT		2

typedef struct {
	unsigned int *sp;
	unsigned int regs[12];
	unsigned int lr;
	unsigned int pc;
	unsigned int cpsr;
	unsigned int mode;
	unsigned int num;
	int stacksize;

	int priority;
	int state;		/* Running, Ready, Waiting, Suspended */

	/* Next and previous task in the queue */
	struct taskstruct *qnext, *qprev;
} taskstruct;

void init_task(taskstruct * task, unsigned int* stack, void (*function)(void));
