#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // Pour htonl et htons
#include "elf_header.h"
#include "elf_section.h"
#include "elf_table_symbole.h"
#include "elf_relocation.h"

void set_header_shoff(Elf32_Ehdr *header, Elf32_Off value) {
    header->e_shoff = htonl(value);
}

Elf32_Word get_sh_name(const Elf32_Shdr *shdr) {
    return ntohl(shdr->sh_name);
}

void set_header_phoff(Elf32_Ehdr *header, Elf32_Off value) {
    header->e_phoff = htonl(value);
}

void set_header_shentsize(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_shentsize = htons(value);
}

void set_header_phentsize(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_phentsize = htons(value);
}

void set_header_phnum(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_phnum = htons(value);
}

void set_header_ehsize(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_ehsize = htons(value);
}

void set_header_shnum(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_shnum = htons(value);
}

void set_header_shstrndx(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_shstrndx = htons(value);
}

void set_header_type(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_type = htons(value);
}

void set_header_machine(Elf32_Ehdr *header, Elf32_Half value) {
    header->e_machine = htons(value);
}

void set_header_version(Elf32_Ehdr *header, Elf32_Word value) {
    header->e_version = htonl(value);
}

void set_header_flag(Elf32_Ehdr *header, Elf32_Word value) {
    header->e_flags = htonl(value);
}

void set_header_entry(Elf32_Ehdr *header, Elf32_Addr value) {
    header->e_entry = htonl(value);
}


void set_section_name(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_name = htonl(value);
}

void set_section_type(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_type = htonl(value);
}

void set_section_offset(Elf32_Shdr *shdr, Elf32_Off value) {
    shdr->sh_offset = htonl(value);
}

void set_section_size(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_size = htonl(value);
}

void set_section_addr(Elf32_Shdr *shdr, Elf32_Addr value) {
    shdr->sh_addr = htonl(value);
}

void set_section_addralign(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_addralign = htonl(value);
}

void set_section_flags(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_flags = htonl(value);
}

void set_section_link(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_link = htonl(value);
}

void set_section_info(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_info = htonl(value);
}

void set_section_entsize(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_entsize = htonl(value);
}

void set_section_sh_name(Elf32_Shdr *shdr, Elf32_Word value) {
    shdr->sh_name = htonl(value);
}

typedef struct {
    Elf32_Sym *symbols; // Tableau dynamique de symboles
    size_t size; // Taille du tableau
} Symbol;

void setsize(Symbol *s,size_t size){
    (s->size)=(size);
}

void setsymstindx(Elf32_Sym *symbol, Elf32_Word stindx) {
    symbol->st_shndx = htons(stindx);
}

// Ajout de la gestion de l'adresse du symbole
void setsymvalue(Elf32_Sym *symbol, Elf32_Addr value) {
    symbol->st_value = htonl(value);
}

void update_symbol_addresses(Symbol *sym, Elf32_Shdr *sections, size_t num_sections, char *shstrtab) {
    for (size_t i = 0; i < sym->size; i++) {
        Elf32_Sym *symbol = &sym->symbols[i];
        Elf32_Half section_index = get_symbol_shndx(symbol);

        // Vérifier si l'indice de section est valide
        if (section_index == SHN_UNDEF || section_index == SHN_ABS || section_index >= num_sections) {
            continue;  // Ignorer les symboles non définis ou absolus
        }

        // Obtenir le nom et les drapeaux de la section
        const char *section_name = get_section_name(section_index, sections, shstrtab);
        uint32_t section_flags = get_section_flags(&sections[section_index]);

        // Vérifier si la section est allouable et correspond à .text, .data ou .bss
        if ((section_flags & SHF_ALLOC) &&
            (strcmp(section_name, ".text") == 0 || 
             strcmp(section_name, ".data") == 0 || 
             strcmp(section_name, ".bss") == 0)) {
            
            // Récupérer l'adresse de la section
            Elf32_Addr section_address = get_section_addr(&sections[section_index]);

            // Mettre à jour la valeur du symbole (st_value)
            if (get_symbol_value(symbol) == 0) {
                // Si le symbole n'a pas de valeur, attribuer l'adresse de la section
                setsymvalue(symbol, section_address);
            } else {
                // Ajouter l'adresse de la section à la valeur existante
                setsymvalue(symbol, get_symbol_value(symbol) + section_address);
            }
        }
    }
}


void apply_relocation(Elf32_Rel *rel, Elf32_Sym *symbols, char *target_data, Elf32_Addr addresse) {

    uint32_t type = ELF32_R_TYPE(ntohl(rel->r_info));
    uint32_t sym_index = ELF32_R_SYM(ntohl(rel->r_info));
    Elf32_Sym *symbol = &symbols[sym_index];
    uint32_t symbol_value = get_symbol_value(symbol);
    uint32_t *location = (uint32_t *)(target_data + ntohl(rel->r_offset));

    uint32_t instruction = 0;
    uint32_t place = 0;
    int32_t displacement = 0;

    switch (type) {
        case R_ARM_ABS32:
            *location = htonl(symbol_value);
            break;

        case R_ARM_ABS16:
            *(uint16_t *)location = htons((uint16_t)symbol_value);
            break;

        case R_ARM_ABS8:
            *(uint8_t *)location = (uint8_t)((uint8_t)symbol_value);
            break;

           
        case R_ARM_JUMP24:
            instruction = ntohl(*location);
            place = addresse + ntohl(rel->r_offset);

            if (symbol_value & 0x3) {
                fprintf(stderr, "Error: Target address not aligned\n");
                return;
            }
           
            displacement = ((int32_t)symbol_value - (int32_t)place - 8);
           
            if (displacement < -33554432 || displacement > 33554428) {
                fprintf(stderr, "Error: Branch target out of range\n");
                return;
            }
           
            instruction &= 0xFF000000;  // Preserve condition
            instruction |= 0x00FFFFFF & (displacement >> 2); // Sign-extended displacement
            *location = htonl(instruction);
            break;
           
        case R_ARM_CALL:
            instruction = ntohl(*location);
            place = addresse + ntohl(rel->r_offset);
            displacement = ((int32_t)symbol_value - (int32_t)place - 8);

            if (displacement & 0x3) {
                fprintf(stderr, "Error: Target address not aligned\n");
                return;
            }

            displacement=displacement >> 2;
           
            if (displacement < -0x800000 || displacement > 0x7FFFFF) {
                fprintf(stderr, "Error: Branch target out of range\n");
                return;
            }

            instruction = 0xEB000000 | (displacement & 0x00FFFFFF);
            *location = htonl(instruction);
            break;
           
        default:
            fprintf(stderr, "Type de relocation non pris en charge : %u\n", type);
    }
}



void process_and_remove_relocation_sections(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *shdrs, char *shstrtab, const char *output_path) {
    size_t old_shnum = get_header_shnum(header);
    int new_shnum = 0;
    int *section_mapping = malloc(old_shnum * sizeof(int));

    if (!section_mapping) {
        perror("Erreur d'allocation mémoire pour le mapping des sections");
        exit(EXIT_FAILURE);
    }

    // Premier passage : Créer le mapping des sections
    for (size_t i = 0; i < old_shnum; i++) {
        if (get_section_type(&shdrs[i]) == SHT_REL) {
            section_mapping[i] = -1;
        } else {
            section_mapping[i] = new_shnum++;
        }
    }

    // Lecture de la table des symboles
    Symbol sym = { NULL, 0 };
    for (int i = 0; i < old_shnum; i++) {
        if (get_section_type(&shdrs[i]) == SHT_SYMTAB || get_section_type(&shdrs[i]) == SHT_DYNSYM) {
            Elf32_Shdr *symtab_hdr = &shdrs[i];
            Elf32_Word symtab_size = get_section_size(symtab_hdr);
            Elf32_Word symtab_offset = get_section_offset(symtab_hdr);
            size_t num_symbols = symtab_size / get_section_entsize(symtab_hdr);

            sym.symbols = malloc(symtab_size);
            setsize(&sym, num_symbols);

            if (!sym.symbols) {
                perror("Erreur lors de l'allocation mémoire pour les symboles");
                free(section_mapping);
                exit(EXIT_FAILURE);
            }

            fseek(file, symtab_offset, SEEK_SET);
            if (fread(sym.symbols, 1, symtab_size, file) != symtab_size) {
                perror("Erreur lors de la lecture de la table des symboles");
                free(sym.symbols);
                free(section_mapping);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Création de la nouvelle table des chaînes
    size_t new_shstrtab_size = 0;
    char *new_shstrtab = malloc(get_section_size(&shdrs[get_header_shstrndx(header)]));
    if (!new_shstrtab) {
        perror("Erreur d'allocation mémoire pour new_shstrtab");
        free(section_mapping);
        if (sym.symbols) free(sym.symbols);
        exit(EXIT_FAILURE);
    }
    new_shstrtab[0] = '\0';
    new_shstrtab_size = 1;

    // Allouer l'espace pour les nouvelles sections
    Elf32_Shdr *new_shdrs = malloc(new_shnum * sizeof(Elf32_Shdr));
    if (!new_shdrs) {
        free(section_mapping);
        free(new_shstrtab);
        if (sym.symbols) free(sym.symbols);
        exit(EXIT_FAILURE);
    }

    // Deuxième passage : Copier et ajuster les sections
    Elf32_Off current_offset = sizeof(Elf32_Ehdr);
    int new_index = 0;

    // Mise à jour des index de sections dans la table des symboles
    for (size_t i = 0; i < sym.size; i++) {
        Elf32_Half old_shndx = get_symbol_shndx(&sym.symbols[i]);
        if (old_shndx < old_shnum && section_mapping[old_shndx] != -1) {
            setsymstindx(&sym.symbols[i], section_mapping[old_shndx]);
        } else if (old_shndx >= SHN_LORESERVE) {
            // Garder les valeurs spéciales intactes
            continue;
        } else {
            setsymstindx(&sym.symbols[i], SHN_UNDEF);
        }
    }

    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        goto cleanup;
    }

    // Écrire l'en-tête ELF initial
    if (fwrite(header, sizeof(Elf32_Ehdr), 1, output_file) != 1) {
        perror("Erreur lors de l'écriture de l'en-tête");
        goto cleanup;
    }
    

    // Première passe : calculer la taille finale de la table des chaînes
    size_t final_shstrtab_size = 1; // Pour le '\0' initial
    for (size_t i = 0; i < old_shnum; i++) {
        if (section_mapping[i] != -1) {
            if (get_sh_name(&shdrs[i]) != 0) {
                const char *old_name = &shstrtab[get_sh_name(&shdrs[i])];
                final_shstrtab_size += strlen(old_name) + 1;
            }
        }
    }

    // Copier les sections non-REL
    current_offset = sizeof(Elf32_Ehdr);

    // Attribuer des adresses aux nouvelles sections
    uint32_t current_address = 0x58; // Adresse initiale pour les sections

    for (size_t i = 0; i < old_shnum; i++) {
        if (section_mapping[i] != -1) {
            new_shdrs[new_index] = shdrs[i];

            // Récupérer l'alignement requis pour cette section
            Elf32_Word alignment = get_section_addralign(&new_shdrs[new_index]);
            if (alignment > 0) {
                // Aligner l'adresse courante sur le multiple requis
                current_address = (current_address + alignment - 1) & ~(alignment - 1);
            }
            // Mettre à jour l'adresse virtuelle de la section
            new_shdrs[new_index].sh_addr = htonl(current_address);
            // Incrémenter l'adresse courante en tenant compte de la taille de la section
            if (get_section_size(&new_shdrs[new_index]) == 0) {
                current_address += (alignment > 0) ? alignment : 1; // Incrément basé sur l'alignement ou une valeur minimale
            } else {
                // Incrémenter l'adresse courante en tenant compte de la taille de la section
                current_address += get_section_size(&new_shdrs[new_index]);
            }

            set_section_offset(&new_shdrs[new_index], current_offset);

            // Gestion des noms de sections
            if (get_sh_name(&new_shdrs[new_index]) != 0) {
                const char *old_name = &shstrtab[get_sh_name(&new_shdrs[new_index])];
                size_t name_len = strlen(old_name) + 1;
                memcpy(&new_shstrtab[new_shstrtab_size], old_name, name_len);
                set_section_sh_name(&new_shdrs[new_index], new_shstrtab_size);
                new_shstrtab_size += name_len;
            }

            // Mise à jour des liens entre sections
            if (get_section_link(&new_shdrs[new_index]) < old_shnum) {
                int new_link = section_mapping[get_section_link(&new_shdrs[new_index])];
                set_section_link(&new_shdrs[new_index], new_link >= 0 ? new_link : 0);
            }

            // Copie du contenu de la section
            Elf32_Word section_size = get_section_size(&shdrs[i]);
            if (section_size > 0) {
                char *section_data = malloc(section_size);
                if (!section_data) {
                    perror("Erreur d'allocation mémoire pour les données de section");
                    goto cleanup;
                }
                fseek(file, get_section_offset(&shdrs[i]), SEEK_SET);
                if (get_section_type(&shdrs[i]) == SHT_SYMTAB || 
                    get_section_type(&shdrs[i]) == SHT_DYNSYM) {
                    update_symbol_addresses(&sym, new_shdrs, new_shnum, new_shstrtab);
                    if (fwrite(sym.symbols, 1, section_size, output_file) != section_size) {
                        free(section_data);
                        goto cleanup;
                    }
                    current_offset += section_size;
                } else if (get_section_type(&shdrs[i]) == SHT_STRTAB &&
                        i == get_header_shstrndx(header)) {
                    // Écrire la nouvelle table des chaînes
                    if (fwrite(new_shstrtab, 1, new_shstrtab_size, output_file) != new_shstrtab_size) {
                        free(section_data);
                        goto cleanup;
                    }
                    set_section_size(&new_shdrs[new_index], final_shstrtab_size);
                    current_offset += final_shstrtab_size;  // Utiliser la taille finale
                } else {
                    // Copier les autres sections normalement
                    if (fread(section_data, 1, section_size, file) != section_size ||
                        fwrite(section_data, 1, section_size, output_file) != section_size) {
                        free(section_data);
                        goto cleanup;
                    }
                    current_offset += section_size;
                }
                free(section_data);
            }
            new_index++;
        }
    }

    for (int i = 0; i < old_shnum; i++) {
        if (ntohl(shdrs[i].sh_type) == SHT_REL) {
            Elf32_Shdr *rel_section = &shdrs[i];
            Elf32_Shdr *target_section = &shdrs[get_section_info(rel_section)];
            

            // Charger les entrées de réimplantation
            Elf32_Rel *rel_entries = malloc(get_section_size(rel_section));
            fseek(file, get_section_offset(rel_section), SEEK_SET);
            if(fread(rel_entries, get_section_size(rel_section), 1, file) != 1){
                perror("Erreur lors de la lecture des entrées de réimplantation.");
                goto cleanup;
            }

            // Charger les données de la section cible
            char *target_data = malloc(get_section_size(target_section));
            Elf32_Addr addresse = get_section_addr(&new_shdrs[section_mapping[get_section_info(rel_section)]]);

            fseek(file, get_section_offset(target_section), SEEK_SET);
            if(fread(target_data, get_section_size(target_section), 1, file) != 1){
                perror("Erreur lors de la lecture des entrées de réimplantation.");
                goto cleanup;
            }
    
            // Appliquer chaque réimplantation
            size_t num_relocations = get_section_size(rel_section) / sizeof(Elf32_Rel);
            for (size_t j = 0; j < num_relocations; j++) {
                apply_relocation(&rel_entries[j], sym.symbols, target_data, addresse);
            }

            // Obtenir l'offset mis à jour pour la section cible
            Elf32_Off new_offset = get_section_offset(&new_shdrs[section_mapping[get_section_info(rel_section)]]);

            // Déplacer à l'offset correct dans le fichier
            if (fseek(output_file, new_offset, SEEK_SET) != 0) {
                perror("Erreur lors du déplacement à l'offset calculé");
                goto cleanup;
            }
            // Écrire les données modifiées
            if (fwrite(target_data, 1, get_section_size(target_section), output_file) != get_section_size(target_section)) {
                perror("Erreur lors de l'écriture des données modifiées");
                goto cleanup;
            }

            // Libérer la mémoire
            free(rel_entries);
            free(target_data);
        }
    }

    // Mise à jour finale de l'en-tête ELF
    uint32_t entry = 0;
    for (int i = 0; i < new_shnum; i++) {
        if (strcmp(get_section_name(i, new_shdrs, new_shstrtab), ".text") == 0) {
            entry =  get_section_addr(&new_shdrs[i]);
        }
    }

    set_header_entry(header, entry);
    set_header_shnum(header, new_shnum);
    set_header_shoff(header, current_offset);
    set_header_shstrndx(header, section_mapping[get_header_shstrndx(header)]);


    fseek(output_file, 0, SEEK_END);

    // Écrire la table des sections mise à jour
    for (int i = 0; i < new_shnum; i++) {
        if (fwrite(&new_shdrs[i], sizeof(Elf32_Shdr), 1, output_file) != 1) {
            perror("Erreur lors de l'écriture de la table des sections");
            goto cleanup;
        }
    }

    // Réécrire l'en-tête avec les valeurs mises à jour
    rewind(output_file);
    if (fwrite(header, sizeof(Elf32_Ehdr), 1, output_file) != 1) {
        perror("Erreur lors de la mise à jour de l'en-tête");
        goto cleanup;
    }

    cleanup:
        if (output_file) fclose(output_file);
        free(new_shdrs);
        free(section_mapping);
        free(new_shstrtab);
        if (sym.symbols) free(sym.symbols);
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_elf_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_path = argv[1];

    // Créer un nom de fichier de sortie automatiquement
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s_modified", input_path); // Ajoute "_modified" au nom du fichier d'entrée

    FILE *file = fopen(input_path, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier ELF");
        exit(EXIT_FAILURE);
    }

    // Lire l'en-tête ELF
    Elf32_Ehdr header;
    if(fread(&header, sizeof(Elf32_Ehdr), 1, file) != 1){
        perror("Erreur lors de la lecture de l'entête ELF.");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Lire la table des sections
    Elf32_Shdr *shdrs = get_sections(file, &header);
    char *shstrtab = get_shstrtab(file, &header, shdrs);

    // Supprimer les sections REL et écrire dans le fichier de sortie
    process_and_remove_relocation_sections(file, &header, shdrs, shstrtab, output_path);

    // Libération des ressources
    free(shdrs);
    free(shstrtab);
    fclose(file);

    printf("Le fichier modifié a été écrit dans: %s\n", output_path);

    return 0;
}

