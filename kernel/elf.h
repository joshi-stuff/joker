#ifndef _ELF_H
#define _ELF_H

#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4
#define SHT_HASH     5
#define SHT_DYNAMIC  6
#define SHT_NOTE     7
#define SHT_NOBITS   8
#define SHT_REL      9
#define SHT_SHLIB    10
#define SHT_DYNSYM   11
#define SHT_LOPROC   0x70000000
#define SHT_HIPROC   0x7FFFFFFF
#define SHT_LOUSER   0x80000000
#define SHT_HIUSER   0xFFFFFFFF

#define SHF_WRITE     1
#define SHF_ALLOC     2
#define SHF_EXECINSTR 4
#define SHF_MASKPROC  0xF0000000

#define STB_LOCAL     0
#define STB_GLOBAL    1
#define STB_WEAK      2
#define STB_LOPROC    13
#define STB_HIPROC    15

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_LOPROC  13
#define STT_HIPROC  15

typedef struct _elf32_shdr_t {
  uint32_t sh_name;
  uint32_t sh_type;
  uint32_t sh_flags;
  void* sh_addr;
  uint32_t sf_offset;
  uint32_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint32_t sh_addralign;
  uint32_t sh_entsize;
} elf32_shdr_t;

typedef struct _elf32_sym_t {
  uint32_t st_name;
  void* st_value;
  uint32_t st_size;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
} elf32_sym_t;

#define elf_get_entry(entry_size, entries, index) \
          ((elf32_shdr_t*)(((uint8_t*)(entries))+(index)*(entry_size)))
#define elf_get_string(strings_entry, index) \
          (((char*)(strings_entry)->sh_addr)+index)
#define elf_get_sym_entry(entry_size, entries, index) \
          ((elf32_sym_t*)(((uint8_t*)(entries))+(index)*(entry_size)))
#define elf_get_sym_entry_bind(sym_entry) \
          (((sym_entry)->st_info)>>4)
#define elf_get_sym_entry_type(sym_entry) \
          (((sym_entry)->st_info)&0x0F)

#endif

/* http://www.skyfree.org/linux/references/ELF_Format.pdf */
