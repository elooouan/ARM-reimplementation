#!/bin/bash

# Définir le compilateur et les options de compilation
ARM_COMPILER="arm-linux-gnueabihf-gcc"
FLAGS="-mbig-endian -marm -g -O2 -nostdlib --entry main -Wl,-EB"

# Répertoire des fichiers assembleur
TEST_DIR="./tests/TEST_ETAPE3"

# Liste des fichiers assembleur à compiler
FILES=(
    "exemple1_ETAPE3_normal.s"
    "exemple2_ETAPE3_normal.s"
    "exemple3_ETAPE3_long.s"
    "exemple4_ETAPE3_vide.s"
    "exemple5_ETAPE3_erreur.s"
    "exemple6_ETAPE3_REL.s"
    "exemple7_ETAPE3_condtionnel.s"
)

# Boucle pour compiler chaque fichier
for FILE in "${FILES[@]}"; do
    FILE_PATH="$TEST_DIR/$FILE"
    if [[ -f "$FILE_PATH" ]]; then
        OBJ_FILE="${FILE_PATH%.s}.o"
        echo ""
        echo " Compilation de $FILE_PATH en ELF 32 bits Big Endian..."
        $ARM_COMPILER -c $FLAGS -o "$OBJ_FILE" "$FILE_PATH"
        
        if [[ $? -ne 0 ]]; then
            echo "❌ Échec de la compilation de $FILE_PATH"
        else
            echo "✅ Fichier Objet généré : $OBJ_FILE"
        fi
    else
        echo "⚠️ Fichier $FILE introuvable dans $TEST_DIR, saut du fichier."
    fi
done

# Afficher le résultat final
echo " Compilation terminée."
