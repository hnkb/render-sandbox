
#include "Shapes.h"
#include <cmath>

using namespace std;


Mesh addPolygon(vector<float2>& vertices, vector<uint32_t>& indices, const int vertexCount = 100)
{
	const auto startVertex = (int)vertices.size();
	const auto startIndex = (int)indices.size();

	const float radius = 0.5f;
	const float angleStep = 2.f * 3.14159265359f / vertexCount;

	for (int i = 0; i < vertexCount; ++i)
	{
		const float angle = i * angleStep;
		const float x = radius * cos(angle);
		const float y = radius * sin(angle);

		vertices.emplace_back(x, y);
	}

	for (int i = 1; i < vertexCount - 1; ++i)
	{
		indices.push_back(startVertex);
		indices.push_back(startVertex + i);
		indices.push_back(startVertex + i + 1);
	}

	return { startIndex, (int)indices.size() - startIndex };
}

void createPolygonShapes(MeshCollection& shapes)
{
	vector<float2> vertices = { { +.5f, +.5f }, { +.5f, -.5f }, { -.5f, -.5f }, { -.5f, +.5f } };
	vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };

	vertices.reserve(200);
	indices.reserve(600);

	// add a quad with vertices and indices already filled
	shapes.meshes.push_back({ 0, 6 });

	// add low-res and high-res circles
	shapes.meshes.push_back(addPolygon(vertices, indices, 36));
	shapes.meshes.push_back(addPolygon(vertices, indices, 100));

	// add polygons with 3 to 10 sides
	for (int i = 3; i <= 10; i++)
		shapes.meshes.push_back(addPolygon(vertices, indices, i));

	shapes.buffer.reset(
		new DeviceBuffer(vertices.size(), vertices.data(), indices.size(), indices.data()));
}
