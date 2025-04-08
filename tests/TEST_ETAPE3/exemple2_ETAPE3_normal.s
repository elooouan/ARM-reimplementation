    .section .text
    .global _start
_start:
    MOV R0, #3              @ Charger une valeur (par exemple, 3)
    CMP R0, #1              @ Comparer la valeur avec 1
    BGT case_2_and_up       @ Sauter si R0 > 1
    BEQ case_1              @ Aller à case_1 si R0 == 1
    B default_case          @ Sinon, aller au cas par défaut

case_2_and_up:
    CMP R0, #2
    BEQ case_2              @ Aller à case_2 si R0 == 2
    CMP R0, #3
    BEQ case_3              @ Aller à case_3 si R0 == 3
    B default_case          @ Sinon, aller au cas par défaut

case_1:
    MOV R1, #10             @ Exemple d'action pour le cas 1
    B end

case_2:
    MOV R1, #20             @ Exemple d'action pour le cas 2
    B end

case_3:
    MOV R1, #30             @ Exemple d'action pour le cas 3
    B end

default_case:
    MOV R1, #0              @ Valeur par défaut
    B end

end:
    MOV R7, #1              @ Appel système "exit"
    SVC #0
