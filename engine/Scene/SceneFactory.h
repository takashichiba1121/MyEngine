#pragma once
#include "AbstractSceneFactory.h"
class SceneFactory :
    public AbstractSceneFactory
{
    /// <summary>
    /// �V�[������
    /// </summary>
    /// <param name="sceneName">�V�[����</param>
    /// <returns>���������V�[��</returns>
    BaseScene* CreateScene(const std::string& sceneName) override;
};

