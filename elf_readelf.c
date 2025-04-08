#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf_header.h"
#include "elf_section.h"
#include "elf_section_contenu.h"
#include "elf_relocation.h"
#include "elf_table_symbole.h"

void print_help(const char *prog_name) {
    printf("Usage: %s [options] <ELF file>\n", prog_name);
    printf("Options:\n");
    printf("  -h        Display ELF header\n");
    printf("  -S        Display section headers\n");
    printf("  -s        Display symbol table\n");
    printf("  -r        Display relocation sections\n");
    printf("  -x        <number|name>  Dump the contents of section <number|name> as bytes\n");
    printf("  -o        Display this help message\n");
}

int main(int argc, char *argv[]) {
    int option;

    FILE *file;

    while ((option = getopt(argc, argv, "h:S:s:x:r:o")) != -1) {
        switch (option) {
            case 'h':
                file = fopen(optarg, "rb");
                if(file){
                    afficher_entete_elf(file); 
                    fclose(file);
                }
                break;

            case 'S':
                file = fopen(optarg, "rb");
                if(file){
                    afficher_section_headers(file); 
                }
                break;

            case 's':
                file = fopen(optarg, "rb");
                if(file){
                    afficher_table_symboles(file); 
                    fclose(file);
                }
                break;

            case 'x':
                if (argc < 4) {
                    fprintf(stderr, "Error: Option -x requires two arguments\n");
                    exit(1);
                }
                read_section_content(argv[2], argv[3]);
                break;

            case 'r':
                file = fopen(optarg, "rb");
                if(file){
                    print_relocation_info(file); 
                }
                break;

            case 'o':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
                break;
                
            default:
                print_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // No options were provided
    if (optind == 1) {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}