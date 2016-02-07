#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#define MMU_MIN_ADDRESS 0x000007FF
#define MMU_MAX_ADDRESS 0xFFFFFFFF

typedef struct _range_t {
  void* start;
  void* end;
} range_t;

void mmu_init(mmap_entry_t* mmap_addr, uint32_t mmap_length,
    range_t* lock_ranges, size_t lock_ranges_count);
void mmu_reclaim(void* start, void* end);

void* mmu_alloc(size_t size);
void* mmu_realloc(void* ptr, size_t size);
void mmu_free(void* ptr);

uint32_t mmu_available();
void* mmu_last_address();
void mmu_dump();
