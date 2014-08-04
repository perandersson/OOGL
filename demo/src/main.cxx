#include <gl/pogl.hxx>
#include <thread>

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
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
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	in vec4 vs_Color;

	void main()
	{
		gl_FragColor = vs_Color;
	}
)" };

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
	IPOGLDeviceContext* context = device->GetDeviceContext();

	// Create an effect based on the supplied vertex- and fragment shader
	IPOGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderProgramType::VERTEX_SHADER);
	IPOGLShaderProgram* fragmentShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderProgramType::FRAGMENT_SHADER);
	IPOGLShaderProgram* programs[] = { vertexShader, fragmentShader };
	IPOGLEffect* simpleEffect = context->CreateEffectFromPrograms(programs, 2);
	vertexShader->Release();
	fragmentShader->Release();

	// Create vertex buffer
	POGL_POSITION_COLOR_VERTEX vertices[] = {
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), POGL_COLOR4F(1.0f, 0.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(-0.5f, 0.5f, 0.0f), POGL_COLOR4F(1.0f, 1.0f, 0.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.5f, 0.5f, 0.0f), POGL_COLOR4F(1.0f, 1.0f, 1.0f, 1.0f)),
		POGL_POSITION_COLOR_VERTEX(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), POGL_COLOR4F(0.0f, 0.0f, 1.0f, 1.0f))
	};
	IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(vertices, sizeof(vertices), POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

	// Create index buffer
	POGL_UINT32 indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	IPOGLIndexBuffer* indexBuffer = context->CreateIndexBuffer(indices, sizeof(indices), POGLVertexType::UNSIGNED_INT, POGLBufferUsage::STATIC);

	// Increase reference counter
	simpleEffect->AddRef();
	vertexBuffer->AddRef();
	indexBuffer->AddRef();

	// New thread
	std::thread t([device, simpleEffect, vertexBuffer, indexBuffer] {
		// Retrieves a device context for this thread
		IPOGLDeviceContext* context = device->GetDeviceContext();

		// Apply the simple effect and draw some geometry to to the screen
		IPOGLRenderState* state = context->Apply(simpleEffect);
		state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

		POGL_MAT4F projectionMatrix;
		POGL_MAT4F viewMatrix;
		POGL_MAT4F modelMatrix;

		// Set uniforms
		state->FindUniformByName("ProjectionMatrix")->SetMatrix(projectionMatrix);
		state->FindUniformByName("ViewMatrix")->SetMatrix(viewMatrix);
		state->FindUniformByName("ModelMatrix")->SetMatrix(modelMatrix);
		
		// Get uniform usable to set texture
		IPOGLUniform* texture = state->FindUniformByName("Texture");
		texture->GetSamplerState()->SetMinFilter(POGLMinFilter::LINEAR);

		// Draw the vertex buffer
		state->Draw(vertexBuffer, indexBuffer);

		// End the frame
		state->EndFrame();

		// Release the context resource when you are done with it
		context->Release();

		// Release buffers
		indexBuffer->Release();
		vertexBuffer->Release();
		simpleEffect->Release();
	});
	t.join();

	// Release the index- and vertex buffers
	indexBuffer->Release();
	vertexBuffer->Release();

	// Release the simple effect resources
	simpleEffect->Release();

	// Release the context resource when you are done with it
	context->Release();

	// Release the device when you are done with it
	device->Release();

	// Quit application
	return 0;
}
