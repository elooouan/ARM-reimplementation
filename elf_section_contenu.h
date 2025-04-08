#ifndef __ELF_SECTION_CONTENU_H__
#define __ELF_SECTION_CONTENU_H__

#include <stdio.h>
#include <elf.h>
#include "elf_section.h"

/*
   afficher_contenu_section
   description : Affiche le contenu d'une section ELF spécifiée par son en-tête.
   parametres :
       - file : Pointeur vers le fichier ELF ouvert.
       - section_header : Pointeur vers l'en-tête de la section à afficher.
       - section_name : Nom de la section à afficher.
   valeur de retour : aucun
   effets de bord : Affiche le contenu de la section à la sortie standard.
*/
void afficher_contenu_section(FILE *file, Elf32_Shdr *section_header, const char *section_name);

/*
   afficher_section_par_index
   description : Recherche et affiche une section spécifiée par son index.
   parametres :
       - file : Pointeur vers le fichier ELF ouvert.
       - ehdr : Pointeur vers l'en-tête ELF.
       - shdrs : Tableau des en-têtes de sections.
       - shstrtab : Table des chaînes de noms de sections.
       - section_index : Index de la section à afficher.
   valeur de retour : aucun
   effets de bord : Affiche le contenu de la section à la sortie standard.
*/
void afficher_section_par_index(FILE *file, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrs, char *shstrtab, int section_index);

/*
   afficher_section_par_nom
   description : Recherche et affiche une section spécifiée par son nom.
   parametres :
       - file : Pointeur vers le fichier ELF ouvert.
       - ehdr : Pointeur vers l'en-tête ELF.
       - shdrs : Tableau des en-têtes de sections.
       - shstrtab : Table des chaînes de noms de sections.
       - nom : Nom de la section à rechercher et afficher.
   valeur de retour : aucun
   effets de bord : Affiche le contenu de la section à la sortie standard.
*/
void afficher_section_par_nom(FILE *file, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrs, char *shstrtab, const char *nom);

/*
   read_section_content
   description : Fonction principale pour lire et afficher une section du fichier ELF.
   parametres :
       - filename : Nom du fichier ELF à analyser.
       - section_arg : Argument spécifiant la section à afficher (nom ou index).
   valeur de retour : aucun
   effets de bord : Ouvre le fichier ELF, recherche la section spécifiée, et affiche son contenu.
*/
void read_section_content(const char *filename, const char *section_arg);

#endif // __ELF_SECTION_CONTENU_H__