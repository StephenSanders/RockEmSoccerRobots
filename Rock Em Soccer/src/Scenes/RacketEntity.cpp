#include <Foundation/Resource.h>
#include <Foundation/Game.h>

#include <engine/engine.h>
#include <engine/sound.h>
#include <engine/keyboard.h>
#include <engine/controller.h>
#include "RacketEntity.h"
#include <time.h>


void RacketEntity::start()
{
	model.addMesh(mesh);
	shadow = Resource::loadSprite("sprite/shadow.png");
	boostSound = Resource::loadSound("sound/boost.wav");
	velocity.x=0.0f;
	velocity.z=0.0f;
	velocity.y=0.0f;
	punched=0;
	boost = 0;
	jumping=0;
	rumbleTime = 0.0f;
	scaleUniform(0.5f);

	state = RacketEntity::STATE_NONE;
}

void RacketEntity::end()
{
	Resource::releaseSprite(shadow);
	Resource::releaseMesh(mesh);
	Resource::releaseSound(boostSound);
}

void RacketEntity::update()
{
	if(jumping){
		velocity.y -= RACKET_ACCELERATION * 0.5f * time_delta;
	}

	if(time_current - punching >= 0.25){
		punching = 0.0f;
	}
	if(jumping && position.y < 0.0f){
		position.y = 0.0f;
		velocity.y = 0.0f;
		jumping = 0;
	}
	controller_select(useController);
	if(punched && useController){
		controller_rumble(CONTROLLER_RUMBLE_RIGHT);
		rumbleTime = time_current;
	}
	if(rumbleTime > 0.0f && (time_current - rumbleTime)  > 0.4f){
		controller_rumble_stop();
		rumbleTime = 0.0f;
	}
	lastPos=position;
	/* update position and deal with jumping and boosting with keyboard*/
	if(!useController){
		if(!jumping && !punching && !punched && key_down(keyJump)){
			velocity.y += RACKET_ENTITY_SPEED * 2.0f;
			jumping = 1;
		}
		if(!boost && !punching && !punched && ((*boostBar) >= 0.5f) && key_pressed(keyBoost)){
			velocity.x += velocity.x * 2.0f;
			velocity.z += velocity.z * 2.0f;
			boost = 1;
			*boostBar -= 0.5f;
			sound_play(boostSound, 0);
		}
		if(key_down(keyDown)){
			if(velocity.z < RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.z += RACKET_ACCELERATION * time_delta;
		}
		if(key_down(keyUp)){
			if(velocity.z > -RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.z -= RACKET_ACCELERATION * time_delta;
		
		}
		if(key_down(keyLeft)){
		
			if(velocity.x < RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.x += RACKET_ACCELERATION * time_delta;
		}
		if(key_down(keyRight)){
			if(velocity.x > -RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.x -= RACKET_ACCELERATION * time_delta;
		}
		if(!key_down(keyLeft)  && !key_down(keyRight)){
			if(velocity.x > 0){
				velocity.x -=  RACKET_ACCELERATION * time_delta;
				if(velocity.x < 0){
					velocity.x = 0;
				}
			}
			if(velocity.x < 0){
				velocity.x +=  RACKET_ACCELERATION * time_delta;
				if(velocity.x > 0){
					velocity.x = 0;
				}

			}
		}
		if(!key_down(keyDown) && !key_down(keyUp)){
			if(velocity.z > 0){
				velocity.z -=  RACKET_ACCELERATION * time_delta;
				if(velocity.z < 0){
					velocity.z = 0;
				}

			}
	
			if(velocity.z < 0){
				velocity.z +=  RACKET_ACCELERATION * time_delta;
				if(velocity.z > 0){
					velocity.z = 0;
				}
				if(punched)
					if(velocity.z >= -RACKET_ENTITY_SPEED)
						punched=0;

			}
		}
	}/*deal with jumping and boosting and moving with controller*/
	else{
		if(!jumping && !punched && !punching && controller_button_down(BUTT_A)){
			velocity.y += RACKET_ENTITY_SPEED * 2.0f;
			jumping = 1;
		}
		if(!boost && !punched && !punching && (*boostBar >= 0.5f) && controller_button_pressed(BUTT_X)){
			velocity.x += velocity.x * 2.0f;
			velocity.z += velocity.z * 2.0f;
			boost = 1;
			*boostBar -= 0.5f;
			sound_play(boostSound, 0);
		}
		if(controller_button_down(BUTT_D_DOWN)){
			if(velocity.z < RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.z += RACKET_ACCELERATION * time_delta;
		}
		if(controller_button_down(BUTT_D_UP)){
			if(velocity.z > -RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.z -= RACKET_ACCELERATION * time_delta;
		
		}
		if(controller_button_down(BUTT_D_LEFT)){
		
			if(velocity.x < RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.x += RACKET_ACCELERATION * time_delta;
		}
		if(controller_button_down(BUTT_D_RIGHT)){
			if(velocity.x > -RACKET_ENTITY_SPEED && !punched && !punching)
				velocity.x -= RACKET_ACCELERATION * time_delta;
		}
		if(!controller_button_down(BUTT_D_LEFT)  && !controller_button_down(BUTT_D_RIGHT)){
			float analogX = controller_left_stick_x();
			if(analogX == 0 || punched){
				if(velocity.x > 0){
					velocity.x -=  RACKET_ACCELERATION * (2.0f) * time_delta;
					if(velocity.x < 0){
						velocity.x = 0;
					}
				}
				if(velocity.x < 0){
					velocity.x +=  RACKET_ACCELERATION * (2.0f) * time_delta;
					if(velocity.x > 0){
						velocity.x = 0;
					}

				}
			}
			if(!punched && !punching &&!boost)
				velocity.x += RACKET_ACCELERATION * (-1.75f) * analogX * time_delta;

		}
		if(!controller_button_down(BUTT_D_DOWN) && !controller_button_down(BUTT_D_UP)){
			float analogY = controller_left_stick_y();
			if(analogY == 0 || punched){
				if(velocity.z > 0){
					velocity.z -=  RACKET_ACCELERATION * (2.0f) * time_delta;
					if(velocity.z < 0){
						velocity.z = 0;
					}

				}
	
				if(velocity.z < 0){
					velocity.z +=  RACKET_ACCELERATION * (2.0f) * time_delta;
					if(velocity.z > 0){
						velocity.z = 0;
					}
					if(punched)
						if(velocity.z >= -RACKET_ENTITY_SPEED)
							punched=0;

				}
			}
			if(!punched && !punching && !boost)
					velocity.z += RACKET_ACCELERATION * (-1.75f) * analogY * time_delta;
		}
	}
	if(velocity.x > RACKET_ENTITY_SPEED && !punched && !boost)
		velocity.x = RACKET_ENTITY_SPEED;
	else if(velocity.x < -RACKET_ENTITY_SPEED && !punched && !boost)
		velocity.x = -RACKET_ENTITY_SPEED;
	if(velocity.z > RACKET_ENTITY_SPEED && !punched && !boost)
		velocity.z = RACKET_ENTITY_SPEED;
	else if(velocity.z < -RACKET_ENTITY_SPEED && !punched && !boost)
		velocity.z = -RACKET_ENTITY_SPEED;
	if(abs(velocity.x) - RACKET_ENTITY_SPEED <= 0 && abs(velocity.z) - RACKET_ENTITY_SPEED <= 0){
		punched = 0;
		boost = 0;
	}

		if(boost || punched){
				if(velocity.x > 0){
					velocity.x -=  RACKET_ACCELERATION  * (1.5f) * time_delta;
					if(velocity.x < 0){
						velocity.x = 0;
					}
				}
				if(velocity.x < 0){
					velocity.x +=  RACKET_ACCELERATION  * (1.5f) * time_delta;
					if(velocity.x > 0){
						velocity.x = 0;
					}

				}
				if(velocity.z > 0){
					velocity.z -=  RACKET_ACCELERATION  * (1.5f) * time_delta;
					if(velocity.z < 0){
						velocity.z = 0;
					}

				}
	
				if(velocity.z < 0){
					velocity.z +=  RACKET_ACCELERATION  * (1.5f) * time_delta;
					if(velocity.z > 0){
						velocity.z = 0;
					}


				}
			}
		if(velocity.z >= -RACKET_ENTITY_SPEED)
			if(velocity.z <= RACKET_ENTITY_SPEED)
				if(velocity.x <= RACKET_ENTITY_SPEED)
					if(velocity.x >= -RACKET_ENTITY_SPEED){
						boost = 0;
						punched = 0;
					}
	position.x += velocity.x * time_delta;
	position.z += velocity.z * time_delta;
	position.y += velocity.y * time_delta;
	
	if (punching > 0.0f && state != STATE_PUNCHING) {
		mesh->setSpeed(3.0f);
		mesh->setAnimation(1, 100.0f);
		state = STATE_PUNCHING;
	} else if (abs(velocity.x) > 0 || abs(velocity.z) > 0) {
		setRotation(-57.2957795f * atan2(velocity.x, -velocity.z), 0.0f, 1.0f, 0.0f);
		
		if (state != STATE_RUN) {
			mesh->setSpeed(1.6f);
			mesh->setAnimation(3);
			state = STATE_RUN;
		}
	} else if (state != STATE_IDLE) {
		mesh->setSpeed(1.0f);
		mesh->setAnimation(2);
		state = STATE_IDLE;
	}
}
void RacketEntity::render()
{
	Graphics::loadIdentity();
	Graphics::translate(position.x - 0.05f, 0.05f, position.z);
	Graphics::scale(0.025f, 1.0f, 0.03f);
	Graphics::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	shadow->render(0);
	model.render();
}
