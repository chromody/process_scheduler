
#include <stdint.h>
#include <stddef.h>
#include "queue.h"

/// We are adding to the back and taking from the front. It is still fifo, just backwards.
/*
	|---|		|---|		|---|		|---|
	|	| ---> 	|	| ---> 	|	| ---> 	|	|
	|---|		|---|		|---|		|---|
	HEAD								TAIL

	We add to the back

	|---|		|---|		|---|		|---|
	| 	| ---> 	|	| ---> 	|	| ---> 	|	|
	|---|		|---|		|---|		|---|
	HEAD					OLD			NEW
							TAIL		TAIL
	
	We take from the front

	|---|		|---|		|---|		|---|
	|	| X 	|	| ---> 	|	| --->  |	|
	|---|		|---|		|---|		|---|
	OLD			NEW						TAIL
	HEAD		HEAD
*/
void enqueue(PCB_Q_t *q, PCB_t *pcb) {
    if (q->size >= MAX_PROCESSES) {
        return; // Queue is full, do not enqueue. Maybe return a error response like 0
    }
    
    pcb->next = NULL; // Set the next pointer of the new PCB to NULL // maybe should be done on allocation

    if (q->tail) {
        // If the queue is not empty, link the new PCB at the end
        q->tail->next = pcb;
    } else {
        // If the queue is empty, set the head to the new PCB
        q->head = pcb;
    }

    // Update the tail to the new PCB
    q->tail = pcb;
    q->size++; // Increment the size of the queue
}

PCB_t *dequeue(PCB_Q_t *q) {
    if (q->size == 0) {
        return NULL; // Queue is empty, nothing to dequeue // maybe return error response
    }
    
    PCB_t *pcb = q->head; // Get the head PCB
    q->head = pcb->next; // Move the head pointer to the next PCB
    
    if (q->head == NULL) {
        // If the queue is now empty, update the tail as well
        q->tail = NULL;
    }

    q->size--; // Decrement the size of the queue
    pcb->next = NULL; // Clean up the next pointer
    return pcb; // Return the dequeued PCB
}
