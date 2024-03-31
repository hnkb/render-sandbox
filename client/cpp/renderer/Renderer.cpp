
#include "Camera.h"
#include "../device/Buffer.h"
#include "../device/Shader.h"
#include <emscripten/emscripten.h>

using namespace std;

bool shouldRender = true;
Camera camera;

unique_ptr<ShaderProgram> program;
unique_ptr<DeviceBuffer> buffer;

const float2 vertexPositions[] = {
	{ 0.75f, 0.75f },
	{ 0.75f, -0.75f },
	{ -0.75f, -0.75f },
};


extern "C" int renderFrame(double time, void* userData)
{
	if (!shouldRender)
		return true;

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
		const auto scale = camera.scaleWithAR();
		const auto& offset = camera.view.offset;
		glUniform4f(program->uniforms.camera, offset.x, offset.y, scale.x, scale.y);
	}

	if (buffer)
		buffer->draw();

	glBindVertexArray(0);
	glUseProgram(0);

	shouldRender = false;
	return true;
}


extern "C" EMSCRIPTEN_KEEPALIVE void resizeCanvas(int width, int height)
{
	camera.setViewport({ width, height });
	glViewport(0, 0, width, height);
	shouldRender = true;
}

extern "C" EMSCRIPTEN_KEEPALIVE void cameraMove(float x, float y)
{
	camera.view.offset -= float2(x, y) * camera.pixelSize;
	shouldRender = true;
}

extern "C" EMSCRIPTEN_KEEPALIVE void cameraZoom(float x, float y, float delta)
{
	// I want the cursor position on screen to point to the same world coordinates as
	// before the scaling. So, I subtract the difference between the new world position
	// and the original one from the view offset.
	auto before = camera.pixelToWorld({ x, y });
	camera.view.scale *= powf(.995f, delta);
	auto after = camera.pixelToWorld({ x, y });
	camera.view.offset -= (before - after) * camera.scaleWithAR();
	shouldRender = true;
}
