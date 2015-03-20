/*
 * windows_sound.c
 * Stephen Sanders & Jake Singer
 *
 */
#include <engine/sound.h>

#if WINDOWS

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#define SOUNDS_MAX 32

static LPDIRECTSOUND8 player;
static LPLPDIRECTSOUNDBUFFER buffers[SOUNDS_MAX];
int nextFree;
int highest_allocated;

static int find_next_free();

int FASTCALL
sound_init(void)
{
	HWND window = GetActiveWindow();
	HRESULT help;
	int i;
	help = DirectSoundCreate8(NULL, &player, NULL);
	help = IDirectSound_SetCooperativeLevel(player, window, DSSCL_NORMAL);
	for (i = 0; i < SOUNDS_MAX; ++i) {		
		buffers[i]=NULL;
	}
	nextFree = 0;
	highest_allocated =- 1;
	return 0;
}

int FASTCALL
sound_end(void)
{
	sound_release_all();
	IDirectSound8_Release(player);
	return 0;
}

/*Adds a sound into the buffers*/
int FASTCALL
sound_load(const char *path)
{
	int handle;
	DSBUFFERDESC buff_desc;
	WAVEFORMATEX wave_form;
	HMMIO hmmio;
	BYTE *wave_data;
	LPVOID sound_block1,sound_block2;
	DWORD bytes_sound_block1,bytes_sound_block2;
	MMCKINFO dad,son;
	uint32_t size;
	HRESULT help;
	char buf[512];
	IUnknown *pUnknown;
	if(nextFree==-1){
		nextFree=find_next_free();
		if(nextFree==-1)
			return -1;
	}
	memset(buf,0,sizeof(buf));
	pUnknown=(IUnknown*)buf;
	memset(&wave_form, 0, sizeof(WAVEFORMATEX)); 
	wave_form.wFormatTag = WAVE_FORMAT_PCM; 
	wave_form.nChannels = 2; 
	wave_form.nSamplesPerSec = 44100; 
	wave_form.nBlockAlign = 4;	
	wave_form.nAvgBytesPerSec = wave_form.nSamplesPerSec * wave_form.nBlockAlign; 
	wave_form.wBitsPerSample = 16; 
	memset(&buff_desc, 0, sizeof(DSBUFFERDESC));
	buff_desc.dwSize = sizeof(DSBUFFERDESC);
	buff_desc.dwFlags = DSBCAPS_STATIC;
	wave_data = 0;
	dad.ckid = (FOURCC)0;
	dad.cksize = 0;
	dad.fccType = (FOURCC)0;
	dad.dwDataOffset = 0;
	dad.dwFlags = 0;
	son=dad;
	hmmio=mmioOpen((LPSTR)path,NULL,MMIO_READ|MMIO_ALLOCBUF);
	if(hmmio==NULL)
		return 0;
	dad.fccType=mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio, &dad,NULL, MMIO_FINDRIFF)){
		mmioClose(hmmio,0);
		return 0;
	}
	son.ckid=mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hmmio, &son, &dad, MMIO_FINDCHUNK)){
		mmioClose(hmmio, 0); 
		return 0; 
	}
	size = son.cksize;
	wave_data=(BYTE*)malloc(size);
	if(wave_data==NULL){
		mmioClose(hmmio,0);
		return 0;
	}
	if((int)mmioRead(hmmio,(char*)wave_data,size)!=size){
		mmioClose(hmmio,0);
		free(wave_data);
		return 0;
	}
	buff_desc.dwBufferBytes = size;
	buff_desc.lpwfxFormat = &wave_form;
	buffers[nextFree]=(LPDIRECTSOUNDBUFFER*)malloc(sizeof(LPDIRECTSOUNDBUFFER) * 1);
	/*buffers[nextFree][0]=0;*/
    help=IDirectSound8_CreateSoundBuffer(player,(LPCDSBUFFERDESC)&buff_desc, buffers[nextFree], NULL);
	IDirectSoundBuffer_Lock(*buffers[nextFree],0,size,&sound_block1,&bytes_sound_block1,&sound_block2,&bytes_sound_block2,0);
	CopyMemory(sound_block1,wave_data,bytes_sound_block1);
	if(sound_block2!=NULL)
		CopyMemory(sound_block2,wave_data+bytes_sound_block1,bytes_sound_block2);
	IDirectSoundBuffer_Unlock(*buffers[nextFree],sound_block1,bytes_sound_block1,sound_block2,bytes_sound_block2);
	free(wave_data);
	handle=nextFree;
	if(handle>highest_allocated)
		highest_allocated=handle;
	nextFree=-1;
	return handle;

	}

/*Releases sound out of the buffers*/
int FASTCALL
sound_release(int id)
{
	/* TODO: Doesn't deal with if user tries to release sound that isn't there*/
	if(id==-1||!buffers[id])
		return -1;
	sound_stop(id);
	free(buffers[id]);
	buffers[id]=NULL;
	nextFree=id;
	return id;
}

/*stops all currently playing sounds*/
int FASTCALL
sound_release_all()
{
	int i;
	for(i=highest_allocated;i>=0;--i){
		sound_release(i);
	}
	return 0;
}

/* TODO: returns the ID for the sound with that name, returns -1 or something if not there*/
int FASTCALL
sound_lookup(const char *name)
{
	return 0;
}

/*PLAYS A SOUND, 1 LOOP INFINITELY*/
int FASTCALL
sound_play(int id, int loop)
{
	HRESULT help;
	/*IDirectSoundBuffer_SetCurrentPosition(*buffers[id],0);*/
	if(loop)
		help=IDirectSoundBuffer_Play(*buffers[id],0,0, DSBPLAY_LOOPING);
	else
		help=IDirectSoundBuffer_Play(*buffers[id],0,0, 0);

	help = IDirectSoundBuffer_Restore(*buffers[id]);
	return id;


}


int FASTCALL
sound_play_at_time(int id, int loop, DWORD seek)
{
	if(id == -1)
		return -1;
	IDirectSoundBuffer_SetCurrentPosition(*buffers[id], seek);
	sound_play(id, loop);
	return id;
}

/*pauses sound by id*/
int FASTCALL
sound_stop(int id)
{
	if(id==-1)
		return -1;
	IDirectSoundBuffer_Stop(*buffers[id]);

	return id;
}

/* TODO: Tells if the sound with current id is playing*/
int FASTCALL
sound_playing(int id)
{
	DWORD result;
	IDirectSoundBuffer_GetStatus(*buffers[id],&result);
	return ((result) & DSBSTATUS_PLAYING );
}

extern int FASTCALL
sound_get_time(int id, LPDWORD time_cursor)
{
	if(id==-1)
		return -1;
	IDirectSoundBuffer_GetCurrentPosition(*buffers[id],time_cursor, NULL);
	return id;
}

/*syncs sounds*/
extern int FASTCALL
sound_sync(int sync_to, int id, int loop)
{
	DWORD where_sync;
	if(sync_to == -1 || id == -1)
		return -1;
	sound_get_time(sync_to, &where_sync);
	sound_play_at_time(id, loop, where_sync);
	return id;

}

/*
 * Internal Sound Functions
 */

/*finds next free buffer space*/
static int
find_next_free()
{
	int i;
	for(i=0;i<SOUNDS_MAX;i++)
	{
		if(buffers[i]==NULL)
			return i;
	}
	return -1;
}

#endif /* WINDOWS */
