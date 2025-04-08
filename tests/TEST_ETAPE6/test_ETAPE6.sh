#!/bin/bash

# Trouver tous les fichiers correspondant à *.o_modified dans le répertoire actuel
FILES=$(ls ./tests/TEST_ETAPE6/*.o_modified)

# Vérification si des fichiers ont été trouvés
if [ -z "$FILES" ]; then
    echo "Erreur : Aucun fichier *.o_modified n'a été trouvé dans le répertoire actuel."
    exit 2
fi

# Traitement de chaque fichier correspondant
for FILE in $FILES; do
    echo "Vérification de $FILE ..."

    # Vérification de la présence de la section .REL dans le fichier
    if ./elf_readelf -S "$FILE" | grep -iq "\.rel"; then
        echo "Le fichier $FILE contient une section .REL. ❌"
    else
        echo "Le fichier $FILE ne contient pas de section .REL. ✅"
    fi
done
