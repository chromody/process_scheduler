/*
Author: Jesus
box.S
Purpose: To draw a box based on user inputs (w0, w1, w2, w3)
CSC-4100, 001
*/

//The box() function must be written in ARMv8 assembler.  The code below is given to get you started:
.section .data //user function for data. We will print + for our box
char_block: .byte '+'

.extern     putc_to //arguments are: unsigned int row, unsigned int col, const char c
.extern     _term_txtheight //function to find height of terminal
.extern     _term_txtwidth //function to find width of terminal

.section .text
.global     box

/*
	Name: box
	Arguments: w0(srow), w1(scol), w2(erow), w3(ecol)
	Purpose: Takes in the row and column boundaries and prints a box in those boundaries
*/
box: 
 //------------------------------------------------------------------Prologue
    // standard entry: save link and stack frame registers
    //    load the stack frame with the pointer to current frame
    stp     x29, x30, [sp, -16]!

    // Save the arguments.  Called functions may overwrite x0-x3
    sub     sp, sp, 32
    stp     x0, x1, [sp]
    stp     x2, x3, [sp, 16]

    // sr in w0, sc in w1, er in w2, ec in w3
    
//--------------------------------------------------------------------

    // Make sure the argument are valid
    //passing variable onto function
    sub sp, sp, 32 //startRow, startColumn, endRow, endColumn need to be saved on stack
    stp w0, w1, [sp]
    stp w2, w3, [sp, 16]
    bl      _box_check_box_

    cmp     w0, 0
    bne     _box_error_

    ldp w0, w1, [sp] //restoring stack and registers
    ldp w2, w3, [sp, 16]
    add sp, sp, 32

 
    // first, draw the corners:
    // +        +
    // +        +
    sub sp, sp, 32//passing values on stack
    ldp     w0, w1, [sp]
    ldp     w2, w3, [sp, 16]
    bl      _box_draw_corners_
    add sp, sp, 32//restoring stack

    // Now, draw the top and bottom:
    // +------+
    // +------+
    sub sp, sp, 32//passing values
    ldp w0, w1, [sp]
    ldp w2, w3, [sp, 16]
    bl _box_draw_top_bottom_
    add sp, sp, 32//restoring stack
    
    // You figure out what goes here
    //?? not sure it works though
    
    // draw left and right sides
    sub sp, sp, 32//passing values
    ldp w0, w1, [sp]
    ldp w2, w3, [sp, 16]
    bl _box_draw_left_right_
    add sp, sp, 32//restoring stack

    // You figure out what goes here

    mov     w0, 0//returning 0 if no errors
    b       _box_exit_

_box_error_:
    mov     w0, 1//returning 1 if errors

_box_exit_:
    add     sp, sp, 32   // pop save arg regs

    ldp     x29, x30, [sp], 16
    ret
    
/*
    Inputs: startRow, startColumn, endRow, endColumn
    Purpose: to check if the bounds make logical sense
    Functions called: int term_txtheight(), int term_txtwidth()
    Total registers: sp, x0, x30(link register), w0 (startRow), w1(startColumn), w2(endRow), w3(endColumn), x9(term_txtheight), x10(term_txtwidth)
*/
 _box_check_box_:
    //Prologue
    //    save link and stack frame registers
    //    load the stack frame with the pointer to current frame
    stp     x29, x30, [sp, -16]!
    ldp w0, w1, [sp, 16]
    ldp w2, w3, [sp, 32]


    //----------------------Getting max height
    sub sp, sp, 16
    stp x30, x0, [sp]
    bl term_txtheight // calling function for max height

    sub sp, sp, 16 //making space on the stack
    str x0, [sp] // to pass the result from x0 to x9
    ldr x9, [sp] // mov instruction wouldn't pass it ??
    add sp, sp, 16

    ldp x30, x0, [sp]
    add sp, sp, 16
    //----------------------

    //----------------------Getting max height
    sub sp, sp, 16
    stp x30, x0, [sp]
    bl term_txtwidth // calling function for max height\

    sub sp, sp, 16 //again passing the result to x10 via the stack
    str x0, [sp]
    ldr x10, [sp]
    add sp, sp, 16

    ldp x30, x0, [sp]
    add sp, sp, 16
    //----------------------

    //----------------------Checking for error
    mov w0, 0
    cmp x2, x9 //comparing endRow max_width
    bge _box_check_box_error

    cmp x3, x10 //comparing endColumn max_height
    bge _box_check_box_error

    cmp x2, x0 //checkiun
    blt _box_check_box_error

    cmp x3, x1
    blt _box_check_box_error
    //----------------------

    //Epilogue
    ldp     x29, x30, [sp], 16
    ret
    //

    _box_check_box_error:
        add x0, x0, 1
        ldp     x29, x30, [sp], 16
        ret


/*
    Inputs: startRow, startColumn, endRow, endColumn
    Purpose: to print the corners of the box
    Functions called: putc_to()
    Total registers: sp, x0, x30(link register), w0 (startRow), w1(startColumn), w2(endRow), w3(endColumn)
*/
 _box_draw_corners_:

    //Prologue
    //    save link and stack frame registers
    //    load the stack frame with the pointer to current frame
    stp x29, x30, [sp, -16]!
    ldp w0, w1, [sp, 16]
    ldp w2, w3, [sp, 32]

    //--printing the top left corner (sr, sc)
    sub sp, sp, 32 //saving w0 -> w3 registers as the putc_to function edits these registers
    stp w0, w1, [sp] // I guess I should save x0->x8 as these can be edited, but that would be like 80 bits on the stack thats crazy
    stp w2, w3, [sp, 16] //maybe I'm doing this wrong if I am please tell me

    adr x10, char_block //getting the char +
    ldrb w2, [x10] //loading that into w2 to pass to putc_to

    sub sp, sp, 32 //preparing to call putc_to
    stp w0, w1, [sp] //w0 is start_row, w1 is start_column, w2 is the character we are printing
    str w2, [sp, 16]
    bl putc_to
    ldp w0, w1, [sp]
    ldr w2, [sp, 16]
    add sp, sp, 32

    ldp w0, w1, [sp] //restoring w0 -> w3
    ldp w2, w3, [sp, 16]
    add sp, sp, 32
    //---------

    //--printing the top right corner (er, sc)
    sub sp, sp, 32 //saving w0 -> w3
    stp w0, w1, [sp]
    stp w2, w3, [sp, 16]

    mov w0, w2 //we want to pass w2 as the end_row

    adr x10, char_block
    ldrb w2, [x10]

    sub sp, sp, 32
    stp w0, w1, [sp]
    str w2, [sp, 16] //w0 is end row, w1 is start_column, w2 is the character we are printing
    bl putc_to
    ldp w0, w1, [sp]
    ldr w2, [sp, 16]
    add sp, sp, 32

    ldp w0, w1, [sp] //restoring w0 -> w3
    ldp w2, w3, [sp, 16]
    add sp, sp, 32
    //

    //--printing the bottom left corner (sr, ec)
    sub sp, sp, 32 //saving w0 -> w3
    stp w0, w1, [sp]
    stp w2, w3, [sp, 16]

    mov w1, w3

    adr x10, char_block
    ldrb w2, [x10]

    sub sp, sp, 32
    stp w0, w1, [sp]
    str w2, [sp, 16] //w0 is start_row, w1 is end_column, w2 is the character we are printing
    bl putc_to
    ldp w0, w1, [sp]
    ldr w2, [sp, 16]
    add sp, sp, 32

    ldp w0, w1, [sp] //restoring w0 -> w3
    ldp w2, w3, [sp, 16]
    add sp, sp, 32
    //

    //--printing the bottom left corner (er, ec)
    sub sp, sp, 32 //saving w0 -> w3
    stp w0, w1, [sp]
    stp w2, w3, [sp, 16]

    mov w0, w2
    mov w1, w3

    adr x10, char_block
    ldrb w2, [x10]

    sub sp, sp, 32
    stp w0, w1, [sp]
    str w2, [sp, 16] //w0 is end_row, w1 is end_colunmn, w2 is the character we are printing
    bl putc_to
    ldp w0, w1, [sp]
    ldr w2, [sp, 16]
    add sp, sp, 32

    ldp w0, w1, [sp] //restoring w0 -> w3
    ldp w2, w3, [sp, 16]
    add sp, sp, 32
    //

    //Epilogue
    ldp x29, x30, [sp], 16
    ret


/*
    Inputs: startRow, startColumn, endRow, endColumn
    Purpose: to print the top and bottom of the box
    Functions called: putc_to
    Total registers: sp, x0, x30(link register), w0 (startRow), w1(startColumn), w2(endRow), w3(endColumn)
*/
_box_draw_top_bottom_:
    //PROLOGUE
    stp x29, x30, [sp, -16]!

    mov w9, w0 //we are by start row
    //add w9, 1 // we start ahead of sr
    _box_draw_top_bottom_loop: // i = sr + i; i < er; ++i
        //--printing top
        sub sp, sp, 32 //saving w0 -> w3
        stp w0, w1, [sp]
        stp w2, w3, [sp, 16]

        mov w0, w9
        adr x10, char_block
        ldrb w2, [x10]

        sub sp, sp, 32
        stp w0, w1, [sp] //w0 is our row, w1 is start_column, w2 is the character we are printing (sr + i, sc)
        str w2, [sp, 16]
        bl putc_to
        ldp w0, w1, [sp]
        ldr w2, [sp, 16]
        add sp, sp, 32

        ldp w0, w1, [sp] //restoring w0 -> w3
        ldp w2, w3, [sp, 16]
        add sp, sp, 32
        //---------

        //--printing bottom
        sub sp, sp, 32 //saving w0 -> w3
        stp w0, w1, [sp]
        stp w2, w3, [sp, 16]

        mov w0, w9
        mov w1, w3
        adr x10, char_block
        ldrb w2, [x10]

        sub sp, sp, 32
        stp w0, w1, [sp] //w0 is our row, w1 is end_row, w2 is the character we are printing (sr + i, ec)
        str w2, [sp, 16]
        bl putc_to
        ldp w0, w1, [sp]
        ldr w2, [sp, 16]
        add sp, sp, 32

        ldp w0, w1, [sp] //restoring w0 -> w3
        ldp w2, w3, [sp, 16]
        add sp, sp, 32
        //---------


        add w9, w9, 1
        cmp w9, w2
        blt _box_draw_top_bottom_loop

    //EPILOGUE
    ldp x29, x30, [sp], 16
    ret

/*
    Inputs: startRow, startColumn, endRow, endColumn
    Purpose: to print the left and right portion of the box
    Functions called: putc_to()
    Total registers: sp, x0, x30(link register), w0 (startRow), w1(startColumn), w2(endRow), w3(endColumn)
*/
_box_draw_left_right_:
    //PROLOGUE
    stp x29, x30, [sp, -16]!

    mov w9, w1 // we start at start column
    _box_draw_left_right_loop: // i = sc + i; i < ec; ++i
        //--printing left
        sub sp, sp, 32 //saving w0 -> w3
        stp w0, w1, [sp]
        stp w2, w3, [sp, 16]

        mov w1, w9
        adr x10, char_block
        ldrb w2, [x10]

        sub sp, sp, 32
        stp w0, w1, [sp] //w0 is start_row, w1 is our row, w2 is the character we are printing (sr, sc + i)
        str w2, [sp, 16]
        bl putc_to
        ldp w0, w1, [sp]
        ldr w2, [sp, 16]
        add sp, sp, 32

        ldp w0, w1, [sp] //restoring w0 -> w3
        ldp w2, w3, [sp, 16]
        add sp, sp, 32
        //---------

        //--printing right
        sub sp, sp, 32 //saving w0 -> w3
        stp w0, w1, [sp]
        stp w2, w3, [sp, 16]

        mov w0, w2
        mov w1, w9
        adr x10, char_block
        ldrb w2, [x10]

        sub sp, sp, 32
        stp w0, w1, [sp] //w0 is end_row, w1 is our column, w2 is the character we are printing (er, sc + i)
        str w2, [sp, 16]
        bl putc_to
        ldp w0, w1, [sp]
        ldr w2, [sp, 16]
        add sp, sp, 32

        ldp w0, w1, [sp] //restoring w0 -> w3
        ldp w2, w3, [sp, 16]
        add sp, sp, 32
        //---------


        add w9, w9, 1
        cmp w9, w3 // we end at end column
        blt _box_draw_left_right_loop

    //EPILOGUE
    ldp x29, x30, [sp], 16
    ret

