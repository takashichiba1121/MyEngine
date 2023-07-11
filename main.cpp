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
#include"GameScene.h"
#include"PostEffectCommon.h"
#include"PostEffectBlurW.h"
#include"PostEffectBlurH.h"
#include"PostEffectLuminance.h"
#include"PostEffectMixed.h"
#include"PostEffectTest.h"
#include"PostEffectMultiRenderTarget.h"
#include"assimpModel.h"
#include"assimpObject3d.h"
#include"LightGroup.h"

using namespace DirectX;
using namespace std;

//windowsアプリのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	unique_ptr<WinApp> winApp = nullptr;
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	MSG msg{};

	unique_ptr<DirectXCommon> dxCommon = nullptr;
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	//DirectX初期化処理　ここから

	Input::Initialize(winApp.get());

	SpriteCommon::Initialize(dxCommon.get());

	Texture::Initialize(dxCommon.get());

	Sprite::StaticInitialize();

	Model::SetDevice(dxCommon->GetDevice());

	LightGroup::StaticInitialize(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	AssimpModel::SetDevice(dxCommon->GetDevice());

	assimpObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp.get(), dxCommon.get());
	imguiManager imGui;

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	unique_ptr<PostEffectCommon> postEffectCommon = std::make_unique<PostEffectCommon>();

	postEffectCommon->StaticInitialize(dxCommon.get());

	unique_ptr<PostEffectLuminance> postEffectLuminance = std::make_unique<PostEffectLuminance>();

	postEffectLuminance->Initialize(postEffectCommon.get());

	unique_ptr<PostEffectBlur> postEffectBlur = std::make_unique<PostEffectBlur>();

	postEffectBlur->Initialize(postEffectCommon.get());

	unique_ptr<PostEffectMixed> postEffectMixed = std::make_unique<PostEffectMixed>();

	postEffectMixed->Initialize(postEffectCommon.get());

	unique_ptr<PostEffectTest> postEffectTest = std::make_unique<PostEffectTest>();

	postEffectTest->Initialize(postEffectCommon.get());

	unique_ptr<PostEffectMultiRenderTarget> postEffectMultiRenderTarget = std::make_unique<PostEffectMultiRenderTarget>();

	postEffectMultiRenderTarget->Initialize(postEffectCommon.get());

	unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize();

	bool frg = true;

	//ゲームループ
	while (true) {

		//×ボタンで終了メッセージがきたらゲームループを抜ける
		if (winApp->ProcessMessage()) {
			break;
		}
		//DirectX毎フレーム処理　ここから

		//入力の更新
		Input::Update();

		imGui.Begin();
		gameScene->Update();
		imGui.End();

		//postEffect->Update();

		//if (Input::TriggerKey(DIK_0)||Input::PadTriggerKey(XINPUT_GAMEPAD_A))
		//{
		//	if (frg)
		//	{
		//		frg = false;
		//	}
		//	else
		//	{
		//		frg = true;
		//	}
		//}

		//描画コマンドここから

		/*if (frg)
		{
			postEffectMultiRenderTarget->PreDrawScene(dxCommon->GetCommandList());

			gameScene->PostEffectDraw(dxCommon);

			postEffectMultiRenderTarget->PostDrawScene();

			postEffectTest->PreDrawScene(dxCommon->GetCommandList());

			postEffectMultiRenderTarget->Draw();

			postEffectTest->PostDrawScene();
		}
		else
		{

			postEffectLuminance->PreDrawScene(dxCommon->GetCommandList());

			gameScene->PostEffectDraw(dxCommon);

			postEffectLuminance->PostDrawScene();

			postEffectBlur->PreDrawScene(dxCommon->GetCommandList());

			postEffectLuminance->Draw();

			postEffectBlur->PostDrawScene();

			postEffectMixed->PreDrawScene(dxCommon->GetCommandList());

			postEffectBlur->Draw();

			postEffectMixed->PostDrawScene();
		}*/
		dxCommon->PreDraw();
		//if (frg)
		//{
		//	postEffectTest->Draw();
		//}
		//else
		//{
		//	postEffectMixed->Draw(postEffectLuminance->GettextureHandle());
		//}

		gameScene->Draw(dxCommon.get());

		imguiManager::Draw();

		dxCommon->PostDrow();

		//描画コマンドここまで

		//DirectX毎フレーム処理　ここまで

	}

	Texture::fin();

	imguiManager::Finalize();

	winApp->Finalize();

	return 0;
}