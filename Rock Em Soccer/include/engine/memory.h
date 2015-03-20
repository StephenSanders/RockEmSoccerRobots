/*
 * <engine/memory.h>
 * Stephen Sanders & Jake Singer
 *
 * Memory manager based on Jason Hughes' " A Highly Optimized Portable Memory
 * Manager" detailed in "Game Engine Gems": Volume 2.
 */

#ifndef __H_ENGINE_MEMORY__
#define __H_ENGINE_MEMORY__
#ifdef __cplusplus
extern "C" {
#endif

#include <engine/define.h>
#include <engine/types.h>

/* initialize and release all memory */
extern int FASTCALL
mem_init(void);

extern int FASTCALL
mem_end(void);

/* allocate and free memory */
extern void * FASTCALL
mem_alloc(uint32_t bytes);

extern void FASTCALL
mem_free(void *mem);

/* check for errors (this is very slow, do not use in release builds) */
extern int FASTCALL
mem_verify(void);

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_MEMORY__ */
