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

		gameScene->ObjectDraw();

		Object3d::PostDraw();


		spriteCommon->PreDraw();

		gameScene->SpriteDraw();

		spriteCommon->PostDrow();

		//�`��R�}���h�����܂�

		dxCommon->PostDrow();

		//DirectX���t���[�������@�����܂�

	}

	delete spriteCommon;

	delete gameScene;

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}