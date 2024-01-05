#pragma once

#include"BaseScene.h"
#include<string>
/*
* 抽象的なシーンファクトリー
*/
class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;

	virtual BaseScene* CreateScene(const std::string& sceneName) = 0; 
};

