#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	model2.reset(Model::LoadFormOBJ("cube"));

	obj2 = std::make_unique<Object3d>();

	obj2->SetModel(model2.get());

	obj2->Initialize();

	obj2->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void GameScene::Update()
{
	Vector3 eye = Object3d::GetEye();

	if (Input::PushKey(DIK_Q))
	{
		eye.x++;
	}

	if (Input::PushKey(DIK_E))
	{
		eye.x--;
	}

	Object3d::SetEye(eye);

	Vector3 rot = obj2->GetRot();

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

	obj2->SetRot(rot);

	obj2->Update();

	imgui->Begin();

	ImGui::ShowDemoWindow();

	ImGui::Text("Hello, world %d", 123);


	ImGui::SliderFloat("X", &objX, -50.0f, 50.0f);

	ImGui::SliderFloat("Y", &objY, -50.0f, 50.0f);

	Vector3 move = obj2->GetPosition();

	move.x = objX;

	move.y = objY;

	obj2->SetPosition(move);

	imgui->End();
}

void GameScene::Draw()
{
	obj2->Draw();
}

void GameScene::SpriteDraw()
{

}
