#!/bin/bash

# Vérifier que le paramètre est bien passé
if [ -z "$1" ]; then
    echo "Erreur : Aucun paramètre spécifié. Utilisez un paramètre entre 1 et 7."
    exit 1
fi

# Utilisation de la structure case pour exécuter le script en fonction du paramètre
case $1 in
    1)
        chmod +x ./tests/TEST_ETAPE1/test_ETAPE1.sh
        ./tests/TEST_ETAPE1/test_ETAPE1.sh 1
        ;;
    2)
        chmod +x ./tests/TEST_ETAPE2/test_ETAPE2.sh
        ./tests/TEST_ETAPE2/test_ETAPE2.sh 2
        ;;
    3)
        chmod +x ./tests/TEST_ETAPE3/test_ETAPE3.sh
        ./tests/TEST_ETAPE3/test_ETAPE3.sh 3
        ;;
    4)
        chmod +x ./tests/TEST_ETAPE4/test_ETAPE4.sh
        ./tests/TEST_ETAPE4/test_ETAPE4.sh 4
        ;;
    5)
        chmod +x ./tests/TEST_ETAPE5/test_ETAPE5.sh
        ./tests/TEST_ETAPE5/test_ETAPE5.sh 5
        ;;
    6)
        chmod +x ./tests/TEST_ETAPE6/test_ETAPE6.sh
        ./tests/TEST_ETAPE6/test_ETAPE6.sh 6
        ;;
    7)
        chmod +x ./tests/TEST_ETAPE7/test_ETAPE7.sh
        ./tests/TEST_ETAPE7/test_ETAPE7.sh 7
        ;;
    *)
        echo "Erreur : Paramètre non valide. Veuillez utiliser un numéro entre 1 et 7."
        exit 1
        ;;
esac
