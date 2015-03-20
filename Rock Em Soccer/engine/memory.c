/*
 * memory.c
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#include <engine/memory.h>
#include "memory/memory_osapi.h"
#include "memory/memory_smallpage.h"
#include "memory/memory_mediumpage.h"
#include <engine/bits.h>

/* entries contain linked lists for allocing [32, 64, 96] bytes */
static void *smallpages[MEM_SMALL_PAGE_NUM];

/* linked lists to full pages of corresponding sizes */
static void *smallpages_full[MEM_SMALL_PAGE_NUM];

/* index represents number of consecutive blocks in linked pages */
static void *mediumpages[MEM_MEDIUM_PAGE_NUM];

/* each bit signifies a non-null entry in mediumpages for fast searching */
static uint32_t  mediumpage_mask[(MEM_MEDIUM_PAGE_NUM + 31) / 32];

/* allocation tracting */
static uint32_t small_mem_allocated, medium_mem_allocated, large_mem_allocated,
                small_allocations,   medium_allocations,   large_allocations;

int FASTCALL
mem_init(void)
{
	uint32_t i;

	for (i = 0; i < MEM_SMALL_PAGE_NUM; ++i) {
		smallpages[i] = 0;
		smallpages_full[i] = 0;
	}

	for (i = 0; i < MEM_MEDIUM_PAGE_NUM; ++i) {
		mediumpages[i] = 0;
	}

	/* no pages exist yet */
	bit_clear_range(&mediumpage_mask, 0, MEM_MEDIUM_PAGE_NUM - 1);
	small_mem_allocated = medium_mem_allocated = large_mem_allocated = 0;
	small_allocations = medium_allocations = large_allocations = 0;

	/* initialize system */
	mem_osapi_init();

	return 0;
}

int FASTCALL
mem_end(void)
{
	/* TODO: these checks should not run in release mode */
	uint32_t i;

	for (i = 0; i < MEM_SMALL_PAGE_NUM; ++i) {
		if (smallpages[i] != 0) return -1;
		if (smallpages_full[i] != 0) return -1;
	}

	for (i = 0; i < MEM_MEDIUM_PAGE_NUM; ++i) {
		if (mediumpages[i] != 0) return -1;
	}

	if (bit_clz_range(&mediumpage_mask, 0, MEM_MEDIUM_PAGE_NUM - 1)
		!= MEM_MEDIUM_PAGE_NUM) {
		return -1;
	}

	/* shutdown system */
	mem_osapi_release();

	return 0;
}

void * FASTCALL
mem_alloc(uint32_t bytes)
{
	uint32_t small_list_index, small_size;

	/* allocation must be at least MEM_SMALL_ALLOC_SIZE big */
	bytes = (bytes < MEM_SMALL_ALLOC_SIZE ? MEM_SMALL_ALLOC_SIZE : bytes);

	/* find correct linked list slot assuming a small sized allocation */
	small_list_index = (bytes - 1) / MEM_SMALL_ALLOC_SIZE;
	small_size = small_list_index * MEM_SMALL_ALLOC_SIZE + MEM_SMALL_ALLOC_SIZE;

	/* small allocation */
	if (small_size < MEM_MEDIUM_ALLOC_SIZE) {
		void *result;
		void *page = smallpages[small_list_index];
		int is_full = 0;

		/* if no page exists, make new one */
		if (!page) {
			page = mem_osapi_allocsmallpage();
			mem_smallpage_init(page, small_size);
			smallpages[small_list_index] = page;
		}

		/* attempt allocating memory */
		result = mem_smallpage_alloc(page, small_size, &is_full);

		/* if page is now full, move to appropriate list */
		if (is_full) {
			mem_smallpage_list_remove(page, &smallpages[small_list_index]);
			mem_smallpage_list_insert(page, &smallpages_full[small_list_index]);
		}

		/* track allocation */
		small_mem_allocated += small_size;
		++small_allocations;

		/* return the memory */
		return result;
	/* medium allocation */
	} else if(bytes < MEM_LARGE_ALLOC_SIZE) {
		const uint32_t medium_size = (bytes < MEM_MEDIUM_ALLOC_SIZE
			? MEM_MEDIUM_ALLOC_SIZE : ((bytes - MEM_MEDIUM_ALLOC_SIZE
			+ MEM_MEDIUM_ALLOC_STEP - 1) / MEM_MEDIUM_ALLOC_STEP)
			* MEM_MEDIUM_ALLOC_STEP + MEM_MEDIUM_ALLOC_SIZE);
		const uint32_t blocks_required = (medium_size + MEM_MEDIUM_ALLOC_STEP
			- 1) / MEM_MEDIUM_ALLOC_STEP;
		const uint32_t page_index = bit_clz_range(&mediumpage_mask,
			blocks_required, MEM_MEDIUM_PAGE_NUM - 1);

		void *result;
		void *page = 0;

		/* no page available with enough space, create a new one */
		if (page_index == MEM_MEDIUM_PAGE_NUM) {
			page = mem_osapi_allocmediumpage();
			mem_mediumpage_init(page);
		} else {
			page = mediumpages[page_index];
			mem_mediumpage_list_remove(page, &mediumpages[page_index]);
			
			if (!mediumpages[page_index]) {
				bit_clear(&mediumpage_mask, page_index);
			}
		}

		result = mem_mediumpage_alloc(page, medium_size);
		
		{
			const uint32_t big = MEM_MEDIUM_PAGE_TAIL(page)->bigblock_size;
			if (!mediumpages[big]) {
				bit_set(&mediumpage_mask, big);
			}

			mem_mediumpage_list_insert(page, &mediumpages[big]);
		}

		medium_mem_allocated += medium_size;
		++medium_allocations;

		return result;
	/* large allocation */
	} else {
		large_mem_allocated += bytes;
		++large_allocations;
		return mem_osapi_alloclarge(bytes);
	}

	return 0;
}

void FASTCALL
mem_free(void *mem)
{
	void *page = mem_osapi_issmallblock(mem);

	/* is small alloc? */
	if (page) {
		const uint32_t block_size = MEM_SMALL_PAGE_TAIL(page)->block_size;
		const uint32_t list_index = (block_size - MEM_SMALL_ALLOC_SIZE)
			/ MEM_SMALL_ALLOC_SIZE;

		void **page_head = &smallpages[list_index];
		void **page_head_full = &smallpages_full[list_index];
		int is_empty, was_full = 0;

		/* free the allocation */
		is_empty = mem_smallpage_free(page, mem, &was_full);

		/* if page was listed as full, move to the usable list */
		if (was_full) {
			mem_smallpage_list_remove(page, page_head_full);
			mem_smallpage_list_insert(page, page_head);
		}

		/* if the page is now empty, free it */
		if (is_empty) {
			mem_smallpage_list_remove(page, page_head);
			mem_osapi_freesmallpage(page);
		}

		small_mem_allocated -= block_size;
		--small_allocations;
	} else {
		page = mem_osapi_ismediumblock(mem);

		/* is medium alloc? */
		if (page) {
			const uint32_t big = MEM_MEDIUM_PAGE_TAIL(page)->bigblock_size;

			uint32_t bytes_freed = 0;

			mem_mediumpage_list_remove(page, &mediumpages[big]);

			if (!mediumpages[big]) {
				bit_clear(&mediumpage_mask, big);
			}

			if (mem_mediumpage_free(page, mem, &bytes_freed)) {
				mem_osapi_freemediumpage(page);
			} else {
				const uint32_t new_big =\
					MEM_MEDIUM_PAGE_TAIL(page)->bigblock_size;

				if (!mediumpages[new_big]) {
					bit_set(&mediumpage_mask, new_big);
				}

				mem_mediumpage_list_insert(page, &mediumpages[new_big]);
			}

			medium_mem_allocated -= bytes_freed;
			--medium_allocations;
		/* large alloc */
		} else {
			uint32_t bytes_freed = 0;

			mem_osapi_freelarge(mem, &bytes_freed);

			large_mem_allocated -= bytes_freed;
			--large_allocations;
		}
	}

	return;
}

extern int FASTCALL
mem_verify(void)
{
	uint32_t i;

	/* check small pages */
	for (i = 0; i < MEM_SMALL_PAGE_NUM; ++i) {
		if (smallpages[i]) {
			if (!mem_osapi_issmallblock(smallpages[i])) {
				return -1;
			}
			mem_smallpage_verify(smallpages[i]);
		}
	}

	/* check medium pages */
	for (i = 0; i < MEM_MEDIUM_PAGE_NUM; ++i) {
		if (mediumpages[i]) {
			if (!mem_osapi_ismediumblock(mediumpages[i])) {
				return -1;
			}
			mem_mediumpage_verify(mediumpages[i]);
		}
	}

	/* large allocations handled by system */

	return 0;
}
