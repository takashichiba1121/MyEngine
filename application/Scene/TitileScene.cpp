#include "TitileScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"Texture.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"Framework.h"
void TitileScene::Initialize()
{

	Object3d::SetEye({ 0.0f,0.0f,-200.0f });

	Object3d::SetTarget({ 0.0f,0.0f,1.0f });

	keyTexHandle_ = Texture::Instance()->LoadTexture("Resources/spaceKey.png");

	padTexHandle_ = Texture::Instance()->LoadTexture("Resources/Abotton.png");

	spaceSprite_ = std::make_unique<Sprite>();

	if (Input::Instance()->IsLinkGamePad())
	{
		spaceSprite_->Initialize(padTexHandle_);
	}
	else 
	{
		spaceSprite_->Initialize(keyTexHandle_);
	}

	spaceSprite_->SetPosition({ 640.0f,515.0f });

	spaceSprite_->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite_->Update();

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(Texture::Instance()->LoadTexture("Resources/scene.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->SetPosition({ 0,float(startSpriteY_) });

	sceneSprite_->Update();

	titleSprite_ = std::make_unique<Sprite>();

	titleSprite_->Initialize(Texture::Instance()->LoadTexture("Resources/Titlerogo.png"));

	titleSprite_->SetPosition({ 640,230 });

	titleSprite_->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->Update();

	startSprite_ = std::make_unique<Sprite>();

	startSprite_->Initialize(Texture::Instance()->LoadTexture("Resources/Start.png"));

	startSprite_->SetPosition({ 640,600 });

	startSprite_->SetAnchorPoint({ 0.5f,0.5f });

	startSprite_->Update();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle", true));

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

	particles_->SetTextureHandle(Texture::Instance()->LoadTexture("Resources/effect4.png"));

	bgm.Load("Resources/GameClear.wav");

	bgm.Play(true,0.1f);
}

void TitileScene::Finalize()
{
	bgm.Stop();
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
			uint32_t life = static_cast<uint32_t>( rand() / RAND_MAX * rnd_life + constlife);

			//XYZの広がる距離
			const float rnd_pos = 28.0f;
			Vector3 startPos{};
			startPos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
			startPos.y = -8;
			startPos.z = -180;


			Vector3 accel = {0,0,0};

			//消えるまでの時間
			const float rndScale = 0.7f;
			//最低限のライフ
			const float constScale = 0.7f;
			float scale = ( float ) rand() / RAND_MAX * rndScale + constScale;
			//追加
			particles_->Add(life,startPos,{ 0,0.2f,0 },accel,scale,scale,{ 1,1,1,1 },{ 1,1,1,0 });
		}
	}


	if (sceneStart_)
	{
		frame_--;
		float f = (float)frame_ / endFrame_;

		sceneSprite_->SetPosition({ 0,((endSpriteY_ - startSpriteY_) * f) + startSpriteY_ });

		if (frame_ <= 0)
		{
			sceneStart_ = false;
			frame_ = 0;
		}
		sceneSprite_->Update();
	}
	else
	{
		if (Input::Instance()->IsLinkGamePad())
		{
			spaceSprite_->SetTexture(padTexHandle_);
			if (Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A))
			{
				sceneChange_ = true;
			}
		}
		else
		{
			spaceSprite_->SetTexture(keyTexHandle_);
			if (Input::Instance()->TriggerKey(DIK_SPACE))
			{
				sceneChange_ = true;
			}
		}
	}
	if (sceneChange_)
	{
		if (frame_ < endFrame_)
		{
			frame_++;

			float f = Easing::easeOutBounce((float)frame_ / endFrame_);

			sceneSprite_->SetPosition({ 0,((endSpriteY_ - startSpriteY_) * f) + startSpriteY_ });

		}
		else if (frame_ >= endFrame_ + 5)
		{
			frame_ = 120;

			sceneStart_ = true;

			sceneChange_ = false;

			SceneManager::Instance()->ChangeScene("GAME");
		}
		else
		{
			frame_++;
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

	SpriteCommon::Instance()->PreDraw();
	spaceSprite_->Draw();
	startSprite_->Draw();
	titleSprite_->Draw();
	sceneSprite_->Draw();
	SpriteCommon::Instance()->PostDraw();
}
