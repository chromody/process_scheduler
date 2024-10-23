.section .text
.global dispatch

// Function to start the first process
go:
    b restore_context

dispatch:

restore_context:
    ret  // Return to the next process
