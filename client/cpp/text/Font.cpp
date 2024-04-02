#include "Font.h"
#include "../utils/File.h"
#include <hb.h>

using namespace std;


Font::Font(const filesystem::path& filename)
{
	auto file = filename;

	file.replace_extension(".vert");
	auto vertices = File::readAll<float2>(file);

	file.replace_extension(".idx");
	auto indices = File::readAll<uint32_t>(file);

	file.replace_extension(".mesh");
	meshes = File::readAll<Mesh>(file);

	buffer.reset(
		new DeviceBuffer(vertices.size(), vertices.data(), indices.size(), indices.data()));

	printf(
		"Font '%s' loaded into %.2f MB buffer with %zu glyphs, %zu vertices, %zu indices.\n",
		filename.stem().c_str(),
		(vertices.size() * sizeof(vertices[0]) + indices.size() * sizeof(indices[0])) / 1024.
			/ 1024.,
		meshes.size(),
		vertices.size(),
		indices.size());

	auto blob = hb_blob_create_from_file(filename.u8string().c_str());
	hb_face = hb_face_create(blob, 0);
	hb_font = hb_font_create((hb_face_t*)hb_face);
	hb_blob_destroy(blob);

	hb_font_extents_t extents;
	if (!hb_font_get_h_extents((hb_font_t*)hb_font, &extents))
		fprintf(stderr, "Failed to get font extents.\n");
	line_height = (extents.ascender - extents.descender + extents.line_gap)
				  / hb_face_get_upem((hb_face_t*)hb_face);
}

Font::~Font()
{
	hb_font_destroy((hb_font_t*)hb_font);
	hb_face_destroy((hb_face_t*)hb_face);
}
