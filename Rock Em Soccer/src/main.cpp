/*
 * main.cpp
 * Stephen Sanders & Jake Singer
 *
 * Main file for Kung Fu Kickers.
 *
 * While the engine is written in C for simplicity and minimalism, all game
 * code will be in C++. The purpose of this is to encourage a focus on high
 * level concepts and overal program flow rather than technical specifics.
 */

#include <engine/engine.h>
#include <Foundation/Game.h>

#include "Scenes/MenuScene.h"

int main(int argc, char *argv[])
{
	mem_init();
	Resource::setContentPath("../data/");
	Game::init("Rock Em' Soccer Robots", "./config.ini", new MenuScene());
	Game::loop();
	Game::release();
	mem_end();
	return 0;
}
