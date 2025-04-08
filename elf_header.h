#ifndef ELF_HEADER_H
#define ELF_HEADER_H

#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <stdlib.h>
#include <arpa/inet.h>

/*
   get_header_type_name
   description : Retourne le nom du type de l'en-tête ELF sous forme de chaîne.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Chaîne représentant le type de fichier ELF.
   effets de bord : aucun
*/
const char *get_header_type_name(Elf32_Ehdr *header);

/*
   get_header_machine_name
   description : Retourne le nom de l'architecture de la machine sous forme de chaîne.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Chaîne représentant l'architecture de la machine.
   effets de bord : aucun
*/
const char *get_header_machine_name(Elf32_Ehdr *header);

/*
   get_header_OSABI_name
   description : Retourne le nom de l'OS/ABI sous forme de chaîne.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Chaîne représentant l'OS/ABI.
   effets de bord : aucun
*/
const char *get_header_OSABI_name(Elf32_Ehdr *header);

/*
   get_header_shoff
   description : Récupère l'offset de la table des en-têtes de sections.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Offset de la table des en-têtes de sections.
   effets de bord : aucun
*/
Elf32_Off get_header_shoff(Elf32_Ehdr *header);

/*
   get_header_phoff
   description : Récupère l'offset de la table des en-têtes de segments.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Offset de la table des en-têtes de segments.
   effets de bord : aucun
*/
Elf32_Off get_header_phoff(Elf32_Ehdr *header);

/*
   get_header_shentize
   description : Récupère la taille d'une entrée dans la table des en-têtes de sections.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Taille d'une entrée dans la table des sections.
   effets de bord : aucun
*/
Elf32_Half get_header_shentize(Elf32_Ehdr *header);

/*
   get_header_phentsize
   description : Récupère la taille d'une entrée dans la table des en-têtes de segments.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Taille d'une entrée dans la table des segments.
   effets de bord : aucun
*/
Elf32_Half get_header_phentsize(Elf32_Ehdr *header);

/*
   get_header_phnum
   description : Récupère le nombre d'entrées dans la table des en-têtes de segments.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Nombre d'entrées dans la table des segments.
   effets de bord : aucun
*/
Elf32_Half get_header_phnum(Elf32_Ehdr *header);

/*
   get_header_ehsize
   description : Récupère la taille de l'en-tête ELF.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Taille de l'en-tête ELF.
   effets de bord : aucun
*/
Elf32_Half get_header_ehsize(Elf32_Ehdr *header);

/*
   get_header_shnum
   description : Récupère le nombre d'entrées dans la table des en-têtes de sections.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Nombre d'entrées dans la table des sections.
   effets de bord : aucun
*/
Elf32_Half get_header_shnum(Elf32_Ehdr *header);

/*
   get_header_shstrndx
   description : Récupère l'index de la table des chaînes de noms de sections.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Index de la table des chaînes de noms de sections.
   effets de bord : aucun
*/
Elf32_Half get_header_shstrndx(Elf32_Ehdr *header);

/*
   get_header_type
   description : Récupère le type de fichier ELF.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Type de fichier ELF.
   effets de bord : aucun
*/
Elf32_Half get_header_type(Elf32_Ehdr *header);

/*
   get_header_machine
   description : Récupère le type d'architecture de la machine.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Type d'architecture de la machine.
   effets de bord : aucun
*/
Elf32_Half get_header_machine(Elf32_Ehdr *header);

/*
   get_header_version
   description : Récupère la version de l'en-tête ELF.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Version de l'en-tête ELF.
   effets de bord : aucun
*/
Elf32_Word get_header_version(Elf32_Ehdr *header);

/*
   get_header_flag
   description : Récupère les flags spécifiques au processeur.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Flags spécifiques au processeur.
   effets de bord : aucun
*/
Elf32_Word get_header_flag(Elf32_Ehdr *header);

/*
   get_header_entry
   description : Récupère l'adresse virtuelle du point d'entrée.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Adresse virtuelle du point d'entrée.
   effets de bord : aucun
*/
Elf32_Addr get_header_entry(Elf32_Ehdr *header);

/*
   get_header_OSABI
   description : Récupère l'identifiant de l'OS/ABI.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Identifiant de l'OS/ABI.
   effets de bord : aucun
*/
unsigned char get_header_OSABI(Elf32_Ehdr *header);

/*
   get_header_ABIVersion
   description : Récupère la version de l'ABI.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Version de l'ABI.
   effets de bord : aucun
*/
unsigned char get_header_ABIVersion(Elf32_Ehdr *header);

/*
   get_header_CLASS
   description : Récupère la classe du fichier ELF (32 bits ou 64 bits).
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Classe du fichier ELF.
   effets de bord : aucun
*/
unsigned char get_header_CLASS(Elf32_Ehdr *header);

/*
   get_header_DATA
   description : Récupère le format d'encodage des données.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : Format d'encodage des données.
   effets de bord : aucun
*/
unsigned char get_header_DATA(Elf32_Ehdr *header);

/*
   get_header
   description : Lit l'en-tête ELF à partir d'un fichier.
   parametres : elf_file (fichier ELF)
   valeur de retour : Pointeur vers l'en-tête ELF ou NULL en cas d'erreur.
   effets de bord : Alloue dynamiquement de la mémoire pour l'en-tête ELF.
*/
Elf32_Ehdr *get_header(FILE *elf_file);

/*
   valider_elf
   description : Vérifie si le fichier est un fichier ELF valide.
   parametres : header (pointeur vers l'en-tête ELF)
   valeur de retour : 1 si valide, 0 sinon.
   effets de bord : Affiche un message d'erreur si le fichier n'est pas valide.
*/
int valider_elf(Elf32_Ehdr *header);

/*
   afficher_entete_elf
   description : Affiche le contenu de l'en-tête ELF.
   parametres : file (fichier ELF)
   valeur de retour : aucun
   effets de bord : Affiche les informations de l'en-tête ELF à la sortie standard.
*/
void afficher_entete_elf(FILE *file);

#endif // ELF_HEADER_H
