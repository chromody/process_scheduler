#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"

typedef struct PCB_Q {
	PCB_t *head;
	PCB_t *tail;
	int size;
}	PCB_Q_t;


void enqueue(PCB_Q_t *q, PCB_t *pcb);
PCB_t *dequeue(PCB_Q_t *q);

extern PCB_Q_t readyQueue;

#endif 