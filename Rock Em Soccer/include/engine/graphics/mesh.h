/*
 * <engine/graphics/mesh.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_GRAPHICS_MESH__
#define __H_ENGINE_GRAPHICS_MESH__

#include <engine/graphics.h>

#include <string>

class Mesh
{
public:
	Mesh(const std::string &path);
	~Mesh();

	void render();

	D3DXVECTOR3 center();
	float radius();
	void bounds(D3DXVECTOR3 *min, D3DXVECTOR3 *max);

	
	bool setAnimation(int index, float transition = 250.0f);
	void setSpeed(float speed);

protected:
	friend class Graphics;

	D3DXVECTOR3 sphereCenter, minBounds, maxBounds;
	float sphereRadius;
	int modelId, animationId, animationCount;

	void *threadData;
	DWORD threadId;
	HANDLE loadThread;
};

#endif /* __H_ENGINE_GRAPHICS_MESH__ */
