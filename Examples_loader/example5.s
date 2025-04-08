.global _start
.global loop
.global end

.text
_start:
    mov r0, #5            // Initialiser r0 à 5

    bl loop                // Appeler la fonction loop

    bl end                 // Appel à la fin (end)

loop:
    cmp r0, #0             // Comparer r0 avec 0
    beq end                 // Si r0 == 0, sauter à 'end'
    subs r0, r0, #1         // Soustraire 1 de r0
    b loop                  // Revenir à 'loop'

end:
    swi 0x123456            // Appel système pour terminer

