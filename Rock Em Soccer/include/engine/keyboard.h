/*
 * <engine/keyboard.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_KEYBOARD__
#define __H_ENGINE_KEYBOARD__
#ifdef __cplusplus
extern "C" {
#endif


#include <engine/engine.h>

enum {
	KEY_A = 0,
	KEY_D,
	KEY_P,
	KEY_S,
	KEY_W,
	KEY_X,
	KEY_Z,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_SPACE,
	KEY_RSHIFT,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_MAX
};

enum {
	KEY_STATE_DOWN     = 0x0100,
	KEY_STATE_UP       = 0x0101,
	KEY_STATE_PRESSED  = 0x1000,
	KEY_STATE_RELEASED = 0x0010
};

extern int FASTCALL
keyboard_init(void);

extern int FASTCALL
keyboard_end(void);

extern int FASTCALL
keyboard_update(void);

extern int FASTCALL
keyboard_updated(void);

extern int FASTCALL
keyboard_clear(void);

extern uint8_t FASTCALL
key_check(uint8_t code);

extern int FASTCALL
key_pressed(uint8_t code);

extern int FASTCALL
key_down(uint8_t code);

extern int FASTCALL
key_released(uint8_t code);

extern int FASTCALL
key_up(uint8_t code);

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_KEYBOARD__ */