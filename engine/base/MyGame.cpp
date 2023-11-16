#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"
#include"imguiManager.h"
#include"PostEffectLuminance.h"
#include"PostEffectBlur.h"
#include"PostEffectMixeds.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::Instance()->SetSceneFactory(sceneFactory_.get());

	SceneManager::Instance()->ChangeScene("TITLE");

	renderTarget1 = std::make_unique<RenderTarget>();

	renderTarget1->Initialize();

	renderTarget2 = std::make_unique<RenderTarget>();

	renderTarget2->Initialize();

	renderTarget3 = std::make_unique<RenderTarget>();

	renderTarget3->Initialize();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
} 

void MyGame::Draw()
{
	renderTarget1->PreDraw(dxCommon_->GetCommandList());

	SceneManager::Instance()->Draw(dxCommon_.get());

	renderTarget1->PostDraw(dxCommon_->GetCommandList());

	renderTarget2->PreDraw(dxCommon_->GetCommandList());

	PostEffectLuminance::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget1->GettexHandle());

	renderTarget2->PostDraw(dxCommon_->GetCommandList());

	renderTarget3->PreDraw(dxCommon_->GetCommandList());

	PostEffectBlur::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget2->GettexHandle());

	renderTarget3->PostDraw(dxCommon_->GetCommandList());

	Framework::GetDxCommon()->PreDraw();

	PostEffectMixeds::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget3->GettexHandle(),renderTarget1->GettexHandle());

	//SceneManager::Instance()->Draw(dxCommon_.get());

	SceneManager::Instance()->SpriteDraw();

#ifdef _DEBUG
	imguiManager::Draw();
#endif
	Framework::GetDxCommon()->PostDrow();

	
}
