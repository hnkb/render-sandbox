
#include "Buffer.h"

constexpr bool drawWireframe = false;


DeviceBuffer::DeviceBuffer(
	const GLsizeiptr vertexCount,
	const float2* posData,
	const GLsizeiptr indexCount,
	const uint32_t* indexData)
	: vertexCount(vertexCount),
	  indexCount(indexCount)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, buffers);

	glBindVertexArray(vao);

	if (posData)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float2), posData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (indexCount && indexData)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indexCount * sizeof(uint32_t),
			indexData,
			GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

DeviceBuffer::~DeviceBuffer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, buffers);
}

void DeviceBuffer::draw(int first, int count) const
{
	constexpr GLenum mode = drawWireframe ? GL_LINE_LOOP : GL_TRIANGLES;

	glBindVertexArray(vao);
	if (indexCount)
		glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(first * sizeof(uint32_t)));
	else
		glDrawArrays(mode, first, count);
}
