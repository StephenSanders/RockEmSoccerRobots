#include <Foundation/Model.h>
#include <Foundation/Entity.h>

void *Model::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Model::operator delete(void *mem)
{
	mem_free(mem);
}

void Model::addMesh(Mesh *mesh)
{
	meshes.push_back(mesh);
}

void Model::render()
{
	if (entity)
		render(entity->position, entity->rotation, entity->scale);
	else
		render(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

void Model::render(D3DXVECTOR3 position)
{
	if (entity)
		render(position, entity->rotation, entity->scale);
	else
		render(position, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

void Model::render(D3DXVECTOR3 position, D3DXQUATERNION rotation)
{
	if (entity)
		render(position, rotation, entity->scale);
	else
		render(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

void Model::render(D3DXVECTOR3 position, D3DXVECTOR3 scale)
{
	if (entity)
		render(position, entity->rotation, scale);
	else
		render(position, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), scale);
}

void Model::render(D3DXVECTOR3 position, D3DXQUATERNION rotation, D3DXVECTOR3 scale)
{
	Graphics::matrixMode(Graphics::modelMatrix);
	Graphics::loadIdentity();
	Graphics::pushMatrix();
	Graphics::translate(position);
	Graphics::rotate(rotation);
	Graphics::scale(scale);
	for (std::vector<Mesh *>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->render();
	}
	Graphics::popMatrix();
}
