/*
 * memory_mediumpage.h
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#ifndef __H_ENGINE_MEMORY_MEDIUMPAGE__
#define __H_ENGINE_MEMORY_MEDIUMPAGE__

#include <engine/memory.h>
#include "memory_osapi.h"

/*
 * Medium Page Layout:
 * [alloc][...][...][alloc][...][...][...]
 * [padding]
 * [allocation bit mask]        <- start of footer
 * [allocation start bit mask]
 * [struct mem_mediumpage_tail] <- end of footer
 */

/*
 * Medium Page Tail (End of Footer)
 */

/* medium page tail structure */
struct mem_mediumpage_tail
{
	uint32_t bigblock_index;
	uint32_t bigblock_size;
	uint32_t free_count;
	void *prev, *next;
};

/*
 * Medium Page Information
 */

/* number of allocation blocks worth on a medium page */
#define MEM_MEDIUM_PAGE_NUM             (MEM_PAGE_SIZE/MEM_MEDIUM_ALLOC_STEP)

/* DO NOT USE: for internal computation */
#define MEM_MEDIUM_PAGE_IDEAL_BLOCKS    ((MEM_PAGE_SIZE + MEM_MEDIUM_ALLOC_STEP\
                                        - 1) / MEM_MEDIUM_ALLOC_STEP)

/* total footer size (includes two bit masks) */
#define MEM_MEDIUM_PAGE_FOOTER_SIZE     ((MEM_MEDIUM_PAGE_IDEAL_BLOCKS + 31)   \
                                        / 32 * 4 * 2 /* two 4 byte masks */    \
                                        + sizeof(struct mem_mediumpage_tail))

/* number of allocation blocks per page */
#define MEM_MEDIUM_PAGE_BLOCKS          ((MEM_PAGE_SIZE                        \
                                        - MEM_MEDIUM_PAGE_FOOTER_SIZE)         \
                                        / MEM_MEDIUM_ALLOC_STEP)

/* size of a single bitmask in bytes */
#define MEM_MEDIUM_PAGE_MASK_SIZE       (MEM_MEDIUM_PAGE_BLOCKS + 31) / 32 * 4

/* location of tail structure relative to start of page */
#define MEM_MEDIUM_PAGE_TAIL_OFFSET     (MEM_PAGE_SIZE                         \
                                        - sizeof(struct mem_mediumpage_tail))

/* location of free block bitmask */
#define MEM_MEDIUM_PAGE_FREE_OFFSET     (MEM_MEDIUM_PAGE_TAIL_OFFSET           \
                                        - MEM_MEDIUM_PAGE_MASK_SIZE)

/* location of alloc end bitmask */
#define MEM_MEDIUM_PAGE_END_OFFSET      (MEM_MEDIUM_PAGE_TAIL_OFFSET           \
                                        - MEM_MEDIUM_PAGE_MASK_SIZE            \
                                        - MEM_MEDIUM_PAGE_MASK_SIZE)

/* get the tail location */
#define MEM_MEDIUM_PAGE_TAIL(page)      ((struct mem_mediumpage_tail *)        \
                                        ((char *)page                          \
                                        + MEM_MEDIUM_PAGE_TAIL_OFFSET))

/* get the free block bitmask */
#define MEM_MEDIUM_PAGE_FREEMASK(page)  ((void *)((char *)page                 \
                                        + MEM_MEDIUM_PAGE_FREE_OFFSET))

/* get the alloc end bitmask */
#define MEM_MEDIUM_PAGE_ENDMASK(page)   ((void *)((char *)page                 \
                                        + MEM_MEDIUM_PAGE_END_OFFSET))

/*
 * Medium Page Functions
 */

extern void mem_mediumpage_init(void *page);
extern void *mem_mediumpage_alloc(void *page, uint32_t bytes);
extern int mem_mediumpage_free(void *page, void *mem, uint32_t *bytes);
extern void mem_mediumpage_list_insert(void *page, void **list);
extern void mem_mediumpage_list_remove(void *page, void **list);
extern int mem_mediumpage_verify(void *page);
extern uint32_t mem_mediumpage_updatebigblock(void *page);

#endif /* __H_ENGINE_MEMORY_MEDIUMPAGE__ */
