/*
 * Scene.h
 * Stephen Sanders & Jake Singer
 *
 * A Scene is a collection of game Entities and code to manage them. Though
 * Scenes are convenient for managing entire game 'levels', it is encouraged
 * to layer scenes (e.g. use seperate Scenes for an area in the game world
 * and the user interface).
 */

#ifndef __H_GAME_SCENE__
#define __H_GAME_SCENE__

#include <Foundation/Entity.h>
#include <Foundation/Camera.h>

#include <engine/engine.h>
#include <list>
#include <map>

typedef std::list<Entity *>::iterator EntityIterator;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	/*
	 * Events to Override
	 */

	/* called on scene start */
	virtual void start();

	/* called on scene end */
	virtual void end();

	/* called during game loop if not paused */
	virtual void update();

	/* called if visible to render */
	virtual void render();

	/*
	 * Entity Management
	 */

	/* add entity to scene (buffered: changes on frame end) */
	void addEntity(Entity *entity);

	/* remove entity from scene (buffered: changes on frame end) */
	void removeEntity(Entity *entity);

	/* remove all entities from scene (buffered: changes on frame end) */
	void removeAllEntities();

	/* get list of entities */
	const std::list<Entity *> *getEntities();

	/*
	 * Tag Management
	 */

	Entity *getFirstEntityWithTag(const std::string &tag);
	std::list<Entity *> *getEntitiesWithTag(const std::string &tag);

	/*
	 * Public Variables
	 */

	/* should scene be updated? */
	bool paused;

	/* should scene be rendered? */
	bool visible;

	Camera camera;

protected:
	friend Entity;
	std::map<std::string, std::list<Entity *>> tagMap;

private:
	/* entities */
	std::list<Entity *> entities;
	std::list<Entity *> entitiesToAdd;
	std::list<Entity *> entitiesToRemove;

	void updateEntityLists();
};

#endif /* __H_GAME_SCENE__ */
