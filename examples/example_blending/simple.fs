#version 330

in vec4 vs_Color;

layout(location = 0) out vec4 color;

void main()
{
	color = vs_Color;
}
