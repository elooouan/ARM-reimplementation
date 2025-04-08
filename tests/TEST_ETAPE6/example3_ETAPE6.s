.global _start
.extern external_func

.section .text
_start:
    bl external_func  @ Appel de la fonction externe
    mov r7, #1
    svc #0
