#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"
#include"imguiManager.h"
#include"PostEffectLuminance.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::Instance()->SetSceneFactory(sceneFactory_.get());

	SceneManager::Instance()->ChangeScene("TITLE");

	renderTarget1 = std::make_unique<RenderTarget>();

	renderTarget1->Initialize();
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

	/*SceneManager::Instance()->Draw(dxCommon_.get());*/

	renderTarget1->PostDraw(dxCommon_->GetCommandList());

	Framework::GetDxCommon()->PreDraw();

	//PostEffectLuminance::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget1->GettexHandle());

	SceneManager::Instance()->Draw(dxCommon_.get());

#ifdef _DEBUG
	imguiManager::Draw();
#endif
	Framework::GetDxCommon()->PostDrow();

	
}
