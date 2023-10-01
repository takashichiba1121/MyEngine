#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

	BaseScene* scene = new TitileScene();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::Instance()->SetSceneFactory(sceneFactory_.get());

	SceneManager::Instance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

	//gameScene->Update();
}

void MyGame::Draw()
{
	Framework::GetDxCommon()->PreDraw();

	SceneManager::Instance()->Draw(dxCommon_.get());

	Framework::GetDxCommon()->PostDrow();
}
