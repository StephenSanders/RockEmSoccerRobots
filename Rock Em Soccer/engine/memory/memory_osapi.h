/*
 * memory_osapi.h
 * Stephen Sanders & Jake Singer
 *
 * OS specific API calls for memmory allocation.
 * Windows: windows/windows_memory.c
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#ifndef __H_ENGINE_MEMORY_OSAPI__
#define __H_ENGINE_MEMORY_OSAPI__

#include <engine/memory.h>

/*
 * Memory Configuration
 */

/* minimum allocation */
#define MEM_SMALL_ALLOC_SIZE  (32) /* 32 bytes */

/* small sizes: (MEM_SMALL_ALLOC_SIZE + (n * MEM_SMALL_ALLOC_STEP)) */
/* #define MEM_SMALL_ALLOC_STEP  32 */ /* TODO: add support for this */

/* sizes >= MEM_MEDIUM_ALLOC_SIZE and < MEM_LARGE_ALLOC_SIZE are medium */
#define MEM_MEDIUM_ALLOC_SIZE (128) /* 128 bytes */

/* medium sizes: (MEM_MEDIUM_ALLOC_SIZE + (n * MEM_MEDIUM_ALLOC_STEP)) */
#define MEM_MEDIUM_ALLOC_STEP (256) /* 256 bytes */

/* sizes >= MEM_LARGE_ALLOC_SIZE are large */
#define MEM_LARGE_ALLOC_SIZE  (4096) /* 4 KB */

/* total memory for small and medium allocations */
#define MEM_POOL_SIZE         (512 * 1024 * 1024) /* 512 MB */

/* size of each page */
#define MEM_PAGE_SIZE         (4096) /* 4 KB */

/* number of pages */
#define MEM_PAGE_COUNT        (MEM_POOL_SIZE / MEM_PAGE_SIZE)

/*
 * Memory Functions
 */

extern void FASTCALL
mem_osapi_init(void);

extern void FASTCALL
mem_osapi_release(void);

/* query size */
extern void * FASTCALL
mem_osapi_issmallblock(void *mem);

extern void * FASTCALL
mem_osapi_ismediumblock(void *mem);

extern int FASTCALL
mem_osapi_islargeblock(void *mem);

/* make new page */
extern void * FASTCALL
mem_osapi_allocsmallpage(void);

extern void * FASTCALL
mem_osapi_allocmediumpage(void);

/* release page */
extern void FASTCALL
mem_osapi_freesmallpage(void *mem);

extern void FASTCALL
mem_osapi_freemediumpage(void *mem);

/* large allocations */
extern void * FASTCALL
mem_osapi_alloclarge(uint32_t bytes);

extern void FASTCALL
mem_osapi_freelarge(void *mem, uint32_t *bytes);

#endif /* __H_ENGINE_MEMORY_OSAPI__ */
