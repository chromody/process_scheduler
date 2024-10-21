/*
	Author: Jesus Villanueva-Segovia
	Name: kernel.c
	Purpose: This will serve as the driver for our operating system? It just draws a box and hello string right now
*/
#include <stdint.h>

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
PCB_Q_t ready_queue; // The ready queue for processes
PCB_t pcbs[MAX_PROCESSES]; // Fixed-size array for PCBs
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

void p1();
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
	//defining our boundaries
	unsigned int startRow = 21;
	unsigned int startColumn = 49;
	unsigned int endRow = 27;
	unsigned int endColumn = 79;
	clear_src(startRow, startColumn, endRow, endColumn); // clearing the section that we draw in

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(24, 59, "Running Processes"); //printing hello world

	ready_queue.head = NULL; 
    ready_queue.tail = NULL; 
    ready_queue.size = 0;   

    if (create_process(p1) != 0) return -1;

	return 0;
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

void enqueue(PCB_Q_t *q, PCB_t *pcb) {
    if (q->size >= MAX_PROCESSES) {
        return; // Queue is full, do not enqueue
    }
    
    pcb->next = NULL; // Set the next pointer of the new PCB to NULL

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
        return NULL; // Queue is empty, nothing to dequeue
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
    uint64_t *stackptr = (uint64_t *)alloc_stack();
    if (stackptr == NULL) {
        return -1; // Error: stack allocation failed
    }

    uint64_t *sp = stackptr + 1024; // Set sp to the top of the stack

    for (int i = 0; i <= 32; i++) {
        sp--; // Move sp down the stack
        *sp = 0; // Initialize register values to 0
    }

    sp[30] = (uint64_t)code_address; // Set the code address

    PCB_t *pcb = alloc_pcb();
    if (pcb == NULL) {
        return -1; // Error: PCB allocation failed
    }

    pcb->sp = (uint64_t)sp; // Set the stack pointer in the PCB
    pcb->pid = next_pid++; // Assign a unique process ID

    enqueue(&ready_queue, pcb); // Enqueue the PCB onto the ready queue

    return 0; // No errors occurred
}

uint64_t* alloc_stack() {
    static int stack_index = 0; // Track the next stack to use
    if (stack_index < MAX_PROCESSES) {
        return stacks[stack_index++]; // Return the next available stack
    }
    return NULL; // No more stacks available
}

PCB_t* alloc_pcb() {
    static int pcb_index = 0; // Track the next PCB to use
    if (pcb_index < MAX_PROCESSES) {
        return &pcbs[pcb_index++]; // Return the next available PCB
    }
    return NULL; // No more PCBs available
}

void p1() {
    char message[] = "Process 1: 0";
    int num = 0;

    draw_box(9, 23, 11, 39); // Draw a box for the output

    while (1) {
        message[11] = num + '0'; // Update the number
        print(message, 10, 25);  // Print at row 10, column 25
        num++;
        if (num > 9) num = 0;    // Reset after 9
        dispatch();               // Switch to another process
    }
}
//--------------------------------------------------------------------------------------------------
