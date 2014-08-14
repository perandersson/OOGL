#include <gl/pogl.h>
#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include "POGLExampleWindow.h"

static const POGL_CHAR DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;

	void main()
	{
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_FS[] = { R"(
	#version 330

	// Output color for the render target location "0" defined when creating the framebuffer
	layout(location = 0) out vec4 texture0;

	// Output color for the render target location "1" defined when creating the framebuffer
	layout(location = 1) out vec4 texture1;

	void main()
	{
		// Draw red pixels to framebuffer location "0"
		texture0 = vec4(1, 0, 0, 1);

		// Draw blue pixels to framebuffer location "1"
		texture1 = vec4(0, 0, 1, 1);
	}
)" };

static const POGL_CHAR TEXTURING_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;
	layout(location = 2) in vec2 texCoord;

	out vec2 vs_TexCoord;

	void main()
	{
		vs_TexCoord = texCoord;
		gl_Position = vec4(position, 1.0);
	}
)" };

static const POGL_CHAR TEXTURING_FS[] = { R"(
	#version 330

	// Uniform texture
	uniform sampler2D Texture0;

	// Uniform texture
	uniform sampler2D Texture1;

	in vec2 vs_TexCoord;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = texture(Texture0, vs_TexCoord) + texture(Texture1, vs_TexCoord);
	}
)" };

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 7: Framebuffers"));

	// Create a POGL device based on the supplied information
	POGL_DEVICE_INFO deviceInfo = { 0 };
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#endif
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	try {
		IPOGLDeviceContext* context = device->GetDeviceContext();
		
		//
		// Load the vertex- and fragment shader used to render to the framebuffer render targets
		//

		IPOGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_VS, 
			sizeof(DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_VS), POGLShaderProgramType::VERTEX_SHADER);
		IPOGLShaderProgram* fragmentShader = context->CreateShaderProgramFromMemory(DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_FS, 
			sizeof(DRAW_VERTICES_TO_FRAMEBUFFER_EFFECT_FS), POGLShaderProgramType::FRAGMENT_SHADER);
		IPOGLShaderProgram* programs[] = { vertexShader, fragmentShader };
		IPOGLEffect* framebufferEffect = context->CreateEffectFromPrograms(programs, 2);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Load the vertex- and fragment shader used to render the resulted framebuffer to the screen
		//

		vertexShader = context->CreateShaderProgramFromMemory(TEXTURING_VS, sizeof(TEXTURING_VS), POGLShaderProgramType::VERTEX_SHADER);
		fragmentShader = context->CreateShaderProgramFromMemory(TEXTURING_FS, sizeof(TEXTURING_FS), POGLShaderProgramType::FRAGMENT_SHADER);
		IPOGLShaderProgram* programs2[] = { vertexShader, fragmentShader };
		IPOGLEffect* resultEffect = context->CreateEffectFromPrograms(programs2, 2);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create something to render to the framebuffer
		//

		const POGL_POSITION_VERTEX VERTICES[] = {
			POGL_POSITION_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f)),
			POGL_POSITION_VERTEX(POGL_VECTOR3F(0.0f, 0.5f, 0.0f)),
			POGL_POSITION_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f))
		};
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Create a fullscreen quad
		//

		const POGL_POSITION_TEXCOORD_VERTEX FULLSCREEN_VERTICES[] = {
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), POGL_VECTOR2F(0.0f, 0.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(-0.5f, 0.5f, 0.0f), POGL_VECTOR2F(0.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(0.5f, 0.5f, 0.0f), POGL_VECTOR2F(1.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), POGL_VECTOR2F(1.0f, 0.0f))
		};
		IPOGLVertexBuffer* fullscreenVB = context->CreateVertexBuffer(FULLSCREEN_VERTICES, sizeof(FULLSCREEN_VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		const POGL_UINT8 INDICES[] = {
			0, 1, 2,
			2, 3, 0
		};
		IPOGLIndexBuffer* fullscreenIB = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_BYTE, POGLBufferUsage::STATIC);

		//
		// Prepare and create a framebuffer to render to
		//

		IPOGLTexture2D* texture0 = context->CreateTexture2D(POGL_SIZEI(1024, 768), POGLTextureFormat::RGBA32F, nullptr);
		IPOGLTexture2D* texture1 = context->CreateTexture2D(POGL_SIZEI(1024, 768), POGLTextureFormat::RGBA32F, nullptr);
		IPOGLTexture2D* depthTexture = context->CreateTexture2D(POGL_SIZEI(1024, 768), POGLTextureFormat::DEPTH24, nullptr);

		//
		// Create a framebuffer that renders to the following textures. The index in the 
		// textures array defines the color location used in the fragment shader
		//

		IPOGLTexture* textures[] = { texture0, texture1 };
		IPOGLFramebuffer* framebuffer = context->CreateFramebuffer(textures, 2, depthTexture);

		//
		// Release the textures. We don't really need them at this point anymore
		//

		texture0->Release(); texture1->Release(); depthTexture->Release();

		while (POGLProcessEvents()) {
			
			//
			// Bind the effect used when drawing to the framebuffer
			//

			IPOGLRenderState* state = context->Apply(framebufferEffect);
			state->SetFramebuffer(framebuffer);
			state->SetViewport(POGL_RECTI(0, 0, 1024, 768));
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);
			state->Draw(vertexBuffer);
			state->Release();

			//
			// Bind the effect used when drawing the textures generated by the effect above
			// and draw them on a fullscreen quad
			//

			state = context->Apply(resultEffect);
			state->SetFramebuffer(nullptr);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);
			
			IPOGLTexture* texture0 = framebuffer->GetTexture(0);
			state->FindUniformByName("Texture0")->SetTexture(texture0); 
			texture0->Release();

			IPOGLTexture* texture1 = framebuffer->GetTexture(1);
			state->FindUniformByName("Texture1")->SetTexture(texture1);
			texture1->Release();

			state->Draw(fullscreenVB, fullscreenIB);
			state->Release();

			//
			// End the frame and swap the display buffers
			//

			device->EndFrame();
		}

		framebuffer->Release();
		vertexBuffer->Release();
		fullscreenVB->Release();
		fullscreenIB->Release();
		resultEffect->Release();
		framebufferEffect->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();

	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
