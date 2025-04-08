    .section .text
    .global _start

_start:
    mov r0, #10            @ Charger 10 dans r0
    add r1, r2, r3         @ ERREUR : r1, r2 et r3 doivent être initialisés
    mov r4, r1             @ Déplacer un résultat incorrect dans r4
exit:
    mov r7, #1             @ Appel système pour quitter (numéro de l'appel système)
    swi 0                  @ Effectuer l'appel système
