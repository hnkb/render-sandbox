#version 300 es

layout (location = 0) in vec2 position;
uniform vec4 camera;
uniform vec2 offset;

vec4 applyCamera(vec2 position)
{
	return vec4(position * camera.zw + camera.xy, 0, 1.f);
}

void main()
{
	gl_Position = applyCamera(position + offset);
}
