/*
 * <Foundation/Resource.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_RESOURCE__
#define __H_GAME_RESOURCE__

#include <engine/sound.h>
#include <engine/graphics.h>
#include <list>
#include <string>

class Resource
{
public:
	Resource();
	virtual ~Resource();

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	std::string filename;
	int type;
	int referenceCount;

	std::string filePath();
	std::string fileExtension();

	static int loadSound(const std::string &filename);
	static Mesh *loadMesh(const std::string &filename);
	static Sprite *loadSprite(const std::string &filename);

	static void releaseSound(int soundId);
	static void releaseMesh(Mesh *mesh);
	static void releaseSprite(Sprite *sprite);

	static void setContentPath(const std::string &path);

	static std::string contentPath;

private:
	enum {
		typeUnknown = 0,
		typeSound,
		typeMesh,
		typeSprite
	};

	static Resource *getResourceByFilename(const std::string &path);

	static std::list<Resource *> resources;
};

class SoundResource : public Resource
{
public:
	SoundResource();
	~SoundResource();

	int soundId;
};

class MeshResource : public Resource
{
public:
	MeshResource();
	~MeshResource();

	Mesh *mesh;
};

class SpriteResource : public Resource
{
public:
	SpriteResource();
	~SpriteResource();

	Sprite *sprite;
};

#endif /* __H_GAME_RESOURCE__ */
