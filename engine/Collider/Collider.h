#pragma once
#include"Vector3.h"
/*
* 当たり判定の関数(適宜追加)
*/
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
		float scale;
	};

	enum Type
	{
		Collsion,
		Inside,
	};
	//キューブ同士の当たり判定
	bool CubeAndCube(Cube A,Cube B,Type type);

	//四角同士の当たり判定
	bool QuadAndQuad(Cube A, Cube B,Type type);

	//球同士のああtリ判定
	bool SphereAndSphere(Sphere A,Sphere B,Type type,Vector3* reject = nullptr);
} 