#include <gl/pogl.h>
#include <gl/poglext.h>
#include <thread>
#include "POGLExampleWindow.h"

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;
	layout(location = 2) in vec2 texCoord;

	out vec2 vs_TexCoord;

	void main()
	{
		vs_TexCoord = texCoord;
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	#version 330

	uniform sampler2D Texture;

	in vec2 vs_TexCoord;

	void main()
	{
		gl_FragColor = texture(Texture, vs_TexCoord);
	}
)" };

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 4: Texturing"));

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
		const POGL_POSITION_TEXCOORD_VERTEX VERTICES[] = {
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), POGL_VECTOR2F(0.0f, 0.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(-0.5f, 0.5f, 0.0f), POGL_VECTOR2F(0.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(0.5f, 0.5f, 0.0f), POGL_VECTOR2F(1.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), POGL_VECTOR2F(1.0f, 0.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		// Create index buffers
		const POGL_UINT8 INDICES[] = {
			0, 1, 2,
			2, 3, 0
		};
		IPOGLIndexBuffer* indexBuffers = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_BYTE, POGLBufferUsage::STATIC);

		// Create texture
		IPOGLTexture2D* texture = POGLLoadTextureFromFile(context, POGL_TOCHAR("texture.bmp"));

		while (POGLProcessEvents()) {
			// Prepare the simple effect
			IPOGLRenderState* state = context->Apply(simpleEffect);

			// Clear the color and depth buffer
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			// Set uniforms
			state->FindUniformByName("Texture")->SetTexture(texture);

			// Draw the vertices inside the vertex buffer
			state->Draw(vertexBuffer, indexBuffers);

			// Nofiy the rendering engine that we are finished using the bound effect this frame
			state->EndFrame();

			// Swap the back buffer with the front buffer
			device->SwapBuffers();
		}

		// Release resources
		texture->Release();
		indexBuffers->Release();
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
