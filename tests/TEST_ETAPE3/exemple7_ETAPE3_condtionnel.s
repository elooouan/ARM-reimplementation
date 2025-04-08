.global _start

.section .text
_start:
#ifdef DEBUG
    mov r0, #1
#else
    mov r0, #0
#endif
    mov r7, #1
    svc #0
