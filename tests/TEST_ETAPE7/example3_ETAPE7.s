    .section .text
    .global main
main:
    mov r0, #0
    bx lr

    .section .data
undefined_symbol:
    .word invalid_reference

    .section .bss
    .global reserved_space
reserved_space:
    .space 4
