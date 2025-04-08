#!/bin/bash

# Liste des fichiers ELF à tester
FILES=(
    "./tests/TEST_ETAPE2/example1_ETAPE2.o"
    "./tests/TEST_ETAPE2/example2_ETAPE2.o"
    "./tests/TEST_ETAPE2/example3_ETAPE2.o"
    "./tests/TEST_ETAPE2/example4_ETAPE2.o"
    "./tests/TEST_ETAPE2/example5_ETAPE2.o"
    "./tests/TEST_ETAPE2/example6_ETAPE2.o"
    "./tests/TEST_ETAPE2/example7_ETAPE2.o"
)

# Vérification de l'existence des fichiers et lancement de la comparaison pour chaque fichier
for FILE in "${FILES[@]}"; do
    if [ ! -f "$FILE" ]; then
        echo "Erreur : Le fichier $FILE n'existe pas."
        continue
    fi

    # Fichiers temporaires pour les sorties
    READELFFILE=$(mktemp)
    ELFREADELFFILE=$(mktemp)
    DIFF_COUNT_FILE=$(mktemp)

    # Initialiser le compteur de différences
    echo 0 > "$DIFF_COUNT_FILE"

    # Extraire les tableaux des sections
    LANG=en_US.UTF-8 readelf -S "$FILE" | awk '/\[Nr\]/ {flag=1; next} /^\s*$/ {flag=0} flag' > "$READELFFILE"
    ./elf_readelf -S "$FILE" | awk '/\[Nr\]/ {flag=1; next} /^\s*$/ {flag=0} flag' > "$ELFREADELFFILE"

    # Nettoyer les lignes
    sed -i 's/[[:space:]]\+/ /g' "$READELFFILE"
    sed -i 's/[[:space:]]\+/ /g' "$ELFREADELFFILE"
    sed -i 's/^ //; s/ $//' "$READELFFILE"
    sed -i 's/^ //; s/ $//' "$ELFREADELFFILE"

    # Titre de comparaison
    echo ""
    echo "Comparaison Structurée entre readelf et elf_readelf (Différences en rouge avec Éléments en Vert)"
    echo "----------------------------------------------------------------------------------------------"
    printf "%-60s | %-60s\n" "readelf" "elf_readelf"
    echo "----------------------------------------------------------------------------------------------"

    LINE_NUM=0

    # Lire les deux fichiers ligne par ligne
    paste "$READELFFILE" "$ELFREADELFFILE" | while IFS=$'\t' read -r LINE1 LINE2; do
        ((LINE_NUM++))
        
        # Découper les lignes en colonnes individuelles
        COLUMNS1=($(echo "$LINE1" | tr -s ' '))
        COLUMNS2=($(echo "$LINE2" | tr -s ' '))
        
        # Comparer colonne par colonne et formater les différences
        DIFFERENCE_FOUND=0
        FORMATTED_LINE1=""
        FORMATTED_LINE2=""

        for i in "${!COLUMNS1[@]}"; do
            if [ "${COLUMNS1[$i]}" == "${COLUMNS2[$i]}" ]; then
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"  # Vert pour les éléments communs
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            else
                FORMATTED_LINE1+="\033[1;31m${COLUMNS1[$i]} \033[0m"  # Rouge pour les différences
                FORMATTED_LINE2+="\033[1;31m${COLUMNS2[$i]} \033[0m"
                DIFFERENCE_FOUND=1
            fi
        done

        # Afficher uniquement si une différence est détectée
        if [ $DIFFERENCE_FOUND -eq 1 ]; then
            printf "%-60b | %-60b \n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
        fi
    done

    # Lire le compteur de différences
    DIFF_COUNT=$(<"$DIFF_COUNT_FILE")

    # Afficher le message final avec couleur jaune
    if [ "$DIFF_COUNT" -eq 0 ]; then
        echo -e "\n\033[1;33mAucune différence détectée entre readelf et elf_readelf dans $FILE.\033[0m"
    else
        echo -e "\n\033[1;33m$DIFF_COUNT différences ont été détectées entre readelf et elf_readelf dans $FILE.\033[0m"
    fi

    # Nettoyage des fichiers temporaires
    rm "$READELFFILE" "$ELFREADELFFILE" "$DIFF_COUNT_FILE"
done
