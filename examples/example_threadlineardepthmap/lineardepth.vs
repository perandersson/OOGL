#version 330

// Projection matrix
uniform mat4 ProjectionMatrix;

// View matrix
uniform mat4 ViewMatrix;

// Model matrix
uniform mat4 ModelMatrix;

// The input vertex position
layout(location = 0) in vec3 position;

// View-space position
out vec4 vs_ViewSpacePosition;

void main()
{
	vs_ViewSpacePosition = ViewMatrix * ModelMatrix * vec4(position, 1.0);
	gl_Position = ProjectionMatrix * vs_ViewSpacePosition;
}
