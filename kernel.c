/*
	Author: Jesus Villanueva-Segovia
	Name: kernel.c
	Purpose: This will serve as the driver for our operating system? It just draws a box and hello string right now
*/
#include <stdint.h>
#include <stddef.h>

//--Preprocessor Definition ------------------------------------------------------------------------
#define MAX_PROCESSES 4
#define STACK_SIZE 1024
//----------------------------------------------------------------------------------------

//--Structure --------------------------------------------------------------------------------------
typedef struct PCB {
	uint64_t* sp;
	uint32_t pid;
	struct PCB *next;
} PCB_t;

typedef struct PCB_Q {
	PCB_t *head;
	PCB_t *tail;
	int size;
}	PCB_Q_t;
//----------------------------------------------------------------------------------------

//--Global Variables -------------------------------------------------------------------------------
PCB_Q_t readyQueue;
int next_pid = 0;
PCB_t *running; // Pointer to the currently running PCB
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
PCB_t pcbs[MAX_PROCESSES];
//--------------------------------------------------------------------------------------------------

//--Prototypes
void putc_to(unsigned int row, unsigned int col, const char c); //Prototype to print a character, defined in libos.a
void print_to(unsigned int row, unsigned int col, const char[]); //Prototype to print some text, defined in libos.a
void clear_src(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to clear at a ordered pair, defined in libos.a
void box(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to draw a box, defined in box.S
int term_txtheight(); //Prototype to find the maximum height of the terminal, defined in libos.a
int term_txtwidth(); //Prototype to find the maximum width of the terminal, defined in libos.a


int create_process();
uint64_t* alloc_stack();
PCB_t *alloc_pcb();
void go();
void enqueue(PCB_Q_t *q, PCB_t *pcb);
PCB_t *dequeue(PCB_Q_t *q);
int create_process(int (*code_address)());
void dispatch();


int p1();
int p2();
int p3();
int p4();

//--Main 
int main() {
	int retval = 0;

	//defining our boundaries
	clear_src(0, 0, term_txtheight(), term_txtwidth()); // clearing the section that we draw in
	print_to(0, 0, "Printing Process!"); //printing hello world
	
	readyQueue.head = NULL;
	readyQueue.tail = NULL;
	readyQueue.size = 0;

	retval = create_process(p1);
	if (retval != 0) {return -1;}

	retval = create_process(p2);
	if (retval != 0) {return -1;}

	retval = create_process(p3);
	if (retval != 0) {return -1;}

	retval = create_process(p4);
	if (retval != 0) {return -1;}

	go();

	return 0;
}

// Name: clear_src
// Arguments: unsigned int srow, unsigned int scol, unsigned int erow, unsigned int ecol
// Purpose: This will clear a defined section of the terminal
void clear_src(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn) {

	const char c = ' ';
	for(unsigned int x = startRow; x < endRow; ++x) {
		for(unsigned int y = startColumn; y < endColumn; ++y) {
			putc_to(x, y, c);
		}
	}
}

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

int create_process(int (*code_address)()) {
	uint64_t* stackptr = alloc_stack();
	if (stackptr == NULL) {return -1;}

	uint64_t* sp = stackptr + STACK_SIZE;

	for (int i = 0; i <= 32; ++i) {
		--sp;
		stackptr[*sp] = 0;
	}

	stackptr[(*sp)+30] = (uint64_t)code_address;
	
	PCB_t* pcb = alloc_pcb();
	if (pcb == NULL) {return -1;}

	pcb->sp = sp;
	pcb->pid = next_pid;

	enqueue(&readyQueue, pcb);

	return 0;
}

uint64_t* alloc_stack() {
	static int next_stack = 0;
	if (readyQueue.size > MAX_PROCESSES) {
		return NULL;
	}
	return stacks[next_stack++];
}

PCB_t* alloc_pcb() {
	static int next_process = 0;
	if (readyQueue.size > MAX_PROCESSES) {
		return NULL;
	}
	return &pcbs[next_process++];
}

void go() {

}

int p1() {
	unsigned int startRow = 9;
	unsigned int startColumn = 23;
	unsigned int endRow = 11;
	unsigned int endColumn = 39;
	char message[] = "Process 1: 0";
	int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(startRow + 1, startColumn + 2, message); 

	while (1==0) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
	dispatch();
	//call dispatch
}

int p2() {
	unsigned int startRow = 13;
	unsigned int startColumn = 23;
	unsigned int endRow = 15;
	unsigned int endColumn = 39;
	char message[] = "Process 2: 0";
	int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(startRow + 1, startColumn + 2, message); 

	while (1==0) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
	dispatch();
	//call dispatch
}

int p3() {
	unsigned int startRow = 9;
	unsigned int startColumn = 49;
	unsigned int endRow = 11;
	unsigned int endColumn = 65;
	char message[] = "Process 3: 0";
	int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(startRow + 1, startColumn + 2, message); 

	while (1==0) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
	dispatch();
	//call dispatch
}

int p4() {
	unsigned int startRow = 13;
	unsigned int startColumn = 49;
	unsigned int endRow = 15;
	unsigned int endColumn = 65;
	char message[] = "Process 4: 0";
	int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(startRow + 1, startColumn + 2, message); 

	while (1==0) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
	dispatch();
	//call dispatch
}