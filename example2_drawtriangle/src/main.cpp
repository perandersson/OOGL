#include <gl/pogl.h>
#include <thread>

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;
	layout(location = 1) in vec4 color;

	out vec4 vs_Color;

	void main()
	{
		vs_Color = color;
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	#version 330

	in vec4 vs_Color;

	void main()
	{
		gl_FragColor = vs_Color;
	}
)" };

POGL_HANDLE windowHandle = nullptr;
const POGL_CHAR* CLASS_NAME = POGL_TOCHAR("example2_drawtriangle");

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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

void DestroyExampleWindow(POGL_HANDLE windowHandle)
{
	if (windowHandle != NULL) {
		CloseWindow((HWND)windowHandle);
	}

	HINSTANCE applicationHandle = GetModuleHandle(NULL);
	UnregisterClass(CLASS_NAME, applicationHandle);
}

HWND CreateExampleWindow(const POGL_SIZEI& size, const POGL_STRING& title)
{
	HINSTANCE applicationHandle = GetModuleHandle(NULL);

	WNDCLASSEX windowProperties = { 0 };
	windowProperties.cbSize = sizeof(WNDCLASSEX);
	windowProperties.style = CS_HREDRAW | CS_VREDRAW;
	windowProperties.lpfnWndProc = WindowProc;
	windowProperties.hInstance = applicationHandle;
	windowProperties.lpszClassName = CLASS_NAME;
	windowProperties.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassEx(&windowProperties);

	const DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	const DWORD style = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;

	RECT windowSize = { 0, 0, (LONG)size.x, (LONG)size.y };
	AdjustWindowRectEx(&windowSize, style, false, exStyle);

	HWND window = CreateWindowEx(exStyle, CLASS_NAME, title.c_str(), style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
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

MSG messageQueue;
bool ProcessEvents()
{
	bool running = true;
	while (PeekMessage(&messageQueue, NULL, 0, 0, PM_REMOVE)) {
		if (messageQueue.message == WM_QUIT) {
			running = false;
			return false;
		}
		TranslateMessage(&messageQueue);
		DispatchMessage(&messageQueue);
	}
	return running;
}

void Alert(POGL_HANDLE windowHandle, const POGL_STRING& message)
{
	MessageBox((HWND)windowHandle, message.c_str(), POGL_TOCHAR("Fatal Error"), MB_ICONERROR | MB_OK);
}

#else
#error IMPLEMENT NON-WINDOWS
#endif

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = CreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 2: Create Triangle"));

	// Create a POGL device based on the supplied information
	POGL_DEVICE_INFO deviceInfo;
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = 0;
#endif
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	try {
		// Create a device context for main thread
		IPOGLDeviceContext* context = device->GetDeviceContext();

		// Create an effect based on the supplied vertex- and fragment shader
		IPOGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderProgramType::VERTEX_SHADER);
		IPOGLShaderProgram* fragmentShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderProgramType::FRAGMENT_SHADER);
		IPOGLShaderProgram* programs[] = { vertexShader, fragmentShader };
		IPOGLEffect* simpleEffect = context->CreateEffectFromPrograms(programs, 2);
		vertexShader->Release();
		fragmentShader->Release();

		// Create vertex buffer
		POGL_POSITION_COLOR_VERTEX vertices[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), POGL_COLOR4F(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), POGL_COLOR4F(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.0f, 0.5f, 0.0f), POGL_COLOR4F(0.0f, 0.0f, 1.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(vertices, sizeof(vertices), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		while (ProcessEvents()) {
			// Prepare the simple effect
			IPOGLRenderState* state = context->Apply(simpleEffect);

			// Clear the color and depth buffer
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			// Draw the vertices inside the vertex buffer
			state->Draw(vertexBuffer);

			// Nofiy the rendering engine that we are finished using the bound effect this frame
			state->EndFrame();

			// Swap the back buffer with the front buffer
			device->SwapBuffers();
		}

		// Release resources
		vertexBuffer->Release();
		simpleEffect->Release();
		context->Release();
	}
	catch (POGLException e) {
#pragma push_macro("GetMessage")
#undef GetMessage
		Alert(windowHandle, e.GetMessage());
#pragma pop_macro("GetMessage")
	}


	device->Release();

	DestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
