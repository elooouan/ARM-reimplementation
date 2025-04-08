#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "elf_section.h"
#include "elf_header.h"
#include "elf_section_contenu.h"
#include "elf_relocation.h"

const char *get_relocation_type(uint32_t r_type) {
    switch (r_type) {
        case R_ARM_CALL:     return "R_ARM_CALL";
        case R_ARM_ABS32:    return "R_ARM_ABS32";
        case R_ARM_ABS16:    return "R_ARM_ABS16";
        case R_ARM_ABS8:     return "R_ARM_ABS8";
        case R_ARM_JUMP24:   return "R_ARM_JUMP24";
        default:             return "Unknown";
    }
}

void print_relocation_info(FILE *fp) {
    // Read ELF header
    Elf32_Ehdr  ehdr = *(get_header(fp));

    if (!valider_elf(&ehdr)) {
        exit(EXIT_FAILURE);
    }

    Elf32_Shdr *shdrs = get_sections(fp, &ehdr);
    char *shstrtab = get_shstrtab(fp, &ehdr, shdrs);
    
    // Find and process relocation sections
    for (int i = 0; i < get_section_e_shnum(&ehdr); i++) {
        Elf32_Word sh_type = get_section_type(&shdrs[i]);

        if (sh_type == SHT_REL) {
            // Get section name
            const char *section_name = get_section_name(i, shdrs, shstrtab);

            // Get section attributes using utility functions
            Elf32_Off sh_offset = get_section_offset(&shdrs[i]);
            Elf32_Word sh_size = get_section_size(&shdrs[i]);
            Elf32_Word sh_entsize =get_section_entsize(&shdrs[i]);

            // Display section information
            printf("Relocation section '%s' at offset 0x%x contains %d entries:\n",
                   section_name,
                   sh_offset,
                   sh_size / sh_entsize);

            printf(" Offset      Info        Type             Index\n");

            // Allocate memory for relocation entries
            Elf32_Rel *rels = malloc(sh_size);
            if (!rels) {
                perror("Error allocating memory for relocation entries");
                continue;
            }

            // Read the relocation entries
            fseek(fp, sh_offset, SEEK_SET);
            if (fread(rels, sh_size, 1, fp) != 1) {
                perror("Error reading relocation entries");
                free(rels);
                continue;
            }

            // Process relocation entries
            for (int j = 0; j < sh_size / sh_entsize; j++) {
                uint32_t r_offset = ntohl(rels[j].r_offset);
                uint32_t r_info = ntohl(rels[j].r_info);
                uint32_t r_type = ELF32_R_TYPE(r_info);

                // Extract symbol index from r_info
                uint32_t sym_index = ELF32_R_SYM(r_info);

                // Display the relocation information with symbol index
                printf("%08x    %08x    %-16s   %08x\n",
                       r_offset,                // Offset
                       r_info,                 // Info
                       get_relocation_type(r_type), // Type (as string)
                       sym_index);             // Symbol Index
            }

            free(rels);
        }
    }

    // Free remaining memory
    free(shdrs);
    if (shstrtab) {
        free(shstrtab);
    }
    fclose(fp);
}