/*
 * <engine/graphics/light.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_GRAPHICS_LIGHT__
#define __H_ENGINE_GRAPHICS_LIGHT__

#include <engine/graphics.h>

class Light
{
public:
	static void ambient(DWORD color);

	Light(bool enable = FALSE);
	~Light();

	bool enable();
	bool disable();

	/* Type of the light source. This value is one of the members of the
	   D3DLIGHTTYPE enumerated type. */
	void setType(D3DLIGHTTYPE type);

	/* Diffuse color emitted by the light. This member is a D3DCOLORVALUE
	   structure. */
	void setDiffuse(D3DCOLORVALUE color);

	/* Specular color emitted by the light. This member is a D3DCOLORVALUE
	   structure. */
	void setSpecular(D3DCOLORVALUE color);

	/* Ambient color emitted by the light. This member is a D3DCOLORVALUE
	   structure. */
	void setAmbient(D3DCOLORVALUE color);

	/* Position of the light in world space, specified by a D3DVECTOR
	   structure. This member has no meaning for directional lights and is
	   ignored in that case. */
	void setPosition(D3DVECTOR position);

	/* Direction that the light is pointing in world space, specified by a
	   D3DVECTOR structure. This member has meaning only for directional and
	   spotlights. This vector need not be normalized, but it should have a
	   nonzero length. */
	void setDirection(D3DVECTOR direction);

	/* Distance beyond which the light has no effect. The maximum allowable
	   value for this member is the square root of FLT_MAX. This member does
	   not affect directional lights. */
	void setRange(float range);

	/* Decrease in illumination between a spotlight's inner cone (the angle
	   specified by Theta) and the outer edge of the outer cone (the angle
	   specified by Phi).
	   The effect of falloff on the lighting is subtle. Furthermore, a small
	   performance penalty is incurred by shaping the falloff curve. For these
	   reasons, most developers set this value to 1.0. */
	void setFalloff(float falloff);

	/* Value specifying how the light intensity changes over distance.
	   Attenuation values are ignored for directional lights. This member
	   represents an attenuation constant. Valid values for this member range
	   from 0.0 to infinity. For non-directional lights, all three attenuation
	   values should not be set to 0.0 at the same time. */
	void setAttenuation0(float attenuation0);
	void setAttenuation1(float attenuation1);
	void setAttenuation2(float attenuation2);

	/* Angle, in radians, of a spotlight's inner cone - that is, the fully
	   illuminated spotlight cone. This value must be in the range from 0
	   through the value specified by Phi. */
	void setTheta(float theta);

	/* Angle, in radians, defining the outer edge of the spotlight's outer
	   cone. Points outside this cone are not lit by the spotlight. This value
	   must be between 0 and pi. */
	void setPhi(float phi);

protected:
	friend class Graphics;
	
	D3DLIGHT9 light;
	DWORD index;
	bool enabled;
};

#endif /* __H_ENGINE_GRAPHICS_LIGHT__ */
