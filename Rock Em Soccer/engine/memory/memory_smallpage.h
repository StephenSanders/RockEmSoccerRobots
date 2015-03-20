/*
 * memory_smallpage.h
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#ifndef __H_ENGINE_MEMORY_SMALLPAGE__
#define __H_ENGINE_MEMORY_SMALLPAGE__

#include <engine/memory.h>
#include "memory_osapi.h"

/*
 * Small Page Layout:
 * [alloc][...][...][alloc][...][...][...]
 * [padding]
 * [allocation bit mask]       <- start of footer
 * [struct mem_smallpage_tail] <- end of footer
 */

/*
 * Small Page Tail (End of Footer)
 */

/* small page tail structure */
struct mem_smallpage_tail
{
	uint16_t free_count;
	uint16_t block_size;
	void *prev, *next;
};

/*
 * Small Page Information
 */

/* total number of allocation blocks on a small page */
#define MEM_SMALL_PAGE_NUM              ((MEM_MEDIUM_ALLOC_SIZE - 1)/          \
                                        MEM_SMALL_ALLOC_SIZE)

/* location of tail structure relative to start of page */
#define MEM_SMALL_PAGE_TAIL_OFFSET      (MEM_PAGE_SIZE -                       \
                                        sizeof(struct mem_smallpage_tail))

/* get the tail location */
#define MEM_SMALL_PAGE_TAIL(page)       ((struct mem_smallpage_tail *)((char *)\
                                        page + MEM_SMALL_PAGE_TAIL_OFFSET))

/*
 * Small Page Functions
 */

extern void mem_smallpage_init(void *page, uint32_t block_size);
extern void *mem_smallpage_alloc(void *page, uint32_t block_size, int *is_full);
extern int mem_smallpage_free(void *page, void *mem, int *was_full);
extern void mem_smallpage_list_insert(void *page, void **list);
extern void mem_smallpage_list_remove(void *page, void **list);
extern int mem_smallpage_verify(void *page);

#endif /* __H_ENGINE_MEMORY_SMALLPAGE__ */
