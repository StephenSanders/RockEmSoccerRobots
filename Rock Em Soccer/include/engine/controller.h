/*
 * <engine/controller.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_CONTROLLER__
#define __H_ENGINE_CONTROLLER__
#ifdef __cplusplus
extern "C" {
#endif


#include <engine/engine.h>

enum {
	BUTT_D_UP=0,
	BUTT_D_DOWN,
	BUTT_D_LEFT,
	BUTT_D_RIGHT,
	BUTT_START,
	BUTT_BACK,
	BUTT_L_THUMB_B,
	BUTT_R_THUMB_B,
	BUTT_LB,
	BUTT_RB,
	BUTT_A,
	BUTT_B,
	BUTT_X,
	BUTT_Y,
	BUTT_MAX,
	BUTT_L_THUMB_UP,
	BUTT_L_THUMB_DOWN,
	BUTT_L_THUMB_LEFT,
	BUTT_L_THUMB_RIGHT
	/* BUTT_L_TRIG,   */
	/* BUTT_R_TRIG,   */
	/* BUTT_L_THUMB_S,*/
	/* BUTT_L_THUMB_S,*/
};

enum {
	BUTT_STATE_DOWN     = 0x0001,
	BUTT_STATE_UP       = 0x0000,
	BUTT_STATE_PRESSED  = 0x0010,
	BUTT_STATE_RELEASED = 0x0100
};

enum{
	CONTROLLER_RUMBLE_LEFT = 0,
	CONTROLLER_RUMBLE_RIGHT = 1
};

enum{
	CONTROLLER_1 = 0x01,
	CONTROLLER_2 = 0x02,
	CONTROLLER_3 = 0x04,
	CONTROLLER_4 = 0x08,
	CONTROLLER_1_2 = 0x03,
	CONTROLLER_1_2_3 = 0x07,
	CONTROLLER_1_2_3_4 = 0x0f
};

extern int FASTCALL
controller_init(void);

extern int FASTCALL
controller_end(void);

extern int FASTCALL
controller_check_connections(void);

extern int FASTCALL
controller_update(void);

extern int FASTCALL
controller_updated(void);

extern int FASTCALL
controller_update_all(void);

extern int FASTCALL
controller_clear(void);

extern uint16_t FASTCALL
controller_button_check(uint8_t code);

extern int FASTCALL
controller_button_pressed(uint8_t code);

extern int FASTCALL
controller_button_down(uint8_t code);

extern int FASTCALL
controller_button_released(uint8_t code);

extern int FASTCALL
controller_button_up(uint8_t code);

extern int FASTCALL
controller_connected(void);

extern int FASTCALL
controller_select(uint8_t i);

extern int FASTCALL
controller_rumble(int side);

extern int FASTCALL
controller_rumble_stop(void);

extern float FASTCALL
controller_left_stick_x(void);

extern float FASTCALL
controller_left_stick_y(void);




#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_CONTROLLER__ */