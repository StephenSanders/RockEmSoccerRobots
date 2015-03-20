/*
 * <engine/audio.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_SOUND__
#define __H_ENGINE_SOUND__
#ifdef __cplusplus
extern "C" {
#endif

#include <engine/engine.h>
#include <Windows.h>

#define LOOP_INFINITELY -1

/* initialize sound stuff */
extern int FASTCALL
sound_init(void);

/* shuts down sound stuff */
extern int FASTCALL
sound_end(void);

/*Adds a sound into the buffers*/
extern int FASTCALL
sound_load(const char *path);

/*Releases sound out of the buffers*/
extern int FASTCALL
sound_release(int id);

/*releases all sounds*/
extern int FASTCALL
sound_release_all(void);

/*returns the ID for the sound with that name, returns -1 or something if not there*/
extern int FASTCALL
sound_lookup(const char *name);

/*PLAYS A SOUND, -1 LOOP INFINITELY*/
extern int FASTCALL
sound_play(int id, int loop);

extern int FASTCALL
sound_play_at_time(int id, int loop, DWORD seek);

/*pauses sound by id*/
extern int FASTCALL
sound_stop(int id);

/*returns time in song as cursor LPDWORD to be used in sound_play_at_time(int id, LPDWORD cursor) */
extern int FASTCALL
sound_get_time(int id, LPDWORD time_cursor);

/*syncs sounds*/
extern int FASTCALL
sound_sync(int sync_to, int id, int loop);

/*Tells if the sound with current id is playing*/
extern int FASTCALL
sound_playing(int id);
	
#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_SOUND__ */