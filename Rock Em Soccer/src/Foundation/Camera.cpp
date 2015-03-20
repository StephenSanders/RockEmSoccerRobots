#include <Foundation/Camera.h>
#include <engine/memory.h>

Camera::Camera() : projection(perspectiveProjection), fov(45.0f)
{ }

Camera::Camera(const Camera &camera) : projection(camera.projection)
{ }

void *Camera::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Camera::operator delete(void *mem)
{
	mem_free(mem);
}

void Camera::setProjection(int mode)
{
	projection = mode;
}

void Camera::setFOV(float angle)
{
	fov = angle;
}

void Camera::lookAt(D3DXVECTOR3 position, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	Graphics::lookAt(
		position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z
	);
}

void Camera::bind()
{
	Graphics::matrixMode(Graphics::projectionMatrix);
	Graphics::loadIdentity();
	switch (projection) {
		case orthographicProjection:
			Graphics::orthographic(0.0f, (float)Graphics::viewWidth, 0.0f, (float)Graphics::viewHeight, 0.0f, 100.0f);
			break;
		default:
			Graphics::perspective(fov, (float)Graphics::viewWidth / (float)Graphics::viewHeight, 1.0f, 1000.0f);
			break;
	}
	
	Graphics::matrixMode(Graphics::viewMatrix);
	Graphics::loadIdentity();
	D3DXMATRIX transform;
	D3DXMatrixAffineTransformation(&transform, 1.0f, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &rotation, &position);
	Graphics::setTransform(transform);
}
