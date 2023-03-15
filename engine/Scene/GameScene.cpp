#include "GameScene.h"
#include"DirectXCommon.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	sprite = std::make_unique<Sprite>();

	sprite->SetPosition({0,0});

	sprite->Initialize(Texture::LoadTexture(L"Resources/mario.jpg"));

	model.reset(Model::LoadFormOBJ("cube"));

	obj = std::make_unique<Object3d>();

	obj->SetModel(model.get());

	obj->Initialize();

	Object3d::SetEye({ 0.0f,0.0f,-5.0f });

	sound = std::make_unique<Sound>();

	sound->SoundLoadWave("Resources/GameClear.wav");

	sound->SoundPlayWave(true,0.1f);
}

void GameScene::Update()
{
	if (Input::TriggerKey(DIK_SPACE))
	{ 
		
	}

	obj->Update();

	sprite->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	//obj->Draw();

	Object3d::PostDraw();

	SpriteCommon::PreDraw();

	sprite->Draw();

	SpriteCommon::PostDraw();
}
