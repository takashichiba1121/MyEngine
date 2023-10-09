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

	spaceTexHandle = Texture::Instance()->LoadTexture("Resources/spaceKey.png");

	aTexHandle = Texture::Instance()->LoadTexture("Resources/Abotton.png");

	spaceSprite = std::make_unique<Sprite>();

	if (Input::Instance()->IsLinkGamePad())
	{
		spaceSprite->Initialize(aTexHandle);
	}
	else 
	{
		spaceSprite->Initialize(spaceTexHandle);
	}

	spaceSprite->SetPosition({ 640.0f,515.0f });

	spaceSprite->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite->Update();

	sceneSprite = std::make_unique<Sprite>();

	sceneSprite->Initialize(Texture::Instance()->LoadTexture("Resources/scene.png"));

	sceneSprite->SetAnchorPoint({ 0,0 });

	sceneSprite->SetScale({ 1280,720 });

	sceneSprite->SetPosition({ 0,float(startSpriteY) });

	sceneSprite->Update();

	titleSprite = std::make_unique<Sprite>();

	titleSprite->Initialize(Texture::Instance()->LoadTexture("Resources/Titlerogo.png"));

	titleSprite->SetPosition({ 640,230 });

	titleSprite->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite->Update();

	startSprite = std::make_unique<Sprite>();

	startSprite->Initialize(Texture::Instance()->LoadTexture("Resources/Start.png"));

	startSprite->SetPosition({ 640,600 });

	startSprite->SetAnchorPoint({ 0.5f,0.5f });

	startSprite->Update();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle", true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 0,0,0 });

	skyObj_->SetScale({ 200,200,200 });

	skyObj_->SetRot({ 0.0f,0,0 });

	skyObj_->Update();

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());

	particle = std::make_unique<ParticleManager>();

	particle->Initialize();

	particle->SetTextureHandle(Texture::Instance()->LoadTexture("Resources/effect4.png"));
}

void TitileScene::Finalize()
{
}

void TitileScene::Update()
{
	ParticleFlame++;
	if ( ParticleFlame >= 30 )
	{
		ParticleFlame = 0;
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
			particle->Add(life,startPos,{ 0,0.2f,0 },accel,scale,scale,{ 1,1,1,1 },{ 1,1,1,0 });
		}
	}


	if (sceneStart)
	{
		frame--;
		float f = (float)frame / endFrame;

		sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

		if (frame <= 0)
		{
			sceneStart = false;
			frame = 0;
		}
		sceneSprite->Update();
	}
	else
	{
		if (Input::Instance()->IsLinkGamePad())
		{
			spaceSprite->SetTexture(aTexHandle);
			if (Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A))
			{
				sceneChange = true;
			}
		}
		else
		{
			spaceSprite->SetTexture(spaceTexHandle);
			if (Input::Instance()->TriggerKey(DIK_SPACE))
			{
				sceneChange = true;
			}
		}
	}
	if (sceneChange)
	{
		if (frame < endFrame)
		{
			frame++;

			float f = Easing::easeOutBounce((float)frame / endFrame);

			sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

		}
		else if (frame >= endFrame + 5)
		{
			frame = 120;

			sceneStart = true;

			sceneChange = false;

			SceneManager::Instance()->ChangeScene("GAME");
		}
		else
		{
			frame++;
		}
	}
	sceneSprite->Update();

	spaceSprite->Update();

	particle->Update();

	ImGui::Begin("Partcle");

	ImGui::Text("%d", frame);

	ImGui::Text("%f", sceneSprite->GetPosition().y);

	ImGui::End();
}

void TitileScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());
	skyObj_->Draw();
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	particle->Draw();

	ParticleManager::PostDraw();

	SpriteCommon::Instance()->PreDraw();
	spaceSprite->Draw();
	startSprite->Draw();
	titleSprite->Draw();
	sceneSprite->Draw();
	SpriteCommon::Instance()->PostDraw();
}
