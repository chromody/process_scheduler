/*
	Author: Jesus Villanueva-Segovia
	Name: process.c
	Purpose: This contains the functions to manage processes
*/
#include "process.h"
#include "queue.h"

//--Variables ----------------------------------------------------------------------------------------------------
int nextPID = 1;
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
PCB_t pcbs[MAX_PROCESSES];
//----------------------------------------------------------------------------------------------------------------

//--Functions ----------------------------------------------------------------------------------------------------

// Name: create_process
// Arguments: integer pointer to a function
// Purpose: This will create a process. It will allocate and initialize its stack and pcb, then it will place that pcb on the ready queue.
int create_process(int (*code_address)()) {
	uint64_t* stackptr = alloc_stack(); // allocating the stack
	if (stackptr == NULL) {return -1;} // checking if its null

	uint64_t* sp = stackptr + STACK_SIZE; // getting a pointer to the bottom of the stack.

	for (int i = 0; i <= 32; ++i) { //initializing every register in the stack to be zero
		--sp; // moving down the stack
		*sp = 0; // setting the value at which the stack pointer points to to zero
	}

	*(sp+30) = (uint64_t)code_address; // storing our process instruction address in our link register (x30)
	
	PCB_t* pcb = alloc_pcb(); // allocating our pcb
	if (pcb == NULL) {return -1;} // checking for any errors

	pcb->sp = sp; // pointer the stack pointer to the bottom of our allocated stack
	pcb->pid = nextPID++; // incrementing our pcb pid

	enqueue(&readyQueue, pcb); // putting our pcb on our queue

	return 0; // return 0
}

// Name: alloc_stack
// Arguments: 
// Purpose: we allocate the stack for a process. It pulls it from our global matrix of stacks (unsigned integers)
uint64_t* alloc_stack() {
	static int next_stack = 0; // static integer to count the stack for our next process
	if (readyQueue.size > MAX_PROCESSES) { // only reason why I could think this will return null. If we try to allocate too many processes
		return NULL;
	}
	return stacks[next_stack++]; //return them the pointer to a column in our global matrix of stacks
}

// Name: alloc_pcb
// Arguments: 
// Purpose: we allocate the process control block (pcb) for a process. It pulls it from our global array of pcbs.
PCB_t* alloc_pcb() {
	static int next_process = 0; // static integer to count the pcb for our next process
	if (readyQueue.size > MAX_PROCESSES) {
		return NULL;
	}
	return &pcbs[next_process++]; // return address of our array of real processes. Honestly I guess I could make that array a pointer, but I don't know if it matters.
}
//----------------------------------------------------------------------------------------------------------------
