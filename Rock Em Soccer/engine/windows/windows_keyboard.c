/*
 * windows_keyboard.c
 * Stephen Sanders & Jake Singer
 *
 */

#include <engine/keyboard.h>

#if WINDOWS

#include <windows.h>

SHORT key_states[KEY_MAX];
const uint8_t key_codes[KEY_MAX] = {
	/* KEY_A      */    0x41,
	/* KEY_D      */    0x44,
	/* KEY_P      */    0x50,
	/* KEY_S      */    0x53,
	/* KEY_W      */    0x57,
	/* KEY_X      */	0x58,
	/* KEY_Z      */	0x5A,
	/* KEY_UP     */    VK_UP,
	/* KEY_DOWN   */    VK_DOWN,
	/* KEY_LEFT   */    VK_LEFT,
	/* KEY_RIGHT  */    VK_RIGHT,
	/* KEY_LSHIFT */    VK_LSHIFT,
	/* KEY_LCTRL  */    VK_LCONTROL,
	/* KEY_SPACE  */    VK_SPACE,
	/* KEY_RSHIFT */	VK_RSHIFT,
	/* KEY_ENTER */     VK_RETURN,
	/* KEY_ESCAPE */	VK_ESCAPE
};
int updated;


int FASTCALL
keyboard_init(void)
{
	return 0;
}

int FASTCALL
keyboard_end(void)
{
	return 0;
}

/* Updates all the key states*/
int FASTCALL
keyboard_update(void)
{
	int i;
	SHORT new_key;
	updated = 0;
	for(i=0;i<KEY_MAX;++i){
		if ((GetAsyncKeyState(key_codes[i]) & 0x8000))
			new_key=KEY_STATE_DOWN;
		else
			new_key=KEY_STATE_UP;
		if(new_key==KEY_STATE_DOWN&&key_states[i]==KEY_STATE_UP){
			key_states[i]=KEY_STATE_PRESSED;
			updated = 1;
		}
		else if(new_key==KEY_STATE_UP&&key_states[i]==KEY_STATE_DOWN)
			key_states[i]=KEY_STATE_RELEASED;
		else
			key_states[i] = new_key;
	}
	return 0;
}

int FASTCALL
keyboard_updated(void)
{
	return updated;
}

int FASTCALL
keyboard_clear(void)
{
	int i;
	for(i=0;i<KEY_MAX;++i){
		key_states[i]=KEY_STATE_UP;
	}
	return 0;
}

/*check a specific key state*/
uint8_t FASTCALL
key_check(uint8_t code)
{
	return (uint8_t)key_states[code];
}

int FASTCALL
key_pressed(uint8_t code)
{
	return (key_states[code]==KEY_STATE_PRESSED);
}

int FASTCALL
key_down(uint8_t code)
{
	return (key_states[code]==KEY_STATE_DOWN);
}

int FASTCALL
key_released(uint8_t code)
{
	return (key_states[code]==KEY_STATE_RELEASED);
}

int FASTCALL
key_up(uint8_t code)
{
	return (key_states[code]==KEY_STATE_UP);
}

#endif /* WINDOWS */
