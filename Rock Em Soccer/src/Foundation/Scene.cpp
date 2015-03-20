/*
 * Scene.cpp
 * Stephen Sanders & Jake Singer
 */

#include <Foundation/Scene.h>
#include <engine/memory.h>

static std::list<Entity *> entities;
static std::list<Entity *> entitiesToAdd;
static std::list<Entity *> entitiesToRemove;

Scene::Scene() : paused(false), visible(true)
{ }

Scene::~Scene()
{ }

void *Scene::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Scene::operator delete(void *mem)
{
	mem_free(mem);
}

void Scene::start()
{

}

void Scene::end()
{
	removeAllEntities();
	updateEntityLists();
}

void Scene::update()
{
	if (!paused) {
		for (EntityIterator it = entities.begin(); it != entities.end(); ++it) {
			(*it)->update();
		}
	}
	updateEntityLists();
}

void Scene::render()
{
	if (visible) {
		for (EntityIterator it = entities.begin(); it != entities.end(); ++it) {
			(*it)->render();
		}
	}
}

void Scene::addEntity(Entity *entity)
{
	/* TODO: debug check if in scene or marked for adding */
	if (entity->scene != NULL) {
		/* error, already in scene */
	} else {
		entity->scene = this;
		entitiesToAdd.push_back(entity);
	}
}

void Scene::removeEntity(Entity *entity)
{
	/* TODO: debug check if not in scene or marked for removal */
	entitiesToRemove.push_back(entity);
}

void Scene::removeAllEntities()
{
	entitiesToRemove.clear();
	for (EntityIterator it = entities.begin(); it != entities.end(); ++it) {
		entitiesToRemove.push_back(*it);
	}
}

const std::list<Entity *> *Scene::getEntities()
{
	return &entities;
}

/*
	* Tag Management
	*/

Entity *Scene::getFirstEntityWithTag(const std::string &tag)
{
	for (EntityIterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->hasTag(tag)) {
			return (*it);
		}
	}
	return NULL;
}

std::list<Entity *> *Scene::getEntitiesWithTag(const std::string &tag)
{
	if (tagMap.count(tag) == 0)
		return NULL;
	else
		return &tagMap[tag];
}

void Scene::updateEntityLists()
{
	entitiesToRemove.sort();
	entitiesToRemove.unique();

	for (EntityIterator it = entitiesToRemove.begin(); it != entitiesToRemove.end(); ++it) {
		(*it)->end();
		entities.remove(*it);
		(*it)->scene = NULL;
		delete (*it);
	}
	entitiesToRemove.clear();

	for (EntityIterator it = entitiesToAdd.begin(); it != entitiesToAdd.end(); ++it) {
		entities.push_back(*it);
		(*it)->scene = this;
		(*it)->start();
	}
	entitiesToAdd.clear();
}
