#!/bin/bash

# Définir le compilateur et les options de compilation
ARM_COMPILER="arm-linux-gnueabihf-gcc"
FLAGS="-mbig-endian -marm -g -O2 -nostdlib --entry main -Wl,-EB"

# Liste des fichiers assembleur à compiler
FILES=(
    "./tests/TEST_ETAPE7/example1_ETAPE7.s"
    "./tests/TEST_ETAPE7/example2_ETAPE7.s"
    "./tests/TEST_ETAPE7/example3_ETAPE7.s"
)

# Vérifier la présence du compilateur
if ! command -v $ARM_COMPILER &> /dev/null; then
    echo "Erreur : Le compilateur $ARM_COMPILER n'est pas installé ou introuvable dans le PATH."
    exit 1
fi

# Boucle pour compiler chaque fichier en ELF 32 bits Big Endian
for FILE in "${FILES[@]}"; do
    if [[ -f "$FILE" ]]; then
        OBJ_FILE="${FILE%.s}.o"
        
        echo " Compilation de $FILE en ELF 32 bits Big Endian..."
        $ARM_COMPILER -c $FLAGS -o "$OBJ_FILE" "$FILE"
        
        if [[ $? -eq 0 ]]; then
            echo " Fichier Objet généré : $OBJ_FILE"
        else
            echo " Erreur lors de la compilation de $FILE"
        fi
    else
        echo "⚠️ Fichier $FILE introuvable, saut de ce fichier."
    fi
done

