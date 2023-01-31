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
}

void GameScene::Update()
{
	XINPUT_STATE pad = Input::GetPad();

	if (Input::TriggerKey(DIK_Z))
	{
		if (operation)
		{
			operation = false;
		}
		else
		{
			operation = true;
		}
	}
	Vector3 eye = Object3d::GetEye();

	Vector3 rot = obj->GetRot();

	if (operation)
	{

		eye.x += (pad.Gamepad.bLeftTrigger / 255)*2;

		eye.x -= (pad.Gamepad.bRightTrigger / 255)*2;

		if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			rot.x--;
		}
		if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			rot.x++;
		}
		if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			rot.y++;
		}
		if (pad.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			rot.y--;
		}
	}
	else
	{
		if (Input::PushKey(DIK_Q))
		{
			eye.x++;
		}

		if (Input::PushKey(DIK_E))
		{
			eye.x--;
		}

		if (Input::PushKey(DIK_A))
		{
			rot.x--;
		}
		if (Input::PushKey(DIK_D))
		{
			rot.x++;
		}
		if (Input::PushKey(DIK_W))
		{
			rot.y++;
		}
		if (Input::PushKey(DIK_S))
		{
			rot.y--;
		}
	}

	Object3d::SetEye(eye);

	obj->SetRot(rot);

	obj->Update();

	imgui->Begin();

	ImGui::ShowDemoWindow();

	ImGui::Text("Push Z");
	if (operation)
	{
		ImGui::Text("gamPad");
	}
	else
	{
		ImGui::Text("keyboard");
	}

	ImGui::SliderFloat("X", &objX, -50.0f, 50.0f);

	ImGui::SliderFloat("Y", &objY, -50.0f, 50.0f);

	Vector2 move = sprite->GetPosition();

	move.x = objX;

	move.y = objY;

	sprite->SetPosition(move);

	imgui->End();

	sprite->Update();
}

void GameScene::Draw()
{
	obj->Draw();
}

void GameScene::SpriteDraw()
{
	sprite->Draw();
}
