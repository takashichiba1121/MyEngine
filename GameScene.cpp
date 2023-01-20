#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete obj;
	delete model;
	delete sprite;
	delete sprite2;
}

void GameScene::Initialize(GameScene* gameScene)
{
	uint32_t tex2 = Texture::LoadTexture(L"Resources/Number.png");

	uint32_t tex = Texture::LoadTexture(L"Resources/reimu.png");

	sprite = new Sprite;

	sprite->Initialize(tex);

	sprite->SetPosition({ WinApp::window_width / 2,WinApp::window_heigit / 2 });

	sprite->SetAnchorPoint({ 0.0f,0.0f });

	sprite2 = new Sprite;

	sprite2->Initialize(tex);

	sprite2->SetPosition({ WinApp::window_width / 2,WinApp::window_heigit / 2 });

	sprite2->SetAnchorPoint({ 1.0f,1.0f });


	model = Model::LoadFormOBJ("playerbullet");

	obj->SetModel(model);

	obj->Initialize();
}

void GameScene::Update()
{
	sprite->Update();

	sprite2->Update();

	obj->Update();

}

void GameScene::ObjectDraw()
{
	obj->Draw();
}

void GameScene::SpriteDraw()
{

	sprite->Draw();

	sprite2->Draw();
}
