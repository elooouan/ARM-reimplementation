ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
AM_CFLAGS+=-D BIG_ENDIAN_SIMULATOR
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG

LDADD=-lpthread


SUBDIRS=
SUBDIRS+=. Examples_loader

bin_PROGRAMS=
bin_PROGRAMS+=ARM_runner_example
bin_PROGRAMS+=elf_readelf
bin_PROGRAMS+=process_rel
bin_PROGRAMS+=load_sim

COMMON=debug.h debug.c util.h util.c

EXTRA_DIST=filtre.pl
SIMULATOR_INTERFACE=csapp.h csapp.c scanner.h scanner.l gdb_protocol.h gdb_protocol.c arm_simulator_interface.h arm_simulator_interface.c

ARM_runner_example_SOURCES=$(COMMON) $(SIMULATOR_INTERFACE) ARM_runner_example.c

elf_readelf_SOURCES=$(COMMON) elf_header.h elf_section.h elf_section_contenu.h elf_readelf.c elf_relocation.c elf_header.c elf_section.c elf_section_contenu.c elf_relocation.h elf_table_symbole.c elf_table_symbole.h

process_rel_SOURCES=$(COMMON) process_rel.c elf_header.h elf_header.c elf_section.h elf_section.c elf_table_symbole.c elf_table_symbole.h elf_relocation.c elf_relocation.h

load_sim_SOURCES=$(COMMON) $(SIMULATOR_INTERFACE) load_sim.c elf_header.h elf_header.c elf_section.h elf_section.c 

EXTRA_DIST+=sample_run.sh

