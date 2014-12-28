#include <gl/pogl.h>
#include <gl/poglmath.h>
#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std::chrono;

#include "POGLExampleWindow.h"

IPOGLProgram* LoadLinearDepthProgram(IPOGLRenderContext* context)
{
	IPOGLShader* vertexShader = context->CreateShaderFromFile(POGL_TOCHAR("lineardepth.vs"), POGLShaderType::VERTEX_SHADER);
	IPOGLShader* fragmentShader = context->CreateShaderFromFile(POGL_TOCHAR("lineardepth.fs"), POGLShaderType::FRAGMENT_SHADER);
	IPOGLShader* shaders[] = { vertexShader, fragmentShader };
	IPOGLProgram* linearDepthProgram = context->CreateProgramFromShaders(shaders, 2);
	vertexShader->Release();
	fragmentShader->Release();
	return linearDepthProgram;
}

IPOGLProgram* LoadResultProgram(IPOGLRenderContext* context)
{
	IPOGLShader* vertexShader = context->CreateShaderFromFile(POGL_TOCHAR("result.vs"), POGLShaderType::VERTEX_SHADER);
	IPOGLShader* fragmentShader = context->CreateShaderFromFile(POGL_TOCHAR("result.fs"), POGLShaderType::FRAGMENT_SHADER);
	IPOGLShader* shaders2[] = { vertexShader, fragmentShader };
	IPOGLProgram* resultProgram = context->CreateProgramFromShaders(shaders2, 2);
	vertexShader->Release();
	fragmentShader->Release();
	return resultProgram;
}

void CreateCubeBuffers(IPOGLRenderContext* context, IPOGLVertexBuffer** _out_vertexBuffer, IPOGLIndexBuffer** _out_indexBuffer)
{
	const POGL_POSITION_COLOR_VERTEX VERTICES[] = {
		// Front vertices
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-1.0f, -1.0f, 1.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(1.0f, -1.0f, 1.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(1.0f, 1.0f, 1.0f), POGL_COLOR4(0.0f, 0.0f, 1.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-1.0f, 1.0f, 1.0f), POGL_COLOR4(1.0f, 1.0f, 1.0f, 1.0f)),

		// Back vertices
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-1.0f, -1.0f, -1.0f), POGL_COLOR4(1.0f, 0.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(1.0f, -1.0f, -1.0f), POGL_COLOR4(0.0f, 1.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(1.0f, 1.0f, -1.0f), POGL_COLOR4(0.0f, 0.0f, 1.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3(-1.0f, 1.0f, -1.0f), POGL_COLOR4(1.0f, 1.0f, 1.0f, 1.0f))
	};
	*_out_vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::IMMUTABLE);

	const POGL_UINT32 INDICES[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1
	};
	*_out_indexBuffer = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_INT, POGLBufferUsage::IMMUTABLE);
}

void CreateFullscreenQuad(IPOGLRenderContext* context, IPOGLVertexBuffer** _out_vertexBuffer, IPOGLIndexBuffer** _out_indexBuffer)
{
	const POGL_POSITION_TEXCOORD_VERTEX FULLSCREEN_VERTICES[] = {
		POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-1.0f, -1.0f, 0.0f), POGL_VECTOR2(0.0f, 0.0f)),
		POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(-1.0f, 1.0f, 0.0f), POGL_VECTOR2(0.0f, 1.0f)),
		POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(1.0f, 1.0f, 0.0f), POGL_VECTOR2(1.0f, 1.0f)),
		POGL_POSITION_TEXCOORD_VERTEX(POGL_VECTOR3(1.0f, -1.0f, 0.0f), POGL_VECTOR2(1.0f, 0.0f))
	};
	*_out_vertexBuffer = context->CreateVertexBuffer(FULLSCREEN_VERTICES, sizeof(FULLSCREEN_VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::IMMUTABLE);

	const POGL_UINT8 INDICES[] = {
		0, 1, 2,
		2, 3, 0
	};
	*_out_indexBuffer = context->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_BYTE, POGLBufferUsage::IMMUTABLE);
}

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Linear Depth-Map"));

	// Create a POGL device based on the supplied information
	POGL_DEVICE_INFO deviceInfo = { 0 };
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	try {
		IPOGLRenderContext* context = device->GetRenderContext();

		//
		// Load the vertex- and fragment programs
		//

		IPOGLProgram* linearDepthProgram = LoadLinearDepthProgram(context);
		IPOGLProgram* resultProgram = LoadResultProgram(context);

		IPOGLVertexBuffer* cubeVertexBuffer = nullptr;
		IPOGLIndexBuffer* cubeIndexBuffer = nullptr;
		CreateCubeBuffers(context, &cubeVertexBuffer, &cubeIndexBuffer);

		IPOGLVertexBuffer* fullscreenVB = nullptr;
		IPOGLIndexBuffer* fullscreenIB = nullptr;
		CreateFullscreenQuad(context, &fullscreenVB, &fullscreenIB);

		//
		// Prepare and create a framebuffer to render to. 
		//
		// The linear depth value is drawn to the "linearDepthTexture". We do not change the gl_FragDepth (depthTexture) value because
		// most of the built-in driver optimizations that uses the we don't manually modify the z value.
		//

		IPOGLTexture2D* linearDepthTexture = context->CreateTexture2D(POGL_SIZE(1024, 768), POGLTextureFormat::RGBA32F, nullptr);
		IPOGLTexture2D* depthTexture = context->CreateTexture2D(POGL_SIZE(1024, 768), POGLTextureFormat::DEPTH24, nullptr);

		IPOGLTexture* textures[] = { linearDepthTexture };
		IPOGLFramebuffer* framebuffer = context->CreateFramebuffer(textures, 1, depthTexture);

		//
		// Release the depth texture. We don't really need it anymore
		//

		depthTexture->Release();

		//
		// Prepare the camera
		// 

		POGL_MAT4 perspective;
		const POGL_FLOAT FAR_CLIP_DISTANCE = 100.0f;
		POGLMat4Perspective(45.0f, 1.2f, 0.1f, FAR_CLIP_DISTANCE, &perspective);

		POGL_MAT4 lookAt;
		POGLMat4LookAt(POGL_VECTOR3(0.0f, 20.0f, 20.0f), POGL_VECTOR3(0.f, 0.f, 0.f), POGL_VECTOR3(0.0f, 1.0f, 0.0f), &lookAt);

		//
		// Prepare the static uniforms and properties
		//

		linearDepthProgram->FindUniformByName("ProjectionMatrix")->SetMatrix(perspective);
		linearDepthProgram->FindUniformByName("ViewMatrix")->SetMatrix(lookAt);
		linearDepthProgram->FindUniformByName("FarClipDistance")->SetFloat(FAR_CLIP_DISTANCE);
		linearDepthProgram->SetDepthTest(true);
		linearDepthProgram->SetDepthFunc(POGLDepthFunc::LESS);
		resultProgram->FindUniformByName("LinearDepthTexture")->SetTexture(linearDepthTexture);

		context->SetViewport(POGL_RECT(0, 0, 1024, 768));

		static const POGL_INT32 TOP_FPS = 1000 / 60; // Generate the linear depth map at max 60 FPS
		POGL_INT32 totalMillis = 0;
		auto start = high_resolution_clock::now();
		auto end = start;
		while (POGLProcessEvents()) {

			//
			// Generate the linear depth value
			//

			// Set the maximum FPS limit to 60 FPS
			if (totalMillis <= 0) {
				totalMillis = TOP_FPS;
				IPOGLRenderState* state = context->Apply(linearDepthProgram);
				state->SetFramebuffer(framebuffer);

				state->SetVertexBuffer(cubeVertexBuffer);
				state->SetIndexBuffer(cubeIndexBuffer);
				state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

				//
				// Retrieve the ModelMatrix for the currently bound program
				//

				IPOGLUniform* modelMatrixUniform = state->FindUniformByName("ModelMatrix");

				//
				// Draw 400 cubes over the screen
				//

				static const POGL_FLOAT OFFSET = -30.0f;
				for (POGL_UINT32 x = 0; x < 20; ++x) {
					for (POGL_UINT32 z = 0; z < 20; ++z) {
						POGL_MAT4 modelMatrix;
						POGLMat4Translate(POGL_VECTOR3(x * 3.0f + OFFSET, 0.0f, z * 3.0f + OFFSET), &modelMatrix);
						modelMatrixUniform->SetMatrix(modelMatrix);
						state->DrawIndexed();
					}
				}

				state->Release();

			}

			//
			// Draw the result onto the linear depth to the screen
			//

			IPOGLRenderState* state = context->Apply(resultProgram);
			state->SetFramebuffer(nullptr);
			
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			state->SetVertexBuffer(fullscreenVB);
			state->SetIndexBuffer(fullscreenIB);
			state->DrawIndexed();
			
			state->Release();

			//
			// End the frame and swap the display buffers
			//

			device->EndFrame();

			end = high_resolution_clock::now();
			totalMillis -= duration_cast<duration<POGL_UINT32, std::milli>>(end - start).count();
			start = end;
		}

		linearDepthTexture->Release();
		framebuffer->Release();
		cubeVertexBuffer->Release();
		cubeIndexBuffer->Release();
		fullscreenVB->Release();
		fullscreenIB->Release();
		resultProgram->Release();
		linearDepthProgram->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();

	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
