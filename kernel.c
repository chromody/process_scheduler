/*
	Author: Jesus Villanueva-Segovia
	Name: kernel.c
	Purpose: This will serve as the driver for our operating system? It just draws a box and hello string right now
*/


//--Prototypes
void putc_to(unsigned int row, unsigned int col, const char c); //Prototype to print a character, defined in libos.a
void print_to(unsigned int row, unsigned int col, const char[]); //Prototype to print some text, defined in libos.a
void clear_src(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to clear at a ordered pair, defined in libos.a
void box(unsigned int startRow, unsigned int startColumn, unsigned int endRow, unsigned int endColumn); //Prototype to draw a box, defined in box.S
int term_txtheight(); //Prototype to find the maximum height of the terminal, defined in libos.a
int term_txtwidth(); //Prototype to find the maximum width of the terminal, defined in libos.a


int p1();
int p2();
int p3();
int p4();

//--Main 
int main() {
	//defining our boundaries
	clear_src(0, 0, term_txtheight(), term_txtwidth()); // clearing the section that we draw in
	print_to(0, 0, "Printing Process!"); //printing hello world
	

	p3();
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


int p1() {
	unsigned int startRow = 9;
	unsigned int startColumn = 23;
	unsigned int endRow = 11;
	unsigned int endColumn = 39;
	char message[] = "Process 1: 0";
	int num = 0;

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(startRow + 1, startColumn + 2, message); 

	while (1==1) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
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

	while (1==1) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
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

	while (1==1) {
		message[11] = '0' + ++num;
		print_to(startRow + 1, startColumn + 2, message); //printing hello world;
		if (num > 9) {
			num = 0; 
		}
	}
	//call dispatch
}