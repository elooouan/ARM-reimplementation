.global _start

_start:
    ldr r0, =var1
    ldr r1, =var2
    b .

.data
var1:
    .word 0x11111111

var2:
    .word 0x22222222
