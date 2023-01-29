#include<d3dcompiler.h>
#include<Windows.h>
#include <tchar.h>
#include<iostream>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<DirectXTex.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include"object3d.h"
#include"input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Texture.h"
#include"GameScene.h"
#include"Audio.h"

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

	SpriteCommon* spriteCommon=nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Texture::Initialize(dxCommon->GetDevice());

	Sprite::StaticInitialize(spriteCommon);

	Model::SetDevice(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_heigit);

	Sound::StaticInitialize();

	
	GameScene* gameScene=new GameScene;
	gameScene->Initialize(gameScene);

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

		//描画コマンドここから
		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCommandList());

		gameScene->ObjectDraw();

		Object3d::PostDraw();


		spriteCommon->PreDraw();

		gameScene->SpriteDraw();

		spriteCommon->PostDrow();

		//描画コマンドここまで

		dxCommon->PostDrow();

		//DirectX毎フレーム処理　ここまで

	}

	delete spriteCommon;

	delete gameScene;

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}