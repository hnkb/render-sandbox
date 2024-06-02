#include "Font.h"
#include "../utils/File.h"
#include <hb.h>

using namespace std;


Font::Font(const string& name)
{
	const auto filename = filesystem::path("fonts") / (name + ".ttf");
	auto file = filename;

	file.replace_extension(".vert");
	vertices = File::readAll<float>(file);

	file.replace_extension(".idx");
	indices = File::readAll<uint32_t>(file);

	file.replace_extension(".mesh");
	meshes = File::readAll<Mesh>(file);

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
}

Font::~Font()
{
	hb_font_destroy((hb_font_t*)hb_font);
	hb_face_destroy((hb_face_t*)hb_face);
}

MeshArray Font::getMeshesArray() const
{
	auto output = emscripten::val::array();
	for (auto& mesh : meshes)
		output.call<void>("push", emscripten::val(mesh));
	return MeshArray(output);
}


using namespace emscripten;

EMSCRIPTEN_BINDINGS(vector_text)
{
	value_array<float2>("float2")
		.element(&float2::x)
		.element(&float2::y)
		;

	value_object<Mesh>("Mesh")
		.field("startIndex", &Mesh::startIndex)
		.field("indexCount", &Mesh::indexCount)
		;

	value_object<Font::ShapedGlyph>("ShapedGlyph")
		.field("index", &Font::ShapedGlyph::index)
		.field("pos", &Font::ShapedGlyph::pos)
		;

	class_<Font>("Font")
		.constructor<const string&>()
		.function("shape", &Font::shape, return_value_policy::take_ownership())
		.property("vertexData", &Font::getVertexData)
		.property("indexData", &Font::getIndexData)
		.function("getMeshesArray", &Font::getMeshesArray, return_value_policy::take_ownership())
		;

	register_type<Float32Array>("Float32Array");
	register_type<Uint32Array>("Uint32Array");
	register_type<MeshArray>("Mesh[]");
	register_type<ShapedGlyphArray>("ShapedGlyph[] | null");
}
