/*
 * <engine/time.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_TIME__
#define __H_ENGINE_TIME__
#ifdef __cplusplus
extern "C" {
#endif

extern float time_delta;
extern float time_current;

extern void time_init(void);
extern void time_update(void);

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_TIME__ */
