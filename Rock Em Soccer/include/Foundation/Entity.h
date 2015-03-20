/*
 * <Foundation/Entity.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_ENTITY__
#define __H_GAME_ENTITY__

#include <engine/engine.h>
#include <Foundation/Transform.h>
#include <Foundation/Model.h>
#include <string>
#include <list>

class Scene;

class Entity : public Transform
{
public:
	Entity();
	virtual ~Entity();

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	/*
	 * Events to Override
	 */

	/* called when added to scene */
	virtual void start();

	/* called when removed from scene */
	virtual void end();

	/* called during game loop if not paused */
	virtual void update();

	/* called if visible to render */
	virtual void render();

	/*
	 * Tag Management
	 */

	/* add tag to entity */
	void addTag(const std::string &tag);

	/* remove tag from entity */
	void removeTag(const std::string &tag);

	/* remove all tags */
	void removeAllTags();

	/* return true if a matching tag exists */
	bool hasTag(const std::string &tag);

	/* get list of tags */
	const std::list<std::string> *getTags();

	/*
	 * Public Variables
	 */

	/* should scene be updated? */
	bool paused;

	/* should scene be rendered? */
	bool visible;

protected:
	friend class Scene;
	friend class Model;
	Scene *scene;
	Model model;

private:
	std::list<std::string> tags;
};

#endif /* __H_GAME_ENTITY__ */
