#include <gl/pogl.h>
#include <thread>
#include <atomic>
#include "POGLExampleWindow.h"

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;

	void main()
	{
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	#version 330

	void main()
	{
		gl_FragColor = vec4(1, 1, 1, 1);
	}
)" };

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 5: Threaded Vertices"));

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
		const POGL_POSITION_VERTEX VERTICES[] = {
			POGL_POSITION_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f)),
			POGL_POSITION_VERTEX(POGL_VECTOR3F(0.0f, 0.5f, 0.0f)),
			POGL_POSITION_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::DYNAMIC);

		std::atomic<bool> running(true);
		std::thread t([device, vertexBuffer, &running] {
			IPOGLDeviceContext* context = device->GetDeviceContext();
			while (running.load()) {
				// Lock
				// glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_UNSYNCHRONIZED_BIT​);
				// Fence
				// glMapBuffer
				// glUnmapBuffer​
				//POGL_POSITION_VERTEX* ptr = (POGL_POSITION_VERTEX*)context->Map(vertexBuffer, )
				IPOGLStream* stream = context->OpenStream(vertexBuffer, POGLStreamType::WRITE);
				POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)stream->GetPtr();
				vertices[0].position.x = -1.0f;
				stream->Release();
				
			}
			context->Release();
		});

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

		running.store(false);
		t.join();

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
