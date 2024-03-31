#pragma once

#include "../device/Buffer.h"
#include <vector>


struct Mesh
{
	int startIndex;
	int indexCount;
};

struct MeshCollection
{
	std::vector<Mesh> meshes;
	std::unique_ptr<DeviceBuffer> buffer;
};


struct Primitive
{
	Primitive(const MeshCollection& collection, int meshIndex, uint32_t color, const float2& offset)
		: offset(offset),
		  color(color),
		  collection(collection),
		  meshIndex(meshIndex)
	{}

	Primitive(const MeshCollection& collection, int meshIndex, uint32_t color, float x, float y)
		: Primitive(collection, meshIndex, color, float2(x, y))
	{}

	// TODO: proper world transform matrix
	float2 offset;
	// float scale;
	// float rotation;

	// color 0 means continue with previous values
	uint32_t color;

	const MeshCollection& collection;
	int meshIndex;
};
