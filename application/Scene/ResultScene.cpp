#include "ResultScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"TextureManager.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"Framework.h"
void ResultScene::Initialize()
{

	Object3d::SetEye({ 0.0f,0.0f,-200.0f });

	Object3d::SetTarget({ 0.0f,0.0f,1.0f });

	padTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/Abotton.png");

	buttonSprite_ = std::make_unique<Sprite>();

	buttonSprite_->Initialize(padTexHandle_);

	buttonSprite_->SetPosition({ 640.0f,515.0f });

	buttonSprite_->SetAnchorPoint({ 0.5f,0.5f });

	buttonSprite_->Update();

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/scene.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->Update();

	resultSprite_ = std::make_unique<Sprite>();

	resultSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Clear.png"));

	resultSprite_->SetPosition({ 640,230 });

	resultSprite_->SetAnchorPoint({ 0.5f,0.5f });

	resultSprite_->Update();

	titleSprite_ = std::make_unique<Sprite>();

	titleSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Title.png"));

	titleSprite_->SetPosition({ 640,600 });

	titleSprite_->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->Update();

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

	particles_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect4.png"));
}

void ResultScene::Finalize()
{
}

void ResultScene::Update()
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
		frame_++;
		float f = ( float ) frame_ / endFrame_;

		sceneSprite_->SetDissolve(f);

		if ( frame_ >= ( int32_t ) endFrame_ )
		{
			sceneStart_ = false;

			sceneSprite_->SetMaskTexture(TextureManager::Instance()->LoadTexture("Resources/Dissolve3.png"));
		}
		sceneSprite_->Update();
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
		if ( frame_ > 0 )
		{
			frame_--;

			float f = ( float ) frame_ / endFrame_;

			sceneSprite_->SetDissolve(f);

		}
		else if ( frame_ < -5 )
		{
			SceneManager::Instance()->ChangeScene("TITLE");
		}
		else
		{
			frame_--;
		}
	}
	sceneSprite_->Update();

	buttonSprite_->Update();

	particles_->Update();
}

void ResultScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());
	skyObj_->Draw();
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	particles_->Draw();

	ParticleManager::PostDraw();
}

void ResultScene::SpriteDraw()
{
	SpriteCommon::Instance()->PreDraw();
	buttonSprite_->Draw();
	titleSprite_->Draw();
	resultSprite_->Draw();
	sceneSprite_->DissolveDraw();
	SpriteCommon::Instance()->PostDraw();
}
