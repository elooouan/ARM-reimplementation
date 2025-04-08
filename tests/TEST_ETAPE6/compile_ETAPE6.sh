#!/bin/bash

# Définir le compilateur et les options de compilation pour ARM 32 bits Little Endian
ARM_COMPILER="arm-none-eabi-gcc"
FLAGS="-mbig-endian -marm -g -O2 -nostdlib --entry main -Wl,-EB"

# Liste des fichiers assembleur à compiler
FILES=(
    "./tests/TEST_ETAPE6/example1_ETAPE6.s"
    "./tests/TEST_ETAPE6/example2_ETAPE6.s"
    "./tests/TEST_ETAPE6/example3_ETAPE6.s"
    "./tests/TEST_ETAPE6/example4_ETAPE6.s"
    "./tests/TEST_ETAPE6/example5_ETAPE6.s"
    "./tests/TEST_ETAPE6/example6_ETAPE6.s"
)

# Boucle pour compiler chaque fichier en 32 bits Little Endian
for FILE in "${FILES[@]}"; do
    if [[ -f "$FILE" ]]; then
        OBJ_FILE="${FILE%.s}.o"
        
        echo "🔄 Compilation de $FILE en ELF 32 bits Big Endian..."
        $ARM_COMPILER -c $FLAGS -o "$OBJ_FILE" "$FILE"
        
        echo "✅ Fichier Objet généré : $OBJ_FILE"
    else
        echo "⚠️ Fichier $FILE introuvable, saut du fichier."
    fi
done

# Afficher le résultat final
echo "🚀 Compilation terminée. Les fichiers objets ELF sont disponibles dans le dossier actuel."
