/*
 * PongHUDScene.h
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_MENUSCENE__
#define __H_GAME_MENUSCENE__

#include <Foundation/Scene.h>
#include <engine/graphics.h>
#include "PongScene.h"

enum{
	MENU_STATE_START = 0,
	MENU_STATE_MAIN = 1,
	MENU_STATE_INGAME = 2,
	MENU_STATE_ENDGAME = 3,
	MENU_STATE_CONTROLS = 4,
	MENU_STATE_DISCONNECT = 5,
	MENU_STATE_PAUSEGAME = 6,
	MENU_STATE_CREDITS = 7
};


class MenuScene : public Scene
{
public:

	void start();
	
	void end();

	void update();

	void render();

private:
	bool loading;
	Sprite *titleSprite, *pressanykeySprite, *playSprite, *howToPlaySprite, *resumeSprite, *creditsSprite, *quitSprite, *backgroundSprite, *splashSprite;
	Sprite *backSprite, *controllerSprite, *keyboardSprite, *controlsSprite, *staffSprite; 
	PongScene *gameScene;
	int menuState, selected, colorSelected, activeController, controlToShow;
	int intro;
};

#endif /* __H_GAME_MENUSCENE__ */
