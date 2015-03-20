/*
 * Game.h
 * Stephen Sanders & Jake Singer
 *
 * Handles the highest level of game flow. No algorithms or scenerio specific
 * code should reside here. Code for said scenerios should be put with the
 * respective Scene or game Entity subclass.
 */

#ifndef __H_GAME__
#define __H_GAME__

#include <engine/engine.h>

#include <Foundation/Scene.h>
#include <Foundation/Resource.h>
#include <string>
#include <list>
#include <map>

typedef std::list<Scene *>::iterator SceneIterator;

enum{
	TEAM_BLUE = 1,
	TEAM_RED = 2
};

enum{
	PLAY_INGAME = 0,
	PLAY_FINISHED = 1,
	PLAY_NOT_STARTED = 3
};

class Game
{
public:

	static struct _state {
		int numPlayers;
		int playerControls[4];
		int blueScore;
		int redScore;
		int lastWon;
		int playState;
		int mainTheme;
		int music2; 
		int	music3;
		int	music4;
		float musicStarted;
		float redBoost;
		float blueBoost;
	} state;

	static bool init(const std::string &name = "Game", const std::string &configPath = "./config.ini", Scene *startScene = NULL);
	static bool release();

	/* run time control */
	static bool loop();
	static void exit();

	/*
	 * Scene Management
	 */

	/* add or remove scenes (buffered: changes on frame end) */
	static void pushScene(Scene *scene);
	static Scene *popScene();
	static bool insertScene(Scene *scene, SceneIterator position);
	static bool removeScene(Scene *scene);
	static void removeAllScenes();

	/* find scenes */
	static Scene *bottomScene();
	static Scene *topScene();
	static const std::list<Scene *> *getScenes();

	/*
	 * Public Variables
	 */

	/* should the game end? */
	static bool done;
	static std::list<Scene *> scenes;
private:
	/* scenes */
	
	static std::list<Scene *> scenesToRemove;
	static std::map<uint32_t, std::list<Scene *>> scenesToInsert;

	static void updateSceneLists();
};

#endif /* __H_GAME__ */
