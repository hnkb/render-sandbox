#version 300 es

layout (location = 0) in vec2 position;
uniform vec4 camera;

vec4 applyCamera(vec2 position)
{
	return vec4((position + camera.xy) * camera.zw, 0, 1.f);
}

void main()
{
	gl_Position = applyCamera(position);
}
