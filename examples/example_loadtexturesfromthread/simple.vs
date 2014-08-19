#version 330

uniform vec2 Translate;
uniform vec2 Scale;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out vec2 vs_TexCoord;

void main()
{
	vs_TexCoord = texCoord;
	vec3 newPos = (position + vec3(Translate, 1.0)) * vec3(Scale, 1.0);
	gl_Position = vec4(newPos, 1.0);
}