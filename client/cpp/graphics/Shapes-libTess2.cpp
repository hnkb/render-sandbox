
#include "Shapes.h"
#include <tesselator.h>
#include <cmath>

using namespace std;


vector<float2> createPolygon(const int vertexCount = 100)
{
	vector<float2> vertices;

	const float radius = 0.5f;
	const float angleStep = 2.f * 3.14159265359f / vertexCount;

	for (int i = 0; i < vertexCount; ++i)
	{
		const float angle = i * angleStep;
		const float x = radius * cos(angle);
		const float y = radius * sin(angle);

		vertices.emplace_back(x, y);
	}

	return vertices;
}

Mesh addPolygon(const vector<float2>& input, vector<float2>& vertices, vector<uint32_t>& indices)
{
	const auto startVertex = (int)vertices.size();
	const auto startIndex = (int)indices.size();

	TESStesselator* tess = tessNewTess(nullptr);
	if (!tess)
	{
		fprintf(stderr, "Error: Couldn't create tessellator");
		return { 0, 0 };
	}

	tessSetOption(tess, TESS_CONSTRAINED_DELAUNAY_TRIANGULATION, 1);

	tessAddContour(tess, 2, input.data(), sizeof(float2), input.size());

	if (!tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, 3, 2, nullptr))
	{
		fprintf(stderr, "Error: Couldn't tessellate");
		return { 0, 0 };
	}

	copy_n((float2*)tessGetVertices(tess), tessGetVertexCount(tess), back_inserter(vertices));

	auto elem = tessGetElements(tess);
	const auto numIndices = 3 * tessGetElementCount(tess);
	for (int i = 0; i < numIndices; i++)
		indices.push_back(elem[i] + startVertex);

	tessDeleteTess(tess);
	return { startIndex, (int)indices.size() - startIndex };
}

void createPolygonShapes2(MeshCollection& shapes)
{
	vector<float2> vertices;
	vector<uint32_t> indices;

	vertices.reserve(200);
	indices.reserve(600);

	// add a quad
	shapes.meshes.push_back(addPolygon(
		{ { +.5f, +.5f }, { +.5f, -.5f }, { -.5f, -.5f }, { -.5f, +.5f } },
		vertices,
		indices));

	// add low-res and high-res circles
	shapes.meshes.push_back(addPolygon(createPolygon(36), vertices, indices));
	shapes.meshes.push_back(addPolygon(createPolygon(100), vertices, indices));

	// add polygons with 3 to 10 sides
	for (int i = 3; i <= 10; i++)
		shapes.meshes.push_back(addPolygon(createPolygon(i), vertices, indices));

	shapes.buffer.reset(
		new DeviceBuffer(vertices.size(), vertices.data(), indices.size(), indices.data()));
	// return shapes;
}
