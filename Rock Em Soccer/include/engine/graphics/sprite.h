/*
 * <engine/graphics/sprite.h>
 * Stephen Sanders & Jake Singer
 *
 * Adapted from C3DSprite by Ian Parberry.
 */

#ifndef __H_ENGINE_GRAPHICS_SPRITE__
#define __H_ENGINE_GRAPHICS_SPRITE__

#include <engine/graphics.h>

#include <string>

class Sprite
{
public:
	Sprite(int frames);
	~Sprite();

	bool load(const std::string &path, int frame);
	void render(int frame);

	uint32_t width, height;

protected:
	friend class Graphics;

	LPDIRECT3DTEXTURE9 *_textures;
	D3DXIMAGE_INFO _info;
	LPDIRECT3DVERTEXBUFFER9 _vertices;
	int _frames;
};

#endif /* __H_ENGINE_GRAPHICS_SPRITE__ */
