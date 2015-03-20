/*
 * <engine/graphics/sprite.cpp>
 * Stephen Sanders & Jake Singer
 *
 * Adapted from C3DSprite by Ian Parberry.
 */

#include <engine/graphics/sprite.h>

typedef struct SPRITEVERTEX { 
	D3DXVECTOR3 p; ///< Position.
	float u,v; ///< Texture coordinates.
} SPRITEVERTEX;
#define SPRITEVERTEXFVF (D3DFVF_XYZ | D3DFVF_TEX1)

Sprite::Sprite(int frames)
{
	_frames = frames;
	_textures = new LPDIRECT3DTEXTURE9[frames];
	for (int i = 0; i < frames; ++i)
		_textures[i] = NULL;
	ZeroMemory(&_info, sizeof(D3DXIMAGE_INFO));
	_vertices = NULL;
}

Sprite::~Sprite()
{
	if (_vertices) _vertices->Release();
	for (int i = 0; i < _frames; ++i) {
		if (_textures[i]) _textures[i]->Release();
	}
	delete [] _textures;
}

bool Sprite::load(const std::string &path, int frame)
{
	if (FAILED(D3DXCreateTextureFromFileEx(
		Graphics::d3ddev,
		path.c_str(),
		0, 0, 0, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&_info,
		NULL,
		&_textures[frame]
	))) {
		_textures[frame] = NULL;
		return FALSE;
	}
	
	if (_vertices == NULL) {
		if (FAILED(Graphics::d3ddev->CreateVertexBuffer(
			4 * sizeof(SPRITEVERTEX),
			D3DUSAGE_WRITEONLY,
			SPRITEVERTEXFVF,
			D3DPOOL_MANAGED,
			&_vertices,
			NULL
		))) {
			return FALSE;
		}
	}

	SPRITEVERTEX *data;
	width = _info.Width; height = _info.Height;

	float halfwidth  = (float)width / 2.0f;
	float halfheight = (float)height / 2.0f;

	if (SUCCEEDED(_vertices->Lock(0, 0, (void **)&data, 0))) {
		data[0].p = D3DXVECTOR3(halfwidth, halfheight, 0.0f);
		data[0].u = 1.0f; data[0].v = 0.0f;

		data[1].p = D3DXVECTOR3(halfwidth, -halfheight, 0.0f);
		data[1].u = 1.0f; data[1].v = 1.0f;

		data[2].p = D3DXVECTOR3(-halfwidth, halfheight, 0.0f);
		data[2].u = 0.0f; data[2].v = 0.0f;

		data[3].p = D3DXVECTOR3(-halfwidth, -halfheight, 0.0f);
		data[3].u = 0.0f; data[3].v = 1.0f;

		_vertices->Unlock();
	} else {
		return FALSE;
	}

	return TRUE;
}

void Sprite::render(int frame)
{
	Graphics::d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Graphics::d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	for (int i = 0; i < 8; ++i) {
		Graphics::d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		Graphics::d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Graphics::d3ddev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	}
	*/
	Graphics::d3ddev->SetStreamSource(0, _vertices, 0, sizeof(SPRITEVERTEX));
	Graphics::d3ddev->SetFVF(SPRITEVERTEXFVF);

	if (frame >= 0 && frame < _frames && _textures[frame]) {
		Graphics::d3ddev->SetTexture(0, _textures[frame]);
	}

	Graphics::d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	Graphics::d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
