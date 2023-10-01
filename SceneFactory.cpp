#include "SceneFactory.h"
#include"TitileScene.h"
#include"GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//Ÿ‚ÌƒV[ƒ“‚ğ¶¬
	BaseScene* newScene = nullptr;

	if (sceneName=="TITLE")
	{
		newScene = new TitileScene();
	}
	else if (sceneName=="GAME")
	{
		newScene = new GameScene();
	}
	return newScene;
}
