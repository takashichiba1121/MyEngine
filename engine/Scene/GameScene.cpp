#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	sprite = std::make_unique<Sprite>();

	sprite->Initialize(Texture::LoadTexture(L"Resources/mario.jpg"));

	model.reset(Model::LoadFormOBJ("cube"));

	obj = std::make_unique<Object3d>();

	obj->SetModel(model.get());

	obj->Initialize();

	Object3d::SetEye({ 0.0f,0.0f,-15.0f });

	sound = std::make_unique<Sound>();

	sound->SoundLoadWave("Resources/GameClear.wav");

	sound->SoundPlayWave(true,0.1f);

	polygonMana = std::make_unique<PolygonMana>();

	polygonMana->Initialize();
}

void GameScene::Update()
{
	if (Input::TriggerKey(DIK_SPACE))
	{ 
		
	}

	obj->Update();

	sprite->Update();

	polygonMana->Update();
}

void GameScene::Draw()
{
	obj->Draw();
}

void GameScene::SpriteDraw()
{
	sprite->Draw();
}
