#!/usr/bin/env bash

DIRECTORY="./tests/TEST_ETAPE7"

echo -e "\n\033[1;32m=== Démarrage de l'exécution automatique dans $DIRECTORY ===\033[0m"

# ----------------------------
# FONCTIONS D'EXTRACTION
# ----------------------------

extract_NOTYPE_symbols_orig() {
  local elf_file=$1
  local output_file=$2

  echo -e "\n\033[1;34m=== Extraction des symboles NOTYPE (original) depuis : $elf_file ===\033[0m"
  ./elf_readelf -s "$elf_file" | awk '
      $4 == "NOTYPE" && $7 != "0" && $8 != "" {
          printf "%s %s %s\n", $2, $7, $8
      }
  ' > "$output_file"
  echo -e "\033[1;33mSymboles NOTYPE extraits (original, sans Ndx=0) :\033[0m"
  cat "$output_file"
}

extract_NOTYPE_symbols_mod() {
  local elf_file=$1
  local output_file=$2

  echo -e "\n\033[1;34m=== Extraction des symboles NOTYPE (modifié) depuis : $elf_file ===\033[0m"
  ./elf_readelf -s "$elf_file" | awk '
      $4 == "NOTYPE" && $7 != "0" && $8 != "" {
          printf "%s %s %s\n", $2, $7, $8
      }
  ' > "$output_file"
  echo -e "\033[1;33mSymboles NOTYPE extraits (modifié, sans Ndx=0) :\033[0m"
  cat "$output_file"
}

extract_section_addresses_mod() {
  local elf_file=$1
  local output_file=$2

  echo -e "\n\033[1;34m=== Extraction des adresses de sections (modifié) depuis : $elf_file ===\033[0m"

  > "$output_file"
  ./elf_readelf -S "$elf_file" | while IFS= read -r line; do
    if [[ "$line" =~ ^[[:space:]]*\[[[:space:]]*([0-9]+)\][[:space:]]+([^[:space:]]+)[[:space:]]+([^[:space:]]+)[[:space:]]+([[:xdigit:]]+) ]]; then
      local sect_index="${BASH_REMATCH[1]}"
      local sect_addr="${BASH_REMATCH[4]}"

      if [[ "$sect_index" -eq 0 ]]; then
        continue
      fi

      echo "$sect_index $sect_addr" >> "$output_file"
    fi
  done
  echo -e "\033[1;33mAdresses de sections extraites :\033[0m"
  cat "$output_file"
}

# ----------------------------
# FONCTION DE VÉRIFICATION 
# ----------------------------
check_symbols_modindex() {
  local file_orig=$1
  local file_mod=$2

  # Fichiers temporaires
  local SYM_ORIG=$(mktemp)
  local SYM_MOD=$(mktemp)
  local SEC_MOD=$(mktemp)

  # 1) Extraire symboles NOTYPE (original)
  extract_NOTYPE_symbols_orig "$file_orig" "$SYM_ORIG"

  # 2) Extraire symboles NOTYPE (modifié)
  extract_NOTYPE_symbols_mod "$file_mod" "$SYM_MOD"

  # 3) Extraire adresses de sections (modifié)
  extract_section_addresses_mod "$file_mod" "$SEC_MOD"

  echo -e "\n\033[1;34m=== Vérification (ind. du modifié) : valeur_orig + addr_section_mod = valeur_mod ? ===\033[0m"
  local DIFF_COUNT=0

  # Pour chaque symbole du fichier original (Ndx=0 ignoré)
  while read -r orig_val_hex dummy_orig_ndx orig_name; do
      # Chercher le symbole (même nom) dans SYM_MOD
      mod_line=$(grep " $orig_name$" "$SYM_MOD" | head -n1)
      if [[ -z "$mod_line" ]]; then
        echo -e "\033[1;31m[ERREUR]\033[0m Symbole '$orig_name' introuvable dans le modifié."
        ((DIFF_COUNT++))
        continue
      fi

      # Valeur_mod, ndx_mod, Nom
      mod_val_hex=$(echo "$mod_line" | awk '{print $1}')
      mod_ndx=$(echo "$mod_line" | awk '{print $2}')

      # Retrouver l'adresse de la section ndx_mod
      addr_line=$(grep "^$mod_ndx " "$SEC_MOD" | head -n1)
      if [[ -z "$addr_line" ]]; then
        echo -e "\033[1;31m[ERREUR]\033[0m Section '$mod_ndx' (pour '$orig_name') non trouvée dans le fichier modifié."
        ((DIFF_COUNT++))
        continue
      fi
      addr_hex=$(echo "$addr_line" | awk '{print $2}')

      # Calcul
      orig_val_dec=$((0x$orig_val_hex))
      addr_dec=$((0x$addr_hex))
      mod_val_dec=$((0x$mod_val_hex))
      total_dec=$((orig_val_dec + addr_dec))

      # Vérification
      if [[ $total_dec -eq $mod_val_dec ]]; then
        echo -e "\033[1;32m[OK]\033[0m $orig_name => \033[36m$orig_val_hex + $addr_hex = $mod_val_hex\033[0m"
      else
        echo -e "\033[1;31m[ERREUR]\033[0m $orig_name => \033[36m$orig_val_hex + $addr_hex != $mod_val_hex\033[0m"
        ((DIFF_COUNT++))
      fi

  done < "$SYM_ORIG"

  if [[ $DIFF_COUNT -eq 0 ]]; then
    echo -e "\n\033[1;32m✅ Aucune incohérence détectée pour $(basename "$file_orig").\033[0m"
  else
    echo -e "\n\033[1;31m❌ $DIFF_COUNT incohérence(s) détectée(s) pour $(basename "$file_orig").\033[0m"
  fi

  rm -f "$SYM_ORIG" "$SYM_MOD" "$SEC_MOD"
}


# =====================
# BOUCLE PRINCIPALE
# =====================
# Parcours des fichiers .o non suffixés par "_modified"
# afin de leur associer le fichier modifié correspondant.
for orig_file in "$DIRECTORY"/*.o; do
  # S'il n'y a pas de .o du tout, la boucle ne s'exécute pas
  if [[ "$orig_file" == *_modified.o ]]; then
    # Ignore les fichiers déjà suffixés par "_modified.o"
    continue
  fi
  # Construit le nom du modifié
  mod_file="${orig_file}_modified"
  if [[ ! -f "$mod_file" ]]; then
    echo -e "\033[1;31mAucun fichier modifié pour $(basename "$orig_file") (attendu: $(basename "$mod_file")).\033[0m"
    continue
  fi

  echo
  echo -e "\033[1;32m=== Traitement de la paire : $(basename "$orig_file") et $(basename "$mod_file") ===\033[0m"
  check_symbols_modindex "$orig_file" "$mod_file"
done

echo
echo -e "\033[1;32m=== Fin du script (traitement automatique) ===\033[0m"
