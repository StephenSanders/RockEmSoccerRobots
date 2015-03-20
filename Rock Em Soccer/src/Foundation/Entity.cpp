/*
 * Entity.cpp
 * Stephen Sanders & Jake Singer
 */

#include <Foundation/Entity.h>
#include <Foundation/Scene.h>
#include <engine/memory.h>

Entity::Entity() : paused(false), visible(true), scene(NULL)
{
	model.entity = this;
}

Entity::~Entity() { }

void *Entity::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Entity::operator delete(void *mem)
{
	mem_free(mem);
}

/*
 * Events to Override
 */

/* called when added to scene */
void Entity::start() { }

/* called when removed from scene */
void Entity::end() { }

/* called during game loop if not paused */
void Entity::update() { }

/* called if visible to render */
void Entity::render()
{
	if(visible)
		model.render();
}

/*
 * Tag Management
 */

/* add tag to entity */
void Entity::addTag(const std::string &tag)
{
	if (scene && !hasTag(tag)) {
		tags.push_back(tag);
		scene->tagMap[tag].push_back(this);
	}
}

/* remove tag from entity */
void Entity::removeTag(const std::string &tag)
{
	if (!scene || !hasTag(tag)) {
		/* error! */
	}

	for (std::list<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
		if ((*it).compare(tag) == 0) {
			tags.erase(it);
			scene->tagMap[tag].remove(this);
		}
	}
}

/* remove all tags */
void Entity::removeAllTags()
{
	if (!scene) {
		for (std::list<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
			tags.erase(it);
			scene->tagMap[*it].remove(this);
		}
	}
}

/* return true if a matching tag exists */
bool Entity::hasTag(const std::string &tag)
{
	for (std::list<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
		if ((*it).compare(tag) == 0) {
			return true;
		}
	}
	return false;
}

/* get list of tags */
const std::list<std::string> *Entity::getTags()
{
	return &tags;
}
