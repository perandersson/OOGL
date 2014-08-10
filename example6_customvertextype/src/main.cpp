#include <gl/pogl.h>
#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include "POGLExampleWindow.h"

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
	#version 330

	// The "position" value is located on location = "0", defined in CustomVertexLayout
	layout(location = 0) in vec3 position;

	// The "value" value is located on location = "2", defined in CustomVertexLayout
	layout(location = 2) in float value;

	// Send the value to the fragment shader
	out float vs_Value;

	void main()
	{
		vs_Value = value;
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	#version 330

	in float vs_Value;

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vec4(vs_Value, vs_Value, vs_Value, 1);
	}
)" };

//
// Custom vertex structure
// 
// Each custom vertex structure must have a matching POGL_VERTEX_LAYOUT object. The order of the attributes inside the custom
// vertex structure must match the attribute location order in the POGL_VERTEX_LAYOUT object.
//

struct CustomVertex
{
	POGL_VECTOR3F position;
	POGL_FLOAT value;

	CustomVertex() {}
	CustomVertex(const POGL_VECTOR3F& p, POGL_FLOAT v) { position = p; value = v; }
	CustomVertex(const CustomVertex& v) { position = v.position; value = v.value; }
	~CustomVertex() {}
	inline CustomVertex& operator=(const CustomVertex& rhs) { position = rhs.position; value = rhs.value;  return *this; }
};

//
// Layout object describing the data inside the CustomVertex.
//

static const POGL_VERTEX_LAYOUT CustomVertexLayout = {

	//
	// Vertex attribute locations. 
	//
	// The index of POGL_VERTEX_LAYOUT_FIELD fields array will become the attribute location.
	// 
	// The attributes can be put in any location that you want, as long as they are in the same order as
	// they are in the custom vertex structure. The POGL_VECTOR3F "position" is located before the POGL_FLOAT "value" in this 
	// example, thus the "position" must be have a lower attribute location than the "value". 
	//

	{ 
		// 
		// Attribute location = 0
		//
		// First value tells us how large the attribute is. This is almost always the size of the attribute.
		// 
		// The second value tells us what type of data is contained in the attribute (floats, integers etc.)
		//
		// The third value indicates if we want OpenGL to normalize the value on input. This is almost always "false".
		//

		{ sizeof(POGL_VECTOR3F), POGLVertexType::FLOAT, false },

		//
		// Do not use attribute location = 1
		// 
		// You are free to put the attributes in any location that you want.
		//

		{ 0 },

		//
		// Attribute location = 2
		//

		{ sizeof(POGL_FLOAT), POGLVertexType::FLOAT, false },

		//
		// No more attributes
		//

		0
	},
	
	//
	// The offset between each vertex. This is almost always the size of the vertex
	//

	sizeof(CustomVertex)
};


static const POGL_UINT32 CIRCLE_PTS = 365;

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 6: Custom Vertex Type"));

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

		IPOGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderProgramType::VERTEX_SHADER);
		IPOGLShaderProgram* fragmentShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderProgramType::FRAGMENT_SHADER);
		IPOGLShaderProgram* programs[] = { vertexShader, fragmentShader };
		IPOGLEffect* simpleEffect = context->CreateEffectFromPrograms(programs, 2);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create data for a triangle.
		//

		const CustomVertex VERTICES[] = {
			CustomVertex(POGL_VECTOR3F(-0.5f, -0.5f, 0.0f), 1.0f),
			CustomVertex(POGL_VECTOR3F(0.0f, 0.5f, 0.0f), 0.0f),
			CustomVertex(POGL_VECTOR3F(0.5f, -0.5f, 0.0f), 1.0f)
		};

		//
		// Create a vertex buffer resource based using the custom vertex structure
		//

		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(VERTICES, sizeof(VERTICES), &CustomVertexLayout, POGLPrimitiveType::TRIANGLE, POGLBufferUsage::STATIC);

		while (POGLProcessEvents()) {
			IPOGLRenderState* state = context->Apply(simpleEffect);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);

			//
			// Draw the vertex buffer in the same way as normal
			//

			state->Draw(vertexBuffer);

			state->EndFrame();
			device->SwapBuffers();
		}

		//
		// Release the vertex resources in the same way as normal
		//

		vertexBuffer->Release();
		simpleEffect->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	device->Release();

	POGLDestroyExampleWindow(windowHandle);
	return 0;
}
