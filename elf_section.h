#ifndef __ELF_SECTION_H__
#define __ELF_SECTION_H__

#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include "elf_header.h"

/*
   get_section_name
   description : Récupère le nom d'une section à partir de son index.
   parametres : 
       section_index (index de la section),
       shdrs (table des en-têtes de sections),
       shstrtab (table des noms des sections)
   valeur de retour : Chaîne contenant le nom de la section.
   effets de bord : aucun
*/
const char *get_section_name(int section_index, Elf32_Shdr *shdrs, char *shstrtab);

/*
   get_section_type
   description : Récupère le type d'une section.
   parametres : Section_header (pointeur vers l'en-tête de la section)
   valeur de retour : Type de la section (valeur numérique).
   effets de bord : aucun
*/
Elf32_Word get_section_type(Elf32_Shdr *Section_header);

/*
   get_section_offset
   description : Récupère l'offset de la section dans le fichier ELF.
   parametres : shdr (pointeur vers l'en-tête de la section)
   valeur de retour : Offset de la section.
   effets de bord : aucun
*/
Elf32_Off get_section_offset(Elf32_Shdr *shdr);

/*
   get_section_e_shnum
   description : Récupère le nombre total de sections dans le fichier ELF.
   parametres : shdr (pointeur vers l'en-tête ELF)
   valeur de retour : Nombre de sections.
   effets de bord : aucun
*/
Elf32_Half get_section_e_shnum(Elf32_Ehdr *shdr);

/*
   get_section_size
   description : Récupère la taille d'une section.
   parametres : shdr (pointeur vers l'en-tête de la section)
   valeur de retour : Taille de la section en octets.
   effets de bord : aucun
*/
Elf32_Word get_section_size(Elf32_Shdr *shdr);

/*
   get_section_addr
   description : Récupère l'adresse virtuelle de la section.
   parametres : shdr (pointeur vers l'en-tête de la section)
   valeur de retour : Adresse virtuelle de la section.
   effets de bord : aucun
*/
Elf32_Addr get_section_addr(Elf32_Shdr *shdr);

/*
   get_section_addralign
   description : Récupère l'alignement de la section.
   parametres : section (pointeur vers l'en-tête de la section)
   valeur de retour : Valeur d'alignement de la section.
   effets de bord : aucun
*/
Elf32_Word get_section_addralign(Elf32_Shdr *section);

/*
   get_section_flags
   description : Récupère les flags associés à la section.
   parametres : section (pointeur vers l'en-tête de la section)
   valeur de retour : Flags de la section.
   effets de bord : aucun
*/
Elf32_Word get_section_flags(Elf32_Shdr *section);

/*
   get_section_entsize
   description : Récupère la taille d'entrée (entsize) d'une section.
   parametres : section (pointeur vers l'en-tête de la section)
   valeur de retour : Taille d'entrée de la section.
   effets de bord : aucun
*/
Elf32_Word get_section_entsize(Elf32_Shdr *section);

/*
   get_section_link
   description : Récupère le lien (link) associé à une section.
   parametres : section (pointeur vers l'en-tête de la section)
   valeur de retour : Index du lien de la section.
   effets de bord : aucun
*/
Elf32_Word get_section_link(Elf32_Shdr *section);

/*
   get_section_info
   description : Récupère les informations supplémentaires associées à une section.
   parametres : section (pointeur vers l'en-tête de la section)
   valeur de retour : Valeur d'information supplémentaire de la section.
   effets de bord : aucun
*/
Elf32_Word get_section_info(Elf32_Shdr *section);

/*
   get_section_type_name
   description : Retourne le nom du type de section sous forme de chaîne.
   parametres : sh_type (type de section)
   valeur de retour : Chaîne décrivant le type de la section.
   effets de bord : aucun
*/
const char *get_section_type_name(Elf32_Word sh_type);

/*
   get_sections
   description : Récupère les en-têtes des sections à partir du fichier ELF.
   parametres : 
       file (pointeur vers le fichier ELF),
       header (pointeur vers l'en-tête ELF)
   valeur de retour : Tableau d'en-têtes de sections.
   effets de bord : Alloue dynamiquement de la mémoire pour les en-têtes de sections.
*/
Elf32_Shdr *get_sections(FILE *file, Elf32_Ehdr *header);

/*
   get_shstrtab
   description : Récupère la table des noms des sections (shstrtab) à partir du fichier ELF.
   parametres : 
       file (pointeur vers le fichier ELF),
       header (pointeur vers l'en-tête ELF),
       shdrs (table des en-têtes de sections)
   valeur de retour : Tableau contenant les noms des sections.
   effets de bord : Alloue dynamiquement de la mémoire pour la table des noms.
*/
char *get_shstrtab(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *shdrs);

/*
   afficher_section_headers
   description : Lit et affiche les en-têtes des sections du fichier ELF.
   parametres : file (pointeur vers le fichier ELF)
   valeur de retour : aucun
   effets de bord : Affiche les informations des sections à la sortie standard.
*/
void afficher_section_headers(FILE *file);

#endif // __ELF_SECTION_H__
