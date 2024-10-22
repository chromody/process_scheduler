.section .text
.global dispatch

dispatch:
    // Save the current process context
    sub sp, sp, #(16 * 16)  // Allocate space on the stack for registers
    stp x0, x1, [sp]       // Save x0 and x1
    stp x2, x3, [sp, #16]  // Save x2 and x3
    stp x4, x5, [sp, #32]  // Save x4 and x5
    stp x6, x7, [sp, #48]  // Save x6 and x7
    stp x8, x9, [sp, #64]  // Save x8 and x9
    stp x10, x11, [sp, #80]  // Save x10 and x11
    stp x12, x13, [sp, #96]  // Save x12 and x13
    stp x14, x15, [sp, #112]  // Save x14 and x15
    stp x16, x17, [sp, #128]  // Save x16 and x17
    stp x18, x19, [sp, #144]  // Save x18 and x19
    stp x20, x21, [sp, #160]  // Save x20 and x21
    stp x22, x23, [sp, #176]  // Save x22 and x23
    stp x24, x25, [sp, #192]  // Save x24 and x25
    stp x26, x27, [sp, #208]  // Save x26 and x27
    stp x28, x29, [sp, #224]  // Save x28 and x29
    mrs x0, SPSR_EL1      // Save the PSTATE
    str x0, [sp, #240]

    // Store the current SP in the PCB
    //ldr x1, =running
    //str sp, [x1]

    // Enqueue the current process
    //ldr x0, =ready_queue
    //bl enqueue

    // Dequeue the next process
    //ldr x0, =ready_queue
    //bl dequeue
    //mov x1, x0  // Move the dequeued PCB to x1

    // Restore the context of the next process
    //ldr x0, [x1]  // Load the SP from the PCB
    //mov sp, x0    // Restore the SP
    //ldr x0, [sp, #240]  // Restore the PSTATE
    //msr SPSR_EL1, x0

    // Restore the general-purpose registers
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

    add sp, sp, #(16 * 16)  // Deallocate the stack space
    ret  // Return to the next process