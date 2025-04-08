.global main
.text
main:
    mov r0, #42
    swi 0x123456
.data
donnee:
    .word 0x11223344
