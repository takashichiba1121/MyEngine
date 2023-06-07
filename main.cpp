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
#include"assimpModel.h"
#include"assimpObject3d.h"

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

	AssimpModel::SetDevice(dxCommon->GetDevice());

	assimpObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp, dxCommon);

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	PostEffectCommon* postEffectCommon=new PostEffectCommon;

	/*postEffectCommon->StaticInitialize(dxCommon);

	PostEffectLuminance* postEffectLuminance = new PostEffectLuminance;

	postEffectLuminance->Initialize(postEffectCommon);

	PostEffectBlurW* postEffectBlurW = new PostEffectBlurW;

	postEffectBlurW->Initialize(postEffectCommon);

	PostEffectBlurH* postEffectBlurH = new PostEffectBlurH;

	postEffectBlurH->Initialize(postEffectCommon);

	PostEffectMixed* postEffectMixed = new PostEffectMixed;

	postEffectMixed->Initialize(postEffectCommon);*/

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

		/*postEffectLuminance->PreDrawScene(dxCommon->GetCommandList());

		gameScene->PostEffectDraw(dxCommon);

		postEffectLuminance->PostDrawScene();

		postEffectBlurW->PreDrawScene(dxCommon->GetCommandList());

		postEffectLuminance->Draw();

		postEffectBlurW->PostDrawScene();

		postEffectBlurH->PreDrawScene(dxCommon->GetCommandList());

		postEffectBlurW->Draw();

		postEffectBlurH->PostDrawScene();

		postEffectMixed->PreDrawScene(dxCommon->GetCommandList());

		postEffectBlurH->Draw();

		postEffectMixed->PostDrawScene();*/

		//�`��R�}���h�����܂�

		//imguiManager::Draw();
		dxCommon->PreDraw();
		
		//postEffectMixed->Draw(postEffectLuminance->GettextureHandle());

		gameScene->Draw(dxCommon);
		
		dxCommon->PostDrow();

		//�`��R�}���h�����܂�

		//DirectX���t���[�������@�����܂�

	}
	delete postEffectCommon;

	//delete postEffectBlurW;

	//delete postEffectBlurH;

	//delete postEffectLuminance;

	//delete postEffectMixed;

	delete gameScene;

	imguiManager::Finalize();

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}