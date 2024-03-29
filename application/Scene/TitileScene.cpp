#include "TitileScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"TextureManager.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"Framework.h"
void TitileScene::Initialize()
{

	Object3d::SetEye({ 0.0f,0.0f,-200.0f });

	Object3d::SetTarget({ 0.0f,0.0f,1.0f });

	padTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/Texture/Abotton.png");

	spaceSprite_ = std::make_unique<Sprite>();

	spaceSprite_->Initialize(padTexHandle_);

	spaceSprite_->SetPosition({ 640.0f,515.0f });

	spaceSprite_->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite_->Update();

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/scene2.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->Update();

	titleSprite_ = std::make_unique<Sprite>();

	titleSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Titlerogo.png"));

	titleSprite_->SetPosition({ 640,230 });

	titleSprite_->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->Update();

	startSprite_ = std::make_unique<Sprite>();

	startSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Start.png"));

	startSprite_->SetPosition({ 640,600 });

	startSprite_->SetAnchorPoint({ 0.5f,0.5f });

	startSprite_->Update();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle",true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 0,0,0 });

	skyObj_->SetScale({ 200,200,200 });

	skyObj_->SetRot({ 0.0f,0,0 });

	skyObj_->Update();

	light_.reset(LightGroup::Create());

	Object3d::SetLight(light_.get());

	particles_ = std::make_unique<ParticleManager>();

	particles_->Initialize();

	particles_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/Texture/effect4.png"));
}

void TitileScene::Finalize()
{
}

void TitileScene::Update()
{
	ParticleFlame_++;
	if ( ParticleFlame_ >= 30 )
	{
		ParticleFlame_ = 0;
		for ( int i = 0; i < 2; i++ )
		{
			//消えるまでの時間
			const float rnd_life = 300.0f;
			//最低限のライフ
			const float constlife = 100;
			uint32_t life = static_cast< uint32_t >( rand() / RAND_MAX * rnd_life + constlife );

			//XYZの広がる距離
			const float rnd_pos = 28.0f;
			Vector3 startPos{};
			startPos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
			startPos.y = -8;
			startPos.z = -180;


			Vector3 accel = { 0,0,0 };

			//消えるまでの時間
			const float rndScale = 0.7f;
			//最低限のライフ
			const float constScale = 0.7f;
			float scale = ( float ) rand() / RAND_MAX * rndScale + constScale;
			//追加
			particles_->Add(life,startPos,{ 0,0.2f,0 },accel,scale,scale,{ 1,1,1,1 },{ 1,1,1,0 });
		}
	}


	if ( sceneStart_ )
	{
		SceneChangeFrame_--;
		float f = ( float ) SceneChangeFrame_ / cEndSceneChangeFrame_;

		sceneSprite_->SetColor({1,1,1,f});

		if ( SceneChangeFrame_ <=0 )
		{
			sceneStart_ = false;
		}
	}
	else
	{
		if ( Input::Instance()->IsLinkGamePad() )
		{
			if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A) )
			{
				sceneChange_ = true;
			}
		}
		else
		{
			if ( Input::Instance()->TriggerKey(DIK_SPACE) )
			{
				sceneChange_ = true;
			}
		}
	}
	if ( sceneChange_ )
	{
		if ( SceneChangeFrame_ < cEndSceneChangeFrame_ )
		{
			SceneChangeFrame_++;

			float f = ( float ) SceneChangeFrame_ / cEndSceneChangeFrame_;

			sceneSprite_->SetColor({ 1,1,1,f });

		}
		else if ( SceneChangeFrame_ < cEndSceneChangeFrame_ +5 )
		{
			SceneChangeFrame_ = 0;

			sceneStart_ = true;

			sceneChange_ = false;

			SceneManager::Instance()->ChangeScene("GAME");
		}
		else
		{
			SceneChangeFrame_++;
		}
	}
	sceneSprite_->Update();

	spaceSprite_->Update();

	particles_->Update();
}

void TitileScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());
	skyObj_->Draw();
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	particles_->Draw();

	ParticleManager::PostDraw();
}

void TitileScene::SpriteDraw()
{
	SpriteCommon::Instance()->PreDraw();
	spaceSprite_->Draw();
	startSprite_->Draw();
	titleSprite_->Draw();
	sceneSprite_->Draw();
	SpriteCommon::Instance()->PostDraw();
}
