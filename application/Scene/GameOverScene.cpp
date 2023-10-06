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

	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

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
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
{
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

	SpriteCommon::Instance()->PreDraw();
	buttonSprite->Draw();
	titleSprite->Draw();
	gameOverSprite->Draw();
	sceneSprite->Draw();
	SpriteCommon::Instance()->PostDraw();
}
