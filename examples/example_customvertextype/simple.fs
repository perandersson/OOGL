#version 330

in float vs_Value;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(vs_Value, vs_Value, vs_Value, 1);
}