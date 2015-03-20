/*
 * Camera.h
 * Stephen Sanders & Jake Singer
 */

#include <engine/graphics.h>
#include <Foundation/Transform.h>

#ifndef __H_GAME_CAMERA__
#define __H_GAME_CAMERA__

class Camera : public Transform
{
public:
	Camera();
	Camera(const Camera &camera);

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	void setProjection(int mode);
	void setFOV(float angle);
	void lookAt(D3DXVECTOR3 position, D3DXVECTOR3 target, D3DXVECTOR3 up);

	void bind();

	enum projectionType {
		perspectiveProjection = 0,
		orthographicProjection
	};

	int projection;
	float fov;
};

#endif /* __H_GAME_CAMERA__ */
