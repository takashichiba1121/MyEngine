#include"object3d.h"
#include"input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Texture.h"
#include"Audio.h"
#include"imguiManager.h"
#include<imgui.h>

using namespace DirectX;
using namespace std;

//windowsアプリのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = nullptr;
	winApp = new WinApp;
	winApp->Initialize();

	MSG msg{};

	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//DirectX初期化処理　ここから

	Input::Initialize(winApp);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Texture::Initialize(dxCommon->GetDevice());

	Sprite::StaticInitialize(spriteCommon);

	Model::SetDevice(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_heigit);

	Sound::StaticInitialize();

	imguiManager* imgui = new imguiManager;
	imgui->Initialize(winApp, dxCommon);

	Model* model2;

	Object3d* obj2 = new Object3d;

	model2 = Model::LoadFormOBJ("cube");

	obj2->SetModel(model2);

	obj2->Initialize();

	//obj2->SetScale({ 10.0f, 10.0f, 10.0f });

	obj2->SetPosition({ 0.0f, 0.0f, 0.0f });

	float objX = 0;

	float objY = 0;

	//ゲームループ
	while (true) {

		//×ボタンで終了メッセージがきたらゲームループを抜ける
		if (winApp->ProcessMessage()) {
			break;
		}
		//DirectX毎フレーム処理　ここから

		//入力の更新
		Input::Update();

		Vector3 eye=Object3d::GetEye();

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


		ImGui::SliderFloat("X", &objX,-50.0f, 50.0f);

		ImGui::SliderFloat("Y", &objY, -50.0f, 50.0f);

		Vector3 move = obj2->GetPosition();

		move.x = objX;

		move.y = objY;

		obj2->SetPosition(move);

		imgui->End();

		//描画コマンドここから
		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCommandList());

		obj2->Draw();

		Object3d::PostDraw();


		spriteCommon->PreDraw();

		spriteCommon->PostDrow();

		//描画コマンドここまで

		imgui->Draw();

		dxCommon->PostDrow();

		//DirectX毎フレーム処理　ここまで

	}

	delete model2;
	delete obj2;

	delete spriteCommon;

	imgui->Finalize();

	delete imgui;

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}