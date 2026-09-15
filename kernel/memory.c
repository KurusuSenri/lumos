#include "../include/memory.h"
#include "../include/memory_map.h"

static uint64_t next_page;
static uint64_t pages_left;

/* Requires a validated map and valid output pointers. */
int find_largest_memory_region(const BootInfo *info,
                               uint64_t *best_start,
                               uint64_t *best_pages)
{
    *best_start = 0;
    *best_pages = 0;

    uint64_t count = memory_map_count(info);

    for (uint64_t i = 0; i < count; ++i) {
        const MemoryDescriptor *entry = memory_map_get(info, i);

        if (entry->type == MEMORY_TYPE_CONVENTIONAL &&
            entry->pages > *best_pages) {
            *best_start = entry->physical_start;
            *best_pages = entry->pages;
        }
    }

    return *best_pages != 0;
}

/*
 * Initialize once with a region returned by
 * find_largest_memory_region().
 */
void page_allocator_init(uint64_t start, uint64_t pages)
{
    next_page = start;
    pages_left = pages;
}

/* Return 1 on success and output the physical address. */
int alloc_page(uint64_t *physical_address)
{
    if (physical_address == 0 || pages_left == 0)
        return 0;

    *physical_address = next_page;
    next_page += MEMORY_PAGE_SIZE;
    --pages_left;

    return 1;
}