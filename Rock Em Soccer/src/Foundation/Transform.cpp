#include <Foundation/Transform.h>
#include <engine/memory.h>
#include <cmath>

Transform::Transform() :
	position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	scale(1.0f, 1.0f, 1.0f)
{
	D3DXQuaternionIdentity(&rotation);
}

Transform::Transform(const Transform &transform) :
	rotation(transform.rotation),
	position(transform.position),
	scale(transform.scale)
{ }

void *Transform::operator new(size_t bytes)
{
	return mem_alloc(bytes);
}

void Transform::operator delete(void *mem)
{
	mem_free(mem);
}

void Transform::rotate(float angle, D3DXVECTOR3 axis)
{
	D3DXQUATERNION transform;
	D3DXQuaternionRotationAxis(&transform, &axis, D3DXToRadian(angle));
	D3DXQuaternionNormalize(&rotation, &rotation);
	D3DXQuaternionMultiply(&rotation, &transform, &rotation);
}

void Transform::rotate(float angle, float x, float y, float z)
{
	rotate(angle, D3DXVECTOR3(x, y, z));
}

void Transform::rotate(float yaw, float pitch, float roll)
{
	D3DXQUATERNION transform;
	D3DXQuaternionRotationYawPitchRoll(&transform, D3DXToRadian(yaw), D3DXToRadian(pitch), D3DXToRadian(roll));
	D3DXQuaternionNormalize(&rotation, &rotation);
	D3DXQuaternionMultiply(&rotation, &transform, &rotation);
}

void Transform::setRotation(float angle, D3DXVECTOR3 axis)
{
	D3DXQuaternionIdentity(&rotation);
	rotate(angle, axis);
}

void Transform::setRotation(float angle, float x, float y, float z)
{
	setRotation(angle, D3DXVECTOR3(x, y, z));
}

void Transform::getRotation(float *angle, D3DXVECTOR3 *axis)
{
	float radians;
	D3DXQuaternionNormalize(&rotation, &rotation);
	D3DXQuaternionToAxisAngle(&rotation, axis, &radians);
	*angle = D3DXToDegree(radians);
}

void Transform::getRotation(float *angle, float *x, float *y, float *z)
{
	D3DXVECTOR3 axis;
	getRotation(angle, &axis);
	*x = axis.x; *y = axis.y; *z = axis.z;
}

void Transform::scaleUniform(float scaling)
{
	scale = D3DXVECTOR3(scaling, scaling, scaling);
}

void Transform::slerp(Transform *start, Transform *end, float weight)
{
	D3DXQuaternionNormalize(&rotation, &rotation);
	D3DXQuaternionSlerp(&rotation, &(start->rotation), &(end->rotation), weight);
	D3DXVec3Lerp(&position, &(start->position), &(end->position), weight);
	D3DXVec3Lerp(&scale, &(start->scale), &(end->scale), weight);
}
