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

using namespace DirectX;
using namespace std;


////定数バッファ用データ構造体（マテリアル）
//struct ConstBufferDataMaterial {
//	XMFLOAT4 color;//色（RGBA）
//};

//ウィンドウプロージャー
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wapram, LPARAM lparam) {
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wapram, lparam);
}
//windowsアプリのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//const int window_width = 1280;//横
	//const int window_heigit = 720;//縦

	////ウインドウクラスの設定
	//WNDCLASSEX w{};
	//w.cbSize = sizeof(WNDCLASSEX);
	//w.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロ―ジャーの設定
	//w.lpszClassName = L"DirectXGame";//ウインドウクラス名
	//w.hInstance = GetModuleHandle(nullptr);//ウインドウハンドル
	//w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	////ウインドウクラスをOSに登録する
	//RegisterClassEx(&w);
	////ウインドウサイズ｛X座標　Y座標　横幅　縦幅｝
	//RECT wrc = { 0,0,window_width,window_heigit };
	////自動サイズを補正する
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	////ウインドウオブジェクトの生成
	//HWND hwnd = CreateWindow(w.lpszClassName,//クラス名
	//	L"DirectXGame",//タイトルバーの文字
	//	WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
	//	CW_USEDEFAULT,//表示X座標（OSに任せる）
	//	CW_USEDEFAULT,//表示Y座標（OSに任せる）
	//	wrc.right - wrc.left,//ウインドウ横幅
	//	wrc.bottom - wrc.top,//ウインドウ縦幅
	//	nullptr,//親ウインドウハンドル
	//	nullptr,//メニューハンドル
	//	w.hInstance,//呼び出しアプリケーションハンドル
	//	nullptr);//オプション

	////ウインドウを表示状態にする
	//ShowWindow(hwnd, SW_SHOW);

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