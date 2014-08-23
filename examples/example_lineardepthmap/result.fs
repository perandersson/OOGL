#version 330

// Uniform texture
uniform sampler2D LinearDepthTexture;

in vec2 vs_TexCoord;

layout(location = 0) out vec4 color;

void main()
{
	vec4 geometry = texture(LinearDepthTexture, vs_TexCoord);
	color = vec4(geometry.a, geometry.a, geometry.a, 1.0);
}
