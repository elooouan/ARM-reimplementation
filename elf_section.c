#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "elf_header.h"
#include "elf_section.h"

const char *get_section_name(int section_index, Elf32_Shdr *shdrs, char *shstrtab) {
    if (shstrtab == NULL) {
        return "Nom de section non disponible";
    }
    Elf32_Word name_offset = ntohl(shdrs[section_index].sh_name);
    return &shstrtab[name_offset];
}

Elf32_Word get_section_type(Elf32_Shdr *Section_header){
    return ntohl(Section_header->sh_type);
}

Elf32_Off get_section_offset(Elf32_Shdr *shdr){
    return ntohl(shdr->sh_offset);
}

Elf32_Word get_section_size(Elf32_Shdr *shdr){
    return ntohl(shdr->sh_size);
}

Elf32_Addr get_section_addr(Elf32_Shdr *shdr){
    return ntohl(shdr->sh_addr);
}

Elf32_Half get_section_e_shnum(Elf32_Ehdr *shdr){
    return ntohs(shdr->e_shnum);
}

Elf32_Word get_section_addralign(Elf32_Shdr *section) {
    return ntohl(section->sh_addralign);
}

Elf32_Word get_section_flags(Elf32_Shdr *section) {
    return ntohl(section->sh_flags);
}

Elf32_Word get_section_link(Elf32_Shdr *section){
    return ntohl(section->sh_link);
}

Elf32_Word get_section_info(Elf32_Shdr *section){
    return ntohl(section->sh_info);
}

Elf32_Word get_section_entsize(Elf32_Shdr *section){
    return ntohl(section->sh_entsize);
}

const char *get_section_type_name(Elf32_Word sh_type) {
    switch (sh_type) {
        case SHT_NULL:           return "NULL";
        case SHT_PROGBITS:       return "PROGBITS";
        case SHT_SYMTAB:         return "SYMTAB";
        case SHT_STRTAB:         return "STRTAB";
        case SHT_RELA:           return "RELA";
        case SHT_HASH:           return "HASH";
        case SHT_DYNAMIC:        return "DYNAMIC";
        case SHT_NOTE:           return "NOTE";
        case SHT_NOBITS:         return "NOBITS";
        case SHT_REL:            return "REL";
        case SHT_SHLIB:          return "SHLIB";
        case SHT_DYNSYM:         return "DYNSYM";
        case SHT_INIT_ARRAY:     return "INIT_ARRAY";
        case SHT_FINI_ARRAY:     return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY:  return "PREINIT_ARRAY";
        case SHT_GROUP:          return "GROUP";
        case SHT_SYMTAB_SHNDX:   return "SYMTAB_SHNDX";
        case SHT_NUM:            return "NUM";
        case SHT_LOOS:           return "LOOS";
        case SHT_GNU_ATTRIBUTES: return "GNU_ATTRIBUTES";
        case SHT_GNU_HASH:       return "GNU_HASH";
        case SHT_GNU_LIBLIST:    return "GNU_LIBLIST";
        case SHT_CHECKSUM:       return "CHECKSUM";
        case SHT_LOSUNW:         return "LOSUNW";
        case SHT_SUNW_COMDAT:    return "SUNW_COMDAT";
        case SHT_SUNW_syminfo:   return "SUNW_syminfo";
        case SHT_GNU_verdef:     return "GNU_verdef";
        case SHT_GNU_verneed:    return "GNU_verneed";
        case SHT_GNU_versym:     return "GNU_versym";
        case SHT_LOPROC:         return "LOPROC";
        case SHT_HIPROC:         return "HIPROC";
        case SHT_LOUSER:         return "LOUSER";
        case SHT_HIUSER:         return "HIUSER";
        default:                 return "UNKNOWN";
    }
}


Elf32_Shdr *get_sections(FILE *file, Elf32_Ehdr *header){
    Elf32_Shdr *shdr;

    if (valider_elf(header) != 1) {
        perror("Error reading ELF header");
        fclose(file);
        exit(1);
    }


    if (fseek(file, get_header_shoff(header), SEEK_SET) != 0) {
        perror("Error seeking to section header table");
        fclose(file);
        exit(1);
    }

    shdr = malloc(get_header_shentize(header) * get_header_shnum(header));
    if (!shdr) {
        perror("Error allocating memory for section headers");
        fclose(file);
        exit(1);
    }

    if (fread(shdr, get_header_shentize(header),get_header_shnum(header), file) != get_header_shnum(header)) {
        perror("Error reading section headers");
        free(shdr);
        fclose(file);
        exit(1);
    }

    return shdr;
}

char *get_shstrtab(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *shdrs){
    // Read section header string table
    char *shstrtab = NULL;
    if (get_header_shstrndx(header) != SHN_UNDEF) {
        Elf32_Shdr shstrtab_hdr = shdrs[get_header_shstrndx(header)];
        shstrtab = malloc(ntohl(shstrtab_hdr.sh_size));
        if (!shstrtab) {
            perror("Error allocating memory for section header string table");
            free(shdrs);
            fclose(file);
            exit(1);
        }


        if (fseek(file, (ntohl(shstrtab_hdr.sh_offset)), SEEK_SET) != 0 ||
            fread(shstrtab, (ntohl(shstrtab_hdr.sh_size)), 1, file) != 1) {
            perror("Error reading section header string table");
            free(shstrtab);
            free(shdrs);
            fclose(file);
            exit(1);
        }
    }

    return shstrtab;
}


void afficher_section_headers(FILE *file) {

    Elf32_Ehdr *header = get_header(file);
    Elf32_Shdr *shdrs = get_sections(file,header);
    char *shstrtab = get_shstrtab(file, header, shdrs);

    printf("There are %d section headers, starting at offset 0x%x:\n\n",
           get_header_shnum(header), get_header_shoff(header));


    printf("  [Nr] Name                 Type             Addr      Off      Size     ES   Flg       Lk   Inf   Al\n");
    for (int i = 0; i < get_header_shnum(header); i++) {
        const char *name = shstrtab ? get_section_name(i,shdrs,shstrtab) : "";
        
 // Construction des flags
        char flags[9] = "";  // Jusqu'à 8 flags + null terminator
        if (get_section_flags(&shdrs[i]) & SHF_WRITE) strncat(flags, "W", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_ALLOC) strncat(flags, "A", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_EXECINSTR) strncat(flags, "X", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_MERGE) strncat(flags, "M", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_STRINGS) strncat(flags, "S", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_INFO_LINK) strncat(flags, "I", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_LINK_ORDER) strncat(flags, "L", sizeof(flags) - strlen(flags) - 1);
        if (get_section_flags(&shdrs[i]) & SHF_OS_NONCONFORMING) strncat(flags, "O", sizeof(flags) - strlen(flags) - 1);

        // Affichage des informations des sections
        printf("  [%2d] %-18s   %-15s  %08x  %06x   %06x   %02x   %-8s %2d   %3d   %2d\n",
               i,                                             // Index
               name,                                          // Nom de la section
               get_section_type_name(get_section_type(&shdrs[i])), // Type de la section
               get_section_addr(&shdrs[i]),                    // Adresse de la section
               get_section_offset(&shdrs[i]),                  // Offset dans le fichier
               get_section_size(&shdrs[i]),                    // Taille de la section
               get_section_entsize(&shdrs[i]),                 // Entrée de la section (si applicable)
               flags,                                          // Flags construits
               get_section_link(&shdrs[i]),                    // Lien de la section
               get_section_info(&shdrs[i]),                    // Infos supplémentaires
               get_section_addralign(&shdrs[i]));              // Alignement
    }

    printf("Key to Flags: \nW (write), A (alloc), X (execute), M (merge), S (strings), I (info), \nL (link order), O (extra OS processing required), G (group), T (TLS), \nC (compressed), x (unknown), o (OS specific), E (exclude), \nD (mbind), y (purecode), p (processor specific)\n");

    // Clean up
    free(shstrtab);
    free(shdrs);
    fclose(file);
}