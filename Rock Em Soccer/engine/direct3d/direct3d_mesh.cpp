/*
 * <engine/graphics/mesh.cpp>
 * Stephen Sanders & Jake Singer
 */

#include <engine/debug.h>
#include <engine/time.h>
#include <engine/graphics/mesh.h>

Mesh::Mesh(const std::string &path)
{
	/* load model */
	modelId = -1;
	Graphics::animator->LoadXFile(path, &modelId);
	animationCount = Graphics::animator->GetNumberOfAnimationSets(modelId);
	animationId = 0;
	Graphics::animator->GetBoundingShapes(modelId, &minBounds, &maxBounds,&sphereCenter, &sphereRadius);
}

Mesh::~Mesh()
{
	/* unload model */
	Graphics::animator->DiscardModel(modelId);
}

void Mesh::render()
{
	if (modelId >= 0) {
		Graphics::animator->GetBoundingShapes(modelId, &minBounds, &maxBounds,&sphereCenter, &sphereRadius);
		Graphics::animator->Render(modelId, Graphics::getTransform(), time_delta * 1000.0f);
	}
}

D3DXVECTOR3 Mesh::center()
{
	if (modelId >= 0) {
		return sphereCenter;
	} else {
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

float Mesh::radius()
{
	return sphereRadius;
}

void Mesh::bounds(D3DXVECTOR3 *min, D3DXVECTOR3 *max)
{
	if (modelId >= 0) {
		*min = minBounds; *max = maxBounds;
	} else {
		*min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		*max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

bool Mesh::setAnimation(int index, float transition)
{
	if (modelId >= 0) {
		if (animationCount < index)
			return false;
		Graphics::animator->ChangeAnimationSet(modelId, index, transition);
		animationId = index;
		return true;
	} else {
		return false;
	}
}

void Mesh::setSpeed(float speed)
{
	if (modelId >= 0) {
		Graphics::animator->ChangeAnimationPlaybackSpeed(modelId, speed);
	}
}
