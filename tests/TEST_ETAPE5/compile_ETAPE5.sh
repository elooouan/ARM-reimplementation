#!/bin/bash

# Compilateur pour ARM 32 bits
ARM_COMPILER="arm-linux-gnueabihf-gcc"  # Utilisez le bon compilateur pour votre environnement

# Options de compilation pour ARM 32 bits
CFLAGS="-mbig-endian -marm -g -O2 -nostdlib --entry main -Wl,-EB"

# Répertoire des fichiers source
SRC_DIR="./tests/TEST_ETAPE5"

# Fichiers source à compiler (de example1_ETAPE5.s à example7_ETAPE5.s)
for i in {1..7}; do
    src_file="${SRC_DIR}/example${i}_ETAPE5.s"
    
    # Vérifier que le fichier source existe
    if [ ! -f "$src_file" ]; then
        echo "Fichier source introuvable: $src_file"
        continue
    fi

    # Nom du fichier objet (sans extension .s)
    obj_file="${src_file%.*}.o"

    # Compiler le fichier source en fichier objet
    echo "Compilation de $src_file..."
    $ARM_COMPILER $CFLAGS -c "$src_file" -o "$obj_file"

    # Vérifier si la compilation a réussi
    if [ $? -eq 0 ]; then
        echo "Compilation réussie: $obj_file"
    else
        echo "Échec de la compilation: $src_file"
    fi
done
