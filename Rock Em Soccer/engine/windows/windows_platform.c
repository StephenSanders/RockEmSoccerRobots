/*
 * windows_platform.c
 * Stephen Sanders & Jake Singer
 */

#include <engine/platform.h>

#if WINDOWS

#include <windows.h>
#include <windowsx.h>

static uint32_t  _mode = 0;
static HINSTANCE _instance = 0;
static HWND      _window = 0;

/*
 * Windows Specific Functionality
 */

/* main entry point for the engine */
int WINAPI
WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmd, int mode)
{
	UNREFERENCED_PARAMETER(prev);
	_instance = instance;

	/* TODO: hangle arguments and display flags */

	/* hand off responsibility to the game */
	return __engine_main(0, 0);
}

LRESULT CALLBACK
WinProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		case WM_CLOSE:
			DestroyWindow(window);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATEAPP:
			if ((_mode & PLATFORM_FULLSCREEN) && !(_mode & PLATFORM_WINDOWED)) {
				if (wparam) {
					ShowWindowAsync(window, SW_SHOW);
				} else {
					ShowWindowAsync(window, SW_SHOWMINIMIZED);
				}
			}
			break;
		case WM_MOVE:
			break;
		case WM_MOVING:
			break;
		case WM_KEYDOWN: {
			} break;
		default:
			return DefWindowProc(window, msg, wparam, lparam);
	}
	return 0L;
}

/*
 * Platform Functionality
 */

int
platform_init(const char *name, uint32_t width, uint32_t height, uint32_t mode)
{
	WNDCLASSEX wc;
	DWORD style, style_ex;
	RECT rect;

	/* initial window sizing */
	rect.left   = (LONG)0;
	rect.top    = (LONG)0;
	rect.right  = (LONG)width;
	rect.bottom = (LONG)height;

	/* setup window class */
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize        = sizeof(wc);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WinProc;
	wc.hInstance     = _instance;
	wc.hIcon         = LoadIcon(_instance, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = "GameWindow";
	wc.hIconSm       = LoadIcon(_instance, IDI_APPLICATION);

	/* register window */
	if (!RegisterClassEx(&wc)) {
		return -1;
	}

	/* set window and display properties based on window mode */
	if (mode & PLATFORM_FULLSCREEN) {
		if (mode & PLATFORM_WINDOWED) {
			rect.right  = width  = GetSystemMetrics(SM_CXSCREEN);
			rect.bottom = height = GetSystemMetrics(SM_CYSCREEN);

			style_ex = WS_EX_APPWINDOW;
		} else {
			/* TODO: fix fullscreen capture */
			DEVMODE display;
			display.dmSize       = sizeof(display);
			display.dmPelsWidth  = GetSystemMetrics(SM_CXSCREEN);
			display.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
			display.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&display, CDS_FULLSCREEN)
				!= DISP_CHANGE_SUCCESSFUL
			) {
				return -1;
			}

			style_ex = WS_EX_TOPMOST;
		}

		style    = WS_POPUP;
		ShowCursor(FALSE);
	} else {
		style_ex = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style    = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	}

	/* adjust sizing for window border */
	AdjustWindowRectEx(&rect, style, 0, style_ex);

	/* setup the window */
	if (!(_window = CreateWindowEx(
		style_ex,
		"GameWindow", name,
		style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		(GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2,
		rect.right - rect.left, rect.bottom - rect.top,
		0, 0, _instance, 0
	))) {
		return -1;
	}

	/* get window up and running */
	if (mode & PLATFORM_HIDDEN) {
		ShowWindowAsync(_window, SW_SHOWMINIMIZED);
	} else {
		ShowWindowAsync(_window, SW_SHOW);
	}
	UpdateWindow(_window);

	/* These needed? */
	SetForegroundWindow(_window);
	SetFocus(_window);

	_mode = mode;
	return 0;
}

int
platform_update(void)
{
	MSG msg = {0};
	if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
		if (!GetMessage(&msg, 0, 0, 0)) {
			return 1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

	return 0;
}

int
platform_release(void)
{
	UnregisterClass("GameWindow", _instance);
	return 0;
}

#endif /* WINDOWS */
