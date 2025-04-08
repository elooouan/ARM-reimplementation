    .section .text
    .global main
main:
    mov r0, #1
    bx lr

    .section .data
var1:
    .word 0x1234

    .section .bss
var2:
    .space 4
