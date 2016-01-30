#include <stdio.h>

#include "kernel.h"
#include "mmu.h"

#define _ADD(x, y) (((uint32_t)x)+((uint32_t)y))
#define _SUB(x, y) (((uint32_t)x)-((uint32_t)y))
#define _LT(x, y)  (((uint32_t)x)<((uint32_t)y))
#define _LTE(x, y) (((uint32_t)x)<=((uint32_t)y))
#define _GT(x, y)  (((uint32_t)x)>((uint32_t)y))
#define _GTE(x, y) (((uint32_t)x)>=((uint32_t)y))

/* Ranges **********************************************************************/
typedef struct __range {
  uint32_t start;
  uint32_t end;
} _range;

void _append_range(_range* ranges, size_t *rcount, _range* range) {
  // TODO: sort and coalesce blocks
  ranges[(*rcount)++] = *range;
}

void _insert_range(_range* ranges, size_t *rcount, size_t index, _range* range) {
  for (size_t i = *rcount; i > index; i--) {
    ranges[i] = ranges[i - 1];
  }
  ranges[index] = *range;
  (*rcount)++;
}

void _remove_range(_range* ranges, size_t *rcount, size_t index) {
  for (size_t i = index + 1; i < *rcount; i++) {
    ranges[i - 1] = ranges[i];
  }
  (*rcount)--;
}

int _loverlap_range(_range* left, _range* right) {
  return left->start < right->start && left->end >= right->start;
}

int _contains_range(_range* left, _range* right) {
  return left->start <= right->start && left->end >= right->end;
}

void _lock_ranges(_range* ranges, size_t *rcount, _range* lock) {
  for (size_t i = 0; i < *rcount; i++) {
    _range* range = ranges + i;

    if (_contains_range(lock, range)) {
      _remove_range(ranges, rcount, i);
      i--;
    } else if (_loverlap_range(range, lock)) {
      range->end = lock->start - 1;
    } else if (_loverlap_range(lock, range)) {
      range->start = lock->end + 1;
    } else if (_contains_range(range, lock)) {
      if (range->start == lock->start) {
        range->start = lock->end + 1;
      } else if (range->end == lock->end) {
        range->end = lock->start - 1;
      } else {
        _insert_range(ranges, rcount, i, range);
        range[i].end = lock->start - 1;
        ranges[i + 1].start = lock->end + 1;
        i++;
      }
    }
  }
}

void _dump_ranges(_range* ranges, size_t rcount) {
  for (size_t i = 0; i < rcount; i++) {
    printf("%X-%X ", ranges[i].start, ranges[i].end);
  }
  printf("\n");
}

/* Nodes **********************************************************************/
#define _NODE_MAGIC    0xDED0
#define _NODE_MIN_SIZE 1024   /* should be at least sizeof(_node) */

typedef struct __node {
  uint16_t magic;
  uint16_t free;
  void* end;
  struct __node* prev;
  struct __node* next;
  uint8_t data[];
} _node;

static void* _last_address = 0;
static _node* _first_node = 0;
static _node* _last_node = 0;

static _node* _node_for(void* ptr) {
  _node* node = (_node*) _SUB(ptr, sizeof(_node));
  ASSERT(_is_valid_node(node));
  return node;
}

static int _is_valid_node(_node* node) {
  if (node->magic != _NODE_MAGIC || node->next->magic != _NODE_MAGIC) {
    return 0;
  }

  return 1;
}

static int _are_adjacent_nodes(_node* left, _node* right) {
  return _ADD(left->end, 1) == (uint32_t) right;
}

static size_t _node_data_size(_node* node) {
  return _node_size(node) - sizeof(_node);
}

static size_t _node_size(_node* node) {
  return _SUB(node->end, node->data) + 1;
}

static void _append_node(void* start, void* end) {
  _node* new_node = (_node*) start;

  new_node->magic = _NODE_MAGIC;
  new_node->free = 1;
  new_node->end = end;
  new_node->next = 0;

  if (!_first_node) {
    _first_node = new_node;
    new_node->prev = 0;
  } else {
    _last_node->next = new_node;
    new_node->prev = _last_node;
  }

  _last_node = new_node;
}

static void _split_node(_node* node, size_t size) {
  _node* next = node->next;
  void* end = node->end;

  node->end = (void*) _ADD(node, sizeof(_node)+size-1);
  node->next = _ADD(node->end, 1);
  next->prev = node->next;

  _node* new_node = node->next;

  new_node->magic = _NODE_MAGIC;
  new_node->free = 1;
  new_node->end = end;
  new_node->prev = node;
  new_node->next = next;
}

static void _coalesce_node(_node* node) {
  ASSERT(node->free);

  while (node) {
    _node* next = node->next;

    if (next && next->free && _are_adjacent_nodes(node, next)) {
      next->magic = 0;

      node->next = next->next;
      node->end = next->end;

      if (node->next) {
        node->next->prev = node;
      } else {
        _last_node = node;
      }
    }

    _node* prev = node->prev;

    if (prev && prev->free) {
      node = prev;
    } else {
      node = 0;
    }
  }
}

/* MMU ************************************************************************/
void mmu_init(mmap_entry* mmap_addr, uint32_t mmap_length, void* kernel_start,
    void* kernel_end, void* stack_start, void* stack_end) {

  printf("mmu: loading memory map\n");

  // We assume this memory is always free because kernel is loaded in hi memory
  _range* ranges = (_range*) MMU_MIN_ADDRESS;
  size_t rcount = 0;

  // Visit mmap_entries
  for (mmap_entry* mma = mmap_addr; _SUB(mma, mmap_addr) < mmap_length; mma =
      (mmap_entry*) _ADD(mma, mma->entry_size + 4)) {

    uint64_t base_addr = mma->base_addr;
    uint64_t end_addr = mma->base_addr + mma->length - 1;

    // Log found block
    printf("mmu: found %s block %llX-%llX\n",
        (mma->type == MMAP_TYPE_AVAILABLE) ? "usable" : "locked", base_addr,
        end_addr);

    // Gather free blocks
    if ((mma->type == MMAP_TYPE_AVAILABLE) && (base_addr <= MMU_MAX_ADDRESS)) {

      // Calculate last valid address
      if (_GT(end_addr, _last_address)) {
        _last_address = (void*) (uint32_t) end_addr;
      }

      // Impose low limit
      if (base_addr < MMU_MIN_ADDRESS) {
        base_addr = MMU_MIN_ADDRESS;
      }

      // Impose high limit
      if (end_addr > MMU_MAX_ADDRESS) {
        end_addr = MMU_MAX_ADDRESS;
      }

      // Register range
      _range range = { base_addr, end_addr };
      _append_range(ranges, &rcount, &range);
    }
  }

  // Lock kernel in memory
  _range kernel_range = { (uint32_t) kernel_start, (uint32_t) kernel_end };
  _lock_ranges(ranges, &rcount, &kernel_range);

  // Lock stack in memory
  _range stack_range = { (uint32_t) stack_start, (uint32_t) stack_end };
  _lock_ranges(ranges, &rcount, &stack_range);

  // Copy nodes to stack to avoid overwriting them
  _range ranges_copy[rcount];
  for (size_t i = 0; i < rcount; i++) {
    ranges_copy[i] = ranges[i];
  }

  // Register nodes
  for (size_t i = 0; i < rcount; i++) {
    if ((ranges_copy[i].end - ranges_copy[i].start) > sizeof(_node)) {
      _append_node((void*) ranges_copy[i].start, (void*) ranges_copy[i].end);
    }
  }

  printf("mmu: last valid address is %X\n", mmu_last_address());
}

void* mmu_alloc(size_t size) {
  for (_node* node = _first_node; node; node = node->next) {
    if (node->free) {
      size_t node_size = _node_size(node);

      if (node_size >= size) {

        if ((node_size - size) >= _NODE_MIN_SIZE) {
          _split_node(node, size);
        }

        node->free = 0;
        return node->data;
      }
    }
  }

  return (void*) 0;
}

void* mmu_realloc(void* ptr, size_t size) {
  uint8_t* new_data = mmu_alloc(size);

  _node* node = _node_for(ptr);
  size_t data_size = _node_data_size(node);
  if (data_size > size) {
    data_size = size;
  }

  for (size_t i = 0; i < data_size; i++) {
    new_data[i] = node->data[i];
  }

  mmu_free(ptr);

  return new_data;
}

void mmu_free(void* ptr) {
  _node* node = _node_for(ptr);

  ASSERT(!node->free);

  node->free = 1;

  _coalesce_node(node);
}

void* mmu_last_address() {
  return _last_address;
}

void mmu_dump() {
  printf("mmu_dump: ");
  for (_node* node = _first_node; node; node = node->next) {
    if (node != _first_node) {
      printf(", ");
    }
    printf("%c(%X-%X)", node->free ? 'o' : 'x', node, node->end);
  }
  printf("\n");
}

