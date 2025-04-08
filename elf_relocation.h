
#ifndef ELF_RELOCATION_H
#define ELF_RELOCATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <elf.h>  
#include <arpa/inet.h>
#include <string.h>
#include "elf_section.h"
#include "elf_header.h"
#include "elf_section_contenu.h"
#include "elf_relocation.h"

// Define ARM relocation types
#define R_ARM_CALL 28
#define R_ARM_V4BX 40
#define R_ARM_JUMP24 29

/*
   get_relocation_type
   description : Convertit un type de réimplantation en chaîne de caractères.
   parametres : r_type (type de réimplantation à convertir)
   valeur de retour : chaîne de caractères représentant le type
   effets de bord : aucun
*/
const char *get_relocation_type(uint32_t r_type);

/*
   print_relocation_info
   description : Lit et affiche les informations de réimplantation depuis un fichier ELF.
   parametres : fp (fichier ELF)
   valeur de retour : aucun
   effets de bord : affiche les informations à la sortie standard
*/
void print_relocation_info(FILE *fp);

#endif

