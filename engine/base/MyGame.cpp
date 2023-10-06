#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"
#include"imguiManager.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

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
} 

void MyGame::Draw()
{
	Framework::GetDxCommon()->PreDraw();

	SceneManager::Instance()->Draw(dxCommon_.get());

	imguiManager::Draw();

	Framework::GetDxCommon()->PostDrow();

	
}
