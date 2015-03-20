#include <Foundation/Resource.h>

#include <engine/engine.h>
#include <engine/sound.h>
#include <engine/keyboard.h>
#include "BallEntity.h"
#include "RacketEntity.h"
#include <cmath>
#include <climits>


void BallEntity::start()
{
	bounceSound[0] = Resource::loadSound("sound/bounce1.wav");
	bounceSound[1] = Resource::loadSound("sound/bounce2.wav");
	bounceSound[2] = Resource::loadSound("sound/bounce3.wav");
	bounceSound[3] = Resource::loadSound("sound/bounce4.wav");


	mesh = Resource::loadMesh("model/ball.x");
	model.addMesh(mesh);

	shadow = Resource::loadSprite("sprite/shadow.png");

	velocity.x = 0.0f;

	position = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	scaleUniform(0.75f);
}

void BallEntity::end()
{
	Resource::releaseSprite(shadow);
	Resource::releaseMesh(mesh);
	Resource::releaseSound(bounceSound[0]);
	Resource::releaseSound(bounceSound[1]);
	Resource::releaseSound(bounceSound[2]);
	Resource::releaseSound(bounceSound[3]);
}

void BallEntity::update()
{
	//if (key_pressed(KEY_LCTRL))
	//	reset(); 

	/* update position */
	if(velocity.x > 0){
		velocity.x -= BALL_DECCELERATION * time_delta;
		if(velocity.x < 0)
			velocity.x = 0;
	}
	if(velocity.x < 0){
		velocity.x += BALL_DECCELERATION * time_delta;
		if(velocity.x > 0)
			velocity.x = 0;
	}
	if(velocity.z > 0){
		velocity.z -= BALL_DECCELERATION * time_delta;
		if(velocity.z < 0)
			velocity.z = 0;
	}
	if(velocity.z < 0){
		velocity.z += BALL_DECCELERATION * time_delta;
		if(velocity.z > 0)
			velocity.z = 0;
	}

	if(position.y > 0.5f){
		velocity.y -= BALL_DECCELERATION * 5.0f * time_delta;
	}
	else if(position.y <= 0.5f){
		position.y = 0.5f;
		if(kickUp && velocity.y < 0.0f){
			velocity.y *= 0.75f;
			sound_play(bounceSound[((int)((((float)rand())/((float)RAND_MAX)) * 3.99f))],0);
			if(abs(velocity.y) >= 1.0f){
				velocity.y *= -1.0f;
			}
			else{
			kickUp=0;
			velocity.y = 0.0f;
			}
		}
	}

	if(velocity.x > BALL_MAX_SPEED)
		velocity.x = BALL_MAX_SPEED;
	if(velocity.z > BALL_MAX_SPEED)
		velocity.z = BALL_MAX_SPEED;
	if(velocity.x < (-1 * (BALL_MAX_SPEED)))
		velocity.x = -1 * (BALL_MAX_SPEED);
	if(velocity.z < (-1.0f * (BALL_MAX_SPEED)))
		velocity.z = -1.0f * (BALL_MAX_SPEED);
	if(velocity.y > (BALL_MAX_SPEED / 1.5f))
		velocity.y = BALL_MAX_SPEED / 1.5f;
	if(velocity.y < -(BALL_MAX_SPEED / 1.5f))
		velocity.y = -1.0f * (BALL_MAX_SPEED / 1.5f);

	position.z += velocity.z * time_delta;
	position.x += velocity.x * time_delta;
	position.y += velocity.y * time_delta;


	rotate(-velocity.z/16.0f, 0.0f, -velocity.x/16.0f);

	if(position.y <= 0.5f){
		position.y = 0.5f;

	}

}

void BallEntity::render()
{
	if(visible){
	Graphics::loadIdentity();
	Graphics::translate(position.x - 0.1f, 0.05f, position.z);
	Graphics::scale(0.015f - (0.0005 * position.y));
	Graphics::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	shadow->render(0);
	model.render();
	}
}

void BallEntity::reset()
{
	velocity.x = 0.0f;
	velocity.z = 0.0f;
	velocity.y = 0.0f;

	position = D3DXVECTOR3(0.0f, 0.5f, 0.0f); 
}