/*
* MenuScene.cpp
* Stephen Sanders & Jake Singer
*/

#include "MenuScene.h"

#include <engine/keyboard.h>
#include <engine/controller.h>
#include <Foundation/Resource.h>
#include <Foundation/Game.h>

#include "PongScene.h"

#include <stdlib.h>

enum{
	SPRITE_DEFAULT = 0,
	SPRITE_BLUE = 1,
	SPRITE_RED = 2,
	
};

#define TITLE_Y_START (((float)Graphics::viewHeight/2.0f) + 30.0f)
#define TITLE_Y_MAIN (((float)Graphics::viewHeight/2.0f) + 110.0f)
#define TITLE_Y_SPEED (5.0f)

static float titleAnimator;

void MenuScene::start()	
{
	loading = true;

	gameScene = new PongScene();
	Game::insertScene(gameScene, Game::scenes.begin());

	camera.setProjection(Camera::orthographicProjection);
	menuState = MENU_STATE_START;
	Game::state.playState = PLAY_NOT_STARTED;

	titleSprite = new Sprite(3);
	titleSprite->load(Resource::contentPath + "sprite/title_empty.png", SPRITE_DEFAULT);
	titleSprite->load(Resource::contentPath + "sprite/title_blue.png", SPRITE_BLUE);
	titleSprite->load(Resource::contentPath + "sprite/title_red.png", SPRITE_RED);
	pressanykeySprite = new Sprite(1);
	pressanykeySprite->load(Resource::contentPath + "sprite/pressanykey.png", 0);
	backgroundSprite = new Sprite(1);
	backgroundSprite->load(Resource::contentPath + "sprite/background.png", 0);

	playSprite = new Sprite(3);
	playSprite->load(Resource::contentPath + "sprite/button_play_default.png", SPRITE_DEFAULT);
	playSprite->load(Resource::contentPath + "sprite/button_play_blue.png", SPRITE_BLUE);
	playSprite->load(Resource::contentPath + "sprite/button_play_red.png", SPRITE_RED);

	howToPlaySprite = new Sprite(3);
	howToPlaySprite->load(Resource::contentPath + "sprite/button_howtoplay_default.png", SPRITE_DEFAULT);
	howToPlaySprite->load(Resource::contentPath + "sprite/button_howtoplay_blue.png", SPRITE_BLUE);
	howToPlaySprite->load(Resource::contentPath + "sprite/button_howtoplay_red.png", SPRITE_RED);

	creditsSprite = new Sprite(3);
	creditsSprite->load(Resource::contentPath + "sprite/button_credits_default.png", SPRITE_DEFAULT);
	creditsSprite->load(Resource::contentPath + "sprite/button_credits_blue.png", SPRITE_BLUE);
	creditsSprite->load(Resource::contentPath + "sprite/button_credits_red.png", SPRITE_RED);

	quitSprite = new Sprite(3);
	quitSprite->load(Resource::contentPath + "sprite/button_quit_default.png", SPRITE_DEFAULT);
	quitSprite->load(Resource::contentPath + "sprite/button_quit_blue.png", SPRITE_BLUE);
	quitSprite->load(Resource::contentPath + "sprite/button_quit_red.png", SPRITE_RED);
		
	resumeSprite = new Sprite(3);
	resumeSprite->load(Resource::contentPath + "sprite/button_resume_default.png", SPRITE_DEFAULT);
	resumeSprite->load(Resource::contentPath + "sprite/button_resume_blue.png", SPRITE_BLUE);
	resumeSprite->load(Resource::contentPath + "sprite/button_resume_red.png", SPRITE_RED);

	backSprite = new Sprite(3);
	backSprite->load(Resource::contentPath + "sprite/button_back_default.png", SPRITE_DEFAULT);
	backSprite->load(Resource::contentPath + "sprite/button_back_blue.png", SPRITE_BLUE);
	backSprite->load(Resource::contentPath + "sprite/button_back_red.png", SPRITE_RED);

	controllerSprite = new Sprite(3);
	controllerSprite->load(Resource::contentPath + "sprite/button_controller_default.png",SPRITE_DEFAULT);
	controllerSprite->load(Resource::contentPath + "sprite/button_controller_blue.png",SPRITE_BLUE);
	controllerSprite->load(Resource::contentPath + "sprite/button_controller_red.png",SPRITE_RED);

	keyboardSprite = new Sprite(3);
	keyboardSprite->load(Resource::contentPath + "sprite/button_keyboard_default.png",SPRITE_DEFAULT);
	keyboardSprite->load(Resource::contentPath + "sprite/button_keyboard_blue.png",SPRITE_BLUE);
	keyboardSprite->load(Resource::contentPath + "sprite/button_keyboard_red.png",SPRITE_RED);

	controlsSprite = new Sprite(2);
	controlsSprite->load(Resource::contentPath + "sprite/controls_keyboard.png", 0);
	controlsSprite->load(Resource::contentPath + "sprite/controls_controller.png", 1);

	staffSprite = new Sprite(1);
	staffSprite->load(Resource::contentPath + "sprite/credits.png",0);


	
	intro = Resource::loadSound("sound/introVoice.wav");
	Game::state.mainTheme = Resource::loadSound("music/track1.wav");

	gameScene->paused=1;
	titleAnimator = 0.0f;
	selected = 0;
	srand((uint32_t)time_current);
	colorSelected = 1 + (rand() % 2);
	activeController = 0;
}

void MenuScene::end()
{
	Scene::end();
	delete titleSprite;
	delete pressanykeySprite;
	delete backgroundSprite;
	delete playSprite;
	delete howToPlaySprite;
	delete creditsSprite;
	delete resumeSprite;
	delete staffSprite;
	if(splashSprite)
		delete splashSprite;
	Resource::releaseSound(intro);
	Resource::releaseSound(Game::state.mainTheme);
}

void MenuScene::update()
{
	/*This deals with not letting the player play unless a controller is plugged in*/
	if (controller_connected() == 0){
		 MessageBox(NULL, "The game won't work unless you plug in an xbox 360 controller, and grab a friend. Please find both then press OK.",
        "Sorry", MB_OK | MB_ICONEXCLAMATION);
		 controller_check_connections();
		 if(controller_connected() == 0)
			 Game::exit();
	}
	Scene::update();
	/*Resets up the main menu based on who won if a game has just finished*/
	if(Game::state.playState == PLAY_FINISHED){
		colorSelected = Game::state.lastWon;
		menuState = MENU_STATE_MAIN;
		gameScene->paused = 1;
		selected = 0;
		Game::state.playState = PLAY_NOT_STARTED;
		visible = 1;
		((PongScene*)gameScene)->HUDScene->visible = 0;
	}
	if(activeController)
		controller_select(activeController);
	
	/*This deals with the interaction with different menu states*/ 
	switch(menuState){
			case MENU_STATE_START:
				if (key_pressed(KEY_ESCAPE) || controller_button_pressed(BUTT_B)) {
					Game::exit();
				} 
				else {
					if(keyboard_updated()){
						Game::state.playerControls[0] = 0;
						Game::state.numPlayers++;
						menuState = MENU_STATE_MAIN;
						sound_play(intro,0);
						if(!sound_playing(Game::state.mainTheme)){
							sound_play(Game::state.mainTheme,1);
							Game::state.musicStarted = time_current;
						}
					}
					else if((activeController = controller_updated())){
						controller_select(activeController);
						Game::state.numPlayers++;
						menuState = MENU_STATE_MAIN;
						sound_play(intro,0);
						if(!sound_playing(Game::state.mainTheme)){
							sound_play(Game::state.mainTheme,1);
							Game::state.musicStarted = time_current;
						}
					}
				}
				titleAnimator = (titleAnimator > 0.0f ? titleAnimator - (TITLE_Y_SPEED * time_delta) : 0.0f);
				break;
			case MENU_STATE_MAIN:
				titleAnimator = (titleAnimator < 1.0f ? titleAnimator + (TITLE_Y_SPEED * time_delta) : 1.0f);
				if((selected == 0) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					visible = 0;
					controller_check_connections();
					gameScene->changePlayers(4);
					gameScene->visible = true;
					gameScene->paused = 0;
					menuState = MENU_STATE_INGAME;
					Game::state.playState = PLAY_INGAME;
				}
				if((selected == 1) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					
					menuState = MENU_STATE_CONTROLS;
					if(key_pressed(KEY_ENTER))
						controlToShow = selected = 0;
					else
						controlToShow = selected = 1;
				}
				if((selected == 2) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					
					menuState = MENU_STATE_CREDITS;

				}
				if((selected == 3) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					Game::exit();
				}
				if(key_pressed(KEY_DOWN) || controller_button_pressed(BUTT_D_DOWN) || (controller_button_pressed(BUTT_L_THUMB_DOWN))){
					if(selected == 3)
						selected = 0;
					else
						selected++;
				}
				else if(key_pressed(KEY_UP) || controller_button_pressed(BUTT_D_UP) || (controller_button_pressed(BUTT_L_THUMB_UP))){
					if(selected == 0)
						selected = 3;
					else
						selected--;
				} else if (key_pressed(KEY_ESCAPE) || controller_button_pressed(BUTT_B)) {
					menuState = MENU_STATE_START;
					if(sound_playing(Game::state.mainTheme)){
						sound_stop(Game::state.mainTheme);
					}
				}
				break;
			case MENU_STATE_INGAME:
				if (key_pressed(KEY_ESCAPE)) {
					menuState = MENU_STATE_PAUSEGAME;
					gameScene->paused = 1;
					visible = 1;
				}
				else if(controller_select((activeController = controller_updated()))){
					if(controller_button_pressed(BUTT_START)){
						menuState = MENU_STATE_PAUSEGAME;
						gameScene->paused = 1;
						visible = 1;
					}
				}
				break;
			case MENU_STATE_PAUSEGAME:
				if((selected == 0) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					visible = 0;
					gameScene->paused = 0;
					menuState = MENU_STATE_INGAME;
				}
				if((selected == 1) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					
					menuState = MENU_STATE_CONTROLS;
					if(key_pressed(KEY_ENTER))
						controlToShow = selected = 0;
					else
						controlToShow = selected = 1;
				}
				if((selected == 2) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){
					
					menuState = MENU_STATE_CREDITS;

				}
				if((selected == 3) && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A))){

					((PongScene*)gameScene)->reset();
					((PongScene*)gameScene)->HUDScene->visible = 0;

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

					Game::state.blueScore  = 0;
					Game::state.redScore = 0;
					menuState = MENU_STATE_MAIN;
				}
				if(key_pressed(KEY_DOWN) || controller_button_pressed(BUTT_D_DOWN) || (controller_button_pressed(BUTT_L_THUMB_DOWN))){
					if(selected == 3)
						selected = 0;
					else
						selected++;
				}
				else if(key_pressed(KEY_UP) || controller_button_pressed(BUTT_D_UP) || (controller_button_pressed(BUTT_L_THUMB_UP))){
					if(selected == 0)
						selected = 3;
					else
						selected--;
				} else if (key_pressed(KEY_ESCAPE) || controller_button_pressed(BUTT_B) || controller_button_pressed(BUTT_START)) {
					visible = 0;
					gameScene->paused = 0;
					menuState = MENU_STATE_INGAME;
				}
				break;
			case MENU_STATE_CONTROLS:

				if(key_pressed(KEY_DOWN) || controller_button_pressed(BUTT_D_DOWN) || (controller_button_pressed(BUTT_L_THUMB_DOWN))){
					if(selected < 2)
						selected = 2;
					else
						selected = controlToShow;
				}
				else if(key_pressed(KEY_UP) || controller_button_pressed(BUTT_D_UP) || (controller_button_pressed(BUTT_L_THUMB_UP))){
					if(selected == 2)
						selected = controlToShow;
					else
						selected = 2;
				} 
				else if(key_pressed(KEY_LEFT) || controller_button_pressed(BUTT_D_LEFT) || (controller_button_pressed(BUTT_L_THUMB_LEFT)) || key_pressed(KEY_RIGHT) || controller_button_pressed(BUTT_D_RIGHT) || (controller_button_pressed(BUTT_L_THUMB_RIGHT))){
					if(selected == 1)
						controlToShow = selected = 0;
					else if(selected == 0)
						controlToShow = selected = 1;
				}
				else if (key_pressed(KEY_ESCAPE) || controller_button_pressed(BUTT_B) || (selected == 2 && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A) ))) {
					selected = 1;
					if(Game::state.playState == PLAY_INGAME)
						menuState = MENU_STATE_PAUSEGAME;
					else
						menuState = MENU_STATE_MAIN;
				}
				break;
			case MENU_STATE_CREDITS:
				if (key_pressed(KEY_ESCAPE) || controller_button_pressed(BUTT_B) || (selected == 2 && (key_pressed(KEY_ENTER) || controller_button_pressed(BUTT_A) ))) {
					if(Game::state.playState == PLAY_INGAME)
						menuState = MENU_STATE_PAUSEGAME;
					else
						menuState = MENU_STATE_MAIN;
				}
				break;
		}
		
}

void MenuScene::render()
{
	if (loading) {
		camera.bind();
		Graphics::depthMask(false);
		Graphics::lightingMask(false);
		camera.lookAt(
			D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1.0f, 0.0f)
		);

		Graphics::matrixMode(Graphics::modelMatrix);
		Graphics::loadIdentity();

		splashSprite = new Sprite(1);
		splashSprite->load(Resource::contentPath + "sprite/credits.png", 0);
		Graphics::pushMatrix();
		Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, ((float)Graphics::viewHeight/2.0f), 0.0f));
		splashSprite->render(0);
		Graphics::popMatrix();

		Graphics::lightingMask(true);
		Graphics::depthMask(true);
		
		loading = false;
		return;
	}

	Scene::render();

	camera.bind();
	Graphics::depthMask(false);
	Graphics::lightingMask(false);
	camera.lookAt(
		D3DXVECTOR3(0.0f, 0.0f, -10.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f)
	);

	Graphics::matrixMode(Graphics::modelMatrix);
	Graphics::loadIdentity();

	/* background */
	Graphics::pushMatrix();
	Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, ((float)Graphics::viewHeight/2.0f), 0.0f));
	Graphics::scale((1.0f) + (-0.5f * (menuState == MENU_STATE_PAUSEGAME)), (float)Graphics::viewHeight, 1.0f);
	backgroundSprite->render(0);
	Graphics::popMatrix();

	float titleY = TITLE_Y_START * (1 - titleAnimator) + TITLE_Y_MAIN * titleAnimator;
	switch(menuState){
		case MENU_STATE_START:
			/* press any key */
			Graphics::pushMatrix();
			Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, ((float)Graphics::viewHeight/2.0f) - 58.0f - (sin(time_current * 2.0f) * 4.0f), 0.0f));
			pressanykeySprite->render(0);
			Graphics::popMatrix();
			Graphics::pushMatrix();
			Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, titleY, 0.0f));
			titleSprite->render(Game::state.lastWon);
			Graphics::popMatrix();
			break;
		case MENU_STATE_MAIN:
			/* title */
			Graphics::pushMatrix();
				Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, titleY, 0.0f));
				titleSprite->render(Game::state.lastWon);

				Graphics::translate(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
				playSprite->render((selected == 0) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				howToPlaySprite->render((selected == 1) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				creditsSprite->render((selected == 2) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				quitSprite->render((selected == 3) * colorSelected);
			Graphics::popMatrix();
			break;
		case MENU_STATE_PAUSEGAME:
			Graphics::pushMatrix();
				Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, titleY - 45.0f, 0.0f));
				resumeSprite->render((selected == 0) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				howToPlaySprite->render((selected == 1) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				creditsSprite->render((selected == 2) * colorSelected);

				Graphics::translate(D3DXVECTOR3(0.0f, -50.0f, 0.0f));
				quitSprite->render((selected == 3) * colorSelected);
			Graphics::popMatrix();
			break;
		case MENU_STATE_CONTROLS:
				Graphics::pushMatrix();
				Graphics::translate(D3DXVECTOR3(((float)Graphics::viewWidth/2.0f) - 200.0f, titleY + 120.0f, 0.0f));
				keyboardSprite->render((selected == 0) * colorSelected);

				Graphics::translate(D3DXVECTOR3(400.0f, 0.0f , 0.0f));
				controllerSprite->render((selected == 1) * colorSelected);

				Graphics::translate(D3DXVECTOR3(-200.0f, -240.0f, 0.0f));
				controlsSprite->render(controlToShow);

				Graphics::translate(D3DXVECTOR3(300.0f, -150.0f, 0.0f));
				backSprite->render((selected == 2) * colorSelected);
			Graphics::popMatrix();
			break;
		case MENU_STATE_CREDITS:
			Graphics::pushMatrix();
				Graphics::translate(D3DXVECTOR3((float)Graphics::viewWidth/2.0f, ((float)Graphics::viewHeight/2.0f), 0.0f));
				splashSprite->render(0);
				Graphics::translate(D3DXVECTOR3(300.0f, -150.0f, 0.0f));
				backSprite->render((selected == 2) * colorSelected);
			Graphics::popMatrix();
	}

	Graphics::lightingMask(true);
	Graphics::depthMask(true);

}