/*
	Author: Jesus Villanueva-Segovia
	Name: queue.c
	Purpose: This contains our queue functions and our ready queue
*/
#include "queue.h"

//--Functions ----------------------------------------------------------------------------------------------------
// Name: enqueue
// Arguments: pointer to queue of process control blocks, and pointer to a process control block
// Purpose: This will add a process's pcb to a queue of pcbs, which will be ready queue. It adds to the end of the queue
void enqueue(PCB_Q_t *pcbQueue, PCB_t *pcb) {
    if (pcbQueue->size >= MAX_PROCESSES) { //check if our queue size is at its limit. But linked lists don't have a defined limit of size
        return; // Queue is full
    }
    
    pcb->next = NULL; //set the pcb's next pointer to null

    if (pcbQueue->tail) { //if the queue is not empty
        pcbQueue->tail->next = pcb; // Add the pcb to after the tail
    } else { //if the queue is empty
        pcbQueue->head = pcb; // set head to the pcb
    }

    pcbQueue->tail = pcb; // Update the pcb's tail
    pcbQueue->size++; // Incrementing the size of the pcb
}

// Name: dequeue
// Arguments: pointer to queue of process control blocks
// Purpose: This will remove a pcb from the head of the queue
PCB_t* dequeue(PCB_Q_t *pcbQueue) {
    if (pcbQueue->size == 0) { // queue is empty so we return null
        return NULL; 
    }
    
    PCB_t *pcb = pcbQueue->head; // getting head of queue
    pcbQueue->head = pcb->next; // moving the head up one pcb
    
    if (pcbQueue->head == NULL) { // if the heade is now null, then it was the last pcb
        pcbQueue->tail = NULL;
    }

    pcbQueue->size--; // we removed a pcb so we are decrementing it
    pcb->next = NULL; // no dangling pointers
    return pcb; // returning the pcb now
}
//----------------------------------------------------------------------------------------------------------------