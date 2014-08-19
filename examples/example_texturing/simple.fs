#version 330

uniform sampler2D Texture;

in vec2 vs_TexCoord;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(Texture, vs_TexCoord);
}