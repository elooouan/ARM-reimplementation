.global _start

_start:
    b long_jump       @ Test jump to long_jump

long_jump:
    mov r0, #42       @ Just a test operation
    b .               @ Infinite loop
