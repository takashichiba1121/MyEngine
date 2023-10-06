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

class LevelLoad
{
public:
	static LevelData* Load(const std::string fullpath); 
};