﻿#include <gl/pogl.h>
#include <gl/poglext.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cmath>
#include <chrono>
#include "POGLExampleWindow.h"

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Load Textures from thread"));

	// Create a POGL device based on the supplied information
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
		// Create a fullscreen quad vertex buffer with the appropriate texture coordinates.
		//

		const POGL_POSITION_TEXCOORD_VERTEX VERTICES[] = {
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(1.0f, -1.0f, 0.0f), POGL_VECTOR2(0.0f, 0.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(1.0f, 1.0f, 0.0f), POGL_VECTOR2(0.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-1.0f, 1.0f, 0.0f), POGL_VECTOR2(1.0f, 1.0f)),
			POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-1.0f, -1.0f, 0.0f), POGL_VECTOR2(1.0f, 0.0f))
		};
		IPOGLVertexBuffer* fullscreenVB = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		//
		// Create an index buffer usable when drawing the vertex buffers above
		//

		const POGL_UINT8 INDICES[] = {
			0, 1, 2,
			2, 3, 0
		};
		IPOGLIndexBuffer* fullscreenIB = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_BYTE, POGLBufferUsage::STATIC);

		//
		// Create a thread with the purpose of loading the four textures. 
		//

		IPOGLDeferredRenderContext* deferredContext = device->CreateDeferredRenderContext();
		std::atomic<IPOGLTexture2D*> texture0;
		std::atomic<IPOGLTexture2D*> texture1;
		std::atomic<IPOGLTexture2D*> texture2;
		std::atomic<IPOGLTexture2D*> texture3;
		std::thread t([&texture0, &texture1, &texture2, &texture3, deferredContext] {
			//
			// Load the texture after one second
			//

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			texture0 = POGLXLoadBMPImageFromFile(deferredContext, "texture0.bmp");

			//
			// Use flush instead of FlushAndWait. We are allowed to use Flush because we have a limited set of commands
			// generated by this thread. The Flush method is perfect for resource loading.
			//

			deferredContext->Flush();


			//
			// Load the texture after one second
			//

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			texture1 = POGLXLoadBMPImageFromFile(deferredContext, "texture1.bmp");

			//
			// Flush
			//

			deferredContext->Flush();

			//
			// Load the texture after one second
			//

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			texture2 = POGLXLoadBMPImageFromFile(deferredContext, "texture2.bmp");

			//
			// Flush
			//

			deferredContext->Flush();

			//
			// Load the texture after one second
			//

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			texture3 = POGLXLoadBMPImageFromFile(deferredContext, "texture3.bmp");

			//
			// Flush
			//

			deferredContext->Flush();
		});

		while (POGLProcessEvents()) {
			//
			// Execute the commands generated by the deferred context
			//
			
			deferredContext->ExecuteCommands(context);

			//
			// Draw the textures onto the screen
			//

			IPOGLRenderState* state = context->Apply(program);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			auto textureUniform = state->FindUniformByName("Texture");
			auto translateUniform = state->FindUniformByName("Translate");
			auto scaleUniform = state->FindUniformByName("Scale");

			textureUniform->SetTexture(texture0);
			translateUniform->SetFloat(-1.0f, 1.0f);
			scaleUniform->SetFloat(0.5f, 0.5f);
			state->SetVertexBuffer(fullscreenVB);
			state->SetIndexBuffer(fullscreenIB);
			state->DrawIndexed();

			textureUniform->SetTexture(texture1);
			translateUniform->SetFloat(1.0f, 1.0f);
			scaleUniform->SetFloat(0.5f, 0.5f);
			state->DrawIndexed();

			textureUniform->SetTexture(texture2);
			translateUniform->SetFloat(-1.0f, -1.0f);
			scaleUniform->SetFloat(0.5f, 0.5f);
			state->DrawIndexed();

			textureUniform->SetTexture(texture3);
			translateUniform->SetFloat(1.0f, -1.0f);
			scaleUniform->SetFloat(0.5f, 0.5f);
			state->DrawIndexed();

			state->Release();

			//
			// End the current frame
			//

			device->EndFrame();
		}

		t.join();
		texture0.load()->Release();
		texture1.load()->Release();
		texture2.load()->Release();
		texture3.load()->Release();

		program->Release();
		deferredContext->Release();
		fullscreenVB->Release();
		fullscreenIB->Release();
		context->Release();
		device->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	// Destroy the example window
	POGLDestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
