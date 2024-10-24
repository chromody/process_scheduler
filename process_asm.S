/*
	Author: Jesus Villanueva-Segovia
	Name: process_asm.S
	Purpose: This will house our process scheduling functions. 
*/

.section .text

//--Global Functions/Labels ------------------------------------------------------------------------
.global dispatch, go
//--------------------------------------------------------------------------------------------------

//--Global Variables -------------------------------------------------------------------------------
.global readyQueue, nextPid, running, stacks, pcbs
//--------------------------------------------------------------------------------------------------



//--Functions/Labels -------------------------------------------------------------------------------

// Name: go
// Arguments: 
// Purpose: This will branch to restore context to start our task scheduling
go:
    b restore_context

// Name: dispatch
// Arguments: 
// Purpose: This will dispatch a process. So it will save the context of the currently running process. Then it will place that on our global ready queue, and then it will fall through into our restore_context function/label
dispatch: //basically it ends the process
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
    sub sp, sp, #256
    stp x0, x1, [sp]             // Restore x0 and x1
    stp x2, x3, [sp, #16]        // Restore x2 and x3
    stp x4, x5, [sp, #32]        // Restore x4 and x5
    stp x6, x7, [sp, #48]        // Restore x6 and x7
    stp x8, x9, [sp, #64]        // Restore x8 and x9
    stp x10, x11, [sp, #80]      // Restore x10 and x11
    stp x12, x13, [sp, #96]      // Restore x12 and x13
    stp x14, x15, [sp, #112]     // Restore x14 and x15
    stp x16, x17, [sp, #128]     // Restore x16 and x17
    stp x18, x19, [sp, #144]     // Restore x18 and x19
    stp x20, x21, [sp, #160]     // Restore x20 and x21
    stp x22, x23, [sp, #176]     // Restore x22 and x23
    stp x24, x25, [sp, #192]     // Restore x24 and x25
    stp x26, x27, [sp, #208]     // Restore x26 and x27
    stp x28, x29, [sp, #224]     // Restore x28 and x29
    str x30, [sp, #240]

    mrs     x0, SPSR_EL1      // Save PSTATE
	str     x0, [sp, 16*15+8]

    adr     x1, running
    ldr     x1, [x1]  // put the first value in the PCB (sp) into x1
    mov     x4, sp    // We can't str sp directly, so put sp into x4...
    str     x4, [x1]  //    Then store the new sp in x4 to the PCB 

    ldr x0, =readyQueue          // Load the value 42 into register x0 (parameter for my_function)
    ldr x2, =running
    ldr x1, [x2]

    sub sp, sp, 16
    str x30, [sp]
    bl enqueue
    ldr x30, [sp]
    add sp, sp, 16

// Name: restore_context
// Arguments: 
// Purpose: This will restore the context of the process that is at the head of the ready queue.
restore_context: // Basically it starts the next process.
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
    ldr x0, =readyQueue // Loading in the address of readyQueue into x0, as we are going to pass it as a argument to dequeue
    
    sub sp, sp, 16 // Saving the stack. I don't know if this is required, but I will keep it here for safety
    str x30, [sp]
    bl dequeue       // branching into our process
    ldr x30, [sp]
    add sp, sp, 16
    
    ldr x1, =running      // loading the address of the result variable
    str x0, [x1]         // storing the return value into result

    adr x1, running //loading the address of running into x1
    str x0, [x1] // storing that into x0. I believe do this right before but I just decided to copy and paste this part
    ldr x2, [x0] // loading the value of our stack pointer into x2
    mov sp, x2 // moving that into our actual stack pointer

    ldr x0, [sp, 16*15+8] //saving PSTATE on the stack
    msr SPSR_EL1, x0

    // Restore registers x0-x30 from the stack
    ldp x0, x1, [sp]             
    ldp x2, x3, [sp, #16]        
    ldp x4, x5, [sp, #32]        
    ldp x6, x7, [sp, #48]        
    ldp x8, x9, [sp, #64]       
    ldp x10, x11, [sp, #80]      
    ldp x12, x13, [sp, #96]      
    ldp x14, x15, [sp, #112]     
    ldp x16, x17, [sp, #128]     
    ldp x18, x19, [sp, #144]     
    ldp x20, x21, [sp, #160]     
    ldp x22, x23, [sp, #176]    
    ldp x24, x25, [sp, #192]  
    ldp x26, x27, [sp, #208]     
    ldp x28, x29, [sp, #224]     
    ldr x30, [sp, #240]

    add sp, sp, #256 // adding 16 * 16 to the stack

    ret  // Return to the next process
