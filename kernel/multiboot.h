#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#include "elf.h"

#define MMAP_TYPE_AVAILABLE 0x00000001

#define DRIVE_MODE_CHS 0x00000000
#define DRIVE_MODE_LBA 0x00000001

#define MBI_FLAG_MEM               0x00000001 
#define MBI_FLAG_BOOT_DEVICE       0x00000002
#define MBI_FLAG_CMDLINE           0x00000004
#define MBI_FLAG_MODS              0x00000008
#define MBI_FLAG_SYMS_AOUT         0x00000010
#define MBI_FLAG_SYMS_ELF          0x00000020
#define MBI_FLAG_MMAP              0x00000040
#define MBI_FLAG_DRIVES            0x00000080
#define MBI_FLAG_CONFIG_TABLE      0x00000100
#define MBI_FLAG_BOOT_LOADER_NAME  0x00000200
#define MBI_FLAG_APM_TABLE         0x00000400
#define MBI_FLAG_VBE               0x00000800

typedef struct _mod_entry_t {
  void* mod_start;
  void* mod_end;
  char* string;
  uint32_t reserved;
} mod_entry_t;

typedef struct _aout_t {
  uint32_t tabsize;
  uint32_t strsize;
  void* addr;
  uint32_t reserved;
} aout_t;

typedef struct _elf_t {
  uint32_t num;
  uint32_t size;
  elf32_shdr_t* addr;
  uint32_t shndx;
} elf_t;

typedef union _syms_data_t {
  aout_t aout;
  elf_t elf;
} syms_data_t;

typedef struct _mmap_entry_t {
  uint32_t entry_size;
  uint64_t base_addr;
  uint64_t length;
  uint32_t type;
} mmap_entry_t;

typedef struct _drive_entry_t {
  uint32_t entry_size;
  uint32_t drive_number;
  uint32_t drive_mode;
  uint32_t drive_cylinders;
  uint32_t drive_heads;
  uint32_t drive_sectors;
  uint16_t drive_ports[];
} drive_entry_t;

typedef struct _apm_data_t {
  uint16_t version;
  uint16_t cseg;
  uint32_t offset;
  uint16_t cseg_16;
  uint16_t dseg;
  uint16_t flags;
  uint16_t cseg_len;
  uint16_t cseg_16_len;
  uint16_t dseg_len;
} apm_data_t;

typedef struct _multiboot_info_t {
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  char* cmdline;
  uint32_t mods_count;
  mod_entry_t* mods_addr;
  syms_data_t syms;
  uint32_t mmap_length;
  mmap_entry_t* mmap_addr;
  uint32_t drives_length;
  drive_entry_t* drives_addr;
  void* config_table;
  char* boot_loader_name;
  apm_data_t* apm_table;
  void* vbe_control_info;
  void* vbe_mode_info;
  uint32_t vbe_mode;
  uint32_t vbe_interface_seg;
  uint32_t vbe_interface_off;
  uint32_t vbe_interface_len;
} multiboot_info_t;

#endif

/* [multiboot_info.config_table] http://www.ousob.com/ng/rbrown/ng4516c.php */
/* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format */
