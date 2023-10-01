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

	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

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

	skyObj_->SetRot({ 1.57f,0,0 });

	skyObj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	skyObj_->Update();

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());
}

void TitileScene::Finalize()
{
}

void TitileScene::Update()
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

	SpriteCommon::Instance()->PreDraw();
	spaceSprite->Draw();
	startSprite->Draw();
	titleSprite->Draw();
	sceneSprite->Draw();
	SpriteCommon::Instance()->PostDraw();
}
