#include "process.h"
#include "queue.h"

int create_process(int (*code_address)()) {
	uint64_t* stackptr = alloc_stack();
	if (stackptr == NULL) {return -1;}

	uint64_t* sp = stackptr + STACK_SIZE;

	for (int i = 0; i <= 32; ++i) {
		--sp;
		*sp = 0;
	}

	*(sp+30) = (uint64_t)code_address;
	
	PCB_t* pcb = alloc_pcb();
	if (pcb == NULL) {return -1;}

	pcb->sp = sp;
	pcb->pid = nextPID++;

	enqueue(&readyQueue, pcb);

	return 0;
}

uint64_t* alloc_stack() {
	static int next_stack = 0;
	if (readyQueue.size > MAX_PROCESSES) {
		return NULL;
	}
	return stacks[next_stack++];
}

PCB_t* alloc_pcb() {
	static int next_process = 0;
	if (readyQueue.size > MAX_PROCESSES) {
		return NULL;
	}
	return &pcbs[next_process++];
}

PCB_t *running; // Pointer to the currently running PCB
int nextPID = 1;
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
PCB_t pcbs[MAX_PROCESSES];
