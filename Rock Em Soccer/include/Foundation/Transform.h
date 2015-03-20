/*
 * Transform.h
 * Stephen Sanders & Jake Singer
 */

#include <engine/graphics.h>

#ifndef __H_GAME_TRANSFORM__
#define __H_GAME_TRANSFORM__

class Transform
{
public:
	Transform();
	Transform(const Transform &transform);

	void *operator new(size_t bytes);
	void operator delete(void *mem);

	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 scale;

	void rotate(float angle, D3DXVECTOR3 axis);
	void rotate(float angle, float x, float y, float z);
	void rotate(float yaw, float pitch, float roll);
	void setRotation(float angle,  D3DXVECTOR3 axis);
	void setRotation(float angle, float x, float y, float z);
	void getRotation(float *angle, D3DXVECTOR3 *axis);
	void getRotation(float *angle, float *x, float *y, float *z);

	void scaleUniform(float scaling);

	/* make spherical linear interpolation using weighted value 0.0f to 1.0f */
	void slerp(Transform *start, Transform *end, float weight);
};

#endif /* __H_GAME_TRANSFORM__ */
