    .section .data
data:
    .word 42                @ Mot de donnée initialisé à 42

    .section .text
    .global _start
_start:
    LDR R0, =data           @ Charger l'adresse de "data" dans R0
    LDR R1, [R0]            @ Charger la valeur de l'adresse pointée par R0 dans R1
    ADD R1, R1, #8          @ Ajouter 8 à la valeur de R1
    STR R1, [R0]            @ Stocker le résultat à l'adresse pointée par R0

    MOV R7, #1              @ Appel système "exit"
    SVC #0                  @ Appel système
