#include <stdint.h>
#include <stddef.h>
#include "queue.h"

int next_pid = 0; // Process ID counter
int current_pid = 0;
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks

int create_process(int (*code_address)());
uint64_t* alloc_stack();
PCB_t* alloc_pcb();

int create_process(int (*code_address)()) {
    PCB_t *pcb = NULL; // Get the next PCB
    uint64_t *stackptr = alloc_stack(); // Allocate a stack
    if (stackptr == NULL) {
        return -1; // Error: stack allocation failed
    }

    uint64_t *sp = stackptr + STACK_SIZE; // Set sp to the top of the stack

    for (int i = 0; i <= 32; i++) {
        sp--; 
        *sp = 0; // Initialize registers to 0
    }

    sp[30] = (uint64_t)code_address; // Set the code address

	pcb = alloc_pcb((uint64_t)sp, code_address, NULL);
	if (pcb == NULL) {
		return -1;
	}
	pcb->sp = sp;
	pcb->pid = next_pid;

    enqueue(&ready_queue, pcb); // Enqueue the PCB

    return 0; // No errors
}

uint64_t* alloc_stack() {
	static int next_stack = 0; // Track the next stack to use
    if (next_stack >= MAX_PROCESSES) {
	    return NULL; // No more stacks available
	}
	return stacks[next_stack++]; // Return the next available stack
}

PCB_t* alloc_pcb(uint64_t sp, uint32_t pid, PCB_t *next) {
	static int pcb_count = 0; // Track the next stack to use
	if (pcb_count >= MAX_PROCESSES) {
		return NULL;
	}
	PCB_t* newPCB;
	++pcb_count;

	(*newPCB).sp = sp;
	(*newPCB).pid = pid;
	(*newPCB).next = next;

	return newPCB;
}