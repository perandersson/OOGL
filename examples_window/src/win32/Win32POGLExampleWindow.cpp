#include "POGLExampleWindow.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <chrono>
using namespace std::chrono;

POGL_HANDLE gPOGLWindowHandle = nullptr;
const POGL_CHAR* EXAMPLE_WINDOW_CLASS_NAME = POGL_TOCHAR("POGLExampleWindow");
POGL_STRING gTitle;

high_resolution_clock::time_point gPrev;
POGL_FLOAT gTotalTime = 0.0f;
POGL_FLOAT gTimeSinceLastTick = 0.0f;
POGL_UINT32 gNumFPS = 0;

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

	gPrev = high_resolution_clock::now();
	gTitle = title;
	gPOGLWindowHandle = window;
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

	// Move the timer cursor one tick forward
	high_resolution_clock::time_point now = high_resolution_clock::now();
	duration<POGL_FLOAT> time_span = now - gPrev;
	gPrev = now;
	gTimeSinceLastTick = time_span.count();

	gNumFPS++;
	gTotalTime += POGLGetTimeSinceLastTick();
	if (gTotalTime >= 1.0f) {
		gTotalTime = gTotalTime - 1.0f;
		POGL_CHAR tmp[10];
#ifdef _UNICODE
		_itow(gNumFPS, tmp, 10);
#else
		_itoa(gNumFPS, tmp, 10);
#endif
		gNumFPS = 0;
		SetWindowText((HWND)gPOGLWindowHandle, (gTitle + POGL_STRING(" - FPS: ") + POGL_STRING(tmp)).c_str());
	}
	return running;
}

void POGLAlert(const POGLException& e)
{
#pragma push_macro("GetMessage")
#undef GetMessage
	const POGL_STRING& message = e.GetMessage();
	MessageBox((HWND)gPOGLWindowHandle, message.c_str(), POGL_TOCHAR("Fatal Error"), MB_ICONERROR | MB_OK);
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

POGL_FLOAT POGLGetTimeSinceLastTick()
{
	return gTimeSinceLastTick;
}
