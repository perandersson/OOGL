#version 330

// Output color for the render target location "0" defined when creating the framebuffer
layout(location = 0) out vec4 texture0;

// Output color for the render target location "1" defined when creating the framebuffer
layout(location = 1) out vec4 texture1;

void main()
{
	// Draw red pixels to framebuffer location "0"
	texture0 = vec4(1, 0, 0, 1);

	// Draw blue pixels to framebuffer location "1"
	texture1 = vec4(0, 0, 1, 1);
}
