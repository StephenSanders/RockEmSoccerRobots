/*
 * Resource.cpp
 * Stephen Sanders & Jake Singer
 */

#include <Foundation/Resource.h>
#include <engine/memory.h>

std::list<Resource *> Resource::resources;
std::string Resource::contentPath;

Resource::Resource() : type(typeUnknown), referenceCount(0)
{ }

Resource::~Resource()
{ }

void *Resource::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Resource::operator delete(void *mem)
{
	mem_free(mem);
}

/*
 *
 */

int Resource::loadSound(const std::string &filename)
{
	SoundResource *resource = NULL;
	std::string path = contentPath + filename;

	resource = (SoundResource *)getResourceByFilename(filename);
	if (!resource) {
		resource = new SoundResource();
		resource->soundId = sound_load(path.c_str());
		resource->type = typeSound;
		resource->filename = filename;
		if (resource->soundId >= 0)
			resources.push_back((Resource *)resource);
	}
	if (resource)
		resource->referenceCount++;
	return resource->soundId;
}

Mesh *Resource::loadMesh(const std::string &filename)
{
	MeshResource *resource = NULL;
	std::string path = contentPath + filename;

	resource = (MeshResource *)getResourceByFilename(filename);
	if (!resource) {
		resource = new MeshResource();
		resource->mesh = new Mesh(path);
		resource->type = typeMesh;
		resource->filename = filename;
		if (resource->mesh)
			resources.push_back((Resource *)resource);
	}
	if (resource)
		resource->referenceCount++;
	return resource->mesh;
}

Sprite *Resource::loadSprite(const std::string &filename)
{
	SpriteResource *resource = NULL;
	std::string path = contentPath + filename;

	resource = (SpriteResource *)getResourceByFilename(filename);
	if (!resource) {
		resource = new SpriteResource();
		resource->sprite = new Sprite(1);
		resource->sprite->load(path, 0);
		resource->type = typeSprite;
		resource->filename = filename;
		if (resource->sprite)
			resources.push_back((Resource *)resource);
	}
	if (resource)
		resource->referenceCount++;
	return resource->sprite;
}

void Resource::releaseSound(int soundId)
{
	SoundResource *resource = NULL;
	for (std::list<Resource *>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if ((*it)->type == typeSound) {
			resource = (SoundResource *)(*it);
			if (resource->soundId == soundId) {
				break;
			}
		}
	}
	if (resource) {
		if (--(resource->referenceCount) <= 0) {
			resources.remove(resource);
			delete resource;
		}
	}	
}

void Resource::releaseMesh(Mesh *mesh)
{
	MeshResource *resource = NULL;
	for (std::list<Resource *>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if ((*it)->type == typeMesh) {
			resource = (MeshResource *)(*it);
			if (resource->mesh == mesh) {
				break;
			}
		}
	}
	if (resource) {
		if (--(resource->referenceCount) <= 0) {
			resources.remove(resource);
			delete resource;
		}
	}	
}

void Resource::releaseSprite(Sprite *sprite)
{
	SpriteResource *resource = NULL;
	for (std::list<Resource *>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if ((*it)->type == typeSprite) {
			resource = (SpriteResource *)(*it);
			if (resource->sprite == sprite) {
				break;
			}
		}
	}
	if (resource) {
		if (--(resource->referenceCount) <= 0) {
			resources.remove(resource);
			delete resource;
		}
	}	
}

void Resource::setContentPath(const std::string &path)
{
	contentPath = path;
}

Resource *Resource::getResourceByFilename(const std::string &path)
{
	for (std::list<Resource *>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if ((*it)->filename.compare(path) == 0) {
			return (*it);
		}
	}
	return NULL;
}

/*
 *
 */

SoundResource::SoundResource()
{

}

SoundResource::~SoundResource()
{
	sound_release(soundId);
}

MeshResource::MeshResource()
{

}

MeshResource::~MeshResource()
{
	delete mesh;
}

SpriteResource::SpriteResource()
{

}

SpriteResource::~SpriteResource()
{
	delete sprite;
}
