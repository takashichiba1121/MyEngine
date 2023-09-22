#include "MyGame.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

	gameScene = make_unique<GameScene>();
	gameScene->Initialize();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

	gameScene->Update();
}

void MyGame::Draw()
{
	Framework::GetDxCommon()->PreDraw();

	gameScene->Draw(Framework::GetDxCommon());

	Framework::GetDxCommon()->PostDrow();
}
