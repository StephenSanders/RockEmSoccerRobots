/*
 * memory_smallpage.c
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#include "memory_smallpage.h"
#include <engine/bits.h>

void mem_smallpage_init(void *page, uint32_t block_size)
{
	struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(page);
	uint8_t *bitmask;

	const uint32_t max_allocs_per_page =
		(MEM_PAGE_SIZE - sizeof(struct mem_smallpage_tail)) / block_size;
	const uint32_t bytes_required_for_tail =
		(max_allocs_per_page + 31) / 32 * 4 + sizeof(struct mem_smallpage_tail);
	const uint32_t num_blocks =
		(MEM_PAGE_SIZE - bytes_required_for_tail) / block_size;
	const uint32_t num_bytes_in_bitmask = (num_blocks + 31) / 32 * 4;

	tail->block_size = (uint16_t)block_size;
	tail->free_count = (uint16_t)num_blocks;
	tail->next = 0;
	tail->prev = 0;

	bitmask = (uint8_t *)tail - num_bytes_in_bitmask;
	bit_set_range(bitmask, 0, num_blocks - 1);

	return;
}

void *mem_smallpage_alloc(void *page, uint32_t block_size, int *is_full)
{
	struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(page);
	uint32_t one_bit_index;
	void *result;
	uint8_t *bitmask;

	const uint32_t max_allocs_per_page =
		(MEM_PAGE_SIZE - sizeof(struct mem_smallpage_tail)) / block_size;
	const uint32_t bytes_required_for_tail =
		(max_allocs_per_page + 31) / 32 * 4 + sizeof(struct mem_smallpage_tail);
	const uint32_t num_blocks =
		(MEM_PAGE_SIZE - bytes_required_for_tail) / block_size;
	const uint32_t num_bytes_in_bitmask = (num_blocks + 31) / 32 * 4;

	bitmask = (uint8_t *)tail - num_bytes_in_bitmask;
	one_bit_index = bit_clz_range(bitmask, 0, num_blocks - 1);

	bit_clear(bitmask, one_bit_index);

	result = (uint8_t *)page + one_bit_index * block_size;

	--(tail->free_count);
	*is_full = (tail->free_count == 0);

	return result;
}

int mem_smallpage_free(void *page, void *mem, int *was_full)
{
	struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(page);
	uint32_t block_number;
	uint8_t *bitmask;

	const uint32_t block_size = tail->block_size;
	const uint32_t max_allocs_per_page =
		(MEM_PAGE_SIZE - sizeof(struct mem_smallpage_tail)) / block_size;
	const uint32_t bytes_required_for_tail =
		(max_allocs_per_page + 31) / 32 * 4 + sizeof(struct mem_smallpage_tail);
	const uint32_t num_blocks =
		(MEM_PAGE_SIZE - bytes_required_for_tail) / block_size;
	const uint32_t num_bytes_in_bitmask = (num_blocks + 31) / 32 * 4;

	bitmask = (uint8_t *)tail - num_bytes_in_bitmask;
	block_number = ((uint32_t)mem - (uint32_t)page) / block_size;

	bit_set(bitmask, block_number);
	++(tail->free_count);

	*was_full = (tail->free_count == 1);

	return (tail->free_count == num_blocks);
}

void mem_smallpage_list_insert(void *page, void **list)
{
	struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(page);
	void *head_page = *list;

	if (head_page) {
		struct mem_smallpage_tail *page_tail = MEM_SMALL_PAGE_TAIL(head_page);
		page_tail->prev = page;
	}

	tail->next = head_page;
	*list = page;

	return;
}

void mem_smallpage_list_remove(void *page, void **list)
{
	struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(page);

	if (*list == page) {
		*list = tail->next;
	}

	if (tail->prev) {
		struct mem_smallpage_tail *prev_tail = MEM_SMALL_PAGE_TAIL(tail->prev);
		prev_tail->next = tail->next;
	}

	if (tail->next) {
		struct mem_smallpage_tail *next_tail = MEM_SMALL_PAGE_TAIL(tail->next);
		next_tail->prev = tail->prev;
	}

	tail->next = 0;
	tail->prev = 0;

	return;
}

int mem_smallpage_verify(void *page)
{
	void *current_page = page;

	while (current_page) {
		struct mem_smallpage_tail *tail = MEM_SMALL_PAGE_TAIL(current_page);
		uint32_t i, bits_set = 0;

		const uint32_t block_size = tail->block_size;
		const uint32_t max_allocs_per_page =
			(MEM_PAGE_SIZE - sizeof(struct mem_smallpage_tail)) / block_size;
		const uint32_t bytes_required_for_tail =
			(max_allocs_per_page + 31) / 32 * 4 + sizeof(struct mem_smallpage_tail);
		const uint32_t num_blocks =
			(MEM_PAGE_SIZE - bytes_required_for_tail) / block_size;
		const uint32_t num_bytes_in_bitmask = (num_blocks + 31) / 32 * 4;

		uint8_t *bitmask = (uint8_t *)tail - num_bytes_in_bitmask;

		for (i = 0; i < num_blocks; ++i) {
			if (bit_clz_range(bitmask, i, i) == i) {
				++bits_set;
			}
		}

		if (bits_set != tail->free_count) {
			return -1;
		}

		current_page = tail->next;
	}
	return 0;
}
