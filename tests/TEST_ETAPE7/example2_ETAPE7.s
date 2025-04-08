.text
    .global func1
func1:
    mov r0, #5
    bx lr


    .global func2
func2:
    mov r0, #10
    bx lr

.data
data_var:
    .word 0xDEADBEEF

.bss
bss_var:
    .space 8
