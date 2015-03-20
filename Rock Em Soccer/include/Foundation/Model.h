/*
 * Model.h
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_MODEL__
#define __H_GAME_MODEL__

#include <engine/graphics.h>
#include <vector>

class Entity;

class Model
{
public:
	Model() : entity(NULL) { }

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	void addMesh(Mesh *mesh);

	/* unspecified values will rely on entity's transform data (if it exists) */
	void render();
	void render(D3DXVECTOR3 position);
	void render(D3DXVECTOR3 position, D3DXQUATERNION rotation);
	void render(D3DXVECTOR3 position, D3DXVECTOR3 scale);
	void render(D3DXVECTOR3 position, D3DXQUATERNION rotation, D3DXVECTOR3 scale);

protected:
	friend class Entity;
	Entity *entity;

private:
	std::vector<Mesh *> meshes;
};

#endif /* __H_GAME_MODEL__ */
