
#include "Camera.h"
#include "../device/Shader.h"
#include "../graphics/Shapes.h"
#include "../text/Font.h"
#include "../utils/File.h"
#include <emscripten/emscripten.h>

using namespace std;

bool shouldRender = true;

Camera camera;
vector<Primitive> primitives;

unique_ptr<ShaderProgram> program;
MeshCollection shapes;
vector<unique_ptr<Font>> fonts;


void addText(const Font& font, const string& text, float x, float y, uint32_t color = 0)
{
	const auto layout = font.shape(text);
	for (const auto& glyph : layout)
		primitives.emplace_back(
			font,
			(int)glyph.index,
			&glyph == &layout[0] ? color : 0,
			glyph.pos + float2(x, y));
}

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


		fonts.emplace_back(new Font("fonts/OpenSans-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/Overpass-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/Overpass-Bold.ttf"));
		fonts.emplace_back(new Font("fonts/IBMPlexSansArabic-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/MPLUS1p-Regular.ttf"));

		addText(*fonts[4], "明日は晴れるといいですね。\n桜の花も咲くでしょう。\n春の訪れを感じます。", -3.f, -3.5f, 0x800080ffu);
		addText(*fonts[3], "الا یا ایها الساقی ادر کأسا و ناولها\nکه عشق آسان نمود اول ولی افتاد مشکل‌ها", -3.f, 1.5f, 0x800080ffu);
		addText(*fonts[2], "webgl playground;", -3.f, 4.2f, 0xffu);
		addText(*fonts[1], "The quick brown fox jumps over the lazy dog.", -3.f, 5.3f, 0xffu);
		addText(*fonts[3], "الجمال في الحياة يكمن في القدرة على رؤية الجمال\nفي الأشياء الصغيرة والتقدير للحظات البسيطة.", -3.f, 7.f, 0xdd5544ffu);

		// float start = 9.f;
		// for (auto& font : fonts)
		// 	addText(*font, "Z̶̢̛͖̺͖͎̙͓̝̝͋ͬ̒ͬ̇ͦͧͫ͑̒̋̅ͣ̒͋̃̏͟͝͝͠ͅa̴̷̢̭͈̝͕͓̭̬̦̍̾͊ͧ̊̇͒͂̌ͯ͐̀̔̕̕͢͠l̷̴̵̢̡̢̛͈̯̙̫̱̫̮͙̹̫͚͙̦̳̭̦̄̑͗̋͆̋́͌͐̓̔̇̐͆ͧ̂̌͘͟͠͞͝͠ͅg̛͛̃͆̾ͫ͟ơ̵̴̵̶̢̩͇͚̳̤ͥ͛͆̀ͣͫ̌ͥ̾ͮ͊ͤͩ̍̚ L̼̲͙̇̄ͦ̈́ͪ̇́̐̇͜e̵͙̺̤̺͍͊̑̐̐̓̎͡v̜e͚̜̐̂ͮ̋̽ļ̧̭̞͕̰̯͍̻͈ͪ̅̑͢ T̴̢̲͈́̈́ͯw̶̵̫̩̥̩̦̙̣̼̬ͭ͆ͯ͐͂͐̈̒̎̕̕̚͝e̸̴̛̻͍̤̗̙̜͙̗̰̹͋̈́̏̓̋́̀̎̇̌̑̈̏ͮ̚̕͢l̸̷͓͎̗ͭ́̇ͥͭ̔̚͜͠ͅv̰̬̮̖̞̪̎̒͋͗ͯ́̑͌͜͝ẽ̡̞͈͎̹ͩ͐̽ͬ̾͊ͭ͟", -3.f, start += 1.f, 0xffu);

		const auto fileContent = File::readAll<char>("text.txt");
		const auto text = string(fileContent.begin(), fileContent.end());
		addText(*fonts[0], text, 20.f, -3.6f, 0xffu);
		addText(*fonts[0], text, 70.f, -3.6f, 0xdd5544ffu);
		addText(*fonts[0], text, 120.f, -3.6f, 0x800080ffu);
		addText(*fonts[0], text, 170.f, -3.6f, 0x0000a0ffu);

		// for (int row = 0; row < 80; row++)
		// 	for (int col = 0; col < 90; col++)
		// 		addText(*fonts[0], "Miro", col * 3.f, row * 2.f, row == 0 && col == 0 ? 0xffu : 0);
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
