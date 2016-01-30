#include <stdint.h>

#include "multiboot.h"

#define MMU_MIN_ADDRESS 0x000007FF
#define MMU_MAX_ADDRESS 0xFFFFFFFF

void mmu_init(mmap_entry* mmap_addr, uint32_t mmap_length, void* kernel_start,
    void* kernel_end, void* stack_start, void* stack_end);
void* mmu_alloc(size_t size);
void* mmu_realloc(void* ptr, size_t size);
void mmu_free(void* ptr);

void* mmu_last_address();
void mmu_dump();
