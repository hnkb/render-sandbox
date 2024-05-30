#include "Font.h"
#include <hb.h>

using namespace std;


ShapedGlyphArray Font::shape(const string& text) const
{
	auto face = (hb_face_t*)hb_face;
	auto font = (hb_font_t*)hb_font;

	const float scale = hb_face_get_upem(face);

	float2 cursor = { 0, 0 };
	auto output = emscripten::val::array();

	auto buffer = hb_buffer_create();
	hb_buffer_reset(buffer);
	hb_buffer_add_utf8(buffer, text.c_str(), -1, 0, -1);
	hb_buffer_guess_segment_properties(buffer);

	hb_shape(font, buffer, NULL, 0);

	unsigned int glyph_count;
	auto glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
	auto glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);

	for (unsigned int i = 0; i < glyph_count; i++)
	{
		ShapedGlyph glyph;
		glyph.index = glyph_info[i].codepoint;
		glyph.pos = cursor + float2 { glyph_pos[i].x_offset / scale, glyph_pos[i].y_offset / scale };
		output.call<void>("push", emscripten::val(glyph));

		cursor.x += glyph_pos[i].x_advance / scale;
		cursor.y += glyph_pos[i].y_advance / scale;
	}

	hb_buffer_destroy(buffer);

	return ShapedGlyphArray(output);
}
