/*
	Author: Jesus Villanueva-Segovia
	Name: process.h
	Purpose: This serves as a header file for the process c file.
*/

#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include <stddef.h>

//--Preprocessor Definition --------------------------------------------------------------------------------------
#define MAX_PROCESSES 4
#define STACK_SIZE 1024
//----------------------------------------------------------------------------------------------------------------

//--Structure ----------------------------------------------------------------------------------------------------
typedef struct PCB {
	uint64_t* sp;
	uint32_t pid;
	struct PCB *next;
} PCB_t;
//----------------------------------------------------------------------------------------------------------------

//--Variables ----------------------------------------------------------------------------------------------------
extern PCB_t *running; // Pointer to the currently running PCB
extern int nextPID;
extern uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
extern PCB_t pcbs[MAX_PROCESSES];
//----------------------------------------------------------------------------------------------------------------

//--Functions ----------------------------------------------------------------------------------------------------
int create_process(int (*code_address)());
uint64_t* alloc_stack();
PCB_t *alloc_pcb();
//----------------------------------------------------------------------------------------------------------------

#endif