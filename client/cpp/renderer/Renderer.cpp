
#include "Camera.h"
#include "../device/Shader.h"
#include "../graphics/Shapes.h"
#include "../text/Font.h"
#include "../utils/File.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

using namespace std;

bool shouldRender = true;

Camera camera;
vector<Primitive> primitives;

unique_ptr<ShaderProgram> program;
MeshCollection shapes;
vector<unique_ptr<Font>> fonts;

float2 pos;

void addText(const Font& font, const string& text, float x, float y, uint32_t color = 0)
{
	pos = (&font == fonts[3].get()) ? float2(0, .835f) : float2(0, .887565f);

	const auto layout = font.shape(text);
	for (const auto& glyph : layout)
		primitives.emplace_back(
			font,
			(int)glyph.index,
			&glyph == &layout[0] ? color : 0,//(uint32_t)(rand()|0xff),
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

		// for (int i = 0; i < shapes.meshes.size(); i++)
		// 	primitives.emplace_back(shapes, i, 0xdd554480, i * 1.1f - 3.f, -.25f);


		fonts.emplace_back(new Font("fonts/OpenSans-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/Overpass-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/Overpass-Bold.ttf"));
		fonts.emplace_back(new Font("fonts/IBMPlexSansArabic-Regular.ttf"));
		fonts.emplace_back(new Font("fonts/MPLUS1p-Regular.ttf"));

		// addText(*fonts[0], "Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", 0, 0, 0xdd5544ffu);
		addText(*fonts[4], "明日は晴れるといいですね。\n桜の花も咲くでしょう。\n春の訪れを感じます。", 0, 0, 0xdd5544ffu);
		// addText(*fonts[3], "الا یا ایها الساقی ادر کأسا و ناولها\nکه عشق آسان نمود اول ولی افتاد مشکل‌ها", 0, 0, 0xdd554480u);

// addText(*fonts[3], "الا یا hello ایها الساقی ادر کأسا و ناولها\nکه عشق آسان نمود اول ولی افتاد مشکل‌ها", 0, 0, 0xdd554480u);
		// addText(*fonts[0], "Z̶̢̛͖̺͖͎̙͓̝̝͋ͬ̒ͬ̇ͦͧͫ͑̒̋̅ͣ̒͋̃̏͟͝͝͠ͅa̴̷̢̭͈̝͕͓̭̬̦̍̾͊ͧ̊̇͒͂̌ͯ͐̀̔̕̕͢͠l̷̴̵̢̡̢̛͈̯̙̫̱̫̮͙̹̫͚͙̦̳̭̦̄̑͗̋͆̋́͌͐̓̔̇̐͆ͧ̂̌͘͟͠͞͝͠ͅg̛͛̃͆̾ͫ͟ơ̵̴̵̶̢̩͇͚̳̤ͥ͛͆̀ͣͫ̌ͥ̾ͮ͊ͤͩ̍̚ L̼̲͙̇̄ͦ̈́ͪ̇́̐̇͜e̵͙̺̤̺͍͊̑̐̐̓̎͡v̜e͚̜̐̂ͮ̋̽ļ̧̭̞͕̰̯͍̻͈ͪ̅̑͢ T̴̢̲͈́̈́ͯw̶̵̫̩̥̩̦̙̣̼̬ͭ͆ͯ͐͂͐̈̒̎̕̕̚͝e̸̴̛̻͍̤̗̙̜͙̗̰̹͋̈́̏̓̋́̀̎̇̌̑̈̏ͮ̚̕͢l̸̷͓͎̗ͭ́̇ͥͭ̔̚͜͠ͅv̰̬̮̖̞̪̎̒͋͗ͯ́̑͌͜͝ẽ̡̞͈͎̹ͩ͐̽ͬ̾͊ͭ͟", -3.f, 1.f, 0xffu);

		// const auto fileContent = File::readAll<char>("text.txt");
		// const auto text = string(fileContent.begin(), fileContent.end());
		// addText(*fonts[0], text, 0, 0, 0xdd5544ffu);

		camera.view.scale = .35f;
		camera.view.offset = { -.7f, .35f };
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

	{
		const auto dpr = (float)emscripten_get_device_pixel_ratio();
		// // const auto pos = float2(0, .887565f) * camera.view.scale;
		// const auto pos = float2(0, .835f) * camera.view.scale;

		const auto offset =
			((pos* camera.view.scale + camera.view.offset) / camera.pixelSize + camera.screenSize / 2) / dpr;

		const auto fontSize = camera.screenSize.y / 2.f / dpr;

		EM_ASM_(
			{
				const scale = $0;
				const translateX = $1;
				const translateY = $2;
				const fontSize = $3;

				const editor = document.getElementById('editor');
				editor.style.transform = 'translate(' + translateX + 'px,' + translateY + 'px) '
										 + 'scale(' + scale + ',' + scale + ')';
				editor.style.fontSize = fontSize + 'px';
				editor.style.lineHeight = fontSize + 'px';
			},
			camera.view.scale,
			offset.x,
			offset.y,
			fontSize);
	}

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
