.global _start

_start:
    ldr r0, =large_value
    b .

.data
large_value:  
    .word 0xFFFFFFFF  @ Maximum 32-bit unsigned value
