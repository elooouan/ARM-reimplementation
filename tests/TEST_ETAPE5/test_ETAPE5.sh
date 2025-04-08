#!/bin/bash

# Répertoire contenant les fichiers objets
OBJ_DIR="./tests/TEST_ETAPE5"

# Vérification de la présence de fichiers ELF dans le répertoire
ELF_FILES=("$OBJ_DIR"/*.o)

if [ ${#ELF_FILES[@]} -eq 0 ]; then
    echo "Aucun fichier ELF trouvé dans $OBJ_DIR"
    exit 1
fi

# Fichiers temporaires pour les sorties
READELFFILE=$(mktemp)
ELFREADELFFILE=$(mktemp)
DIFF_COUNT_FILE=$(mktemp)

# Initialiser le compteur de différences
echo 0 > "$DIFF_COUNT_FILE"

# Traitement de chaque fichier ELF
for FILE in "${ELF_FILES[@]}"; do
    echo "Comparaison pour le fichier ELF : $FILE"

    # Extraire uniquement les tables de réimplantation sans les noms des sections
    LANG=en_US.UTF-8 readelf -r "$FILE" | awk '/^Relocation section/ {flag=1; next} /^\s*$/ {flag=0} flag' | awk '{print $1, $2, $3, $4}' > "$READELFFILE"
    ./elf_readelf -r "$FILE" | awk '/^Relocation section/ {flag=1; next} /^\s*$/ {flag=0} flag' | awk '{print $1, $2, $3, $4}' > "$ELFREADELFFILE"

    # Nettoyer chaque ligne pour enlever les espaces et caractères invisibles
    sed -i 's/[[:space:]]\+/ /g' "$READELFFILE"
    sed -i 's/[[:space:]]\+/ /g' "$ELFREADELFFILE"
    sed -i 's/^ //; s/ $//' "$READELFFILE"
    sed -i 's/^ //; s/ $//' "$ELFREADELFFILE"

    # Titre de comparaison
    echo ""
    echo "Comparaison Structurée entre readelf et elf_readelf (Avec Mots-Clés Sym.Value et Index en Vert)"
    echo "----------------------------------------------------------------------------------------------"
    printf "%-60s | %-60s\n" "readelf" "elf_readelf"
    echo "----------------------------------------------------------------------------------------------"

    DIFF_COUNT=0
    LINE_NUM=0

    # Lire les deux fichiers ligne par ligne
    paste "$READELFFILE" "$ELFREADELFFILE" | while IFS=$'\t' read -r LINE1 LINE2; do
        ((LINE_NUM++))
        
        # Découper les lignes en colonnes individuelles
        COLUMNS1=($(echo "$LINE1" | awk '{for(i=1;i<=NF;i++) print $i}'))
        COLUMNS2=($(echo "$LINE2" | awk '{for(i=1;i<=NF;i++) print $i}'))
        
        # Vérifier si le nombre de colonnes est différent
        if [ ${#COLUMNS1[@]} -ne ${#COLUMNS2[@]} ]; then
            printf "\033[1;31m%-60s | %-60s (Nombre de colonnes différent)\033[0m\n" "$LINE1" "$LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
            continue
        fi
        
        # Comparaison colonne par colonne
        DIFFERENCE_FOUND=0
        FORMATTED_LINE1=""
        FORMATTED_LINE2=""

        for i in "${!COLUMNS1[@]}"; do
            if [[ "$i" -eq 2 ]]; then
                # Type column is green
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            elif [[ "$i" -eq 3 ]]; then
                # Sym.Value or Index is green
                FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
            else
                # Offset and Info columns
                if [ "${COLUMNS1[$i]}" != "${COLUMNS2[$i]}" ]; then
                    FORMATTED_LINE1+="\033[1;31m${COLUMNS1[$i]} \033[0m"
                    FORMATTED_LINE2+="\033[1;31m${COLUMNS2[$i]} \033[0m"
                    DIFFERENCE_FOUND=1
                else
                    FORMATTED_LINE1+="\033[1;32m${COLUMNS1[$i]} \033[0m"
                    FORMATTED_LINE2+="\033[1;32m${COLUMNS2[$i]} \033[0m"
                fi
            fi
        done

        # Afficher la ligne avec ou sans différences
        if [ $DIFFERENCE_FOUND -eq 1 ]; then
            printf "%-60b | %-60b\n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
        else
            printf "%-60b | %-60b\n" "$FORMATTED_LINE1" "$FORMATTED_LINE2"
        fi
    done

    # Lire le compteur de différences
    DIFF_COUNT=$(<"$DIFF_COUNT_FILE")

    # Afficher le message final
    if [ "$DIFF_COUNT" -eq 0 ]; then
        echo -e "\n\033[1;32mLes tableaux readelf et elf_readelf sont parfaitement identiques.\033[0m"
    else
        echo -e "\n\033[1;31m$DIFF_COUNT différences numériques ont été détectées entre les tableaux.\033[0m"
    fi
done

# Suppression automatique des fichiers temporaires
rm -f "$READELFFILE" "$ELFREADELFFILE" "$DIFF_COUNT_FILE"
