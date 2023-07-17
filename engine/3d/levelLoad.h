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
		DirectX::XMVECTOR trans;
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR center;
		DirectX::XMVECTOR size;
	};

	std::list<ObjectData> objects;
};

class LevelLoad
{
public:
	static LevelData* Load();
};