#!/bin/bash

# Boucle pour chaque étape (1 à 7)
for i in {1..7}
do
    # Définir le répertoire correspondant à l'étape
    dir="./tests/TEST_ETAPE${i}"

    # Vérifier si le répertoire existe
    if [ -d "$dir" ]; then
        echo "Suppression des fichiers .o dans $dir"

        # Supprimer tous les fichiers .o dans ce répertoire
        find "$dir" -type f -name "*.o" -exec rm -f {} \;

        # Supprimer tous les fichiers .o_modified dans le répertoire de l'étape 6 et 7
        if [[ "$i" -eq 6 || "$i" -eq 7 ]]; then
            find "$dir" -type f -name "*.o_modified" -exec rm -f {} \;
        fi

        echo "Fichiers .o supprimés dans $dir"
    else
        echo "Répertoire $dir non trouvé."
    fi
done

echo "Suppression des fichiers .o et .o_modified terminée."
