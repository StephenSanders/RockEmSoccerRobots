/*
 * <engine/graphics/light.cpp>
 * Stephen Sanders & Jake Singer
 */

#include <engine/graphics/light.h>

#include <engine/bits.h>

static uint8_t freeMask = 0xFF;

void Light::ambient(DWORD color)
{
	Graphics::d3ddev->SetRenderState(D3DRS_AMBIENT, color);
}

Light::Light(bool enable)
{
	if (enable && freeMask != 0) {
		index = bit_clz(freeMask);
		bit_clear(&freeMask, index);
		enabled = TRUE;
		Graphics::d3ddev->SetLight(index, &light);
		Graphics::d3ddev->LightEnable(index, TRUE);
	} else {
		enabled = FALSE;
	}
	ZeroMemory(&light, sizeof(light));
}

Light::~Light()
{
	if (enabled) {
		bit_set(&freeMask, index);
		Graphics::d3ddev->LightEnable(index, FALSE);
	}
}

bool Light::enable()
{
	if (!enabled) {
		if (freeMask != 0) {
			index = bit_clz(freeMask);
			bit_clear(&freeMask, index);
			enabled = TRUE;
			Graphics::d3ddev->SetLight(index, &light);
			Graphics::d3ddev->LightEnable(index, TRUE);
		} else {
			enabled = FALSE;
		}
	}
	return enabled;
}

bool Light::disable()
{
	if (enabled) {
		bit_set(&freeMask, index);
		enabled = FALSE;
		Graphics::d3ddev->LightEnable(index, FALSE);
	}
	return enabled;
}

void Light::setType(D3DLIGHTTYPE type)
{
	light.Type = type;
}

void Light::setDiffuse(D3DCOLORVALUE color)
{
	light.Diffuse = color;
}

void Light::setSpecular(D3DCOLORVALUE color)
{
	light.Specular = color;
}

void Light::setAmbient(D3DCOLORVALUE color)
{
	light.Ambient = color;
}

void Light::setPosition(D3DVECTOR position)
{
	light.Position = position;
}

void Light::setDirection(D3DVECTOR direction)
{
	light.Direction = direction;
}

void Light::setRange(float range)
{
	light.Range = range;
}

void Light::setFalloff(float falloff)
{
	light.Falloff = falloff;
}

void Light::setAttenuation0(float attenuation0)
{
	light.Attenuation0 = attenuation0;
}

void Light::setAttenuation1(float attenuation1)
{
	light.Attenuation1 = attenuation1;
}

void Light::setAttenuation2(float attenuation2)
{
	light.Attenuation2 = attenuation2;
}

void Light::setTheta(float theta)
{
	light.Theta = theta;
}

void Light::setPhi(float phi)
{
	light.Phi = phi;
}
