#pragma once
#include "Types.h"


struct Vertex
{
	Vec3 position;
	Color color;
	Vec2 UV;
};

struct TransformData
{
	Vec3 offset;
	float dummy;
};

