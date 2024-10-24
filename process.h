#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include <stddef.h>

#define MAX_PROCESSES 4
#define STACK_SIZE 1024

//--Structure --------------------------------------------------------------------------------------
typedef struct PCB {
	uint64_t* sp;
	uint32_t pid;
	struct PCB *next;
} PCB_t;

int create_process(int (*code_address)());

uint64_t* alloc_stack();
PCB_t *alloc_pcb();

extern PCB_t *running; // Pointer to the currently running PCB
extern int nextPID;
extern uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
extern PCB_t pcbs[MAX_PROCESSES];


#endif