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
#include"Light.h"

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

	Light::StaticInitialize(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	AssimpModel::SetDevice(dxCommon->GetDevice());

	assimpObject3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sound::StaticInitialize();

	imguiManager::StaticInitialize(winApp, dxCommon);

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	PostEffectCommon* postEffectCommon=new PostEffectCommon;

	postEffectCommon->StaticInitialize(dxCommon);

	PostEffectLuminance* postEffectLuminance = new PostEffectLuminance;

	postEffectLuminance->Initialize(postEffectCommon);

	PostEffectBlur* postEffectBlur = new PostEffectBlur;

	postEffectBlur->Initialize(postEffectCommon);

	PostEffectMixed* postEffectMixed = new PostEffectMixed;

	postEffectMixed->Initialize(postEffectCommon);

	PostEffectTest* postEffectTest = new PostEffectTest;

	postEffectTest->Initialize(postEffectCommon);

	PostEffectMultiRenderTarget* postEffectMultiRenderTarget = new PostEffectMultiRenderTarget;

	postEffectMultiRenderTarget->Initialize(postEffectCommon);

	GameScene* gameScene = new GameScene;
	gameScene->Initialize();

	bool frg=true;

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

		if (Input::TriggerKey(DIK_0)||Input::PadTriggerKey(XINPUT_GAMEPAD_A))
		{
			if (frg)
			{
				frg = false;
			}
			else
			{
				frg = true;
			}
		}

		//�`��R�}���h��������

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

		//imguiManager::Draw();
		dxCommon->PreDraw();
		//if (frg)
		//{
		//	postEffectTest->Draw();
		//}
		//else
		//{
		//	postEffectMixed->Draw(postEffectLuminance->GettextureHandle());
		//}

		gameScene->Draw(dxCommon);
		
		dxCommon->PostDrow();

		//�`��R�}���h�����܂�

		//DirectX���t���[�������@�����܂�

	}
	delete postEffectCommon;

	delete postEffectBlur;

	delete postEffectLuminance;

	delete postEffectMixed;

	delete postEffectTest;

	delete gameScene;

	imguiManager::Finalize();

	delete dxCommon;

	winApp->Finalize();

	delete winApp;

	return 0;
}