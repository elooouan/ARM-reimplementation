#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <stdlib.h>
#include "elf_header.h"

Elf32_Off get_header_shoff(Elf32_Ehdr *header) {
    return ntohl(header->e_shoff);
}

Elf32_Off get_header_phoff(Elf32_Ehdr *header) {
    return ntohl(header->e_phoff);
}

Elf32_Half get_header_shentize(Elf32_Ehdr *header) {
    return ntohs(header->e_shentsize);
}

Elf32_Half get_header_phentsize(Elf32_Ehdr *header) {
    return ntohs(header->e_phentsize);
}

Elf32_Half get_header_phnum(Elf32_Ehdr *header) {
    return ntohs(header->e_phnum);
}

Elf32_Half get_header_ehsize(Elf32_Ehdr *header) {
    return ntohs(header->e_ehsize);
}

Elf32_Half get_header_shnum(Elf32_Ehdr *header) {
    return ntohs(header->e_shnum);
}

Elf32_Half get_header_shstrndx(Elf32_Ehdr *header) {
    return ntohs(header->e_shstrndx);
}

Elf32_Half get_header_type(Elf32_Ehdr *header) {
    return ntohs(header->e_type);
}

Elf32_Half get_header_machine(Elf32_Ehdr *header) {
    return ntohs(header->e_machine);
}

Elf32_Word get_header_version(Elf32_Ehdr *header) {
    return ntohl(header->e_version);
}

Elf32_Word get_header_flag(Elf32_Ehdr *header) {
    return ntohl(header->e_flags);
}

Elf32_Addr get_header_entry(Elf32_Ehdr *header) {
    return ntohl(header->e_entry);
}

unsigned char get_header_OSABI(Elf32_Ehdr *header) {
    return header->e_ident[EI_OSABI];
}

unsigned char get_header_ABIVersion(Elf32_Ehdr *header) {
    return header->e_ident[EI_ABIVERSION];
}

unsigned char get_header_CLASS(Elf32_Ehdr *header) {
    return header->e_ident[EI_CLASS];
}

unsigned char get_header_DATA(Elf32_Ehdr *header) {
    return header->e_ident[EI_DATA];
}

const char *get_header_type_name(Elf32_Ehdr *header) {
    switch(get_header_type(header)) {
        case ET_NONE:
            return("No file type");
        case ET_REL:
            return("REL (Relocatable file)");
        case ET_EXEC:
            return("EXE (Executable file)");
        case ET_DYN:
            return("SOF (Shared Object File)");
        case ET_CORE:
            return("CF (Core file)");
        case ET_LOPROC:
            return("Processor-specific (low)");
        case ET_HIPROC:
            return("Processor-specific (high)");
        default:
            return("Unknown type");
    }
}

const char *get_header_machine_name(Elf32_Ehdr *header) {
    switch (get_header_machine(header)) {
        case EM_NONE:
            return("No machine");
        case EM_ARM:
            return("ARM");
        case EM_M32:
            return("AT&T WE 32100");
        case EM_SPARC:
            return("SPARC");
        case EM_386:
            return("Intel 80386");
        case EM_68K:
            return("Motorola 68000");
        case EM_88K:
            return("Motorola 88000");
        case EM_860:
            return("Intel 80860");
        case EM_MIPS:
            return("MIPS RS3000 Big-Endian");
        case 10:
            return("MIPS RS4000 Big-Endian");
        case 11: case 12: case 13: case 14: case 15: case 16:
            return("Reserved for future use");
        default:
            return("Unknown machine");
    }
}

const char *get_header_OSABI_name(Elf32_Ehdr *header) {
    switch (get_header_OSABI(header)) {
        case ELFOSABI_NONE: 
            return("UNIX - System V");
        case ELFOSABI_HPUX: 
            return("HP-UX");
        case ELFOSABI_NETBSD: 
            return("NetBSD");
        case ELFOSABI_GNU: 
            return("GNU/Linux");
        case ELFOSABI_SOLARIS: 
            return("Sun Solaris");
        case ELFOSABI_AIX: 
            return("IBM AIX");
        case ELFOSABI_IRIX: 
            return("SGI Irix");
        case ELFOSABI_FREEBSD: 
            return("FreeBSD");
        case ELFOSABI_TRU64: 
            return("Compaq TRU64 UNIX");
        case ELFOSABI_MODESTO: 
            return("Novell Modesto");
        case ELFOSABI_OPENBSD: 
            return("OpenBSD");
        case ELFOSABI_ARM_AEABI: 
            return("ARM EABI");
        case ELFOSABI_ARM: 
            return("ARM");
        case ELFOSABI_STANDALONE: 
            return("Standalone (embedded) application");
        default: 
            return("Unknown OS/ABI");
    }
}

Elf32_Ehdr *get_header(FILE *elf_file) {
    Elf32_Ehdr *elf_header = malloc(sizeof(Elf32_Ehdr)); 
    if (!elf_header) {
        perror("Failed to allocate memory for ELF header");
        fclose(elf_file);
        return NULL;
    }

    if (fread(elf_header, 1, sizeof(Elf32_Ehdr), elf_file) != sizeof(Elf32_Ehdr)) {
        perror("Failed to read ELF header");
        free(elf_header);
        fclose(elf_file);
        return NULL;
    }
    return elf_header;
}

int valider_elf(Elf32_Ehdr *header) {
    if (header->e_ident[EI_MAG0] != ELFMAG0 ||
        header->e_ident[EI_MAG1] != ELFMAG1 ||
        header->e_ident[EI_MAG2] != ELFMAG2 ||
        header->e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr,"Le fichier n'est pas un fichier ELF valide.\n");
        return 0;
    }
    return 1;
}

// Fonction pour afficher les informations ELF
void afficher_entete_elf(FILE *file) {
    Elf32_Ehdr *header = get_header(file);
    if(valider_elf(header)) {
        fprintf(stdout, "ELF header:\n");

        fprintf(stdout,"Magic:   ");
        for (int i = 0; i < EI_NIDENT; i++) {
            fprintf(stdout,"%02x ", header->e_ident[i]);
        }
        fprintf(stdout,"\n");

        fprintf(stdout,"Class : %s\n", get_header_CLASS(header) == ELFCLASS32 ? "ELF32" : "ELF64");
        fprintf(stdout,"Data : %s\n", get_header_DATA(header) == ELFDATA2LSB ? "2's complement, little endian" : "2's complement, big endian");
        fprintf(stdout,"Version : %s\n", get_header_version(header) == EV_NONE ? "Invalid ELF version" : "1 (current)");
        fprintf(stdout,"OS/ABI: %s\n", get_header_OSABI_name(header));
        fprintf(stdout,"ABI Version: %d\n", get_header_ABIVersion(header));
        fprintf(stdout,"Type: %s\n", get_header_type_name(header));
        fprintf(stdout,"Machine: %s\n", get_header_machine_name(header));
        fprintf(stdout, "Version: 0x%x\n", get_header_version(header));
        fprintf(stdout,"Entry point address : 0x%x\n", get_header_entry(header));
        fprintf(stdout,"Start of program headers: %x (bytes into file)\n", get_header_phoff(header));
        fprintf(stdout,"Start of section headers: %u (bytes into file)\n", get_header_shoff(header));
        fprintf(stdout,"Flags: 0x%x, Version5 EABI\n", get_header_flag(header));
        fprintf(stdout,"Size of this header: %u (bytes)\n", get_header_ehsize(header));
        fprintf(stdout,"Size of program headers: %d (bytes)\n", get_header_phentsize(header));
        fprintf(stdout,"Number of program headers: %d\n", get_header_phnum(header));
        fprintf(stdout,"Size of section headers: %u (bytes)\n", get_header_shentize(header));
        fprintf(stdout,"Number of section headers: %u\n", get_header_shnum(header));
        fprintf(stdout,"Section header string table index: %u\n", get_header_shstrndx(header));
    }
}
