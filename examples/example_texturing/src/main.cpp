#include <gl/pogl.h>
#include <gl/poglext.h>
#include <thread>
#include "POGLExampleWindow.h"

int main()
{
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Texturing"));
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
		IPOGLRenderContext* context = device->GetRenderContext();

		IPOGLShader* vertexShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.vs"), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.fs"), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);
		vertexShader->Release();
		fragmentShader->Release();

		// 
		// Create a quad vertex buffer with the appropriate texture coordinates. This example makes use of one of the built-in vertex types.
		// See the later examples on how you can create your own vertex structures if you want.
		//

		const POGL_POSITION_TEXCOORD_VERTEX VERTICES[] = {
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-0.5f, -0.5f, 0.0f), POGL_VECTOR2(0.0f, 0.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-0.5f, 0.5f, 0.0f), POGL_VECTOR2(0.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(0.5f, 0.5f, 0.0f), POGL_VECTOR2(1.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(0.5f, -0.5f, 0.0f), POGL_VECTOR2(1.0f, 0.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Create an index buffer usable when drawing the vertex buffer above
		//

		const POGL_UINT8 INDICES[] = {
			0, 1, 2,
			2, 3, 0
		};
		IPOGLIndexBuffer* indexBuffers = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_BYTE, POGLBufferUsage::STATIC);

		//
		// Load a BMP file and return a usable texture resource. This is part of the extended POGL library.
		//
		IPOGLTexture2D* texture = POGLXLoadBMPImageFromFile(context, POGL_TOCHAR("texture.bmp"));

		while (POGLProcessEvents()) {
			IPOGLRenderState* state = context->Apply(program);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			//
			// Locate the "Texture" uniform (see fragment shader above) and set the texture to it. As described before, all 
			// unneccessary "state" changes made on the IPOGLRenderState level is prevented so the texture is
			// only bound once. 
			//
			// If the texture is already bound then the only thing the "SetTexture" method does is a couple of simple integer
			// comparisons. The IPOGLUniform instance returned by the render state is RenderState specific, which means that you
			// are not allowed to use it on anything other than when rendering on the state returned by the IPOGLRenderContext::Apply(IPOGLProgram*)
			// method above. 

			state->FindUniformByName("Texture")->SetTexture(texture);

			state->SetVertexBuffer(vertexBuffer);
			state->SetIndexBuffer(indexBuffers);
			state->DrawIndexed();
			state->Release();
			device->EndFrame();
		}

		//
		// Release resources
		//

		texture->Release();
		indexBuffers->Release();
		vertexBuffer->Release();
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
