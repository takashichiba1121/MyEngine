#include "MyGame.h"
#include"TitileScene.h"
#include"SceneFactory.h"
#include"imguiManager.h"
#include"PostEffectLuminance.h"
#include"PostEffectBlur.h"
#include"PostEffectMixeds.h"
#include"imguiManager.h"
#include<imgui.h>
#include"input.h"


using namespace std;

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::Instance()->SetSceneFactory(sceneFactory_.get());

	SceneManager::Instance()->ChangeScene("TITLE");

	renderTarget1_ = std::make_unique<RenderTarget>();

	renderTarget1_->Initialize();

	renderTarget2_ = std::make_unique<RenderTarget>();

	renderTarget2_->Initialize();

	renderTarget3_ = std::make_unique<RenderTarget>();

	renderTarget3_->Initialize();

	LuminanceColor_ = { 0.299f,0.587f,0.114f };
	smoothstepMax_ = 0.9f;
	smoothstepMin_ = 0.6f;

	sigma_ = 0.005f;
	stepWidth_ = 0.001f;
}

void MyGame::Finalize()
{
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

#ifdef _DEBUG
	ImGui::Begin("postEffect");

	ImGui::Text("Blur");
	ImGui::SliderFloat("sigma",&sigma_,0.005f,0.5f,"%0.3f");
	ImGui::SliderFloat("stepWidth",&stepWidth_,0.001f,0.005f,"%0.3f");

	ImGui::Text("Luminance");
	ImGui::SliderFloat("Max",&smoothstepMax_,0.0f,1.0f,"%1.2f");
	ImGui::SliderFloat("Min",&smoothstepMin_,0.0f,1.0f,"%1.2f");

	ImGui::SliderFloat3("color",&LuminanceColor_.x,0.0f,1.0f,"%1.3f");

	ImGui::End();

	PostEffectLuminance::Instance()->Update(LuminanceColor_,smoothstepMax_,smoothstepMin_);

	PostEffectBlur::Instance()->Update(sigma_,stepWidth_);

	if (Input::Instance()->TriggerKey(DIK_1))
	{
		if (isPostEffect_ )
		{
			isPostEffect_ = false;
		}
		else
		{
			isPostEffect_ = true;
		}
	}
#endif
}

void MyGame::Draw()
{
	renderTarget1_->PreDraw(dxCommon_->GetCommandList());

	SceneManager::Instance()->Draw(dxCommon_.get());

	renderTarget1_->PostDraw(dxCommon_->GetCommandList());

	renderTarget2_->PreDraw(dxCommon_->GetCommandList());

	PostEffectLuminance::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget1_->GettexHandle());

	renderTarget2_->PostDraw(dxCommon_->GetCommandList());

	renderTarget3_->PreDraw(dxCommon_->GetCommandList());

	PostEffectBlur::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget2_->GettexHandle());

	renderTarget3_->PostDraw(dxCommon_->GetCommandList());

	Framework::GetDxCommon()->PreDraw();
	if ( isPostEffect_ )
	{
		PostEffectMixeds::Instance()->Draw(dxCommon_->GetCommandList(),renderTarget3_->GettexHandle(),renderTarget1_->GettexHandle());
	}
	else
	{

		SceneManager::Instance()->Draw(dxCommon_.get());
	}

	SceneManager::Instance()->SpriteDraw();

#ifdef _DEBUG
	imguiManager::Draw();
#endif
	Framework::GetDxCommon()->PostDrow();


}
