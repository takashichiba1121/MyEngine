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

//windows�A�v���̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = nullptr;
	winApp = new WinApp;
	winApp->Initialize();

	MSG msg{};

	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//DirectX�����������@��������

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

	//�Q�[�����[�v
	while (true) {

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (winApp->ProcessMessage()) {
			break;
		}
		//DirectX���t���[�������@��������

		//���͂̍X�V
		Input::Update();

		gameScene->Update();

		//postEffect->Update();

		//�`��R�}���h��������

		postEffect->PreDrawScene(dxCommon->GetCommandList());

		gameScene->PostEffectDraw(dxCommon);

		postEffect->PostDrawScene();

		//�`��R�}���h�����܂�

		//imguiManager::Draw();
		dxCommon->PreDraw();

		postEffect->Draw();

		gameScene->Draw(dxCommon);
		
		dxCommon->PostDrow();

		//�`��R�}���h�����܂�

		//DirectX���t���[�������@�����܂�

	}
	delete postEffect;

	delete gameScene;

	imguiManager::Finalize();

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}