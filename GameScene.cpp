#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete obj;
	delete model;
	delete model2;
	delete obj2;
}

void GameScene::Initialize(GameScene* gameScene)
{

	model2 = Model::LoadFormOBJ("cube");

	obj2->SetModel(model2);

	obj2->Initialize();

	obj2->SetScale({ 10.0f, 10.0f, 10.0f });

	obj2->SetPosition({ 30.0f, 00.0f, 0.0f });
}

void GameScene::Update()
{
	obj2->Update();
}

void GameScene::ObjectDraw()
{
	obj2->Draw();
}

void GameScene::SpriteDraw()
{

}
