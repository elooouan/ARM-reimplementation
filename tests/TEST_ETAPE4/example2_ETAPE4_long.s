    .section .text              @ Section principale de code
    .global _start
_start:
    MOV R0, #0                  @ Initialiser R0 à 0
    LDR R1, =data_array         @ Charger l'adresse de data_array dans R1
    BL process_array            @ Appeler la fonction process_array
    B end_program               @ Sauter à la fin du programme

process_array:
    MOV R2, #0                  @ Initialiser l'indice du tableau à 0
loop:
    LDR R3, [R1, R2, LSL #2]    @ Charger data_array[R2] dans R3
    ADD R3, R3, #10             @ Ajouter 10 à chaque élément
    STR R3, [R1, R2, LSL #2]    @ Stocker la nouvelle valeur
    ADD R2, R2, #1              @ Passer à l'élément suivant
    CMP R2, #100                @ Vérifier si tous les éléments sont traités
    BLT loop                    @ Reprendre la boucle si R2 < 100
    BX LR                       @ Retourner au programme principal

end_program:
    MOV R7, #1                  @ Appel système pour terminer (exit)
    SVC #0

    .section .data              @ Section de données initialisées
data_array:
    .word 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    .word 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    .word 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    .word 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    .word 40, 41, 42, 43, 44, 45, 46, 47, 48, 49
    .word 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
    .word 60, 61, 62, 63, 64, 65, 66, 67, 68, 69
    .word 70, 71, 72, 73, 74, 75, 76, 77, 78, 79
    .word 80, 81, 82, 83, 84, 85, 86, 87, 88, 89
    .word 90, 91, 92, 93, 94, 95, 96, 97, 98, 99

constants:
    .word 0xDEADBEEF            @ Valeur constante hexadécimale
    .float 3.1415926           @ Constante PI
    .double 2.718281828459045  @ Constante e
    .asciz "Large ARM Example" @ Chaîne en lecture seule

    .section .bss              @ Section de données non initialisées
large_buffer:
    .space 1024                @ Buffer de 1 Ko
other_data:
    .space 512                 @ Zone supplémentaire de 512 octets

    .section .rodata           @ Section de données en lecture seule
strings:
    .asciz "Hello, World!"
    .asciz "Testing ARM ELF"
    .asciz "More Read-Only Data"

    .section .debug_info       @ Section pour les informations de débogage
debug_variable1:
    .word 0x12345678
debug_variable2:
    .word 0xABCDEF01

    .section .comment          @ Section pour les commentaires
    .asciz "Generated for testing large ELF files with multiple sections."

    .section .note.gnu.build-id @ Note spécifique GNU
    .align 4
    .long 4                     @ Longueur de la note
    .long 3                     @ Type de la note
    .asciz "GCC"                @ Nom de la note
    .byte 1, 2, 3, 4, 5         @ Données de la note

    .section .custom_section1  @ Section personnalisée
custom_data1:
    .word 100, 200, 300, 400

    .section .custom_section2  @ Une autre section personnalisée
custom_data2:
    .byte 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08

    .section .custom_section3  @ Une troisième section
custom_data3:
    .asciz "This is a test for complex ELF files."
