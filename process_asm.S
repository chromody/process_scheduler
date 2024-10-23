.section .text
.global dispatch
.global go

// Function to start the first process
go:
    b restore_context

dispatch:
    /*
    dispatch:
    1)	subtract 16*16 from the stack
    2)	copy all registers to the stack the stack
			(in the order x0,x1,...,x30)
    3)	put the PSTATE into the stack with the
			following code:
			   mrs     x0, SPSR_EL1      // Save PSTATE
			   str     x0, [sp, 16*15+8]
	4)	store the new sp value into the pcb where Running points
			(note that you cannot move sp into memory directly,
             you have to do something like:
             adr     x1, Running
             ldr     x1, [x1]  // put the first value in the PCB (sp) into x1
    		 mov     x4, sp    // We can't str sp directly, so put sp into x4...
    		 str     x4, [x1]  //    Then store the new sp in x4 to the PCB 
	5)	call enqueue passing it address of ReadyQ and the value of
			Running
     */

restore_context:
    /*
    restore_context:   // Note that this is where go() jumps
    6)	call dequeue passing it the address of ReadyQ
    7)  move the return value of dequeue into Running
    8)  read the value out of the returned pcb and put
        	it into the sp register. The code should look
            like the following:
            	adr		x1, Running
                str		x0, [x1]   // store the pointer to the PCB into the Running variable
    			ldr		x2, [x0]   // x2 now has the value of the stack ptr (we cant ldr sp directly)
    			mov		sp, x2     // sp now has the top of stack for the chosen process
    9)	restore the PSTATE stored in the stack
            ldr     x0, [sp, 16*15+8]   // Restore PSTATE
    		msr     SPSR_EL1, x0
    10) copy all register values for x0-x30 from the stack at
            which sp points into those registers.
    11) add 16*16 to sp
    12) return
    */
    ret  // Return to the next process
