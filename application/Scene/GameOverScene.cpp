#include"GameOverScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"Texture.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"Framework.h"
void GameOverScene::Initialize()
{

	Object3d::SetEye({ 0.0f,0.0f,-200.0f });

	Object3d::SetTarget({ 0.0f,0.0f,1.0f });

	spaceTexHandle = Texture::Instance()->LoadTexture("Resources/spaceKey.png");

	aTexHandle = Texture::Instance()->LoadTexture("Resources/Abotton.png");

	buttonSprite = std::make_unique<Sprite>();

	if (Input::Instance()->IsLinkGamePad())
	{
		buttonSprite->Initialize(aTexHandle);
	} 
	else
	{
		buttonSprite->Initialize(spaceTexHandle);
	}

	buttonSprite->SetPosition({ 640.0f,515.0f });

	buttonSprite->SetAnchorPoint({ 0.5f,0.5f });

	buttonSprite->Update();

	sceneSprite = std::make_unique<Sprite>();

	sceneSprite->Initialize(Texture::Instance()->LoadTexture("Resources/scene.png"));

	sceneSprite->SetAnchorPoint({ 0,0 });

	sceneSprite->SetScale({ 1280,720 });

	sceneSprite->SetPosition({ 0,float(startSpriteY) });

	sceneSprite->Update();

	gameOverSprite = std::make_unique<Sprite>();

	gameOverSprite->Initialize(Texture::Instance()->LoadTexture("Resources/GameOver.png"));

	gameOverSprite->SetPosition({ 640,230 });

	gameOverSprite->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSprite->Update();

	titleSprite = std::make_unique<Sprite>();

	titleSprite->Initialize(Texture::Instance()->LoadTexture("Resources/Title.png"));

	titleSprite->SetPosition({ 640,600 });

	titleSprite->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite->Update();

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

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
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
			buttonSprite->SetTexture(aTexHandle);
			if (Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A))
			{
				sceneChange = true;
			}
		}
		else
		{
			buttonSprite->SetTexture(spaceTexHandle);
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

			SceneManager::Instance()->ChangeScene("TITLE");
		}
		else
		{
			frame++;
		}
	}
	sceneSprite->Update();

	buttonSprite->Update();

	particle->Update();

	ImGui::Begin("Partcle");

	ImGui::Text("%d", frame);

	ImGui::Text("%f", sceneSprite->GetPosition().y);

	ImGui::End();
}

void GameOverScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());
	skyObj_->Draw();
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());

	particle->Draw();

	ParticleManager::PostDraw();

	SpriteCommon::Instance()->PreDraw();
	buttonSprite->Draw();
	titleSprite->Draw();
	gameOverSprite->Draw();
	sceneSprite->Draw();
	SpriteCommon::Instance()->PostDraw();
}
