#include "Font.h"
#include <hb.h>
#include <sstream>

using namespace std;

vector<Font::ShapedGlyph> Font::shape(const string& text) const
{
	auto face = (hb_face_t*)hb_face;
	auto font = (hb_font_t*)hb_font;

	// Set a scale compatible with design units from our mesh
	// We should find a nice nay to calculate point and pixel sizes
	const float scale = hb_face_get_upem(face);  // 64.f;

	auto buffer = hb_buffer_create();

	float2 cursor = { 0, 0 };

	vector<ShapedGlyph> output;
	output.reserve(text.size());

	istringstream stream(text);
	string line;
	while (getline(stream, line))
	{
		hb_buffer_reset(buffer);
		hb_buffer_add_utf8(buffer, line.c_str(), -1, 0, -1);
		hb_buffer_guess_segment_properties(buffer);

		hb_shape(font, buffer, NULL, 0);

		unsigned int glyph_count;
		auto glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
		auto glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);

		for (unsigned int i = 0; i < glyph_count; i++)
		{
			auto pos =
				cursor + float2 { glyph_pos[i].x_offset / scale, glyph_pos[i].y_offset / scale };
			cursor.x += glyph_pos[i].x_advance / scale;
			cursor.y += glyph_pos[i].y_advance / scale;
			output.emplace_back(glyph_info[i].codepoint, pos);
		}

		cursor.x = 0;
		cursor.y += line_height;
	}

	hb_buffer_destroy(buffer);

	return output;
}
