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

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vs_Color;
	}
)" };

int main()
{
	//
	// Create an example window. This function is NOT part of the main POGL library and is only usable for these examples.
	// You are responsible for creating a valid window to be drawn to on your own. 
	//

	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Draw Triangle"));

	POGL_DEVICE_INFO deviceInfo = { 0 };
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
		//
		// Retrieves the immediately device context.
		//

		IPOGLRenderContext* context = device->GetRenderContext();

		//
		// Load a very simple vertex- and fragment shader and link them into a a GPU program
		// usable when drawing the geometry onto the screen. 
		//
		// The array of IPOGLShader's must end with a nullptr (or 0).
		//

		IPOGLShader* vertexShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);

		//
		// We release the shader resources, since they are not needed anymore. You are allowed to reuse the shader resources 
		// for other programs if you want to.
		// 

		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create data for a triangle.
		//

		const POGL_POSITION_COLOR_VERTEX VERTICES[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-0.5f, -0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.0f, 0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.5f, -0.5f, 0.0f), POGL_COLOR4(0.0f, 0.0f, 1.0f, 1.0f))
		};

		// 
		// Create a vertex buffer based on the above vertices. This example makes use of one of the built-in vertex types.
		// See the later examples on how you can create your own vertex structures if you want.
		// 
		// The layout locations when using the built-in vertex types are:
		//
		// Position = 0
		// Color    = 1
		// TexCoord = 2
		// Normal   = 3
		// Tangent  = 4
		//
		// You are free to use any layout locations that you want if you create your own vertex types.
		//

		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Poll the opened window's events. This is NOT part of the POGL library
		//

		while (POGLProcessEvents()) {
			
			//
			// Prepare the program. The result is a state which is responsible for preventing unneccessary OpenGL state changes.
			//

			IPOGLRenderState* state = context->Apply(program);

			//
			// Clear the color and depth buffer on the screen
			// 

			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			// 
			// Bind the vertex buffer before drawing it. 
			// The IPOGLRenderState will prevent uneccessary state changes if you call this method multiple times with the same vertex buffer
			//

			state->Bind(vertexBuffer);

			//
			// Draw the triangle vertex buffer. This will automatically bind the vertex buffer if it's not already bound
			//

			state->Draw();

			//
			// Nofiy the rendering engine that we are finished using the bound program this frame.
			//

			state->Release();

			//
			// End the frame and swap the back buffer with the front buffer so that we can see the result onto the screen
			//

			device->EndFrame();
		}

		//
		// Release resources
		//

		vertexBuffer->Release();
		program->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();

	//
	// Destroy the example window. This is NOT part of the POGL library
	//

	POGLDestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
