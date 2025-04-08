#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "elf_header.h"
#include "elf_section.h"
#include "arm_simulator_interface.h"

void load_elf_to_simulator(const char *filename, char *hostname, char *servicename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open ELF file");
        exit(EXIT_FAILURE);
    }

    Elf32_Ehdr *ehdr = get_header(file);
    if (!valider_elf(ehdr)) {
        fprintf(stderr, "Invalid ELF file.\n");
        free(ehdr);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    Elf32_Shdr *shdrs = get_sections(file, ehdr);
    char *shstrtab = get_shstrtab(file, ehdr, shdrs);

    arm_simulator_data_t simulator = arm_connect(hostname, servicename);

    for (int i = 0; i < get_header_shnum(ehdr); i++) {
        if (ntohl(shdrs[i].sh_flags) & SHF_ALLOC) {
            
            uint32_t section_size = get_section_size(&shdrs[i]);
            uint32_t section_addr = get_section_addr(&shdrs[i]);
            uint32_t *section_data = malloc(section_size);

            fseek(file, get_section_offset(&shdrs[i]), SEEK_SET);
            if(fread(section_data, 1, section_size, file) != section_size){
                perror("Failed to read section data.");
                free(section_data);
                exit(EXIT_FAILURE);
            }

            arm_write_memory(simulator, section_addr, section_data, section_size);

            free(section_data);
        }
    }

    arm_write_register(simulator, 15, get_header_entry(ehdr));

    arm_run(simulator);

    free(ehdr);
    free(shdrs);
    free(shstrtab);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <ELF file> <hostname> <servicename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    load_elf_to_simulator(argv[1], argv[2], argv[3]);
    return EXIT_SUCCESS;
}
