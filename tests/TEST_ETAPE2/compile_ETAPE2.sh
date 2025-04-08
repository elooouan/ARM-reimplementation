#!/bin/bash

# Définir le compilateur et les options de compilation
ARM_COMPILER="arm-linux-gnueabihf-gcc"
FLAGS="-mbig-endian -marm -g -O2 -nostdlib --entry main -Wl,-EB"

# Liste des fichiers assembleur à compiler
FILES=(
    "./tests/TEST_ETAPE2/example1_ETAPE2.s"
    "./tests/TEST_ETAPE2/example2_ETAPE2.s"
    "./tests/TEST_ETAPE2/example3_ETAPE2.s"
    "./tests/TEST_ETAPE2/example4_ETAPE2.s"
    "./tests/TEST_ETAPE2/example5_ETAPE2.s"
    "./tests/TEST_ETAPE2/example6_ETAPE2.s"
    "./tests/TEST_ETAPE2/example7_ETAPE2.s"
)

# Boucle pour compiler chaque fichier en 32 bits Big Endian
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

