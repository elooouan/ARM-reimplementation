#!/bin/bash

# Liste des fichiers .o à traiter (tous les fichiers correspondant à example*.o)
FILES=$(ls ./tests/TEST_ETAPE6/example*.o)

# Vérification si des fichiers ont été trouvés
if [ -z "$FILES" ]; then
    echo "Erreur : Aucun fichier example*.o trouvé."
    exit 1
fi

# Boucle pour traiter chaque fichier .o
for FILE in $FILES; do
    # Vérification si le fichier existe
    if [ ! -f "$FILE" ]; then
        echo "Erreur : Le fichier $FILE n'existe pas."
        continue
    fi

    # Vérification que le fichier est bien un fichier ELF
    file "$FILE" | grep -q "ELF"
    if [ $? -ne 0 ]; then
        echo "Erreur : Le fichier $FILE n'est pas un fichier ELF valide."
        continue
    fi

    # Appel à ../../q6_q7 pour générer le fichier .o modifié
    MODIFIED_FILE="${FILE%.o}_modified.o"
    echo "🔄 Appel de ./process_rel pour modifier le fichier $FILE..."

    ./process_rel "$FILE"
    if [ $? -eq 0 ]; then
        echo "✅ Le fichier modifié a été écrit dans : $MODIFIED_FILE"
    else
        echo "Erreur : Échec de la modification du fichier $FILE."
    fi
done
