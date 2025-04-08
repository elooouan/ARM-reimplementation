.global _start
.global ext_func     @ Simulate external function

_start:
    bl ext_func       @ Test external function call
    b .

.data
ext_func:
    .word 0x0         @ Placeholder for dynamic linking
