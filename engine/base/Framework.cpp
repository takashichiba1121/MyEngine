#include "Framework.h"
#include"object3d.h"
#include"input.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"TextureManager.h"
#include"Sound.h"
#include"imguiManager.h"
#include<imgui.h>
#include"GameScene.h"
#include"assimpModel.h"
#include"assimpObject3d.h"
#include"LightGroup.h"
#include"ParticleManager.h"
#include"RenderTarget.h"
#include"PostEffectLuminance.h"
#include"PostEffectBlur.h"
#include"PostEffectMixeds.h"

using namespace MyEngine;

void Framework::Initialize()
{
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	dxCommon_ = std::make_unique<DirectXCommon>(); 
	dxCommon_->Initialize(winApp_.get());

	//DirectX初期化処理　ここから

	Input::Instance()->Initialize(winApp_.get());

	SpriteCommon::Instance()->Initialize(dxCommon_.get());

	TextureManager::Instance()->Initialize(dxCommon_.get());

	Sprite::StaticInitialize(dxCommon_.get());

	Model::SetDevice(dxCommon_->GetDevice());

	LightGroup::StaticInitialize(dxCommon_->GetDevice());

	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	AssimpModel::SetDevice(dxCommon_->GetDevice());

	assimpObject3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp_.get(), dxCommon_.get());

	ParticleManager::StaticInitialize(dxCommon_->GetDevice());

	PostEffectCommon::Instance()->Initialize(dxCommon_.get());

	RenderTarget::StaticInitialze(dxCommon_->GetDevice());

	PostEffectLuminance::Instance()->Initialize();

	PostEffectBlur::Instance()->Initialize();

	PostEffectMixeds::Instance()->Initialize();
}

void Framework::Finalize()
{
	SceneManager::Instance()->Finalize();

	sceneFactory_ = nullptr;

	Sound::StaticFinalize();

	PostEffectCommon::Instance()->Fin();

	PostEffectLuminance::Instance()->Fin();

	PostEffectBlur::Instance()->Fin();

	PostEffectMixeds::Instance()->Fin();

	TextureManager::Instance()->Finalize();

	imguiManager::Finalize();

	SpriteCommon::Instance()->Finalize();

	Object3d::Finalize();
	assimpObject3d::Finalize();

	ParticleManager::Finalize();

	dxCommon_ = nullptr;

	winApp_->Finalize();
}

void Framework::Update()
{
	endRequst_ = winApp_->ProcessMessage();

	//入力の更新
	Input::Instance()->Update();

	SceneManager::Instance()->Update();
}

void Framework::Run()
{
	Initialize();


	//ゲームループ
	while (true) {

		//×ボタンで終了メッセージがきたらゲームループを抜ける
		if (IsEndRequst()) {
			break;
		}
		//DirectX毎フレーム処理　ここから
		imguiManager imGui;

		imGui.Begin();
		Update();
		imGui.End();

		//描画コマンドここから
		Draw();

		//描画コマンドここまで

		//DirectX毎フレーム処理　ここまで

	}

	Finalize();
}
