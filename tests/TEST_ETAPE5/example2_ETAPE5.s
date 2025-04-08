.global _start

_start:
    ldr r0, =aligned_data  @ Load the address of aligned_data into r0
    b .                     @ Infinite loop to halt execution

.data
    .align 4                @ Align the following data to 4-byte boundary
aligned_data:
    .word 0xAABBCCDD        @ A 32-bit value
