#pragma once

#include "../renderer/Primitive.h"


void createPolygonShapes(MeshCollection& shapes);
void createPolygonShapes3(MeshCollection& shapes);

namespace Shapes
{
	constexpr int Quad = 0;
	constexpr int CircleLo = 1;
	constexpr int CircleHi = 2;
	constexpr int Triangle = 3;
	constexpr int Square = 4;
	constexpr int Pentagon = 5;
	constexpr int Hexagon = 6;
	constexpr int Heptagon = 7;
	constexpr int Octagon = 8;
	constexpr int Nonagon = 9;
	constexpr int Decagon = 10;
}
