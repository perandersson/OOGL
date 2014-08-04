#include <gl/pogl.h>
#include <thread>
#include "POGLExampleWindow.h"

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

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 2: Create Triangle"));

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
		const POGL_POSITION_COLOR_VERTEX VERTICES[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), POGL_COLOR4F(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.0f, 0.5f, 0.0f), POGL_COLOR4F(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), POGL_COLOR4F(0.0f, 0.0f, 1.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		while (POGLProcessEvents()) {
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
		POGLAlert(windowHandle, e);
	}

	device->Release();

	// Destroy the example window
	POGLDestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
