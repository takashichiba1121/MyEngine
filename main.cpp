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

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Texture::Initialize(dxCommon->GetDevice());

	Sprite::StaticInitialize(spriteCommon);

	Model::SetDevice(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_heigit);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp, dxCommon);

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

		//�`��R�}���h��������
		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCommandList());

		gameScene->Draw();

		Object3d::PostDraw();


		spriteCommon->PreDraw();

		gameScene->SpriteDraw();

		spriteCommon->PostDrow();

		//�`��R�}���h�����܂�

		imguiManager::Draw();

		dxCommon->PostDrow();

		//DirectX���t���[�������@�����܂�

	}

	delete gameScene;

	delete spriteCommon;

	imguiManager::Finalize();

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}