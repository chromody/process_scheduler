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


//--Main 
int main() {
	//defining our boundaries
	unsigned int startRow = 21;
	unsigned int startColumn = 49;
	unsigned int endRow = 27;
	unsigned int endColumn = 79;
	clear_src(startRow, startColumn, endRow, endColumn); // clearing the section that we draw in

	box(startRow, startColumn, endRow, endColumn); // drawing the box
	
	print_to(24, 59, "Hello World!"); //printing hello world
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
