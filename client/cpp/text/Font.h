#pragma once

#include "../renderer/Primitive.h"
#include <string>
#include <memory>
#include <filesystem>


class Font : public MeshCollection
{
public:
	Font(const std::filesystem::path& filename);
	~Font();

	void* hb_font;
	void* hb_face;
	float line_height = 0;

	struct ShapedGlyph
	{
		uint32_t index;
		float2 pos;
		ShapedGlyph(uint32_t index, float2 pos) : index(index), pos(pos) {}
	};

	std::vector<ShapedGlyph> shape(const std::string& text) const;
};
