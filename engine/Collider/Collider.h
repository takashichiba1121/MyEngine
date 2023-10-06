#pragma once
#include"Vector3.h"

struct  Cube
{
	Vector3 Pos;
	Vector3 oldPos;
	Vector3 scale;
};

namespace Collider
{
	bool CubeAndCube(Cube A,Cube B);

	bool QuadAndQuad(Cube A, Cube B);
} 