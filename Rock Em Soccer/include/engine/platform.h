/*
 * <engine/platform.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_PLATFORM__
#define __H_ENGINE_PLATFORM__
#ifdef __cplusplus
extern "C" {
#endif

#include <engine/define.h>
#include <engine/types.h>

/* platform presentation modes */
enum {
	PLATFORM_WINDOWED         = 0x00000001,
	PLATFORM_FULLSCREEN       = 0x00000002,
	PLATFORM_HIDDEN           = 0x00000004
};

/* initialize platformy things */
extern int
platform_init(const char *name, uint32_t width, uint32_t height, uint32_t mode);

extern int
platform_update(void);

extern int
platform_release(void);

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_PLATFORM__ */
