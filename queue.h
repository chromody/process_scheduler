#ifndef QUEUE_H_   /* Include guard */
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>

//--Preprocessor Definition ------------------------------------------------------------------------
#define MAX_PROCESSES 10
#define STACK_SIZE 1024
//--------------------------------------------------------------------------------------------------

//--Structure --------------------------------------------------------------------------------------
typedef struct PCB {
    uint64_t sp;       // Stack pointer
    uint32_t pid;      // Process ID
    struct PCB *next;  // Pointer to the next PCB in the linked list
} PCB_t;

typedef struct PCB_Q {
    PCB_t *head; // Pointer to the head of the queue (linked list)
    PCB_t *tail; // Pointer to the tail of the queue (linked list)
    int size;    // Current number of PCBs in the queue
} PCB_Q_t;

//--------------------------------------------------------------------------------------------------

//--Global Variables -------------------------------------------------------------------------------
int next_pid = 0; // Process ID counter
int current_pid = 0;
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
PCB_Q_t ready_queue; // The ready queue for processes
PCB_t running;
//--------------------------------------------------------------------------------------------------


#endif // QUEUE_H_
