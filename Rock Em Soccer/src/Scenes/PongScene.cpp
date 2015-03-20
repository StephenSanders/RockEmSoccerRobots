/*
* PongScene.cpp
* Stephen Sanders & Jake Singer
*/

#include <Foundation/Resource.h>
#include <Foundation/Game.h>

#include "PongScene.h"
#include "PongHUDScene.h"
#include <engine/engine.h>
#include <engine/sound.h>
#include <engine/keyboard.h>
#include <engine/controller.h>
#include <cmath>
#define MAX_LOOP_CYCLES 1000

void PongScene::start()
{
	HUDScene = new PongHUDScene();
	Game::pushScene(HUDScene);
	HUDScene->visible = false;
	Game::state.blueScore  = 0;
	Game::state.redScore = 0;
	Game::state.blueBoost = 1.0f;
	Game::state.redBoost = 1.0f;
	goal = 0.0f;


	p1Goal[0] = Resource::loadSound("sound/p1Score.wav");
	p1Goal[1] = Resource::loadSound("sound/p1Score2.wav");
	p1Goal[2] = Resource::loadSound("sound/p1Score3.wav");
	p2Goal[0] = Resource::loadSound("sound/p2Score.wav");
	p2Goal[1] = Resource::loadSound("sound/p2Score2.wav");
	p2Goal[2] = Resource::loadSound("sound/p2Score3.wav");
	p1Win = Resource::loadSound("sound/p1Win.wav");
	p2Win = Resource::loadSound("sound/P2Win.wav");
	punchSound = Resource::loadSound("sound/punch.wav");
	Game::state.music2 = Resource::loadSound("music/track2.wav");
	Game::state.music3 = Resource::loadSound("music/track3_quiet.wav");
	Game::state.music4 = Resource::loadSound("music/track4_quiet.wav");

	fieldMesh = Resource::loadMesh("model/field.x");
	field.addMesh(fieldMesh);
	controller_check_connections();
	racket[0] = new RacketEntity();
	racket[0]->boostBar = &Game::state.blueBoost;
	if(controller_connected() != CONTROLLER_1_2 && controller_connected() != CONTROLLER_1_2_3_4){
		racket[0]->keyUp=KEY_UP;
		racket[0]->keyDown=KEY_DOWN;
		racket[0]->keyLeft=KEY_LEFT;
		racket[0]->keyRight=KEY_RIGHT;
		racket[0]->keyPunch=KEY_X;
		racket[0]->keyJump=KEY_SPACE;
		racket[0]->keyKick=KEY_Z;
		racket[0]->keyBoost=KEY_LSHIFT;
		racket[0]->useController = 0;
	}
	else
		racket[0]->useController = CONTROLLER_1;

	racket[0]->mesh = Resource::loadMesh("model/mech_blue.x");
	addEntity(racket[0]);
	numPlayers++;

	
	/*These keys for racket[1] were for testing purposes,
	Now the game does not allow you to launch it without having at least  1 controller plugged in.*/
	racket[1] = new RacketEntity();
	racket[1]->keyDown=KEY_S;
	racket[1]->keyUp=KEY_W;
	racket[1]->keyLeft=KEY_A;
	racket[1]->keyRight=KEY_D;
	racket[1]->keyPunch=KEY_P;
	racket[1]->keyJump=KEY_RSHIFT;
	racket[1]->keyBoost=KEY_ENTER;
	racket[1]->keyKick=KEY_P;
	racket[1]->boostBar=&Game::state.redBoost;
	if(racket[0]->useController != CONTROLLER_1 && controller_connected() & CONTROLLER_1)
		racket[1]->useController = CONTROLLER_1;
	else if(racket[0]->useController == CONTROLLER_1)
		racket[1]->useController = CONTROLLER_2;
	else
		racket[1]->useController = 0;
	racket[1]->mesh = Resource::loadMesh("model/mech_red.x");
	addEntity(racket[1]);
	numPlayers++;
	ball = new BallEntity();
	addEntity(ball);
	/* set up initial positions */
	reset();

	/* camera */
	camera.setProjection(Camera::perspectiveProjection);
	camera.setFOV(12.0f);

	/* light */ {
		light1.setType(D3DLIGHT_DIRECTIONAL);
		light1.setDiffuse(D3DXCOLOR(1.0f, 0.8f, 0.7f, 1.0f));
		light1.setDirection(D3DXVECTOR3(-0.5f, -1.0f, -0.75f));
		light1.enable();
	}
}

/*this function allows the game to set the number of players playing from 2 or 4)*/
void PongScene::changePlayers(int numPlay)
{
	if(controller_connected() != CONTROLLER_1_2 && controller_connected() != CONTROLLER_1_2_3_4){
		racket[0]->keyUp=KEY_UP;
		racket[0]->keyDown=KEY_DOWN;
		racket[0]->keyLeft=KEY_LEFT;
		racket[0]->keyRight=KEY_RIGHT;
		racket[0]->keyPunch=KEY_X;
		racket[0]->keyJump=KEY_SPACE;
		racket[0]->keyKick=KEY_Z;
		racket[0]->keyBoost=KEY_LSHIFT;
		racket[0]->useController = 0;
	}
	else
		racket[0]->useController = CONTROLLER_1;
	/*These keys for racket[1] were for testing purposes,
	Now the game does not allow you to launch it without having at least  1 controller plugged in.*/
	racket[1]->keyDown=KEY_S;
	racket[1]->keyUp=KEY_W;
	racket[1]->keyLeft=KEY_A;
	racket[1]->keyRight=KEY_D;
	racket[1]->keyPunch=KEY_P;
	racket[1]->keyJump=KEY_RSHIFT;
	racket[1]->keyBoost=KEY_ENTER;
	racket[1]->keyKick=KEY_P;
	if(racket[0]->useController != CONTROLLER_1 && controller_connected() & CONTROLLER_1)
		racket[1]->useController = CONTROLLER_1;
	else if(racket[0]->useController == CONTROLLER_1)
		racket[1]->useController = CONTROLLER_2;
	else
		racket[1]->useController = 0;

	/*Allows for 4 player using 3 controllers*/
	if(controller_connected() == CONTROLLER_1_2_3)
	{
		racket[2] = new RacketEntity();
		racket[2]->useController = CONTROLLER_2;
		racket[2]->mesh = Resource::loadMesh("model/mech_blue.x");
		racket[2]->boostBar=&Game::state.blueBoost;
		addEntity(racket[2]);
		numPlayers++;
		racket[3] = new RacketEntity();
		racket[3]->useController = CONTROLLER_3;
		racket[3]->mesh = Resource::loadMesh("model/mech_red.x");
		racket[3]->boostBar=&Game::state.redBoost;
		addEntity(racket[3]);
		numPlayers++;
	}

	/*allows for 4 player using 4 controllers*/
	else if(controller_connected() == CONTROLLER_1_2_3_4)
	{
		racket[2] = new RacketEntity();
		racket[2]->useController = CONTROLLER_3;
		racket[2]->mesh = Resource::loadMesh("model/mech_blue.x");
		racket[2]->boostBar=&Game::state.blueBoost;
		addEntity(racket[2]);
		numPlayers++;
		racket[3] = new RacketEntity();
		racket[3]->useController = CONTROLLER_4;
		racket[3]->mesh = Resource::loadMesh("model/mech_red.x");
		racket[3]->boostBar=&Game::state.redBoost;
		addEntity(racket[3]);
		numPlayers++;
	}
	else{
		if(racket[2]){
			removeEntity(racket[2]);
			numPlayers--;
		}
		if(racket[3]){
			removeEntity(racket[3]);
			numPlayers--;
		}
	}
	reset();

}

void PongScene::end()
{
	Scene::end(); /* releases all entities */

	Resource::releaseSound(p1Goal[0]);
	Resource::releaseSound(p1Goal[1]);
	Resource::releaseSound(p1Goal[2]);
	Resource::releaseSound(p2Goal[0]);
	Resource::releaseSound(p2Goal[1]);
	Resource::releaseSound(p2Goal[2]);
	Resource::releaseSound(p1Win);
	Resource::releaseSound(p2Win);
	Resource::releaseSound(punchSound);
	Resource::releaseSound(Game::state.music2);
	Resource::releaseSound(Game::state.music3);
	Resource::releaseSound(Game::state.music4);


	Resource::releaseMesh(fieldMesh);

	light1.disable();
}

void PongScene::update()
{
	Scene::update();
	collisionTest();
	if(Game::state.playState == PLAY_INGAME)
		HUDScene->visible = true;
	/*this allows a period of time after a goal is scored before it resets the field.*/
	if(goal && (time_current - goal) >= 2.5f){
		goal = 0.0f;
		reset();
	}
}

void PongScene::render()
{
	Light::ambient(D3DCOLOR_XRGB(40, 40, 90));
	/* basic zoom */
	float distanceX = abs(racket[0]->position.x - racket[1]->position.x);
	
	float distanceZ = 1.5f * abs(racket[0]->position.z - racket[1]->position.z);

	if(numPlayers == 4){
		float distanceX2 = abs(racket[0]->position.x - racket[2]->position.x);
		float distanceX3 = abs(racket[0]->position.x - racket[3]->position.x);
		float distanceX4 = abs(racket[1]->position.x - racket[2]->position.x);
		float distanceX5 = abs(racket[0]->position.x - racket[3]->position.x);
		float distanceX6 = abs(racket[3]->position.x - racket[4]->position.x);

		float distanceZ2 = abs(racket[0]->position.z - racket[2]->position.z);
		float distanceZ3 = abs(racket[0]->position.z - racket[3]->position.z);
		float distanceZ4 = abs(racket[1]->position.z - racket[2]->position.z);
		float distanceZ5 = abs(racket[0]->position.z - racket[3]->position.z);
		float distanceZ6 = abs(racket[3]->position.z - racket[4]->position.z);

		distanceX = max(distanceX, distanceX2);
		distanceX = max(distanceX, distanceX3);
		distanceX = max(distanceX, distanceX4);
		distanceX = max(distanceX, distanceX5);
		distanceX = max(distanceX, distanceX6);

		distanceZ = max(distanceZ, distanceZ2);
		distanceZ = max(distanceZ, distanceZ2);
		distanceZ = max(distanceZ, distanceZ2);
		distanceZ = max(distanceZ, distanceZ2);
		distanceZ = max(distanceZ, distanceZ2);

	}
	float distance = max(distanceX, distanceZ);
	distance = max(16, distance);
	distance = min(100, distance);
	camera.setFOV(10.0f + (distance/8.0f));
	float xVal = ball->position.x;
	float zVal = ball->position.z;
	for(int i = 0; i<numPlayers;i++){
		xVal += racket[i]->position.x;
		zVal += racket[i]->position.z;
	}
	camera.bind();
	camera.lookAt(
		D3DXVECTOR3(((xVal) / (numPlayers + 1.0f)), 120.0f, 100.0f),
		D3DXVECTOR3(((xVal) / (numPlayers + 1.0f)), 0.0f, ((zVal) / (numPlayers + 1.0f))),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f)
	);

	/* render field */
	D3DXQUATERNION rotation;
	D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXToRadian(180));
	field.render(D3DXVECTOR3(0.0f, 0.0f, 0.0f), rotation);
	
	/* render entities */
	Scene::render();
}

/*this is where the physics happens*/
void PongScene::collisionTest()
{
	/*deals with collisions for every player*/
	for(int i=0; i<numPlayers; i++){
		if(racket[i]->position.x > 44.5f){
			racket[i]->position.x = 44.5f;
		}
		if(racket[i]->position.x < -44.5f){
			racket[i]->position.x = -44.5f;
		}
		if(racket[i]->position.z > 25.5f){
			racket[i]->position.z = 25.5f;
		}
		if(racket[i]->position.z < -25.5f){
			racket[i]->position.z = -25.5f;
		}

		controller_select(racket[i]->useController);
		/*first checks collisions with other players, then the players will slide around each other to an extent if they do collide*/
		for(int j=0; j<numPlayers;j++){
			if(j!=i){
				if(abs(racket[j]->position.x-racket[i]->position.x)<3.0f&&abs(racket[j]->position.z-racket[i]->position.z)<3.0f&&abs(racket[j]->position.y-racket[i]->position.y)<5.0f){

					
					int loop = 0;
					while(abs(racket[j]->position.x-racket[i]->position.x)<3.0f&&abs(racket[j]->position.z-racket[i]->position.z)<3.0f&&abs(racket[j]->position.y-racket[i]->position.y)<5.0f){
						racket[j]->position.x -= racket[j]->velocity.x * time_delta;
						racket[j]->position.z -= racket[j]->velocity.z * time_delta;
					
						racket[i]->position.x -= racket[i]->velocity.x * time_delta;
						racket[i]->position.z -= racket[i]->velocity.z * time_delta;

						if((abs(racket[i]->velocity.x) - abs(racket[j]->velocity.x)) == 0){
							if(racket[i]->position.z > racket[j]->position.z){
								racket[i]->position.z += 10.0f * time_delta;
								racket[j]->position.z -= 10.0f * time_delta;
							}
							else{
								racket[j]->position.z += 10.0f * time_delta;
								racket[i]->position.z -= 10.0f * time_delta;
							}
						}

						if(((abs(racket[i]->velocity.z) - abs(racket[j]->velocity.z)) == 0)|| abs(racket[j]->position.y-racket[i]->position.y)<5.0f ){
							if(racket[i]->position.x > racket[j]->position.x){
								racket[i]->position.x += 10.0f * time_delta;
								racket[j]->position.x -= 10.0f * time_delta;
							}
							else{
								racket[j]->position.x += 10.0f * time_delta;
								racket[i]->position.x -= 10.0f * time_delta;
							}
						}
						loop++;
						if(loop > MAX_LOOP_CYCLES)
							break;
					}
				}
				/*This deals with punching a player*/
				if((abs(racket[j]->position.x-racket[i]->position.x)<4.0f&&abs(racket[j]->position.z-racket[i]->position.z)<4.0f)&&((!racket[i]->useController && key_pressed(racket[i]->keyPunch)) || (racket[i]->useController && controller_button_pressed(BUTT_B)))){
					
						if(abs(racket[i]->velocity.x) > 0.0f)
							racket[j]->velocity.x += RACKET_ENTITY_SPEED  * 3.5f * (racket[i]->velocity.x/(abs(racket[i]->velocity.x)));
						else
							racket[j]->velocity.x -= RACKET_ENTITY_SPEED * 3.5f * ((racket[i]->position.x - racket[j]->position.x)/(sqrt(pow((racket[i]->position.x - racket[j]->position.x),2) + pow((racket[i]->position.z - racket[j]->position.z),2))));
						if(abs(racket[i]->velocity.z) > 0.0f)
							racket[j]->velocity.z += RACKET_ENTITY_SPEED * 3.5f * (racket[i]->velocity.z/(abs(racket[i]->velocity.z)));
						else
							racket[j]->velocity.z -= RACKET_ENTITY_SPEED * 3.5f *  ((racket[i]->position.z - racket[j]->position.z)/(sqrt(pow((racket[i]->position.x - racket[j]->position.x),2) + pow((racket[i]->position.z - racket[j]->position.z),2))));
						racket[j]->boost = 0;
						racket[j]->punched=1;
						racket[i]->punching = time_current;
						sound_play(punchSound, 0);
						racket[i]->velocity.x = 0.0f;
						racket[i]->velocity.z = 0.0f;
					
					}
			}
		}

		/*This deals with collision with the ball*/
		if((ball->visible)&&abs(ball->position.x-racket[i]->position.x)<1.0f&&abs(ball->position.z-racket[i]->position.z)<3.0f&&abs(ball->position.y-racket[i]->position.y)<5.0f){
			sound_play(ball->bounceSound[((int)((((float)rand())/((float)RAND_MAX)) * 3.99f))],0);
				ball->velocity.x *= -1;
				ball->velocity.x += racket[i]->velocity.x;
				ball->velocity.z *= -1;
				ball->velocity.z += racket[i]->velocity.z;
				if(ball->kickUp)
					ball->velocity.y += (racket[i]->velocity.y / 4.0f);

				int loop = 0;
			while(abs(ball->position.x-racket[i]->position.x)<1.0f&&abs(ball->position.z-racket[i]->position.z)<3.0f&&abs(ball->position.y-racket[i]->position.y)<5.0f){
				ball->position.x += ball->velocity.x * time_delta;
				ball->position.z += ball->velocity.z * time_delta;
				ball->position.y += ball->velocity.z * time_delta;

				if((ball->velocity.y == 0) && (ball->position.y-racket[i]->position.y)<5.0f ){
							if(racket[i]->position.x > ball->position.x){
								racket[i]->position.x += 10.0f * time_delta;
								ball->position.x -= 10.0f * time_delta;
							}
							else{
								ball->position.x += 10.0f * time_delta;
								racket[i]->position.x -= 10.0f * time_delta;
							}
						}
				loop++;
				if(loop > MAX_LOOP_CYCLES)
					break;
			}
		}
		if((ball->visible)&&abs(ball->position.x-racket[i]->position.x)<2.0f&&abs(ball->position.z-racket[i]->position.z)<4.0f){
				if((!racket[i]->useController && key_pressed(racket[i]->keyKick)) || (racket[i]->useController && controller_button_pressed(BUTT_Y))){
						ball->velocity.y += sqrt(pow(racket[i]->velocity.x,2) + pow(racket[i]->velocity.z,2)) + ((BALL_MAX_SPEED/2.0f) * sqrt(pow(ball->velocity.x,2) + pow(ball->velocity.z,2))/(BALL_MAX_SPEED));
						if(ball->velocity.y > (BALL_MAX_SPEED / 1.5f))
							ball->velocity.y = BALL_MAX_SPEED / 1.5f;
						else if(ball->velocity.y < BALL_MAX_SPEED / 4.0f)
							ball->velocity.y = BALL_MAX_SPEED / 4.0f;
						ball->velocity.x += racket[i]->velocity.x;
						ball->velocity.z += racket[i]->velocity.z;
						ball->kickUp = 1;
						sound_play(ball->bounceSound[((int)((((float)rand())/((float)RAND_MAX)) * 3.99f))],0);
				}
		}
	}

	/*This deals with the ball going into the goal*/
	if((ball->visible)&&ball->position.x > 44.5f){


		if(abs(ball->position.z) < 5 && ball->position.y < 10.0f)
		{
			Game::state.redScore++;
			DWORD time =  fmod((time_current - Game::state.musicStarted), 50.0f) * (44100 * 4);
			if(Game::state.redScore >= 2)
				if(!sound_playing(Game::state.music2))
					sound_play_at_time(Game::state.music2, 1, time);
			if(Game::state.redScore >= 3)
				if(!sound_playing(Game::state.music3))
					sound_play_at_time(Game::state.music3, 1, time);
			if(Game::state.redScore >= 4){
				if(!sound_playing(Game::state.music4))
					sound_play_at_time(Game::state.music4, 1, time);
				if(Game::state.blueScore >=3)
					if(sound_playing(Game::state.mainTheme))
						sound_stop(Game::state.mainTheme);
			}
			

			if(Game::state.redScore < 5){
				goal = time_current;
				sound_play(p2Goal[((int)((((float)rand())/((float)RAND_MAX)) * 3.0f)) ],0);
				ball->visible = 0;
				ball->velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
			}
			else{
				goal = time_current;
				sound_play(p2Win,0);
				Game::state.lastWon = TEAM_RED;
				Game::state.redScore = 5;
				ball->visible = 0;
				ball->velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
			}

		}
		else{
			ball->position.x = 44.5f;
			ball->velocity.x *= -1;
		}
	}
	if((ball->visible)&&ball->position.x < -44.5f){
		

		if(abs(ball->position.z) < 5 && ball->position.y < 10.0f)
		{
			Game::state.blueScore++;

			DWORD time =  fmod((time_current - Game::state.musicStarted), 50.0f) * (44100 * 4);
			if(Game::state.blueScore >= 2)
				if(!sound_playing(Game::state.music2))
					sound_play_at_time(Game::state.music2, 1, time);
			if(Game::state.blueScore >= 3)
				if(!sound_playing(Game::state.music3))
					sound_play_at_time(Game::state.music3, 1, time);
			if(Game::state.blueScore >= 4){
				if(!sound_playing(Game::state.music4))
					sound_play_at_time(Game::state.music4, 1, time);
				if(Game::state.redScore >=3)
					if(sound_playing(Game::state.mainTheme))
						sound_stop(Game::state.mainTheme);
			}

			if(Game::state.blueScore < 5){
				goal = time_current;
				sound_play(p1Goal[((int)((((float)rand())/((float)RAND_MAX)) * 3.0f))],0);
				ball->visible = 0;
				ball->velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);

			}
			else{
				sound_play(p1Win,0);
				Game::state.lastWon = TEAM_BLUE;
				Game::state.blueScore =  5;				
				ball->visible = 0;
				goal = time_current;
				ball->velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
			}

		}
		else{
			ball->position.x = -44.5f;
			ball->velocity.x *= -1;
		}
	}
	if((ball->visible)&&ball->position.z > 25.5f){
		ball->position.z = 25.5f;
		ball->velocity.z *= -1;
	}
	if((ball->visible)&&ball->position.z < -25.5f){
		ball->position.z = -25.5f;
		ball->velocity.z *= -1;
	}

}

/*This resets the field after a score*/

void PongScene::reset()
{
	D3DXQUATERNION identityRotation;
	D3DXQuaternionIdentity(&identityRotation);
	racket[0]->position = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
	racket[0]->rotation = identityRotation;
	racket[0]->rotate(90, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	racket[1]->position = D3DXVECTOR3(-15.0f, 0.0f, -0.0f);
	racket[1]->rotation = identityRotation;
	racket[1]->rotate(-90, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	if(numPlayers >= 3 ){
		racket[2]->position = D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		racket[2]->rotation = identityRotation;
		racket[2]->rotate(90, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	if(numPlayers >= 4){
		racket[3]->position = D3DXVECTOR3(-20.0f, 0.0f, 0.0f);
		racket[3]->rotation = identityRotation;
		racket[3]->rotate(-90, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	if(Game::state.blueScore == 5 || Game::state.redScore == 5){
		Game::state.playState = PLAY_FINISHED;
		Game::state.blueScore = Game::state.redScore = 0; 
		if(sound_playing(Game::state.music2))
			sound_stop(Game::state.music2);
		if(sound_playing(Game::state.music3))
			sound_stop(Game::state.music3);
		if(sound_playing(Game::state.music4))
			sound_stop(Game::state.music4);
		if(!sound_playing(Game::state.mainTheme)){
			DWORD time =  fmod((time_current - Game::state.musicStarted), 50.0f) * (44100 * 4);
			sound_play_at_time(Game::state.mainTheme, 1, time);
			Game::state.musicStarted = time_current;
		}
	}

	ball->reset();
	ball->visible = 1;
}


