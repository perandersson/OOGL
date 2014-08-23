#version 330

// The far clip distance.
uniform float FarClipDistance;

// Draw the linear depth value to this texture
layout(location = 0) out vec4 linearDepthTexture;

// View-space position for the current pixel
in vec4 vs_ViewSpacePosition;

void main()
{
	// Draw the linear depth to one of the components (in this case the alpha component)
	linearDepthTexture = vec4(0, 0, 0, -vs_ViewSpacePosition.z / FarClipDistance);
}
