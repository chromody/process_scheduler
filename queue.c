#include "queue.h"

// Enqueue a PCB to the ready queue
void enqueue(PCB_Q_t *q, PCB_t *pcb) {
    if (q->size >= MAX_PROCESSES) {
        return; // Queue is full
    }
    
    pcb->next = NULL; // Set the next pointer of the new PCB

    if (q->tail) {
        q->tail->next = pcb; // Link the new PCB at the end
    } else {
        q->head = pcb; // Set head if queue is empty
    }

    q->tail = pcb; // Update the tail
    q->size++; // Increment the size of the queue
}

// Dequeue a PCB from the ready queue
PCB_t *dequeue(PCB_Q_t *q) {
    if (q->size == 0) {
        return NULL; // Queue is empty
    }
    
    PCB_t *pcb = q->head; // Get the head PCB
    q->head = pcb->next; // Move the head pointer to the next PCB
    
    if (q->head == NULL) {
        q->tail = NULL; // Update tail if queue is now empty
    }

    q->size--; // Decrement the size of the queue
    pcb->next = NULL; // Clean up the next pointer
    return pcb; // Return the dequeued PCB
}

PCB_Q_t readyQueue;