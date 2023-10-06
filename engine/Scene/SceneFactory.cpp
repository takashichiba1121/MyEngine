#include "SceneFactory.h"
#include"TitileScene.h"
#include"GameScene.h"
#include"GameOverScene.h"
#include"ResultScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;

	if (sceneName=="TITLE")
	{
		newScene = new TitileScene();
	}
	else if (sceneName=="GAME")
	{
		newScene = new GameScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GameOverScene();
	}
	else if (sceneName == "RESULT")
	{
		newScene = new ResultScene();
	}
	return newScene;
}
