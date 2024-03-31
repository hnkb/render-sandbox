#pragma once

#include <GLES3/gl3.h>
#include "../utils/Math.h"


class DeviceBuffer
{
public:
	DeviceBuffer(
		const GLsizeiptr vertexCount,
		const float2* posData,
		const GLsizeiptr indexCount = 0,
		const uint32_t* indexData = nullptr);
	~DeviceBuffer();

	void draw(int first, int count) const;
	void draw() const { return draw(0, indexCount ? indexCount : vertexCount); }

private:
	GLuint vao;
	GLuint buffers[2];

	GLsizeiptr vertexCount;
	GLsizeiptr indexCount;
};
