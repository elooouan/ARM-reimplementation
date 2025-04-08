.global _start

_start:
    ldr r0, =var      @ R_ARM_ABS32 test
    bl func           @ R_ARM_CALL test
    b .               @ Infinite loop

func:
    mov r1, #0x1      @ Test simple operation
    bx lr             @ Return

.data
var:  
    .word 0x12345678
