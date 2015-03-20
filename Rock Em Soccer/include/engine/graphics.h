/*
 * <engine/graphics.h>
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_ENGINE_GRAPHICS__
#define __H_ENGINE_GRAPHICS__

#include <engine/define.h>
#include <engine/types.h>
#include <xanimator/xanimator.h>

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <engine/graphics/mesh.h>
#include <engine/graphics/light.h>
#include <engine/graphics/sprite.h>

class Graphics
{
public:
	/* init and end graphics subsystem */
	static bool init(uint32_t width, uint32_t height, bool fullscreen);
	static void end();

	/* renderer interface */
	static bool beginFrame();
	static void endFrame();
	static void presentFrame();
	static bool restore();
	static bool lost();

	static void depthMask(bool flag);
	static void lightingMask(bool flag);

	/*
	 * Matrix Transformations
	 */
	enum {
		modelMatrix = D3DTS_WORLD,
		viewMatrix = D3DTS_VIEW,
		projectionMatrix = D3DTS_PROJECTION
	};

	/* set transformation mode */
	static void matrixMode(int mode);

	/* reset transformation matrix */
	static void loadIdentity();

	/* construct perspective projectionmatrix */
	static void perspective(float fov, float aspect, float nearClip, float farClip);

	/* construct orthographic projection matrix */
	static void orthographic(float left, float right, float bottom, float top, float nearClip, float farClip);

	/* construct view matrix facing point */
	static void lookAt(float eyeX, float eyeY, float eyeZ,
	                   float atX, float atY, float atZ,
	                   float upX, float upY, float upZ);

	/* basic transformations */
	static void translate(float x, float y, float z);
	static void translate(D3DXVECTOR3 translation);
	static void rotate(float a, float x, float y, float z);
	static void rotate(D3DXQUATERNION orientation);
	static void scale(float x, float y, float z);
	static void scale(D3DXVECTOR3 scaling);
	static void scale(float s);
	static void setTransform(D3DXMATRIX transform);

	/* get current transformation */
	static D3DXMATRIX getTransform();
	static D3DXMATRIX getModelView();

	/* push at pop transformation matrices */
	static void pushMatrix();
	static void popMatrix();

	/*
	 * Variables
	 */
	static uint32_t viewWidth, viewHeight;
	static bool viewFullscreen;

	static IXAnimator *animator;

protected:
	friend class Light;
	friend class Mesh;
	friend class Sprite;

	static LPDIRECT3D9 d3d;
	static LPDIRECT3DDEVICE9 d3ddev;
	static D3DPRESENT_PARAMETERS d3dpp;
	static LPDIRECT3DSURFACE9 d3dbackbuffer;

	static LPD3DXMATRIXSTACK matrixStack;

private:
	static HRESULT createD3DDevice(HWND window, D3DDISPLAYMODE mode);
	static bool chooseDisplayMode(D3DDISPLAYMODE &mode);

	static D3DTRANSFORMSTATETYPE _matrixMode;
};

#endif /* __H_ENGINE_GRAPHICS__ */
