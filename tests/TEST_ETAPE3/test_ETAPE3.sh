#!/bin/bash

# Répertoire des fichiers ELF
TEST_DIR="tests/TEST_ETAPE3"

# Chemin relatif vers elf_readelf
ELF_READELF="./elf_readelf"

# Tableau pour stocker les sections avec des différences
declare -A DIFF_SECTIONS

# Fonction pour afficher une comparaison structurée des sections
compare_sections() {
    local FILE=$1
    local SECTION=$2

    # Fichiers temporaires pour les sorties
    READELFFILE=$(mktemp)
    ELFREADELFFILE=$(mktemp)
    DIFF_COUNT_FILE=$(mktemp)

    # Initialiser le compteur de différences
    echo 0 > "$DIFF_COUNT_FILE"

    # Extraire le contenu de la section avec readelf et elf_readelf
    LANG=en_US.UTF-8 readelf -x "$SECTION" "$FILE" > "$READELFFILE" 2>/dev/null
    $ELF_READELF -x "$FILE" "$SECTION" > "$ELFREADELFFILE" 2>/dev/null

    # Nettoyer les fichiers de sortie pour ignorer les lignes inutiles
    grep -v '^Hex dump of section' "$READELFFILE" | grep -v '^NOTE:' | grep -v 'NOTE: This section has relocations against it, but these have NOT been applied to this dump.' | grep -v '^Section .* has no data to dump.' > "${READELFFILE}_cleaned"
    grep -v '^Vidange hexadécimale de la section' "$ELFREADELFFILE" | grep -v '^NOTE:' | grep -v 'NOTE: This section has relocations against it, but these have NOT been applied to this dump.' | grep -v '^Section .* has no data to dump.' > "${ELFREADELFFILE}_cleaned"

    # Nettoyer les fichiers de sortie pour ignorer les lignes inutiles
    mv "${READELFFILE}_cleaned" "$READELFFILE"
    mv "${ELFREADELFFILE}_cleaned" "$ELFREADELFFILE"

    # Supprimer les espaces inutiles et nettoyer les lignes
    sed -i 's/[[:space:]]\+/ /g' "$READELFFILE"
    sed -i 's/[[:space:]]\+/ /g' "$ELFREADELFFILE"
    sed -i 's/^ //; s/ $//' "$READELFFILE"
    sed -i 's/^ //; s/ $//' "$ELFREADELFFILE"

    # Titre de comparaison
    echo -e "\n🔍 \033[1;34mComparaison de la section $SECTION entre readelf et elf_readelf\033[0m"
    echo "----------------------------------------------------------------------------------------------"
    printf "%-60s | %-60s\n" " elf_readelf" " readelf"
    echo "----------------------------------------------------------------------------------------------"

    # Lire les deux fichiers ligne par ligne
    paste "$READELFFILE" "$ELFREADELFFILE" | while IFS=$'\t' read -r LINE1 LINE2; do
        # Comparaison des lignes
        if [ "$LINE1" != "$LINE2" ]; then
            printf "\033[1;31m%-60s | %-60s ❌\033[0m\n" "$LINE1" "$LINE2"
            COUNT=$(<"$DIFF_COUNT_FILE")
            echo $((COUNT + 1)) > "$DIFF_COUNT_FILE"
        else
            printf "\033[1;32m%-60s | %-60s ✅\033[0m\n" "$LINE1" "$LINE2"
        fi
    done

    # Lire le compteur de différences
    DIFF_COUNT=$(<"$DIFF_COUNT_FILE")

    # Enregistrer les sections avec des différences
    if [ "$DIFF_COUNT" -gt 0 ]; then
        DIFF_SECTIONS["$FILE"]+="$SECTION, "
    fi

    # Afficher le message final
    if [ "$DIFF_COUNT" -eq 0 ]; then
        echo -e "\n\033[1;32mLes données de la section $SECTION sont parfaitement identiques.\033[0m"
    else
        echo -e "\n\033[1;31m$DIFF_COUNT différences détectées dans la section $SECTION.\033[0m"
    fi

    # Nettoyage des fichiers temporaires
    rm "$READELFFILE" "$ELFREADELFFILE" "$DIFF_COUNT_FILE"
}

# Parcourir les fichiers ELF générés
for OBJ_FILE in "$TEST_DIR"/*.o; do
    if [[ -f "$OBJ_FILE" ]]; then
        echo ""
        echo "---------------------------------------------------------------------------"
        echo "🔍 Test des sections du fichier $OBJ_FILE"
        echo "---------------------------------------------------------------------------"

        # Lister les noms des sections du fichier ELF
        SECTIONS=$(readelf -S "$OBJ_FILE" | awk '/[0-9]+\]/ {print $2}' | tr -d '[]')

        # Comparer chaque section
        for SECTION in $SECTIONS; do
            compare_sections "$OBJ_FILE" "$SECTION"
        done
    else
        echo "⚠️ Aucun fichier ELF trouvé dans $TEST_DIR."
    fi
done

# Afficher les sections avec des différences
echo -e "\n Résumé des différences détectées :"
for FILE in "${!DIFF_SECTIONS[@]}"; do
    SECTIONS_LIST="${DIFF_SECTIONS[$FILE]}"
    echo -e "Fichier $FILE : Sections avec différences : ${SECTIONS_LIST}"
done

# Afficher le message final
echo "Tests terminés."
