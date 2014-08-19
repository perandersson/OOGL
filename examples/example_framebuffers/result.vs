#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out vec2 vs_TexCoord;

void main()
{
	vs_TexCoord = texCoord;
	gl_Position = vec4(position, 1.0);
}