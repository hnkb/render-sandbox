
#include "Shapes.h"
#include <delabella.h>
#include <cmath>

using namespace std;


void delabellaTriangulate(
	IDelaBella2<float>* idb,
	float2* start,
	size_t count,
	vector<uint32_t>& indices,
	int offset = 0)
{
	int verts = idb->Triangulate((int)count, &start->x, &start->y, sizeof(float2));
	if (verts > 0)
	{
		const auto tris = idb->GetNumPolygons();
		auto dela = idb->GetFirstDelaunaySimplex();
		for (int i = 0; i < tris; i++)
		{
			indices.push_back(dela->v[0]->i + offset);
			indices.push_back(dela->v[1]->i + offset);
			indices.push_back(dela->v[2]->i + offset);
			dela = dela->next;
		}
	}
	else
	{
		fprintf(stderr, "no points given or all points are colinear");
	}
}

Mesh addPolygonBella(
	IDelaBella2<float>* idb,
	vector<float2>& vertices,
	vector<uint32_t>& indices,
	const int vertexCount = 100)
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

	delabellaTriangulate(idb, vertices.data() + startVertex, vertexCount, indices, startVertex);

	return { startIndex, (int)indices.size() - startIndex };
}

void createPolygonShapes3(MeshCollection& shapes)
{
	auto idb = IDelaBella2<float>::Create();

	vector<float2> vertices = { { +.5f, +.5f }, { +.5f, -.5f }, { -.5f, -.5f }, { -.5f, +.5f } };
	vector<uint32_t> indices;

	vertices.reserve(200);
	indices.reserve(600);

	// add a quad with vertices already filled
	delabellaTriangulate(idb, vertices.data(), vertices.size(), indices);
	shapes.meshes.push_back({ 0, (int)indices.size() });

	// add low-res and high-res circles
	shapes.meshes.push_back(addPolygonBella(idb, vertices, indices, 36));
	shapes.meshes.push_back(addPolygonBella(idb, vertices, indices, 100));

	// add polygons with 3 to 10 sides
	for (int i = 3; i <= 10; i++)
		shapes.meshes.push_back(addPolygonBella(idb, vertices, indices, i));

	shapes.buffer.reset(
		new DeviceBuffer(vertices.size(), vertices.data(), indices.size(), indices.data()));

	idb->Destroy();
}
