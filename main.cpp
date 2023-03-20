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
#include"PostEffect.h"

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

	SpriteCommon::Initialize(dxCommon);

	Texture::Initialize(dxCommon->GetDevice());

	Sprite::StaticInitialize();

	Model::SetDevice(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp, dxCommon);

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	PostEffect* postEffect = new PostEffect;

	postEffect->Initialize(dxCommon);

	GameScene* gameScene = new GameScene;
	gameScene->Initialize();

	//ゲームループ
	while (true) {

		//×ボタンで終了メッセージがきたらゲームループを抜ける
		if (winApp->ProcessMessage()) {
			break;
		}
		//DirectX毎フレーム処理　ここから

		//入力の更新
		Input::Update();

		gameScene->Update();

		//postEffect->Update();

		//描画コマンドここから

		postEffect->PreDrawScene(dxCommon->GetCommandList());

		gameScene->PostEffectDraw(dxCommon);

		postEffect->PostDrawScene();

		//描画コマンドここまで

		//imguiManager::Draw();
		dxCommon->PreDraw();

		postEffect->Draw();

		gameScene->Draw(dxCommon);
		
		dxCommon->PostDrow();

		//描画コマンドここまで

		//DirectX毎フレーム処理　ここまで

	}
	delete postEffect;

	delete gameScene;

	imguiManager::Finalize();

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}