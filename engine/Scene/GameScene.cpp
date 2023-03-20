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

	//sprite->SetPosition({-70,-70});

	sprite->Initialize(Texture::LoadTexture(L"Resources/texture.png"));
	sprite->SetScale({100,100});

	model.reset(Model::LoadFormOBJ("cube"));
	
	obj = std::make_unique<Object3d>();

	obj->SetModel(model.get());

	obj->Initialize();

	obj->SetRot({45.0f,-135.0f,0.0f});

	Object3d::SetEye({ 0.0f,0.0f,-20.0f });

	pMan = std::make_unique<ParticleManager>();

	pMan->Initialize();

	//sound = std::make_unique<Sound>();

	//sound->SoundLoadWave("Resources/GameClear.wav");

	//sound->SoundPlayWave(true,0.1f);
}

void GameScene::Update()
{

	//sprite->SetPosition(pos);
	if (Input::TriggerKey(DIK_SPACE))
	{
		isP = true;
	}
	if (isP)
	{
		flame++;
		//スペースキーを押していたら
		for (int i = 0; i < 50; i++)
		{
			//消えるまでの時間
			const float rnd_life = 30.0f;
			//最低限のライフ
			const float constlife = 10;
			float life = (float)rand() / RAND_MAX * rnd_life - rnd_life / 2.0f + constlife;

			//XYZの広がる距離
			const float rnd_pos = 20.0f;
			//Y方向には最低限の飛ぶ距離
			const float constPosY = 15;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			//追加
			pMan->InAdd(life, pos, { 0,0,0 }, 0.1, 0.1, { 1,1,0,0 }, { 1,1,0,1 });
		}
		if (flame>=10)
		{
			isP = false;
			flame = 0;
		}
	}

	obj->Update();

	pMan->Update();

	sprite->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	//obj->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//pMan->Draw();
	ParticleManager::PostDraw();

	SpriteCommon::PreDraw();

	//sprite->Draw();

	SpriteCommon::PostDraw();
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{

	Object3d::PreDraw(dxCommon->GetCommandList());

	//obj->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	pMan->Draw();
	ParticleManager::PostDraw();

	SpriteCommon::PreDraw();

	//sprite->Draw();

	SpriteCommon::PostDraw();
}
