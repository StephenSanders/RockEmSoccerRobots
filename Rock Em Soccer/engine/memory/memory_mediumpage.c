/*
 * memory_mediumpage.c
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#include "memory_mediumpage.h"
#include <engine/bits.h>

void mem_mediumpage_init(void *page)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);

	bit_clear_range(
		MEM_MEDIUM_PAGE_ENDMASK(page), 0, MEM_MEDIUM_PAGE_BLOCKS - 1
	);

	bit_set_range(
		MEM_MEDIUM_PAGE_FREEMASK(page), 0, MEM_MEDIUM_PAGE_BLOCKS - 1
	);

	tail->bigblock_index = 0;
	tail->bigblock_size = MEM_MEDIUM_PAGE_BLOCKS;
	tail->free_count = MEM_MEDIUM_PAGE_BLOCKS;
	tail->next = 0;
	tail->prev = 0;

	return;
}

void *mem_mediumpage_alloc(void *page, uint32_t bytes)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);

	/* how many blocks required? */
	const uint32_t alloc_bits_required = (bytes + MEM_MEDIUM_ALLOC_STEP - 1)
		/ MEM_MEDIUM_ALLOC_STEP;

	void *bitmask_start_free      = MEM_MEDIUM_PAGE_FREEMASK(page);
	void *bitmask_start_alloc_end = MEM_MEDIUM_PAGE_ENDMASK(page);

	/* get biggest free spot */
	void *result = (uint8_t *)page + tail->bigblock_index
		* MEM_MEDIUM_ALLOC_STEP;

	/* mark blocks being allocated */
	bit_clear_range(
		bitmask_start_free,
		tail->bigblock_index,
		tail->bigblock_index + alloc_bits_required - 1
	);

	/* mark end of allocation */
	bit_set(
		bitmask_start_alloc_end, tail->bigblock_index + alloc_bits_required - 1
	);

	/* find new big block */
	mem_mediumpage_updatebigblock(page);
	tail->free_count -= alloc_bits_required;

	return result;
}

int mem_mediumpage_free(void *page, void *mem, uint32_t *bytes)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);

	void *bitmask_start_free      = MEM_MEDIUM_PAGE_FREEMASK(page);
	void *bitmask_start_alloc_end = MEM_MEDIUM_PAGE_ENDMASK(page);

	const uint32_t starting_bit = ((uint32_t)mem - (uint32_t)page)
		/ MEM_MEDIUM_ALLOC_STEP;
	const uint32_t end_of_alloc = bit_clz_range(
		bitmask_start_alloc_end, starting_bit, MEM_MEDIUM_ALLOC_STEP
	);

	/* free up blocks */
	bit_set_range(bitmask_start_free, starting_bit, end_of_alloc);

	/* remove end marking */
	bit_clear(bitmask_start_alloc_end, end_of_alloc);

	tail->free_count += end_of_alloc - starting_bit + 1;

	*bytes = (end_of_alloc - starting_bit + 1) * MEM_MEDIUM_ALLOC_STEP;

	mem_mediumpage_updatebigblock(page);

	return (tail->free_count == MEM_MEDIUM_PAGE_BLOCKS);
}

void mem_mediumpage_list_insert(void *page, void **list)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);

	if (*list) {
		struct mem_mediumpage_tail *next_tail = MEM_MEDIUM_PAGE_TAIL(*list);
		next_tail->prev = page;
	}

	tail->next = *list;
	*list = page;

	return;
}

void mem_mediumpage_list_remove(void *page, void **list)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);

	if (*list == page) {
		*list = tail->next;
	}

	if (tail->prev) {
		struct mem_mediumpage_tail *prev_tail = MEM_MEDIUM_PAGE_TAIL(tail->prev);
		prev_tail->next = tail->next;
	}

	if (tail->next) {
		struct mem_mediumpage_tail *next_tail = MEM_MEDIUM_PAGE_TAIL(tail->next);
		next_tail->prev = tail->prev;
	}

	tail->next = 0;
	tail->prev = 0;

	return;
}

int mem_mediumpage_verify(void *page)
{
	void *current_page = page;

	while (current_page) {
		struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(current_page);

		void *bitmask_start_free      = MEM_MEDIUM_PAGE_FREEMASK(page);
		void *bitmask_start_alloc_end = MEM_MEDIUM_PAGE_ENDMASK(page);

		uint32_t i, bits_set = 0, alloc_bits = 0;

		for (i = 0; i < MEM_MEDIUM_PAGE_BLOCKS; ++i) {
			if (bit_clz_range(bitmask_start_free, i, i) == i) {
				++bits_set;
			}
		}

		if (bits_set != tail->free_count) {
			return -1;
		}

		for (i = 0; i < MEM_MEDIUM_PAGE_BLOCKS; ++i) {
			if (bit_clz_range(bitmask_start_alloc_end, i, i) == i) {
				++alloc_bits;
			}
		}

		if (alloc_bits > MEM_MEDIUM_PAGE_BLOCKS - tail->free_count) {
			return -1;
		}

		current_page = tail->next;
	}

	return 0;
}

uint32_t mem_mediumpage_updatebigblock(void *page)
{
	struct mem_mediumpage_tail *tail = MEM_MEDIUM_PAGE_TAIL(page);
	void *bitmask_start_free = MEM_MEDIUM_PAGE_FREEMASK(page);

	uint32_t biggest_block = 0;
	uint32_t biggest_block_index = 0;
	uint32_t bit = 0;

	while (bit < MEM_MEDIUM_PAGE_BLOCKS - biggest_block) {
		/* find start */
		const uint32_t one_bit_index = bit_clz_range(
			bitmask_start_free, bit, MEM_MEDIUM_PAGE_BLOCKS - 1
		);

		/* find end */
		const uint32_t zero_bit_index = bit_clb_range(
			bitmask_start_free, one_bit_index + 1, MEM_MEDIUM_PAGE_BLOCKS - 1
		);

		/* calculate size */
		const uint32_t current_block_length = zero_bit_index - one_bit_index;

		/* no more free space? */
		if (one_bit_index == MEM_MEDIUM_PAGE_BLOCKS) {
			break;
		}

		/* found potential big block */
		if (current_block_length > biggest_block) {
			biggest_block = current_block_length;
			biggest_block_index = one_bit_index;
		}

		bit = zero_bit_index + 1;
	}

	/* update records */
	tail->bigblock_index = biggest_block_index;

	return (tail->bigblock_size = biggest_block);
}
