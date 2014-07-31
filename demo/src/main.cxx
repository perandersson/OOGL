#include <gl/pogl.hxx>
#include <thread>

static const POGL_CHAR SIMPLE_EFFECT[] = { R"(
<Effect>
	<VertexShader>
	<![CDATA[
		uniform mat4 ProjectionMatrix;
		uniform mat4 ViewMatrix;
		uniform mat4 ModelMatrix;

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec4 color;

		out vec4 vs_Color;

		void main()
		{
			vs_Color = color;
			gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
		}
	]]>
	</VertexShader>
	<FragmentShader>
	<![CDATA[
		in vec4 vs_Color;

		void main()
		{
			gl_FragColor = vs_Color;
		}
	]]>
	</FragmentShader>
</Effect>
)"};

int main()
{
	// Create a POGL device
	POGL_DEVICE_INFO deviceInfo;
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = 0;
#endif
	deviceInfo.window = nullptr;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	// Create a device context for main thread
	IPOGLDeviceContext* context = device->GetContext();

	// Load effect
	IPOGLEffect* simpleEffect = context->CreateEffectFromMemory(SIMPLE_EFFECT, sizeof(SIMPLE_EFFECT));

	// Create vertex buffer
	IPOGLBuffer* vertexBuffer = context->CreateBuffer();

	std::thread t([device, simpleEffect, vertexBuffer] {
		// Retrieves a device context for this thread
		IPOGLDeviceContext* context = device->GetContext();

		POGL_MAT4F projectionMatrix;
		POGL_MAT4F viewMatrix;
		POGL_MAT4F modelMatrix;
		
		// Apply the simple effect and draw some geometry to to the screen
		IPOGLRenderState* state = context->Apply(simpleEffect);
		state->Clear(POGLClearType::COLOR_AND_DEPTH);

		// Set uniforms
		state->FindUniformByName("ProjectionMatrix")->SetMatrix(&projectionMatrix);
		state->FindUniformByName("ViewMatrix")->SetMatrix(&viewMatrix);
		state->FindUniformByName("ModelMatrix")->SetMatrix(&modelMatrix);

		// Draw the vertex buffer
		state->Draw(vertexBuffer);

		// End the frame
		state->EndFrame();

		// Release the context resource when you are done with it
		context->Release();
	});
	t.join();

	// Release the simple effect resources
	simpleEffect->Release();

	// Release the context resource when you are done with it
	context->Release();

	// Release the device when you are done with it
	device->Release();

	// Quit application
	return 0;
}
