#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "elf_table_symbole.h"

const char *get_symbol_type(unsigned char info) {
    switch (ELF32_ST_TYPE(info)) {
        case STT_NOTYPE:  return "NOTYPE";
        case STT_OBJECT:  return "OBJECT";
        case STT_FUNC:    return "FUNC";
        case STT_SECTION: return "SECTION";
        case STT_FILE:    return "FILE";
        case STT_LOPROC:  return "LOPROC";
        case STT_HIPROC:  return "HIPROC";
        default:          return "UNKNOWN";
    }
}

const char *get_symbol_binding(unsigned char info) {
    switch (ELF32_ST_BIND(info)) {
        case STB_LOCAL:     return "LOCAL";
        case STB_GLOBAL:    return "GLOBAL";
        case STB_WEAK:      return "WEAK";
        case STB_LOPROC:    return "LOPROC";
        case STB_HIPROC:    return "HIPROC";
        default:            return "UNKNOWN";
    }
}

const char *get_symbol_visibility(unsigned char other) {
    switch (ELF32_ST_VISIBILITY(other)) {
        case STV_DEFAULT:   return "DEFAULT";
        case STV_INTERNAL:  return "INTERNAL";
        case STV_HIDDEN:    return "HIDDEN";
        case STV_PROTECTED: return "PROTECTED";
        default:            return "UNKNOWN";
    }
}

Elf32_Word get_symbol_name(Elf32_Sym *symbol) {
    return ntohl(symbol->st_name);
}

Elf32_Addr get_symbol_value(Elf32_Sym *symbol) {
    return ntohl(symbol->st_value);
}

Elf32_Word get_symbol_size(Elf32_Sym *symbol) {
    return ntohl(symbol->st_size);
}

Elf32_Half get_symbol_shndx(Elf32_Sym *symbol) {
    return ntohs(symbol->st_shndx);
}

void afficher_table_symboles(FILE *file) {
    Elf32_Ehdr *ehdr = get_header(file);
    Elf32_Shdr *shdrs = get_sections(file, ehdr);
    char *shstrtab = get_shstrtab(file, ehdr, shdrs);

    for (int i = 0; i < get_header_shnum(ehdr); i++) {
        if (get_section_type(&shdrs[i]) == SHT_SYMTAB || get_section_type(&shdrs[i]) == SHT_DYNSYM) {
            printf("\nTable des symboles dans la section '%s':\n", get_section_name(i, shdrs, shstrtab));

            Elf32_Shdr symtab_hdr = shdrs[i];
            Elf32_Shdr strtab_hdr = shdrs[get_section_link(&symtab_hdr)];

            fseek(file, get_section_offset(&symtab_hdr), SEEK_SET);
            Elf32_Sym *symbols = malloc(get_section_size(&symtab_hdr));

            if (fread(symbols, 1, get_section_size(&symtab_hdr), file) != get_section_size(&symtab_hdr)) {
                perror("Erreur lors de la lecture des symboles");
                free(symbols);
                return;
            }

            fseek(file, get_section_offset(&strtab_hdr), SEEK_SET);
            char *strtab = malloc(get_section_size(&strtab_hdr));
            if (fread(strtab, 1, get_section_size(&strtab_hdr), file) != get_section_size(&strtab_hdr)) {
                perror("Erreur lors de la lecture de la table des chaînes");
                free(symbols);
                free(strtab);
                return;
            }

            printf("  Num:    Valeur   Taille Type    Lien   Vis        Ndx   Nom\n");
            printf("  ----   --------  ------ ------- ------ --------   ---   ---\n");

            int symbol_count = get_section_size(&symtab_hdr) / get_section_entsize(&symtab_hdr);
            for (int j = 0; j < symbol_count; j++) {
                Elf32_Sym sym = symbols[j];
                Elf32_Word name_index = get_symbol_name(&sym);
                Elf32_Half shndx = get_symbol_shndx(&sym);
                const char *symbol_name = "";

                if (ELF32_ST_TYPE(sym.st_info) == STT_SECTION && shndx < get_header_shnum(ehdr)) {
                    // Si le type est SECTION, afficher le nom depuis .shstrtab
                    symbol_name = get_section_name(shndx, shdrs, shstrtab);
                } else if (name_index < get_section_size(&strtab_hdr)) {
                    // Sinon, afficher le nom depuis .strtab
                    symbol_name = &strtab[name_index];
                } else {
                    symbol_name = "<invalid>";
                }

                printf("  %-5d  %08x  %-6u %-7s %-6s %-10s %-6d %-4s\n",
                       j,
                       get_symbol_value(&sym),
                       get_symbol_size(&sym),
                       get_symbol_type(sym.st_info),
                       get_symbol_binding(sym.st_info),
                       get_symbol_visibility(sym.st_other),
                       shndx,
                       symbol_name);
            }

            free(symbols);
            free(strtab);
        }
    }
}