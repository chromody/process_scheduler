/*
	Author: Jesus Villanueva-Segovia
	Name: kernel.c
	Purpose: This will serve as the driver for our operating system? It just draws a box and hello string right now
*/
#include <stdint.h>
#include <stddef.h>
#include "queue.c"
#include "process.c"

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
