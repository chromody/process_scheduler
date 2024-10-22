/*
	Author: Jesus Villanueva-Segovia
	Name: kernel.c
	Purpose: This will serve as the driver for our operating system? It just draws a box and hello string right now
*/
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
PCB_Q_t ready_queue; // The ready queue for processes
PCB_t running;
uint64_t stacks[MAX_PROCESSES][STACK_SIZE]; // Fixed-size array for stacks
//--------------------------------------------------------------------------------------------------


//--Prototypes -------------------------------------------------------------------------------------
void putc_to(unsigned int row, unsigned int col, const char c); //Prototype to print a character, defined in libos.a
void print_to(unsigned int row, unsigned int col, const char[]); //Prototype to print some text, defined in libos.a

// * Related to the box function
void clear_src(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to clear at a ordered pair, defined in libos.a
void box(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to draw a box, defined in box.S
int term_txtheight(); //Prototype to find the maximum height of the terminal, defined in libos.a
int term_txtwidth(); //Prototype to find the maximum width of the terminal, defined in libos.a

// * Related to process function
void enqueue(PCB_Q_t *q, PCB_t *pcb);
PCB_t *dequeue(PCB_Q_t *q);
int create_process(int (*code_address)());
uint64_t* alloc_stack();
PCB_t* alloc_pcb();
void go();
void dispatch();

int p1();
int p2();
int p3();
int p4();

//--------------------------------------------------------------------------------------------------


//--Main -------------------------------------------------------------------------------------------
int main() {
	/*
	int main() {
    		clear the screen
    		print "Running processes" on the first line of the screen

		→   initialize Ready queue data structure      
		→   set retval to the return value of create_process(p1)
		→   check if error (retval != 0)
	    	// repeat the above for process p2, p3, and p4

	    	// Now begin running the first process ...
		→   go()
	    	// go never returns to main
	}
	*/
	clear_src(0, 0, term_txtheight(), term_txtwidth()); // clearing the monitor
	print_to(0, 0, "Running Processes"); //printing hello world

	//box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	ready_queue.head = NULL; 
    ready_queue.tail = NULL; 
    ready_queue.size = 0;   
	int retval = 0;
	
	retval = create_process(*p1);
    if (retval != 0) return -1;

	retval = create_process(*p2);
    if (retval != 0) return -1;

	retval = create_process(*p3);
    if (retval != 0) return -1;

	retval = create_process(*p4);
    if (retval != 0) return -1;

	go();
}
//--------------------------------------------------------------------------------------------------


//--Functions --------------------------------------------------------------------------------------

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

int create_process(int (*code_address)()) {
    PCB_t *pcb = NULL; // Get the next PCB
    uint64_t *stackptr = alloc_stack(); // Allocate a stack
    if (stackptr == NULL) {
        return -1; // Error: stack allocation failed
    }

    uint64_t *sp = stackptr + STACK_SIZE; // Set sp to the top of the stack

    for (int i = 0; i <= 32; i++) {
        sp--; 
        *sp = 0; // Initialize registers to 0
    }

    sp[30] = (uint64_t)code_address; // Set the code address

	pcb = alloc_pcb((uint64_t)sp, code_address, NULL);
	if (pcb == NULL) {
		return -1;
	}
	pcb->sp = sp;
	pcb->pid = next_pid;

    enqueue(&ready_queue, pcb); // Enqueue the PCB

    return 0; // No errors
}

uint64_t* alloc_stack() {
	static int next_stack = 0; // Track the next stack to use
    if (next_stack >= MAX_PROCESSES) {
	    return NULL; // No more stacks available
	}
	return stacks[next_stack++]; // Return the next available stack
}

PCB_t* alloc_pcb(uint64_t sp, uint32_t pid, PCB_t *next) {
	static int pcb_count = 0; // Track the next stack to use
	if (pcb_count >= MAX_PROCESSES) {
		return NULL;
	}
	PCB_t* newPCB;
	++pcb_count;

	(*newPCB).sp = sp;
	(*newPCB).pid = pid;
	(*newPCB).next = next;

	return newPCB;
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
	print_to(10, 25, message); //printing process 1 : num


    while (1) {
        message[11] = num + '0'; // Update the number
		print_to(10, 25, message); //printing process 1 : num
        num++;
        if (num > 9) num = 0;    // Reset after 9
    }
}

int p2() {
	unsigned int startRow = 13;
	unsigned int startColumn = 23;
	unsigned int endRow = 15;
	unsigned int endColumn = 39;

    char message[] = "Process 2: 0";
    int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	print_to(14, 25, message); //printing process 1 : num

    while (1) {
        message[11] = num + '0'; // Update the number
		print_to(24, 59, message); //printing process 1 : num
        num++;
        if (num > 9) num = 0;    // Reset after 9
    }
}

int p3() {
	unsigned int startRow = 9;
	unsigned int startColumn = 49;
	unsigned int endRow = 11;
	unsigned int endColumn = 39;

    char message[] = "Process 3: 0";
    int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	print_to(10, 50, message); //printing process 1 : num

    while (1) {
        message[11] = num + '0'; // Update the number
		print_to(10, 25, message); //printing process 1 : num
        num++;
        if (num > 9) num = 0;    // Reset after 9
    }
}

int p4() {
	unsigned int startRow = 13;
	unsigned int startColumn = 49;
	unsigned int endRow = 15;
	unsigned int endColumn = 39;

    char message[] = "Process 4: 0";
    int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	print_to(14, 50, message); //printing process 1 : num

    while (1) {
        message[11] = num + '0'; // Update the number
		print_to(14, 50, message); //printing process 1 : num
        num++;
        if (num > 9) num = 0;    // Reset after 9
    }
}
//--------------------------------------------------------------------------------------------------
