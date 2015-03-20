
#include <engine/controller.h>

#if WINDOWS

#include <windows.h>
#include <XInput.h>
#include <math.h>

SHORT button_states[4][BUTT_MAX + 4];
float controller_lstick_x[4], controller_lstick_y[4];

XINPUT_STATE controller_state[4];

int controller_selected, updated_controller;

uint8_t connected;

const int butt_codes[BUTT_MAX] = {
	/* BUTT_D_UP      */    XINPUT_GAMEPAD_DPAD_UP,
	/* BUTT_D_DOWN    */    XINPUT_GAMEPAD_DPAD_DOWN,
	/* BUTT_D_LEFT    */    XINPUT_GAMEPAD_DPAD_LEFT,
	/* BUTT_D_RIGHT   */    XINPUT_GAMEPAD_DPAD_RIGHT,
	/* BUTT_START     */    XINPUT_GAMEPAD_START,
	/* BUTT_BACK      */    XINPUT_GAMEPAD_BACK,
	/* BUTT_L_THUMB_B */    XINPUT_GAMEPAD_LEFT_THUMB,
	/* BUTT_R_THUMB_B */    XINPUT_GAMEPAD_RIGHT_THUMB,
	/* BUTT_LB        */    XINPUT_GAMEPAD_LEFT_SHOULDER,
	/* BUTT_RB        */    XINPUT_GAMEPAD_RIGHT_SHOULDER,
	/* BUTT_A         */    XINPUT_GAMEPAD_A,
	/* BUTT_B         */    XINPUT_GAMEPAD_B,
	/* BUTT_X         */    XINPUT_GAMEPAD_X,
	/* BUTT_Y         */    XINPUT_GAMEPAD_Y
	/* BUTT_L_THUMB_UP */	
	/* BUTT_L_THUMB_DOWN */
};



void controller_normalize_deadzone_left(void);

int FASTCALL
controller_init(void)
{
	DWORD connect;
	int i;
	connected = 0;
	for(controller_selected = 0; controller_selected < 4; controller_selected++)
	{
		ZeroMemory( &controller_state[controller_selected], sizeof(XINPUT_STATE) );
		connect = XInputGetState(controller_selected, &controller_state[controller_selected]);
		if(connect == ERROR_SUCCESS){
			for(i = 0; i < BUTT_MAX; i++){
				if(controller_state[controller_selected].Gamepad.wButtons & butt_codes[10])
					button_states[controller_selected][i] = BUTT_STATE_PRESSED;
				else
					button_states[controller_selected][i] = BUTT_STATE_UP;
			}
			connected += (uint8_t)pow(2.0,controller_selected);
			button_states[controller_selected][BUTT_L_THUMB_UP] = BUTT_STATE_UP;
			button_states[controller_selected][BUTT_L_THUMB_DOWN] = BUTT_STATE_UP;
			controller_normalize_deadzone_left();
		}

	}

	controller_selected = 0;
	return connected;
}

int FASTCALL
controller_end(void)
{
	return 0;
}

/*This function checks to see which controllers are currently connected*/
int FASTCALL
controller_check_connections(void)
{
	DWORD connect;
	int i;
	connected = 0;
	for(controller_selected = 0; controller_selected < 4; controller_selected++)
	{
		ZeroMemory( &controller_state[controller_selected], sizeof(XINPUT_STATE) );
		connect = XInputGetState(controller_selected, &controller_state[controller_selected]);
		if(connect == ERROR_SUCCESS){
			for(i = 0; i < BUTT_MAX; i++){
				if(controller_state[controller_selected].Gamepad.wButtons & butt_codes[10])
					button_states[controller_selected][i] = BUTT_STATE_PRESSED;
				else
					button_states[controller_selected][i] = BUTT_STATE_UP;
			}
			connected += (uint8_t)pow(2.0,controller_selected);
			controller_normalize_deadzone_left();
		}

	}
	controller_selected = 0;
	return connected;
}

/*Updates button states of the controller buttons*/
int FASTCALL
controller_update(void)
{
	int i;
	DWORD connect;
	memset(&controller_state[controller_selected], 0, sizeof(XINPUT_STATE)); 
	connect = XInputGetState(controller_selected, &controller_state[controller_selected]);
	if(connect == ERROR_SUCCESS){
		for(i = 0; i < BUTT_MAX; i++){
			if(controller_state[controller_selected].Gamepad.wButtons & butt_codes[i]){
				if(button_states[controller_selected][i] == BUTT_STATE_UP){
					button_states[controller_selected][i] = BUTT_STATE_PRESSED;
					if(!updated_controller)
						updated_controller = controller_selected + 1;
				}
				else
					button_states[controller_selected][i] = BUTT_STATE_DOWN;
			}
			else{
				if(button_states[controller_selected][i] == BUTT_STATE_DOWN)
					button_states[controller_selected][i] = BUTT_STATE_RELEASED;
				else
					button_states[controller_selected][i] = BUTT_STATE_UP;
			}
			
		}
		controller_normalize_deadzone_left();
		return 0;
	}
	return -1;
}

int FASTCALL
controller_updated(void)
{
	return updated_controller;
}

int FASTCALL
controller_update_all(void)
{	
	int old_selected = controller_selected;
	updated_controller = 0;
	if(controller_select(CONTROLLER_1))
		controller_update();
	if(controller_select(CONTROLLER_2))
		controller_update();
	if(controller_select(CONTROLLER_3))
		controller_update();
	if(controller_select(CONTROLLER_4))
		controller_update();
	controller_selected = old_selected;
	return 0;
}



int FASTCALL
controller_clear(void)
{
	return 0;
}

uint16_t FASTCALL
controller_button_check(uint8_t code)
{
	return button_states[controller_selected][code];
}

int FASTCALL
controller_button_pressed(uint8_t code)
{
	return (button_states[controller_selected][code] == BUTT_STATE_PRESSED);
}

int FASTCALL
controller_button_down(uint8_t code)
{
	return (button_states[controller_selected][code] == BUTT_STATE_DOWN);
}

int FASTCALL
controller_button_released(uint8_t code)
{
	return (button_states[controller_selected][code] == BUTT_STATE_RELEASED);
}

int FASTCALL
controller_button_up(uint8_t code)
{
	return (button_states[controller_selected][code] == BUTT_STATE_UP);
}

float FASTCALL
controller_left_stick_x(void)
{
	return controller_lstick_x[controller_selected];
}
float FASTCALL
controller_left_stick_y(void)
{
	return controller_lstick_y[controller_selected];
}

int FASTCALL
controller_connected(void)
{
	return connected;
}

/*used to select which controller is to be used or checked currently*/
int FASTCALL
controller_select(uint8_t controller)
{
	if(controller & connected){
		if(controller == CONTROLLER_1)
			controller_selected = 0;
		else if(controller == CONTROLLER_2)
			controller_selected = 1;
		else if(controller == CONTROLLER_3)
			controller_selected = 2;
		else if(controller == CONTROLLER_4)
			controller_selected = 3;
		return 1;
	}
	return 0;
}

/*Calling this function will rumble the controller.*/
int FASTCALL
controller_rumble(int side)
{
	XINPUT_VIBRATION rumble;
	ZeroMemory( &rumble, sizeof(XINPUT_VIBRATION));
	if(side==CONTROLLER_RUMBLE_LEFT){
		rumble.wLeftMotorSpeed = 32000;
		rumble.wRightMotorSpeed = 0;
	}
	else{
		rumble.wRightMotorSpeed = 30000;
		rumble.wLeftMotorSpeed = 0;
	}
	XInputSetState(controller_selected, &rumble);
	return 1;
}

int FASTCALL
controller_rumble_stop(void)
{
	XINPUT_VIBRATION rumble;
	ZeroMemory( &rumble, sizeof(XINPUT_VIBRATION));
		rumble.wLeftMotorSpeed = 0;
		rumble.wRightMotorSpeed = 0;
	XInputSetState(controller_selected, &rumble);
	return 1;
}

/*
 * Internal Controller Functions
 */

/*Deals with the deadzone of the controller, used Microsoft recommended values.*/

void controller_normalize_deadzone_left(void)
{
	float l_x,l_y,mag,norm_mag;
	l_x = controller_state[controller_selected].Gamepad.sThumbLX;
	l_y = controller_state[controller_selected].Gamepad.sThumbLY;

	mag = (float) sqrt(l_x * l_x + l_y * l_y);
	controller_lstick_x[controller_selected] = l_x / mag;
	controller_lstick_y[controller_selected] = l_y / mag;
	
	norm_mag = 0;

	if(mag > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE){

		if(mag > 32767)
			mag = 32767;
		mag -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		norm_mag = mag / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else{
	
		mag = 0.0;
		norm_mag = 0.0;
	}

	/*The following is where the stick presses are translated into button press style 
	to provide an easy check to just see if a control stick is pressed and which direction.*/
	controller_lstick_x[controller_selected] *= norm_mag;
	controller_lstick_y[controller_selected] *= norm_mag;
	if((button_states[controller_selected][BUTT_L_THUMB_UP] == BUTT_STATE_UP) && (controller_lstick_y[controller_selected] > 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_UP] = BUTT_STATE_PRESSED;
	else if((button_states[controller_selected][BUTT_L_THUMB_UP] == BUTT_STATE_PRESSED) && (controller_lstick_y[controller_selected] > 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_UP] = BUTT_STATE_DOWN;
	else if(button_states[controller_selected][BUTT_L_THUMB_UP] && (controller_lstick_y[controller_selected] < 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_UP] = BUTT_STATE_UP;

	if(!button_states[controller_selected][BUTT_L_THUMB_DOWN] && (controller_lstick_y[controller_selected] < -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_DOWN] = BUTT_STATE_PRESSED;
	else if(button_states[controller_selected][BUTT_L_THUMB_DOWN] && (controller_lstick_y[controller_selected] < -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_DOWN] = BUTT_STATE_DOWN;
	else if(button_states[controller_selected][BUTT_L_THUMB_DOWN] && (controller_lstick_y[controller_selected] > -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_DOWN] = BUTT_STATE_UP;

	if((button_states[controller_selected][BUTT_L_THUMB_RIGHT] == BUTT_STATE_UP) && (controller_lstick_x[controller_selected] > 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_RIGHT] = BUTT_STATE_PRESSED;
	else if((button_states[controller_selected][BUTT_L_THUMB_RIGHT] == BUTT_STATE_PRESSED) && (controller_lstick_x[controller_selected] > 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_RIGHT] = BUTT_STATE_DOWN;
	else if(button_states[controller_selected][BUTT_L_THUMB_RIGHT] && (controller_lstick_x[controller_selected] < 0.5f))
		button_states[controller_selected][BUTT_L_THUMB_RIGHT] = BUTT_STATE_UP;

	if(!button_states[controller_selected][BUTT_L_THUMB_LEFT] && (controller_lstick_x[controller_selected] < -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_LEFT] = BUTT_STATE_PRESSED;
	else if(button_states[controller_selected][BUTT_L_THUMB_LEFT] && (controller_lstick_x[controller_selected] < -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_LEFT] = BUTT_STATE_DOWN;
	else if(button_states[controller_selected][BUTT_L_THUMB_LEFT] && (controller_lstick_x[controller_selected] > -0.5f))
		button_states[controller_selected][BUTT_L_THUMB_LEFT] = BUTT_STATE_UP;
}	

#endif /* WINDOWS */