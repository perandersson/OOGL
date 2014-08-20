#include <gl/pogl.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <cmath>
#include "POGLExampleWindow.h"

static const POGL_UINT32 CIRCLE_PTS = 365;

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOCHAR("Example: Threaded Vertex Buffer Mapping"));

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

		IPOGLShader* vertexShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.vs"), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromFile(POGL_TOCHAR("simple.fs"), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);
		vertexShader->Release();
		fragmentShader->Release();


		//
		// Create vertex buffer for circle geometry
		//

		std::vector<POGL_POSITION_VERTEX> vertices;
		vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3(0, 0, 0)));
		for (POGL_UINT32 i = 0; i < CIRCLE_PTS; ++i) {
			const POGL_FLOAT x = 0.1f * cosf(i * 0.0174532925f);
			const POGL_FLOAT y = 0.1f * sinf(i * 0.0174532925f);
			vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3(x, y, 0)));
		}
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(&vertices[0], vertices.size() * sizeof(POGL_POSITION_VERTEX), POGLPrimitiveType::TRIANGLE_FAN, POGLBufferUsage::DYNAMIC);

		std::atomic<bool> running(true);
		std::atomic<POGL_FLOAT> totalTime(0.0f);
		
		//
		// Create two threads. Each thread update different parts of the same VertexBuffer instance.
		//
		// The first thread is responsible for increasing and decreasing the radius of the top-part of the circle over time.
		// The second thread is responsible for increasing and decreasing the radius of the bottom-part of the circle over time.
		//

		IPOGLDeferredRenderContext* t1context = device->CreateDeferredRenderContext();
		std::condition_variable t1cond;
		std::thread t1([t1context, vertexBuffer, &running, &totalTime, &t1cond] {
			try {
				std::mutex m;
				std::unique_lock<std::mutex> lock(m);

				while (running) {
					const POGL_FLOAT totalTimeFlt = totalTime.load();

					const POGL_UINT32 offset = 1;
					const POGL_UINT32 length = CIRCLE_PTS / 2;

					//
					// Map the internal memory
					//

					POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)t1context->Map(vertexBuffer, offset * sizeof(POGL_POSITION_VERTEX),
						length * sizeof(POGL_POSITION_VERTEX), POGLResourceMapType::WRITE);
					POGL_POSITION_VERTEX* ptr = vertices;

					//
					// Update the buffer data
					//

					const POGL_FLOAT radius = sinf((totalTimeFlt * 15.0f) * 0.0174532925f) * 0.5f;
					for (POGL_UINT32 i = 0; i < length; ++i) {
						ptr->position.x = radius * cosf(i * 0.0174532925f);
						ptr->position.y = radius * sinf(i * 0.0174532925f);
						ptr->position.z = 0;
						ptr++;
					}

					//
					// Unmap the vertex buffer
					//

					t1context->Unmap(vertexBuffer);

					//
					// Flush the commands 
					//

					t1context->Flush();

					//
					// Wait until the commands have been executed. Otherwise the deferred context will start allocating memory faster then it will be released.
					// The reason for this happening is because this thread will be executed much much faster than the main thread because of all
					// OpenGL commands. We allocate variables faster than we can release them.
					//

					if (running)
						t1cond.wait(lock);
				}
			}
			catch (POGLException e) {
				POGLAlert(e);
			} 
		});

		IPOGLDeferredRenderContext* t2context = device->CreateDeferredRenderContext();
		std::condition_variable t2cond;
		std::thread t2([t2context, vertexBuffer, &running, &totalTime, &t2cond] {
			try {
				std::mutex m;
				std::unique_lock<std::mutex> lock(m);

				while (running) {
					const POGL_FLOAT totalTimeFlt = totalTime.load();

					const POGL_UINT32 offset = 1 + CIRCLE_PTS / 2;
					const POGL_UINT32 length = (1 + CIRCLE_PTS) - offset;

					//
					// Map the internal memory
					//

					POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)t2context->Map(vertexBuffer, offset * sizeof(POGL_POSITION_VERTEX),
						length * sizeof(POGL_POSITION_VERTEX), POGLResourceMapType::WRITE);
					POGL_POSITION_VERTEX* ptr = vertices;

					//
					// Update the buffer data
					//

					const POGL_FLOAT radius = sinf((totalTimeFlt * 15.0f) * 0.0174532925f) * 0.5f;
					for (POGL_UINT32 i = 0; i < length; ++i) {
						ptr->position.x = radius * cosf((i + length) * 0.0174532925f);
						ptr->position.y = radius * sinf((i + length) * 0.0174532925f);
						ptr->position.z = 0;
						ptr++;
					}

					//
					// Unmap the vertex buffer
					//

					t2context->Unmap(vertexBuffer);

					//
					// Flush the commands 
					//

					t2context->Flush();
					
					//
					// Wait until the commands have been executed. Otherwise the deferred context will start allocating memory faster then it will be released.
					// The reason for this happening is because this thread will be executed much much faster than the main thread because of all
					// OpenGL commands. We allocate variables faster than we can release them.
					//

					if (running)
						t2cond.wait(lock);
				}
			}
			catch (POGLException e) {
				POGLAlert(e);
			}
		});

		POGL_FLOAT totalTimeFlt = 0.0f;
		while (POGLProcessEvents()) {
			totalTimeFlt += POGLGetTimeSinceLastTick();
			totalTime.store(totalTimeFlt);

			//
			// Execute commands generated by the t1context and t2context
			//

			t1context->ExecuteCommands(context);
			t1cond.notify_one();
			t2context->ExecuteCommands(context);
			t2cond.notify_one();

			//
			// Draw the vertex buffer using the "simple effect"
			//

			IPOGLRenderState* state = context->Apply(program);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);
			state->Draw(vertexBuffer);
			state->Release();

			//
			// End the current frame
			//

			device->EndFrame();
		}

		running = false;
		t1cond.notify_one();
		t1.join();
		t2cond.notify_one();
		t2.join();

		// Release resources
		vertexBuffer->Release();
		program->Release();
		t1context->Release();
		t2context->Release();
		context->Release();

		// Release the device
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
