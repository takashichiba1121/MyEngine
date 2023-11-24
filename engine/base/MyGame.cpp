#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"
#include"imguiManager.h"
#include"PostEffectLuminance.h"
#include"PostEffectBlur.h"
#include"PostEffectMixeds.h"
#include"imguiManager.h"
#include<imgui.h>


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

	LuminanceColor = { 0.299f,0.587f,0.114f };
	smoothstepMax = 0.9f;
	smoothstepMin = 0.6f;

	sigma = 0.01f;
	stepWidth = 0.002f;
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

	ImGui::Begin("postEffect");

	ImGui::Text("Blur");
	ImGui::SliderFloat("sigma",&sigma,0.01f,0.5f,"%0.2f");
	ImGui::SliderFloat("stepWidth",&stepWidth,0.001f,0.01f,"%0.3f");

	ImGui::Text("Luminance");
	ImGui::SliderFloat("Max",&smoothstepMax,0.0f,1.0f,"%1.2f");
	ImGui::SliderFloat("Min",&smoothstepMin,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat3("color",&LuminanceColor.x,0.0f,1.0f,"%1.3f");

	ImGui::End();

	PostEffectLuminance::Instance()->Update(LuminanceColor,smoothstepMax,smoothstepMin);

	PostEffectBlur::Instance()->Update(sigma,stepWidth);
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
