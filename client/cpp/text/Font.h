#pragma once

#include "../utils/Math.h"
#include <string>
#include <memory>
#include <emscripten/bind.h>


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

	emscripten::val shape(const std::string& text) const;

	auto getVertexData() const { return toTypedMemoryView(vertices); }
	auto getIndexData() const { return toTypedMemoryView(indices); }
	auto& getMeshesArray() const { return meshesArray; }

private:
	void* hb_font;
	void* hb_face;

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	emscripten::val meshesArray;
};
