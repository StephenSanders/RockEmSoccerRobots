/*
 * <engine/graphics.cpp>
 * Stephen Sanders & Jake Singer
 */

#include <engine/debug.h>
#include <engine/graphics.h>

#if WINDOWS

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

uint32_t Graphics::viewWidth, Graphics::viewHeight;
bool Graphics::viewFullscreen;
LPDIRECT3D9 Graphics::d3d;
LPDIRECT3DDEVICE9 Graphics::d3ddev;
D3DPRESENT_PARAMETERS Graphics::d3dpp;
LPDIRECT3DSURFACE9 Graphics::d3dbackbuffer;
LPD3DXMATRIXSTACK Graphics::matrixStack;
IXAnimator *Graphics::animator;
D3DTRANSFORMSTATETYPE Graphics::_matrixMode;

bool Graphics::init(uint32_t width, uint32_t height, bool fullscreen)
{
	HRESULT result;
	viewWidth = width;
	viewHeight = height;
	viewFullscreen = fullscreen;

	if (!(d3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return FALSE;

	D3DDISPLAYMODE displaymode;
	if (viewFullscreen) {
		viewWidth = GetSystemMetrics(SM_CXSCREEN);
		viewHeight = GetSystemMetrics(SM_CYSCREEN);
		if (!chooseDisplayMode(displaymode))
			return FALSE;
	} else {
		displaymode.Format = D3DFMT_X8R8G8B8;
		displaymode.Width = viewWidth;
		displaymode.Height = viewHeight;
		displaymode.RefreshRate = 0;
	}

	result = createD3DDevice(GetActiveWindow(), displaymode);
	if (FAILED(result)) {
		d3ddev = NULL;
		return FALSE;
	}

	result = d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dbackbuffer);
	if (FAILED(result)) {
		d3dbackbuffer = NULL;
		return TRUE;
	}

	result = D3DXCreateMatrixStack(0, &matrixStack);
	if (FAILED(result)) {
		matrixStack = NULL;
		return TRUE;
	}
	_matrixMode = (D3DTRANSFORMSTATETYPE)modelMatrix;

	animator = CreateXAnimator(d3ddev);
	if (!animator)
		return FALSE;

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddev->SetRenderState(D3DRS_DITHERENABLE, TRUE);

	return TRUE;
}

void Graphics::end()
{
	delete animator;
	if (d3dbackbuffer) d3dbackbuffer->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

bool Graphics::beginFrame()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	loadIdentity();
	return SUCCEEDED(d3ddev->BeginScene());
}

void Graphics::endFrame()
{
	d3ddev->EndScene();
}

void Graphics::presentFrame()
{
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

bool Graphics::restore()
{
	d3dbackbuffer->Release();
	d3dbackbuffer = NULL;

	if (FAILED(d3ddev->Reset(&d3dpp)))
		return FALSE;

	if (FAILED(d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dbackbuffer)))
		return FALSE;

	return SUCCEEDED(d3ddev->TestCooperativeLevel());
}

bool Graphics::lost()
{
	return FAILED(d3ddev->TestCooperativeLevel());
}

void Graphics::depthMask(bool flag)
{
	d3ddev->SetRenderState(D3DRS_ZENABLE , flag);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

void Graphics::lightingMask(bool flag)
{
	d3ddev->SetRenderState(D3DRS_LIGHTING, flag);
}

void Graphics::matrixMode(int mode)
{
	_matrixMode = (D3DTRANSFORMSTATETYPE)mode;
}

void Graphics::loadIdentity()
{
	matrixStack->LoadIdentity();
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::perspective(float fov, float aspect, float nearClip, float farClip)
{
	D3DXMATRIXA16 projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(fov), aspect, nearClip, farClip);
	matrixStack->MultMatrixLocal(&projection);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::orthographic(float left, float right, float bottom, float top, float nearClip, float farClip)
{
	D3DXMATRIXA16 projection;
	D3DXMatrixOrthoOffCenterLH(&projection, left, right, bottom, top, nearClip, farClip);
	matrixStack->MultMatrixLocal(&projection);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::lookAt(float eyeX, float eyeY, float eyeZ,
                      float atX, float atY, float atZ,
                      float upX, float upY, float upZ)
{
	D3DXMATRIXA16 projection;
	D3DXMatrixLookAtLH(
		&projection,
		&D3DXVECTOR3(eyeX, eyeY, eyeZ),
		&D3DXVECTOR3(atX, atY, atZ),
		&D3DXVECTOR3(upX, upY, upZ)
	);
	matrixStack->MultMatrixLocal(&projection);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::translate(float x, float y, float z)
{
	D3DXMATRIX translation = *(matrixStack->GetTop());
	D3DXMatrixTranslation(&translation, x, y, z);
	matrixStack->MultMatrixLocal(&translation);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::translate(D3DXVECTOR3 translation)
{
	translate(translation.x, translation.y, translation.z);
	
}

void Graphics::rotate(float a, float x, float y, float z)
{
	D3DXMATRIX rotation = *(matrixStack->GetTop());
	a = D3DXToRadian(a);
	D3DXMatrixRotationAxis(&rotation, &D3DXVECTOR3(x, y, z), a);
	matrixStack->MultMatrixLocal(&rotation);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::rotate(D3DXQUATERNION orientation)
{
	D3DXMATRIX rotation = *(matrixStack->GetTop());
	D3DXMatrixRotationQuaternion(&rotation, &orientation);
	matrixStack->MultMatrixLocal(&rotation);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::scale(float x, float y, float z)
{
	D3DXMATRIX scaling = *(matrixStack->GetTop());
	D3DXMatrixScaling(&scaling, x, y, z);
	matrixStack->MultMatrixLocal(&scaling);
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::scale(D3DXVECTOR3 scaling)
{
	scale(scaling.x, scaling.y, scaling.z);
}

void Graphics::scale(float s)
{
	scale(s, s, s);
}

void Graphics::setTransform(D3DXMATRIX transform)
{
	d3ddev->SetTransform(_matrixMode, &transform);
}

D3DXMATRIX Graphics::getTransform()
{
	return *(matrixStack->GetTop());
}

D3DXMATRIX Graphics::getModelView()
{
	D3DXMATRIX modelView, world, view;
	d3ddev->GetTransform(D3DTS_WORLD, &world);
	d3ddev->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixMultiply(&modelView, &world, &view);
	return modelView;
}

void Graphics::pushMatrix()
{
	matrixStack->Push();
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

void Graphics::popMatrix()
{
	matrixStack->Pop();
	d3ddev->SetTransform(_matrixMode, matrixStack->GetTop());
}

/*
 * Private Members
 */

HRESULT Graphics::createD3DDevice(HWND window, D3DDISPLAYMODE mode)
{
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = mode.Width;
	d3dpp.BackBufferHeight           = mode.Height;
	d3dpp.BackBufferFormat           = mode.Format;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;//(viewFullscreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD);
	d3dpp.hDeviceWindow              = window;
	d3dpp.Windowed                   = !viewFullscreen;
	d3dpp.EnableAutoDepthStencil     = TRUE;
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;
	d3dpp.Flags                      = NULL;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//mode.RefreshRate * (UINT)viewFullscreen;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE; //D3DPRESENT_INTERVAL_DEFAULT;
/*
	if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R8G8B8, TRUE, D3DMULTISAMPLE_4_SAMPLES, NULL)) {
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	}
*/
	return d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
	);
}

bool Graphics::chooseDisplayMode(D3DDISPLAYMODE &mode)
{
	D3DFORMAT format = D3DFMT_X8R8G8B8;
	int numGraphicsModes = d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, format);
	if (numGraphicsModes <= 0)
		return FALSE;

	bool found = FALSE;
	D3DDISPLAYMODE temp;
	for (int i = 0; i < numGraphicsModes && !found; ++i) {
		d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, format, i, &temp);
		if (temp.Width == viewWidth && temp.Height == viewHeight) {
			mode = temp;
			found = TRUE;
		}
	}

	return found;
}

#endif /* WINDOWS */
