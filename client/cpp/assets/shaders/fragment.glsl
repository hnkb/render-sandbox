#version 300 es

precision lowp float;
uniform uint color;
out vec4 outputColor;

vec4 unpackColor()
{
	return vec4(color >> 24, (color >> 16) & uint(0xff), (color >> 8) & uint(0xff), color & uint(0xff)) / 255.f;
}

void main()
{
	outputColor = unpackColor();
}
