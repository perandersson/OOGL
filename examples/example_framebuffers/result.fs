#version 330

// Uniform texture
uniform sampler2D Texture0;

// Uniform texture
uniform sampler2D Texture1;

in vec2 vs_TexCoord;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(Texture0, vs_TexCoord) + texture(Texture1, vs_TexCoord);
}