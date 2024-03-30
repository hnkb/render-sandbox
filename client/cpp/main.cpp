#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <string>

extern "C" {
// void renderFrame()
int renderFrame(double time, void* userData)
{
	// emscripten_console_log(("Rendering frame " + std::to_string(time)).c_str());
	// emscripten_console_log("Rendering frame");
	glClearColor(0.866f, 0.333f, 0.266f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}
}

int main()
{
	EmscriptenWebGLContextAttributes attrs;
	emscripten_webgl_init_context_attributes(&attrs);
	attrs.majorVersion = 2;
	attrs.alpha = false;
	// attrs.stencil = true;
	attrs.failIfMajorPerformanceCaveat = true;

	auto ctx = emscripten_webgl_create_context("canvas.main", &attrs);
	if (ctx <= 0 || emscripten_webgl_make_context_current(ctx) != EMSCRIPTEN_RESULT_SUCCESS)
	{
		emscripten_console_error("Failed to create WebGL context in C++");
		return -1;
	}

	// emscripten_set_main_loop(renderFrame, 0, 1);
	emscripten_request_animation_frame_loop(renderFrame, nullptr);

	return 0;
}
