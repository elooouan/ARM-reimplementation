#!/bin/bash

# Définir les répertoires et fichiers à exécuter
tests=("ETAPE1" "ETAPE2" "ETAPE3" "ETAPE4" "ETAPE5" "ETAPE6" "ETAPE7")

# Permission 

chmod +x tests.sh
chmod +x supprimer_objet_tests.sh
chmod +x ./tests/TEST_ETAPE6/supprimer_REL.sh
chmod +x ./tests/TEST_ETAPE7/supprimer_REL.sh

# Boucle pour exécuter chaque étape
for test in "${tests[@]}"
do  
    chmod +x ./tests/TEST_$test/compile_$test.sh
    echo "Exécution de ./tests/TEST_$test/compile_$test.sh"
    ./tests/TEST_$test/compile_$test.sh
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution de ./tests/TEST_$test/compile_$test.sh"
        exit 1
    fi
    echo ""
    echo "-------------------------------------------------------------------------"
    echo "-------------------------------------------------------------------------"
    echo "-------------------------------------------------------------------------"
    echo ""
done

echo "Toutes les étapes ont été exécutées avec succès."
