#pragma once

#include "../utils/Math.h"
#include <string>
#include <memory>
#include <emscripten/bind.h>


EMSCRIPTEN_DECLARE_VAL_TYPE(Float32Array);
EMSCRIPTEN_DECLARE_VAL_TYPE(Uint32Array);
EMSCRIPTEN_DECLARE_VAL_TYPE(MeshArray);
EMSCRIPTEN_DECLARE_VAL_TYPE(ShapedGlyphArray);

struct Mesh
{
	int startIndex;
	int indexCount;
};

template <typename Type>
inline auto toTypedMemoryView(const std::vector<Type>& vec)
{
	return emscripten::val(emscripten::typed_memory_view(vec.size(), vec.data()));
}

class Font
{
public:
	Font(const std::string& name);
	~Font();

	struct ShapedGlyph
	{
		uint32_t index;
		float2 pos;
	};

	ShapedGlyphArray shape(const std::string& text) const;

	auto getVertexData() const { return Float32Array(toTypedMemoryView(vertices)); }
	auto getIndexData() const { return Uint32Array(toTypedMemoryView(indices)); }
	MeshArray getMeshesArray() const;

private:
	void* hb_font;
	void* hb_face;

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::vector<Mesh> meshes;
};
