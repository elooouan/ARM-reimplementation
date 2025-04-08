#!/bin/bash

# Liste des fichiers ELF à tester
FILES=("./tests/TEST_ETAPE1/example1_ETAPE1.o" "./tests/TEST_ETAPE1/example2_ETAPE1.o")
echo "----------------------------------------------------------------------------------------------"

# Vérification de l'existence des fichiers
for FILE in "${FILES[@]}"; do
    if [ ! -f "$FILE" ]; then
        echo "Erreur : Le fichier $FILE n'existe pas."
        exit 1
    fi
    echo "Fichier testé: $FILE"

    # Fichiers temporaires pour les sorties
    READELFFILE=$(mktemp)
    ELFREADELFFILE=$(mktemp)
    DIFF_COUNT_FILE=$(mktemp)

    # Initialiser le compteur de différences
    echo 0 > "$DIFF_COUNT_FILE"

    # Extraire les tables des symboles
    LANG=en_US.UTF-8 readelf -h "$FILE" | awk -F': ' 'NF > 1 {print $2}' > "$READELFFILE"
    ./elf_readelf -h "$FILE" | awk -F': ' 'NF > 1 {print $2}' > "$ELFREADELFFILE"

    # Nettoyer chaque ligne pour enlever les espaces et caractères invisibles
    sed -i 's/[[:space:]]\+/ /g' "$READELFFILE"
    sed -i 's/[[:space:]]\+/ /g' "$ELFREADELFFILE"
    sed -i 's/^ //; s/ $//' "$READELFFILE"
    sed -i 's/^ //; s/ $//' "$ELFREADELFFILE"


    LINE_NUM=1

    # Lire les deux fichiers ligne par ligne
    paste "$READELFFILE" "$ELFREADELFFILE" | while IFS=$'\t' read -r LINE1 LINE2; do
        ((LINE_NUM++))
        
        # Découper les lignes en colonnes individuelles
        COLUMNS1=($(echo "$LINE1" | tr -s ' '))
        COLUMNS2=($(echo "$LINE2" | tr -s ' '))
        
        # Comparaison colonne par colonne
        DIFFERENCE_FOUND=0
        FORMATTED_LINE1=""
        FORMATTED_LINE2=""

        for i in "${!COLUMNS1[@]}"; do
            NUM1=$(echo "${COLUMNS1[$i]}" | sed 's/^0*//')
            NUM2=$(echo "${COLUMNS2[$i]}" | sed 's/^0*//')
            
            if [ "$NUM1" != "$NUM2" ]; then
                FORMATTED_LINE1+="\033[1;31m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;31m${COLUMNS2[$i]} \033[0m"
                DIFFERENCE_FOUND=1
            else
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            fi
        done

        if [ $DIFFERENCE_FOUND -eq 1 ]; then
            echo -e "\n🔍 \033[1;34mComparaison Structurée entre readelf et elf_readelf (Symboles Alignés)\033[0m 🔍"
            echo "----------------------------------------------------------------------------------------------"
            printf "%-60s | %-60s\n" " elf_readelf" " readelf"
            echo "----------------------------------------------------------------------------------------------"

            printf "%-60b | %-60b ❌ (Ligne $LINE_NUM)\n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
        fi
    done

    # Lire le compteur de différences
    DIFF_COUNT=$(<"$DIFF_COUNT_FILE")

    # Afficher le message final
    if [ "$DIFF_COUNT" -eq 0 ]; then
        echo -e "\n\033[1;32mLes symboles readelf et elf_readelf sont identiques.\033[0m "
    else
        echo -e "\n \033[1;31m$DIFF_COUNT différences ont été détectées entre les symboles.\033[0m "
    fi
    echo "----------------------------------------------------------------------------------------------"

    # Nettoyage des fichiers temporaires
    rm "$READELFFILE" "$ELFREADELFFILE" "$DIFF_COUNT_FILE"
done
