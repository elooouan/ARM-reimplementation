#!/bin/bash

# Liste des fichiers ELF à tester
FILES=(
    "./tests/TEST_ETAPE4/example1_ETAPE4_normal.o"
    "./tests/TEST_ETAPE4/example2_ETAPE4_long.o"
    "./tests/TEST_ETAPE4/example3_ETAPE4_vide.o"
    "./tests/TEST_ETAPE4/example4_ETAPE4_erreur.o"
    "./tests/TEST_ETAPE4/example5_ETAPE4_REL.o"
    "./tests/TEST_ETAPE4/example6_ETAPE4_condtionnel.o"
    "./tests/TEST_ETAPE4/example7_ETAPE4.o"
)

# Vérification de l'existence des fichiers
for FILE in "${FILES[@]}"; do
    if [ ! -f "$FILE" ]; then
        echo "Erreur : Le fichier $FILE n'existe pas."
        exit 1
    fi
done

# Fichiers temporaires pour les sorties
READELFFILE=$(mktemp)
ELFREADELFFILE=$(mktemp)
DIFF_COUNT_FILE=$(mktemp)

# Initialiser le compteur de différences
echo 0 > "$DIFF_COUNT_FILE"

# Traitement des fichiers un par un
for FILE in "${FILES[@]}"; do
    echo -e "\n🔍 Test sur le fichier : $FILE 🔍"
    
    # Extraire les tables des symboles
    LANG=en_US.UTF-8 readelf -s "$FILE" | awk '/Num:/ {flag=1; next} /^\s*$/ {flag=0} flag' | sed 's/^[[:space:]]*\([0-9]\+\):/\1/' > "$READELFFILE"
    ./elf_readelf -s "$FILE" | awk '/Num:/ {flag=1; next} /^\s*$/ {flag=0} flag' | tail -n +2 > "$ELFREADELFFILE"

    # Nettoyer chaque ligne pour enlever les espaces et caractères invisibles
    sed -i 's/[[:space:]]\+/ /g' "$READELFFILE"
    sed -i 's/[[:space:]]\+/ /g' "$ELFREADELFFILE"
    sed -i 's/^ //; s/ $//' "$READELFFILE"
    sed -i 's/^ //; s/ $//' "$ELFREADELFFILE"

    # Comparaison ligne par ligne
    LINE_NUM=0
    paste "$READELFFILE" "$ELFREADELFFILE" | while IFS=$'\t' read -r LINE1 LINE2; do
        ((LINE_NUM++))
        COLUMNS1=($(echo "$LINE1" | tr -s ' '))
        COLUMNS2=($(echo "$LINE2" | tr -s ' '))

        DIFFERENCE_FOUND=0
        FORMATTED_LINE1=""
        FORMATTED_LINE2=""

        for i in "${!COLUMNS1[@]}"; do
            NUM1=$(echo "${COLUMNS1[$i]}" | sed 's/^0*//')
            NUM2=$(echo "${COLUMNS2[$i]}" | sed 's/^0*//')

            # Si l'un des éléments est "UND" et l'autre "0", ignorer cette différence
            if [[ ( "${COLUMNS1[$i]}" == "UND" && "${COLUMNS2[$i]}" == "0" ) || ( "${COLUMNS1[$i]}" == "0" && "${COLUMNS2[$i]}" == "UND" ) ]]; then
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            elif [ "$NUM1" != "$NUM2" ]; then
                FORMATTED_LINE1+="\033[1;31m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;31m${COLUMNS2[$i]} \033[0m"
                DIFFERENCE_FOUND=1
            else
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            fi
        done

        # Afficher les lignes, qu'il y ait une différence ou non
        if [ $DIFFERENCE_FOUND -eq 1 ]; then
            printf "%-60b | %-60b ❌\n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
        else
            printf "%-60b | %-60b ✔️\n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
        fi
    done

    # Lire le compteur de différences
    DIFF_COUNT=$(<"$DIFF_COUNT_FILE")

    # Afficher le message final pour ce fichier
    if [ "$DIFF_COUNT" -eq 0 ]; then
        echo -e "\033[1;32mTest réussi : Les symboles readelf et elf_readelf sont parfaitement identiques.\033[0m"
    else
        echo -e "\033[1;31m$DIFF_COUNT différences numériques détectées dans $FILE.\033[0m"
    fi

    # Réinitialiser le compteur pour le fichier suivant
    echo 0 > "$DIFF_COUNT_FILE"
done

# Nettoyage des fichiers temporaires
rm "$READELFFILE" "$ELFREADELFFILE" "$DIFF_COUNT_FILE"
