.global _start

_start:
    b test_func

test_func:
    mov r0, #7
    b .
