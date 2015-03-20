/*
 * windows_memory.c
 * Stephen Sanders & Jake Singer
 *
 * OS specific API calls for memmory allocation.
 * Windows: windows/windows_memory.c
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

extern "C" {
#include "../memory/memory_osapi.h"
}
#include <stdio.h> /* TODO: move to unified debug system */
#include <vector>
#include <hash_map>
#include <algorithm>
#include <windows.h>

static void *pool;
static uint32_t last_page_freed;
static std::vector<bool> freepage_mask;
static std::vector<bool> smallpage_mask;

stdext::hash_map<void *, uint32_t> large_allocs;

extern void FASTCALL
mem_osapi_init(void)
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	if (MEM_PAGE_SIZE != info.dwPageSize) {
		/* TODO: move to unified debug system */
		char buffer[128];
		sprintf_s(buffer, 128, "Page size should be: %u, is: %u \n",
			info.dwPageSize, MEM_PAGE_SIZE);
		OutputDebugString(buffer);
	}
	
	pool = VirtualAlloc(
		0, MEM_POOL_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
	);

	freepage_mask = std::vector<bool>(MEM_PAGE_COUNT, true);
	smallpage_mask = std::vector<bool>(MEM_PAGE_COUNT, false);
	last_page_freed = 0;
}

extern void FASTCALL
mem_osapi_release(void)
{
	VirtualFree(pool, 0, MEM_RELEASE);
}

/*
 * Query Size
 */

void * FASTCALL
mem_osapi_issmallblock(void *mem)
{
	/* ensure mem is in the pool */
	if ((uint32_t)mem < (uint32_t)pool ||
	    (uint32_t)mem >= (uint32_t)pool + MEM_POOL_SIZE) {
		return NULL;
	}

	/* page to check */
	uint32_t page_index = ((uint32_t)mem - (uint32_t)pool) / MEM_PAGE_SIZE;

	/* is small page? */
	if (smallpage_mask[page_index]) {
		return ((char *)pool + page_index * MEM_PAGE_SIZE);
	}

	return NULL;
}

void * FASTCALL
mem_osapi_ismediumblock(void *mem)
{
	/* ensure mem is in the pool */
	if ((uint32_t)mem < (uint32_t)pool ||
	    (uint32_t)mem >= (uint32_t)pool + MEM_POOL_SIZE) {
		return NULL;
	}

	/* page to check */
	uint32_t page_index = ((uint32_t)mem - (uint32_t)pool) / MEM_PAGE_SIZE;

	/* is medium page? */
	if (!smallpage_mask[page_index]) {
		return ((char *)pool + page_index * MEM_PAGE_SIZE);
	}

	return NULL;
}

int FASTCALL
mem_osapi_islargeblock(void *mem)
{
	/* ensure mem is outside of the pool */
	if ((uint32_t)mem < (uint32_t)pool ||
	    (uint32_t)mem >= (uint32_t)pool + MEM_POOL_SIZE) {
		return 1;
	}

	return 0;
}

/*
 * Make New Page
 */

void * FASTCALL
mem_osapi_allocsmallpage(void)
{
	/* first iteration searches starting at the cached value */
	for (uint32_t page_index = last_page_freed;
		page_index < MEM_PAGE_COUNT; ++page_index) {
		if (freepage_mask[page_index]) {
			freepage_mask[page_index] = false;
			smallpage_mask[page_index] = true;
			last_page_freed = page_index + 1;
			return ((char *)pool + page_index * MEM_PAGE_SIZE);
		}
	}

	/* second half of loop starts at the beginning */
	for (uint32_t page_index = 0; page_index < last_page_freed; ++page_index) {
		if (freepage_mask[page_index])
		{
			freepage_mask[page_index] = false;
			smallpage_mask[page_index] = true;
			last_page_freed = page_index + 1;			
			return ((char *)pool + page_index * MEM_PAGE_SIZE);
		}
	}

	return NULL;
}

void * FASTCALL
mem_osapi_allocmediumpage(void)
{
	/* first iteration searches starting at the cached value */
	for (uint32_t page_index = last_page_freed;
		page_index < MEM_PAGE_COUNT; ++page_index) {
		if (freepage_mask[page_index]) {
			freepage_mask[page_index] = false;
			smallpage_mask[page_index] = false;
			last_page_freed = page_index + 1;
			return ((char *)pool + page_index * MEM_PAGE_SIZE);
		}
	}

	/* second half of loop starts at the beginning */
	for (uint32_t page_index = 0; page_index < last_page_freed; ++page_index) {
		if (freepage_mask[page_index])
		{
			freepage_mask[page_index] = false;
			smallpage_mask[page_index] = false;
			last_page_freed = page_index + 1;			
			return ((char *)pool + page_index * MEM_PAGE_SIZE);
		}
	}
	return NULL;
}

/*
 * Release Page
 */

void FASTCALL
mem_osapi_freesmallpage(void *mem)
{
	const uint32_t page_index = ((uint32_t)mem - (uint32_t)pool) / MEM_PAGE_SIZE;
	freepage_mask[page_index] = true;
	last_page_freed = page_index;
}

void FASTCALL
mem_osapi_freemediumpage(void *mem)
{
	const uint32_t page_index = ((uint32_t)mem - (uint32_t)pool) / MEM_PAGE_SIZE;
	freepage_mask[page_index] = true;
	last_page_freed = page_index;
}

/*
 * Large Allocations
 */

void * FASTCALL
mem_osapi_alloclarge(uint32_t bytes)
{
	void *result = malloc(bytes);

	large_allocs.insert(std::make_pair(result, bytes));

	return result;
}

void FASTCALL
mem_osapi_freelarge(void *mem, uint32_t *bytes)
{
	stdext::hash_map<void *, uint32_t>::iterator i = large_allocs.find(mem);

	if (i != large_allocs.end()) {
		if (bytes) {
			*bytes = i->second;
		}

		free(mem);		

		large_allocs.erase(i);
	}
}
