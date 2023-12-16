/*
* 当たり判定の関数(適宜追加)
*/

#pragma once
#include"Vector3.h"

namespace Collider
{
	struct  Cube
	{
		Vector3 Pos;
		Vector3 oldPos;
		Vector3 scale;
	};

	struct  Sphere
	{
		Vector3 Pos;
		Vector3 scale;
	};

	enum Type
	{
		Collsion,
		Inside,
	};

	bool CubeAndCube(Cube A,Cube B,Type type);

	bool QuadAndQuad(Cube A, Cube B,Type type);

	bool SphereAndSphere(Sphere A,Sphere B,Type type);
} 