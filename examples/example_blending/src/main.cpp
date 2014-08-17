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
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOSTRING("Example: Blending"));
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
		IPOGLDeviceContext* context = device->GetDeviceContext();

		IPOGLShader* vertexShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create two triangles with different colors.
		//

		const POGL_POSITION_COLOR_VERTEX VERTICES[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-0.5f, -0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.0f, 0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.5f, -0.5f, 0.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		const POGL_POSITION_COLOR_VERTEX VERTICES_INV[] = {
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.5f, 0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(0.0f, -0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
			POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-0.5f, 0.5f, 0.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f))
		};
		IPOGLVertexBuffer* vertexBufferInv = context->CreateVertexBuffer(VERTICES_INV, sizeof(VERTICES_INV), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Setup effect properties. This can be done on the associated RenderState as well, but since we want this to be applied for all
		// contexts when this applied we do it on the effect instead. 
		//
		// Changes made on the program will NOT be applied until the next time we call IPOGLDeviceContext::Apply(IPOGLProgram*)
		//

		program->SetBlend(true);
		program->SetBlendFunc(POGLSrcFactor::SRC_COLOR, POGLDstFactor::ONE_MINUS_SRC_COLOR);

		while (POGLProcessEvents()) {
			IPOGLRenderState* state = context->Apply(program);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			//
			// Draw the two triangles
			//

			state->Draw(vertexBuffer);
			state->Draw(vertexBufferInv);

			state->Release();
			device->EndFrame();
		}

		//
		// Release resources
		//

		vertexBuffer->Release();
		vertexBufferInv->Release();
		program->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();
	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
