/*
	Author: Jesus Villanueva-Segovia
	Name: queue.c
	Purpose: This contains our queue functions and our ready queue
*/
#include "queue.h"

//--Functions ----------------------------------------------------------------------------------------------------
// Name: enqueue
// Arguments: queue of process control blocks
// Purpose: This will create a process. It will allocate and initialize its stack and pcb, then it will place that pcb on the ready queue.
void enqueue(PCB_Q_t *pcbQueue, PCB_t *pcb) {
    if (pcbQueue->size >= MAX_PROCESSES) {
        return; // Queue is full
    }
    
    pcb->next = NULL; // Set the next pointer of the new PCB

    if (pcbQueue->tail) {
        pcbQueue->tail->next = pcb; // Link the new PCB at the end
    } else {
        pcbQueue->head = pcb; // Set head if queue is empty
    }

    pcbQueue->tail = pcb; // Update the tail
    pcbQueue->size++; // Increment the size of the queue
}

// Dequeue a PCB from the ready queue
PCB_t *dequeue(PCB_Q_t *pcbQueue) {
    if (pcbQueue->size == 0) {
        return NULL; // Queue is empty
    }
    
    PCB_t *pcb = pcbQueue->head; // Get the head PCB
    pcbQueue->head = pcb->next; // Move the head pointer to the next PCB
    
    if (pcbQueue->head == NULL) {
        pcbQueue->tail = NULL; // Update tail if queue is now empty
    }

    pcbQueue->size--; // Decrement the size of the queue
    pcb->next = NULL; // Clean up the next pointer
    return pcb; // Return the dequeued PCB
}
//----------------------------------------------------------------------------------------------------------------