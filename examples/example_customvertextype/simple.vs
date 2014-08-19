#version 330

// The "position" value is located on location = "0", defined in CustomVertexLayout
layout(location = 0) in vec3 position;

// The "value" value is located on location = "2", defined in CustomVertexLayout
layout(location = 2) in float value;

// Send the value to the fragment shader
out float vs_Value;

void main()
{
	vs_Value = value;
	gl_Position = vec4(position, 1.0);
}
