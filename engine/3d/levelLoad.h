#pragma once
#include"json.hpp"
#include<d3d12.h>
#include <DirectXMath.h>
#include"Model.h"
#include"object3d.h"

struct LevelData
{
	struct ObjectData
	{
		std::string fileName;
		Vector3 trans;
		Vector3 scale;
		Vector3 rot;
		Vector3 center;
		Vector3 size;
		std::string tagName;
	};

	std::list<ObjectData> objects;
};
/*
* jsonからオブジェクトデータを読みとってまとめる。
*/
class LevelLoad
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="fullpath">jsonファイルのパス</param>
	/// <returns>複数のオブジェクトの情報がまとまった専用の構造体</returns>
	static LevelData* Load(const std::string fullpath); 
};