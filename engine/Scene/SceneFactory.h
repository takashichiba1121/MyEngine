#pragma once
#include "AbstractSceneFactory.h"
/*
* シーンの生成
*/
class SceneFactory :
    public AbstractSceneFactory
{
    /// <summary>
    /// シーン生成
    /// </summary>
    /// <param name="sceneName">シーン名</param>
    /// <returns>生成したシーン</returns>
    BaseScene* CreateScene(const std::string& sceneName) override; 
};

