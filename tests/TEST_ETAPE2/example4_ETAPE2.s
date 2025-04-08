.section .text
.align 4
.global text_section
text_section:
    mov r0, #1

.section .data
.align 8
.global data_section
data_section:
    .word 0xCAFEBABE
