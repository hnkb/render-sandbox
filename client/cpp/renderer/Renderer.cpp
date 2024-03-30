
#include "../device/Buffer.h"
#include "../device/Shader.h"

using namespace std;


unique_ptr<ShaderProgram> program;
unique_ptr<DeviceBuffer> buffer;

const float2 vertexPositions[] = {
	{ 0.75f, 0.75f },
	{ 0.75f, -0.75f },
	{ -0.75f, -0.75f },
};


extern "C" int renderFrame(double time, void* userData)
{
	if (!program)
	{
		program.reset(new ShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl"));
		buffer.reset(new DeviceBuffer(sizeof(vertexPositions) / sizeof(float2), vertexPositions));
	}

	glClearColor(.98f, .98f, .98f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set the program and camera which remain constant throughout the frame
	{
		glUseProgram(*program);
		glUniform4f(program->uniforms.camera, 0, 0, 1.f, 1.f);
	}

	if (buffer)
		buffer->draw();

	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}
