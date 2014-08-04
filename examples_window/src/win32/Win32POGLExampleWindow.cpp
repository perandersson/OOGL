#include "POGLExampleWindow.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

POGL_HANDLE gPOGLWindowHandle = nullptr;
const POGL_CHAR* EXAMPLE_WINDOW_CLASS_NAME = POGL_TOCHAR("POGLExampleWindow");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_ACTIVATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

POGL_HANDLE POGLCreateExampleWindow(const POGL_SIZEI& size, const POGL_STRING& title)
{
	HINSTANCE applicationHandle = GetModuleHandle(NULL);

	WNDCLASSEX windowProperties = { 0 };
	windowProperties.cbSize = sizeof(WNDCLASSEX);
	windowProperties.style = CS_HREDRAW | CS_VREDRAW;
	windowProperties.lpfnWndProc = WindowProc;
	windowProperties.hInstance = applicationHandle;
	windowProperties.lpszClassName = EXAMPLE_WINDOW_CLASS_NAME;
	windowProperties.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassEx(&windowProperties);

	const DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	const DWORD style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;

	RECT windowSize = { 0, 0, (LONG)size.x, (LONG)size.y };
	AdjustWindowRectEx(&windowSize, style, false, exStyle);

	HWND window = CreateWindowEx(exStyle, EXAMPLE_WINDOW_CLASS_NAME, title.c_str(), style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
		windowSize.right - windowSize.left, windowSize.bottom - windowSize.top,
		NULL, NULL, applicationHandle, NULL);

	if (window != NULL) {
		ShowWindow(window, SW_SHOW);
		SetForegroundWindow(window);
		SetFocus(window);
		UpdateWindow(window);
	}

	return window;
}

MSG gPOGLMessageQueue;
bool POGLProcessEvents()
{
	bool running = true;
	while (PeekMessage(&gPOGLMessageQueue, NULL, 0, 0, PM_REMOVE)) {
		if (gPOGLMessageQueue.message == WM_QUIT) {
			running = false;
			return false;
		}
		TranslateMessage(&gPOGLMessageQueue);
		DispatchMessage(&gPOGLMessageQueue);
	}
	return running;
}

void POGLAlert(POGL_HANDLE windowHandle, const POGLException& e)
{
#pragma push_macro("GetMessage")
#undef GetMessage
	const POGL_STRING& message = e.GetMessage();
	MessageBox((HWND)windowHandle, message.c_str(), POGL_TOCHAR("Fatal Error"), MB_ICONERROR | MB_OK);
#pragma pop_macro("GetMessage")
}

void POGLDestroyExampleWindow(POGL_HANDLE windowHandle)
{
	if (windowHandle != NULL) {
		CloseWindow((HWND)windowHandle);
	}

	HINSTANCE applicationHandle = GetModuleHandle(NULL);
	UnregisterClass(EXAMPLE_WINDOW_CLASS_NAME, applicationHandle);
}

