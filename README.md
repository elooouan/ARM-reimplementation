

# Mode d'Emploi

_Ce document explique brièvement la compilation et l’exécution des programmes utilitaires développés dans le cadre de ce projet._

## <ins>Phase 1</ins>

### _Setup_

Ces programmes ont été ajoutés au fichier `Makefile.am`. Pour les compiler, il suffit de lancer les commandes suivantes : `./configure`, puis `make`. Cela compilera le programme.

Pour exécuter le tout, utilisez le fichier **elf_readelf**, dont le fonctionnement simule la commande `readelf`. 
Sa syntaxe est la suivante: `./elf_readelf <option> <fichier>`
<br></br>

### _Les Options de `./elf_readelf`_ 
Pour afficher le menu des options disponibles, exécutez simplement : `./elf_readelf`

→ Le programme propose **5** options :

**-h** : Affichage de l'en-tête.<br>
**-S** : Affichage de la table des sections.<br>
**-s** : Affichage de la table des symboles.<br>
**-r** : Affichage des tables de réimplantation.<br>
**-x** : Affichage du contenu d'une section. Cette option nécessite un argument supplémentaire (le numéro ou le nom de la section) en plus du nom du fichier en ligne de commande.<br>

**Exemple :** `./elf_readelf -x <fichier_elf> .text ` ou encore `./elf_readelf -x <fichier_elf> 1`


## <ins>Phase 2</ins>

_Cette phase nécessite la mise-en-place d’un simulateur ARM_

### _Setup_

Dans un premier temps, nous avons besoin de faire `./configure`, suivi à nouveau de `make` (si ces derniers n’ont pas déjà était fait)

Ainsi, les fichiers .o seront créés au sein du dossier : **Examples_loader**
<br></br>


### _Réimplantation_

Pour effectuer la réimplantation d’un fichier objet, obtenu par la compilation d’un code assembleur pour ***ARM 32 bits*** en ***big endian***, il suffit d’exécuter `./process_rel`, en passant en argument le fichier à modifier.

Cette commande génère un nouveau fichier ***ELF*** avec le même nom, auquel est ajouté le suffixe *_modified*.<br>

Puis on lance le ***simulateur ARM***, en faisant bien en sorte que le **port (XXXX)** soit libre:<br>
`./arm_simulator --gdb-port XXXX --trace-registers --trace-memory --trace-state SVC &`<br>

Finalement, on charge le fichier précédemment obtenu dans le simulateur au port précédemment utilisé avec la commande:<br>
 `./load_sim Examples_loader/example.o_modified localhost XXXX`


