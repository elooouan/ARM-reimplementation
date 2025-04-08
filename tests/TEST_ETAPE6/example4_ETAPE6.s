.global _start
.global var1
.global var2

.section .text
_start:
    ldr r4, =var1
    ldr r5, =var2
    mov r7, #1
    svc #0

.section .data
var1: .word 0x87654321

.section .bss
var2: .skip 4
