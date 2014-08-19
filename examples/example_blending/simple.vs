#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 vs_Color;

void main()
{
	vs_Color = color;
	gl_Position = vec4(position, 1.0);
}