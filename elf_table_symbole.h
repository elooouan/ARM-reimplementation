#ifndef __ELF_TABLE_SYMBOLE__
#define __ELF_TABLE_SYMBOLE__

#include "elf_section_contenu.h"

/*
   get_symbol_type
   description : Récupère le type d'un symbole ELF sous forme de chaîne.
   parametres : info (octet contenant les informations sur le type et le binding du symbole)
   valeur de retour : Chaîne décrivant le type du symbole.
   effets de bord : aucun
*/
const char *get_symbol_type(unsigned char info);

/*
   get_symbol_binding
   description : Récupère la portée (binding) d'un symbole ELF sous forme de chaîne.
   parametres : info (octet contenant les informations sur le type et le binding du symbole)
   valeur de retour : Chaîne décrivant la portée du symbole.
   effets de bord : aucun
*/
const char *get_symbol_binding(unsigned char info);

/*
   get_symbol_visibility
   description : Récupère la visibilité d'un symbole ELF sous forme de chaîne.
   parametres : other (octet contenant les informations sur la visibilité du symbole)
   valeur de retour : Chaîne décrivant la visibilité du symbole.
   effets de bord : aucun
*/
const char *get_symbol_visibility(unsigned char other);

/*
   get_symbol_name
   description : Récupère l'offset du nom du symbole dans la table des chaînes de caractères.
   parametres : Symbol (pointeur vers le symbole ELF)
   valeur de retour : Offset du nom du symbole.
   effets de bord : aucun
*/
Elf32_Word get_symbol_name(Elf32_Sym *Symbol);

/*
   get_symbol_value
   description : Récupère la valeur associée à un symbole ELF.
   parametres : Symbol (pointeur vers le symbole ELF)
   valeur de retour : Valeur du symbole (adresse ou constante).
   effets de bord : aucun
*/
Elf32_Addr get_symbol_value(Elf32_Sym *Symbol);

/*
   get_symbol_size
   description : Récupère la taille d'un symbole ELF.
   parametres : Symbol (pointeur vers le symbole ELF)
   valeur de retour : Taille du symbole en octets.
   effets de bord : aucun
*/
Elf32_Word get_symbol_size(Elf32_Sym *Symbol);

/*
   get_symbol_shndx
   description : Récupère l'index de la section associée au symbole.
   parametres : Symbole (pointeur vers le symbole ELF)
   valeur de retour : Index de la section associée au symbole.
   effets de bord : aucun
*/
Elf32_Half get_symbol_shndx(Elf32_Sym *Symbole);

/*
   afficher_table_symboles
   description : Lit et affiche la table des symboles d'un fichier ELF.
   parametres : file (pointeur vers le fichier ELF)
   valeur de retour : aucun
   effets de bord : Affiche les informations de la table des symboles à la sortie standard.
*/
void afficher_table_symboles(FILE *file);

#endif
