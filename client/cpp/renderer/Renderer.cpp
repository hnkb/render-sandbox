
#include "Camera.h"
#include "../device/Shader.h"
#include "../graphics/Shapes.h"
#include <emscripten/emscripten.h>

using namespace std;

bool shouldRender = true;

Camera camera;
vector<Primitive> primitives;

unique_ptr<ShaderProgram> program;
MeshCollection shapes;


extern "C" int renderFrame(double time, void* userData)
{
	if (!shouldRender)
		return true;

	if (!program)
	{
		program.reset(new ShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl"));
		createPolygonShapes(shapes);

		for (int i = 0; i < shapes.meshes.size(); i++)
			primitives.emplace_back(shapes, i, 0xdd554480, i * 1.1f - 3.f, -.25f);
	}

	glClearColor(.98f, .98f, .98f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the program and camera which remain constant throughout the frame
	{
		glUseProgram(*program);
		const auto scale = camera.scaleWithAR();
		const auto& offset = camera.view.offset;
		glUniform4f(program->uniforms.camera, offset.x, offset.y, scale.x, scale.y);
	}

	for (const auto& p : primitives)
	{
		glUniform2fv(program->uniforms.offset, 1, (float*)&p.offset);
		if (p.color)
			glUniform1ui(program->uniforms.color, p.color);
		p.collection.buffer->draw(
			p.collection.meshes[p.meshIndex].startIndex,
			p.collection.meshes[p.meshIndex].indexCount);
	}

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
