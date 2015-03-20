/*
 * Game.cpp
 * Stephen Sanders & Jake Singer
 *
 * Handles the highest level of game flow. No algorithms or scenerio specific
 * code should reside here. Code for said scenerios should be put with the
 * respective Scene or game Entity subclass.
 */

#include <Foundation/Game.h>

#include <engine/platform.h>
#include <engine/graphics.h>
#include <engine/sound.h>
#include <engine/keyboard.h>
#include <engine/controller.h>
#include <Foundation/Settings.h>

#include <cstdio>

Game::_state Game::state;
bool Game::done;
std::list<Scene *> Game::scenes;
std::list<Scene *> Game::scenesToRemove;
std::map<uint32_t, std::list<Scene *>> Game::scenesToInsert;

bool Game::init(const std::string &name, const std::string &configPath, Scene *startScene)
{
	uint32_t width, height, mode;

	done = false;
	
	/* initialize subsystems */
	Settings::init(configPath);
	width = Settings::getWidth();
	height = Settings::getHeight();
	mode = Settings::getFullscreen();

	platform_init(name.c_str(), width, height, mode);
	Graphics::init(width, height, (mode & PLATFORM_FULLSCREEN) != 0);
	time_init(); /* does not need an end() call */
	sound_init();
	keyboard_init();
	controller_init();

	/* game state */
	state.numPlayers = 0;
	state.blueScore = 0;
	state.redScore = 0;
	state.playState = 0;
	state.lastWon = 0;
	state.playState = PLAY_INGAME;

	/* set the starting scene */
	if (startScene) {
		scenes.push_back(startScene);
		startScene->start();
	}
	return true;
}

bool Game::release()
{
	/* release subsystems in reverse order of initialization */
	keyboard_end();
	controller_end();
	sound_end();
	Graphics::end();
	platform_release();
	Settings::release();
	return true;
}

bool Game::loop()
{
	while (!done) {
		/* update platform stuff, exiting if called for */
		if (platform_update())
			Game::exit();
		time_update();
		keyboard_update();
		controller_update_all();

		/* update all scenes */
		for (SceneIterator it = scenes.begin(); it != scenes.end(); ++it) {
			if (!(*it)->paused)
				(*it)->update();
		}

		/* ensure graphics device is intact */
		//if (Graphics::lost())
		//	Graphics::restore();

		/* render all scenes */
		if (Graphics::beginFrame()) {
			for (SceneIterator it = scenes.begin(); it != scenes.end(); ++it) {
				if ((*it)->visible)
					(*it)->render();
			}
			Graphics::endFrame();
		}
		Graphics::presentFrame();

		/* apply scene changes */
		updateSceneLists();
	}

	return true;
}

void Game::exit()
{
	/* remove everything! */
	removeAllScenes();

	/* game is now done */
	done = true;
}

/*
 * Scene Management
 */

/* push and pop scenes */
void Game::pushScene(Scene *scene) { insertScene(scene, scenes.end()); }
Scene *Game::popScene() { Scene *s = topScene(); scenesToRemove.push_back(s); return s; }

/* insert scene at any location */
bool Game::insertScene(Scene *scene, SceneIterator position)
{
	uint32_t index = 0;
	for (SceneIterator it = scenes.begin(); it != scenes.end(); ++it, ++index) {
		if (it == position) {
			scenesToInsert[index].push_back(scene);
			return true;
		}
	}
	scenesToInsert[index].push_back(scene);
	return true;
}

/* remove any scene */
bool Game::removeScene(Scene *scene)
{
	for (SceneIterator it = scenes.begin(); it != scenes.end(); ++it) {
		if (*it == scene) {
			scenesToRemove.push_back(scene);
			return true;
		}
	}
	return false;
}

/* remove all the scenes! */
void Game::removeAllScenes()
{
	scenesToRemove.clear();
	for (SceneIterator it = scenes.begin(); it != scenes.end(); ++it) {
		scenesToRemove.push_back(*it);
	}
}

/* get bottom and top scenes */
Scene *Game::bottomScene() { return *(scenes.begin()); }
Scene *Game::topScene() { return *(scenes.end()); }
const std::list<Scene *> *Game::getScenes()
{
	return &scenes;
}

/*
 * Internal Scene Management
 */

void Game::updateSceneLists()
{
	/* resolve inserts */
	for (std::map<uint32_t, std::list<Scene *>>::iterator i = scenesToInsert.begin(); i != scenesToInsert.end(); ++i) {
		for (SceneIterator it = (*i).second.begin(); it != (*i).second.end(); ++it) {
			/* insert scene and start it */
			SceneIterator pos = scenes.begin();
			for (uint32_t j = 0; j < i->first; ++j)
				++pos;
			scenes.insert(pos, *it);
			(*it)->start();
		}
	}
	scenesToInsert.clear();

	/* remove duplicates */
	scenesToRemove.sort();
	scenesToRemove.unique();

	/* resolve removes */
	for (SceneIterator it = scenesToRemove.begin(); it != scenesToRemove.end(); ++it) {
		/* end scene and remove it */
		(*it)->end();
		scenes.remove(*it);

		delete (*it);
	}
	scenesToRemove.clear();
}
