#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "elf_section_contenu.h"

// Fonction pour afficher le contenu d'une section en hexadécimal et ASCII
void afficher_contenu_section(FILE *file, Elf32_Shdr *section_header, const char *section_name) {
    Elf32_Off section_offset = get_section_offset(section_header); // Offset dans le fichier
    Elf32_Word section_size = get_section_size(section_header);    // Taille de la section
    Elf32_Addr section_addr = get_section_addr(section_header);    // Adresse virtuelle

    // Se déplacer à l'offset de la section
    if (fseek(file, section_offset, SEEK_SET) != 0) {
        perror("Erreur lors du déplacement vers l'offset de la section");
        return;
    }

    // Lire le contenu de la section
    unsigned char *buffer = malloc(section_size);
    if (!buffer) {
        perror("Erreur d'allocation mémoire");
        return;
    }

    if (fread(buffer, 1, section_size, file) != section_size) {
        perror("Erreur de lecture du contenu de la section");
        free(buffer);
        return;
    }

    // Affichage des informations sur la section
    //printf("Nom de la section: '%s'\n", section_name);
    printf("\nVidange hexadécimale de la section '%s':\n", section_name);

    for (int i = 0; i < section_size; i += 16) {
        // Adresse de la ligne
        printf("  0x%08x ", section_addr + i); // Utilisation de l'adresse virtuelle

        // Affichage des octets en groupes de 4
        for (int j = 0; j < 16 && i + j < section_size; j++) {
            if (j > 0 && j % 4 == 0) {
                printf(" "); // Ajouter un espace après chaque groupe de 4 octets
            }
            printf("%02x", buffer[i + j]);
        }

        // Ajouter des espaces pour compléter la ligne si elle est plus courte que 16 octets
        if (section_size - i < 16) {
            int remaining = 16 - (section_size - i);
            printf(" ");
            for (int k = 0; k < remaining; k++)
            {
                printf("  "); // Ajout d'espaces pour compléter la ligne
            }
        }

        // Affichage des caractères ASCII
        printf(" ");
        for (int j = 0; j < 16 && i + j < section_size; j++) {
            unsigned char c = buffer[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.'); // Caractère ASCII ou '.'
        }
        printf("\n");
    }

    free(buffer);
}

// Fonction pour rechercher une section par son index
void afficher_section_par_index(FILE *file, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrs, char *shstrtab, int section_index) {
    if (section_index < 0 || section_index >= get_section_e_shnum(ehdr)) {
        fprintf(stderr, "Index de section invalide : %d\n", section_index);
        return;
    }

    const char *section_name = get_section_name(section_index, shdrs, shstrtab);
    afficher_contenu_section(file, &shdrs[section_index], section_name);
}

// Fonction pour rechercher une section par son nom
void afficher_section_par_nom(FILE *file, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrs, char *shstrtab, const char *nom) {
    for (int i = 0; i < get_section_e_shnum(ehdr); i++) {
        const char *section_name = get_section_name(i, shdrs, shstrtab);
        if (strcmp(section_name, nom) == 0) {
            afficher_contenu_section(file, &shdrs[i], section_name);
            return;
        }
    }

    fprintf(stderr, "Section '%s' introuvable\n", nom);
}

// Fonction principale pour lire et afficher une section
void read_section_content(const char *filename, const char *section_arg) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    Elf32_Ehdr *ehdr = malloc(sizeof(Elf32_Ehdr));
    if (!ehdr) {
        perror("Erreur d'allocation mémoire pour l'en-tête ELF");
        fclose(file);
        return;
    }

    if (fread(ehdr, sizeof(Elf32_Ehdr), 1, file) != 1) {
        perror("Erreur de lecture de l'en-tête ELF");
        free(ehdr);
        fclose(file);
        return;
    }

    // Vérification du magic number ELF
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Ce n'est pas un fichier ELF valide\n");
        free(ehdr);
        fclose(file);
        return;
    }

    // Lire la table des en-têtes de sections
    fseek(file, get_header_shoff(ehdr), SEEK_SET);

    Elf32_Shdr *shdrs = malloc(ehdr->e_shentsize *ehdr->e_shnum);
    if (!shdrs) {
        perror("Erreur d'allocation mémoire pour les en-têtes de sections");
        free(ehdr);
        fclose(file);
        return;
    }

    if (fread(shdrs, get_header_shentize(ehdr), get_header_shnum(ehdr), file) != get_header_shnum(ehdr)) {
        perror("Erreur de lecture des en-têtes de sections");
        free(shdrs);
        free(ehdr);
        fclose(file);
        return;
    }
    // Lire la table des noms des sections
    char *shstrtab = NULL;
    if (get_header_shstrndx(ehdr) != SHN_UNDEF) {
        Elf32_Shdr *shstrtab_hdr = &shdrs[get_header_shstrndx(ehdr)];
        shstrtab = malloc(shstrtab_hdr->sh_size);
        if (!shstrtab) {
            perror("Erreur d'allocation mémoire pour la table des noms de sections");
            free(shdrs);
            free(ehdr);
            fclose(file);
            return;
        }

        fseek(file, get_section_offset(shstrtab_hdr), SEEK_SET);
        if (fread(shstrtab, get_section_size(shstrtab_hdr), 1, file) != 1) {
            perror("Erreur de lecture de la table des noms de sections");
            free(shstrtab);
            free(shdrs);
            free(ehdr);
            fclose(file);
            return;
        }
    }

    // Vérifier si l'argument est un index ou un nom
    if (isdigit(section_arg[0])) {
        afficher_section_par_index(file, ehdr, shdrs, shstrtab, atoi(section_arg));
    } else {
        afficher_section_par_nom(file, ehdr, shdrs, shstrtab, section_arg);
    }

    // Nettoyage
    free(shstrtab);
    free(shdrs);
    free(ehdr);
    fclose(file);
}