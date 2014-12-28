#include <gl/pogl.h>
#include <gl/poglmath.h>
#include <thread>
#include <atomic>
#include "POGLExampleWindow.h"

int main()
{
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Draw Cube"));

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

		//
		// Set viewport
		//

		context->SetViewport(POGL_RECT(0, 0, 1024, 768));

		std::atomic<bool> running(true);
		std::atomic<POGL_FLOAT> angle(0.0f);

		IPOGLDeferredRenderContext* deferredContext = device->CreateDeferredRenderContext();
		std::condition_variable condition;
		std::thread t([deferredContext, &angle, &condition, &running] {
			try {
				//
				// Create vertex data for a cube.
				//

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
				IPOGLVertexBuffer* vertexBuffer = deferredContext->CreateVertexBuffer(VERTICES, sizeof(VERTICES), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::IMMUTABLE);

				//
				// Create the index buffer for the cube
				//

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
				IPOGLIndexBuffer* indexBuffer = deferredContext->CreateIndexBuffer(INDICES, sizeof(INDICES), POGLVertexType::UNSIGNED_INT, POGLBufferUsage::IMMUTABLE);

				// 
				// Create the shaders in this thread and use it as if it was created in the main thread
				//
				// Please note that the program isn't actually created until the deferred render context is
				// executed in the main thread.
				//

				IPOGLShader* vertexShader = deferredContext->CreateShaderFromFile(POGL_TOCHAR("simple.vs"), POGLShaderType::VERTEX_SHADER);
				IPOGLShader* fragmentShader = deferredContext->CreateShaderFromFile(POGL_TOCHAR("simple.fs"), POGLShaderType::FRAGMENT_SHADER);
				IPOGLShader* shaders[] = { vertexShader, fragmentShader };
				IPOGLProgram* program = deferredContext->CreateProgramFromShaders(shaders, 2);
				vertexShader->Release();
				fragmentShader->Release();

				//
				// Generate a perspective- and lookat matrix
				//

				POGL_MAT4 perspective;
				POGLMat4Perspective(45.0f, 1.2f, 0.1f, 100.0f, &perspective);

				POGL_MAT4 lookAt;
				POGLMat4LookAt(POGL_VECTOR3(0.0f, 20.0f, 20.0f), POGL_VECTOR3(0.f, 0.f, 0.f), POGL_VECTOR3(0.0f, 1.0f, 0.0f), &lookAt);

				//
				// Set static uniforms for this program
				//

				program->FindUniformByName("ProjectionMatrix")->SetMatrix(perspective);
				program->FindUniformByName("ViewMatrix")->SetMatrix(lookAt);

				//
				// Set default properties for this program
				//

				program->SetDepthTest(true);
				program->SetDepthFunc(POGLDepthFunc::LESS);

				//
				// Run while loop
				//

				std::mutex m;
				std::unique_lock<std::mutex> lock(m);
				while (running) {
					
					// 
					// Apply the program (note that the program is actually loaded, but not initialized, in this thread).
					//
					// This has to be done in inside the running render loop. 
					// Why is this important here? It's important because the commands will be cleared after the flush method is invoked
					// and we can't know for sure if some other program or buffer is bound somewhere else in the program (from this thread's point of view).
					// 
					// Any unneccessary state-changes are prevented when the apply command is executed on the immediately render context (if the program is already bound).
					//

					IPOGLRenderState* state = deferredContext->Apply(program);

					//
					// Set the active vertex- and index buffers
					//

					state->SetVertexBuffer(vertexBuffer);
					state->SetIndexBuffer(indexBuffer);

					state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

					//
					// Set the same model multiple times
					//

					IPOGLUniform* modelMatrixUniform = state->FindUniformByName("ModelMatrix");

					POGL_MAT4 modelMatrix;
					POGLMat4Translate(POGL_VECTOR3(-5.0f, 0.0f, 0.0f), &modelMatrix);
					POGLMat4Rotate(angle, modelMatrix, POGL_VECTOR3(0.0f, 1.0f, 0.0f), &modelMatrix);
					modelMatrixUniform->SetMatrix(modelMatrix);
					state->DrawIndexed();

					modelMatrix = POGL_MAT4();
					POGLMat4Translate(POGL_VECTOR3(5.0f, 0.0f, 0.0f), &modelMatrix);
					POGLMat4Rotate(angle, modelMatrix, POGL_VECTOR3(0.0f, 1.0f, 0.0f), &modelMatrix);
					modelMatrixUniform->SetMatrix(modelMatrix);
					state->DrawIndexed();

					modelMatrix = POGL_MAT4();
					POGLMat4Translate(POGL_VECTOR3(0.0f, 0.0f, 5.0f), &modelMatrix);
					POGLMat4Rotate(angle, modelMatrix, POGL_VECTOR3(0.0f, 1.0f, 0.0f), &modelMatrix);
					modelMatrixUniform->SetMatrix(modelMatrix);
					state->DrawIndexed();

					modelMatrix = POGL_MAT4();
					POGLMat4Translate(POGL_VECTOR3(0.0f, 0.0f, -5.0f), &modelMatrix);
					POGLMat4Rotate(angle, modelMatrix, POGL_VECTOR3(0.0f, 1.0f, 0.0f), &modelMatrix);
					modelMatrixUniform->SetMatrix(modelMatrix);
					state->DrawIndexed();

					state->Release();

					//
					// Flush the commands so that they can be executed in the main thread
					//

					deferredContext->Flush();

					//
					// Wait until the commands have been executed. Otherwise the deferred context will start allocating memory faster then it will be released.
					// The reason for this happening is because this thread will be executed much much faster than the main thread because of all
					// OpenGL commands. We allocate variables faster than we can release them.
					//

					if (running)
						condition.wait(lock);
				}

				vertexBuffer->Release();
				indexBuffer->Release();
				program->Release();
			}
			catch (POGLException e) {
				POGLAlert(e);
			}
		});
		
		POGL_FLOAT angleFlt = 0.0f;
		const POGL_FLOAT ROTATION_SPEED = 90.0f;

		while (POGLProcessEvents()) {
			angleFlt += POGLGetTimeSinceLastTick() * ROTATION_SPEED;
			if (angleFlt > 360.0f)
				angleFlt = 360.0f - angle;
			angle = angleFlt;

			//
			// Execute the deferred context commands and notify the thread so that it generates the commands once again
			//
			
			deferredContext->ExecuteCommands(context);
			condition.notify_one();

			// Swap buffers
			device->EndFrame();
		}

		//
		// Stop running and wait for the thread to stop running
		//

		running = false;
		condition.notify_one();
		t.join();

		deferredContext->Release();
		context->Release();
		device->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	
	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
