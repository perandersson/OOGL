#version 330

// Data sent to the fragment shader from the vertex
// shader
in vec4 vs_Color;

layout(location = 0) out vec4 color;

void main()
{
	color = vs_Color;
}
