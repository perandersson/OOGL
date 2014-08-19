#version 330

// Uniform containing the projection matrix set by the
// main application
uniform mat4 ProjectionMatrix;

// Uniform containing the view matrix
uniform mat4 ViewMatrix;

// Uniform containing the model matrix (rotation in this case)
uniform mat4 ModelMatrix;

// Vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

// Data sent to the fragment shader from the vertex
// shader
out vec4 vs_Color;

void main()
{
	vs_Color = color;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(position, 1.0);
}
